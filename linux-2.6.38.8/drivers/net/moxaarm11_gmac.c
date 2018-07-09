/* 
 * linux/drivers/net/moxaarm11_gmac.c
 *
 * Copyright 2010 Moxa Tech., Inc.
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
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>
#include <linux/etherdevice.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/phy.h>
#include <linux/platform_device.h>
#include <linux/mii.h>
#include <linux/skbuff.h>
#include <linux/if.h>
#include <linux/mtd/mtd.h>

#include <mach/map.h>
#include <mach/system-control.h>

//#define MOXAARM11_GMAC_TIMESTAMP_ENABLE

#define MOXAARM11_GMAC_IPC_OFFLOAD

#ifdef CONFIG_MOXAMACRO_GMAC_NAPI
#define NAPI_WEIGHT 32
#endif

#include "moxaarm11_gmac.h"
#include "moxaarm11_gmac_mii.c"
#include "moxaarm11_gmac_timestamp.c"

/* The following are the basic register control functions. */
void moxaarm11_gmac_set_mac_address(u32 base, unsigned char *macaddr)
{
	u32	val;
	
	val = (((u32)macaddr[5] << 8) &0xff00) | ((u32)macaddr[4] & 0xff);
	writel(val, (void *)base);
	val = (((u32)macaddr[3]<<24) & 0xff000000) |
	      (((u32)macaddr[2]<<16) & 0x00ff0000) |
	      (((u32)macaddr[1]<<8)  & 0x0000ff00) |
	      (((u32)macaddr[0]) & 0x000000ff);
	writel(val, (void *)(base+4));
	
}

void moxaarm11_gmac_get_mac_address(u32 base, unsigned char *macaddr)
{
	u32	val;

	val = readl((void *)base);
	macaddr[0] = (val >> 8) & 0xff;
	macaddr[1] = val & 0xff;
	val = readl((void *)(base+4));
	macaddr[2] = (val >> 24) & 0xff;
	macaddr[3] = (val >> 16) & 0xff;
	macaddr[4] = (val >> 8) & 0xff;
	macaddr[5] = val & 0xff;
}

void moxaarm11_gmac_reset(struct net_device *ndev)
{
	u32	data, reg=ndev->base_addr+GMAC_DMA_BUS_MODE;
	u32 count=0;

	writel(SoftwareReset, (void *)reg);	// software reset
	while ( readl((void *)reg) & SoftwareReset ){
		mdelay(10);
		count++;
		if(count >= 100){
			err_printk("GMAC reset timeout!\n");
			break;
		}	
	}
	data = readl((void *)reg);
	dbg_printk("DATA after Reset = %08x\n", data);	
}

void inline moxaarm11_gmac_rx_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	
	writel( readl((void *)reg) | ReceiverEnable, (void *)reg);
}

void inline moxaarm11_gmac_rx_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	
	writel( readl((void *)reg) & ~ReceiverEnable, (void *)reg);
}

void inline moxaarm11_gmac_tx_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	
	writel( readl((void *)reg) | TransmitterEnable, (void *)reg);

}

void inline moxaarm11_gmac_tx_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	
	writel( readl((void *)reg) & ~TransmitterEnable, (void *)reg);
}

void inline moxaarm11_gmac_dma_rx_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_DMA_OP_MODE;
	
	writel( readl((void *)reg) | StartReceive, (void *)reg);
}

void inline moxaarm11_gmac_dma_rx_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_DMA_OP_MODE;
	
	writel( readl((void *)reg) & ~StartReceive, (void *)reg);
}

void inline moxaarm11_gmac_dma_tx_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_DMA_OP_MODE;
	
	writel( readl((void *)reg) | StartTransmission, (void *)reg);
}

void inline moxaarm11_gmac_dma_tx_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_DMA_OP_MODE;
	
	writel( readl((void *)reg) & ~StartTransmission, (void *)reg);
}


void inline moxaarm11_gmac_dma_bus_mode_init(struct net_device *ndev, u32 init_value )
{
	writel(init_value, (void *)(ndev->base_addr+GMAC_DMA_BUS_MODE));
}

void inline moxaarm11_gmac_dma_operation_mode_init(struct net_device *ndev, u32 init_value )
{
	writel(init_value, (void *)(ndev->base_addr+GMAC_DMA_OP_MODE));
}

void inline moxaarm11_gmac_init_rx_desc_base(struct net_device *ndev, u32 init_value)
{
	writel(init_value, (void *)(ndev->base_addr+GMAC_DMA_RXDESC_ADDR));
}

void moxaarm11_gmac_init_tx_desc_base(struct net_device *ndev, u32 init_value )
{
	writel(init_value, (void *)(ndev->base_addr+GMAC_DMA_TXDESC_ADDR));
}

void inline moxaarm11_gmac_resume_dma_tx(struct net_device *ndev)
{
	writel(0, (void *)(ndev->base_addr+GMAC_DMA_TXPOLL_DEMAND));
}

void inline moxaarm11_gmac_resume_dma_rx(struct net_device *ndev)
{
	writel(0, (void *)(ndev->base_addr+GMAC_DMA_RXPOLL_DEMAND));
}	

void inline moxaarm11_gmac_dma_axi_mode_init(struct net_device *ndev, u32 init_value)
{
	writel(init_value, (void *)(ndev->base_addr+GMAC_DMA_AXI_MODE));
}	
void inline moxaarm11_gmac_watchdog_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) & ~WatchdogDisable, (void *)reg);
}

void inline moxaarm11_gmac_watchdog_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) | WatchdogDisable, (void *)reg);
}

void inline moxaarm11_gmac_jab_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) &  ~JabberDisable, (void *)reg);
}

void inline moxaarm11_gmac_jab_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) | JabberDisable, (void *)reg);
}

void inline moxaarm11_gmac_frame_burst_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) | FrameBurstEnable, (void *)reg);
}

void inline moxaarm11_gmac_frame_burst_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) & ~FrameBurstEnable, (void *)reg);
}

void inline moxaarm11_gmac_jumbo_frame_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) | JumboFrameEnable, (void *)reg);
}

void inline moxaarm11_gmac_jumbo_frame_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) & ~JumboFrameEnable, (void *)reg);
}

void inline moxaarm11_gmac_disable_carrier_sense(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) | DisableCSDT, (void *)reg);
}

void inline moxaarm11_gmac_select_gmii(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) & ~PortSelectMII, (void *)reg);
}

void inline moxaarm11_gmac_select_mii(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) | PortSelectMII, (void *)reg);
}

void inline moxaarm11_gmac_set_speed_10M(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) & ~FastEthernetSpeed, (void *)reg);
	//printk("Set GMAC: 10M base \n");		
}

void inline moxaarm11_gmac_set_speed_100M(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) | FastEthernetSpeed, (void *)reg);
	//printk("Set GMAC: 100M base \n");			
}

void inline moxaarm11_gmac_rx_own_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) & ~DisableReceiveOwn, (void *)reg);
}

void inline moxaarm11_gmac_rx_own_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) | DisableReceiveOwn, (void *)reg);
}

void inline moxaarm11_gmac_loopback_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) | LoopbackMode, (void *)reg);
}

void inline moxaarm11_gmac_loopback_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) & ~LoopbackMode, (void *)reg);
}

void inline moxaarm11_gmac_full_duplex(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) | FullDuplexMode, (void *)reg);
	//printk("Set GMAC: Full Duplex \n");	
}

void inline moxaarm11_gmac_half_duplex(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) & ~FullDuplexMode, (void *)reg);
	//printk("Set GMAC: Half Duplex \n");	
}

void inline moxaarm11_gmac_retry_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) & ~DisableRetry, (void *)reg);
}

void inline moxaarm11_gmac_retry_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) | DisableRetry, (void *)reg);
}

void inline moxaarm11_gmac_pad_crc_strip_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) | AutoPadCRCStrip, (void *)reg);
}

void inline moxaarm11_gmac_pad_crc_strip_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) & ~AutoPadCRCStrip, (void *)reg);
}

void inline moxaarm11_gmac_set_backoff_limit(struct net_device *ndev, u32 limit)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG, tmp;
	tmp = readl((void *)reg) & ~BackOffLimit1; 	//clear back of limit
	writel( tmp |limit, (void *)reg);
}

void inline moxaarm11_gmac_deferral_check_enable(struct net_device *ndev, u32 limit)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) | DeferralCheck, (void *)reg);
}

void inline moxaarm11_gmac_deferral_check_disable(struct net_device *ndev, u32 limit)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) & ~DeferralCheck, (void *)reg);
}


void inline moxaarm11_gmac_frame_filter_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) & ~ReceiveAll, (void *)reg);
}

void inline moxaarm11_gmac_frame_filter_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) | ReceiveAll, (void *)reg);
}

void inline moxaarm11_gmac_set_pass_control(struct net_device *ndev, u32 GmacPassControl)
{
	u32	reg=ndev->base_addr+GMAC_FRAME, tmp;
	tmp = readl((void *)reg) & ~PassControlFramesAllCorrectAddr; 	//clear Pass control frames
	writel( readl((void *)reg) | GmacPassControl, (void *)reg);
}

void inline moxaarm11_gmac_broadcast_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) & ~DisableBroadcastFrame, (void *)reg);
}

void inline moxaarm11_gmac_broadcast_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) | DisableBroadcastFrame, (void *)reg);
}


void inline moxaarm11_gmac_src_addr_filter_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) | SAFilterEnable, (void *)reg);
}

void inline moxaarm11_gmac_src_addr_filter_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) & ~SAFilterEnable, (void *)reg);
}

void inline moxaarm11_gmac_pass_all_multicast_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) | PassAllMulticast, (void *)reg);
}

void inline moxaarm11_gmac_pass_all_multicast_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) & ~PassAllMulticast, (void *)reg);
}

void inline moxaarm11_gmac_dst_addr_filter_inverse(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) | DAInverseFiltering, (void *)reg);
}


void inline moxaarm11_gmac_dst_addr_filter_normal(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) & ~DAInverseFiltering, (void *)reg);
}

void inline moxaarm11_gmac_multicast_hash_filter_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) | HashMulticast, (void *)reg);
}

void inline moxaarm11_gmac_multicast_hash_filter_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) & ~HashMulticast, (void *)reg);
}

void inline moxaarm11_gmac_promisc_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) | PromiscuousMode, (void *)reg);
}

void inline moxaarm11_gmac_promisc_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) & ~PromiscuousMode, (void *)reg);
}

void inline moxaarm11_gmac_unicast_hash_filter_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) | HashUnicast, (void *)reg);
}

void inline moxaarm11_gmac_unicast_hash_filter_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) & ~HashUnicast, (void *)reg);
}

void inline moxaarm11_gmac_hash_perfect_filter_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) | HashorPerfectFilter, (void *)reg);
}

void inline moxaarm11_gmac_hash_perfect_filter_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FRAME;
	writel( readl((void *)reg) & ~HashorPerfectFilter, (void *)reg);
}


void inline moxaarm11_gmac_unicast_pause_frame_detect_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FLOW_CONTROL;
	writel( readl((void *)reg) | UnicastPauseFrameDetect, (void *)reg);
}

void inline moxaarm11_gmac_unicast_pause_frame_detect_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FLOW_CONTROL;
	writel( readl((void *)reg) & ~UnicastPauseFrameDetect, (void *)reg);
}

void inline moxaarm11_gmac_rx_flow_control_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FLOW_CONTROL;
	writel( readl((void *)reg) | ReceiveFlowControlEnable, (void *)reg);

}

void inline moxaarm11_gmac_rx_flow_control_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FLOW_CONTROL;
	writel( readl((void *)reg) & ~ReceiveFlowControlEnable, (void *)reg);

}
	
void inline moxaarm11_gmac_tx_flow_control_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FLOW_CONTROL;
	writel( readl((void *)reg) | TransmitFlowControlEnable, (void *)reg);
}

void inline moxaarm11_gmac_tx_flow_control_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_FLOW_CONTROL;
	writel( readl((void *)reg) & ~TransmitFlowControlEnable, (void *)reg);
}

