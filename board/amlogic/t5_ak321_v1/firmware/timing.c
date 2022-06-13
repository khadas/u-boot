// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <asm/arch/secure_apb.h>
#include <asm/arch/timing.h>
#include <asm/arch/ddr_define.h>

ddr_set_t __ddr_setting[] __attribute__ ((section(".ddr_settings"))) = {
{
		.cfg_board_common_setting.timming_magic 				= 0,
		.cfg_board_common_setting.timming_max_valid_configs 		= 1,
		.cfg_board_common_setting.timming_struct_version			= 0,
		.cfg_board_common_setting.timming_struct_org_size			= sizeof(ddr_set_t),
		.cfg_board_common_setting.timming_struct_real_size			= 0,									   //0
		.cfg_board_common_setting.fast_boot 				= { 0},
		.cfg_board_common_setting.fast_boot[0]				= 0x1,
		.cfg_board_common_setting.fast_boot[3]				= 0xC6,
		.cfg_board_common_setting.ddr_func = DDR_FUNC | DDR_FUNC_ENABLE_DDR_ID,
		.cfg_board_common_setting.board_id					= CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType					= CONFIG_DDR_TYPE_DDR4,
		.cfg_board_common_setting.dram_rank_config				= CONFIG_DDR0_32BIT_RANK0_CH0,
		.cfg_board_common_setting.DisabledDbyte 				= CONFIG_DISABLE_D32_D63,
		.cfg_board_common_setting.dram_cs0_base_add 			= 0,
		.cfg_board_common_setting.dram_cs1_base_add 			= 0,
		.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_2048MB,
		.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
		.cfg_board_common_setting.dram_x4x8x16_mode 			= CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming					= CONFIG_USE_DDR_2T_MODE,
		.cfg_board_common_setting.log_level 				= LOG_LEVEL_BASIC,
		.cfg_board_common_setting.ddr_rdbi_wr_enable				= DDR_WRITE_READ_DBI_DISABLE,
		/* pll ssc config:
		 *
		 *   pll_ssc_mode = (1<<20) | (1<<8) | ([strength] << 4) | [mode],
		 *      ppm = strength * 500
		 *      mode: 0=center, 1=up, 2=down
		 *
		 *   eg:
		 *     1. config 1000ppm center ss. then mode=0, strength=2
		 *        .pll_ssc_mode = (1<<20) | (1<<8) | (2 << 4) | 0,
		 *     2. config 3000ppm down ss. then mode=2, strength=6
		 *        .pll_ssc_mode = (1<<20) | (1<<8) | (6 << 4) | 2,
		 */
		 //SSC_DISABLE,(1 << 20) | (0 << 8) | (2 << 4) | 0,
		 //center_ssc_1000ppm,
		.cfg_board_common_setting.pll_ssc_mode = (1 << 20) | (1 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.org_tdqs2dq					= 0,
		.cfg_board_common_setting.reserve1_test_function			= { 0 },
		.cfg_board_common_setting.ddr_dmc_remap 				= DDR_DMC_REMAP_DDR4_32BIT,
#if 0
		.cfg_board_common_setting.ac_pinmux = {
			0,	0,	0,	1,	2,	3,	8, 27,
			10, 2,	9,	21, 5,	14, 1, 0,
			25, 13, 12, 4,	7,	22, 0, 0,
			0,	0,	6,	3,	20, 0,	0, 15,
			26, 11,
		},
#endif
#if 1
		//ak321 ac pinmux
		.cfg_board_common_setting.ac_pinmux = {
			0,	0,	0,	1,	2,	3,	3, 12,
			20, 8,	10,	11, 4,	0, 28, 13,
			21, 24, 9, 6, 27,	25, 0, 0,
			0,	0,	7,	2,	23, 0,	0, 1,
			5, 26, 0
		},
#endif
#if 0
		.cfg_board_common_setting.ac_pinmux = {
			0, 0, 0, 1, 2, 3, 0, 1,
			2, 3, 4, 5, 6,  7, 8, 9,
			10, 11, 12, 13, 14, 15,
			0, 0, 0, 0, 20, 21, 22,
			0, 0, 25, 26, 27,
		},
#endif
		.cfg_board_common_setting.ddr_dqs_swap					= 0,
		.cfg_board_common_setting.ddr_dq_remap = {
			0,	1,	2,	3,	4,	5,	6,	7,
			8,	9,	10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, 29, 30, 31,
			32, 33, 34, 35
		},															 //d0-d31 dm0 dm1 dm2 dm3
		.cfg_board_common_setting.ddr_vddee_setting 			= { 0 },
		.cfg_board_SI_setting_ps[0].DRAMFreq = 1176,
		.cfg_board_SI_setting_ps[0].PllBypassEn = 0,
		.cfg_board_SI_setting_ps[0].training_SequenceCtrl = 0,
		.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_60_OHM,
		.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_60_OHM,
		.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_60_OHM,
		.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_48_OHM,
		.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_48_OHM,
		.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
		.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,
		.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_48_OHM,
		.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_60_OHM,
		.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR_WR_ODT_0_OHM,
		.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
		.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting = DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		.cfg_board_SI_setting_ps[0].reserve2 = 0,
		.cfg_board_SI_setting_ps[0].vref_ac_permil = 0,
		.cfg_board_SI_setting_ps[0].vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps[0].vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps[0].max_core_timmming_frequency = 0,
		.cfg_board_SI_setting_ps[0].training_phase_parameter = { 0 },
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org = {
			128, 128, 128, 128, 128, 128, 128, 128,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384,
		}, //total 36
		.cfg_ddr_training_delay_ps[0].ac_trace_delay = {
			128, 128, 128, 128, 128, 128, 128, 256,
			256, 256, 256, 256, 256, 256 + 100, 256, 256,
			256, 256, 256, 256, 256, 256, 256, 256,
			256, 256, 256, 256, 256 + 30, 256, 256, 256,
			256, 256, 256 + 100, 256,
		}, //total 36

		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[0] = 0x00000040,// 64
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[1] = 0x00000040,// 64
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[2] = 00,// 70 cs
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[3] = 0x00000040,// 64
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[4] = 0x00000040,// 64
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[5] = 0x00000040,// 64
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[6] = 0x00000080,// 128
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[7] = 0x000000c0,// 192
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[8] = 0x000000dc,// 220
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[9] = 0x00000140,// 320
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[10] = 0x00000140,// 320
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[11] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[12] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[13] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[14] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[15] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[16] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[17] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[18] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[19] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[20] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[21] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[22] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[23] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[24] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[25] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[26] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[27] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[28] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[29] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[30] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[31] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[32] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[33] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[34] = 0x00000000,// 0
		.cfg_board_SI_setting_ps[0].ac_trace_delay_org[35] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[0] = 0x00000040 + 100,// 64
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[1] = 0x00000040,// 64
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[2] = 0x000000,// 70
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[3] = 0x00000040,// 64
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[4] = 0x00000040 + 100,// 64
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[5] = 0x00000040,// 64
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[6] = 0x00000080,// 128
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[7] = 0x000000c0,// 192
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[8] = 0x000000dc + 50,// 220
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[9] = 0x00000140 + 50,// 320
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[10] = 0x00000140,// 320
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[11] = 0x00000140,// 320
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[12] = 0x0000010e,// 270
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[13] = 0x00000172,// 370
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[14] = 0x00000118,// 280
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[15] = 0x00000140 + 50,// 320
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[16] = 0x000000dc + 50,// 220
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[17] = 0x00000100 + 50,// 256
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[18] = 0x00000140,// 320
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[19] = 0x00000140,// 320
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[20] = 0x00000140,// 320
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[21] = 0x00000140,// 320
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[22] = 0x00000140,// 320
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[23] = 0x00000140,// 320
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[24] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[25] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[26] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[27] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[28] = 0x000000c0 + 50,// 192
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[29] = 0x00000140,// 320
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[30] = 0x00000140,// 320
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[31] = 0x00000140,// 320
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[32] = 0x00000140,// 320
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[33] = 0x00000140,// 320
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[34] = 0x00000180,// 384
		.cfg_ddr_training_delay_ps[0].ac_trace_delay[35] = 0x000000c0 + 50,// 192
		.cfg_ddr_training_delay_ps[0].write_dqs_delay[0] = 0x000000b9,// 185
		.cfg_ddr_training_delay_ps[0].write_dqs_delay[1] = 0x000000b4,// 180
		.cfg_ddr_training_delay_ps[0].write_dqs_delay[2] = 0x000000e1,// 225
		.cfg_ddr_training_delay_ps[0].write_dqs_delay[3] = 0x000000d7,// 215
		.cfg_ddr_training_delay_ps[0].write_dqs_delay[4] = 0x00000080,// 128
		.cfg_ddr_training_delay_ps[0].write_dqs_delay[5] = 0x00000080,// 128
		.cfg_ddr_training_delay_ps[0].write_dqs_delay[6] = 0x00000080,// 128
		.cfg_ddr_training_delay_ps[0].write_dqs_delay[7] = 0x00000080,// 128
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0] = 0x00000108,// 264
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1] = 0x000000f2,// 242
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2] = 0x00000100,// 256
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3] = 0x000000f5,// 245
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4] = 0x00000103,// 259
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5] = 0x000000e5,// 229
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6] = 0x00000108,// 264
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7] = 0x000000f9,// 249
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8] = 0x000000f8,// 248
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9] = 0x000000f6,// 246
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10] = 0x000000f2,// 242
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11] = 0x00000101,// 257
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12] = 0x000000e9,// 233
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13] = 0x00000104,// 260
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14] = 0x000000f1,// 241
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15] = 0x000000f2,// 242
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16] = 0x000000ef,// 239
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17] = 0x000000f4,// 244
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18] = 0x00000126,// 294
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19] = 0x00000113,// 275
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20] = 0x00000125,// 293
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21] = 0x0000010f,// 271
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22] = 0x00000130,// 304
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23] = 0x0000011b,// 283
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24] = 0x0000011e,// 286
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25] = 0x0000010e,// 270
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26] = 0x0000011f,// 287
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27] = 0x00000128,// 296
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28] = 0x00000118,// 280
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29] = 0x00000132,// 306
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30] = 0x00000119,// 281
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31] = 0x00000117,// 279
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32] = 0x0000010e,// 270
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33] = 0x00000117,// 279
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34] = 0x00000100,// 256
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35] = 0x00000119,// 281
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[36] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[37] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[38] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[39] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[40] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[41] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[42] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[43] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[44] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[45] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[46] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[47] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[48] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[49] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[50] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[51] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[52] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[53] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[54] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[55] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[56] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[57] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[58] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[59] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[60] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[61] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[62] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[63] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[64] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[65] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[66] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[67] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[68] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[69] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[70] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[71] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0] = 0x00000311,// 785
		.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1] = 0x0000030d,// 781
		.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2] = 0x00000330,// 816
		.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3] = 0x00000338,// 824
		.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].read_dqs_delay[0] = 0x0000007e,// 126
		.cfg_ddr_training_delay_ps[0].read_dqs_delay[1] = 0x00000081,// 129
		.cfg_ddr_training_delay_ps[0].read_dqs_delay[2] = 0x0000007a,// 122
		.cfg_ddr_training_delay_ps[0].read_dqs_delay[3] = 0x0000007d,// 125

		//.cfg_ddr_training_delay_ps[0].read_dqs_delay[4] = 0x00000000,// 0
		//.cfg_ddr_training_delay_ps[0].read_dqs_delay[5] = 0x00000000,// 0
		//.cfg_ddr_training_delay_ps[0].read_dqs_delay[6] = 0x00000000,// 0
		//.cfg_ddr_training_delay_ps[0].read_dqs_delay[7] = 0x00000000,// 0
		//for sip 4pcs ddr4 rank01 2GB from ak301 timing.c
		.cfg_ddr_training_delay_ps[0].read_dqs_delay[4] = 0x00000080,
		.cfg_ddr_training_delay_ps[0].read_dqs_delay[5] = 0x00000080,
		.cfg_ddr_training_delay_ps[0].read_dqs_delay[6] = 0x00000080,
		.cfg_ddr_training_delay_ps[0].read_dqs_delay[7] = 0x00000080,

		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0] = 0x00000046,// 70
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1] = 0x00000036,// 54
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2] = 0x0000004c,// 76
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3] = 0x00000034,// 52
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4] = 0x00000047,// 71
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5] = 0x00000033,// 51
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6] = 0x00000057,// 87
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7] = 0x00000034,// 52
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8] = 0x00000040,// 64
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9] = 0x0000004a,// 74
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10] = 0x0000003a,// 58
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11] = 0x00000052,// 82
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12] = 0x00000036,// 54
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13] = 0x0000004e,// 78
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14] = 0x00000039,// 57
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15] = 0x00000047,// 71
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16] = 0x00000037,// 55
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17] = 0x00000042,// 66
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18] = 0x00000041,// 65
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19] = 0x0000002f,// 47
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20] = 0x0000003c,// 60
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21] = 0x0000002d,// 45
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22] = 0x00000048,// 72
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23] = 0x0000003b,// 59
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24] = 0x00000040,// 64
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25] = 0x0000002f,// 47
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26] = 0x00000039,// 57
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27] = 0x0000004b,// 75
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28] = 0x00000037,// 55
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29] = 0x00000052,// 82
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30] = 0x00000039,// 57
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31] = 0x00000045,// 69
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32] = 0x00000032,// 50
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33] = 0x0000003f,// 63
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34] = 0x0000002d,// 45
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35] = 0x0000003e,// 62

		//for sip 4pcs ddr4 rank01 2GB from ak301 timing.c
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[36] = 0x00000054,       // 84
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[37] = 0x0000003e,       // 62
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[38] = 0x00000053,       // 83
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[39] = 0x0000003e,       // 62
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[40] = 0x00000056,       // 86
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[41] = 0x00000043,       // 67
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[42] = 0x0000005a,       // 90
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[43] = 0x00000040,       // 64
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[44] = 0x0000004a,       // 74
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[45] = 0x00000053,       // 83
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[46] = 0x0000003c,       // 60
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[47] = 0x0000004f,       // 79
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[48] = 0x00000041,       // 65
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[49] = 0x00000056,       // 86
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[50] = 0x00000044,       // 68
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[51] = 0x0000004b,       // 75
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[52] = 0x00000045,       // 69
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[53] = 0x00000049,       // 73
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[54] = 0x0000005a,       // 90
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[55] = 0x00000041,       // 65
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[56] = 0x00000056,       // 86
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[57] = 0x0000003e,       // 62
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[58] = 0x00000058,       // 88
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[59] = 0x0000003c,       // 60
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[60] = 0x00000054,       // 84
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[61] = 0x00000042,       // 66
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[62] = 0x0000004b,       // 75
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[63] = 0x00000059,       // 89
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[64] = 0x0000003c,       // 60
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[65] = 0x0000004d,       // 77
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[66] = 0x00000040,       // 64
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[67] = 0x0000004e,       // 78
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[68] = 0x0000003d,       // 61
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[69] = 0x00000051,       // 81
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[70] = 0x0000003d,       // 61
		.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[71] = 0x00000047,       // 71

		.cfg_ddr_training_delay_ps[0].soc_bit_vref[0] = 0,// 38
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[1] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[2] = 0x00000025,// 37
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[3] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[4] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[5] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[6] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[7] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[8] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[9] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[10] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[11] = 0x0000002b,// 43
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[12] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[13] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[14] = 0x00000026,// 38
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[15] = 0x00000026,// 38
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[16] = 0x00000026,// 38
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[17] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[18] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[19] = 0x0000002b,// 43
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[20] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[21] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[22] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[23] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[24] = 0x0000002c,// 44
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[25] = 0x0000002b,// 43
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[26] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[27] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[28] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[29] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[30] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[31] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[32] = 0x00000026,// 38
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[33] = 0x0000002b,// 43
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[34] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[35] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[36] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[37] = 0x00000031,// 49
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[38] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[39] = 0x00000030,// 48
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[40] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[41] = 0x00000032,// 50
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[42] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps[0].soc_bit_vref[43] = 0x0000002f,// 47
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[0] = 0x0000001f,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[1] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[2] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[3] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[4] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[5] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[6] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[7] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[8] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[9] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[10] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[11] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[12] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[13] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[14] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[15] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[16] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[17] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[18] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[19] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[20] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[21] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[22] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[23] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[24] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[25] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[26] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[27] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[28] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[29] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[30] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[31] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[32] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[33] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[34] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps[0].dram_bit_vref[35] = 0x00000000,// 0

