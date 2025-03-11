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
    ConnectionReadWorker reader (SP<ConnectionSocket>(std::move(connectionSocket)) );
    cout<<"accepted";
    ConnectionWriteWorker writer (SP<ConnectionSocket>(std::move(connectionSocket)) );

    Fiber::AddTask([&reader]{
        reader.run();
    },"fd reader");
    Fiber::AddTask([&writer]{
        writer.run();
    },"fd writer");
    Fiber::AddTask([&reader,&writer]{
        while(true){
            UP<Buffer> bf;
            if(!reader.read(bf)){
                LOG_INFO(gl)<<"elegant exit";
                return ;
            }

            LOG_INFO(gl)<<bf->toString();
            bf->clear();
            reader.recycle(bf);
        }
    },"reader");

    Fiber::Start(2);
}