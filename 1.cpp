// 函数模板不是函数，函数模板必须实例化，才会生成实际的函数定义 -> 粗略理解，模板只有“用了”它，才会生成实际代码
// C++ 模板是“静态” 编译期 没有运行时的开销

#include <functional>
#include <iostream>
#include <numeric>
#include <type_traits>

using namespace std::string_literals;

#if 0
template <typename T>
T max(T a, T b) { return a > b ? a : b; }

struct X
{
};

int main()
{
    std::cout << max(1, 2) << std::endl; // 自动推导 隐式实例化
    // std::cout << max(X{}, X{}) << std::endl; //error 实例化失败
    return 0;
}
#endif

#if 0
template <typename T>
T max(const T &a, const T &b) { return a > b ? a : b; } // 只是函数模板的形参类型的一部分

int main()
{
    max(1, 2);
    max<double>(1, 1.2);
    max(static_cast<double>(1), 1.2);

    // max<std::string>("kk"s, "1"); // ADL 实参依赖查找 实参为std::string对象，而std::stirng定义在std 而在std中存在max函数
    ::max<std::string>("kk"s, "1");
    ::max("kk"s, std::string("1"));
}
#endif

#if 0
template <typename T>
void f(T &&t) {}

template <class Ty>
constexpr Ty &&forward(Ty &Arg) noexcept { return static_cast<Ty &&>(Arg); }

int main()
{
    int a = 10;
    f(a);  // f<int &> 参数类型为int &
    f(10); // f<int &&>
    ::forward<int>(a);  // 无引用折叠，Ty是int
    ::forward<int &>(a);
    ::forward<int &&>(a);
}
#endif

#if 0
#define T7
#ifdef T1
template <typename T = int>
void f() {}

void test()
{
    f();         // 默认为f<int>
    f<double>(); // 显示指明
}
#endif

#ifdef T2
template <typename T1, typename T2, typename RT>
RT max(const T1 &a, const T2 &b) { return a > b ? a : b; }

void test()
{
    // max(1, 1.2); // 无法推导RT -> 解决方式见如下T3、T4、T5
}
#endif

#ifdef T3
template <typename T1, typename T2, typename RT = double>
RT max(const T1 &a, const T2 &b) { return a > b ? a : b; }

void test()
{
    max(1, 1.2);
}
#endif

#ifdef T4
template <typename RT, typename T1, typename T2>
RT max(const T1 &a, const T2 &b) { return a > b ? a : b; }

void test()
{
    max<std::string>("1", "1.2"s);
}
#endif

#ifdef T5
template <typename T1, typename T2, typename RT = decltype(true ? T1{} : T2{})> // 不求值语境
RT max(const T1 &a, const T2 &b)
{
    return a > b ? a : b;
}

void test()
{
    // max("1", "1.2"s);    //msvc好像可以编译通过，而gcc编译失败，三目运算符在gcc中，对于const char[2]、std::string,表达式的类型无法推导出合适的公共类型。
    max(1, 1.2);                             // 此时gcc可以通过编译 RT 为
    using type1 = decltype(true ? 1 : 1.2);  // double
    using type2 = decltype(false ? 1 : 1.2); // double 公共类型
    std::cout << std::is_same_v<type1, type2> << '\n';
}
#endif

#ifdef T6
template <typename T1, typename T2>
auto max(const T1 &a, const T2 &b) -> decltype(true ? a : b) // 后置返回类型， auto不是推导而是后置
// 与上一种语法的区别在于，返回类型可能存在不一样
{
    return a > b ? a : b;
}

void test()
{
    max("1", "1.2"s);
    max(1, 1.2);
}
#endif

#ifdef T7
// C++14 引入了返回类型推导
auto max(const auto &a, const auto &b) // auto 默认没有CV和引用
{
    return a > b ? a : b;
}

decltype(auto) max_(const auto &a, const auto &b) // 返回引用类型
{
    return a > b ? a : b;
}

void test()
{
    max("1", "1.2"s);
    max(1, 1.2);
    max_("1", "1.2"s);
    max_(1, 1.2);
}
#endif

int main()
{
    test();
    return 0;
}
#endif

#if 0
template <std::size_t N = 66>
void f() { std::cout << N << std::endl; }

int main()
{
    f<100>();
    f();
    return 0;
}
#endif

#if 0
// “重载决议” : 选择最"匹配"最"合适"的函数
template <typename T>
void test(T) { std::puts("template"); }

void test(int) { std::puts("int"); }

int main()
{
    test(1);   // 优先选择非模板的普通函数
    test(1.2); // 隐式实例化
    test("1");
    return 0;
}
#endif

/*可变参数模板*/
#if 0
// 形参包 C++14
// 形参包展开
void f(const char *, int, double) { std::puts("value"); }
void f(const char **, int *, double *) { std::puts("&"); }
template <typename... Args>
void sum(Args... args) //  const char* arg0, int arg1, double arg2
{
    f(args...);  // 相当于 f(arg0, arg1, arg2)
    f(&args...); // 相当于 f(&arg0, &arg1, &arg2)
}
// Args : 类型形参包, args : 函数形参包
// args : 全部传入的参数 Args : 所有参数的类型

// 模式 ...前面的就是模式

int main()
{
    sum("luse", 1, 1.2);
    return 0;
}
#endif

#if 0
template <typename... Args>
void print(const Args &...args)
{
    int _[]{(std::cout << args << ' ', 0)...};
    std::cout << sizeof(_) / sizeof(int) << std::endl;
    std::cout << sizeof...(Args) << ' ' << sizeof...(args) << std::endl;
}
// (std::cout << arg0 << ' ', 0), (std::cout << arg1 << ' ', 0), (std::cout << arg2 << ' ', 0)
// (std::cout << args << ' ', 0) 模式, 逗号表达式,返回0用来初始化数组，无用的数组
int main()
{
    print("luse", 1, 1.2);
    return 0;
}
#endif

#if 0
template <typename... Args>
void print(const Args &...args)
{
    int _[]{(std::cout << args << ' ', 0)...};
}

template <typename T, std::size_t N, typename... Args>
void f(const T (&array)[N], Args... index)
{
    print(array[index]...);
    // print(array[index0], array[index1], array[index2]);
}

// const T (&array)[N]是一个数组引用 int 10 const int(&array)[10]
// 内建的数字，它的size是它的类型的一部分, int[1]和int[2]不是一个类型 N理所应当可以被推导
int main()
{
    int array[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    f(array, 1, 3, 5);
}
#endif

#if 1
template <typename... Args, typename RT = std::common_type_t<Args...>>
RT sum(const Args &...args)
{
    RT _[]{args...};
    // return std::accumulate(std::begin(_), std::end(_), 0); // 根据初值类型决定返回值类型,此时返回类型为int,产生了精度损失
    return std::accumulate(std::begin(_), std::end(_), RT{}); // RT需要有默认构造
}

int main()
{
    double ret = sum(1, 2, 3, 5.4);
    std::cout << ret << std::endl;
}
#endif
