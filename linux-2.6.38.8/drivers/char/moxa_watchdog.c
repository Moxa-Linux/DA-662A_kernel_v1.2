/*
 * This is Moxa UC7000 watch dog driver for CV case.
 *
 * History:
 * Date		Aurhor			Comment
 * 09-15-2004	Victor Yu.		Create it. I name it sWatchDog
 * 06-10-2005	Jared Wu.		Fix the watch dog devic driver bug. kernel_thread() is a system call cannot be invoke in timer timeout interrupt. I move it to the moxa_swtd_init()
 * 06-18-2009   Wade Liang		Change moxaart series to interrupt mode
 */
#define __KERNEL_SYSCALLS__
#include <linux/proc_fs.h> /* Necessary because we use the proc fs */
#include <linux/version.h>
#include <linux/unistd.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/mm.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/irq.h>
#include <asm/bitops.h>
#include <linux/reboot.h>
#include <asm/types.h>
#include <linux/notifier.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
#include <linux/workqueue.h>
#endif
#if defined (DA68X)
#include "x86_moxa_swtd.h"
#elif defined (CONFIG_CPU_IXP43X) || defined (CONFIG_ARCH_DA66X_UC7400) || defined (ARCH_IXDP422)
#include "ixp43x_moxa_swtd.h"
#elif defined(CONFIG_ARCH_MOXAART)
#include "moxaart_moxa_swtd.h"
#elif defined(CONFIG_ARCH_EP93XX)
#include "ep93xx_moxa_swtd.h"
#endif
#ifndef WATCHDOG_NOWAYOUT
#define WATCHDOG_NOWAYOUT 0
#endif

static struct proc_dir_entry *swtd_proc_file;
static int			opencounts=0;
static int			swtduserenabled=0;
static unsigned long		swtdtime=DEFAULT_WATCHDOG_TIME;
static struct timer_list	timer_swtd;
static int			bswtd_timeout = 0;
static int			nowayout = WATCHDOG_NOWAYOUT;
static int			debug = 0;
static char			expect_close;
static spinlock_t 		swtd_lock=SPIN_LOCK_UNLOCKED;
#if defined(CONFIG_ARCH_MOXAART)
static int irq_res=-1;
static unsigned long flags=0;
#endif
#if defined(CONFIG_ARCH_EP93XX)
static int			pollcnt=0;
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
static wait_queue_head_t swtd_queue; 
#endif

// add by Jared Wu. 03-10-2009 Ack the watchdog instead of disable it.
// this means that if the kernel crashed, the hardware watchdog will
// reset in 15 sec.
static void swtd_ack(unsigned long swtd_ack_time)
{
	if (debug)
		printk("<1>swtd_ack: swtd_time=%lu\n",swtd_ack_time);
#ifndef __WDT_TEST__
#if defined (DA68X)
	superio_enter_config();
	superio_set_logic_device(8);	//logic device 8
	superio_set_reg((swtd_ack_time/1000), 0xF6);	//Reg:F6,30 sec
#elif defined (CONFIG_CPU_IXP43X) || defined (CONFIG_ARCH_DA66X_UC7400)
	*IXP4XX_OSWK = IXP4XX_WDT_KEY;
	*IXP4XX_OSWE = 0x0;
	*IXP4XX_OSWT = WATCHDOG_COUNTER(swtd_ack_time);
	*IXP4XX_OSWE = IXP4XX_WDT_COUNT_ENABLE | IXP4XX_WDT_RESET_ENABLE;
	*IXP4XX_OSWK = 0;
#elif defined (ARCH_IXDP422)
	*IXP425_OSWK = 0x482e;
	*IXP425_OSWE = 0x0;
	*IXP425_OSWT = WATCHDOG_COUNTER(swtd_ack_time);
	*IXP425_OSWE = IXP4XX_WDOG_CNT_ENA | IXP4XX_WDOG_RST_ENA;
	*IXP425_OSWK = 0;
#elif defined(CONFIG_ARCH_MOXAART)
	*(unsigned int *)(MOXAART_WATCHDOG_VA_BASE+4) = WATCHDOG_COUNTER(swtd_ack_time);
	*(unsigned int *)(MOXAART_WATCHDOG_VA_BASE+8) = 0x5ab9;
	/* Change to interrupt mode 0x05, reset mode is 0x03*/
	*(unsigned int *)(MOXAART_WATCHDOG_VA_BASE+12) = 0x05;
#elif defined(CONFIG_ARCH_EP93XX)
	// Not used
#endif
#endif
}

