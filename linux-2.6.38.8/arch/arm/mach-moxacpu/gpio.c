//#include	<linux/config.h>
#include	<linux/module.h>
#include	<linux/kernel.h>
#include	<linux/types.h>
#include	<linux/spinlock.h>

#include	<asm/io.h>
#include	<mach/cpe/cpe.h>
#include	<mach/gpio.h>

//#define VICTOR_USE_LOCK_IRQ
/* use struct to access gpio register */
static mcpu_gpio_reg_t	*gpio_reg=(mcpu_gpio_reg_t *)CPE_GPIO_VA_BASE;
static spinlock_t	gpio_lock=SPIN_LOCK_UNLOCKED;


void	mcpu_gpio_inout(u32 gpio, int inout)
{
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long	flags;
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif
	switch ( inout ) {
	case MCPU_GPIO_INPUT :
		/* set 0 for input */
		writel(readl(&gpio_reg->pin_dir)&~gpio, &gpio_reg->pin_dir);
		break;
	case MCPU_GPIO_OUTPUT :
		/* set 1 for input */
		writel(readl(&gpio_reg->pin_dir)|gpio, &gpio_reg->pin_dir);
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
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif
	if ( readl(&gpio_reg->pin_dir) & gpio )
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

/* set the dout register */
void	mcpu_gpio_set(u32 gpio, int highlow)
{
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long	flags;
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif

	switch ( highlow ) {
	case MCPU_GPIO_HIGH :
		writel(readl(&gpio_reg->data_out)|gpio, &gpio_reg->data_out);
		break;
	case MCPU_GPIO_LOW :
		writel(readl(&gpio_reg->data_out)&~gpio, &gpio_reg->data_out);
		break;
	}

#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&gpio_lock, flags);
#else
	spin_unlock(&gpio_lock);
#endif
}
EXPORT_SYMBOL(mcpu_gpio_set);

/* get current data
 * if pin is out: read from data_out
 * else read form data_int 
 */
u32	mcpu_gpio_get(u32 gpio)
{
	u32	ret;
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long	flags;
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif
	ret = readl(&gpio_reg->pin_dir);
	if ( ret & gpio )	/* out put */
		ret = readl(&gpio_reg->data_out) & gpio;
	else
		ret = readl(&gpio_reg->data_in) & gpio;
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&gpio_lock, flags);
#else
	spin_unlock(&gpio_lock);
#endif
	return ret;
}
EXPORT_SYMBOL(mcpu_gpio_get);

void	mcpu_gpio_mp_set(u32 gpio)
{
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long	flags;
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif
	*(volatile unsigned int *)(CPE_PMU_VA_BASE+0x100) |= gpio;
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&gpio_lock, flags);
#else
	spin_unlock(&gpio_lock);
#endif
}
EXPORT_SYMBOL(mcpu_gpio_mp_set);

void	mcpu_gpio_mp_clear(u32 gpio)
{
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long	flags;
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif
	*(volatile unsigned int *)(CPE_PMU_VA_BASE+0x100) &= ~gpio;
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&gpio_lock, flags);
#else
	spin_unlock(&gpio_lock);
#endif
}
EXPORT_SYMBOL(mcpu_gpio_mp_clear);

/* Physical address */
static mcpu_gpio_reg_t	*gpio_reg_phy=(mcpu_gpio_reg_t *)CPE_GPIO_BASE;
void	mcpu_gpio_inout_phy(u32 gpio, int inout)
{
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long	flags;
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif
	switch ( inout ) {
	case MCPU_GPIO_INPUT :
		/* set 0 for input */
		writel(readl(&gpio_reg_phy->pin_dir)&~gpio, &gpio_reg_phy->pin_dir);
		break;
	case MCPU_GPIO_OUTPUT :
		/* set 1 for input */
		writel(readl(&gpio_reg_phy->pin_dir)|gpio, &gpio_reg_phy->pin_dir);
		break;
	}
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&gpio_lock, flags);
#else
	spin_unlock(&gpio_lock);
#endif
}
EXPORT_SYMBOL(mcpu_gpio_inout_phy);

u32	mcpu_gpio_get_inout_phy(u32 gpio)
{
	u32	ret;

#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long	flags;
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif
	if ( readl(&gpio_reg_phy->pin_dir) & gpio )
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
EXPORT_SYMBOL(mcpu_gpio_get_inout_phy);

/* set the dout register */
void	mcpu_gpio_set_phy(u32 gpio, int highlow)
{
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long	flags;
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif

	switch ( highlow ) {
	case MCPU_GPIO_HIGH :
		writel(readl(&gpio_reg_phy->data_out)|gpio, &gpio_reg_phy->data_out);
		break;
	case MCPU_GPIO_LOW :
		writel(readl(&gpio_reg_phy->data_out)&~gpio, &gpio_reg_phy->data_out);
		break;
	}

#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&gpio_lock, flags);
#else
	spin_unlock(&gpio_lock);
#endif
}
EXPORT_SYMBOL(mcpu_gpio_set_phy);

/* get current data
 * if pin is out: read from data_out
 * else read form data_int 
 */
u32	mcpu_gpio_get_phy(u32 gpio)
{
	u32	ret;

#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	unsigned long	flags;
	spin_lock_irqsave(&gpio_lock, flags);
#else
	spin_lock(&gpio_lock);
#endif
	ret = readl(&gpio_reg_phy->pin_dir);
	if ( ret & gpio )	/* out put */
		ret = readl(&gpio_reg_phy->data_out) & gpio;
	else
		ret = readl(&gpio_reg_phy->data_in) & gpio;
#ifdef VICTOR_USE_LOCK_IRQ	// add by Victor Yu. 07-25-2007
	spin_unlock_irqrestore(&gpio_lock, flags);
#else
	spin_unlock(&gpio_lock);
#endif
	return ret;
}
EXPORT_SYMBOL(mcpu_gpio_get_phy);

