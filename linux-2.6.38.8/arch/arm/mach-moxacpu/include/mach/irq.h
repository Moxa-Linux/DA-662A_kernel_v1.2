/* asm/arch-cpe/irq.h */

#ifndef __ASM_ARCH_IRQ_H__
#define __ASM_ARCH_IRQ_H__

#include <mach/hardware.h>
#include <mach/io.h>
#include <mach/irqs.h>

#include <asm/irq.h>

//set_irq_chip()
//include linux/irq.h
#include <asm/mach/irq.h>

//#include <mach/ftpci.h>
#include "cpe/cpe.h"

extern void cpe_mask_irq(unsigned int irq);
extern void cpe_unmask_irq(unsigned int irq);
extern void cpe_mask_ack_irq(unsigned int irq);
extern void cpe_clear_irq(unsigned int intNum);
extern void cpe_int_init(void);

#if 1	// add by Victor Yu. 05-17-2005
extern struct irq_chip	cpe_irq_chip;
//#include <linux/interrupt.h>
#endif

static struct resource irq_resource = {
	.name	= "irq_handler",
	.start	= CPE_IC_VA_BASE, 
	.end	= CPE_IC_VA_BASE + FIQ_STATUS_REG + 4,
};

static inline void irq_init_irq(void)
{
#if 0
	unsigned long flags;
	int irq;

	save_and_cli(flags);
	/* initailize */
	cpe_int_init();
	restore_flags(flags);

	for (irq = 0; irq < NR_IRQS; irq++) {
		/* register ack/mask/umask function  */
		set_irq_chip(irq, &cpe_irq_chip);
		set_irq_handler(irq, do_level_IRQ);
		set_irq_flags(irq, IRQF_VALID);
	}
#endif 
	//unsigned int i, mode = 0, level = 0;
	unsigned int i; 

	/*
	 * Disable arch_idle() by default since it is buggy
	 * For more info see arch/arm/mach-gemini/include/mach/system.h
	 */
	disable_hlt();

	request_resource(&iomem_resource, &irq_resource);

	for (i = 0; i < NR_IRQS; i++) {
		/* register ack/mask/umask function  */
		set_irq_chip(i, &cpe_irq_chip);
#if 0
		if((i >= IRQ_TIMER1 && i <= IRQ_TIMER3) || (i >= IRQ_SERIRQ0 && i <= IRQ_SERIRQ1)) {
			set_irq_handler(i, handle_edge_irq);
			mode |= 1 << i;
			level |= 1 << i;
		} else {			
			set_irq_handler(i, handle_level_irq);
		}
#endif
		/* set all as level trigger */
		set_irq_handler(i, handle_level_irq);
		//set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
		//TODO: add IRQF_PROBE? 
		set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
	}

#if 0
	/* Disable all interrupts */
	__raw_writel(0, IRQ_MASK(CPE_IC_VA_BASE));
	__raw_writel(0, FIQ_MASK(CPE_IC_VA_BASE));

	/* Set interrupt mode */
	__raw_writel(mode, IRQ_TMODE(CPE_IC_VA_BASE));
	__raw_writel(level, IRQ_TLEVEL(CPE_IC_VA_BASE));
#endif
}

static inline int fixup_irq(int irq)
{
#ifdef CONFIG_ARCH_CPE
	unsigned int status;
	unsigned int i;

	if( irq == IRQ_EXT_A321 ) {
		status=*(volatile unsigned int *)(CPE_A321_IC_VA_BASE+IRQ_STATUS_REG);
		if( status & (1<<(IRQ_A321_PCI-CPE_A321_IRQ_START)) ) { //pci irq
#ifdef CONFIG_PCI
		switch ( ftpci_get_irq() ) {
                case 0: return VIRQ_PCI_A;
                case 1: return VIRQ_PCI_B;
                case 2: return VIRQ_PCI_C;
                case 3: return VIRQ_PCI_D;
		}
#endif	// CONFIG_PCI
        	for( i=0; i<32 ;i++ ) {
			if ( status & (1<<i) )
                		return (i+CPE_A321_IRQ_START);
		}
        }
        
    }
#endif	// CONFIG_ARCH_CPE

#ifdef CONFIG_ARCH_MOXACPU
#ifdef CONFIG_PCI
	if ( irq == IRQ_PCI ) {
		switch ( ftpci_get_irq() ) {
                case 0: return VIRQ_PCI_A;
                case 1: return VIRQ_PCI_B;
                case 2: return VIRQ_PCI_C;
                case 3: return VIRQ_PCI_D;
		}
	}
#endif	// CONFIG_PCI
#endif	// CONFIG_ARCH_MOXACPU

    return irq;
}

#endif /* __ASM_ARCH_IRQ_H__ */
