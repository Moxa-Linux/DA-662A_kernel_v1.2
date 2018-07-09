/*
 * This is Moxa DCU-8620 AC Power Loss Detect device driver.
 * This device driver will create a DCU modle file located at /proc/driver/dcu_ac_power
 * The bit 0 stored the AC power loss detection iformation.
 *	bit 0 value	mean
 *	0		Power supplied
 *	1		Power loss
 *
 * History:
 * Date		Aurhor			Comment
 * 11-27-2012	Jared Wu.		Create it.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/proc_fs.h>

#define DCU_ACPOWER_ADDR		0x530000B0

static volatile unsigned char *dcu_ac_power_addr;
static struct proc_dir_entry *proc_dcu_ac_power;
static struct resource *dcu_ac_power_base_res=NULL;

ssize_t proc_dcu_ac_power_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
	int len=0;

	if( offset > 0 )
		return 0;

	/* The bit 0 stored the ac_power information  */
	len += sprintf(buffer, "%x\n", ((unsigned int)*dcu_ac_power_addr & 0x01));

	return len;
}

static void __exit dcu_ac_power_exit(void)
{
	if( dcu_ac_power_addr )
		iounmap( (unsigned char*)dcu_ac_power_addr );

	if ( dcu_ac_power_base_res )
		release_resource(dcu_ac_power_base_res);

	remove_proc_entry("driver/dcu_ac_power", proc_dcu_ac_power);
}

static int __init dcu_ac_power_init(void)
{
	int retval=0;

	printk("Moxa DCU AC Power detection driver v1.0\n");

	proc_dcu_ac_power=create_proc_read_entry("driver/dcu_ac_power",0444, NULL, proc_dcu_ac_power_read, NULL);
	if( proc_dcu_ac_power == NULL ) {
		printk("%s[%x]:Can not create /proc/driver/dcu_ac_power file", __FUNCTION__, __LINE__);
		retval=-ENOMEM;
		goto dcu_proc_error;
	}

	dcu_ac_power_base_res = request_mem_region(DCU_ACPOWER_ADDR, 0x01, "MoxaDCUACPower");
	if ( dcu_ac_power_base_res == NULL ) {
		printk("Moxa MoxaDCUACPower request memory region fail !\n");
		retval=-ENOMEM;
		goto dcu_request_mem_error;
	}

	dcu_ac_power_addr = ioremap(DCU_ACPOWER_ADDR, 0x01);
	if ( !dcu_ac_power_addr ) {
		printk("%s[%d]: Fail to map DCU_ACPOWER_ADDR\n",__FUNCTION__, __LINE__);
		retval=-ENOMEM;
		goto dcu_ioremap_error;
	}

	return 0;

dcu_ioremap_error:
	release_resource(dcu_ac_power_base_res);
dcu_request_mem_error:
	remove_proc_entry("driver/dcu_ac_power", proc_dcu_ac_power);
dcu_proc_error:
	return retval;
}

module_init(dcu_ac_power_init);
module_exit(dcu_ac_power_exit);

MODULE_AUTHOR("Jared Wu");
MODULE_LICENSE("GPL");
