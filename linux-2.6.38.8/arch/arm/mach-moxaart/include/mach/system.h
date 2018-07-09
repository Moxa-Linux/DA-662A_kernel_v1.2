/*
 * arch/arm/mach-moxaart/include/mach/system.h
 *
 * Copyright (c) 1999 Nicolas Pitre <nico@cam.org>
 * Copyright (c) 2001 RidgeRun, Inc (http://www.ridgerun.org)
 *
 */
#include <linux/io.h>
#include <linux/spinlock.h>


extern void moxaart_wdt_set_flash_read_mode(void); 
extern void moxaart_wdt_set_mode_timeout(int mode, int tick); 
#if 0 // Jared, 2012-11-12
extern int cpu_do_idle(void);
extern void cpu_reset(unsigned long addr) __attribute__((noreturn));
#endif

static inline void arch_idle(void)
{
	cpu_do_idle();
}
#if 0 // Jared, 2012-11-12
extern inline void arch_reset(char mode, const char *cmd)
#else
inline void arch_reset(char mode, const char *cmd)
#endif
{
    spinlock_t lock = SPIN_LOCK_UNLOCKED; 

    spin_lock_irq(&lock); 
	moxaart_wdt_set_flash_read_mode(); 

	if ( mode == 's' ) {
		// Jump to ROM address 0
		cpu_reset(0);
	} else {
		/* set to reset mode, and timeout = 1 tick */
		moxaart_wdt_set_mode_timeout(1, 1); 
	}

    /* keep here until reboot */
    while(1); 
}
