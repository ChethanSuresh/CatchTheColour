#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/slab.h>

char *kbuf;
static int major;
size_t k_size;
static int serial_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "Open serial\n");
	return 0;	
}

static ssize_t serial_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
	unsigned long return_cou;
	printk(KERN_INFO "Read serial\n");
	return_cou = copy_to_user(user_buf, kbuf, 1);
	return 0;
}

static ssize_t serial_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
	printk(KERN_INFO "Write function\n");
	return count;
}

static int serial_close(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "Close function\n");
	return 0;
} 

static const struct file_operations serial_fops = {
	.open           = serial_open,
	.release	= serial_close,
	.read		= serial_read,
	.write		= serial_write,
};

static int __init open_init(void)
{
	printk(KERN_INFO "Init serial\n");
	major = register_chrdev(0, "serial_test",&serial_fops);

	k_size = 1;
	kbuf = kmalloc(1,GFP_KERNEL);
	return 0;
}

static void __exit close_exit(void)
{
	unregister_chrdev(major, "serial_test");
	printk(KERN_INFO "Good bye!\n");
}

module_init(open_init);
module_exit(close_exit);

MODULE_LICENSE("GPL");

