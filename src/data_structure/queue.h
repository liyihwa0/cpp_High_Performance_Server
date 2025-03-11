#include "../global_def.h"
#include "../exception/exception.h"


namespace wa{
    template<typename T>
    class Queue{

    };


    template <typename T>
    class CircularQueue {
    public:
        CircularQueue(Size capacity=1024)
                : capacity_(capacity), front_(0), rear_(0), size_(0) {
            queue_ = new T[capacity_];
        }

        ~CircularQueue() {
            delete[] queue_;
        }



        Bool push(T value) { // value已经是拷贝后的值了
            if (full()) {
                return FALSE;  // 队列已满
            }
            queue_[rear_] = std::move(value);  // 移动进去
            rear_ = (rear_ + 1) % capacity_;
            ++size_;
            return TRUE;
        }


        Bool pop() {
            if (empty()) {
                return FALSE;
            }
            front_ = (front_ + 1) % capacity_;
            --size_;
            return TRUE;
        }

        T popFront() {
            if (empty()) {
                throw CODE_LOCATION_EXCEPTION("Queue is empty");
            }
            T poppedValue = std::move(queue_[front_]);  // 将front_移动出来
            front_ = (front_ + 1) % capacity_;  // Move front pointer forward
            --size_;
            return poppedValue;  // Return the popped value
        }


        // 返回队头元素，但不删除
        T& front() {
            if (empty()) {
                throw CODE_LOCATION_EXCEPTION("Queue is empty");
            }
            return queue_[front_];
        }

        const T& front() const {
            if (empty()) {
                throw CODE_LOCATION_EXCEPTION("Queue is empty");
            }
            return queue_[front_];
        }

        Bool empty() const {
            return size_ == 0;
        }

        Bool full() const {
            return size_ == capacity_;
        }

        Size size() const {
            return size_;
        }


    private:
        Size capacity_;
        Size front_;
        Size rear_;
        Size size_;
        T* queue_;
    };


}