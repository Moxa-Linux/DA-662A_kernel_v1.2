/*
 * arch/arm/mach-moxaarm11/include/mach/io.h
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

#include <linux/bitops.h>
#include <mach/hardware.h>
#include <mach/map.h>
#include <mach/pcie.h>

#define IO_SPACE_LIMIT 0xffffffff

/*PCI control and status registers*/
#define PCI_CRP_AD_CBE_OFFSET   0x00
#define PCI_CRP_WDATA_OFFSET    0x04
#define PCI_CRP_RDATA_OFFSET    0x08
#define PCI_CIO_AD_OFFSET       0x0c
#define PCI_CIO_CBE_OFFSET      0x10
#define PCI_CIO_WDATA_OFFSET    0x14
#define PCI_CIO_RDATA_OFFSET    0x18
#define PCI_ERR_OFFSET          0x1c
#define PCI_ERR_ADDR_OFFSET	0x20
#define PCI_AHB_ERR_OFFSET      0x24
#define PCI_AHB_ERR_ADDR_OFFSET	0x28
#define PCI_FLUSH_FIFO_OFFSET   0x2c

#define CRP_AD_CBE_BESL		20
#define CRP_CMD_CONFIGREAD      0x000a0000
#define CRP_CMD_CONFIGWRITE     0x000b0000

#define MOXAARM11_PCI_CIO_CBE_BESL      4
#define CIO_CMD_CONFIGREAD      0x0a
#define CIO_CMD_CONFIGWRITE     0x0b
#define CIO_CMD_IO_READ		0x02
#define CIO_CMD_IO_WRITE	0x03
#define CIO_CMD_MEM_READ	0x06
#define CIO_CMD_MEM_WRITE	0x07

extern u32	moxaarm11_pci_read(u32 addr, u32 cmd);
extern void	moxaarm11_pci_write(u32 addr, u32 cmd, u32 data);

#define __mem_pci(a)	(a)
#ifndef CONFIG_PCI
#define	__io(v)		__typesafe_io(v)
#else

#if 0
//
// redefine the memory access
//
#define	__is_pci_mem_address(p)	((unsigned long)p >= MOXAARM11_PA_PCI_MEM_BASE && (unsigned long)p < MOXAARM11_PA_PCI_IO_BASE)

static inline void __iomem * __moxaarm11_ioremap(unsigned long addr, size_t size,
                                                unsigned int mtype)
{
        if (!__is_pci_mem_address(addr))
                return __arm_ioremap(addr, size, mtype);

        return (void __iomem *)addr;
}

static inline void __moxaarm11_iounmap(void __iomem *addr)
{
        if ( !__is_pci_mem_address((__force u32)addr) )
                __iounmap(addr);
}

#define __arch_ioremap		__moxaarm11_ioremap
#define __arch_iounmap		__moxaarm11_iounmap

#define writeb(v, p)		__moxaarm11_writeb(v, p)
#define writew(v, p)		__moxaarm11_writew(v, p)
#define writel(v, p)		__moxaarm11_writel(v, p)

#define writesb(p, v, l)	__moxaarm11_writesb(p, v, l)
#define writesw(p, v, l)	__moxaarm11_writesw(p, v, l)
#define writesl(p, v, l)	__moxaarm11_writesl(p, v, l)

#define readb(p)		__moxaarm11_readb(p)
#define readw(p)		__moxaarm11_readw(p)
#define readl(p)		__moxaarm11_readl(p)

#define readsb(p, v, l)		__moxaarm11_readsb(p, v, l)
#define readsw(p, v, l)		__moxaarm11_readsw(p, v, l)
#define readsl(p, v, l)		__moxaarm11_readsl(p, v, l)

static inline void 
__moxaarm11_writeb(u8 value, volatile void __iomem *p)
{
	u32	addr, n, byte_enables, data;		

	addr = (u32)p;
	if ( __is_pci_mem_address(addr) ) {
		__raw_writeb(value, addr);
		return;
	}

	n = addr & 3;
	byte_enables = (0xf & ~BIT(n)) << MOXAARM11_PCI_CIO_CBE_BESL;
	data = value << (8*n);
	moxaarm11_pci_write(addr, byte_enables | CIO_CMD_MEM_WRITE, data);
}

static inline void 
__moxaarm11_writesb(volatile void __iomem *io_addr, const u8 *vaddr, u32 count)
{
	while (count--)
		writeb(*vaddr++, io_addr);
}

