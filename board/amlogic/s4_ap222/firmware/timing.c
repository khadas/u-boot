/*
 * board/amlogic/s4_ap222/firmware/timing.c
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
#define CPU_CLK					1512

ddr_set_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {
#if 1
{
	//timing_config,4layer 4pcs ddr4 rank01, ap222.
	.cfg_board_common_setting.timming_magic					= 0,
	.cfg_board_common_setting.timming_max_valid_configs			= sizeof(__ddr_setting[0]) / sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_version			= 0,
	.cfg_board_common_setting.timming_struct_org_size			= sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_real_size			= 0,                                       //0
	.cfg_board_common_setting.fast_boot					= { 0,					   0,(1 << 3) | (2) },
	.cfg_board_common_setting.ddr_func					= 0,
	.cfg_board_common_setting.board_id					= CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.DramType					= CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.dram_rank_config				= CONFIG_DDR0_32BIT_RANK01_CH0,
	.cfg_board_common_setting.DisabledDbyte					= CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.dram_cs0_base_add				= 0,
	.cfg_board_common_setting.dram_cs1_base_add				= 0,
	.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,
	.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_1024MB,
	.cfg_board_common_setting.dram_x4x8x16_mode				= CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.Is2Ttiming					= CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.log_level					= LOG_LEVEL_BASIC,
	.cfg_board_common_setting.ddr_rdbi_wr_enable				= DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.pll_ssc_mode					= DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.org_tdqs2dq					= 0,
	.cfg_board_common_setting.reserve1_test_function			= { 0 },
	.cfg_board_common_setting.ddr_dmc_remap					= DDR_DMC_REMAP_DDR4_32BIT,
	//af419 ac pinmux
	#if 0
	.cfg_board_common_setting.ac_pinmux					=
	{
		0,  0, 0,  1,  2,  3,  27, 12,
		21, 9, 8,  0,  14, 10, 6,  7,
		20, 5, 22, 13, 15, 2,  0,  0,
		0,  0, 11, 26, 4,  0,  0,  25,
		3,  1,
	},
	#endif
	//s4 ddr4 ac pinmux
	.cfg_board_common_setting.ac_pinmux					=
	{
		0,	0,	0,	1,	2,	3,
		27,	10,	25,	5,	28,	11,	24,	0,	26,	7,	4,	21,	2,	20,	8,	13,	0,	0,	0,	0,	6,	12,	3,	0,	0,	9,	1,	23,	0
	},
	.cfg_board_common_setting.ddr_dqs_swap					= 0,
	.cfg_board_common_setting.ddr_dq_remap					=
	{
		0,  1,	2,  3,	4,  5,	6,  7,
		8,  9,	10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35
	},                                                           //d0-d31 dm0 dm1 dm2 dm3
	.cfg_board_common_setting.ddr_vddee_setting				= { 0 },
	.cfg_board_SI_setting_ps[0].DRAMFreq = 1176,
	.cfg_board_SI_setting_ps[0].PllBypassEn = 0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl = 0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT01_CS1_ODT01__R_CS0_ODT1_CS1_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_60_OHM,
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
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org =
	{
		128, 128, 128 - 40, 128, 128, 128, 128, 128,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384,
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[0].ac_trace_delay =
	{
		128, 128, 128 - 40, 128, 128, 128, 128, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384,
	},

	#if 1
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=0x000000e1,// 225
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=0x000000eb,// 235
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=0x000000e1,// 225
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=0x000000d7,// 215
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x000000d7,// 215
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x000000e1,// 225
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x000000e1,// 225
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x000000d7,// 215
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=0x00000130,// 304
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=0x00000120,// 288
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=0x00000132,// 306
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=0x00000125,// 293
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=0x0000012b,// 299
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=0x00000115,// 277
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=0x00000131,// 305
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=0x0000012d,// 301
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=0x00000124,// 292
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=0x00000134,// 308
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=0x00000115,// 277
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=0x00000127,// 295
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=0x0000011f,// 287
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=0x00000141,// 321
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=0x00000129,// 297
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=0x00000137,// 311
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=0x00000120,// 288
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=0x0000012a,// 298
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=0x00000127,// 295
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=0x00000112,// 274
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=0x00000119,// 281
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=0x00000115,// 277
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=0x0000012b,// 299
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=0x0000011c,// 284
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=0x00000116,// 278
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=0x00000120,// 288
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=0x0000011f,// 287
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=0x0000012b,// 299
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=0x00000113,// 275
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=0x0000011f,// 287
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=0x00000117,// 279
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=0x00000126,// 294
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=0x00000117,// 279
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=0x0000011c,// 284
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=0x00000113,// 275
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=0x0000011f,// 287
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[36]=0x0000012d,// 301
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[37]=0x0000011f,// 287
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[38]=0x00000125,// 293
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[39]=0x0000011e,// 286
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[40]=0x0000011a,// 282
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[41]=0x00000108,// 264
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[42]=0x00000121,// 289
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[43]=0x0000011f,// 287
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[44]=0x0000011b,// 283
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[45]=0x0000012a,// 298
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[46]=0x0000010c,// 268
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[47]=0x00000118,// 280
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[48]=0x00000115,// 277
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[49]=0x0000012f,// 303
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[50]=0x0000011f,// 287
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[51]=0x00000126,// 294
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[52]=0x00000118,// 280
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[53]=0x0000011d,// 285
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[54]=0x00000139,// 313
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[55]=0x0000011b,// 283
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[56]=0x0000011e,// 286
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[57]=0x00000123,// 291
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[58]=0x00000127,// 295
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[59]=0x00000118,// 280
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[60]=0x0000011b,// 283
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[61]=0x0000011e,// 286
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[62]=0x0000012d,// 301
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[63]=0x0000012c,// 300
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[64]=0x00000117,// 279
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[65]=0x00000122,// 290
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[66]=0x00000118,// 280
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[67]=0x00000127,// 295
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[68]=0x00000119,// 281
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[69]=0x0000011c,// 284
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[70]=0x00000118,// 280
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[71]=0x00000122,// 290
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=0x00000376,// 886
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=0x00000372,// 882
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=0x00000366,// 870
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=0x0000036c,// 876
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000378,// 888
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000373,// 883
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x0000036d,// 877
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000371,// 881
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=0x00000063,// 99
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=0x00000075,// 117
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=0x00000060,// 96
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000071,// 113
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000086,// 134
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000079,// 121
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000086,// 134
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=0x00000028,// 40
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=0x0000003a,// 58
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=0x0000002e,// 46
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=0x00000033,// 51
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=0x00000028,// 40
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=0x0000002f,// 47
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=0x00000031,// 49
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=0x0000003b,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=0x0000002a,// 42
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=0x0000002c,// 44
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=0x0000004a,// 74
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=0x00000034,// 52
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=0x00000042,// 66
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=0x0000002d,// 45
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=0x0000003b,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=0x0000002b,// 43
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=0x0000002c,// 44
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=0x0000004a,// 74
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=0x0000003b,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=0x00000037,// 55
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=0x00000037,// 55
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=0x00000027,// 39
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=0x00000032,// 50
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=0x0000002b,// 43
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=0x0000003e,// 62
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=0x00000029,// 41
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=0x00000031,// 49
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=0x0000002a,// 42
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=0x0000002f,// 47
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[36]=0x0000004a,// 74
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[37]=0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[38]=0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[39]=0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[40]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[41]=0x00000031,// 49
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[42]=0x00000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[43]=0x00000040,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[44]=0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[45]=0x00000050,// 80
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[46]=0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[47]=0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[48]=0x0000003a,// 58
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[49]=0x00000060,// 96
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[50]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[51]=0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[52]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[53]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[54]=0x0000005b,// 91
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[55]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[56]=0x00000053,// 83
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[57]=0x00000043,// 67
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[58]=0x00000052,// 82
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[59]=0x00000043,// 67
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[60]=0x00000051,// 81
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[61]=0x00000037,// 55
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[62]=0x0000004a,// 74
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[63]=0x00000053,// 83
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[64]=0x00000037,// 55
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[65]=0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[66]=0x0000003e,// 62
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[67]=0x00000057,// 87
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[68]=0x0000003e,// 62
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[69]=0x0000004a,// 74
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[70]=0x00000040,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[71]=0x00000046,// 70
	#endif
	                                                //total 72
	.cfg_ddr_training_delay_ps[0].soc_bit_vref =
	{
		0,  40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 48, 40, 48,
		40, 48, 40, 48
	},                                                     //total 44
	.cfg_ddr_training_delay_ps[0].dram_bit_vref =
	{
		0,  32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[0].reserve_training_parameter = { 0 },

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
	.cfg_board_SI_setting_ps[1].ac_trace_delay_org =
	{
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128,
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[1].ac_trace_delay =
	{
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128,
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[1].write_dqs_delay =
	{
		0, 0, 0, 0, 0, 0, 0, 0
	},
	.cfg_ddr_training_delay_ps[1].write_dq_bit_delay =
	{
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
	.cfg_ddr_training_delay_ps[1].read_dqs_gate_delay =
	{
		192, 192, 192, 192, 192, 192, 192, 192
	},                                                     //total 8
	.cfg_ddr_training_delay_ps[1].read_dqs_delay =
	{
		64, 64, 64, 64, 64, 64, 64, 64
	},                                                                  //total 8
	.cfg_ddr_training_delay_ps[1].read_dq_bit_delay =
	{
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},                                                     //total 72
	.cfg_ddr_training_delay_ps[1].soc_bit_vref =
	{
		0,  40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40
	},                                                     //total 44
	.cfg_ddr_training_delay_ps[1].dram_bit_vref =
	{
		0,  32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[1].reserve_training_parameter = { 0 },
},
#endif
#if 0
{
	//timing_config,4layer 2pcs ddr4 rank0, gangzhen pcb.
	.cfg_board_common_setting.timming_magic					= 0,
	.cfg_board_common_setting.timming_max_valid_configs			= sizeof(__ddr_setting[0]) / sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_version			= 0,
	.cfg_board_common_setting.timming_struct_org_size			= sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_real_size			= 0,                                       //0
	.cfg_board_common_setting.fast_boot					= { 0,					   0,(1 << 3) | (4) },
	.cfg_board_common_setting.ddr_func					= 0,
	.cfg_board_common_setting.board_id					= CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.DramType					= CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.dram_rank_config				= CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.DisabledDbyte					= CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.dram_cs0_base_add				= 0,
	.cfg_board_common_setting.dram_cs1_base_add				= 0,
	.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_2048MB,
	.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.dram_x4x8x16_mode				= CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.Is2Ttiming					= CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.log_level					= LOG_LEVEL_BASIC,
	.cfg_board_common_setting.ddr_rdbi_wr_enable				= DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.pll_ssc_mode					= DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.org_tdqs2dq					= 0,
	.cfg_board_common_setting.reserve1_test_function			= { 0 },
	.cfg_board_common_setting.ddr_dmc_remap					= DDR_DMC_REMAP_DDR4_32BIT,
	//af419 ac pinmux
	#if 0
	.cfg_board_common_setting.ac_pinmux					=
	{
		0,  0, 0,  1,  2,  3,  27, 12,
		21, 9, 8,  0,  14, 10, 6,  7,
		20, 5, 22, 13, 15, 2,  0,  0,
		0,  0, 11, 26, 4,  0,  0,  25,
		3,  1,
	},
	#endif
	//s4 ddr4 ac pinmux
	.cfg_board_common_setting.ac_pinmux					=
	{
		0,	0,	0,	1,	2,	3,
		27,	10,	25,	5,	28,	11,	24,	0,	26,	7,	4,	21,	2,	20,	8,	13,	0,	0,	0,	0,	6,	12,	3,	0,	0,	9,	1,	23,	0
	},
	.cfg_board_common_setting.ddr_dqs_swap					= 0,
	.cfg_board_common_setting.ddr_dq_remap					=
	{
		0,  1,	2,  3,	4,  5,	6,  7,
		8,  9,	10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35
	},                                                           //d0-d31 dm0 dm1 dm2 dm3
	.cfg_board_common_setting.ddr_vddee_setting				= { 0 },
	.cfg_board_SI_setting_ps[0].DRAMFreq = 1320,
	.cfg_board_SI_setting_ps[0].PllBypassEn = 0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl = 0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
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
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org =
	{
		128, 128, 128 - 40, 128, 128, 128, 128, 128,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384,
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[0].ac_trace_delay =
	{
		128, 128, 128 - 40, 128, 128, 128, 128, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384,
	},

	#if 1
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=0x000000c8,// 200
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=0x000000c3,// 195
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=0x000000b4,// 180
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=0x000000af,// 175
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=0x0000010b,// 267
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=0x000000f7,// 247
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=0x00000105,// 261
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=0x00000100,// 256
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=0x000000e4,// 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=0x000000ff,// 255
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=0x000000fe,// 254
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=0x000000f8,// 248
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=0x0000010e,// 270
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=0x00000106,// 262
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=0x000000fa,// 250
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=0x00000100,// 256
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=0x000000fa,// 250
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=0x00000107,// 263
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=0x000000fd,// 253
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=0x00000100,// 256
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=0x000000ff,// 255
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=0x000000ef,// 239
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=0x00000100,// 256
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=0x000000f8,// 248
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=0x000000f2,// 242
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=0x000000fa,// 250
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=0x00000111,// 273
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=0x00000103,// 259
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=0x0000010c,// 268
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=0x000000f6,// 246
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=0x000000ff,// 255
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[36]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[37]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[38]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[39]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[40]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[41]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[42]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[43]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[44]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[45]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[46]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[47]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[48]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[49]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[50]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[51]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[52]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[53]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[54]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[55]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[56]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[57]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[58]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[59]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[60]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[61]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[62]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[63]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[64]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[65]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[66]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[67]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[68]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[69]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[70]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[71]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=0x0000032d,// 813
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=0x0000032a,// 810
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=0x00000337,// 823
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=0x00000344,// 836
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=0x0000008c,// 140
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=0x00000078,// 120
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=0x0000007a,// 122
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=0x00000063,// 99
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=0x00000050,// 80
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=0x0000003c,// 60
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=0x00000033,// 51
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=0x0000003a,// 58
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=0x00000043,// 67
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=0x00000053,// 83
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=0x0000002e,// 46
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=0x0000003a,// 58
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=0x00000051,// 81
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=0x0000003c,// 60
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=0x00000040,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=0x00000050,// 80
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=0x0000002f,// 47
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=0x00000051,// 81
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=0x00000043,// 67
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=0x00000054,// 84
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=0x00000030,// 48
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=0x00000043,// 67
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=0x00000030,// 48
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=0x00000051,// 81
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=0x00000032,// 50
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=0x00000030,// 48
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=0x0000003d,// 61
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[36]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[37]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[38]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[39]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[40]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[41]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[42]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[43]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[44]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[45]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[46]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[47]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[48]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[49]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[50]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[51]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[52]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[53]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[54]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[55]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[56]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[57]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[58]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[59]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[60]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[61]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[62]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[63]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[64]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[65]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[66]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[67]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[68]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[69]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[70]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[71]=0x00000000,// 0
	#endif
	                                                //total 72
	.cfg_ddr_training_delay_ps[0].soc_bit_vref =
	{
		0,  40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 48, 40, 48,
		40, 48, 40, 48
	},                                                     //total 44
	.cfg_ddr_training_delay_ps[0].dram_bit_vref =
	{
		0,  32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[0].reserve_training_parameter = { 0 },

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
	.cfg_board_SI_setting_ps[1].ac_trace_delay_org =
	{
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128,
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[1].ac_trace_delay =
	{
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128,
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[1].write_dqs_delay =
	{
		0, 0, 0, 0, 0, 0, 0, 0
	},
	.cfg_ddr_training_delay_ps[1].write_dq_bit_delay =
	{
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
	.cfg_ddr_training_delay_ps[1].read_dqs_gate_delay =
	{
		192, 192, 192, 192, 192, 192, 192, 192
	},                                                     //total 8
	.cfg_ddr_training_delay_ps[1].read_dqs_delay =
	{
		64, 64, 64, 64, 64, 64, 64, 64
	},                                                                  //total 8
	.cfg_ddr_training_delay_ps[1].read_dq_bit_delay =
	{
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},                                                     //total 72
	.cfg_ddr_training_delay_ps[1].soc_bit_vref =
	{
		0,  40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40
	},                                                     //total 44
	.cfg_ddr_training_delay_ps[1].dram_bit_vref =
	{
		0,  32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[1].reserve_training_parameter = { 0 },
},
#endif
{
	//timing_config,4layer 2pcs ddr4 rank0, ap229.
	.cfg_board_common_setting.timming_magic					= 0,
	.cfg_board_common_setting.timming_max_valid_configs			= sizeof(__ddr_setting[1]) / sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_version			= 0,
	.cfg_board_common_setting.timming_struct_org_size			= sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_real_size			= 0,                                       //0
	.cfg_board_common_setting.fast_boot					= { 0,					   0,(1 << 3) | (4) },
	.cfg_board_common_setting.ddr_func					= 0,
	.cfg_board_common_setting.board_id					= CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.DramType					= CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.dram_rank_config				= CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.DisabledDbyte					= CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.dram_cs0_base_add				= 0,
	.cfg_board_common_setting.dram_cs1_base_add				= 0,
	.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_2048MB,
	.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.dram_x4x8x16_mode				= CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.Is2Ttiming					= CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.log_level					= LOG_LEVEL_BASIC,
	.cfg_board_common_setting.ddr_rdbi_wr_enable				= DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.pll_ssc_mode					= DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.org_tdqs2dq					= 0,
	.cfg_board_common_setting.reserve1_test_function			= { 0 },
	.cfg_board_common_setting.ddr_dmc_remap					= DDR_DMC_REMAP_DDR4_32BIT,
	//af419 ac pinmux
	#if 0
	.cfg_board_common_setting.ac_pinmux					=
	{
		0,  0, 0,  1,  2,  3,  27, 12,
		21, 9, 8,  0,  14, 10, 6,  7,
		20, 5, 22, 13, 15, 2,  0,  0,
		0,  0, 11, 26, 4,  0,  0,  25,
		3,  1,
	},
	#endif
	//s4 ddr4 ac pinmux
	.cfg_board_common_setting.ac_pinmux					=
	{
		0,	0,	0,	1,	2,	3,
		27,	10,	25,	5,	28,	11,	24,	0,	26,	7,	4,	21,	2,	20,	8,	13,	0,	0,	0,	0,	6,	12,	3,	0,	0,	9,	1,	23,	0
	},
	.cfg_board_common_setting.ddr_dqs_swap					= 0,
	.cfg_board_common_setting.ddr_dq_remap					=
	{
		0,  1,	2,  3,	4,  5,	6,  7,
		8,  9,	10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35
	},                                                           //d0-d31 dm0 dm1 dm2 dm3
	.cfg_board_common_setting.ddr_vddee_setting				= { 0 },
	.cfg_board_SI_setting_ps[0].DRAMFreq = 1320,
	.cfg_board_SI_setting_ps[0].PllBypassEn = 0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl = 0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
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
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org =
	{
		128, 128, 128 - 40, 128, 128, 128, 128, 128,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384,
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[0].ac_trace_delay =
	{
		128, 128, 128 - 40, 128, 128, 128, 128, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384,
	},

	#if 1
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=0x00000131,// 305
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=0x00000127,// 295
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=0x00000136,// 310
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=0x00000127,// 295
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=0x0000017a,// 378
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=0x00000164,// 356
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=0x0000017d,// 381
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=0x00000174,// 372
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=0x00000176,// 374
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=0x00000160,// 352
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=0x0000017c,// 380
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=0x0000017a,// 378
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=0x0000016e,// 366
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=0x00000175,// 373
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=0x00000143,// 323
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=0x0000015c,// 348
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=0x0000015e,// 350
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=0x0000017b,// 379
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=0x0000015c,// 348
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=0x00000172,// 370
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=0x0000015c,// 348
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=0x0000015e,// 350
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=0x00000178,// 376
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=0x0000014f,// 335
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=0x00000165,// 357
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=0x00000160,// 352
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=0x00000175,// 373
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=0x00000168,// 360
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=0x0000015e,// 350
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=0x00000173,// 371
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=0x00000164,// 356
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=0x00000178,// 376
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=0x0000015c,// 348
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=0x0000015f,// 351
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=0x00000153,// 339
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=0x00000170,// 368
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=0x0000015a,// 346
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=0x0000015d,// 349
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=0x00000153,// 339
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=0x00000164,// 356
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[36]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[37]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[38]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[39]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[40]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[41]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[42]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[43]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[44]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[45]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[46]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[47]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[48]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[49]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[50]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[51]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[52]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[53]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[54]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[55]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[56]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[57]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[58]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[59]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[60]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[61]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[62]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[63]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[64]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[65]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[66]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[67]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[68]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[69]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[70]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[71]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=0x00000421,// 1057
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=0x00000420,// 1056
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=0x0000041a,// 1050
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=0x0000041a,// 1050
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=0x00000085,// 133
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=0x00000093,// 147
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=0x00000099,// 153
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=0x0000008a,// 138
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=0x00000054,// 84
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=0x0000003c,// 60
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=0x0000004f,// 79
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=0x00000030,// 48
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=0x00000057,// 87
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=0x00000066,// 102
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=0x00000052,// 82
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=0x00000042,// 66
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=0x00000067,// 103
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=0x0000005f,// 95
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=0x00000051,// 81
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=0x00000057,// 87
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=0x0000003c,// 60
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=0x00000052,// 82
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=0x0000006c,// 108
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=0x00000060,// 96
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=0x0000004f,// 79
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=0x0000006a,// 106
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=0x00000060,// 96
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=0x0000003c,// 60
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=0x00000069,// 105
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=0x00000033,// 51
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=0x00000033,// 51
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[36]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[37]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[38]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[39]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[40]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[41]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[42]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[43]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[44]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[45]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[46]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[47]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[48]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[49]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[50]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[51]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[52]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[53]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[54]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[55]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[56]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[57]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[58]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[59]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[60]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[61]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[62]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[63]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[64]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[65]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[66]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[67]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[68]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[69]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[70]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[71]=0x00000000,// 0
	#endif
	                                                //total 72
	.cfg_ddr_training_delay_ps[0].soc_bit_vref =
	{
		0,  40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 48, 40, 48,
		40, 48, 40, 48
	},                                                     //total 44
	.cfg_ddr_training_delay_ps[0].dram_bit_vref =
	{
		0,  32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[0].reserve_training_parameter = { 0 },

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
	.cfg_board_SI_setting_ps[1].ac_trace_delay_org =
	{
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128,
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[1].ac_trace_delay =
	{
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128, 128, 128, 128, 128,
		128, 128, 128, 128,
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[1].write_dqs_delay =
	{
		0, 0, 0, 0, 0, 0, 0, 0
	},
	.cfg_ddr_training_delay_ps[1].write_dq_bit_delay =
	{
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
	.cfg_ddr_training_delay_ps[1].read_dqs_gate_delay =
	{
		192, 192, 192, 192, 192, 192, 192, 192
	},                                                     //total 8
	.cfg_ddr_training_delay_ps[1].read_dqs_delay =
	{
		64, 64, 64, 64, 64, 64, 64, 64
	},                                                                  //total 8
	.cfg_ddr_training_delay_ps[1].read_dq_bit_delay =
	{
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},                                                     //total 72
	.cfg_ddr_training_delay_ps[1].soc_bit_vref =
	{
		0,  40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40
	},                                                     //total 44
	.cfg_ddr_training_delay_ps[1].dram_bit_vref =
	{
		0,  32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[1].reserve_training_parameter = { 0 },
},
};

board_clk_set_t __board_clk_setting
__attribute__ ((section(".clk_param"))) = {
	/* clock settings for bl2 */
	.cpu_clk				= CPU_CLK / 24 * 24,
