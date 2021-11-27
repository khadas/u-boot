/*
 * board/amlogic/p1_as409/firmware/timing.c
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <asm/arch/secure_apb.h>
#include <asm/arch/timing.h>
#include <asm/arch/ddr_define.h>

/* board clk defines */
#define CPU_CLK                                 1512

/* ddr config support multiple configs for boards which use same bootloader:
 * config steps:
 * 1. add a new data struct in __ddr_setting[]
 * 2. config correct board_id, ddr_type, freq, etc..
 */


/* CAUTION!! */
/* Confirm ddr configs with hardware designer,
 * if you don't know how to config, then don't edit it
 */

/* Key configs */
/*
 * board_id: check hardware adc config
 * dram_rank_config:
 *            #define CONFIG_DDR_CHL_AUTO					0xF
 *            #define CONFIG_DDR0_16BIT_CH0				0x1
 *            #define CONFIG_DDR0_16BIT_RANK01_CH0		0x4
 *            #define CONFIG_DDR0_32BIT_RANK0_CH0			0x2
 *            #define CONFIG_DDR0_32BIT_RANK01_CH01		0x3
 *            #define CONFIG_DDR0_32BIT_16BIT_RANK0_CH0	0x5
 *            #define CONFIG_DDR0_32BIT_16BIT_RANK01_CH0	0x6
 * DramType:
 *            #define CONFIG_DDR_TYPE_DDR3				0
 *            #define CONFIG_DDR_TYPE_DDR4				1
 *            #define CONFIG_DDR_TYPE_LPDDR4				2
 *            #define CONFIG_DDR_TYPE_LPDDR3				3
 * DRAMFreq:
 *            {pstate0, pstate1, pstate2, pstate3} //more than one pstate means use dynamic freq
 *
 */


#define CONFIG_DDR_TYPE_LPDDR5                                    6
#define DDR_RFC_TYPE_LPDDR5_4Gbx1				15
#define DDR_RFC_TYPE_LPDDR5_6Gbx1				16
#define DDR_RFC_TYPE_LPDDR5_8Gbx1				17
#define         DDR0_1G_DDR1_0G          0
#define         DDR0_2G_DDR1_0G          1
#define         DDR0_1G_DDR1_1G          2
#define         DDR0_2G_DDR1_1G          3
#define         DDR0_2G_DDR1_2G          4
#define         DDR0_2G_DDR1_2G_RANK01   5
#define         DDR0_4G_DDR1_4G_RANK01   6

//#define BOARD_ENABLE_DDR4    1
//#define BOARD_ENABLE_LPDDR4   1
#define BOARD_ENABLE_LPDDR5   1
#define BOARD_ENABLE_LPDDR5_RDL_MODIFY   1
//#define BOARD_ENABLE_LPDDR5_OPS_FT_PINMUX   1

#define DDR_RESV_CHECK_ID_ENABLE  0Xfe
#define SAR_ADC_DDR_ID_BASE   0
#define SAR_ADC_DDR_ID_STEP   80

#define ADC_PORT0 (0 << 6)
#define ADC_PORT1 (1 << 6)
#define ADC_PORT2 (2 << 6)
#define ADC_PORT3 (3 << 6)
#define ADC_ID_0   0 // size lp4  1+2 gpioid_11(1,0)
#define ADC_ID_1   1 // size lp4  3+4 gpioid_11(1,0)
#define ADC_ID_2   2 // size lp4  5+6 gpioid_11(1,0)
#define ADC_ID_3   3 // size lp4x  1+2 gpioid_11(1,0)
#define ADC_ID_4   4 // size lp4x  3+4 gpioid_11(1,0)
#define ADC_ID_5   5 // size lp4x  7+8 gpioid_11(1,0)
#define ADC_ID_6   6 // size lp5  1+2 gpioid_11(1,0)
#define ADC_ID_7   7 // size lp5  3+4 gpioid_11(1,0)
#define ADC_ID_8   8 // size lp5  5+6 gpioid_11(1,0)
#define ADC_ID_9   9 // size lp5  7+8 gpioid_11(1,0)
#define ADC_ID_10   10 //
#define ADC_ID_11   11 //
#define ADC_ID_12   12 //

