# [第三季-第2课-GDB程序调试 ](https://www.cnblogs.com/free-1122/p/11341929.html)

2.1GDB简介
l  GDB是GNU发布的一款功能强大的程序调试工具。GDB主要完成下面三个方面的功能：
启动被调试的程序。
让被调试的程序在指定的位置停住。
当程序被停住时，可以检测程序的状态（如变量值）。
```
#include<stdio.h>
void myprint(int res)
{
printf(“result = %d \n”,res);
}
void main()
{
Int i;
long result = 0;
for(i=1;i<=100;i++)
{
result+=1;
}
myprint(result) ;
}
```
2.2 `GDB的使用流程`
1.编译生成可执行文件
gcc -g tet.c -o tst
启动GDB
gdb tst
在main函数处设置断点
break main
运行程序
run
利用更加丰富的gdb命令对程序进行调试。
2.3 `GDB命令`
list(l) 查看程序
break(b) 函数名 在某函数入口处添加断点
break(b) 行号 在指定行添加断点
break(b) 文件名：行号 在指定文件的指定行添加断点
info break 查看所有设置的断点
delete 断点编号 删除断点
next(n) 单步运行程序（不进入子函数）
step(s) 单步运行程序（进入子函数）
continue(c) 继续运行程序
print(p) 变量名 查看指定变量值
set var=value 设置变量的值
quit(q) 退出gdb