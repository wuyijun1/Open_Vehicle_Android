第二季-专题4-我是bootloader设计师 

专题4-我是bootloader设计师
第1课-Bootloader设计蓝图
bootloader的作用
(1)Bootloader; (2)boot parameters; (3)kernel; (4)boot filesystem
以上的四步是嵌入式系统的启动流程，其中的bootloader相当于一个助推器，可以启动linux的内核。
bootloader设计方法-模仿
我们进行模仿，就要模仿行业老大，我们要模仿的是u-boot
u-boot的介绍
u-boot是用于多种嵌入式CPU（MIPS，x86,ARM等）的bootloader程序，u-boot不仅支持嵌入式linux系统的引导，还支持VxWorks，QNX等多种嵌入式操作系统。
u-boot有两种模式，自主模式和开发模式：
当一个产品卖出去后，我们希望的效果就是，系统可以完成自主的启动与运行，这就是我们所说的自主模式；当然我们在内核的开发的时候，我们不希望每一次都要下载新的内核，需要通过网络对内核进行一系列的调试，这就是开发模式。
建立u-boot代码
我们首先安装SourceinSight软件，在Windows环境下安装，安装之后建立新的project，为了方便就把它命名为Uboot，存储的位置自定义，设置如下：
 
         安装好了之后的效果，如下：（这是没有添加代码的情况）
 
         将我们的压缩包u-boot.tar导入linux系统 tar xvzf u-boot.tar.gz，进行文件解压。为了完成Windows下的软件能够访问linux下的文件，将linux下的文件进行虚拟文件映射。
         首先将我们的linux文件的所在的地址复制下来，选择Windows下面的工具，进行映射网络驱动器。如下：
 
 
         在souceinsight界面下的Project选择add and remove project files，在z:盘符下加载所有的文件，勾选包括子文件的选项。
 
         之后会显示导入3800多的文件，导入完成后，可以在软件里面看到这些文件，但是文件里面只包含.c和.h的文件，并没有看到.s和.S的汇编文件，这是因为我们并没有把汇编文件导入里面，在options里面找到document options选项，将.s和.S文件加入里面，设置如下，设置好之后接着扫描z盘，add tree完成对新的文件的添加。
 
         在完成文件的添加后，我们就可以查看文件，随便点开一个主文件，里面有各种的函数调用，为了可以直接查看，函数是如何实现的，可以开启同步设置，设置的位置如下图，在R的左边的第一个就是。当完成同步之后，整个文件的函数都变了颜色，可以点开查看。
 
         完成这一列系的设置，我们完成了该软件的一系列的配置，在开发过程中可以查看软件的如何实现与程序的结构。
 
 
第2课-ARM处理器启动流程
S3c2440-S3c6410-S5pv210对比学习
启动方式
地址布局
启动流程
 
S3c2440：
启动方式
（1）       nor flash，内存：2M
（2）       nand flash，内存：256M
地址布局
S3c2440的nand flash不能直接访问，要通过一系列的寄存器来处理。地址空间0地址处放的是BootSRAM，大小是4k，它的别名是stepping stone，也就是垫脚石。nand flash不参加统一编址，在0地址处安排的是stepping stone。
启动流程
当我们上电，选择从nand flash启动的时候，处理器首先把nand flash前端的4K放在stepping stone，再运行stepping stone的代码。
Bootloader的大小不止4K，剩下的部分需要复制到内存中去，这4K内存的作用不止要进行一系列的初始化作用，还要进行后面完整bootloader的复制操作。
S3c2440的低地址从0x30000000开始
 
 
S3c6410：
启动方式（5种，下面介绍2中）
（1）       SD卡（IROM）
（2）       nand flash（IROM）
（3）       nor flash（支持这种其中方式，但是6410的开发板没有安装）
内存布局
 
         S3c6410的内存0地址处，存储的是镜像，用什么方式启动就是什么的镜像
nand flash启动流程
 
         上电后从iROM率先启动，里面放了一系列的初始化文件，把nand flash最前面的8k的bootloader放在stepping stone中运行，stepping stone再把剩余的bootloader放在SDRAM中。
 
S5pv210：
启动方式
IROM：SD卡、nand falsh
串口、USB
内存空间
地址映射表为：
 
         IROM启动从0xD0000000处开始，垫脚石IRAM从0xD0020000处开始。0地址处依旧采用的是映射的方式，用哪一种方式启动，就将对应的启动方式拷贝到0地址处。
