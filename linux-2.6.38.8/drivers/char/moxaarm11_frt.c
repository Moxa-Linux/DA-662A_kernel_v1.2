/*
 * This is Moxa CPU DIO device driver.
 * It is from misc interface. So the device node major number is 10.
 * The device node minor number is following:
 * dio:		104
 *
 * History:
 * Date		Aurhor			Comment
 * 11-10-2010	ChinFu Yang.		Create it.
 */
 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/rtc.h>
#include <linux/timer.h>
#include <linux/ioport.h>
#include <linux/proc_fs.h>
#include <mach/irqs.h>
#include <linux/irq.h>
#include <mach/map.h>
#include <asm/io.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <mach/system.h>
#include <mach/system-control.h>

#include "moxaarm11_frt.h"





static struct moxaarm11_frt_struct moxaarm11_frt;

static struct cdev *frt_cdev;

static int dev_no;

/**	\brief	
 *
 *	Control the FRT compare interrupt A
 *	(for timer) 
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	on_off : enable(1) / disable(0)
 *
 *	\retval		int : error code
*/
int frt_output_compare_interrupt_a(unsigned int frt_port , int on_off)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable);
	if(on_off == OFF){
		reg &= ~(SET_BIT(3));
	}else{
		reg |= (SET_BIT(3));
	}
	writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable);
#ifdef MOXAARM11_FRT_DBG
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable);
	dbg("timer_interrupt_enable[0x%.8x] = 0x%.8x\r\n" ,(u32)&moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable ,reg);
#endif
	return 0;
}



/**	\brief	
 *
 *	Control the FRT compare interrupt B
 *	(for timer) 
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	on_off : enable(1) / disable(0)
 *
 *	\retval		int : error code
*/
int frt_output_compare_interrupt_b(unsigned int frt_port , int on_off)
{

	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable);
	if(on_off == OFF){
		reg &= ~(SET_BIT(2));
	}else{
		reg |= (SET_BIT(2));
	}
	writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable);
#ifdef MOXAARM11_FRT_DBG
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable);
	dbg("timer_interrupt_enable[0x%.8x] = 0x%.8x\r\n" , (u32)&moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable , reg);
#endif	
	return 0;
}



/**	\brief	
 *
 *	Control the FRT compare overflow interrupt 
 *	(for FRC) 
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	on_off : enable(1) / disable(0)
 *
 *	\retval		int : error code
*/
int frt_overflow_interrupt(unsigned int frt_port , int on_off)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable);
	if(on_off == OFF){
		reg &= ~(SET_BIT(1));
	}else{
		reg |= (SET_BIT(1));
	}
	writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable);
#ifdef MOXAARM11_FRT_DBG
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable);
	dbg("timer_interrupt_enable[0x%.8x] = 0x%.8x\r\n" , (u32)&moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable , reg);
#endif		
	return 0;
}



/**	\brief	
 *
 *	Control the FRT Output Compare Flag A
 *	(for timer) 
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	rw : READ(0) / WRITE(1)
 *
 *	\retval		int : error code
*/
int frt_output_compare_flag_a(unsigned int frt_port , int rw)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->free_ctrl_sts);
	printk("free_ctrl_sts[0x%.8x] = 0x%.8x\r\n" , (u32)&moxaarm11_frt.frt_regs[frt_port]->free_ctrl_sts , reg);	
	if(rw == READ){
		return ((reg >> 3) & 0x1);
	}else{
		reg &= ~(SET_BIT(3));
		writel(reg , &moxaarm11_frt.frt_regs[frt_port]->free_ctrl_sts);
#ifdef MOXAARM11_FRT_DBG
		reg = readl(&moxaarm11_frt.frt_regs[frt_port]->free_ctrl_sts);
		dbg("free_ctrl_sts[0x%.8x] = 0x%.8x\r\n" , (u32)&moxaarm11_frt.frt_regs[frt_port]->free_ctrl_sts , reg);
#endif			
	}
	return 0;
}




/**	\brief	
 *
 *	Control the FRT Output Compare Flag B
 *	(for timer) 
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	rw : READ(0) / WRITE(1)
 *
 *	\retval		int : error code
*/
int frt_output_compare_flag_b(unsigned int frt_port , int rw)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->free_ctrl_sts);
	if(rw == READ){
		return ((reg >> 2) & 0x1);
	}else{
		reg &= ~(SET_BIT(2));
		writel(reg , &moxaarm11_frt.frt_regs[frt_port]->free_ctrl_sts);
#ifdef MOXAARM11_FRT_DBG
		reg = readl(&moxaarm11_frt.frt_regs[frt_port]->free_ctrl_sts);
		dbg("free_ctrl_sts[0x%.8x] = 0x%.8x\r\n" ,  (u32)&moxaarm11_frt.frt_regs[frt_port]->free_ctrl_sts , reg);
#endif			
	}
	return 0;
}





