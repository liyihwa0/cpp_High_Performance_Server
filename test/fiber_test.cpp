#include "src/fiber.h"
#include "iostream"
#include <mcheck.h>

void task1() {
    std::cout << Fiber::Id()<<" ""Task 1 started!" << std::endl;
    Fiber::AwaitTime(2000);  // 等待2秒
    std::cout << "Task 1 resumed after 2 seconds!" << std::endl;
}

void task2() {
    std::cout <<Fiber::Id()<<" " "Task 2 started!" << std::endl;
    Fiber::AwaitTime(3000);  // 等待3秒
    std::cout << "Task 2 resumed after 3 seconds!" << std::endl;
}

int main() {

    try {

        // 创建并添加任务1和任务2
        Fiber::AddTask(task1);
        Fiber::AddTask(task2);

        // 启动Fiber框架
        Fiber::Start(2);

        std::cout << "All tasks completed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }


    try {

        // 创建并添加任务1和任务2
        Fiber::AddTask(task1);
        Fiber::AddTask(task2);

        // 启动Fiber框架
        Fiber::Start(10);

        std::cout << "All tasks completed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}