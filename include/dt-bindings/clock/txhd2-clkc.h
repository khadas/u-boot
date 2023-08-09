/*
 * include/dt-bindings/clock/txhd2-clkc.h
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

#ifndef __DT_BINDINGS_CLOCK_TXHD2_H
#define __DT_BINDINGS_CLOCK_TXHD2_H
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

#define CLKID_SAR_ADC_GATE	11
#define CLKID_SPICC_A_GATE	12
#define CLKID_SPICC_B_GATE	13
#define CLKID_SPICC_C_GATE	14
#define CLKID_SD_EMMC_C_GATE	15

#define CLKID_SARADC_MUX	16
#define CLKID_SPICC_A_MUX	17
#define CLKID_SD_EMMC_C_MUX	18

#define CLKID_SARADC_DIV	19
#define CLKID_SPICC_A_DIV	20
#define CLKID_SD_EMMC_C_DIV	21


#define CLKID_UNREALIZED	100

#endif /* __TXHD2_CLKC_H */
