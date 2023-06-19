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

#define  CACLU_CLK_DDR0   1464
#define  CACLU_CLK_DDR1   1512       // 600 //1176 //1320 //1464 //1584 //1800 //792 //912 //1008
#define  CONFIG_BOARD_TIMMING

#define ENABLE_T5M_SKT_BOARD 1  //SKT AY309 6layer 4pcs ddr4
//#define ENABLE_T5M_REF_BOARD 1  //REF AY301 2layer 1pcs ddr4

#if ENABLE_T5M_SKT_BOARD    //timing_config,SKT AY309 6layer 1pcs ddr4
ddr_set_ps0_only_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {
	{//start ddr0
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs = 0,
		//sizeof(__ddr_setting) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,
		.cfg_board_common_setting.fast_boot = { 0					       },
		.cfg_board_common_setting.ddr_func = 0,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_DDR4,
		.cfg_board_common_setting.enable_lpddr4x_mode = 0,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
		//.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_16BIT_CH0,
		//.cfg_board_common_setting.dram_cs0_base_add = 0,
		//.cfg_board_common_setting.dram_cs1_base_add = 0,
		.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.DisabledDbyte[0] = 0xf0,
		//bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
		.cfg_board_common_setting.DisabledDbyte[1] = 0xf0,
		//bit 0 -3 ch1 cs0 ,bit 4-7 ch1
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		//log_level = 0xff,boot few log,can test ddr window and max frequency;
		//log_level = 4,training log;
		.cfg_board_common_setting.log_level = 4,
		//4,//LOG_LEVEL_BASIC,
		.cfg_board_common_setting.dbi_enable = DDR_WRITE_READ_DBI_DISABLE,

		//CNTL1 bit23=0 disable SSC,cfg_board_common_setting.pll_ssc_mode = 0,
		//center_ssc_1000ppm,(1 << 29) | (1 << 8) | (2 << 4) | 0
		//CNTL1 bit23=1 enable SSC,
		//CNTL2
		//bit29, 0-29.5k, 1-31.5k, 2-50k
		//bit[11:8], 1-x500ppm, 2-x1000ppm, 3-x1500ppm
		//bit4, 1-12,ssc value = bit[7:4] * bit[11:8];
		//bit0, 0-center_ssc, 1-up_ssc, 1-down_ssc
		.cfg_board_common_setting.pll_ssc_mode = (1 << 29) | (1 << 8) | (2 << 4) | 0,

		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test = { 0 },
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR4_32BIT,
		.cfg_board_common_setting.ddr_dqs_swap = 0,

		.cfg_board_common_setting.ddr_vddee_setting = {
			0
		},
		.cfg_board_SI_setting_ps.DRAMFreq = CACLU_CLK_DDR0,
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

		.cfg_board_common_setting.dbi_enable = 0x00000000,
		.cfg_board_common_setting.ddr_rfc_type = DDR_RFC_TYPE_DDR4_2Gbx8,

		#ifdef CONFIG_BOARD_TIMMING     //skt lp4 board
		#define TDQS2DQ  ((0 * 128 * CACLU_CLK_DDR1) / 1000000)
		//#define BOARD_DQS_DELAY   64+32
		//clk should use 64 steps
		#define TDQSCK_ADD_DQS   ((0 * 128 * CACLU_CLK_DDR1) / 1000000)
		#define CLK_DELAY  (0)
		#define PHY_TDQS2DQ  ((0 * 128 * CACLU_CLK_DDR1) / 1000000)
		#define WRITE_LEVELING_DELAY  0 //((480 * 128 * CACLU_CLK * 2) / 1000000)
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
		//.cfg_ddr_training_delay_ps.dfe_offset = 0,

		.cfg_board_common_setting.ac_pinmux = {
			3,  6,	5,  0,	1,  8,	9,  7,	13, 2,
			11, 14, 4,  15, 10, 26, 27, 20, 23, 22, 21,
			19, 16, 18, 12, 17, 24, 25, 28, 29,
		},

		.cfg_board_common_setting.ddr_dq_remap = {
			0, 1, 2, 3, 4, 5, 6, 32, 7,
			8, 9, 10, 11, 12, 13, 14, 33, 15,
			16, 17, 18, 19, 20, 21, 22, 23, 34,
			24, 25, 26, 27, 28, 29, 30, 31, 35,
		},
		//.cfg_board_common_setting.ddr_dq_remap= {
		// 3, 0, 2, 1, 4, 6, 5, 7,  14, 12, 13, 15,
		.cfg_ddr_training_delay_ps.reserve_para = {
			//cs0 write dqs,lane0-lane3
			(1 << 7) | 8, (1 << 7) | 8, (1 << 7) | 8, (1 << 7) | 8,
			//cs1 write dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs0 read dqs,lane0-lane3
			(1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16,
			//cs1 read dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		},
//		.cfg_ddr_training_delay_ps.reserve_para[0] = (1 << 7) | 0x8,//write dqs
//		.cfg_ddr_training_delay_ps.reserve_para[1] = (1 << 7) | 0x8,//write dqs
//		.cfg_ddr_training_delay_ps.reserve_para[2] = (1 << 7) | 0x8,//write dqs
//		.cfg_ddr_training_delay_ps.reserve_para[3] = (1 << 7) | 0x8,//write dqs
//		.cfg_ddr_training_delay_ps.reserve_para[8 + 0] = (1 << 7) | 0x10,//read dqs
//		.cfg_ddr_training_delay_ps.reserve_para[8 + 1] = (1 << 7) | 0x10,//read dqs
//		.cfg_ddr_training_delay_ps.reserve_para[8 + 2] = (1 << 7) | 0x10,//read dqs
//		.cfg_ddr_training_delay_ps.reserve_para[8 + 3] = (1 << 7) | 0x10,//read dqs
		//.cfg_ddr_training_delay_ps.dac_offset[0] = 0,//(1 << 7) | 0x10,
		//.cfg_ddr_training_delay_ps.dac_offset[1] = 0,//(0 << 7) | 0x10,
		//.cfg_ddr_training_delay_ps.dac_offset[0] = (1 << 7) | 0x5,
		//.cfg_ddr_training_delay_ps.dac_offset[1] = (0 << 7) | 0x5,
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

		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 256 + 0,        // cs
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 256 + 0,        // cs
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 128 + 128,      //ck
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 128 + 128,      //ck
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 256,              //cke
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 256,              //cke
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 0,              //odt0
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 0,              //odt1

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
		//0 for auto training

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
	{//ddr1
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs = 0,
		//sizeof(__ddr_setting) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,
		.cfg_board_common_setting.fast_boot = { 0 },
		.cfg_board_common_setting.ddr_func = 0,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_DDR4,
		.cfg_board_common_setting.enable_lpddr4x_mode = 0,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_16BIT_CH0,
		//CONFIG_DDR0_16BIT_RANK01_CH0,
		//.cfg_board_common_setting.dram_cs0_base_add = 0,
		//.cfg_board_common_setting.dram_cs1_base_add = 0,
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
		.cfg_board_common_setting.DisabledDbyte[0] = 0xf0,
		//bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
		.cfg_board_common_setting.DisabledDbyte[1] = 0xf0,
		//bit 0 -3 ch1 cs0 ,bit 4-7 ch1
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		//log_level = 0xff,boot few log,can test ddr window and max frequency;
		//log_level = 4,training log;
		.cfg_board_common_setting.log_level = 4,
		//4,//LOG_LEVEL_BASIC,
		.cfg_board_common_setting.dbi_enable = DDR_WRITE_READ_DBI_DISABLE,

		//CNTL1 bit23=0 disable SSC,cfg_board_common_setting.pll_ssc_mode = 0,
		//center_ssc_1000ppm,(1 << 29) | (1 << 8) | (2 << 4) | 0
		//CNTL1 bit23=1 enable SSC,
		//CNTL2
		//bit29, 0-29.5k, 1-31.5k, 2-50k
		//bit[11:8], 1-x500ppm, 2-x1000ppm, 3-x1500ppm
		//bit4, 1-12,ssc value = bit[7:4] * bit[11:8];
		//bit0, 0-center_ssc, 1-up_ssc, 1-down_ssc
		.cfg_board_common_setting.pll_ssc_mode = (1 << 29) | (1 << 8) | (2 << 4) | 0,

		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test = {
			0
		},
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR4_16BIT_2,
		//DDR_DMC_REMAP_DDR4_32BIT,
		//DDR_DMC_REMAP_DDR4_32BIT,
		//DDR_DMC_REMAP_DDR4_32BIT,
		// .cfg_board_common_setting.ac_pinmux = {
		//2, 3, 1, 0, 5, 4, 0, 0, 0, 0, 1, 3, 5, 2, 4, 0 },
		.cfg_board_common_setting.ddr_dqs_swap = 0,
		// .cfg_board_common_setting.ddr_dq_remap = {
		//3, 2, 6, 5, 7, 4, 0, 1, 10, 8, 14, 13, 15,
		//12, 9, 11, 23, 21, 17, 18, 16, 19, 20, 22, 31, 28, 24, 25, 27, 26, 30, 29 },
		.cfg_board_common_setting.ddr_vddee_setting = {
			0
		},
		.cfg_board_SI_setting_ps.DRAMFreq = CACLU_CLK_DDR1,
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
// 1 //real chip stk lp4
		.cfg_board_common_setting.dbi_enable = 0x00000000,
		.cfg_board_common_setting.ddr_rfc_type = DDR_RFC_TYPE_DDR4_2Gbx8,

//.cfg_board_common_setting.max_core_timmming_frequency=0x00000e10,// 3600,0x0000006a

//.cfg_board_common_setting.lpddr4_x8_mode=0x00000000,// 0,0x00000087
//.cfg_board_common_setting.tdqs2dq=0x00000000,// 0,0x0000008a
//.cfg_board_common_setting.dfe_offset_value=0x00000000,// 0,0x0000008e
//.cfg_board_common_setting.training_offset=0x00000000,// 0,0x0000008f

#ifdef CONFIG_BOARD_TIMMING             //skt lp4 board
//#if 0
//#define TDQS2DQ  ((0 * 128 * CACLU_CLK_DDR1) / 1000000)
//#define TDQSCK_ADD_DQS   ((0 * 128 * CACLU_CLK_DDR1) / 1000000) //clk should use 64 steps
//#define CLK_DELAY  (0)
//#define PHY_TDQS2DQ  ((0 * 128 * CACLU_CLK_DDR1) / 1000000)
#define WRITE_LEVELING_DDR1  (64 + 0) //((480 * 128 * CACLU_CLK * 2) / 1000000)
#endif
		.cfg_board_SI_setting_ps.clk_drv_ohm = 60,
		.cfg_board_SI_setting_ps.cs_drv_ohm = 40,
		.cfg_board_SI_setting_ps.ac_drv_ohm = 60,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = 40,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = 40,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = 60,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = 0,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = 40,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = 40,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = 0,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = 120,
		.cfg_board_SI_setting_ps.dram_drv_pull_up_cal_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_range =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		//.cfg_ddr_training_delay_ps.dfe_offset = 0,
//#ifdef CONFIG_PXP_TIMMING
//		.cfg_board_common_setting.ac_pinmux = {
//			0,	1,	2,	3,
//4,	5,	24,	25,	28,	29,
//		6,	7,	8,	9,	10,
//11,	12,	13,	14,	15,	16,
//		17,	18,	19,	20,	21,	22,	23,	26,	27,
//		},
//#else
		.cfg_board_common_setting.ac_pinmux = {
			13, 23, 7,  22, 27, 26, 16, 11, 2,  6,
			8,  5,	21, 0,	14, 15, 12, 18, 19, 10, 20,
			1,  3,	9,  4,	17, 24, 25, 28, 29,
		},
#ifdef CONFIG_BOARD_TIMMING
		.cfg_board_common_setting.ddr_dq_remap = {
			4, 0, 5, 1, 6, 3, 7, 2, 32,
			12, 10, 14, 9, 13, 8, 11, 33, 15,
			16, 17, 18, 19, 20, 21, 22, 34, 23,
			24, 25, 26, 27, 28, 29, 30, 35, 31,
		},
#endif

		//.cfg_board_common_setting.ddr_dq_remap= {
		//3, 0, 2, 1, 4, 6, 5, 7,  14, 12, 13, 15,
		.cfg_ddr_training_delay_ps.tx_offset[0] = (0 << 7) | 0x0,
		.cfg_ddr_training_delay_ps.rx_offset[0] = (0 << 7) | 0x0,
		.cfg_ddr_training_delay_ps.reserve_para = {
			//cs0 write dqs,lane0-lane3
			(1 << 7) | 8, (1 << 7) | 8, (1 << 7) | 8, (1 << 7) | 8,
			//cs1 write dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs0 read dqs,lane0-lane3
			(1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16,
			//cs1 read dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		},
		//.cfg_ddr_training_delay_ps.reserve_para[0] = (1 << 7) | 0x8,//write dqs
		//.cfg_ddr_training_delay_ps.reserve_para[1] = (1 << 7) | 0x8,//write dqs
		//.cfg_ddr_training_delay_ps.reserve_para[2] = (1 << 7) | 0x8,//write dqs
		//.cfg_ddr_training_delay_ps.reserve_para[3] = (1 << 7) | 0x8,//write dqs
		//.cfg_ddr_training_delay_ps.reserve_para[8 + 0] = (1 << 7) | 0x10,//read dqs
		//.cfg_ddr_training_delay_ps.reserve_para[8 + 1] = (1 << 7) | 0x10,//read dqs
		//.cfg_ddr_training_delay_ps.reserve_para[8 + 2] = (1 << 7) | 0x10,//read dqs
		//.cfg_ddr_training_delay_ps.reserve_para[8 + 3] = (1 << 7) | 0x10,//read dqs
		//.cfg_ddr_training_delay_ps.dac_offset[0] = 0,//(1 << 7) | 0x10,
		//.cfg_ddr_training_delay_ps.dac_offset[1] = 0,//(0 << 7) | 0x10,
		//.cfg_ddr_training_delay_ps.dac_offset[0] = (1 << 7) | 0x5,
		//.cfg_ddr_training_delay_ps.dac_offset[1] = (0 << 7) | 0x5,
		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 256 + 128,
		//cke 128 only 1UI margin
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 256 + 128,
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 256 + 110,//110,// cs//skt 110 ref 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 256 + 0,         // cs
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 128 + 128,      //ck
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 128 + 128,      //ck
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 256,              //cke
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 256,              //cke
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 0,               //odt0
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 0,               //odt1
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

		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 128 + WRITE_LEVELING_DDR1 + 0,
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 128 + WRITE_LEVELING_DDR1 + 0,
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 128 + WRITE_LEVELING_DDR1 + 0,
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 128 + WRITE_LEVELING_DDR1 + 0,
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 128 + WRITE_LEVELING_DDR1 + 0,
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 128 + WRITE_LEVELING_DDR1 + 0,
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 128 + WRITE_LEVELING_DDR1 + 0,
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 128 + WRITE_LEVELING_DDR1 + 0,
		.cfg_ddr_training_delay_ps.write_wck_delay[0] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[1] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[2] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[3] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[4] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[5] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[6] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[7] = 0x00000200,

		.cfg_ddr_training_delay_ps.wdq_delay[0] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[1] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[2] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[3] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[4] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[5] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[6] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[7] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[8] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[9] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[10] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[11] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[12] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[13] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[14] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[15] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[16] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[17] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[18] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[19] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[20] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[21] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[22] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[23] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[24] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[25] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[26] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[27] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[28] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[29] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[30] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[31] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[32] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[33] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[34] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[35] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[36] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[37] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[38] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[39] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[40] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[41] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[42] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[43] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[44] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[45] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[46] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[47] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[48] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[49] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[50] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[51] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[52] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[53] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[54] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[55] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[56] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[57] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[58] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[59] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[60] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[61] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[62] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[63] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[64] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[65] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[66] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[67] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[68] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[69] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[70] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.wdq_delay[71] = 192 + TDQS2DQ + WRITE_LEVELING_DDR1,

		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 128,

		.cfg_ddr_training_delay_ps.soc_bit_vref0[0] = 0x000001f,
		//0 for auto training

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
};
#endif

#define DDR_EFUSE_ID_NTC_C_1G_X2   1
#define DDR_EFUSE_ID_SAM_C_1G_X2   2
#define DDR_EFUSE_ID_SAM_F_512M_X2   3
#define DDR_EFUSE_ID_SAM_E_512M_X2   4
#define DDR_EFUSE_ID_NTC_E_512M_X2   5

//bit 6 adc_channel bit 0-5 adc value,chan 3 value 8 is layer 2
#define DDR_ID_ACS_ADC   ((3 << 6) | (8))

#define DDR_RESV_CHECK_ID_ENABLE  0Xfe
#define SAR_ADC_DDR_ID_BASE   0
#define SAR_ADC_DDR_ID_STEP   80
//85  0-125 step 0
//#define BOARD_1G_1G_ADC_ID   (SAR_ADC_DDR_ID_BASE + SAR_ADC_DDR_ID_STEP)
//167 126-200 step 1
//#define BOARD_2G_1G_ADC_ID   (SAR_ADC_DDR_ID_BASE + SAR_ADC_DDR_ID_STEP + SAR_ADC_DDR_ID_STEP)
#define DDR_TIMMING_OFFSET(X) (unsigned int)(unsigned long)(&(((ddr_set_ps0_only_t *)(0))->X))
#define DDR_TIMMING_OFFSET_SIZE(X) sizeof(((ddr_set_ps0_only_t *)(0))->X)

//bit24-31 define ID and size
#define DDR_ID_FROM_EFUSE  (0Xffu << 24)
#define DDR_ID_FROM_ADC  (0Xfeu << 24)
#define DDR_ID_FROM_GPIO_CONFIG1  (0Xfdu << 24)
#define DDR_ID_START_MASK  (0XFFDDCCBBu)

#define DDR_ADC_CH0  (0X0u << 5)
#define DDR_ADC_CH1  (0X1u << 5)
#define DDR_ADC_CH2  (0X2u << 5)
#define DDR_ADC_CH3  (0X3u << 5)
#define DDR_ADC_CH4  (0X4u << 5)

#define DDR_ADC_VALUE0  (0X0u << 0)
#define DDR_ADC_VALUE1  (0X1u << 0)
#define DDR_ADC_VALUE2  (0X2u << 0)
#define DDR_ADC_VALUE3  (0X3u << 0)
#define DDR_ADC_VALUE4  (0X4u << 0)
#define DDR_ADC_VALUE5  (0X5u << 0)
#define DDR_ADC_VALUE6  (0X6u << 0)
#define DDR_ADC_VALUE7  (0X7u << 0)
#define DDR_ADC_VALUE8  (0X8u << 0)
#define DDR_ADC_VALUE9  (0X9u << 0)
#define DDR_ADC_VALUE10  (0Xau << 0)
#define DDR_ADC_VALUE11  (0Xbu << 0)
#define DDR_ADC_VALUE12  (0Xcu << 0)
#define DDR_ADC_VALUE13  (0Xdu << 0)
#define DDR_ADC_VALUE14  (0Xeu << 0)
#define DDR_ADC_VALUE15  (0Xfu << 0)

typedef  struct   ddr_para_data {
	//start from	DDR_ID_START_MASK,ddr_id;//bit0-23
	//ddr_id value,bit 24-31 ddr_id source,
	//0xfe source from adc ,0xfd source from gpio_default_config
	//reg_offset
	//bit 0-15 parameter offset value,bit16-23 overrid
	// size,bit24-31 mux ddr_id source
	//unsigned int	reg_offset;
	//unsigned int	value;

	//bit0-15 only support data size = 1byte
	// or 2bytes,no support int value
	uint32_t	value : 16;
	uint32_t	reg_offset : 12;        //bit16-27
	uint32_t	data_size : 4;//bit28-31 if data size =15,then  will mean DDR_ID start
}  ddr_para_data_t;

typedef  struct   ddr_para_data_start {
	uint32_t	id_value : 24;          //bit0-23  efuse id or ddr id
	//uint32_t	id_adc_ch : 2;//bit6-7
	uint32_t	id_src_from : 8;        //bit24-31 ddr id from adc or gpio
}  ddr_para_data_start_t;

#define DDR_TIMMING_TUNE_STRUCT_SIZE(a)  sizeof(a)

#define DDR_TIMMING_TUNE_TIMMING0_F(PARA, VALUE) ((DDR_TIMMING_OFFSET(PARA)) << 16) |\
((DDR_TIMMING_OFFSET_SIZE(PARA)) << 28) | VALUE
#define DDR_TIMMING_TUNE_TIMMING1_F(PARA, VALUE) ((sizeof(ddr_set_ps0_only_t) +\
DDR_TIMMING_OFFSET(PARA)) << 16) | ((DDR_TIMMING_OFFSET_SIZE(PARA)) << 28) | (VALUE)

#define DDR_TIMMING_TUNE_START(id_src_from, id_adc_ch, id_value) (id_src_from) |\
(id_adc_ch) | (id_value)

uint32_t __bl2_ddr_reg_data[] __attribute__ ((section(".ddr_2acs_data"))) = {
	//DDR_ID
	DDR_TIMMING_TUNE_START(DDR_ID_FROM_ADC, DDR_ADC_CH4, DDR_ADC_VALUE0),
	//DDR1-2GB-16bit,DDR0-x
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_common_setting.dram_ch0_size_MB,
		((DRAM_SIZE_ID_256MBX8 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_common_setting.dram_ch1_size_MB,
		((DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	//DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_SI_setting_ps.DRAMFreq, 1176),
	//DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_SI_setting_ps.DRAMFreq, 1176),

	DDR_TIMMING_TUNE_START(DDR_ID_FROM_ADC, DDR_ADC_CH4, DDR_ADC_VALUE1),
	//DDR1-1GB-16bit,DDR0-x
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_common_setting.dram_ch0_size_MB,
		((DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_common_setting.dram_ch1_size_MB,
		((DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),

	DDR_TIMMING_TUNE_START(DDR_ID_FROM_EFUSE, 0, DDR_EFUSE_ID_NTC_C_1G_X2),
	//efuse = ,offset ddr0
	//.cfg_ddr_training_delay_ps.reserve_para[0] = (1 << 7) | 0x8,//write dqs
	//.cfg_ddr_training_delay_ps.reserve_para[1] = (1 << 7) | 0x8,//write dqs
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.reserve_para[0], (0 << 7) | 2),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.reserve_para[1], (0 << 7) | 2),

	//DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_SI_setting_ps.DRAMFreq, 1176),
	//DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_SI_setting_ps.DRAMFreq, 1176),
	//...
	//...
	//data end
};

////_ddr_para_2nd_setting
//*/
uint32_t __ddr_parameter_reg_index[] __attribute__
	((section(".ddr_2acs_index"))) = {
	DDR_ID_START_MASK,
	DDR_TIMMING_TUNE_STRUCT_SIZE(__bl2_ddr_reg_data),
	//0,
};

uint32_t __bl2_reg[] __attribute__ ((section(".generic_param"))) = {
	DDR_ID_START_MASK,
	DDR_TIMMING_TUNE_STRUCT_SIZE(__bl2_ddr_reg_data),
	//0,
};

