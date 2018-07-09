#ifndef _CPE_TIME_H_
#define _CPE_TIME_H_
#include <mach/cpe/cpe.h>
#include <mach/cpe_int.h>

#define TIMER1_COUNT                0x0
#define TIMER1_LOAD                 0x4
#define TIMER1_MATCH1               0x8
#define TIMER1_MATCH2               0xC
#define TIMER2_COUNT                0x10
#define TIMER2_LOAD                 0x14
#define TIMER2_MATCH1               0x18
#define TIMER2_MATCH2               0x1C
#define TIMER3_COUNT                0x20
#define TIMER3_LOAD                 0x24
#define TIMER3_MATCH1               0x28
#define TIMER3_MATCH2               0x2C 
#define TIMER_CR                    0x30
#define TIMER_INTR_STATE	0x34
#define TIMER_INTR_MASK		0x38

typedef struct
{       
	unsigned int Tm1En:1;		// Timer1 enable bit
	unsigned int Tm1Clock:1;	// Timer1 clock source (0: PCLK, 1: EXT1CLK)
	unsigned int Tm1OfEn:1;		// Timer1 over flow interrupt enable bit
	unsigned int Tm2En:1;
	unsigned int Tm2Clock:1;
	unsigned int Tm2OfEn:1;
	unsigned int Tm3En:1;
	unsigned int Tm3Clock:1;
	unsigned int Tm3OfEn:1;       
	unsigned int Reserved;        
} cpe_time_ctrl_t;

typedef struct 
{
	unsigned int TimerValue;
	unsigned int TimerLoad;
	unsigned int TimerMatch1;
	unsigned int TimerMatch2;
} cpe_time_reg_t;

#if 0	// mask by Victor Yu. 09-06-2007
extern cpe_time_reg_t *TimerBase[];
extern unsigned int cpe_timer_enable(unsigned int timer);
extern unsigned int cpe_timer_disable(unsigned int timer);
extern void cpe_timer_set_counter(unsigned int timer, unsigned int value);
extern void cpe_timer_set_reload(unsigned int timer, unsigned int value);
extern unsigned int cpe_timer_get_counter(unsigned int timer);
extern unsigned long cpe_gettimeoffset(void);
#endif

#endif	// _CPE_TIME_H
