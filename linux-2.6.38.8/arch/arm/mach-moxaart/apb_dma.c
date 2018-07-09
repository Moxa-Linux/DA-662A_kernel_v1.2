
/*
 * Copyright (C) 2005 Moxa Group All Rights Reserved.
 *
 * History:
 *	Date			Author			Comment
 *	12-01-2005	Victor Yu.		Create it.
 *	11-03-2011	Vincent Wu.		Modified for 2.6.38.8
 */
#include	<mach/moxaart/moxaart.h>
#include	<linux/module.h>
#include <linux/dma-mapping.h>
#include	<linux/init.h>
#include	<linux/interrupt.h>
#include	<linux/mm.h>
#include	<linux/slab.h>
#include	<asm/io.h>
#include	<mach/dma.h>
#include	<asm/cacheflush.h>
#include	<asm/irq.h>
#include	<mach/moxaart_int.h>
#include	<linux/irq.h>
#include	<linux/delay.h>

//#define MOXA_DEBUG
#ifdef MOXA_DEBUG
#define DBG(x...)	printk(x)
#define dbg_printk(fmt, s...)	printk("%s,%d:" fmt, __FUNCTION__, __LINE__, ##s)
#else
#define DBG(x...)
#define dbg_printk(fmt, s...)	
#endif


static apb_dma_priv	apb_dma_channel[APB_DMA_MAX_CHANNEL];
static spinlock_t	apb_dma_lock;

apb_dma_priv	*apb_dma_alloc(int req_no)
{
	int		i;
	unsigned long	flags;
	apb_dma_priv	*priv=apb_dma_channel;

	spin_lock_irqsave(&apb_dma_lock, flags);
	for ( i=0; i<APB_DMA_MAX_CHANNEL; i++, priv++ ) {
		if ( priv->used_flag == 0 ) {
			priv->used_flag = 1;
			priv->irq_handler = NULL;
			priv->irq_handler_param = NULL;
			priv->conf_param = NULL;
			priv->error_flag = 0;
			priv->req_no = req_no;
			switch ( req_no ) {
			case APB_DMA_SPI_TX_REQ_NO :
				*(unsigned int *)(MOXAART_PMU_VA_BASE+0xA0) = 0;
				break;
			case APB_DMA_SPI_RX_REQ_NO :
				*(unsigned int *)(MOXAART_PMU_VA_BASE+0xA4) = 0;
				break;
			case APB_DMA_SD_REQ_NO :
				*(unsigned int *)(MOXAART_PMU_VA_BASE+0xB8) = 0;
				break;
			case APB_DMA_AC97_TX_REQ_NO :
				*(unsigned int *)(MOXAART_PMU_VA_BASE+0xBC) = 0;
				break;
			case APB_DMA_AC97_RX_REQ_NO :
				*(unsigned int *)(MOXAART_PMU_VA_BASE+0xC0) = 0;
				break;
			case APB_DMA_USB_DEVICE_REQ_NO :
				*(unsigned int *)(MOXAART_PMU_VA_BASE+0xCC) = 0;
				break;
			}
			spin_unlock_irqrestore(&apb_dma_lock, flags);
			DBG("apb_dma_alloc uses DMA channel %d\n", i);
			return priv;
		}
	}
	spin_unlock_irqrestore(&apb_dma_lock, flags);
	return NULL;
}
EXPORT_SYMBOL(apb_dma_alloc);

void	apb_dma_release(apb_dma_priv *priv)
{
	unsigned long	flags;

	spin_lock_irqsave(&apb_dma_lock, flags);
	if ( priv == NULL ) {
		spin_unlock_irqrestore(&apb_dma_lock, flags);
		return;
	}
	priv->used_flag = 0;
	priv->error_flag = 0;
	priv->irq_handler = NULL;
	priv->irq_handler_param = NULL;
	priv->conf_param = NULL;
	priv->req_no = 0;
	spin_unlock_irqrestore(&apb_dma_lock, flags);
}
EXPORT_SYMBOL(apb_dma_release);