#ifdef CONFIG_PXP_DDR
	.pxp					= 1,
#else
	.pxp					= 0,
#endif
	.low_console_baud = CONFIG_LOW_CONSOLE_BAUD,
};


#define VCCK_VAL				AML_VCCK_INIT_VOLTAGE
#define VDDEE_VAL				AML_VDDEE_INIT_VOLTAGE
/* VCCK PWM table */
#if   (VCCK_VAL == 1039)
	#define VCCK_VAL_REG	0x00000022
#elif (VCCK_VAL == 1029)
	#define VCCK_VAL_REG	0x00010021
#elif (VCCK_VAL == 1019)
	#define VCCK_VAL_REG	0x00020020
#elif (VCCK_VAL == 1009)
	#define VCCK_VAL_REG	0x0003001f
#elif (VCCK_VAL == 999)
	#define VCCK_VAL_REG	0x0004001e
#elif (VCCK_VAL == 989)
	#define VCCK_VAL_REG	0x0005001d
#elif (VCCK_VAL == 979)
	#define VCCK_VAL_REG	0x0006001c
#elif (VCCK_VAL == 969)
	#define VCCK_VAL_REG	0x0007001b
#elif (VCCK_VAL == 959)
	#define VCCK_VAL_REG	0x0008001a
#elif (VCCK_VAL == 949)
	#define VCCK_VAL_REG	0x00090019
