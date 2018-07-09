
/*
 * This program is the Moxa CPU ethernet device driver.
 *
 * History:
 * Date		Author			Comment
 * 06-15-2005	Victor Yu.		Create it. Make it for Faraday demo board.
 * 11-04-2005	Victor Yu.		Modify it to support Moxa CPU demo board.
 * 09-20-2011   Wade Liang		Refer to bfin_mac.c for new structure
 */

//#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/in.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/spinlock.h>
#include <linux/skbuff.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/bitops.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/dma.h>
#include <linux/mtd/mtd.h>
#include "moxacpu_mac.h"
#include <mach/moxaart_int.h>
#include <mach/hardware.h>
#include <linux/ethtool.h>
#include <linux/mii.h>
#include <linux/sched.h>

//#define MCPU_MAC_DEBUG
//#define USE_SCHEDULE_WORK
#define HAVE_MULTICAST


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



#ifdef HAVE_MULTICAST
static int crc32( char * s, int length )
{
        /* indices */
        int			perByte;
        int			perBit;
        /* crc polynomial for Ethernet */
        const unsigned long	poly=0xedb88320;
        /* crc value - preinitialized to all 1's */
        unsigned long		crc_value=0xffffffff;

        for ( perByte = 0; perByte < length; perByte ++ ) {
                unsigned char   c;

                c = *(s++);
                for ( perBit = 0; perBit < 8; perBit++ ) {
                        crc_value = (crc_value>>1)^
                                (((crc_value^c)&0x01)?poly:0);
                        c >>= 1;
                }
        }
        return  crc_value;
}



static void mcpu_mac_setmulticast(struct net_device *ndev)
{
	int crc_val;
	struct netdev_hw_addr *ha; 
	unsigned int ioaddr = ndev->base_addr; 

	netdev_for_each_mc_addr(ha, ndev) {
		if (!(*ha->addr & 1)){
			continue;
		}
		crc_val = crc32(ha->addr, 6 );
		crc_val = (crc_val>>26) & 0x3f;                   // ¨ú MSB 6 bit
		if (crc_val >= 32){
			outl(inl(ioaddr+MATH1_REG_OFFSET) | (1UL<<(crc_val-32)), ioaddr+MATH1_REG_OFFSET);
		}else{
			outl(inl(ioaddr+MATH0_REG_OFFSET) | (1UL<<crc_val), ioaddr+MATH0_REG_OFFSET);
		}
	}
}



static void mcpu_mac_set_multicast_list(struct net_device *ndev)
{
	mcpu_mac_priv_t		*priv = (mcpu_mac_priv_t *)netdev_priv(ndev); 
	unsigned long		flags;

	local_irq_save(flags);
#ifdef USE_SCHEDULE_WORK
	spin_lock(&priv->rxlock);
#endif
	spin_lock(&priv->txlock);

	/* promisc mode */
	if (ndev->flags & IFF_PROMISC){
		priv->maccr |= RCV_ALL;
	}else{
		priv->maccr &= ~RCV_ALL;
	}

	if (ndev->flags & IFF_ALLMULTI){
		priv->maccr |= RX_MULTIPKT;
	}else{
		priv->maccr &= ~RX_MULTIPKT;
	}

	if (!netdev_mc_empty(ndev)) {
		priv->maccr |= RX_MULTIPKT; //add by Johnson.Liu 2007/07/06
		priv->maccr |= HT_MULTI_EN;
		mcpu_mac_setmulticast(ndev);
	} else {
        	priv->maccr &= ~RX_MULTIPKT; //add by Johnson.Liu 2007/07/06
		priv->maccr &= ~HT_MULTI_EN;
	}		

	outl(priv->maccr, ndev->base_addr+MACCR_REG_OFFSET);

	spin_unlock(&priv->txlock);
#ifdef USE_SCHEDULE_WORK
	spin_unlock(&priv->rxlock);
#endif
	local_irq_restore(flags);
}
#endif	// HAVE_MULTICAST



static void mywordcopy(void *dest, void *source, int len)
{
	unsigned short	*pd=(unsigned short *)dest;
	unsigned short	*ps=(unsigned short *)source;
	int		wlen=len>>1;

	while ( wlen > 0 ) {
		*pd++=*ps++;
		wlen--;
	}
	if ( len & 1 ){
		*(unsigned char *)pd = *(unsigned char *)ps;
	}		
}



