/* 
 * linux/drivers/net/moxaarm11_gmac_timestamp.c
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


void moxaarm11_gmac_timestamp_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) & ~TimestampEnable, (void *)reg);
}

void moxaarm11_gmac_timestamp_interrupt_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_INT_MASK;
	writel( readl((void *)reg) | TimestampInterruptMask, (void *)reg);
}
#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
/*
 * The following functions are IEEE1588 related functions.
 */
void moxaarm11_gmac_timestamp_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	dbg_printk("%s:\n",__FUNCTION__);
	writel( readl((void *)reg) | TimestampEnable, (void *)reg);
}

void moxaarm11_gmac_timestamp_interrupt_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_INT_MASK;
	writel( readl((void *)reg) & ~TimestampInterruptMask, (void *)reg);
}

void moxaarm11_gmac_timestamp_addr_filter_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) | EnableMACAddressPTPFrameFiltering, (void *)reg);
}

void moxaarm11_gmac_timestamp_addr_filter_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) & ~EnableMACAddressPTPFrameFiltering, (void *)reg);
}

void moxaarm11_gmac_timestamp_set_clock_type(struct net_device *ndev, u32 clock_type)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	u32 val;
	val = readl((void *)reg) & ~SelectPacketsForSnapshots;
	writel( val | clock_type, (void *)reg);
}

void moxaarm11_gmac_timestamp_master_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) | SnapshotForMessagesRelevantToMaster, (void *)reg);
}

void moxaarm11_gmac_timestamp_master_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) & ~SnapshotForMessagesRelevantToMaster, (void *)reg);
}

void moxaarm11_gmac_timestamp_event_message_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) | SnapshotForEventMessages, (void *)reg);
}

void moxaarm11_gmac_timestamp_event_message_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) & ~SnapshotForEventMessages, (void *)reg);
}

void moxaarm11_gmac_timestamp_ipv4_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) | EnablePTPOverIPv4, (void *)reg);
}

void moxaarm11_gmac_timestamp_ipv4_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) & ~EnablePTPOverIPv4, (void *)reg);
}

void moxaarm11_gmac_timestamp_ipv6_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) | EnablePTPOverIPv6, (void *)reg);
}

void moxaarm11_gmac_timestamp_ipv6_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) & ~EnablePTPOverIPv6, (void *)reg);
}

void moxaarm11_gmac_timestamp_ethernet_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) | EnablePTPOverEthernet, (void *)reg);
}

void moxaarm11_gmac_timestamp_ethernet_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) & ~EnablePTPOverEthernet, (void *)reg);
}

void moxaarm11_gmac_timestamp_ptpv2(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) | EnablePTPv2, (void *)reg);
}

void moxaarm11_gmac_timestamp_ptpv1(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) & ~EnablePTPv2, (void *)reg);
}

void moxaarm11_gmac_timestamp_all_frames_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) | TimestampForAllFrames, (void *)reg);
}

void moxaarm11_gmac_timestamp_all_frames_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) & ~TimestampForAllFrames, (void *)reg);
}

void moxaarm11_gmac_timestamp_digital_rollover(struct net_device *ndev)
{
	//the timestamp low register rolls over after 0x3B9A_C9FF value.
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) | TimestampDigitalOrBinaryRollover, (void *)reg);
}

void moxaarm11_gmac_timestamp_binary_rollover(struct net_device *ndev)
{
	//the timestamp low register rolls over after 0x7FFF_FFFF value.
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) & ~TimestampDigitalOrBinaryRollover, (void *)reg);
}

s32  moxaarm11_gmac_timestamp_addend_update(struct net_device *ndev, u32 addend_value)
{
	u32 loop_variable;
	u32 addend_reg, control_reg;
	
	control_reg = ndev->base_addr + GMAC_TIMESTAMP_CONTROL;
	addend_reg = ndev->base_addr + GMAC_TIMESTAMP_ADDEND;
	
	writel(addend_value, (void *)addend_reg);// Load the addend_value in to Addend register
	for(loop_variable = 0; loop_variable < DEFAULT_LOOP_VARIABLE; loop_variable++){ //Wait till the busy bit gets cleared with in a certain amount of time
		if(!((readl((void *)control_reg)) & AddendRegUpdate)){ // if it is cleared then break
			break;
		} 
		udelay(DEFAULT_DELAY_VARIABLE);
       }
	if(loop_variable < DEFAULT_LOOP_VARIABLE)
		writel(readl((void *)control_reg) | AddendRegUpdate, (void *)control_reg);
	else{
		err_printk("Error::: The AddendRegUpdate bit is not getting cleared !!!!!!\n");
		return -1;
	}
	return 0;

}