#if 1
		.cfg_ddr_training_delay_ps[0].reserve_training_parameter = {
			//cs0 write dqs,lane0-lane3
			(0 << 7) | 8,  (0 << 7) | 8,  (0 << 7) | 0,  (0 << 7) | 0,
			//cs1 write dqs,lane0-lane3
			(0 << 7) | 0,  (0 << 7) | 0,  (0 << 7) | 0,  (0 << 7) | 0,
			//cs0 read dqs,lane0-lane3
			(1 << 7) | 10,  (1 << 7) | 10,  (1 << 7) | 6,  (1 << 7) | 10,
			//cs1 read dqs,lane0-lane3
			(0 << 7) | 0,  (0 << 7) | 0,  (0 << 7) | 0,  (0 << 7) | 0,
		},
#endif
		.cfg_board_SI_setting_ps[1].DRAMFreq = 667,
		.cfg_board_SI_setting_ps[1].PllBypassEn = 0,
		.cfg_board_SI_setting_ps[1].training_SequenceCtrl = 0,
		.cfg_board_SI_setting_ps[1].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		.cfg_board_SI_setting_ps[1].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps[1].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps[1].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
		.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,
		.cfg_board_SI_setting_ps[1].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
		.cfg_board_SI_setting_ps[1].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_60_OHM,
		.cfg_board_SI_setting_ps[1].dram_data_wr_odt_ohm = DDR_DRAM_DDR_WR_ODT_0_OHM,
		.cfg_board_SI_setting_ps[1].dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
		.cfg_board_SI_setting_ps[1].dram_data_drv_pull_up_calibration_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps[1].lpddr4_dram_vout_voltage_range_setting = DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		.cfg_board_SI_setting_ps[1].reserve2 = 0,
		.cfg_board_SI_setting_ps[1].vref_ac_permil = 0,
		.cfg_board_SI_setting_ps[1].vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps[1].vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps[1].max_core_timmming_frequency = 0,
		.cfg_board_SI_setting_ps[1].training_phase_parameter = { 0 },
		.cfg_board_SI_setting_ps[1].ac_trace_delay_org = {
			128, 128, 128, 128, 128, 128, 128, 128,
			128, 128, 128, 128, 128, 128, 128, 128,
			128, 128, 128, 128, 128, 128, 128, 128,
			128, 128, 128, 128, 128, 128, 128, 128,
			128, 128, 128, 128,
		}, //total 36
		.cfg_ddr_training_delay_ps[1].ac_trace_delay = {
			128, 128, 128, 128, 128, 128, 128, 128,
			128, 128, 128, 128, 128, 128, 128, 128,
			128, 128, 128, 128, 128, 128, 128, 128,
			128, 128, 128, 128, 128, 128, 128, 128,
			128, 128, 128, 128,
		}, //total 36
		.cfg_ddr_training_delay_ps[1].write_dqs_delay = {
			0, 0, 0, 0, 0, 0, 0, 0
		},
		.cfg_ddr_training_delay_ps[1].write_dq_bit_delay = {
			50, 50, 50, 50, 50, 50, 50, 50,
			50, 50, 50, 50, 50, 50, 50, 50,
			50, 50, 50, 50, 50, 50, 50, 50,
			50, 50, 50, 50, 50, 50, 50, 50,
			50, 50, 50, 50, 50, 50, 50, 50,
			50, 50, 50, 50, 50, 50, 50, 50,
			50, 50, 50, 50, 50, 50, 50, 50,
			50, 50, 50, 50, 50, 50, 50, 50,
			50, 50, 50, 50, 50, 50, 50, 50,
		},
		.cfg_ddr_training_delay_ps[1].read_dqs_gate_delay = {
			192, 192, 192, 192, 192, 192, 192, 192
		}, //total 8
		.cfg_ddr_training_delay_ps[1].read_dqs_delay = {
			64, 64, 64, 64, 64, 64, 64, 64
		},              //total 8
		.cfg_ddr_training_delay_ps[1].read_dq_bit_delay = {
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0
		}, //total 72
		.cfg_ddr_training_delay_ps[1].soc_bit_vref = {
			0,  40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40
		}, //total 44
		.cfg_ddr_training_delay_ps[1].dram_bit_vref = {
			0,  32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32
		}, //total 36
		.cfg_ddr_training_delay_ps[1].reserve_training_parameter = { 0 },
},
};

