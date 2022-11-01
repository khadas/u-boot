/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/include/asm/arch-t5w/clock.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __CLOCK_T5W_H
#define __CLOCK_T5W_H

#define T5W_CLKCTRL_NAND_CLK_CTRL                      (0x0097  << 2)
#define T5W_CLKCTRL_SD_EMMC_CLK_CTRL                   (0x0099  << 2)
#define T5W_CLKCTRL_SPICC_CLK_CTRL                     (0x00f7  << 2)
#define T5W_CLKCTRL_SAR_CLK_CTRL0                      ((0x0024  << 2) + 0x1BA000)//????
#define T5W_CLKCTRL_SPICC_CLK_CTRL1                    (0x00f8  << 2)

#define T5W_ANACTRL_SYSPLL_CTRL0                       (0xe0 << 2)
#define T5W_ANACTRL_FIXPLL_CTRL0                       (0xd0 << 2)
#define T5W_ANACTRL_GP0PLL_CTRL0                       (0x20 << 2)

#include <dt-bindings/clock/t5w-clkc.h>
#define NR_CLKS				(CLKID_END_BASE)

#endif /* __CLOCK_T5W_H */
