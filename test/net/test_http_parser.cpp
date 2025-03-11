#include <netinet/in.h>
#include "src/fiber/channel.h"
#include "src/fiber/fiber.h"
#include "iostream"
#include "src/http/http_common.h"
#include "src/net/socket.h"
#include "src/worker/socket_worker.h"
using namespace std;
using namespace wa;
using namespace wa::http;
#define PORT 8888

int main(){
    ListenerSocket listenerSocket;
    listenerSocket.defaultInit("0.0.0.0",PORT);
    UP<ConnectionSocket> connectionSocket=listenerSocket.accept();
    ConnectionReadWorker ccManager (SP<ConnectionSocket>(std::move(connectionSocket)) );
    Fiber::AddTask([&ccManager]{
        ccManager.run();
    },"fd reader");
    Fiber::AddTask([&ccManager]{
        ccManager.runWriter();
    },"fd writer");

    SP<Channel<UP<HttpRequest>>> requestChannel(new Channel<UP<HttpRequest>>(10));
    HttpRequestParser parser(ccManager.readableChannel(),
                             ccManager.emptyChannel(),
                             requestChannel);

    Fiber::AddTask([&parser]{
        parser.run();
    },"parser");

    Fiber::AddTask([&requestChannel,&ccManager]{
        while(TRUE){
            UP<HttpRequest> request;
            if(!requestChannel->read(request)){
                break;
            }
            LOG_INFO(gl)<<request->uri();
            UP<Buffer> bf(new Buffer(16));
            memcpy(bf->readableAddr(),"123456\0",7);
            bf->advanceRead(7);
            ccManager.write(bf);
        }

    },"reader");

    Fiber::Start(1);
}