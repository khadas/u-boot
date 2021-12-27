/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __DT_BINDINGS_CLOCK_A5_H
#define __DT_BINDINGS_CLOCK_A5_H
/*
 * CLKID index values
 */
#define CLKID_XTAL		0
#define CLKID_FIXED_PLL         1
#define CLKID_FCLK_DIV2         2
#define CLKID_FCLK_DIV3         3
#define CLKID_FCLK_DIV4       	4
#define CLKID_FCLK_DIV5         5
#define CLKID_FCLK_DIV7       	6
#define CLKID_FCLK_DIV2P5       7
#define CLKID_SYS_CLK		8
#define CLKID_SYS_PLL		9
#define CLKID_GP0_PLL		10

#define CLKID_GATE_BASE		11
#define CLKID_SARADC		(CLKID_GATE_BASE + 0)
#define CLKID_SPICC_0		(CLKID_GATE_BASE + 1)
#define CLKID_SPICC_1		(CLKID_GATE_BASE + 2)
#define CLKID_SD_EMMC_C		(CLKID_GATE_BASE + 3)
#define CLKID_SD_EMMC_A		(CLKID_GATE_BASE + 4)

#define CLKID_MUX_BASE		(CLKID_GATE_BASE + 5)
#define CLKID_SARADC_SEL	(CLKID_MUX_BASE + 0)
#define CLKID_SPICC_0_SEL	(CLKID_MUX_BASE + 1)
#define CLKID_SPICC_1_SEL	(CLKID_MUX_BASE + 2)
#define CLKID_SD_EMMC_C_SEL	(CLKID_MUX_BASE + 3)
#define CLKID_SD_EMMC_A_SEL	(CLKID_MUX_BASE + 4)

#define CLKID_DIV_BASE		(CLKID_MUX_BASE + 5)
#define CLKID_SARADC_DIV	(CLKID_DIV_BASE + 0)
#define CLKID_SPICC_0_DIV	(CLKID_DIV_BASE + 1)
#define CLKID_SPICC_1_DIV	(CLKID_DIV_BASE + 2)
#define CLKID_SD_EMMC_C_DIV	(CLKID_DIV_BASE + 3)
#define CLKID_SD_EMMC_A_DIV	(CLKID_DIV_BASE + 4)

#define CLKID_UNREALIZED	100

#endif /* __A5_CLKC_H */
