第4课 - goto,void,extern,sizeof分析
国嵌课程笔记

4.1 goto

l 高手潜规则：禁用goto

l 项目经验：程序质量与goto的出现次数成反比

l 最后的判决：将goto打入冷宫

我们分析如下的程序：

#include <stdio.h>

void func(int n)

{

    int* p = NULL;

    if(  n < 0 )

    {

        goto STATUS;

    }

    p = malloc(sizeof(int) * n);

STATUS:

    p[0] = n;    

}

int main()

{  

    f(1);

    f(-1);   

    return 0;

}

我们运行该程序，若是n小于0，就会跳过堆内存的分配，而此时的p是空指针，给空指针赋值，就会出现程序的崩溃。当n是大于0的情况下，就会执行很好。也就是说goto命令破坏了，程序的顺序执行。

4.2 void

1. void修饰函数返回值和参数。

如果函数没有返回值，那么应该将其声明为void型；如果函数没有参数，应该声明其参数为void。viod修饰函数返回值和参数仅为了表示无。

不存在void变量
c语言没有定义void究竟是多大内存的别名，没有void的标尺，无法在内存中剪裁出void对应的变量。

void指针的意义
(1) c语言规定只有相同类型的指针才可以相互赋值

(2) void*指针作为左值用于“接收”任意类型的指针

(3) void*指针作为右值给其它指针时需要强制类型转换

int* pI = (int*)malloc(sizeof(int));  //malloc返回值是void的指针

char* pC = (char*)malloc(sizeof(char));

void* p = NULL;

int* pni = NULL;

char* pnc = NULL;

 

p = pI;   // ok

pbi = p;  // oops!

p = pC; // ok

pnc = p; // oops!

（4）手把手教学--实现memset函数    

 

#include<stdio.h>

void* memset(void* p, char v, int size)

{

void* ret = p;

char* dest = (char*)p;

int i = 0;

for(i=0;i<size;i++)

{

dest[i] = v;

}

return ret;

}

int main()

{

int a[5] = {1,2,3,4,5};

int i = 0;

for(i=0; i<5; i++)

{

printf("%d\n",a[i]);

}

memset(a, 0, sizeof(a));

for(i=0; i<5; i++)

{

printf("%d\n",a[i]);

}

return 0;

}

运行结果：

1

2

3

4

5

0

0

0

0

0

用void*可以传递任意类型的地址值，无论是数还是数组

4.3 extern

l extern用于声明外部定义的变量和函数

l extern用于“告诉”编译器用C方式编译

C++编译器和一些变种C编译器默认会按“自己”的方式编译函数和变量，通过extern关键可以命令编译器“以标准C方式进行编译”。例如：

extern “C”

{

int f(int a,int b)

{

return a + b;

}

}

例程：

// test.c

#include <stdio.h>

/*

extern "C"

{

    int add(int a, int b)

    {

        return a + b;

    }

}

*/

extern int g;

extern int get_min(int a, int b);

int main()

{  

printf("%d\n",g);

printf("%d\n",get_min(1, 5));

    return 0;

}

// test2.c

int g = 100;

int get_min(int a, int b)

{

    return (a < b) ? a : b;

}

4.4 为sizeof证明

l sizeof是编译器的内置指示符，不是函数

l sizeof用于“计算”相应实体所占的内存大小

l sizeof的值在编译期间就已经确定

例程：

#include <stdio.h>

int main()

{  

    int a;

    printf("%d\n", sizeof(a));

    printf("%d\n", sizeof a);

    printf("%d\n", sizeof(int));

    return 0;

}

运行结果：

4

4

4

 