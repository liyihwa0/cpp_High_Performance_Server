#include "src/protobuf/test_proto.pb.h"
#include "iostream"
#include "string"
using namespace hello;
class UserServiceRpcImpl:UserServiceRpc{

    void Login(::google::protobuf::RpcController* controller,
               const ::hello::LoginRequest* request,
               ::hello::LoginResponse* response,
               ::google::protobuf::Closure* done) override {
        // 1. 获取请求数据
        std::string username = request->name();
        std::string password = request->pwd();

        // 2. 业务逻辑
        if (username == "admin" && password == "password123") {
            response->set_success(true);
        } else {
            response->set_success(false);
        }

        // 3. 调用 done 来标记 RPC 调用完成
        done->Run();
    }
};
int main(){
    LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("1234");
    std::string send_str=request.SerializeAsString();

    LoginRequest parsed;
    if(parsed.ParseFromString(send_str)){
        std::cout<<parsed.name();
        std::cout<<parsed.pwd();
    }
}