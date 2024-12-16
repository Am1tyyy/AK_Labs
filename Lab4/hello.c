#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

// Оголошення параметра модуля
static uint hello_count = 1;	// Значення за замовчанням
module_param(hello_count, uint, 0644);
MODULE_PARM_DESC(hello_count, "Number of times to print Hello, world!");

// Ініціалізація модуля
static int __init hello_init(void) {
	int i;

	pr_info("Hello module loaded with hello_count=%u\n", hello_count);

	// Перевірка значення параметра
	if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10)) {
		pr_warn("hello_count is 0 or between 5 and 10: continuing.\n");
	} else if (hello_count > 10) {
		pr_err("hello_count is greater than 10: module not loaded.\n");
		return -EINVAL;
	}

	// Виведення "Hello, world!" задану кількість разів
	for (i = 0; i < hello_count; i++) {
		pr_info("Hello, world!\n");
	}

	return 0;
}

// Завершення роботи модуля
static void __exit hello_exit(void) {
	pr_info("Goodbye, world!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Borodai Vladyslav");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");


