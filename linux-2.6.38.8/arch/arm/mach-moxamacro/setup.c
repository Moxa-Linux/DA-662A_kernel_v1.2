/* 
 * linux/arch/arm/mach-moxaarm11/moxaarm11-setup.c
 *
 * Copyright 2010 Moxa Tech., Inc.
 *	Victor Yu.<victor.yu@moxa.com>
 *	http://www.moxa.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/serial_8250.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/i2c.h>
#include <linux/fb.h>
#include <linux/delay.h>
#include <linux/smsc911x.h>

#include <video/platform_lcd.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <asm/irq.h>
#include <asm/setup.h> //add by Vincent Wu 03-03-2010
#include <asm/mach-types.h>

#include <mach/map.h>
#include <mach/irqs.h>
#include <mach/cpu.h>




static struct uart_port moxaarm11_uart_init[] __initdata = {
	[0] = {
		.membase	= (void *)MOXAARM11_VA_UART1_BASE, // virtual address
		.mapbase	= MOXAARM11_PA_UART1_BASE, // physical address
		.irq		= MOXAARM11_IRQ_UART1,
		.flags		= UPF_SKIP_TEST,
		.iotype		= UPIO_MEM32,
		.regshift	= 2,
		.uartclk	= MOXAARM11_APBCLK,
		//.uartclk	= 30 * 1000000,
		.line		= 0,	// which port number on system
		.type		= PORT_16550A,
		.fifosize 	= 16,
		.irqflags	= IRQF_TRIGGER_HIGH,
	},
	[1] = {
		.membase	= (void *)MOXAARM11_VA_UART2_BASE,// virtual address
		.mapbase	= MOXAARM11_PA_UART2_BASE,// physical address
		.irq		= MOXAARM11_IRQ_UART2,
		.flags		= UPF_SKIP_TEST,
		.iotype		= UPIO_MEM32,
		.regshift	= 2,
		.uartclk	= MOXAARM11_APBCLK,
		//.uartclk	= 30 * 1000000,
		.line		= 1,	// which port number on system
		.type		= PORT_16550A,
		.fifosize 	= 16,
		.irqflags	= IRQF_TRIGGER_HIGH,
	},
};

// NOR flash resource description
static struct resource moxaarm11_nor_flash_resource = {
	.start = MOXAARM11_PA_NOR_BASE,
	.end   = MOXAARM11_PA_NOR_BASE + MOXAARM11_NOR_SIZE  - 1,
	.flags = IORESOURCE_MEM,
};
static struct platform_device moxaarm11_nor_flash  = {
	.name = "MoxaARM11NORFlash",
	.id   = -1,
	.num_resources = 1,
	.resource = &moxaarm11_nor_flash_resource,
};

// NAND flash resource description
static struct resource moxaarm11_nand_flash_resource = {
	.start = MOXAARM11_PA_NAND_BASE,
	.end   = MOXAARM11_PA_NAND_BASE  + SZ_64K - 1,
	.flags = IORESOURCE_MEM,
};
static struct platform_device moxaarm11_nand_flash  = {
	.name = "MoxaARM11NANDFlash",
	.id   = -1,
	.num_resources = 1,
	.resource = &moxaarm11_nand_flash_resource,
};

// USB host resource description
// there are two USB host port embedded
static struct resource moxaarm11_usbhost_resource[] = {
	[0] = {	// first port 
		.start = MOXAARM11_PA_USB1_BASE,
		.end   = MOXAARM11_PA_USB1_BASE  + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MOXAARM11_IRQ_USB1,
		.end   = MOXAARM11_IRQ_USB1,
		.flags = IORESOURCE_IRQ,
	},
	[2] = {	// second port 
		.start = MOXAARM11_PA_USB2_BASE,
		.end   = MOXAARM11_PA_USB2_BASE  + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[3] = {
		.start = MOXAARM11_IRQ_USB2,
		.end   = MOXAARM11_IRQ_USB2,
		.flags = IORESOURCE_IRQ,
	},
};
static u64	moxaarm11_usb_host_dma_mask=0xffffffffUL;
static struct platform_device moxaarm11_usbhost = {
	.name = "MoxaARM11USBHost",
	.id   = -1,
	.num_resources = ARRAY_SIZE(moxaarm11_usbhost_resource),
	.dev	= {
		.dma_mask	= &moxaarm11_usb_host_dma_mask,
		.coherent_dma_mask = 0xffffffff,
	},
	.resource = moxaarm11_usbhost_resource,
};

// SD/MMC resource description
static struct resource moxaarm11_sd_mmc_resource[] = {
	[0] = {
		.start = MOXAARM11_PA_SDMMC_BASE,
		.end   = MOXAARM11_PA_SDMMC_BASE  + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MOXAARM11_IRQ_SDMMC,
		.end   = MOXAARM11_IRQ_SDMMC,
		.flags = IORESOURCE_IRQ,
	},
};
static struct platform_device moxaarm11_sd_mmc = {
	.name = "MoxaARM11SDMMC",
	.id   = -1,
	.num_resources = ARRAY_SIZE(moxaarm11_sd_mmc_resource),
	.resource = moxaarm11_sd_mmc_resource,
};

// RTC resource description
static struct resource moxaarm11_rtc_resource[] = {
	[0] = {
		.start = MOXAARM11_PA_RTC_BASE,
		.end   = MOXAARM11_PA_RTC_BASE  + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MOXAARM11_IRQ_RTC,
		.end   = MOXAARM11_IRQ_RTC,
		.flags = IORESOURCE_IRQ,
	},
};
static struct platform_device moxaarm11_rtc = {
	.name = "MoxaARM11RTC",
	.id   = -1,
	.num_resources = ARRAY_SIZE(moxaarm11_rtc_resource),
	.resource = moxaarm11_rtc_resource,
};

// WatchDog resource description
static struct resource moxaarm11_watchdog_resource[] = {
	[0] = {
		.start = MOXAARM11_PA_WATCHDOG_BASE,
		.end   = MOXAARM11_PA_WATCHDOG_BASE  + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MOXAARM11_IRQ_WATCHDOG,
		.end   = MOXAARM11_IRQ_WATCHDOG,
		.flags = IORESOURCE_IRQ,
	},
};
static struct platform_device moxaarm11_watchdog = {
	.name = "MoxaARM11WatchDog",
	.id   = -1,
	.num_resources = ARRAY_SIZE(moxaarm11_watchdog_resource),
	.resource = moxaarm11_watchdog_resource,
};

// UART resource description
static struct resource moxaarm11_uart_resource[] = {
	[0] = {
		.start = MOXAARM11_PA_UART1_BASE,
		.end   = MOXAARM11_PA_UART1_BASE  + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MOXAARM11_IRQ_UART1,
		.end   = MOXAARM11_IRQ_UART1,
		.flags = IORESOURCE_IRQ,
	},
	[2] = {
		.start = MOXAARM11_PA_UART2_BASE,
		.end   = MOXAARM11_PA_UART2_BASE  + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[3] = {
		.start = MOXAARM11_IRQ_UART2,
		.end   = MOXAARM11_IRQ_UART2,
		.flags = IORESOURCE_IRQ,
	},	
};
static struct platform_device moxaarm11_uart = {
	.name = "MoxaARM11UART",
	.id   = -1,
	.num_resources = ARRAY_SIZE(moxaarm11_uart_resource),
	.resource = moxaarm11_uart_resource,
};

// Timer resource description
static struct resource moxaarm11_timer_resource[] = {
	[0] = {
		.start = MOXAARM11_PA_TIMER_BASE,
		.end   = MOXAARM11_PA_TIMER_BASE  + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MOXAARM11_IRQ_TIMER,
		.end   = MOXAARM11_IRQ_TIMER,
		.flags = IORESOURCE_IRQ,
	},	
};
static struct platform_device moxaarm11_timer = {
	.name = "MoxaARM11Timer",
	.id   = -1,
	.num_resources = ARRAY_SIZE(moxaarm11_timer_resource),
	.resource = moxaarm11_timer_resource,
};

// Crypto resource description
static struct resource moxaarm11_crypto_resource[] = {
	[0] = {
		.start = MOXAARM11_PA_CRYPTO_BASE,
		.end   = MOXAARM11_PA_CRYPTO_BASE  + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MOXAARM11_IRQ_CRYPTO,
		.end   = MOXAARM11_IRQ_CRYPTO,
		.flags = IORESOURCE_IRQ,
	},	
};
static struct platform_device moxaarm11_crypto = {
	.name = "MoxaARM11Crypto",
	.id   = -1,
	.num_resources = ARRAY_SIZE(moxaarm11_crypto_resource),
	.resource = moxaarm11_crypto_resource,
};

// I2C resource description
static struct resource moxaarm11_i2c_resource[] = {
	[0] = {
		.start = MOXAARM11_PA_I2C1_BASE,
		.end   = MOXAARM11_PA_I2C1_BASE  + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MOXAARM11_IRQ_I2C1,
		.end   = MOXAARM11_IRQ_I2C1,
		.flags = IORESOURCE_IRQ,
	},
	[2] = {
		.start = MOXAARM11_PA_I2C2_BASE,
		.end   = MOXAARM11_PA_I2C2_BASE  + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[3] = {
		.start = MOXAARM11_IRQ_I2C2,
		.end   = MOXAARM11_IRQ_I2C2,
		.flags = IORESOURCE_IRQ,
	},	
};
static struct platform_device moxaarm11_i2c = {
	.name = "MoxaARM11I2C",
	.id   = -1,
	.num_resources = ARRAY_SIZE(moxaarm11_i2c_resource),
	.resource = moxaarm11_i2c_resource,
};

// We use pre-define mapping for booting needs.
// GPIO resource description
static struct resource moxaarm11_gpio_resource[] = {
	[0] = {
		.start = MOXAARM11_PA_GPIO_BASE,
		.end   = MOXAARM11_PA_GPIO_BASE  + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MOXAARM11_IRQ_GPIO,
		.end   = MOXAARM11_IRQ_GPIO,
		.flags = IORESOURCE_IRQ,
	},	
};
static struct platform_device moxaarm11_gpio = {
	.name = "MoxaARM11GPIO",
	.id   = -1,
	.num_resources = ARRAY_SIZE(moxaarm11_gpio_resource),
	.resource = moxaarm11_gpio_resource,
};

// SPI resource description
static struct resource moxaarm11_spi_resource[] = {
	[0] = {
		.start = MOXAARM11_PA_SPI_BASE,
		.end   = MOXAARM11_PA_SPI_BASE  + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MOXAARM11_IRQ_SPI,
		.end   = MOXAARM11_IRQ_SPI,
		.flags = IORESOURCE_IRQ,
	},	
};
static struct platform_device moxaarm11_spi = {
	.name = "MoxaARM11SPI",
	.id   = -1,
	.num_resources = ARRAY_SIZE(moxaarm11_spi_resource),
	.resource = moxaarm11_spi_resource,
};

#if 0
// PCIe resource description
static struct resource moxaarm11_pcie_resource[] = {
	[0] = {
		.start = MOXAARM11_PA_PCIE_BASE,
		.end   = MOXAARM11_PA_PCIE_BASE  + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MOXAARM11_IRQ_PCIE,
		.end   = MOXAARM11_IRQ_PCIE,
		.flags = IORESOURCE_IRQ,
	},	
};
static struct platform_device moxaarm11_pcie = {
	.name = "MoxaARM11PCIe",
	.id   = -1,
	.num_resources = ARRAY_SIZE(moxaarm11_pcie_resource),
	.resource = moxaarm11_pcie_resource,
};
#endif

// MoxaUART resource description
// there are 4 ports emebedded, and included interrupt vector I/O 
// (offset 0xc0) & interface I/O address (offset 0xe0)
static struct resource moxaarm11_moxauart_resource[] = {
	[0] = {
		.start = MOXAARM11_PA_MOXAUART_BASE,
		.end   = MOXAARM11_PA_MOXAUART_BASE  + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MOXAARM11_IRQ_MOXAUART,
		.end   = MOXAARM11_IRQ_MOXAUART,
		.flags = IORESOURCE_IRQ,
	},	
};
static struct platform_device moxaarm11_moxauart = {
	.name = "MoxaARM11MOXAUART",
	.id   = -1,
	.num_resources = ARRAY_SIZE(moxaarm11_moxauart_resource),
	.resource = moxaarm11_moxauart_resource,
};

// FRT resource description
static struct resource moxaarm11_frt_resource[] = {
	[0] = {
		.start = MOXAARM11_PA_FRT_BASE,
		.end   = MOXAARM11_PA_FRT_BASE  + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MOXAARM11_IRQ_FRT,
		.end   = MOXAARM11_IRQ_FRT,
		.flags = IORESOURCE_IRQ,
	},
};
static struct platform_device moxaarm11_frt = {
	.name = "MoxaARM11FRT",
	.id   = -1,
	.num_resources = ARRAY_SIZE(moxaarm11_frt_resource),
	.resource = moxaarm11_frt_resource,
};

// GMAC1/2 (garbit ethernet) resource description
// there are two ports here
static struct resource moxaarm11_gmac_resource[] = {
	[0] = {	// first port description
		.start = MOXAARM11_PA_GMAC1_BASE,
		.end   = MOXAARM11_PA_GMAC1_BASE + SZ_8K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MOXAARM11_IRQ_GMAC1,
		.end   = MOXAARM11_IRQ_GMAC1,
		.flags = IORESOURCE_IRQ,
	},
	[2] = {
		.start = MOXAARM11_IRQ_WAKE_GMAC1,
		.end   = MOXAARM11_IRQ_WAKE_GMAC1,
		.flags = IORESOURCE_IRQ,
	},	
	[3] = {	// second port description
		.start = MOXAARM11_GMAC2_BASE,
		.end   = MOXAARM11_GMAC2_BASE + SZ_8K - 1,
		.flags = IORESOURCE_MEM,
	},
	[4] = {
		.start = MOXAARM11_IRQ_GMAC2,
		.end   = MOXAARM11_IRQ_GMAC2,
		.flags = IORESOURCE_IRQ,
	},
	[5] = {
		.start = MOXAARM11_IRQ_WAKE_GMAC2,
		.end   = MOXAARM11_IRQ_WAKE_GMAC2,
		.flags = IORESOURCE_IRQ,
	},	
};
static struct platform_device moxaarm11_gmac = {
	.name = "MoxaARM11GMAC",
	.id   = -1,
	.num_resources = ARRAY_SIZE(moxaarm11_gmac_resource),
	.resource = moxaarm11_gmac_resource,
};

#if 0
// Ethernet resource description
static struct resource moxaarm11_ethernet_resource[] = {
	[0] = {	// first port description
		.start = MOXAARM11_PA_ETHERNET_BASE,
		.end   = MOXAARM11_PA_ETHERNET_BASE + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = MOXAARM11_IRQ_ETHERNET,
		.end   = MOXAARM11_IRQ_ETHERNET,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device moxaarm11_ethernet = {
	.name = "MoxaARM11Ethernet",
	.id   = -1,
	.num_resources = ARRAY_SIZE(moxaarm11_ethernet_resource),
	.resource = moxaarm11_ethernet_resource,
};

// external RTC
static struct platform_device moxaarm11_external_rtc = {
        .name = "MoxaARM11ExternalRTC",
        .id   = -1,
        .num_resources = 0,
        .resource = NULL,
};
#endif

#if 1
// i2c-gpio
#include <linux/i2c-gpio.h>
#include <linux/i2c.h>
static struct i2c_gpio_platform_data	i2c_gpio_pdata = {
	.sda_pin		= 13,
	.scl_pin		= 12,
	.scl_is_output_only	= 1,
};
static struct platform_device	i2c_gpio = {
	.name		= "i2c-gpio",
	.dev		= {
		.platform_data	= &i2c_gpio_pdata,
	},
	.id		= -1,
	.num_resources	= 0,
	.resource	= NULL,
};

static struct i2c_board_info 	i2c_gpio_board_info[] = {
	{
	.type	= "isl1208",
	.addr	= 0x6f,
	},
};
#endif

// Moxa ARM11 platform all componment devices resource description
static struct platform_device *moxaarm11_devices[] __initdata = {
	&moxaarm11_nor_flash,
	&i2c_gpio,
	&moxaarm11_nand_flash,
	&moxaarm11_usbhost,		
	&moxaarm11_sd_mmc,	
	&moxaarm11_rtc,
	//&moxaarm11_external_rtc,
	&moxaarm11_uart,
	&moxaarm11_timer,
	&moxaarm11_watchdog,	// mask by Victor Yu. 07-01-2010
	&moxaarm11_crypto,
	&moxaarm11_i2c,
	&moxaarm11_gpio,	// mask by Vicor Yu. 07-12-2010
	&moxaarm11_spi,	
//	&moxaarm11_pcie,	//mask by Vincent Wu. 03-14-2011
	&moxaarm11_moxauart,	
	&moxaarm11_frt,		
	&moxaarm11_gmac,
	//&moxaarm11_ethernet,
};

static struct map_desc moxaarm11_iodesc[] = {
	{	// SoC first serial port 
		.virtual        = MOXAARM11_VA_UART1_BASE,
        	.pfn            = __phys_to_pfn(MOXAARM11_PA_UART1_BASE),
        	.length         = SZ_4K,
        	.type           = MT_DEVICE
	},	
	{	// SoC second serial port
		.virtual        = MOXAARM11_VA_UART2_BASE,
        	.pfn            = __phys_to_pfn(MOXAARM11_PA_UART2_BASE),
        	.length         = SZ_4K,
        	.type           = MT_DEVICE
	},
	{	// SoC interrrupt vector
		.virtual        = MOXAARM11_VA_VIC_BASE,
        	.pfn            = __phys_to_pfn(MOXAARM11_PA_VIC_BASE),
        	.length         = SZ_4K,
        	.type           = MT_DEVICE
	},
	{	// SoC timer
		.virtual        = MOXAARM11_VA_TIMER_BASE,
        	.pfn            = __phys_to_pfn(MOXAARM11_PA_TIMER_BASE),
        	.length         = SZ_4K,
        	.type           = MT_DEVICE
	},
	{	// PCI Host
		.virtual        = MOXAARM11_VA_PCI_CFG_BASE,
        	.pfn            = __phys_to_pfn(MOXAARM11_PA_PCI_CFG_BASE),
        	.length         = SZ_4K,
        	.type           = MT_DEVICE
	},	
	{	// PCI-Express Host
		.virtual        = MOXAARM11_VA_PCIE_BASE,
        	.pfn            = __phys_to_pfn(MOXAARM11_PA_PCIE_BASE),
        	.length         = SZ_4K,
        	.type           = MT_DEVICE
	},
	{	// PCI-Express Configuration
		.virtual        = MOXAARM11_VA_PCIE_CFG_BASE,
        	.pfn            = __phys_to_pfn(MOXAARM11_PA_PCIE_CFG_BASE),
        	.length         = SZ_64K,
        	.type           = MT_DEVICE
	},	
	{	// PCI-Express IO
		.virtual        = MOXAARM11_VA_PCIE_IO_BASE,
        	.pfn            = __phys_to_pfn(MOXAARM11_PA_PCIE_IO_BASE),
        	.length         = SZ_64K,
        	.type           = MT_DEVICE
	},		
#if 1	// add by Victor Yu. 07-01-2010
	{	// watchdog Configuration
		.virtual        = MOXAARM11_VA_WATCHDOG_BASE,
        	.pfn            = __phys_to_pfn(MOXAARM11_PA_WATCHDOG_BASE),
        	.length         = SZ_4K,
        	.type           = MT_DEVICE
	},	
	{	// GPIO Base Configuration
		.virtual        = MOXAARM11_VA_GPIO_BASE,
        	.pfn            = __phys_to_pfn(MOXAARM11_PA_GPIO_BASE),
        	.length         = SZ_4K,
        	.type           = MT_DEVICE
	},	
	{	// system control Base Configuration
		.virtual        = MOXAARM11_VA_SMC_BASE,
        	.pfn            = __phys_to_pfn(MOXAARM11_PA_SMC_BASE),
        	.length         = SZ_4K,
        	.type           = MT_DEVICE
	},
	{	// system control Base Configuration
		.virtual        = MOXAARM11_VA_SYSTEM_BASE,
        	.pfn            = __phys_to_pfn(MOXAARM11_PA_SYSTEM_BASE),
        	.length         = SZ_4K,
        	.type           = MT_DEVICE
	},	
#endif
#if 1	// used by GMAC device driver
	{	// NOR Flash address of the system flash
		.virtual        = MOXAARM11_VA_NOR_BASE,
        	.pfn            = __phys_to_pfn(MOXAARM11_PA_NOR_BASE),
        	.length         = SZ_16M,
        	.type           = MT_DEVICE
	},
#endif
};

static void __init moxaarm11_map_io(void)
{
	iotable_init(moxaarm11_iodesc, ARRAY_SIZE(moxaarm11_iodesc));	// first to map address
#ifdef CONFIG_SERIAL_8250	
	early_serial_setup(&moxaarm11_uart_init[0]);	// embedded UART port
	early_serial_setup(&moxaarm11_uart_init[1]);	// embedded UART port
#endif	
}

static void __init moxaarm11_machine_init(void)
{
	platform_add_devices(moxaarm11_devices, ARRAY_SIZE(moxaarm11_devices));	
	i2c_register_board_info(0, i2c_gpio_board_info, 1);
}

static void __init moxaarm11_fixup (struct machine_desc *desc, struct tag *tags,
	      char **cmdline, struct meminfo *mi)  //add by Vincent Wu 03-03-2010
{
	/*
	 * Bank start addresses are not present in the information
	 * passed in from the boot loader.  We could potentially
	 * detect them, but instead we hard-code them.
	 */
#ifdef CONFIG_MOXAMACRO_DDR2_SDRAM
	mi->bank[0].start = PHYS_OFFSET;
	mi->bank[0].size = 128*1024*1024;
	mi->nr_banks = 1;
#else	
	mi->bank[0].start = PHYS_OFFSET;
	mi->bank[0].size = 64*1024*1024;
	mi->bank[1].start = PHYS_OFFSET+64*1024*1024;
	mi->bank[1].size = 64*1024*1024;
	mi->nr_banks = 2;
#endif	
}

MACHINE_START(MOXAMACRO, "MOXAMACRO")
	.boot_params = 0x100, 
	.init_irq	= moxaarm11_init_irq,
	.map_io		= moxaarm11_map_io,
	.fixup      	= moxaarm11_fixup,	
	.init_machine	= moxaarm11_machine_init,
	.timer		= &moxaarm11_timer_fn,
MACHINE_END
