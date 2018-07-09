/*
 * linux/drivers/crypto/moxaart_crypto.c
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

#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/crypto.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/workqueue.h>
#include <linux/irq.h>


#include <crypto/des.h>
#include <crypto/aes.h>
#include <crypto/scatterwalk.h>

#include <mach/moxaart_int.h>
#include "moxaart_crypto.h"


void inline moxaart_crypto_set_control(u32 base, u32 control)
{
	dbg_printk("control=0x%x\n", control);
	writel(control,  (void *)(base + CRYPTO_CONTROL));

}

void inline moxaart_crypto_enable_intr(u32 base, u32 intr)
{
	dbg_printk("intr=0x%x\n", intr);
	writel(intr,  (void *)(base + CRYPTO_INTR_ENABLE));

}

inline u32 moxaart_crypto_intr_status(u32 base)
{
	dbg_printk("\n");
	return readl((void *)(base + CRYPTO_INTR_SRC));
}

inline void moxaart_crypto_clear_intr(u32 base, u32 intr)
{
	dbg_printk("\n");
	writel(intr,  (void *)(base + CRYPTO_INTR_CLR));
}

inline void moxaart_crypto_set_dma(u32 base, u32 src, u32 dst, u32 size)
{
	dbg_printk("src=0x%x dst=0x%x size=0x%x\n", src, dst, size);
	writel(src,  (void *)(base + CRYPTO_DMA_SRC));
	writel(dst,  (void *)(base + CRYPTO_DMA_DES));
	writel(size,  (void *)(base + CRYPTO_DMA_TRAS_SIZE));

}

void inline moxaart_crypto_start_dma(u32 base)
{
	dbg_printk("\n");
	writel(DmaEn,  (void *)(base + CRYPTO_DMA_CTRL));
}

inline void moxaart_crypto_set_fifo_th(u32 base, u8 out, u8 in)
{
	dbg_printk("\n");
	writel(out<<8 | in,  (void *)(base + CRYPTO_FIFO_THOLD));
}

static inline u32 cipher_enc_control(struct crypto_tfm *tfm)
{
	return container_of(tfm->__crt_alg, struct moxaart_alg,crypto_alg)->enc_control;
}

static inline u32 cipher_dec_control(struct crypto_tfm *tfm)
{
	return container_of(tfm->__crt_alg, struct moxaart_alg,crypto_alg)->dec_control;
}

static inline u32 cipher_virtual_addr(struct crypto_tfm *tfm)
{
	return container_of(tfm->__crt_alg, struct moxaart_alg,crypto_alg)->plat_data->virtaddr;
}

static inline struct list_head *cipher_request_list(struct crypto_tfm *tfm)
{
	return &container_of(tfm->__crt_alg, struct moxaart_alg,crypto_alg)->plat_data->req_list;
}

static inline spinlock_t *cipher_lock(struct crypto_tfm *tfm)
{
	return &container_of(tfm->__crt_alg, struct moxaart_alg,crypto_alg)->plat_data->cipher_lock;
}

static void* moxaart_scatterlist_combine(struct scatterlist *sg, u32 total_length)
{
	u8* buffer;
	void *ptr;
	u32 index=0;

	buffer = kmalloc(total_length ,GFP_KERNEL);
	while(total_length){
		ptr = page_address(sg_page(sg)) + sg->offset;
		dbg_printk("total_length= %d ptr=%p\n",total_length,ptr);

		memcpy(&buffer[index], ptr, sg->length);
		index += sg->length;
		total_length -= sg->length;
		sg = scatterwalk_sg_next(sg);
	}

	return buffer;
}

static void moxaart_scatterlist_divide(struct scatterlist *sg, u8* buffer, u32 total_length)
{
	u8 *ptr;
	u32 index=0;

	while(total_length){
		ptr = page_address(sg_page(sg)) + sg->offset;
		memcpy(ptr, &buffer[index], sg->length);
		index += sg->length;
		total_length -= sg->length;
		sg = scatterwalk_sg_next(sg);
	}

}

static int moxaart_crypto_request_list_add_tail(struct ablkcipher_request *req, int decrypt)
{
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	struct list_head *req_list = cipher_request_list(&ablkcipher->base);
	struct moxaart_crypto_request *req_item = kmalloc(sizeof(struct moxaart_crypto_request), GFP_KERNEL);

	req_item->req = req;
	req_item->inprogress = 0;
	req_item->decrypt = decrypt;

	list_add_tail(&req_item->req_list_item, req_list);

	if(list_entry(req_list->next, struct moxaart_crypto_request, req_list_item)->inprogress){
		dbg_printk("\n");
		return -EBUSY;
	}else
		return 0;
}

static int	moxaart_crypto_setkey(struct crypto_ablkcipher *ablkcipher, const u8 *key, unsigned int keylen)
{
	struct crypto_aes_ctx *aes_ctx = crypto_ablkcipher_ctx(ablkcipher);
	struct moxaart_des_ctx *des_ctx = crypto_ablkcipher_ctx(ablkcipher);
	struct crypto_tfm *tfm = crypto_ablkcipher_tfm(ablkcipher);
	int ret = 0;
	u32	tmp[DES_EXPKEY_WORDS];

	dbg_printk("keylen = %d\n" ,  keylen);

	//if(tfm->crt_flags & CRYPTO_TFM_REQ_WEAK_KEY){
		dbg_printk("tfm->crt_flags & CRYPTO_TFM_REQ_WEAK_KEY=%d\n", tfm->crt_flags & CRYPTO_TFM_REQ_WEAK_KEY);
		ret = des_ekey(tmp, key);
		dbg_printk("tfm->crt_flags=0x%x weak key=%d\n", tfm->crt_flags, ret);

		if (unlikely(ret == 0)) {
			tfm->crt_flags |= CRYPTO_TFM_RES_WEAK_KEY;
			return -EINVAL;
		}
	//}

	if ( ((cipher_enc_control(&ablkcipher->base)&Method) == MethodDES) || ((cipher_enc_control(&ablkcipher->base)&Method) == Method3DES)){
		dbg_printk("\n");
		memcpy(des_ctx->key, key, keylen);
		des_ctx->keylen = keylen;
	}else if (cipher_enc_control(&ablkcipher->base) & MethodAES){
		dbg_printk("\n");
		memcpy(aes_ctx->key_enc, key, keylen);
		memcpy(aes_ctx->key_dec, key, keylen);
		aes_ctx->key_length = keylen;
	}

	return 0;
}

static int moxaart_crypto_perform(struct ablkcipher_request *req, int decrypt)
{
	struct crypto_ablkcipher *ablkcipher = crypto_ablkcipher_reqtfm(req);
	struct crypto_aes_ctx *aes_ctx = crypto_ablkcipher_ctx(ablkcipher);
	struct moxaart_des_ctx *des_ctx = crypto_ablkcipher_ctx(ablkcipher);
	struct moxaart_ablkcipher_req_ctx *req_ctx = ablkcipher_request_ctx(req);
	int	ivsize=crypto_ablkcipher_ivsize(ablkcipher);
	u32 cipher_virt_base = cipher_virtual_addr(&ablkcipher->base);
	spinlock_t *lock = cipher_lock(&ablkcipher->base);
	unsigned long flags;
	u32 control;
	int i;

	dbg_printk("cipher_virt_base=0x%x\n",cipher_virt_base);


	spin_lock_irqsave(lock, flags);
	control = decrypt ? cipher_dec_control(&ablkcipher->base) : cipher_enc_control(&ablkcipher->base);

	/* step 1: set cipher control */
	if (control & MethodAES){
		if (aes_ctx->key_length == AES_KEYSIZE_128)
			control |= MethodAES128;
		else if (aes_ctx->key_length == AES_KEYSIZE_192)
			control |= MethodAES192;
		else if (aes_ctx->key_length == AES_KEYSIZE_256)
			control |= MethodAES256;
	}
	moxaart_crypto_set_control(cipher_virt_base, control);

	/* step 2: set IVs if there are, 4 bytes each */
	for(i=0; i<4; i++){
		if(i < (ivsize/4))
			writel(htonl(*((u32 *)req->info + i)), (void *)(cipher_virt_base + CRYPTO_IV0 + i*4));
		else
			writel(0, (void *)(cipher_virt_base + CRYPTO_IV0 + i*4));
		dbg_printk("IV%d=0x%x\n", i, *((u32 *)req->info + i));
		dbg_printk("IV%d = 0x%x\n", i, readl((void *)(cipher_virt_base + CRYPTO_IV0 + i*4)));
	}

	/* step 3: set keys, 4 bytes each*/
	for(i=0; i<8; i++){
		if (control & MethodAES){
			if(i < (aes_ctx->key_length/4)){
				writel(htonl(aes_ctx->key_enc[i]), (void *)(cipher_virt_base + CRYPTO_KEY0 + i*4));
			}else
				writel(0, (void *)(cipher_virt_base + CRYPTO_KEY0 + i*4));

		}else{
			if(i < (des_ctx->keylen/4)){
				writel(htonl(des_ctx->key[i]), (void *)(cipher_virt_base + CRYPTO_KEY0 + i*4));
			}else
				writel(0, (void *)(cipher_virt_base + CRYPTO_KEY0 + i*4));
		}
		dbg_printk("KEY%d = 0x%x\n", i, readl((void *)(cipher_virt_base + CRYPTO_KEY0 + i*4)));
	}

	/* step 4:	set DMA controls */
	if(req->src->length != req->nbytes){
		dbg_printk("Multi SG\n");
		req_ctx->src_buf = moxaart_scatterlist_combine(req->src, req->nbytes);
		req_ctx->dst_buf = kmalloc(req->nbytes, GFP_KERNEL);
	}else{
		dbg_printk("One SG\n");
		req_ctx->src_buf = page_address(sg_page(req->src)) + req->src->offset;
		req_ctx->dst_buf = req_ctx->src_buf;
	}

