/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/include/asm/arch-TXHD2/clock.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __CLOCK_TXHD2_H
#define __CLOCK_TXHD2_H

// basic 0xff646000 need to check new offet while not secure_apb
#define TXHD2_BASIC_BASE                                 (0xA000)
#define TXHD2_CLKCTRL_NAND_CLK_CTRL                      (TXHD2_BASIC_BASE + (0x0097 << 2))
#define TXHD2_CLKCTRL_SD_EMMC_CLK_CTRL                   (TXHD2_BASIC_BASE + (0x0099 << 2))
#define TXHD2_CLKCTRL_SPICC_CLK_CTRL                     (TXHD2_BASIC_BASE + (0x00f7 << 2))
#define TXHD2_CLKCTRL_SAR_CLK_CTRL0                      (TXHD2_BASIC_BASE +\
		(0x0024 << 2) + 0x1BA000)
#define TXHD2_CLKCTRL_SPICC_CLK_CTRL1                    (TXHD2_BASIC_BASE + (0x00f8 << 2))

// basic 0xff63c000
#define TXHD2_HHI_SYS_PLL_CNTL0                              (0x0bd << 2)
#define TXHD2_HHI_FIX_PLL_CNTL0                              (0x0a8 << 2)
#define TXHD2_HHI_GP0_PLL_CNTL0                              (0x010 << 2)

#include <dt-bindings/clock/txhd2-clkc.h>
#define NR_CLKS				(CLKID_END_BASE)

#endif /* __CLOCK_TXHD2_H */
