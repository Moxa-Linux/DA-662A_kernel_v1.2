/*
 * This program is the Moxa CPU GPIO driver.
 *
 * History:
 * Date			Author			Comment
 * 11-01-2011	ChinFu Yang		Created it. Make it for MoxaArt CPU.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/slab.h>

#include <linux/irq.h>
#include <linux/spinlock.h>
#include <linux/irqnr.h>

#include <mach/moxaart_int.h>
#include "moxaart_gpio.h"
#include <asm/hw_irq.h>


#define DRV_NAME "MOXA Art GPIO Driver"

#define DEBUG_MOXAART_GPIO
#ifdef DEBUG_MOXAART_GPIO
#define dbg_gpio(msg, arg...)	printk("*** %s: %s()[%d]: " msg, __FILE__, __func__, __LINE__, ##arg)
#else	
#define dbg_gpio(x...)
#endif

struct moxaart_gpio_module {
	struct gpio_chip gpio;
	/* base address of registers */
	moxaart_gpio_reg_t __iomem *regs;
	int vic_irq_num;
	int intr_start;
	unsigned int irq_mask;
	spinlock_t lock;
};


static struct irq_chip moxaart_gpio_irq_chip = {
	.name			= "moxaart gpio irq",
	.irq_ack			= moxaart_gpio_irq_ack,
	.irq_mask		= moxaart_gpio_irq_mask,
	.irq_unmask		= moxaart_gpio_irq_unmask,
	.irq_set_type		= moxaart_gpio_irq_set_type,
};


static void moxaart_gpio_irq_mask(struct irq_data *d)
{
	struct moxaart_gpio_module *module = irq_data_get_irq_chip_data(d);
	int pin = d->irq - module->intr_start;
	module->irq_mask |= ~(1 << (d->irq - module->intr_start));
	moxaart_gpio_mask_intr_pin(pin);
}

static void moxaart_gpio_irq_unmask(struct irq_data *d)
{
	struct moxaart_gpio_module *module = irq_data_get_irq_chip_data(d);
	int pin = d->irq - module->intr_start;
	module->irq_mask &= 1 << (d->irq - module->intr_start);
	moxaart_gpio_unmask_intr_pin(pin);
}


// Reference : vr41xx_set_irq_trigger()
static int moxaart_gpio_irq_set_type(struct irq_data *d, unsigned int type)
{
	struct moxaart_gpio_module *module = irq_data_get_irq_chip_data(d);
	uint32_t pin = d->irq - module->intr_start;
	unsigned long flags;

	spin_lock_irqsave(&module->lock, flags);
	moxaart_gpio_inout(pin , MCPU_GPIO_INPUT);
	switch(type){
		case IRQ_TYPE_EDGE_RISING:
			moxaart_gpio_set_trigger(pin , MCPU_GPIO_EDGE_TRIGGER);
			moxaart_gpio_set_edge_mode(pin , MCPU_GPIO_EDGE_SINGLE);
			moxaart_gpio_set_active_mode(pin , MCPU_GPIO_EDGE_RISING_NEG);
			break;
		case IRQ_TYPE_EDGE_FALLING:
			moxaart_gpio_set_trigger(pin , MCPU_GPIO_EDGE_TRIGGER);
			moxaart_gpio_set_edge_mode(pin , MCPU_GPIO_EDGE_SINGLE);
			moxaart_gpio_set_active_mode(pin , MCPU_GPIO_EDGE_FALLING_NEG);
			break;
		case IRQ_TYPE_EDGE_BOTH:
			moxaart_gpio_set_trigger(pin , MCPU_GPIO_EDGE_TRIGGER);
			moxaart_gpio_set_edge_mode(pin , MCPU_GPIO_EDGE_BOTH);
			break;
		case IRQ_TYPE_LEVEL_HIGH:
			moxaart_gpio_set_trigger(pin , MCPU_GPIO_LEVEL_TRIGGER);
			moxaart_gpio_set_active_mode(pin , MCPU_GPIO_LEVEL_HIGH_NEG);
			break;
		case IRQ_TYPE_LEVEL_LOW:
			moxaart_gpio_set_trigger(pin , MCPU_GPIO_LEVEL_TRIGGER);
			moxaart_gpio_set_active_mode(pin , MCPU_GPIO_LEVEL_LOW_NEG);
			break;			
		default:
			printk("*** Wrong GPIO IRQ Type \n");
			break;
	}
	spin_unlock_irqrestore(&module->lock, flags);
	return 0;
}



