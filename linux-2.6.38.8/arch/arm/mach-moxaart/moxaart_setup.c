/*
 * History:
 * Date		Author			Comment
 * 05-12-2005	Victor Yu.		Create it.
 */
#include <linux/init.h>
#include <linux/device.h>
#include <linux/serial.h>
#include <linux/tty.h>
#include <linux/serial_core.h>
#include <linux/serial_8250.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/interrupt.h>
#include <asm/elf.h>
#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/mach/map.h>
#include <mach/moxaart/moxaart.h>
#include <mach/irq.h>
#include <mach/hardware.h>
#include <asm/sizes.h>
#include <linux/mtd/partitions.h>

/*
 * XXX: linux suggest map io to Fxxxxxx, 
 * and leave 0xC0000000 0xD000000, 0XE000000 for other use 
 * to see momory map in datasheet 
 *
 * Following is used Moxa CPU demo board
 * Physical     Logical
 * 90100000	f9010000	AHB controller
 * 90200000	f9020000	SMC
 * 90300000	f9030000	SDRAM controller
 * 90400000	f9040000	DMA
 * 90500000	f9050000	APB bridge
 * 90b00000	f90b0000	USB 2.0 device
 * 90c00000	f90c0000	PCI bridge
 * 92300000	f9230000	EBI
 * 98100000	f9810000	PMU (power management)
 * 98200000	f9820000	UART (port 1 - 6), embedded on CPU
 * 98400000	f9840000	timer #1 & #2
 * 98500000	f9850000	watchdog timer
 * 98600000	f9860000	RTC, embedded on CPU
 * 98700000	f9870000	GPIO
 * 98800000	f9880000	INTC (interrupt controller)
 * 98b00000	f98b0000	SPI
 * 98e00000	f98e0000	SD controller
 * 99400000	f9940000	AC97
 * a0000000	a0000000	PCI memory
 */

/* struct map_desc
 * virtual address
 * page frame number: use __phys_to_pfn 
 * length
 * type
 */
static struct map_desc moxaart_io_desc[] __initdata = {
	{MOXAART_AHB_VA_BASE,	__phys_to_pfn(MOXAART_AHB_BASE),	SZ_4K, MT_DEVICE},
	{MOXAART_AHBDMA_VA_BASE,	__phys_to_pfn(MOXAART_AHBDMA_BASE),	SZ_4K, MT_DEVICE},
	{MOXAART_APBDMA_VA_BASE,	__phys_to_pfn(MOXAART_APBDMA_BASE),	SZ_4K, MT_DEVICE},
	{MOXAART_PMU_VA_BASE,	__phys_to_pfn(MOXAART_PMU_BASE),	SZ_4K, MT_DEVICE},
	{MOXAART_TIMER_VA_BASE,	__phys_to_pfn(MOXAART_TIMER_BASE),	SZ_4K, MT_DEVICE},
	{MOXAART_GPIO_VA_BASE,	__phys_to_pfn(MOXAART_GPIO_BASE),	SZ_4K, MT_DEVICE},
	{MOXAART_IC_VA_BASE,	__phys_to_pfn(MOXAART_IC_BASE),	SZ_4K, MT_DEVICE},
	{MOXAART_PCI_VA_BASE,	__phys_to_pfn(MOXAART_PCI_BASE),	SZ_64K, MT_DEVICE},  
 	{PCI_MEM_VA_BASE,	__phys_to_pfn(MOXAART_PCI_MEM),	PCI_MEM_SIZE, MT_DEVICE},
	{MOXAART_UART_VA_BASE,	__phys_to_pfn(MOXAART_UART_BASE),	SZ_4K, MT_DEVICE},
	{MOXAART_SPI_VA_BASE,	__phys_to_pfn(MOXAART_SPI_BASE),	SZ_4K, MT_DEVICE},
	{MOXAART_EBI_VA_BASE,	__phys_to_pfn(MOXAART_EBI_BASE),	SZ_4K, MT_DEVICE},
	{MOXAART_RTC_VA_BASE,	__phys_to_pfn(MOXAART_RTC_BASE),	SZ_4K, MT_DEVICE},
	{MOXAART_WATCHDOG_VA_BASE,	__phys_to_pfn(MOXAART_WATCHDOG_BASE),	SZ_4K, MT_DEVICE},
//	{MOXAART_FTMAC_VA_BASE,	__phys_to_pfn(MOXAART_FTMAC_BASE),	SZ_4K, MT_DEVICE},
//	{MOXAART_FTMAC2_VA_BASE,	__phys_to_pfn(MOXAART_FTMAC2_BASE),	SZ_4K, MT_DEVICE},
};

