
/*
 * This file is the define for Moxa CPU MAC controller.
 *
 * History:
 * Date		Author			Comment
 * 06-15-2005	Victor Yu.		Create it.
 * 10-25-2011	ChinFu Yang	Modified it.
 */
#ifndef _MOXACPU_MAC_H
#define _MOXACPU_MAC_H
#include <linux/mii.h>


//#define DEBUG_MOXAART_MAC
#ifdef DEBUG_MOXAART_MAC
#define dbg_mac(msg, arg...)	printk("*** %s: %s()[%d]: " msg, __FILE__, __func__, __LINE__, ##arg)
#else	
#define dbg_mac(x...)
#endif


typedef struct tx_desc_struct {
	// TXDES0
	union {
		unsigned int	ui;
#define TXDMA_OWN		(1<<31)
#define TXPKT_EXSCOL		(1<<1)
#define TXPKT_LATECOL		(1<<0)
		/* this is bit fields, all are 32 bit in ubit */
		struct {
			unsigned int	TxPktLateCol:1;	// is aborted due to late collision
			unsigned int	TxPktExsCol:1;	// is aborted after 16 collisions
			unsigned int	Reserved1:29;
			unsigned int	TxDMAOwn:1;	// is owned by the MAC controller
		} ubit;
	} txdes0;

	// TXDES1
	union {
		unsigned int	ui;
#define EDOTR			(1<<31)
#define TXIC			(1<<30)
#define TX2FIC			(1<<29)
#define FTS			(1<<28)
#define LTS			(1<<27)
#define TXBUF_SIZE_MASK		0x7ff
#define TXBUF_SIZE_MAX		(TXBUF_SIZE_MASK+1)
		struct {
			unsigned int	TxBufSize:11;	// transmit buffer size in byte
			unsigned int	Reserved2:16;
			unsigned int	Lts:1;		// is the last descriptor of a Tx
							// packet
			unsigned int	Fts:1;		// is the first descriptor of a Tx
							// packet
			unsigned int	Tx2fic:1;	// transmit to FIFO interrupt on
							// completion
			unsigned int	Txic:1;		// transmit interrupt on completion
			unsigned int	Edotr:1;	// end descriptor of transmit ring
		} ubit;
	} txdes1;

	// TXDES2
	struct {
		unsigned int	phyTxBufBaseAddr;// transmit buffer physical base address
		unsigned char	*virtTxBufBaseAddr;// transmit buffer virtual base address
	} txdes2;
} tx_desc_t;






typedef struct rx_desc_struct {
	
	// RXDES0
	union {
		unsigned int	ui;
#define RXDMA_OWN		(1<<31)
#define FRS			(1<<29)
#define LRS			(1<<28)
#define RX_ODD_NB		(1<<22)
#define RUNT			(1<<21)
#define FTL			(1<<20)
#define CRC_ERR			(1<<19)
#define RX_ERR			(1<<18)
#define BROADCAST_RXDES0	(1<<17)
#define MULTICAST_RXDES0	(1<<16)
#define RFL_MASK		0x7ff
#define RFL_MAX			(RFL_MASK+1)
		struct {
			unsigned int	RecvFrameLen:11;	// receive frame length
			unsigned int	Reserved1:5;
			unsigned int	Multicast:1;	// multicast frame
			unsigned int	Broadcast:1;	// broadcast frame
			unsigned int	RxErr:1;	// receive error
			unsigned int	CRCErr:1;	// CRC error
			unsigned int	Ftl:1;		// frame too long
			unsigned int	Runt:1;		// runt packet, less than 64 bytes
			unsigned int	RxOddNb:1;	// receive odd nibbles
			unsigned int	Reserved2:5;
			unsigned int	Lrs:1;		// last receive segment descriptor
			unsigned int	Frs:1;		// first receive segment descriptor
			unsigned int	Reserved3:1;
			unsigned int	RxDMAOwn:1;	// RXDMA onwership
		} ubit;
	} rxdes0;



	// RXDES1
	union {
		unsigned int	ui;
#define EDORR			(1<<31)
#define RXBUF_SIZE_MASK		0x7ff
#define RXBUF_SIZE_MAX		(RXBUF_SIZE_MASK+1)
		struct {
			unsigned int	RxBufSize:11;	// receive buffer size
			unsigned int	Reserved4:20;
			unsigned int	Edorr:1;	// end descriptor of receive ring
		} ubit;
	} rxdes1;



	// RXDES2
	struct {
		unsigned int	Buffer1AddressPointer;// receive buffer physical base address
		unsigned char	*virtRxBufBaseAddr;// receive buffer virtual base address
	} rxdes2;
	
} rx_desc_t;




