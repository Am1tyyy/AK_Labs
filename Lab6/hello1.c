#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>
#include "hello1.h"

MODULE_AUTHOR("Borodai Vladyslav");
MODULE_DESCRIPTION("Simulate kmalloc error");
MODULE_LICENSE("Dual BSD/GPL");

static LIST_HEAD(hello_list);
static int alloc_counter = 0;
void print_hello(void)
{
	struct hello_entry *entry;
	entry = kmalloc(sizeof(*entry), GFP_KERNEL);

	alloc_counter++;
	if (alloc_counter == 5) {
		entry = NULL; //імітую помилку kmalloc()
	}

	// Використання entry без перевірки
	entry->time_before = ktime_get(); // Тут виникне помилка
	pr_info("Hello, world!\n");
	entry->time_after = ktime_get();

	list_add_tail(&entry->list, &hello_list);
}
EXPORT_SYMBOL(print_hello);

static void __exit hello1_exit(void)
{
	struct hello_entry *entry, *tmp;

	pr_info("Ending hello1 module and printing times:\n");

	list_for_each_entry_safe(entry, tmp, &hello_list, list) {
		pr_info("Time taken: %lld ns\n",
			ktime_to_ns(ktime_sub(entry->time_after, entry->time_before)));
		list_del(&entry->list);
		kfree(entry);
	}
}

module_exit(hello1_exit);
