/* 
 * linux/drivers/mmc/host/moxaart_sd.c
 *
 * Copyright 2005 Moxa Tech., Inc.
 *	Victor Yu.<victor.yu@moxa.com>
 *	http://www.moxa.com/
 *
 * Copyright 2011 Moxa Tech., Inc.
 *	Vincent Wu.<vincent.wu@moxa.com>
 *	http://www.moxa.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *Some vital driver programing key points were desribed as comments 
 *and marked as "Key Point"
 *
*/
#include <linux/module.h>
#include <linux/dma-mapping.h>
#include <linux/mmc/host.h>
#include <linux/platform_device.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/highmem.h>
#include <linux/mmc/sd.h>

#include <asm/delay.h>

#include <mach/dma.h>
#include	<mach/moxaart_int.h>
#include	<mach/gpio.h>

#include "moxaart_sd.h"


static irqreturn_t moxaart_sd_irq_handler(int irq, void *pdev)
{
	struct mmc_host *mmc = dev_get_drvdata(&((struct platform_device *)pdev)->dev);
	struct moxaart_sd_host *moxaart_sd=mmc_priv(mmc);
	u32            status;

	// get the interrupt status
        status = readl(&moxaart_sd->reg->status);

        // acknowledge the interurpt
	if ( status & SD_CARD_CHANGE ) {       // has card inserted or removed
		dbg_printk("SD_CARD_CHANGE\n");
		writel(SD_CLR_CARD_CHANGE, &moxaart_sd->reg->clear);
		tasklet_schedule(&moxaart_sd->card_change_tasklet);
	}

	if ( status & (SD_FIFO_ORUN|SD_FIFO_URUN) ) {
		dbg_printk("SD_FIFO_ORUN|SD_FIFO_URUN\n");
		writel(status&(SD_FIFO_ORUN|SD_FIFO_URUN), &moxaart_sd->reg->clear);
		tasklet_schedule(&moxaart_sd->fifo_run_tasklet);
	}

	return IRQ_HANDLED;
}

static void moxaart_sd_dma_irq_handler(void *param)
{
        struct moxaart_sd_host      *moxaart_sd=(struct moxaart_sd_host *)param;

        if (moxaart_sd)
                tasklet_schedule(&moxaart_sd->fifo_run_tasklet);
}

static inline void moxaart_sd_init_sg(struct moxaart_sd_host* moxaart_sd, struct mmc_data* data)
{
	/*
	* Get info. about SG list from data structure.
	*/
	moxaart_sd->cur_sg = data->sg;
	moxaart_sd->num_sg = data->sg_len;

	moxaart_sd->remain = moxaart_sd->cur_sg->length;
	if ( moxaart_sd->remain > moxaart_sd->size )
		moxaart_sd->remain = moxaart_sd->size;
	moxaart_sd->mapped_sg = NULL;
	data->error = ERR_NONE;
}


static inline int moxaart_sd_next_sg(struct moxaart_sd_host* moxaart_sd)
{
	struct mmc_data *data=moxaart_sd->data;
	int     remain;

	/*
	* Skip to next SG entry.
	*/
	moxaart_sd->cur_sg++;
	moxaart_sd->num_sg--;

	/*
	* Any entries left?
	*/
	if (moxaart_sd->num_sg > 0) {
		moxaart_sd->remain = moxaart_sd->cur_sg->length;
		remain = moxaart_sd->size - data->bytes_xfered;
		if ( remain > 0 && remain < moxaart_sd->remain ) {
			moxaart_sd->remain = remain;
		}
        }

        return moxaart_sd->num_sg;
}

static inline char *moxaart_sd_kmap_sg(struct moxaart_sd_host* moxaart_sd)
{
        moxaart_sd->mapped_sg = kmap(sg_page(moxaart_sd->cur_sg));
        return moxaart_sd->mapped_sg + moxaart_sd->cur_sg->offset;
}

