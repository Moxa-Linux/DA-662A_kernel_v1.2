/* 
 * linux/arch/arm/mach-moxamacro/moxaarm11-pci.c
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

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <asm/dma-mapping.h>

#include <asm/cputype.h>
#include <asm/irq.h>
#include <asm/sizes.h>
#include <asm/system.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <asm/mach/pci.h>
#include <mach/hardware.h>
#include <mach/map.h>
#include <mach/gpio.h>
#include <mach/system-control.h>

#ifdef CONFIG_PCIE
static struct pci_bus __init	*moxaarm11_pcie_scan_bus(struct pci_sys_data *sys);
void				moxaarm11_pcie_preinit(void);
int				moxaarm11_pcie_setup(struct pci_sys_data *sys);
#endif // CONFIG_PCIE

/*
 * Define CSR directly R/W macros 
 */
#define MOXAARM11_PCI_CSR(x) ((volatile u32 *)(MOXAARM11_VA_PCI_CFG_BASE+(x)))
#define moxaarm11_csr_write_byte(offset, data)	__raw_writeb(data, MOXAARM11_PCI_CSR(offset))
#define moxaarm11_csr_write_word(offset, data)	__raw_writew(data, MOXAARM11_PCI_CSR(offset))
#define moxaarm11_csr_write_dword(offset, data)	__raw_writel(data, MOXAARM11_PCI_CSR(offset))
#define moxaarm11_csr_read_byte(offset)		__raw_readb(MOXAARM11_PCI_CSR(offset))
#define moxaarm11_csr_read_word(offset)		__raw_readw(MOXAARM11_PCI_CSR(offset))
#define moxaarm11_csr_read_dword(offset)	__raw_readl(MOXAARM11_PCI_CSR(offset))

/*
 * MOXAARM11 PCI IRQs
 */
#define MOXAARM11_PCI_MAX_DEV	6

#define PCI_CFG_TRDY		0x00000040
#define PCI_CFG_RETRY_TIMEOUT	0x00000041

#define PCI_CFG_CMD_INIT_VAL	(PCI_COMMAND_IO|PCI_COMMAND_MEMORY|PCI_COMMAND_MASTER|PCI_COMMAND_PARITY|PCI_COMMAND_SERR)
#define PCI_CFG_LTIMER_INIT_VAL 0x40
#define PCI_CFG_TRDY_INIT_VAL	0x10

#define SZ_BYTE		1
#define SZ_WORD		2
#define SZ_DOUBLE_WORD	4
/*
 * Mask table, bits to mask for quantity of size 1, 2 or 4 bytes.
 * 0 and 3 are not valid indexes...
 */
static u32 bytemask[] = {
        /*0*/   0,
        /*1*/   0xff,
        /*2*/   0xffff,
        /*3*/   0,
        /*4*/   0xffffffff,
};


/*
 * PCI cfg an I/O routines are done by programming a
 * command/byte enable register, and then read/writing
 * the data from a data regsiter. We need to ensure
 * these transactions are atomic or we will end up
 * with corrupt data on the bus or in a driver.
 */
static DEFINE_SPINLOCK(moxaarm11_pci_lock);

u32	moxaarm11_pci_read(u32 addr, u32 cmd)
{
	unsigned long	flags;
	u32		data, stat;

	spin_lock_irqsave(&moxaarm11_pci_lock, flags);

	/* set the target address*/
	moxaarm11_csr_write_dword(PCI_CIO_AD_OFFSET, addr);
	/* set up and execute the read */
	moxaarm11_csr_write_dword(PCI_CIO_CBE_OFFSET, cmd);

	/*Key Point: Check if read */
	stat = moxaarm11_csr_read_byte(PCI_ERR_OFFSET);
	if (!(stat & 0x1)){
		/* the result of the read is now in NP_RDATA */
		data = moxaarm11_csr_read_dword(PCI_CIO_RDATA_OFFSET);
	} else {
		//printk("%s,%d:pci_transaction_error=%x,addr=%x,cmd=%x\n", __FUNCTION__, __LINE__, stat, addr, cmd);
		data = 0;
		moxaarm11_csr_write_dword(PCI_ERR_OFFSET, stat);
	}

	spin_unlock_irqrestore(&moxaarm11_pci_lock, flags);

	return data;
}
EXPORT_SYMBOL(moxaarm11_pci_read);

