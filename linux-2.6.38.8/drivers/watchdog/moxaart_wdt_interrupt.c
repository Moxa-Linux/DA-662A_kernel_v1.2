/*
 *
 * drivers/char/watchdog/moxaart_wdt.c
 *
 * 11-26-2011   Wade Liang		Create it. See Documentation/watchdog for more detail 
 *
 */

#include <linux/io.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>

static int nowayout = WATCHDOG_NOWAYOUT;
static int timeout = 60;	/* (secs) Default is 1 minute */
static unsigned long wdt_status;
#define	WDT_IN_USE		0
#define	WDT_OK_TO_CLOSE		1
static DEFINE_SPINLOCK(wdt_lock);

/* Debug */
//#define DEBUG
#ifdef DEBUG
#define drv_dbg(fmt, args...) printk(KERN_DEBUG "%s: "fmt, __FUNCTION__, ##args)
#else 
#define drv_dbg(fmt, args...)
#endif
#define drv_err(fmt, args...) printk(KERN_ERR "moxaart_wdt: "fmt, ##args)
#define drv_info(fmt, args...) printk(KERN_INFO "moxaart_wdt: "fmt, ##args)

/*  
 *  APB_CLOCK is for bus and watchdog, APB=(arm peripheral bus), 
 *  APB_CLK= the HZ of bus for watchdog 
 */
#define WDT_GET_TICK_FROM_SEC(sec)	(APB_CLK * sec)

typedef struct _wdt_regs {
	unsigned long WdCounter; 
	unsigned long WdLoad;		/* Contain the value would be reload to WdCounter */
	unsigned long WdRestart;	/* Load WdLoad to WdCounter */
#define RestartReload 0x5ab9 
	unsigned long WdCR;
#define CREnable	0x01
#define CRRst		0x02		/* System Reset  */
#define CRIntr	0x04
	unsigned long WdStatus;		/* Set to 0 when the counter reaches zero */
	unsigned long WdClear;		/* Clear WdStatus */
	unsigned long WdIntrLen;
} wdt_regs_t; 
wdt_regs_t *wdt_regs; 

#ifdef DEBUG
static void dump_regs(wdt_regs_t *wr) {
	drv_dbg("%s\n", __FUNCTION__);
	drv_dbg("counter=%x\n", (unsigned int)wr->WdCounter);
	drv_dbg("load=%x\n", (unsigned int)wr->WdLoad);
	drv_dbg("restart=%x\n", (unsigned int)wr->WdRestart);
	drv_dbg("cr=%x\n", (unsigned int)wr->WdCR);
	drv_dbg("status=%x\n", (unsigned int)wr->WdStatus);
	drv_dbg("clear=%x\n", (unsigned int)wr->WdClear);
	drv_dbg("intrlen=%x\n", (unsigned int)wr->WdIntrLen);
}
#endif

static int flag_interrupt_mode = 1;
static int flag_soft_reboot = 1; 
static struct timer_list wdt_timer;
static int wdt_timer_start = 0;

static void wdt_soft_reboot(struct work_struct *work)
{
	char    *argv[2], *envp[5];

    if ( in_interrupt() )
            return;
    argv[0] = "/sbin/reboot";
    argv[1] = 0;
    envp[0] = "HOME=/";
    envp[1] = "PATH=/sbin:/bin:/usr/sbin:/usr/bin";
    envp[2] = 0;
    call_usermodehelper(argv[0], argv, envp, 0);
}
DECLARE_WORK(wq_soft_reboot, wdt_soft_reboot);


/* 
 * mode - 0: interrrupt mode, 1: reset mode 
 */
enum { MODE_INTERRUPT, MODE_RESET }; 
static void moxaart_wdt_set_mode_timeout(int mode, int tick) 
{
	spin_lock(&wdt_lock);
	wdt_regs->WdLoad = tick; 
	wdt_regs->WdRestart = RestartReload; 
	wdt_regs->WdCR = (mode == MODE_INTERRUPT)? CREnable | CRIntr : CREnable | CRRst; 
	spin_unlock(&wdt_lock);
}

EXPORT_SYMBOL(moxaart_wdt_set_mode_timeout); 

static void wdt_timer_timeout(unsigned long ignore)
{
    /* use 15 sec for soft reboot,  */
	moxaart_wdt_set_mode_timeout(MODE_INTERRUPT, WDT_GET_TICK_FROM_SEC(15));
    schedule_work(&wq_soft_reboot);
}

#define WRITE_STATE_MACHINE     (1<<7)      /* 1:Ready 0: Busy */
#define ERASE_SUSPEND           (1<<6)
#define ERASE_CLEAR_LCOK_BITSSTATUS (1<<5)
#define PROBRAM_SET_LOCK_BIT    (1<<4)
#define PROGRAM_VOLTAGE         (1<<3)
#define PROGRAM_SUSPEND         (1<<2)
#define DEVICE_PROTECT          (1<<1)

