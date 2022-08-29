/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _ARCH_MESON_A1_CLOCK_H_
#define _ARCH_MESON_A1_CLOCK_H_

/*
 * Clock controller register address
 * APB_BASE:  APB0_BASE_ADDR = 0xfe000800
 */
#define A1_SYS_OSCIN_CTRL                  0x0
#define A1_RTC_BY_OSCIN_CTRL0              0x4
#define A1_RTC_BY_OSCIN_CTRL1              0x8
#define A1_RTC_CTRL                        0xc
#define A1_SYS_CLK_CTRL0                   0x10
#define A1_AXI_CLK_CTRL0                   0x14
#define A1_SYS_CLK_EN0                     0x1c
#define A1_SYS_CLK_EN1                     0x20
#define A1_AXI_CLK_EN                      0x24
#define A1_DSPA_CLK_EN                     0x28
#define A1_DSPB_CLK_EN                     0x2c
#define A1_DSPA_CLK_CTRL0                  0x30
#define A1_DSPB_CLK_CTRL0                  0x34
#define A1_CLK12_24_CTRL                   0x38
#define A1_GEN_CLK_CTRL                    0x3c
#define A1_TIMESTAMP_CTRL0                 0x40
#define A1_TIMESTAMP_CTRL1                 0x44
#define A1_TIMESTAMP_CTRL2                 0x48
#define A1_TIMESTAMP_VAL0                  0x4c
#define A1_TIMESTAMP_VAL1                  0x50
#define	A1_TIMEBASE_CTRL0                  0x54
#define A1_TIMEBASE_CTRL1                  0x58
#define A1_SAR_ADC_CLK_CTRL                0xc0
#define A1_PWM_CLK_AB_CTRL                 0xc4
#define A1_PWM_CLK_CD_CTRL                 0xc8
#define A1_PWM_CLK_EF_CTRL                 0xcc
#define A1_SPICC_CLK_CTRL                  0xd0
#define A1_TS_CLK_CTRL                     0xd4
#define A1_SPIFC_CLK_CTRL                  0xd8
#define A1_USB_BUSCLK_CTRL                 0xdc
#define A1_SD_EMMC_CLK_CTRL                0xe0
#define A1_CECA_CLK_CTRL0                  0xe4
#define A1_CECA_CLK_CTRL1                  0xe8
#define A1_CECB_CLK_CTRL0                  0xec
#define A1_CECB_CLK_CTRL1                  0xf0
#define A1_PSRAM_CLK_CTRL                  0xf4
#define A1_DMC_CLK_CTRL                    0xf8
#define A1_FCLK_DIV1_SEL			0xfc
#define A1_TST_CTRL			0x100

/*
 * For PLl register offset
 * APB_BASE:  APB0_BASE_ADDR = 0xfe007c00
 */
/*
 * basic clocks and pll clocks address
 * are not continuous, add 0x7400 for each clock
 */
#define A1_ANACTRL_FIXPLL_CTRL0		(0x80 + 0x7400)
#define A1_ANACTRL_FIXPLL_CTRL1            (0x84 + 0x7400)
#define A1_ANACTRL_FIXPLL_CTRL2            (0x88 + 0x7400)
#define A1_ANACTRL_FIXPLL_CTRL3            (0x8c + 0x7400)
#define A1_ANACTRL_FIXPLL_CTRL4            (0x90 + 0x7400)
#define A1_ANACTRL_FIXPLL_STS              (0x94 + 0x7400)
#define A1_ANACTRL_SYSPLL_CTRL0            (0x100 + 0x7400)
#define A1_ANACTRL_SYSPLL_CTRL1            (0x104 + 0x7400)
#define A1_ANACTRL_SYSPLL_CTRL2		(0x108 + 0x7400)
#define A1_ANACTRL_SYSPLL_CTRL3            (0x10c + 0x7400)
#define A1_ANACTRL_SYSPLL_CTRL4            (0x110 + 0x7400)
#define A1_ANACTRL_SYSPLL_STS              (0x114 + 0x7400)
#define A1_ANACTRL_HIFIPLL_CTRL0           (0x140 + 0x7400)
#define A1_ANACTRL_HIFIPLL_CTRL1           (0x144 + 0x7400)
#define A1_ANACTRL_HIFIPLL_CTRL2           (0x148 + 0x7400)
#define A1_ANACTRL_HIFIPLL_CTRL3           (0x14c + 0x7400)
#define A1_ANACTRL_HIFIPLL_CTRL4           (0x150 + 0x7400)
#define A1_ANACTRL_HIFIPLL_STS             (0x154 + 0x7400)
#define A1_ANACTRL_AUDDDS_CTRL0            (0x180 + 0x7400)
#define A1_ANACTRL_AUDDDS_CTRL1            (0x184 + 0x7400)
#define A1_ANACTRL_AUDDDS_CTRL2            (0x188 + 0x7400)
#define A1_ANACTRL_AUDDDS_CTRL3            (0x18c + 0x7400)
#define A1_ANACTRL_AUDDDS_CTRL4            (0x190 + 0x7400)
#define A1_ANACTRL_AUDDDS_STS              (0x194 + 0x7400)
#define A1_ANACTRL_MISCTOP_CTRL0           (0x1c0 + 0x7400)
#define A1_ANACTRL_POR_CNTL                (0x208 + 0x7400)

/*
 * CPU clock register offset
 * APB_BASE:  APB1_BASE_ADDR = 0xfd000000
 */
/* cpu clock will never be used in uboot
#define CPUCTRL_CLK_CTRL0		0x80
#define CPUCTRL_CLK_CTRL1		0x84
*/

#endif
