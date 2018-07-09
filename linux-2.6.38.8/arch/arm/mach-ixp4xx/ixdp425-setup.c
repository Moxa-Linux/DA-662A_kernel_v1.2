/*
 * arch/arm/mach-ixp4xx/ixdp425-setup.c
 *
 * IXDP425/IXCDP1100 board-setup
 *
 * Copyright (C) 2003-2005 MontaVista Software, Inc.
 *
 * Author: Deepak Saxena <dsaxena@plexity.net>
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/serial.h>
#include <linux/tty.h>
#include <linux/serial_8250.h>
#include <linux/i2c-gpio.h>
#include <linux/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/delay.h>
#include <asm/types.h>
#include <asm/setup.h>
#include <asm/memory.h>
#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/irq.h>
#include <asm/mach/arch.h>
#include <asm/mach/flash.h>

//#define UC8430
#define UC8481_NAND_ON_CS2 /* Jared, 04-11-2012, use CS#2 for NAND flash */

#if 0 // Jared, unused
#define IXDP425_SDA_PIN		7
#define IXDP425_SCL_PIN		6
#endif

/* NAND Flash pins */
#if !(defined UC8481_NAND_ON_CS2) /* Jared, 04-11-2012, use CS#2 for NAND flash*/
#define IXDP425_NAND_NCE_PIN	9 /* Jared, 06-15-2011, for NAND control */
#endif

#define IXDP425_NAND_CMD_BYTE	0x01
#define IXDP425_NAND_ADDR_BYTE	0x02

#if (defined CONFIG_MACH_MOXA_UC8480) || (defined CONFIG_MACH_MOXA_DCU8620) // Add by Jared 06-09-2011
static struct mtd_partition uc8400partition[] = {
	{
		.name = "BootLoader", // Bootloader, mtd0
	#if (defined UC8430)
		.size = 0x000A0000,
	#else
		.size = 0x00100000,
	#endif
		.offset = 0,
		.mask_flags = 0,
	},
	{
		.name = "Kernel", // kernel, mtd1
		.size = 0x001E0000, 
		.offset = MTDPART_OFS_APPEND, // 0x100000
		.mask_flags = 0,
	},
	{
		.name = "RootDisk", // RootDisk, mtd2
	#if (defined UC8430)
		.size = 0x01d60000,
	#else
		.size = 0x01d00000, 
	#endif
		.offset = MTDPART_OFS_APPEND, //0x002E0000
		.mask_flags = 0,
	},
	{
		.name = "Redboot FIS", // Redboot FIS and Config, mtd3
		.size = 0x00020000, 
		.offset = MTDPART_OFS_APPEND, // 0x01fe0000
		.mask_flags = 0,
	}
};
#elif (defined CONFIG_MACH_MOXA_UC8400) /* For old UC-8410/8416/8418-LX */
static struct mtd_partition uc8400partition[] = {
	{
		.name = "BootLoader", // Bootloader, mtd0
		.size = 0x000a0000, 
		.offset = 0,
		.mask_flags = 0,
	},
	{
		.name = "Kernel", // kernel, mtd1
		.size = 0x001E0000, 
		.offset = MTDPART_OFS_APPEND, // 0xA0000
		.mask_flags = 0,
	},
	{
		.name = "RootDisk", // RootDisk, mtd2
		.size = 0x00d60000, 
		.offset = MTDPART_OFS_APPEND, //0x00280000
		.mask_flags = 0,
	},
	{
		.name = "Redboot FIS", // Redboot FIS and Config, mtd3
		.size = 0x00020000, 
		.offset = MTDPART_OFS_APPEND, // 0x01000000
		.mask_flags = 0,
	},
};
#endif

static struct flash_platform_data ixdp425_flash_data = {
	.map_name	= "cfi_probe",
	.width		= 2,
#if 1 // Add by Jared 06-09-2011
#ifdef CONFIG_MTD_PARTITIONS
	.parts	 	= uc8400partition,
	.nr_parts	= ARRAY_SIZE(uc8400partition),
#endif
#endif
};

static struct resource ixdp425_flash_resource = {
#if (defined CONFIG_MACH_MOXA_UC8480) || (defined CONFIG_MACH_MOXA_DCU8620) // Add by Jared 06-09-2011
	.start		= IXP4XX_EXP_BUS_BASE(0),
	.end		= SZ_32M - 1,
#elif (defined CONFIG_MACH_MOXA_UC8400) /* For old UC-8410/8416/8418-LX */
	.start		= IXP4XX_EXP_BUS_BASE(0),
	.end		= SZ_16M - 1,
#endif
	.flags		= IORESOURCE_MEM,
};

