#pragma once
#include "iostream"
#include <fstream>
#include "../smart_ptr.h"
using namespace std;
namespace wa{
    class FileReader {
    private:
        String filename_;               // 文件名
        ifstream file_;                 // 文件流
        UP<String> currentLine_; // 当前读取的行

        // 读取一行，返回一个 String*
        String * readLine() {
            if(file_.eof()){
                return NULLPTR;
            }
            String *line=new String ();
            Char ch;
            while (file_.get(ch)) { // 逐字符读取
                if (ch == '\n') {
                    break; // 遇到换行符停止读取
                }
                line->push_back(ch); // 将字符添加到行中
            }
            return line; // 返回读取的行
        }

    public:
        // 构造函数
        FileReader(const string& filename)
                : filename_(filename) {
            file_.open(filename_);
            if (!file_.is_open()) {
                throw runtime_error("Could not open file: " + filename_);
            }
            // 读取第一行
            currentLine_.reset(readLine());
        }

        // 读取一行
        UP<String > nextLine() {
            UP<String > rt=currentLine_;
            currentLine_.reset(readLine());
            return rt; // 返回当前行
        }

        // 读取剩余的行
        UP<Vector<UP<String >>> lastLines() {
            UP<Vector<UP<String >>> lines(new Vector<UP<String>>);
            if(currentLine_.get()){
                lines->emplace_back(std::move(currentLine_));
            }

            String * line;

            while ((line = readLine())) {
                lines->emplace_back(line); // 将读取的行添加到 vector 中
            }

            return lines; // 返回所有读取的行
        }

        // 检查是否还有更多行可读
        Bool hasMore() const {
            return currentLine_.get() != NULLPTR; // 如果当前行不为空，则还有更多行
        }

        // 返回文件名
        String filename() const {
            return filename_;
        }

        // 关闭文件
        void close() {
            if (file_.is_open()) {
                file_.close();
            }
        }

        // 析构函数，确保文件在对象销毁时关闭
        ~FileReader() {
            close(); // 确保文件关闭
        }
    };
}