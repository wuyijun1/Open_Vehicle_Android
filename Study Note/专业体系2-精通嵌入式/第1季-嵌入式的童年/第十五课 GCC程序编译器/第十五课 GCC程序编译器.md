嵌入式-第一季-第15课 

第十五课 GCC程序编译器
 
 
一．GCC简介
1．gcc特点
gcc（GUN  C  Compiler）是GNU推出的功能强大、性能优越的多平台编译器，是GNU的代表之一。gcc可以在多种硬体平台上编译出可执行文件，其执行的效率与一般的编译器相比平均效率要提高20%-30%。
2．gcc的使用
gcc最近本额度用法是：
gcc  [options]  filename
options：编译器所需要的编译选项
Filenames：要编译的文件名
例如：
用touch hello.c创建hello.c文件，利用vim进入hello.c进行编译
 
hello.c
#include <stdio.h>
int main(viod)
{
printf(“hello world!\n”);
return 0;
}
#gcc hello.c
运行完 gcc helle.c后，会出现一个a.out文件运行下面的程序就会出现我们要打印的文字了。
#./a.out
二．Gcc深入使用
Gcc编译流程
Gcc编译器能将C/C++语言源程序、汇编程序形成可执行文件。整个形成过程被细分为四个阶段：
gcc -E hello.c -o hello.i   -E表示预处理，-o表示输出
gcc -S hello.i -o hello.s
gcc -C hello.s -o hello.o
gcc hello.o -o
Hello.c 预处理Hello.i 编译 Hello.s 汇编 hello.o链接hello
Gcc通过后缀来区别输入文件的类别，常见类型有：
.c为后缀的文件：C语言源代码文件
.a为后缀的文件：是由目标文件构成的文件库
.C，.cc或.cxx为后缀的文件：是C++源文件代码
.h为后缀的文件：头文件
.o为后缀的文件：是编译后的目标文件
.s为后缀的文件：是汇编语言源代码文件
Gcc编译选项
掌握Gcc的使用方法，最重要的是掌握Gcc的编译选项，Gcc编译器额编译选项大约有100多个，其中多数是我们根本用不到的，这里我们只介绍最基本、最重要的参数。
-o output_filename：确定可执行文件的名称为output_filename。如果不给出这个选项，gcc就给出预设的可执行文件a.out。
-O，对程序进行优化编译、链接，采用这个选项，整个代码会在编译、链接过程中进行优化处理，这样产生的可执行文件的执行效率可以提高，但是，编译、链接的速度就会相应的慢下来。
-O2，比-O更好的优化编译、链接，当然整个编译、链接过程会更慢。
举例：
#include<stdio.h>
int main(void)
{
double counter;
double result;
double temp;
for(counter=0;counter<2000.0*2000.0*2000.0/20.0+2020;
counter+=(5-1)/4)
{
temp  =  counter1979;
result  =  counter;
}
printf(result is %lf\\n,result);
result  0;
}
（1）gcc optimize.c -o optimize
time ./optimize
（2）gcc -O optimize.c -o optimize
time ./optimize
对比两次执行的输出结果不难看出，程序的性能的确得到了很大幅度的改善。
-c:不链接。编译器只是有输入的.c等源代码文件编译生成.o为后缀的中间文件。
-g:产生调试工具（GNU的gdb）所需要的符号信息，要相对便一处的程序进行调试，就必须加入这个选项。
-I dirname：将dirname所指出的目录作为编译器所寻找的头文件标准路径。
C程序中的头文件包含两种情况：
#include<A.h>
#include”B.h”
对于<>，gcc在系统预设的头文件目录（如/usr/include）中搜寻相应的文件；而对于””，gcc在当前的目录中搜寻头文件。-I选项的作用是告诉gcc，如果在当前目录中没有找到需要的文件，就到指定的dirname目录中去寻找。
例如：gcc foo.c -I /home/include -o foo
l 编译错误与便已经奥都是我们的程序设计中经常遇到的问题。编译错误一般是带吗中出现语法错误，程序无法运行，必须修改。编译警告是编译器发现程序中不合理的地方（比如类型不匹配），建议最好修改，但是不修改也是可以编译的。
-Wall：生成所有警告信息
-w：不生成任何警告信息
-DMACRO：定义MACRO宏，等效于在程序中使用#define MACRO