/*
 *  linux/arch/arm/mm/proc-arm922.S: MMU functions for ARM922
 *
 *  Copyright (C) 1999,2000 ARM Limited
 *  Copyright (C) 2000 Deep Blue Solutions Ltd.
 *  Copyright (C) 2001 Altera Corporation
 *  hacked for non-paged-MM by Hyok S. Choi, 2003.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * These are the low level assembler for performing cache and TLB
 * functions on the arm922.
 *
 *  CONFIG_CPU_ARM922_CPU_IDLE -> nohlt
 */
#include <linux/linkage.h>
#include <linux/init.h>
#include <asm/assembler.h>
#include <asm/hwcap.h>
#include <asm/pgtable-hwdef.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/ptrace.h>
#include "proc-macros.S"
#include <mach/debug-macro.S>

/*
 * The size of one data cache line.
 */
#if 0	/* mask by Victor Yu. 05-30-2005 */
#define CACHE_DLINESIZE	32
#else	/* add by Victor Yu. 05-30-2005 */
#define CACHE_DLINESIZE	16
#define CACHE_DSIZE	16384
#define CACHE_ISIZE	16384
#define CACHE_ILINESIZE	16
#endif

/*
 * The number of data cache segments.
 */
#define CACHE_DSEGMENTS	4

/*
 * The number of lines in a cache segment.
 */
#define CACHE_DENTRIES	64

/*
 * This is the size at which it becomes more efficient to
 * clean the whole cache, rather than using the individual
 * cache line maintainence instructions.  (I think this should
 * be 32768).
 */
#define CACHE_DLIMIT	8192


	.text
/*
 * cpu_arm922_proc_init()
 */
ENTRY(cpu_arm922_proc_init)
	mov	pc, lr

/*
 * cpu_arm922_proc_fin()
 */
ENTRY(cpu_arm922_proc_fin)
	mrc	p15, 0, r0, c1, c0, 0		@ ctrl register
	bic	r0, r0, #0x1000			@ ...i............
	bic	r0, r0, #0x000e			@ ............wca.
	mcr	p15, 0, r0, c1, c0, 0		@ disable caches
	mov	pc, lr

/*
 * cpu_arm922_reset(loc)
 *
 * Perform a soft reset of the system.  Put the CPU into the
 * same state as it would be if it had been reset, and branch
 * to what would be the reset vector.
 *
 * loc: location to jump to for soft reset
 */
	.align	5
ENTRY(cpu_arm922_reset)
	mov	ip, #0
	mcr	p15, 0, ip, c7, c7, 0		@ invalidate I,D caches
	mcr	p15, 0, ip, c7, c10, 4		@ drain WB
#ifdef CONFIG_MMU
	mcr	p15, 0, ip, c8, c7, 0		@ invalidate I & D TLBs
#endif
	mrc	p15, 0, ip, c1, c0, 0		@ ctrl register
	bic	ip, ip, #0x000f			@ ............wcam
	bic	ip, ip, #0x1100			@ ...i...s........
#if 1	/* add by Victor Yu. 06-09-2005 */
#ifdef CONFIG_ARCH_MOXACPU
	bic	ip, ip, #0x0800			@ off BTB
#endif
#endif
	mcr	p15, 0, ip, c1, c0, 0		@ ctrl register
#if 1	/* add by Victor Yu. 06-09-2005 */
#ifdef CONFIG_ARCH_MOXACPU
	nop
	nop
#endif
#endif
	mov	pc, r0

/*
 * cpu_arm922_do_idle()
 */
	.align	5
ENTRY(cpu_arm922_do_idle)
#if 0	/* add by Victor Yu. 06-09-2005 */
	mcr	p15, 0, r0, c7, c0, 4		@ Wait for interrupt
#endif
	mov	pc, lr


#ifndef CONFIG_CPU_DCACHE_WRITETHROUGH

/*
 *	flush_icache_all()
 *
 *	Unconditionally clean and invalidate the entire icache.
 */