static void moxaart_sd_do_fifo(struct moxaart_sd_host *moxaart_sd, struct mmc_data *data)
{
	u32    buffer;
	int     wcnt, i;
	dbg_printk("\n");

	if ( moxaart_sd->mapped_sg ) {
		kunmap(moxaart_sd->mapped_sg);
		moxaart_sd_next_sg(moxaart_sd);
	}

	if ( moxaart_sd->size == data->bytes_xfered ) {
		return;
	}

	buffer = (u32)moxaart_sd_kmap_sg(moxaart_sd);
	if ( moxaart_sd->size > SD_FIFO_LENB && moxaart_sd->dma ) {
		apb_dma_conf_param      param;
		param.size = moxaart_sd->remain;
		param.burst_mode = APB_DMAB_BURST_MODE;
		param.data_width = APB_DMAB_DATA_WIDTH_4;
		if ( data->flags & MMC_DATA_WRITE ) {
			param.source_sel = APB_DMAB_SOURCE_AHB;
			param.source_virt_addr = buffer;
			param.source_phys_addr = virt_to_phys((void *)buffer);
			param.source_inc = APB_DMAB_DEST_INC_4_16;

			param.dest_sel = APB_DMAB_DEST_APB;			
			param.dest_virt_addr = (u32)&moxaart_sd->reg->data_window;
			param.dest_phys_addr = moxaart_sd->data_window_phys;
			param.dest_inc = APB_DMAB_DEST_INC_0;			
		} else {
			param.source_sel = APB_DMAB_DEST_APB;
			param.source_virt_addr = (u32)&moxaart_sd->reg->data_window;
			param.source_phys_addr = moxaart_sd->data_window_phys;
			param.source_inc = APB_DMAB_DEST_INC_0;
		
			param.dest_sel = APB_DMAB_SOURCE_AHB;
			param.dest_virt_addr = (u32)buffer;
			param.dest_phys_addr = virt_to_phys((void *)buffer);
			param.dest_inc = APB_DMAB_DEST_INC_4_16;
		}
		data->bytes_xfered += moxaart_sd->remain;
		apb_dma_conf(moxaart_sd->dma, &param);
		apb_dma_enable(moxaart_sd->dma);
	} else {
		wcnt = moxaart_sd->remain >> 2;
		if ( data->flags & MMC_DATA_WRITE ) {
			for ( i=0; i<wcnt; i++, buffer+=4 )
				writel(*(u32 *)buffer, &moxaart_sd->reg->data_window);
		} else {
			for ( i=0; i<wcnt; i++, buffer+=4 )
				*(u32 *)buffer = readl(&moxaart_sd->reg->data_window);
		}
		wcnt <<= 2;
		moxaart_sd->remain -= wcnt;
		data->bytes_xfered += wcnt;
	}
}

