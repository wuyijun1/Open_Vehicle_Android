第1课-基本数据类型分析

什么是数据类型？我们可以这样理解：
l` 数据类型可以理解为固定内存大小的别名`

l `数据类型是创建变量的模子`

char 1byte；short 2byte；int 4byte

变量的本质
l `变量是一段实际连续存储空间的别名`

l `程序中通过变量来申请并命名存储空间`

l `通过变量的名字可以使用存储空间`

实例分析
```c
#include <stdio.h>

typedef int INT32;

typedef unsigned char BYTE;

typedef struct _demo

{

short s;

BYTE b1;

BYTE b2;

INT32 i;

} DEMO;

int main()

{

INT32 i32;

BYTE byte;

DEMO d;

printf("%d,%d\n",sizeof(INT32),sizeof(i32));

printf("%d,%d\n",sizeof(BYTE),sizeof(byte));

printf("%d,%d\n",sizeof(DEMO),sizeof(d));

return 0;

}
```
运行结果：

4   4

1   1

8   8