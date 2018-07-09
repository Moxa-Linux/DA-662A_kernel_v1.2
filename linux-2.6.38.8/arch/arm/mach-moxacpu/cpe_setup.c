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
#include <mach/cpe/cpe.h>
#include <mach/irq.h>
#include <mach/hardware.h>
#include <asm/sizes.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/nand.h>

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
static struct map_desc moxacpu_io_desc[] __initdata = {
	{CPE_AHB_VA_BASE,	__phys_to_pfn(CPE_AHB_BASE),	SZ_4K, MT_DEVICE},
	{CPE_AHBDMA_VA_BASE,	__phys_to_pfn(CPE_AHBDMA_BASE),	SZ_4K, MT_DEVICE},
	{CPE_APBDMA_VA_BASE,	__phys_to_pfn(CPE_APBDMA_BASE),	SZ_4K, MT_DEVICE},
	{CPE_PMU_VA_BASE,	__phys_to_pfn(CPE_PMU_BASE),	SZ_4K, MT_DEVICE},
	{CPE_TIMER_VA_BASE,	__phys_to_pfn(CPE_TIMER_BASE),	SZ_4K, MT_DEVICE},
	{CPE_GPIO_VA_BASE,	__phys_to_pfn(CPE_GPIO_BASE),	SZ_4K, MT_DEVICE},
	{CPE_IC_VA_BASE,	__phys_to_pfn(CPE_IC_BASE),	SZ_4K, MT_DEVICE},
	{CPE_PCI_VA_BASE,	__phys_to_pfn(CPE_PCI_BASE),	SZ_64K, MT_DEVICE},  
 	{PCI_MEM_VA_BASE,	__phys_to_pfn(CPE_PCI_MEM),	PCI_MEM_SIZE, MT_DEVICE},
	{CPE_UART_VA_BASE,	__phys_to_pfn(CPE_UART_BASE),	SZ_4K, MT_DEVICE},
	{CPE_SPI_VA_BASE,	__phys_to_pfn(CPE_SPI_BASE),	SZ_4K, MT_DEVICE},
	{CPE_EBI_VA_BASE,	__phys_to_pfn(CPE_EBI_BASE),	SZ_4K, MT_DEVICE},
	{CPE_WATCHDOG_VA_BASE,	__phys_to_pfn(CPE_WATCHDOG_BASE),	SZ_4K, MT_DEVICE},
	{CPE_RTC_VA_BASE,	__phys_to_pfn(CPE_RTC_BASE),	SZ_4K, MT_DEVICE},
	{CPE_FTMAC_VA_BASE,	__phys_to_pfn(CPE_FTMAC_BASE),	SZ_4K, MT_DEVICE},
	{CPE_FTMAC2_VA_BASE,	__phys_to_pfn(CPE_FTMAC2_BASE),	SZ_4K, MT_DEVICE},
};

static struct uart_port cpe_serial_ports[] = {
	{
		.iobase		= CPE_UART1_VA_BASE,
		.mapbase	= CPE_UART1_BASE,
		.irq		= IRQ_UART,
		.flags		= UPF_SKIP_TEST | UPF_SHARE_IRQ,
		.iotype		= UPIO_PORT,
		.regshift	= 2,
		.uartclk	= CONFIG_UART_CLK,
		.line		= 0,
		.type		= PORT_16550A,
		.fifosize	= 16
	}, {
		.iobase		= CPE_UART2_VA_BASE,
		.mapbase	= CPE_UART2_BASE,
		.irq		= IRQ_UART,
		.flags		= UPF_SKIP_TEST | UPF_SHARE_IRQ,
		.iotype		= UPIO_PORT,
		.regshift	= 2,
		.uartclk	= CONFIG_UART_CLK,
		.line		= 1,
		.type		= PORT_16550A,
		.fifosize	= 16
	}
};

/* Initail serial and map io */
void __init moxacpu_map_io(void)
{
	early_serial_setup(&cpe_serial_ports[0]);
	early_serial_setup(&cpe_serial_ports[1]);
	iotable_init(moxacpu_io_desc, ARRAY_SIZE(moxacpu_io_desc));
}

static void __init moxacpu_fixup(struct machine_desc *desc, struct tag *tags, char **cmdline, struct meminfo *mi)
{
	/* setup meminfo */
	mi->nr_banks      = 1;
	mi->bank[0].start = 0;
#if defined(CONFIG_ARCH_UC_7112_LX_PLUS) || defined(CONFIG_ARCH_W311) || defined(CONFIG_ARCH_W321) || defined(CONFIG_ARCH_W325) || defined(CONFIG_ARCH_W315)
	mi->bank[0].size  = SZ_32M;
#elif defined(CONFIG_ARCH_IA241_32128) || defined(CONFIG_ARCH_W345_IMP1) || defined(CONFIG_ARCH_IA241_16128)
	mi->bank[0].size  = SZ_128M;
#else	
	mi->bank[0].size  = SZ_64M;
#endif	
}

extern void	cpe_timer_init(void);
static struct sys_timer moxacpu_timer = {
	.init	= cpe_timer_init,
};



