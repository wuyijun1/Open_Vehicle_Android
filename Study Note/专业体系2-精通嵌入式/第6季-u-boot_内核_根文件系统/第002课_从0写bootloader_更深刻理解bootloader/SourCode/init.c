/* nand flash  时序 */#define TACLS      0
#define TWRPH0     1
#define TWRPH1     0

/*     nand  flash  寄存器       */
#define   NFCONF     *((volatile unsigend long *)0X4E000000); //配置寄存器(用来设置时序)
#define   NFCONT     *((volatile unsigend long *)0X4E000000); //控制寄存器(用来使能nandflash控制器以及ECC编码器,还有控制芯片使能CE脚)
#define   NFCMMD    *((volatile unsigend char *)0X4E000000);//发送命令寄存器(命令只有8位)
#define   NFADDR     *((volatile unsigend char *)0X4E000000);//发送地址寄存器(地址只有8位)
#define   NFDATA      *((volatile unsigend int *)0X4E000000);//读/写数据寄存器(数据只有8位)
#define   NFSTAT      *((volatile unsigend int *)0X4E000000);//运行状态寄存器(用于判断RnB脚)

/*因为Nand Flash只有8位I/O脚，所以NFCMMD/ NFADDR/ NFDATA三个寄存器值都是unsigend char型  */

void nand_init(void)
{

 /* 设置时序 */
NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);

/* bit4=1:初始化ECC,  bit1=1:禁止片选  bit0=1:启动nandflash控制器*/    
NFCONT = (1<<4)|(1<<1)|(1<<0);

}
void nand_select(void)               //使能片选
{
	int i;
	NFCONT&=~(1<<1);        // NFCONT控制器位1置0
	for(i=0;i<10;i++);           //等待芯片使能成功
}
void nand_deselect(void)                 //取消片选
{
	int i;
	NFCONT&=~(1<<1);        // NFCONT控制器位1置0
	for(i=0;i<10;i++);           //等待芯片使能成功
}
void nand_cmd(unsigned char cmd)
{
	volatile int i;
	NFCMMD = cmd;
	for (i = 0; i < 10; i++);
}
void nand_wait_ready(void)
{
	while (!(NFSTAT & 1));
}
unsigned char nand_data(void)
{
	return NFDATA;
}
void nand_addr(unsigned int addr)
{
	unsigned int col  = addr % 2048;
	unsigned int page = addr / 2048;
	volatile int i;

	NFADDR = col & 0xff;		/* A7~A0,第1周期 */
	for (i = 0; i < 10; i++);
    
	NFADDR = (col >> 8) & 0xff;	/* A10~A8,第2周期 */
	for (i = 0; i < 10; i++);
	
	NFADDR  = page & 0xff;		/* A18~A11,第3周期 */
	for (i = 0; i < 10; i++);
    
	NFADDR  = (page >> 8) & 0xff;	/* A26~A19,第4周期 */
	for (i = 0; i < 10; i++);
    
	NFADDR  = (page >> 16) & 0xff;	/* A27~A28,第5周期 */
	for (i = 0; i < 10; i++);	
}
/* 
* src:源地址,为32位地址,所以用unsigend int表示
* dest:目的地址内容,由于这里是将数据读出到目的地址内容中,所以需要用到*指针,
*	   因为每个地址里存的是一个字节,所以用unsigend char型 
*/
void nand_read(unsigned int  src,unsigned char  *dest,unsigned int  len)
{

int col = src % 2048;      //第一次读，可能不是读的页首地址,所以需要记录当前页的位置
int i=0;                //当前读了0次

nand_select();           //1使能片选nCE     
while(i<len)
{ 
    nand_cmd(0X00);         //2发送读命令0X00  
    nand_addr(src);     	// 3发送yuan地址(分为5个周期)
    nand_cmd(0X30);         //4发送读命令0X30    
    nand_wait_idle();       //5等待RnB信号为高电平

   for(;(col<2048)&&(i<len);col++)      //连续读页内数据
   {
       dest[i]=nand_read_data();            //6.读数据
       i++;
       src++;
   }
   col=0;
}
nand_deselect();                // 取消片选nCE
}
/**************************************************/
/* 				 	重定位函数					  */
/**************************************************/

/* 复制代码段（长度为len = __bss_start- _start）到SDRAM链接地址dest(0x30000000)处*/
void copy_code_to_sdram(unsigned char* src, unsigned char* dest, unsigned int len)
{	
	unsigned int i = 0;
	
	/* 判断nor启动还是nand启动 */
	if (isBootFromNorFlash())
	{
		while (i < len)			/*Nor启动，则直接复制*/
		{
			dest[i] = src[i];
			i++;
		}
	}
	else
	{
		nand_read((unsigned int)src, dest, len);
	}
}
/*******************************************************/
/* 				判断是NOR启动还是NAND启动				*/
/*******************************************************/

int isBootFromNorFlash(void)
{
	volatile int *p = (volatile int *)0;
	
	unsigned int tmp = *p;
	
	*p = 0x12345678;		/*向flash写*/
	if (*p == 0x12345678)
	{
		*p = tmp;			/* 若能成功写入,就是nand flash */
		return 0;
	}
	else
	{
		return 1;			/* 写不成功,就是NOR flash*/
	}
}
————————————————
/******************************************************/
/* 				 	bss段清除函数        			  */
/******************************************************/

void clear_bss(void)
{
	extern int __bss_start, __bss_end;
	int *p = &__bss_start;
	
	for (; p < &__bss_end; p++)
		*p = 0;
}
