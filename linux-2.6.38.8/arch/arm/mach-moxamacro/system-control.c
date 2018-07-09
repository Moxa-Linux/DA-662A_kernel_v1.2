
/*
 * This file is implemented about Moxa Macro System Controller. You can use
 * these APIs to control the system for multi function componment.
 * The license is belonged to Moxa Inc.
 *
 * History :
 * MM-DD-YYYY	Author 			Comment
 * 06-14-2012	Victor Yu.		Initialized it.
 */
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/init.h>
#include <linux/module.h>
#include <mach/map.h>
#include <mach/system-control.h>

static SystemControlReg_t	*SystemControlReg=(SystemControlReg_t *)MOXAARM11_VA_SYSTEM_BASE;

void	moxaarm11_io_pin_select(u32 pin, u32 sel)
{
	u32	val;

	val = readl(&SystemControlReg->IOFunctionSelect);
	if ( sel ) 
		val |= (1 << (pin+PIOA_SHIFT_BIT));
        else
                val &= ~(1 << (pin+PIOA_SHIFT_BIT));
	writel(val, &SystemControlReg->IOFunctionSelect);
}
EXPORT_SYMBOL(moxaarm11_io_pin_select);

void	moxaarm11_io_function_select(enum moxaarm11_io_function sel)
{
	u32	val;

	val = readl(&SystemControlReg->IOFunctionSelect);
	val &= ~SEL_PCI_GPIO_MASK;
	val |= ((sel << 1) & SEL_PCI_GPIO_MASK);
	writel(val, &SystemControlReg->IOFunctionSelect);
}
EXPORT_SYMBOL(moxaarm11_io_function_select);

void	moxaarm11_io_nand_smc(enum moxaarm11_io_nand_smc sel)
{
	u32	val;

	val = readl(&SystemControlReg->IOFunctionSelect);
	val &= ~SEL_NAND;
	val |= sel;
	writel(val, &SystemControlReg->IOFunctionSelect);
}
EXPORT_SYMBOL(moxaarm11_io_nand_smc);

void	moxaarm11_gmac_delay_mode(void)
{
	u32	val;

	val = readl(&SystemControlReg->IOFunctionSelect);
	val |= (GMAC1_RX_DMODE|GMAC1_TX_DMODE|GMAC0_RX_DMODE|GMAC0_TX_DMODE);
	writel(val, &SystemControlReg->IOFunctionSelect);
}
EXPORT_SYMBOL(moxaarm11_gmac_delay_mode);

void moxaarm11_smc_init(int cs)
{
	StaticMemoryReg_t	*smc=(StaticMemoryReg_t *)(MOXAARM11_VA_SMC_BASE + (sizeof(StaticMemoryReg_t)*cs));

	writel(0x06, &smc->IDLE);
	writel(0x0a, &smc->WaitState1);
	writel(0x0a, &smc->WaitState2);
	writel(0x00, &smc->OutEnAssDelay);
	writel(0x01, &smc->WriteEnAssDelay);
	writel(0x40, &smc->Control);
	writel(0x03, &smc->Status);
}
EXPORT_SYMBOL(moxaarm11_smc_init);
