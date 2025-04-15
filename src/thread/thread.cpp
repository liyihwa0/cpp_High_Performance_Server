#include "./thread.h"

namespace wa {

    // 构造函数
    Thread::Thread(Task task, std::string name) :
            task_(std::move(task)), name_(std::move(name)), isRunning_(FALSE), id_(0), sem_(0) {
        Int rt = pthread_create(&thread_, nullptr, &Thread::run, this);
        if (rt) {
            throw OS_ERRNO_EXCEPTION(rt);
        }
        // 等待 run 中设置好 thisThread 的信息
        sem_.wait();
    }

    // 析构函数
    Thread::~Thread() {
        detach();
    }

    // 获取线程名称
    const String& Thread::name() const {
        return name_;
    }

    // 获取当前线程的名称
    const String& Thread::Name() {
        return This()->name_;
    }

    // 设置线程名称
    void Thread::SetName(const std::string &name) {
        thisThread_->name_ = name;
    }

    Thread* Thread::This(){
        if(thisThread_!= nullptr){ [[likely]]
            return thisThread_;
        }else{
            thisThread_=new Thread(Task{},"NONE");
        }
        return thisThread_;
    }

    // 获取当前线程的 ID
    ID Thread::Id() {
        return This()->id_;
    }

    // 获取当前对象的线程 ID
    ID Thread::id() const {
        return id_;
    }

    // 判断线程是否正在运行
    Bool Thread::isRunning() const {
        return isRunning_;
    }

    // 获取线程 ID
    PThread Thread::pThread() const {
        return thread_;
    }

    // 取消线程
    Bool Thread::cancel() const {
        if (isRunning_) {
            return pthread_cancel(thread_) == 0;
        }
        return TRUE;
    }

    // 分离线程
    Bool Thread::detach() const {
        if (isRunning_) {
            Bool rt = pthread_detach(thread_) == 0; // 等待线程结束
            return rt;
        }
        return TRUE;
    }

    // 等待线程完成
    Bool Thread::join() {
        if (isRunning_) {
            Bool rt = pthread_join(thread_, nullptr) == 0; // 等待线程结束
            isRunning_ = FALSE;
            return rt;
        }
        return TRUE;
    }

    // 线程入口函数
    Void* Thread::run(Void *arg) {
        thisThread_ = (Thread *)arg;
        thisThread_->isRunning_ = TRUE;
        thisThread_->id_ = Helper::GetThreadId();
        if (thisThread_->name_.empty()) {
            std::stringstream ss;
            ss << "t" << thisThread_->id_;
            thisThread_->name_ = ss.str();
        }

        thisThread_->sem_.post();

        if (thisThread_->task_) {
            thisThread_->task_(); // 调用传入的任务
        }

        thisThread_->isRunning_ = FALSE;
        return nullptr;
    }
     thread_local Thread*  Thread::thisThread_= nullptr;

} // namespace wa