void	moxaarm11_pci_write(u32 addr, u32 cmd, u32 data)
{
	unsigned long	flags;

	spin_lock_irqsave(&moxaarm11_pci_lock, flags);

	/* set the target address*/
	moxaarm11_csr_write_dword(PCI_CIO_AD_OFFSET, addr);

	/* set up the write */
	moxaarm11_csr_write_dword(PCI_CIO_CBE_OFFSET, cmd);

	/* execute the write by writing to NP_WDATA */
	moxaarm11_csr_write_dword(PCI_CIO_WDATA_OFFSET, data);		

	spin_unlock_irqrestore(&moxaarm11_pci_lock, flags);
}
EXPORT_SYMBOL(moxaarm11_pci_write);

static u32 crp_read(u32 ad_cbe)
{
	unsigned long	flags;
	u32		data;

	spin_lock_irqsave(&moxaarm11_pci_lock, flags);
	moxaarm11_csr_write_dword(PCI_CRP_AD_CBE_OFFSET, CRP_CMD_CONFIGREAD | ad_cbe);
	data = moxaarm11_csr_read_dword(PCI_CRP_RDATA_OFFSET);
	spin_unlock_irqrestore(&moxaarm11_pci_lock, flags);
	return data;
}

/*
 * Write to PCI config space
 */
static void crp_write(u32 ad_cbe, u32 data)
{
	unsigned long flags;

	spin_lock_irqsave(&moxaarm11_pci_lock, flags);
	moxaarm11_csr_write_dword(PCI_CRP_AD_CBE_OFFSET, CRP_CMD_CONFIGWRITE | ad_cbe);
	moxaarm11_csr_write_dword(PCI_CRP_WDATA_OFFSET, data);
	spin_unlock_irqrestore(&moxaarm11_pci_lock, flags);
}

static u32 local_byte_lane_enable_bits(u32 n, int size)
{
	if ( size == SZ_BYTE )
		return (0xf & ~BIT(n)) << CRP_AD_CBE_BESL;
	if ( size == SZ_WORD )
		return (0xf & ~(BIT(n) | BIT(n+1))) << CRP_AD_CBE_BESL;
	if ( size == SZ_DOUBLE_WORD )
		return 0;
	return 0xffffffff;
}

int local_read_config(int where, int size, u32 *value)
{
	u32	n, byte_enables, data;

	pr_debug("local_read_config from %d size %d\n", where, size);
	n = where & 3;
	byte_enables = local_byte_lane_enable_bits(n, size);
	if (byte_enables == 0xffffffff)
		return PCIBIOS_BAD_REGISTER_NUMBER;
	data = crp_read((where & ~3)|byte_enables);
	*value = (data >> (8*n)) & bytemask[size];
	pr_debug("local_read_config read %#x\n", *value);
	return PCIBIOS_SUCCESSFUL;
}
EXPORT_SYMBOL(local_read_config);

int local_write_config(int where, int size, u32 value)
{
	u32	n, byte_enables, data;

	pr_debug("local_write_config %#x to %d size %d\n", value, where, size);
	n = where & 3;
	byte_enables = local_byte_lane_enable_bits(n, size);
	if (byte_enables == 0xffffffff)
		return PCIBIOS_BAD_REGISTER_NUMBER;
	data = value << (8*n);
	crp_write((where & ~3) | byte_enables, data);
	return PCIBIOS_SUCCESSFUL;
}
EXPORT_SYMBOL(local_write_config);

static u32 moxaarm11_config_addr(u8 bus_num, u16 devfn, int where)
{
	u32	addr;

	if (!bus_num) {
		/* type 0 */
		addr = BIT(31-PCI_SLOT(devfn)) | ((PCI_FUNC(devfn)) << 8) | (where & ~3);
	} else {
		/* type 1 */
		addr = (bus_num << 16) | ((PCI_SLOT(devfn)) << 11) | ((PCI_FUNC(devfn)) << 8) | (where & ~3) | 1;
	}
	return addr;
}

