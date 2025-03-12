#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include "../../global_def.h"
#include "../../net/worker/socket_worker.h"
#include "./rpc_framework.pb.h"
#include "functional"
#pragma once

namespace wa{
    namespace rpc  {
        enum RpcCode{
            SUCCESS=0,
            REQUEST_SERVICE_NOT_FOUND,
            REQUEST_METHOD_NOT_FOUND,
            REQUEST_ARGS_PARSE_ERROR,
            NETWORK_ERROR,
            RESPONSE_HEADER_PARSE_ERROR,
            RESPONSE_DATA_PARSE_ERROR
        };

        inline Bool rpcSuccess(RpcCode code){
            return code==RpcCode::SUCCESS;
        }

        struct RpcRequest{
            String service;
            String method;
            String args;
        };

        struct RpcResponse{
            RpcCode errno;
            String args;
        };

        class RpcMethodBase{
        public:
            virtual RpcCode _call(google::protobuf::Message* request, google::protobuf::Message* response) const = 0;
            virtual const String& methodName() const = 0;
            virtual const String& serviceName() const = 0;
            virtual google::protobuf::Message* requestInstance()const=0;
            virtual google::protobuf::Message* responseInstance() const=0;
            virtual ~RpcMethodBase() = default;
        };

        template<typename REQ,typename RSP>
        class RpcMethod :public RpcMethodBase{
        protected:
            REQ* request_;
            RSP* response_;
            String methodName_;
            String serviceName_;

            RpcMethod(const String& methodName_,const String & serviceName=""):
                    request_(new REQ()), response_(new RSP()), methodName_(methodName_), serviceName_(serviceName){}

        public:
            virtual ~RpcMethod(){
                delete request_;
                delete response_;
            }
            REQ* requestInstance()const override{
                return request_->New();
            }
            RSP* responseInstance() const override{
                return response_->New();
            }
            const String& methodName()const override{
                return methodName_;
            }

            const String& serviceName()const override{
                return serviceName_;
            }

            RpcCode _call(google::protobuf::Message* request, google::protobuf::Message* response)const final {
                REQ* castedReq=dynamic_cast<REQ*>(request);
                LOG_ASSERT(gl,castedReq!= nullptr);
                RSP* castedRsp=dynamic_cast<RSP*>(response);
                LOG_ASSERT(gl,castedRsp!= nullptr);
                return call(castedReq,castedRsp);
            }

            virtual RpcCode call(REQ* , RSP*) const = 0;
        };


        // 客户端类
        template<typename REQ,typename RSP>
        class RpcMethodCaller:RpcMethod<REQ,RSP>{
        private:
            UP<ConnectionSocket> connectionSocket_;  // 用于网络通信

        public:
            // 构造函数
            RpcMethodCaller(UP<ConnectionSocket> connectionSocket,
                            const String& methodName,
                            const String& serviceName)
                    : connectionSocket_(connectionSocket), RpcMethod<REQ, RSP>(methodName,serviceName) {

            }


            // call方法可以被同时调用,可以保证call方法是按序执行的
            RpcCode call(REQ* request, RSP* response) const override {
                // 序列化请求数据
                FrameworkRequestHeader requestHeader;
                requestHeader.set_servicename(RpcMethod<REQ,RSP>::serviceName());
                requestHeader.set_methodname(RpcMethod<REQ,RSP>::methodName());
                requestHeader.set_argssize(request->ByteSizeLong());
                //todo buffer大小目前被固定到了1024字节
                Buffer buffer(1024);
                U32 headerSize=requestHeader.ByteSizeLong();
                buffer.write32(headerSize);

                requestHeader.SerializeToArray(buffer.writeableAddr(),buffer.writeableLength());
                buffer.advanceWrite(headerSize);
                if(request->ByteSizeLong()){
                    request->SerializeToArray(buffer.writeableAddr(),buffer.writeableLength());
                    buffer.advanceWrite(request->ByteSizeLong());
                }

                if(!connectionSocket_->send(buffer)){
                    return RpcCode::NETWORK_ERROR;
                }
                buffer.clear();
                // 接收响应数据
                FrameworkResponseHeader responseHeader;
                if (!connectionSocket_->recv(buffer)) {
                    return RpcCode::NETWORK_ERROR;
                }
                // 先读4字节responseHeaderLen
                headerSize=buffer.read32();

                // 读headerSize字节的responseHeader并解析响应
                if(!responseHeader.ParseFromArray(buffer.readableAddr(),headerSize)){
                    return RpcCode::RESPONSE_HEADER_PARSE_ERROR;
                }
                buffer.advanceRead(headerSize);
                if (responseHeader.code() != RpcCode::SUCCESS) {
                    return  (RpcCode) responseHeader.code();
                }

                // 反序列化响应数据
                if (!response->ParseFromArray(buffer.readableAddr(),buffer.readableLength())) {
                    return RpcCode::RESPONSE_DATA_PARSE_ERROR;
                }

                return RpcCode::SUCCESS;
            }
        };


