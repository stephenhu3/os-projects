#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

/* Called on module load. */
int simple_init(void) {
	printk(KERN_INFO "Loading Module iterateTasks\n");

	struct task_struct *task;
	
	// part 1: iterature through tasks linearly
	for_each_process(task) {
		/* on each iteration task points to the next task */

		char stateString[10];

		/* process state is determined by its state integer: 
		/  unrunnable if state = -1, runnable if zero, stopped if positive */
		if ((int)task->state == -1)
			strcpy(stateString, "Unrunnable");
		else if ((int)task->state == 0)
			strcpy(stateString, "Runnable");
		else if ((int)task->state > 0)
			strcpy(stateString, "Stopped");

		char *stateName = stateString;

		/* print out the process name, state, and process id */
		printk(KERN_INFO "Process name: %s\n", task->comm);
		printk(KERN_INFO "State: %s\n", stateName);
		printk(KERN_INFO "Process ID: %i\n", task->pid);
	}

	return 0;
}

/* Call on module remove. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ITERATE TASKS");
MODULE_AUTHOR("STEPHEN");
