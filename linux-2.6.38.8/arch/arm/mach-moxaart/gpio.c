//#include	<linux/config.h>
#include	<linux/module.h>
#include	<linux/kernel.h>
#include	<linux/types.h>
#include	<linux/spinlock.h>

#include	<asm/io.h>
#include	<mach/moxaart/moxaart.h>
#include	<mach/gpio.h>


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/slab.h>





/* use struct to access gpio register */
static moxaart_gpio_reg_t *gpio_reg = (moxaart_gpio_reg_t *)MOXAART_GPIO_VA_BASE;
static spinlock_t gpio_lock = SPIN_LOCK_UNLOCKED;


void	moxaart_gpio_inout(u32 gpio, int inout)
{
	spin_lock(&gpio_lock);
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
	spin_unlock(&gpio_lock);
}
EXPORT_SYMBOL(moxaart_gpio_inout);

u32	moxaart_gpio_get_inout(u32 gpio)
{
	u32	ret;

	spin_lock(&gpio_lock);
	if ( readl(&gpio_reg->pin_dir) & gpio )
		ret = MCPU_GPIO_OUTPUT;
	else
		ret = MCPU_GPIO_INPUT;
	spin_unlock(&gpio_lock);
	return ret;
}
EXPORT_SYMBOL(moxaart_gpio_get_inout);

/* set the dout register */
void	moxaart_gpio_set(u32 gpio, int highlow)
{
	spin_lock(&gpio_lock);

	switch ( highlow ) {
	case MCPU_GPIO_HIGH :
		writel(readl(&gpio_reg->data_out)|gpio, &gpio_reg->data_out);
		break;
	case MCPU_GPIO_LOW :
		writel(readl(&gpio_reg->data_out)&~gpio, &gpio_reg->data_out);
		break;
	}

	spin_unlock(&gpio_lock);
}
EXPORT_SYMBOL(moxaart_gpio_set);

/* get current data
 * if pin is out: read from data_out
 * else read form data_int 
 */
u32	moxaart_gpio_get(u32 gpio)
{
	u32	ret;

	spin_lock(&gpio_lock);
	ret = readl(&gpio_reg->pin_dir);
	if ( ret & gpio )	/* out put */
		ret = readl(&gpio_reg->data_out) & gpio;
	else
		ret = readl(&gpio_reg->data_in) & gpio;
	spin_unlock(&gpio_lock);
	return ret;
}
EXPORT_SYMBOL(moxaart_gpio_get);

void	moxaart_gpio_mp_set(u32 gpio)
{
	spin_lock(&gpio_lock);
	*(volatile unsigned int *)(MOXAART_PMU_VA_BASE+0x100) |= gpio;
	spin_unlock(&gpio_lock);
}
EXPORT_SYMBOL(moxaart_gpio_mp_set);

void	moxaart_gpio_mp_clear(u32 gpio)
{
	spin_lock(&gpio_lock);
	*(volatile unsigned int *)(MOXAART_PMU_VA_BASE+0x100) &= ~gpio;
	spin_unlock(&gpio_lock);
}
EXPORT_SYMBOL(moxaart_gpio_mp_clear);

/* Physical address */
static moxaart_gpio_reg_t	*gpio_reg_phy=(moxaart_gpio_reg_t *)MOXAART_GPIO_BASE;
void	moxaart_gpio_inout_phy(u32 gpio, int inout)
{
	spin_lock(&gpio_lock);
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
	spin_unlock(&gpio_lock);
}
EXPORT_SYMBOL(moxaart_gpio_inout_phy);

u32	moxaart_gpio_get_inout_phy(u32 gpio)
{
	u32	ret;

	spin_lock(&gpio_lock);
	if ( readl(&gpio_reg_phy->pin_dir) & gpio )
		ret = MCPU_GPIO_OUTPUT;
	else
		ret = MCPU_GPIO_INPUT;
	spin_unlock(&gpio_lock);
	return ret;
}
EXPORT_SYMBOL(moxaart_gpio_get_inout_phy);

/* set the dout register */
void	moxaart_gpio_set_phy(u32 gpio, int highlow)
{
	spin_lock(&gpio_lock);

	switch ( highlow ) {
	case MCPU_GPIO_HIGH :
		writel(readl(&gpio_reg_phy->data_out)|gpio, &gpio_reg_phy->data_out);
		break;
	case MCPU_GPIO_LOW :
		writel(readl(&gpio_reg_phy->data_out)&~gpio, &gpio_reg_phy->data_out);
		break;
	}

	spin_unlock(&gpio_lock);
}
EXPORT_SYMBOL(moxaart_gpio_set_phy);

/* get current data
 * if pin is out: read from data_out
 * else read form data_int 
 */
u32	moxaart_gpio_get_phy(u32 gpio)
{
	u32	ret;

	spin_lock(&gpio_lock);
	ret = readl(&gpio_reg_phy->pin_dir);
	if ( ret & gpio )	/* out put */
		ret = readl(&gpio_reg_phy->data_out) & gpio;
	else
		ret = readl(&gpio_reg_phy->data_in) & gpio;
	spin_unlock(&gpio_lock);
	return ret;
}
EXPORT_SYMBOL(moxaart_gpio_get_phy);


















