第四季-专题21-USB驱动程序设计 

专题21-USB驱动程序设计
第1课-USB总线介绍
USB发展史
USB(Universal Serial Bus )，通用串行总线，是一种外部总线标准，用于规范电脑与外部设备的连接和通讯。USB是在1994年底由英特尔、康柏、IBM、Microsoft等多家公司联合提出的，自1996年推出后，已成功替代串口和并口，成为当今个人电脑和大量智能设备的必配接口之一。
USB 1.0出现在1996年的，速度只有1.5Mb/s1998年升级为USB 1.1，速度也提升到12Mb/s，称之为”full speed”
USB2.0规范是由USB1.1规范演变而来的。它的传输速率达到了480Mbps，称之为”high speed”
USB3.0提供了十倍于USB 2.0的传输速度和更高的节能效率，被称为”super speed”
USB硬件接口
（1）     标准A口
（2）     标准B口
（3）     mini-usb口
（4）     micro-usb口
 
 
（5）     USB信号线
USB接口有4根线，两根电源线，两根信号线。USB接口的输出电压和电流是：
+5V 500mA
l  USB数据线：（正）DATA+、USBD+、PD+、USBDT+
l  USB数据线：（负）DATA-、USBD-、PD-、USBDT-
l  地线： GND、Ground
 
USB系统拓扑结构
 
对于每个USB系统来说，都有一个称为主机控制器的设备，该控制器和一个根Hub作为一个整体。这个根Hub下可以接多级的Hub，每个子Hub又可以接子Hub。每个作为一个节点接在不同级别的Hub上。每条USB总线上最多可以接127个设备。
常见的USB主控制器规格有：
l  OHCI：主要是非PC系统上的USB芯片
l  UHCI：大多是Intel和Via主板上的USB控制器芯片。他们都是由USB1.1规格的。
l  EHCI是有Intel等几个厂商研发，兼容OHCI，UHCI ，遵循USB2.0规范。
 
 
 
 
 
 
第2课-USB协议分析
USB设备逻辑结构
（1）     逻辑组织结构
在USB设备的逻辑组织中，包含设备、配置、接口和端点4个层次。设备通常有一个或多个配置,配置通常有一个或多个接口,接口有零或多个端点。
 
       每个USB设备都可以包含一个或多个配置，不同的配置使设备表现出不同的功能组合，配置由多个接口组成。在USB协议中，接口代表一个基本的功能，一个功能复杂的USB设备可以具有多个接口,而接口是端点的汇集
一个USB播放器带有音频,视频功能,还有旋钮和按钮。
配置1: 音频(接口)+旋钮(接口)
配置2: 视频(接口)+旋钮(接口)
配置3: 音频(接口)+视频(接口)+按钮(接口)
音频接口,视频接口,按钮接口,旋钮接口均需要一个驱动程序。
USB设备中的唯一可寻址的部分是设备端点，端点的作用类似于寄存器。每个端点在设备内部有唯一的端点号，这个端点号是在设备设计时给定的。主机和设备的通信最终都作用于设备上的各个端点。每个端点所支持的操作都是单向的，要么只读，要么只写。
（2）     设备描述符
当我们把USB设备（如：USB鼠标）插到我们的PC时，主机能够自动识别出我们的USB设备类型。
在每一个USB设备内部，包含了固定格式的数据，通过这些数据，USB主机就可以获取USB设备的类型、生产厂商等信息。这些固定格式的数据，我们就称之为USB描述符。标准的USB设备有5种USB描述符：设备描述符，配置描述符，接口描述符，端点描述符，字符串描述符。
       一个USB设备只有一个设备描述符，设备描述符长度为18个字节，格式如左图。
 