static struct platform_device ixdp425_flash = {
	.name		= "IXP4XX-Flash",
	.id		= 0,
	.dev		= {
		.platform_data = &ixdp425_flash_data,
	},
	.num_resources	= 1,
	.resource	= &ixdp425_flash_resource,
};

#if defined(CONFIG_MTD_NAND_PLATFORM) || \
    defined(CONFIG_MTD_NAND_PLATFORM_MODULE)

#ifdef CONFIG_MTD_PARTITIONS
const char *part_probes[] = { "cmdlinepart", NULL };

#if 0	// mask by Victor Yu. 01-25-2008
static struct mtd_partition ixdp425_partitions[] = {
	{
		.name	= "ixp400 NAND FS 0",
		.offset	= 0,
		.size 	= SZ_8M
	}, {
		.name	= "ixp400 NAND FS 1",
		.offset	= MTDPART_OFS_APPEND,
		.size	= MTDPART_SIZ_FULL
	},
};
#else /* For old UC-8410/8416/8418-LX */
static struct mtd_partition ixdp425_nand_partition[] = {
	#if (defined CONFIG_MACH_MOXA_DCU8620)
	{
		.name	= "UserDisk", // mtd 4
		.size 	= SZ_64M,
		.offset	= MTDPART_OFS_APPEND,
		.mask_flags = 0,
	},
	{
		.name	= "OptDisk", // mtd 5
		.size 	= MTDPART_SIZ_FULL,
		.offset	= MTDPART_OFS_APPEND,
		.mask_flags = 0,
	}
	#else
	{
		.name	= "UserDisk", // mtd 4
		.size 	= MTDPART_SIZ_FULL,	// SZ_512M,
		.offset = MTDPART_OFS_APPEND,	// 0x02000000,
		.mask_flags = 0,
	}
	#endif
};
#endif
#endif

static void
ixdp425_flash_nand_cmd_ctrl(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *this = mtd->priv;
	int offset = (int)this->priv;

	if (ctrl & NAND_CTRL_CHANGE) {
#if !(defined UC8481_NAND_ON_CS2) /* Jared, 04-11-2012, use CS#2 for NAND flash*/
		if (ctrl & NAND_NCE) {
			gpio_line_set(IXDP425_NAND_NCE_PIN, IXP4XX_GPIO_LOW);
			//udelay(5); // mask by Victor Yu. 04-29-2009
		} else {
			gpio_line_set(IXDP425_NAND_NCE_PIN, IXP4XX_GPIO_HIGH);
		}
#endif

		offset = (ctrl & NAND_CLE) ? IXDP425_NAND_CMD_BYTE : 0;
		offset |= (ctrl & NAND_ALE) ? IXDP425_NAND_ADDR_BYTE : 0;
		this->priv = (void *)offset;
	}
	
	if (cmd != NAND_CMD_NONE)
		writeb(cmd, this->IO_ADDR_W + offset);
}

static struct platform_nand_data ixdp425_flash_nand_data = {
	.chip = {
		.nr_chips		= 1, 
		.chip_delay		= 30,
		.options		= NAND_NO_AUTOINCR,
#ifdef CONFIG_MTD_PARTITIONS
		.part_probe_types 	= part_probes,
#if (defined CONFIG_MACH_MOXA_UC8480) || (defined CONFIG_MACH_MOXA_DCU8620) /* Add by Jared 06-09-2011 */
		.partitions	 	= ixdp425_nand_partition,
		.nr_partitions	 	= ARRAY_SIZE(ixdp425_nand_partition),
#elif (defined CONFIG_MACH_MOXA_UC8400) /* For old UC-8410/8416/8418-LX */
		.partitions	 	= ixdp425_partitions,
		.nr_partitions	 	= ARRAY_SIZE(ixdp425_partitions),
#endif
#endif
	},
	.ctrl = {
		.cmd_ctrl 		= ixdp425_flash_nand_cmd_ctrl
	}
};

static struct resource ixdp425_flash_nand_resource = {
	.flags		= IORESOURCE_MEM,
};

