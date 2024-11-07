#include <array>
#include <functional>
#include <iostream>
#include <numeric>
#include <tuple>
#include <type_traits>

using namespace std::string_literals;

#if 0
#define T5
#ifdef T1
template <typename T = int, std::size_t N = 10>
struct X
{
};

int main()
{
    X<int> x;
    return 0;
}
#endif

#ifdef T2
template <typename T>
struct X
{
    T t;
};

int main()
{
    X x{1}; // C++17 CTAD
    return 0;
}
#endif

#ifdef T3
template <typename T>
struct X
{
    X(const T &, const T &) {}
};

// 自动推导

int main()
{
    X x1{1, 2};
    X x2(1, 2);
    auto x3 = new X{1, 3};
    return 0;
}
#endif

#ifdef T4
template <typename Ty, std::size_t N>
struct array
{
    Ty arr[N];
};

// 自定义推导指引
template <typename T, typename... Args>
array(T t, Args...) -> array<T, sizeof...(Args) + 1>;

int main()
{
    std::array arr{1, 2};
    ::array arr2{1, 2};
    return 0;
}
#endif

#ifdef T5
// 让一个类模板，如果推导为int，就让它实际成为size_t
template <typename T>
struct Test
{
    Test(T v) : t{v} {}

private:
    T t;
};

Test(int) -> Test<std::size_t>;
template <typename T>
Test(T *) -> Test<T[]>;
// 模板名称(实际被推导类型a)->模板名称<想要让类型a被推导为的类型>

int main()
{
    Test t{1};
    char *p = nullptr;
    Test t(p); // t 是 Test<char[]>
    return 0;
}
#endif
#endif

#if 0
#define T1
#ifdef T1
template <typename T = int>
struct X
{
};

// 只允许在函数作用域以及全局作用域中可以省略<>  >=C++17

struct Test
{
    X<> x; // 类中声明不允许省略<>, 即使满足C++17,同理于静态数据成员
    static inline X<> x2;
};

int main()
{
    X x;    // C++17 ok
    X<> x2; // C++11 ok
    return 0;
}
#endif
#endif

#if 0
// 模板模板形参
#define T4
#ifdef T1
template <template <typename T> class Ty>
struct X
{
    Ty<int> t;
};

template <typename T>
struct Y
{
    void f() { std::puts("y"); }
};

template <typename T>
struct Y2
{
    void f() { std::puts("y2"); }
};

int main()
{
    X<Y> x;
    x.t.f();
    X<Y2> x1;
    x1.t.f();
    return 0;
}
#endif

#ifdef T2
template <typename T>
struct Y
{
};

template <template <typename T> class = Y, typename = int, std::size_t = 10>
struct X
{
};

int main()
{
    X x;
}
#endif

#ifdef T3
template <typename T>
struct Y1
{
};

template <template <typename T> class... Ty>
struct X
{
};

template <typename T>
struct Y2
{
};

int main()
{
    X<Y1, Y2, Y2, Y1> x;
}
#endif

#ifdef T4
template <std::size_t>
struct Y1
{
};

template <template <std::size_t> class T>
struct X1
{
};

template <typename... Args>
struct Y2
{
};

template <typename... Args>
struct Y3
{
};

template <template <typename... Args> class... Tys>
struct X2
{
};

int main()
{
    X1<Y1> x1;
    X2<Y2, Y3, Y3, Y2> x2;
    return 0;
}
#endif
#endif

#if 0
// 成员函数模板
template <typename T>
struct Class_template
{
    template <typename... Args>
    void f(Args &&...args) {}
};

struct X
{
    template <typename... Args>
    void f(Args &&...args) {}
};

int main()
{
    Class_template<int> t;
    t.f(1, 2, 3, 1.2);

    X x;
    x.f("1", 1, 2, 1.2);
    return 0;
}
#endif

#if 1
template <typename... Args>
struct X
{
    X(Args... args) : value{args...} {}
    std::tuple<Args...> value;
};

int main()
{
    std::tuple<int, double> t{1., 1.2};
    std::get<0>(t);
    std::get<int>(t);
    X x{1, "kk", 1.2, 's'};
    std::cout << std::get<1>(x.value) << std::endl;

    return 0;
}
#endif