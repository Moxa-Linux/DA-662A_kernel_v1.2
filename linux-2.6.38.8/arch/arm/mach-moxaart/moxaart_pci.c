/* 
	History:
	Date		Author			Comment
	11-04-2005	Victor Yu.		Create it.
*/
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/ptrace.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/init.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/mach/pci.h>
#include <asm/sizes.h>
#include <mach/moxaart/moxaart.h>
#include <mach/moxaart_int.h>
#include <mach/moxaart_pci.h>
#include <asm/mach-types.h>

#define LOCAL_READ_WRITE_LOCK_IRQ	1
#define MOXAART_PCI_IRQ_STATUS_ADDR	0x4c

//#define DEBUG_MOXAART_PCI			1
#ifdef DEBUG_MOXAART_PCI
#define dbg_printk(x...)		printk(x)
#else
#define dbg_printk(x...)
#endif

#define CONFIG_CMD(bus, device_fn, where)   (0x80000000 | (bus << 16) | (device_fn << 8) | (where & ~3) )
static spinlock_t	moxaart_pci_lock;
static unsigned int	pci_config_addr;
static unsigned int	pci_config_data;
int			moxaart_pci_probed=1;

static int moxaart_pci_read_config_byte(struct pci_bus *bus, unsigned int devfn, int where, u8 *val)
{
	unsigned long   flags;
	u32             v;
	unsigned int    shift;

	spin_lock_irqsave(&moxaart_pci_lock, flags);
	*(volatile unsigned int *)pci_config_addr=CONFIG_CMD(bus->number,devfn,where);	
	v=*(volatile unsigned int *)pci_config_data;
	spin_unlock_irqrestore(&moxaart_pci_lock, flags);
	shift = (where&0x3)*8;
	*val = (v>>shift)&0xff;
	return PCIBIOS_SUCCESSFUL;
}

static int moxaart_pci_read_config_word(struct pci_bus *bus, unsigned int devfn, int where, u16 *val)
{
	unsigned long   flags;
	u32             v;
	unsigned int    shift;

	spin_lock_irqsave(&moxaart_pci_lock, flags);
	*(volatile unsigned int *)pci_config_addr=CONFIG_CMD(bus->number,devfn,where);
	v=*(volatile unsigned int *)pci_config_data;
	spin_unlock_irqrestore(&moxaart_pci_lock, flags);
	shift = (where&0x3)*8;
	*val = (v>>shift)&0xffff;
	return PCIBIOS_SUCCESSFUL;
}

static void moxaart_pci_local_read_config_word(int where, u16 *val)
{
#ifdef LOCAL_READ_WRITE_LOCK_IRQ	// mask by Victor Yu. 05-15-2006
	unsigned long   flags;
#endif
	u32             v;
	unsigned int    shift;

#ifdef LOCAL_READ_WRITE_LOCK_IRQ	// mask by Victor Yu. 05-15-2006
	spin_lock_irqsave(&moxaart_pci_lock, flags);
#else
	spin_lock(&moxaart_pci_lock);
#endif
	*(volatile unsigned int *)pci_config_addr=CONFIG_CMD(0,0,where);
	v=*(volatile unsigned int *)pci_config_data;
#ifdef LOCAL_READ_WRITE_LOCK_IRQ	// mask by Victor Yu. 05-15-2006
	spin_unlock_irqrestore(&moxaart_pci_lock, flags);
#else
	spin_unlock(&moxaart_pci_lock);
#endif
	shift = (where&0x3)*8;
	*val = (v>>shift)&0xffff;
}

static int moxaart_pci_read_config_dword(struct pci_bus *bus, unsigned int devfn, int where, u32 *val)
{
	unsigned long   flags;
	u32             v;

	spin_lock_irqsave(&moxaart_pci_lock, flags);
	*(volatile unsigned int *)pci_config_addr=CONFIG_CMD(bus->number,devfn,where);
	v=*(volatile unsigned int *)pci_config_data;
	spin_unlock_irqrestore(&moxaart_pci_lock, flags);
	*val = v;
	return PCIBIOS_SUCCESSFUL;
}

