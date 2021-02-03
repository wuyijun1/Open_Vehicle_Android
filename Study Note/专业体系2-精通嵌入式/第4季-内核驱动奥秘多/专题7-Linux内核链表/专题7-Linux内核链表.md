第四季-专题7-Linux内核链表 

专题7-Linux内核链表
1.链表简介
链表是一种常用的数据结构，它通过指针将一系列数据节点连接成一条数据链。相对于数组，链表具有更好的动态性，建立链表时无需预先知道数据总量，可以随机分配空间，可以高效地在链表中的任意位置实时插入或删除数据。链表的开销主要是访问的顺序性和组织链的空间损失。
传统链表与Linux内核链表的区别：
    以前我们学习的链表主要有，单项链表、双向链表、双向循环链表。我们用的Linux内核链表示双向循环链表。Linux内核链表创建的目的就是，使用链表是统一操作和统一的函数。内核链表同样有节点、
 
2．内核链表-结构   
struct list_head
{
struct list_head *next, *prev;
};
list_head结构包含两个指向list_head结构的指针prev和next，由此可见，内核的链表具备双链表功能，实际上，通常它都组织成双向循环链表。
（1）INIT_LIST_HEAD:创建链表
（2）list_add：在链表头插入节点
（3）list_add_tail：在链表尾插入节点
（4）list_del：删除节点
（5）list_entry：取出节点
（6）list_for_each：遍历链表
例子：
    我们要做的是在我们创建的内核模块中加入链表的内容。
创建新的makefile文件：
boj-m := mylist.o
KDIR := /home/free/part3/linux-TQ210
all:
    make -C $(KDIR) M=$(PWD) modules CROSS_COMPILE=arm-linux- ARCH=arm
创建新的mylist.c文件：
#include<linux/module.h>
#include<linux/init.h>
#include<linux/list.h>
 
struct score
{
    int num;
    int english;
    int math;
    struct list_head list;
};
 
struct list_head score_head;
struct score stu1,stu2,stu3;
struct list_head *pos;
struct score *tmp;
 
int mylist_init()
{
    INIT_LIST_HEAD(&score_head);
 
    stu1.num = 1;
    stu1.english = 91;
    stu1.math = 97;
    list_add_tail(&(stu1,list),&score_head);
 
    stu2.num = 2;
    stu2.english = 92;
    stu2.math = 98;
    list_add_tail(&(stu2,list),&score_head);
 
    stu3.num = 2;
    stu3.english = 93;
    stu3.math = 99;
    list_add_tail(&(stu3,list),&score_head);
 
    list_for_each(pos,&score_head)
    {
        tmp = list_entry(pos,struct score,list);
        printk("num %d,english is %d,math is %d\n",tmp_num,tmp-english,tmp-math);
        /*list_head指向节点的指针域，并不包含节点中的内容，为了使，指针指向结点的开头，
        我们使用函数list_entry，这样我们就能够通过pos指针查看，节点中的内容。*/
    }
 
    return 0;
}
void mylist_exit()
{
    //在退出的时候，我们可以从节点中删除结点。
    list_del(&(stu1,list));
    list_del(&(stu2,list));
    list_del(&(stu3,list));
}
 
module_init(mylist_init);
module_exit(mylist_exit);
运行：
    把编译好的mylist.ko文件导入开发板中，安装，这时就会显示每个学生的学号以及各科成绩。
 
3．分析
4.  在应用程序中移植内核链表
    