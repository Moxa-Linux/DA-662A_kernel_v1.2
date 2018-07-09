/*
 * This is Moxa DA-66X device LED/ID/LCM/Keypad driver.
 * It is from misc interface. So the device node major number is 10.
 * The device node minor number is following:
 * led	;	100
 * id	:	101
 * lcm	:	102
 * keypad:	103
 * pio:		104
 * 
 * There devices are mapping system memory is following:
 * led	:	0x4c000000	write only, CS2
 * id	:	0x4c000000	read only, CS2
 * lcm	:	0x50000000	write only, CS3
 *	:	0x54000000	write only, CS4
 *	:	0x58000000	read only, CS5
 * keypad:	0x54000000	read only, CS4
 *
 * History:
 * Date		Aurhor			Comment
 * 11-27-2003	Victor Yu.		Create it.
 * 05-26-2004	Victor Yu.		Add to support UC7220 product model.
 * 					Change version from 1.0 to 1.1.
 * 10-11-2004	Victor Yu.		Add to support software reset button. Read it from 
 * 					keypad register bit5. 1 for pressed. 0 for released.
 * 01-13-2005	Victor Yu.		Add to support UC7408 & UC7420_INV with DIO.
 * 					change ther version from 1.1 to 1.2
 * 06-08-2007	Jared Wu.		get target_software_id at moxa_misc_init()
 *					Integrate lcm driver to executable both on UC/DA. 
 *					Register file_operations dependent on software_id at boot time.
 * 03-06-2015	Victor Yu.		We change CPU to Moxa Macro. And the model name is changed to
 *					DA662A. We use the GPIO14 to identifile 8 ports or 16 ports.
 *					High is 16 ports, low is 8 ports.
 *					Remove UC7420 LCM device driver.
 * 03-10-2015	Victor Yu.		Add to use GPIO2 for S/W reset button on DA662A.
 */
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
#include <linux/proc_fs.h>
#ifdef CONFIG_ARCH_MOXAMACRO
#include <linux/gpio.h>
#include <linux/pci.h>
#include <linux/sched.h>
#define SW_RESET_GPIO	2
#endif
#include <linux/mtd/mtd.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <mach/system-control.h>
#include "id.h"

#define MOXA_MISC_VERSION	"1.4"

#define LED_ID_ADDR		0x4c000000	// CS2
#define LCM_CMD0_ADDR		0x50000000	// CS3
#define LCM_CMD1_ADDR		0x54000000	// CS4
#define KEYPAD_ADDR		0x54000000	// CS4
#define LCM_DATA_ADDR		0x58000000	// CS5
#define REGION_SIZE		0x10

#define MOXA_LED_MINOR		100
#define MOXA_ID_MINOR		101
#define MOXA_LCM_MINOR		102
#define MOXA_KEYPAD_MINOR	103
#define MOXA_SW_READY_MINOR	104

//#define _DEBUG_PRINT  // Add by Jared for debugging the LCM driver. 06-12-2006.
#ifdef _DEBUG_PRINT
    #define DBG_PRINT(x...)       printk(x)
#else
    #define DBG_PRINT(x...)
#endif

static struct resource	*led_id_resource;	// use 0x52000000
static struct resource	*lcm_cmd0_resource;	// use 0x53000000
static struct resource	*lcm_cmd1_resource;	// use 0x54000000
static struct resource	*lcm_data_resource;	// use 0x55000000

static volatile unsigned char	*led_id_addr;
static volatile unsigned char	*lcm_cmd0_addr;
static volatile unsigned char 	*lcm_cmd1_addr;
static volatile unsigned char	*lcm_data_addr;
static volatile unsigned char	*keypad_addr;

static spinlock_t		led_id_lock= SPIN_LOCK_UNLOCKED;
static spinlock_t               keypad_lock= SPIN_LOCK_UNLOCKED;

#define KEYPAD_KEY4		(1<<4)
//
// LED file operation function call
//
#define IOCTL_LED_OUTPUT	1
static long led_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch ( cmd ) {
	case IOCTL_LED_OUTPUT :
	{
		unsigned char	v;
		
		if ( copy_from_user(&v, (void *)arg, 1) )
			return -EFAULT;
		spin_lock(&led_id_lock);
		*led_id_addr = ~v;
		spin_unlock(&led_id_lock);
		break;
	}
	default:
		return -EINVAL;
	}
	return 0;
}

static int led_open(struct inode *inode, struct file *file)
{
	if ( MINOR(inode->i_rdev) == MOXA_LED_MINOR )
		return 0;
	return -ENODEV;
}

static int led_release(struct inode *inode, struct file *file)
{
	return 0;
}

static struct file_operations led_fops = {
	.owner		= THIS_MODULE,
	.llseek		= NULL,
	.unlocked_ioctl	= &led_ioctl,
	.open		= &led_open,
	.release	= &led_release,
};
static struct miscdevice led_dev = {
	MOXA_LED_MINOR,
	"led",
	&led_fops
};

//
// ID file operation function call
//
static unsigned long rc7000_software_id;
static struct proc_dir_entry *proc_swid;
static struct proc_dir_entry *proc_serid;
static struct proc_dir_entry *proc_isolation;

ssize_t proc_swid_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
	return sprintf(buffer, "%x\n", (unsigned int)rc7000_software_id);
}

ssize_t proc_isolation_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
	return sprintf(buffer, "%d\n", *keypad_addr & KEYPAD_KEY4); // zero has isolation
}

#define ENV_SIZE	0x4000
/*
 * s1 is either a simple 'name', or a 'name=value' pair.
 * s2 is a 'name=value' pair.
 * If the names match, return the value of s2, else NULL.
 */

static char *envmatch (char * s1, char * s2)
{

        while (*s1 == *s2++)
                if (*s1++ == '=')
                        return s2;
        if (*s1 == '\0' && *(s2 - 1) == '=')
                return s2;
        return NULL;
}

ssize_t proc_serid_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
	struct mtd_info	*mtd;
	char		*buf, *env, *nxt, *val=NULL;
	int		len;

	*eof = 1;
	if ( offset )
		return 0;
	buf = kmalloc(ENV_SIZE, GFP_KERNEL);
	if ( buf == NULL )
		return 0;
	mtd = get_mtd_device(NULL, 0);
	if ( mtd ) {
		mtd->read(mtd, 0xC0000, ENV_SIZE, &len, buf);
		for ( env=&buf[sizeof(u32)]; *env; env=nxt+1 ) {
			for ( nxt=env; *nxt; ++nxt ) {
				if ( nxt >= &buf[ENV_SIZE] ) {
					kfree(buf);
					return 0;
				}
			}
			val = envmatch ("serialnumber", env);
			if ( val )
				break;
		}
	}
	if ( val ) {
		kfree(buf);
		return sprintf(buffer, "%s\n", val);
	}
	kfree(buf);
	return 0;
}

