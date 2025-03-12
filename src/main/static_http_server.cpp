#include "src/fiber/channel.h"
#include "src/fiber/fiber.h"
#include "src/http/http_common.h"
#include "src/net/socket.h"
#include "src/net/worker/socket_worker.h"
#include "src/http/request.h"
#include "src/http/request_parser.h"
#include "src/http/response.h"
#include "src/http/worker/request_parse_worker.h"
#include "src/http/servlet.h"
#include <filesystem>
using namespace std;
using namespace wa;
using namespace wa::http;
#define PORT 8889
#define min(a,b) (a)<(b)?(a):(b)

// 定义的全局BufferPool
#define GLOBAL_BUFFER_POOL_SIZE 2048        //2k个
#define GLOBAL_BUFFER_POOL_BUFFER_SIZE 10 //2kb
SP<Channel<UP<Buffer>>> gBufferPool(new Channel<UP<Buffer>>(GLOBAL_BUFFER_POOL_SIZE));

int main(){
    gl->setLevel(wa::LogLevel::WARN);

    // 初始化gBufferPool
    for(Int i=0;i<GLOBAL_BUFFER_POOL_SIZE;i++){
        gBufferPool->write(UP<Buffer> (new Buffer(GLOBAL_BUFFER_POOL_BUFFER_SIZE)));
    }


    DispatchServlet dispatchServlet;
    dispatchServlet.scanStaticDir("./static");


    SP<ListenerSocket> listenerSocket(new ListenerSocket);
    listenerSocket->defaultInit("0.0.0.0",PORT);


    SP<ListenerWorker> listenerWorker(new ListenerWorker(listenerSocket));
    Fiber::AddTask([listenerWorker]{
        listenerWorker->run();
    },"listener");

    Fiber::AddTask([listenerWorker,&dispatchServlet]{
        UP<ConnectionSocket> connection;
        while(listenerWorker->accept(connection)){
            LOG_INFO(gl)<<"create connection";
            SP<ConnectionSocket> spConnection(connection);
            SP<ConnectionReadWorker> reader(new ConnectionReadWorker(spConnection,gBufferPool));
            Fiber::AddTask([reader]{
                reader->run();// 在reader结束时,我们应该同样关闭writer
            },"reader");

            SP<ConnectionWriteWorker> writer(new ConnectionWriteWorker(spConnection,gBufferPool));
            Fiber::AddTask([writer]{
                writer->run();
            },"writer");

            SP<Channel<UP<HttpRequest>>> requestChannel(new Channel<UP<HttpRequest>>(10));

            Fiber::AddTask([reader,requestChannel]{
                HttpRequestParseWorker parserWorker(reader->readableChannel(),
                                         gBufferPool,
                                         requestChannel);
                parserWorker.run();
            },"parser");

            Fiber::AddTask([requestChannel,writer,reader, &dispatchServlet,spConnection]{
                while(TRUE){
                    UP<HttpRequest> request;
                    if(!requestChannel->read(request)){
                        break;
                    }
                    UP<HttpResponse> response(new HttpResponse());
                    dispatchServlet.handle(*request,*response);
                    response->setKeepAlive(request->keepAlive());

                    String responseString=response->toString();
                    Int i=0;
                    while(i<responseString.length()){
                        UP<Buffer> bf;
                        gBufferPool->read(bf);
                        int cplen=min(bf->writeableLength(),responseString.length()-i);
                        memcpy(bf->writeableAddr(),responseString.data()+i,cplen);
                        i+=cplen;
                        bf->advanceWrite(cplen);
                        if(!writer->write(bf)){
                            goto end;
                        }
                    }

                    if(!response->keepAlive()){
                        break;
                    }
                }
                end:
                reader->stop();
                writer->stop();
            },"request handler");
        }
    },"create client handler");


    Fiber::Start(2);
    LOG_INFO(gl)<<"exit";
}