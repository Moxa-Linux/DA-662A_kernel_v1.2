/*
 *	A simple generic Real Time Clock interface for Linux.
 *
 * 	History:
 * 	Date		Author			Comment
 * 	12-27-2005	Victor Yu.		Create it.
 */

#define RTC_VERSION		"1.0"

#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/rtc.h>
#include <linux/interrupt.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/arch/cpe/cpe.h>
#include <asm/arch/gpio.h>

/* FIXME: IA240/241 use reset/sclk/data 8/28/9
 * W311 use 26/24/25
 * OTHER use 7/5/6
 */
#define GPIO_RTC_RESET		(1<<8)
#define GPIO_RTC_SCLK		(1<<28)
#define GPIO_RTC_DATA		(1<<9)

#define RTC_PROTECT_W   	0x8E
#define RTC_PROTECT_R   	0x8F
#define RTC_YEAR_W      	0x8C
#define RTC_YEAR_R      	0x8D
#define RTC_DAY_W       	0x8A
#define RTC_DAY_R       	0x8B
#define RTC_MONTH_W     	0x88
#define RTC_MONTH_R     	0x89
#define RTC_DATE_W      	0x86
#define RTC_DATE_R      	0x87
#define RTC_HOURS_W     	0x84
#define RTC_HOURS_R     	0x85
#define RTC_MINUTES_W 		0x82
#define RTC_MINUTES_R 		0x83
#define RTC_SECONDS_W 		0x80
#define RTC_SECONDS_R 		0x81

#define RTC_DELAY_TIME  	8       // 8 usecond

static unsigned long rtc_status = 0;	/* bitmapped status byte.       */

static int rtc_read_proc(char *page, char **start, off_t off,
			 int count, int *eof, void *data);


#define RTC_IS_OPEN             0x01	/* means /dev/rtc is in use     */

static spinlock_t rtc_lock;
static unsigned long epoch = 2000;	/* year corresponding to 0x00	*/

static const unsigned char days_in_mo[] = 
{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static u8 rtc_read_reg(u8 Cmd)
{
    u8			data;
    int 		i, v;
    unsigned long	flags;
    
    save_flags(flags);
    cli();
    mcpu_gpio_inout(GPIO_RTC_DATA, MCPU_GPIO_OUTPUT);
    mcpu_gpio_set(GPIO_RTC_RESET, MCPU_GPIO_HIGH);

	/* write command byte */
    for ( i=0; i<8; i++, Cmd>>=1 ){
    	mcpu_gpio_set(GPIO_RTC_SCLK, MCPU_GPIO_LOW);
    	if ( Cmd & 1 )
    		mcpu_gpio_set(GPIO_RTC_DATA, MCPU_GPIO_HIGH);
    	else
    		mcpu_gpio_set(GPIO_RTC_DATA, MCPU_GPIO_LOW);
    	mcpu_gpio_set(GPIO_RTC_SCLK, MCPU_GPIO_HIGH);
    }
    
    mcpu_gpio_inout(GPIO_RTC_DATA, MCPU_GPIO_INPUT);

    /* read data byte */
    	mcpu_gpio_set(GPIO_RTC_SCLK, MCPU_GPIO_LOW);
	mcpu_gpio_set(GPIO_RTC_SCLK, MCPU_GPIO_HIGH);
	v = mcpu_gpio_get(GPIO_RTC_DATA);
    	if ( v )
    		data |= (1<<i);
    }
    mcpu_gpio_set(GPIO_RTC_SCLK, MCPU_GPIO_LOW);
    mcpu_gpio_set(GPIO_RTC_RESET, MCPU_GPIO_LOW);
    restore_flags(flags);

    return data;
}

static void rtc_write_reg(u8 Cmd, u8 Data)
{
    int 		i;
    unsigned long	flags;

    save_flags(flags);
    cli();
    mcpu_gpio_inout(GPIO_RTC_DATA, MCPU_GPIO_OUTPUT);
    mcpu_gpio_set(GPIO_RTC_RESET, MCPU_GPIO_HIGH);
    //udelay(RTC_DELAY_TIME);
    /* write command byte */
    for ( i=0; i<8; i++,Cmd>>=1 ) {
    	mcpu_gpio_set(GPIO_RTC_SCLK, MCPU_GPIO_LOW);
    	if ( Cmd & 1 )
    		mcpu_gpio_set(GPIO_RTC_DATA, MCPU_GPIO_HIGH);
    	else
    		mcpu_gpio_set(GPIO_RTC_DATA, MCPU_GPIO_LOW);
        mcpu_gpio_set(GPIO_RTC_SCLK, MCPU_GPIO_HIGH);
    }
    
    /* write data byte */
    mcpu_gpio_inout(GPIO_RTC_DATA, MCPU_GPIO_OUTPUT);
    for ( i=0; i<8; i++,Data>>=1 ){
    	mcpu_gpio_set(GPIO_RTC_SCLK, MCPU_GPIO_LOW);
    	if ( Data & 1 )
    		mcpu_gpio_set(GPIO_RTC_DATA, MCPU_GPIO_HIGH);
    	else
    		mcpu_gpio_set(GPIO_RTC_DATA, MCPU_GPIO_LOW);
    	mcpu_gpio_set(GPIO_RTC_SCLK, MCPU_GPIO_HIGH);
    }
    mcpu_gpio_set(GPIO_RTC_SCLK, MCPU_GPIO_LOW);
    mcpu_gpio_set(GPIO_RTC_RESET, MCPU_GPIO_LOW);
    mcpu_gpio_inout(GPIO_RTC_DATA, MCPU_GPIO_INPUT);
    restore_flags(flags);
}

