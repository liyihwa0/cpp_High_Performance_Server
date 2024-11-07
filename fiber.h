//
// Created by Liyihwa on 24-11-4.
//

#ifndef LEARNING_FIBER_H
#define LEARNING_FIBER_H
#include "global_def.h"
#include <sys/timerfd.h>
#include "functional"
#include <ucontext.h>
#include <chrono>
#include "vector"
#include "epoll.h"
#include "iostream"

#define STACK_SIZE 131072 // 128KB
#define MAIN_FIBER_SLEEP_TIME 5000 // 主协程休眠5us

class Fiber{
public:
    enum class State{
        READY,          //就绪状态,等待被调度
        RUNNING,        // 执行中
        SUSPEND,        // 被挂起,协程调用了 yield主动释放执行权
        WAITING,        // 等待io事件或者主动AwaitTime
        FINISHED        // 协程执行完毕,无事可做
    };

    static Fiber* This(){
        return thisFiber_;
    }

    static Fiber* Main(){
        return mainFiber_;
    }

    static Void CreateMainFiber(){
        Fiber* fiber=new Fiber();
        fiber->id_=0;
        getcontext(&fiber-> context_);
        fiber-> context_.uc_stack.ss_sp = malloc(STACK_SIZE);   // 分配128KB的栈内存
        fiber-> context_.uc_stack.ss_size = STACK_SIZE;         // 设置栈大小
        fiber-> context_.uc_stack.ss_flags = 0;                 // 默认标志
        mainFiber_=fiber;
    }

    Fiber()=default;
    ~Fiber(){
        free(context_.uc_stack.ss_sp); // 释放栈内存
    };



    // 协程的入口
    // 调用者需要先添加几个协程到Fiber类中,再去调用Start
    static Void Start(Size maxEvent){
        epoll_=new Epoll(maxEvent);
        CreateMainFiber();
        makecontext(&mainFiber_-> context_,&Fiber::MainFiberWork1,0);

        swapcontext(&Fiber::mainFiberRecoverContext_,&mainFiber_->context_);

        delete mainFiber_;
        delete epoll_;
        for(Fiber * subFiber : subFibers_){
            delete subFiber;
        }
    }


    // 等待 time毫秒后继续执行
    static Void AwaitTime(Size timeMs) {

        // 记录等待的起始时间
        Timestamp startTime = std::chrono::steady_clock::now();
        // 记录等待的总时间长度
        Timestamp endTime = startTime + std::chrono::milliseconds(timeMs);
        // 将协程状态设置为 SUSPEND
        thisFiber_->state_ = State::SUSPEND;
        // 保存当前协程的恢复时间
        thisFiber_->wakeUpTime_ = endTime;
        // 切换到主协程
        SwitchToMainFiber();
    }

    // 等待传入的文件描述符准备好后,继续执行
    static Void AwaitFd(FileDescriptor fd, EpollEvent epollEvent){
        epollEvent.data.ptr=thisFiber_;
        epoll_->addEvent(fd,epollEvent);
        thisFiber_->state_=State::WAITING;
        SwitchToMainFiber();
    }

    static Void Yield(){
        thisFiber_->state_=State::SUSPEND;
        thisFiber_->wakeUpTime_ = std::chrono::steady_clock::now();
        SwitchToMainFiber();
    }

    static Size Id(){
        return thisFiber_->id_;
    }

    Size id() const{
        return id_;
    }

    State state() const{
        return state_;
    }

    static State State(){
        return thisFiber_->state_;
    }

    static Void AddTask(Task task){
        Fiber* fiber=new Fiber();
        fiber->state_=State::READY;
        fiber->id_=++count_;
        getcontext(&(fiber->context_));
        fiber->context_.uc_stack.ss_sp = malloc(STACK_SIZE);   // 分配128KB的栈内存
        fiber->context_.uc_stack.ss_size = STACK_SIZE;         // 设置栈大小
        fiber->context_.uc_stack.ss_flags = 0;                 // 默认标志
        fiber->task_=task;
        makecontext(&(fiber->context_),&Fiber::SubFiberWork1,0);
        subFibers_.push_back(fiber);
    }


private:
    static thread_local Fiber* thisFiber_;
    static thread_local Fiber* mainFiber_;
    static thread_local UContext mainFiberRecoverContext_;
    static thread_local std::vector<Fiber*> subFibers_;
    static thread_local Epoll *epoll_;
    static thread_local Size count_;
    Task task_;
    enum State state_;
    UContext context_;
    Size id_;
    std::chrono::steady_clock::time_point wakeUpTime_;


    // 执行fiber,该函数只会被mainFiber调用到
    Void switchTo(){
        Fiber::thisFiber_=this;
        this->state_=State::RUNNING;
        swapcontext(& (mainFiber_->context_), & (this->context_));
        if(this->state_==State::RUNNING){
            this->state_=State::WAITING;
        }
        thisFiber_=mainFiber_;
    }

    // 主协程的任务
    // 在每一次被调度时
    // 1.查询epoll有无准备好的事件
    // 2.遍历事件,找到对应fiber,将其切换为就绪态
    // 3.在就绪的fiber中选择一个出来执行
    // 4.如果所有协程都执行完毕,则结束MainFiberWork1
    static Void MainFiberWork1(){
        while(true){
            usleep(5);
            Int okEventCount=epoll_->wait(0);
            EpollEvent * okEvents=epoll_->events();
            for(Size i=0;i<okEventCount;i++){
                // 取出fiber指针
                Fiber * fiber= (Fiber *) okEvents[i].data.ptr;
                // 转移状态
                fiber->state_=State::READY;
            }

            // 检查 SUSPEND 状态的协程是否超时
            auto currentTime = std::chrono::steady_clock::now();
            for (Fiber *fiber : subFibers_) {
                if (fiber->state_ == State::SUSPEND && fiber->wakeUpTime_ <= currentTime) {

                    fiber->state_ = State::READY;
                }
            }

            Size readyCount=0;
            Size finishedCount=0;
            for(Size i=0;i<Fiber::subFibers_.size();i++){
                switch (subFibers_[i]->state_) {
                    case State::READY:
                        readyCount++;
                        break;
                    case State::FINISHED:
                        finishedCount++;
                        break;
                }
            }

            if(finishedCount==subFibers_.size()){
                break;
            }

            if(readyCount==0){
                continue;
            }
            // 随机选择下一个待执行的协程
            Size nextRun=rand()%readyCount;
            for(Size i=0;i<Fiber::subFibers_.size();i++){
                if(subFibers_[i]->state_==State::READY){
                    if(nextRun--==0){
                        subFibers_[i]->switchTo();
                        break;
                    }
                }
            }
        }

        setcontext(&mainFiberRecoverContext_);
    }

    static Void SubFiberWork1(){
        thisFiber_->task_();
        thisFiber_->state_=State::FINISHED;
        // sub fiber 在所有操作做完后,需要切回主协程
        SwitchToMainFiber();
    }

    static Void SwitchToMainFiber(){
        Fiber * temp=thisFiber_;
        thisFiber_=mainFiber_;
        swapcontext(&(temp->context_),&(mainFiber_->context_));
    }
};

thread_local Size Fiber::count_ = 0;  // 这里对count_进行初始化
thread_local Fiber* Fiber::thisFiber_= nullptr;
thread_local Fiber* Fiber::mainFiber_ = nullptr;
thread_local Epoll* Fiber::epoll_ = nullptr;
thread_local UContext Fiber::mainFiberRecoverContext_;
thread_local std::vector<Fiber*> Fiber::subFibers_;



#endif //LEARNING_FIBER_H

