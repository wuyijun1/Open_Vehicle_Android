# 第2课 - auto,register,static分析 

**auto关键字**
C 言中的 量可以有自己的属性。
在定义变量的候可以加上“属性”关键字。
“属性”关键字指明变量的特有意义。  
	auto即C 言中局部变量的默认属性，编译器默认所有的局部变量都是auto的。
因为register变量可能不存放在内存中，所以不能用“&”来获取register变量的地址。不过这里使用auto关键字时有一个限定条件，那就是必须给申明的变量赋予一个初始值，否则编译器在编译阶段将会报错。
static关键字
l static关键字指明变量的“静态”属性。

l `static`关键字同时具有`“作用域限定符”`的意义。

static修饰的局部变量存储在程序静态区，static的另一个意义是文件作用域的标示符。

注：`static修饰的全局变量作用域只是生命的文件中，修饰的函数作用域只是声明的文件中`。

register关键字
l register`关键字实名将便令存储于寄存器`中。

l register只是请求寄存器变量，但是不一定请求成功（变量的长度必须是cpu可以接受的长度）。
因为register变量可能不存放在内存中，所以不能用“&”来获取register变量的地址。
注：`register变量必须是cpu可以接受的值，不能用&运算符来获取register变量的地址`。

小结：

l` auto变量存储在程序的栈中，默认属性，只能修饰局部变量`。

l `static变量存储在程序静态区`中。

l `register变量请求存储于CPU寄存器，不能定义全局变量。在实时性要求高的时候，尽量使用`。

例程
（1）
```C
#include <stdio.h>
void f1()
{
int i = 0;
i++;
printf("%d\n",i);
}

void f2()
{
static int i = 0;
i++;
printf("%d\n",i);
}

int main()
{
    auto int i = 0;
    register int j = 0;
    static int k = 0; 

printf("%0X\n",&i);
//printf("%0X\n",&j);  C语言只能打印内存中的地址，寄存器中的地址打印不来。
printf("%0X\n",&k);
for(i=0; i<5;i++)
{
f1();
}   //会打印5个1

for(i=0;i<5; i ++)
{
f2();
}//会打印12345，静态的局部变量只会被初始化一次。它的生命周期是全局的，整个程序。
    return 0;
}
```
（2）建立另一个c文件，test2.c
```C
 #include<stdio.h>

int test2_g = 1;

test1.c的内容是：

#include <stdio.h>

extern int test2_g ;

int main()
{
printf(“%d\n”,test2_g);
    return 0;
}
```
运行结果：1

l 若我们将test2.c中内容改为：

static int test2_g = 1;

这样运行第一个程序就会出错，因为我们已经把变量限定在了程序2中。

我们继续改，将程序2中加如下的命令:

int test2_func()

{

return test2_g();

}

将程序1改为：

#include <stdio.h>

extern int test2_func() ;

int main()

{

printf(“%d\n”,test2_func());

    return 0;

}

运行结果：1

l 若我们将程序2改为

#include<stdio.h>

static int test2_g = 1;

static int test2_func()

{

return test2_g();

}

这样程序1再一次无法编译

我们再次改动程序2：

#include<stdio.h>

static int test2_g = 1;

static int test2_func()

{

return test2_g();

}

static int test2_ff()

{

return test2_func();

}

程序1变为：

#include <stdio.h>

extern int test2_ff() ;

int main()

{

printf(“%d\n”,test2_ff());

    return 0;

}

运行结果：1

以上的例子都体现了static文件限定符的作用。

 