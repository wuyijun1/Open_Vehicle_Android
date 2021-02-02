# 第19课 - #pragma预处理分析

 

# #pragma简介
(1) #pragma是编译器指示字，用于指示编译器完成一些特定的动作。

(2) #pragma所定义的很多指示字是编译器和操作系统特有的。

(3) #pragma在不同的编译器间是不可移植的。

l 预处理器将忽略它不认识的#pragma指令。

l 两个不同的编译器可能以两种不同的方式解释同一条#pragma指令。

一般用法：#pragma parameter

注：不同的parameter参数语法和意义各不相同

 

#pragma message
(1) message参数在大多数的编译器中都有相似的实现。

(2) message参数在编译时输出消息到编译输出窗口中。

(3) message可用于代码的版本控制。

注意message是VC特有的编译器指示字，GCC中将其忽略。

 

例子：#pragma在不同编译器下的使用示例

#include <stdio.h>

#if defined(ANDROID20)

    #pragma message("Compile Android SDK 2.0...")

    #define VERSION "Android 2.0"

#elif defined(ANDROID23)

    #pragma message("Compile Android SDK 2.3...")

    #define VERSION "Android 2.3"

#elif defined(ANDROID40)

    #pragma message("Compile Android SDK 4.0...")

    #define VERSION "Android 4.0"

#else

    #error Compile Version is not provided!

#endif

int main()

{

    printf("%s\n", VERSION);

    return 0;

}

为了在VC中，显示像在linux下面一样的结果。我们在工程-设置中，添加相应的变量ANDROID23，就会显示我们要的结果了。

在linux下面，gcc和g++是识别不了pragma的

 

#pragma pack
（1）什么是内存对齐？

不同类型的数据在内存中按照一定的规则排列；而不是顺序的一个接一个的排放，这就是对齐。

#include <stdio.h>

 

int main()

{   

struct Test1

{           //   起始    大小   空

char c1;  //   0        1   

short s;  //    2        2    1

char c2;  //   4        1     3

int i;    //    8        4

};

 

struct Test2

{

char c1;  //   0        1    

char c2;  //   1        1

short s;  //   2         2

int i;     //   4        4

};

printf("%d\n",sizeof(struct Test1));

printf("%d\n",sizeof(struct Test2));

 

}

运行结果：12

                  8

 

（2）为什么需要内存对齐？

l CPU对内存的读取不是连续的，而是分成块读取的，块的大小只能是1、2、4、8、16字节。

l 当读取操作的数据未对齐，则需要两次总线周期来访问内存，因此性能会大打折扣。

l 某些硬件平台只能从规定的地址处取某些特定类型的（偶地址）数据，否则抛出硬件异常。

#pragma pack能够改变编译器的默认对齐方式

 

#include <stdio.h>

int main()

{   

struct Test

{

char c1;

short s;

char c2;

int i;

};

 

#pragma pack(2)

struct Test1

{

char c1;

short s;

char c2;

int i;

};

#pragma pack

 

 

#pragma pack(4)

struct Test2

{

char c1;

char c2;

short s;

int i;

};

#pragma pack

 

printf("%d\n",sizeof(struct Test));   //系统默认是4个字节对齐

printf("%d\n",sizeof(struct Test1));

printf("%d\n",sizeof(struct Test2));

 

}

运行结果：12

                  10

                  8

 

struct占用的内存大小
（1）第一个成员起始于0偏移处。

（2）每个成员按其类型大小和指定对齐参数n中较小的一个进行对齐。

l 偏移地址和成员占用大小均需对齐。

l 结构体成员的对齐参数为其所有成员使用的对齐参数的最大值。

（3）结构体总长度必须为所有对齐参数的整数倍。

 

Intel和微软的面试题--结构体大小的计算

#include <stdio.h>

#pragma pack(8)

struct S1

{                 //  起始  大小    空

    short a;      //  0      2  

    long b;       //  4      4       2

};

struct S2

{

    char c;      //  0       1     

    struct S1 d;   //  4       8      3             

    double e;    //  16      8      4

};  //对于结构体选它的对齐大小要选它结构中，最大的数据大小。

 

#pragma pack()

int main()

{

    struct S2 s2;  

    printf("%d\n", sizeof(struct S1));

    printf("%d\n", sizeof(struct S2));

    printf("%d\n", (int)&(s2.d) - (int)&(s2.c));

/*d和c的起始地址查了4，所以结果是4 */

    return 0;

}

运行结果：8

                 24

                 4

linux下运行是 8，20，4，因为在linux下面会变成默认的4字节对齐。

struct S2

{

    char c;      //  0       1     

    struct S1 d;   //  4       8      3             

    double e;    //  12      8      0

};  //对于结构体选它的对齐大小要选它结构中，最大的数据大小。

我们看一下12的选取，double类型占8个字节，8字节和4字节（对齐字节）进行比较，选小的4。找4的最小超过的公倍数12。

 

课后思考：

问：

结构体变量是否可以直接用memcmp函数进行相等判断？

答：

如果结构体内都是基本的数据成员，那是可以的，例如常见的 POINT 结构，RECT结构等，可以用memcmp进行相等判断；
如果结构体中不全是基本数据成员，例如结构体中有个CString 成员，那就不能这样判断了。
所以具体要看你的结构体定义，不能一概而论。