#elif (VCCK_VAL == 939)
	#define VCCK_VAL_REG	0x000a0018
#elif (VCCK_VAL == 929)
	#define VCCK_VAL_REG	0x000b0017
#elif (VCCK_VAL == 919)
	#define VCCK_VAL_REG	0x000c0016
#elif (VCCK_VAL == 909)
	#define VCCK_VAL_REG	0x000d0015
#elif (VCCK_VAL == 899)
	#define VCCK_VAL_REG	0x000e0014
#elif (VCCK_VAL == 889)
	#define VCCK_VAL_REG	0x000f0013
#elif (VCCK_VAL == 879)
	#define VCCK_VAL_REG	0x00100012
#elif (VCCK_VAL == 869)
	#define VCCK_VAL_REG	0x00110011
#elif (VCCK_VAL == 859)
	#define VCCK_VAL_REG	0x00120010
#elif (VCCK_VAL == 849)
	#define VCCK_VAL_REG	0x0013000f
#elif (VCCK_VAL == 839)
	#define VCCK_VAL_REG	0x0014000e
#elif (VCCK_VAL == 829)
	#define VCCK_VAL_REG	0x0015000d
#elif (VCCK_VAL == 819)
	#define VCCK_VAL_REG	0x0016000c
#elif (VCCK_VAL == 809)
	#define VCCK_VAL_REG	0x0017000b
