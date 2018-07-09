
/*
 *	History:
 *	Date		Author			Comment
 *	11-14-2005	Victor Yu.		Create it.
 */
//#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/cpufreq.h>
#include <linux/ioport.h>

#include <asm/div64.h>
//#include <asm/hardware.h>
#include <linux/platform_device.h>
#include <asm/system.h>
#include <asm/pgtable.h>
#include <asm/mach/map.h>
#include <asm/irq.h>

#ifdef CONFIG_ARCH_MOXACPU
static u64	mcpu_usb_host_dma_mask=0xffffffffUL;
static struct resource mcpu_usb_host_resources[] = {
	{
		.start	= IO_ADDRESS(CPE_USBHOST_BASE),
		.end	= IO_ADDRESS(CPE_USBHOST_BASE+4096),
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= IRQ_USBHOST,
		.flags	= IORESOURCE_IRQ,
	}
};
static struct platform_device mcpu_usb_host_device = {
	.name		= "ehci",
	.id		= -1,
	.dev		= {
		.dma_mask	= &mcpu_usb_host_dma_mask,
		.coherent_dma_mask = 0xffffffff,
	},
	.num_resources	= ARRAY_SIZE(mcpu_usb_host_resources),
	.resource	= mcpu_usb_host_resources,
};

static struct platform_device *ia240_devices[] __initdata = {
	&mcpu_usb_host_device,
};

static int __init	ia240_init(void)
{
	return platform_add_devices(ia240_devices, ARRAY_SIZE(ia240_devices));
}

arch_initcall(ia240_init);
#endif	// CONFIG_ARCH_MOXACPU
