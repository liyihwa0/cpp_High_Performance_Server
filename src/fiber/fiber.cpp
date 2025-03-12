#include "./fiber.h"
#include "../net/epoll.h"
#include "../global_def.h"
#include "src/logger/logger.h"
#include <cstdlib>
#include <sstream>
#include <map>
#include <utility>
#include <sys/epoll.h>

#define EPOLL_EVENTS_SIZE 1024
namespace wa {

    // FiberLock
    FiberLock::FiberLock() : spinlock_(),awaitingFibers_(), awaitingFibersLock_() {}
    FiberLock::~FiberLock() = default;

    Bool FiberLock::lock() {
        if(spinlock_.tryLock()) {
            return TRUE;
        }
        else{
            // 说明有其他协程在用锁,我们需要将自己协程保存到队伍末尾
            Fiber::thisFiber_->state_=Fiber::FiberState::WAITING_LOCK;
            awaitingFibersLock_.lock();
            awaitingFibers_.push(Fiber::This());
            Fiber::SwitchToMainFiber([&]{
                awaitingFibersLock_.unlock();
            });

            // 等到再次被唤醒时,已经持有FiberLock了
            return TRUE;
        }
    }

    Bool FiberLock::unlock() {
        awaitingFibersLock_.lock();
        if(!awaitingFibers_.empty()){
            Fiber * front=awaitingFibers_.front();
            awaitingFibers_.pop();
            awaitingFibersLock_.unlock();
            front->state_=Fiber::READY;
            {
                auto rfl= AsScopedLock(Fiber::readyFibersLock_);
                Fiber::readyFibers_.push(front);
            }
        }else{
            awaitingFibersLock_.unlock();
            spinlock_.unlock();
        }
        return TRUE;
    }

    Bool FiberLock::tryLock() {
        return spinlock_.tryLock();
    }

    // Fiber serverInit

    thread_local Fiber* Fiber::thisFiber_ = nullptr;
    thread_local Fiber* Fiber::mainFiber_ = nullptr;


    thread_local Size Fiber::idCounter_ = 0;
    Bool Fiber::stopping_ = false;

    std::vector<UP<Thread>> Fiber::threads_;

    // AWaitLock
    std::unordered_set<Fiber*> Fiber::allFibers_;           // 所有协程
    SpinLock Fiber::allFibersLock_;                         // 协程池锁


    std::queue<Fiber*> Fiber::readyFibers_;                 // 已经可以执行的协程
    SpinLock Fiber::readyFibersLock_;                       // 已经可以执行的协程的锁

    std::vector<Fiber*> Fiber::waitingTimerFibers_;
    SpinLock Fiber::waitingTimerFibersLock_;

    std::queue<Fiber*> Fiber::finishedFibers_;                  // 已经执行结束的协程
    SpinLock Fiber::finishedFiberLock_;

    Size Fiber::taskCount_=0;
    Size Fiber::finishedFiberCount_=0;

    OneEventEpoll* Fiber::writeEpoll_= nullptr;

    OneEventEpoll* Fiber::readEpoll_= nullptr;



    Fiber::Fiber(Task task, Bool useIdCounter,const String&name)
            : state_(FiberState::READY), task_(std::move(task)),  name_(),lock_() {

        if (useIdCounter) {
            id_ = ++idCounter_;
        } else {
            id_ = 0;
        }

        if(name.empty()){
            name_ = "f" + std::to_string(id_);
        }
        else{
            name_=name;
        }

        state_ = FiberState::READY;
        getcontext(&context_);
        context_.uc_stack.ss_sp = malloc(STACK_SIZE);
        context_.uc_stack.ss_size = STACK_SIZE;
        context_.uc_stack.ss_flags = 0;
    }

    void Fiber::Start(Int threadCount) {
        readEpoll_=new OneEventEpoll();
        writeEpoll_=new OneEventEpoll();




        for (Int i = 0; i < threadCount; i++) {
            threads_.emplace_back(new Thread(ThreadTask));
        }

        for (UP<Thread>& t : threads_) {
            t->join();
        }

        delete readEpoll_;
        delete writeEpoll_;
        idCounter_ = 0;
        for (auto fiber : allFibers_) {
            delete fiber;
        }
        allFibers_.clear();
        threads_.clear();
        taskCount_=0;
        finishedFiberCount_=0;
    }

    // 每个线程要执行的任务
    void Fiber::ThreadTask() {
        UContext recoveryContext;
        getcontext(&recoveryContext);
        CreateMainFiber(&recoveryContext);
        swapcontext(&recoveryContext, &mainFiber_->context_);
        delete mainFiber_;
    }