#elif (VCCK_VAL == 799)
	#define VCCK_VAL_REG	0x0018000a
#elif (VCCK_VAL == 789)
	#define VCCK_VAL_REG	0x00190009
#elif (VCCK_VAL == 779)
	#define VCCK_VAL_REG	0x001a0008
#elif (VCCK_VAL == 769)
	#define VCCK_VAL_REG	0x001b0007
#elif (VCCK_VAL == 759)
	#define VCCK_VAL_REG	0x001c0006
#elif (VCCK_VAL == 749)
	#define VCCK_VAL_REG	0x001d0005
#elif (VCCK_VAL == 739)
	#define VCCK_VAL_REG	0x001e0004
#elif (VCCK_VAL == 729)
	#define VCCK_VAL_REG	0x001f0003
#elif (VCCK_VAL == 719)
	#define VCCK_VAL_REG	0x00200002
#elif (VCCK_VAL == 709)
	#define VCCK_VAL_REG	0x00210001
#elif (VCCK_VAL == 699)
	#define VCCK_VAL_REG	0x00220000
#else
	#error "VCCK val out of range\n"
#endif

/* VDDEE_VAL_REG */
#if    (VDDEE_VAL == 700)
	#define VDDEE_VAL_REG	0x120000
#elif (VDDEE_VAL == 710)
	#define VDDEE_VAL_REG	0x110001
