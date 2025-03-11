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
    gl->setLevel(wa::LogLevel::WARN);

    DispatchServlet dispatchServlet;
    dispatchServlet.scanDir("./static");


    SP<ListenerSocket> listenerSocket(new ListenerSocket);
    listenerSocket->defaultInit("0.0.0.0",PORT);



    Fiber::AddTask([listenerSocket,&dispatchServlet]{
        UP<ConnectionSocket> connection;
        while(TRUE){
            Int accpet=listenerSocket->accept(connection);
            if(accpet==1){
                SP<ConnectionSocket> spConnection(connection);
                Fiber::AddTask([spConnection,&dispatchServlet]{
                        LOG_DEBUG(gl)<<"create connection";
                        HttpRequestParser parser;
                        // 拿到新链接后
                        while(TRUE){
                            // 异步读数据出来
                            UP<Buffer> buffer(new Buffer(2048));
                            Bool success=spConnection->recv(*buffer);
                            LOG_ASSERT(gl,success);
                            LOG_ASSERT(gl,buffer->readableLength()!=2048);
                            HttpParseState state= parser.input(buffer);
                            HttpResponse response;
                            LOG_ASSERT(gl, state == wa::http::HttpParseState::DONE);
                            UP<HttpRequest> request=parser.getRequest();
                            dispatchServlet.handle(*request,response);


                            Buffer toWrite(response.toString());
                            // 异步写入
                            success= spConnection->send(toWrite);
                            LOG_ASSERT(gl,success);
                            spConnection->close();
                            LOG_DEBUG(gl)<<"connection ok";
                            return ;
                        }
                },"handle client");
            }
        }
    },"listener");




    Fiber::Start(6);
}