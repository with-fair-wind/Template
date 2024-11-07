// 约束与概念
#include <array>
#include <concepts>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

#if 0
#define T2
#ifdef T1
// 我需要写一个函数模板 add，想要要求传入的对象必须是支持 operator+ 的，应该怎么写？
template <typename T>
concept Add = requires(T a) { a + a; }; //// "需要表达式 a+a 是可以通过编译的有效表达式"

template <Add T>
auto add(const T &t1, const T &t2)
{
    return t1 + t2;
}

int main()
{
    struct X
    {
    };
    // add(X{}, X{});
    add(1, 2);
    constexpr bool v = Add<int>;
    //> 每个概念都是一个**谓词**，它在**编译时求值**，并在将之用作约束时成为模板接口的一部分。
    std::cout << std::boolalpha << v << std::endl;
    std::cout << std::boolalpha << Add<int> << std::endl;
}
#endif

#ifdef T2
#if 0
decltype(auto) max(const std::integral auto &a, const std::integral auto &b)
{
    return a > b ? a : b;
}
#else
// 定义概念（concept） 时声明的约束表达式，只需要是编译期可得到 bool 类型的表达式即可。
template <typename T>
concept Int = std::is_same_v<T, int>;

decltype(auto) max(const Int auto &a, const Int auto &b)
{
    return a > b ? a : b;
}
#endif
int main()
{
    ::max(1, 2);
    // ::max('1', '2');
    // ::max(1., 2);
    Int auto a = 1;
}
#endif
#endif

#if 0
// requries子句 : requries后面要求是(期待的是)编译期求值,返回bool类型的表达式
template <typename T>
concept add = requires(T a) { a + a; };

template <typename T>
// requires add<T>
// requires std::is_same_v<T, int>
// requires(sizeof(T) >= 4)
    requires requires(T t) { t + t; }
// 第一个requires是requires子句，而第二个requires(T t) { t + t; }是requires表达式，恰恰就是编译期求值
void f(T)
{
}

int main()
{
    f(1);
    // f('a');
}
#endif

#if 0
#define T2
#ifdef T1
template <typename T>
concept Integral = std::is_integral_v<T>;

// 合取
template <typename T>
concept SignedIntegral = Integral<T> && std::is_signed_v<T>;

void s_f(const SignedIntegral auto &) {}

int main()
{
    s_f(1);
    // s_f(1u);
}
#endif

#ifdef T2
struct X
{
    // static constexpr bool value = true;
    int c{}; // 扩大大小
};

// 要求 T 是大小 sizeof 大于 1 且支持一个函数调用

template <typename T>
constexpr bool get_value() { return T::value; }

template <typename T>
// requires(sizeof(T) > 1 && get_value<T>())
    requires(sizeof(T) > 1 || get_value<T>())
void f(T)
{
}

int main()
{
    X x;
    f(x);
    return 0;
}
#endif
#endif

#if 0
// requires表达式 :  产生描述约束的 bool 类型的纯右值表达式。
// 注意，requires 表达式 和 requires 子句，没关系。
// requires { 要求序列 }
// requires(形参列表 (可选)) {要求序列}

// requires表达式里面写的东西，满足语法，能通过编译，就是true否则为false
template <typename T>
void f(T)
{
    constexpr bool v1 = requires { T::type; };          // 只要T::type是合法表达式，则返回true，否则为false
    constexpr bool v2 = requires { typename T::type; }; // 只要T::type是合法表达式，则返回true，否则为false
    std::cout << std::boolalpha << "v1: " << v1 << ", v2: " << v2 << std::endl;
}

struct X
{
    using type = int;
};

struct Y
{
    static constexpr int type = 0;
};

int main()
{
    f(1);
    f(X{});
    f(Y{});
    return 0;
}
#endif

#if 0
// 简单要求
template <typename T>
concept Addable = requires(T a, T b) { a + b; };

template <typename T, class U>
concept Swappable = requires(T &&t, U &&u) {
    swap(std::forward<T>(t), std::forward<U>(u));
    swap(std::forward<U>(u), std::forward<T>(t));
};

template <typename T>
    requires(Addable<T> && Swappable<T, T>)
struct Test
{
};

namespace loser
{
    struct X
    {
        X operator+(const X &) const
        {
            return *this;
        }
    };

    void swap(const X &, const X &) {}
};

int main()
{
    // 实参依赖查找， X和swap函数在一个命名空间，所以swap可以不加限定
    Test<loser::X> test;
    std::cout << std::boolalpha << Addable<loser::X> << '\n';             // true
    std::cout << std::boolalpha << Swappable<loser::X, loser::X> << '\n'; // true

    return 0;
}
#endif

#if 0
template <typename T>
struct S
{
};

template <typename T>
using Ref = T &;

template <typename T>
concept C = requires {
    typename T::X2;   // 要求嵌套类型
    typename T::type; // 要求嵌套类型
    typename S<T>;    // 要求类模板特化
    typename Ref<T>;  // 要求别名模板代换
};

struct X
{
    using X2 = int;
    using type = int;
};

struct Y
{
    struct X2
    {
    };
};

int main()
{
    std::cout << std::boolalpha << C<X> << std::endl;
    std::cout << std::boolalpha << C<Y> << std::endl;
}
#endif

#if 0
// 复合要求
template <typename T>
concept C = requires(T x) {
    { *x } -> std::convertible_to<typename T::inner>;
    { x + 1 } -> std::same_as<int>;
    { x * 1 } -> std::convertible_to<T>;
    { x.~T() } noexcept; // 要求x.~T()是不会抛出异常的合法表达式  //析构函数默认是noexcept(比较特殊)
    { x.f() } noexcept;
};
// 表达式{*x}必须合法
// 类型T::inner 必须存在
// 要求{*x}能够隐式转换到 T::inner 类型
// x + 1 必须合法 且必须是int类型 == std::same_as<decltype(x + 1), int>;

struct X
{
    void f() noexcept {}
    // ~X() noexcept(false) {}
    int operator*() const { return 0; }
    int operator+(int) const { return 0; }
    X operator*(int) const { return *this; }
    using inner = int;
};

int main()
{
    std::cout << std::boolalpha << C<X> << std::endl;
    std::cout << std::boolalpha << std::same_as<int, int> << std::endl;
    std::cout << std::boolalpha << std::same_as<int, void> << std::endl;
}
#endif

#if 0
template <typename T>
concept C = requires(T a) {
    // requires子句要求编译期产生bool值的表达式
    std::is_same_v<T, size_t>;          // 满足语法即可
    requires std::is_same_v<T, double>; // 要求T是int
    requires std::same_as<T, double>;   // 要求T是int
    requires requires { a + a; };       // 要求a+a 是合法表达式
    a + a;
    requires sizeof(a) > 4;
};

int main()
{
    std::cout << std::boolalpha << C<double> << std::endl;
}
#endif

#if 1
template <typename T>
concept have_type = requires {
    typename T::type;
};

template <typename T>
struct X
{
    static void f() { std::puts("主模板"); }
};

template <have_type T>
struct X<T>
{
    using type = typename T::type;
    static void f() { std::puts("偏特化 T::type"); }
};

struct Test
{
    using type = int;
};

struct Test2
{
    // using type = double;
    void f() {}
};

int main()
{
    X<Test>::f();
    X<int>::f();
    X<Test2>::f();
    return 0;
}
#endif