pll_set_t __pll_setting = {
	.cpu_clk				= CONFIG_CPU_CLK / 24 * 24,
#ifdef CONFIG_PXP_EMULATOR
	.pxp					= 1,
#else
	.pxp					= 0,
#endif
	.spi_ctrl				= 0,
	.lCustomerID			= CONFIG_AML_CUSTOMER_ID,
#ifdef CONFIG_DEBUG_MODE
	.debug_mode				= CONFIG_DEBUG_MODE,
	.ddr_clk_debug			= CONFIG_DDR_CLK_DEBUG,
	.cpu_clk_debug			= CONFIG_CPU_CLK_DEBUG,
#endif

#ifdef CONFIG_AML_SUPPORT_TLV
	.nCFGTAddr              = AML_BL2_TMASTER_DDR_ADDR,
#endif

	.low_console_baud		= CONFIG_LOW_CONSOLE_BAUD,
};

#if 0
#define DDR_TIMMING_OFFSET(X)  (unsigned int)(unsigned long)(&(((ddr_set_t *)(0))->X))
#define DDR_TIMMING_OFFSET_SIZE(X)  sizeof(((ddr_set_t *)(0))->X)
#define DDR_TIMMING_TUNE(DDR_ID, PARA, VALUE) { DDR_ID, DDR_TIMMING_OFFSET(PARA), VALUE, DDR_TIMMING_OFFSET_SIZE(PARA), 0, 1 }
//DDR_ID,default ADC_CH1
#define DDR_ADC_CH0  (0X0u << 6)
#define DDR_ADC_CH1  (0X1u << 6)
#define DDR_ADC_CH2  (0X2u << 6)
#define DDR_ADC_CH3  (0X3u << 6)