/* call by ISR */
static void mcpu_mac_recv(void *ptr)
{
	struct net_device	*ndev=(struct net_device *)ptr;
	mcpu_mac_priv_t		*priv=(mcpu_mac_priv_t *)netdev_priv(ndev);
	rx_desc_t		*rxdesc;
	int			len;
	struct sk_buff		*skb;

	unsigned char		*data;
	unsigned int		ui;
	int			rxnow=priv->RxDescNow;
#ifndef USE_SCHEDULE_WORK
	int			loops=RX_DESC_NUM;
#endif	// USE_SCHEDULE_WORK

	debug("mcpu_mac_recv test01\n");
	
#ifdef USE_SCHEDULE_WORK
	spin_lock(&priv->rxlock);
#endif	// USE_SCHEDULE_WORK
repeat_recv:
	rxdesc = &priv->virtRxDescBaseAddr[rxnow];
	ui = rxdesc->rxdes0.ui;
	if ( ui & RXDMA_OWN ) {
#ifdef USE_SCHEDULE_WORK
		spin_unlock(&priv->rxlock);
#else
#ifdef MCPU_MAC_DEBUG
		if ( loops == RX_DESC_NUM )
			printk("Bad receive packet !\n");
#endif	// MCPU_MAC_DEBUG
#endif	// USE_SCHEDULE_WORK
		return;
	}
#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)
	if ( ui & (RX_ERR|CRC_ERR|RUNT|RX_ODD_NB) ) {
#else
	if ( ui & (RX_ERR|CRC_ERR|FTL|RUNT|RX_ODD_NB) ) {
#endif
		debug("Ethernet receive packet error !\n");
		priv->stats.rx_dropped++;
		priv->stats.rx_errors++;
		goto recv_finish;
	}
	len = ui & RFL_MASK;
	if ( len > RX_BUF_SIZE )
		len = RX_BUF_SIZE;
	skb = dev_alloc_skb(len+2);
	if ( skb == NULL ) {
		debug("Allocate memory fail !\n");
		priv->stats.rx_dropped++;
		goto recv_finish;
	}
	skb_reserve(skb, 2);
	skb->dev = ndev;
	data = skb_put(skb, len);
	debug("receive data pointer = 0x%x\n", (unsigned long)data);
	mywordcopy((void *)data, (void *)rxdesc->rxdes2.virtRxBufBaseAddr, len);
	skb->protocol = eth_type_trans(skb, ndev);
	netif_rx(skb);
	priv->stats.rx_packets++;
	priv->stats.rx_bytes += len;
	if ( ui & MULTICAST_RXDES0 )
		priv->stats.multicast++;
	debug("Receive a good packet.\n");

recv_finish:
	rxdesc->rxdes0.ui = RXDMA_OWN;
	rxnow++;
	rxnow &= RX_DESC_NUM_MASK;
	priv->RxDescNow = rxnow;

#ifdef USE_SCHEDULE_WORK
	goto repeat_recv;
#else	// USE_SCHEDULE_WORK
	if ( loops-- > 0 )
		goto repeat_recv;
#endif	// USE_SCHEDULE_WORK

#ifdef USE_SCHEDULE_WORK
	spin_unlock(&priv->rxlock);
#endif	// USE_SCHEDULE_WORK


}



static void mcpu_mac_free_memory(struct net_device *ndev)
{
	mcpu_mac_priv_t		*priv=(mcpu_mac_priv_t *)netdev_priv(ndev); 

	if (priv->virtTxDescBaseAddr)
		dma_free_coherent(NULL, sizeof(tx_desc_t)*TX_DESC_NUM, priv->virtTxDescBaseAddr, priv->phyTxDescBaseAddr);
	if (priv->virtRxDescBaseAddr)
		dma_free_coherent(NULL, sizeof(rx_desc_t)*RX_DESC_NUM, priv->virtRxDescBaseAddr, priv->phyRxDescBaseAddr);
	if (priv->virtTxBufBaseAddr)
		dma_free_coherent(NULL, TX_BUF_SIZE*TX_DESC_NUM, priv->virtTxBufBaseAddr, priv->phyTxBufBaseAddr);
	if (priv->virtRxBufBaseAddr)
		dma_free_coherent(NULL, RX_BUF_SIZE*RX_DESC_NUM, priv->virtRxBufBaseAddr, priv->phyRxBufBaseAddr);
}



static void mcpu_mac_setup_desc_ring(struct net_device *ndev)
{
	mcpu_mac_priv_t *priv=(mcpu_mac_priv_t *)netdev_priv(ndev); 
	int	i;
	tx_desc_t *txdesc;
	rx_desc_t *rxdesc;
	unsigned char *virtbuf;
	unsigned int phybuf;

	virtbuf = priv->virtTxBufBaseAddr;
	phybuf = priv->phyTxBufBaseAddr;
	for ( i=0; i<TX_DESC_NUM; i++, virtbuf+=TX_BUF_SIZE, phybuf+=TX_BUF_SIZE ) {
		txdesc = &priv->virtTxDescBaseAddr[i];
		memset(txdesc, 0, sizeof(tx_desc_t));
		txdesc->txdes2.phyTxBufBaseAddr = phybuf;
		txdesc->txdes2.virtTxBufBaseAddr = virtbuf;
	}
	priv->virtTxDescBaseAddr[TX_DESC_NUM-1].txdes1.ubit.Edotr = 1;
	
	virtbuf = priv->virtRxBufBaseAddr;
	phybuf = priv->phyRxBufBaseAddr;
	for ( i=0; i<RX_DESC_NUM; i++, virtbuf+=RX_BUF_SIZE, phybuf+=RX_BUF_SIZE ) {
		rxdesc = &priv->virtRxDescBaseAddr[i];
		memset(rxdesc, 0, sizeof(rx_desc_t));
		rxdesc->rxdes0.ubit.RxDMAOwn = 1;
		rxdesc->rxdes1.ubit.RxBufSize = RX_BUF_SIZE;
		rxdesc->rxdes2.phyRxBufBaseAddr = phybuf;
		rxdesc->rxdes2.virtRxBufBaseAddr = virtbuf;
	}
	priv->virtRxDescBaseAddr[RX_DESC_NUM-1].rxdes1.ubit.Edorr = 1;

	priv->TxDescNow = priv->RxDescNow = 0;
	
	// reset the MAC controler Tx/Rx desciptor base address
	outl(priv->phyTxDescBaseAddr, ndev->base_addr+TXR_BADR_REG_OFFSET);
	outl(priv->phyRxDescBaseAddr, ndev->base_addr+RXR_BADR_REG_OFFSET);
}



static void mcpu_mac_reset(struct net_device *ndev)
{
	unsigned int	reg = ndev->base_addr+MACCR_REG_OFFSET;

	outl(SW_RST, reg);	// software reset
	while (inl(reg) & SW_RST) mdelay(10);
	// maybe we need to disable the all interrupt
	outl(0, ndev->base_addr+IMR_REG_OFFSET);
#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE) //johnson modify for 802.1Q support 2008-11-14
	((mcpu_mac_priv_t *)netdev_priv(ndev))->maccr = RX_BROADPKT | ENRX_IN_HALFTX | CRC_APD | RX_FTL;
#else
	((mcpu_mac_priv_t *)netdev_priv(ndev))->maccr = RX_BROADPKT | ENRX_IN_HALFTX | CRC_APD;
#endif
}



static void mcpu_mac_set_mac_address(unsigned int base, unsigned char *macaddr)
{
	unsigned int	val;

	val = (((u32)macaddr[0] << 8) &0xff00) | ((u32)macaddr[1] & 0xff);
	outl(val, base);
	val = (((u32)macaddr[2]<<24) & 0xff000000) |
	      (((u32)macaddr[3]<<16) & 0x00ff0000) |
	      (((u32)macaddr[4]<<8)  & 0x0000ff00) |
	      (((u32)macaddr[5]) & 0x000000ff);
	outl(val, base+4);
}



#ifdef MCPU_MAC_DEBUG	// add by Victor Yu. 03-14-2006
static void mcpu_mac_get_mac_address(unsigned int base, unsigned char *macaddr)
{
	unsigned int	val;

	val = inl(base);
	macaddr[0] = (val >> 8) & 0xff;
	macaddr[1] = val & 0xff;
	val = inl(base+4);
	macaddr[2] = (val >> 24) & 0xff;
	macaddr[3] = (val >> 16) & 0xff;
	macaddr[4] = (val >> 8) & 0xff;
	macaddr[5] = val & 0xff;
}
#endif



static void mcpu_mac_enable(struct net_device *ndev)
{
	mcpu_mac_priv_t *priv = (mcpu_mac_priv_t *)netdev_priv(ndev); 
	unsigned int base=ndev->base_addr;

	outl(0x00001010, base+ITC_REG_OFFSET);
	outl(0x00000001, base+APTC_REG_OFFSET);
	outl(0x00000390, base+DBLAC_REG_OFFSET);
#ifdef MCPU_MAC_DEBUG
	outl(RPKT_FINISH_M|NORXBUF_M|AHB_ERR_M, base+IMR_REG_OFFSET);
#else
	outl(RPKT_FINISH_M, base+IMR_REG_OFFSET);
#endif
	priv->maccr |= (RCV_EN | XMT_EN | RDMA_EN | XDMA_EN);
	outl(priv->maccr, base+MACCR_REG_OFFSET);
}



static void mcpu_mac_tx_timeout(struct net_device *ndev)
{
	mcpu_mac_priv_t	*priv=(mcpu_mac_priv_t *)netdev_priv(ndev); 
	unsigned long	flags;

	debug("mcpu_mac_tx_timeout test01\n");
	local_irq_save(flags);
#ifdef USE_SCHEDULE_WORK
	spin_lock(&priv->rxlock);
#endif
	spin_lock(&priv->txlock);
	mcpu_mac_reset(ndev);
	/* read mac addr, and set to dev->dev_addr */
	mcpu_mac_set_mac_address(ndev->base_addr+MAC_MADR_REG_OFFSET, ndev->dev_addr);
	mcpu_mac_setup_desc_ring(ndev);
	mcpu_mac_enable(ndev);
	spin_unlock(&priv->txlock);
#ifdef USE_SCHEDULE_WORK
	spin_unlock(&priv->rxlock);
#endif
	local_irq_restore(flags);
	netif_wake_queue(ndev);
	ndev->trans_start = jiffies;
}



static int mcpu_mac_hard_start_xmit(struct sk_buff *skb, struct net_device *ndev)
{
	mcpu_mac_priv_t *priv=(mcpu_mac_priv_t *)netdev_priv(ndev); 
	tx_desc_t *txdesc;
	int			len;
	int			txnow=priv->TxDescNow;

	debug("mcpu_mac_hard_start_xmit test01\n");
	spin_lock(&priv->txlock);

	// first check the Tx buffer is enough or not
	txdesc = &priv->virtTxDescBaseAddr[txnow];
	if ( txdesc->txdes0.ubit.TxDMAOwn ) {
		debug("No Tx space to transmit the packet !\n");
		priv->stats.tx_dropped++;
		goto xmit_final;
	}

	len = skb->len > TX_BUF_SIZE ? TX_BUF_SIZE : skb->len;
	mywordcopy((void *)txdesc->txdes2.virtTxBufBaseAddr, (void *)skb->data, len);
	debug("transmit data pointer = 0x%x\n", (unsigned long)skb->data);
	if ( skb->len < ETH_ZLEN ) {
		memset(&txdesc->txdes2.virtTxBufBaseAddr[skb->len], 0, ETH_ZLEN-skb->len);
		len = ETH_ZLEN;
	}
	txdesc->txdes1.ubit.Lts = 1;
	txdesc->txdes1.ubit.Fts = 1;
	txdesc->txdes1.ubit.Tx2fic = 0;
	txdesc->txdes1.ubit.Txic = 0;
	txdesc->txdes1.ubit.TxBufSize = len;
	txdesc->txdes0.ui = TXDMA_OWN;
	outl(0xffffffff, ndev->base_addr+TXPD_REG_OFFSET);	// start to send packet
	txnow++;
	txnow &= TX_DESC_NUM_MASK;
	priv->TxDescNow = txnow;
	ndev->trans_start = jiffies;
	priv->stats.tx_packets++;
	priv->stats.tx_bytes += len;

xmit_final:
	spin_unlock(&priv->txlock);
	dev_kfree_skb_any(skb);

	return 0;
}



static int mcpu_mac_open(struct net_device *ndev)
{
	mcpu_mac_priv_t	*priv=(mcpu_mac_priv_t *)netdev_priv(ndev);
	unsigned long flags;

	debug("mcpu_mac_open test01\n");

	local_irq_save(flags);

#ifdef USE_SCHEDULE_WORK
	spin_lock(&priv->rxlock);
#endif

	spin_lock(&priv->txlock);
	mcpu_mac_reset(ndev);
	/* read mac address, set to dev_addr */
	mcpu_mac_set_mac_address(ndev->base_addr+MAC_MADR_REG_OFFSET, ndev->dev_addr);
	mcpu_mac_setup_desc_ring(ndev);
	mcpu_mac_enable(ndev);
	spin_unlock(&priv->txlock);

#ifdef USE_SCHEDULE_WORK
	spin_unlock(&priv->rxlock);
#endif

	local_irq_restore(flags);

	/* allow device to transmit package*/
	netif_start_queue(ndev);
	debug("IMR=0x%x, MACCR=0x%x\n", inl(ndev->base_addr+IMR_REG_OFFSET), inl(ndev->base_addr+MACCR_REG_OFFSET));

#ifdef MCPU_MAC_DEBUG
	{
	unsigned char	macaddr[6];
	int		i = 0;
	mcpu_mac_get_mac_address(ndev->base_addr+MAC_MADR_REG_OFFSET, macaddr);
	printk("Get MAC address = ");
	for ( i=0; i<6; i++ ){
		printk("%02X ", macaddr[i]);
	}
	printk("\n");
	}
#endif

	return 0;
}



static int mcpu_mac_stop(struct net_device *ndev)
{
	mcpu_mac_priv_t		*priv=(mcpu_mac_priv_t *)netdev_priv(ndev); 
	unsigned long		flags;

	debug("mcpu_mac_stop test01\n");
	/* disable transmit package */
	netif_stop_queue(ndev);

	local_irq_save(flags);
#ifdef USE_SCHEDULE_WORK
	spin_lock(&priv->rxlock);
#endif
	spin_lock(&priv->txlock);
	outl(0, ndev->base_addr+IMR_REG_OFFSET);		// disable all interrupt
	outl(0, ndev->base_addr+MACCR_REG_OFFSET);	// disable all function
	spin_unlock(&priv->txlock);
#ifdef USE_SCHEDULE_WORK
	spin_unlock(&priv->rxlock);
#endif
	local_irq_restore(flags);

	return 0;
}



static struct net_device_stats *mcpu_mac_get_stats(struct net_device *ndev)
{
	mcpu_mac_priv_t		*priv=(mcpu_mac_priv_t *)netdev_priv(ndev); 

	debug("mcpu_mac_get_stats test01\n");
	return &priv->stats;
}



static irqreturn_t mcpu_mac_interrupt(int irq, void *dev_id)
{
	struct net_device	*ndev=(struct net_device *)dev_id;
	unsigned int		ists;
#ifdef USE_SCHEDULE_WORK
	mcpu_mac_priv_t		*priv=(mcpu_mac_priv_t *)netdev_priv(ndev); 
#endif

	
	//debug("mcpu_mac_interrupt test01\n");
	ists = inl(ndev->base_addr+ISR_REG_OFFSET);
	if ( ists & RPKT_FINISH ) {
#ifdef USE_SCHEDULE_WORK
		schedule_work(&priv->rqueue);
#else
		mcpu_mac_recv((void *)ndev);
#endif
	} else {
#ifdef MCPU_MAC_DEBUG
		if ( ists & NORXBUF ) {
			printk("Receiver no Rx buffer interrupt\n");
			outl(inl(ndev->base_addr+IMR_REG_OFFSET)&~NORXBUF_M, ndev->base_addr+IMR_REG_OFFSET);
			//return IRQ_HANDLED;
		}
		if ( ists & AHB_ERR ) {
			printk("Receiver AHB error interrupt.\n");
			//return IRQ_HANDLED;
		}
#endif
		//return IRQ_NONE;
	}

	return IRQ_HANDLED;
}




static int mcpu_mac_init(struct net_device *ndev, int mac_num)
{
	struct mtd_info *mtd;
	int len = 0;

	mcpu_mac_priv_t	*priv=(mcpu_mac_priv_t *)netdev_priv(ndev); 

	debug("mcpu_mac_init() \n");
	memset((void *)priv, 0, sizeof(mcpu_mac_priv_t));
	spin_lock_init(&priv->txlock);

#ifdef USE_SCHEDULE_WORK
	spin_lock_init(&priv->rxlock);
	INIT_WORK(&priv->rqueue, &mcpu_mac_recv, (void *)ndev);
#endif

	/* allocate DMA for the descriptor and buffer memory */
	priv->virtTxDescBaseAddr = (tx_desc_t *)dma_alloc_coherent(NULL, sizeof(tx_desc_t)*TX_DESC_NUM, 
			(dma_addr_t *)&priv->phyTxDescBaseAddr, GFP_DMA|GFP_KERNEL);	
	if ( priv->virtTxDescBaseAddr == NULL || (priv->phyTxDescBaseAddr & 0x0f) ) {
		debug("*** Allocate the Tx descriptor memory fail !\n");
		goto init_fail;
	}

	priv->virtRxDescBaseAddr = (rx_desc_t *)dma_alloc_coherent(NULL, sizeof(rx_desc_t)*RX_DESC_NUM, 
			(dma_addr_t *)&priv->phyRxDescBaseAddr, GFP_DMA|GFP_KERNEL);
	if ( priv->virtRxDescBaseAddr == NULL || (priv->phyRxDescBaseAddr & 0x0f) ) {
		debug("Allocate the Rx descriptor memory fail !\n");
		goto init_fail;
	}

	priv->virtTxBufBaseAddr = (unsigned char *)dma_alloc_coherent(NULL, TX_BUF_SIZE*TX_DESC_NUM, 
			(dma_addr_t *)&priv->phyTxBufBaseAddr, GFP_DMA|GFP_KERNEL);
	if ( priv->virtTxBufBaseAddr == NULL || (priv->phyTxBufBaseAddr & 0x03) ) {
		debug("Allocate the Tx buffer memory fail !\n");
		goto init_fail;
	}
	priv->virtRxBufBaseAddr = (unsigned char *)dma_alloc_coherent(NULL, RX_BUF_SIZE*RX_DESC_NUM, 
			(dma_addr_t *)&priv->phyRxBufBaseAddr, GFP_DMA|GFP_KERNEL);
	if ( priv->virtRxBufBaseAddr == NULL || (priv->phyRxBufBaseAddr & 0x03) ) {
		debug("Allocate the Rx buffer memory fail !\n");
		goto init_fail;
	}

	/* setup the ethernet basic */
	ether_setup(ndev);

	/* reset the MAC */
	mcpu_mac_reset(ndev);
	mcpu_mac_setup_desc_ring(ndev);

	/* set MAC address to dev_addr */
	mtd = get_mtd_device(NULL, 0);
	
	if (mac_num == 0) {
		/* LAN0 */
		if ( mtd == NULL ) {
			memcpy(ndev->dev_addr, (char *)(MOXAART_FLASH_VA_BASE+ARCH_FLASH_MAC0), 6);
		} else {
			 mtd->read(mtd, ARCH_FLASH_MAC0, 6, &len, ndev->dev_addr);
		}
	} else { 
		/* LAN1 */
		if ( mtd == NULL ) {
			memcpy(ndev->dev_addr, (char *)(MOXAART_FLASH_VA_BASE+ARCH_FLASH_MAC1), 6);
		} else {
			mtd->read(mtd, ARCH_FLASH_MAC1, 6, &len, ndev->dev_addr);
		}
	}

	/* irq */
	moxaart_int_set_irq(ndev->irq, LEVEL, H_ACTIVE);
	if (request_irq(ndev->irq, mcpu_mac_interrupt, IRQF_DISABLED, ndev->name, ndev) ) {
		debug("Request interrupt service fail !\n");
		goto init_fail;
	}

	return 0;

init_fail:
	mcpu_mac_free_memory(ndev);
	return -ENOMEM;
}



// Add by Jared 12-04-2008 -------------------------------
#if 1  
static spinlock_t mii_lock = SPIN_LOCK_UNLOCKED; 
int flib_FTMAC100_PHY_Read_Waiting(struct net_device *ndev) {
	u32 wResult;
	unsigned long t;

	for(t = jiffies;;) {
		if((jiffies - t) > (HZ/100) ) {
			printk("<1>%s[%d]timeout\n",__FUNCTION__,__LINE__);
			return (-1);
		}
		wResult = inl(ndev->base_addr+PHYCR_REG_OFFSET);
		if( (wResult & FTMAC100_REG_PHY_READ) == 0 )
			break;
	}
	return 0;
}



int flib_FTMAC100_PHY_Write_Waiting(struct net_device *ndev) {
	u32 wResult;
	unsigned long t;

	for(t = jiffies;;) {
		if((jiffies - t) > (HZ/100) ) {
			printk("<1>%s[%d]\n",__FUNCTION__,__LINE__);
			return (-1);
		}
		wResult = inl(ndev->base_addr+PHYCR_REG_OFFSET);
		if( (wResult & FTMAC100_REG_PHY_WRITE) == 0 )
			break;
	}
	return 0;
}



u32 PhyMiiReadRtn (struct net_device *ndev, u8 phyAddr, u8 phyReg, u16 *value) {
	u32 dwTemp;

	dwTemp= (phyAddr<<16) | ((phyReg & 0x1f) << 21) | FTMAC100_REG_PHY_READ;
	outl(dwTemp, ndev->base_addr + PHYCR_REG_OFFSET);

	if(flib_FTMAC100_PHY_Read_Waiting(ndev) != 0){
		printk("<1>%s[%d]\n",__FUNCTION__,__LINE__);
		return (-1);
	}

	*value = inw(ndev->base_addr + PHYCR_REG_OFFSET);

	return 0;
}



u32 PhyMiiWriteRtn (struct net_device *ndev,  u8 phyAddr, u8 phyReg, u16 value) {
	u32 dwTemp, miiTimeout;

	dwTemp = (phyAddr<<16) | ((phyReg & 0x1f) << 21) | FTMAC100_REG_PHY_WRITE;
	outl(value, ndev->base_addr + PHYWDATA_REG_OFFSET);
	udelay(10) ;

	if(flib_FTMAC100_PHY_Write_Waiting(ndev) != 0){
		printk("<1>%s[%d]\n",__FUNCTION__,__LINE__);
		return (-1);
	}

	for(miiTimeout=10; miiTimeout>0; miiTimeout-- ) {
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout( (1*HZ)/1000 );
	}

	outl(dwTemp, ndev->base_addr + PHYCR_REG_OFFSET);

	if(flib_FTMAC100_PHY_Read_Waiting(ndev) != 0){
		printk("<1>%s[%d]\n",__FUNCTION__,__LINE__);
		return (-2);
	}

	for(miiTimeout=10; miiTimeout>0; miiTimeout-- ) {
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout( (1*HZ)/1000 );
	}

	return 0;
}



#include <linux/ethtool.h>
static int netdev_ethtool_ioctl(struct net_device *ndev, void *useraddr)
{
    u32 dwTemp;
    int ethcmd, res = 0;

    if (copy_from_user (&ethcmd, useraddr, sizeof (ethcmd)))
      return -EFAULT;

    switch (ethcmd) {
      case ETHTOOL_GLINK: {
        struct ethtool_value edata = {ETHTOOL_GLINK};
	
	dwTemp= (1<<16) | ((PHY_STATUS_REG & 0x1f) << 21) | FTMAC100_REG_PHY_READ;
	outl(dwTemp, ndev->base_addr + PHYCR_REG_OFFSET);
	if(flib_FTMAC100_PHY_Read_Waiting(ndev) != 0){
		res = -1;
	}
	dwTemp = inw(ndev->base_addr + PHYCR_REG_OFFSET);

	if ( (dwTemp & Link_Status) == Link_Status)
		edata.data= 1;
	else
		edata.data= 0;

	if (copy_to_user( useraddr, &edata, sizeof(edata)))
	  res= -EFAULT;

	return res;
      }

      default:
        return -EOPNOTSUPP;
    }
}



static int mcpu_do_dev_ioctl(struct net_device *ndev, struct ifreq *req, int cmd)
{
	struct mii_ioctl_data *data = (struct mii_ioctl_data *) & req->ifr_data;
	int res=0;

	switch (cmd)
	{
		/* Get address of MII PHY in use */
		case SIOCGMIIPHY:
		case SIOCDEVPRIVATE:
#ifdef	ONE_MII_Interface
			if(port){
				data->phy_id = 0x2;
			}else{
				data->phy_id = 0x10;
			}
#else
			data->phy_id = 1;
#endif
			break;
		/* Read MII PHY register */
		case SIOCGMIIREG:		
		case SIOCDEVPRIVATE+1:
			spin_lock(&mii_lock);
			if ((res = PhyMiiReadRtn (ndev, data->phy_id, data->reg_num, &data->val_out)))
	    {
				printk("Error reading MII reg %d on phy %d\n",
					data->reg_num, data->phy_id);
				res = -1;
			}
			spin_unlock(&mii_lock);
			break;
		/* Write MII PHY register */
		case SIOCSMIIREG:
		case SIOCDEVPRIVATE+2:
			spin_lock(&mii_lock);
			if ((res = PhyMiiWriteRtn (ndev, data->phy_id, data->reg_num, data->val_in))) {
				printk("Error writing MII reg %d on phy %d\n", data->reg_num, data->phy_id);
				res = -1;
			}
			spin_unlock(&mii_lock);
			break;
		case SIOCETHTOOL:
			return netdev_ethtool_ioctl(ndev, req->ifr_data);
		default:
			res = -EOPNOTSUPP;
	}

	return res;
}

#endif 
// end of add by Jared 12-04-2008-------------------------------------



static struct net_device_ops mcpu_mac_netdev_ops = { 
	.ndo_open	= mcpu_mac_open,			/* open interface, ex run ifup */
	.ndo_stop	= mcpu_mac_stop,			/* ifdown */
	.ndo_start_xmit= mcpu_mac_hard_start_xmit, /* transmit data to hardware */
	.ndo_tx_timeout	= mcpu_mac_tx_timeout,	/* if timeout, restart NIC */
	.ndo_set_multicast_list = mcpu_mac_set_multicast_list,
	.ndo_change_mtu	= eth_change_mtu, 
	.ndo_validate_addr	= eth_validate_addr, 
	.ndo_do_ioctl	= mcpu_do_dev_ioctl,
	.ndo_get_stats	= mcpu_mac_get_stats,	/* get statistic information */
#if 0
	.ndo_set_mac_address = 
 	.name		= ARCH_NAME0; 
	.base_addr	= IO_ADDRESS(MOXAART_FTMAC_BASE),
	.irq		= IRQ_MAC,
	.init		= &mcpu_mac_init,			/* call by register_netdev(). old way */
	.get_stats	= &mcpu_mac_get_stats,	/* get statistic information */
	 private in put in alloc_etherdev
	.priv		= (void *)&mcpu_mac_priv,
#endif
};


#if 0
static int __devinit mcpu_mac_probe(struct platform_device *pdev)
{
	struct net_device *ndev;
	struct resource *res;
	mcpu_mac_priv_t *priv;
	u32 regs_phy_addr = 0;
	u32 regs_len = 0;	
	u32 wakeup_irq = 0;
	int portno = 0;
	int retval = 0;
	u32 gmii_mac_base = 0;

	for ( portno = 0 ; portno < ARCH_DEV_NUM ; portno++ ) {
		printk("MOXAARM11 GMAC Device Driver Version 1.0, GMAC%d Probe: ", portno + 1);
		
		if (!(ndev = alloc_etherdev(sizeof(moxaarm11_gmac_priv_t))))
		return -ENOMEM;
		SET_NETDEV_DEV(ndev, &pdev->ndev);
		moxaarm11_gmac_ndev[portno] = ndev;		//keep net_device pointer for freeing memory in remove fn.
		dbg_printk("ndev %d address: 0x%08x\n",portno, (u32)ndev);		
		
		strcpy( ndev->name,  gmac_interface_name[portno]);
		res = platform_get_resource(pdev, IORESOURCE_IRQ, portno*2);
		if (!res) {
			dev_err(&pdev->ndev,
				"Found GMAC with no IRQ. Check %s setup!\n",
				dev_name(&pdev->ndev));
			retval = -ENODEV;
			goto fail_request_resource;
		}
		ndev->irq = res->start;

		res = platform_get_resource(pdev, IORESOURCE_IRQ, portno*2+1);
		if (!res) {
			dev_err(&pdev->ndev,
				"Found GMAC with no wake IRQ. Check %s setup!\n",
				dev_name(&pdev->ndev));
			retval = -ENODEV;
			goto fail_request_resource;
		}
		wakeup_irq = res->start;
		
		res = platform_get_resource(pdev, IORESOURCE_MEM, portno);
		if (!res) {
			dev_err(&pdev->ndev,
				"Found GMAC with no register addr. Check %s setup!\n", dev_name(&pdev->ndev));
			retval = -ENODEV;
			goto fail_request_resource;
		}

		ndev->mem_start = regs_phy_addr = res->start;
		ndev->mem_end = res->end;
		regs_len = res->end - res->start + 1;
		if (!request_mem_region(regs_phy_addr , regs_len, res->name)) {
			dev_err(&pdev->ndev, "controller already in use\n");
			retval = -EBUSY;
			goto fail_request_resource;
		}

		ndev->base_addr= (unsigned long)ioremap_nocache(regs_phy_addr, regs_len);
		if ( (void *)ndev->base_addr == NULL ) {
			dev_err(&pdev->ndev, "error mapping memory\n");
			retval = -EFAULT;
			goto fail_ioremap;
		}

		ndev->netdev_ops = (struct net_device_ops *)&moxaarm11_netdev_gmac_ops;
		
		retval = register_netdev(ndev);
		if ( retval ) {
			dev_err(&pdev->ndev, "registeration failed!\n");
			goto fail_register;
		}
		priv = netdev_priv(ndev);
		priv->wakeup_irq = wakeup_irq;
		//Key Point: The two MDC/MDIO interfaces both utilize GMAC1 controller to access phy chip.		
		if(portno == 0)
			gmii_mac_base = ndev->base_addr;
		priv->gmii_mac_base = gmii_mac_base;
		priv->phyid = portno+1;
		priv->mii_if.phy_id = priv->phyid;
		
		moxaarm11_io_pin_select(GMAC_AUX_TIMESTAMP_TRIG1, 1);
#ifdef CONFIG_MOXAARM11_GMAC_NAPI
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
#endif


static struct net_device *ndev[ARCH_DEV_NUM]; 
static int __init mcpu_mac_init_module(void)
{
	int ret = 0;
	int i = 0;

	printk("Load Moxa ART Ethernet Device Driver Version 1.0");

	for (i = 0; i < ARCH_DEV_NUM; i++) {

		/* alloc: this call ether_setup automatically */
		ndev[i] = alloc_etherdev(sizeof(mcpu_mac_priv_t)); 

		if (!ndev[i]) {
			printk("*** Can't allocate net device!\n");
			return -ENOMEM;
		}

		/* assign resource */
		if (i == 0) {
			ndev[i]->base_addr = IO_ADDRESS(MOXAART_FTMAC_BASE); 
			ndev[i]->irq = IRQ_MAC; 
			strncpy(ndev[i]->name, ARCH_NAME0, sizeof(ndev[i]->name)); 
		} else if (i == 1) {
			ndev[i]->base_addr = IO_ADDRESS(MOXAART_FTMAC2_BASE); 
			ndev[i]->irq = IRQ_MAC2; 
			strncpy(ndev[i]->name, ARCH_NAME1, sizeof(ndev[i]->name)); 
		} else {
			printk("*** Incorrect resource!\n");
			return -1; 
		}

		/* initiilze DMA / MAC address / IRQ */
		mcpu_mac_init(ndev[i], i); 

		ndev[i]->netdev_ops = &mcpu_mac_netdev_ops; 
		ndev[i]->ethtool_ops = NULL; /* user space support */

		ret = register_netdev(ndev[i]);
		if (ret) {
			printk("*** Regiser netdev fail!\n");
			return ret;
		}
	}

	printk("Register Ethernet ok.\n");
	return 0;
}



static void __exit mcpu_mac_cleanup_module(void)
{
	int i;

	printk("Moxa CPU Ethernet Device Driver unload.\n");
	for (i = 0; i < ARCH_DEV_NUM; i++) {
		mcpu_mac_free_memory(ndev[i]);
		free_irq(ndev[i]->irq, ndev[i]);
		unregister_netdev(ndev[i]);
		free_netdev(ndev[i]);
	}
}
module_init(mcpu_mac_init_module);
module_exit(mcpu_mac_cleanup_module);

MODULE_DESCRIPTION("MOXA ART MAC Driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChinFu Yang");
	
