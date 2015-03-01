#include <linux/sched.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>
struct task struct *task;

// // part 1: iterature through tasks using depth-first search
// struct task struct *task; struct list head *list;
// list_for_each(list, &init task->children) {
// 	task = list entry(list, struct task struct, sibling); /* task points to the next child in the list */
// }


/* Called on module load. */
int simple_init(void)
{
	printk(KERN_INFO "Loading Module\n");

	// part 1: iterature through tasks linearly
	for_each_process(task) {
		/* on each iteration task points to the next task */
		// display name char comm[16];
		// state volatile long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
		// process id int pid;
		*char processName = comm;

		char stateString[10];

		if ((int)task->state == -1)
			strcpy(stateString, "Unrunnable");
		else if ((int)task->state == 0)
			strcpy(stateString, "Runnable");
		else if ((int)task->state > 0)
			strcpy(stateString, "Stopped");

		*char stateName = stateString;

		// pid_t pidVal = waitpid(process_id, &status, WNOHANG); /* WNOHANG def'd in wait.h */
		// if (return_pid == -1) {
		//     strcpy(stateString, "Unrunnable");
		// } else if (return_pid == 0) {
		//     strcpy(stateString, "Unrunnable");
		// } else if (return_pid == process_id) {
		//     strcpy(stateString, "Stopped");
		// }

		printk(KERN_INFO "Process name: %s\n", *processName);
		printk(KERN_INFO "State: %s\n", *stateName);
		printk(KERN_INFO "Process ID: %i\n", task->pid);

	}

	return 0;
}

/* Call on module remove. */
void simple_exit(void)
{
	printk(KERN_INFO "Removing Module\n");

	// list_for_each_entry_safe(person, temp_person, &birthday_list, list) {
	// 	printk(KERN_INFO "Person[%i] birthday on %i/%i/%i removed...\n", person->day, person->day, person->month, person->year);
		
	// 	list_del(&person->list);
	// 	kfree(person);
	// }

}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ITERATE TASKS");
MODULE_AUTHOR("STEPHEN");
