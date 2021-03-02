#define CLKDIVN		0X4C000014	/*设置FCLK:HCLK:PCLK的比例*/
#define MPLLCON		0x4C000004	/*设置FCLK频率*/
#define S3C2440_MPLL_200MHZ     ((0x5c<<12)|(0x01<<4)|(0x02))
#define S3C2440_MPLL_400MHZ     ((0x5c<<12)|(0x01<<4)|(0x01))

#define WTCON	0x53000000		/*看门狗寄存器*/
#define BWSCON	0X48000000		/*BANK寄存器*/

.text					/*设置代码段*/
.global _start			/*定义全局变量，要被链接脚本用到*/
_start:					/*_start跳转到这里实现硬件初始化*/

	/* 1.关看门狗*/
	ldr r0, =WTCON
	mov r1, #0
	str r1, [r0]

	/* 2.设置时钟（必须设为异步总线模式） */
	ldr r0, =CLKDIVN
	mov r1, #5			/* FCLK:HCLK:PCLK=1:4:8 */
	str r1, [r0]

	mrc	p15, 0, r1, c1, c0, 0	/*设置为asynchronous bus mode*/
	orr	r1, r1, #0xc0000000
	mcr	p15, 0, r1, c1, c0, 0
 
	ldr r0, =MPLLCON			/* 设置时钟频率FCLK为400MHZ */
	ldr r1, =S3C2440_MPLL_400MHZ
	str r1, [r0]

	/* 3.使能ICache，加快指令访问速度；因为目前没有开启MMU，所以还不能开启数据缓存DCache */
	mrc p15, 0, r0, c1, c0, 0	/* read control reg */
	orr r0, r0, #(1<<12)
	mcr	p15, 0, r0, c1, c0, 0   /* write it back */


	/* 4.初始化SDRAM */
	ldr r0, =BWSCON
	adr r1, sdram_config     	/* 使用adr跳转，因为SDRAM未初始化 */
	add r3, r0, #(13*4)
1:
	ldr r2, [r1], #4
	str r2, [r0], #4
	cmp r0, r3
	bne 1b						/*back to 1 when no equal*/

	/* 4.重定位 */
	ldr sp, =0x34000000			/*因为SDRAM大小为64MB，所以堆栈指针设在最高地址处*/

	bl nand_init

	mov r0, #0					/* src = r0 */
	ldr r1, =_start				/* dest = r1,_start为代码段的起始和链接地址0x30000000 */
	ldr r2, =__bss_start
	sub r2, r2, r1				/* len = r2,代码段长度*/
	
	bl copy_code_to_sdram		/* 调用C函数copy_code_to_sdram(src, dest, len)*/
	bl clear_bss				/* 清除bss段*/
	
/* 5.执行main */
	ldr lr, =halt
	ldr pc, =main
	mov pc，lr					/* 若main函数跳出后,使PC等于lr链接寄存器，避免程序跑飞 */
halt:
	b halt						/* 死循环,避免跑飞 */

sdram_config:
	.long 0x22011110	 //BWSCON
	.long 0x00000700	 //BANKCON0
	.long 0x00000700	 //BANKCON1
	.long 0x00000700	 //BANKCON2
	.long 0x00000700	 //BANKCON3  
	.long 0x00000700	 //BANKCON4
	.long 0x00000700	 //BANKCON5
	.long 0x00018005	 //BANKCON6
	.long 0x00018005	 //BANKCON7
	.long 0x008C04F4	 // REFRESH
	.long 0x000000B1	 //BANKSIZE
	.long 0x00000030	 //MRSRB6
	.long 0x00000030	 //MRSRB7
