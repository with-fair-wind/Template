#include "test.h"
#include "test_template.h"

/*
test.h 只是存放了函数声明，函数定义在 test.cpp 中，我们编译的时候是选择编译了 main.cpp 与 test.cpp 这两个文件，那么为什么程序可以成功编译运行呢
是怎么找到函数定义的呢？明明 main.cpp 其实预处理过后只有函数声明而没有函数定义。
这就是链接器做的事情，如果编译器在编译一个翻译单元（如 main.cpp）的时候，如果发现找不到函数的定义，那么就会空着一个符号地址，将它编译为目标文件。期待链接器在链接的时候去其他的翻译单元找到定义来填充符号。
test.cpp 里面存放了 f 的函数定义，并且具备外部链接，在编译成目标文件之后之后，和 main.cpp 编译的目标文件进行链接，链接器能找到函数 f 的符号。
不单单是函数，全局变量等都是这样，这是编译链接的基本原理和步骤

模板，只有你“用”了它，才会生成实际的代码。
单纯的放在一个 .cpp 文件中，它不会生成任何实际的代码，自然也没有函数定义，也谈不上链接器找符号了。
*/
int main()
{
    f(); // 非模板，OK
    // f_t(1); // 模板 链接错误
    return 0;
}