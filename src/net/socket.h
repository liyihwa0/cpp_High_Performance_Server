
#pragma once
#include "src/global_def.h"
#include "src/exception/exception.h"
#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/epoll.h>
#include "src/logger/logger.h"
#include "src/fiber/channel.h"
namespace wa {



    class ListenerSocket;
    class SslListenerSocket;
    class ConnectionSocket{
        friend ListenerSocket;
        friend SslListenerSocket;

    protected:
        FileDescriptor connectionFd_;
        SockAddrV4 addr_;
        Bool writeClosed_;
        Bool readClosed_;
        SpinLock lock_;
        ConnectionSocket(FileDescriptor fd,SockAddrV4 addr):connectionFd_(fd),addr_(addr),writeClosed_(FALSE),readClosed_(FALSE){
            Helper::SetNonBlocking(connectionFd_);
        }
    public:
        ConnectionSocket():connectionFd_(-1),addr_(),writeClosed_(FALSE),readClosed_(FALSE){}

        ~ConnectionSocket() {
            close();
        }

        Bool connect(SockAddrV4 addr) {
            if (connectionFd_ != -1) {
                // 已经连接
                return FALSE;
            }

            connectionFd_ = socket(AF_INET, SOCK_STREAM, 0);
            if (connectionFd_ == -1) {
                throw ERRNO_EXCEPTION(); // 创建套接字失败
            }

            addr_ = addr;
            Helper::SetNonBlocking(connectionFd_);

            int result = ::connect(connectionFd_, (struct sockaddr*)&addr_, sizeof(addr_));
            if (result == -1) {
                if (errno == EINPROGRESS) {
                    // 连接正在进行中，使用非阻塞方式
                    return TRUE; // 连接成功
                } else {
                    // 连接失败
                    ::close(connectionFd_);
                    connectionFd_ = -1;
                    throw ERRNO_EXCEPTION();
                }
            }

            return TRUE; // 连接成功
        }

        Bool connect(const String& ip, Int port) {
            if (connectionFd_ != -1) {
                // 已经连接
                return FALSE;
            }

            // 设置SockAddrV4结构体
            SockAddrV4 addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);

            // 将IP字符串转换为网络字节序的二进制形式
            int result = inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
            if (result <= 0) {
                // IP地址格式错误
                throw std::invalid_argument("Invalid IP address format");
            }

            // 调用原始connect方法
            return connect(addr);
        }

        virtual Bool connected(){
            return connectionFd_!=-1;
        }

        // 封装了send,异步的写入
        Bool send(Buffer& buffer){
            if(writeClosed_){
                return -1;
            }
            while(buffer.hasMore()){
                Int len=::send(connectionFd_,buffer.readableAddr(),buffer.readableLength(),0);
                if(len>0){
                    buffer.advanceRead(len);
                }else if(len==0){ //写端被关闭,我们再也不能写入了
                    goto writeClose;
                }else{
                    if (errno == EAGAIN||errno==EWOULDBLOCK){ //异步写入时,fd还没准备好
                        Fiber::AwaitEvent(connectionFd_,EPOLLOUT);
                    }else if(errno==EPIPE){ //fd已关闭
                        goto writeClose;
                    }else{
                        throw ERRNO_EXCEPTION(); //其他错误情况
                    }
                }
            }
            return TRUE;

            writeClose:
            closeWrite();
            return FALSE;
        }

        // 封装了recv,异步的读取
        Bool recv(Buffer& buffer){
            if(readClosed_){
                return FALSE;
            }
            while(TRUE){
                Int len=::recv(connectionFd_, buffer.writeableAddr(), buffer.writeableLength(), 0);
                if(len>0){
                    buffer.advanceWrite(len);
                    return TRUE;
                }else if(len==0){ //读端被关闭,有可能是因为对方已经写完
                    goto closeRead;
                }else{
                    if (errno == EAGAIN||errno==EWOULDBLOCK){ //异步写入时,fd还没准备好
                        Fiber::AwaitEvent(connectionFd_,EPOLLIN);
                    }else if(errno==EPIPE || errno==ECONNRESET){ //fd已关闭
                        goto closeRead;
                    }else{
                        throw ERRNO_EXCEPTION(); //其他错误情况
                    }
                }
            }
            closeRead:

            closeRead();
            return FALSE;
        }