        // 如果我们要提供一个服务,需要写一个类,来继承RpcService
        class RpcServiceProvider;
        class RpcService{

            // RpcMethodWithCallback 继承自 RpcMethod，封装了回调逻辑
            template<typename REQ,typename RSP>
            class RpcMethodWithCallback : public RpcMethod<REQ,RSP> {
            private:
                std::function<RpcCode(REQ*, RSP*)> callback_;

            public:
                RpcMethodWithCallback(const String& name,
                                      std::function<RpcCode(REQ*, RSP*)> callback)
                        : RpcMethod<REQ,RSP>(name), callback_(callback) {}

                // 调用回调函数处理请求和响应
                RpcCode call(REQ* req, RSP* res) const override {
                    return callback_(req, res);
                }
            };

            friend RpcServiceProvider;
            String name_;
        protected:
            RpcService(const String&name):name_(name){}

//            template<typename REQ,typename RSP>
//            void addMethod(UP<RpcMethod<REQ, RSP>> method){
//                String methodName=method->methodName();
//                methods_[methodName]=SP<RpcMethod<google::protobuf::Message, google::protobuf::Message>>(method);
//            }

            template<typename REQ,typename RSP>
            void addMethod(const String& methodName,
                           std::function<RpcCode(REQ*, RSP*)> callback) {

                // 创建一个新的 RpcMethod 实例，使用回调函数
                auto method = UP<RpcMethodBase>(new RpcMethodWithCallback<REQ, RSP>(methodName, callback));

                // 将方法添加到 methods_ 映射中
                methods_[methodName] = method;
            }
            std::unordered_map<String,UP<RpcMethodBase>> methods_;
        public:


            RpcService() = default;

            const String& name()const {
                return name_;
            }
        };



        class RpcServiceProvider{
            SP<Channel<UP<Buffer>>> bufferPool_;
            std::unordered_map<std::string ,SP<RpcService>> services_;
            class RpcRequestParser{
                const Int _32BYTES_LEN=4;   // varint32最多需要5字节数据就能完成解析
                enum class RpcRequestParseState{
                    START,
                    WAIT_HEADER_SIZE,
                    WAIT_HEADER,
                    WAIT_ARGS,
                    DONE,
                    ERROR,
                };

                void reset(){
                    state_=RpcRequestParseState::WAIT_HEADER_SIZE;
                    tempHeaderSize_.clear();
                    tempHeader_.clear();
                    tempArgs_.clear();
                    headerSize_=0;
                    request_.reset(new RpcRequest());
                }

                Buffer tempHeaderSize_; // 用于临时保存HeaderSize
                void parseHeaderSize(UP<Buffer> &buffer){
                    // 从buffer中读取若干字节数据给tempHeaderSize_
                    // 若干字节 = min(_32BYTES_LEN-tempHeaderSize_.readableLength(),buffer->readableLength);
                    Int x=std::min(_32BYTES_LEN - tempHeaderSize_.readableLength(), buffer->readableLength());


                    buffer->copy(tempHeaderSize_,x);
                    buffer->advanceRead(x);

                    if(tempHeaderSize_.readableLength() >= _32BYTES_LEN){
                        headerSize_=tempHeaderSize_.read32();
                        state_=RpcRequestParseState::WAIT_HEADER;
                    }
                }

