第20课 - #和##运算符使用解析

 

#运算符
`#运算符用于在预编译期将宏参数转换为字符串`

#include <stdio.h>

#define CONVERS(x) #x

int main()

{  

    printf("%s\n", CONVERS(Hello world!));

    printf("%s\n", CONVERS(100));

    printf("%s\n", CONVERS(while));

    printf("%s\n", CONVERS(return));

    return 0;

}

运算结果：Hello world! 

                 100 

                  while 

                  return

例子：#运算符在宏中的妙用

#include <stdio.h>

#define CALL(f, p) (printf("Call function %s\n", #f), f(p))

int square(int n)

{

    return n * n;

}

int f(int x)

{

    return x;

}

int main()

{

    

    printf("1. %d\n", CALL(square, 4));

    printf("2. %d\n", CALL(f, 10));

    return 0;

}

运行结果：Call function square

　　　　　　16

          Call function f

　　　　　　10

 

 

 

 

##运算符
`##运算符用于在预编译期粘连两个符号`

#include <stdio.h>

#define NAME(n) name##n

int main()

{  

    int NAME(1);  //相当于定义了NAME1

    int NAME(2);  //相当于定义了NAME2

    NAME(1) = 1;

    NAME(2) = 2;   

    printf("%d\n", NAME(1));

    printf("%d\n", NAME(2));

    return 0;

}

运行结果：1

                  2

 

例子：利用##定义结构类型

#include <stdio.h>

#define STRUCT(type) typedef struct _tag_##type type;\

struct _tag_##type

STRUCT(Student)   //将结构体重新定义得到的

{

    char* name;

    int id;

};

//经过预处理得到的会是下面的语句

/*

typedef struct _tag_Student Student;struct _tag_Student

{

    char* name;

    int id;

};

*/

int main()

{  

    Student s1;

    Student s2;

    s1.name = "s1";

    s1.id = 0;   

    s2.name = "s2";

    s2.id = 1;  

    printf("%s\n", s1.name);

    printf("%d\n", s1.id);

    printf("%s\n", s2.name);

    printf("%d\n", s2.id);

    return 0;

}

宏很强大，有时候是函数所达不到的。

注意：PRINT_XN()宏用#运算符组合字符串，`##运算符把记号组合为一个新的标识符`。

其实，##运算符在这里看来并没有起到多大的便利，反而会让我们感觉到不习惯。但是，`使用##运算符有时候是可以提高封装性及程序的可读性`。如跑实时操作系统的stm32程序中，关于gpio的硬件驱动中有如下代码：

#define __STM32_PIN(index, gpio, gpio_index) \
{ \
index, GPIO##gpio##_CLK_ENABLE, GPIO##gpio, GPIO_PIN_##gpio_index \
}
假设这样使用该宏：

__STM32_PIN(7, C, 13)

该宏将展开为：

{7, GPIOC_CLK_ENABLE, GPIOC, GPIO_PIN_13}