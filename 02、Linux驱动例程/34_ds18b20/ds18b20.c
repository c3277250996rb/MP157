/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: ds18b20.c
作者	  	: 正点原子Linux团队
版本	   	: V1.0
描述	   	: DS18B20 单总线驱动程序
其他	   	: 无
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2021/08/24 正点原子Linux团队创建
***************************************************************/
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of_gpio.h>
#include <linux/errno.h>

struct ds18b20_dev {
    struct miscdevice mdev;	/* MISC设备 */
    int gpio;				/* GPIO的编号 */
    unsigned char data[2]; /* 接收原始数据的BUFF */
	struct timer_list timer;	/* 定时器 */
    struct work_struct work;	/* 工作队列 */
};

#define HIGH 1	
#define LOW 0

struct ds18b20_dev ds18b20_device;

/*
 * @description	 : 	设置GPIO的输出值
 * @param - value: 	输出value的值 
 * @return 	     :  无
 */
static void ds18b20_set_output(int value)
{
    if(value)
        gpio_direction_output(ds18b20_device.gpio, 1);
    else
        gpio_direction_output(ds18b20_device.gpio, 0);
}

/*
 * @description	: 	设置GPIO为输入模式
 * @param 		:	无
 * @return 	  	:   无
 */
static void ds18b20_set_input(void)
{
    gpio_direction_input(ds18b20_device.gpio);
}

/*
 * @description	: 	获取GPIO的值
 * @param 		:	无 
 * @return 	  	:   GPIO的电平
 */
static int ds18b20_get_io(void)
{
    return gpio_get_value(ds18b20_device.gpio); 
}

/*
 * @description	: 	写一位数据
 * @param 	bit	: 	要写入的位数
 * @return 	  	:   无
 */
static void ds18b20_write_bit(int bit)
{
	local_irq_disable();		  /* 关闭处理器的所有中断 */
    if(bit) {
        ds18b20_set_output(LOW);  /* 把DQ拉低 */
        udelay(2);				  /* 拉低2us */
        ds18b20_set_output(HIGH); /* 把DQ拉高 */
        udelay(60); 			  /* 拉高60us */
    } else {
        ds18b20_set_output(LOW);  /* 把DQ拉低 */
        udelay(60);				  /* 拉低60us */
        ds18b20_set_output(HIGH); /* 把DQ拉高 */
        udelay(2);				  /* 拉高2us */
    }
	local_irq_enable();			  /* 开启处理器的所有中断 */
}

/*
 * @description	: 	读一位数据
 * @param 		: 	无
 * @return 	  	:   返回读取一位的数据
 */
static int ds18b20_read_bit(void)
{
    u8 bit = 0;
	local_irq_disable();
    ds18b20_set_output(LOW);	/* 把DQ拉低 */
    udelay(1);					/* 拉低1us */

    ds18b20_set_output(HIGH);	/* 把DQ拉高 */
    udelay(1);					/* 拉高1us */
    
    ds18b20_set_input();		/* 设置为输入模式，开始接收数据 */

    if(ds18b20_get_io())		/* 获取DQ的电平，高为1，低为0 */
        bit = 1;
    udelay(50);					/* 延时50us */
	 local_irq_enable();
    return bit;					/* 返回读取数据 */
}

/*
 * @description	: 	写一个字节到DS18B20
 * @param byte  : 	要写入的字节
 * @return 	  	:   无
 */
static void ds18b20_write_byte(u8 byte)
{
    int i;
    for(i = 0; i < 8; i++) {
        if(byte & 0x01)
            ds18b20_write_bit(1); /* write 1 */
        else
            ds18b20_write_bit(0); /* write 0 */
        byte >>= 1;	/* 右移一位获取高一位的数据 */
    }
}

/*
 * @description	: 	读取一个字节的数据
 * @param 		: 	无
 * @return 	  	:   读取到的数据
 */
static char ds18b20_read_byte(void)
{
    int i;
    u8 byte = 0;
    for(i = 0; i < 8; i++) {	/* DS18B20先输出低位数据 ,高位数据后输出 */
        if(ds18b20_read_bit())
            byte |= (1 << i);
        else
            byte &= ~(1 << i);
    }
    return byte;
}

/*
 * @description	: 	初始化DS18B20
 * @param 		: 	无
 * @return 	  	:   0，初始化成功，1，失败
 */
static int ds18b20_init(void)
{
    int ret = -1;
	ds18b20_set_output(HIGH);	/* 把DQ拉高 */
	udelay(1);					/* 拉高1us */
	ds18b20_set_output(LOW);	/* 把DQ拉低 */
    udelay(500);				/* 拉低500us */
	ds18b20_set_output(HIGH);	/* 把DQ拉高 */
	udelay(60);					/* 拉高60us */
	ds18b20_set_input();		/* 设置为输入模式 */
    ret = ds18b20_get_io();		/* 获取到低电平做响应 */
	udelay(240);				/* 延时240us */
	
	return ret;		
}
 
/*
 * @description		: 打开设备
 * @param - inode 	: 传递给驱动的inode
 * @param - filp 	: 设备文件，file结构体有个叫做pr似有ate_data的成员变量
 * 					  一般在open的时候将private_data似有向设备结构体。
 * @return 			: 0 成功;其他 失败
 */
