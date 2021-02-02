# 第7课-enum和typedef分析

 

枚举类型的使用方法
(1) enum是一种自定义类型。

(2) enum默认常量在前一个值的基础上一次加1。

(3) enum类型的变量只能取定义时的离散值。

enum Color

{

GREEN,    //默认第一个的值是0

RED = 2,   

BLUE      //默认是前一个数值的基础上加一

};

enum Color c = GREEN;

printf(“%d\n”,c);    //0

这里时不能取地址的，常量是不用分配空间，取地址的。编译时，会进入符号表。

枚举类型和#define的区别
(1) #define宏常量只是简单的进行值替换，枚举常量是真正意义上的常量。

(2) #define宏常量无法被调用，枚举常量可以。

(3) #define宏常量无类型信息，枚举常量是一种特定类型的常量。

 

typedef的意义
(1) `typedef用于给一个已经存在的数据类型重命名。`

(2) typedef并没有产生新的类型。

(3) typedef重定义的类型不能进行unsigned和signed扩展。

 

typedef和#define的区别
(1) typedef是给已有的类型取别名。

(2) #define为简单的字符串替换，无别名的概念。

 

思考：

我们现在比较下面2个语句的区别：

tppedef char* PCHAR

PCHAR p1, p2

//p1， p2是char*类型的指针。

 

#define PCHAR char*

PCHAR p3, p4

//p3是char*类型的指针，p4是个char类型。