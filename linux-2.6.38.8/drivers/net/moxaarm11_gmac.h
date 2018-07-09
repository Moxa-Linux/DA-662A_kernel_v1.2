/*
 * linux/drivers/net/moxaarm11-gmac.h
 *
 * Copyright 2010 Moxa Tech., Inc.
 *      Vincent Wu.<vincent.wu@moxa.com>
 *      http://www.moxa.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/
#ifndef MOXAMACRO_GMAC_H
#define MOXAMACRO_GMAC_H

#include <linux/bitops.h>

#ifdef CONFIG_MOXAMACRO_GMAC_DEBUG
#define dbg_printk(x...)	printk(x)
#else	// MCPU_MAC_DEBUG
#define dbg_printk(x...)
#endif	// MCPU_MAC_DEBUG

#define err_printk(x...)	printk(x)

#define DRV_NAME			"MoxaARM11GMAC"
#define DRV_VERSION			"1.0"
#define MOXAARM11_MAX_GMAC_PORT_NO	2
#define MOXAARM11_GMAC_INTERFACE_NAME1	"eth0\0"
#define MOXAARM11_GMAC_INTERFACE_NAME2	"eth1\0"

#define RX_DESC_NUM		64
#define TX_DESC_NUM		64
#define RX_DESC_NUM_MASK	(RX_DESC_NUM-1)
#define TX_DESC_NUM_MASK	(TX_DESC_NUM-1)
/*Frame buffer size should be larger than Ethernet MTU (1518 bytes).*/
#define TX_BUF_SIZE		1600
#define RX_BUF_SIZE		1600

#define DEFAULT_LOOP_VARIABLE   10000
#define DEFAULT_DELAY_VARIABLE  10

/*TX DMA descriptor format.*/
typedef struct tx_desc_struct {
	// TDES0
	union {
		u32	Status;
#define TXDMA_OWN		BIT(31)	
		struct {
			u32	DeferredBit:1;
			u32	UnderflowError:1;
			u32	ExcessiveDeferral:1;	
			u32	CollisionCount:4;
			u32	VLANFrame:1;
			
			u32	ExcessiveCollision:1;
			u32	LateCollision:1;
			u32	NoCarrier:1;
			u32	LossofCarrier:1;
			u32	IPPayloadError:1;
			u32	FrameFlushed:1;
			u32	JabberTimeout:1;
			u32	ErrorSummary:1;
			
			u32	IPHeaderError:1;
			u32 	TransmitTimestampStatus:1;
			u32	Reserved0:2;
			u32	SecondAddressChained:1;
			u32	TransmitEndofRing:1;
			u32	ChecksumInsertionControl:2;
#define CIC_DISABLE	0x0				
#define CIC_IP_ONLY	0x1				
#define CIC_IP_TCP	0x2				
#define CIC_IP_TCP_PSEUDO	0x3				
			
			u32	Reserved1:1;
			u32	TransmitTimestampEnable:1;
			u32	DisablePad:1;
			u32	DisableCRC:1;
			u32	FirstSegment:1;		
			u32	LastSegment:1;
			u32	InterruptonCompletion:1;
			u32	OwnBit:1;	// is owned by the DMA controller
		} ubit;
	} tdes0;

	// TDES1
	union {
		u32	ui;
#define TXBUF1SIZE_MASK		0x00001FFF		
#define TXBUF2SIZE_MASK		0x00001FFF<<16		
		struct {
			u32	TransmitBuffer1Size:13;
			u32	Reserved0:3;
			u32	TransmitBuffer2Size:13;
			u32	Reserved1:3;
		} ubit;
	} tdes1;

	// TDES2
	struct {
		u32	Buffer1AddressPointer;
	} tdes2;

	// TDES3
	union {
		u32	NextDescriptorAddress;	//in chain mode
		u32	Buffer2AddressPointer;	//in ring mode
	} tdes3;	

#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
	// TDES4
	union {
		u32	Reserved;	
	} tdes4;	

	// TDES5
	union {
		u32	Reserved;
	} tdes5;	

	// TDES6
	union {
		u32	TransmitFrameTimestampLow ;
	} tdes6;		

	// TDES7
	union {
		u32	TransmitFrameTimestampHigh ;
	} tdes7;	
#endif	
} tx_desc_t;