#if 1	// add by Victor Yu. 01-10-2005
static int	day_of_year[12]={0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
#endif
static void get_rtc_time(struct rtc_time *rtc_tm)
{
	unsigned char	v;
	
	spin_lock_irq(&rtc_lock);
	v = rtc_read_reg(RTC_SECONDS_R);
	rtc_tm->tm_sec = (((v & 0x70) >> 4) * 10) + (v & 0x0F);
	v = rtc_read_reg(RTC_MINUTES_R);
	rtc_tm->tm_min = (((v & 0x70) >> 4) * 10) + (v & 0x0F);
	v = rtc_read_reg(RTC_HOURS_R);
	if ( v & 0x80 ) {	// 12-hour mode
		rtc_tm->tm_hour = (((v & 0x10) >> 4) * 10) + (v & 0x0F);
		if ( v & 0x20 ) {	// PM mode
			rtc_tm->tm_hour += 12;
			if ( rtc_tm->tm_hour >= 24 )
				rtc_tm->tm_hour = 0;
		}
	} else {	// 24-hour mode
		rtc_tm->tm_hour = (((v & 0x30) >> 4) * 10) + (v & 0x0F);
	}
	v = rtc_read_reg(RTC_DATE_R);
	rtc_tm->tm_mday = (((v & 0x30) >> 4) * 10) + (v & 0x0F);
	v = rtc_read_reg(RTC_MONTH_R);
	rtc_tm->tm_mon = (((v & 0x10) >> 4) * 10) + (v & 0x0F);
	rtc_tm->tm_mon--;
	v = rtc_read_reg(RTC_YEAR_R);
	rtc_tm->tm_year = (((v & 0xF0) >> 4) * 10) + (v & 0x0F);
	if ((rtc_tm->tm_year += (epoch - 1900)) <= 69)
		rtc_tm->tm_year += 100;
#if 1	// add by Victor Yu. 01-10-2005
	v = rtc_read_reg(RTC_DAY_R);
	rtc_tm->tm_wday = (v & 0x0f) - 1;
	rtc_tm->tm_yday = day_of_year[rtc_tm->tm_mon];
	rtc_tm->tm_yday += (rtc_tm->tm_mday-1);
	if ( rtc_tm->tm_mon >= 2 ) {
		if ( !(rtc_tm->tm_year % 4) && (rtc_tm->tm_year % 100) )
			rtc_tm->tm_yday++;
	}
	rtc_tm->tm_isdst = 0;
#endif
	spin_unlock_irq(&rtc_lock);
}

static int
rtc_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
	  unsigned long arg)
{
	struct rtc_time rtc_tm;
	unsigned char	v;

	switch (cmd) {
	case RTC_RD_TIME:	/* Read the time/date from RTC  */
		get_rtc_time(&rtc_tm);
		return copy_to_user((void *) arg, &rtc_tm, sizeof(rtc_tm)) ? 
			-EFAULT : 0;
	case RTC_SET_TIME:	/* Set the RTC */
	{
		unsigned char mon, day, hrs, min, sec, leap_yr;
		unsigned int yrs;
		
		if (!capable(CAP_SYS_TIME))
			return -EACCES;

		if (copy_from_user(&rtc_tm, 
				   (struct rtc_time *) arg,
		                   sizeof(struct rtc_time))) 
			return -EFAULT;
			
		yrs = rtc_tm.tm_year + 1900;
		mon = rtc_tm.tm_mon + 1;   /* tm_mon starts at zero */
		day = rtc_tm.tm_mday;
		hrs = rtc_tm.tm_hour;
		min = rtc_tm.tm_min;
		sec = rtc_tm.tm_sec;

		if (yrs < 1970)
			return -EINVAL;

		leap_yr = ((!(yrs % 4) && (yrs % 100)) || !(yrs % 400));

		if ((mon > 12) || (day == 0))
			return -EINVAL;

		if (day > (days_in_mo[mon] + ((mon == 2) && leap_yr)))
			return -EINVAL;
			
		if ((hrs >= 24) || (min >= 60) || (sec >= 60))
			return -EINVAL;

		if ((yrs -= epoch) > 255)    /* They are unsigned */
			return -EINVAL;
		spin_lock_irq(&rtc_lock);
		/* These limits and adjustments are independant of
		 * whether the chip is in binary mode or not.
		 */
		if (yrs > 169) {
			spin_unlock_irq(&rtc_lock);
			return -EINVAL;
		}
		if (yrs >= 100)
			yrs -= 100;
			
		rtc_write_reg(RTC_PROTECT_W, 0);
		v = ((hrs / 10) << 4) | (hrs % 10);
		rtc_write_reg(RTC_HOURS_W, v);
		v = ((min / 10) << 4) | (min % 10);
		rtc_write_reg(RTC_MINUTES_W, v);
		v = ((sec / 10) << 4) | (sec % 10);
		rtc_write_reg(RTC_SECONDS_W, v);
		v = ((yrs / 10) << 4) | (yrs % 10);
		rtc_write_reg(RTC_YEAR_W, v);
		v = ((mon / 10) << 4) | (mon % 10);
		rtc_write_reg(RTC_MONTH_W, v);
		v = ((day / 10) << 4) | (day % 10);
		rtc_write_reg(RTC_DATE_W, v);
		rtc_write_reg(RTC_PROTECT_W, 0x80);
		
		spin_unlock_irq(&rtc_lock);
		return 0;
	}
	default:
		return -EINVAL;
	}
}