static inline void 
__moxaarm11_writew(u16 value, volatile void __iomem *p)
{
	u32	addr, n, byte_enables, data;		

	addr = (u32)p;
	if ( __is_pci_mem_address(addr) ) {
		__raw_writew(value, addr);
		return;
	}

	n = addr & 3;
	byte_enables = (0xf & ~(BIT(n) | BIT(n+1))) << MOXAARM11_PCI_CIO_CBE_BESL;
	data = value << (8*n);
	moxaarm11_pci_write(addr, byte_enables | CIO_CMD_MEM_WRITE, data);
}

static inline void 
__moxaarm11_writesw(volatile void __iomem *io_addr, const u16 *vaddr, u32 count)
{
	while (count--)
		writew(*vaddr++, io_addr);
}

static inline void 
__moxaarm11_writel(u32 value, volatile void __iomem *p)
{
	u32	addr;

	addr = (u32)p;
	if ( __is_pci_mem_address(addr) ) {
		__raw_writel(value, addr);
		return;
	}

	moxaarm11_pci_write(addr, CIO_CMD_MEM_WRITE, value);
}

static inline void 
__moxaarm11_writesl(volatile void __iomem *io_addr, const u32 *vaddr, u32 count)
{
	while (count--)
		writel(*vaddr++, io_addr);
}

static inline u8 
__moxaarm11_readb(const volatile void __iomem *p)
{
	u32	addr, n, byte_enables, data;

	addr = (u32)p;
	if ( __is_pci_mem_address(addr) )
		return __raw_readb(addr);

	n = addr & 3;
	byte_enables = (0xf & ~BIT(n)) << MOXAARM11_PCI_CIO_CBE_BESL;
	data = moxaarm11_pci_read(addr, byte_enables | CIO_CMD_MEM_READ);

	return data >> (8*n);
}

static inline void 
__moxaarm11_readsb(const volatile void __iomem *io_addr, u8 *vaddr, u32 count)
{
	while (count--)
		*vaddr++ = readb(io_addr);
}

static inline u16 
__moxaarm11_readw(const volatile void __iomem *p)
{
	u32	addr, n, byte_enables, data;

	addr = (u32)p;
	if ( __is_pci_mem_address(addr) )
		return __raw_readw(addr);

	n = addr & 3;
	byte_enables = (0xf & ~(BIT(n)|BIT(n+1))) << MOXAARM11_PCI_CIO_CBE_BESL;
	data = moxaarm11_pci_read(addr, byte_enables | CIO_CMD_MEM_READ);

	return data >> (8*n);
}

static inline void 
__moxaarm11_readsw(const volatile void __iomem *io_addr, u8 *vaddr, u32 count)
{
	while (count--)
		*vaddr++ = readw(io_addr);
}

static inline u32 
__moxaarm11_readl(const volatile void __iomem *p)
{
	u32	addr, data;

	addr = (u32)p;
	if ( __is_pci_mem_address(addr) )
		return __raw_readw(addr);

	data = moxaarm11_pci_read(addr, CIO_CMD_MEM_READ);

	return data;
}

static inline void 
__moxaarm11_readsl(const volatile void __iomem *io_addr, u8 *vaddr, u32 count)
{
	while (count--)
		*vaddr++ = readl(io_addr);
}

/*
 * We can use the built-in functions b/c they end up calling writeb/readb
 */
#define memset_io(c,v,l)                _memset_io((c),(v),(l))
#define memcpy_fromio(a,c,l)            _memcpy_fromio((a),(c),(l))
#define memcpy_toio(c,a,l)              _memcpy_toio((c),(a),(l))
#endif

// -------------------------------------------------------------------
// -------------------------------------------------------------------
// redefine the I/O access
//
#define	outb(p, v)		__moxaarm11_outb(p, v)
#define	outw(p, v)		__moxaarm11_outw(p, v)
#define	outl(p, v)		__moxaarm11_outl(p, v)
	
#define	outsb(p, v, l)		__moxaarm11_outsb(p, v, l)
#define	outsw(p, v, l)		__moxaarm11_outsw(p, v, l)
#define	outsl(p, v, l)		__moxaarm11_outsl(p, v, l)

#define	inb(p)			__moxaarm11_inb(p)
#define	inw(p)			__moxaarm11_inw(p)
#define	inl(p)			__moxaarm11_inl(p)