ENTRY(arm922_flush_icache_all)
	mov	r0, #0
	mcr	p15, 0, r0, c7, c5, 0		@ invalidate I cache
	mov	pc, lr
ENDPROC(arm922_flush_icache_all)

/*
 *	flush_user_cache_all()
 *
 *	Clean and invalidate all cache entries in a particular
 *	address space.
 */
ENTRY(arm922_flush_user_cache_all)
	/* FALLTHROUGH */

/*
 *	flush_kern_cache_all()
 *
 *	Clean and invalidate the entire cache.
 */
ENTRY(arm922_flush_kern_cache_all)
#if 0	/* mask by Victor Yu. 05-30-2005 */
	mov	r2, #VM_EXEC
	mov	ip, #0
__flush_whole_cache:
	mov	r1, #(CACHE_DSEGMENTS - 1) << 5	@ 8 segments
1:	orr	r3, r1, #(CACHE_DENTRIES - 1) << 26 @ 64 entries
2:	mcr	p15, 0, r3, c7, c14, 2		@ clean+invalidate D index
	subs	r3, r3, #1 << 26
	bcs	2b				@ entries 63 to 0
	subs	r1, r1, #1 << 5
	bcs	1b				@ segments 7 to 0
	tst	r2, #VM_EXEC
	mcrne	p15, 0, ip, c7, c5, 0		@ invalidate I cache
	mcrne	p15, 0, ip, c7, c10, 4		@ drain WB
	mov	pc, lr
#else	/* add by Victor Yu. 05-30-2005 */
	mov     ip, #0

#ifndef CONFIG_CPU_ICACHE_DISABLE
        mcr     p15, 0, ip, c7, c5, 0           @ invalidate I cache
#endif

__flush_whole_cache:

#ifndef CONFIG_CPU_DCACHE_DISABLE
        mov     ip, #0
#  ifdef CONFIG_CPU_DCACHE_WRITETHROUGH
        mcr     p15, 0, ip, c7, c6, 0           @ invalidate D cache
#  else
        mcr     p15, 0, ip, c7,c14, 0           @ clean/invalidate D cache
#  endif
#endif /*CONFIG_CPU_DCACHE_DISABLE*/
	mov	pc, lr
#endif

/*
 *	flush_user_cache_range(start, end, flags)
 *
 *	Clean and invalidate a range of cache entries in the
 *	specified address range.
 *
 *	- start	- start address (inclusive)
 *	- end	- end address (exclusive)
 *	- flags	- vm_flags describing address space
 */
ENTRY(arm922_flush_user_cache_range)
#if 0	/* mask by Victor Yu. 05-30-2005 */
	mov	ip, #0
	sub	r3, r1, r0			@ calculate total size
	cmp	r3, #CACHE_DLIMIT
	bhs	__flush_whole_cache

1:	mcr	p15, 0, r0, c7, c14, 1		@ clean+invalidate D entry
	tst	r2, #VM_EXEC
	mcrne	p15, 0, r0, c7, c5, 1		@ invalidate I entry
	add	r0, r0, #CACHE_DLINESIZE
	cmp	r0, r1
	blo	1b
	tst	r2, #VM_EXEC
	mcrne	p15, 0, ip, c7, c10, 4		@ drain WB
	mov	pc, lr
#else	/* add by Victor Yu. 05-30-2005 */
        mov     ip, #0
        sub     r3, r1, r0                      @ calculate total size
#ifndef CONFIG_CPU_ICACHE_DISABLE
        tst     r2, #VM_EXEC                    @ executable region?
        mcrne   p15, 0, ip, c7, c5, 0           @ invalidate I cache
#endif

#ifndef CONFIG_CPU_DCACHE_DISABLE
	cmp     r3, #CACHE_DSIZE                @ total size >= limit?
        bhs     __flush_whole_cache             @ flush whole D cache

