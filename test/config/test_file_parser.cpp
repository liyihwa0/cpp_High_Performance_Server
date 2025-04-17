#include <iostream>
#include <fstream>
#include "src/configer/file_parser.h" 
#include "src/global_def.h"
using namespace wa;
void CreateTestIniFile(const String& filename) {
    std::ofstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: Could not create test INI file." << std::endl;
        return;
    }

    // 写入测试内容
    file << "[General]\n";
    file << "name=Test Application\n";
    file << "version=1.0.0\n";
    file << "\n"; // 空行
    file << "[Settings]\n";
    file << "fullscreen=true\n";
    file << "resolution=1920x1080\n";
    file << "# This is a comment\n";
    file << "language=en\n";
    file << "[Advanced]\n";
    file << "debug_mode=false\n";
    file << "max_users=100\n";
    file.close();
}

void TestParseIniFile() {
    String filename = "test.ini";
    CreateTestIniFile(filename);

    // 解析 INI 文件
    SP<ConfigValues> configValues = wa::ParseIniFile(filename);

    // 测试读取值
    Vector<String> path;

    path.push_back("General");
    path.push_back("name");
    String name = configValues->getString(path, "default_name");
    std::cout << "Name: " << name << std::endl; // 应该输出 "Test Application"

    path.pop_back();
    path.push_back("version");
    String version = configValues->getString(path, "default_version");
    std::cout << "Version: " << version << std::endl; // 应该输出 "1.0.0"

    path.pop_back();
    path.push_back("Settings");
    path.push_back("fullscreen");
    Bool fullscreen = configValues->getValue<bool>(path, "false");
    std::cout << "Fullscreen: " << (fullscreen ? "true" : "false") << std::endl; // 应该输出 "true"

    path.pop_back();
    path.push_back("resolution");
    String resolution = configValues->getString(path, "default_resolution");
    std::cout << "Resolution: " << resolution << std::endl; // 应该输出 "1920x1080"

    path.pop_back();
    path.push_back("Advanced");
    path.push_back("debug_mode");
    Bool debugMode = configValues->getValue<Bool>(path, "true");
    std::cout << "Debug Mode: " << (debugMode ? "true" : "false") << std::endl; // 应该输出 "false"

    path.pop_back();
    path.push_back("max_users");
    Int maxUsers = configValues->getValue<Int>(path, 0);
    std::cout << "Max Users: " << maxUsers << std::endl; // 应该输出 "100"
}

int main() {
    TestParseIniFile();
    return 0;
}