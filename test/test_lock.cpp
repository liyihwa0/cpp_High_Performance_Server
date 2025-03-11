#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include "src/thread/thread.h"
#include "src/locker/locker.h"  // 引入你定义的 SpinLock 等类
#include "src/global_def.h"
#include "src/smart_ptr.h"
// 共享的计数器
int counter(0);
using namespace wa;
using namespace std;
// 创建 SpinLock 实例
SpinLock spinLock;

void incrementCounter() {
    // 每个线程都尝试加锁并更新计数器
    for (int i = 0; i < 10000000; ++i) {
        auto lock= AsScopedLock(spinLock);
        ++counter;        // 修改共享资源
    }
}

int main() {
    {
        std::vector<UP<Thread>> threads;
        // 创建多个线程来增加计数器
        for (int i = 0; i < 5; ++i) {
            threads.emplace_back(new Thread(incrementCounter));
            threads.back()->start();
        }

        // 等待所有线程完成
        for (auto& t : threads) {
            std::cout<<t->join()<<std::endl;
        }


        // 输出最终的计数值
        std::cout << "Final counter value: " << counter << std::endl;
    }

    return 0;
}
