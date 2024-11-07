// 使用 有限状态机解析http报文
#include "global_def.h"
#include<iostream>
#include <map>
#include <cstring>
#include "buffer.h"
#include<cstdio>
typedef enum HttpStatusCode {
    // 1xx Informational
    HTTP_STATUS_CONTINUE = 100,
    HTTP_STATUS_SWITCHING_PROTOCOLS = 101,
    HTTP_STATUS_PROCESSING = 102,
    
    // 2xx Success
    HTTP_STATUS_OK = 200,
    HTTP_STATUS_CREATED = 201,
    HTTP_STATUS_ACCEPTED = 202,
    HTTP_STATUS_NO_CONTENT = 204,
    HTTP_STATUS_PARTIAL_CONTENT = 206,
    
    // 3xx Redirection
    HTTP_STATUS_MOVED_PERMANENTLY = 301,
    HTTP_STATUS_FOUND = 302,
    HTTP_STATUS_SEE_OTHER = 303,
    HTTP_STATUS_NOT_MODIFIED = 304,
    HTTP_STATUS_TEMPORARY_REDIRECT = 307,
    HTTP_STATUS_PERMANENT_REDIRECT = 308,

    // 4xx Client Errors
    HTTP_STATUS_BAD_REQUEST = 400,
    HTTP_STATUS_UNAUTHORIZED = 401,
    HTTP_STATUS_FORBIDDEN = 403,
    HTTP_STATUS_NOT_FOUND = 404,
    HTTP_STATUS_METHOD_NOT_ALLOWED = 405,
    HTTP_STATUS_REQUEST_TIMEOUT = 408,
    HTTP_STATUS_CONFLICT = 409,
    HTTP_STATUS_GONE = 410,
    HTTP_STATUS_PAYLOAD_TOO_LARGE = 413,
    HTTP_STATUS_URI_TOO_LONG = 414,
    HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE = 415,
    HTTP_STATUS_TOO_MANY_REQUESTS = 429,
    
    // 5xx Server Errors
    HTTP_STATUS_INTERNAL_SERVER_ERROR = 500,
    HTTP_STATUS_NOT_IMPLEMENTED = 501,
    HTTP_STATUS_BAD_GATEWAY = 502,
    HTTP_STATUS_SERVICE_UNAVAILABLE = 503,
    HTTP_STATUS_GATEWAY_TIMEOUT = 504,
    HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED = 505
} HttpStatusCode;

// HTTP解析的状态机状态
typedef enum class HttpParseState {
    REQUEST_LINE,  // 解析请求行
    HEADERS,       // 解析头部
    BODY,          // 解析请求体
    DONE,          // 解析完成
    ERROR          // 解析错误
}HttpParseState;


typedef enum HttpParseEvent{
    HTTP_PARSE_NEED_MORE,               // 请求不完整，需要继续读取客户数据
    HTTP_PARSE_OK,                      // 表示获得了一个完成的客户请求
    HTTP_PARSE_BAD_REQUEST,            // 表示客户请求语法错误
    HTTP_PARSE_INTERNAL_ERROR,         // 表示服务器内部错误
    HTTP_PARSE_CLOSED_CONNECTION       // 表示客户端已经关闭连接
}HttpParseEvent;

typedef struct HttpRequest{
    std::string method;
    std::string uri;
    std::string version;
    std::map<std::string,std::string> headers;
    std::string body;
};


class HttpParser{
private:
    HttpRequest * request_;
    HttpParseState state_;    // 当前状态

    Bool parseRequestLine(const Buffer& buffer) {
        // 从 buffer 中提取请求行的子串
        Size startIndex= buffer.startIndex();
        Size spaceIndex= buffer.find(' ');
        if(spaceIndex== buffer.endIndex()){
            return FALSE;           // 没有找到空格，格式错误
        }
        request_->method.assign((char *) buffer.getBuffer() + startIndex, spaceIndex - startIndex);
        
        // 再次寻找下一个space
        startIndex=spaceIndex+1;
        spaceIndex= buffer.find(' ', spaceIndex + 1);
        if(spaceIndex== buffer.endIndex()){
            return FALSE;           // 没有找到空格，格式错误
        }
        request_->uri.assign((char *) buffer.getBuffer() + startIndex, spaceIndex - startIndex);

        // 读取剩余的作为HTTP version
        request_->version.assign((char *) buffer.getBuffer() + startIndex + 1, buffer.endIndex() - startIndex - 1);
        return TRUE;
    }

    Bool parseHeader(const std::string& line) {
        std::size_t pos = line.find(": ");
        if (pos == std::string::npos) {
            std::cerr << "Error parsing header: " << line << std::endl;
            return false;
        }
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 2);
        request_->headers[key] = value;
        return true;
    }

    bool parseBody(const std::string &line){
        
    }


    ~HttpParser(){}

public:    

    HttpParser() : state_(HttpParseState::REQUEST_LINE) {}

     HttpParseEvent Parse(Buffer& buffer) {
        Size end= buffer.endIndex();
        Size start= buffer.startIndex();
        Size idx= buffer.startIndex();
        while (idx<end) {
            switch (state_) {
                case HttpParseState::REQUEST_LINE: {
                    // ptr右移直到遇到 \r\n
                    while(idx<end-1){
                        if(buffer[idx]=='\r'&&buffer[idx+1]=='\n') break;
                        ++idx;
                    }
                    
                    if(!(buffer[idx]=='\r'&&buffer[idx+1]=='\n')){  // 请求行不完整,在缓冲区(buffer)足够大时,不该出现这种情况
                            // 返回 HTTP_PARSE_BAD_REQUEST , 状态为 HTTP_STATUS_URI_TOO_LONG
                            //todo
                            return HTTP_PARSE_BAD_REQUEST;
                    }
                    

                    // 从start到idx-1,交给 parseRequestLine处理
                    if (!parseRequestLine(Buffer::Wrap(buffer.getBuffer() + idx, idx - 1 - start))) {
                        state_ = HttpParseState::ERROR;
                        return HTTP_PARSE_BAD_REQUEST;
                    }
                    state_ = HttpParseState::HEADERS;
                    break;
                }

                case HttpParseState::HEADERS: {
                    pos = data.find("\r\n\r\n");
                    if (pos == std::string::npos) {
                        return HTTP_PARSE_NEED_MORE;  // 头部不完整
                    }
                    std::string headersPart = data.substr(0, pos + 2);
                    data.erase(0, pos + 4);  // 移除头部及终止符

                    std::istringstream headerStream(headersPart);
                    std::string line;
                    while (std::getline(headerStream, line) && line != "\r") {
                        if (!parseHeader(line)) {
                            state_ = HttpParseState::ERROR;
                            return HTTP_PARSE_BAD_REQUEST;
                        }
                    }

                    if (contentLength > 0) {
                        state_ = HttpParseState::BODY;
                    } else {
                        state_ = HttpParseState::DONE;
                        return HTTP_PARSE_OK;
                    }
                    break;
                }

                case HttpParseState::BODY: {
                    if (parseBody(data.c_str(), data.size())) {
                        state_ = HttpParseState::DONE;
                        return HTTP_PARSE_OK;  // 完成解析
                    }
                    return HTTP_PARSE_NEED_MORE;  // 请求体不完整
                }

                case HttpParseState::DONE: {
                    return HTTP_PARSE_OK;  // 解析已完成
                }

                case HttpParseState::ERROR: {
                    return HTTP_PARSE_BAD_REQUEST;  // 解析失败
                }
            }
        }

        return HTTP_PARSE_NEED_MORE;  // 等待更多数据
    }
};