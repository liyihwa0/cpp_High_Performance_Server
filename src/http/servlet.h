#pragma once
#include "./request.h"
#include "./response.h"
#include "unordered_map"
namespace wa {
    namespace http {

        class Servlet {
        public:
            virtual void handle(HttpRequest& request, HttpResponse& response) = 0;
        };

        class FunctionServlet : public Servlet {
        public:
            void handle(HttpRequest& request, HttpResponse& response) override{

            }
        };

        // 用于加载静态的
        class StaticServlet : public Servlet {
        private:
            String content_;
            String contentType_;
            // 根据文件扩展名来设置 Content-Type
            String getContentType(const String& filename) {
                static const std::unordered_map<String, String> mimeTypes = {
                        {".html", "text/html; charset=UTF-8"},
                        {".htm", "text/html; charset=UTF-8"},
                        {".css", "text/css"},
                        {".js", "application/javascript"},
                        {".json", "application/json"},
                        {".png", "image/png"},
                        {".jpg", "image/jpeg"},
                        {".jpeg", "image/jpeg"},
                        {".gif", "image/gif"},
                        {".svg", "image/svg+xml"},
                        {".txt", "text/plain; charset=UTF-8"}
                };

                // 获取文件扩展名
                size_t pos = filename.find_last_of('.');
                if (pos == String::npos) {
                    return "application/octet-stream";  // 默认二进制流
                }
                String extension = filename.substr(pos);

                // 查找并返回对应的 Content-Type
                auto it = mimeTypes.find(extension);
                if (it != mimeTypes.end()) {
                    return it->second;
                }
                return "application/octet-stream";  // 如果没有匹配，返回默认类型
            }
        public:
            StaticServlet(String filename){
                std::ifstream file(filename);
                if (!file.is_open()) {
                    throw CODE_LOCATION_EXCEPTION("File not fount "+ filename);
                }
                std::stringstream buffer;
                buffer << file.rdbuf();
                content_ = buffer.str();
                contentType_ = getContentType(filename);  // 设置文件类型
            }
            void handle(HttpRequest& request, HttpResponse& response) override{
                response.setStatus(HTTP_STATUS_OK);
                // 设置响应的头部信息
                response.setHeader("Content-Type", contentType_);
                response.setHeader("Content-Length",std::to_string(content_.length()));
                response.setBody(content_);
            }
        };

        class DispatchServlet: public Servlet{
            std::unordered_map<String,UP<Servlet>> servlets_;
            StaticServlet notFound_;
        public:
            DispatchServlet(): notFound_("./static/404.html"){}
            void scanDir(const String &path){
                namespace fs = std::filesystem;

                try {
                    // 遍历目录
                    for (const auto& entry : fs::directory_iterator(path)) {
                        if (fs::is_regular_file(entry.status())) {
                            // 获取文件的相对路径
                            String filePath = entry.path().string();

                            // 对每个文件创建一个 StaticServlet 实例，并注册到 servlets_
                            std::string fileName = entry.path().filename().string();
                            addServlet("/" + fileName, UP<Servlet>(new StaticServlet(filePath)));
                        }
                    }
                } catch (const fs::filesystem_error& e) {
                    // 处理文件系统错误
                    std::cerr << "Error scanning directory: " << e.what() << std::endl;
                }
            }
            void addServlet(const String & path, UP<Servlet> servlet){
                servlets_[path]=servlet;
            }

            void handle(HttpRequest& request, HttpResponse& response) override{
                auto it=servlets_.find(request.uri());
                if(it!=servlets_.end()){
                    it->second->handle(request,response);
                }else{
                    notFound_.handle(request,response);
                    response.setStatus(HTTP_STATUS_NOT_FOUND);
                }
            }
        };

    }
}