s32  moxaarm11_gmac_timestamp_time_update(struct net_device *ndev, u32 seconds, u32 subseconds)
//If 	digitial rollover is enabled, subseconds = nanoseconds, or subseconds = 1/(2^32) seconds
{
	u32 loop_variable;
	u32 seconds_update_reg, subseconds_update_reg,control_reg;
	
	control_reg = ndev->base_addr + GMAC_TIMESTAMP_CONTROL;
	seconds_update_reg = ndev->base_addr + GMAC_SECOND_UPDATE;
	subseconds_update_reg = ndev->base_addr + GMAC_SUBSECOND_UPDATE;

	writel(seconds, (void *)seconds_update_reg);// Load the high value to Timestamp High register
	writel(subseconds, (void *)subseconds_update_reg);// Load the high value to Timestamp High register
	for(loop_variable = 0; loop_variable < DEFAULT_LOOP_VARIABLE; loop_variable++){ //Wait till the busy bit gets cleared with in a certain amount of time
		if(!((readl((void *)control_reg)) & TimestampUpdate)){ // if it is cleared then break
			break;
		} 
		udelay(DEFAULT_DELAY_VARIABLE);
	}
	if(loop_variable < DEFAULT_LOOP_VARIABLE)
		writel(readl((void *)control_reg) | TimestampUpdate, (void *)control_reg);
	else{
		err_printk("Error::: The TimestampUpdate bit is not getting cleared !!!!!!\n");
		return -1;
	}
	
	return 0;
}

s32  moxaarm11_gmac_timestamp_time_init(struct net_device *ndev, u32 seconds, u32 subseconds)
//If 	digitial rollover is enabled, subseconds = nanoseconds, or subseconds = 1/(2^32) seconds
{
	u32 loop_variable;
	u32 seconds_update_reg, subseconds_update_reg,control_reg;
	
	control_reg = ndev->base_addr + GMAC_TIMESTAMP_CONTROL;
	seconds_update_reg = ndev->base_addr + GMAC_SECOND_UPDATE;
	subseconds_update_reg = ndev->base_addr + GMAC_SUBSECOND_UPDATE;

	writel(seconds, (void *)seconds_update_reg);// Load the high value to Timestamp High register
	writel(subseconds, (void *)subseconds_update_reg);// Load the high value to Timestamp High register
	for(loop_variable = 0; loop_variable < DEFAULT_LOOP_VARIABLE; loop_variable++){ //Wait till the busy bit gets cleared with in a certain amount of time
		if(!((readl((void *)control_reg)) & TimestampInitialize)){ // if it is cleared then break
			break;
		} 
		udelay(DEFAULT_DELAY_VARIABLE);
	}
	if(loop_variable < DEFAULT_LOOP_VARIABLE)
		writel(readl((void *)control_reg) | TimestampInitialize, (void *)control_reg);
	else{
		err_printk("Error::: The TimestampInitialize bit is not getting cleared !!!!!!\n");
		return -1;
	}
	
	return 0;
}

void moxaarm11_gmac_timestamp_fine_update(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) | TimestampFineOrCoarseUpdate, (void *)reg);
}

void moxaarm11_gmac_timestamp_coarse_update(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) & ~TimestampFineOrCoarseUpdate, (void *)reg);
}

void moxaarm11_gmac_timestamp_subsecond_increment_init(struct net_device *ndev, u32 value)
{
	writel(value, (void *)(ndev->base_addr+GMAC_SUB_SECOND_INC));
}

void moxaarm11_gmac_timestamp_read_time(struct net_device *ndev, u16 * higher_sec_val, u32 * sec_val, u32 *  sub_sec_val)
{
	*higher_sec_val = readl((void *)(ndev->base_addr + GMAC_HIGHER_SECOND));
	*sec_val = readl((void *)(ndev->base_addr + GMAC_SECOND));
	*sub_sec_val = readl((void *)(ndev->base_addr + GMAC_SUBSECOND));
}

void moxaarm11_gmac_timestamp_highersecond_write(struct net_device *ndev, u32 value)
{
	writel(value & 0xFFFF, (void *)(ndev->base_addr+GMAC_HIGHER_SECOND));
}

void moxaarm11_gmac_timestamp_target_time_write(struct net_device *ndev, u32 * sec_val, u32 *  sub_sec_val)
{
	writel(sec_val, (void *)(ndev->base_addr+GMAC_TARGET_SECOND));
	writel(sub_sec_val, (void *)(ndev->base_addr+GMAC_TARGET_SUBSECOND));	
}

void moxaarm11_gmac_timestamp_clear_auxiliary_time_fifo(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_TIMESTAMP_CONTROL;
	writel( readl((void *)reg) |AuxiliarySnapshotFIFOClear, (void *)reg);

}

