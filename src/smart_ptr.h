//
// Created by Liyihwa on 24-11-7.
//

#pragma once
#include "./global_def.h"
#include "atomic"

namespace wa{

    template<typename  T>
    class PointerDeleter{
    public:
        void static DoDelete(T *ptr){
            delete ptr;
        }
    };
    template<typename  T>
    class ArrayDeleter{
    public:
        void static DoDelete(T *ptr){
            delete[] ptr;
        }
    };



    // 独占指针
    template <typename T,typename Deleter>
    class UPBase {
    public:
        explicit UPBase(T* ptr = nullptr) : ptr_(ptr) {}



        UPBase(UPBase& other):ptr_(other.ptr_){
            other.ptr_= nullptr;
        }

        UPBase& operator=(UPBase& other){
            if (this != &other) {
                // 释放当前资源
                delete ptr_;
                ptr_ = other.ptr_;
                other.ptr_ = nullptr; // 防止原对象析构时释放资源
            }
            return *this;
        }

        UPBase(UPBase&& other) noexcept : ptr_(other.ptr_) {
            other.ptr_ = nullptr; // 防止原对象析构时释放资源
        }
        UPBase& operator=(UPBase&& other) noexcept {
            if (this != &other) {
                delete ptr_;  // 释放当前资源
                ptr_ = other.ptr_;  // 移动资源
                other.ptr_ = nullptr;  // 防止原对象析构时释放资源
            }
            return *this;
        }

        // 析构函数
        ~UPBase() {
            Deleter::DoDelete(ptr_);
            ptr_= nullptr;
        }


        // 获取原始指针
        T* get() const noexcept{
            return ptr_;
        }

        // 重置指针并销毁原对象
        void reset(T* ptr = nullptr) noexcept{
            delete ptr_;
            ptr_ = ptr;
        }

    protected:
        T* ptr_; // 原始指针
    };

    template<typename T,typename Deleter>
    class SP;

    template<typename T,typename Deleter=PointerDeleter<T>>
    class UP:public UPBase<T,Deleter >{
    friend SP<T,PointerDeleter<T>>;
    public:
        UP(T* ptr= nullptr) : UPBase<T, PointerDeleter<T>>(ptr){}

        // 克隆方法
        UP clone() const {
            return UP(this->ptr_ ? new T(*this->ptr_) : nullptr); // 深拷贝
        }

        // 解引用运算符
        T& operator*() const {
            return *this->ptr_;
        }

        // 指针访问运算符
        T* operator->() const {
            return this->ptr_;
        }
    };

    template<typename T>
    class UP<T[]>:public UPBase<T,ArrayDeleter<T> >{
    public:
        friend SP<T[],ArrayDeleter<T> >;
        explicit UP(T* ptr = nullptr) : UPBase<T, ArrayDeleter<T>>(ptr){}

        T& operator[](Size idx) const {
            return this->ptr_[idx];
        }
    };


    template <typename T, typename Deleter>
    class SPBase {
    protected:
        class SPNode {
        public:
            T* resource_;               // 资源指针
            std::atomic<Int> count_;   // 原子引用计数
            SPNode(T* resource,Int count):resource_(resource), count_(count){}
        };

        SPNode* node_;  // 资源和计数器的节点

    public:
        // 默认构造函数
        explicit SPBase(T* ptr = nullptr) noexcept
        :node_(ptr? new SPNode(ptr,1): nullptr){}


        // 拷贝构造函数
        SPBase(const SPBase& other) noexcept {
            acquire(other);
        }

        // 移动构造函数
        SPBase(SPBase&& other) noexcept {
            node_ = other.node_;
            other.node_ = nullptr;
        }

        // 拷贝赋值运算符
        SPBase& operator=(const SPBase& other) noexcept {
            if (this != &other) [[likely]] {
                release();
                acquire(other);
            }
            return *this;
        }

        // 移动赋值运算符
        SPBase& operator=(SPBase&& other) noexcept {
            if (this != &other) [[likely]] {
                release();
                node_ = other.node_;
                other.node_ = nullptr;
            }
            return *this;
        }

        // 析构函数
        ~SPBase() {
            release();
        }

        // 重置 shared_ptr
        void reset(T* ptr = nullptr) {
            release();
            if (ptr) {
                node_ = new SPNode(ptr,1);
            } else {
                node_ = nullptr;
            }
        }

        // 获取原始指针
        T* get() const {
            return node_ ? node_->resource_ : nullptr;
        }

    protected:
        // 增加引用计数
        void acquire(const SPBase& other) {
            node_ = other.node_;
            if (node_) {
                node_->count_.fetch_add(1, std::memory_order_relaxed);
            }
        }

        // 释放资源
        void release() {
            if (node_ && node_->count_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                Deleter::DoDelete(node_->resource_);
                delete node_;
            }
            node_ = nullptr;
        }
    };


    // 通用版本的 Shared Pointer
    template <typename T,typename Deleter=PointerDeleter<T> >
    class SP : public SPBase<T, Deleter> {
    public:
        explicit SP(T* ptr = nullptr):SPBase<T, Deleter>(ptr){}

        explicit SP(UP<T> up) noexcept {
            this->node_ = up.ptr_ ? new typename SPBase<T, PointerDeleter<T>>::SPNode{up.ptr_, 1} : nullptr;
            up.ptr_= nullptr;
        }

        // 克隆方法
        SP clone() const {
            if (this->node_) {
                T* newResource = new T(*this->node_->resource_);
                return SP(newResource); // 返回新的 SP
            }
            return SP(); // 返回一个空的 SP
        }

        T& operator*() const {
            return *(this->get());
        }

        // 指针访问运算符
        T* operator->() const {
            return this->get();
        }
    };

    // 数组特化版本的 Shared Pointer
    template <typename T>
    class SP<T[],ArrayDeleter<T>> : public SPBase<T, ArrayDeleter<T>> {
    public:
        explicit SP(T* ptr = nullptr)
                : SPBase<T, ArrayDeleter<T>>(ptr) {}
        explicit SP(UP<T[]> up) noexcept {
            this->node_ = up.ptr_ ? new typename SPBase<T, ArrayDeleter<T>>::SPNode{up.ptr_, 1} : nullptr;
            up.ptr_ = nullptr;  // 释放 UP 中的指针资源
        }

        // 下标访问运算符
        T& operator[](size_t index) const {
            return this->get()[index];
        }
    };



}



