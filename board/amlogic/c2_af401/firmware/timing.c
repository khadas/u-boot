// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <asm/arch/secure_apb.h>
#include <asm/arch/timing.h>
#include <asm/arch/ddr_define.h>

/* board clk defines */
#define CPU_CLK					1512
#define TIMMING_MAX_CONFIG		1

ddr_set_t __ddr_setting[] __attribute__ ((section(".ddr_settings"))) = {
#if 0//timing_config,af400
{
	//af400
	.cfg_board_common_setting.	board_id					=	CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.	DramType					=	CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.	dram_rank_config			=	CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.	dram_cs0_size_MB			=	CONFIG_DDR0_SIZE_AUTO_SIZE,//CONFIG_DDR0_SIZE_AUTO_SIZE,CONFIG_DDR0_SIZE_2048MB,1024MB,512MB,0MB,256,128,1536,768,3072,4096
	.cfg_board_common_setting.	dram_cs1_size_MB			=	CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.	pll_ssc_mode				=	DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.	ddr_rdbi_wr_enable			=	DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.	dram_x4x8x16_mode			=	CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.	ddr_dmc_remap				=	DDR_DMC_REMAP_DDR4_32BIT,
	.cfg_board_common_setting.	fast_boot					=	{0,0,(1<<3)|(4)},
	.cfg_board_common_setting.	fast_boot[0]				=	0x1,
	.cfg_board_common_setting.	fast_boot[3]				=	0x46,

	//DDR frequency 1
	.cfg_board_SI_setting_ps[0].DRAMFreq					=	1320,//1320,1200,1104,1008,912,792
	.cfg_board_SI_setting_ps[0].PllBypassEn					=	0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl		=	0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config				=	DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p			=	DDR_SOC_DATA_DRV_ODT_48_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n			=	DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm			=	DDR_DRAM_DDR4_DRV_48_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm			=	DDR_DRAM_DDR4_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm		=	DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm				=	DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].reserve2					=	0,
	.cfg_board_SI_setting_ps[0].vref_ac_permil 				=	0,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil 		=	0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil		=	0,
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency				=	0,
	.cfg_board_SI_setting_ps[0].training_phase_parameter				=	{0},
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org = {
	128,128,128-20,128,128,128,128,128,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	},//total 36

	#if 1
	.cfg_ddr_training_delay_ps[0].ac_trace_delay = {
	128,128,128-20,128,128,128,128,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	},//total 36
	#endif
	//write delay line setting
	#if 0
	.cfg_ddr_training_delay_ps[0].	write_dqs_delay = {128,128,128,128,128,128,128,128} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,

	.cfg_ddr_training_delay_ps[0].	write_dq_bit_delay = {
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,},
	#endif
	#if 0
	//.cfg_ddr_training_delay_ps[0].	read_dqs_gate_delay = {256,256,256,256,256,256,256,256} , //total 8
	.cfg_ddr_training_delay_ps[0].	read_dqs_gate_delay = {0,0,0,0,0,0,0,0} , //total 8 //edge mode
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64} , //total 8

	.cfg_ddr_training_delay_ps[0].	read_dqs_delay = {64,64,64,64,64,64,64,64} , //total 8
	.cfg_ddr_training_delay_ps[0].	read_dq_bit_delay = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0} , //total 72
	#endif

	#if 1
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=0x0000005f,// 95
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=0x00000073,// 115
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=0x0000005f,// 95
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=0x00000087,// 135
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=0x0000009f,// 159
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=0x00000095,// 149
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=0x00000092,// 146
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=0x0000009f,// 159
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=0x000000bb,// 187
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=0x000000cd,// 205
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=0x000000b1,// 177
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=0x000000c5,// 197
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=0x000000ad,// 173
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=0x000000a1,// 161
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=0x000000d3,// 211
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=0x000000a2,// 162
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=0x000000d1,// 209
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=0x000000d3,// 211
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=0x000000d0,// 208
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=0x000000a7,// 167
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=0x000000a9,// 169
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=0x000000b9,// 185
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=0x000000ae,// 174
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=0x000000b1,// 177
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=0x000000b2,// 178
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=0x000000bd,// 189
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=0x000000d1,// 209
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=0x000000d0,// 208
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=0x000000c6,// 198
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=0x000000d1,// 209
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=0x000000bf,// 191
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=0x000000c1,// 193
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=0x000000d3,// 211
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=0x000000c1,// 193
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=0x000000dc,// 220
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=0x000000ae,// 174
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=0x000000a1,// 161
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=0x000000ab,// 171
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=0x000000ac,// 172
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=0x000000bd,// 189
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
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=0x0000037a,// 890
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=0x00000359,// 857
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=0x00000352,// 850
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=0x0000034f,// 847
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=0x00000075,// 117
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=0x0000006c,// 108
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=0x00000058,// 88
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=0x0000007c,// 124
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=0x00000034,// 52
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=0x00000033,// 51
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=0x0000002b,// 43
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=0x00000032,// 50
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=0x00000050,// 80
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=0x00000062,// 98
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=0x00000043,// 67
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=0x00000058,// 88
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=0x00000028,// 40
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=0x00000028,// 40
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=0x00000042,// 66
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=0x00000026,// 38
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=0x0000002c,// 44
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=0x0000002a,// 42
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=0x00000032,// 50
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=0x00000029,// 41
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=0x0000002c,// 44
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=0x0000003e,// 62
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=0x0000003b,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=0x0000003b,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=0x00000050,// 80
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=0x0000003d,// 61
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=0x00000030,// 48
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=0x0000002e,// 46
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=0x0000002f,// 47
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=0x00000033,// 51
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=0x00000000,// 0
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

	.cfg_ddr_training_delay_ps[0].	soc_bit_vref = {
	0,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,50,40,50,40,50,40,50},//total 44
	.cfg_ddr_training_delay_ps[0].	dram_bit_vref = {
	0,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32},//total 36
	.cfg_ddr_training_delay_ps[0].reserve_training_parameter = {
		(0 << 7) | 6, (0 << 7) | 16, (0 << 7) | 10, (0 << 7) | 3,
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		(0 << 7) | 2, (0 << 7) | 4, (0 << 7) | 4, (0 << 7) | 10,
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
	},

	//pinmux setting
	#if 1  //af409
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	8,	27,	10,	2,	9,	21,	5,	14,	1,	0,	25,	13,	12,	4,	7,	22,	0,	0,	0,	0,	6,	3,	20,	0,	0,	15,	26,	11,
	},
	#endif
	#if 0  //af419
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	27,	12,	21,	9,	8,	0,	14,	10,	6,	7,	20,	5,	22,	13,	15,	2,	0,	0,	0,	0,	11,	26,	4,	0,	0,	25,	3,	1,
	},
	#endif
	.cfg_board_common_setting.	ddr_dqs_swap	=	0,

	#if 1
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	0,1,2,3,4,5,6,7,
	8,9,10,11,12,13,14,15,
	16,17,18,19,20,21,22,23,
	24,25,26,27,28,29,30,31,
	32,33,34,35
	},//d0-d31 dm0 dm1 dm2 dm3
	#endif

	.cfg_board_common_setting.	timming_magic				=	0x66223333,//ddr_set_t start
	.cfg_board_common_setting.	timming_max_valid_configs	=	TIMMING_MAX_CONFIG,//sizeof(__ddr_setting)/sizeof(ddr_set_t),
	.cfg_board_common_setting.	timming_struct_version		=	0,
	.cfg_board_common_setting.	timming_struct_org_size		=	sizeof(ddr_set_t),
	.cfg_board_common_setting.	timming_struct_real_size	=	0,	//0
	.cfg_board_common_setting.	ddr_func					=	0,
	.cfg_board_common_setting.	DisabledDbyte				=	CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.	dram_cs0_base_add			=	0,
	.cfg_board_common_setting.	dram_cs1_base_add			=	0,

	.cfg_board_common_setting.	Is2Ttiming					=	CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.	log_level					=	LOG_LEVEL_BASIC,
	.cfg_board_common_setting.	org_tdqs2dq					=	0,
	.cfg_board_common_setting.	reserve1_test_function		=	{0},
	.cfg_board_common_setting.	ddr_vddee_setting			=	{0},

	//DDR frequency 2
	.cfg_board_SI_setting_ps[1].DRAMFreq					=	600,
	.cfg_board_SI_setting_ps[1].PllBypassEn					=	0,
	.cfg_board_SI_setting_ps[1].training_SequenceCtrl		=	0,
	.cfg_board_SI_setting_ps[1].ddr_odt_config				=	DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[1].clk_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].cs_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].ac_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_p			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_n			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_p			=	DDR_SOC_DATA_DRV_ODT_120_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_n			=	DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_drv_ohm			=	DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_odt_ohm			=	DDR_DRAM_DDR4_ODT_60_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_wr_odt_ohm		=	DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_ac_odt_ohm				=	DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[1].reserve2					=	0,
	.cfg_board_SI_setting_ps[1].vref_ac_permil 				=	0,
	.cfg_board_SI_setting_ps[1].vref_soc_data_permil 		=	0,
	.cfg_board_SI_setting_ps[1].vref_dram_data_permil		=	0,
	.cfg_board_SI_setting_ps[1].max_core_timmming_frequency				=	0,
	.cfg_board_SI_setting_ps[1].training_phase_parameter				=	{0},
	.cfg_board_SI_setting_ps[1].ac_trace_delay_org = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36

	#if 1
	.cfg_ddr_training_delay_ps[1]. ac_trace_delay = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36
	#endif
	.cfg_ddr_training_delay_ps[1]. write_dqs_delay = {0,0,0,0,0,0,0,0},
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32},
	#if 1
	.cfg_ddr_training_delay_ps[1]. write_dq_bit_delay = {
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,},
	#endif

	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {256,256,256,256,256,256,256,256},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_gate_delay = {192,192,192,192,192,192,192,192},//total 8
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_delay = {64,64,64,64,64,64,64,64},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dq_bit_delay = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},//total 72
	.cfg_ddr_training_delay_ps[1]. soc_bit_vref = {
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40},//total 44
	.cfg_ddr_training_delay_ps[1]. dram_bit_vref = {
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32},//total 36

	.cfg_ddr_training_delay_ps[1].reserve_training_parameter	=	{0},//ddr_set_t end
},
#endif //end af400

