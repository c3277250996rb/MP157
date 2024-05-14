/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: dht11.c
作者	  	: 正点原子Linux团队
版本	   	: V1.0
描述	   	: DHT11 单总线驱动程序
其他	   	: 无
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2021/08/26 正点原子Linux团队创建
***************************************************************/
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of_gpio.h>

struct dht11_dev {
	struct miscdevice mdev; 	/* MISC设备 */
	int gpio;					/* GPIO编号 */
	struct timer_list timer;	/* 定时器 */
    struct work_struct work;	/* 工作队列 */
    u8 data[5];					/* 数据BUFF */
};

#define  HIGH                   1
#define  LOW                    0

struct dht11_dev dht11_device;

/*
 * @description	 : 	设置GPIO的输出值
 * @param - value: 	输出value的值 
 * @return 	     :  无
 */
static void dht11_set_output(int val)
{
	if(val)
		gpio_direction_output(dht11_device.gpio, 1);
	else
		gpio_direction_output(dht11_device.gpio, 0);
}

/*
 * @description	: 	设置GPIO为输入模式
 * @param 		:	无
 * @return 	  	:   无
 */
static void dht11_set_input(void)
{
	gpio_direction_input(dht11_device.gpio);
}

/*
 * @description	: 	获取GPIO的值
 * @param 		:	无 
 * @return 	  	:   GPIO的电平
 */
static unsigned char dht11_get_io(void)
{
	return gpio_get_value(dht11_device.gpio);
}

/*
 * @description	: 	读取一个字节的数据
 * @param 		: 	无
 * @return 	  	:   读取到的数据
 */
static unsigned char dht11_read_byte(void)
{
	unsigned char i, time = 0, data = 0;
	local_irq_disable();
	
	for(i = 0; i < 8; i++) {
		time = 0;

		while(dht11_get_io() == 0) {
			udelay(1);
			time++;
			if (time > 100) {
				return -EINVAL;
			}
		}
		udelay(45);		/* 延时45us */
		if (dht11_get_io() == 1) {	/* 获取到高电平，数据就为1，否则就是0 */
			data |= 1<<(7 - i);		
			time = 0;
			while(dht11_get_io() == 1) {
				udelay(1);
				time++;
				if (time > 100)
					return -EINVAL;
			}
		}
	}
	local_irq_enable();
	
    return data;
}

/*
 * @description	: 	DHT11的初始化
 * @param 		: 	无
 * @return 	  	:   0，初始化成功；	其它表示失败
 */
static int dht11_init(void)
{
	dht11_set_output(HIGH);	/* 把拉高Duot */
	udelay(30);				/* 拉高30us */

	dht11_set_output(LOW);	/* 把拉低Duot */
	mdelay(20);				/* 拉低20us */

	dht11_set_output(HIGH);	/* 把拉高Duot */
	udelay(30);				/* 拉高30us */

	dht11_set_input();		/* 设置Duot为输入模式 */
    udelay(200);			/* 延时200us */
    if(!dht11_get_io()) {	/* 不是高电平，DHT11就没有响应 */
        return -ENODEV;
    }
	return 0;
}

/*
 * @description		: 打开设备
 * @param - inode 	: 传递给驱动的inode
 * @param - filp 	: 设备文件，file结构体有个叫做pr似有ate_data的成员变量
 * 					  一般在open的时候将private_data似有向设备结构体。
 * @return 			: 0 成功;其他 失败
 */
static int dht11_open(struct inode *inode, struct file *filp)
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
static ssize_t dht11_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt) 
{
    int ret = 0;
	ret = copy_to_user(buf, &dht11_device.data[0], 5);

	return ret;
}

static struct file_operations dht11_fops = {
	.owner	= THIS_MODULE,
	.open	= dht11_open,
	.read   = dht11_read,
};

/*
 * @description     : 使用内核的工作队列，获取温度的原始数据
 * @param - work 	: work的结构体
 * @return          : 无
 */
static void dht11_work_callback(struct work_struct *work)
{
    int i = 0;
	unsigned char buff[5];
	
    if(!dht11_init()) {
        for(i = 0; i < 5; i++) {
            buff[i] = dht11_read_byte();	/* 获取数据 */
        }
			/* 校验数据是否正确 */
	    if((buff[0] + buff[1] + buff[2] + buff[3]) == buff[4]) {
		    memcpy(&dht11_device.data[0], &buff[0], 5);
        }
    }
}

/*
 * @description     : 定时器的操作函数，每1s去获取一次数据
 * @param - asg 	: 定时器的结构体
 * @return          : 无
 */
static void dht11_timer_callback(struct timer_list *arg)
{
    schedule_work(&dht11_device.work);
    mod_timer(&dht11_device.timer, jiffies + (1500 * HZ/1000));
}

/*
 * @description	: 	GPIO的初始化函数
 * @param pdev	:	platform设备 	
 * @return 	  	:   0表示转换成功，其它值表示转换失败
 */
static int dht11_request_gpio(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	int ret;

	dht11_device.gpio = of_get_named_gpio(dev->of_node, "dht11-gpio", 0);
	if (!gpio_is_valid(dht11_device.gpio)) {
		dev_err(dev, "Failed to get gpio");
		return -EINVAL;
	}

	ret = devm_gpio_request(dev, dht11_device.gpio, "DHT11 Gpio");
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
static int dht11_probe(struct platform_device *pdev)
{
	struct miscdevice *mdev;
	int ret;

	dev_info(&pdev->dev, "dht11 device and driver matched successfully!\n");

	ret = dht11_request_gpio(pdev);
	if (ret)
		return ret;

	/* 初始化 MISG设备 */
	mdev = &dht11_device.mdev;
	mdev->name	= "dht11";
	mdev->minor	= MISC_DYNAMIC_MINOR;
	mdev->fops	= &dht11_fops;
	
	/* 初始化定时器 */
	timer_setup(&dht11_device.timer, dht11_timer_callback, 0);
    dht11_device.timer.expires=jiffies + msecs_to_jiffies(1500);
    add_timer(&dht11_device.timer);
	
	/* 初始化工作队列 */
    INIT_WORK(&dht11_device.work, dht11_work_callback);
	
	/* MISG 设备注册 */
	return misc_register(mdev);
}

/*
 * @description     : 驱动的remove函数，移除驱动的时候此函数会执行
 * @param - pdev 	: pdev设备
 * @return          : 0，成功;其他负值,失败
 */
static int dht11_remove(struct platform_device *pdev)
{
	gpio_set_value(dht11_device.gpio, 0);

	/* 卸载MISG设备 */
	misc_deregister(&dht11_device.mdev);
	/* 卸载定时器 */
    del_timer(&dht11_device.timer);
	/* 卸载工作队列 */
    cancel_work_sync(&dht11_device.work);

	dev_info(&pdev->dev, "DHT11 driver has been removed!\n");
	return 0;
}

static const struct of_device_id dht11_of_match[] = {
	{ .compatible = "alientek,dht11" },
	{ /* Sentinel */ }
};

static struct platform_driver dht11_driver = {
	.driver = {
		.owner			= THIS_MODULE,
		.name			= "dht11",
		.of_match_table	= dht11_of_match,
	},
	.probe		= dht11_probe,
	.remove		= dht11_remove,
};

module_platform_driver(dht11_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ALIENTEK");
MODULE_INFO(intree, "Y");
