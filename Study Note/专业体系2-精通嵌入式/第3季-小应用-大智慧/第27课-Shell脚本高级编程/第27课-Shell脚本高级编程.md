第三季-第27课-Shell脚本高级编程 

第27课-Shell脚本高级编程
 
 
28.1 什么是脚本
脚本是一个包含一系列命令序列的文本文件（可以通过编辑器进行编辑，而且可以执行）。当运行这个脚本文件时，文件中包含的命令序列将得到自动执行。例如我们创建7个文件，可以不断的创建7个文件，也可以通过一个脚本文件(.sh文件)来创建。
 
28.2 脚本编程技术
1. 基本结构
（1）解析器：#！后面的部分，表示该脚本用哪个解析器来操作；
（2）命令序列：由众多的命令组成。
2. 变量
shell脚本允许用户设置和使用自己的变量，变量可以使数字或者字符串，用户无需指定其类型，也无需在使用前定义。赋值号的左右两边不用空格，注释用#开头。
#!/bin/sh
a=”hello world”
b=5
echo “A is: $a”
echo “B is %b”
注：在运行该程序之前我们会发现有错，是因为我们使用的是windows下的文件和linux下的文件不同。所以，我们通过dos2unix shell.sh进行转换。我们在转换之前要安装相应的rpm包。
3. 参数
同C程序一样，shell脚本中可以使用命令行参数
$#：传入脚本的命令行参数个数
$*：所有命令行参数值，在各个参数值之间留有空格
$0：命令本身（shell本身）
$1：第一个命令行参数
$2：第二个命令行参数
例如：
 
