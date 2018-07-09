
/*
 * This program is the Moxa CPU ethernet device driver.
 *
 * History:
 * Date		Author			Comment
 * 06-15-2005	Victor Yu.		Create it. Make it for Faraday demo board.
 * 11-04-2005	Victor Yu.		Modify it to support Moxa CPU demo board.
 * 09-20-2011   	Wade Liang	Refer to bfin_mac.c for new structure
 * 10-11-2011	ChinFu Yang	Port it to Linux-2.6.38 with platform driver model.
 */

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
#include <linux/platform_device.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/bitops.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/dma.h>
#include <linux/mtd/mtd.h>
#include <mach/moxaart_int.h>
#include <mach/hardware.h>
#include <linux/ethtool.h>
#include <linux/mii.h>
#include <linux/sched.h>
#include "moxacpu_mac.h"

//#define USE_SCHEDULE_WORK
#define HAVE_MULTICAST


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



int flib_ftmac100_phy_read_waiting(struct net_device *ndev)
{
	u32 wResult;
	unsigned long t;

	for(t = jiffies;;) {
		if((jiffies - t) > (HZ/100) ) {
			printk("<1>%s[%d]timeout\n",__func__,__LINE__);
			return (-1);
		}
		wResult = inl(ndev->base_addr+PHYCR_REG_OFFSET);
		if( (wResult & FTMAC100_REG_PHY_READ) == 0 )
			break;
	}

	return 0;
}



int flib_ftmac100_phy_write_waiting(struct net_device *ndev)
{
	u32 wResult;
	unsigned long t;


	for(t = jiffies;;) {
		if((jiffies - t) > (HZ/100) ) {
			printk("<1>%s[%d]\n",__func__,__LINE__);
			return (-1);
		}
		wResult = inl(ndev->base_addr+PHYCR_REG_OFFSET);
		if( (wResult & FTMAC100_REG_PHY_WRITE) == 0 )
			break;
	}
	
	return 0;
}



int moxaart_mac_phy_read_reg (struct net_device *ndev, int phyAddr, int phyReg) 
{
	u32 dwTemp = 0;

	dwTemp= (phyAddr<<16) | ((phyReg & 0x1f) << 21) | FTMAC100_REG_PHY_READ;
	outl(dwTemp, ndev->base_addr + PHYCR_REG_OFFSET);
	if(flib_ftmac100_phy_read_waiting(ndev) != 0){
		printk("*** <1>%s[%d]\n",__func__,__LINE__);
		return (-1);
	}

	dwTemp = inw(ndev->base_addr + PHYCR_REG_OFFSET);	
	return dwTemp;
}





void moxaart_mac_phy_write_reg (struct net_device *ndev,  int phyAddr, int phyReg, int value) 
{
	u32 dwTemp, miiTimeout;
	dwTemp = (phyAddr<<16) | ((phyReg & 0x1f) << 21) | FTMAC100_REG_PHY_WRITE;
	outl(value, ndev->base_addr + PHYWDATA_REG_OFFSET);
	udelay(10) ;

	if(flib_ftmac100_phy_write_waiting(ndev) != 0){
		printk("*** <1>%s[%d]\n",__func__,__LINE__);
		return ;
	}

	for(miiTimeout=10; miiTimeout>0; miiTimeout-- ) {
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout( (1*HZ)/1000 );
	}

	outl(dwTemp, ndev->base_addr + PHYCR_REG_OFFSET);

	if(flib_ftmac100_phy_read_waiting(ndev) != 0){
		printk("*** <1>%s[%d]\n",__func__,__LINE__);
		return ;
	}

	for(miiTimeout=10; miiTimeout>0; miiTimeout-- ) {
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout( (1*HZ)/1000 );
	}
}


void inline moxaart_mac_full_duplex(struct net_device *ndev)
{
	u32	reg = ndev->base_addr+MACCR_REG_OFFSET;
	writel( readl((void *)reg) | FULLDUP, (void *)reg);
	printk("Set MAC: Full Duplex mode\n");	
}
void inline moxaart_mac_half_duplex(struct net_device *ndev)
{
	u32	reg = ndev->base_addr+MACCR_REG_OFFSET;
	writel( readl((void *)reg) & ~FULLDUP, (void *)reg);
	printk("Set MAC: Half Duplex mode\n");	
}

void inline moxaart_mac_set_speed_100M(struct net_device *ndev)
{
	printk("Set MAC: 100Mbps mode\n");	
}
void inline moxaart_mac_set_speed_10M(struct net_device *ndev)
{
	printk("Set MAC: 10Mbps mode\n");	
}