#define CMD_PROG_SUSPEND    0xb0    /* allow programming interrupt to read data in other flash memory */
#define CMD_PROG_RESUME     0xd0
#define CMD_READ_STATUS_REG 0x70
#define CMD_READ_ARRAY      0xff
/* 
 * Turn flash to suspend mode, and to read mode
 * to avoid watchdog reboot with flash in write mode
 * refer to intel Program suspend flow chat
 */
static irqreturn_t wdt_interrupt_handler(int irq, void *dev_id)
{
	static int i = 10000;
	static u8 status_reg;
	static spinlock_t 		lock=SPIN_LOCK_UNLOCKED;
	static unsigned long flags;
    volatile u8* cmd_addr = (u8 *)MOXAART_FLASH_VA_BASE+0x55*2; 

	drv_dbg("enter\n");
	spin_lock_irqsave(&lock, flags);

	/* get status register, if in erase suspend or programing suspend, add a D0 ehre to resume first */
    *cmd_addr = CMD_READ_STATUS_REG; 
	status_reg =  *cmd_addr; 
    drv_info("status_reg=%x\n", status_reg);

   if (!(status_reg & PROGRAM_SUSPEND) && !(status_reg & ERASE_SUSPEND)) {
        /* enter program_suspend or erase_suspend mode first */
        *cmd_addr = CMD_PROG_SUSPEND; 
        do {
            *cmd_addr = CMD_READ_STATUS_REG; 
            status_reg =  *cmd_addr; 
            drv_info("status_reg=%x\n", status_reg);
            if ((status_reg & WRITE_STATE_MACHINE)) {
                break;
            } 
            mdelay(10);
            i--;
        } while (i > 0);
    }

    *cmd_addr = CMD_READ_ARRAY; 
	mdelay(100);

	printk("Starting to reboot ... \n");

    moxaart_wdt_set_mode_timeout(MODE_RESET, 1);

	while(1);

	printk("should not run here\n");

	return IRQ_HANDLED;
	
}

static void wdt_enable(void)
{
	drv_dbg("enable timeout=%d tick=%lu\n", timeout, (unsigned long)WDT_GET_TICK_FROM_SEC(timeout));
    moxaart_wdt_set_mode_timeout(MODE_INTERRUPT, WDT_GET_TICK_FROM_SET(timeout)); 

    if (flag_soft_reboot) 
    {
        if (wdt_timer_start == 0) {
            init_timer(&wdt_timer); 
            wdt_timer.function = wdt_timer_timeout; 
            wdt_timer.expires = jiffies + WDT_GET_TICK_FROM_SEC(timeout) - HZ*50/1000; /* quick 50 ms than hardware */
            wdt_timer_start = 1; 
        } else {
            mod_timer(&wdt_timer, jiffies + WDT_GET_TICK_FROM_SEC(timeout)); 
        }
    }
}

static void wdt_disable(void)
{
	drv_dbg("disable\n");
	spin_lock(&wdt_lock);
	wdt_regs->WdCR = 0; 
	spin_unlock(&wdt_lock);

    if (flag_soft_reboot)
    {
        del_timer(&wdt_timer); 
        wdt_timer_start = 0; 
    }
}

static int moxaart_wdt_open(struct inode *inode, struct file *file)
{
    int ret; 

	drv_dbg("open\n");
	if (test_and_set_bit(WDT_IN_USE, &wdt_status))
		return -EBUSY;

	clear_bit(WDT_OK_TO_CLOSE, &wdt_status);
	wdt_enable();

    if (flag_interrupt_mode) {
        ret = request_irq(IRQ_WATCHDOG, wdt_interrupt_handler, IRQF_DISABLED, "wtd_irq", NULL); 
        if (ret) {
            drv_err("Can't assign interrupt\n"); 
        }
    }

	return nonseekable_open(inode, file);
}

static ssize_t
moxaart_wdt_write(struct file *file, const char *data, size_t len, loff_t *ppos)
{
	drv_dbg("write\n");
	if (len) {
		if (!nowayout) {
			size_t i;

			clear_bit(WDT_OK_TO_CLOSE, &wdt_status);

			for (i = 0; i != len; i++) {
				char c;

				if (get_user(c, data + i))
					return -EFAULT;
				if (c == 'V')
					set_bit(WDT_OK_TO_CLOSE, &wdt_status);
			}
		}
		wdt_enable();
	}
	return len;
}

static const struct watchdog_info ident = {
	.options	= WDIOF_MAGICCLOSE | WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING,
	.identity	= "Moxaart Watchdog",
};


/* 
 * Note: not support WDIOC_SETOPTIONS due to it can be replaced 
 * by open and write opertions 
 */