#define DDR_TIMMING_OFFSET(X) (unsigned int)(unsigned long)(&(((ddr_set_t *)(0))->X))
#define DDR_TIMMING_OFFSET_SIZE(X) sizeof(((ddr_set_t *)(0))->X)
#define DDR_TIMMING_TUNE_TIMMING0(DDR_ID, PARA, VALUE) {DDR_ID, DDR_TIMMING_OFFSET(PARA), VALUE, DDR_TIMMING_OFFSET_SIZE(PARA), 0, DDR_RESV_CHECK_ID_ENABLE}
#define DDR_TIMMING_TUNE_TIMMING1(DDR_ID, PARA, VALUE) {DDR_ID, sizeof(ddr_set_t) + DDR_TIMMING_OFFSET(PARA), VALUE, DDR_TIMMING_OFFSET_SIZE(PARA), 0, DDR_RESV_CHECK_ID_ENABLE}
bl2_reg_t __bl2_reg[] __attribute__ ((section(".generic_param"))) = {
	DDR_TIMMING_TUNE_TIMMING0(
	ADC_PORT0 + ADC_ID_5,
	DramType,
	CONFIG_DDR_TYPE_LPDDR4
	),
	DDR_TIMMING_TUNE_TIMMING0(
	ADC_PORT0 + ADC_ID_5,
	enable_lpddr4x_mode,
	1
	),
	DDR_TIMMING_TUNE_TIMMING0(
	ADC_PORT0 + ADC_ID_5,
	DRAMFreq[0],
	2112
	),
	DDR_TIMMING_TUNE_TIMMING0(
	ADC_PORT0 + ADC_ID_5,
	DRAMFreq[1],
	792
	),
	DDR_TIMMING_TUNE_TIMMING0(
	ADC_PORT0 + ADC_ID_5,
	ddr_rfc_type,
	DDR_RFC_TYPE_LPDDR4_8Gbx1
	),
	DDR_TIMMING_TUNE_TIMMING0(
	ADC_PORT0 + ADC_ID_5,
	training_SequenceCtrl[0],
	0x131f
	),
	DDR_TIMMING_TUNE_TIMMING0(
	ADC_PORT0 + ADC_ID_5,
	ddr_rdbi_wr_enable,
	0
	),
	DDR_TIMMING_TUNE_TIMMING0(
	ADC_PORT0 + ADC_ID_5,
	dram_ac_odt_ohm,
	120
	),
	DDR_TIMMING_TUNE_TIMMING0(
	ADC_PORT0 + ADC_ID_5,
	ddr_dmc_remap[0],
	(0 | 5 << 5 | 6 << 10 | 7 << 15 | 8 << 20 | 9 << 25)
	),
	DDR_TIMMING_TUNE_TIMMING0(
	ADC_PORT0 + ADC_ID_5,
	ddr_dmc_remap[1],
	(10 | 0 << 5 | 0 << 10 | 11 << 15 | 15 << 20 | 16 << 25)
	),
	DDR_TIMMING_TUNE_TIMMING0(
	ADC_PORT0 + ADC_ID_5,
	ddr_dmc_remap[2],
	(17 | 18 << 5 | 19 << 10 | 20 << 15 | 21 << 20 | 22 << 25)
	),
	DDR_TIMMING_TUNE_TIMMING0(
	ADC_PORT0 + ADC_ID_5,
	ddr_dmc_remap[3],
	(23 | 24 << 5 | 25 << 10 | 26 << 15 | 27 << 20 | 28 << 25)
	),
	DDR_TIMMING_TUNE_TIMMING0(
	ADC_PORT0 + ADC_ID_5,
	ddr_dmc_remap[4],
	(29 | 12 << 5 | 13 << 10 | 14 << 15 | 0 << 20 | 0 << 25)
	),

	{
	ADC_PORT0 + ADC_ID_5, DDR_TIMMING_OFFSET(ddr_lpddr34_ca_remap[0]),
	(1 << 0) | (0 << 8) | (3 << 16) | (2 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, DDR_TIMMING_OFFSET(ac_pinmux[0]),
	(1 << 0) | (0 << 8) | (2 << 16) | (5 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, DDR_TIMMING_OFFSET(ac_pinmux[4]),
	(4 << 0) | (3 << 8) | (6 << 16) | (0 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, DDR_TIMMING_OFFSET(ac_pinmux[12]),
	(1 << 0) | (4 << 8) | (2 << 16) | (5 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, DDR_TIMMING_OFFSET(ac_pinmux[16]),
	(0 << 0) | (3 << 8) | (6 << 16) | (0 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[0]),
	(5 << 0) | (4 << 8) | (1 << 16) | (2 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[4]),
	(0 << 0) | (3 << 8) | (7 << 16) | (6 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[8]),
	(8 << 0) | (10 << 8) | (9 << 16) | (11 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[12]),
	(13 << 0) | (15 << 8) | (14 << 16) | (12 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[16]),
	(18 << 0) | (19 << 8) | (23 << 16) | (22 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[20]),
	(21 << 0) | (20 << 8) | (16 << 16) | (17 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[24]),
	(27 << 0) | (25 << 8) | (26 << 16) | (28 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[28]),
	(24 << 0) | (31 << 8) | (29 << 16) | (30 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},

	DDR_TIMMING_TUNE_TIMMING1(
	ADC_PORT0 + ADC_ID_5,
	DramType,
	CONFIG_DDR_TYPE_LPDDR4
	),
	DDR_TIMMING_TUNE_TIMMING1(
	ADC_PORT0 + ADC_ID_5,
	enable_lpddr4x_mode,
	1
	),
	DDR_TIMMING_TUNE_TIMMING1(
	ADC_PORT0 + ADC_ID_5,
	DRAMFreq[0],
	2112
	),
	DDR_TIMMING_TUNE_TIMMING1(
	ADC_PORT0 + ADC_ID_5,
	DRAMFreq[1],
	792
	),
	DDR_TIMMING_TUNE_TIMMING1(
	ADC_PORT0 + ADC_ID_5,
	ddr_rfc_type,
	DDR_RFC_TYPE_LPDDR4_8Gbx1
	),
	DDR_TIMMING_TUNE_TIMMING1(
	ADC_PORT0 + ADC_ID_5,
	training_SequenceCtrl[0],
	0x131f
	),
	DDR_TIMMING_TUNE_TIMMING1(
	ADC_PORT0 + ADC_ID_5,
	ddr_rdbi_wr_enable,
	0
	),
	DDR_TIMMING_TUNE_TIMMING1(
	ADC_PORT0 + ADC_ID_5,
	dram_ac_odt_ohm,
	120
	),
	DDR_TIMMING_TUNE_TIMMING1(
	ADC_PORT0 + ADC_ID_5,
	ddr_dmc_remap[0],
	(0 | 5 << 5 | 6 << 10 | 7 << 15 | 8 << 20 | 9 << 25)
	),
	DDR_TIMMING_TUNE_TIMMING1(
	ADC_PORT0 + ADC_ID_5,
	ddr_dmc_remap[1],
	(10 | 0 << 5 | 0 << 10 | 11 << 15 | 15 << 20 | 16 << 25)
	),
	DDR_TIMMING_TUNE_TIMMING1(
	ADC_PORT0 + ADC_ID_5,
	ddr_dmc_remap[2],
	(17 | 18 << 5 | 19 << 10 | 20 << 15 | 21 << 20 | 22 << 25)
	),
	DDR_TIMMING_TUNE_TIMMING1(
	ADC_PORT0 + ADC_ID_5,
	ddr_dmc_remap[3],
	(23 | 24 << 5 | 25 << 10 | 26 << 15 | 27 << 20 | 28 << 25)
	),
	DDR_TIMMING_TUNE_TIMMING1(
	ADC_PORT0 + ADC_ID_5,
	ddr_dmc_remap[4],
	(29 | 12 << 5 | 13 << 10 | 14 << 15 | 0 << 20 | 0 << 25)
	),
	{
	ADC_PORT0 + ADC_ID_5, sizeof(ddr_set_t) + DDR_TIMMING_OFFSET(ddr_lpddr34_ca_remap[0]),
	(0 << 0) | (1 << 8) | (2 << 16) | (3 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, sizeof(ddr_set_t) + DDR_TIMMING_OFFSET(ac_pinmux[0]),
	(4 << 0) | (1 << 8) | (3 << 16) | (5 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, sizeof(ddr_set_t) + DDR_TIMMING_OFFSET(ac_pinmux[4]),
	(2 << 0) | (0 << 8) | (6 << 16) | (0 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, sizeof(ddr_set_t) + DDR_TIMMING_OFFSET(ac_pinmux[12]),
	(3 << 0) | (5 << 8) | (4 << 16) | (0 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, sizeof(ddr_set_t) + DDR_TIMMING_OFFSET(ac_pinmux[16]),
	(2 << 0) | (1 << 8) | (6 << 16) | (0 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, sizeof(ddr_set_t) + DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[0]),
	(7 << 0) | (2 << 8) | (1 << 16) | (3 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, sizeof(ddr_set_t) + DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[4]),
	(4 << 0) | (0 << 8) | (5 << 16) | (6 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, sizeof(ddr_set_t) + DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[8]),
	(8 << 0) | (10 << 8) | (9 << 16) | (11 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, sizeof(ddr_set_t) + DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[12]),
	(13 << 0) | (15 << 8) | (14 << 16) | (12 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, sizeof(ddr_set_t) + DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[16]),
	(16 << 0) | (19 << 8) | (20 << 16) | (23 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, sizeof(ddr_set_t) + DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[20]),
	(22 << 0) | (21 << 8) | (18 << 16) | (17 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, sizeof(ddr_set_t) + DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[24]),
	(28 << 0) | (31 << 8) | (29 << 16) | (30 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
	{
	ADC_PORT0 + ADC_ID_5, sizeof(ddr_set_t) + DDR_TIMMING_OFFSET(ddr_lpddr34_dq_remap[28]),
	(25 << 0) | (26 << 8) | (24 << 16) | (27 << 24),
	4, 0, DDR_RESV_CHECK_ID_ENABLE},
};

#if BOARD_ENABLE_LPDDR4

#define ENABLE_LPDDR4X_MODE 1
#ifdef CONFIG_PXP_DDR_FIX_2112_TIMMING
	#define   LPDDR4_DDR_CLK  2112
#else
	#define   LPDDR4_DDR_CLK  2112//2112//1584//1200//600
#endif

#define DDR_SIZE_TYPE_1         DDR0_2G_DDR1_2G//DDR0_4G_DDR1_4G_RANK01

ddr_set_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {
	{
	//.fast_boot[0]=0x000000fd,// 253
	.board_id = CONFIG_BOARD_ID_MASK,
	.version = 1,
	//.dram_rank_config		= CONFIG_DDR0_32BIT_RANK0_CH0,
	.DramType = CONFIG_DDR_TYPE_LPDDR4,
	.enable_lpddr4x_mode = ENABLE_LPDDR4X_MODE,
	.DRAMFreq = { LPDDR4_DDR_CLK,		     792,	     0,	     0	    },
	.ddr_rfc_type = DDR_RFC_TYPE_LPDDR4_8Gbx1,
	.ddr_base_addr = CFG_DDR_BASE_ADDR,
	.ddr_start_offset = CFG_DDR_START_OFFSET,
	.DisabledDbyte[0] = 0xf0,//bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
	.DisabledDbyte[1] = 0xf0,//bit 0 -3 ch1 cs0 ,bit 4-7 ch1 cs1,
	.Is2Ttiming = 0,
	.HdtCtrl = 0xc8,
	//.dram_ch0_size_MB		= 0xffff,
	//.dram_ch1_size_MB		= 0,
#if (DDR_SIZE_TYPE_1 == DDR0_1G_DDR1_0G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_0G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_1G_DDR1_1G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_1G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_2G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_2G_RANK01)
	.DisabledDbyte[0] = 0x00,                               //bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
	.DisabledDbyte[1] = 0x00,                               //bit 0 -3 ch1 cs0 ,bit 4-7 ch1 cs1,
	.Is2Ttiming = 0,
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_4G_DDR1_4G_RANK01)
	.DisabledDbyte[0] = 0x00,                               //bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
	.DisabledDbyte[1] = 0x00,                               //bit 0 -3 ch1 cs0 ,bit 4-7 ch1 cs1,
	.Is2Ttiming = 0,
	//.HdtCtrl = 0x4,
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#endif
	.training_SequenceCtrl = { 0x131f,			  0x61 },//ddr3 0x21f 0x31f
	//use 0x23 0x13  compatibility with 1rank and 2rank
	//targeting rank 0. [3:0] is used
	//for write ODT [7:4] is used for //read ODT
	.phy_odt_config_rank = { 0x23,				0x13 },
	//use 0d0d compatibility with 1rank and 2rank  //0808
	.dfi_odt_config = 0x0d0d,
	.PllBypassEn = 0,         //bit0-ps0,bit1-ps1
	.ddr_rdbi_wr_enable = 0x0,//bit 0 read-dbi,bit 1 write dbi
	.clk_drv_ohm = 40,
	.cs_drv_ohm = 40,
	.ac_drv_ohm = 40,
	.soc_data_drv_ohm_p = 40,
	.soc_data_drv_ohm_n = 40,
	.soc_data_odt_ohm_p = 0,
	.soc_data_odt_ohm_n = 40,
	.dram_data_drv_ohm = 40,        //48,
	.dram_data_odt_ohm = 60,        //60,
	.dram_ac_odt_ohm = 120,         //120,
	.soc_clk_slew_rate = 0x3ff,
	.soc_cs_slew_rate = 0x3ff,
	.soc_ac_slew_rate = 0x3ff,
	.soc_data_slew_rate = 0x2ff,
	.vref_output_permil = 0,
	.vref_receiver_permil = 0,
	.vref_dram_permil = 0,
	.lpddr4_dram_vout_voltage_1_3_2_5_setting = 1,

	.ac_trace_delay = { 32 + 0, 32 + 0, 32 + 0, 32 + 0, 32,
						32 + 0, 32 + 0, 32 + 0, 32 + 0, 32 + 0},
	.ddr_dmc_remap =
	{
	//[0] = (5 | 6 << 5 | 7 << 10 | 8 << 15 | 9 << 20 | 10 << 25),
	//[1] = (11 | 0 << 5 | 0 << 10 | 15 << 15 | 16 << 20 | 17 << 25),
	//[2] = (18 | 19 << 5 | 20 << 10 | 21 << 15 | 22 << 20 | 23 << 25),
	//[3] = (24 | 25 << 5 | 26 << 10 | 27 << 15 | 28 << 20 | 29 << 25),
	//[4] = (30 | 12 << 5 | 13 << 10 | 14 << 15 | 0 << 20 | 0 << 25),

	[0] = (0 | 5 << 5 | 6 << 10 | 7 << 15 | 8 << 20 | 9 << 25),
	[1] = (10 | 0 << 5 | 0 << 10 | 11 << 15 | 15 << 20 | 16 << 25),
	[2] = (17 | 18 << 5 | 19 << 10 | 20 << 15 | 21 << 20 | 22 << 25),
	[3] = (23 | 24 << 5 | 25 << 10 | 26 << 15 | 27 << 20 | 28 << 25),
	[4] = (29 | 12 << 5 | 13 << 10 | 14 << 15 | 0 << 20 | 0 << 25),
	},
	.ddr_lpddr34_ca_remap = {1, 0, 3, 2},
	.ac_pinmux = {1, 0, 2, 5, 4, 3, 6, 0, 0, 0, 0, 0,
				1, 4, 2, 5, 0, 3, 6},
	.ddr_lpddr34_dq_remap = {
		5, 4, 1, 2, 0, 3, 7, 6,
		8, 10, 9, 11, 13, 15, 14, 12,
		2 +  16, 3 + 16, 7 + 16, 6 + 16, 5 + 16, 4 + 16, 0 + 16, 1 + 16,
		11 + 16, 9 + 16, 10 + 16, 12 + 16, 8 + 16, 15 + 16, 13 + 16, 14 + 16},

	.dram_rtt_nom_wr_park = {00, 00},

	/* pll ssc config:
	 *
	 *   pll_ssc_mode = (1 << 20) | (1 << 8) | ([strength] << 4) | [mode],
	 *      ppm = strength * 500
	 *      mode: 0=center, 1=up, 2=down
	 *
	 *   eg:
	 *     1. config 1000ppm center ss. then mode=0, strength=2
	 *        .pll_ssc_mode = (1 << 20) | (1 << 8) | (2 << 4) | 0,
	 *     2. config 3000ppm down ss. then mode=2, strength=6
	 *        .pll_ssc_mode = (1 << 20) | (1 << 8) | (6 << 4) | 2,
	 */
	//.pll_ssc_mode = (1 << 20) | (1 << 8) | (2 << 4) | 0,//center_ssc_1000ppm
	.ddr_func = DDR_FUNC,
	.magic = DRAM_CFG_MAGIC,
	//{0x1,0x0},enable slt 4 DRAMFreq test;
	//{0x0,0x0},disable slt 4 DRAMFreq test;
	.slt_test_function = {0x0, 0x0},
	.fast_boot[0] = 0,
	},
	{
	//.fast_boot[0]=0x000000fd,// 253
	.board_id = CONFIG_BOARD_ID_MASK,
	.version = 1,
	//.dram_rank_config		= CONFIG_DDR0_32BIT_RANK0_CH0,
	.DramType = CONFIG_DDR_TYPE_LPDDR4,
	.enable_lpddr4x_mode = ENABLE_LPDDR4X_MODE,
	.DRAMFreq = { LPDDR4_DDR_CLK,		     792,	     0,	     0	    },
	.ddr_rfc_type = DDR_RFC_TYPE_LPDDR4_8Gbx1,
	.ddr_base_addr = CFG_DDR_BASE_ADDR,
	.ddr_start_offset = CFG_DDR_START_OFFSET,
	.DisabledDbyte[0] = 0xf0,//bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
	.DisabledDbyte[1] = 0xf0,//bit 0 -3 ch1 cs0 ,bit 4-7 ch1 cs1,
	.Is2Ttiming = 0,
	.HdtCtrl = 0xc8,
	//.dram_ch0_size_MB		= 0xffff,
	//.dram_ch1_size_MB		= 0,
#if (DDR_SIZE_TYPE_1 == DDR0_1G_DDR1_0G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_0G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_1G_DDR1_1G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_1G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_2G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_2G_RANK01)
	.DisabledDbyte[0] = 0x00,                               //bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
	.DisabledDbyte[1] = 0x00,                               //bit 0 -3 ch1 cs0 ,bit 4-7 ch1 cs1,
	.Is2Ttiming = 0,
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_4G_DDR1_4G_RANK01)
	.DisabledDbyte[0] = 0x00,                               //bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
	.DisabledDbyte[1] = 0x00,                               //bit 0 -3 ch1 cs0 ,bit 4-7 ch1 cs1,
	.Is2Ttiming = 0,
	//.HdtCtrl = 0x4,
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#endif
	.training_SequenceCtrl = { 0x131f,			  0x61 },//ddr3 0x21f 0x31f
	//use 0x23 0x13  compatibility with 1rank and 2rank
	//targeting rank 0. [3:0] is used
	//for write ODT [7:4] is used for //read ODT
	.phy_odt_config_rank = { 0x23,				0x13 },
	//use 0d0d compatibility with 1rank and 2rank  //0808
	.dfi_odt_config = 0x0d0d,
	.PllBypassEn = 0,         //bit0-ps0,bit1-ps1
	.ddr_rdbi_wr_enable = 0x0,//bit 0 read-dbi,bit 1 write dbi
	.clk_drv_ohm = 40,
	.cs_drv_ohm = 40,
	.ac_drv_ohm = 40,
	.soc_data_drv_ohm_p = 40,
	.soc_data_drv_ohm_n = 40,
	.soc_data_odt_ohm_p = 0,
	.soc_data_odt_ohm_n = 40,
	.dram_data_drv_ohm = 40,        //48,
	.dram_data_odt_ohm = 60,        //60,
	.dram_ac_odt_ohm = 120,         //120,
	.soc_clk_slew_rate = 0x3ff,
	.soc_cs_slew_rate = 0x3ff,
	.soc_ac_slew_rate = 0x3ff,
	.soc_data_slew_rate = 0x2ff,
	.vref_output_permil = 0,
	.vref_receiver_permil = 0,
	.vref_dram_permil = 0,
	.lpddr4_dram_vout_voltage_1_3_2_5_setting = 1,

	.ac_trace_delay = { 32 + 0, 32 + 0, 32 + 0, 32 + 0, 32,
						32 + 0, 32 + 0, 32 + 0, 32 + 0, 32 + 0},
	.ddr_dmc_remap =
	{
	//[0] = (5 | 6 << 5 | 7 << 10 | 8 << 15 | 9 << 20 | 10 << 25),
	//[1] = (11 | 0 << 5 | 0 << 10 | 15 << 15 | 16 << 20 | 17 << 25),
	//[2] = (18 | 19 << 5 | 20 << 10 | 21 << 15 | 22 << 20 | 23 << 25),
	//[3] = (24 | 25 << 5 | 26 << 10 | 27 << 15 | 28 << 20 | 29 << 25),
	//[4] = (30 | 12 << 5 | 13 << 10 | 14 << 15 | 0 << 20 | 0 << 25),

	[0] = (0 | 5 << 5 | 6 << 10 | 7 << 15 | 8 << 20 | 9 << 25),
	[1] = (10 | 0 << 5 | 0 << 10 | 11 << 15 | 15 << 20 | 16 << 25),
	[2] = (17 | 18 << 5 | 19 << 10 | 20 << 15 | 21 << 20 | 22 << 25),
	[3] = (23 | 24 << 5 | 25 << 10 | 26 << 15 | 27 << 20 | 28 << 25),
	[4] = (29 | 12 << 5 | 13 << 10 | 14 << 15 | 0 << 20 | 0 << 25),
	},
	.ddr_lpddr34_ca_remap = {0, 1, 2, 3},
	.ac_pinmux = {4, 1, 3, 5, 2, 0,  6, 0, 0, 0, 0, 0,
				3, 5, 4, 0, 2, 1, 6},
	.ddr_lpddr34_dq_remap = {
		7, 2, 1, 3, 4, 0, 5, 6,
		8, 10, 9, 11, 13, 15, 14, 12,
		0 + 16, 3 + 16, 4 + 16, 7 + 16, 6 + 16, 5 + 16, 2 + 16, 1 + 16, 12 + 16,
		15 + 16, 13 + 16, 14 + 16, 9 + 16, 10 + 16, 8 + 16, 11 + 16},
	.dram_rtt_nom_wr_park = {00, 00},

	/* pll ssc config:
	 *
	 *   pll_ssc_mode = (1 << 20) | (1 << 8) | ([strength] << 4) | [mode],
	 *      ppm = strength * 500
	 *      mode: 0=center, 1=up, 2=down
	 *
	 *   eg:
	 *     1. config 1000ppm center ss. then mode=0, strength=2
	 *        .pll_ssc_mode = (1 << 20) | (1 << 8) | (2 << 4) | 0,
	 *     2. config 3000ppm down ss. then mode=2, strength=6
	 *        .pll_ssc_mode = (1 << 20) | (1 << 8) | (6 << 4) | 2,
	 */
	//.pll_ssc_mode			= (1 << 20) | (1 << 8) | (2 << 4) | 0,//center_ssc_1000ppm
	.ddr_func = DDR_FUNC,
	.magic = DRAM_CFG_MAGIC,
	//{0x1,0x0},enable slt 4 DRAMFreq test;
	//{0x0,0x0},disable slt 4 DRAMFreq test;
	.slt_test_function = {0x0, 0x0},
	.fast_boot[0] = 0,
	},
};
#endif //end BOARD_ENABLE_LPDDR4

#if BOARD_ENABLE_LPDDR5

#ifdef CONFIG_PXP_DDR_FIX_2112_TIMMING
	#define   LPDDR5_DDR_DATA_RATE  792 * 8//6400
#else
	#define   LPDDR5_DDR_DATA_RATE  792 * 8//4000//6400/4000/4800//2800-7280
#endif

#define DDR_SIZE_TYPE_1         DDR0_2G_DDR1_2G//DDR0_4G_DDR1_4G_RANK01

ddr_set_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {
	{
	//.fast_boot[0]=0x000000fd,// 253
	.board_id = CONFIG_BOARD_ID_MASK,
	.version = 1,
	//.dram_rank_config		= CONFIG_DDR0_32BIT_RANK0_CH0,
	.DramType = CONFIG_DDR_TYPE_LPDDR5,
	.enable_lpddr4x_mode = 0,//ENABLE_LPDDR4X_MODE,
	.DRAMFreq = {LPDDR5_DDR_DATA_RATE / 8,		     0,	     0,	     0},
	.ddr_rfc_type = DDR_RFC_TYPE_LPDDR5_8Gbx1,
	.ddr_base_addr = CFG_DDR_BASE_ADDR,
	.ddr_start_offset = CFG_DDR_START_OFFSET,
	.DisabledDbyte[0] = 0xf0,//bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
	.DisabledDbyte[1] = 0xf0,//bit 0 -3 ch1 cs0 ,bit 4-7 ch1 cs1,
	.Is2Ttiming = 0,
	.HdtCtrl = 0xc8,
	//.dram_ch0_size_MB		= 0xffff,
	//.dram_ch1_size_MB		= 0,
#if (DDR_SIZE_TYPE_1 == DDR0_1G_DDR1_0G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_0G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_1G_DDR1_1G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_1G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_2G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_2G_RANK01)
	.DisabledDbyte[0] = 0x00,                               //bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
	.DisabledDbyte[1] = 0x00,                               //bit 0 -3 ch1 cs0 ,bit 4-7 ch1 cs1,
	.Is2Ttiming = 0,
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_4G_DDR1_4G_RANK01)
	.DisabledDbyte[0] = 0x00,                               //bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
	.DisabledDbyte[1] = 0x00,                               //bit 0 -3 ch1 cs0 ,bit 4-7 ch1 cs1,
	.Is2Ttiming = 0,
	//.HdtCtrl = 0x4,
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#endif
	.training_SequenceCtrl = { 0x175f,			  0x61 },//ddr3 0x21f 0x31f
	//use 0x23 0x13  compatibility with 1rank and 2rank
	//targeting rank 0. [3:0] is used
	//for write ODT [7:4] is used for //read ODT
	.phy_odt_config_rank = { 0x23,				0x13 },
	//use 0d0d compatibility with 1rank and 2rank  //0808
	.dfi_odt_config = 0x0d0d,
	.PllBypassEn = 0,         //bit0-ps0,bit1-ps1
	.ddr_rdbi_wr_enable = 0x3,//bit 0 read-dbi,bit 1 write dbi
	.clk_drv_ohm = 40,
	.cs_drv_ohm = 40,
	.ac_drv_ohm = 40,
	.soc_data_drv_ohm_p = 40,
	.soc_data_drv_ohm_n = 40,
	.soc_data_odt_ohm_p = 0,
	.soc_data_odt_ohm_n = 40,
	.dram_data_drv_ohm = 40,        //48,
	.dram_data_odt_ohm = 60,        //60,
	.dram_ac_odt_ohm = 240,         //120,

	.dac_offset = {
		(1 << 7) | (18),
		(0 << 7) | (12),
		(1 << 7) | (18),
		(0 << 7) | (2)
	},//bit 7 offset direction 0 ++  1 --
	.rx_offset = {
		(1 << 7) | (1),
		(1 << 7) | (1)
	},//bit 7 offset direction 0 ++  1 --
	.tx_offset = {
		(1 << 7) | (0),
		(1 << 7) | (0),
	},//bit 7 offset direction 0 ++  1 --
	.soc_clk_slew_rate = 0x3ff,
	.soc_cs_slew_rate = 0x3ff,
	.soc_ac_slew_rate = 0x3ff,
	.soc_data_slew_rate = 0x2ff,
	.vref_output_permil = 000,
	.vref_receiver_permil = 0,
	.vref_dram_permil = 0,
	.lpddr4_dram_vout_voltage_1_3_2_5_setting = 1,

	.ac_trace_delay = { 32 + 0, 32 + 0, 32 + 0, 32 + 0, 32,
						32 + 0, 32 + 0, 32 + 0, 32 + 0, 32 + 0},
	.ddr_dmc_remap = {
	//[0] = (5 | 6 << 5 | 7 << 10 | 8 << 15 | 9 << 20 | 10 << 25),
	//[1] = (11 | 0 << 5 | 0 << 10 | 15 << 15 | 16 << 20 | 17 << 25),
	//[2] = (18 | 19 << 5 | 20 << 10 | 21 << 15 | 22 << 20 | 23 << 25),
	//[3] = (24 | 25 << 5 | 26 << 10 | 27 << 15 | 28 << 20 | 29 << 25),
	//[4] = (30 | 12 << 5 | 13 << 10 | 14 << 15 | 0 << 20 | 0 << 25),

	[0] = (0  |  0 << 5 |  6 << 10 |  7 << 15 |  8 << 20 | 9 << 25),
	[1] = (10 | 11 << 5 |  0 << 10 | 15 << 15 | 16 << 20 | 17 << 25),
	[2] = (18 | 19 << 5 | 20 << 10 | 21 << 15 | 22 << 20 | 23 << 25),
	[3] = (24 | 25 << 5 | 26 << 10 | 27 << 15 | 28 << 20 | 29 << 25),
	[4] = (30 | 12 << 5 | 13 << 10 | 14 << 15 |  0 << 20 |  0 << 25),
	},
	.ddr_lpddr34_ca_remap = {1, 0, 2, 3},
	.ac_pinmux = {	6, 0, 2, 5, 3, 4, 1, 0, 0, 0, 0, 0,
					3, 0, 2, 5, 4, 6, 1, 0},
#ifdef BOARD_ENABLE_LPDDR5_RDL_MODIFY
	.ac_pinmux = {	1, 0, 2, 5, 3, 4, 6, 0, 0, 0, 0, 0,
					1, 0, 2, 5, 4, 6, 3, 0},
#endif
	.ddr_lpddr34_dq_remap = {1, 6, 2, 7, 4, 5, 3, 0,
							9, 10, 8, 11, 14, 15, 13, 12,
							5, 4, 7, 3, 0, 1, 2, 6,
							10, 9, 11, 12, 13, 15, 8, 14},

	.dram_rtt_nom_wr_park = {00, 00},

	/* pll ssc config:
	 *
	 *   pll_ssc_mode = (1 << 20) | (1 << 8) | ([strength] << 4) | [mode],
	 *      ppm = strength * 500
	 *      mode: 0=center, 1=up, 2=down
	 *
	 *   eg:
	 *     1. config 1000ppm center ss. then mode=0, strength=2
	 *        .pll_ssc_mode = (1 << 20) | (1 << 8) | (2 << 4) | 0,
	 *     2. config 3000ppm down ss. then mode=2, strength=6
	 *        .pll_ssc_mode = (1 << 20) | (1 << 8) | (6 << 4) | 2,
	 */
	//.pll_ssc_mode			= (1 << 20) | (1 << 8) | (2 << 4) | 0,//center_ssc_1000ppm
	.ddr_func =  (DDR_FUNC | DDR_FUNC_CONFIG_DFE_FUNCTION |
	DDR_FUNC_CONFIG_OFFSET_TRAINING_PER_DELAY |
	DDR_FUNC_CONFIG_RD_ECC_FUNCTION),
	.magic = DRAM_CFG_MAGIC,
	//{0x1,0x0},enable slt 4 DRAMFreq test;
	//{0x0,0x0},disable slt 4 DRAMFreq test;
	.slt_test_function = {0x0, 0x0},
	.fast_boot[0] = 0,
	},
	{
	//.fast_boot[0]=0x000000fd,// 253
	.board_id = CONFIG_BOARD_ID_MASK,
	.version = 1,
	//.dram_rank_config		= CONFIG_DDR0_32BIT_RANK0_CH0,
	.DramType = CONFIG_DDR_TYPE_LPDDR5,
	.enable_lpddr4x_mode = 0,// ENABLE_LPDDR4X_MODE,
	.DRAMFreq = {LPDDR5_DDR_DATA_RATE / 8,		     0,	     0,	     0},
	.ddr_rfc_type = DDR_RFC_TYPE_LPDDR5_8Gbx1,
	.ddr_base_addr = CFG_DDR_BASE_ADDR,
	.ddr_start_offset = CFG_DDR_START_OFFSET,
	.DisabledDbyte[0] = 0xf0,//bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
	.DisabledDbyte[1] = 0xf0,//bit 0 -3 ch1 cs0 ,bit 4-7 ch1 cs1,
	.Is2Ttiming = 0,
	.HdtCtrl = 0xc8,
	//.dram_ch0_size_MB		= 0xffff,
	//.dram_ch1_size_MB		= 0,
#if (DDR_SIZE_TYPE_1 == DDR0_1G_DDR1_0G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_0G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_1G_DDR1_1G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_1G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_2G)
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_2G_DDR1_2G_RANK01)
	.DisabledDbyte[0] = 0x00,                               //bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
	.DisabledDbyte[1] = 0x00,                               //bit 0 -3 ch1 cs0 ,bit 4-7 ch1 cs1,
	.Is2Ttiming = 0,
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX2 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#elif (DDR_SIZE_TYPE_1 == DDR0_4G_DDR1_4G_RANK01)
	.DisabledDbyte[0] = 0x00,                               //bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
	.DisabledDbyte[1] = 0x00,                               //bit 0 -3 ch1 cs0 ,bit 4-7 ch1 cs1,
	.Is2Ttiming = 0,
	//.HdtCtrl = 0x4,
	.dram_ch0_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
	.dram_ch1_size_MB =
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
#endif
	.training_SequenceCtrl = { 0x175f,			  0x61 },//ddr3 0x21f 0x31f
	//use 0x23 0x13  compatibility with 1rank and 2rank
	//targeting rank 0. [3:0] is used
	//for write ODT [7:4] is used for //read ODT
	.phy_odt_config_rank = { 0x23,				0x13 },
	//use 0d0d compatibility with 1rank and 2rank  //0808
	.dfi_odt_config = 0x0d0d,
	.PllBypassEn = 0,         //bit0-ps0,bit1-ps1
	.ddr_rdbi_wr_enable = 0x3,//bit 0 read-dbi,bit 1 write dbi
	.clk_drv_ohm = 40,
	.cs_drv_ohm = 40,
	.ac_drv_ohm = 40,
	.soc_data_drv_ohm_p = 40,
	.soc_data_drv_ohm_n = 40,
	.soc_data_odt_ohm_p = 0,
	.soc_data_odt_ohm_n = 40,
	.dram_data_drv_ohm = 40,        //48,
	.dram_data_odt_ohm = 60,        //60,
	.dram_ac_odt_ohm = 240,         //120,

	.dac_offset = {
		(1 << 7) | (18),
		(0 << 7) | (12),
		(1 << 7) | (18),
		(0 << 7) | (2)
	},//bit 7 offset direction 0 ++  1 --
	.rx_offset = {
		(1 << 7) | (1),
		(1 << 7) | (1)
	},//bit 7 offset direction 0 ++  1 --
	.tx_offset = {
		(1 << 7) | (0),
		(1 << 7) | (0),
	},//bit 7 offset direction 0 ++  1 --
	//.cfg_ddr_training_delay_ps[0].soc_bit_vref0[2] = (0 << 7) | (10),
	.soc_clk_slew_rate = 0x3ff,
	.soc_cs_slew_rate = 0x3ff,
	.soc_ac_slew_rate = 0x3ff,
	.soc_data_slew_rate = 0x2ff,
	.vref_output_permil = 000,
	.vref_receiver_permil = 0,
	.vref_dram_permil = 0,
	.lpddr4_dram_vout_voltage_1_3_2_5_setting = 1,

	.ac_trace_delay = { 32 + 0, 32 + 0, 32 + 0, 32 + 0, 32,
						32 + 0, 32 + 0, 32 + 0, 32 + 0, 32 + 0},
	.ddr_dmc_remap = {
	//[0] = (5 | 6 << 5 | 7 << 10 | 8 << 15 | 9 << 20 | 10 << 25),
	//[1] = (11 | 0 << 5 | 0 << 10 | 15 << 15 | 16 << 20 | 17 << 25),
	//[2] = (18 | 19 << 5 | 20 << 10 | 21 << 15 | 22 << 20 | 23 << 25),
	//[3] = (24 | 25 << 5 | 26 << 10 | 27 << 15 | 28 << 20 | 29 << 25),
	//[4] = (30 | 12 << 5 | 13 << 10 | 14 << 15 | 0 << 20 | 0 << 25),

	[0] = (0  |  0 << 5 |  6 << 10 |  7 << 15 |  8 << 20 | 9 << 25),
	[1] = (10 | 11 << 5 |  0 << 10 | 15 << 15 | 16 << 20 | 17 << 25),
	[2] = (18 | 19 << 5 | 20 << 10 | 21 << 15 | 22 << 20 | 23 << 25),
	[3] = (24 | 25 << 5 | 26 << 10 | 27 << 15 | 28 << 20 | 29 << 25),
	[4] = (30 | 12 << 5 | 13 << 10 | 14 << 15 |  0 << 20 |  0 << 25),
	},
	.ddr_lpddr34_ca_remap = {0, 1, 3, 2},
	.ac_pinmux = {6, 2, 1, 5, 0, 3, 4, 0, 0, 0, 0, 0, 4, 3, 1, 2, 0, 6, 5, 0},
#ifdef BOARD_ENABLE_LPDDR5_RDL_MODIFY
	.ac_pinmux = {4, 2, 1, 5, 0, 3, 6, 0, 0, 0, 0, 0, 5, 3, 1, 2, 0, 6, 4, 0},
#endif
	.ddr_lpddr34_dq_remap = {4, 7, 1, 2, 5, 6, 0, 3,
							10, 9, 11, 8, 12, 15, 13, 14,
							5, 4, 6, 3, 2, 1, 0, 7,
							15, 13, 14, 12, 8, 10, 9, 11},
#ifdef BOARD_ENABLE_LPDDR5_OPS_FT_PINMUX
	.ddr_lpddr34_dq_remap = {4, 7, 1, 2, 6, 5, 0, 3,
							10, 9, 11, 8, 12, 15, 13, 14,
							5, 4, 6, 3, 2, 1, 0, 7,
							15, 13, 14, 12, 8, 10, 9, 11},
#endif
	.dram_rtt_nom_wr_park = { 00,				 00 },

	/* pll ssc config:
	 *
	 *   pll_ssc_mode = (1 << 20) | (1 << 8) | ([strength] << 4) | [mode],
	 *      ppm = strength * 500
	 *      mode: 0=center, 1=up, 2=down
	 *
	 *   eg:
	 *     1. config 1000ppm center ss. then mode=0, strength=2
	 *        .pll_ssc_mode = (1 << 20) | (1 << 8) | (2 << 4) | 0,
	 *     2. config 3000ppm down ss. then mode=2, strength=6
	 *        .pll_ssc_mode = (1 << 20) | (1 << 8) | (6 << 4) | 2,
	 */
	//.pll_ssc_mode			= (1 << 20) | (1 << 8) | (2 << 4) | 0,//center_ssc_1000ppm
	.ddr_func =  (DDR_FUNC | DDR_FUNC_CONFIG_DFE_FUNCTION |
	DDR_FUNC_CONFIG_OFFSET_TRAINING_PER_DELAY |
	DDR_FUNC_CONFIG_RD_ECC_FUNCTION),
	.magic = DRAM_CFG_MAGIC,
	//{0x1,0x0},enable slt 4 DRAMFreq test;
	//{0x0,0x0},disable slt 4 DRAMFreq test;
	.slt_test_function = {0x0, 0x0},
	.fast_boot[0] = 0,
	},
};
#endif //end BOARD_ENABLE_LPDDR5

board_clk_set_t __board_clk_setting
__attribute__ ((section(".clk_param"))) = {
	/* clock settings for bl2 */
	.cpu_clk	= CPU_CLK / 24 * 24,
#ifdef CONFIG_PXP_EMULATOR
	.pxp		= 1,
#else
	.pxp		= 0,
#endif
	.low_console_baud = CONFIG_LOW_CONSOLE_BAUD,
};


#define VCCK_A_VAL				AML_VCCK_A_INIT_VOLTAGE
#define VCCK_B_VAL				AML_VCCK_B_INIT_VOLTAGE
#define VDD_CPUA_VAL			AML_VDD_CPUA_INIT_VOLTAGE
#define VDD_CPUB_VAL			AML_VDD_CPUB_INIT_VOLTAGE

/* VCCK_A PWM table */
#if   (VCCK_A_VAL == 1039)
	#define VCCK_A_VAL_REG	0x00000022
#elif (VCCK_A_VAL == 1029)
	#define VCCK_A_VAL_REG	0x00010021
#elif (VCCK_A_VAL == 1019)
	#define VCCK_A_VAL_REG	0x00020020
#elif (VCCK_A_VAL == 1009)
	#define VCCK_A_VAL_REG	0x0003001f
#elif (VCCK_A_VAL == 999)
	#define VCCK_A_VAL_REG	0x0004001e
#elif (VCCK_A_VAL == 989)
	#define VCCK_A_VAL_REG	0x0005001d
#elif (VCCK_A_VAL == 979)
	#define VCCK_A_VAL_REG	0x0006001c
#elif (VCCK_A_VAL == 969)
	#define VCCK_A_VAL_REG	0x0007001b
#elif (VCCK_A_VAL == 959)
	#define VCCK_A_VAL_REG	0x0008001a
#elif (VCCK_A_VAL == 949)
	#define VCCK_A_VAL_REG	0x00090019
#elif (VCCK_A_VAL == 939)
	#define VCCK_A_VAL_REG	0x000a0018
#elif (VCCK_A_VAL == 929)
	#define VCCK_A_VAL_REG	0x000b0017
#elif (VCCK_A_VAL == 919)
	#define VCCK_A_VAL_REG	0x000c0016
#elif (VCCK_A_VAL == 909)
	#define VCCK_A_VAL_REG	0x000d0015
#elif (VCCK_A_VAL == 899)
	#define VCCK_A_VAL_REG	0x000e0014
#elif (VCCK_A_VAL == 889)
	#define VCCK_A_VAL_REG	0x000f0013
#elif (VCCK_A_VAL == 879)
	#define VCCK_A_VAL_REG	0x00100012
#elif (VCCK_A_VAL == 869)
	#define VCCK_A_VAL_REG	0x00110011
#elif (VCCK_A_VAL == 859)
	#define VCCK_A_VAL_REG	0x00120010
#elif (VCCK_A_VAL == 849)
	#define VCCK_A_VAL_REG	0x0013000f
#elif (VCCK_A_VAL == 839)
	#define VCCK_A_VAL_REG	0x0014000e
#elif (VCCK_A_VAL == 829)
	#define VCCK_A_VAL_REG	0x0015000d
#elif (VCCK_A_VAL == 819)
	#define VCCK_A_VAL_REG	0x0016000c
#elif (VCCK_A_VAL == 809)
	#define VCCK_A_VAL_REG	0x0017000b
#elif (VCCK_A_VAL == 799)
	#define VCCK_A_VAL_REG	0x0018000a
#elif (VCCK_A_VAL == 789)
	#define VCCK_A_VAL_REG	0x00190009
#elif (VCCK_A_VAL == 779)
	#define VCCK_A_VAL_REG	0x001a0008
#elif (VCCK_A_VAL == 769)
	#define VCCK_A_VAL_REG	0x001b0007
#elif (VCCK_A_VAL == 759)
	#define VCCK_A_VAL_REG	0x001c0006
#elif (VCCK_A_VAL == 749)
	#define VCCK_A_VAL_REG	0x001d0005
#elif (VCCK_A_VAL == 739)
	#define VCCK_A_VAL_REG	0x001e0004
#elif (VCCK_A_VAL == 729)
	#define VCCK_A_VAL_REG	0x001f0003
#elif (VCCK_A_VAL == 719)
	#define VCCK_A_VAL_REG	0x00200002
#elif (VCCK_A_VAL == 709)
	#define VCCK_A_VAL_REG	0x00210001
#elif (VCCK_A_VAL == 699)
	#define VCCK_A_VAL_REG	0x00220000
#else
	#error "VCCK_A val out of range\n"
#endif

/* VCCK_B PWM table */
#if   (VCCK_B_VAL == 1039)
	#define VCCK_B_VAL_REG	0x00000022
#elif (VCCK_B_VAL == 1029)
	#define VCCK_B_VAL_REG	0x00010021
#elif (VCCK_B_VAL == 1019)
	#define VCCK_B_VAL_REG	0x00020020
#elif (VCCK_B_VAL == 1009)
	#define VCCK_B_VAL_REG	0x0003001f
#elif (VCCK_B_VAL == 999)
	#define VCCK_B_VAL_REG	0x0004001e
#elif (VCCK_B_VAL == 989)
	#define VCCK_B_VAL_REG	0x0005001d
#elif (VCCK_B_VAL == 979)
	#define VCCK_B_VAL_REG	0x0006001c
#elif (VCCK_B_VAL == 969)
	#define VCCK_B_VAL_REG	0x0007001b
#elif (VCCK_B_VAL == 959)
	#define VCCK_B_VAL_REG	0x0008001a
#elif (VCCK_B_VAL == 949)
	#define VCCK_B_VAL_REG	0x00090019
#elif (VCCK_B_VAL == 939)
	#define VCCK_B_VAL_REG	0x000a0018
#elif (VCCK_B_VAL == 929)
	#define VCCK_B_VAL_REG	0x000b0017
#elif (VCCK_B_VAL == 919)
	#define VCCK_B_VAL_REG	0x000c0016
#elif (VCCK_B_VAL == 909)
	#define VCCK_B_VAL_REG	0x000d0015
#elif (VCCK_B_VAL == 899)
	#define VCCK_B_VAL_REG	0x000e0014
#elif (VCCK_B_VAL == 889)
	#define VCCK_B_VAL_REG	0x000f0013
#elif (VCCK_B_VAL == 879)
	#define VCCK_B_VAL_REG	0x00100012
#elif (VCCK_B_VAL == 869)
	#define VCCK_B_VAL_REG	0x00110011
#elif (VCCK_B_VAL == 859)
	#define VCCK_B_VAL_REG	0x00120010
#elif (VCCK_B_VAL == 849)
	#define VCCK_B_VAL_REG	0x0013000f
#elif (VCCK_B_VAL == 839)
	#define VCCK_B_VAL_REG	0x0014000e
#elif (VCCK_B_VAL == 829)
	#define VCCK_B_VAL_REG	0x0015000d
#elif (VCCK_B_VAL == 819)
	#define VCCK_B_VAL_REG	0x0016000c
#elif (VCCK_B_VAL == 809)
	#define VCCK_B_VAL_REG	0x0017000b
#elif (VCCK_B_VAL == 799)
	#define VCCK_B_VAL_REG	0x0018000a
#elif (VCCK_B_VAL == 789)
	#define VCCK_B_VAL_REG	0x00190009
#elif (VCCK_B_VAL == 779)
	#define VCCK_B_VAL_REG	0x001a0008
#elif (VCCK_B_VAL == 769)
	#define VCCK_B_VAL_REG	0x001b0007
#elif (VCCK_B_VAL == 759)
	#define VCCK_B_VAL_REG	0x001c0006
#elif (VCCK_B_VAL == 749)
	#define VCCK_B_VAL_REG	0x001d0005
#elif (VCCK_B_VAL == 739)
	#define VCCK_B_VAL_REG	0x001e0004
#elif (VCCK_B_VAL == 729)
	#define VCCK_B_VAL_REG	0x001f0003
#elif (VCCK_B_VAL == 719)
	#define VCCK_B_VAL_REG	0x00200002
#elif (VCCK_B_VAL == 709)
	#define VCCK_B_VAL_REG	0x00210001
#elif (VCCK_B_VAL == 699)
	#define VCCK_B_VAL_REG	0x00220000
#else
	#error "VCCK_B val out of range\n"
#endif

/* VDD_CPUA_REG */
#if    (VDD_CPUA_VAL == 500)
#define VDD_CPUA_REG   0x01
#elif (VDD_CPUA_VAL == 505)
#define VDD_CPUA_REG   0x02
#elif (VDD_CPUA_VAL == 510)
#define VDD_CPUA_REG   0x03
#elif (VDD_CPUA_VAL == 515)
#define VDD_CPUA_REG   0x04
#elif (VDD_CPUA_VAL == 520)
#define VDD_CPUA_REG   0x05
#elif (VDD_CPUA_VAL == 525)
#define VDD_CPUA_REG   0x06
#elif (VDD_CPUA_VAL == 530)
#define VDD_CPUA_REG   0x07
#elif (VDD_CPUA_VAL == 535)
#define VDD_CPUA_REG   0x08
#elif (VDD_CPUA_VAL == 540)
#define VDD_CPUA_REG   0x09
#elif (VDD_CPUA_VAL == 545)
#define VDD_CPUA_REG   0x0a
#elif (VDD_CPUA_VAL == 550)
#define VDD_CPUA_REG   0x0b
#elif (VDD_CPUA_VAL == 555)
#define VDD_CPUA_REG   0x0c
#elif (VDD_CPUA_VAL == 560)
#define VDD_CPUA_REG   0x0d
#elif (VDD_CPUA_VAL == 565)
#define VDD_CPUA_REG   0x0e
#elif (VDD_CPUA_VAL == 570)
#define VDD_CPUA_REG   0x0f
#elif (VDD_CPUA_VAL == 575)
#define VDD_CPUA_REG   0x10
#elif (VDD_CPUA_VAL == 580)
#define VDD_CPUA_REG   0x11
#elif (VDD_CPUA_VAL == 585)
#define VDD_CPUA_REG   0x12
#elif (VDD_CPUA_VAL == 590)
#define VDD_CPUA_REG   0x13
#elif (VDD_CPUA_VAL == 595)
#define VDD_CPUA_REG   0x14
#elif (VDD_CPUA_VAL == 600)
#define VDD_CPUA_REG   0x15
#elif (VDD_CPUA_VAL == 605)
#define VDD_CPUA_REG   0x16
#elif (VDD_CPUA_VAL == 610)
#define VDD_CPUA_REG   0x17
#elif (VDD_CPUA_VAL == 615)
#define VDD_CPUA_REG   0x18
#elif (VDD_CPUA_VAL == 620)
#define VDD_CPUA_REG   0x19
#elif (VDD_CPUA_VAL == 625)
#define VDD_CPUA_REG   0x1a
#elif (VDD_CPUA_VAL == 630)
#define VDD_CPUA_REG   0x1b
#elif (VDD_CPUA_VAL == 635)
#define VDD_CPUA_REG   0x1c
#elif (VDD_CPUA_VAL == 640)
#define VDD_CPUA_REG   0x1d
#elif (VDD_CPUA_VAL == 645)
#define VDD_CPUA_REG   0x1e
#elif (VDD_CPUA_VAL == 650)
#define VDD_CPUA_REG   0x1f
#elif (VDD_CPUA_VAL == 655)
#define VDD_CPUA_REG   0x20
#elif (VDD_CPUA_VAL == 660)
#define VDD_CPUA_REG   0x21
#elif (VDD_CPUA_VAL == 665)
#define VDD_CPUA_REG   0x22
#elif (VDD_CPUA_VAL == 670)
#define VDD_CPUA_REG   0x23
#elif (VDD_CPUA_VAL == 675)
#define VDD_CPUA_REG   0x24
#elif (VDD_CPUA_VAL == 680)
#define VDD_CPUA_REG   0x25
#elif (VDD_CPUA_VAL == 685)
#define VDD_CPUA_REG   0x26
#elif (VDD_CPUA_VAL == 690)
#define VDD_CPUA_REG   0x27
#elif (VDD_CPUA_VAL == 695)
#define VDD_CPUA_REG   0x28
#elif (VDD_CPUA_VAL == 700)
#define VDD_CPUA_REG   0x29
#elif (VDD_CPUA_VAL == 705)
#define VDD_CPUA_REG   0x2a
#elif (VDD_CPUA_VAL == 710)
#define VDD_CPUA_REG   0x2b
#elif (VDD_CPUA_VAL == 715)
#define VDD_CPUA_REG   0x2c
#elif (VDD_CPUA_VAL == 720)
#define VDD_CPUA_REG   0x2d
#elif (VDD_CPUA_VAL == 725)
#define VDD_CPUA_REG   0x2e
#elif (VDD_CPUA_VAL == 730)
#define VDD_CPUA_REG   0x2f
#elif (VDD_CPUA_VAL == 735)
#define VDD_CPUA_REG   0x30
#elif (VDD_CPUA_VAL == 740)
#define VDD_CPUA_REG   0x31
#elif (VDD_CPUA_VAL == 745)
#define VDD_CPUA_REG   0x32
#elif (VDD_CPUA_VAL == 750)
#define VDD_CPUA_REG   0x33
#elif (VDD_CPUA_VAL == 755)
#define VDD_CPUA_REG   0x34
#elif (VDD_CPUA_VAL == 760)
#define VDD_CPUA_REG   0x35
#elif (VDD_CPUA_VAL == 765)
#define VDD_CPUA_REG   0x36
#elif (VDD_CPUA_VAL == 770)
#define VDD_CPUA_REG   0x37
#elif (VDD_CPUA_VAL == 775)
#define VDD_CPUA_REG   0x38
#elif (VDD_CPUA_VAL == 780)
#define VDD_CPUA_REG   0x39
#elif (VDD_CPUA_VAL == 785)
#define VDD_CPUA_REG   0x3a
#elif (VDD_CPUA_VAL == 790)
#define VDD_CPUA_REG   0x3b
#elif (VDD_CPUA_VAL == 795)
#define VDD_CPUA_REG   0x3c
#elif (VDD_CPUA_VAL == 800)
#define VDD_CPUA_REG   0x3d
#elif (VDD_CPUA_VAL == 805)
#define VDD_CPUA_REG   0x3e
#elif (VDD_CPUA_VAL == 810)
#define VDD_CPUA_REG   0x3f
#elif (VDD_CPUA_VAL == 815)
#define VDD_CPUA_REG   0x40
#elif (VDD_CPUA_VAL == 820)
#define VDD_CPUA_REG   0x41
#elif (VDD_CPUA_VAL == 825)
#define VDD_CPUA_REG   0x42
#elif (VDD_CPUA_VAL == 830)
#define VDD_CPUA_REG   0x43
#elif (VDD_CPUA_VAL == 835)
#define VDD_CPUA_REG   0x44
#elif (VDD_CPUA_VAL == 840)
#define VDD_CPUA_REG   0x45
#elif (VDD_CPUA_VAL == 845)
#define VDD_CPUA_REG   0x46
#elif (VDD_CPUA_VAL == 850)
#define VDD_CPUA_REG   0x47
#elif (VDD_CPUA_VAL == 855)
#define VDD_CPUA_REG   0x48
#elif (VDD_CPUA_VAL == 860)
#define VDD_CPUA_REG   0x49
#elif (VDD_CPUA_VAL == 865)
#define VDD_CPUA_REG   0x4a
#elif (VDD_CPUA_VAL == 870)
#define VDD_CPUA_REG   0x4b
#elif (VDD_CPUA_VAL == 875)
#define VDD_CPUA_REG   0x4c
#elif (VDD_CPUA_VAL == 880)
#define VDD_CPUA_REG   0x4d
#elif (VDD_CPUA_VAL == 885)
#define VDD_CPUA_REG   0x4e
#elif (VDD_CPUA_VAL == 890)
#define VDD_CPUA_REG   0x4f
#elif (VDD_CPUA_VAL == 895)
#define VDD_CPUA_REG   0x50
#elif (VDD_CPUA_VAL == 900)
#define VDD_CPUA_REG   0x51
#elif (VDD_CPUA_VAL == 905)
#define VDD_CPUA_REG   0x52
#elif (VDD_CPUA_VAL == 910)
#define VDD_CPUA_REG   0x53
#elif (VDD_CPUA_VAL == 915)
#define VDD_CPUA_REG   0x54
#elif (VDD_CPUA_VAL == 920)
#define VDD_CPUA_REG   0x55
#elif (VDD_CPUA_VAL == 925)
#define VDD_CPUA_REG   0x56
#elif (VDD_CPUA_VAL == 930)
#define VDD_CPUA_REG   0x57
#elif (VDD_CPUA_VAL == 935)
#define VDD_CPUA_REG   0x58
#elif (VDD_CPUA_VAL == 940)
#define VDD_CPUA_REG   0x59
#elif (VDD_CPUA_VAL == 945)
#define VDD_CPUA_REG   0x5a
#elif (VDD_CPUA_VAL == 950)
#define VDD_CPUA_REG   0x5b
#elif (VDD_CPUA_VAL == 955)
#define VDD_CPUA_REG   0x5c
#elif (VDD_CPUA_VAL == 960)
#define VDD_CPUA_REG   0x5d
#elif (VDD_CPUA_VAL == 965)
#define VDD_CPUA_REG   0x5e
#elif (VDD_CPUA_VAL == 970)
#define VDD_CPUA_REG   0x5f
#elif (VDD_CPUA_VAL == 975)
#define VDD_CPUA_REG   0x60
#elif (VDD_CPUA_VAL == 980)
#define VDD_CPUA_REG   0x61
#elif (VDD_CPUA_VAL == 985)
#define VDD_CPUA_REG   0x62
#elif (VDD_CPUA_VAL == 990)
#define VDD_CPUA_REG   0x63
#elif (VDD_CPUA_VAL == 995)
#define VDD_CPUA_REG   0x64
#elif (VDD_CPUA_VAL == 1000)
#define VDD_CPUA_REG   0x65
#elif (VDD_CPUA_VAL == 1005)
#define VDD_CPUA_REG   0x66
#elif (VDD_CPUA_VAL == 1010)
#define VDD_CPUA_REG   0x67
#elif (VDD_CPUA_VAL == 1015)
#define VDD_CPUA_REG   0x68
#elif (VDD_CPUA_VAL == 1020)
#define VDD_CPUA_REG   0x69
#elif (VDD_CPUA_VAL == 1025)
#define VDD_CPUA_REG   0x6a
#elif (VDD_CPUA_VAL == 1030)
#define VDD_CPUA_REG   0x6b
#elif (VDD_CPUA_VAL == 1035)
#define VDD_CPUA_REG   0x6c
#elif (VDD_CPUA_VAL == 1040)
#define VDD_CPUA_REG   0x6d
#elif (VDD_CPUA_VAL == 1045)
#define VDD_CPUA_REG   0x6e
#elif (VDD_CPUA_VAL == 1050)
#define VDD_CPUA_REG   0x6f
#elif (VDD_CPUA_VAL == 1055)
#define VDD_CPUA_REG   0x70
#elif (VDD_CPUA_VAL == 1060)
#define VDD_CPUA_REG   0x71
#elif (VDD_CPUA_VAL == 1065)
#define VDD_CPUA_REG   0x72
#elif (VDD_CPUA_VAL == 1070)
#define VDD_CPUA_REG   0x73
#elif (VDD_CPUA_VAL == 1075)
#define VDD_CPUA_REG   0x74
#elif (VDD_CPUA_VAL == 1080)
#define VDD_CPUA_REG   0x75
#elif (VDD_CPUA_VAL == 1085)
#define VDD_CPUA_REG   0x76
#elif (VDD_CPUA_VAL == 1090)
#define VDD_CPUA_REG   0x77
#elif (VDD_CPUA_VAL == 1095)
#define VDD_CPUA_REG   0x78
#elif (VDD_CPUA_VAL == 1100)
#define VDD_CPUA_REG   0x79
#elif (VDD_CPUA_VAL == 1105)
#define VDD_CPUA_REG   0x7a
#elif (VDD_CPUA_VAL == 1110)
#define VDD_CPUA_REG   0x7b
#elif (VDD_CPUA_VAL == 1115)
#define VDD_CPUA_REG   0x7c
#elif (VDD_CPUA_VAL == 1120)
#define VDD_CPUA_REG   0x7d
#elif (VDD_CPUA_VAL == 1125)
#define VDD_CPUA_REG   0x7e
#elif (VDD_CPUA_VAL == 1130)
#define VDD_CPUA_REG   0x7f
#elif (VDD_CPUA_VAL == 1135)
#define VDD_CPUA_REG   0x80
#elif (VDD_CPUA_VAL == 1140)
#define VDD_CPUA_REG   0x81
#elif (VDD_CPUA_VAL == 1145)
#define VDD_CPUA_REG   0x82
#elif (VDD_CPUA_VAL == 1150)
#define VDD_CPUA_REG   0x83
#elif (VDD_CPUA_VAL == 1155)
#define VDD_CPUA_REG   0x84
#elif (VDD_CPUA_VAL == 1160)
#define VDD_CPUA_REG   0x85
#elif (VDD_CPUA_VAL == 1165)
#define VDD_CPUA_REG   0x86
#elif (VDD_CPUA_VAL == 1170)
#define VDD_CPUA_REG   0x87
#elif (VDD_CPUA_VAL == 1175)
#define VDD_CPUA_REG   0x88
#elif (VDD_CPUA_VAL == 1180)
#define VDD_CPUA_REG   0x89
#elif (VDD_CPUA_VAL == 1185)
#define VDD_CPUA_REG   0x8a
#elif (VDD_CPUA_VAL == 1190)
#define VDD_CPUA_REG   0x8b
#elif (VDD_CPUA_VAL == 1195)
#define VDD_CPUA_REG   0x8c
#elif (VDD_CPUA_VAL == 1200)
#define VDD_CPUA_REG   0x8d
#elif (VDD_CPUA_VAL == 1205)
#define VDD_CPUA_REG   0x8e
#elif (VDD_CPUA_VAL == 1210)
#define VDD_CPUA_REG   0x8f
#elif (VDD_CPUA_VAL == 1215)
#define VDD_CPUA_REG   0x90
#elif (VDD_CPUA_VAL == 1220)
#define VDD_CPUA_REG   0x91
#elif (VDD_CPUA_VAL == 1225)
#define VDD_CPUA_REG   0x92
#elif (VDD_CPUA_VAL == 1230)
#define VDD_CPUA_REG   0x93
#elif (VDD_CPUA_VAL == 1235)
#define VDD_CPUA_REG   0x94
#elif (VDD_CPUA_VAL == 1240)
#define VDD_CPUA_REG   0x95
#elif (VDD_CPUA_VAL == 1245)
#define VDD_CPUA_REG   0x96
#elif (VDD_CPUA_VAL == 1250)
#define VDD_CPUA_REG   0x97
#elif (VDD_CPUA_VAL == 1255)
#define VDD_CPUA_REG   0x98
#elif (VDD_CPUA_VAL == 1260)
#define VDD_CPUA_REG   0x99
#elif (VDD_CPUA_VAL == 1265)
#define VDD_CPUA_REG   0x9a
#elif (VDD_CPUA_VAL == 1270)
#define VDD_CPUA_REG   0x9b
#elif (VDD_CPUA_VAL == 1275)
#define VDD_CPUA_REG   0x9c
#elif (VDD_CPUA_VAL == 1280)
#define VDD_CPUA_REG   0x9d
#elif (VDD_CPUA_VAL == 1285)
#define VDD_CPUA_REG   0x9e
#elif (VDD_CPUA_VAL == 1290)
#define VDD_CPUA_REG   0x9f
#elif (VDD_CPUA_VAL == 1295)
#define VDD_CPUA_REG   0xa0
#elif (VDD_CPUA_VAL == 1300)
#define VDD_CPUA_REG   0xa1
#elif (VDD_CPUA_VAL == 1305)
#define VDD_CPUA_REG   0xa2
#elif (VDD_CPUA_VAL == 1310)
#define VDD_CPUA_REG   0xa3
#elif (VDD_CPUA_VAL == 1315)
#define VDD_CPUA_REG   0xa4
#elif (VDD_CPUA_VAL == 1320)
#define VDD_CPUA_REG   0xa5
#elif (VDD_CPUA_VAL == 1325)
#define VDD_CPUA_REG   0xa6
#elif (VDD_CPUA_VAL == 1330)
#define VDD_CPUA_REG   0xa7
#elif (VDD_CPUA_VAL == 1335)
#define VDD_CPUA_REG   0xa8
#elif (VDD_CPUA_VAL == 1340)
#define VDD_CPUA_REG   0xa9
#elif (VDD_CPUA_VAL == 1345)
#define VDD_CPUA_REG   0xaa
#elif (VDD_CPUA_VAL == 1350)
#define VDD_CPUA_REG   0xab
#else
#error "VDD_CPUA val out of range\n"
#endif

/* VDD_CPUB_REG */
#if    (VDD_CPUB_VAL == 500)
#define VDD_CPUB_REG   0x01
#elif (VDD_CPUB_VAL == 505)
#define VDD_CPUB_REG   0x02
#elif (VDD_CPUB_VAL == 510)
#define VDD_CPUB_REG   0x03
#elif (VDD_CPUB_VAL == 515)
#define VDD_CPUB_REG   0x04
#elif (VDD_CPUB_VAL == 520)
#define VDD_CPUB_REG   0x05
#elif (VDD_CPUB_VAL == 525)
#define VDD_CPUB_REG   0x06
#elif (VDD_CPUB_VAL == 530)
#define VDD_CPUB_REG   0x07
#elif (VDD_CPUB_VAL == 535)
#define VDD_CPUB_REG   0x08
#elif (VDD_CPUB_VAL == 540)
#define VDD_CPUB_REG   0x09
#elif (VDD_CPUB_VAL == 545)
#define VDD_CPUB_REG   0x0a
#elif (VDD_CPUB_VAL == 550)
#define VDD_CPUB_REG   0x0b
#elif (VDD_CPUB_VAL == 555)
#define VDD_CPUB_REG   0x0c
#elif (VDD_CPUB_VAL == 560)
#define VDD_CPUB_REG   0x0d
#elif (VDD_CPUB_VAL == 565)
#define VDD_CPUB_REG   0x0e
#elif (VDD_CPUB_VAL == 570)
#define VDD_CPUB_REG   0x0f
#elif (VDD_CPUB_VAL == 575)
#define VDD_CPUB_REG   0x10
#elif (VDD_CPUB_VAL == 580)
#define VDD_CPUB_REG   0x11
#elif (VDD_CPUB_VAL == 585)
#define VDD_CPUB_REG   0x12
#elif (VDD_CPUB_VAL == 590)
#define VDD_CPUB_REG   0x13
#elif (VDD_CPUB_VAL == 595)
#define VDD_CPUB_REG   0x14
#elif (VDD_CPUB_VAL == 600)
#define VDD_CPUB_REG   0x15
#elif (VDD_CPUB_VAL == 605)
#define VDD_CPUB_REG   0x16
#elif (VDD_CPUB_VAL == 610)
#define VDD_CPUB_REG   0x17
#elif (VDD_CPUB_VAL == 615)
#define VDD_CPUB_REG   0x18
#elif (VDD_CPUB_VAL == 620)
#define VDD_CPUB_REG   0x19
#elif (VDD_CPUB_VAL == 625)
#define VDD_CPUB_REG   0x1a
#elif (VDD_CPUB_VAL == 630)
#define VDD_CPUB_REG   0x1b
#elif (VDD_CPUB_VAL == 635)
#define VDD_CPUB_REG   0x1c
#elif (VDD_CPUB_VAL == 640)
#define VDD_CPUB_REG   0x1d
#elif (VDD_CPUB_VAL == 645)
#define VDD_CPUB_REG   0x1e
#elif (VDD_CPUB_VAL == 650)
#define VDD_CPUB_REG   0x1f
#elif (VDD_CPUB_VAL == 655)
#define VDD_CPUB_REG   0x20
#elif (VDD_CPUB_VAL == 660)
#define VDD_CPUB_REG   0x21
#elif (VDD_CPUB_VAL == 665)
#define VDD_CPUB_REG   0x22
#elif (VDD_CPUB_VAL == 670)
#define VDD_CPUB_REG   0x23
#elif (VDD_CPUB_VAL == 880)
#define VDD_CPUB_REG   0x4d
#elif (VDD_CPUB_VAL == 885)
#define VDD_CPUB_REG   0x4e
#elif (VDD_CPUB_VAL == 890)
#define VDD_CPUB_REG   0x4f
#elif (VDD_CPUB_VAL == 895)
#define VDD_CPUB_REG   0x50
#elif (VDD_CPUB_VAL == 900)
#define VDD_CPUB_REG   0x51
#elif (VDD_CPUB_VAL == 905)
#define VDD_CPUB_REG   0x52
#elif (VDD_CPUB_VAL == 910)
#define VDD_CPUB_REG   0x53
#elif (VDD_CPUB_VAL == 915)
#define VDD_CPUB_REG   0x54
#elif (VDD_CPUB_VAL == 920)
#define VDD_CPUB_REG   0x55
#elif (VDD_CPUB_VAL == 925)
#define VDD_CPUB_REG   0x56
#elif (VDD_CPUB_VAL == 930)
#define VDD_CPUB_REG   0x57
#elif (VDD_CPUB_VAL == 935)
#define VDD_CPUB_REG   0x58
#elif (VDD_CPUB_VAL == 940)
#define VDD_CPUB_REG   0x59
#elif (VDD_CPUB_VAL == 945)
#define VDD_CPUB_REG   0x5a
#elif (VDD_CPUB_VAL == 950)
#define VDD_CPUB_REG   0x5b
#elif (VDD_CPUB_VAL == 955)
#define VDD_CPUB_REG   0x5c
#elif (VDD_CPUB_VAL == 960)
#define VDD_CPUB_REG   0x5d
#elif (VDD_CPUB_VAL == 965)
#define VDD_CPUB_REG   0x5e
#elif (VDD_CPUB_VAL == 970)
#define VDD_CPUB_REG   0x5f
#elif (VDD_CPUB_VAL == 975)
#define VDD_CPUB_REG   0x60
#elif (VDD_CPUB_VAL == 980)
#define VDD_CPUB_REG   0x61
#elif (VDD_CPUB_VAL == 985)
#define VDD_CPUB_REG   0x62
#elif (VDD_CPUB_VAL == 990)
#define VDD_CPUB_REG   0x63
#elif (VDD_CPUB_VAL == 995)
#define VDD_CPUB_REG   0x64
#elif (VDD_CPUB_VAL == 1000)
#define VDD_CPUB_REG   0x65
#elif (VDD_CPUB_VAL == 1005)
#define VDD_CPUB_REG   0x66
#elif (VDD_CPUB_VAL == 1010)
#define VDD_CPUB_REG   0x67
#elif (VDD_CPUB_VAL == 1015)
#define VDD_CPUB_REG   0x68
#elif (VDD_CPUB_VAL == 1020)
#define VDD_CPUB_REG   0x69
#elif (VDD_CPUB_VAL == 1025)
#define VDD_CPUB_REG   0x6a
#elif (VDD_CPUB_VAL == 1030)
#define VDD_CPUB_REG   0x6b
#elif (VDD_CPUB_VAL == 1035)
#define VDD_CPUB_REG   0x6c
#elif (VDD_CPUB_VAL == 1040)
#define VDD_CPUB_REG   0x6d
#elif (VDD_CPUB_VAL == 1045)
#define VDD_CPUB_REG   0x6e
#elif (VDD_CPUB_VAL == 1050)
#define VDD_CPUB_REG   0x6f
#elif (VDD_CPUB_VAL == 1055)
#define VDD_CPUB_REG   0x70
#elif (VDD_CPUB_VAL == 1060)
#define VDD_CPUB_REG   0x71
#elif (VDD_CPUB_VAL == 1065)
#define VDD_CPUB_REG   0x72
#elif (VDD_CPUB_VAL == 1070)
#define VDD_CPUB_REG   0x73
#elif (VDD_CPUB_VAL == 1075)
#define VDD_CPUB_REG   0x74
#elif (VDD_CPUB_VAL == 1080)
#define VDD_CPUB_REG   0x75
#elif (VDD_CPUB_VAL == 1085)
#define VDD_CPUB_REG   0x76
#elif (VDD_CPUB_VAL == 1090)
#define VDD_CPUB_REG   0x77
#elif (VDD_CPUB_VAL == 1095)
#define VDD_CPUB_REG   0x78
#elif (VDD_CPUB_VAL == 1100)
#define VDD_CPUB_REG   0x79
#elif (VDD_CPUB_VAL == 1105)
#define VDD_CPUB_REG   0x7a
#elif (VDD_CPUB_VAL == 1110)
#define VDD_CPUB_REG   0x7b
#elif (VDD_CPUB_VAL == 1115)
#define VDD_CPUB_REG   0x7c
#elif (VDD_CPUB_VAL == 1120)
#define VDD_CPUB_REG   0x7d
#elif (VDD_CPUB_VAL == 1125)
#define VDD_CPUB_REG   0x7e
#elif (VDD_CPUB_VAL == 1130)
#define VDD_CPUB_REG   0x7f
#elif (VDD_CPUB_VAL == 1135)
#define VDD_CPUB_REG   0x80
#elif (VDD_CPUB_VAL == 1140)
#define VDD_CPUB_REG   0x81
#elif (VDD_CPUB_VAL == 1145)
#define VDD_CPUB_REG   0x82
#elif (VDD_CPUB_VAL == 1150)
#define VDD_CPUB_REG   0x83
#elif (VDD_CPUB_VAL == 1155)
#define VDD_CPUB_REG   0x84
#elif (VDD_CPUB_VAL == 1160)
#define VDD_CPUB_REG   0x85
#elif (VDD_CPUB_VAL == 1165)
#define VDD_CPUB_REG   0x86
#elif (VDD_CPUB_VAL == 1170)
#define VDD_CPUB_REG   0x87
#elif (VDD_CPUB_VAL == 1175)
#define VDD_CPUB_REG   0x88
#elif (VDD_CPUB_VAL == 1180)
#define VDD_CPUB_REG   0x89
#elif (VDD_CPUB_VAL == 1185)
#define VDD_CPUB_REG   0x8a
#elif (VDD_CPUB_VAL == 1190)
#define VDD_CPUB_REG   0x8b
#elif (VDD_CPUB_VAL == 1195)
#define VDD_CPUB_REG   0x8c
#elif (VDD_CPUB_VAL == 1200)
#define VDD_CPUB_REG   0x8d
#elif (VDD_CPUB_VAL == 1205)
#define VDD_CPUB_REG   0x8e
#elif (VDD_CPUB_VAL == 1210)
#define VDD_CPUB_REG   0x8f
#elif (VDD_CPUB_VAL == 1215)
#define VDD_CPUB_REG   0x90
#elif (VDD_CPUB_VAL == 1220)
#define VDD_CPUB_REG   0x91
#elif (VDD_CPUB_VAL == 1225)
#define VDD_CPUB_REG   0x92
#elif (VDD_CPUB_VAL == 1230)
#define VDD_CPUB_REG   0x93
#elif (VDD_CPUB_VAL == 1235)
#define VDD_CPUB_REG   0x94
#elif (VDD_CPUB_VAL == 1240)
#define VDD_CPUB_REG   0x95
#elif (VDD_CPUB_VAL == 1245)
#define VDD_CPUB_REG   0x96
#elif (VDD_CPUB_VAL == 1250)
#define VDD_CPUB_REG   0x97
#elif (VDD_CPUB_VAL == 1255)
#define VDD_CPUB_REG   0x98
#elif (VDD_CPUB_VAL == 1260)
#define VDD_CPUB_REG   0x99
#elif (VDD_CPUB_VAL == 1265)
#define VDD_CPUB_REG   0x9a
#elif (VDD_CPUB_VAL == 1270)
#define VDD_CPUB_REG   0x9b
#elif (VDD_CPUB_VAL == 1275)
#define VDD_CPUB_REG   0x9c
#elif (VDD_CPUB_VAL == 1280)
#define VDD_CPUB_REG   0x9d
#elif (VDD_CPUB_VAL == 1285)
#define VDD_CPUB_REG   0x9e
#elif (VDD_CPUB_VAL == 1290)
#define VDD_CPUB_REG   0x9f
#elif (VDD_CPUB_VAL == 1295)
#define VDD_CPUB_REG   0xa0
#elif (VDD_CPUB_VAL == 1300)
#define VDD_CPUB_REG   0xa1
#elif (VDD_CPUB_VAL == 1305)
#define VDD_CPUB_REG   0xa2
#elif (VDD_CPUB_VAL == 1310)
#define VDD_CPUB_REG   0xa3
#elif (VDD_CPUB_VAL == 1315)
#define VDD_CPUB_REG   0xa4
#elif (VDD_CPUB_VAL == 1320)
#define VDD_CPUB_REG   0xa5
#elif (VDD_CPUB_VAL == 1325)
#define VDD_CPUB_REG   0xa6
#elif (VDD_CPUB_VAL == 1330)
#define VDD_CPUB_REG   0xa7
#elif (VDD_CPUB_VAL == 1335)
#define VDD_CPUB_REG   0xa8
#elif (VDD_CPUB_VAL == 1340)
#define VDD_CPUB_REG   0xa9
#elif (VDD_CPUB_VAL == 1345)
#define VDD_CPUB_REG   0xaa
#elif (VDD_CPUB_VAL == 1350)
#define VDD_CPUB_REG   0xab
#else
#error "VDD_CPUB val out of range\n"
#endif

#define I2C_BUS		0x2
#define I2C_M3		9

#define I2C_DEV_ADDR_BD2657    0x6a
#define BD2657_REG_REGLOCK		0xa0
#define BD2657_REG_BUCK0_VOLT_RUN  0x0a
#define BD2657_REG_BUCK2_VOLT_RUN  0x12
#define BD2657_REG_CRD_TIMER	0x2a
#define BD2657_REG_CRD_TIMER_VALUE	0x00

/* gpio/pinmux/pwm init */
register_ops_t __bl2_ops_reg[MAX_REG_OPS_ENTRIES]
__attribute__ ((section(".misc_param"))) = {
	/*config vddee and vcpua and vcpub - pwm_a and pwm_b*/
	{PWMAB_PWM_A,		VCCK_A_VAL_REG,  	0xffffffff,	0, 0, 0},
	{PWMAB_PWM_B,		VCCK_B_VAL_REG,  	0xffffffff,	0, 0, 0},
	{PWMAB_MISC_REG_AB,	(0x3 << 0), 		(0x3 << 0), 	0, 0, 0},

	/* enable vddcpu dc-dc, set TEST_N to high */
	{ PADCTRL_TESTN_O,	(0x1 << 0), (0x1 << 0), 0, 0, 0 },
	{ PADCTRL_TESTN_OEN,	(0x0 << 0), (0x1 << 0), 0, 0, 0 },

	/* set pwm a and pwm b clock rate to 24M, enable them */
	{CLKCTRL_PWM_CLK_AB_CTRL, ((0x1 << 8) | (0x1 << 24)), 0xffffffff, 0, 0, 0},

	/* set GPIOE_0 GPIOE_1 drive strength to 3 */
	{ PADCTRL_GPIOE_DS,	0xf,	    0xf,	0, 0, 0 },
	/* set GPIOE_0 GPIOE_1 mux to pwma pwmb */
	{ PADCTRL_PIN_MUX_REG4, (0x1 << 0), (0xf << 0), 0, 0, 0 },
	{ PADCTRL_PIN_MUX_REG4, (0x1 << 4), (0xf << 4), 0, 0, 0 },

		/*set pinmux GPIOD_2(I2CM_J_SCL) and GPIOD_3(I2CM_J_SDA) for pmic*/
	{PADCTRL_GPIOD_DS,	(0xf << 4),	(0xf << 4),	0,	0,	0},
	{PADCTRL_GPIOD_PULL_EN,	(0x3 << 2),	(0x3 << 2),	0,	0,	0},
	{PADCTRL_GPIOD_PULL_UP,	(0x3 << 2),	(0x3 << 2),	0,	0,	0},
	{PADCTRL_PIN_MUX_REG2,	(0x11 << 8),	(0xff << 8),	0,	0,	0},

	/* bd2657 : set vdd_cpua voltage*/
	{BD2657_REG_BUCK0_VOLT_RUN, VDD_CPUA_REG, 0xff, 0,
		(I2C_BUS << 4) | (I2C_M3), I2C_DEV_ADDR_BD2657},
	/* bd2657 : set vdd_cpub voltage*/
	{BD2657_REG_BUCK2_VOLT_RUN, VDD_CPUB_REG, 0xff, 0,
		(I2C_BUS << 4) | (I2C_M3), I2C_DEV_ADDR_BD2657},

	/* bd2657 : set Cold-Reset duration*/
	{BD2657_REG_CRD_TIMER, BD2657_REG_CRD_TIMER_VALUE, 0xff, 0,
		(I2C_BUS << 4) | (I2C_M3), I2C_DEV_ADDR_BD2657},
};

#define DEV_FIP_SIZE 0x300000
#define DDR_FIP_SIZE 0x40000
/* for all the storage parameter */
storage_parameter_t __store_para __attribute__ ((section(".store_param"))) = {
	.common					={
		.version			= 0x01,
		.device_fip_container_size	= DEV_FIP_SIZE,
		.device_fip_container_copies	= 4,
		.ddr_fip_container_size = DDR_FIP_SIZE,
	},
	.nand					={
		.version			= 0x01,
		.bbt_pages			= 0x1,
		.bbt_start_block		= 20,
		.discrete_mode			= 1,
		.setup_data.nand_setup_data	= (2 << 20) |		    \
						  (0 << 19) |			  \
						  (1 << 17) |			  \
						  (1 << 14) |			  \
						  (0 << 13) |			  \
						  (64 << 6) |			  \
						  (4 << 0),
		.reserved_area_blk_cnt		= 48,
		.page_per_block			= 64,
		.use_param_page_list		= 0,
	},
};
