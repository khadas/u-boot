/*
 * board/amlogic/p1_skt_v1/firmware/timing.c
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
#define BOARD_ENABLE_LPDDR4   1
//#define BOARD_ENABLE_LPDDR5   1
#define BOARD_ENABLE_LPDDR5_RDL_MODIFY   1

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
	.DRAMFreq = { LPDDR4_DDR_CLK,		     0,	     0,	     0	    },
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
	.DRAMFreq = { LPDDR4_DDR_CLK,		     0,	     0,	     0	    },
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
		(1 << 7) | (15),
		(1 << 7) | (0),
		(1 << 7) | (15),
		(1 << 7) | (0)
	},//bit 7 offset direction 0 ++  1 --
	.rx_offset = {
		(1 << 7) | (0),
		(1 << 7) | (0)
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
		(1 << 7) | (15),
		(1 << 7) | (0),
		(1 << 7) | (15),
		(1 << 7) | (0)
	},//bit 7 offset direction 0 ++  1 --
	.rx_offset = {
		(1 << 7) | (0),
		(1 << 7) | (0)
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

#define VDDEE_VAL                               AML_VDDEE_INIT_VOLTAGE
#define VCCK_A_VAL                              AML_VDD_CPUA_INIT_VOLTAGE
#define VCCK_B_VAL                              AML_VDD_CPUB_INIT_VOLTAGE
#define VDD_GPU_VAL                             AML_VDDGPU_INIT_VOLTAGE
#define VDD_NPU_VAL                             AML_VDDNPU_INIT_VOLTAGE

/* VDDEE_VAL_REG ax208 board by  mesured  value */
#if   (VDDEE_VAL == 680)
#define VDDEE_VAL_REG   0x120000
#elif (VDDEE_VAL == 690)
#define VDDEE_VAL_REG   0x110001
#elif (VDDEE_VAL == 700)
#define VDDEE_VAL_REG   0x100002
#elif (VDDEE_VAL == 710)
#define VDDEE_VAL_REG   0xf0003
#elif (VDDEE_VAL == 720)
#define VDDEE_VAL_REG   0xe0004
#elif (VDDEE_VAL == 730)
#define VDDEE_VAL_REG   0xd0005
#elif (VDDEE_VAL == 740)
#define VDDEE_VAL_REG   0xc0006
#elif (VDDEE_VAL == 750)
#define VDDEE_VAL_REG   0xb0007
#elif (VDDEE_VAL == 760)
#define VDDEE_VAL_REG   0xa0008
#elif (VDDEE_VAL == 770)
#define VDDEE_VAL_REG   0x90009
#elif (VDDEE_VAL == 780)
#define VDDEE_VAL_REG   0x8000a
#elif (VDDEE_VAL == 790)
#define VDDEE_VAL_REG   0x7000b
#elif (VDDEE_VAL == 800)
#define VDDEE_VAL_REG   0x6000c
#elif (VDDEE_VAL == 810)
#define VDDEE_VAL_REG   0x5000d
#elif (VDDEE_VAL == 820)
#define VDDEE_VAL_REG   0x4000e
#elif (VDDEE_VAL == 830)
#define VDDEE_VAL_REG   0x3000f
#elif (VDDEE_VAL == 840)
#define VDDEE_VAL_REG   0x20010
#elif (VDDEE_VAL == 850)
#define VDDEE_VAL_REG   0x10011
#elif (VDDEE_VAL == 860)
#define VDDEE_VAL_REG   0x12
#else
#error "VDDEE val out of range\n"
#endif

