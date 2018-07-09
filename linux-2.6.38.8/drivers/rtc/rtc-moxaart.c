/*
 *	A simple generic Real Time Clock interface for Linux.
 *
 * 	History:
 * 	Date		Author			Comment
 * 	12-27-2005	Victor Yu.		Create it.
 * 	11-03-2011  Wade.Liang      Modify and use gpiolib
 */

#define RTC_VERSION		"1.0"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/rtc.h>
#include <asm/io.h>
#include <linux/bcd.h>
#include <linux/spinlock.h>
#include <mach/gpio.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>

/* TODO: 
 * IA240/241 use reset/sclk/data 8/28/9
 * W311 use 26/24/25
 * OTHER use 7/5/6
 */
#define GPIO_RTC_RESET		CONFIG_MOXAART_RTC_GPIO_RESET
#define GPIO_RTC_SCLK		CONFIG_MOXAART_RTC_GPIO_SCLK
#define GPIO_RTC_DATA		CONFIG_MOXAART_RTC_GPIO_DATA


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

//#define DEBUG
#ifdef DEBUG
#define drv_dbg(fmt, args...) printk(KERN_DEBUG "%s: "fmt, __FUNCTION__, ##args)
#else 
#define drv_dbg(fmt, args...)
#endif

#define drv_err(fmt, args...) printk(KERN_ERR "MoxaArt RTC: "fmt, ##args)
#define drv_info(fmt, args...) printk(KERN_INFO "MoxaArt RTC: "fmt, ##args)

static spinlock_t rtc_lock = SPIN_LOCK_UNLOCKED;
static spinlock_t rtc_hw_lock = SPIN_LOCK_UNLOCKED;
static unsigned long epoch = 2000;	/* year corresponding to 0x00 */

/* default set all RTC GPIO to output*/
static struct gpio rtc_gpios[] = {
    { GPIO_RTC_RESET, GPIOF_DIR_OUT, "rtc_reset"}, 
    { GPIO_RTC_SCLK, GPIOF_DIR_OUT, "rtc_sclk"}, 
    { GPIO_RTC_DATA, GPIOF_DIR_OUT, "rtc_data"}, 
}; 