#ifdef MOXAART_CRYPTO_DEBUG
	dbg_printk("data: ");
	for(i = 0; i<req->nbytes; i++ )
		printk("%x ",req_ctx->src_buf[i]);
	printk("\n");
#endif

	req_ctx->src_dma_addr = (u32)dma_map_single(NULL, req_ctx->src_buf, req->nbytes, DMA_TO_DEVICE);

	req_ctx->dst_dma_addr = (u32)dma_map_single(NULL, req_ctx->dst_buf, req->nbytes, DMA_FROM_DEVICE);

	moxaart_crypto_set_dma(cipher_virt_base, req_ctx->src_dma_addr, req_ctx->dst_dma_addr, req->nbytes);

	moxaart_crypto_clear_intr(cipher_virt_base, DoneIntr | ErrIntr | StopIntr);
	/* step 5:	enable transfer interrupts*/
	moxaart_crypto_enable_intr(cipher_virt_base, DoneIntr | ErrIntr | StopIntr);

	/* step 6:	enable DMA engine  */
	moxaart_crypto_start_dma(cipher_virt_base);

	spin_unlock_irqrestore(lock, flags);

	return -EINPROGRESS;
}

static int	moxaart_encrypt(struct ablkcipher_request *req)
{
	dbg_printk("\n");
	if(moxaart_crypto_request_list_add_tail(req, Encrypt) < 0 )
		return -EBUSY;

	dbg_printk("\n");
	return moxaart_crypto_perform(req, Encrypt);
}

