/* apb_dma.h */
#ifndef _APB_DMA_H_
#define _APB_DMA_H_

#define APBDMA_DMA_CHAN_BASE           0x80
#define APBDMA_DMA_SRC_0x0             0x0
#define APBDMA_DMA_DEST_0x4            0x4
#define APBDMA_DMA_CYCLE_0x8           0x8
#define APBDMA_DMA_CMD_0xC             0xc

#define APBDMA_WIDTH_8BIT               0x0
#define APBDMA_WIDTH_16BIT              0x1
#define APBDMA_WIDTH_32BIT              0x2

#define APBDMA_TYPE_APB                 0
#define APBDMA_TYPE_AHB                 1

#define APBDMA_NO_TRIGGER_IRQ           0x0
#define APBDMA_TRIGGER_IRQ              0x1

#define APBDMA_CTL_FIX                  0
#define APBDMA_CTL_INC1                 1
#define APBDMA_CTL_INC2                 2
#define APBDMA_CTL_INC4                 4
#define APBDMA_CTL_INC8                 8
#define APBDMA_CTL_INC16                16
#define APBDMA_CTL_DEC1                 (-1)
#define APBDMA_CTL_DEC2                 (-2)
#define APBDMA_CTL_DEC4                 (-4)

typedef struct
{
    /* add dma parameter */
    unsigned int    src;        // source address for dma
    unsigned int    dest;       // dest address for dma
    unsigned int    width;      // data width (0/1/2=>8/16/32)
    unsigned int    req_num;    // hardware request/grant number
    int             sctl;       // source incremental
    int             dctl;       // dest incremental
    unsigned int    stype;      // source type (0/1 => APB/AHB)
    unsigned int    dtype;      // dest type (0/1 => APB/AHB)
    unsigned int    burst;      // (0/1)==>(disable/enable)
    unsigned int    size;       // dma count
    unsigned int    irq;        // (0/1)==>(disable/enable)
} apb_dma_parm_t;

typedef struct
{
    /* init parameter */
    unsigned int    base;
    unsigned int    channel_base;
    unsigned int    channel;
} apb_dma_data_t;



apb_dma_data_t *apb_dma_alloc(void);
void apb_dma_free(apb_dma_data_t *priv);
int apb_dma_init(apb_dma_data_t *priv);
inline void apb_dma_add(apb_dma_data_t *priv,apb_dma_parm_t *parm);
inline void apb_dma_start(apb_dma_data_t *priv);
void apb_dma_reset(apb_dma_data_t *priv);
inline void apb_dma_clear_int(apb_dma_data_t *priv);
#endif
