/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <asm/arch/regs.h>
#include <asm/arch/register.h>

#define DMA_T0   (uint32_t)(0xfe006000 + 0x00)
#define DMA_T1   (uint32_t)(0xfe006000 + 0x04)
#define DMA_T2   (uint32_t)(0xfe006000 + 0x08)
#define DMA_T3   (uint32_t)(0xfe006000 + 0x0c)
#define DMA_T4   (uint32_t)(0xfe006000 + 0x10)
#define DMA_T5   (uint32_t)(0xfe006000 + 0x14)
#define DMA_STS0 (uint32_t)(0xfe006000 + 0x20)
#define DMA_STS1 (uint32_t)(0xfe006000 + 0x24)
#define DMA_STS2 (uint32_t)(0xfe006000 + 0x28)
#define DMA_STS3 (uint32_t)(0xfe006000 + 0x2c)
#define DMA_STS4 (uint32_t)(0xfe006000 + 0x30)
#define DMA_STS5 (uint32_t)(0xfe006000 + 0x34)
#define DMA_CFG  (uint32_t)(0xfe006000 + 0x40)
#define DMA_SEC  (uint32_t)(0xfe006000 + 0x44)
#define DMA_END  (uint32_t)(0xfe006000 + 0x3ff)

#define P_DMA_T0   (volatile uint32_t *)(0xfe006000 + 0x00)
#define P_DMA_T1   (volatile uint32_t *)(0xfe006000 + 0x04)
#define P_DMA_T2   (volatile uint32_t *)(0xfe006000 + 0x08)
#define P_DMA_T3   (volatile uint32_t *)(0xfe006000 + 0x0c)
#define P_DMA_T4   (volatile uint32_t *)(0xfe006000 + 0x10)
#define P_DMA_T5   (volatile uint32_t *)(0xfe006000 + 0x14)
#define P_DMA_STS0 (volatile uint32_t *)(0xfe006000 + 0x20)
#define P_DMA_STS1 (volatile uint32_t *)(0xfe006000 + 0x24)
#define P_DMA_STS2 (volatile uint32_t *)(0xfe006000 + 0x28)
#define P_DMA_STS3 (volatile uint32_t *)(0xfe006000 + 0x2c)
#define P_DMA_STS4 (volatile uint32_t *)(0xfe006000 + 0x30)
#define P_DMA_STS5 (volatile uint32_t *)(0xfe006000 + 0x34)
#define P_DMA_CFG  (volatile uint32_t *)(0xfe006000 + 0x40)
#define P_DMA_SEC  (volatile uint32_t *)(0xfe006000 + 0x44)