static struct platform_device ixdp425_flash_nand = {
	.name		= "gen_nand",
	.id		= -1,
	.dev		= {
		.platform_data = &ixdp425_flash_nand_data,
	},
	.num_resources	= 1,
	.resource	= &ixdp425_flash_nand_resource,
};
#endif	/* CONFIG_MTD_NAND_PLATFORM */

#if 0 // Jared, unused
static struct i2c_gpio_platform_data ixdp425_i2c_gpio_data = {
	.sda_pin	= IXDP425_SDA_PIN,
	.scl_pin	= IXDP425_SCL_PIN,
};

static struct platform_device ixdp425_i2c_gpio = {
	.name		= "i2c-gpio",
	.id		= 0,
	.dev	 = {
		.platform_data	= &ixdp425_i2c_gpio_data,
	},
};
#endif
static struct resource ixdp425_uart_resources[] = {
	{
		.start		= IXP4XX_UART1_BASE_PHYS,
		.end		= IXP4XX_UART1_BASE_PHYS + 0x0fff,
		.flags		= IORESOURCE_MEM
	},
#if 0 // Jared, unused
	{
		.start		= IXP4XX_UART2_BASE_PHYS,
		.end		= IXP4XX_UART2_BASE_PHYS + 0x0fff,
		.flags		= IORESOURCE_MEM
	}
#endif
};

static struct plat_serial8250_port ixdp425_uart_data[] = {
	{
		.mapbase	= IXP4XX_UART1_BASE_PHYS,
		.membase	= (char *)IXP4XX_UART1_BASE_VIRT + REG_OFFSET,
		.irq		= IRQ_IXP4XX_UART1,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
		.iotype		= UPIO_MEM,
		.regshift	= 2,
		.uartclk	= IXP4XX_UART_XTAL,
	},
#if 0 // Jared, unused
	{
		.mapbase	= IXP4XX_UART2_BASE_PHYS,
		.membase	= (char *)IXP4XX_UART2_BASE_VIRT + REG_OFFSET,
		.irq		= IRQ_IXP4XX_UART2,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
		.iotype		= UPIO_MEM,
		.regshift	= 2,
		.uartclk	= IXP4XX_UART_XTAL,
	},
#endif
	{ },
};

static struct platform_device ixdp425_uart = {
	.name			= "serial8250",
	.id			= PLAT8250_DEV_PLATFORM,
	.dev.platform_data	= ixdp425_uart_data,
#if 0 // Jared, unused
	.num_resources		= 2,
#else
	.num_resources		= 1,
#endif
	.resource		= ixdp425_uart_resources
};

/* Built-in 10/100 Ethernet MAC interfaces */
static struct eth_plat_info ixdp425_plat_eth[] = {
	{
		.phy		= 16, /* original 0, Jared 06-09-2011 */
		.rxq		= 2,
		.txreadyq	= 19,
	}, {
		.phy		= 2, /* original 1, Jared 06-09-2011 */
		.rxq		= 4,
		.txreadyq	= 21,
	}
};

static struct platform_device ixdp425_eth[] = {
	{
		.name			= "ixp4xx_eth",
		.id			= IXP4XX_ETH_NPEA,
		.dev.platform_data	= ixdp425_plat_eth,
	}, {
		.name			= "ixp4xx_eth",
		.id			= IXP4XX_ETH_NPEC,
		.dev.platform_data	= ixdp425_plat_eth + 1,
	}
};

static struct platform_device *ixdp425_devices[] __initdata = {
#if 0 // Jared, unused
	&ixdp425_i2c_gpio,
#endif
	&ixdp425_flash,
#if defined(CONFIG_MTD_NAND_PLATFORM) || \
    defined(CONFIG_MTD_NAND_PLATFORM_MODULE)
	&ixdp425_flash_nand,
#endif
	&ixdp425_uart,
	&ixdp425_eth[0],
	&ixdp425_eth[1],
};