/**	\brief	
 *
 *	Control the FRT Overflow Flag 
 *	(for FRC)
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	rw : READ(0) / WRITE(1)
 *
 *	\retval		int : error code
*/
int frt_frc_overflow_flag(unsigned int frt_port , int rw)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->free_ctrl_sts);
	if(rw == READ){
		return ((reg >> 1) & 0x1);
	}else{
		reg &= ~(SET_BIT(1));
		writel(reg , &moxaarm11_frt.frt_regs[frt_port]->free_ctrl_sts);
#ifdef MOXAARM11_FRT_DBG
		reg = readl(&moxaarm11_frt.frt_regs[frt_port]->free_ctrl_sts);
		dbg("free_ctrl_sts[0x%.8x] = 0x%.8x\r\n" ,  (u32)&moxaarm11_frt.frt_regs[frt_port]->free_ctrl_sts , reg);
#endif			
	}
	return 0;
}




/**	\brief	
 *
 *	Get / Set the Free Running Counter.
 *	(for FRC) 
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	rw : READ(0) / WRITE(1)
 *	\param[in]	val : value to be written
 *
 *	\retval		int : error code
*/
int frt_frc_counter_rw(unsigned int frt_port , int rw , unsigned int val)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	val &= 0xffff;	// 16bits
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->free_counter);
	if(rw == READ){
		return reg;
	}else{
		reg = val;
		writel(reg , &moxaarm11_frt.frt_regs[frt_port]->free_counter);
#ifdef MOXAARM11_FRT_DBG
		reg = readl(&moxaarm11_frt.frt_regs[frt_port]->free_counter);
		dbg("free_ctrl_sts[0x%.8x] = 0x%.8x\r\n" ,  (u32)&moxaarm11_frt.frt_regs[frt_port]->free_counter , reg);
#endif			
	}
	return 0;
}




/**	\brief	
 *
 *	Get/Set the value of output compare register A , B
 *	(for timer) 
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	rw : READ(0) / WRITE(1)
 *	\param[in]	val : value to be written
 *
 *	\retval		int : error code
*/
int frt_output_compare_ab_rw(unsigned int frt_port , int rw , unsigned int val)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	val &= 0xffff;	// 16bits
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->free_output_cmp_a_b);
	if(rw == READ){
		return reg;
	}else{
		reg = val;
		writel(reg , &moxaarm11_frt.frt_regs[frt_port]->free_output_cmp_a_b);
#ifdef MOXAARM11_FRT_DBG
		reg = readl(&moxaarm11_frt.frt_regs[frt_port]->free_output_cmp_a_b);
		dbg("free_output_cmp_a_b[0x%.8x] = 0x%.8x\r\n" ,  (u32)&moxaarm11_frt.frt_regs[frt_port]->free_output_cmp_a_b , reg);
#endif			
	}
	return 0;
}



/**	\brief	
 *
 *	Select the clock source
 *	(for FRC)
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	rw : READ(0) / WRITE(1)
 *	\param[in]	val : value to be written
 *						00: Internal clock ps0 is counted
 *						01: Internal clock ps1 is counted
 *						10: Internal clock ps2 is counted
 *						11: Rising edge of external clock input pin (FTCI) is counted
 *
 *	\retval		int : error code
*/
int frt_select_frc_clock_source_rw(unsigned int frt_port , int rw , unsigned int val)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	val &= 0x3;		// 2bits
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl);
	if(rw == READ){
		return reg;
	}else{
		reg = val;
		writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_ctrl);
#ifdef MOXAARM11_FRT_DBG
		reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl);
		dbg("timer_ctrl[0x%.8x] = 0x%.8x\r\n" , (u32)&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl , reg);
#endif			
	}
	return 0;
}



/**	\brief	
 *
 *	Select the output compare matching registers.
 *	(for timer) 
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	rw : READ(0) / WRITE(1)
 *	\param[in]	val : value to be written , 
					0: Compare-match A and B control FTOA output.
					1: Only compare-match A controls FTOA output.
 *
 *
 *	\retval		int : error code
*/
int frt_output_compare_select_rw(unsigned int frt_port , int rw , unsigned int val)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	val &= 0x1;		// 1bit
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_output_cmp_ctrl);
	if(rw == READ){
		reg = (reg >> 6) & 0x1;
		return reg;
	}else{
		if(val == 0){
			reg &= ~(SET_BIT(6));
		}else{
			reg |= SET_BIT(6);
		}
		writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_output_cmp_ctrl);