/* VCCK_D_VAL(A55) PWM table */
#if   (VCCK_A_VAL == 1039)
#define VCCK_A_VAL_REG  0x00000022
#elif (VCCK_A_VAL == 1029)
#define VCCK_A_VAL_REG  0x00010021
#elif (VCCK_A_VAL == 1019)
#define VCCK_A_VAL_REG  0x00020020
#elif (VCCK_A_VAL == 1009)
#define VCCK_A_VAL_REG  0x0003001f
#elif (VCCK_A_VAL == 999)
#define VCCK_A_VAL_REG  0x0004001e
#elif (VCCK_A_VAL == 989)
#define VCCK_A_VAL_REG  0x0005001d
#elif (VCCK_A_VAL == 979)
#define VCCK_A_VAL_REG  0x0006001c
#elif (VCCK_A_VAL == 969)
#define VCCK_A_VAL_REG  0x0007001b
#elif (VCCK_A_VAL == 959)
#define VCCK_A_VAL_REG  0x0008001a
#elif (VCCK_A_VAL == 949)
#define VCCK_A_VAL_REG  0x00090019
#elif (VCCK_A_VAL == 939)
#define VCCK_A_VAL_REG  0x000a0018
#elif (VCCK_A_VAL == 929)
#define VCCK_A_VAL_REG  0x000b0017
#elif (VCCK_A_VAL == 919)
#define VCCK_A_VAL_REG  0x000c0016
#elif (VCCK_A_VAL == 909)
#define VCCK_A_VAL_REG  0x000d0015
#elif (VCCK_A_VAL == 899)
#define VCCK_A_VAL_REG  0x000e0014
#elif (VCCK_A_VAL == 889)
#define VCCK_A_VAL_REG  0x000f0013
#elif (VCCK_A_VAL == 879)
#define VCCK_A_VAL_REG  0x00100012
#elif (VCCK_A_VAL == 869)
#define VCCK_A_VAL_REG  0x00110011
#elif (VCCK_A_VAL == 859)
#define VCCK_A_VAL_REG  0x00120010
#elif (VCCK_A_VAL == 849)
#define VCCK_A_VAL_REG  0x0013000f
#elif (VCCK_A_VAL == 839)
#define VCCK_A_VAL_REG  0x0014000e
#elif (VCCK_A_VAL == 829)
#define VCCK_A_VAL_REG  0x0015000d
#elif (VCCK_A_VAL == 819)
#define VCCK_A_VAL_REG  0x0016000c
#elif (VCCK_A_VAL == 809)
#define VCCK_A_VAL_REG  0x0017000b
#elif (VCCK_A_VAL == 799)
#define VCCK_A_VAL_REG  0x0018000a
#elif (VCCK_A_VAL == 789)
#define VCCK_A_VAL_REG  0x00190009
#elif (VCCK_A_VAL == 779)
#define VCCK_A_VAL_REG  0x001a0008
#elif (VCCK_A_VAL == 769)
#define VCCK_A_VAL_REG  0x001b0007
#elif (VCCK_A_VAL == 759)
#define VCCK_A_VAL_REG  0x001c0006
#elif (VCCK_A_VAL == 749)
#define VCCK_A_VAL_REG  0x001d0005
#elif (VCCK_A_VAL == 739)
#define VCCK_A_VAL_REG  0x001e0004
#elif (VCCK_A_VAL == 729)
#define VCCK_A_VAL_REG  0x001f0003
#elif (VCCK_A_VAL == 719)
#define VCCK_A_VAL_REG  0x00200002
#elif (VCCK_A_VAL == 709)
#define VCCK_A_VAL_REG  0x00210001
#elif (VCCK_A_VAL == 699)
#define VCCK_A_VAL_REG  0x00220000
#else
#error "VCCK_A val out of range\n"
#endif

/* VCCK_A_VAL(A76) PWM table */
#if   (VCCK_B_VAL == 1039)
#define VCCK_B_VAL_REG  0x00000022
#elif (VCCK_B_VAL == 1029)
#define VCCK_B_VAL_REG  0x00010021
#elif (VCCK_B_VAL == 1019)
#define VCCK_B_VAL_REG  0x00020020
#elif (VCCK_B_VAL == 1009)
#define VCCK_B_VAL_REG  0x0003001f
#elif (VCCK_B_VAL == 999)
#define VCCK_B_VAL_REG  0x0004001e
#elif (VCCK_B_VAL == 989)
#define VCCK_B_VAL_REG  0x0005001d
#elif (VCCK_B_VAL == 979)
#define VCCK_B_VAL_REG  0x0006001c
#elif (VCCK_B_VAL == 969)
#define VCCK_B_VAL_REG  0x0007001b
#elif (VCCK_B_VAL == 959)
#define VCCK_B_VAL_REG  0x0008001a
#elif (VCCK_B_VAL == 949)
#define VCCK_B_VAL_REG  0x00090019
#elif (VCCK_B_VAL == 939)
#define VCCK_B_VAL_REG  0x000a0018
#elif (VCCK_B_VAL == 929)
#define VCCK_B_VAL_REG  0x000b0017
#elif (VCCK_B_VAL == 919)
#define VCCK_B_VAL_REG  0x000c0016
#elif (VCCK_B_VAL == 909)
#define VCCK_B_VAL_REG  0x000d0015
#elif (VCCK_B_VAL == 899)
#define VCCK_B_VAL_REG  0x000e0014
#elif (VCCK_B_VAL == 889)
#define VCCK_B_VAL_REG  0x000f0013
#elif (VCCK_B_VAL == 879)
#define VCCK_B_VAL_REG  0x00100012
#elif (VCCK_B_VAL == 869)
#define VCCK_B_VAL_REG  0x00110011
#elif (VCCK_B_VAL == 859)
#define VCCK_B_VAL_REG  0x00120010
#elif (VCCK_B_VAL == 849)
#define VCCK_B_VAL_REG  0x0013000f
#elif (VCCK_B_VAL == 839)
#define VCCK_B_VAL_REG  0x0014000e
#elif (VCCK_B_VAL == 829)
#define VCCK_B_VAL_REG  0x0015000d
#elif (VCCK_B_VAL == 819)
#define VCCK_B_VAL_REG  0x0016000c
#elif (VCCK_B_VAL == 809)
#define VCCK_B_VAL_REG  0x0017000b
#elif (VCCK_B_VAL == 799)
#define VCCK_B_VAL_REG  0x0018000a
#elif (VCCK_B_VAL == 789)
#define VCCK_B_VAL_REG  0x00190009
#elif (VCCK_B_VAL == 779)
#define VCCK_B_VAL_REG  0x001a0008
#elif (VCCK_B_VAL == 769)
#define VCCK_B_VAL_REG  0x001b0007
#elif (VCCK_B_VAL == 759)
#define VCCK_B_VAL_REG  0x001c0006
#elif (VCCK_B_VAL == 749)
#define VCCK_B_VAL_REG  0x001d0005
#elif (VCCK_B_VAL == 739)
#define VCCK_B_VAL_REG  0x001e0004
#elif (VCCK_B_VAL == 729)
#define VCCK_B_VAL_REG  0x001f0003
#elif (VCCK_B_VAL == 719)
#define VCCK_B_VAL_REG  0x00200002
#elif (VCCK_B_VAL == 709)
#define VCCK_B_VAL_REG  0x00210001
#elif (VCCK_B_VAL == 699)
#define VCCK_B_VAL_REG  0x00220000
#else
#error "VCCK_B val out of range\n"
#endif

