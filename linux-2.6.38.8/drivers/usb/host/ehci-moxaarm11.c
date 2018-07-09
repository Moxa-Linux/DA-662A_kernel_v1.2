/*
 * Moxa ARM11 CPU EHCI Host Controller Driver
 *
 * Author: Victor Yu <victor.yu@moxa.com>
 *
 * Based on "ehci-ixp4xx.c" by Vladimir Barinov <vbarinov@ru.mvista.com> 
 *
 * 2010 (c) Moxa Systems, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#include <linux/platform_device.h>
#include <mach/irqs.h>

#define MOXAARM11_MAX_USBHOST_PORT_NO	2

#define UHCI_BASE_OFFSET	0
#define DC_BASE_OFFSET		0x4000
#define OTG_BASE_OFFSET		0x8000
#define EHCI_BASE_OFFSET	0xc000

// UHCI register define
struct moxaarm11_uhci_reg_struct {
	u32	usb_command;	// offset 0x00
#define UHCI_MAXP_64		(1<<7) // max packet 64
#define UHCI_MAXP_32		0 // max packet 32
#define UHCI_CF			(1<<6) // configure flag
#define UHCI_SWDBG		(1<<5) // software debug
#define UHCI_FGR		(1<<4) // force global resume
#define UHCI_EGSM		(1<<3) // enter global suspend mode
#define UHCI_GRESET		(1<<2) // glogal reset
#define UHCI_HCRESET		(1<<1) // host controller reset
#define UHCI_RUN		(1<<0)
#define UHCI_STOP		0
	u32	usb_status;	// offset 0x04
	u32	usb_int_enable;	// offset 0x08
	u32	frame_list_base_addr;	// offset 0x0c
	u32	reversed1[28];	// offset 0x10 - 0x7c
	u32	frame_number;	// offset 0x80
	u32	start_frame;	// offset 0x84
	u32	port_status;	// offset 0x88
	u32	reversed2;	// offset 0x8c
	u32	version;	// offset 0x90
	u32	misc;		// offset 0x94
};

// OTG register define
struct moxaarm11_otg_reg_struct {
	u32	link_write;	// This register is controlled by the OTG
				// driver to manage A-Device and B-Device
				// state machine, VBUS driving, SPR and HNP
				// functions.
				// offset 0x00, BCWR1
#define EHC_EN			(1<<30)	// enable OTG_EHC
#define VBUS_TIMER_4US		0
#define VBUS_TIMER_8US		(1<<26)
#define VBUS_TIMER_16US		(2<<26)
#define VBUS_TIMER_DISABLE	(3<<26)
#define DP_TIMER_4US		0
#define DP_TIMER_8US		(1<<24)
#define DP_TIMER_16US		(2<<24)
#define DP_TIMER_DISABLE	(3<<24)
#define TEST_MODE_NONE		0
#define TEST_MODE_A_WBCON	(1<<16)
#define TEST_MODE_A_HOST	(2<<16)
#define TEST_MODE_A_SUSP	(3<<16)
#define TEST_MODE_A_PERI	(4<<16)
#define TEST_MODE_A_IDLE	(7<<16)
#define TEST_MODE_B_PERI	(0x0a<<16)
#define TEST_MODE_B_WACON	(0x0b<<16)
#define TEST_MODE_B_HOST	(0x0c<<16)
#define TEST_MODE_B_IDLE	(0x0f<<16)
#define B_HNP_FAIL		(1<<15)
#define B_HNP_OK		(1<<14)
#define B_HNP_ENABLE		(1<<13)
#define B_HNP_REQ		(1<<12)
#define B_CONN_EVT		(1<<11)	// B-Device, connect event
#define B_CHRG_VBUS		(1<<10) // charging VBUS
#define B_CHRG_DP		(1<<9) // chariing DP
#define B_DSCHB_VBUS		(1<<8) // discharging VBUS
#define A_HNP_EN		(1<<7)
#define A_IDLE			(1<<3)
#define A_CONN_EVT		(1<<2) // A-Device, connect event
#define A_VBUS_REG		(1<<1)
#define ID_PULLUP		(1<<0)
	u32	reversed1[3];
	u32	link_status;	// offset 0x10, BCSR1
#define A_HNP_START		(1<<16)
#define SESS_END		(1<<15)
#define VBUS_VALID		(1<<14)
#define B_VALID			(1<<13)
#define A_VALID			(1<<12)
#define LINE_SE1		(1<<11)
#define LINE_SE0		(1<<10)
#define LINE_K			(1<<9)
#define LINE_J			(1<<8)
#define ID_PIN			(1<<7)
#define COONECTED_MINI_A	0
#define COONECTED_MINI_B	(1<<7)
#define DRDB_CS_MASK		(3<<4)
#define DRDB_CS_B_IDLE		0
#define DRDB_CS_B_PERI		(1<<4)
#define DRDB_CS_B_WACON		(2<<4)
#define DRDB_CS_B_HOST		(3<<4)
#define DRDA_CS_MASK		7
#define DRDA_CS_A_IDLE		0
#define DRDA_CS_A_WVRISE	1
#define DRDA_CS_A_WBCON		2
#define DRDA_CS_A_HOST		3
#define DRDA_CS_A_SUSP		4
#define DRDA_CS_A_PERI		5
#define DRDA_CS_A_WVFALL	6
	u32	reversed2[3];
	u32	interrupt_status; // offset 0x20, ISR1
#define ALL_INT_STS_MASK	0x3f
#define B_HNP_END_INT_STS	(1<<5)
#define B_HNP_START_INT_STS	(1<<4)
#define B_DISC_EVT_INT_STS	(1<<3)
#define A_HNP_END_INT_STS	(1<<2)
#define A_DISC_EVT_INT_STS	(1<<1)
#define A_LOW_PWR_INT_STS	(1<<0)
	u32	reversed3[3];
	u32	interrupt_enable; // offset 0x30, IER1
#define ALL_INT_EN_MASK		0x3f
#define B_HNP_END_INT_EN	(1<<5)
#define B_HNP_START_INT_EN	(1<<4)
#define B_DISC_EVT_INT_EN	(1<<3)
#define A_HNP_END_INT_EN	(1<<2)
#define A_DISC_EVT_INT_EN	(1<<1)
#define A_LOW_PWR_INT_EN	(1<<0)
	u32	reversed4[3];
	u32	interrupt_disable; // offset 0x40, IDR1
#define ALL_INT_DIS_MASK	0x3f
#define B_HNP_END_INT_DIS	(1<<5)
#define B_HNP_START_INT_DIS	(1<<4)
#define B_DISC_EVT_INT_DIS	(1<<3)
#define A_HNP_END_INT_DIS	(1<<2)
#define A_DISC_EVT_INT_DIS	(1<<1)
#define A_LOW_PWR_INT_DIS	(1<<0)
	u32	reversed5[3];
	u32	ip_control; // offset 0x50,IPCR
#define EHC_CKG_DISABLE		(1<<10) // OTG_EHC clock gating control disable
#define UHC_CKG_DISABLE		(1<<9)
#define DC_CKG_DISABLE		(1<<8)
#define A_HNP_PROC		(1<<5)
#define OTG_PHY_RESET		(1<<4)
#define OTG_IRQ_EN		(1<<3)
#define EHC_IRQ_EN		(1<<2)
#define UHC_IRQ_EN		(1<<1)
#define DC_IRQ_EN		(1<<0)
	u32	version; // offset 0x54, OTGVR
	u32	reversed6[2];
	u32	link_ap_control; // offset 0x60, BACR
#define SW_PHY_SUSP		(1<<0) // force PHY enter suspend mode
};

struct moxaarm11_usb_host_struct {
	int					portno;
#if 1	// add by Victor Yu. 09-24-2010
	int					irqno;
#endif
	struct usb_hcd				*hcd;
	u32					physical_base;
	u32					physical_len;
	void __iomem				*virtual_base;
	struct ehci_regs			*ehci_regs;
	struct moxaarm11_otg_reg_struct		*otg_regs;
	struct moxaarm11_uhci_reg_struct	*uhci_regs;
};
static struct moxaarm11_usb_host_struct	moxaarm11_usbhost[MOXAARM11_MAX_USBHOST_PORT_NO];

static irqreturn_t moxaarm11_otg_isr(int irq, void *ptr)
{
	struct moxaarm11_usb_host_struct	*mxhost=ptr;
	u32					status;

	status = readl(&mxhost->otg_regs->interrupt_status);
	if ( status & ALL_INT_STS_MASK ) {
		writel(status, &mxhost->otg_regs->interrupt_status);
		if ( status & A_DISC_EVT_INT_STS ) {
			writel(EHC_EN|A_VBUS_REG, &mxhost->otg_regs->link_write);
		}
		return IRQ_HANDLED;
	}
	//ehci_irq(mxhost->hcd);
	return IRQ_NONE;
}

static int moxaarm11_ehci_init(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	int retval = 0;

#if 0
	ehci->big_endian_desc = 1;
	ehci->big_endian_mmio = 1;
#endif
	
	ehci->caps = hcd->regs;
	ehci->regs = hcd->regs;
	
	ehci->hcs_params = (1<<12) |// number of companion controller
			   (1<<8) | // number of ports per companion controller
			   (1<<0); // N_Ports
	//hcd->has_tt = 1;
	//ehci_reset(ehci);

	retval = ehci_init(hcd);
	if (retval)
		return retval;

	ehci_port_power(ehci, 0);

	return retval;
}

static const struct hc_driver moxaarm11_ehci_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "Moxa ARM11 Host Controller",
	.hcd_priv_size		= sizeof(struct ehci_hcd),
	.irq			= ehci_irq,
	.flags			= HCD_MEMORY | HCD_USB2,
	.reset			= moxaarm11_ehci_init,
	.start			= ehci_run,
	.stop			= ehci_stop,
	.shutdown		= ehci_shutdown,
	.urb_enqueue		= ehci_urb_enqueue,
	.urb_dequeue		= ehci_urb_dequeue,
	.endpoint_disable	= ehci_endpoint_disable,
	.endpoint_reset		= ehci_endpoint_reset,
	.get_frame_number	= ehci_get_frame,
	.hub_status_data	= ehci_hub_status_data,
	.hub_control		= ehci_hub_control,
#if defined(CONFIG_PM)
	.bus_suspend		= ehci_bus_suspend,
	.bus_resume		= ehci_bus_resume,
#endif
	.relinquish_port	= ehci_relinquish_port,
	.port_handed_over	= ehci_port_handed_over,

	.clear_tt_buffer_complete	= ehci_clear_tt_buffer_complete,
};

#if 1	// add by Victor Yu. 09-24-2010
void	moxaarm11_force_ehci(void __iomem *ehci_regs)
{
	struct moxaarm11_otg_reg_struct		*otg_regs=ehci_regs - EHCI_BASE_OFFSET + OTG_BASE_OFFSET;

	dbg_printk("%s: %d ehci_regs=0x%x\n", __func__, __LINE__,ehci_regs);
	/*writel(SW_PHY_SUSP,&otg_regs->link_ap_control);
	udelay(20);
	writel(0x0,&otg_regs->link_ap_control);
	udelay(20);*/

