#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>


struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
};



// list_del(struct list_head *element);



static LIST_HEAD(birthday_list);
struct birthday *person, *nextPerson;


/* This function is called when the module is loaded */
int simple_init (void) {
	printk(KERN_INFO "Loading Module\n");

	int i;
	for (i = 0; i < 5; i++) {
		person = kmalloc(sizeof(*person), GFP_KERNEL);
		person->day = 2+(2*i);
		person->month = 8+i;
		person->year = 1995-i;
		INIT_LIST_HEAD(&person->list);
		list_add_tail(&person->list, &birthday_list);
	}


	list_for_each_entry(person, &birthday_list, list) {
		/* on each iteration, ptr points to next birthday struct */
		printk(KERN_INFO "Person: %d Birthday: %d / %d / %d", i+1, person->month, person->day, person->year);
	}


	return 0;
}

/* This function is called when the module is removed */
void simple_exit (void) {
	printk(KERN_INFO "Removing Module\n");


	list_for_each_entry_safe(person, nextPerson, &birthday_list, list) {
		printk(KERN_INFO "Removing Persons from list");
		list_del(&person->list);
		kfree(person);
	}

}

/* Macros for registering module entry and exit points */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