void inline moxaarm11_gmac_pause_control(struct net_device *ndev)
{
	u32 dma_operation_mode;
	u32 mac_flow_control;
	u32 reg;

	reg = ndev->base_addr+GMAC_DMA_OP_MODE;
	dma_operation_mode = readl((void *)reg);
	dma_operation_mode |= ThresholdActivatingFlowControl2KB | ThresholdDeactivatingFlowControl2KB | HWFlowControl;
	writel(dma_operation_mode, (void *)reg);

	reg = ndev->base_addr+GMAC_FLOW_CONTROL;
	mac_flow_control = readl((void *)reg);
	mac_flow_control |= ReceiveFlowControlEnable | TransmitFlowControlEnable | (PauseTime&256);
	writel(mac_flow_control, (void *)reg);

	return;
}

void inline moxaarm11_gmac_rx_checksum_offload_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) | ChecksumOffload, (void *)reg);
}

void inline moxaarm11_gmac_rx_checksum_offload_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_CONFIG;
	writel( readl((void *)reg) & ~ChecksumOffload, (void *)reg);
}
	
void inline moxaarm11_gmac_mmc_interrupt_disable_all(struct net_device *ndev)
{
	writel(0xFFFFFFFF, (void *)(ndev->base_addr + GMAC_MMC_RX_INT_MASK));
	writel(0xFFFFFFFF, (void *)(ndev->base_addr + GMAC_MMC_TX_INT_MASK));	
	return;
}


/* This GMAC module supports waking up from power-down. 
 * The following are the PMT control functions.
 */
void inline moxaarm11_gmac_pmt_int_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_INT_MASK;
	writel(readl((void *)reg) &~PMTInterruptMask, (void *)reg);
}

void inline moxaarm11_gmac_pmt_int_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_INT_MASK;
	writel(readl((void *)reg) | PMTInterruptMask, (void *)reg);
}

void inline moxaarm11_gmac_power_down_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_PMT_CS;
	writel(readl((void *)reg) | PowerDown, (void *)reg);
}

void inline moxaarm11_gmac_power_down_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_PMT_CS;
	writel(readl((void *)reg) &~PowerDown, (void *)reg);
}

void inline moxaarm11_gmac_magic_packet_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_PMT_CS;
	writel(readl((void *)reg) | MagicPacketEnable, (void *)reg);
	//printk("magic packet enable, GMAC_PMT_CS=0x%x\n", readl((void *)reg));
}

void inline moxaarm11_gmac_magic_packet_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_PMT_CS;
	writel(readl((void *)reg) & ~MagicPacketEnable, (void *)reg);
	//printk("magic packet enable, GMAC_PMT_CS=0x%x\n", readl((void *)reg));
}

void inline moxaarm11_gmac_wakeup_frame_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_PMT_CS;
	writel(readl((void *)reg) | WakeUpFrameEnable, (void *)reg);
}

void inline moxaarm11_gmac_wakeup_frame_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_PMT_CS;
	writel(readl((void *)reg) & ~WakeUpFrameEnable, (void *)reg);
}

void inline moxaarm11_gmac_pmt_unicast_enable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_PMT_CS;
	writel(readl((void *)reg) | GlobalUnicast, (void *)reg);
}

void inline moxaarm11_gmac_pmt_unicast_disable(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_PMT_CS;
	writel(readl((void *)reg) & ~GlobalUnicast, (void *)reg);
}

bool inline moxaarm11_gmac_is_magic_packet_received(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_PMT_CS;
	return((readl((void *)reg) & MagicPacketReceived) == MagicPacketReceived);
}

bool inline moxaarm11_gmac_is_wakeup_frame_received(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_PMT_CS;
	return((readl((void *)reg) & WakeUpFrameReceived) == WakeUpFrameReceived);
}

#define WAKEUP_REG_LENGTH 8
void moxaarm11_gmac_write_wakeup_frame_register(struct net_device *ndev, u32 * filter_contents)
{
	s32 i;
	u32	reg=ndev->base_addr+GMAC_PMT_CS;
	writel(readl((void *)reg) | WakeUpFrameFilterPointerReset, (void *)reg);
	udelay(1);	
	for(i =0; i<WAKEUP_REG_LENGTH; i++)
		writel(*(filter_contents + i), (void *)ndev->base_addr+GMAC_WAKE_FRAME);
}

u32 moxaarm11_gmac_wakeup_filter_config[] = {
					0x00000000,	// For Filter0 CRC is not computed may be it is 0x0000
					0x000000FF,	// For Filter1 CRC is computed on 0,1,2,3,4,5,6,7 bytes from offset
					0x00000000,	// For Filter2 CRC is not computed may be it is 0x0000
					0x00000000, // For Filter3 CRC is not computed may be it is 0x0000
					0x00000100, // Filter 0,2,3 are disabled, Filter 1 is enabled and filtering applies to only unicast packets
					0x00003200, // Filter 0,2,3 (no significance), filter 1 offset is 50 bytes from start of Destination MAC address 
					0x7eED0000, // No significance of CRC for Filter0, Filter1 CRC is 0x7EED, 
					0x00000000  // No significance of CRC for Filter2 and Filter3 
};

static irqreturn_t moxaarm11_gmac_pmt_interrupt_handler(int irq, void *dev_id/*, struct pt_regs *regs*/)
{
	struct net_device *ndev;
	u32 status;


	ndev  = (struct net_device *) dev_id;
	if(ndev == NULL){
		err_printk("%s: Unknown Device.\n", __FUNCTION__);
		return -1;
	}
	status = readl((void *)(ndev->base_addr+GMAC_PMT_CS));
	//printk("%s status=0x%x\n",__FUNCTION__ , status);
	return IRQ_HANDLED;	
}

static void moxaarm11_gmac_power_down(struct net_device *ndev)
{
	//printk("Set the GMAC to power down mode...\n");
	// Disable the Dma engines in tx path
	GMAC_power_down = 1;	// Let ISR know that Mac is going to be in the power down mode
	moxaarm11_gmac_dma_tx_disable(ndev);
	udelay(10);		//allow any pending transmission to complete
	// Disable the Mac for both tx and rx
	moxaarm11_gmac_tx_disable(ndev);
	moxaarm11_gmac_rx_disable(ndev);
        udelay(10); 		//Allow any pending buffer to be read by host
	//Disable the Dma in rx path
        moxaarm11_gmac_dma_rx_disable(ndev);

	//enable the power down mode
	moxaarm11_gmac_pmt_unicast_enable(ndev);
	
	//prepare the gmac for magic packet reception and wake up frame reception
	moxaarm11_gmac_magic_packet_enable(ndev);
	moxaarm11_gmac_write_wakeup_frame_register(ndev, moxaarm11_gmac_wakeup_filter_config);
	moxaarm11_gmac_wakeup_frame_enable(ndev);	//Don't enable wakeup frame if you didn't set the wakeup frame filter

	//gate the application and transmit clock inputs to the code. This is not done in this driver :).

	//enable the Mac for reception
	moxaarm11_gmac_rx_enable(ndev);

	//Enable the assertion of PMT interrupt
	moxaarm11_gmac_pmt_int_enable(ndev);
	//enter the power down mode
	moxaarm11_gmac_power_down_enable(ndev);
}

static void moxaarm11_gmac_linux_powerup_mac(struct net_device *ndev)
{
	GMAC_power_down = 0;	// Let ISR know that MAC is out of power down now
	//Disable the assertion of PMT interrupt
	moxaarm11_gmac_pmt_int_disable(ndev);
	moxaarm11_gmac_pmt_unicast_disable(ndev);
	moxaarm11_gmac_magic_packet_disable(ndev);
	moxaarm11_gmac_wakeup_frame_disable(ndev);

	//Enable the mac and Dma rx and tx paths
	moxaarm11_gmac_rx_enable(ndev);
      	moxaarm11_gmac_dma_rx_enable(ndev);

	moxaarm11_gmac_tx_enable(ndev);
	moxaarm11_gmac_dma_tx_enable(ndev);
	return;
}

static void moxaarm11_gmac_setup_desc_ring(struct net_device *ndev)
{
	moxaarm11_gmac_priv_t	*priv=netdev_priv(ndev);
	int			i;
	tx_desc_t		*txdesc;
	rx_desc_t		*rxdesc;
	
	for ( i=0; i<TX_DESC_NUM; i++) {
		txdesc = &priv->virtTxDescBaseAddr[i];
		memset(txdesc, 0, sizeof(tx_desc_t));
		txdesc->tdes2.Buffer1AddressPointer = 0;
		txdesc->tdes0.ubit.SecondAddressChained = 0;
		txdesc->tdes0.ubit.TransmitEndofRing = 0;
	}
	priv->virtTxDescBaseAddr[TX_DESC_NUM-1].tdes0.ubit.TransmitEndofRing = 1;
	for ( i=0; i<RX_DESC_NUM; i++ ) {
		rxdesc = &priv->virtRxDescBaseAddr[i];
		dbg_printk("moxaarm11_gmac_setup_desc_ring: rxdesc: 0x%08x \n", (unsigned int)rxdesc);
		memset(rxdesc, 0, sizeof(rx_desc_t));
		rxdesc->rdes0.ubit.OwnBit = 1;
		rxdesc->rdes1.ubit.ReceiveBuffer1Size = RX_BUF_SIZE;
		rxdesc->rdes2.Buffer1AddressPointer = priv->phyRxBufBaseAddr[i];	
		rxdesc->rdes1.ubit.SecondAddressChained = 0;

	}
	priv->virtRxDescBaseAddr[RX_DESC_NUM-1].rdes1.ubit.ReceiveEndofRing = 1;
	dbg_printk("First Rx desc des0=0x%x, des1=%x\n", priv->virtRxDescBaseAddr[0].rdes0.Status, priv->virtRxDescBaseAddr[0].rdes1.ui);

	priv->TxDescFree = 0;
	priv->TxDescBusy = 0;
	priv->RxDescFree = 0;
	priv->TxDescBusy = 0;
	priv->TxPending = 0;
		
	// reset the MAC controler Tx/Rx desciptor base address
	moxaarm11_gmac_init_tx_desc_base(ndev, priv->phyTxDescBaseAddr);
	moxaarm11_gmac_init_rx_desc_base(ndev, priv->phyRxDescBaseAddr);
	
	dbg_printk("Tx/Rx desc phy=0x%x,0x%x, virt=0x%x,0x%x\n", priv->phyTxDescBaseAddr, priv->phyRxDescBaseAddr, (u32)priv->virtTxDescBaseAddr, (u32)priv->virtRxDescBaseAddr);
//	dbg_printk("set Tx desc base address=0x%x, Rx=0x%x\n", readl(dev->base_addr+TXR_BADR_REG_OFFSET), readl(dev->base_addr+RXR_BADR_REG_OFFSET));
}

static u32 moxaarm11_gmac_set_tx_desc_xmit(moxaarm11_gmac_priv_t *priv, struct sk_buff *skb)
{
	int		len, txdescfree=priv->TxDescFree;
	tx_desc_t	*txdesc=&priv->virtTxDescBaseAddr[txdescfree];

	dbg_printk("%s: ",__FUNCTION__);
	dbg_printk("TxDescFree: %02d txdesc: 0x%08x \n",txdescfree,(u32)txdesc);

	if ( unlikely(txdesc->tdes0.ubit.OwnBit) ) 
		return -1;
	dbg_printk("%s: line: %d\n",__FUNCTION__,__LINE__);	

	if ( unlikely(skb->len < ETH_ZLEN) )
		len = ETH_ZLEN;
	else
		len = skb->len;
	skb->len = len;
	priv->virtTxSkbBufBaseAddr[txdescfree] = skb;
	/*Key Point: We must always remeber to sync the content in the CPU cache and SDRAM. 
			     the function "dma_map_single" not only maps virtual addresses to physical addresses, 
			     but also sync the content from the CPU cache.*/
	priv->phyTxBufBaseAddr[txdescfree] = txdesc->tdes2.Buffer1AddressPointer = dma_map_single(&priv->ndev->dev, (void *)skb->data, len, DMA_BIDIRECTIONAL);
	dbg_printk("%s: line: %d\n",__FUNCTION__,__LINE__);	
	dbg_printk("%s: line: %d skb->len: %d\n",__FUNCTION__,__LINE__,skb->len);	
	
	txdesc->tdes1.ubit.TransmitBuffer1Size = len;
	txdesc->tdes0.ubit.FirstSegment = 1;
	txdesc->tdes0.ubit.LastSegment = 1;
	txdesc->tdes0.ubit.InterruptonCompletion = 1;

	/*if(skb->ip_summed == CHECKSUM_NONE)
	{
		printk("HW CRC\n");
		txdesc->tdes0.ubit.ChecksumInsertionControl = CIC_IP_TCP;
	}*/

#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
	txdesc->tdes0.ubit.TransmitTimestampEnable = 1;
#endif
	txdesc->tdes0.ubit.OwnBit = 1;
	dbg_printk("%s: line: %d\n",__FUNCTION__,__LINE__);	
			
	txdescfree = (txdesc->tdes0.ubit.TransmitEndofRing == 1) ? 0 : txdescfree + 1;
	priv->TxDescFree = txdescfree;

	txdesc->tdes1.ubit.TransmitBuffer2Size = 0;
	if(txdesc->tdes0.ubit.SecondAddressChained)
		txdesc->tdes3.NextDescriptorAddress = (u32)&priv->virtTxDescBaseAddr[txdescfree];
	else
		txdesc->tdes3.Buffer2AddressPointer = 0;	

	dbg_printk("tdes0: 0x%08x tdes1: 0x%08x tdes2: 0x%08x tdes3: 0x%08x\n",txdesc->tdes0.Status,txdesc->tdes1.ui,txdesc->tdes2.Buffer1AddressPointer,txdesc->tdes3.Buffer2AddressPointer) ;

	return txdescfree;	
}

