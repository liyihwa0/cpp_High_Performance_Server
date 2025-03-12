#include "../request_parser.h"
#include "../../smart_ptr.h"
namespace wa {
    namespace http {
        class HttpRequestParseWorker {
        private:
            SP<Channel<UP<HttpRequest>>> httpRequestChannel_;  // output 用于输出解析后的http request
            SP<Channel<UP<Buffer>>> contentBufferChannel_;     // input  用于输入有内容的buffer
            SP<Channel<UP<Buffer>>> recycleBufferChannel_;     // output 用于回收空buffer
            HttpRequestParser parser_;
        public:
            HttpRequestParseWorker(SP<Channel<UP<wa::Buffer>>> contentChannel,
                                   SP<Channel<UP<wa::Buffer>>> emptyChannel,
                                   SP<Channel<UP<wa::http::HttpRequest>>> output) : contentBufferChannel_(
                    contentChannel),
                                                                                    recycleBufferChannel_(emptyChannel),
                                                                                    httpRequestChannel_(output),
                                                                                    parser_() {}

            void run() {
                UP<Buffer> buffer;
                while (TRUE) {
                    if (!contentBufferChannel_->read(buffer)) { // 没有内容可读了
                        break;
                    }

                    while (buffer->hasMore()) {
                        if (parser_.input(buffer) == HttpParseState::DONE) {
                            if (!httpRequestChannel_->write(parser_.getRequest())) { // 说明下游已经不再需要解析了
                                goto finish;
                            }
                        }
                    }

                    // 将用完的buffer归还
                    recycleBufferChannel_->write(buffer);
                }
                finish:
                httpRequestChannel_->close();
                contentBufferChannel_->close();
                if (buffer.get()) {
                    buffer->clear();
                    recycleBufferChannel_->write(buffer);
                }

            }

        };
    }
}

