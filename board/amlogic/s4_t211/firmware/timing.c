// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <asm/arch/secure_apb.h>
#include <asm/arch/timing.h>
#include <asm/arch/ddr_define.h>

/* board clk defines */
#define CPU_CLK					1512

ddr_set_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {
#if 1  //timing_config,T211
{
	//T211
	.cfg_board_common_setting.timming_magic					= 0,
	.cfg_board_common_setting.timming_max_valid_configs			= sizeof(__ddr_setting[0]) / sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_version			= 0,
	.cfg_board_common_setting.timming_struct_org_size			= sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_real_size			= 0,                                       //0
	.cfg_board_common_setting.fast_boot					= { 1,0,0,0xc6, },
	.cfg_board_common_setting.ddr_func					= 0,
	.cfg_board_common_setting.board_id					= CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.DramType					= CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.dram_rank_config				= CONFIG_DDR0_32BIT_16BIT_RANK0_CH0,
	.cfg_board_common_setting.DisabledDbyte					= CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.dram_cs0_base_add				= 0,
	.cfg_board_common_setting.dram_cs1_base_add				= 0,
	.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1536MB,
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
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_34_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_34_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_80_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_48_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting = DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].dfe_offset = 0,
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
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0] = 0x0000080+50+80,                                                                                                  // 340
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1] = 0x0000080+50+80,                                                                                                  // 335
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2] = 0x0000080+50+80,                                                                                                  // 315
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3] = 0x0000080+50+80,                                                                                                  // 345
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4] = 0x0000080+50+80,                                                                                                  // 256
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5] = 0x0000080+50+80,                                                                                                  // 256
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6] = 0x0000080+50+80,                                                                                                  // 256
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7] = 0x0000080+50+80,                                                                                                  // 256
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0] = 0x00000c0+50+80,                                                                                               // 414
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1] = 0x00000c0+50+80,                                                                                               // 393
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2] = 0x00000c0+50+80,                                                                                               // 387
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3] = 0x00000c0+50+80,                                                                                               // 422
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4] = 0x00000c0+50+80,                                                                                               // 427
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5] = 0x00000c0+50+80,                                                                                               // 402
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6] = 0x00000c0+50+80,                                                                                               // 397
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7] = 0x00000c0+50+80,                                                                                               // 402
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8] = 0x00000c0+50+80,                                                                                               // 406
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9] = 0x00000c0+50+80,                                                                                               // 384
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10] = 0x00000c0+50+80,                                                                                              // 384
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11] = 0x00000c0+50+80,                                                                                              // 371
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12] = 0x00000c0+50+80,                                                                                              // 380
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13] = 0x00000c0+50+80,                                                                                              // 398
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14] = 0x00000c0+50+80,                                                                                              // 390
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15] = 0x00000c0+50+80,                                                                                              // 377
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16] = 0x00000c0+50+80,                                                                                              // 388
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17] = 0x00000c0+50+80,                                                                                              // 384
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18] = 0x00000c0+50+80,                                                                                              // 381
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19] = 0x00000c0+50+80,                                                                                              // 365
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20] = 0x00000c0+50+80,                                                                                              // 378
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21] = 0x00000c0+50+80,                                                                                              // 396
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22] = 0x00000c0+50+80,                                                                                              // 382
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23] = 0x00000c0+50+80,                                                                                              // 399
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24] = 0x00000c0+50+80,                                                                                              // 387
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25] = 0x00000c0+50+80,                                                                                              // 397
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26] = 0x00000c0+50+80,                                                                                              // 382
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27] = 0x00000c0+50+80,                                                                                              // 380
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28] = 0x00000c0+50+80,                                                                                              // 407
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29] = 0x00000c0+50+80,                                                                                              // 398
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30] = 0x00000c0+50+80,                                                                                              // 409
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31] = 0x00000c0+50+80,                                                                                              // 385
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32] = 0x00000c0+50+80,                                                                                              // 379
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33] = 0x00000c0+50+80,                                                                                              // 399
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34] = 0x00000c0+50+80,                                                                                              // 380
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35] = 0x00000c0+50+80,                                                                                              // 393
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[36] = 0x0000000,                                                                                          // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[37] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[38] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[39] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[40] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[41] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[42] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[43] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[44] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[45] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[46] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[47] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[48] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[49] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[50] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[51] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[52] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[53] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[54] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[55] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[56] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[57] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[58] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[59] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[60] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[61] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[62] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[63] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[64] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[65] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[66] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[67] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[68] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[69] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[70] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[71] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay =
	{
		128*7, 128*7, 128*7, 128*7, 128*7, 128*7, 128*7, 128*7
	},                                                     //total 8
	.cfg_ddr_training_delay_ps[0].read_dqs_delay =
	{
		64, 64, 80, 64, 64, 64, 64, 64
	},                                                                  //total 8
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay =
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
	},
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
	.cfg_ddr_training_delay_ps[0].reserve_training_parameter = { 0xa+4, 0x5+5, 0xa+4, 0x7+8,0,0,0,0,0x4,0x4,0x82,0x3 },

	.cfg_board_SI_setting_ps[0].vref_soc_data_permil =0x0,// 0
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil=0x000002bc,// 700
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[0]=0x00000080,// 128
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[1]=0x00000080,// 128
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[2]=0x00000058,// 88
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[3]=0x00000080,// 128
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[4]=0x00000080,// 128
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[5]=0x00000080,// 128
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[6]=0x00000080,// 128
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[7]=0x00000080,// 128
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[8]=0x00000180,// 384
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[9]=0x00000180,// 384
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[10]=0x00000100,// 256
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[11]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[12]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[13]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[14]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[15]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[16]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[17]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[18]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[19]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[20]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[21]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[22]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[23]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[24]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[25]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[26]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[27]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[28]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[29]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[30]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[31]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[32]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[33]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[34]=0x00000000,// 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org[35]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[0]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[1]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[2]=0x00000058,// 88
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[3]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[4]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[5]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[6]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[7]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[8]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[9]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[10]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[11]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[12]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[13]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[14]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[15]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[16]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[17]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[18]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[19]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[20]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[21]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[22]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[23]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[24]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[25]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[26]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[27]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[28]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[29]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[30]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[31]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[32]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[33]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[34]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[35]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=0x000000b0,// 176
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=0x000000c4,// 196
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=0x000000c7,// 199
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=0x000000a7,// 167
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=0x000000db,// 219
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=0x000000e7,// 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=0x000000e3,// 227
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=0x000000c9,// 201
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=0x000000e6,// 230
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=0x0000010b,// 267
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=0x000000e8,// 232
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=0x000000f7,// 247
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=0x000000f4,// 244
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=0x00000104,// 260
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=0x000000fc,// 252
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=0x000000fa,// 250
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=0x000000e0,// 224
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=0x000000e4,// 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=0x000000f0,// 240
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=0x000000ce,// 206
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=0x000000de,// 222
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=0x000000ef,// 239
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=0x000000d8,// 216
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=0x000000df,// 223
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=0x000000d9,// 217
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=0x000000e0,// 224
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
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=0x00000356,// 854
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=0x00000357,// 855
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=0x0000033e,// 830
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=0x00000348,// 840
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=0x00000086,// 134
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=0x0000007f,// 127
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=0x0000006f,// 111
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=0x00000066,// 102
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=0x00000058,// 88
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=0x00000034,// 52
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=0x00000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=0x00000057,// 87
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=0x00000061,// 97
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=0x0000005c,// 92
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=0x0000004a,// 74
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
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[0]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[1]=0x00000026,// 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[2]=0x00000029,// 41
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[3]=0x00000025,// 37
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[4]=0x00000024,// 36
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[5]=0x0000002b,// 43
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[6]=0x00000027,// 39
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[7]=0x00000027,// 39
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[8]=0x00000026,// 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[9]=0x00000027,// 39
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[10]=0x00000022,// 34
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[11]=0x00000023,// 35
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[12]=0x00000023,// 35
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[13]=0x00000023,// 35
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[14]=0x00000023,// 35
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[15]=0x00000024,// 36
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[16]=0x00000026,// 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[17]=0x00000026,// 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[18]=0x00000027,// 39
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[19]=0x00000028,// 40
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[20]=0x00000025,// 37
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[21]=0x00000024,// 36
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[22]=0x00000024,// 36
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[23]=0x00000025,// 37
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[24]=0x0000002a,// 42
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[25]=0x00000026,// 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[26]=0x00000026,// 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[27]=0x00000023,// 35
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[28]=0x00000026,// 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[29]=0x00000024,// 36
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[30]=0x00000028,// 40
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[31]=0x00000027,// 39
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[32]=0x00000026,// 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[33]=0x00000026,// 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[34]=0x00000025,// 37
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[35]=0x00000026,// 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[36]=0x00000026,// 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[37]=0x00000030,// 48
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[38]=0x00000026,// 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[39]=0x0000002f,// 47
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[40]=0x00000026,// 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[41]=0x0000002f,// 47
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[42]=0x00000026,// 38
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[43]=0x0000002b,// 43
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
	.cfg_board_SI_setting_ps[1].dfe_offset = 0,
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
#endif  //end T211
#if 1  //timing_config,
{
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
	.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,
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
	.cfg_board_SI_setting_ps[0].DRAMFreq = 792,
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
	.cfg_board_SI_setting_ps[0].dfe_offset = 0,
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
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0] = 0x0000080+50+80,                                                                                                  // 340
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1] = 0x0000080+50+80,                                                                                                  // 335
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2] = 0x0000080+50+80,                                                                                                  // 315
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3] = 0x0000080+50+80,                                                                                                  // 345
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4] = 0x0000080+50+80,                                                                                                  // 256
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5] = 0x0000080+50+80,                                                                                                  // 256
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6] = 0x0000080+50+80,                                                                                                  // 256
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7] = 0x0000080+50+80,                                                                                                  // 256
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0] = 0x00000c0+50+80,                                                                                               // 414
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1] = 0x00000c0+50+80,                                                                                               // 393
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2] = 0x00000c0+50+80,                                                                                               // 387
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3] = 0x00000c0+50+80,                                                                                               // 422
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4] = 0x00000c0+50+80,                                                                                               // 427
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5] = 0x00000c0+50+80,                                                                                               // 402
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6] = 0x00000c0+50+80,                                                                                               // 397
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7] = 0x00000c0+50+80,                                                                                               // 402
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8] = 0x00000c0+50+80,                                                                                               // 406
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9] = 0x00000c0+50+80,                                                                                               // 384
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10] = 0x00000c0+50+80,                                                                                              // 384
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11] = 0x00000c0+50+80,                                                                                              // 371
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12] = 0x00000c0+50+80,                                                                                              // 380
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13] = 0x00000c0+50+80,                                                                                              // 398
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14] = 0x00000c0+50+80,                                                                                              // 390
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15] = 0x00000c0+50+80,                                                                                              // 377
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16] = 0x00000c0+50+80,                                                                                              // 388
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17] = 0x00000c0+50+80,                                                                                              // 384
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18] = 0x00000c0+50+80,                                                                                              // 381
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19] = 0x00000c0+50+80,                                                                                              // 365
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20] = 0x00000c0+50+80,                                                                                              // 378
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21] = 0x00000c0+50+80,                                                                                              // 396
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22] = 0x00000c0+50+80,                                                                                              // 382
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23] = 0x00000c0+50+80,                                                                                              // 399
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24] = 0x00000c0+50+80,                                                                                              // 387
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25] = 0x00000c0+50+80,                                                                                              // 397
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26] = 0x00000c0+50+80,                                                                                              // 382
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27] = 0x00000c0+50+80,                                                                                              // 380
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28] = 0x00000c0+50+80,                                                                                              // 407
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29] = 0x00000c0+50+80,                                                                                              // 398
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30] = 0x00000c0+50+80,                                                                                              // 409
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31] = 0x00000c0+50+80,                                                                                              // 385
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32] = 0x00000c0+50+80,                                                                                              // 379
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33] = 0x00000c0+50+80,                                                                                              // 399
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34] = 0x00000c0+50+80,                                                                                              // 380
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35] = 0x00000c0+50+80,                                                                                              // 393
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[36] = 0x0000000,                                                                                          // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[37] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[38] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[39] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[40] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[41] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[42] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[43] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[44] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[45] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[46] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[47] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[48] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[49] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[50] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[51] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[52] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[53] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[54] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[55] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[56] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[57] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[58] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[59] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[60] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[61] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[62] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[63] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[64] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[65] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[66] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[67] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[68] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[69] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[70] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[71] = 0x00000000,                                                                                              // 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay =
	{
		128*7, 128*7, 128*7, 128*7, 128*7, 128*7, 128*7, 128*7
	},                                                     //total 8
	.cfg_ddr_training_delay_ps[0].read_dqs_delay =
	{
		64, 64, 80, 64, 64, 64, 64, 64
	},                                                                  //total 8
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay =
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
	},
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
		32,  32, 32, 32, 32, 32, 32, 32,
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
	.cfg_board_SI_setting_ps[1].dfe_offset = 0,
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
#endif //end
#if 0  //timing_config,T212_DONGLE 4layer LPDDR4 rank01
{
	//T212_DONGLE 4layer LPDDR4 rank01
	.cfg_board_common_setting.timming_magic					= 0,
	.cfg_board_common_setting.timming_max_valid_configs			= sizeof(__ddr_setting[0]) / sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_version			= 0,
	.cfg_board_common_setting.timming_struct_org_size			= sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_real_size			= 0,                                       //0
	.cfg_board_common_setting.fast_boot					= { 0 },
	.cfg_board_common_setting.ddr_func					= DDR_FUNC_CONFIG_DFE_FUNCTION,
	.cfg_board_common_setting.board_id					= CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.DramType					= CONFIG_DDR_TYPE_LPDDR4,
	.cfg_board_common_setting.dram_rank_config				= CONFIG_DDR0_32BIT_RANK01_CH0,
	.cfg_board_common_setting.DisabledDbyte					= CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.dram_cs0_base_add				= 0,
	.cfg_board_common_setting.dram_cs1_base_add				= 0,
	.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,//CONFIG_DDR0_SIZE_AUTO_SIZE,CONFIG_DDR0_SIZE_1024MB,
	.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_1024MB,
	.cfg_board_common_setting.dram_x4x8x16_mode				= CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.Is2Ttiming					= CONFIG_USE_DDR_1T_MODE,
	.cfg_board_common_setting.log_level					= LOG_LEVEL_BASIC,
	.cfg_board_common_setting.ddr_rdbi_wr_enable				= DDR_WRITE_READ_DBI_DISABLE,//DDR_WRITE_READ_DBI_DISABLE,DDR_READ_DBI_ENABLE
	.cfg_board_common_setting.pll_ssc_mode					= DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.org_tdqs2dq					= 0,
	.cfg_board_common_setting.reserve1_test_function			= { 0 },
	.cfg_board_common_setting.ddr_dmc_remap					= DDR_DMC_REMAP_LPDDR4_32BIT,//DDR_DMC_REMAP_LPDDR4_32BIT,//DDR_DMC_REMAP_DDR3_32BIT

	//s4 lpddr4 ac pinmux
	.cfg_board_common_setting.ac_pinmux					=
	{
		0,	0,	0,	1,	0,	1,	2,	0,
		5,	1,	0,	0,	0,	1,	0,	2,
		0,	3,	0,	3,	5,	0,	0,	0,
		0,	0,	4,	4,	0,	0,	0,	0,
		0,	0,	0
	},
	.cfg_board_common_setting.ddr_dqs_swap					= 0,
	.cfg_board_common_setting.ddr_dq_remap					=
	{
		8,	12,	13,	11,	14,	9,	10,	15,
		0,	2,	5,	6,	3,	7,	1,	4,
		27,	31,	28,	24,	25,	29,	26,	30,
		21,	18,	22,	19,	16,	17,	23,	20,
		33,	32,	35,	34
	},                                                           //d0-d31 dm0 dm1 dm2 dm3
	.cfg_board_common_setting.ddr_vddee_setting				= { 0 },
	//DDR frequercy 1
	.cfg_board_SI_setting_ps[0].DRAMFreq = 1320,
	.cfg_board_SI_setting_ps[0].PllBypassEn = 0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl = 0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_80_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_LPDDR4_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_LPDDR4_ODT_80_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm = DDR_DRAM_LPDDR4_AC_ODT_120_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting = DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].dfe_offset = 0,
	.cfg_board_SI_setting_ps[0].vref_ac_permil = 300,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil = 330,          //330
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency = 0,
	.cfg_board_SI_setting_ps[0].training_phase_parameter = { 0 },
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org =
	{
		64 + 128,	64 + 128,	128,	128,	64 + 128,      64 + 128,      0 + 128,	     0 + 128,      //0,0,0,0,0,0,0,0,
		128 + 20 + 128, 128 + 20 + 128, 128 + 60 + 128 - 44, 128 + 20 + 128 - 36, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128,
		128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128,
		128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128,
		128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[0].ac_trace_delay =
	{
		64 + 128,	64 + 128,	128,	128,	64 + 128,      64 + 128,      0 + 128,	     128 + 128,   //0,0,0,0,0,0,0,0,
		128 + 20 + 128, 128 + 20 + 128, 128 + 60 + 128 - 44, 128 + 20 + 128 - 36, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128,
		128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128,
		128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128,
		128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,
	},

#if 1
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=0x0000006e,// 110
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=0x0000008c,// 140
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=0x0000006e,// 110
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=0x0000008c,// 140
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x0000006e,// 110
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000082,// 130
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x0000006e,// 110
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x0000008c,// 140
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=0x00000153,// 339
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=0x00000144,// 324
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=0x0000014c,// 332
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=0x0000014d,// 333
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=0x0000014b,// 331
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=0x00000135,// 309
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=0x0000014c,// 332
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=0x00000152,// 338
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=0x00000142,// 322
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=0x0000016e,// 366
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=0x00000145,// 325
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=0x00000157,// 343
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=0x00000159,// 345
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=0x00000169,// 361
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=0x00000158,// 344
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=0x00000162,// 354
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=0x00000158,// 344
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=0x0000015c,// 348
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=0x0000015d,// 349
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=0x00000142,// 322
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=0x00000147,// 327
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=0x0000013c,// 316
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=0x0000014f,// 335
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=0x0000013f,// 319
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=0x0000014a,// 330
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=0x0000014b,// 331
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=0x0000014d,// 333
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=0x0000016e,// 366
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=0x0000014e,// 334
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=0x00000164,// 356
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=0x0000015e,// 350
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=0x0000016b,// 363
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=0x00000159,// 345
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=0x00000163,// 355
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=0x00000159,// 345
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=0x0000015e,// 350
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[36]=0x00000155,// 341
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[37]=0x00000145,// 325
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[38]=0x0000014e,// 334
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[39]=0x0000014f,// 335
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[40]=0x0000014e,// 334
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[41]=0x00000138,// 312
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[42]=0x0000014d,// 333
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[43]=0x00000154,// 340
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[44]=0x00000145,// 325
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[45]=0x00000166,// 358
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[46]=0x0000013e,// 318
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[47]=0x00000151,// 337
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[48]=0x00000151,// 337
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[49]=0x00000161,// 353
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[50]=0x00000152,// 338
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[51]=0x0000015c,// 348
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[52]=0x00000150,// 336
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[53]=0x00000153,// 339
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[54]=0x00000160,// 352
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[55]=0x00000144,// 324
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[56]=0x0000014c,// 332
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[57]=0x00000140,// 320
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[58]=0x00000152,// 338
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[59]=0x00000142,// 322
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[60]=0x0000014d,// 333
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[61]=0x0000014e,// 334
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[62]=0x00000150,// 336
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[63]=0x00000172,// 370
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[64]=0x00000153,// 339
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[65]=0x00000168,// 360
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[66]=0x00000162,// 354
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[67]=0x0000016e,// 366
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[68]=0x0000015c,// 348
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[69]=0x00000166,// 358
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[70]=0x0000015c,// 348
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[71]=0x00000163,// 355

	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=0x000003a5,// 933
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=0x00000399,// 921
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=0x000003aa,// 938
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=0x0000038e,// 910
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x0000034e,// 846
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000444,// 1092
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000354,// 852
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000337,// 823
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=0x0000007f,// 127
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=0x00000085,// 133
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=0x00000079,// 121
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=0x00000086,// 134
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000081,// 129
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000089,// 137
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000085,// 133
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000092,// 146
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=0x00000037,// 55
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=0x0000003c,// 60
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=0x00000040,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=0x00000052,// 82
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=0x00000031,// 49
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=0x00000053,// 83
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=0x00000053,// 83
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=0x0000003b,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=0x00000043,// 67
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=0x0000005d,// 93
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=0x00000040,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=0x0000005e,// 94
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=0x00000054,// 84
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=0x00000042,// 66
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=0x00000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[36]=0x0000004a,// 74
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[37]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[38]=0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[39]=0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[40]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[41]=0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[42]=0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[43]=0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[44]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[45]=0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[46]=0x00000032,// 50
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[47]=0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[48]=0x0000003d,// 61
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[49]=0x00000057,// 87
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[50]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[51]=0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[52]=0x00000040,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[53]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[54]=0x00000062,// 98
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[55]=0x0000003d,// 61
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[56]=0x0000004f,// 79
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[57]=0x0000003c,// 60
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[58]=0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[59]=0x00000042,// 66
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[60]=0x00000050,// 80
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[61]=0x00000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[62]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[63]=0x00000067,// 103
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[64]=0x0000003d,// 61
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[65]=0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[66]=0x0000004a,// 74
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[67]=0x00000066,// 102
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[68]=0x00000042,// 66
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[69]=0x0000005e,// 94
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[70]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[71]=0x00000052,// 82
#endif
#if 1
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[0]=24,//0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[1]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[2]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[3]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[4]=0x00000019,// 25
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[5]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[6]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[7]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[8]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[9]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[10]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[11]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[12]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[13]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[14]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[15]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[16]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[17]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[18]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[19]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[20]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[21]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[22]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[23]=0x00000019,// 25
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[24]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[25]=0x00000019,// 25
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[26]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[27]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[28]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[29]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[30]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[31]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[32]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[33]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[34]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[35]=0x00000018,// 24
#endif
#if 1
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[36]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[37]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[38]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[39]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[40]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[41]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[42]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[43]=0x00000018,// 24
#endif
#if 1
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[0] = 0x00000000,    // 0X5b,// 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[1] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[2] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[3] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[4] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[5] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[6] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[7] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[8] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[9] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[10] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[11] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[12] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[13] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[14] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[15] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[16] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[17] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[18] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[19] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[20] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[21] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[22] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[23] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[24] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[25] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[26] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[27] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[28] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[29] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[30] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[31] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[32] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[33] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[34] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[35] = 0x00000000,   // 0
#endif

	.cfg_ddr_training_delay_ps[0].reserve_training_parameter = {
		(0 << 7) | 0x08, (0 << 7) | 0x08, (0 << 7) | 0x08, (0 << 7) | 0x08,
		(0 << 7) | 0x08, (0 << 7) | 0x08, (0 << 7) | 0x08, (0 << 7) | 0x08,
		(1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10,
		(1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10,
	},
	//DDR frequercy 2
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
	.cfg_board_SI_setting_ps[1].dfe_offset = 0,
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
#endif //end T212_DONGLE 4layer LPDDR4 rank01
#if 0  //timing_config,T212_DONGLE 4layer LPDDR4 rank0
{
	//T212_DONGLE 4layer LPDDR4 rank0
	.cfg_board_common_setting.timming_magic					= 0,
	.cfg_board_common_setting.timming_max_valid_configs			= sizeof(__ddr_setting[0]) / sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_version			= 0,
	.cfg_board_common_setting.timming_struct_org_size			= sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_real_size			= 0,                                       //0
	.cfg_board_common_setting.fast_boot					= { 0 },
	.cfg_board_common_setting.ddr_func					= DDR_FUNC_CONFIG_DFE_FUNCTION,
	.cfg_board_common_setting.board_id					= CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.DramType					= CONFIG_DDR_TYPE_LPDDR4,
	.cfg_board_common_setting.dram_rank_config				= CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.DisabledDbyte					= CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.dram_cs0_base_add				= 0,
	.cfg_board_common_setting.dram_cs1_base_add				= 0,
	.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,//CONFIG_DDR0_SIZE_AUTO_SIZE,CONFIG_DDR0_SIZE_1024MB,
	.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.dram_x4x8x16_mode				= CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.Is2Ttiming					= CONFIG_USE_DDR_1T_MODE,
	.cfg_board_common_setting.log_level					= LOG_LEVEL_BASIC,
	.cfg_board_common_setting.ddr_rdbi_wr_enable				= DDR_WRITE_READ_DBI_DISABLE,//DDR_WRITE_READ_DBI_DISABLE,DDR_READ_DBI_ENABLE
	.cfg_board_common_setting.pll_ssc_mode					= DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.org_tdqs2dq					= 0,
	.cfg_board_common_setting.reserve1_test_function			= { 0 },
	.cfg_board_common_setting.ddr_dmc_remap					= DDR_DMC_REMAP_LPDDR4_32BIT,//DDR_DMC_REMAP_LPDDR4_32BIT,//DDR_DMC_REMAP_DDR3_32BIT

	//s4 lpddr4 ac pinmux
	.cfg_board_common_setting.ac_pinmux					=
	{
		0,	0,	0,	1,	0,	1,	2,	0,
		5,	1,	0,	0,	0,	1,	0,	2,
		0,	3,	0,	3,	5,	0,	0,	0,
		0,	0,	4,	4,	0,	0,	0,	0,
		0,	0,	0
	},
	.cfg_board_common_setting.ddr_dqs_swap					= 0,
	.cfg_board_common_setting.ddr_dq_remap					=
	{
		8,	12,	13,	11,	14,	9,	10,	15,
		0,	2,	5,	6,	3,	7,	1,	4,
		27,	31,	28,	24,	25,	29,	26,	30,
		21,	18,	22,	19,	16,	17,	23,	20,
		33,	32,	35,	34
	},                                                           //d0-d31 dm0 dm1 dm2 dm3
	.cfg_board_common_setting.ddr_vddee_setting				= { 0 },
	//DDR frequercy 1
	.cfg_board_SI_setting_ps[0].DRAMFreq = 1320,
	.cfg_board_SI_setting_ps[0].PllBypassEn = 0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl = 0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_80_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_LPDDR4_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_LPDDR4_ODT_80_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm = DDR_DRAM_LPDDR4_AC_ODT_120_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting = DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].dfe_offset = 0,
	.cfg_board_SI_setting_ps[0].vref_ac_permil = 300,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil = 330,          //330
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency = 0,
	.cfg_board_SI_setting_ps[0].training_phase_parameter = { 0 },
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org =
	{
		64 + 128,	64 + 128,	128,	128,	64 + 128,      64 + 128,      0 + 128,	     0 + 128,      //0,0,0,0,0,0,0,0,
		128 + 20 + 128, 128 + 20 + 128, 128 + 60 + 128 - 44, 128 + 20 + 128 - 36, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128,
		128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128,
		128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128,
		128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[0].ac_trace_delay =
	{
		64 + 128,	64 + 128,	128,	128,	64 + 128,      64 + 128,      0 + 128,	     128 + 128,   //0,0,0,0,0,0,0,0,
		128 + 20 + 128, 128 + 20 + 128, 128 + 60 + 128 - 44, 128 + 20 + 128 - 36, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128,
		128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128,
		128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128, 128 + 0 + 128,
		128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,	128 + 0 + 128,
	},

#if 1
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=0x0000006e,// 110
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=0x0000008c,// 140
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=0x0000006e,// 110
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=0x0000008c,// 140
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x0000006e,// 110
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000082,// 130
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x0000006e,// 110
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x0000008c,// 140
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=0x00000153,// 339
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=0x00000144,// 324
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=0x0000014c,// 332
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=0x0000014d,// 333
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=0x0000014b,// 331
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=0x00000135,// 309
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=0x0000014c,// 332
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=0x00000152,// 338
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=0x00000142,// 322
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=0x0000016e,// 366
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=0x00000145,// 325
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=0x00000157,// 343
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=0x00000159,// 345
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=0x00000169,// 361
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=0x00000158,// 344
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=0x00000162,// 354
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=0x00000158,// 344
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=0x0000015c,// 348
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=0x0000015d,// 349
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=0x00000142,// 322
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=0x00000147,// 327
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=0x0000013c,// 316
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=0x0000014f,// 335
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=0x0000013f,// 319
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=0x0000014a,// 330
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=0x0000014b,// 331
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=0x0000014d,// 333
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=0x0000016e,// 366
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=0x0000014e,// 334
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=0x00000164,// 356
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=0x0000015e,// 350
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=0x0000016b,// 363
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=0x00000159,// 345
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=0x00000163,// 355
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=0x00000159,// 345
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=0x0000015e,// 350
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[36]=0x00000155,// 341
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[37]=0x00000145,// 325
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[38]=0x0000014e,// 334
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[39]=0x0000014f,// 335
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[40]=0x0000014e,// 334
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[41]=0x00000138,// 312
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[42]=0x0000014d,// 333
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[43]=0x00000154,// 340
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[44]=0x00000145,// 325
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[45]=0x00000166,// 358
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[46]=0x0000013e,// 318
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[47]=0x00000151,// 337
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[48]=0x00000151,// 337
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[49]=0x00000161,// 353
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[50]=0x00000152,// 338
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[51]=0x0000015c,// 348
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[52]=0x00000150,// 336
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[53]=0x00000153,// 339
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[54]=0x00000160,// 352
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[55]=0x00000144,// 324
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[56]=0x0000014c,// 332
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[57]=0x00000140,// 320
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[58]=0x00000152,// 338
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[59]=0x00000142,// 322
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[60]=0x0000014d,// 333
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[61]=0x0000014e,// 334
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[62]=0x00000150,// 336
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[63]=0x00000172,// 370
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[64]=0x00000153,// 339
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[65]=0x00000168,// 360
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[66]=0x00000162,// 354
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[67]=0x0000016e,// 366
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[68]=0x0000015c,// 348
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[69]=0x00000166,// 358
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[70]=0x0000015c,// 348
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[71]=0x00000163,// 355

	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=0x000003a5,// 933
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=0x00000399,// 921
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=0x000003aa,// 938
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=0x0000038e,// 910
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x0000034e,// 846
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000444,// 1092
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000354,// 852
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000337,// 823
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=0x0000007f,// 127
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=0x00000085,// 133
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=0x00000079,// 121
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=0x00000086,// 134
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000081,// 129
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000089,// 137
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000085,// 133
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000092,// 146
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=0x00000037,// 55
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=0x0000003c,// 60
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=0x00000040,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=0x00000052,// 82
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=0x00000031,// 49
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=0x00000053,// 83
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=0x00000053,// 83
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=0x0000003b,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=0x00000043,// 67
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=0x0000005d,// 93
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=0x00000040,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=0x0000005e,// 94
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=0x00000054,// 84
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=0x00000042,// 66
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=0x00000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[36]=0x0000004a,// 74
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[37]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[38]=0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[39]=0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[40]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[41]=0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[42]=0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[43]=0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[44]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[45]=0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[46]=0x00000032,// 50
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[47]=0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[48]=0x0000003d,// 61
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[49]=0x00000057,// 87
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[50]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[51]=0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[52]=0x00000040,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[53]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[54]=0x00000062,// 98
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[55]=0x0000003d,// 61
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[56]=0x0000004f,// 79
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[57]=0x0000003c,// 60
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[58]=0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[59]=0x00000042,// 66
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[60]=0x00000050,// 80
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[61]=0x00000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[62]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[63]=0x00000067,// 103
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[64]=0x0000003d,// 61
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[65]=0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[66]=0x0000004a,// 74
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[67]=0x00000066,// 102
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[68]=0x00000042,// 66
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[69]=0x0000005e,// 94
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[70]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[71]=0x00000052,// 82
#endif
#if 1
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[0]=24,//0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[1]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[2]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[3]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[4]=0x00000019,// 25
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[5]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[6]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[7]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[8]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[9]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[10]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[11]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[12]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[13]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[14]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[15]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[16]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[17]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[18]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[19]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[20]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[21]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[22]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[23]=0x00000019,// 25
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[24]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[25]=0x00000019,// 25
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[26]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[27]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[28]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[29]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[30]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[31]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[32]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[33]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[34]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[35]=0x00000018,// 24
#endif
#if 1
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[36]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[37]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[38]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[39]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[40]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[41]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[42]=0x00000018,// 24
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[43]=0x00000018,// 24
#endif
#if 1
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[0] = 0x00000000,    // 0X5b,// 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[1] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[2] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[3] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[4] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[5] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[6] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[7] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[8] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[9] = 0x00000000,    // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[10] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[11] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[12] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[13] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[14] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[15] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[16] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[17] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[18] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[19] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[20] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[21] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[22] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[23] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[24] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[25] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[26] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[27] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[28] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[29] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[30] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[31] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[32] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[33] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[34] = 0x00000000,   // 0
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[35] = 0x00000000,   // 0
#endif

	.cfg_ddr_training_delay_ps[0].reserve_training_parameter = { 0 },

	//DDR frequercy 2
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
	.cfg_board_SI_setting_ps[1].dfe_offset = 0,
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
#endif //end T212_DONGLE 4layer LPDDR4 rank0
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
#if 0
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
#else
	/*enable y_led (gpioao_4)*/
	{PWMAB_PWM_B,			YELLOW_LED_VAL,	0xffffffff,	0, 0, 0},
	{PWMAB_MISC_REG_AB,		(0x1 << 1),	(0x1 << 1),	0, 0, 0},
	{CLKCTRL_PWM_CLK_AB_CTRL,	(0x1 << 24),	(0x1 << 24),	0, 0, 0},
	{PADCTRL_PIN_MUX_REGG,		(3 << 16),	(0xf << 16),	0, 0, 0},
	/* enable vddcpu dc-dc, set TEST_N to high */
	{PADCTRL_TESTN_O,	(0x1 << 0), 		(0x1 << 0),	0, 0, 0},
	{PADCTRL_TESTN_OEN,	(0x0 << 0), 		(0x1 << 0), 0, 0, 0},
	/* GPIO pull enable */
	{PADCTRL_GPIOX_PULL_EN,  (0x0 << 8),  (0xff << 8),  0, BL2_INIT_STAGE_1,  0}, /* Disable GPIOX_8 ~ GPIOX_15 */
	/* GPIO direction */
	{PADCTRL_GPIOX_OEN,  (0x0 << 6),  (0x1 << 6),  0, BL2_INIT_STAGE_1,  0}, /* GPIOX_6: Out */
	/* GPIO output */
	{PADCTRL_GPIOX_O,  (0x1 << 6),  (0x1 << 6),  0,  BL2_INIT_STAGE_1, 0},  /* GPIOX_6: High */
	/* GPIO pinmux */
	{PADCTRL_PIN_MUX_REG3,  (0x0 << 24),  (0xf << 24),  0,  BL2_INIT_STAGE_1,  0}, /* GPIOX_6 [27:24] */
#endif
};

#define DEV_FIP_SIZE 0x300000
#define DDR_FIP_SIZE 0x40000
/* for all the storage parameter */
storage_parameter_t __store_para __attribute__ ((section(".store_param"))) = {
	.common				= {
		.version = 0x01,
		.device_fip_container_size = DEV_FIP_SIZE,
		.device_fip_container_copies = 4,
		.ddr_fip_container_size = DDR_FIP_SIZE,
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
