#ifndef __EXTRA_REGISTER_H__
#define __EXTRA_REGISTER_H__

/*dma*/
#define DMA_T0   (uint32_t)0xff63e000
#define DMA_T1   (uint32_t)0xff63e004
#define DMA_T2   (uint32_t)0xff63e008
#define DMA_T3   (uint32_t)0xff63e00c
#define DMA_T4   (uint32_t)0xff63e010
#define DMA_T5   (uint32_t)0xff63e014
#define DMA_STS0 (uint32_t)0xff63e020
#define DMA_STS1 (uint32_t)0xff63e024
#define DMA_STS2 (uint32_t)0xff63e028
#define DMA_STS3 (uint32_t)0xff63e02c
#define DMA_STS4 (uint32_t)0xff63e030
#define DMA_STS5 (uint32_t)0xff63e034
#define DMA_CFG  (uint32_t)0xff63e040
#define DMA_SEC  (uint32_t)0xff63e044
#define DMA_END  (uint32_t)0xff63ffff

#define P_DMA_T0   (volatile uint32_t *)0xff63e000
#define P_DMA_T1   (volatile uint32_t *)0xff63e004
#define P_DMA_T2   (volatile uint32_t *)0xff63e008
#define P_DMA_T3   (volatile uint32_t *)0xff63e00c
#define P_DMA_T4   (volatile uint32_t *)0xff63e010
#define P_DMA_T5   (volatile uint32_t *)0xff63e014
#define P_DMA_STS0 (volatile uint32_t *)0xff63e020
#define P_DMA_STS1 (volatile uint32_t *)0xff63e024
#define P_DMA_STS2 (volatile uint32_t *)0xff63e028
#define P_DMA_STS3 (volatile uint32_t *)0xff63e02c
#define P_DMA_STS4 (volatile uint32_t *)0xff63e030
#define P_DMA_STS5 (volatile uint32_t *)0xff63e034
#define P_DMA_CFG  (volatile uint32_t *)0xff63e040
#define P_DMA_SEC  (volatile uint32_t *)0xff63e044

#endif