#define IOCTL_HARDWARE_ID_GET		100
#define IOCTL_SOFTWARE_ID_GET		200
static long id_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch ( cmd ) {
	case IOCTL_HARDWARE_ID_GET :
	{
		unsigned char	v;
		
		spin_lock(&led_id_lock);
		v = *led_id_addr;
		spin_unlock(&led_id_lock);
		if ( copy_to_user((void *)arg, &v, 1) )
			return -EFAULT;
		break;
	}
	case IOCTL_SOFTWARE_ID_GET :
		if ( copy_to_user((void *)arg, &rc7000_software_id, sizeof(u32)) )
		return -EFAULT;
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int id_open(struct inode *inode, struct file *file)
{
	if ( MINOR(inode->i_rdev) == MOXA_ID_MINOR )
		return 0;
	return -ENODEV;
}

static int id_release(struct inode *inode, struct file *file)
{
	return 0;
}

static struct file_operations id_fops = {
	owner:THIS_MODULE,
	llseek:NULL,
	unlocked_ioctl:id_ioctl,
	open:id_open,
	release:id_release,
};
static struct miscdevice id_dev = {
	MOXA_ID_MINOR,
	"id",
	&id_fops
};

//
// keypad file operation function call
//
#define IOCTL_KEYPAD_BASE	0x100
#define IOCTL_KEYPAD_HAS_PRESS	(1+IOCTL_KEYPAD_BASE)
#define IOCTL_KEYPAD_GET_KEY	(2+IOCTL_KEYPAD_BASE)
#define IOCTL_KEYPAD_SOUND_SET	(3+IOCTL_KEYPAD_BASE)

#define IOCTL_GET_SW_RESET_BUTTON	0x100	// add by Victor Yu. 10-11-2004

#define KEY0			0
#define KEY1			1
#define KEY2			2
#define KEY3			3
#define KEY4			4

#define KEYPAD_BUFFER_LENGTH	32
#define KEYPAD_BUFFER_MASK	(KEYPAD_BUFFER_LENGTH-1)
#define KEYPAD_KEY0		(1<<0)
#define KEYPAD_KEY1		(1<<1)
#define KEYPAD_KEY2		(1<<2)
#define KEYPAD_KEY3		(1<<3)
#if 0	// masked by Victo rYU. 2015/11/20
#define KEYPAD_MASK		0x1f
#else
#define KEYPAD_MASK		0x0f	// because the key4 is used to check it is ioslation or not
#endif
#define KEYPAD_POLL_TIME	((HZ/100)*2) // too slow will lose checking, 2015/11/20 Victor Yu.

#define SW_RESET_BUTTON_PRESS	0x20	// add by Victor Yu. 10-11-2004

#define KEYPAD_SOUND_TIME	30
#define KEYPAD_KEY0_FREQ	1500
#define KEYPAD_KEY1_FREQ	1600
#define KEYPAD_KEY2_FREQ	1700
#define KEYPAD_KEY3_FREQ	1800
#define KEYPAD_KEY4_FREQ	1900

#define KEYPAD_KEY0_SOUND	((KEYPAD_SOUND_TIME<<16)|KEYPAD_KEY0_FREQ)
#define KEYPAD_KEY1_SOUND	((KEYPAD_SOUND_TIME<<16)|KEYPAD_KEY1_FREQ)
#define KEYPAD_KEY2_SOUND	((KEYPAD_SOUND_TIME<<16)|KEYPAD_KEY2_FREQ)
#define KEYPAD_KEY3_SOUND	((KEYPAD_SOUND_TIME<<16)|KEYPAD_KEY3_FREQ)
#define KEYPAD_KEY4_SOUND	((KEYPAD_SOUND_TIME<<16)|KEYPAD_KEY4_FREQ)

static	int		keypad_wptr=0, keypad_rptr=0;
static 	unsigned char	keypad_buffer[KEYPAD_BUFFER_LENGTH];
static	unsigned char	keypadold;
static	struct timer_list keypadtimer;
static	int 		keypadtimer_on=0;
static	int		keypad_opened=0;

static void keypad_poll(unsigned long ingore)
{
	unsigned char	v, v1;
	
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
#if 0	// masked by Victor Yu. 2015/11/20, the key4 is used to check it is isolation or not
			} else if ( !(v & KEYPAD_KEY4) && (v1 & KEYPAD_KEY4) ) {
				keypad_buffer[keypad_wptr++] = KEY4;
#endif
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

static long keypad_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int	v;
	
	switch ( cmd ) {
	case IOCTL_KEYPAD_HAS_PRESS :		
		spin_lock(&keypad_lock);
		v = (keypad_wptr - keypad_rptr) & KEYPAD_BUFFER_MASK;
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
	case IOCTL_KEYPAD_SOUND_SET :
		if ( copy_from_user(&v, (void *)arg, sizeof(int)) )
			return -EFAULT;
		if ( v != 1 && v != 0 )
			return -EINVAL;
		/* masked by Victor Yu. 2015/4/24, don't use
		spin_lock(&keypad_lock);
		spin_unlock(&keypad_lock);
		*/
		break;
	// following add by Victor Yu. 10-11-2004
	case IOCTL_GET_SW_RESET_BUTTON :
		spin_lock(&keypad_lock);
		if ( *keypad_addr & SW_RESET_BUTTON_PRESS )
			v = 1;
		else
			v = 0;
		spin_unlock(&keypad_lock);
		if ( copy_to_user((void *)arg, &v, sizeof(int)) )
			return -EFAULT;
		break;
	// above add by Victor Yu. 10-11-2004
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
	unlocked_ioctl:keypad_ioctl,
	open:keypad_open,
	release:keypad_release,
};
static struct miscdevice keypad_dev = {
	MOXA_KEYPAD_MINOR,
	"keypad",
	&keypad_fops
};

// --------------------------------------------------------
// The software ready driver. Modify by Jared 06-12-2006.
// --------------------------------------------------------
// following on LCM register 1
#define OCTL_LCM_BASE		0x100
#define SW_READY_LED_ON		0x00
#define SW_READY_LED_OFF	0x08
#define FAULT_LED_ON		0x00
#define FAULT_LED_OFF		0x04

// ioctl command define
#define IOCTL_LCM_BASE			0x100
#define IOCTL_SW_READY_LED_ON		(11+IOCTL_LCM_BASE)
#define IOCTL_SW_READY_LED_OFF		(12+IOCTL_LCM_BASE)
#define IOCTL_FAULT_LED_ON		(100+IOCTL_LCM_BASE)
#define IOCTL_FAULT_LED_OFF		(101+IOCTL_LCM_BASE)

#define BITMAP_LCM_CMD1_BL      0x20
static unsigned char	lcm_ctrl=SW_READY_LED_OFF|BITMAP_LCM_CMD1_BL;
static long sw_ready_ioctl(struct file *file, unsigned int cmd, unsigned long arg)	  
{
	spin_lock(&keypad_lock);
	switch ( cmd ) {
	case IOCTL_SW_READY_LED_ON :
		lcm_ctrl &= ~SW_READY_LED_OFF;
		*((unsigned char*)lcm_cmd1_addr) = lcm_ctrl;
		break;
	case IOCTL_SW_READY_LED_OFF :
		lcm_ctrl |= SW_READY_LED_OFF;
		*((unsigned char*)lcm_cmd1_addr) = lcm_ctrl;
		break;
	case IOCTL_FAULT_LED_ON :
		*((unsigned char*)lcm_cmd0_addr) = FAULT_LED_ON;
		break;
	case IOCTL_FAULT_LED_OFF :
		*((unsigned char*)lcm_cmd0_addr) = FAULT_LED_OFF;
		break;
	default:
		return -EINVAL;
	}
	spin_unlock(&keypad_lock);
	return 0;
}

static int sw_ready_open(struct inode *inode, struct file *file)
{
	if ( MINOR(inode->i_rdev) == MOXA_SW_READY_MINOR )
		return 0;

	return -ENODEV;
}

static int sw_ready_release(struct inode *inode, struct file *file)
{
	return 0;
}

static struct file_operations sw_ready_fops = {
	owner:THIS_MODULE,
	unlocked_ioctl:sw_ready_ioctl,
	open:sw_ready_open,
	release:sw_ready_release,
};
static struct miscdevice sw_ready_dev = {
	MOXA_SW_READY_MINOR,
	"sw_ready",
	&sw_ready_fops
};

// ----------------------------------------------------------------
// DA66X LCM operation function call. Modify by Jared 06-12-2006.
// ----------------------------------------------------------------
// Define I/O control commands
#define IOCTL_LCM_INIT          1001
#define IOCTL_LCM_PUT_CHAR      1005

// Define I/O control commands
#define IOCTL_LCM_GOTO_XY		(1+IOCTL_LCM_BASE)
#define IOCTL_LCM_CLS			(2+IOCTL_LCM_BASE)
#define IOCTL_LCM_CLEAN_LINE		(3+IOCTL_LCM_BASE)
#define IOCTL_LCM_GET_XY		(4+IOCTL_LCM_BASE)
#define IOCTL_LCM_BACK_LIGHT_ON		(5+IOCTL_LCM_BASE)
#define IOCTL_LCM_BACK_LIGHT_OFF	(6+IOCTL_LCM_BASE)
#define IOCTL_LCM_PIX_ON		(7+IOCTL_LCM_BASE)
#define IOCTL_LCM_PIX_OFF		(8+IOCTL_LCM_BASE)
#define IOCTL_LCM_CURSOR_ON		(9+IOCTL_LCM_BASE)
#define IOCTL_LCM_CURSOR_OFF		(10+IOCTL_LCM_BASE)
#define IOCTL_LCM_AUTO_SCROLL_ON	(13+IOCTL_LCM_BASE)
#define IOCTL_LCM_AUTO_SCROLL_OFF	(14+IOCTL_LCM_BASE)
#define IOCTL_LCM_BLINK_ON		(15+IOCTL_LCM_BASE)  // DA-66X only
#define IOCTL_LCM_BLINK_OFF		(16+IOCTL_LCM_BASE)  // DA-66X only
#define IOCTL_SET_DOUT			(15+IOCTL_LCM_BASE)  // UC-7420/7410 only
#define IOCTL_GET_DOUT			(16+IOCTL_LCM_BASE)  // UC-7420/7410 only
#define IOCTL_GET_DIN			(17+IOCTL_LCM_BASE)    // UC-7420/7410 only

// BITMAP OF LCM_CMD1
#define BITMAP_LCM_CMD1_RS      0x01
#define BITMAP_LCM_CMD1_RW      0x02
#define BITMAP_LCM_CMD1_E       0x04
#define BITMAP_LCM_CMD1_SWREADY 0x08
#define BITMAP_LCM_CMD1_D_I     0x10
#define BITMAP_LCM_CMD1_373WEN  0x40

// LCM Operations
#define LCM_OP_WRITE_CMD        0
#define LCM_OP_READ_BUSY_FLAG   (BITMAP_LCM_CMD1_RW)
#define LCM_OP_WRITE_DATA       (BITMAP_LCM_CMD1_RS)
#define LCM_OP_READ_DATA        (BITMAP_LCM_CMD1_RS|BITMAP_LCM_CMD1_RW)

// LCM commands
#define LCM_CMD_CLEAR_DISPLAY       0x01
#define LCM_CMD_RETURN_HOME         0x02
#define LCM_CMD_ENTRY_MODE_SET      0x04
#define LCM_CMD_DISPLAY_ON_OFF      0x08
#define LCM_CMD_CURSOR_DISP_SHIFT   0x10
#define LCM_CMD_FUNC_SET            0x20
#define LCM_CMD_SET_CG_RAM_ADDR     0x40
#define LCM_CMD_SET_DD_RAM_ADDR     0x80

// LCM command/status sub code
#define BITMAP_CODE_BUSY_FLAG       0x80
#define BITMAP_CODE_INSC            0x02
#define BITMAP_CODE_SHIFT           0x01 
#define BITMAP_CODE_DISPLAY_ON      0x04
#define BITMAP_CODE_CURSOR_ON       0x02
#define BITMAP_CODE_BLINK_ON        0x01
#define BITMAP_CODE_DISP_SHIFT      0x08
#define BITMAP_CODE_SHIFT_TO_RIGHT  0x04
#define BITMAP_CODE_DATA_LENGTH_8   0x10
#define BITMAP_CODE_TWO_LINES       0x08
#define BITMAP_CODE_FONT_SZ_5_10    0x04

#define NEW_CHAR_SIZE 24
static int g_lcm_dislay_mode=-1;

static unsigned char	lcm_newchars[NEW_CHAR_SIZE]={
	0x04, 0x0E, 0x15, 0x04, 0x04, 0x04, 0x04, 0,
	0x04, 0x04, 0x04, 0x04, 0x15, 0x0E, 0x04, 0,
	0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0,
};
				  
typedef struct lcm_xy {
	int x, y;
}lcm_xy_t;
static int	lcm_x, lcm_y;

static unsigned char sds_lcm_read_op(unsigned char op)
{
	unsigned char	reg_val=0, data;

	if (op!=LCM_OP_READ_BUSY_FLAG && op!=LCM_OP_READ_DATA)
		return -1;

	spin_lock(&keypad_lock);
   
	// Read original LCM_CMD1 value.
	//reg_val = readb(lcm_cmd1_addr);
	//DBG_PRINT("sds_lcm_read_op> reg_val1=0x%02X\n", reg_val);
	//reg_val |= BITMAP_LCM_CMD1_BL;

	// Set the new LCM_CMD1 value.
	reg_val&=~(LCM_OP_READ_BUSY_FLAG | LCM_OP_READ_DATA );
	reg_val |= op;
	reg_val |= BITMAP_LCM_CMD1_373WEN;
	reg_val |= lcm_ctrl;
	//reg_val = ( lcm_ctrl & SW_READY_LED_OFF ) ? ( reg_val | SW_READY_LED_OFF ) : ( reg_val & (~SW_READY_LED_OFF) );
	DBG_PRINT("sds_lcm_read_op> reg_val2=0x%02X\n", reg_val);
	writeb(reg_val, lcm_cmd1_addr);
	readb(lcm_cmd1_addr);
	udelay(1);

	//Raise Enable bit.
	reg_val|=BITMAP_LCM_CMD1_E;
	reg_val |= lcm_ctrl;
	//reg_val = ( lcm_ctrl & SW_READY_LED_OFF ) ? ( reg_val | SW_READY_LED_OFF ) : ( reg_val & (~SW_READY_LED_OFF) );
	DBG_PRINT("sds_lcm_read_op> reg_val3=0x%02X\n", reg_val);
	writeb(reg_val, lcm_cmd1_addr);
	readb(lcm_cmd1_addr);
	udelay(1);

	// Read data from LCM_DATA
	data = readb(lcm_data_addr);
	DBG_PRINT("sds_lcm_read_op> data1=0x%02X\n", data);
    
	// Pull down Enable bit.
	reg_val&=~BITMAP_LCM_CMD1_E;
	reg_val |= lcm_ctrl;
	//reg_val = ( lcm_ctrl & SW_READY_LED_OFF ) ? ( reg_val | SW_READY_LED_OFF ) : ( reg_val & (~SW_READY_LED_OFF) );
	DBG_PRINT("sds_lcm_read_op> reg_val4=0x%02X\n", reg_val);
	writeb(reg_val, lcm_cmd1_addr);
	readb(lcm_cmd1_addr);

	spin_unlock(&keypad_lock);
	return data;
}

#define BUSY_LOOP	129
int sds_lcm_write_op(int chk_busy_flag, unsigned char op, char data)
{
	unsigned char	reg_val=0;
	int		cnt;

	if (op!=LCM_OP_WRITE_CMD && op!=LCM_OP_WRITE_DATA)
		return -1;

	spin_lock(&keypad_lock);
	DBG_PRINT("sds_lcm_write_op> data=0x%02X\n", data);

	// Make sure busy flag is clear.
	if (chk_busy_flag) {
		DBG_PRINT("LCM_OP_READ_BUSY_FLAG> enter1\n");
		spin_unlock(&keypad_lock);
		cnt = 0;
		while (sds_lcm_read_op(LCM_OP_READ_BUSY_FLAG) & BITMAP_CODE_BUSY_FLAG) {
			udelay(1);
			if ( ++cnt >= BUSY_LOOP )
				break;
		}
		spin_lock(&keypad_lock);
		DBG_PRINT("LCM_OP_READ_BUSY_FLAG> exit1\n");
	}

	// Read original LCM_CMD1 value.
	//reg_val = readb(lcm_cmd1_addr);
	//DBG_PRINT("sds_lcm_write_op> reg_val1=0x%02X\n", reg_val);
	//reg_val |= BITMAP_LCM_CMD1_BL;

	// Set the new LCM_CMD1 value.
	reg_val&=~(LCM_OP_WRITE_CMD | LCM_OP_WRITE_DATA);
	reg_val|=op;
	reg_val&=~(BITMAP_LCM_CMD1_RW | BITMAP_LCM_CMD1_373WEN);
	reg_val |= lcm_ctrl;
	//reg_val = ( lcm_ctrl & SW_READY_LED_OFF ) ? ( reg_val | SW_READY_LED_OFF ) : ( reg_val & (~SW_READY_LED_OFF) );
	DBG_PRINT("sds_lcm_write_op> reg_val2=0x%02X\n", reg_val);
	writeb(reg_val, lcm_cmd1_addr);
	readb(lcm_cmd1_addr);
	udelay(1);

	// Raise Enable bit.
	reg_val |= BITMAP_LCM_CMD1_E;
	reg_val |= lcm_ctrl;
	//reg_val = ( lcm_ctrl & SW_READY_LED_OFF ) ? ( reg_val | SW_READY_LED_OFF ) : ( reg_val & (~SW_READY_LED_OFF) );
	DBG_PRINT("sds_lcm_write_op> reg_val3=0x%02X\n", reg_val);
	writeb(reg_val, lcm_cmd1_addr);
	readb(lcm_cmd1_addr);
	udelay(1);

	// Write data to LCM_DATA
	writeb((unsigned char)data, lcm_data_addr);

	// Pull down Enable bit.
	reg_val&=~BITMAP_LCM_CMD1_E;
	reg_val |= lcm_ctrl;
	//reg_val = ( lcm_ctrl & SW_READY_LED_OFF ) ? ( reg_val | SW_READY_LED_OFF ) : ( reg_val & (~SW_READY_LED_OFF) );
	DBG_PRINT("sds_lcm_write_op> reg_val4=0x%02X\n", reg_val);
	writeb(reg_val, lcm_cmd1_addr);
	readb(lcm_cmd1_addr);

	// Set BITMAP_LCM_CMD1_RW and 373wen to 1.
	reg_val|=(BITMAP_LCM_CMD1_RW | BITMAP_LCM_CMD1_373WEN);
	reg_val |= lcm_ctrl;
	//reg_val = ( lcm_ctrl & SW_READY_LED_OFF ) ? ( reg_val | SW_READY_LED_OFF ) : ( reg_val & (~SW_READY_LED_OFF) );
	DBG_PRINT("sds_lcm_write_op> reg_val5=0x%02X\n", reg_val);
	writeb(reg_val, lcm_cmd1_addr);
	readb(lcm_cmd1_addr);

	// Make sure busy flag is clear.
	spin_unlock(&keypad_lock);
	if (chk_busy_flag) {
		DBG_PRINT("LCM_OP_READ_BUSY_FLAG> enter2\n");
		cnt = 0;
		while (sds_lcm_read_op(LCM_OP_READ_BUSY_FLAG) & BITMAP_CODE_BUSY_FLAG) {
			udelay(1);
			if ( ++cnt >= BUSY_LOOP )
				break;
		}
		DBG_PRINT("LCM_OP_READ_BUSY_FLAG> exit2\n");
	}
	return 0;
}

// line_mode=0: one line
// line_mode=1: two lines
int sds_lcm_init_module(unsigned char line_mode)
{
	int	i, ret;
	
	if (line_mode!=0 && line_mode!=1)
		return -1;
	else
		g_lcm_dislay_mode=line_mode;

	lcm_x= lcm_y= 0;

	sds_lcm_write_op(0, LCM_OP_WRITE_CMD, LCM_CMD_FUNC_SET | BITMAP_CODE_DATA_LENGTH_8);
	mdelay(5);
	sds_lcm_write_op(0, LCM_OP_WRITE_CMD, LCM_CMD_FUNC_SET | BITMAP_CODE_DATA_LENGTH_8);
	udelay(200);
	sds_lcm_write_op(0, LCM_OP_WRITE_CMD, LCM_CMD_FUNC_SET | BITMAP_CODE_DATA_LENGTH_8);
	udelay(200);

	if (line_mode==0)
		sds_lcm_write_op(1, LCM_OP_WRITE_CMD, LCM_CMD_FUNC_SET | BITMAP_CODE_DATA_LENGTH_8);
	else
		sds_lcm_write_op(1, LCM_OP_WRITE_CMD, LCM_CMD_FUNC_SET | BITMAP_CODE_DATA_LENGTH_8 | BITMAP_CODE_TWO_LINES);

	sds_lcm_write_op(1, LCM_OP_WRITE_CMD, LCM_CMD_DISPLAY_ON_OFF); // display OFF
	sds_lcm_write_op(1, LCM_OP_WRITE_CMD, LCM_CMD_CLEAR_DISPLAY);  // display CLEAR
	sds_lcm_write_op(1, LCM_OP_WRITE_CMD, LCM_CMD_ENTRY_MODE_SET | BITMAP_CODE_INSC);  // Entry mode set: increase mode
	sds_lcm_write_op(1, LCM_OP_WRITE_CMD, LCM_CMD_DISPLAY_ON_OFF | BITMAP_CODE_DISPLAY_ON); // display ON
    
	// Set new data
	sds_lcm_write_op(1, LCM_OP_WRITE_CMD, LCM_CMD_SET_CG_RAM_ADDR|0x10 );// set CGRAM address
	for( i=0;i<NEW_CHAR_SIZE;i++) {
		DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_PUT_CHAR ch=%c\n", lcm_newchars[i]);
		ret=sds_lcm_write_op(1, LCM_OP_WRITE_DATA, lcm_newchars[i]);
		if (ret!=0) {
			DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_PUT_CHAR fail\n\n");
			return -EFAULT;
		}
	}
    
	return 0;
}

#if 1	// added by Victor Yu. 2015/7/15
static unsigned char	default_val=LCM_CMD_DISPLAY_ON_OFF | BITMAP_CODE_DISPLAY_ON;
#endif
static long sds_lcm_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int		ret, nIndex;
	unsigned char	nStart, nEnd, val8;
	lcm_xy_t	xy;
    
#if 0	// masked by Victor Yu. 2015/7/15
	default_val = LCM_CMD_DISPLAY_ON_OFF | BITMAP_CODE_DISPLAY_ON | BITMAP_CODE_BLINK_ON;
#endif

	switch (cmd) {
        case IOCTL_LCM_INIT:
            DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_INIT enter\n");
      	    if (copy_from_user(&val8, (unsigned char*)arg, sizeof(unsigned char)))
    	        return -EFAULT;

            ret=sds_lcm_init_module(val8);
            if (ret!=0)
            {
                DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_INIT fail\n\n");
                return -EFAULT;
            }
    	    break;

        case IOCTL_LCM_CLS:
            DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_CLS enter\n");
            ret=sds_lcm_write_op(1, LCM_OP_WRITE_CMD, LCM_CMD_CLEAR_DISPLAY);
	    msleep(1);// sleep some time to wait LCM ready
            if (ret!=0)
            {
                DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_CLS fail\n\n");
                return -EFAULT;
            }
    	    break;
    	  
    	  case IOCTL_LCM_CLEAN_LINE :
    	  	// not implement yet! 
    	  	DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_CLEAN_LINE enter\n");
    	  	
    	  	// get current position
    	  	val8 = sds_lcm_read_op(LCM_OP_READ_BUSY_FLAG)& 0x7F;
    	  	
    	  	if( val8 >= 0x40 && val8 <= 0x67 ) {
    	  		nStart=0x40;
    	  		nEnd=0x67;
   	  	} else {
    	  		nStart=0;
    	  		nEnd=0x27;    	  		
   	  	}
   	  	  
   	  	// Goto the head of this line
 	  	ret=sds_lcm_write_op(1, LCM_OP_WRITE_CMD, LCM_CMD_SET_DD_RAM_ADDR | nStart);
		if (ret!=0) {
			DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_CLEAN_LINE fail 2\n");
			return -EFAULT;
		}

   	  	// Clear this line
 	  	for( nIndex=nStart ; nIndex <= nEnd; nIndex++ ) {
 	  	      ret=sds_lcm_write_op(1, LCM_OP_WRITE_DATA, ' ');
 	  	      if (ret!=0) {
				DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_CLEAN_LINE fail\n");
				return -EFAULT;
			}
 	  	}
 	  	    
 	  	// Back to the head of this line
 	  	ret=sds_lcm_write_op(1, LCM_OP_WRITE_CMD, LCM_CMD_SET_DD_RAM_ADDR | nStart);
		if (ret!=0) {
			DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_CLEAN_LINE fail 2\n");
			return -EFAULT;
		}
    	  	break;

        case IOCTL_LCM_GOTO_XY:
            DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_GOTO_XY enter\n");
      	    if (copy_from_user(&xy, (unsigned char*)arg, sizeof(lcm_xy_t)))
    	        return -EFAULT;

    	      if( xy.y == 0)
    	      	val8=xy.x;
    	      else
    	        val8=xy.x+0x40;

            if ((g_lcm_dislay_mode==0 && val8 > 0x4F) ||
                (g_lcm_dislay_mode==1 && ((val8 > 0x27 && val8 < 0x40) || val8 > 0x67)))
                return -EFAULT;

            ret=sds_lcm_write_op(1, LCM_OP_WRITE_CMD, LCM_CMD_SET_DD_RAM_ADDR | val8);
            if (ret!=0)
            {
                DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_GOTO_XY fail\n");
                return -EFAULT;
            }
    	    break;
    	  case IOCTL_LCM_GET_XY:
    	      DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_GET_XY enter\n");
    	      val8=sds_lcm_read_op(LCM_OP_READ_BUSY_FLAG);
    	      
    	     	if( val8 >= 0x40 && val8 <= 0x67 ) {
    	     		xy.x = val8 - 0x40;
    	     		xy.y = 1;
    	     	}
    	     	else {
    	     		xy.x = val8;
    	     		xy.y = 0;
    	     	}
    	      if ( copy_to_user((void *)arg, &xy, sizeof(lcm_xy_t) ) )
    	      	return -EFAULT;
    	    break;

        case IOCTL_LCM_CURSOR_ON:
            DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_CURSOR_ON enter\n");
	    default_val |= (BITMAP_CODE_CURSOR_ON);
            ret=sds_lcm_write_op(1, LCM_OP_WRITE_CMD, default_val);
            if (ret!=0)
            {
                DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_CURSOR_ON fail\n");
                return -EFAULT;
            }
    	    break;
    	    
        case IOCTL_LCM_CURSOR_OFF:
            DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_CURSOR_OFF enter\n");
#if 0	// masked by Victor Yu. 2015/7/15
            ret=sds_lcm_write_op(1, LCM_OP_WRITE_CMD, (default_val & (~BITMAP_CODE_CURSOR_ON)) );
#else
	    default_val &= ~BITMAP_CODE_CURSOR_ON;
            ret=sds_lcm_write_op(1, LCM_OP_WRITE_CMD, default_val);
#endif
            if (ret!=0)
            {
                DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_CURSOR_OFF fail\n");
                return -EFAULT;
            }
    	    break;
	case IOCTL_LCM_BLINK_ON:
	    DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_BLINK_ON enter\n");
	    default_val |= BITMAP_CODE_BLINK_ON;
            ret=sds_lcm_write_op(1, LCM_OP_WRITE_CMD, default_val );
            if (ret!=0)
            {
                DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_BLINK_ON fail\n");
                return -EFAULT;
            }
    	    break;
	case IOCTL_LCM_BLINK_OFF:
            DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_BLINK_OFF enter\n");
	    default_val &= ~BITMAP_CODE_BLINK_ON;
            ret=sds_lcm_write_op(1, LCM_OP_WRITE_CMD, default_val);
            if (ret!=0)
            {
                DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_BLIND_OFF fail\n");
                return -EFAULT;
            }
    	    break;
        case IOCTL_LCM_PUT_CHAR:
            DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_PUT_CHAR enter\n");
      	    if (copy_from_user(&val8, (unsigned char*)arg, sizeof(unsigned char)))
    	        return -EFAULT;

            DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_PUT_CHAR ch=%c\n", val8);
            ret=sds_lcm_write_op(1, LCM_OP_WRITE_DATA, val8);
            if (ret!=0)
            {
                DBG_PRINT("moxa_sds_lcm> IOCTL_LCM_PUT_CHAR fail\n\n");
                return -EFAULT;
            }
    	    break;
        default:
            return -EINVAL;
    }

    return 0;
}

static ssize_t sds_lcm_write(struct file * filp, const char * buf, size_t count, loff_t *ppos)
{
	int i;

	for ( i=0; i<count; i++ )
		sds_lcm_ioctl(filp, IOCTL_LCM_PUT_CHAR, (unsigned long)&buf[i] );
		
	return count;
}

static int sds_lcm_open(struct inode *inode, struct file *file)
{
	if ( MINOR(inode->i_rdev) == MOXA_LCM_MINOR )
		return 0;

	return -ENODEV;
}

static int sds_lcm_release(struct inode *inode, struct file *file)
{
	return 0;
}

static struct file_operations sds_lcm_fops = {
	owner:THIS_MODULE,
	unlocked_ioctl:sds_lcm_ioctl,
	write: sds_lcm_write,
	open:sds_lcm_open,
	release:sds_lcm_release,
};

static struct miscdevice sds_lcm_dev = {
	MOXA_LCM_MINOR,
	"sds_lcm",
	&sds_lcm_fops
};

#ifdef CONFIG_ARCH_MOXAMACRO
/*---------------------------------------
 * S/W reset polling function
 *----------------------------------------*/
#define STDEF_POLL_TIME		(HZ/5) // about 200ms
static wait_queue_head_t	stdef_queue, ledon_queue, ledoff_queue;
static char			*rmdir_etc_argv[3]={"/bin/setdef", "-e", NULL};
static char			*rmdir_envp[3]={"HOME=/", "TERM=linux", NULL};
static struct timer_list	stdef_timer;
static int			counter=0;
static int	stdef_rmdir(void *unused)
{
        interruptible_sleep_on( &stdef_queue);
        // reboot/shutdown will invoke interrupt, so we must check here.
        if( counter < 25 ) {
        	kernel_thread(stdef_rmdir, NULL, CLONE_FS | CLONE_FILES);
                return 0;
	}
        call_usermodehelper(rmdir_etc_argv[ 0], rmdir_etc_argv, rmdir_envp, 0);
        return 0;
}

static int ledon(void *unused)
{
        interruptible_sleep_on(&ledon_queue);
	spin_lock(&keypad_lock);
	lcm_ctrl &= ~SW_READY_LED_OFF;
	*((unsigned char*)lcm_cmd1_addr) = lcm_ctrl;
	spin_unlock(&keypad_lock);
        kernel_thread(ledon, NULL, CLONE_FS | CLONE_FILES);
        return 0;
}

static int ledoff(void *unused)
{
        interruptible_sleep_on( &ledoff_queue);
	spin_lock(&keypad_lock);
	lcm_ctrl |= SW_READY_LED_OFF;
	*((unsigned char*)lcm_cmd1_addr) = lcm_ctrl;
	spin_unlock(&keypad_lock);
        kernel_thread( ledoff, NULL, CLONE_FS | CLONE_FILES);
        return 0;
}

static void	stdef_poll(unsigned long ingore)
{
        int	status;

        del_timer(&stdef_timer);
        status=__gpio_get_value(SW_RESET_GPIO);

        DBG_PRINT("%s[%d]:%d\n", __FUNCTION__, __LINE__, status);

        if( !status )
                counter++;
        else
                counter= 0;

        if( ((counter+4)%5) == 0 )
                wake_up_interruptible(&ledoff_queue);
        else if( ((counter+1)%5) == 0 )
                wake_up_interruptible(&ledon_queue);
        if( counter > 25) // 200ms * 25 = 5 seconds
                wake_up_interruptible(&stdef_queue);

        stdef_timer.function = stdef_poll;
        stdef_timer.expires = jiffies + STDEF_POLL_TIME;
        add_timer(&stdef_timer);
}
#endif

//
// Moxa DA-66X LED/ID/Keypad/LCM misc device initialize function call
//
static void moxa_misc_free_resource(void)
{
	// first unmap memory resource
	if ( led_id_addr )
		iounmap( ( unsigned char *)led_id_addr);
	if ( lcm_cmd0_addr )
		iounmap( ( unsigned char *)lcm_cmd0_addr);
	if ( lcm_cmd1_addr )
		iounmap( ( unsigned char *)lcm_cmd1_addr);
	if ( lcm_data_addr )
		iounmap( ( unsigned char *)lcm_data_addr);
		
	// release request memory request
	if ( led_id_resource )
		release_mem_region(LED_ID_ADDR, REGION_SIZE);
	if ( lcm_cmd0_resource )
		release_mem_region(LCM_CMD0_ADDR, REGION_SIZE);
	if ( lcm_cmd1_resource )
		release_mem_region(LCM_CMD1_ADDR, REGION_SIZE);
	if ( lcm_data_resource )
		release_mem_region(LCM_DATA_ADDR, REGION_SIZE);
}

static void __exit moxa_misc_exit(void)
{
#ifdef CONFIG_ARCH_MOXAMACRO
	// free S/W reset GPIO & wait queue, kthread
	gpio_free(SW_RESET_GPIO);
#endif

	moxa_misc_free_resource();

	// unregister misc
	misc_deregister(&led_dev);
	misc_deregister(&id_dev);
	misc_deregister(&sw_ready_dev);
	misc_deregister(&keypad_dev);
	switch(rc7000_software_id) {
	case DA660_PLUS_SOFTWARE_ID:
	case DA661_SOFTWARE_ID:
	case DA662_SOFTWARE_ID:
	case DA663_SOFTWARE_ID:
#ifdef CONFIG_ARCH_MOXAMACRO
	case DA662A_8_SOFTWARE_ID :
	case DA662A_16_SOFTWARE_ID :
#endif
		misc_deregister(&sds_lcm_dev);
		break;
	default:
		printk("%s[%x]:The software id is not recoginsable!\n", __FUNCTION__, __LINE__);
	}
		
	spin_lock(&keypad_lock);
	if ( keypadtimer_on ) {
		del_timer(&keypadtimer);
		keypadtimer_on = 0;
	}
	spin_unlock(&keypad_lock);
	remove_proc_entry("swid", NULL);
	remove_proc_entry("serid", NULL);
	del_timer(&stdef_timer);
}

static int __init moxa_misc_init(void)
{
	int	reg_led=0, reg_id=0, reg_keypad=0, reg_lcm=0, reg_sw_ready=0;
	
	// setup the software ID
#ifdef CONFIG_ARCH_MOXAMACRO
	#define ID_GPIO		14 // high for 16 ports, low for 8 ports
	if ( gpio_request_one(ID_GPIO, GPIOF_DIR_IN, "modelid") ) { // request fail, we check the PCI MU860
		struct pci_dev	*p;
		rc7000_software_id = DA662A_8_SOFTWARE_ID;
		p = pci_get_device(0x1393, 0x0001, NULL);
		if ( p && pci_get_device(0x1393, 0x0001, p) )
			rc7000_software_id = DA662A_16_SOFTWARE_ID;
	} else { // request OK.
		if ( __gpio_get_value(ID_GPIO) ) // high
			rc7000_software_id = DA662A_16_SOFTWARE_ID;
		else
			rc7000_software_id = DA662A_8_SOFTWARE_ID;
		gpio_free(ID_GPIO);
	}
#else
	rc7000_software_id = DA662_SOFTWARE_ID;
#endif

	// request S/W reset GPIO
#ifdef CONFIG_ARCH_MOXAMACRO
	if ( gpio_request_one(SW_RESET_GPIO, GPIOF_DIR_IN, "sw_reset") ) { // request fail
		printk("%s:Request GPIO %d fail !\n", __FILE__, SW_RESET_GPIO);
		return -1;
	}
#endif

	// create /proc/driver/swid interface
	proc_swid=create_proc_read_entry("driver/swid",0644, NULL, proc_swid_read, NULL);
	if( proc_swid == NULL ) {
		printk("%s[%x]:Can not create /proc/driver/swid file", __FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	// create /proc/driver/serid interface
	proc_serid=create_proc_read_entry("driver/serid",0644, NULL, proc_serid_read, NULL);
	if( proc_serid == NULL ) {
		printk("%s[%x]:Can not create /proc/driver/serid file", __FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	// create /proc/driver/isolation interface
	proc_isolation=create_proc_read_entry("driver/isolation",0644, NULL, proc_isolation_read, NULL);
	if( proc_isolation == NULL ) {
		printk("%s[%x]:Can not create /proc/driver/isolation file", __FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	// initialize the remap memory resource
	led_id_resource = NULL;
	lcm_cmd0_resource = NULL;
	lcm_cmd1_resource = NULL;
	lcm_data_resource = NULL;
	
	// initialize the memory resource
	led_id_addr = NULL;
	lcm_cmd0_addr = NULL;
	lcm_cmd1_addr = NULL;
	lcm_data_addr = NULL;
	keypad_addr = NULL;
	
	// initialize the bus timing
	moxaarm11_smc_init(3);	// CS2
	moxaarm11_smc_init(4);	// CS3
	moxaarm11_smc_init(5);	// CS4
	moxaarm11_smc_init(6);	// CS5

	// request the memory resoruce
	led_id_resource = request_mem_region(LED_ID_ADDR, REGION_SIZE, "Moxa DA-66X misc");
	if ( !led_id_resource ) {
		printk("%s[%x]: Could not request LED/ID memory region.\n", __FUNCTION__, __LINE__);
		goto moxa_misc_init_err;
	}
	
	// request the memory resoruce
	lcm_cmd0_resource = request_mem_region(LCM_CMD0_ADDR, REGION_SIZE, "Moxa DA-66X misc");
	if ( !lcm_cmd0_resource ) {
		printk("%s[%x]: Could not request Keypad/LCM_CMD0 memory region.\n", __FUNCTION__, __LINE__);
		goto moxa_misc_init_err;
	}
	lcm_cmd1_resource = request_mem_region(LCM_CMD1_ADDR, REGION_SIZE, "Moxa DA-66X misc");
	if ( !lcm_cmd1_resource ) {
		printk("%s[%x]: Could not request LCM_CMD1 memory region.\n", __FUNCTION__, __LINE__);
		goto moxa_misc_init_err;
	}
	lcm_data_resource = request_mem_region(LCM_DATA_ADDR, REGION_SIZE, "Moxa DA-66X misc");
	if ( !lcm_data_resource ) {
		printk("%s[%x]: Could not request LCM_DATA memory region.\n", __FUNCTION__, __LINE__);
		goto moxa_misc_init_err;
	}
	
	// remap the memory resource
	led_id_addr = ioremap(LED_ID_ADDR, REGION_SIZE);
	if ( !led_id_addr ) {
		printk("%s[%x]: Failed to map LED/ID memory region.\n", __FUNCTION__, __LINE__);
		goto moxa_misc_init_err;
	}
	
	// remap the memory resource
	lcm_cmd0_addr = ioremap(LCM_CMD0_ADDR, REGION_SIZE);
	if ( !lcm_cmd0_addr ) {
		printk("%s[%x]: Failed to map Keypad/LCM_CMD0 memory region.\n", __FUNCTION__, __LINE__);
		goto moxa_misc_init_err;
	}

	// Turn off software fault led
	*((unsigned char*) lcm_cmd0_addr) |= (unsigned char) 0x04;

	lcm_cmd1_addr = ioremap(LCM_CMD1_ADDR, REGION_SIZE);
	if ( !lcm_cmd1_addr ) {
		printk("%s[%x]: Failed to map LCM_CMD1 memory region.\n", __FUNCTION__, __LINE__);
		goto moxa_misc_init_err;
	}
	keypad_addr = lcm_cmd1_addr;

	lcm_data_addr = ioremap(LCM_DATA_ADDR, REGION_SIZE);
	if ( !lcm_data_addr ) {
		printk("%s[%x]: Failed to map LCM_DATA memory region.\n", __FUNCTION__, __LINE__);
		goto moxa_misc_init_err;
	}
	
	// register misc
	if ( misc_register(&led_dev) ) {
		printk("%s[%x]: Register LED misc fail !\n", __FUNCTION__, __LINE__);
		goto moxa_misc_init_err;
	}
	reg_led = 1;
	if ( misc_register(&id_dev) ) {
		printk("%s[%x]: Register ID misc fail !\n", __FUNCTION__, __LINE__);
		goto moxa_misc_init_err;
	}
	reg_id = 1;
	
	switch(rc7000_software_id) {
	case DA660_PLUS_SOFTWARE_ID:
	case DA661_SOFTWARE_ID:
	case DA662_SOFTWARE_ID:
	case DA663_SOFTWARE_ID:
#ifdef CONFIG_ARCH_MOXAMACRO
	case DA662A_8_SOFTWARE_ID :
	case DA662A_16_SOFTWARE_ID :
#endif
		//lcm_ctrl = SW_READY_LED_OFF;
		if ( misc_register(&sds_lcm_dev) ) {
			printk("%s[%x]: Register LCM misc fail !\n", __FUNCTION__, __LINE__);
			goto moxa_misc_init_err;
		}

		sds_lcm_init_module(1); // 2-lines mode
		reg_lcm = 1;
		break;
	default:
		printk("%s[%x]:The software id is not recoginsable!\n", __FUNCTION__, __LINE__);
	}
	
	if ( misc_register(&keypad_dev) ) {
		printk("%s[%x]: Register Keypad misc fail !\n", __FUNCTION__, __LINE__);
		goto moxa_misc_init_err;
	}
	reg_keypad = 1;
	init_timer(&keypadtimer);
	
	if ( misc_register(&sw_ready_dev) ) {
		printk("%s[%x]: Register software ready led misc fail !\n", __FUNCTION__, __LINE__);
		goto moxa_misc_init_err;
	}
	reg_sw_ready = 1;
	
	// initialize the S/W reset polling function
	init_timer(&stdef_timer);
        stdef_timer.function = stdef_poll;
        stdef_timer.expires = jiffies + STDEF_POLL_TIME;
        add_timer(&stdef_timer);
        init_waitqueue_head(&stdef_queue);
        kernel_thread(stdef_rmdir, NULL, CLONE_FS | CLONE_FILES);
        init_waitqueue_head(&ledon_queue);
        kernel_thread(ledon, NULL, CLONE_FS | CLONE_FILES);
        init_waitqueue_head(&ledoff_queue);
        kernel_thread(ledoff, NULL, CLONE_FS | CLONE_FILES);

	printk("Moxa DA-66X-LX/ID/Keypad/LCM driver v"MOXA_MISC_VERSION"\n");

	return 0;

moxa_misc_init_err:
	moxa_misc_free_resource();
	if ( reg_led )
		misc_deregister(&led_dev);
	if ( reg_id )
		misc_deregister(&id_dev);
	if ( reg_keypad )
		misc_deregister(&keypad_dev);
	if ( reg_sw_ready )
		misc_deregister(&sw_ready_dev);
	
	if ( reg_lcm ) {
		switch(rc7000_software_id) {
		case DA660_PLUS_SOFTWARE_ID:
		case DA661_SOFTWARE_ID:
		case DA662_SOFTWARE_ID:
		case DA663_SOFTWARE_ID:
#ifdef CONFIG_ARCH_MOXAMACRO
		case DA662A_8_SOFTWARE_ID :
		case DA662A_16_SOFTWARE_ID :
#endif
			misc_deregister(&sds_lcm_dev);
			break;
		default:
			printk("%s[%x]:The software id is not recoginsable!\n", __FUNCTION__, __LINE__);
		}
	}
	remove_proc_entry("driver/swid", NULL);
	remove_proc_entry("driver/serid", NULL);
	return -ENOMEM;
}

module_init(moxa_misc_init);
module_exit(moxa_misc_exit);

MODULE_AUTHOR("Victor Yu");
MODULE_LICENSE("GPL");