设备描述符说明:
bLength : 描述符长度，固定为0x12。
bDescriptorType : 设备描述符类型，固定为0x01。
bcdUSB : USB 规范发布号。表示了本设备能适用于那种协议，如2.0=0200
bDeviceClass : 类型代码。
bDeviceSubClass : 子类型代码。
bDeviceProtocol : 协议代码。
bMaxPacketSize0 : 端点０最大分组大小。
idVendor : 供应商ID。
idProduct : 产品ID（由厂商分配）。
bcdDevice : 设备出产编码，由厂家自行设置。
iManufacturer : 厂商描述符字符串索引．索引到对应的字符串描述符。
iProduct : :产品描述符字符串索引。
iSerialNumber : 设备序列号字符串索引。
bNumConfigurations : 可能的配置数。
（3）     配置描述符
USB配置描述符长度为8个字节，格式如下图
 
 
配置描述符说明：
       bLength : 描述符长度，固定为0x09。
bDescriptorType : 配置描述符类型，固定为0x02。
wTotalLength : 返回整个数据的长度，指此配置返回的配置描述符，接口描述符以及端点描述符的全部大小。
bNumInterfaces : 配置所支持的接口数，指该配置配备的接口数量，也表示该配置下接口描述符数量。
bConfigurationValue : 作为Set Configuration的一个参数选择配置值。
iConfiguration : 用于描述该配置字符串描述符的索引。
bmAttributes : 供电模式选择。Bit4-0保留，D7:总线供电，D6:自供电，D5:远程唤醒．
MaxPower : 总线供电的USB设备的最大消耗电流，以2mA为单位。
（4）     接口描述符
USB接口描述符长度为8个字节
 
 
接口描述符说明：
bLength : 描述符长度，固定为0x09。
bDescriptorType : 接口描述符类型，固定为0x04。
bInterfaceNumber: 该接口的编号。
bAlternateSetting : 用于为上一个字段选择可供替换的设置。
bNumEndpoint : 使用的端点数目，端点０除外。
bInterfaceClass : 类型代码（由USB组织分配）。
bInterfaceSunClass : 子类型代码（由USB组织分配）。
bInterfaceProtocol : 协议代码（由USB组织分配）。
iInterface : 字符串描述符的索引。
（5）     端点描述符
USB端点描述符长度为7个字节。
 
 
 
bLength : 描述符大小，固定为0x07。
bDescriptorType : 接口描述符类型，固定为0x05。
bEndpointType : USB设备的端点地址。Bit7，方向，对于控制端点可以忽略，1/0:IN/OUT。Bit6-4，保留。BIt3-0：端点号．
bmAttributes : 端点属性，Bit7-2，保留。BIt1-0：00控制，01同步，02批量，03中断。
wMaxPacketSize : 本端点接收或发送的最大信息包大小。
bInterval : 轮训数据传送端点的时间间隔．对于批量传送和控制传送的端点忽略．对于同步传送的端点，必须为１，对于中断传送的端点，范围为1-255。
USB数据通信
（1）     通讯模型
 
（2）     传输
USB的数据传输通讯首先是基于传输（Transfer）的，传输的类型有：中断传输、批量传输、同步传输、控制传输
（3）     事务
一次传输由一个或多个事务（transaction）构成，事务可以分为：In事务，Out事务，Setup事务。
（4）     包
一个事务由一个或多个包（packet）构成，包可以分为：令牌包（setup）、数据包（data）、握手包（ACK）和特殊包。
（5）     域
一个包由多个域构成,域可分为：同步域（SYNC），标识域（PID），地址域（ADDR），端点域（ENDP），帧号域（FRAM），数据域（DATA），校验域（CRC）。
USB设备枚举
USB设备在正常工作以前, 第一件要做的事就是枚举。枚举是让主机认得这个USB设备, 并且为该设备准备资源，建立好主机和设备之间的数据传递通道。
步骤：
（1）获取设备描述符
（2）复位
（3）设置地址
（4）再次获取设备描述符
（5）获取配置描述符
（6）获取接口、端点描述符
（7）获取字符串描述符
（8）选择设备配置
 
第3课-LinuxUSB系统架构
软件系统架构
 
