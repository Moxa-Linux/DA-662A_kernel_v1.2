/*
 * This is Moxa DCU-8620 GPO device driver.
 * This device driver will create a control file at /proc/driver/dcu_3g_gpo
 * Write a value 0 can turn off the 3G module power. Value 1 for turn on the 3G module power.
 *	value	mean
 *	0	Turn off the 3G power
 *	1	Turn on the 3G power
 *
 * History:
 * Date		Aurhor			Comment
 * 07-18-2013	Jared Wu.		Create it.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>

#define DCU_3G_GPO_ADDR		0x53000090

static volatile unsigned char *dcu_3g_gpo_addr;
static struct proc_dir_entry *proc_dcu_3g_gpo;
static struct resource *dcu_3g_gpo_base_res=NULL;
static struct mutex dcu_3g_gpo_sem;

int proc_dcu_3g_gpo_write(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
	int	dcu_3g_gpo_value;
	char	kbuffer[64];

	if ( copy_from_user(kbuffer, (char*)buffer , count) )
		return -EFAULT;

	kbuffer[count+1] = '\0';
	sscanf(kbuffer, "%d", &dcu_3g_gpo_value);

	if ( dcu_3g_gpo_value < 0 || dcu_3g_gpo_value > 1 ) {
		printk(KERN_ERR "The input value is not 0 or 1\n");
		return -EINVAL;
	}

	mutex_lock(&dcu_3g_gpo_sem);

	if ( dcu_3g_gpo_value == 0 )
		writeb( 0 , dcu_3g_gpo_addr); // Turn off the 3G module power
	else
		writeb( 2 , dcu_3g_gpo_addr); // Turn on the 3G module power

	mutex_unlock(&dcu_3g_gpo_sem);

	return count;
}

static void __exit dcu_3g_gpo_exit(void)
{
	if( dcu_3g_gpo_addr )
		iounmap( (unsigned char*)dcu_3g_gpo_addr );

	if ( dcu_3g_gpo_base_res )
		release_resource(dcu_3g_gpo_base_res);

	remove_proc_entry("driver/dcu_3g_gpo", NULL);
}

static int __init dcu_3g_gpo_init(void)
{
	int retval=0;

	printk("Moxa DCU GPO device driver v1.0\n");

	proc_dcu_3g_gpo=create_proc_entry("driver/dcu_3g_gpo",0666, NULL);
	if( proc_dcu_3g_gpo == NULL ) {
		printk("%s[%x]:Can not create /proc/driver/dcu_3g_gpo file", __FUNCTION__, __LINE__);
		retval=-ENOMEM;
		goto dcu_proc_error;
	}
	proc_dcu_3g_gpo->write_proc=proc_dcu_3g_gpo_write;

	dcu_3g_gpo_base_res = request_mem_region(DCU_3G_GPO_ADDR, 0x01, "MoxaDCU3GGPO");
	if ( dcu_3g_gpo_base_res == NULL ) {
		printk("Moxa MoxaDCU3GGPO request memory region fail !\n");
		retval=-ENOMEM;
		goto dcu_request_mem_error;
	}

	dcu_3g_gpo_addr = ioremap(DCU_3G_GPO_ADDR, 0x01);
	if ( !dcu_3g_gpo_addr ) {
		printk("%s[%d]: Fail to map DCU_3G_GPO_ADDR\n",__FUNCTION__, __LINE__);
		retval=-ENOMEM;
		goto dcu_ioremap_error;
	}

	mutex_init(&dcu_3g_gpo_sem);

	return 0;

dcu_ioremap_error:
	release_resource(dcu_3g_gpo_base_res);
dcu_request_mem_error:
	remove_proc_entry("driver/dcu_3g_gpo", proc_dcu_3g_gpo);
dcu_proc_error:
	return retval;
}

module_init(dcu_3g_gpo_init);
module_exit(dcu_3g_gpo_exit);

MODULE_AUTHOR("Jared Wu");
MODULE_LICENSE("GPL");
