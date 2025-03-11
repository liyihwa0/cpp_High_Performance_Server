#include <netinet/in.h>
#include "src/fiber/channel.h"
#include "src/fiber/fiber.h"
#include "iostream"
#include "src/http/http_common.h"

using namespace std;
using namespace wa;
using namespace wa::http;
#define PORT 8888

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

    SP<Channel<UP<Buffer>> > bfChannel(new Channel<UP<Buffer>>(10));

    FdBufferStore store(client_fd,2,8);
    Fiber::AddTask([&store]{
        store.run();
    },"bf store");
    Fiber::AddTask([&store]{
        while(true){
            UP<Buffer> bf=store.consumeBuffer();
            String s;
            while(bf->hasMore()){
                s.push_back(bf->pop());
            }
            std::cout<<s<<endl;
            store.recycleBuffer(bf);
        }
    },"consumer");

    Fiber::Start(1);
}