#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

#define IO_SPACE_LIMIT      0xffffffff
#define __io(a)             (a)
//#define __mem_pci(a)        ((unsigned long)(a))
#define __mem_pci(a)        (a)

#if 0	// mask by Victor Yu. 06-13-2005
#define __arch_getw(a) 	    (*(volatile unsigned short *)(a))
#define __arch_putw(v,a)    (*(volatile unsigned short *)(a) = (v))
#endif

#if 0	// mask by Victor Yu. 01-09-2006
#define iomem_valid_addr(iomem,sz)  (1)
#define iomem_to_phys(iomem)        (iomem)
#endif

#endif
