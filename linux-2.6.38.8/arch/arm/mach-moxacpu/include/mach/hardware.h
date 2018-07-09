/* 
    hardware.h 
    maintened by ivan wang 2004/8/18 11:25 
*/
#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

#if 1	// add by Victor Yu. 05-16-2005
#define not_complete_yet	1
#endif

#include "cpe/cpe.h"
/* the mini io address is 0x6000,that is IO will allocate from 0-0x6000 offset*/
#define PCIBIOS_MIN_IO  0x6000
/* the mini MEM address is 0x100000,that is MEM will allocate from 0-0x100000 offset*/
#define PCIBIOS_MIN_MEM 0x100000

#define pcibios_assign_all_busses() 1
#endif
