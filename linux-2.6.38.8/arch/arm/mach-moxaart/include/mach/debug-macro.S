/*
 * Debugging macro include header
 *
 *  Copyright (C) 1994-1999 Russell King
 *  Copyright (C) 2001-2006 Storlink, Corp.
 *  Copyright (C) 2008-2009 Paulius Zaleckas <paulius.zaleckas@teltonika.lt>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <mach/hardware.h>

.macro	addruart, rp, rv
ldr	\rp, =MOXAART_UART1_BASE			@ physical
ldr	\rv, =0xf9820000 @ virtual
.endm

/* use GPIO 0~3 in led_num is 0 to 3 */
/* 
.macro  ledon, ra, rb, led_num
ldr 	\ra, = 0x98700000 
ldr \rb, [\ra]
	#low activte
	bic \rb, \rb, #1<<\led_num @ low active, so clear bit 
	str \rb, [\ra]
.endm

.macro  ledoff, ra, rb, led_num
ldr 	\ra, = 0x98700000
	ldr \rb, [\ra]
	#low activte
	orr \rb, \rb, #1<<\led_num @ low active, so clear bit 
	str \rb, [\ra]
.endm
*/

/* 
 * ra: gpio base
 * rb: 
 * val: register to set
 */
.macro  ledset, ra, rb, val
	#read gpio value
	ldr 	\ra, = MOXAART_GPIO_BASE
	ldr \rb, [\ra]		   @ read current gpio value
	#set LED, it is low active(bic)
	ldr \ra, = \val
	@led0 
	tst \ra, #0x01  @eq for and == 0
	orreq \rb, \rb, #1<<0	@ led off
	bicne \rb, \rb, #1<<0	@ led on
	@led1
	tst \ra, #0x02
	orreq \rb, \rb, #1<<1	@ led off
	bicne \rb, \rb, #1<<1	@ led on
	@led2
	tst \ra, #0x04
	orreq \rb, \rb, #1<<2	@ led off
	bicne \rb, \rb, #1<<2	@ led on
	@led3
	tst \ra, #0x08 
	orreq \rb, \rb, #1<<3	@ led off
	bicne \rb, \rb, #1<<3	@ led on
	#save to gpio
	ldr 	\ra, = MOXAART_GPIO_BASE
	str \rb, [\ra]
.endm

#define UART_SHIFT	2
#define FLOW_CONTROL
#include <asm/hardware/debug-8250.S>
