# 第6课 - struct和union分析

 

思考：

空结构体占用多大内存？

下面程序输出什么？

#include<stdio.h>

#include<time.h>

struct D

{

};

int main()

{

struct D d1;

struct D d2;

printf("%d\n",sizeof(struct D));

printf("%d, %0X\n",sizeof(d1), &d1);

printf("%d, %0X\n",sizeof(d2), &d2);

return 0;

}

运行结果：

1

1, 18FF44

1, 18FF40

C语言中并没有明确规定空的结构体占多大的内存，在gcc编译器中运行就会是0，在g++中的运行结果，显示的是1。我们尽量让空结构体也占一个字节，防止不同的变量占用相同的地址。

 

由结构体产生柔性数组
(1) 柔性数组即数组大小待定的数组

(2) C语言中结构体的最后一个元素可以是大小未知的数组

(3) C语言中可以由结构体产生柔性数组

#include <stdio.h>

#include <malloc.h>

typedef struct _soft_array

{

    int len;

    int array[];

}SoftArray;

//柔性数组，假设后面的 int array[]占用0个字节，所以，这个结构体是占用4个字节的。

int main()

{  

    int i = 0;

SoftArray* sa = (SoftArray*)malloc(sizeof(SoftArray) + sizeof(int) * 10);  

//前面的4个空间存int len，后面的40个空间存int array[]。柔性数组有长度信息。

    sa->len = 10;   

    for(i=0; i<sa->len; i++)

    {

        sa->array[i] = i + 1;

    }    

    for(i=0; i<sa->len; i++)

    {

        printf("%d ", sa->array[i]);   

    }

    free(sa);

    printf("\n");    

    return 0;

}

运行结果：1 2 3 4 5 6 7 8 9 10

创建费波纳茨数列

（1）定义柔性数组（2）创建柔性数组（3）生成费波纳茨数列（4）释放柔性数组

#include <stdio.h>

#include <malloc.h>

typedef struct _soft_array

{

    int len;

    int array[];

}SoftArray;

SoftArray* create_soft_array(int size)  //创建柔性数组

{

    SoftArray* ret = NULL;

    if( size > 0 )

    {

        ret = (SoftArray*)malloc(sizeof(*ret) + sizeof(*(ret->array)) * size);

        

        ret->len = size;

    }  

    return ret;

}

void fac(SoftArray* sa)   //生成数列

{

    int i = 0;    

    if( NULL != sa )

    {

        if( 1 == sa->len )

        {

           sa->array[0] = 1;

        }

        else

        {

            sa->array[0] = 1;

            sa->array[1] = 1;

            for(i=2; i<sa->len; i++)

            {

                sa->array[i] = sa->array[i-1] + sa->array[i-2];

            }

        }

    }

}

void delete_soft_array(SoftArray* sa) //释放柔性数组

{

    free(sa);

}

int main()

{

    int i = 0;

    SoftArray* sa = create_soft_array(10);

    fac(sa);

    for(i=0; i<sa->len; i++)

    {

        printf("%d ", sa->array[i]);

    }

printf("\n");

    delete_soft_array(sa);

    return 0;

}

运行结果：1 1 2 3 5 8 13 21 34 55

 

 

 

 

 

 

## union和struct的区别
struct中的每个域在内存中都独立分配空间

union只分配最大域的空间，所有域共享这个空间 ，所有域共享这个空间

#include <stdio.h>

struct A

{

int a;

int b;

int c;

};

union B

{

int a;

int b;

int c;

};

int main()

{

printf("%d\n", sizeof(struct A));  //12

    printf("%d\n", sizeof(union B));   //4

    return 0;

}

 

 

union使用的注意事项
union的使用受系统大小端的影响

 

#include<stdio.h>

#include<time.h>

union C

{

int i;

char c;

};

int main()

{

union C b;

b.i = 1;

printf("%d\n",b.c);

return 0;

}

若是大端模式打印0，若是小端模式打印1.