1:
#ifdef CONFIG_CPU_DCACHE_WRITETHROUGH
        mcr     p15, 0, r0, c7, c6, 1           @ invalidate D entry
#else
        mcr     p15, 0, r0, c7, c14, 1          @ clean and invalidate D entry
#endif
        add     r0, r0, #CACHE_DLINESIZE
        cmp     r0, r1
        bls     1b
#endif  /* CONFIG_CPU_DCACHE_DISABLE */
        mov     pc, lr
#endif

/*
 *	flush_kern_dcache_page(void *page)
 *
 *	Ensure no D cache aliasing occurs, either with itself or
 *	the I cache
 *
 *	- addr	- page aligned address
 */
ENTRY(arm922_flush_kern_dcache_page)
#if 0   /* mask by Victor Yu. 05-30-2005 */
	add	r1, r0, #PAGE_SZ
1:	mcr	p15, 0, r0, c7, c14, 1		@ clean+invalidate D entry
	add	r0, r0, #CACHE_DLINESIZE
	cmp	r0, r1
	blo	1b
	mov	r0, #0
	mcr	p15, 0, r0, c7, c5, 0		@ invalidate I cache
	mcr	p15, 0, r0, c7, c10, 4		@ drain WB
	mov	pc, lr
#else	/* add by Victor Yu. 05-30-2005 */
	add     r1, r0, #PAGE_SZ
        /* fall through */
#endif

/*
 *	coherent_kern_range(start, end)
 *
 *	Ensure coherency between the Icache and the Dcache in the
 *	region described by start, end.  If you have non-snooping
 *	Harvard caches, you need to implement this function.
 *
 *	- start	- virtual start address
 *	- end	- virtual end address
 */
ENTRY(arm922_coherent_kern_range)
	/* FALLTHROUGH */

/*
 *	coherent_user_range(start, end)
 *
 *	Ensure coherency between the Icache and the Dcache in the
 *	region described by start, end.  If you have non-snooping
 *	Harvard caches, you need to implement this function.
 *
 *	- start	- virtual start address
 *	- end	- virtual end address
 */
ENTRY(arm922_coherent_user_range)
#if 0	/* mask by Victor Yu. 06-09-2005 */
	bic	r0, r0, #CACHE_DLINESIZE - 1
1:	mcr	p15, 0, r0, c7, c10, 1		@ clean D entry
	mcr	p15, 0, r0, c7, c5, 1		@ invalidate I entry
	add	r0, r0, #CACHE_DLINESIZE
	cmp	r0, r1
	blo	1b
	mcr	p15, 0, r0, c7, c10, 4		@ drain WB
	mov	pc, lr
#else	/* add by Victor Yu. 06-09-2005 */
	bic     r0, r0, #CACHE_DLINESIZE-1
#if !(defined(CONFIG_CPU_DCACHE_DISABLE) && defined(CONFIG_CPU_ICACHE_DISABLE))
1:
#ifndef CONFIG_CPU_DCACHE_DISABLE
#ifdef CONFIG_CPU_DCACHE_WRITETHROUGH
        mcr     p15, 0, r0, c7, c6, 1           @ invalidate D entry
#else
        mcr     p15, 0, r0, c7, c14, 1          @ clean and invalidate D entry
#endif
#endif /* CONFIG_CPU_DCACHE_DISABLE */
#ifndef CONFIG_CPU_ICACHE_DISABLE
        mcr     p15, 0, r0, c7, c5, 1           @ invalidate I entry
#endif
        add     r0, r0, #CACHE_DLINESIZE
        cmp     r0, r1
        bls     1b                              @ Luke Lee 05/19/2005 blo->bls
#endif /* !(defined(CONFIG_CPU_DCACHE_DISABLE) && defined(CONFIG_CPU_ICACHE_DISABLE)) */
        mov     pc, lr
#endif

