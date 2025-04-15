#include <iostream>
#include <fstream>
#include <string>
#include "src/global_def.h" // 确保包含你的定义
#include "src/util/caster.h" // 确保包含你的转换工具
#include "src/tester/unit_tester.h" // 假设 DataInputer 类在这个头文件中
#include "src/util/helper.h"
namespace wa {
    namespace tester {
        void testDataInputer() {
            cout<<Helper::Pwd()<<endl;
            // 创建一个测试文件并写入数据
            const std::string testFileName = "test_data.txt";
            std::ofstream testFile(testFileName);
            if (!testFile) {
                std::cerr << "Failed to create test file." << std::endl;
                return;
            }

            // 写入测试数据
            testFile << "Hello World\n";
            testFile << "\"This is a test\"\n";
            testFile << "Another line\n";
            testFile << "\"Multi-line\nString\"\n";
            testFile << "End of test\n";
            testFile.close();

            // 使用 DataInputer 读取文件
            try {
                DataInputer dataInputer(testFileName);

                while (dataInputer.hasMore()) {
                    std::string value = dataInputer.getNext<std::string>();
                    std::cout << "Read value: " << value << std::endl;
                }
                dataInputer.getNext<String>();
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
            

        }
    }
}

int main() {
    wa::tester::testDataInputer();
    return 0;
}