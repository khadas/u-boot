/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * board/amlogic/t5_ak309_v1/firmware/timing.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <asm/arch/secure_apb.h>
#include <asm/arch/timing.h>
#include <asm/arch/ddr_define.h>

ddr_set_t __ddr_setting[] __attribute__ ((section(".ddr_settings"))) = {
{
	.cfg_board_common_setting.timming_magic								= 0,
	.cfg_board_common_setting.timming_max_valid_configs						= 1,
	.cfg_board_common_setting.timming_struct_version						= 0,
	.cfg_board_common_setting.timming_struct_org_size						= sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_real_size						= 0,                                      //0
	.cfg_board_common_setting.fast_boot								= { 0 },
	.cfg_board_common_setting.fast_boot[0]							= 0x1,
	.cfg_board_common_setting.fast_boot[3]							= 0x46,
	.cfg_board_common_setting.ddr_func								= 0,
	.cfg_board_common_setting.board_id								= CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.DramType								= CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.dram_rank_config							= CONFIG_DDR0_32BIT_RANK01_CH0,
	.cfg_board_common_setting.DisabledDbyte								= CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.dram_cs0_base_add							= 0,
	.cfg_board_common_setting.dram_cs1_base_add							= 0,
	.cfg_board_common_setting.dram_cs0_size_MB							= CONFIG_DDR0_SIZE_1024MB,
	.cfg_board_common_setting.dram_cs1_size_MB							= CONFIG_DDR1_SIZE_1024MB,
	.cfg_board_common_setting.dram_x4x8x16_mode							= CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.Is2Ttiming								= CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.log_level								= LOG_LEVEL_BASIC,
	.cfg_board_common_setting.ddr_rdbi_wr_enable							= DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.pll_ssc_mode								= DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.org_tdqs2dq								= 0,
	.cfg_board_common_setting.reserve1_test_function						= { 0 },
	.cfg_board_common_setting.ddr_dmc_remap								= DDR_DMC_REMAP_DDR4_32BIT,
#if 0
	.cfg_board_common_setting.ac_pinmux								= {
		0,  0,	0,  1,	2,  3,	8, 27,
		10, 2,	9,  21, 5,  14, 1, 0,
		25, 13, 12, 4,	7,  22, 0, 0,
		0,  0,	6,  3,	20, 0,	0, 15,
		26, 11,
	},
#endif
#if 1
	//ak329 ac pinmux
	.cfg_board_common_setting.ac_pinmux								= {
		0,  0,	0,  1,	2,  3,	7, 28,
		20, 9,	2,  21, 6,  10, 0, 1,
		27, 11, 12, 4,	8,  25, 0, 0,
		0,  0,	5,  3,	23, 0,	0, 24,
		26, 13, 0
	},
#endif
#if 0
	SIP package
	2.1 - T963(17.45X16 .8)
	4DDR4 (NTC)

	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		7, 28, 20, 9, 2, 21,6,	10, 0, 1, 27, 11, 12, 4, 8, 25, 0, 0, 0, 0, 5, 3, 23, 0, 0, 24, 26, 13,
	};

	4DDR4 (SAMSUNG)

	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		8, 27, 10, 2, 9, 21,5,	23, 1, 0, 25, 13, 12, 4, 7, 28, 0, 0, 0, 0, 6, 3, 20, 0, 0, 24, 26, 11,
	};

#endif
#if 0
	.cfg_board_common_setting.ac_pinmux								=
	{
		0, 0, 0, 1, 2, 3,
		0, 1, 2, 3, 4, 5,6,  7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 20, 21, 22, 0, 0, 25, 26, 27,
	},

#endif
	.cfg_board_common_setting.ddr_dqs_swap								= 0,
	.cfg_board_common_setting.ddr_dq_remap								= {
		0,  1,	2,  3,	4,  5,	6,  7,
		8,  9,	10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35
	},       //d0-d31 dm0 dm1 dm2 dm3

	.cfg_board_common_setting.ddr_vddee_setting							= { 0 },
	.cfg_board_SI_setting_ps[0].DRAMFreq = 1176,
	.cfg_board_SI_setting_ps[0].PllBypassEn = 0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl = 0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT01_CS1_ODT01__R_CS0_ODT1_CS1_ODT0, //DDR_DRAM_ODT_W_CS0_ODT0,//DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,//DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_60_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_120_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,            //for pxp dram weak driver model
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_240_OHM,
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
		256, 256, 256, 256, 256, 256+100, 256, 256,
		256, 256, 256, 256, 256, 256, 256, 256,
		256, 256, 256, 256, 256+30, 256, 256, 256,
		256, 256, 256+100, 256,
	}, //total 36
