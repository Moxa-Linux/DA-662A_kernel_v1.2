
#ifndef _SYSTEM_CONTROL_H
#define _SYSTEM_CONTROL_H

typedef struct _SystemControlReg_struct {
	unsigned int	ClockSpeedSelection;	// default : 0x00000000
#define PLLAB_CLOCK	1	// CLKA=PLLA, CLKB=PLLB, default vaule
#define XTAL_CLOCK	0	// XTAL clock
#define SDRAM_1_2_CLKA	0	// SDRAM clock source = 1/2 * CLKA
#define SDRAM_1_4_CLKA	(1<<1)	// SDRAM clock source = 1/4 * CLKA
#define SYSCK_SEL_CLKA	0	// AXI_CLK = 1/2 * CLKA
				// AHB_CLK = 1/4 * CLKA
				// APB_CLK = 1/8 * CLKA
#define SYSCK_SEL_CLKB	(1<<2)	// AXI_CLK = 1/2 * CLKB
				// AHB_CLK = 1/4 * CLKB
				// APB_CLK = 1/8 * CLKB
#define CKCHG		(1<<3)	// change the clock multiplexers
				// This bit returns to '0' automatically
#define PLLA_LOCK	(1<<4)	// read only
#define PLLB_LOCK	(1<<5)	// read only
#define PLLDDR2_LOCK	(1<<6)	// read only
	unsigned int	PLLABParamter;	// default : 0x4B824781
#define PLLB_OD_MASK	(3<<30)	// PLLB output divider controll
#define PLLB_OD_SHIFT_BIT	30
#define PLLB_F_MASK	(0x7f<<23)	// PLLB feedback divider control
#define PLLB_F_SHIFT_BIT	23
#define PLLB_R_MASK	(0x1f<<18)	// PLLB divider control
#define PLLB_R_SHIFT_BIT	18
#define PLLB_BS_MASK	(3<<16)		// PLLB output band control
#define PLLB_BS_SHIFT_BIT	16
#define PLLA_OD_MASK	(3<<14)	// PLLA output divider controll
#define PLLA_OD_SHIFT_BIT	14
#define PLLA_F_MASK	(0x7f<<7)	// PLLA feedback divider control
#define PLLA_F_SHIFT_BIT	7
#define PLLA_R_MASK	(0x1f<<2)	// PLLA divider control
#define PLLA_R_SHIFT_BIT	2
#define PLLA_BS_MASK	(3<<0)		// PLLA output band control
#define PLLA_BS_SHIFT_BIT	0
	unsigned int	PLLDDR2Parameter;	// default : 0x00001601
#define PLLDDR2_OD_MASK	(3<<14)	// PLLDDR2 output divider control
#define PLLDDR2_OD_SHIFT_BIT	14
#define PLLDDR2_F_MASK	(0x7f<<7)	// PLLDDR2 feedback divider control
#define PLLDDR2_F_SHIFT_BIT	7
#define PLLDDR2_R_MASK	(0x1f<<2)	// PLLDDR2 divider control
#define PLLDDR2_R_SHIFT_BIT	2
#define PLLDDR2_BS_MASK	(3<<0)		// PLLDDR2 output band control
#define PLLDDR2_BS_SHIFT_BIT	0
	unsigned int	PowerManagement;	// default : 0x00000000
#define USB_WAKEUP_EN	(1<<3)	// enable wake-up from USB
#define USB_WAKEUP_DIS	0	// disable wake-up from USB
#define GMAC_WAKEUP_EN	(1<<2)	// enable wake-up from ethernet
#define GMAC_WAKEUP_DIS	0	// disable wake-up from ethernet
#define GPIO_WAKEUP_EN	(1<<1)	// enable wake-up from GPIO
#define GPIO_WAKEUP_DIS	0	// disable wake-up from GPIO
#define STANDBY_MODE	(1<<0)	// standby mode
#define NORMAL_MODE	0	// normal operation mode
	unsigned int	ChipID;	// default value is 0x4040
	unsigned int	ClockEnable;	// default : 0x00000FFE
#define PLLDDR2_PD	(1<<15)	// PLL DDR2 power off
#define PLLB_PD		(1<<14)	// PLLB power off
#define PLLA_PD		(1<<13)	// PLLA	power off
#define RTCCK_EN	(1<<12)	// RTC clock enable
#define WDTCK_EN	(1<<11)	// WDT clock enable
#define MUARTCK_EN	(1<<10)	// Moxa UART clock enable
#define UARTCK_EN	(1<<9)	// UART clock enable
#define I2CCK_EN	(1<<8)	// I2C controller clock enable
#define SDCK_EN		(1<<7)	// SD controller clock enable
#define TMCK_EN		(1<<6)	// timer clock enable
#define SPICK_EN	(1<<5)	// SPI controller clock enable
#define PCIE_EN		(1<<4)	// normal operation
#define PCI_EN		(1<<3)	// PCI clock enable
#define USB2_EN		(1<<2)	// normal operation
#define USB1_EN		(1<<1)	// normal operation
#define MEM_SEL		(1<<0)	// DDR2 is selected, 0 is for SDR
				// read only, hardware set
	unsigned int	IOFunctionSelect;	// default : 0x000003C0
#define PIOA15_SEL_I2C_SDA1	(1<<31)
#define PIOA14_SEL_I2C_CLK1	(1<<30)
#define PIOA13_SEL_I2C_SDA0	(1<<29)
#define PIOA12_SEL_I2C_CLK0	(1<<28)
#define PIOA11_SEL_GMAC1	(1<<27)	// GMAC1 pmt_intro output
#define PIOA10_SEL_EXT_UART_CLK	(1<<26)	// external UART clock input
#define PIOA9_SEL_PTP_TRIG4	(1<<25)
#define PIOA8_SEL_PTP_TRIG3	(1<<24)
#define PIOA7_SEL_PTP_TRIG2	(1<<23)
#define PIOA6_SEL_PTP_TRIG1	(1<<22)
#define PIOA5_SEL_NPCS		(1<<21)
#define PIOA4_SEL_WD_RST	(1<<20)
#define PIOA3_SEL_PCI_INTDJ	(1<<19)
#define PIOA2_SEL_PCI_INTCJ	(1<<18)
#define PIOA1_SEL_PCI_INTBJ	(1<<17)
#define PIOA0_SEL_PCI_INTAJ	(1<<16)
#define GPIOA_USED_FOR_GPIO	0
#define GPIOA_USED_FOR_OTHERS	1
#define PIOA_GPIO_MASK		0xffff0000
#define PIOA_SHIFT_BIT		16
#define GMAC1_RX_DMODE		(1<<9)	// delay mode
#define GMAC1_TX_DMODE		(1<<8)	// delay mode
#define GMAC0_RX_DMODE		(1<<7)	// delay mode
#define GMAC0_TX_DMODE		(1<<6)	// delay mode
#define SDIO_PULL_UP		(1<<5)// disable pull-up function in SD IO pads
#define DW_EXT_UCLK		(1<<4)	// select external clock for UART
					// 0 : select APB clock for UART
					// default is 0
#define M_EXT_UCLK		(1<<3)	// select external clock for Moxa UART
					// 0 : select APB clock for Moxa UART
					// default is 0
#define SEL_PCI_GPIO_MASK	(3<<1)	// 0 : PCI
					// 1 : Moxa UART * 4 ports
					// 2 : Moxa UART port 2 & 3, GPIOB
					// 3 : FRT, Moxa UART port 2 & 3
#define SEL_PCI			0
#define SEL_MOXA_UART		(1<<1)
#define SEL_GPIOB		(2<<1)
#define SEL_FRT			(3<<1)
#define SEL_NAND		1	// NAND controller
	unsigned int	PinStatus;	// default : 0x00000000
					// read only
#define BTS			(1<<3)	// boot mode selection
#define SMBUSGNT		(1<<2)	// SMC SMBUSGNT
#define SMBUSREQ		(1<<3)	// SMC SMBUSREQ
#define HOLDA			(1<<1)	// EBI HOLDA
#define HOLD			1	// EBI HOLD
	unsigned int	RemapControl;	// default : 0x00000000
#define REMAP_MASK		7
#define REMAP_EBI_NCS		0 // remap EBI_NCS[0] from 0x44000000 to 0x0
#define REMAP_AXI_SRAM		1 // remap AXI SRAM from 0xc0000000 to 0x0
#define REMAP_SDRAM		2 // remap SDRAM from 0x60000000 to 0x0
#define REMAP_DDR2		4 // remap DDR2 from 0x80000000 to 0x0
	unsigned int	PTPTrigger;	// default : 0x00000000
#define PTP_ENTRY4_MASK		(0xf<<12)
#define PTP_ENTRY3_MASK		(0xf<<8)
#define PTP_ENTRY2_MASK		(0xf<<4)
#define PTP_ENTRY1_MASK		(0xf<<0)
} SystemControlReg_t;