/*RX DMA descriptor format.*/
typedef struct rx_desc_struct {
	// RDES0
	union {
		u32	Status;
#define RXDMA_OWN		BIT(31)
		struct {
			//union{
				u32	RxMACAddress:1;  
			//	u32	ExtendedStatusAvailable:1;
			//}b0;
			u32	CrcError:1;
			u32	DribbleBitError:1;
			u32	ReceiveError:1;
			u32	ReceiveWatchdogTimeout:1;
			u32	FrameType:1;
			u32	LateCollision:1;
			//union{
				u32	IPcChecksumError:1;
			//	u32	GiantFrame:1;
			//}b7;
			
			u32	LastDescriptor:1;
			u32	FirstDescriptor:1;
			u32	VLANTag:1;
			u32	OverflowError:1;
			u32	LengthError:1;
			u32	SourceAddressFilterFail:1;
			u32	DescriptorError:1;
			u32	ErrorSummary:1;
			
			u32	FrameLength:14;
			u32	DestinationAddressFilterFail:1;
			u32	OwnBit:1;
		} ubit;
	} rdes0;

	// RDES1
	union {
		u32	ui;
		struct {
			u32	ReceiveBuffer1Size:13;
			u32	Reserved0:1;
			u32	SecondAddressChained:1;
			u32	ReceiveEndofRing:1;
			
			u32	ReceiveBuffer2Size:13;
			u32	Reserved1:2;
			u32	DisableInterruptonCompletion:1;
		} ubit;
	} rdes1;

	// RDES2
	struct {
		u32	Buffer1AddressPointer;
	} rdes2;

	// RDES3
	union {
		u32	NextDescriptorAddress;	//in chain mode
		u32	Buffer2AddressPointer;	//in ring mode
	} rdes3;	

#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
	// rDES4
	union {
		u32	Extstatus;
		struct {
			u32	IPPayloadType:3;
			u32	IPHeaderError:1;
			u32	IPPayloadError:1; 
			u32	IPChecksumBypassed:1;
			u32	IPv4PacketReceived:1;
			u32	IPv6PacketReceived:1;
			
			u32	MessageType:4;				
			u32	PTPFrameType:1;
			u32	PTPVersion:1;
			u32	TimestampDropped:1;
			u32	Reserved0:1;
			
			u32	AVPacketReceived:1;
			u32	AVTaggedPacketReceived:1;
			u32	VLANTagPriorityValue:3;
			u32	Reserved1:11;		
		} ubit;
	} rdes4;	

	// RDES5
	union {
		u32	Reserved;
	} rdes5;	

	// RDES6
	union {
		u32	ReceiveFrameTimestampLow ;
	} rdes6;		

	// RDES7
	union {
		u32	ReceiveFrameTimestampHigh ;
	} rdes7;	
#endif	
} rx_desc_t;

#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
enum ptp_message_type{
	noPTPmessage = 0,
	syncMessage = 1,
	followUpMessage,
	delayReqMessage,
	delayResqMessage,
	pdelayReqMessage,
	pdelayRespMessage,
	pdelayRespFollowUpMessage,
	announceMessage,
	managementMessage,
	signalingMessage,	
};

/* GNU has not well-defined the SIO requesting IDs for the IEEE1588 related requests,
 * so we temporarily utilize the private SIO requesting IDs for the IEEE1588 application.
 */
#define PTP_GET_RX_TIMESTAMP_SYNC	SIOCDEVPRIVATE
#define PTP_GET_RX_TIMESTAMP_DEL_REQ	(SIOCDEVPRIVATE + 1)
#define PTP_GET_RX_TIMESTAMP_PDEL_REQ	(SIOCDEVPRIVATE + 2)
#define PTP_GET_RX_TIMESTAMP_PDEL_RESP	(SIOCDEVPRIVATE + 3)
#define PTP_GET_TX_TIMESTAMP	(SIOCDEVPRIVATE + 4)
#define PTP_SET_CNT	(SIOCDEVPRIVATE + 5)
#define PTP_GET_CNT	(SIOCDEVPRIVATE + 6)
#define PTP_ADJ_FREQ	(SIOCDEVPRIVATE + 7)
#define PTP_GET_FREQ	(SIOCDEVPRIVATE + 8)
#define PTP_SET_PERIOD	(SIOCDEVPRIVATE + 9)
#define PTP_GET_PERIOD	(SIOCDEVPRIVATE + 10)
#define PTP_GET_TIME	(SIOCDEVPRIVATE + 11)
#define PTP_SET_TIME		(SIOCDEVPRIVATE + 12)
#define PTP_UPDATE_TIME	(SIOCDEVPRIVATE + 13)
#define PTP_SET_TARGET_TIME	(SIOCDEVPRIVATE + 14)

