#include "src/fiber/channel.h"
#include "src/fiber/fiber.h"
#include "src/http/http_common.h"
#include "src/net/socket.h"
#include "src/net/worker/socket_worker.h"
#include "src/http/request.h"
#include "src/http/request_parser.h"
#include "src/http/response.h"
using namespace std;
using namespace wa;
using namespace wa::http;
#define PORT 8888
#define min(a,b) (a)<(b)?(a):(b)

// 定义的全局BufferPool
#define GLOBAL_BUFFER_POOL_SIZE 2048        //2k个
#define GLOBAL_BUFFER_POOL_BUFFER_SIZE 2048 //2kb
SP<Channel<UP<Buffer>>> gBufferPool(new Channel<UP<Buffer>>(GLOBAL_BUFFER_POOL_SIZE));

int main(){
    gl->setLevel(wa::LogLevel::DEBUG);

    // 初始化gBufferPool
    for(Int i=0;i<GLOBAL_BUFFER_POOL_SIZE;i++){
        gBufferPool->write(UP<Buffer> (new Buffer(GLOBAL_BUFFER_POOL_BUFFER_SIZE)));
    }


    SP<ListenerSocket> listenerSocket(new ListenerSocket);
    listenerSocket->defaultInit("0.0.0.0",PORT);


    SP<ListenerWorker> listenerWorker(new ListenerWorker(listenerSocket));
    Fiber::AddTask([listenerWorker]{
        listenerWorker->run();
    },"listener");

    Fiber::AddTask([listenerWorker]{
        UP<ConnectionSocket> connection;
        while(listenerWorker->accept(connection)){
            SP<ConnectionSocket> client(connection);

            Fiber::AddTask([client]{
                LOG_INFO(gl)<<"new client";

                HttpRequestParser parser;
                // 解析第一个报文出来
                UP<Buffer> buffer(new Buffer(2048));
                while (TRUE){
                    if(!client->recv(*buffer)){
                        return ;
                    }
                    LOG_INFO(gl)<<"parsing..";
                    HttpParseState state= parser.input(buffer);
                    if(state==wa::http::HttpParseState::DONE){   // 如果确定解析完毕
                        auto request=parser.getRequest();

                        if(request->method()!=HttpMethod::CONNECT){
                            LOG_INFO(gl)<<"Unsupport method "<<request->methodString();
                            auto response=HttpResponse::make502Response();
                            UP<Buffer> writeBuffer(new Buffer(response->toString()));
                            client->send(*writeBuffer);
                            client->close();
                        }else{
                            LOG_INFO(gl)<<"supported CONNECT method";

                            SP<ConnectionSocket> remote=SP<ConnectionSocket>(new ConnectionSocket());

                            // 需要建立新连接
                            String domainPort=request->uri();
                            String domain="",port="80";
                            Size pos=domainPort.find(":");
                            if (pos == std::string::npos) {
                                domain=domainPort;
                            }else{
                                domain=domainPort.substr(0,pos);
                                port=domainPort.substr(pos+1);
                            }
                            domain=Helper::DnsParse(domain);

                            // 如果与远程连接失败,则断开连接
                            if(!remote->connect(domain, stoi(port))){
                                LOG_WARN(gl)<<"connect remote failed";
                                auto response=HttpResponse::make502Response();
                                UP<Buffer> _502Buffer(new Buffer(response->toString()));
                                client->send(*_502Buffer);
                                client->close();
                            }else { //成功建立起了连接
                                // 向客户端发送200
                                LOG_INFO(gl)<<"connect remote success";
                                auto response=HttpResponse::makeConnectionEstablishedResponse();
                                UP<Buffer> _200Buffer(new Buffer(response->toString()));
                                client->send(*_200Buffer);
                                if (buffer->hasMore()) {
                                    remote->send(*buffer);
                                }
                                Fiber::AddTask([remote, client] {
                                    ConnectionForwardWorker forwardWorker(client, remote);
                                    forwardWorker.run();
                                    LOG_INFO(gl)<<"client2remote stop";
                                }, "client 2 remote");
                                Fiber::AddTask([remote, client] {
                                    ConnectionForwardWorker forwardWorker(remote, client);
                                    forwardWorker.run();
                                    LOG_INFO(gl)<<"remote2client stop";
                                }, "remote 2 client");
                            }
                        }

                        break;
                    }
                }
            },"client handler");


        }
    },"connection handler");


    Fiber::Start(1);
    LOG_INFO(gl)<<"exit";
}