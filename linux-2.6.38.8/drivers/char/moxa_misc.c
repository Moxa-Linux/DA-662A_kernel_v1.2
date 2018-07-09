/*
 * This is Moxa linux embedded platform series device driver.
 * This device driver will control the READY LED & RESET button.
 * Also this device driver will control debug LED.
 * We add to support LCM/Keypad/DI/DO.
 * It is from misc interface. So the device node major number is 10.
 * The device node minor number is following:
 * lcm  :       102
 * keypad:      103
 * dio:         104	// DI & DO
 * mxmisc:	105	// ready led, reset button & debug LED
 *
 * The UC-8000 device mapping memory:
 * lcm		: 0x53000010      write only, for UC8000 platform
 *		: 0x53000020      write only, for UC8000 platform
 *		: 0x53000030      read only, for UC8000 platform
 * keypad	: 0x53000040      read only, for UC8000 platform
 * di		: 0x53000050      read only, for UC8000 platform
 * do		: 0x53000010      write only, for UC8000 platform
 * do		: 0x53000020      write only, for UC8000 platform
 * ready-led	: 0x53000020      write only, for UC8000 platform
 * reset-button	: 0x53000040      read only, for UC8000 platform
 * IGT signal	: 0x53000060      read only, bit 7, for UC8480 platform
 * debug-led	: 0x53000000      write only, for UC8000 platform
 *
 * The UC-8400 device mapping memory:
 * ready-led	: GPIO 12      write only, for UC8400 platform
 * reset-button	: GPIO 13      write only, for UC8400 platform
 * di		: 0x53000010      write only, for UC8400 platform
 * do		: 0x53000020      write only, for UC8400 platform
 * debug-led	: 0x53000070      write only, for UC8400 platform
 *
 * History:
 * Date		Aurhor			Comment
 * 02-22-2006	Victor Yu.		Create it.
 * 06-04-2009	Victor Yu. 		Add to support LCM/Keypad/DI/DO.
 */
#include <linux/version.h>
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
#include <linux/timer.h>
#include <linux/ioport.h>
#include <linux/kmod.h>
#include <linux/errno.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <linux/gpio.h>
//#include <mach/gpio.h>

#define MISC_VERSION		"1.2"	// Driver version

#if defined(CONFIG_MACH_MOXA_IA262) || defined(CONFIG_MACH_MOXA_EM2260) || defined(CONFIG_MACH_MOXA_IA260)
#define CONFIG_MACH_MOXA_IA261
#elif defined(CONFIG_ARCH_IA241_32128) || defined(CONFIG_ARCH_EM1280)// add by Victor Yu. 05-22-2007
#define CONFIG_ARCH_IA241
#endif

//#define DEBUG
#ifdef DEBUG
#define dbg_printk(x,args...)	printk("%s,%s,%d: " x,__FILE__, __FUNCTION__, __LINE__, ##args);
#else
#define dbg_printk(x,args...)
#endif

#if defined(CONFIG_ARCH_IA240) || defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345) || defined(CONFIG_ARCH_UC_7112_LX_PLUS) || defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W325) || defined(CONFIG_ARCH_W315)
#include <mach/hardware.h>

#elif defined(CONFIG_MACH_MOXA_IA261)||defined(CONFIG_MACH_MOXA_W406)   // add by Victor Yu. 12-13-2007
#ifdef CONFIG_MACH_MOXA_W406
#include <mach/moxa-w406.h>
#else
#include <mach/moxa-ia261.h>
#endif

#define MCPU_GPIO_OUTPUT	GPIO_OUT
#define MCPU_GPIO_INPUT		GPIO_IN
#define MCPU_GPIO_HIGH		EP93XX_GPIO_HIGH
#define MCPU_GPIO_LOW		EP93XX_GPIO_LOW

//#define moxaart_gpio_inout(x,y)	gpio_line_config(x,y)
//#define moxaart_gpio_set(x,y)	gpio_line_set(x,y)
//#define moxaart_gpio_get(x)	gpio_line_get(x)

#endif

#define MOXA_LCM_MINOR          102
#define MOXA_KEYPAD_MINOR       103
#define MOXA_DIO_MINOR          104
#define MOXA_MISC_MINOR		105


#if defined(CONFIG_ARCH_IA240) || defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345) || defined(CONFIG_ARCH_UC_7112_LX_PLUS) || defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W325) || defined(CONFIG_ARCH_W315)
#define SW_READY_GPIO		(1<<27)
#define SW_RESET_GPIO		(1<<25)
#elif defined(CONFIG_MACH_MOXA_W406)   // add by Victor Yu. 12-13-2007
#define SW_READY_GPIO           EP93XX_GPIO_LINE_GRLED
#define SW_RESET_GPIO           EP93XX_GPIO_LINE_EGPIO2
#elif defined(CONFIG_MACH_MOXA_IA261)	// add by Victor Yu. 12-13-2007
#define SW_READY_GPIO		EP93XX_GPIO_LINE_EGPIO11
#define SW_RESET_GPIO		EP93XX_GPIO_LINE_EGPIO6
#elif defined (CONFIG_MACH_MOXA_UC8000) // add by Victor Yu. 04-27-2009
#define SW_READY_ADDR		0x23
#define SW_RESET_ADDR		0x43
#define SW_READY_GPIO		(1<<3)
#define SW_RESET_GPIO		(1<<5)
static volatile unsigned char	*sw_ready_addr=NULL;
static volatile unsigned char	*sw_reset_addr=NULL;
static unsigned char		sw_ready_value=SW_READY_GPIO;
#elif defined (CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
#define SW_READY_GPIO		12
#define SW_RESET_GPIO		13
#else
#define SW_READY_GPIO		(1<<4)
#define SW_RESET_GPIO		(1<<23)
#endif

#if defined(CONFIG_ARCH_IA240) || defined(CONFIG_ARCH_IA241)// mask by Victor Yu. 04-20-2007
#define DEBUG_LED_0		(1<<0)
#define DEBUG_LED_1		(1<<1)
#define DEBUG_LED_2		(1<<2)
#define DEBUG_LED_3		(1<<3)
#define DEBUG_LED		(DEBUG_LED_0|DEBUG_LED_1|DEBUG_LED_2|DEBUG_LED_3)
#elif defined(CONFIG_MACH_MOXA_IA261)	// add by Victor Yu. 03-03-2008
#define DEBUG_LED_0		EP93XX_GPIO_LINE_EGPIO10
#define DEBUG_LED_1		EP93XX_GPIO_LINE_EGPIO3
#define DEBUG_LED_2		EP93XX_GPIO_LINE_EGPIO2
#define DEBUG_LED_3		EP93XX_GPIO_LINE_EGPIO1
#define DEBUG_LED_4		EP93XX_GPIO_LINE_EGPIO0
#define DEBUG_LED_5		EP93XX_GPIO_LINE_GRLED

#define IOCTL_USER_LED1_ON	0x50
#define IOCTL_USER_LED1_OFF	0x51
#define IOCTL_USER_LED2_ON	0x52
#define IOCTL_USER_LED2_OFF	0x53
#define USER_LED1		EP93XX_GPIO_LINE_EGPIO7
#define USER_LED2		EP93XX_GPIO_LINE_EGPIO9
#elif defined (CONFIG_MACH_MOXA_UC8000) || defined (CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
#define MISC_BASE_ADDR		0x53000000
#define MISC_BASE_REGION_SIZE	0x100
	#if defined(CONFIG_MACH_MOXA_UC8000)
#define DEBUG_LED_ADDR		0x03
	#elif defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
#define IGT_ADDR		0x53000060
#define DEBUG_LED_ADDR		0x53000070
		#if !defined(CONFIG_MACH_MOXA_UC8406_NGC)
#define KEYPAD_ADDR		0x43
#define LCM_CMD0_ADDR		0x13
#define LCM_CMD1_ADDR		0x23
#define LCM_DATA_ADDR		0x33
static volatile unsigned char   *lcm_cmd0_addr;
static volatile unsigned char   *lcm_cmd1_addr;
static volatile unsigned char   *lcm_data_addr;
static volatile unsigned char   *keypad_addr;
static volatile unsigned char	*igt_addr;
static struct resource		*misc_igt_base_res=NULL;
static spinlock_t               keypad_lock= SPIN_LOCK_UNLOCKED;
		#endif	// CONFIG_MACH_MOXA_UC8406_NGC
	#else	// CONFIG_MACH_MOXA_UC8000
#define DEBUG_LED_ADDR		0x73
	#endif	// CONFIG_MACH_MOXA_UC8000
#define DEBUG_LED_0		(1<<0)
#define DEBUG_LED_1		(1<<1)
#define DEBUG_LED_2		(1<<2)
#define DEBUG_LED_3		(1<<3)
#define DEBUG_LED		(DEBUG_LED_0|DEBUG_LED_1|DEBUG_LED_2|DEBUG_LED_3)
static volatile unsigned char	*debug_led_addr=NULL;
static volatile unsigned char	*misc_base_addr=NULL;
#endif
static struct resource		*misc_base_res=NULL;

