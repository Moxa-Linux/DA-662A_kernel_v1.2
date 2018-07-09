/* 
 * linux/drivers/mmc/host/moxaart_sd.h
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
#ifndef _MOXAART_SD_H
#define _MOXAART_SD_H

//#define MOXAART_SD_DEBUG
#ifdef MOXAART_SD_DEBUG
#define dbg_printk(fmt, s...)	printk("%s,%d:" fmt, __FUNCTION__, __LINE__, ##s)
#else
#define dbg_printk(fmt, s...)
#endif


// register
#define SD_CMD_REG             0
#define SD_ARG_REG             4
#define SD_RESP0_REG           8
#define SD_RESP1_REG           0x0c
#define SD_RESP2_REG           0x10
#define SD_RESP3_REG           0x14
#define SD_RESP_CMD_REG        0x18
#define SD_DATA_CTRL_REG       0x1c
#define SD_DATA_TIMER_REG      0x20
#define SD_DATA_LEN_REG        0x24
#define SD_STATUS_REG          0x28
#define SD_CLEAR_REG           0x2c
#define SD_INT_MASK_REG        0x30
#define SD_POWER_CTRL_REG      0x34
#define SD_CLOCK_CTRL_REG      0x38
#define SD_BUS_WIDTH_REG       0x3c
#define SD_DATA_WIN_REG        0x40
#define SD_FEATURE_REG         0x44
#define SD_REVISION_REG        0x48

typedef struct _moxaart_sd_reg {
        unsigned int    command;
#define SD_SDC_RST                     (1<<10)
#define SD_CMD_EN                      (1<<9)
#define SD_APP_CMD                     (1<<8)
#define SD_LONG_RSP                    (1<<7)
#define SD_NEED_RSP                    (1<<6)
#define SD_CMD_IDX_MASK                0x3f
        unsigned int    argument;
        unsigned int    response0;
        unsigned int    response1;
        unsigned int    response2;
        unsigned int    response3;
        unsigned int    response_command;
#define SD_RSP_CMD_APP                 (1<<6)
#define SD_RSP_CMD_IDX_MASK            0x3f
        unsigned int    data_control;
#define SD_DATA_EN                     (1<<6)
#define SD_DMA_EN                      (1<<5)
#define SD_DATA_WRITE                  (1<<4)
#define SD_BLK_SIZE_MASK               0x0f
        unsigned int    data_timer;
        unsigned int    data_length;
#define SD_DATA_LEN_MASK               0xffffff
        unsigned int    status;
#define SD_WRITE_PROT                  (1<<12)
#define SD_CARD_DETECT                 (1<<11)
#define SD_CARD_CHANGE                 (1<<10)
#define SD_FIFO_ORUN                   (1<<9)
#define SD_FIFO_URUN                   (1<<8)
#define SD_DATA_END                    (1<<7)
#define SD_CMD_SENT                    (1<<6)
#define SD_DATA_CRC_OK                 (1<<5)
#define SD_RSP_CRC_OK                  (1<<4)
#define SD_DATA_TIMEOUT                (1<<3)
#define SD_RSP_TIMEOUT                 (1<<2)
#define SD_DATA_CRC_FAIL               (1<<1)
#define SD_RSP_CRC_FAIL                (1<<0)
        unsigned int    clear;
#define SD_CLR_CARD_CHANGE             (1<<10)
#define SD_CLR_FIFO_ORUN               (1<<9)
#define SD_CLR_FIFO_URUN               (1<<8)
#define SD_CLR_DATA_END                (1<<7)
#define SD_CLR_CMD_SENT                (1<<6)
#define SD_CLR_DATA_CRC_OK             (1<<5)
#define SD_CLR_RSP_CRC_OK              (1<<4)
#define SD_CLR_DATA_TIMEOUT            (1<<3)
#define SD_CLR_RSP_TIMEOUT             (1<<2)
#define SD_CLR_DATA_CRC_FAIL           (1<<1)
#define SD_CLR_RSP_CRC_FAIL            (1<<0)
        unsigned int    interrupt_mask;
#define SD_INT_CARD_CHANGE             (1<<10)
#define SD_INT_FIFO_ORUN               (1<<9)
#define SD_INT_FIFO_URUN               (1<<8)
#define SD_INT_DATA_END                (1<<7)
#define SD_INT_CMD_SENT                (1<<6)
#define SD_INT_DATA_CRC_OK             (1<<5)
#define SD_INT_RSP_CRC_OK              (1<<4)
#define SD_INT_DATA_TIMEOUT            (1<<3)
#define SD_INT_RSP_TIMEOUT             (1<<2)
#define SD_INT_DATA_CRC_FAIL           (1<<1)
#define SD_INT_RSP_CRC_FAIL            (1<<0)
        unsigned int    power_control;
#define SD_POWER_ON                 (1<<4)
#define SD_POWER_MASK               0x0f
        unsigned int    clock_control;
#define SD_CLK_DIS                     (1<<8)
#define SD_CLK_SD                      (1<<7)
#define SD_CLK_DIV_MASK                0x7f
        unsigned int    bus_width;
#define SD_WIDE_BUS_SUPPORT            (1<<3)
#define SD_WIDE_BUS                    (1<<2)  // bus width=4
#define SD_SINGLE_BUS                  (1<<0)  // bus width=1
        unsigned int    data_window;
        unsigned int    feature;
#define SD_CPRM_FUNCTION               (1<<8)
        unsigned int    revision;
} moxaart_sd_reg;

#define SD_RETRY_COUNT         100

#define SD_FIFO_LENW   4       // 4 words, total 4 * 4 = 16 bytes
#define SD_FIFO_LENB   16      // 16 bytes

#define ERR_NONE             0 /* no error occurred */

struct moxaart_sd_platform_data{
	u32	irq;
	u32 phyaddr;
	u32 phyaddr_len;
	u32 virtaddr;
};

#define MOXAART_SD_DATA_WINDOW_OFFSET		0x40
struct moxaart_sd_host {
        struct mmc_host         *mmc;
        spinlock_t              lock;
        moxaart_sd_reg              *reg;
	  u32	data_window_phys;
        apb_dma_priv            *dma;
#ifdef MOXAART_SD_SUPPORT_GET_CLOCK
        unsigned int            sysclk;
#endif
        struct mmc_request      *mrq;
        struct mmc_data         *data;

        struct scatterlist      *cur_sg;        /* Current SG entry */
        unsigned int            num_sg;         /* Number of entries left */
        void                    *mapped_sg;     /* vaddr of mapped sg */
        unsigned int            remain;         /* Data left in curren entry */
        int                     size;           /* Total size of transfer */

        struct tasklet_struct   card_change_tasklet;
        struct tasklet_struct   fifo_run_tasklet;
};


#endif  // _MOXASD_H

