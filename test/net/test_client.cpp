#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <chrono>
#include <thread>

int main() {
    try {
        // 创建套接字
        int clientFd = socket(AF_INET, SOCK_STREAM, 0);
        if (clientFd == -1) {
            throw std::runtime_error("Failed to create socket");
        }

        // 设置服务器地址
        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(8888);
        if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
            throw std::runtime_error("Invalid address");
        }

        // 连接到服务器
        if (connect(clientFd, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
            throw std::runtime_error("Connection failed");
        }
        int len=0;
        int times=5;
        char buffer[1024];  // 用于接收服务器响应
        // 主循环，每隔1秒发送文件内容
        while (true) {
            sleep(2);
            // 读取文件并将内容转换为字符串
            std::string content="GET /index.html HTTP/1.1\r\n"
                                "Host: www.example.com\r\n"
                                "User-Agent: HttpRequest/1.0\r\n\r\n";

            // 发送请求内容
            ssize_t bytesSent = write(clientFd, content.c_str(), content.size());
            if (bytesSent == -1) {
                throw std::runtime_error("Failed to send message");
            }
            len += content.length();

            std::cout << len << ": Sent " << bytesSent << " bytes to server." << std::endl;

//            // 读取服务器响应
//            ssize_t bytesRead = read(clientFd, buffer, sizeof(buffer) - 1);  // 保证有空间存储字符串结尾'\0'
//            if (bytesRead == -1) {
//                throw std::runtime_error("Failed to read from server");
//            } else if (bytesRead == 0) {
//                std::cout << "Server closed connection." << std::endl;
//                break;  // 服务器关闭连接
//            }

//            std::cout << "Received " << bytesRead << " bytes from server: " << std::endl;

        }

        // 关闭连接
        close(clientFd);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
