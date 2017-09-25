#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

static int sample_open(struct inode *inode, struct file *file)
{
        return 0;
}

static int sample_close(struct inode *inodep, struct file *filp)
{
   	return 0;
}

static ssize_t sample_write(struct file *file, const char *buf,
                            size_t len, loff_t *ppos)
{

	char msg[len];
	size_t cmp;

	copy_from_user(msg, buf, len);
	cmp = strncmp(buf, "kcowle", 3);
	if (cmp != 0)
	{
		printk(KERN_DEBUG "Failed, returning-EINVAL\n");
		return -EINVAL;
	}
	else
	{
		printk(KERN_DEBUG "Pass Returning len\n");
		return len;
	}
	return -EINVAL;
}

static ssize_t main_read(struct file *flip,
			char *buf, size_t count, loff_t *offset)
{
	char *login = "kcowle";
	int index = 0;
	
	if (buf[0] == 'k')
	{
		put_user('\0', buf);
		return 0;
	}
	while (count && (login[index] != 0))
	{
		put_user(login[index], buf++);
		count--;
		index++;
	}
	put_user('\0', buf);
	return 7;
}

static const struct file_operations sample_fops = {
    .read			= main_read,
    .write			= sample_write,
    .open			= sample_open,
    .release			= sample_close,
    .llseek 			= no_llseek,
};

struct miscdevice sample_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "misc char device driver",
    .fops = &sample_fops,
};

static int __init misc_init(void)
{
    int error;
    
    error = misc_register(&sample_device);
    if (error) {
        return error;
    }
    
    return 0;
}

static void __exit misc_exit(void)
{
    misc_deregister(&sample_device);
}

module_init(misc_init)
module_exit(misc_exit)

MODULE_DESCRIPTION("Misc device driver");
MODULE_AUTHOR("Kcowle");
MODULE_LICENSE("GPL");
