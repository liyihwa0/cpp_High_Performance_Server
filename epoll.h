#include "global_def.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <exception>
#include <stdexcept>
#include <vector>


class Epoll{
    FileDescriptor epollFd_;
    EpollEvent *events_;
    Size eventsCount_;
    Size listeningEventsCount_;
public:    
    Epoll(Size eventsCount):eventsCount_(eventsCount){
        epollFd_ = epoll_create(1);  // 创建 epoll 实例
        if (epollFd_ == -1) {
            throw std::runtime_error("Failed to create epoll instance");
        }
        events_=new EpollEvent[eventsCount_];
    }

    ~Epoll(){
        if(epollFd_!=-1){
            close(epollFd_);
        }
        delete events_;
    }


    Void addEvent(FileDescriptor fd ,EpollEvent& event){
        int ret = epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event);
        if (ret == -1) {
            throw std::runtime_error("Failed to add event to epoll");
        }
        listeningEventsCount_++;
    }

    Void delEvent(FileDescriptor fd) {
        int ret = epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, nullptr);
        if (ret == -1) {
            throw std::runtime_error("Failed to delete event from epoll");
        }
        listeningEventsCount_--;
    }

    EpollEvent * events(){
        return events_;
    }
    
    // 等待事件的发生,通过events返回
    Int wait(Int timeout = -1) {
        if(listeningEventsCount_!=0){
            int num = epoll_wait(epollFd_, events_, eventsCount_, timeout);
            if (num == -1) {
                throw std::runtime_error("epoll_wait error");
            }
            return num;
        }
        return 0;
    }
};