        SockAddrV4 getAddr(){
            return addr_;
        }

        virtual // closeRead表示不再读取了
        void closeRead(){
            auto lock= AsScopedLock(lock_);
            if(readClosed_) return;
            readClosed_=TRUE;
            shutdown(connectionFd_,SHUT_RD);
            Fiber::DeleteEvent(connectionFd_,EPOLLIN);
            if(writeClosed_){
                LOG_DEBUG(gl)<<"connection "<<connectionFd_<<" closed";
                ::close(connectionFd_);
                connectionFd_=-1;
            }
        }

        virtual // closeWrite表示不再写入了
        void closeWrite(){
            auto lock= AsScopedLock(lock_);
            if(writeClosed_) return;
            writeClosed_=TRUE;
            shutdown(connectionFd_,SHUT_WR);
            Fiber::DeleteEvent(connectionFd_,EPOLLOUT);
            if(readClosed_){
                LOG_DEBUG(gl)<<"connection "<<connectionFd_<<" closed";
                ::close(connectionFd_);
                connectionFd_=-1;
            }
        }

        void close(){
            auto lock= AsScopedLock(lock_);
            if(connectionFd_==-1) return;
            if(!readClosed_){
                Fiber::DeleteEvent(connectionFd_,EPOLLIN);
                readClosed_=TRUE;
            }
            if(!writeClosed_){
                Fiber::DeleteEvent(connectionFd_,EPOLLOUT);
                writeClosed_=TRUE;
            }

            LOG_DEBUG(gl)<<"connection "<<connectionFd_<<" closed";
            connectionFd_=-1;
            ::close(connectionFd_);
        }

        Bool writeClosed() const {
            return writeClosed_;
        }

        Bool readClosed() const{
            return readClosed_;
        }

        Bool closed() const{
            return readClosed_&&writeClosed_;
        }

        FileDescriptor fd() const{
            return connectionFd_;
        }
    };

    template <typename T>
    class SslCtxDeleter {
    public:
        static void DoDelete(T* ptr) {
            if (ptr) {
                SSL_CTX_free(ptr);  // 释放 SSL 上下文
            }
        }
    };

