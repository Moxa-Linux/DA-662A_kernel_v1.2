/*  Copyright (C) MOXA Inc. All rights reserved.

    This software is distributed under the terms of the
    MOXA License.  See the file COPYING-MOXA for details.
*/
/*
 * This is Moxa linux embedded platform series device driver.
 * This device driver control the Programable LED to enable/disable or turn on/off the LED.
 *
 * The UC-8481 device mapping memory:
 * Programmable LED: 0x53000050    For UC8481 hardware platform, Programmable LED enable/disable, on/off
 * 	bit_0	Action
 * 	--------------------------------------
 * 	0	Disable programmable LED
 * 	1	Enable programmable LED
 *	
 * 	bit_1	Action
 * 	--------------------------------------
 * 	0	LED off
 * 	1	LED on

 * History:
 * Date		Aurhor			Comment
 * 04-13-2012	Jared Wu.		Create for 1cut UC-8481
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>

/*
 * Debug
 */
//#define DEBUG

//;#define PROGRAMMABLE_LED_EN	0x10	/* 1: enable, 0: disable */
//#define PROGRAMMABLE_LED_ON	0x20	/* 1: on, 0: off */
#define PROGRAMMABLE_LED_EN	0x01	/* 1: enable, 0: disable */
#define PROGRAMMABLE_LED_ON	0x02	/* 1: on, 0: off */

struct slot_info {
	int     num;
	int     value;	// 1 for on, 0 for off
 };

#define MX_PROGRAMMABLE_LED_NAME	"moxa_pled"
#define MX_PROGRAMMABLE_LED_ADDR	0x53000050
#define MX_PROGRAMMABLE_LED_REGION_SIZE	0x1

static struct resource	*mx_pled_res=NULL;
static volatile unsigned char	*mx_pled_addr=NULL;
static struct mutex mx_pled_sem;

int pled_en_read_proc(char *page, char **start, off_t off,
			 int count, int *eof, void *data)
{
	s32 len=0;

	if ( off > count )
		return len;

	len += sprintf(page+len, "%d\n", \
		(readb(mx_pled_addr)&PROGRAMMABLE_LED_EN) ? 1 : 0);

	return len;
}

int pled_en_write_proc(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
	s32	pled_en_value=0;
	u8	kbuffer[64];
	u8	pled_bitmask=0;

	if ( copy_from_user(kbuffer, (char*)buffer , count) )
		return -EFAULT;

	kbuffer[count+1] = '\0';
	sscanf(kbuffer, "%d", &pled_en_value);
	//printk(KERN_DEBUG "%d\n", pled_en_value);

	if ( pled_en_value < 0 || pled_en_value > 1 ) {
		printk(KERN_ERR "The input value is not 0 or 1\n");
		return -EINVAL;
	}

	mutex_lock(&mx_pled_sem);

	pled_bitmask=readb(mx_pled_addr); // read the original pled_bitmask value
	if( pled_en_value == 1 ) {
		pled_bitmask |= PROGRAMMABLE_LED_EN ;
	}
	else {
		pled_bitmask &= (~PROGRAMMABLE_LED_EN) ;
	}

	writeb(pled_bitmask , mx_pled_addr);

	mutex_unlock(&mx_pled_sem);

	return count;
}

int pled_on_off_read_proc(char *page, char **start, off_t off,
			 int count, int *eof, void *data)
{
	int len=0;

	if ( off > count )
		return len;

	len += sprintf(page+len, "%d\n", \
		(readb(mx_pled_addr)&PROGRAMMABLE_LED_ON) ? 1 : 0);

	return len;
}

int pled_on_off_write_proc(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
	s32	pled_on_off_value=0;
	u8	kbuffer[64];
	u8	pled_bitmask=0;

	if ( copy_from_user(kbuffer, (char*)buffer , count) )
		return -EFAULT;

	kbuffer[count+1] = '\0';
	sscanf(kbuffer, "%d", &pled_on_off_value);
//	printk(KERN_DEBUG "%d\n", pled_on_off_value);

	if ( pled_on_off_value < 0 || pled_on_off_value > 1 ) {
		printk(KERN_ERR "The input value is not 0 or 1\n");
		return -EINVAL;
	}

	mutex_lock(&mx_pled_sem);

	pled_bitmask=readb(mx_pled_addr); // read the original pled_bitmask value

	if ( pled_on_off_value == 1 ) {
		pled_bitmask |= PROGRAMMABLE_LED_ON;
	}
	else {
		pled_bitmask &= (~PROGRAMMABLE_LED_ON);
	}

	writeb(pled_bitmask , mx_pled_addr);

	mutex_unlock(&mx_pled_sem);

	return count;
}

struct proc_dir_entry *pled_proc, *pled_en_proc, *pled_on_off_proc;

static int __init mx_pled_init_module (void) 
{
	printk("initializing MOXA programmable LED driver\n");

	/* register proc entry */
	pled_proc = proc_mkdir("driver/pled", NULL);
	if ( !pled_proc ) {
		printk("Moxa programmable LED driver: Create /proc/driver/pled fail !\n");
		goto pled_proc_create_fail;
	}

	pled_en_proc = create_proc_read_entry("driver/pled/en", 0644, 0, pled_en_read_proc, pled_proc);

	if ( !pled_en_proc ) {
		printk("Moxa programmable LED driver: Create /proc/driver/pled/en fail !\n");
		goto pled_en_proc_create_fail;
	}

	pled_en_proc->write_proc = pled_en_write_proc;

	pled_on_off_proc = create_proc_read_entry("driver/pled/on_off", 0644, 0, pled_on_off_read_proc, pled_proc);

	if ( !pled_on_off_proc ) {
		printk("Moxa programmable LED driver: Create /proc/driver/pled/on_off fail !\n");
		goto pled_on_off_proc_create_fail;
	}

	pled_on_off_proc->write_proc = pled_on_off_write_proc;

	mx_pled_res = request_mem_region(MX_PROGRAMMABLE_LED_ADDR, MX_PROGRAMMABLE_LED_REGION_SIZE, MX_PROGRAMMABLE_LED_NAME);
	if ( mx_pled_res == NULL ) {
		printk("Moxa programmable LED request memory region fail !\n");
		goto request_mem_region_error;
	}
	
	mx_pled_addr = ioremap(MX_PROGRAMMABLE_LED_ADDR, MX_PROGRAMMABLE_LED_REGION_SIZE);

	mutex_init(&mx_pled_sem);

	return 0;

request_mem_region_error:
	remove_proc_entry("driver/pled/on_off", NULL);
pled_on_off_proc_create_fail:
	remove_proc_entry("driver/pled/en", NULL);
pled_en_proc_create_fail:
	remove_proc_entry("driver/pled", NULL);
pled_proc_create_fail:
	return -EIO;
}

/*
 * close and cleanup module
 */
static void __exit mx_pled_cleanup_module (void) 
{
	printk("cleaning up MOXA programmable LED driver\n");

	if( mx_pled_addr )
		iounmap( (unsigned char*)mx_pled_addr );

	if ( mx_pled_res )
		release_mem_region(MX_PROGRAMMABLE_LED_ADDR, MX_PROGRAMMABLE_LED_REGION_SIZE);

	remove_proc_entry("driver/pled/on_off", NULL);
	remove_proc_entry("driver/pled/en", NULL);
	remove_proc_entry("driver/pled", NULL);
}

module_init(mx_pled_init_module);
module_exit(mx_pled_cleanup_module);
MODULE_AUTHOR("Jared_wu@moxa.com");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MOXA programmable LED driver for enable/disable, turn on/off the programmable LED");

