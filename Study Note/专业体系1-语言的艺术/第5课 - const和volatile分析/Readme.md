第5课 - const和volatile分析

国嵌课程笔记

# 5.1  const

1. 概念

l 在C语言中const修饰的变量是只读的，其本质还是变量

l const修饰的变量会在内存中占用空间

l `本质上const只对编译器有用，在运行是无用`

例程
#include<stdio.h>

#include<malloc.h>

int main()

{

const int cc = 1;

printf("%d",cc);

return 0;

cc = 3;

printf("%d",cc);

}

这个程序就是运行不了的，会显示给只读文件赋值。我们做如下修改：

#include<stdio.h>

#include<malloc.h>

int main()

{

const int cc = 1;

int *p = (int*)&cc;

printf("%d\n",cc);

*p = 3;

printf("%d\n",cc);

return 0;

}

运行结果：1

  3

现代编译器对const的处理方式
当赋予cc一个const变量的值的时候，就会在程序中，生成一个表，对应的分别是：cc，类型，数值。

左值：报错

右值：（1）int cd = cc; 就会用cc对应的值来代替它

（2）int *p = (int*)&cc; 当执行这个时，再去执行（1）的内容就不会去替换了，而是变成了传统的赋值。

const修饰数组
l 在c语言中const修饰的数组是只读的。

l const修饰的数组空间不可被改变（针对现在的编译器，但也不是绝对的）。

const中的内容也是可以改的，如下：

#include<stdio.h>

#include<malloc.h>

int main()

{

const int A[5] = {1,2,3,4,5};

int *p = (int*)A;

int i = 0;

for(i=0; i<5; i++)

{

p[1] = 5-i;

}

return 0;

}

const修饰指针
const int* p;   //p可变（p指向的地址），p指向的地址的内容不可变

int const* p;   //p可变，p指向的内容不可变

int* const p;   //p不可变，p指向的内容不可变

const int* const p;  //p和p指向的内容都不可变

口诀：左数右指

当const出现在*号左边时指针指向的数据为常量，当const出现在*右边时指针本身为常量。

const修饰函数参数和返回值
const修饰函数参数表示在函数体内不希望改变参数的值

const修饰函数返回值表示返回值不可改变，多用于返回指针的情形

如下程序：
#include<stdio.h>

#include<malloc.h>

const int* func()

{

static int count = 0;

count++;

return &count;

}

int main()

{

const int* p = func();//若是改成 int* p = func();就会显示有问题。

printf("%d\n",*p);

return 0;

}

运行结果是：1

 

# 5.2  volatile（和很少见）

l volatile可以理解为“编译器警告指示字”。

l volatile用于告诉编译器必须每次去内存中取变量值

l volatile主要修饰可能被多个线程访问的变量

l volatile也可以修饰可能被未知因数更改的变量

int obj = 10;

int a = 0;

int b = 0;

a = obj;

sleep(100);

b = obj;

对于这一段程序，编译器在编译的时候发现obj没有被当成左值使用，因此会“聪明”的直接将obj替换成10，而把a和b都复制为10。这么做是没问题的，但是若是在sleep()过程中，来了一个硬件中断，改变了obj中的值，假设是100，但是我们仍会给b赋值10。

这样就出了因为优化而出现的错误。为了改善这个问题，我们可以这样改写第一行代码：

volatile int obj = 10;这样系统就不会对它做优化。

思考题：

const和volatile可不可以同时修饰一个变量
可以

const valatile int i = 0; 这个时候i具有什么属性？编译器如何处理这个变量？
如果一个变量不会被本程序改变，通常可能给它加上const，但如果该变量可能被其他程序改变而本程序又在检测这个变量的值，就需要给它加上volatile，于是变量就同时有volatile和const了，这个时候i具有const和volatile的双重属性。i变量不可以在编译过程中被程序代码修改，同时编译器不得对i进行优化编译。

 