#define RESET_POLL_TIME		(HZ/5)
#define RESET_TIMEOUT		(HZ * 5)
//
// file operaiton function call
//
#define IOCTL_SW_READY_ON	_IO(MOXA_MISC_MINOR,1)
#define IOCTL_SW_READY_OFF	_IO(MOXA_MISC_MINOR,2)

#define IOCTL_DEBUG_LED_OUT	0x100
#define IOCTL_GET_IGT		0x101
static spinlock_t		resetlock;

#if defined(CONFIG_MACH_MOXA_UC8000) && !defined(CONFIG_MACH_MOXA_UC8406_NGC)
//-- start section -- keypad driver ---------------------------
// keypad file operation function call
#define IOCTL_KEYPAD_HAS_PRESS  1
#define IOCTL_KEYPAD_GET_KEY    2

#define KEY0                    0
#define KEY1                    1
#define KEY2                    2
#define KEY3                    3
#define KEY4                    4

#define KEYPAD_BUFFER_LENGTH    32
#define KEYPAD_BUFFER_MASK      (KEYPAD_BUFFER_LENGTH-1)
#define KEYPAD_KEY0             (1<<0)
#define KEYPAD_KEY1             (1<<1)
#define KEYPAD_KEY2             (1<<2)
#define KEYPAD_KEY3             (1<<3)
#define KEYPAD_KEY4             (1<<4)
#define KEYPAD_MASK             0x1f
#define KEYPAD_POLL_TIME        (HZ/5)

static  int             keypad_wptr=0, keypad_rptr=0;
static  unsigned char   keypad_buffer[KEYPAD_BUFFER_LENGTH];
static  unsigned char   keypadold;
static  struct timer_list keypadtimer;
static  int             keypadtimer_on=0;
static  int             keypad_opened=0;

static void keypad_poll(unsigned long ingore)
{
        unsigned char   v, v1;

        spin_lock(&keypad_lock);
        del_timer(&keypadtimer);
        keypadtimer_on = 0;
        if ( keypad_opened <= 0 ) {
                spin_unlock(&keypad_lock);
                return;
        }
        v = *keypad_addr & KEYPAD_MASK;
        v1 = v ^ keypadold;
        keypadold = v;
        if ( v1 ) {
                if ( ((keypad_wptr+1)&KEYPAD_BUFFER_MASK) != keypad_rptr ) {
                        if ( !(v & KEYPAD_KEY0) && (v1 & KEYPAD_KEY0) ) {
                                keypad_buffer[keypad_wptr++] = KEY0;
                        } else if ( !(v & KEYPAD_KEY1) && (v1 & KEYPAD_KEY1) ) {
                                keypad_buffer[keypad_wptr++] = KEY1;
                        } else if ( !(v & KEYPAD_KEY2) && (v1 & KEYPAD_KEY2) ) {
                                keypad_buffer[keypad_wptr++] = KEY2;
                        } else if ( !(v & KEYPAD_KEY3) && (v1 & KEYPAD_KEY3) ) {
                                keypad_buffer[keypad_wptr++] = KEY3;
                        } else if ( !(v & KEYPAD_KEY4) && (v1 & KEYPAD_KEY4) ) {
                                keypad_buffer[keypad_wptr++] = KEY4;
                        }
                        keypad_wptr &= KEYPAD_BUFFER_MASK;
                }
        }      

        keypadtimer.function = keypad_poll;
        keypadtimer.expires = jiffies + KEYPAD_POLL_TIME;
        keypadtimer_on = 1;
        add_timer(&keypadtimer);
        spin_unlock(&keypad_lock);
}

static int keypad_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
        int             v;

        switch ( cmd ) {
        case IOCTL_KEYPAD_HAS_PRESS :
                spin_lock(&keypad_lock);
#if 1
                v = (keypad_wptr - keypad_rptr) & KEYPAD_BUFFER_MASK;
#else
                if ( keypad_wptr == keypad_rptr )
                        v = 0;
                else
                        v = 1;
#endif
                spin_unlock(&keypad_lock);
                if ( copy_to_user((void *)arg, &v, sizeof(int)) )
                        return -EFAULT;
                break;
        case IOCTL_KEYPAD_GET_KEY :
                spin_lock(&keypad_lock);
                if ( keypad_wptr == keypad_rptr )
                        v = -1;
                else {
                        v = keypad_buffer[keypad_rptr++];
                        keypad_rptr &= KEYPAD_BUFFER_MASK;
                }
                spin_unlock(&keypad_lock);
                if ( copy_to_user((void *)arg, &v, sizeof(int)) )
                        return -EFAULT;
                break;
        default:
                return -EINVAL;
        }
        return 0;
}

static int keypad_open(struct inode *inode, struct file *file)
{
        if ( MINOR(inode->i_rdev) != MOXA_KEYPAD_MINOR )
                return -ENODEV;
        spin_lock(&keypad_lock);
        if ( keypad_opened == 0 ) {
                keypad_wptr = keypad_rptr = 0;
                keypadold = *keypad_addr & KEYPAD_MASK;
                keypadtimer.function = keypad_poll;
                keypadtimer.expires = jiffies + KEYPAD_POLL_TIME;
                keypadtimer_on = 1;
                add_timer(&keypadtimer);
        }
        keypad_opened++;
        spin_unlock(&keypad_lock);
        return 0;
}

static int keypad_release(struct inode *inode, struct file *file)
{
        spin_lock(&keypad_lock);
        if ( keypad_opened > 0 ) {
                keypad_opened--;
                if ( keypad_opened == 0 ) {
                        if ( keypadtimer_on ) {
                                del_timer(&keypadtimer);
                                keypadtimer_on = 0;
                        }
                        keypad_wptr = keypad_rptr = 0;
                }
        }
        spin_unlock(&keypad_lock);
        return 0;
}

static struct file_operations keypad_fops = {
        owner:THIS_MODULE,
        llseek:NULL,
        ioctl:keypad_ioctl,
        open:keypad_open,
        release:keypad_release,
};
static struct miscdevice keypad_dev = {
        MOXA_KEYPAD_MINOR,
        "keypad",
        &keypad_fops
};
//-- end section -- keypad driver ------------------------------------

