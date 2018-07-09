/* 
 * linux/drivers/net/moxaarm11_gmac_mii.c
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

#define BCM5461_REGS_SIZE 0x1F*sizeof(u16)
#define BCM5461_MII_CONTROL_REG	0x0
#define BCM5461_MII_AUTONEGO_REG	0x4
#define BCM5461_MISC_CONTROL_REG	0x18

#define BCM5461_MISC_CONTROL_REG_READ_SELECT	0x7<<12
#define BCM5461_MISC_CONTROL_REG_WRITE_SELECT	0x7
#define BCM5461_MISC_CONTROL_REG_WRITE_ENABLE	0x1<<15
#define BCM5461_MISC_CONTROL_LINK_STATUS_DISABLE  0x1<<5

s32 moxaarm11_gmac_phy_read_reg(struct net_device * ndev, int phy_id, int reg)
{
	u32 addr, data;
	u32 loop_variable;
	moxaarm11_gmac_priv_t		*priv=netdev_priv(ndev);

	addr = ((phy_id << GMIIPhyAddrShift) & GMIIPhyAddr) | ((reg << GMIIRegisterShifit) & GMIIRegister);
	addr = addr | CSRClockRange30M; 	
	addr = addr | GMIIBusy ; //Gmii busy bit
	dbg_printk("read phyid=%d addr=0x%x\n", phy_id, addr);
	writel(addr, (volatile void *)(priv->gmii_mac_base + GMAC_GMII_ADDR));

        for(loop_variable = 0; loop_variable < DEFAULT_LOOP_VARIABLE; loop_variable++){ //Wait till the busy bit gets cleared with in a certain amount of time
                if (!(readl((const volatile void *)(priv->gmii_mac_base + GMAC_GMII_ADDR)) & GMIIBusy)){
                break;
                }
	        udelay(DEFAULT_DELAY_VARIABLE);
        }
		
        if(loop_variable < DEFAULT_LOOP_VARIABLE){
             data = (readl((const volatile void *)(priv->gmii_mac_base + GMAC_GMII_DATA)) & 0xFFFF);
	dbg_printk("read data=0x%x\n", data);		
		return data; 
        }
	else{
	        err_printk("Error::: PHY not responding. Busy bit didnot get cleared !!!!!!\n");
		return -1;
	}	

}

void moxaarm11_gmac_phy_write_reg(struct net_device * ndev, int phy_id, int reg, int data)
{
	u32 addr;
	u32 loop_variable;
	moxaarm11_gmac_priv_t		*priv=netdev_priv(ndev);

	writel(data, (volatile void *)(priv->gmii_mac_base+ GMAC_GMII_DATA));
	dbg_printk("write data=0x%x\n", data);		
	addr = GMIIWrite | ((phy_id << GMIIPhyAddrShift) & GMIIPhyAddr) | ((reg << GMIIRegisterShifit) & GMIIRegister);
	addr = addr | CSRClockRange30M; 	
	addr = addr | GMIIBusy ; //Gmii busy bit 
	dbg_printk("write phyid=%d addr=0x%x\n", phy_id, addr);
	writel(addr, (volatile void *)(priv->gmii_mac_base + GMAC_GMII_ADDR));
	
	for(loop_variable = 0; loop_variable < DEFAULT_LOOP_VARIABLE; loop_variable++){
                if (!(readl((const volatile void *)(priv->gmii_mac_base + GMAC_GMII_ADDR)) & GMIIBusy)){
	                break;
	                }
	        udelay(DEFAULT_DELAY_VARIABLE);
        }

        if(loop_variable < DEFAULT_LOOP_VARIABLE)
		return;
	else{
	        err_printk("Error::: PHY not responding. Busy bit didnot get cleared !!!!!!\n");
		return;
	}	
}

/*
 *	The following functions are for Ethtool testing, but not fully implemented and verified.
 */

const char ethtool_stats_keys[][ETH_GSTRING_LEN]=
{
        { "tx_ok" },
        { "rx_ok" },
        { "tx_err" },
        { "rx_err" },
        { "rx_fifo" },
        { "frame_align" },
        { "tx_ok_1col" },
        { "tx_ok_mcol" },
        { "rx_ok_phys" },
        { "rx_ok_bcast" },
        { "rx_ok_mcast" },
        { "tx_abort" },
        { "tx_underrun" },
        { "rx_frags" },
};
#define MOXAARM11_ETH_NUM_STATS ARRAY_SIZE(ethtool_stats_keys)

static void moxaarm11_gmac_get_drvinfo (struct net_device *ndev, struct ethtool_drvinfo *info)
{
        strcpy (info->driver, DRV_NAME);
        strcpy (info->version, DRV_VERSION);
}

