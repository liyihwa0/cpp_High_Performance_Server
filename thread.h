//
// Created by Liyihwa on 24-11-3.
//

#ifndef LEARNING_THREAD_H
#define LEARNING_THREAD_H

#include <sstream>
#include "global_def.h"
#include "string"
#include "thread"
#include "functional"
#include "locker.h"
#include "util.h"


/*
 * 需要注意的是 ,不能在start()之前,使用Thread的tid,Tid,Name方法
 */



namespace wa {


    class Thread {
    public:

        Thread(std::function<Void(Void *)> callBack, Void *arg = nullptr, std::string name="") :
                callBack_(callBack), arg_(arg), name_(std::move(name)), isRunning_(FALSE), id_(0), sem_(0) {

        }

        ~Thread() {
            if (isRunning_) {
                pthread_detach(thread_);
            }
        }

        const std::string &name() const {
            return name_;
        }

        static const std::string &Name() {
            return thisThread->name_;
        }

        static Void SetName(const std::string &name){
            thisThread->name_=name;
        }

        static Tid Id() {
            return thisThread->id_;
        }

        Tid id() const {
            return id_;
        }

        Bool isRunning() const {
            return isRunning_;
        }

        PThread pThread() const {
            return thread_;
        }

        Bool cancel() const {
            return pthread_cancel(thread_)==0;
        }

        Bool detach() const {
            return pthread_detach(thread_)==0;
        }

        // 等待线程完成
        Bool join() {
            return pthread_join(thread_, nullptr)==0; // 等待线程结束
        }

        Bool start() {
            Int rt = pthread_create(&thread_, nullptr, &Thread::run, this);
            if (rt) {
                //todo : 使用自己的日志库
                throw std::logic_error("pthread_create error");
            }
            //等待 run中设置好 thisThread的信息
            sem_.wait();
            return rt==0;
        }

        static Thread *GetThis() {
            return thisThread;
        }


    private:
        static thread_local Thread *thisThread;

        // 线程入口函数
        static Void *run(Void *arg) {
            thisThread = (Thread *) arg;
            thisThread->isRunning_ = TRUE;
            thisThread->id_ = GetThreadId();
            if(thisThread->name_==""){
                std::stringstream ss;
                ss<<"t"<<thisThread->id_;
                thisThread->name_=ss.str();
            }

            thisThread->sem_.post();

            if (thisThread->callBack_) {
                thisThread->callBack_(thisThread->arg_); // 调用传入的函数
            }
            thisThread->isRunning_ = FALSE;
            return nullptr;
        }

        // 禁止拷贝构造
        Thread(const Thread &) = delete;

        // 禁止移动构造
        Thread(Thread &&) = delete;


        Tid id_;
        pthread_t thread_;
        std::function<Void(Void *)> callBack_;
        Void *arg_;
        std::string name_;
        Bool isRunning_;
        Semaphore sem_;

    };

    thread_local Thread *Thread::thisThread = nullptr; // 定义

}


#endif //LEARNING_THREAD_H

