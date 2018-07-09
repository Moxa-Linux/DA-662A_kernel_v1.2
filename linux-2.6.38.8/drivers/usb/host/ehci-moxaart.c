/* 
 * linux/drivers/usb/host/ehci-moxaart.c
 *
 * Copyright 2011 Moxa Tech., Inc.
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
#include	<linux/irq.h>
#include <linux/platform_device.h>
#include <mach/moxaart_int.h>
#include	<mach/gpio.h>



struct moxaart_usb_host_struct {
	int					irq;

	struct usb_hcd		*hcd;
	u32					physical_base;
	u32					physical_len;
	void __iomem			*virtual_base;
	struct ehci_regs		*ehci_regs;
};


static int moxaart_ehci_reset(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	int retval = 0;

#if 0  //For IW Cinterion 3G device
	writel(0x00000000, MOXAART_GPIO_VA_BASE);
	writel(0x02000000, MOXAART_GPIO_VA_BASE + 0x8);
	mdelay(100);
#endif

	ehci->caps = hcd->regs;
	ehci->regs = hcd->regs + HC_LENGTH(ehci_readl(ehci, &ehci->caps->hc_capbase));
	dbg_printk("ehci_hc_reset: ehci->caps=0x%p, ehci->regs=0x%p\n", ehci->caps, ehci->regs);

	ehci->hcs_params = ehci_readl(ehci, &ehci->caps->hcs_params);

	ehci_reset(ehci);

	retval = ehci_init(hcd);
	if (retval)
		return retval;

	ehci_port_power(ehci, 0);

	return retval;
}

static const struct hc_driver moxaart_ehci_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "Moxa ART Host Controller",
	.hcd_priv_size		= sizeof(struct ehci_hcd),
	.irq			= ehci_irq,
	.flags			= HCD_MEMORY | HCD_USB2,
	.reset			= moxaart_ehci_reset,
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

static int moxaart_ehci_probe(struct platform_device *pdev)
{
	const struct hc_driver	*driver=&moxaart_ehci_hc_driver;
	struct moxaart_usb_host_struct	*moxaart_usbhost = kzalloc(sizeof(struct moxaart_usb_host_struct), GFP_KERNEL);
	struct resource		*res;
	int ret = 0;

	if (usb_disabled())
		return -ENODEV;

	if(!moxaart_usbhost){
		printk("%s: Failed to allocate memory!\n", pdev->name);
		return -ENOMEM;
	}	
	pdev->dev.platform_data = (void *)moxaart_usbhost;
		
	/*Try to get the platform resources.*/
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if ( !res) {
		printk("%s: Failed to get the memory resource!\n", pdev->name);
		ret = -ENODEV;
		goto get_resource_failed;
	}
	moxaart_usbhost->physical_base = res->start;
	moxaart_usbhost->physical_len  = res->end - res->start + 1;

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if ( !res) {
		printk("%s: Failed to get the IRQ resource!\n", pdev->name);
		ret = -ENODEV;
		goto get_resource_failed;
	}
	moxaart_usbhost->irq = res->start;

	if (!request_mem_region(moxaart_usbhost->physical_base , moxaart_usbhost->physical_len, driver->description)){
		printk("%s: Failed to request hardware address region!\n", pdev->name);
		ret = -ENOMEM;
		goto request_mem_region_failed;
	}

	moxaart_usbhost->virtual_base  = ioremap_nocache(moxaart_usbhost->physical_base , moxaart_usbhost->physical_len);
	if ( moxaart_usbhost->virtual_base == NULL )  {
		printk("%s: Failed to remap hardware physical address!\n", pdev->name);		
		ret = -ENOMEM;
		goto ioremap_failed;
	}

	moxaart_usbhost->hcd = usb_create_hcd(driver, &pdev->dev, dev_name(&pdev->dev));
	if (!moxaart_usbhost->hcd) {
		ret = -ENOMEM;
		goto fail_create_hcd;
	}
	moxaart_usbhost->hcd->irq = moxaart_usbhost->irq;

	moxaart_usbhost->hcd->rsrc_start = moxaart_usbhost->physical_base;
	moxaart_usbhost->hcd->rsrc_len = 0x1000;
	moxaart_usbhost->hcd->regs = moxaart_usbhost->virtual_base;
	moxaart_usbhost->ehci_regs = moxaart_usbhost->hcd->regs + 0x10;
	dbg_printk("ehci vbase=0x%p\n",  moxaart_usbhost->ehci_regs);
	dbg_printk("ehci irq=%d\n",  moxaart_usbhost->irq);
	
	if(usb_add_hcd(moxaart_usbhost->hcd, moxaart_usbhost->irq, IRQF_SHARED | IRQF_DISABLED | IRQ_TYPE_LEVEL_HIGH))
			goto fail_add_hcd;

	//int i;
	//for( i = 0; i< 0x20; i++)
	//	printk("ehci_reg[0x%x]=0x%x\n", i*4, *(((u32 *)moxaart_usbhost->ehci_regs)+i));

	return ret;

fail_add_hcd:
	usb_put_hcd(moxaart_usbhost->hcd);
fail_create_hcd:
	iounmap(moxaart_usbhost->virtual_base);
ioremap_failed:
	release_mem_region(moxaart_usbhost->physical_base , moxaart_usbhost->physical_len);
request_mem_region_failed:
get_resource_failed:	
	kfree(moxaart_usbhost);
	return ret;
}


static int moxaart_ehci_remove(struct platform_device *pdev)
{
	struct moxaart_usb_host_struct	*moxaart_usbhost = (struct moxaart_usb_host_struct*)pdev->dev.platform_data;

	usb_remove_hcd(moxaart_usbhost->hcd);
	usb_put_hcd(moxaart_usbhost->hcd);	
	iounmap(moxaart_usbhost->virtual_base);
	release_mem_region(moxaart_usbhost->physical_base , moxaart_usbhost->physical_len);
	kfree(moxaart_usbhost);
	return 0;
}


static struct platform_driver moxaart_ehci_driver = {
	.probe = moxaart_ehci_probe,
	.remove = moxaart_ehci_remove,
	.driver = {
		.name = "MoxaArtUSBHost",
	},
};


MODULE_ALIAS("platform:MoxaArtUSBHost");
