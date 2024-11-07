//
// Created by Liyihwa on 24-11-7.
//

#ifndef CPP_HIGH_PERFORMANCE_SERVER_UNIQUE_PTR_H
#define CPP_HIGH_PERFORMANCE_SERVER_UNIQUE_PTR_H
#include "global_def.h"
namespace wa{
    // 唯一指针
    template <typename T>
    class UPtr {
    public:
        // 构造函数
        explicit UPtr(T* ptr = nullptr) : ptr_(ptr) {}

        // 移动构造函数 (独占所有权)
        // uptr(xxx);
        UPtr(UPtr&& other) noexcept : ptr_(other.ptr_) {
            other.ptr_ = nullptr; // 防止原对象析构时释放资源
        }

        // 移动赋值运算符 (独占所有权)
        // uptr= xxx;
        UPtr& operator=(UPtr&& other)=delete;

        // 禁用复制构造函数 (确保独占所有权)
        UPtr(const UPtr& other) = delete;

        // 禁用复制赋值运算符 (确保独占所有权)
        UPtr& operator=(const UPtr& other) = delete;

        // 析构函数
        ~UPtr() {
            delete ptr_; // 自动销毁所管理的对象
        }

        // 解引用运算符
        T& operator*() const {
            return *ptr_;
        }

        // 指针访问运算符
        T* operator->() const {
            return ptr_;
        }

        // 获取原始指针
        T* get() const {
            return ptr_;
        }

        // 释放所有权并返回原始指针
        T* release() {
            T* temp = ptr_;
            ptr_ = nullptr;
            return temp;
        }

        // 重置指针并销毁原对象
        Void reset(T* ptr = nullptr) {
            delete ptr_;
            ptr_ = ptr;
        }

    private:
        T* ptr_; // 原始指针
    };
};

#endif //CPP_HIGH_PERFORMANCE_SERVER_UNIQUE_PTR_H
