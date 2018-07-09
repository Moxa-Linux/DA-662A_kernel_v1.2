/* 
 * linux/drivers/rtc/rtc-moxaarm11.c
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
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <linux/rtc.h>
#include <linux/clk.h>
#include <linux/time.h>
#include <linux/bitops.h>


#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/delay.h>

#include <mach/irqs.h>
#include <mach/hardware.h>

#ifndef __ASM_ARCH_REGS_RTC_H
#define __ASM_ARCH_REGS_RTC_H

struct moxaarm11_rtc_regs_t{
	u32 counter_status;
	u32 match;
	u32 reload_value;
	u32 control;
	u32 restart;
	u32 reset;
};

enum moxaarm11_rtc_time_format{
	day_shift = 17,
	hour_shift = 12,
	min_shift = 6,
	sec_shift = 0,
	day_mask = 0x7FFF<<day_shift,
	hour_mask = 0x1F<<hour_shift,
	min_mask = 0x3F<<min_shift,
	sec_mask = 0x3F<<sec_shift,

};

enum moxaarm11_rtc_control{
	restart_status = BIT(5),
	rtc_enable = BIT(0),
};

enum moxaarm11_rtc_restart{
	restart_status_cmd = 0x5A,
};

enum moxaarm11_rtc_reset{
	reset_rtc_cmd = 0x99,
};

struct moxaarm11_rtc_private_data_t{
	u32 irq;
	u32 physical_base_addr;
	u32 virtual_base_addr;
	u32 regs_len;
	u32 tick_cnt;
};

static struct moxaarm11_rtc_regs_t *rtc_regs;
static struct moxaarm11_rtc_private_data_t rtc_priv;

#endif
static irqreturn_t moxaarm11_rtc_alarm_irq(int irq, void *id)
{

	struct rtc_device *rdev = id;
	
	printk("In alarm ISR\n");
	moxaarm11_vic_clear_intr(irq);
	rtc_update_irq(rdev, 1, RTC_AF|RTC_IRQF);
	return IRQ_HANDLED;
}


static int moxaarm11_rtc_open(struct device *dev)
{
	struct platform_device 	*pdev = to_platform_device(dev);
	struct rtc_device	*rtc_dev = platform_get_drvdata(pdev);
	unsigned int ctrl;
	int ret;
		
	/*enable RTC alarm*/
	ret = request_irq(rtc_priv.irq, moxaarm11_rtc_alarm_irq, 
				IRQF_DISABLED, "moxaarm11-rtc alarm", rtc_dev);
	if (ret) {
		dev_err(dev, "A IRQ%d error %d\n", rtc_priv.irq, ret);
		return ret;
	}	

	ctrl = readl(&rtc_regs->control);
	writel(ctrl|rtc_enable, &rtc_regs->control);

	return ret;
}

static inline void moxaarm11_rtc_enable(struct platform_device *pdev)
{
	writel(rtc_enable, &rtc_regs->control);
}

static inline void moxaarm11_rtc_disable(struct platform_device *pdev)
{
	writel(0x0, &rtc_regs->control);
}

static void moxaarm11_rtc_release(struct device *dev)
{
	struct platform_device 	*pdev = to_platform_device(dev);
	struct rtc_device 	*rtc_dev = platform_get_drvdata(pdev);
	
	free_irq(rtc_priv.irq, rtc_dev);
	moxaarm11_vic_clear_intr(rtc_priv.irq);
}


static int moxaarm11_rtc_gettime(struct device *dev, struct rtc_time *rtc_tm)
{
	u32 rtc_day;
	unsigned long time;

	memset(rtc_tm, 0, sizeof(struct rtc_time));
	rtc_tm->tm_sec = (readl(&rtc_regs->counter_status) & sec_mask) >> sec_shift;	
	rtc_tm->tm_min = (readl(&rtc_regs->counter_status) & min_mask) >> min_shift;
	rtc_tm->tm_hour = (readl(&rtc_regs->counter_status) & hour_mask) >> hour_shift;
	
	rtc_day= (readl(&rtc_regs->counter_status) & day_mask) >> day_shift;
	//	printk("tm %d %d %d %d %d %d\n", rtc_tm->tm_year, rtc_tm->tm_mon, rtc_tm->tm_mday, rtc_tm->tm_hour, rtc_tm->tm_min, rtc_tm->tm_sec);
	//printk("rtc_day: %d\n", rtc_day);
	time = (((((rtc_day * 24) + rtc_tm->tm_hour) * 60) + rtc_tm->tm_min) * 60) + rtc_tm->tm_sec;	
	//printk("time: %ld\n", time);
	rtc_time_to_tm(time, rtc_tm);
	return 0;
}

#define SEC_PER_DAY 86400
#define SETTIME_DELAY 3
static int moxaarm11_rtc_settime(struct device *dev, struct rtc_time *rtc_tm)
{
	unsigned int reload_time, rtc_day;
	unsigned long time;

	/*Key Point: The hardware setting operation needs approximately 3 seconds.*/
	rtc_tm->tm_sec += SETTIME_DELAY;
	
	rtc_tm_to_time(rtc_tm, &time);
	rtc_day = time/SEC_PER_DAY;
	reload_time = (rtc_day<<day_shift)|(rtc_tm->tm_hour<<hour_shift)|(rtc_tm->tm_min<<min_shift)|(rtc_tm->tm_sec<<sec_shift);
	//printk("%s: %d reload_time: 0x%8x %d %d %d %d", __FUNCTION__, __LINE__,reload_time, rtc_day,rtc_tm->tm_hour, rtc_tm->tm_min, rtc_tm->tm_sec);		
	writel(reload_time, &rtc_regs->reload_value);
	writel(restart_status_cmd, &rtc_regs->restart);

	while (readl(&rtc_regs->control)&restart_status) 
		udelay(10);
	
	return 0;
}