static void swtd_enable(void)
{
	if (debug)
		printk("swtd_enable: swtdtime=%lu\n",swtdtime);
#ifndef __WDT_TEST__
#if defined (DA68X)
	superio_enter_config();
	superio_set_logic_device(8);	//logic device 8
	superio_set_reg(1, 0x30);	//Reg:30 active WDT
	superio_set_reg(0, 0xF5);	//Reg:F5
	superio_set_reg(0, 0xF7);	//Reg:F7
	superio_set_reg((swtdtime+WATCHDOG_DEFER_TIME)/1000, 0xF6);	//Reg:F6,30 sec
#elif defined (CONFIG_CPU_IXP43X) || defined (CONFIG_ARCH_DA66X_UC7400)
	*IXP4XX_OSWK = IXP4XX_WDT_KEY;
	*IXP4XX_OSWE = 0x0;
	*IXP4XX_OSWT = WATCHDOG_COUNTER(swtdtime+WATCHDOG_DEFER_TIME);
	*IXP4XX_OSWE = IXP4XX_WDT_COUNT_ENABLE | IXP4XX_WDT_RESET_ENABLE;
	*IXP4XX_OSWK = 0;
#elif defined (ARCH_IXDP422)
	*IXP425_OSWK = 0x482e;
	*IXP425_OSWE = 0x0;
	*IXP425_OSWT = WATCHDOG_COUNTER(swtdtime+WATCHDOG_DEFER_TIME);
	//*IXP425_OSWE = IXP4XX_WDOG_CNT_ENA | IXP4XX_WDOG_INT_ENA | IXP4XX_WDOG_RST_ENA ;
	*IXP425_OSWE = IXP4XX_WDOG_CNT_ENA | IXP4XX_WDOG_RST_ENA ;
	*IXP425_OSWK = 0;
#elif defined(CONFIG_ARCH_MOXAART)
	*(unsigned int *)(MOXAART_WATCHDOG_VA_BASE+4) = WATCHDOG_COUNTER(swtdtime+WATCHDOG_DEFER_TIME);
	*(unsigned int *)(MOXAART_WATCHDOG_VA_BASE+8) = 0x5ab9;
	*(unsigned int *)(MOXAART_WATCHDOG_VA_BASE+12) = 0x03;
#elif defined(CONFIG_ARCH_EP93XX)
	__raw_writew(0x5555, EP93XX_WATCHDOG_BASE);	// reset watchdog counter
	__raw_writew(0xaaaa, EP93XX_WATCHDOG_BASE);	// enable watchdog
#endif
#endif
}

static void swtd_disable(void)
{
	if (debug)
		printk("swtd_disable\n");
#ifndef __WDT_TEST__
#if defined (DA68X)
	superio_enter_config();
	superio_set_logic_device(8);	//logic device 8
	superio_set_reg(0, 0xF6);	//Reg:F6 counter register
#elif defined (CONFIG_CPU_IXP43X) || defined (CONFIG_ARCH_DA66X_UC7400)
	*IXP4XX_OSWK = IXP4XX_WDT_KEY;
	*IXP4XX_OSWE = 0;
	*IXP4XX_OSWK = 0;
#elif defined (ARCH_IXDP422)
	*IXP425_OSWK = 0x482e;
	*IXP425_OSWE = 0;
	*IXP425_OSWK = 0;
#elif defined(CONFIG_ARCH_MOXAART)
	*(unsigned int *)(MOXAART_WATCHDOG_VA_BASE+12) = 0;
#elif defined(CONFIG_ARCH_EP93XX)
	__raw_writew(0xaa55, EP93XX_WATCHDOG_BASE);
#endif
#endif
}

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
static void swtd_reboot(void *unused)
{
	char    *argv[2], *envp[5];

        if ( in_interrupt() )
                return;
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,25)
	if ( !current->fs->root )
                return;	
