/*
 * This is Moxa CPU DIO device driver.
 * It is from misc interface. So the device node major number is 10.
 * The device node minor number is following:
 * dio:		104
 *
 * History:
 * Date		Aurhor			Comment
 * 12-06-2005	Victor Yu.		Create it.
 */
//#include <linux/config.h>
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

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>
//#include <asm/arch/cpe/cpe.h>
#include <mach/gpio.h>

//#define DEBUG
#ifdef WADE
#define drv_dbg(fmt, args...) printk(KERN_INFO "%s: "fmt, __FUNCTION__, ##args)
#else 
#define drv_dbg(fmt, args...)
#endif
#define drv_err(fmt, args...) printk(KERN_ERR "MoxaArt DIO: "fmt, ##args)
#define drv_info(fmt, args...) printk(KERN_INFO "MoxaArt DIO: "fmt, ##args)


#define MOXA_DIO_MINOR		104

#if defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345) || defined(CONFIG_ARCH_W345_IMP1)
#define DO_BASE_SHIFT		22
#define DO_MASK			(0x01<<DO_BASE_SHIFT)
#define GPIO_MASK		(DO_MASK)
#else
#define DI_BASE_SHIFT		4
#define DO_BASE_SHIFT		18
#define DI_MASK			(0x0f<<DI_BASE_SHIFT)
#define DO_MASK			(0x0f<<DO_BASE_SHIFT)
#define GPIO_MASK		(DI_MASK|DO_MASK)
#endif

//
// DIO file operaiton function call
//
#if defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345) || defined(CONFIG_ARCH_W345_IMP1)
#define MAX_DIO			1	// just only DO
#elif defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W311) || defined(CONFIG_ARCH_w325) || defined(CONFIG_ARCH_W315) || defined(CONFIG_UC_7112_LX_PLUS)
#define MAX_DIO			0
#else
#define MAX_DIO			4
#endif
#define DIO_INPUT		1
#define DIO_OUTPUT		0
#define DIO_HIGH		1
#define DIO_LOW			0
#define DIO_BASE		3
#define IOCTL_DIO_GET_MODE      1
#define IOCTL_DIO_SET_MODE      2
#define IOCTL_DIO_GET_DATA      3
#define IOCTL_DIO_SET_DATA      4
#define IOCTL_SET_DOUT		15
#define IOCTL_GET_DOUT		16
#define IOCTL_GET_DIN		17
struct dio_set_struct {
	int	io_number;
	int	mode_data;	// 1 for input, 0 for output, 1 for high, 0 for low
};


struct proc_dir_entry* dio_proc;
#define dio_proc_name "driver/dio"
#define MAX_GPIO      32
int dio_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	char	*out = page;
	int	pin_count, len;
 
	for ( pin_count = 0 ; pin_count < MAX_GPIO ; pin_count++ )
		out += sprintf( out ,"PIN%d:%s:%s\n",pin_count, moxaart_gpio_get_inout(1<<pin_count)?"OUT":"IN", moxaart_gpio_get(1<<pin_count)?"HIGH":"LOW");  

	len = out -page - off;
	if (len < count)  {
		*eof = 1;
		if (len <= 0) 
			return 0;
	}  else
		len = count;
	*start = page + off;

	return len;
}