static void moxaart_mac_setmulticast(struct net_device *ndev)
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


static void moxaart_mac_set_multicast_list(struct net_device *ndev)
{
	moxaart_mac_priv_t *priv = (moxaart_mac_priv_t *)netdev_priv(ndev); 
	unsigned long	flags;

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
		moxaart_mac_setmulticast(ndev);
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



s32 moxaart_mac_set_rx_desc(moxaart_mac_priv_t * priv)
{
	rx_desc_t *rxdesc = &priv->virtRxDescBaseAddr[priv->RxDescNow];

	rxdesc->rxdes1.ubit.RxBufSize = RX_BUF_SIZE;
	rxdesc->rxdes2.Buffer1AddressPointer = priv->phyRxBufBaseAddr[priv->RxDescNow];

	rxdesc->rxdes0.ubit.RxDMAOwn = 1;

	dbg_mac("RxDescNow: %02d rxdesc: 0x%08x \n", priv->RxDescNow , (u32)rxdesc);

	priv->RxDescNow++;
	priv->RxDescNow &= RX_DESC_NUM_MASK;

	return priv->RxDescNow;
}




static void moxaart_mac_setup_desc_ring(struct net_device *ndev)
{
	moxaart_mac_priv_t *priv=(moxaart_mac_priv_t *)netdev_priv(ndev); 
	int	i;
	tx_desc_t *txdesc;
	rx_desc_t *rxdesc;
	unsigned char *virtbuf_addr;
	unsigned int phybuf_addr;


	// 1. TX
	virtbuf_addr = priv->virtTxBufBaseAddr;
	phybuf_addr = priv->phyTxBufBaseAddr;
	for ( i = 0 ; i < TX_DESC_NUM ; i++, virtbuf_addr += TX_BUF_SIZE , phybuf_addr += TX_BUF_SIZE ) {
		txdesc = &priv->virtTxDescBaseAddr[i];
		memset(txdesc, 0, sizeof(tx_desc_t));
		txdesc->txdes2.phyTxBufBaseAddr = phybuf_addr;
		txdesc->txdes2.virtTxBufBaseAddr = virtbuf_addr;
	}
	priv->virtTxDescBaseAddr[TX_DESC_NUM-1].txdes1.ubit.Edotr = 1;


	// 2. RX
	for ( i = 0 ; i < RX_DESC_NUM ; i++) {
		rxdesc = &priv->virtRxDescBaseAddr[i];
	//dbg_printk("moxaarm11_gmac_setup_desc_ring: rxdesc: 0x%08x \n", rxdesc);		
		memset(rxdesc, 0, sizeof(rx_desc_t));
		rxdesc->rxdes1.ubit.RxBufSize = RX_BUF_SIZE;
		rxdesc->rxdes2.Buffer1AddressPointer = priv->phyRxBufBaseAddr[i];
		rxdesc->rxdes0.ubit.RxDMAOwn = 1;
	}
	priv->virtRxDescBaseAddr[RX_DESC_NUM-1].rxdes1.ubit.Edorr = 1;

	priv->RxDescNow = 0;
	priv->RxDescBusy = 0;
	priv->TxDescNow = 0;
	priv->TxDescBusy = 0;
	
	// reset the MAC controler Tx/Rx desciptor base address
	outl(priv->phyTxDescBaseAddr, ndev->base_addr+TXR_BADR_REG_OFFSET);
	outl(priv->phyRxDescBaseAddr, ndev->base_addr+RXR_BADR_REG_OFFSET);
}



static void moxaart_mac_reset(struct net_device *ndev)
{
	unsigned int reg = ndev->base_addr+MACCR_REG_OFFSET;
	
	dbg_mac("mac reset\r\n");
	outl(SW_RST, reg);	// software reset
	while (inl(reg) & SW_RST) mdelay(10);
	// maybe we need to disable the all interrupt
	outl(0, ndev->base_addr+IMR_REG_OFFSET);
#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE) //johnson modify for 802.1Q support 2008-11-14
	((moxaart_mac_priv_t *)netdev_priv(ndev))->maccr = RX_BROADPKT | ENRX_IN_HALFTX | CRC_APD | RX_FTL;
#else
	((moxaart_mac_priv_t *)netdev_priv(ndev))->maccr = RX_BROADPKT | ENRX_IN_HALFTX | CRC_APD;
#endif
}



static void moxaart_mac_set_mac_address(unsigned int base, unsigned char *macaddr)
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



#ifdef DEBUG_MOXAART_MAC	// add by Victor Yu. 03-14-2006
static void moxaart_mac_get_mac_address(unsigned int base, unsigned char *macaddr)
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



static void moxaart_mac_enable(struct net_device *ndev)
{
	moxaart_mac_priv_t *priv = (moxaart_mac_priv_t *)netdev_priv(ndev); 
	unsigned int base=ndev->base_addr;

	outl(0x00001010, base+ITC_REG_OFFSET);
	outl(0x00000001, base+APTC_REG_OFFSET);
	outl(0x00000390, base+DBLAC_REG_OFFSET);
#ifdef DEBUG_MOXAART_MAC
	outl(RPKT_FINISH_M|NORXBUF_M|AHB_ERR_M, base+IMR_REG_OFFSET);
#else
	outl(RPKT_FINISH_M, base+IMR_REG_OFFSET);
#endif
	priv->maccr |= (RCV_EN | XMT_EN | RDMA_EN | XDMA_EN);
	outl(priv->maccr, base+MACCR_REG_OFFSET);
}



static void moxaart_mac_tx_timeout(struct net_device *ndev)
{
	moxaart_mac_priv_t *priv=(moxaart_mac_priv_t *)netdev_priv(ndev); 
	unsigned long	flags;

	local_irq_save(flags);
#ifdef USE_SCHEDULE_WORK
	spin_lock(&priv->rxlock);
#endif
	spin_lock(&priv->txlock);
	moxaart_mac_reset(ndev);
	/* read mac addr, and set to dev->dev_addr */
	moxaart_mac_set_mac_address(ndev->base_addr+MAC_MADR_REG_OFFSET, ndev->dev_addr);
	moxaart_mac_setup_desc_ring(ndev);
	moxaart_mac_enable(ndev);
	spin_unlock(&priv->txlock);
#ifdef USE_SCHEDULE_WORK
	spin_unlock(&priv->rxlock);
#endif
	local_irq_restore(flags);
	netif_wake_queue(ndev);
	ndev->trans_start = jiffies;
}



static int moxaart_mac_hard_start_xmit(struct sk_buff *skb, struct net_device *ndev)
{
	moxaart_mac_priv_t *priv = (moxaart_mac_priv_t *)netdev_priv(ndev); 
	tx_desc_t *txdesc;
	int len;

	spin_lock(&priv->txlock);

	// first check the Tx buffer is enough or not
	txdesc = &priv->virtTxDescBaseAddr[priv->TxDescNow];
	if ( txdesc->txdes0.ubit.TxDMAOwn ) {
		dbg_mac("No Tx space to transmit the packet !\n");
		priv->stats.tx_dropped++;
		goto xmit_final;
	}

	len = skb->len > TX_BUF_SIZE ? TX_BUF_SIZE : skb->len;
	
	txdesc->txdes2.phyTxBufBaseAddr = dma_map_single(&ndev->dev, (void *)skb->data, len, DMA_TO_DEVICE);
	
	dbg_mac("transmit  priv->TxDescNow = %d ; data pointer = 0x%.8lx\n",priv->TxDescNow,  (unsigned long)skb->data);
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
	(priv->TxDescNow)++;
	priv->TxDescNow &= TX_DESC_NUM_MASK;
	ndev->trans_start = jiffies;
	priv->stats.tx_packets++;
	priv->stats.tx_bytes += len;

xmit_final:
	spin_unlock(&priv->txlock);
	dev_kfree_skb_any(skb);	// skb from upper layer.

	return 0;
}



/* call by ISR */
static void moxaart_mac_recv(void *ptr)
{
	struct net_device	*ndev=(struct net_device *)ptr;
	moxaart_mac_priv_t *priv=(moxaart_mac_priv_t *)netdev_priv(ndev);
	rx_desc_t *rxdesc;
	int len;
	struct sk_buff *skb;
	unsigned char *data;
	unsigned int ui;

	
#ifndef USE_SCHEDULE_WORK
	int loops=RX_DESC_NUM;
#endif	// USE_SCHEDULE_WORK


#ifdef USE_SCHEDULE_WORK
	spin_lock(&priv->rxlock);
#endif	// USE_SCHEDULE_WORK

repeat_recv:
	rxdesc = &priv->virtRxDescBaseAddr[priv->RxDescNow];
	ui = rxdesc->rxdes0.ui;
	if ( ui & RXDMA_OWN ) {
#ifdef USE_SCHEDULE_WORK
		spin_unlock(&priv->rxlock);
#else
#ifdef DEBUG_MOXAART_MAC
		if ( loops == RX_DESC_NUM ){
			printk("*** Bad receive packet ! RXDMA_OWN == 0\n");
		}
#endif	
#endif	// USE_SCHEDULE_WORK
		return;
	}
	
#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)
	if ( ui & (RX_ERR|CRC_ERR|RUNT|RX_ODD_NB) ) {
#else
	if ( ui & (RX_ERR|CRC_ERR|FTL|RUNT|RX_ODD_NB) ) {
#endif
		dbg_mac("*** Ethernet receive packet error !\n");
		priv->stats.rx_dropped++;
		priv->stats.rx_errors++;
		goto recv_finish;
	}
	len = ui & RFL_MASK;
	if ( len > RX_BUF_SIZE ){
		len = RX_BUF_SIZE;
	}

	skb = (struct sk_buff *)priv->virtRxSkbBufBaseAddr[priv->RxDescNow];
	if ( skb == NULL  || skb->head == NULL ) {
		printk("*** RX skb is NULL!\n");
		goto recv_finish;	
	}


	data=skb_put(skb , len);
	data = skb_tail_pointer(skb)- len;
	dbg_mac("receive data pointer = 0x%x len: %d data: \n\n", (u32)data, len);
	skb->dev = ndev;
	skb->protocol = eth_type_trans(skb, ndev);
	
	if(netif_rx(skb) == NET_RX_DROP){
		printk("*** netif RX error! \r\n");
	}
	priv->stats.rx_packets++;
	priv->stats.rx_bytes += len;
	if ( ui & MULTICAST_RXDES0 ){
		priv->stats.multicast++;
	}
	dbg_mac("Receive a good packet.\n");

	//Now lets allocate the skb for the emptied descriptor
	priv->virtRxSkbBufBaseAddr[priv->RxDescNow] = dev_alloc_skb(RX_BUF_SIZE);
	if(priv->virtRxSkbBufBaseAddr[priv->RxDescNow] == NULL){
		printk("*** RX recv() skb memory allocation failed \n");
		priv->stats.rx_dropped++;
	}
	priv->phyRxBufBaseAddr[priv->RxDescNow] = dma_map_single(&ndev->dev, (void *)priv->virtRxSkbBufBaseAddr[priv->RxDescNow]->data, priv->virtRxSkbBufBaseAddr[priv->RxDescNow]->len, DMA_FROM_DEVICE);
	priv->RxDescNow = moxaart_mac_set_rx_desc(priv);


recv_finish:
	dbg_mac("IN recv_finish: \r\n");
	rxdesc->rxdes0.ui = RXDMA_OWN;


#ifdef USE_SCHEDULE_WORK
	goto repeat_recv;
#else	// USE_SCHEDULE_WORK
	if ( loops-- > 0 ){
		goto repeat_recv;
	}	
#endif	// USE_SCHEDULE_WORK

#ifdef USE_SCHEDULE_WORK
	spin_unlock(&priv->rxlock);
#endif	// USE_SCHEDULE_WORK


}



void  moxaart_gmac_take_rx_desc_ownership(struct net_device *ndev)
{
	moxaart_mac_priv_t *priv=netdev_priv(ndev);
	s32 i;
	rx_desc_t *desc;
	desc = priv->virtRxDescBaseAddr;
	for(i = 0; i < RX_DESC_NUM; i++){
		desc->rxdes0.ubit.RxDMAOwn = 0;
		desc++;		
	}
}

void  moxaart_gmac_take_tx_desc_ownership(struct net_device *ndev)
{
	moxaart_mac_priv_t *priv=netdev_priv(ndev);
	s32 i;
	tx_desc_t *desc;
	desc = priv->virtTxDescBaseAddr;
	
	for(i = 0; i < TX_DESC_NUM; i++){
		desc->txdes0.ubit.TxDMAOwn = 0;
		desc ++;		
	}

}


static int moxaart_mac_open(struct net_device *ndev)
{

	moxaart_mac_priv_t *priv=(moxaart_mac_priv_t *)netdev_priv(ndev);
	unsigned long flags;

	local_irq_save(flags);

#ifdef USE_SCHEDULE_WORK
	spin_lock(&priv->rxlock);
#endif
	spin_lock(&priv->txlock);

	moxaart_mac_reset(ndev);
	
	/* read mac address, set to dev_addr */
	moxaart_mac_set_mac_address(ndev->base_addr+MAC_MADR_REG_OFFSET, ndev->dev_addr);

	moxaart_mac_setup_desc_ring(ndev);
	moxaart_mac_enable(ndev);

	spin_unlock(&priv->txlock);

#ifdef USE_SCHEDULE_WORK
	spin_unlock(&priv->rxlock);
#endif

	local_irq_restore(flags);

	/* allow device to transmit package*/
	netif_start_queue(ndev);
	dbg_mac("IMR=0x%x, MACCR=0x%x\n", inl(ndev->base_addr+IMR_REG_OFFSET), inl(ndev->base_addr+MACCR_REG_OFFSET));

	
#ifdef DEBUG_MOXAART_MAC
	{
		unsigned char	macaddr[6];
		int		i = 0;
		moxaart_mac_get_mac_address(ndev->base_addr+MAC_MADR_REG_OFFSET , macaddr);
		printk("Get MAC address = ");
		for ( i=0; i<6; i++ ){
			printk("%02X ", macaddr[i]);
		}
		printk("\n");
	}
#endif
	return 0;
}


static int moxaart_mac_stop(struct net_device *ndev)
{
	moxaart_mac_priv_t *priv=(moxaart_mac_priv_t *)netdev_priv(ndev); 
	unsigned long		flags;

	dbg_mac("mcpu_mac_stop test01\n");

	local_irq_save(flags);
#ifdef USE_SCHEDULE_WORK
	spin_lock(&priv->rxlock);
#endif
	spin_lock(&priv->txlock);

	outl(0, ndev->base_addr+IMR_REG_OFFSET);		// disable all interrupt
	outl(0, ndev->base_addr+MACCR_REG_OFFSET);	// disable all function

	/* disable transmit package */
	netif_stop_queue(ndev);
	spin_unlock(&priv->txlock);
#ifdef USE_SCHEDULE_WORK
	spin_unlock(&priv->rxlock);
#endif
	local_irq_restore(flags);

	return 0;
}



static struct net_device_stats *moxaart_mac_get_stats(struct net_device *ndev)
{
	moxaart_mac_priv_t *priv=(moxaart_mac_priv_t *)netdev_priv(ndev); 

	return &priv->stats;
}



static irqreturn_t moxaart_mac_isr(int irq, void *dev_id)
{
	struct net_device	*ndev=(struct net_device *)dev_id;
	unsigned int		ists;
#ifdef USE_SCHEDULE_WORK
	moxaart_mac_priv_t		*priv=(moxaart_mac_priv_t *)netdev_priv(ndev); 
#endif


	ists = inl(ndev->base_addr+ISR_REG_OFFSET);
	if ( ists & RPKT_FINISH ) {
#ifdef USE_SCHEDULE_WORK
		schedule_work(&priv->rqueue);
#else
		moxaart_mac_recv((void *)ndev);
#endif
	} else {
#ifdef DEBUG_MOXAART_MAC
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



static void moxaart_mac_free_memory(struct net_device *ndev)
{
	int i;
	moxaart_mac_priv_t *priv=(moxaart_mac_priv_t *)netdev_priv(ndev); 

	if (priv->virtTxDescBaseAddr){
		dma_free_coherent(NULL, sizeof(tx_desc_t) * TX_DESC_NUM, priv->virtTxDescBaseAddr, priv->phyTxDescBaseAddr);
	}
	if (priv->virtRxDescBaseAddr){
		dma_free_coherent(NULL, sizeof(rx_desc_t) * RX_DESC_NUM, priv->virtRxDescBaseAddr, priv->phyRxDescBaseAddr);
	}
	if (priv->virtTxBufBaseAddr){
		dma_free_coherent(NULL, TX_BUF_SIZE * TX_DESC_NUM, priv->virtTxBufBaseAddr, priv->phyTxBufBaseAddr);
	}

	for( i = 0 ; i < RX_DESC_NUM ; i++ ){
		if (priv->virtRxSkbBufBaseAddr[i])
			kfree_skb(priv->virtRxSkbBufBaseAddr[i]);
	}
}




static int moxaart_mac_init(struct net_device *ndev)
{
	struct mtd_info *mtd;
	int len = 0;
	int i = 0;

	moxaart_mac_priv_t *priv=(moxaart_mac_priv_t *)netdev_priv(ndev); 

	memset((void *)priv, 0, sizeof(moxaart_mac_priv_t));
	spin_lock_init(&priv->txlock);

#ifdef USE_SCHEDULE_WORK
	spin_lock_init(&priv->rxlock);
	INIT_WORK(&priv->rqueue, &moxaart_mac_recv);
#endif

	/* allocate DMA for the descriptor and buffer memory */
	priv->virtTxDescBaseAddr = (tx_desc_t *)dma_alloc_coherent(NULL, sizeof(tx_desc_t) * TX_DESC_NUM, 
			(dma_addr_t *)&priv->phyTxDescBaseAddr, GFP_DMA|GFP_KERNEL);	
	if ( priv->virtTxDescBaseAddr == NULL || (priv->phyTxDescBaseAddr & 0x0f) ) {
		dbg_mac("*** Allocate the Tx descriptor memory fail !\n");
		goto init_fail;
	}

	priv->virtRxDescBaseAddr = (rx_desc_t *)dma_alloc_coherent(NULL, sizeof(rx_desc_t) * RX_DESC_NUM, 
			(dma_addr_t *)&priv->phyRxDescBaseAddr, GFP_DMA|GFP_KERNEL);
	if ( priv->virtRxDescBaseAddr == NULL || (priv->phyRxDescBaseAddr & 0x0f) ) {
		dbg_mac("Allocate the Rx descriptor memory fail !\n");
		goto init_fail;
	}

	priv->virtTxBufBaseAddr = (unsigned char *)dma_alloc_coherent(NULL, TX_BUF_SIZE * TX_DESC_NUM, 
			(dma_addr_t *)&priv->phyTxBufBaseAddr, GFP_DMA|GFP_KERNEL);
	if ( priv->virtTxBufBaseAddr == NULL || (priv->phyTxBufBaseAddr & 0x03) ) {
		dbg_mac("Allocate the Tx buffer memory fail !\n");
		goto init_fail;
	}




	// Prepare RX skb.
	for(i=0;i<RX_DESC_NUM;i++){
		priv->virtRxSkbBufBaseAddr[i]=dev_alloc_skb(RX_BUF_SIZE);	
		priv->phyRxBufBaseAddr[i]= dma_map_single(&ndev->dev, (void *)priv->virtRxSkbBufBaseAddr[i]->data, priv->virtRxSkbBufBaseAddr[i]->len, DMA_FROM_DEVICE);
		if ( priv->virtRxSkbBufBaseAddr[i] == NULL || (priv->phyRxBufBaseAddr[i] & 0x03) ) {
			printk("*** Allocate the Rx buffer memory fail !\n");
			goto init_fail;
		}
	}



	/* setup the ethernet basic */
	ether_setup(ndev);

	/* reset the MAC */
	moxaart_mac_reset(ndev);
	moxaart_mac_setup_desc_ring(ndev);

	/* set MAC address to dev_addr from mtd */
	mtd = get_mtd_device(NULL, 0);


	if ( mtd == NULL ) {
		if(priv->mac_num == 0){
			memcpy(ndev->dev_addr, (char *)(MOXAART_FLASH_VA_BASE+CONFIG_ARCH_FLASH_MAC0), 6);
		}else{
			memcpy(ndev->dev_addr, (char *)(MOXAART_FLASH_VA_BASE+CONFIG_ARCH_FLASH_MAC1), 6);
		}
	} else {
		if(priv->mac_num == 0){
		 	mtd->read(mtd, CONFIG_ARCH_FLASH_MAC0, 6, &len, ndev->dev_addr);
		}else{
			mtd->read(mtd, CONFIG_ARCH_FLASH_MAC1, 6, &len, ndev->dev_addr);
		}
	}

	priv->mii_if.dev = ndev;
	priv->mii_if.mdio_read = moxaart_mac_phy_read_reg;
	priv->mii_if.mdio_write = moxaart_mac_phy_write_reg;
	priv->mii_if.phy_id_mask = 0x1f;
	priv->mii_if.reg_num_mask = 0x1f;


	/* irq */
	moxaart_int_set_irq(ndev->irq, LEVEL, H_ACTIVE);
	if (request_irq(ndev->irq, moxaart_mac_isr, IRQF_DISABLED, ndev->name, ndev) ) {
		dbg_mac("Request interrupt service fail !\n");
		goto init_fail;
	}

	return 0;

init_fail:
	moxaart_mac_free_memory(ndev);
	return -ENOMEM;
}




#include <linux/ethtool.h>
static int moxaart_netdev_ethtool_ioctl(struct net_device *ndev, void *useraddr)
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
	if(flib_ftmac100_phy_read_waiting(ndev) != 0){
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

static int moxaart_do_dev_ioctl(struct net_device *ndev, struct ifreq *req, int cmd)
{
	moxaart_mac_priv_t *priv = netdev_priv(ndev);
	struct mii_ioctl_data *mii_data=if_mii(req);
	int res = 0;
	
	switch (cmd){
		/* Get address of MII PHY in use */
		case SIOCGMIIPHY:
		case SIOCDEVPRIVATE:
			mii_data->phy_id = priv->mii_if.phy_id;
			break;
		/* Read MII PHY register */
		case SIOCGMIIREG:		
		case SIOCDEVPRIVATE+1:
			spin_lock(&priv->mii_lock);
			res = generic_mii_ioctl(&priv->mii_if, if_mii(req), cmd, NULL);
			spin_unlock(&priv->mii_lock);
			break;
		/* Write MII PHY register */
		case SIOCSMIIREG:
		case SIOCDEVPRIVATE+2:
			spin_lock(&priv->mii_lock);
			res = generic_mii_ioctl(&priv->mii_if, if_mii(req), cmd, NULL);
			spin_unlock(&priv->mii_lock);

			if (!capable(CAP_NET_ADMIN)){
				return -EPERM;	
			}

                   	switch(mii_data->reg_num) {
				case MII_BMCR:
					if (priv->mii_if.force_media && (mii_data->val_in & BMCR_FULLDPLX)){
						moxaart_mac_full_duplex(ndev);
					}else if (priv->mii_if.force_media && !(mii_data->val_in & BMCR_FULLDPLX)){
						moxaart_mac_half_duplex(ndev);					
					}

					if (priv->mii_if.force_media && (mii_data->val_in & BMCR_SPEED100)){
						moxaart_mac_set_speed_100M(ndev);
					}else if (priv->mii_if.force_media && !(mii_data->val_in & BMCR_SPEED100)){
						moxaart_mac_set_speed_10M(ndev);	   
					}

					break;
				default:
					printk("MII no match ...\r\n");
					break;
                   	}

			break;
		case SIOCETHTOOL:
			return moxaart_netdev_ethtool_ioctl(ndev, req->ifr_data);
		default:
			res = -EOPNOTSUPP;
	}

	return res;

}

static struct net_device_ops moxaart_mac_netdev_ops = { 
	.ndo_init		= moxaart_mac_init,					
	.ndo_open		= moxaart_mac_open,			/* open interface, ex run ifup */
	.ndo_stop		= moxaart_mac_stop,			/* ifdown */
	.ndo_start_xmit		= moxaart_mac_hard_start_xmit, /* transmit data to hardware */
	.ndo_tx_timeout		= moxaart_mac_tx_timeout,	/* if timeout, restart NIC */
	.ndo_set_multicast_list	= moxaart_mac_set_multicast_list,
	.ndo_change_mtu		= eth_change_mtu, 
	.ndo_validate_addr	= eth_validate_addr, 
	.ndo_do_ioctl		= moxaart_do_dev_ioctl,
	.ndo_get_stats		= moxaart_mac_get_stats,	/* get statistic information */
};

static char mac_interface_name[ARCH_DEV_MAX_NUM][IFNAMSIZ]= {CONFIG_MOXAART_MAC_INTERFACE_NAME0, CONFIG_MOXAART_MAC_INTERFACE_NAME1};
static struct net_device *moxaart_ndev[CONFIG_MOXAART_MAC_NUM]; 
static int __devinit moxaart_mac_probe(struct platform_device *pdev)
{
	struct net_device	*ndev;
	struct resource		*res;
	moxaart_mac_priv_t	*priv;
	u32			regs_phy_addr=0;
	u32			regs_len=0;	
	int			portno, retval=0;

	for ( portno = 0 ; portno < CONFIG_MOXAART_MAC_NUM ; portno++ ) {
		printk("MOXA ART Ethernet Device Driver Version 1.0, MAC%d Probe: ", portno);
		
		if (!(ndev = alloc_etherdev(sizeof(moxaart_mac_priv_t)))){
			return -ENOMEM;
		}

		SET_NETDEV_DEV(ndev, &pdev->dev);

		moxaart_ndev[portno] = ndev;		//keep net_device pointer for freeing memory in remove fn.
		dbg_mac("ndev %d address: 0x%.8x\n",portno, (u32)ndev);		
		
		strcpy( ndev->name,  mac_interface_name[portno]);
		res = platform_get_resource(pdev, IORESOURCE_IRQ, portno);
		if (!res) {
			dev_err(&pdev->dev,	"Found GMAC with no IRQ. Check %s setup!\n",	dev_name(&pdev->dev));
			retval = -ENODEV;
			goto fail_request_resource;
		}
		ndev->irq = res->start;

	
		res = platform_get_resource(pdev, IORESOURCE_MEM, portno);
		if (!res) {
			dev_err(&pdev->dev,	"Found GMAC with no register addr. Check %s setup!\n", dev_name(&pdev->dev));
			retval = -ENODEV;
			goto fail_request_resource;
		}
		ndev->mem_start = regs_phy_addr = res->start;
		ndev->mem_end = res->end;
		regs_len = res->end - res->start + 1;
		if (!request_mem_region(ndev->mem_start , regs_len, res->name)) {
			dev_err(&pdev->dev, "controller already in use\n");
			retval = -EBUSY;
			goto fail_request_resource;
		}

		ndev->base_addr= (unsigned long)ioremap_nocache(regs_phy_addr, regs_len);
		if ( (void *)ndev->base_addr == NULL ) {
			dev_err(&pdev->dev, "error mapping memory from ioremap_nocache()\n");
			retval = -EFAULT;
			goto fail_ioremap;
		}

		ndev->netdev_ops = (struct net_device_ops *)&moxaart_mac_netdev_ops;
		
		retval = register_netdev(ndev);
		if ( retval ) {
			dev_err(&pdev->dev, "*** Ethernet registeration failed!\n");
			goto fail_register;
		}
		priv = netdev_priv(ndev);

		if(portno == 0){
			priv->mii_if.phy_id = CONFIG_MOXAART_MAC_PHY_ADDR0;
		}else{
			priv->mii_if.phy_id = CONFIG_MOXAART_MAC_PHY_ADDR1;
		}
		priv->mii_lock = SPIN_LOCK_UNLOCKED;
		priv->mac_num = portno;
		
		
		printk("successed. \n");
		dbg_mac("Device Name: %s \n", ndev->name);				
		printk("Virtual Address: 0x%08x \n", (u32)ndev->base_addr);
		printk("phyid = %d\r\n" , priv->mii_if.phy_id);
		
	}
	return 0;

fail_register:
	iounmap((unsigned long*)ndev->base_addr);
fail_ioremap:
	release_mem_region(regs_phy_addr, regs_len);
fail_request_resource:	
	free_netdev(ndev);
	printk("failed. \n");
	return retval;
}



static int  moxaart_mac_remove(struct platform_device *pdev)
{
	int portno = 0;
	moxaart_mac_priv_t *priv;

	dbg_mac("moxaart_mac_remove(): \n");

	for ( portno = 0 ; portno < CONFIG_MOXAART_MAC_NUM ; portno++ ) {
		dbg_mac("%s: ndev %d address: 0x%08x\n",__func__, portno, (u32)moxaart_ndev[portno]);		

		priv = netdev_priv(moxaart_ndev[portno]);

		if(priv->mac_stop != 1){
			moxaart_ndev[portno]->netdev_ops->ndo_stop(moxaart_ndev[portno]);
		}
		
		moxaart_mac_free_memory(moxaart_ndev[portno]);
		iounmap((void *)moxaart_ndev[portno]->base_addr);		
		release_mem_region(moxaart_ndev[portno]->mem_start, 
								moxaart_ndev[portno]->mem_end - moxaart_ndev[portno]->mem_start + 1);
		
		unregister_netdev(moxaart_ndev[portno]);		
	}


	return 0;
}



static struct platform_driver moxaart_mac_driver = {
	.driver.name	= DRV_NAME,
	.probe		= moxaart_mac_probe,
	.remove		= moxaart_mac_remove,
};



static int __init moxaart_mac_module_init(void)
{
	printk("%s \n",__FUNCTION__);
	return platform_driver_register(&moxaart_mac_driver);
}

static void __exit moxaart_mac_module_exit(void)
{
	printk("%s \n",__FUNCTION__);
	platform_driver_unregister(&moxaart_mac_driver);
	return;
}

module_init(moxaart_mac_module_init);
module_exit(moxaart_mac_module_exit);

MODULE_DESCRIPTION("MOXA ART MAC Driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChinFu Yang");
