/* SPDX-ed:   drivers/clk/meson/clk_meson_t3.c
License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __CLOCK_T5M_H
#define __CLOCK_T5M_H

// REG_BASE:  REGISTER_BASE_ADDR = 0xfe000000
// -----------------------------------------------
#define T5M_CLKCTRL_NAND_CLK_CTRL                      (0x005a  << 2)
#define T5M_CLKCTRL_SD_EMMC_CLK_CTRL                   (0x005b  << 2)
#define T5M_CLKCTRL_SPICC_CLK_CTRL                     (0x005d  << 2)
#define T5M_CLKCTRL_SAR_CLK_CTRL0                      (0x005f  << 2)
#define T5M_CLKCTRL_SPICC_CLK_CTRL1                    (0x0070  << 2)

//========================================================================

// REG_BASE:  REGISTER_BASE_ADDR = 0xfe008000
// -----------------------------------------------
#define T5M_ANACTRL_SYS0PLL_CTRL0                      ((0x00 << 2) + 0x8000)
#define T5M_ANACTRL_FIXPLL_CTRL0                       ((0x10 << 2) + 0x8000)
#define T5M_ANACTRL_GP0PLL_CTRL0                       ((0x20 << 2) + 0x8000)
//========================================================================

/*
 * CPU clok register offset
 * APB_BASE:  APB1_BASE_ADDR = 0xfe007400
 */

#include <dt-bindings/clock/t5m-clkc.h>
#define NR_CLKS				(CLKID_END_BASE)

#endif /* __CLOCK_T5M_H */