#ifdef MOXAARM11_FRT_DBG
		reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_output_cmp_ctrl);
		dbg("timer_output_cmp_ctrl[0x%.8x] = 0x%.8x\r\n" , (u32)&moxaarm11_frt.frt_regs[frt_port]->timer_output_cmp_ctrl ,reg);
#endif				
	}
	return 0;
}





/**	\brief	
 *
 *	Select the FRC(0x08), OCRA(0x0C), and TCNT(0x2C)
 *
 *	\param[in]	frt_port : which frt port
 *
 *
 *	\retval		int : error code
*/
int frt_force_select_frc_ocra_tcnt(unsigned int frt_port)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}

	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_output_cmp_ctrl);
	reg |= SET_BIT(5);
	reg &= ~(SET_BIT(4));
	writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_output_cmp_ctrl);
#ifdef MOXAARM11_FRT_DBG
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_output_cmp_ctrl);
	dbg("timer_output_cmp_ctrl[0x%.8x] = 0x%.8x\r\n" , (u32)&moxaarm11_frt.frt_regs[frt_port]->timer_output_cmp_ctrl ,reg);
#endif			
	return 0;
}



/**	\brief	
 *
 *	Select the OCRB(0x0C) only.
 *
 *	\param[in]	frt_port : which frt port
 *
 *
 *	\retval		int : error code
*/
int frt_force_select_ocrb(unsigned int frt_port)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}

	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_output_cmp_ctrl);
	reg |= SET_BIT(5);
	reg |= SET_BIT(4);
	writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_output_cmp_ctrl);
#ifdef MOXAARM11_FRT_DBG
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_output_cmp_ctrl);
	dbg("timer_output_cmp_ctrl[0x%.8x] = 0x%.8x\r\n" ,(u32)&moxaarm11_frt.frt_regs[frt_port]->timer_output_cmp_ctrl , reg);
#endif			
	return 0;
}




/**	\brief	
 *
 *	Enable/Disable the signal of output compare A to output pin.
 *	(for timer) 
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	on_off : OFF:Disable , ON:Enable
 *
 *
 *	\retval		int : error code
*/
int frt_output_compare_pin_a(unsigned int frt_port , int on_off)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_output_cmp_ctrl);
	if(on_off == OFF){
		reg &= ~(SET_BIT(3));
	}else{
		reg |= (SET_BIT(3));
	}
	writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable);
#ifdef MOXAARM11_FRT_DBG
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable);
	dbg("timer_interrupt_enable[0x%.8x] = 0x%.8x\r\n" , (u32)&moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable ,reg);
#endif			
	return 0;
}





/**	\brief	
 *
 *	Setting level output to FRT output pin when matching the output compare register A.
 *	(for timer) 
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	on_off : OFF:0 (Low level) , ON:1 (High Level)
 *
 *
 *	\retval		int : error code
*/
int frt_output_compare_level_a(unsigned int frt_port , int on_off)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_output_cmp_ctrl);
	if(on_off == OFF){
		reg &= ~(SET_BIT(1));
	}else{
		reg |= (SET_BIT(1));
	}
	writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable);
#ifdef MOXAARM11_FRT_DBG
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable);
	dbg("timer_interrupt_enable[0x%.8x] = 0x%.8x\r\n" , (u32)&moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable ,reg);
#endif		
	return 0;
}





/**	\brief	
 *
 *	Setting level output to FRT output pin when matching the output compare register B.
 *	(for timer)
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	on_off : OFF:0 (Low level) , ON:1 (High Level)
 *
 *
 *	\retval		int : error code
*/
int frt_output_compare_level_b(unsigned int frt_port , int on_off)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_output_cmp_ctrl);
	if(on_off == OFF){
		reg &= ~(SET_BIT(0));
	}else{
		reg |= (SET_BIT(0));
	}
	writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable);
#ifdef MOXAARM11_FRT_DBG
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable);
	dbg("timer_interrupt_enable[0x%.8x] = 0x%.8x\r\n" , (u32)&moxaarm11_frt.frt_regs[frt_port]->timer_interrupt_enable ,reg);
#endif		
	return 0;
}




/**	\brief	
 *
 *	Enable/Disable stop condition when output compare register matching TCNT
 *	(for timer)
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	on_off : OFF:Disable , ON:Enable 
 *
 *	\retval		int : error code
*/
int frt_output_compare_stop_b(unsigned int frt_port , int on_off)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts);
	if(on_off == OFF){
		reg &= ~(SET_BIT(5));
	}else{
		reg |= (SET_BIT(5));
	}
	writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts);
