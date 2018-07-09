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
#include <mach/irqs.h>
#include <linux/irq.h>
#include <mach/map.h>

#include <mach/gpio.h>

#define MOXA_DIO_MINOR		104
#define MOXAARM11_GPIO_IRQ	13

//
// DIO file operaiton function call
//
#define DIO_INPUT		1
#define DIO_OUTPUT		0
#define DIO_HIGH		1
#define DIO_LOW			0

#define DIO_INTR_LEVEL		0
#define DIO_INTR_EDGE		1
#define DIO_INTR_FALLING	0
#define DIO_INTR_RAISING	1
#define DIO_INTR_BOTH		2
#define DIO_INTR_LOW		0
#define DIO_INTR_HIGH		1

#define IOCTL_DIO_GET_MODE      1
#define IOCTL_DIO_SET_MODE      2
#define IOCTL_DIO_GET_DATA      3
#define IOCTL_DIO_SET_DATA      4
#define IOCTL_SET_DOUT		15
#define IOCTL_GET_DOUT		16
#define IOCTL_GET_DIN		17
#define IOCTL_SET_INTR		18
#define IOCTL_SET_DIR		19		// set direction

#define dio_proc_name "driver/dio"

#define MAX_DIO			32

struct dio_set_struct {
	int	io_number;
	int	mode_data;	// 1 for input, 0 for output, 1 for high, 0 for low
	int	level_edge;		// 1 = edge , 0 = level
	int	trigger_mode;	// 0 = low(falling) , 1 = high(rising) , 2 = both edge
};

static irqreturn_t dio_interrupt_handler(int irq, void *dev_id)
{

	mcpu_gpio_reg_t *gpio_reg;
	int i = 0;
	unsigned int iir = 0;

	gpio_reg = (mcpu_gpio_reg_t *)MOXAARM11_VA_GPIO_BASE;
	iir =  gpio_reg->mask_interrupt_status;
	
//	printk(KERN_ALERT"dio_interrupt_handler!%d\n" , __LINE__);	
	for( i = 0 ; i < MAX_DIO ; i++){
		if( iir & ( 0x1 << i ) ){
			/* do something here... */
			printk("GPIO pin %d interrupt generated!!\n");		
			mcpu_gpio_clear_irq_pending( 0x1 << i );
		}
	}
	return IRQ_HANDLED;
}



static struct irqaction moxaarm11_gpio_irq = {
        .name           = "MoxaARM11GPIO",
        .flags          	= IRQF_SHARED,
        .handler        = dio_interrupt_handler,
};



struct proc_dir_entry* dio_proc;

int dio_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	char	*out = page;
	int	pin_count, len;
 
	for ( pin_count = 0 ; pin_count < MAX_DIO ; pin_count++ ){
		out += sprintf( out ,"PIN%d:%s:%s\n",pin_count, mcpu_gpio_get_inout(1<<pin_count)?"OUT":"IN", mcpu_gpio_get(1<<pin_count)?"HIGH":"LOW");  
	}

	len = out -page - off;
	if (len < count)  {
		*eof = 1;
		if (len <= 0) {
			return 0;
		}
	}  else{
		len = count;
	}
	*start = page + off;

	return len;
}

int dio_write_proc(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
	char	kbuffer[64], in_out_mode[2], mode[2], pin[3];
	int	num_count, var_num, byte_count;

	if ( copy_from_user(kbuffer, (char*)buffer , count) ){
		return -EFAULT;
	}

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
					printk("The max pin number is %d !!\n",MAX_DIO);	
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

	if ( num_count >= MAX_DIO ) {
		//printk("The max pin number is %d !!\n",MAX_GPIO-1);	
		goto error_write_proc;
	}
		
	if ( var_num == MCPU_GPIO_OUTPUT  ) {
		mcpu_gpio_inout(1<<num_count, MCPU_GPIO_OUTPUT );
		if ( byte_count == DIO_HIGH )
			mcpu_gpio_set(1<<num_count , MCPU_GPIO_HIGH);
		else if ( byte_count == DIO_LOW )
			mcpu_gpio_set(1<<num_count, MCPU_GPIO_LOW);
		else
			goto error_write_proc;
	} else if ( var_num == MCPU_GPIO_INPUT ) {
		mcpu_gpio_inout(1<<num_count ,  MCPU_GPIO_INPUT );
	} else {
		//printk("OUTPUT = 1, INPUT = 0 !!\n");	
		goto error_write_proc;
	}

	return count;

error_write_proc:
	return -EINVAL;
}



