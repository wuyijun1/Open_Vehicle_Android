第31课 - 程序的内存布局

 

代码在可执行程序中有如下的对应关系
 

有初始值的放在data段，没有初始的放在bss段。静态存储区就对应了这两个区域。

我们写的函数，可移执行的代码段，放在text段。

这里并不见堆和栈的踪影，因为这只是我们的可执行文件的布局，并不会我们执行起来，进程的布局，所以是看不到堆和栈的。

文件布局在内存中的映射


|   |   | 高地址内存 |
| :-----| ----: | :----: |
| File header |   | 栈 |
| .text |   | 堆 |
| .data |   | .bss |
| .data |   | .data |
| .bss |   | .text |
| .bss |   | 未映射区域 |
| a.out |   | a.out进程的地址空间 |


当文件a.out运行的时候，在我们的内存空间中就出现了右侧的地址空间。由上到下分别是栈、堆（空闲列表法）、.bss、.data等等。我们的静态存储区是和我们的栈和堆是不同的，它不仅在程序运行的时候分配空间，而且会把空间中的内容最终写到我们的可执行文件中去。所以`.bss（存的未初始化的数据）、.data（存的初始化的数据）、.text（我们编写的程序代码）`。未映射区域存的内容（有自己独特的用处）不是我们这里介绍的，是在操作系统中该去介绍的。

各个段的作用
l `堆栈段`是在程序运行后才正式存在，是程序运行的基础。C语言的执行是从main函数开始的，而这个函数的开始就是要有一些相应的活动记录，这是需要堆栈的。

l` .bss段`存放的是未初始化的全局变量和静态变量。没有初始值的文件，默认0。当映射这个区域的时候，直接将其中的内容都初始为0就好。

l` .text`段存放的是程序中的可执行代码。

l` .data`段保存的是那些已经初始化了的全局变量和静态变量。应将初始化了的值，当映射的时候，里面的值是要保存的。所以它和.bss段要分开保存。

l `.rodata`段存放的是程序中的常量值，如字符串常量。为了理解这个.rodata的含义，我们举一个例子：
```C 
#include<stdio.h>

int main()

{

char* p = "hello world\n";

printf(p);

}

运行结果：hello world

我们若做更改:

#include<stdio.h>

int main()

{

char* p = "hello world\n";

p[0] = 'H';

printf(p);

}
```
运行出错，若是在linux系统下，就会显示段错误。

实际上我们的hello world就是存放在.rodata段，这个数据段的内容实际和.data段是并列的，但是这个段的数据是只读的。所以内容不能更改。

 

程序术语对应关系
l `静态存储区通常指程序中.bss和.data段`

l `只读区通常指程序中的.rodata段`

l `局部变量所占空间为栈上的空间`

l `动态空间为堆上的空间`

l `程序可执行代码存放在.text段`

问：函数的地址对应程序的哪个段？

函数的地址就是程序的进程空里面存放代码段的某个地址。我们下面分析一下，我们知道函数的代码存放在.text段上，但是函数执行的数据又在活动记录（栈）上，这有没有问题呢？当然没问题，函数的那些代码最后是会被翻译成汇编代码的，汇编代码操作的是内存。就是只要有我们的栈顶指针esp，我们在操作的过程中，就会找到我们的所有的数据变量。即使存在字节对齐问题也是没有问题的。也就是我们能通过指针读出，活动记录里面的所有int、char等数据类型来给我们的函数使用。并且我们在在函数体内也可也去作.bss、.data等。