void	moxaart_gpio_inout_pin(u32 gpio, int inout)
{
	spin_lock(&gpio_lock);
	switch ( inout ) {
	case MCPU_GPIO_INPUT :
		/* set 0 for input */
		writel(readl(&gpio_reg->pin_dir)&(~(1<<gpio)), &gpio_reg->pin_dir);
		break;
	case MCPU_GPIO_OUTPUT :
		/* set 1 for input */
		writel(readl(&gpio_reg->pin_dir)|(1<<gpio), &gpio_reg->pin_dir);
		break;
	}
	spin_unlock(&gpio_lock);
}
EXPORT_SYMBOL(moxaart_gpio_inout_pin);



/* set the dout register */
void	moxaart_gpio_set_pin(u32 gpio, int highlow)
{
	spin_lock(&gpio_lock);

	switch ( highlow ) {
	case MCPU_GPIO_HIGH :
		writel(readl(&gpio_reg->data_out)|(1<<gpio), &gpio_reg->data_out);
		break;
	case MCPU_GPIO_LOW :
		writel(readl(&gpio_reg->data_out)&(~(1<<gpio)), &gpio_reg->data_out);
		break;
	}

	spin_unlock(&gpio_lock);
}
EXPORT_SYMBOL(moxaart_gpio_set_pin);

/* get current data
 * if pin is out: read from data_out
 * else read form data_int 
 */
u32	moxaart_gpio_get_pin(u32 gpio)
{
	u32	ret;

	spin_lock(&gpio_lock);
	ret = readl(&gpio_reg->pin_dir);
	if ( ret & (1<<gpio) )	/* out put */
		ret = readl(&gpio_reg->data_out) & (1<<gpio);
	else
		ret = readl(&gpio_reg->data_in) & (1<<gpio);
	spin_unlock(&gpio_lock);
	return ret;
}
EXPORT_SYMBOL(moxaart_gpio_get_pin);


void moxaart_gpio_set_trigger(u32 pin,u32 trigger)
{
   	u32 tmp, tmp1;
	
	spin_lock(&gpio_lock);
 	tmp = inl(&gpio_reg->int_trigger);

  	if(trigger){
  		tmp |= 1<<pin;
  	}else{
  	    tmp1 = 1<<pin;
  	    tmp &= ~tmp1;
  	}
	outl(tmp , &gpio_reg->int_trigger);  	
	spin_unlock(&gpio_lock);
}
EXPORT_SYMBOL(moxaart_gpio_set_trigger);

void moxaart_gpio_set_edge_mode(u32 pin,u32 both)
{
    	u32 tmp,tmp1;
	spin_lock(&gpio_lock);
 	tmp = inl(&gpio_reg->int_both);

  	if(both){
  		tmp |= 1<<pin;
  	}else{
  	    	tmp1 = 1<<pin;
  		tmp &= ~tmp1;
  	}
	outl(tmp , &gpio_reg->int_both);
	spin_unlock(&gpio_lock);
}
EXPORT_SYMBOL(moxaart_gpio_set_edge_mode);


void moxaart_gpio_set_active_mode(u32 pin,u32 active)
{
    	u32 tmp, tmp1;
	spin_lock(&gpio_lock);
 	tmp = inl(&gpio_reg->int_rise_neg);

  	if(active){
  		tmp |= 1<<pin;	
  	}else{
		tmp1 = 1<<pin;
		tmp &= ~tmp1;    
  	}
	outl(tmp , &gpio_reg->int_rise_neg);
	spin_unlock(&gpio_lock);
}
EXPORT_SYMBOL(moxaart_gpio_set_active_mode);


void  moxaart_gpio_set_intr_enable_pin(u32 pin)
{
	u32 tmp;
	spin_lock(&gpio_lock);
 	tmp = inl(&gpio_reg->int_enable);
	tmp |= 1<<pin;
  	outl(tmp , &gpio_reg->int_enable);
	spin_unlock(&gpio_lock);
}       
EXPORT_SYMBOL(moxaart_gpio_set_intr_enable_pin);


void  moxaart_gpio_mask_intr_pin(u32 pin)
{
	u32 tmp;
	spin_lock(&gpio_lock);
 	tmp = inl(&gpio_reg->int_mask);
	tmp |= 1<<pin;
  	outl(tmp , &gpio_reg->int_mask);
	spin_unlock(&gpio_lock);
}       
EXPORT_SYMBOL(moxaart_gpio_mask_intr_pin);



void  moxaart_gpio_unmask_intr_pin(u32 pin)
{
	u32 tmp;
	spin_lock(&gpio_lock);
 	tmp = inl(&gpio_reg->int_mask);
	tmp &= ~(1<<pin);
  	outl(tmp , &gpio_reg->int_mask);
	spin_unlock(&gpio_lock);
}       
EXPORT_SYMBOL(moxaart_gpio_unmask_intr_pin);



void  moxaart_gpio_clear_intr(u32 val)
{
	spin_lock(&gpio_lock);
	outl(val , &gpio_reg->int_clear);
	spin_unlock(&gpio_lock);
}       
EXPORT_SYMBOL(moxaart_gpio_clear_intr);


void  moxaart_gpio_clear_intr_pin(u32 pin)
{
	spin_lock(&gpio_lock);
	outl((1<<pin) , &gpio_reg->int_clear);
	spin_unlock(&gpio_lock);
}       
EXPORT_SYMBOL(moxaart_gpio_clear_intr_pin);