static int moxaarm11_gmac_phy_get_regs_len(struct net_device *ndev)
{
        return BCM5461_REGS_SIZE;
}

static int moxaarm11_gmac_get_sset_count (struct net_device *ndev, int sset)
{
        switch (sset) {
        case ETH_SS_STATS:
                return MOXAARM11_ETH_NUM_STATS;
        default:
                return -EOPNOTSUPP;
        }
}

static int moxaarm11_gmac_phy_get_settings(struct net_device *ndev, struct ethtool_cmd *cmd)
{
        moxaarm11_gmac_priv_t *priv = netdev_priv(ndev);
        int rc;
        unsigned long flags;

        spin_lock_irqsave(&priv->txlock, flags);
	//spin_lock(&priv->rxlock);
	 
        rc = mii_ethtool_gset(&priv->mii_if, cmd);
		
	//spin_unlock(&priv->rxlock);		
        spin_unlock_irqrestore(&priv->txlock, flags);

        return rc;
}

static int moxaarm11_gmac_phy_set_settings(struct net_device *ndev, struct ethtool_cmd *cmd)
{
        moxaarm11_gmac_priv_t *priv = netdev_priv(ndev);
        int rc;
        unsigned long flags;

        spin_lock_irqsave(&priv->txlock, flags);
	//spin_lock(&priv->rxlock);
        rc = mii_ethtool_sset(&priv->mii_if, cmd);
	//spin_unlock(&priv->rxlock);		
        spin_unlock_irqrestore(&priv->txlock, flags);

        return rc;
}

static int moxaarm11_gmac_phy_nway_reset(struct net_device *ndev)
{
        moxaarm11_gmac_priv_t *priv = netdev_priv(ndev);
        return mii_nway_restart(&priv->mii_if);
}

static void moxaarm11_gmac_phy_get_regs(struct net_device *ndev, struct ethtool_regs *regs, void *p)
{
        moxaarm11_gmac_priv_t *priv = netdev_priv(ndev);
        unsigned long flags;

        if (regs->len < BCM5461_REGS_SIZE)
                return /* -EINVAL */;

        regs->version = 1;

        spin_lock_irqsave(&priv->txlock, flags);
	//spin_lock(&priv->rxlock);
        memcpy_fromio(p,(u32 *)ndev->base_addr, 0x4000);
	//spin_unlock(&priv->rxlock);		
        spin_unlock_irqrestore(&priv->txlock, flags);
}

#if 1 /* Jared, 2016-08-08, Support to check the link status */
u32 moxaarm11_get_link(struct net_device *ndev)
{
	moxaarm11_gmac_priv_t *priv = netdev_priv(ndev);
	return	mii_link_ok(&priv->mii_if);
}
#endif

static const struct ethtool_ops moxaarm11_gmac_ethtool_ops = {
        .get_drvinfo            = moxaarm11_gmac_get_drvinfo,
        .get_regs_len           = moxaarm11_gmac_phy_get_regs_len,
        .get_sset_count         = moxaarm11_gmac_get_sset_count,
        .get_settings           = moxaarm11_gmac_phy_get_settings,
        .set_settings           = moxaarm11_gmac_phy_set_settings,
        .nway_reset             = moxaarm11_gmac_phy_nway_reset,
#if 1 /* Jared, 2016-08-08, Support to check the link status */
        .get_link               = moxaarm11_get_link,
#else
        .get_link               = ethtool_op_get_link,
#endif
//        .get_msglevel           = moxaarm11_gmac_phy_get_msglevel,
//        .set_msglevel           = moxaarm11_gmac_phy_set_msglevel,
//        .get_rx_csum            = moxaarm11_gmac_phy_get_rx_csum,
//        .set_rx_csum            = moxaarm11_gmac_phy_set_rx_csum,
        .set_tx_csum            = ethtool_op_set_tx_csum, // local! 
        .set_sg                 = ethtool_op_set_sg,
        .set_tso                = ethtool_op_set_tso,
        .get_regs               = moxaarm11_gmac_phy_get_regs,
//        .get_wol                = moxaarm11_gmac_phy_get_wol,
//        .set_wol                = moxaarm11_gmac_phy_set_wol,
//        .get_strings            = moxaarm11_gmac_phy_get_strings,
//        .get_ethtool_stats      = moxaarm11_gmac_phy_get_ethtool_stats,
//        .get_eeprom_len         = moxaarm11_gmac_phy_get_eeprom_len,
//        .get_eeprom             = moxaarm11_gmac_phy_get_eeprom,
//        .set_eeprom             = moxaarm11_gmac_phy_set_eeprom,
};


