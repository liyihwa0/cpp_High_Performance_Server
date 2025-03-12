
#include <iostream>
#include "src/exception/exception.h"
void f(const String&s){
    std::cout<<"f left"<<std::endl;
}
void f(String&&s){
    std::cout<<"f right"<<std::endl;
}
void g(const String&s){
    std::cout<<"g left"<<std::endl;
    f(s);
}
void g(String&&s){
    std::cout<<"g right"<<std::endl;
    f(s);
}

int main() {
    String left("left");
    g(left);

    g(String("right"));
    return 0;
}