USB-MassStorage驱动体验
Device Drivers -> USB support ->support for host…
 
 
       General setup --->
[*] Configure standard kernel features (for small systems) --->
[*] Support for hot-pluggable devices (NEW)
Device Drivers --->
Block devices --->
<*> Low Performance USB Block driver
 
Device Drivers --->
SCSI device support --->
<*> SCSI device support
<*> SCSI disk support
<*> SCSI generic support
 
File systems --->
DOS/FAT/NT Filesystems --->
<*> MSDOS fs support
<*> VFAT (Windows-95) fs support
(936) Default codepage for FAT
(cp936) Default iocharset for FAT
Partition Types --->
[*] PC BIOS (MSDOS partition tables) support
Native Language Support --->
<*> Simplified Chinese charset (CP936, GB2312)
<*> NLS UTF8
 
USB-RNDIS驱动体验
Device Drivers ->USB support ->USB gadget support
 
 
       使用RNDIS
 
 
第4课-LinuxUSB设备驱动设计
USB驱动模型
 
USB设备包括配置(configuration)、接口（interface）和端点(endpoint)，一个USB设备驱动程序对应一个USB接口，而非整个USB设备。
在Linux内核中，使用struct usb_driver结构描述一个USB驱动。
struct usb_driver {
const char *name; /*驱动程序名*/
/* 当USB核心发现了该驱动能够处理的USB接口时，调用该函数*/
int (*probe) (struct usb_interface *intf, const struct usb_device_id *id);
/* 当相应的USB接口被移除时，调用该函数*/
void (*disconnect) (struct usb_interface *intf);
/* USB驱动能够处理的设备列表*/
const struct usb_device_id *id_table;
}
URB
（1）     URB通讯模型
 
USB请求块（USB request block-URB）是
USB设备驱动中用来与USB设备通信所用的基本载体和核心数据结构，非常类似于网络
设备驱动中的sk_buff结构体，是USB主机与设备通信的“电波”。
步骤：
l  USB 设备驱动程序创建并初始化一个访问特定端点的urb，并提交给USB core；
l  USB core提交该urb到USB主控制器驱动程序；
l  USB 主控制器驱动程序根据该urb描述的信息，来访问USB设备；
l  当设备访问结束后，USB 主控制器驱动程序通知USB 设备驱动程序。
（2）     创建URB
struct urb *usb_alloc_urb(int iso_packets, gfp_t mem_flags)
参数：
iso_packets：urb所包含的等时数据包的个数。
mem_flags：内存分配标识(如GFP_KERNEL)，参考kmalloc。
（3）     初始化URB
对于中断urb，使用usb_fill_int_urb函数来初始化
对于批量urb，使用usb_fill_bulk_urb函数来初始化
对于控制urb，使用usb_fill_control_urb函数来初始化
对于等时urb，只能手动地初始化urb。
static inline void usb_fill_int_urb(
struct urb *urb, //待初始化的urb
struct usb_device *dev, //urb所要访问的设备
unsigned int pipe, //要访问的端点所对应的管道，
void *transfer_buffer, //保存传输数据的buffer
int buffer_length, //buffer长度
usb_complete_t complete_fn, //urb完成时调用的函数
void *context, //赋值到urb->context的数据
int interval) //urb被调度的时间间隔
（4）     提交URB
在完成urb的创建和初始化后，USB驱动需要将urb提交给USB核心.
int usb_submit_urb(struct urb *urb, gfp_t mem_flags)
参数：
urb：要提交urb的指针
mem_flags: 内存分配标识(如GFP_KERNEL)，参考kmalloc
URB被提交到USB核心后，USB核心指定usb主控制器驱动程序来处理该urb，处理完之后，urb完成函数将被调用。
HID协议
HID(Human Interface Device)，属于人机交互类的设备，如USB鼠标，USB键盘，USB游戏操纵杆等。该类设备必须遵循HID设计规范。
鼠标驱动分析
 