#if 0//timing_config,af409
{
	//af409
	.cfg_board_common_setting.	board_id					=	CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.	DramType					=	CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.	dram_rank_config			=	CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.	dram_cs0_size_MB			=	CONFIG_DDR0_SIZE_AUTO_SIZE,//CONFIG_DDR0_SIZE_AUTO_SIZE,CONFIG_DDR0_SIZE_2048MB,1024MB,512MB,0MB,256,128,1536,768,3072,4096
	.cfg_board_common_setting.	dram_cs1_size_MB			=	CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.	pll_ssc_mode				=	DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.	ddr_rdbi_wr_enable			=	DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.	dram_x4x8x16_mode			=	CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.	ddr_dmc_remap				=	DDR_DMC_REMAP_DDR4_32BIT,
	.cfg_board_common_setting.	fast_boot					=	{0,0,(1<<3)|(4)},
	.cfg_board_common_setting.	fast_boot[0]				=	0x1,
	.cfg_board_common_setting.	fast_boot[3]				=	0x46,

	//DDR frequency 1
	.cfg_board_SI_setting_ps[0].DRAMFreq					=	1200,//1320,1200,1104,1008,912,792
	.cfg_board_SI_setting_ps[0].PllBypassEn					=	0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl		=	0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config				=	DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p			=	DDR_SOC_DATA_DRV_ODT_48_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n			=	DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm			=	DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm			=	DDR_DRAM_DDR4_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm		=	DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm				=	DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].reserve2					=	0,
	.cfg_board_SI_setting_ps[0].vref_ac_permil 				=	0,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil 		=	0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil		=	0,
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency				=	0,
	.cfg_board_SI_setting_ps[0].training_phase_parameter				=	{0},
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org = {
	128,128,90,128,128,128,128,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	},//total 36

	#if 0
	.cfg_ddr_training_delay_ps[0].	ac_trace_delay = {
	128,128,128,128,128,128,128,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	},//total 36
	#endif
	#if 1
	.cfg_ddr_training_delay_ps[0].	ac_trace_delay = {
	128,128,90,128,128,128,128,256,
	256,256,256,256,256,256,256,256,
	256,256,256,256,256,256,256,256,
	256,256,256,256,256,256,256,256,
	256,256,256,256,
	},//total 36
	#endif
	//write delay line setting
	#if 0
	.cfg_ddr_training_delay_ps[0].	write_dqs_delay = {0+256,0+256,0+256,0+256,0+256,0+256,0+256,0+256} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,

	.cfg_ddr_training_delay_ps[0].	write_dq_bit_delay = {
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,},
	#endif
	#if 1
	.cfg_ddr_training_delay_ps[0].  write_dqs_delay = {266,266,266,266,0+300-128,0+300-128,0+300-128,0+300-128} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,

	.cfg_ddr_training_delay_ps[0].	write_dq_bit_delay = {
	50+266,50+266,50+266,50+266,50+266,50+266,50+266,50+266,
	50+266,50+266,50+266,50+266,50+266,50+266,50+266,50+266,
	50+266,50+266,50+266,50+266,50+266,50+266,50+266,50+266,
	50+266,50+266,50+266,50+266,50+266,50+266,50+266,50+266,
	50+266,50+266,50+266,50+266,50+266,50+266,50+266,50+266,
	50+266,50+266,50+266,50+266,50+266,50+266,50+266,50+266,
	50+266,50+266,50+266,50+266,50+266,50+266,50+266,50+266,
	50+266,50+266,50+266,50+266,50+266,50+266,50+266,50+266,
	50+266,50+266,50+266,50+266,50+266,50+266,50+266,50+266,},
	#endif
	//.cfg_ddr_training_delay_ps[0].	read_dqs_gate_delay = {256,256,256,256,256,256,256,256} , //total 8
	.cfg_ddr_training_delay_ps[0].	read_dqs_gate_delay = {192,192,192,192,192,192,192,192} , //total 8 //edge mode
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64} , //total 8

	.cfg_ddr_training_delay_ps[0].	read_dqs_delay = {64,64,64,64,64,64,64,64} , //total 8
	.cfg_ddr_training_delay_ps[0].	read_dq_bit_delay = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0} , //total 72
	.cfg_ddr_training_delay_ps[0].	soc_bit_vref = {
	0,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,50,40,50,40,50,40,50} , //total 44
	.cfg_ddr_training_delay_ps[0].	dram_bit_vref = {
	0,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32},//total 36
	.cfg_ddr_training_delay_ps[0].	reserve_training_parameter	=	{0}	,
	//.cfg_ddr_training_delay_ps[0].reserve_training_parameter={((1<<7)|1),},

	//pinmux setting
	#if 1  //af409
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	8,	27,	10,	2,	9,	21,	5,	14,	1,	0,	25,	13,	12,	4,	7,	22,	0,	0,	0,	0,	6,	3,	20,	0,	0,	15,	26,	11,
	},
	#endif
	#if 0  //af419
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	27,	12,	21,	9,	8,	0,	14,	10,	6,	7,	20,	5,	22,	13,	15,	2,	0,	0,	0,	0,	11,	26,	4,	0,	0,	25,	3,	1,
	},
	#endif
	.cfg_board_common_setting.	ddr_dqs_swap	=	0,

	#if 1
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	0,1,2,3,4,5,6,7,
	8,9,10,11,12,13,14,15,
	16,17,18,19,20,21,22,23,
	24,25,26,27,28,29,30,31,
	32,33,34,35
	},//d0-d31 dm0 dm1 dm2 dm3
	#endif

	.cfg_board_common_setting.	timming_magic				=	0x66223333,//ddr_set_t start
	.cfg_board_common_setting.	timming_max_valid_configs	=	TIMMING_MAX_CONFIG,//sizeof(__ddr_setting)/sizeof(ddr_set_t),
	.cfg_board_common_setting.	timming_struct_version		=	0,
	.cfg_board_common_setting.	timming_struct_org_size		=	sizeof(ddr_set_t),
	.cfg_board_common_setting.	timming_struct_real_size	=	0,	//0
	.cfg_board_common_setting.	ddr_func					=	0,
	.cfg_board_common_setting.	DisabledDbyte				=	CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.	dram_cs0_base_add			=	0,
	.cfg_board_common_setting.	dram_cs1_base_add			=	0,

	.cfg_board_common_setting.	Is2Ttiming					=	CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.	log_level					=	LOG_LEVEL_BASIC,
	.cfg_board_common_setting.	org_tdqs2dq					=	0,
	.cfg_board_common_setting.	reserve1_test_function		=	{0},
	.cfg_board_common_setting.	ddr_vddee_setting			=	{0},

	//DDR frequency 2
	.cfg_board_SI_setting_ps[1].DRAMFreq					=	600,
	.cfg_board_SI_setting_ps[1].PllBypassEn					=	0,
	.cfg_board_SI_setting_ps[1].training_SequenceCtrl		=	0,
	.cfg_board_SI_setting_ps[1].ddr_odt_config				=	DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[1].clk_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].cs_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].ac_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_p			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_n			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_p			=	DDR_SOC_DATA_DRV_ODT_120_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_n			=	DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_drv_ohm			=	DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_odt_ohm			=	DDR_DRAM_DDR4_ODT_60_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_wr_odt_ohm		=	DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_ac_odt_ohm				=	DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[1].reserve2					=	0,
	.cfg_board_SI_setting_ps[1].vref_ac_permil 				=	0,
	.cfg_board_SI_setting_ps[1].vref_soc_data_permil 		=	0,
	.cfg_board_SI_setting_ps[1].vref_dram_data_permil		=	0,
	.cfg_board_SI_setting_ps[1].max_core_timmming_frequency				=	0,
	.cfg_board_SI_setting_ps[1].training_phase_parameter				=	{0},
	.cfg_board_SI_setting_ps[1].ac_trace_delay_org = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36

	#if 1
	.cfg_ddr_training_delay_ps[1]. ac_trace_delay = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36
	#endif
	.cfg_ddr_training_delay_ps[1]. write_dqs_delay = {0,0,0,0,0,0,0,0},
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32},
	#if 1
	.cfg_ddr_training_delay_ps[1]. write_dq_bit_delay = {
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,},
	#endif

	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {256,256,256,256,256,256,256,256},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_gate_delay = {192,192,192,192,192,192,192,192},//total 8
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_delay = {64,64,64,64,64,64,64,64},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dq_bit_delay = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},//total 72
	.cfg_ddr_training_delay_ps[1]. soc_bit_vref = {
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40},//total 44
	.cfg_ddr_training_delay_ps[1]. dram_bit_vref = {
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32},//total 36

	.cfg_ddr_training_delay_ps[1].reserve_training_parameter	=	{0},//ddr_set_t end
},
#endif //end af409

#if 0//timing_config,af419
{
	//af419
	.cfg_board_common_setting.	board_id					=	CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.	DramType					=	CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.	dram_rank_config			=	CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.	dram_cs0_size_MB			=	CONFIG_DDR0_SIZE_AUTO_SIZE,//CONFIG_DDR0_SIZE_AUTO_SIZE,CONFIG_DDR0_SIZE_2048MB,1024MB,512MB,0MB,256,128,1536,768,3072,4096
	.cfg_board_common_setting.	dram_cs1_size_MB			=	CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.	pll_ssc_mode				=	DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.	ddr_rdbi_wr_enable			=	DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.	dram_x4x8x16_mode			=	CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.	ddr_dmc_remap				=	DDR_DMC_REMAP_DDR4_32BIT,
	.cfg_board_common_setting.	fast_boot					=	{0,0,(1<<3)|(4)},
	.cfg_board_common_setting.	fast_boot[0]				=	0x1,
	.cfg_board_common_setting.	fast_boot[3]				=	0x46,

	//DDR frequency 1
	.cfg_board_SI_setting_ps[0].DRAMFreq					=	1200,//1320,1200,1104,1008,912,792
	.cfg_board_SI_setting_ps[0].PllBypassEn					=	0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl		=	0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config				=	DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p			=	DDR_SOC_DATA_DRV_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n			=	DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm			=	DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm			=	DDR_DRAM_DDR4_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm		=	DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm				=	DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].reserve2					=	0,
	.cfg_board_SI_setting_ps[0].vref_ac_permil 				=	0,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil 		=	0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil		=	0,
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency				=	0,
	.cfg_board_SI_setting_ps[0].training_phase_parameter				=	{0},
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org = {
	128,128,128-40,128,128,128,128,128,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	},//total 36

	#if 1
	.cfg_ddr_training_delay_ps[0].	ac_trace_delay = {
	128,128,128-40,128,128,128,128,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	},//total 36
	#endif
	//write delay line setting
	#if 0
	.cfg_ddr_training_delay_ps[0].	write_dqs_delay = {0+256,0+256,0+256,0+256,0+256,0+256,0+256,0+256} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,

	.cfg_ddr_training_delay_ps[0].	write_dq_bit_delay = {
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,},
	#endif

	#if 1
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=0x00000154,// 340
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=0x0000014f,// 335
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=0x0000013b,// 315
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=0x00000159,// 345
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x00000100,// 256
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000100,// 256
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000100,// 256
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000100,// 256
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=0x0000019e,// 414
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=0x00000189,// 393
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=0x00000183,// 387
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=0x000001a6,// 422
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=0x000001ab,// 427
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=0x00000192,// 402
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=0x0000018d,// 397
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=0x00000192,// 402
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=0x00000196,// 406
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=0x00000173,// 371
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=0x0000017c,// 380
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=0x0000018e,// 398
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=0x00000186,// 390
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=0x00000179,// 377
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=0x00000184,// 388
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=0x00000180,// 384
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=0x0000017d,// 381
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=0x0000016d,// 365
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=0x0000017a,// 378
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=0x0000018c,// 396
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=0x0000017e,// 382
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=0x0000018f,// 399
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=0x00000183,// 387
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=0x0000018d,// 397
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=0x0000017e,// 382
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=0x0000017c,// 380
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=0x00000197,// 407
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=0x0000018e,// 398
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=0x00000199,// 409
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=0x00000181,// 385
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=0x0000017b,// 379
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=0x0000018f,// 399
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=0x0000017c,// 380
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=0x00000189,// 393
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
	#endif

	//.cfg_ddr_training_delay_ps[0].	read_dqs_gate_delay = {256,256,256,256,256,256,256,256} , //total 8
	.cfg_ddr_training_delay_ps[0].	read_dqs_gate_delay = {192,192,192,192,192,192,192,192} , //total 8 //edge mode
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64} , //total 8

	.cfg_ddr_training_delay_ps[0].	read_dqs_delay = {64,64,64,64,64,64,64,64} , //total 8
	.cfg_ddr_training_delay_ps[0].	read_dq_bit_delay = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0} , //total 72
	.cfg_ddr_training_delay_ps[0].	soc_bit_vref = {
	0,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40} , //total 44
	.cfg_ddr_training_delay_ps[0].	dram_bit_vref = {
	0,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32},//total 36
	.cfg_ddr_training_delay_ps[0].	reserve_training_parameter	=	{0}	,
	//.cfg_ddr_training_delay_ps[0].reserve_training_parameter={((1<<7)|1),},

	//pinmux setting
	#if 0  //af409
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	8,	27,	10,	2,	9,	21,	5,	14,	1,	0,	25,	13,	12,	4,	7,	22,	0,	0,	0,	0,	6,	3,	20,	0,	0,	15,	26,	11,
	},
	#endif
	#if 1  //af419
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	27,	12,	21,	9,	8,	0,	14,	10,	6,	7,	20,	5,	22,	13,	15,	2,	0,	0,	0,	0,	11,	26,	4,	0,	0,	25,	3,	1,
	},
	#endif
	.cfg_board_common_setting.	ddr_dqs_swap	=	0,

	#if 1
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	0,1,2,3,4,5,6,7,
	8,9,10,11,12,13,14,15,
	16,17,18,19,20,21,22,23,
	24,25,26,27,28,29,30,31,
	32,33,34,35
	},//d0-d31 dm0 dm1 dm2 dm3
	#endif

	.cfg_board_common_setting.	timming_magic				=	0x66223333,//ddr_set_t start
	.cfg_board_common_setting.	timming_max_valid_configs	=	TIMMING_MAX_CONFIG,//sizeof(__ddr_setting)/sizeof(ddr_set_t),
	.cfg_board_common_setting.	timming_struct_version		=	0,
	.cfg_board_common_setting.	timming_struct_org_size		=	sizeof(ddr_set_t),
	.cfg_board_common_setting.	timming_struct_real_size	=	0,	//0
	.cfg_board_common_setting.	ddr_func					=	0,
	.cfg_board_common_setting.	DisabledDbyte				=	CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.	dram_cs0_base_add			=	0,
	.cfg_board_common_setting.	dram_cs1_base_add			=	0,

	.cfg_board_common_setting.	Is2Ttiming					=	CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.	log_level					=	LOG_LEVEL_BASIC,
	.cfg_board_common_setting.	org_tdqs2dq					=	0,
	.cfg_board_common_setting.	reserve1_test_function		=	{0},
	.cfg_board_common_setting.	ddr_vddee_setting			=	{0},

	//DDR frequency 2
	.cfg_board_SI_setting_ps[1].DRAMFreq					=	600,
	.cfg_board_SI_setting_ps[1].PllBypassEn					=	0,
	.cfg_board_SI_setting_ps[1].training_SequenceCtrl		=	0,
	.cfg_board_SI_setting_ps[1].ddr_odt_config				=	DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[1].clk_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].cs_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].ac_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_p			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_n			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_p			=	DDR_SOC_DATA_DRV_ODT_120_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_n			=	DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_drv_ohm			=	DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_odt_ohm			=	DDR_DRAM_DDR4_ODT_60_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_wr_odt_ohm		=	DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_ac_odt_ohm				=	DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[1].reserve2					=	0,
	.cfg_board_SI_setting_ps[1].vref_ac_permil 				=	0,
	.cfg_board_SI_setting_ps[1].vref_soc_data_permil 		=	0,
	.cfg_board_SI_setting_ps[1].vref_dram_data_permil		=	0,
	.cfg_board_SI_setting_ps[1].max_core_timmming_frequency				=	0,
	.cfg_board_SI_setting_ps[1].training_phase_parameter				=	{0},
	.cfg_board_SI_setting_ps[1].ac_trace_delay_org = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36

	#if 1
	.cfg_ddr_training_delay_ps[1]. ac_trace_delay = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36
	#endif
	.cfg_ddr_training_delay_ps[1]. write_dqs_delay = {0,0,0,0,0,0,0,0},
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32},
	#if 1
	.cfg_ddr_training_delay_ps[1]. write_dq_bit_delay = {
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,},
	#endif

	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {256,256,256,256,256,256,256,256},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_gate_delay = {192,192,192,192,192,192,192,192},//total 8
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_delay = {64,64,64,64,64,64,64,64},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dq_bit_delay = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},//total 72
	.cfg_ddr_training_delay_ps[1]. soc_bit_vref = {
	0,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40},//total 44
	.cfg_ddr_training_delay_ps[1]. dram_bit_vref = {
	0,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32},//total 36

	.cfg_ddr_training_delay_ps[1].reserve_training_parameter	=	{0},//ddr_set_t end
},
#endif //end af419

