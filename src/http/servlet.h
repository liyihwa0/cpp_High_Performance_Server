#pragma once

#include <filesystem>
#include "./request.h"
#include "./response.h"
#include "../data_structure/heap.h"
#include "unordered_map"
namespace wa {
    namespace http {

        class Servlet {
        public:
            virtual void handle(HttpRequest& request, HttpResponse& response) = 0;
        };

        class FunctionServlet : public Servlet {
        public:
            virtual void handle(HttpRequest& request, HttpResponse& response) = 0;
        };

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

        // 根据文件扩展名来设置 Content-Type
        static String getContentType(const String& filename) {
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

        void handleStaticRequest(String filename, HttpResponse& response){
            std::ifstream file(filename);
            if (!file.is_open()) {
                throw CODE_LOCATION_EXCEPTION("File not fount "+ filename);
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            response.setStatus(HTTP_STATUS_OK);
            // 设置响应的头部信息
            response.setHeader("Content-Type", getContentType(filename));
            response.setHeader("Content-Length",std::to_string(buffer.str().length()));
            response.setBody(buffer.str());
        }

        void handleStaticRequest(String filename,const String& filecontent, HttpResponse& response){

            response.setStatus(HTTP_STATUS_OK);
            // 设置响应的头部信息
            response.setHeader("Content-Type", getContentType(filename));
            response.setHeader("Content-Length",std::to_string(filecontent.length()));
            response.setBody(filecontent);
        }

        // 用于加载静态的请求
        class StaticServlet : public Servlet {
        private:
            String content_;
            String contentType_;

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

//        class CachedServlet: public Servlet{
//            enum class FileStatus{
//                NOT_CACHED=1,
//                CACHED=2,
//                TOO_LARGE=3,
//                NOT_FOUND=4
//            };
//            struct FileNode {
//                String name_;
//                String content_;
//                Timer lastVisit_;
//                FileStatus status_;
//                FileNode(FileStatus status):status_(status){
//
//                }
//            };
//
//
//
//            class FileNodeComparator{
//            public:
//                static Bool Compare(const FileNode* a, const FileNode* b) {
//                    return a->lastVisit_ < b->lastVisit_;
//                }
//            };
//
//            Size maxCacheSize_;
//            Size maxFileSize_;
//            String baseDir_;
//            std::unordered_map<String,FileNode*> files_;
//            // 定义一个 LRU 缓存：文件路径作为键，文件内容及其类型作为值
//            wa::Heap<FileNode*,FileNodeComparator> lruSet_;
//
//        public:
//            CachedServlet(String baseDir,
//                          Size maxCacheSize,
//                          Size maxFileSize):
//                          baseDir_(std::move(baseDir)),
//                          maxCacheSize_(maxCacheSize),
//                          maxFileSize_(maxFileSize),
//                          lruSet_(maxCacheSize){
//            }
//            void handle(wa::http::HttpRequest &request, wa::http::HttpResponse &response) override{
//                String filepath=request.uri();
//                // 判断文件状态
//                auto fd= files_.find(filepath);
//                if(fd==files_.end()){
//                    // 读文件
//                    String fullFilePath = baseDir_ + "/" + filepath;
//
//                    // 检查文件是否存在
//                    std::ifstream file(fullFilePath, std::ios::binary);
//                    if (!file.is_open()) {
//                        return;  // 文件不存在,直接返回
//                    }
//
//                    file.seekg(0, std::ios::end);
//                    Size fileSize = file.tellg();
//                    file.seekg(0, std::ios::beg);
//
//                    if (fileSize > maxFileSize_) {
//                        files_[filepath] = new FileNode(FileStatus::TOO_LARGE);  // 标记文件为过大
//                        StaticServlet(filepath).handle(request,response);
//                        return;
//                    }
//                }
//
//                switch (fd->second->status_) {
//                    case FileStatus::NOT_FOUND:
//                        return;
//                    case FileStatus::TOO_LARGE:
//                        StaticServlet(filepath).handle(request,response);
//                        return ;
//                    case FileStatus::NOT_CACHED:
//                        // 文件不在缓存中，读取文件
//                        String fullFilePath = baseDir_ + "/" + filepath;
//
//                        // 检查文件是否存在
//                        std::ifstream file(fullFilePath, std::ios::binary);
//                        if (!file.is_open()) {
//                            return;  // 文件不存在直接返回
//                        }
//                        std::stringstream buffer;
//                        buffer << file.rdbuf();
//                        String content = buffer.str();
//                        // 缓存文件内容
//                        FileNode* fileNode=new FileNode(FileStatus::CACHED);
//                        fileNode->name_ = filepath;
//                        fileNode->content_ = content;
//                        fileNode->lastVisit_ = Timer::Now();  // 设置文件的最近访问时间
//
//                        // 检查缓存是否满了，如果满了，进行 LRU 替换
//                        if (lruSet_.size() >= maxCacheSize_) {
//                            // 使用 LRU 算法移除最久未访问的文件
//                            auto leastUsedFile = lruSet_.top();
//                            lruSet_.pop();
//                            files_.erase(leastUsedFile->name_);
//                        }
//
//                        // 将文件添加到缓存
//                        lruSet_.add(fileNode);  // 假设 Heap 类有 push 方法
//                        files_[filepath] = fileNode;  // 标记文件为已缓存
//                    case FileStatus::CACHED:
//                        auto cachedFileNode=files_[filepath];
//                        handleStaticRequest(cachedFileNode->name_,cachedFileNode->content_,response);
//                        return;
//                }
//
//            };
//
//        };

        class DispatchServlet: public Servlet{
            std::unordered_map<String,SP<Servlet>> servlets_;
            StaticServlet notFoundPage_;
            StaticServlet indexPage_;


        public:
            DispatchServlet(
                    Size maxCacheSize = 10,
                            Size maxFileSize=10*1024): notFoundPage_("./static/404.html"),
                                                       indexPage_("./static/index.html"){}
                                                       

            void scanStaticDir(const String &path){
                namespace fs = std::filesystem;

                try {
                    // 遍历目录
                    for (const auto& entry : fs::directory_iterator(path)) {
                        if (fs::is_regular_file(entry.status())) {
                            // 获取文件的相对路径
                            String filePath = entry.path().string();

                            // 对每个文件创建一个 StaticServlet 实例，并注册到 servlets_
                            std::string fileName = entry.path().filename().string();

                            LOG_INFO(gl)<<filePath<<" load";
                            
                            //servlets_["/"+fileName] =SP<StaticServlet>(new StaticServlet(filePath));//servlets_;
                        }
                    }
                } catch (const fs::filesystem_error& e) {
                    // 处理文件系统错误
                    std::cerr << "Error scanning directory: " << e.what() << std::endl;
                }
            }

            void addServlet(const String & path, SP<Servlet> servlet){
                servlets_[path]=std::move(servlet);
            }

            void handle(HttpRequest& request, HttpResponse& response) override{

                auto it=servlets_.find(request.uri());
                if(it!=servlets_.end()){
                    it->second->handle(request,response);
                }else{
                    notFoundPage_.handle(request, response);
                    response.setStatus(HTTP_STATUS_NOT_FOUND);
                }
            }
        };

    }
}
