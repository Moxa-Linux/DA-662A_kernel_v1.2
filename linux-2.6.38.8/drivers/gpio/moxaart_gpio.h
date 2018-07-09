
/*
 * This file is the define for Moxa ART CPU MAC controller.
 *
 * History:
 * Date		Author			Comment
 * 11-01-2011	ChinFu Yang	Created it.
 */
 
#define MOXAART_GPIO_INTR_PIN_START		(MOXAART_NR_BASIC_IRQS+MOXAART_NR_PCI_IRQ)
#define MOXAART_GPIO_INTR_PIN_NUM		32
#define MOXAART_GPIO_INTR_PIN_END		(MOXAART_GPIO_INTR_PIN_START+MOXAART_GPIO_INTR_PIN_NUM)


void moxaart_gpio_irq_ack(struct irq_data *d);
static void moxaart_gpio_irq_mask(struct irq_data *d);
static void moxaart_gpio_irq_unmask(struct irq_data *d);
static int moxaart_gpio_irq_set_type(struct irq_data *d, unsigned int type);


