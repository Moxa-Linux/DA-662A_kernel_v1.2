#ifndef __ASM_ARCH_IRQ_H__
#define __ASM_ARCH_IRQ_H__

#include <mach/hardware.h>
#include <mach/io.h>
#include <mach/irqs.h>

#include <asm/irq.h>
#include <asm/mach/irq.h>

#include <mach/moxaart/moxaart.h>
#include <mach/moxaart_pci.h>
//#define DEBUG_MOXA
#include <linux/debug_moxa.h>

extern void moxaart_mask_irq(unsigned int irq);
extern void moxaart_unmask_irq(unsigned int irq);
extern void moxaart_mask_ack_irq(unsigned int irq);
extern void moxaart_clear_irq(unsigned int intNum);
extern void moxaart_int_init(void);

extern struct irq_chip	moxaart_irq_chip;

static struct resource irq_resource = {
	.name	= "irq_handler",
	.start	= MOXAART_IC_VA_BASE, 
	.end	= MOXAART_IC_VA_BASE + FIQ_STATUS_REG + 4,
};

static inline void irq_init_irq(void)
{
	unsigned int i; 

	/*
	 * Disable arch_idle() by default since it is buggy
	 * For more info see arch/arm/mach-gemini/include/mach/system.h
	 */
	disable_hlt();

	request_resource(&iomem_resource, &irq_resource);


	debugm("NR_IRQS=%d\n", NR_IRQS);
	for (i = 0; i < NR_IRQS; i++) {
		debugm("set irq=%d chip_name=%s\n", i, (&moxaart_irq_chip)->name);

		/* register ack/mask/umask function  */
		set_irq_chip(i, &moxaart_irq_chip);

		/* set all as level trigger */
		set_irq_handler(i, handle_level_irq);

		//set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
		//TODO: add IRQF_PROBE? 
		set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
	}
}

static inline int fixup_irq(int irq)
{
#ifdef CONFIG_PCI
	if ( irq == IRQ_PCI ) {
		switch ( moxaart_pci_get_irq() ) {
                case 0: return VIRQ_PCI_A;
                case 1: return VIRQ_PCI_B;
                case 2: return VIRQ_PCI_C;
                case 3: return VIRQ_PCI_D;
		}
	}
#endif	// CONFIG_PCI

    return irq;
}

#endif /* __ASM_ARCH_IRQ_H__ */
