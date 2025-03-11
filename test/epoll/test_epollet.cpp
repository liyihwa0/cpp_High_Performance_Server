/*
 * 针对epollet的测试
 * 1. epollet会在每次有新数据到达时就通知,而不是从无数据到有数据的瞬间通知
 * 2. 在fd有数据时,再通过et模式添加socket,会导致立即触发一次, 允许数据先到达fd,程序再将socket添加给et
 */


#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <thread>
#include <atomic>
#include <fcntl.h>

#define PORT 8888
#define MAX_EVENTS 10

std::atomic<bool> server_running(true);

void server_thread() {
    // 创建socket
    int server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create server socket!" << std::endl;
        return;
    }

    // 配置服务器地址
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Failed to bind server socket!" << std::endl;
        close(server_fd);
        return;
    }

    // 监听
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Failed to listen on server socket!" << std::endl;
        close(server_fd);
        return;
    }

    // 创建epoll实例
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Failed to create epoll instance!" << std::endl;
        close(server_fd);
        return;
    }

    // 将server_fd添加到epoll
    struct epoll_event event;
    event.events = EPOLLIN ;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        std::cerr << "Failed to add server socket to epoll!" << std::endl;
        close(server_fd);
        close(epoll_fd);
        return;
    }

    struct epoll_event events[MAX_EVENTS];
    while (server_running) {
        // 等待事件
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, 0);
        if (num_events == -1) {
            std::cerr << "Epoll wait error!" << std::endl;
            break;
        }

        // 处理每个事件
        for (int i = 0; i < num_events; ++i) {
            if (events[i].data.fd == server_fd) {
                // 处理新连接
                int new_socket = accept(server_fd, NULL, NULL);
                if (new_socket == -1) {
                    std::cerr << "Failed to accept connection!" << std::endl;
                    continue;
                }

                // 设置新连接为非阻塞模式
                fcntl(new_socket, F_SETFL, O_NONBLOCK);

                // 添加新连接到epoll
                event.events = EPOLLIN | EPOLLET ;
                event.data.fd = new_socket;
                sleep(60);
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
                    std::cerr << "Failed to add new socket to epoll!" << std::endl;
                    close(new_socket);
                }
            } else {
                std::cout<<"epoll event trigger"<<std::endl;
//                char buffer[1024];
//                // 数据可读
//                // 读取一部分出来
//                // 数据可读
//                int client_fd = events[i].data.fd;
//                ssize_t bytes_read = recv(client_fd, buffer, 10, 0);
//                if (bytes_read == -1) {
//                    std::cerr << "Failed to read from socket!" << std::endl;
//                } else if (bytes_read == 0) {
//                    // 客户端关闭了连接
//                    std::cout << "Client closed the connection." << std::endl;
//                    close(client_fd);
//                } else {
//                    // 成功读取数据
//                    std::cout << "Server: Read " << bytes_read << " bytes from client." << std::endl;
//                    std::cout << "Data: " << std::string(buffer, bytes_read) << std::endl;
//                }
            }
        }
    }

    // 清理
    close(server_fd);
    close(epoll_fd);
}

void client_thread() {
    // 创建socket
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        std::cerr << "Failed to create client socket!" << std::endl;
        return;
    }

    // 配置服务器地址
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // 连接到服务器
    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "  to connect to server!" << std::endl;
        close(client_fd);
        return;
    }

    // 每秒发送数据
    char message[] = "Hello, Server!";
    int times=5;
    while (times--) {
        ssize_t bytes_sent = send(client_fd, message, sizeof(message), 0);
        if (bytes_sent == -1) {
            std::cerr << "Failed to send data!" << std::endl;
        } else {
            std::cout << "Client: Sent message to server." << std::endl;
        }
        sleep(1);
    }

    // 清理
    close(client_fd);
}

int main() {
    // 启动服务器线程
    std::thread server(server_thread);

    // 启动客户端线程
    std::thread client(client_thread);

    // 运行10秒，后停止服务器和客户端
    sleep(10);
    server_running = false;

    // 等待线程结束
    server.join();
    client.join();

    return 0;
}