/*Timestamp buffers' size and format to store the inbound/outbound timestamp for the requesting from upper layer.*/
#define RX_TIMESTAMP_BUF_SIZE	16
#define TX_TIMESTAMP_BUF_SIZE	16
typedef struct moxaarm11_gmac_timestamp_struct{
	u16	seq;	
	u16	higherSeconds;
	u32	seconds;
	u32	subseconds;
	u8	valid;
}moxaarm11_gamc_timestamp_t;

#define ETH_PKT_TYPE_OFFSET	0xc

#define IPV4_PKT_PTP_SN_OFFSET	0x48
#define IPV6_PKT_PTP_SN_OFFSET	0x48	//this value has not been well defined
#define ETH_PKT_PTP_SN_OFFSET	0x2Ca

#define ETH_PKT_TYPE_IPV4	0x0800
#define ETH_PKT_TYPE_IPV6	0x86DD
#define ETH_PKT_TYPE_PTP	0x88F7
#endif
#define IOCTL_POWER_DOWN     (SIOCDEVPRIVATE + 15)

typedef struct moxaarm11_gmac_priv_struct {
	u32		phyTxDescBaseAddr;	// Tx descriptor physical base address
	tx_desc_t	*virtTxDescBaseAddr;	// Tx descriptor virtual base address
	u32		phyRxDescBaseAddr;	// Rx descriptor physical base address
	rx_desc_t	*virtRxDescBaseAddr;	// Rx descriptor virtual base address
	//u32	phyTxBufBaseAddr;	// Tx buffer physical base address
	//unsigned char	*virtTxBufBaseAddr;	// Tx buffer virtual base address
	u32		phyRxBufBaseAddr[RX_DESC_NUM];	// Rx buffer physical base address
	u32		phyTxBufBaseAddr[TX_DESC_NUM];	// Tx buffer physical base address
	//unsigned char	*virtRxBufBaseAddr;	// Rx buffer virtual base address	
	struct sk_buff	*virtRxSkbBufBaseAddr[RX_DESC_NUM];	// Rx buffer virtual base address
	struct sk_buff	*virtTxSkbBufBaseAddr[TX_DESC_NUM];	// Tx buffer virtual base address
	
	int		TxDescFree;	// Tx descriptor now first free index
	int		TxDescBusy;	// Tx descriptor now first busy index	
	int		RxDescFree;	// Rx descriptor now first free index
	int		RxDescBusy;	// Rx descriptor now first busy index		
	int		TxPending;
	struct net_device_stats	stats;	// OS about the ethernet statistics

	spinlock_t	txlock;
	//spinlock_t	rxlock;

	u32			gmacControl;	// store the gmac control register value
	u32			gmacFrame;		//store the gma frame control register value

	struct mii_if_info      mii_if;
	struct net_device	*ndev;
	int			netStop;
	int			wakeup_irq;
	int			phyid;	
	int			gmii_mac_base;	
	
#ifdef MOXAMACRO_GMAC_TIMESTAMP_ENABLE
	moxaarm11_gamc_timestamp_t rxTimestamp[RX_TIMESTAMP_BUF_SIZE];
	u16 rxTimestampWriteIndex;
	u16 rxTimestampReadIndex;
	moxaarm11_gamc_timestamp_t txTimestamp[TX_TIMESTAMP_BUF_SIZE];
	u16 txTimestampWriteIndex;
	u16 txTimestampReadIndex;	
#endif	

#ifdef CONFIG_MOXAMACRO_GMAC_NAPI
	struct napi_struct rx_napi;
#endif	
} moxaarm11_gmac_priv_t;

