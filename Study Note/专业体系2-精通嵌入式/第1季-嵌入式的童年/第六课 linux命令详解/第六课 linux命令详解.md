嵌入式-第一季-第6课 

第六课 linux命令详解
一．用户管理类命令
1．添加用户
格式：useradd 【选项】 用户名
例如：useradd david
添加名字为david的用户，当用户创建成功后，会在/home/目录下生成与用户名同名的目录。
2．删除用户
格式：userdel 【选项】用户名
例如：userdel -r david
删除名字为david的用户，“-r”选项会使/home目录下的该用户目录也被删除。
3．修改密码
格式：passwd 【选项】 用户名
例如：passwd david
修改david用户的密码。
4．切换用户
格式：su 【选项】 用户名
例如：su - root
切换到root用户，并将root用户的环境变量同时带入。
5．添加用户组
格式：groupadd 【选项】 用户组名字
例如：groupadd group1
添加组名为“group1”的用户组
6．删除用户组
格式：groupdel 【用户组名】
例如：groupdel group1
删除组名为“group1”的用户组。
二．文件与目录类命令
1．查看目录
格式：ls 【选项】 目录或文件
例如：
（1）ls /home
显示/home目录下的文件或者目录（不包含隐含文件）。
（1）ls -a /home    注意中间有空格的存在
显示/home目录下的所有文件与目录（含隐含文件）。
（2）ls -l /home
显示/home目录下的文件与目录的详细信息。
2．改变工作目录
格式：cd 目录名
例如：
（1）cd /home/
进入/home目录
（2）cd ..
退回到上一级目录
3．复制文件/目录
格式：cp 【选项】 原文件或目录，目标文件或者目录
例如：
（1）cp /home/test /tmp
将/home目录下的test文件copy到/temp目录下，注意不同文件之间的空格。
（2）cp -r /home/dir1 /tmp/
将/home目录下的dir1目录copy到/tmp目录下
4．移动或者更名
格式：mv 【选项】 原文件或者目录 目标文件或者目录
例如：
(1) mv /home/test /home/test1
将/home目录下的test文件更名为test1
(2) mv /home/dir1 /tmp/
将/home目录下dir1目录移动（剪切）到/tmp目录下
5．删除文件/目录
格式：rm 【选项】文件或者目录
例如：
（1）rm /home/test
删除/home目录下的test文件
（2）rm -r /home/dir
删除/home目录下的dir目录
6．改变文件权限
格式：chmod  [who]  [+ | - | =]  [mode]  文件名
参数：
who：u表是文件的所有者
      g表示与文件所有者同组的用户
o表示“其他用户”
a表示“所有用户”。它是系统的默认值
mode：
+添加某个权限
-取消谋和权限
=赋予给定的权限
例如：chmod  g+w  hello.c
7．创建目录
格式：mkdir 【选项】 目录名
例如：
（1）mkdir /home/workdir
在/home目录下创建workdir目录
（2）mkdir -p /home/dir1/dir2
创建/home/dir1/dir2目录，如果dir1不存在，就先创建dir1
8．查看目录大小
格式：du 目录名
例如：du /tmp
9．查看当前的目录途径
格式：pwd
例如：pwd
显示当前工作目录的绝对路径
三．压缩打包类命令
Linux环境中，压缩文件的扩展名通常为*.tar;  *.tar.gz;  *.tgz:  *.gz;  *.bz2。不同的后缀的文件，需要使用不同的命令来处理。
 *.gz      gzip命令压缩的文件
*.bz2     bzip2命令压缩的文件
*.tar   tar压缩的文件
*.tar.gz   tar打包后，再进过gzip压缩的文件
*.tar.bz2   tar命令打包后，在经过bzip2压缩的文件
压缩解压--gzip
格式：gzip 【选项】 文件名
例如：
（1）gzip /root/anaconda-ks.cfg
压缩文件
（2）gzip -d /root/anaconda-ks.cfg.gz
解压文件
压缩解压--bzip2
格式：bzip2 【选项】 文件名
例如：
（1）bzip2 /root/anaconda-ks.cfg
压缩文件
（2）Bzip2 -d /root/anaconda-ks.cfg.bz2
解压文件
打包压缩
格式：tar 【选项】 目录或者文件
例如：
（1）tar cvf tmp.tar /tmp
将tmp目录打包为一个文件
（2）tar xvf tmp.tar
将打包文件解压
（3）tar cvzf tmp.tar.gz /tmp
将tmp目录打包为一个文件
（4）tar xvzf tmp.tar.gz
将打包文件解开
四．磁盘管理类命令
1．加载光盘--mount
格式：mount 【选项】 设备名 挂载目录
例如：mount  /dev/cdrom  /mnt
这里需要说明，当在windows系统中操作时，我们插入优盘，插入光驱，就会显示，我们就可以直接访问。但是，这在，linux系统中是不行的，我们插入外设时，仅仅只会显示一个文件，这个文件是无法直接打开的，我们要将它挂载到一个目录上才可以。这个挂载目录我们通常会选取，根目录下面的mnt目录。
当我们插入光盘时，光盘所对应的文件就会时，dev目录下的cdrom，我们挂载光盘也是用的ios文件，这里要是挂载之后没有显示，可以重新启动一下。处理正确的话我们会在主界面看到光盘的显示，我们只需要运行上述代码即可。
2．卸载光盘--umount
格式：umount 挂载目录
例如：umount  /mnt
重要提示：请在执行umount之前，保证用户已经退出了mnt目录。
3．查看磁盘
格式：df【选项】
例如：df -hl
4．创建新文件--touchu
格式：touch 文件名
例如：touch  newfile
创建一个名字为“newfile”的空白文件
5．文件链接-ln
在linux系统中，才在类似于windows中“开解方式”的链接文件。链接文件又分为“软连接”和“硬链接”。
格式：ln【选项】文件名 链接名
例如：
（1）ln oldfile hilink
为oldfile文件创建一个硬链接hlink
（2）ln -s oldfile slink
为oldfile文件创建一个软连接slink
l 硬链接与软连接的区别
五．查找类命令
1．查找文件/目录--find
格式：find 查找路径 -name 文件名
例如：find /root/ -name install.log
在root目录下寻找名字为install.log的文件
2．查找字符串
格式：grep “字符串” 路径 【选项】
例如：
（1）grep “hello” ./ -rm
在当路径下寻找包含字符串“hello”的文件
（2）Grep “he*” ./ -rn
在当前的路径下寻找包含以“he”开头的字符串的文件
六．文件管理类命令
1．查看进程--PS
格式：ps 【选项】
例如：ps  aux
2．杀死进程--kill
格式：kill 【选项】 pid
例如：
vi hello.c
ps aux
Kill
3．查看内存--free
格式：free 【选项】
例如：free
4．CPU占有率--top
格式：top 【选项】
例如：top
七．杂项命令大聚合
1．软件管理--rpm
格式：rpm 【选项】【软件包】
例如：
（1）rpm -ivh  /mnt/Packages/xinet-2.3.14-34.el6.i686.rpm
安装xinet-2.3.14-34.el6.i686.rpm
（2）rpm -qa
查看系统中已经安装的所有软件包
（3）rpm -e xinet-2.3.14-34.el6.i686
卸载已经安装的xinet-2.3.14-34.el6.i686
2．管道操作--|
格式：命令1|命令2
例如：rpm -qa|grep tar
查询系统中安装的所有软件包，并从中找出带有字符串“tar”的。
3．查看帮助--man
格式：man 命令名
例如：man  find
查看find命令的用法
4．关闭linux系统
格式：shutdown 【选项】 时间
例如；shutdown now