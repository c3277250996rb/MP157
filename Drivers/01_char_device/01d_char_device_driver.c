#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include <linux/device.h>

#define GLOBALMEM_SIZE	0x1000
#define MEM_CLEAR 0x1
#define GLOBALMEM_MAJOR 230
#define CHRDEVBASE_NAME "globalmem"

#define DEVICE_COUNT 10

static int globalmem_major = GLOBALMEM_MAJOR;
static int enable_multiple = 0;

module_param(globalmem_major, int, S_IRUGO);
module_param(enable_multiple, int, S_IRUGO);

struct globalmem_dev {
	struct cdev cdev;
	unsigned char mem[GLOBALMEM_SIZE];
};

struct globalmem_dev *globalmem_devp;


static int globalmem_open(struct inode *inode, struct file *filp){
	if(enable_multiple){
		struct globalmem_dev *minor_device = container_of(inode -> i_cdev, struct globalmem_dev, cdev);
		filp -> private_data = minor_device;
	}else{
		filp -> private_data = globalmem_devp;
	}
	
	return 0;
}

static int globalmem_release(struct inode *inode, struct file *filp){
	return 0;
}

static long globalmem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg){
	struct globalmem_dev *client_device = filp -> private_data;

	switch(cmd){
		case MEM_CLEAR:
			memset(client_device -> mem, 0, GLOBALMEM_SIZE);
			printk(KERN_INFO "globalmem is set to zero\n");
			break;
		default:
			return -EINVAL;
	}

	return 0;
}

static ssize_t globalmem_read(struct file *filp, char __user * user_buffer, size_t size, loff_t * ppos){
	unsigned long p = *ppos;
	unsigned int count = size;
	int result = 0;
	struct globalmem_dev *client_device = filp -> private_data;

	if(p >= GLOBALMEM_SIZE)	//相对位移超出最大内存边界
		return 0;

	if(count > GLOBALMEM_SIZE - p)	//读取数量不可超过剩余数量
		count = GLOBALMEM_SIZE - p;

	if(copy_to_user(user_buffer, client_device -> mem + p, count)){
		result = -EFAULT;
	}else{
		*ppos += count;
		result = count;
		printk(KERN_INFO "read %u bytes(s) from %lu\n", count, p);
	}

	return result;
}

static ssize_t globalmem_write(struct file *filp, const char __user * user_buffer, size_t size, loff_t * ppos){
	unsigned long p = *ppos;
	unsigned int count = size;
	int result = 0;
	struct globalmem_dev *client_device = filp -> private_data;

	if(p >= GLOBALMEM_SIZE)	//相对位移超出最大内存边界
		return 0;

	if(count > GLOBALMEM_SIZE - p)	//读取数量不可超过剩余数量
		count = GLOBALMEM_SIZE - p;
	
	if(copy_from_user(client_device -> mem + p, user_buffer, count)){
		result = -EFAULT;
	}else{
		*ppos += count;
		result = count;
		printk(KERN_INFO "write %u bytes(s) to %lu\n", count, p);
	}

	return result;
}

static loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig){
	loff_t ret = 0;
	switch (orig) {
	case 0:
		if (offset < 0) {
			ret = -EINVAL;
			break;
		}
		if ((unsigned int)offset > GLOBALMEM_SIZE) {
			ret = -EINVAL;
			break;
		}
		filp->f_pos = (unsigned int)offset;
		ret = filp->f_pos;
		break;
	case 1:
		if ((filp->f_pos + offset) > GLOBALMEM_SIZE) {
			ret = -EINVAL;
			break;
		}
		if ((filp->f_pos + offset) < 0) {
			ret = -EINVAL;
			break;
		}
		filp->f_pos += offset;
		ret = filp->f_pos;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	return ret;
}


static const struct file_operations globalmem_fops = {
	.owner = THIS_MODULE,
	.llseek = globalmem_llseek,
	.read = globalmem_read,
	.write = globalmem_write,
	.unlocked_ioctl = globalmem_ioctl,
	.open = globalmem_open,
	.release = globalmem_release,
};

static void globalmem_setup_cdev(struct globalmem_dev *client_device, int minor_number){
	int error, dev_d = MKDEV(globalmem_major, minor_number);	// MKDEV原型: #define MKDEV(major, minor) (((major) << MINORBITS) | (minor))

	cdev_init(&client_device -> cdev, &globalmem_fops);
	client_device -> cdev.owner = THIS_MODULE;

	error = cdev_add(&client_device -> cdev, dev_d, 1);
	if(error){
		printk(KERN_NOTICE  "Error %d adding globalmem%d", error, minor_number);
	}else{
		printk(KERN_INFO "create minor: %d\n", minor_number);
	}
}


static int __init globalmem_init(void){
	const int minor_count = enable_multiple ? DEVICE_COUNT : 1;
	int result = 0;
	int i;
	dev_t dev_d = MKDEV(globalmem_major, 0);

	if(globalmem_major){
		result = register_chrdev_region(dev_d, minor_count, CHRDEVBASE_NAME);
	}else{	//动态分配
		// 原型: int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, const char *name);
		result = alloc_chrdev_region(&dev_d, 0, minor_count, CHRDEVBASE_NAME);
		globalmem_major = MAJOR(dev_d);
	}

	if(result < 0){
		printk(KERN_INFO "Failed to register\n");
		return result;
	}

	globalmem_devp = kzalloc(sizeof(struct globalmem_dev) * minor_count, GFP_KERNEL);	// GFP_KERNEL是分配标志

	if(!globalmem_devp){
		result = -ENOMEM;
		goto fail_malloc;
	}

	for(i = 0; i < minor_count; i++)
		globalmem_setup_cdev(globalmem_devp + i, i);

	printk(KERN_INFO "%s init\n", CHRDEVBASE_NAME);
	return 0;

fail_malloc:
	unregister_chrdev_region(dev_d, minor_count);
	return result;
}module_init(globalmem_init);


static void __exit globalmem_exit(void){
	int i;
	const int minor_count = enable_multiple ? DEVICE_COUNT : 1;

	for(i = 0; i < minor_count; i++)
		cdev_del(&(globalmem_devp + i) -> cdev);

	kfree(globalmem_devp);
	unregister_chrdev_region(MKDEV(globalmem_major, 0), minor_count);
	printk("%s exit\r\n", CHRDEVBASE_NAME);

}module_exit(globalmem_exit);


/* 
 * LICENSE和作者信息
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZAT");
MODULE_INFO(intree, "Y");

