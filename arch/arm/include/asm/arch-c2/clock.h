/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __C2_H
#define __C2_H

/*
 * Clock controller register address
 * APB_BASE:  APB0_BASE_ADDR = 0xfe000800
 */
#define C2_SYS_OSCIN_CTRL                  0x0
#define C2_RTC_BY_OSCIN_CTRL0              0x4
#define C2_RTC_BY_OSCIN_CTRL1              0x8
#define C2_RTC_CTRL                        0xc
#define C2_SYS_CLK_CTRL0                   0x10
#define C2_AXI_CLK_CTRL0                   0x14
#define C2_SYS_CLK_EN0                     0x18
#define C2_SYS_CLK_EN1                     0x1c
#define C2_SYS_CLK_EN2                     0x20
#define C2_AXI_CLK_EN                      0x24
#define C2_DSPA_CLK_EN                     0x28
#define C2_DSPB_CLK_EN                     0x2c
#define C2_DSPA_CLK_CTRL0                  0x30
#define C2_DSPB_CLK_CTRL0                  0x34
#define C2_CLK12_24_CTRL                   0x38
#define C2_GEN_CLK_CTRL                    0x3c
#define C2_TIMESTAMP_CTRL0                 0x40
#define C2_TIMESTAMP_CTRL1                 0x44
#define C2_TIMESTAMP_CTRL2                 0x48
#define C2_TIMESTAMP_VAL0                  0x4c
#define C2_TIMESTAMP_VAL1                  0x50
#define	C2_TIMEBASE_CTRL0                  0x54
#define C2_TIMEBASE_CTRL1                  0x58
#define C2_SAR_ADC_CLK_CTRL                0xc0
#define C2_PWM_CLK_AB_CTRL                 0xc4
#define C2_PWM_CLK_CD_CTRL                 0xc8
#define C2_PWM_CLK_EF_CTRL                 0xcc
#define C2_SPICC_CLK_CTRL                  0xd0
#define C2_TS_CLK_CTRL                     0xd4
#define C2_SPIFC_CLK_CTRL                  0xd8
#define C2_USB_BUSCLK_CTRL                 0xdc
#define C2_SD_EMMC_CLK_CTRL                0xe0
#define C2_CECA_CLK_CTRL0                  0xe4
#define C2_CECA_CLK_CTRL1                  0xe8
#define C2_CECB_CLK_CTRL0                  0xec
#define C2_CECB_CLK_CTRL1                  0xf0
#define C2_PSRAM_CLK_CTRL                  0xf4
#define C2_DMC_CLK_CTRL                    0xf8
#define C2_FCLK_DIV1_SEL			0xfc
#define C2_TST_CTRL			0x100
#define C2_WAVE_CLK_CTRL0			0x104
#define C2_WAVE_CLK_CTRL1			0x108
#define C2_JPEG_CLK_CTRL			0x10C
#define C2_MIPI_ISP_CLK_CTRL		0x110
#define C2_NNA_CLK_CTRL		        0x114
#define C2_GDC_CLK_CTRL		        0x118
#define C2_GE2D_CLK_CTRL		        0x11C
#define C2_SD_EMMC_CLK_CTRL1               0x120
#define C2_ETH_CLK_CTRL                    0x124
#define C2_PWM_CLK_GH_CTRL                 0x128
#define C2_PWM_CLK_IJ_CTRL                 0x12C
#define C2_MBIST_ATSPEED_CTRL              0x130

/*
 * For PLl register offset
 * APB_BASE:  APB0_BASE_ADDR = 0xfe007c00
 */
#define C2_ANACTRL_FIXPLL_GATE_DIS	   0x40 + 0x7400
#define C2_ANACTRL_FIXPLL_CTRL0		   0x80 + 0x7400
#define C2_ANACTRL_FIXPLL_CTRL1            0x84 + 0x7400
#define C2_ANACTRL_FIXPLL_CTRL2            0x88 + 0x7400
#define C2_ANACTRL_FIXPLL_CTRL3            0x8c + 0x7400
#define C2_ANACTRL_FIXPLL_CTRL4            0x90 + 0x7400
#define C2_ANACTRL_FIXPLL_CTRL5            0x94 + 0x7400
#define C2_ANACTRL_FIXPLL_CTRL6            0x98 + 0x7400
#define C2_ANACTRL_FIXPLL_STS              0x9c + 0x7400
#define C2_ANACTRL_GPPLL_CTRL0		   0xc0 + 0x7400
#define C2_ANACTRL_GPPLL_CTRL1             0xc4 + 0x7400
#define C2_ANACTRL_GPPLL_CTRL2             0xc8 + 0x7400
#define C2_ANACTRL_GPPLL_CTRL3             0xcc + 0x7400
#define C2_ANACTRL_GPPLL_CTRL4             0xd0 + 0x7400
#define C2_ANACTRL_GPPLL_CTRL5             0xd4 + 0x7400
#define C2_ANACTRL_GPPLL_STS               0xd8 + 0x7400
#define C2_ANACTRL_GPPLL_CTRL6             0xdc + 0x7400
#define C2_ANACTRL_SYSPLL_CTRL0            0x100 + 0x7400
#define C2_ANACTRL_SYSPLL_CTRL1            0x104 + 0x7400
#define C2_ANACTRL_SYSPLL_CTRL2		   0x108 + 0x7400
#define C2_ANACTRL_SYSPLL_CTRL3            0x10c + 0x7400
#define C2_ANACTRL_SYSPLL_CTRL4            0x110 + 0x7400
#define C2_ANACTRL_SYSPLL_STS              0x114 + 0x7400
#define C2_ANACTRL_HIFIPLL_CTRL0           0x140 + 0x7400
#define C2_ANACTRL_HIFIPLL_CTRL1           0x144 + 0x7400
#define C2_ANACTRL_HIFIPLL_CTRL2           0x148 + 0x7400
#define C2_ANACTRL_HIFIPLL_CTRL3           0x14c + 0x7400
#define C2_ANACTRL_HIFIPLL_CTRL4           0x150 + 0x7400
#define C2_ANACTRL_HIFIPLL_STS             0x154 + 0x7400
//#define C1_ANACTRL_AUDDDS_CTRL0            0x180 + 0x7400
//#define C1_ANACTRL_AUDDDS_CTRL1            0x184 + 0x7400
//#define C1_ANACTRL_AUDDDS_CTRL2            0x188 + 0x7400
//#define C1_ANACTRL_AUDDDS_CTRL3            0x18c + 0x7400
//#define C1_ANACTRL_AUDDDS_CTRL4            0x190 + 0x7400
//#define C1_ANACTRL_AUDDDS_STS              0x194 + 0x7400
#define C2_ANACTRL_MISCTOP_CTRL0           0x1c0 + 0x7400
#define C2_ANACTRL_POR_CNTL                0x208 + 0x7400

/*
 * CPU clock register offset
 * APB_BASE:  APB1_BASE_ADDR = 0xfe007400
 */
/*
 *#define C1_CPUCTRL_CLK_CTRL0		0x0 + 0x7400
 *#define C1_CPUCTRL_CLK_CTRL1		0x4 + 0x7400
 *#define C1_CPUCTRL_CLK_CTRL5		0x14 + 0x7400
 *#define C1_CPUCTRL_CLK_CTRL6		0x18 + 0x7400
 */

#include <dt-bindings/clock/c2-clkc.h>
#define NR_CLKS				(CLKID_END_BASE)

#endif /* __C1_H */
