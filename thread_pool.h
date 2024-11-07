#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <global_def.h>
#include <pthread.h>  // 用于 pthread_t 和 pthread_create 等
#include <exception>  // 用于 std::exception
#include "locker.h"
#include <queue>      // 用于任务队列
#include <functional> // 用于 std::function
#include "thread.h"
#include "exception"

// 定义线程任务类型为函数指针，线程任务接受 void* 参数并返回 void*
typedef  std::function<void()> ThreadTask;


class ThreadPool {
public:
    ThreadPool(Size size, Size maxTaskSize)
            : size_(size), maxTaskSize_(maxTaskSize), stop_(false), sem_(maxTaskSize) {

        threads_ = new wa::Thread*[size];
        for (Size i = 0; i < size; i++) {
            try {
                threads_[i]=new wa::Thread(&ThreadPool::work1,this);

                if(!threads_[i]->start()){
                    throw std::runtime_error("Thread start failed");
                }
            }catch(const std::exception &e) {
                for(Size j=0;j<i;j++){
                    threads_[j]->cancel();
                    delete threads_[j];
                }
                delete[] threads_;
                throw ;
            }
        }
    }

    ~ThreadPool() {
        stop_ = TRUE;
        sem_.post();
        for (Size i = 0; i < size_; i++) {
            threads_[i]->join();
        }
        delete[] threads_;
    }

    Bool add(const ThreadTask& task) {

        mutex_.lock();
        if (taskQueue_.size() >= maxTaskSize_) {
            mutex_.unlock();
            return FALSE; // 队列已满
        }
        taskQueue_.push(task);
        sem_.post();
        mutex_.unlock();

        return TRUE;
    }

    Bool stop(){
        stop_=TRUE;
        return TRUE;
    }
private:

    // 信号量以及互斥锁版本
    static void work1(void * arg) {
        ThreadPool* pool = (ThreadPool*)arg;
        while (true) {
            pool->sem_.wait();
            pool->mutex_.lock();
            if (pool->stop_ && pool->taskQueue_.empty()) {
                pool->mutex_.unlock(); // 确保在退出前解锁
                break; // 如果停止且任务队列为空，退出
            }
            if (!pool->taskQueue_.empty()) {
                ThreadTask task = pool->taskQueue_.front();
                pool->taskQueue_.pop();
                pool->mutex_.unlock(); // 解锁以执行任务
                task(); // 执行任务
            } else {
                pool->mutex_.unlock(); // 解锁以便其他线程能访问
            }
        }
    }


    Bool stop_;              // 记录线程池是否停止
    wa::Thread** threads_;     // 线程数组
    Size size_;              // 线程池大小
    Size maxTaskSize_;       // 最大任务数量
    Mutex mutex_;
    Semaphore sem_;
    std::queue<ThreadTask> taskQueue_; // 任务队列
};
#endif
