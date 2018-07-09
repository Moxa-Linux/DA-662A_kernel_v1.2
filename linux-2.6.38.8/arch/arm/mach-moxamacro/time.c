/* 
 * linux/arch/arm/mach-moxaarm11/moxaarm11-setup.c
 *
 * Copyright 2010 Moxa Tech., Inc.
 *	Vincent Wu.<vincent.wu@moxa.com>
 *	http://www.moxa.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/platform_device.h>

#include <asm/system.h>
#include <asm/leds.h>
#include <asm/mach-types.h>

#include <asm/irq.h>
#include <asm/mach/time.h>
#include <asm/hardware/vic.h>
#include <mach/cpu.h>

#include <mach/map.h>
#include <mach/irqs.h>
#include <mach/timer.h>


//\\ ChinFu : TCM ------------------------------------------ 2011-07-27
#define __section(S) __attribute__ ((__section__(#S)))
#define __dtcm_data __section(.dtcm.data)

#define __itcm_func __attribute__((long_call)) __section(.itcm.text) noinline
#define __itcm_local_func __section(.itcm.text)

static inline u32 moxaarm11_ostimer_pending(void)
{
	u32 pend = readl((const volatile void *)(MOXAARM11_VA_VIC_BASE + VIC_RAW_STATUS));
	return pend & (1 << MOXAARM11_IRQ_TIMER);
}

/***
 * Returns microsecond  since last clock interrupt.  Note that interrupts
 * will have been disabled by do_gettimeoffset()
 * IRQs are disabled before entering here from do_gettimeofday()
 */
static unsigned long moxaarm11_gettimeoffset (void)
{
	unsigned long tdone;
	unsigned long tval;

	/* work out how many ticks have gone since last timer interrupt */

	tval =  readl((const volatile void *)MOXAARM11_TIMER_STST);
	tdone = TIMER_RELOAD_VALUE - tval;

	/* check to see if there is an interrupt pending */

	if (moxaarm11_ostimer_pending()) {
		/* re-read the timer, and try and fix up for the missed
		 * interrupt. Note, the interrupt may go off before the
		 * timer has re-loaded from wrapping.
		 */

		tval =  readl((const volatile void *)MOXAARM11_TIMER_STST);
		tdone = TIMER_RELOAD_VALUE - tval;

		if (tval != 0)
			tdone += TIMER_RELOAD_VALUE;
	}

	return TICKS2USECS(tdone);
}


/*
 * IRQ handler for the timer
 */
static irqreturn_t 
moxaarm11_timer_interrupt(int irq, void *dev_id)
{
	//unsigned int i = 0;

	moxaarm11_vic_clear_intr(MOXAARM11_IRQ_TIMER);
	timer_tick();	
#if 0
//	__asm__ __volatile__ ("mcr p15, 0, %0, c7, c14, 0": :"r" (i));		// D
	__asm__ __volatile__ ("mcr p15, 0, %0, c7, c5, 0 \n"	\
		"nop  \n"			\
		"nop  \n"			\
		"nop  \n"			\
		"nop  \n"			\
		"nop  \n"			\
		"nop  \n"			\
		"nop  \n"			\
		"nop  \n"			\
		"nop  \n"			\
		"nop  \n"			\
		"nop  \n"			\
		"nop  \n"			\
		:
		:"r" (i)
			);		// I	
#endif

//	__asm__ __volatile__ ("mcr p15, 0, %0, c7, c7, 0": :"r" (i));		// D

/*
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x1000
	mcr	p15, 0, r0, c1, c0, 0
*/	
	

	return IRQ_HANDLED;
}

static struct irqaction moxaarm11_timer_irq = {
	.name		= "MOXAARM11 Timer Tick",
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.handler	= moxaarm11_timer_interrupt,
};

static void moxaarm11_timer_init(void)
{
	writel(0, (volatile void __iomem *)MOXAARM11_TIMER_CONTROL);
	writel(TIMER_RELOAD_VALUE - 1, (volatile void __iomem *)MOXAARM11_TIMER_RELOAD);
	writel(TIMER_RELOAD_VALUE - 1, (volatile void __iomem *)MOXAARM11_TIMER_STST);
	moxaarm11_vic_set_intr_trigger(MOXAARM11_IRQ_TIMER, vicc_edge_activeRaising);
	
	setup_irq(MOXAARM11_IRQ_TIMER, &moxaarm11_timer_irq);

	writel(TIMER_ENABLE, (volatile void __iomem *)MOXAARM11_TIMER_CONTROL);
}

struct sys_timer moxaarm11_timer_fn = {
	.init		= moxaarm11_timer_init,
	.offset		= moxaarm11_gettimeoffset,
};
