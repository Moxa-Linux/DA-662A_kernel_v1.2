#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/debug_moxa.h>
#include <mach/irq.h>
#include <mach/hardware.h>
#include <mach/io.h>
#include <asm/io.h>
#include <mach/irq.h>
#include <mach/moxaart/moxaart.h>
#include <mach/moxaart_pci.h>

#define VICTOR_LOCK_IRQ	// add by Victor Yu. 07-25-2007

static spinlock_t moxaart_int_lock;

static void moxaart_irq_set_mode(unsigned int base_p,unsigned int irq,unsigned int edge)
{	
	if ( edge )
		*(volatile unsigned int *)(base_p+IRQ_MODE_REG)|=(1<<irq);
	else
		*(volatile unsigned int *)(base_p+IRQ_MODE_REG)&=~(1<<irq);	
}	

static void moxaart_irq_set_level(unsigned int base_p,unsigned int irq,unsigned int low)
{	
	if ( low ) 
		*(volatile unsigned int *)(base_p+IRQ_LEVEL_REG)|=(1<<irq);
	else
		*(volatile unsigned int *)(base_p+IRQ_LEVEL_REG)&=~(1<<irq);
}	

static void moxaart_fiq_set_mode(unsigned int base_p,unsigned int fiq,unsigned int edge)
{
    debug_in();
	if ( edge ) 
		*(volatile unsigned int *)(base_p+FIQ_MODE_REG)|=(1<<fiq);
	else
		*(volatile unsigned int *)(base_p+FIQ_MODE_REG)&=~(1<<fiq);
    debug_out();
}	

static void moxaart_fiq_set_level(unsigned int base_p,unsigned int fiq,unsigned int low)
{
    debug_in();
	if ( low ) 
		*(volatile unsigned int *)(base_p+FIQ_LEVEL_REG)|=(1<<fiq);
	else
		*(volatile unsigned int *)(base_p+FIQ_LEVEL_REG)&=~(1<<fiq);
    debug_out();
}	


void moxaart_int_set_irq(unsigned int irq,int mode,int level)
{
	unsigned long   flags;
    
    debugm("irq=%d\n", irq);
	spin_lock_irqsave(&moxaart_int_lock, flags);
   	if ( irq < 32 ) { //irq
#ifdef CONFIG_PCI	// add by Victor Yu. 05-23-2006
		if ( irq == IRQ_PCI )
			goto moxaart_int_set_irq_exit;
#endif
		moxaart_irq_set_mode(MOXAART_IC_VA_BASE,irq ,mode);
		moxaart_irq_set_level(MOXAART_IC_VA_BASE,irq,level);
		goto moxaart_int_set_irq_exit;
   	}		
	if ( irq < 64 ) { //fiq
	   	irq-=32;
#ifdef CONFIG_PCI	// add by Victor Yu. 05-23-2006
		if ( irq == IRQ_PCI )
			goto moxaart_int_set_irq_exit;
#endif
		moxaart_fiq_set_mode(MOXAART_IC_VA_BASE,irq,mode);
		moxaart_fiq_set_level(MOXAART_IC_VA_BASE,irq,level);
		goto moxaart_int_set_irq_exit;
	}

#ifdef CONFIG_PCI
	//pci virtual irq
	if ( (irq==VIRQ_PCI_A)||(irq==VIRQ_PCI_B)||(irq==VIRQ_PCI_C)||(irq==VIRQ_PCI_D) ) {
		if( !moxaart_pci_probed )
			goto moxaart_int_set_irq_exit;
		moxaart_irq_set_mode(MOXAART_IC_VA_BASE,IRQ_PCI,LEVEL);
		moxaart_irq_set_level(MOXAART_IC_VA_BASE,IRQ_PCI,H_ACTIVE);
		//goto moxaart_int_set_irq_exit;
	}
#endif	// CONFIG_PCI

	//printk("Not support irq %d\n",irq);

moxaart_int_set_irq_exit:
	spin_unlock_irqrestore(&moxaart_int_lock, flags);    
    debug_out();
}