static void moxaarm11_gmac_free_memory(struct net_device *ndev)
{
	int i;
	moxaarm11_gmac_priv_t		*priv=netdev_priv(ndev);

	if ( priv->virtTxDescBaseAddr )
		dma_free_coherent(NULL, sizeof(tx_desc_t)*TX_DESC_NUM, priv->virtTxDescBaseAddr, priv->phyTxDescBaseAddr);
	if ( priv->virtRxDescBaseAddr )
		dma_free_coherent(NULL, sizeof(rx_desc_t)*RX_DESC_NUM, priv->virtRxDescBaseAddr, priv->phyRxDescBaseAddr);
	
	for( i=0; i<RX_DESC_NUM; i++ ) {
		if ( priv->virtRxSkbBufBaseAddr[i] ) {
			dma_unmap_single(&priv->ndev->dev, priv->phyRxBufBaseAddr[i], RX_BUF_SIZE+2, DMA_BIDIRECTIONAL);
			kfree_skb(priv->virtRxSkbBufBaseAddr[i]);
		}
	}

#if 1	// added by Victor Yu. 2015-3-17
	for( i=0; i<TX_DESC_NUM; i++ ) {
		if ( priv->virtTxSkbBufBaseAddr[i] ) {
			dma_unmap_single(&priv->ndev->dev, priv->phyTxBufBaseAddr[i], priv->virtTxSkbBufBaseAddr[i]->len, DMA_BIDIRECTIONAL);
			kfree_skb(priv->virtTxSkbBufBaseAddr[i]);
		}
	}
#endif
}

void  moxaarm11_gmac_take_rx_desc_ownership(struct net_device *ndev)
{
	moxaarm11_gmac_priv_t		*priv=netdev_priv(ndev);
	s32 i;
	rx_desc_t *desc;
	desc = priv->virtRxDescBaseAddr;
	for(i = 0; i < RX_DESC_NUM; i++){
		desc->rdes0.ubit.OwnBit = 0;
		
		if(desc->rdes1.ubit.SecondAddressChained)	//This descriptor is in chain mode
			desc = (rx_desc_t *)desc->rdes3.NextDescriptorAddress;
		else
			desc += 1;		
	}
}

void  moxaarm11_gmac_take_tx_desc_ownership(struct net_device *ndev)
{
	moxaarm11_gmac_priv_t		*priv=netdev_priv(ndev);
	s32 i;
	tx_desc_t *desc;
	desc = priv->virtTxDescBaseAddr;
	
	for(i = 0; i < TX_DESC_NUM; i++){
		desc->tdes0.ubit.OwnBit = 0;
		
		if(desc->tdes0.ubit.SecondAddressChained)	//This descriptor is in chain mode
			desc = (tx_desc_t *)desc->tdes3.NextDescriptorAddress;
		else
			desc += 1;		
	}

}

int	moxaarm11_gmac_set_rx_desc(moxaarm11_gmac_priv_t * priv)
{
	int		RxDescFree=priv->RxDescFree;
	rx_desc_t	*rxdesc=&priv->virtRxDescBaseAddr[RxDescFree];

	rxdesc->rdes1.ubit.ReceiveBuffer1Size = RX_BUF_SIZE;
	rxdesc->rdes2.Buffer1AddressPointer = priv->phyRxBufBaseAddr[RxDescFree];

#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
	rxdesc->rdes4.Extstatus = 0;
	rxdesc->rdes5.Reserved = 0;	
	rxdesc->rdes6.ReceiveFrameTimestampLow = 0;
	rxdesc->rdes7.ReceiveFrameTimestampHigh = 0;
#endif
	rxdesc->rdes1.ubit.DisableInterruptonCompletion = 0;		
	rxdesc->rdes0.ubit.OwnBit = 1;

	dbg_printk("RxDescFree: %02d rxdesc: 0x%08x rdes0: 0x%08x\n", RxDescFree,(u32)rxdesc, rxdesc->rdes0.Status);
#if 0
	priv->RxDescFree     =  rxdesc->rdes1.ubit.ReceiveEndofRing? 0 : RxDescFree + 1;
#else
	RxDescFree++;
	RxDescFree &= RX_DESC_NUM_MASK;
	priv->RxDescFree = RxDescFree;
#endif

	rxdesc->rdes1.ubit.ReceiveBuffer2Size = 0;
	if(rxdesc->rdes1.ubit.SecondAddressChained)
		rxdesc->rdes3.NextDescriptorAddress = (u32)&priv->virtRxDescBaseAddr[priv->RxDescFree];
	else
		rxdesc->rdes3.Buffer2AddressPointer = 0;
	
	return RxDescFree;
}

static int moxaarm11_gmac_handle_received_data(struct net_device *ndev)
{
	moxaarm11_gmac_priv_t	*priv;
	int			RxDescFree, len;
	rx_desc_t		*rxdesc;
	unsigned char		*data;
	int			rx_count=0;
#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
	u16			time_stamp_higher;
#endif
	struct sk_buff		*skb; //This is the pointer to hold the received data
	
	dbg_printk("%s\n",__FUNCTION__);	
	priv = netdev_priv(ndev);
	if(priv == NULL){
		dbg_printk("Net device private data null\n");
		return rx_count;
	}

#if 1
	RxDescFree = priv->RxDescFree;
#endif
	/*Handle the Receive Descriptors*/
	do {
		rxdesc = (rx_desc_t *)&priv->virtRxDescBaseAddr[RxDescFree];
		if(rxdesc->rdes0.ubit.OwnBit != 0)
			break;	
		if(rxdesc->rdes0.ubit.DestinationAddressFilterFail != 0){
			err_printk("Failed to pass destination address filter!!\n");
			break;			
		}
		if(rxdesc->rdes1.ubit.ReceiveBuffer1Size == 0)
			break;

		if(rxdesc->rdes0.ubit.ErrorSummary) {
			err_printk("Error seen in RxDesc: %02d rxdesc: 0x%08x rdes0.status: 0x%08x\n",RxDescFree,(u32)rxdesc,rxdesc->rdes0.Status);
			if(rxdesc->rdes0.ubit.CrcError)
				dbg_printk("GMAC RX CRC Error! \n");

			if(rxdesc->rdes0.ubit.ReceiveError)
				err_printk("GMAC GMII RX Error! \n");

			if(rxdesc->rdes0.ubit.ReceiveWatchdogTimeout)
				err_printk("GMAC RX Watchdog Timeout Error! \n");

			if(rxdesc->rdes0.ubit.LateCollision)
				dbg_printk("GMAC RX Late Collision Error! \n");

			if(rxdesc->rdes0.ubit.IPcChecksumError)
#ifdef MOXAMACRO_GMAC_IPC_OFFLOAD
				dbg_printk("GMAC RX IPC Checksum Error! \n");
#else
				dbg_printk("GMAC RX Giant Frame Error! \n");
#endif

			if(rxdesc->rdes0.ubit.OverflowError)
				dbg_printk("GMAC RX Buffer Overflow in MTL Error! \n");

			if(rxdesc->rdes0.ubit.DescriptorError)
				err_printk("GMAC RX Descriptor Error! \n");
		}

		dbg_printk("RxDescFree: %02d rxdesc: 0x%08x rdes0: 0x%08x\n",RxDescFree,(u32)rxdesc,rxdesc->rdes0.Status);
		dbg_printk("Received Data at Rx Descriptor %d for skb 0x%08x whose status is %08x\n",RxDescFree,(u32)priv->virtRxSkbBufBaseAddr[RxDescFree]->data,rxdesc->rdes0.Status);

		skb = (struct sk_buff *)priv->virtRxSkbBufBaseAddr[RxDescFree];
		/*At first step unmapped the dma address*/
		dma_unmap_single(&priv->ndev->dev, priv->phyRxBufBaseAddr[RxDescFree], RX_BUF_SIZE+2, DMA_BIDIRECTIONAL);

		if(skb == NULL || skb->head == NULL)
			printk("RX SKB is NULL\n");
		if(rxdesc->rdes0.ubit.ErrorSummary == 0 && rxdesc->rdes0.ubit.FirstDescriptor == 1 && rxdesc->rdes0.ubit.LastDescriptor == 1) {
			len =  rxdesc->rdes0.ubit.FrameLength;
			
			data=skb_put(skb,len);
			data = skb_tail_pointer(skb)- len;
			dbg_printk("receive data pointer = 0x%x len: %d data:", (u32)data, len);

#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE			
			if (rxdesc->rdes4.ubit.MessageType == syncMessage || rxdesc->rdes4.ubit.MessageType == delayReqMessage
			|| rxdesc->rdes4.ubit.MessageType == pdelayReqMessage	|| rxdesc->rdes4.ubit.MessageType == pdelayRespMessage) {
				time_stamp_higher = readl((void *)(ndev->base_addr + GMAC_HIGHER_SECOND));
			
	#ifdef MOXAMACRO_GMAC_IPC_OFFLOAD			
				if(rxdesc->rdes4.ubit.IPv4PacketReceived == 1) 
	#else
				if(data[ETH_PKT_TYPE_OFFSET]<<8 | data[ETH_PKT_TYPE_OFFSET+1] == ETH_PKT_TYPE_IPV4) 
	#endif				
					priv->rxTimestamp[priv->rxTimestampWriteIndex].seq = data[IPV4_PKT_PTP_SN_OFFSET]<<8 | data[IPV4_PKT_PTP_SN_OFFSET+1];
	#ifdef MOXAMACRO_GMAC_IPC_OFFLOAD				
				else if(rxdesc->rdes4.ubit.IPv6PacketReceived)
	#else				
				else if(data[ETH_PKT_TYPE_OFFSET]<<8 | data[ETH_PKT_TYPE_OFFSET+1] == ETH_PKT_TYPE_IPV6)
	#endif			
					priv->rxTimestamp[priv->rxTimestampWriteIndex].seq = data[IPV6_PKT_PTP_SN_OFFSET]<<8 | data[IPV6_PKT_PTP_SN_OFFSET+1];
				else if((data[ETH_PKT_TYPE_OFFSET]<<8 | data[ETH_PKT_TYPE_OFFSET+1]) == ETH_PKT_TYPE_PTP) 
					priv->rxTimestamp[priv->rxTimestampWriteIndex].seq = data[ETH_PKT_PTP_SN_OFFSET]<<8 | data[ETH_PKT_PTP_SN_OFFSET+1];
				else
					priv->rxTimestamp[priv->rxTimestampWriteIndex].seq = 0;
			
				dbg_printk("priv->rxTimestampWriteIndex:%d  sid: %d\n",priv->rxTimestampWriteIndex,priv->rxTimestamp[priv->rxTimestampWriteIndex].seq);
				dbg_printk("rdes4: 0x%08x rdes6: 0x%08x rdes7: 0x%08x \n", rxdesc->rdes4.Extstatus, rxdesc->rdes6.ReceiveFrameTimestampLow,  rxdesc->rdes7.ReceiveFrameTimestampHigh);
			
				priv->rxTimestamp[priv->rxTimestampWriteIndex].higherSeconds= time_stamp_higher;
				priv->rxTimestamp[priv->rxTimestampWriteIndex].seconds= rxdesc->rdes7.ReceiveFrameTimestampHigh;
				priv->rxTimestamp[priv->rxTimestampWriteIndex].subseconds= rxdesc->rdes6.ReceiveFrameTimestampLow;
				priv->rxTimestamp[priv->rxTimestampWriteIndex].valid = 1;
				priv->rxTimestampWriteIndex= (priv->rxTimestampWriteIndex+1 >= RX_TIMESTAMP_BUF_SIZE)? 0 : priv->rxTimestampWriteIndex+1;
			}
		
#endif	
			skb->dev = ndev;
			skb->protocol = eth_type_trans(skb, ndev);
#ifndef CONFIG_MOXAMACRO_GMAC_NAPI			
			if(netif_rx(skb) == NET_RX_DROP)
#else
			if(netif_receive_skb(skb) == NET_RX_DROP)
#endif
				dbg_printk("netif RX error! \r\n");

			ndev->last_rx = jiffies;
			priv->stats.rx_packets++;
			priv->stats.rx_bytes += len;		
			rx_count++;
		} else {
			err_printk( "RXDesc Error with status: %08x\n",rxdesc->rdes0.Status);
			dev_kfree_skb_irq(skb);
			priv->stats.rx_errors++;
			priv->stats.collisions       += rxdesc->rdes0.ubit.LateCollision;
			priv->stats.rx_crc_errors    += rxdesc->rdes0.ubit.CrcError;
			priv->stats.rx_frame_errors  += rxdesc->rdes0.ubit.DribbleBitError;
			priv->stats.rx_length_errors += rxdesc->rdes0.ubit.LengthError;
		}
			
		//Now lets allocate the skb for the emptied descriptor
		priv->virtRxSkbBufBaseAddr[RxDescFree] = dev_alloc_skb(RX_BUF_SIZE + 2);
		if(priv->virtRxSkbBufBaseAddr[RxDescFree] == NULL){
			dbg_printk("SKB memory allocation failed \n");
			priv->stats.rx_dropped++;
		}
		priv->phyRxBufBaseAddr[RxDescFree] = dma_map_single(&priv->ndev->dev, (void *)priv->virtRxSkbBufBaseAddr[RxDescFree]->data, RX_BUF_SIZE+2, DMA_BIDIRECTIONAL);
		RxDescFree = moxaarm11_gmac_set_rx_desc(priv);
	} while(RxDescFree >= 0);

	return rx_count;
}


