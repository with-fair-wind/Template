#include <array>
#include <functional>
#include <iostream>
#include <numeric>
#include <tuple>
#include <type_traits>

// 函数模板全特化
#if 0
template <typename T1, typename T2>
auto f(const T1 &a, const T2 &b) // 主函数模板
{
    return a + b;
}

//"定制"需求：传一个double和int类型，则返回相减的结果
template <>
auto f<double, int>(const double &a, const int &b) // 全特化
{
    return a - b;
}

struct X
{
    void f() const {}
};

struct X2
{
    void f2() const {}
};

template <typename T>
void test(const T &t) { t.f(); }
template <>
void test<X2>(const X2 &t) { t.f2(); }

int main()
{
    std::cout << f(2, 1) << std::endl;
    std::cout << f(2., 1) << std::endl;

    X x;
    test(x);
    X x2;
    test(x2);
}
#endif

// 类模板全特化
#if 0
#define T2
#ifdef T1
template <typename T>
struct is_void
{
    static constexpr bool value = false;
};

template <>
struct is_void<void>
{
    static constexpr bool value = true;
};

template <typename T>
constexpr bool is_void_v = is_void<T>::value;

int main()
{
    std::cout << std::boolalpha << std::is_void<void>::value << std::endl;
    std::cout << std::boolalpha << std::is_void<int>::value << std::endl;
    std::cout << std::boolalpha << ::is_void<void>::value << std::endl;
    std::cout << std::boolalpha << ::is_void<int>::value << std::endl;
    std::cout << std::boolalpha << ::is_void_v<void> << std::endl;
    std::cout << std::boolalpha << ::is_void_v<int> << std::endl;
    return 0;
}
#endif

#ifdef T2
template <typename T>
struct X
{
    void f() const { std::puts("f"); }
};

template <>
struct X<int>
{
    int n;
    void f() const { std::puts("X<int>"); }
    void f2() const {}
    static constexpr int n2 = 10;
};

int main()
{
    X<void> x;
    x.f();
    // x.f2(); // error
    X<int> x_i;
    x_i.f2();
    x_i.f();
    return 0;
}
#endif
#endif

// 变量模板全特化
#if 0
#define T2
#ifdef T1
template <typename T>
constexpr const char *s = "??";

template <>
constexpr const char *s<void> = "void";

int main()
{
    std::cout << s<void> << std::endl;
    return 0;
}
#endif

#ifdef T2
template <typename T>
constexpr bool is_void_v = false;

template <>
constexpr bool is_void_v<void> = true;

int main()
{
    std::cout << std::boolalpha << ::is_void_v<void> << std::endl;
    std::cout << std::boolalpha << ::is_void_v<int> << std::endl;
    return 0;
}
#endif
#endif

//  细节补充
#if 0
#define Q3
#ifdef Q1
// 特化必须在导致隐式实例化的首次使用之前，在每个发生这种使用的翻译单元中声明

template <typename T>
void f(const T &) {}

void f2()
{
    f(1); // “使用”了函数模板f隐式实例化了f<int>
}

// 顺序错了，得放到f2前面
template <>
void f<int>(const int &) {}

int main()
{
    return 0;
}
#endif

#ifdef Q2
// 只有声明没有定义的模板特化可以像其它不完整类型一样使用(eg：可以使用到它的指针或引用)
template <typename T> // 主模板声明
class X;
template <> // 特化声明
class X<int>;

X<int> *p;
// X<int> x //error
#endif

#ifdef Q3
/* 函数模板和变量模板的显式特化是否为 inline / constexpr / constinit / consteval
只与显式特化自身有关，主模板的声明是否带有对应说明符对它没有影响。
模板声明中出现的属性在它的显式特化中也没有效果：
*/
template <typename T>
int f(T) { return 6; }

template <>
constexpr int f<int>(int) { return 6; }

template <typename T>
constexpr int g(T) { return 6; }

template <>
int g<int>(int) { return 6; }

int main()
{
    constexpr auto n = f<int>(0);
    // constexpr auto n = f<double>(0); // error

    // constexpr auto n2 = g<int>(0); // error
    constexpr auto n2 = g<double>(0); // error
    return 0;
}
#endif
#endif

#if 1
template <typename T>
struct A
{
    struct B
    {
    };
    template <typename U>
    struct C
    {
    };
};

template <>
struct A<void>
{
    void f(); // 类内声明
    struct B
    {
    };
    template <typename U>
    struct C
    {
    };
};
// template<> // 加了错误
void A<void>::f() { std::puts("A<void>::f()"); } // 类外定义

// 特化成员类 A<char> 的情况下 B 类定义
template <>
struct A<char>::B
{
    void f();
};
void A<char>::B::f() { std::puts("A<char>::B::f()"); } // 类外定义

//  特化成员类 A<int> 的情况下 C 类定义
template <>
template <typename U>
struct A<int>::C
{
    void f();
};
template <>
template <typename U>
void A<int>::C<U>::f()
{
    std::puts("A<char>::C<U>::f()");
}

/* 不能这样特化
template <typename T>
template <>
struct A<T>::C<double>
{
};
*/

// 类里面有成员函数模板，怎么特化
struct X
{
    template <typename T>
    void f(T) { std::puts("f<T>"); }

    // template <>
    // void f<int>(int) { std::puts("f<int>"); } // 类内特化 // gcc编译失败? msvc可以通过
};
template <>
void X::f<double>(double) { std::puts("f<double>"); } // 类内特化

// 特化类模板的成员函数模板
template <typename U>
struct X1
{
    template <typename T>
    void f(T) { std::puts("f<T>"); }

    // template <>
    // void f<int>(int) { std::puts("f<int>"); } // 类内特化 // gcc编译失败? msvc可以通过
};
template <>
template <>
void X1<void>::f<double>(double) { std::puts("f<double>"); } // 类内特化

int main()
{
    A<void> a;
    a.f();
    A<char>::B b_c;
    b_c.f();
    A<int>::C<void> c_v;
    c_v.f();
    A<int>::C<int> c_i;
    c_i.f();

    X x;
    x.f(1);
    x.f(1.2);
    x.f("kk");

    X1<void> x1;
    x1.f(1);
    x1.f(1.2);

    return 0;
}

#endif
