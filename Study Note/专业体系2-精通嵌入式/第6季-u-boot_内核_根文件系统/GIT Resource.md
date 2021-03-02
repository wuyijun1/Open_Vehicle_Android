# 用 git 下载 busybox 源码
git   clone https://git.busybox.net/busybox

# 用 git 下载 uboot 源码
1. clone 出仓库：
git clone git://git.denx.de/u-boot.git u-boot
1
2. 查看仓库的一些信息：
git status
1
3. 查看 release 版本(打的标签)：
git tag
1
4. 切换到特定的标签版本下：
git checkout v2016.01-rc1
1
5. 查看本标签与其他的标签的不同：
git diff v2016.01
# 用 git 下载 Linux 源码
下载指定分支
$ git clone https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git --depth 1 --branch linux-4.15.y
1
获取分支log信息
$ git fetch --unshallow    
1
添加其他远程分支
如果需要切换到其他分支，可以使用以下指令：

$ git remote set-branches --add origin linux-3.12.y
1
获取远程分支
$ git fetch --depth=1 origin linux-3.12.y
1
切换分支
$ git checkout linux-3.12.y
$ git fetch --unshallow
1
2
查看远程分支信息
$ git branch -r
1
现在我们的本地仓库已经拥有了两个linux-stable分支了
# GIT获取Android源码 [zz]
git clone git://android.git.kernel.org/kernel/common.git
