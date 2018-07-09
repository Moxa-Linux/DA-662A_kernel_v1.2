/* 
 * linux/arch/arm/mach-moxaarm11/moxaarm11-irq.c
 *
 * Copyright 2010 Moxa Tech., Inc.
 *	Vincent Wu.<vincent.wu@moxa.com>
 *	http://www.moxa.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/serial_core.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/module.h>

#include <mach/map.h>
#include <mach/irqs.h>
#include <mach/timer.h>
#include <mach/system-control.h>

// define the VIC register structure
struct moxaarm11_vic_struct {
	u32 	irq_status;	// 0x00, IRQ status
	u32	fiq_status;	// 0x04, FIQ status
	u32 	raw_int_status;	// 0x08, raw interrupt status
	u32 	int_select;	// 0x0c, interrupt select
	u32 	int_enable;	// 0x10, interrupt enable
	u32 	int_en_clear;	// 0x14, interrupt enable clear
	u32 	soft_int;	// 0x18, software interrupt
	u32 	soft_int_clear;	// 0x1c, software interrupt clear
	u32	protect_enable;	// 0x20, protection enable
	u32	int_sense;	// 0x24, interrupt sense
	u32	int_both_edge;	// 0x28, interrupt both edges
	u32	int_event;	// 0x2c, interrupt event
	u32	vec_addr;	// 0x30, vector address
	u32	def_vec_addr;	// 0x34, default vector address
	u32	edge_trig_int;	// 0x38, edge-triggered interrupt
	u32	reversed1[49];
	union vec_addr_table_union {
		u32	vec_addr_array[16];
		struct vec_addr_each_struct {
			u32	addr0;// 0x100, vector address register 0
			u32	addr1;// 0x104, vector address register 1
			u32	addr2;// 0x108, vector address register 2
			u32	addr3;// 0x10c, vector address register 3
			u32	addr4;// 0x110, vector address register 4
			u32	addr5;// 0x114, vector address register 5
			u32	addr6;// 0x118, vector address register 6
			u32	addr7;// 0x11c, vector address register 7
			u32	addr8;// 0x120, vector address register 8
			u32	addr9;// 0x124, vector address register 9
			u32	addr10;// 0x128, vector address register 10
			u32	addr11;// 0x12c, vector address register 11
			u32	addr12;// 0x130, vector address register 12
			u32	addr13;// 0x134, vector address register 13
			u32	addr14;// 0x138, vector address register 14
			u32	addr15;// 0x13c, vector address register 15
		} vec_addr_each;
	} vec_addr_table;
	u32	reversed2[48];
	union vec_ctrl_table_union {
		u32	vec_ctrl_array[16];
		struct vec_ctrl_each_struct {
			u32	ctrl0;// 0x200, vector control register 0
#define VIC_VEC_ENABLE	(1<<5)	// enables vector interrupt
#define VIC_SRC_MASK	0x1f	// select interrupt source, for 32 sources
			u32	ctrl1;// 0x204, vector control register 1
			u32	ctrl2;// 0x208, vector control register 2
			u32	ctrl3;// 0x20c, vector control register 3
			u32	ctrl4;// 0x210, vector control register 4
			u32	ctrl5;// 0x214, vector control register 5
			u32	ctrl6;// 0x218, vector control register 6
			u32	ctrl7;// 0x21c, vector control register 7
			u32	ctrl8;// 0x220, vector control register 8
			u32	ctrl9;// 0x224, vector control register 9
			u32	ctrl10;// 0x228, vector control register 10
			u32	ctrl11;// 0x22c, vector control register 11
			u32	ctrl12;// 0x230, vector control register 12
			u32	ctrl13;// 0x234, vector control register 13
			u32	ctrl14;// 0x238, vector control register 14
			u32	ctrl15;// 0x23c, vector control register 15
		} vec_ctrl_each;
	} vec_ctrl_table;
};

#define MOXAARM11_READ_REG(r)		(*((volatile unsigned int *) (r)))
#define MOXAARM11_WRITE_REG(r,v)	(*((volatile unsigned int *) (r)) = ((unsigned int)   (v)))

static void moxaarm11_vic_reset(void)
{
	MOXAARM11_WRITE_REG(MOXAARM11_VA_VIC_BASE+ MOXAARM11_VIC_INT_SEL_OFFSET,      0x00000000);
	MOXAARM11_WRITE_REG(MOXAARM11_VA_VIC_BASE + MOXAARM11_VIC_ENABLE_SET_OFFSET,   0x00000000);
	MOXAARM11_WRITE_REG(MOXAARM11_VA_VIC_BASE + MOXAARM11_VIC_ENABLE_CLEAR_OFFSET, 0xFFFFFFFF);
	MOXAARM11_WRITE_REG(MOXAARM11_VA_VIC_BASE + MOXAARM11_VIC_EDGE_CLEAR_OFFSET,   0xFFFFFFFF);
}

void moxaarm11_vic_set_intr_trigger(int irqnr, vic_intr_trig_mode_E mode)
{
	unsigned int nMask=0;
	unsigned int nRegValue;
	unsigned int nVICVA;

	nMask = (1 << irqnr);
	nVICVA = MOXAARM11_VA_VIC_BASE;           /* switch to VIC1 */

	/* edge or level trigger */
	nRegValue =MOXAARM11_READ_REG(nVICVA + MOXAARM11_VIC_SENSE_OFFSET);
	if (mode == vicc_edge_activeFalling || mode == vicc_edge_activeRaising 
		|| mode == vicc_edge_activeBoth) {
	 	/* edge trigger : corresponding bit shall be reset as 0 */
		nRegValue &= (~nMask);
	} else {
		/* level trigger : corresponding bit shall be set as 1 */
		nRegValue |= nMask;
	}
	MOXAARM11_WRITE_REG(nVICVA + MOXAARM11_VIC_SENSE_OFFSET, nRegValue);

	/* active high or active low */
	nRegValue = MOXAARM11_READ_REG(nVICVA + MOXAARM11_VIC_EVENT_OFFSET);
	if (mode == vicc_edge_activeFalling || mode == vicc_level_activeLow) {
		/* active low : corresponding bit shall be reset as 0 */
		nRegValue &= (~nMask);
	} else if (mode == vicc_edge_activeRaising || mode == vicc_level_activeHigh) {
		/* active high : corresponding bit shall be set as 1 */
		nRegValue |= nMask;
	}
	MOXAARM11_WRITE_REG(nVICVA + MOXAARM11_VIC_EVENT_OFFSET, nRegValue);

	/* both edge? */
	nRegValue = MOXAARM11_READ_REG(nVICVA + MOXAARM11_VIC_BOTH_EDGE_OFFSET);
	if (mode == vicc_edge_activeBoth) {
		/* both edge : corresponding bit shall be set as 1 */
		nRegValue |= nMask;
	} else {
		/* single edge : corresponding bit shall be reset as 0 */
		nRegValue &= (~nMask);
	}
	MOXAARM11_WRITE_REG(nVICVA + MOXAARM11_VIC_BOTH_EDGE_OFFSET, nRegValue);
}
EXPORT_SYMBOL(moxaarm11_vic_set_intr_trigger);

