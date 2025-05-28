// SFINAE
#include <array>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

// 待决名消除歧义符
/*
在模板（包括别名模版）的声明或定义中，不是当前实例化的成员且取决于某个模板形参的名字不会被认为是类型
除非使用关键词 typename 或它已经被设立为类型名（例如用 typedef 声明或通过用作基类名）
*/

#if 0
#if 0
template <typename T>
const typename T::type &f(const T &) { return typename T::type{}; }
// 不会被认为是类型

struct X
{
    using type = int;
};

int main()
{
    X x;
    f(x);
    return 0;
}
#else
int p = 1;

template <typename T>
void foo(const std::vector<T> &v)
{
    // std::vector<T>::const_iterator 是待决名，
    typename std::vector<T>::const_iterator it = v.begin();

    // 下列内容因为没有 'typename' 而会被解析成
    // 类型待决的成员变量 'const_iterator' 和某变量 'p' 的乘法。
    // 因为在此处有一个可见的全局 'p'，所以此模板定义能编译。
    typename std::vector<T>::const_iterator *p; // msvc可以通过编译，但是gcc不行(不加typename)

    typedef typename std::vector<T>::const_iterator iter_t;
    iter_t *p2; // iter_t 是待决名，但已知它是类型名
}

int main()
{
    std::vector<int> v;
    foo(v); // 实例化失败
}
#endif
#endif

// template 消除歧义符
// 与此相似，模板定义中不是当前实例化的成员的待决名同样不被认为是模板名，除非使用消歧义关键词 template，或它已被设立为模板名
#if 0
template <typename T>
struct S
{
    template <typename U>
    void foo() {}
};

template <typename T>
void bar()
{
    S<T> s;
    s.template foo<T>();
};

// 关键词 template 只能以这种方式用于运算符 ::（作用域解析）、->（通过指针的成员访问）和 .（成员访问）之后，下列表达式都是合法示例：

// T::template foo<X>();
// s.template foo<X>();
// this->template foo<X>();
// typename T::template iterator<int>::value_type v;

int main()
{
    bar<int>();
    return 0;
}
#endif

#if 0
#define T3
#ifdef T1
void g(double) { std::cout << "g(double)\n"; }
void g(int) { std::cout << "g(int)\n"; }

int main()
{
    g(1);
    return 0;
}
#endif

#ifdef T2
void g(double) { std::cout << "g(double)\n"; }

// 非待决名在模板定义点查找并绑定。即使在模板实例化点有更好的匹配，也保持此绑定
template <typename T>
struct S
{
    void f() const
    {
        g(1); // 非待决名
    }
};
void g(int) { std::cout << "g(int)\n"; }

int main()
{
    g(1);
    S<void> s;
    s.f();
    return 0;
}
#endif

#ifdef T3
void f() { std::puts("全局"); }
template <typename T>
struct X
{
    void f() const
    {
        std::puts("X");
    }
};

// 非待决名：检查该模板的定义时将进行无限定的名字查找
// 待决名：它的查找会推迟到得知它的模板实参之时

template <typename T>
struct Y : X<T>
{
    void t1() const
    {
        this->f(); // X // ！！！无限定名字查找 //this依赖于模板X，this->f() 是一个待决名
    }
    void t2() const
    {
        f(); // 全局
    }
};

// 是否this访问成员是有区别的

int main()
{
    Y<void> y;
    y.t1();
    y.t2();
    return 0;
}

#endif
#endif

#if 0
#define T2
#ifdef T1
// 代换失败不是错误”(Substitution Failure Is Not An Error)

// 在函数模板的重载决议[1] 中会应用此规则：当模板形参在替换成显式指定的类型或推导出的类型失败时，从重载集中丢弃这个特化，而非导致编译失败。(会去找其他的匹配，但是当其他也没有时，编译还是会报错)

template <typename T, typename T2 = typename T::type>
void f(int) { std::puts("int"); }

template <typename T, typename T2 = decltype(T{} + T{})>
void f(double) { std::puts("double"); }

struct X
{
    using type = int;
};

int main()
{
    // T:int int::type ? 不符合基本语法 代换失败 丢弃特化
    f<int>(1);
    // T:X X::type = int
    f<X>(1);
    f<X>(1.2);
}
#endif

#ifdef T2
// 代换失败(SFINAE 错误) 、硬错误
template <typename T>
struct B
{
    using type = typename T::type;
};

template <typename T>
void foo(double) { std::puts("SFINAE"); }

#if 1
template <
    class T,
    class V = typename B<T>::type> // 硬错误
