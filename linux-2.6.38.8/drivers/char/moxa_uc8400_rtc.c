#define RTC_VERSION		"1.0"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/rtc.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>

#define GPIO_RTC_SCLK	4
#define GPIO_RTC_DATA	5

#define RTC_BASE_YEAR		1970
#define RTC_BASE_MONTH		1
#define RTC_BASE_DAY		1
#define RTC_BASE_HOUR		0
#define RTC_BASE_MINUTE		0
#define RTC_BASE_SECOND		0

#define RTC_DELAY_TIME		3

#define RTC_ADDR_SECOND  	0x00
#define RTC_ADDR_MINUTE  	0x01
#define RTC_ADDR_HOUR  		0x02
#define RTC_ADDR_DATE  		0x03
#define RTC_ADDR_MONTH  	0x04
#define RTC_ADDR_YEAR  		0x05
#define RTC_ADDR_WEEKDAY 	0x06
#define RTC_ADDR_SR 		0x07
#define RTC_ADDR_INT 		0x08
#define RTC_ADDR_RESERVED	0x09
#define RTC_ADDR_ATR 		0x0A
#define RTC_ADDR_DTR 		0x0B

#define RTC_ADDR_SCA 		0x0C
#define RTC_ADDR_MNA 		0x0D
#define RTC_ADDR_HRA 		0x0E
#define RTC_ADDR_DTA 		0x0F
#define RTC_ADDR_MOA 		0x10
#define RTC_ADDR_DWA 		0x11

#define RTC_ADDR_USR1 		0x12
#define RTC_ADDR_USR2 		0x13

#define RTC_IS_OPEN             0x01	/* means /dev/rtc is in use     */

static unsigned long rtc_status = 0;	/* bitmapped status byte.       */
static spinlock_t rtc_lock;
static unsigned long epoch = 2000;	/* year corresponding to 0x00	*/

