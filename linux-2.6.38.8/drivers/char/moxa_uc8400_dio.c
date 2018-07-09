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
#if defined(CONFIG_ARCH_MOXACPU) 
#include <asm/arch/cpe/cpe.h>
#endif
#include <mach/gpio.h>

#define MOXA_DIO_MINOR		104

#if defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345)
#define DO_BASE_SHIFT		22
#define DO_MASK			(0x01<<DO_BASE_SHIFT)
#define GPIO_MASK		(DO_MASK)
#elif defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620) // Add by Jared 07-24-2008

#include <linux/pci.h>

#define DIN_ADDR                0x53000010
#define DOUT_ADDR               0x53000020
#define REGION_SIZE             0x10
#define GPIO_MASK               0x0f

//#define HAS_DIO_MODULE          // support 1st DIO modules
//#define HAS_SECOND_DIO                // support 2nd DIO modules

#ifdef HAS_SECOND_DIO
#define MAX_CAN_DIO             16
#define MAX_CAN_DIN_PORT        16
#define MAX_CAN_DOUT_PORT       16
#else
#define MAX_CAN_DIO             8
#define MAX_CAN_DIN_PORT        8
#define MAX_CAN_DOUT_PORT       8
#endif

#ifdef HAS_DIO_MODULE
static u32              di_address=0xe402, do_address=0xe401;
#ifdef HAS_SECOND_DIO
static u32              di2_address=0xe404, do2_address=0xe403;
#endif

#endif  // HAS_DIO_MODULE


static int bPCIRegistered = 0; // 11-07-2008 Jared: flag for PCI registered
struct pci_dev *pcidev; // keep the pci_dev structure
static volatile unsigned char   *din_addr=NULL, *dout_addr=NULL;
static unsigned dout_keep_value=0;
static struct resource          *din_res=NULL, *dout_res=NULL;
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
#if defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345)
#define MAX_DIO			1	// just only DO
#elif defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W311) || defined(CONFIG_ARCH_w325) || defined(CONFIG_ARCH_W315) || defined(CONFIG_UC_7112_LX_PLUS)
#define MAX_DIO			0
#elif defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620) // Add by Jared 07-24-2008
#define MAX_DIO			4
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

// 11-07-2008, Jared: for supporting the DIO on CAN board
static int max_dio = MAX_DIO;

struct proc_dir_entry* dio_proc;
#define dio_proc_name "driver/dio"
#define MAX_GPIO      32
int dio_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	char	*out = page;
	int	pin_count, len;

#if defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
        for ( pin_count = 0 ; pin_count < max_dio ; pin_count++ ) {
		if ( pin_count < max_dio ) // read the DI from 8410
			out += sprintf( out ,"PIN%d:IN:%s\n",pin_count,\
                        (*din_addr)&(1<<pin_count)?"HIGH":"LOW");
#ifdef HAS_DIO_MODULE // Add by Jared 07-24-2008
                else // read the DI from CAN board
                        out += sprintf( out ,"PIN%d:IN:%s\n",pin_count,\
                        inb(di_address)&((1<<pin_count)>>max_dio)?"HIGH":"LOW");
#endif
        }
        for ( pin_count = 0 ; pin_count < max_dio ; pin_count++ ) {
                out += sprintf( out ,"PIN%d:OUT:%s\n",pin_count,\
                        (dout_keep_value)&(1<<pin_count)?"HIGH":"LOW");
        }
#else
	for ( pin_count = 0 ; pin_count < MAX_GPIO ; pin_count++ )
		out += sprintf( out ,"PIN%d:%s:%s\n",pin_count, mcpu_gpio_get_inout(1<<pin_count)?"OUT":"IN", mcpu_gpio_get(1<<pin_count)?"HIGH":"LOW");  
