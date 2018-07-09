/* 
 * linux/drivers/mtd/maps/moxaart.c
 *
 * Copyright 2011 Moxa Tech., Inc.
 *	Vincent Wu.<vincent.wu@moxa.com>
 *	http://www.moxa.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *Some vital driver programing key points were desribed as comments 
 *and marked as "Key Point"
 *
*/

#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/slab.h>

#include <linux/errno.h>
#include <linux/init.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <linux/reboot.h>

#include <mach/hardware.h>
#include <mach/io.h>

#ifdef CONFIG_MTD_MOXAART_DEBUG
#define dbg_printk(fmt, s...)	printk("%s,%d:" fmt, __FUNCTION__, __LINE__, ##s)
#else 
#define dbg_printk(fmt, s...)
#endif

struct moxaart_flash_info {
	struct mtd_info *mtd;
	struct map_info map;
	struct mtd_partition *partitions;
	struct resource *res;

	struct notifier_block mtd_notifier;
};

#ifdef CONFIG_MTD_MOXAART_CUSTOMIZED_PARTITION
#define BOOT_LOADER_SIZE 	CONFIG_MTD_MOXAART_PARTITION_BOOT_LOADER_SIZE
#define KERNEL_SIZE			CONFIG_MTD_MOXAART_PARTITION_KERNEL_SIZE
#define ROOT_DISK_SIZE		CONFIG_MTD_MOXAART_PARTITION_ROOT_DISK_SIZE

#ifdef  CONFIG_MTD_MOXAART_PARTITION_USER_DISK_SIZE_AUTO
#define USER_DISK_SIZE		MTDPART_SIZ_FULL
#else
#define USER_DISK_SIZE		CONFIG_MTD_MOXAART_PARTITION_USER_DISK_SIZE 
#endif

#else
#define BOOT_LOADER_SIZE	0x40000
#define KERNEL_BOOT_LOADER_SIZE	0x200000
#define KERNEL_SIZE		(KERNEL_BOOT_LOADER_SIZE-BOOT_LOADER_SIZE)
#define ROOT_DISK_SIZE		0x800000
#define USER_DISK_SIZE		MTDPART_SIZ_FULL //(MOXAART_FLASH_SZ-KERNEL_BOOT_LOADER_SIZE-ROOT_DISK_SIZE)
#endif
static struct mtd_partition moxaart_flash_partitions[] = {
	{
		.name =		"BootLoader",
		.size =		BOOT_LOADER_SIZE,/* hopefully Moxa boot will stay 128k + 128*/
		.offset =	0,
		//.mask_flags =	MTD_WRITEABLE,  /* force read-only */
	}, {
		.name =		"Kernel",
		.size =		KERNEL_SIZE,
		.offset =		MTDPART_OFS_APPEND,
		//.mask_flags =	MTD_WRITEABLE,  /* force read-only */
	}, {
		.name =		"RootDisk",
		.size =		ROOT_DISK_SIZE,
		.offset =		MTDPART_OFS_APPEND,
	}, {
		.name =		"UserDisk",
		.size =		USER_DISK_SIZE,
		.offset = 		MTDPART_OFS_APPEND,
	}
};

static int moxaart_mtd_reboot(struct notifier_block *notifier, unsigned long code, void *p)
{
	struct moxaart_flash_info *info = container_of(notifier, struct moxaart_flash_info, mtd_notifier);
	
        if(code != SYS_RESTART)
                return NOTIFY_DONE;

	//Added by Wade ,06/19/09  
	*( u16 *)(info->map.virt + (0x55 * 2)) = 0xb0;
	*( u16 *)(info->map.virt + (0x55 * 2)) = 0xff;
        return NOTIFY_DONE;
}
EXPORT_SYMBOL(moxaart_mtd_reboot);

static int moxaart_flash_remove(struct platform_device *dev)
{
	struct moxaart_flash_info *info = platform_get_drvdata(dev);

	platform_set_drvdata(dev, NULL);

	if(!info)
		return 0;

	if (info->mtd) {
		del_mtd_partitions(info->mtd);
		map_destroy(info->mtd);
		unregister_reboot_notifier(&info->mtd_notifier);		
	}
	
	if (info->map.virt)
		iounmap(info->map.virt);

	if (info->res) {
		release_resource(info->res);
	}
	
	return 0;
}


