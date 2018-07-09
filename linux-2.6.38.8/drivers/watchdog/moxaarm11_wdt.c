/* 
 * linux/drivers/watchdog/moxaarm11_wdt.c
 *
 * Copyright 2010 Moxa Tech., Inc.
 *	Vincent Wu.<vincent.wu@moxa.com>
 *	http://www.moxa.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/watchdog.h>
#include <linux/miscdevice.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/signal.h>

#include <mach/hardware.h>
#include <mach/map.h>
#include <mach/irqs.h>
#include <mach/system-control.h>

#if 1	// added by Victor Yu.2015/6/10
#define CLOCK_VALUE		49500000	// 49.5M
#define MSEC_TO_COUNT(x)	((CLOCK_VALUE/1000)*(x))
static int	COUNT_TO_MSEC(u32 count) {
	u32	ret;

	if ( count >= (0xFFFFFFFF/1000) ) {
		if ( count >= (0xFFFFFFFF/100) ) {
			if ( count >= (0xFFFFFFFF/10) ) {
				ret = count / CLOCK_VALUE;
				ret *= 1000;
			} else {
				ret = count * 10;
				ret /= CLOCK_VALUE;
				ret *= 100;
			}
		} else {
			ret = count * 100;
			ret /= CLOCK_VALUE;
			ret *= 10;
		}
	} else {
		ret = count * 1000;
		ret /= CLOCK_VALUE;
	}
	return (int)ret;
}
#define DEFAULT_WDT_COUNT	MSEC_TO_COUNT(30000)	// added by Victor Yu. 2015/6/10

static int nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, int, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started "
	"(default=" __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

static unsigned long wdt_status;
#define WDT_IN_USE              0
#define WDT_OK_TO_CLOSE         1
#endif

#define SUPPORT_OLD_FEATURE	// added by Victor Yu. 2015/6/10
#ifdef SUPPORT_OLD_FEATURE 	// added by Victor Yu. 2015/6/10, to support old Moxa watchdog ioctl
#define IOCTL_SWATCHDOG_ENABLE  1
#define IOCTL_SWATCHDOG_DISABLE 2
#define IOCTL_SWATCHDOG_GET     3
#define IOCTL_SWATCHDOG_ACK     4
#endif

#ifndef __ASM_ARCH_REGS_WDT_H
#define __ASM_ARCH_REGS_WDT_H

/*
* Watch Dog Control Registers
*/
struct moxaarm11_wdt_regs_t{
	u32 counter_status;
	u32 counter_reload;
	u32 counter_restart;
	u32 control;
	u32 timeout_status;
	u32 clear_timeout_status;
	u32 reset_width;
};

enum moxaarm11_wdt_counter_restart{
	counter_restart_cmd = 0x4755,
};

enum moxaarm11_wdt_control{
	clock_select = BIT(4),
	external_signal_enable = BIT(3),
	interrupt_enable = BIT(2),
	reset_enable = BIT(1),
	wdt_enable = BIT(0),
};

enum moxaarm11_timeout_status{
	timeout_occur = BIT(0),
};

enum moxaarm11_clear_timeout_status{
	clear_timeout_status = BIT(0),
};

enum moxaarm11_reset_width{
	reset_width_mask = 0xFF,
};

struct moxaarm11_wdt_private_data_t{
	struct device	*dev;
	u32 irq;	
	u32 physical_base_addr;
	u32 virtual_base_addr;
	u32 reg_len;
	u32 wdt_count;
#ifdef SUPPORT_OLD_FEATURE 	// added by Victor Yu. 2015/6/10
	int	startflag;	// enable or disable
#endif
};

static struct moxaarm11_wdt_regs_t *wdt_regs;
static struct moxaarm11_wdt_private_data_t wdt_priv;

#endif /*__ASM_ARCH_REGS_WDT_H*/

//#define CONFIG_MOXAARM11_WDT_DBG
#ifdef CONFIG_MOXAARM11_WDT_DBG
	#define debug_printk(msg...) printk(msg); 
#else
	#define debug_printk(msg...)
#endif 

static const struct watchdog_info moxaarm11_wdt_ident = {
	.options	 = WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE,
	.firmware_version= 0,
	.identity	 = "MoxaARM11WatchDog",
};

