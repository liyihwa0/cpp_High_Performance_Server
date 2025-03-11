
#include "src/net/epoll.h"
#include "src/thread/thread.h"
#include <netinet/in.h>
#include "iostream"
using namespace std;
#define PORT 8888
using namespace wa;
int main(){
    // 创建 socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("TcpServer creation failed");
        return 1;
    }

    // 设置地址复用选项
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt failed");
        close(server_fd);
        return 1;
    }
    // 设置 socket 地址结构
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 绑定套接字
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }

    // 开始监听
    if (listen(server_fd, 5) == -1) {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }

    cout << "Server is listening on port " << PORT << "...\n";

    // 只接收一个客户端连接
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1) {
        perror("Accept failed");
        close(server_fd);
        return 1;
    }
    cout << "Accepted connection, handling client...\n";



    EpollEvent event;
    event.events=EPOLLIN | EPOLLET | EPOLLONESHOT ;
    event.data.fd=client_fd;
    Epoll epoll;
    epoll.addEvent(client_fd,event);
    while(true){
        Int ok=epoll.wait();
        auto events=epoll.events();
        for(int i=0;i<ok;i++){
            auto event=events[i];
            if (event.events & EPOLLIN) {  // 如果是可读事件
                // 获取触发事件的客户端文件描述符
                int client_fd = event.data.fd;
                char buffer[16];

                // 读取数据
                ssize_t bytesRead=0;
                while(true){
                    bytesRead=read(client_fd,buffer,sizeof(buffer));
                    if (bytesRead == -1) {
                        std::cerr << "read error: " << strerror(errno) << std::endl;
                        // 处理错误
                    } else if (bytesRead == 0) {
                        std::cout << "Client closed connection" << std::endl;
                        close(client_fd);  // 客户端关闭连接
                        return 0;
                    } else {
                        // 处理读取的数据
                        buffer[bytesRead] = '\0';  // 确保数据是以'\0'结尾的字符串
                        std::cout << "Received data: " << buffer << std::endl;
                        // 进一步处理数据
                        break;
                    }
                    cout<<"主动关闭"<<endl;
                    close(client_fd);
                    epoll.delEvent(client_fd);
                }
                std::cout << "Received down"  << std::endl;
                epoll.addEvent(client_fd,event);
            }
        }
    }



}