static void moxaart_sd_prepare_data(struct moxaart_sd_host *moxa_sd, struct mmc_data *data)
{
	u32    timeout, datactrl;
	int             blksz_bits;

	dbg_printk("\n");
	moxa_sd->data = data;
	// initialize the data size
	moxa_sd->size = data->blocks * data->blksz;
	blksz_bits = ffs(data->blksz) - 1;
	BUG_ON(1 << blksz_bits != data->blksz);
	moxaart_sd_init_sg(moxa_sd, data);

	// initialize the timeout value
	timeout = (moxa_sd->mmc->f_max/1000) * (data->timeout_ns/1000);
	timeout *= 2;

	// initialize the data control
	datactrl = (blksz_bits & SD_BLK_SIZE_MASK) | SD_DATA_EN;
	if ( data->flags & MMC_DATA_WRITE ) {
		datactrl |= SD_DATA_WRITE;
	}
	if ( moxa_sd->size > SD_FIFO_LENB && moxa_sd->dma ) {
		datactrl |= SD_DMA_EN;
	}
	writel(timeout, &moxa_sd->reg->data_timer);
	writel(moxa_sd->size, &moxa_sd->reg->data_length);
	writel(datactrl, &moxa_sd->reg->data_control);

	if ( moxa_sd->size > SD_FIFO_LENB && moxa_sd->dma ) {
		writel(SD_INT_CARD_CHANGE, &moxa_sd->reg->interrupt_mask);
		moxaart_sd_do_fifo(moxa_sd, data);
	} else {
		writel(SD_INT_FIFO_URUN|SD_INT_FIFO_ORUN|SD_INT_CARD_CHANGE, &moxa_sd->reg->interrupt_mask);
	}
}
static void moxaart_sd_send_command(struct moxaart_sd_host *moxa_sd, struct mmc_command *cmd)
{
        u32    status, cmdctrl;
        int             retry=0;
	dbg_printk("\n");

	cmd->error = -ETIMEDOUT;

	// first clear status
	writel(SD_CLR_RSP_TIMEOUT|SD_CLR_RSP_CRC_OK|SD_CLR_RSP_CRC_FAIL|SD_CLR_CMD_SENT, &moxa_sd->reg->clear);

	// write argument
	writel(cmd->arg, &moxa_sd->reg->argument);

	// write command
	cmdctrl = cmd->opcode & SD_CMD_IDX_MASK;
	if ( cmdctrl == SD_APP_SET_BUS_WIDTH ||
		cmdctrl == SD_APP_OP_COND ||
		cmdctrl == SD_APP_SEND_SCR )       // this is SD application specific command
		cmdctrl |= SD_APP_CMD;
	if ( cmd->flags & MMC_RSP_136 ) {
		cmdctrl |= (SD_LONG_RSP|SD_NEED_RSP);
	} else {
		cmdctrl |= SD_NEED_RSP;
	}
	writel(cmdctrl|SD_CMD_EN, &moxa_sd->reg->command);

	// wait response
	while ( retry++ < SD_RETRY_COUNT ) {
		status = readl(&moxa_sd->reg->status);
		if ( status & SD_CARD_DETECT ) {       // card is removed
			cmd->error = -ETIMEDOUT;
			break;
		}
		if ( cmdctrl & SD_NEED_RSP ) {
			if ( status & SD_RSP_TIMEOUT ) {
				writel(SD_CLR_RSP_TIMEOUT, &moxa_sd->reg->clear);
				cmd->error = -ETIMEDOUT;
				break;
			}
			if ( (cmd->flags&MMC_RSP_CRC) && (status&SD_RSP_CRC_FAIL) ) {
				writel(SD_CLR_RSP_CRC_FAIL, &moxa_sd->reg->clear);
				cmd->error = -EILSEQ;
				break;
			}
			if ( status & SD_RSP_CRC_OK ) {
				writel(SD_CLR_RSP_CRC_OK, &moxa_sd->reg->clear);
				// read response
				cmd->resp[0] = readl(&moxa_sd->reg->response0);
				cmd->resp[1] = readl(&moxa_sd->reg->response1);
				cmd->resp[2] = readl(&moxa_sd->reg->response2);
				cmd->resp[3] = readl(&moxa_sd->reg->response3);
				cmd->error = ERR_NONE;
				break;
			}
		} else {
			if ( status & SD_CMD_SENT ) {
				writel(SD_CLR_CMD_SENT, &moxa_sd->reg->clear);
				cmd->error = ERR_NONE;
				break;
			}
		}
	}
}

static void moxaart_sd_request_done(struct moxaart_sd_host *moxaart_sd)
{
        struct mmc_request      *mrq=moxaart_sd->mrq;

        if ( mrq == NULL ) {
                return;
        }
        moxaart_sd->mrq = NULL;
        moxaart_sd->data = NULL;
        mmc_request_done(moxaart_sd->mmc, mrq);
}

static int moxaart_sd_card_present(struct mmc_host *mmc)
{
        struct moxaart_sd_host      *moxaart_sd=mmc_priv(mmc);

        return ((readl(&moxaart_sd->reg->status) & SD_CARD_DETECT) == 0);
}