static void moxaarm11_wdt_start(void)
{
#ifdef SUPPORT_OLD_FEATURE 	// added by Victor Yu. 2015/6/10
	wdt_priv.startflag = 1;
#endif
#if 0	// masked by Victor Yu. 2015/6/10
	wdt_priv.wdt_count = 4096;
#endif
#if 1	// added by Victor Yu. 2015/6/10
	writel(wdt_priv.wdt_count, &wdt_regs->counter_reload);
#endif
	writel(wdt_enable|external_signal_enable, &wdt_regs->control);
	debug_printk("WDT control register=%08x\n", readl(&wdt_regs->control));
}

static void moxaarm11_wdt_stop(void)
{
#ifdef SUPPORT_OLD_FEATURE 	// added by Victor Yu. 2015/6/10
	wdt_priv.startflag = 0;
#endif
	writel(0, &wdt_regs->control);
	debug_printk("WDT control register=%08x\n", readl(&wdt_regs->control));
}

static void moxaarm11_wdt_keepalive(void)
{
	writel(counter_restart_cmd, &wdt_regs->counter_restart);
#if 0	// masked by Victor Yu. 2015/6/10
	wdt_priv.wdt_count = readl(&wdt_regs->counter_reload);
#endif
	debug_printk("%s:WDT reload register=%08x\n", __FUNCTION__, wdt_priv.wdt_count);
}

static unsigned int moxaarm11_wdt_getsts(void)
{
	u32 counter_status;
	counter_status = readl(&wdt_regs->counter_status);
	debug_printk("%s:WDT counter status register=%08x\n", __FUNCTION__, counter_status);
	return counter_status;
}

static void moxaarm11_wdt_settimeout(unsigned int new_time)
{
#if 1	// added by Victor Yu. 2015/6/10
	wdt_priv.wdt_count = new_time;
#endif
	writel(new_time, &wdt_regs->counter_reload);
	debug_printk("%s:WDT reload register=%08x\n", __FUNCTION__, readl(&wdt_regs->counter_reload));
}

static ssize_t moxaarm11_wdt_write(struct file *file, const char *data, size_t len, loff_t *ppos)
{
        if ( !len )
                return 0;

        if (!nowayout) {
                size_t i;

                clear_bit(WDT_OK_TO_CLOSE, &wdt_status);

                for (i = 0; i != len; i++) {
                        char c;

                        if (get_user(c, data + i))
                                return -EFAULT;
                        if (c == 'V') {
                                set_bit(WDT_OK_TO_CLOSE, &wdt_status);
                                break;
			}
                }
        }
        moxaarm11_wdt_keepalive();

        return len;
}

static long moxaarm11_wdt_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	unsigned int new_count;
	unsigned int curr_sts;

	switch (cmd) {
#ifdef SUPPORT_OLD_FEATURE 	// added by Victor Yu. 2015/6/10, to support Moxa old watchdog ioctl
	case IOCTL_SWATCHDOG_ENABLE :
		copy_from_user(&new_count, argp, sizeof(unsigned int));
		debug_printk("%s,%s,%d:get new time=%d\n", __FILE__, __FUNCTION__, __LINE__, new_count);
		new_count = MSEC_TO_COUNT(new_count);
		debug_printk("%s,%s,%d:get new count=%d\n", __FILE__, __FUNCTION__, __LINE__, new_count);
		moxaarm11_wdt_settimeout(new_count);
		moxaarm11_wdt_start();
		moxaarm11_wdt_keepalive();
		return 0;
	case IOCTL_SWATCHDOG_DISABLE :
		moxaarm11_wdt_stop();
		return 0;
	case IOCTL_SWATCHDOG_GET :
		{
		struct {
			int		mode;
			unsigned long	time;
		} nowset;
		nowset.mode = wdt_priv.startflag;
		nowset.time = COUNT_TO_MSEC(wdt_priv.wdt_count);
		debug_printk("%s,%s,%d:now count=%d\n", __FILE__, __FUNCTION__, __LINE__, wdt_priv.wdt_count);
		debug_printk("%s,%s,%d:now time=%d\n", __FILE__, __FUNCTION__, __LINE__, COUNT_TO_MSEC(wdt_priv.wdt_count));
		return copy_to_user(argp, &nowset, sizeof(nowset));
		}
	case IOCTL_SWATCHDOG_ACK :
		moxaarm11_wdt_keepalive();
		return 0;
#endif

	case WDIOC_GETSUPPORT:
		return  copy_to_user((struct watchdog_info *)arg, &moxaarm11_wdt_ident, sizeof(moxaarm11_wdt_ident)) ? -EFAULT : 0;
	case WDIOC_GETBOOTSTATUS:
	case WDIOC_GETSTATUS:
		curr_sts = moxaarm11_wdt_getsts();
		return copy_to_user(argp, &curr_sts, sizeof(unsigned int));
	case WDIOC_KEEPALIVE:
		moxaarm11_wdt_keepalive();
		return 0;
	case WDIOC_GETTIMEOUT:
		return copy_to_user(argp, &wdt_priv.wdt_count, sizeof(unsigned int));
	case WDIOC_SETTIMEOUT:
		copy_from_user(&new_count, argp, sizeof(unsigned int));
		moxaarm11_wdt_settimeout(new_count);
		moxaarm11_wdt_keepalive();
		return 0;
	default:
		break;
	}
	return -ENOTTY;
}

