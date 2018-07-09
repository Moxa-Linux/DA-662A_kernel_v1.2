/* 
Author :	Victor Yu.
Date :		11-04-2005
*/

#ifndef     _A320C_H_
#define     _A320C_H_

/*****************************************************************
    IO Mapping
*****************************************************************/
#define IO_BASE	                        0xf0000000
#define MEM_ADDRESS(x)                  ((x&0x0fffffff)+IO_BASE)
#define IO_ADDRESS(x)                   (((x>>4)&0xffff0000) + (x&0xffff) + IO_BASE) 
#define PHY_ADDRESS(x)                  (((x<<4)&0xfff00000)+(x&0xffff))

/******************************************************************
    AHB/APB device register mapping 
 *****************************************************************/
#define CPE_AHB_BASE                    0x90100000
#define CPE_SRAMC_BASE                  0x90200000
#define CPE_SDRAMC_BASE                 0x90300000
#define CPE_AHBDMA_BASE                 0x90400000
#define CPE_APBDMA_BASE                 0x90500000
#define CPE_PMU_BASE                    0x98100000
#define CPE_TIMER_BASE			0x98400000
#define CPE_TIMER1_BASE                 0x98400000
#define CPE_TIMER2_BASE                 0x98400010
#define CPE_GPIO_BASE                   0x98700000  //GPIO controller
#define CPE_IC_BASE                     0x98800000	//Interrupt controllor 
#define CPE_SD_BASE                    	0x98e00000
#define CPE_PCI_BASE                    0x90c00000
#define CPE_PCI_MEM                     0xa0000000
#define CPE_FTMAC_BASE                  0x90900000
#define CPE_FTMAC2_BASE                 0x92000000	// 2nd MAC
#define CPE_USBDEV_BASE                 0x90b00000   //USB device

#define MEM_PHY_ADDRESS(x)		((x&0x0fffffff)+CPE_PCI_MEM)

#define CPE_UART_BASE			0x98200000
#define CPE_UART1_BASE			0x98200000
#define CPE_UART2_BASE			0x98200020
#define CPE_UART3_BASE			0x98200040
#define CPE_UART4_BASE			0x98200060
#define CPE_UART5_BASE			0x98200080
#define CPE_UART6_BASE			0x982000a0
#define CPE_UART_INT_VEC_BASE		0x982000c0
#define CPE_UART_MODE_BASE		0x982000e4
#define CPE_SPI_BASE                    0x98b00000
#define CPE_USBHOST_BASE		0x90a00000
#define CPE_AES_DES_BASE		0x90f00000
#define CPE_AC97_BASE			0x99400000
#define CPE_RTC_BASE			0x98600000
#define CPE_WATCHDOG_BASE		0x98500000
#define CPE_EBI_BASE			0x92300000
//#if defined CONFIG_ARCH_W311
//#define CPE_WLAN_LED_REG_BASE		0x84000000
//#endif

/**********************************************
virtual address
**********************************************/
#define CPE_AHB_VA_BASE			IO_ADDRESS(CPE_AHB_BASE)
#define CPE_SRAMC_VA_BASEi		IO_ADDRESS(CPE_SRAMC_BASE)
#define CPE_SDRAMC_VA_BASE		IO_ADDRESS(CPE_SDRAMC_BASE)
#define CPE_AHBDMA_VA_BASE		IO_ADDRESS(CPE_AHBDMA_BASE)
#define CPE_APBDMA_VA_BASE		IO_ADDRESS(CPE_APBDMA_BASE)
#define CPE_PMU_VA_BASE			IO_ADDRESS(CPE_PMU_BASE)
#define CPE_TIMER_VA_BASE		IO_ADDRESS(CPE_TIMER_BASE)
#define CPE_TIMER1_VA_BASE		IO_ADDRESS(CPE_TIMER1_BASE)
#define CPE_TIMER2_VA_BASE		IO_ADDRESS(CPE_TIMER2_BASE)
#define CPE_GPIO_VA_BASE		IO_ADDRESS(CPE_GPIO_BASE)
#define CPE_IC_VA_BASE			IO_ADDRESS(CPE_IC_BASE)
#define CPE_SD_VA_BASE			IO_ADDRESS(CPE_SD_BASE)
#define CPE_PCI_VA_BASE			IO_ADDRESS(CPE_PCI_BASE)
#define CPE_PCI_VA_MEM			IO_ADDRESS(CPE_PCI_MEM)
#define CPE_FTMAC_VA_BASE		IO_ADDRESS(CPE_FTMAC_BASE)
#define CPE_FTMAC2_VA_BASE		IO_ADDRESS(CPE_FTMAC2_BASE)	//2nd MAC
#define CPE_USBDEV_VA_BASE		IO_ADDRESS(CPE_USBDEV_BASE)	//USB device

