#pragma once
#include "../global_def.h"
#include "./http_common.h"
#include <string>
#include <map>

namespace wa {
    namespace http {
        class HttpResponse {
        private:
            String version_;
            HttpStatusCode statusCode_;
            std::map<String, String> headers_;
            std::map<String, String> cookies_;
            String body_;

        public:
            HttpResponse() : version_("HTTP/1.1"), statusCode_(HTTP_STATUS_OK) {}

            void setVersion(const String& version){
                version_=version;
            }
            void setStatus(HttpStatusCode code){
                statusCode_=code;
            }
            HttpStatusCode getStatus() const{
                return statusCode_;
            }
            String getStatusMessage() const{
                return HttpHelper::StatusCodeToString(statusCode_);
            }
            void setHeader(const String & key, const String& value){
                headers_[key]=value;
            }
            String header(const String& key, const String& _default = "") const{
                auto it = headers_.find(key);
                return (it != headers_.end()) ? it->second : _default;
            }
            void delHeader(const String& key){
                headers_.erase(key);
            }
            Bool hasHeader(const String& key) const{
                return headers_.find(key)!=headers_.end();
            }
            void setCookie(const String& key, const String& value){
                cookies_[key]=value;
            }
            String cookie(const String& key, const String& _default = "") const{
                auto it = cookies_.find(key);
                return (it != cookies_.end()) ? it->second : _default;
            }
            void delCookie(const String& key){
                cookies_.erase(key);
            }
            bool hasCookie(const String& key) const{
                return cookies_.find(key)!=cookies_.end();
            }
            void setBody(const String& body){
                body_=body;
            }
            const String& getBody() const{
                return body_;
            }
            Bool hasBody() const{
                return !body_.empty();
            }

            Bool keepAlive() const{
                auto it=headers_.find("Connection");
                return it!=headers_.end()&&it->second=="keep-alive";
            }

            void setKeepAlive(Bool flag) {
                if (flag) {
                    setHeader("Connection", "keep-alive");
                } else {
                    setHeader("Connection", "close");
                }
            }

            String toString() const{
                String response = version_ + " " + getStatusMessage() + "\r\n";
                for (const auto& header : headers_) {
                    response += header.first + ": " + header.second + "\r\n";
                }
                response += "\r\n";
                response += body_;
                return response;
            }

            static UP<HttpResponse> SUCCESS(){
                UP<HttpResponse> response(new HttpResponse());
                response->setHeader("Content-Type", "text/plain");
                return response;
            }
        };

    }
}