#define DDR_ADC_VALUE0  (0X0u << 0)//DDR_ID_DDR4_2PCS_1GB
#define DDR_ADC_VALUE1  (0X1u << 0)//DDR_ID_DDR4_2PCS_2GB,default
#define DDR_ADC_VALUE2  (0X2u << 0)//DDR_ID_DDR4_2PCS_3GB
#define DDR_ADC_VALUE3  (0X3u << 0)//DDR_ID_DDR4_2PCS_1_5GB
#define DDR_ADC_VALUE4  (0X4u << 0)//DDR_ID_DDR4_2PCS_4GB
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

//efuse
#define T963_H8_NTC_EID       0xC0A
#define T963_H8_SUM_EID       0xC4A
#define T963_H35_NTC_EID      0x2C1A
#define T963_H35_SUM_EID      0x2C5A
#define T963_H9_NTC_EID       0x6C2A
#define T963_H9_SUM_EID       0x6C6A
#define T963_H_NA_EID         0x1400

#define T963S_H8_NTC_EID      0xC8A
#define T963S_H8_SUM_EID      0xCCA
#define T963S_H35_SUM_EID     0x2CDA
#define T950D4_H5_NTC_EID     0x509
#define T950D4_H6_NTC_EID     0x909
#define T950D4_H8_NTC_EID     0xC0A
#define T950D4_H8_SUM_EID     0xC4A
#define T950D4_H35_NTC_EID    0x2C1A
#define T950D4_H35_SUM_EID    0x2C5A
#define T950X4_H5_NTC_EID     0xD09
#define T950X4_H8_NTC_EID     0xC8A
#define T950X4_H8_SUM_EID     0xCCA
#define T950X4_H35_SUM_EID    0x2CDA
#define T950X4_B5_SUM_EID     0x4C6
#endif

