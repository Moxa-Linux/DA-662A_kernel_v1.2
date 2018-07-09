/*
 * Low-level IRQ helper macros for Gemini platform.
 *
 *  Copyright (C) 2001-2006 Storlink, Corp.
 *  Copyright (C) 2008-2009 Paulius Zaleckas <paulius.zaleckas@teltonika.lt>
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#include <mach/hardware.h>

#define IRQ_STATUS	0x14

	.macro  disable_fiq
	.endm

	.macro  get_irqnr_preamble, base, tmp
	.endm

	.macro  arch_ret_to_user, tmp1, tmp2
	.endm

	.macro get_fiqnr_and_base, irqnr, irqstat, base, tmp
	ldr		\base, =(IO_ADDRESS(MOXAART_IC_BASE)+FIQ_STATUS_REG)
	ldr		\irqstat, [\base]
	mov             \irqnr, #32
2001:
	tst             \irqstat, #1
	bne             2002f
	add             \irqnr, \irqnr, #1
	mov             \irqstat, \irqstat, lsr #1 @logic shift right
	cmp             \irqnr, #64
	bcc             2001b
2002:
	.endm

	/* XXX: irqnr: number, irqstat: status, base: irqstat base address 
	 * check if irq [0:31] is set
	 */
	.macro get_irqnr_and_base, irqnr, irqstat, base, tmp
	ldr		\base, =(IO_ADDRESS(MOXAART_IC_BASE)+IRQ_STATUS_REG)
	ldr		\irqstat, [\base]
	mov             \irqnr, #0
2003:
	tst             \irqstat, #1
	bne             2004f
	add             \irqnr, \irqnr, #1
	mov             \irqstat, \irqstat, lsr #1
	cmp             \irqnr, #32
	bcc             2003b
2004:
	.endm