static const unsigned char days_in_mo[] ={0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static int day_of_year[12]={0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

void IIC_START(void)
{
	gpio_line_config(GPIO_RTC_DATA, IXP4XX_GPIO_OUT);
	udelay(RTC_DELAY_TIME);

	gpio_line_set(GPIO_RTC_DATA, IXP4XX_GPIO_HIGH);
	udelay(RTC_DELAY_TIME);
	gpio_line_set(GPIO_RTC_SCLK, IXP4XX_GPIO_HIGH);
	udelay(RTC_DELAY_TIME);
	gpio_line_set(GPIO_RTC_DATA, IXP4XX_GPIO_LOW);
	udelay(RTC_DELAY_TIME);
	gpio_line_set(GPIO_RTC_SCLK, IXP4XX_GPIO_LOW);
}

void IIC_STOP(void)
{
	gpio_line_config(GPIO_RTC_DATA, IXP4XX_GPIO_OUT);
	udelay(RTC_DELAY_TIME);
	gpio_line_set(GPIO_RTC_DATA, IXP4XX_GPIO_LOW);
	
	udelay(RTC_DELAY_TIME);
	gpio_line_set(GPIO_RTC_SCLK, IXP4XX_GPIO_HIGH);
	udelay(RTC_DELAY_TIME);
	gpio_line_set(GPIO_RTC_DATA, IXP4XX_GPIO_HIGH);
	udelay(RTC_DELAY_TIME);
	gpio_line_set(GPIO_RTC_SCLK, IXP4XX_GPIO_LOW);
}

void IIC_Send_Byte(unsigned char ch)
{
	unsigned char count=8;
	gpio_line_config(GPIO_RTC_DATA, IXP4XX_GPIO_OUT);

	while ( count-- )
	{
		udelay(RTC_DELAY_TIME);
		if( (ch & 0x80) == 0x80 )
		{
			gpio_line_set(GPIO_RTC_DATA, IXP4XX_GPIO_HIGH);
			udelay(RTC_DELAY_TIME);
			gpio_line_set(GPIO_RTC_SCLK, IXP4XX_GPIO_HIGH);
			udelay(RTC_DELAY_TIME+1);
			gpio_line_set(GPIO_RTC_SCLK, IXP4XX_GPIO_LOW);
		}
		else
		{
			gpio_line_set(GPIO_RTC_DATA, IXP4XX_GPIO_LOW);
			udelay(RTC_DELAY_TIME);
			gpio_line_set(GPIO_RTC_SCLK, IXP4XX_GPIO_HIGH);
			udelay(RTC_DELAY_TIME+1);
			gpio_line_set(GPIO_RTC_SCLK, IXP4XX_GPIO_LOW);
		}
		ch = ch << 1;
	}
}

unsigned char IIC_Recv_Byte(void)
{
	unsigned char count=8, data=0;
	
	gpio_line_config(GPIO_RTC_DATA, IXP4XX_GPIO_IN);

	while (count--){
		data = data << 1;
		udelay(RTC_DELAY_TIME+5);
		gpio_line_set(GPIO_RTC_SCLK, IXP4XX_GPIO_HIGH);
		udelay(RTC_DELAY_TIME+5);

		if ( *IXP4XX_GPIO_GPINR & (1<<GPIO_RTC_DATA) ) {
			data = data | 0x01;
		}
		else {
			data = data & 0xFE;
		}
		gpio_line_set(GPIO_RTC_SCLK, IXP4XX_GPIO_LOW);
	}

	return data;
}

int IIC_CHECK_ACK(void)
{
	udelay(1);
	gpio_line_config(GPIO_RTC_DATA, IXP4XX_GPIO_IN);
	udelay(RTC_DELAY_TIME+1);

	gpio_line_set(GPIO_RTC_SCLK, IXP4XX_GPIO_HIGH);
	udelay(RTC_DELAY_TIME+1);

	if ( *IXP4XX_GPIO_GPINR & (1<<GPIO_RTC_DATA) ) {
		printk("%s[%d]:No ACK\n",__FUNCTION__,__LINE__);
	}
	gpio_line_set(GPIO_RTC_SCLK, IXP4XX_GPIO_LOW);
	
	return 0;
}

void RTCWriteRegister(unsigned char addr, unsigned char val)
{
	IIC_START();
	IIC_Send_Byte(0xde);
	IIC_CHECK_ACK();
	IIC_Send_Byte(addr);
	IIC_CHECK_ACK();
	IIC_Send_Byte(val);
	IIC_CHECK_ACK();
	IIC_STOP();
}

unsigned char RTCReadRegister(unsigned char addr)
{
	unsigned char val;
	/* start a dummy write */
	IIC_START();
	IIC_Send_Byte(0xde);	/* send device addr */
	IIC_CHECK_ACK();
	IIC_Send_Byte(addr);	/* send word addr */
	IIC_CHECK_ACK();
	IIC_START();
	IIC_Send_Byte(0xdf);
	IIC_CHECK_ACK();
	val = IIC_Recv_Byte();
	//IIC_SLAVE_NoACK();
	IIC_STOP();

	return val;
}

static void get_rtc_time(struct rtc_time *rtc_tm)
{
	unsigned char	v;
	
	spin_lock_irq(&rtc_lock);
	v = RTCReadRegister(RTC_ADDR_SECOND);
	rtc_tm->tm_sec = (((v & 0x70) >> 4) * 10) + (v & 0x0F);
	v = RTCReadRegister(RTC_ADDR_MINUTE);
	rtc_tm->tm_min = (((v & 0x70) >> 4) * 10) + (v & 0x0F);
	v = RTCReadRegister(RTC_ADDR_HOUR);
	if ( (v & 0x80) == 0 ) {// 12-hour mode
		rtc_tm->tm_hour = (((v & 0x10) >> 4) * 10) + (v & 0x0F);
		if ( v & 0x20 ) {	// PM mode
			rtc_tm->tm_hour += 12;
			if ( rtc_tm->tm_hour >= 24 )
				rtc_tm->tm_hour = 0;
		}
	} else {	// 24-hour mode
		rtc_tm->tm_hour = (((v & 0x30) >> 4) * 10) + (v & 0x0F);
	}
	v = RTCReadRegister(RTC_ADDR_DATE);
	rtc_tm->tm_mday = (((v & 0x30) >> 4) * 10) + (v & 0x0F);
	v = RTCReadRegister(RTC_ADDR_MONTH);
	rtc_tm->tm_mon = (((v & 0x10) >> 4) * 10) + (v & 0x0F);
	rtc_tm->tm_mon--;
	v = RTCReadRegister(RTC_ADDR_YEAR);
	rtc_tm->tm_year = (((v & 0xF0) >> 4) * 10) + (v & 0x0F);
	if ((rtc_tm->tm_year += (epoch - 1900)) <= 69)
		rtc_tm->tm_year += 100;
	v = RTCReadRegister(RTC_ADDR_WEEKDAY);

#if 0 // Comment out by Jared 08-26-2008 ISL1208I, day of the week is 0-6
	rtc_tm->tm_wday = (v & 0x0f) - 1;
#else
	rtc_tm->tm_wday = (v & 0x0f);
#endif
	rtc_tm->tm_yday = day_of_year[rtc_tm->tm_mon];
	rtc_tm->tm_yday += (rtc_tm->tm_mday-1);
	if ( rtc_tm->tm_mon >= 2 ) {
		if ( !(rtc_tm->tm_year % 4) && (rtc_tm->tm_year % 100) )
			rtc_tm->tm_yday++;
	}
	rtc_tm->tm_isdst = 0;
	spin_unlock_irq(&rtc_lock);
}

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

static int rtc_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
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
			
//		RTCWriteRegister(RTC_PROTECT_W, 0);
#if 0	// Comment out by Jared 08-26-2008, 12 hours format
		v = ((hrs / 10) << 4) | (hrs % 10);
		RTCWriteRegister(RTC_ADDR_HOUR, v);
#else   // 24 hours format
		v = 0x80 | ((hrs / 10) << 4) | (hrs % 10);
		RTCWriteRegister(RTC_ADDR_HOUR, v);
#endif
		v = ((min / 10) << 4) | (min % 10);
		RTCWriteRegister(RTC_ADDR_MINUTE, v);
		v = ((sec / 10) << 4) | (sec % 10);
		RTCWriteRegister(RTC_ADDR_SECOND, v);
		v = ((yrs / 10) << 4) | (yrs % 10);
		RTCWriteRegister(RTC_ADDR_YEAR, v);
		v = ((mon / 10) << 4) | (mon % 10);
		RTCWriteRegister(RTC_ADDR_MONTH, v);
		v = ((day / 10) << 4) | (day % 10);
		RTCWriteRegister(RTC_ADDR_DATE, v);
//		RTCWriteRegister(RTC_PROTECT_W, 0x80);
		
		spin_unlock_irq(&rtc_lock);
		return 0;
	}
	default:
		return -EINVAL;
	}
}

