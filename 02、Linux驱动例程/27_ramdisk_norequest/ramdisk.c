/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: ramdisk.c
作者	  	: 正点原子
版本	   	: V1.0
描述	   	: 内存模拟硬盘，实现块设备驱动，本驱动使用请求队列。
其他	   	: 无
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2021/4/30 正点原子Linux团队
***************************************************************/
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>	
#include <linux/slab.h>		
#include <linux/fs.h>		
#include <linux/errno.h>	
#include <linux/types.h>	
#include <linux/fcntl.h>	
#include <linux/hdreg.h>
#include <linux/kdev_t.h>
#include <linux/vmalloc.h>
#include <linux/genhd.h>
#include <linux/blk-mq.h>
#include <linux/buffer_head.h>	
#include <linux/bio.h>

#define RAMDISK_SIZE	(2 * 1024 * 1024) 	/* 容量大小为2MB */
#define RAMDISK_NAME	"ramdisk"			/* 名字 */
#define RADMISK_MINOR	3					/* 表示有三个磁盘分区！不是次设备号为3！*/

/* ramdisk设备结构体 */
struct ramdisk_dev{
	int major;						/* 主设备号 					  */
	unsigned char *ramdiskbuf;		/* ramdisk内存空间,用于模拟块设备 */
	struct gendisk *gendisk; 		/* gendisk 						  */
	struct request_queue *queue;	/* 请求队列 					  */
	spinlock_t lock;				/* 自旋锁 						  */
};

struct ramdisk_dev *ramdisk = NULL;		/* ramdisk设备指针 */


/*
 * @description		: 打开块设备
 * @param - dev 	: 块设备
 * @param - mode 	: 打开模式
 * @return 			: 0 成功;其他 失败
 */
int ramdisk_open(struct block_device *dev, fmode_t mode)
{
	printk("ramdisk open\r\n");
	return 0;
}

/*
 * @description		: 释放块设备
 * @param - disk 	: gendisk
 * @param - mode 	: 模式
 * @return 			: 0 成功;其他 失败
 */
void ramdisk_release(struct gendisk *disk, fmode_t mode)
{
	printk("ramdisk release\r\n");
}

/*
 * @description		: 获取磁盘信息
 * @param - dev 	: 块设备
 * @param - geo 	: 模式
 * @return 			: 0 成功;其他 失败
 */
int ramdisk_getgeo(struct block_device *dev, struct hd_geometry *geo)
{
	/* 这是相对于机械硬盘的概念 */
	geo->heads = 2;			/* 磁头 */
	geo->cylinders = 32;	/* 柱面 */
	geo->sectors = RAMDISK_SIZE / (2 * 32 *512); /* 一个磁道上的扇区数量 */
	return 0;
}

/* 
 * 块设备操作函数 
 */
static struct block_device_operations ramdisk_fops =
{
	.owner	 = THIS_MODULE,
	.open	 = ramdisk_open,
	.release = ramdisk_release,
	.getgeo  = ramdisk_getgeo,
};

/*
 * @description	: “制造请求”函数
 * @param-q 	: 请求队列
 * @return 		: 无
 */
static blk_qc_t ramdisk_make_request_fn(struct request_queue *q, struct bio *bio)
{
	int offset;
	struct bio_vec bvec;
	struct bvec_iter iter;
	unsigned long len = 0;
	struct ramdisk_dev *dev = q->queuedata;

	offset = (bio->bi_iter.bi_sector) << 9;	/* 获取要操作的设备的偏移地址 */
	spin_lock(&dev->lock);	
	/* 处理bio中的每个段 */
	bio_for_each_segment(bvec, bio, iter){
		char *ptr = page_address(bvec.bv_page) + bvec.bv_offset;
		len = bvec.bv_len;

		if(bio_data_dir(bio) == READ)	/* 读数据 */
			memcpy(ptr, dev->ramdiskbuf + offset, len);
		else if(bio_data_dir(bio) == WRITE)	/* 写数据 */
			memcpy(dev->ramdiskbuf + offset, ptr, len);
		offset += len;
	}
	spin_unlock(&dev->lock);
    bio_endio(bio);
    return BLK_QC_T_NONE;
}

