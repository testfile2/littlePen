#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kcowle");
MODULE_DESCRIPTION("Hello World for wethinkcode");


static struct usb_device_id usb_table [] = {
	{ USB_DEVICE(0x258a, 0x0001)},
	{ }
};

MODULE_DEVICE_TABLE(usb, usb_table);

static int main_probe(struct usb_interface *interface,
	const struct usb_device_id *id)
{
	printk(KERN_DEBUG "Hello World!\n");
	return 0;
}

static void main_disconnect(struct usb_interface *interface)
{
}

static struct usb_driver USB_Keyboard_Driver = {
	.name		= "Universal USB keyboard Driver",
	.probe		= main_probe,
	.disconnect	= main_disconnect,
	.id_table	= usb_table
};

static int __init main_init(void)
{
	if (usb_register(&USB_Keyboard_Driver) < 0)
		printk(KERN_DEBUG "Loading of UKD failed.\n");
	else
		printk(KERN_DEBUG "Successfully loaded USB driver.\n");
	return 0;
}

static void __exit main_cleanup(void)
{
	usb_deregister(&USB_Keyboard_Driver);
	printk(KERN_DEBUG "Cleaning up module.\n");
}


module_init(main_init);
module_exit(main_cleanup);

