#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

#define NUM_PERSON 5

struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
};

static LIST_HEAD(birthday_list);

struct birthday *person, *temp_person;

/* Called on module load. */
int simple_init(void)
{
	printk(KERN_INFO "Loading Module\n");

	int i;

	for(i = 0; i < NUM_PERSON; i++) {
		person = kmalloc(sizeof(*person), GFP_KERNEL);
		person->day = i;
		person->month = 1+i;
		person->year = 2000+i;
		INIT_LIST_HEAD(&person->list);
		list_add_tail(&person->list, &birthday_list);
	}

	list_for_each_entry(person, &birthday_list, list){
		printk(KERN_INFO "Person[%i] birthday on %i/%i/%i.\n", person->day, person->day, person->month, person->year);
	}

	return 0;
}

/* Call on module remove. */
void simple_exit(void)
{
	printk(KERN_INFO "Removing Module\n");

	list_for_each_entry_safe(person, temp_person, &birthday_list, list) {
		printk(KERN_INFO "Person[%i] birthday on %i/%i/%i removed...\n", person->day, person->day, person->month, person->year);
		
		list_del(&person->list);
		kfree(person);
	}

}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

