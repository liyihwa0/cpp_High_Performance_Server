#include "./logger.h"

namespace wa {

    const char* LogLevel::ToString(LogLevel::Level level) {
        switch (level) {
            case DEBUG:
                return "DEBUG";
            case INFO:
                return "INFO";
            case WARN:
                return "WARN";
            case ERROR:
                return "ERROR";
            case FATAL:
                return "FATAL";
            default:
                return "UNKNOW";
        }
    }

    LogEvent::LogEvent(SP<Logger> logger, LogLevel::Level level, I64 elapsed, const String& file, U32 line, UP<std::stringstream> message)
            : logger_(std::move(logger)), level_(level), time_(Timer::Now()), elapsed_(elapsed),
              threadName_(Thread::Name()),
              threadId_(Thread::Id()),
              fiberId_(Fiber::Id()),
              fiberName_(Fiber::Name()),
              file_(file), line_(line), message_(std::move(message)) {}



    // DateTimeFormatItem 实现
    LogFormatter::DateTimeFormatItem::DateTimeFormatItem(const String &format) : format_(format) {}

    void LogFormatter::DateTimeFormatItem::write(OStream &stream, const LogEvent &logEvent)const {
        stream << logEvent.time_.format(format_);
    }

    // MessageFormatItem 实现
    void LogFormatter::MessageFormatItem::write(OStream &stream, const LogEvent &logEvent)const {
        stream << logEvent.message_->str();
    }

    // LogLevelFormatItem 实现
    void LogFormatter::LogLevelFormatItem::write(OStream &stream, const LogEvent &logEvent)const {
        stream << LogLevel::ToString(logEvent.level_);
    }

    // ElapsedTimeFormatItem 实现
    void LogFormatter::ElapsedTimeFormatItem::write(OStream &stream, const LogEvent &logEvent)const {
        stream << logEvent.elapsed_;
    }

    // LoggerNameFormatItem 实现
    void LogFormatter::LoggerNameFormatItem::write(OStream &stream, const LogEvent &logEvent) const{
        stream << logEvent.logger_->name();
    }

    // ThreadIdFormatItem 实现
    void LogFormatter::ThreadIdFormatItem::write(OStream &stream, const LogEvent &logEvent)const {
        stream << logEvent.threadId_;
    }

    // ThreadNameFormatItem 实现
    void LogFormatter::ThreadNameFormatItem::write(OStream &stream, const LogEvent &logEvent)const {
        stream << logEvent.threadName_;
    }

    // FiberIdFormatItem 实现
    void LogFormatter::FiberIdFormatItem::write(OStream &stream, const LogEvent &logEvent)const {
        stream << logEvent.fiberId_;
    }

    // FiberNameFormatItem 实现
    void LogFormatter::FiberNameFormatItem::write(OStream &stream, const LogEvent &logEvent)const {
        stream << logEvent.fiberName_;
    }

    // FilenameFormatItem 实现
    void LogFormatter::FilenameFormatItem::write(OStream &stream, const LogEvent &logEvent)const {
        stream << logEvent.file_;
    }

    // LineNumberFormatItem 实现
    void LogFormatter::LineNumberFormatItem::write(OStream &stream, const LogEvent &logEvent)const {
        stream << logEvent.line_;
    }

    // OriginTextFormatItem 实现
    LogFormatter::OriginTextFormatItem::OriginTextFormatItem(UP<String> text) : text_(std::move(text)) {}

    void LogFormatter::OriginTextFormatItem::write(OStream &stream, const LogEvent &logEvent)const {
        stream << *text_;
    }

    LogFormatter::LogFormatter(const String &fmt) {
        reset(fmt);
    }