#ifdef MOXAARM11_FRT_DBG
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts);
	dbg("timer_ctrl_sts[0x%.8x] = 0x%.8x\r\n" , (u32)&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts ,reg);
#endif		
	return 0;
}





/**	\brief	
 *
 *	Get/Set stop condition status of output compare register matching TCNT
 *	(for timer) 
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	rw : READ(0) / WRITE(1)
 *	\param[in]	val : value to be written , 0: clear
 *
 *
 *	\retval		int : error code
*/
int frt_output_compare_stop_status_b_rw(unsigned int frt_port , int rw , unsigned int val)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	val &= 0x1;		// 1bit
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts);
	if(rw == READ){
		reg = (reg >> 7) & 0x1;
		return reg;
	}else{
		if(val == 0){
			reg &= ~(SET_BIT(7));
		}else{
			reg |= SET_BIT(7);
		}
		writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts);
#ifdef MOXAARM11_FRT_DBG
		reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts);
		dbg("timer_ctrl_sts[0x%.8x] = 0x%.8x\r\n" ,(u32)&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts , reg);
#endif				
	}
	return 0;
}





/**	\brief	
 *
 *	Enable/Disable Counter Clear B.
 *	(for timer)
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	on_off : OFF:Disable , ON:Enable 
						0: Clear of TCNT is prohibited.
						1: TCNT is cleared by compare-match B.
 *
 *	\retval		int : error code
*/
int frt_output_compare_counter_clear_b(unsigned int frt_port , int on_off)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts);
	if(on_off == OFF){
		reg &= ~(SET_BIT(3));
	}else{
		reg |= (SET_BIT(3));
	}
	writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts);
#ifdef MOXAARM11_FRT_DBG
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts);
	dbg("timer_ctrl_sts[0x%.8x] = 0x%.8x\r\n" , (u32)&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts ,reg);
#endif			
	return 0;
}



/**	\brief	
 *
 *	Force timer and FRC not synchronized. 
 *
 *	\param[in]	frt_port : which frt port
 *
 *	\retval		int : error code
*/
int frt_force_timer_frc_not_sync(unsigned int frt_port)
{

	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts);
	reg &= ~(SET_BIT(2));
	writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts);
#ifdef MOXAARM11_FRT_DBG
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts);
	dbg("timer_ctrl_sts[0x%.8x] = 0x%.8x\r\n" , (u32)&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts ,reg);
#endif			
	return 0;
}





/**	\brief	
 *
 *	Select the clock source. Clock input to TCNT is selected. Only valid when SYNC = 0.
 *	(for Timer)
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	rw : READ(0) / WRITE(1)
 *	\param[in]	val : value to be written
 *						00: Internal clock ps0 is counted
 *						01: Internal clock ps1 is counted
 *						10: Internal clock ps2 is counted
 *						11: Rising edge of external clock input pin (FTCI) is counted
 *
 *	\retval		int : error code
*/
int frt_select_timer_clock_source_rw(unsigned int frt_port , int rw , unsigned int val)
{
	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	val &= 0x3;		// 2bits
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts);
	if(rw == READ){
		return reg;
	}else{
		reg = val;
		writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts);
#ifdef MOXAARM11_FRT_DBG
		reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts);
		dbg("timer_ctrl_sts[0x%.8x] = 0x%.8x\r\n" , (u32)&moxaarm11_frt.frt_regs[frt_port]->timer_ctrl_sts ,reg);
#endif				
	}
	return 0;
}






/**	\brief	
 *
 *	Get/Set the timer counter.
 *
 *	\param[in]	frt_port : which frt port
 *	\param[in]	rw : READ(0) / WRITE(1)
 *	\param[in]	val : value to be written
 *
 *	\retval		int : error code
*/
int frt_timer_counter_rw(unsigned int frt_port , int rw , unsigned int val)
{

	u32 reg;
	if(frt_port >= MOXAARM11_FRT_MAX){
		return -1;
	}
	val &= 0xFFFF;		// 16bits
	reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_counter);
	if(rw == READ){
		return reg;
	}else{
		reg = val;
		writel(reg , &moxaarm11_frt.frt_regs[frt_port]->timer_counter);
#ifdef MOXAARM11_FRT_DBG
		reg = readl(&moxaarm11_frt.frt_regs[frt_port]->timer_counter);
		dbg("timer_counter[0x%.8x] = 0x%.8x\r\n" , (u32)&moxaarm11_frt.frt_regs[frt_port]->timer_counter ,reg);
#endif			
	}
	return 0;
}