/* VDD_GPU PWM table */
#if   (VDD_GPU_VAL == 720)
#define VDD_GPU_VAL_REG 0x120000
#elif (VDD_GPU_VAL == 730)
#define VDD_GPU_VAL_REG 0x110001
#elif (VDD_GPU_VAL == 740)
#define VDD_GPU_VAL_REG 0x100002
#elif (VDD_GPU_VAL == 750)
#define VDD_GPU_VAL_REG 0xf0003
#elif (VDD_GPU_VAL == 760)
#define VDD_GPU_VAL_REG 0xe0004
#elif (VDD_GPU_VAL == 770)
#define VDD_GPU_VAL_REG 0xd0005
#elif (VDD_GPU_VAL == 780)
#define VDD_GPU_VAL_REG 0xc0006
#elif (VDD_GPU_VAL == 790)
#define VDD_GPU_VAL_REG 0xb0007
#elif (VDD_GPU_VAL == 800)
#define VDD_GPU_VAL_REG 0xa0008
#elif (VDD_GPU_VAL == 810)
#define VDD_GPU_VAL_REG 0x90009
#elif (VDD_GPU_VAL == 820)
#define VDD_GPU_VAL_REG 0x8000a
#elif (VDD_GPU_VAL == 830)
#define VDD_GPU_VAL_REG 0x7000b
#elif (VDD_GPU_VAL == 840)
#define VDD_GPU_VAL_REG 0x6000c
#elif (VDD_GPU_VAL == 850)
#define VDD_GPU_VAL_REG 0x5000d
#elif (VDD_GPU_VAL == 860)
#define VDD_GPU_VAL_REG 0x4000e
#elif (VDD_GPU_VAL == 870)
#define VDD_GPU_VAL_REG 0x3000f
#elif (VDD_GPU_VAL == 880)
#define VDD_GPU_VAL_REG 0x20010
#elif (VDD_GPU_VAL == 890)
#define VDD_GPU_VAL_REG 0x10011
#elif (VDD_GPU_VAL == 900)
#define VDD_GPU_VAL_REG 0x12
#else
#error "VDD_GPU val out of range\n"
#endif

/* VDD_NPU PWM table */
#if   (VDD_NPU_VAL == 720)
#define VDD_NPU_VAL_REG 0x120000
#elif (VDD_NPU_VAL == 730)
#define VDD_NPU_VAL_REG 0x110001
#elif (VDD_NPU_VAL == 740)
#define VDD_NPU_VAL_REG 0x100002
#elif (VDD_NPU_VAL == 750)
#define VDD_NPU_VAL_REG 0xf0003
#elif (VDD_NPU_VAL == 760)
#define VDD_NPU_VAL_REG 0xe0004
#elif (VDD_NPU_VAL == 770)
#define VDD_NPU_VAL_REG 0xd0005
#elif (VDD_NPU_VAL == 780)
#define VDD_NPU_VAL_REG 0xc0006
#elif (VDD_NPU_VAL == 790)
#define VDD_NPU_VAL_REG 0xb0007
#elif (VDD_NPU_VAL == 800)
#define VDD_NPU_VAL_REG 0xa0008
#elif (VDD_NPU_VAL == 810)
#define VDD_NPU_VAL_REG 0x90009
#elif (VDD_NPU_VAL == 820)
#define VDD_NPU_VAL_REG 0x8000a
#elif (VDD_NPU_VAL == 830)
#define VDD_NPU_VAL_REG 0x7000b
#elif (VDD_NPU_VAL == 840)
#define VDD_NPU_VAL_REG 0x6000c
#elif (VDD_NPU_VAL == 850)
#define VDD_NPU_VAL_REG 0x5000d
#elif (VDD_NPU_VAL == 860)
#define VDD_NPU_VAL_REG 0x4000e
#elif (VDD_NPU_VAL == 870)
#define VDD_NPU_VAL_REG 0x3000f
#elif (VDD_NPU_VAL == 880)
#define VDD_NPU_VAL_REG 0x20010
#elif (VDD_NPU_VAL == 890)
#define VDD_NPU_VAL_REG 0x10011
#elif (VDD_NPU_VAL == 900)
#define VDD_NPU_VAL_REG 0x12
#else
#error "VDD_NPU val out of range\n"
#endif

