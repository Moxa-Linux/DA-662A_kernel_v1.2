/*  Copyright (C) MOXA Inc. All rights reserved.

    This software is distributed under the terms of the
    MOXA License.  See the file COPYING-MOXA for details.
*/
/*
 * This is Moxa linux embedded platform series device driver.
 * This device driver will control the USB GPIO to power on/off the PH8 module.
 *
 * The UC-8481 device mapping memory:
 * reset-button	: 0x53000080      For UC8481 hardware platform, PH8 Power on/of, IGD control
 * 	Power on/off function for PH8 module, 3 seconds
 * 		GPIO_1	GPIO_0	Action
 *	 	-----------------------------
 *	 	0	0	Standby
 * 		0	1	PCIE1_OFF
 * 		1	0	PCIE2_OFF
 *		1	1	PCIE3_OFF
 *
 * 	Power IGT signal for initialize the PH8 module, 1 seconds
 * 		GPIO_3	GPIO_2	Action
 * 		-----------------------------
 * 		0	0	Standby
 * 		0	1	PCIE1_GPO#
 * 		1	0	PCIE2_GPO#
 *		1	1	PCIE3_GPO#
 *
 * History:
 * Date		Aurhor			Comment
 * 10-20-2011	Jared Wu.		Create it.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/mutex.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <asm/uaccess.h>

/*
 * Debug
 */
//#define DEBUG
#ifdef DEBUG 
#define p(fmt, args...) printk(MX_USB_GPIO_NAME "%s: "fmt, __FUNCTION__,##args)
#else
#define p(fmt, args...) 
#endif

/*
 * Ioctl
 */
#define IOCTL_SET_SLOT_POWER	_IOW(MX_USB_GPIO_MINOR,0,int)
#define IOCTL_SET_SLOT_IGT	_IOW(MX_USB_GPIO_MINOR,1,int)

struct slot_info {
	int     num;
	int     value;	// 1 for on, 0 for off
 };


#define MX_USB_GPIO_MINOR	108
#define MX_USB_GPIO_NAME	"moxa_usb_gpio"
#define MX_USB_GPIO_ADDR	0x53000080
#define MX_USB_GPIO_REGION_SIZE	0x1

static struct resource	*mx_usb_gpio_res=NULL;
static volatile unsigned char	*mx_usb_gpio_addr=NULL;
static struct mutex mx_usb_gpio_sem;

static int usb_gpio_open (struct inode *inode, struct file *file) 
{
	return 0;
}

static int usb_gpio_release (struct inode *inode, struct file *file) 
{
	return 0;
}


/* 
 *  I/O control 
 */
static long usb_gpio_ioctl(struct file *file,unsigned int cmd, unsigned long arg) 
{
	struct slot_info   si;

	if ( copy_from_user(&si, (struct slot_info *)arg, sizeof(struct slot_info)) )
		return -EFAULT;

	p("si.num=0x%x, si.value=0x%x\n", si.num, si.value);

	mutex_lock(&mx_usb_gpio_sem);

	switch (cmd) {
		case IOCTL_SET_SLOT_POWER:
			switch (si.num) {
				case 1:
					if ( si.value == 0 )
						*mx_usb_gpio_addr |= 0x01;
					else
						*mx_usb_gpio_addr = 0;
					break;
				case 2:
					if ( si.value == 0 )
						*mx_usb_gpio_addr |= 0x02;
					else
						*mx_usb_gpio_addr = 0;
					break;
				case 3:
					if ( si.value == 0 )
						*mx_usb_gpio_addr |= 0x03;
					else
						*mx_usb_gpio_addr = 0;
					break;
				default:
					printk("slot number:%d, slot value:%d\n", si.num, si.value);
					mutex_unlock(&mx_usb_gpio_sem);
					return -EINVAL;
			}
			break;

		case IOCTL_SET_SLOT_IGT:
			switch (si.num) {
				case 1:
					if ( si.value == 0 )
						*mx_usb_gpio_addr |= 0x04;
					else
						*mx_usb_gpio_addr = 0;
					break;
				case 2:
					if ( si.value == 0 )
						*mx_usb_gpio_addr |= 0x08;
					else
						*mx_usb_gpio_addr = 0;
					break;
				case 3:
					if ( si.value == 0 )
						*mx_usb_gpio_addr |= 0x0c;
					else
						*mx_usb_gpio_addr = 0;
					break;
				default:
					printk("slot number:%d, slot value:%d\n", si.num, si.value);
					mutex_unlock(&mx_usb_gpio_sem);
					return -EINVAL;
			}
			break;
		default:
			printk("Incorrect parameter, cmd(%x)!=IOCTL_SET_SLOT_POWER(%x) or  IOCTL_SET_SLOT_POWER(%x)\n", cmd, IOCTL_SET_SLOT_POWER, IOCTL_SET_SLOT_IGT);
			mutex_unlock(&mx_usb_gpio_sem);
			return -EINVAL;
			break;
	}

	mutex_unlock(&mx_usb_gpio_sem);

	return 0;
}

static struct file_operations mx_usb_gpio_fops = {
	.owner	=	THIS_MODULE,
	.unlocked_ioctl	=	usb_gpio_ioctl,
	.open	=	usb_gpio_open,
	.release=	usb_gpio_release,
};

static struct miscdevice mx_usb_gpio_dev = {
	.minor = MX_USB_GPIO_MINOR,
	.name = MX_USB_GPIO_NAME,
	.fops = &mx_usb_gpio_fops,
};

static int __init usb_gpio_init_module (void) 
{
	int res;
	printk("initializing MOXA USB GPIO driver\n");

	/* register misc driver */
	res = misc_register(&mx_usb_gpio_dev);
	if ( res < 0 ) {
		printk("Moxa USB GPIO driver: Register misc fail !\n");
		goto misc_register_error;
	}

	mx_usb_gpio_res = request_mem_region(MX_USB_GPIO_ADDR, MX_USB_GPIO_REGION_SIZE, MX_USB_GPIO_NAME);
	if ( mx_usb_gpio_res == NULL ) {
		printk("Moxa USB GPIO request memory region fail !\n");
		goto request_mem_region_error;
	}
	
	mx_usb_gpio_addr = ioremap(MX_USB_GPIO_ADDR, MX_USB_GPIO_REGION_SIZE);

	mutex_init(&mx_usb_gpio_sem);

	return 0;

request_mem_region_error:
	misc_deregister(&mx_usb_gpio_dev);
misc_register_error:
	return -EIO;
}

/*
 * close and cleanup module
 */
static void __exit usb_gpio_cleanup_module (void) 
{
	printk("cleaning up MOXA USB GPIO driver\n");

	if( mx_usb_gpio_addr )
		iounmap( (unsigned char*)mx_usb_gpio_addr );

	if ( mx_usb_gpio_res )
		release_mem_region(MX_USB_GPIO_ADDR, MX_USB_GPIO_REGION_SIZE);

	misc_deregister(&mx_usb_gpio_dev);
}

module_init(usb_gpio_init_module);
module_exit(usb_gpio_cleanup_module);
MODULE_AUTHOR("Jared_wu@moxa.com");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MOXA USB GPIO driver for power off/on the PH8 module");

