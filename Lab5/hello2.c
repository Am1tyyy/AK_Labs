#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include "hello1.h"

// Параметр для кількості викликів функції
static uint hello_count = 1;
module_param(hello_count, uint, 0644);
MODULE_PARM_DESC(hello_count, "Number of times to call print_hello");

// Ініціалізація модуля
static int __init hello2_init(void) {
	int i;

	pr_info("Hello2 module loaded with hello_count=%u\n", hello_count);

	// Перевірка значення параметра
	if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10)) {
		pr_warn("hello_count is 0 or between 5 and 10: continuing.\n");
	} else if (hello_count > 10) {
		pr_err("hello_count is greater than 10: module not loaded.\n");
		return -EINVAL;
	}

	// Виклик функції print_hello() задану кількість разів
	for (i = 0; i < hello_count; i++) {
		print_hello();
	}

	return 0;
}

// Завершення роботи модуля
static void __exit hello2_exit(void) {
	pr_info("Goodbye from hello2 module\n");
}

module_init(hello2_init);
module_exit(hello2_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Hello2 module using print_hello()");