//-- start section -- LCM driver ------------------------------------
static unsigned char fnt8x8[]={
0x3E,0x51,0x49,0x45,0x3E,0x00,0x00,0x00,0x40,0x42,0x7F,0x40,0x40,0x00,0x00,0x00,
0x42,0x61,0x51,0x49,0x66,0x00,0x00,0x00,0x22,0x49,0x49,0x49,0x36,0x00,0x00,0x00,
0x18,0x14,0x52,0x7F,0x50,0x00,0x00,0x00,0x27,0x45,0x45,0x45,0x39,0x00,0x00,0x00,
0x3C,0x4A,0x49,0x49,0x32,0x00,0x00,0x00,0x03,0x01,0x79,0x05,0x03,0x00,0x00,0x00,
0x36,0x49,0x49,0x49,0x36,0x00,0x00,0x00,0x26,0x49,0x49,0x49,0x3E,0x00,0x00,0x00,
0x7C,0x12,0x11,0x12,0x7C,0x00,0x00,0x00,0x41,0x7F,0x49,0x49,0x36,0x00,0x00,0x00,
0x1C,0x22,0x41,0x41,0x22,0x00,0x00,0x00,0x7F,0x41,0x41,0x22,0x1C,0x00,0x00,0x00,
0x41,0x7F,0x49,0x49,0x63,0x00,0x00,0x00,0x41,0x7F,0x49,0x09,0x03,0x00,0x00,0x00,
0x7F,0x3E,0x3E,0x1C,0x1C,0x08,0x08,0x00,0x08,0x08,0x1C,0x1C,0x3E,0x3E,0x7F,0x00,
0x00,0x24,0x66,0xFF,0xFF,0x66,0x24,0x00,0x00,0x5F,0x5F,0x00,0x00,0x5F,0x5F,0x00,
0x06,0x0F,0x09,0x7F,0x7F,0x01,0x7F,0x7F,0x40,0xDA,0xBF,0xA5,0xFD,0x59,0x03,0x02,
0x00,0x70,0x70,0x70,0x70,0x70,0x70,0x00,0x80,0x94,0xB6,0xFF,0xFF,0xB6,0x94,0x80,
0x00,0x04,0x06,0x7F,0x7F,0x06,0x04,0x00,0x00,0x10,0x30,0x7F,0x7F,0x30,0x10,0x00,
0x08,0x08,0x08,0x2A,0x3E,0x1C,0x08,0x00,0x08,0x1C,0x3E,0x2A,0x08,0x08,0x08,0x00,
0x3C,0x3C,0x20,0x20,0x20,0x20,0x20,0x00,0x08,0x1C,0x3E,0x08,0x08,0x3E,0x1C,0x08,
0x30,0x38,0x3C,0x3E,0x3E,0x3C,0x38,0x30,0x06,0x0E,0x1E,0x3E,0x3E,0x1E,0x0E,0x06,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5F,0x00,0x00,0x00,0x00,
0x00,0x07,0x00,0x00,0x00,0x07,0x00,0x00,0x14,0x7F,0x14,0x14,0x14,0x7F,0x14,0x00,
0x24,0x2A,0x6B,0x2A,0x10,0x00,0x00,0x00,0x43,0x23,0x10,0x08,0x04,0x62,0x61,0x00,
0x32,0x4D,0x49,0x4D,0x52,0x28,0x40,0x00,0x04,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x1C,0x22,0x41,0x00,0x00,0x00,0x00,0x00,0x41,0x22,0x1C,0x00,0x00,0x00,
0x08,0x2A,0x1C,0x08,0x08,0x1C,0x2A,0x08,0x00,0x08,0x08,0x3E,0x08,0x08,0x00,0x00,
0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x00,0x00,0x00,
0x00,0x00,0x60,0x60,0x00,0x00,0x00,0x00,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,
0x3E,0x61,0x51,0x49,0x45,0x43,0x3E,0x00,0x40,0x42,0x7F,0x40,0x40,0x00,0x00,0x00,
0x62,0x51,0x51,0x49,0x49,0x46,0x00,0x00,0x22,0x41,0x41,0x49,0x49,0x36,0x00,0x00,
0x18,0x14,0x12,0x11,0x51,0x7F,0x50,0x00,0x27,0x45,0x45,0x45,0x45,0x39,0x00,0x00,
0x3C,0x4A,0x49,0x49,0x48,0x30,0x00,0x00,0x03,0x01,0x01,0x71,0x09,0x07,0x00,0x00,
0x36,0x49,0x49,0x49,0x49,0x36,0x00,0x00,0x06,0x49,0x49,0x49,0x29,0x1E,0x00,0x00,
0x00,0x00,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x66,0x00,0x00,0x00,0x00,0x00,
0x08,0x14,0x22,0x41,0x00,0x00,0x00,0x00,0x24,0x24,0x24,0x24,0x24,0x00,0x00,0x00,
0x00,0x00,0x41,0x22,0x14,0x08,0x00,0x00,0x02,0x01,0x01,0x51,0x09,0x06,0x00,0x00,
0x3E,0x41,0x41,0x51,0x51,0x11,0x0E,0x00,0x7C,0x12,0x11,0x12,0x7C,0x00,0x00,0x00,
0x41,0x7F,0x49,0x49,0x49,0x36,0x00,0x00,0x1C,0x22,0x41,0x41,0x41,0x22,0x00,0x00,
0x41,0x7F,0x41,0x41,0x41,0x22,0x1C,0x00,0x7F,0x49,0x49,0x49,0x41,0x41,0x41,0x00,
0x7F,0x09,0x09,0x09,0x01,0x01,0x01,0x00,0x1C,0x22,0x41,0x41,0x41,0x51,0x32,0x00,
0x7F,0x08,0x08,0x08,0x08,0x7F,0x00,0x00,0x00,0x41,0x7F,0x41,0x00,0x00,0x00,0x00,
0x30,0x40,0x40,0x40,0x41,0x3F,0x01,0x00,0x00,0x7F,0x08,0x08,0x08,0x14,0x63,0x00,
0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x00,0x7F,0x02,0x04,0x08,0x04,0x02,0x7F,0x00,
0x7F,0x02,0x04,0x08,0x10,0x20,0x7F,0x00,0x1C,0x22,0x41,0x41,0x41,0x22,0x1C,0x00,
0x00,0x7F,0x09,0x09,0x09,0x09,0x06,0x00,0x1E,0x21,0x21,0x21,0x61,0x5E,0x00,0x00,
0x7F,0x09,0x09,0x09,0x19,0x66,0x00,0x00,0x26,0x49,0x49,0x49,0x49,0x32,0x00,0x00,
0x01,0x01,0x01,0x7F,0x01,0x01,0x01,0x00,0x3F,0x40,0x40,0x40,0x40,0x3F,0x00,0x00,
0x1F,0x20,0x40,0x40,0x20,0x1F,0x00,0x00,0x7F,0x20,0x10,0x08,0x10,0x20,0x7F,0x00,
0x41,0x22,0x14,0x08,0x14,0x22,0x41,0x00,0x07,0x08,0x78,0x08,0x07,0x00,0x00,0x00,
0x41,0x61,0x51,0x49,0x45,0x43,0x41,0x00,0x00,0x7F,0x41,0x41,0x41,0x00,0x00,0x00,
0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x00,0x00,0x41,0x41,0x41,0x7F,0x00,0x00,0x00,
0x08,0x04,0x02,0x01,0x02,0x04,0x08,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x00,0x00,0x00,0x03,0x04,0x00,0x00,0x00,0x20,0x54,0x54,0x54,0x14,0x78,0x40,0x00,
0x00,0x7F,0x48,0x48,0x48,0x48,0x30,0x00,0x38,0x44,0x44,0x44,0x44,0x28,0x00,0x00,
0x30,0x48,0x48,0x48,0x48,0x3F,0x00,0x00,0x38,0x54,0x54,0x54,0x54,0x08,0x00,0x00,
0x08,0x7E,0x09,0x09,0x01,0x02,0x00,0x00,0x98,0xA4,0xA4,0xA4,0xA4,0x78,0x00,0x00,
0x00,0x7F,0x08,0x08,0x08,0x08,0x70,0x00,0x00,0x00,0x00,0x7D,0x00,0x00,0x00,0x00,
0x60,0x80,0x80,0x80,0x80,0x7D,0x00,0x00,0x7F,0x10,0x10,0x10,0x28,0x44,0x00,0x00,
0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x78,0x04,0x08,0x10,0x08,0x04,0x78,0x00,
0x7C,0x04,0x04,0x04,0x04,0x78,0x00,0x00,0x38,0x44,0x44,0x44,0x44,0x38,0x00,0x00,
0xFC,0x24,0x24,0x24,0x24,0x18,0x00,0x00,0x18,0x24,0x24,0x24,0x24,0xFC,0x00,0x00,
0x7C,0x08,0x04,0x04,0x04,0x08,0x00,0x00,0x48,0x54,0x54,0x54,0x54,0x24,0x00,0x00,
0x00,0x00,0x04,0x7F,0x04,0x00,0x00,0x00,0x3C,0x40,0x40,0x40,0x40,0x3C,0x00,0x00,
0x1C,0x20,0x40,0x40,0x20,0x1C,0x00,0x00,0x3C,0x40,0x20,0x10,0x20,0x40,0x3C,0x00,
0x00,0x44,0x28,0x10,0x28,0x44,0x00,0x00,0x9C,0xA0,0xA0,0xA0,0xA0,0x7C,0x00,0x00,
0x44,0x44,0x64,0x54,0x4C,0x44,0x00,0x00,0x00,0x08,0x08,0x36,0x41,0x41,0x00,0x00,
0x00,0x00,0x00,0x77,0x00,0x00,0x00,0x00,0x41,0x41,0x36,0x08,0x08,0x00,0x00,0x00,
0x02,0x03,0x01,0x03,0x02,0x03,0x01,0x00,0x70,0x48,0x44,0x42,0x44,0x48,0x70,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

// following on LCM command
#define LCM_CMD_DISPLAY_ON              0x3f
#define LCM_CMD_DISPLAY_OFF             0x3e
#define LCM_CMD_DISPLAY_START_LINE      0xc0
#define LCM_DISPLAY_START_LINE_MASK     0x3F    // 0-63
#define LCM_CMD_SET_PAGE                0xb8    // x address
#define LCM_SET_PAGE_MASK               0x07    // x address 0-7
#define LCM_CMD_SET_ADDRESS             0x40    // y address, from 0x40 to 0xc0
#define LCM_SET_ADDRESS_MASK            0x3F    // y address 0-63
#define LCM_STATUS_BUSY                 0x80
#define LCM_STATUS_ON                   0x20
#define LCM_STATUS_RESET                0x10
#define LCM_MODE_SELECT                 0x05

// following on LCM register 0
#define LCM_CS1                 0x01
#define LCM_CS2                 0x02

// following on LCM register 1
#define LCM_RESET               0x01
#define LCM_READ                0x02
#define LCM_WRITE               0x00
#define LCM_DATA                0x10
#define LCM_CMD                 0x00
#define SW_READY_LED_ON         0x00
#define SW_READY_LED_OFF        0x08
#define LCM_BACK_LIGHT_ON       0x20
#define LCM_BACK_LIGHT_OFF      0x00
#define LCM_EBIT_ON             0x04
#define LCM_EBIT_OFF            0x00
#define LCM_373WEN_ON           0x40
#define LCM_373WEN_OFF          0x00
#define DOUT_NEED_ON            0x40

#define LCM_MAX_X_DOTS          128
#define LCM_MAX_Y_DOTS          64
#define LCM_HALF_X_DOTS         (LCM_MAX_X_DOTS/2)
#define LCM_X_DOTS              8
#define LCM_Y_DOTS              8
#define LCM_MAX_X               (LCM_MAX_X_DOTS/LCM_X_DOTS)
#define LCM_MAX_Y               (LCM_MAX_Y_DOTS/LCM_Y_DOTS)

// ioctl command define
#define IOCTL_LCM_GOTO_XY               1
#define IOCTL_LCM_CLS                   2
#define IOCTL_LCM_CLEAN_LINE            3
#define IOCTL_LCM_GET_XY                4
#define IOCTL_LCM_BACK_LIGHT_ON         5
#define IOCTL_LCM_BACK_LIGHT_OFF        6
#define IOCTL_LCM_PIX_ON                7
#define IOCTL_LCM_PIX_OFF               8
#define IOCTL_LCM_CURSOR_ON             9
#define IOCTL_LCM_CURSOR_OFF            10
#define IOCTL_LCM_AUTO_SCROLL_ON        13
#define IOCTL_LCM_AUTO_SCROLL_OFF       14

typedef struct lcm_xy {
        int     x;      // 0 - 15
        int     y;      // 0 - 7
} lcm_xy_t;

static unsigned char    lcm_pix_buffer[LCM_MAX_Y][LCM_MAX_X_DOTS];
static unsigned char    lcm_ctrl=LCM_BACK_LIGHT_ON|LCM_RESET;
static int              lcm_auto_scroll_flag=1;
static int              lcmx, lcmy;     // dots position

#define LCM_MUST_DELAY
#ifdef LCM_MUST_DELAY
#define	lcm_delay()	udelay(10)
#else	// LCM_MUST_DELAY
#define lcm_delay()
#endif	// LCM_MUST_DELAY

static void LCM_write_cmd(unsigned char cmd, unsigned char cs)
{
	dbg_printk("\n");
        spin_lock(&resetlock);
        *lcm_cmd1_addr = lcm_ctrl | sw_ready_value | LCM_EBIT_OFF | LCM_READ | LCM_CMD | LCM_373WEN_ON;
	lcm_delay();
        *lcm_cmd1_addr = lcm_ctrl | sw_ready_value | LCM_EBIT_OFF | LCM_WRITE | LCM_CMD | LCM_373WEN_ON;
	lcm_delay();
        *lcm_cmd0_addr = cs;
	lcm_delay();
        *lcm_cmd1_addr = lcm_ctrl | sw_ready_value | LCM_EBIT_ON | LCM_WRITE | LCM_CMD | LCM_373WEN_OFF;
	lcm_delay();
        *lcm_data_addr = cmd;
	lcm_delay();
        *lcm_cmd1_addr = lcm_ctrl | sw_ready_value | LCM_EBIT_OFF | LCM_WRITE | LCM_CMD | LCM_373WEN_ON;
	lcm_delay();
        *lcm_cmd1_addr = lcm_ctrl | sw_ready_value | LCM_EBIT_OFF | LCM_READ | LCM_CMD | LCM_373WEN_ON;
	lcm_delay();
        spin_unlock(&resetlock);
	dbg_printk("\n");
}

static void LCM_write_data(unsigned char data, unsigned char cs)
{
	dbg_printk("\n");
        spin_lock(&resetlock);
        *lcm_cmd1_addr = lcm_ctrl | sw_ready_value | LCM_EBIT_OFF | LCM_READ | LCM_DATA | LCM_373WEN_ON;
	lcm_delay();
        *lcm_cmd1_addr = lcm_ctrl | sw_ready_value | LCM_EBIT_OFF | LCM_WRITE | LCM_DATA | LCM_373WEN_ON;
	lcm_delay();
        *lcm_cmd0_addr = cs;
	lcm_delay();
        *lcm_cmd1_addr = lcm_ctrl | sw_ready_value | LCM_EBIT_ON | LCM_WRITE | LCM_DATA | LCM_373WEN_OFF;
	lcm_delay();
        *lcm_data_addr = data;
	lcm_delay();
        *lcm_cmd1_addr = lcm_ctrl | sw_ready_value | LCM_EBIT_OFF | LCM_WRITE | LCM_DATA | LCM_373WEN_ON;
	lcm_delay();
        *lcm_cmd1_addr = lcm_ctrl | sw_ready_value | LCM_EBIT_OFF | LCM_READ | LCM_DATA | LCM_373WEN_ON;
	lcm_delay();
        spin_unlock(&resetlock);
	dbg_printk("\n");
}

static void lcm_up_one_line(void)
{
        int     x, y, index, i;

        // first up one line for lcm buffer
        spin_lock(&resetlock);
        for ( y=1; y<LCM_MAX_Y; y++ ) {
                for ( x=0; x<LCM_MAX_X; x++ ) {
                        memcpy(&lcm_pix_buffer[y-1][x*LCM_X_DOTS], &lcm_pix_buffer[y][x*LCM_X_DOTS], LCM_X_DOTS);
                }
        }
        for ( x=0; x<LCM_MAX_X; x++ ) {
                memset(&lcm_pix_buffer[LCM_MAX_Y-1][x*LCM_X_DOTS], 0, LCM_X_DOTS);
        }
        spin_unlock(&resetlock);

        // second redisplay up one line on LCM
        LCM_write_cmd(LCM_CMD_DISPLAY_OFF, LCM_CS1);
        LCM_write_cmd(LCM_CMD_DISPLAY_OFF, LCM_CS2);
        for ( y=0; y<LCM_MAX_Y; y++ ) {
                for ( x=0; x<LCM_MAX_X; x++ ) {
                        if ( x == 0 ) {
                                LCM_write_cmd(LCM_CMD_SET_ADDRESS, LCM_CS1);
                                LCM_write_cmd(LCM_CMD_SET_PAGE | (y & LCM_SET_PAGE_MASK), LCM_CS1);
                        } else if ( x == (LCM_HALF_X_DOTS/LCM_X_DOTS) ) {
                                LCM_write_cmd(LCM_CMD_SET_ADDRESS, LCM_CS2);
                                LCM_write_cmd(LCM_CMD_SET_PAGE | (y & LCM_SET_PAGE_MASK), LCM_CS2);
                        }
                        index = x * LCM_X_DOTS;
                        for ( i=0; i<LCM_X_DOTS; i++, index++ ) {
                                if ( x < (LCM_HALF_X_DOTS/LCM_X_DOTS) ) {
                                        //LCM_write_data(fnt8x8[index], LCM_CS1);
                                        LCM_write_data(lcm_pix_buffer[y][index], LCM_CS1);
                                } else {
                                        //LCM_write_data(fnt8x8[index], LCM_CS2);
                                        LCM_write_data(lcm_pix_buffer[y][index], LCM_CS2);
                                }
                        }
                }
        }
        LCM_write_cmd(LCM_CMD_DISPLAY_ON, LCM_CS1);
        LCM_write_cmd(LCM_CMD_DISPLAY_ON, LCM_CS2);
        spin_lock(&resetlock);
        lcmx = 0;
        lcmy = LCM_MAX_Y - 1;
        spin_unlock(&resetlock);
}

static void LCM_printf_char(unsigned char ch)
{
        int             index, i;

	dbg_printk("\n");
        if ( ch == '\n' ) {
                spin_lock(&resetlock);
                for ( i=(lcmx/LCM_X_DOTS); i<LCM_MAX_X; i++ ) {
                        memset(&lcm_pix_buffer[lcmy][i*LCM_X_DOTS], 0, LCM_X_DOTS);
                }
                lcmx = 0;
                lcmy++;
                if ( lcmy >= LCM_MAX_Y ) {
                        if ( lcm_auto_scroll_flag ) {
                                lcmy = LCM_MAX_Y - 1;
                                spin_unlock(&resetlock);
                                lcm_up_one_line();
                                return;
                        } else {
                                lcmy = 0;
                        }
                }
                spin_unlock(&resetlock);
                return;
        }
        if ( lcmx < LCM_HALF_X_DOTS ) {
                LCM_write_cmd(LCM_CMD_SET_ADDRESS|(unsigned char)lcmx, LCM_CS1);
                LCM_write_cmd(LCM_CMD_SET_PAGE | (lcmy & LCM_SET_PAGE_MASK), LCM_CS1);
        } else {
                LCM_write_cmd(LCM_CMD_SET_ADDRESS|(unsigned char)(lcmx-LCM_HALF_X_DOTS), LCM_CS2);
                LCM_write_cmd(LCM_CMD_SET_PAGE | (lcmy & LCM_SET_PAGE_MASK), LCM_CS2);
        }
        index = ch * LCM_Y_DOTS;
        for ( i=0; i<LCM_X_DOTS; i++, index++, lcmx++ ) {
                spin_lock(&resetlock);
                lcm_pix_buffer[lcmy][lcmx] = fnt8x8[index];
                spin_unlock(&resetlock);
                if ( lcmx < LCM_HALF_X_DOTS )
                        LCM_write_data(fnt8x8[index], LCM_CS1);
                else
                        LCM_write_data(fnt8x8[index], LCM_CS2);
        }
        spin_lock(&resetlock);
        //lcmx += LCM_X_DOTS;
        if ( lcmx >= LCM_MAX_X_DOTS ) {
                lcmx = 0;
                lcmy++;
                if ( lcmy >= LCM_MAX_Y ) {
                        if ( lcm_auto_scroll_flag ) {
                                lcmy = LCM_MAX_Y - 1;
                                spin_unlock(&resetlock);
                                lcm_up_one_line();
                                return;
                        } else {
                                lcmy = 0;
                        }
                }
        }
        spin_unlock(&resetlock);
}

static void LCM_clear_line(void)
{
        int     x, index, i;

	dbg_printk("\n");
        for ( x=0; x<LCM_MAX_X; x++ ) {
                spin_lock(&resetlock);
                memset(&lcm_pix_buffer[lcmy][x*LCM_X_DOTS], 0, LCM_X_DOTS);
                spin_unlock(&resetlock);
                //LCM_write_cmd(LCM_CMD_DISPLAY_OFF, LCM_CS1);
                //LCM_write_cmd(LCM_CMD_DISPLAY_OFF, LCM_CS2);
                if ( x == 0 ) {
                        LCM_write_cmd(LCM_CMD_SET_ADDRESS, LCM_CS1);
                        LCM_write_cmd(LCM_CMD_SET_PAGE | (lcmy & LCM_SET_PAGE_MASK), LCM_CS1);
                } else if ( x == (LCM_HALF_X_DOTS/LCM_X_DOTS) ) {
                        LCM_write_cmd(LCM_CMD_SET_ADDRESS, LCM_CS2);
                        LCM_write_cmd(LCM_CMD_SET_PAGE | (lcmy & LCM_SET_PAGE_MASK), LCM_CS2);
                }
                index = x * LCM_X_DOTS;
                for ( i=0; i<LCM_X_DOTS; i++, index++ ) {
                        if ( x < (LCM_HALF_X_DOTS/LCM_X_DOTS) ) {
                                //LCM_write_data(fnt8x8[index], LCM_CS1);
                                LCM_write_data(lcm_pix_buffer[lcmy][index], LCM_CS1);
                        } else {
                                //LCM_write_data(fnt8x8[index], LCM_CS2);
                                LCM_write_data(lcm_pix_buffer[lcmy][index], LCM_CS2);
                        }
                }
                //LCM_write_cmd(LCM_CMD_DISPLAY_ON, LCM_CS1);
                //LCM_write_cmd(LCM_CMD_DISPLAY_ON, LCM_CS2);
        }
        spin_lock(&resetlock);
        lcmx = 0;
        spin_unlock(&resetlock);
}

static void LCM_cls(void)
{
        int     x, y;

	dbg_printk("\n");
        LCM_write_cmd(LCM_CMD_DISPLAY_OFF, LCM_CS1);
        LCM_write_cmd(LCM_CMD_DISPLAY_OFF, LCM_CS2);
        for ( y=0; y<LCM_MAX_Y; y++ ) {
                // clear LCM hardware
                for ( x=0; x<LCM_MAX_X_DOTS; x++ ) {
                        if ( x == 0 ) {
                                LCM_write_cmd(LCM_CMD_SET_ADDRESS, LCM_CS1);
                                LCM_write_cmd(LCM_CMD_SET_PAGE | (y & LCM_SET_PAGE_MASK), LCM_CS1);
                        } else if ( x == LCM_HALF_X_DOTS ) {
                                LCM_write_cmd(LCM_CMD_SET_ADDRESS, LCM_CS2);
                                LCM_write_cmd(LCM_CMD_SET_PAGE | (y & LCM_SET_PAGE_MASK), LCM_CS2);
                        }
                        spin_lock(&resetlock);
                        lcm_pix_buffer[y][x] = 0;
                        spin_unlock(&resetlock);
                        if ( x < LCM_HALF_X_DOTS )
                                LCM_write_data(0, LCM_CS1);
                        else
                                LCM_write_data(0, LCM_CS2);
                }
        }
        LCM_write_cmd(LCM_CMD_DISPLAY_ON, LCM_CS1);
        LCM_write_cmd(LCM_CMD_DISPLAY_ON, LCM_CS2);
        spin_lock(&resetlock);
        lcmx = lcmy = 0;
        spin_unlock(&resetlock);
}

/*
 * I don't know why cannot malloc memory from kernel.
 * 03-10-2004   Victor Yu.
 */
#define USE_WRITE_BUFFER
static ssize_t lcm_write(struct file * filp, const char * buf, size_t count, loff_t *ppos)
{
#ifdef USE_WRITE_BUFFER
        char    write_buffer[LCM_MAX_X * LCM_MAX_Y];
#else
        char    *ptr;
#endif
        int     i;

	dbg_printk("\n");
        if ( count == 0 )
                return 0;
	dbg_printk("\n");
#ifdef USE_WRITE_BUFFER
        if ( count > (LCM_MAX_X * LCM_MAX_Y) )
                count = LCM_MAX_X * LCM_MAX_Y;
        if ( copy_from_user(write_buffer, buf, count) ) {
                return -EFAULT;
        }
        for ( i=0; i<count; i++ )
                LCM_printf_char(write_buffer[i]);
#else
        ptr = kmalloc(count, GFP_KERNEL);
        if ( ptr == NULL )
                return -ENOMEM;
        if ( copy_from_user(ptr, buf, count) ) {
                kfree(ptr);
                return -EFAULT;
        }
        for ( i=0; i<count; i++ )
                LCM_printf_char(*ptr++);
        kfree(ptr);
#endif
	dbg_printk("\n");
        return count;
}

static int lcm_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
        lcm_xy_t        pos;

        switch ( cmd ) {
        case IOCTL_LCM_PIX_ON :
        case IOCTL_LCM_PIX_OFF :
        {
                unsigned char   v, mask;

                if ( copy_from_user(&pos, (void *)arg, sizeof(pos)) )
                        return -EFAULT;
                if ( pos.x < 0 || pos.x >= LCM_MAX_X_DOTS || pos.y < 0 || pos.y >= LCM_MAX_Y_DOTS )
                        return -EINVAL;

                spin_lock(&resetlock);
                v = lcm_pix_buffer[pos.y/LCM_Y_DOTS][pos.x];
                mask = 1 << (pos.y % LCM_Y_DOTS);
                if ( cmd == IOCTL_LCM_PIX_ON )
                        v |= mask;
                else
                        v &= ~mask;
                lcm_pix_buffer[pos.y/LCM_Y_DOTS][pos.x] = v;
                spin_unlock(&resetlock);
                if ( pos.x < LCM_HALF_X_DOTS ) {
                        LCM_write_cmd(LCM_CMD_SET_ADDRESS|(unsigned char)pos.x, LCM_CS1);
                        LCM_write_cmd(LCM_CMD_SET_PAGE | ((pos.y/LCM_Y_DOTS) & LCM_SET_PAGE_MASK), LCM_CS1);
                        LCM_write_data(v, LCM_CS1);
                } else {
                        LCM_write_cmd(LCM_CMD_SET_ADDRESS|(unsigned char)(pos.x-LCM_HALF_X_DOTS), LCM_CS2);
                        LCM_write_cmd(LCM_CMD_SET_PAGE | ((pos.y/LCM_Y_DOTS) & LCM_SET_PAGE_MASK), LCM_CS2);
                        LCM_write_data(v, LCM_CS2);
                }
                break;
        }
        case IOCTL_LCM_GOTO_XY :
                if ( copy_from_user(&pos, (void *)arg, sizeof(pos)) )
                        return -EFAULT;
                if ( pos.x < 0 || pos.x >= LCM_MAX_X || pos.y < 0 || pos.y >= LCM_MAX_Y )
                        return -EINVAL;
                spin_lock(&resetlock);
                lcmx = pos.x * LCM_X_DOTS;
                lcmy = pos.y;
                spin_unlock(&resetlock);
                break;
        case IOCTL_LCM_CLS :
                LCM_cls();
                break;
        case IOCTL_LCM_CLEAN_LINE :
                LCM_clear_line();
                break;
        case IOCTL_LCM_GET_XY :
                spin_lock(&resetlock);
                pos.x = lcmx / LCM_X_DOTS;
                pos.y = lcmy;
                spin_unlock(&resetlock);
                if ( copy_to_user((void *)arg, &pos,sizeof(pos)) )
                        return -EFAULT;
                break;
        case IOCTL_LCM_BACK_LIGHT_ON :
                spin_lock(&resetlock);
                lcm_ctrl |= LCM_BACK_LIGHT_ON;
                lcm_ctrl |= sw_ready_value;
                *lcm_cmd1_addr = lcm_ctrl;
                spin_unlock(&resetlock);
                break;
        case IOCTL_LCM_BACK_LIGHT_OFF :
                spin_lock(&resetlock);
                lcm_ctrl &= ~LCM_BACK_LIGHT_ON;
                lcm_ctrl |= sw_ready_value;
                *lcm_cmd1_addr = lcm_ctrl;
                spin_unlock(&resetlock);
                break;
        case IOCTL_LCM_AUTO_SCROLL_ON :
                spin_lock(&resetlock);
                lcm_auto_scroll_flag = 1;
                spin_unlock(&resetlock);
                break;
        case IOCTL_LCM_AUTO_SCROLL_OFF :
                spin_lock(&resetlock);
                lcm_auto_scroll_flag = 0;
                spin_unlock(&resetlock);
                break;
        default:
                return -EINVAL;
        }

        return 0;
}