static int moxaarm11_rtc_getalarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	struct rtc_time *alm_tm = &alrm->time;

	alm_tm->tm_sec = (readl(&rtc_regs->match) & sec_mask) >> sec_shift;
	alm_tm->tm_min = (readl(&rtc_regs->match) & min_mask) >> min_shift;
	alm_tm->tm_hour= (readl(&rtc_regs->match) & hour_mask) >> hour_shift;
	alm_tm->tm_mday= (readl(&rtc_regs->match) & day_mask) >> day_shift;

	return 0;
}

static int moxaarm11_rtc_setalarm(struct device *dev, struct rtc_wkalrm *alrm)
{	
	struct rtc_time *tm = &alrm->time;
	unsigned int match_time;

	match_time = (tm->tm_mday<<day_shift)|(tm->tm_hour<<hour_shift)|(tm->tm_min<<min_shift)|(tm->tm_sec<<sec_shift);
	writel(match_time, &rtc_regs->match);

	if (alrm->enabled) {
		enable_irq_wake(rtc_priv.irq);
	} else {
		disable_irq_wake(rtc_priv.irq);
	}

	return 0;
}

static int moxaarm11_rtc_alarm_en(struct device *dev, unsigned int enable)
{
	if (enable) {
		/*Nothing to do. Once if the hour, min, and sec settings in the match are reseasonable,
		the alarm interrupt is enable.*/
	} else {
		writel(0xFFFF, &rtc_regs->match);
	}
	return 0;
}

static const struct rtc_class_ops moxaarm11_rtc_ops = {
	.open			= moxaarm11_rtc_open,
	.release		= moxaarm11_rtc_release,
	.read_time		= moxaarm11_rtc_gettime,
	.set_time		= moxaarm11_rtc_settime,
	.read_alarm		= moxaarm11_rtc_getalarm,
	.set_alarm		= moxaarm11_rtc_setalarm,
	.alarm_irq_enable 	= moxaarm11_rtc_alarm_en,
};

static int __devinit moxaarm11_rtc_probe(struct platform_device *pdev)
{
	struct rtc_device *rdev;
	int 	ret=0;
	struct resource *res;
	
	printk("%s:\n", __FUNCTION__);

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res) {
		dev_err(&pdev->dev,
			"Found HC with no IRQ. Check %s setup!\n",
			dev_name(&pdev->dev));
		return -ENODEV;
	}
	rtc_priv.irq= res->start;
	printk("rtc_priv.irq=%d\n", rtc_priv.irq);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res == NULL) {
		dev_err(&pdev->dev, "no memory resource specified\n");
		return -ENOENT;
	}

	rtc_priv.physical_base_addr= res->start;
	rtc_priv.regs_len= (res->end - res->start) + 1;
	if (!request_mem_region(rtc_priv.physical_base_addr, rtc_priv.regs_len, pdev->name)) {
		dev_dbg(&pdev->dev, "failed to get memory region\n");
		ret = -EBUSY;
		goto err_req;
	}

	rtc_priv.virtual_base_addr = (u32)ioremap_nocache(rtc_priv.physical_base_addr, rtc_priv.regs_len);
	if (rtc_priv.virtual_base_addr == 0) {
		dev_dbg(&pdev->dev, "error mapping memory\n");
		ret = -EFAULT;
		goto err_req;
	}else
		rtc_regs = (struct moxaarm11_rtc_regs_t *)rtc_priv.virtual_base_addr;
	
	printk("rtc_priv.virtual_base_addr=0x%p\n", rtc_regs); 
	
	/*enable RTC*/
	moxaarm11_rtc_enable(pdev);

	device_init_wakeup(&pdev->dev, 1);
	/*register device*/
	rdev= rtc_device_register("MoxaARM11RTC", &pdev->dev, &moxaarm11_rtc_ops, THIS_MODULE);
	if (IS_ERR(rdev)) {
		dev_err(&pdev->dev, "rtc_device_register fail\n");
		ret = PTR_ERR(rdev);
		moxaarm11_rtc_disable(pdev);
		goto err_nortc;
	}
	platform_set_drvdata(pdev, rdev);

err_req:
	return ret;
err_nortc:
	return ret;
}

static int __devexit moxaarm11_rtc_remove(struct platform_device *pdev)
{
	rtc_device_unregister(&pdev->dev);
	iounmap((void *)rtc_priv.virtual_base_addr);
	release_mem_region(rtc_priv.physical_base_addr, rtc_priv.regs_len);
	
	return 0;
}

static struct platform_driver moxaarm11_rtc_driver = {
	.probe		= moxaarm11_rtc_probe,
	.remove		= __devexit_p(moxaarm11_rtc_remove),
	.driver		= {
		.name	= "MoxaARM11RTC",
		.owner	= THIS_MODULE,
	}, 
};

static int __init moxaarm11_rtc_init(void)
{
	return platform_driver_register(&moxaarm11_rtc_driver);
}

static void __exit moxaarm11_rtc_exit(void)
{
	platform_driver_unregister(&moxaarm11_rtc_driver);
}

module_init(moxaarm11_rtc_init);
module_exit(moxaarm11_rtc_exit);

MODULE_DESCRIPTION("MOXAARM11 RTC Driver");
MODULE_AUTHOR("Vincent Wu");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform: MoxaARM11RTC");
