#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/slab.h>

#define RED     24  //GPIO1_24
#define YELLOW  200 //GPIO7_8
#define BLUE    90  //GPIO3_26
#define SWITCH	91  //GPIO3_27 


// IOCTL 
#include <linux/ioctl.h>
#define IOCTL_MAGIC 0xBF
#define IOCTL_RED_LED_ON _IO(IOCTL_MAGIC, 1)
#define IOCTL_RED_LED_ON_WRITE_PARAM _IOW(IOCTL_MAGIC, 2, int)
#define IOCTL_RED_LED_ON_READ_PARAM _IOR(IOCTL_MAGIC, 3, int)
#define IOCTL_RED_LED_ON_RW_PARAM _IOWR(IOCTL_MAGIC, 4, int)


char *kbuf;
static int major;
static int write_count = 0;
static int tmp_write_count = 0;

static int major;
static int chethan_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "Open function\n");
	tmp_write_count = write_count;
	return 0;	
}

static ssize_t chethan_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
	printk(KERN_INFO "Read function\n");
	unsigned long tmp = write_count;
   	printk(KERN_INFO "Read function\n");
	if(tmp_write_count != 0){
    		copy_to_user(user_buf, kbuf, tmp);
		tmp_write_count = 0;
	 return tmp;
	}

	// Toggle red LED 
	gpio_direction_output(RED,1);
	msleep(1000); //sleep for 1 sec
	gpio_direction_output(RED,0);

	return 0;
}

//ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
static ssize_t chethan_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
	printk(KERN_INFO "Write function\n");
	copy_from_user(kbuf, user_buf, count);
	write_count = count;

	if (*user_buf == '1') {
		gpio_direction_output(YELLOW,1);
	} else if (*user_buf == '0') {
		gpio_direction_output(YELLOW,0);
	}	
	return count;
}

static int chethan_close(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "Close function\n");
	return 0;
} 

static long chethan_unlocked_ioctl (struct file *file, unsigned int command, unsigned long param){
	switch(command){
		case IOCTL_RED_LED_ON: 
			printk(KERN_INFO "Without param red led\n");
			break;
		case IOCTL_RED_LED_ON_WRITE_PARAM:
			printk(KERN_INFO "With write param red led: %d\n", param);
			gpio_direction_output(RED,param);
			break;
		case IOCTL_RED_LED_ON_READ_PARAM:
			printk(KERN_INFO "With read param red led\n");
			break;
		/*
		case IOCTL_RED_LED_ON_RW_PARAM:
			printk(KERN_INFO "With read/write param red led\n");
			break;
		*/
		default:
			printk(KERN_INFO "Not implemented\n");
			break;
	}
	return 0;
}


static const struct file_operations chethan_fops = {
	.open           = chethan_open,
	.release	= chethan_close,
	.read		= chethan_read,
	.write		= chethan_write,
	.unlocked_ioctl = chethan_unlocked_ioctl,
};

static int __init open_init(void)
{
	printk(KERN_INFO "Init LED GPIO1_24 GPIO7_8 \n");
	kbuf = kmalloc(100,GFP_KERNEL);
	major = register_chrdev(0, "chethan",&chethan_fops);
	// Turn off LED first
	gpio_direction_output(RED,0);
	gpio_direction_output(YELLOW,0);
	gpio_direction_output(BLUE,0);
	return 0;
}

static void __exit close_exit(void)
{
	unregister_chrdev(major, "chethan");
	printk(KERN_INFO "Good bye!\n");
}

module_init(open_init);
module_exit(close_exit);

MODULE_LICENSE("GPL");