static int moxaarm11_wdt_open(struct inode *inode, struct file *file)
{
	/*start the timer*/
	debug_printk("%s:", __FUNCTION__);
	if (test_and_set_bit(WDT_IN_USE, &wdt_status))
                return -EBUSY;

	// FIXME: Jared, 03-27-2017, In nowayout mode, the watchdog timer should not be stopped in the released entry.
	if ( !nowayout )
		clear_bit(WDT_OK_TO_CLOSE, &wdt_status);

	moxaarm11_wdt_start();

	return nonseekable_open(inode, file);
}

static ssize_t moxaarm11_wdt_release(struct inode *inode, struct file *file)
{
	debug_printk("%s:", __FUNCTION__);


#if 1	/* 04-12-2017, Jared said: The system should reset when got coredump */
	sigset_t *sigset = &current->pending.signal;

	if ( current->signal->group_exit_code == SIGQUIT || \
		current->signal->group_exit_code == SIGILL || \
		current->signal->group_exit_code == SIGABRT || \
		current->signal->group_exit_code == SIGFPE || \
		current->signal->group_exit_code == SIGSEGV ) {

			printk(KERN_CRIT "WATCHDOG: coredump and device closed unexpectedly - timer will not stop\n") ;

			moxaarm11_wdt_keepalive();

		return 0;
	}
	else if ( signal_pending(current) ) {

		printk(KERN_CRIT "WATCHDOG: got signal\n");

		/*
		if ( sigismember (sigset, SIGKILL) || \
		    sigismember (sigset, SIGINT) || \
		    sigismember (sigset, SIGTERM) || \
		    sigismember (sigset, SIGQUIT) || 
		    sigismember (sigset, SIGBUS) || \
		    sigismember (sigset, SIGFPE) ) {

			//printk(KERN_CRIT "WATCHDOG: Device closed expectedly - timer will be stopped\n");
		}
		*/
	}
	else 
#endif
	if ( test_bit(WDT_OK_TO_CLOSE, &wdt_status) ) {

		printk("WATCHDOG: Device closed expectedly - timer will be stopped\n");

	}
	
	printk("WATCHDOG: Device closed expectedly - timer will be stopped\n");
	moxaarm11_wdt_stop();
	clear_bit(WDT_IN_USE, &wdt_status);
	clear_bit(WDT_OK_TO_CLOSE, &wdt_status);
	wdt_priv.wdt_count = DEFAULT_WDT_COUNT; // added by Victor yu. 2015/6/10

	return 0;
}

static const struct file_operations moxaarm11_wdt_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl	= moxaarm11_wdt_ioctl,
	.open		= moxaarm11_wdt_open,
	.write		= moxaarm11_wdt_write,
	.release	= moxaarm11_wdt_release,
};

static struct miscdevice moxaarm11_wdt_miscdev = {
	.minor		= WATCHDOG_MINOR,
	.name		= "watchdog",
	.fops		= &moxaarm11_wdt_fops,
};

