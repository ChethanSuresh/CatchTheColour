#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

static int hello_init(void)
{
	printk(KERN_INFO "Hello, World! param=%d \n",hello_param);
	return 0;
}


static void hello_exit(void)
{
	printk(KERN_INFO "Good bye!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
