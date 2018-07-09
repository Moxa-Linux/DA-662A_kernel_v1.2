 /*
 * This program is the Moxa Macro GPIO driver.
 *
 * History:
 * Date			Author			Comment
 * 12-14-2011	ChinFu Yang		Created it. Make it for Moxa Macro CPU.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/rtc.h>
#include <linux/timer.h>
#include <linux/ioport.h>
#include <linux/proc_fs.h>
#include <mach/irqs.h>
#include <linux/irq.h>
#include <mach/map.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <mach/system-control.h>

#define MOXAMACRO_GPIO_INTR_PIN_START           (MOXAARM11_VIC_IRQ_MAX)
#define MOXAMACRO_GPIO_INTR_PIN_END             (MOXAMACRO_GPIO_INTR_PIN_START+MOXAARM11_GPIO_IRQ_MAX)

//#define USE_THREADED_IRQ
//#define USE_NESTED_IRQ

//#define DEBUG_MOXAMACRO_GPIO
#ifdef DEBUG_MOXAMACRO_GPIO
#define dbg_gpio(msg, arg...)	printk("*** %s: %s()[%d]: " msg, __FILE__, __func__, __LINE__, ##arg)
#else	
#define dbg_gpio(x...)
#endif

struct moxamacro_gpio_module {
	struct gpio_chip	gpio;
	int			intr_start;
};

static void moxamacro_gpio_irq_mask(unsigned int irq)
{
	struct moxamacro_gpio_module *module = (struct moxamacro_gpio_module *)get_irq_chip_data(irq);
	int pin = irq - module->intr_start;

	mcpu_gpio_intr_enable(1<<pin , 0);	// 0 = mask , 1 = accessible
}

static void moxamacro_gpio_irq_unmask(unsigned int irq)
{
	struct moxamacro_gpio_module *module = (struct moxamacro_gpio_module *)get_irq_chip_data(irq);
	int pin = irq - module->intr_start;

	mcpu_gpio_intr_enable(1<<pin , 1);	// 0 = mask , 1 = accessible
}

static void moxamacro_gpio_irq_ack(unsigned int irq)
{
	struct moxamacro_gpio_module *module = (struct moxamacro_gpio_module *)get_irq_chip_data(irq);
	int pin = irq - module->intr_start;

	mcpu_gpio_clear_irq_pending(1<<pin);
}

static void moxamacro_gpio_irq_mask_ack(unsigned int irq)
{
	struct moxamacro_gpio_module *module = (struct moxamacro_gpio_module *)get_irq_chip_data(irq);
	int pin = irq - module->intr_start;

	mcpu_gpio_intr_enable(1<<pin , 0);	// 0 = mask , 1 = accessible
	mcpu_gpio_clear_irq_pending(1<<pin);
}

static int moxamacro_gpio_irq_set_type(unsigned int irq, unsigned int type)
{
	struct moxamacro_gpio_module	*module = (struct moxamacro_gpio_module *)get_irq_chip_data(irq);
	uint32_t			pin = irq - module->intr_start;

	mcpu_gpio_inout(1<<pin , MCPU_GPIO_INPUT);
	switch(type){
	case IRQ_TYPE_EDGE_RISING:
		mcpu_gpio_set_to_irq(1<<pin , vicc_edge_activeRaising);
		break;
	case IRQ_TYPE_EDGE_FALLING:
		mcpu_gpio_set_to_irq(1<<pin , vicc_edge_activeFalling);
		break;
	case IRQ_TYPE_EDGE_BOTH:
		mcpu_gpio_set_to_irq(1<<pin , vicc_edge_activeBoth);
		break;
	case IRQ_TYPE_LEVEL_HIGH:
		mcpu_gpio_set_to_irq(1<<pin , vicc_level_activeHigh);
		break;
	case IRQ_TYPE_LEVEL_LOW:
		mcpu_gpio_set_to_irq(1<<pin , vicc_level_activeLow);
		break;			
	default:
		printk("*** Wrong GPIO IRQ Type \n");
		break;
	}
	return 0;
}

static struct irq_chip moxamacro_gpio_irq_chip = {
	.name		= "GPIOIrq",
	.ack		= moxamacro_gpio_irq_ack,
	.mask		= moxamacro_gpio_irq_mask,
	.mask_ack	= moxamacro_gpio_irq_mask_ack,
	.unmask		= moxamacro_gpio_irq_unmask,
	.set_type	= moxamacro_gpio_irq_set_type,
};

static irqreturn_t moxamacro_gpio_irq_handler(int irq, void *dev)
{
	struct moxamacro_gpio_module	*module=dev;
	u32				pending_req;
	int				pin, istart;
	irqreturn_t			ret=IRQ_NONE;
	
	istart = module->intr_start;
	pending_req = mcpu_gpio_get_mask_interrupt_status();
	while ( pending_req ) {
		pin = __ffs(pending_req);
#ifdef USE_NESTED_IRQ
		handle_nested_irq(pin + istart);
#else
		generic_handle_irq(pin + istart);
#endif
		pending_req &= ~(1 << pin);
		ret = IRQ_HANDLED;
	};

	return ret;
}

int moxamacro_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
	struct moxamacro_gpio_module *module;

	module = container_of(chip, struct moxamacro_gpio_module, gpio);
	return module->intr_start + offset;
}

int moxamacro_gpio_direction_input(struct gpio_chip *chip,unsigned offset)
{
	mcpu_gpio_inout(1<<offset , MCPU_GPIO_INPUT);
	return 0;
}

int moxamacro_gpio_get_value(struct gpio_chip *chip, unsigned offset)
{
	return mcpu_gpio_get(1<<offset);
}

int moxamacro_gpio_direction_output(struct gpio_chip *chip,unsigned offset, int value)
{
	mcpu_gpio_inout(1<<offset , MCPU_GPIO_OUTPUT);
	mcpu_gpio_set(1<<offset , value);
	return 0;
}

void	moxamacro_gpio_set_value(struct gpio_chip *chip, unsigned offset, int value)
{
	mcpu_gpio_set(1<<offset , value);
}

static int moxamacro_gpio_irq_init(struct moxamacro_gpio_module *module)
{
	int	start=module->intr_start;
	int	irq;

	mcpu_gpio_mask_pin(0xffffffff , 1);	// set all GPIO pin accessiable
	mcpu_gpio_clear_irq_pending(0xffffffff);// clear all GPIO irq pending
	mcpu_gpio_intr_enable(0xffffffff , 0);	// disable all GPIO interrupt none, 0 = mask , 1 = accessible

	for (irq = start ; irq < MOXAMACRO_GPIO_INTR_PIN_END ; irq++) {
		set_irq_chip(irq , &moxamacro_gpio_irq_chip);
		set_irq_handler(irq, handle_level_irq);
#ifdef USE_THREADED_IRQ
		set_irq_nested_thread(irq, 1);
#endif
		set_irq_chip_data(irq, module);
		set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
	}
	
	return 0;
}

static void moxamacro_gpio_irq_remove(struct moxamacro_gpio_module *module)
{
	int	start = module->intr_start;
	int	irq;

	for (irq = start; irq < MOXAMACRO_GPIO_INTR_PIN_END; irq++) {		
		set_irq_flags(irq, 0);
		set_irq_chip(irq, NULL);
		set_irq_chip_data(irq, NULL);
		free_irq(irq , module);
	}
}

static int __devinit moxamacro_gpio_probe(struct platform_device *pdev)
{

	struct device			*dev = &pdev->dev;
	struct moxamacro_gpio_module	*module;
	int				ret=0;

	module = kzalloc(sizeof(*module), GFP_KERNEL);
	if (!module) {
		dev_err(dev, "unable to allocate private data\n");
		ret = -ENOMEM;
		goto out_return;
	}

	platform_set_drvdata(pdev, module);

/*
	// initialize these GPIOs mutifunction
#ifndef CONFIG_PCI
	{	// set to be GPIOA
	int	i;
	for ( i=0; i<4; i++ )
		moxaarm11_io_pin_select(i, GPIOA_USED_FOR_GPIO);
	}
#endif
*/

	module->intr_start = MOXAMACRO_GPIO_INTR_PIN_START;			

	/* Initialize the GPIO data structures */
	module->gpio.dev = &pdev->dev;
	module->gpio.label = pdev->name;
	module->gpio.owner = THIS_MODULE;
	module->gpio.direction_input = moxamacro_gpio_direction_input;
	module->gpio.get = moxamacro_gpio_get_value;
	module->gpio.direction_output = moxamacro_gpio_direction_output;
	module->gpio.set = moxamacro_gpio_set_value;
	module->gpio.to_irq = moxamacro_gpio_to_irq;
	module->gpio.can_sleep = 0;
	module->gpio.base = 0;
	module->gpio.ngpio = ARCH_NR_GPIOS;
	
	ret = gpiochip_add(&(module->gpio));
	if (ret) {
		dev_err(dev, "unable to add GPIO chip\n");
		goto out_removeirq;
	}

	/* Initialize the GPIO IRQ */
	ret = moxamacro_gpio_irq_init(module);
	if (ret){
		printk("*** moxamacro_gpio_irq_init() Fail ..................");
		goto out_free_module;
	}	