static int moxaart_flash_probe(struct platform_device *dev)
{
	struct moxaart_flash_info *info;
	int err = -1;

	info = kmalloc(sizeof(struct moxaart_flash_info), GFP_KERNEL);
	if(!info) {
		err = -ENOMEM;
		goto Error;
	}
	memset(info, 0, sizeof(struct moxaart_flash_info));

	info->res = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if ( !info->res) {
		printk(KERN_ERR "%s: Failed to get the memory resource!\n", dev->name);
		err = -ENODEV;
		goto Error;
	}

	dbg_printk("dev->resource->start=0x%x dev->resource->end=0x%x\n", dev->resource->start, dev->resource->end);
	
	if(!request_mem_region(dev->resource->start, dev->resource->end - dev->resource->start + 1, "MoxaArtNORFlash")){
		printk(KERN_ERR "MoxaArtNORFlash: Could not reserve memory region\n");
		err = -ENOMEM;
		goto Error;
	}
	
	/*
	 * Tell the MTD layer we're not 1:1 mapped so that it does
	 * not attempt to do a direct access on us.
	 */
	//info->map.phys = NO_XIP; //Key Point: For a non-linear address space, don't give the physical address here!!!

	info->map.phys = dev->resource->start;
	info->map.size = dev->resource->end - dev->resource->start + 1;

	/*
	 * We only support 16-bit accesses for now. If and when
	 * any board use 8-bit access, we'll fixup the driver to
	 * handle that.
	 */
	info->map.bankwidth = 2;
	info->map.name = dev_name(&dev->dev);
	simple_map_init(&info->map);	
	dbg_printk("%s\n",info->map.name);


	dbg_printk("phyaddr=0x%x  size=0x%x\n", (u32)dev->resource->start, (u32)info->map.size);
	info->map.virt = ioremap(dev->resource->start, info->map.size);
	if (!info->map.virt) {
		printk(KERN_ERR "MoxaArtNORFlash: Failed to ioremap region\n");
		err = -EIO;
		goto Error;
	}
	dbg_printk("info->map.virt=0x%x\n",(u32)info->map.virt);

	info->mtd = do_map_probe("cfi_probe", &info->map);   //Intel chip 
	if (!info->mtd) {
		printk(KERN_ERR "MoxaArtNORFlash: map_probe failed\n");
		err = -ENXIO;
		goto Error;
	}
	info->mtd->owner = THIS_MODULE;

	dbg_printk("add_mtd_partitions\n");
	info->partitions = moxaart_flash_partitions;
	err = add_mtd_partitions(info->mtd, info->partitions, ARRAY_SIZE(moxaart_flash_partitions));	
	if(err) {
		printk(KERN_ERR "Could not parse partitions\n");
		goto Error;
	}


	info->mtd_notifier.notifier_call = moxaart_mtd_reboot;
	info->mtd_notifier.next = NULL;
	info->mtd_notifier.priority = 0;
	err =  register_reboot_notifier(&info->mtd_notifier);
	if(err) {
		printk(KERN_ERR "Could not register reboot notifier\n");
		goto Error;
	}
	
	return 0;

Error:
	moxaart_flash_remove(dev);
	return err;
}


static struct platform_driver moxaart_flash_driver = {
	.probe		= moxaart_flash_probe,
	.remove		= moxaart_flash_remove,
	.driver		= {
		.name	= "MoxaArtNORFlash",
		.owner	= THIS_MODULE,
	},
};

static int __init moxaart_flash_init(void)
{
	return platform_driver_register(&moxaart_flash_driver);
}

static void __exit moxaart_flash_exit(void)
{
	platform_driver_unregister(&moxaart_flash_driver);
}


module_init(moxaart_flash_init);
module_exit(moxaart_flash_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MTD map driver for MOXA ART systems");
MODULE_AUTHOR("Vincent Wu");
MODULE_ALIAS("platform:MoxaArtNORFlash");
