/* XXX: may use /i2c/algos/i2c-algo-bit.c */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/rtc.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/types.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>
/* gpio access */
#include <asm/arch/cpe/cpe.h>
#include <asm/arch/gpio.h>

#define RTC_VERSION		"1.0"

/* device for IA240 */
#define GPIO_RTC_SCL	1<<28
#define GPIO_RTC_SDA	1<<9
/* the min is 1300ns for some timing */
#define RTC_DELAY_TIME		2

#define RTC_BASE_YEAR		1970
#define RTC_BASE_MONTH		1
#define RTC_BASE_DAY		1
#define RTC_BASE_HOUR		0
#define RTC_BASE_MINUTE		0
#define RTC_BASE_SECOND		0

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

#define BCD_TO_DEC(bcd, high_mask) ((((bcd & high_mask) >> 4) * 10) + (bcd & 0x0f))
#define DEC_TO_BCD(dec) (((dec / 10) << 4) | (dec % 10))

#define RTC_ID_WRITE	0xde
#define RTC_ID_READ		0xdf

//#define DEBUG
#ifdef DEBUG
#define debug(fmt, args...) printk("%s: "fmt, __FUNCTION__, ##args)
#else
#define debug(fmt, args...) 
#endif
#ifdef DEBUGV
#define debugv(fmt, args...) printk("%s: "fmt, __FUNCTION__, ##args)
#else 
#define debugv(fmt, args...) 
#endif

