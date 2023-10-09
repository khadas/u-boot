// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * board/amlogic/t3_ar311/firmware/timing.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <asm/arch/secure_apb.h>
#include <asm/arch/timing.h>
#include <asm/arch/ddr_define.h>

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
 *		#define		CONFIG_DDR0_16BIT_CH0					1
 *		#define		CONFIG_DDR0_16BIT_RANK01_CH0			4
 *		#define		CONFIG_DDR0_32BIT_RANK0_CH0				2
 *		#define		CONFIG_DDR0_32BIT_RANK01_CH01			3
 *		#define		CONFIG_DDR0_32BIT_16BIT_RANK0_CH0		5
 *		#define		CONFIG_DDR0_32BIT_16BIT_RANK01_CH0		6
 *		#define		CONFIG_DDR0_32BIT_RANK01_CH0			7
 *		#define		CONFIG_DDR0_32BIT_RANK0_CH01			8
 *		#define         CONFIG_DDR0_32BIT_RANK0_16BIT_RANK1_LOW_CH0		0x9
 *		#define         CONFIG_DDR0_32BIT_RANK0_16BIT_RANK1_HIGH_CH0	0xa
 *		#define         CONFIG_DDR0_32BIT_16BIT_RANK0_CH0_MODE2			0xB
 *		#define         CONFIG_DDR0_32BIT_RANK0_16BIT_RANK1_LOW_CH0_MODE2	0xC
 *
 *		#define		CONFIG_DISABLE_D32_D63		0xf0
 *		#define		CONFIG_DISABLE_D16_D31		0xfc
 * DramType:
 *		#define		CONFIG_DDR_TYPE_DDR3		0
 *		#define		CONFIG_DDR_TYPE_DDR4		1
 *		#define		CONFIG_DDR_TYPE_LPDDR4		2
 *		#define		CONFIG_DDR_TYPE_LPDDR3		3
 *		#define		CONFIG_DDR_TYPE_LPDDR2		4
 *		#define		CONFIG_DDR_TYPE_LPDDR4X		5
 *
 */

#define ENABLE_A4_SKT_DDR4_BOARD 1
// #define ENABLE_A4_SKT_DDR3_BOARD 1
// #define ENABLE_A4_SKT_LPDDR4_BOARD 1

#ifdef ENABLE_A4_SKT_DDR4_BOARD
// 600 //1176 //1320 //1464 //1584 //1800 //792 //912 //1008
#define  DDR4_CLK_DDR0   1584
#define  DDR4_CLK_DDR1   1584
#endif

#ifdef ENABLE_A4_SKT_DDR3_BOARD
// 600 //1176 //1320 //1464 //1584 //1800 //792 //912 //1008
#define  DDR3_CLK_DDR0   1056
#define  DDR3_CLK_DDR1   1056
#endif

#ifdef ENABLE_A4_SKT_LPDDR4_BOARD
// 600 //1176 //1320 //1464 //1584 //1800 //792 //912 //1008
#define  LPDDR4_CLK_DDR0   1512
#define  LPDDR4_CLK_DDR1   1512
#endif

#define  CONFIG_BOARD_TIMMING

 //timing_config,REF BC302 2layer 4pcs ddr4
