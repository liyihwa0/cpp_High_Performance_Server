#include <iostream>
#include <thread>
#include <vector>
#include "src/logger/logger.h"
using namespace wa;

void writeLogs(LogAppender& appender, const std::string& threadName) {
    for (int i = 0; i < 100; ++i) {
        appender << "[" << threadName << "] Message " << threadName << std::endl;
    }
}


int main() {
    try {
        FileLogAppender fileAppender("test.log");
        StdLogAppender consoleAppender;

        // 创建多个线程同时写入日志
        std::vector<std::thread> threads;

        // 线程写入文件日志
        for (int i = 0; i < 3; ++i) {
            threads.emplace_back(writeLogs, std::ref(fileAppender), "FileThread" + std::to_string(i));
        }

        // 线程写入控制台日志
        for (int i = 0; i < 2; ++i) {
            threads.emplace_back(writeLogs, std::ref(consoleAppender), "ConsoleThread" + std::to_string(i));
        }

        // 等待所有线程完成
        for (auto& t : threads) {
            t.join();
        }

        std::cout << "Logging completed!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