static void moxaart_sd_request(struct mmc_host *mmc, struct mmc_request *mrq)
{
	struct moxaart_sd_host      *moxaart_sd=mmc_priv(mmc);
	struct mmc_command      *cmd;
	unsigned long           flags;

	dbg_printk("\n");
	spin_lock_irqsave(&moxaart_sd->lock, flags);
	moxaart_sd->mrq = mrq;
	cmd = mrq->cmd;

	// if no card inserted, return timeout error
	if (!moxaart_sd_card_present(mmc)){ // card is removed
		cmd->error = -ENOMEDIUM;
		goto request_done;
	}

	// request include data or not
	if ( cmd->data ) {
		moxaart_sd_prepare_data(moxaart_sd, cmd->data);
	}

        // do request command
	moxaart_sd_send_command(moxaart_sd, cmd);

	if ( cmd->data && cmd->error == ERR_NONE ) {
		spin_unlock_irqrestore(&moxaart_sd->lock, flags);
		return;
        }

request_done:
	moxaart_sd_request_done(moxaart_sd);
	spin_unlock_irqrestore(&moxaart_sd->lock, flags);

}

#define MMC_VDD_360     23
#define MIN_POWER       (MMC_VDD_360 - SD_POWER_MASK)
static void moxaart_sd_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
	struct moxaart_sd_host      *moxaart_sd=mmc_priv(mmc);
	unsigned long           flags;
	int	div;
	dbg_printk("\n");

	spin_lock_irqsave(&moxaart_sd->lock, flags);
	if (ios->clock) {
#ifdef MOXAART_SD_SUPPORT_GET_CLOCK
		div = (moxaart_sd->sysclk / (moxaart_sd->mmc->f_max * 2)) - 1;
#else
		div = (APB_CLK / (moxaart_sd->mmc->f_max * 2)) - 1;
#endif
		if ( div > SD_CLK_DIV_MASK )
			div = SD_CLK_DIV_MASK;
		else if ( div < 0 )
                        div = 0;

		dbg_printk("div=0x%x\n", div);
		
		div |= SD_CLK_SD;
		writel(div, &moxaart_sd->reg->clock_control);
		
	} else if ( !(readl(&moxaart_sd->reg->clock_control) & SD_CLK_DIS) ) {
		/*
		* Ensure that the clock is off.
		*/
		dbg_printk("div=0x%x\n", div);		
		writel(readl(&moxaart_sd->reg->clock_control)|SD_CLK_DIS, &moxaart_sd->reg->clock_control);
	}

	if ( ios->power_mode == MMC_POWER_OFF ) {
		writel(readl(&moxaart_sd->reg->power_control)&~SD_POWER_ON, &moxaart_sd->reg->power_control);
	} else {
		unsigned short  power;
		if ( ios->vdd < MIN_POWER )
			power = 0;
		else
			power = ios->vdd - MIN_POWER;
		writel(SD_POWER_ON|(u32)power, &moxaart_sd->reg->power_control);
        }

	if ( ios->bus_width == MMC_BUS_WIDTH_1 ) {
		writel(SD_SINGLE_BUS, &moxaart_sd->reg->bus_width);
	} else {
		writel(SD_WIDE_BUS, &moxaart_sd->reg->bus_width);
	}
	spin_unlock_irqrestore(&moxaart_sd->lock, flags);

}

static int      moxaart_sd_get_ro(struct mmc_host *mmc)
{
        struct moxaart_sd_host      *moxaart_sd=mmc_priv(mmc);

        if ( readl(&moxaart_sd->reg->status) & SD_WRITE_PROT)
                return 1;
        else
                return 0;	
}

static struct mmc_host_ops moxaart_sd_ops = {
	.request	= moxaart_sd_request,
	.set_ios	= moxaart_sd_set_ios,
	.get_ro	= moxaart_sd_get_ro,
	.get_cd	= moxaart_sd_card_present,
};

