#pragma once
#include "../global_def.h"
#include "./fiber.h"
#include "../data_structure/queue.h"
#include "../locker/locker.h"
#include "../logger/logger.h"
#include "../buffer.h"
// Channel类的用法同go

/*
 * 每秒大约可执行13万次左右
 */
namespace wa {
    template <typename T>
    class Channel {
    private:
        CircularQueue<T> channelQueue_;              // 存放数据的
        std::queue<Fiber *> consumerFibers_;         // 正在等待读channel的协程
        std::queue<Fiber *> producerFibers_;         // 正在等待写channel的协程
        SpinLock lock_;                              // 对channel的加锁保护
        Bool closed_;

    public:
        explicit Channel(Size size) : channelQueue_(size),closed_(FALSE),lock_(),consumerFibers_(),producerFibers_() {}
        ~Channel(){
            close();
        }

        // 读操作
        // 当channel被关闭后,依旧可以读,直到再也没有数据
        Bool read(T &res) {
            lock_.lock(); // 对队列加锁保护

            // 等待读取条件
            while (channelQueue_.empty()&&!closed_) { // 当不满足该条件时,跳出来
                consumerFibers_.push(Fiber::This());  // 将当前协程加入等待队列
                Fiber::thisFiber_->state_ = Fiber::FiberState::WAITING_CHANNEL;

                // 切换到主协程，当前协程等待
                Fiber::SwitchToMainFiber([this] {
                    lock_.unlock();  // 切换后释放锁
                });

                lock_.lock();  // 被唤醒后重新获取锁
            }

            Bool rt=FALSE;
            if(!channelQueue_.empty()) {
                res = channelQueue_.popFront(); //读数据
                rt=TRUE;

                if (!closed_&& !producerFibers_.empty()) { //唤醒一个生产者
                    Fiber *next = producerFibers_.front();
                    producerFibers_.pop();
                    // 唤醒协程
                    next->state_ = Fiber::READY;
                    {
                        auto sl = AsScopedLock(Fiber::readyFibersLock_);
                        Fiber::readyFibers_.push(next);
                    }
                }
            }

            lock_.unlock();
            return rt;
        }

        Bool write(T t) {
            lock_.lock();

            if(closed_){
                lock_.unlock();
                return FALSE;
            }
            // 等待可写
            while(channelQueue_.full()&&!closed_){
                producerFibers_.push(Fiber::This());  // 将当前协程加入等待队列
                Fiber::thisFiber_->state_ = Fiber::FiberState::WAITING_CHANNEL;

                // 切换到主协程，当前协程等待
                Fiber::SwitchToMainFiber([this] {
                    lock_.unlock();  // 切换后释放锁
                });

                lock_.lock();  // 被唤醒后重新获取锁
            }

            // 运行到此处,说明要么close了,要么能写
            if(closed_){
                lock_.unlock();
                return FALSE;
            }else if(!channelQueue_.full()){
                channelQueue_.push(std::move(t));

                if (!consumerFibers_.empty()) { // 唤醒消费者
                    Fiber *next = consumerFibers_.front();
                    consumerFibers_.pop();
                    next->state_ = Fiber::READY; // 唤醒协程
                    {
                        auto sl= AsScopedLock(Fiber::readyFibersLock_);
                        Fiber::readyFibers_.push(next);
                    }
                }
                lock_.unlock();
                return TRUE;
            }

            // 不会运行到此行代码
            LOG_ERROR(gl)<<"this line shouldn't reach";
            lock_.unlock();
            return FALSE;
        }

        // 当channel被关闭后
        // 不允许再写入,允许读,直到把channel中多余数据读完
        void close(){
            auto sl =AsScopedLock(lock_);
            if(closed_) return;
            closed_=TRUE;
            // 唤醒处在写端的协程
            while(producerFibers_.size() != 0){
                Fiber *writer = producerFibers_.front();
                producerFibers_.pop();
                // 唤醒协程
                writer->state_ = Fiber::READY;
                {
                    auto sl= AsScopedLock(Fiber::readyFibersLock_);
                    Fiber::readyFibers_.push(writer);
                }
            }

            // 唤醒全部消费者
            if(channelQueue_.empty()){
                while(consumerFibers_.size() != 0){
                    Fiber *reader = consumerFibers_.front();
                    consumerFibers_.pop();
                    // 唤醒协程
                    reader->state_ = Fiber::READY;
                    {
                        auto sl= AsScopedLock(Fiber::readyFibersLock_);
                        Fiber::readyFibers_.push(reader);
                    }
                }
            }
        }
    };


}
