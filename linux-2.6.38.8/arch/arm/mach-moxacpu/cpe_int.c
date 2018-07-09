#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>

#include <mach/irq.h>
#include <mach/hardware.h>
#include <mach/io.h>
#include <asm/io.h>
#include <mach/irq.h>
//#include <mach/system.h>
#include <mach/cpe/cpe.h>
//#include <mach/ftpci.h>

//#define VICTOR_LOCK_IRQ	// add by Victor Yu. 07-25-2007

static spinlock_t cpe_int_lock;

#if 0	// mask by Victor Yu. 07-25-2007
inline void cpe_irq_set_mode(unsigned int base_p,unsigned int irq,unsigned int edge)
#else
static void cpe_irq_set_mode(unsigned int base_p,unsigned int irq,unsigned int edge)
#endif
{	
	if ( edge )
		*(volatile unsigned int *)(base_p+IRQ_MODE_REG)|=(1<<irq);
	else
		*(volatile unsigned int *)(base_p+IRQ_MODE_REG)&=~(1<<irq);	
}	

#if 0	// mask by Victor Yu. 07-25-2007
inline void cpe_irq_set_level(unsigned int base_p,unsigned int irq,unsigned int low)
#else
static void cpe_irq_set_level(unsigned int base_p,unsigned int irq,unsigned int low)
#endif
{	
	if ( low ) 
		*(volatile unsigned int *)(base_p+IRQ_LEVEL_REG)|=(1<<irq);
	else
		*(volatile unsigned int *)(base_p+IRQ_LEVEL_REG)&=~(1<<irq);
}	

#if 0	// mask by Victor Yu. 07-25-2007
inline void cpe_fiq_set_mode(unsigned int base_p,unsigned int fiq,unsigned int edge)
#else
static void cpe_fiq_set_mode(unsigned int base_p,unsigned int fiq,unsigned int edge)
#endif
{
	if ( edge ) 
		*(volatile unsigned int *)(base_p+FIQ_MODE_REG)|=(1<<fiq);
	else
		*(volatile unsigned int *)(base_p+FIQ_MODE_REG)&=~(1<<fiq);
}	

#if 0	// mask by Victor Yu. 07-25-2007
inline void cpe_fiq_set_level(unsigned int base_p,unsigned int fiq,unsigned int low)
#else
static void cpe_fiq_set_level(unsigned int base_p,unsigned int fiq,unsigned int low)
#endif
{
	if ( low ) 
		*(volatile unsigned int *)(base_p+FIQ_LEVEL_REG)|=(1<<fiq);
	else
		*(volatile unsigned int *)(base_p+FIQ_LEVEL_REG)&=~(1<<fiq);
}	