static int lcm_open(struct inode *inode, struct file *file)
{
        if ( MINOR(inode->i_rdev) == MOXA_LCM_MINOR )
                return 0;
        return -ENODEV;
}

static int lcm_release(struct inode *inode, struct file *file)
{
        return 0;
}

static struct file_operations lcm_fops = {
        owner:THIS_MODULE,
        llseek:NULL,
        write:lcm_write,
        ioctl:lcm_ioctl,
        open:lcm_open,
        release:lcm_release,
};
static struct miscdevice lcm_dev = {
        MOXA_LCM_MINOR,
        "lcm",
        &lcm_fops
};
//-- end section -- LCM driver ------------------------------------
#endif	// CONFIG_MACH_MOXA_UC8000 && !CONFIG_MACH_MOXA_UC8406_NGC

//-- start section -- reset to default and sw ready driver ------------------
static struct timer_list	resettimer;
static unsigned long		endresettime, intervaltime;
static int			ledonoffflag;
static struct work_struct	resetqueue;

static void     settodefault(struct work_struct *unused)
{
        char    *argv[2], *envp[5];

        if ( in_interrupt() )
                return;
#if 0  // Jared 08-03-2011
        if ( !current->fs->root )
                return;
#endif
        argv[0] = "/bin/setdef";
        argv[1] = 0;
        envp[0] = "HOME=/";
        envp[1] = "PATH=/sbin:/bin:/usr/sbin:/usr/bin";
        envp[2] = 0;
        call_usermodehelper(argv[0], argv, envp, 0);
}