ddr_reg_t __ddr_reg[] = {
	/* demo, user defined override register */
	//DDR_TIMMING_TUNE(T963_H35_NTC_EID, cfg_board_SI_setting_ps[0].DRAMFreq, 1176),

	//for sip 4pcs ddr4 rank01 2GB from ak301 timing.c
	//T963_H9_SUM_EID,T963_H9_NTC_EID
	//.cfg_board_common_setting.dram_cs0_size_MB = CONFIG_DDR0_SIZE_1024MB;
	//.cfg_board_common_setting.dram_cs1_size_MB = CONFIG_DDR0_SIZE_1024MB;
	DDR_TIMMING_TUNE(T963_H9_SUM_EID, cfg_board_common_setting.dram_cs0_size_MB,
	CONFIG_DDR0_SIZE_1024MB),
	DDR_TIMMING_TUNE(T963_H9_SUM_EID, cfg_board_common_setting.dram_cs1_size_MB,
	CONFIG_DDR0_SIZE_1024MB),
	DDR_TIMMING_TUNE(T963_H9_NTC_EID, cfg_board_common_setting.dram_cs0_size_MB,
	CONFIG_DDR0_SIZE_1024MB),
	DDR_TIMMING_TUNE(T963_H9_NTC_EID, cfg_board_common_setting.dram_cs1_size_MB,
	CONFIG_DDR0_SIZE_1024MB),

//	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
//	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
//	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_120_OHM,
//	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
//	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_40_OHM,
//	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_240_OHM,
	DDR_TIMMING_TUNE(T963_H9_SUM_EID, cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p,
	DDR_SOC_DATA_DRV_ODT_40_OHM),
	DDR_TIMMING_TUNE(T963_H9_SUM_EID, cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n,
	DDR_SOC_DATA_DRV_ODT_40_OHM),
	DDR_TIMMING_TUNE(T963_H9_SUM_EID, cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p,
	DDR_SOC_DATA_DRV_ODT_120_OHM),
	DDR_TIMMING_TUNE(T963_H9_SUM_EID, cfg_board_SI_setting_ps[0].dram_data_drv_ohm,
	DDR_DRAM_DDR4_DRV_34_OHM),
	DDR_TIMMING_TUNE(T963_H9_SUM_EID, cfg_board_SI_setting_ps[0].dram_data_odt_ohm,
	DDR_DRAM_DDR4_ODT_40_OHM),
	DDR_TIMMING_TUNE(T963_H9_SUM_EID, cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm,
	DDR_DRAM_DDR4_WR_ODT_240_OHM),

	DDR_TIMMING_TUNE(T963_H9_NTC_EID, cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p,
	DDR_SOC_DATA_DRV_ODT_40_OHM),
	DDR_TIMMING_TUNE(T963_H9_NTC_EID, cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n,
	DDR_SOC_DATA_DRV_ODT_40_OHM),
	DDR_TIMMING_TUNE(T963_H9_NTC_EID, cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p,
	DDR_SOC_DATA_DRV_ODT_120_OHM),
	DDR_TIMMING_TUNE(T963_H9_NTC_EID, cfg_board_SI_setting_ps[0].dram_data_drv_ohm,
	DDR_DRAM_DDR4_DRV_34_OHM),
	DDR_TIMMING_TUNE(T963_H9_NTC_EID, cfg_board_SI_setting_ps[0].dram_data_odt_ohm,
	DDR_DRAM_DDR4_ODT_40_OHM),
	DDR_TIMMING_TUNE(T963_H9_NTC_EID, cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm,
	DDR_DRAM_DDR4_WR_ODT_240_OHM),

	//ak321,use ADC_CH1,DDR_ID=0;
	DDR_TIMMING_TUNE(DDR_ADC_CH1 + DDR_ADC_VALUE0,
		cfg_board_common_setting.dram_cs0_size_MB, CONFIG_DDR0_SIZE_1024MB),

	//ak321,use ADC_CH1,DDR_ID=2;DDR_ID_DDR4_2PCS_3GB
	DDR_TIMMING_TUNE(DDR_ADC_CH1 + DDR_ADC_VALUE2,
		cfg_board_common_setting.dram_rank_config, CONFIG_DDR0_32BIT_16BIT_RANK0_CH0),
	DDR_TIMMING_TUNE(DDR_ADC_CH1 + DDR_ADC_VALUE2,
		cfg_board_common_setting.dram_cs0_size_MB, CONFIG_DDR0_SIZE_3072MB),

	//ak321,use ADC_CH1,DDR_ID=3;DDR_ID_DDR4_2PCS_1_5GB
	DDR_TIMMING_TUNE(DDR_ADC_CH1 + DDR_ADC_VALUE3,
		cfg_board_common_setting.dram_rank_config, CONFIG_DDR0_32BIT_16BIT_RANK0_CH0),
	DDR_TIMMING_TUNE(DDR_ADC_CH1 + DDR_ADC_VALUE3,
		cfg_board_common_setting.dram_cs0_size_MB, CONFIG_DDR0_SIZE_1536MB),

	//ak321,use ADC_CH1,DDR_ID=4;DDR_ID_DDR4_2PCS_4GB
	DDR_TIMMING_TUNE(DDR_ADC_CH1 + DDR_ADC_VALUE4,
		cfg_board_common_setting.dram_cs0_size_MB, CONFIG_DDR0_SIZE_4096MB),

	{0, 0, 0, 0, 0, 0},
};

