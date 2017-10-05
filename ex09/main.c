#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/dcache.h>
#include <linux/init.h>
#include <linux/mount.h>
#include <linux/path.h>
#include <linux/namei.h>
#include <linux/string.h>

static int __init misc_init(void)
{
	struct path *path = NULL;
	struct dentry *thedentry;
	struct dentry *curdentry;
	char *temp;

	kern_path("/", LOOKUP_FOLLOW, path);
	thedentry = path->dentry;
	list_for_each_entry(curdentry, &thedentry->d_subdirs, d_child)
	{
		temp = (char *)kmalloc(sizeof(char *) * (strlen(curdentry->d_name.name) + 10), GFP_KERNEL);
		strcat(temp, "/");
		strncat(temp, curdentry->d_name.name, strlen(curdentry->d_name.name));
		kern_path(temp, LOOKUP_FOLLOW, path);
		if (path_is_mountpoint(path))
		{
			printk("%s: is a mountpoint", path->dentry->d_name.name);
		}
		else
			printk("%s: is not a mountpoint", path->dentry->d_name.name);
		kfree(temp);
	}
	
		
	return 0;
}

static void __exit misc_exit(void)
{
}

module_init(misc_init)
module_exit(misc_exit)

MODULE_DESCRIPTION("Misc device driver");
MODULE_AUTHOR("Kcowle");
MODULE_LICENSE("GPL");