void cpe_int_set_irq(unsigned int irq,int mode,int level)
{
	unsigned long   flags;
    
	spin_lock_irqsave(&cpe_int_lock, flags);
   	if ( irq < 32 ) { //irq
#ifdef CONFIG_PCI	// add by Victor Yu. 05-23-2006
		if ( irq == IRQ_PCI )
			goto cpe_int_set_irq_exit;
#endif
		cpe_irq_set_mode(CPE_IC_VA_BASE,irq ,mode);
		cpe_irq_set_level(CPE_IC_VA_BASE,irq,level);
		goto cpe_int_set_irq_exit;
   	}		
	if ( irq < 64 ) { //fiq
	   	irq-=32;
#ifdef CONFIG_PCI	// add by Victor Yu. 05-23-2006
		if ( irq == IRQ_PCI )
			goto cpe_int_set_irq_exit;
#endif
		cpe_fiq_set_mode(CPE_IC_VA_BASE,irq,mode);
		cpe_fiq_set_level(CPE_IC_VA_BASE,irq,level);
		goto cpe_int_set_irq_exit;
	}

#ifdef CONFIG_ARCH_CPE
	if ( irq < 96 ) { //a321 irq
	   	irq-=64;
		cpe_irq_set_mode(CPE_A321_IC_VA_BASE,irq,mode);
		cpe_irq_set_level(CPE_A321_IC_VA_BASE,irq,level);
		cpe_irq_set_mode(CPE_IC_VA_BASE,IRQ_EXT_A321,LEVEL);
		cpe_irq_set_level(CPE_IC_VA_BASE,IRQ_EXT_A321,H_ACTIVE);
		goto cpe_int_set_irq_exit;
	}
	if ( irq < 150 ) { //a321 fiq
	   	irq-=96;
		cpe_fiq_set_mode(CPE_A321_IC_VA_BASE,irq,mode);
		cpe_fiq_set_level(CPE_A321_IC_VA_BASE,irq,level);
		cpe_fiq_set_mode(CPE_IC_VA_BASE,IRQ_EXT_A321,LEVEL);
		cpe_fiq_set_level(CPE_IC_VA_BASE,IRQ_EXT_A321,H_ACTIVE);
		goto cpe_int_set_irq_exit;
	}

#ifdef CONFIG_PCI
	//pci virtual irq
	if ( (irq==VIRQ_PCI_A)||(irq==VIRQ_PCI_B)||(irq==VIRQ_PCI_C)||(irq==VIRQ_PCI_D) ) {
		if( !ftpci_probed )
			goto cpe_int_set_irq_exit;
		cpe_irq_set_mode(CPE_A321_IC_VA_BASE,IRQ_A321_PCI,mode);
		cpe_irq_set_level(CPE_A321_IC_VA_BASE,IRQ_A321_PCI,level);
		cpe_irq_set_mode(CPE_IC_VA_BASE,IRQ_EXT_A321,LEVEL);
		cpe_irq_set_level(CPE_IC_VA_BASE,IRQ_EXT_A321,H_ACTIVE);
#if 1	// add by Victor Yu. 10-20-2005
		goto cpe_int_set_irq_exit;
#endif	// 10-20-2005
	}
#endif	// CONFIG_PCI
#endif	// CONFIG_ARCH_CPE

#ifdef CONFIG_ARCH_MOXACPU
#ifdef CONFIG_PCI
	//pci virtual irq
	if ( (irq==VIRQ_PCI_A)||(irq==VIRQ_PCI_B)||(irq==VIRQ_PCI_C)||(irq==VIRQ_PCI_D) ) {
		if( !ftpci_probed )
			goto cpe_int_set_irq_exit;
		cpe_irq_set_mode(CPE_IC_VA_BASE,IRQ_PCI,LEVEL);
		cpe_irq_set_level(CPE_IC_VA_BASE,IRQ_PCI,H_ACTIVE);
		//goto cpe_int_set_irq_exit;
	}
#endif	// CONFIG_PCI
#endif	// CONFIG_ARCH_MOXACPU

	//printk("Not support irq %d\n",irq);

cpe_int_set_irq_exit:
	spin_unlock_irqrestore(&cpe_int_lock, flags);    
}

static void cpe_int_clear_irq(unsigned int base,unsigned int irq)
{
	*(volatile unsigned int *)(base+IRQ_CLEAR_REG)=1<<irq;
}

static void cpe_int_clear_fiq(unsigned int base,unsigned int irq)
{	
	*(volatile unsigned int *)(base+FIQ_CLEAR_REG)=1<<irq;
}

#if 0	// mask by Victor Yu. 07-25-2007
inline void cpe_int_disable_irq(unsigned int base,unsigned int irq)
#else
static void cpe_int_disable_irq(unsigned int base,unsigned int irq)
#endif
{
	*(volatile unsigned int *)(base+IRQ_MASK_REG)&=~(1<<irq);
}

#if 0	// mask by Victor Yu. 07-25-2007
inline void cpe_int_disable_fiq(unsigned int base,unsigned int irq)
#else
static void cpe_int_disable_fiq(unsigned int base,unsigned int irq)
#endif
{
	*(volatile unsigned int *)(base+FIQ_MASK_REG)&=~(1<<irq);
}

/*  Turn the interrupt source on. */
#if 0	// mask by Victor Yu. 07-25-2007
inline void cpe_int_enable_irq(unsigned int base,unsigned int irq)
#else
static void cpe_int_enable_irq(unsigned int base,unsigned int irq)
#endif
{
	*(volatile unsigned int *)(base+IRQ_MASK_REG)|=(1<<irq);
}

#if 0	// mask by Victor Yu. 07-25-2007
inline void cpe_int_enable_fiq(unsigned int base,unsigned int irq)
#else
static void cpe_int_enable_fiq(unsigned int base,unsigned int irq)
#endif
{    
	*(volatile unsigned int *)(base+FIQ_MASK_REG)|=(1<<irq);
}

