#include "src/fiber/fiber.h"
#include "src/logger/logger.h"
#include <netinet/in.h>
#define PORT 8888
using namespace std;
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
    Fiber::AddTask([&client_fd]{
        EpollEvent event;
        event.events = EPOLLIN  | EPOLLET | EPOLLONESHOT;
        // 不断从fd中读数据,并将结果写入bufferChannel
        Helper::SetNonBlocking(client_fd);
        char buffer[1024];  // 你可以调整缓冲区大小
        int len=0;
        while(TRUE){
            Fiber::AwaitEvent(client_fd, event);
            // 不断读取数据直到 fd 没有更多数据
            ssize_t bytesRead;
            while ((bytesRead = read(client_fd, buffer, sizeof(buffer))) > 0) {
                // 将数据打印到控制台
                len++;
                std::ostringstream oss;
                oss.write(buffer, bytesRead);
                len+=oss.str().length();

                LOG_INFO(gl)<<len<<std::endl;
            }

            // 如果读取的字节数为 0，表示没有更多数据
            if (bytesRead == 0) {
                std::cout << "\nNo more data, closing fd." << std::endl;
                break; // 可以在这里选择退出循环，或者关闭连接
            }

            // 如果发生了错误，打印错误并退出循环
            if (bytesRead < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // 可能是非阻塞模式下暂时没有数据可读，继续等待
                    continue;
                } else {
                    std::cerr << "Read error: " << strerror(errno) << std::endl;
                    break;
                }
            }
        }
    });
    Fiber::Start(2);
}