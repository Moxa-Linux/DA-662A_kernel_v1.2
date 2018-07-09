/*
 * drivers/mtd/maps/moxaarm11.c
 *
 * MTD Map file for Moxa Tech. ARM11 based systems. Please do not make per-board
 * changes in here.
 *
 * Original Author: Moxa Tech.
 * Maintainer: Victor Yu <victor.yu@moxa.com>
 *
 * Copyright (C) 2010 Moxa Tech.
 *
 */
#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <asm/io.h>
#include <asm/mach/flash.h>
#include <linux/reboot.h>

#define MOXAARM11_CHIP_NUM		2
#define MOXAARM11_CHIP_SIZE		SZ_16M
#define MOXAARM11_CHIP_ADDR_SHIFT	24
#define MOXAARM11_BANK_SIZE		SZ_64M
#define MOXAARM11_BANK_ADDR_SHIFT	26

static inline u16 flash_read16(const void __iomem *addr)
{
	return __raw_readw(addr);
}

static inline void flash_write16(u16 d, void __iomem *addr)
{
	__raw_writew(d, addr);
}

#define	BYTE0(h)	((h) & 0xFF)
#define	BYTE1(h)	(((h) >> 8) & 0xFF)
static map_word moxaarm11_read16(struct map_info *map, unsigned long ofs)
{
	map_word val;
	val.x[0] = flash_read16(map->virt + ofs);
	return val;
}

/*
 * The IXP4xx expansion bus only allows 16-bit wide acceses
 * when attached to a 16-bit wide device (such as the 28F128J3A),
 * so we can't just memcpy_fromio().
 */
static void moxaarm11_copy_from(struct map_info *map, void *to,
			     unsigned long from, ssize_t len)
{
	u8 *dest = (u8 *) to;
	void __iomem *src = map->virt + from;

	if (len <= 0)
		return;

	if (from & 1) {
		*dest++ = BYTE1(flash_read16(src));
                src++;
		--len;
	}

	while (len >= 2) {
		u16 data = flash_read16(src);
		*dest++ = BYTE0(data);
		*dest++ = BYTE1(data);
		src += 2;
		len -= 2;
        }

	if (len > 0)
		*dest++ = BYTE0(flash_read16(src));
}

/*
 * Unaligned writes are ignored, causing the 8-bit
 * probe to fail and proceed to the 16-bit probe (which succeeds).
 */
static void moxaarm11_probe_write16(struct map_info *map, map_word d, unsigned long adr)
{
	if (!(adr & 1))
		flash_write16(d.x[0], map->virt + adr);
}

/*
 * Fast write16 function without the probing check above
 */
static void moxaarm11_write16(struct map_info *map, map_word d, unsigned long adr)
{
	flash_write16(d.x[0], map->virt + adr);
}


struct moxaarm11_flash_info {
	struct mtd_info *mtd[MOXAARM11_CHIP_NUM];
	struct map_info map[MOXAARM11_CHIP_NUM];
	struct mtd_partition *partitions[MOXAARM11_CHIP_NUM];
	struct resource *res;
};
const char *name_str[MOXAARM11_CHIP_NUM]={"NORFlash-0", "NORFlash-1"};

#ifndef CONFIG_MTD_MOXAMACRO_CUSTOMIZED_PARTITION
static struct mtd_partition moxaarm11_system_flash_partitions[] = {
        {
                .name = "BootLoader",   // /dev/mtdblock0
                .size = 0x100000,        // 1M,
                .offset = 0,
        }, {
                .name = "Kernel",       // /dev/mtdblock1
                .size = 0x300000,       // 3M,
                .offset = MTDPART_OFS_APPEND,
        }, {
                .name = "RootDisk",     // /dev/mtdblock2
                .size = 0x0C00000,	// 12M
                .offset = MTDPART_OFS_APPEND,
        }, {
                .name = "UserSpace",    // /dev/mtdblock3
                .size = MTDPART_SIZ_FULL,      
                .offset = MTDPART_OFS_APPEND,

        },
};
#else
static struct mtd_partition moxaarm11_system_flash_partitions[] = {
        {
                .name = "BootLoader",   // /dev/mtdblock0
                .size = CONFIG_MTD_MOXAMACRO_PARTITION_BOOT_LOADER_SIZE,
                .offset = 0,
        }, {
                .name = "Kernel",       // /dev/mtdblock1
                .size = CONFIG_MTD_MOXAMACRO_PARTITION_KERNEL_SIZE,
                .offset = MTDPART_OFS_APPEND,
        }, {
                .name = "RootDisk",     // /dev/mtdblock2
                .size = CONFIG_MTD_MOXAMACRO_PARTITION_ROOT_DISK_SIZE,
                .offset = MTDPART_OFS_APPEND,		
        }, {
		.name = "UserSpace",    // /dev/mtdblock3
#ifdef CONFIG_MTD_MOXAMACRO_PARTITION_USER_DISK_SIZE_AUTO
                .size = MTDPART_SIZ_FULL,
#else
		.size = CONFIG_MTD_MOXAMACRO_PARTITION_USER_DISK_SIZE,
#endif
                .offset = MTDPART_OFS_APPEND,

        },
};	
#endif

