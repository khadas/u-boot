/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __C3_H
#define __C3_H

//========================================================================
//  CLK_CTRL
//========================================================================
// -----------------------------------------------
// REG_BASE:  REGISTER_BASE_ADDR = 0xfe000000
// -----------------------------------------------
#define C3_SYS_CLK_EN0		(0x0011  << 2)
#define C3_SYS_CLK_EN1		(0x0012  << 2)
#define C3_SYS_CLK_EN2		(0x0013  << 2)
#define C3_SPIFC_CLK_CTRL	(0x0068  << 2)
#define C3_SAR_ADC_CLK_CTRL	(0x005f  << 2)
#define C3_SPICC_CLK_CTRL	(0x005d  << 2)
#define C3_SD_EMMC_CLK_CTRL	(0x005b  << 2)
#define C3_NAND_CLK_CTRL	(0x005a  << 2)


//========================================================================
//  ANA_CTRL - Registers
//========================================================================
// -----------------------------------------------
// REG_BASE:  REGISTER_BASE_ADDR = 0xfe008000
// -----------------------------------------------
#define C3_ANACTRL_SYSPLL_CTRL0                       ((0x0000  << 2) + 0x8000)
#define C3_ANACTRL_SYSPLL_CTRL1                       ((0x0001  << 2) + 0x8000)
#define C3_ANACTRL_SYSPLL_CTRL2                       ((0x0002  << 2) + 0x8000)
#define C3_ANACTRL_SYSPLL_CTRL3                       ((0x0003  << 2) + 0x8000)
#define C3_ANACTRL_SYSPLL_CTRL4                       ((0x0004  << 2) + 0x8000)
#define C3_ANACTRL_SYSPLL_CTRL5                       ((0x0005  << 2) + 0x8000)
#define C3_ANACTRL_SYSPLL_CTRL6                       ((0x0006  << 2) + 0x8000)
#define C3_ANACTRL_SYSPLL_STS                         ((0x0007  << 2) + 0x8000)
#define C3_ANACTRL_FIXPLL_CTRL0                       ((0x0010  << 2) + 0x8000)
#define C3_ANACTRL_FIXPLL_CTRL1                       ((0x0011  << 2) + 0x8000)
#define C3_ANACTRL_FIXPLL_CTRL2                       ((0x0012  << 2) + 0x8000)
#define C3_ANACTRL_FIXPLL_CTRL3                       ((0x0013  << 2) + 0x8000)
#define C3_ANACTRL_FIXPLL_CTRL4                       ((0x0014  << 2) + 0x8000)
#define C3_ANACTRL_FIXPLL_CTRL5                       ((0x0015  << 2) + 0x8000)
#define C3_ANACTRL_FIXPLL_CTRL6                       ((0x0016  << 2) + 0x8000)
#define C3_ANACTRL_FIXPLL_STS                         ((0x0017  << 2) + 0x8000)

#include <dt-bindings/clock/c3-clkc.h>
#define NR_CLKS				(CLKID_END_BASE)

#endif /* __C1_H */