/**	\brief	
 *
 *	Read the interrupt status of FRT
 *	Just a status, not for clear pending.
 *
 *	\retval		int : error code
*/
int frt_get_interrupt_status(void)
{
	return readl(moxaarm11_frt.intr_sts);
}





//------------------------------------------------------------------------------------------------------------


unsigned int frt_isr_count;
static irqreturn_t frt_isr(int irq, void *dev_id)
{
	int interrupt_status = frt_get_interrupt_status();
	int frt_port;

	printk("frt_isr_count = %d\r\n", ++frt_isr_count);

	for(frt_port = 0; frt_port < 8; frt_port++){
		if (!(interrupt_status & (0x1<<frt_port)))
			continue;
		printk("[FRT ISR]Port %d \n", frt_port);
		// clear match flag
		if(frt_output_compare_flag_a(frt_port , READ)){
			printk("[FRT ISR] Clear compare flag A\r\n");
			frt_output_compare_flag_a(frt_port, WRITE);
			//wake_up_interruptible(&moxaarm11_frt.cmp_a_wait);
		}
		if(frt_output_compare_flag_b(frt_port , READ)){
			printk("[FRT ISR] Clear compare flag B\r\n");
			frt_output_compare_flag_b(frt_port, WRITE);
			//wake_up_interruptible(&moxaarm11_frt.cmp_b_wait);
		}
		if(frt_frc_overflow_flag(frt_port , READ)){
			printk("[FRT ISR] Clear FRC overflow flag\r\n");
			frt_frc_overflow_flag(frt_port, WRITE);
			//wake_up_interruptible(&moxaarm11_frt.overflow_wait);
		}
		//writel(0x0 , &moxaarm11_frt.frt_regs[frt_port]->free_ctrl_sts);
	}
	moxaarm11_vic_clear_intr(moxaarm11_frt.irqno);
	return IRQ_HANDLED;
}




static struct irqaction moxaarm11_frt_irq = {
	.name           = "MoxaARM11FRT",
	.flags          	= IRQF_SHARED,
	.handler        = frt_isr,
};



static int moxaarm11_frt_open(struct inode *inode, struct file *file)
{
	unsigned int frt_port = 0;
	
	const int minor = iminor(inode);
	if(minor < 0 || minor >= MOXAARM11_FRT_MAX){
		return -ENODEV;
	}
	if( moxaarm11_frt.ref_count[minor] > 0 ){
		return -EBUSY;
	}else{
		
	}
	printk("[DRV] FRT open() minor = %d\r\n" , minor);
	frt_port = minor;
	
	// 1. Programm TCNT, FRC, OCRA, and OCRB Registers
//	printk("1. Programm TCNT, FRC, OCRA, and OCRB Registers\r\n");
	frt_force_timer_frc_not_sync(frt_port);	

	// 2. Disable intr , clear flag
//	printk("2. clear interrupt flag\r\n");
	frt_output_compare_interrupt_a(frt_port, OFF);
	frt_output_compare_interrupt_b(frt_port, OFF);
	frt_overflow_interrupt(frt_port, OFF);
	frt_output_compare_flag_a(frt_port, WRITE);
	frt_output_compare_flag_b(frt_port, WRITE);
	frt_frc_overflow_flag(frt_port, WRITE);

	// 3. Set External Clock Source for input counting
//	printk("3. Set External Clock Source for input counting\r\n");
	frt_select_frc_clock_source_rw(frt_port , WRITE , 0x3);
	frt_select_timer_clock_source_rw(frt_port , WRITE , 0x3);
	
//	printk("[DRV] init done...\r\n");
	moxaarm11_frt.ref_count[minor]++;
	return 0;	
}


static int moxaarm11_frt_release(struct inode *inode, struct file *file)	
{
	const int minor = iminor(inode);
	printk("moxaarm11_frt_release()\r\n");
	moxaarm11_frt.ref_count[minor]--;
	return 0;
}