ddr_set_ps0_only_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {
#if ENABLE_A4_SKT_DDR4_BOARD
	{
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs =
			sizeof(ddr_set_ps0_only_t) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,
		.cfg_board_common_setting.fast_boot = {
			0, 0, 0, 0
		},
		.cfg_board_common_setting.ddr_func = (0 + (1 << 29)),
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_DDR4,
		.cfg_board_common_setting.enable_lpddr4x_mode = 0,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_16BIT_CH0,
		.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.DisabledDbyte[0] = 0xfc,
		//bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
		.cfg_board_common_setting.DisabledDbyte[1] = 0xfc,
		//bit 0 -3 ch1 cs0 ,bit 4-7 ch1
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		.cfg_board_common_setting.log_level = 4,
		//4,//LOG_LEVEL_BASIC,
		.cfg_board_common_setting.dbi_enable = DDR_WRITE_READ_DBI_DISABLE,
		.cfg_board_common_setting.pll_ssc_mode =
			(1 << 20) | (1 << 8) | (2 << 4) | 0,
		//center_ssc_1000ppm,//SSC_DISABLE,(1 << 20) | (0 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test = {
			0
		},
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR4_16BIT_2,
		.cfg_board_common_setting.ddr_dqs_swap = 0,

		.cfg_board_common_setting.ddr_vddee_setting = {
			0
		},
		.cfg_board_SI_setting_ps.DRAMFreq = DDR4_CLK_DDR0,
		// .cfg_ddr_training_delay_ps.DRAMFreq = 600,
		// 2112,//1176,cfg_ddr_training_delay_ps
		//.cfg_ddr_training_delay_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		.cfg_board_SI_setting_ps.dfi_odt_config = DDR_DRAM_ODT_DDR4_PARK_ENABLE,
		//DDR_DRAM_ODT_W_CS0_ODT0,
		//.cfg_ddr_training_delay_ps.dfe_offset = 0,
		.cfg_board_SI_setting_ps.vref_ac_permil = 0,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,
		//real chip stk lp4
		.cfg_board_common_setting.ddr_rfc_type = DDR_RFC_TYPE_DDR4_2Gbx8,
		.cfg_board_common_setting.pll_ssc_mode = 0x00000000,

#ifdef CONFIG_BOARD_TIMMING     //skt lp4 board
#define TDQS2DQ  ((0 * 128 * DDR4_CLK_DDR0) / 1000000)
//#define BOARD_DQS_DELAY   64+32
#define TDQSCK_ADD_DQS   ((0 * 128 * DDR4_CLK_DDR0) / 1000000) //clk should use 64 steps
#define CLK_DELAY  (0)
#define PHY_TDQS2DQ  ((0 * 128 * DDR4_CLK_DDR0) / 1000000)
#define WRITE_LEVELING_DELAY  ((250 * 128 * DDR4_CLK_DDR0 * 2) / 1000000)
//for 1584M 100 STEP 250ps //((480 * 128 * CACLU_CLK * 2) / 1000000)
#endif
		.cfg_board_SI_setting_ps.clk_drv_ohm = 60,
		.cfg_board_SI_setting_ps.cs_drv_ohm = 60,
		.cfg_board_SI_setting_ps.ac_drv_ohm = 60,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = 40,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = 40,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = 60,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = 0,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = 40,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = 60,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = 0,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = 120,
		.cfg_board_SI_setting_ps.dram_drv_pull_up_cal_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_range =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,

		.cfg_board_common_setting.ac_pinmux = {
			1,  16, 7,  26, 8,  13, 11, 3,	22, 9,
			2,  23, 5,  27, 21, 20, 19, 4,	10, 12, 0,
			15, 17, 18, 6,	14, 24, 25, 28, 29,
		},
		.cfg_board_common_setting.ddr_dq_remap = {
			12,
			33,
			10,
			8,
			14,
			9,
			11,
			13,
			15,
			0,
			2,
			32,
			4,
			1,
			5,
			6,
			3,
			7,
			21,
			22,
			23,
			18,
			17,
			16,
			34,
			19,
			20,
			24,
			25,
			35,
			27,
			28,
			26,
			31,
			30,
			29,
		},
		.cfg_ddr_training_delay_ps.reserve_para = {
			//cs0 write dqs,lane0-lane3
			(1 << 7) | 10, (1 << 7) | 6, (0 << 7) | 0, (0 << 7) | 0,
			//cs1 write dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs0 read dqs,lane0-lane3
			(1 << 7) | 10, (1 << 7) | 10, (0 << 7) | 0, (0 << 7) | 0,
			//cs1 read dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		},
		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 384,
		//cke 128 only 1UI margin
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 384,
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 256 + 0,         // cs
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 256 + 0,        // cs
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 128 + 128,      //ck
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 128 + 128,      //ck
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 256,            //cke
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 256,            //cke
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 0,               //odt0
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 0,              //odt1
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[0] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[1] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[2] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[3] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[4] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[5] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[6] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[7] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[8] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[9] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[10] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[11] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[12] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[13] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[14] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[15] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[16] = 64,
		// .cfg_ddr_training_delay_ps.read_dq_delay_t[17] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[0] = 0x49,//73,0x118
		.cfg_ddr_training_delay_ps.read_dq_delay_t[1] = 0x58,//88,0x11a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[2] = 0x5c,//92,0x11c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[3] = 0x4f,//79,0x11e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[4] = 0x65,//101,0x120
		.cfg_ddr_training_delay_ps.read_dq_delay_t[5] = 0x65,//101,0x122
		.cfg_ddr_training_delay_ps.read_dq_delay_t[6] = 0x4c,//76,0x124
		.cfg_ddr_training_delay_ps.read_dq_delay_t[7] = 0x6d,//109,0x126
		.cfg_ddr_training_delay_ps.read_dq_delay_t[8] = 0x4d,//77,0x128
		.cfg_ddr_training_delay_ps.read_dq_delay_t[9] = 0x5a,//90,0x12a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[10] = 0x4a,//74,0x12c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[11] = 0x57,//87,0x12e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[12] = 0x3f,//63,0x130
		.cfg_ddr_training_delay_ps.read_dq_delay_t[13] = 0x71,//113,0x132
		.cfg_ddr_training_delay_ps.read_dq_delay_t[14] = 0x68,//104,0x134
		.cfg_ddr_training_delay_ps.read_dq_delay_t[15] = 0x45,//69,0x136
		.cfg_ddr_training_delay_ps.read_dq_delay_t[16] = 0x67,//103,0x138
		.cfg_ddr_training_delay_ps.read_dq_delay_t[17] = 0x51,//81,0x13a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[18] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[19] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[20] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[21] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[22] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[23] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[24] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[25] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[26] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[27] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[28] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[29] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[30] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[31] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[32] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[33] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[34] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[35] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[36] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[37] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[38] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[39] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[40] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[41] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[42] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[43] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[44] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[45] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[46] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[47] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[48] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[49] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[50] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[51] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[52] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[53] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[54] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[55] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[56] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[57] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[58] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[59] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[60] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[61] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[62] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[63] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[64] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[65] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[66] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[67] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[68] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[69] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[70] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[71] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[0] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[1] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[2] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[3] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[4] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[5] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[6] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[7] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[8] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[9] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[10] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[11] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[12] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[13] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[14] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[15] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[16] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[17] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[18] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[19] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[20] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[21] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[22] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[23] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[24] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[25] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[26] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[27] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[28] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[29] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[30] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[31] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[32] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[33] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[34] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[35] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[36] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[37] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[38] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[39] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[40] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[41] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[42] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[43] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[44] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[45] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[46] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[47] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[48] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[49] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[50] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[51] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[52] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[53] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[54] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[55] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[56] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[57] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[58] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[59] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[60] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[61] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[62] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[63] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[64] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[65] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[66] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[67] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[68] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[69] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[70] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[71] = 64,
		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 128 + WRITE_LEVELING_DELAY + 0,
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 128 + WRITE_LEVELING_DELAY + 0,
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 128 + WRITE_LEVELING_DELAY + 0,
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 128 + WRITE_LEVELING_DELAY + 0,
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 128 + WRITE_LEVELING_DELAY + 0,
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 128 + WRITE_LEVELING_DELAY + 0,
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 128 + WRITE_LEVELING_DELAY + 0,
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 128 + WRITE_LEVELING_DELAY + 0,
		.cfg_ddr_training_delay_ps.write_wck_delay[0] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[1] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[2] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[3] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[4] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[5] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[6] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[7] = 0x00000200,
		.cfg_ddr_training_delay_ps.wdq_delay[0] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[1] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[2] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[3] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[4] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[5] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[6] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[7] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[8] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[9] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[10] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[11] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[12] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[13] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[14] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[15] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[16] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[17] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[18] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[19] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[20] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[21] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[22] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[23] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[24] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[25] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[26] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[27] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[28] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[29] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[30] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[31] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[32] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[33] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[34] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[35] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[36] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[37] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[38] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[39] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[40] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[41] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[42] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[43] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[44] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[45] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[46] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[47] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[48] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[49] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[50] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[51] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[52] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[53] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[54] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[55] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[56] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[57] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[58] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[59] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[60] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[61] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[62] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[63] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[64] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[65] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[66] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[67] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[68] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[69] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[70] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[71] = 192 + TDQS2DQ + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 0,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 0,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 0,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 0,
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 128,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[0] = 0x000001f,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[1] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[2] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[3] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[4] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[5] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[6] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[7] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[8] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[9] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[10] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[11] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[12] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[13] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[14] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[15] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[16] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[17] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[18] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[19] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[20] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[21] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[22] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[23] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[24] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[25] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[26] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[27] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[28] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[29] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[30] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[31] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[32] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[33] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[34] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[35] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[0] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[1] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[2] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[3] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[4] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[5] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[6] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[7] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[8] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[9] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[10] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[11] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[12] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[13] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[14] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[15] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[16] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[17] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[18] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[19] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[20] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[21] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[22] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[23] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[24] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[25] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[26] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[27] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[28] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[29] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[30] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[31] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[32] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[33] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[34] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[35] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[0] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[1] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[2] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[3] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[4] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[5] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[6] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[7] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[8] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[9] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[10] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[11] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[12] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[13] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[14] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[15] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[16] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[17] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[18] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[19] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[20] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[21] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[22] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[23] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[24] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[25] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[26] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[27] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[28] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[29] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[30] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[31] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[32] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[33] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[34] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[35] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[0] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[1] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[2] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[3] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[4] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[5] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[6] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[7] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[8] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[9] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[10] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[11] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[12] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[13] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[14] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[15] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[16] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[17] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[18] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[19] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[20] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[21] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[22] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[23] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[24] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[25] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[26] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[27] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[28] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[29] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[30] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[31] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[32] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[33] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[34] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[35] = 0x00000030,
		.cfg_ddr_training_delay_ps.dram_vref[0] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[1] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[2] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[3] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[4] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[5] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[6] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[7] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[8] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[9] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[10] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[11] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[12] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[13] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[14] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[15] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[16] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[17] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[18] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[19] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[20] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[21] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[22] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[23] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[24] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[25] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[26] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[27] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[28] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[29] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[30] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[31] = 0x00000000,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[0] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[1] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[2] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[3] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[4] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[5] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[6] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[7] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[0] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[1] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[2] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[3] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[4] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[5] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[6] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[7] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[0] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[1] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[2] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[3] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[4] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[5] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[6] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[7] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[0] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[1] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[2] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[3] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[4] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[5] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[6] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[7] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[0] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[1] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[2] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[3] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[4] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[5] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[6] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[7] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[0] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[1] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[2] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[3] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[4] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[5] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[6] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[7] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dq_tx[0] = 0x00000002,
		.cfg_ddr_training_delay_ps.dca_dq_tx[1] = 0x00000002,
		.cfg_ddr_training_delay_ps.dca_dq_tx[2] = 0x00000002,
		.cfg_ddr_training_delay_ps.dca_dq_tx[3] = 0x00000002,
		.cfg_ddr_training_delay_ps.dca_dq_tx[4] = 0x00000002,
		.cfg_ddr_training_delay_ps.dca_dq_tx[5] = 0x00000002,
		.cfg_ddr_training_delay_ps.dca_dq_tx[6] = 0x00000002,
		.cfg_ddr_training_delay_ps.dca_dq_tx[7] = 0x00000002,
		.cfg_ddr_training_delay_ps.dfi_mrl[0] = 0x00000004,
		.cfg_ddr_training_delay_ps.dfi_mrl[1] = 0x00000004,
		.cfg_ddr_training_delay_ps.dfi_mrl[2] = 0x00000004,
		.cfg_ddr_training_delay_ps.dfi_mrl[3] = 0x00000004,
		.cfg_ddr_training_delay_ps.dfi_hwtmrl = 0x00000004,
		.cfg_ddr_training_delay_ps.csr_hwtctrl = 0x00000004,
		.cfg_ddr_training_delay_ps.pptdqscnttg0[0] = 0x00000026,
		.cfg_ddr_training_delay_ps.pptdqscnttg0[1] = 0x00000026,
		.cfg_ddr_training_delay_ps.pptdqscnttg0[2] = 0x00000026,
		.cfg_ddr_training_delay_ps.pptdqscnttg0[3] = 0x00000026,
		.cfg_ddr_training_delay_ps.pptdqscnttg1[0] = 0x00000000,
		.cfg_ddr_training_delay_ps.pptdqscnttg1[1] = 0x00000000,
		.cfg_ddr_training_delay_ps.pptdqscnttg1[2] = 0x00000000,
		.cfg_ddr_training_delay_ps.pptdqscnttg1[3] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg0[0] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg0[1] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg0[2] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg0[3] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg1[0] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg1[1] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg1[2] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg1[3] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[0][0] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[1][0] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[2][0] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[3][0] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[0][1] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[1][1] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[2][1] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[3][1] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[0][2] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[1][2] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[2][2] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[3][2] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[0][3] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[1][3] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[2][3] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[3][3] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[0][4] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[1][4] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[2][4] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[3][4] = 0x00000000,
	},
#endif//end BA409_BOARD

#if ENABLE_A4_SKT_DDR3_BOARD
	{
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs =
			sizeof(ddr_set_ps0_only_t) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,
		.cfg_board_common_setting.fast_boot = {
			0, 0, 0, 0
		},
		.cfg_board_common_setting.ddr_func = 0,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_DDR3,
		.cfg_board_common_setting.enable_lpddr4x_mode = 0,
		//.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_16BIT_CH0,
		//.cfg_board_common_setting.dram_cs0_base_add = 0,
		//.cfg_board_common_setting.dram_cs1_base_add = 0,
		.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX1 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.DisabledDbyte[0] = 0xfc,
		//bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
		.cfg_board_common_setting.DisabledDbyte[1] = 0xfc,
		//bit 0 -3 ch1 cs0 ,bit 4-7 ch1
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		.cfg_board_common_setting.log_level = 4,
		//4,//LOG_LEVEL_BASIC,
		.cfg_board_common_setting.dbi_enable = DDR_WRITE_READ_DBI_DISABLE,
		.cfg_board_common_setting.pll_ssc_mode =
			(1 << 20) | (1 << 8) | (2 << 4) | 0,
		//center_ssc_1000ppm,//SSC_DISABLE,(1 << 20) | (0 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test = {
			0
		},
		//.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR4_32BIT_T3X,
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR3_16BIT,
		//DDR_DMC_REMAP_DDR4_32BIT,
		//DDR_DMC_REMAP_DDR4_32BIT,
		// .cfg_board_common_setting.ac_pinmux = {
		//2, 3, 1, 0, 5, 4, 0, 0, 0, 0, 1, 3, 5, 2, 4, 0 },
		.cfg_board_common_setting.ddr_dqs_swap = 0,

		.cfg_board_common_setting.ddr_vddee_setting = {
			0
		},
		.cfg_board_SI_setting_ps.DRAMFreq = DDR3_CLK_DDR0,
		//.cfg_ddr_training_delay_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		.cfg_board_SI_setting_ps.dfi_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		//.cfg_ddr_training_delay_ps.dfe_offset = 0,
		.cfg_board_SI_setting_ps.vref_ac_permil = 0,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,
		//real chip stk lp4
		.cfg_board_common_setting.ddr_rfc_type = DDR_RFC_TYPE_DDR3_512Mbx8,
		.cfg_board_common_setting.pll_ssc_mode = 0x00000000,

#ifdef CONFIG_BOARD_TIMMING     //skt lp4 board
#define TDQS2DQ  ((0 * 128 * DDR3_CLK_DDR0) / 1000000)
//#define BOARD_DQS_DELAY   64+32
#define TDQSCK_ADD_DQS   ((0 * 128 * DDR3_CLK_DDR0) / 1000000) //clk should use 64 steps
#define CLK_DELAY  (0)
#define PHY_TDQS2DQ  ((0 * 128 * DDR3_CLK_DDR0) / 1000000)
#define LEVEL_DELAY  ((50 * 128 * DDR3_CLK_DDR0 * 2) / 1000000)
//for 1584M 100 STEP 250ps //((480 * 128 * DDR3_CLK_DDR0 * 2) / 1000000)
#endif
		.cfg_board_SI_setting_ps.clk_drv_ohm = 40,
		.cfg_board_SI_setting_ps.cs_drv_ohm = 40,
		.cfg_board_SI_setting_ps.ac_drv_ohm = 40,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = 40,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = 40,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = 120,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = 120,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = 40,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = 120,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = 0,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = 120,
		.cfg_board_SI_setting_ps.dram_drv_pull_up_cal_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_range =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,

		.cfg_board_common_setting.ac_pinmux = {
			0xb,//11,0x6c
			0x13,//19,0x6d
			0x8,//8,0x6e
			0x1a,//26,0x6f
			0x5,//5,0x70
			0x6,//6,0x71
			0xd,//13,0x72
			0x1,//1,0x73
			0x16,//22,0x74
			0xe,//14,0x75
			0x7,//7,0x76
			0x17,//23,0x77
			0x4,//4,0x78
			0x1b,//27,0x79
			0x10,//16,0x7a
			0x14,//20,0x7b
			0xc,//12,0x7c
			0x0,//0,0x7d
			0x15,//21,0x7e
			0xa,//10,0x7f
			0x9,//9,0x80
			0x11,//17,0x81
			0x12,//18,0x82
			0x13,//19,0x83
			0x2,//2,0x84
			0x3,//3,0x85
			0x18,//24,0x86
			0x19,//25,0x87
			0x1c,//28,0x88
			0x1d,//29,0x89
		},
		.cfg_board_common_setting.ddr_dq_remap = {
			12,
			33,
			10,
			8,
			14,
			9,
			11,
			13,
			15,
			0,
			2,
			32,
			4,
			1,
			5,
			6,
			3,
			7,
			21,
			22,
			23,
			18,
			17,
			16,
			34,
			19,
			20,
			24,
			25,
			35,
			27,
			28,
			26,
			31,
			30,
			29,
		},

		.cfg_ddr_training_delay_ps.reserve_para = {
			//cs0 write dqs,lane0-lane3
			(1 << 7) | 8, (1 << 7) | 8, (0 << 7) | 0, (0 << 7) | 0,
			//cs1 write dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs0 read dqs,lane0-lane3
			(1 << 7) | 12, (1 << 7) | 8, (0 << 7) | 0, (0 << 7) | 0,
			//cs1 read dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		},

#define  AC_OFFSET  (0)
		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 256 + AC_OFFSET - 64,
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 256 + AC_OFFSET,
		//cke 128 only 1UI margin
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 128 + AC_OFFSET,         // cs
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 128 + AC_OFFSET,        // cs
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 128 + AC_OFFSET + 64,      //ck
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 128 + AC_OFFSET + 64,      //ck
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 128 + AC_OFFSET,            //cke
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 128 + AC_OFFSET,            //cke
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 128 + AC_OFFSET,               //odt0
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 128 + AC_OFFSET,              //odt1

		.cfg_ddr_training_delay_ps.read_dq_delay_t[0] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[1] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[2] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[3] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[4] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[5] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[6] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[7] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[8] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[9] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[10] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[11] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[12] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[13] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[14] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[15] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[16] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[17] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[18] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[19] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[20] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[21] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[22] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[23] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[24] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[25] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[26] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[27] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[28] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[29] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[30] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[31] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[32] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[33] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[34] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[35] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[36] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[37] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[38] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[39] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[40] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[41] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[42] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[43] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[44] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[45] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[46] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[47] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[48] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[49] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[50] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[51] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[52] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[53] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[54] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[55] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[56] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[57] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[58] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[59] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[60] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[61] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[62] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[63] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[64] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[65] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[66] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[67] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[68] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[69] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[70] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[71] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[0] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[1] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[2] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[3] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[4] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[5] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[6] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[7] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[8] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[9] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[10] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[11] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[12] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[13] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[14] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[15] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[16] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[17] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[18] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[19] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[20] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[21] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[22] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[23] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[24] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[25] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[26] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[27] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[28] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[29] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[30] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[31] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[32] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[33] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[34] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[35] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[36] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[37] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[38] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[39] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[40] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[41] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[42] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[43] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[44] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[45] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[46] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[47] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[48] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[49] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[50] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[51] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[52] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[53] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[54] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[55] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[56] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[57] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[58] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[59] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[60] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[61] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[62] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[63] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[64] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[65] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[66] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[67] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[68] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[69] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[70] = 64,
		.cfg_ddr_training_delay_ps.read_dq_delay_c[71] = 64,

		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = AC_OFFSET + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = AC_OFFSET + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = AC_OFFSET + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = AC_OFFSET + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = AC_OFFSET + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = AC_OFFSET + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = AC_OFFSET + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = AC_OFFSET + LEVEL_DELAY,

		.cfg_ddr_training_delay_ps.write_wck_delay[0] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[1] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[2] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[3] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[4] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[5] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[6] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[7] = 0x00000200,

		.cfg_ddr_training_delay_ps.wdq_delay[0] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[1] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[2] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[3] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[4] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[5] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[6] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[7] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[8] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[9] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[10] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[11] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[12] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[13] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[14] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[15] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[16] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[17] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[18] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[19] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[20] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[21] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[22] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[23] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[24] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[25] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[26] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[27] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[28] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[29] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[30] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[31] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[32] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[33] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[34] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[35] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[36] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[37] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[38] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[39] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[40] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[41] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[42] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[43] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[44] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[45] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[46] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[47] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[48] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[49] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[50] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[51] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[52] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[53] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[54] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[55] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[56] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[57] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[58] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[59] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[60] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[61] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[62] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[63] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[64] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[65] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[66] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[67] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[68] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[69] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[70] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,
		.cfg_ddr_training_delay_ps.wdq_delay[71] = 64 + AC_OFFSET + TDQS2DQ + LEVEL_DELAY,

		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = (128 << 2) + 0 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = (128 << 2) + 0 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = (128 << 2) + 0 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = (128 << 2) + 0 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = (128 << 2) + 0 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = (128 << 2) + 0 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = (128 << 2) + 0 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = (128 << 2) + 0 + AC_OFFSET,

		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 128,

		.cfg_ddr_training_delay_ps.soc_bit_vref0[0] = 0x000000,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[1] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[2] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[3] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[4] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[5] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[6] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[7] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[8] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[9] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[10] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[11] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[12] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[13] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[14] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[15] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[16] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[17] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[18] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[19] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[20] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[21] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[22] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[23] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[24] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[25] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[26] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[27] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[28] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[29] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[30] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[31] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[32] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[33] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[34] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[35] = 0x000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[0] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[1] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[2] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[3] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[4] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[5] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[6] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[7] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[8] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[9] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[10] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[11] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[12] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[13] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[14] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[15] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[16] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[17] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[18] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[19] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[20] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[21] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[22] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[23] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[24] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[25] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[26] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[27] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[28] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[29] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[30] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[31] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[32] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[33] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[34] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref1[35] = 0x0000001a,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[0] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[1] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[2] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[3] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[4] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[5] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[6] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[7] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[8] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[9] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[10] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[11] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[12] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[13] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[14] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[15] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[16] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[17] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[18] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[19] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[20] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[21] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[22] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[23] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[24] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[25] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[26] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[27] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[28] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[29] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[30] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[31] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[32] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[33] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[34] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref2[35] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[0] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[1] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[2] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[3] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[4] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[5] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[6] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[7] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[8] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[9] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[10] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[11] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[12] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[13] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[14] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[15] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[16] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[17] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[18] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[19] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[20] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[21] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[22] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[23] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[24] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[25] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[26] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[27] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[28] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[29] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[30] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[31] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[32] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[33] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[34] = 0x00000030,
		.cfg_ddr_training_delay_ps.soc_bit_vref3[35] = 0x00000030,
		.cfg_ddr_training_delay_ps.dram_vref[0] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[1] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[2] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[3] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[4] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[5] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[6] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[7] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[8] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[9] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[10] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[11] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[12] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[13] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[14] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[15] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[16] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[17] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[18] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[19] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[20] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[21] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[22] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[23] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[24] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[25] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[26] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[27] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[28] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[29] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[30] = 0x00000000,
		.cfg_ddr_training_delay_ps.dram_vref[31] = 0x00000000,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[0] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[1] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[2] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[3] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[4] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[5] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[6] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_t[7] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[0] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[1] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[2] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[3] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[4] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[5] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[6] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_t[7] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[0] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[1] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[2] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[3] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[4] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[5] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[6] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_t[7] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[0] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[1] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[2] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[3] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[4] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[5] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[6] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_tx_c[7] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[0] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[1] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[2] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[3] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[4] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[5] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[6] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_wck_rx_c[7] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[0] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[1] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[2] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[3] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[4] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[5] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[6] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dqs_tx_c[7] = 0x00000266,
		.cfg_ddr_training_delay_ps.dca_dq_tx[0] = 0x00000002,
		.cfg_ddr_training_delay_ps.dca_dq_tx[1] = 0x00000002,
		.cfg_ddr_training_delay_ps.dca_dq_tx[2] = 0x00000002,
		.cfg_ddr_training_delay_ps.dca_dq_tx[3] = 0x00000002,
		.cfg_ddr_training_delay_ps.dca_dq_tx[4] = 0x00000002,
		.cfg_ddr_training_delay_ps.dca_dq_tx[5] = 0x00000002,
		.cfg_ddr_training_delay_ps.dca_dq_tx[6] = 0x00000002,
		.cfg_ddr_training_delay_ps.dca_dq_tx[7] = 0x00000002,
		.cfg_ddr_training_delay_ps.dfi_mrl[0] = 0x00000004,
		.cfg_ddr_training_delay_ps.dfi_mrl[1] = 0x00000004,
		.cfg_ddr_training_delay_ps.dfi_mrl[2] = 0x00000004,
		.cfg_ddr_training_delay_ps.dfi_mrl[3] = 0x00000004,
		.cfg_ddr_training_delay_ps.dfi_hwtmrl = 0x00000004,
		.cfg_ddr_training_delay_ps.csr_hwtctrl = 0x00000004,

		.cfg_ddr_training_delay_ps.pptdqscnttg0[0] = 0x00000026,
		.cfg_ddr_training_delay_ps.pptdqscnttg0[1] = 0x00000026,
		.cfg_ddr_training_delay_ps.pptdqscnttg0[2] = 0x00000026,
		.cfg_ddr_training_delay_ps.pptdqscnttg0[3] = 0x00000026,
		.cfg_ddr_training_delay_ps.pptdqscnttg1[0] = 0x00000000,
		.cfg_ddr_training_delay_ps.pptdqscnttg1[1] = 0x00000000,
		.cfg_ddr_training_delay_ps.pptdqscnttg1[2] = 0x00000000,
		.cfg_ddr_training_delay_ps.pptdqscnttg1[3] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg0[0] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg0[1] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg0[2] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg0[3] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg1[0] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg1[1] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg1[2] = 0x00000000,
		.cfg_ddr_training_delay_ps.PptWck2DqoCntTg1[3] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[0][0] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[1][0] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[2][0] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[3][0] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[0][1] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[1][1] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[2][1] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[3][1] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[0][2] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[1][2] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[2][2] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[3][2] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[0][3] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[1][3] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[2][3] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[3][3] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[0][4] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[1][4] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[2][4] = 0x00000000,
		.cfg_ddr_training_delay_ps.RxReplicaPhase[3][4] = 0x00000000,

		.cfg_board_common_setting.ac_pinmux = {
			19, 14, 18, 26, 3, 0, 5, 9, 2,
			13, 7, 23, 22, 27, 11, 1, 4, 17,
			15, 8, 12, 10, 20, 6, 16, 21, 24, 25, 28, 29,
		},

		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 0x100,//256,0xdc
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 0x100,//256,0xde
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 0x100,//256,0xe0
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 0x80,//128,0xe2
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 0x100,//256,0xe4
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 0x100,//256,0xe6
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 0x100,//256,0xe8
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 0x100,//256,0xea
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 0x80,//128,0xec
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 0x100,//256,0xee
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 0x100,//256,0xf0
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 0x80,//128,0xf2
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 0x100,//256,0xf4
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 0x80,//128,0xf6
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 0x100,//256,0xf8
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 0x100,//256,0xfa
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 0x100,//256,0xfc
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 0x100,//256,0xfe
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 0x100,//256,0x100
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 0x100,//256,0x102
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 0x100,//256,0x104
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 0x100,//256,0x106
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 0x100,//256,0x108
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 0x100,//256,0x10a
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 0x100,//256,0x10c
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 0x100,//256,0x10e
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 0x80,//128,0x110
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 0x80,//128,0x112
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 0xc0,//192,0x114
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 0xc0,//192,0x116
		.cfg_ddr_training_delay_ps.read_dq_delay_t[0] = 0x37,//55,0x118
		.cfg_ddr_training_delay_ps.read_dq_delay_t[1] = 0x3f,//63,0x11a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[2] = 0x45,//69,0x11c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[3] = 0x37,//55,0x11e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[4] = 0x48,//72,0x120
		.cfg_ddr_training_delay_ps.read_dq_delay_t[5] = 0x47,//71,0x122
		.cfg_ddr_training_delay_ps.read_dq_delay_t[6] = 0x41,//65,0x124
		.cfg_ddr_training_delay_ps.read_dq_delay_t[7] = 0x4a,//74,0x126
		.cfg_ddr_training_delay_ps.read_dq_delay_t[8] = 0x3a,//58,0x128
		.cfg_ddr_training_delay_ps.read_dq_delay_t[9] = 0x45,//69,0x12a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[10] = 0x3c,//60,0x12c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[11] = 0x3f,//63,0x12e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[12] = 0x3d,//61,0x130
		.cfg_ddr_training_delay_ps.read_dq_delay_t[13] = 0x55,//85,0x132
		.cfg_ddr_training_delay_ps.read_dq_delay_t[14] = 0x4f,//79,0x134
		.cfg_ddr_training_delay_ps.read_dq_delay_t[15] = 0x39,//57,0x136
		.cfg_ddr_training_delay_ps.read_dq_delay_t[16] = 0x51,//81,0x138
		.cfg_ddr_training_delay_ps.read_dq_delay_t[17] = 0x44,//68,0x13a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[18] = 0x0,//0,0x13c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[19] = 0x0,//0,0x13e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[20] = 0x0,//0,0x140
		.cfg_ddr_training_delay_ps.read_dq_delay_t[21] = 0x0,//0,0x142
		.cfg_ddr_training_delay_ps.read_dq_delay_t[22] = 0x0,//0,0x144
		.cfg_ddr_training_delay_ps.read_dq_delay_t[23] = 0x0,//0,0x146
		.cfg_ddr_training_delay_ps.read_dq_delay_t[24] = 0x0,//0,0x148
		.cfg_ddr_training_delay_ps.read_dq_delay_t[25] = 0x0,//0,0x14a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[26] = 0x0,//0,0x14c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[27] = 0x0,//0,0x14e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[28] = 0x0,//0,0x150
		.cfg_ddr_training_delay_ps.read_dq_delay_t[29] = 0x0,//0,0x152
		.cfg_ddr_training_delay_ps.read_dq_delay_t[30] = 0x0,//0,0x154
		.cfg_ddr_training_delay_ps.read_dq_delay_t[31] = 0x0,//0,0x156
		.cfg_ddr_training_delay_ps.read_dq_delay_t[32] = 0x0,//0,0x158
		.cfg_ddr_training_delay_ps.read_dq_delay_t[33] = 0x0,//0,0x15a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[34] = 0x0,//0,0x15c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[35] = 0x0,//0,0x15e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[36] = 0x0,//0,0x160
		.cfg_ddr_training_delay_ps.read_dq_delay_t[37] = 0x0,//0,0x162
		.cfg_ddr_training_delay_ps.read_dq_delay_t[38] = 0x0,//0,0x164
		.cfg_ddr_training_delay_ps.read_dq_delay_t[39] = 0x0,//0,0x166
		.cfg_ddr_training_delay_ps.read_dq_delay_t[40] = 0x0,//0,0x168
		.cfg_ddr_training_delay_ps.read_dq_delay_t[41] = 0x0,//0,0x16a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[42] = 0x0,//0,0x16c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[43] = 0x0,//0,0x16e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[44] = 0x0,//0,0x170
		.cfg_ddr_training_delay_ps.read_dq_delay_t[45] = 0x0,//0,0x172
		.cfg_ddr_training_delay_ps.read_dq_delay_t[46] = 0x0,//0,0x174
		.cfg_ddr_training_delay_ps.read_dq_delay_t[47] = 0x0,//0,0x176
		.cfg_ddr_training_delay_ps.read_dq_delay_t[48] = 0x0,//0,0x178
		.cfg_ddr_training_delay_ps.read_dq_delay_t[49] = 0x0,//0,0x17a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[50] = 0x0,//0,0x17c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[51] = 0x0,//0,0x17e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[52] = 0x0,//0,0x180
		.cfg_ddr_training_delay_ps.read_dq_delay_t[53] = 0x0,//0,0x182
		.cfg_ddr_training_delay_ps.read_dq_delay_t[54] = 0x0,//0,0x184
		.cfg_ddr_training_delay_ps.read_dq_delay_t[55] = 0x0,//0,0x186
		.cfg_ddr_training_delay_ps.read_dq_delay_t[56] = 0x0,//0,0x188
		.cfg_ddr_training_delay_ps.read_dq_delay_t[57] = 0x0,//0,0x18a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[58] = 0x0,//0,0x18c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[59] = 0x0,//0,0x18e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[60] = 0x0,//0,0x190
		.cfg_ddr_training_delay_ps.read_dq_delay_t[61] = 0x0,//0,0x192
		.cfg_ddr_training_delay_ps.read_dq_delay_t[62] = 0x0,//0,0x194
		.cfg_ddr_training_delay_ps.read_dq_delay_t[63] = 0x0,//0,0x196
		.cfg_ddr_training_delay_ps.read_dq_delay_t[64] = 0x0,//0,0x198
		.cfg_ddr_training_delay_ps.read_dq_delay_t[65] = 0x0,//0,0x19a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[66] = 0x0,//0,0x19c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[67] = 0x0,//0,0x19e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[68] = 0x0,//0,0x1a0
		.cfg_ddr_training_delay_ps.read_dq_delay_t[69] = 0x0,//0,0x1a2
		.cfg_ddr_training_delay_ps.read_dq_delay_t[70] = 0x0,//0,0x1a4
		.cfg_ddr_training_delay_ps.read_dq_delay_t[71] = 0x0,//0,0x1a6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[0] = 0x0,//0,0x1a8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[1] = 0x0,//0,0x1aa
		.cfg_ddr_training_delay_ps.read_dq_delay_c[2] = 0x0,//0,0x1ac
		.cfg_ddr_training_delay_ps.read_dq_delay_c[3] = 0x0,//0,0x1ae
		.cfg_ddr_training_delay_ps.read_dq_delay_c[4] = 0x0,//0,0x1b0
		.cfg_ddr_training_delay_ps.read_dq_delay_c[5] = 0x0,//0,0x1b2
		.cfg_ddr_training_delay_ps.read_dq_delay_c[6] = 0x0,//0,0x1b4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[7] = 0x0,//0,0x1b6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[8] = 0x0,//0,0x1b8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[9] = 0x0,//0,0x1ba
		.cfg_ddr_training_delay_ps.read_dq_delay_c[10] = 0x0,//0,0x1bc
		.cfg_ddr_training_delay_ps.read_dq_delay_c[11] = 0x0,//0,0x1be
		.cfg_ddr_training_delay_ps.read_dq_delay_c[12] = 0x0,//0,0x1c0
		.cfg_ddr_training_delay_ps.read_dq_delay_c[13] = 0x0,//0,0x1c2
		.cfg_ddr_training_delay_ps.read_dq_delay_c[14] = 0x0,//0,0x1c4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[15] = 0x0,//0,0x1c6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[16] = 0x0,//0,0x1c8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[17] = 0x0,//0,0x1ca
		.cfg_ddr_training_delay_ps.read_dq_delay_c[18] = 0x0,//0,0x1cc
		.cfg_ddr_training_delay_ps.read_dq_delay_c[19] = 0x0,//0,0x1ce
		.cfg_ddr_training_delay_ps.read_dq_delay_c[20] = 0x0,//0,0x1d0
		.cfg_ddr_training_delay_ps.read_dq_delay_c[21] = 0x0,//0,0x1d2
		.cfg_ddr_training_delay_ps.read_dq_delay_c[22] = 0x0,//0,0x1d4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[23] = 0x0,//0,0x1d6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[24] = 0x0,//0,0x1d8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[25] = 0x0,//0,0x1da
		.cfg_ddr_training_delay_ps.read_dq_delay_c[26] = 0x0,//0,0x1dc
		.cfg_ddr_training_delay_ps.read_dq_delay_c[27] = 0x0,//0,0x1de
		.cfg_ddr_training_delay_ps.read_dq_delay_c[28] = 0x0,//0,0x1e0
		.cfg_ddr_training_delay_ps.read_dq_delay_c[29] = 0x0,//0,0x1e2
		.cfg_ddr_training_delay_ps.read_dq_delay_c[30] = 0x0,//0,0x1e4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[31] = 0x0,//0,0x1e6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[32] = 0x0,//0,0x1e8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[33] = 0x0,//0,0x1ea
		.cfg_ddr_training_delay_ps.read_dq_delay_c[34] = 0x0,//0,0x1ec
		.cfg_ddr_training_delay_ps.read_dq_delay_c[35] = 0x0,//0,0x1ee
		.cfg_ddr_training_delay_ps.read_dq_delay_c[36] = 0x0,//0,0x1f0
		.cfg_ddr_training_delay_ps.read_dq_delay_c[37] = 0x0,//0,0x1f2
		.cfg_ddr_training_delay_ps.read_dq_delay_c[38] = 0x0,//0,0x1f4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[39] = 0x0,//0,0x1f6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[40] = 0x0,//0,0x1f8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[41] = 0x0,//0,0x1fa
		.cfg_ddr_training_delay_ps.read_dq_delay_c[42] = 0x0,//0,0x1fc
		.cfg_ddr_training_delay_ps.read_dq_delay_c[43] = 0x0,//0,0x1fe
		.cfg_ddr_training_delay_ps.read_dq_delay_c[44] = 0x0,//0,0x200
		.cfg_ddr_training_delay_ps.read_dq_delay_c[45] = 0x0,//0,0x202
		.cfg_ddr_training_delay_ps.read_dq_delay_c[46] = 0x0,//0,0x204
		.cfg_ddr_training_delay_ps.read_dq_delay_c[47] = 0x0,//0,0x206
		.cfg_ddr_training_delay_ps.read_dq_delay_c[48] = 0x0,//0,0x208
		.cfg_ddr_training_delay_ps.read_dq_delay_c[49] = 0x0,//0,0x20a
		.cfg_ddr_training_delay_ps.read_dq_delay_c[50] = 0x0,//0,0x20c
		.cfg_ddr_training_delay_ps.read_dq_delay_c[51] = 0x0,//0,0x20e
		.cfg_ddr_training_delay_ps.read_dq_delay_c[52] = 0x0,//0,0x210
		.cfg_ddr_training_delay_ps.read_dq_delay_c[53] = 0x0,//0,0x212
		.cfg_ddr_training_delay_ps.read_dq_delay_c[54] = 0x0,//0,0x214
		.cfg_ddr_training_delay_ps.read_dq_delay_c[55] = 0x0,//0,0x216
		.cfg_ddr_training_delay_ps.read_dq_delay_c[56] = 0x0,//0,0x218
		.cfg_ddr_training_delay_ps.read_dq_delay_c[57] = 0x0,//0,0x21a
		.cfg_ddr_training_delay_ps.read_dq_delay_c[58] = 0x0,//0,0x21c
		.cfg_ddr_training_delay_ps.read_dq_delay_c[59] = 0x0,//0,0x21e
		.cfg_ddr_training_delay_ps.read_dq_delay_c[60] = 0x0,//0,0x220
		.cfg_ddr_training_delay_ps.read_dq_delay_c[61] = 0x0,//0,0x222
		.cfg_ddr_training_delay_ps.read_dq_delay_c[62] = 0x0,//0,0x224
		.cfg_ddr_training_delay_ps.read_dq_delay_c[63] = 0x0,//0,0x226
		.cfg_ddr_training_delay_ps.read_dq_delay_c[64] = 0x0,//0,0x228
		.cfg_ddr_training_delay_ps.read_dq_delay_c[65] = 0x0,//0,0x22a
		.cfg_ddr_training_delay_ps.read_dq_delay_c[66] = 0x0,//0,0x22c
		.cfg_ddr_training_delay_ps.read_dq_delay_c[67] = 0x0,//0,0x22e
		.cfg_ddr_training_delay_ps.read_dq_delay_c[68] = 0x0,//0,0x230
		.cfg_ddr_training_delay_ps.read_dq_delay_c[69] = 0x0,//0,0x232
		.cfg_ddr_training_delay_ps.read_dq_delay_c[70] = 0x0,//0,0x234
		.cfg_ddr_training_delay_ps.read_dq_delay_c[71] = 0x0,//0,0x236
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x60,//96,0x238
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x60,//96,0x23a
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x0,//0,0x23c
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x0,//0,0x23e
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x70,//112,0x240
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x70,//112,0x242
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x0,//0,0x244
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x0,//0,0x246
		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x5d,//93,0x248
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x56,//86,0x24a
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x0,//0,0x24c
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0x0,//0,0x24e
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x100,//256,0x250
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x100,//256,0x252
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x0,//0,0x254
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x0,//0,0x256
		.cfg_ddr_training_delay_ps.write_wck_delay[0] = 0x0,//0,0x258
		.cfg_ddr_training_delay_ps.write_wck_delay[1] = 0x0,//0,0x25a
		.cfg_ddr_training_delay_ps.write_wck_delay[2] = 0x0,//0,0x25c
		.cfg_ddr_training_delay_ps.write_wck_delay[3] = 0x0,//0,0x25e
		.cfg_ddr_training_delay_ps.write_wck_delay[4] = 0x0,//0,0x260
		.cfg_ddr_training_delay_ps.write_wck_delay[5] = 0x0,//0,0x262
		.cfg_ddr_training_delay_ps.write_wck_delay[6] = 0x0,//0,0x264
		.cfg_ddr_training_delay_ps.write_wck_delay[7] = 0x0,//0,0x266
		.cfg_ddr_training_delay_ps.wdq_delay[0] = 0x85,//133,0x268
		.cfg_ddr_training_delay_ps.wdq_delay[1] = 0x90,//144,0x26a
		.cfg_ddr_training_delay_ps.wdq_delay[2] = 0x8e,//142,0x26c
		.cfg_ddr_training_delay_ps.wdq_delay[3] = 0x8f,//143,0x26e
		.cfg_ddr_training_delay_ps.wdq_delay[4] = 0x91,//145,0x270
		.cfg_ddr_training_delay_ps.wdq_delay[5] = 0x96,//150,0x272
		.cfg_ddr_training_delay_ps.wdq_delay[6] = 0x97,//151,0x274
		.cfg_ddr_training_delay_ps.wdq_delay[7] = 0x94,//148,0x276
		.cfg_ddr_training_delay_ps.wdq_delay[8] = 0x8f,//143,0x278
		.cfg_ddr_training_delay_ps.wdq_delay[9] = 0x8e,//142,0x27a
		.cfg_ddr_training_delay_ps.wdq_delay[10] = 0x8b,//139,0x27c
		.cfg_ddr_training_delay_ps.wdq_delay[11] = 0x91,//145,0x27e
		.cfg_ddr_training_delay_ps.wdq_delay[12] = 0x8a,//138,0x280
		.cfg_ddr_training_delay_ps.wdq_delay[13] = 0xa3,//163,0x282
		.cfg_ddr_training_delay_ps.wdq_delay[14] = 0x97,//151,0x284
		.cfg_ddr_training_delay_ps.wdq_delay[15] = 0x85,//133,0x286
		.cfg_ddr_training_delay_ps.wdq_delay[16] = 0x9b,//155,0x288
		.cfg_ddr_training_delay_ps.wdq_delay[17] = 0x90,//144,0x28a
		.cfg_ddr_training_delay_ps.wdq_delay[18] = 0x0,//0,0x28c
		.cfg_ddr_training_delay_ps.wdq_delay[19] = 0x0,//0,0x28e
		.cfg_ddr_training_delay_ps.wdq_delay[20] = 0x0,//0,0x290
		.cfg_ddr_training_delay_ps.wdq_delay[21] = 0x0,//0,0x292
		.cfg_ddr_training_delay_ps.wdq_delay[22] = 0x0,//0,0x294
		.cfg_ddr_training_delay_ps.wdq_delay[23] = 0x0,//0,0x296
		.cfg_ddr_training_delay_ps.wdq_delay[24] = 0x0,//0,0x298
		.cfg_ddr_training_delay_ps.wdq_delay[25] = 0x0,//0,0x29a
		.cfg_ddr_training_delay_ps.wdq_delay[26] = 0x0,//0,0x29c
		.cfg_ddr_training_delay_ps.wdq_delay[27] = 0x0,//0,0x29e
		.cfg_ddr_training_delay_ps.wdq_delay[28] = 0x0,//0,0x2a0
		.cfg_ddr_training_delay_ps.wdq_delay[29] = 0x0,//0,0x2a2
		.cfg_ddr_training_delay_ps.wdq_delay[30] = 0x0,//0,0x2a4
		.cfg_ddr_training_delay_ps.wdq_delay[31] = 0x0,//0,0x2a6
		.cfg_ddr_training_delay_ps.wdq_delay[32] = 0x0,//0,0x2a8
		.cfg_ddr_training_delay_ps.wdq_delay[33] = 0x0,//0,0x2aa
		.cfg_ddr_training_delay_ps.wdq_delay[34] = 0x0,//0,0x2ac
		.cfg_ddr_training_delay_ps.wdq_delay[35] = 0x0,//0,0x2ae
		.cfg_ddr_training_delay_ps.wdq_delay[36] = 0x0,//0,0x2b0
		.cfg_ddr_training_delay_ps.wdq_delay[37] = 0x0,//0,0x2b2
		.cfg_ddr_training_delay_ps.wdq_delay[38] = 0x0,//0,0x2b4
		.cfg_ddr_training_delay_ps.wdq_delay[39] = 0x0,//0,0x2b6
		.cfg_ddr_training_delay_ps.wdq_delay[40] = 0x0,//0,0x2b8
		.cfg_ddr_training_delay_ps.wdq_delay[41] = 0x0,//0,0x2ba
		.cfg_ddr_training_delay_ps.wdq_delay[42] = 0x0,//0,0x2bc
		.cfg_ddr_training_delay_ps.wdq_delay[43] = 0x0,//0,0x2be
		.cfg_ddr_training_delay_ps.wdq_delay[44] = 0x0,//0,0x2c0
		.cfg_ddr_training_delay_ps.wdq_delay[45] = 0x0,//0,0x2c2
		.cfg_ddr_training_delay_ps.wdq_delay[46] = 0x0,//0,0x2c4
		.cfg_ddr_training_delay_ps.wdq_delay[47] = 0x0,//0,0x2c6
		.cfg_ddr_training_delay_ps.wdq_delay[48] = 0x0,//0,0x2c8
		.cfg_ddr_training_delay_ps.wdq_delay[49] = 0x0,//0,0x2ca
		.cfg_ddr_training_delay_ps.wdq_delay[50] = 0x0,//0,0x2cc
		.cfg_ddr_training_delay_ps.wdq_delay[51] = 0x0,//0,0x2ce
		.cfg_ddr_training_delay_ps.wdq_delay[52] = 0x0,//0,0x2d0
		.cfg_ddr_training_delay_ps.wdq_delay[53] = 0x0,//0,0x2d2
		.cfg_ddr_training_delay_ps.wdq_delay[54] = 0x0,//0,0x2d4
		.cfg_ddr_training_delay_ps.wdq_delay[55] = 0x0,//0,0x2d6
		.cfg_ddr_training_delay_ps.wdq_delay[56] = 0x0,//0,0x2d8
		.cfg_ddr_training_delay_ps.wdq_delay[57] = 0x0,//0,0x2da
		.cfg_ddr_training_delay_ps.wdq_delay[58] = 0x0,//0,0x2dc
		.cfg_ddr_training_delay_ps.wdq_delay[59] = 0x0,//0,0x2de
		.cfg_ddr_training_delay_ps.wdq_delay[60] = 0x0,//0,0x2e0
		.cfg_ddr_training_delay_ps.wdq_delay[61] = 0x0,//0,0x2e2
		.cfg_ddr_training_delay_ps.wdq_delay[62] = 0x0,//0,0x2e4
		.cfg_ddr_training_delay_ps.wdq_delay[63] = 0x0,//0,0x2e6
		.cfg_ddr_training_delay_ps.wdq_delay[64] = 0x0,//0,0x2e8
		.cfg_ddr_training_delay_ps.wdq_delay[65] = 0x0,//0,0x2ea
		.cfg_ddr_training_delay_ps.wdq_delay[66] = 0x0,//0,0x2ec
		.cfg_ddr_training_delay_ps.wdq_delay[67] = 0x0,//0,0x2ee
		.cfg_ddr_training_delay_ps.wdq_delay[68] = 0x0,//0,0x2f0
		.cfg_ddr_training_delay_ps.wdq_delay[69] = 0x0,//0,0x2f2
		.cfg_ddr_training_delay_ps.wdq_delay[70] = 0x0,//0,0x2f4
		.cfg_ddr_training_delay_ps.wdq_delay[71] = 0x0,//0,0x2f6

	},
#endif//end BA409_BOARD
};