/*
 *	dma_inv_range(start, end)
 *
 *	Invalidate (discard) the specified virtual address range.
 *	May not write back any entries.  If 'start' or 'end'
 *	are not cache line aligned, those lines must be written
 *	back.
 *
 *	- start	- virtual start address
 *	- end	- virtual end address
 *
 * (same as v4wb)
 */
ENTRY(arm922_dma_inv_range)
#if 1	/* mask by Victor Yu. 05-30-2005 */
	tst	r0, #CACHE_DLINESIZE - 1
	bic	r0, r0, #CACHE_DLINESIZE - 1
	mcrne	p15, 0, r0, c7, c10, 1		@ clean D entry
	tst	r1, #CACHE_DLINESIZE - 1
	mcrne	p15, 0, r1, c7, c10, 1		@ clean D entry
1:	mcr	p15, 0, r0, c7, c6, 1		@ invalidate D entry
	add	r0, r0, #CACHE_DLINESIZE
	cmp	r0, r1
	blo	1b
	mcr	p15, 0, r0, c7, c10, 4		@ drain WB
	mov	pc, lr
#else	/* add by Victor Yu. 05-30-2005 */
#ifndef CONFIG_CPU_DCACHE_DISABLE
# ifndef CONFIG_CPU_DCACHE_WRITETHROUGH
        tst     r0, #CACHE_DLINESIZE -1
        bic     r0, r0, #CACHE_DLINESIZE -1
        mcrne   p15, 0, r0, c7, c10, 1          @ clean boundary D entry
        mcr     p15, 0, r1, c7, c10, 1          @ clean boundary D entry
# else
        bic     r0, r0, #CACHE_DLINESIZE -1
# endif	/* CONFIG_CPU_DCACHE_WRITETHROUGH */
1:      mcr     p15, 0, r0, c7, c6, 1           @ invalidate D entry
        add     r0, r0, #CACHE_DLINESIZE
        cmp     r0, r1
        bls     1b                              @ Luke Lee 05/19/2005 blo->bls
#endif /* CONFIG_CPU_DCACHE_DISABLE */
        mov     pc, lr
#endif

/*
 *	dma_clean_range(start, end)
 *
 *	Clean the specified virtual address range.
 *
 *	- start	- virtual start address
 *	- end	- virtual end address
 *
 * (same as v4wb)
 */
ENTRY(arm922_dma_clean_range)
#if 0	/* mask by Victor Yu. 05-30-2005 */
	bic	r0, r0, #CACHE_DLINESIZE - 1
1:	mcr	p15, 0, r0, c7, c10, 1		@ clean D entry
	add	r0, r0, #CACHE_DLINESIZE
	cmp	r0, r1
	blo	1b
	mcr	p15, 0, r0, c7, c10, 4		@ drain WB
	mov	pc, lr
#else	/* add by Victor Yu. 05-30-2005 */
#ifndef CONFIG_CPU_DCACHE_DISABLE
#ifndef CONFIG_CPU_DCACHE_WRITETHROUGH
        bic     r0, r0, #CACHE_DLINESIZE - 1
1:      mcr     p15, 0, r0, c7, c10, 1          @ clean D entry
        add     r0, r0, #CACHE_DLINESIZE
        cmp     r0, r1
        bls     1b                              @ Luke Lee 05/19/2005 blo->bls
#endif	/* CONFIG_CPU_DCACHE_WRITETHROUGH */
#endif	/* CONFIG_CPU_DCACHE_DISABLE */
        mov     pc, lr
#endif

/*
 *	dma_flush_range(start, end)
 *
 *	Clean and invalidate the specified virtual address range.
 *
 *	- start	- virtual start address
 *	- end	- virtual end address
 */
//ENTRY(arm922_dma_flush_range)
#if 0	/* mask by Victor Yu. 06-09-2005 */
	bic	r0, r0, #CACHE_DLINESIZE - 1