//	writel(readl(&otg_regs->ip_control)|0x10, &otg_regs->ip_control);
//	mdelay(20);		

	
	writel(0, &otg_regs->interrupt_enable);
	writel(ALL_INT_DIS_MASK, &otg_regs->interrupt_status);
	writel(ALL_INT_DIS_MASK, &otg_regs->interrupt_disable);

	writel(EHC_EN|TEST_MODE_A_HOST|A_VBUS_REG|VBUS_TIMER_16US|DP_TIMER_16US, &otg_regs->link_write);
	writel(0x01800300, ehci_regs + 0x60);
	//writel(0x03000300, ehci_regs + 0x60);
	//writel(0x02200200, ehci_regs + 0x60);
	
	while ( (readl(&otg_regs->link_status)&DRDA_CS_MASK) != DRDA_CS_A_HOST ) 
		udelay(100);
	writel(UHC_CKG_DISABLE|DC_CKG_DISABLE|EHC_IRQ_EN, &otg_regs->ip_control);

	/*if (readl(ehci_regs)|0x1 == 1){
		printk("%s: %d \n", __func__, __LINE__);
		writel(0x0 ,ehci_regs+0x40);
		writel(readl(ehci_regs+0x44)|0x2, ehci_regs+0x44);
		writel(readl(ehci_regs)&~0x1, ehci_regs);
		mdelay(1);
		printk("%s: %d \n", __func__, __LINE__);
		writel(0x1 ,ehci_regs+0x40);
		writel(readl(ehci_regs)|0x1, ehci_regs);		
		writel(readl(ehci_regs+0x44)|0x2, ehci_regs+0x44);
		
	}*/
	
	
}
EXPORT_SYMBOL(moxaarm11_force_ehci);
#endif