#elif (VDDEE_VAL == 720)
	#define VDDEE_VAL_REG	0x100002
#elif (VDDEE_VAL == 730)
	#define VDDEE_VAL_REG	0xf0003
#elif (VDDEE_VAL == 740)
	#define VDDEE_VAL_REG	0xe0004
#elif (VDDEE_VAL == 750)
	#define VDDEE_VAL_REG	0xd0005
#elif (VDDEE_VAL == 760)
	#define VDDEE_VAL_REG	0xc0006
#elif (VDDEE_VAL == 770)
	#define VDDEE_VAL_REG	0xb0007
#elif (VDDEE_VAL == 780)
	#define VDDEE_VAL_REG	0xa0008
#elif (VDDEE_VAL == 790)
	#define VDDEE_VAL_REG	0x90009
#elif (VDDEE_VAL == 800)
	#define VDDEE_VAL_REG	0x8000a
#elif (VDDEE_VAL == 810)
	#define VDDEE_VAL_REG	0x7000b
#elif (VDDEE_VAL == 820)
	#define VDDEE_VAL_REG	0x6000c
#elif (VDDEE_VAL == 830)
	#define VDDEE_VAL_REG	0x5000d
#elif (VDDEE_VAL == 840)
	#define VDDEE_VAL_REG	0x4000e
