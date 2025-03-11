
#include "src/fiber/fiber.h"
#include "src/timer.h"
#include "iostream"
#include "src/logger/logger.h"
using namespace wa;

void task1() {
    sleep(2);
    LOG_INFO(gl)<<"before  AwaitTime";
    Fiber::AwaitTime(Timer(3*Timer::Unit::MS));
    LOG_INFO(gl)<<"after AwaitTime";
}


int main(){
    for(int i=0;i<100;i++){
        Fiber::AddTask(task1);
    }
    Fiber::Start(2);
}