#2.3
echo "number of var:" $#
echo "value of vars" $*
echo "name of script" $0
echo "value of var1" $1
echo "value of var2" $2
我们输入命令./shell.sh a 3后，显示的是
number of var:2
value of vars a 3
name of script ./shell.sh
value of var1 a
value of var2 3
4. 数字计算
shell脚本提供了一个专门的命令expr来计算数学表达式，例如expre 5 + 1，但是当要把计算的结果复制给其他变量的时候，还必须使用反引号。~线下面的符号就是`
var = `expr 20/10`
例如：
#2.4
var1=10
var2=20
var3=`expr $var1 / $var2`
var4=`expr $var1 + $var2`
echo $var3
echo $var4
运行结果：0
          30
5. 流程控制
（1）shell脚本中最基本的流程控制语句就是if-then，使用格式：
if[ condition ]（特别提醒：方括号左右两边必须有空格）
then
commands
else
commands
fi
l  条件比较
比较操作
整数操作
字符串操作
相同
-eq
=
不同
-ne
!=
大于
-gt
> 
小于
-lt
< 
大于或等于
-ge
 
小于或等于
-le
 
为空
-z
 
不为空
-n
 
例：
比较整数a和b是否相等：if [ $a = $b]（也可用eq）
判断整数a是否大于整数b：if [ $a -gt $b]
比较字符串a和b是否相等：if [ $a = $b]
判断字符串a是否为空：if [ -z $a]
判断整数变量a是否大于b：if [ $a -gt $b]
注意：
l  在括号的左右都留有空格
l  “=”左右都有空格
 
针对文件目录的
-e 文件已经存在
-f 文件是普通文件
-s 文件大小不为零
-d 文件是一个目录
-r 文件对当前用户可以读取
-w 文件对当前用户可以写入
-x 文件对当前用户可以执行
#!/bin/sh
folder=/home
[ -r “$folder” ] && echo “Can read $folder”   当前面条件为真的时候执行后面的操作
[ -f “$folder” ] || echo “this is not file”       当前面的条件为假的时候执行后面的操作
 
例：
var=10
if [ $var -gt 5 ]
then 
    echo "the value is qreater then 5"
fi
运行结果：the value is qreater then 5
（2）for语句
shell脚本提供了for语句，用于创建类似于c语言编程中for语句一样的循环。使用格式：
for var in list
do 
commands
done
例：
#!/bin/bash
list="Sun Mon Tue Wed Thur Fri Sat"
for day in $list
do 
    echo $day
done
运行结果：
Sun 
Mon 
Tue 
Wed 
Thur 
Fri 
Sat
（3）while语句
shell脚本提供了while语句，用于创建类似于c语言编程中while语句一样的循环。使用格式：
while condition
do
commands
done
例：
#!/bin/bash
var=5
while [ $var -gt 0 ]
do 
    echo $var
    var=`expr $var - 1`
done
运行结果：
5
4
3
2
1
 
6. sed编辑器
Sed（Stream Editior）：流式编辑器又名行(hang)编辑器，每次只编辑一行，格式如下：
sed [操作选项] ‘命令’ 文件名
常用操作选项：
-n：指定处理后只显示该行
-e：进行多项编辑任务
-i：直接修改读取的文件内容，而不是由屏幕输出
l  常用命令选项：
p       打印匹配行
a       新增
c       替代一行
d       删除定位行
s        代替一行中的某些部分
l  sed范例
这些修改除了加-i都是对原文件没有影响的。
（1）显示
sed -n ‘2p’ tmp.txt 只显示第2行
sed -n ‘1,3p’ tmp.txt 打印第1行到第3行-F
sed -n ‘/mov/’p tmp.txt 打印含movie的行
（2）删除
sed ‘2d’ tmp.txt 删除第2行（会打印出来剩下的部分）
sed ‘3,$d’ tmp.txt 删除第3行到最后一行，$：最后一行
（3）查询
sed -n ‘/hello/p’ tmp.txt 查询包括关键字hello的所有行
（4）代替
sed ‘1c Hi’ tmp.txt 第1行代替为Hi
sed -n ‘/hello/p’ tmp.txt |sed ‘sed ‘s/hello/bird/g’ 
（5）插入
sed -i ‘$a bye’ tmp.txt 在最后一行插入bye
 
7. awk分析器
awk是一个强大的文本分析工具，它把文件逐行的读入，以空格为默认分隔符将每行分割为多个字段。
使用方式：awk  ‘{pattern + action}' filenames
pattern 要查找的内容
action 找到匹配内容时所执行的命令
范例：
last -n 5|awk  ‘{print $1}’      last -n 5表示最近登陆的5个用户。 
cat /etc/passwd | awk -F ‘:’ ‘{print $1}’      -F表示指定分隔符
awk –F  ‘:’  ‘$1==”root” {print $0}’ /etc/passwd
 
28.3 综合实例
qcd分析
qcd.sh
qcd ()
{
        mkdir -p /.qcd/tmp 
    [ -x /.qcd/tmp/qcd_tmp ] || touch /.qcd/tmp/qcd_tmp 
    chmod 700 /.qcd/tmp
    QD=/.qcd/tmp/qcd_tmp
    export QD
    /usr/local/sbin/qcde "$@"
    [ ! `cat $QD` ] || cd "`cat $QD`"
    rm -f "$QD"
        unset QD;
}
 
install
#!/bin/sh
#qcd install 
#written by xiewei 2004 
 
setup_content=/etc/qcd
history_dir=$setup_content/history_dir
bin_content=/usr/local/sbin
prof_content=/etc/profile.d
 
setup()
{
    #check 
    if [ -r $history_dir ]      #如果$history_dir 对当前用户可读
    then   
    echo -n "You have installed qcd , overwrite it(y\Y or q\Q or n\N)? "
    while read choice
    do
        if [ "$choice" = "y"  -o  "$choice" = "Y" ] #要改写安装路径
        then
            break
        fi 
 
        if [ "$choice" = "q"  -o  "$choice" = "Q" ]  #什么也不做退出
        then
            echo "Nothing to do!"
            exit 1
        fi
               
        if [ "$choice" = "n"  -o  "$choice" = "N" ]  #安装到默认的位置
        then
            cp -f ./qcd $bin_content/
        cp -f ./qcd.sh $prof_content/
        echo "install qcd OK, but do not overwrite it!"
            echo "version is 1.00"
            exit 1
        fi 
 
        echo -n "You have installed qcd, overwrite it(y\Y or q\Q or n\N)? " 
    done
    fi
    
    if [ -r $setup_content ]
    then
    :
    else
    mkdir $setup_content
    fi
    
    cp -f ./qcde $bin_content/
    cp -f ./history_dir $setup_content/
    cp -f ./qcd.sh $prof_content/
}
 
delete()
{
    [ -r $history_dir ] || [ -r $bin_content ] || \
    ! echo "Your computer has not qcd!" || ! echo "Nothing to do!"
    
    echo -n "Are you sure to delete qcd(y\Y or q\Q)? "
    while read choice
    do
        if [ "$choice" = "y"  -o  "$choice" = "Y" ]  
        then
            break
        fi 
    
    if [ "$choice" = "q"  -o  "$choice" = "Q" ]  
    then
        echo "Nothing to do!"
        exit 1
    fi
    
    echo -n "Are you sure to delete qcd(y\Y or q\Q)? "
    done
 
    rm -rf $setup_content
    rm -f  $bin_content/qcd
    rm -f  $prof_content/qcd.sh
}
 
usage()
{
    echo "<install> install qcd on your computer."
    echo "<install del> delete qcd from your computer."
    exit 1
}
 
echo "Qcd Install Software"
echo "Written By XieWei 2004" 
 
if [ $# -eq 0 ]    #如果传入的参数个数为0?then
    setup
    echo "install qcd OK!"
    echo "version is 1.00"
    exit 1
fi    
 
if [ $# -gt 1 ]    #如果传入的参数个数大于0
then
    usage
fi    
 
case $1 in        #如果传入的第一个参数是del
del)
    delete
    echo "have delete qcd OK!"
    ;;
*)
    usage    #提示信息
    ;;
esac
 
 