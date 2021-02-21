# [UBOOT启动内核过程 ](https://www.cnblogs.com/PengfeiSong/p/6407903.html)

## 1、摘要
（1）、启动4步骤
第一步：将内核搬移到DDR中
第二步：校验内核格式、CRC等
第三步：准备传参
第四步：跳转执行内核
（2）、涉及到的主要函数是：do_bootm和do_bootm_linux
（3）、uboot能启动的内核格式：zImage uImage fdt方式（设备树）
（4）、跳转与函数指针的方式运行内核
## 2、vmlinuz和zImage和uImage　
(1)uboot经过编译直接生成的elf格式的可执行程序是u-boot，这个程序类似于windows下的exe格式，在操作系统下是可以直接执行的。但是这种格式不能用来烧录下载。我们用来烧录下载的是u-boot.bin，这个东西是由u-boot使用arm-linux-objcopy工具进行加工（主要目的是去掉一些无用的）得到的。这个u-boot.bin就叫镜像（image），镜像就是用来烧录到iNand中执行的。
(2)linux内核经过编译后也会生成一个elf格式的可执行程序，叫vmlinux或vmlinuz，这个就是原始的未经任何处理加工的原版内核elf文件；嵌入式系统部署时烧录的一般不是这个vmlinuz/vmlinux，而是要用objcopy工具去制作成烧录镜像格式（就是u-boot.bin这种，但是内核没有.bin后缀），经过制作加工成烧录镜像的文件就叫Image（制作把78M大的精简成了7.5M，因此这个制作烧录镜像主要目的就是缩减大小，节省磁盘）。
(3)原则上Image就可以直接被烧录到Flash上进行启动执行（类似于u-boot.bin），但是实际上并不是这么简单。实际上linux的作者们觉得Image还是太大了所以对Image进行了压缩，并且在image压缩后的文件的前端附加了一部分解压缩代码。构成了一个压缩格式的镜像就叫zImage。（因为当年Image大小刚好比一张软盘（软盘有2种，1.2M的和1.44MB两种）大，为了节省1张软盘的钱于是乎设计了这种压缩Image成zImage的技术）。
(4)uboot为了启动linux内核，还发明了一种内核格式叫uImage。uImage是由zImage加工得到的，uboot中有一个工具，可以将zImage加工生成uImage。注意：uImage不关linux内核的事，linux内核只管生成zImage即可，然后uboot中的mkimage工具再去由zImage加工生成uImage来给uboot启动。这个加工过程其实就是在zImage前面加上64字节的uImage的头信息即可。
(4)原则上uboot启动时应该给他uImage格式的内核镜像，但是实际上uboot中也可以支持zImage，是否支持就看x210_sd.h中是否定义了LINUX_ZIMAGE_MAGIC这个宏。所以大家可以看出：有些uboot是支持zImage启动的，有些则不支持。但是所有的uboot肯定都支持uImage启动。
(5)如果直接在kernel底下去make uImage会提供mkimage command not found。解决方案是去uboot/tools下cp mkimage /usr/local/bin/，复制mkimage工具到系统目录下。再去make uImage即可。
## 3、do_bootm函数
　一般情况bootm命令对应do_bootm函数。如果在do前加下划线_do_xxx表示为内部调用函数
(1)命令名前加do_即可构成这个命令对应的函数，因此当我们bootm命令执行时，uboot实际执行的函数叫do_bootm函数，在cmd_bootm.c。
(2)do_bootm刚开始定义了一些变量，然后用宏来条件编译执行了secureboot的一些代码（主要进行签名认证），先不管他；然后进行了一些一些细节部分操作，也不管他。然后到了CONFIG_ZIMAGE_BOOT，用这个宏来控制进行条件编译一段代码，这段代码是用来支持zImage格式的内核启动的。
## 4、zImage启动细节
(1)do_bootm函数中一直到397行的after_header_check这个符号处，都是在进行镜像的头部信息校验。校验时就要根据不同种类的image类型进行不同的校验。所以do_bootm函数的核心就是去分辨传进来的image到底是什么类型，然后按照这种类型的头信息格式去校验。校验通过则进入下一步准备启动内核；如果校验失败则认为镜像有问题，所以不能启动。
LINUX_ZIMAGE_MAGIC
(1)这个是一个定义的魔数，这个数等于0x016f2818，表示这个镜像是一个zImage。也就是说zImage格式的镜像中在头部的一个固定位置存放了这个数作为格式标记。如果我们拿到了一个image，去他的那个位置去取4字节判断它是否等于LINUX_ZIMAGE_MAGIC，则可以知道这个镜像是不是一个zImage。
(2)命令 bootm 0x30008000，所以do_boom的argc=2，argv[0]=bootm argv[1]=0x30008000。但是实际bootm命令还可以不带参数执行。如果不带参数直接bootm，则会从CFG_LOAD_ADDR地址去执行（定义在x210_sd.h中）。
(3)zImage头部开始的第37-40字节处存放着zImage标志魔数，从这个位置取出然后对比LINUX_ZIMAGE_MAGIC。可以用二进制阅读软件来打开zImage查看，就可以证明。很多软件都可以打开二进制文件，如winhex、UltraEditor。
image_header_t
(1)这个数据结构是我们uboot启动内核使用的一个标准启动数据结构，zImage头信息也是一个image_header_t，但是在实际启动之前需要进行一些改造。hdr->ih_os = IH_OS_LINUX;
hdr->ih_ep = ntohl(addr);这两句就是在进行改造。
(2)images全局变量是do_bootm函数中使用，用来完成启动过程的。zImage的校验过程其实就是先确认是不是zImage，确认后再修改zImage的头信息到合适，修改后用头信息去初始化images这个全局变量，然后就完成了校验。
## 5、uImage启动
(1)LEGACY(遗留的)，在do_bootm函数中，这种方式指的就是uImage的方式。
(2)uImage方式是uboot本身发明的支持linux启动的镜像格式，但是后来这种方式被一种新的方式替代，这个新的方式就是设备树方式（在do_bootm方式中叫FIT）
(3)uImage的启动校验主要在boot_get_kernel函数中，主要任务就是校验uImage的头信息，并且得到真正的kernel的起始位置去启动。
总结1：uboot本身设计时只支持uImage启动，原来uboot的代码也是这样写的。后来有了fdt方式之后，就把uImage方式命令为LEGACY方式，fdt方式命令为FIT方式，于是乎多了写#if #endif添加的代码。后来移植的人又为了省事添加了zImage启动的方式，又为了省事把zImage启动方式直接写在了uImage和fdt启动方式之前，于是乎又有了一对#if #endif。于是乎整天的代码看起来很恶心。
总结2：第二阶段校验头信息结束，下面进入第三阶段，第三阶段主要任务是启动linux内核，调用do_bootm_linux函数来完成。
## 6、启动
(1)ep就是entrypoint的缩写，就是程序入口。一个镜像文件的起始执行部分不是在镜像的开头（镜像开头有n个字节的头信息），真正的镜像文件执行时第一句代码在镜像的中部某个字节处，相当于头是有一定的偏移量的。这个偏移量记录在头信息中。
(2)一般执行一个镜像都是：第一步先读取头信息，然后在头信息的特定地址找MAGIC_NUM，由此来确定镜像种类；第二步对镜像进行校验；第三步再次读取头信息，由特定地址知道这个镜像的各种信息（镜像长度、镜像种类、入口地址）；第四步就去entrypoint处开始执行镜像。
(3)theKernel = (void (*)(int, int, uint))ep;将ep赋值给theKernel，则这个函数指向就指向了内存中加载的OS镜像的真正入口地址（就是操作系统的第一句执行的代码）。
7、传参准备
(1)uboot在启动内核时，机器码要传给内核。uboot传给内核的机器码是怎么确定的？第一顺序备选是环境变量machid，第二顺序备选是gd->bd->bi_arch_num（x210_sd.h中硬编码配置的）
(2)从110行到144行就是uboot在给linux内核准备传递的参数处理。（uboot/lib_arm/bootm.c）
(2)Starting kernel ... 这个是uboot中最后一句打印出来的东西。这句如果能出现，说明uboot整个是成功的，也成功的加载了内核镜像，也校验通过了，也找到入口地址了，也试图去执行了。如果这句后串口就没输出了，说明内核并没有被成功执行。原因一般是：传参（80%）、内核在DDR中的加载地址·······
## 7、传参详解
（1）、tag方式传参
(1)struct tag，tag是一个数据结构，在uboot和linux kernel中都有定义tag数据机构，而且定义是一样的。
(2)tag_header和tag_xxx。tag_header中有这个tag的size和类型编码，kernel拿到一个tag后先分析tag_header得到tag的类型和大小，然后将tag中剩余部分当作一个tag_xxx来处理。
(3)tag_start与tag_end。kernel接收到的传参是若干个tag构成的，这些tag由tag_start起始，到tag_end结束。
(4)tag传参的方式是由linux kernel发明的，kernel定义了这种向我传参的方式，uboot只是实现了这种传参方式从而可以支持给kernel传参。
（2）、x210_sd.h中配置传参宏
(1)CONFIG_SETUP_MEMORY_TAGS，tag_mem，传参内容是内存配置信息。
(2)CONFIG_CMDLINE_TAG，tag_cmdline，传参内容是启动命令行参数，也就是uboot环境变量的bootargs.
(3)CONFIG_INITRD_TAG
(4)CONFIG_MTDPARTITION，传参内容是iNand/SD卡的分区表。
(5)起始tag是ATAG_CORE、结束tag是ATAG_NONE，其他的ATAG_XXX都是有效信息tag。
思考：内核如何拿到这些tag？
uboot最终是调用theKernel函数来执行linux内核的，uboot调用这个函数（其实就是linux内核）时传递了3个参数。这3个参数就是uboot直接传递给linux内核的3个参数，通过寄存器来实现传参的。（第1个参数就放在r0中，第二个参数放在r1中，第3个参数放在r2中）第1个参数固定为0，第2个参数是机器码，第3个参数传递的就是大片传参tag的首地址。
（3）、移植时注意事项
(1)`uboot移植时一般只需要配置相应的宏即可`
(2)`kernel启动不成功，注意传参是否成功。传参不成功首先看uboot中bootargs设置是否正确，其次看uboot是否开启了相应宏以支持传参`。