static int dio_unlocked_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	struct dio_set_struct	set;
	//spinlock_t lock;
	unsigned long flags;
	
	//spin_lock_init(&lock);

	switch ( cmd ) {
	case IOCTL_SET_DOUT :
		if ( copy_from_user(&set, (struct dio_set_struct *)arg, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		if ( set.io_number < 0 || set.io_number >= MAX_DIO )
			return -EINVAL;
		if ( set.mode_data == DIO_HIGH ){

			mcpu_gpio_set(1<<(set.io_number), MCPU_GPIO_HIGH);
		}else if ( set.mode_data == DIO_LOW ){
	
			mcpu_gpio_set(1<<(set.io_number), MCPU_GPIO_LOW);
		}else{
			return -EINVAL;
		}
		break;
	case IOCTL_GET_DOUT :
	case IOCTL_GET_DIN :
		if ( copy_from_user(&set, (struct dio_set_struct *)arg, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		if ( set.io_number < 0 || set.io_number >= MAX_DIO )
			return -EINVAL;
		if ( cmd == IOCTL_GET_DOUT ) {
			if ( mcpu_gpio_get(1<<(set.io_number)) )
				set.mode_data = 1;
			else
				set.mode_data = 0;
		} else {
			if ( mcpu_gpio_get(1<<(set.io_number)) )
				set.mode_data = 1;
			else
				set.mode_data = 0;
		}
		if ( copy_to_user((struct dio_set_struct *)arg, &set, sizeof(struct dio_set_struct)) )
			return -EFAULT;
		break;
	case IOCTL_SET_DIR:
		if ( copy_from_user(&set, (struct dio_set_struct *)arg, sizeof(struct dio_set_struct)) ){
			return -EFAULT;		
		}
		if ( set.io_number < 0 || set.io_number >= MAX_DIO ){
			return -EINVAL;
		}
		if (set.mode_data == DIO_OUTPUT ){
			mcpu_gpio_inout( 0x1<<(set.io_number) , MCPU_GPIO_OUTPUT);		
		}else if (set.mode_data == DIO_INPUT ){
			mcpu_gpio_inout( 0x1<<(set.io_number) , MCPU_GPIO_INPUT);		
		}else{
			return -EINVAL;
		}
		break;
	case IOCTL_SET_INTR:
		if ( copy_from_user(&set, (struct dio_set_struct *)arg, sizeof(struct dio_set_struct)) ){
			return -EFAULT;		
		}
		if ( set.io_number < 0 || set.io_number >= MAX_DIO ){
			return -EINVAL;
		}	


		if( set.level_edge == DIO_INTR_LEVEL && set.trigger_mode== DIO_INTR_LOW){
			mcpu_gpio_set_to_irq(1<<(set.io_number), vicc_level_activeLow);
		}else if( set.level_edge == DIO_INTR_LEVEL && set.trigger_mode == DIO_INTR_HIGH){
			mcpu_gpio_set_to_irq(1<<(set.io_number), vicc_level_activeHigh);
		}else if( set.level_edge == DIO_INTR_EDGE && set.trigger_mode == DIO_INTR_FALLING){
			mcpu_gpio_set_to_irq(1<<(set.io_number), vicc_edge_activeFalling);
		}else if( set.level_edge == DIO_INTR_EDGE && set.trigger_mode == DIO_INTR_RAISING){
			mcpu_gpio_set_to_irq(1<<(set.io_number), vicc_edge_activeRaising);
		}else if( set.level_edge == DIO_INTR_EDGE && set.trigger_mode == DIO_INTR_BOTH){
			mcpu_gpio_set_to_irq(1<<(set.io_number), vicc_edge_activeBoth);
		}else{
			return -EINVAL;
		}

	
	default:
		return -EINVAL;
	}
	return 0;

}

static int dio_open(struct inode *inode, struct file *file)
{
	if ( MINOR(inode->i_rdev) == MOXA_DIO_MINOR ){
		return 0;
	}
	
	return -ENODEV;
}

static int dio_release(struct inode *inode, struct file *file)	
{
	return 0;
}


static struct file_operations dio_fops = {
	owner:THIS_MODULE,
	llseek:NULL,
	unlocked_ioctl:dio_unlocked_ioctl,
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
	unsigned long flags;
	//spinlock_t lock;
	//spin_lock_init(&lock);
	
	remove_proc_entry(dio_proc_name,0);
	misc_deregister(&dio_dev);
	//spin_lock_irqsave(&lock , flags);
	remove_irq(MOXAARM11_GPIO_IRQ , &moxaarm11_gpio_irq);
	//spin_unlock_irqrestore(&lock , flags);
}

static int __init dio_init(void)
{
	//spinlock_t lock;
	unsigned long flags;

	printk("Register IO misc ver 1.4 , [2010-09-02], ");
	if ( misc_register(&dio_dev) ) {
		printk("fail !\n");
		return -ENOMEM;
	}

	/* Register GPIO ISR */
	//spin_lock_irqsave(lock, flags);
	moxaarm11_vic_set_intr_trigger(MOXAARM11_GPIO_IRQ, vicc_level_activeHigh);
	if( setup_irq(MOXAARM11_GPIO_IRQ , &moxaarm11_gpio_irq) ){
		printk(KERN_ALERT"Request interrupt service fail !\n");
		goto dio_init;	
	}
	//spin_unlock_irqrestore(lock, flags);

//	mcpu_gpio_inout(0x000000ff, MCPU_GPIO_INPUT);		// 0~7 input	(FPGA)
//	mcpu_gpio_inout(0x0000ff00, MCPU_GPIO_OUTPUT);		// 8~15 output (FPGA)
	mcpu_gpio_inout(0x0000ffff, MCPU_GPIO_OUTPUT);
	
	mcpu_gpio_mask_pin(0xFFFFFFFF, 1);
	mcpu_gpio_clear_irq_pending(0xFFFFFFFF);

	dio_proc = create_proc_entry(dio_proc_name, 0, NULL);
	dio_proc->read_proc = dio_read_proc;
	dio_proc->write_proc = dio_write_proc;

	printk("OK.\n");
	return 0;

dio_init:
	return -EINTR;
	
}

module_init(dio_init);
module_exit(dio_exit);

MODULE_AUTHOR("Victor Yu");
MODULE_LICENSE("GPL");
