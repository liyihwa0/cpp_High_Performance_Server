#include "iostream"
#include "thread"
#include "vector"
#include "mutex"
#include "condition_variable"
#include "queue"
#include "functional"
using namespace std;
class ThreadPool{
private:
    bool stop_;
    vector<thread> threads_;
    std::mutex queueMutex_;  // 保护任务队列的互斥量
    std::queue<std::function<void()>> tasks_;  // 任务队列
    std::condition_variable condition_;  // 条件变量，用于通知线程执行任务
public:
    explicit ThreadPool(int threadCount):stop_(false){
        for(int i=0;i<threadCount;i++){
            threads_.emplace_back([this]{
                while(true){
                    std::function<void()> task;
                    {
                        std::unique_lock<mutex> lock(queueMutex_);
                        condition_.wait(lock,[this]{
                            return this->stop_||!this->tasks_.empty();
                        });

                        if(this->stop_&&this->tasks_.empty()){
                            break;
                        }
                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }
                    task();
                }
            });
        }
    }

    void addTask(const function<void()>& task){
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            tasks_.push(task);
            condition_.notify_one();
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            stop_ = true;
        }
        condition_.notify_all();
        for (std::thread& worker : threads_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
};
void task(){
    cout<<"ok"<<endl;
}
int main(){
    ThreadPool t(5);
    t.addTask(task);
    t.addTask(task);
    t.addTask(task);
    t.addTask(task);
    t.addTask(task);
   // sleep(5);

}