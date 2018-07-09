
#ifndef _MOXAART_GPIO_H
#define _MOXAART_GPIO_H

/* refer to Table 247. FTGPIO010 Register Summary */
typedef struct _moxaart_gpio_reg_struct {
	unsigned int	data_out;  
	unsigned int	data_in;
	unsigned int	pin_dir;	// direction register: 1 for output, 0 for input
	unsigned int	reserved;
	unsigned int	data_set;
	unsigned int	data_clear;
	unsigned int	pin_pull_enable;
	unsigned int	pin_pull_type;
	unsigned int	int_enable;
	unsigned int	int_raw_state;
	unsigned int	int_masked_state;
	unsigned int	int_mask;
	unsigned int	int_clear;
	unsigned int	int_trigger;
	unsigned int	int_both;
	unsigned int	int_rise_neg;
	unsigned int	bounce_enable;
	unsigned int	bounce_pre_scale;
} moxaart_gpio_reg_t;

#define MCPU_GPIO_INPUT		0
#define MCPU_GPIO_OUTPUT	1
#define MCPU_GPIO_HIGH		1
#define MCPU_GPIO_LOW		0

#define MCPU_GPIO_EDGE_TRIGGER	0	
#define MCPU_GPIO_LEVEL_TRIGGER	1

#define MCPU_GPIO_EDGE_SINGLE		0	
#define MCPU_GPIO_EDGE_BOTH		1

#define MCPU_GPIO_EDGE_RISING_NEG	0	
#define MCPU_GPIO_EDGE_FALLING_NEG	1
#define MCPU_GPIO_LEVEL_HIGH_NEG		0
#define MCPU_GPIO_LEVEL_LOW_NEG		1


#define MCPU_GPIO_NUM		32

extern void	moxaart_gpio_inout(u32 gpio, int inout);
extern u32	moxaart_gpio_get_inout(u32 gpio);
extern void	moxaart_gpio_set(u32 gpio, int highlow);
extern u32	moxaart_gpio_get(u32 gpio);
extern void	moxaart_gpio_mp_set(u32 gpio);
extern void	moxaart_gpio_mp_clear(u32 gpio);

extern void	moxaart_gpio_inout_phy(u32 gpio, int inout);
extern u32	moxaart_gpio_get_inout_phy(u32 gpio);
extern void	moxaart_gpio_set_phy(u32 gpio, int highlow);
extern u32	moxaart_gpio_get_phy(u32 gpio);
extern void	moxaart_gpio_mp_set_phy(u32 gpio);
extern void	moxaart_gpio_mp_clear_phy(u32 gpio);

extern void	moxaart_gpio_inout_pin(u32 gpio, int inout);
extern void	moxaart_gpio_set_pin(u32 gpio, int highlow);
extern u32	moxaart_gpio_get_pin(u32 gpio);
extern void 	moxaart_gpio_set_trigger(u32 pin,u32 trigger);
extern void 	moxaart_gpio_set_edge_mode(u32 pin,u32 both);
extern void 	moxaart_gpio_set_active_mode(u32 pin,u32 active);

extern void  	moxaart_gpio_set_intr_enable_pin(u32 pin);
extern void  	moxaart_gpio_mask_intr_pin(u32 pin);
extern void  	moxaart_gpio_unmask_intr_pin(u32 pin);	
extern void  moxaart_gpio_clear_intr(u32 val);
extern void  	moxaart_gpio_clear_intr_pin(u32 pin);	

// for GPIOLIB
extern int inline gpio_to_irq(unsigned gpio);

#define gpio_set_value(gpio, value)   __gpio_set_value(gpio, value)
#define gpio_get_value(gpio)   __gpio_get_value(gpio)

#endif	// _MOXAART_GPIO_H