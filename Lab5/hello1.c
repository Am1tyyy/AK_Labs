#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"

// Структура для роботи зі списком
struct hello_node {
	struct list_head list;
	ktime_t time_before;
	ktime_t time_after;
};

// Голова списку
static LIST_HEAD(hello_list);

// Експортована функція
void print_hello(void) {
	struct hello_node *node;

	// Виділення пам'яті для нового вузла списку
	node = kmalloc(sizeof(*node), GFP_KERNEL);
	if (!node) {
		pr_err("Failed to allocate memory for list node\n");
		return;
	}

	// Засікаємо час до друку
	node->time_before = ktime_get();
	pr_info("Hello, world!\n");
	// Засікаємо час після друку
	node->time_after = ktime_get();

	// Додаємо вузол до списку
	list_add_tail(&node->list, &hello_list);
}
EXPORT_SYMBOL(print_hello);

// Вивантаження модуля
static void __exit hello1_exit(void) {
	struct hello_node *node, *tmp;

	// Прохід по списку, виведення часу друку і звільнення пам'яті
	list_for_each_entry_safe(node, tmp, &hello_list, list) {
		pr_info("Print took %lld ns\n",
			ktime_to_ns(ktime_sub(node->time_after, node->time_before)));
		list_del(&node->list);
		kfree(node);
	}

	pr_info("Goodbye from hello1 module\n");
}

// Ініціалізація модуля
static int __init hello1_init(void) {
	pr_info("Hello1 module loaded\n");
	return 0;
}

module_init(hello1_init);
module_exit(hello1_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Hello1 module exporting print_hello()");