/*
 * @description	: 初始化队列相关操作
 * @set		 	: blk_mq_tag_set对象
 * @return 		: request_queue的地址
 */
static struct request_queue * create_req_queue(struct ramdisk_dev *set)
{
	struct request_queue *q;

	q = blk_alloc_queue(GFP_KERNEL);
	
	blk_queue_make_request(q, ramdisk_make_request_fn);
	
    q->queuedata = set;
	return q;
}

/*
 * @description	: 创建块设备，为应用层提供接口。
 * @set		 	: ramdisk_dev对象
 * @return 		: 0，表示成功;其它值为失败
 */
static int create_req_gendisk(struct ramdisk_dev *set)
{
	struct ramdisk_dev *dev = set;

	/* 1、分配并初始化 gendisk */
    dev->gendisk = alloc_disk(RADMISK_MINOR);
	if(dev == NULL)
		return -ENOMEM;
	
	/* 2、添加(注册)disk */
	dev->gendisk->major = ramdisk->major; /* 主设备号 */
	dev->gendisk->first_minor = 0;		  /* 起始次设备号 */
	dev->gendisk->fops = &ramdisk_fops;	  /* 操作函数 */
	dev->gendisk->private_data = set;	  /* 私有数据 */
	dev->gendisk->queue = dev->queue;	  /* 请求队列 */
	sprintf(dev->gendisk->disk_name, RAMDISK_NAME); /* 名字 */
	set_capacity(dev->gendisk, RAMDISK_SIZE/512);	/* 设备容量(单位为扇区)*/
	add_disk(dev->gendisk);
	return 0;
}

/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */
static int __init ramdisk_init(void)
{
	int ret = 0;
	struct ramdisk_dev * dev;
	printk("ramdisk init\r\n");
	
	/* 1、申请内存 */
	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if(dev == NULL) {
		return -ENOMEM;
	}
	
	dev->ramdiskbuf = kmalloc(RAMDISK_SIZE, GFP_KERNEL);
	if(dev->ramdiskbuf == NULL) {
		printk(KERN_WARNING "dev->ramdiskbuf: vmalloc failure.\n");
		return -ENOMEM;
	}
	ramdisk = dev;
	
	/* 2、初始化自旋锁 */
	spin_lock_init(&dev->lock);

	/* 3、注册块设备 */
	dev->major = register_blkdev(0, RAMDISK_NAME); /* 由系统自动分配主设备号 */
	if(dev->major < 0) {
		goto register_blkdev_fail;
	}
	
	/* 4、创建多队列 */
	dev->queue = create_req_queue(dev);
	if(dev->queue == NULL) {
		goto create_queue_fail;
	}
	
	/* 5、创建块设备 */
	ret = create_req_gendisk(dev);
	if(ret < 0)
		goto create_gendisk_fail;
	
    return 0;

create_gendisk_fail:
	blk_cleanup_queue(dev->queue);
create_queue_fail:
	unregister_blkdev(dev->major, RAMDISK_NAME);
register_blkdev_fail:
	kfree(dev->ramdiskbuf);
	kfree(dev);
	return -ENOMEM;
}

/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */
static void __exit ramdisk_exit(void)
{
	
	printk("ramdisk exit\r\n");
	/* 释放gendisk */
	del_gendisk(ramdisk->gendisk);
	put_disk(ramdisk->gendisk);

	/* 清除请求队列 */
	blk_cleanup_queue(ramdisk->queue);
	
	/* 注销块设备 */
	unregister_blkdev(ramdisk->major, RAMDISK_NAME);

	/* 释放内存 */
	kfree(ramdisk->ramdiskbuf);
	kfree(ramdisk);
}

module_init(ramdisk_init);
module_exit(ramdisk_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ALIENTEK");
MODULE_INFO(intree, "Y");
