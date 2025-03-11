#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>

#define BUF_SIZE 4096
#define PORT 8080  // 代理服务器监听的端口

// 处理客户端连接的函数
void handle_client(int client_sock) {
    char buffer[BUF_SIZE];
    ssize_t read_size;

    // 读取客户端请求
    read_size = read(client_sock, buffer, sizeof(buffer));
    if (read_size <= 0) {
        perror("Failed to read client request");
        close(client_sock);
        return;
    }

    // 输出收到的报文内容
    std::cout << "Received request:" << std::endl;
    std::cout.write(buffer, read_size);  // 输出读取的字节数据
    std::cout << std::endl;

    // 假设客户端请求是标准的 HTTP GET 请求
    // 提取目标主机名和端口（这里只处理 GET 请求，其他方法如 POST 需要根据需要扩展）
    std::string request(buffer, read_size);
    size_t host_pos = request.find("Host: ");
    if (host_pos == std::string::npos) {
        std::cerr << "No Host header found" << std::endl;
        close(client_sock);
        return;
    }

    size_t host_end = request.find("\r\n", host_pos);
    std::string host = request.substr(host_pos + 6, host_end - host_pos - 6);

    // 获取目标主机地址
    struct hostent* he = gethostbyname(host.c_str());
    if (!he) {
        std::cerr << "Failed to resolve host" << std::endl;
        close(client_sock);
        return;
    }

    // 连接到目标服务器
    int target_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (target_sock < 0) {
        perror("Failed to create socket to target");
        close(client_sock);
        return;
    }

    struct sockaddr_in target_addr;
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(80);  // 默认 HTTP 端口
    memcpy(&target_addr.sin_addr.s_addr, he->h_addr_list[0], he->h_length);

    if (connect(target_sock, (struct sockaddr*)&target_addr, sizeof(target_addr)) < 0) {
        perror("Failed to connect to target");
        close(client_sock);
        close(target_sock);
        return;
    }

    // 将客户端请求转发给目标服务器
    write(target_sock, buffer, read_size);

    // 读取目标服务器的响应
    while ((read_size = read(target_sock, buffer, sizeof(buffer))) > 0) {
        // 将目标服务器的响应转发回客户端
        write(client_sock, buffer, read_size);
    }

    // 关闭连接
    close(target_sock);
    close(client_sock);
}

int main() {
    // 创建监听套接字
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Failed to create server socket");
        return 1;
    }

    // 设置服务器地址
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 绑定套接字
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_sock);
        return 1;
    }

    // 开始监听
    if (listen(server_sock, 5) < 0) {
        perror("Listen failed");
        close(server_sock);
        return 1;
    }

    std::cout << "HTTP Proxy Server listening on port " << PORT << "..." << std::endl;

    // 接受客户端连接
    while (true) {
        int client_sock = accept(server_sock, nullptr, nullptr);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        // 处理客户端请求
        handle_client(client_sock);
    }

    // 关闭服务器套接字
    close(server_sock);
    return 0;
}
