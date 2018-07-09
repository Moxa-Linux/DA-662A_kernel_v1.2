/*
 *  linux/include/asm-arm/arch-ti926/keyboard.h
 *
 *  Copyright (C) 2000 RidgeRun, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Keyboard driver definitions for ARM
 */
#ifndef __ASM_ARM_ARCH_MOXAART_KEYBOARD_H
#define __ASM_ARM_ARCH_MOXAART_KEYBOARD_H

#define kbd_request_region()

#define aux_request_irq
#define aux_free_irq

#ifdef CONFIG_FARADAY_KMI						/// ¦³ keyboard
	///#define kbd_setkeycode 						fkmi_setkeycode
	///#define kbd_getkeycode						fkmi_getkeycode
	///#define kbd_translate						fkmi_translate
	///#define kbd_unexpected_up					fkmi_unexpected_up
	#define kbd_leds							fkmi_leds
	///#define kbd_init_hw							fkmi_init_hw
	#define kbd_enable_irq()					
	#define kbd_disable_irq()					
#else
	#define kbd_setkeycode(sc,kc) (-EINVAL)
	#define kbd_getkeycode(sc) (-EINVAL)
	#define kbd_translate(sc,kcp,rm) ({ *(kcp) = (sc); 1; })
	#define kbd_unexpected_up(kc) (0200)
	#define kbd_leds(leds)
	#define kbd_init_hw()
	#define kbd_enable_irq()					
	#define kbd_disable_irq()					
#endif

/* add by Charles Tsai for keyboard output process */
typedef struct StructFaradayLEDS{
	unsigned int LEDS;		/* final LED state */
	unsigned int LEDSState[3]; 	/* 0: up 1:down*/
	unsigned int LEDSOnOff[3];	/* 0:off, 1:on*/
	unsigned int update; 		/* 1: need update, 0: no need*/
} FaradayLEDS;
#define FA_SCROLL_LOCK 0x46
#define FA_NUM_LOCK    0x45
#define FA_CAP_LOCK    0x3a
#define FA_KMI_ACK     0xfa
/* end add */

#endif /* __ASM_ARM_ARCH_TI925_KEYBOARD_H */