/*
 *  Copyright (c) 1999-2001 Vojtech Pavlik
 *
 *  USB HIDBP Mouse support
 */
 
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Should you need to contact me, the author, you can do so either by
 * e-mail - mail your message to <vojtech@ucw.cz>, or by paper mail:
 * Vojtech Pavlik, Simunkova 1594, Prague 8, 182 00 Czech Republic
 */
 
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb/input.h>
#include <linux/hid.h>
 
/* for apple IDs */
#ifdef CONFIG_USB_HID_MODULE
#include "../hid-ids.h"
#endif
 
/*
 * Version Information
 */
#define DRIVER_VERSION "v1.6"
#define DRIVER_AUTHOR "Vojtech Pavlik <vojtech@ucw.cz>"
#define DRIVER_DESC "USB HID Boot Protocol mouse driver"
#define DRIVER_LICENSE "GPL"
 
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE(DRIVER_LICENSE);
 
struct usb_mouse {
    char name[128];
    char phys[64];
    struct usb_device *usbdev;
    struct input_dev *dev;
    struct urb *irq;
 
    signed char *data;
    dma_addr_t data_dma;
};
 
static void usb_mouse_irq(struct urb *urb)
{
    struct usb_mouse *mouse = urb->context;
    signed char *data = mouse->data;
    struct input_dev *dev = mouse->dev;
    int status;
 
    /* 检测urb传输是否成功 */
    switch (urb->status) {
    case 0:         /* success */
        break;
    case -ECONNRESET: /* unlink */
    case -ENOENT:
    case -ESHUTDOWN:
        return;
    /* -EPIPE:  should clear the halt */
    default:    /* error */
        goto resubmit;
    }
 
    /* 报告按键状态 */
    input_report_key(dev, BTN_LEFT,   data[0] & 0x01);
    input_report_key(dev, BTN_RIGHT,  data[0] & 0x02);
    input_report_key(dev, BTN_MIDDLE, data[0] & 0x04);
    input_report_key(dev, BTN_SIDE,   data[0] & 0x08);
    input_report_key(dev, BTN_EXTRA,  data[0] & 0x10);
 
    input_report_rel(dev, REL_X,     data[1]);
    input_report_rel(dev, REL_Y,     data[2]);
    input_report_rel(dev, REL_WHEEL, data[3]);
 
    input_sync(dev);
resubmit:
    /* 提交下次传输 */
    status = usb_submit_urb (urb, GFP_ATOMIC);
    if (status)
        err ("can't resubmit intr, %s-%s/input0, status %d",
                mouse->usbdev->bus->bus_name,
                mouse->usbdev->devpath, status);
}
 
static int usb_mouse_open(struct input_dev *dev)
{
    struct usb_mouse *mouse = input_get_drvdata(dev);
 
    mouse->irq->dev = mouse->usbdev;
    if (usb_submit_urb(mouse->irq, GFP_KERNEL))
        return -EIO;
 
    return 0;
}
 
static void usb_mouse_close(struct input_dev *dev)
{
    struct usb_mouse *mouse = input_get_drvdata(dev);
 
    usb_kill_urb(mouse->irq);
}
 
static int usb_mouse_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
    /* 设备描述 usb_device */
    /* 接口描述 usb_interface */
    struct usb_device *dev = interface_to_usbdev(intf);
   
    /* 接口设置描述 */
    struct usb_host_interface *interface;
   
    /* 端点描述符 */
    struct usb_endpoint_descriptor *endpoint;
       
    struct usb_mouse *mouse;
    struct input_dev *input_dev;
    int pipe, maxp;
    int error = -ENOMEM;
 
    /* 获取当前接口设置 */
    interface = intf->cur_altsetting;
 