static void __init ixdp425_init(void)
{
	ixp4xx_sys_init();

	ixdp425_flash_resource.start = IXP4XX_EXP_BUS_BASE(0);
	ixdp425_flash_resource.end =
#if (defined CONFIG_MACH_MOXA_UC8480) || (defined CONFIG_MACH_MOXA_DCU8620) /* Add by Jared 06-09-2011 */
		IXP4XX_EXP_BUS_BASE(0) + (ixp4xx_exp_bus_size*2) - 1;
#elif (defined CONFIG_MACH_MOXA_UC8400) /* For old UC-8410/8416/8418-LX */
		IXP4XX_EXP_BUS_BASE(0) + ixp4xx_exp_bus_size - 1;
#endif

#if defined(CONFIG_MTD_NAND_PLATFORM) || \
    defined(CONFIG_MTD_NAND_PLATFORM_MODULE)
#if (defined UC8481_NAND_ON_CS2) /* Jared, 04-11-2012, use CS#2 for NAND flash*/
	ixdp425_flash_nand_resource.start = IXP4XX_EXP_BUS_BASE(2),
	ixdp425_flash_nand_resource.end   = IXP4XX_EXP_BUS_BASE(2) + 0x10 - 1;
	#if 0   /* mask by Jared Wu. 04-12-2012, bootload has set it */
	*IXP4XX_EXP_CS2 = IXP4XX_EXP_BUS_CS_EN |
			  IXP4XX_EXP_BUS_STROBE_T(1) |	/* extend by 1 clock */
			  IXP4XX_EXP_BUS_CYCLES(0) |	/* Intel cycles */
			  IXP4XX_EXP_BUS_SIZE(0) |	/* 512bytes addr space*/
			  IXP4XX_EXP_BUS_WR_EN |
			  IXP4XX_EXP_BUS_BYTE_EN;	/* 8 bit data bus */
	#endif
#else
	#if 1 // mapped to 0x53000000
	ixdp425_flash_nand_resource.start = IXP4XX_EXP_BUS_BASE(3),
	ixdp425_flash_nand_resource.end   = IXP4XX_EXP_BUS_BASE(3) + 0x10 - 1;
	gpio_line_config(IXDP425_NAND_NCE_PIN, IXP4XX_GPIO_OUT);
	#else // mapped to 0x53000100
	ixdp425_flash_nand_resource.start = IXP4XX_EXP_BUS_BASE(3) + 0x100,
	ixdp425_flash_nand_resource.end   = IXP4XX_EXP_BUS_BASE(3) + 0x100 + 0x10 - 1;
	gpio_line_config(IXDP425_NAND_NCE_PIN, IXP4XX_GPIO_OUT);
	#endif

#endif
#endif

	if (cpu_is_ixp43x()) {
		ixdp425_uart.num_resources = 1;
		ixdp425_uart_data[1].flags = 0;
	}

	platform_add_devices(ixdp425_devices, ARRAY_SIZE(ixdp425_devices));
}

#ifdef CONFIG_ARCH_IXDP425
MACHINE_START(IXDP425, "Intel IXDP425 Development Platform")
	/* Maintainer: MontaVista Software, Inc. */
	.map_io		= ixp4xx_map_io,
	.init_irq	= ixp4xx_init_irq,
	.timer		= &ixp4xx_timer,
	.boot_params	= 0x0100,
	.init_machine	= ixdp425_init,
MACHINE_END
#endif

#ifdef CONFIG_MACH_IXDP465
MACHINE_START(IXDP465, "Intel IXDP465 Development Platform")
	/* Maintainer: MontaVista Software, Inc. */
	.map_io		= ixp4xx_map_io,
	.init_irq	= ixp4xx_init_irq,
	.timer		= &ixp4xx_timer,
	.boot_params	= 0x0100,
	.init_machine	= ixdp425_init,
MACHINE_END
#endif

#ifdef CONFIG_ARCH_PRPMC1100
MACHINE_START(IXCDP1100, "Intel IXCDP1100 Development Platform")
	/* Maintainer: MontaVista Software, Inc. */
	.map_io		= ixp4xx_map_io,
	.init_irq	= ixp4xx_init_irq,
	.timer		= &ixp4xx_timer,
	.boot_params	= 0x0100,
	.init_machine	= ixdp425_init,
MACHINE_END
#endif

#ifdef CONFIG_MACH_KIXRP435
MACHINE_START(KIXRP435, "Intel KIXRP435 Reference Platform")
	/* Maintainer: MontaVista Software, Inc. */
	.map_io		= ixp4xx_map_io,
	.init_irq	= ixp4xx_init_irq,
	.timer		= &ixp4xx_timer,
	.boot_params	= 0x0100,
	.init_machine	= ixdp425_init,
MACHINE_END
#endif