/* We use rtc_lock to protect against concurrent opens. So the BKL is not
 * needed here. Or anywhere else in this driver. */
static int rtc_open(struct inode *inode, struct file *file)
{
	spin_lock_irq(&rtc_lock);

	if (rtc_status & RTC_IS_OPEN) {
		spin_unlock_irq(&rtc_lock);
		return -EBUSY;
	}

	rtc_status |= RTC_IS_OPEN;

	spin_unlock_irq(&rtc_lock);
	return 0;
}

static int rtc_release(struct inode *inode, struct file *file)
{
	spin_lock_irq(&rtc_lock);
	rtc_status &= ~RTC_IS_OPEN;
	spin_unlock_irq(&rtc_lock);
	return 0;
}

/*
 *	The various file operations we support.
 */

static struct file_operations rtc_fops = {
	owner:THIS_MODULE,
	llseek:no_llseek,
	ioctl:rtc_ioctl,
	open:rtc_open,
	release:rtc_release,
};

static struct miscdevice rtc_dev = {
	RTC_MINOR,
	"rtc",
	&rtc_fops
};

static int __init rtc_init(void)
{
#if 1	// add by Victor Yu. 04-21-2005, to avoid the RTS stop
	struct rtc_time rtc_tm;

	// set the CPU for GPIO
	mcpu_gpio_mp_set(GPIO_RTC_RESET|GPIO_RTC_SCLK|GPIO_RTC_DATA);


	// default set all RTC GPIO to output on the Moxa CPU
	mcpu_gpio_inout(GPIO_RTC_RESET|GPIO_RTC_SCLK|GPIO_RTC_DATA, MCPU_GPIO_OUTPUT);

	get_rtc_time(&rtc_tm);
//printk("YYYY-MON-DAY-HH-MM-SS=%d-%d-%d-%d-%d-%d\n", rtc_tm.tm_year, rtc_tm.tm_mon, rtc_tm.tm_mday, rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);
	if ( rtc_tm.tm_sec == 0 && rtc_tm.tm_min == 0 && rtc_tm.tm_min == 0 && rtc_tm.tm_hour == 0 &&
	     rtc_tm.tm_year == 100 && rtc_tm.tm_mon == 0 && rtc_tm.tm_mday == 1 ) {
		printk("The RTC has stoped. Now reenable it.\n");
		rtc_write_reg(RTC_PROTECT_W,0);/* Disable Write Protect */
		rtc_write_reg(RTC_SECONDS_W,0);/* Enable OSC */
		rtc_write_reg(RTC_PROTECT_W,0x80);/* Enable Write Protect */
	}
#endif
	misc_register(&rtc_dev);
	create_proc_read_entry("driver/rtc", 0, 0, rtc_read_proc, NULL);

	printk(KERN_INFO "Generic Moxa RC7000 RTC Driver v" RTC_VERSION "\n");

	return 0;
}

static void __exit rtc_exit(void)
{
	remove_proc_entry("driver/rtc", NULL);
	misc_deregister(&rtc_dev);

}

module_init(rtc_init);
module_exit(rtc_exit);

/*
 *	Info exported via "/proc/driver/rtc".
 */

static int rtc_proc_output(char *buf)
{
	char *p;
	struct rtc_time tm;

	get_rtc_time(&tm);

	p = buf;

	/*
	 * There is no way to tell if the luser has the RTC set for local
	 * time or for Universal Standard Time (GMT). Probably local though.
	 */
	p += sprintf(p,
		     "rtc_time\t: %02d:%02d:%02d\n"
		     "rtc_date\t: %04d-%02d-%02d\n"
	 	     "rtc_epoch\t: %04lu\n",
		     tm.tm_hour, tm.tm_min, tm.tm_sec,
		     tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, epoch);

	return p - buf;
}

static int rtc_read_proc(char *page, char **start, off_t off,
			 int count, int *eof, void *data)
{
	int len = rtc_proc_output(page);
	if (len <= off + count)
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len > count)
		len = count;
	if (len < 0)
		len = 0;
	return len;
}

MODULE_AUTHOR("Victor Yu");
MODULE_LICENSE("GPL");