// 应当关注 B<T> 而非 B<T>::type，因为是直接在实例化模板 B 的时候就失败了，被当成硬错误；如果 B<T> 实例化成功，而没有 ::type，则被当成代换失败（不过这里是不可能）
void foo(int)
{
    std::puts("SFINAE T::type B<T>::type");
}

int main()
{
    foo<void>(1);

    // foo<C>(1);
    // foo<B<C>>(1);
    // foo<C>(1.);
    // foo<B<C>>(1.);
}
#else
template <
    class T,
    class U = typename T::type, // 如果T没有别名type 那么就是SFINAE失败(代换失败)
    class V = typename B<T>::type>
void foo(int)
{
    std::puts("SFINAE T::type B<T>::type");
}

int main()
{
    struct C
    {
        using type = int;
    };

    foo<void>(1);
    foo<C>(1);
    foo<B<C>>(1);
    foo<C>(1.);
    foo<B<C>>(1.);
}
#endif
#endif
#endif

#if 0
// 我需要写一个函数模板 add，想要要求传入的对象必须是支持 operator+ 的，应该怎么写？
template <typename T>
auto add(const T &t1, const T &t2) -> decltype(t1 + t2)
{
    std::puts("SFINAE+");
    return t1 + t2;
}

struct X
{
};

// 这样有啥好处吗？使用了 SFINAE 看起来还变复杂了。我就算不用这写法，如果对象没有 operator+ 不是一样会编译错误吗？
// 虽然前面说了 SFINAE 可以影响重载决议，我知道这个很有用，但是我这个函数根本没有别的重载，这样写还是有必要的吗？

// 如果就是简单写一个 add 函数模板不使用 SFINAE，那么编译器在编译的时候，会尝试模板实例化，生成函数定义，发现你这类型根本没有 operator+，于是实例化模板错误。
// 如果按照我们上面的写法使用 SFINAE，根据“代换失败不是错误”的规则，从重载集中丢弃这个特化 add，然而又没有其他的 add 重载，所以这里的错误是“未找到匹配的重载函数”。

// 这里的重点是什么？是模板实例化，能不要实例化就不要实例化！！！
// 如果我们使用 SFINAE，编译器就是直接告诉我：“未找到匹配的重载函数”，我们自然知道就是传入的参数没有满足要求。而且实例化模板也是有开销的，很多时候甚至很大。

int main()
{
    add(1, 2);
    add(X{}, X{});
}
#endif

#if 1
#define T1
#ifdef T1
// std::enable_if
#if 0
// 如何要求T必须是int类型呢? SFINAE
template <typename T, typename SFINAE = std::enable_if_t<std::is_same_v<T, int>>>
void f(T) {}

int main()
{
    f(1);
    // f(1.);
}
#else
template <typename T, std::enable_if_t<std::is_same_v<T, int>, int *> = nullptr>
void f(T) {}
int main()
{
    f(1);
}
#endif
#endif

#ifdef T2
template <typename Ty, std::size_t N>
struct array
{
    Ty arr[N];
};

template <class Type, class... Args>
array(Type, Args...) -> array<std::enable_if_t<(std::is_same_v<Type, Args> && ...), Type>, sizeof...(Args) + 1>;

int main()
{
    ::array arr{1., 2, 3, 4, 5};
    // ::array arr{1, 2, 3, 4, 5};

    return 0;
}
#endif

#ifdef T3
// 我要写一个函数模板 add，我要求传入的对象需要支持默认构造、 + 以及它需要有别名 type ，成员 value、f。
template <typename T, typename = std::void_t<decltype(T{} + T{}), typename T::type, decltype(&T::value), decltype(&T::f)>>
auto add(const T &t1, const T &t2)
{
    return t1 + t2;
}

struct Test
{
    int value;
    int operator+(const Test &t) const
    {
        return this->value + t.value;
    }
    using type = void;
    void f() const {}
};

int main()
{
    Test t{1};
    add(t, t);
}
#endif

#ifdef T4
// 我要写一个函数模板 add，我要求传入的对象需要支持 + 以及它需要有别名 type ，成员 value、f。
// std::declval
template <typename T, typename = std::void_t<decltype(std::declval<T>() + std::declval<T>()), typename T::type, decltype(&T::value), decltype(&T::f)>>
auto add(const T &t1, const T &t2)
{
    return t1 + t2;
}

struct Test
{
    int value;
    Test(int v) : value(v) {}
    int operator+(const Test &t) const
    {
        return this->value + t.value;
    }
    using type = void;
    void f() const {}
};

int main()
{
    Test t{1};
    add(t, t);
}
#endif
#endif

#if 0
template <typename T1, typename T2 = void>
struct X
{
    static void f() { std::puts("主模板"); }
};

template <typename T>
struct X<T, std::void_t<typename T::type, decltype(&T::f)>>
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
    using type = double;
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