#define VCCK_VAL				CONFIG_VCCK_INIT_VOLTAGE
#define VDDEE_VAL				CONFIG_VDDEE_INIT_VOLTAGE
/* VCCK PWM table */
#if   (VCCK_VAL == 800)
	#define VCCK_VAL_REG	0x0018000A
#elif (VCCK_VAL == 810)
	#define VCCK_VAL_REG	0x0017000B
#elif (VCCK_VAL == 820)
	#define VCCK_VAL_REG	0x0016000C
#elif (VCCK_VAL == 830)
	#define VCCK_VAL_REG	0x0015000D
#elif (VCCK_VAL == 840)
	#define VCCK_VAL_REG	0x0014000E
#elif (VCCK_VAL == 850)
	#define VCCK_VAL_REG	0x0013000F
#elif (VCCK_VAL == 860)
	#define VCCK_VAL_REG	0x00120010
#elif (VCCK_VAL == 870)
	#define VCCK_VAL_REG	0x00110011
#elif (VCCK_VAL == 880)
	#define VCCK_VAL_REG	0x00100012
#elif (VCCK_VAL == 890)
	#define VCCK_VAL_REG	0x000F0013
#elif (VCCK_VAL == 900)
	#define VCCK_VAL_REG	0x000E0014
#elif (VCCK_VAL == 910)
	#define VCCK_VAL_REG	0x000D0015
