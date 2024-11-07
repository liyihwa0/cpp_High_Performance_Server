//
// Created by Liyihwa on 24-11-3.
//

#ifndef LEARNING_UTIL_H
#define LEARNING_UTIL_H
#include "global_def.h"


Tid GetThreadId() {
    return syscall(SYS_gettid);
}


//todo
void ASSERT_FAIL(const char* condition, const char* file, int line,const char *notification) {
    std::cerr << "Assertion failed: " << condition
              << ", file " << file << ", line " << line << " " <<notification<< std::endl;
    // 这里可以选择抛出异常，终止程序，或者自定义处理
    throw std::runtime_error("Assertion failed");
}
// 定义 ASSERT 宏，便于条件检查
#define ASSERT(condition,notification) do {          \
    if (!(condition)) {                 \
        ASSERT_FAIL(#condition, __FILE__, __LINE__,notification); \
    }                                   \
} while (0)



#endif //LEARNING_UTIL_H
