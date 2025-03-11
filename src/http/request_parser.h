#pragma once

#include <memory>
#include "../global_def.h"
#include "map"
#include "./request.h" // 引用 HttpRequest

namespace wa {
    namespace http {

        // HTTP解析的有限状态机状态
        typedef enum class HttpParseState {
            START,
            WAIT_METHOD,
            WAIT_URI,
            WAIT_VERSION,
            WAIT_CRLF,
            WAIT_HEADER_KEY,
            WAIT_HEADER_VALUE,
            WAIT_EMPTY_LINE,
            WAIT_BODY,
            DONE,
            ERROR
        } HttpParseState;

        // 基于状态有限机的http request解析器
        class HttpRequestParser {
        private:
            void init();
            UP<HttpRequest> request_;
            Int remainingContentLength_;
            String method_;
            String headerKey_;
            String headerVal_;
            HttpParseState currentState_;
            SP<Channel<UP<HttpRequest>> > httpRequestChannel_;  // output 用于输出解析后的http request
            SP<Channel<UP<Buffer>> > contentBufferChannel_;     // input  用于输入有内容的buffer
            SP<Channel<UP<Buffer>> > recycleBufferChannel_;     // output 用于回收空buffer

            void parseMethod(const UP<Buffer>& buffer);
            void parseURI(const UP<Buffer>& buffer);
            void parseVersion(const UP<Buffer>& buffer);
            void parseCRLF(const UP <Buffer> &buffer);
            void parseHeaderKey(const UP<Buffer>& buffer);
            void parseHeaderValue(const UP<Buffer>& buffer);
            void parseEmptyLine(const UP <Buffer> &buffer);
            void parseBody(const UP<Buffer>& buffer);
            HttpParseState input(const UP<Buffer>& buffer);

        public:
            HttpRequestParser(SP<Channel<UP<Buffer>> > contentChannel,
                              SP<Channel<UP<Buffer>> > emptyChannel,
                              SP<Channel<UP<HttpRequest> > > output);
            void run();
        };

    }
}
