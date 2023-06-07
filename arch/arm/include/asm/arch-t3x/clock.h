/* SPDX-ed:   drivers/clk/meson/clk_meson_t3.c
License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __CLOCK_T3X_H
#define __CLOCK_T3X_H

// REG_BASE:  REGISTER_BASE_ADDR = 0xfe000000
// -----------------------------------------------
#define T3X_CLKCTRL_NAND_CLK_CTRL                      (0x005a  << 2)
#define T3X_CLKCTRL_SD_EMMC_CLK_CTRL                   (0x005b  << 2)
#define T3X_CLKCTRL_SPICC_CLK_CTRL                     (0x005d  << 2)
#define T3X_CLKCTRL_SAR_CLK_CTRL0                      (0x005f  << 2)
#define T3X_CLKCTRL_SPICC_CLK_CTRL                     (0x005d  << 2)
#define T3X_CLKCTRL_SPICC_CLK_CTRL1                    (0x0070  << 2)
#define T3X_CLKCTRL_SPICC_CLK_CTRL2                    (0x0071  << 2)

//========================================================================

// REG_BASE:  REGISTER_BASE_ADDR = 0xfe000000
// -----------------------------------------------
#define T3X_CLKCTRL_SYS0PLL_CTRL0                      (0x200 << 2)
#define T3X_CLKCTRL_FIXPLL_CTRL0                       (0x220 << 2)
#define T3X_CLKCTRL_GP0PLL_CTRL0                       (0x240 << 2)
//========================================================================

/*
 * CPU clok register offset
 * APB_BASE:  APB1_BASE_ADDR = 0xfe007400
 */

#include <dt-bindings/clock/t3x-clkc.h>
#define NR_CLKS				(CLKID_END_BASE)

#endif /* __CLOCK_T3X_H */
