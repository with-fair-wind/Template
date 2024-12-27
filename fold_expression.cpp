// 折叠表达式

#include <array>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <tuple>
#include <type_traits>

/*
一元右折叠 `(E 运算符 ...)` 成为 `(E1 运算符 (... 运算符 (EN-1 运算符 EN)))`
一元左折叠 `(... 运算符 E)` 成为 `(((E1 运算符 E2) 运算符 ...) 运算符 EN)`
二元右折叠 `(E 运算符 ... 运算符 I)` 成为 `(E1 运算符 (... 运算符 (EN−1 运算符 (EN 运算符 I)))
二元左折叠 `(I 运算符 ... 运算符 E)` 成为 `((((I 运算符 E1) 运算符 E2) 运算符 ...) 运算符 EN)`
E是使用了形参包的整个表达式
*/
#if 0
#if 0
template <typename... Args>
void print(const Args &...args)
{
    ((std::cout << args << ' '), ...);
}
// 一元右折叠
// E:(std::cout << args << ' ')
// 运算符:,
// 最外层括号是折叠表达式语法的一部分

void print(const char(&args0)[5], const int& args1, const double& args2) {
    (std::cout << args0 << ' '), ((std::cout << args1 << ' '), (std::cout << args2 << ' '));
}
#else
template <typename... Args>
void print(const Args &...args)
{
    (..., (std::cout << args << ' '));
}
// 一元左折叠

void print(const char (&args0)[5], const int &args1, const double &args2)
{
    ((std::cout << args0 << ' '), (std::cout << args1 << ' ')), (std::cout << args2 << ' ');
}

int main()
{
    print("kk", 1, 1.2);
    return 0;
}
#endif
#endif

#if 0
template <int... I>
constexpr int v_right = (I - ...); // 一元右折叠

template <int... I>
constexpr int v_left = (... - I); // 一元左折叠

int main()
{
    std::cout << v_right<4, 5, 6> << std::endl; // (4-(5-6))
    std::cout << v_left<4, 5, 6> << std::endl;  // ((4-5)-6)
    return 0;
    // 右折叠就是"先算"右边，左折叠就是"先算"左边
}
#endif

#if 1
template <typename... Args>
void print(Args &&...args)
{
    (std::cout << ... << args) << std::endl;
}
// 二元左折叠
// I : 初值 std::cout
// 运算法 : <<
// E : args

template <int... I>
constexpr int v1 = (I + ... + 10); // 二元右折叠

template <int... I>
constexpr int v2 = (10 + ... + I); // 二元左折叠

int main()
{
    print("kk", 1, 1.2);
    std::cout << v1<1, 2, 3, 4> << std::endl;
    // (1+(2+(3+(4+10))))
    std::cout << v2<1, 2, 3, 4> << std::endl;
    // ((((10+1)+2)+3)+4)
    return 0;
}
#endif