static int moxaarm11_gmac_handle_transmit_over(struct net_device *ndev)
{
	moxaarm11_gmac_priv_t	*priv;
	int			TxDescBusy, tx_count=0;
	tx_desc_t		*txdesc;
	
#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
	u16 time_stamp_higher;
	u8 *tx_buf;
#endif
	priv = netdev_priv(ndev);
	
	/*Handle the transmit Descriptors*/
	do {
		TxDescBusy = priv->TxDescBusy;
		txdesc = (tx_desc_t* )&priv->virtTxDescBaseAddr[TxDescBusy];
		dbg_printk("%s: line: %d :%d\n",__FUNCTION__,__LINE__,TxDescBusy);	
		if(txdesc->tdes0.ubit.OwnBit != 0)
			break;
		dbg_printk("%s: line: %d\n",__FUNCTION__,__LINE__);	
		
		if(txdesc->tdes1.ubit.TransmitBuffer1Size == 0)
			break;

		if(unlikely(txdesc->tdes0.ubit.ErrorSummary))
			err_printk("Error seen in TX Desc:%02d addr: %08x tdes0:%08x tdes1:%08x tdes2:%08x tdes3:%08x\n",TxDescBusy,(u32)txdesc,txdesc->tdes0.Status,txdesc->tdes1.ubit.TransmitBuffer1Size,txdesc->tdes2.Buffer1AddressPointer, txdesc->tdes3.Buffer2AddressPointer);		


		dbg_printk("(get)%02d addr: %08x tdes0:%08x tdes1:%08x tdes2:%08x tdes3:%08x\n",TxDescBusy,(u32)txdesc,txdesc->tdes0.Status,txdesc->tdes1.ubit.TransmitBuffer1Size,txdesc->tdes2.Buffer1AddressPointer, txdesc->tdes3.Buffer2AddressPointer);		
	
	//moxaarm11_gmac_dma_tx_disable(ndev);		
#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
		time_stamp_higher = readl((void *)(ndev->base_addr + GMAC_HIGHER_SECOND));
		tx_buf = priv->virtTxSkbBufBaseAddr[TxDescBusy]->data;//priv->virtTxBufBaseAddr + TxDescBusy*TX_BUF_SIZE;
	#ifdef CONFIG_MOXAMACRO_GMAC_DEBUG	
		{
		int	i;
		dbg_printk("transmitted data pointer = 0x%x : ", (u32)tx_buf);	
		for (i=0; i<priv->virtTxSkbBufBaseAddr[TxDescBusy]->len; i++)
			dbg_printk("%x ",tx_buf[i]);
		dbg_printk("\n");
		}
	#endif

		if( (tx_buf[ETH_PKT_TYPE_OFFSET]<<8 | tx_buf[ETH_PKT_TYPE_OFFSET+1] ) == ETH_PKT_TYPE_IPV4) //IPv4
			priv->txTimestamp[priv->txTimestampWriteIndex].seq =   ((u16)tx_buf[IPV4_PKT_PTP_SN_OFFSET])<<8 | (u16)tx_buf[IPV4_PKT_PTP_SN_OFFSET+1];
		else if( (tx_buf[ETH_PKT_TYPE_OFFSET]<<8 | tx_buf[ETH_PKT_TYPE_OFFSET+1] ) == ETH_PKT_TYPE_IPV6) //IPv6
			priv->txTimestamp[priv->txTimestampWriteIndex].seq =   ((u16)tx_buf[IPV6_PKT_PTP_SN_OFFSET])<<8 | (u16)tx_buf[IPV6_PKT_PTP_SN_OFFSET+1];	
		else if( (tx_buf[ETH_PKT_TYPE_OFFSET]<<8 | tx_buf[ETH_PKT_TYPE_OFFSET+1] ) == ETH_PKT_TYPE_PTP) //ETH
			priv->txTimestamp[priv->txTimestampWriteIndex].seq =   ((u16)tx_buf[ETH_PKT_PTP_SN_OFFSET])<<8 | (u16)tx_buf[ETH_PKT_PTP_SN_OFFSET+1];
		else
			priv->txTimestamp[priv->txTimestampWriteIndex].seq = 0;

		dbg_printk("txTimestampWriteIndex:%d sid: %d %d %d",priv->txTimestampWriteIndex, priv->txTimestamp[priv->txTimestampWriteIndex].seq ,tx_buf[IPV4_PKT_PTP_SN_OFFSET], tx_buf[IPV4_PKT_PTP_SN_OFFSET+1]);
		dbg_printk("tdes4:%08x tdes5:%08x tdes6:%08x tdes7:%08x\n ",txdesc->tdes4.Reserved, txdesc->tdes5.Reserved, txdesc->tdes6.TransmitFrameTimestampLow, txdesc->tdes7.TransmitFrameTimestampHigh);

		priv->txTimestamp[priv->txTimestampWriteIndex].higherSeconds= time_stamp_higher;
		priv->txTimestamp[priv->txTimestampWriteIndex].seconds= txdesc->tdes7.TransmitFrameTimestampHigh;
		priv->txTimestamp[priv->txTimestampWriteIndex].subseconds= txdesc->tdes6.TransmitFrameTimestampLow;
		priv->txTimestamp[priv->txTimestampWriteIndex].valid = 1;
		priv->txTimestampWriteIndex= (priv->txTimestampWriteIndex+1 >= TX_TIMESTAMP_BUF_SIZE)? 0 : priv->txTimestampWriteIndex+1;
	
#endif
		priv->TxDescBusy     = txdesc->tdes0.ubit.TransmitEndofRing? 0 : TxDescBusy + 1;

		dbg_printk("Finished Transmit at Tx Descriptor %d for buffer virtual addr  0x%08x and buffer phys = %08x whose status is %08x \n", TxDescBusy,(u32)__phys_to_virt(txdesc->tdes2.Buffer1AddressPointer),txdesc->tdes2.Buffer1AddressPointer,txdesc->tdes0.Status);

		if(txdesc->tdes0.ubit.ErrorSummary == 0 && txdesc->tdes0.ubit.FirstSegment == 1 && txdesc->tdes0.ubit.LastSegment){
			priv->stats.tx_bytes += txdesc->tdes1.ubit.TransmitBuffer1Size;
			priv->stats.tx_packets++;
		} else {	
			dbg_printk("Error in Status %08x\n",txdesc->tdes0.Status);
			priv->stats.tx_errors++;
			priv->stats.tx_aborted_errors += txdesc->tdes0.ubit.ExcessiveCollision | txdesc->tdes0.ubit.LateCollision;
			priv->stats.tx_carrier_errors += txdesc->tdes0.ubit.NoCarrier | txdesc->tdes0.ubit.LossofCarrier;
		}
		priv->stats.collisions += txdesc->tdes0.ubit.CollisionCount;
		txdesc->tdes1.ubit.TransmitBuffer1Size = 0;

		dma_unmap_single(&priv->ndev->dev, priv->phyTxBufBaseAddr[TxDescBusy], priv->virtTxSkbBufBaseAddr[TxDescBusy]->len, DMA_BIDIRECTIONAL);
		dev_kfree_skb_any(priv->virtTxSkbBufBaseAddr[TxDescBusy]);
		priv->virtTxSkbBufBaseAddr[TxDescBusy] = NULL;
		tx_count++;
		priv->TxPending = (priv->TxDescBusy <= priv->TxDescFree)? 
			(priv->TxDescFree - priv->TxDescBusy):(RX_DESC_NUM - priv->TxDescBusy + priv->TxDescFree);		
	} while(true);

	return tx_count;
}

void moxaarm11_gmac_clear_interrupt(struct net_device *ndev)
{
	u32	reg=ndev->base_addr+GMAC_DMA_STATUS;
	writel( readl((void *)reg), (void *)reg);	//write 1 clear;
}

void moxaarm11_gmac_dma_interrupt_init(struct net_device *ndev, u32 interrupts)
{
	u32	reg=ndev->base_addr+GMAC_DMA_INT_EN;
	writel(interrupts, (void *)reg);
}

void moxaarm11_gmac_dma_interrupt_enable(struct net_device *ndev, u32 interrupts)
{
	u32	reg=ndev->base_addr+GMAC_DMA_INT_EN;
	writel( readl((void *)reg) | interrupts, (void *)reg);
}


void moxaarm11_gmac_dma_interrupt_disable(struct net_device *ndev, u32 interrupts)
{
	u32	reg=ndev->base_addr+GMAC_DMA_INT_EN;
	writel( readl((void *)reg) & ~interrupts, (void *)reg);
}

