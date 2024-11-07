#ifndef CONNECTION_H
#define CONNECTION_H

#include <arpa/inet.h>
#include <sys/epoll.h>
#include "epoll.h"
#include <sys/socket.h>  // 套接字 API，例如 socket(), bind(), connect(), listen(), accept() 等
#include <unistd.h>
#include <iostream>
#include <string>

#include "global_def.h"
#include <fcntl.h>

void TestRead(FileDescriptor fd){
    U8 arr[1024];
    Size byteSize=0;
    Size sizeAll=0;
    while (true) {
        byteSize = read(fd, arr, 1024);

        if(byteSize==-1){ //读出现错误
            if(errno==EAGAIN||errno==EWOULDBLOCK){ //没有数据
                break; 
            }
        }else if(byteSize==0){
            std::cout<<"Client closed connection"<<std::endl;
            return ;    //对方关闭了链接
        }
        sizeAll+=byteSize;
    }
    std::cout<<"Read from client: " <<sizeAll<<" bytes"<<std::endl;
}

void TestWrite(FileDescriptor fd){
    
    // 准备要发送的大数据
    std::string response = "This is a large amount of data from server. " + std::string(500, 'A');
    send(fd, response.c_str(), response.size(), 0);
    std::cout << "Send to client" << std::endl;

}

class ConnectionManager {
   private:
    Epoll epoll;
    FileDescriptor listenFd;  // 用于监听链接的fd
    EpollEvent *events;
    Size maxEventSize;
    Size currentConnectSize;  // 记录当前与多少个用户建立了链接

    void addEvent(FileDescriptor fd){
        EpollEvent event;
        event.data.fd = fd;
        event.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLONESHOT ;  // 等待可读事件 | 可写事件 | 边缘触发 | 只触发一次 
        this->epoll.addEvent(event);
        
        int flag=fcntl(fd,F_GETFL);
        fcntl(fd,F_SETFL,flag|O_NONBLOCK);  // 设置不阻塞
    }

    void delEvent(FileDescriptor fd){
        this->epoll.delEvent(fd);
    }


   public:
    ConnectionManager(std::string ip, U16 port) {
        this->listenFd = socket(PF_INET,      // ipv4
                                SOCK_STREAM,  // tcp
                                0);

        // 地址复用,方便快速重启
        int opt = 1;
        setsockopt(this->listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        // 绑定
        SockAddrV4 address;
        address.sin_family = AF_INET;          // ipv4协议栈
        address.sin_addr.s_addr = INADDR_ANY;  // 任意网卡的报文都接收
        address.sin_port = htons(port);

        // 监听
        if (bind(this->listenFd, (SockAddr *)&address, sizeof(address)) == -1) {
            throw std::exception();
        }

        // 监听
        if (listen(this->listenFd, 5) == -1) {
            throw std::exception();
        }


        EpollEvent ev;
        ev.data.fd = this->listenFd;
        ev.events = EPOLLIN | EPOLLONESHOT;

        epoll.addEvent(ev);

        this->maxEventSize = 2048;
        this->currentConnectSize=1;
        this->events=new EpollEvent[2048];
    }

    ~ConnectionManager() {
        close(this->listenFd);
    }


    void start() {
        while (true) {
            int num = this->epoll.wait(this->events, this->maxEventSize);
            if ((num < 0) && (errno != EINTR)) {  // 忽略 被打断信号
                break;
            }

            for (int i = 0; i < num; i++) {
                FileDescriptor fd = this->events[i].data.fd;
                if (fd == this->listenFd) {
                    SockAddrV4 clientAddr;
                    SockLen len = SizeOf(clientAddr);
                    FileDescriptor connFd = accept(this->listenFd, (SockAddr *)&clientAddr, &len);

                    if (this->currentConnectSize >= this->maxEventSize) {
                        // todo: 给客户端写一个服务器正忙的信息
                        close(connFd);
                        continue;
                    }

                    this->addEvent(connFd); // 交给epoll去监听
                    this->currentConnectSize++; // 用户数++
                }else if(events[i].events&(EPOLLRDHUP | EPOLLHUP | EPOLLERR)){ 
                    //处理异常情况
                    this->delEvent(events[i].data.fd);
                    close(events[i].data.fd);
                    this->currentConnectSize--;
                }else if(events[i].events&EPOLLIN){ //有读事件
                    TestRead(events[i].data.fd);
                }else if(events[i].events&EPOLLOUT){ //有写事件
                    TestWrite(events[i].data.fd);
                } 
            }
        }
    }
};


#endif