/* Platform Device */
// ethernet, SoC 2 MAC
static struct resource moxaart_ethernet_resources[] = {
	[0] = {
		.start	= CPE_FTMAC_BASE,
		.end	= CPE_FTMAC_BASE+4095,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_MAC,
		.end	= IRQ_MAC,
		.flags  = IORESOURCE_IRQ,
	},
	[2] = {
		.start	= CPE_FTMAC2_BASE,
		.end	= CPE_FTMAC2_BASE+4095,
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



// SD host controller
static struct resource mcpu_sd_resources[] = {
	[0] = {
		.start	= CPE_SD_BASE,
		.end	= CPE_SD_BASE+4095,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_SD,
		.end	= IRQ_SD,
		.flags  = IORESOURCE_IRQ,
	},
};
static struct platform_device mcpu_sd_device = {
	.name           = "moxacpu-sd",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(mcpu_sd_resources),
	.resource       = mcpu_sd_resources,
};

// NOR flash
static struct resource mcpu_nor_flash_resources[] = {
	[0] = {
		.start  = CPE_FLASH_BASE,
		.end    = CPE_FLASH_BASE+CPE_FLASH_SZ-1,
		.flags  = IORESOURCE_MEM,
	},
};
static struct platform_device mcpu_nor_flash_device = {
	.name           = "MoxaArtNORFlash",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(mcpu_nor_flash_resources),
	.resource       = mcpu_nor_flash_resources,
};

// USB host, compatiable with EHCI
static u64      mcpu_usb_host_dma_mask=0xffffffffUL;
static struct resource mcpu_usb_host_resources[] = {
	[0] = {
		.start  = CPE_USBHOST_BASE,
		.end    = CPE_USBHOST_BASE+4095,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start  = IRQ_USBHOST,
		.end	= IRQ_USBHOST,
		.flags  = IORESOURCE_IRQ,
	}
};

static struct platform_device mcpu_usb_host_device = {
	.name           = "moxacpu-ehci",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(mcpu_usb_host_resources),
	.dev            = {
		.dma_mask		= &mcpu_usb_host_dma_mask,
		.coherent_dma_mask	= 0xffffffff,
	},
	.resource       = mcpu_usb_host_resources,
};

#ifdef CONFIG_MTD_PARTITIONS
const char *part_probes[] = { "cmdlinepart", NULL };

static struct mtd_partition moxacpu_partitions[] = {
	{
		.name	= "EM1280 NAND FS",
		.offset	= 0,
		.size 	= MTDPART_SIZ_FULL
	}
};
#endif

static struct resource moxacpu_flash_nand_resource = {
	.flags		= IORESOURCE_MEM,
};

static void
moxacpu_flash_nand_cmd_ctrl(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *this = mtd->priv;
	int offset = (int)this->priv;

	if (ctrl & NAND_CTRL_CHANGE) {
		offset = (ctrl & NAND_CLE) ? 0x01 : 0;
		offset |= (ctrl & NAND_ALE) ? 0x02 : 0;
		this->priv = (void *)offset;
	}
	
	if (cmd != NAND_CMD_NONE){
		//printk("this->IO_ADDR_W:%x offset:%x\n",this->IO_ADDR_W,offset);
		writeb(cmd, this->IO_ADDR_W + offset);
	}
}

static struct platform_nand_data moxacpu_flash_nand_data = {
	.chip = {
		.chip_delay		= 30,
		.options		= NAND_NO_AUTOINCR,
#ifdef CONFIG_MTD_PARTITIONS
		.part_probe_types 	= part_probes,
		.partitions	 	= moxacpu_partitions,
		.nr_partitions	 	= ARRAY_SIZE(moxacpu_partitions),
#endif
	},
	.ctrl = {
		.cmd_ctrl 		= moxacpu_flash_nand_cmd_ctrl
	}
};

static struct platform_device moxacpu_flash_nand = {
	.name		= "gen_nand",
	.id		= -1,
	.dev		= {
		.platform_data = &moxacpu_flash_nand_data,
	},
	.num_resources	= 1,
	.resource	= &moxacpu_flash_nand_resource,
};

// cryptor resource description
static struct resource moxaart_crypto_resource[] = {
	[0] = {	// first port description
		.start = CPE_AES_DES_BASE,
		.end   = CPE_AES_DES_BASE + SZ_4K - 1,
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

static struct platform_device *moxacpu_devices[] __initdata = {
	&mcpu_usb_host_device,
	&moxaart_crypto_device,		
	&moxaart_ethernet_device,
	&mcpu_sd_device,
	&mcpu_nor_flash_device,
#if defined(CONFIG_MTD_NAND_PLATFORM) || defined(CONFIG_MTD_NAND_PLATFORM_MODULE)
	&moxacpu_flash_nand,
#endif
};


void __init	moxacpu_init(void)
{
	platform_add_devices(moxacpu_devices, ARRAY_SIZE(moxacpu_devices));
}


/* struct machine_desc */
MACHINE_START(MOXACPU, "Moxa CPU development platform")
	.boot_params = 0x100, 
	.fixup = moxacpu_fixup, 
	.map_io = moxacpu_map_io, /* map physical to virtual address */
	//.init_early
	.init_irq = irq_init_irq, 
	.timer = &moxacpu_timer, 
	.init_machine = moxacpu_init,  /* register  platform_device */
MACHINE_END