#if 0
	.cfg_ddr_training_delay_ps[0].write_dqs_delay = {
		128, 128, 128, 128, 128, 128, 128, 128
	},
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay = {
		192, 192, 192, 192, 192, 192, 192, 192,
		192, 192, 192, 192, 192, 192, 192, 192,
		192, 192, 192, 192, 192, 192, 192, 192,
		192, 192, 192, 192, 192, 192, 192, 192,
		192, 192, 192, 192, 192, 192, 192, 192,
		192, 192, 192, 192, 192, 192, 192, 192,
		192, 192, 192, 192, 192, 192, 192, 192,
		192, 192, 192, 192, 192, 192, 192, 192,
		192, 192, 192, 192, 192, 192, 192, 192,
	},
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay = {
		0, 0, 0, 0, 0, 0, 0, 0
	},         //total 8  //edge gate
	.cfg_ddr_training_delay_ps[0].read_dqs_delay = {
		64, 64, 64, 64, 64, 64, 64, 64
	},      //total 8
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay = {
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
	.cfg_ddr_training_delay_ps[0].soc_bit_vref = {
		0,  40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 50, 40, 50,
		40, 50, 40, 50
	}, //total 44
	.cfg_ddr_training_delay_ps[0].dram_bit_vref = {
		0,  32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32
	}, //total 36
#endif
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0] = 0x0000009b,          // 155
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1] = 0x000000a5,          // 165
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2] = 0x0000009b,          // 155
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3] = 0x0000008c,          // 140
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4] = 0x0000009b,          // 155
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5] = 0x0000009b,          // 155
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6] = 0x0000009b,          // 155
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7] = 0x00000091,          // 145
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0] = 0x000000e4,       // 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1] = 0x000000cc,       // 204
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2] = 0x000000dc,       // 220
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3] = 0x000000ce,       // 206
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4] = 0x000000e6,       // 230
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5] = 0x000000d0,       // 208
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6] = 0x000000de,       // 222
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7] = 0x000000dc,       // 220
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8] = 0x000000d5,       // 213
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9] = 0x000000e6,       // 230
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10] = 0x000000d7,      // 215
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11] = 0x000000e6,      // 230
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12] = 0x000000d8,      // 216
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13] = 0x000000ee,      // 238
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14] = 0x000000de,      // 222
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15] = 0x000000e9,      // 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16] = 0x000000e5,      // 229
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17] = 0x000000e2,      // 226
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18] = 0x000000da,      // 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19] = 0x000000ca,      // 202
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20] = 0x000000d6,      // 214
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21] = 0x000000c9,      // 201
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22] = 0x000000d4,      // 212
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23] = 0x000000b9,      // 185
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24] = 0x000000c8,      // 200
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25] = 0x000000c5,      // 197
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26] = 0x000000c9,      // 201
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27] = 0x000000d7,      // 215
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28] = 0x000000c7,      // 199
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29] = 0x000000de,      // 222
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30] = 0x000000c8,      // 200
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31] = 0x000000d4,      // 212
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32] = 0x000000cc,      // 204
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33] = 0x000000d1,      // 209
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34] = 0x000000c1,      // 193
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35] = 0x000000ce,      // 206
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[36] = 0x000000e4,      // 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[37] = 0x000000cc,      // 204
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[38] = 0x000000dd,      // 221
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[39] = 0x000000d2,      // 210
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[40] = 0x000000e7,      // 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[41] = 0x000000d1,      // 209
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[42] = 0x000000de,      // 222
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[43] = 0x000000dc,      // 220
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[44] = 0x000000d9,      // 217
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[45] = 0x000000dc,      // 220
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[46] = 0x000000ca,      // 202
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[47] = 0x000000dc,      // 220
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[48] = 0x000000cf,      // 207
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[49] = 0x000000e6,      // 230
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[50] = 0x000000d5,      // 213
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[51] = 0x000000e2,      // 226
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[52] = 0x000000db,      // 219
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[53] = 0x000000d7,      // 215
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[54] = 0x000000da,      // 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[55] = 0x000000c8,      // 200
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[56] = 0x000000d3,      // 211
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[57] = 0x000000c8,      // 200
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[58] = 0x000000d5,      // 213
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[59] = 0x000000b6,      // 182
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[60] = 0x000000c5,      // 197
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[61] = 0x000000c3,      // 195
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[62] = 0x000000c8,      // 200
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[63] = 0x000000d4,      // 212
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[64] = 0x000000c5,      // 197
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[65] = 0x000000dc,      // 220
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[66] = 0x000000c9,      // 201
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[67] = 0x000000d0,      // 208
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[68] = 0x000000cb,      // 203
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[69] = 0x000000cf,      // 207
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[70] = 0x000000bf,      // 191
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[71] = 0x000000cc,      // 204
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0] = 0x00000322,      // 802
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1] = 0x00000313,      // 787
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2] = 0x0000031d,      // 797
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3] = 0x00000334,      // 820
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4] = 0x0000031f,      // 799
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5] = 0x00000311,      // 785
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6] = 0x0000031c,      // 796
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7] = 0x00000336,      // 822
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0] = 0x00000080,           // 114
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1] = 0x00000080,           // 113
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2] = 0x00000080,           // 103
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3] = 0x00000080,           // 101
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4] = 0x00000080,           // 121
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5] = 0x00000080,           // 116
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6] = 0x00000080,           // 124
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7] = 0x00000080,           // 102
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0] = 0x0000004e,        // 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1] = 0x00000036,        // 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2] = 0x0000004d,        // 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3] = 0x00000036,        // 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4] = 0x00000051,        // 81
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5] = 0x0000003c,        // 60
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6] = 0x00000054,        // 84
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7] = 0x00000036,        // 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8] = 0x00000043,        // 67
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9] = 0x0000004f,        // 79
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10] = 0x0000003a,       // 58
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11] = 0x0000004b,       // 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12] = 0x0000003b,       // 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13] = 0x00000051,       // 81
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14] = 0x0000003f,       // 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15] = 0x0000004b,       // 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16] = 0x00000042,       // 66
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17] = 0x00000045,       // 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18] = 0x00000049,       // 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19] = 0x00000031,       // 49
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20] = 0x00000044,       // 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21] = 0x0000002f,       // 47
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22] = 0x00000046,       // 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23] = 0x0000002d,       // 45
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24] = 0x0000003f,       // 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25] = 0x00000031,       // 49
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26] = 0x0000003a,       // 58
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27] = 0x00000050,       // 80
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28] = 0x00000033,       // 51
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29] = 0x00000041,       // 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30] = 0x00000035,       // 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31] = 0x00000043,       // 67
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32] = 0x00000033,       // 51
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33] = 0x00000046,       // 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34] = 0x00000033,       // 51
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35] = 0x0000003d,       // 61
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
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[0] = (915 - 240) / 15,       // 43 (915-240)/15
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[1] = (915 - 240) / 15,       // 43
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[2] = (915 - 240) / 15,       // 42
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[3] = (915 - 240) / 15,       // 42
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[4] = (915 - 240) / 15,       // 44
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[5] = (915 - 240) / 15,       // 41
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[6] = (915 - 240) / 15,       // 43
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[7] = (915 - 240) / 15,       // 43
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[8] = (915 - 240) / 15,       // 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[9] = (915 - 240) / 15,       // 41
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[10] = (915 - 240) / 15,      // 42
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[11] = (915 - 240) / 15,      // 40
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[12] = (915 - 240) / 15,      // 42
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[13] = (915 - 240) / 15,      // 42
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[14] = (915 - 240) / 15,      // 42
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[15] = (915 - 240) / 15,      // 43
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[16] = (915 - 240) / 15,      // 41
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[17] = (915 - 240) / 15,      // 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[18] = (915 - 240) / 15,      // 44
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[19] = (915 - 240) / 15,      // 43
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[20] = (915 - 240) / 15,      // 44
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[21] = (915 - 240) / 15,      // 42
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[22] = (915 - 240) / 15,      // 43
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[23] = (915 - 240) / 15,      // 43
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[24] = (915 - 240) / 15,      // 44
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[25] = (915 - 240) / 15,      // 43
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[26] = (915 - 240) / 15,      // 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[27] = (915 - 240) / 15,      // 44
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[28] = (915 - 240) / 15,      // 43
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[29] = (915 - 240) / 15,      // 42
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[30] = (915 - 240) / 15,      // 41
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[31] = (915 - 240) / 15,      // 43
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[32] = 0x0000002b,            // 43
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[33] = 0x0000002b,            // 43
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[34] = 0x0000002b,            // 43
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[35] = 0x00000026,            // 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[36] = 0x00000026,            // 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[37] = 0x00000035,            // 53
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[38] = 0x00000026,            // 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[39] = 0x00000033,            // 51
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[40] = 0x00000026,            // 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[41] = 0x00000036,            // 54
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[42] = 0x00000026,            // 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[43] = 0x00000036,            // 54