static void moxaart_pci_local_read_config_dword(int where, u32 *val)
{
#ifdef LOCAL_READ_WRITE_LOCK_IRQ	// mask by Victor Yu. 05-15-2006
	unsigned long   flags;
#endif
	u32             v;

#ifdef LOCAL_READ_WRITE_LOCK_IRQ	// mask by Victor Yu. 05-15-2006
	spin_lock_irqsave(&moxaart_pci_lock, flags);
#else
	spin_lock(&moxaart_pci_lock);
#endif
	*(volatile unsigned int *)pci_config_addr=CONFIG_CMD(0,0,where);
	v=*(volatile unsigned int *)pci_config_data;
#ifdef LOCAL_READ_WRITE_LOCK_IRQ	// mask by Victor Yu. 05-15-2006
	spin_unlock_irqrestore(&moxaart_pci_lock, flags);
#else
	spin_unlock(&moxaart_pci_lock);
#endif
	*val = v;
}

static int moxaart_pci_write_config_byte(struct pci_bus *bus, unsigned int devfn, int where, u8 val)
{
	u32             org_val;
	unsigned long   flags;
	unsigned int    shift;

	shift = (where&0x3)*8;
	spin_lock_irqsave(&moxaart_pci_lock, flags);
	*(volatile unsigned int *)pci_config_addr=CONFIG_CMD(bus->number,devfn,where);
	org_val=*(volatile unsigned int *)pci_config_data;
	org_val=(org_val&~(0xff<<shift))|((u32)val<<shift);
	*(volatile unsigned int *)pci_config_data=org_val;
	spin_unlock_irqrestore(&moxaart_pci_lock, flags);
	return PCIBIOS_SUCCESSFUL;
}

static int moxaart_pci_write_config_word(struct pci_bus *bus, unsigned int devfn, int where, u16 val)
{
        u32             org_val;
	unsigned long   flags;
	unsigned int    shift;

	shift = (where&0x3)*8;
	spin_lock_irqsave(&moxaart_pci_lock, flags);
	*(volatile unsigned int *)pci_config_addr=CONFIG_CMD(bus->number, devfn, where);
	org_val=*(volatile unsigned int *)pci_config_data;
	org_val=(org_val&~(0xffff<<shift))|((u32)val<<shift);
	*(volatile unsigned int *)pci_config_data=org_val;
	spin_unlock_irqrestore(&moxaart_pci_lock, flags);
	return PCIBIOS_SUCCESSFUL;
}

static void moxaart_pci_local_write_config_word(int where, u16 val)
{
        u32             org_val;
#ifdef LOCAL_READ_WRITE_LOCK_IRQ	// mask by Victor Yu. 05-15-2006
	unsigned long   flags;
#endif
	unsigned int    shift;

	shift = (where&0x3)*8;
#ifdef LOCAL_READ_WRITE_LOCK_IRQ	// mask by Victor Yu. 05-15-2006
	spin_lock_irqsave(&moxaart_pci_lock, flags);
#else
	spin_lock(&moxaart_pci_lock);
#endif
	*(volatile unsigned int *)pci_config_addr=CONFIG_CMD(0, 0, where);
	org_val=*(volatile unsigned int *)pci_config_data;
	org_val=(org_val&~(0xffff<<shift))|((u32)val<<shift);
	*(volatile unsigned int *)pci_config_data=org_val;
#ifdef LOCAL_READ_WRITE_LOCK_IRQ	// mask by Victor Yu. 05-15-2006
	spin_unlock_irqrestore(&moxaart_pci_lock, flags);
#else
	spin_unlock(&moxaart_pci_lock);
#endif
}

static int moxaart_pci_write_config_dword(struct pci_bus *bus, unsigned int devfn, int where, u32 val)
{
	unsigned long flags;
	spin_lock_irqsave(&moxaart_pci_lock, flags);
	*(volatile unsigned int *)pci_config_addr=CONFIG_CMD(bus->number, devfn, where);
	*(volatile unsigned int *)pci_config_data=val;
	spin_unlock_irqrestore(&moxaart_pci_lock, flags);
	return PCIBIOS_SUCCESSFUL;
}

static void moxaart_pci_local_write_config_dword(int where, u32 val)
{
#ifdef LOCAL_READ_WRITE_LOCK_IRQ	// mask by Victor Yu. 05-15-2006
	unsigned long flags;
	spin_lock_irqsave(&moxaart_pci_lock, flags);
#else
	spin_lock(&moxaart_pci_lock);
#endif
	*(volatile unsigned int *)pci_config_addr=CONFIG_CMD(0, 0, where);
	*(volatile unsigned int *)pci_config_data=val;
#ifdef LOCAL_READ_WRITE_LOCK_IRQ	// mask by Victor Yu. 05-15-2006
	spin_unlock_irqrestore(&moxaart_pci_lock, flags);
#else
	spin_unlock(&moxaart_pci_lock);
#endif
}

