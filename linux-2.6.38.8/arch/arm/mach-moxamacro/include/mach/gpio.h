
#ifndef _MOXAMACRO_GPIO_H
#define _MOXAMACRO_GPIO_H

#include <linux/bitops.h>
#include <mach/irqs.h>

#define ARCH_NR_GPIOS 32

typedef struct _mcpu_gpio_reg_struct {
	volatile unsigned int	data; // offset 0x00
#define MCPU_GPIO_HIGH          1
#define MCPU_GPIO_LOW           0
	volatile unsigned int	direction; // offset 0x04, 0 - input, 1 - output
#define MCPU_GPIO_INPUT         0
#define MCPU_GPIO_OUTPUT        1
	volatile unsigned int	interrupt_sensitivity_control; // offset 0x08
					// 0 - edge, 1 - level
#define MCPU_GPIO_INT_LEVEL	1
#define MCPU_GPIO_INT_EDGE	0
	volatile unsigned int	interrupt_both_edge; // offset 0x0c, 1 - both
#define MCPU_GPIO_INT_BOTH_EDGE	1
	volatile unsigned int	interrupt_event; // offset 0x10, 
					 // 0 - falling edge or low level
					 // 1 - rising edge or high level
#define MCPU_GPIO_INT_FAILING_EDGE	0
#define MCPU_GPIO_INT_RISING_EDGE	1
	volatile unsigned int	interrupt_enable; // offset 0x14, 0 - disable
					  // 1 - enable
#define MCPU_GPIO_INT_ENABLE	1
#define MCPU_GPIO_INT_DISABLE	0
	volatile unsigned int	interrupt_mode_control; // offset 0x18
					// 0 - software. 1 - hardware
#define MCPU_GPIO_INT_SOFT_CONTROl	0
#define MCPU_GPIO_INT_HARD_CONTROL	1
	volatile unsigned int	raw_edge_interrupt; // offset 0x1c
	volatile unsigned int	mask_interrupt_status; // offset 0x20
	volatile unsigned int	interrupt_clear; // offset 0x24
	volatile unsigned int	mask_data_rw; // offset 0x28
	volatile unsigned int	debounce_enable; // offset 0x2c
} mcpu_gpio_reg_t;

extern void     mcpu_gpio_inout(u32 gpio, int inout);
extern u32      mcpu_gpio_get_inout(u32 gpio);
extern void     mcpu_gpio_set(u32 gpio, int highlow);
extern u32      mcpu_gpio_get(u32 gpio);
extern void	mcpu_gpio_set_to_irq(u32 gpio, vic_intr_trig_mode_E mode);
extern void	mcpu_gpio_clear_irq_pending(u32 gpio);
extern void	mcpu_gpio_mask_pin(u32 gpio , int on_off);
extern void     mcpu_gpio_intr_enable(u32 gpio , int on_off);
extern u32	mcpu_gpio_get_mask_interrupt_status(void);

#endif	// _MOXAARM11_GPIO_H
