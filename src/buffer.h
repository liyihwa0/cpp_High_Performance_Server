#pragma once
#include"./global_def.h"
#include "./exception/exception.h"
#include "./smart_ptr.h"
#include<iostream>

namespace wa{
    class Buffer {
    private:
        SP<U8[]> buffer_;
        Size capacity_;
        Size startIndex_;  // 当前读取位置
        Size endIndex_;
    public:
        Buffer(SP<U8[]> buffer,Size length):
                buffer_(std::move(buffer)),
                capacity_(length),
                startIndex_(0),
                endIndex_(0){}
        Buffer(Int capacity):
        capacity_(capacity),
        buffer_(new U8[capacity]),
        startIndex_(0),
        endIndex_(0){}

        Buffer(const String& string) {
            capacity_ = string.size();  // 设置缓冲区大小为字符串长度
            buffer_ = SP<U8[]>(new U8[capacity_]);  // 创建一个新的缓冲区
            startIndex_ = 0;
            endIndex_ = capacity_;

            // 拷贝字符串数据到缓冲区
            for (Size i = 0; i < capacity_; ++i) {
                buffer_[i] = static_cast<U8>(string[i]);
            }
        }


        // 检查是否还有剩余数据可供读取
        Bool hasMore() const {
            return endIndex_>startIndex_;
        }

        Int readableLength() const {
            return endIndex_-startIndex_;
        }

        Int writeableLength() const{
            return capacity_-endIndex_;
        }

        Int capacity() const{
            return capacity_;
        }

        // 返回当前字符，但不移动startIndex
        U8 peek() const {
            if (!hasMore()) {
                throw std::out_of_range("No more data in the buffer");
            }
            return buffer_[startIndex_];
        }

        U8 pop(){
            if (!hasMore()) {
                throw std::out_of_range("No more data in the buffer");
            }
            return buffer_[startIndex_++];
        }

        U8* readableAddr(){
            return buffer_.get()+startIndex_;
        }

        U8* writeableAddr(){
            return buffer_.get()+endIndex_;
        }

        void advanceRead(Int length){
            if(startIndex_+length>endIndex_){
                throw CODE_LOCATION_EXCEPTION("newStartIndex is out of bounds");
            }
            startIndex_+=length;
        }

        void advanceWrite(Int length){
            if(endIndex_+length>capacity_){
                throw CODE_LOCATION_EXCEPTION("newEndIndex is out of bounds");
            }
            endIndex_+=length;
        }


        // 重载 [] 操作符，允许通过索引访问缓冲区中的字符
        U8 operator[](Size index) const {
            if (index >= capacity_) {
                throw std::out_of_range("Index out of bounds");
            }
            return buffer_[index];
        }

        void clear(){
            startIndex_=endIndex_=0;
        }

        void copy(Buffer& dest, Int length) {
            // 检查 a 和 b 是否有足够的数据和空间
            if (this->readableLength() < length) {
                throw std::out_of_range("Not enough data in source buffer to copy.");
            }
            if (dest.writeableLength() < length) {
                throw std::out_of_range("Not enough space in destination buffer.");
            }

            // 执行数据拷贝
            std::memcpy(dest.writeableAddr(), this->readableAddr(), length);

            // 更新 destination buffer 的 endIndex
            dest.advanceWrite(length);
        }

        String toString() const {
            std::string result;
            // 以十六进制格式输出
            for (Size i = startIndex_; i < endIndex_; ++i) {
                // 直接将每个字节作为 ASCII 字符追加到字符串中
                result += static_cast<char>(buffer_[i]);
            }
            return result;
        }

        String subString(Size start, Size length) const {
            // 确保起始位置和长度在有效范围内
            if (start + length + startIndex_ > endIndex_) {
                throw std::out_of_range("SubString range is out of bounds.");
            }

            // 使用 std::string 构造子字符串
            std::string result;
            for (Size i = startIndex_ + start; i <  startIndex_ + start + length; ++i) {
                result += static_cast<char>(buffer_[i]);
            }

            return result;
        }

        void write32(U32 value){
            // 检查缓冲区是否有足够的空间写入4个字节
            if (writeableLength() < sizeof(U32)) {
                throw CODE_LOCATION_EXCEPTION("No more space to write 32bit");
            }

            // 直接将数据写入缓冲区
            std::memcpy(writeableAddr(), &value, sizeof(value));

            // 更新endIndex_，表示已经写入了4个字节
            advanceWrite(sizeof(U32));
        }

        U32 read32(){
            // 检查缓冲区是否有足够的数据可供读取
            if (readableLength() < sizeof(U32)) {
                throw CODE_LOCATION_EXCEPTION("No more space to read 32bit");
            }

            // 从缓冲区读取4个字节
            U32 value;
            std::memcpy(&value, readableAddr(), sizeof(value));

            // 更新startIndex_，表示已经读取了4个字节
            advanceRead(sizeof(U32));

            return value;
        }

        ~Buffer()=default;
    };
}