/*interrupr handler*/
static irqreturn_t moxaarm11_wdt_irq(int irqno, void *param)
{
	writel(clear_timeout_status, &wdt_regs->clear_timeout_status);
	dev_info(wdt_priv.dev, "watchdog timer expired! (irq) \n");
	printk("watchdog timer expired! (irq) \n");

	return IRQ_HANDLED;
}

static int moxaarm11_wdt_probe(struct platform_device *pdev)
{
	struct resource	*res;
	int	ret=0;

	wdt_priv.dev = &pdev->dev;

	debug_printk("%s: Probe=%p\n", __FUNCTION__, pdev);
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res == NULL) {
		dev_err(wdt_priv.dev, "no memory resource specified\n");
		return -ENOENT;
	}

	wdt_priv.physical_base_addr = res->start;
	wdt_priv.reg_len= (res->end - res->start) + 1;
	if (!request_mem_region(wdt_priv.physical_base_addr, wdt_priv.reg_len, pdev->name)) {
		dev_dbg(wdt_priv.dev, "failed to request memory region\n");
		ret = -EBUSY;
		goto err_req;
	}

	wdt_priv.virtual_base_addr = (u32)ioremap_nocache(wdt_priv.physical_base_addr, wdt_priv.reg_len);
	if (wdt_priv.virtual_base_addr == 0) {
		dev_dbg(wdt_priv.dev, "error mapping memory\n");
		ret = -EFAULT;
		goto err_req;
	}
	wdt_regs = (struct moxaarm11_wdt_regs_t *)wdt_priv.virtual_base_addr;
	debug_printk("\twdt_priv.virtual_base_addr=0x%x", wdt_priv.virtual_base_addr);

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (res == NULL) {
		dev_err(wdt_priv.dev, "no irq resource specified\n");
		ret = -ENOENT;
		goto err_req;
	}

	moxaarm11_io_pin_select(4, GPIOA_USED_FOR_OTHERS);

	wdt_priv.irq = res->start;
	if (request_irq(wdt_priv.irq, moxaarm11_wdt_irq, 0, pdev->name, pdev)) {
		dev_err(wdt_priv.dev, "failed to install irq\n");
		goto err_req;
	}
#ifdef SUPPORT_OLD_FEATURE 	// added by Victor Yu. 2015/6/10
	wdt_priv.startflag = 0;
#endif
#if 1	// added by Victor yu. 2015/6/10
	wdt_priv.wdt_count = DEFAULT_WDT_COUNT;
#endif

	ret = misc_register(&moxaarm11_wdt_miscdev);
	if (ret) {
		dev_err(wdt_priv.dev, "can't register miscdev on minor=%d (%d)\n",
			 WATCHDOG_MINOR, ret);
		goto err_irq;
	}
	debug_printk( "\twdt_minor=%d\n", WATCHDOG_MINOR);
err_irq:
	free_irq(wdt_priv.irq, pdev);
err_req:
	release_mem_region(wdt_priv.physical_base_addr, wdt_priv.reg_len);
	return ret;
}

static int moxaarm11_wdt_remove(struct platform_device *dev)
{
	release_mem_region(wdt_priv.physical_base_addr, wdt_priv.reg_len);
	free_irq(wdt_priv.irq, dev);
	misc_deregister(&moxaarm11_wdt_miscdev);
	return 0;
}

static struct platform_driver moxaarm11_wdt_driver = {
	.probe		= moxaarm11_wdt_probe,
	.remove		= moxaarm11_wdt_remove,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "MoxaARM11WatchDog",
	},
};

static int __init watchdog_init(void)
{
	printk("Load Moxa Macro watchdog device driver.\n");
	return platform_driver_register(&moxaarm11_wdt_driver);
}

static void __exit watchdog_exit(void)
{
	printk("Unload Moxa Macro watchdog device driver.\n");
	platform_driver_unregister(&moxaarm11_wdt_driver);
}

module_init(watchdog_init);
module_exit(watchdog_exit);

MODULE_AUTHOR("Vincent Wu");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MOXAARM11 Watchdog Device Driver");
