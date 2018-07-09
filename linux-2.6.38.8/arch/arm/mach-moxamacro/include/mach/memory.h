
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

#ifdef CONFIG_MOXAMACRO_REMAP
#define PHYS_OFFSET     UL(0x00000000)
#else
#ifdef CONFIG_MOXAMACRO_DDR2_SDRAM
#define PHYS_OFFSET     UL(0x80000000)
#else
#define PHYS_OFFSET     UL(0x60000000)
#endif
#endif

#endif
