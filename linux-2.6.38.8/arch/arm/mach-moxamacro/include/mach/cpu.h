/* 
 * linux/arch/arm/mach-moxaarm11/include/mach/cpu.h
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

#ifndef _MOXAMACRO_CPU_H
#define _MOXAMACRO_CPU_H

/* core initialisation functions */
extern void moxaarm11_init_irq(void);
extern struct sys_timer moxaarm11_timer_fn;

#define MOXAARM11_APBCLK	49500000 	

#endif	// _MOXAMACRO_CPU_H
