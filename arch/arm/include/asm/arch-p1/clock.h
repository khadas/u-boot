/* SPDX-ed:   drivers/clk/meson/clk_meson_p1.c
License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __CLOCK_P1_H
#define __CLOCK_P1_H

// REG_BASE:  REGISTER_BASE_ADDR = 0xfe000000
// -----------------------------------------------
#define P1_CLKCTRL_NAND_CLK_CTRL		(0x005a  << 2)
#define P1_CLKCTRL_SD_EMMC_CLK_CTRL		(0x005b  << 2)
#define P1_CLKCTRL_SPICC_CLK_CTRL		(0x005d  << 2)
#define P1_CLKCTRL_SAR_CLK_CTRL0		(0x005f  << 2)
#define P1_CLKCTRL_SPICC_CLK_CTRL1		(0x0070  << 2)
#define P1_CLKCTRL_SPICC_CLK_CTRL2		(0x0071  << 2)

//========================================================================

// -----------------------------------------------
#define P1_CLKCTRL_GP0PLL_CTRL0			(0x0240 << 2)
#define P1_CLKCTRL_GP0PLL_CTRL1			(0x0241 << 2)
#define P1_CLKCTRL_GP0PLL_CTRL2			(0x0242 << 2)
#define P1_CLKCTRL_GP0PLL_CTRL3			(0x0243 << 2)
#define P1_CLKCTRL_GP0PLL_CTRL4			(0x0244 << 2)
#define P1_CLKCTRL_GP0PLL_CTRL5			(0x0245 << 2)
#define P1_CLKCTRL_GP0PLL_CTRL6			(0x0246 << 2)
#define P1_CLKCTRL_GP0PLL_STS			(0x0247 << 2)
#define P1_CLKCTRL_GP1PLL_CTRL0			(0x0280 << 2)
#define P1_CLKCTRL_GP1PLL_CTRL1			(0x0281 << 2)
#define P1_CLKCTRL_GP1PLL_CTRL2			(0x0282 << 2)
#define P1_CLKCTRL_GP1PLL_CTRL3			(0x0283 << 2)
#define P1_CLKCTRL_GP1PLL_STS			(0x0284 << 2)
#define P1_CLKCTRL_GP2PLL_CTRL0			(0x0290 << 2)
#define P1_CLKCTRL_GP2PLL_CTRL1			(0x0291 << 2)
#define P1_CLKCTRL_GP2PLL_CTRL2			(0x0292 << 2)
#define P1_CLKCTRL_GP2PLL_CTRL3			(0x0293 << 2)
#define P1_CLKCTRL_GP2PLL_STS			(0x0294 << 2)
#define P1_CLKCTRL_SYS0PLL_CTRL0		(0x0200 << 2)
#define P1_CLKCTRL_SYS0PLL_CTRL1		(0x0201 << 2)
#define P1_CLKCTRL_SYS0PLL_CTRL2		(0x0202 << 2)
#define P1_CLKCTRL_SYS0PLL_CTRL3		(0x0203 << 2)
#define P1_CLKCTRL_SYS0PLL_STS			(0x0204 << 2)
#define P1_CLKCTRL_SYS1PLL_CTRL0		(0x0210 << 2)
#define P1_CLKCTRL_SYS1PLL_CTRL1		(0x0211 << 2)
#define P1_CLKCTRL_SYS1PLL_CTRL2		(0x0212 << 2)
#define P1_CLKCTRL_SYS1PLL_CTRL3		(0x0213 << 2)
#define P1_CLKCTRL_SYS1PLL_STS			(0x0214 << 2)
#define P1_CLKCTRL_FIXPLL_CTRL0			(0x0220 << 2)
#define P1_CLKCTRL_FIXPLL_CTRL1			(0x0221 << 2)
#define P1_CLKCTRL_FIXPLL_CTRL2			(0x0222 << 2)
#define P1_CLKCTRL_FIXPLL_CTRL3			(0x0223 << 2)
#define P1_CLKCTRL_FIXPLL_CTRL4			(0x0224 << 2)
#define P1_CLKCTRL_FIXPLL_CTRL5			(0x0225 << 2)
#define P1_CLKCTRL_FIXPLL_CTRL6			(0x0226 << 2)
#define P1_CLKCTRL_FIXPLL_STS			(0x0227 << 2)
//========================================================================

/*
 * CPU clock register offset
 * APB_BASE:  APB1_BASE_ADDR = 0xfe007400
 */

#include <dt-bindings/clock/p1-clkc.h>
#define NR_CLKS				(CLKID_END_BASE)

#endif /* __CLOCK_P1_H */
