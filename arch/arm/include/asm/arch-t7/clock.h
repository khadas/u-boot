/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __CLOCK_T7_H
#define __CLOCK_T7_H

// REG_BASE:  REGISTER_BASE_ADDR = 0xfe000000
// -----------------------------------------------
#define T7_CLKCTRL_NAND_CLK_CTRL                      (0x005a  << 2)
#define T7_CLKCTRL_SD_EMMC_CLK_CTRL                   (0x005b  << 2)
#define T7_CLKCTRL_SPICC_CLK_CTRL                     (0x005d  << 2)
#define T7_CLKCTRL_SAR_CLK_CTRL0                      (0x005f  << 2)
#define T7_CLKCTRL_SPICC_CLK_CTRL1                    (0x0070  << 2)
#define T7_CLKCTRL_SPICC_CLK_CTRL2                    (0x0071  << 2)

//========================================================================

// REG_BASE:  REGISTER_BASE_ADDR = 0xfe008000
// -----------------------------------------------
#define T7_ANACTRL_SYSPLL_CTRL0                       ((0x00 << 2) + 0x8000)
#define T7_ANACTRL_SYSPLL_CTRL1                       ((0x01 << 2) + 0x8000)
#define T7_ANACTRL_SYSPLL_CTRL2                       ((0x02 << 2) + 0x8000)
#define T7_ANACTRL_SYSPLL_CTRL3                       ((0x03 << 2) + 0x8000)
#define T7_ANACTRL_SYSPLL_CTRL4                       ((0x04 << 2) + 0x8000)
#define T7_ANACTRL_SYSPLL_CTRL5                       ((0x05 << 2) + 0x8000)
#define T7_ANACTRL_SYSPLL_CTRL6                       ((0x06 << 2) + 0x8000)
#define T7_ANACTRL_SYSPLL_STS                         ((0x07 << 2) + 0x8000)
#define T7_ANACTRL_FIXPLL_CTRL0                       ((0x10 << 2) + 0x8000)
#define T7_ANACTRL_FIXPLL_CTRL1                       ((0x11 << 2) + 0x8000)
#define T7_ANACTRL_FIXPLL_CTRL2                       ((0x12 << 2) + 0x8000)
#define T7_ANACTRL_FIXPLL_CTRL3                       ((0x13 << 2) + 0x8000)
#define T7_ANACTRL_FIXPLL_CTRL4                       ((0x14 << 2) + 0x8000)
#define T7_ANACTRL_FIXPLL_CTRL5                       ((0x15 << 2) + 0x8000)
#define T7_ANACTRL_FIXPLL_CTRL6                       ((0x16 << 2) + 0x8000)
#define T7_ANACTRL_FIXPLL_STS                         ((0x17 << 2) + 0x8000)
#define T7_ANACTRL_GP0PLL_CTRL0                       ((0x20 << 2) + 0x8000)
//========================================================================

/*
 * CPU clock register offset
 * APB_BASE:  APB1_BASE_ADDR = 0xfe007400
 */

#include <dt-bindings/clock/t7-clkc.h>
#define NR_CLKS				(CLKID_END_BASE)

#endif /* __CLOCK_T7_H */
