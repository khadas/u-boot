/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2018 - AmLogic, Inc.
 */
#ifndef _ARCH_MESON_G12A_CLOCK_H_
#define _ARCH_MESON_G12A_CLOCK_H_

/*
 * Clock controller register offsets
 *
 * Register offsets from the data sheet are listed in comment blocks below.
 * Those offsets must be multiplied by 4 before adding them to the base address
 * to get the right value
 */

#define HHI_GCLK_MPEG0			0x140 /* 0x50 offset in data sheet1 */
#define HHI_GCLK_MPEG1			0x144 /* 0x51 offset in data sheet1 */
#define HHI_GCLK_MPEG2			0x148 /* 0x52 offset in data sheet1 */
#define HHI_GCLK_OTHER			0x150 /* 0x54 offset in data sheet1 */

#define HHI_NAND_CLK_CNTL              0x25c /* 0x97 offset in data sheet1 */
#define HHI_SD_EMMC_CLK_CNTL   0x264 /* 0x99 offset in data sheet1 */

#define HHI_FIX_PLL_CNTL0		0x2A0 /* 0xa8 offset in data sheet1 */
#define HHI_FIX_PLL_CNTL1		0x2A4 /* 0xa9 offset in data sheet1 */

#define HHI_SYS_PLL_CNTL0		0x2f4 /* 0xbd offset in data sheet1 */
#define HHI_SYS_PLL_CNTL1		0x2f8 /* 0xbe offset in data sheet1 */
#define HHI_SYS_PLL_CNTL2		0x2fc /* 0xbf offset in data sheet1 */
#define HHI_SYS_PLL_CNTL3		0x300 /* 0xc0 offset in data sheet1 */
#define HHI_SYS_PLL_CNTL4		0x304 /* 0xc1 offset in data sheet1 */
#define HHI_SYS_PLL_CNTL5		0x308 /* 0xc2 offset in data sheet1 */
#define HHI_SYS_PLL_CNTL6		0x30c /* 0xc3 offset in data sheet1 */
#define HHI_SPICC_CLK_CNTL		0x3dc /* 0xf7 offset in data sheet1 */

#define CLKID_UNREALIZED		0xfff

/* AO registers */
#define AO_SAR_CLK			0x90
#define AO_CLK_GATE0			0x4c

ulong meson_measure_clk_rate(unsigned int clk);

#endif