static void moxaart_sd_fifo_run(unsigned long param)
{
	struct moxaart_sd_host      *moxaart_sd=(struct moxaart_sd_host *)param;
	struct mmc_data         *data;
	unsigned long           flags;
	u32            status;

	spin_lock_irqsave(&moxaart_sd->lock, flags);
	
	moxaart_sd = (struct moxaart_sd_host *)param;
	data = moxaart_sd->data;
	
	if ( moxaart_sd->mrq == NULL || data == NULL ) {
		spin_unlock_irqrestore(&moxaart_sd->lock, flags);
		return;
	}

	moxaart_sd_do_fifo(moxaart_sd, data);
	
	if ( moxaart_sd->size == data->bytes_xfered ) {
		while ( 1 ) {
			status = readl(&moxaart_sd->reg->status);
			if ( status & (SD_DATA_CRC_OK|SD_DATA_CRC_FAIL|SD_DATA_END) )
				break;
			current->state = TASK_INTERRUPTIBLE;
			schedule_timeout(5);
		}
		if ( status & SD_DATA_CRC_OK ) {
			writel(SD_CLR_DATA_CRC_OK, &moxaart_sd->reg->clear);
		}
		if ( status & SD_DATA_CRC_FAIL ) {
			writel(SD_CLR_DATA_CRC_FAIL, &moxaart_sd->reg->clear);
			data->error = -EILSEQ;
		}
		if ( status & SD_DATA_END ) {
			writel(SD_CLR_DATA_END, &moxaart_sd->reg->clear);
		}
		if ( data->stop ) {
			moxaart_sd_send_command(moxaart_sd, data->stop);
		}
	} else {
		spin_unlock_irqrestore(&moxaart_sd->lock, flags);
		return;
	}

	moxaart_sd_request_done(moxaart_sd);

	spin_unlock_irqrestore(&moxaart_sd->lock, flags);
}

static void moxaart_sd_card_change(unsigned long param)
{
	struct mmc_host *mmc = (struct mmc_host *)param;
	struct moxaart_sd_host *moxaart_sd = mmc_priv(mmc);
	int                     delay;
	unsigned long           flags;

	spin_lock_irqsave(&moxaart_sd->lock, flags);
		
	udelay(1000);
	udelay(1000);
	udelay(1000);
		
	if (!moxaart_sd_card_present(mmc)){ // card is removed
		printk("Moxa CPU SD/MMC card is removed.\n");
		delay = 0;
		if ( moxaart_sd->data ) {
			if ( moxaart_sd->dma && moxaart_sd->size > SD_FIFO_LENB ) {
				apb_dma_disable(moxaart_sd->dma);
			}
			moxaart_sd->size = moxaart_sd->data->bytes_xfered;
			moxaart_sd_fifo_run(*(unsigned long *)moxaart_sd);
			moxaart_sd->data->error = -ENOMEDIUM;
			moxaart_sd_request_done(moxaart_sd);
		}
		
		if ( moxaart_sd->mrq ) {
			moxaart_sd->mrq->cmd->error = -ENOMEDIUM;
			moxaart_sd_request_done(moxaart_sd);
		}
	} else {        // card inserted
		printk("Moxa CPU SD/MMC card is inserted.\n");
		if ( readl(&moxaart_sd->reg->clock_control) & SD_CLK_SD ) {  // SD
			moxaart_sd->mmc->f_max = 25000000;
		} else {
			moxaart_sd->mmc->f_max = 20000000;
		}
		delay = 500;
	}
	writel(SD_CLR_CARD_CHANGE, &moxaart_sd->reg->clear);
	spin_unlock_irqrestore(&moxaart_sd->lock, flags);
	mmc_detect_change(moxaart_sd->mmc, msecs_to_jiffies(delay));

}