#define CPE_UART_VA_BASE		IO_ADDRESS(CPE_UART_BASE)
#define CPE_UART1_VA_BASE		IO_ADDRESS(CPE_UART1_BASE)
#define CPE_UART2_VA_BASE		IO_ADDRESS(CPE_UART2_BASE)
#define CPE_UART3_VA_BASE		IO_ADDRESS(CPE_UART3_BASE)
#define CPE_UART4_VA_BASE		IO_ADDRESS(CPE_UART4_BASE)
#define CPE_UART5_VA_BASE		IO_ADDRESS(CPE_UART5_BASE)
#define CPE_UART6_VA_BASE		IO_ADDRESS(CPE_UART6_BASE)
#define CPE_UART_INT_VEC_VA_BASE	IO_ADDRESS(CPE_UART_INT_VEC_BASE)
#define CPE_UART_MODE_VA_BASE		IO_ADDRESS(CPE_UART_MODE_BASE)
#define CPE_SPI_VA_BASE			IO_ADDRESS(CPE_SPI_BASE)
#define CPE_USBHOST_VA_BASE		IO_ADDRESS(CPE_USBHOST_BASE)
#define CPE_AES_DES_VA_BASE		IO_ADDRESS(CPE_AES_DES_BASE)
#define CPE_AC97_VA_BASE		IO_ADDRESS(CPE_AC97_BASE)
#define CPE_RTC_VA_BASE			IO_ADDRESS(CPE_RTC_BASE)
#define CPE_WATCHDOG_VA_BASE		IO_ADDRESS(CPE_WATCHDOG_BASE)
#define CPE_EBI_VA_BASE			IO_ADDRESS(CPE_EBI_BASE)
//#if defined CONFIG_ARCH_W311
//#define CPE_WLAN_LED_REG_VA_BASE	IO_ADDRESS( CPE_WLAN_LED_REG_BASE)
//#endif

/*****************************************************************
    Clock Setting
*****************************************************************/
#define AHB_CLK                     	96000000
#define APB_CLK                     	48000000
//#define fMCLK_MHz			48000000
#define fMCLK_MHz			APB_CLK
#define CONFIG_UART_CLK			14745600
#define CONFIG_SYS_CLK			192000000
#define CONFIG_SERIAL_CPE_DEFAULT_BAUD	115200

/*****************************************************************
    IRQ
*****************************************************************/
/*
interrupt:
Following is used for Moxa CPU.
 0-31       F526 irq
 32-63      F526 fiq
 64 - 67    Virtual IRQ (PCI)
 68 - 99    Virtual IRQ (reserved)
 */
#define CPE_NR_IRQS                     32
#define CPE_NR_FIQS                     32
#define CPE_NR_BASIC_IRQS		(CPE_NR_IRQS+CPE_NR_FIQS)

#define CPE_VIRQ_START			CPE_NR_BASIC_IRQS
#define NR_IRQS				(CPE_NR_BASIC_IRQS+16)