int dio_write_proc(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
	char	kbuffer[64], in_out_mode[2], mode[2], pin[3];
	int	num_count, var_num, byte_count;

	if ( copy_from_user(kbuffer, (char*)buffer , count) )
		return -EFAULT;

	kbuffer[count+1] = '\0';
	/* ex. echo 'pin in_out_mode mode' > /proc/driver/dio 
	 * in_out_mode: 0:in 1:out
	 * mode: 0: active 1:x */
	memset(pin,0,3);
	memset(in_out_mode,0,2);
	memset(mode,0,2);
	num_count = var_num = 0 ;

	for ( byte_count = 0 ; byte_count < count ; byte_count++ ) {
		if ( kbuffer[byte_count] != ' ' && kbuffer[byte_count] != '\0' && kbuffer[byte_count] != '\n' ) {
			if ( var_num == 0 ) {
				num_count++;     
				if ( num_count > 2 ) {
					printk("The max pin number is %d !!\n",MAX_GPIO);	
					return -EINVAL;
				}			      
				pin[num_count-1] =  kbuffer[byte_count];   
				pin[num_count] = '\0';
			}

			if ( var_num == 1 ) {
				num_count++;     
				if ( num_count > 1 ) {
					printk("The in mode = 1 out mode = 0 !!\n");	
					return -EINVAL;
				}			      
				in_out_mode[num_count-1] =  kbuffer[byte_count];   
				in_out_mode[num_count] =  '\0';   
			}
     
			if ( var_num == 2 ) {
				num_count++;     
				if ( num_count > 1 ) {
					printk("The in mode = 1 out mode = 0 !!\n");	
					return -EINVAL;
				}			      
				mode[num_count-1] =  kbuffer[byte_count];   
				mode[num_count] =  '\0';   
			}

		} else {
			if ( pin[0] != 0 && in_out_mode[0] == 0  ) {
				var_num = 1;
				num_count = 0;
			}
			if ( pin[0]!= 0 && in_out_mode[0] != 0 && mode[0] == 0 ) {
				var_num = 2;
				num_count = 0;
			}
		}
	}

	num_count = simple_strtol(pin, NULL, 0);	// to keep pin number
	var_num = simple_strtol(in_out_mode, NULL, 0);	// to keep in/out setting
	byte_count = simple_strtol(mode, NULL, 0);	// to keep state setting

	if ( num_count >= MAX_GPIO ) {
		//printk("The max pin number is %d !!\n",MAX_GPIO-1);	
		goto error_write_proc;
	}
		
	if ( var_num == MCPU_GPIO_OUTPUT  ) {
		moxaart_gpio_inout(1<<num_count, MCPU_GPIO_OUTPUT );
		if ( byte_count == DIO_HIGH )
			moxaart_gpio_set(1<<num_count , MCPU_GPIO_HIGH);
		else if ( byte_count == DIO_LOW )
			moxaart_gpio_set(1<<num_count, MCPU_GPIO_LOW);
		else
			goto error_write_proc;
	} else if ( var_num == MCPU_GPIO_INPUT ) {
		moxaart_gpio_inout(1<<num_count ,  MCPU_GPIO_INPUT );
	} else {
		//printk("OUTPUT = 1, INPUT = 0 !!\n");	
		goto error_write_proc;
	}

	return count;

error_write_proc:
	return -EINVAL;
}

static long dio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct dio_set_struct	set;

    drv_dbg("ioctl=%x\n", cmd);

