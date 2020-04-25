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
    static const int local_buf_size = 15;

private:
    size_t m_size; // string 的长度
    size_t m_capacity;
    union //  储存string
    {
        char buffer[local_buf_size];
        char *ptr;
    } m_data;

    bool sso = true;

public:
    MyString() : m_size(0), m_capacity(local_buf_size)
    {
        cout << "MyString default ctor" << endl;
    };

    // 字符初始化
    // explicit 关键字能防止隐式转化
    MyString(char *str) : m_size(myStrLen(str)), m_capacity(local_buf_size)
    {
        // Todo: 考虑 len = 0 的情况
        cout << "MyString char ctor" << endl;
        if (m_size < m_capacity)
        {
            myStrncpy(m_data.buffer, str, m_size);
        }
        else
        {
            sso = false;
            m_capacity = 16;
            while (m_capacity < m_size)
            {
                m_capacity = (m_capacity << 1);
            }
            m_data.ptr = new char[m_capacity];
            myStrncpy(m_data.ptr, str, m_size);
        }
    }

    // 拷贝构造函数
    MyString(const MyString &other)
    {
        cout << "MyString copy ctor" << endl;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        sso = other.sso;
        if (sso == true)
        {
            for (int i = 0; i < m_size; ++i)
            {
                m_data.buffer[i] = other.m_data.buffer[i];
            }
        }
        else
        {
            m_data.ptr = new char[m_capacity];
            myStrncpy(m_data.ptr, other.m_data.ptr, m_size);
        }
    }

    ~MyString()
    {
        cout << "MyString dtor" << endl;
        if (sso == false)
        {
            delete[] m_data.ptr;
            m_data.ptr = nullptr;
        }
    }

    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }

    void append(char ch)
    {
        m_size++;
        if (sso == true)
        {
            if (m_size <= m_capacity)
            {
                m_data.buffer[m_size - 1] = ch;
                return;
            }
            m_capacity = 16;
        }
        else
        {
            if (m_size <= m_capacity)
            {
                m_data.ptr[m_size - 1] = ch;
                return;
            }
        }
        // 如果不是的话,则要开始从堆上分配
        while (m_capacity < m_size)
        {
            m_capacity = (m_capacity << 1);
        }

        char *temp = new char[m_capacity];
        if (sso == true)
        {
            myStrncpy(temp, m_data.buffer, local_buf_size);
        }
        else
        {
            myStrncpy(temp, m_data.ptr, m_size - 1);
        }
        sso = false;
        temp[m_size - 1] = ch;
        m_data.ptr = temp;
    }

    void append(char *begin)
    {
        append(begin, myStrLen(begin));
    }

    void append(char *begin, size_t len)
    {
        size_t final_size = m_size + len;
        if (sso == true)
        {
            if (final_size <= m_capacity)
            {
                for (size_t i = 0; i < len; ++i)
                {
                    m_data.buffer[m_size + i] = *(begin + i);
                }
                m_size = final_size;
                return;
            }
            m_capacity = 16;
        }
        else
        {
            if (final_size <= m_capacity)
            {
                myStrncpy(m_data.ptr + m_size, begin, len);
                m_size = final_size;
                return;
            }
        }
        // 如果不是的话,则要开始从堆上分配
        while (m_capacity < final_size)
        {
            m_capacity = (m_capacity << 1);
        }

        char *temp = new char[m_capacity];
        if (sso == true)
        {
            myStrncpy(temp, m_data.buffer, local_buf_size);
            myStrncpy(temp + local_buf_size, begin, len);
        }
        else
        {
            myStrncpy(temp, m_data.ptr, m_size);
            myStrncpy(temp + m_size, begin, len);
        }
        sso = false;
        m_data.ptr = temp;
        m_size = final_size;
    }

    // 运算符重载
    // 下标运算符重载
    char &operator[](size_t index)
    {
        // 可以加一个assert 确保index位于正常范围
        assert(index >= 0 && index < m_size);

        if (sso)
        {
            return m_data.buffer[index];
        }

        return m_data.ptr[index];
    }

    // 输入输出运算符重载 输入输出运算符必须是非成员函数
    // friend istream &operator>>(istream &os, const MyString &s);
    friend ostream &operator<<(ostream &os, const MyString &s);

    // 赋值运算符重载
    MyString &operator=(const MyString &other)
    {
        cout << "MyString::operator=" << endl;
        if (this != &other)
        {
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            if (other.sso == true)
            {
                for (int i = 0; i < m_size; ++i)
                {
                    m_data.buffer[i] = other.m_data.buffer[i];
                }
            }
            else
            {
                if (sso == false)
                {
                    delete[] m_data.ptr;
                }
                m_data.ptr = new char[m_capacity];
                myStrncpy(m_data.ptr, other.m_data.ptr, m_size);
            }
        }
        sso = other.sso;
        return *this;
    }

    // friend MyString operator+(const MyString &lhs, const MyString &rhs);
    // friend MyString operator+(const MyString &lhs, const char &rhs);

    // 算数和相等运算符重载
    // friend bool operator<(const MyString &lhs, const MyString &rhs);
};

// istream &operator>>(istream &is, MyString &s)
// {
//     //
//     s = MyString("");
//     char c;
//     while (is && isspace(is.peek()))
//     {
//         is.ignore();
//     }

//     while (is && !isspace(is.peek()))
//     {
//         is >> c;
//         s = s + c;
//     }
//     return is;
// }

ostream &operator<<(ostream &os, const MyString &s)
{
    if (s.sso == true)
    {
        for (size_t i = 0; i < s.size(); ++i)
        {
            os << s.m_data.buffer[i];
        }
    }
    else
    {
        for (size_t i = 0; i < s.size(); ++i)
        {
            os << s.m_data.ptr[i];
        }
    }
    return os;
}

// MyString operator+(const MyString &lhs, const MyString &rhs)
// {
//     size_t len = lhs.size() + rhs.size();
//     MyString ret(len);
//     myStrncpy(ret.buff_, lhs.buff_, lhs.size());
//     myStrncpy(ret.buff_ + lhs.size(), rhs.buff_, rhs.size());
//     return ret;
// }

// MyString operator+(const MyString &lhs, const char &rhs)
// {
//     size_t len = lhs.size() + 1;
//     MyString ret(len);
//     myStrncpy(ret.buff_, lhs.buff_, lhs.size());
//     ret.buff_[lhs.size()] = rhs;
//     return ret;
// }

// bool operator<(const MyString &lhs, const MyString &rhs)
// {
//     if (myStrcmp(lhs.buff_, rhs.buff_) < 0)
//     {
//         return true;
//     }
//     return false;
// }