#if 1
	.cfg_ddr_training_delay_ps[0].reserve_training_parameter = {
		(0 << 7) | 0,  (0 << 7) | 0,  (0 << 7) | 0,  (0 << 7) | 5,
		(1 << 7) | 0,  (0 << 7) | 0,  (0 << 7) | 0,  (0 << 7) | 5,
		(1 << 7) | 15, (1 << 7) | 15, (1 << 7) | 10, (1 << 7) | 10,
		(1 << 7) | 15, (1 << 7) | 15, (1 << 7) | 15, (1 << 7) | 15,
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
}
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

};

ddr_reg_t __ddr_reg[] = {
	/* demo, user defined override register */
	{0xaabbccdd, 0, 0, 0, 0, 0},
	{0x11223344, 0, 0, 0, 0, 0},
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
#else
	#error "VCCK val out of range\n"
#endif

/* VDDEE PWM table */
#if   (VDDEE_VAL == 750)
	#define VDDEE_VAL_REG	0x00120000
#elif (VDDEE_VAL == 760)
	#define VDDEE_VAL_REG	0x00110001
#elif (VDDEE_VAL == 770)
	#define VDDEE_VAL_REG	0x00100002
#elif (VDDEE_VAL == 780)
	#define VDDEE_VAL_REG	0x000f0003
#elif (VDDEE_VAL == 790)
	#define VDDEE_VAL_REG	0x000e0004
#elif (VDDEE_VAL == 800)
	#define VDDEE_VAL_REG	0x000d0005
#elif (VDDEE_VAL == 810)
	#define VDDEE_VAL_REG	0x000c0006
#elif (VDDEE_VAL == 820)
	#define VDDEE_VAL_REG	0x000b0007
#elif (VDDEE_VAL == 830)
	#define VDDEE_VAL_REG	0x000a0008
#elif (VDDEE_VAL == 840)
	#define VDDEE_VAL_REG	0x00090009
#elif (VDDEE_VAL == 850)
	#define VDDEE_VAL_REG	0x0008000a
#elif (VDDEE_VAL == 860)
	#define VDDEE_VAL_REG	0x0007000b
#elif (VDDEE_VAL == 870)
	#define VDDEE_VAL_REG	0x0006000c
#elif (VDDEE_VAL == 880)
	#define VDDEE_VAL_REG	0x0005000d
#elif (VDDEE_VAL == 890)
	#define VDDEE_VAL_REG	0x0004000e
#elif (VDDEE_VAL == 900)
	#define VDDEE_VAL_REG	0x0003000f
#elif (VDDEE_VAL == 910)
	#define VDDEE_VAL_REG	0x00020010
#elif (VDDEE_VAL == 920)
	#define VDDEE_VAL_REG	0x00010011
#elif (VDDEE_VAL == 930)
	#define VDDEE_VAL_REG	0x00000012
#else
	#error "VDDEE val out of range\n"
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
	{PWMAB_PWM_A,		VDDEE_VAL_REG,	0xffffffff,	0,	BL2_INIT_STAGE_1, 0},
	/* PWM_B VCCK_VAL_REG */
	{PWMAB_PWM_B,		VCCK_VAL_REG,	0xffffffff,	0,	BL2_INIT_STAGE_1, 0},
	{PWMAB_MISC_REG_AB,	((3 << 0) | (1 << 15) | (1 << 23)),  0xffffffff, 0, BL2_INIT_STAGE_1, 0},
	/* SET GPIOE_0 GPIOE_1 DS to 3 */
	{AO_PAD_DS_B,		(0xF << 0), (0xF << 0),		0,	BL2_INIT_STAGE_1, 0},
	/* MUX GPIOE_0 to PWM_A GPIOE_1 to PWM_B */
	{AO_RTI_PINMUX_REG1,	((1 << 16) | (1 << 20)), (0xFF << 16), 0, BL2_INIT_STAGE_1, 0},
	/* Enable VCCK SET GPIOD_10 HIGH */
	{AO_RTI_PINMUX_REG1,	(0 << 8),	(0xF << 8), 	0, BL2_INIT_STAGE_1, 0},
	{AO_GPIO_O_EN_N,	(0 << 10),	(1 << 10),	0, BL2_INIT_STAGE_1, 0},
	{AO_GPIO_O,		(1 << 10),	(1 << 10),	0, BL2_INIT_STAGE_1, 0},
	/* Enable 5V_EN ,set gpioao_3 low --> several millisecond--set high in board_init*/
	//{AO_GPIO_O_EN_N,    (0 << 3),                (1 << 3),     0, BL2_INIT_STAGE_1, 0},
	///{AO_GPIO_O,         (0 << 3),                (1 << 3),   0, BL2_INIT_STAGE_1, 0},
	/* GPIOH_12 pull high */
	//{PREG_PAD_GPIO2_O,     (1 << 12),               (1 << 12),   0, BL2_INIT_STAGE_1, 0},
	//{PREG_PAD_GPIO2_EN_N,  (0 << 12),               (1 << 12),   0, BL2_INIT_STAGE_1, 0},
};