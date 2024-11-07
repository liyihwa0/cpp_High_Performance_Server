#include"global_def.h"
#include<iostream>


class Buffer {
private:
    U8 *buffer_;
    Size length_;
    Size startIndex_;  // 当前读取位置
    Size endIndex_;
    Bool needFree_;
    ~Buffer(){
        if(needFree_){
            delete[] buffer_;
        }
    }
    Buffer(){}
public:
    Buffer(Size length): length_(length){
        buffer_=new U8[length];
        needFree_=true;
    }

    static Buffer& Wrap(U8* buffer, Size length){ //用Buffer对已有的缓冲区包装
        Buffer bf=Buffer();
        bf.startIndex_=0;
        bf.endIndex_=length-1;
        bf.buffer_=buffer;
        bf.length_=length;
        bf.needFree_=false;
        return bf;
    }

    // 检查是否还有剩余数据可供读取
    Bool hasMore() const {
        return startIndex_ < length_;
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

    Size find(U8 key) const{
        for(Size i=startIndex_;i<endIndex_;i++){
            if(buffer_[i]==key){
                return i;
            }
        }
        return endIndex_;
    }

    Size find(U8 key, Size fromIndex) const{
         for(Size i=fromIndex;i<endIndex_;i++){
            if(buffer_[i]==key){
                return i;
            }
        }
        return endIndex_;
    }
    
    U8* getBuffer() const {
        return buffer_;
    }

    Size startIndex() const {
        return startIndex_;
    }
    
    Size endIndex() const {
        return endIndex_;
    }

    // 重载 [] 操作符，允许通过索引访问缓冲区中的字符
    U8 operator[](Size index) const {
        if (index >= length_) {
            throw std::out_of_range("Index out of bounds");
        }
        return buffer_[index];
    }
};