void	apb_dma_set_irq(apb_dma_priv *priv, void (*func)(void *param), void *param)
{
	unsigned long	flags;
	unsigned int	cmd;

	spin_lock_irqsave(&apb_dma_lock, flags);
	if ( priv == NULL ) {
		spin_unlock_irqrestore(&apb_dma_lock, flags);
		return;
	}
	priv->irq_handler = func;
	priv->irq_handler_param = param;
	priv->error_flag = 0;
	cmd = readl(&priv->reg->command.ul);
	cmd |= (APB_DMA_FIN_INT_EN | APB_DMA_ERR_INT_EN);
	writel(cmd, &priv->reg->command.ul);
	spin_unlock_irqrestore(&apb_dma_lock, flags);
}
EXPORT_SYMBOL(apb_dma_set_irq);

void	apb_dma_release_irq(apb_dma_priv *priv)
{
	unsigned long	flags;
	unsigned int	cmd;

	spin_lock_irqsave(&apb_dma_lock, flags);
	if ( priv == NULL ) {
		spin_unlock_irqrestore(&apb_dma_lock, flags);
		return;
	}
	cmd = readl(&priv->reg->command.ul);
	cmd &= ~(APB_DMA_FIN_INT_EN | APB_DMA_ERR_INT_EN);
	writel(cmd, &priv->reg->command.ul);
	priv->irq_handler = NULL;
	priv->irq_handler_param = NULL;
	priv->error_flag = 0;
	spin_unlock_irqrestore(&apb_dma_lock, flags);
}
EXPORT_SYMBOL(apb_dma_release_irq);

void	apb_dma_conf(apb_dma_priv *priv, apb_dma_conf_param *param)
{
	unsigned long	flags;
	union _command	cmd;
	unsigned int	size;

	if ( param == NULL || priv == NULL )
		return;
	spin_lock_irqsave(&apb_dma_lock, flags);
#ifdef CONFIG_UCLINUX
	writel(param->source_virt_addr, &priv->reg->source_addr);
	writel(param->dest_virt_addr, &priv->reg->dest_addr);
#else	// CONFIG_UCLINUX
	priv->conf_param = param;
	dbg_printk("param->source_virt_addr=0x%x\n", param->source_virt_addr);		
	dbg_printk("param->source_phys_addr=0x%x\n", param->source_phys_addr);		
	writel(param->source_phys_addr, &priv->reg->source_addr);

	dbg_printk("param->dest_virt_addr=0x%x\n", param->dest_virt_addr);
	dbg_printk("param->dest_phys_addr=0x%x\n", param->dest_phys_addr);		
	writel(param->dest_phys_addr, &priv->reg->dest_addr);
#endif	// CONFIG_UCLINUX
	size = param->size;
	switch ( param->data_width ) {
	case APB_DMAB_DATA_WIDTH_1 :
		break;
	case APB_DMAB_DATA_WIDTH_2 :
		size >>= 1;
		break;
	case APB_DMAB_DATA_WIDTH_4 :
	default :
		size >>= 2;
		break;
	}
	if ( param->burst_mode )
		size >>= 2;
	writel(size, &priv->reg->cycles);
	cmd.ul = readl(&priv->reg->command.ul);
	cmd.bits.data_width = param->data_width;
	if ( param->dest_sel == APB_DMAB_DEST_AHB ) {	// AHB
#ifndef CONFIG_UCLINUX	// add by Victor Yu. 07-10-2007
		dma_map_single(NULL, (void *)param->dest_virt_addr, param->size, DMA_FROM_DEVICE);// equivalent to dmac_inv_range
#endif
		cmd.bits.dest_req_no = 0;
	} else {	// APB
		cmd.bits.dest_req_no = priv->req_no;
	}
	cmd.bits.dest_sel = param->dest_sel;
	if ( param->source_sel == APB_DMAB_SOURCE_AHB ) {	// AHB
#ifndef CONFIG_UCLINUX
		dma_map_single(NULL, (void*)param->source_virt_addr, param->size, DMA_TO_DEVICE); // equivalent to dmac_flush_range
#endif	// CONFIG_UCLINUX
		cmd.bits.source_req_no = 0;
	} else {	// APB
		cmd.bits.source_req_no = priv->req_no;
	}
	cmd.bits.source_sel = param->source_sel;
	cmd.bits.burst = param->burst_mode;
	cmd.bits.dest_inc = param->dest_inc;
	cmd.bits.source_inc = param->source_inc;
	writel(cmd.ul, &priv->reg->command.ul);

	dbg_printk("&priv->reg = 0x%x\n", priv->reg);
	dbg_printk("read &priv->reg->source_addr =0x%x\n", readl(&priv->reg->source_addr));
	dbg_printk("read &priv->reg->dest_addr =0x%x\n", readl(&priv->reg->dest_addr));
	dbg_printk("read &priv->reg->cycles =0x%x\n", readl(&priv->reg->cycles));
	dbg_printk("read &priv->reg->command =0x%x\n", readl(&priv->reg->command));
	
	spin_unlock_irqrestore(&apb_dma_lock, flags);
	
}
EXPORT_SYMBOL(apb_dma_conf);

