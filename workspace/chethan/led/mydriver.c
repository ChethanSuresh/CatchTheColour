#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

//static int archana_open();
//static int archana_close();

char *kbuf;
static int major;
static int write_count = 0;
static int tmp_write_count = 0;
static int archana_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "Open function\n");
	tmp_write_count = write_count;
	return 0;	
}

static ssize_t archana_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
//	int len;
	unsigned long tmp = write_count;
        printk(KERN_INFO "Read function\n");
	if(tmp_write_count != 0)
        {
		copy_to_user(user_buf, kbuf, tmp);
		tmp_write_count = 0;
		return tmp;
	}
	return 0;
}

static ssize_t archana_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
        printk(KERN_INFO "Write function\n");
	copy_from_user(kbuf, user_buf, count);
	write_count = count;
	return count;
}

static int archana_close(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "Close function\n");
        return 0;
} 

static const struct file_operations archana_fops = {
	.open           = archana_open,
	.release	= archana_close,
	.read		= archana_read,
	.write		= archana_write,
};

static int __init open_init(void)
{
	kbuf = kmalloc(100,GFP_KERNEL);	
	major = register_chrdev(0, "archana",&archana_fops);
	printk(KERN_INFO "Hello, World! \n");
	return 0;
}


static void __exit close_exit(void)
{
	unregister_chrdev(major, "archana");
	printk(KERN_INFO "Good bye!\n");
}

module_init(open_init);
module_exit(close_exit);

MODULE_LICENSE("GPL");