#define	insb(p, v, l)		__moxaarm11_insb(p, v, l)
#define	insw(p, v, l)		__moxaarm11_insw(p, v, l)
#define	insl(p, v, l)		__moxaarm11_insl(p, v, l)

#define	__is_pci_io_address(p)	((unsigned long)p >= MOXAARM11_PA_PCI_IO_BASE)
static inline void __moxaarm11_outb(u8 value, u32 addr)
{
	u32		n, byte_enables, data;		
#ifdef CONFIG_PCIE
	unsigned long	flags;

	if (addr >= MOXAARM11_VA_PCIE_IO_BASE && addr < MOXAARM11_VA_PCIE_IO_END){
		spin_lock_irqsave(&moxaarm11_pcie_lock, flags);
		__raw_writel(PCIE_ADDR_VIEW_OUT | PCIE_ADDR_VIEW_REGION2, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);

		__raw_writeb(value, (void *)addr);

		__raw_writel(PCIE_ADDR_VIEW_IN | PCIE_ADDR_VIEW_REGION2, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);	
		spin_unlock_irqrestore(&moxaarm11_pcie_lock, flags);	
		return;
	}
#endif
	if ( !__is_pci_io_address(addr) ) {
		__raw_writeb(value, addr);
		return;
	}
	n = addr & 3;
	byte_enables = (0xf & ~BIT(n)) << MOXAARM11_PCI_CIO_CBE_BESL;
	data = value << (8*n);
	moxaarm11_pci_write(addr, byte_enables | CIO_CMD_IO_WRITE, data);
}

static inline void __moxaarm11_outsb(u32 io_addr, const u8 *vaddr, u32 count)
{
	while (count--)
		outb(*vaddr++, io_addr);
}

static inline void __moxaarm11_outw(u16 value, u32 addr)
{
	u32		n, byte_enables, data;
#ifdef CONFIG_PCIE
	unsigned long	flags;

	if (addr >= MOXAARM11_VA_PCIE_IO_BASE && addr < MOXAARM11_VA_PCIE_IO_END){
		spin_lock_irqsave(&moxaarm11_pcie_lock, flags);
		__raw_writel(PCIE_ADDR_VIEW_OUT | PCIE_ADDR_VIEW_REGION2, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);

		__raw_writew(value, (void *)addr);

		__raw_writel(PCIE_ADDR_VIEW_IN | PCIE_ADDR_VIEW_REGION2, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);	
		spin_unlock_irqrestore(&moxaarm11_pcie_lock, flags);	
		return;
	}
#endif		
	if ( !__is_pci_io_address(addr) ) {
		__raw_writew(value, addr);
		return;
	}
	n = addr & 3;
	byte_enables = (0xf & ~(BIT(n) | BIT(n+1))) << MOXAARM11_PCI_CIO_CBE_BESL;
	data = value << (8*n);
	moxaarm11_pci_write(addr, byte_enables | CIO_CMD_IO_WRITE, data);
}

static inline void __moxaarm11_outsw(u32 io_addr, const u16 *vaddr, u32 count)
{
	while (count--)
		outw(cpu_to_le16(*vaddr++), io_addr);
}

static inline void __moxaarm11_outl(u32 value, u32 addr)
{
#ifdef CONFIG_PCIE
	unsigned long	flags;

	if (addr >= MOXAARM11_VA_PCIE_IO_BASE && addr < MOXAARM11_VA_PCIE_IO_END){
		spin_lock_irqsave(&moxaarm11_pcie_lock, flags);
		__raw_writel(PCIE_ADDR_VIEW_OUT | PCIE_ADDR_VIEW_REGION2, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);

		__raw_writel(value, (void *)addr);

		__raw_writel(PCIE_ADDR_VIEW_IN | PCIE_ADDR_VIEW_REGION2, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);	
		spin_unlock_irqrestore(&moxaarm11_pcie_lock, flags);	
		return;
	}
#endif
	if ( !__is_pci_io_address(addr) ) {
		__raw_writel(value, addr);
		return;
	}
	moxaarm11_pci_write(addr, CIO_CMD_IO_WRITE, value);
}

static inline void __moxaarm11_outsl(u32 io_addr, const u32 *vaddr, u32 count)
{
	while (count--)
		outl(*vaddr++, io_addr);
}

