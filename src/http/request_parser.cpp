#include "./request_parser.h"
#include <iostream>

namespace wa {
    namespace http {

        // HttpRequestParser 类函数实现
        void HttpRequestParser::init() {
            request_.reset(new HttpRequest());
            currentState_ = HttpParseState::WAIT_METHOD;
            remainingContentLength_=0;
            method_.clear();
            headerKey_.clear();
            headerVal_.clear();
        }

        HttpRequestParser::HttpRequestParser(SP<Channel<UP<wa::Buffer>>> contentChannel,
                                             SP<Channel<UP<wa::Buffer>>> emptyChannel,
                                             SP<Channel<UP<wa::http::HttpRequest>>> output): contentBufferChannel_(std::move(contentChannel)),
                                                                                             recycleBufferChannel_(std::move(emptyChannel)),
                                                                                             httpRequestChannel_(std::move(output)),
                                                                                             currentState_(HttpParseState::START),
                                                                                             request_(),
                                                                                             method_(),
                                                                                             headerKey_(),
                                                                                             headerVal_()
        {}



        // HttpRequestParser 类的继续实现
        void HttpRequestParser::parseMethod(const UP<Buffer>& buffer) {

            LOG_ASSERT(gl,currentState_==HttpParseState::WAIT_METHOD);

            while (buffer->hasMore()){
                Char c=buffer->pop();
                if(c==' '){
                    // 当遇到空格时，说明方法解析完毕，检查是否是有效的 HTTP 方法
                    HttpMethod method = HttpHelper::StringToMethod(method_);
                    if (method == HttpMethod::UNDEFINED) {
                        currentState_ = HttpParseState::ERROR;  // 无效方法，进入错误状态
                    } else {
                        request_->method_ = method;  // 保存方法到 HttpRequest 中
                        currentState_ = HttpParseState::WAIT_URI;  // 进入解析 URI 的状态
                        return;
                    }
                } else {
                    // 如果还不是空格，继续解析方法
                    method_.push_back(c);
                }
            }
        }

        void HttpRequestParser::parseURI(const UP<Buffer>& buffer) {

            LOG_ASSERT(gl,currentState_==HttpParseState::WAIT_URI);

            while (buffer->hasMore()){
                Char c=buffer->pop();
                if(c==' '){
                    currentState_ = HttpParseState::WAIT_VERSION;  // 进入解析VERSION的状态
                    return;
                } else {
                    // 如果还不是空格，继续解析URI
                    request_->uri_.push_back(c);
                }
            }
        }

        void HttpRequestParser::parseVersion(const UP<Buffer>& buffer) {

            LOG_ASSERT(gl,currentState_==HttpParseState::WAIT_VERSION);

            while (buffer->hasMore()){
                Char c=buffer->pop();
                if(c=='\r'){
                    currentState_ = HttpParseState::WAIT_CRLF;
                    return;
                }else{
                    //不是换行符,继续解析version
                    request_->version_.push_back(c);
                }
            }
        }

        void HttpRequestParser::parseCRLF(const UP<Buffer>& buffer) {

            LOG_ASSERT(gl,currentState_==HttpParseState::WAIT_CRLF);

            while (buffer->hasMore()){
                Char c=buffer->pop();
                if(c=='\n'){
                    currentState_=HttpParseState::WAIT_HEADER_KEY;
                    return;
                }else{
                    currentState_ = HttpParseState::ERROR;  // 无效进入错误状态
                    return;
                }
            }
        }

