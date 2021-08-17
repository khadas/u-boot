/*
 * include/dt-bindings/clock/sc2-clkc.h
 *
 * Copyright (C) 2018 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#ifndef __DT_BINDINGS_CLOCK_P1_H
#define __DT_BINDINGS_CLOCK_P1_H
/*
 * CLKID index values
 */
#define CLKID_XTAL		0
#define CLKID_FIXED_PLL         1
#define CLKID_FCLK_DIV2         2
#define CLKID_FCLK_DIV3         3
#define CLKID_FCLK_DIV5         4
#define CLKID_FCLK_DIV7         5
#define CLKID_FCLK_DIV2P5       6
#define CLKID_FCLK_DIV4       	7
#define CLKID_SYS_CLK		8
#define CLKID_SYS_PLL		9
#define CLKID_GP0_PLL		10
#define CLKID_GP1_PLL		11
#define	CLKID_SD_EMMC_A_XTAL_GATE	12
#define	CLKID_SD_EMMC_B_XTAL_GATE	13
#define	CLKID_SD_EMMC_C_XTAL_GATE	14


#define CLKID_GATE_BASE		15
#define CLKID_SAR_ADC_GATE	(CLKID_GATE_BASE + 0)
#define CLKID_SPICC_A_GATE	(CLKID_GATE_BASE + 1)
#define CLKID_SPICC_B_GATE	(CLKID_GATE_BASE + 2)
#define CLKID_SPICC_C_GATE	(CLKID_GATE_BASE + 3)
#define CLKID_SPICC_D_GATE	(CLKID_GATE_BASE + 4)
#define CLKID_SPICC_E_GATE	(CLKID_GATE_BASE + 5)
#define CLKID_SPICC_F_GATE	(CLKID_GATE_BASE + 6)
#define CLKID_SD_EMMC_B_GATE	(CLKID_GATE_BASE + 7)
#define CLKID_SD_EMMC_C_GATE	(CLKID_GATE_BASE + 8)
#define CLKID_SD_EMMC_A_GATE	(CLKID_GATE_BASE + 9)

#define CLKID_MUX_BASE		(CLKID_GATE_BASE + 10)
#define CLKID_SARADC_MUX	(CLKID_MUX_BASE + 0)
#define CLKID_SPICC_A_MUX	(CLKID_MUX_BASE + 1)
#define CLKID_SPICC_B_MUX	(CLKID_MUX_BASE + 2)
#define CLKID_SPICC_C_MUX	(CLKID_MUX_BASE + 3)
#define CLKID_SPICC_D_MUX	(CLKID_MUX_BASE + 4)
#define CLKID_SPICC_E_MUX	(CLKID_MUX_BASE + 5)
#define CLKID_SPICC_F_MUX	(CLKID_MUX_BASE + 6)
#define CLKID_SD_EMMC_B_MUX	(CLKID_MUX_BASE + 7)
#define CLKID_SD_EMMC_C_MUX	(CLKID_MUX_BASE + 8)
#define CLKID_SD_EMMC_A_MUX	(CLKID_MUX_BASE + 9)

#define CLKID_DIV_BASE		(CLKID_MUX_BASE + 10)
#define CLKID_SARADC_DIV	(CLKID_DIV_BASE + 0)
#define CLKID_SPICC_A_DIV	(CLKID_DIV_BASE + 1)
#define CLKID_SPICC_B_DIV	(CLKID_DIV_BASE + 2)
#define CLKID_SPICC_C_DIV	(CLKID_DIV_BASE + 3)
#define CLKID_SPICC_D_DIV	(CLKID_DIV_BASE + 4)
#define CLKID_SPICC_E_DIV	(CLKID_DIV_BASE + 5)
#define CLKID_SPICC_F_DIV	(CLKID_DIV_BASE + 6)
#define CLKID_SD_EMMC_B_DIV	(CLKID_DIV_BASE + 7)
#define CLKID_SD_EMMC_C_DIV	(CLKID_DIV_BASE + 8)
#define CLKID_SD_EMMC_A_DIV	(CLKID_MUX_BASE + 9)


#define CLKID_UNREALIZED	100

#endif /* __P1_CLKC_H */