static int moxaart_pci_read_config(struct pci_bus *bus, unsigned int devfn, int where,
			  int size, u32 *val)
{
        int r; 

	switch (size) {
	case 1:
	        r = moxaart_pci_read_config_byte(bus,devfn,where,(u8*)val);
		break;
	case 2:
		r = moxaart_pci_read_config_word(bus,devfn,where,(u16*)val);
		break;
	default:
		r = moxaart_pci_read_config_dword(bus,devfn,where,val);
		break;
	}

	return r;
}

static int moxaart_pci_write_config(struct pci_bus *bus, unsigned int devfn, int where,
			      int size, u32 val)
{
        int r; 

	switch (size) {
	case 1:
 	        r = moxaart_pci_write_config_byte(bus, devfn, where, (u8)val);
		break;
	case 2:
 	        r = moxaart_pci_write_config_word(bus, devfn, where, (u16)val);
		break;
	case 4:
 	        r = moxaart_pci_write_config_dword(bus, devfn, where, val);
		break;
	default:
	        printk( "Invalid size for moxaart_pci_write()\n" );
		r = PCIBIOS_FUNC_NOT_SUPPORTED;  // Luke Lee 03/23/2005 ins 1
	}

	return r;
}

static struct pci_ops moxaart_pci_ops = {
	.read  = moxaart_pci_read_config,
	.write = moxaart_pci_write_config,
};

/* You need to write the physical address to PCI mapping for bridge */
/* You need to modify the source file drivers/pci/setup-res.c */
static struct resource pci_io = {
	.name	= "PCI I/O Space",
#if 1	// mask by Victor Yu. 01-09-2006
	.start	= PCI_IO_VA_BASE,
	.end	= PCI_IO_VA_END-1,
#else
	.start	= MOXAART_PCI_IO_BASE,
	.end	= MOXAART_PCI_IO_END-1,
#endif
	.flags	= IORESOURCE_IO,
};

/* using physical address for memory resource*/
static struct resource pci_mem = {
	.name	= "PCI Memory Space",
#if 1
	.start	= PCI_MEM_VA_BASE,
	.end	= PCI_MEM_VA_END-1,
#else
	.start	= PCI_MEM_BASE,
	.end	= PCI_MEM_END-1,
#endif
	.flags	= IORESOURCE_MEM,
};

// Luke Lee 03/23/2005 unrem 1 rem 1
int __init moxaart_pci_setup_resource(struct resource **resource)
{
	if ( moxaart_pci_probed == 0 )
		return 0;
	if (request_resource(&ioport_resource, &pci_io)) {
		printk(KERN_ERR "PCI: unable to allocate io region\n");
		return 0;
	}
	if (request_resource(&iomem_resource, &pci_mem)) {
		printk(KERN_ERR "PCI: unable to allocate non-prefetchable memory region\n");
		return 0;
	}

	/*
	 * bus->resource[0] is the IO resource for this bus
	 * bus->resource[1] is the mem resource for this bus
	 * bus->resource[2] is the prefetch mem resource for this bus
	 */
	resource[0] = &pci_io;
	resource[1] = &pci_mem;
	resource[2] = NULL;

	return 1;
}

inline int moxaart_pci_get_irq(void)
{
	unsigned int     status;

	moxaart_pci_local_read_config_dword(MOXAART_PCI_IRQ_STATUS_ADDR, &status);
	status >>= 28;
	if ( status & 0x1 )
		return 0;
	if ( status & 0x2 )
		return 1;
	if ( status & 0x4 )
		return 2;
	if ( status & 0x8 )
		return 3;
	return -1;
}

void moxaart_pci_clear_irq(unsigned int irq)
{
	unsigned int     status;

	moxaart_pci_local_read_config_dword(MOXAART_PCI_IRQ_STATUS_ADDR, &status);
	status &= 0xfffffff;
	switch ( irq ) {
	case 0 :
		status |= 0x10000000;
		break;
	case 1 :
		status |= 0x20000000;
		break;
	case 2 :
		status |= 0x40000000;
		break;
	case 3 :
		status |= 0x80000000;
		break;
	}
	moxaart_pci_local_write_config_dword(MOXAART_PCI_IRQ_STATUS_ADDR, status);
}