typedef struct mac_control_reg_struct {
	unsigned int	isr;	// interrupt status, 0x0
#define RPKT_FINISH		(1<<0)	// RXDMA has received packets into RX buffer
					// successfully
#define NORXBUF			(1<<1)	// receive buffer unavailable
#define XPKT_FINISH		(1<<2)	// TXDMA has moved data into the TX FIFO
#define NOTXBUF			(1<<3)	// transmit buffer unavailable
#define XPKT_OK_INT_STS		(1<<4)	// packets transmitted to ethernet successfully
#define XPKT_LOST_INT_STS	(1<<5)	// packets transmitted to ethernet lost due to late
					// collision or excessive collision
#define RPKT_SAV		(1<<6)	// packets received into RX FIFO successfully
#define RPKT_LOST_INT_STS	(1<<7)	// received packet lost due to RX FIFO full
#define AHB_ERR			(1<<8)	// AHB error
#define PHYSTS_CHG		(1<<9)	// PHY link status change
	unsigned int	imr;	// interrupt mask, 0x4
#define RPKT_FINISH_M		(1<<0)	// interrupt mask of ISR[0]
#define NORXBUF_M		(1<<1)	// interrupt mask of ISR[1] 
#define XPKT_FINISH_M		(1<<2)	// interrupt mask of ISR[2] 
#define NOTXBUF_M		(1<<3)	// interrupt mask of ISR[3] 
#define XPKT_OK_M		(1<<4)	// interrupt mask of ISR[4] 
#define XPKT_LOST_M		(1<<5)	// interrupt mask of ISR[5] 
#define RPKT_SAV_M		(1<<6)	// interrupt mask of ISR[6] 
#define RPKT_LOST_M		(1<<7)	// interrupt mask of ISR[7] 
#define AHB_ERR_M		(1<<8)	// interrupt mask of ISR[8] 
#define PHYSTS_CHG_M		(1<<9)	// interrupt mask of ISR[9] 
	unsigned int	mac_madr;	// MAC most significant address, 0x8
#define MAC_MADR_MASK		0xffff	// the most significant 2 bytes of MAC address
	unsigned int	mac_ldar;	// MAC least significant address, 0xc
	unsigned int	matht0;		// multicast address hash table 0, 0x10
	unsigned int	matht1;		// multicast address hash table 1, 0x14
	unsigned int	txpd;		// transmit poll demand, 0x18
	unsigned int	rxpd;		// receive poll demand, 0x1c
	unsigned int	txr_badr;	// transmit ring base address, 0x20
	unsigned int	rxr_badr;	// receive ring base address, 0x24
	unsigned int	itc;		// interrupt timer control, 0x28
#define TXINT_TIME_SEL		(1<<15)	// defines the period of TX cycle time
#define TXINT_THR_MASK		(1<<14|1<13|1<12)
#define TXINT_CNT_MASK		(1<<11|1<<10|1<<9|1<<8)
#define RXINT_TIME_SEL		(1<<7)	// defines the period of RX cycle time
#define RXINT_THR_MASK		(1<<6|1<<5|1<<4)
#define RXINT_CNT_MASK		(1<<3|1<<2|1<<1|1<<0)
	unsigned int	aptc;		// automatic polling timer control, 0x2c
#define TXPOLL_TIME_SEL		(1<<12)	// defines the period of TX poll time
#define TXPOLL_CNT_MASK		(1<<11|1<<10|1<<9|1<<8)
#define TXPOLL_CNT_SHIFT_BIT	8
#define RXPOLL_TIME_SEL		(1<<4)	// defines the period of RX poll time
#define RXPOLL_CNT_MASK		(1<<3|1<<2|1<<1|1<<0)
#define RXPOLL_CNT_SHIFT_BIT	0
	unsigned int	dblac;		// DMA burst length and arbitration control, 0x30
#define RX_THR_EN		(1<<9)	// enable RX FIFO threshold arbitration
#define RXFIFO_HTHR_MASK	(1<<8|1<<7|1<<6)
#define RXFIFO_LTHR_MASK	(1<<5|1<<4|1<<3)
#define INCR16_EN		(1<<2)	// use INCR16 burst command in AHB bus
#define INCR8_EN		(1<<1)	// use INCR8 burst command in AHB bus
#define INCR4_EN		(1<<0)	// use INCR4 burst command in AHB bus
	unsigned int	reserved1[21];	// 0x34 - 0x84
	unsigned int	maccr;		// MAC control, 0x88
#define RX_BROADPKT		(1<<17)	// receive boradcast packet
#define RX_MULTIPKT		(1<<16)	// receive all multicast packet
#define FULLDUP			(1<<15)	// full duplex
#define CRC_APD			(1<<14)	// append CRC to transmitted packet
#define RCV_ALL			(1<<12)	// not check incoming packet's dest. address
#define RX_FTL			(1<<11)	// store incoming packet even if its length is
					// great than 1518 bytes
#define RX_RUNT			(1<<10)	// store incoming packet even if its length is
					// less than 64 bytes
#define HT_MULTI_EN		(1<<9)	// enable storing incoming packet if the packet
					// passes hash table address filtering and is a
					// multicast packet
#define RCV_EN			(1<<8)	// receiver enable
#define ENRX_IN_HALFTX		(1<<6)	// enable packet reception when transmitting 
					// packet in half duplex mode
#define XMT_EN			(1<<5)	// transmitter enable
#define CRC_DIS			(1<<4)	// disable CRC check when receiving packets
#define LOOP_EN			(1<<3)	// internal loop-back
#define SW_RST			(1<<2)	// software reset, last 64 AHB bus clocks
#define RDMA_EN			(1<<1)	// enable receive DMA channel
#define XDMA_EN			(1<<0)	// enable transmit DMA channel
	unsigned int	macsr;		// MAC status, 0x8c
#define COL_EXCEED		(1<<11)	// collision amount exceeds 16
#define LATE_COL		(1<<10)	// transmitter detects late collision
#define XPKT_LOST		(1<<9)	// packet transmitted to ethernet lost due to late
					// collision or excessive collision
#define XPKT_OK			(1<<8)	// packets transmitted to ethernet successfully
#define RUNT_MAC_STS		(1<<7)	// receiver detects a runt packet
#define FTL_MAC_STS		(1<<6)	// receiver detects a frame that is too long
#define CRC_ERR_MAC_STS		(1<<5)
#define RPKT_LOST		(1<<4)	// received packets list due to RX FIFO full
#define RPKT_SAVE		(1<<3)	// packets received into RX FIFO successfully
#define COL			(1<<2)	// incoming packet dropped due to collision
#define MCPU_BROADCAST		(1<<1)
#define MCPU_MULTICAST		(1<<0)
	unsigned int	phycr;		// PHY control, 0x90
#define MIIWR			(1<<27)	// initialize a write sequence to PHY by setting
					// this bit to 1. This bit would be auto cleared
					// after the write operation is finished.
#define MIIRD			(1<<26)
#define REGAD_MASK		(1<<25|1<<24|1<<23|1<<22|1<<21)
#define PHYAD_MASK		(1<<20|1<<19|1<<18|1<<17|1<<16)
#define MIIRDATA_MASK		0xffff
	unsigned int	phywdata;	// PHY write data, 0x94
#define MIIWDATA_MASK		0xffff
	unsigned int	fcr;		// flow control, 0x98
#define PAUSE_TIME_MASK		0xffff0000
#define FC_HIGH_MASK		(1<<15|1<<14|1<<13|1<<12)
#define FC_LOW_MASK		(1<<11|1<<10|1<<9|1<<8)
#define RX_PAUSE		(1<<4)	// receive pause frame
#define TXPAUSED		(1<<3)	// packet transmission is paused due to receive
					// pause frame
#define FCTHR_EN		(1<<2)	// enable flow control threshold mode.
#define TX_PAUSE		(1<<1)	// transmit pause frame
#define FC_EN			(1<<0)	// flow control mode enable
	unsigned int	bpr;		// back pressure, 0x9c
#define BK_LOW_MASK		(1<<11|1<<10|1<<9|1<<8)
#define BKJAM_LEN_MASK		(1<<7|1<<6|1<<5|1<<4)
#define BK_MODE			(1<<1)	// back pressure address mode
#define BK_EN			(1<<0)	// back pressure mode enable
	unsigned int	reserved2[9];	// 0xa0 - 0xc0
	unsigned int	ts;		// test seed, 0xc4
#define TEST_SEED_MASK		0x3fff
	unsigned int	dmafifos;	// DMA/FIFO state, 0xc8
#define TXD_REQ			(1<<31)	// TXDMA request
#define RXD_REQ			(1<<30)	// RXDMA request
#define DARB_TXGNT		(1<<29)	// TXDMA grant
#define DARB_RXGNT		(1<<28)	// RXDMA grant
#define TXFIFO_EMPTY		(1<<27)	// TX FIFO is empty
#define RXFIFO_EMPTY		(1<<26)	// RX FIFO is empty
#define TXDMA2_SM_MASK		(1<<14|1<<13|1<<12)
#define TXDMA1_SM_MASK		(1<<11|1<<10|1<<9|1<<8)
#define RXDMA2_SM_MASK		(1<<6|1<<5|1<<4)
#define RXDMA1_SM_MASK		(1<<3|1<<2|1<<1|1<<0)
	unsigned int	tm;		// test mode, 0xcc
#define SINGLE_PKT		(1<<26)	// single packet mode
#define PTIMER_TEST		(1<<25)	// automatic polling timer test mode
#define ITIMER_TEST		(1<<24)	// interrupt timer test mode
#define TEST_SEED_SEL		(1<<22)	// test seed select
#define SEED_SEL		(1<<21)	// seed select
#define TEST_MODE		(1<<20)	// transmission test mode
#define TEST_TIME_MASK		(1<<19|1<<18|1<<17|1<<16|1<<15|1<<14|1<<13|1<<12|1<<11|1<<10)
#define TEST_EXCEL_MASK		(1<<9|1<<8|1<<7|1<<6|1<<5)
	unsigned int	reserved3;	// 0xd0
	unsigned int	txmcol_xscol;	// TX_MCOL and TX_SCOL counter, 0xd4
#define TX_MCOL_MASK		0xffff0000
#define TX_MCOL_SHIFT_BIT	16
#define TX_SCOL_MASK		0xffff
#define TX_SCOL_SHIFT_BIT	0
	unsigned int	rpf_aep;	// RPF and AEP counter, 0xd8
#define RPF_MASK		0xffff0000
#define RPF_SHIFT_BIT		16
#define AEP_MASK		0xffff
#define AEP_SHIFT_BIT		0
	unsigned int	xm_pg;		// XM and PG counter, 0xdc
#define XM_MASK			0xffff0000
#define XM_SHIFT_BIT		16
#define PG_MASK			0xffff
#define PG_SHIFT_BIT		0
	unsigned int	runtcnt_tlcc;	// RUNT_CNT and TLCC counter, 0xe0
#define RUNT_CNT_MASK		0xffff0000
#define RUNT_CNT_SHIFT_BIT	16
#define TLCC_MASK		0xffff
#define TLCC_SHIFT_BIT		0
	unsigned int	crcercnt_ftlcnt;	// CRCER_CNT and FTL_CNT counter, 0xe4
#define CRCER_CNT_MASK		0xffff0000
#define CRCER_CNT_SHIFT_BIT	16
#define FTL_CNT_MASK		0xffff
#define FTL_CNT_SHIFT_BIT	0
	unsigned int	rlc_rcc;	// RLC and RCC counter, 0xe8
#define RLC_MASK		0xffff0000
#define RLC_SHIFT_BIT		16
#define RCC_MASK		0xffff
#define RCC_SHIFT_BIT		0
	unsigned int	broc;		// BROC counter, 0xec
	unsigned int	mulca;		// MULCA counter, 0xf0	
	unsigned int	rp;		// RP counter, 0xf4
	unsigned int	xp;		// XP counter, 0xf8
} mac_control_reg_t;




