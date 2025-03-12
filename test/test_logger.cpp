#include "src/global_def.h"
#include "src/logger/logger.h"
#include "vector"
#include "iostream"
using namespace wa;
std::vector<UP<Thread> > threads;
void task(){
    for(int i=0;i<100;i++){
        LOG_INFO(gl)<<"123456"<<"778899";
    }
}

int main(){

    for(int i=0;i<100;i++){
        threads.emplace_back(new Thread(task));
    }


    for(int i=0;i<100;i++){
        threads[i]->join();
    }
}