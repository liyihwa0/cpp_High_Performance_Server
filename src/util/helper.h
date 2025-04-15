#pragma once
#include "src/global_def.h"
#include <fcntl.h>
#include <signal.h>
#include <cstring>
#include <csignal>
#include <cxxabi.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "string"
#include <sys/syscall.h> // 定义 SYS_gettid
#include <unistd.h>       // 定义 syscall 函数
#include <filesystem>

namespace wa{
    class Helper{
    public:
        static String Join(const Vector<String>& vec, const String& delimiter){
            String result;
            for (Int i = 0; i < vec.size(); i++) {
                if (i > 0) {
                    result += delimiter; // 添加分隔符
                }
                result += vec[i]; // 添加当前字符串
            }
            return result;
        }

        static Vector<String> Split(const String& str, const String& delimiter = " ") {
            Vector<String> tokens;
            size_t start = 0;
            size_t end = str.find(delimiter);

            while (end != String::npos) {
                tokens.push_back(str.substr(start, end - start));
                start = end + delimiter.length();
                end = str.find(delimiter, start);
            }

            // 添加最后一个 token
            tokens.push_back(str.substr(start, end));

            return tokens;
        }
        
        static ID GetThreadId() {
            return syscall(SYS_gettid);
        }
    
        Void static AddSignal(int sig,SignalHandler sigHandler){
            SignalAction sa;
            memset(&sa,'\0',sizeof(sa));
            sa.sa_handler= sigHandler;
            sigfillset(&sa.sa_mask);
            sigaction(sig,&sa,nullptr);
        }
    
        Int static SetNonBlocking(FileDescriptor fd) {
            int flags = fcntl(fd, F_GETFL, 0);
            if (flags == -1) return -1;
    
            flags |= O_NONBLOCK;
            return fcntl(fd, F_SETFL, flags);
        }
    
        static String DnsParse(const String&domain){
            struct addrinfo hints, *res;
            std::memset(&hints, 0, sizeof(hints));
            // 调用 getaddrinfo
            int ret = getaddrinfo(domain.c_str(), nullptr, &hints, &res);
            if (ret != 0) {
                return std::string("Error: ") + gai_strerror(ret);
            }
    
            // 遍历结果并获取第一个有效地址
            char ipstr[INET6_ADDRSTRLEN];
            for (struct addrinfo *p = res; p != nullptr; p = p->ai_next) {
                void *addr;
                if (p->ai_family == AF_INET) { // IPv4
                    struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
                    addr = &(ipv4->sin_addr);
                } else if (p->ai_family == AF_INET6) { // IPv6
                    struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
                    addr = &(ipv6->sin6_addr);
                } else {
                    continue; // Skip other address families
                }
    
                // 转换为字符串形式
                inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
                freeaddrinfo(res); // 释放 addrinfo 链表
                return std::string(ipstr);
            }
    
            freeaddrinfo(res);
            return "Error: No valid IP address found.";
        }
    
    
        template <typename T>
        static String TypeName(const T&var,Bool useDemangle=TRUE){
            if(useDemangle)
                return Demangle(typeid(T).name());
            else
                return typeid(T).name();
        }
    
        template <typename T>
        static String TypeName(const T&&var,Bool useDemangle=TRUE){
            if(useDemangle)
                return Demangle(typeid(T).name());
            else
                return typeid(T).name();
        }
    
    
        static Bool SetReuseAddr(FileDescriptor fd){
            // 设置端口复用
            Int opt = 1;
            return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1;
        }

        static String Pwd() {
            try {
                // 使用 std::filesystem 获取当前工作目录
                return std::filesystem::current_path().string();
            } catch (const std::exception& e) {
                return "Error: " + String(e.what()); // 错误处理
            }
        }
        
        static Bool IsSpaceChar(Char c){
            return std::isspace(c);
        }
        
    private:
        static std::string Demangle(const char* mangled_name) {
            int status = 0;
            // 使用智能指针管理内存，以确保在函数返回后自动释放
            std::unique_ptr<char, void(*)(void*)> demangled_name(
                    abi::__cxa_demangle(mangled_name, nullptr, nullptr, &status), std::free);
            // 如果成功解码，返回解码后的名称；否则返回原始的 mangled 名称
            return (status == 0) ? demangled_name.get() : mangled_name;
        }
    };

}


