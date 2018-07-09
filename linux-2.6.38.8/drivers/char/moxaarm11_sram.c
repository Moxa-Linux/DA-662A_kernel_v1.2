/* 
 * linux/drivers/char/moxaarm11_sram.c
 *
 * Copyright 2010 Moxa Tech., Inc.
 *	Vincent Wu.<vincent.wu@moxa.com>
 *	http://www.moxa.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This driver is for SRAM burning test only.
 *
 * Device node: /dev/sram with major number 10, minor number 105
 *
*/


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <mach/map.h>

#define MOXAARM11_SRAM_MINOR		105


 
static long moxaarm11_sram_unlocked_octl( struct file *file, unsigned int cmd, unsigned long arg)
{
	return 0;
}

static int moxaarm11_sram_open(struct inode *inode, struct file *file)
{
	if ( MINOR(inode->i_rdev) == MOXAARM11_SRAM_MINOR){
		inode->i_size = SZ_64K;
		return 0;
	}
	else	
		return -ENODEV;
}

static int moxaarm11_sram_release(struct inode *inode, struct file *file)	
{
	return 0;
}

static ssize_t moxaarm11_sram_read(struct file *file, char __user *user_char, size_t size, loff_t *loff)
{
	char *sram_base = (char *)MOXAARM11_VA_SRAM_BASE;
	char *read_addr = sram_base + file->f_pos;
	long retval;

	if(size>0){
		retval = copy_to_user(user_char, read_addr, size);
		if(retval == 0){
			//printk("moxaarm11_sram_read: %ld bytes\n", size);
			file->f_pos += size;
			return size;
		}
	}

	return size;
}

static ssize_t moxaarm11_sram_write(struct file *file, const char __user *user_char, size_t size, loff_t *loff)
{
	char *sram_base = (char *)MOXAARM11_VA_SRAM_BASE;
	char *write_addr = sram_base + file->f_pos;
	long retval;

	if(size>0){
		retval = copy_from_user(write_addr, user_char, size);
		if(retval == 0){
			//printk("moxaarm11_sram_write: %ld bytes\n", size);
			file->f_pos += size;
			return size;
		}
	}

	return size;
}

static struct file_operations moxaarm11_sram_fops = {
	owner:THIS_MODULE,
	llseek:generic_file_llseek,
	read:moxaarm11_sram_read,
	write:moxaarm11_sram_write,
	unlocked_ioctl:moxaarm11_sram_unlocked_octl,
	open:moxaarm11_sram_open,
	release:moxaarm11_sram_release,
};



static struct miscdevice moxaarm11_sram_dev = {
	MOXAARM11_SRAM_MINOR,
	"SRAM",
	&moxaarm11_sram_fops
};



static void __exit moxaarm11_sram_exit(void)
{
	misc_deregister(&moxaarm11_sram_dev);
}

static int __init moxaarm11_sram_init(void)
{
	printk("Register SRAM as MISC device [12-092011] ");
	if ( misc_register(&moxaarm11_sram_dev) ) {
		printk("failed!\n");
		return -ENOMEM;
	}

	printk("OK.\n");
	return 0;	
}

module_init(moxaarm11_sram_init);
module_exit(moxaarm11_sram_exit);

MODULE_AUTHOR("Vincent Wu");
MODULE_LICENSE("GPL");