#if defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345) || defined(CONFIG_ARCH_W345_IMP1)
	switch ( cmd ) {
	case IOCTL_SET_DOUT :
		if ( copy_from_user(&set, (struct dio_set_struct *)arg, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		if ( set.io_number < 0 || set.io_number >= MAX_DIO )
			return -EINVAL;
		if ( set.mode_data == DIO_HIGH )
			moxaart_gpio_set(1<<(set.io_number+DO_BASE_SHIFT), MCPU_GPIO_HIGH);
		else if ( set.mode_data == DIO_LOW )
			moxaart_gpio_set(1<<(set.io_number+DO_BASE_SHIFT), MCPU_GPIO_LOW);
		else
			return -EINVAL;
		break;
	case IOCTL_GET_DOUT :
		if ( copy_from_user(&set, (struct dio_set_struct *)arg, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		if ( set.io_number < 0 || set.io_number >= MAX_DIO )
			return -EINVAL;
		if ( moxaart_gpio_get(1<<(set.io_number+DO_BASE_SHIFT)) )
			set.mode_data = 1;
		else
			set.mode_data = 0;
		if ( copy_to_user((struct dio_set_struct *)arg, &set, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		break;
	default:
		return -EINVAL;
	}
	return 0;
#elif defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W311) || defined(CONFIG_ARCH_w325) || defined(CONFIG_ARCH_W315) || defined(CONFIG_UC_7112_LX_PLUS)
	return -EINVAL;
#else
	switch ( cmd ) {
	case IOCTL_SET_DOUT :
		if ( copy_from_user(&set, (struct dio_set_struct *)arg, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		if ( set.io_number < 0 || set.io_number >= MAX_DIO )
			return -EINVAL;
		if ( set.mode_data == DIO_HIGH )
			moxaart_gpio_set(1<<(set.io_number+DO_BASE_SHIFT), MCPU_GPIO_HIGH);
		else if ( set.mode_data == DIO_LOW )
			moxaart_gpio_set(1<<(set.io_number+DO_BASE_SHIFT), MCPU_GPIO_LOW);
		else
			return -EINVAL;
		break;
	case IOCTL_GET_DOUT :
	case IOCTL_GET_DIN :
        drv_dbg("get_din\n");
		if ( copy_from_user(&set, (struct dio_set_struct *)arg, sizeof(struct dio_set_struct)) ) {
            drv_dbg("copy_from_user error\n");
			return -EFAULT;
        }
		if ( set.io_number < 0 || set.io_number >= MAX_DIO ) {
            drv_dbg("Incorrect io_number=%d\n", set.io_number);
			return -EINVAL;
        }
		if ( cmd == IOCTL_GET_DOUT ) {
			if ( moxaart_gpio_get(1<<(set.io_number+DO_BASE_SHIFT)) )
				set.mode_data = 1;
			else
				set.mode_data = 0;
		} else {
			if ( moxaart_gpio_get(1<<(set.io_number+DI_BASE_SHIFT)) )
				set.mode_data = 1;
			else
				set.mode_data = 0;
		}
		if ( copy_to_user((struct dio_set_struct *)arg, &set, sizeof(struct dio_set_struct)) ) {
            drv_dbg("copy_to_user error\n");
			return -EFAULT;
        }
		break;
	default:
		return -EINVAL;
	}
    drv_dbg("return 0\n");
	return 0;
#endif
}

static int dio_open(struct inode *inode, struct file *file)
{
	if ( MINOR(inode->i_rdev) == MOXA_DIO_MINOR )
		return 0;

	return -ENODEV;
}

static int dio_release(struct inode *inode, struct file *file)
{
	return 0;
}

static struct file_operations dio_fops = {
	owner:THIS_MODULE,
	llseek:NULL,
	unlocked_ioctl:dio_ioctl,
	open:dio_open,
	release:dio_release,
};

static struct miscdevice dio_dev = {
	MOXA_DIO_MINOR,
	"dio",
	&dio_fops
};

static void __exit dio_exit(void)
{
    printk("Unregister DIO driver\n"); 
	remove_proc_entry(dio_proc_name,0);
	misc_deregister(&dio_dev);
}

static int __init dio_init(void)
{
	printk("Register IO misc ver1.0.1 ");
	if ( misc_register(&dio_dev) ) {
		printk("fail !\n");
		return -ENOMEM;
	}
	
	// set the CPU for GPIO
#if defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345) || defined(CONFIG_ARCH_W345_IMP1)
	moxaart_gpio_mp_set(GPIO_MASK);
#elif defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W311) || defined(CONFIG_ARCH_w325) || defined(CONFIG_ARCH_W315) || defined(CONFIG_UC_7112_LX_PLUS)
	// none DI & DO
#else
	moxaart_gpio_mp_set(GPIO_MASK);
#endif

	// default set all GPIO for input/ouput
#if defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345) || defined(CONFIG_ARCH_W345_IMP1)
	moxaart_gpio_inout(DO_MASK, MCPU_GPIO_OUTPUT);
#elif defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W311) || defined(CONFIG_ARCH_w325) || defined(CONFIG_ARCH_W315) || defined(CONFIG_UC_7112_LX_PLUS)
	// none DI & DO
#else
	moxaart_gpio_inout(DI_MASK, MCPU_GPIO_INPUT);
	moxaart_gpio_inout(DO_MASK, MCPU_GPIO_OUTPUT);
#endif

	dio_proc = create_proc_entry(dio_proc_name, 0, NULL);
	dio_proc->read_proc = dio_read_proc;
	dio_proc->write_proc = dio_write_proc;

	printk("OK.\n");
	return 0;
}

module_init(dio_init);
module_exit(dio_exit);

MODULE_AUTHOR("Victor Yu");
MODULE_LICENSE("GPL");
