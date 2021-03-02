#include “setup.h”              

static struct tag *params;               //定义个tag结构体变量params指针

void setup_start_tag (void)              //开始tag
{
	params = (struct tag *) 0x30000100;  //tag起始地址等于0X30000100
	params->hdr.tag = ATAG_CORE;         //头部常量tag=0x54410001
	params->hdr.size = tag_size (tag_core);    //size=5,

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;
	params = tag_next (params);     //parmas=( struct tag *)((u32 *)parmas+ params->hdr.size)  
}

// setup_start_tag (bd)保存tag参数如下:
setup_memory_tags (void)                //内存tag
{

int i;
params->hdr.tag = ATAG_MEM;               //头部常量tag=0x54410002
params->hdr.size = tag_size (tag_mem32);       //size=4
params->u.mem.start = 0x30000000;            //SDRAM起始地址
params->u.mem.size = 0x4000000;             //SDRAM内存大小64M
params = tag_next (params);                  //指向下个tag
}  

// setup_memory_tag s(bd)保存tag参数如下:
int strlen(char  *str)          //uboot不依赖任何库,所以需要自己写strlen函数
{
   int  i=0;
   while(str[i])
   {
    i++;
   }
return i;
}

void strcpy(char  *dest, char  *src)
{  
   while((*dest++=*src++)!=’\0’&&*dest!=’\0’);  
}

 

setup_commandline_tag (char  *cmdline)     //命令行tag
/**cmdline :指向命令行参数                                                             */
/*一般为:“boottargs=noinitrd root=/dev/mtdblock3 init=/linuxrc console=ttySAC0”     */
{
 int len=strlen(cmdline)+1;                       //计算cmdline长度,并加上结束符
params->hdr.tag = ATAG_CMDLINE;             //头部常量tag=0x54410009
params->hdr.size =(sizeof (struct tag_header) +len+3) >> 2;    /*size=(字符串长度+头部长度) >>2 */
/*“+3”表示:按4字节对齐,比如当总长度=(1,2,3,4)时,size=(总长度+3)>>2=1，实现4字节对齐         */ 
strcpy (params->u.cmdline.cmdline, cmdline);      //复制形参字符串到params->u.cmdline.cmdline
params = tag_next (params);                    //执行下个tag
}

 
setup_end_tag (void)                            //结束tag
{
 params->hdr.tag = 0;                   
 params->hdr.size = 0;
}





void main(void)

{

void (*theKernel)(int zero, int arch, unsigned int params); 

/*定义一个函数指针theKernel,其中第一个参数zero:0           */

/* arch:机器ID ,由于芯片类型很多,内核为了辨别芯片而定义的机器ID，其中2440芯片的ID号是362,*/

/* params :tag参数位置,这里我们的tag起始地址=0x30000100*/

  /*1 初 始 化 串 口 0 , 使 内 核 能 打 印 信 息  */

  uart0_init();                                             //调用serial.h头文件里的uart0_init()
  puts(“uart0 init OK\r\n”);                                //打印uart0初始化  

  /*2从  nand   flash  里 把 内 核 复 制 到  SDRAM  中 */

  puts(“copy  kernel  from  nand\r\n”);                  //打印内核复制
  nand_read((0x60000+64),0X30008000,0X200000);              //烧写2MB,多烧写点避免出错       

/*

0x60000+64:表示内核在nand(存储)地址上位置,

0X30008000:内核在sdram(运行)地址上位置

0X200000:内核长度2MB

因为Flash上存的内核格式是：uImage(64B头部(header)  + 真正的内核 )

在uboot界面中输入mtd命令可以看到:

              kernel分区位于 nand的0X00060000~0x00260000

所以在nand中真正的内核地址=0x60000+64,

在uboot界面中输入boot命令可以看到:

            Data Size:    1848656 Bytes =1.8 MB

            Load Address: 30008000

所以内核目的地址=0X30008000

长度=1.8MB

*/

  /*3 设  置  T  A  G  参  数        */

 puts(“set  boot  params\r\n”);                  //打印设置参数信息
 setup_start_tag (void);                      //在0X30000100地址保存start_tag数据，
 setup_memory_tags (void);                      //保存memory_tag数据,让内核知道内存多大
 setup_commandline_tag (“boottargs=noinitrd root=/dev/mtdblock3 init=/linuxrc console=ttySAC0”);  
 /*保存命令行bootargs参数,让内核知道根文件系统位置在/dev/mtdblock3,指定开机运行第一个脚本/linuxrc,指定打印串口0*/
 setup_end_tag (void);                            //初始化tag结构体结束

 
 /*  4 跳 转 执 行          */

   puts(“boot   kernel\r\n”);                      //打印启动内核
   theKernel = (void (*)(int, int, unsigend int))0x30008000;
    // 设置theKernel地址=0x30008000,用于后面启动内核
   theKernel(0,362,0x300000100);        //362:机器ID，  0x300000100: params(tag)地址
/*传递参数跳转执行到0x30008000启动内核,           */
/*相当于: mov r0,#0                             */
/*ldr r1,=362                                  */
/*ldr r2,= 0x300000100                         */
/*mov pc,#0x30008000                           */
  puts(“kernel  ERROR\r\n”);                  //打印内核启动出错

}