void moxaart_gpio_irq_ack(struct irq_data *d)
{
	struct moxaart_gpio_module *module = irq_data_get_irq_chip_data(d);
	int pin = d->irq - module->intr_start;
	printk("GPIO Interrupt ACK , pin = %d \n" , pin);
}

static irqreturn_t moxaart_gpio_irq_handler(int irq, void *dev)
{
	struct moxaart_gpio_module *module = dev;
	u32 pending_req;
	u32 pin;
	int ret = 0;


	pending_req = readl(&module->regs->int_masked_state);

	if (!pending_req){
		printk("GPIO has no pending interrupt\r\n");		
		return IRQ_HANDLED;
	}

	do {
		pin = __ffs(pending_req);
		generic_handle_irq(pin + module->intr_start);
		pending_req &= ~(1 << pin);
		moxaart_gpio_clear_intr_pin(pin);
	} while (pending_req);

	return IRQ_HANDLED;
}




static irqreturn_t moxaart_gpio_irq_handler_pin(int irq, void *devid)
{
	printk("GPIO PIN INTR in moxaart_gpio_irq_handler_pin() , irq = %d\n" , irq);
	return IRQ_HANDLED;
}


static void __devinit moxaart_gpio_init(struct moxaart_gpio_module *module)
{
	// initial .......................... for test .....................
	int pin = 6;	// Pin 6 is the RESET button of MOXA-IW Product OnCell 5104
	moxaart_gpio_inout_pin(pin , MCPU_GPIO_INPUT);
	moxaart_gpio_set_trigger(pin , MCPU_GPIO_EDGE_TRIGGER);
	moxaart_gpio_set_edge_mode(pin , MCPU_GPIO_EDGE_BOTH);
	moxaart_gpio_unmask_intr_pin(pin);
	moxaart_gpio_set_intr_enable_pin(pin);
}


int moxaart_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
	struct moxaart_gpio_module *module;

	module = container_of(chip, struct moxaart_gpio_module, gpio);
	return module->intr_start + offset;

}

int moxaart_gpio_direction_input(struct gpio_chip *chip,unsigned offset)
{
	moxaart_gpio_inout_pin(offset , MCPU_GPIO_INPUT);
	return 0;
}

int moxaart_gpio_get_value(struct gpio_chip *chip, unsigned offset)
{
	return moxaart_gpio_get_pin(offset);
}


int moxaart_gpio_direction_output(struct gpio_chip *chip,unsigned offset, int value)
{
	moxaart_gpio_inout_pin(offset , MCPU_GPIO_OUTPUT);
	return 0;
}

void	moxaart_gpio_set_value(struct gpio_chip *chip,	unsigned offset, int value)
{
	moxaart_gpio_set_pin(offset , value);
}



static int moxaart_gpio_irq_init(struct moxaart_gpio_module *module)
{
	int start = module->intr_start;
	int pin = 0;
	int irq = 0;
	int ret = 0;

	
	for (irq = start ; irq < MOXAART_GPIO_INTR_PIN_END ; irq++) {
		pin = irq - start;		

		moxaart_gpio_mask_intr_pin(pin);
		set_irq_chip(irq , &moxaart_gpio_irq_chip);
		set_irq_chip_data(irq, module);
		set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);

		ret = request_irq(irq, moxaart_gpio_irq_handler_pin, IRQF_SHARED | IRQ_TYPE_LEVEL_HIGH, "moxaart gpio irq pin", module);
		if (ret) {
			printk(KERN_ERR "%s: failed to obtain irq in moxaart_gpio_irq_init(), ret = %d \n", module->gpio.label , ret);
			return -1;
		}
	
		moxaart_gpio_mask_intr_pin(pin);
		moxaart_gpio_inout_pin(pin , MCPU_GPIO_INPUT);
		moxaart_gpio_set_trigger(pin , MCPU_GPIO_EDGE_TRIGGER);
		moxaart_gpio_set_edge_mode(pin , MCPU_GPIO_EDGE_BOTH);
		moxaart_gpio_clear_intr_pin(pin);
		moxaart_gpio_unmask_intr_pin(pin);
	}
	
	return 0;
}


static void moxaart_gpio_irq_remove(struct moxaart_gpio_module *module)
{
	int start = module->intr_start;
	int irq;

	for (irq = start; irq < MOXAART_GPIO_INTR_PIN_END; irq++) {
		set_irq_flags(irq, 0);
		set_irq_chip(irq, NULL);
		set_irq_chip_data(irq, NULL);
		free_irq(irq , module);
	}
}