    void Fiber::AwaitTime(Timer timer) {
        thisFiber_->state_ = FiberState::WAITING_TIME;
        thisFiber_->wakeupTime_ = timer + Timer::Now();

        // 将自己添加到 waitingTimer队列中

        waitingTimerFibersLock_.lock();
        auto it = waitingTimerFibers_.begin();
        for(;it!=waitingTimerFibers_.end();it++){
            if(thisFiber_->wakeupTime_<=((*it)->wakeupTime_)){
                waitingTimerFibers_.insert(it, thisFiber_);
                break;
            }
        }
        if(it==waitingTimerFibers_.end()){
            waitingTimerFibers_.push_back(thisFiber_);
        }

        SwitchToMainFiber([]{
            waitingTimerFibersLock_.unlock();

        });
    }

    // 等待fd上的epollEvent事件发生
    // 所有的event均为 LT + ONESHOT模式
    void Fiber::AwaitEvent(FileDescriptor fd, RWEvent  event) {

        event&=(EPOLLIN|EPOLLOUT);
        if(event==0||event==(EPOLLIN|EPOLLOUT)){
            throw CODE_LOCATION_EXCEPTION("wrong event: ",event);
        }


        // 假设有主协程卡在 epoll_wait,那么新加进去的epollEvent会被立即读到
        // 然后epollEvent中设置的FiberPtr会将协程切换到执行态,然而我们这时候还没保存协程并退出呢
        // 我们需要切回主协程->加锁->设置epoll事件->解锁
        Fiber* fb=thisFiber_;
        if(event&EPOLLIN){
            SwitchToMainFiber([fb,fd,event]{
                fb->lock_.lock();
                readEpoll_->addEvent(fd,event,fb);
                fb->state_=FiberState::WAITING_FD;
                fb->lock_.unlock();
            });
        }else{
            SwitchToMainFiber([fb,fd,event]{
                fb->lock_.lock();
                writeEpoll_->addEvent(fd,event,fb);
                fb->state_=FiberState::WAITING_FD;
                fb->lock_.unlock();
            });
        }
    }



    void Fiber::DeleteEvent(FileDescriptor fd,RWEvent event) {
        event&=(EPOLLIN|EPOLLOUT);
        if(event==0||event==(EPOLLIN|EPOLLOUT)){
            throw CODE_LOCATION_EXCEPTION("wrong event: ",event);
        }
        if(event&EPOLLIN) {
            readEpoll_->delEvent(fd);
        }else{
            writeEpoll_->delEvent(fd);
        }
    }

    Fiber* Fiber::This() {
        if (thisFiber_) {
            return thisFiber_;
        }

        thisFiber_ = new Fiber(Task{}, FALSE,"NONE");
        return thisFiber_;
    }

    void Fiber::Stop() {
        stopping_ = TRUE;
        if (thisFiber_ != mainFiber_) {
            SwitchToMainFiber({});
        }
    }

    void Fiber::Yield() {
        allFibersLock_.lock();
        thisFiber_->state_ = FiberState::YIELD;
        SwitchToMainFiber({});
    }

    ID Fiber::Id() {
        return This()->id_;
    }

    const String& Fiber::Name() {
        return This()->name_;
    }

     Fiber::FiberState Fiber::State() {
        return thisFiber_->state_;
    }

    void Fiber::AddTask(const Task& task,const String& name) {

        // 先检查有无已经结束的协程
        Fiber * fiber= nullptr;
        {
            auto lock = AsScopedLock(finishedFiberLock_);
            taskCount_++;
            if(!finishedFibers_.empty()){
                fiber=finishedFibers_.front();
                finishedFibers_.pop();
            }
        }
        if(fiber!= nullptr){
            fiber->lock_.lock();
            fiber->state_=READY;
            fiber->task_=task;
            fiber->name_=name;
            makecontext(&fiber->context_, &Fiber::SubFiberWork1, 0);
            {
                auto lock= AsScopedLock(readyFibersLock_);
                readyFibers_.push(fiber);
            }
            fiber->lock_.unlock();
            return;
        }


        fiber = new Fiber(task,TRUE,name);
        makecontext(&(fiber->context_), &Fiber::SubFiberWork1, 0);
        {
            auto lock= AsScopedLock(readyFibersLock_);
            readyFibers_.push(fiber);
        }
    }


    Fiber::~Fiber() {
        free(context_.uc_stack.ss_sp);
    }

