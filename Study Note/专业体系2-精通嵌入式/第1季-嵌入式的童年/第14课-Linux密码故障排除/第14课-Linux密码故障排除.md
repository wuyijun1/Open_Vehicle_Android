嵌入式-第一季-第14课 

第十四课 linux密码故障排除
破解root用户密码的步骤：
在系统启动时进入grub选项菜单。
在grup选项菜单按e进入编辑模式
编辑kernel那行，添加/init 1.
按b重启
进入系统后，将root密码设置为空密码。
#vim  /etc/passwd
将第一行的root:x:0....中的X删除，变成root::0...然后保存退出。
重启，登录linux后修改root密码
#passwd  root