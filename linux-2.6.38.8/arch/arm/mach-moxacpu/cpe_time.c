/*
 * time.c  Timer functions for cpe
 */

#include <linux/interrupt.h>
#include <linux/irq.h>

#include <linux/time.h>
#include <linux/timex.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <asm/mach/time.h>
#include <asm/io.h>
//#include <mach/hardware.h>
//#include <mach/time.h>
//#include <mach/time.h>

#include <mach/cpe/cpe.h>
#include <mach/cpe_int.h>
#include <mach/cpe_time.h>
#define MAX_TIMER   2

cpe_time_reg_t *TimerBase[] ={  0, 
                                (cpe_time_reg_t *)CPE_TIMER1_VA_BASE,
                                (cpe_time_reg_t *)CPE_TIMER2_VA_BASE
                             };

unsigned int cpe_timer_enable(unsigned int timer)
{   
    cpe_time_ctrl_t *t=(cpe_time_ctrl_t *)(CPE_TIMER1_VA_BASE+TIMER_CR);

    if ((timer == 0) || (timer > MAX_TIMER))
		return 0;

    switch(timer)
    {
        case 1:
    	    t->Tm1En=1;
    	    t->Tm1OfEn=1;
#if 1	// add by Victor Yu. 06-01-2005
	    t->Tm1Clock = 0;	// use PCLK
#endif
        	break;    
        case 2:
    	    t->Tm2En=1;
    	    t->Tm2OfEn=1;
#if 1	// add by Victor Yu. 06-01-2005
	    t->Tm2Clock = 0;	// use PCLK
#endif
        	break; 
        case 3:
    	    t->Tm3En=1;
    	    t->Tm3OfEn=1;
#if 1	// add by Victor Yu. 06-01-2005
	    t->Tm3Clock = 0;	// use PCLK
#endif
    	    break; 
        
        default:
        	break;   
    }
    return 1;
}


/* This routine stops the specified timer hardware. */
unsigned int cpe_timer_disable(unsigned int timer)
{
    cpe_time_ctrl_t *t=(cpe_time_ctrl_t *)(CPE_TIMER1_VA_BASE + TIMER_CR);
    
    if ((timer == 0) || (timer > MAX_TIMER))
		return 0;

    switch(timer)
    {
        case 1:
    	    t->Tm1En=0;
    	    t->Tm1OfEn=0;
    	    break;    
        case 2:
    	    t->Tm2En=0;
    	    t->Tm2OfEn=0;
    	    break; 
        case 3:
    	    t->Tm3En=0;
    	    t->Tm3OfEn=0;
    	    break; 
        
        default:
    	break;
   
    }
    return 1;
}

void cpe_timer_set_counter(unsigned int timer, unsigned int value)
{
    volatile cpe_time_reg_t *t = TimerBase[timer];    
    t->TimerValue = value; 
}

void cpe_timer_set_reload(unsigned int timer, unsigned int value)
{
    volatile cpe_time_reg_t *t = TimerBase[timer];    
    t->TimerLoad = value;   
}

// --------------------------------------------------------------------
//	warning:
//		timer = 1, 2, 3
// --------------------------------------------------------------------
unsigned int cpe_timer_get_counter(unsigned int timer)
{
    volatile cpe_time_reg_t *t = TimerBase[timer];
    return t->TimerValue;
}

#define SET_COUNTER	(APB_CLK / HZ)
#define USED_TIMER	1
unsigned long cpe_gettimeoffset (void)
{
#if 1	// add by Victor Yu. 02-26-2007
	unsigned long	volatile offsetticks;

	offsetticks = cpe_timer_get_counter(USED_TIMER);
	offsetticks = SET_COUNTER - offsetticks;
#if 0	// mask by Victor Yu. 01-31-2008
	if ( (*(volatile unsigned int *)(CPE_IC_VA_BASE+IRQ_STATUS_REG) & IRQ_TIMER1) ||
	     (*(volatile unsigned int *)(CPE_IC_VA_BASE+IRQ_STATUS_REG) & IRQ_TIMER1) ) {
#else
	if ( *(volatile unsigned int *)(CPE_TIMER1_VA_BASE+TIMER_INTR_STATE) ) {
		//printk("[has timer interrupt pending , %d !]\n", offsetticks);
#endif
		offsetticks = cpe_timer_get_counter(USED_TIMER);
		offsetticks = SET_COUNTER - offsetticks;
		offsetticks += SET_COUNTER;
	}
	offsetticks = offsetticks / (APB_CLK / 1000000);	// tansfer ticks to usec
	return offsetticks;
#else	// 02-26-2007
	return 0;
#endif	// 02-26-2007
}

static irqreturn_t cpe_timer_interrupt(int irq, void *dev_id)
{
	timer_tick();
	*(volatile unsigned int *)(CPE_TIMER1_VA_BASE+TIMER_INTR_STATE) = 0;
	//last_ticks=0;
	return IRQ_HANDLED;
}

/* 
 *
 * IRQF_DISABLED - keep irqs disabled when calling the action handler.
 *                 DEPRECATED. This flag is a NOOP and scheduled to be removed
 * IRQF_SAMPLE_RANDOM - irq is used to feed the random generator
 * IRQF_SHARED - allow sharing the irq among several devices
 * IRQF_PROBE_SHARED - set by callers when they expect sharing mismatches to occur
 * IRQF_TIMER - Flag to mark this interrupt as timer interrupt
 * IRQF_PERCPU - Interrupt is per cpu
 * IRQF_NOBALANCING - Flag to exclude this interrupt from irq balancing
 * IRQF_IRQPOLL - Interrupt is used for polling (only the interrupt that is
 *                registered first in an shared interrupt is considered for
 *                performance reasons)
 * IRQF_ONESHOT - Interrupt is not reenabled after the hardirq handler finished.
 *                Used by threaded interrupts which need to keep the
 *                irq line disabled until the threaded handler has been run.
 * IRQF_NO_SUSPEND - Do not disable this IRQ during suspend
 */
static struct irqaction cpe_timer_irq = {
	.name		= "Moxa CPE timer interrupt",
	.flags		= IRQF_DISABLED | IRQF_TIMER,
	//.flags		= SA_INTERRUPT,
	.handler	= cpe_timer_interrupt
};

void __init cpe_timer_init(void)
{
#ifdef TIMER_INC_MODE
	cpe_timer_set_reload(1, 0xffffffff - APB_CLK/HZ);
	cpe_timer_set_counter(1, 0xffffffff - APB_CLK/HZ);
#else
	cpe_timer_set_reload(1, APB_CLK/HZ);
	cpe_timer_set_counter(1, APB_CLK/HZ);
#endif

	if( !cpe_timer_enable(1) ) {
                panic("can not enable timer\n");
	}

	printk("IRQ timer at interrupt number 0x%x clock %d\r\n",IRQ_TIMER1,APB_CLK);
	cpe_int_set_irq(IRQ_TIMER1, EDGE, L_ACTIVE);
	setup_irq(IRQ_TIMER1, &cpe_timer_irq);
}
