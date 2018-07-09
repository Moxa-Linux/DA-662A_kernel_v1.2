/* 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/
#include <mach/map.h>
#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H __FILE__

#define pcibios_assign_all_busses()             1
#define PCIBIOS_MIN_IO          0x10000	// 64K
#define PCIBIOS_MIN_MEM		(128 * 1024 * 1024) // 128M

#endif /* __ASM_ARCH_IRQ_H */