static const unsigned char days_in_mo[] = 
{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static u8 rtc_read(u8 cmd)
{
    u8			data;
    int 		i, v;
    unsigned long	flags;
    
    spin_lock_irqsave(&rtc_hw_lock, flags); 

    gpio_direction_output(GPIO_RTC_DATA, MCPU_GPIO_LOW); 
    gpio_set_value(GPIO_RTC_RESET, MCPU_GPIO_HIGH); 

	/* write command byte */
    for ( i=0; i<8; i++, cmd>>=1 ){
    	gpio_set_value(GPIO_RTC_SCLK, MCPU_GPIO_LOW);
    	if ( cmd & 1 )
    		gpio_set_value(GPIO_RTC_DATA, MCPU_GPIO_HIGH);
    	else
    		gpio_set_value(GPIO_RTC_DATA, MCPU_GPIO_LOW);
    	gpio_set_value(GPIO_RTC_SCLK, MCPU_GPIO_HIGH);
    }

    /* read data byte */
    gpio_direction_input(GPIO_RTC_DATA);

    for (i = 0, data =0; i < 8; i++) {
        gpio_set_value(GPIO_RTC_SCLK, MCPU_GPIO_LOW);
        gpio_set_value(GPIO_RTC_SCLK, MCPU_GPIO_HIGH);
        v = gpio_get_value(GPIO_RTC_DATA);
        if (v) {
                data |= (1<<i);
        }
    } 

    gpio_set_value(GPIO_RTC_SCLK, MCPU_GPIO_LOW);
    gpio_set_value(GPIO_RTC_RESET, MCPU_GPIO_LOW);

    spin_unlock_irqrestore(&rtc_hw_lock, flags); 

    return data;
}

static void rtc_write(u8 cmd, u8 Data)
{
    int 		i;
    unsigned long	flags;

    spin_lock_irqsave(&rtc_hw_lock, flags); 

    gpio_direction_output(GPIO_RTC_DATA, MCPU_GPIO_LOW);
    gpio_set_value(GPIO_RTC_RESET, MCPU_GPIO_HIGH); 

    /* write command byte */
    for ( i=0; i<8; i++,cmd>>=1 ) {
    	gpio_set_value(GPIO_RTC_SCLK, MCPU_GPIO_LOW);
    	if ( cmd & 1 )
    		gpio_set_value(GPIO_RTC_DATA, MCPU_GPIO_HIGH);
    	else
    		gpio_set_value(GPIO_RTC_DATA, MCPU_GPIO_LOW);
        gpio_set_value(GPIO_RTC_SCLK, MCPU_GPIO_HIGH);
    }
    
    /* write data byte */
    gpio_direction_output(GPIO_RTC_DATA, MCPU_GPIO_LOW);
    for ( i=0; i<8; i++,Data>>=1 ){
    	gpio_set_value(GPIO_RTC_SCLK, MCPU_GPIO_LOW);
    	if ( Data & 1 )
    		gpio_set_value(GPIO_RTC_DATA, MCPU_GPIO_HIGH);
    	else
    		gpio_set_value(GPIO_RTC_DATA, MCPU_GPIO_LOW);
    	gpio_set_value(GPIO_RTC_SCLK, MCPU_GPIO_HIGH);
    }
    gpio_set_value(GPIO_RTC_SCLK, MCPU_GPIO_LOW);
    gpio_set_value(GPIO_RTC_RESET, MCPU_GPIO_LOW);
    gpio_direction_input(GPIO_RTC_DATA);

    spin_unlock_irqrestore(&rtc_hw_lock, flags); 
}

static int moxaart_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	unsigned char	v;
    static int	day_of_year[12]={0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    
    drv_dbg("enter");	
	spin_lock_irq(&rtc_lock);
	v = rtc_read(RTC_SECONDS_R);
	tm->tm_sec = bcd2bin(v);

	v = rtc_read(RTC_MINUTES_R);
	tm->tm_min = bcd2bin(v);

	v = rtc_read(RTC_HOURS_R);
	if ( v & 0x80 ) {	
        /* 12-hour mode */
		tm->tm_hour = bcd2bin(v);
		if ( v & 0x20 ) {	
            /* PM mode */
			tm->tm_hour += 12;
			if ( tm->tm_hour >= 24 )
				tm->tm_hour = 0;
		}
	} else {	
        /* 24-hour mode */
		tm->tm_hour = bcd2bin(v);
	}

	v = rtc_read(RTC_DATE_R);
	tm->tm_mday = bcd2bin(v); 

	v = rtc_read(RTC_MONTH_R);
	tm->tm_mon = bcd2bin(v);
	tm->tm_mon--;

	v = rtc_read(RTC_YEAR_R);
	tm->tm_year = bcd2bin(v);
	if ((tm->tm_year += (epoch - 1900)) <= 69)
		tm->tm_year += 100;

	v = rtc_read(RTC_DAY_R);
	tm->tm_wday = (v & 0x0f) - 1;
	tm->tm_yday = day_of_year[tm->tm_mon];
	tm->tm_yday += (tm->tm_mday-1);

	if ( tm->tm_mon >= 2 ) {
		if ( !(tm->tm_year % 4) && (tm->tm_year % 100) )
			tm->tm_yday++;
	}

	tm->tm_isdst = 0;
	spin_unlock_irq(&rtc_lock);

    drv_dbg("tm: sec=%d min=%d hour=%d mday=%d mon=%d year=%d\n", 
            tm->tm_sec, tm->tm_min, tm->tm_hour, tm->tm_mday, tm->tm_mon, tm->tm_year);	
    return rtc_valid_tm(tm);
}

static int moxaart_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
    unsigned char mon, day, hrs, min, sec, leap_yr;
    unsigned int yrs;
    u8 v;
    
    drv_dbg("tm: sec=%d min=%d hour=%d mday=%d mon=%d year=%d\n", 
            tm->tm_sec, tm->tm_min, tm->tm_hour, tm->tm_mday, tm->tm_mon, tm->tm_year);	
    yrs = tm->tm_year + 1900;
    mon = tm->tm_mon + 1;   /* tm_mon starts at zero */
    day = tm->tm_mday;
    hrs = tm->tm_hour;
    min = tm->tm_min;
    sec = tm->tm_sec;

    if (yrs < 1970) {
        drv_info("error year=%d\n", yrs);
        return -EINVAL;
    }

    leap_yr = ((!(yrs % 4) && (yrs % 100)) || !(yrs % 400));

    if ((mon > 12) || (day == 0)) {
        drv_info("error mon=%d day=%d\n", mon, day);
        return -EINVAL;
    }

    if (day > (days_in_mo[mon] + ((mon == 2) && leap_yr))) {
        drv_info("error day info\n");
        return -EINVAL;
    }
        
    if ((hrs >= 24) || (min >= 60) || (sec >= 60)) {
        drv_info("errot hrs=%d min=%d sec=%d\n", hrs, min, sec);
        return -EINVAL;
    }

    if ((yrs -= epoch) > 255) {   /* They are unsigned */
        drv_info("year=%u. It should be 2000 ~ 2054\n", yrs);
        return -EINVAL;
    }

    spin_lock_irq(&rtc_lock);

    /* These limits and adjustments are independant of
     * whether the chip is in binary mode or not. 
     */
    if (yrs > 169) {
        drv_info("yrs=%u > 169", yrs);
        spin_unlock_irq(&rtc_lock);
        return -EINVAL;
    }

    if (yrs >= 100)
        yrs -= 100;
        
    rtc_write(RTC_PROTECT_W, 0);

    v = bin2bcd(hrs);
    rtc_write(RTC_HOURS_W, v);

    v = bin2bcd(min); 
    rtc_write(RTC_MINUTES_W, v);

    v = bin2bcd(sec);
    rtc_write(RTC_SECONDS_W, v);

    v = bin2bcd(yrs);
    rtc_write(RTC_YEAR_W, v);

    v = bin2bcd(mon);
    rtc_write(RTC_MONTH_W, v);

    v = bin2bcd(day);
    rtc_write(RTC_DATE_W, v);

    rtc_write(RTC_PROTECT_W, 0x80);
    
    spin_unlock_irq(&rtc_lock);

    return 0;
}

