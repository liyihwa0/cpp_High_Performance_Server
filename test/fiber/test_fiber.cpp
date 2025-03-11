#include "src/fiber/fiber.h"
#include "src/logger/logger.h"
#include "iostream"
using namespace wa;
void task3();
FiberLock fl;
void task1() {
    LOG_INFO(gl)<<"started!" ;
    // task1持有锁
    fl.lock();
    LOG_INFO(gl)<<"get Lock";
    Fiber::AwaitTime(Timer(3*Timer::Unit::S));
    LOG_INFO(gl)<<"release Lock";
    fl.unlock();
}
// 任务函数，每个协程打印一个数字
void task() {
    // 获取锁，确保每次只有一个协程在打印
    fl.lock();
    LOG_INFO(gl) << " started!";
    //Fiber::AwaitTime(Timer(1 * Timer::Unit::MS));  // 模拟一些工作，暂停1ms
    LOG_INFO(gl) << " finished!";
    fl.unlock();  // 释放锁
}

void task2() {
    Fiber::AwaitTime(Timer(3*Timer::Unit::S));
    fl.lock();
    LOG_INFO(gl)<<"get Lock";
    Fiber::AwaitTime(Timer(2*Timer::Unit::MS));
    LOG_INFO(gl)<<"release Lock";
    fl.unlock();
}


#include "unordered_map"
std::unordered_map<String ,int> mp;

int main() {

    try {
        // 创建并添加100个任务
        for (int i = 0; i < 100; ++i) {
            Fiber::AddTask(task);
        }

        // 启动Fiber框架，最多启动2个协程
        Fiber::Start(10);

        std::cout << "===========================================" << std::endl;
    } catch (const std::exception& e) {
        mp[e.what()]++;
        for (auto it : mp) {
            std::cerr << it.first << " " << it.second << std::endl;
        }
    }

    return 0;
}