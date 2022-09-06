#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("eboris");
MODULE_DESCRIPTION("Allignment 01 in Little Penguin");
MODULE_VERSION("1.0");

static int __init lp_hello_init(void) {
	printk(KERN_INFO "Hello, World !\n");
	return 0;
}

static void __exit lp_hello_exit(void) {
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(lp_hello_init);
module_exit(lp_hello_exit);