1:	mcr	p15, 0, r0, c7, c14, 1		@ clean+invalidate D entry
	add	r0, r0, #CACHE_DLINESIZE
	cmp	r0, r1
	blo	1b
	mcr	p15, 0, r0, c7, c10, 4		@ drain WB
	mov	pc, lr
#else	/* add by Victor Yu. 06-09-2005 */
	.globl  arm922_dma_flush_range
        .set    arm922_dma_flush_range, arm922_coherent_kern_range
#endif

/*
 *	dma_map_area(start, size, dir)
 *	- start	- kernel virtual start address
 *	- size	- size of region
 *	- dir	- DMA direction
 */
ENTRY(arm922_dma_map_area)
	add	r1, r1, r0
	cmp	r2, #DMA_TO_DEVICE
	beq	arm922_dma_clean_range
	bcs	arm922_dma_inv_range
	b	arm922_dma_flush_range
ENDPROC(arm922_dma_map_area)

/*
 *	dma_unmap_area(start, size, dir)
 *	- start	- kernel virtual start address
 *	- size	- size of region
 *	- dir	- DMA direction
 */
ENTRY(arm922_dma_unmap_area)
	mov	pc, lr
ENDPROC(arm922_dma_unmap_area)

ENTRY(arm922_cache_fns)
	.long	arm922_flush_icache_all
	.long	arm922_flush_kern_cache_all
	.long	arm922_flush_user_cache_all
	.long	arm922_flush_user_cache_range
	.long	arm922_coherent_kern_range
	.long	arm922_coherent_user_range
	.long	arm922_flush_kern_dcache_page
	.long	arm922_dma_inv_range
	.long	arm922_dma_clean_range
	.long	arm922_dma_flush_range
#if 1	/* add by Victor Yu. 06-09-2005 */
	.size	arm922_cache_fns, . - arm922_cache_fns
#endif
#endif


ENTRY(cpu_arm922_dcache_clean_area)
#ifndef CONFIG_CPU_DCACHE_WRITETHROUGH
1:	mcr	p15, 0, r0, c7, c10, 1		@ clean D entry
	add	r0, r0, #CACHE_DLINESIZE
	subs	r1, r1, #CACHE_DLINESIZE
	bhi	1b
#endif
	mov	pc, lr

/* =============================== PageTable ============================== */

/*
 * cpu_arm922_switch_mm(pgd)
 *
 * Set the translation base pointer to be as described by pgd.
 *
 * pgd: new page tables
 */
	.align	5
ENTRY(cpu_arm922_switch_mm)
#ifdef CONFIG_MMU
	mov	ip, #0
#if 0	/* mask by Victor Yu. 06-09-2005 */
#ifdef CONFIG_CPU_DCACHE_WRITETHROUGH
	mcr	p15, 0, ip, c7, c6, 0		@ invalidate D cache
#else
@ && 'Clean & Invalidate whole DCache'
@ && Re-written to use Index Ops.
@ && Uses registers r1, r3 and ip

	mov	r1, #(CACHE_DSEGMENTS - 1) << 5	@ 4 segments
1:	orr	r3, r1, #(CACHE_DENTRIES - 1) << 26 @ 64 entries
2:	mcr	p15, 0, r3, c7, c14, 2		@ clean & invalidate D index
	subs	r3, r3, #1 << 26
	bcs	2b				@ entries 63 to 0
	subs	r1, r1, #1 << 5
	bcs	1b				@ segments 7 to 0
#endif	/* CONFIG_CPU_DCACHE_WRITETHROUGH */
#else	/* add by Victor Yu. 06-09-2005 */
#ifndef CONFIG_CPU_DCACHE_DISABLE
#ifdef CONFIG_CPU_DCACHE_WRITETHROUGH
        mcr     p15, 0, ip, c7, c6, 0           @ invalidate D cache
#else
        mcr     p15, 0, ip, c7, c14, 0          @ Clean and invalidate whole DCache