#if 1//timing_config,af401 DDR3
{
	//af401 DDR3
	.cfg_board_common_setting.	board_id					=	CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.	DramType					=	CONFIG_DDR_TYPE_DDR3,
	.cfg_board_common_setting.	dram_rank_config			=	CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.	dram_cs0_size_MB			=	CONFIG_DDR0_SIZE_512MB,//CONFIG_DDR0_SIZE_AUTO_SIZE,CONFIG_DDR0_SIZE_1024MB,CONFIG_DDR0_SIZE_768MB
	.cfg_board_common_setting.	dram_cs1_size_MB			=	CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.	pll_ssc_mode				=	DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.	ddr_rdbi_wr_enable			=	DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.	dram_x4x8x16_mode			=	CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.	ddr_dmc_remap				=	DDR_DMC_REMAP_DDR3_32BIT,
	.cfg_board_common_setting.	fast_boot					=	{0},
	.cfg_board_common_setting.	fast_boot[0]				=	0x1,
	.cfg_board_common_setting.	fast_boot[3]				=	0x46,

	//DDR frequency 1
	.cfg_board_SI_setting_ps[0].DRAMFreq					=	912,//1008,912,792,667
	.cfg_board_SI_setting_ps[0].PllBypassEn					=	0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl		=	0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config				=	DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p			=	DDR_SOC_DATA_DRV_ODT_120_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n			=	DDR_SOC_DATA_DRV_ODT_120_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm			=	DDR_DRAM_DDR3_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm			=	DDR_DRAM_DDR3_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm		=	DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm				=	DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].reserve2					=	0,
	.cfg_board_SI_setting_ps[0].vref_ac_permil 				=	0,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil 		=	0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil		=	0,
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency				=	0,
	.cfg_board_SI_setting_ps[0].training_phase_parameter				=	{0},
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org = {
	128,128,128-20,128,128,128,128,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	},//total 36

	#if 1
	.cfg_ddr_training_delay_ps[0].	ac_trace_delay = {
	128,128,128-20,128,128,128,128,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	},//total 36
	#endif
	//write delay line setting
	#if 0
	.cfg_ddr_training_delay_ps[0].	write_dqs_delay = {0,0,0,0,0,0,0,0} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,

	.cfg_ddr_training_delay_ps[0].	write_dq_bit_delay = {
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,},
	#endif
	#if 0
	//.cfg_ddr_training_delay_ps[0].	read_dqs_gate_delay = {256,256,256,256,256,256,256,256} , //total 8
	.cfg_ddr_training_delay_ps[0].	read_dqs_gate_delay = {896-128,896-128,896-128,896-128,896-128,896-128,896-128,896-128} , //total 8
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64} , //total 8

	.cfg_ddr_training_delay_ps[0].	read_dqs_delay = {64,64,64,64,64,64,64,64} , //total 8
	.cfg_ddr_training_delay_ps[0].	read_dq_bit_delay = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0} , //total 72
	#endif

	#if 1
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=0x00000069,// 105
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=0x0000007d,// 125
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=0x00000069,// 105
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=0x00000087,// 135
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x000000a0,// 160
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000019,// 25
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x000000a0,// 160
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x000000a0,// 160
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=0x000000ad,// 173
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=0x0000009c,// 156
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=0x0000009e,// 158
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=0x000000a9,// 169
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=0x000000b5,// 181
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=0x000000b8,// 184
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=0x000000af,// 175
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=0x000000b6,// 182
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=0x000000aa,// 170
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=0x000000a5,// 165
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=0x000000c8,// 200
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=0x000000af,// 175
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=0x000000c7,// 199
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=0x000000bf,// 191
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=0x000000ba,// 186
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=0x000000b4,// 180
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=0x000000ab,// 171
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=0x000000b6,// 182
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=0x000000ac,// 172
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=0x000000ae,// 174
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=0x000000af,// 175
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=0x000000b4,// 180
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=0x000000c4,// 196
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=0x000000ba,// 186
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=0x000000c1,// 193
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=0x000000c4,// 196
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=0x000000ba,// 186
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=0x000000c8,// 200
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=0x000000d2,// 210
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=0x000000c5,// 197
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=0x000000d3,// 211
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=0x000000af,// 175
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=0x000000b7,// 183
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=0x000000b7,// 183
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=0x000000b9,// 185
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=0x000000c1,// 193

	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=0x0000037e,// 894
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=0x0000036a,// 874
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=0x00000378,// 888
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=0x00000370,// 880

	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=0x00000087,// 135
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=0x0000008b,// 139
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=0x00000072,// 114
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=0x00000093,// 147

	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=0x0000003c,// 60
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=0x0000003a,// 58
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=0x00000042,// 66
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=0x00000053,// 83
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=0x00000077,// 119
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=0x0000003e,// 62
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=0x00000043,// 67
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=0x00000042,// 66
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=0x00000094,// 148
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=0x00000040,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=0x0000003c,// 60
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=0x00000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=0x00000052,// 82
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=0x00000061,// 97
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=0x00000053,// 83
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=0x0000005a,// 90
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=0x0000003b,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=0x00000040,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=0x00000047,// 71
	#endif

	.cfg_ddr_training_delay_ps[0].	soc_bit_vref = {
	32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,
	32,32+5,32,32+5,32,32+5,32,32+5,},//total 44

	.cfg_ddr_training_delay_ps[0].	dram_bit_vref = {
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32},//total 36
	.cfg_ddr_training_delay_ps[0].	reserve_training_parameter	=	{0}	,
	//.cfg_ddr_training_delay_ps[0].reserve_training_parameter={((1<<7)|1),},
	//.cfg_ddr_training_delay_ps[0].reserve_training_parameter	=	{DDR_SOC_READ_DQS_GATE_MODE_EDGE_MODE_1_DDR3,}	,

	//pinmux setting
	#if 0  //af409
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	8,	27,	10,	2,	9,	21,	5,	14,	1,	0,	25,	13,	12,	4,	7,	22,	0,	0,	0,	0,	6,	3,	20,	0,	0,	15,	26,	11,
	},
	#endif
	#if 0  //af419
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	27,	12,	21,	9,	8,	0,	14,	10,	6,	7,	20,	5,	22,	13,	15,	2,	0,	0,	0,	0,	11,	26,	4,	0,	0,	25,	3,	1,
	},
	#endif
	#if 1  //real DDR3
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	0,	0,	0,	0,	20,	21,	22,	0,	0,	25,	26,	27,	0
	},
	#endif
	.cfg_board_common_setting.	ddr_dqs_swap	=	0,

	#if 1
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	0,1,2,3,4,5,6,7,
	8,9,10,11,12,13,14,15,
	16,17,18,19,20,21,22,23,
	24,25,26,27,28,29,30,31,
	32,33,34,35
	},//d0-d31 dm0 dm1 dm2 dm3
	#endif

	.cfg_board_common_setting.	timming_magic				=	0x66223333,//ddr_set_t start
	.cfg_board_common_setting.	timming_max_valid_configs	=	TIMMING_MAX_CONFIG,//sizeof(__ddr_setting)/sizeof(ddr_set_t),
	.cfg_board_common_setting.	timming_struct_version		=	0,
	.cfg_board_common_setting.	timming_struct_org_size		=	sizeof(ddr_set_t),
	.cfg_board_common_setting.	timming_struct_real_size	=	0,	//0
	.cfg_board_common_setting.	ddr_func					=	0,
	.cfg_board_common_setting.	DisabledDbyte				=	CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.	dram_cs0_base_add			=	0,
	.cfg_board_common_setting.	dram_cs1_base_add			=	0,

	.cfg_board_common_setting.	Is2Ttiming					=	CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.	log_level					=	LOG_LEVEL_BASIC,
	.cfg_board_common_setting.	org_tdqs2dq					=	0,
	.cfg_board_common_setting.	reserve1_test_function		=	{0},
	.cfg_board_common_setting.	ddr_vddee_setting			=	{0},

	//DDR frequency 2
	.cfg_board_SI_setting_ps[1].DRAMFreq					=	600,
	.cfg_board_SI_setting_ps[1].PllBypassEn					=	0,
	.cfg_board_SI_setting_ps[1].training_SequenceCtrl		=	0,
	.cfg_board_SI_setting_ps[1].ddr_odt_config				=	DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[1].clk_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].cs_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].ac_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_p			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_n			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_p			=	DDR_SOC_DATA_DRV_ODT_120_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_n			=	DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_drv_ohm			=	DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_odt_ohm			=	DDR_DRAM_DDR4_ODT_60_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_wr_odt_ohm		=	DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_ac_odt_ohm				=	DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[1].reserve2					=	0,
	.cfg_board_SI_setting_ps[1].vref_ac_permil 				=	0,
	.cfg_board_SI_setting_ps[1].vref_soc_data_permil 		=	0,
	.cfg_board_SI_setting_ps[1].vref_dram_data_permil		=	0,
	.cfg_board_SI_setting_ps[1].max_core_timmming_frequency				=	0,
	.cfg_board_SI_setting_ps[1].training_phase_parameter				=	{0},
	.cfg_board_SI_setting_ps[1].ac_trace_delay_org = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36

	#if 1
	.cfg_ddr_training_delay_ps[1]. ac_trace_delay = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36
	#endif
	.cfg_ddr_training_delay_ps[1]. write_dqs_delay = {0,0,0,0,0,0,0,0},
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32},
	#if 1
	.cfg_ddr_training_delay_ps[1]. write_dq_bit_delay = {
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,},
	#endif

	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {256,256,256,256,256,256,256,256},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_gate_delay = {192,192,192,192,192,192,192,192},//total 8
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_delay = {64,64,64,64,64,64,64,64},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dq_bit_delay = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},//total 72
	.cfg_ddr_training_delay_ps[1]. soc_bit_vref = {
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40},//total 44
	.cfg_ddr_training_delay_ps[1]. dram_bit_vref = {
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32},//total 36

	.cfg_ddr_training_delay_ps[1].reserve_training_parameter	=	{0},//ddr_set_t end
},
#endif //end af401 DDR3