#else
	//if ( !current->fs->root.dentry ) // Jared, not fix
#endif
        argv[0] = "/sbin/reboot";
        argv[1] = 0;
        envp[0] = "HOME=/";
        envp[1] = "PATH=/sbin:/bin:/usr/sbin:/usr/bin";
        envp[2] = 0;
        call_usermodehelper(argv[0], argv, envp, 0);
}
#else
static char	*argv_init[2]={"reboot", NULL};
static char	*envp_init[3]={"HOME=/", "TERM=linux", NULL};

static int swtd_reboot(void *unused)
{
	interruptible_sleep_on(&swtd_queue);
	printk("<1>exec reboot.\n");
	execve("/sbin/reboot", argv_init, envp_init);
	return 0;
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
DECLARE_WORK(rebootqueue, swtd_reboot, NULL);
#else
DECLARE_WORK(rebootqueue, swtd_reboot);
#endif

static void swtd_poll(unsigned long ignore)
{
	spin_lock(&swtd_lock);
#if defined(CONFIG_ARCH_EP93XX)
	if ( pollcnt++ <= (swtdtime/WATCHDOG_ACK_JIFFIES) ) {
		timer_swtd.expires = jiffies + WATCHDOG_ACK_JIFFIES;
		add_timer(&timer_swtd);
		swtd_enable();
		spin_unlock(&swtd_lock);
		return ;
	}
	swtd_enable();
	if (debug)
		printk("<1>swtd_poll: Now reboot the system.\n");
	#ifndef __WDT_TEST__
	schedule_work(&rebootqueue);
	#endif
	bswtd_timeout = 1;
#else  // (CONFIG_CPU_IXP43X) || defined(CONFIG_ARCH_EP93XX) || defined(CONFIG_ARCH_MOXAART)

	if ( swtduserenabled ) {
		swtd_ack(WATCHDOG_DEFER_TIME);
		if (debug)
			printk("<1>swtd_poll: Now reboot the system.\n");
	#ifndef __WDT_TEST__
		#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
		schedule_work(&rebootqueue);
		#else
		/* Jared said, awake any swtd_reboot kernel thread */
		wake_up_interruptible(&swtd_queue);
		#endif
	#endif
		bswtd_timeout = 1;
	}
       	else
	{
		if (debug)
			printk("swtd_poll: ack the hardware watchdog timer\n");
	/* 1-17-2005 Jared said, if disable the watchdog, 
	 * it would not reboot while the system is very busy 
	 */
		timer_swtd.expires = jiffies +WATCHDOG_ACK_JIFFIES(swtdtime);
		add_timer(&timer_swtd);
		swtd_ack(swtdtime+WATCHDOG_DEFER_TIME);
	}
#endif
	spin_unlock(&swtd_lock);
}

ssize_t moxaswtd_proc_read(char *buffer,char **buffer_location,off_t offset, int buffer_length, int *eof, void *data)
{
    int len=0; /* The number of bytes actually used */
	    
    if (offset > 0) {
      len =0;
    }
    else {
  	//Fill the buffer and get its length
      len += sprintf(buffer+len,
	     "user enable\t: %d\n"
	     "ack time\t: %d msec\n"
#if defined (DA68X)
	     "hardware watchdog counter\t: %d sec\n"
#elif defined (CONFIG_CPU_IXP43X)
	     "hardware watchdog counter\t: %d msec\n"
#endif
             ,swtduserenabled, (int)swtdtime 
#if defined (DA68X)
	     ,superio_get_reg(0xF6)
#elif defined (CONFIG_CPU_IXP43X)
	     ,(unsigned int)(*IXP4XX_OSWT)/66666
#endif
	     );
    }
    return len;
}

static int swtd_open(struct inode *inode, struct file *file)
{
	if ( MINOR(inode->i_rdev) != MOXA_WATCHDOG_MINOR )
		return -ENODEV;
	
	spin_lock_irq(&swtd_lock);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
	if (nowayout)
		__module_get(THIS_MODULE);
#endif
	bswtd_timeout = 0; // reset the timeout flag
	opencounts++;
	spin_unlock_irq(&swtd_lock);

	return 0;
}

// Kernel ack the watchdog timer and reset the state machine
static void swtd_release_timer(void) {
	swtdtime = DEFAULT_WATCHDOG_TIME;
#if defined(CONFIG_ARCH_EP93XX)
	swtd_disable();
	del_timer(&timer_swtd);
#else
	mod_timer(&timer_swtd, jiffies + WATCHDOG_ACK_JIFFIES(swtdtime));
	swtd_ack(swtdtime+WATCHDOG_DEFER_TIME);
#endif
	swtduserenabled = 0;
	bswtd_timeout = 0; // reset the timeout flag

}

static int swtd_release(struct inode *inode, struct file *file)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
	sigset_t *sigset = &current->signal->shared_pending.signal;
#else
	sigset_t *sigset = &current->pending.signal;
#endif

	spin_lock_irq(&swtd_lock);

	if (debug)
		printk("<1>swtd_release entry\n");
	opencounts--;

	if ( opencounts <= 0 ) {
		/*
		 *      Shut off the timer.
		 */
		if (expect_close == 42) {
			printk("<1>swtd_release: expect close\n");
			if ( !bswtd_timeout  ) {
				swtd_release_timer();
			}
		}
		else if ( signal_pending(current) ) {
			if (debug)
				printk("<1>swtd_release[%d] has signal pending\n",__LINE__);
			if ( sigismember (sigset, SIGKILL) || \
			    sigismember (sigset, SIGINT) || \
			    sigismember (sigset, SIGTERM) ) {
				if (debug)
					printk("<1>swtd_release[%d] get SIGKILL/SIGINT/SIGTERM signal\n",__LINE__);
				if ( !bswtd_timeout  ) {
					swtd_release_timer();
				}
			}
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
		} else if ( current->signal->group_exit_code == SIGQUIT || \
			current->signal->group_exit_code == SIGILL || \
			current->signal->group_exit_code == SIGABRT || \
			current->signal->group_exit_code == SIGFPE || \
			current->signal->group_exit_code == SIGSEGV ) {
			if (debug)
				printk("<1>swtd_release[%d] got coredump\n",__LINE__);
		}
#else
		} else if ( sigismember (sigset, SIGILL) || \
			    sigismember (sigset, SIGABRT) || \
			    sigismember (sigset, SIGFPE) || \
			    sigismember (sigset, SIGSEGV) ) {
			    if (debug)
				printk("<1>swtd_release[%d] got coredump\n",__LINE__);
			}
#endif
		else {	// normal close the file handle
			if (debug)
				printk("<1>swtd_release_l1[%d] kernel ack the watchdog timer\n",__LINE__);
			if ( !bswtd_timeout  ) {
#if 1 // For Taitung
				swtd_release_timer();
#endif
			}
		}
		expect_close = 0;
	}
	spin_unlock_irq(&swtd_lock);

	return 0;
}

