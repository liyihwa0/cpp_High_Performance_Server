#pragma once

#include "../global_def.h"
#include "../thread/thread.h"
#include <functional>
#include <ucontext.h>
#include <chrono>
#include <map>
#include <unordered_set>
#include <vector>
#include "../net/epoll.h"
#include <iostream>
#include "../smart_ptr.h"
#include "../timer.h"
#include "../locker/locker.h"

#include "shared_mutex"
#define STACK_SIZE 131072 // 128KB

namespace wa {

    template<class T>
    class Channel;
    class FiberLock;
    class Fiber {
        friend class FiberLock;

        template<class T>
        friend class Channel;
        enum FiberState {
            READY = 0,
            RUNNING=1,
            YIELD=2,
            WAITING_TIME=3,     // 理论上最小堆(实际上用的vector)
            WAITING_LOCK=4,     // 每一个锁,后边跟一个被阻塞的队列
            WAITING_FD=5,       // 协程指针放到event.data.ptr中
            WAITING_CHANNEL=6,  // 每一个channel,后边跟一个被阻塞的队列
            FINISHED=7,         // 协程运行结束
        };
    public:


        static void Start(Int threadCount = 1);
        static void ThreadTask();
        static void AwaitTime(Timer timer);
        static void AwaitEvent(FileDescriptor fd, RWEvent event);
        static void DeleteEvent(FileDescriptor fd, RWEvent event);
        static Fiber* This();
        static void Stop();
        static void Yield();
        static ID Id();
        static const String & Name();
        static FiberState State();
        static void AddTask(const Task& task,const String&name="");
        static String Dump();
        ~Fiber();

    private:

        Fiber(Task task, Bool useIdCounter = TRUE,const String&name="");
        void reuseFiber(Task task,const String&name);
        void switchTo();
        String stateToString(){
            switch (this->state_) {
                case READY:
                    return "READY";
                case RUNNING:
                    return "RUNNING";
                case YIELD:
                    return "YIELD";
                case WAITING_TIME:
                    return "WAITING_TIME";
                case WAITING_LOCK:
                    return "WAITING_LOCK";
                case WAITING_FD:
                    return "WAITING_FD";
                case WAITING_CHANNEL:
                    return "WAITING_CHANNEL";
                case FINISHED:
                    return "FINISHED";
                default:
                    return "UNKNOWN";  // 如果枚举值无效，则返回 "UNKNOWN"
            }
        }
        static void MainFiberWork1();
        static void SubFiberWork1();
        static void SwitchToMainFiber(Task afterSwitchCallback);
        static void CreateMainFiber(UContext * recoveryContext);

        static thread_local Fiber* thisFiber_;                  // 当前执行的协程
        static thread_local Fiber* mainFiber_;                  // 主协程
        static thread_local Size idCounter_;                    // 协程id计数器

        static OneEventEpoll* readEpoll_;
        static OneEventEpoll* writeEpoll_;


        static Bool stopping_;                                  // 停止标志

        // AwaitEvent
        // AwaitTimer
        // AwaitChannel
        // AWaitLock
        static std::unordered_set<Fiber*> allFibers_;           // 所有协程
        static SpinLock allFibersLock_;                         // 协程池锁

        static std::queue<Fiber*> readyFibers_;                 // 已经可以执行的协程
        static SpinLock readyFibersLock_;                       // 已经可以执行的协程的锁

        static std::vector<Fiber*> waitingTimerFibers_;         // 等待固定时间的fiber
        static SpinLock waitingTimerFibersLock_;

        static std::queue<Fiber*> finishedFibers_;                  // 已经执行结束的协程
        static SpinLock finishedFiberLock_;


        //static std::priority_queue<Int,Fiber*> waitFor

        static std::vector<UP<Thread>> threads_;                // 线程池

        static Size taskCount_;
        static Size finishedFiberCount_;

        Task task_;                                             // 协程的任务
        enum FiberState state_;                                 // 协程的状态
        UContext context_;                                      // 协程的上下文
        Task afterSwitchCallback_;                              // 在完成协程栈切换后要执行的函数
        ID id_;                                                 // 协程id
        String name_;                                           // 协程名称
        Timer wakeupTime_;                                      // 协程的唤醒时间
        SpinLock lock_;                                         // 协程单独的锁
    };

    /*
     * 协程锁
     * 在加不到锁时,会切换到其他协程,直到拿到锁
     * 在持有锁时,保证只有持有锁的协程会运行到临界区中
     */
    class FiberLock : public Lock {
    private:
        std::queue<Fiber *> awaitingFibers_;
        SpinLock awaitingFibersLock_;
        SpinLock spinlock_;

    public:
        FiberLock();
        ~FiberLock() override;
        Bool lock() override;
        Bool unlock() override;
        Bool tryLock() override;
    };


}