static inline u8 __moxaarm11_inb(u32 addr)
{
	u32		n, byte_enables, data;
#ifdef CONFIG_PCIE
	unsigned long	flags;
	
	if (addr >= MOXAARM11_VA_PCIE_IO_BASE && addr < MOXAARM11_VA_PCIE_IO_END){
		spin_lock_irqsave(&moxaarm11_pcie_lock, flags);

		__raw_writel(PCIE_ADDR_VIEW_OUT | PCIE_ADDR_VIEW_REGION2, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);
		data = __raw_readb((void *)addr); 		
		__raw_writel(PCIE_ADDR_VIEW_IN | PCIE_ADDR_VIEW_REGION2, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);	

		spin_unlock_irqrestore(&moxaarm11_pcie_lock, flags);	

		return data;
	}
#endif	
	if ( !__is_pci_io_address(addr) )
		return __raw_readb(addr);
	n = addr & 3;
	byte_enables = (0xf & ~BIT(n)) << MOXAARM11_PCI_CIO_CBE_BESL;
	data = moxaarm11_pci_read(addr, byte_enables | CIO_CMD_IO_READ);

	return data >> (8*n);
}

static inline void __moxaarm11_insb(u32 io_addr, u8 *vaddr, u32 count)
{
	while (count--)
		*vaddr++ = inb(io_addr);
}

static inline u16 __moxaarm11_inw(u32 addr)
{
	u32		n, byte_enables, data;
#ifdef CONFIG_PCIE
	unsigned long	flags;
	
	if (addr >= MOXAARM11_VA_PCIE_IO_BASE && addr < MOXAARM11_VA_PCIE_IO_END){
		spin_lock_irqsave(&moxaarm11_pcie_lock, flags);

		__raw_writel(PCIE_ADDR_VIEW_OUT | PCIE_ADDR_VIEW_REGION2, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);
		data = __raw_readw((void *)addr); 		
		__raw_writel(PCIE_ADDR_VIEW_IN | PCIE_ADDR_VIEW_REGION2, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);	

		spin_unlock_irqrestore(&moxaarm11_pcie_lock, flags);	

		return data;
	}
#endif		
	if ( !__is_pci_io_address(addr) )
		return __raw_readw(addr);
	n = addr & 3;
	byte_enables = (0xf & ~(BIT(n) | BIT(n+1))) << MOXAARM11_PCI_CIO_CBE_BESL;
	data = moxaarm11_pci_read(addr, byte_enables | CIO_CMD_IO_READ);

	return data>>(8*n);
}

static inline void __moxaarm11_insw(u32 io_addr, u16 *vaddr, u32 count)
{
	while (count--)
		*vaddr++ = le16_to_cpu(inw(io_addr));
}

static inline u32 __moxaarm11_inl(u32 addr)
{
	u32		data;
#ifdef CONFIG_PCIE
	unsigned long	flags;
	
	if (addr >= MOXAARM11_VA_PCIE_IO_BASE && addr < MOXAARM11_VA_PCIE_IO_END){
		spin_lock_irqsave(&moxaarm11_pcie_lock, flags);

		__raw_writel(PCIE_ADDR_VIEW_OUT | PCIE_ADDR_VIEW_REGION2, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);
		data = __raw_readl((void *)addr); 		
		__raw_writel(PCIE_ADDR_VIEW_IN | PCIE_ADDR_VIEW_REGION2, MOXAARM11_VA_PCIE_BASE + PCIE_ADDR_VIEW_OFF);	

		spin_unlock_irqrestore(&moxaarm11_pcie_lock, flags);	

		return data;
	}
#endif	
	if ( !__is_pci_io_address(addr) )
		return __raw_readl(addr);
	data = moxaarm11_pci_read(addr, CIO_CMD_IO_READ);

	return data;
}

static inline void __moxaarm11_insl(u32 io_addr, u32 *vaddr, u32 count)
{
	while (count--)
		*vaddr++ = inl(io_addr);
}

static inline unsigned int __moxaarm11_ioread8(const void __iomem *addr)
{
	unsigned long port = (unsigned long __force)addr;

	if (__is_pci_io_address(port))
		return	(unsigned int)__moxaarm11_inb(port);
	else
		return (unsigned int)__raw_readb(addr);
}

static inline void __moxaarm11_ioread8_rep(const void __iomem *addr, void *vaddr, u32 count)
{
	unsigned long port = (unsigned long __force)addr;

	if (__is_pci_io_address(port))
		__moxaarm11_insb(port, vaddr, count);
	else
		__raw_readsb(addr, vaddr, count);
}

