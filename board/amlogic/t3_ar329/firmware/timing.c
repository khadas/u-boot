
/*
 * board/amlogic/t3_ar329/firmware/timing.c
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
#define DSU_CLK					1200

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
 *		#define 	CONFIG_DDR0_32BIT_RANK0_16BIT_RANK1_LOW_CH0		0x9
 *		#define 	CONFIG_DDR0_32BIT_RANK0_16BIT_RANK1_HIGH_CH0	0xa
 *		#define 	CONFIG_DDR0_32BIT_16BIT_RANK0_CH0_MODE2			0xB
 *		#define 	CONFIG_DDR0_32BIT_RANK0_16BIT_RANK1_LOW_CH0_MODE2	0xC

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

#define ENABLE_SKT_BOARD 1
//#define ENABLE_REF_BOARD 1

#if ENABLE_REF_BOARD	//timing_config,AR331
ddr_set_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {
//ddr_set_t ddr_set_t_default[2] ={
{
	.cfg_board_common_setting.timming_magic								= 0,
	.cfg_board_common_setting.timming_max_valid_configs						= 0,//sizeof(__ddr_setting) / sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_version						= 0,
	.cfg_board_common_setting.timming_struct_org_size						= sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_real_size						= 0,                                      //0
	.cfg_board_common_setting.fast_boot								= { 0 },
	//.cfg_board_common_setting.ddr_func								= DDR_FUNC_CONFIG_DFE_FUNCTION,
	.cfg_board_common_setting.board_id								= CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.DramType								= CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.dram_rank_config							= CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.DisabledDbyte								= CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.dram_cs0_base_add							= 0,
	.cfg_board_common_setting.dram_cs1_base_add							= 0,
	//.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,
	//.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_x4x8x16_mode							= CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.Is2Ttiming								= CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.log_level								= LOG_LEVEL_BASIC,
	.cfg_board_common_setting.ddr_rdbi_wr_enable							= DDR_WRITE_READ_DBI_DISABLE,             //DDR_WRITE_READ_DBI_ENABLE,//DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.pll_ssc_mode								= DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.org_tdqs2dq								= 0,
	.cfg_board_common_setting.reserve1_test_function						= { 0 },
	.cfg_board_common_setting.ddr_dmc_remap								= DDR_DMC_REMAP_DDR4_32BIT,
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0,  0,	0,  1,	2,  3,	8, 27,
		10, 2,	9,  21, 5,  14, 1, 0,
		25, 13, 12, 4,	7,  22, 0, 0,
		0,  0,	6,  3,	20, 0,	0, 15,
		26, 11,
	},
#endif
#if 0
	//T3 sip ac pinmux
	.cfg_board_common_setting.ac_pinmux		=
	{//sip
		0,	0,	0,	1,	2,	3,
		8,	27,	10,	2,	9,	21,	5,	23,	1,	0,	25,	13,	12,	4,	7,	28,	0,	0,	0,	0,	6,	3,	20,	0,	0,	24,	26,	11,	0
	},
#endif
#if 1
	.cfg_board_common_setting.ac_pinmux		=
	{//test chip
		0,	0,	0,	1,	2,	3,
		3,	28,	20,	5,	10,	13,	11,	8,	0,	12,	27,	9,	1,	6,	4,	25,	0,	0,	0,	0,	2,	7,	23,	0,	0,	24,	26,	21,	0
	},
#endif

#if 0
	//SIP package
	//2.1 - T963(17.45X16 .8)
	//4DDR4 (NTC)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		7, 28, 20, 9, 2, 21,6,	10, 0, 1, 27, 11, 12, 4, 8, 25, 0, 0, 0, 0, 5, 3, 23, 0, 0, 24, 26, 13,
	};

	//4DDR4 (SAMSUNG)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		8, 27, 10, 2, 9, 21,5,	23, 1, 0, 25, 13, 12, 4, 7, 28, 0, 0, 0, 0, 6, 3, 20, 0, 0, 24, 26, 11,
	};
#endif
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0, 0, 0, 1, 2, 3,
		0, 1, 2, 3, 4, 5,6,  7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 20, 21, 22, 0, 0, 25, 26, 27,
	},
#endif
	.cfg_board_common_setting.ddr_dqs_swap								= 0,
	.cfg_board_common_setting.ddr_dq_remap								=
	{
		0,  1,	2,  3,	4,  5,	6,  7,
		8,  9,	10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35
	},                                                           //d0-d31 dm0 dm1 dm2 dm3

	.cfg_board_common_setting.ddr_vddee_setting							= { 0 },
	.cfg_board_SI_setting_ps[0].DRAMFreq = 1008,//skt
	.cfg_board_SI_setting_ps[0].PllBypassEn = 0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl = 0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,                                                        //DDR_DRAM_ODT_W_CS0_ODT0,//DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,//DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                                //for pxp dram weak driver model
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_60_OHM,
	//.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_240_OHM,

#if 0
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,                                                       //DDR_DRAM_ODT_W_CS0_ODT0,//DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,//DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                                //for pxp dram weak driver model
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_80_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_0_OHM,
#endif
	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting = DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].dfe_offset = 4,
	.cfg_board_SI_setting_ps[0].vref_ac_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil = 0,
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency = 0,                                                            //0,
	.cfg_board_SI_setting_ps[0].training_phase_parameter = { 0 },
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org =
	{
		128, 128, 128 , 128, 128, 128, 128, 128,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384,
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[0].ac_trace_delay =
	{
		128, 128, 128 , 128, 128, 128, 128, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384,
	},

#if 0           //fail delay;
	.cfg_ddr_training_delay_ps[0].ac_trace_delay =
	{
		128, 128, 128, 128, 128, 128, 128, 384,
		384, 384, 384, 384, 384, 384, 384, 384,
		384, 384, 384, 384, 384, 384, 384, 384,
		384, 384, 384, 384, 384, 384, 384, 384,
		384, 384, 384, 384,
	},                                                     //total 36
#endif
	//.cfg_ddr_training_delay_ps[0].reserve_training_parameter = {0,0,0,0,0,0,0,0,0+0,0+0,0+0,0+0},
	.cfg_ddr_training_delay_ps[0].reserve_training_parameter = {0,0,0,0,0,0,0,0,0x80+0x10,0x80+0x10,0x80+0x10,0x80+0x10},
	//.cfg_ddr_training_delay_ps[0].reserve_training_parameter = {0xa,0xa,0xa,0xa,0,0,0,0,0x80+0x18,0x80+0x18,0x80+0x18,0x80+0x18},

	.cfg_board_SI_setting_ps[0].vref_soc_data_permil =0x000002a8,// 680
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
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[2]=0x00000080-60,// 88
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[3]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[4]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[5]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[6]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[7]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[8]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[9]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[10]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[11]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[12]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[13]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[14]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[15]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[16]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[17]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[18]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[19]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[20]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[21]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[22]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[23]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[24]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[25]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[26]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[27]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[28]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[29]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[30]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[31]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[32]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[33]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[34]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[35]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=0x000000b9,// 185
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=0x000000a5,// 165
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=0x000000d7,// 215
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=0x000000cd,// 205
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000102,// 258
#if 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=0x000000f1,// 241
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=0x000000f7,// 247
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=0x000000e8,// 232
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=0x000000f1,// 241
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=0x000000fb,// 251
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=0x000000ea,// 234
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=0x000000f2,// 242
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=0x000000e7,// 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=0x000000e2,// 226
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=0x000000e5,// 229
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=0x000000de,// 222
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=0x000000e7,// 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=0x000000ed,// 237
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=0x000000e8,// 232
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=0x000000e0,// 224
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=0x000000e4,// 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=0x00000112,// 274
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=0x0000010b,// 267
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=0x00000117,// 279
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=0x00000109,// 265
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=0x00000110,// 272
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=0x00000107,// 263
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=0x00000117,// 279
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=0x00000111,// 273
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=0x0000010e,// 270
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=0x00000112,// 274
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=0x0000010a,// 266
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=0x00000115,// 277
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=0x00000106,// 262
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=0x0000011a,// 282
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=0x0000010a,// 266
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=0x00000117,// 279
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=0x0000010a,// 266
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=0x0000010f,// 271
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
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=0x00000299,// 665
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=0x000002ad,// 685
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=0x000002b5,// 693
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=0x000002be,// 702
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=0x00000090,// 144
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=0x00000083,// 131
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=0x00000095,// 149
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=0x00000087,// 135
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=0x00000043,// 67
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=0x0000003b,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=0x00000043,// 67
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=0x00000052,// 82
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=0x0000003b,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=0x00000042,// 66
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=0x0000003a,// 58
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=0x00000040,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=0x0000003b,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=0x00000040,// 64
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=0x0000003c,// 60
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=0x00000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=0x0000003b,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=0x0000004f,// 79
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=0x00000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=0x0000003b,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=0x00000050,// 80
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=0x00000051,// 81
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=0x00000042,// 66
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
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=216,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=225,//0x000000db,// 219
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=216,//0x000000e7,// 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=225,//0x000000e3,// 227
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=216,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=225,//0x000000c9,// 201
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=216,//0x000000e6,// 230
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=225,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=208,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=208,//0x0000010b,// 267
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=208,//0x000000e8,// 232
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=208,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=208,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=208,//0x000000f7,// 247
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=208,//0x000000f4,// 244
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=208,//0x00000104,// 260
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=221,//0x000000fc,// 252
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=210,//0x000000fa,// 250
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=221,//0x000000e0,// 224
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=210,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=221,//0x000000e4,// 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=221,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=215,//0x000000f0,// 240
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=215,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=215,//0x000000ce,// 206
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=224,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=215,//0x000000de,// 222
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=224,//0x000000ef,// 239
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=220,//0x000000d8,// 216
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=210,//0x000000df,// 223
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=216,//0x000000d9,// 217
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=217,//0x000000e0,// 224
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
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=545,// 854
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=567,// 855
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=568,//0x0000033e,// 830
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=557,//0x00000348,// 840
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=132,//0x00000086,// 134
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=132,//0x0000007f,// 127
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=132,//0x0000006f,// 111
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=132,//0x00000066,// 102
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=57,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=68,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=70,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=59,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=71,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=62,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=68,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=64,//0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=58,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=67,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=56,//0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=65,//0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=59,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=78,//0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=60,//0x00000058,// 88
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=68,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=64,//0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=67,//0x00000034,// 52
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=55,//0x0064000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=72,//0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=61,//0x00000057,// 87
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=70,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=58,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=67,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=64,//0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=57,//0x00000061,// 97
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=62,//0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=58,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=66,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=61,//0x0000005c,// 92
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=65,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=58,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=60,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=60,//0x0000004a,// 74
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
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[0]=0x0000000,// 40
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
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[0]=0x0,// 0
},
{
	.cfg_board_common_setting.timming_magic								= 0,
	.cfg_board_common_setting.timming_max_valid_configs						= 0,//sizeof(__ddr_setting) / sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_version						= 0,
	.cfg_board_common_setting.timming_struct_org_size						= sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_real_size						= 0,                                      //0
	.cfg_board_common_setting.fast_boot								= { 0 },
	//.cfg_board_common_setting.ddr_func								= DDR_FUNC_CONFIG_DFE_FUNCTION,
	.cfg_board_common_setting.board_id								= CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.DramType								= CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.dram_rank_config							= CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.DisabledDbyte								= CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.dram_cs0_base_add							= 0,
	.cfg_board_common_setting.dram_cs1_base_add							= 0,
	//.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,
	//.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_x4x8x16_mode							= CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.Is2Ttiming								= CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.log_level								= LOG_LEVEL_BASIC,
	.cfg_board_common_setting.ddr_rdbi_wr_enable							= DDR_WRITE_READ_DBI_DISABLE,             //DDR_WRITE_READ_DBI_ENABLE,//DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.pll_ssc_mode								= DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.org_tdqs2dq								= 0,
	.cfg_board_common_setting.reserve1_test_function						= { 0 },
	.cfg_board_common_setting.ddr_dmc_remap								= DDR_DMC_REMAP_DDR4_32BIT,
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0,  0,	0,  1,	2,  3,	8, 27,
		10, 2,	9,  21, 5,  14, 1, 0,
		25, 13, 12, 4,	7,  22, 0, 0,
		0,  0,	6,  3,	20, 0,	0, 15,
		26, 11,
	},
#endif
#if 0
	//T3 sip ac pinmux
	.cfg_board_common_setting.ac_pinmux		=
	{//sip
		0,	0,	0,	1,	2,	3,
		8,	27,	10,	2,	9,	21,	5,	23,	1,	0,	25,	13,	12,	4,	7,	28,	0,	0,	0,	0,	6,	3,	20,	0,	0,	24,	26,	11,	0
	},
#endif
#if 1  //test chip
	.cfg_board_common_setting.ac_pinmux		=
	{//test chip
		0,	0,	0,	1,	2,	3,
		5,	28,	0,	26,	1,	20,	3,	10,	9,	13,	8,	25,	27,	2,	12,	6,	0,	0,	0,	0,	4,	7,	11,	0,	0,	24,	23,	21,	0
	},
#endif

#if 0
	//SIP package
	//2.1 - T963(17.45X16 .8)
	//4DDR4 (NTC)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		7, 28, 20, 9, 2, 21,6,	10, 0, 1, 27, 11, 12, 4, 8, 25, 0, 0, 0, 0, 5, 3, 23, 0, 0, 24, 26, 13,
	};

	//4DDR4 (SAMSUNG)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		8, 27, 10, 2, 9, 21,5,	23, 1, 0, 25, 13, 12, 4, 7, 28, 0, 0, 0, 0, 6, 3, 20, 0, 0, 24, 26, 11,
	};
#endif
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0, 0, 0, 1, 2, 3,
		0, 1, 2, 3, 4, 5,6,  7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 20, 21, 22, 0, 0, 25, 26, 27,
	},
#endif
	.cfg_board_common_setting.ddr_dqs_swap								= 0,
	.cfg_board_common_setting.ddr_dq_remap								=
	{
		0,  1,	2,  3,	4,  5,	6,  7,
		8,  9,	10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35
	},                                                           //d0-d31 dm0 dm1 dm2 dm3

	.cfg_board_common_setting.ddr_vddee_setting							= { 0 },
	.cfg_board_SI_setting_ps[0].DRAMFreq = 1008,
	.cfg_board_SI_setting_ps[0].PllBypassEn = 0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl = 0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,                                                        //DDR_DRAM_ODT_W_CS0_ODT0,//DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,//DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                                //for pxp dram weak driver model
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_60_OHM,
	//.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_240_OHM,

	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting = DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].dfe_offset = 4,
	.cfg_board_SI_setting_ps[0].vref_ac_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil = 0,
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency = 0,                                                            //0,
	.cfg_board_SI_setting_ps[0].training_phase_parameter = { 0 },
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org =
	{
		128, 128, 128 , 128, 128, 128, 128, 128,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384,
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[0].ac_trace_delay =
	{
		128, 128, 128 , 128, 128, 128, 128, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384,
	},
	//.cfg_ddr_training_delay_ps[0].reserve_training_parameter = {0,0,0,0,0,0,0,0,0+0,0+0,0+0,0+0},
	.cfg_ddr_training_delay_ps[0].reserve_training_parameter = {0,0,0,0,0,0,0,0,0x80+0x10,0x80+0x10,0x80+0x10,0x80+0x10},
	//.cfg_ddr_training_delay_ps[0].reserve_training_parameter = {0xa,0xa,0xa,0xa,0,0,0,0,0x80+0x18,0x80+0x18,0x80+0x18,0x80+0x18},

	.cfg_board_SI_setting_ps[0].vref_soc_data_permil =0x000002a8,// 680
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
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[2]=0x00000080-60,// 88
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[3]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[4]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[5]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[6]=0x00000080,// 128
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[7]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[8]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[9]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[10]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[11]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[12]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[13]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[14]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[15]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[16]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[17]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[18]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[19]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[20]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[21]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[22]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[23]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[24]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[25]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[26]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[27]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[28]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[29]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[30]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[31]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[32]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[33]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[34]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[35]=0x00000180-40,// 384
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=128,//0x000000b0,// 176
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=128,//0x000000c4,// 196
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=160,//0x000000c7,// 199
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=160,//0x000000a7,// 167
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000102,// 258

	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=0x000000af,// 175
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=0x000000af,// 175
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=0x000000af,// 175
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=0x000000b9,// 185
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000102,// 258
#if 0
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=0x000000fd,// 253
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=0x000000e7,// 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=0x000000f6,// 246
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=0x000000e8,// 232
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=0x000000ef,// 239
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=0x000000f2,// 242
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=0x000000f0,// 240
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=0x000000e7,// 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=0x000000f4,// 244
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=0x000000e8,// 232
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=0x000000e4,// 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=0x000000f0,// 240
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=0x000000eb,// 235
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=0x000000f8,// 248
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=0x000000ed,// 237
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=0x00000101,// 257
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=0x000000ee,// 238
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=0x000000eb,// 235
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=0x000000eb,// 235
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=0x000000de,// 222
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=0x00000106,// 262
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=0x000000f1,// 241
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=0x00000106,// 262
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=0x000000fb,// 251
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=0x000000fc,// 252
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=0x000000e8,// 232
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=0x000000f7,// 247
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=0x000000ea,// 234
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=0x000000f6,// 246
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
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=0x0000028f,// 655
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=0x00000290,// 656
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=0x000002bb,// 699
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=0x000002b4,// 692
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=0x00000088,// 136
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=0x00000086,// 134
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=0x00000091,// 145
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=0x0000008c,// 140
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=0x00000037,// 55
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=0x00000051,// 81
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=0x00000043,// 67
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=0x0000003d,// 61
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=0x00000034,// 52
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=0x0000004a,// 74
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=0x0000003b,// 59
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=0x0000003e,// 62
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=0x00000050,// 80
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=0x00000043,// 67
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=0x0000003e,// 62
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=0x00000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=0x00000044,// 68
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=0x00000051,// 81
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=0x00000037,// 55
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=0x00000053,// 83
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=0x00000037,// 55
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=0x00000043,// 67
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
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=216,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=225,//0x000000db,// 219
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=216,//0x000000e7,// 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=225,//0x000000e3,// 227
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=216,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=225,//0x000000c9,// 201
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=216,//0x000000e6,// 230
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=225,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=208,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=208,//0x0000010b,// 267
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=208,//0x000000e8,// 232
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=208,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=208,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=208,//0x000000f7,// 247
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=208,//0x000000f4,// 244
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=208,//0x00000104,// 260
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=221,//0x000000fc,// 252
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=210,//0x000000fa,// 250
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=221,//0x000000e0,// 224
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=210,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=221,//0x000000e4,// 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=221,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=215,//0x000000f0,// 240
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=215,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=215,//0x000000ce,// 206
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=224,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=215,//0x000000de,// 222
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=224,//0x000000ef,// 239
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=220,//0x000000d8,// 216
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=210,//0x000000df,// 223
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=216,//0x000000d9,// 217
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=217,//0x000000e0,// 224
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
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=545,// 854
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=567,// 855
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=568,//0x0000033e,// 830
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=557,//0x00000348,// 840
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=132,//0x00000086,// 134
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=132,//0x0000007f,// 127
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=132,//0x0000006f,// 111
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=132,//0x00000066,// 102
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=57,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=68,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=70,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=59,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=71,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=62,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=68,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=64,//0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=58,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=67,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=56,//0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=65,//0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=59,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=78,//0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=60,//0x00000058,// 88
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=68,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=64,//0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=67,//0x00000034,// 52
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=55,//0x0064000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=72,//0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=61,//0x00000057,// 87
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=70,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=58,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=67,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=64,//0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=57,//0x00000061,// 97
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=62,//0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=58,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=66,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=61,//0x0000005c,// 92
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=65,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=58,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=60,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=60,//0x0000004a,// 74
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
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[0]=0x0000000,// 40
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
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[0]=0x0,// 0
},
};
#endif	//ENABLE_REF_BOARD
#if 0	//timing_config,
ddr_set_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {
//ddr_set_t ddr_set_t_default[2] ={
{
	.cfg_board_common_setting.timming_magic								= 0,
	.cfg_board_common_setting.timming_max_valid_configs						= 0,//sizeof(__ddr_setting) / sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_version						= 0,
	.cfg_board_common_setting.timming_struct_org_size						= sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_real_size						= 0,                                      //0
	.cfg_board_common_setting.fast_boot								= { 0 },
	.cfg_board_common_setting.ddr_func								= DDR_FUNC_CONFIG_DFE_FUNCTION,
	.cfg_board_common_setting.board_id								= CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.DramType								= CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.dram_rank_config							= CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.DisabledDbyte								= CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.dram_cs0_base_add							= 0,
	.cfg_board_common_setting.dram_cs1_base_add							= 0,
	//.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,
	//.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_x4x8x16_mode							= CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.Is2Ttiming								= CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.log_level								= LOG_LEVEL_BASIC,
	.cfg_board_common_setting.ddr_rdbi_wr_enable							= DDR_WRITE_READ_DBI_DISABLE,             //DDR_WRITE_READ_DBI_ENABLE,//DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.pll_ssc_mode								= DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.org_tdqs2dq								= 0,
	.cfg_board_common_setting.reserve1_test_function						= { 0 },
	.cfg_board_common_setting.ddr_dmc_remap								= DDR_DMC_REMAP_DDR4_32BIT,
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0,  0,	0,  1,	2,  3,	8, 27,
		10, 2,	9,  21, 5,  14, 1, 0,
		25, 13, 12, 4,	7,  22, 0, 0,
		0,  0,	6,  3,	20, 0,	0, 15,
		26, 11,
	},
#endif
#if 1
	//T3 sip ac pinmux
	.cfg_board_common_setting.ac_pinmux		=
	{//sip
		0,	0,	0,	1,	2,	3,
		8,	27,	10,	2,	9,	21,	5,	23,	1,	0,	25,	13,	12,	4,	7,	28,	0,	0,	0,	0,	6,	3,	20,	0,	0,	24,	26,	11,	0
	},
#endif
#if 0  //test chip
	.cfg_board_common_setting.ac_pinmux		=
	{//test chip
		0,	0,	0,	1,	2,	3,
		3,	28,	20,	5,	10,	13,	11,	8,	0,	12,	27,	9,	1,	6,	4,	25,	0,	0,	0,	0,	2,	7,	23,	0,	0,	24,	26,	21,	0
	},
#endif

#if 0
	//SIP package
	//2.1 - T963(17.45X16 .8)
	//4DDR4 (NTC)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		7, 28, 20, 9, 2, 21,6,	10, 0, 1, 27, 11, 12, 4, 8, 25, 0, 0, 0, 0, 5, 3, 23, 0, 0, 24, 26, 13,
	};

	//4DDR4 (SAMSUNG)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		8, 27, 10, 2, 9, 21,5,	23, 1, 0, 25, 13, 12, 4, 7, 28, 0, 0, 0, 0, 6, 3, 20, 0, 0, 24, 26, 11,
	};
#endif
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0, 0, 0, 1, 2, 3,
		0, 1, 2, 3, 4, 5,6,  7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 20, 21, 22, 0, 0, 25, 26, 27,
	},
#endif
	.cfg_board_common_setting.ddr_dqs_swap								= 0,
	.cfg_board_common_setting.ddr_dq_remap								=
	{
		0,  1,	2,  3,	4,  5,	6,  7,
		8,  9,	10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35
	},                                                           //d0-d31 dm0 dm1 dm2 dm3

	.cfg_board_common_setting.ddr_vddee_setting							= { 0 },
	.cfg_board_SI_setting_ps[0].DRAMFreq = 1320,
	.cfg_board_SI_setting_ps[0].PllBypassEn = 0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl = 0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,                                                        //DDR_DRAM_ODT_W_CS0_ODT0,//DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,//DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                                //for pxp dram weak driver model
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_60_OHM,
	//.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_240_OHM,

#if 0
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,                                                       //DDR_DRAM_ODT_W_CS0_ODT0,//DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,//DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                                //for pxp dram weak driver model
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_80_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_0_OHM,
#endif
	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting = DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].dfe_offset = 4,
	.cfg_board_SI_setting_ps[0].vref_ac_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil = 0,
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency = 0,                                                            //0,
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

#if 0           //fail delay;
	.cfg_ddr_training_delay_ps[0].ac_trace_delay =
	{
		128, 128, 128, 128, 128, 128, 128, 384,
		384, 384, 384, 384, 384, 384, 384, 384,
		384, 384, 384, 384, 384, 384, 384, 384,
		384, 384, 384, 384, 384, 384, 384, 384,
		384, 384, 384, 384,
	},                                                     //total 36
#endif

	.cfg_ddr_training_delay_ps[0].reserve_training_parameter = {0xa,0xa,0xa,0xa,0,0,0,0,0x80+0x18,0x80+0x18,0x80+0x18,0x80+0x18},

	.cfg_board_SI_setting_ps[0].vref_soc_data_permil =0x000002a8,// 680
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
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=160,//0x000000b0,// 176
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=145,//0x000000c4,// 196
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=160,//0x000000c7,// 199
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=160,//0x000000a7,// 167
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=216,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=225,//0x000000db,// 219
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=216,//0x000000e7,// 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=225,//0x000000e3,// 227
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=216,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=225,//0x000000c9,// 201
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=216,//0x000000e6,// 230
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=225,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=208,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=208,//0x0000010b,// 267
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=208,//0x000000e8,// 232
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=208,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=208,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=208,//0x000000f7,// 247
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=208,//0x000000f4,// 244
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=208,//0x00000104,// 260
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=221,//0x000000fc,// 252
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=210,//0x000000fa,// 250
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=221,//0x000000e0,// 224
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=210,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=221,//0x000000e4,// 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=221,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=215,//0x000000f0,// 240
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=215,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=215,//0x000000ce,// 206
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=224,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=215,//0x000000de,// 222
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=224,//0x000000ef,// 239
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=220,//0x000000d8,// 216
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=210,//0x000000df,// 223
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=216,//0x000000d9,// 217
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=217,//0x000000e0,// 224
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
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=545,// 854
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=567,// 855
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=568,//0x0000033e,// 830
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=557,//0x00000348,// 840
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=132,//0x00000086,// 134
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=132,//0x0000007f,// 127
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=132,//0x0000006f,// 111
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=132,//0x00000066,// 102
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=57,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=68,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=70,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=59,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=71,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=62,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=68,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=64,//0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=58,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=67,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=56,//0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=65,//0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=59,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=78,//0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=60,//0x00000058,// 88
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=68,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=64,//0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=67,//0x00000034,// 52
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=55,//0x0064000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=72,//0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=61,//0x00000057,// 87
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=70,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=58,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=67,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=64,//0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=57,//0x00000061,// 97
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=62,//0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=58,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=66,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=61,//0x0000005c,// 92
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=65,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=58,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=60,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=60,//0x0000004a,// 74
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
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[0]=0x0000000,// 40
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
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[0]=0x0,// 0
},
{
	.cfg_board_common_setting.timming_magic								= 0,
	.cfg_board_common_setting.timming_max_valid_configs						= 0,//sizeof(__ddr_setting) / sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_version						= 0,
	.cfg_board_common_setting.timming_struct_org_size						= sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_real_size						= 0,                                      //0
	.cfg_board_common_setting.fast_boot								= { 0 },
	.cfg_board_common_setting.ddr_func								= DDR_FUNC_CONFIG_DFE_FUNCTION,
	.cfg_board_common_setting.board_id								= CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.DramType								= CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.dram_rank_config							= CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.DisabledDbyte								= CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.dram_cs0_base_add							= 0,
	.cfg_board_common_setting.dram_cs1_base_add							= 0,
	//.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,
	//.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_x4x8x16_mode							= CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.Is2Ttiming								= CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.log_level								= LOG_LEVEL_BASIC,
	.cfg_board_common_setting.ddr_rdbi_wr_enable							= DDR_WRITE_READ_DBI_DISABLE,             //DDR_WRITE_READ_DBI_ENABLE,//DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.pll_ssc_mode								= DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.org_tdqs2dq								= 0,
	.cfg_board_common_setting.reserve1_test_function						= { 0 },
	.cfg_board_common_setting.ddr_dmc_remap								= DDR_DMC_REMAP_DDR4_32BIT,
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0,  0,	0,  1,	2,  3,	8, 27,
		10, 2,	9,  21, 5,  14, 1, 0,
		25, 13, 12, 4,	7,  22, 0, 0,
		0,  0,	6,  3,	20, 0,	0, 15,
		26, 11,
	},
#endif
#if 1
	//T3 sip ac pinmux
	.cfg_board_common_setting.ac_pinmux		=
	{//sip
		0,	0,	0,	1,	2,	3,
		8,	27,	10,	2,	9,	21,	5,	23,	1,	0,	25,	13,	12,	4,	7,	28,	0,	0,	0,	0,	6,	3,	20,	0,	0,	24,	26,	11,	0
	},
#endif
#if 0  //test chip
	.cfg_board_common_setting.ac_pinmux		=
	{//test chip
		0,	0,	0,	1,	2,	3,
		3,	28,	20,	5,	10,	13,	11,	8,	0,	12,	27,	9,	1,	6,	4,	25,	0,	0,	0,	0,	2,	7,	23,	0,	0,	0,	26,	21,	0
	},
#endif

#if 0
	//SIP package
	//2.1 - T963(17.45X16 .8)
	//4DDR4 (NTC)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		7, 28, 20, 9, 2, 21,6,	10, 0, 1, 27, 11, 12, 4, 8, 25, 0, 0, 0, 0, 5, 3, 23, 0, 0, 24, 26, 13,
	};

	//4DDR4 (SAMSUNG)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		8, 27, 10, 2, 9, 21,5,	23, 1, 0, 25, 13, 12, 4, 7, 28, 0, 0, 0, 0, 6, 3, 20, 0, 0, 24, 26, 11,
	};
#endif
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0, 0, 0, 1, 2, 3,
		0, 1, 2, 3, 4, 5,6,  7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 20, 21, 22, 0, 0, 25, 26, 27,
	},
#endif
	.cfg_board_common_setting.ddr_dqs_swap								= 0,
	.cfg_board_common_setting.ddr_dq_remap								=
	{
		0,  1,	2,  3,	4,  5,	6,  7,
		8,  9,	10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35
	},                                                           //d0-d31 dm0 dm1 dm2 dm3

	.cfg_board_common_setting.ddr_vddee_setting							= { 0 },
	.cfg_board_SI_setting_ps[0].DRAMFreq = 1320,
	.cfg_board_SI_setting_ps[0].PllBypassEn = 0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl = 0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,                                                        //DDR_DRAM_ODT_W_CS0_ODT0,//DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,//DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                                //for pxp dram weak driver model
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_60_OHM,
	//.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_240_OHM,

	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting = DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].dfe_offset = 4,
	.cfg_board_SI_setting_ps[0].vref_ac_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil = 0,
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency = 0,                                                            //0,
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

	.cfg_ddr_training_delay_ps[0].reserve_training_parameter =  {0xa,0xa,0xa,0xa,0,0,0,0,0x80+0x18,0x80+0x18,0x80+0x18,0x80+0x18},

	.cfg_board_SI_setting_ps[0].vref_soc_data_permil =0x000002a8,// 680
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
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=160,//0x000000b0,// 176
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=145,//0x000000c4,// 196
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=160,//0x000000c7,// 199
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=160,//0x000000a7,// 167
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=216,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=225,//0x000000db,// 219
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=216,//0x000000e7,// 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=225,//0x000000e3,// 227
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=216,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=225,//0x000000c9,// 201
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=216,//0x000000e6,// 230
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=225,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=208,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=208,//0x0000010b,// 267
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=208,//0x000000e8,// 232
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=208,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=208,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=208,//0x000000f7,// 247
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=208,//0x000000f4,// 244
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=208,//0x00000104,// 260
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=221,//0x000000fc,// 252
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=210,//0x000000fa,// 250
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=221,//0x000000e0,// 224
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=210,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=221,//0x000000e4,// 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=221,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=215,//0x000000f0,// 240
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=215,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=215,//0x000000ce,// 206
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=224,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=215,//0x000000de,// 222
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=224,//0x000000ef,// 239
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=220,//0x000000d8,// 216
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=210,//0x000000df,// 223
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=216,//0x000000d9,// 217
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=217,//0x000000e0,// 224
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
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=545,// 854
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=567,// 855
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=568,//0x0000033e,// 830
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=557,//0x00000348,// 840
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=132,//0x00000086,// 134
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=132,//0x0000007f,// 127
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=132,//0x0000006f,// 111
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=132,//0x00000066,// 102
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=57,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=68,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=70,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=59,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=71,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=62,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=68,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=64,//0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=58,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=67,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=56,//0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=65,//0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=59,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=78,//0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=60,//0x00000058,// 88
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=68,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=64,//0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=67,//0x00000034,// 52
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=55,//0x0064000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=72,//0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=61,//0x00000057,// 87
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=70,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=58,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=67,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=64,//0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=57,//0x00000061,// 97
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=62,//0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=58,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=66,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=61,//0x0000005c,// 92
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=65,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=58,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=60,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=60,//0x0000004a,// 74
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
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[0]=0x0000000,// 40
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
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[0]=0x0,// 0
},
};
#endif
#if ENABLE_SKT_BOARD	//timing_config,AR309
ddr_set_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {
//ddr_set_t ddr_set_t_default[2] ={
{
	.cfg_board_common_setting.timming_magic								= 0,
	.cfg_board_common_setting.timming_max_valid_configs						= 0,//sizeof(__ddr_setting) / sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_version						= 0,
	.cfg_board_common_setting.timming_struct_org_size						= sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_real_size						= 0,                                      //0
	.cfg_board_common_setting.fast_boot								= { 0 },
	//.cfg_board_common_setting.ddr_func								= DDR_FUNC_CONFIG_DFE_FUNCTION,
	.cfg_board_common_setting.board_id								= CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.DramType								= CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.dram_rank_config							= CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.DisabledDbyte								= CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.dram_cs0_base_add							= 0,
	.cfg_board_common_setting.dram_cs1_base_add							= 0,
	//.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,
	//.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_x4x8x16_mode							= CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.Is2Ttiming								= CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.log_level								= LOG_LEVEL_BASIC,
	.cfg_board_common_setting.ddr_rdbi_wr_enable							= DDR_WRITE_READ_DBI_DISABLE,             //DDR_WRITE_READ_DBI_ENABLE,//DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.pll_ssc_mode								= DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.org_tdqs2dq								= 0,
	.cfg_board_common_setting.reserve1_test_function						= { 0 },
	.cfg_board_common_setting.ddr_dmc_remap								= DDR_DMC_REMAP_DDR4_32BIT,
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0,  0,	0,  1,	2,  3,	8, 27,
		10, 2,	9,  21, 5,  14, 1, 0,
		25, 13, 12, 4,	7,  22, 0, 0,
		0,  0,	6,  3,	20, 0,	0, 15,
		26, 11,
	},
#endif
#if 0
	//T3 sip ac pinmux
	.cfg_board_common_setting.ac_pinmux		=
	{//sip
		0,	0,	0,	1,	2,	3,
		8,	27,	10,	2,	9,	21,	5,	23,	1,	0,	25,	13,	12,	4,	7,	28,	0,	0,	0,	0,	6,	3,	20,	0,	0,	24,	26,	11,	0
	},
#endif
#if 1  //test chip
	.cfg_board_common_setting.ac_pinmux		=
	{//test chip
		0,	0,	0,	1,	2,	3,
		3,	28,	20,	5,	10,	13,	11,	8,	0,	12,	27,	9,	1,	6,	4,	25,	0,	0,	0,	0,	2,	7,	23,	0,	0,	24,	26,	21,	0
	},
#endif

#if 0
	//SIP package
	//2.1 - T963(17.45X16 .8)
	//4DDR4 (NTC)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		7, 28, 20, 9, 2, 21,6,	10, 0, 1, 27, 11, 12, 4, 8, 25, 0, 0, 0, 0, 5, 3, 23, 0, 0, 24, 26, 13,
	};

	//4DDR4 (SAMSUNG)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		8, 27, 10, 2, 9, 21,5,	23, 1, 0, 25, 13, 12, 4, 7, 28, 0, 0, 0, 0, 6, 3, 20, 0, 0, 24, 26, 11,
	};
#endif
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0, 0, 0, 1, 2, 3,
		0, 1, 2, 3, 4, 5,6,  7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 20, 21, 22, 0, 0, 25, 26, 27,
	},
#endif
	.cfg_board_common_setting.ddr_dqs_swap								= 0,
	.cfg_board_common_setting.ddr_dq_remap								=
	{
		0,  1,	2,  3,	4,  5,	6,  7,
		8,  9,	10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35
	},                                                           //d0-d31 dm0 dm1 dm2 dm3

	.cfg_board_common_setting.ddr_vddee_setting							= { 0 },
	.cfg_board_SI_setting_ps[0].DRAMFreq = 1008,//skt
	.cfg_board_SI_setting_ps[0].PllBypassEn = 0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl = 0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,                                                        //DDR_DRAM_ODT_W_CS0_ODT0,//DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,//DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                                //for pxp dram weak driver model
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_60_OHM,
	//.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_240_OHM,

#if 0
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,                                                       //DDR_DRAM_ODT_W_CS0_ODT0,//DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,//DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                                //for pxp dram weak driver model
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_80_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_0_OHM,
#endif
	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting = DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].dfe_offset = 4,
	.cfg_board_SI_setting_ps[0].vref_ac_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil = 0,
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency = 0,                                                            //0,
	.cfg_board_SI_setting_ps[0].training_phase_parameter = { 0 },
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org =
	{
		128, 128, 128 , 128, 128, 128, 128, 128,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384,
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[0].ac_trace_delay =
	{
		128, 128, 128 , 128, 128, 128, 128, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384,
	},

#if 0           //fail delay;
	.cfg_ddr_training_delay_ps[0].ac_trace_delay =
	{
		128, 128, 128, 128, 128, 128, 128, 384,
		384, 384, 384, 384, 384, 384, 384, 384,
		384, 384, 384, 384, 384, 384, 384, 384,
		384, 384, 384, 384, 384, 384, 384, 384,
		384, 384, 384, 384,
	},                                                     //total 36
#endif
	//.cfg_ddr_training_delay_ps[0].reserve_training_parameter = {0,0,0,0,0,0,0,0,0+0,0+0,0+0,0+0},
	.cfg_ddr_training_delay_ps[0].reserve_training_parameter = {0xa,0xa,0xa,0xa,0,0,0,0,0x80+0x18,0x80+0x18,0x80+0x18,0x80+0x18},

	.cfg_board_SI_setting_ps[0].vref_soc_data_permil =0x000002a8,// 680
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
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[2]=0x00000080,// 88
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
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=0x00000145,// 325
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=0x0000013b,// 315
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=0x0000014f,// 335
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=0x00000145,// 325
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000102,// 258

	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=216,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=225,//0x000000db,// 219
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=216,//0x000000e7,// 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=225,//0x000000e3,// 227
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=216,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=225,//0x000000c9,// 201
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=216,//0x000000e6,// 230
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=225,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=208,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=208,//0x0000010b,// 267
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=208,//0x000000e8,// 232
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=208,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=208,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=208,//0x000000f7,// 247
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=208,//0x000000f4,// 244
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=208,//0x00000104,// 260
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=221,//0x000000fc,// 252
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=210,//0x000000fa,// 250
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=221,//0x000000e0,// 224
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=210,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=221,//0x000000e4,// 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=221,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=215,//0x000000f0,// 240
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=215,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=215,//0x000000ce,// 206
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=224,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=215,//0x000000de,// 222
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=224,//0x000000ef,// 239
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=220,//0x000000d8,// 216
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=210,//0x000000df,// 223
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=216,//0x000000d9,// 217
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=217,//0x000000e0,// 224
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
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=545,// 854
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=567,// 855
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=568,//0x0000033e,// 830
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=557,//0x00000348,// 840
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=132,//0x00000086,// 134
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=132,//0x0000007f,// 127
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=132,//0x0000006f,// 111
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=132,//0x00000066,// 102
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=57,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=68,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=70,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=59,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=71,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=62,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=68,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=64,//0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=58,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=67,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=56,//0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=65,//0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=59,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=78,//0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=60,//0x00000058,// 88
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=68,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=64,//0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=67,//0x00000034,// 52
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=55,//0x0064000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=72,//0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=61,//0x00000057,// 87
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=70,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=58,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=67,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=64,//0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=57,//0x00000061,// 97
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=62,//0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=58,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=66,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=61,//0x0000005c,// 92
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=65,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=58,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=60,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=60,//0x0000004a,// 74
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
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[0]=0x0000000,// 40
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
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[0]=0x0,// 0
},
{
	.cfg_board_common_setting.timming_magic								= 0,
	.cfg_board_common_setting.timming_max_valid_configs						= 0,//sizeof(__ddr_setting) / sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_version						= 0,
	.cfg_board_common_setting.timming_struct_org_size						= sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_real_size						= 0,                                      //0
	.cfg_board_common_setting.fast_boot								= { 0 },
	//.cfg_board_common_setting.ddr_func								= DDR_FUNC_CONFIG_DFE_FUNCTION,
	.cfg_board_common_setting.board_id								= CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.DramType								= CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.dram_rank_config							= CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.DisabledDbyte								= CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.dram_cs0_base_add							= 0,
	.cfg_board_common_setting.dram_cs1_base_add							= 0,
	//.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,
	//.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_x4x8x16_mode							= CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.Is2Ttiming								= CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.log_level								= LOG_LEVEL_BASIC,
	.cfg_board_common_setting.ddr_rdbi_wr_enable							= DDR_WRITE_READ_DBI_DISABLE,             //DDR_WRITE_READ_DBI_ENABLE,//DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.pll_ssc_mode								= DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.org_tdqs2dq								= 0,
	.cfg_board_common_setting.reserve1_test_function						= { 0 },
	.cfg_board_common_setting.ddr_dmc_remap								= DDR_DMC_REMAP_DDR4_32BIT,
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0,  0,	0,  1,	2,  3,	8, 27,
		10, 2,	9,  21, 5,  14, 1, 0,
		25, 13, 12, 4,	7,  22, 0, 0,
		0,  0,	6,  3,	20, 0,	0, 15,
		26, 11,
	},
#endif
#if 0
	//T3 sip ac pinmux
	.cfg_board_common_setting.ac_pinmux		=
	{//sip
		0,	0,	0,	1,	2,	3,
		8,	27,	10,	2,	9,	21,	5,	23,	1,	0,	25,	13,	12,	4,	7,	28,	0,	0,	0,	0,	6,	3,	20,	0,	0,	24,	26,	11,	0
	},
#endif
#if 1  //test chip
	.cfg_board_common_setting.ac_pinmux		=
	{//test chip
		0,	0,	0,	1,	2,	3,
		5,	28,	0,	26,	1,	20,	3,	10,	9,	13,	8,	25,	27,	2,	12,	6,	0,	0,	0,	0,	4,	7,	11,	0,	0,	24,	23,	21,	0
	},
#endif

#if 0
	//SIP package
	//2.1 - T963(17.45X16 .8)
	//4DDR4 (NTC)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		7, 28, 20, 9, 2, 21,6,	10, 0, 1, 27, 11, 12, 4, 8, 25, 0, 0, 0, 0, 5, 3, 23, 0, 0, 24, 26, 13,
	};

	//4DDR4 (SAMSUNG)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		8, 27, 10, 2, 9, 21,5,	23, 1, 0, 25, 13, 12, 4, 7, 28, 0, 0, 0, 0, 6, 3, 20, 0, 0, 24, 26, 11,
	};
#endif
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0, 0, 0, 1, 2, 3,
		0, 1, 2, 3, 4, 5,6,  7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 20, 21, 22, 0, 0, 25, 26, 27,
	},
#endif
	.cfg_board_common_setting.ddr_dqs_swap								= 0,
	.cfg_board_common_setting.ddr_dq_remap								=
	{
		0,  1,	2,  3,	4,  5,	6,  7,
		8,  9,	10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35
	},                                                           //d0-d31 dm0 dm1 dm2 dm3

	.cfg_board_common_setting.ddr_vddee_setting							= { 0 },
	.cfg_board_SI_setting_ps[0].DRAMFreq = 1008,
	.cfg_board_SI_setting_ps[0].PllBypassEn = 0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl = 0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,                                                        //DDR_DRAM_ODT_W_CS0_ODT0,//DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,//DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                                //for pxp dram weak driver model
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_60_OHM,
	//.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_240_OHM,

	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting = DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].dfe_offset = 4,
	.cfg_board_SI_setting_ps[0].vref_ac_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil = 0,
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency = 0,                                                            //0,
	.cfg_board_SI_setting_ps[0].training_phase_parameter = { 0 },
	.cfg_board_SI_setting_ps[0].ac_trace_delay_org =
	{
		128, 128, 128 , 128, 128, 128, 128, 128,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384,
	},                                                     //total 36
	.cfg_ddr_training_delay_ps[0].ac_trace_delay =
	{
		128, 128, 128 , 128, 128, 128, 128, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384, 384, 384, 384, 384,
		384, 384, 384,	    384,
	},
	//.cfg_ddr_training_delay_ps[0].reserve_training_parameter = {0,0,0,0,0,0,0,0,0+0,0+0,0+0,0+0},
	.cfg_ddr_training_delay_ps[0].reserve_training_parameter =  {0xa,0xa,0xa,0xa,0,0,0,0,0x80+0x18,0x80+0x18,0x80+0x18,0x80+0x18},

	.cfg_board_SI_setting_ps[0].vref_soc_data_permil =0x000002a8,// 680
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
	.cfg_ddr_training_delay_ps[0].ac_trace_delay[2]=0x00000080,// 88
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
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=128,//0x000000b0,// 176
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=128,//0x000000c4,// 196
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=160,//0x000000c7,// 199
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=160,//0x000000a7,// 167
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000102,// 258

	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=0x00000131,// 305
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=0x00000131,// 305
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=0x00000113,// 275
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=0x00000113,// 275
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000102,// 258

	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=216,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=225,//0x000000db,// 219
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=216,//0x000000e7,// 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=225,//0x000000e3,// 227
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=216,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=225,//0x000000c9,// 201
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=216,//0x000000e6,// 230
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=225,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=208,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=208,//0x0000010b,// 267
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=208,//0x000000e8,// 232
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=208,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=208,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=208,//0x000000f7,// 247
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=208,//0x000000f4,// 244
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=208,//0x00000104,// 260
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=221,//0x000000fc,// 252
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=210,//0x000000fa,// 250
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=221,//0x000000e0,// 224
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=210,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=221,//0x000000e4,// 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=221,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=215,//0x000000f0,// 240
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=215,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=215,//0x000000ce,// 206
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=224,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=215,//0x000000de,// 222
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=224,//0x000000ef,// 239
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=220,//0x000000d8,// 216
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=210,//0x000000df,// 223
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=216,//0x000000d9,// 217
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=217,//0x000000e0,// 224
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
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=545,// 854
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=567,// 855
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=568,//0x0000033e,// 830
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=557,//0x00000348,// 840
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=132,//0x00000086,// 134
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=132,//0x0000007f,// 127
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=132,//0x0000006f,// 111
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=132,//0x00000066,// 102
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=57,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=68,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=70,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=59,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=71,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=62,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=68,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=64,//0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=58,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=67,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=56,//0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=65,//0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=59,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=78,//0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=60,//0x00000058,// 88
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=68,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=64,//0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=67,//0x00000034,// 52
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=55,//0x0064000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=72,//0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=61,//0x00000057,// 87
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=70,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=58,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=67,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=64,//0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=57,//0x00000061,// 97
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=62,//0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=58,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=66,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=61,//0x0000005c,// 92
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=65,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=58,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=60,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=60,//0x0000004a,// 74
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
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[0]=0x0000000,// 40
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
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[0]=0x0,// 0
},
};
#endif	//ENABLE_SKT_BOARD
#if 0	//timing_config,
ddr_set_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {
//ddr_set_t ddr_set_t_default[2] ={
{
	.cfg_board_common_setting.timming_magic								= 0,
	.cfg_board_common_setting.timming_max_valid_configs						= 0,//sizeof(__ddr_setting) / sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_version						= 0,
	.cfg_board_common_setting.timming_struct_org_size						= sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_real_size						= 0,                                      //0
	.cfg_board_common_setting.fast_boot								= { 0 },
	.cfg_board_common_setting.ddr_func								= DDR_FUNC_CONFIG_DFE_FUNCTION,
	.cfg_board_common_setting.board_id								= CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.DramType								= CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.dram_rank_config							= CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.DisabledDbyte								= CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.dram_cs0_base_add							= 0,
	.cfg_board_common_setting.dram_cs1_base_add							= 0,
	//.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,
	//.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_x4x8x16_mode							= CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.Is2Ttiming								= CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.log_level								= LOG_LEVEL_BASIC,
	.cfg_board_common_setting.ddr_rdbi_wr_enable							= DDR_WRITE_READ_DBI_DISABLE,             //DDR_WRITE_READ_DBI_ENABLE,//DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.pll_ssc_mode								= DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.org_tdqs2dq								= 0,
	.cfg_board_common_setting.reserve1_test_function						= { 0 },
	.cfg_board_common_setting.ddr_dmc_remap								= DDR_DMC_REMAP_DDR4_32BIT,
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0,  0,	0,  1,	2,  3,	8, 27,
		10, 2,	9,  21, 5,  14, 1, 0,
		25, 13, 12, 4,	7,  22, 0, 0,
		0,  0,	6,  3,	20, 0,	0, 15,
		26, 11,
	},
#endif
#if 1
	//T3 sip ac pinmux
	.cfg_board_common_setting.ac_pinmux		=
	{//sip
		0,	0,	0,	1,	2,	3,
		8,	27,	10,	2,	9,	21,	5,	23,	1,	0,	25,	13,	12,	4,	7,	28,	0,	0,	0,	0,	6,	3,	20,	0,	0,	24,	26,	11,	0
	},
#endif
#if 0  //test chip
	.cfg_board_common_setting.ac_pinmux		=
	{//test chip
		0,	0,	0,	1,	2,	3,
		3,	28,	20,	5,	10,	13,	11,	8,	0,	12,	27,	9,	1,	6,	4,	25,	0,	0,	0,	0,	2,	7,	23,	0,	0,	24,	26,	21,	0
	},
#endif

#if 0
	//SIP package
	//2.1 - T963(17.45X16 .8)
	//4DDR4 (NTC)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		7, 28, 20, 9, 2, 21,6,	10, 0, 1, 27, 11, 12, 4, 8, 25, 0, 0, 0, 0, 5, 3, 23, 0, 0, 24, 26, 13,
	};

	//4DDR4 (SAMSUNG)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		8, 27, 10, 2, 9, 21,5,	23, 1, 0, 25, 13, 12, 4, 7, 28, 0, 0, 0, 0, 6, 3, 20, 0, 0, 24, 26, 11,
	};
#endif
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0, 0, 0, 1, 2, 3,
		0, 1, 2, 3, 4, 5,6,  7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 20, 21, 22, 0, 0, 25, 26, 27,
	},
#endif
	.cfg_board_common_setting.ddr_dqs_swap								= 0,
	.cfg_board_common_setting.ddr_dq_remap								=
	{
		0,  1,	2,  3,	4,  5,	6,  7,
		8,  9,	10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35
	},                                                           //d0-d31 dm0 dm1 dm2 dm3

	.cfg_board_common_setting.ddr_vddee_setting							= { 0 },
	.cfg_board_SI_setting_ps[0].DRAMFreq = 1320,
	.cfg_board_SI_setting_ps[0].PllBypassEn = 0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl = 0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,                                                        //DDR_DRAM_ODT_W_CS0_ODT0,//DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,//DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                                //for pxp dram weak driver model
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_60_OHM,
	//.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_240_OHM,

#if 0
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,                                                       //DDR_DRAM_ODT_W_CS0_ODT0,//DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,//DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                                //for pxp dram weak driver model
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_80_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_0_OHM,
#endif
	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting = DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].dfe_offset = 4,
	.cfg_board_SI_setting_ps[0].vref_ac_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil = 0,
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency = 0,                                                            //0,
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

#if 0           //fail delay;
	.cfg_ddr_training_delay_ps[0].ac_trace_delay =
	{
		128, 128, 128, 128, 128, 128, 128, 384,
		384, 384, 384, 384, 384, 384, 384, 384,
		384, 384, 384, 384, 384, 384, 384, 384,
		384, 384, 384, 384, 384, 384, 384, 384,
		384, 384, 384, 384,
	},                                                     //total 36
#endif

	.cfg_ddr_training_delay_ps[0].reserve_training_parameter = {0xa,0xa,0xa,0xa,0,0,0,0,0x80+0x18,0x80+0x18,0x80+0x18,0x80+0x18},

	.cfg_board_SI_setting_ps[0].vref_soc_data_permil =0x000002a8,// 680
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
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=160,//0x000000b0,// 176
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=145,//0x000000c4,// 196
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=160,//0x000000c7,// 199
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=160,//0x000000a7,// 167
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=216,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=225,//0x000000db,// 219
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=216,//0x000000e7,// 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=225,//0x000000e3,// 227
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=216,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=225,//0x000000c9,// 201
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=216,//0x000000e6,// 230
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=225,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=208,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=208,//0x0000010b,// 267
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=208,//0x000000e8,// 232
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=208,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=208,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=208,//0x000000f7,// 247
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=208,//0x000000f4,// 244
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=208,//0x00000104,// 260
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=221,//0x000000fc,// 252
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=210,//0x000000fa,// 250
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=221,//0x000000e0,// 224
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=210,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=221,//0x000000e4,// 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=221,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=215,//0x000000f0,// 240
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=215,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=215,//0x000000ce,// 206
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=224,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=215,//0x000000de,// 222
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=224,//0x000000ef,// 239
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=220,//0x000000d8,// 216
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=210,//0x000000df,// 223
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=216,//0x000000d9,// 217
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=217,//0x000000e0,// 224
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
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=545,// 854
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=567,// 855
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=568,//0x0000033e,// 830
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=557,//0x00000348,// 840
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=132,//0x00000086,// 134
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=132,//0x0000007f,// 127
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=132,//0x0000006f,// 111
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=132,//0x00000066,// 102
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=57,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=68,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=70,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=59,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=71,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=62,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=68,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=64,//0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=58,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=67,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=56,//0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=65,//0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=59,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=78,//0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=60,//0x00000058,// 88
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=68,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=64,//0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=67,//0x00000034,// 52
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=55,//0x0064000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=72,//0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=61,//0x00000057,// 87
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=70,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=58,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=67,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=64,//0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=57,//0x00000061,// 97
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=62,//0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=58,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=66,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=61,//0x0000005c,// 92
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=65,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=58,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=60,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=60,//0x0000004a,// 74
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
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[0]=0x0000000,// 40
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
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[0]=0x0,// 0
},
{
	.cfg_board_common_setting.timming_magic								= 0,
	.cfg_board_common_setting.timming_max_valid_configs						= 0,//sizeof(__ddr_setting) / sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_version						= 0,
	.cfg_board_common_setting.timming_struct_org_size						= sizeof(ddr_set_t),
	.cfg_board_common_setting.timming_struct_real_size						= 0,                                      //0
	.cfg_board_common_setting.fast_boot								= { 0 },
	.cfg_board_common_setting.ddr_func								= DDR_FUNC_CONFIG_DFE_FUNCTION,
	.cfg_board_common_setting.board_id								= CONFIG_BOARD_ID_MASK,
	.cfg_board_common_setting.DramType								= CONFIG_DDR_TYPE_DDR4,
	.cfg_board_common_setting.dram_rank_config							= CONFIG_DDR0_32BIT_RANK0_CH0,
	.cfg_board_common_setting.DisabledDbyte								= CONFIG_DISABLE_D32_D63,
	.cfg_board_common_setting.dram_cs0_base_add							= 0,
	.cfg_board_common_setting.dram_cs1_base_add							= 0,
	//.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,
	//.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
	.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET),
	.cfg_board_common_setting.dram_x4x8x16_mode							= CONFIG_DRAM_MODE_X16,
	.cfg_board_common_setting.Is2Ttiming								= CONFIG_USE_DDR_2T_MODE,
	.cfg_board_common_setting.log_level								= LOG_LEVEL_BASIC,
	.cfg_board_common_setting.ddr_rdbi_wr_enable							= DDR_WRITE_READ_DBI_DISABLE,             //DDR_WRITE_READ_DBI_ENABLE,//DDR_WRITE_READ_DBI_DISABLE,
	.cfg_board_common_setting.pll_ssc_mode								= DDR_PLL_SSC_DISABLE,
	.cfg_board_common_setting.org_tdqs2dq								= 0,
	.cfg_board_common_setting.reserve1_test_function						= { 0 },
	.cfg_board_common_setting.ddr_dmc_remap								= DDR_DMC_REMAP_DDR4_32BIT,
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0,  0,	0,  1,	2,  3,	8, 27,
		10, 2,	9,  21, 5,  14, 1, 0,
		25, 13, 12, 4,	7,  22, 0, 0,
		0,  0,	6,  3,	20, 0,	0, 15,
		26, 11,
	},
#endif
#if 1
	//T3 sip ac pinmux
	.cfg_board_common_setting.ac_pinmux		=
	{//sip
		0,	0,	0,	1,	2,	3,
		8,	27,	10,	2,	9,	21,	5,	23,	1,	0,	25,	13,	12,	4,	7,	28,	0,	0,	0,	0,	6,	3,	20,	0,	0,	24,	26,	11,	0
	},
#endif
#if 0  //test chip
	.cfg_board_common_setting.ac_pinmux		=
	{//test chip
		0,	0,	0,	1,	2,	3,
		3,	28,	20,	5,	10,	13,	11,	8,	0,	12,	27,	9,	1,	6,	4,	25,	0,	0,	0,	0,	2,	7,	23,	0,	0,	0,	26,	21,	0
	},
#endif

#if 0
	//SIP package
	//2.1 - T963(17.45X16 .8)
	//4DDR4 (NTC)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		7, 28, 20, 9, 2, 21,6,	10, 0, 1, 27, 11, 12, 4, 8, 25, 0, 0, 0, 0, 5, 3, 23, 0, 0, 24, 26, 13,
	};

	//4DDR4 (SAMSUNG)
	unsigned char ddr_ac_pinmux_ddr4_default[AML_AC_PINMUX_G1_TOTAL + AML_AC_PINMUX_G0_TOTAL] =
	{
		0, 0,  0,  1, 2, 3,
		8, 27, 10, 2, 9, 21,5,	23, 1, 0, 25, 13, 12, 4, 7, 28, 0, 0, 0, 0, 6, 3, 20, 0, 0, 24, 26, 11,
	};
#endif
#if 0
	.cfg_board_common_setting.ac_pinmux		=
	{
		0, 0, 0, 1, 2, 3,
		0, 1, 2, 3, 4, 5,6,  7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 20, 21, 22, 0, 0, 25, 26, 27,
	},
#endif
	.cfg_board_common_setting.ddr_dqs_swap								= 0,
	.cfg_board_common_setting.ddr_dq_remap								=
	{
		0,  1,	2,  3,	4,  5,	6,  7,
		8,  9,	10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35
	},                                                           //d0-d31 dm0 dm1 dm2 dm3

	.cfg_board_common_setting.ddr_vddee_setting							= { 0 },
	.cfg_board_SI_setting_ps[0].DRAMFreq = 1320,
	.cfg_board_SI_setting_ps[0].PllBypassEn = 0,
	.cfg_board_SI_setting_ps[0].training_SequenceCtrl = 0,
	.cfg_board_SI_setting_ps[0].ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,                                                        //DDR_DRAM_ODT_W_CS0_ODT0,//DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,//DDR_DRAM_ODT_W_CS0_ODT0,
	.cfg_board_SI_setting_ps[0].clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
	.cfg_board_SI_setting_ps[0].soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                                //for pxp dram weak driver model
	.cfg_board_SI_setting_ps[0].dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_60_OHM,
	//.cfg_board_SI_setting_ps[0].dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_240_OHM,

	.cfg_board_SI_setting_ps[0].dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
	.cfg_board_SI_setting_ps[0].dram_data_drv_pull_up_calibration_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
	.cfg_board_SI_setting_ps[0].lpddr4_dram_vout_voltage_range_setting = DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
	.cfg_board_SI_setting_ps[0].dfe_offset = 4,
	.cfg_board_SI_setting_ps[0].vref_ac_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_soc_data_permil = 0,
	.cfg_board_SI_setting_ps[0].vref_dram_data_permil = 0,
	.cfg_board_SI_setting_ps[0].max_core_timmming_frequency = 0,                                                            //0,
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

	.cfg_ddr_training_delay_ps[0].reserve_training_parameter =  {0xa,0xa,0xa,0xa,0,0,0,0,0x80+0x18,0x80+0x18,0x80+0x18,0x80+0x18},

	.cfg_board_SI_setting_ps[0].vref_soc_data_permil =0x000002a8,// 680
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
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[0]=160,//0x000000b0,// 176
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[1]=145,//0x000000c4,// 196
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[2]=160,//0x000000c7,// 199
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[3]=160,//0x000000a7,// 167
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[4]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[5]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[6]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dqs_delay[7]=0x00000102,// 258
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[0]=216,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[1]=225,//0x000000db,// 219
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[2]=216,//0x000000e7,// 231
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[3]=225,//0x000000e3,// 227
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[4]=216,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[5]=225,//0x000000c9,// 201
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[6]=216,//0x000000e6,// 230
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[7]=225,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[9]=208,//0x000000da,// 218
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[10]=208,//0x0000010b,// 267
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[11]=208,//0x000000e8,// 232
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[12]=208,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[13]=208,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[14]=208,//0x000000f7,// 247
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[15]=208,//0x000000f4,// 244
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[16]=208,//0x00000104,// 260
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[18]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[19]=221,//0x000000fc,// 252
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[20]=210,//0x000000fa,// 250
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[21]=221,//0x000000e0,// 224
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[22]=210,//0x000000f3,// 243
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[23]=221,//0x000000e4,// 228
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[24]=210,//0x000000f9,// 249
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[25]=221,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[27]=215,//0x000000f0,// 240
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[28]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[29]=215,//0x000000ec,// 236
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[30]=224,//0x000000f5,// 245
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[31]=215,//0x000000ce,// 206
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[32]=224,//0x000000e9,// 233
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[33]=215,//0x000000de,// 222
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[34]=224,//0x000000ef,// 239
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[8]=220,//0x000000d8,// 216
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[17]=210,//0x000000df,// 223
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[26]=216,//0x000000d9,// 217
	.cfg_ddr_training_delay_ps[0].write_dq_bit_delay[35]=217,//0x000000e0,// 224
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
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[0]=545,// 854
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[1]=567,// 855
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[2]=568,//0x0000033e,// 830
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[3]=557,//0x00000348,// 840
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_gate_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[0]=132,//0x00000086,// 134
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[1]=132,//0x0000007f,// 127
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[2]=132,//0x0000006f,// 111
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[3]=132,//0x00000066,// 102
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[4]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[5]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[6]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dqs_delay[7]=0x00000000,// 0
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[0]=57,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[1]=68,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[2]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[3]=70,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[4]=59,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[5]=71,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[6]=62,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[7]=68,//0x0000004b,// 75
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[8]=64,//0x0000004c,// 76
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[9]=58,//0x00000059,// 89
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[10]=67,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[11]=56,//0x00000046,// 70
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[12]=65,//0x00000035,// 53
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[13]=59,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[14]=78,//0x00000041,// 65
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[15]=60,//0x00000058,// 88
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[16]=68,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[17]=64,//0x00000045,// 69
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[18]=58,//0x00000055,// 85
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[19]=67,//0x00000034,// 52
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[20]=55,//0x0064000049,// 73
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[21]=72,//0x00000038,// 56
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[22]=61,//0x00000057,// 87
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[23]=70,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[24]=58,//0x0000004d,// 77
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[25]=67,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[26]=64,//0x00000048,// 72
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[27]=57,//0x00000061,// 97
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[28]=62,//0x00000039,// 57
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[29]=58,//0x0000004e,// 78
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[30]=66,//0x0000003f,// 63
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[31]=61,//0x0000005c,// 92
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[32]=65,//0x00000036,// 54
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[33]=58,//0x00000056,// 86
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[34]=60,//0x00000047,// 71
	.cfg_ddr_training_delay_ps[0].read_dq_bit_delay[35]=60,//0x0000004a,// 74
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
	.cfg_ddr_training_delay_ps[0].soc_bit_vref[0]=0x0000000,// 40
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
	.cfg_ddr_training_delay_ps[0].dram_bit_vref[0]=0x0,// 0
},
};
#endif

board_clk_set_t __board_clk_setting
__attribute__ ((section(".clk_param"))) = {
	/* clock settings for bl2 */
	.cpu_clk				= CPU_CLK / 24 * 24,
	.dsu_clk				= DSU_CLK / 24 * 24,
#ifdef CONFIG_PXP_EMULATOR
	.pxp					= 1,
#else
	.pxp					= 0,
#endif
	.high_console_baud = CONFIG_HIGH_CONSOLE_BAUD,
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

	/* demo, user defined override register */

	/* enable vddcpu dc-dc, set TEST_N to high */
	{PADCTRL_TESTN_O,	(0x1 << 0), 		(0x1 << 0),	0, 0, 0},
	{PADCTRL_TESTN_OEN,	(0x0 << 0), 		(0x1 << 0), 0, 0, 0},

	/* set pwm h and pwm j clock rate to 24M, enable them */

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
		.device_fip_container_size = 0x300000,
		.device_fip_container_copies = 4,
	},
	.nand				= {
		.version = 0x01,
		.bbt_pages = 0x1,
		.bbt_start_block = 0x20,
		.discrete_mode = 1,
		.setup_data.nand_setup_data = (2 << 20) |		\
					(0 << 19) |                     \
					(1 << 17) |                     \
					(4 << 14) |                     \
					(0 << 13) |                     \
					(64 << 6) |                     \
					(4 << 0),
		.reserved_area_blk_cnt = 48,
		.page_per_block = 128,
		.use_param_page_list = 0,
	},
};