#define ISR_REG_OFFSET			0x00
#define IMR_REG_OFFSET			0x04
#define MAC_MADR_REG_OFFSET		0x08
#define MAC_LADR_REG_OFFSET		0x0C
#define MATH0_REG_OFFSET		0x10
#define MATH1_REG_OFFSET		0x14
#define TXPD_REG_OFFSET			0x18
#define RXPD_REG_OFFSET			0x1C
#define TXR_BADR_REG_OFFSET		0x20
#define RXR_BADR_REG_OFFSET		0x24
#define ITC_REG_OFFSET			0x28
#define APTC_REG_OFFSET			0x2C
#define DBLAC_REG_OFFSET		0x30
#define MACCR_REG_OFFSET		0x88
#define MACSR_REG_OFFSET		0x8C
#define PHYCR_REG_OFFSET		0x90
#define PHYWDATA_REG_OFFSET		0x94
#define FCR_REG_OFFSET			0x98
#define BPR_REG_OFFSET			0x9C
#define TS_REG_OFFSET			0xC4
#define DMAFIFOS_REG_OFFSET		0xC8
#define TM_REG_OFFSET			0xCC
#define TX_MCOL_TX_SCOL_REG_OFFSET	0xD4
#define RPF_AEP_REG_OFFSET		0xD8
#define XM_PG_REG_OFFSET		0xDC
#define RUNT_CNT_TLCC_REG_OFFSET	0xE0
#define CRCER_CNT_FTL_CNT_REG_OFFSET	0xE4
#define RLC_RCC_REG_OFFSET		0xE8
#define BROC_REG_OFFSET			0xEC
#define MULCA_REG_OFFSET		0xF0
#define RP_REG_OFFSET			0xF4
#define XP_REG_OFFSET			0xF8