#if 1 //Jared, porting for 2.6.38
static long swtd_ioctl (struct file *file, unsigned int ioc_cmd, unsigned long arg)
#else
static int swtd_ioctl (struct inode *inode, struct file *file, unsigned int ioc_cmd, unsigned long arg)
#endif
{
	unsigned long		time;
	struct swtd_set_struct	nowset;

	switch ( ioc_cmd ) {
	case IOCTL_WATCHDOG_ENABLE :
		if ( copy_from_user(&time, (unsigned long *)arg, sizeof(unsigned long)) )
			return -EFAULT;
		if ( time < WATCHDOG_MIN_TIME || time > WATCHDOG_MAX_TIME )
			return -EINVAL;
		spin_lock_irq(&swtd_lock);
		if ( !bswtd_timeout ) {
			// Switch to user mode watchdog.
			// When the kernel timer timeout, the system will reboot
			swtduserenabled = 1;
			swtdtime = time;
#if defined(CONFIG_ARCH_EP93XX)
			pollcnt = 0;
			swtd_enable();
			if(timer_pending(&timer_swtd))
				mod_timer(&timer_swtd, jiffies + WATCHDOG_ACK_JIFFIES);
			else
				add_timer(&timer_swtd);
#else
			mod_timer(&timer_swtd, jiffies + WATCHDOG_ACK_JIFFIES(swtdtime));
			swtd_ack(swtdtime+WATCHDOG_DEFER_TIME);
#endif
		}
		spin_unlock_irq(&swtd_lock);
		break;
	case IOCTL_WATCHDOG_DISABLE :
		spin_lock_irq(&swtd_lock);
		if ( swtduserenabled && !bswtd_timeout ) {
#if defined(CONFIG_ARCH_EP93XX)
			pollcnt=0;
			swtd_disable();
			del_timer(&timer_swtd);
#else
			// Switch to kernel mode watchdog.
			// The kernel timer will acknowledge the HW watchdog
			swtd_enable();
			mod_timer(&timer_swtd, jiffies + WATCHDOG_ACK_JIFFIES(swtdtime));
#endif
			swtdtime = DEFAULT_WATCHDOG_TIME;
			bswtd_timeout = 0; // reset the timeout flag
			swtduserenabled = 0;
		}
		spin_unlock_irq(&swtd_lock);
		break;
	case IOCTL_WATCHDOG_GET_SETTING :
		nowset.mode = swtduserenabled;
		nowset.time = swtdtime;
		if ( copy_to_user((void *)arg, &nowset, sizeof(nowset)) )
			return -EFAULT;
		break;
	case IOCTL_WATCHDOG_ACK :
		spin_lock_irq(&swtd_lock);
		if ( swtduserenabled && !bswtd_timeout ) {
#if defined(CONFIG_ARCH_EP93XX)
			pollcnt=0;
			swtd_enable();
			mod_timer(&timer_swtd, jiffies + WATCHDOG_ACK_JIFFIES);
#else
			// Switch to user mode watchdog.
			// When the kernel timer timeout, the system will reboot
			mod_timer(&timer_swtd, jiffies + WATCHDOG_ACK_JIFFIES(swtdtime));
			swtd_ack(swtdtime+WATCHDOG_DEFER_TIME);
#endif
		}
		spin_unlock_irq(&swtd_lock);

		break;
	default:
		return -EINVAL;
	}
	return 0;
}

