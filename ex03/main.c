#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>

/*
 * Sleep. If we slept for long enough tell user space about it
*/

int do_work(int *my_int, int retval)
{
        int x;
        int temp = *my_int;
        int ret;
        for (x = 0; x < *my_int; ++x)
		udelay(10);
	if (temp < 10)
		pr_info("We slep't a long time!");
	ret = x * temp;
	return ret;
}

int my_init(void)
{
	int ret = 10;
	ret = do_work(&ret, ret);
	return 0;
}

void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);