#elif (VDDEE_VAL == 850)
	#define VDDEE_VAL_REG	0x3000f
#elif (VDDEE_VAL == 860)
	#define VDDEE_VAL_REG	0x20010
#elif (VDDEE_VAL == 870)
	#define VDDEE_VAL_REG	0x10011
#elif (VDDEE_VAL == 880)
	#define VDDEE_VAL_REG	0x12
#else
	#error "VDDEE val out of range\n"
#endif

bl2_reg_t __bl2_reg[] __attribute__ ((section(".generic_param"))) = {
	//hxbao, need fine tune
	{0,			0,            		0xffffffff,   0, 0, 0},
};

/* gpio/pinmux/pwm init */
register_ops_t __bl2_ops_reg[MAX_REG_OPS_ENTRIES]
__attribute__ ((section(".misc_param"))) = {

	/* config vddee and vcck pwm - pwm_h and pwm_j*/
	{PWMGH_PWM_B,		VDDEE_VAL_REG,  	0xffffffff,	0, 0, 0},
	{PWMIJ_PWM_B,		VCCK_VAL_REG,  		0xffffffff,	0, 0, 0},
	{PWMGH_MISC_REG_AB,	(0x1 << 1), 		(0x1 << 1), 0, 0, 0},
	{PWMIJ_MISC_REG_AB,	(0x1 << 1), 		(0x1 << 1), 0, 0, 0},
	/* set pwm h and pwm j clock rate to 24M, enable them */
	{CLKCTRL_PWM_CLK_GH_CTRL,	(1 << 24), 	0xffffffff, 	0, 0, 0},
	{CLKCTRL_PWM_CLK_IJ_CTRL,	(1 << 24) , 	0xffffffff, 	0, 0, 0},
	/* set GPIOE_0 GPIOE_1 drive strength to 3 */
	{PADCTRL_GPIOE_DS,	0xf, 	0xf,		0, 0, 0},
	/* set GPIOE_0 GPIOE_1 mux to pwmh pwmj */
	{PADCTRL_PIN_MUX_REGI,	(0x3 << 0),		(0xf << 0),	0, 0, 0},
	{PADCTRL_PIN_MUX_REGI,	(0x3 << 4),		(0xf << 4),	0, 0, 0},
};

/* for all the storage parameter */
storage_parameter_t __store_para __attribute__ ((section(".store_param"))) = {
	.common				= {
		.version = 0x01,
		.device_fip_container_size = 0x380000,
		.device_fip_container_copies = 4,
	},
	.nand				= {
		.version = 0x01,
		.bbt_pages = 0x1,
		.bbt_start_block = 20,
		.discrete_mode = 1,
		.setup_data.nand_setup_data = (2 << 20) |		\
					(0 << 19) |                     \
					(1 << 17) |                     \
					(1 << 14) |                     \
					(0 << 13) |                     \
					(64 << 6) |                     \
					(4 << 0),
		.reserved_area_blk_cnt = 48,
		.page_per_block = 64,
		.use_param_page_list = 0,
	},
};
