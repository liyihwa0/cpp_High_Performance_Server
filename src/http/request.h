#pragma once
#include "./http_common.h"
#include "../global_def.h"
#include <string>
#include <map>

namespace wa {
    namespace http {

        // HttpRequest类
        class HttpRequest {
        friend class HttpRequestParser;
        private:
            HttpMethod method_;
            String uri_;
            String version_;
            std::map<String, String> headers_;
            std::map<String, String> params_;
            std::map<String,String> cookies_;
            String body_;

        public:
            HttpRequest(){

            }
            Bool keepAlive() const{
                auto it=headers_.find("Connection");
                return it!=headers_.end()&&it->second=="keep-alive";
            }
            String header(const String& key, const String& default_ = "") const{
                auto it = headers_.find(key);
                return (it != headers_.end()) ? it->second : default_;
            }
            String param(const String& key, const String& default_ = "") const{
                auto it = params_.find(key);
                return (it != params_.end()) ? it->second : default_;
            }
            String cookie(const String& key, const String& default_ = "") const{
                auto it = cookies_.find(key);
                return (it != cookies_.end()) ? it->second : default_;
            }
            const String& body() const{
                return body_;
            }
            const String& uri() const{
                return uri_;
            }
            const String& version() const{
                return version_;
            }
            String methodString() const{
                return  HttpHelper::MethodToString(method_);
            }
            void delHeader(const String& key){
                headers_.erase(key);
            }
            void delParam(const String& key){
                params_.erase(key);
            }
            void delCookie(const String& key){
                cookies_.erase(key);
            }
            Bool hasHeader(const String& key) const{
                return headers_.find(key) != headers_.end();
            }
            Bool hasParam(const String& key) const{
                return params_.find(key) != params_.end();
            }
            Bool hasCookie(const String& key) const{
                return cookies_.find(key) != cookies_.end();
            }

            String toString()const{
                // 组装请求行：method, uri, version
                String requestString = methodString() + " " + uri_ + " " + version_ + "\r\n";

                // 添加请求头部
                for (const auto& header : headers_) {
                    requestString += header.first + ": " + header.second + "\r\n";
                }

                // 如果请求体不为空，添加一个空行和请求体内容
                if (!body_.empty()) {
                    requestString += "\r\n" + body_;
                } else {
                    // 如果没有请求体，添加一个空行
                    requestString += "\r\n";
                }

                return requestString;
            }
        };

    }
}
