#include "src/fiber/fiber.h"
#include "src/fiber/channel.h"
using namespace std;
using namespace wa;
void test1(){
    Channel<String> channel(16);
    Fiber::AddTask([&channel]{
        int i=20000000;
        while(i--){
            channel.write("123");
            channel.write("321");
        }
        channel.close();
    },"writer");
    Fiber::AddTask([&channel]{
        int len=0;
        while(true){
            String res;
            if(!channel.read(res)){
                break;
            }
            len+=res.length();
        }
        cout<<len<<endl;
    },"reader");
    auto now=Timer::Now();
    Fiber::Start(1);
    cout<<(Timer::Now()-now).format("%M:%S")<<endl;
}
void test2(){
    // 创建三个管道
    Channel<String> channel1(1);
    Channel<String> channel2(1);
    Channel<String> channel3(1);

    // 创建三个生产者协程，分别写入不同的管道
    Fiber::AddTask([&channel1] {
        int i = 1000;
        while (i--) {
            channel1.write("Message from Producer 1: " + to_string(i));
        }
        channel1.close();

    }, "Producer 1");

    Fiber::AddTask([&channel2] {
        int i = 1000;
        while (i--) {
            channel2.write("Message from Producer 2: " + to_string(i));
        }
        channel2.close();
    }, "Producer 2");

    Fiber::AddTask([&channel3] {
        int i = 1000;
        while (i--) {
            channel3.write("Message from Producer 3: " + to_string(i));
        }
        channel3.close();
    }, "Producer 3");

    // 创建三个消费者协程，分别从不同的管道读取数据
    Fiber::AddTask([&channel1] {
        while (true) {
            String message;

            if(!channel1.read(message)){
                LOG_INFO(gl)<<"Consumer 1 exit " ;
                break;
            }
            LOG_INFO(gl)<<"Consumer 1 received: " << message;
        }
    }, "Consumer 1");

    Fiber::AddTask([&channel2] {
        while (true) {
            String  message;
            if(!channel2.read(message)){
                LOG_INFO(gl)<<"Consumer 2 exit " ;
                break;
            }
            LOG_INFO(gl)<<"Consumer 2 received: " << message;
        }
    }, "Consumer 2");
    Fiber::AddTask([&channel3] {
        while (true) {
            String  message;
            if(!channel3.read(message)){
                LOG_INFO(gl)<<"Consumer 3 exit";
                break;
            }
            LOG_INFO(gl)<<"Consumer 3 received: " << message;
        }
    }, "Consumer 3");

    // 启动协程
    Fiber::Start(3);
}
int main(){
    gl->setLevel(wa::LogLevel::INFO);
    test1();

}