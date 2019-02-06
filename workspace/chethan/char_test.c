#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

//static int chethan_open();
//static int chethan_close();

char *kbuf;
static int major;
static int write_count = 0;
static int tmp_write_count = 0;
static int chethan_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "Open function\n");
	tmp_write_count = write_count;
	return 0;	
}

static ssize_t chethan_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
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

static ssize_t chethan_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
    printk(KERN_INFO "Write function\n");
	copy_from_user(kbuf, user_buf, count);
	write_count = count;
	return count;
}

static int chethan_close(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "Close function\n");
        return 0;
} 

static const struct file_operations chethan_fops = {
	.open           = chethan_open,
	.release	= chethan_close,
	.read		= chethan_read,
	.write		= chethan_write,
};

static int __init open_init(void)
{
	kbuf = kmalloc(100,GFP_KERNEL);	
	major = register_chrdev(0, "chethan",&chethan_fops);
	printk(KERN_INFO "Hello, World! \n");
	return 0;
}


static void __exit close_exit(void)
{
	unregister_chrdev(major, "chethan");
	printk(KERN_INFO "Good bye!\n");
	kfree(kbuf);
}

module_init(open_init);
module_exit(close_exit);

MODULE_LICENSE("GPL");

