/* 
 * linux/arch/arm/mach-moxaarm11/include/mach/system.h
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

#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H __FILE__

#include <linux/io.h>
#include <asm/proc-fns.h>
#include <mach/map.h>
#include <mach/system-control.h>

static void arch_idle(void)
{
	// if we don't implement power manager, we have not call cpu_do_idle()
	//cpu_do_idle();
}

typedef struct watchdog_reg_struct {
	u32	ControlStatus;	// offset 0x00, read only 
	u32	CounterReloadValue;	// offset 0x04
	u32	CounterRestart;	// offset 0x08, if write value 0x4755, it will reload counter
	u32	Control;	// offset 0x0C
#define ENABLE_WDT		1
#define DISABLE_WDT		0
#define ENABLE_RESET_SYSTEM	2
#define ENABLE_INTERRUPT	4
#define ENABLE_EXTERNAL_SIGNAL	8
	u32	TimeoutStatus;	// offset 0x10
	u32	ClearTimeoutStatus;	// offset 0x14
	u32	ResetWidth;	// offset 0x18
} watchdog_reg_t;

static void arch_reset(char mode, const char *cmd)
{
	watchdog_reg_t	*wdt=(watchdog_reg_t *)MOXAARM11_VA_WATCHDOG_BASE;

	if ( mode == 's') {	
		cpu_reset(0);
	} else {
		moxaarm11_io_pin_select(4, GPIOA_USED_FOR_OTHERS);
		writel(0, &wdt->Control);
		writel(1, &wdt->CounterReloadValue);
		writel(ENABLE_WDT|ENABLE_EXTERNAL_SIGNAL, &wdt->Control);
		writel(0x4755, &wdt->CounterRestart);
	}
}

#endif /* __ASM_ARCH_IRQ_H */
