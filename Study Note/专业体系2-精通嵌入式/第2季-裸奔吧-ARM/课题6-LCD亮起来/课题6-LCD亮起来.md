# [第二季-专题16-LCD亮起来](https://www.cnblogs.com/free-1122/p/11452213.html) 

第二季-专题16-LCD亮起来 

专题6-LCD亮起来
第1课-LCD工作原理解析
LCD硬件体系架构
（1）     LCD液晶屏
 
液晶属于一种有机化合物，分子形状为长棒状，在不同的电流作用下，分子会做有规律的旋转，这样对光线（背光灯产生）产生了一定的控制，形成一个像素，而很多像素又可构成完整图象。
STN液晶屏；GF液晶屏；TFT液晶屏；TFD液晶屏
LCD是Liquid Crystal Display的简称。液晶显示器按驱动方式可分为STN，GF，TFT等种类，其中TFT屏在嵌入式开发中经常使用。
（2）     LCD驱动器
LCD驱动芯片是控制液晶分子发生变化的模块，它为液晶分子的偏转提供电压。通
常情况下，LCD驱动芯片都与液晶屏面板做到一个电路板上
（3）     LCD控制器
液晶要能正确的显示图像，除了要有LCD驱动芯片，还要有LCD控制器来给LCD驱动芯片提供必要的数据和控制信号.多数ARM处理器内部都集成有LCD控制器。
l  REGBANK: 由17个可编程的寄存器组和一块256*16的调色板内存组成，通过这些寄存器，可以配置LCD控制器.
l  LCDCDMA: 一个专用的DMA，它能自动地把在帧内存中的视频数据传送到LCD驱动器，通过使用这个DMA通道，视频数据在不需要CPU的干预的情况下显示在LCD屏上.
l  VIDPRCS: 接收来自LCDCDMA的数据，将数据转换为合适的数据格式，比如说4/8位单扫，4位双扫显示模式，然后通过数据端口VD[23:0]传送视频数据到LCD驱动器；
l  4.TIMEGEN: 由可编程的逻辑组成，他生成LCD驱动器需要的控制信号，比如VSYNC、HSYNC、VCLK和LEND等，而这些控制信号又与REGBANK寄存器组中的配置密切相关，通过不同的配置，TIMEGEN就能产生这些信号的不同值，从而支持不同的LCD驱动器(即不同的STN/TFT屏)。
LCD显示于信号
（1）     LCD显示流程
l  显示从屏幕左上角第一行的第一个点开始，一个点一个点地在LCD上显示，当显示到屏幕的最右边就结束这一行(Line)。
l  接下来显示指针又回到屏幕的左边从第二行开始显示，显示指针在从第一行的右边回到第二行的左边是需要一定的时间的，我们称之为行切换。
l  以此类推，显示指针就这样一行一行的显示至矩形的右下角才把一幅图像(帧：frame)显示完成，这一帧的显示时间在时序图上表示为VSYNC（垂直同步信号）。
（2）     LCD显示时序信号
 
?VSYNC：垂直同步信号
?HSYNC：水平同步信号
?VCLK：象素时钟信号
?VD[23:0]：LCD像素数据输出端口
?VDEN：数据使能信号
?LEND：行结束信号
 
第2课-2440LCD驱动程序设计
LCD初始化
（1）     引脚初始化
（2）     时序初始化
（3）     帧缓冲初始化
（4）     杂项初始化
LCD图像显示
（1）     数据显示
（2）     单色显示
 ```C
#define GPCCON (*(volatile unsigned long *)0x56000020)
#define GPDCON (*(volatile unsigned long *)0x56000030)
#define GPGCON (*(volatile unsigned long *)0x56000060)
#define LCDCON1 (*(volatile unsigned long *)0x4D000000)
#define LCDCON2 (*(volatile unsigned long *)0x4D000004)
#define LCDCON3 (*(volatile unsigned long *)0x4D000008)
#define LCDCON4 (*(volatile unsigned long *)0x4D00000C)
#define LCDCON5 (*(volatile unsigned long *)0x4D000010)
#define LCDSADDR1 (*(volatile unsigned long *)0x4D000014)
#define LCDSADDR2 (*(volatile unsigned long *)0x4D000018)
#define LCDSADDR3 (*(volatile unsigned long *)0x4D00001C)
#define TPAL (*(volatile unsigned long *)0x4D000050)
 
#define VSPW 9
#define VBPD 1
#define LINEVAL 271
#define VFPD 1
#define CLKVAL 4   //10 = 100/((CLKVAL+1)*2)
#define HSPW 40
#define HBPD 1
#define HOZVAL 479
#define HFPD 1
 
unsigned short LCDBUFFER[272][480];
 
typedef unsigned int U32;
typedef unsigned short U16;
typedef unsigned char U8;
 
void lcd_port_init()
{
    GPDCON = 0xaaaaaaaa;
    GPCCON = 0xaaaaaaaa;     
}
 
void lcd_control_init()
{  
    LCDCON1 = (CLKVAL<<8)|(0x3<<5)|(0xc<<1)|(0<<0);
    LCDCON2 = (VBPD<<24)|(LINEVAL<<14)|(VFPD<<6)|(VSPW);
    LCDCON3 = (HBPD<<19)|(HOZVAL<<8)|(HFPD);
    LCDCON4 = (HSPW);
    LCDCON5 = (1<<11)|(1<<9)|(1<<8);
   
    LCDSADDR1 = (((U32)LCDBUFFER>>22)<<21) | (((U32)LCDBUFFER>>1)&0x1fffff);
    LCDSADDR2 = (((U32)LCDBUFFER+272*480*2)>>1)&0x1fffff;
    LCDSADDR3 = (0<<11) | (480*2/2);
   
    TPAL = 0;
}
 
 
void lcd_init()
{
    lcd_port_init();
    lcd_control_init();
   
    //打开LCD电源
    GPGCON |= (0x3<<8);
    LCDCON5 |= (1<<3);  
    LCDCON1 |= 1;
}
 
void point(unsigned int x,unsigned int y,unsigned int color)
{
    unsigned int red, green, blue;
   
    red = (color>>19) & 0x1f;
    green = (color>>10) & 0x3f;
    blue = (color>>3) & 0x1f;
    LCDBUFFER[y][x] = (unsigned short)((red<<11)|(green<<5)|blue);
}
 
void Paint_Bmp(U16 x0,U16 y0,U16 wide,U16 high,const U8 *bmp)
{
    U16 x,y;
    U16 c;
    U32 p = 0;
   
    for( y = y0 ; y < y0+high ; y++ )
    {
    for( x = x0 ; x < x0+wide ; x++ )
    {
        c = bmp[p] | (bmp[p+1]<<8) ;                                      
       
        if ( ( x < 480) && ( y < 272) )
            LCDBUFFER[y][x] = c ;
       
        p = p + 2 ; 
    }
    }
}
 
void clearSrc(unsigned int color)
{
    TPAL = (1<<24)|(color&0xffffff);  
}
 
extern unsigned char bmp[90200];
 
void lcd_test()
{
    int x;
   
    for(x=0;x<480;x++)
        point(x++,150,0xff0000);
       
    Paint_Bmp(0,0,220,220,bmp);
   
    clearSrc(0xff0000);
}
```