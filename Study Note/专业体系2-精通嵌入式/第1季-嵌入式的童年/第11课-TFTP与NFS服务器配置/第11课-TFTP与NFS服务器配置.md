嵌入式-第一季-第11课 

第十一课 TFTP与NFS服务器
一．交叉开发
嵌入式系统开发采用交叉开发模式，其中产生嵌入式软件的平台叫做宿主机，通常为PC电脑，运行嵌入式软件的平台称为目标机。宿主机一般通过网络，USB，JTAG等方式讲软件下载到目标机。
二．tftp服务器
为了完成交叉开发，我们可以通过数据线来下载数据，同样，我们也可以通过网络来下载数据。tftp协议就是一款网络协议，可以将数据通过网络下载到目标机。它的目的主要目的是从开发板上下载，我们这里先用windows系统来实验。
安装tftp服务器
rpm  -ivh/mnt/Packages/xinetd-2.3-14-34.el6.i686.rpm
配置tftp服务器
vim  /etc/xinetd.d/tftp
重启tftp服务器
etc/init.d/xinetd  restart
测试tftp服务器
在这一个过程中我们切记要关闭防火墙和输入setenforce permissive
注意：这里有个值得注意的地方，我们在运行tftp32这个软件的时候，不见要设置linux的防火墙，将它们关闭，也要讲我们windows系统的防火墙一并关闭，否者会出现。timeout ，block的标识。
三．NFS服务器
NFS（网络文件系统）为两台Linux主机之间的通讯，提供了类似于windows上共享目录一样的服务。在嵌入式Linux开发中，Linux目标机经常通过NFS方式访问Linux宿主机上的文件。为了实现这种访问，首先需要在Linux宿主机上搭建NFS服务器。
配置NFS服务器
vim /etc/exports
添加：
/temp.*(rw,sync,no_root_squash)
/temp: 共享的目录
*: 允许访问共享目录的主机IP, *表示所有
(rw,sync,no_root_squash):访问权限，这里rw表示可读可写，sync表示网络上的用户将修改后的文件写回原来的文件，no_root_squash表示当用户是用root登陆的时候，我们访问的时候也是具有root权限的。这里有个小技巧，就是后面的内容一般都不该，直接使用就好。
启动NFS服务器
/etc/init.d/nfs restart
我们这里的验证放在后面的学习当中。