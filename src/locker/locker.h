#pragma once
#include <atomic>
#include <pthread.h>
#include <semaphore.h>
#include "queue"
#include "../global_def.h"
#include "../exception/exception.h"

namespace wa {
    class Condition;
    class Lock {
    public:
        virtual ~Lock() = default;
        virtual Bool lock() = 0;
        virtual Bool unlock() = 0;
        virtual Bool tryLock() = 0;
    };

    class BlockingLock : public Lock {
        friend Condition;
    public:
        BlockingLock();
        ~BlockingLock() override;
        Bool lock() override;
        Bool unlock() override;
        Bool tryLock() override;

    private:
        pthread_mutex_t mutex_;
    };

    class SpinLock : public Lock {
    public:
        SpinLock();
        ~SpinLock() override;
        Bool lock() override;
        Bool unlock() override;
        Bool tryLock() override;

    private:
        pthread_spinlock_t spinlock_;
    };


    class Condition {
    public:
        Condition(BlockingLock& mutex);
        ~Condition();
        Bool wait(const struct timespec* timeout = nullptr);
        Bool signal();
        Bool broadcast();

    private:
        pthread_cond_t condition_;
        BlockingLock& mutex_;
    };

    class Semaphore {
    public:
        Semaphore(UInt count = 0);
        ~Semaphore();
        Bool wait();
        Bool tryWait();
        Bool post();

    private:
        sem_t sem_;
        Semaphore(const Semaphore&) = delete;
        Semaphore(Semaphore&&) = delete;
        Semaphore& operator=(const Semaphore&) = delete;
    };

    class ReadWriteLock {
    public:
        ReadWriteLock(){
            pthread_rwlock_init(&rwlock_, nullptr);
        };
        ~ReadWriteLock(){
            pthread_rwlock_destroy(&rwlock_);
        };

        // 获取读锁
        Bool lockRead(){
            return pthread_rwlock_rdlock(&rwlock_) == 0;
        }

        // 获取写锁
        Bool lockWrite(){
            return pthread_rwlock_wrlock(&rwlock_) == 0;
        }

        // 释放锁
        Bool unlock(){
            return pthread_rwlock_unlock(&rwlock_) == 0;
        }

        // 尝试获取读锁
        Bool tryLockRead(){
            return pthread_rwlock_tryrdlock(&rwlock_) == 0;
        }

        // 尝试获取写锁
        Bool tryLockWrite(){
            return pthread_rwlock_trywrlock(&rwlock_) == 0;
        }

    private:
        pthread_rwlock_t rwlock_;  // 实际的读写锁
    };


    template<class T>
    class ReentrantLock {
    public:
        ReentrantLock();
        Bool lock();
        Bool unlock();
        Bool tryLock();
        Bool lockOnce();

    private:
        T lock_;
        std::atomic<pthread_t> owner_;
        std::atomic<UInt> lockCount_;
    };

    template<class T>
    class ScopedLock {
    public:
        explicit ScopedLock(T& lock);
        Bool unlock();
        ~ScopedLock();

    private:
        T& lock_;
        Bool locked_;
    };



    template<class L,class V>
    class LockedVariable{ //带锁的变量
    private:
        L lock_;
        V variable_;
    public:

        // 传入多个参数构造
        template<typename... Args>
        LockedVariable(Args&&... args)
                : variable_(std::forward<Args>(args)...) {}  // 完美转发参数到 variable_ 的构造函数

        V* lock();
        void unlock();
        V* tryLock();
    };

    template<class T,class V>
    V* LockedVariable<T,V>::tryLock() {
        if(lock_.tryLock()){
            return &variable_;
        }
        return nullptr;
    }

    template<class T,class V>
    void LockedVariable<T,V>::unlock() {
        lock_.unlock();
    }

    template<class T,class V>
    V* LockedVariable<T,V>::lock() {
        lock_.lock();
        return &variable_;
    }

    // ReentrantLock
    template<class T>
    ReentrantLock<T>::ReentrantLock() : owner_(0), lockCount_(0) {}

    template<class T>
    Bool ReentrantLock<T>::lock() {
        pthread_t currentThread = pthread_self();
        if (owner_ == currentThread) {
            lockCount_++;
            return TRUE;
        }

        if (lock_.lock()) {
            owner_ = currentThread;
            lockCount_ = 1;
            return TRUE;
        }
        return FALSE;
    }

    template<class T>
    Bool ReentrantLock<T>::unlock() {
        pthread_t currentThread = pthread_self();
        if (owner_ != currentThread) {
            throw std::runtime_error("Unlock called by non-owner thread");
        }

        lockCount_--;
        if (lockCount_ == 0) {
            owner_ = 0;
            lock_.unlock();
        }
        return TRUE;
    }

    template<class T>
    Bool ReentrantLock<T>::tryLock() {
        pthread_t currentThread = pthread_self();
        if (owner_ == currentThread) {
            lockCount_++;
            return TRUE;
        }

        if (lock_.tryLock()) {
            owner_ = currentThread;
            lockCount_ = 1;
            return TRUE;
        }
        return FALSE;
    }

    template<class T>
    Bool ReentrantLock<T>::lockOnce() {
        pthread_t currentThread = pthread_self();
        if (owner_ == currentThread) {
            return TRUE;
        }

        if (lock_.lock()) {
            owner_ = currentThread;
            lockCount_ = 1;
            return TRUE;
        }
        return FALSE;
    }

    // ScopedLock
    template<class T>
    ScopedLock<T>::ScopedLock(T& lock) : lock_(lock), locked_(FALSE) {
        if(!lock_.lock()){
            throw CODE_LOCATION_EXCEPTION();
        }
        locked_ = TRUE;
    }
    template<class T>
    Bool ScopedLock<T>::unlock() {
        if(locked_){
            locked_=!lock_.unlock();
        }

        return !locked_;
    }
    template<class T>
    ScopedLock<T>::~ScopedLock() {
        if (locked_) {
            lock_.unlock();
        }
    }
    template<class T>
    ScopedLock<T> AsScopedLock(T& lock){
        return ScopedLock<T>(lock);
    }

    // 局部读锁
    class ScopedReadLock {
    public:
        explicit ScopedReadLock(ReadWriteLock& lock) : lock_(lock),locked_(FALSE) {
            lock_.lockRead();  // 在构造时获取读锁
            locked_=TRUE;
        }

        Bool unlock(){
            if(locked_){
                locked_=!lock_.unlock();
            }
            return !locked_;
        }

        ~ScopedReadLock() {
            if(locked_)
                lock_.unlock();  // 在析构时释放读锁
        }

    private:
        ReadWriteLock& lock_;
        Bool locked_;
    };

    // 局部写锁
    class ScopedWriteLock {
    public:
        explicit ScopedWriteLock(ReadWriteLock& lock) :lock_(lock),locked_(FALSE) {
            lock_.lockRead();  // 在构造时获取读锁
            locked_=TRUE;
        }
        Bool unlock(){
            if(locked_){
                locked_=!lock_.unlock();
            }
            return !locked_;
        }
        ~ScopedWriteLock() {
            if(locked_){
                lock_.unlock();
            }
        }
    private:
        ReadWriteLock& lock_;
        Bool locked_;
    };

}
