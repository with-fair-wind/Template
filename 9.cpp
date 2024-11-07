/*
 * @Author: kk 921232958@qq.com
 * @Date: 2024-08-29 23:15:26
 * @LastEditors: kk 921232958@qq.com
 * @LastEditTime: 2024-08-31 13:59:38
 * @FilePath: \Template\9.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
// 其它
#include <array>
#include <concepts>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

#if 0
template <std::size_t... I>
void f(std::index_sequence<I...>)
{
    int _[]{(std::cout << I << ' ', 0)...};
}

template <typename... Args>
void print(const std::tuple<Args...> &tuple)
{
    [&]<std::size_t... I>(std::index_sequence<I...>)
    { ((std::cout << std::get<I>(tuple) << ' '), ...); }(std::make_index_sequence<sizeof...(Args)>());
}

int main()
{
    f(std::make_index_sequence<10>()); // 生成了一个序列
    std::tuple<int, double> m_tuple{1, 1.2};
    std::cout << '\n'
              << std::get<0>(m_tuple) << std::endl;
    print(m_tuple);
}
#endif

#if 0
// 使用模板包装C风格API进行调用
// 经典的 void * + 变参模板
// 简单来说，我们需要写一个类包装一个这样的函数 f ，支持任意可调用对象与任意类型和个数的参数，最终都执行函数 f。
void f(unsigned (*start_address)(void *), void *args)
{
    start_address(args);
    std::cout << "f\n";
}

struct X
{
    template <typename Fn, typename... Args>
    X(Fn &&func, Args &&...args)
    {
        using Tuple = std::tuple<std::decay_t<Fn>, std::decay_t<Args>...>;
        auto Decay_copied = std::make_unique<Tuple>(std::forward<Fn>(func), std::forward<Args>(args)...);
        auto Invoker_proc = start<Tuple>(std::make_index_sequence<1 + sizeof...(Args)>{});
        f(Invoker_proc, Decay_copied.release());
    }

    template <typename Tuple, std::size_t... Indices>
    static constexpr auto start(std::index_sequence<Indices...>) noexcept
    {
        return &Invoke<Tuple, Indices...>; // 返回函数指针
    }

    template <class Tuple, std::size_t... Indices>
    static unsigned int Invoke(void *RawVals) noexcept
    {
        const std::unique_ptr<Tuple> FnVals(static_cast<Tuple *>(RawVals));
        Tuple &Tup = *FnVals.get();
        std::invoke(std::move(std::get<Indices>(Tup))...); // 真正的调用
        return 0;
    }
};

#define T3
#ifdef T1
void func(int &a)
{
    std::cout << &a << '\n';
}

int main()
{
    int a{};
    std::cout << &a << '\n';
    func(a);
    X{func, std::ref(a)};
}
#endif

#ifdef T2
void func(int &a)
{
    std::cout << &a << '\n';
}

int main()
{
    int a{};
    std::cout << &a << '\n';
    func(a);
    X{func, a};
}
#endif

#ifdef T3
void func(const int &a)
{
    std::cout << &a << '\n';
}

int main()
{
    int a{};
    std::cout << &a << '\n';
    func(a);
    X{func, a};
}
#endif
#endif

#if 0
// 编译期if
struct Y
{
    void f() const
    {
        std::puts("f");
    }
};

struct X
{
    void func() const
    {
        std::puts("func");
    }
};

template <typename T>
void f(T t)
{
    if constexpr (std::is_same_v<T, X>)
        t.func();
    else
        t.f(); // 舍弃语句 但是依然会进行语法检查
}

int main()
{
    f(X{});
    f(Y{});
    return 0;
}
#else
template <typename T>
auto f() // C++14返回类型推导，如果有多个返回语句，必须被推导为相同的类型，但是编译期if可以解决
{
    if constexpr (std::is_same_v<T, int>)
        return 0;
    else
        return 1.0;
}

int main()
{
    f<int>();
}
#endif