/*
 *      swtd_write:
 *      @file: file handle to the watchdog
 *      @buf: buffer to write (unused as data does not matter here
 *      @count: count of bytes
 *      @ppos: pointer to the position to write. No seeks allowed
 *
 *      A write to a watchdog device is defined as a keepalive signal. Any
 *      write of data will do, as we we don't define content meaning.
 */

static ssize_t swtd_write(struct file *file, const char *buf, \
			    size_t count, loff_t *ppos)
{
	if (count)
	{
		if (!nowayout)
		{
			size_t i;

			/* In case it was set long ago */
#if 0 // Comment out by Jared 2009-05-11, don't reset the expect_close flag
			expect_close = 0;
#endif
			for (i = 0; i != count; i++)
			{
				char c;
				if (get_user(c, buf + i))
					return -EFAULT;
				if (c == 'V')
					expect_close = 42;
			}
		}

		/* someone wrote to us, we should restart timer */
		spin_lock_irq(&swtd_lock);
		if ( !bswtd_timeout ) {
			// Switch to user mode watchdog.
			// When the kernel timer timeout, the system will reboot
			swtduserenabled = 1;
#if defined(CONFIG_ARCH_EP93XX)
			pollcnt=0;
			mod_timer(&timer_swtd, jiffies + WATCHDOG_ACK_JIFFIES);
			swtd_enable();
#else
			mod_timer(&timer_swtd, jiffies + WATCHDOG_ACK_JIFFIES(swtdtime));
			swtd_ack(swtdtime+WATCHDOG_DEFER_TIME);
#endif
		}
		spin_unlock_irq(&swtd_lock);

	}
	return count;
}
#if defined(CONFIG_ARCH_MOXAART)
/* IRQ handler: for hw watchdog and system reboot will call */
static irqreturn_t swtd_interrupt_handler(int irq, void *dev_id)
{
	static int i = 3000;
	static unsigned char sr;
	static spinlock_t 		lock=SPIN_LOCK_UNLOCKED;
	static unsigned long flags;

	if (debug)
		printk("swtd_interrupt_handler: enter\n");

	spin_lock_irqsave(&lock, flags);

	/* 
	 * Added by wade ,09/10/10  
	 * turn flash to suspend mode, and to read mode
	 * to avoid watchdog reboot with flash in write mode
	 * refer to intel Program suspend flow chat
	 */

	/* get status register, if in erase suspend or programing suspend, add a D0 ehre to resume first */
	sr =  *(unsigned char*)(MOXAART_FLASH_VA_BASE+0x55*2);

	if (sr & 0x04) {
		*( volatile u16 *)(MOXAART_FLASH_VA_BASE+0x55*2) = 0xd0;
		mdelay(10);
	} else if (sr & 0x40) {
		*( volatile u16 *)(MOXAART_FLASH_VA_BASE+0x55*2) = 0xd0;
		mdelay(10);
	}

	/* program suspend  */	
	*( volatile u16 *)(MOXAART_FLASH_VA_BASE+0x55*2) = 0xb0;
	/* read read status register to check if entering suspend mode */
	do {
		sr =  *(unsigned char*)(MOXAART_FLASH_VA_BASE+0x55*2);
		if (sr & 0x80) {
			break;
		} 
		mdelay(10);
		i--;
	} while (i > 0);

	/* change to  read array mode */
	*( volatile u16 *)(MOXAART_FLASH_VA_BASE+0x55*2) = 0xff;
	/* add delay here to make sure flash is changed to read array mode */
	mdelay(100);

	// reset the CPU
	printk("Starting to reboot ... \n");
	*(unsigned int *)(MOXAART_WATCHDOG_VA_BASE+4) = 1;
	// to start the watch dog
	*(unsigned int *)(MOXAART_WATCHDOG_VA_BASE+8) = 0x5ab9;
	// set to reset the CPU
	*(unsigned int *)(MOXAART_WATCHDOG_VA_BASE+12) = 0x03;

	while(1);


	return IRQ_HANDLED;
	
}