#define TX_DESC_NUM		64
#define TX_DESC_NUM_MASK	(TX_DESC_NUM-1)
#define RX_DESC_NUM		64
#define RX_DESC_NUM_MASK	(RX_DESC_NUM-1)
#define TX_BUF_SIZE		1600
#define RX_BUF_SIZE		1600
#if TX_BUF_SIZE >= TXBUF_SIZE_MAX
#error Moxa CPU ethernet device driver Tx buffer size too large !
#endif
#if RX_BUF_SIZE >= RXBUF_SIZE_MAX
#error Moxa CPU ethernet device driver Rx buffer size too large !
#endif


typedef struct moxaart_mac_priv_struct {
	unsigned int	phyTxDescBaseAddr;		// Tx descriptor physical base address
	tx_desc_t		*virtTxDescBaseAddr;	// Tx descriptor virtual base address
	unsigned int	phyRxDescBaseAddr;		// Rx descriptor physical base address
	rx_desc_t	*virtRxDescBaseAddr;	// Rx descriptor virtual base address
	
	unsigned int	phyTxBufBaseAddr;		// Tx buffer physical base address
	unsigned char	*virtTxBufBaseAddr;		// Tx buffer virtual base address
	unsigned int	phyRxBufBaseAddr[RX_DESC_NUM];		// Rx buffer physical base address
	unsigned char	*virtRxBufBaseAddr;		// Rx buffer virtual base address
	
	int			TxDescNow;				// Tx descriptor now first used index
	int			RxDescNow;				// Rx descriptor now first used index
	int			TxDescBusy;				// Tx descriptor now first busy index	
	int			RxDescBusy;				// Rx descriptor now first busy index		
	struct net_device_stats	stats;			// OS about the ethernet statistics
	spinlock_t	txlock;
	spinlock_t	rxlock;
	unsigned int	maccr;					// store the maccr control register value

	struct work_struct	rqueue;				// add by Victor Yu. 07-04-2005
	int 			mac_num;
	struct mii_if_info	mii_if;				//\\ ChinFu : Added ---------- 2011-10-17
	int			mac_stop;				//\\ ChinFu : Added ---------- 2011-10-17
	spinlock_t 	mii_lock; 				//\\ ChinFu : Added ---------- 2011-10-25
	struct sk_buff *virtRxSkbBufBaseAddr[RX_DESC_NUM];	// Rx buffer virtual base address	//\\ ChinFu : Added ---------- 2011-11-15
	struct sk_buff *virtTxSkbBufBaseAddr[TX_DESC_NUM];	// Tx buffer virtual base address	//\\ ChinFu : Added ---------- 2011-11-15
} moxaart_mac_priv_t;


