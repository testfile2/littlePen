#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kcowle");
MODULE_DESCRIPTION("Hello World for wethinkcode");

static int __init main_init(void)
{
	printk(KERN_DEBUG "Hello World!\n");
	return 0;
}

static void __exit main_cleanup(void)
{
	printk(KERN_DEBUG "Cleaning up module.\n");
}

module_init(main_init);
module_exit(main_cleanup);