static inline unsigned int __moxaarm11_ioread16(const void __iomem *addr)
{
	unsigned long port = (unsigned long __force)addr;

	if (__is_pci_io_address(port))
		return	(unsigned int)__moxaarm11_inw(port);
	else
		return (unsigned int)__raw_readw(addr);
}

static inline void __moxaarm11_ioread16_rep(const void __iomem *addr, void *vaddr, u32 count)
{
	unsigned long port = (unsigned long __force)addr;

	if (__is_pci_io_address(port))
		__moxaarm11_insw(port, vaddr, count);
	else
		__raw_readsw(addr, vaddr, count);
}

static inline unsigned int __moxaarm11_ioread32(const void __iomem *addr)
{
	unsigned long port = (unsigned long __force)addr;
	if (__is_pci_io_address(port))
		return	(unsigned int)__moxaarm11_inl(port);
	else 
		return (unsigned int)__raw_readl(addr);
}

static inline void __moxaarm11_ioread32_rep(const void __iomem *addr, void *vaddr, u32 count)
{
	unsigned long port = (unsigned long __force)addr;

	if (__is_pci_io_address(port))
		__moxaarm11_insl(port, vaddr, count);
	else
		__raw_readsl(addr, vaddr, count);
}

static inline void __moxaarm11_iowrite8(u8 value, void __iomem *addr)
{
	unsigned long port = (unsigned long __force)addr;

	if (__is_pci_io_address(port))
		__moxaarm11_outb(value, port);
	else
		__raw_writeb(value, addr);
}

static inline void __moxaarm11_iowrite8_rep(void __iomem *addr, const void *vaddr, u32 count)
{
	unsigned long port = (unsigned long __force)addr;

	if (__is_pci_io_address(port))
		__moxaarm11_outsb(port, vaddr, count);
	else
		__raw_writesb(addr, vaddr, count);
}

static inline void __moxaarm11_iowrite16(u16 value, void __iomem *addr)
{
	unsigned long port = (unsigned long __force)addr;

	if (__is_pci_io_address(port))
		__moxaarm11_outw(value, port);
	else
		__raw_writew(value, addr);
}

static inline void __moxaarm11_iowrite16_rep(void __iomem *addr, const void *vaddr, u32 count)
{
	unsigned long port = (unsigned long __force)addr;

	if (__is_pci_io_address(port))
		__moxaarm11_outsw(port, vaddr, count);
	else
		__raw_writesw(addr, vaddr, count);
}

static inline void __moxaarm11_iowrite32(u32 value, void __iomem *addr)
{
	unsigned long port = (unsigned long __force)addr;

	if (__is_pci_io_address(port))
		__moxaarm11_outl(value, port);
	else
		__raw_writel(value, addr);
}

static inline void __moxaarm11_iowrite32_rep(void __iomem *addr, const void *vaddr, u32 count)
{
	unsigned long port = (unsigned long __force)addr;

	if (__is_pci_io_address(port))
		__moxaarm11_outsl(port, vaddr, count);
	else
		__raw_writesl(addr, vaddr, count);
}

#define	ioread8(p)			__moxaarm11_ioread8(p)
#define	ioread16(p)			__moxaarm11_ioread16(p)
#define	ioread32(p)			__moxaarm11_ioread32(p)

#define	ioread8_rep(p, v, c)		__moxaarm11_ioread8_rep(p, v, c)
#define	ioread16_rep(p, v, c)		__moxaarm11_ioread16_rep(p, v, c)
#define	ioread32_rep(p, v, c)		__moxaarm11_ioread32_rep(p, v, c)

#define	iowrite8(v,p)			__moxaarm11_iowrite8(v,p)
#define	iowrite16(v,p)			__moxaarm11_iowrite16(v,p)
#define	iowrite32(v,p)			__moxaarm11_iowrite32(v,p)

#define	iowrite8_rep(p, v, c)		__moxaarm11_iowrite8_rep(p, v, c)
#define	iowrite16_rep(p, v, c)		__moxaarm11_iowrite16_rep(p, v, c)
#define	iowrite32_rep(p, v, c)		__moxaarm11_iowrite32_rep(p, v, c)

#define	ioport_map(port, nr)		((void __iomem*)(port))
#define	ioport_unmap(addr)

#endif	// !CONFIG_PCI

#endif	//  __ASM_ARM_ARCH_IO_H
