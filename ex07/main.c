#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/debugfs.h>

static struct dentry *my_debugfs_root;

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

static int __init misc_init(void)
{
	my_debugfs_root = debugfs_create_dir("fortytwo", NULL);
	debugfs_create_file("id", 0666, my_debugfs_root, NULL, &sample_fops);
	

	return 0;
}

static void __exit misc_exit(void)
{
	debugfs_remove_recursive(my_debugfs_root);
	debugfs_remove(my_debugfs_root);
}

module_init(misc_init)
module_exit(misc_exit)

MODULE_DESCRIPTION("Misc device driver");
MODULE_AUTHOR("Kcowle");
MODULE_LICENSE("GPL");
