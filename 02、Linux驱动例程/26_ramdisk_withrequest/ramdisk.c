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
	struct blk_mq_tag_set tag_set; 	/* blk_mq_tag_set 				  */
	spinlock_t lock;				/* 自旋锁 						  */
};

struct ramdisk_dev *ramdisk = NULL;		/* ramdisk设备指针 */

/*
 * @description	: 处理传输过程
 * @param-req 	: 请求
 * @return 		: 0，成功；其它表示失败
 */
static int ramdisk_transfer(struct request *req)
{	
	unsigned long start = blk_rq_pos(req) << 9;  	/* blk_rq_pos获取到的是扇区地址，左移9位转换为字节地址 */
	unsigned long len  = blk_rq_cur_bytes(req);		/* 大小   */
	

	/* bio中的数据缓冲区
	 * 读：从磁盘读取到的数据存放到buffer中
	 * 写：buffer保存这要写入磁盘的数据
	 */
	void *buffer = bio_data(req->bio);		
	
	if(rq_data_dir(req) == READ) 		/* 读数据 */	
		memcpy(buffer, ramdisk->ramdiskbuf + start, len);
	else if(rq_data_dir(req) == WRITE) 	/* 写数据 */
		memcpy(ramdisk->ramdiskbuf + start, buffer, len);
		
	return 0;

}

/*
 * @description	: 开始处理传输数据的队列
 * @hctx 		: 硬件相关的队列结构体
 * @bd			: 数据相关的结构体
 * @return 		: 0，成功；其它值为失败
 */
static blk_status_t _queue_rq(struct blk_mq_hw_ctx *hctx, const struct blk_mq_queue_data* bd)
{
	struct request *req = bd->rq; /* 通过bd获取到request队列*/
	struct ramdisk_dev *dev = req->rq_disk->private_data;
	int ret;
	
	blk_mq_start_request(req);    /* 开启处理队列 */
	spin_lock(&dev->lock);		  
	ret = ramdisk_transfer(req);  /* 处理数据 */
	blk_mq_end_request(req, ret); /* 结束处理队列 */
	spin_unlock(&dev->lock);
	
	return BLK_STS_OK;
	
}
/*
 * 队列操作函数
 */
static struct blk_mq_ops mq_ops = {
    .queue_rq = _queue_rq,
};

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
 * @description	: 初始化队列相关操作
 * @set		 	: blk_mq_tag_set对象
 * @return 		: request_queue的地址
 */
static struct request_queue * create_req_queue(struct blk_mq_tag_set *set)
{
	struct request_queue *q;

#if 0

/*
 *这里是使用了blk_mq_init_sq_queue 函数
 *进行初始化的。
 */
	q = blk_mq_init_sq_queue(set, &mq_ops, 2, BLK_MQ_F_SHOULD_MERGE);
	
#else
	int ret;
	
	memset(set, 0, sizeof(*set));
	set->ops = &mq_ops;		//操作函数
    set->nr_hw_queues = 2;	//硬件队列
    set->queue_depth = 2;	//队列深度
	set->numa_node = NUMA_NO_NODE;//numa节点
	set->flags =  BLK_MQ_F_SHOULD_MERGE; //标记在bio下发时需要合并

	ret = blk_mq_alloc_tag_set(set); //使用函数进行再次初始化
	if (ret) {
		printk(KERN_WARNING "sblkdev: unable to allocate tag set\n");
		return ERR_PTR(ret);
	}
	
	q = blk_mq_init_queue(set); //分配请求队列
	if(IS_ERR(q)) {
		blk_mq_free_tag_set(set);
		return q;
	}
#endif

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
	dev->queue = create_req_queue(&dev->tag_set);
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
	blk_mq_free_tag_set(&dev->tag_set);
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
	
	/* 释放blk_mq_tag_set */
	blk_mq_free_tag_set(&ramdisk->tag_set);
	
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