static u32 byte_lane_enable_bits(u32 n, int size)
{
	if ( size == SZ_BYTE )
		return (0xf & ~BIT(n)) << MOXAARM11_PCI_CIO_CBE_BESL;
	if ( size == SZ_WORD )
		return (0xf & ~(BIT(n) | BIT(n+1))) << MOXAARM11_PCI_CIO_CBE_BESL;
	if ( size == SZ_DOUBLE_WORD )
		return 0;
	return 0xffffffff;
}

static int moxaarm11_pci_read_config(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 *value)
{
	u32		n, byte_enables, addr, data;
	u8		bus_num = bus->number;

	pr_debug("read_config from %d size %d dev %d:%d:%d\n", where, size,
		bus_num, PCI_SLOT(devfn), PCI_FUNC(devfn));
	
	*value = 0xffffffff;
	n = where & 3;
	byte_enables = byte_lane_enable_bits(n, size);
	if (byte_enables == 0xffffffff)
		return PCIBIOS_BAD_REGISTER_NUMBER;

	addr = moxaarm11_config_addr(bus_num, devfn, where);
	data = moxaarm11_pci_read(addr, byte_enables | CIO_CMD_CONFIGREAD);

	*value = (data >> (8*n)) & bytemask[size];
	pr_debug("read_config_byte read %#x\n", *value);

	return PCIBIOS_SUCCESSFUL;
}

static int moxaarm11_pci_write_config(struct pci_bus *bus,  unsigned int devfn, int where, int size, u32 value)
{
	u32		n, byte_enables, addr, data;
	u8		bus_num = bus->number;

	pr_debug("write_config_byte %#x to %d size %d dev %d:%d:%d\n", value, where, size, bus_num, PCI_SLOT(devfn), PCI_FUNC(devfn));

	n = where & 3;
	byte_enables = byte_lane_enable_bits(n, size);
	if (byte_enables == 0xffffffff)
		return PCIBIOS_BAD_REGISTER_NUMBER;

	addr = moxaarm11_config_addr(bus_num, devfn, where);
	data = value << (8*n);
	moxaarm11_pci_write(addr, byte_enables | CIO_CMD_CONFIGWRITE, data);
		
	return PCIBIOS_SUCCESSFUL;
}

	
struct pci_ops moxaarm11_pci_ops = {
	.read =  moxaarm11_pci_read_config,
	.write = moxaarm11_pci_write_config,
};

struct pci_bus __devinit *moxaarm11_pci_sys_scan_bus(int nr, struct pci_sys_data *sys)
{
        struct pci_bus *bus;

        if (nr == 0) {
                bus = pci_scan_bus(sys->busnr, &moxaarm11_pci_ops, sys);
        } else if (nr == 1 ) {
#ifdef CONFIG_PCIE        
                bus = moxaarm11_pcie_scan_bus(sys);
#else
                bus = NULL;
#endif
        } else {
                bus = NULL;
                BUG();
        }

        return bus;
}
	
/*
 * Attention: SYN PCI Bridge doesn't design in the Int for itself
 * So this meaning that don't care the "bridge error condition".
 * SYN PCI irq function()
 */
static irqreturn_t moxaarm11_pci_irq(int irq, void *dev)
{
	u32		st, cmd, stat, addr;

	local_read_config(PCI_COMMAND, SZ_WORD, &cmd);
	local_read_config(PCI_STATUS, SZ_WORD, &st);
	if (st & 0xff) {
		printk("PCI Bridge CMD: %08X, Status: %08X\n", cmd, st);
		local_write_config(PCI_STATUS, SZ_WORD, st);
	}

	stat = moxaarm11_csr_read_dword(PCI_ERR_OFFSET);
	if (stat & 0x3) {
		addr = moxaarm11_csr_read_dword(PCI_ERR_ADDR_OFFSET);
		printk("PCI Error: %x @ %08X\n", stat & 0x3, addr);
		moxaarm11_csr_write_byte(PCI_FLUSH_FIFO_OFFSET, 0x1);
		moxaarm11_csr_write_dword(PCI_ERR_OFFSET, stat);
	}

	stat = moxaarm11_csr_read_dword(PCI_AHB_ERR_OFFSET);
	if (stat & 0x1) {
		addr = moxaarm11_csr_read_dword(PCI_AHB_ERR_ADDR_OFFSET);
		printk("AHB Error: %x @ %08X\n", stat & 0x1, addr);
		moxaarm11_csr_write_dword(PCI_AHB_ERR_OFFSET, stat);
	}
	return IRQ_HANDLED;
}