    // 主协程调度到空闲协程
    void Fiber::switchTo() {

        this->state_=FiberState::RUNNING;
        thisFiber_ = this;
        swapcontext(&(mainFiber_->context_), &(this->context_));

        // 运行到此处时,已经切回来了,我们需要完成子协程剩余的工作
        if(this->afterSwitchCallback_!= nullptr){
            this->afterSwitchCallback_();
            this->afterSwitchCallback_= nullptr;
        }
    }

//    void Fiber::MainFiberWork1() {
//        while (true) {
//            {
//                allFibersLock_.lock();
//                Int okEventCount = epoll_->wait(0);
//                auto& okEvents = epoll_->events();
//                for (Size i = 0; i < okEventCount; i++) {
//                    Fiber* fiber = (Fiber*)okEvents[i].data.ptr;
//                    fiber->state_ = FiberState::READY;
//                }
//
//                auto currentTime = Timer::Now();
//                for (Fiber* fiber : allFibers_) {
//                    if (fiber->state_ == FiberState::WAITING_TIME && fiber->wakeupTime_ <= currentTime || fiber->state_==YIELD) {
//                        fiber->state_ = FiberState::READY;
//                    }
//                }
//
//                Size readyCount = 0;
//                Size finishedCount = 0;
//
//                for (Fiber* fiber : allFibers_) {
//                    switch (fiber->state_) {
//                        case FiberState::READY:
//                            readyCount++;
//                            break;
//                        case FiberState::FINISHED:
//                            finishedCount++;
//                            break;
//                    }
//                }
//
//                if (finishedCount == allFibers_.size()) {
//                    allFibersLock_.unlock();
//                    break;
//                }
//
//                if (readyCount == 0) {
//                    allFibersLock_.unlock();
//                    continue;
//                }
//
//                Int nextRun = rand() % readyCount;
//                for (Fiber* fiber : allFibers_) {
//                    if (fiber->state_ == FiberState::READY) {
//                        if (nextRun-- == 0) {
//                            fiber->switchTo();
//                            // 当协程从子协程切换到主协程时,需要先拿锁,因此在切回来后,需要释放
//                            allFibersLock_.unlock();
//
//                            break;
//                        }
//                    }
//                }
//            }
//
//            if (Fiber::stopping_) break;
//        }
//    }
    void Fiber::MainFiberWork1() {
        std::vector<EpollEvent > vec(EPOLL_EVENTS_SIZE);
        while (true) {

           Int count=readEpoll_->wait(vec);
           for(Int i=0;i<count;i++){
               Fiber* fiber = (Fiber*)vec[i].data.ptr;
               auto fiberLock=AsScopedLock(fiber->lock_);
               fiber->state_ = FiberState::READY;
               auto readyFibersLock= AsScopedLock(readyFibersLock_);
               readyFibers_.push(fiber);
           }


            count= writeEpoll_->wait(vec);
            for(Int i=0;i<count;i++){
                Fiber* fiber = (Fiber*)vec[i].data.ptr;
                auto fiberLock=AsScopedLock(fiber->lock_);
                fiber->state_ = FiberState::READY;
                auto readyFibersLock= AsScopedLock(readyFibersLock_);
                readyFibers_.push(fiber);
            }



                // 调度第一个进入ready状态的协程
                Fiber * fiber= nullptr;
                {
                    auto rf = AsScopedLock(readyFibersLock_);
                    if (!readyFibers_.empty()) {
                        fiber = readyFibers_.front();
                        readyFibers_.pop();
                    }
                }
                if(fiber!= nullptr) {
                    // 切入协程
                    fiber->switchTo();
                }

                if (Fiber::stopping_) break;

                if(Fiber::taskCount_==Fiber::finishedFiberCount_) break;
            }
        }

    void Fiber::SubFiberWork1() {

        thisFiber_->task_();

        LOG_DEBUG(gl)<<"work finished";
        thisFiber_->state_ = FiberState::FINISHED;
        // 协程执行完毕后,需要将其记录进finishedFiber
        {
            auto sl= AsScopedLock(finishedFiberLock_);
            finishedFiberCount_++;
            finishedFibers_.push(thisFiber_);
        }
        SwitchToMainFiber({});
    }

    /*
     * 切换到主协程的方法
     */
    void Fiber::SwitchToMainFiber(Task afterSwitchCallback) {

        // 协程因为期待某些事件发生,切回主协程
        thisFiber_->afterSwitchCallback_=std::move(afterSwitchCallback);
        Fiber* temp = thisFiber_;
        thisFiber_ = mainFiber_;
        swapcontext(&(temp->context_), &(mainFiber_->context_));
    }

    void Fiber::CreateMainFiber(UContext* recoveryContext) {
        Fiber* fiber = new Fiber(nullptr, FALSE,"main");
        fiber->context_.uc_link = recoveryContext;
        makecontext(&fiber->context_, &Fiber::MainFiberWork1, 0);
        mainFiber_ = fiber;
        thisFiber_ = fiber;
    }



}
