/* 
 * linux/arch/arm/mach-moxamacro/moxaarm11-pcie.c
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


#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/irq.h>

#include <mach/map.h>
#include <mach/irqs.h>
#include <mach/io.h>
#include <asm/mach/pci.h>
#include <mach/pcie.h>
/*
 * Mask table, bits to mask for quantity of size 1, 2 or 4 bytes.
 * 0 and 3 are not valid indexes...
 */
static u32 pcie_bytemask[] = {
        /*0*/   0,
        /*1*/   0xff,
        /*2*/   0xffff,
        /*3*/   0,
        /*4*/   0xffffffff,
};
static u32 pcie_cfg_va_base;

static int moxaarm11_pcie_valid_config(int bus, int dev)
{

        /*
         * Don't go out when trying to access --
         * 1. nonexisting device on local bus
         * 2. where there's no device connected (no link)
         */
        if (bus == 1 && dev == 0 && 
			((readl(MOXAARM11_VA_SYSTEM_BASE+Misc_LinkUpReg)&0xFF&0x10)>>4))
                return 1;

     	 if (bus == 1 && dev >= 1 )
                return 0;

     	 if (bus ==2 && dev == 0)
		 	return 1;

     	 if (bus ==0 || bus > 3)
                return 0;
		 
        return 0;
}

int moxaarm11_pcie_read(u32 base, struct pci_bus *bus,
                       u32 devfn, int where, int size, u32 *val)
{
	u32 offset;
	unsigned long	flags;
		
	printk("%s:%d read addr=0x%x\n", __FUNCTION__, __LINE__, base + (PCIE_CONF_BUS(bus->number) |
                PCIE_CONF_DEV(PCI_SLOT(devfn)) |
                PCIE_CONF_FUNC(PCI_FUNC(devfn)) |
                PCIE_CONF_REG(where)));

	spin_lock_irqsave(&moxaarm11_pcie_lock, flags);
	
	writel(PCIE_ADDR_VIEW_OUT | PCIE_ADDR_VIEW_REGION1, 
		MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);

	if(bus->number != 1){	//type 1
		writel(PCIE_ADDR_CTRL1_CFGRW1, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_CTRL1_OFF);
		offset = PCIE_CONF_BUS(bus->number) | PCIE_CONF_DEV(PCI_SLOT(devfn)) |
		             PCIE_CONF_FUNC(PCI_FUNC(devfn)) | PCIE_CONF_REG(where);
	}else{
		writel(PCIE_ADDR_CTRL1_CFGRW0, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_CTRL1_OFF);
		offset = PCIE_CONF_REG(where);
	}

	*val = readl(base + offset);
		


printk("%s:%d read value=0x%x\n", __FUNCTION__, __LINE__, *val);

        if (size == 1)
                *val = (*val >> (8 * (where & 3))) & pcie_bytemask[size];
        else if (size == 2)
                *val = (*val >> (8 * (where & 3))) & pcie_bytemask[size];
printk("%s:%d read value=0x%x\n", __FUNCTION__, __LINE__, *val);

	writel(PCIE_ADDR_VIEW_IN | PCIE_ADDR_VIEW_REGION2, 
		MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);

	spin_unlock_irqrestore(&moxaarm11_pcie_lock, flags);

	
        return PCIBIOS_SUCCESSFUL;
}

int moxaarm11_pcie_write(u32 base, struct pci_bus *bus,
                       u32 devfn, int where, int size, u32 val)
{
        int ret = PCIBIOS_SUCCESSFUL;
	u32 offset;
	u32 data;
	unsigned long	flags;

printk("%s:%d write value=0x%x\n", __FUNCTION__, __LINE__, val);

printk("%s:%d write addr=0x%x\n", __FUNCTION__, __LINE__, base + (PCIE_CONF_BUS(bus->number) |
                PCIE_CONF_DEV(PCI_SLOT(devfn)) |
                PCIE_CONF_FUNC(PCI_FUNC(devfn)) |
                PCIE_CONF_REG(where)));

	spin_lock_irqsave(&moxaarm11_pcie_lock, flags);

	writel(PCIE_ADDR_VIEW_OUT | PCIE_ADDR_VIEW_REGION1, 
			MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);

	if(bus->number != 1){
		writel(PCIE_ADDR_CTRL1_CFGRW1, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_CTRL1_OFF);
		offset = PCIE_CONF_BUS(bus->number) | PCIE_CONF_DEV(PCI_SLOT(devfn)) |
		             PCIE_CONF_FUNC(PCI_FUNC(devfn)) | PCIE_CONF_REG(where);		
	}else{
		writel(PCIE_ADDR_CTRL1_CFGRW0, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_CTRL1_OFF);
		offset = PCIE_CONF_REG(where);		
	}
	
        if (size == 4){
		data = val;
	}else {
		data = readl(base + offset) & ~ (pcie_bytemask[size]<<(8*(where&3)));
printk("%s:%d write data=0x%x\n", __FUNCTION__, __LINE__, data);
		data |= (val&pcie_bytemask[size])<<(8*(where&3));
printk("%s:%d write data=0x%x\n", __FUNCTION__, __LINE__, data);

	}
		
	writel(PCIE_ADDR_VIEW_IN | PCIE_ADDR_VIEW_REGION1, 
		MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);

	writel(data, base + offset);

	writel(PCIE_ADDR_VIEW_IN | PCIE_ADDR_VIEW_REGION2, 
		MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);

	spin_unlock_irqrestore(&moxaarm11_pcie_lock, flags);
	
        return ret;
}

