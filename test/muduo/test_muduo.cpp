#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include <set>
#include <string>
#include "src/global_def.h"
class ChatServer {
public:
    // ChatServer的构造函数
    ChatServer(muduo::net::EventLoop* loop,
               const muduo::net::InetAddress& listenAddr);
    void start();

private:
    // 连接新的客户端的回调函数
    void onConnection(const muduo::net::TcpConnectionPtr& conn);
    // 接受消息的回调函数
    void onMessage(const muduo::net::TcpConnectionPtr& conn,
                   muduo::net::Buffer* buf,
                   muduo::Timestamp time);
    // 管理Server的组件
    muduo::net::TcpServer server_;
    // // 保存连接的客户端
    std::set<muduo::net::TcpConnectionPtr> connections_; // 保存连接的客户端
};

using namespace muduo;
using namespace muduo::net;

ChatServer::ChatServer(EventLoop* loop, const InetAddress& listenAddr)
        : server_(loop, listenAddr, "ChatServer") {
    server_.setConnectionCallback(
            std::bind(&ChatServer::onConnection, this, std::placeholders::_1));
    server_.setMessageCallback(
            std::bind(&ChatServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void ChatServer::start() {
    server_.start();
}

void ChatServer::onConnection(const TcpConnectionPtr& conn) {
    if (conn->connected()) {
        LOG_INFO << "New connection from " << conn->peerAddress().toIpPort();
        connections_.insert(conn);
    } else {
        LOG_INFO << "Connection from " << conn->peerAddress().toIpPort() << " closed";
        connections_.erase(conn);
    }
}

void ChatServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time) {
    std::string data = buf->retrieveAsString(10);  // 读取前10个字节，并将其转为字符串
    LOG_INFO << "Received 10: " << data;
}

int main()
{
    int port = 8888;
    muduo::net::InetAddress listenAddr(port);	// 监听Addr
    muduo::net::EventLoop loop;

    ChatServer server(&loop, listenAddr);
    server.start();
    loop.loop();
    return 0;
}