static void reset_poll(unsigned long ingore)
{
#if (defined CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
	int	v;
#endif	// CONFIG_MACH_MOXA_UC8400

	spin_lock(&resetlock);
	del_timer(&resettimer);
#if defined(CONFIG_ARCH_IA240) || defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345) || defined(CONFIG_ARCH_UC_7112_LX_PLUS) || defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W325) || defined(CONFIG_ARCH_W315)
	if ( !moxaart_gpio_get(SW_RESET_GPIO) )
#elif defined(CONFIG_MACH_MOXA_W406) || defined(CONFIG_MACH_MOXA_IA261)
	if ( !gpio_get_value(SW_RESET_GPIO) )
#elif defined (CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
	gpio_line_get(SW_RESET_GPIO, &v);
	if ( !v )
#elif defined (CONFIG_MACH_MOXA_UC8000)
	#if defined(CONFIG_MACH_MOXA_UC8406_NGC)
	if ( !(*sw_reset_addr & SW_RESET_GPIO) )
	#else	// CONFIG_MACH_MOXA_UC8406_NGC
	if ( (*sw_reset_addr & SW_RESET_GPIO) )
	#endif	// CONFIG_MACH_MOXA_UC8406_NGC)
#endif
	{
		if ( endresettime == 0 ) {
			endresettime = jiffies + RESET_TIMEOUT;
			intervaltime = jiffies + HZ;
		} else if ( time_after(jiffies, endresettime) ){ // need to reset
#if defined(CONFIG_ARCH_IA240) || defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345) || defined(CONFIG_ARCH_UC_7112_LX_PLUS) || defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W325) || defined(CONFIG_ARCH_W315)
			moxaart_gpio_set(SW_READY_GPIO, MCPU_GPIO_HIGH);
#elif defined(CONFIG_MACH_MOXA_IA261) || defined(CONFIG_MACH_MOXA_W406)
			gpio_set_value(SW_READY_GPIO, MCPU_GPIO_LOW);
#elif defined (CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
			gpio_line_set(SW_READY_GPIO, IXP4XX_GPIO_LOW);
#elif defined (CONFIG_MACH_MOXA_UC8000)
			sw_ready_value |= SW_READY_GPIO;
#endif
			schedule_work(&resetqueue);
			goto poll_exit;
		} else if ( time_after(jiffies, intervaltime) ) {
			if ( ledonoffflag ) {	// off ready led
				ledonoffflag = 0;
#if defined(CONFIG_MACH_MOXA_IA261) || defined(CONFIG_MACH_MOXA_W406) 
                gpio_set_value(SW_READY_GPIO, MCPU_GPIO_LOW);
#elif defined(CONFIG_ARCH_IA240) || defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345) || defined(CONFIG_ARCH_UC_7112_LX_PLUS) || defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W325) || defined(CONFIG_ARCH_W315)
				moxaart_gpio_set(SW_READY_GPIO, MCPU_GPIO_HIGH);
#elif defined (CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
				gpio_line_set(SW_READY_GPIO, IXP4XX_GPIO_LOW);
#elif defined (CONFIG_MACH_MOXA_UC8000)
				sw_ready_value |= SW_READY_GPIO;
#endif
			} else {	// on ready led
				ledonoffflag = 1;
#if defined(CONFIG_MACH_MOXA_IA261) || defined(CONFIG_MACH_MOXA_W406)
				gpio_set_value(SW_READY_GPIO, MCPU_GPIO_HIGH);
#elif defined(CONFIG_ARCH_IA240) || defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345) || defined(CONFIG_ARCH_UC_7112_LX_PLUS) || defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W325) || defined(CONFIG_ARCH_W315)
				moxaart_gpio_set(SW_READY_GPIO, MCPU_GPIO_LOW);
#elif defined (CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
				gpio_line_set(SW_READY_GPIO, IXP4XX_GPIO_HIGH);
#elif defined (CONFIG_MACH_MOXA_UC8000)
				sw_ready_value &= ~SW_READY_GPIO;
#endif
			}
			intervaltime = jiffies + HZ;
		}
	} else {
		if ( endresettime ) {	// abort to reset process
			endresettime = 0;
			ledonoffflag = 1;
#if defined(CONFIG_MACH_MOXA_IA261) || defined(CONFIG_MACH_MOXA_W406)
		gpio_set_value(SW_READY_GPIO, MCPU_GPIO_HIGH);
#elif defined(CONFIG_ARCH_IA240) || defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345) || defined(CONFIG_ARCH_UC_7112_LX_PLUS) || defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W325) || defined(CONFIG_ARCH_W315)
			moxaart_gpio_set(SW_READY_GPIO, MCPU_GPIO_LOW);
#elif defined (CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
			gpio_line_set(SW_READY_GPIO, IXP4XX_GPIO_HIGH);
#elif defined (CONFIG_MACH_MOXA_UC8000)
			sw_ready_value &= ~SW_READY_GPIO;
#endif
		}
	}
	resettimer.function = reset_poll;
	resettimer.expires = jiffies + RESET_POLL_TIME;
	add_timer(&resettimer);
poll_exit:
#ifdef CONFIG_MACH_MOXA_UC8000	// add by Victor Yu. 04-27-2009
	#if defined(CONFIG_MACH_MOXA_UC8406_NGC)
	*sw_ready_addr = sw_ready_value;
	#else	// CONFIG_MACH_MOXA_UC8406_NGC
	*sw_ready_addr = sw_ready_value | lcm_ctrl;
	#endif	// CONFIG_MACH_MOXA_UC8406_NGC
#endif
	spin_unlock(&resetlock);
}

