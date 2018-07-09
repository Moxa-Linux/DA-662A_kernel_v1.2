/*
 * This is Moxa software id device driver.
 * This device driver will create a DCU modle file located at /proc/driver/dcu_model
 * The latest 4 bits stored the model information.
 *	switch value	return value
 *	0		0xF
 *	1		0xE
 *	2		0xD
 *	...		...
 *
 * History:
 * Date		Aurhor			Comment
 * 10-31-2012	Jared Wu.		Create it.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/proc_fs.h>

#define DCU_MODEL_ADDR		0x530000A0

static volatile unsigned char *dcu_model_addr;
static struct proc_dir_entry *proc_dcu_model;
static struct resource *dcu_model_base_res=NULL;

ssize_t proc_dcu_model_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
	int len=0;

	if( offset > 0 )
		return 0;

	/* The latest 4 bits stored the model information  */
	len += sprintf(buffer, "%x\n", ((unsigned int)*dcu_model_addr & 0x000F));

	return len;
}

static void __exit dcu_model_exit(void)
{
	if( dcu_model_addr )
		iounmap( (unsigned char*)dcu_model_addr );

	if ( dcu_model_base_res )
		release_resource(dcu_model_base_res);

	remove_proc_entry("driver/dcu_model", proc_dcu_model);
}

static int __init dcu_model_init(void)
{
	int retval=0;

	printk("Moxa DCU Model driver v1.0\n");

	proc_dcu_model=create_proc_read_entry("driver/dcu_model",0444, NULL, proc_dcu_model_read, NULL);
	if( proc_dcu_model == NULL ) {
		printk("%s[%x]:Can not create /proc/driver/dcu_model file", __FUNCTION__, __LINE__);
		retval=-ENOMEM;
		goto dcu_proc_error;
	}

	dcu_model_base_res = request_mem_region(DCU_MODEL_ADDR, 0x01, "MoxaDCUModel");
	if ( dcu_model_base_res == NULL ) {
		printk("Moxa MoxaDCUModel request memory region fail !\n");
		retval=-ENOMEM;
		goto dcu_request_mem_error;
	}

	dcu_model_addr = ioremap(DCU_MODEL_ADDR, 0x01);
	if ( !dcu_model_addr ) {
		printk("%s[%d]: Fail to map DCU_MODEL_ADDR\n",__FUNCTION__, __LINE__);
		retval=-ENOMEM;
		goto dcu_ioremap_error;
	}

	return 0;

dcu_ioremap_error:
	release_resource(dcu_model_base_res);
dcu_request_mem_error:
	remove_proc_entry("driver/dcu_model", proc_dcu_model);
dcu_proc_error:
	return retval;
}

module_init(dcu_model_init);
module_exit(dcu_model_exit);

MODULE_AUTHOR("Jared Wu");
MODULE_LICENSE("GPL");