        void HttpRequestParser::parseHeaderKey(const UP<Buffer>& buffer){
            LOG_ASSERT(gl,currentState_==HttpParseState::WAIT_HEADER_KEY);
            while (buffer->hasMore()){
                Char c=buffer->pop();
                if(c=='\r'){
                    // 这里需要帮助 body的解析器记录content-readableLength
                    auto cl=request_->headers_.find("Content-Length");
                    if(cl!=request_->headers_.end()){
                        remainingContentLength_ = std::stoi(cl->second);
                    }
                    currentState_=HttpParseState ::WAIT_EMPTY_LINE;

                    return;
                }
                else if(c==':'){
                    // 当遇到:时，说明key解析完毕
                    currentState_ = HttpParseState::WAIT_HEADER_VALUE;
                    return;
                } else{
                    //不是":",继续解析key
                    headerKey_.push_back(c);
                }
            }
        }

        void HttpRequestParser::parseHeaderValue(const UP<Buffer>& buffer){
            LOG_ASSERT(gl,currentState_==HttpParseState::WAIT_HEADER_VALUE);
            while (buffer->hasMore()){
                Char c=buffer->pop();
                if(c=='\r'){
                    // header 的k,v都解析完毕,记录他们
                    request_->headers_[std::move(headerKey_)]=std::move(headerVal_);
                    currentState_=HttpParseState ::WAIT_CRLF;
                    return;
                }else {
                    //继续解析value
                    headerVal_.push_back(c);
                }
            }
        }

        void HttpRequestParser::parseEmptyLine(const UP<Buffer>& buffer){
            LOG_ASSERT(gl,currentState_==HttpParseState::WAIT_EMPTY_LINE);
            while (buffer->hasMore()) {
                Char c=buffer->pop();
                if(c!='\n'){
                    currentState_=HttpParseState::ERROR;
                    return;
                }
                if(remainingContentLength_==0){

                    httpRequestChannel_->write(std::move(request_));
                    currentState_=HttpParseState ::DONE;
                    return;
                }else{
                    currentState_=HttpParseState::WAIT_BODY;
                    return;
                }
            }
        }

        //todo 这里需要判断客户端采用Content-Length方式,Transfer-Encoding: chunked,还是无body的方式来传输报文
        // 目前只处理Content-Length这种
        void HttpRequestParser::parseBody(const UP<wa::Buffer> &buffer) {
            LOG_ASSERT(gl,currentState_==HttpParseState::WAIT_BODY);

            while (remainingContentLength_!=0&&buffer->hasMore()) {
                request_->body_.push_back(buffer->pop());
                remainingContentLength_--;
            }
            if(remainingContentLength_==0){
                httpRequestChannel_->write(std::move(request_));
                currentState_=HttpParseState ::DONE;
                return;
            }
        }


        HttpParseState HttpRequestParser::input(const UP<Buffer>& buffer) {
            while (buffer->hasMore()){
                switch (currentState_) {
                    case HttpParseState::WAIT_METHOD:
                        parseMethod(buffer);
                        break;
                    case HttpParseState::WAIT_URI:
                        parseURI(buffer);
                        break;
                    case HttpParseState::WAIT_VERSION:
                        parseVersion(buffer);
                        break;
                    case HttpParseState::WAIT_CRLF:
                        parseCRLF(buffer);
                        break;
                    case HttpParseState::WAIT_HEADER_KEY:
                        parseHeaderKey(buffer);
                        break;
                    case HttpParseState::WAIT_HEADER_VALUE:
                        parseHeaderValue(buffer);
                        break;
                    case HttpParseState::WAIT_EMPTY_LINE:
                        parseEmptyLine(buffer);
                        break;
                    case HttpParseState::WAIT_BODY:
                        parseBody(buffer);
                        break;

                    case HttpParseState::START:
                    case HttpParseState::DONE:
                        init();
                        input(buffer);
                        break;
                    default:
                        return currentState_;
                }
            }
            return currentState_;
        }

        void HttpRequestParser::run() {
            while (TRUE){
                UP<Buffer> buffer;

                if(!contentBufferChannel_->read(buffer)){ // 没有内容可读了
                    break;
                }

                input(buffer);
                // 执行完input后,buffer一定空了

                recycleBufferChannel_->write(buffer);
            }
            httpRequestChannel_->close();
        }

    }
}
