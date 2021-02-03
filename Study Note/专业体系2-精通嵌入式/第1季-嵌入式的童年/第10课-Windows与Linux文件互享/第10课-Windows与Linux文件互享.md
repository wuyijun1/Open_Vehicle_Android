嵌入式-第一季-第10课 

第十课 windows与linux文件互享
一．前提
从windows能够ping到Linux
关闭linux的防火墙：/etc/init.d/iptables stop
二．步骤
winSCP
这是一种软件，可以将linux系统和windows系统之间相互连接，文件传递。
Samba服务器
（1）安装samba服务器
在装载samba服务器之前我们是要用到之前我们用的红帽光盘中的相关软件，用到了命令mount /dev/cdrom  /mnt 将指定的光盘文件装载在/mnt文件中。
rpm -ivh /mnt/Packages/samba-3.5.10-125.el6.i686.rpm
（2）添加用户并且修改密码
useradd  win
passwd  win
（3）设置该用户登录smaba服务的密码
smbpasswd -a win
（4）配置samba服务
vim  /etc/samba/smb.conf
（5）重启samba服务
/etc/init.d/smb  restart
这里说明，注意事项。首先安装samba服务器要在根目录下来未完成，不要在菜单录下操作。我们添加的用户win就是为了在它的界面下实现文件传输的，不可以在其他的用户下完成文件传输，即使我们可以查看root目录下的文件。设置的smbpasswd密码是为了在通过ie浏览器能ping到linux系统时的开门钥匙。配置samba服务器是为了，能够查看root的文件，我们可以复制其中的文件（yy）在粘贴（p）接着更改其中的内容。
常见问题
通过samba访问linux速度很慢，而且很多的目录访问失败。
故障原因
未关闭Selinux，果果执行命令setenforce  permissive将其关闭。
Selinux是linux系统中的一个安全模块，和防火墙有些类似。
注意：Vmware Tools也是能进行虚拟机和主机之间的转换的，但是只是限制于虚拟机，应用不广泛。
 
注：这里说明，我们要在ie浏览器里面输入\\192.168.159.128.这里面的斜杠的方向是要注意的，而且该地址只是应用在本人的电脑的。