#if 0//timing_config,google DDR3 board
{
	//google DDR3 board
	.cfg_board_common_setting.	board_id					=	CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.	DramType					=	CONFIG_DDR_TYPE_DDR3,
	.cfg_board_common_setting.	dram_rank_config			=	CONFIG_DDR0_32BIT_16BIT_RANK0_CH0,
	.cfg_board_common_setting.	dram_cs0_size_MB			=	CONFIG_DDR0_SIZE_768MB,//CONFIG_DDR0_SIZE_AUTO_SIZE,CONFIG_DDR0_SIZE_1024MB,CONFIG_DDR0_SIZE_768MB
	.cfg_board_common_setting.	dram_cs1_size_MB			=	CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.	pll_ssc_mode				=	DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.	ddr_rdbi_wr_enable			=	DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.	dram_x4x8x16_mode			=	CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.	ddr_dmc_remap				=	DDR_DMC_REMAP_DDR3_32BIT,
	.cfg_board_common_setting.	fast_boot					=	{0},

	//DDR frequency 1
	.cfg_board_SI_setting_ps[0].DRAMFreq					=	912,//1008,912,792,667
	.cfg_board_SI_setting_ps[0].PllBypassEn					=	0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl		=	0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config				=	DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p			=	DDR_SOC_DATA_DRV_ODT_120_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n			=	DDR_SOC_DATA_DRV_ODT_120_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm			=	DDR_DRAM_DDR3_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm			=	DDR_DRAM_DDR3_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm		=	DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm				=	DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].reserve2					=	0,
	.cfg_board_SI_setting_ps[0].vref_ac_permil 				=	0,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil 		=	0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil		=	0,
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency				=	0,
	.cfg_board_SI_setting_ps[0].training_phase_parameter				=	{0},
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org = {
	128,128,128-20,128,128,128,128,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	},//total 36

	#if 1
	.cfg_ddr_training_delay_ps[0].	ac_trace_delay = {
	128,128,128-20,128,128,128,128,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	},//total 36
	#endif
	//write delay line setting
	#if 0
	.cfg_ddr_training_delay_ps[0].	write_dqs_delay = {0,0,0,0,0,0,0,0} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,

	.cfg_ddr_training_delay_ps[0].	write_dq_bit_delay = {
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,},
	#endif
	#if 0
	//.cfg_ddr_training_delay_ps[0].	read_dqs_gate_delay = {256,256,256,256,256,256,256,256} , //total 8
	.cfg_ddr_training_delay_ps[0].	read_dqs_gate_delay = {896-128,896-128,896-128,896-128,896-128,896-128,896-128,896-128} , //total 8
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64} , //total 8

	.cfg_ddr_training_delay_ps[0].	read_dqs_delay = {64,64,64,64,64,64,64,64} , //total 8
	.cfg_ddr_training_delay_ps[0].	read_dq_bit_delay = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0} , //total 72
	#endif

	#if 1
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=0x0000007d,// 125
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=0x00000091,// 145
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=0x00000091,// 145
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=0x0000009b,// 155
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x000000a0,// 160
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x000000a0,// 160
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x000000a0,// 160
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x000000a0,// 160
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=0x000000c2,// 194
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=0x000000b0,// 176
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=0x000000af,// 175
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=0x000000b0,// 176
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=0x000000c0,// 192
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=0x000000c3,// 195
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=0x000000b4,// 180
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=0x000000be,// 190
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=0x000000ba,// 186
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=0x000000c1,// 193
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=0x000000d5,// 213
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=0x000000c1,// 193
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=0x000000d0,// 208
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=0x000000cc,// 204
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=0x000000c3,// 195
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=0x000000be,// 190
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=0x000000c0,// 192
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=0x000000c8,// 200
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=0x000000e0,// 224
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=0x000000e5,// 229
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=0x000000e2,// 226
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=0x000000e7,// 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=0x000000e4,// 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=0x000000e6,// 230
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=0x000000eb,// 235
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=0x000000e7,// 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=0x000000fd,// 253
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=0x000000e8,// 232
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=0x000000e4,// 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=0x000000ee,// 238
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=0x000000f2,// 242
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
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=0x0000038c,// 908
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=0x0000037a,// 890
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=0x0000039a,// 922
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=0x00000395,// 917
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=0x0000007f-0x30,// 127
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=0x0000007d-0x30,// 125
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=0x0000006d-0x30,// 109
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=0x0000006f-0x30,// 111
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=0x00000048-0x30,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=0x00000044-0x30,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=0x00000038-0x30,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=0x0000003b-0x30,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=0x00000049-0x30,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=0x0000004b-0x30,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=0x0000003b-0x30,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=0x00000049-0x30,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=0x00000040-0x30,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=0x00000052-0x30,// 82
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=0x00000039-0x30,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=0x00000040-0x30,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=0x00000045-0x30,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=0x00000042-0x30,// 66
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=0x00000034-0x30,// 52
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=0x0000003c-0x30,// 60
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=0x00000086-0x30,// 134
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=0x0000003f-0x30,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=0x00000048-0x30,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=0x00000037-0x30,// 55
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=0x00000048-0x30,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=(0x0000004e)-0x30,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=0x00000044-0x30,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=0x00000042-0x30,// 66
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=0x00000047-0x30,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=0x0000007f-0x30,// 127
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=0x00000040-0x30,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=0x00000052-0x30,// 82
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=0x0000003d-0x30,// 61
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=(0x0000004e)-0x30,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=0x0000003c-0x30,// 60
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=(0x0000003e)-0x30,// 62
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=0x0000003f-0x30,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=0x0000004c-0x30,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=0x00000091-0x30,// 145
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

	.cfg_ddr_training_delay_ps[0].	soc_bit_vref = {
	32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,
	32,32+5,32,32+5,32,32+5,32,32+5,},//total 44

	.cfg_ddr_training_delay_ps[0].	dram_bit_vref = {
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32},//total 36
	.cfg_ddr_training_delay_ps[0].	reserve_training_parameter	=	{0}	,
	//.cfg_ddr_training_delay_ps[0].reserve_training_parameter={((1<<7)|1),},
	//.cfg_ddr_training_delay_ps[0].reserve_training_parameter	=	{DDR_SOC_READ_DQS_GATE_MODE_EDGE_MODE_1_DDR3,}	,

	//pinmux setting
	#if 0  //af409
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	8,	27,	10,	2,	9,	21,	5,	14,	1,	0,	25,	13,	12,	4,	7,	22,	0,	0,	0,	0,	6,	3,	20,	0,	0,	15,	26,	11,
	},
	#endif
	#if 0  //af419
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	27,	12,	21,	9,	8,	0,	14,	10,	6,	7,	20,	5,	22,	13,	15,	2,	0,	0,	0,	0,	11,	26,	4,	0,	0,	25,	3,	1,
	},
	#endif
	#if 1  //real DDR3
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	0,	0,	0,	0,	20,	21,	22,	0,	0,	25,	26,	27,	0
	},
	#endif
	.cfg_board_common_setting.	ddr_dqs_swap	=	0,

	#if 1
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	0,1,2,3,4,5,6,7,
	8,9,10,11,12,13,14,15,
	16,17,18,19,20,21,22,23,
	24,25,26,27,28,29,30,31,
	32,33,34,35
	},//d0-d31 dm0 dm1 dm2 dm3
	#endif

	.cfg_board_common_setting.	timming_magic				=	0x66223333,//ddr_set_t start
	.cfg_board_common_setting.	timming_max_valid_configs	=	TIMMING_MAX_CONFIG,//sizeof(__ddr_setting)/sizeof(ddr_set_t),
	.cfg_board_common_setting.	timming_struct_version		=	0,
	.cfg_board_common_setting.	timming_struct_org_size		=	sizeof(ddr_set_t),
	.cfg_board_common_setting.	timming_struct_real_size	=	0,	//0
	.cfg_board_common_setting.	ddr_func					=	0,
	.cfg_board_common_setting.	DisabledDbyte				=	CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.	dram_cs0_base_add			=	0,
	.cfg_board_common_setting.	dram_cs1_base_add			=	0,

	.cfg_board_common_setting.	Is2Ttiming					=	CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.	log_level					=	LOG_LEVEL_BASIC,
	.cfg_board_common_setting.	org_tdqs2dq					=	0,
	.cfg_board_common_setting.	reserve1_test_function		=	{0},
	.cfg_board_common_setting.	ddr_vddee_setting			=	{0},

	//DDR frequency 2
	.cfg_board_SI_setting_ps[1].DRAMFreq					=	600,
	.cfg_board_SI_setting_ps[1].PllBypassEn					=	0,
	.cfg_board_SI_setting_ps[1].training_SequenceCtrl		=	0,
	.cfg_board_SI_setting_ps[1].ddr_odt_config				=	DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[1].clk_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].cs_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].ac_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_p			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_n			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_p			=	DDR_SOC_DATA_DRV_ODT_120_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_n			=	DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_drv_ohm			=	DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_odt_ohm			=	DDR_DRAM_DDR4_ODT_60_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_wr_odt_ohm		=	DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_ac_odt_ohm				=	DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[1].reserve2					=	0,
	.cfg_board_SI_setting_ps[1].vref_ac_permil 				=	0,
	.cfg_board_SI_setting_ps[1].vref_soc_data_permil 		=	0,
	.cfg_board_SI_setting_ps[1].vref_dram_data_permil		=	0,
	.cfg_board_SI_setting_ps[1].max_core_timmming_frequency				=	0,
	.cfg_board_SI_setting_ps[1].training_phase_parameter				=	{0},
	.cfg_board_SI_setting_ps[1].ac_trace_delay_org = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36

	#if 1
	.cfg_ddr_training_delay_ps[1]. ac_trace_delay = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36
	#endif
	.cfg_ddr_training_delay_ps[1]. write_dqs_delay = {0,0,0,0,0,0,0,0},
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32},
	#if 1
	.cfg_ddr_training_delay_ps[1]. write_dq_bit_delay = {
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,},
	#endif

	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {256,256,256,256,256,256,256,256},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_gate_delay = {192,192,192,192,192,192,192,192},//total 8
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_delay = {64,64,64,64,64,64,64,64},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dq_bit_delay = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},//total 72
	.cfg_ddr_training_delay_ps[1]. soc_bit_vref = {
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40},//total 44
	.cfg_ddr_training_delay_ps[1]. dram_bit_vref = {
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32},//total 36

	.cfg_ddr_training_delay_ps[1].reserve_training_parameter	=	{0},//ddr_set_t end
},
#endif //end google DDR3 board

