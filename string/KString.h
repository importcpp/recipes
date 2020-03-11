#pragma once
#include <iostream>
#include <assert.h>
using namespace std;

// 一些辅助函数
size_t myStrLen(char *str)
{
    size_t len = 0;
    char *p = str;
    while ((*p) != '\0')
    {
        len++;
        p++;
    }
    return len;
}

// 字符拷贝函数
void myStrncpy(char *dest, char *src, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        *dest++ = *src++;
    }
}

int myStrcmp(char *lhs, char *rhs)
{
    while ((*lhs) == (*rhs))
    {
        if (*lhs == '\0')
        {
            return 0;
        }
        lhs++;
        rhs++;
    }
    return (*lhs) < (*rhs) ? -1 : 1;
}

class MyString
{
private:
    size_t len_; // string 的长度
    char *buff_; // 储存string

public:
    MyString() : len_(0), buff_(nullptr)
    {
        cout << "MyString default ctor" << endl;
    };

    // 字符初始化
    // explicit 关键字能防止隐式转化
    MyString(char *str) : len_(myStrLen(str)), buff_(new char[len_])
    {
        // Todo: 考虑 len = 0 的情况
        cout << "MyString char ctor" << endl;
        myStrncpy(buff_, str, len_);
    }

    MyString(size_t len, char ch = ' ') : len_(len), buff_(new char[len_])
    {
        for (size_t i = 0; i < len; ++i)
        {
            buff_[i] = ch;
        }
    }

    // 拷贝构造函数
    MyString(const MyString &other)
    {
        cout << "MyString copy ctor" << endl;
        this->len_ = other.size();
        buff_ = new char[len_];
        myStrncpy(buff_, other.buff_, len_);
    }

    ~MyString()
    {
        cout << "MyString dtor" << endl;
        delete[] buff_;
    }

    size_t size() const { return len_; }

    // 运算符重载
    // 下标运算符重载
    char &operator[](size_t index)
    {
        // 可以加一个assert 确保index位于正常范围
        assert(index >= 0 && index < len_);
        return buff_[index];
    }

    // 输入输出运算符重载 输入输出运算符必须是非成员函数
    friend istream &operator>>(istream &os, const MyString &s);
    friend ostream &operator<<(ostream &os, const MyString &s);

    // 赋值运算符重载
    MyString &operator=(const MyString &other)
    {
        cout << "MyString::operator=" << endl;
        if (this != &other)
        {
            delete[] buff_;
            len_ = other.size();
            buff_ = new char[len_];
            myStrncpy(buff_, other.buff_, len_);
        }
        return *this;
    }

    friend MyString operator+(const MyString &lhs, const MyString &rhs);
    friend MyString operator+(const MyString &lhs, const char &rhs);

    // 算数和相等运算符重载
    friend bool operator<(const MyString &lhs, const MyString &rhs);
};

istream &operator>>(istream &is, MyString &s)
{
    //
    s = MyString("");
    char c;
    while (is && isspace(is.peek()))
    {
        is.ignore();
    }

    while (is && !isspace(is.peek()))
    {
        is >> c;
        s = s + c;
    }
    return is;
}

ostream &operator<<(ostream &os, const MyString &s)
{
    for (size_t i = 0; i < s.size(); ++i)
    {
        os << s.buff_[i];
    }
    return os;
}

MyString operator+(const MyString &lhs, const MyString &rhs)
{
    size_t len = lhs.size() + rhs.size();
    MyString ret(len);
    myStrncpy(ret.buff_, lhs.buff_, lhs.size());
    myStrncpy(ret.buff_ + lhs.size(), rhs.buff_, rhs.size());
    return ret;
}

MyString operator+(const MyString &lhs, const char &rhs)
{
    size_t len = lhs.size() + 1;
    MyString ret(len);
    myStrncpy(ret.buff_, lhs.buff_, lhs.size());
    ret.buff_[lhs.size()] = rhs;
    return ret;
}

bool operator<(const MyString &lhs, const MyString &rhs)
{
    if (myStrcmp(lhs.buff_, rhs.buff_) < 0)
    {
        return true;
    }
    return false;
}