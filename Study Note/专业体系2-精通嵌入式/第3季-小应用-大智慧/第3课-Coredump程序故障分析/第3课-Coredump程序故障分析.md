# [第三季-第3课-Coredump程序故障分析 ](https://www.cnblogs.com/free-1122/p/11341931.html)


问：如果一个程序运行3天后才会出错，这个时候难道需要我们一直用GDB调试程序3天吗？
答：我们需要更加厉害的武器——Core dump
3.1 core dump的定义
Core dump又叫做核心转存。当程序在运行过程中发生异常，这时linux系统可以把程序出错时的内存内容存储在一个core文件中，这个过程叫做`Core dump`。
3.2 segment fault
Linux应用程序在运行的过程中，经常会遇到segment fault（段错误）这样的错误。产生的原因通常有一下几种：
l  数组访问越界
l  访问空指针
l  栈溢出
l  修改只读内存
3.3 **core dump使能**
在linux系统中，默认是关闭core dump功能的，但是可以通过来ulimit命令打开/关闭core dump功能。
打开：ulimit -c unlimited
关闭：ulimit -c 0
3.4 core文件分析
发生core dump之后，可以使用gdb进行查看core文件的内容，以定位程序出错的位置。
用法：gdb程序名core文件名
例如：gdb ./test core.12525，必须是./test的形式
3.4  [错误分析] (https://www.cnblogs.com/gary-guo/p/5553220.html)
在我们用gcc命令去生成文件的时候，我们需要加上-g这一个指令。因为有这个指令的存在我们才能看到文件的详细信息。在我们执行./seg1时候生成的core文件能将具体的问题加以体现。

core文件生成路径:
输入可执行文件运行命令的同一路径下。
若系统生成的core文件不带其它任何扩展名称，则全部命名为core。新的core文件生成将覆盖原来的core文件。

1）/proc/sys/kernel/core_uses_pid可以控制core文件的文件名中是否添加pid作为扩展。文件内容为1，表示添加pid作为扩展名，生成的core文件格式为core.xxxx；为0则表示生成的core文件同一命名为core。
可通过以下命令修改此文件：
sudo nano /proc/sys/kernel/core_uses_pid

把0改为1就可以了。

2）proc/sys/kernel/core_pattern可以控制core文件名格式。
可通过以下命令修改此文件：

sudo nano /proc/sys/kernel/core_pattem

把core改为core-%e-%p-%t 

这样就产生的文件名为core-命令名-pid-时间戳
以下是参数列表:
    %p - insert pid into filename 添加pid
    %u - insert current uid into filename 添加当前uid
    %g - insert current gid into filename 添加当前gid
    %s - insert signal that caused the coredump into the filename 添加导致产生core的信号
    %t - insert UNIX time that the coredump occurred into filename 添加core文件生成时的unix时间
    %h - insert hostname where the coredump happened into filename 添加主机名
    %e - insert coredumping executable name into filename 添加命令名
注意：待调试的可执行文件，在编译的时候需要加-g，core文件才能正常显示出错信息！有时候core信息很大，超出了开发板的空间限制，生成的core信息会残缺不全而无法使用，可以通过挂载到PC的方式来规避这一点。
错误实例：

```C
//1.  访问空指针
#include <stdio.h>
#include <stdlib.h>

void main()
{        
    int *ptr = NULL;       

    *ptr = 0;
}
2.  访问只读内存
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{        
    char *ptr = "123456";   //正确方式：ptr[] = "123456"; 
        ptr[0] = '7';
}




```