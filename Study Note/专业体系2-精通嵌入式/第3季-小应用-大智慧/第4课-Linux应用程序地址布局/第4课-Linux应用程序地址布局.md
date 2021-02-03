第三季-第4课-Linux应用程序地址布局 

第4课-Linux应用程序地址布局
4.1 程序构成
在学习linux应用程序开发时，经常会遇到如下的概念：代码段、数据段、BSS段（Block Started by Symbol，又名：未初始化数据段）、堆（heap）和栈（staack）。而这些部分也是构成Linux应用程序的重要组成部分。
4.2 内存布局
当linux应用程序在内存运行的时候，以上组成部分在内存中又是如何布局的呢？
 
从低地址到高地址分别为：代码段、数据段、BSS段、堆、栈。
堆向高内存地址生长。
栈向低内存地址生长。
其中所有的代码段的地址（linux系统中）的初始位置都是从0x8048000开始的。这是个虚拟地址，它虽然是固定的，但是它映射的具体的地址是不一样的。
4.3 手把手教学
下面列出来我们需要的代码：addr.c
#include<stdio.h>
#include<stdlib.h>
 
int global_init_a=1; //全局的，初始化变量,数据段
int global_uninit_a;//全局的，没有初始化的变量，数据段
static int static_global_init_a=1; //全局的，静态的，初始化的变量，数据段
static int static_global_uninit_a; //全局的，静态的，未初始化的变量，数据段
const int const_global_a=1; //全局的，常量，代码段
 
int global_init_b=1; //全局的，初始化的变量
int global_uninit_b; //全局，未初始化
static int static_global_init_b=1; //全局，静态，初始化
static int static_global_uninit_b; //全局，静态，未初始化
const int const_global_b=1; //全局、常量
 
int main()
{
         int local_init_a=1; //局部，初始化的，栈
         int local_uninit_a; //局部，未初始化，栈
         static int static_local_init_a=1; //局部的，静态，初始化的，数据段
         static int static_local_uninit_a; //局部的，静态，未初始化的，数据段
         const int const_local_a=1; //局部，常量，栈
 
         int local_init_b=1; //局部，初始化的
         int local_uninit_b; //局部，未初始化
         static int static_local_init_b=1; //局部，静态，初始化
         static int static_local_uninit_b; //局部，静态，未初始化
         const int const_local_b=1; //局部，常量
 
         int *malloc_p_a;
         malloc_p_a=malloc(sizeof(int)); //通过malloc分配得到的局部，堆
                  
         printf("&global_init_a=%p,global_init_a=%d\n",&global_init_a,global_init_a);
         printf("&global_uninit_a=%p,global_uninit_a=%d\n",&global_uninit_a,global_uninit_a);
         printf("&static_global_init_a=%p,static_global_init_a=%d\n",&static_global_init_a,static_global_init_a);
         printf("&static_global_uninit_a=%p,static_global_uninit_a=%d\n",&static_global_uninit_a,static_global_uninit_a);
         printf("&const_global_a=%p,const_global_a=%d\n",&const_global_a,const_global_a);
 
         printf("&global_init_b=%p,global_init_b=%d\n",&global_init_b,global_init_b);
         printf("&global_uninit_b=%p,global_uninit_b=%d\n",&global_uninit_b,global_uninit_b);
         printf("&static_global_init_b=%p,static_global_init_b=%d\n",&static_global_init_b,static_global_init_b);
         printf("&static_global_uninit_b=%p,static_global_uninit_b=%d\n",&static_global_uninit_b,static_global_uninit_b);
         printf("&const_global_b=%p,const_global_b=%d\n",&const_global_b,const_global_b);
 
         printf("&local_init_a=%p,local_init_a=%d\n",&local_init_a,local_init_a);
         printf("&local_uninit_a=%p,local_uninit_a=%d\n",&local_uninit_a,local_uninit_a);
         printf("&static_local_init_a=%p,static_local_init_a=%d\n",&static_local_init_a,static_local_init_a);
         printf("&static_local_uninit_a=%p,static_local_uninit_a=%d\n",&static_local_uninit_a,static_local_uninit_a);
         printf("&const_local_a=%p,const_local_a=%d\n",&const_local_a,const_local_a);
 
         printf("&local_init_b=%p,local_init_b=%d\n",&local_init_b,local_init_b);
         printf("&local_uninit_b=%p,local_uninit_b=%d\n",&local_uninit_b,local_uninit_b);
         printf("&static_local_init_b=%p,static_local_init_b=%d\n",&static_local_init_b,static_local_init_b);
         printf("&static_local_uninit_a=%p,static_local_uninit_b=%d\n",&static_local_uninit_b,static_local_uninit_b);
         printf("&const_local_b=%p,const_local_b=%d\n",&const_local_b,const_local_b);
         printf("malloc_p_a=%p,*malloc_p_a=%d\n",malloc_p_a,*malloc_p_a);
         while(1)
         return 0;
}
运行结果：
 
当运行程序./addr时，若想停止下来，就按ctrl+c，就能退出来。
         在另一个终端中，输入命令ps aux，显示系统的进程，找到addr的进程，我们找到的进程号是4213，输入命令cat /proc/4213/maps，显示程序的布局：
 
4.4 数据存放
1.代码段：代码，全局变量（const）、字符串常量。08048000-08049000
2.数据段：全局变量（初始化以及未初始化的）、静态变量（全局的和局部的、初始化以及未初始化的）。08049000-0804a000
3.堆：动态分配的区域,如malloc。0834e000-0836f000
4.栈：局部变量（初始化以及未初始化的，但是不包含静态变量）、局部只读变量（const）。
4.5 BSS段去了哪里？
         利用readelf -S 程序名，分析BSS段
         我们使用file命令分析文件的格式，如下：
显示的是elf格式的32位文件，利用readelf命令可以提取addr的详细信息，如下：