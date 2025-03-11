#include "src/global_def.h"
#include "src/data_structure/skiplist.h"
#include <iostream>
using namespace std;
using namespace wa;

int main() {
    // 初始化跳表，晋升概率为0.5，最大高度为10
    SkipList<int, int> sl(0.8, 10);

    // 测试 add 方法
    cout << "Testing add method..." << endl;
    sl.add(1, 100);
    sl.add(2, 200);
    sl.add(3, 300);

    sl.add(4, 400);

    sl.add(5, 500);

    std::cout<<sl.dump()<<std::endl;

    // 测试 get 方法
    cout << "\nTesting get method..." << endl;
    cout << "Get key 1: " << sl.get(5, -1) << endl; // 应该输出 300
    cout << "Get key 6 (not exists): " << sl.get(6, -1) << endl; // 应该输出 -1

    // 测试 set 方法
    cout << "\nTesting set method..." << endl;
    sl.add(3, 350); // 假设 set 方法可以通过 add 实现
    sl.set(6, 600); // 插入新的键值对



    // 验证 set 和 add 的效果
    cout << "\nGet key 3 after update: " << sl.get(3, -1) << endl; // 应该输出 350
    cout << "Get key 6 after adding: " << sl.get(6, -1) << endl;   // 应该输出 600
    std::cout<<sl.dump()<<std::endl;

    return 0;
}
