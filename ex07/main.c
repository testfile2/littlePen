#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/debugfs.h>
#include <linux/timer.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Knage");
MODULE_DESCRIPTION("Fortytwo module.");

static struct dentry *fortytwo_dir;
static DEFINE_MUTEX(cache_lock);
char str[4096];

static int id_open(struct inode *inode, struct file *file) {
	return 0;
}

static int id_close(struct inode *inodep, struct file *filp) {
	return 0;
}

static ssize_t id_write(struct file *file, const char __user *buf,
			size_t len, loff_t *pppos)
{
	if (strncmp("knage", buf, 5) == 0) {
		return 6;
	}
	return -EFAULT;
}

static ssize_t id_read(struct file *filep, char *buf, size_t len, loff_t *offset)
{
	char *login = "knage";
	ssize_t bytes = len < (5-(*offset)) ? len : (5-(*offset));
	if (copy_to_user(buf, login, bytes)) {
		return -EFAULT;
	}
	(*offset) += bytes;
	return bytes;
}

static const struct file_operations id_fops = {
	.owner	  =  THIS_MODULE,
	.open     =  id_open,
	.write    =  id_write,
	.read     =  id_read,
	.release  =  id_close,
	.llseek   =  no_llseek,
};

static ssize_t jiffies_read(struct file *filep, char *buf, size_t len, loff_t *offset)
{
	char jif_char[10];
	sprintf(jif_char, "%ld", jiffies);
	return simple_read_from_buffer(buf, len, offset, jif_char, 10);
}

static const struct file_operations jiffies_fops = {
	.read     =  jiffies_read,
};


static ssize_t foo_write(struct file *file, const char __user *buf,
			 size_t len, loff_t *offset)
{
	ssize_t res = 0;
	mutex_lock(&cache_lock);
	memset(str, 0, strlen(str));
	res = simple_write_to_buffer(str, 4096, offset, buf, len);
	mutex_unlock(&cache_lock);
	return res;
}

static ssize_t foo_read(struct file *filep, char *buf, size_t len, loff_t *offset)
{
	ssize_t ret = 0;
	mutex_lock(&cache_lock);
	ret = simple_read_from_buffer(buf, len, offset, str, strlen(str));
	mutex_unlock(&cache_lock);
	return ret;
}

static const struct file_operations foo_fops = {
	.write    =  foo_write,
	.read     =  foo_read,
};

static int __init hello_init(void)
{
	fortytwo_dir = debugfs_create_dir("fortytwo", NULL);
	if (!fortytwo_dir) {
		printk(KERN_INFO "Failed to create dir.\n");
		return 0;
	}

	debugfs_create_file("id", 0666, fortytwo_dir, NULL, &id_fops);
	debugfs_create_file("jiffies", 0444, fortytwo_dir, NULL, &jiffies_fops);
	debugfs_create_file("foo", 0644, fortytwo_dir, NULL, &foo_fops);
	return 0;
}

static void __exit hello_cleanup(void)
{
	debugfs_remove_recursive(fortytwo_dir);
	debugfs_remove(fortytwo_dir);
}

module_init(hello_init);
module_exit(hello_cleanup);