static int	moxaart_decrypt(struct ablkcipher_request *req)
{
	dbg_printk("\n");
	if(moxaart_crypto_request_list_add_tail(req, Decrypt) < 0 )
		return -EBUSY;

	dbg_printk("\n");
	return moxaart_crypto_perform(req, Decrypt);
}

static int moxaart_ablkcipher_cra_init(struct crypto_tfm *tfm)
{
	dbg_printk("\n");
	return 0;
}

static void moxaart_ablkcipher_cra_exit(struct crypto_tfm *tfm)
{
	dbg_printk("\n");

}

static irqreturn_t moxaart_crypto_irq_handler(int irq, void *pdev)
{
	struct moxaart_crypto_platform_data *plat_data = dev_get_platdata(&((struct platform_device *)pdev)->dev);
	struct list_head *req_list = &plat_data->req_list;
	struct ablkcipher_request *req;
	struct moxaart_ablkcipher_req_ctx *req_ctx;
	struct moxaart_crypto_request *req_item;
	int ret = 0;
#ifdef MOXAART_CRYPTO_DEBUG
	int i;
#endif

	dbg_printk("\n");
	if(moxaart_crypto_intr_status(plat_data->virtaddr) & DoneIntr){
		dbg_printk("DoneIntr\n");
		req_item = list_entry(req_list->next, struct moxaart_crypto_request, req_list_item);
		req = (struct ablkcipher_request *)req_item->req;
		req_ctx = ablkcipher_request_ctx(req);

		/*Key Point: The cache of the destination buffer should be synchronized from the RAM. */
		dma_unmap_single(NULL, req_ctx->dst_dma_addr, req->nbytes, DMA_FROM_DEVICE);

#ifdef MOXAART_CRYPTO_DEBUG
		dbg_printk("data: ");
		for(i = 0; i<req->nbytes; i++ )
			printk("%x ",req_ctx->dst_buf[i]);
		printk("\n");
#endif

		if(req->dst->length != req->nbytes)
			moxaart_scatterlist_divide(req->dst, req_ctx->dst_buf, req->nbytes);

		list_del(req_list->next);
		kfree(req_item);
		/*
		 * Key Point: If the encrypt/decrypt function return "-EINPROGRESS" or "-EBUSY",
		 *			the upper layer will wait until this driver call the callback function "compelete".
		 */
		req->base.complete(&req->base, ret);
		moxaart_crypto_clear_intr(plat_data->virtaddr, DoneIntr);
	}

	if (moxaart_crypto_intr_status(plat_data->virtaddr) & ErrIntr){
		printk("%s: ErrIntr\n", ((struct platform_device *)pdev)->name);

		moxaart_crypto_clear_intr(plat_data->virtaddr, ErrIntr);
	}

	if (moxaart_crypto_intr_status(plat_data->virtaddr) & StopIntr){
		printk("%s StopIntr\n", ((struct platform_device *)pdev)->name);

		moxaart_crypto_clear_intr(plat_data->virtaddr, StopIntr);
	}

	return IRQ_HANDLED;
}

