
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

#define MAX_EVENTS 10
#define PORT 4433

// 初始化 OpenSSL
void init_openssl() {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}

// 创建 SSL 上下文
SSL_CTX* create_context() {
    const SSL_METHOD* method = TLS_server_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        std::cerr << "Unable to create SSL context\n";
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    return ctx;
}

// 加载 SSL 证书和私钥
void configure_ssl(SSL_CTX* ctx) {
    if (SSL_CTX_use_certificate_file(ctx, "/root/server.crt", SSL_FILETYPE_PEM) <= 0) {
        std::cerr << "Unable to load certificate\n";
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "/root/server.key", SSL_FILETYPE_PEM) <= 0) {
        std::cerr << "Unable to load private key\n";
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    if (!SSL_CTX_check_private_key(ctx)) {
        std::cerr << "Private key does not match the public certificate\n";
        exit(1);
    }
}

// 将套接字设置为非阻塞
void set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

// 处理 HTTP 请求并返回响应
void handle_https_request(SSL* ssl) {
    char buffer[1024];
    int bytes_read = SSL_read(ssl, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        std::cout << "Received request:\n" << buffer << std::endl;

        // 简单的 HTTP 响应
        const char* response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 13\r\n"
                "\r\n"
                "Hello, HTTPS!";
        SSL_write(ssl, response, strlen(response));
    }
}

// 主逻辑
int main() {
    // 初始化 OpenSSL
    init_openssl();
    SSL_CTX* ctx = create_context();
    configure_ssl(ctx);

    // 创建监听套接字
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(1);
    }

    set_nonblocking(server_fd);

    // 创建 epoll 实例
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(1);
    }

    // 将 server_fd 添加到 epoll 中监听事件
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) < 0) {
        perror("epoll_ctl");
        exit(1);
    }

    struct epoll_event events[MAX_EVENTS];

    while (true) {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (n == -1) {
            perror("epoll_wait");
            exit(1);
        }

        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == server_fd) {
                // 处理新的客户端连接
                int client_fd = accept(server_fd, nullptr, nullptr);
                if (client_fd == -1) {
                    perror("accept");
                    continue;
                }

                set_nonblocking(client_fd);

                SSL* ssl = SSL_new(ctx);
                SSL_set_fd(ssl, client_fd);

                // 启动 SSL 握手
                int ret = SSL_accept(ssl);
                if (ret <= 0) {
                    int ssl_err = SSL_get_error(ssl, ret);
                    if (ssl_err == SSL_ERROR_WANT_READ || ssl_err == SSL_ERROR_WANT_WRITE) {
                        // 等待 epoll 事件触发
                        ev.events = EPOLLIN | EPOLLOUT;
                        ev.data.fd = client_fd;
                        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
                    } else {
                        // 错误处理
                        std::cerr << "SSL_accept failed\n";
                        ERR_print_errors_fp(stderr);
                        SSL_free(ssl);
                        close(client_fd);
                    }
                }
            } else {
                // 处理已连接的客户端
                int client_fd = events[i].data.fd;
                SSL* ssl = SSL_get_ex_data(SSL_(), 0);  // 获取 SSL 对象

                if (events[i].events & EPOLLIN) {
                    // 读取数据
                    handle_https_request(ssl);
                }

                if (events[i].events & EPOLLOUT) {
                    // SSL_write 会处理写入数据
                    SSL_shutdown(ssl);
                    SSL_free(ssl);
                    close(client_fd);
                }
            }
        }
    }

    // 清理
    close(epoll_fd);
    close(server_fd);
    SSL_CTX_free(ctx);
    return 0;
}