//
//    class SslConnectionSocket : public ConnectionSocket {
//    private:
//        SSL* ssl_; // SSL 对象
//        SP<SSL_CTX,SslCtxDeleter<SSL_CTX>> ctx_;
//
//
//        // 重写关闭操作，处理 SSL 特有的关闭流程
//        void closeSSL() {
//            if (ssl_) {
//                SSL_shutdown(ssl_);
//                SSL_free(ssl_);
//                ssl_ = nullptr;
//            }
//        }
//
//    public:
//        SslConnectionSocket(SP<SSL_CTX,SslCtxDeleter<SSL_CTX>> ctx): ssl_(nullptr), ctx_(std::move(ctx)) {
//
//            SSL* now=SSL_new(ctx_.get());
//            if(now==NULL)
//            {
//                throw std::runtime_error("ssl new error");
//            }
////            SSL_set_fd(now,cli);
//            SSL_accept(now);
//
//            // 初始化 OpenSSL
//            if (!SSL_library_init()) {
//                throw std::runtime_error("OpenSSL library initialization failed");
//            }
//
//            SSL_load_error_strings();
//            OpenSSL_add_all_algorithms();
//
//            ctx_.reset(SSL_CTX_new(TLS_client_method())); // 创建客户端上下文
//            if (!ctx_.get()) {
//                throw std::runtime_error("Failed to create SSL_CTX");
//            }
//        }
//
//        ~SslConnectionSocket() {
//            closeSSL();
//        }
//
//        Bool connect(SockAddrV4 addr){
//            if (!ConnectionSocket::connect(addr)) {
//                return FALSE;
//            }
//            if(ctx_.get()== nullptr){
//                return FALSE;
//            }
//            ssl_ = SSL_new(ctx_.get());
//            if (!ssl_) {
//                throw std::runtime_error("Failed to create SSL object");
//            }
//
//            SSL_set_fd(ssl_, fd()); // 将连接的文件描述符与 SSL 关联
//
//            int result = SSL_connect(ssl_); // 执行 SSL 握手
//            if (result != 1) {
//                int err = SSL_get_error(ssl_, result);
//                throw std::runtime_error("SSL handshake failed: " + std::to_string(err));
//            }
//
//            return TRUE;
//        }
//
//        Bool send(Buffer& buffer){
//            if (writeClosed()) {
//                return FALSE;
//            }
//
//            while (buffer.hasMore()) {
//                Int len = SSL_write(ssl_, buffer.readableAddr(), buffer.readableLength());
//                if (len > 0) {
//                    buffer.advanceRead(len);
//                } else if (len == 0) {
//                    closeWrite();
//                    return FALSE;
//                } else {
//                    int err = SSL_get_error(ssl_, len);
//                    if (err == SSL_ERROR_WANT_WRITE) {
//                        // 等待写操作
//                        Fiber::AwaitEvent(fd(), EPOLLOUT);
//                    } else {
//                        throw std::runtime_error("SSL write failed: " + std::to_string(err));
//                    }
//                }
//            }
//            return TRUE;
//        }
//
//        Bool recv(Buffer& buffer){
//            if (readClosed()) {
//                return FALSE;
//            }
//
//            while (true) {
//                Int len = SSL_read(ssl_, buffer.writeableAddr(), buffer.writeableLength());
//                if (len > 0) {
//                    buffer.advanceWrite(len);
//                    return TRUE;
//                } else if (len == 0) {
//                    closeRead();
//                    return FALSE;
//                } else {
//                    int err = SSL_get_error(ssl_, len);
//                    if (err == SSL_ERROR_WANT_READ) {
//                        // 等待读操作
//                        Fiber::AwaitEvent(fd(), EPOLLIN);
//                    } else {
//                        throw std::runtime_error("SSL read failed: " + std::to_string(err));
//                    }
//                }
//            }
//        }
//
//        void closeRead() {
//            ConnectionSocket::closeRead();
//        }
//
//        void closeWrite() {
//            ConnectionSocket::closeWrite();
//        }
//
//        void close(){
//            closeSSL();
//            ConnectionSocket::close();
//        }
//
//        Bool connected() {
//            return ssl_ != nullptr && ConnectionSocket::connected();
//        }
//
//        SSL* ssl() const {
//            return ssl_;
//        }
//
//    };
//

    class ListenerSocket {
    private:
        FileDescriptor serverFd_; // 服务端套接字文件描述符

    public:
        // 构造函数
        ListenerSocket() : serverFd_(-1) {
            serverFd_ = socket(AF_INET, SOCK_STREAM, 0); // 创建 TCP 套接字
            if (serverFd_ < 0) {
                LOG_ERROR(gl)<<"TcpServer create error with errno"<<errno;
                throw ERRNO_EXCEPTION(errno);
            }
        }

        void defaultInit(const std::string ip,Int port){
            ignorePIPE();
            setNonblocking();
            setReuseAddr();
            bind(ip,port);
            listen();
        }

        // 析构函数
        ~ListenerSocket() {
            if (serverFd_ >= 0) {
                ::close(serverFd_);
            }
        }

        // 返回套接字文件描述符
        FileDescriptor fd() const { return serverFd_; }

        // 设置地址复用
        void setReuseAddr() const {
            int opt = 1;
            if (setsockopt(serverFd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
                LOG_ERROR(gl)<<"TcpServer setReuseAddr error with errno"<<errno;
                throw ERRNO_EXCEPTION(errno);
            }
        }

        // 忽略SIGPIPE信号
        void ignorePIPE() const{
            signal(SIGPIPE, SIG_IGN);  // 忽略 SIGPIPE 信号
        }


        // 设置非阻塞模式
        void setNonblocking() const {
            int flags = fcntl(serverFd_, F_GETFL, 0);
            if (flags == -1) {
                throw ERRNO_EXCEPTION(errno);
            }

            if (fcntl(serverFd_, F_SETFL, flags | O_NONBLOCK) == -1) {
                throw ERRNO_EXCEPTION(errno);
            }
        }

        virtual // 绑定地址和端口
        void bind(const std::string& ip, int port) const {
            SockAddrV4 serveraddr;
            memset(&serveraddr, 0, sizeof(serveraddr));
            serveraddr.sin_family = AF_INET;

            // 将字符串 IP 地址转换为网络字节序的二进制形式
            if (inet_pton(AF_INET, ip.c_str(), &serveraddr.sin_addr) <= 0) {
                throw ERRNO_EXCEPTION(errno,String("Invalid IP:")+ip);
            }

            serveraddr.sin_port = htons(port); // 绑定端口号

            // 绑定地址和端口
            if (::bind(serverFd_, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
                throw ERRNO_EXCEPTION(errno,"Failed to bind address and port");
            }
        }

        virtual // 开始监听
        void listen() const {
            if (::listen(serverFd_, SOMAXCONN) < 0) {
                throw ERRNO_EXCEPTION(errno);
            }
        }

        // 关闭套接字
        void close() {
            Fiber::DeleteEvent(serverFd_,EPOLLIN);
            if (::close(serverFd_) < 0) {
                throw ERRNO_EXCEPTION(errno);
            }
            serverFd_ = -1;
        }

        Bool accept(UP<ConnectionSocket>& connection) {
            // 创建 SockAddrV4 实例来存储客户端的地址信息
            SockAddrV4 clientAddr;
            socklen_t clientlen = sizeof(clientAddr);
            while(TRUE){
                FileDescriptor clientFd = ::accept(serverFd_, (struct sockaddr*)&clientAddr, &clientlen);
                if (clientFd < 0) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        Fiber::AwaitEvent(serverFd_,EPOLLIN);   //异步等待读事件
                    }else{
                        close();
                        return FALSE;
                    }
                }else{
                    connection.reset(new ConnectionSocket(clientFd,clientAddr));
                    return TRUE;
                }
            }

        }



    };