void moxaarm11_gmac_ptp_get_rx_timestamp(moxaarm11_gmac_priv_t *priv, struct ifreq *ifr, moxaarm11_gamc_timestamp_t *rx_time, int mode)
{
	printk("rxTimestampReadIndex: %d  seq: %d\n",priv->rxTimestampReadIndex, priv->rxTimestamp[priv->rxTimestampReadIndex].seq);
	if(priv->rxTimestamp[priv->rxTimestampReadIndex].valid == 0)
	{
		rx_time->valid = 0;
		return;
	}

	rx_time->valid = 1;
	rx_time->seq = priv->rxTimestamp[priv->rxTimestampReadIndex].seq;
	rx_time->higherSeconds = priv->rxTimestamp[priv->rxTimestampReadIndex].higherSeconds;
	rx_time->seconds = priv->rxTimestamp[priv->rxTimestampReadIndex].seconds;
	rx_time->subseconds = priv->rxTimestamp[priv->rxTimestampReadIndex].subseconds;
	
	priv->rxTimestamp[priv->rxTimestampReadIndex].valid = 0;
	priv->rxTimestampReadIndex = (priv->rxTimestampReadIndex+1 >= RX_TIMESTAMP_BUF_SIZE)? 0 : priv->rxTimestampReadIndex+1;

}

void moxaarm11_gmac_ptp_get_tx_timestamp(moxaarm11_gmac_priv_t *priv, struct ifreq *ifr, moxaarm11_gamc_timestamp_t *tx_time)
{
	dbg_printk("txTimestampReadIndex: %d  seq: %d\n",priv->txTimestampReadIndex, priv->txTimestamp[priv->txTimestampReadIndex].seq);
	if(priv->txTimestamp[priv->txTimestampReadIndex].valid == 0)
	{
		tx_time->valid = 0;
		return;
	}

	tx_time->valid = 1;
	tx_time->seq = priv->txTimestamp[priv->txTimestampReadIndex].seq;
	tx_time->higherSeconds = priv->txTimestamp[priv->txTimestampReadIndex].higherSeconds;
	tx_time->seconds = priv->txTimestamp[priv->txTimestampReadIndex].seconds;
	tx_time->subseconds = priv->txTimestamp[priv->txTimestampReadIndex].subseconds;
	dbg_printk("tx_time->seconds=%ld \n", tx_time->seconds);

	priv->txTimestamp[priv->txTimestampReadIndex].valid = 0;
	priv->txTimestampReadIndex = (priv->txTimestampReadIndex+1 >= TX_TIMESTAMP_BUF_SIZE)? 0 : priv->txTimestampReadIndex+1;

}

void moxaarm11_gmac_ptp_adj_freq(moxaarm11_gmac_priv_t *priv, u32 *addend_value)
{
	if (priv->ndev == NULL)
	{
		err_printk("%s: net device pointer is missing. \n", __FUNCTION__);
		return;
	}
	moxaarm11_gmac_timestamp_addend_update(priv->ndev, *addend_value);
}

void moxaarm11_gmac_ptp_get_freq(moxaarm11_gmac_priv_t *priv, u32 *addend_value)
{
	if (priv->ndev == NULL)
	{
		err_printk("%s: net device pointer is missing. \n", __FUNCTION__);
		return;
	}
	*addend_value = readl((void *)(priv->ndev->base_addr + GMAC_TIMESTAMP_ADDEND));
}

void moxaarm11_gmac_ptp_set_period(moxaarm11_gmac_priv_t *priv, u32 *increment_value)
{
	if (priv->ndev == NULL)
	{
		err_printk("%s: net device pointer is missing. \n", __FUNCTION__);
		return;
	}
	moxaarm11_gmac_timestamp_subsecond_increment_init(priv->ndev, *increment_value);
}

void moxaarm11_gmac_ptp_get_period(moxaarm11_gmac_priv_t *priv, u32 *increment_value)
{
	if (priv->ndev == NULL)
	{
		err_printk("%s: net device pointer is missing. \n", __FUNCTION__);
		return;
	}
	*increment_value = readl((void *)(priv->ndev->base_addr + GMAC_SUB_SECOND_INC));
}

void moxaarm11_gmac_ptp_set_time(moxaarm11_gmac_priv_t *priv, moxaarm11_gamc_timestamp_t *time)
{
	if (priv->ndev == NULL)
	{
		err_printk("%s: net device pointer is missing. \n", __FUNCTION__);
		return;
	}
	moxaarm11_gmac_timestamp_highersecond_write(priv->ndev, time->higherSeconds);
	moxaarm11_gmac_timestamp_time_init(priv->ndev, time->seconds, time->subseconds);
}

void moxaarm11_gmac_ptp_get_time(moxaarm11_gmac_priv_t *priv, moxaarm11_gamc_timestamp_t *time)
{
	if (priv->ndev == NULL)
	{
		err_printk("%s: net device pointer is missing. \n", __FUNCTION__);
		time->valid = 0;
		return;
	}
	time->valid = 1;
	moxaarm11_gmac_timestamp_read_time(priv->ndev, &time->higherSeconds, &time->seconds, &time->subseconds);
}

void moxaarm11_gmac_ptp_update_time(moxaarm11_gmac_priv_t *priv, moxaarm11_gamc_timestamp_t *time)
{
	if (priv->ndev == NULL)
	{
		err_printk("%s: net device pointer is missing. \n", __FUNCTION__);
		return;
	}
	moxaarm11_gmac_timestamp_time_update(priv->ndev, time->seconds, time->subseconds);
}
#endif
