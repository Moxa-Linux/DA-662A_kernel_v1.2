/* 
 * linux/arch/arm/mach-moxaarm11/include/mach/debug-macro.S
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/
#include <mach/map.h>

                .macro  addruart,rx
                mrc     p15, 0, \rx, c1, c0
                tst     \rx, #1                 @ MMU enabled?
                moveq   \rx, #(MOXAARM11_PA_UART2_BASE&0xff000000)
                orreq   \rx, \rx, #(MOXAARM11_PA_UART2_BASE&0x00ff0000)
                movne   \rx, #(MOXAARM11_VA_UART2_BASE&0xff000000)
                orrne   \rx, \rx, #(MOXAARM11_VA_UART2_BASE&0x00ff0000)
                add     \rx,\rx,#3              @ Uart regs are at off set of 3 if
                                                @ byte writes used - Big Endian.
                .endm

#define UART_SHIFT      2
#include <asm/hardware/debug-8250.S>