//Define GMAC basic registers
#define GMAC_CONFIG			0x00
enum gmac_config
{
	ReceiverEnable	= BIT(2),
	TransmitterEnable	= BIT(3),
	DeferralCheck	= BIT(4),
	
	BackOffLimit10	= 0x0<<5,	// 2-bit value, 4 choices: 10,8,4,1
	BackOffLimit8	= 0x1<<5,	
	BackOffLimit4	= 0x2<<5,	
	BackOffLimit1	= 0x3<<5,	
	
	AutoPadCRCStrip = BIT(7),
	
	DisableRetry	= BIT(9),
	ChecksumOffload	= BIT(10),
	FullDuplexMode	= BIT(11),
	LoopbackMode	= BIT(12),
	DisableReceiveOwn	= BIT(13),
	FastEthernetSpeed	= BIT(14),	// 1 for 100M, and 0 for 10M
	PortSelectMII	= BIT(15),	// 1 for MII, and 0 for GMII

	DisableCSDT	= BIT(16),		//Carrier Sense During Transmission 
	InterFrameGap96b	= 0x7<<17,		// 3-bit value, 8 choices: 96, 88, ..., 40
	JumboFrameEnable	= BIT(20),
	FrameBurstEnable	= BIT(21),
	JabberDisable		= BIT(22),
	WatchdogDisable		= BIT(23),
	
	TransmitConfigRGMII	= BIT(24),
	CRCstripforTypeframes= BIT(25),	
};
#define GMAC_FRAME				0x04
enum gmac_frame
{
	PromiscuousMode	= BIT(0),	//pass all frames
	HashUnicast	= BIT(1),
	HashMulticast	= BIT(2),
	DAInverseFiltering	= BIT(3),
	PassAllMulticast	= BIT(4),
	DisableBroadcastFrame	= BIT(5),
	
	PassControlFramesNone	= 0x0<<6,	// 2-bit value,
	PassControlFramesAllExceptPause	= 0x1<<6,
	PassControlFramesAll	= 0x2<<6,
	PassControlFramesAllCorrectAddr	= 0x3<<6,
	
	SAInverseFiltering	= BIT(8),
	SAFilterEnable	= BIT(9),
	HashorPerfectFilter	= BIT(10),
	
	ReceiveAll = BIT(31),
};

#define GMAC_HASH_HIGH		0x08
#define GMAC_HASH_LOW			0x0c
#define GMAC_GMII_ADDR			0x10
enum gmac_gmii_addr
{
	CSRClockRangeShfit	= 2,
	GMIIRegisterShifit	= 6,
	GMIIPhyAddrShift		=11,

	GMIIBusy	= BIT(0),
	GMIIWrite	= BIT(1),
	CSRClockRange	= 0xF<<CSRClockRangeShfit, // 4-bit value
	CSRClockRange30M	= 0xB<<CSRClockRangeShfit, // 4-bit value
	GMIIRegister	= 0x1F<<GMIIRegisterShifit,
	GMIIPhyAddr	= 0x1F<<GMIIPhyAddrShift,
};
#define GMAC_GMII_DATA			0x14
#define GMAC_FLOW_CONTROL	0x18
enum gmac_flow_control
{
	FlowControlBusyorBackpressureActivate	= BIT(0),
	TransmitFlowControlEnable	= BIT(1),
	ReceiveFlowControlEnable	= BIT(2),
	UnicastPauseFrameDetect	= BIT(3),
	
	PauseLowThreshold4Slot	= 0x0<<4,	// 2-bit value
	PauseLowThreshold28Slot	= 0x1<<4,
	PauseLowThreshold144Slot	= 0x2<<4,	
	PauseLowThreshold256Slot	= 0x3<<4,	
	
	DisableZeroQuantaPause	= BIT(7),
	PauseTime	= 0xFFFF<<16,	// 16-bit value
};