                String tempHeader_; // 用于临时保存Header
                void parseHeader(UP<Buffer> &buffer){
                    // 从buffer中读取若干字节数据给tempHeader_
                    // 若干字节 = min(headerSize_-tempHeader_.length(),buffer->readableLength());
                    UInt x=std::min(headerSize_-tempHeader_.length(),(UInt ) buffer->readableLength());

                    tempHeader_+=buffer->subString(0,x);
                    buffer->advanceRead(x);

                    if(tempHeader_.size()>=headerSize_){
                        if(!rpcHeader_.ParseFromString(tempHeader_)){
                            state_=RpcRequestParseState::ERROR;
                            return;
                        }

                        if(rpcHeader_.argssize()!=0){
                            state_=RpcRequestParseState::WAIT_ARGS;
                        }else{
                            state_=RpcRequestParseState::DONE;
                        }
                    }
                }

                String tempArgs_; // 用于临时保存args
                void parseArgs(UP<Buffer> &buffer){
                    // 从buffer中读取若干字节数据给tempArgs_
                    // 若干字节 = min(argsSize_-tempArgs_.length(),buffer->readableLength());
                    UInt x=std::min(rpcHeader_.argssize()-tempArgs_.length(),(UInt) buffer->readableLength());

                    tempArgs_+=buffer->subString(0,x);
                    buffer->advanceRead(x);

                    if(tempArgs_.size()>=rpcHeader_.argssize()){
                         request_->service=rpcHeader_.servicename();
                         request_->method=rpcHeader_.methodname();
                         request_->args=std::move(tempArgs_);
                         state_=RpcRequestParseState::DONE;
                    }
                }
                U32 headerSize_;
                FrameworkRequestHeader rpcHeader_;
                UP<RpcRequest> request_;
                RpcRequestParseState state_;
            public:
                RpcRequestParser(): tempHeaderSize_(_32BYTES_LEN), headerSize_(0){}
                Bool parse(UP<Buffer> &buffer){
                    while (buffer->hasMore()){
                        switch (state_) {
                            case RpcRequestParseState::START:
                                reset();
                                break;
                            case RpcRequestParseState::WAIT_HEADER_SIZE:
                                // 从中读取headerSize
                                parseHeaderSize(buffer);
                                break;
                            case RpcRequestParseState::WAIT_HEADER:
                                parseHeader(buffer);
                                break;
                            case RpcRequestParseState::WAIT_ARGS:
                                parseArgs(buffer);
                                break;
                            case RpcRequestParseState::DONE:
                                return TRUE;
                            case RpcRequestParseState::ERROR:
                                return FALSE;
                        }
                    }
                    return state_==RpcRequestParseState::DONE;
                }
                UP<RpcRequest> request(){
                    if(state_!=RpcRequestParseState::DONE){
                        throw CODE_LOCATION_EXCEPTION("request not ready");
                    }

                    UP<RpcRequest> rt=request_;
                    reset();
                    return rt;
                }
            };

        public:
            //todo 256个buffer
            RpcServiceProvider(): bufferPool_(new Channel<UP<Buffer>>(256)){
                for(Int i=0;i<256;i++){
                    bufferPool_->write(UP<Buffer> (new Buffer(1024)));
                }
            }
            void publishService(UP<RpcService> service){
                String serviceName=service->name();
                services_[serviceName]=SP<RpcService>(service);
            }

