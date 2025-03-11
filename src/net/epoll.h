#pragma once

#include "../global_def.h"
#include <vector>
#include "../locker/locker.h"
#include "src/timer.h"

inline Int max(Int a, Int b) {
    return a > b ? a : b;
}

namespace wa {

    // 能监听读或写事件的epoll,但是只能监听一个
    // epoll会将所有的event变为 水平触发|ONESHOT
    class OneEventEpoll {
        class FdNode {
        public:
            Bool contain_;
            SpinLock lock_;
            FdNode();
        };

        FileDescriptor epollFd_;
        std::vector<FdNode*> contains_;
        ReadWriteLock rwLock_;
        Int watchingCount_;

    public:
        OneEventEpoll();
        void addEvent(FileDescriptor fd, RWEvent rwEvent, Void* data);
        void delEvent(FileDescriptor fd);
        Int wait(std::vector<EpollEvent>& events, Timer timeout=Timer(0));  // 5毫秒
        ~OneEventEpoll();
    };
}