static int moxaarm11_frt_unlocked_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	
	const int minor = iminor(inode);
	struct moxaarm11_frt_ioctl_struct set;
	int ret = 0;
	unsigned short max_16bit = 65535;

	unsigned short current_a_value = 0;
	unsigned short current_b_value = 0;
	unsigned short reverse = 0;


	if( copy_from_user(&set, (struct moxaarm11_frt_ioctl_struct *)arg, sizeof(struct moxaarm11_frt_ioctl_struct)) ){
		return -EFAULT;
	}
	
	switch ( cmd ) {
		case IOCTL_SLEEP:
			if(set.ab == FRT_IOCTL_A){
				frt_force_select_frc_ocra_tcnt(minor);
				frt_output_compare_ab_rw(minor, FRT_IOCTL_WRITE, set.val);
				frt_output_compare_interrupt_a(minor, FRT_IOCTL_ON);
				interruptible_sleep_on(&moxaarm11_frt.cmp_a_wait);
			}else if(set.ab == FRT_IOCTL_B){
				frt_force_select_ocrb(minor);
				frt_output_compare_ab_rw(minor, FRT_IOCTL_WRITE, set.val);
				frt_output_compare_interrupt_b(minor, FRT_IOCTL_ON);
				interruptible_sleep_on(&moxaarm11_frt.cmp_b_wait);
			}else if(set.ab == FRT_IOCTL_OVER){		
				set.val &= 0xFFFF;
				reverse = max_16bit - set.val;
				frt_force_select_frc_ocra_tcnt(minor);
				frt_frc_counter_rw(minor, FRT_IOCTL_WRITE, reverse);
				frt_overflow_interrupt(minor, FRT_IOCTL_ON);
				interruptible_sleep_on(&moxaarm11_frt.overflow_wait);
			}else{
				return -EINVAL;
			}
			
	
			break;		
		case IOCTL_OUTPUT_CMP_INTR:		// enable / disable interrupt
			if(set.ab == FRT_IOCTL_A){
				printk("FRT_IOCTL_A %d\n", set.on_off);
				frt_output_compare_interrupt_a(minor, set.on_off);
			}else if(set.ab == FRT_IOCTL_B){
				printk("FRT_IOCTL_B %d\n", set.on_off);
				frt_output_compare_interrupt_b(minor, set.on_off);
			}else{
				return -EINVAL;
			}
			break;
		case IOCTL_OVERFLOW_INTR:		// enable / disable interrupt
			frt_overflow_interrupt(minor, set.on_off);
			break;
/*			
		case IOCTL_OUTPUT_CMP_FLAG:		// reserved ~~~
			if(set.ab == FRT_IOCTL_A){
				ret = frt_output_compare_flag_a(minor, set.rw);
			}else if(set.ab == FRT_IOCTL_B){
				ret = frt_output_compare_flag_b(minor, set.rw);
			}else{
				return -EINVAL;
			}
			set.val = ret;
			if ( copy_to_user((struct moxaarm11_frt_ioctl_struct *)arg, &set, sizeof(struct moxaarm11_frt_ioctl_struct)) ){
				return -EFAULT;
			}
			break;
			
		case IOCTL_OVERFLOW_FLAG:			// reserved ~~~
			ret = frt_frc_overflow_flag(minor, set.rw);
			set.val = ret;
			if ( copy_to_user((struct moxaarm11_frt_ioctl_struct *)arg, &set, sizeof(struct moxaarm11_frt_ioctl_struct)) ){
				return -EFAULT;
			}			
			break;
*/			
		case IOCTL_FRC_COUNTER:		// set FRC counter
			ret = frt_frc_counter_rw(minor, set.rw, set.val);
			set.val = ret;
			if ( copy_to_user((struct moxaarm11_frt_ioctl_struct *)arg, &set, sizeof(struct moxaarm11_frt_ioctl_struct)) ){
				return -EFAULT;
			}			
			break;
		case IOCTL_OUTPUT_CMP:			// set compare value
			if(set.ab == FRT_IOCTL_A){
				frt_force_select_frc_ocra_tcnt(minor);
			}else if(set.ab == FRT_IOCTL_B){
				frt_force_select_ocrb(minor);
			}else{
				return -EINVAL;
			}
			ret = frt_output_compare_ab_rw(minor, set.rw, set.val);
			set.val = ret;
			if ( copy_to_user((struct moxaarm11_frt_ioctl_struct *)arg, &set, sizeof(struct moxaarm11_frt_ioctl_struct)) ){
				return -EFAULT;
			}			
			break;
		case IOCTL_SELECT_FRC_CLK_SRC:		// internal / external source
			ret = frt_select_frc_clock_source_rw(minor, set.rw, set.val);
			set.val = ret;
			if ( copy_to_user((struct moxaarm11_frt_ioctl_struct *)arg, &set, sizeof(struct moxaarm11_frt_ioctl_struct)) ){
				return -EFAULT;
			}			
			break;
		
		case IOCTL_OUTPUT_CMP_SELECT:			// reserved ~~~
			ret = frt_output_compare_select_rw(minor, set.rw, set.val);
			set.val = ret;
			if ( copy_to_user((struct moxaarm11_frt_ioctl_struct *)arg, &set, sizeof(struct moxaarm11_frt_ioctl_struct)) ){
				return -EFAULT;
			}			
			break;
/*				
		case IOCTL_FORCE_ORCA:			// reserved ~~~
			frt_force_select_frc_ocra_tcnt(minor);
			break;
		case IOCTL_FORCE_ORCB:			// reserved ~~~
			frt_force_select_ocrb(minor);
			break;
*/			
		case IOCTL_OUTPUT_CMP_PIN:		// enable/disable output when match A
			frt_output_compare_pin_a(minor, set.on_off);
			break;
		case IOCTL_OUTPUT_CMP_LEVEL:		// set output signal high/low when match A/B
			if(set.ab == FRT_IOCTL_A){
				frt_output_compare_level_a(minor, set.on_off);
			}else if(set.ab == FRT_IOCTL_B){
				frt_output_compare_level_b(minor, set.on_off);
			}else{
				return -EINVAL;
			}
			break;
		case IOCTL_OUTPUT_CMP_STOP:		// enable/disable STOP counting when match B
			frt_output_compare_stop_b(minor, set.on_off);
			break;
/*			
		case IOCTL_OUTPUT_CMP_STOP_STS:			// reserved ~~~
			ret = frt_output_compare_stop_status_b_rw(minor, set.rw, set.val);
			set.val = ret;
			if ( copy_to_user((struct moxaarm11_frt_ioctl_struct *)arg, &set, sizeof(struct moxaarm11_frt_ioctl_struct)) ){
				return -EFAULT;
			}			
			break;
*/			
		case IOCTL_OUTPUT_CMP_COUNTER_CLR:		// enable/disable Clear counter value when match B
			frt_output_compare_counter_clear_b(minor, set.on_off);
			break;
		case IOCTL_FORCE_TIMER_FRC_NO_SYNC:
			frt_force_timer_frc_not_sync(minor);		// enable/disable sync FRC and Timer
			break;
		case IOCTL_SELECT_TIMER_CLK_SRC:			// set Timer Source
			ret = frt_select_timer_clock_source_rw(minor, set.rw, set.val);
			set.val = ret;
			if ( copy_to_user((struct moxaarm11_frt_ioctl_struct *)arg, &set, sizeof(struct moxaarm11_frt_ioctl_struct)) ){
				return -EFAULT;
			}			
			break;
		case IOCTL_TIMER_COUNTER:					// set Timer value
			ret = frt_timer_counter_rw(minor, set.rw, set.val);
			set.val = ret;
			if ( copy_to_user((struct moxaarm11_frt_ioctl_struct *)arg, &set, sizeof(struct moxaarm11_frt_ioctl_struct)) ){
				return -EFAULT;
			}			
			break;			
/*			
		case IOCTL_GET_INTR_STS:			// reserved ~~~
			ret = frt_get_interrupt_status();
			set.val = ret;
			if ( copy_to_user((struct moxaarm11_frt_ioctl_struct *)arg, &set, sizeof(struct moxaarm11_frt_ioctl_struct)) ){
				return -EFAULT;
			}				
			break;	
*/			

		case IOCTL_DBG:					// for driver debug
			ret = moxaarm11_vic_get_sts();
			set.val = ret;
			if ( copy_to_user((struct moxaarm11_frt_ioctl_struct *)arg, &set, sizeof(struct moxaarm11_frt_ioctl_struct)) ){
				return -EFAULT;
			}					
		default:
			break;
	
	}

	return 0;
}



