第3课 - if,switch,do,while,for分析

 　　国嵌课程

if
(1) if语句用于根据条件选择执行语句。

(2) else不能独立存在且总是与它最近的if相匹配。

(3) else语句后可以接连其他if语句。

 

l if语句中零值比较的注意点

bool型变量应该直接出现于条件中，不要进行比较。

bool b = TURE;

if(b)

{

//statement 1

}

else

{

//statement 2

}

普通变量和0值比较时（或者常数），0值应该出现在比较符号左边。

int i = 1;

if(0 == i)

{

//statement 1

}

else

{

//statement 2

}

float型变量不能直接进行0值比较，需要定义精度。

#define EPSION 0.0000001

float f = 0.0;

if((EPSION <= f) && (f <= EPSION))

{

//statement 1

}

else

{

//statement 2

}

 

witch
(1) switch语句对应单个条件多个分值的情形。

(2) 每个case语句分支必须要有break， break，，否则会导致分支重叠。

(3) default语句有必要加上，以处理特殊情况。

l case语句中的值只能是整型或字符型。

l case语句排列顺序分析。

按字母或数字顺序排列各条语句。

正常情况放在前面，异常情况放在后面。

default语句只用于处理真正的默认情况。

#include <stdio.h>

void f1(int i)

{

    if( i < 6 )

    {

        printf("Failed!\n");

    }

    else if( (6 <= i) && (i <= 8) )

    {

        printf("Good!\n");

    }

    else

    {

        printf("Perfect!\n");

    }

}

void f2(char i)

{

    switch(i)

    {

        case 'c':

            printf("Compile\n");

            break;

        

        case 'd':

            printf("Debug\n");

            break;

        

        case 'o':

            printf("Object\n");

            break;

            

        case 'r':

            printf("Run\n");

            break;

        

        default:

            printf("Unknown\n");

            break;       

    }

}

int main()

{

    f1(5);

    f1(9);  

    f2('o');

    f2('d');

    f2('e');

}

 

小结：

(1) if语句实用于需要“按片”进行判断的情形中。

(2) switch语句实用于需要对各个离散值进行分别判断的情形中。

(3) if语句可以安全从功能上代替switch语句，但switch语句无法代替if语句。

(4) switch语句对于多分支判断的情形更加简洁。

 

循环语句的基本工作方式
(1) 通过条件表达式判定是否执行循环体。

(2) 条件表达式遵循if语句表达式的原则。

 

do，while，for的区别
(1) do语句先执行后判断，循环体至少执行一次。

(2) while语句先判断后执行，循环体可能不执行 ，循环体可能不执行。

(3) for语句先判断后执行，相比while更简洁。

#include <stdio.h>

int f1(int n)

{

    int ret = 0;

    int i = 0;    

    for(i=1; i<=n; i++)

    {

        ret += i;

    }

    return ret;

}

int f2(int n)  //传个负数进来，就是死循环。

{

    int ret = 0;

    

    while( (n > 0) && (ret += n--) );

    

    return ret;

}

int f3(int n)

{

    int ret = 0;

    if( n > 0 )   //带0的时候，死循环。

    {

        do

        {

            ret += n--;

        }while( n );

    }   

    return ret;

}

int main()

{

    printf("%d\n", f1(10));  //55

    printf("%d\n", f2(10));  //55

    printf("%d\n", f3(10));  //55

}

 

break和continue的区别
(1) break表示终止循环的执行。

(2) continue表示终止本次循环体，进入下次循环执行。

#include <stdio.h>

#include <malloc.h>

int func(int n)    //可以防止内存泄漏。

{

    int i = 0;

    int ret = 0;

    int* p = (int*)malloc(sizeof(int) * n);

    do

    {

        if( NULL == p ) break;       

        if( n < 0 ) break;

        for(i=0; i<n; i++)

        {

            p[i] = i;

            printf("%d\n", p[i]);

        }     

        ret = 1;

    }while(0);   

    free(p);    

    return ret;

}

int main()

{

    if( func(10) )

    {

        printf("OK");

    }

    else

    {

        printf("ERROR");

    }

}

思考：

switch能否用continue关键字？为什么 ？为什么？

不能。因为continue关键字是为循环语句设计，与分支语句switch没有任何关系。

 