//
// Created by Liyihwa on 2024/10/30.
//

#ifndef LEARNING_LOGGER_H

#define LEARNING_LOGGER_H

/*
 * LogLevel规范:
 *      等级      说明                              示例场景
 * 1. DEBUG:     调试专用
 * 2. INFO:      普通的日志信息
 * 3. WARN:      警告信息,但方法还能继续执行            HTTP解析时,某个非关键参数为NULL
 * 4. ERROR:     错误信息,方法需要立即停止或者抛出异常    HTTP解析时,某个关键参数为NULL
 * 5. FATAL:     服务器错误,需要立马停止服务            数组越界等
 */

// 日志级别
enum class LogLevel{
    DEBUG=1,
    INFO=2,
    WARN=3,
    ERROR=4,
    FATAL=5
};

class LogFormatter{

};

class LogAppender{

};

class Logger{
   private:
    LogLevel logLevel_;
   public:
    void debug();
    void info();
    void warn();
    void error();
    void fatal();
};

#endif  // LEARNING_LOGGER_H