static int __devinit moxaart_gpio_probe(struct platform_device *pdev)
{

	struct device *dev = &pdev->dev;
	struct moxaart_gpio_module *module;
	struct resource *res;
	int ret = 0;
	int i = 0;
	int irq = 0;

	module = kzalloc(sizeof(*module), GFP_KERNEL);
	if (!module) {
		dev_err(dev, "unable to allocate private data\n");
		ret = -ENOMEM;
		goto out_return;
	}

	platform_set_drvdata(pdev, module);
	spin_lock_init(&module->lock);


#if 0	// dynamic allocate ...
	/* get GPIO IRQ Number for this module */
	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res) {
		dev_err(&pdev->dev,	"Found GPIO with no IRQ. Check %s setup!\n",	dev_name(&pdev->dev));
		ret = -ENODEV;
		goto out_free_module;
	}
	module->irq_num = res->start;

	/* get access to the GPIO registers for this module */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "Found GPIO with no MEM resource. Check %s setup!\n",	dev_name(&pdev->dev));
		ret = -ENODEV;
		goto out_free_module;
	}
#endif


	module->regs = (moxaart_gpio_reg_t *)MOXAART_GPIO_VA_BASE;
	if (!module->regs) {
		dev_err(dev, "GPIO registers ioremap FAIL\n");
		ret = -ENOMEM;
		goto out_free_module;
	}
	module->vic_irq_num = IRQ_GPIO;
	module->intr_start = MOXAART_GPIO_INTR_PIN_START;
	module->irq_mask = readl(&module->regs->int_mask);
//	moxaart_gpio_init(module);	// For testing, gpio interrupt pin 6 = reset button


	/* Initialize the GPIO data structures */
	module->gpio.dev = &pdev->dev;
	module->gpio.label = pdev->name;
	module->gpio.owner = THIS_MODULE;
	module->gpio.direction_input = moxaart_gpio_direction_input;
	module->gpio.get = moxaart_gpio_get_value;
	module->gpio.direction_output = moxaart_gpio_direction_output;
	module->gpio.set = moxaart_gpio_set_value;
	module->gpio.to_irq = moxaart_gpio_to_irq;
	module->gpio.can_sleep = 0;
	module->gpio.base = 0;
	module->gpio.ngpio = MCPU_GPIO_NUM;
	
	ret = gpiochip_add(&(module->gpio));
	if (ret) {
		dev_err(dev, "unable to add GPIO chip\n");
		goto out_iounmap_regs;
	}


	/* Initialize the GPIO IRQ */
	ret = moxaart_gpio_irq_init(module);
	if (ret){
		printk("*** moxaart_gpio_irq_init() Fail ..................");
		goto out_free_module;
	}	
	moxaart_gpio_init(module);// For Interrupt testing

	
	ret = request_irq(module->vic_irq_num, moxaart_gpio_irq_handler, IRQF_SHARED | IRQ_TYPE_LEVEL_HIGH, "moxaart gpio irq", module);
	if (ret) {
		printk(KERN_ERR "%s: failed to obtain irq\n", pdev->name);
		goto out_removeirq;
	}
	printk("Successed.\n");


	
	return 0;

out_iounmap_regs:
//	iounmap(module->regs);
out_removeirq:
	moxaart_gpio_irq_remove(module);
out_free_module:
	kfree(module);
out_return:
	return ret;

}




static int __devexit moxaart_gpio_remove(struct platform_device *pdev)
{
	struct moxaart_gpio_module *module = platform_get_drvdata(pdev);
	struct device *dev = &pdev->dev;
	int ret;
	
	moxaart_gpio_irq_remove(module);
	ret = gpiochip_remove(&module->gpio);
	if (ret) {
		dev_err(dev, "unable to remove GPIO chip\n");
		return ret;
	}

//	iounmap(module->regs);
	kfree(module);
	return 0;
}


static struct platform_driver moxaart_gpio_driver = {
	.driver = {
		.name	= "MOXA Art GPIO Driver",
		.owner	= THIS_MODULE,
	},
	.probe		= moxaart_gpio_probe,
	.remove		= __devexit_p(moxaart_gpio_remove),
};

static int __init moxaart_gpio_module_init(void)
{
	printk("%s \n",__FUNCTION__);
	return platform_driver_register(&moxaart_gpio_driver);
}


static void __exit moxaart_gpio_module_exit(void)
{
	printk("%s \n",__FUNCTION__);
	platform_driver_unregister(&moxaart_gpio_driver);
}

module_init(moxaart_gpio_module_init);
module_exit(moxaart_gpio_module_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChinFu Yang");
MODULE_DESCRIPTION("GPIO driver for Moxa Art chipset");


