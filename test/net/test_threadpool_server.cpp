#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
 #include <sys/epoll.h>
#include <thread>
#include <cstring>
#include <netinet/in.h>
#include <fcntl.h>
#include <vector>
#include "src/thread/thread_pool.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
wa::ThreadPool pool(2000, 2000);
class HttpServer {
public:
    HttpServer(int port);
    ~HttpServer();
    void start();

private:
    int server_fd;
    int epoll_fd;
    sockaddr_in server_addr;
    std::vector<std::thread> threads;

    void handle_connection(int client_fd);
    void run_epoll();
};

HttpServer::HttpServer(int port) {
    // 创建服务器套接字
    server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (server_fd == -1) {
        std::cerr << "Socket creation failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // 绑定服务器套接字
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Bind failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 监听客户端连接
    if (listen(server_fd, 10) == -1) {
        std::cerr << "Listen failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 创建 epoll 实例
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Epoll create failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

HttpServer::~HttpServer() {
    close(server_fd);
    close(epoll_fd);
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void HttpServer::handle_connection(int client_fd) {
    char buffer[1024];
    ssize_t len = 0;

    // 接收数据
    while ((len = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
        buffer[len] = '\0';  // 添加字符串结束符

        // 处理接收到的 HTTP 请求，这里简化为打印收到的内容
        //std::cout << "Received request: " << buffer << std::endl;

        // 发送 HTTP 响应
        const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!";
        send(client_fd, response, strlen(response), 0);

        close(client_fd);  // 关闭文件描述符
        break;
    }
}



void HttpServer::run_epoll() {
    struct epoll_event event, events[10];
    event.events = EPOLLIN;
    event.data.fd = server_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        std::cerr << "Epoll ctl failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    while (true) {
        int num_events = epoll_wait(epoll_fd, events, 10, -1);
        for (int i = 0; i < num_events; ++i) {
            if (events[i].data.fd == server_fd) {
                // 接受新连接
                int client_fd = accept(server_fd, nullptr, nullptr);
                if (client_fd == -1) {
                    std::cerr << "Accept failed!" << std::endl;
                    continue;
                }

                // 创建新线程处理这个连接
                pool.add([&]{
                    handle_connection(client_fd);
                });
            }
        }
    }
}

void HttpServer::start() {
    run_epoll();
}

int main() {
    HttpServer server(8888);

    server.start();
    return 0;
}
