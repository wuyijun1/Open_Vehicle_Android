第四季-专题18-FLASH驱动程序设计 

专题18-FLASH驱动程序设计
第1课-块设备驱动系统架构
块设备快速体验
块设备是指只能以块为单位进行访问的设备，块大小一般是512个字节的整数倍。常见的块设备包括硬件，SD卡，光盘等。
l  insmod simple-blk.ko
l  ls /dev/simp_blkdev0
l  mkfs.ext3 /dev/simp_blk0
l  mkdir –p /mnt/blk
l  mount /dev/simp_blk0 /mnt/blk
l  cp /etc/init.d/* /mnt/blk
l  ls /mnt/blk
l  umount /mnt/blk
l  ls /mnt/blk
块设备驱动系统架构
 
（1）     系统架构-VFS
VFS是对各种具体文件系统的一种封装，为用户程序访问文件提供统一的接口。
 
（2）     系统架构-cache
当用户发起文件访问请求的时候，首先会到Disk Cache中寻找文件是否被缓存了，如果在cache中，则直接从cache中读取。如果数据不在缓存中，就必须要到具体的文件系统中读取数据了。
（3）     Mapping Layer
l  首先确定文件系统的block size,然后计算所请求的数据包含多少个block。
l  调用具体文件系统的函数来访问文件的inode结构，确定所请求的数据在磁盘上的地址。
（4）     Generic Block Layer
Linux内核把把块设备看作是由若干个扇区组成的数据空间。上层的读写请求在通用块层被构造成一个或多个bio结构。
（5）     I/O Scheduler Layer
I/O调度层负责采用某种算法(如：电梯调度算法)将I/O操作进行排序。
 
电梯调度算法的基本原则：如果电梯现在朝上运动，如果当前楼层的上方和下方都有请求，则先响应所有上方的请求，然后才向下响应下方的请求；如果电梯向下运动，则刚好相反。
（6）在块系统架构的最底层，由块设备驱动根据排序好的请求，对硬件进行数据访问。
第2课-块设备驱动系统实例分析
 
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
 
#include <linux/sched.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>   /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/timer.h>
#include <linux/types.h> /* size_t */
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/hdreg.h> /* HDIO_GETGEO */
#include <linux/kdev_t.h>
#include <linux/vmalloc.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h> /* invalidate_bdev */
#include <linux/bio.h>
 
MODULE_LICENSE("Dual BSD/GPL");
 
static int major = 0;
 
static int sect_size = 512;
 
static int nsectors = 1024;
 
/*
* The internal representation of our device.
*/
struct blk_dev{
         int size;                        /* Device size in sectors */
         u8 *data;                        /* The data array */
         struct request_queue *queue;     /* The device request queue */
         struct gendisk *gd;              /* The gendisk structure */
};
 
struct blk_dev *dev;
 
 
/*
* Handle an I/O request, in sectors.
*/
static void blk_transfer(struct blk_dev *dev, unsigned long sector,
   unsigned long nsect, char *buffer, int write)
{
unsigned long offset = sector*sect_size;
unsigned long nbytes = nsect*sect_size;
 
if ((offset + nbytes) > dev->size) {
   printk (KERN_NOTICE "Beyond-end write (%ld %ld)\n", offset, nbytes);
   return;
}
if (write)
   memcpy(dev->data + offset, buffer, nbytes);
else
   memcpy(buffer, dev->data + offset, nbytes);
}
 
/*
* The simple form of the request function.
*/
static void blk_request(struct request_queue *q)
{
struct request *req;
 
req = blk_fetch_request(q);
while (req != NULL) {
   struct blk_dev *dev = req->rq_disk->private_data;
 
   blk_transfer(dev, blk_rq_pos(req), blk_rq_cur_sectors(req), req->buffer, rq_data_dir(req));
  
   if(!__blk_end_request_cur(req, 0))
   {
    req = blk_fetch_request(q);
   }
}
}
 
/*
* The device operations structure.
*/
static struct block_device_operations blk_ops = {
.owner            = THIS_MODULE,
};
 
 
/*
* Set up our internal device.
*/
static void setup_device()
{
/*
* Get some memory.
*/
dev->size = nsectors*sect_size;
dev->data = vmalloc(dev->size);
if (dev->data == NULL) {
   printk (KERN_NOTICE "vmalloc failure.\n");
   return;
}
 
   dev->queue = blk_init_queue(blk_request, NULL);
   if (dev->queue == NULL)
    goto out_vfree;
 
blk_queue_logical_block_size(dev->queue, sect_size);
dev->queue->queuedata = dev;
/*
* And the gendisk structure.
*/
dev->gd = alloc_disk(1);
if (! dev->gd) {
   printk (KERN_NOTICE "alloc_disk failure\n");
   goto out_vfree;
}
dev->gd->major = major;
dev->gd->first_minor = 0;
dev->gd->fops = &blk_ops;
dev->gd->queue = dev->queue;
dev->gd->private_data = dev;
sprintf (dev->gd->disk_name, "simp_blk%d", 0);
set_capacity(dev->gd, nsectors*(sect_size/sect_size));
add_disk(dev->gd);
return;
 
out_vfree:
if (dev->data)
   vfree(dev->data);
}
 
static int __init blk_init(void)
{
/*
* Get registered.
*/
major = register_blkdev(major, "blk");
if (major <= 0) {
   printk(KERN_WARNING "blk: unable to get major number\n");
   return -EBUSY;
}
 
dev = kmalloc(sizeof(struct blk_dev), GFP_KERNEL);
if (dev == NULL)
   goto out_unregister;
 
   setup_device();
    
return 0;
 
out_unregister:
unregister_blkdev(major, "sbd");
return -ENOMEM;
}
 
static void blk_exit(void)
{
 
   if (dev->gd) {
    del_gendisk(dev->gd);
    put_disk(dev->gd);
   }
   if (dev->queue)
    blk_cleanup_queue(dev->queue);
   if (dev->data)
    vfree(dev->data);
 
   unregister_blkdev(major, "blk");
kfree(dev);
}
 
module_init(blk_init);
module_exit(blk_exit);
 
 
第3课-简单块设备驱动设计
第4课-MTD系统架构
MTD设备体验
FLASH在嵌入式系统中是必不可少的，它是bootloader、linux内核和文件系统的最佳载体。在Linux内核中引入了MTD子系统为NORFLASH和NAND FLASH设备提供统一的接口，从而使得FLASH驱动的设计大为简化。
MTD架构分析
 
第5课-yaffs2文件系统使用
MTD分区设置
Yaffs2文件系统制作
Uboot参数设置
下载烧写与启动
第6课-nandflash驱动程序设计