/* Panic hander */
static int swtd_panic_handler(struct notifier_block *this,
                              unsigned long         event,
                              void                  *unused)
{	
	/* Avoid the software interrupt of swtd_ack */
	spin_lock_bh(&swtd_lock);

	if (debug) printk("swtd_panic_handler: enter\n");

	/* Reset flash state */
	if (debug) printk("swtd_panic_handler: reset flash state\n");
	*( volatile u16 *)(MOXAART_FLASH_VA_BASE+0x55*2) = 0xb0;//Suspend block erase
	*( volatile u16 *)(MOXAART_FLASH_VA_BASE+0x55*2) = 0xff;//Change to read array mode

	/* Call hardware reboot */
	if (debug) printk("swtd_panic_handler: call hardware rebooot\n");
	*(unsigned int *)(MOXAART_WATCHDOG_VA_BASE+4) = 1;
	*(unsigned int *)(MOXAART_WATCHDOG_VA_BASE+8) = 0x5ab9;
	*(unsigned int *)(MOXAART_WATCHDOG_VA_BASE+12) = 0x03;


        return NOTIFY_OK;

	spin_unlock_bh(&swtd_lock);
}

/* Structure for notification */
static struct notifier_block swtd_panic_notifier = {
        swtd_panic_handler,
        NULL,
        150   /* priority: INT_MAX >= x >= 0 */
};

#endif

static struct file_operations moxa_swtd_fops = {
	.owner=THIS_MODULE,
	.open=swtd_open,
	.write=swtd_write,
#if 1 //Jared, porting for 2.6.38
	.unlocked_ioctl=swtd_ioctl,
#else
	.ioctl=swtd_ioctl,
#endif
	.release=swtd_release,
};