void cpe_unmask_irq(unsigned int irq)
{
#ifdef VICTOR_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long   flags;
#endif

#ifdef VICTOR_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_lock_irqsave(&cpe_int_lock, flags);
#else
	spin_lock(&cpe_int_lock);
#endif
	if ( irq < 32 ) { //irq
#ifdef CONFIG_PCI	// add by Victor Yu. 05-23-2006
		if ( irq == IRQ_PCI )
			goto cpe_unmask_irq_exit;
#endif
		cpe_int_clear_irq(CPE_IC_VA_BASE,irq);
		cpe_int_enable_irq(CPE_IC_VA_BASE,irq);
		goto cpe_unmask_irq_exit;
	}
	if ( irq < 64 ) { //fiq
		irq-=32;
#ifdef CONFIG_PCI	// add by Victor Yu. 05-23-2006
		if ( irq == IRQ_PCI )
			goto cpe_unmask_irq_exit;
#endif
		cpe_int_clear_fiq(CPE_IC_VA_BASE,irq);
		cpe_int_enable_fiq(CPE_IC_VA_BASE,irq);
		goto cpe_unmask_irq_exit;
	}

#ifdef CONFIG_ARCH_CPE
	if ( irq < 96 ) { //a321 irq
	   	irq-=64;
		cpe_int_clear_irq(CPE_A321_IC_VA_BASE,irq);
		cpe_int_clear_irq(CPE_IC_VA_BASE,IRQ_EXT_A321);
		cpe_int_enable_irq(CPE_A321_IC_VA_BASE,irq);
		cpe_int_enable_irq(CPE_IC_VA_BASE,IRQ_EXT_A321);
		goto cpe_unmask_irq_exit;
	}
	if ( irq < 150 ) { //a321 fiq
	   	irq-=96;
		cpe_int_clear_fiq(CPE_A321_IC_VA_BASE,irq);
		cpe_int_clear_fiq(CPE_IC_VA_BASE,IRQ_EXT_A321);
		cpe_int_enable_fiq(CPE_A321_IC_VA_BASE,irq);
		cpe_int_enable_fiq(CPE_IC_VA_BASE,IRQ_EXT_A321);
		goto cpe_unmask_irq_exit;
	}
    
#ifdef CONFIG_PCI
	//pci virtual irq
	if( (irq==VIRQ_PCI_A)||(irq==VIRQ_PCI_B)||(irq==VIRQ_PCI_C)||(irq==VIRQ_PCI_D) ) {
		if( !ftpci_probed )
			goto cpe_unmask_irq_exit;
		ftpci_clear_irq(irq-150);
		cpe_int_clear_irq(CPE_A321_IC_VA_BASE,IRQ_A321_PCI);
		cpe_int_enable_irq(CPE_A321_IC_VA_BASE,IRQ_A321_PCI); //always enabled
		cpe_int_clear_irq(CPE_IC_VA_BASE,IRQ_EXT_A321);
		cpe_int_enable_irq(CPE_IC_VA_BASE,IRQ_EXT_A321); //always enabled
		goto cpe_unmask_irq_exit;
	}    
#endif	// CONFIG_PCI
#endif	// CONFIG_ARCH_CPE

#ifdef CONFIG_ARCH_MOXACPU
#ifdef CONFIG_PCI
	//pci virtual irq
	if( (irq==VIRQ_PCI_A)||(irq==VIRQ_PCI_B)||(irq==VIRQ_PCI_C)||(irq==VIRQ_PCI_D) ) {
		if( !ftpci_probed )
			goto cpe_unmask_irq_exit;
		ftpci_clear_irq(irq-CPE_VIRQ_START);
		cpe_int_clear_irq(CPE_IC_VA_BASE,IRQ_PCI);
		cpe_int_enable_irq(CPE_IC_VA_BASE,IRQ_PCI); //always enabled
		//goto cpe_unmask_irq_exit;
	}    
#endif	// CONFIG_PCI
#endif	// CONFIG_ARCH_MOXACPU

cpe_unmask_irq_exit:
#ifdef VICTOR_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&cpe_int_lock, flags);    
#else
	spin_unlock(&cpe_int_lock);    
#endif
	return;
}