static struct file_operations moxaarm11_frt_fops = {	
	owner:THIS_MODULE,	
	llseek:NULL,	
	unlocked_ioctl:moxaarm11_frt_unlocked_ioctl,	
	open:moxaarm11_frt_open,	
	release:moxaarm11_frt_release,
};



static int __devinit moxaarm11_frt_probe(struct platform_device *pdev)
{
	struct resource	*res;

	int irq;
	int retval = 0;
	int i = 0;
	
	
	memset(&moxaarm11_frt, 0, sizeof(moxaarm11_frt));
	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res) {
		dev_err(&pdev->dev,
			"*** Found FRT with no IRQ. Check %s setup!\n",
			dev_name(&pdev->dev));
		return -ENODEV;
	}
	irq = res->start;
	moxaarm11_frt.irqno = irq;
	
	
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev,
			"*** Found FRT with no register addr. Check %s setup!\n",
			dev_name(&pdev->dev));
		return -ENODEV;
	}	
	moxaarm11_frt.physical_base = res->start;
	moxaarm11_frt.physical_len  = res->end - res->start + 1;
	

	if (!request_mem_region(moxaarm11_frt.physical_base , moxaarm11_frt.physical_len, "FRT Registers")) {
		dev_dbg(&pdev->dev, "*** controller already in use\n");
		retval = -EBUSY;
		goto fail_request_resource;
	}

	moxaarm11_frt.virtual_base  = ioremap_nocache(moxaarm11_frt.physical_base , moxaarm11_frt.physical_len);
	if ( moxaarm11_frt.virtual_base == NULL ) {
		dev_dbg(&pdev->dev, "*** error mapping memory\n");
		retval = -EFAULT;
		goto fail_ioremap;
	}	
	
	for( i = 0 ; i < MOXAARM11_FRT_MAX ; i++){
		moxaarm11_frt.frt_regs[i] = (struct moxaarm11_frt_reg_struct *)(moxaarm11_frt.virtual_base + MOXAARM11_FRT_SPCE_OFFSET * (i+1)); // (i+1) = MOXAIXP's FRT Spec.
		printk("frt%d virt address = 0x%x\n", i, moxaarm11_frt.frt_regs[i]);

	}
	moxaarm11_frt.intr_sts = (u32 *)(moxaarm11_frt.virtual_base + MOXAARM11_FRT_INTR_STS);
	
	dev_no = MKDEV(MOXAARM11_FRT_MAJOR, MOXAARM11_FRT_MINOR_START);
	
	if(register_chrdev_region(dev_no, MOXAARM11_FRT_MAX, "MOXAARM11 FRT")){
		printk(KERN_ALERT"*** Couldn't register FRT major number, major = %d\r\n", MOXAARM11_FRT_MAJOR);
		return -ENODEV;
	}

	frt_cdev = cdev_alloc();
	frt_cdev->ops = &moxaarm11_frt_fops;

	moxaarm11_io_function_select(io_function_moxauart_2_to_3_FRT);
	
	if(cdev_add(frt_cdev, dev_no, MOXAARM11_FRT_MAX)){
		printk(KERN_ALERT"*** Couldn't add FRT device to kernel, major = %d\r\n", MOXAARM11_FRT_MAJOR);
		return -ENODEV;
	}


	if(request_irq(moxaarm11_frt.irqno, &frt_isr, IRQF_DISABLED, "MoxaARM11FRT", NULL)){
		printk(KERN_ALERT"Request interrupt service fail !\n");
		retval = - EINVAL;
		goto fail_interrupt;
	}

	// setting Interrupt Service Routine
	moxaarm11_vic_set_intr_trigger(moxaarm11_frt.irqno, vicc_edge_activeRaising);	
	moxaarm11_vic_clear_intr(moxaarm11_frt.irqno);

	// init wait queue
	init_waitqueue_head(&moxaarm11_frt.cmp_a_wait);
	init_waitqueue_head(&moxaarm11_frt.cmp_b_wait);
	init_waitqueue_head(&moxaarm11_frt.overflow_wait);
	