static int moxaarm11_ehci_probe(struct platform_device *pdev)
{
	struct usb_hcd		*hcd;
	const struct hc_driver	*driver=&moxaarm11_ehci_hc_driver;
	struct resource		*res;
	int			irq, retval, portno;
	char *hcd_name;

	if (usb_disabled())
		return -ENODEV;
	
	memset(moxaarm11_usbhost, 0, sizeof(moxaarm11_usbhost));
	// three are two USB host controller embedded
	for ( portno=0; portno<MOXAARM11_MAX_USBHOST_PORT_NO; portno++ ) {
		struct moxaarm11_usb_host_struct	*mxhost;
		struct moxaarm11_otg_reg_struct		*otg_regs;
		struct moxaarm11_uhci_reg_struct	*uhci_regs;

		mxhost = &moxaarm11_usbhost[portno];
		res = platform_get_resource(pdev, IORESOURCE_IRQ, portno);
		if (!res) {
			dev_err(&pdev->dev,
				"Found HC with no IRQ. Check %s setup!\n",
				dev_name(&pdev->dev));
			return -ENODEV;
		}
		irq = res->start;
		mxhost->portno = portno;
#if 1	// add by Victor Yu. 09-24-2010
		mxhost->irqno = irq;
#endif

		mxhost->hcd = hcd = usb_create_hcd(driver, &pdev->dev, dev_name(&pdev->dev));
		if (!hcd) {
			retval = -ENOMEM;
			goto fail_create_hcd;
		}
		hcd->irq = irq;

		res = platform_get_resource(pdev, IORESOURCE_MEM, portno);
		if (!res) {
			dev_err(&pdev->dev,
				"Found HC with no register addr. Check %s setup!\n", dev_name(&pdev->dev));
			retval = -ENODEV;
			goto fail_request_resource;
		}

		mxhost->physical_base = res->start;
		mxhost->physical_len  = res->end - res->start + 1;
		if (!request_mem_region(mxhost->physical_base , mxhost->physical_len, driver->description)) {
			dev_dbg(&pdev->dev, "controller already in use\n");
			retval = -EBUSY;
			goto fail_request_resource;
		}

		mxhost->virtual_base  = ioremap_nocache(mxhost->physical_base , mxhost->physical_len);
		if ( mxhost->virtual_base == NULL ) {
			dev_dbg(&pdev->dev, "error mapping memory\n");
			retval = -EFAULT;
			goto fail_ioremap;
		}

		hcd->rsrc_start = res->start + EHCI_BASE_OFFSET;
		hcd->rsrc_len = 0x1000;
		hcd->regs = mxhost->virtual_base + EHCI_BASE_OFFSET;
		mxhost->ehci_regs = hcd->regs;
		printk("%s, %d:ehci vbase=%p\n", __FUNCTION__, __LINE__, mxhost->ehci_regs);
		otg_regs = mxhost->otg_regs = mxhost->virtual_base + OTG_BASE_OFFSET;
		printk("%s, %d:otg vbase=%p\n", __FUNCTION__, __LINE__, mxhost->otg_regs);

#if 0	// mask by Victor YU. 09-24-2010
		// reset OTG
		// make sure the PHY doesn't enter reset mode
		//writel(SW_PHY_SUSP, &otg_regs->link_ap_control);
		//mdelay(20);
		//writel(0, &otg_regs->link_ap_control);

		// disable all OTG interrupt
		//writel(EHC_CKG_DISABLE | UHC_CKG_DISABLE | DC_CKG_DISABLE, &otg_regs->ip_control);

		writel(0, &otg_regs->interrupt_enable);
		writel(ALL_INT_DIS_MASK, &otg_regs->interrupt_status);
		writel(ALL_INT_DIS_MASK, &otg_regs->interrupt_disable);

		// reset OTG phy
		//writel(OTG_PHY_RESET|UHC_CKG_DISABLE|DC_CKG_DISABLE, &otg_regs->ip_control);
		//mdelay(20);	// waiting PHY reset finished
		//writel(EHC_EN|ID_PULLUP, &otg_regs->link_write);
		//mdelay(20);

		// enable A-Device Host mode
		//writel(EHC_EN|A_CONN_EVT|A_VBUS_REG|TEST_MODE_A_HOST, &otg_regs->link_write);
		writel(EHC_EN|TEST_MODE_A_HOST|A_VBUS_REG|VBUS_TIMER_16US|DP_TIMER_16US, &otg_regs->link_write);
		//writel(EHC_EN|A_VBUS_REG, &otg_regs->link_write);
#if 0
		writel(EHC_EN|A_VBUS_REG, &otg_regs->link_write);
		mdelay(20);	// waiting PHY reset finished
		writel(EHC_EN|A_CONN_EVT, &otg_regs->link_write);
#endif

#if 1 /*Added by Vincent Wu. 08-25-2010*/
		// Set Tx Start Threshold to 512 Bytes for FPGA platform
		writel(0x300, mxhost->ehci_regs + 0x60);
#endif

		// waiting for changed mode finished
		while ( (readl(&otg_regs->link_status)&DRDA_CS_MASK) != DRDA_CS_A_HOST ) udelay(100);
		//writel(EHC_EN|A_VBUS_REG, &otg_regs->link_write);

#if 0
		// stop UHCI
		uhci_regs = mxhost->uhci_regs = mxhost->virtual_base + UHCI_BASE_OFFSET;
		writel(0, &uhci_regs->usb_int_enable);
		writel(UHCI_CF|UHCI_EGSM|UHCI_STOP, &uhci_regs->usb_command);
#endif

		// setting EHCI micro fram to be 8 micro-frames
		// this is a special for Moxa ARM11 CPU
		//writel(0x00080000, mxhost->ehci_regs);

		moxaarm11_vic_set_intr_trigger(irq, vicc_level_activeHigh);
		// enable OTG for EHC interrupt
		writel(UHC_CKG_DISABLE|DC_CKG_DISABLE|EHC_IRQ_EN, &otg_regs->ip_control);
		//request_irq(irq, &moxaarm11_otg_isr, IRQF_SHARED, "MoxaOTG", mxhost);
#else
		moxaarm11_vic_set_intr_trigger(irq, vicc_level_activeHigh);
		moxaarm11_force_ehci(hcd->regs);
#endif

		retval = usb_add_hcd(hcd, irq, IRQF_SHARED | IRQF_DISABLED);
		if (retval)
			goto fail_add_hcd;
	}
	return retval;

fail_add_hcd:
	iounmap(moxaarm11_usbhost[portno].virtual_base);
fail_ioremap:
	release_mem_region(moxaarm11_usbhost[portno].physical_base, moxaarm11_usbhost[portno].physical_len);
fail_request_resource:
	usb_put_hcd(hcd);
fail_create_hcd:
	dev_err(&pdev->dev, "init %s fail, %d\n", dev_name(&pdev->dev), retval);
	if ( portno == 1 ) {
		iounmap(moxaarm11_usbhost[0].virtual_base);
		release_mem_region(moxaarm11_usbhost[0].physical_base, moxaarm11_usbhost[0].physical_len);
		usb_put_hcd(moxaarm11_usbhost[0].hcd);
	}
	memset(moxaarm11_usbhost, 0, sizeof(moxaarm11_usbhost));
	return retval;
}

static int moxaarm11_ehci_remove(struct platform_device *pdev)
{
	int	portno;

	for ( portno=0; portno<MOXAARM11_MAX_USBHOST_PORT_NO; portno++ ) {
		struct	usb_hcd	*hcd;
		hcd = moxaarm11_usbhost[portno].hcd;
		if ( hcd == NULL )
			continue;
		usb_remove_hcd(hcd);
		iounmap(moxaarm11_usbhost[portno].virtual_base);
		release_mem_region(moxaarm11_usbhost[portno].physical_base, moxaarm11_usbhost[portno].physical_len);
		usb_put_hcd(hcd);
		moxaarm11_usbhost[portno].hcd = NULL;
	}

	return 0;
}

MODULE_ALIAS("platform:moxaarm11-ehci");

static struct platform_driver moxaarm11_ehci_driver = {
	.probe = moxaarm11_ehci_probe,
	.remove = moxaarm11_ehci_remove,
	.driver = {
		.name = "MoxaARM11USBHost",
	},
};
