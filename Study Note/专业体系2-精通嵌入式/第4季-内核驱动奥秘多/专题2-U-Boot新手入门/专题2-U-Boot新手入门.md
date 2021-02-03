第四季-专题2-U-Boot新手入门 

专题2-U-Boot新手入门
 
对于u-boot的使用主要分为两个方面，一个方面是u-boot的使用，另一个是它的开发。
2.1 编译u-boot
嵌入式的层次：bootloader（u-boot），linux内核，根文件系统。
1. 配置U-Boot
TQ210:    make TQ210_config
Smart210:   make smart210_config
OK210:     make forlinx_linux_config
OK6410:    make forlinx_nand_ram256_config
Tiny6410:   make tiny6410_config
TQ2440:   make TQ2440_config
Mini2440:   make mini2440_config
 
2. 下载与运行
TQ210:    tftp 0xc0008000 uImage
Smart210:  tftp 0x20000000 uImage
OK210:    tftp 0xc0008000 uImage
OK6410:   tftp 0xc0008000 uImage
Tiny6410:  tftp 0xc0008000 uImage
TQ2440:   tftp 0x31000000 uImage
Mini2440:  tftp 0x31000000 uImage
 
2.2  u-boot命令详解
1. 帮助命令
尽管UBOOT提供了丰富的命令集，但不同的开发板所支持的命令却不一样（可配置），help 命令可用于察看当前单板所支持的命令。
# help
autoscr -run script from memory
base -print or set address offset
bdinfo -print Board Info structure
bootm -boot application image from memory
2. 环境变量相关命令
（1）printenv:查看环境变量
#printenv
ipaddr=192.168.0.1
ethaddr=12:34:56:78:9A:BC
serverip=192.168.1.5
（2）setenv 添加、修改、删除环境变量
l  setenv name value ...
- add/modify environment variable 'name' to 'value’ 添加和更改条件变量
l  setenv name
- delete environment variable 'name'             删除条件变量
（3）saveenv 保存环境变量
将当前定义的所有变量及其值存入flash中。（即使在关机重启依然存在）
我们正常更改的条件变量都是在内存中的，只有经过这个语句的更改，更改才会体现在flash中。
3. 文件下载
正常的文件下载有很多种的方式，其中最常用也是最简单的就是tftp下载。tftp 通过网络下载文件。
注意：使用tftp，需要先配置好网络
#setenv ethaddr 12:34:56:78:9A:BC                  开发板的网卡地址，有一个就行
#setenv ipaddr 192.168.0.3                           开发板ip地址
#setenv server ip 192.168.0.2 （tftp服务器的地址）     linux的ip地址
#ping 192.168.0.2(tftp服务器的地址)
范例：
#tftp 0xc0008000 uImage
把tftp服务器上的uImage下载到0xc0008000处。
注：netstat -a |grep tftp  可以查询tftp程序是否已经启动。
不同的开发板下载地址如下:
2440：30008000
6410：50008000
210：20008000
 
 
 
4. 执行程序
bootm {addr} {arg}
执行固定格式的2进制程序
范例：
#bootm 0xc0008000
5. 查看内存内容
（1）md 显示内存区的内容。
md采用十六进制和ASCII码两种形式来显示存储单元的内容。
这条命令还可以采用长度标识符 .l, .w和.b 。
mm表示修改
md [.b, .w, .l] address
范例：
md.w 100000
00100000: 2705 1956 5050 4342 6f6f 7420 312e 312e
00100010: 3520 284d 6172 2032 3120 3230 3032 202d
（2）修改内存内容
mm 修改内存，地址自动递增。
mm [.b, .w, .l] address
mm 提供了一种互动修改存储器内容的方法。它会显示地址和当前值，然后提示用户输入。如果你输入了一个合法的十六进制数，这个新的值将会被写入该地址。然后提示下一个地址。如果你没有输入任何值，只是按了一下回车，那么该地址的内容保持不变。
如果想结束输入，则输入空格，然后回车。
=> mm 100000
00100000: 27051956 ? 0
00100004: 50504342 ? AABBCCDD
6. nand flash命令
（1）擦除nand flash
nand erase 起始地址start 长度len
擦除start处开始的，长度为len的区域
范例：
#nand erase 0x400000 0x500000
（2）写/读nand flash
nand write 内存起始地址 flash起始地址 长度len
将内存起始地址处，长度为len的数据，写入flash起始地址处。
范例：
#nand write c0008000 400000 500000
nand read 内存起始地址 flash起始地址 长度len
将flash起始地址处，长度为len的数据，读到内存起始地址处。
范例：
#nand read c0008000 400000 500000
7. 设置自启动
（1）设置从nand flash自动启动
#setenv bootcmd nand read c0008000 400000 500000 \; bootm c0008000
（2）设置自动下载内核到内存后启动
#setenv bootcmd tftp c0008000 uImage \; bootm c0008000
 
l  制作启动SD卡：
在制作SD卡之前我们先准备4G或者8G的一个大的SD卡，再就是一个读卡器。在使用前确保电脑上或者虚拟机上已经安装了对应的驱动，若是没有安装，请自行安装。
将SD卡连接到电脑，先将它格式化，之后通过虚拟机断开它与windows的连接，连接到虚拟机上。我们在终端中输入，fdisk -l命令，会显示下面的内容：
 
根据SD卡的实际大小（我们用的是4G的），我们可以知道它对应的文件夹是/dev/sdb。
 
我们输入命令dd iflag=dsync oflag=dsync if=./uboot/u-boot.bin of=/dev/sdb seek=1回车运行，将文件烧写到SD卡中，显示如下：
 
iflag=dsync，读取文件时，采用同步方式
oflag=dsync，写入文件时，采用同步方式
if =xx.bin，要复制的文件名
of=/dev/sdb，文件被复制到的设备，/dev/sdb表示sd卡，当我们把内存卡插到电脑上的时候，会出现对应的一个文件，这个文件一般就是dev/sdb
seek=1，sd卡的第一个扇区
 
l  安装bootloader：
嵌入式的层次：bootloader（u-boot），linux内核，根文件系统。
对于BootLoader，我们常用的就是u-boot。我们现在按步骤安装u-boot，先将u-boot的压缩包通过samba服务器传到linux系统中，我们把它放在目录/root/home/part1中，通过命令：tar xvzf uboot.tar.gz对文件进行解压。通过cd uboot进入uboot目录。
下面我们进行的工作就是uboot的编译，它的编译有两个环节，第一个环节就是进行配置。为什么要进行配置呢？因为我们用的这个文件它是可能使用于很多的开发板的，可能成百上千，但是我们编译出来的映像只能运行在一个开发板，所以我们需要配置这一步。
配置实际上就是通过Makefile中的一个称为目标的信息来配置的，我们输入命令：vim Makefile，进入Makefile文件，输入/TQ210查到我们需要的配置信息，如下：
 
这句话的意思就是我们如果配置的是对应TQ210的开发板的话，就输入命令：make TQ210_config命令。
在配置完成之后，我们进行编译，输入命令：make ARCH=arm CROSS_COMPILE=arm-linux-
进行编译，其中ARCH=arm表示对应的系统，CROSS_COMPILE=arm-linux-表示对应的交叉工具链，可以是arm-linux-gcc货值arm-linux-ld等。
编译完成后，我们会在文件夹下面找到u-boot.bin文件，整个文件夹的样式如下图：