#endif
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

			if ( var_num == 1 ) {	// to read input(DI) or output(DO) mode
				num_count++;     
				if ( num_count > 1 ) {
					printk("The input mode = 1, output mode = 0 !!\n");	
					return -EINVAL;
				}			      
				in_out_mode[num_count-1] =  kbuffer[byte_count];   
				in_out_mode[num_count] =  '\0';   
			}
     
			if ( var_num == 2 ) {	// to read high or low for output(DO) mode
				num_count++;     
				if ( num_count > 1 ) {
					printk("The high = 1, low = 0 !!\n");	
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
			if ( pin[0] != 0 && in_out_mode[0] != 0 && mode[0] == 0 ) {
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
#if defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
	if ( var_num == 1 )     // output mode
#else
	if ( var_num == MCPU_GPIO_OUTPUT  )
#endif
    {
#if ! ( defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620) )
		mcpu_gpio_inout(1<<num_count, MCPU_GPIO_OUTPUT );
#endif
		if ( byte_count == DIO_HIGH ) {
#if defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
			dout_keep_value |= (1<<num_count);
                        if ( num_count < max_dio )
                                *dout_addr |= (1 << num_count);
#ifdef HAS_DIO_MODULE // Add by Jared 07-24-2008
                        else
				outb((dout_keep_value>>max_dio), do_address);
#endif
#else
			mcpu_gpio_set(1<<num_count , MCPU_GPIO_HIGH);
#endif
        }
		else if ( byte_count == DIO_LOW ) {
#if defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
			dout_keep_value &= ~(1<<num_count);
                        if ( num_count < max_dio )
                                *dout_addr &= ~(1 << num_count);
#ifdef HAS_DIO_MODULE // Add by Jared 07-24-2008
                        else
				outb((dout_keep_value>>max_dio), do_address);
#endif
#else
			mcpu_gpio_set(1<<num_count, MCPU_GPIO_LOW);
#endif
        }
		else
			goto error_write_proc;
#if defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
	} else if ( var_num == 0 ) {	// input mode
#else
	} else if ( var_num == MCPU_GPIO_INPUT ) {
#endif
#if !(defined(CONFIG_MACH_MOXA_UC8400) || defined(CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620))
		mcpu_gpio_inout(1<<num_count ,  MCPU_GPIO_INPUT );
#endif

	} else {
		//printk("OUTPUT = 1, INPUT = 0 !!\n");	
		goto error_write_proc;
	}

	return count;

error_write_proc:
	return -EINVAL;
}

#if 1 // Jared, porting to 2.6.38
static long dio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
#else
static int dio_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
#endif
{
	struct dio_set_struct	set;

#if defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345)
	switch ( cmd ) {
	case IOCTL_SET_DOUT :
		if ( copy_from_user(&set, (struct dio_set_struct *)arg, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		if ( set.io_number < 0 || set.io_number >= max_dio )
			return -EINVAL;
		if ( set.mode_data == DIO_HIGH )
			mcpu_gpio_set(1<<(set.io_number+DO_BASE_SHIFT), MCPU_GPIO_HIGH);
		else if ( set.mode_data == DIO_LOW )
			mcpu_gpio_set(1<<(set.io_number+DO_BASE_SHIFT), MCPU_GPIO_LOW);
		else
			return -EINVAL;
		break;
	case IOCTL_GET_DOUT :
		if ( copy_from_user(&set, (struct dio_set_struct *)arg, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		if ( set.io_number < 0 || set.io_number >= max_dio )
			return -EINVAL;
		if ( mcpu_gpio_get(1<<(set.io_number+DO_BASE_SHIFT)) )
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
#elif defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)

	switch ( cmd ) {
	case IOCTL_SET_DOUT :
		if ( copy_from_user(&set, (struct dio_set_struct *)arg, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		if ( set.io_number < 0 || set.io_number >= max_dio )
			return -EINVAL;
		if ( set.mode_data == DIO_HIGH ) {
#if 0	// 11-07-2008 Jared, support Moxa CAN DIO
				*dout_addr |= (1 << set.io_number);
#else
			dout_keep_value |= (1 << set.io_number);
			if ( set.io_number < max_dio )
				*dout_addr |= (1 << set.io_number);
#ifdef HAS_DIO_MODULE // Add by Jared 07-24-2008
			else
				outb((dout_keep_value>>max_dio), do_address);
#endif
#endif
		} else if ( set.mode_data == DIO_LOW ) {
			dout_keep_value &= ~(1 << set.io_number);
#if 0	// 11-07-2008 Jared, support Moxa CAN DIO
			*dout_addr &= ~(1 << set.io_number);
#else
			if ( set.io_number < max_dio ) // Set the Dout on 8410
				*dout_addr &= ~(1 << set.io_number);
#ifdef HAS_DIO_MODULE // Add by Jared 07-24-2008
			else // Set the Dout on CAN board
				outb((dout_keep_value>>max_dio), do_address);
#endif
#endif 
		} else
			return -EINVAL;
		break;
	case IOCTL_GET_DOUT :
		if ( copy_from_user(&set, (struct dio_set_struct *)arg, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		if ( set.io_number < 0 || set.io_number >= max_dio )
			return -EINVAL;
		set.mode_data = dout_keep_value & (1<<set.io_number)? 1 : 0;
		if ( copy_to_user((struct dio_set_struct *)arg, &set, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		break;
	case IOCTL_GET_DIN :
		if ( copy_from_user(&set, (struct dio_set_struct *)arg, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		if ( set.io_number < 0 || set.io_number >= max_dio )
			return -EINVAL;
#if 0	// 11-07-2008 Jared, support Moxa CAN DIO
		if ( (*din_addr) & (1<<set.io_number) )
			set.mode_data = 1;
		else
			set.mode_data = 0;
#else
		if ( set.io_number < max_dio ) // read the DI from 8410
				set.mode_data = (*din_addr) & (1<<set.io_number)? 1 : 0;
#ifdef HAS_DIO_MODULE // Add by Jared 07-24-2008
		else // read the DI from CAN board
				set.mode_data = inb(di_address) & ((1<<set.io_number)>>max_dio)? 1 : 0;

#endif
#endif
		if ( copy_to_user((struct dio_set_struct *)arg, &set, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		break;
	default:
		return -EINVAL;
	}
	return 0;

#else // end of #if defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
	switch ( cmd ) {
	case IOCTL_SET_DOUT :
		if ( copy_from_user(&set, (struct dio_set_struct *)arg, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		if ( set.io_number < 0 || set.io_number >= max_dio )
			return -EINVAL;
		if ( set.mode_data == DIO_HIGH )
			mcpu_gpio_set(1<<(set.io_number+DO_BASE_SHIFT), MCPU_GPIO_HIGH);
		else if ( set.mode_data == DIO_LOW )
			mcpu_gpio_set(1<<(set.io_number+DO_BASE_SHIFT), MCPU_GPIO_LOW);
		else
			return -EINVAL;
		break;
	case IOCTL_GET_DOUT :
	case IOCTL_GET_DIN :
		if ( copy_from_user(&set, (struct dio_set_struct *)arg, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		if ( set.io_number < 0 || set.io_number >= max_dio )
			return -EINVAL;
		if ( cmd == IOCTL_GET_DOUT ) {
			if ( mcpu_gpio_get(1<<(set.io_number+DO_BASE_SHIFT)) )
				set.mode_data = 1;
			else
				set.mode_data = 0;
		} else {
			if ( mcpu_gpio_get(1<<(set.io_number+DI_BASE_SHIFT)) )
				set.mode_data = 1;
			else
				set.mode_data = 0;
		}
		if ( copy_to_user((struct dio_set_struct *)arg, &set, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		break;
	default:
		return -EINVAL;
	}
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
#if 1 // Jared, porting to 2.6.38
	unlocked_ioctl:dio_ioctl,
#else
	ioctl:dio_ioctl,
#endif
	open:dio_open,
	release:dio_release,
};

static struct miscdevice dio_dev = {
	MOXA_DIO_MINOR,
	"dio",
	&dio_fops
};

#if defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620) // Add by Jared 07-24-2008

static int __devinit	moxacan_dio_init_one(struct pci_dev *pcidev, const struct pci_device_id *pciid)
{
	if ( pci_enable_device(pcidev) )
		return -EIO;

#ifdef HAS_DIO_MODULE
//	do_address = pci_resource_start(pcidev, 3);
//	di_address = do_address + 1;
	#ifdef HAS_SECOND_DIO
		do2_address = do_address + 2;
		di2_address = do_address + 3;
	#endif	// HAS_SECOND_DIO

	if ( request_region(do_address, 4, "moxacan_dio") == NULL )
		return -EIO;

	// initialize the DO value
	outb((dout_keep_value>>max_dio), do_address);
	printk("Moxa DI base IO address = 0x%x, DO base IO address = 0x%x\n", di_address, do_address);

	max_dio = max_dio + MAX_CAN_DIO;
	printk("<1>%d DI/DO are Probed\n", max_dio);
#endif	// HAS_DIO_MODULE


	return 0;
}

static void __devexit	moxacan_dio_remove_one(struct pci_dev *pcidev)
{
	printk("%s entry.\n", __FUNCTION__);

	pci_disable_device(pcidev);
#ifdef HAS_DIO_MODULE // Add by Jared 07-24-2008
	release_region(do_address, 4);
#endif	// HAS_DIO_MODULE
}
#if 0
static struct pci_device_id moxacan_dio_pci_tbl[] __devinitdata =
{
	{0x1393, 0x0100, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
	{0}
};

MODULE_DEVICE_TABLE(pci, moxacan_dio_pci_tbl);

static struct pci_driver	moxacan_dio_pci_driver = {
	name:		"moxacan_dio",
	id_table:	moxacan_dio_pci_tbl,
	probe:		moxacan_dio_init_one,
	remove:		__devexit_p(moxacan_dio_remove_one),
};
#endif
#endif


static void __exit dio_exit(void)
{
#if defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620) // Add by Jared 07-24-2008
	// Un-initialize the Moxa CAN/IO PCI driver
	if ( bPCIRegistered == 1 ) {
		moxacan_dio_remove_one(pcidev);
		//pci_unregister_driver(&moxacan_dio_pci_driver);
	}

	if ( din_res ) {
		release_resource(din_res);
		kfree(din_res);
	}
	if ( dout_res ) {
		release_resource(dout_res);
		kfree(dout_res);
	}
	#if 0	// mask by Victor Yu. You have to be on bit0 & bit1 address to protect NAND access
	if(din_addr)
		iounmap( (unsigned char*)din_addr );
	if(dout_addr)
		iounmap( (unsigned char*)dout_addr );
	#else
	if(din_addr)
		iounmap( (unsigned char*)(din_addr-3) );
	if(dout_addr)
		iounmap( (unsigned char*)(dout_addr-3) );
	#endif
#endif
	remove_proc_entry(dio_proc_name,0);
	misc_deregister(&dio_dev);
}

static int __init dio_init(void)
{
	printk("Register IO misc ver1.0 ");
	if ( misc_register(&dio_dev) ) {
		printk("fail !\n");
		return -ENOMEM;
	}
	
	// set the CPU for GPIO
#if defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345)
	mcpu_gpio_mp_set(GPIO_MASK);
#elif defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W311) || defined(CONFIG_ARCH_w325) || defined(CONFIG_ARCH_W315) || defined(CONFIG_UC_7112_LX_PLUS) || defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620) // none DI & DO
	// none DI & DO
#else
	mcpu_gpio_mp_set(GPIO_MASK);
#endif

	// default set all GPIO for input/ouput
#if defined(CONFIG_ARCH_W341) || defined(CONFIG_ARCH_W345)
	mcpu_gpio_inout(DO_MASK, MCPU_GPIO_OUTPUT);
#elif defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W311) || defined(CONFIG_ARCH_w325) || defined(CONFIG_ARCH_W315) || defined(CONFIG_UC_7112_LX_PLUS)
	// none DI & DO
#elif defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620) // Add by Jared 07-24-2008
	din_res = request_mem_region(DIN_ADDR, REGION_SIZE, "moxadin");
	if ( din_res == NULL ) {
		printk("Fail to request Moxa DIN addr !\n");
		goto Error;
	}
	dout_res = request_mem_region(DOUT_ADDR, REGION_SIZE, "moxadout");
	if ( dout_res == NULL ) {
		printk("Fail to request Moxa DOUT addr !\n");
		goto Error;
	}
#if 1	// mask by Victor Yu. You have to be on bit0 & bit1 address to protect NAND access
	din_addr = ioremap(DIN_ADDR, REGION_SIZE);
#else
	din_addr = ioremap(DIN_ADDR, REGION_SIZE) + 3;
#endif
	if( !din_addr) {
		printk("%s[%d]: Fail to map DIN_ADDR\n",__FUNCTION__, __LINE__);
		goto Error;
	}
#if 1	// mask by Victor Yu. You have to be on bit0 & bit1 address to protect NAND access
	dout_addr = ioremap(DOUT_ADDR, REGION_SIZE);
#else
	dout_addr = ioremap(DOUT_ADDR, REGION_SIZE) + 3;
#endif
	if( !dout_addr) {
		printk("%s[%d]: Fail to map DOUT_ADDR\n",__FUNCTION__, __LINE__);
		goto Error;
	}

	// Jared 11-07-2008, Initialize the DIO resource on Moxa CAN board
//	if ( pci_register_driver(&moxacan_dio_pci_driver) == 0 ) {
	pcidev = pci_get_device(0x1393, 0x0100, NULL);
	if (  pcidev != NULL ) {
		printk("Register Moxa CAN/IO PCI driver success\n");
		moxacan_dio_init_one(pcidev, NULL);
		bPCIRegistered = 1;
	}
	else {
		printk("<1>%d DI/DO are Probed\n", max_dio);
	}
#else
	mcpu_gpio_inout(DI_MASK, MCPU_GPIO_INPUT);
	mcpu_gpio_inout(DO_MASK, MCPU_GPIO_OUTPUT);
#endif

	dio_proc = create_proc_entry(dio_proc_name, 0, NULL);
	dio_proc->read_proc = dio_read_proc;
	dio_proc->write_proc = dio_write_proc;

	printk("OK.\n");
	return 0;
#if defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620) // Add by Jared 07-24-2008
Error:
	if ( din_res ) {
		release_resource(din_res);
		kfree(din_res);
	}
	if ( dout_res ) {
		release_resource(dout_res);
		kfree(dout_res);
	}
	if ( din_addr ) 
		iounmap((void __iomem *)din_addr);
	if ( dout_addr ) 
		iounmap((void __iomem *)dout_addr);
	return -1;
#endif
}

module_init(dio_init);
module_exit(dio_exit);

MODULE_AUTHOR("Victor Yu");
MODULE_LICENSE("GPL");