static int moxaarm11_pcie_read_config(struct pci_bus *bus, u32 devfn, int where,
                        int size, u32 *value)
{
	int ret;

printk("%s:%d bus->number=0x%x  PCI_SLOT(devfn)=0x%x PCI_FUNC(devfn)=0x%x PCIE_CONF_REG(where)=0x%x\n", __FUNCTION__, __LINE__, bus->number, PCI_SLOT(devfn), PCI_FUNC(devfn), PCIE_CONF_REG(where));
	if (moxaarm11_pcie_valid_config(bus->number, PCI_SLOT(devfn)) == 0) {
		*value = 0xffffffff;
		return PCIBIOS_DEVICE_NOT_FOUND;
	}
printk("%s:%d\n", __FUNCTION__, __LINE__);

	ret = moxaarm11_pcie_read(pcie_cfg_va_base, bus, devfn, where, size, value);

printk("%s:%d read ret=0x%x value=0x%x\n", __FUNCTION__, __LINE__, ret, *value);
	return ret;
}

static int moxaarm11_pcie_write_config(struct pci_bus *bus, u32 devfn,
                        int where, int size, u32 value)
{
        int ret;
printk("%s:%d\n", __FUNCTION__, __LINE__);

        if (moxaarm11_pcie_valid_config(bus->number, PCI_SLOT(devfn)) == 0)
                return PCIBIOS_DEVICE_NOT_FOUND;
printk("%s:%d\n", __FUNCTION__, __LINE__);

        ret = moxaarm11_pcie_write(pcie_cfg_va_base, bus, devfn, where, size, value);
        return ret;
}

static struct pci_ops moxaarm11_pcie_ops = {
        .read = moxaarm11_pcie_read_config,
        .write = moxaarm11_pcie_write_config,
};

struct pci_bus __init *
moxaarm11_pcie_scan_bus(struct pci_sys_data *sys)
{
        struct pci_bus *bus;
        bus = pci_scan_bus(sys->busnr, &moxaarm11_pcie_ops, sys);
        return bus;
}

static struct resource moxaarm11_pcie_data_resource[] = {
	[0] = {
		.name = "PCI Express I/O Space",
		.start = MOXAARM11_VA_PCIE_IO_BASE,
		.end   = MOXAARM11_VA_PCIE_IO_BASE + MOXAARM11_PCIE_IO_WINDOW - 1,
		.flags = IORESOURCE_IO,
	},
	[1] = {
		.name =  "PCI Express Memory Space",
		.start = MOXAARM11_PA_PCIE_MEM_BASE,
		.end   = MOXAARM11_PA_PCIE_MEM_BASE + SZ_64M - 1,
		.flags = IORESOURCE_MEM,
	},
};

static int moxaarm11_pcie_abort_handler(unsigned long addr, unsigned int fsr, struct pt_regs *regs)
{
	u32 val;

	printk("PCIE abort: address = 0x%08lx fsr = 0x%03x PC = 0x%08lx LR = 0x%08lx\n",addr, fsr, regs->ARM_pc, regs->ARM_lr);

		
/*	val = readl(MOXAARM11_VA_PCIE_BASE + PCIE_CMD_OFF);
	printk("PCIE status= 0x%x\n", val);
	writel(val, MOXAARM11_VA_PCIE_BASE + PCIE_CMD_OFF);
*/
	if (fsr & (1 << 10))
		regs->ARM_pc += 4;
	return 0;
}


