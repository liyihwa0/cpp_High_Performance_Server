#include <sys/epoll.h>
#include "./epoll.h"
#include "../logger/logger.h"

namespace wa {

    OneEventEpoll::FdNode::FdNode() : contain_(FALSE), lock_() {}

    OneEventEpoll::OneEventEpoll() : rwLock_(), contains_(), epollFd_(-1), watchingCount_(0) {
        epollFd_ = epoll_create(1);  // 创建 epoll_ 实例
        if (epollFd_ == -1) {
            throw ERRNO_EXCEPTION(errno, "Failed to create epoll_ instance");
        }
    }

    void OneEventEpoll::addEvent(FileDescriptor fd, RWEvent rwEvent, Void* data) {
        if (fd == -1 || rwEvent & EPOLLIN & EPOLLOUT || !rwEvent) return;
        FdNode* node;
        ScopedReadLock readLock(rwLock_);
        if (fd >= contains_.size()) {
            readLock.unlock();

            ScopedWriteLock writeLock(rwLock_);
            contains_.resize(max(fd + 1, 1.5 * contains_.size()));
            for (auto &it : contains_) {
                if (!it) {
                    it = new FdNode();
                }
            }
            node = contains_[fd];
        } else {
            node = contains_[fd];
            readLock.unlock();
        }

        EpollEvent event;
        event.data.ptr = data;
        event.events = (rwEvent & (EPOLLIN | EPOLLOUT)) | EPOLLONESHOT;

        auto lock = AsScopedLock(node->lock_);
        // 判断是添加事件还是修改事件
        Int op = node->contain_ ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;
        Int ret = epoll_ctl(epollFd_, op, fd, &event);
        if (ret == -1) {
            throw ERRNO_EXCEPTION(op == EPOLL_CTL_MOD ? "mod" : "add", (event.events & EPOLLIN) ? "in" : "out");
        }
        node->contain_ = TRUE;
        watchingCount_+= op==EPOLL_CTL_ADD;
    }

    void OneEventEpoll::delEvent(FileDescriptor fd) {
        if (fd == -1) return;

        FdNode* node;

        {
            ScopedReadLock sl(rwLock_);
            if (fd >= contains_.size()) return;
            node = contains_[fd];
        }

        auto lock = AsScopedLock(node->lock_);
        // 删除事件
        if (node->contain_) {
            Int ret = epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, nullptr);
            if (ret == -1) {
                throw ERRNO_EXCEPTION(errno);
            }
            node->contain_ = FALSE;
            watchingCount_--;
        }
    }

    Int OneEventEpoll::wait(std::vector<EpollEvent>& events, Timer timeout) {
        Int ret = epoll_wait(epollFd_, events.data(), events.size(), timeout.getMS());
        if (ret == -1) {
            if(errno==EINTR) return 0;
            throw ERRNO_EXCEPTION(errno, "epoll_wait failed");
        }
        return ret;
    }

    OneEventEpoll::~OneEventEpoll() {
        if (epollFd_ != -1) {
            close(epollFd_);
        }
    }
}
