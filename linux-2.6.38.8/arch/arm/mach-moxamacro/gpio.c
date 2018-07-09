
#include	<linux/module.h>
#include	<linux/kernel.h>
#include	<linux/types.h>
#include	<linux/spinlock.h>
#include	<linux/io.h>
#include	<mach/map.h>
#include	<mach/irqs.h>
#include	<mach/gpio.h>

#define VICTOR_USE_LOCK_IRQ
static mcpu_gpio_reg_t	*gpio_reg=(mcpu_gpio_reg_t *)MOXAARM11_VA_GPIO_BASE;
static spinlock_t	gpio_lock=SPIN_LOCK_UNLOCKED;

void	mcpu_gpio_inout(u32 gpio, int inout)
{
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long	flags;
#endif

#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif
	switch ( inout ) {
	case MCPU_GPIO_INPUT :
		writel(readl(&gpio_reg->direction)&~gpio, &gpio_reg->direction);
		break;
	case MCPU_GPIO_OUTPUT :
		writel(readl(&gpio_reg->direction)|gpio, &gpio_reg->direction);
		break;
	}
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&gpio_lock, flags);
#else
	spin_unlock(&gpio_lock);
#endif
}
EXPORT_SYMBOL(mcpu_gpio_inout);

u32	mcpu_gpio_get_inout(u32 gpio)
{
	u32	ret;
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long	flags;
#endif

#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif
	if ( readl(&gpio_reg->direction) & gpio )
		ret = MCPU_GPIO_OUTPUT;
	else
		ret = MCPU_GPIO_INPUT;
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&gpio_lock, flags);
#else
	spin_unlock(&gpio_lock);
#endif
	return ret;
}
EXPORT_SYMBOL(mcpu_gpio_get_inout);

void	mcpu_gpio_set(u32 gpio, int highlow)
{
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long	flags;
#endif

#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif
	switch ( highlow ) {
	case MCPU_GPIO_HIGH :
		writel(readl(&gpio_reg->data)|gpio, &gpio_reg->data);
		break;
	case MCPU_GPIO_LOW :
		writel(readl(&gpio_reg->data)&~gpio, &gpio_reg->data);
		break;
	}
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&gpio_lock, flags);
#else
	spin_unlock(&gpio_lock);
#endif
}
EXPORT_SYMBOL(mcpu_gpio_set);

u32	mcpu_gpio_get(u32 gpio)
{
	u32	ret;
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long	flags;
#endif

#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif
	ret = readl(&gpio_reg->data) & gpio;
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&gpio_lock, flags);
#else
	spin_unlock(&gpio_lock);
#endif
	return ret;
}
EXPORT_SYMBOL(mcpu_gpio_get);

void	mcpu_gpio_set_to_irq(u32 gpio, vic_intr_trig_mode_E mode)
{
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long	flags;
#endif

#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif

	switch ( mode ) {
	case vicc_edge_activeFalling :
		writel(readl(&gpio_reg->interrupt_sensitivity_control)&~gpio, &gpio_reg->interrupt_sensitivity_control);
		writel(readl(&gpio_reg->interrupt_both_edge)&~gpio, &gpio_reg->interrupt_both_edge);
		writel(readl(&gpio_reg->interrupt_event)&~gpio, &gpio_reg->interrupt_event);
		break;
	case vicc_edge_activeRaising :
		writel(readl(&gpio_reg->interrupt_sensitivity_control)&~gpio, &gpio_reg->interrupt_sensitivity_control);
		writel(readl(&gpio_reg->interrupt_both_edge)&~gpio, &gpio_reg->interrupt_both_edge);
		writel(readl(&gpio_reg->interrupt_event)|gpio, &gpio_reg->interrupt_event);
		break;
	case vicc_edge_activeBoth :
		writel(readl(&gpio_reg->interrupt_sensitivity_control)&~gpio, &gpio_reg->interrupt_sensitivity_control);
		writel(readl(&gpio_reg->interrupt_both_edge)|gpio, &gpio_reg->interrupt_both_edge);
		break;
	case vicc_level_activeHigh :
		writel(readl(&gpio_reg->interrupt_sensitivity_control)|gpio, &gpio_reg->interrupt_sensitivity_control);
		writel(readl(&gpio_reg->interrupt_event)|gpio, &gpio_reg->interrupt_event);
		break;
	case vicc_level_activeLow :
		writel(readl(&gpio_reg->interrupt_sensitivity_control)|gpio, &gpio_reg->interrupt_sensitivity_control);
		writel(readl(&gpio_reg->interrupt_event)&~gpio, &gpio_reg->interrupt_event);
		break;
	}
	writel(readl(&gpio_reg->interrupt_enable)|gpio, &gpio_reg->interrupt_enable);

#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&gpio_lock, flags);
#else
	spin_unlock(&gpio_lock);
#endif
}
EXPORT_SYMBOL(mcpu_gpio_set_to_irq);

void	mcpu_gpio_clear_irq_pending(u32 gpio)
{
#ifdef VICTOR_USE_LOCK_IRQ	
	spin_lock(&gpio_lock);
#endif
	writel(readl(&gpio_reg->interrupt_clear)|gpio, &gpio_reg->interrupt_clear);

#ifdef VICTOR_USE_LOCK_IRQ	
	spin_unlock(&gpio_lock);
#endif
}
EXPORT_SYMBOL(mcpu_gpio_clear_irq_pending);

void	mcpu_gpio_mask_pin(u32 gpio , int on_off)
{
#ifdef VICTOR_USE_LOCK_IRQ	
	unsigned long	flags;
#endif

#ifdef VICTOR_USE_LOCK_IRQ	
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif
	if( on_off == 1 ){		// accessible
		writel(readl(&gpio_reg->mask_data_rw)|gpio, &gpio_reg->mask_data_rw);
	}else{				// mask
		writel(readl(&gpio_reg->mask_data_rw)&~gpio, &gpio_reg->mask_data_rw);
	}

#ifdef VICTOR_USE_LOCK_IRQ	
	spin_unlock_irqrestore(&gpio_lock, flags);
#else
	spin_unlock(&gpio_lock);
#endif
}
EXPORT_SYMBOL(mcpu_gpio_mask_pin);

void    mcpu_gpio_intr_enable(u32 gpio , int on_off)
{
#ifdef VICTOR_USE_LOCK_IRQ
        unsigned long   flags;
#endif

#ifdef VICTOR_USE_LOCK_IRQ
        spin_lock_irqsave(&gpio_lock, flags);
#else
        spin_lock(&gpio_lock);
#endif
        if( on_off == 1 ){              // enable
                writel(readl(&gpio_reg->interrupt_enable)|gpio, &gpio_reg->interrupt_enable);
        }else{                          // disable
                writel(readl(&gpio_reg->interrupt_enable)&~gpio, &gpio_reg->interrupt_enable);
        }


#ifdef VICTOR_USE_LOCK_IRQ
        spin_unlock_irqrestore(&gpio_lock, flags);
#else
	spin_unlock(&gpio_lock);
#endif
}
EXPORT_SYMBOL(mcpu_gpio_intr_enable);

u32	mcpu_gpio_get_mask_interrupt_status(void)
{
	return readl(&gpio_reg->mask_interrupt_status);
}
EXPORT_SYMBOL(mcpu_gpio_get_mask_interrupt_status);
