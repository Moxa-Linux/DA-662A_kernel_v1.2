/* 
 * linux/arch/arm/mach-moxamacro/include/mach/timer.h
 *
 * Copyright 2010 Moxa Tech., Inc.
 *	Vincent Wu.<vincentr.wu@moxa.com>
 *	http://www.moxa.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/



#define MOXAARM11_TIMERREG(x) (MOXAARM11_VA_TIMER_BASE + (x))


#define MOXAARM11_TIMER_STST	      MOXAARM11_TIMERREG(0x00)
#define MOXAARM11_TIMER_RELOAD	      MOXAARM11_TIMERREG(0x04)
#define MOXAARM11_TIMER_MATCH1	      MOXAARM11_TIMERREG(0x08)
#define MOXAARM11_TIMER_MATCH2	      MOXAARM11_TIMERREG(0x0c)
#define MOXAARM11_TIMER_CONTROL	      MOXAARM11_TIMERREG(0x30)


//#define TICKS_PER_nSEC			49500
#define TICKS_PER_nSEC			50000
#define TICKS_PER_uSEC			50

#define nSEC_1				1
#define nSEC_5				(nSEC_1 * 5)
#define nSEC_10				(nSEC_1 * 10)
#define nSEC_25				(nSEC_1 * 25)
#define mSEC_1				1000
#define mSEC_5				(mSEC_1 * 5)
#define mSEC_10				(mSEC_1 * 10)
#define mSEC_25				(mSEC_1 * 25)
#define SEC_1				(mSEC_1 * 1000)
#define TICKS2USECS(x)		((mSEC_1 * x) / TICKS_PER_nSEC)


#define TIMER_INTERVAL		(TICKS_PER_nSEC * nSEC_10)
#define TIMER_RELOAD_VALUE		(TIMER_INTERVAL)
#define TIMER_ENABLE		0x0001