static int __devinit  moxaart_sd_probe(struct platform_device *pdev)
{
	struct resource         *res;
	struct moxaart_sd_platform_data *plat_data = kzalloc( sizeof(struct moxaart_sd_platform_data), GFP_KERNEL);
	struct mmc_host         *mmc;
	struct moxaart_sd_host      *moxaart_sd=NULL;
	int ret = 0;

	if(!plat_data){
		printk("%s: Failed to allocate memory!\n", pdev->name);
		return -ENOMEM;
	}	
	pdev->dev.platform_data = plat_data;

	/*Try to get the platform resources.*/
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if ( !res) {
		printk("%s: Failed to get the memory resource!\n", pdev->name);
		ret = -ENODEV;
		goto get_resource_failed;
	}
	plat_data->phyaddr = res->start;
	plat_data->phyaddr_len = res->end - res->start + 1;
	
	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if ( !res) {
		printk("%s: Failed to get the IRQ number!\n", pdev->name);
		ret = -ENODEV;
		goto get_resource_failed;
	}
	plat_data->irq = res->start;

	/*Try to request the memory region for sd control.*/
	if ( !request_mem_region(plat_data->phyaddr, plat_data->phyaddr_len, "MoxaArtSd") ) {
		printk("%s: Failed to request hardware address region!\n", pdev->name);
		ret = -ENOMEM;
		goto get_resource_failed;
	}

	/*Try to map the physical address of the sd controller to a virtual address. */
	plat_data->virtaddr = (u32)ioremap_nocache(plat_data->phyaddr, plat_data->phyaddr_len);
	dbg_printk("plat_data->virtaddr=0x%x\n", plat_data->virtaddr);
	if ( !plat_data->virtaddr) {
		printk("%s: Failed to remap sd hardware physical address!\n", pdev->name);		
		ret = -ENOMEM;
		goto ioremap_failed;
	}

	mmc = mmc_alloc_host(sizeof(struct moxaart_sd_host), &pdev->dev); //private data size = sizeof(struct moxaart_sd_host)
	if (!mmc) {
		printk("%s: Failed to allocate mmc host!\n", pdev->name);		
		ret = -ENOMEM;
		goto alloc_mmc_host_failed;
	}

	mmc->ops = &moxaart_sd_ops;
	mmc->f_min = 400000;
	mmc->f_max = 25000000;
	mmc->ocr_avail = 0xffff00;      // support 2.0v - 3.6v power

	moxaart_sd = mmc_priv(mmc);
	moxaart_sd->mmc = mmc;
	moxaart_sd->data_window_phys = plat_data->phyaddr + MOXAART_SD_DATA_WINDOW_OFFSET;
	spin_lock_init(&moxaart_sd->lock);
	tasklet_init(&moxaart_sd->card_change_tasklet, moxaart_sd_card_change, (unsigned long)mmc);
	tasklet_init(&moxaart_sd->fifo_run_tasklet, moxaart_sd_fifo_run, (unsigned long)moxaart_sd);
	moxaart_sd->reg = (moxaart_sd_reg *)plat_data->virtaddr;
	moxaart_sd->dma = apb_dma_alloc(APB_DMA_SD_REQ_NO);
	if ( moxaart_sd->dma ) {
		apb_dma_set_irq(moxaart_sd->dma, moxaart_sd_dma_irq_handler, moxaart_sd);
	}


#ifdef MOXAART_SD_SUPPORT_GET_CLOCK
        // get system clock
        {
	        u32    mul, val, div;
	        mul = (*(volatile u32 *)(MOXAART_PMU_VA_BASE+0x30) >> 3) & 0x1ff;
	        val = (*(volatile u32 *)(MOXAART_PMU_VA_BASE+0x0c) >> 4) & 0x7;
	        switch ( val ) {
		        case 0 :        div = 2;        break;
		        case 1 :        div = 3;        break;
		        case 2 :        div = 4;        break;
		        case 3 :        div = 6;        break;
		        case 4 :        div = 8;        break;
		        default :       div = 2;        break;
	        }
	        moxaart_sd->sysclk = (38684*mul + 10000) / (div * 10000);
	        moxaart_sd->sysclk = (moxaart_sd->sysclk * 1000000) / 2;
        }
#endif

	//Key Point:  change I/O multiplexing to SD, so the GPIO 17-10 will be fail
	moxaart_gpio_mp_clear(0xff<<10);


	/*
	* Ensure that the host controller is shut down, and setup
	* with our defaults.
	*/
	writel(0, &moxaart_sd->reg->interrupt_mask);  // disable all interrupt
	writel(SD_SDC_RST, &moxaart_sd->reg->command);       // reset chip
	while ( readl(&moxaart_sd->reg->command) & SD_SDC_RST);      // wait for reset finished
	writel(0, &moxaart_sd->reg->interrupt_mask);  // disable all interrupt

	// to check any card inserted or not
	if (moxaart_sd_card_present(mmc)){ // card is present
		if ( readl(&moxaart_sd->reg->clock_control) & SD_CLK_SD ) {  // is SD card
			mmc->f_max = 25000000;
		} else {        // is MMC card
			mmc->f_max = 20000000;
		}
	}

	mmc->caps = MMC_CAP_4_BIT_DATA;
	writel(SD_WIDE_BUS, &moxaart_sd->reg->bus_width);

	/*Try to register the IRQ and its handler.*/
	if (request_irq(plat_data->irq, moxaart_sd_irq_handler, IRQF_SHARED |  IRQ_TYPE_LEVEL_HIGH, pdev->name, pdev) ) {
		printk("%s: Failed to request interrupt service!\n", pdev->name);		
		ret = -ENOMEM;
		goto request_irq_failed;
		
	}
	
	writel(SD_INT_CARD_CHANGE, &moxaart_sd->reg->interrupt_mask);
	//writel(MSD_INT_CARD_CHANGE|MSD_INT_FIFO_ORUN|MSD_INT_FIFO_URUN, &moxaart_sd->reg->interrupt_mask);
	dev_set_drvdata(&pdev->dev, mmc);
	ret = mmc_add_host(mmc);
	if(ret){
		printk("%s: Failed to add mmc host!\n", pdev->name);		
		goto add_mmc_host_failed;
	}
		
	return 0;

add_mmc_host_failed:
	free_irq(plat_data->irq, pdev);
request_irq_failed:
	mmc_free_host(mmc);
alloc_mmc_host_failed:
	iounmap((void *)plat_data->virtaddr);
ioremap_failed:
	release_mem_region(plat_data->phyaddr, plat_data->phyaddr_len);
get_resource_failed:	
	kfree(plat_data);
	return ret;
}

