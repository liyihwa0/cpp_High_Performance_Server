//
// Created by Liyihwa on 2024/10/30.
//
#pragma once
#include <utility>

#include "../global_def.h"
#include "../exception/exception.h"
#include "../locker/locker.h"
#include "../smart_ptr.h"
#include "../timer.h"
#include "../thread/thread.h"
#include "../fiber/fiber.h"

namespace wa {
    class Logger;
    class LogLevel;
    class LogAppender;
    class LogFormatter;
    /*
     * LogLevel规范:
     *      等级      说明                              示例场景
     * 1. DEBUG:     调试专用
     * 2. INFO:      普通的日志信息
     * 3. WARN:      警告信息,但方法还能继续执行            HTTP解析时,某个非关键参数为NULL
     * 4. ERROR:     错误信息,该方法需要立即停止或者抛出异常   HTTP解析时,某个关键参数为NULL
     * 5. FATAL:     服务器错误,需要立马停止服务            数组越界等
     */
    class LogLevel {
    public:
        enum Level {
            UNKNOW = 0,
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5
        };

        static const char* ToString(LogLevel::Level level);
    };

    class LogEvent {
    public:
        LogEvent(SP<Logger> logger, LogLevel::Level level, I64 elapsed, const String& file, U32 line, UP<std::stringstream> message);
        SP<Logger> logger_;
        LogLevel::Level level_;
        I64 elapsed_;
        String file_;
        U32 line_;
        UP<std::stringstream> message_;
        Timer time_;
        String threadName_;
        ID threadId_;
        String fiberName_;
        ID fiberId_;
    };



    /**
        * @brief 日志格式化器
        * @details
        *  %m  消息
        *  %p  日志级别
        *  %r  累计毫秒数
        *  %c  日志名称
        *  %t 线程id
        *  %N 线程名称
        *  %f 协程id
        *  %d  时间
        *  %F 文件名
        *  %l 行号
        *  默认格式 "%d{%Y-%m-%d %H:%M:%S}\t%t\t%N\t%f\t%F:%l\t[\t%c-\t%p]\t%m\n"
        */
    class LogFormatter {
    private:
        // 定义抽象基类 FormatItem
        class FormatItem {
        public:
            virtual ~FormatItem() = default;
            virtual void write(OStream &stream, const LogEvent &logEvent)const = 0;
        };

        class  DateTimeFormatItem : public FormatItem{
        public:
            explicit DateTimeFormatItem(const String &format = "%Y-%m-%d %H:%M:%S");
            void write(OStream &stream, const LogEvent &logEvent)const override;
        private:
            String format_;
        };
        class MessageFormatItem : public FormatItem {
        public:
            void write(OStream &stream, const LogEvent &logEvent)const override;
        };
        class LogLevelFormatItem : public FormatItem {
        public:
            void write(OStream &stream, const LogEvent &logEvent)const override;
        };

        class ElapsedTimeFormatItem : public FormatItem {
        public:
            void write(OStream &stream, const LogEvent &logEvent)const override;
        };

        class LoggerNameFormatItem : public FormatItem {
        public:
            void write(OStream &stream, const LogEvent &logEvent)const override;
        };

        class ThreadIdFormatItem : public FormatItem {
        public:
            void write(OStream &stream, const LogEvent &logEvent)const override;
        };

        class ThreadNameFormatItem : public FormatItem {
        public:
            void write(OStream &stream, const LogEvent &logEvent)const override;
        };

        class FiberIdFormatItem : public FormatItem {
        public:
            void write(OStream &stream, const LogEvent &logEvent)const override;
        };

        class FiberNameFormatItem : public FormatItem {
        public:
            void write(OStream &stream, const LogEvent &logEvent)const override;
        };

        class FilenameFormatItem : public FormatItem {
        public:
            void write(OStream &stream, const LogEvent &logEvent)const override;
        };

        class LineNumberFormatItem : public FormatItem {
        public:
            void write(OStream &stream, const LogEvent &logEvent)const override;
        };

        class OriginTextFormatItem : public FormatItem {
        public:
            explicit OriginTextFormatItem(UP<String> text);
            void write(OStream &stream, const LogEvent &logEvent)const override;

        private:
            UP<String> text_;
        };

        std::vector<UP<FormatItem>> formatItems_;
    public:
        explicit LogFormatter(const String &fmt="%d{%Y-%m-%d %H:%M:%S}\t[%c\t%p]\t%F:%l\t(%t,%f)\t %m\n");
        void reset(const String& fmt);
        UP<std::stringstream> formattedStream(const LogEvent & logEvent);
    };

    class LogAppender {
    public:
        virtual ~LogAppender() = default;
        virtual OStream & lockStream() = 0;
        virtual void unlockStream() = 0;
    };

    class FileLogAppender : public LogAppender {
    public:
        explicit FileLogAppender(const std::string& filename);
        ~FileLogAppender() override;
        OStream & lockStream() override;
        void unlockStream() override;

    private:
        OFStream fileStream_;
        String filename_;
        SpinLock lock_;
    };

    class StdLogAppender : public LogAppender {
    public:
        explicit StdLogAppender();
        ~StdLogAppender() override;
        OStream & lockStream() override;
        void unlockStream() override;
    };

    class Logger {
    public:
        explicit Logger(LogLevel::Level level, UP<LogFormatter> logFormatter = UP<LogFormatter>(new LogFormatter()), const String &name = "Default Logger",SP<LogAppender> appender=SP<LogAppender>(nullptr));

        void write(const LogEvent& logEvent);
        Bool judgeLevel(LogLevel::Level level) const;
        const String& name() const;
        Logger& setLevel(LogLevel::Level level);
        Logger& addAppender(SP<LogAppender> appender);
        Logger& setFormatter(UP<LogFormatter> logFormatter);
        Logger& setName(const String& name);
        ~Logger();

    private:
        std::vector<SP<LogAppender>> appender_;
        LogLevel::Level logLevel_;
        UP<LogFormatter> logFormatter_;
        String name_;
    };

    class LogEventWrapper {
    public:
        LogEventWrapper(SP<Logger> logger, LogLevel::Level level, I64 elapsed, const String& file, U32 line);
        ~LogEventWrapper();
        std::stringstream& stream();

    private:
        UP<LogEvent> logEvent_;
    };

    // 全局日志器
    extern  SP<Logger> gl;
}



#define LOG_LEVEL(logger,level) \
    if(logger->judgeLevel(level)) \
        wa::LogEventWrapper(logger,level,0,__FILE__,__LINE__).stream()

#define LOG_DEBUG(logger) LOG_LEVEL(logger,LogLevel::Level::DEBUG)
#define LOG_INFO(logger) LOG_LEVEL(logger,LogLevel::Level::INFO)
#define LOG_WARN(logger) LOG_LEVEL(logger,LogLevel::Level::WARN)
#define LOG_ERROR(logger) LOG_LEVEL(logger,LogLevel::Level::ERROR)
#define LOG_FATAL(logger) LOG_LEVEL(logger,LogLevel::Level::FATAL)
#define LOG_ASSERT(logger,condition) \
    if (!(condition)) { LOG_ERROR(logger) << "Assertion failed: " #condition; }