static const struct rtc_class_ops moxaart_rtc_ops = {
	.read_time = moxaart_rtc_read_time,
	.set_time = moxaart_rtc_set_time,
};

static int __init moxaart_rtc_probe(struct platform_device *pdev)
{
	struct rtc_device *rtc;
	struct rtc_time tm;
    int err = 0; 

	/* set I/O Multiplexer to GPIO mode */
	moxaart_gpio_mp_set(GPIO_RTC_RESET|GPIO_RTC_SCLK|GPIO_RTC_DATA);

    err = gpio_request_array(rtc_gpios, ARRAY_SIZE(rtc_gpios)); 
    if (err) {
        drv_info("Can't request gpio! ret=%d\n", err); 
    } else {
        drv_info("Request GPIO OK\n"); 
    }

	moxaart_rtc_read_time(NULL, &tm);
	if ( tm.tm_sec == 0 && tm.tm_min == 0 && tm.tm_min == 0 && tm.tm_hour == 0 &&
	     tm.tm_year == 100 && tm.tm_mon == 0 && tm.tm_mday == 1 ) {
		drv_info("The RTC has stoped. Now reenable it.\n");
		rtc_write(RTC_PROTECT_W,0);     /* Disable Write Protect */
		rtc_write(RTC_SECONDS_W,0);     /* Enable OSC */
		rtc_write(RTC_PROTECT_W,0x80);  /* Enable Write Protect */
	}

	rtc = rtc_device_register("rtc-moxaart", &pdev->dev, &moxaart_rtc_ops,
				  THIS_MODULE);
	if (IS_ERR(rtc)) {
        drv_info("Can't register RTC driver\n"); 
		return PTR_ERR(rtc);
    }

    drv_info("Load driver\n");

	platform_set_drvdata(pdev, rtc);
	return 0;
}

static int __exit moxaart_rtc_remove(struct platform_device *pdev)
{
	struct rtc_device *rtc = platform_get_drvdata(pdev);

	rtc_device_unregister(rtc);
    gpio_free_array(rtc_gpios, ARRAY_SIZE(rtc_gpios)); 
	return 0;
}

static struct platform_driver moxaart_rtc_driver = {
	.driver = {
		.name = "MoxaArtRTC", 
		.owner = THIS_MODULE,
	},
	.remove = __exit_p(moxaart_rtc_remove),
};

static int __init moxaart_rtc_init(void)
{
    drv_dbg("init\n");
	return platform_driver_probe(&moxaart_rtc_driver, moxaart_rtc_probe);
}

static void __exit moxaart_rtc_exit(void)
{
    platform_driver_unregister(&moxaart_rtc_driver);
}

module_init(moxaart_rtc_init);
module_exit(moxaart_rtc_exit);
MODULE_AUTHOR("WadeLiang");
MODULE_LICENSE("GPL");
