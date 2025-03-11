
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>

#define BUF_SIZE 4096
#define SOCKS_VERSION 5

// 握手请求的结构
struct Socks5HandshakeRequest {
    uint8_t version;
    uint8_t nmethods;
    uint8_t methods[0];  // 可变长度
};

// 握手响应的结构
struct Socks5HandshakeResponse {
    uint8_t version;
    uint8_t method;
};

// 请求结构
struct Socks5Request {
    uint8_t version;
    uint8_t cmd;
    uint8_t reserved;
    uint8_t address_type;
    uint8_t address[256];
    uint16_t port;
};

// 客户端请求的命令类型
enum CommandType {
    CONNECT = 0x01,
    BIND = 0x02,
    UDP_ASSOCIATE = 0x03
};

// SOCKS5 代理响应的结构
struct Socks5Response {
    uint8_t version;
    uint8_t reply;
    uint8_t reserved;
    uint8_t address_type;
    uint8_t address[256];
    uint16_t port;
};

// 处理客户端连接
void handle_client(int client_sock) {
    // 1. 握手阶段
    uint8_t buffer[BUF_SIZE];
    ssize_t read_size = read(client_sock, buffer, sizeof(buffer));
    if (read_size < 0) {
        perror("Read error during handshake");
        close(client_sock);
        return;
    }

    // SOCKS5 握手请求（仅支持无认证）
    if (buffer[0] != SOCKS_VERSION) {
        std::cerr << "Unsupported SOCKS version" << std::endl;
        close(client_sock);
        return;
    }

    uint8_t nmethods = buffer[1];
    uint8_t methods[nmethods];
    memcpy(methods, buffer + 2, nmethods);

    // 回复握手响应，选择不需要认证
    Socks5HandshakeResponse handshake_response = {SOCKS_VERSION, 0};  // 0 表示无认证
    write(client_sock, &handshake_response, sizeof(handshake_response));

    // 2. 请求阶段
    read_size = read(client_sock, buffer, sizeof(buffer));
    if (read_size < 0) {
        perror("Read error during request");
        close(client_sock);
        return;
    }

    // 解析 SOCKS5 请求
    Socks5Request* request = reinterpret_cast<Socks5Request*>(buffer);
    if (request->version != SOCKS_VERSION || request->cmd != CONNECT) {
        std::cerr << "Unsupported SOCKS version or command" << std::endl;
        close(client_sock);
        return;
    }

    // 处理目标地址
    std::string target_ip;
    uint16_t target_port = ntohs(request->port);

    if (request->address_type == 0x01) {  // IPv4 地址
        struct in_addr addr;
        memcpy(&addr, request->address, 4);
        target_ip = inet_ntoa(addr);
    } else if (request->address_type == 0x03) {  // 域名地址
        target_ip = std::string(reinterpret_cast<char*>(request->address), buffer[4]);
    } else {
        std::cerr << "Unsupported address type" << std::endl;
        close(client_sock);
        return;
    }

    std::cout << "Connecting to target " << target_ip << ":" << target_port << std::endl;

    // 3. 连接到目标服务器
    int target_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (target_sock < 0) {
        perror("Failed to create socket to target");
        close(client_sock);
        return;
    }

    struct sockaddr_in target_addr;
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(target_port);
    target_addr.sin_addr.s_addr = inet_addr(target_ip.c_str());

    if (connect(target_sock, (struct sockaddr*)&target_addr, sizeof(target_addr)) < 0) {
        perror("Failed to connect to target");
        close(target_sock);
        close(client_sock);
        return;
    }

    // 4. 代理连接成功，回复客户端
    Socks5Response response = {SOCKS_VERSION, 0x00, 0x00, 0x01};  // 请求成功，使用 IPv4 地址
    write(client_sock, &response, sizeof(response));

    // 5. 数据转发阶段
    fd_set read_fds;
    while (true) {
        FD_ZERO(&read_fds);
        FD_SET(client_sock, &read_fds);
        FD_SET(target_sock, &read_fds);

        int max_fd = std::max(client_sock, target_sock);

        int activity = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);
        if (activity < 0) {
            perror("Select error");
            break;
        }

        if (FD_ISSET(client_sock, &read_fds)) {
            read_size = read(client_sock, buffer, sizeof(buffer));
            if (read_size <= 0) break;
            write(target_sock, buffer, read_size);
        }

        if (FD_ISSET(target_sock, &read_fds)) {
            read_size = read(target_sock, buffer, sizeof(buffer));
            if (read_size <= 0) break;
            write(client_sock, buffer, read_size);
        }
    }

    // 关闭连接
    close(client_sock);
    close(target_sock);
}

int main() {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Failed to create server socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1080);  // SOCKS5 默认端口
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_sock);
        return 1;
    }

    if (listen(server_sock, 5) < 0) {
        perror("Listen failed");
        close(server_sock);
        return 1;
    }

    std::cout << "SOCKS5 Proxy Server listening on port 1080..." << std::endl;

    while (true) {
        int client_sock = accept(server_sock, nullptr, nullptr);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        // 处理每个客户端连接
        handle_client(client_sock);
    }

    close(server_sock);
    return 0;
}