static struct miscdevice wdt_miscdev = {
	.minor = MOXA_WATCHDOG_MINOR,
	.name = "swtd",
	.fops = &moxa_swtd_fops,
};

static int __init moxaswtd_init(void) {
	// register misc
	if ( misc_register(&wdt_miscdev)!=0 ) {
		printk("Moxa DA-681/662-LX WatchDog: Register misc fail !\n");
		goto moxa_swtd_init_err1;
	}
#if defined(CONFIG_ARCH_MOXAART)
	/* Register IRQ handler */
	irq_res= request_irq(IRQ_WATCHDOG, swtd_interrupt_handler, IRQF_DISABLED,"swtd_irq", NULL);
	if (irq_res) {
		printk( KERN_EMERG "swtd: can't get assigned irq %i\n", IRQ_WATCHDOG);
		return -1;
	}
	
	/* Resister panic handler */
	atomic_notifier_chain_register(&panic_notifier_list, &swtd_panic_notifier);
#endif

	init_timer(&timer_swtd);
	timer_swtd.function=swtd_poll;
#if defined(CONFIG_ARCH_EP93XX)
	timer_swtd.expires = jiffies + WATCHDOG_ACK_JIFFIES;
#else
	timer_swtd.expires = jiffies + WATCHDOG_ACK_JIFFIES(swtdtime);
	add_timer(&timer_swtd);
	swtd_enable();
#endif

	swtd_proc_file = create_proc_read_entry("driver/swtd", 0644, NULL, moxaswtd_proc_read, NULL);
	if ( !swtd_proc_file ) {
        	printk("<1>moxaswtd_init:create_proc_read_entry() fail\n");
		goto moxa_swtd_init_err2;
	}

#if defined (DA68X)
	struct resource *base_res;

	base_res = request_region(SUPERIO_CONFIG_PORT, 2, "swtd");
        if (!base_res) {
		printk("<1>moxaswtd_init: can't get I/O address 0x%x\n", SUPERIO_CONFIG_PORT);
		goto moxa_swtd_init_err3;
        }
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
	init_waitqueue_head  (&swtd_queue);
	kernel_thread(swtd_reboot, NULL, CLONE_FS | CLONE_FILES | CLONE_SIGNAL);
#endif

	printk (KERN_INFO "initialized. (nowayout=%d)\n", nowayout);
	printk (KERN_INFO "initialized. (debug=%d)\n", debug);

	return 0;

#if defined (DA68X)
moxa_swtd_init_err3:
	remove_proc_entry("driver/swtd", NULL);
#endif
moxa_swtd_init_err2:
	if(timer_pending(&timer_swtd))
		del_timer(&timer_swtd);
	misc_deregister(&wdt_miscdev);
moxa_swtd_init_err1:
	return -ENOMEM;
}

static void __exit moxaswtd_exit(void) {

#if defined (DA68X)
	release_region(SUPERIO_CONFIG_PORT, 2);
	superio_exit_config();
#endif
	remove_proc_entry("driver/swtd", NULL);
	swtd_disable();
	if(timer_pending(&timer_swtd))
		del_timer(&timer_swtd);
	if ( swtduserenabled ) {
		swtduserenabled = 0;
		opencounts = 0;
	}
	misc_deregister(&wdt_miscdev);

#if defined(CONFIG_ARCH_MOXAART)
	/* Free IRQ */
	free_irq(IRQ_WATCHDOG, NULL);

	/* Free panic hander */
	atomic_notifier_chain_unregister(&panic_notifier_list, &swtd_panic_notifier);
#endif

}

module_init(moxaswtd_init);
module_exit(moxaswtd_exit);

MODULE_AUTHOR("Jared_Wu@moxa.com.tw");
MODULE_LICENSE("GPL");
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
module_param(nowayout, int, 0);
#else
MODULE_PARM(nowayout, "i");
#endif
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started (default=CONFIG_WATCHDOG_NOWAYOUT)");
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
module_param(debug, int, 0);
#else
MODULE_PARM(debug, "i");
#endif
MODULE_PARM_DESC(debug, "print the debug message in this driver");