#ifndef USE_THREADED_IRQ
	ret = request_irq(MOXAARM11_IRQ_GPIO, 
			moxamacro_gpio_irq_handler, 
			IRQF_SHARED|IRQ_TYPE_LEVEL_HIGH, 
			"moxamacro gpio irq", 
			module);
#else
	ret = request_threaded_irq(MOXAARM11_IRQ_GPIO,
				NULL,
				moxamacro_gpio_irq_handler,
				IRQF_SHARED|IRQ_TYPE_LEVEL_HIGH,
				"moxamacro gpio irq",
				module);
#endif
	if (ret) {
		printk(KERN_ERR "%s: failed to obtain irq\n", pdev->name);
		goto out_removeirq;
	}
	printk("Moxa Macro CPU GPIO loaded successed.\n");
	
	return 0;

out_removeirq:
	moxamacro_gpio_irq_remove(module);
out_free_module:
	kfree(module);
out_return:
	return ret;

}

static int __devexit moxamacro_gpio_remove(struct platform_device *pdev)
{
	struct moxamacro_gpio_module *module = platform_get_drvdata(pdev);
	struct device *dev = &pdev->dev;
	int ret;
	
	moxamacro_gpio_irq_remove(module);
	free_irq(MOXAARM11_IRQ_GPIO, module);
	ret = gpiochip_remove(&module->gpio);
	if (ret) {
		dev_err(dev, "unable to remove GPIO chip\n");
		return ret;
	}

	kfree(module);
	return 0;
}

static struct platform_driver moxmacro_gpio_driver = {
	.driver = {
		.name	= "MoxaARM11GPIO",
		.owner	= THIS_MODULE,
	},
	.probe		= moxamacro_gpio_probe,
	.remove		= __devexit_p(moxamacro_gpio_remove),
};

static int __init moxamacro_gpio_module_init(void)
{
	return platform_driver_register(&moxmacro_gpio_driver);
}

static void __exit moxamacro_gpio_module_exit(void)
{
	platform_driver_unregister(&moxmacro_gpio_driver);
}

module_init(moxamacro_gpio_module_init);
module_exit(moxamacro_gpio_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChinFu Yang");
MODULE_DESCRIPTION("GPIO driver for Moxa Macro chipset");
