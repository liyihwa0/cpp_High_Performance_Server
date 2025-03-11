#include <filesystem>
#include "src/fiber/channel.h"
#include "src/fiber/fiber.h"
#include "src/http/http_common.h"
#include "src/net/socket.h"
#include "src/worker/socket_worker.h"
#include "src/http/request.h"
#include "src/http/request_parser.h"
#include "src/http/response.h"
#include "src/http/servlet.h"
using namespace std;
using namespace wa;
using namespace wa::http;
#define PORT 8888
#define min(a,b) (a)<(b)?(a):(b)
int main(){
    gl->setLevel(wa::LogLevel::INFO);

    DispatchServlet dispatchServlet;
    dispatchServlet.scanDir("./static");


    SP<ListenerSocket> listenerSocket(new ListenerSocket);
    listenerSocket->defaultInit("0.0.0.0",PORT);

    SP<ListenerWorker> listenerWorker(new ListenerWorker(listenerSocket));
    Fiber::AddTask([listenerWorker]{
        listenerWorker->run();
    },"listener");

    Fiber::AddTask([listenerWorker,&dispatchServlet]{
        UP<ConnectionSocket> connection;
        while(listenerWorker->accept(connection)){
            SP<ConnectionSocket> spConnection(connection);
            Fiber::AddTask([spConnection,&dispatchServlet]{
                LOG_DEBUG(gl)<<"create connection";
                HttpRequestParser parser;
                // 拿到新链接后
                while(TRUE){
                    // 循环读取数据出来
                    UP<Buffer> buffer(new Buffer(2048));
                    while(TRUE){
                        Int res=spConnection->recv(*buffer);
                        if(res>0){
                            HttpParseState state= parser.input(buffer);
                            if(state == wa::http::HttpParseState::DONE){
                                UP<HttpRequest> request=parser.getRequest();

                                HttpResponse response;
                                dispatchServlet.handle(*request,response);
                                String responseString=response.toString();

                                Buffer bf(responseString.length());
                                memcpy((void *) responseString.c_str(), bf.writeableAddr(), bf.writeableLength());
                                while(TRUE){
                                    Int writeRes=spConnection->send(bf); //假设一定写完
                                    if(writeRes==-1){
                                        // 全部写完后,关闭链接,并return
                                        spConnection->close();
                                        return ;
                                    }
                                    LOG_INFO(gl)<<writeRes;
                                    LOG_ASSERT(gl,writeRes==responseString.length());
                                    break;
                                }
                                // 全部写完后,关闭链接,并return
                                spConnection->close();
                                return ;
                            }
                        }else if(res==0){  //异步无数据
                            Fiber::AwaitEvent(spConnection->fd(),EPOLLIN);
                        }else{
                            spConnection->close();
                            LOG_INFO(gl)<<"不正常退出";
                            return;
                        }
                    }
                }
                LOG_DEBUG(gl)<<"create connection ok";
            });
        }
    },"create client handler");


    Fiber::Start(6);
}