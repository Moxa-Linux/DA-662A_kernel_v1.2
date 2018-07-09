/*
 * This is Moxa software id device driver.
 * This device driver will create a software ID file located at /proc/driver/swid
 * It is from proc interface.
 *
 * History:
 * Date		Aurhor			Comment
 * 07-31-2008	Jared Wu.		Create it.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#if defined (CONFIG_MACH_MOXA_UC8000) || defined (CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
#include <mach/redboot_config.h>  // for reading software id from redboot configure partition
#include <mach/id.h>  // for UC7420_PLUS_SOFTWARE_ID, ...
#endif


static unsigned long software_id;
static struct proc_dir_entry *proc_swid;

ssize_t proc_swid_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
  int len=0;

  len = sprintf(buffer, "%lx\n", software_id);

  return len;
}

static void __exit swid_exit(void)
{
	remove_proc_entry("driver/swid", proc_swid);
}

static int __init swid_init(void)
{
	printk("Moxa Software ID driver v1.0\n");

#if defined (CONFIG_MACH_MOXA_UC8000) || defined (CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)
	// Add by Jared 06/16/2006. create /proc/driver/swid for software id
	#if 0
	config_get("target_software_id", &software_id);
	#else // Jared, 05-10-2010
	config_get_target_swid(&software_id);
	#endif
#endif	// End: add by Jared
	proc_swid=create_proc_read_entry("driver/swid",0644, NULL, proc_swid_read, NULL);
	if( proc_swid == NULL ) {
		printk("%s[%x]:Can not create /proc/driver/swid file", __FUNCTION__, __LINE__);
		return -ENOMEM;
	}

	return 0;
}

module_init(swid_init);
module_exit(swid_exit);

MODULE_AUTHOR("Jared Wu");
MODULE_LICENSE("GPL");
