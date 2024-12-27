// 变量模板

#include <array>
#include <functional>
#include <iostream>
#include <numeric>
#include <tuple>
#include <type_traits>

#if 0
template <typename T>
constexpr T v{};

int main()
{
    v<int>; // 相当于 constexpr int v{} const
    std::cout << std::is_same_v<decltype(v<int>), const int> << std::endl;
    v<double>; // 两个全局对象，地址不同
    std::cout << &v<double> << std::endl;
    std::cout << &v<int> << std::endl;
}
#endif

#if 0
template <std::size_t N = 66>
constexpr std::size_t v = N;

int main()
{
    const int b = v<10>;
    std::cout << b << std::endl;
    std::cout << v<10> << std::endl;
    std::cout << v<> << std::endl;
    return 0;
}
#endif

#if 0
template <std::size_t... value>
constexpr std::size_t array[]{value...};

int main()
{
    for (const auto &i : array<1, 2, 3, 4, 5>)
        std::cout << i << ' ';

    std::cout << std::endl
              << std::is_same_v<decltype(array<1, 2, 3, 4, 5>), const std::size_t[5]> << std::endl;
}
#endif

#if 1
#define T6
#ifdef T1
struct X
{
    static int n; // 声明
};
int X::n = 0; // 定义
#endif

#ifdef T2
struct X
{
    const static int n = 10; // 不是定义，还是声明
};
const int X::n; // 添加类外定义
// 读取编译时常量，不是“ODR”使用

int main()
{
    int b = X::n;                    // 是声明，但是可以通过编译，但是下面的就不行了。涉及逆天规则
    std::cout << &X::n << std::endl; // error 通过添加第67行定义就可以了
    return 0;
}
#endif

#ifdef T3
struct X
{
    static inline int n = 10; // 添加内联修饰，此时已经是定义，无需类外定义
};

// 读取编译时常量，不是“ODR”使用

int main()
{
    int b = X::n;
    std::cout << &X::n << std::endl;
    return 0;
}
#endif

#ifdef T4
struct X
{
    static constexpr int n = 10; // 已经是定义，无需类外定义 constexpr在C++17修饰静态数据成员的时候，蕴含了inline
};

// 读取编译时常量，不是“ODR”使用

int main()
{
    int b = X::n;
    std::cout << &X::n << std::endl;
    return 0;
}
#endif

#ifdef T5
struct limits
{
    template <typename T>
    static const T min;
};

template <typename T>
const T limits::min{};

int main()
{
    int a = limits::min<int>;
    auto p = &limits::min<int>;
    return 0;
}
#endif

#ifdef T6
struct limits
{
    template <typename T>
    inline static const T min{}; // 或者 constexpr static T min{};
    //
};

int main()
{
    int a = limits::min<int>;
    auto p = &limits::min<int>;
    return 0;
}
#endif
#endif