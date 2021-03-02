CC      = arm-linux-gcc               //定义CC变量=arm-linux-gcc,简化书写,编译命令,(*.C,*.S)文件生成*.O文件

LD      = arm-linux-ld                 //连接命令,将多个*.O文件生成 boot.elf       

AR      = arm-linux-ar                 //库管理命令,这里没有用到 

OBJCOPY = arm-linux-objcopy            //复制/格式转换命令, boot.elf生成boot.dis

OBJDUMP = arm-linux-objdump           //反汇编命令,boot.bin生成boot.dis

 
//GCC编译参数,-Wall:显示所有错误和警告, -O2:采用2级编译优化
CFLAGS           := -Wall -O2

//添加头文件参数,-nostdinc忽略缺省目录, -fno-builtin不连接系统标准启动文件和标准库文件（表示不用自带的strlen()等库函数）
CPPFLAGS     := -nostdinc -fno-builtin       


//定义objs变量，包含生成boot.bin目标文件需要的依赖文件
objs := start.o init.o boot.o 

//执行生成目标文件，首先是先满足objs所有依赖文件都拥有，才执行
boot.bin: $(objs)     
	${LD} -Tuboot.lds -o boot_elf $^
    ${OBJCOPY} -O binary -S boot_elf $@
    ${OBJDUMP} -D -m arm boot_elf > boot.dis

//-c编译不连接。$@表示目标文件   $<表示第一个依赖文件
%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<   
 
%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.bin *.elf *.dis *.o