static int moxaart_irq_set_type(unsigned int irq, unsigned int flow_type)
{
	int mode, level;

    debugm("irq=%d flow_type=%d\n", irq, flow_type); 
	switch (flow_type){

	case IRQ_TYPE_EDGE_RISING:
		mode = EDGE;
		level = H_ACTIVE;
		break;
	case IRQ_TYPE_EDGE_FALLING:
		mode = EDGE;
		level = L_ACTIVE;                
		break;
	case IRQ_TYPE_LEVEL_HIGH:
		mode = LEVEL;
		level = H_ACTIVE;                
		break;
	case IRQ_TYPE_LEVEL_LOW:
		mode = LEVEL;
		level = L_ACTIVE;
		break;
    /* XXX: add by wade */
    case IRQ_TYPE_PROBE: 
        debugm("get IRQ_TYPE_PROBE, just return 0\n");
        return 0;
	default:
		return -EINVAL;
    }

	moxaart_int_set_irq(irq, mode, level);
	//printk("irq%d mode = %d level=%d\n", irq, mode, level);
    debug_out();
	return 0;
}


static void moxaart_int_clear_irq(unsigned int base,unsigned int irq)
{
	*(volatile unsigned int *)(base+IRQ_CLEAR_REG)=1<<irq;
}

static void moxaart_int_clear_fiq(unsigned int base,unsigned int irq)
{	
	*(volatile unsigned int *)(base+FIQ_CLEAR_REG)=1<<irq;
}

static void moxaart_int_disable_irq(unsigned int base,unsigned int irq)
{
	*(volatile unsigned int *)(base+IRQ_MASK_REG)&=~(1<<irq);
}

static void moxaart_int_disable_fiq(unsigned int base,unsigned int irq)
{
    debug_in();
	*(volatile unsigned int *)(base+FIQ_MASK_REG)&=~(1<<irq);
    debug_out();
}

/*  Turn the interrupt source on. */
static void moxaart_int_enable_irq(unsigned int base,unsigned int irq)
{
	*(volatile unsigned int *)(base+IRQ_MASK_REG)|=(1<<irq);
}

static void moxaart_int_enable_fiq(unsigned int base,unsigned int irq)
{    
	*(volatile unsigned int *)(base+FIQ_MASK_REG)|=(1<<irq);
}

void moxaart_unmask_irq(unsigned int irq)
{
#ifdef VICTOR_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long   flags;
#endif

    if (irq != 19) 
        debugm("irq=%d\n", irq);
#ifdef VICTOR_LOCK_IRQ	// add by Victor Yu. 07-25-2007
    if (irq != 19) 
        debugm("enter lock\n");
	spin_lock_irqsave(&moxaart_int_lock, flags);
    if (irq != 19) 
        debugm("enter critical region\n");
#else
	spin_lock(&moxaart_int_lock);
#endif
	if ( irq < 32 ) { //irq
#ifdef CONFIG_PCI	// add by Victor Yu. 05-23-2006
		if ( irq == IRQ_PCI )
			goto moxaart_unmask_irq_exit;
#endif
		moxaart_int_clear_irq(MOXAART_IC_VA_BASE,irq);
		moxaart_int_enable_irq(MOXAART_IC_VA_BASE,irq);
		goto moxaart_unmask_irq_exit;
	}
	if ( irq < 64 ) { //fiq
		irq-=32;
#ifdef CONFIG_PCI	// add by Victor Yu. 05-23-2006
		if ( irq == IRQ_PCI ) {
            debugm("get pci irq\n");
			goto moxaart_unmask_irq_exit;
        }
#endif
		moxaart_int_clear_fiq(MOXAART_IC_VA_BASE,irq);
		moxaart_int_enable_fiq(MOXAART_IC_VA_BASE,irq);
		goto moxaart_unmask_irq_exit;
	}

#ifdef CONFIG_PCI
	//pci virtual irq
	if( (irq==VIRQ_PCI_A)||(irq==VIRQ_PCI_B)||(irq==VIRQ_PCI_C)||(irq==VIRQ_PCI_D) ) {
		if( !moxaart_pci_probed )
			goto moxaart_unmask_irq_exit;
		moxaart_pci_clear_irq(irq-MOXAART_VIRQ_START);
		moxaart_int_clear_irq(MOXAART_IC_VA_BASE,IRQ_PCI);
		moxaart_int_enable_irq(MOXAART_IC_VA_BASE,IRQ_PCI); //always enabled
		//goto moxaart_unmask_irq_exit;
	}    
#endif	// CONFIG_PCI

moxaart_unmask_irq_exit:
#ifdef VICTOR_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&moxaart_int_lock, flags);    
#else
	spin_unlock(&moxaart_int_lock);    
#endif
    if (irq != 19)
        debug_out();
	return;
}