#endif	/* CONFIG_CPU_DCACHE_WRITETHROUGH */
#endif	/*CONFIG_CPU_DCACHE_DISABLE*/
#endif
#endif /* End CONFIG_MMU */
	mcr	p15, 0, ip, c7, c5, 0		@ invalidate I cache
	mcr	p15, 0, ip, c7, c10, 4		@ drain WB
#if 1	/* add by Victor Yu. 06-09-2005 */
#ifdef CONFIG_ARCH_MOXACPU
	bic     r0, r0, #0xff                   @ clear bits [7:0]
        bic     r0, r0, #0x3f00                 @ clear bits [13:8]
#endif
#endif
	mcr	p15, 0, r0, c2, c0, 0		@ load page table pointer
	mcr	p15, 0, ip, c8, c7, 0		@ invalidate I & D TLBs
#if 1	/* add by Victor Yu. 06-09-2005 */
#ifdef CONFIG_ARCH_MOXACPU
	nop
        nop
#endif
#endif
	mov	pc, lr

/*
 * cpu_arm922_set_pte_ext(ptep, pte, ext)
 *
 * Set a PTE and flush it out
 */
	.align	5
ENTRY(cpu_arm922_set_pte_ext)
#ifdef CONFIG_MMU
	armv3_set_pte_ext
#if 0	/* mask by Victor Yu. 06-09-2005 */
	mov	r0, r0
	mcr	p15, 0, r0, c7, c10, 1		@ clean D entry
	mcr	p15, 0, r0, c7, c10, 4		@ drain WB
#else	// add by Victor Yu. 06-09-2005 */
	mov     r2, #0
        mcr     p15, 0, r2, c7, c10, 0          @ clean D cache all
#endif
#endif  /* CONFIG_MMU */
	mov	pc, lr

	__CPUINIT

	.type	__arm922_setup, #function
__arm922_setup:
	ledset r7, r8 5 @debug
	mov	r0, #0
	mcr	p15, 0, r0, c7, c7		@ invalidate I,D caches on v4
	mcr	p15, 0, r0, c7, c10, 4		@ drain write buffer on v4
#ifdef CONFIG_MMU
	mcr	p15, 0, r0, c8, c7		@ invalidate I,D TLBs on v4
#endif
#ifdef CONFIG_ARCH_MOXACPU
	mcr     p15, 0, r0, c7, c5, 5           @ invalidate IScratchpad RAM
	mcr     p15, 0, r0, c7, c5, 6           @ invalidate BTB all

        mov     r0, #1
        mcr     p15, 0, r0, c1, c1, 0           @ turn-on ECR

        mrc     p15, 0, r0, c9, c1, 0           @ DScratchpad
        bic     r0, r0, #1
        mcr     p15, 0, r0, c9, c1, 0		@ disable it

        mrc     p15, 0, r0, c9, c1, 1           @ IScratchpad
        bic     r0, r0, #1
        mcr     p15, 0, r0, c9, c1, 1		@ disable it

	mrc	p15, 0, r0, c1, c0, 0		@ diable Branch Target buffer
	bic	r0, r0, #0x0800
        mcr     p15, 0, r0, c1, c0, 0		

        mov     r0, #0
        mcr     p15, 0, r0, c1, c1, 0           @ turn-off ECR
#endif	/* CONFIG_ARCH_MOXACPU */
	adr	r5, arm922_crval
	ldmia	r5, {r5, r6}
	mrc	p15, 0, r0, c1, c0		@ get control register v4
	bic	r0, r0, r5
	orr	r0, r0, r6
	mov	pc, lr
	.size	__arm922_setup, . - __arm922_setup

	/*
	 *  R
	 * .RVI ZFRS BLDP WCAM
	 * ..11 0001 ..11 0101
	 * 
	 */
	.type	arm922_crval, #object
arm922_crval:
	crval	clear=0x00003f3f, mmuset=0x00003135, ucset=0x00001130

	__INITDATA

