第四季-专题9-Linux驱动开发前奏 

专题9-Linux驱动开发前奏
## 第1课-驱动开发概述
驱动分类
常规分类法：字符设备，块设备，网络设备。
（1）       字符设备驱动
字符设备是一种按字节来访问的设备，字符驱动则负责驱动字符设备，这样的
驱动通常实现open, close, read和write 系统调用。例：串口，LED，按键。
（2）   块设备驱动
在大部分的Unix系统中, 块设备定义为：以块(通常是512字节)为最小传输单位的设备，块设备不能按字节处理数据
而Linux则允许块设备传送任意数目的字节。因此,块和字符设备的区别仅仅是驱动的与内核的接口不同。常见的块设备包括硬盘,flash,SD卡……
（3）   网络接口驱动
网络接口可以是一个硬件设备,如网卡；但也可以是一个纯粹的软件设备, 比如回环接口(lo)。一个网络接口负责发送和接收数据报文。
 
总线分类方法：USB设备，PCI设备、平台总线设备
 
剖析LED驱动程序
LED（linux）驱动程序如下：
```C
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <mach/gpio-bank-k.h>
#include "led.h"
#define LEDCON 0x7f008800
#define LEDDAT 0x7f008808
unsigned int *led_config;
unsigned int *led_data;
struct cdev cdev;
dev_t devno;
int led_open(struct inode *node, struct file *filp)
{
    led_config = ioremap(LEDCON,4);
    writel(0x11110000,led_config);  //设置配置寄存器，第一个硬件操作部分
    led_data = ioremap(LEDDAT,4);
    return 0;
}
long led_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
        case LED_ON:                   //设计data寄存器，第二个硬件操作
            writel(0x00,led_data);
            return 0;
                case LED_OFF:
            writel(0xff,led_data);
            return 0;
        default:
            return -EINVAL;
    }
}
static struct file_operations led_fops =
{
    .open = led_open,
    .unlocked_ioctl = led_ioctl,
};
static int led_init()
{
    cdev_init(&cdev,&led_fops);
    alloc_chrdev_region(&devno, 0 , 1 , "myled");
    cdev_add(&cdev, devno, 1);
    return 0;  
}
static void led_exit()
{
    cdev_del(&cdev);
    unregister_chrdev_region(devno,1);
}
module_init(led_init);
module_exit(led_exit);
```
   LED的驱动主要有两个部分，一个是设置GPIO寄存器的状态（输出），另一个是由GPIO的data寄存器控制灯的亮与灭。初始化代码只能放在第一个子函数中，且其中的格式不能变。这是因为在linux系统下写驱动程序都是有固定格式的。
 
l  驱动的学习步骤：
（1）   驱动程序模型
分析范例程序-制作思维导图（总结规律）-自己编写代码-驱动程序框架-复习裸机驱动-将裸机中的硬件操作移植到Linux驱动中，这样就完成了完整的Linux驱动。
（2）   硬件操作实现
（3）   驱动程序测试
注意：
    在驱动学习的初期不要过多的阅读内核代码。
 
## 第2课-硬件访问技术
硬件访问流程
驱动程序控制设备，主要是通过访问设备内的寄存器来达到控制目的,因此我们讨论如何访问硬件，就成了如何访问这些寄存器了。
地址映射
在Linux系统中，无论是内核程序还是应用程序，都只能使用虚拟地址，而芯片手册中给出的硬件寄存器地址或者RAM地址则是物理地址，无法直接使用，因此，我们读写寄存器的第1步就是将接使用，因此，我们将它的物理地址映射为虚拟地址。
（1）   动态映射
所谓动态映射，是指在驱动程序中采用ioremap函数将物理地址映射为虚拟地址。
原型：void * ioremap(physaddr, size)
参数：
Physaddr：待映射的物理地址
Size: 映射的区域长度
返回值：映射后的虚拟地址
（2）   静态映射
所谓静态映射，是指Linux系统根据用户事先指定的映射关系，在内核启动时，自动地将物理地址映射为虚拟地址。
在静态映射中，用户是通过map_desc结构来指明物理地址与虚拟地址的映射关系。
struct map_desc {
unsigned long virtual; /* 映射后的虚拟地址*/
unsigned long pfn; /* 物理地址所在的页帧号*/
unsigned long /* 物理地址所在的页帧号*/
unsigned long length; /* 映射长度*/
unsigned int type; /* 映射的设备类型*/
};
pfn: 利用__phys_to_pfn(物理地址)可以计算出物理地址所在的物理页帧号。
寄存器读写
在完成地址映射后，就可以读写寄存器了，Linux内核提供了一系列函数，来读写寄存器。
unsigned ioread8(void *addr)
unsigned ioread16(void *addr)
unsigned ioread32(void *addr)
unsigned readb(address)
unsigned readw(address)
unsigned address)
unsigned readl(address)
void iowrite8(u8 value, void *addr)
void iowrite16(u16 value, void *addr)
void iowrite32(u32 value, void *addr)
void writeb(unsigned value, address)
void writew(unsigned value, address)
void writel(unsigned value, address)

分类: 嵌入式