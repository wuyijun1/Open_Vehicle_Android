第二季-专题13-NandFlash变硬盘 

专题3-NandFlash变硬盘
第1课-NandFlash原理解析
角色分析
在电脑中有硬盘，它是用来存储文件的。嵌入式系统是防电脑的系统，在嵌入式系统中，NandFlash就相当于硬盘的存在。
NandFlash的分类
2.1 根据物理结构上的区别，NandFlash主要分为如下两类：
l  SLC（Single Level Cell）：单层式存储
l  MLC（Muti Level Cell）：多层式存储
SLC在存储格上只存一位数据，而MLC则存放两位数据
2.2 MLC对比SLC
价格：由于MLC采用了更高密度的存储方式，因此同容量的MLC价格上远低于SLC
访问速度：SLC的访问速度一般要比MLC快3倍以上
使用寿命：SLC能进行10万次的擦写，MLC能进行1万次
功耗：MLC功耗比SLC高15%左右
访问方式
（1）       对比内存与NandFlash的编址区别
CPU内部有NandFlash控制，负责NandFlash与CPU之间的通信，通过地址、命令和数据三个寄存器来完成这一系列的通信。
（2）       NandFlash地址构成
一块NandFlash划分成多个Block，一个Block划分成多个page，一个page划分成两个部分。
（3）       信号引脚
CLE(Command Latch Enable): 命令锁存允许
ALE(Address Lactch Enable): 地址锁存允许
CE:芯片选择
RE:读允许
WE:写允许
WE:写允许
WP:在写或擦除期间，提供写保护
 R/B:读/忙
第2课-NandFlash驱动设计~读
#define NFCONF (*(volatile unsigned long*)0x4E000000)
#define NFCONT (*(volatile unsigned long*)0x4E000004)
#define NFSTAT (*(volatile unsigned char*)0x4E000020)
#define NFCMD  (*(volatile unsigned char*)0x4E000008)
#define NFADDR (*(volatile unsigned char*)0x4E00000C)
#define NFDATA (*(volatile unsigned char*)0x4E000010)
 
#define TACLS 1
#define TWRPH0 2
#define TWRPH1 1
 
 
void select_chip()
{
    NFCONT &= ~(1<<1);
}
 
void deselect_chip()
{
    NFCONT |= (1<<1);
}
 
void clear_RnB()
{
   NFSTAT |= (1<<2);
}
 
void send_cmd(unsigned cmd)
{
     NFCMD = cmd;
}
 
void send_addr(unsigned addr)
{
     NFADDR = addr;
}
 
void wait_RnB()
{
   while (!(NFSTAT&(1<<2)))  
   {
       ;
   }
}
 
void nand_reset()
{
    //选中flash
    select_chip();
   
    //清除RnB
    clear_RnB();
   
   
    //发送0xff命令
    send_cmd(0xff);
   
   
    //等待RnB
    wait_RnB();
   
   
    //取消选中flash
    deselect_chip();
}
 
void nand_init()
{
    //初始化NFCONF
    NFCONF = (TACLS<<12) | (TWRPH0<<8) | (TWRPH1<<4);
   
    //初始化NFCONT
    NFCONT = (1<<0) | (1<<1);
   
    //复位
    nand_reset();
}
 
void NF_PageRead(unsigned long addr,unsigned char* buff)
{
    int i;
   
    //选中nandflash芯片
    select_chip();
   
    //清除RnB
    clear_RnB();
   
    //发送命令0x00
    send_cmd(0x00);
   
    //发送列地址
    send_addr(0x00);
    send_addr(0x00);
   
    //发送行地址
    send_addr(addr&0xff);
    send_addr((addr>>8)&0xff);
    send_addr((addr>>16)&0xff);
   
    //发送命令0x30
    send_cmd(0x30);
   
    //等待RnB
    wait_RnB();
   
    //读取数据
    for(i=0;i<2048;i++)
    {
       buff[i] = NFDATA; 
    }
   
    //取消选中nandflash芯片
    deselect_chip();
}
 
 
void nand_to_ram(unsigned long start_addr, unsigned char* sdram_addr, int size)
{
     int i;
     
    for( i=(start_addr >>11); size>0;)
    {
        NF_PageRead(i,sdram_addr);
        size -= 2048;
        sdram_addr += 2048;
        i++;
    }
}
 