#define GMAC_VLAN_TAG			0x1c
#define GMAC_VERSION			0x20
#define GMAC_DEBUG				0x24
#define GMAC_WAKE_FRAME		0x28
#define GMAC_PMT_CS			0x2c
enum gmac_pmt_cs
{
	WakeUpFrameFilterPointerReset = BIT(31),
	GlobalUnicast	= BIT(9),
	WakeUpFrameReceived	= BIT(6),
	MagicPacketReceived	= BIT(5),
	WakeUpFrameEnable	= BIT(2),
	MagicPacketEnable	= BIT(1),
	PowerDown	= BIT(0),
};
#define GMAC_LPI_CS				0x30
#define GMAC_LPI_TIMER			0x34
#define GMAC_INT_STATUS		0x38
#define GMAC_INT_MASK			0x3c
enum gmac_int_mask
{
	RGMIIInterruptMask	= BIT(0),
	PCSLinkStatusInterruptMask	= BIT(1),
	PCSANCompletionInterruptMask	= BIT(2),
	PMTInterruptMask	= BIT(3),
	TimestampInterruptMask	= BIT(9),
	LPIInterruptMask	= BIT(10),
};
#define GMAC_ADDR0_HIGH		0x40
#define GMAC_ADDR0_LOW		0x44
#define GMAC_ADDR1_HIGH		0x48
#define GMAC_ADDR1_LOW		0x4c
#define gmacAddressEnable		BIT(31)


//Define phy control registers
#define GMAC_AN_CONTROL	0xC0
#define GMAC_AN_STATUS	0xC4
#define GMAC_AN_ADVERT	0xC8
#define GMAC_AN_PARTNER_ABILITY	0xCC
#define GMAC_AN_EXPANSION	0xD0
#define GMAC_TBI_EXTEND_STATUS	0xd4
#define GMAC_RGMII_STATUS	0xd8
enum gmac_rgmii_status
{
	LinkModeFD	= BIT(0),
	LinkSpeedMask	= BIT(1) | BIT(2),
	LinkSpeed10M	= 0,
	LinkSpeed100M	= BIT(1),
	LinkSpeed1000M	= BIT(2),
	LinkStatusUp		= BIT(3)
};

//Define MMC Registers
#define GMAC_MMC_CONTROL	0x100
#define GMAC_MMC_RX_INT	0x104
#define GMAC_MMC_TX_INT	0x108
#define GMAC_MMC_RX_INT_MASK	0x10C
#define GMAC_MMC_TX_INT_MASK	0x110


//Define GMAC IEEE1588 registers
#define GMAC_TIMESTAMP_CONTROL	0x700
enum gmac_timestamp_control
{
	TimestampEnable	= BIT(0),  
	TimestampFineOrCoarseUpdate	= BIT(1),	//set for fine, reset for coarse
	TimestampInitialize	= BIT(2),
	TimestampUpdate	= BIT(3),
	TimeStampInterruptEnable	= BIT(4),
	AddendRegUpdate	= BIT(5),
	TimestampForAllFrames	= BIT(8),
	TimestampDigitalOrBinaryRollover	= BIT(9),
	EnablePTPv2= BIT(10),
	EnablePTPOverEthernet	= BIT(11),
	EnablePTPOverIPv6	= BIT(12),
	EnablePTPOverIPv4	= BIT(13),
	SnapshotForEventMessages	= BIT(14),
	SnapshotForMessagesRelevantToMaster	= BIT(15),	

	SelectPacketsForSnapshots	= 0x3<<16,		// 2-bit value
	TimestampOrdinaryClock	= 0x0<<16,
	TimestampBoundaryClock	= 0x1<<16,
	TimestampE2ETransparentClock	= 0x2<<16,
	TimestampP2PTransparentClock	= 0x3<<16,
	
	EnableMACAddressPTPFrameFiltering	= BIT(18),	
	AuxiliarySnapshotFIFOClear	= BIT(19),	
};
#define GMAC_SUB_SECOND_INC	0x704
#define GMAC_SECOND	0x708
#define GMAC_SUBSECOND	0x70C
#define GMAC_SECOND_UPDATE	0x710
#define GMAC_SUBSECOND_UPDATE	0x714
#define GMAC_TIMESTAMP_ADDEND	0x718
#define GMAC_TARGET_SECOND	0x71C
#define GMAC_TARGET_SUBSECOND	0x720
#define GMAC_HIGHER_SECOND	0x724
#define GMAC_TIMESTAMP_STATUS	0x728
enum gmac_timestamp_status
{
		TimestampSecondsOverflow	= BIT(0),	
		TimestampTargetTimeReached	= BIT(1),	
		AuxiliaryTimstampTriggerSnapshot	= BIT(2),	
		AuxiliaryTimestampSnapshotMissed= BIT(24),	
		AuxiliaryTimestampNumberOfSnapshotsShift = 25,
		AuxiliaryTimestampNumberOfSnapshots= 0x7<<AuxiliaryTimestampNumberOfSnapshotsShift,	// 3-bit value
};
#define GMAC_AUX_TIMESTAMP_TRIG1 6
#define GMAC_AUX_TIMESTAMP_TRIG2 7
#define GMAC_AUX_TIMESTAMP_TRIG3 8
#define GMAC_AUX_TIMESTAMP_TRIG4 9

