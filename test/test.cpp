#include "iostream"
struct Example {
    char a;    // 1字节
    char c;    // 1字节
    int b;     // 4字节，对齐到4字节边界
};
using namespace std;
int main(){
    std::cout<<sizeof (Example);
}