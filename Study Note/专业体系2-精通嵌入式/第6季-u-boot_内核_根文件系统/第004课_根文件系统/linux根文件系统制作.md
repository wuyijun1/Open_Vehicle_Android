# [嵌入式Linux利用busybox制作根文件系统](https://blog.csdn.net/qq_24835087/article/details/104429824?utm_medium=distribute.pc_relevant.none-task-blog-OPENSEARCH-13.control&dist_request_id=&depth_1-utm_source=distribute.pc_relevant.none-task-blog-OPENSEARCH-13.control)

# [Linux开发之根文件系统构建及过程详解](https://blog.csdn.net/wangweijundeqq/article/details/82533485)

git   clone https://git.busybox.net/busybox


在嵌入式中移植的内核下载到开发板上，是没有办法真正的启动Linux操作系统的，会出现无法加载文件系统的错误。

那么根文件系统在系统启动中到底是什么时候挂载的呢？先将/dev/ram0挂载，而后执行/linuxrc.等其执行完后。切换根目录，再挂载具体的根文件系统.根文件系统执行完之后，也就是到了Start_kernel()函数的最后，执行init的进程，也就第一个用户进程。对系统进行各种初始化的操作。

根文件系统之所以在前面加一个”根“，说明它是加载其它文件系统的”根“，既然是根的话，那么如果没有这个根，其它的文件系统也就没有办法进行加载的。它包含系统引导和使其他文件系统得以挂载（mount）所必要的文件。根文件系统包括Linux启动时所必须的目录和关键性的文件

一.创建基本目录

1.一般我们的rootfs目录会建在/opt目录下，但我为了不覆盖以前存在的rootfs，我在home目录下键rootfs目录进行根文件系统制作

[weishusheng@localhost ~]$ mkdir rootfs

 

2.在目录下创建各子目录

mkdir -p

{apps,bin,data,dev,info,proc,root,sbin,sys,tmp,var,etc/{,init.d,dropbear},mnt/{,usb,sdc,nfs,dev},usr/{,bin,sbin,lib,share},lib/{,modules/{,3.0.0}}}

 

3.dev目录下创建设备文件，因为内核挂载完文件系统后，init进程需要用到/dev/console和/dev/null这两个设备文件来调用mdev构建dev，所以必须在制作文件系统时静态创建这两个设备文件，否则在系统启动时将提示：Waring：unable to open an initial console:

执行：

[weishusheng@localhost rootfs]$ sudo mknod -m666 dev/null c 1 3
[weishusheng@localhost rootfs]$ sudo mknod -m666 dev/console c 5 1
[weishusheng@localhost rootfs]$ sudo mknod -m666 dev/ttyS0 c 4 64
[weishusheng@localhost rootfs]$ sudo mknod -m666 dev/ttySAC0 c 4 64
[weishusheng@localhost rootfs]$ sudo mknod dev/mtdblock0 b 31 0
[weishusheng@localhost rootfs]$ sudo mknod dev/mtdblock1 b 31 1
[weishusheng@localhost rootfs]$ sudo mknod dev/mtdblock2 b 31 2
[weishusheng@localhost rootfs]$ sudo mknod dev/mtdblock3 b 31 3
[weishusheng@localhost rootfs]$ sudo mknod dev/mtdblock4 b 31 4
[weishusheng@localhost rootfs]$ sudo mknod dev/mtdblock5 b 31 5
[weishusheng@localhost rootfs]$ sudo mknod dev/mtdblock6 b 31 6
[weishusheng@localhost rootfs]$ sudo mknod dev/mtdblock7 b 31 7
[weishusheng@localhost rootfs]$ sudo mknod dev/mtdblock8 b 31 8
[weishusheng@localhost rootfs]$ sudo mknod dev/mtdblock9 b 31 9

 