//bit 6 adc_channel bit 0-5 adc value,chan 3 value 8 is layer 2
#define DDR_ID_ACS_ADC   ((3 << 6) | (8))

#define DDR_RESV_CHECK_ID_ENABLE  0Xfe
#define SAR_ADC_DDR_ID_BASE   0
#define SAR_ADC_DDR_ID_STEP   80

#define DDR_TIMMING_OFFSET(X) (unsigned int)(unsigned long)(&(((ddr_set_ps0_only_t *)(0))->(X)))
#define DDR_TIMMING_OFFSET_SIZE(X) sizeof(((ddr_set_ps0_only_t *)(0))->(X))
#define DDR_TIMMING_TUNE_TIMMING0(DDR_ID, PARA, VALUE) (DDR_ID, \
DDR_TIMMING_OFFSET(PARA), (VALUE), DDR_TIMMING_OFFSET_SIZE(PARA), 0, \
DDR_RESV_CHECK_ID_ENABLE)
#define DDR_TIMMING_TUNE_TIMMING1(DDR_ID, PARA, VALUE) (DDR_ID, \
(sizeof(ddr_set_t) + (DDR_TIMMING_OFFSET(PARA))), (VALUE), DDR_TIMMING_OFFSET_SIZE(PARA), \
0, DDR_RESV_CHECK_ID_ENABLE)

