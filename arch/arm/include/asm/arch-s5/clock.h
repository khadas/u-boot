/* SPDX-ed:   drivers/clk/meson/clk_meson_p1.c
License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __CLOCK_S5_H
#define __CLOCK_S5_H

// REG_BASE:  REGISTER_BASE_ADDR = 0xfe000000
// -----------------------------------------------
#define S5_CLKCTRL_SYS_CLK_EN0_REG1		(0x0012  << 2)
#define S5_CLKCTRL_ETH_CLK_CTRL			(0x0059  << 2)
#define S5_CLKCTRL_NAND_CLK_CTRL		(0x005a  << 2)
#define S5_CLKCTRL_SD_EMMC_CLK_CTRL		(0x005b  << 2)
#define S5_CLKCTRL_SPICC_CLK_CTRL		(0x005d  << 2)
#define S5_CLKCTRL_SAR_CLK_CTRL0		(0x005f  << 2)
#define S5_CLKCTRL_SPICC_CLK_CTRL1		(0x0070  << 2)
#define S5_CLKCTRL_SPICC_CLK_CTRL2		(0x0071  << 2)

//========================================================================

// -----------------------------------------------
#define S5_CLKCTRL_GP0PLL_CTRL0			(0x0240 << 2)
#define S5_CLKCTRL_GP0PLL_CTRL1			(0x0241 << 2)
#define S5_CLKCTRL_GP0PLL_CTRL2			(0x0242 << 2)
#define S5_CLKCTRL_GP0PLL_CTRL3			(0x0243 << 2)
#define S5_CLKCTRL_GP0PLL_STS			(0x0247 << 2)
#define S5_CLKCTRL_GP1PLL_CTRL0			(0x0280 << 2)
#define S5_CLKCTRL_GP1PLL_CTRL1			(0x0281 << 2)
#define S5_CLKCTRL_GP1PLL_CTRL2			(0x0282 << 2)
#define S5_CLKCTRL_GP1PLL_CTRL3			(0x0283 << 2)
#define S5_CLKCTRL_GP1PLL_STS			(0x0284 << 2)
#define S5_CLKCTRL_GP2PLL_CTRL0			(0x0285 << 2)
#define S5_CLKCTRL_GP2PLL_CTRL1			(0x0286 << 2)
#define S5_CLKCTRL_GP2PLL_CTRL2			(0x0287 << 2)
#define S5_CLKCTRL_GP2PLL_CTRL3			(0x0288 << 2)
#define S5_CLKCTRL_GP2PLL_STS			(0x0289 << 2)
#define S5_CLKCTRL_SYS0PLL_CTRL0		(0x0200 << 2)
#define S5_CLKCTRL_SYS0PLL_CTRL1		(0x0201 << 2)
#define S5_CLKCTRL_SYS0PLL_CTRL2		(0x0202 << 2)
#define S5_CLKCTRL_SYS0PLL_CTRL3		(0x0203 << 2)
#define S5_CLKCTRL_SYS0PLL_STS			(0x0204 << 2)
#define S5_CLKCTRL_SYS1PLL_CTRL0		(0x0210 << 2)
#define S5_CLKCTRL_SYS1PLL_CTRL1		(0x0211 << 2)
#define S5_CLKCTRL_SYS1PLL_CTRL2		(0x0212 << 2)
#define S5_CLKCTRL_SYS1PLL_CTRL3		(0x0213 << 2)
#define S5_CLKCTRL_SYS1PLL_STS			(0x0214 << 2)
#define S5_CLKCTRL_FIXPLL_CTRL0			(0x0220 << 2)
#define S5_CLKCTRL_FIXPLL_CTRL1			(0x0221 << 2)
#define S5_CLKCTRL_FIXPLL_CTRL2			(0x0222 << 2)
#define S5_CLKCTRL_FIXPLL_CTRL3			(0x0223 << 2)
#define S5_CLKCTRL_FIXPLL_STS			(0x0227 << 2)
//========================================================================

/*
 * CPU clok register offset
 * APB_BASE:  APB1_BASE_ADDR = 0xfe007400
 */

#include <dt-bindings/clock/s5-clkc.h>
#define NR_CLKS				(CLKID_END_BASE)

#endif /* __CLOCK_S5_H */