void	apb_dma_enable(apb_dma_priv *priv)
{
	unsigned long	flags;
	unsigned int	cmd;

	spin_lock_irqsave(&apb_dma_lock, flags);
	cmd = readl(&priv->reg->command.ul);
	cmd |= APB_DMA_ENABLE;
	writel(cmd, &priv->reg->command.ul);	
	spin_unlock_irqrestore(&apb_dma_lock, flags);
}

EXPORT_SYMBOL(apb_dma_enable);

void	apb_dma_disable(apb_dma_priv *priv)
{
	unsigned long	flags;
	unsigned int	cmd;

	spin_lock_irqsave(&apb_dma_lock, flags);
	cmd = readl(&priv->reg->command.ul);
	cmd &= ~APB_DMA_ENABLE;
	writel(cmd, &priv->reg->command.ul);	
	spin_unlock_irqrestore(&apb_dma_lock, flags);
}
EXPORT_SYMBOL(apb_dma_disable);

static irqreturn_t apb_dma_irq(int irq, void *pdev)//(int irq, void *devid, struct pt_regs *regs)
{
	int		i;
	unsigned int	cmd;
	apb_dma_priv	*priv=apb_dma_channel;

	DBG("apb_dma_irq test01\n");
	for ( i=0; i<APB_DMA_MAX_CHANNEL; i++, priv++ ) {
		cmd = readl(&priv->reg->command.ul);
		if ( cmd & APB_DMA_FIN_INT_STS ) {
			DBG("apb_dma_irq finish interrupt channel [%d]\n", i);
			cmd &= ~APB_DMA_FIN_INT_STS;
#ifndef CONFIG_UCLINUX
			{
			apb_dma_conf_param	*conf;
			if ( (conf=priv->conf_param) != NULL ) {
				if ( conf->dest_sel == APB_DMAB_DEST_AHB ) {	// to DRAM
					dma_unmap_single(NULL, conf->dest_phys_addr, conf->size, DMA_FROM_DEVICE);// equivalent to dmac_inv_range					
				}
				priv->conf_param = NULL;
			}
			}
#endif	// CONFIG_UCLINUX
		}
		if ( cmd & APB_DMA_ERR_INT_STS ) {
			DBG("apb_dma_irq error interrupt channel [%d]\n", i);
			cmd &= ~APB_DMA_ERR_INT_STS;
			priv->error_flag = 1;
		}
		if ( priv->used_flag && priv->irq_handler ) {
			priv->irq_handler(priv->irq_handler_param);
		}
		priv->error_flag = 0;
		writel(cmd, &priv->reg->command.ul);
	}

	return IRQ_HANDLED;
}

static int __init	apb_dma_init(void)
{
	int		ret, i;
	apb_dma_priv	*priv=apb_dma_channel;

	printk("Moxa CPU APB DMA Device Driver V1.0 load ");

	memset(apb_dma_channel, 0, sizeof(apb_dma_channel));
	spin_lock_init(&apb_dma_lock);

	for ( i=0; i<APB_DMA_MAX_CHANNEL; i++, priv++ ) {
		priv->reg = (apb_dma_reg *)(MOXAART_APBDMA_VA_BASE+0x80+i*sizeof(apb_dma_reg));
	}

	ret = request_irq(IRQ_APBDMA, apb_dma_irq,  IRQF_DISABLED | IRQ_TYPE_EDGE_RISING, "APB DMA", NULL);
	if ( ret )
		printk("fail !\n");
	else
		printk("OK.\n");
	
	return ret;
}

module_init(apb_dma_init);