//
//    class SslListenerSocket : public ListenerSocket {
//    private:
//        SP<SSL_CTX,SslCtxDeleter<SSL_CTX> > ctx_;
//
//    public:
//        SslListenerSocket() : ctx_(nullptr),ListenerSocket() {
//
//        }
//
//        void defaultInit(const String& ip,Int port,
//                const String& certPath,const String& keyPath){
//            // 初始化 OpenSSL
//            if (!SSL_library_init()) {
//                throw std::runtime_error("OpenSSL library initialization failed");
//            }
//
//            SSL_load_error_strings();
//            OpenSSL_add_all_algorithms();
//
//            ctx_.reset(SSL_CTX_new(TLS_method())); // 创建 SSL 上下文
//            if (!ctx_.get()) {
//                throw std::runtime_error("Failed to create SSL_CTX");
//            }
//            SSL_CTX_set_options(ctx_.get(),
//                                SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 |
//                                SSL_OP_NO_COMPRESSION |
//                                SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION);
//
//            // 加载https证书
//            if(SSL_CTX_use_certificate_chain_file(ctx_.get(),certPath.c_str())!=1)
//            {
//                throw std::runtime_error("cert load wrong");
//            }
//
//            if(SSL_CTX_use_PrivateKey_file(ctx_.get(),keyPath.c_str(),SSL_FILETYPE_PEM)!=1)
//            {//privkey.pem的路径
//                throw std::runtime_error("key load wrong");
//            }
//
//            ListenerSocket::defaultInit(ip,port);
//        }
//
//        ~SslListenerSocket() =default;
//
//        // 接受连接并返回 SslConnectionSocket（重载）
//        Bool accept(UP<SslConnectionSocket>& connection) {
//            // 创建 SockAddrV4 实例来存储客户端的地址信息
//            SockAddrV4 clientAddr;
//            socklen_t clientlen = sizeof(clientAddr);
//            while (TRUE) {
//                // 接受 TCP 连接
//                FileDescriptor clientFd = ::accept(fd(), (struct sockaddr*)&clientAddr, &clientlen);
//                if (clientFd < 0) {
//                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
//                        Fiber::AwaitEvent(fd(), EPOLLIN);  // 异步等待新连接
//                    } else {
//                        close();
//                        return FALSE;
//                    }
//                } else {
//                    // 创建 SSL 连接对象
//                    UP<SslConnectionSocket> sslConnection = UP<SslConnectionSocket>(new SslConnectionSocket());
//
//                    // 初始化 SSL 连接
//                    if (!sslConnection->serverInit(clientFd, clientAddr, ctx_)) {
//                        // 如果 SSL 连接失败，关闭客户端连接
//                        sslConnection->close();
//                        return FALSE;
//                    }
//
//                    connection = std::move(sslConnection); // 返回 SSL 连接对象
//                    return TRUE;
//                }
//            }
//        }
//    };
}