第二季-专题2-我从内部看ARM 

专题2-我从内部看ARM
第一课-家族检阅
一．用的型号
芯片：6410（A11，三星公司）；2440（A9，三星公司）；210（A8，三星公司）
Arm核：ArmA8（v7）；ArmA9（v4指令架构）；Arm11（V6）
指令架构：ArmV6；ArmV7，ArmV4
芯片之中包含arm核，核包含指令架构
二．性能变迁
 
经典：ARM7，ARM9，ARM11
Cortex系列，
M系列，面对工业控制领域，类似单片机但是比单片机强大：M0，M1，M3，M4
R系列,面向实时性的操作：R4
A系列，面对多媒体:A5,A8,A9。
对比：ARM7相当于M3
Arm9，arm11大于R4小于A5，目前确实A9的最强大。
三．对比芯片
 
2440（Arm9）
6410（Arm11）
210（A8）
处理速度
400-500
533-667
800-1024
缓存
16k
16k
32k
内存接口
SDARM（快没了）
DDR，ADRAM
DDR1，DDR2
支持的os
Wince，linux
Wince linux，androd
Wince linux，androd 
其他
停产
正常生产
正常生产
指令架构
ArmV4
ArmV6
ArmV7
 
 
 
 
 
 
第2课-arm处理器工作模式
         学习ARM的知识，最全面的书籍就是Arm公司的手册ARM Architecture Reference Manual，能全面的介绍与Arm相关的知识。
 
Arm有7种工作模式，见下表：
Processor mode
Mode number
Description
User（一般）
user
0b0000
Normal program execution mode
FIQ（快速中断）
fiq
0b0001
Supports a high-speed data transfer or channel p[rocess
IRQ（普通中断）
irq
0b0002
Used for general-purpose interrupt handling
Supervisor（保护，权限较高）
svc
0b0003
A protected mode for the operating system
Abort
abt
0b0004
Implements virtual memory and/or hardware coprocessors
Undefined（未定义指令模式）
und
0b0005
Supports software emulation of g\hardware comprocessors
System
sys
0b0006
Runs priviledged operating system tasks(ARMv4 and above)
在不同的工作模式下，指令与寄存器的使用都是不同的，Linux主要运行在user(用户态程序)和svc(内核运行在该模式) 。
 
 
第三课-arm寄存器详解
modes
 
Privileged modes
 
 
Exception modes
User
System
Supervisor 
Abort 
Undefined
Interrupt
Fast interrupt
R0
R0
R0
R0
R0
R0
R0
R1
R1
R1
R1
R1
R1
R1
R2
R2
R2
R2
R2
R2
R2
R3
R3
R3
R3
R3
R3
R3
R4
R4
R4
R4
R4
R4
R4
R5
R5
R5
R5
R5
R5
R5
R6
R6
R6
R6
R6
R6
R6
R7
R7
R7
R7
R7
R7
R7
R8
R8
R8
R8
R8
R8
R8_fiq
R9
R9
R9
R9
R9
R9
R9_fiq
R10
R10
R10
R10
R10
R10
R10_fiq
R11
R11
R11
R11
R11
R11
R11_fiq
R12
R12
R12
R12
R12
R12
R12_fiq
R13
R13
R13_svc
R13_abt
R13_und
R13_irq
R13_fiq
R14
R14
R14_svc
R14_abt
R14_und
R14_irq
R14_fiq
Pc(指针)R15
pc
pc
pc
pc
pc
pc
CPSR
CPSR
CPSR
CPSR
CPSR
CPSR
CPSR
 
 
SPSR_svc
SPSR_abt
SPSR_und
SPSR_irq
SPSR_fiq
 
一．Arm有37个寄存器，31个通用寄存器，还有6个程序状态寄存器
通用寄存器：R0-R7不分组寄存器，R8-R14(不同工作模式下使用的名字不同、寄存器不同)分组寄存器，R15是PC指针，分成三组。
R13-R15十分特殊，R13当做pc的堆栈指针；R14是链接寄存器，保存函数的返回地址，当发生异常的时候返回地址；R15被称为程序计数器，也就是pc指针。
程序状态寄存器：CPSR、SPSR。SPSR只有在Excrption mode下才存在。当有中断发生的时候，程序寄存器CPSR发生改变，为了使得中断结束后CPSR的状态能保持之前没中断之前的状态，把它的值保存在同样是程序状态寄存器的SPSR中。当中断结束的时候，SPSR将其中保存的值再传给CPSR，使得程序保持之前的运行状态。
二．书写格式
程序状态寄存器的格式如下：
 
对于程序状态寄存器中所有的量，并不需要我们都了解，但是需要了解标记为黄色的量的意义。
 
N表示当两个数进行减法操作，结果为正数是1，结果为负数是0；
Z表示当两个数进行减法操作，结果为0是1，其他结果是0；
 
I表示，当它的值是1的时候，不能进行中断；
F表示，当它的值是1的时候，不能进行快速中断，快速中断比中断级别高；
         M能表明处理器的7中工作模式，具体如上面的图。
第四课-arm的寻址方式
一．寻址方式
所谓的寻址方式就是处理器根据指令中给出的信息来找到指令所需要的操作数的方式。
二．主要的寻址方式
立即数寻址
立即数寻址，是一种特殊的寻址方式，操作数本身就在给定的指令中，只要取出指令也就找到了操作数。这个操作数被称为立即数，对应的寻址方式就是立即数寻址。
ADD R0，R0，#0x3f；R0R0+0x3f
在以上的两条指令中，第二个源操作数就是立即数，要求以“#”为前缀。
寄存器寻址
寄存器寻址就是利用寄存器中的数值作为操作数，这种寻址方式是一种执行效率较高的寻址方式。
ADD R0，R1，R2；R0R1+R2
该指令的执行效果是将寄存器R1和R2的内容相加，结果放在寄存器R0中。
寄存器间接寻址
寄存器间接寻址就是寄存器中存放的是操作数在内存中的地址。立即数寻址，数就在操作指令中；寄存器寻址的数在寄存器中；寄存器间接寻址的数在内存中。
LDR R0，R1, [R2]; R0[R2]
在第一条指令中，以寄存器R2的值作为操作数的地址，在存储器中取得一个操作数后与R1相加，结果存入寄存器R0中。
基址变址寻址
基址变址寻址就是将寄存器里的内容（基地址）与指令中给出的地址偏移量相加，从而得到操作数在内存中的地址：
LDR R0，[R1, #4]; R0[R1+4]
相对寻址
与基址变址寻址方式类似，相对寻址PC指针的当前值为基地址，指令中的地址符号为偏移量，将两者相加之后得到的操作数的有效地址。以下程序段为完成子程序的调用和返回，跳转指令BL采用了相对寻址方式：
BL  NEXT；跳转到程序NEXT处执行
 
NEXT
 
MOV PC，LR；从子程序返回