第3课-NandFlash驱动设计~写
 
#define NFCONF (*(volatile unsigned long*)0x4E000000)
#define NFCONT (*(volatile unsigned long*)0x4E000004)
#define NFSTAT (*(volatile unsigned char*)0x4E000020)
#define NFCMD  (*(volatile unsigned char*)0x4E000008)
#define NFADDR (*(volatile unsigned char*)0x4E00000C)
#define NFDATA (*(volatile unsigned char*)0x4E000010)
 
#define TACLS 1
#define TWRPH0 2
#define TWRPH1 1
 
 
void select_chip()
{
    NFCONT &= ~(1<<1);
}
 
void deselect_chip()
{
    NFCONT |= (1<<1);
}
 
void clear_RnB()
{
   NFSTAT |= (1<<2);
}
 
void send_cmd(unsigned cmd)
{
     NFCMD = cmd;
}
 
void send_addr(unsigned addr)
{
     NFADDR = addr;
}
 
void wait_RnB()
{
   while (!(NFSTAT&(1<<2)))  
   {
       ;
   }
}
 
void nand_reset()
{
    //选中flash
    select_chip();
   
    //清除RnB
    clear_RnB();
   
   
    //发送0xff命令
    send_cmd(0xff);
   
   
    //等待RnB
    wait_RnB();
   
   
    //取消选中flash
    deselect_chip();
}
 
void nand_init()
{
    //初始化NFCONF
    NFCONF = (TACLS<<12) | (TWRPH0<<8) | (TWRPH1<<4);
   
    //初始化NFCONT
    NFCONT = (1<<0) | (1<<1);
   
    //复位
    nand_reset();
}
 
void NF_PageRead(unsigned long addr,unsigned char* buff)
{
    int i;
   
    //选中nandflash芯片
    select_chip();
   
    //清除RnB
    clear_RnB();
   
    //发送命令0x00
    send_cmd(0x00);
   
    //发送列地址
    send_addr(0x00);
    send_addr(0x00);
   
    //发送行地址
    send_addr(addr&0xff);
    send_addr((addr>>8)&0xff);
    send_addr((addr>>16)&0xff);
   
    //发送命令0x30
    send_cmd(0x30);
   
    //等待RnB
    wait_RnB();
   
    //读取数据
    for(i=0;i<2048;i++)
    {
       buff[i] = NFDATA; 
    }
   
    //取消选中nandflash芯片
    deselect_chip();
}
 
 
void nand_to_ram(unsigned long start_addr, unsigned char* sdram_addr, int size)
{
     int i;
    
    for( i=(start_addr >>11); size>0;)
    {
        NF_PageRead(i,sdram_addr);
        size -= 2048;
        sdram_addr += 2048;
        i++;
    }
}
 
 
int NF_WritePage(unsigned long addr, unsigned char* buff)
{
    int ret;
    int i;
   
    //选中flash芯片
    select_chip();
   
    //清除RnB
    clear_RnB();
   
    //发送命令0x80
    send_cmd(0x80);
   
    //发送列地址
    send_addr(0x00);
    send_addr(0x00);
   
    //发送行地址
    send_addr(addr&0xff);
    send_addr((addr>>8)&0xff);
    send_addr((addr>>16)&0xff);
   
    //写入数据
    for(i=0;i<2048;i++)
    {
       NFDATA = buff[i];
    }
   
    //发送命令0x10
    send_cmd(0x10);
   
    //等待RnB
    wait_RnB();
   
    //发送命令0x70
    send_cmd(0x70);
   
    //读取写入结果
    ret = NFDATA;
   
    //取消选中flash芯片
    deselect_chip();
   
    return ret; 
}