#elif (VCCK_VAL == 920)
	#define VCCK_VAL_REG	0x000C0016
#elif (VCCK_VAL == 930)
	#define VCCK_VAL_REG	0x000B0017
#elif (VCCK_VAL == 940)
	#define VCCK_VAL_REG	0x000A0018
#elif (VCCK_VAL == 950)
	#define VCCK_VAL_REG	0x00090019
#elif (VCCK_VAL == 960)
	#define VCCK_VAL_REG	0x0008001A
#elif (VCCK_VAL == 970)
	#define VCCK_VAL_REG	0x0007001B
#elif (VCCK_VAL == 980)
	#define VCCK_VAL_REG	0x0006001C
#elif (VCCK_VAL == 990)
	#define VCCK_VAL_REG	0x0005001D
#elif (VCCK_VAL == 1000)
	#define VCCK_VAL_REG	0x0004001E
#elif (VCCK_VAL == 1010)
	#define VCCK_VAL_REG	0x0003001F
#elif (VCCK_VAL == 1020)
	#define VCCK_VAL_REG	0x00020020
#elif (VCCK_VAL == 1030)
	#define VCCK_VAL_REG	0x00010021
#elif (VCCK_VAL == 1040)
	#define VCCK_VAL_REG	0x00000022
#else
	#error "VCCK val out of range"
#endif

/* VDDEE PWM table */
#if   (VDDEE_VAL == 730)
	#define PWM_CONSTANT_OUT
	#define VDDEE_VAL_REG	0x00140000
#elif (VDDEE_VAL == 740)
	#define VDDEE_VAL_REG	0x00120000
#elif (VDDEE_VAL == 750)
	#define VDDEE_VAL_REG	0x00110001
#elif (VDDEE_VAL == 760)
	#define VDDEE_VAL_REG	0x00100002
#elif (VDDEE_VAL == 770)
	#define VDDEE_VAL_REG	0x000f0003
#elif (VDDEE_VAL == 780)
	#define VDDEE_VAL_REG	0x000e0004
#elif (VDDEE_VAL == 790)
	#define VDDEE_VAL_REG	0x000d0005
#elif (VDDEE_VAL == 800)
	#define VDDEE_VAL_REG	0x000c0006
