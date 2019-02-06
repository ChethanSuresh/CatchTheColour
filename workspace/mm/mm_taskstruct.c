#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/delay.h>


//unsigned long start = vma->vm_start;
//unsigned long end =  vma->vm_start;
//unsigned long len = vma->vm_end - vma->vm_start;

	struct task_struct *task;
static int memory_init(void)
{
	printk(KERN_INFO "Kernel task info \n");
	//struct task_mm *task->mm_struct;
	//struct task_mm_area *task_mm->vm_area_struct;

	
	task = current;
	printk(KERN_INFO "Task PID: %d\n", task->pid);
	//printk("VMA Start %ld", task_mm_area->vm_start);
	return 0;
}

static void memory_exit(void)
{
	printk(KERN_INFO "Good bye!\n");
}

/*
//int (*mmap) (struct file *, struct vm_area_struct *);
static const struct file_operations memory_fops = {
	//.write      = chethan_write,
	//.mmap	= memory_mmap,
};
*/

module_init(memory_init);
module_exit(memory_exit);

MODULE_LICENSE("GPL");