static int __devexit  moxaart_sd_remove(struct platform_device *pdev)
{

	struct mmc_host *mmc = dev_get_drvdata(&pdev->dev);
	struct moxaart_sd_platform_data *plat_data = dev_get_platdata(&pdev->dev);

	dev_set_drvdata(&pdev->dev, NULL);

	if (mmc) {
		struct moxaart_sd_host      *moxaart_sd=mmc_priv(mmc);

		mmc_remove_host(mmc);

		// stop SD/MMC
		if ( moxaart_sd->dma ) {
			apb_dma_disable(moxaart_sd->dma);
			apb_dma_release_irq(moxaart_sd->dma);
			apb_dma_release(moxaart_sd->dma);
		}
		writel(0, &moxaart_sd->reg->interrupt_mask);
		writel(0, &moxaart_sd->reg->power_control);
		writel(readl(&moxaart_sd->reg->clock_control)|SD_CLK_DIS, &moxaart_sd->reg->clock_control);

		tasklet_kill(&moxaart_sd->card_change_tasklet);
		tasklet_kill(&moxaart_sd->fifo_run_tasklet);

		if (plat_data){
			free_irq(plat_data->irq, pdev);
			iounmap((void *)plat_data->virtaddr);
			release_mem_region(plat_data->phyaddr, plat_data->phyaddr_len);
			kfree(plat_data);
		}
		
		mmc_free_host(mmc);
	}

	return 0;
}

static struct platform_driver moxaart_sd_driver = {
        .driver = {
                .name   = "MoxaArtSd",
                .owner  = THIS_MODULE,
        },
        .probe          = moxaart_sd_probe,
        .remove         = moxaart_sd_remove,
};

static int __init moxaart_sd_init(void)
{
        return platform_driver_register(&moxaart_sd_driver);
}

static void __exit moxaart_sd_exit(void)
{
        platform_driver_unregister(&moxaart_sd_driver);
}

module_init(moxaart_sd_init);
module_exit(moxaart_sd_exit);

MODULE_DESCRIPTION("MOXA ART MMC/SD driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Victor Yu <victor.yu@moxa.com>; Vincent Wu <vincent.wu@moxa.com>");
