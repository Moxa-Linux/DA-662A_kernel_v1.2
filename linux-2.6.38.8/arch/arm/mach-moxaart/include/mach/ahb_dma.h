/* ahb_dma.h */
#ifndef _AHB_DMA_H_
#define _AHB_DMA_H_

/* register definition */
#define AHBDMA_INT_TC_0x04     0x04
#define AHBDMA_ISR_0x8          0x8
#define AHBDMA_INT_ERR_0x0C     0x0C
#define AHBDMA_ENABLE_0x24      0x24

#define AHBDMA_CHANNEL_CSR_0x0          0x0
#define AHBDMA_CHANNEL_CFG_0x4          0x4
#define AHBDMA_CHANNEL_SRC_0x8          0x8
#define AHBDMA_CHANNEL_DST_0xC          0xc
#define AHBDMA_CHANNEL_LLP_0x10         0x10
#define AHBDMA_CHANNEL_TXSZ_0x14        0x14

#define AHBDMA_WIDTH_8BIT       0x0
#define AHBDMA_WIDTH_16BIT      0x1
#define AHBDMA_WIDTH_32BIT      0x2
#define AHBDMA_CTL_INC          0x0
#define AHBDMA_CTL_DEC          0x1
#define AHBDMA_CTL_FIX          0x2

#define AHBDMA_MASTER_0         0x0
#define AHBDMA_MASTER_1         0x1

#define AHBDMA_NO_TRIGGER_IRQ   0x0
#define AHBDMA_TRIGGER_IRQ      0x1    //john

//john add for interrupt status, error
#define INT_TRIGGER 0x01
#define INT_ERROR   0x02

typedef struct
{
	unsigned int source;
	unsigned int dest;
	unsigned int llp;
	unsigned int control;
} ahb_lld_t;

typedef struct
{
    /* add dma parameter */
    unsigned int    src;        //source address for dma
    unsigned int    dest;       //dest address for dma
    unsigned int    sw;         //source width (0/1/2=>8/16/32)
    unsigned int    dw;         //dest width (0/1/2=>8/16/32)
    unsigned int    sctl;       //source control (0/1/2/3=>inc/dec/fix/x)
    unsigned int    dctl;       // dest coontrol (0/1/2/3=>inc/dec/fix/x)
    unsigned int    size;       // dma count
    unsigned int    irq;        // (0/1)==>(disable/enable)
} ahb_dma_parm_t;

typedef struct
{
    /* init parameter */
    unsigned int    base;
    unsigned int    channel_base;
    unsigned int    llp_master; //0,1
    unsigned int    src_data_master;//0,1
    unsigned int    dest_data_master;//0,1
    unsigned int    llp_count;
    unsigned int    channel;
    unsigned int    hw_handshake;
    unsigned int    llp_last_idx;   //0(register),1(first llp),2(second llp),3...
    unsigned int    llp_free_idx;   //0(register),1(first llp),2(second llp),3...    
    ahb_lld_t       *ahb_dma_lld;
    ahb_lld_t       *ahb_last_lld;
    ahb_lld_t       *ahb_dma_lld_phys;
} ahb_dma_data_t;


ahb_dma_data_t *ahb_dma_alloc(void);
void ahb_dma_free(ahb_dma_data_t *priv);
int ahb_dma_init(ahb_dma_data_t *priv);
void ahb_dma_add(ahb_dma_data_t *priv,ahb_dma_parm_t *parm);
void ahb_dma_reset(ahb_dma_data_t *priv);

inline void ahb_dma_start(ahb_dma_data_t *priv);
inline void ahbdma_clear_int(ahb_dma_data_t *priv);
inline u32 ahbdma_get_dest_addr(ahb_dma_data_t *priv);

u32 ahbdma_get_status(ahb_dma_data_t *priv);

#endif