[weishusheng@localhost rootfs]$ ls -l dev/
total 0
crw-rw-rw- 1 root root 5, 1 Aug 23 10:46 console
brw-r--r-- 1 root root 31, 0 Aug 23 10:47 mtdblock0
brw-r--r-- 1 root root 31, 1 Aug 23 10:47 mtdblock1
brw-r--r-- 1 root root 31, 2 Aug 23 10:47 mtdblock2
brw-r--r-- 1 root root 31, 3 Aug 23 10:47 mtdblock3
brw-r--r-- 1 root root 31, 4 Aug 23 10:47 mtdblock4
brw-r--r-- 1 root root 31, 5 Aug 23 10:48 mtdblock5
brw-r--r-- 1 root root 31, 6 Aug 23 10:48 mtdblock6
brw-r--r-- 1 root root 31, 7 Aug 23 10:48 mtdblock7
brw-r--r-- 1 root root 31, 8 Aug 23 10:48 mtdblock8
brw-r--r-- 1 root root 31, 9 Aug 23 10:48 mtdblock9
crw-rw-rw- 1 root root 1, 3 Aug 23 10:46 null
crw-rw-rw- 1 root root 4, 64 Aug 23 10:46 ttyS0
crw-rw-rw- 1 root root 4, 64 Aug 23 10:47 ttySAC0

 

4.var目录下创建符号链接文件

[weishusheng@localhost rootfs]$ ln -s /tmp var/lock
[weishusheng@localhost rootfs]$ ln -s /tmp var/log
[weishusheng@localhost rootfs]$ ln -s /tmp var/run
[weishusheng@localhost rootfs]$ ln -s /tmp var/tmp
[weishusheng@localhost rootfs]$ ls -l var/
total 0
lrwxrwxrwx 1 weishusheng weishusheng 4 Aug 23 10:53 lock -> /tmp
lrwxrwxrwx 1 weishusheng weishusheng 4 Aug 23 10:53 log -> /tmp
lrwxrwxrwx 1 weishusheng weishusheng 4 Aug 23 10:54 run -> /tmp
lrwxrwxrwx 1 weishusheng weishusheng 4 Aug 23 10:54 tmp -> /tmp

 

5.拷贝交叉编译器中的动态库到相应的目录下

