/* 
 * linux/arch/arm/mach-moxaarm11/include/mach/uncompress.h
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

#ifndef _ARCH_UNCOMPRESS_H_
#define _ARCH_UNCOMPRESS_H_

#include <asm/mach-types.h>
#include <linux/serial_reg.h>
#include <mach/map.h>

#define TX_DONE (UART_LSR_TEMT|UART_LSR_THRE)

static volatile u32* uart_base;

static inline void putc(int c)
{
	// Check THRE and TEMT bits before we transmit the character.
	while ((uart_base[UART_LSR] & TX_DONE)!=TX_DONE)
		barrier();
	
	*uart_base = (u32)(c&0xff);
}

static inline void flush(void)
{
}

static __inline__ void __arch_decomp_setup(unsigned long arch_id)
{
	if(CONFIG_DECOMPRESS_UART_PORT == 1)
		uart_base = (volatile u32*) MOXAARM11_PA_UART2_BASE;
	else
		uart_base = (volatile u32*) MOXAARM11_PA_UART1_BASE;

#ifdef CONFIG_ARCH_MOXAMACRO
	uart_base[UART_IER] = uart_base[UART_IER] &~0x80;
#endif
}

/*
 * arch_id is a variable in decompress_kernel()
 */
#define arch_decomp_setup()	__arch_decomp_setup(arch_id)

#define arch_decomp_wdog()

#endif // _ARCH_UNCOMPRESS_H_