static int ds18b20_open(struct inode *inode, struct file *filp)
{
	return 0;
}
/*
 * @description		: 从设备读取数据 
 * @param - filp 	: 要打开的设备文件(文件描述符)
 * @param - buf 	: 返回给用户空间的数据缓冲区
 * @param - cnt 	: 要读取的数据长度
 * @param - offt 	: 相对于文件首地址的偏移
 * @return 			: 读取的字节数，如果为负值，表示读取失败
 */
static ssize_t ds18b20_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt) 
{
    int ret;
    ret = copy_to_user(buf, &ds18b20_device.data[0], 2);
	if(ret)
        return -ENOMEM;
	return ret;
}

static struct file_operations ds18b20_fops = {
	.owner	= THIS_MODULE,
	.open = ds18b20_open,
	.read	= ds18b20_read,
};

/*
 * @description     : 使用内核的工作队列，获取温度的原始数据
 * @param - work 	: work的结构体
 * @return          : 无
 */
static void ds18b20_work_callback(struct work_struct *work)
{
	int ret = -1;
	 ret = ds18b20_init();	/* 初始化ds18b20 */
     if(ret != 0)
         goto out1;
	ds18b20_write_byte(0XCC);	/* 跳过ROM */
	ds18b20_write_byte(0X44);	/* 开启温度采集 */
	
     ret = ds18b20_init();		/* 初始化ds18b20 */
     if(ret != 0)
         goto out1;
	
	ds18b20_write_byte(0XCC);	/* 跳过ROM */
	ds18b20_write_byte(0XBE);	/* 开启读取温度 */
	
	ds18b20_device.data[0] = ds18b20_read_byte();	/* 获取低位数据 */
	ds18b20_device.data[1] = ds18b20_read_byte();	/* 获取高位数据 */
out1:
    return;
}

/*
 * @description     : 定时器的操作函数，每1s去获取一次数据
 * @param - asg 	: 定时器的结构体
 * @return          : 无
 */
static void ds18b20_timer_callback(struct timer_list *arg)
{
    schedule_work(&ds18b20_device.work);	/* 运行工作队列，去获取温度 */
    mod_timer(&ds18b20_device.timer, jiffies + (1000 * HZ/1000));	/* 定时1s */
}

/*
 * @description	: 	GPIO的初始化函数
 * @param pdev	:	platform设备 	
 * @return 	  	:   0表示转换成功，其它值表示转换失败
 */
static int ds18b20_request_gpio(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    int ret;
	
    ds18b20_device.gpio = of_get_named_gpio(dev->of_node, "ds18b20-gpio", 0);
    if (!gpio_is_valid(ds18b20_device.gpio)) {
        dev_err(dev, "Failed to get gpio");
        return -EINVAL;
    }

    ret = devm_gpio_request(dev, ds18b20_device.gpio, "DS18B20 Gpio");
    if (ret) {
        dev_err(dev, "Failed to request gpio");
        return ret;
    }

    return 0;
}

/*
  * @description    : 驱动的probe函数，当驱动与
  *                    设备匹配以后此函数就会执行
  * @param - pdev  	: pdev设备
  * @return 		: 0表示转换成功，其它值表示转换失败
  */
static int ds18b20_probe(struct platform_device *pdev)
{
    struct miscdevice *mdev;
    int ret;

	dev_info(&pdev->dev, "ds18b20 device and driver matched successfully!\n");
	
	/* GPIO的初始化 */
    ret = ds18b20_request_gpio(pdev);
    if(ret)
        return ret;
	/* 初始化 MISG设备 */
    mdev = &ds18b20_device.mdev;
    mdev->name = "ds18b20";
    mdev->minor = MISC_DYNAMIC_MINOR;
    mdev->fops = &ds18b20_fops;
	
	/* 初始化定时器 */
	timer_setup(&ds18b20_device.timer, ds18b20_timer_callback, 0);
    ds18b20_device.timer.expires=jiffies + msecs_to_jiffies(1000);
    add_timer(&ds18b20_device.timer);
    
	/* 初始化工作队列 */
	INIT_WORK(&ds18b20_device.work, ds18b20_work_callback);
	
	/* MISG 设备注册 */
    return misc_register(mdev);
}

/*
 * @description     : 驱动的remove函数，移除驱动的时候此函数会执行
 * @param - pdev 	: pdev设备
 * @return          : 0，成功;其他负值,失败
 */
static int ds18b20_remove(struct platform_device *pdev)
{
	dev_info(&pdev->dev, "DS18B20 driver has been removed!\n");
	
	/* 卸载MISG设备 */
	misc_deregister(&ds18b20_device.mdev);
	/* 卸载定时器 */
	del_timer(&ds18b20_device.timer);
	/* 卸载工作队列 */
    cancel_work_sync(&ds18b20_device.work);
    return 0;
}

static const struct of_device_id ds18b20_of_match[] = {
	{ .compatible = "alientek,ds18b20" },
	{ /* Sentinel */ }
};

static struct platform_driver ds18b20_driver = {
	.driver = {
		.name			= "ds18b20",
		.of_match_table	= ds18b20_of_match,
	},
	.probe		= ds18b20_probe,
	.remove		= ds18b20_remove,
};

module_platform_driver(ds18b20_driver);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ALIENTEK");
MODULE_INFO(intree, "Y");