　　   /* 根据HID规范，鼠标只有一个端点（不包含0号控制端点）*/
    if (interface->desc.bNumEndpoints != 1)
        return -ENODEV;
 
    /* 获取端点0描述符 */
    endpoint = &interface->endpoint[0].desc;
   
    /* 根据HID规范，鼠标唯一的端点应为中断端点 */
    if (!usb_endpoint_is_int_in(endpoint))
        return -ENODEV;
 
    /* 生成中断管道 */
    pipe = usb_rcvintpipe(dev, endpoint->bEndpointAddress);
   
    /* 返回该端点能够传输的最大的包长度，鼠标的返回的最大数据包为4个字节。*/ 
    maxp = usb_maxpacket(dev, pipe, usb_pipeout(pipe));
 
    /* 创建input设备 */
    mouse = kzalloc(sizeof(struct usb_mouse), GFP_KERNEL);
    input_dev = input_allocate_device();
    if (!mouse || !input_dev)
        goto fail1;
 
   /* 申请内存空间用于数据传输，data 为指向该空间的地址*/
    mouse->data = usb_buffer_alloc(dev, 8, GFP_ATOMIC, &mouse->data_dma);
    if (!mouse->data)
        goto fail1;
   
    /* 分配URB */
    mouse->irq = usb_alloc_urb(0, GFP_KERNEL);
    if (!mouse->irq)
        goto fail2;
 
    mouse->usbdev = dev;
    mouse->dev = input_dev;
 
    if (dev->manufacturer)
        strlcpy(mouse->name, dev->manufacturer, sizeof(mouse->name));
 
    if (dev->product) {
        if (dev->manufacturer)
            strlcat(mouse->name, " ", sizeof(mouse->name));
        strlcat(mouse->name, dev->product, sizeof(mouse->name));
    }
 
    if (!strlen(mouse->name))
        snprintf(mouse->name, sizeof(mouse->name),
             "USB HIDBP Mouse %04x:%04x",
             le16_to_cpu(dev->descriptor.idVendor),
             le16_to_cpu(dev->descriptor.idProduct));
 
    /* usb_make_path 用来获取 USB 设备在 Sysfs 中的路径*/
    usb_make_path(dev, mouse->phys, sizeof(mouse->phys));
   
    strlcat(mouse->phys, "/input0", sizeof(mouse->phys));
 
    /* 字符设备初始化 */
    input_dev->name = mouse->name;
    input_dev->phys = mouse->phys;
    usb_to_input_id(dev, &input_dev->id);
    input_dev->dev.parent = &intf->dev;
 
    input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
    input_dev->keybit[BIT_WORD(BTN_MOUSE)] = BIT_MASK(BTN_LEFT) |
        BIT_MASK(BTN_RIGHT) | BIT_MASK(BTN_MIDDLE);
    input_dev->relbit[0] = BIT_MASK(REL_X) | BIT_MASK(REL_Y);
    input_dev->keybit[BIT_WORD(BTN_MOUSE)] |= BIT_MASK(BTN_SIDE) |
        BIT_MASK(BTN_EXTRA);
    input_dev->relbit[0] |= BIT_MASK(REL_WHEEL);
 
    input_set_drvdata(input_dev, mouse);
 
    input_dev->open = usb_mouse_open;
    input_dev->close = usb_mouse_close;
 
    /* 初始化中断URB */
    /* 思考实验：将interval参数设置为1分钟，观察现象 */
    usb_fill_int_urb(mouse->irq, dev, pipe, mouse->data,
             (maxp > 8 ? 8 : maxp),
             usb_mouse_irq, mouse, endpoint->bInterval);
    mouse->irq->transfer_dma = mouse->data_dma;
    mouse->irq->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
 
    error = input_register_device(mouse->dev);
    if (error)
        goto fail3;
 
    /*将mouse指针保存到intf的dev成员中*/
    usb_set_intfdata(intf, mouse);
    return 0;
 
fail3: 
    usb_free_urb(mouse->irq);
fail2: 
    usb_buffer_free(dev, 8, mouse->data, mouse->data_dma);
fail1: 
    input_free_device(input_dev);
    kfree(mouse);
    return error;
}
 
