#ifndef WA_EXCEPTION_H
#define WA_EXCEPTION_H
#include <exception>
#include <string>
#include <sstream>
#include <cstring>
#include <utility>
#include "src/global_def.h"

namespace wa {
    namespace expcetion{
           class CodeLocationException : public std::exception {
            protected:
                String file_;      // 文件名
                Int line_;         // 行号
                String function_;  // 函数名
                mutable String fullMessage_; // 缓存完整的错误消息，可变以支持 const
        
            public:
                // 构造函数
                template <typename... Args>
                CodeLocationException(const String& file,
                                      Int line,
                                      const String& function,
                                      Args&&... messages)
                        :fullMessage_(combineMessages( file+":"+std::to_string(line)+"("+function+")",std::forward<Args>(messages)...)) {}
        
                // 返回完整的错误消息
                virtual const char* what() const noexcept override {
                    if (fullMessage_.empty()) {
                        std::ostringstream oss;
                        oss << file_ << ":" << line_ << " (" << function_ << ")";
                        fullMessage_ = oss.str();
                    }
                    return fullMessage_.c_str();
                }
        
            protected:
                // 组合消息（带空格分隔）
                template <typename... Args>
                static std::string combineMessages(Args&&... messages) {
                    std::ostringstream oss;
                    ((oss << std::forward<Args>(messages) << " "), ...); // 折叠表达式
                    std::string result = oss.str();
                    if (!result.empty() && result.back() == ' ') {
                        result.pop_back(); // 去掉末尾多余的空格
                    }
                    return result;
                }
            };
            
            class OsException : public CodeLocationException {
            private:
                Int errno_; // 错误码
                mutable String fullMessage_;
            public:
                // 构造函数
                template <typename... Args>
                OsException(const String& file,
                            Int line,
                            const String& function,
                            int err_code,
                            Args&&... messages)
                        : CodeLocationException(file, line, function, std::forward<Args>(messages)...),
                          errno_(err_code) {}
        
                // 返回完整的错误消息
                const char* what() const noexcept override {
                    if (fullMessage_.empty()) {
                        std::ostringstream oss;
                        oss << CodeLocationException::what(); // 基类错误位置信息
                        oss << ": CODE(" << errno_ << "), " << strerror(errno_);
                        fullMessage_ = oss.str();
                    }
                    return fullMessage_.c_str();
                }
        
                // 返回错误码
                Int errCode() const noexcept { return errno_; }
            };

        class OutOfRangeException : public CodeLocationException {
        public:
            // 构造函数
            template <typename... Args>
            OutOfRangeException(const String& file,
                                Int line,
                                const String& function,
                                Args&&... messages)
                    : CodeLocationException(file, line, function, std::forward<Args>(messages)...) {}
        };

        class InvalidArgumentException : public CodeLocationException {
        public:
            // 构造函数
            template <typename... Args>
            InvalidArgumentException(const String& file,
                                     Int line,
                                     const String& function,
                                     Args&&... messages)
                    : CodeLocationException(file, line, function, std::forward<Args>(messages)...) {}
        };
        
        class FileNotFoundException:public CodeLocationException{
        public:
            // 构造函数
            template <typename... Args>
            FileNotFoundException(const String& file,
                                     Int line,
                                     const String& function,
                                     Args&&... messages)
                    : CodeLocationException(file, line, function, std::forward<Args>(messages)...) {}
        };
    }
} // namespace wa


// 宏定义
#define CODE_LOCATION_EXCEPTION(...) \
    wa::expcetion::CodeLocationException(__FILE__, __LINE__, __FUNCTION__ __VA_OPT__(,) __VA_ARGS__)

#define OS_ERRNO_EXCEPTION(...) \
    wa::expcetion::OsException( __FILE__, __LINE__, __FUNCTION__,errno __VA_OPT__(,) __VA_ARGS__)

#define OUT_OF_RANGE_EXCEPTION(...) \
    wa::expcetion::OutOfRangeException(__FILE__, __LINE__, __FUNCTION__ __VA_OPT__(,) __VA_ARGS__)
    
#define INVALID_ARGUMENT_EXCEPTION(...) \
    wa::expcetion::InvalidArgumentException(__FILE__, __LINE__, __FUNCTION__ __VA_OPT__(,) __VA_ARGS__)
    
#define FILE_NOT_FOUND_EXCEPTION(...) \
    wa::expcetion::FileNotFoundException(__FILE__, __LINE__, __FUNCTION__ __VA_OPT__(,) __VA_ARGS__)

#endif