[weishusheng@localhost rootfs]$ cp -af /opt/buildroot-2011.11/arm920t/usr/arm-linux/sysroot/lib/*so* lib/

[weishusheng@localhost rootfs]$ cp -af /opt/buildroot-2011.11/arm920t/usr/arm-linux/lib/*so* lib/

[weishusheng@localhost rootfs]$ ls lib/
ld-uClibc-0.9.32.so libdl-0.9.32.so libm.so.0 libresolv-0.9.32.so libutil-0.9.32.so
ld-uClibc.so.0 libdl.so.0 libnsl-0.9.32.so libresolv.so.0 libutil.so.0
libc.so.0 libgcc_s.so libnsl.so.0 librt-0.9.32.so modules
libcrypt-0.9.32.so libgcc_s.so.1 libpthread-0.9.32.so librt.so.0
libcrypt.so.0 libm-0.9.32.so libpthread.so.0 libuClibc-0.9.32.so
[weishusheng@localhost rootfs]$

6.目录下创建一些文件

[weishusheng@localhost rootfs]$ cd etc/
[weishusheng@localhost etc]$ ll
total 8
drwxrwxr-x 2 weishusheng weishusheng 4096 Aug 23 10:40 dropbear
drwxrwxr-x 2 weishusheng weishusheng 4096 Aug 23 10:40 init.d

 

[lingyun@localhost etc]$ ls
dropbear init.d
[lingyun@localhost etc]$ vim inittab
# /etc/inittab
#
# Copyright (C) 2014-8-23 weishusheng
#
# Note: BusyBox init doesn't support runlevels. The runlevels field is
# completely ignored by BusyBox init. If you want runlevels, use sysvinit.
#
# Format for each entry: <id>:<RUnlevels>:<action>:<process>
#
# id == tty to run on, or empty for /dev/console.
# If specified, then /dev/$id device must exist
# runlevels == ignored, busybox doesn't support it
# action == one of sysinit, respawn, askfirst, wait, and once
# process == program to run

# Startup the system
# mount all the file systems specified in /etc/fstab
::sysinit:/bin/mount -a

#Use mdev as hotplug to auto mount USB storage or SD card
::sysinit:/bin/echo /sbin/mdev > /proc/sys/kernel/hotplug

#Use mdev to auto generate the device node in /dev path
::sysinit:/sbin/mdev -s

#make shm, pts support
::sysinit:/bin/mkdir -p /dev/pts
::sysinit:/bin/mkdir -p /dev/shm
::sysinit:/bin/mount -t devpts devpts /dev/pts

#Mount our apps/info partition
null::wait:/bin/mount -o sync,noatime,ro -t jffs2 /dev/mtdblock6 /apps
null::wait:/bin/mount -o sync,noatime,ro -t jffs2 /dev/mtdblock7 /info

#Set hostname
null::sysinit:/bin/hostname -F /etc/hostname

#Enable console logon
null::respawn:/sbin/getty -L ttyS0 115200 vt100

# now run any rc scripts
null::wait:/etc/init.d/rcS

# system daemon
null::respawn:/sbin/syslogd -n
null::respawn:/sbin/klogd -n

# Stuff to do before rebooting
null::shutdown:/bin/umount /apps
null::shutdown:/bin/umount /info
null::shutdown:/bin/killall klogd
null::shutdown:/bin/killall syslogd

null::shutdown:/bin/umount -a -r
#null::shutdown:/sbin/swapoff -a

 

7.创建/etc/init.d/rcS脚本

[weishusheng@localhost etc]$ vim init.d/rcS
#!/bin/sh

# Copyright (C) 2014-8-23 weishusheng
# Start all init scripts in /etc/init.d
# executing them in numerical order.
#

for i in /etc/init.d/S??* ; do
$i
done

 

8.配置网卡的启动脚本

[weishusheng@localhost etc]$ vim init.d/S01_network
#!/bin/sh

ifconfig eth0 192.168.1.111 netmask 255.255.255.0 up

9.etc目录下的启动脚本

[weishusheng@localhost etc]$ vim init.d/S99_rcsApp
#!/bin/sh

# Copyright (C) 2014-08-23 weishusheng
#
# Start all init scripts in /apps/etc/init.d
# executing them in numerical order.
#
if (test -d /apps/etc/init.d)
then
for i in /apps/etc/init.d/S??* ; do
$i
done
fi

 

10.修改init.d目录下的文件权限

chmod 777 init.d/*

[weishusheng@localhost etc]$ ll init.d/*
-rwxrwxrwx 1 weishusheng weishusheng 64 Aug 23 11:11 init.d/S01_network
-rwxrwxrwx 1 weishusheng weishusheng 234 Aug 23 11:21 init.d/S99_rcsApp
-rwxrwxrwx 1 weishusheng weishusheng 208 Aug 23 11:09 init.d/rcS

 

11.创建fstab文件

[weishusheng@localhost etc]$ vim fstab
# /etc/fstab: static file system information.
# Copyright (C) 2014-08-23 weishusheng
#
#<File system> <mount pt> <type> <options> <dump> <pass>
#devpts /dev/pts devpts defaults 0 0
#/dev/root / ext2 rw,noauto 0 1
proc /proc proc defaults 0 0
tmpfs /tmp tmpfs defaults 0 0
tmpfs /dev tmpfs defaults 0 0
sysfs /sys sysfs defaults 0 0

 

12.创建hostname,hosts,TZ文件

[weishusheng@localhost etc]$ echo "root" > hostname
[weishusheng@localhost etc]$ echo "127.0.0.1 localhost" >> hosts
[weishusheng@localhost etc]$ echo "MST7MDT" >> TZ
[weishusheng@localhost etc]$ echo "Copyright (C) 2014 weishusheng" >> issue      #系统启动时的启动信息

13.创建profile文件

[weishusheng@localhost etc]$ vim profile

# /etc/profile: system-wide .profile file for the Bourne shells.

 

export PATH=\

/bin:\

/sbin:\

/usr/bin:\

/usr/sbin:\

/usr/local/bin:\

/apps/bin:\

/apps/tools:\

/apps/tslib/bin\

 

# If running interactively, then:

if [ "$PS1" ]; then

 

    if [ "$BASH" ]; then

        export PS1="[\u@\h \W]\\$ "

        alias ll='/bin/ls --color=tty -laFh'

        alias ls='/bin/ls --color=tty -F'

        export

        

LS_COLORS='no=00:fi=00:di=01;34:ln=01;36:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:ex=01;

32:*.tar=01;31:*.tgz=01;31:*.arj=01;31:*.taz=01;31:*.lzh=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.gz=01;31:*.bz2=01;31:*.deb=0

1;31:*.rpm=01;31:*.jar=01;31:*.jpg=01;35:*.jpeg=01;35:*.png=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35

:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.mpg=01;35:*.mpeg=01;35:*.avi=01;35:*.fli=01;35:*.gl=01;35:*.d

l=01;35:*.xcf=01;35:*.xwd=01;35:';

    else

      if [ "`id -u`" -eq 0 ]; then

        export PS1='>: '

      else

        export PS1='>: '

      fi

    fi

# System Setting

    set -o vi

    alias  ll='ls -l'

    export USER=`id -un`

    export LOGNAME=$USER

    export HOSTNAME=`/bin/hostname`

    export HISTSIZE=1000

    export HISTFILESIZE=1000

    export PAGER='/bin/more '

    export EDITOR='/bin/vi'

    export INPUTRC=/etc/inputrc

    export DMALLOC_OPTIONS=debug=0x34f47d83,inter=100,log=logfile

    export VAR1=

    export VAR2=

    export VAR3=

    export VAR4=

    export VAR5=

    export LD_LIBRARY_PATH=/lib:/usr/lib/

# QT Extendded 4.4.3 Setting

    export QTDIR=/apps/qt-extended-4.4.3

    export QWS_MOUSE_PROTO='TSLIB:/dev/event0'

    export QWS_DISPLAY='LinuxFB:/dev/fb0'

    export QWS_DISPLAY='LinuxFB:mmWidth240:mmHeight320:0'

    export QWS_SIZE='240x320'

    export QT_PLUGIN_PATH=$QTDIR/plugins/

    export QT_QWS_FONTDIR=$QTDIR/lib/fonts

    export PATH=$QTDIR/bin:$PATH

    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$QTDIR/lib

# Touch Scree tslib Setting

    export TSLIB_ROOT=/apps/tslib

    export TSLIB_CONFFILE=$TSLIB_ROOT/etc/ts.conf

    export TSLIB_CALIBFILE=$TSLIB_ROOT/etc/pointercal

    export TSLIB_TSDEVICE=/dev/event0

    export TSLIB_CONSOLEDEVICE=none

    export TSLIB_FBDEVICE=/dev/fb0

fi;

14.创建指定一些协议所使用的端口号文件protocols

[weishusheng@localhost etc]$ vim protocols

# /etc/protocols:

# $Id: protocols,v 1.1.1.1 2001/09/12 19:03:24 andersee Exp $

#

# Internet (IP) protocols

#

#       from: @(#)protocols     5.1 (Berkeley) 4/17/89

#

# Updated for NetBSD based on RFC 1340, Assigned Numbers (July 1992).

 

ip      0       IP              # internet protocol, pseudo protocol number

icmp    1       ICMP            # internet control message protocol

igmp    2       IGMP            # Internet Group Management

ggp     3       GGP             # gateway-gateway protocol

ipencap 4       IP-ENCAP        # IP encapsulated in IP (officially ``IP'')

st      5       ST              # ST datagram mode

tcp     6       TCP             # transmission control protocol

egp     8       EGP             # exterior gateway protocol

pup     12      PUP             # PARC universal packet protocol

udp     17      UDP             # user datagram protocol

hmp     20      HMP             # host monitoring protocol

xns-idp 22      XNS-IDP         # Xerox NS IDP

rdp     27      RDP             # "reliable datagram" protocol

iso-tp4 29      ISO-TP4         # ISO Transport Protocol class 4

xtp     36      XTP             # Xpress Tranfer Protocol

ddp     37      DDP             # Datagram Delivery Protocol

idpr-cmtp       39      IDPR-CMTP       # IDPR Control Message Transport

rspf    73      RSPF            #Radio Shortest Path First.

vmtp    81      VMTP            # Versatile Message Transport

ospf    89      OSPFIGP         # Open Shortest Path First IGP

ipip    94      IPIP            # Yet Another IP encapsulation

encap   98      ENCAP           # Yet Another IP encapsulation

 

15.创建mdev.conf文件

mdev会在/etc目录下找mdev的配置文件: mdev.conf. 如果该文件不存在，那么在执行mdev –s这个命令时，

会提示找不到mdev.conf，这时我们可以建一个空的mdev.conf文件解决这个问题。下面创建使用mdev自动挂载u盘和SD卡的配置/etc/mdev.conf

[weishusheng@localhost etc]$ vim mdev.conf
sd[a-z][0-9] 0:0 0777 @(mount /dev/$MDEV /mnt/usb)
sd[a-z] 0:0 0777 $(umount /mnt/usb)
ub[a-z][0-9] 0:0 0777 @(mount /dev/$MDEV /mnt/usb)
ub[a-z] 0:0 0777 $(umount /mnt/usb)
mmcblk[0-9]p[0-9] 0:0 0777 @(mount /dev/$MDEV /mnt/sdc)
mmcblk[0-9] 0:0 0777 $(umount /mnt/sdc)

16.创建用户组group文件

它的格式如下：

 

groupname:password:gid:members

 

第一个字段为用户组名称

第二个字段为用户组密码，当为x时密码是映射到/etc/gshadow中的，是非逆的

第三个字段为GID，及组号，为正整数或0，0被付于了root用户组；系统通常会预留一些较靠前的GID给系统虚拟用户之用，每个系统预留的GID都不同，Fedora预留了500个，所以我们添加新用户组时是从500开始的。GID的范围由/etc/login.defs中的GID_MIN和GID_MAX决定

第四个字段为用户列表，每个用户间用逗号分隔

 

这里的password代表组口令，很少用到。它可使原先不在这个群组中的用户可以通过newgrp命令暂时继承该组的权限，使用 newgrp命令时会新开一个shell。口令的加密方式和passwd文件中的口令一样，所以如果需设置组口令，要用passwd程序虚设一个用户，再把该用户password节中的加密口令拷贝到/etc/group文件中。members列代表组成员，我们可把需加入该组的用户以逗号分隔添加到这里即可。同一组的成员可继承该组所拥有的权限。

 



 

17.创建用户passwd文件

它的格式如下：

username:password:uid:gid:gecos:homedir:shell

 

第一个字段为登录名

第二个字段为口令，一般被映射到shadow文件中

第三个字段为UID

第四个字段为GID

第五个字段为用户名全称，gecos是通用电子计算机操作系统的缩写，是Bell实验室中的一台大型主机。

第六个字段为用户根目录

第七个字段为用户所用SHELL的类型

 

Unix系统最初是用明文保存密码的，后来由于安全的考虑，采用crypt()算法加密密码并存放在/etc/passwd文件。现在，由于计算机处理能力的提高，使密码破解变得越来越容易。/etc/passwd文件是所有合法用户都可访问的，大家都可互相看到密码的加密字符串，这给系统带来很大的安全威胁。现代的Unix系统使用影子密码系统，它把密码从/etc/pa sswd文件中分离出来，真正的密码保存在/etc/shadow文件中，shadow文件只能由超级用户访问。这样入侵者就不能获得加密密码串，用于破解。使用shadow密码文件后，/etc/passwd文件中所有帐户的password域的内容为"x"，如果password域的内容为"*"，则该帐号被停用。使用passwd这个程序可修改用户的密。



18. 创建密码映射shadow文件

他的格式如下：

username:password:last_change:min_change:max_change:warm:failed_expire:expiration:reserved

第一字段：用户名（也被称为登录名），在/etc/shadow中，用户名和/etc/passwd 是相同的，这样就把passwd 和shadow中用的用户记录联系在一起；这个字段是非空的；

第二字段：密码（已被加密），这个字段是非空的；

第三字段：上次修改口令的时间；这个时间是从1970年01月01日算起到最近一次修改口令的时间间隔（天数），您可以通过passwd 来修改用户的密码，然后查看/etc/shadow中此字段的变化；

第四字段：两次修改口令间隔最少的天数；如果这个字段的值为空，帐号永久可用；

第五字段：两次修改口令间隔最多的天数；如果这个字段的值为空，帐号永久可用；

第六字段：提前多少天警告用户口令将过期；如果这个字段的值为空，帐号永久可用；

第七字段：在口令过期之后多少天禁用此用户；如果这个字段的值为空，帐号永久可用；

第八字段：用户过期日期；此字段指定了用户作废的天数（从1970年的1月1日开始的天数），如果这个字段的值为空，帐号永久可用；

第九字段：保留字段，目前为空，以备将来发展之用； 

这里我们设置为不用密码登陆，将password格式的内容清空：

[weishusheng@localhost etc]$ vim shadow
root::0:0:99999:7:::

 

二. 在文件系统中安装busybox

 

BusyBox各版本下载地址

免费下载地址在 http://linux.linuxidc.com/

用户名与密码都是www.linuxidc.com

 具体下载目录在 /pub/BusyBox/

 

1.解压

[weishusheng@localhost ~]$ tar -xjf busybox-1.20.0.tar.bz2

2.进入 busybox-1.20.0，修改Makefile

 

[weishusheng@localhost ~]$ cd busybox-1.20.0
[weishusheng@localhost busybox-1.20.0]$ vim Makefile

把CROSS_COMPILER为：

CROSS_COMPILE ?= /opt/buildroot-2012.08/arm920t/usr/bin/arm-linux-

（每个人路径不一样，找到自己真正的交叉编译器路径）

3.配置make menuconfig

[weishusheng@localhost busybox-1.20.0]$ export TERM=vt100
[weishusheng@localhost busybox-1.20.0]$ make menuconfig

Busybox Settings  --->

       General Configuration  --->

             [*] Don't use /usr

       Installation Options ("make install" behavior)  --->

             What kind of applet links to install (as soft-links)  --->

             (/home/weishusheng/rootfs) BusyBox installation prefix

#其他选项结合和自己的需求定制

编译完成后

[weishusheng@localhost busybox-1.20.0]$ file busybox
busybox: ELF 32-bit LSB executable, ARM, version 1 (SYSV), dynamically linked (uses shared libs), stripped

 

4.安装busybox到你的文件系统下

[weishusheng@localhost busybox-1.20.0]$ sudo make install

安装完了之后

ls ../rootfs

apps  bin  data  dev  etc  info  lib  linuxrc  mnt  proc  root  sbin  sys  tmp  usr  var

发现多了个linuxrc的文件。

 

三.移植dropbear

1.首先编译生成PC版的，在制作密钥时用到

 

[weishusheng@localhost ~]$ wget http://matt.ucc.asn.au/dropbear/releases/dropbear-0.53.1.tar.bz2

 

2.解压

[weishusheng@localhost ~]$ tar -xjf dropbear-0.53.1.tar.bz2

3.

[weishusheng@localhost dropbear-0.53.1]$ ./configure && make

生成的文件： 

    dropbear： ssh2 server

    dropbearkey: 密钥生成器

    dropbearconvert: 可以转换openssh的密钥

    dbclient: ssh2 client 

4.

[weishusheng@localhost dropbear-0.53.1]$ ./dropbearkey -t rsa -f /home/weishusheng/rootfs/etc/dropbear/dropbear_rsa_host_key

[weishusheng@localhost dropbear-0.53.1]$ ./dropbearkey -t dss -f /home/weishusheng/rootfs/etc/dropbear/dropbear_dss_host_key

[weishusheng@localhost dropbear-0.53.1]$ chmod 666 /home/weishusheng/rootfs/etc/dropbear/dropbear_*

5.编译ARM版本的

[weishusheng@localhost dropbear-0.53.1]$ make distclean

[weishusheng@localhost dropbear-0.53.1]$ ./configure CC=/opt/buildroot-2011.11/arm920t/usr/bin/arm-linux-gcc --build=i686 --host=arm-linux --disable-zlib

[weishusheng@localhost dropbear-0.53.1]$ make

 

[weishusheng@localhost dropbear-0.53.1]$ file dropbear
dropbear: ELF 32-bit LSB executable, ARM, version 1 (SYSV), dynamically linked (uses shared libs), not stripped

 

[weishusheng@localhost dropbear-0.53.1]$ file dbclient
dbclient: ELF 32-bit LSB executable, ARM, version 1 (SYSV), dynamically linked (uses shared libs), not stripped

 

[weishusheng@localhost dropbear-0.53.1]$ mv dbclient ssh

[weishusheng@localhost dropbear-0.53.1]$ arm-linux-strip dropbear

（如果arm-linux-strip命令执行错误，给出交叉编译器路径[weishusheng@localhost dropbear-0.53.1]$ /opt/buildroot-2011.11/arm920t/usr/bin/arm-linux-strip dropbear）

[weishusheng@localhost dropbear-0.53.1]$ /opt/buildroot-2011.11/arm920t/usr/bin/arm-linux-strip ssh

拷贝到rootfs目录下

[weishusheng@localhost dropbear-0.53.1]$ cp dropbear ssh /home/weishusheng/rootfs/usr/sbin/

6.在文件系统中创建启动脚本

[weishusheng@localhost ~]$ cd rootfs/

[weishusheng@localhost rootfs]$ vim etc/init.d/S04_dropbear
#!/bin/sh
/usr/sbin/dropbear

[weishusheng@localhost rootfs]$ chmod 777 etc/init.d/S04_dropbear

 

 

到此时，如果我们直接编译下载，会提示

VFS: Cannot open root device "(null)" or unknown-block(0,0)的错误信息，原因是制作initramfs文件系统启动时会在根文件系统中执行第一个init程序，它对uboot传过来的参数init=/linuxrc不予理睬，所以需要在上面制作的根文件系统里的根目录加入init程序，否则无法启动。init程序和linuxrc程序一样都是符号链接文件，它们都是指向/bin/busybox程序。

#解决办法：

在制作好的文件系统中加入init程序，其实就是相当于制作initramfs文件系统了！！！

[weishusheng@localhost rootfs]$ ln -s bin/busybox init

 

7.在内核中指定根文件系统的目录，然后重新编译下载

[weishusheng@localhost linux-3.0]$ sudo make menuconfig

General setup  --->
       [*] Initial RAM filesystem and RAM disk (initramfs/initrd) support
       (/home/weishusheng/rootfs) Initramfs source file(s)    #输入根文件系统的所在目录  

[weishusheng@localhost linux-3.0]$ make

由于我们把根文件系统完善成了initramfs文件系统，u-boot专递给内核的参数需要改动，在开发板启动u-boot后进入调试模式，输入以下命令

[ s3c2440@wss ]# set bkr 'tftp 30008000 linuxrom-s3c2440.bin;nand erase 100000 800000;nand write 30008000 100000 800000'

[ s3c2440@wss ]# set bootcmd_initramdisk 'nand read 30008000 100000 800000;bootm 30008000'

[ s3c2440@wss ]# set bootargs 'console=ttyS0,115200 mem=64M init=/linuxrc rw loglevel=7'

[ s3c2440@wss]# set bootcmd 'run bootcmd_initramdisk'

[ s3c2440@wss]# save

 然后把内核下载到开发板上，ok 了。

 