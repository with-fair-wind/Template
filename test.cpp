/*
 * @Author: kk 921232958@qq.com
 * @Date: 2024-08-30 02:00:10
 * @LastEditors: kk 921232958@qq.com
 * @LastEditTime: 2024-08-31 01:47:12
 * @FilePath: \Template\test.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include <iostream>
#include <utility> // for std::forward

#if 1
#if 0
void targetFunc(int *ptr)
{
    // 处理指针
    std::cout << &ptr << std::endl;
}
#else
void targetFunc(int *&ptr)
{
    // 处理指针
    std::cout << &ptr << std::endl;
}

void targetFunc(int *&&ptr)
{
    // 处理指针
    std::cout << &ptr << std::endl;
}
#endif
template <typename T>
void forwardingFunc(T &&arg)
{
    targetFunc(std::forward<T>(arg)); // 完美转发
}

int main()
{
    int x = 42;
    int *p = &x;
    std::cout << &p << std::endl;
    forwardingFunc(p);            // 转发左值指针
    forwardingFunc(new int(100)); // 转发右值指针

    return 0;
}
#else
void overloaded(int &x)
{
    std::cout << "Lvalue reference overload, &x = " << &x << "\n";
}

void overloaded(int &&x)
{
    std::cout << "Rvalue reference overload, &x = " << &x << "\n";
}

template <typename T>
void forwarding(T &&arg)
{
    overloaded(std::forward<T>(arg));
}

int main()
{
    int a = 10;
    std::cout << &a << std::endl;
    forwarding(a);            // 调用 Lvalue reference overload
    forwarding(10);           // 调用 Rvalue reference overload
    forwarding(std::move(a)); // 调用 Rvalue reference overload
}
#endif