static void usb_mouse_disconnect(struct usb_interface *intf)
{
    struct usb_mouse *mouse = usb_get_intfdata (intf);
 
    usb_set_intfdata(intf, NULL);
    if (mouse) {
        usb_kill_urb(mouse->irq);
        input_unregister_device(mouse->dev);
        usb_free_urb(mouse->irq);
        usb_buffer_free(interface_to_usbdev(intf), 8, mouse->data, mouse->data_dma);
        kfree(mouse);
    }
}
 
static struct usb_device_id usb_mouse_id_table [] = {
    { USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
        USB_INTERFACE_PROTOCOL_MOUSE) },
    { } /* Terminating entry */
};
 
MODULE_DEVICE_TABLE (usb, usb_mouse_id_table);
 
static struct usb_driver usb_mouse_driver = {
    .name       = "usbmouse",   /* 驱动名 */
    .probe      = usb_mouse_probe, /* 捕获函数 */
    .disconnect  = usb_mouse_disconnect, /* 卸载函数 */
    .id_table   = usb_mouse_id_table, /* 设备列表 */
};
 
static int __init usb_mouse_init(void)
{
    /* 注册鼠标驱动程序 */
    int retval = usb_register(&usb_mouse_driver);
    if (retval == 0)
        printk(KERN_INFO KBUILD_MODNAME ": " DRIVER_VERSION ":"
                DRIVER_DESC "\n");
    return retval;
}
 
static void __exit usb_mouse_exit(void)
{
    usb_deregister(&usb_mouse_driver);
}
 
module_init(usb_mouse_init);
module_exit(usb_mouse_exit);
 
第5课-USB下载线驱动设计
dnw.c
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
 
const char* dev = "/dev/dnw0";
 
 
int main(int argc, char* argv[])
{
    unsigned char* file_buffer = NULL;
   
    long int addr  =  0;
   
    if( 3 != argc )  {
        printf("Usage: dwn <filename> address\n");
        return 1;
    }
   
 
    int fd = open(argv[1], O_RDONLY);
    if(-1 == fd) {
        printf("Can not open file - %s\n", argv[1]);
        return 1;
    }
    addr = strtol((char *) argv[2] ,NULL, 16);
   
    printf("addr = %x \n", addr);
   
 
    // get file size
    struct stat file_stat;
    if( -1 == fstat(fd, &file_stat) ) {
        printf("Get file size filed!\n");
        return 1;
    }  
 
    file_buffer = (unsigned char*)malloc(file_stat.st_size+10);
    if(NULL == file_buffer) {
        printf("malloc failed!\n");
        goto error;
    }
    //memset(file_buffer, '\0', sizeof(file_buffer)); // bad code ! corrected by Qulory
    memset(file_buffer, '\0', sizeof(char)*(file_stat.st_size+10));
 
    // the first 8 bytes in the file_buffer is reserved, the last 2 bytes also;
    if( file_stat.st_size !=  read(fd, file_buffer+8, file_stat.st_size)) {
        printf("Read file failed!\n");
        goto error;
    }
 
    printf("File name : %s\n", argv[1]);
    printf("File size : %ld bytes\n", file_stat.st_size);// off_t is long int
 
    int fd_dev = open(dev, O_WRONLY);
    if( -1 == fd_dev) {
        printf("Can not open %s\n", dev);
        goto error;
    }
 
    /*
     * Note: the first 4 bytes store the dest addr ;
     * the following 4 bytes store the file size ;
     * and the last 2 bytes store the sum of each bytes of the file ;
     */
    *((unsigned long*)file_buffer) = addr; //load address
    *((unsigned long*)file_buffer+1) = file_stat.st_size+10; //file size
    unsigned short sum = 0;
    int i;
    for(i=8; i<file_stat.st_size+8; i++)   {
        sum += file_buffer[i];
    }
 
    *((unsigned short*)(file_buffer+8+file_stat.st_size)) = sum;
 
    printf("Start Sending data...\n");
    size_t remain_size = file_stat.st_size+10;
    size_t block_size = 512;
    size_t written = 0;
    while(remain_size > 0) {
        size_t to_write = remain_size > block_size ? block_size:remain_size;
        size_t real_write = write(fd_dev, file_buffer+written, to_write);
        if( to_write != real_write)   {
            printf(" write  /dev/secbulk0 failed!  to_write = %u real_write = %u \n" , to_write ,real_write );
            return 1;
        }
        remain_size -= to_write;
        written += to_write;
        printf("\rSent %lu%% \t %u bytes !", written*100/(file_stat.st_size+10),  written);
        fflush(stdout);
 
    }  
 
    printf("OK\n");
    return 0;
 
error:
    if(-1 != fd_dev) {
        close(fd_dev);
    }
    if(fd != -1)  {
        close(fd);
    }
    if( NULL != file_buffer ) {
        free(file_buffer);
    }
    return -1;
}
 