static int moxaarm11_gmac_init(struct net_device *ndev)
{
	moxaarm11_gmac_priv_t		*priv=netdev_priv(ndev);		
	int i;
	struct mtd_info			*mtd;

	dbg_printk("GMAC net device init \n");

	// first initialize the private variable to zero 
	memset((void *)priv, 0, sizeof(moxaarm11_gmac_priv_t));
	spin_lock_init(&priv->txlock);
	//spin_lock_init(&priv->rxlock);

	// allocate the descriptor and buffer memory
	priv->virtTxDescBaseAddr = (tx_desc_t *)dma_alloc_coherent(NULL, sizeof(tx_desc_t)*TX_DESC_NUM, (dma_addr_t *)&priv->phyTxDescBaseAddr, GFP_DMA|GFP_KERNEL);	
	if ( priv->virtTxDescBaseAddr == NULL || (priv->phyTxDescBaseAddr & 0x0f) ) {
		err_printk("Allocate the Tx descriptor memory fail !\n");
		goto init_fail;
	}
	dbg_printk("priv->virtTxDescBaseAddr: 0x%08x \n", (u32)priv->virtTxDescBaseAddr);
	
	priv->virtRxDescBaseAddr = (rx_desc_t *)dma_alloc_coherent(NULL, sizeof(rx_desc_t)*RX_DESC_NUM, (dma_addr_t *)&priv->phyRxDescBaseAddr, GFP_DMA|GFP_KERNEL);
	if ( priv->virtRxDescBaseAddr == NULL || (priv->phyRxDescBaseAddr & 0x0f) ) {
		err_printk("Allocate the Rx descriptor memory fail !\n");
		goto init_fail;
	}
	dbg_printk("priv->virtRxDescBaseAddr: 0x%08x \n", (u32)priv->virtRxDescBaseAddr);

	/*Key Point: We only need to prepare RX SKB, TX SKB should be allocated by the kernel network module*/
	for( i=0; i<RX_DESC_NUM; i++ ) {
		priv->virtRxSkbBufBaseAddr[i]=dev_alloc_skb(RX_BUF_SIZE+2);		
		if ( priv->virtRxSkbBufBaseAddr[i] == NULL ) {
			err_printk("Allocate the Rx buffer memory fail !\n");
			goto init_fail;
		}
		priv->phyRxBufBaseAddr[i]= dma_map_single(&priv->ndev->dev, (void *)priv->virtRxSkbBufBaseAddr[i]->data, RX_BUF_SIZE+2, DMA_BIDIRECTIONAL);
		/*
		if ( priv->virtRxSkbBufBaseAddr[i] == NULL || (priv->phyRxBufBaseAddr[i] & 0x03) ) {
			err_printk("Allocate the Rx buffer memory fail !\n");
			goto init_fail;
		}
		*/
	}
	for ( i=0; i<TX_DESC_NUM; i++ ) 
		priv->virtTxSkbBufBaseAddr[i] = NULL;

	priv->mii_if.dev = ndev;
	priv->mii_if.mdio_read = moxaarm11_gmac_phy_read_reg;
	priv->mii_if.mdio_write = moxaarm11_gmac_phy_write_reg;
	priv->mii_if.phy_id_mask = 0x1f;
	priv->mii_if.reg_num_mask = 0x1f;
	priv->ndev = ndev;
	priv->netStop = 1;
	ether_setup(ndev);

	/* Key Point: The MAC H/W address should be written in some specified address in the  
	   NOR flash via the U-Boot and assign with MOXA vender code 00-90-E8.*/
	// first set the default value
	ndev->dev_addr[0] = 0x00;
	ndev->dev_addr[1] = 0x90;
	ndev->dev_addr[2] = 0xE8;
	ndev->dev_addr[3] = 0x72;
	ndev->dev_addr[4] = 0x20;
	ndev->dev_addr[5] = 0x01;
	mtd = get_mtd_device(NULL, 0);
	if ( mtd ) {	// open OK
		int	len;

		mtd->read(mtd, 0xE0000, 6, &len, ndev->dev_addr);

		if ( len != 6 || ndev->dev_addr[0] != 0x00 || ndev->dev_addr[1] != 0x90 || ndev->dev_addr[2] != 0xe8 ) {
			ndev->dev_addr[0] = 0x00;
			ndev->dev_addr[1] = 0x90;
			ndev->dev_addr[2] = 0xE8;
			ndev->dev_addr[3] = 0x72;
			ndev->dev_addr[4] = 0x20;
			ndev->dev_addr[5] = 0x01;
		}
	}
	ndev->dev_addr[5] += (u8)ndev->name[3] - (u8)'0';

	// reset the MAC
	moxaarm11_gmac_reset(ndev);	

	return 0;

init_fail:
	moxaarm11_gmac_free_memory(ndev);
	return -ENOMEM;

}

static void moxaarm11_gmac_control_overall_init(struct net_device *ndev)
{
	u32	val;

	//val = *(unsigned volatile int *)(MOXAARM11_VA_SYSTEM_BASE+MOXAARM11_SYSTEM_IO_CONTORL);
	val = readl(MOXAARM11_VA_SYSTEM_BASE+MOXAARM11_SYSTEM_IO_CONTORL);
	val |= 0xF<<6;
	//*(unsigned volatile int *)(MOXAARM11_VA_SYSTEM_BASE+MOXAARM11_SYSTEM_IO_CONTORL) = val;
	writel(val, MOXAARM11_VA_SYSTEM_BASE+MOXAARM11_SYSTEM_IO_CONTORL);

#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
	moxaarm11_gmac_dma_bus_mode_init(ndev,	AddrAlignedBeats | PBLModex8|RxBurstLength32 |  BurstLength32 |  AlternateDescSize|FixedBurst);
	moxaarm11_gmac_timestamp_enable(ndev);
	moxaarm11_gmac_timestamp_fine_update(ndev);		
	moxaarm11_gmac_timestamp_digital_rollover(ndev);	
	moxaarm11_gmac_timestamp_addend_update(ndev, 0xCEDE6243);	  //0x85555555
	moxaarm11_gmac_timestamp_subsecond_increment_init(ndev, 25);	//set as 20 nanosecond
	moxaarm11_gmac_timestamp_highersecond_write(ndev, 0);		
	moxaarm11_gmac_timestamp_time_init(ndev, 0, 0);

	moxaarm11_gmac_timestamp_set_clock_type(ndev, TimestampOrdinaryClock);
	//moxaarm11_gmac_timestamp_event_message_enable(ndev);
	moxaarm11_gmac_timestamp_ipv4_enable(ndev);
	moxaarm11_gmac_timestamp_ptpv2(ndev);
	moxaarm11_gmac_timestamp_all_frames_enable(ndev);

	moxaarm11_gmac_timestamp_interrupt_enable(ndev);	

	priv->rxTimestampWriteIndex=0;
	priv->rxTimestampReadIndex=0;		
	priv->txTimestampWriteIndex=0;
	priv->txTimestampReadIndex=0;
#else
	moxaarm11_gmac_timestamp_disable(ndev);
	moxaarm11_gmac_timestamp_interrupt_disable(ndev);
	moxaarm11_gmac_dma_bus_mode_init(ndev,	AddrAlignedBeats | PBLModex8|RxBurstLength32 |  BurstLength32 | FixedBurst);
#endif

	/*Key Point: the setting of the AXI MAX. request limit should be according to the actual number of AXI channels, or AXI frames may be lost.*/
	//moxaarm11_gmac_dma_axi_mode_init(ndev, AXIMaxReadReqLimit01 | AXIMaxWriteReqLimit01 | AXIUndefinedBurstLength);

	moxaarm11_gmac_dma_axi_mode_init(ndev, AXIMaxReadReqLimit01 | AXIMaxWriteReqLimit01 | AXIBurstLength4 | AXIBurstLength8 | AXIBurstLength16 | AXIBurstLength32 | AXIBurstLength64 | AXIBurstLength128 | AXIBurstLength256); 

	//for test only 20110809
	//moxaarm11_gmac_dma_axi_mode_init(ndev, AXIMaxReadReqLimit01 | AXIMaxWriteReqLimit01 | AXIBurstLength4 | AXIBurstLength8 | AXIBurstLength16 | AXIBurstLength32 ); 

	/*GMAC Config */
	dbg_printk("GMAC Configuration...\n");
	moxaarm11_gmac_watchdog_enable(ndev);
	moxaarm11_gmac_jab_enable(ndev);
	//moxaarm11_gmac_frame_burst_enable(ndev);
	moxaarm11_gmac_jumbo_frame_disable(ndev);
	moxaarm11_gmac_rx_own_enable(ndev);
#ifdef MOXAMACRO_GMAC_IPC_OFFLOAD 
	moxaarm11_gmac_rx_checksum_offload_enable(ndev);
	//ndev->features = NETIF_F_HW_CSUM;
#endif	
	/*GMAC loopback Configuration*/
	moxaarm11_gmac_loopback_disable(ndev);

#ifdef CONFIG_MOXAMACRO_DDR2_SDRAM
	/*PHY Configuration*/
	/*Key Point: Don't force the speed and mode of PHY, or others can't detect the duplex mode of this port. 
	    the auto-advertisment should always be enabled.*/
	//printk("PHY Configuration ID=%d...\n", priv->phyid);	    
	moxaarm11_gmac_phy_write_reg(ndev, priv->phyid,  MII_BMCR, BMCR_RESET);	
	moxaarm11_gmac_phy_write_reg(ndev, priv->phyid,  MII_ADVERTISE, ADVERTISE_100FULL|ADVERTISE_CSMA);	//phy for 100M	FD
	moxaarm11_gmac_phy_write_reg(ndev, priv->phyid,  MII_BMCR, BMCR_ANENABLE | BMCR_ANRESTART);

	/*Key Point: To let GMAC able to detect PHY link status, we need to turn the RGMII inband frame of the BCM5461 Ethernet PHY chip.*/
	moxaarm11_gmac_phy_write_reg(ndev, priv->phyid,  BCM5461_MISC_CONTROL_REG, BCM5461_MISC_CONTROL_REG_READ_SELECT| BCM5461_MISC_CONTROL_REG_WRITE_SELECT);
	val = moxaarm11_gmac_phy_read_reg(ndev, priv->phyid,  BCM5461_MISC_CONTROL_REG);
	val &= ~ BCM5461_MISC_CONTROL_LINK_STATUS_DISABLE;
	moxaarm11_gmac_phy_write_reg(ndev, priv->phyid,  BCM5461_MISC_CONTROL_REG, BCM5461_MISC_CONTROL_REG_WRITE_ENABLE | val | BCM5461_MISC_CONTROL_REG_WRITE_SELECT);
#endif	
	/*GMAC Bus Configuration*/
	moxaarm11_gmac_full_duplex(ndev);		//or half duplex, we should follow the settings of PHY
	moxaarm11_gmac_retry_enable(ndev);
	moxaarm11_gmac_pad_crc_strip_disable(ndev);
	moxaarm11_gmac_tx_enable(ndev);
	moxaarm11_gmac_rx_enable(ndev);

	moxaarm11_gmac_select_mii(ndev);		//or gmii 

	moxaarm11_gmac_set_speed_100M(ndev); //we should follow the settings of PHY

	/*Frame Filter Configuration*/
	dbg_printk("Frame Filter Configuration...\n");	
	moxaarm11_gmac_frame_filter_enable(ndev);
	//moxaarm11_gmac_hash_perfect_filter_enable(ndev);
	moxaarm11_gmac_set_pass_control(ndev,PassControlFramesNone);
	moxaarm11_gmac_broadcast_enable(ndev);
	moxaarm11_gmac_src_addr_filter_disable(ndev);
	moxaarm11_gmac_pass_all_multicast_enable(ndev);
	moxaarm11_gmac_dst_addr_filter_normal(ndev);
	moxaarm11_gmac_multicast_hash_filter_disable(ndev);
	moxaarm11_gmac_promisc_disable(ndev);
	moxaarm11_gmac_unicast_hash_filter_disable(ndev);

	/*DMA Control Configuration*/
	moxaarm11_gmac_dma_operation_mode_init(ndev, TransmitStoreForward |ReceiveStoreForward 
	| HWFlowControl |ForwardUndersizedFrames | ForwardErrorFrames | Operate2ndFrame);
	
	/*Flow Control Configuration*/
	dbg_printk("Flow Control Configuration...\n");		
	//moxaarm11_gmac_unicast_pause_frame_detect_disable(ndev);
	moxaarm11_gmac_rx_flow_control_enable(ndev);
	moxaarm11_gmac_tx_flow_control_enable(ndev);
	moxaarm11_gmac_pause_control(ndev); // This enables the pause control in Full duplex mode of operation

	dbg_printk("Disable all MMC Interrupts...\n");		
	moxaarm11_gmac_mmc_interrupt_disable_all(ndev);


}


