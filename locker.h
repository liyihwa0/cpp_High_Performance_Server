#ifndef LOCKER_H
#define LOCKER_H

#include "global_def.h"
#include<mutex>
#include<exception>
#include <semaphore.h>
#include "iostream"
class Condition;

class Mutex{
    friend class Condition;
public:
    Mutex(){
        if(pthread_mutex_init(&(this->mutex_),NULL)!=0){
            throw std::exception();
        }
    }
    ~Mutex(){
        pthread_mutex_destroy(&(this->mutex_));
    }

    Bool lock(){
        return pthread_mutex_lock(&(this->mutex_))==0;
    }

    Bool unlock(){
        return pthread_mutex_unlock(&(this->mutex_))==0;
    }

    Bool tryLock(){
        return pthread_mutex_trylock(&(this->mutex_))==0;
    }

    PMutex mutex(){
        return mutex_;
    }

private:
    pthread_mutex_t mutex_;
};


class Condition{
public:

    Condition(const Mutex &mutex):mutex_(mutex){
        if (pthread_mutex_init(&mutex_.mutex_, nullptr) != 0) {
            throw std::exception();
        }
        if (pthread_cond_init(&condition_, nullptr) != 0) {
            pthread_mutex_destroy(&mutex_.mutex_);  // 清理已初始化的资源
            throw std::exception();
        }
    }
    Condition(){
        std::cout<<"111"<<std::endl;
        if (pthread_mutex_init(&mutex_.mutex_, nullptr) != 0) {
            throw std::exception();
        }
        if (pthread_cond_init(&condition_, nullptr) != 0) {
            pthread_mutex_destroy(&mutex_.mutex_);  // 清理已初始化的资源
            throw std::exception();
        }
    }

    ~Condition() {
        pthread_cond_destroy(&condition_);
    }

    // 重载的 wait 方法：允许传入 timeout，timeout 为 nullptr 时不超时
    Bool wait(const struct timespec* timeout = nullptr) {
        pthread_mutex_lock(&mutex_.mutex_);
        int result;
        if (timeout == nullptr) {
            // 无超时等待
            result = pthread_cond_wait(&condition_, &mutex_.mutex_);
        } else {
            // 带超时等待
            result = pthread_cond_timedwait(&condition_, &mutex_.mutex_, timeout);
        }
        pthread_mutex_unlock(&mutex_.mutex_);
        return result==0;  // 如果返回 0，表示等待成功；否则返回超时或其他错误
    }

    Bool signal(){
       return pthread_cond_signal(&(this->condition_))==0;
    }

    Bool broadcast(){
       return pthread_cond_broadcast(&(this->condition_))==0;
    }

private:
    pthread_cond_t condition_;
    Mutex mutex_;
};


class Semaphore {
public:
    // 构造函数：初始化信号量，初始值为 count
    Semaphore(UInt count = 0) {
        if (sem_init(&sem_, 0, count) != 0) {
            throw std::exception();
        }
    }

    // 析构函数：销毁信号量
    ~Semaphore() {
        sem_destroy(&sem_);
    }

    Bool wait() {
        return sem_wait(&sem_)==0;
    }

    Bool tryWait() {
        return sem_trywait(&sem_) == 0;
    }

    Bool post() {
        return sem_post(&sem_)==0;
    }

private:
    sem_t sem_;  // 信号量对象
    Semaphore(const Semaphore&)=delete; //不允许拷贝构造
    Semaphore(const Semaphore&&)=delete; //不允许移动构造
    Semaphore operator=(const Semaphore)=delete; //不允许 =



};


#endif