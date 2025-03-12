#include "src/net/rpc/rpc.h"
#include "./rpc_test.pb.h"
using namespace wa;

class HelloService:public rpc::RpcService{
public:
    HelloService(): rpc::RpcService("HelloService"){
        std::function<rpc::RpcCode(rpc::HelloRequest*, rpc::HelloResponse*)> callback =
                [](rpc::HelloRequest* request, rpc::HelloResponse* response) {
                    response->set_msg(request->msg()+" recved");
                    return rpc::RpcCode::SUCCESS;
                };

        addMethod("HelloMethod",callback);// UP<rpc::RpcMethod<rpc::HelloRequest,rpc::HelloResponse>>(rpcMethod));
    }
    virtual ~HelloService()=default;
};

int main(){

    rpc::RpcServiceProvider provider;
    provider.publishService(UP<rpc::RpcService>(new HelloService()));
    Fiber::AddTask([&provider]{
        provider.run();
    },"provider"); //rpc 服务器已启动

    Fiber::AddTask([]{
        UP<ConnectionSocket> connection=UP<ConnectionSocket>(new ConnectionSocket());
        connection->connect("127.0.0.1",8888);

        rpc::RpcMethodCaller<rpc::HelloRequest,rpc::HelloResponse> caller(
                connection,
                               "HelloMethod",
                               "HelloService"
                               );
        rpc::HelloRequest request;
        request.set_msg("123");
        rpc::HelloResponse response;
        std::cout<<caller.call(&request,&response)<<" "<<response.msg()<<std::endl;
    },"client1");
    gl->setLevel(wa::LogLevel::DEBUG);
    Fiber::Start(2);

}