static irqreturn_t moxaarm11_gmac_interrupt_handler(int irq, void *dev_id/*, struct pt_regs *regs*/);
static int moxaarm11_gmac_open(struct net_device *ndev)
{
	moxaarm11_gmac_priv_t		*priv;
	unsigned long		flags;

	dbg_printk("%s:\n",__FUNCTION__);

	if(ndev == NULL){
		err_printk("%s: Unknown Device.\n", __FUNCTION__);
		return -1;
	}

	priv = netdev_priv(ndev);	
	if(priv == NULL){
		err_printk("%s: Adapter Structure Missing.\n", __FUNCTION__);
		return -1;
	}

	spin_lock_irqsave(&priv->txlock, flags);
	
	moxaarm11_gmac_reset(ndev);
	
	GMAC_power_down = 0;

#if 1 /* Jared, 2016-10-24, Fix the MAC number carry issue */
	if ( (strcmp(ndev->name, "eth1") == 0) && (ndev->dev_addr[5] == 0x00)  ) {

		ndev->dev_addr[5] = 0x00;

		if ( ndev->dev_addr[4] != 0xFF )
			ndev->dev_addr[4]++;
		else {
			ndev->dev_addr[4] = 0x00;
			
			if ( ndev->dev_addr[3] != 0xFF )
				ndev->dev_addr[3]++;
			else
				ndev->dev_addr[3] = 0x00;
		}	
	}
#endif

	moxaarm11_gmac_set_mac_address(ndev->base_addr+GMAC_ADDR0_HIGH, ndev->dev_addr);
	moxaarm11_gmac_setup_desc_ring(ndev);

	moxaarm11_gmac_control_overall_init(ndev);
	
	dbg_printk("Enable DMA Interrupts...\n");		
	moxaarm11_gmac_dma_interrupt_init(ndev, InitInterruptEnable);
	moxaarm11_gmac_dma_rx_enable(ndev);
	moxaarm11_gmac_resume_dma_rx(ndev);
	//Tx should be enabled after the own bit of the first Tx descriptor is set.

#ifdef CONFIG_MOXAMACRO_GMAC_NAPI
	napi_enable(&priv->rx_napi);
#endif

	moxaarm11_vic_set_intr_trigger(ndev->irq, vicc_level_activeHigh);
	if ( request_irq(ndev->irq, &moxaarm11_gmac_interrupt_handler, IRQF_SHARED, ndev->name, ndev) ) {
		err_printk("Request interrupt service fail !\n");
		goto init_fail;
	}

	moxaarm11_gmac_magic_packet_enable(ndev);
	//printk("priv->wakeup_irq =%d\n", priv->wakeup_irq);
	moxaarm11_vic_set_intr_trigger(priv->wakeup_irq, vicc_level_activeHigh);
	if ( request_irq(priv->wakeup_irq, &moxaarm11_gmac_pmt_interrupt_handler, IRQF_SHARED, ndev->name, ndev) ) {
		err_printk("Request interrupt service fail !\n");
		goto init_fail;
	}		
	
	spin_unlock_irqrestore(&priv->txlock, flags);
	
	netif_start_queue(ndev);
	priv->netStop = 0;

#ifdef MOXAMACRO_GMAC_DEBUG
	{
	unsigned char	macaddr[6];
	int		i;
	moxaarm11_gmac_get_mac_address(ndev->base_addr+GMAC_ADDR0_HIGH, macaddr);
	printk("Get MAC address = ");
	for ( i=0; i<6; i++ )
		dbg_printk("%02X ", macaddr[i]);
	dbg_printk("\n");
	}
#endif
	return 0;
init_fail:
	return -EBUSY;
}

static int moxaarm11_gmac_stop(struct net_device *ndev)
{
	moxaarm11_gmac_priv_t		*priv=netdev_priv(ndev);
	unsigned long flags;
	
	dbg_printk("%s:\n",__FUNCTION__);		
	dbg_printk("Disable DMA Interrupts...\n");		

	netif_stop_queue(ndev);
	free_irq(ndev->irq, ndev);
	free_irq(priv->wakeup_irq, ndev);

	spin_lock_irqsave(&priv->txlock, flags);
	//spin_lock(&priv->rxlock);
	
	moxaarm11_gmac_dma_interrupt_disable(ndev, 0xFFFFFFFF); //disable all interrupts

#ifdef CONFIG_MOXAMACRO_GMAC_NAPI
	napi_disable(&priv->rx_napi);
#endif


	dbg_printk("Disable GMAC Rx...\n");			
	moxaarm11_gmac_dma_rx_disable(ndev);
	moxaarm11_gmac_take_rx_desc_ownership(ndev);

	dbg_printk("Disable GMAC Tx...\n");				
	moxaarm11_gmac_dma_tx_disable(ndev);
	moxaarm11_gmac_take_tx_desc_ownership(ndev);

	//spin_unlock(&priv->rxlock);
	spin_unlock_irqrestore(&priv->txlock, flags);

	priv->netStop = 1;
		
	return 0;
}


static int moxaarm11_gmac_start_xmit(struct sk_buff *skb, struct net_device *ndev)
{
	moxaarm11_gmac_priv_t	*priv=netdev_priv(ndev);
	unsigned long		flags;

	dbg_printk("%s: \n",__FUNCTION__);
	if(unlikely(skb == NULL)){
		err_printk("skb is NULL What happened to Linux Kernel? \n ");
		return -1;
	}
	dbg_printk("transmit data pointer = 0x%x\n", (u32)skb->data);

	if(unlikely(priv == NULL)){
		err_printk("priv is NULL. \n ");
		return -1;
	}

#ifndef CONFIG_MOXAMACRO_GMAC_TX_INT	
	moxaarm11_gmac_handle_transmit_over(ndev);

	/*Stop the network queue*/	
	if(unlikely(priv->TxPending > (TX_DESC_NUM - 10))){
		dbg_printk("netif_stop_queue priv->TxPending=%d\n", priv->TxPending);
		netif_stop_queue(ndev); 
		/*Key Point: enable TX interrupt to wake up.*/
		moxaarm11_gmac_dma_interrupt_enable(ndev, TransmitInterrupt);
	}
#else
	//netif_stop_queue(ndev); // masked by Victor Yu. 2015-3-17
#endif
	spin_lock_irqsave(&priv->txlock, flags);
	if ( moxaarm11_gmac_set_tx_desc_xmit(priv, skb) < 0) {
		dbg_printk("No Tx space to transmit the packet !\n");
		priv->stats.tx_dropped++;	
		spin_unlock_irqrestore(&priv->txlock, flags);
		dev_kfree_skb_any(skb);		
		netif_stop_queue(ndev); // added by Victor Yu. 2015-3-17
		return -EBUSY;
	}	
	

	/*Now force the DMA to start transmission*/	
	moxaarm11_gmac_dma_tx_enable(ndev);	
	moxaarm11_gmac_resume_dma_tx(ndev);
	ndev->trans_start = jiffies;
	spin_unlock_irqrestore(&priv->txlock, flags);

	return 0;

}

static int moxaarm11_gmac_ioctl(struct net_device *ndev, struct ifreq *rq, int cmd)
{
	moxaarm11_gmac_priv_t *priv = netdev_priv(ndev);
	struct mii_ioctl_data *mii_data=if_mii(rq);
	int rc = 0;
	unsigned long flags;
#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
	u32 *addend_value, *increment;
	moxaarm11_gamc_timestamp_t *rx_time, *tx_time, *curr_time, *update_time;	
#endif	
	
	//dbg_printk("%s:\n",__FUNCTION__);
	if (!netif_running(ndev))
		return -EINVAL;
	
	if (priv==NULL)
		return -EINVAL;

	if (mii_data==NULL)
		return -EINVAL;


	spin_lock_irqsave(&priv->txlock, flags);
	//spin_lock(&priv->rxlock);		

	switch(cmd) {
		case SIOCGMIIPHY:
		case SIOCGMIIREG:
 			rc = generic_mii_ioctl(&priv->mii_if, if_mii(rq), cmd, NULL);
			break;
		case SIOCSMIIREG:{
			/*Key Point: For RGMII comunication, once the link speed and mode of PHY change,
			                    the GMAC RGMII bus speed and mode also need to be changed.*/
			u16 val = mii_data->val_in;
			rc = generic_mii_ioctl(&priv->mii_if, if_mii(rq), cmd, NULL);			

			if (!capable(CAP_NET_ADMIN))
				return -EPERM;	
		
			if (mii_data->phy_id == priv->mii_if.phy_id) {
	                   switch(mii_data->reg_num) {
					case MII_BMCR:
						if(val & BMCR_ANRESTART)
							break;
						
						if (priv->mii_if.force_media && (val & BMCR_FULLDPLX))
							moxaarm11_gmac_full_duplex(ndev);
						else if (priv->mii_if.force_media && !(val & BMCR_FULLDPLX))
							moxaarm11_gmac_half_duplex(ndev);

						if (priv->mii_if.force_media && (val & BMCR_SPEED100))
							moxaarm11_gmac_set_speed_100M(ndev);
						else if (priv->mii_if.force_media && !(val & BMCR_SPEED100))
							moxaarm11_gmac_set_speed_10M(ndev);	                                
	                             break;
									
					case MII_ADVERTISE:				
	                                if (val & (ADVERTISE_10FULL | ADVERTISE_100FULL))
							moxaarm11_gmac_full_duplex(ndev);
						else if (val & (ADVERTISE_10HALF| ADVERTISE_100HALF))
							moxaarm11_gmac_half_duplex(ndev);

						if (val & (ADVERTISE_100FULL| ADVERTISE_100HALF))
							moxaarm11_gmac_set_speed_100M(ndev);
						else if (val & (ADVERTISE_10FULL| ADVERTISE_10HALF))
							moxaarm11_gmac_set_speed_10M(ndev);	                                						
					break;
	                   	}
                   	}

			break;
		}		
#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
		case PTP_GET_RX_TIMESTAMP_SYNC:
		case PTP_GET_RX_TIMESTAMP_DEL_REQ:
		case PTP_GET_RX_TIMESTAMP_PDEL_REQ:
		case PTP_GET_RX_TIMESTAMP_PDEL_RESP:
			rx_time = (moxaarm11_gamc_timestamp_t *)rq->ifr_data;
			moxaarm11_gmac_ptp_get_rx_timestamp(priv, rq, rx_time, cmd);	
		break;

		case PTP_GET_TX_TIMESTAMP:
			tx_time = (moxaarm11_gamc_timestamp_t *)rq->ifr_data;
			moxaarm11_gmac_ptp_get_tx_timestamp(priv, rq, tx_time);				
			dbg_printk("tx_time->valid=%d tx_time->seq=%d\n",tx_time->valid, tx_time->seq);
		break;

		case PTP_ADJ_FREQ:
			addend_value = (u32 *)rq->ifr_data; 
			moxaarm11_gmac_ptp_adj_freq(priv, addend_value);
		break;
			
		case PTP_GET_FREQ:
			addend_value = (u32 *)rq->ifr_data; 
			moxaarm11_gmac_ptp_get_freq(priv, addend_value);			
		break;

		case PTP_SET_PERIOD:
			increment = (u32 *)rq->ifr_data;
			moxaarm11_gmac_ptp_set_period(priv, increment);						
		break;

		case PTP_GET_PERIOD:
			increment = (u32 *)rq->ifr_data;
			moxaarm11_gmac_ptp_get_period(priv, increment);									
		break;

		case PTP_SET_TIME:
			curr_time = (moxaarm11_gamc_timestamp_t *)rq->ifr_data;
			moxaarm11_gmac_ptp_set_time(priv, curr_time);							
		break;

		case PTP_GET_TIME:
			curr_time = (moxaarm11_gamc_timestamp_t *)rq->ifr_data;
			moxaarm11_gmac_ptp_get_time(priv, curr_time);										
		break;

		case PTP_UPDATE_TIME:
			update_time = (moxaarm11_gamc_timestamp_t *)rq->ifr_data;
			moxaarm11_gmac_ptp_update_time(priv, update_time);													
		break;
		
#endif		
		case IOCTL_POWER_DOWN:
			moxaarm11_gmac_power_down(ndev);
		break;
		default:
			break;

	}

	//spin_unlock(&priv->rxlock);				
	spin_unlock_irqrestore(&priv->txlock, flags);
	return rc;
}

