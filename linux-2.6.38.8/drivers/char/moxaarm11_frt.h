/*  Copyright (C) MOXA Inc. All rights reserved.

    This software is distributed under the terms of the
    MOXA License.  See the file COPYING-MOXA for details.
*/
/*
    frt.h

    Function declarations of FRT.
    
    2010-09-28	Chin-Fu Yang.			Create it.
*/
#ifndef _FRT_H
#define _FRT_H


/*------------------------------------------------------ Macro / Define -----------------------------*/

#define VPlong    		*(vulong *)
#define VPshort  		*(vushort *)
#define VPchar   		*(vuchar *)


#ifndef ON
#define ON		1
#endif

#ifndef OFF
#define	OFF		0
#endif

#ifndef WRITE
#define WRITE 1
#endif

#ifndef READ
#define READ 0
#endif

#define SET_BIT(b)		(1UL << (b))		// Set Bit b (ex.  00000000100000)
#define SET_BITS(h,l)	(((1UL << ( (h) - (l) + 1) ) - 1) << l)	// Set Bit(fill) From L to H  (ex. 000011100000)
#define SET_BVAL(v,p)	((v) << (p))		// Set value 'v' to bit position 'p' (ex. 0x0003F800)

#define MOXAARM11_FRT_SPCE_OFFSET	0x100

#define MOXAARM11_FRT_MAX				8
	
#define MOXAARM11_FRT_INTR_STS					(0x900)	// FRT interrupt Status

#define MOXAARM11_FRT_MAJOR			250
#define MOXAARM11_FRT_MINOR_START			0

#define MOXAARM11_FRT_IRQ				20

#define MOXAARM11_FRT_DBG


#ifdef MOXAARM11_FRT_DBG
//	#define dbg(msg, arg...) printk("*** %s: %s()[%d]: " msg, __FILE__, __FUNCTION__, __LINE__, ##arg)
	#define dbg(msg, arg...) printk("%s()[%d],"msg, __func__, __LINE__, ##arg)
#else
	#define dbg(msg, arg...) 
#endif


// IOCTL
#define IOCTL_OUTPUT_CMP_INTR				10
#define IOCTL_OVERFLOW_INTR				11
#define IOCTL_OUTPUT_CMP_FLAG				12		// reserved ~~~
#define IOCTL_OVERFLOW_FLAG				13		// reserved ~~~
#define IOCTL_FRC_COUNTER					14
#define IOCTL_OUTPUT_CMP					15		
#define IOCTL_SELECT_FRC_CLK_SRC			16
#define IOCTL_OUTPUT_CMP_SELECT			17		// reserved ~~~
#define IOCTL_FORCE_ORCA					18		// reserved ~~~
#define IOCTL_FORCE_ORCB					19		// reserved ~~~
#define IOCTL_OUTPUT_CMP_PIN				20
#define IOCTL_OUTPUT_CMP_LEVEL			21
#define IOCTL_OUTPUT_CMP_STOP				22
#define IOCTL_OUTPUT_CMP_STOP_STS		23		// reserved ~~~
#define IOCTL_OUTPUT_CMP_COUNTER_CLR		24
#define IOCTL_FORCE_TIMER_FRC_NO_SYNC	25
#define IOCTL_SELECT_TIMER_CLK_SRC		26
#define IOCTL_TIMER_COUNTER				27
#define IOCTL_GET_INTR_STS					28		// reserved ~~~

#define IOCTL_SLEEP			29	

#define IOCTL_DBG							99


/*------------------------------------------------------ Structure ----------------------------------*/
typedef volatile unsigned char	vuchar;
typedef volatile unsigned long	vulong;
typedef volatile unsigned short	vushort;


struct moxaarm11_frt_reg_struct{
	u32 timer_interrupt_enable;	// offset 0x00, Timer Interrupt Enable Register 
	u32 free_ctrl_sts;				// offset 0x04, Timer Control/Status Register 
	u32 free_counter;				// offset 0x08, Free Running Counter Register 
	u32 free_output_cmp_a_b;		// offset 0x0c, Output Compare Register A/ Output Compare Register B
	u32 timer_ctrl;				// offset 0x10, Timer Control Register 1
	u32 timer_output_cmp_ctrl;		// offset 0x14, Timer Output Compare Control Register 1
	u32 reserved0[4];						// offset 0x18~0x24
	u32 timer_ctrl_sts;			// offset 0x28, Timer Counter Control Status Register 1
	u32 timer_counter;				// offset 0x2C, Timer Counter Register 1
};


struct moxaarm11_frt_struct {
	int					irqno;
	u32					physical_base;
	u32					physical_len;
	void __iomem			*virtual_base;
	struct moxaarm11_frt_reg_struct *frt_regs[MOXAARM11_FRT_MAX];
	int 					ref_count[MOXAARM11_FRT_MAX];
	u32 					*intr_sts;
	wait_queue_head_t		cmp_a_wait;
	wait_queue_head_t		cmp_b_wait;
	wait_queue_head_t		overflow_wait;
};




#define FRT_IOCTL_A			0
#define FRT_IOCTL_B			1
#define FRT_IOCTL_OVER		2

#define FRT_IOCTL_ON		1
#define FRT_IOCTL_OFF		0
#define FRT_IOCTL_READ		0
#define FRT_IOCTL_WRITE	1

#define FRT_IOCTL_CLK_IN_0		0x0
#define FRT_IOCTL_CLK_IN_1		0x1
#define FRT_IOCTL_CLK_IN_2		0x2
#define FRT_IOCTL_CLK_EXT_0	0x3

struct moxaarm11_frt_ioctl_struct{
	int 	ab;
	int	on_off;
	int	rw;
	unsigned int	val;
};


/*------------------------------------------------------ Extern / Function Declaration -----------------*/
int frt_test_in_timer_match_a_intr(unsigned int gpio_out_pin);
void diag_do_frt_in_timer_match_a_intr(void);
int frt_test_in_timer_match_b_intr(unsigned int gpio_out_pin);
void diag_do_frt_in_timer_match_b_intr(void);
int frt_test_in_timer_match_ab_intr(unsigned int gpio_out_pin);
void diag_do_frt_in_timer_match_ab_intr(void);
int frt_test_in_frc_overflow_intr(unsigned int gpio_out_pin);
void diag_do_frt_in_frc_overflow_intr(void);
int frt_test_in_timer_stop(unsigned int gpio_out_pin);
void diag_do_frt_in_timer_stop(void);

// out
int frt_test_out_timer_match_ab_intr(unsigned int gpio_out_pin);
void diag_do_frt_out_timer_match_ab_intr(void);


#endif