static int moxaart_pci_probe(void)
{
	*(volatile unsigned int *)pci_config_addr=0x80000000;
	if(*(volatile unsigned int *)pci_config_addr==0x80000000)
		moxaart_pci_probed=1;
	else
		moxaart_pci_probed=0;
	*(volatile unsigned int *)pci_config_addr=0x0;

	return moxaart_pci_probed;
}

void __init moxaart_pci_preinit(void)
{
	u16             val;

	dbg_printk("[moxaart_pci_preinit test01]\n");
	pci_config_addr=MOXAART_PCI_VA_BASE+MOXAART_PCI_CFG_ADR_REG;
	pci_config_data=MOXAART_PCI_VA_BASE+MOXAART_PCI_CFG_DATA_REG;

	if( !moxaart_pci_probe() )
		return;
        
	dbg_printk("[moxaart_pci_preinit test03]\n");
	spin_lock_init(&moxaart_pci_lock);
        
#if 1	// add by Victor Yu. 05-24-2006
	*(volatile unsigned int *)(MOXAART_PCI_VA_BASE+MOXAART_PCI_IOSIZE_REG) = MOXAART_PCI_BASE_IO_SIZE_4M;
	*(volatile unsigned int *)(MOXAART_PCI_VA_BASE+MOXAART_PCI_PROT_REG) = 1;
	moxaart_pci_local_read_config_word(0x0c, &val);
	val &= 0x7ff;
	val |= 0xa800;
	moxaart_pci_local_write_config_word(0x0c, val);
#endif
	moxaart_pci_local_read_config_word(PCI_INT_MASK,&val);
	val|=(PCI_INTA_ENABLE|PCI_INTB_ENABLE|PCI_INTC_ENABLE|PCI_INTD_ENABLE);
	moxaart_pci_local_write_config_word(PCI_INT_MASK,val);
	moxaart_pci_local_write_config_dword(PCI_MEM_BASE_SIZE1, MOXAART_PCI_BASE_ADR_SIZE_1GB);
}

/*
 * This routine handles multiple bridges.
 */
static u8 __init moxaart_swizzle(struct pci_dev *dev, u8 *pinp)
{
        dbg_printk( "moxaart_swizzle(%X,%X)\n\r",(unsigned)dev,(unsigned)pinp );
	return 0;
}

static int __init moxaart_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	dbg_printk("moxaart_map_irq,slot=%d pin=%d\n",PCI_SLOT(dev->devfn),pin);
	switch(PCI_SLOT(dev->devfn)) {
	case 8:
	case 12:
		return VIRQ_PCI_A;
	case 9:
		return VIRQ_PCI_B;
	case 10:
		return VIRQ_PCI_C;
	case 11:
		return VIRQ_PCI_B;
	default:
		dbg_printk("Not Support Slot %d\n",slot);
	break;
	}

	return -1;
}

int __init moxaart_pci_setup(int nr, struct pci_sys_data *sys)
{
	int ret = 0;

	dbg_printk("[moxaart_pci_setup test01]\n");
	if ( moxaart_pci_probed == 0 || nr >= 1 )
		return ret;
	dbg_printk("[moxaart_pci_setup test02]\n");
	return moxaart_pci_setup_resource(sys->resource);
}

static struct pci_bus *moxaart_pci_scan_bus(int nr, struct pci_sys_data *sys)
{
	dbg_printk("[moxaart_pci_scan_bus test01]\n");
	if ( moxaart_pci_probed == 0 )
		return NULL;
	return pci_scan_bus(sys->busnr, &moxaart_pci_ops, sys);
}

static struct hw_pci moxaart_pci  __initdata = {
	.swizzle		= moxaart_swizzle,
	.map_irq		= moxaart_map_irq,
	.setup			= moxaart_pci_setup,
	.nr_controllers		= 1,
	.scan			= moxaart_pci_scan_bus,
	.preinit		= moxaart_pci_preinit,
};

static int __init a320_pci_init(void)
{
	dbg_printk("[pci_init test01]\n");
	if (machine_is_moxaart())
		pci_common_init(&moxaart_pci);
	return 0;
}

subsys_initcall(a320_pci_init);