void moxaart_crypto_schedule_work(struct work_struct *work)
{
	struct moxaart_crypto_request *req_item;
	struct moxaart_crypto_platform_data *plat_data = container_of(work, struct moxaart_crypto_platform_data, crypto_work);

	if(list_empty(&plat_data->req_list))
		return;
	else
		req_item = list_entry(plat_data->req_list.next, struct moxaart_crypto_request, req_list_item);

	moxaart_crypto_perform(req_item->req, req_item->decrypt);
}


static struct	moxaart_alg moxaart_crypto_algs[]={
	{
		.crypto_alg =	{
			.cra_driver_name = "ecb(des)-moxaart",
			.cra_name               = "ecb(des)",
			.cra_flags              = CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
			.cra_blocksize          = DES_BLOCK_SIZE,
			.cra_ctxsize            = sizeof(struct moxaart_des_ctx),
			.cra_module             = THIS_MODULE,
			.cra_type		= &crypto_ablkcipher_type,
			.cra_alignmask          = 3,
			.cra_u                  = {
				.ablkcipher = {
					.min_keysize    = DES_KEY_SIZE,
					.max_keysize    = DES_KEY_SIZE,
					.ivsize		= DES_BLOCK_SIZE,
					.geniv		= "eseqiv",
					.setkey         = moxaart_crypto_setkey,
					.encrypt        = moxaart_encrypt,
					.decrypt        = moxaart_decrypt
				}
			},
			.cra_init	= moxaart_ablkcipher_cra_init,
			.cra_exit	= moxaart_ablkcipher_cra_exit,
		},
		.enc_control = Encrypt | MethodDES | EModeECB | ParityCheck,
		.dec_control = Decrypt | MethodDES | EModeECB | ParityCheck,
		.registered = 0
	},
	{
		.crypto_alg =	{
			.cra_driver_name = "cbc(des)-moxaart",
			.cra_name               = "cbc(des)",
			.cra_flags              = CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
			.cra_blocksize          = DES_BLOCK_SIZE,
			.cra_ctxsize            = sizeof(struct moxaart_des_ctx),
			.cra_module             = THIS_MODULE,
			.cra_type		= &crypto_ablkcipher_type,
			.cra_alignmask          = 3,
			.cra_u                  = {
				.ablkcipher = {
					.min_keysize    = DES_KEY_SIZE,
					.max_keysize    = DES_KEY_SIZE,
					.ivsize		= DES_BLOCK_SIZE,
					.geniv		= "eseqiv",
					.setkey         = moxaart_crypto_setkey,
					.encrypt        = moxaart_encrypt,
					.decrypt        = moxaart_decrypt
				}
			},
			.cra_init	= moxaart_ablkcipher_cra_init,
			.cra_exit	= moxaart_ablkcipher_cra_exit,
		},
		.enc_control = Encrypt | MethodDES | EModeCBC | ParityCheck | FirstBlock,
		.dec_control = Decrypt | MethodDES | EModeCBC | ParityCheck | FirstBlock,
		.registered = 0
	},
	{
		.crypto_alg =	{
			.cra_driver_name = "ecb(des3_ede)-moxaart",
			.cra_name               = "ecb(des3_ede)",
			.cra_flags              = CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
			.cra_blocksize          = DES_BLOCK_SIZE,
			.cra_ctxsize            = sizeof(struct moxaart_des_ctx),
			.cra_module             = THIS_MODULE,
			.cra_type		= &crypto_ablkcipher_type,
			.cra_alignmask          = 3,
			.cra_u                  = {
				.ablkcipher = {
					.min_keysize    = DES3_EDE_KEY_SIZE,
					.max_keysize    = DES3_EDE_KEY_SIZE,
					.ivsize		= DES3_EDE_BLOCK_SIZE,
					.geniv		= "eseqiv",
					.setkey         = moxaart_crypto_setkey,
					.encrypt        = moxaart_encrypt,
					.decrypt        = moxaart_decrypt
				}
			},
			.cra_init	= moxaart_ablkcipher_cra_init,
			.cra_exit	= moxaart_ablkcipher_cra_exit,
		},
		.enc_control = Encrypt | Method3DES | EModeECB | ParityCheck,
		.dec_control = Decrypt | Method3DES | EModeECB | ParityCheck,
		.registered = 0
	},
	{
		.crypto_alg =	{
			.cra_driver_name = "cbc(des3_ede)-moxaart",
			.cra_name               = "cbc(des3_ede)",
			.cra_flags              = CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
			.cra_blocksize          = DES_BLOCK_SIZE,
			.cra_ctxsize            = sizeof(struct moxaart_des_ctx),
			.cra_module             = THIS_MODULE,
			.cra_type		= &crypto_ablkcipher_type,
			.cra_alignmask          = 3,
			.cra_u                  = {
				.ablkcipher = {
					.min_keysize    = DES3_EDE_KEY_SIZE,
					.max_keysize    = DES3_EDE_KEY_SIZE,
					.ivsize		= DES3_EDE_BLOCK_SIZE,
					.geniv		= "eseqiv",
					.setkey         = moxaart_crypto_setkey,
					.encrypt        = moxaart_encrypt,
					.decrypt        = moxaart_decrypt
				}
			},
			.cra_init	= moxaart_ablkcipher_cra_init,
			.cra_exit	= moxaart_ablkcipher_cra_exit,
		},
		.enc_control = Encrypt | Method3DES | EModeCBC | ParityCheck | FirstBlock,
		.dec_control = Decrypt | Method3DES | EModeCBC | ParityCheck | FirstBlock,
		.registered = 0
	},
	{
		.crypto_alg =	{
			.cra_driver_name = "ecb(aes)-moxaart",
			.cra_name               = "ecb(aes)",
			.cra_flags              = CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
			.cra_blocksize          = DES_BLOCK_SIZE,
			.cra_ctxsize            = sizeof(struct crypto_aes_ctx),
			.cra_module             = THIS_MODULE,
			.cra_priority		=101,  //to let this hardware crypto algorithm has a priority higher than the software one.
			.cra_type		= &crypto_ablkcipher_type,
			.cra_alignmask          = 3,
			.cra_u                  = {
				.ablkcipher = {
					.min_keysize    = AES_MIN_KEY_SIZE,
					.max_keysize    = AES_MAX_KEY_SIZE,
					.ivsize		= AES_BLOCK_SIZE,
					.setkey         = moxaart_crypto_setkey,
					.encrypt        = moxaart_encrypt,
					.decrypt        = moxaart_decrypt
				}
			},
			.cra_init	= moxaart_ablkcipher_cra_init,
			.cra_exit	= moxaart_ablkcipher_cra_exit,
		},
		.enc_control = Encrypt | MethodAES | EModeECB,
		.dec_control = Decrypt | MethodAES | EModeECB,
		.registered = 0
	},
	{
		.crypto_alg =	{
			.cra_driver_name = "cbc(aes)-moxaart",
			.cra_name               = "cbc(aes)",
			.cra_flags              = CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
			.cra_blocksize          = DES_BLOCK_SIZE,
			.cra_ctxsize            = sizeof(struct crypto_aes_ctx),
			.cra_module             = THIS_MODULE,
			.cra_priority		=101,  //to let this hardware crypto algorithm has a priority higher than the software one.
			.cra_type		= &crypto_ablkcipher_type,
			.cra_alignmask          = 3,
			.cra_u                  = {
				.ablkcipher = {
					.min_keysize    = AES_MIN_KEY_SIZE,
					.max_keysize    = AES_MAX_KEY_SIZE,
					.ivsize		= AES_BLOCK_SIZE,
					.setkey         = moxaart_crypto_setkey,
					.encrypt        = moxaart_encrypt,
					.decrypt        = moxaart_decrypt
				}
			},
			.cra_init	= moxaart_ablkcipher_cra_init,
			.cra_exit	= moxaart_ablkcipher_cra_exit,
		},
		.enc_control = Encrypt | MethodAES | EModeCBC | FirstBlock,
		.dec_control = Decrypt | MethodAES | EModeCBC | FirstBlock,
		.registered = 0
	},
};

