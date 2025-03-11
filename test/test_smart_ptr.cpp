#include <iostream>
#include "src/smart_ptr.h" // 假设 `UniPtr.h` 文件中包含了 UP 定义
using namespace std;



class TestClass {
public:
    TestClass(int x, const std::string& str) : x_(x), str_(str) {}

    void print() const {
        std::cout << "TestClass: x = " << x_ << ", str = " << str_ << std::endl;
    }

    ~TestClass(){
        std::cout << "TestClass: x = " << x_ << "destroy" <<std::endl;
    }
private:
    int x_;
    std::string str_;
};

using namespace wa;

// 函数：接收SP作为参数并访问其中的对象
void processSP(SP<TestClass> sp) {
    std::cout << "Inside processSP function:\n";
    sp->print();  // 使用指针访问运算符
}

void processUP(UP<TestClass> up){
    up->print();
}

int main() {

    {
        UP<TestClass> t1;
        UP<TestClass> t2=t1;

    }
    {
        UP<int[]> sp2(new int[3]{1, 2, 3});  // 正确：通过 new[] 动态分配并初始化数组
        cout<<sp2[1]<<sp2[2]<<endl;


    }

    {


        // 创建一个SP对象
        SP<TestClass> sp1(new TestClass(1, "Hello"));
        sp1->print();  // 使用指针访问运算符
        processSP(sp1); //这里会调用拷贝构造


        // 使用拷贝构造函数
        SP<TestClass> sp2 = sp1;
        sp2->print();

        sp2=sp1;

        // 使用移动构造函数
        SP<TestClass> sp3 = std::move(sp1);
        sp3->print();

        // 使用拷贝赋值运算符
        SP<TestClass> sp4;
        sp4 = sp2;
        sp4->print();


        SP<TestClass> sp5;
        sp5=SP(new TestClass(1, "Hello"));
        sp5->print();
    }

    auto sp= SP<TestClass>(new TestClass(2,"123"));
    sp->print();

    auto up=UP<TestClass>(new TestClass(1,"up"));
    processUP(std::move(up));
    return 0;
}

