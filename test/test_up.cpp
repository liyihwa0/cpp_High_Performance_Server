//
// Created by Liyihwa on 24-11-15.
//

#include <atomic>
#include "src/util/timer.h"
#include "iostream"
#include "src/global_def.h"
#include "src/thread/thread.h"
#include "src/logger/logger.h"

using namespace std;
namespace test{
    template<class T>
    struct PointerDeleter{
        static void Delete(T*t){
            delete t;
        }
    };
    template<class T,class Deleter=PointerDeleter<T>>
    class SP{
        struct SpNode{
            T* ptr_;
            atomic<int> count_;
        };
        SpNode* node_;
    public:
        // 构造
        explicit SP(T* ptr= nullptr){
            if(ptr){
                node_=new SpNode{ptr,1};
            }else{
                node_= nullptr;
            }
        }


        // 拷贝构造
        SP(const SP& other)noexcept{
            node_=other.node_;
            if(node_){
                node_->count_.fetch_add(1);
            }
        }

        // 移动构造
        SP(SP&& other)noexcept{
            node_=other.node_;
            if(other.node_){
                other.node_= nullptr;
            }
        }

        // 拷贝赋值
        SP& operator=(const SP& other) noexcept{
            if(this==&other||this->node_==other.node_) return *this;
            release();
            node_=other.node_;
            if(node_){
                node_->count_.fetch_add(1);
            }
            return *this;
        }

        // 移动赋值
        SP& operator=(SP&& other) noexcept {
            if(this==&other || this->node_==other.node_) return *this;
            release();
            node_=other.node_;
            other.node_== nullptr;
            return *this;
        }


        T* get(){
            if(node_){
                return node_->ptr_;
            }
            return nullptr;
        }

        void release() noexcept{
            if(node_){
                if(node_->count_.fetch_sub(1)==1){
                    Deleter::Delete(node_->ptr_);
                    delete node_;
                    node_= nullptr;
                }
            }
        }

        T* operator->(){
            return node_->ptr_;
        }

        // 检查是否为空
        explicit operator bool() const {
            return node_ != nullptr;
        }

        void reset(T* ptr) noexcept{
            release();
            node_=new SpNode({ptr,1});
        }

        int count() noexcept{
            return node_->count_.load(std::memory_order_relaxed);
        }

        ~SP(){
            release();
        }
    };



};
using namespace test;
// 测试自定义类
struct Test {
    int value;
    Test(int v) : value(v) {
        cout << "Test object created with value: " << value << endl;
    }
    ~Test() {
        cout << "Test object destroyed with value: " << value << endl;
    }
};
// 自定义删除器
struct CustomDeleter {
    static void Delete(Test* t) {
        cout << "CustomDeleter called for value: " << t->value << endl;
        delete t;
    }
};
SP<Test> testSp(new Test(1));
void threadTest(){
    for(int i=0;i<100;i++){
        SP<Test> threadTask(testSp);
        std::cout<<threadTask->value<<std::endl;
    }
}
int main() {
    for(int i=0;i<100;i++){
        wa::Thread t(threadTest);
        t.join();
    }

    testSp.release();
    LOG_INFO(wa::gl)<<"asbc";
    return 0;
}
