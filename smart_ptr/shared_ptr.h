#pragma once
#include <iostream>
// 根据shared_ptr的原理简单的实现一个shared_ptr类，
// 主要用于了解shared_ptr的引用计数是怎么工作的

template <class T>
class shared_ptr
{
private:
    long *count_; // 引用计数器
    T *ptr_;      // 原始指针

public:
    shared_ptr() : ptr_(nullptr), count_(nullptr)
    {
    }
    // 因为不能将一个内置指针隐式转化为一个智能指针，所以必须使用直接初始化形式
    // 故加上关键词explicit
    explicit shared_ptr(T *ptr) : ptr_(ptr), count_(nullptr)
    {
        increaseCount();
    }

    shared_ptr(const shared_ptr &sp) : ptr_(sp.ptr_), count_(sp.count_)
    {
        increaseCount();
    }

    ~shared_ptr()
    {
        release();
    }

    void swap(shared_ptr &lhs)
    {
        std::swap(ptr_, lhs.ptr_);
        std::swap(count_, lhs.count_);
    }

    // Ref: https://lokiastari.com/blog/2015/01/15/c-plus-plus-by-example-smart-pointer-part-ii/
    // 赋值操作应该先自身的引用减1, rhs的增1
    // 这里通过简化可以直接通过swap代替
    shared_ptr &operator=(shared_ptr rhs) // 注意这里是值传递，属于implicit copy
    {
        // 通过implicit copy， rhs的计数器已经增1
        // 在通过swap获取指针所指内容即可
        swap(rhs);
        return *this;
    }

    long use_count() const { return count_ == nullptr ? 0 : (*count_); }

private:
    void release()
    {
        decreaseCount();
        count_ = nullptr;
        ptr_ = nullptr;
    }

private:
    // 引用计数变化的一些函数
    void increaseCount()
    {
        if (ptr_ != nullptr)
        {
            if (count_ == nullptr)
            {
                count_ = new long(1);
            }
            else
            {
                ++(*count_);
            }
        }
    }

    void decreaseCount()
    {
        if (count_ != nullptr)
        {
            --(*count_);
            std::cout << "decrease count and count = " << *count_ << std::endl;
            if ((*count_) == 0)
            {
                delete count_;
                delete ptr_;
            }
        }
    }
};
