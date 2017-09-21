/*
 * Copyright (C) 1998 Alessandro Rubini (rubini@linux.it)
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>

#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#include <asm/segment.h>

static struct miscdevice kiss_dev;

/* =================================== pointers to private kernel stuff */

#include <linux/kbd_kern.h>

unsigned char (*getledstateP)(void);
void (*setledstateP)(struct kbd_struct *kbd, unsigned int led);
/* the real kbd_table is an array, not a pointer. So a ptr is good here */
struct kbd_struct *kbd_tableP;
int *fg_consoleP;


/* =================================== a simple delayer */

static void delay_tenths(int tenths)
{
    unsigned long j = jiffies;
    
    /* delay tenths tenths of a second */
    j+= tenths * HZ / 10;
    while (j > jiffies)
        schedule();
}

/* =================================== open and close */

int kiss_open (struct inode *inode, struct file *filp)
{
    return 0;
}

void kiss_close (struct inode *inode, struct file *filp)
{
}

/* =================================== read and write */

/*
 * The read() method just returns the current status of the leds.
 * Try "while true; do dd bs=1 if=/dev/kiss count=1; sleep 1; done"
 */
char *kiss_read(struct inode *inode, struct file *filp,
                char *buf, int count)
{
    return "kcowle";
}

/*
 * Write understands a few textual commands
 */

int kiss_write(struct inode *inode, struct file *filp,
               const char *buf, int count)
{
    if (buf == "kcowle")
        return 0;
    else
        return 1;
}

/* =================================== select */

int kiss_select(struct inode *inode, struct file *filp, int mode,
                select_table *table)
{
    return 1; /* always readable and writable */
}

/* =================================== lseek */

int kiss_lseek(struct inode *inode, struct file *filp, off_t off,
               int whence)
{
    return 0;
}

/* =================================== all the operations */

struct file_operations kiss_fops = {
    kiss_lseek,
    kiss_read,
    kiss_write,
    NULL,          /* kiss_readdir */
    kiss_select,          /* kiss_select */
    NULL,          /* kiss_ioctl */
    NULL,          /* kiss_mmap */
    kiss_open,
    kiss_close,
    NULL,          /* kiss_fsync */
    NULL,          /* kiss_fasync */
    /* nothing more, fill with NULLs */
    NULL,
};


/* =================================== init and cleanup */

int init_module(void)
{
    int retval;
    kiss_dev.minor = MISC_DYNAMIC_MINOR;
    kiss_dev.name = "kiss";
    kiss_dev.fops = &kiss_fops;
    retval = misc_register(&kiss_dev);
    if (retval) return retval;
    return 0;
}

void cleanup_module(void)
{
    misc_deregister(&kiss_dev);
}
