第三季-第3课-Coredump程序故障分析 
第三季-第3课-Coredump程序故障分析 

问：如果一个程序运行3天后才会出错，这个时候难道需要我们一直用GDB调试程序3天吗？
答：我们需要更加厉害的武器——Core dump
3.1 core dump的定义
Core dump又叫做核心转存。当程序在运行过程中发生异常，这时linux系统可以把程序出错时的内存内容存储在一个core文件中，这个过程叫做Core dump。
3.2 segment fault
Linux应用程序在运行的过程中，经常会遇到segment fault（段错误）这样的错误。产生的原因通常有一下几种：
l  数组访问越界
l  访问空指针
l  栈溢出
l  修改只读内存
3.3 core dump使能
在linux系统中，默认是关闭core dump功能的，但是可以通过来ulimit命令打开/关闭core dump功能。
打开：ulimit -c unlimited
关闭：ulimit -c 0
3.4 core文件分析
发生core dump之后，可以使用gdb进行查看core文件的内容，以定位程序出错的位置。
用法：gdb程序名core文件名
例如：gdb ./test core.12525，必须是./test的形式
3.4 错误分析 
在我们用gcc命令去生成文件的时候，我们需要加上-g这一个指令。因为有这个指令的存在我们才能看到文件的详细信息。在我们执行./seg1时候生成的core文件能将具体的问题加以体现。