static int	moxaart_crypto_probe(struct platform_device *pdev)
{
	struct resource         *res;
	struct moxaart_crypto_platform_data *plat_data = kzalloc( sizeof(struct moxaart_crypto_platform_data), GFP_KERNEL);
	int			i, ret = 0;

	if(!plat_data){
		printk("%s: Failed to allocate memory!\n", pdev->name);
		return -ENOMEM;
	}
	pdev->dev.platform_data = plat_data;

	/*Try to get the platform resources.*/
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if ( !res) {
		printk("%s: Failed to get the memory resource!\n", pdev->name);
		ret = -ENODEV;
		goto get_resource_failed;
	}
	plat_data->phyaddr = res->start;
	plat_data->phyaddr_len = res->end - res->start + 1;

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if ( !res) {
		printk("%s: Failed to get the IRQ number!\n", pdev->name);
		ret = -ENODEV;
		goto get_resource_failed;
	}
	plat_data->irq = res->start;

	/*Try to request the memory region for crypto control.*/
	if ( !request_mem_region(plat_data->phyaddr, plat_data->phyaddr_len, "MoxaArtCrypto") ) {
		printk("%s: Failed to request hardware address region!\n", pdev->name);
		ret = -ENOMEM;
		goto get_resource_failed;
	}

	/*Try to map the physical address of the crypto controller to a virtual address. */
	plat_data->virtaddr = (u32)ioremap_nocache(plat_data->phyaddr, plat_data->phyaddr_len);
	dbg_printk("plat_data->virtaddr=0x%x\n", plat_data->virtaddr);
	if ( !plat_data->virtaddr) {
		printk("%s: Failed to remap crypto hardware physical address!\n", pdev->name);
		ret = -ENOMEM;
		goto ioremap_failed;
	}

	/*Try to register the IRQ and its handler.*/
	//moxaart_int_set_irq(plat_data->irq, LEVEL, H_ACTIVE);
	if (request_irq(plat_data->irq, moxaart_crypto_irq_handler, IRQ_TYPE_LEVEL_HIGH, pdev->name, pdev) ) {
		printk("%s: Failed to request interrupt service!\n", pdev->name);
		ret = -ENOMEM;
		goto request_irq_failed;

	}

	// register my crypto accelecator
	for(i=0; i<(sizeof(moxaart_crypto_algs)/sizeof(struct moxaart_alg)); i++){
		if ( (ret=crypto_register_alg(&moxaart_crypto_algs[i].crypto_alg)) ){
			goto register_alg_failed;
		}
		printk("register_alg: %s\n", moxaart_crypto_algs[i].crypto_alg.cra_name);
		moxaart_crypto_algs[i].plat_data = plat_data;
	}

	/*Initialize request list.*/
	INIT_LIST_HEAD(&plat_data->req_list);

	INIT_WORK(&plat_data->crypto_work, moxaart_crypto_schedule_work);

	/*Key Point: The default INFIFOThold is 0x0, and that will cause the processing speed very slow.*/
	moxaart_crypto_set_fifo_th(plat_data->virtaddr, 0x1, 0x1);

	return ret;

register_alg_failed:
	for (i = i-1; i >= 0; i--)
		crypto_unregister_alg(&moxaart_crypto_algs[i].crypto_alg);
	free_irq(plat_data->irq, pdev);
request_irq_failed:
	iounmap((void *)plat_data->virtaddr);
ioremap_failed:
	release_mem_region(plat_data->phyaddr, plat_data->phyaddr_len);
get_resource_failed:
	kfree(plat_data);
	return ret;
}