static long mxmisc_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch ( cmd ) {
#if defined(CONFIG_MACH_MOXA_IA261)
	case IOCTL_USER_LED1_ON :
		gpio_set_value(USER_LED1, MCPU_GPIO_LOW);
		break;
	case IOCTL_USER_LED1_OFF :
		gpio_set_value(USER_LED1, MCPU_GPIO_HIGH);
		break;
	case IOCTL_USER_LED2_ON :
		gpio_set_value(USER_LED2, MCPU_GPIO_LOW);
		break;
	case IOCTL_USER_LED2_OFF :
		gpio_set_value(USER_LED2, MCPU_GPIO_HIGH);
		break;
#endif
	case IOCTL_SW_READY_ON :
		spin_lock(&resetlock);
#if defined(CONFIG_MACH_MOXA_IA261) || defined(CONFIG_MACH_MOXA_W406)
		gpio_set_value(SW_READY_GPIO, MCPU_GPIO_HIGH);
#elif defined(CONFIG_ARCH_IA240) || defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345) || defined(CONFIG_ARCH_UC_7112_LX_PLUS) || defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W325) || defined(CONFIG_ARCH_W315)
		moxaart_gpio_set(SW_READY_GPIO, MCPU_GPIO_LOW);
#elif defined (CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
		gpio_line_set(SW_READY_GPIO, IXP4XX_GPIO_HIGH);
#elif defined (CONFIG_MACH_MOXA_UC8000)	// add by Victor Yu. 04-27-2009
		sw_ready_value &= ~SW_READY_GPIO;
	#if defined(CONFIG_MACH_MOXA_UC8406_NGC)
		*sw_ready_addr = sw_ready_value;
	#else	// CONFIG_MACH_MOXA_UC8406_NGC
		*sw_ready_addr = sw_ready_value | lcm_ctrl;
	#endif	// CONFIG_MACH_MOXA_UC8406_NGC
#endif
		spin_unlock(&resetlock);
		break;
	case IOCTL_SW_READY_OFF:
		spin_lock(&resetlock);
#if defined(CONFIG_MACH_MOXA_IA261) || defined(CONFIG_MACH_MOXA_W406)
		gpio_set_value(SW_READY_GPIO, MCPU_GPIO_LOW);
#elif defined(CONFIG_ARCH_IA240) || defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345) || defined(CONFIG_ARCH_UC_7112_LX_PLUS) || defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W325) || defined(CONFIG_ARCH_W315)
		moxaart_gpio_set(SW_READY_GPIO, MCPU_GPIO_HIGH);
#elif defined (CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
		gpio_line_set(SW_READY_GPIO, IXP4XX_GPIO_LOW);
#elif defined (CONFIG_MACH_MOXA_UC8000)
		sw_ready_value |= SW_READY_GPIO;
	#if defined(CONFIG_MACH_MOXA_UC8406_NGC)
		*sw_ready_addr = sw_ready_value;
	#else	// CONFIG_MACH_MOXA_UC8406_NGC
		*sw_ready_addr = sw_ready_value | lcm_ctrl;
	#endif	// CONFIG_MACH_MOXA_UC8406_NGC
#endif
		spin_unlock(&resetlock);
		break;

	case IOCTL_DEBUG_LED_OUT:
		{
		unsigned int	val;
		if ( copy_from_user(&val, (unsigned int *)arg, sizeof(unsigned int)) )
			return -EFAULT;
#if defined(CONFIG_ARCH_IA240) || defined(CONFIG_ARCH_IA241) // mask by Victor Yu. 04-20-2007
		if ( val & 1 )
			moxaart_gpio_set(DEBUG_LED_0, MCPU_GPIO_LOW);
		else
			moxaart_gpio_set(DEBUG_LED_0, MCPU_GPIO_HIGH);
		if ( val & 2 )
			moxaart_gpio_set(DEBUG_LED_1, MCPU_GPIO_LOW);
		else
			moxaart_gpio_set(DEBUG_LED_1, MCPU_GPIO_HIGH);
		if ( val & 4 )
			moxaart_gpio_set(DEBUG_LED_2, MCPU_GPIO_LOW);
		else
			moxaart_gpio_set(DEBUG_LED_2, MCPU_GPIO_HIGH);
		if ( val & 8 )
			moxaart_gpio_set(DEBUG_LED_3, MCPU_GPIO_LOW);
		else
			moxaart_gpio_set(DEBUG_LED_3, MCPU_GPIO_HIGH);
#elif defined(CONFIG_MACH_MOXA_IA261) // Only IA26X has DEBUG LED 4,5
		if ( val & 0x10 )
			gpio_set_value(DEBUG_LED_4, MCPU_GPIO_LOW);
		else
			gpio_set_value(DEBUG_LED_4, MCPU_GPIO_HIGH);
		if ( val & 0x20 )
			gpio_set_value(DEBUG_LED_5, MCPU_GPIO_HIGH);
		else
			gpio_set_value(DEBUG_LED_5, MCPU_GPIO_LOW);
#elif defined(CONFIG_MACH_MOXA_W406) //Johnson Liu add 2009-02-24
		outb((unsigned char)(val&0x1f),MOXA_IA261_UART_DIO_VIRT_BASE + 0xa0);
#elif defined(CONFIG_MACH_MOXA_UC8000) ||defined (CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
		*debug_led_addr = val;
#endif
		break;
		}
#if defined (CONFIG_MACH_MOXA_UC8480)
	case IOCTL_GET_IGT:
		{
		unsigned int	val;
		val = *igt_addr;
                if ( copy_to_user((void *)arg, &val, sizeof(int)) )
                        return -EFAULT;
		break;
		}
#endif
	default:
		return -EINVAL;
	}
	return 0;
}