/*Define System Control Registers, GMAC SoC componment will be used it*/
#define MOXAARM11_SYSTEM_IO_CONTORL 0x18

enum moxaarm11_io_function{
        io_function_pci = 0,
        io_function_moxauart_0_to_3 = 1,
        io_function_moxauart_2_to_3_GPIOB = 2,
        io_function_moxauart_2_to_3_FRT = 3,
};

enum moxaarm11_io_nand_smc{
        io_smc = 0,
        io_nand = 1,
};

/*
 * The static memory controller registers are started 0x70000000.
 * Each chip select is occupied following structurce size. 
 * The chip select 0 is used register from 0x7000001c. 
 * The chip select 1 is used register from 0x70000038.
 * So, the chip select 7 is used register from 0x70000000.
 */
typedef struct StaticMemoryReg_struct {
	unsigned int	IDLE;	// turn-around cycle between read and write, 
				// used bit 0 - 4
#define IDCY_MASK	0x1f
	unsigned int	WaitState1;	// read cycle, used bit 0 - 5
#define WST1_MASK	0x3f
	unsigned int	WaitState2;	// read cycle, used bit 0 - 4
#define WST2_MASK	0x1f
	unsigned int	OutEnAssDelay;
#define WSTOEN_MASK	0x1f
	unsigned int	WriteEnAssDelay;
#define WSTWEN_MASK	0x3f
	unsigned int	Control;
#define MW_8BIT		0x00
#define MW_16BIT	(1<<6)
#define MW_MASK		(3<<6)
#define BURST_ON	(1<<5)
#define NON_BURST	0
#define WRITE_PROTECT	(1<<4)
#define ACTIVE_HIGH	(1<<3)
#define ACTIVE_LOW	0
#define RBLE_HIGH	0
#define RBLE_LOW	1	// read byte lane enable
	unsigned int	Status;
} StaticMemoryReg_t;

extern void moxaarm11_io_pin_select(u32 pin, u32 sel);
extern void moxaarm11_io_function_select(enum moxaarm11_io_function sel);
extern void moxaarm11_io_nand_smc(enum moxaarm11_io_nand_smc sel);
extern void moxaarm11_gmac_delay_mode(void);
extern void moxaarm11_smc_init(int cs);

#endif	// _SYSTEM_CONTROL_H
