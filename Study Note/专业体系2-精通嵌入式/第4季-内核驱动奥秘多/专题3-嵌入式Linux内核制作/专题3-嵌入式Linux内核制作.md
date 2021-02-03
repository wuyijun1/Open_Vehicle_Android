第四季-专题3-嵌入式Linux内核制作 

专题3-嵌入式Linux内核制作
一．linux内核简介
系统架构
（1）Linux系统是怎么构成的？
 
（2）为什么linux系统分为了用户空间和内核空间？
现代CPU通常实现了不同的工作模式，以ARM为例，就实现了7中工作模式：
用户模式（usr），快速中断（fiq），外部中断（irq）、管理模式（svc），数据访问中止（abt），系统模式（sys），未定义指令异常（und）。
X86也实现了4个不同的级别：Ring0-Ring3。Ring0下，可以执行特权指令，可以访问IO设备等，在RIng3则有很多限制。Linux系统利用了CUP的这个特点，使用了其中的两个级别分别运行LInux内核与应用程序，这样是操作系统得到了充分的保护。例如：如果使用X86，用户代码运行在Ring3，内核代码运行在Ring0.
内核空间和用户空间是程序执行额两种不同的状态，通过系统调用硬件中断能够完成从用户空间到内核空间的转移。
（3）linux内核架构
 
System call interface(sci):系统调用接口，我们事先read和open的代码都是在内核中
Procecss management（PM）进程管理模块，进程的管理、删除与调度。
Memory management（MM）内存管理模块，进行内存的管理与回收
Arch：体系结构相关的代码，cup想换的代码
Virtual file system（VFS）虚拟模块管理系统
Network stack 网络协议栈
Device Drivers（DD）设备驱动程序
linux内核源代码
2.1下载地址           www.kernel.org，官方网站
l  Linux内核源代码采用树形节后进行组织，分场合理地把功能相关的文件都放在同一个目录下，使得程序更具有可读性。
l  Arch目录：arch是architecture的缩写。内核所支持的每一种cpu体系，在该目录下都有对应的子目录。每个cpu的子目录，又进一步分解为boot，mm，kernel等子目录，分别包含控制系统引导，内存管理，系统调用等。
l  Documentation：内核的文档
l  Driversm目录：驱动程序
l  Include目录：内核所需要的头文件。与平台无关的头文件在include/linux子目录下，与平台相关的头文件则放在相应的子目录中。
l  Fs目录：文件系统，存放各种文件系统的实现代码。每个子目录对应一种文件系统的实现，公用的源程序用于实现虚拟文件系统vfs
||--devpts /* /dev/pts虚拟文件系统*/
||--ext2 /*第二扩展文件系统*/
||--fat /*MS的fat32文件系统*/
||--isofs /*ISO9660光盘cd-rom上的文件系统*/
l  Net目录：网络协议的实现代码
||--802     /*802无线通讯协议核心支持代码*/
||--apletalk  /*与苹果系统联网的协议*/
||--ax25    /*AX25无线INTENET协议*/
||--bridge   /*桥接设备*/
||--ipv4     /*IP协议族V4版32位寻址模式*/
||--ipv6             /*IP协议族V6版*/
二．Linux内核配置与编译
为什么要配置内核
基于硬件与软件需求，选出需要的，去掉多余的。
Make config：基于文本模式的交互式配置。
Make menuconfig：基于文本模式的菜单型配置。
 
三．嵌入式linux内核制作
 
setenv bootargs noinitrd console=ttySAC0,115200 init=/init root=/dev/nfs rw nfsroot=192.168.153.129:/home/free/part3/rootfs ip=192.168.153.130:192.168.153.129:192.168.153.1:255.255.255.0::eth0:off