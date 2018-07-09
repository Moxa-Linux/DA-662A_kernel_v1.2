/* 
 * linux/drivers/crypto/moxaart_crypto.h
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

//#define MOXAART_CRYPTO_DEBUG
#ifdef MOXAART_CRYPTO_DEBUG
#define dbg_printk(fmt, s...)	printk("%s,%d:" fmt, __FUNCTION__, __LINE__, ##s)
#else
#define dbg_printk(fmt, s...)
#endif

#define CRYPTO_CONTROL		0x00
enum crypto_control
{
	Decrypt			= BIT(0),
	Encrypt			= 0x0,		
	MethodDES		= 0x0<<1,
	Method3DES		= 0x1<<1,
	MethodAES128	= 0x4<<1,
	MethodAES192	= 0x5<<1,
	MethodAES256	= 0x6<<1,
	MethodAES		= 0x4<<1,
	Method			= 0x7<<1,	
	EModeECB		= 0x0<<4,
	EModeCBC		= 0x1<<4,
	EModeCTR		= 0x2<<4,
	EModeCFB		= 0x4<<4,
	EModeOFB		= 0x5<<4,
	FirstBlock		= BIT(7),
	ParityCheck		= BIT(8),
};

#define CRYPTO_FIFO_STATUS		0x08
#define CRYPTO_PERR_STATUS		0x0c
#define CRYPTO_KEY0				0x10
#define CRYPTO_IV0				0x30
#define CRYPTO_IV1				0x34
#define CRYPTO_IV2				0x38
#define CRYPTO_IV3				0x3c
#define CRYPTO_DMA_SRC			0x48
#define CRYPTO_DMA_DES			0x4c
#define CRYPTO_DMA_TRAS_SIZE	0x50
#define CRYPTO_DMA_CTRL		0x54
enum crypto_dma_ctrl
{
	DmaEn		= BIT(0)
};
#define CRYPTO_FIFO_THOLD		0x58
#define CRYPTO_INTR_ENABLE		0x5c
#define CRYPTO_INTR_SRC			0x60
#define CRYPTO_MASKED_INTR_STATUS	0x64
#define CRYPTO_INTR_CLR			0x68
enum crypto_intr
{
	DoneIntr	= BIT(0),
	ErrIntr		= BIT(1),
	StopIntr		= BIT(2)
};


struct moxaart_crypto_request{
	struct list_head req_list_item;
	void *req;
	int decrypt;	
	int inprogress;
};

struct moxaart_crypto_platform_data{
	u32	irq;
	u32 phyaddr;
	u32 phyaddr_len;
	u32 virtaddr;

	struct list_head req_list;

	struct work_struct crypto_work;
	
	spinlock_t cipher_lock;
};

#define DES_MAX_KEYLENGTH_U32		(DES3_EDE_KEY_SIZE) / sizeof(u32)
struct moxaart_des_ctx {
	int	keylen;
	u32 key[DES_MAX_KEYLENGTH_U32];
};

struct moxaart_alg {
	struct crypto_alg crypto_alg;
	struct moxaart_crypto_platform_data *plat_data;
	u32 enc_control;
	u32 dec_control;

	int registered;
};

struct moxaart_ablkcipher_req_ctx {
	unsigned int pending_data_bytes;
	struct scatterlist *dst_sg_inprogress;
	struct scatterlist *src_sg_inprogress;
	u8 *src_buf;
	u8 *dst_buf;
	dma_addr_t src_dma_addr;
	dma_addr_t dst_dma_addr;
 };


