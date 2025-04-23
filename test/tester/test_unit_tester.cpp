#include "src/global_def.h"
#include "src/tester/unit_tester.h"
using namespace wa;

// 假设 testFunc 是您要测试的函数
Int testFunc(String input1, Int input2) {
    // 这里是一个简单的示例实现
    return input1.length() + input2; // 例如返回 input1 的长度加上 input2
}



int main() {
    // 准备输入文件和期望输出文件
    String testFile = "./static/test/tester/t/unit_tester_test1.test"; // 输入文件
    String resultFile = "./static/test/tester/r/unit_tester_test1.result";  //输出文件

    // 创建 UnitTester 实例
    wa::tester::UnitTester uni(testFile, resultFile);

    
    // 测试循环
    try {
        while (uni.inputHasMore()) {
            String input1 = uni.getNext<String>();
            Int input2 = uni.getNext<Int>();
            uni.expectValues(testFunc(input1, input2)); // 验证输出
        }
        uni.cleanUp(); // 清理剩余数据
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}