static int	moxaart_crypto_remove(struct platform_device *pdev)
{
	struct moxaart_crypto_platform_data *plat_data = pdev->dev.platform_data;
	int	ret = 0;
	int i;

	for(i=0; i<(sizeof(moxaart_crypto_algs)/sizeof(struct moxaart_alg)); i++){
		crypto_unregister_alg(&moxaart_crypto_algs[i].crypto_alg);
		printk("unregister_alg: %s\n", (char *)&moxaart_crypto_algs[i].crypto_alg.cra_name);
	}

	if(plat_data->irq)
		free_irq(plat_data->irq, pdev);
	if(plat_data->virtaddr)
		iounmap((void *)plat_data->virtaddr);
	if(plat_data->phyaddr)
		release_mem_region(plat_data->phyaddr, plat_data->phyaddr_len);
	if(plat_data)
		kfree(plat_data);

	return ret;
}

static struct platform_driver moxaart_crypto_driver = {
	.probe = moxaart_crypto_probe,
	.remove = moxaart_crypto_remove,
	.driver = {
		.name = "MoxaArtCrypto",
	},
};

static int __init	moxaart_crypto_module_init(void)
{
	return platform_driver_register(&moxaart_crypto_driver);
}

static void __exit	moxaart_crypto_module_exit(void)
{

	platform_driver_unregister(&moxaart_crypto_driver);
}

module_init(moxaart_crypto_module_init);
module_exit(moxaart_crypto_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vincent Wu <vincent.wu@moxa.com>");
MODULE_DESCRIPTION("MOXA ART hardware crypto driver");