#define GMAC_PPS_CONTROL	0x72C
#define GMAC_AUX_NANOSECOND	0x730
#define GMAC_AUX_SECOND	0x734

//Define GMAC DMA registers
#define GMAC_DMA_BUS_MODE		0x1000
enum gmac_dma_bus_mode
{
	SoftwareReset	= BIT(0),
		
	DescSkipLength0	= 0x0<<2,		//5-bit value, default to 1
	DescSkipLength1	= 0x1<<2,
	DescSkipLength2	= 0x2<<2,
	DescSkipLength4	= 0x4<<2,
	DescSkipLength8	= 0x8<<2,
	DescSkipLength16	= 0x10<<2,
  
	AlternateDescSize	= BIT(7),
	
	BurstLength0	= 0x0<<8,				//6-bit value, default to 1
	BurstLength1	= 0x1<<8,
	BurstLength2	= 0x2<<8,
	BurstLength4	= 0x4<<8,
	BurstLength8	= 0x8<<8,
	BurstLength16	= 0x10<<8,
	BurstLength32	= 0x20<<8,
	
	FixedBurst	= BIT(16),
	RxDMAPBL	= 0x3F<<17,				//6-bit value default to 1
	RxBurstLength32	= 0x20<<17,
	UseSeparatePBL	= 1<<23,

	PBLModex8	= BIT(24),				//if true, BurstLength * 8
	AddrAlignedBeats	= BIT(25),
};
#define GMAC_DMA_TXPOLL_DEMAND	0x1004
#define GMAC_DMA_RXPOLL_DEMAND	0x1008
#define GMAC_DMA_RXDESC_ADDR		0x100c
#define GMAC_DMA_TXDESC_ADDR		0x1010
#define GMAC_DMA_STATUS			0x1014
enum gmac_dma_status
{
	/*interrupt status bits (bit0 ~ bit16) definition is the same as the enum gmac_dma_int_en*/	
	ReceiveProcessStat	= 0x7<<17, //  3-bit value
	TransmitProcessState	= 0x7<<20, //  3-bit value
	ErrorBits	= 0x7<<23,
	GMACLineinterfaceInterrupt	= BIT(26),
	GMACMMCInterrupt	= BIT(27),
	GMACPMTInterrupt	= BIT(28),
	TimeStampTriggerInterrupt	= BIT(29),
	GMACLPIInterrupt	= BIT(30),
};
#define GMAC_DMA_OP_MODE			0x1018
enum gmac_dma_op_mode
{
	StartReceive = BIT(1),
	Operate2ndFrame	= BIT(2),
	
	ReceiveThreshold64	= 0x0<<3, // 2-bit value
	ReceiveThreshold32	= 0x1<<3,
	ReceiveThreshold96	= 0x2<<3,
	ReceiveThreshold128	= 0x3<<3,
	
	ForwardUndersizedFrames	= BIT(6),
	ForwardErrorFrames	= BIT(7),
	HWFlowControl	= BIT(8),
	
	ThresholdActivatingFlowControl3KB	= 0x0<<9,	// 2-bit value, Full - 1KB
	ThresholdActivatingFlowControl2KB	= 0x1<<9,	//Full - 2KB
	ThresholdActivatingFlowControl1KB	= 0x2<<9,	//Full - 3KB
	ThresholdActivatingFlowControl0KB	= 0x3<<9,	//Full - 4KB

