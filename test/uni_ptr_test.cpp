#include <iostream>
#include "src/unique_ptr.h" // 假设 `UniPtr.h` 文件中包含了 UPtr 定义

class MyClass {
public:
    MyClass(int val) : value_(val) {
        std::cout << "MyClass constructor: " << value_ << std::endl;
    }

    ~MyClass() {
        std::cout << "MyClass destructor: " << value_ << std::endl;
    }

    void hello() const {
        std::cout << "Hello, MyClass! Value: " << value_ << std::endl;
    }

private:
    int value_;
};

int main() {
    // 测试创建和销毁
    {
        wa::UPtr<MyClass> ptr1(new MyClass(10)); // 自动调用构造函数
        ptr1->hello();  // 使用智能指针访问成员函数
    } // ptr1 离开作用域，析构函数自动调用，释放内存

    // 测试移动语义
    {
        wa::UPtr<MyClass> ptr2(new MyClass(20));
        wa::UPtr<MyClass> ptr3 = std::move(ptr2); // 移动ptr2到ptr3
        ptr3->hello();  // 确认ptr3拥有资源

        // ptr2 现在为空，不再拥有资源
        if (!ptr2.get()) {
            std::cout << "ptr2 no longer owns the resource." << std::endl;
        }
    } // ptr3 离开作用域，析构函数自动调用，释放内存

    // 测试reset和release方法
    {
        wa::UPtr<MyClass> ptr4(new MyClass(30));
        ptr4->hello();
        ptr4.reset(new MyClass(40));  // 重置指针，删除旧对象并创建新对象
        ptr4->hello();

        MyClass* rawPtr = ptr4.release();  // 释放所有权并获得原始指针
        rawPtr->hello();
        delete rawPtr; // 记得手动删除释放的原始指针
    }

    return 0;
}