static unsigned long rtc_status = 0;	/* bitmapped status byte.       */
static spinlock_t rtc_lock;
static unsigned long epoch = 2000;	/* year corresponding to 0x00	*/
static const u8 days_in_mo[] ={0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static int day_of_year[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

static inline void delay(void)
{
	udelay(RTC_DELAY_TIME); 
}

static inline void i2c_set_sda_out(void) 
{
    mcpu_gpio_inout(GPIO_RTC_SDA, MCPU_GPIO_OUTPUT);
	debugv("cur=%x\n", mcpu_gpio_get_inout(GPIO_RTC_SDA));
}

static inline void i2c_set_sda_in(void) 
{
    mcpu_gpio_inout(GPIO_RTC_SDA, MCPU_GPIO_INPUT);
	debugv("cur=%x\n", mcpu_gpio_get_inout(GPIO_RTC_SDA));
}

static inline void i2c_set_scl_out(void) 
{
    mcpu_gpio_inout(GPIO_RTC_SCL, MCPU_GPIO_OUTPUT); 
	debugv("cur=%x\n", mcpu_gpio_get_inout(GPIO_RTC_SCL));
}

static inline int i2c_get_sda_val(void)
{
	u32 val; 
	
	/* if SDA=1, val = 1 << GPIO_RTC_SDA */
	val = mcpu_gpio_get(GPIO_RTC_SDA);
	debugv("val=%x\n", val);
	return (val == 0)? 0:1; 

}

static inline void i2c_set_sda_low(void) 
{
    mcpu_gpio_set(GPIO_RTC_SDA, MCPU_GPIO_LOW);
}

static inline void i2c_set_sda_high(void) 
{
    mcpu_gpio_set(GPIO_RTC_SDA, MCPU_GPIO_HIGH);
}

static inline void i2c_set_scl_high(void)
{
	mcpu_gpio_set(GPIO_RTC_SCL, MCPU_GPIO_HIGH); 
}

static inline void i2c_set_scl_low(void)
{
	mcpu_gpio_set(GPIO_RTC_SCL, MCPU_GPIO_LOW); 
}

void i2c_start(void) 
{
	i2c_set_sda_out();

	/* SDA form high to low while SCL=HIGH 
	 * Note: Don't scl(high)->sda(high) it may cause sda from low to high 
	 * instead, change sda to high in scl_low (data change state) 
	 */
	i2c_set_sda_high();
	delay();
	i2c_set_scl_high();
	delay();
	i2c_set_sda_low();
	delay();
	i2c_set_scl_low();
	delay();
	debug("scl=%x sda=%x\n", mcpu_gpio_get(GPIO_RTC_SCL), mcpu_gpio_get(GPIO_RTC_SDA)); 
}

void i2c_stop(void)
{
	i2c_set_sda_out();

	/* SDA form low to high while SCLK=HIGH 
	 * Note: Don't scl(high)->sda(low) it may cause sda from high to low 
	 */
	i2c_set_sda_low();
	delay();
	i2c_set_scl_high();
	delay();
	i2c_set_sda_high();
	delay();
	i2c_set_scl_low();
	debug("scl=%x sda=%x\n", mcpu_gpio_get(GPIO_RTC_SCL), mcpu_gpio_get(GPIO_RTC_SDA)); 
}

void i2c_send_byte(u8 ch)
{
	debug("ch=%x\n", ch); 
	i2c_set_sda_out();
	delay();

	/* sent Bit 7 ~ Bit 0 */
	int i; 
	for (i = 7; i >= 0; i--) {

		if ((ch & (1<<i)) == 1<<i) {
			i2c_set_sda_high();
		} else {
			i2c_set_sda_low();
		}
		delay();

		i2c_set_scl_high();
		delay();

		i2c_set_scl_low();
		delay();
	}
}

u8 i2c_recv_byte(void)
{
	u8 count = 8, data = 0;

	i2c_set_sda_in();
	delay();

	/* Get bit 7 ~ bit 0: SCL high: data valid, low: data change */
	while (count--) {

		/* shift above bits */
		data = data<<1;

		i2c_set_scl_high();
		delay();

		if (i2c_get_sda_val()) {
			data |= 0x01;
		}

		i2c_set_scl_low(); 
		delay();
	}

	debug("data=%x\n", data);
	return data;
}

int i2c_check_ack(void)
{
	i2c_set_sda_in(); 
	delay();

	i2c_set_scl_high();
	delay();

	if (i2c_get_sda_val() == 0) {
		debug("get  ACK\n");
	} else {
		debug("not get  ACK\n");
	}
	delay(); 

	i2c_set_scl_low();
	delay();
	return 0;
}

void rtc_write_reg(u8 addr, u8 val)
{
	debug("addr=%x val=%x\n", addr, val);
	i2c_start();
	i2c_send_byte(RTC_ID_WRITE);
	i2c_check_ack();
	i2c_send_byte(addr);
	i2c_check_ack();
	i2c_send_byte(val);
	i2c_check_ack();
	i2c_stop();
}

u8 rtc_read_reg(u8 addr)
{
	u8 val = 0;

	/* see FIg16: read sequence */
	i2c_start();
	i2c_send_byte(RTC_ID_WRITE);
	i2c_check_ack();
	i2c_send_byte(addr);
	i2c_check_ack();

	i2c_start();
	i2c_send_byte(RTC_ID_READ);
	i2c_check_ack();
	val = i2c_recv_byte();
	i2c_stop();
	debug("addr=%x readval=%x\n", addr, val);
	return val;
}

static void rtc_get_time(struct rtc_time *rtc_tm)
{
	u8	v;
	
	spin_lock_irq(&rtc_lock);

	v = rtc_read_reg(RTC_ADDR_SECOND);
	rtc_tm->tm_sec = BCD_TO_DEC(v, 0x70);

	v = rtc_read_reg(RTC_ADDR_MINUTE);
	rtc_tm->tm_min = BCD_TO_DEC(v, 0x70);

	v = rtc_read_reg(RTC_ADDR_HOUR);
	if ( (v & 0x80) == 0 ) {// 12-hour mode
		rtc_tm->tm_hour = BCD_TO_DEC(v, 0x10);
		if ( v & 0x20 ) {	// PM mode
			rtc_tm->tm_hour += 12;
			if ( rtc_tm->tm_hour >= 24 )
				rtc_tm->tm_hour = 0;
		}
	} else {// 24-hour mode
		rtc_tm->tm_hour = BCD_TO_DEC(v, 0x30);
	}

	v = rtc_read_reg(RTC_ADDR_DATE);
	rtc_tm->tm_mday = BCD_TO_DEC(v, 0x30);

	v = rtc_read_reg(RTC_ADDR_MONTH);
	rtc_tm->tm_mon = BCD_TO_DEC(v, 0x10);
	rtc_tm->tm_mon--;

	v = rtc_read_reg(RTC_ADDR_YEAR);
	rtc_tm->tm_year = BCD_TO_DEC(v, 0xF0);
	if ((rtc_tm->tm_year += (epoch - 1900)) <= 69)
		rtc_tm->tm_year += 100;

	v = rtc_read_reg(RTC_ADDR_WEEKDAY);
#if 0 // Comment out by Jared 08-26-2008 ISL1208I, day of the week is 0-6
	rtc_tm->tm_wday = (v & 0x0f) - 1;
#endif 
	rtc_tm->tm_wday = (v & 0x0f);
	rtc_tm->tm_yday = day_of_year[rtc_tm->tm_mon];
	rtc_tm->tm_yday += (rtc_tm->tm_mday-1);

	if (rtc_tm->tm_mon >= 2) {
		if (!(rtc_tm->tm_year % 4) && (rtc_tm->tm_year % 100))
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

static int rtc_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	struct rtc_time rtc_tm;
	u8	v;

	switch (cmd) {

	case RTC_RD_TIME:	
		/* Read the time/date from RTC  */
		rtc_get_time(&rtc_tm);
		return copy_to_user((void *) arg, &rtc_tm, sizeof(rtc_tm)); 
		break; 

	case RTC_SET_TIME:	
		/* Set the RTC */
		{ 
		u8 mon, day, hrs, min, sec, leap_yr;
		unsigned int yrs;
		
		if (!capable(CAP_SYS_TIME))
			return -EACCES;

		if (copy_from_user(&rtc_tm, (struct rtc_time *) arg, 
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
			
#if 0	// Comment out by Jared 08-26-2008, 12 hours format
		v = DEC_TO_BCD(hrs);
		rtc_write_reg(RTC_ADDR_HOUR, v);
#endif   // 24 hours format

		v = 0x80 | DEC_TO_BCD(hrs);
		rtc_write_reg(RTC_ADDR_HOUR, v);

		v = DEC_TO_BCD(min);
		rtc_write_reg(RTC_ADDR_MINUTE, v);

		v = DEC_TO_BCD(sec);
		rtc_write_reg(RTC_ADDR_SECOND, v);

		v = DEC_TO_BCD(yrs);
		rtc_write_reg(RTC_ADDR_YEAR, v);

		v = DEC_TO_BCD(mon);
		rtc_write_reg(RTC_ADDR_MONTH, v);

		v = DEC_TO_BCD(day);
		rtc_write_reg(RTC_ADDR_DATE, v);
		
		spin_unlock_irq(&rtc_lock);
		debug("rtc_ioctl:set time correct\n"); 
		return 0;
		break; 
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
	ioctl:rtc_ioctl,
	open:rtc_open,
	release:rtc_release,
};

static struct miscdevice rtc_dev = {
	minor: RTC_MINOR,
	name: "moxa_ia240_rtc",
	fops: &rtc_fops,
};

static int rtc_proc_output(char *buf)
{
	char *p;
	struct rtc_time tm;

	rtc_get_time(&tm);
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
	u8 sr; 

	/* set pmu I/O multiplexing control register */
	mcpu_gpio_mp_set(GPIO_RTC_SCL|GPIO_RTC_SDA);

	i2c_set_sda_out();
	i2c_set_scl_out();

	/* initilize status in Fig12 */
	i2c_set_scl_low();
	i2c_set_sda_high();

	/* write rtc enable bit */
	rtc_write_reg(RTC_ADDR_SR, 0x10);
	sr = rtc_read_reg(RTC_ADDR_SR); 

	misc_register(&rtc_dev);
	create_proc_read_entry("driver/rtc", 0, 0, rtc_read_proc, NULL);
	printk(KERN_INFO "Register Moxa RTC Driver" RTC_VERSION "\n");

	return 0;
}

static void __exit rtc_exit(void)
{
	remove_proc_entry("driver/rtc", NULL);
	misc_deregister(&rtc_dev);
	printk(KERN_INFO "Unregister Moxa RTC Driver v" RTC_VERSION "\n");
}

module_init(rtc_init);
module_exit(rtc_exit);
MODULE_AUTHOR("Jared Wu");
MODULE_LICENSE("GPL");
