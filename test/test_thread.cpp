#include "src/thread/thread.h"
#include "src/global_def.h"
#include <iostream>
#include <vector>

using namespace wa;
int a=0;
void TestTask() {
    for(int i=0;i<100000;i++){
        ++a;
    }
}

void TestThread() {
    const int threadCount = 5;

    std::vector<Thread*> threads;

    // 创建并启动多个线程
    for (int i = 0; i < threadCount; i++) {
        // 创建任务对象
        Task task = TestTask;

        // 创建线程对象并启动
        threads.push_back(new Thread(task));
    }



    // 等待所有线程完成
    for (auto& thread : threads) {
        thread->join();  // 等待线程结束
    }

    for(int i=0;i<threadCount;i++){
        delete threads[i];
    }
    threads.clear();


}

int main() {
    TestThread();  // 调用测试函数，创建并启动线程
    std::cout<<a;
    return 0;
}