//bit24-31 define ID and size
#define DDR_ID_FROM_EFUSE  (0Xff << 24)
#define DDR_ID_FROM_ADC  (0Xfe << 24)
#define DDR_ID_FROM_GPIO_CONFIG1  (0Xfd << 24)
#define DDR_ID_FROM_EFUSE_F  (0Xff << 0)
#define DDR_ID_FROM_ADC_F  (0Xfe << 0)
#define DDR_ID_FROM_GPIO_CONFIG1_F  (0Xfd << 0)
#define DDR_ID_FROM_ADC_MULT (0Xfc000000)
#define DDR_ID_FROM_ADC_MULT_F   (0Xfc << 0)
#define DDR_ID_START_MASK  (0XFFDDCCBB)

#define DDR_ADC_CH0 (0X0 << 5)
#define DDR_ADC_CH1 (0X1 << 5)
#define DDR_ADC_CH2 (0X2 << 5)
#define DDR_ADC_CH3 (0X3 << 5)
#define DDR_ADC_CH4 (0X4 << 5)

#define DDR_ADC_VALUE0 (0X0 << 0)
#define DDR_ADC_VALUE1 (0X1 << 0)
#define DDR_ADC_VALUE2 (0X2 << 0)
#define DDR_ADC_VALUE3 (0X3 << 0)
#define DDR_ADC_VALUE4 (0X4 << 0)
#define DDR_ADC_VALUE5 (0X5 << 0)
#define DDR_ADC_VALUE6 (0X6 << 0)
#define DDR_ADC_VALUE7 (0X7 << 0)
#define DDR_ADC_VALUE8 (0X8 << 0)
#define DDR_ADC_VALUE9 (0X9 << 0)
#define DDR_ADC_VALUE10 (0Xa << 0)
#define DDR_ADC_VALUE11 (0Xb << 0)
#define DDR_ADC_VALUE12 (0Xc << 0)
#define DDR_ADC_VALUE13 (0Xd << 0)
#define DDR_ADC_VALUE14 (0Xe << 0)
#define DDR_ADC_VALUE15 (0Xf << 0)
#define V0  (0X0 << 0)
#define V1  (0X1 << 0)
#define V2  (0X2 << 0)
#define V3  (0X3 << 0)
#define V4  (0X4 << 0)
#define V5  (0X5 << 0)
#define V6  (0X6 << 0)
#define V7  (0X7 << 0)
#define V8  (0X8 << 0)
#define V9  (0X9 << 0)
#define V10  (0Xa << 0)
#define V11  (0Xb << 0)
#define V12  (0Xc << 0)