#define DRV_NAME	"MOXA Art MAC Driver"
#define ARCH_DEV_MAX_NUM 		2

#if CONFIG_MOXAART_MAC_NUM < ARCH_DEV_MAX_NUM
#define CONFIG_ARCH_FLASH_MAC1 0x56
#define CONFIG_MOXAART_MAC_INTERFACE_NAME1 "eth1"
#define CONFIG_MOXAART_MAC_PHY_ADDR1 14
#endif


#define	PHY_CNTL_REG 		0x00
#define	PHY_STATUS_REG 	0x01
#define	PHY_ID_REG1    		0x02
#define	PHY_ID_REG2    		0x03
#define	PHY_ANA_REG    		0x04
#define	PHY_ANLPAR_REG 	0x05
#define	PHY_ANE_REG    		0x06
#define	PHY_ECNTL_REG1 	0x10
#define	PHY_QPDS_REG   	0x11
#define	PHY_10BOP_REG  	0x12
#define	PHY_ECNTL_REG2 	0x13
/* PHY Status register */
#define AN_COMPLETE		0x0020
#define Link_Status			0x0004
//FTMAC100 register definition => 90/94: PHY register
#define FTMAC100_REG_PHY_WRITE		0x08000000
#define FTMAC100_REG_PHY_READ			0x04000000



#endif	// MOXACPU_MAC_H