static struct uart_port moxaart_serial_ports[] = {
	{
		.iobase		= MOXAART_UART1_VA_BASE,
		.mapbase	= MOXAART_UART1_BASE,
		.irq		= IRQ_UART,
		.flags		= UPF_SKIP_TEST | UPF_SHARE_IRQ,
		.irqflags	= IRQF_TRIGGER_HIGH,
		.iotype		= UPIO_PORT,
		.regshift	= 2,
		.uartclk	= CONFIG_UART_CLK,
		.line		= 0,
		.type		= PORT_16550A,
		.fifosize	= 16
	}, {
		.iobase		= MOXAART_UART2_VA_BASE,
		.mapbase	= MOXAART_UART2_BASE,
		.irq		= IRQ_UART,
		.flags		= UPF_SKIP_TEST | UPF_SHARE_IRQ,
		.irqflags	= IRQF_TRIGGER_HIGH,
		.iotype		= UPIO_PORT,
		.regshift	= 2,
		.uartclk	= CONFIG_UART_CLK,
		.line		= 1,
		.type		= PORT_16550A,
		.fifosize	= 16
	}
};

/* Initail serial and map io */
void __init moxaart_map_io(void)
{
	early_serial_setup(&moxaart_serial_ports[0]);
	early_serial_setup(&moxaart_serial_ports[1]);
	iotable_init(moxaart_io_desc, ARRAY_SIZE(moxaart_io_desc));
}

static void __init moxaart_fixup(struct machine_desc *desc, struct tag *tags, char **cmdline, struct meminfo *mi)
{
	/* setup meminfo */
	mi->nr_banks      = 1;
	mi->bank[0].start = 0;
	mi->bank[0].size  = CONFIG_ARCH_MOXAART_DRAM_SIZE;
}

extern void	moxaart_timer_init(void);
static struct sys_timer moxaart_timer = {
	.init	= moxaart_timer_init,
};



/* Platform Device */
// ethernet, SoC 2 MAC
static struct resource moxaart_ethernet_resources[] = {
	[0] = {
		.start	= MOXAART_FTMAC_BASE,
		.end	= MOXAART_FTMAC_BASE+4095,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_MAC,
		.end	= IRQ_MAC,
		.flags  = IORESOURCE_IRQ,
	},
	[2] = {
		.start	= MOXAART_FTMAC2_BASE,
		.end	= MOXAART_FTMAC2_BASE+4095,
		.flags  = IORESOURCE_MEM,
	},
	[3] = {
		.start	= IRQ_MAC2,
		.end	= IRQ_MAC2,
		.flags  = IORESOURCE_IRQ,
	},
};
static struct platform_device moxaart_ethernet_device = {
	.name           = "MOXA Art MAC Driver",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(moxaart_ethernet_resources),
	.resource       = moxaart_ethernet_resources,
};


// GPIO
#if 0
static struct resource moxaart_gpio_resources[] = {

	[0] = {
		.start	= MOXAART_GPIO_BASE,
		.end	= MOXAART_GPIO_BASE+4095,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_GPIO,
		.end	= IRQ_GPIO,
		.flags  = IORESOURCE_IRQ,
	},

};
#endif
static struct platform_device moxaart_gpio_device = {
	.name           = "MOXA Art GPIO Driver",
	.id             = -1,
	.num_resources  = 0,
	.resource       = NULL,
};



