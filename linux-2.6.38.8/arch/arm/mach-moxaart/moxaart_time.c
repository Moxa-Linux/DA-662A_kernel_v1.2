/*
 * moxaart_time.c  Timer functions for moxaart
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

#include <mach/moxaart/moxaart.h>
#include <mach/moxaart_int.h>
#include <mach/moxaart_time.h>
#define MAX_TIMER   2

moxaart_time_reg_t *TimerBase[] ={  0, 
                                (moxaart_time_reg_t *)MOXAART_TIMER1_VA_BASE,
                                (moxaart_time_reg_t *)MOXAART_TIMER2_VA_BASE
                             };

unsigned int moxaart_timer_enable(unsigned int timer)
{   
    moxaart_time_ctrl_t *t=(moxaart_time_ctrl_t *)(MOXAART_TIMER1_VA_BASE+TIMER_CR);

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
unsigned int moxaart_timer_disable(unsigned int timer)
{
    moxaart_time_ctrl_t *t=(moxaart_time_ctrl_t *)(MOXAART_TIMER1_VA_BASE + TIMER_CR);
    
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

void moxaart_timer_set_counter(unsigned int timer, unsigned int value)
{
    volatile moxaart_time_reg_t *t = TimerBase[timer];    
    t->TimerValue = value; 
}

void moxaart_timer_set_reload(unsigned int timer, unsigned int value)
{
    volatile moxaart_time_reg_t *t = TimerBase[timer];    
    t->TimerLoad = value;   
}

// --------------------------------------------------------------------
//	warning:
//		timer = 1, 2, 3
// --------------------------------------------------------------------
unsigned int moxaart_timer_get_counter(unsigned int timer)
{
    volatile moxaart_time_reg_t *t = TimerBase[timer];
    return t->TimerValue;
}

#define SET_COUNTER	(APB_CLK / HZ)
#define USED_TIMER	1
unsigned long moxaart_gettimeoffset (void)
{
#if 1	// add by Victor Yu. 02-26-2007
	unsigned long	volatile offsetticks;

	offsetticks = moxaart_timer_get_counter(USED_TIMER);
	offsetticks = SET_COUNTER - offsetticks;
#if 0	// mask by Victor Yu. 01-31-2008
	if ( (*(volatile unsigned int *)(MOXAART_IC_VA_BASE+IRQ_STATUS_REG) & IRQ_TIMER1) ||
	     (*(volatile unsigned int *)(MOXAART_IC_VA_BASE+IRQ_STATUS_REG) & IRQ_TIMER1) ) {
#else
	if ( *(volatile unsigned int *)(MOXAART_TIMER1_VA_BASE+TIMER_INTR_STATE) ) {
		//printk("[has timer interrupt pending , %d !]\n", offsetticks);
#endif
		offsetticks = moxaart_timer_get_counter(USED_TIMER);
		offsetticks = SET_COUNTER - offsetticks;
		offsetticks += SET_COUNTER;
	}
	offsetticks = offsetticks / (APB_CLK / 1000000);	// tansfer ticks to usec
	return offsetticks;
#else	// 02-26-2007
	return 0;
#endif	// 02-26-2007
}

static irqreturn_t moxaart_timer_interrupt(int irq, void *dev_id)
{
	timer_tick();
	*(volatile unsigned int *)(MOXAART_TIMER1_VA_BASE+TIMER_INTR_STATE) = 0;
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
static struct irqaction moxaart_timer_irq = {
	.name		= "Moxa MOXAART timer interrupt",
	.flags		= IRQF_DISABLED | IRQF_TIMER,
	//.flags		= SA_INTERRUPT,
	.handler	= moxaart_timer_interrupt
};

#ifdef CONFIG_MOXAART_FIQ
int moxaart_timer_interrupt_fiq(void)
{
	static unsigned int count = 0;

	count++;
	if(count == 100*5) // 10ms * 100 = 1s, 1s * 5 = 5s
	{
		printk("### FIQ is enabled!!! ###\n");
		count = 0;
	}

	return true;	
}
#endif

void __init moxaart_timer_init(void)
{
#ifdef CONFIG_MOXAART_FIQ
	unsigned int *ptr_fiq = (unsigned int *)(MOXAART_IC_VA_BASE + 0x20); // INTC's FIQ
	unsigned int *ptr_timer = (unsigned int *)(MOXAART_TIMER_VA_BASE + TIMER_CR);
#endif

#ifdef TIMER_INC_MODE
	moxaart_timer_set_reload(1, 0xffffffff - APB_CLK/HZ);
	moxaart_timer_set_counter(1, 0xffffffff - APB_CLK/HZ);
#else
	moxaart_timer_set_reload(1, APB_CLK/HZ);
	moxaart_timer_set_counter(1, APB_CLK/HZ);	
#endif

	if( !moxaart_timer_enable(1) ) {
                panic("can not enable timer\n");
	}

	printk("IRQ timer at interrupt number 0x%x clock %d\r\n",IRQ_TIMER1,APB_CLK);
	moxaart_int_set_irq(IRQ_TIMER1, EDGE, L_ACTIVE);
	setup_irq(IRQ_TIMER1, &moxaart_timer_irq);

#ifdef CONFIG_MOXAART_FIQ
	moxaart_timer_set_reload(2, 0xffffffff - APB_CLK/HZ); // Timer2 is FIQ (Default is 10ms, you can adjust HZ to change it)
	moxaart_timer_set_counter(2, 0xffffffff - APB_CLK/HZ);

	// enable Timer1 and Timer2 (there is a compiler issue for moxaart_timer_enable, so Timer2 can't be enabled by it)
	*ptr_timer = 0xc2d; // 11th and 12th bit must be 1, but there are no descriptions of them in MoxaART document

	printk("FIQ timer at interrupt number 0x%x clock %d\r\n",IRQ_TIMER2,APB_CLK);
	*(ptr_fiq + 1) = 0x00004000; // enable register
	*(ptr_fiq + 2) = 0xffffffff; // interrupt clear register
	*(ptr_fiq + 3) = 0x00004000; // trigger mode register
	*(ptr_fiq + 4) = 0x00004000; // trigger level register	
#endif	
}