	ThresholdDeactivatingFlowControl3KB	= 0x0<<11,	// 2-bit value, Full - 1KB
	ThresholdDeactivatingFlowControl2KB	= 0x1<<11,	//Full - 2KB
	ThresholdDeactivatingFlowControl1KB	= 0x2<<11,	//Full - 3KB
	ThresholdDeactivatingFlowControl0KB	= 0x3<<11,	//Full - 4KB	
	
	StartTransmission	= BIT(13),
	
	TransmitThreshold64	= 0x0<<14, // 3-bit value
	TransmitThreshold128	= 0x1<<14,
	TransmitThreshold192	= 0x2<<14,
	TransmitThreshold256	= 0x3<<14,
	TransmitThreshold40	= 0x4<<14, 
	TransmitThreshold32	= 0x5<<14,
	TransmitThreshold24	= 0x6<<14,
	TransmitThreshold16	= 0x7<<14,
	
	FlushTransmitFIFO	= BIT(20),
	TransmitStoreForward= BIT(21),
	
	DisableFlushingReceivedFrames= BIT(24),
	ReceiveStoreForward= BIT(25),	
	DisableDroppingTCPIPErrorFrames	= BIT(26),
};
#define GMAC_DMA_INT_EN			0x101c
enum gmac_dma_int_en
{
	TransmitInterrupt	= BIT(0),
	TransmitStoppedInterrupt	= BIT(1),
	TransmitBufferUnavailableInterrupt	= BIT(2),		
	TransmitJabberTimeoutInterrupt	= BIT(3),
	OverflowInterrupt	= BIT(4),
	UnderflowInterrupt	= BIT(5),		
	ReceiveInterrupt	= BIT(6),
	ReceiveBufferUnavailableInterrupt	= BIT(7),
	ReceiveStoppedInterrupt	= BIT(8),		
	ReceiveWatchdogTimeoutInterrupt	= BIT(9),
	EarlyTransmitInterrupt	= BIT(10),
	FatalBusErrorInterrupt	= BIT(13),		
	EarlyReceiveInterrupt	= BIT(14),
	AbnormalInterruptSummary	= BIT(15),
	NormalInterruptSummary	= BIT(16),		

	InitInterruptEnable = NormalInterruptSummary|AbnormalInterruptSummary|FatalBusErrorInterrupt|
		ReceiveBufferUnavailableInterrupt|ReceiveInterrupt|ReceiveStoppedInterrupt|
		TransmitInterrupt|TransmitStoppedInterrupt,
};
#define GMAC_DMA_MSBO_COUNT		0x1020
#define GMAC_DMA_AXI_MODE		0x1028
enum gmac_dma_axi_mode
{
	AXIUndefinedBurstLength	= BIT(0),
	AXIBurstLength4	= BIT(1),
	AXIBurstLength8	= BIT(2),
	AXIBurstLength16	= BIT(3),
	AXIBurstLength32	= BIT(4),
	AXIBurstLength64	= BIT(5),
	AXIBurstLength128	= BIT(6),
	AXIBurstLength256	= BIT(7),
	AddressAlignedBeats	= BIT(12),	
	AXIMaxReadReqLimit	= 0xF<<16,	// 4-bit value
	AXIMaxReadReqLimit00	= 0x0<<16,		
	AXIMaxReadReqLimit01	= 0x1<<16,		
	AXIMaxWriteReqLimit	= 0xF<<20,
	AXIMaxWriteReqLimit00	= 0x0<<20,				
	AXIMaxWriteReqLimit01	= 0x1<<20,			
	UnlockonRemoteWakeUp	= BIT(30),	
	EnableLPI	= BIT(31),	
};
#define GMAC_DMA_AXI_STATUS		0x102c

#define GMAC_DMA_CURRENT_TXDESC	0x1048
#define GMAC_DMA_CURRENT_RXDESC	0x104c
#define GMAC_DMA_CURRENT_TXBUF	0x1050
#define GMAC_DMA_CURRENT_RXBUF	0x1054
#define GMAC_DMA_HW_FEATURE		0x1058

static u32 GMAC_power_down;
static char gmac_interface_name[MOXAARM11_MAX_GMAC_PORT_NO][IFNAMSIZ]= {MOXAARM11_GMAC_INTERFACE_NAME1, MOXAARM11_GMAC_INTERFACE_NAME2};
#endif
