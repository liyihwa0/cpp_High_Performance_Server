#pragma once

#include <sstream>
#include "../global_def.h"
#include "string"
#include "thread"
#include "functional"
#include "../locker/locker.h"
#include "../smart_ptr.h"
#include "../exception/exception.h"
#include "../util/helper.h"


namespace wa {
    class Thread {
    public:
        // 构造函数
        Thread(Task task, std::string name="");

        // 析构函数
        ~Thread();

        const String& name() const;

        static const String& Name();

        static void SetName(const std::string &name);

        static ID Id();

        static Thread* This();

        ID id() const;

        Bool isRunning() const;

        PThread pThread() const;

        Bool cancel() const;

        Bool detach() const;

        // 等待线程完成
        Bool join();

    private:
        static thread_local Thread* thisThread_;

        // 线程入口函数
        static Void *run(Void *arg);

        // 禁止拷贝构造
        Thread(const Thread &) = delete;

        // 禁止移动构造
        Thread(Thread &&) = delete;

        ID id_;
        pthread_t thread_;
        Task task_;
        String name_;
        Bool isRunning_;
        Semaphore sem_;
    };
}