static int moxaarm11_pci_fault(unsigned long addr, unsigned int fsr, struct pt_regs *regs)
{
        /* Acknowledge all faults */
	printk("Has PCI fault !\n");
	local_write_config(PCI_STATUS, SZ_WORD, 0xffff);
	moxaarm11_csr_write_byte(PCI_ERR_OFFSET, 0x3);
	moxaarm11_csr_write_byte(PCI_AHB_ERR_OFFSET, 0x1);
	return 0;       /* 0: not a fault, 1: system fault */
}

#define PCI_RESET_GPIO	9	// to reset PCI client component
void __init moxaarm11_pci_preinit(void)
{
	// reset PCI bus devices
	moxaarm11_io_pin_select(PCI_RESET_GPIO, GPIOA_USED_FOR_GPIO);
	mcpu_gpio_inout(1<<PCI_RESET_GPIO, MCPU_GPIO_OUTPUT);
	mcpu_gpio_set(1<<PCI_RESET_GPIO, MCPU_GPIO_LOW);
	msleep(2);
	mcpu_gpio_set(1<<PCI_RESET_GPIO, MCPU_GPIO_HIGH);
	msleep(2);

        /*
         * Hook in our fault handler for PCI errors
         */
	hook_fault_code(4, moxaarm11_pci_fault, SIGBUS, 0, "external abort on linefetch");
	hook_fault_code(6, moxaarm11_pci_fault, SIGBUS, 0, "external abort on linefetch");
	hook_fault_code(8, moxaarm11_pci_fault, SIGBUS, 0, "external abort on non-linefetch");
	hook_fault_code(10, moxaarm11_pci_fault, SIGBUS, 0, "external abort on non-linefetch");

	local_write_config(PCI_COMMAND, SZ_WORD, PCI_CFG_CMD_INIT_VAL);
	local_write_config(PCI_LATENCY_TIMER, SZ_BYTE, PCI_CFG_LTIMER_INIT_VAL);
	local_write_config(PCI_CFG_TRDY, SZ_BYTE, PCI_CFG_TRDY_INIT_VAL);
	local_write_config(PCI_STATUS, SZ_WORD, ~0);

	//local_write_config(0x10, SZ_DOUBLE_WORD, MOXAARM11_PA_PCI_MEM_BASE);
	//local_write_config(0x18, SZ_DOUBLE_WORD, MOXAARM11_PA_PCI_IO_BASE);
	moxaarm11_csr_write_byte(PCI_ERR_OFFSET, ~0);
	moxaarm11_csr_write_byte(PCI_AHB_ERR_OFFSET, ~0);
		
	// setup GPIO 0,1,3 for interrupt source
	moxaarm11_io_pin_select(0, GPIOA_USED_FOR_OTHERS);
	moxaarm11_io_pin_select(1, GPIOA_USED_FOR_OTHERS);
	moxaarm11_io_pin_select(3, GPIOA_USED_FOR_OTHERS);
	/*
	moxaarm11_io_pin_select(4, GPIOA_USED_FOR_GPIO);// used GPIO interrupt
	mcpu_gpio_inout(1<<4, MCPU_GPIO_INPUT);
	*/

	// set SoC I/O multi function to be PCI
	moxaarm11_io_function_select(io_function_pci);

	// setup PCI host controller interrupt service routine
	moxaarm11_vic_set_intr_trigger(MOXAARM11_IRQ_PCI, vicc_level_activeHigh);
	request_irq(MOXAARM11_IRQ_PCI, moxaarm11_pci_irq, 0, "pci_master", NULL);
}

