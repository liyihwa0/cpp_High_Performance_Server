#include "src/data_structure/buffer.h"
#include "src/fiber/channel.h"
#include "src/net/socket.h"

#define BUFFER_COUNT 20
#define BUFFER_SIZE 2048
#define LISTENER_CHANNEL_SIZE 1000
#define CONNECTION_FORWARD_BUFFER_SIZE  1024

namespace wa{
    // 会不断地将 connectionSocket中可读的数据 搬到 forReadBuffer中
    // 在使用 ConnectionReadWorker 时,我们需要
    // 1. 通过 read 获取包含数据的buffer
    // 2. 读buffer
    // 3. 调用recycle回收空buffer
    //  当我们不再需要read读数据时, 使用 stop,使其不再向 connectionSocket_读数据,并关闭connectionSocket_的读端
    class ConnectionReadWorker{
    private:
        SP<ConnectionSocket> connectionSocket_;
        SP<Channel<UP<Buffer>>> forReadBuffers_;
        SP<Channel<UP<Buffer>>> bufferPool_;
    public:

        explicit ConnectionReadWorker(SP<ConnectionSocket> connectionSocket,
                                      SP<Channel<UP<Buffer>>> bufferPool,
                                      Int bufferCount=BUFFER_COUNT):
                connectionSocket_(std::move(connectionSocket)),
                bufferPool_(std::move(bufferPool)),
                forReadBuffers_(new Channel<UP<Buffer>>(bufferCount)){
        }

        Bool read(UP<Buffer>&bf){
            return forReadBuffers_->read(bf);
        }

        SP<Channel<UP<Buffer>>> readableChannel(){
            return forReadBuffers_;
        }

        void stop(){
            forReadBuffers_->close();
        }

        void run(){
            // 不断从emptyBuffer中读取空buffer
            // 将其送给connectionSocket_读取数据
            // 将包含数据的buffer,放入待读队列
            UP<Buffer> bf;
            if(!bufferPool_->read(bf)){
                goto stop;
            }
            bf->clear();

            while(TRUE){
                if(connectionSocket_->recv(*(bf.get()))){  //成功地读到了数据
                    // 将其写入readBuffer
                    if(!forReadBuffers_->write(bf)){
                        goto stop;
                    }
                    // 从bufferPool中再读一个
                    if(!bufferPool_->read(bf)){
                        goto stop;
                    }
                    bf->clear();
                } else {  // 对方关闭或出异常,需要终止read
                    goto stop;
                }
            }
            stop:
            if(bf.get()){
                bufferPool_->write(bf);
            }
            connectionSocket_->closeRead();
            forReadBuffers_->close();
        }

    };

    // 会不断地将 forWriteBuffers 里的数据发送到 connectionSocket中
    // 在使用 ConnectionWriteWorker时,我们需要
    // 1. 通过 getEmptyBuffer 获取空buffer
    // 2. 将数据写入buffer
    // 3. 调用write,将buffer交给 writer处理
    // 当我们不再需要writer写数据时, 使用 stop,使其将剩余的数据写完再结束
    // ConnectionWriteWorker在结束时,还会关闭 connectionSocket的写端
    class ConnectionWriteWorker{
    private:
        SP<ConnectionSocket> connectionSocket_;
        SP<Channel<UP<Buffer>>> forWriteBuffers_;
        SP<Channel<UP<Buffer>>> bufferPool_;
    public:

        explicit ConnectionWriteWorker(
                SP<ConnectionSocket> connectionSocket,
                SP<Channel<UP<Buffer>>> bufferPool,
                Int bufferCount=BUFFER_COUNT):
                connectionSocket_(std::move(connectionSocket)),
                bufferPool_(std::move(bufferPool)),
                forWriteBuffers_(new Channel<UP<Buffer>>(bufferCount)){}


        Bool write(UP<Buffer>&bf){
            return forWriteBuffers_->write(bf);
        }

        void stop(){
            forWriteBuffers_->close();
        }

        void run(){

            UP<Buffer> bf;
            if(!forWriteBuffers_->read(bf)) { // channel被关闭了
                goto stop;
            }

            while(TRUE){
                if(connectionSocket_->send(*(bf.get()))){  //成功地发送了数据
                    if(!bf->hasMore()){
                        bf->clear();
                        if(!bufferPool_->write(bf)){
                            goto stop;
                        }

                        if(!forWriteBuffers_->read(bf)){ //代表不再会有新的可写数据了,写端已被关闭
                            goto stop;
                        }
                    }
                } else {  // 写端被关闭或者出现了别的问题,需要中止
                    goto stop;
                }
            }

            stop:
            connectionSocket_->closeWrite();
            forWriteBuffers_->close();
        }
    };
    class ListenerWorker{
    private:
        SP<ListenerSocket> listenerSocket_;
        Channel<UP<ConnectionSocket>> connectionChannel_;
    public:

        explicit ListenerWorker(SP<ListenerSocket> socket,Int channelSize=LISTENER_CHANNEL_SIZE):
        connectionChannel_(channelSize),
        listenerSocket_(std::move(socket)){}

        Bool accept(UP<ConnectionSocket>& connection){
            return connectionChannel_.read(connection);
        }

        void run(){
            UP<ConnectionSocket> connection;
            while(TRUE){
                if(listenerSocket_->accept(connection)){
                    connectionChannel_.write(connection);
                }else{
                    goto close;
                }
            }
            close:
            throw OS_ERRNO_EXCEPTION();
            connectionChannel_.close();
        }
    };


    // 会不断地读in 里的数据,并发送到 out中
    class ConnectionForwardWorker{
    private:
        SP<ConnectionSocket> in_;
        SP<ConnectionSocket> out_;
        Buffer buffer_;
    public:

        explicit ConnectionForwardWorker(
                SP<ConnectionSocket> in,
                SP<ConnectionSocket> out,
                Int bufferSize=CONNECTION_FORWARD_BUFFER_SIZE):
                in_(std::move(in)), out_(std::move(out)),
                buffer_(bufferSize){}


        void run(){
            while(TRUE){
                if(!in_->recv(buffer_)) {
                    LOG_WARN(gl)<<"failed to recv";
                    goto stop;
                }

                if(!out_->send(buffer_)){
                    LOG_WARN(gl)<<"failed to send";
                    goto stop;
                }
            }


            stop:
            in_->closeRead();
            out_->closeWrite();
        }

        ~ConnectionForwardWorker(){
            in_->closeRead();
            out_->closeWrite();
        }
    };

}