void cpe_mask_ack_irq(unsigned int irq)
{
#ifdef VICTOR_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long   flags;
#endif
       
#ifdef VICTOR_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_lock_irqsave(&cpe_int_lock, flags);
#else
	spin_lock(&cpe_int_lock);
#endif
	if ( irq < 32 ) {	//irq
#ifdef CONFIG_PCI	// add by Victor Yu. 05-23-2006
		if ( irq == IRQ_PCI )
			goto cpe_mask_ack_irq_exit;
#endif
		cpe_int_disable_irq(CPE_IC_VA_BASE,irq);
		goto cpe_mask_ack_irq_exit;
	}

	if ( irq < 64 ) {	//fiq
		irq-=32;
#ifdef CONFIG_PCI	// add by Victor Yu. 05-23-2006
		if ( irq == IRQ_PCI )
			goto cpe_mask_ack_irq_exit;
#endif
		cpe_int_disable_fiq(CPE_IC_VA_BASE,irq);
		goto cpe_mask_ack_irq_exit;
	}

#ifdef CONFIG_ARCH_CPE
	if ( irq < 96 ) { //a321 irq
	   	irq-=64;
	   	cpe_int_disable_irq(CPE_A321_IC_VA_BASE,irq);
	   	goto cpe_mask_ack_irq_exit;
	}
	if ( irq < 150 ) { //a321 fiq
	   	irq-=96;
	   	cpe_int_disable_fiq(CPE_A321_IC_VA_BASE,irq);
	   	goto cpe_mask_ack_irq_exit;
	}
#ifdef CONFIG_PCI
	//pci virtual irq
	if( (irq==VIRQ_PCI_A)||(irq==VIRQ_PCI_B)||(irq==VIRQ_PCI_C)||(irq==VIRQ_PCI_D) ) {
		if( !ftpci_probed )
			goto cpe_mask_ack_irq_exit;
		cpe_int_disable_irq(CPE_A321_IC_VA_BASE,IRQ_A321_PCI); //always enabled
		goto cpe_mask_ack_irq_exit;
	}    
#endif	// CONFIG_PCI
#endif	// CONFIG_ARCH_CPE

#ifdef CONFIG_ARCH_MOXACPU
#ifdef CONFIG_PCI
	//pci virtual irq
	if( (irq==VIRQ_PCI_A)||(irq==VIRQ_PCI_B)||(irq==VIRQ_PCI_C)||(irq==VIRQ_PCI_D) ) {
		if( !ftpci_probed )
			goto cpe_mask_ack_irq_exit;
		cpe_int_disable_irq(CPE_IC_VA_BASE,IRQ_PCI);
		//goto cpe_mask_ack_irq_exit;
	}    
#endif	// CONFIG_PCI
#endif	// CONFIG_ARCH_MOXACPU

cpe_mask_ack_irq_exit:
#ifdef VICTOR_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&cpe_int_lock, flags);    
#else
	spin_unlock(&cpe_int_lock);    
#endif
	return;
}

#if 0	// mask by Victor Yu. 07-25-2007
static void cpe_mask_irq(unsigned int irq)
{
	cpe_mask_ack_irq(irq);
}
#endif

void cpe_int_init(void)
{
#if 1	// add by Victor Yu. 07-26-2007
	unsigned long	flags;
#endif

	spin_lock_init(&cpe_int_lock);
	//init interrupt controller
#if 1	// add by Victor Yu. 07-26-2007
	spin_lock_irqsave(&cpe_int_lock, flags);
#endif
	outl(0, CPE_IC_VA_BASE+IRQ_MASK_REG);
	outl(0, CPE_IC_VA_BASE+FIQ_MASK_REG);
	outl(0xffffffff, CPE_IC_VA_BASE+IRQ_CLEAR_REG);
	outl(0xffffffff, CPE_IC_VA_BASE+FIQ_CLEAR_REG);
#if 1	// add by Victor Yu. 07-26-2007
	spin_unlock_irqrestore(&cpe_int_lock, flags);
#endif
#ifdef CONFIG_ARCH_MOXACPU
#ifdef CONFIG_PCI
	cpe_int_set_irq(IRQ_PCI, LEVEL, H_ACTIVE);
#if 0	// mask by Victor Yu. 05-23-2006
	cpe_int_enable_irq(CPE_IC_VA_BASE,IRQ_PCI); //always enabled
#endif
#endif
#endif

#ifdef CONFIG_ARCH_CPE
	//init a321 interrupt controller
	outl(0, CPE_A321_IC_VA_BASE+IRQ_MASK_REG);
	outl(0, CPE_A321_IC_VA_BASE+FIQ_MASK_REG);
	outl(0xffffffff, CPE_A321_IC_VA_BASE+IRQ_CLEAR_REG);
	outl(0xffffffff, CPE_A321_IC_VA_BASE+FIQ_CLEAR_REG);
	cpe_int_set_irq(IRQ_EXT_A321, LEVEL, H_ACTIVE);
	cpe_int_enable_irq(CPE_IC_VA_BASE,IRQ_EXT_A321);
#endif	// CONFIG_ARCH_CPE
}

#if 1	// add by Victor Yu. 05-17-2005
#include <asm/irq.h>
struct irq_chip	cpe_irq_chip = {
	.ack	= cpe_mask_ack_irq,
	.mask	= cpe_mask_ack_irq,
	.unmask = cpe_unmask_irq,
};
#endif

EXPORT_SYMBOL(cpe_int_set_irq);
EXPORT_SYMBOL(cpe_int_clear_irq);
