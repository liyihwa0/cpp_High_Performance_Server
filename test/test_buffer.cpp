#include "src/buffer.h"
#include "iostream"
int main(){
    wa::Buffer bf(10);
    bf.write32(1024);

    std::cout<<bf.read32();

}