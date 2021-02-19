# C语言-第34课 - 可变参数分析与宏分析 

第34课 - 可变参数分析与宏分析
 
问题引入；如何编写一个可以计算n个数平均值的函数？
下面是一个将所求的元素组成数组，遍历求和的方法：
```C
#include <stdio.h>
float func(int array[], int size)
{
    int i = 0;
    float avr = 0;
    for(i=0; i<size; i++)
    {
        avr += array[i];
    }    
    return avr / size;
}
int main()
{
    int array[] = {1, 2, 3, 4, 5};    
    printf("%f\n", func(array, 5));    
    return 0;
}
```
这里的不方便就是，我们必须把我们要求平均值的数组成一个数组。我们下面看看可变参数的方法:
 ```C
#include <stdio.h>
#include <stdarg.h>
float average(int n, ...) //...表示可变参数
{
    va_list args;   //args就是可变参数的列表
int i = 0;
    float sum = 0;  
    va_start(args, n);   //初始化列表
    for(i=0; i<n; i++)
    {
        sum += va_arg(args, int); //int表示指明类型
    }   
    va_end(args);    
    return sum / n;
}
int main()
{
    printf("%f\n", average(5, 1, 2, 3, 4, 5));
    printf("%f\n", average(4, 1, 2, 3, 4));    
    return 0;
}
 
 ```
可变参数
(1) C语言中可以定义参数可变的函数（如printf函数）。这里就像我们的open函数有两种方式。
(2) 参数可变的函数的实现依赖于stdarg.h头文件。
(3) va_list变量与va_start，va_end和va_arg配合使用能够访问参数值。
 
可变参数的限制
(1) 可变参数必须从头到尾按照顺序逐个访问，不可以取中间的参数。
(2) 参数列表中至少存在一个确定的命名参数。
(3) 可变参数宏无法判断实际存在的参数的数量。
(4) 可变参数宏无法判断参数的实际类型。
注意：va_arg中如果指定了错误的类型，那么结果是不可预测的。
 
l 小结：
可变参数是C语言提供的一种函数的设计技巧。
l 可变参数的函数提供了一种更方便的函数调用方式。
l 可变参数必须顺序的访问。
l 无法直接访问可变参数裂变中间的参数值。
 
 
函数VS宏
(1) 宏是由预处理直接替换展开的，编译器不知道宏的存在。
(2) 函数是由编译器直接编译的实体，调用行为由编译器决定。
(3) 多次使用宏会导致程序代码量增加。
(4) 函数是跳转执行的，因此代码量不会增加。
(5) 宏的效率比函数的高，因为是直接展开，无调用开销。
(6) 函数调用时会创建活动记录，效率不如宏。
例：
```C
#include <stdio.h>
#define RESET(p, len) while( len > 0) ((char*)p)[--len] = 0  //宏
void reset(void* p, int len)   //函数
{
    while( len > 0 ) 
    {
        ((char*)p)[--len] = 0;
    }
}
int main()
{
    int array[] = {1, 2, 3, 4, 5};
    int len = sizeof(array);
int i;
for(i=0;i<5;i++)
{
printf("%d ",array[i]);
}
    printf("\n");
    reset(array, len);
for(i=0;i<5;i++)
{
printf("%d ",array[i]);
}
printf("\n");
    //RESET(array, len);
    return 0;
}
```
运行结果：1 2 3 4 5
          0 0 0 0 0 
将reset(array, len);改为RESET(array, len);运行结果一样。
我们使用gcc -E test.c -o test.i可以得到编译的中间程序test.i 这里面我们能看到宏变量被转换成了函数中的代码。减少了进退栈的开销。
 
宏的有点和缺点
宏的效率比函数的高，但是它的副作用巨大，容易出错。
```C
#include <stdio.h>
#define ADD(a, b) a + b
#define MUL(a, b) a * b
#define _MIN_(a, b) ((a) < (b) ? (a) : (b))
int main()
{
    int i = 1;
    int j = 10;    
    printf("%d\n", MUL(ADD(1, 2), ADD(3, 4)));
    printf("%d\n", _MIN_(i++, j));    
    return 0;
}
```
运行结果：11
  2
分析：这和我们分析的21,1结果不同，我们将宏展开发现了这两条语句：printf(“%d\n”,1 + 2 * 3 +4);printf(“%d\n”,((i++) < (j) ? (i++) : (j)));这就是宏的缺点，若是我们在第一条语句中加括号，会有效果。对于第二条语句就无法解决。
 
函数的有点和缺点
函数存在实参到形参的传递，因此没有副作用，但是函数需要创建活动对象，效率受影响。
```C
#include <stdio.h>
int add(int a, int b)
{
    return a + b;
}
int mul(int a, int b)
{
    return a * b;
}
int _min_(int a, int b)
{
    return a < b ? a : b;
}
int main()
{
    int i = 1;
    int j = 10;    
    printf("%d\n", mul(add(1, 2), add(3, 4)));
    printf("%d\n", _min_(i++, j));
    return 0;
}
```
运行结果：21
  1
 
宏无可代替的优势
宏参数可以是任何C语言的实体。
宏编写的_MIN_参数类型可以是int，float等等。
宏的参数可以是类型名。
```C
#include <stdio.h>
#include <malloc.h>
#define MALLOC(type, n) (type*)malloc(n * sizeof(type))
int main()
{
    int* p = MALLOC(int, 5);   
    int i = 0;   
    for(i=0; i<5; i++)
    {
        p[i] = i + 1;   
        printf("%d ", p[i]);
    }
printf("\n"); 
    free(p); 
    return 0;
}
```
运行结果：1 2 3 4 5
 
   
小结：
l 宏和函数并不是竞争对手。
l 宏能够接受任何类型的参数，效率高，易出错。
l 函数的参数必须是固定类型，效率稍低，不易出错。
l 宏可以实现函数不能实现的功能。