usb_dnw.c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
 
#define BULKOUT_BUFFER_SIZE 512
char *bulkout_buffer;
struct usb_device *udev;
__u8 bulk_out_endaddr;
 
static struct usb_device_id dnw_id_table [] = {
    { USB_DEVICE(0x5345, 0x1234) },
    { }    
};
 
static int dnw_open(struct inode* inode, struct file *file)
{
     bulkout_buffer = kmalloc(BULKOUT_BUFFER_SIZE,GFP_KERNEL);
     return 0;
}
 
static int dnw_release (struct inode* inode, struct file *file)
{
    kfree(bulkout_buffer);
    return 0;
}
 
static ssize_t dnw_write(struct file *file, const char __user *buf, size_t len, loff_t *pos)
{
    size_t to_write;
    size_t total_write = 0;
    size_t act_len;
   
    while(len>0)
    {
    to_write = min(len,(size_t)BULKOUT_BUFFER_SIZE);
       
    copy_from_user(bulkout_buffer,buf+total_write,to_write);
   
    usb_bulk_msg(udev,usb_sndbulkpipe(udev,bulk_out_endaddr),bulkout_buffer,to_write,&act_len,3*HZ);
   
    len -= to_write;
    total_write += to_write;
    }
   
    return total_write;
}
 
static struct file_operations dnw_ops =
{
    .owner = THIS_MODULE,
    .write = dnw_write,
    .open =     dnw_open,
    .release =   dnw_release,
};
 
static struct usb_class_driver dnw_class = {
    .name =     "secbulk%d",
    .fops =     &dnw_ops,
    .minor_base = 100,
};
 
 
static int dnw_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
    /* 接口设置描述 */
    struct usb_host_interface *interface;
    struct usb_endpoint_descriptor *endpoint;  
    int i;
   
    interface = intf->cur_altsetting;
   
   
   
    for(i=0;i<interface->desc.bNumEndpoints;i++)
    {
    endpoint = &interface->endpoint[i].desc;
    if(usb_endpoint_is_bulk_out(endpoint))
    {
        bulk_out_endaddr =    endpoint->bEndpointAddress;
        break;
    }
   
    }
   
    usb_register_dev(intf,&dnw_class);
   
    udev = usb_get_dev(interface_to_usbdev(intf));
   
}
 
static void dnw_disconnect(struct usb_interface *intf)
{
    usb_deregister_dev(intf,&dnw_class);
}
 
struct usb_driver dnw_driver = {
    .name       = "dnw",   /* 驱动名 */
    .probe      = dnw_probe, /* 捕获函数 */
    .disconnect  = dnw_disconnect, /* 卸载函数 */
    .id_table   = dnw_id_table, /* 设备列表 */
};
 
 
 
int dnw_init()
{
    usb_register(&dnw_driver);
    return 0;
}
 
void dnw_exit()
{
    usb_deregister(&dnw_driver);  
}
 
 
module_init(dnw_init);
module_exit(dnw_exit);
MODULE_LICENSE("GPL");

分类: 嵌入式