/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __A5_H
#define __A5_H

// REG_BASE:  REGISTER_BASE_ADDR = 0xfe000000
// -----------------------------------------------
#define A5_CLKCTRL_NAND_CLK_CTRL                      (0x005a  << 2)
#define A5_CLKCTRL_SD_EMMC_CLK_CTRL                   (0x005b  << 2)
#define A5_CLKCTRL_SPICC_CLK_CTRL                     (0x005d  << 2)
#define A5_CLKCTRL_SAR_CLK_CTRL0                      (0x005f  << 2)

//========================================================================

// REG_BASE:  REGISTER_BASE_ADDR = 0xfe008000
// -----------------------------------------------
#define A5_ANACTRL_SYSPLL_CTRL0                       ((0x00 << 2) + 0x8000)
#define A5_ANACTRL_FIXPLL_CTRL0                       ((0x10 << 2) + 0x8000)
#define A5_ANACTRL_GP0PLL_CTRL0                       ((0x20 << 2) + 0x8000)
//========================================================================

/*
 * CPU clock register offset
 * APB_BASE:  APB1_BASE_ADDR = 0xfe007400
 */

#include <dt-bindings/clock/a5-clkc.h>
#define NR_CLKS				(CLKID_END_BASE)

#endif /* __A5_H */