#elif (VDDEE_VAL == 810)
	#define VDDEE_VAL_REG	0x000b0007
#elif (VDDEE_VAL == 820)
	#define VDDEE_VAL_REG	0x000a0008
#elif (VDDEE_VAL == 830)
	#define VDDEE_VAL_REG	0x00090009
#elif (VDDEE_VAL == 840)
	#define VDDEE_VAL_REG	0x0008000a
#elif (VDDEE_VAL == 850)
	#define VDDEE_VAL_REG	0x0007000b
#elif (VDDEE_VAL == 860)
	#define VDDEE_VAL_REG	0x0006000c
#elif (VDDEE_VAL == 870)
	#define VDDEE_VAL_REG	0x0005000d
#elif (VDDEE_VAL == 880)
	#define VDDEE_VAL_REG	0x0004000e
#elif (VDDEE_VAL == 890)
	#define VDDEE_VAL_REG	0x0003000f
#elif (VDDEE_VAL == 900)
	#define VDDEE_VAL_REG	0x00020010
#elif (VDDEE_VAL == 910)
	#define VDDEE_VAL_REG	0x00010011
#elif (VDDEE_VAL == 920)
	#define VDDEE_VAL_REG	0x00000012
#elif (VDDEE_VAL == 930)
	#define PWM_CONSTANT_OUT
	#define VDDEE_VAL_REG	0x00000014
#else
	#error "VDDEE val out of range"
#endif

/* for PWM use */
/* PWM driver check http://scgit.amlogic.com:8080/#/c/38093/ */
#define PWMAB_PWM_A			((0x0000 << 2) + 0xffd1b000)
#define PWMAB_PWM_B			((0x0001 << 2) + 0xffd1b000)
#define PWMAB_MISC_REG_AB	((0x0002 << 2) + 0xffd1b000)

bl2_reg_t __bl2_reg[] = {
	/* demo, user defined override register */
	/* eg: PWM init */
	/* PWM_A VDDEE_VAL_REG */
#ifdef CONFIG_PDVFS_ENABLE
	{PWMAB_PWM_A, 0x000b0007, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_1, 0},
	{PWMAB_PWM_A, 0x0008000a, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_2, 0},
	{PWMAB_PWM_A, 0x0008000a, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_3, 0},
#else
	{PWMAB_PWM_A,		VDDEE_VAL_REG,	0xffffffff,	0,	BL2_INIT_STAGE_1, 0},
#endif

	/* PWM_B VCCK_VAL_REG */
	{PWMAB_PWM_B,		VCCK_VAL_REG,	0xffffffff,	0,	BL2_INIT_STAGE_1, 0},
	/*pwm needs to support full-height or full-low output*/
#ifdef PWM_CONSTANT_OUT
	{PWMAB_MISC_REG_AB,	((3 << 0) | (1 << 15) | (1 << 23) | (1 << 28)),  0xffffffff, 0, BL2_INIT_STAGE_1, 0},
#else
	{PWMAB_MISC_REG_AB,	((3 << 0) | (1 << 15) | (1 << 23)), 0xffffffff, 0, BL2_INIT_STAGE_1, 0},
#endif
	/* SET GPIOE_0 GPIOE_1 DS to 3 */
	{AO_PAD_DS_B,		(0xF << 0), (0xF << 0),		0,	BL2_INIT_STAGE_1, 0},
	/* MUX GPIOE_0 to PWM_A GPIOE_1 to PWM_B */
	{AO_RTI_PINMUX_REG1,	((1 << 16) | (1 << 20)), (0xFF << 16), 0, BL2_INIT_STAGE_1, 0},
	/* Enable VCCK SET GPIOD_10 HIGH */
	{AO_RTI_PINMUX_REG1,	(0 << 8),	(0xF << 8), 	0, BL2_INIT_STAGE_1, 0},
	{AO_GPIO_O_EN_N,	(0 << 10),	(1 << 10),	0, BL2_INIT_STAGE_1, 0},
	{AO_GPIO_O,		(1 << 10),	(1 << 10),	0, BL2_INIT_STAGE_1, 0},
	/* Enable 5V_EN ,set TEST_N low --> several millisecond--set high in board_init*/
	//{AO_SEC_REG0,		(0 << 0),	(1 << 0),	0, BL2_INIT_STAGE_1, 0},
	{AO_GPIO_TEST_N,	(0 << 31),	(1UL << 31),	0, BL2_INIT_STAGE_1, 0},
	/* GPIOH_12 pull high */
	//{PREG_PAD_GPIO2_O,     (1 << 12),               (1 << 12),   0, BL2_INIT_STAGE_1, 0},
	//{PREG_PAD_GPIO2_EN_N,  (0 << 12),               (1 << 12),   0, BL2_INIT_STAGE_1, 0},
};