static int mxmisc_open(struct inode *inode, struct file *file)
{
	if ( MINOR(inode->i_rdev) == MOXA_MISC_MINOR )
		return 0;
	return -ENODEV;
}

static int mxmisc_release(struct inode *inode, struct file *file)
{
	return 0;
}

static struct file_operations mxmisc_fops = {
	owner:THIS_MODULE,
	llseek:NULL,
	unlocked_ioctl:mxmisc_ioctl,
	open:mxmisc_open,
	release:mxmisc_release,
};
static struct miscdevice mxmisc_dev = {
	MOXA_MISC_MINOR,
	"mxmisc",
	&mxmisc_fops
};

static void	release_mxmisc_resource(void)
{
#if defined (CONFIG_MACH_MOXA_UC8000)
	if( misc_base_addr )
		iounmap( (unsigned char*)misc_base_addr );
#elif defined (CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
	if( debug_led_addr )
		iounmap( (unsigned char*)debug_led_addr );
#endif
#if defined (CONFIG_MACH_MOXA_UC8480)
	if( igt_addr )
		iounmap( (unsigned char*)igt_addr );

	if ( misc_igt_base_res ) {
		release_resource(misc_igt_base_res);
		kfree(misc_igt_base_res);
	}
#endif
	if ( misc_base_res ) {
		release_resource(misc_base_res);
		kfree(misc_base_res);
	}
}
//-- end section -- reset to default & sw ready driver ------------------------------------

static void __exit mxmisc_exit(void)
{
	printk("Unregister Moxa misc" MISC_VERSION "\n");
	spin_lock(&resetlock);
	del_timer(&resettimer);
	spin_unlock(&resetlock);
	misc_deregister(&mxmisc_dev);
#if defined (CONFIG_MACH_MOXA_UC8000) || defined (CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
	#if defined (CONFIG_MACH_MOXA_UC8000) && !defined(CONFIG_MACH_MOXA_UC8406_NGC) && !defined(CONFIG_MACH_MOXA_UC8480)
	misc_deregister(&keypad_dev);
	spin_lock(&keypad_lock);
	if ( keypadtimer_on ) {
		del_timer(&keypadtimer);
		keypadtimer_on = 0;
	}
	spin_unlock(&keypad_lock);

	misc_deregister(&lcm_dev);
	#endif	// CONFIG_MACH_MOXA_UC8000

	release_mxmisc_resource();
#elif defined(CONFIG_MACH_MOXA_W406) || defined(CONFIG_MACH_MOXA_IA261)
	#if defined(CONFIG_MACH_MOXA_IA261)
	gpio_free(USER_LED1);
	gpio_free(DEBUG_LED_5);
	gpio_free(DEBUG_LED_4);
	gpio_free(DEBUG_LED_3);
	gpio_free(DEBUG_LED_2);
	gpio_free(DEBUG_LED_1);
	gpio_free(DEBUG_LED_0);
	#endif
	gpio_free(SW_READY_GPIO);
	gpio_free(SW_RESET_GPIO);
#endif
}