void moxaart_mask_ack_irq(unsigned int irq)
{
#ifdef VICTOR_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long   flags;
#endif
       
    if (irq != 19)
        debugm("irq=%d\n", irq);
#ifdef VICTOR_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_lock_irqsave(&moxaart_int_lock, flags);
#else
	spin_lock(&moxaart_int_lock);
#endif
	if ( irq < 32 ) {	//irq
#ifdef CONFIG_PCI	// add by Victor Yu. 05-23-2006
		if ( irq == IRQ_PCI )
			goto moxaart_mask_ack_irq_exit;
#endif
		moxaart_int_disable_irq(MOXAART_IC_VA_BASE,irq);
		goto moxaart_mask_ack_irq_exit;
	}

	if ( irq < 64 ) {	//fiq
		irq-=32;
#ifdef CONFIG_PCI	// add by Victor Yu. 05-23-2006
		if ( irq == IRQ_PCI )  {
            debugm("get pci irq\n");
			goto moxaart_mask_ack_irq_exit;
        }
#endif
		moxaart_int_disable_fiq(MOXAART_IC_VA_BASE,irq);
		goto moxaart_mask_ack_irq_exit;
	}

#ifdef CONFIG_PCI
	//pci virtual irq
	if( (irq==VIRQ_PCI_A)||(irq==VIRQ_PCI_B)||(irq==VIRQ_PCI_C)||(irq==VIRQ_PCI_D) ) {
		if( !moxaart_pci_probed )
			goto moxaart_mask_ack_irq_exit;
		moxaart_int_disable_irq(MOXAART_IC_VA_BASE,IRQ_PCI);
		//goto moxaart_mask_ack_irq_exit;
	}    
#endif	// CONFIG_PCI

moxaart_mask_ack_irq_exit:
#ifdef VICTOR_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&moxaart_int_lock, flags);    
#else
	spin_unlock(&moxaart_int_lock);    
#endif
    if (irq != 19)
        debug_out();
	return;
}

void moxaart_int_init(void)
{
#if 1	// add by Victor Yu. 07-26-2007
	unsigned long	flags;
#endif

	spin_lock_init(&moxaart_int_lock);
	//init interrupt controller
#if 1	// add by Victor Yu. 07-26-2007
	spin_lock_irqsave(&moxaart_int_lock, flags);
#endif
	outl(0, MOXAART_IC_VA_BASE+IRQ_MASK_REG);
	outl(0, MOXAART_IC_VA_BASE+FIQ_MASK_REG);
	outl(0xffffffff, MOXAART_IC_VA_BASE+IRQ_CLEAR_REG);
	outl(0xffffffff, MOXAART_IC_VA_BASE+FIQ_CLEAR_REG);
#if 1	// add by Victor Yu. 07-26-2007
	spin_unlock_irqrestore(&moxaart_int_lock, flags);
#endif
#ifdef CONFIG_PCI
	moxaart_int_set_irq(IRQ_PCI, LEVEL, H_ACTIVE);
#endif
}

#if 1	// add by Victor Yu. 05-17-2005
#include <asm/irq.h>
struct irq_chip	moxaart_irq_chip = {
    .name   = "moxaart_irq_chip", 
	.ack	= moxaart_mask_ack_irq,
	.mask	= moxaart_mask_ack_irq,
	.unmask = moxaart_unmask_irq,
	.set_type = moxaart_irq_set_type
};
#endif

EXPORT_SYMBOL(moxaart_int_set_irq);
EXPORT_SYMBOL(moxaart_int_clear_irq);