#ifdef CONFIG_MOXA_FIQ
void moxaarm11_vic_set_fiq(int irqnr, int mode)
{
	unsigned int nMask=0;
	unsigned int nRegValue;
	unsigned int nVICVA;

	nMask = (1 << irqnr);
	nVICVA = MOXAARM11_VA_VIC_BASE;           

	nRegValue =MOXAARM11_READ_REG(nVICVA + MOXAARM11_VIC_FIQ_STATUS_OFFSET);
	if (mode == vic_mode_fiq) 
		nRegValue |= nMask;
	else 
		nRegValue &= (~nMask);
	MOXAARM11_WRITE_REG(nVICVA + MOXAARM11_VIC_FIQ_STATUS_OFFSET, nRegValue);

}
EXPORT_SYMBOL(moxaarm11_vic_set_fiq);
#endif
	
void moxaarm11_vic_enable_intr(int irq)
{
        register unsigned int regVal;

	if(irq >= MOXAARM11_IRQ_INTAJ && irq <= MOXAARM11_IRQ_INTDJ){
		moxaarm11_io_pin_select(irq-MOXAARM11_IRQ_INTAJ, 1);
	}

	regVal = MOXAARM11_READ_REG(MOXAARM11_VA_VIC_BASE + MOXAARM11_VIC_ENABLE_SET_OFFSET);
	regVal |= (1 << irq);
	MOXAARM11_WRITE_REG(MOXAARM11_VA_VIC_BASE + MOXAARM11_VIC_ENABLE_SET_OFFSET, regVal);
}
EXPORT_SYMBOL(moxaarm11_vic_enable_intr);