static irqreturn_t moxaarm11_gmac_interrupt_handler(int irq, void *dev_id/*, struct pt_regs *regs*/)
{
	/*Kernels passes the netdev structure in the dev_id. So grab it*/
	struct net_device	*ndev;
	moxaarm11_gmac_priv_t	*priv;
#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
	u32			snapshot_num, snapshot_order,aux_seconds, aux_nanoseconds,i;
#endif
	int			i;
	u32			dma_status_reg, status;

	ndev  = (struct net_device *) dev_id;
	if(ndev == NULL){
		err_printk("%s: Unknown Device.\n", __FUNCTION__);
		return IRQ_NONE;
	}

	priv = netdev_priv(ndev);	
	if(priv == NULL){
		err_printk("%s: Adapter Structure Missing.\n", __FUNCTION__);
		return IRQ_NONE;
	}

	dbg_printk("%s:ndev->base_addr: 0x%08x\n",__FUNCTION__, (u32)ndev->base_addr);

	dma_status_reg = readl((void *)(ndev->base_addr + GMAC_DMA_STATUS));

	if(dma_status_reg == 0) {
		err_printk("%s: Interrupt with Unknown Source.\n", __FUNCTION__);
		return IRQ_NONE;
	}

	/*u32 tmp = dma_status_reg & TransmitProcessState;
	if( tmp == (0x4<<20))
	{
		err_printk("%s:Dma Status Reg: 0x%08x 0x%08x\n",__FUNCTION__,dma_status_reg, tmp);
	}

	tmp = dma_status_reg & ErrorBits;
	if( tmp != 0)
	{
		err_printk("%s:Dma Status Reg: 0x%08x 0x%08x\n",__FUNCTION__,dma_status_reg, tmp);
	}*/
	
	dbg_printk("%s:Dma Status Reg: 0x%08x\n",__FUNCTION__,dma_status_reg);
	
	if(dma_status_reg & GMACPMTInterrupt){
		//printk("%s:: Interrupt due to PMT module\n",__FUNCTION__);

		status = readl((void *)(ndev->base_addr+GMAC_PMT_CS));
		if(status&MagicPacketReceived){
			//printk("GMAC wokeup due to Magic Pkt Received\n");
			moxaarm11_gmac_linux_powerup_mac(ndev);
		}
		if(status&WakeUpFrameEnable){
			//printk("GMAC wokeup due to Wakeup Frame Received\n");
			moxaarm11_gmac_linux_powerup_mac(ndev);
		}
	}

	/*if( moxaarm11_gmac_is_magic_packet_received(ndev))
		printk("GMAC wokeup due to Magic Pkt Received\n");
	if(moxaarm11_gmac_is_wakeup_frame_received(ndev))
		printk("GMAC wokeup due to Wakeup Frame Received\n");*/
	
	if(dma_status_reg & GMACMMCInterrupt){
		dbg_printk("%s:: Interrupt due to MMC module, ",__FUNCTION__);////
		status = readl((void *)(ndev->base_addr + GMAC_MMC_RX_INT));
		dbg_printk("RX status: 0x%08x ",status);		
		status = readl((void *)(ndev->base_addr + GMAC_MMC_TX_INT));		
		dbg_printk("TX status: 0x%08x\n",status);		
		//dbg_printk("%s:: gmac_rx_int_status = 0x%08x\n",__FUNCTION__,ndev->base_addr + GMAC_DMA_STATUS);
		//dbg_printk("%s:: gmac_tx_int_status = 0x%08x\n",__FUNCTION__,ndev->base_addr + GMAC_DMA_STATUS);
	}

	if(dma_status_reg & GMACLineinterfaceInterrupt){
		status = readl((void *)(ndev->base_addr + GMAC_RGMII_STATUS));
		dbg_printk("%s:: Interrupt due to GMAC LINE module with status: 0x%08x\n",__FUNCTION__, status);		
		//printk("\nLink Status Changed: ");

		if (status & LinkStatusUp){
			//printk("Link Up - ");
			if ((status & LinkSpeedMask) == LinkSpeed10M){
#ifdef CONFIG_MOXAMACRO_DDR2_SDRAM
					moxaarm11_gmac_select_mii(ndev);
#endif				
				//printk("10MbaseT-");
			} else if ((status & LinkSpeedMask) == LinkSpeed100M){
#ifdef CONFIG_MOXAMACRO_DDR2_SDRAM
				moxaarm11_gmac_select_mii(ndev);
#endif					
				//printk("100MbaseT-");
			} else if ((status & LinkSpeedMask) == LinkSpeed1000M){
#ifdef CONFIG_MOXAMACRO_DDR2_SDRAM
				moxaarm11_gmac_select_gmii(ndev);				
#endif
				//printk("1000Mbase");
			}

			/*
			if ((status & LinkModeFD) && ((status & LinkSpeedMask) != LinkSpeed1000M))
				printk("FD");
			else if ((status & LinkSpeedMask) != LinkSpeed1000M)
				printk("HD");
			printk("\n");
			*/
		} /* else
			printk("Link Down \n");
		*/

	}

	/*Now lets handle the DMA interrupts*/  
        if(dma_status_reg & FatalBusErrorInterrupt){
		err_printk("%s::Fatal Bus Error Inetrrupt Seen\n",__FUNCTION__);
		moxaarm11_gmac_dma_tx_disable(ndev);
		moxaarm11_gmac_dma_rx_disable(ndev);

		moxaarm11_gmac_reset(ndev);

		moxaarm11_gmac_take_tx_desc_ownership(ndev);
		moxaarm11_gmac_take_rx_desc_ownership(ndev);
		moxaarm11_gmac_init_tx_desc_base(ndev, priv->phyTxDescBaseAddr);
		moxaarm11_gmac_init_rx_desc_base(ndev, priv->phyRxDescBaseAddr);
		
		moxaarm11_gmac_set_mac_address(ndev->base_addr+GMAC_ADDR0_HIGH, ndev->dev_addr);

		moxaarm11_gmac_control_overall_init(ndev);

		moxaarm11_gmac_dma_rx_enable(ndev);
		moxaarm11_gmac_resume_dma_rx(ndev);

        }

	if(dma_status_reg & ReceiveInterrupt){
		dbg_printk("%s:: Rx Normal \n", __FUNCTION__);
#ifndef CONFIG_MOXAMACRO_GMAC_NAPI
		moxaarm11_gmac_handle_received_data(ndev);
#else		
		moxaarm11_gmac_dma_interrupt_disable(ndev, ReceiveInterrupt);
		napi_schedule(&priv->rx_napi);
#endif		
	}

	if(dma_status_reg & ReceiveBufferUnavailableInterrupt){
		err_printk("%s::Abnormal Rx Interrupt Seen\n",__FUNCTION__);
		err_printk("Dma Status Reg: 0x%08x\n",dma_status_reg);		
		err_printk("priv->RxDescFree=%d ",priv->RxDescFree);
		for(i=0;i<10;i++)
			err_printk("%d ",priv->virtRxDescBaseAddr[priv->RxDescFree+i].rdes0.ubit.OwnBit);
		
		if(GMAC_power_down == 0){	// If Mac is not in powerdown
			priv->stats.rx_over_errors++;
			moxaarm11_gmac_resume_dma_rx(ndev);
		}
	}


	if(dma_status_reg & ReceiveStoppedInterrupt){
		err_printk("%s::Receiver stopped seeing Rx interrupts\n",__FUNCTION__); //Receiver gone in to stopped state
		if(GMAC_power_down == 0){	// If Mac is not in powerdown
			priv->stats.rx_over_errors++;
			do {
				struct sk_buff *skb = dev_alloc_skb(RX_BUF_SIZE + 2);
				if(skb == NULL){
					dbg_printk("%s::ERROR in skb buffer allocation Better Luck Next time\n",__FUNCTION__);
					break;
				}
				priv->virtRxSkbBufBaseAddr[priv->RxDescFree] =  skb;
				priv->phyRxBufBaseAddr[priv->RxDescFree] = dma_map_single(&priv->ndev->dev, (void *)priv->virtRxSkbBufBaseAddr[priv->RxDescFree]->data, RX_BUF_SIZE+2, DMA_BIDIRECTIONAL);
				status = moxaarm11_gmac_set_rx_desc(priv);		
				dbg_printk("%s::Set Rx Descriptor no %08x for skb %08x \n",__FUNCTION__,status,(u32)skb);
				if(status < 0)
					dev_kfree_skb_irq(skb);//changed from dev_free_skb. If problem check this again
		
			}while(status >= 0);
		
			moxaarm11_gmac_dma_rx_enable(ndev);
			moxaarm11_gmac_resume_dma_rx(ndev);		
		}
	}

	if(dma_status_reg & TransmitInterrupt){
		//xmit function has done its job
		dbg_printk("%s::Finished Normal Transmission \n",__FUNCTION__);
#ifdef CONFIG_MOXAMACRO_GMAC_TX_INT		
		moxaarm11_gmac_handle_transmit_over(ndev);//Do whatever you want after the transmission is over	
		/*Now start the netdev queue*/
		netif_wake_queue(ndev);

#else		
		if((netif_queue_stopped(ndev) == 1)){
			dbg_printk("INT when queue_stopped\n");
			moxaarm11_gmac_handle_transmit_over(ndev);//Do whatever you want after the transmission is over	
		}
		if(priv->TxPending <= (TX_DESC_NUM - 15)){
			moxaarm11_gmac_dma_interrupt_disable(ndev, TransmitInterrupt);
			/*Now start the netdev queue*/
			dbg_printk("netif_wake_queue\n");			
			netif_wake_queue(ndev);
		}
#endif		



	}

        if(dma_status_reg & TransmitBufferUnavailableInterrupt){
		dbg_printk("%s::Tx Buffer Unavailable Interrupt\n",__FUNCTION__);////
		dbg_printk("%s:Dma Status Reg: 0x%08x\n",__FUNCTION__,dma_status_reg);	
	      // if(GMAC_power_down == 0){	// If Mac is not in powerdown
              //  moxaarm11_gmac_handle_transmit_over(ndev);
		//}
	}



	if(dma_status_reg & TransmitStoppedInterrupt){
		dbg_printk("%s::Transmitter stopped sending the packets\n",__FUNCTION__);////
	      /* if(GMAC_power_down == 0){	// If Mac is not in powerdown
		moxaarm11_gmac_dma_tx_disable(ndev);
                moxaarm11_gmac_take_tx_desc_ownership(ndev);
		
		moxaarm11_gmac_dma_tx_enable(ndev);
		netif_wake_queue(ndev);
		dbg_printk("%s::Transmission Resumed\n",__FUNCTION__);
		}*/
	}

#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
	if(dma_status_reg & TimeStampTriggerInterrupt){
		//printk("%s::Time stamp trigger\n",__FUNCTION__);
		status = readl((void *)(ndev->base_addr + GMAC_TIMESTAMP_STATUS));
		//printk("Timestamp status: 0x%x\n",status);
		/*
		if(status & TimestampSecondsOverflow)
			printk("Timestamp seconds overflow!!\n");

		if(status & TimestampTargetTimeReached)
			printk("Timestamp targetTime reached!!\n");
		*/

		if(status & AuxiliaryTimstampTriggerSnapshot){
			snapshot_num= (status & AuxiliaryTimestampNumberOfSnapshots)>>AuxiliaryTimestampNumberOfSnapshotsShift;
			snapshot_order= readl((void *)MOXAARM11_VA_SYSTEM_BASE + 0x24));
			//printk("Number of snapshots: %d snapshot_order=0x%x\n", snapshot_num, snapshot_order);			
			for(i=0; i<snapshot_num; i++){
					aux_seconds = aux_nanoseconds = 0;
					/*Key point: The auxiliary nanoseconds register should be read before the auxiliary seconds register, 
								because the read operation will make the timestamp FIFO pointer move to the next timestamp.*/					
					aux_nanoseconds = readl((void *)(ndev->base_addr + GMAC_AUX_NANOSECOND));					
					aux_seconds = readl((void *)(ndev->base_addr + GMAC_AUX_SECOND));					
					//printk("snapshot %d: pin%d timestamp= %lu:", i, (snapshot_order>>(4*i))&0xF, aux_seconds);
					//printk("%lu\n",aux_nanoseconds);
			}
			writel(0, (void *)(MOXAARM11_VA_SYSTEM_BASE + 0x24));
			moxaarm11_gmac_timestamp_clear_auxiliary_time_fifo(ndev);
		}


		/*
		if(status & AuxiliaryTimestampSnapshotMissed)
			printk("Auxiliary timestamp snapshot missed!!\n");			
		*/

		//writel(0, (void*)(ndev->base_addr + GMAC_TIMESTAMP_STATUS));

	}
