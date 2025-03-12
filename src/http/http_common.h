#pragma once

#include <memory>

#include "../global_def.h"
#include "map"
#include "../smart_ptr.h"
#include "../fiber/channel.h"
#include "../buffer.h"

namespace wa {
    namespace http {
        // HTTP请求方法枚举
        typedef enum HttpMethod {
            UNDEFINED = 0,
            GET,
            POST,
            PUT,
            DELETE,
            HEAD,
            OPTIONS,
            PATCH,
            TRACE,
            CONNECT
        } HttpMethod;
        // HTTP状态码枚举
        typedef enum HttpStatusCode {
            HTTP_STATUS_CONTINUE = 100,
            HTTP_STATUS_OK = 200,
            HTTP_STATUS_CREATED = 201,
            HTTP_STATUS_ACCEPTED = 202,
            HTTP_STATUS_MOVED_PERMANENTLY = 301,
            HTTP_STATUS_FOUND = 302,
            HTTP_STATUS_BAD_REQUEST = 400,
            HTTP_STATUS_NOT_FOUND = 404,
            HTTP_STATUS_INTERNAL_SERVER_ERROR = 500,
            HTTP_STATUS_BAD_GATEWAY=502
            // 可以继续添加其他状态码
        } HttpStatusCode;

        class HttpHelper{
        public:
            // 将字符串转换为 HttpMethod
            static HttpMethod StringToMethod(const String& methodStr);
            static String MethodToString(HttpMethod method);
            // 状态码转换为字符串
            static String StatusCodeToString(HttpStatusCode code);
        };


    }
}