static int __init mxmisc_init(void)
{
	printk("Register Moxa misc ver" MISC_VERSION "\n");
	if ( misc_register(&mxmisc_dev) ) {
		printk("mxmisc register fail !\n");
		goto mxmisc_error;
	}

	// set the CPU for GPIO
#if defined(CONFIG_ARCH_IA240) || defined(CONFIG_ARCH_IA241) || defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345) || defined(CONFIG_ARCH_UC_7112_LX_PLUS) || defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W325) || defined(CONFIG_ARCH_W315)
	moxaart_gpio_mp_set(SW_READY_GPIO|SW_RESET_GPIO);
	// default set all GPIO for input/ouput
	moxaart_gpio_inout(SW_READY_GPIO, MCPU_GPIO_OUTPUT);
	moxaart_gpio_inout(SW_RESET_GPIO, MCPU_GPIO_INPUT);
	moxaart_gpio_set(SW_READY_GPIO, MCPU_GPIO_HIGH);
#elif defined(CONFIG_MACH_MOXA_W406) || defined(CONFIG_MACH_MOXA_IA261)
	int ret;

	ret=gpio_request(SW_READY_GPIO, "ep93xx_ready_gpio");
	if ( ret != 0 )
		goto out_SW_READY_GPIO;
	ret=gpio_request(SW_READY_GPIO, "ep93xx_reset_gpio");
	if ( ret != 0 )
		goto out_SW_RESET_GPIO;

	// default set all GPIO for input/ouput
	gpio_direction_output(SW_READY_GPIO, MCPU_GPIO_OUTPUT);
	gpio_direction_output(SW_RESET_GPIO, MCPU_GPIO_INPUT);
	gpio_set_value(SW_READY_GPIO, MCPU_GPIO_LOW);
#elif defined (CONFIG_MACH_MOXA_UC8000) 
	misc_base_res = request_mem_region(MISC_BASE_ADDR, MISC_BASE_REGION_SIZE, "MoxaMisc");
	if ( misc_base_res == NULL ) {
		printk("Moxa MISC request memory region fail !\n");
		release_mxmisc_resource();
		return -ENOMEM;
	}
	dbg_printk("misc_base_addr=0x%x\n", (unsigned int)misc_base_addr);
	misc_base_addr = ioremap(MISC_BASE_ADDR, MISC_BASE_REGION_SIZE);
	debug_led_addr = misc_base_addr + DEBUG_LED_ADDR;

	#if !defined(CONFIG_MACH_MOXA_UC8406_NGC)
	sw_ready_addr = misc_base_addr + SW_READY_ADDR;
	sw_reset_addr = misc_base_addr + SW_RESET_ADDR;

	keypad_addr = misc_base_addr + KEYPAD_ADDR;
        if ( misc_register(&keypad_dev) ) {
                printk("keypad register fail !\n");
		goto keypad_error;
        }
	init_timer(&keypadtimer);

	lcm_cmd0_addr = misc_base_addr + LCM_CMD0_ADDR;
	lcm_cmd1_addr = misc_base_addr + LCM_CMD1_ADDR;
	lcm_data_addr = misc_base_addr + LCM_DATA_ADDR;
	dbg_printk("lcm_cmd0_addr=0x%x\n", (unsigned int)lcm_cmd0_addr);
	dbg_printk("lcm_cmd1_addr=0x%x\n", (unsigned int)lcm_cmd1_addr);
	dbg_printk("lcm_data_addr=0x%x\n", (unsigned int)lcm_data_addr);
	if ( misc_register(&lcm_dev) ) {
                printk("LCM register fail !\n");
                goto lcm_error;
        }
	LCM_write_cmd(LCM_CMD_DISPLAY_START_LINE, LCM_CS1);
	LCM_write_cmd(LCM_CMD_DISPLAY_START_LINE, LCM_CS2);
	LCM_cls();

	#endif	// !defined(CONFIG_MACH_MOXA_UC8406_NGC)

#elif defined (CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
	gpio_line_config(SW_RESET_GPIO, IXP4XX_GPIO_IN);
	gpio_line_config(SW_READY_GPIO, IXP4XX_GPIO_OUT);

	misc_base_res = request_mem_region(DEBUG_LED_ADDR, 0x10, "MoxaMisc");

	if ( misc_base_res == NULL ) {
		printk("Moxa MISC request memory region fail !\n");
		misc_deregister(&mxmisc_dev);
		return -ENOMEM;
	}

	debug_led_addr = ioremap(DEBUG_LED_ADDR, 0x10);
	if ( !debug_led_addr ) {
		printk("%s[%d]: Fail to map DEBUG_LED_ADDR:%x\n",__FUNCTION__, __LINE__,debug_led_addr);
		release_mxmisc_resource();
		misc_deregister(&mxmisc_dev);
		return -ENOMEM;
	}
#endif

#if defined (CONFIG_MACH_MOXA_UC8480)
	misc_igt_base_res = request_mem_region(IGT_ADDR, 0x10, "MoxaMisc");

	if ( misc_igt_base_res == NULL ) {
		printk("Moxa MISC request memory region fail !\n");
		release_mxmisc_resource();
		misc_deregister(&mxmisc_dev);
		return -ENOMEM;
	}

	igt_addr = ioremap(IGT_ADDR, 0x01);
	if ( !igt_addr ) {
		printk("%s[%d]: Fail to map IGT_ADDR:%x\n",__FUNCTION__, __LINE__,igt_addr);
		release_mxmisc_resource();
		misc_deregister(&mxmisc_dev);
		return -ENOMEM;
	}
#endif

	// set the debug led output
#if defined(CONFIG_ARCH_IA240) || defined(CONFIG_ARCH_IA241)// mask by Victor Yu. 04-20-2007
	moxaart_gpio_inout(DEBUG_LED, MCPU_GPIO_OUTPUT);
#elif defined(CONFIG_MACH_MOXA_IA261)	// add by Victor Yu. 03-03-2008
	ret=gpio_request(DEBUG_LED_0, "ep93xx_debugled0");
	if ( ret != 0 )
		goto out_DEBUG_LED0;
	ret=gpio_request(DEBUG_LED_1, "ep93xx_debugled1");
	if ( ret != 0 )
		goto out_DEBUG_LED1;
	ret=gpio_request(DEBUG_LED_2, "ep93xx_debugled2");
	if ( ret != 0 )
		goto out_DEBUG_LED2;
	ret=gpio_request(DEBUG_LED_3, "ep93xx_debugled3");
	if ( ret != 0 )
		goto out_DEBUG_LED3;
	ret=gpio_request(DEBUG_LED_4, "ep93xx_debugled4");
	if ( ret != 0 )
		goto out_DEBUG_LED4;
	ret=gpio_request(DEBUG_LED_5, "ep93xx_debugled5");
	if ( ret != 0 )
		goto out_DEBUG_LED5;
	ret=gpio_request(USER_LED1, "ep93xx_user_led1");
	if ( ret != 0 )
		goto out_USER_LED1;
	ret=gpio_request(USER_LED2, "ep93xx_user_led2");
	if ( ret != 0 )
		goto out_USER_LED2;
	gpio_direction_output(DEBUG_LED_0, MCPU_GPIO_OUTPUT);
	gpio_direction_output(DEBUG_LED_1, MCPU_GPIO_OUTPUT);
	gpio_direction_output(DEBUG_LED_2, MCPU_GPIO_OUTPUT);
	gpio_direction_output(DEBUG_LED_3, MCPU_GPIO_OUTPUT);
	gpio_direction_output(DEBUG_LED_4, MCPU_GPIO_OUTPUT);
	gpio_direction_output(DEBUG_LED_5, MCPU_GPIO_OUTPUT);
	gpio_direction_output(USER_LED1, MCPU_GPIO_OUTPUT);
	gpio_direction_output(USER_LED2, MCPU_GPIO_OUTPUT);
#endif

	// initialize the reset polling
	INIT_WORK(&resetqueue, settodefault);
	spin_lock_init(&resetlock);
	spin_lock(&resetlock);
	endresettime = 0;
	ledonoffflag = 1;
	init_timer(&resettimer);
	resettimer.function = reset_poll;
	resettimer.expires = jiffies + RESET_POLL_TIME;
	add_timer(&resettimer);
	spin_unlock(&resetlock);

	printk("OK.\n");
	return 0;

#if defined(CONFIG_MACH_MOXA_UC8000) && !defined(CONFIG_MACH_MOXA_UC8406_NGC)
lcm_error:
	misc_deregister(&keypad_dev);

keypad_error:
	misc_deregister(&mxmisc_dev);
	release_mxmisc_resource();

#elif defined(CONFIG_MACH_MOXA_IA261) || defined(CONFIG_MACH_MOXA_W406)	// add by Victor Yu. 03-03-2008

#if defined(CONFIG_MACH_MOXA_IA261)
out_USER_LED2:
	gpio_free(USER_LED1);
out_USER_LED1:
	gpio_free(DEBUG_LED_5);
out_DEBUG_LED5:
	gpio_free(DEBUG_LED_4);
out_DEBUG_LED4:
	gpio_free(DEBUG_LED_3);
out_DEBUG_LED3:
	gpio_free(DEBUG_LED_2);
out_DEBUG_LED2:
	gpio_free(DEBUG_LED_1);
out_DEBUG_LED1:
	gpio_free(DEBUG_LED_0);
#endif
out_DEBUG_LED0:
	gpio_free(SW_READY_GPIO);
out_SW_READY_GPIO:
	gpio_free(SW_RESET_GPIO);
out_SW_RESET_GPIO:

#endif

mxmisc_error:
	return -EIO;
}

module_init(mxmisc_init);
module_exit(mxmisc_exit);

MODULE_AUTHOR("Victor Yu");
MODULE_LICENSE("GPL");