启动流程（nand flash）
 
         上电开始，将IROM被拷贝到内存的0地址处，IROM中的一系列的软件是由厂商固化写好的，可以把它称作BL0（bootloader0），这部分的固件做一系列的初始化；这部分软件将第一部分的BL1导入垫脚石（大小是96Kb）之中；由于垫脚石存储空间大，可以将剩余的BL2也拷贝到垫脚石中。其中BL1的最大空间是16Kb，BL2的最大空间是80Kb，如果BL2大于80Kb，那么第三步就要将BL2拷贝到SDRAM中。
第3课-U-Boot工作流程分析
2440:
1.  程序入口
在start.s的_start处
2.  第一阶段程序分析（BL1）
通过sourceinsight打开start.s对应的芯片，在主makefile界面下，找到2440的配置选项，第三项显示的是smdk2440，进入目录board/Samsung/smdk2440，在里面找到u-boot.elf的连接器文件，在代码段显示cpu/s3c24xx/start.o (.text)，实际在目录找到的是start.s文件。
         在u-boot.elf中显示：
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
     . = 0x00000000;
     . = ALIGN(4);
     .text    :
     {
       cpu/s3c24xx/start.o (.text)
       cpu/s3c24xx/s3c2450/cpu_init.o (.text)
       board/samsung/smdk2450/lowlevel_init.o  (.text)
       cpu/s3c24xx/mmu_table.o   (.text)
       cpu/s3c24xx/onenand_cp.o  (.text)
       cpu/s3c24xx/nand_cp.o (.text)
       cpu/s3c24xx/movi.o (.text)
       *(.text)
       lib_arm/div0.o
     }
说明程序入口在_start处
（1）       设置中断向量表
（2）       设置处理器为svc模式
（3）       刷新ID cache
（4）       关闭MMU和cache
（5）       关闭看门狗
（6）       关闭所有中断
（7）       初始化系统时钟
（8）       初始化串口
（9）       简单初始化nand flash
（10）   进行内存初始化
（11）   复制nand flash中得到bl到内存中
（12）   设置堆栈
（13）   清除bss段
（14）   LED初始化
（15）   跳转到第二阶段入口
3.  第二阶段程序分析（BL2）
（1） 网口初始化
（2） LCD初始化
（3） 解析执行用户命令
 
6410：
1.  程序入口
在start.s的_start处
2.  第一阶段程序分析（BL1）
通过sourceinsight打开start.s对应的芯片
（1）设置中断向量表
（2）设置处理器为svc模式
（3）刷新ID cache
（4）关闭MMU和cache
（5）外设基地址初始化
（6）点亮LED
（7）关闭关门狗
（8）关闭所有中断
（9）初始化系统时钟
（10）初始化串口
（11）简单初始化nand flash
（12）进行内存初始化
（13）复制nand flash中得到bl到内存中
（14）设置堆栈
（15）清除bss段
3.  第二阶段程序分析（BL2）
（1） 网口初始化
（2） LCD初始化
（3） 解析执行用户命令
 
210：
6410：
1.  程序入口
在start.s的_start处
2.  第一阶段程序分析（BL1）
通过sourceinsight打开start.s对应的芯片
（1）设置中断向量表
（2）设置处理器为svc模式
（3）关闭L1 I/D cache
（4）关闭MMU和cache
（5）检查reset状态
（6）恢复io引脚为默认值
（7）关闭关门狗
（8）sram、srom初始化
（9）初始化系统时钟
（10）初始化串口
（11）简单初始化nand flash
（12）取消存储保护区
（13）简单初始化nand flash
（14）disable ABB
（15）设置堆栈
（16）复制nand flash中得到bl到内存中
3.  第二阶段程序分析（BL2）
（1） 网口初始化
（2） LCD初始化
（3） 解析执行用户命令

第4课-Bootloader架构设计（2440）
一. 第一阶段程序分析（BL1） 
1 核心初始化：
1.1 设置中断向量表
1.2 设置处理器为svc模式
1.3关闭看门狗
1.4关闭所有中断
1.5关闭MMU和cache
1.6外设基地址初始化（只6410中使用）
2 C语言编程环境设置：
1.6设置堆栈
1.7清除bss段
3 LED初始化
4 初始化系统时钟
5 进行内存初始化
5.1取消储存保护区（只针对210）
5.2 iram和iram初始化（只针对210）
6 初始化串口
7 复制nand flash中得到bl到内存中
7.1简单的初始化nand flash
7.2复制代码到内存
7.3跳转到第二阶段入口
 
二. 第二阶段程序分析（BL2）
1  MMU初始化
2          中断初始化
2.1中断初始化
2.2按键初始化
3          初始化串口
3.1串口初始化
3.2移植printf函数
4         网卡初始化
5         LCD初始化
5.1触摸板的初始化
5.2 LCD初始化
6  解析执行用户命令
6.1移植tftp命令
6.2 移植bootm命令