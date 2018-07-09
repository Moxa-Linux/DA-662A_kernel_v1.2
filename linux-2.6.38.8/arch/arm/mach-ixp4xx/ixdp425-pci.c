/*
 * arch/arm/mach-ixp4xx/ixdp425-pci.c
 *
 * IXDP425 board-level PCI initialization
 *
 * Copyright (C) 2002 Intel Corporation.
 * Copyright (C) 2003-2004 MontaVista Software, Inc.
 *
 * Maintainer: Deepak Saxena <dsaxena@plexity.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <asm/mach/pci.h>
#include <asm/irq.h>
#include <mach/hardware.h>
#include <asm/mach-types.h>

#define MAX_DEV		4
#define IRQ_LINES	4

/* PCI controller GPIO to IRQ pin mappings */
#define INTA		11
#define INTB		10
#define INTC		9
#define INTD		8

void __init ixdp425_pci_preinit(void)
{
#ifdef CONFIG_MACH_MOXA_UC8000	// add by Jared Wu. 06-10-2011
	set_irq_type(IRQ_MOXA_UC8000_PCI_INTA, IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IRQ_MOXA_UC8000_PCI_INTB, IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IRQ_MOXA_UC8000_PCI_INTC, IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IRQ_MOXA_UC8000_PCI_INTD, IRQ_TYPE_LEVEL_LOW);
#elif defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)	// add by Jared Wu. 10-04-2012
	set_irq_type(IRQ_MOXA_UC8400_PCI_INTA, IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IRQ_MOXA_UC8400_PCI_INTB, IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IRQ_MOXA_UC8400_PCI_INTC, IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IRQ_MOXA_UC8400_PCI_INTD, IRQ_TYPE_LEVEL_LOW);
#else
	set_irq_type(IXP4XX_GPIO_IRQ(INTA), IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IXP4XX_GPIO_IRQ(INTB), IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IXP4XX_GPIO_IRQ(INTC), IRQ_TYPE_LEVEL_LOW);
	set_irq_type(IXP4XX_GPIO_IRQ(INTD), IRQ_TYPE_LEVEL_LOW);
#endif
	ixp4xx_pci_preinit();
}

static int __init ixdp425_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	static int pci_irq_table[IRQ_LINES] = {

#ifdef CONFIG_MACH_MOXA_UC8000	// add by Victor Yu. 05-16-2008
		IRQ_MOXA_UC8000_PCI_INTA,
		IRQ_MOXA_UC8000_PCI_INTB,
		IRQ_MOXA_UC8000_PCI_INTC,
		IRQ_MOXA_UC8000_PCI_INTD
#elif defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620)	// add by Jared Wu. 10-04-2012
		IRQ_MOXA_UC8400_PCI_INTA,
		IRQ_MOXA_UC8400_PCI_INTB,
		IRQ_MOXA_UC8400_PCI_INTC,
		IRQ_MOXA_UC8400_PCI_INTD
#else
		IXP4XX_GPIO_IRQ(INTA),
		IXP4XX_GPIO_IRQ(INTB),
		IXP4XX_GPIO_IRQ(INTC),
		IXP4XX_GPIO_IRQ(INTD)
#endif
	};

#if defined(CONFIG_MACH_MOXA_UC8400) || defined (CONFIG_MACH_MOXA_UC8480) || defined (CONFIG_MACH_MOXA_DCU8620) // add by Jared Wu. 10-04-2012
	int irq = -1;
	switch(slot) {
		case 1: irq = IRQ_MOXA_UC8400_PCI_INTA;
			break;
		case 2:
	#if defined (CONFIG_MACH_MOXA_DCU8620)  // add by Jared Wu. 10-04-2012
		case 3:
		case 6:
	#endif
			irq = IRQ_MOXA_UC8400_PCI_INTB;
			break;
		case 7: irq = IRQ_MOXA_UC8400_PCI_INTC;
			break;
		case 8: irq = IRQ_MOXA_UC8400_PCI_INTD;
			break;
		default:
			printk("%s[%d]PCI irq map fail. slot:%d, pin:%d\n",__FUNCTION__,__LINE__,slot, pin);
	}
	return irq;
#else

	if (slot >= 1 && slot <= MAX_DEV && pin >= 1 && pin <= IRQ_LINES)
		return pci_irq_table[(slot + pin - 2) % 4];

	return -1;
#endif
}

struct hw_pci ixdp425_pci __initdata = {
	.nr_controllers = 1,
	.preinit	= ixdp425_pci_preinit,
	.swizzle	= pci_std_swizzle,
	.setup		= ixp4xx_setup,
	.scan		= ixp4xx_scan_bus,
	.map_irq	= ixdp425_map_irq,
};

int __init ixdp425_pci_init(void)
{
	if (machine_is_ixdp425() || machine_is_ixcdp1100() ||
			machine_is_ixdp465() || machine_is_kixrp435())
		pci_common_init(&ixdp425_pci);
	return 0;
}

subsys_initcall(ixdp425_pci_init);