void moxaarm11_vic_disable_intr(int irq)
{
        register unsigned int regVal;

	if(irq >= MOXAARM11_IRQ_INTAJ && irq <= MOXAARM11_IRQ_INTDJ){
		moxaarm11_io_pin_select(irq-MOXAARM11_IRQ_INTAJ, 0);
	}		

	regVal = MOXAARM11_READ_REG(MOXAARM11_VA_VIC_BASE + MOXAARM11_VIC_ENABLE_CLEAR_OFFSET);
	regVal |= (1 << irq);
	MOXAARM11_WRITE_REG(MOXAARM11_VA_VIC_BASE + MOXAARM11_VIC_ENABLE_CLEAR_OFFSET, regVal);
}
EXPORT_SYMBOL(moxaarm11_vic_disable_intr);

void moxaarm11_vic_clear_intr(int irq)
{
	MOXAARM11_WRITE_REG(MOXAARM11_VA_VIC_BASE + MOXAARM11_VIC_EDGE_CLEAR_OFFSET, (1 << irq));
}
EXPORT_SYMBOL(moxaarm11_vic_clear_intr);


//\\ add by chinfu
unsigned int moxaarm11_vic_get_sts(void)
{
        return MOXAARM11_READ_REG(MOXAARM11_VA_VIC_BASE + MOXAARM11_VIC_STATUS_OFFSET);
}
EXPORT_SYMBOL(moxaarm11_vic_get_sts);

static int moxaarm11_irq_set_type(unsigned int irq, unsigned int flow_type)
{
        vic_intr_trig_mode_E mode;

        switch (flow_type){
        case IRQ_TYPE_EDGE_FALLING:
                mode = vicc_edge_activeFalling;
                break;
        case IRQ_TYPE_EDGE_RISING:
                mode = vicc_edge_activeRaising;
                break;
        case IRQ_TYPE_LEVEL_HIGH:
                mode = vicc_level_activeHigh;
                break;
        case IRQ_TYPE_LEVEL_LOW:
                mode = vicc_level_activeLow;
                break;
        case IRQ_TYPE_EDGE_BOTH:
                mode = vicc_edge_activeBoth;
                break;
	default:
                return -EINVAL;
        }

        moxaarm11_vic_set_intr_trigger(irq, mode);
        return 0;
}

static void moxaarm11_sc_mask_irq(unsigned int irq)
{
	moxaarm11_vic_disable_intr(irq);
}

static void moxaarm11_sc_unmask_irq(unsigned int irq)
{
        moxaarm11_vic_enable_intr(irq);
}

static struct irq_chip sc_chip = {
        .ack    = moxaarm11_sc_mask_irq,
        .mask   = moxaarm11_sc_mask_irq,
        .unmask = moxaarm11_sc_unmask_irq,
	.set_type = moxaarm11_irq_set_type,
};

void __init moxaarm11_init_irq(void)
{
        unsigned int i;

	printk(KERN_DEBUG "%s: initialising interrupts\n", __func__);
        moxaarm11_vic_reset();
        for (i = 0; i < MOXAARM11_VIC_IRQ_MAX; i++) {
		set_irq_chip(i, &sc_chip);
		set_irq_handler(i, handle_level_irq);
		set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
        }
}
