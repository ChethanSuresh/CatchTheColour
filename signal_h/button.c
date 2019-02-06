#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include<linux/uaccess.h>
#include <linux/signal.h>
#include <asm/io.h>
#include <linux/sched.h>

#define YELLOW  200 //GPIO7_8
#define SWITCH  91  //GPIO3_27 


#define  DEVICE_NAME "button"
#define  CLASS_NAME  "buttonClass"

MODULE_LICENSE("GPL");

static int    majorNumber;
static struct class*  btnClass  = NULL;
static struct device* btnDevice = NULL;
static int ret;


static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

//int task_pid ;
struct task_struct *task;
struct task_struct *task1;
struct siginfo sinfo;


/* IOCTL define */
#if 1 
#include <linux/ioctl.h>
#define IOCTL_MAGIC 0xBF
#define IOCTL_APP_INFO _IOR(IOCTL_MAGIC, 1, int *)
//static struct task_struct *task = NULL;

/* IOCTL handler */
static long dev_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
	static int task_pid;
	if (cmd == IOCTL_APP_INFO) {	
		task_pid = arg;
		task1 = current;
		task = pid_task(find_vpid(task_pid), PIDTYPE_PID);
		printk(KERN_INFO "IOCTL by task PID: %d\n", task_pid);
		printk(KERN_INFO "Current task PID: %d\n", task1->pid);
	}
	return 0;
}
#endif

/* IRQ handler */
static irqreturn_t dev_press(void){

	struct siginfo info;
	printk(KERN_INFO "Button press %d\n", task->pid);
	
	//Sending signal to app
	memset(&info, 0, sizeof(struct siginfo));
	info.si_signo = SIGINT;
	info.si_int = 1;

	printk(KERN_INFO "Sending SIGINT signal to app\n");
	send_sig_info(SIGINT, &info, task);
	
	return  (irqreturn_t)IRQ_HANDLED;
}

static struct file_operations fops =
{
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release,
	.unlocked_ioctl = dev_unlocked_ioctl,
};

static int __init button_init(void){

	printk(KERN_INFO "Init Button\n");

	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber<0){
		printk(KERN_ALERT "Button failed to register a major number\n");
		return majorNumber;
	}
	printk(KERN_INFO "Button: registered correctly with major number %d\n", majorNumber);

	// Register the device class
	btnClass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(btnClass)){                // Check for error and clean up if there is
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device class\n");
		return PTR_ERR(btnClass);          // Correct way to return an error on a pointer
	}
	printk(KERN_INFO "Button: device class registered correctly\n");

	// Register the device driver
	btnDevice = device_create(btnClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if (IS_ERR(btnDevice)){               // Clean up if there is an error
		class_destroy(btnClass);           // Repeated code but the alternative is goto statements
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create the device\n");
		return PTR_ERR(btnDevice);
	}

	/* Turn off LED first */
	gpio_direction_input(SWITCH);
	gpio_direction_output(YELLOW,0);
	gpio_set_debounce(SWITCH, 200); //API not working

	/* Register IRQ request */
	ret = request_irq(gpio_to_irq(91), (irq_handler_t)dev_press, IRQF_TRIGGER_FALLING, "button", NULL);
	if (ret < 0){
		printk("Error with requset_irq");
		printk(KERN_ALERT "%s: request_irg failed with %d\n",__func__, ret);
	}

	printk(KERN_INFO "Button: device class created correctly\n"); // Made it! device was initialized
	return 0;
}

static void __exit button_exit(void){

	/*Free the IRQ*/
	free_irq(gpio_to_irq(91), NULL);

	/* unregister char device */
	device_destroy(btnClass, MKDEV(majorNumber, 0));
	class_unregister(btnClass);
	class_destroy(btnClass);
	unregister_chrdev(majorNumber, DEVICE_NAME);

	printk(KERN_INFO "Button: Goodbye\n");
}

static int dev_open(struct inode *inodep, struct file *filep){
	return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
	return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
	return 0;
}

static int dev_release(struct inode *inodep, struct file *filep){

	printk(KERN_INFO "Button: Device successfully closed\n");
	return 0;
}

module_init(button_init);
module_exit(button_exit);
