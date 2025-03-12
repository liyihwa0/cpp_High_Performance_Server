#include "src/raft/raft_core.h"
#include "./rpc.pb.h"
#include "src/data_structure/skiplist.h"
namespace wa{
  namespace raft_kvserver{
      namespace ErrType{
          const String OK = "OK";
          const String ErrNoKey = "ErrNoKey";
          const String ErrWrongLeader = "ErrWrongLeader";
          const String ErrRepeatedRequest = "ErrRepeatedRequest";
      };
        //kvServer provider类
      class KvServer:public rpc::RpcService{
      private:
          raft::Raft node_;
          SkipList<String ,String> db_;
          SP<Channel<UP<raft::ApplyMessage>>> applyCh_;

          std::unordered_map<String ,std::unordered_map<Int ,Bool>> clients_;

          SpinLock lock_;
          ListenerSocket listenerSocket_;

      public:
          KvServer(const String&ip,const Int port):
          rpc::RpcService("kvServer"),
          db_(0.5,6){
              listenerSocket_.defaultInit(ip,port);

              // 初始化RpcService
              addMethod<GetRequest,GetResponse>("get",std::bind(&KvServer::getRpcMethod,this,std::placeholders::_1,std::placeholders::_2));
              addMethod<SetRequest,SetResponse>("set",std::bind(&KvServer::setRpcMethod,this,std::placeholders::_1,std::placeholders::_2));


              //todo
              //node_.serverInit
          }

          // 对外提供的 set服务
          rpc::RpcCode setRpcMethod(SetRequest *request,SetResponse *response){
                // 先判断有没有重复服务过
                lock_.lock();
                if(clients_[request->clientid()][request->requestid()]){
                    response->set_err(ErrType::ErrRepeatedRequest);
                    lock_.unlock();
                    return rpc::RpcCode::SUCCESS;
                }

                clients_[request->clientid()][request->requestid()]=TRUE;

                lock_.unlock();
                Bool success=node_.addLog(request->SerializeAsString());
                if(!success){
                    response->set_err(ErrType::ErrWrongLeader);
                    return rpc::RpcCode::SUCCESS;
                }
                // 执行set操作
                lock_.lock();
                db_.set(request->key(),request->value());
                response->set_err(ErrType::OK);
                lock_.unlock();
                return rpc::RpcCode::SUCCESS;
          }

          // 对外提供的get服务
          rpc::RpcCode getRpcMethod(GetRequest *request,GetResponse *response){
                // 先判断有没有重复服务过
              lock_.lock();
              if(clients_[request->clientid()][request->requestid()]){
                  response->set_err(ErrType::ErrRepeatedRequest);
                  lock_.unlock();
                  return rpc::RpcCode::SUCCESS;
              }

              clients_[request->clientid()][request->requestid()]=TRUE;

              lock_.unlock();
              Bool success=node_.addLog(request->SerializeAsString());
              if(!success){
                  response->set_err(ErrType::ErrWrongLeader);
                  return rpc::RpcCode::SUCCESS;
              }
              // 执行set操作
              lock_.lock();
              if(!db_.contain(request->key())){
                  response->set_err(ErrType::ErrNoKey);
                  lock_.unlock();
                  return rpc::RpcCode::SUCCESS;
              }
              response->set_value(db_.get(request->key(),""));
              response->set_err(ErrType::OK);
              lock_.unlock();
              return rpc::RpcCode::SUCCESS;

          }
    };

      class KvServerCaller{
      private:
          rpc::RpcMethodCaller<GetRequest,GetResponse> getCaller_;
          rpc::RpcMethodCaller<SetRequest,SetResponse> setCaller_;
      public:
          KvServerCaller(UP<ConnectionSocket> get,UP<ConnectionSocket> set):
          getCaller_(get,"get","kv_server"),
          setCaller_(set,"set","kv_server"){}


          rpc::RpcCode get(GetRequest *args,GetResponse *response){
              return getCaller_.call(args,response);
          }
          rpc::RpcCode set(SetRequest *args,SetResponse *response){
              return setCaller_.call(args,response);
          }
      };

      class KvClient{
      private:
          std::vector<UP<KvServerCaller>> serverCaller_;
          String clientId_;
          Int requestId_;
          Int currentLeaderId_;
      public:
          //对外暴露的三个功能和初始化
          String get(std::string key){
              requestId_++;
              raft_kvserver::GetRequest request;
              request.set_key(key);
              request.set_clientid(clientId_);
              request.set_requestid(requestId_);

              while (true) {
                  raft_kvserver::GetResponse response;
                  rpc::RpcCode code = serverCaller_[currentLeaderId_]->get(&request, &response);
                  if (!rpc::rpcSuccess(code) ||
                      response.err() ==
                      raft_kvserver::ErrType::ErrWrongLeader) {  //会一直重试，因为requestId没有改变，因此可能会因为RPC的丢失或者其他情况导致重试，kvserver层来保证不重复执行（线性一致性）
                      currentLeaderId_ = (currentLeaderId_ + 1) % serverCaller_.size();
                      continue;
                  }
                  if (response.err() ==raft_kvserver::ErrType:: ErrNoKey) {
                      return "";
                  }
                  if (response.err() ==raft_kvserver::ErrType:: OK) {
                      return response.value();
                  }
              }
              return "";
          }
          void set(std::string key, std::string value){
              requestId_++;
              raft_kvserver::SetRequest request;
              request.set_key(key);
              request.set_value(value);
              request.set_clientid(clientId_);
              request.set_requestid(requestId_);

              while (true) {
                  raft_kvserver::SetResponse response;
                  rpc::RpcCode code = serverCaller_[currentLeaderId_]->set(&request, &response);
                  if (!rpc::rpcSuccess(code) ||
                      response.err() ==
                      raft_kvserver::ErrType::ErrWrongLeader) {  //会一直重试，因为requestId没有改变，因此可能会因为RPC的丢失或者其他情况导致重试，kvserver层来保证不重复执行（线性一致性）
                      currentLeaderId_ = (currentLeaderId_ + 1) % serverCaller_.size();
                      continue;
                  }

                  return;
              }
          }

      };
  };
};