#endif		

	writel(dma_status_reg, (void *)(ndev->base_addr + GMAC_DMA_STATUS)); //This is the appropriate location to clear the interrupts
	return IRQ_HANDLED;
}

static struct net_device_stats *moxaarm11_gmac_get_stats(struct net_device *ndev)
{
	moxaarm11_gmac_priv_t *priv;

	priv = netdev_priv(ndev);	
	if(priv == NULL){
		err_printk("%s: Priv Data Pointer Missing.\n", __FUNCTION__);
		return 0;
	}

	return &(priv->stats);
}

static const int multicast_filter_limit = 15;
#if 1		//\\ ChinFu : --------------------------- 2011-12-06
static void moxaarm11_gmac_set_mcast_list(struct net_device *ndev)
{
        moxaarm11_gmac_priv_t *priv = netdev_priv(ndev);
        unsigned long	flags;
        u32 		base;       /* Multicast hash filter */
	unsigned int	mc_count = netdev_mc_count(ndev);	//\\ ChinFu : New Header ------------ 2011-12-06
	
	if (ndev->flags & IFF_PROMISC) {
		/* Unconditionally log net taps. */
		printk("%s: Promiscuous mode enabled.\n", ndev->name);                
		spin_lock_irqsave(&priv->txlock, flags);	
		moxaarm11_gmac_frame_filter_disable(ndev);
		moxaarm11_gmac_promisc_enable(ndev);		
		moxaarm11_gmac_broadcast_enable(ndev);
		moxaarm11_gmac_pass_all_multicast_enable(ndev);				
		spin_unlock_irqrestore(&priv->txlock, flags);
//      } else if ((ndev->mc_count > multicast_filter_limit) || (ndev->flags & IFF_ALLMULTI)) {
	} else if ((mc_count > multicast_filter_limit) || (ndev->flags & IFF_ALLMULTI)) {		//\\ ChinFu : New Header ------------ 2011-12-06
                /* Too many to filter perfectly -- accept all multicasts. */
		spin_lock_irqsave(&priv->txlock, flags);					
		moxaarm11_gmac_pass_all_multicast_enable(ndev);				
		spin_unlock_irqrestore(&priv->txlock, flags);
        } else {
		struct netdev_hw_addr	*hw_addr;
		int			i=0;

		spin_lock_irqsave(&priv->txlock, flags);	
		moxaarm11_gmac_frame_filter_enable(ndev);
		moxaarm11_gmac_promisc_disable(ndev);		
		moxaarm11_gmac_broadcast_enable(ndev);
		moxaarm11_gmac_pass_all_multicast_disable(ndev);				

		netdev_for_each_mc_addr(hw_addr, ndev) {
			u8	*dmi_addr;
			u32	tmp;
			
			/* update perfect match registers */
			dmi_addr = hw_addr->addr;
			base = ndev->base_addr + GMAC_ADDR1_HIGH + i*8;
 			tmp = gmacAddressEnable |(((u32)dmi_addr[5] << 8) &0xff00) | ((u32)dmi_addr[4] & 0xff);
 			writel(tmp, (void *)base);
			tmp = (((u32)dmi_addr[3]<<24) & 0xff000000) |
			      (((u32)dmi_addr[2]<<16) & 0x00ff0000) |
			      (((u32)dmi_addr[1]<<8)  & 0x0000ff00) |
			      (((u32)dmi_addr[0]) & 0x000000ff);
			writel(tmp, (void *)(base+4));		
			i++;
		}
		spin_unlock_irqrestore(&priv->txlock, flags);
        }

}
#endif

static const struct net_device_ops moxaarm11_netdev_gmac_ops = {
	.ndo_init = moxaarm11_gmac_init,
	.ndo_open = moxaarm11_gmac_open,
	.ndo_stop = moxaarm11_gmac_stop,
	.ndo_start_xmit = moxaarm11_gmac_start_xmit,
	.ndo_get_stats = moxaarm11_gmac_get_stats,
	.ndo_set_multicast_list = moxaarm11_gmac_set_mcast_list,
	.ndo_do_ioctl = moxaarm11_gmac_ioctl,
	.ndo_change_mtu = eth_change_mtu,
	.ndo_set_mac_address = eth_mac_addr,
	.ndo_validate_addr = eth_validate_addr,
};

#ifdef CONFIG_MOXAMACRO_GMAC_NAPI
static int moxaarm11_gmac_rx_poll(struct napi_struct *napi, int budget)
{
	moxaarm11_gmac_priv_t *priv = container_of(napi, moxaarm11_gmac_priv_t, rx_napi);
	struct net_device *ndev = priv->ndev;
	u32 work_done;
	unsigned long flags;
	
	//spin_lock(&priv->rxlock);		
	spin_lock_irqsave(&priv->txlock, flags);
	work_done = moxaarm11_gmac_handle_received_data(ndev);
	//printk("work_done=%d budget=%d time=%u\n", work_done, budget, jiffies);
	if (work_done < budget){
		//printk("ReceiveInterrupt work_done < budget\n");
		napi_complete(napi);
		writel(ReceiveInterrupt, ndev->base_addr + GMAC_DMA_STATUS);
		moxaarm11_gmac_dma_interrupt_enable(ndev, ReceiveInterrupt);
	}//else
		//printk("rx work_done=%d\n", work_done);

	//spin_unlock(&priv->rxlock);		
	spin_unlock_irqrestore(&priv->txlock, flags);		

	return work_done;
}
#endif

static struct net_device *moxaarm11_gmac_ndev[MOXAARM11_MAX_GMAC_PORT_NO];
static int __devinit moxaarm11_gmac_probe(struct platform_device *pdev)
{
	struct net_device *ndev;
	struct resource *res;
	moxaarm11_gmac_priv_t *priv;
	u32	regs_phy_addr, regs_len, wakeup_irq;
	int	portno, retval;
	u32	gmii_mac_base=0;

	for ( portno=0; portno<MOXAARM11_MAX_GMAC_PORT_NO; portno++ ) {
		printk("MOXAMACRO GMAC Device Driver Version 1.0, GMAC%d Probe: ", portno+1);
		
		if (!(ndev = alloc_etherdev(sizeof(moxaarm11_gmac_priv_t))))
		return -ENOMEM;
		SET_NETDEV_DEV(ndev, &pdev->dev);
		moxaarm11_gmac_ndev[portno] = ndev;		//keep net_device pointer for freeing memory in remove fn.
		dbg_printk("ndev %d address: 0x%08x\n",portno, (u32)ndev);		
		
		strcpy( ndev->name,  gmac_interface_name[portno]);
		res = platform_get_resource(pdev, IORESOURCE_IRQ, portno*2);
		if (!res) {
			dev_err(&pdev->dev,
				"Found GMAC with no IRQ. Check %s setup!\n",
				dev_name(&pdev->dev));
			retval = -ENODEV;
			goto fail_request_resource;
		}
		ndev->irq = res->start;

		res = platform_get_resource(pdev, IORESOURCE_IRQ, portno*2+1);
		if (!res) {
			dev_err(&pdev->dev,
				"Found GMAC with no wake IRQ. Check %s setup!\n",
				dev_name(&pdev->dev));
			retval = -ENODEV;
			goto fail_request_resource;
		}
		wakeup_irq = res->start;
		
		res = platform_get_resource(pdev, IORESOURCE_MEM, portno);
		if (!res) {
			dev_err(&pdev->dev,
				"Found GMAC with no register addr. Check %s setup!\n", dev_name(&pdev->dev));
			retval = -ENODEV;
			goto fail_request_resource;
		}

		ndev->mem_start = regs_phy_addr = res->start;
		ndev->mem_end = res->end;
		regs_len = res->end - res->start + 1;
		if (!request_mem_region(regs_phy_addr , regs_len, res->name)) {
			dev_err(&pdev->dev, "controller already in use\n");
			retval = -EBUSY;
			goto fail_request_resource;
		}

		ndev->base_addr= (unsigned long)ioremap_nocache(regs_phy_addr, regs_len);
		if ( (void *)ndev->base_addr == NULL ) {
			dev_err(&pdev->dev, "error mapping memory\n");
			retval = -EFAULT;
			goto fail_ioremap;
		}

		ndev->netdev_ops = (struct net_device_ops *)&moxaarm11_netdev_gmac_ops;
#if 1 /* Jared, 2016-08-08, support ethtool ops */
		ndev->ethtool_ops = &moxaarm11_gmac_ethtool_ops;
#endif
		
		retval = register_netdev(ndev);
		if ( retval ) {
			dev_err(&pdev->dev, "registeration failed!\n");
			goto fail_register;
		}
		priv = netdev_priv(ndev);
		priv->wakeup_irq = wakeup_irq;
		//Key Point: The two MDC/MDIO interfaces both utilize GMAC1 controller to access phy chip.		
		if( portno == 0 )
			gmii_mac_base = ndev->base_addr;
		priv->gmii_mac_base = gmii_mac_base;
#if 1	/* 2016-08-08, Jared, fix the phy_id value
	  eth0 phy_id: 0x10
	  eth1 phy_id:0x02
	 */
		switch ( portno ) {
			case 0:
				priv->phyid = 0x10;
				break;
			case 1:
				priv->phyid = 0x02;
				break;
			default:
				break;
		}
#else
		priv->phyid = portno+1;
#endif
		priv->mii_if.phy_id = priv->phyid;
		
#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
		moxaarm11_io_pin_select(GMAC_AUX_TIMESTAMP_TRIG1, 1);
		//moxaarm11_io_pin_select(GMAC_AUX_TIMESTAMP_TRIG2, 1);
		//moxaarm11_io_pin_select(GMAC_AUX_TIMESTAMP_TRIG3, 1);
		//moxaarm11_io_pin_select(GMAC_AUX_TIMESTAMP_TRIG4, 1);
#endif

#ifdef CONFIG_MOXAMACRO_GMAC_NAPI
		netif_napi_add(ndev, &priv->rx_napi, moxaarm11_gmac_rx_poll, NAPI_WEIGHT);
#endif
		printk("successed. \n");
		dbg_printk("Device Name: %s \n", ndev->name);				
		printk("Virtual Address: 0x%08x \n", (u32)ndev->base_addr);
		
	}
	return 0;

fail_register:
	iounmap((unsigned long*)ndev->base_addr);
fail_ioremap:
	release_mem_region(regs_phy_addr, regs_len);
fail_request_resource:	
	free_netdev(ndev);
	err_printk("failed. \n");
	return retval;
}



static int  moxaarm11_gmac_remove(struct platform_device *pdev)
{
	int portno;
	moxaarm11_gmac_priv_t *priv;

	dbg_printk("moxaarm11_gmac_remove: \n");

	for ( portno=0; portno<MOXAARM11_MAX_GMAC_PORT_NO; portno++ ) {
		dbg_printk("%s: ndev %d address: 0x%08x\n",__FUNCTION__, portno, (u32)moxaarm11_gmac_ndev[portno]);		

		priv=netdev_priv(moxaarm11_gmac_ndev[portno]);

		if(priv->netStop != 1)
			moxaarm11_gmac_ndev[portno]->netdev_ops->ndo_stop(moxaarm11_gmac_ndev[portno]);
		
		moxaarm11_gmac_free_memory(moxaarm11_gmac_ndev[portno]);
		iounmap((void *)moxaarm11_gmac_ndev[portno]->base_addr);		
		release_mem_region(moxaarm11_gmac_ndev[portno]->mem_start, 
			moxaarm11_gmac_ndev[portno]->mem_end - moxaarm11_gmac_ndev[portno]->mem_start + 1);
		unregister_netdev(moxaarm11_gmac_ndev[portno]);		
	}


	return 0;
}

static struct platform_driver moxaarm11_gmac_driver = {
	.driver.name	= DRV_NAME,
	.probe		= moxaarm11_gmac_probe,
	.remove		= moxaarm11_gmac_remove,

};

static int __init moxaarm11_gmac_module_init(void)
{
	return platform_driver_register(&moxaarm11_gmac_driver);
}
module_init(moxaarm11_gmac_module_init);

static void __exit moxaarm11_gmac_module_exit(void)
{
	platform_driver_unregister(&moxaarm11_gmac_driver);
	return;
}
module_exit(moxaarm11_gmac_module_exit);

MODULE_DESCRIPTION("Moxa MACRO CPU GMAC Driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vincent Wu");