static long moxaart_wdt_ioctl(struct file *file, unsigned int cmd,
							unsigned long arg)
{
	int ret = -ENOTTY;
	int time;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		ret = copy_to_user((struct watchdog_info *)arg, &ident,
				   sizeof(ident)) ? -EFAULT : 0;
        drv_dbg("options=%x identity=%s\n", ident.options, ident.identity);
		break;

	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
        /* not support */
		ret = put_user(0, (int *)arg);
		break;

	case WDIOC_KEEPALIVE:
		wdt_enable();
		ret = 0;
		break;

	case WDIOC_SETTIMEOUT:
		ret = get_user(time, (int *)arg);
		if (ret)
			break;

		if (time <= 0 || time > 60) {
			ret = -EINVAL;
			break;
		}

		timeout = time;
		wdt_enable();
        break; 

	case WDIOC_GETTIMEOUT:
        drv_dbg("timeout=%d\n", timeout);
		ret = put_user(timeout, (int *)arg);
		break;
	}
	return ret;
}

static int moxaart_wdt_release(struct inode *inode, struct file *file)
{
    drv_dbg("release"); 
	if (test_bit(WDT_OK_TO_CLOSE, &wdt_status))
		wdt_disable();
	else
		drv_info("Device closed unexpectedly - timer will not stop\n");

    if (flag_interrupt_mode)
        free_irq(IRQ_WATCHDOG, NULL); 

	clear_bit(WDT_IN_USE, &wdt_status);
	clear_bit(WDT_OK_TO_CLOSE, &wdt_status);

	return 0;
}

static const struct file_operations moxaart_wdt_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.write		= moxaart_wdt_write,
	.unlocked_ioctl	= moxaart_wdt_ioctl,
	.open		= moxaart_wdt_open,
	.release	= moxaart_wdt_release,
};

static struct miscdevice moxaart_wdt_miscdev = {
	.minor		= WATCHDOG_MINOR,
	.name		= "watchdog",
	.fops		= &moxaart_wdt_fops,
};

static __devinit int moxaart_wdt_probe(struct platform_device *pdev)
{
	struct resource *r0 = NULL; 
	int error = 0;

	drv_dbg("probe\n");
	r0 = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r0) {
		drv_err("failed to get memory resource\n");
		error = -ENODEV;
		goto fail;
	}

	r0 = request_mem_region(r0->start, resource_size(r0), pdev->name);
	if (!r0) {
		drv_err("cannot request memory region\n");
		error = -EBUSY;
		goto fail;
	}

	wdt_regs = (wdt_regs_t *)ioremap(r0->start, resource_size(r0));
	drv_dbg("wdt_regs=%x\n", (unsigned int)wdt_regs);
	if (!wdt_regs) {
		drv_err("failed to map memory resource start=%x size=%x\n", 
				r0->start, resource_size(r0));
		error = -ENODEV;
		goto fail_ioremap;
	}

	spin_lock_init(&wdt_lock);

	error = misc_register(&moxaart_wdt_miscdev);
	if (error) {
		dev_err(&pdev->dev, "failed to register miscdev\n");
		goto fail_misc_register;
	}

	drv_info("Load Watchdog driver\n");
	return 0;

fail_misc_register:
	iounmap(wdt_regs);
fail_ioremap:
	release_mem_region(r0->start, resource_size(r0));
fail:
	return error;
}

static __devexit int moxaart_wdt_remove(struct platform_device *pdev)
{
	struct resource *res = NULL; 
	int error = 0; 

	drv_dbg("remove\n");

	error = misc_deregister(&moxaart_wdt_miscdev);
	if (error) {
		dev_err(&pdev->dev, "misc_deregister error\n");
		goto end;
	}

	iounmap(wdt_regs);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "Can't get memory resource\n");
		error = -ENODEV;
		goto end; 
	}

	release_mem_region(res->start, resource_size(res));

end: 
	return error;
}

static struct platform_driver moxaart_wdt_driver = {
	.probe		= moxaart_wdt_probe,
	.remove		= __devexit_p(moxaart_wdt_remove),
	.driver		= {
		.name	= "MoxaArtWatchdog",
		.owner	= THIS_MODULE,
	},
};

static int __init moxaart_wdt_init(void)
{
    drv_dbg("init");
	return platform_driver_register(&moxaart_wdt_driver);
}

static void __exit moxaart_wdt_exit(void)
{
    drv_dbg("exit");
	platform_driver_unregister(&moxaart_wdt_driver);
}


module_init(moxaart_wdt_init);
module_exit(moxaart_wdt_exit);

MODULE_AUTHOR("Wade Liang");
MODULE_DESCRIPTION("Moxaart Watchdog Driver");

module_param(timeout, int, 0);
MODULE_PARM_DESC(timeout, "Watchdog timeout in seconds (default 60s)");

module_param(nowayout, int, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started");

MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);