bl2_reg_t __bl2_reg[] __attribute__ ((section(".generic_param"))) = {
	//hxbao, need fine tune
	{ 0, 0, 0xffffffff, 0, 0, 0 },
};

/* gpio/pinmux/pwm init */
register_ops_t __bl2_ops_reg[MAX_REG_OPS_ENTRIES]
__attribute__ ((section(".misc_param"))) = {
	/* demo, user defined override register */

	/*config vddee vddcpu vddgpu vddnpu pwm
	 *
	 *-pwm_a, pwm_b pwm_c, pwm_d and pwm_e
	 *
	 *For regulator, pls config _VOLTAGE_VAL_
	 */
	{ PWMAB_PWM_B,		   VDDEE_VAL_REG, 0xffffffff, 0, 0, 0},
	{ PWMAB_PWM_A,		   VCCK_B_VAL_REG,  0xffffffff, 0, 0, 0 },
	{ PWMCD_PWM_A,		   VDD_GPU_VAL_REG,  0xffffffff, 0, 0, 0 },
	{ PWMCD_PWM_B,		   VCCK_A_VAL_REG,  0xffffffff, 0, 0, 0 },
	{ PWMEF_PWM_A,		   VDD_NPU_VAL_REG,  0xffffffff, 0, 0, 0 },
	{ PWMAB_MISC_REG_AB, (0x3 << 0) | (0x1 << 15) | (0x1 << 23),
				(0x3 << 0) | (0x1 << 15) | (0x1 << 23), 0, 0, 0 },
	{ PWMCD_MISC_REG_AB, (0x3 << 0) | (0x1 << 15) | (0x1 << 23),
				(0x3 << 0) | (0x1 << 15) | (0x1 << 23), 0, 0, 0 },
	{ PWMEF_MISC_REG_AB, (0x1 << 0) | (0x1 << 15),
				(0x1 << 0) | (0x1 << 15), 0, 0, 0 },
	/* set pwm a and pwm b clock rate to 24M, enable them */
	{ CLKCTRL_PWM_CLK_AB_CTRL, ((0x1 << 8) | (0x1 << 24)), 0xffffffff, 0, 0, 0 },
	/* set pwm c and pwm d clock rate to 24M, enable them */
	{ CLKCTRL_PWM_CLK_CD_CTRL, ((0x1 << 8) | (0x1 << 24)), 0xffffffff, 0, 0, 0 },
	/* set pwm e clock rate to 24M, enable them */
	{ CLKCTRL_PWM_CLK_EF_CTRL, (0x1 << 8), 0xffffffff, 0, 0, 0 },
	/* set GPIOE_0 GPIOE_1  GPIOE_2 GPIOE_3 drive strength to 3 */
	{ PADCTRL_GPIOE_DS,	   0xff,		  0xff,	      0, 0, 0 },
	/* set GPIOE_4 drive strength to 3 */
	{ PADCTRL_GPIOE_DS,	   (0x3 << 8),		  (0x3 << 8),	      0, 0, 0 },
	/* set GPIOE_0 GPIOE_1 GPIOE_2 GPIOE_3 mux to pwmb pwmd pwma pwmc */
	{ PADCTRL_PIN_MUX_REG7,	   (0x1 << 0) | (0x1 << 4) | (0x1 << 8) | (0x1 << 12),
				(0xffff << 0), 0, 0, 0 },
	/* set GPIOE_4 mux to pwme */
	{ PADCTRL_PIN_MUX_REG7,	  (0x1 << 16),		  (0xf << 16), 0, 0, 0 },
	/* enable vddcpu dc-dc, set TEST_N to high */
	{ PADCTRL_TESTN_O,	(0x1 << 0), (0x1 << 0), 0, 0, 0 },
	{ PADCTRL_TESTN_OEN,	(0x0 << 0), (0x1 << 0), 0, 0, 0 },
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
