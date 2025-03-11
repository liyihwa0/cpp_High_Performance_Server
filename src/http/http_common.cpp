#include <utility>

#include "./http_common.h"
#include "../global_def.h"
namespace wa {
    namespace http {


        String HttpHelper::StatusCodeToString(wa::http::HttpStatusCode code) {
            switch (code) {
                case HTTP_STATUS_CONTINUE:
                    return "100 Continue";
                case HTTP_STATUS_OK:
                    return "200 OK";
                case HTTP_STATUS_CREATED:
                    return "201 Created";
                case HTTP_STATUS_ACCEPTED:
                    return "202 Accepted";
                case HTTP_STATUS_MOVED_PERMANENTLY:
                    return "301 Moved Permanently";
                case HTTP_STATUS_FOUND:
                    return "302 Found";
                case HTTP_STATUS_BAD_REQUEST:
                    return "400 Bad Request";
                case HTTP_STATUS_NOT_FOUND:
                    return "404 Not Found";
                case HTTP_STATUS_INTERNAL_SERVER_ERROR:
                    return "500 Internal Server Error";
                default:
                    return "Unknown Status Code";
            }
        }

        String HttpHelper::MethodToString(wa::http::HttpMethod method) {
            switch (method) {
                case HttpMethod::GET:     return "GET";
                case HttpMethod::POST:    return "POST";
                case HttpMethod::PUT:     return "PUT";
                case HttpMethod::DELETE:  return "DELETE";
                case HttpMethod::HEAD:    return "HEAD";
                case HttpMethod::OPTIONS: return "OPTIONS";
                case HttpMethod::PATCH:   return "PATCH";
                case HttpMethod::TRACE:   return "TRACE";
                case HttpMethod::CONNECT: return "CONNECT";
                default:                  return "UNDEFINED"; // 未知方法
            }
        }
        // 初始化methodsMap_
       static std::unordered_map <String, HttpMethod> methodsMap_ {
                {"GET",     HttpMethod::GET},
                {"POST",    HttpMethod::POST},
                {"PUT",     HttpMethod::PUT},
                {"DELETE",  HttpMethod::DELETE},
                {"HEAD",    HttpMethod::HEAD},
                {"OPTIONS", HttpMethod::OPTIONS},
                {"PATCH",   HttpMethod::PATCH},
                {"TRACE",   HttpMethod::TRACE},
                {"CONNECT", HttpMethod::CONNECT}
        };
        HttpMethod HttpHelper::StringToMethod(const String &methodStr) {
            auto it = methodsMap_.find(methodStr);
            if (it != methodsMap_.end()) {
                return it->second;
            }
            return HttpMethod::UNDEFINED;
        }

    }
}