#define VIRQ_PCI_A                      (0+CPE_VIRQ_START)
#define VIRQ_PCI_B                      (1+CPE_VIRQ_START)
#define VIRQ_PCI_C                      (2+CPE_VIRQ_START)
#define VIRQ_PCI_D                      (3+CPE_VIRQ_START)

//irq number
#define IRQ_GPIO			13
#define IRQ_MAC                         25
#define IRQ_TIMER1                      19
#define IRQ_TIMER2                      14 
#define IRQ_TIMER3                      15

#define IRQ_UART			31
#define IRQ_AES_DES			29
#define IRQ_USBHOST			28
#define IRQ_MAC2			27
#define IRQ_PCI				26
#define IRQ_APBDMA			24
#define IRQ_DMAC_ERR			23
#define IRQ_DMAC_TC			22
#define IRQ_DMAC			21
#define IRQ_RTC_SECOND			18
#define IRQ_RTC_ALARM			17
#define IRQ_WATCHDOG			16
#define IRQ_USBDEV_RESUME		12
#define IRQ_USBDEV			11
#define IRQ_PMU				8
#define IRQ_AC97			6
#define IRQ_SD				5
#define IRQ_SPI				2

#define LEVEL                           0
#define EDGE                            1
#define H_ACTIVE                        0
#define L_ACTIVE                        1

#define IRQ_SOURCE_REG                  0
#define IRQ_MASK_REG                    0x04
#define IRQ_CLEAR_REG                   0x08
#define IRQ_MODE_REG                    0x0c
#define IRQ_LEVEL_REG                   0x10
#define IRQ_STATUS_REG                  0x14

#define FIQ_SOURCE_REG                  0x20
#define FIQ_MASK_REG                    0x24
#define FIQ_CLEAR_REG                   0x28
#define FIQ_MODE_REG                    0x2c
#define FIQ_LEVEL_REG                   0x30
#define FIQ_STATUS_REG                  0x34


/*****************************************************************
    Flash
*****************************************************************/
#define CPE_FLASH_BASE                  0x80000000
#define CPE_FLASH_SZ                    0x01000000
#define CPE_FLASH_VA_BASE               0xf4000000
#ifdef CONFIG_ARCH_IA241_32128	// add by Victor Yu. 05-22-2007
#define CPE_FLASH_BASE2			0x88000000
#define CPE_FLASH_VA_BASE2		0xf5000000
#endif	// CONFIG_ARCH_IA241_32128

/*****************************************************************
    PCI
*****************************************************************/
#define PCI_IO_VA_BASE                  (CPE_PCI_VA_BASE+SZ_4K)
#define PCI_IO_VA_SIZE                  (SZ_64K-SZ_4K)
#define PCI_IO_VA_END                   (CPE_PCI_VA_BASE+SZ_64K)
#define CPE_PCI_IO_BASE			(CPE_PCI_BASE+SZ_4K)
#define CPE_PCI_IO_SIZE			PCI_IO_VA_SIZE
#define CPE_PCI_IO_END			(CPE_PCI_BASE+SZ_64K)

#define PCI_MEM_BASE                    CPE_PCI_MEM
#if 0	// mask by Victor Yu. 10-27-2005
#define PCI_MEM_SIZE                    SZ_1M
#else	// add by Victor Yu. 10-27-2005, change the size
#define PCI_MEM_SIZE                    SZ_32M
#endif
#define PCI_MEM_END                     (CPE_PCI_MEM+PCI_MEM_SIZE)
#define PCI_MEM_VA_BASE			MEM_ADDRESS(CPE_PCI_MEM)
#define PCI_MEM_VA_END			(PCI_MEM_VA_BASE+PCI_MEM_SIZE)	

#define PCI_BRIDGE_DEVID                0x4321
#define PCI_BRIDGE_VENID                0x159b

/*****************************************************************
    PMU
*****************************************************************/
#define PMU_SSP_DMA_CHANNEL             0x2

#endif	// _A320C_H
