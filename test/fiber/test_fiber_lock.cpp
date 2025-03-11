#include "src/fiber/fiber.h"
#include "src/timer.h"
#include "src/logger/logger.h"
using namespace std;
using namespace wa;
FiberLock lock;
int counter=0;
void test(){
    for(int i=0;i<100;i++){
        lock.lock();
        counter++;
        lock.unlock();
    }
}
int main(){
    gl->setLevel(wa::LogLevel::WARN);
    for(int th=1;th<=10;th++){
        Timer t(Timer::Now());
        for(int i=0;i<=100;i++){
            Fiber::AddTask(test);
        }
        Fiber::Start(th);
        cout<<th<<" "<<(Timer::Now()-t).getMS()<<std::endl;
    }


}