#if 0//timing_config,af400 LPDDR4
{
	//af400
	.cfg_board_common_setting.	board_id					=	CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.	DramType					=	CONFIG_DDR_TYPE_LPDDR4,
	.cfg_board_common_setting.	dram_rank_config			=	CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.	dram_cs0_size_MB			=	CONFIG_DDR0_SIZE_1024MB,//CONFIG_DDR0_SIZE_AUTO_SIZE,CONFIG_DDR0_SIZE_1024MB,
	.cfg_board_common_setting.	dram_cs1_size_MB			=	CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.	pll_ssc_mode				=	DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.	ddr_rdbi_wr_enable			=	DDR_WRITE_READ_DBI_DISABLE,//DDR_WRITE_READ_DBI_DISABLE,DDR_READ_DBI_ENABLE
	.cfg_board_common_setting.	dram_x4x8x16_mode			=	CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.	ddr_dmc_remap				=	DDR_DMC_REMAP_LPDDR4_32BIT,//DDR_DMC_REMAP_LPDDR4_32BIT,//DDR_DMC_REMAP_DDR3_32BIT
	.cfg_board_common_setting.	fast_boot					=	{0},

	//DDR frequency 1
	.cfg_board_SI_setting_ps[0].DRAMFreq					=	667,//1320,1200,1104,1008,912,792
	.cfg_board_SI_setting_ps[0].PllBypassEn					=	0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl		=	0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config				=	DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p			=	DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n			=	DDR_SOC_DATA_DRV_ODT_80_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm			=	DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm			=	DDR_DRAM_DDR4_ODT_80_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm		=	DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm				=	DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].reserve2					=	0,
	.cfg_board_SI_setting_ps[0].vref_ac_permil 				=	300,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil 		=	0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil		=	330,//330
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency				=	0,
	.cfg_board_SI_setting_ps[0].training_phase_parameter				=	{0},
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org = {
	0,0,0,0,0,0,0,0, //0,0,0,0,0,0,0,0,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36

	#if 1//default cs cke 0 ,other AC=128
	.cfg_ddr_training_delay_ps[0].	ac_trace_delay = {
	128,128,128,128,128,128,0,0, //0,0,0,0,0,0,0,0,
	128+128,128+128,128+128,128+128,128+128,128+128,128+128,128+128,
	128+128,128+128,128+128,128+128,128+128,128+128,128+128,128+128,
	128+128,128+128,128+128,128+128,128+128,128+128,128+128,128+128,
	128+128,128+128,128+128,128+128,
	},//total 36
	#endif
	//write delay line setting
	#if 1
	.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {128+128,128+128,128+128,128+128,128+128,128+128,128+128,128+128,} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,

	.cfg_ddr_training_delay_ps[0]. write_dq_bit_delay = {
	64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,
	64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,
	64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,
	64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,
	64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,
	64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,
	64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,
	64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,64+128+128+256+128,
	} ,
	#endif
	#if 1
	//.cfg_ddr_training_delay_ps[0].	read_dqs_gate_delay = {256,256,256,256,256,256,256,256} , //total 8
	.cfg_ddr_training_delay_ps[0].	read_dqs_gate_delay = {1024,1024,1024,1024,0,0,0,0} , //total 8 //edge mode
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64} , //total 8

	.cfg_ddr_training_delay_ps[0].	read_dqs_delay = {64,64,64,64,64,64,64,64} , //total 8
	.cfg_ddr_training_delay_ps[0].	read_dq_bit_delay = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0} , //total 72
	#endif

	.cfg_ddr_training_delay_ps[0].soc_bit_vref = {
		0, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 26, 32, 26,
		32, 26, 32, 26
	}, //total 44
	.cfg_ddr_training_delay_ps[0].dram_bit_vref = {
		0, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32
	}, //total 36
	.cfg_ddr_training_delay_ps[0].	reserve_training_parameter	=	{0}	,
	//.cfg_ddr_training_delay_ps[0].reserve_training_parameter={((1<<7)|1),},

	//pinmux setting
	#if 1 //lpddr4
	.cfg_board_common_setting.	ac_pinmux	=
	{
	0,	0,	0,	1,	0,	1,
	0,	5,	0,	1,	0,	0,	4,	0,	5,	0,	1,	3,	3,	4,	2,	0,	0,	0,	0,	0,	0,	2,	0,	0,	0,	0,	0,	0,
	},
	#endif
	#if 0  //af409
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	8,	27,	10,	2,	9,	21,	5,	14,	1,	0,	25,	13,	12,	4,	7,	22,	0,	0,	0,	0,	6,	3,	20,	0,	0,	15,	26,	11,
	},
	#endif
	#if 0  //af419
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	27,	12,	21,	9,	8,	0,	14,	10,	6,	7,	20,	5,	22,	13,	15,	2,	0,	0,	0,	0,	11,	26,	4,	0,	0,	25,	3,	1,
	},
	#endif
	.cfg_board_common_setting.	ddr_dqs_swap	=	0,

	#if 1
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	0,1,2,3,4,5,6,7,
	8,9,10,11,12,13,14,15,
	16,17,18,19,20,21,22,23,
	24,25,26,27,28,29,30,31,
	32,33,34,35
	},//d0-d31 dm0 dm1 dm2 dm3
	#endif
	#if 0
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	8,15,11,13,9,12,14,10,
	3,0,6,4,5,7,2,1,
	17,18,23,22,20,21,19,16,
	28,30,27,25,26,24,29,31,
	33,32,34,35
	}	,	 //d0-d31 dm0 dm1 dm2 dm3
	#endif

	.cfg_board_common_setting.	timming_magic				=	0,//ddr_set_t start
	.cfg_board_common_setting.	timming_max_valid_configs	=	TIMMING_MAX_CONFIG,//sizeof(__ddr_setting)/sizeof(ddr_set_t),
	.cfg_board_common_setting.	timming_struct_version		=	0,
	.cfg_board_common_setting.	timming_struct_org_size		=	sizeof(ddr_set_t),
	.cfg_board_common_setting.	timming_struct_real_size	=	0,	//0
	.cfg_board_common_setting.	ddr_func					=	0,
	.cfg_board_common_setting.	DisabledDbyte				=	CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.	dram_cs0_base_add			=	0,
	.cfg_board_common_setting.	dram_cs1_base_add			=	0,

	.cfg_board_common_setting.	Is2Ttiming					=	CONFIG_USE_DDR_1T_MODE,
	.cfg_board_common_setting.	log_level					=	LOG_LEVEL_BASIC,
	.cfg_board_common_setting.	org_tdqs2dq					=	0,
	.cfg_board_common_setting.	reserve1_test_function		=	{0},
	.cfg_board_common_setting.	ddr_vddee_setting			=	{0},

	//DDR frequency 2
	.cfg_board_SI_setting_ps[1].DRAMFreq					=	667,
	.cfg_board_SI_setting_ps[1].PllBypassEn					=	0,
	.cfg_board_SI_setting_ps[1].training_SequenceCtrl		=	0,
	.cfg_board_SI_setting_ps[1].ddr_odt_config				=	DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[1].clk_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].cs_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].ac_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_p			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_n			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_p			=	DDR_SOC_DATA_DRV_ODT_60_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_n			=	DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_drv_ohm			=	DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_odt_ohm			=	DDR_DRAM_DDR4_ODT_60_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_wr_odt_ohm		=	DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_ac_odt_ohm				=	DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[1].reserve2					=	0,
	.cfg_board_SI_setting_ps[1].vref_ac_permil 				=	0,
	.cfg_board_SI_setting_ps[1].vref_soc_data_permil 		=	0,
	.cfg_board_SI_setting_ps[1].vref_dram_data_permil		=	0,
	.cfg_board_SI_setting_ps[1].max_core_timmming_frequency				=	0,
	.cfg_board_SI_setting_ps[1].training_phase_parameter				=	{0},
	.cfg_board_SI_setting_ps[1].ac_trace_delay_org = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36

	#if 1
	.cfg_ddr_training_delay_ps[1]. ac_trace_delay = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36
	#endif
	.cfg_ddr_training_delay_ps[1]. write_dqs_delay = {0,0,0,0,0,0,0,0},
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32},
	#if 1
	.cfg_ddr_training_delay_ps[1]. write_dq_bit_delay = {
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,},
	#endif

	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {256,256,256,256,256,256,256,256},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_gate_delay = {192,192,192,192,192,192,192,192},//total 8
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_delay = {64,64,64,64,64,64,64,64},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dq_bit_delay = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},//total 72
	.cfg_ddr_training_delay_ps[1]. soc_bit_vref = {
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40},//total 44
	.cfg_ddr_training_delay_ps[1]. dram_bit_vref = {
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32},//total 36

	.cfg_ddr_training_delay_ps[1].reserve_training_parameter	=	{0},//ddr_set_t end
},
#endif //end af400 lpddr4

#if 0//timing_config,af409 LPDDR4
{
	//af409
	.cfg_board_common_setting.	board_id					=	CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.	DramType					=	CONFIG_DDR_TYPE_LPDDR4,
	.cfg_board_common_setting.	dram_rank_config			=	CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.	dram_cs0_size_MB			=	CONFIG_DDR0_SIZE_1024MB,
	.cfg_board_common_setting.	dram_cs1_size_MB			=	CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.	pll_ssc_mode				=	DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.	ddr_rdbi_wr_enable			=	DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.	dram_x4x8x16_mode			=	CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.	ddr_dmc_remap				=	DDR_DMC_REMAP_LPDDR4_32BIT,
	.cfg_board_common_setting.	fast_boot					=	{0},

	//DDR frequency 1
	.cfg_board_SI_setting_ps[0].DRAMFreq					=	667,
	.cfg_board_SI_setting_ps[0].PllBypassEn					=	0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl		=	0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config				=	DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p			=	DDR_SOC_DATA_DRV_ODT_48_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n			=	DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm			=	DDR_DRAM_LPDDR4_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm			=	DDR_DRAM_LPDDR4_ODT_120_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm		=	DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm				=	DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].reserve2					=	0,
	.cfg_board_SI_setting_ps[0].vref_ac_permil 				=	0,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil 		=	0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil		=	0,
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency				=	0,
	.cfg_board_SI_setting_ps[0].training_phase_parameter				=	{0},
	#if 0
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,
	},//total 36
	#endif
	#if 1
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org = {
	0,0,0,0,0,0,0,0,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36
	#endif

	#if 0
	.cfg_ddr_training_delay_ps[0].	ac_trace_delay = {
	128,128,128,128,128,128,128,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	},//total 36
	#endif
	#if 1
	.cfg_ddr_training_delay_ps[0].	ac_trace_delay = {
	0,0,0,0,0,0,0,0,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36
	#endif
	//write delay line setting
	#if 0
	.cfg_ddr_training_delay_ps[0].	write_dqs_delay = {0+256,0+256,0+256,0+256,0+256,0+256,0+256,0+256} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,

	.cfg_ddr_training_delay_ps[0].	write_dq_bit_delay = {
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,
	50+256,50+256,50+256,50+256,50+256,50+256,50+256,50+256,},
	#endif
	#if 1
	.cfg_ddr_training_delay_ps[0].	write_dqs_delay = {0,0,0,0,0,0,0,0} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,

	.cfg_ddr_training_delay_ps[0].	write_dq_bit_delay = {
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,},
	#endif
	//.cfg_ddr_training_delay_ps[0].	read_dqs_gate_delay = {256,256,256,256,256,256,256,256} , //total 8
	.cfg_ddr_training_delay_ps[0].	read_dqs_gate_delay = {0,0,0,0,0,0,0,0} , //total 8 //edge mode
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64} , //total 8

	.cfg_ddr_training_delay_ps[0].	read_dqs_delay = {64,64,64,64,64,64,64,64} , //total 8
	.cfg_ddr_training_delay_ps[0].	read_dq_bit_delay = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0} , //total 72
	.cfg_ddr_training_delay_ps[0].	soc_bit_vref = {
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,40,32,40,32,40,32,40} , //total 44
	.cfg_ddr_training_delay_ps[0].	dram_bit_vref = {
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32},//total 36
	.cfg_ddr_training_delay_ps[0].	reserve_training_parameter	=	{0}	,
	//.cfg_ddr_training_delay_ps[0].reserve_training_parameter={((1<<7)|1),},

	//pinmux setting
	#if 0  //af409
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	8,	27,	10,	2,	9,	21,	5,	14,	1,	0,	25,	13,	12,	4,	7,	22,	0,	0,	0,	0,	6,	3,	20,	0,	0,	15,	26,	11,
	},
	#endif
	#if 0  //af419
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	27,	12,	21,	9,	8,	0,	14,	10,	6,	7,	20,	5,	22,	13,	15,	2,	0,	0,	0,	0,	11,	26,	4,	0,	0,	25,	3,	1,
	},
	#endif
	#if 1  //lpddr4
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	0,	1,
	0,	5,	1,	2,	0,	0,	4,	0,	5,	0,	1,	3,	3,	4,	2,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	},
	#endif
	.cfg_board_common_setting.	ddr_dqs_swap	=	0,

	#if 1
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	8,15,11,13,9,12,14,10,
	3,0,6,4,5,7,2,1,
	17,18,23,22,20,21,19,16,
	28,30,27,25,26,24,29,31,
	33,32,34,35
	},//d0-d31 dm0 dm1 dm2 dm3
	#endif

	.cfg_board_common_setting.	timming_magic				=	0x66223333,//ddr_set_t start
	.cfg_board_common_setting.	timming_max_valid_configs	=	TIMMING_MAX_CONFIG,//sizeof(__ddr_setting)/sizeof(ddr_set_t),
	.cfg_board_common_setting.	timming_struct_version		=	0,
	.cfg_board_common_setting.	timming_struct_org_size		=	sizeof(ddr_set_t),
	.cfg_board_common_setting.	timming_struct_real_size	=	0,	//0
	.cfg_board_common_setting.	ddr_func					=	0,
	.cfg_board_common_setting.	DisabledDbyte				=	CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.	dram_cs0_base_add			=	0,
	.cfg_board_common_setting.	dram_cs1_base_add			=	0,

	.cfg_board_common_setting.	Is2Ttiming					=	CONFIG_USE_DDR_1T_MODE,
	.cfg_board_common_setting.	log_level					=	LOG_LEVEL_BASIC,
	.cfg_board_common_setting.	org_tdqs2dq					=	0,
	.cfg_board_common_setting.	reserve1_test_function		=	{0},
	.cfg_board_common_setting.	ddr_vddee_setting			=	{0},

	//DDR frequency 2
	.cfg_board_SI_setting_ps[1].DRAMFreq					=	600,
	.cfg_board_SI_setting_ps[1].PllBypassEn					=	0,
	.cfg_board_SI_setting_ps[1].training_SequenceCtrl		=	0,
	.cfg_board_SI_setting_ps[1].ddr_odt_config				=	DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[1].clk_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].cs_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].ac_drv_ohm					=	DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_p			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_drv_ohm_n			=	DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_p			=	DDR_SOC_DATA_DRV_ODT_120_OHM,
	.cfg_board_SI_setting_ps[1].soc_data_odt_ohm_n			=	DDR_SOC_DATA_DRV_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_drv_ohm			=	DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_odt_ohm			=	DDR_DRAM_DDR4_ODT_60_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_wr_odt_ohm		=	DDR_DRAM_DDR_WR_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_ac_odt_ohm				=	DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[1].dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[1].lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[1].reserve2					=	0,
	.cfg_board_SI_setting_ps[1].vref_ac_permil 				=	0,
	.cfg_board_SI_setting_ps[1].vref_soc_data_permil 		=	0,
	.cfg_board_SI_setting_ps[1].vref_dram_data_permil		=	0,
	.cfg_board_SI_setting_ps[1].max_core_timmming_frequency				=	0,
	.cfg_board_SI_setting_ps[1].training_phase_parameter				=	{0},
	.cfg_board_SI_setting_ps[1].ac_trace_delay_org = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36

	#if 1
	.cfg_ddr_training_delay_ps[1]. ac_trace_delay = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	},//total 36
	#endif
	.cfg_ddr_training_delay_ps[1]. write_dqs_delay = {0,0,0,0,0,0,0,0},
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32},
	#if 1
	.cfg_ddr_training_delay_ps[1]. write_dq_bit_delay = {
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,},
	#endif

	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {256,256,256,256,256,256,256,256},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_gate_delay = {192,192,192,192,192,192,192,192},//total 8
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_delay = {64,64,64,64,64,64,64,64},//total 8
	.cfg_ddr_training_delay_ps[1]. read_dq_bit_delay = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0},//total 72
	.cfg_ddr_training_delay_ps[1]. soc_bit_vref = {
	0,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40},//total 44
	.cfg_ddr_training_delay_ps[1]. dram_bit_vref = {
	0,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32},//total 36

	.cfg_ddr_training_delay_ps[1].reserve_training_parameter	=	{0},//ddr_set_t end
},
#endif //end af409 LPDDR4

