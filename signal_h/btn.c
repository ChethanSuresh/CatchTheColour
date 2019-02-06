#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>          // Required for the copy to user function
#include <linux/gpio/driver.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()


#define YELLOW  200 //GPIO7_8
#define SWITCH  91  //GPIO3_27 
#define DEVICE_NAME "btn"

// IOCTL 
#include <linux/ioctl.h>
#define IOCTL_MAGIC 0xBF
#define IOCTL_APP_INFO _IO(IOCTL_MAGIC, 1)
static irqreturn_t btn_press(void);


/* IRQ handler */
static irqreturn_t btn_press(void){
	printk(KERN_INFO "Button pressed\n");
	return	(irqreturn_t)IRQ_HANDLED;
}

#if 0 
/* IOCTL handler */
static long btn_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
	if (cmd == IOCTL_APP_INFO) {
		printk( "REG_CURRENT_TASK\n");
	}
	return 0;
}
#endif

/* File operations */
static const struct file_operations btn_fops = {
	//.open           = btn_open,
	//.release    = btn_close,
	//.read       = btn_read,
	//.write      = btn_write,
	//.unlocked_ioctl = btn_unlocked_ioctl,
};

/* Initialise driver */
static int __init open_init(void){
	int ret;
	int irq_number;
	printk(KERN_INFO "Init GPIO91 switch\n");


	/* Turn off LED first */
	gpio_direction_input(SWITCH);
	gpio_direction_output(YELLOW,0);
	gpio_set_debounce(SWITCH, 200);

	/* Create IRQ request */
	irq_number = gpio_to_irq(91);
	printk("number %d\n", irq_number);
	ret = request_irq(gpio_to_irq(91), (irq_handler_t)btn_press, IRQF_TRIGGER_FALLING, "btn", NULL);
	if (ret < 0){
		printk("Error with requset_irq");
		printk(KERN_ALERT "%s: request_irg failed with %d\n",__func__, ret);
	}

	return 0;
}

static void __exit close_exit(void){
	/* Turn off LED */
	gpio_direction_output(YELLOW,0);


	/*Free the IRQ*/
	free_irq(gpio_to_irq(91), NULL);
	printk(KERN_INFO "Exit GPIO91 switch\n");

}

module_init(open_init);
module_exit(close_exit);

MODULE_LICENSE("GPL");