static int __init moxaarm11_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	int	irq;
#ifdef CONFIG_MOXAARM11_DDR2_SDRAM
	int	index;
#endif
	int	bus_nr = dev->bus->number;

	if (bus_nr == 0){
	
		static int pci_irq_table[MOXAARM11_PCI_MAX_DEV] = {
#ifdef CONFIG_MOXAARM11_DDR2_SDRAM
			MOXAARM11_IRQ_INTCJ,  //31		
			MOXAARM11_IRQ_INTBJ,  //30		
			MOXAARM11_IRQ_INTAJ, //29
			0, //not connected
#else
			/* here setting is for DA662 */
                        MOXAARM11_IRQ_INTAJ, //28, Moxa, MU860
                        MOXAARM11_IRQ_INTAJ,  //29, Moxa, MU860
                        MOXAARM11_IRQ_INTDJ,  //30, VIVA, USB host
			0, // not connected
                        MOXAARM11_IRQ_INTBJ,  //30, RelTak, Ethernet, 8169
                        MOXAARM11_IRQ_INTBJ,  //30, RelTak, Ethernet, 8169
#endif		
		};
		
#ifdef CONFIG_MOXAARM11_DDR2_SDRAM
		index = (slot+pin-1) & 3;
		irq = pci_irq_table[index];
#else
		irq = pci_irq_table[slot];
#endif
		if ( irq < MOXAARM11_VIC_IRQ_MAX )
			moxaarm11_vic_set_intr_trigger(irq, vicc_level_activeLow);
		return irq;
	}else if (bus_nr >= 1){
#ifdef CONFIG_PCIE
		moxaarm11_vic_set_intr_trigger(MOXAARM11_IRQ_PCIE, vicc_level_activeLow);
		return MOXAARM11_IRQ_PCIE;
#else
		return -1;
#endif
	}else
		return -1;
	
}

static struct resource moxaarm11_pci_data_resource[] = {
	[0] = {
		.name = "PCI I/O Space",
		.start = MOXAARM11_PA_PCI_IO_BASE,
		.end   = MOXAARM11_PA_PCI_IO_BASE + SZ_64K - 1,
		.flags = IORESOURCE_IO,
	},
	[1] = {
		.name =  "PCI Memory Space",
		.start = MOXAARM11_PA_PCI_MEM_BASE,
		.end   = MOXAARM11_PA_PCI_MEM_BASE + SZ_64M - 1,
		.flags = IORESOURCE_MEM,
	},
};

int moxaarm11_pci_setup(struct pci_sys_data *sys)
{
	request_resource(&ioport_resource, &moxaarm11_pci_data_resource[0]);
	request_resource(&iomem_resource, &moxaarm11_pci_data_resource[1]);

	sys->busnr = 0;
	sys->resource[0] = &moxaarm11_pci_data_resource[0];
	sys->resource[1] = &moxaarm11_pci_data_resource[1];
	sys->resource[2] = NULL;

	return 1;
}

int moxaarm11_pci_sys_setup(int nr, struct pci_sys_data *sys)
{
	if (nr == 0){
		moxaarm11_pci_preinit();
		return moxaarm11_pci_setup(sys);
	}else if(nr == 1){
#ifdef CONFIG_PCIE	
		moxaarm11_pcie_preinit();
		return moxaarm11_pcie_setup(sys);
#else		
		return 0;
#endif
	}
	else
		return 0;
}



struct hw_pci moxaarm11_pci __initdata = {
#ifdef CONFIG_PCIE	
	.nr_controllers = 2,
#else
	.nr_controllers = 1,
#endif
	.swizzle        = pci_std_swizzle,
	.setup          = moxaarm11_pci_sys_setup,
	.scan           = moxaarm11_pci_sys_scan_bus,
	.map_irq        = moxaarm11_map_irq,
};

int __init moxaarm11_pci_init(void)
{
	pci_common_init(&moxaarm11_pci);
	return 0;
}

subsys_initcall(moxaarm11_pci_init);