    void LogFormatter::reset(const String& fmt) {
        formatItems_.clear();
        int j = 0;
        for (int i = 0; i < fmt.length();) {
            if (fmt[i] != '%' || (fmt[i] == '%' && i + 1 < fmt.length() && fmt[i + 1] == '%')) {
                j=i;
                UP<String> text(new String(""));
                while (j < fmt.length()) {
                    if (fmt[j] != '%') {
                        text->push_back(fmt[j]);
                    } else if (fmt[j] == '%' && j + 1 < fmt.length() && fmt[j + 1] == '%') {
                        text->push_back('%');
                        j++;
                    } else {
                        break;
                    }
                    j++;
                }
                i = j;
                formatItems_.emplace_back(new OriginTextFormatItem(std::move(text)));
            } else {
                i++;
                if (i >= fmt.length()) {
                    throw CODE_LOCATION_EXCEPTION("err format", fmt);
                }
                switch (fmt[i]) {
                    case 'm':
                        formatItems_.emplace_back(new MessageFormatItem());
                        break;
                    case 'p':
                        formatItems_.emplace_back(new LogLevelFormatItem());
                        break;
                    case 'r':
                        formatItems_.emplace_back(new ElapsedTimeFormatItem());
                        break;
                    case 'c':
                        formatItems_.emplace_back(new LoggerNameFormatItem());
                        break;
                    case 't':
                        formatItems_.emplace_back(new ThreadIdFormatItem());
                        break;
                    case 'N':
                        formatItems_.emplace_back(new ThreadNameFormatItem());
                        break;
                    case 'f':
                        //todo
                        formatItems_.emplace_back(new FiberNameFormatItem());
                        break;
                    case 'd': {
                        j = i + 1;
                        if (j >= fmt.length() || fmt[j] != '{') {
                            throw CODE_LOCATION_EXCEPTION("error fmt", fmt);
                        }
                        while (j < fmt.length() && fmt[j] != '}') j++;
                        // j指向}
                        if (j - 1 - (i + 2) < 0) {
                            throw CODE_LOCATION_EXCEPTION("error fmt", fmt);
                        }
                        formatItems_.emplace_back(new DateTimeFormatItem(fmt.substr(i + 2, j - i - 2)));
                        i = j;
                        break;
                    }
                    case 'F':
                        formatItems_.emplace_back(new FilenameFormatItem());
                        break;
                    case 'l':
                        formatItems_.emplace_back(new LineNumberFormatItem());
                        break;
                }
                i++;
            }
        }
    }

    UP<std::stringstream> LogFormatter::formattedStream(const LogEvent & logEvent) {
        std::stringstream* stream = new std::stringstream();
        for(int i=0;i<formatItems_.size();i++){
            formatItems_[i]->write(*stream,logEvent);
        }
        return UP<std::stringstream>(stream);
    }

    FileLogAppender::FileLogAppender(const std::string& filename) : filename_(filename) {
        fileStream_.open(filename_, std::ios::out | std::ios::app);
        if (!fileStream_.is_open()) {
            throw ERRNO_EXCEPTION(errno,"open failed ",filename);
        }
    }

    FileLogAppender::~FileLogAppender() {
        lock_.lock();
        if (fileStream_.is_open()) {
            fileStream_.close();
        }
        lock_.unlock();
    }

    OStream & FileLogAppender::lockStream() {
        lock_.lock();
        return fileStream_;
    }

    void FileLogAppender::unlockStream() {
        lock_.unlock();
    }

    StdLogAppender::StdLogAppender() = default;
    StdLogAppender::~StdLogAppender() = default;

    OStream & StdLogAppender::lockStream() {
        return std::cout;
    }

    void StdLogAppender::unlockStream() {}

    Logger::Logger(LogLevel::Level level, UP<LogFormatter> logFormatter, const String &name,SP<LogAppender> appender)
            : logLevel_(level),logFormatter_(std::move(logFormatter)),name_(name){
        if(appender.get()!= nullptr){
            appender_.emplace_back(std::move(appender));
        }
    };

    void Logger::write(const LogEvent& logEvent) {
        UP<std::stringstream> formattedStream = logFormatter_->formattedStream(logEvent);
        for (auto& appender : appender_) {
            auto& stream = appender->lockStream();
            try {
                stream << formattedStream->str();
            } catch (std::exception& e) {
                appender->unlockStream();
                throw;
            }
            appender->unlockStream();
        }
    }

    Bool Logger::judgeLevel(LogLevel::Level level) const {
        return level >= logLevel_;
    }

    const String& Logger::name() const {
        return name_;
    }

    Logger& Logger::setLevel(LogLevel::Level level) {
        logLevel_ = level;
        return *this;
    }

    Logger& Logger::addAppender(SP<LogAppender> appender) {
        appender_.push_back(std::move(appender));
        return *this;
    }

    Logger& Logger::setFormatter(UP<LogFormatter> logFormatter) {
        logFormatter_ = std::move(logFormatter);
        return *this;
    }

    Logger& Logger::setName(const String& name) {
        name_ = name;
        return *this;
    }

    Logger::~Logger(){
        if(false){}
    }
    LogEventWrapper::LogEventWrapper(SP<Logger> logger, LogLevel::Level level, I64 elapsed, const String& file, U32 line)
            : logEvent_(new LogEvent(std::move(logger), level, elapsed, file, line, UP<std::stringstream>(new std::stringstream()))) {}

    LogEventWrapper::~LogEventWrapper() {
        logEvent_->logger_->write(*logEvent_);
    }


    std::stringstream& LogEventWrapper::stream() {
        return *(logEvent_->message_);
    }

    SP<Logger> gl(new Logger(LogLevel::INFO, UP<LogFormatter>(new LogFormatter()),"global",
            SP<LogAppender>(new StdLogAppender())));

}