#if 0  //timing_config,for dd3 pxp
//ddr_set_t  ddr_set_t_default=
{
	.cfg_board_common_setting.	timming_magic	=	0	,
	.cfg_board_common_setting.	timming_max_valid_configs	=	sizeof(ddr_set_t_default)/sizeof(ddr_set_t)	,
	.cfg_board_common_setting.	timming_struct_version	=	0	,
	.cfg_board_common_setting.	timming_struct_org_size	=	sizeof(ddr_set_t)	,
	.cfg_board_common_setting.	timming_struct_real_size	=	0	,	//0
	.cfg_board_common_setting.	fast_boot	=	{0}	,
	.cfg_board_common_setting.	ddr_func	=	0	,
	.cfg_board_common_setting.	board_id	=	CONFIG_BOARD_ID_MASK	,
	.cfg_board_common_setting.	DramType	=	CONFIG_DDR_TYPE_DDR3	,
	.cfg_board_common_setting.	dram_rank_config	=	CONFIG_DDR0_32BIT_16BIT_RANK0_CH0	,
	.cfg_board_common_setting.	DisabledDbyte	=	CONFIG_DISABLE_D32_D63	,
	.cfg_board_common_setting.	dram_cs0_base_add	=	0	,
	.cfg_board_common_setting.	dram_cs1_base_add	=	0	,
	.cfg_board_common_setting.	dram_cs0_size_MB	=	CONFIG_DDR0_SIZE_768MB	,
	.cfg_board_common_setting.	dram_cs1_size_MB	=	CONFIG_DDR1_SIZE_0MB	,
	.cfg_board_common_setting.	dram_x4x8x16_mode	=	CONFIG_DRAM_MODE_X16	,
	.cfg_board_common_setting.	Is2Ttiming	=	CONFIG_USE_DDR_2T_MODE	,
	.cfg_board_common_setting.	log_level	=	LOG_LEVEL_BASIC	,
	.cfg_board_common_setting.	ddr_rdbi_wr_enable	=	DDR_WRITE_READ_DBI_DISABLE	,
	.cfg_board_common_setting.	pll_ssc_mode	=	DDR_PLL_SSC_DISABLE	,
	.cfg_board_common_setting.	org_tdqs2dq	=	0	,
	.cfg_board_common_setting.	reserve1_test_function	=	{0}	,
	.cfg_board_common_setting.	ddr_dmc_remap	=	DDR_DMC_REMAP_DDR3_32BIT	,
	#if 0
	.cfg_board_common_setting.	ac_pinmux	=
			{
	0,	0,	0,	1,	2,	3,
	8,	27,	10,	2,	9,	21,	5,	14,	1,	0,	25,	13,	12,	4,	7,	22,	0,	0,	0,	0,	6,	3,	20,	0,	0,	15,	26,	11,
		}	,

		//ac_group0-ac_group1
	#endif
	#if 1
	.cfg_board_common_setting.	ac_pinmux	=
	{
	0,	0,	0,	1,	2,	3,
	0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	0,	0,	0,	0,	20,	21,	22,	0,	0,	25,	26,	27,	0
	}	,
	#endif
	.cfg_board_common_setting.	ddr_dqs_swap	=	0	,
	#if 0
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	3,2,0,1,7,6,5,4,
	14,13,12,15,8,9,11,10,
	20,21,22,23,16,17,19,18,
	24,25,28,26,31,30,27,29,
	32,33,34,35
	}	,	 //d0-d31 dm0 dm1 dm2 dm3
	#endif
	#if 0
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	2,3,1,0,6,7,4,5,
	11,10,9,8,15,14,13,12,
	16,17,18,19,20,21,22,23,
	24,25,26,27,28,29,30,31,
	32,33,34,35
	}	,	 //d0-d31 dm0 dm1 dm2 dm3
	#endif
	#if 0
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	3,2,0,1,7,6,5,4,
	8,9,10,11,12,13,14,15,
	16,17,18,19,20,21,22,23,
	24,25,26,27,28,29,30,31,
	32,33,34,35
	}	,	 //d0-d31 dm0 dm1 dm2 dm3
	//1,0,3,2,5,4,7,6,
	#endif
	#if 0
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	2,7,1,5,3,6,0,4,
	10,11,15,13,14,9,8,12,
	24,29,26,31,25,28,27,30,
	23,18,21,19,22,17,20,16,
	32,33,34,35
	}	,	 //d0-d31 dm0 dm1 dm2 dm3
	#endif

	#if 1
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	0,1,2,3,4,5,6,7,
	8,9,10,11,12,13,14,15,
	16,17,18,19,20,21,22,23,
	24,25,26,27,28,29,30,31,
	32,33,34,35,
	}	,	 //d0-d31 dm0 dm1 dm2 dm3
	#endif
	.cfg_board_common_setting.	ddr_vddee_setting	=	{0}	,


	.cfg_board_SI_setting_ps[0].	DRAMFreq	=	667	,
	.cfg_board_SI_setting_ps[0].	PllBypassEn	=	0	,
	.cfg_board_SI_setting_ps[0].	training_SequenceCtrl	=	0	,
	.cfg_board_SI_setting_ps[0].	ddr_odt_config	=	DDR_DRAM_ODT_W_CS0_ODT0	,
	.cfg_board_SI_setting_ps[0].	clk_drv_ohm	=	DDR_SOC_AC_DRV_40_OHM	,
	.cfg_board_SI_setting_ps[0].	cs_drv_ohm	=	DDR_SOC_AC_DRV_40_OHM	,
	.cfg_board_SI_setting_ps[0].	ac_drv_ohm	=	DDR_SOC_AC_DRV_40_OHM	,
	.cfg_board_SI_setting_ps[0].	soc_data_drv_ohm_p	=	DDR_SOC_DATA_DRV_ODT_40_OHM	,
	.cfg_board_SI_setting_ps[0].	soc_data_drv_ohm_n	=	DDR_SOC_DATA_DRV_ODT_40_OHM	,
	.cfg_board_SI_setting_ps[0].	soc_data_odt_ohm_p	=	DDR_SOC_DATA_DRV_ODT_120_OHM	,
	.cfg_board_SI_setting_ps[0].	soc_data_odt_ohm_n	=	DDR_SOC_DATA_DRV_ODT_120_OHM	,
	.cfg_board_SI_setting_ps[0].	dram_data_drv_ohm	=	DDR_DRAM_DDR3_DRV_34_OHM	,
	.cfg_board_SI_setting_ps[0].	dram_data_odt_ohm	=	DDR_DRAM_DDR3_ODT_60_OHM	,
	.cfg_board_SI_setting_ps[0].	dram_data_wr_odt_ohm	=	DDR_DRAM_DDR_WR_ODT_0_OHM	,
	.cfg_board_SI_setting_ps[0].	dram_ac_odt_ohm	=	DDR_DRAM_DDR_AC_ODT_0_OHM	,
	.cfg_board_SI_setting_ps[0].	dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM	,
	.cfg_board_SI_setting_ps[0].	lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ	,
	.cfg_board_SI_setting_ps[0].	reserve2	=	0	,
	.cfg_board_SI_setting_ps[0].	vref_ac_permil 	=	0	,
	.cfg_board_SI_setting_ps[0].	vref_soc_data_permil 	=	0	,
	.cfg_board_SI_setting_ps[0].	vref_dram_data_permil	=	0	,
	.cfg_board_SI_setting_ps[0].	max_core_timmming_frequency	=	0	,
	.cfg_board_SI_setting_ps[0].	training_phase_parameter	=	{0}	,
	.cfg_board_SI_setting_ps[0]. ac_trace_delay_org = {
	128,128,128,128,128,128,128,128,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	},//total 36
	  #if 0
	.cfg_board_SI_setting_ps[0]. ac_trace_delay_org = {
	123,124,125,126,127,128,128,128,
	129,128,127,126,125,124,123,122,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	  } , //total 36
	  #endif

	#if 1
	.cfg_ddr_training_delay_ps[0]. ac_trace_delay = {
	128,128,128,128,128,128,128,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	  } , //total 36
	  #endif
	  #if 0
	.cfg_ddr_training_delay_ps[0]. ac_trace_delay = {
	123,124,0,126,127,0,0,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	  } , //total 36
	  #endif
	  #if 0
	  .cfg_ddr_training_delay_ps[0]. write_dqs_delay = {0,0,0,0,0,0,0,0} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,

	.cfg_ddr_training_delay_ps[0]. write_dq_bit_delay = {50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,} ,
	#endif
	  #if 0 //real board
	.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {0,0,0,0,0,0,0,0} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {128,128,128,128,128,128,128,128} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {256,256,256,256,256,256,256,256} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,

	.cfg_ddr_training_delay_ps[0]. write_dq_bit_delay = {64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,} ,


	#endif

	  #if 1 //for pxp
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {0,0,0,0,0,0,0,0} ,
	.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {128,128,128,128,128,128,128,128} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {256,256,256,256,256,256,256,256} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,

	.cfg_ddr_training_delay_ps[0]. write_dq_bit_delay = {64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,} ,
	.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {128+512,128+512,128+512,128+512,128+512,128+512,128+512,128+512} , //total 8

	#else
	.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {128,128,128,128,128,128,128,128} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,

	.cfg_ddr_training_delay_ps[0]. write_dq_bit_delay = {64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,
	64+128,64+128,64+128,64+128,64+128,64+128,64+128,64+128,} ,

	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {256,256,256,256,256,256,256,256} , //total 8
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {1094,1094,1094,1094,1094,1094,1094,1094} , //total 8
	.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {896,896,896,896,896,896,896,896} , //total 8
	#endif
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64} , //total 8
	.cfg_ddr_training_delay_ps[0]. read_dqs_delay = {64,64,64,64,64,64,64,64} , //total 8
	.cfg_ddr_training_delay_ps[0]. read_dq_bit_delay = {0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0} , //total 72
	.cfg_ddr_training_delay_ps[0]. soc_bit_vref = {42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,
	42,42,42,42,42,42,42,42,42,42,
	42,42,42,42,42,58,42,58,42,58,42,58,} , //total 44
	.cfg_ddr_training_delay_ps[0]. dram_bit_vref = {32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32} , //total 36
	.cfg_ddr_training_delay_ps[0].	reserve_training_parameter	=	{0}	,

	.cfg_board_SI_setting_ps[1].	DRAMFreq	=	667	,
	.cfg_board_SI_setting_ps[1].	PllBypassEn	=	0	,
	.cfg_board_SI_setting_ps[1].	training_SequenceCtrl	=	0	,
	.cfg_board_SI_setting_ps[1].	ddr_odt_config	=	DDR_DRAM_ODT_W_CS0_ODT0	,
	.cfg_board_SI_setting_ps[1].	clk_drv_ohm	=	DDR_SOC_AC_DRV_40_OHM	,
	.cfg_board_SI_setting_ps[1].	cs_drv_ohm	=	DDR_SOC_AC_DRV_40_OHM	,
	.cfg_board_SI_setting_ps[1].	ac_drv_ohm	=	DDR_SOC_AC_DRV_40_OHM	,
	.cfg_board_SI_setting_ps[1].	soc_data_drv_ohm_p	=	DDR_SOC_DATA_DRV_ODT_40_OHM	,
	.cfg_board_SI_setting_ps[1].	soc_data_drv_ohm_n	=	DDR_SOC_DATA_DRV_ODT_40_OHM	,
	.cfg_board_SI_setting_ps[1].	soc_data_odt_ohm_p	=	DDR_SOC_DATA_DRV_ODT_60_OHM	,
	.cfg_board_SI_setting_ps[1].	soc_data_odt_ohm_n	=	DDR_SOC_DATA_DRV_ODT_0_OHM	,
	.cfg_board_SI_setting_ps[1].	dram_data_drv_ohm	=	DDR_DRAM_DDR4_DRV_34_OHM	,
	.cfg_board_SI_setting_ps[1].	dram_data_odt_ohm	=	DDR_DRAM_DDR4_ODT_60_OHM	,
	.cfg_board_SI_setting_ps[1].	dram_data_wr_odt_ohm	=	DDR_DRAM_DDR_WR_ODT_0_OHM	,
	.cfg_board_SI_setting_ps[1].	dram_ac_odt_ohm	=	DDR_DRAM_DDR_AC_ODT_0_OHM	,
	.cfg_board_SI_setting_ps[1].	dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM	,
	.cfg_board_SI_setting_ps[1].	lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ	,
	.cfg_board_SI_setting_ps[1].	reserve2	=	0	,
	.cfg_board_SI_setting_ps[1].	vref_ac_permil 	=	0	,
	.cfg_board_SI_setting_ps[1].	vref_soc_data_permil 	=	0	,
	.cfg_board_SI_setting_ps[1].	vref_dram_data_permil	=	0	,
	.cfg_board_SI_setting_ps[1].	max_core_timmming_frequency	=	0	,
	.cfg_board_SI_setting_ps[1].	training_phase_parameter	=	{0}	,
	.cfg_board_SI_setting_ps[1]. ac_trace_delay_org = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	  } , //total 36
	  #if 0
	.cfg_board_SI_setting_ps[1]. ac_trace_delay_org = {
	123,124,125,126,127,128,128,128,
	129,128,127,126,125,124,123,122,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	  } , //total 36
	  #endif

	#if 1
	.cfg_ddr_training_delay_ps[1]. ac_trace_delay = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	  } , //total 36
	  #endif
	  #if 0
	.cfg_ddr_training_delay_ps[1]. ac_trace_delay = {
	0,0,0,0,0,0,0,0, //0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,
	  } , //total 36
	  #endif
	.cfg_ddr_training_delay_ps[1]. write_dqs_delay = {0,0,0,0,0,0,0,0} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,
	#if 1
	.cfg_ddr_training_delay_ps[1]. write_dq_bit_delay = {50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,} ,
	#endif
	#if 0
	.cfg_ddr_training_delay_ps[1]. write_dq_bit_delay = {10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,} ,
	#endif
	#if 0
	.cfg_ddr_training_delay_ps[1]. write_dq_bit_delay = {64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64} ,
	//total 72
	#endif
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {256,256,256,256,256,256,256,256} , //total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_gate_delay = {192,192,192,192,192,192,192,192} , //total 8
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64} , //total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_delay = {64,64,64,64,64,64,64,64} , //total 8
	.cfg_ddr_training_delay_ps[1]. read_dq_bit_delay = {0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0} , //total 72
	.cfg_ddr_training_delay_ps[1]. soc_bit_vref = {40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40} , //total 44
	.cfg_ddr_training_delay_ps[1]. dram_bit_vref = {32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32} , //total 36
	.cfg_ddr_training_delay_ps[1].	reserve_training_parameter	=	{0}	,
},
#endif //end for dd3 pxp