#define VX  (0Xf << 0)

typedef struct ddr_para_data {
	// start from	DDR_ID_START_MASK,ddr_id;//bit0-23
	// ddr_id value,bit 24-31 ddr_id source  ,0xfe source
	// from adc ,0xfd source from gpio_default_config
	// reg_offset
	// //bit 0-15 parameter offset value,bit16-23 overrid
	// size,bit24-31 mux ddr_id source unsigned int
	// reg_offset; unsigned int	value;
	uint32_t	value : 16;             // bit0-15 only support data size =1byte
	// or 2bytes,no support int value
	uint32_t	reg_offset : 12;        // bit16-27
	uint32_t	data_size : 4;          // bit28-31 if data size =15,then
	// will mean DDR_ID start
} ddr_para_data_t;

typedef struct ddr_para_data_start {
	uint32_t	id_value : 24;          // bit0-23  efuse id or ddr id
	// uint32_t	id_adc_ch : 2;//bit6-7
	uint32_t	id_src_from : 8;        // bit24-31 ddr id from adc or gpio
} ddr_para_data_start_t;

#define DDR_TIMMING_TUNE_STRUCT_SIZE(a)  sizeof(a)

#define DDR_TIMMING_TUNE_TIMMING0_F(PARA, VALUE) (((DDR_TIMMING_OFFSET(PARA)) << 16) |\
((DDR_TIMMING_OFFSET_SIZE(PARA)) << 28) | (VALUE))
#define DDR_TIMMING_TUNE_TIMMING1_F(PARA, VALUE) (((sizeof(ddr_set_ps0_only_t) +\
DDR_TIMMING_OFFSET(PARA)) << 16) | ((DDR_TIMMING_OFFSET_SIZE(PARA)) << 28) | (VALUE))

#define DDR_TIMMING_TUNE_START(id_src_from, id_adc_ch, id_value) {(id_src_from) |\
(id_adc_ch) | (id_value)}
#define DDR_TIMMING_TUNE_ADC_MULT_START(id_value, ch0, ch1, ch2, ch3, ch4, ch5) ((id_value) |\
(ch0) | ((ch1) << 4) | ((ch2) << 8) | ((ch3) << 12) | ((ch4) << 16) | ((ch5) << 20))

//#if 1
uint32_t __bl2_ddr_reg_data[] __attribute__ ((section(".ddr_2acs_data"))) = {
	DDR_ID_START_MASK,
	//DDR_TIMMING_TUNE_ADC_MULT_START(DDR_ID_FROM_ADC_MULT, V12, V1, VX, VX, VX, VX),
	//data start
	//DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_common_setting.dram_ch0_size_MB,
	//		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
	//		(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
	//		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
	//		(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET)),
	//DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_SI_setting_ps.DRAMFreq, 1176),
	//DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_SI_setting_ps.DRAMFreq, 1200),
};

bl2_reg_t __bl2_reg[] __attribute__ ((section(".generic_param"))) = {
	{ 0, 0, 0xffffffff, 0, 0, 0 },
};

// #endif