// SD host controller
static struct resource moxaart_sd_resources[] = {
	[0] = {
		.start	= MOXAART_SD_BASE,
		.end	= MOXAART_SD_BASE+4095,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_SD,
		.end	= IRQ_SD,
		.flags  = IORESOURCE_IRQ,
	},
};
static struct platform_device moxaart_sd_device = {
	.name           = "MoxaArtSd",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(moxaart_sd_resources),
	.resource       = moxaart_sd_resources,
};

// Watchdog
static struct resource moxaart_watchdog_resources[] = {
	[0] = {
		.start  = MOXAART_WATCHDOG_BASE,
		.end    = MOXAART_WATCHDOG_BASE + 0x1b, 
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_WATCHDOG, 
		.end	= IRQ_WATCHDOG, 
		.flags  = IORESOURCE_IRQ,
	},
};
static struct platform_device moxaart_watchdog_device = {
	.name           = "MoxaArtWatchdog",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(moxaart_watchdog_resources),
	.resource       = moxaart_watchdog_resources,
};

// RTC
static struct platform_device moxaart_rtc_device = {
	.name           = "MoxaArtRTC",
	.id             = -1,
	.num_resources  = 0, 
	.resource       = NULL, 
};

// NOR flash
static struct resource moxaart_nor_flash_resources[] = {
	[0] = {
		.start  = MOXAART_FLASH_BASE,
		.end    = MOXAART_FLASH_BASE+MOXAART_FLASH_SZ-1,
		.flags  = IORESOURCE_MEM,
	},
};
static struct platform_device moxaart_nor_flash_device = {
	.name           = "MoxaArtNORFlash",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(moxaart_nor_flash_resources),
	.resource       = moxaart_nor_flash_resources,
};

// USB host, compatiable with EHCI
static u64      moxaart_usb_host_dma_mask=0xffffffffUL;
static struct resource moxaart_usb_host_resources[] = {
	[0] = {
		.start  = MOXAART_USBHOST_BASE,
		.end    = MOXAART_USBHOST_BASE+4095,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start  = IRQ_USBHOST,
		.end	= IRQ_USBHOST,
		.flags  = IORESOURCE_IRQ,
	}
};

static struct platform_device moxaart_usb_host_device = {
	.name           = "MoxaArtUSBHost",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(moxaart_usb_host_resources),
	.dev            = {
		.dma_mask		= &moxaart_usb_host_dma_mask,
		.coherent_dma_mask	= 0xffffffff,
	},
	.resource       = moxaart_usb_host_resources,
};

// cryptor resource description
static struct resource moxaart_crypto_resource[] = {
	[0] = {	// first port description
		.start = MOXAART_AES_DES_BASE,
		.end   = MOXAART_AES_DES_BASE + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_AES_DES,
		.end   = IRQ_AES_DES,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device moxaart_crypto_device = {
	.name		= "MoxaArtCrypto",
	.id		= -1,
	.num_resources = ARRAY_SIZE(moxaart_crypto_resource),
	.resource	= moxaart_crypto_resource,
};

static struct platform_device *moxaart_devices[] __initdata = {
	&moxaart_usb_host_device,
	&moxaart_crypto_device,		
	&moxaart_ethernet_device,
	&moxaart_sd_device,
	&moxaart_nor_flash_device,
	&moxaart_watchdog_device,
    &moxaart_rtc_device,
	&moxaart_gpio_device,
};


void __init moxaart_init(void)
{
	platform_add_devices(moxaart_devices, ARRAY_SIZE(moxaart_devices));
}


/* struct machine_desc */
MACHINE_START(MOXAART, "Moxa CPU development platform")
	.boot_params = 0x100, 
	.fixup = moxaart_fixup, 
	.map_io = moxaart_map_io, /* map physical to virtual address */
	//.init_early
	.init_irq = irq_init_irq, 
	.timer = &moxaart_timer, 
	.init_machine = moxaart_init,  /* register  platform_device */
MACHINE_END