#ifndef CONFIG_MTD_CONTINOUS_SPACE
static struct mtd_partition moxaarm11_general_flash_partitions[] = {
	{
                .name = "UserSpace",
                .size = MTDPART_SIZ_FULL,      
                .offset = MTDPART_OFS_APPEND,

        },
};
#endif

static int moxaarm11_flash_remove(struct platform_device *dev)
{
	struct moxaarm11_flash_info *info = platform_get_drvdata(dev);
	int cs;

	platform_set_drvdata(dev, NULL);

	if(!info)
		return 0;

	for (cs = 0; cs < MOXAARM11_CHIP_NUM; cs++) {
		if (info->mtd[cs]) {
			del_mtd_partitions(info->mtd[cs]);
			map_destroy(info->mtd[cs]);
		}
	
		if (info->map[cs].virt)
			iounmap(info->map[cs].virt);
	}

	if (info->res) {
		release_resource(info->res);
		kfree(info->res);
	}

	return 0;
}

static int moxaarm11_flash_probe(struct platform_device *dev)
{
	struct moxaarm11_flash_info	*info;
	int				err = -1;
#ifndef CONFIG_MTD_CONTINOUS_SPACE		
	int 				cs;
#endif

	info = kmalloc(sizeof(struct moxaarm11_flash_info), GFP_KERNEL);
	if(!info) {
		err = -ENOMEM;
		goto Error;
	}
	memset(info, 0, sizeof(struct moxaarm11_flash_info));

	platform_set_drvdata(dev, info);


	info->res = request_mem_region(dev->resource->start,
			dev->resource->end - dev->resource->start + 1,
			"MoxaARM11NORFlash");
	if (!info->res) {
		printk(KERN_ERR "MoxaARM11NORFlash: Could not reserve memory region\n");
		err = -ENOMEM;
		goto Error;
	}

#ifndef CONFIG_MTD_CONTINOUS_SPACE		
	for (cs = 0; cs < MOXAARM11_CHIP_NUM; cs++) {
		/*Key Point: SMC chip select is shifted by one, i.e. flash 0 is mapped to cs1, and flash 1 is mapped to cs2, and so on.*/
		moxaarm11_smc_init((cs+1)%8);
		
		/*
		 * Tell the MTD layer we're not 1:1 mapped so that it does
		 * not attempt to do a direct access on us.
		 */
		info->map[cs].phys = dev->resource->start + (cs * MOXAARM11_BANK_SIZE);
		info->map[cs].size = MOXAARM11_CHIP_SIZE;

		/*
		 * We only support 16-bit accesses for now. If and when
		 * any board use 8-bit access, we'll fixup the driver to
		 * handle that.
		 */
		info->map[cs].bankwidth = 2;
		//info->map[cs].name = dev_name(&dev->dev);
		info->map[cs].name = name_str[cs];
		printk(KERN_DEBUG "%s\n",info->map[cs].name);
		//simple_map_init(&info->map[cs]);

		info->map[cs].read = moxaarm11_read16,
		info->map[cs].write = moxaarm11_probe_write16,
		info->map[cs].copy_from = moxaarm11_copy_from,

		printk(KERN_DEBUG "cs%d, phyaddr=0x%x\n",cs, dev->resource->start + (cs * MOXAARM11_BANK_SIZE));
		info->map[cs].virt = ioremap(dev->resource->start + (cs * MOXAARM11_BANK_SIZE),
					 MOXAARM11_CHIP_SIZE);
		if (!info->map[cs].virt) {
			printk(KERN_ERR "MoxaARM11NORFlash: Failed to ioremap region\n");
			err = -EIO;
			goto Error;
		}
		printk(KERN_DEBUG "info->map[cs].virt=0x%x\n",info->map[cs].virt);
		

		info->mtd[cs] = do_map_probe("cfi_probe", &info->map[cs]);   //Intel chip 
		if (!info->mtd[cs]) {
			printk(KERN_ERR "MoxaARM11NORFlash: map_probe failed\n");
			err = -ENXIO;
			goto Error;
		}
		info->mtd[cs]->owner = THIS_MODULE;


		/* Use the fast version */
		info->map[cs].write = moxaarm11_write16;

		if(cs == 0){
			info->partitions[cs] = moxaarm11_system_flash_partitions;
			err = add_mtd_partitions(info->mtd[cs], info->partitions[cs], ARRAY_SIZE(moxaarm11_system_flash_partitions));	
		}
		else{
			info->partitions[cs] = moxaarm11_general_flash_partitions;
			err = add_mtd_partitions(info->mtd[cs], info->partitions[cs], ARRAY_SIZE(moxaarm11_general_flash_partitions));			
		}
		if(err) {
			printk(KERN_ERR "Could not parse partitions\n");
			goto Error;
		}
	}
#else
	/*Key Point: SMC chip select is shifted by one, i.e. flash 0 is mapped to cs1, and flash 1 is mapped to cs2, and so on.*/
	/*
	for (cs = 0; cs < MOXAARM11_CHIP_NUM; cs++) 
		moxaarm11_smc_init((cs+1)%8);
	*/
	
	/*
	 * Tell the MTD layer we're not 1:1 mapped so that it does
	 * not attempt to do a direct access on us.
	 */
	info->map[0].phys = NO_XIP; //Key Point: For a non-linear address space, don't give the physical address here!!!
	info->map[0].size = dev->resource->end - dev->resource->start + 1;

	/*
	 * We only support 16-bit accesses for now. If and when
	 * any board use 8-bit access, we'll fixup the driver to
	 * handle that.
	 */
	info->map[0].bankwidth = 2;
	info->map[0].name = name_str[0];
	//printk(KERN_DEBUG "%s\n",info->map[0].name);

	info->map[0].read = moxaarm11_read16,
	info->map[0].write = moxaarm11_probe_write16,
	info->map[0].copy_from = moxaarm11_copy_from,

	//printk(KERN_DEBUG "phyaddr=0x%x  size=0x%x\n", (u32)dev->resource->start, (u32)info->map[0].size);
	info->map[0].virt = ioremap(dev->resource->start, info->map[0].size);
	if (!info->map[0].virt) {
		printk(KERN_ERR "MoxaARM11NORFlash: Failed to ioremap region\n");
		err = -EIO;
		goto Error;
	}
	//printk(KERN_DEBUG "info->map[0].virt=0x%x\n",(u32)info->map[0].virt);

	info->mtd[0] = do_map_probe("cfi_probe", &info->map[0]);   //Intel chip 
	if (!info->mtd[0]) {
		printk(KERN_ERR "MoxaARM11NORFlash: map_probe failed\n");
		err = -ENXIO;
		goto Error;
	}
	info->mtd[0]->owner = THIS_MODULE;

	/* Use the fast version */
	info->map[0].write = moxaarm11_write16;

	info->partitions[0] = moxaarm11_system_flash_partitions;
	err = add_mtd_partitions(info->mtd[0], info->partitions[0], ARRAY_SIZE(moxaarm11_system_flash_partitions));	

	if(err) {
		printk(KERN_ERR "Could not parse partitions\n");
		goto Error;
	}
#endif

	return 0;

Error:
	moxaarm11_flash_remove(dev);
	return err;
}

static struct platform_driver moxaarm11_flash_driver = {
	.probe		= moxaarm11_flash_probe,
	.remove		= moxaarm11_flash_remove,
	.driver		= {
		.name	= "MoxaARM11NORFlash",
		.owner	= THIS_MODULE,
	},
};

static int __init moxaarm11_flash_init(void)
{
	return platform_driver_register(&moxaarm11_flash_driver);
}

static void __exit moxaarm11_flash_exit(void)
{
	platform_driver_unregister(&moxaarm11_flash_driver);
}


module_init(moxaarm11_flash_init);
module_exit(moxaarm11_flash_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MTD map driver for Moxa Tech. ARM11 systems");
MODULE_AUTHOR("Victor Yu");
MODULE_ALIAS("platform:MoxaARM11NORFlash");