static int rtc_release(struct inode *inode, struct file *file)
{
	spin_lock_irq(&rtc_lock);
	rtc_status &= ~RTC_IS_OPEN;
	spin_unlock_irq(&rtc_lock);

	return 0;
}

static struct file_operations rtc_fops = {
	owner:THIS_MODULE,
	unlocked_ioctl:rtc_ioctl,
	open:rtc_open,
	release:rtc_release,
};

static struct miscdevice rtc_dev = {
	minor: RTC_MINOR,
	name: "rtc",
	fops: &rtc_fops,
};

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

static int __init rtc_init(void)
{
	unsigned char data=0;

	gpio_line_config(GPIO_RTC_SCLK, IXP4XX_GPIO_OUT);
	gpio_line_config(GPIO_RTC_DATA, IXP4XX_GPIO_OUT);

	gpio_line_set(GPIO_RTC_SCLK, IXP4XX_GPIO_LOW);
	gpio_line_set(GPIO_RTC_DATA, IXP4XX_GPIO_HIGH);
	mdelay(5);

	RTCWriteRegister(RTC_ADDR_SR, 0x10);
	mdelay(5);

	data = RTCReadRegister(RTC_ADDR_SR);
#if 0
	printk("%s[%d]:data:%x\n", __FUNCTION__, __LINE__, data);
	if( data == 0 ) {
		printk("The RTC has stoped. Now reenable it.\n");
		RTCWriteRegister(RTC_ADDR_SR, 0x80);
	}
#endif

	misc_register(&rtc_dev);

	create_proc_read_entry("driver/rtc", 0, 0, rtc_read_proc, NULL);

	printk(KERN_INFO "Register Moxa UC8400 RTC Driver v" RTC_VERSION "\n");
	return 0;
}

static void __exit rtc_exit(void)
{
	remove_proc_entry("driver/rtc", NULL);
	misc_deregister(&rtc_dev);
	printk(KERN_INFO "Unregister Moxa UC8400 RTC Driver v" RTC_VERSION "\n");
}

module_init(rtc_init);
module_exit(rtc_exit);
MODULE_AUTHOR("Jared Wu");
MODULE_LICENSE("GPL");