#if 0  //timing_config,for ddr4 pxp remap
//ddr_set_t  ddr_set_t_default=
{
	.cfg_board_common_setting.	timming_magic	=	0	,
	.cfg_board_common_setting.	timming_max_valid_configs	=	sizeof(ddr_set_t_default)/sizeof(ddr_set_t)	,
	.cfg_board_common_setting.	timming_struct_version	=	0	,
	.cfg_board_common_setting.	timming_struct_org_size	=	sizeof(ddr_set_t)	,
	.cfg_board_common_setting.	timming_struct_real_size	=	0	,	//0
	.cfg_board_common_setting.	fast_boot	=	{0}	,
	.cfg_board_common_setting.	ddr_func	=	0	,
	.cfg_board_common_setting.	board_id	=	CONFIG_BOARD_ID_MASK	,
	.cfg_board_common_setting.	DramType	=	CONFIG_DDR_TYPE_DDR4	,
	.cfg_board_common_setting.	dram_rank_config	=	CONFIG_DDR0_32BIT_RANK0_CH0	,
	.cfg_board_common_setting.	DisabledDbyte	=	CONFIG_DISABLE_D32_D63	,
	.cfg_board_common_setting.	dram_cs0_base_add	=	0	,
	.cfg_board_common_setting.	dram_cs1_base_add	=	0	,
	.cfg_board_common_setting.	dram_cs0_size_MB	=	CONFIG_DDR0_SIZE_1024MB	,
	.cfg_board_common_setting.	dram_cs1_size_MB	=	CONFIG_DDR1_SIZE_0MB	,
	.cfg_board_common_setting.	dram_x4x8x16_mode	=	CONFIG_DRAM_MODE_X16	,
	.cfg_board_common_setting.	Is2Ttiming	=	CONFIG_USE_DDR_2T_MODE	,
	.cfg_board_common_setting.	log_level	=	LOG_LEVEL_BASIC	,
	.cfg_board_common_setting.	ddr_rdbi_wr_enable	=	DDR_WRITE_READ_DBI_DISABLE	,
	.cfg_board_common_setting.	pll_ssc_mode	=	DDR_PLL_SSC_DISABLE	,
	.cfg_board_common_setting.	org_tdqs2dq	=	0	,
	.cfg_board_common_setting.	reserve1_test_function	=	{0}	,
	.cfg_board_common_setting.	ddr_dmc_remap	=	DDR_DMC_REMAP_DDR4_32BIT	,
	#if 1  //af409
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	8,	27,	10,	2,	9,	21,	5,	14,	1,	0,	25,	13,	12,	4,	7,	22,	0,	0,	0,	0,	6,	3,	20,	0,	0,	15,	26,	11,
	},
	#endif
	#if 0  //af419
	.cfg_board_common_setting.	ac_pinmux	= {
	0,	0,	0,	1,	2,	3,
	27,	12,	21,	9,	8,	0,	14,	10,	6,	7,	20,	5,	22,	13,	15,	2,	0,	0,	0,	0,	11,	26,	4,	0,	0,	25,	3,	1,
	},
	#endif
	#if 0
	.cfg_board_common_setting.	ac_pinmux	=
	{
	0,	0,	0,	1,	2,	3,
	0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	0,	0,	0,	0,	20,	21,	22,	0,	0,	25,	26,	27,	0
	}	,
	#endif
	.cfg_board_common_setting.	ddr_dqs_swap	=	0	,
	#if 0
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	3,2,0,1,7,6,5,4,
	14,13,12,15,8,9,11,10,
	20,21,22,23,16,17,19,18,
	24,25,28,26,31,30,27,29,
	32,33,34,35
	}	,	 //d0-d31 dm0 dm1 dm2 dm3
	#endif
	#if 0
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	2,3,1,0,6,7,4,5,
	11,10,9,8,15,14,13,12,
	16,17,18,19,20,21,22,23,
	24,25,26,27,28,29,30,31,
	32,33,34,35
	}	,	 //d0-d31 dm0 dm1 dm2 dm3
	#endif
	#if 0
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	3,2,0,1,7,6,5,4,
	8,9,10,11,12,13,14,15,
	16,17,18,19,20,21,22,23,
	24,25,26,27,28,29,30,31,
	32,33,34,35
	}	,	 //d0-d31 dm0 dm1 dm2 dm3
	//1,0,3,2,5,4,7,6,
	#endif
	#if 0
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	2,7,1,5,3,6,0,4,
	10,11,15,13,14,9,8,12,
	24,29,26,31,25,28,27,30,
	23,18,21,19,22,17,20,16,
	32,33,34,35
	}	,	 //d0-d31 dm0 dm1 dm2 dm3
	#endif

	#if 1
	.cfg_board_common_setting.	ddr_dq_remap	=	{
	0,1,2,3,4,5,6,7,
	8,9,10,11,12,13,14,15,
	16,17,18,19,20,21,22,23,
	24,25,26,27,28,29,30,31,
	32,33,34,35
	}	,	 //d0-d31 dm0 dm1 dm2 dm3
	#endif
	.cfg_board_common_setting.	ddr_vddee_setting	=	{0}	,


	.cfg_board_SI_setting_ps[0].	DRAMFreq	=	667	,
	.cfg_board_SI_setting_ps[0].	PllBypassEn	=	0	,
	.cfg_board_SI_setting_ps[0].	training_SequenceCtrl	=	0	,
	.cfg_board_SI_setting_ps[0].	ddr_odt_config	=	DDR_DRAM_ODT_W_CS0_ODT0	,
	.cfg_board_SI_setting_ps[0].	clk_drv_ohm	=	DDR_SOC_AC_DRV_40_OHM	,
	.cfg_board_SI_setting_ps[0].	cs_drv_ohm	=	DDR_SOC_AC_DRV_40_OHM	,
	.cfg_board_SI_setting_ps[0].	ac_drv_ohm	=	DDR_SOC_AC_DRV_40_OHM	,
	.cfg_board_SI_setting_ps[0].	soc_data_drv_ohm_p	=	DDR_SOC_DATA_DRV_ODT_40_OHM	,
	.cfg_board_SI_setting_ps[0].	soc_data_drv_ohm_n	=	DDR_SOC_DATA_DRV_ODT_40_OHM	,
	.cfg_board_SI_setting_ps[0].	soc_data_odt_ohm_p	=	DDR_SOC_DATA_DRV_ODT_0_OHM	,
	.cfg_board_SI_setting_ps[0].	soc_data_odt_ohm_n	=	DDR_SOC_DATA_DRV_ODT_120_OHM	, //for pxp dram weak driver model
	.cfg_board_SI_setting_ps[0].	dram_data_drv_ohm	=	DDR_DRAM_DDR4_DRV_34_OHM	,
	.cfg_board_SI_setting_ps[0].	dram_data_odt_ohm	=	DDR_DRAM_DDR4_ODT_60_OHM	,
	.cfg_board_SI_setting_ps[0].	dram_data_wr_odt_ohm	=	DDR_DRAM_DDR_WR_ODT_0_OHM	,
	.cfg_board_SI_setting_ps[0].	dram_ac_odt_ohm	=	DDR_DRAM_DDR_AC_ODT_0_OHM	,
	.cfg_board_SI_setting_ps[0].	dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM	,
	.cfg_board_SI_setting_ps[0].	lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ	,
	.cfg_board_SI_setting_ps[0].	reserve2	=	0	,
	.cfg_board_SI_setting_ps[0].	vref_ac_permil 	=	0	,
	.cfg_board_SI_setting_ps[0].	vref_soc_data_permil 	=	0	,
	.cfg_board_SI_setting_ps[0].	vref_dram_data_permil	=	0	,
	.cfg_board_SI_setting_ps[0].	max_core_timmming_frequency	=	0	,
	.cfg_board_SI_setting_ps[0].	training_phase_parameter	=	{0}	,
	.cfg_board_SI_setting_ps[0]. ac_trace_delay_org = {
	128,128,128,128,128,128,128,128,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	  } , //total 36
	  #if 0
	.cfg_board_SI_setting_ps[0]. ac_trace_delay_org = {
	123,124,125,126,127,128,128,128,
	129,128,127,126,125,124,123,122,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	  } , //total 36
	  #endif

	#if 1
	.cfg_ddr_training_delay_ps[0]. ac_trace_delay = {
	128,128,128,128,128,128,128,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,384,384,384,384,
	384,384,384,384,
	  } , //total 36
	  #endif
	  #if 0
	.cfg_ddr_training_delay_ps[0]. ac_trace_delay = {
	0,0,0,0,0,0,0,0, //0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,
	  } , //total 36
	  #endif
	#if 1
	.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {128,128,128,128,128,128,128,128} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,

	.cfg_ddr_training_delay_ps[0]. write_dq_bit_delay = {192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,
	192,192,192,192,192,192,192,192,} ,
	#endif
	#if 0
	.cfg_ddr_training_delay_ps[0]. write_dq_bit_delay = {10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,} ,
	#endif
	#if 0
	.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {0,0,0,0,0,0,0,0} ,
	.cfg_ddr_training_delay_ps[0]. write_dq_bit_delay = {64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64} ,
	//total 72
	#endif
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {256,256,256,256,256,256,256,256} , //total 8
	.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {0,0,0,0,0,0,0,0} , //total 8 //edge mode
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64} , //total 8
	.cfg_ddr_training_delay_ps[0]. read_dqs_delay = {64,64,64,64,64,64,64,64} , //total 8
	.cfg_ddr_training_delay_ps[0]. read_dq_bit_delay = {0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0} , //total 72
	.cfg_ddr_training_delay_ps[0]. soc_bit_vref = {0,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,50,40,50,40,50,40,50} , //total 44
	.cfg_ddr_training_delay_ps[0]. dram_bit_vref = {0,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32} , //total 36
	.cfg_ddr_training_delay_ps[0].	reserve_training_parameter	=	{0}	,
	//.cfg_ddr_training_delay_ps[0].reserve_training_parameter={((1<<7)|1),},

	.cfg_board_SI_setting_ps[1].	DRAMFreq	=	667	,
	.cfg_board_SI_setting_ps[1].	PllBypassEn	=	0	,
	.cfg_board_SI_setting_ps[1].	training_SequenceCtrl	=	0	,
	.cfg_board_SI_setting_ps[1].	ddr_odt_config	=	DDR_DRAM_ODT_W_CS0_ODT0	,
	.cfg_board_SI_setting_ps[1].	clk_drv_ohm	=	DDR_SOC_AC_DRV_40_OHM	,
	.cfg_board_SI_setting_ps[1].	cs_drv_ohm	=	DDR_SOC_AC_DRV_40_OHM	,
	.cfg_board_SI_setting_ps[1].	ac_drv_ohm	=	DDR_SOC_AC_DRV_40_OHM	,
	.cfg_board_SI_setting_ps[1].	soc_data_drv_ohm_p	=	DDR_SOC_DATA_DRV_ODT_40_OHM	,
	.cfg_board_SI_setting_ps[1].	soc_data_drv_ohm_n	=	DDR_SOC_DATA_DRV_ODT_40_OHM	,
	.cfg_board_SI_setting_ps[1].	soc_data_odt_ohm_p	=	DDR_SOC_DATA_DRV_ODT_60_OHM	,
	.cfg_board_SI_setting_ps[1].	soc_data_odt_ohm_n	=	DDR_SOC_DATA_DRV_ODT_0_OHM	,
	.cfg_board_SI_setting_ps[1].	dram_data_drv_ohm	=	DDR_DRAM_DDR4_DRV_34_OHM	,
	.cfg_board_SI_setting_ps[1].	dram_data_odt_ohm	=	DDR_DRAM_DDR4_ODT_60_OHM	,
	.cfg_board_SI_setting_ps[1].	dram_data_wr_odt_ohm	=	DDR_DRAM_DDR_WR_ODT_0_OHM	,
	.cfg_board_SI_setting_ps[1].	dram_ac_odt_ohm	=	DDR_DRAM_DDR_AC_ODT_0_OHM	,
	.cfg_board_SI_setting_ps[1].	dram_data_drv_pull_up_calibration_ohm	=	DDR_DRAM_LPDDR4_ODT_40_OHM	,
	.cfg_board_SI_setting_ps[1].	lpddr4_dram_vout_voltage_range_setting	=	DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ	,
	.cfg_board_SI_setting_ps[1].	reserve2	=	0	,
	.cfg_board_SI_setting_ps[1].	vref_ac_permil 	=	0	,
	.cfg_board_SI_setting_ps[1].	vref_soc_data_permil 	=	0	,
	.cfg_board_SI_setting_ps[1].	vref_dram_data_permil	=	0	,
	.cfg_board_SI_setting_ps[1].	max_core_timmming_frequency	=	0	,
	.cfg_board_SI_setting_ps[1].	training_phase_parameter	=	{0}	,
	.cfg_board_SI_setting_ps[1]. ac_trace_delay_org = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	  } , //total 36
	  #if 0
	.cfg_board_SI_setting_ps[1]. ac_trace_delay_org = {
	123,124,125,126,127,128,128,128,
	129,128,127,126,125,124,123,122,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	  } , //total 36
	  #endif

	#if 1
	.cfg_ddr_training_delay_ps[1]. ac_trace_delay = {
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,
	128,128,128,128,
	  } , //total 36
	  #endif
	  #if 0
	.cfg_ddr_training_delay_ps[1]. ac_trace_delay = {
	0,0,0,0,0,0,0,0, //0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,
	  } , //total 36
	  #endif
	.cfg_ddr_training_delay_ps[1]. write_dqs_delay = {0,0,0,0,0,0,0,0} ,
	//.cfg_ddr_training_delay_ps[0]. write_dqs_delay = {32,32,32,32,32,32,32,32} ,
	#if 1
	.cfg_ddr_training_delay_ps[1]. write_dq_bit_delay = {50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,
	50,50,50,50,50,50,50,50,} ,
	#endif
	#if 0
	.cfg_ddr_training_delay_ps[1]. write_dq_bit_delay = {10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,
	10,10,10,10,10,10,10,10,} ,
	#endif
	#if 0
	.cfg_ddr_training_delay_ps[1]. write_dq_bit_delay = {64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64} ,
	//total 72
	#endif
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {256,256,256,256,256,256,256,256} , //total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_gate_delay = {192,192,192,192,192,192,192,192} , //total 8
	//.cfg_ddr_training_delay_ps[0]. read_dqs_gate_delay = {64,64,64,64,64,64,64,64} , //total 8
	.cfg_ddr_training_delay_ps[1]. read_dqs_delay = {64,64,64,64,64,64,64,64} , //total 8
	.cfg_ddr_training_delay_ps[1]. read_dq_bit_delay = {0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0} , //total 72
	.cfg_ddr_training_delay_ps[1]. soc_bit_vref = {40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40,
	40,40,40,40,40,40,40,40,40,40,40} , //total 44
	.cfg_ddr_training_delay_ps[1]. dram_bit_vref = {32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
	32,32,32,32,32,32,32,32,32,32,32,
	32,32,32} , //total 36
	.cfg_ddr_training_delay_ps[1].	reserve_training_parameter	=	{0}	,
},
#endif //end for ddr4 pxp remap
};
//ddr timing end

