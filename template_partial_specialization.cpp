// 模板偏特化
// 函数模板无法偏特化、类模板和变量模板才能偏特化

#include <array>
#include <functional>
#include <iostream>
#include <numeric>
#include <tuple>
#include <type_traits>

#if 0
template <typename T>
const char *s = "?"; // 主模板

template <typename T>
const char *s<T *> = "pointer"; // 偏特化，对指针，这一"类"类型

template <typename T>
const char *s<T[]> = "array"; // 偏特化，但是只是对T[]这一类型，而不是数组类型

int main()
{
    std::cout << s<int *> << std::endl;
    std::cout << s<void *> << std::endl;
    std::cout << s<double *> << std::endl;
    std::cout << s<int> << std::endl;
    // int[]和int[1]不是一个类型
    std::cout << s<int[]> << std::endl;
    std::cout << s<double[]> << std::endl;
    std::cout << s<double[1]> << std::endl;
}
#endif

#if 0
template <typename T1, typename T2>
const char *s = "?";

template <typename T>
const char *s<int, T> = "T1 == int";

int main()
{
    std::cout << s<char, double> << std::endl;
    std::cout << s<int, int *> << std::endl;
    std::cout << s<int, double> << std::endl;
    return 0;
}
#endif

#if 0
template <typename T1, typename T2>
struct X
{
    void f() const {}
};

template <typename T>
struct X<void, T>
{
    void f2() const;
    int a{};
};
template <typename T>
void X<void, T>::f2() const{ std::puts("f2()"); }

int main()
{
    X<void, int> x;
    x.f2();
}
#endif

#if 0
// C++ 标准不允许对嵌套类模板进行部分特化
template <typename T, std::size_t N>
struct X
{
    template <typename T1, typename T2>
    struct Y
    {
        void f() const {}
    };
};

template <>
template <typename T2>
struct X<int, 1>::Y<int, T2>
{
    void f() const { std::puts("f()"); }
};

int main()
{
    X<int, 1>::Y<int, void> y; // GCC无法识别偏特化版本，无法通过编译，除非主模板中有Y类有f成员函数，只能匹配到主模板
    y.f();
    return 0;
}
#endif

#if 1
template <class, class>
struct is_same
{
    static constexpr bool value = false;
};

template <class Ty>
struct is_same<Ty, Ty>
{
    static constexpr bool value = true;
};

#if 0
template <class T1, class T2>
constexpr bool is_same_v = is_same<T1, T2>::value;
#else
template <class T1, class T2>
constexpr bool is_same_v = false;

template <class Ty>
constexpr bool is_same_v<Ty, Ty> = true;
#endif

int main()
{
    std::cout << std::boolalpha << ::is_same<int, int>::value << std::endl;
    std::cout << std::boolalpha << ::is_same<int, char>::value << std::endl;
    std::cout << std::boolalpha << ::is_same_v<int, int> << std::endl;
    std::cout << std::boolalpha << ::is_same_v<int, char> << std::endl;
    return 0;
}

#endif
