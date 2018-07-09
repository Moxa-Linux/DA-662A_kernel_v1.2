#ifndef __MOXA_PCIE_H
#define __MOXA_PCIE_H

#ifdef CONFIG_PCIE
#include <linux/kernel.h>
#include <linux/interrupt.h>

#define	MOXAARM11_VA_PCIE_IO_END	(MOXAARM11_VA_PCIE_IO_BASE+MOXAARM11_PCIE_IO_WINDOW)


/*
 * PCIe unit register offsets.  !!! The address has not well-defined.
 */
#define PCIE_DEV_ID_OFF         0x0000
#define PCIE_CMD_OFF            0x0004
#define PCIE_DEV_REV_OFF        0x0008
#define PCIE_BAR_LO_OFF(n)      (0x0010 + ((n) << 3))
#define PCIE_BAR_HI_OFF(n)      (0x0014 + ((n) << 3))

#define PCIE_MASK_OFF           0x003c

//#define  PCIE_CONF_REG(r)               ((((r) & 0xf00) << 8) | ((r) & 0xfc))
#define  PCIE_CONF_REG(r)               ((r) & 0xffc)
#define  PCIE_CONF_BUS(b)               (((b) & 0xff) << 24)
#define  PCIE_CONF_DEV(d)               (((d) & 0x1f) << 19)
#define  PCIE_CONF_FUNC(f)              (((f) & 0x7) << 16)


#define PCIE_ADDR_VIEW_OFF	0x0900
#define PCIE_ADDR_VIEW_IN	0x1<<31
#define PCIE_ADDR_VIEW_OUT	0x0<<31
#define PCIE_ADDR_VIEW_REGION1 0x0
#define PCIE_ADDR_VIEW_REGION2 0x1


#define PCIE_ADDR_CTRL1_OFF	0x0904
#define PCIE_ADDR_CTRL1_MEMRW	0x0
#define PCIE_ADDR_CTRL1_IORW	0x2
#define PCIE_ADDR_CTRL1_CFGRW0	0x4
#define PCIE_ADDR_CTRL1_CFGRW1	0x5

#define PCIE_ADDR_CTRL2_OFF	0x0908
#define PCIE_ADDR_CTRL2_EN		0x1<<31

#define PCIE_ADDR_BASE_OFF	0x090C
#define PCIE_ADDR_LIMIT_OFF	0x0914
#define PCIE_ADDR_TARGET_OFF	0x0918

#define SYSTEM_AHB_SLAVE_INFO_OFF	0x0118
#define Misc_SlvReqInf	0x0118
#define Misc_ApLtssmEn	0x0128
#define	Misc_LinkUpReg	0x0138
#define	Misc_IntClrReg	0x0168
#define	Phy_Mpll_Setup	0x017C
#define Misc_AppReqInt	0x0124
#define Misc_VenMsgReq	0x0110
#define Misc_VenMsgDW0	0x0100
#define Misc_VenMsgDW1	0x0104
#define Misc_VenMsgDW2	0x0108
#define Misc_VenMsgDW3	0x010C

/*
 * PCI cfg an I/O routines are done by programming a
 * command/byte enable register, and then read/writing
 * the data from a data regsiter. We need to ensure
 * these transactions are atomic or we will end up
 * with corrupt data on the bus or in a driver.
 */
static DEFINE_SPINLOCK(moxaarm11_pcie_lock);
#endif

#endif