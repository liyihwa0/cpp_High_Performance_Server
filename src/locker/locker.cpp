#include "./locker.h"
namespace wa {

    // BlockingLock
    BlockingLock::BlockingLock() {
        if (pthread_mutex_init(&mutex_, nullptr) != 0) {
            throw OS_ERRNO_EXCEPTION();
        }
    }

    BlockingLock::~BlockingLock() {
        pthread_mutex_destroy(&mutex_);
    }

    Bool BlockingLock::lock() {
        return pthread_mutex_lock(&mutex_) == 0;
    }

    Bool BlockingLock::unlock() {
        return pthread_mutex_unlock(&mutex_) == 0;
    }

    Bool BlockingLock::tryLock() {
        return pthread_mutex_trylock(&mutex_) == 0;
    }


    // SpinLock
    SpinLock::SpinLock() {
        if (pthread_spin_init(&spinlock_, PTHREAD_PROCESS_PRIVATE) != 0) {
            throw OS_ERRNO_EXCEPTION();
        }
    }

    SpinLock::~SpinLock() {
        pthread_spin_destroy(&spinlock_);
    }

    Bool SpinLock::lock() {
        return pthread_spin_lock(&spinlock_) == 0;
    }

    Bool SpinLock::unlock() {
        return pthread_spin_unlock(&spinlock_) == 0;
    }

    Bool SpinLock::tryLock() {
        return pthread_spin_trylock(&spinlock_) == 0;
    }


    // Condition
    Condition::Condition(BlockingLock& mutex) : mutex_(mutex) {
        if (pthread_cond_init(&condition_, nullptr) != 0) {
            throw std::exception();
        }
    }

    Condition::~Condition() {
        pthread_cond_destroy(&condition_);
    }

    Bool Condition::wait(const struct timespec* timeout) {
        pthread_mutex_lock(&mutex_.mutex_);
        int result;
        if (timeout == nullptr) {
            result = pthread_cond_wait(&condition_, &mutex_.mutex_);
        } else {
            result = pthread_cond_timedwait(&condition_, &mutex_.mutex_, timeout);
        }
        pthread_mutex_unlock(&mutex_.mutex_);
        return result == 0;
    }

    Bool Condition::signal() {
        return pthread_cond_signal(&condition_) == 0;
    }

    Bool Condition::broadcast() {
        return pthread_cond_broadcast(&condition_) == 0;
    }

    // Semaphore
    Semaphore::Semaphore(UInt count) {
        if (sem_init(&sem_, 0, count) != 0) {
            throw std::exception();
        }
    }

    Semaphore::~Semaphore() {
        sem_destroy(&sem_);
    }

    Bool Semaphore::wait() {
        return sem_wait(&sem_) == 0;
    }

    Bool Semaphore::tryWait() {
        return sem_trywait(&sem_) == 0;
    }

    Bool Semaphore::post() {
        return sem_post(&sem_) == 0;
    }


}
