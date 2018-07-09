
/*
 * linux/include/asm-armnommu/arch-p52/memory.h
 *
 * Copyright (c) 1999 Nicolas Pitre <nico@cam.org>
 * 2001 Mindspeed
 */

#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

#if defined ARCH_UC_7112_LX_PLUS
#include	<asm/sizes.h>
#define CONFIG_SDRAM_SIZE	SZ_32M
#else
#include	<asm/sizes.h>
#define CONFIG_SDRAM_SIZE	SZ_64M
#endif

#define DRAM_BASE       0x00000000
#define PHYS_OFFSET     (DRAM_BASE)

#endif