            void run(){
                //todo
                // 启动一个listener
                Int port=8888;
                ListenerSocket socket;
                socket.defaultInit("0.0.0.0",port);

                // 需要启动 read worker 和 writer worker
                UP<ConnectionSocket> connection;
                while(socket.accept(connection)){
                    SP<Channel<UP<RpcRequest>>> requests(new Channel<UP<RpcRequest>>(10));
                    SP<ConnectionSocket> spConnection(connection);
                    SP<ConnectionReadWorker> reader(new ConnectionReadWorker(spConnection,bufferPool_));
                    Fiber::AddTask([reader]{
                        reader->run();
                    },"provider reader");

                    SP<ConnectionWriteWorker> writer(new ConnectionWriteWorker(spConnection,bufferPool_));
                    Fiber::AddTask([writer]{
                        writer->run();
                    },"provider writer");

                    // 再启动 ServiceProvider worker,用于从 reader 中读取buffer,经过解析后,将结果写入rpc request channel
                    Fiber::AddTask([reader,requests]{
                        UP<Buffer> buffer;
                        RpcRequestParser parser;
                        while (reader->read(buffer)){
                            while (buffer->hasMore()){
                                if(parser.parse(buffer)){
                                    requests->write(parser.request());
                                }
                            }
                        }

                        // 退出循环时,代表reader被关闭了
                        requests->close();
                    },"provider request reader");

                    // 再启动 一个 worker,用于从 requests 中读取request,完成调用后,将结果写入writer
                    Fiber::AddTask([requests,writer, this]{
                        UP<RpcRequest> request;
                        while (requests->read(request)){
                            FrameworkResponseHeader responseHeader;
                            google::protobuf::Message* serviceReq= nullptr;
                            auto it = this->services_.find(request->service);
                            if(it==services_.end()){
                                LOG_WARN(gl)<<"no such service "<<request->service;
                                responseHeader.set_code(REQUEST_SERVICE_NOT_FOUND);
                                responseHeader.set_responsesize(0);
                                UP<Buffer> buffer;
                                bufferPool_->read(buffer);
                                //todo假设始终是成功的
                                LOG_ASSERT(gl,responseHeader.SerializeToArray(buffer->writeableAddr(),buffer->writeableLength()));
                                writer->write(buffer);
                                continue;
                            }

                            auto mit = it->second->methods_.find(request->method);
                            if (mit == it->second->methods_.end()) {
                                LOG_WARN(gl)<<"no such method in service "<<request->service;
                                responseHeader.set_code(REQUEST_METHOD_NOT_FOUND);
                                responseHeader.set_responsesize(0);
                                UP<Buffer> buffer;
                                bufferPool_->read(buffer);
                                //todo假设始终是成功的
                                LOG_ASSERT(gl,responseHeader.SerializeToArray(buffer->writeableAddr(),buffer->writeableLength()));
                                writer->write(buffer);
                                continue;
                            }

                            SP<RpcService> service = it->second;       // 获取service对象  new UserService
                            auto method=mit->second;

                            // 生成rpc方法调用的请求request和响应response参数,由于是rpc的请求，因此请求需要通过request来序列化
                            serviceReq = method->requestInstance();
                            if (!serviceReq->ParseFromString(request->args)) {
                                LOG_WARN(gl)<<"parse args failed at "<<request->service<<" "<<request->method<<" "<<request->args;
                                continue;   // 跳过这个request
                            }

                            google::protobuf::Message* serviceRes = method->responseInstance();
                            RpcCode code= method->_call(serviceReq, serviceRes);
                            responseHeader.set_code(code);
                            responseHeader.set_responsesize(serviceRes->ByteSizeLong());

                            // 我们实际应该先写入FrameworkResponseHeader的大小
                            UP<Buffer> buffer;
                            bufferPool_->read(buffer);

                            buffer->write32(responseHeader.ByteSizeLong());
                            //todo假设始终是成功的
                             LOG_ASSERT(gl,responseHeader.SerializeToArray(buffer->writeableAddr(),buffer->writeableLength()));
                             buffer->advanceWrite(responseHeader.ByteSizeLong());
                            if(responseHeader.responsesize()){
                                // 写入serviceRes
                                LOG_ASSERT(gl, serviceRes->SerializeToArray(buffer->writeableAddr(), buffer->writeableLength()));
                                buffer->advanceWrite(serviceRes->ByteSizeLong());
                            }
                            writer->write(buffer);
                        }

                        // 退出循环时,代表reader被关闭了
                        writer->stop();
                    },"provider service");
                }
            }
        };
    }


};