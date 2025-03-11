#include "src/net/rpc/rpc.h"
#include "./rpc_test.pb.h"
using namespace wa;

class HelloMethod: public rpc::RpcMethod{
public:
    HelloMethod(): rpc::RpcMethod(
            new rpc::HelloRequest(),     // 请求消息类型
            new rpc::HelloResponse(),    // 响应消息类型
            "HelloMethod"               // 方法名称
            ){}

    rpc::RpcCode call(google::protobuf::Message* req, google::protobuf::Message* res) const override {
        auto* request=dynamic_cast<rpc::HelloRequest*>(req);
        LOG_ASSERT(gl,request!= nullptr);
        auto* response=dynamic_cast<rpc::HelloResponse*>(res);
        LOG_ASSERT(gl,response!= nullptr);

        // 以下为业务代码
        response->set_msg("server recv : "+request->msg());

        return rpc::RpcCode::SUCCESS;
    };
};

class HelloService:public rpc::RpcService{
public:
    HelloService(): rpc::RpcService("HelloService"){
        rpc::RpcMethod* rpcMethod= new HelloMethod();
        addMethod(UP<rpc::RpcMethod>(rpcMethod));
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

        rpc::RpcMethodCaller caller(connection,new rpc::HelloRequest(),new rpc::HelloResponse(),
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