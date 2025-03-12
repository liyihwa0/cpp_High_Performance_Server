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
    std::queue<function<void()>> tasks_;
    std::mutex lock_;
    std::vector<std::thread> threads_;
    std::condition_variable conditionVariable_;
public:
    explicit ThreadPool(int threadCount):stop_(false){
        for(int i=0;i<threadCount;i++){
            threads_.emplace_back( [this]{
                while(true){
                    std::unique_lock<std::mutex> uniqueLock(lock_);

                    while(!stop_&& tasks_.empty()){
                        conditionVariable_.wait(uniqueLock);
                    }

                    if(stop_){
                        return ;
                    }
                    function<void()> task;
                    if(!tasks_.empty()){
                        task=std::move(tasks_.front());
                        tasks_.pop();
                        uniqueLock.unlock();
                    }

                    task();
                }
            });
        }
    }

    void addTask(const function<void()>& task){
        std::unique_lock<std::mutex> uniqueLock(lock_);
        if(stop_){
            return;
        }
        tasks_.push(task);
        conditionVariable_.notify_one();
    }

    void stop(){
        std::unique_lock<std::mutex> uniqueLock(lock_);
        stop_= true;
        conditionVariable_.notify_all();
    }

    ~ThreadPool() {
        stop();
        for(auto &it:threads_){
            if(it.joinable())
                it.join();
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
    sleep(1);

}