/*
 * Purpose : Function pointers used to access above functions - all calls
 *	     come through these
 */
	.type	arm922_processor_functions, #object
arm922_processor_functions:
#if 0	/* mask by Victor Yu. 06-08-2005 */
	.word	v4t_early_abort
#else	/* add by Victor YU. 06-08-2005 */
	.word	v4_early_abort
#endif
	.word	legacy_pabort
	.word	cpu_arm922_proc_init
	.word	cpu_arm922_proc_fin
	.word	cpu_arm922_reset
	.word   cpu_arm922_do_idle
	.word	cpu_arm922_dcache_clean_area
	.word	cpu_arm922_switch_mm
	.word	cpu_arm922_set_pte_ext
	.size	arm922_processor_functions, . - arm922_processor_functions

	.section ".rodata"

	.type	cpu_arch_name, #object
cpu_arch_name:
	.asciz	"armv4t"
	.size	cpu_arch_name, . - cpu_arch_name

	.type	cpu_elf_name, #object
cpu_elf_name:
	.asciz	"v4"
	.size	cpu_elf_name, . - cpu_elf_name

	.type	cpu_arm922_name, #object
cpu_arm922_name:
	.asciz	"ARM922T"
#ifndef CONFIG_CPU_ICACHE_DISABLE
	.ascii	"i"
#endif
#ifndef CONFIG_CPU_DCACHE_DISABLE
	.ascii	"d"
#ifdef CONFIG_CPU_DCACHE_WRITETHROUGH
	.ascii	"(wt)"
#else
	.ascii	"(wb)"
#endif
#endif
	.ascii	"\0"
	.size	cpu_arm922_name, . - cpu_arm922_name

	.align


	.section ".proc.info.init", #alloc, #execinstr

#ifdef CONFIG_CPU_DCACHE_WRITETHROUGH
#define __PMD_SECT_BUFFERABLE   0
#else
#define __PMD_SECT_BUFFERABLE   PMD_SECT_BUFFERABLE
#endif
/* define in arm/include/asm/procinfo.h */

	.type	__arm922_proc_info,#object
__arm922_proc_info:
#ifdef CONFIG_ARCH_MOXACPU	/* add by Victor Yu. 05-23-2005 */
	.long	0x66015261
	.long	0xff01fff1
	@mm_mmu_flags
	.long   PMD_TYPE_SECT | \
                __PMD_SECT_BUFFERABLE | \
                PMD_SECT_CACHEABLE | \
                PMD_BIT4 | \
                PMD_SECT_AP_WRITE | \
                PMD_SECT_AP_READ
	@io_mmu_flags
	.long   PMD_TYPE_SECT | \
		PMD_BIT4 | \
		PMD_SECT_AP_WRITE | \
		PMD_SECT_AP_READ
#else	/* mask by Victor Yu. 05-23-2005 */
	.long	0x41009220
	.long	0xff00fff0
	.long   PMD_TYPE_SECT | \
		PMD_SECT_BUFFERABLE | \
		PMD_SECT_CACHEABLE | \
		PMD_BIT4 | \
		PMD_SECT_AP_WRITE | \
		PMD_SECT_AP_READ
	.long   PMD_TYPE_SECT | \
		PMD_BIT4 | \
		PMD_SECT_AP_WRITE | \
		PMD_SECT_AP_READ
#endif	/* CONFIG_ARCH_MOXACPU */

	b	__arm922_setup  @cpu_flush_function
	.long	cpu_arch_name
	.long	cpu_elf_name
	.long	HWCAP_SWP | HWCAP_HALF | HWCAP_THUMB
	.long	cpu_arm922_name
	.long	arm922_processor_functions
	.long	v4wbi_tlb_fns
	.long	v4wb_user_fns
#ifndef CONFIG_CPU_DCACHE_WRITETHROUGH
	.long	arm922_cache_fns
#else
	.long	v4wt_cache_fns
#endif
	.size	__arm922_proc_info, . - __arm922_proc_info