pll_set_t __pll_setting = {
	.cpu_clk				= CPU_CLK / 24 * 24,
#ifdef CONFIG_PXP_EMULATOR
	.pxp					= 1,
#else
	.pxp					= 0,
#endif
	.spi_ctrl				= 0,
	.lCustomerID			= AML_CUSTOMER_ID,
	.log_chl				= 0x3, /* 0x77: all channel enable. 0xFF: with stream info */
	.log_ctrl				= (1<<7) | /* (1<<7), print bl2 log into buffer */
#ifdef CONFIG_SILENT_CONSOLE
							  (1<<6),    /* do not print log buffer */
#else
							  (0<<6),    /* print log buffer before run bl31 */
#endif
.ddr_timming_save_mode 		= 1,
};

ddr_reg_t __ddr_reg[] = {
	/* demo, user defined override register */
	{0xaabbccdd, 0, 0, 0, 0, 0},
	{0x11223344, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0},
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

/* VDDEE_VAL_REG: VDDEE PWM table  0.69v-0.89v*/
#if    (VDDEE_VAL == 711)
	#define VDDEE_VAL_REG	0x120000
#elif (VDDEE_VAL == 721)
	#define VDDEE_VAL_REG	0x110001
#elif (VDDEE_VAL == 731)
	#define VDDEE_VAL_REG	0x100002
#elif (VDDEE_VAL == 741)
	#define VDDEE_VAL_REG	0xf0003
#elif (VDDEE_VAL == 751)
	#define VDDEE_VAL_REG	0xe0004
#elif (VDDEE_VAL == 761)
	#define VDDEE_VAL_REG	0xd0005
#elif (VDDEE_VAL == 771)
	#define VDDEE_VAL_REG	0xc0006
#elif (VDDEE_VAL == 781)
	#define VDDEE_VAL_REG	0xb0007
#elif (VDDEE_VAL == 791)
	#define VDDEE_VAL_REG	0xa0008
#elif (VDDEE_VAL == 801)
	#define VDDEE_VAL_REG	0x90009
#elif (VDDEE_VAL == 811)
	#define VDDEE_VAL_REG	0x8000a
#elif (VDDEE_VAL == 821)
	#define VDDEE_VAL_REG	0x7000b
#elif (VDDEE_VAL == 831)
	#define VDDEE_VAL_REG	0x6000c
#elif (VDDEE_VAL == 841)
	#define VDDEE_VAL_REG	0x5000d
#elif (VDDEE_VAL == 851)
	#define VDDEE_VAL_REG	0x4000e
#elif (VDDEE_VAL == 861)
	#define VDDEE_VAL_REG	0x3000f
#elif (VDDEE_VAL == 871)
	#define VDDEE_VAL_REG	0x20010
#elif (VDDEE_VAL == 881)
	#define VDDEE_VAL_REG	0x10011
#elif (VDDEE_VAL == 891)
	#define VDDEE_VAL_REG	0x12
#else
	#error "VDDEE val out of range\n"
#endif

/* for PWM use */
/*
 * GPIOE_0   PWM_A    VDDEE_PWM
 *
 * GPIOE_1   PWM_B    VCCK_PWM
 */
/* PWM driver check http://scgit.amlogic.com:8080/#/c/38093/ */
#define PADCTRL_PIN_MUX_REG2   	((0x0002  << 2) + 0xfe000400)
#define PADCTRL_GPIOE_DS        ((0x0035  << 2) + 0xfe000400)

/* pwm clock and pwm controller */
#define CLKTREE_PWM_CLK_AB_CTRL ((0x0031  << 2) + 0xfe000800)
#define PWMAB_PWM_A            	((0x0000  << 2) + 0xfe002400)
#define PWMAB_PWM_B             ((0x0001  << 2) + 0xfe002400)
#define PWMAB_MISC_REG_AB       ((0x0002  << 2) + 0xfe002400)

bl2_reg_t __bl2_reg[] = {
	/* demo, user defined override register */
	{0,			0,            		0xffffffff,   0, 0, 0},
#ifdef CONFIG_PDVFS_ENABLE
	{PWMAB_PWM_A, 0x000c0006, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_1, 0},
	{PWMAB_PWM_A, 0x00090009, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_2, 0},
	{PWMAB_PWM_A, 0x0007000b, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_3, 0},
#else
	{PWMAB_PWM_A, VDDEE_VAL_REG, 0xffffffff, 0, BL2_INIT_STAGE_1, 0},
#endif
	{PWMAB_PWM_B,		VCCK_VAL_REG,  		0xffffffff,	0, BL2_INIT_STAGE_1, 0},
	{PWMAB_MISC_REG_AB,	0x3 << 0, 		0x3, 		0, BL2_INIT_STAGE_1, 0},
	/* enable vddcpu dc-dc, set GPIOD_10 high */
	{PADCTRL_GPIOD_OEN,	0x0 << 10, 		0x1 << 10, 	0, BL2_INIT_STAGE_1, 0},
	{PADCTRL_GPIOD_O,	0x1 << 10, 		0x1 << 10, 	0, BL2_INIT_STAGE_1, 0},
	/* set pwm A and pwm B clock rate to 24M, enable them */
	{CLKTREE_PWM_CLK_AB_CTRL,1 << 8 | 1 << 24 , 	0xffffffff, 	0, BL2_INIT_STAGE_1, 0},
	/* set GPIOE_0 GPIOE_1 drive strength to 3*/
	{PADCTRL_GPIOE_DS,	0xf, 			0xf,		0, BL2_INIT_STAGE_1, 0},
	{PADCTRL_PIN_MUX_REG2,	0x3,			0xf,		0, BL2_INIT_STAGE_1, 0},
	{PADCTRL_PIN_MUX_REG2,	0x3 << 4,		0xf << 4,	0, BL2_INIT_STAGE_1, 0},
};