#include <linux/delay.h>
void __init moxaarm11_pcie_preinit(void)
{
	u16 cmd;
	u32 mask;
	u32 retry=0;

//	hook_fault_code(16 + 6, moxaarm11_pcie_abort_handler, SIGBUS, "imprecise external abort");
//	hook_fault_code(4, moxaarm11_pcie_abort_handler, SIGBUS, "external abort on linefetch");
//	hook_fault_code(6, moxaarm11_pcie_abort_handler, SIGBUS, "external abort on linefetch");
//	hook_fault_code(8, moxaarm11_pcie_abort_handler, SIGBUS, "external abort on non-linefetch");
//	hook_fault_code(10, moxaarm11_pcie_abort_handler, SIGBUS, "external abort on non-linefetch");

printk("%s:%d\n", __FUNCTION__, __LINE__); 


	writel(0x03C00000, MOXAARM11_VA_SYSTEM_BASE + SYSTEM_AHB_SLAVE_INFO_OFF);
printk("%s:%d\n", __FUNCTION__, __LINE__); 

	/*Setup up AHB slave translation associated MISC information register.*/
	writel(0x1, MOXAARM11_VA_SYSTEM_BASE + Misc_ApLtssmEn);

    while( !((readl(MOXAARM11_VA_SYSTEM_BASE+Misc_LinkUpReg)&0xFF&0x10)>>4)){
printk("%s:%d\n", __FUNCTION__, __LINE__); 

		udelay(3);
		retry++;
		if (retry >= 10)
			break;
	 };

	pcie_cfg_va_base = (unsigned long)ioremap_nocache(MOXAARM11_PA_PCIE_CFG_BASE, SZ_64M);
	
	/*
	* Master + slave enable.
	*/
	cmd = readl(MOXAARM11_VA_PCIE_BASE + PCIE_CMD_OFF);
	cmd |= PCI_COMMAND_IO;
	cmd |= PCI_COMMAND_MEMORY;
	cmd |= PCI_COMMAND_MASTER;
	writel(cmd, MOXAARM11_VA_PCIE_BASE + PCIE_CMD_OFF);
	printk("%s:%d\n", __FUNCTION__, __LINE__); 


	/*Setup the Configuration Region*/
	writel(PCIE_ADDR_VIEW_OUT | PCIE_ADDR_VIEW_REGION1, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);
	writel(MOXAARM11_PA_PCIE_CFG_BASE, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_BASE_OFF);
	writel(MOXAARM11_PA_PCIE_CFG_BASE + SZ_64M- 1, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_LIMIT_OFF);
	writel(PCIE_ADDR_CTRL1_CFGRW0, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_CTRL1_OFF);
	writel(PCIE_ADDR_CTRL2_EN, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_CTRL2_OFF);

	/*Setup the IO Region*/
	writel(PCIE_ADDR_VIEW_OUT | PCIE_ADDR_VIEW_REGION2, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);
	writel(MOXAARM11_PA_PCIE_IO_BASE, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_BASE_OFF);
	writel(MOXAARM11_PA_PCIE_IO_BASE + SZ_64K - 1, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_LIMIT_OFF);
	writel(PCIE_ADDR_CTRL1_IORW, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_CTRL1_OFF);
	writel(PCIE_ADDR_CTRL2_EN, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_CTRL2_OFF);

	/*Setup the Memory Region*/
	writel(PCIE_ADDR_VIEW_IN | PCIE_ADDR_VIEW_REGION2, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);
	writel(MOXAARM11_PA_PCIE_MEM_BASE, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_BASE_OFF);
	writel(MOXAARM11_PA_PCIE_MEM_BASE + SZ_64M - 1, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_LIMIT_OFF);
	writel(PCIE_ADDR_CTRL1_MEMRW, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_CTRL1_OFF);
	writel(PCIE_ADDR_CTRL2_EN, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_CTRL2_OFF);

	printk("%s:%d\n", __FUNCTION__, __LINE__); 

	/*
	* Enable interrupt lines.
	*/
	mask = readl(MOXAARM11_VA_PCIE_BASE + PCIE_MASK_OFF);
	mask |= 0x000000FF;
	writel(mask, MOXAARM11_VA_PCIE_BASE + PCIE_MASK_OFF);



}



int moxaarm11_pcie_setup(struct pci_sys_data *sys)
{


printk("%s:%d \n", __FUNCTION__, __LINE__); 
		
	request_resource(&ioport_resource, &moxaarm11_pcie_data_resource[0]);
	request_resource(&iomem_resource, &moxaarm11_pcie_data_resource[1]);

	sys->busnr = 1;
	sys->resource[0] = &moxaarm11_pcie_data_resource[0];
	sys->resource[1] = &moxaarm11_pcie_data_resource[1];
	sys->resource[2] = NULL;
printk("%s:%d\n", __FUNCTION__, __LINE__); 

	return 1;
}