fail_request_resource:
		
fail_ioremap:

fail_interrupt:

	return retval;
}


static int __devexit moxaarm11_frt_remove(struct platform_device *pdev)
{
	free_irq(moxaarm11_frt.irqno, NULL);

	unregister_chrdev_region(dev_no, MOXAARM11_FRT_MAX);

	cdev_del(frt_cdev);

	iounmap(moxaarm11_frt.virtual_base);	
	release_mem_region(moxaarm11_frt.physical_base, moxaarm11_frt.physical_len);	

	moxaarm11_io_function_select(io_function_pci); //default setting
	
	return 0;
}


static struct platform_driver moxaarm11_frt_driver = {
	.probe = moxaarm11_frt_probe,
	.remove = __devexit_p(moxaarm11_frt_remove),
	.driver = {
		.name = "MoxaARM11FRT",
	},
};



static int __init moxaarm11_frt_init(void)
{
	int retval = 0;
	retval = platform_driver_register(&moxaarm11_frt_driver);	
	if (retval) {
		printk(KERN_ERR "frt: can't register platform driver\n");
		return -ENODEV;	
	}else{
		printk("Register FRT Driver ver 1.4 , [2011-12-09], OK.\r\n");
	}
	return 0;
}



static void __exit moxaarm11_frt_exit(void)
{
	printk("moxaarm11_frt Removed...\r\n");
	platform_driver_unregister(&moxaarm11_frt_driver);
}


module_init(moxaarm11_frt_init);
module_exit(moxaarm11_frt_exit);
MODULE_AUTHOR("ChinFu Yang");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.4");


