/* 
 * linux/arch/arm/mach-moxaarm11/include/mach/irqs.h
 *
 * Copyright 2010 Moxa Tech., Inc.
 *	Vincent Wu.<vincentr.wu@moxa.com>
 *	http://www.moxa.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#ifndef _MOXAARM11_IRQ_H
#define _MOXAARM11_IRQ_H

#define MOXAARM11_IRQ_GMAC1 	0
#define MOXAARM11_INTMASK_GMAC1	(1 << MOXAARM11_IRQ_GMAC1)

#define MOXAARM11_IRQ_GMAC2 	1
#define MOXAARM11_INTMASK_GMAC2	(1 << MOXAARM11_IRQ_GMAC2)

#define MOXAARM11_IRQ_ETHERNET 	2
#define MOXAARM11_INTMASK_MAC	(1 << MOXAARM11_IRQ_ETHERNET)

#define MOXAARM11_IRQ_NAND 	3
#define MOXAARM11_INTMASK_NAND	(1 << MOXAARM11_IRQ_NAND)

#define MOXAARM11_IRQ_PCI 	4
#define MOXAARM11_INTMASK_PCI 	(1 << MOXAARM11_IRQ_PCI )

#define MOXAARM11_IRQ_PCIE 	5
#define MOXAARM11_INTMASK_PCIE	(1 << MOXAARM11_IRQ_PCIE)

#define MOXAARM11_IRQ_CRYPTO 	6
#define MOXAARM11_INTMASK_CRYPTO	(1 << MOXAARM11_IRQ_CRYPTO)

#define MOXAARM11_IRQ_MOXAUART 	7
#define MOXAARM11_INTMASK_MOXAUART	(1 << MOXAARM11_IRQ_MOXAUART)

#define MOXAARM11_IRQ_USB1 	8
#define MOXAARM11_INTMASK_USB1 	(1 << MOXAARM11_IRQ_USB1 )

#define MOXAARM11_IRQ_USB2 	9
#define MOXAARM11_INTMASK_USB2	(1 << MOXAARM11_IRQ_USB2)

#define MOXAARM11_IRQ_SDMMC 	10
#define MOXAARM11_INTMASK_SDMMC	(1 << MOXAARM11_IRQ_SDMMC)

#define MOXAARM11_IRQ_UART1 	11
#define MOXAARM11_INTMASK_UART1	(1 << MOXAARM11_IRQ_UART1)

#define MOXAARM11_IRQ_UART2 	12
#define MOXAARM11_INTMASK_UART2	(1 << MOXAARM11_IRQ_UART2)

#define MOXAARM11_IRQ_GPIO 	13
#define MOXAARM11_INTMASK_GPIO 	(1 << MOXAARM11_IRQ_GPIO)

#define MOXAARM11_IRQ_SPI 	14
#define MOXAARM11_INTMASK_SPI 	(1 << MOXAARM11_IRQ_SPI )

#define MOXAARM11_IRQ_TIMER 	15
#define MOXAARM11_INTMASK_TIMER	(1 << MOXAARM11_IRQ_TIMER)

#define MOXAARM11_IRQ_WATCHDOG 	16
#define MOXAARM11_INTMASK_WDT	(1 << MOXAARM11_IRQ_WATCHDOG)

#define MOXAARM11_IRQ_I2C1 	17
#define MOXAARM11_INTMASK_I2C1	(1 << MOXAARM11_IRQ_I2C1)

#define MOXAARM11_IRQ_I2C2 	18
#define MOXAARM11_INTMASK_I2C2	(1 << MOXAARM11_IRQ_I2C2)

#define MOXAARM11_IRQ_RTC 	19
#define MOXAARM11_INTMASK_RTC	(1 << MOXAARM11_IRQ_RTC)

#define MOXAARM11_IRQ_FRT 	20
#define MOXAARM11_INTMASK_FRT 	(1 << MOXAARM11_IRQ_FRT )

#define MOXAARM11_IRQ_INTAJ 	21
#define MOXAARM11_INTMASK_INTAJ	(1 << MOXAARM11_IRQ_INTAJ)

#define MOXAARM11_IRQ_INTBJ 	22
#define MOXAARM11_INTMASK_INTBJ	(1 << MOXAARM11_IRQ_INTBJ)

#define MOXAARM11_IRQ_INTCJ 	23
#define MOXAARM11_INTMASK_INTCJ	(1 << MOXAARM11_IRQ_INTCJ)

#define MOXAARM11_IRQ_INTDJ 	24
#define MOXAARM11_INTMASK_INTDJ	(1 << MOXAARM11_IRQ_INTDJ)

#define MOXAARM11_IRQ_WAKE_GMAC1 28
#define MOXAARM11_INTMASK_WAKE_GMAC1 	(1 << MOXAARM11_IRQ_GMAC1 )

#define MOXAARM11_IRQ_WAKE_GMAC2 29
#define MOXAARM11_INTMASK_WAKE_GMAC2	(1 << MOXAARM11_IRQ_GMAC2)

#define MOXAARM11_GPIO_IRQ_MAX	32
#define MOXAARM11_VIC_IRQ_MAX	32
#define NR_IRQS			(MOXAARM11_VIC_IRQ_MAX + MOXAARM11_GPIO_IRQ_MAX)

#define MOXAARM11_VIC_STATUS_OFFSET          0x00
#define MOXAARM11_VIC_FIQ_STATUS_OFFSET      0X04
#define MOXAARM11_VIC_RAW_STATUS_OFFSET      0X08
#define MOXAARM11_VIC_INT_SEL_OFFSET         0x0C
#define MOXAARM11_VIC_ENABLE_SET_OFFSET      0x10
#define MOXAARM11_VIC_ENABLE_CLEAR_OFFSET    0x14
#define MOXAARM11_VIC_SENSE_OFFSET           0x24
#define MOXAARM11_VIC_BOTH_EDGE_OFFSET       0x28
#define MOXAARM11_VIC_EVENT_OFFSET           0x2C
#define MOXAARM11_VIC_EDGE_CLEAR_OFFSET      0x38

#define MOXAARM11_TIMER0_OFFSET              0x00
#define MOXAARM11_TIMER1_OFFSET              0x10
#define MOXAARM11_TIMER2_OFFSET              0x20
#define MOXAARM11_TIMER_CTRL_OFFSET          0x30

#ifndef __ASSEMBLY__
typedef enum{
    vicc_edge_activeFalling = 0,
    vicc_edge_activeRaising = 1,
    vicc_edge_activeBoth    = 2,
    vicc_level_activeHigh   = 3,
    vicc_level_activeLow    = 4
} vic_intr_trig_mode_E;

extern void moxaarm11_vic_set_intr_trigger(int irqnr, vic_intr_trig_mode_E mode);
extern void moxaarm11_vic_clear_intr(int irq);
extern void moxaarm11_vic_enable_intr(int irq);
extern void moxaarm11_vic_disable_intr(int vecno);
unsigned int moxaarm11_vic_get_sts(void);
#endif	// __ASSEMBLY__

#ifdef CONFIG_MOXA_FIQ
typedef enum{
	vic_mode_irq = 0,
	vic_mode_fiq = 1
} vic_mode;
#endif

#endif	// _MOXAARM11_IRQ_H
