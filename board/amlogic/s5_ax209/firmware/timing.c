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
#define CPU_CLK                                 2016

#define BOARD_USE_DDR3                                  0
#define BOARD_USE_DDR4                                  0
#define BOARD_USE_LPDDR3                                0
#define BOARD_USE_LPDDR4                                1

//#define  S5_LPDDR4_REF 1
#define S5_LPDDR4_SKT 1
#define S5_DDR4_SKT 1
#define MAX_VALID_CONFIGS 4 //timming_max_valid_configs
#define CONFIG_BOARD_TIMMING
unsigned char lpddr4_ac_pinmux[] = {
	1,  0,	2,  3,	4,  5,	8,  9,	29,
	28, 0,	7,  0,	6,  0,
	14, 15, 13, 12, 11, 10, 18, 19, 27,
	26, 16, 17, 0,	0,  0,
};

unsigned char ddr4_ac_pinmux[] = {
	9,  3,	16, 14, 0,  13,
	12, 1,	15, 5,
	2,  27, 21, 26, 10, 11,
	8,  19, 7,  18, 20,
	24, 25, 29, 28, 22, 23,
	4,  6,	17,
};

unsigned char lpddr4_dq_pinmux[DWC_DQ_PINMUX_TOTAL] = {
	0,  1,	2,  3,	4,  5,	6,  7,	8,  9,	10, 11, 12, 13,
	14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
	25, 26, 27, 28, 29, 30, 31
};

unsigned char lpddr4_dfi_pinmux[DWC_DFI_PINMUX_TOTAL] = { 0 };

unsigned char lpddr5_ac_pinmux[DWC_AC_PINMUX_TOTAL] = {
	2, 3, 1, 0, 5, 4, 0, 0, 0, 0, 1, 3, 5, 2, 4, 0
};

char init_mrs_seq_ddr3[] = { 0, 1, 2 };
char init_mrs_seq_ddr4[] = { 3, 6, 5, 4, 2, 1, 0 };
char init_mrs_seq_lpddr3[] = { 3, 11 };
char init_mrs_seq_lpddr4[] = { 13, 3, 4, 11, 12, 16, 17, 14, 22, 23, 1, 2, 32, 40, 15, 20 };
char init_mrs_seq_lpddr5[] = { 16, 1,  28, 41, 18, 3,  2,  17, 12,
			       14, 15, 11, 13, 20, 22, 25, 10, 46 };

/* Timing parameters */
ddr_timing_t ddr_global_timing = {
	.identifier		= 0,
	.ddr_mrd		= 4,
	.cfg_ddr_rfcab		= 421,
	.cfg_ddr_rfcpb		= 421,
	.cfg_ddr_rpab		= (17 << 16) | 17,
	.cfg_ddr_rppb		= (17 << 16) | 17,
	.cfg_ddr_rtw		= 4,
	.cfg_ddr_rl		= 16,
	.cfg_ddr_wl		= 12,
	.cfg_ddr_ras		= 39,
	.cfg_ddr_rc		= 45,
	.cfg_ddr_rcd		= 17,
	.cfg_ddr_rrds		= (8 << 16) | 7,
	.cfg_ddr_rrdl		= (8 << 16) | 7,
	.cfg_ddr_faw		= 36,
	.cfg_ddr_rtp		= 10,
	.cfg_ddr_wr		= 19,
	.cfg_ddr_wtrs		= ((10 << 16) | 6),
	.cfg_ddr_wtrl		= ((10 << 16) | 6),
	.cfg_ddr_ccds		= ((6 << 16) | 4),
	.cfg_ddr_ccdl		= ((6 << 16) | 4),
	.cfg_ddr_exsr		= 512,
	.cfg_ddr_xs		= 156,
	.cfg_ddr_xp		= 8,
	.cfg_ddr_xpdll		= 23,
	.cfg_ddr_zqcs		= 128,
	.cfg_ddr_cksre		= 10,
	.cfg_ddr_cksrx		= 10,
	.cfg_ddr_cke		= 4,
	.cfg_ddr_mod		= 24,
	.cfg_ddr_dqs		= 4,
	.cfg_ddr_rstl		= 0,
	.cfg_ddr_zqlat		= 0,
	.ddr_mrr		= 0,
	.cfg_ddr_ckesr		= 4,
	.cfg_ddr_dpd		= 0,
	.cfg_ddr_ckeck		= 4,
	.cfg_ddr_refi		= 0X20014D3b,
	.cfg_ddr_sr		= 0,
	.cfg_ddr_ccdmw		= 0,
	.cfg_ddr_escke		= 0,
	.cfg_ddr_refi_ddr3	= 0,
	.cfg_ddr_dfictrldelay	= 2,
	.cfg_ddr_dfiphywrdata	= 2,
	.cfg_ddr_dfiphywrlat	= 7,
	.cfg_ddr_dfiphyrddataen = 11,
	.cfg_ddr_dfiphyrdlat	= 32,
	.cfg_ddr_dfictrlupdmin	= 24,
	.cfg_ddr_dfictrlupdmax	= 48,
	.cfg_ddr_dfimstrresp	= 0,
	.cfg_ddr_dfirefmski	= 0,
	.cfg_ddr_dfictrlupdi	= 0,
	.cfg_ddr_dfidramclk	= (4 | (4 << 8) | (3 << 16)),
	.cfg_ddr_dfilpresp	= 0,
	.cfg_ddr_dfiphymstr	= 0,
	.cfg_ddr_rtodt		= 0,
	.cfg_ddr_wlmrd		= 0,
	.cfg_ddr_wlo		= 0,
	.cfg_ddr_al		= 0,
	.cfg_ddr_zqcl		= 0,
	.cfg_ddr_zqcsi		= 0,
	.cfg_ddr_zqreset	= 0,
	.cfg_ddr_tdqsck_min	= 0,
	.cfg_ddr_tdqsck_max	= 0,
};

//use schematic
//#ifdef S5_LPDDR4_SKT
uint8_t dq_test_pinmux_w_lp4_skt[2][36] = {
	{
		27,
		31,
		29,
		35,
		24,
		28,
		26,
		30,
		25,
		22,
		23,
		21,
		34,
		20,
		17,
		16,
		18,
		19,
		13,
		9,
		11,
		33,
		10,
		8,
		14,
		12,
		15,
		5,
		4,
		1,
		32,
		0,
		3,
		7,
		2,
		6,
	},
	{
		23,
		20,
		21,
		34,
		16,
		18,
		17,
		19,
		22,
		24,
		28,
		26,
		35,
		25,
		29,
		31,
		30,
		27,
		7,
		6,
		0,
		32,
		1,
		5,
		3,
		4,
		2,
		12,
		14,
		15,
		33,
		9,
		11,
		13,
		8,
		10,
	}
};

//#endif
//#ifdef S5_LPDDR4_REF
uint8_t dq_test_pinmux_w_lp4_ref[2][36] = {
	{
		26,
		31,
		29,
		35,
		24,
		28,
		25,
		30,
		27,
		22,
		23,
		21,
		34,
		20,
		17,
		16,
		18,
		19,
		8,
		14,
		11,
		33,
		10,
		13,
		9,
		12,
		15,
		5,
		4,
		0,
		32,
		6,
		2,
		7,
		1,
		3,
	},
	{
		16,
		20,
		21,
		34,
		22,
		18,
		17,
		19,
		23,
		24,
		28,
		26,
		35,
		25,
		29,
		30,
		31,
		27,
		0,
		1,
		2,
		32,
		3,
		7,
		4,
		6,
		5,
		11,
		14,
		15,
		33,
		8,
		10,
		13,
		12,
		9,
	}
};

uint8_t dq_test_pinmux_w_ddr4_skt[2][36] = {
	{
		3,
		0,
		5,
		32,
		2,
		4,
		1,
		7,
		6,
		12,
		8,
		14,
		33,
		10,
		11,
		9,
		13,
		15,
		16,
		19,
		23,
		34,
		21,
		17,
		20,
		22,
		18,
		24,
		25,
		31,
		35,
		29,
		26,
		28,
		30,
		27,
	},
	{
		14,
		12,
		8,
		33,
		9,
		13,
		11,
		15,
		10,
		0,
		7,
		2,
		32,
		4,
		1,
		5,
		3,
		6,
		21,
		22,
		16,
		34,
		20,
		18,
		19,
		17,
		23,
		24,
		30,
		28,
		35,
		25,
		31,
		26,
		27,
		29,
	}
};

//#endif
//bit 6 adc_channel bit 0-5 adc value,chan 3 value 8 is layer 2
#define DDR_ID_ACS_ADC   ((3 << 6) | (8))

#define DDR_RESV_CHECK_ID_ENABLE  0Xfe
#define SAR_ADC_DDR_ID_BASE   0
#define SAR_ADC_DDR_ID_STEP   80

#define DDR_TIMMING_OFFSET(X) (unsigned int)(unsigned long)(&(((ddr_set_ps0_only_t *)(0))->X))
#define DDR_TIMMING_OFFSET_SIZE(X) sizeof(((ddr_set_ps0_only_t *)(0))->X)
#define DDR_TIMMING_TUNE_TIMMING0(DDR_ID, PARA, VALUE) (DDR_ID, \
DDR_TIMMING_OFFSET(PARA), VALUE, DDR_TIMMING_OFFSET_SIZE(PARA), 0, \
DDR_RESV_CHECK_ID_ENABLE)
#define DDR_TIMMING_TUNE_TIMMING1(DDR_ID, PARA, VALUE) (DDR_ID, \
(sizeof(ddr_set_t) + (DDR_TIMMING_OFFSET(PARA))), VALUE, DDR_TIMMING_OFFSET_SIZE(PARA), \
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

#define DDR_TIMMING_TUNE_TIMMING0_F(PARA, VALUE) (((DDR_TIMMING_OFFSET(PARA)) << 16) | \
((DDR_TIMMING_OFFSET_SIZE(PARA)) << 28) | (VALUE))
#define DDR_TIMMING_TUNE_TIMMING1_F(PARA, VALUE) (((sizeof(ddr_set_ps0_only_t) + \
DDR_TIMMING_OFFSET(PARA)) << 16) | ((DDR_TIMMING_OFFSET_SIZE(PARA)) << 28) | (VALUE))
#define DDR_TIMMING_TUNE_START(id_src_from, id_adc_ch, id_value) ((id_src_from) | \
(id_adc_ch) | (id_value))
#define DDR_TIMMING_TUNE_ADC_MULT_START(id_value, ch0, ch1, ch2, ch3, ch4, ch5) (id_value) |\
(ch0) | ((ch1) << 4) | ((ch2) << 8) | ((ch3) << 12) | ((ch4) << 16) | ((ch5) << 20)

//#if 1
uint32_t __bl2_ddr_reg_data[] __attribute__ ((section(".ddr_2acs_data"))) = {
	DDR_ID_START_MASK,
	//DDR_TIMMING_TUNE_ADC_MULT_START(DDR_ID_FROM_ADC_MULT, VX, VX, V3, VX, VX, VX),
    //data start
	//DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_common_setting.dram_ch0_size_MB,
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET)),
	//DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_common_setting.dram_ch1_size_MB,
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET)),
	//DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_common_setting.dram_ch0_size_MB,
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET)),
	//DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_common_setting.dram_ch1_size_MB,
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
    //                    (DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET)),
};

//#endif

#define  CACLU_CLK_LP4   1848 //1792//600 //1200 //(1900)// (1440)//(1008)
//ddr_set_ps0_only_t ddr_set_t_default[2] = {
//__attribute__ ((section(".ddr_param")))
ddr_set_ps0_only_t __ddr_setting[]
__attribute__ ((section(".ddr_param"))) = {
#if S5_LPDDR4_SKT
	{
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs = MAX_VALID_CONFIGS,
		//sizeof(__ddr_setting) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,
		.cfg_board_common_setting.fast_boot = {
			0, 0, 0, 0
		},
		.cfg_board_common_setting.ddr_func = 0,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_LPDDR4,
		.cfg_board_common_setting.enable_lpddr4x_mode = 2,
		//0  force lp4x   1 force lp4
		//2 auto 4x use nn 4 use pn drivere
		//3 auto + force 4 4x both use nn driver
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
		//.cfg_board_common_setting.dram_cs0_base_add = 0,
		//.cfg_board_common_setting.dram_cs1_base_add = 0,
		.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.DisabledDbyte[0] = 0xf0,
		//bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
		.cfg_board_common_setting.DisabledDbyte[1] = 0xf0,
		//bit 0 -3 ch1 cs0 ,bit 4-7 ch1
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		.cfg_board_common_setting.log_level = 4,
		//4,//LOG_LEVEL_BASIC,
		.cfg_board_common_setting.dbi_enable = 0,
		.cfg_board_common_setting.pll_ssc_mode =
			(1 << 20) | (1 << 8) | (2 << 4) | 0,
		//center_ssc_1000ppm,//SSC_DISABLE,(1 << 20) | (0 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test = {
			0
		},
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_LPDDR4_16BIT,
		//DDR_DMC_REMAP_DDR4_32BIT,
		// .cfg_board_common_setting.ac_pinmux = {
		//2, 3, 1, 0, 5, 4, 0, 0, 0, 0, 1, 3, 5, 2, 4, 0 },
		.cfg_board_common_setting.ddr_dqs_swap = 0,
		//.cfg_board_common_setting.ddr_dq_remap = { 27,
		//31,
		//29,
		//35,
		//24,
		//28,
		//26,
		//30,
		//25,
		//22,
		//23,
		//21,
		//34,
		//20,
		//17,
		//16,
		//18,
		//19,
		//13,
		//9,
		//11,
		//33,
		//10,
		//8,
		//14,
		//12,
		//15,
		//5,
		//4,
		//1,
		//32,
		//0,
		//3,
		//7,
		//2,
		//6, },
		.cfg_board_common_setting.ddr_vddee_setting = {
			0
		},
		.cfg_board_SI_setting_ps.DRAMFreq = CACLU_CLK_LP4,
		// .cfg_ddr_training_delay_ps.DRAMFreq = 600,// 2112,//,cfg_ddr_training_delay_ps
		//.cfg_ddr_training_delay_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		.cfg_board_SI_setting_ps.dfi_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		.cfg_board_SI_setting_ps.clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_0_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = DDR_DRAM_LPDDR4_DRV_40_OHM,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = DDR_DRAM_DDR_WR_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = 120, //240,
		.cfg_board_SI_setting_ps.dram_drv_pull_up_cal_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_range =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		//.cfg_ddr_training_delay_ps.dfe_offset = 0,
		.cfg_board_SI_setting_ps.vref_ac_permil = 260,//420,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,
		//.cfg_ddr_training_delay_ps.training_phase_parameter = {0},
		//.cfg_ddr_training_delay_ps.ac_trace_delay_org = {
		//	128, 128, 128 + 40, 128, 128, 128, 128, 128,
		//	384, 384, 384,      384, 384, 384, 384, 384,
		//	384, 384, 384,      384, 384, 384, 384, 384,
		//	384, 384, 384,      384, 384, 384, 384, 384,
		//	384, 384, 384,      384,
		//},
		//total 36
		.cfg_ddr_training_delay_ps.ac_trace_delay = {
			32 + 0, 32 + 0, 32 + 0, 32 + 0,
			32,	32 + 0, 32 + 0, 32 + 0,
			32 + 0, 32 + 0, 32 + 0, 32 + 0,
			32 + 0, 32 + 0, 32,	32 + 0,
			32 + 0, 32 + 0, 32 + 0, 32 + 0,
			32 + 0, 32 + 0, 32,	32 + 0
		},
// 1 //real chip stk lp4
		.cfg_board_common_setting.dbi_enable = 0x00000000,      // 0,0x00000041
		.cfg_board_common_setting.ddr_rfc_type = 0x0000000d,    // 13,0x00000042
		.cfg_board_common_setting.pll_ssc_mode = 0x00000000,    // 0,0x00000044
//.cfg_board_common_setting.max_core_timmming_frequency=0x00000e10,// 3600,0x0000006a

//.cfg_board_common_setting.lpddr4_x8_mode=0x00000000,// 0,0x00000087
//.cfg_board_common_setting.tdqs2dq=0x00000000,// 0,0x0000008a
//.cfg_board_common_setting.dfe_offset_value=0x00000000,// 0,0x0000008e
//.cfg_board_common_setting.training_offset=0x00000000,// 0,0x0000008f

#ifdef CONFIG_PXP_TIMMING
//pxp
#define TDQS2DQ_LP4   ((410 * 128 * CACLU_CLK_LP4 * 2) / 1000000)       //
//#define TDQSCK   64
#define CLK_DELAY_LP4 0               // (64)
//#define BOARD_DQS_DELAY   64
#define TDQSCK_ADD_DQS_LP4   64         //128   //clk should use 64 steps
#define PHY_TDQS2DQ_LP4  0
#endif
#ifdef CONFIG_RTL_TIMMING
//rtl
#define TDQS2DQ_LP4  ((450 * 128 * CACLU_CLK_LP4) / 1000000)  //
//#define TDQSCK   128 //
//#define BOARD_DQS_DELAY   64+32
#define TDQSCK_ADD_DQS_LP4   ((3000 * 128 * CACLU_CLK_LP4) / 1000000)
#define CLK_DELAY_LP4  (0)
#define PHY_TDQS2DQ_LP4  ((200 * 128 * CACLU_CLK_LP4) / 1000000)
#endif
#ifdef CONFIG_BOARD_TIMMING                                     //skt lp4 board
#define TDQS2DQ_LP4   ((410 * 128 * CACLU_CLK_LP4 * 2) / 1000000)       //
//#define TDQSCK   64
//#define TDQSCK_ADD_DQS_LP4   ((3080 * 128 * CACLU_CLK_LP4) / 1000000)
#define CLK_DELAY_LP4 0                                             // (64)
//#define BOARD_DQS_DELAY   64
#define TDQSCK_ADD_DQS_LP4   64                                     //128
#define PHY_TDQS2DQ_LP4  0
#endif
		.cfg_board_SI_setting_ps.clk_drv_ohm = 40,
		.cfg_board_SI_setting_ps.cs_drv_ohm = 40,
		.cfg_board_SI_setting_ps.ac_drv_ohm = 40,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = 40,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = 40,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = 0,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = 40, //60,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = 40,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = 60,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = 0,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = 120,//240,//120,
		.cfg_board_SI_setting_ps.dram_drv_pull_up_cal_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_range =
			1,//DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		.cfg_board_common_setting.ac_pinmux = {
			1,  0,	2,  3,	4,  5,	8,  9,	29,
			28, 0,	6,  0,	7,  0,
			14, 15, 13, 12, 11, 10, 18, 19, 26,
			27, 17, 16, 0,	0,  0,
		},
		.cfg_ddr_training_delay_ps.rx_offset[0] = (1 << 7) | 0x10,
		.cfg_ddr_training_delay_ps.reserve_para = { //have higher prior over rx_offset
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs0 write dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs1 write dqs,lane0-lane3
			(1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10,
			//cs0 read dqs,lane0-lane3
			(1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10,
			//cs1 read dqs,lane0-lane3
		},
		.cfg_ddr_training_delay_ps.tx_offset[0] = (1 << 7) | 0x8,
		//.cfg_ddr_training_delay_ps.dac_offset[0] = 0,//(1 << 7) | 0x10,
		//.cfg_ddr_training_delay_ps.dac_offset[1] = 0,//(0 << 7) | 0x10,
		.cfg_ddr_training_delay_ps.dac_offset[0] = (1 << 7) | 0x0,
		.cfg_ddr_training_delay_ps.dac_offset[1] = (0 << 7) | 0x0,
		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 256,     //clk
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 256,     //clk
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 256,    //clk
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 256,    //clk
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 0x98,//152，0xd8
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 0x9b,//155，0xda
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 0xb1,//177，0xdc
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 0xa6,//166，0xde
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 0xb4,//180，0xe0
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 0xb2,//178，0xe2
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 0x0,//0，0xe4
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 0x0,//0，0xe6
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 0x1bf,//447，0xe8
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 0x1bf,//447，0xea
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 0x9b,//155，0xec
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 0xbf,//191，0xee
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 0x9b,//155，0xf0
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 0xbf,//191，0xf2
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 0x9b,//155，0xf4
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 0xb4,//180，0xf6
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 0xb2,//178，0xf8
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 0xa6,//166，0xfa
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 0xb1,//177，0xfc
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 0x98,//152，0xfe
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 0x9b,//155，0x100
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 0x0,//0，0x102
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 0x0,//0，0x104
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 0x1bf,//447，0x106
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 0x1bf,//447，0x108
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 0xbf,//191，0x10a
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 0xbf,//191，0x10c
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 0x9b,//155，0x10e
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 0x9b,//155，0x110
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 0x9b,//155，0x112

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
		//pcb write dqs length > clk length 0.5UI, ideal write dqs =
		//clk_delay(==0)+128+wl_offset*128
		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 128,//384 - 64,
		//clk_delay+128+wl_offset*128
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 128,//384 - 64,
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 128,//384 - 64,
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 128,//384 - 64,
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 128,//384 - 64,
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 128,//384 - 64,
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 128,//384 - 64,
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 128,//384 - 64,
		.cfg_ddr_training_delay_ps.write_wck_delay[0] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[1] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[2] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[3] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[4] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[5] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[6] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[7] = 0x00000200,
		.cfg_ddr_training_delay_ps.wdq_delay[0] = 384 - 64 + 64 + TDQS2DQ_LP4,
		//write dqs+64+tdqs2dq
		.cfg_ddr_training_delay_ps.wdq_delay[1] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[2] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[3] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[4] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[5] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[6] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[7] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[8] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[9] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[10] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[11] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[12] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[13] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[14] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[15] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[16] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[17] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[18] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[19] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[20] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[21] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[22] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[23] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[24] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[25] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[26] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[27] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[28] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[29] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[30] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[31] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[32] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[33] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[34] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[35] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[36] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[37] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[38] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[39] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[40] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[41] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[42] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[43] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[44] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[45] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[46] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[47] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[48] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[49] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[50] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[51] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[52] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[53] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[54] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[55] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[56] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[57] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[58] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[59] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[60] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[61] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[62] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[63] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[64] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[65] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[66] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[67] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[68] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[69] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[70] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[71] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 128,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[0] = 0x0000001a,
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

		.cfg_ddr_training_delay_ps.read_dq_delay_t[0] = 0x34,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[1] = 0x4a,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[2] = 0x42,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[3] = 0x3f,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[4] = 0x4b,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[5] = 0x33,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[6] = 0x31,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[7] = 0x44,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[8] = 0x49,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[9] = 0x3f,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[10] = 0x45,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[11] = 0x5b,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[12] = 0x4d,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[13] = 0x5a,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[14] = 0x51,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[15] = 0x4c,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[16] = 0x4c,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[17] = 0x4b,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[18] = 0x39,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[19] = 0x51,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[20] = 0x53,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[21] = 0x47,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[22] = 0x57,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[23] = 0x45,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[24] = 0x43,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[25] = 0x44,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[26] = 0x3a,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[27] = 0x3e,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[28] = 0x4b,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[29] = 0x4b,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[30] = 0x44,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[31] = 0x51,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[32] = 0x46,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[33] = 0x3b,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[34] = 0x4e,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[35] = 0x32,

		.cfg_ddr_training_delay_ps.read_dq_delay_t[0 + 36] = 0x34,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[1 + 36] = 0x4a,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[2 + 36] = 0x42,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[3 + 36] = 0x3f,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[4 + 36] = 0x4b,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[5 + 36] = 0x33,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[6 + 36] = 0x31,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[7 + 36] = 0x44,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[8 + 36] = 0x49,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[9 + 36] = 0x3f,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[10 + 36] = 0x45,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[11 + 36] = 0x5b,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[12 + 36] = 0x4d,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[13 + 36] = 0x5a,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[14 + 36] = 0x51,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[15 + 36] = 0x4c,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[16 + 36] = 0x4c,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[17 + 36] = 0x4b,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[18 + 36] = 0x39,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[19 + 36] = 0x51,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[20 + 36] = 0x53,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[21 + 36] = 0x47,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[22 + 36] = 0x57,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[23 + 36] = 0x45,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[24 + 36] = 0x43,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[25 + 36] = 0x44,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[26 + 36] = 0x3a,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[27 + 36] = 0x3e,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[28 + 36] = 0x4b,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[29 + 36] = 0x4b,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[30 + 36] = 0x44,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[31 + 36] = 0x51,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[32 + 36] = 0x46,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[33 + 36] = 0x3b,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[34 + 36] = 0x4e,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[35 + 36] = 0x32,

		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x80,
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x80,
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x80,
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x80,
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x80,
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x80,
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x80,
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x80,
		//cxmt
		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 0x8d,//141，0xd8
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 0x92,//146，0xda
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 0xa2,//162，0xdc
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 0x9d,//157，0xde
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 0xb0,//176，0xe0
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 0xae,//174，0xe2
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 0x0,//0，0xe4
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 0x0,//0，0xe6
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 0x1bf,//447，0xe8
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 0x1bf,//447，0xea
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 0x92,//146，0xec
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 0xbf,//191，0xee
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 0x92,//146，0xf0
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 0xbf,//191，0xf2
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 0x92,//146，0xf4
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 0xb0,//176，0xf6
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 0xae,//174，0xf8
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 0x9d,//157，0xfa
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 0xa2,//162，0xfc
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 0x8d,//141，0xfe
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 0x92,//146，0x100
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 0x0,//0，0x102
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 0x0,//0，0x104
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 0x1bf,//447，0x106
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 0x1bf,//447，0x108
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 0xbf,//191，0x10a
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 0xbf,//191，0x10c
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 0x92,//146，0x10e
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 0x92,//146，0x110
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 0x92,//146，0x112
		.cfg_ddr_training_delay_ps.read_dq_delay_t[0] = 0x37,//55，0x114
		.cfg_ddr_training_delay_ps.read_dq_delay_t[1] = 0x46,//70，0x116
		.cfg_ddr_training_delay_ps.read_dq_delay_t[2] = 0x3c,//60，0x118
		.cfg_ddr_training_delay_ps.read_dq_delay_t[3] = 0x3f,//63，0x11a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[4] = 0x4c,//76，0x11c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[5] = 0x36,//54，0x11e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[6] = 0x38,//56，0x120
		.cfg_ddr_training_delay_ps.read_dq_delay_t[7] = 0x40,//64，0x122
		.cfg_ddr_training_delay_ps.read_dq_delay_t[8] = 0x46,//70，0x124
		.cfg_ddr_training_delay_ps.read_dq_delay_t[9] = 0x38,//56，0x126
		.cfg_ddr_training_delay_ps.read_dq_delay_t[10] = 0x40,//64，0x128
		.cfg_ddr_training_delay_ps.read_dq_delay_t[11] = 0x49,//73，0x12a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[12] = 0x48,//72，0x12c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[13] = 0x4e,//78，0x12e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[14] = 0x4e,//78，0x130
		.cfg_ddr_training_delay_ps.read_dq_delay_t[15] = 0x54,//84，0x132
		.cfg_ddr_training_delay_ps.read_dq_delay_t[16] = 0x4d,//77，0x134
		.cfg_ddr_training_delay_ps.read_dq_delay_t[17] = 0x45,//69，0x136
		.cfg_ddr_training_delay_ps.read_dq_delay_t[18] = 0x39,//57，0x138
		.cfg_ddr_training_delay_ps.read_dq_delay_t[19] = 0x3f,//63，0x13a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[20] = 0x49,//73，0x13c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[21] = 0x3e,//62，0x13e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[22] = 0x48,//72，0x140
		.cfg_ddr_training_delay_ps.read_dq_delay_t[23] = 0x3f,//63，0x142
		.cfg_ddr_training_delay_ps.read_dq_delay_t[24] = 0x3a,//58，0x144
		.cfg_ddr_training_delay_ps.read_dq_delay_t[25] = 0x37,//55，0x146
		.cfg_ddr_training_delay_ps.read_dq_delay_t[26] = 0x3a,//58，0x148
		.cfg_ddr_training_delay_ps.read_dq_delay_t[27] = 0x35,//53，0x14a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[28] = 0x3b,//59，0x14c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[29] = 0x45,//69，0x14e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[30] = 0x3a,//58，0x150
		.cfg_ddr_training_delay_ps.read_dq_delay_t[31] = 0x3f,//63，0x152
		.cfg_ddr_training_delay_ps.read_dq_delay_t[32] = 0x3a,//58，0x154
		.cfg_ddr_training_delay_ps.read_dq_delay_t[33] = 0x32,//50，0x156
		.cfg_ddr_training_delay_ps.read_dq_delay_t[34] = 0x44,//68，0x158
		.cfg_ddr_training_delay_ps.read_dq_delay_t[35] = 0x30,//48，0x15a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[0 + 36] = 0x37,//55，0x114
		.cfg_ddr_training_delay_ps.read_dq_delay_t[1 + 36] = 0x46,//70，0x116
		.cfg_ddr_training_delay_ps.read_dq_delay_t[2 + 36] = 0x3c,//60，0x118
		.cfg_ddr_training_delay_ps.read_dq_delay_t[3 + 36] = 0x3f,//63，0x11a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[4 + 36] = 0x4c,//76，0x11c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[5 + 36] = 0x36,//54，0x11e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[6 + 36] = 0x38,//56，0x120
		.cfg_ddr_training_delay_ps.read_dq_delay_t[7 + 36] = 0x40,//64，0x122
		.cfg_ddr_training_delay_ps.read_dq_delay_t[8 + 36] = 0x46,//70，0x124
		.cfg_ddr_training_delay_ps.read_dq_delay_t[9 + 36] = 0x38,//56，0x126
		.cfg_ddr_training_delay_ps.read_dq_delay_t[10 + 36] = 0x40,//64，0x128
		.cfg_ddr_training_delay_ps.read_dq_delay_t[11 + 36] = 0x49,//73，0x12a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[12 + 36] = 0x48,//72，0x12c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[13 + 36] = 0x4e,//78，0x12e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[14 + 36] = 0x4e,//78，0x130
		.cfg_ddr_training_delay_ps.read_dq_delay_t[15 + 36] = 0x54,//84，0x132
		.cfg_ddr_training_delay_ps.read_dq_delay_t[16 + 36] = 0x4d,//77，0x134
		.cfg_ddr_training_delay_ps.read_dq_delay_t[17 + 36] = 0x45,//69，0x136
		.cfg_ddr_training_delay_ps.read_dq_delay_t[18 + 36] = 0x39,//57，0x138
		.cfg_ddr_training_delay_ps.read_dq_delay_t[19 + 36] = 0x3f,//63，0x13a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[20 + 36] = 0x49,//73，0x13c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[21 + 36] = 0x3e,//62，0x13e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[22 + 36] = 0x48,//72，0x140
		.cfg_ddr_training_delay_ps.read_dq_delay_t[23 + 36] = 0x3f,//63，0x142
		.cfg_ddr_training_delay_ps.read_dq_delay_t[24 + 36] = 0x3a,//58，0x144
		.cfg_ddr_training_delay_ps.read_dq_delay_t[25 + 36] = 0x37,//55，0x146
		.cfg_ddr_training_delay_ps.read_dq_delay_t[26 + 36] = 0x3a,//58，0x148
		.cfg_ddr_training_delay_ps.read_dq_delay_t[27 + 36] = 0x35,//53，0x14a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[28 + 36] = 0x3b,//59，0x14c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[29 + 36] = 0x45,//69，0x14e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[30 + 36] = 0x3a,//58，0x150
		.cfg_ddr_training_delay_ps.read_dq_delay_t[31 + 36] = 0x3f,//63，0x152
		.cfg_ddr_training_delay_ps.read_dq_delay_t[32 + 36] = 0x3a,//58，0x154
		.cfg_ddr_training_delay_ps.read_dq_delay_t[33 + 36] = 0x32,//50，0x156
		.cfg_ddr_training_delay_ps.read_dq_delay_t[34 + 36] = 0x44,//68，0x158
		.cfg_ddr_training_delay_ps.read_dq_delay_t[35 + 36] = 0x30,//48，0x15a
		.cfg_ddr_training_delay_ps.read_dq_delay_c[0] = 0x0,//0，0x1a4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[1] = 0x0,//0，0x1a6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[2] = 0x0,//0，0x1a8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[3] = 0x0,//0，0x1aa
		.cfg_ddr_training_delay_ps.read_dq_delay_c[4] = 0x0,//0，0x1ac
		.cfg_ddr_training_delay_ps.read_dq_delay_c[5] = 0x0,//0，0x1ae
		.cfg_ddr_training_delay_ps.read_dq_delay_c[6] = 0x0,//0，0x1b0
		.cfg_ddr_training_delay_ps.read_dq_delay_c[7] = 0x0,//0，0x1b2
		.cfg_ddr_training_delay_ps.read_dq_delay_c[8] = 0x0,//0，0x1b4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[9] = 0x0,//0，0x1b6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[10] = 0x0,//0，0x1b8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[11] = 0x0,//0，0x1ba
		.cfg_ddr_training_delay_ps.read_dq_delay_c[12] = 0x0,//0，0x1bc
		.cfg_ddr_training_delay_ps.read_dq_delay_c[13] = 0x0,//0，0x1be
		.cfg_ddr_training_delay_ps.read_dq_delay_c[14] = 0x0,//0，0x1c0
		.cfg_ddr_training_delay_ps.read_dq_delay_c[15] = 0x0,//0，0x1c2
		.cfg_ddr_training_delay_ps.read_dq_delay_c[16] = 0x0,//0，0x1c4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[17] = 0x0,//0，0x1c6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[18] = 0x0,//0，0x1c8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[19] = 0x0,//0，0x1ca
		.cfg_ddr_training_delay_ps.read_dq_delay_c[20] = 0x0,//0，0x1cc
		.cfg_ddr_training_delay_ps.read_dq_delay_c[21] = 0x0,//0，0x1ce
		.cfg_ddr_training_delay_ps.read_dq_delay_c[22] = 0x0,//0，0x1d0
		.cfg_ddr_training_delay_ps.read_dq_delay_c[23] = 0x0,//0，0x1d2
		.cfg_ddr_training_delay_ps.read_dq_delay_c[24] = 0x0,//0，0x1d4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[25] = 0x0,//0，0x1d6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[26] = 0x0,//0，0x1d8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[27] = 0x0,//0，0x1da
		.cfg_ddr_training_delay_ps.read_dq_delay_c[28] = 0x0,//0，0x1dc
		.cfg_ddr_training_delay_ps.read_dq_delay_c[29] = 0x0,//0，0x1de
		.cfg_ddr_training_delay_ps.read_dq_delay_c[30] = 0x0,//0，0x1e0
		.cfg_ddr_training_delay_ps.read_dq_delay_c[31] = 0x0,//0，0x1e2
		.cfg_ddr_training_delay_ps.read_dq_delay_c[32] = 0x0,//0，0x1e4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[33] = 0x0,//0，0x1e6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[34] = 0x0,//0，0x1e8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[35] = 0x0,//0，0x1ea
		.cfg_ddr_training_delay_ps.read_dq_delay_c[36] = 0x0,//0，0x1ec
		.cfg_ddr_training_delay_ps.read_dq_delay_c[37] = 0x0,//0，0x1ee
		.cfg_ddr_training_delay_ps.read_dq_delay_c[38] = 0x0,//0，0x1f0
		.cfg_ddr_training_delay_ps.read_dq_delay_c[39] = 0x0,//0，0x1f2
		.cfg_ddr_training_delay_ps.read_dq_delay_c[40] = 0x0,//0，0x1f4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[41] = 0x0,//0，0x1f6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[42] = 0x0,//0，0x1f8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[43] = 0x0,//0，0x1fa
		.cfg_ddr_training_delay_ps.read_dq_delay_c[44] = 0x0,//0，0x1fc
		.cfg_ddr_training_delay_ps.read_dq_delay_c[45] = 0x0,//0，0x1fe
		.cfg_ddr_training_delay_ps.read_dq_delay_c[46] = 0x0,//0，0x200
		.cfg_ddr_training_delay_ps.read_dq_delay_c[47] = 0x0,//0，0x202
		.cfg_ddr_training_delay_ps.read_dq_delay_c[48] = 0x0,//0，0x204
		.cfg_ddr_training_delay_ps.read_dq_delay_c[49] = 0x0,//0，0x206
		.cfg_ddr_training_delay_ps.read_dq_delay_c[50] = 0x0,//0，0x208
		.cfg_ddr_training_delay_ps.read_dq_delay_c[51] = 0x0,//0，0x20a
		.cfg_ddr_training_delay_ps.read_dq_delay_c[52] = 0x0,//0，0x20c
		.cfg_ddr_training_delay_ps.read_dq_delay_c[53] = 0x0,//0，0x20e
		.cfg_ddr_training_delay_ps.read_dq_delay_c[54] = 0x0,//0，0x210
		.cfg_ddr_training_delay_ps.read_dq_delay_c[55] = 0x0,//0，0x212
		.cfg_ddr_training_delay_ps.read_dq_delay_c[56] = 0x0,//0，0x214
		.cfg_ddr_training_delay_ps.read_dq_delay_c[57] = 0x0,//0，0x216
		.cfg_ddr_training_delay_ps.read_dq_delay_c[58] = 0x0,//0，0x218
		.cfg_ddr_training_delay_ps.read_dq_delay_c[59] = 0x0,//0，0x21a
		.cfg_ddr_training_delay_ps.read_dq_delay_c[60] = 0x0,//0，0x21c
		.cfg_ddr_training_delay_ps.read_dq_delay_c[61] = 0x0,//0，0x21e
		.cfg_ddr_training_delay_ps.read_dq_delay_c[62] = 0x0,//0，0x220
		.cfg_ddr_training_delay_ps.read_dq_delay_c[63] = 0x0,//0，0x222
		.cfg_ddr_training_delay_ps.read_dq_delay_c[64] = 0x0,//0，0x224
		.cfg_ddr_training_delay_ps.read_dq_delay_c[65] = 0x0,//0，0x226
		.cfg_ddr_training_delay_ps.read_dq_delay_c[66] = 0x0,//0，0x228
		.cfg_ddr_training_delay_ps.read_dq_delay_c[67] = 0x0,//0，0x22a
		.cfg_ddr_training_delay_ps.read_dq_delay_c[68] = 0x0,//0，0x22c
		.cfg_ddr_training_delay_ps.read_dq_delay_c[69] = 0x0,//0，0x22e
		.cfg_ddr_training_delay_ps.read_dq_delay_c[70] = 0x0,//0，0x230
		.cfg_ddr_training_delay_ps.read_dq_delay_c[71] = 0x0,//0，0x232
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x70,//112，0x234
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x70,//112，0x236
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x70,//112，0x238
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x70,//112，0x23a
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x70,//112，0x23c
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x70,//112，0x23e
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x70,//112，0x240
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x70,//112，0x242
		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x134,//308，0x244
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x10b,//267，0x246
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x10b,//267，0x248
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0xdd,//221，0x24a
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x100,//256，0x24c
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x100,//256，0x24e
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x100,//256，0x250
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x132,//306，0x252
		.cfg_ddr_training_delay_ps.write_wck_delay[0] = 0x0,//0，0x254
		.cfg_ddr_training_delay_ps.write_wck_delay[1] = 0x0,//0，0x256
		.cfg_ddr_training_delay_ps.write_wck_delay[2] = 0x0,//0，0x258
		.cfg_ddr_training_delay_ps.write_wck_delay[3] = 0x0,//0，0x25c
		.cfg_ddr_training_delay_ps.write_wck_delay[4] = 0x0,//0，0x25a
		.cfg_ddr_training_delay_ps.write_wck_delay[5] = 0x0,//0，0x25e
		.cfg_ddr_training_delay_ps.write_wck_delay[6] = 0x0,//0，0x260
		.cfg_ddr_training_delay_ps.write_wck_delay[7] = 0x0,//0，0x262
		.cfg_ddr_training_delay_ps.wdq_delay[0] = 0x151,//337，0x264
		.cfg_ddr_training_delay_ps.wdq_delay[1] = 0x166,//358，0x266
		.cfg_ddr_training_delay_ps.wdq_delay[2] = 0x158,//344，0x268
		.cfg_ddr_training_delay_ps.wdq_delay[3] = 0x15b,//347，0x26a
		.cfg_ddr_training_delay_ps.wdq_delay[4] = 0x166,//358，0x26c
		.cfg_ddr_training_delay_ps.wdq_delay[5] = 0x14e,//334，0x26e
		.cfg_ddr_training_delay_ps.wdq_delay[6] = 0x150,//336，0x270
		.cfg_ddr_training_delay_ps.wdq_delay[7] = 0x15f,//351，0x272
		.cfg_ddr_training_delay_ps.wdq_delay[8] = 0x166,//358，0x274
		.cfg_ddr_training_delay_ps.wdq_delay[9] = 0x134,//308，0x276
		.cfg_ddr_training_delay_ps.wdq_delay[10] = 0x135,//309，0x278
		.cfg_ddr_training_delay_ps.wdq_delay[11] = 0x13e,//318，0x27a
		.cfg_ddr_training_delay_ps.wdq_delay[12] = 0x13b,//315，0x27c
		.cfg_ddr_training_delay_ps.wdq_delay[13] = 0x13f,//319，0x27e
		.cfg_ddr_training_delay_ps.wdq_delay[14] = 0x143,//323，0x280
		.cfg_ddr_training_delay_ps.wdq_delay[15] = 0x140,//320，0x282
		.cfg_ddr_training_delay_ps.wdq_delay[16] = 0x13d,//317，0x284
		.cfg_ddr_training_delay_ps.wdq_delay[17] = 0x138,//312，0x286
		.cfg_ddr_training_delay_ps.wdq_delay[18] = 0x124,//292，0x288
		.cfg_ddr_training_delay_ps.wdq_delay[19] = 0x12f,//303，0x28a
		.cfg_ddr_training_delay_ps.wdq_delay[20] = 0x134,//308，0x28c
		.cfg_ddr_training_delay_ps.wdq_delay[21] = 0x12a,//298，0x28e
		.cfg_ddr_training_delay_ps.wdq_delay[22] = 0x135,//309，0x290
		.cfg_ddr_training_delay_ps.wdq_delay[23] = 0x12c,//300，0x292
		.cfg_ddr_training_delay_ps.wdq_delay[24] = 0x124,//292，0x294
		.cfg_ddr_training_delay_ps.wdq_delay[25] = 0x122,//290，0x296
		.cfg_ddr_training_delay_ps.wdq_delay[26] = 0x126,//294，0x298
		.cfg_ddr_training_delay_ps.wdq_delay[27] = 0xf0,//240，0x29a
		.cfg_ddr_training_delay_ps.wdq_delay[28] = 0xf7,//247，0x29c
		.cfg_ddr_training_delay_ps.wdq_delay[29] = 0x103,//259，0x29e
		.cfg_ddr_training_delay_ps.wdq_delay[30] = 0xf7,//247，0x2a0
		.cfg_ddr_training_delay_ps.wdq_delay[31] = 0x105,//261，0x2a2
		.cfg_ddr_training_delay_ps.wdq_delay[32] = 0xf6,//246，0x2a4
		.cfg_ddr_training_delay_ps.wdq_delay[33] = 0xef,//239，0x2a6
		.cfg_ddr_training_delay_ps.wdq_delay[34] = 0x100,//256，0x2a8
		.cfg_ddr_training_delay_ps.wdq_delay[35] = 0xe6,//230，0x2aa
		.cfg_ddr_training_delay_ps.wdq_delay[0 + 36] = 0x151,//337，0x264
		.cfg_ddr_training_delay_ps.wdq_delay[1 + 36] = 0x166,//358，0x266
		.cfg_ddr_training_delay_ps.wdq_delay[2 + 36] = 0x158,//344，0x268
		.cfg_ddr_training_delay_ps.wdq_delay[3 + 36] = 0x15b,//347，0x26a
		.cfg_ddr_training_delay_ps.wdq_delay[4 + 36] = 0x166,//358，0x26c
		.cfg_ddr_training_delay_ps.wdq_delay[5 + 36] = 0x14e,//334，0x26e
		.cfg_ddr_training_delay_ps.wdq_delay[6 + 36] = 0x150,//336，0x270
		.cfg_ddr_training_delay_ps.wdq_delay[7 + 36] = 0x15f,//351，0x272
		.cfg_ddr_training_delay_ps.wdq_delay[8 + 36] = 0x166,//358，0x274
		.cfg_ddr_training_delay_ps.wdq_delay[9 + 36] = 0x134,//308，0x276
		.cfg_ddr_training_delay_ps.wdq_delay[10 + 36] = 0x135,//309，0x278
		.cfg_ddr_training_delay_ps.wdq_delay[11 + 36] = 0x13e,//318，0x27a
		.cfg_ddr_training_delay_ps.wdq_delay[12 + 36] = 0x13b,//315，0x27c
		.cfg_ddr_training_delay_ps.wdq_delay[13 + 36] = 0x13f,//319，0x27e
		.cfg_ddr_training_delay_ps.wdq_delay[14 + 36] = 0x143,//323，0x280
		.cfg_ddr_training_delay_ps.wdq_delay[15 + 36] = 0x140,//320，0x282
		.cfg_ddr_training_delay_ps.wdq_delay[16 + 36] = 0x13d,//317，0x284
		.cfg_ddr_training_delay_ps.wdq_delay[17 + 36] = 0x138,//312，0x286
		.cfg_ddr_training_delay_ps.wdq_delay[18 + 36] = 0x124,//292，0x288
		.cfg_ddr_training_delay_ps.wdq_delay[19 + 36] = 0x12f,//303，0x28a
		.cfg_ddr_training_delay_ps.wdq_delay[20 + 36] = 0x134,//308，0x28c
		.cfg_ddr_training_delay_ps.wdq_delay[21 + 36] = 0x12a,//298，0x28e
		.cfg_ddr_training_delay_ps.wdq_delay[22 + 36] = 0x135,//309，0x290
		.cfg_ddr_training_delay_ps.wdq_delay[23 + 36] = 0x12c,//300，0x292
		.cfg_ddr_training_delay_ps.wdq_delay[24 + 36] = 0x124,//292，0x294
		.cfg_ddr_training_delay_ps.wdq_delay[25 + 36] = 0x122,//290，0x296
		.cfg_ddr_training_delay_ps.wdq_delay[26 + 36] = 0x126,//294，0x298
		.cfg_ddr_training_delay_ps.wdq_delay[27 + 36] = 0xf0,//240，0x29a
		.cfg_ddr_training_delay_ps.wdq_delay[28 + 36] = 0xf7,//247，0x29c
		.cfg_ddr_training_delay_ps.wdq_delay[29 + 36] = 0x103,//259，0x29e
		.cfg_ddr_training_delay_ps.wdq_delay[30 + 36] = 0xf7,//247，0x2a0
		.cfg_ddr_training_delay_ps.wdq_delay[31 + 36] = 0x105,//261，0x2a2
		.cfg_ddr_training_delay_ps.wdq_delay[32 + 36] = 0xf6,//246，0x2a4
		.cfg_ddr_training_delay_ps.wdq_delay[33 + 36] = 0xef,//239，0x2a6
		.cfg_ddr_training_delay_ps.wdq_delay[34 + 36] = 0x100,//256，0x2a8
		.cfg_ddr_training_delay_ps.wdq_delay[35 + 36] = 0xe6,//230，0x2aa
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0x124,//292，0x2f4
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0x132,//306，0x2f6
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0x12c,//300，0x2f8
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0x150,//336，0x2fa
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 0x140,//0，0x2fc
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 0x140,//0，0x2fe
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 0x140,//0，0x300
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 0x140,//0，0x302

	},
	{
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs = MAX_VALID_CONFIGS,
		//sizeof(__ddr_setting) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,
		.cfg_board_common_setting.fast_boot = {
			0, 0, 0, 0
		},
		.cfg_board_common_setting.ddr_func = 0,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_LPDDR4,
		.cfg_board_common_setting.enable_lpddr4x_mode = 2,
		//0  force lp4x   1 force lp4
		//2 auto 4x use nn 4 use pn drivere
		//3 auto + force 4 4x both use nn driver
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
		//.cfg_board_common_setting.dram_cs0_base_add = 0,
		//.cfg_board_common_setting.dram_cs1_base_add = 0,
		.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.DisabledDbyte[0] = 0xf0,
		//bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
		.cfg_board_common_setting.DisabledDbyte[1] = 0xf0,
		//bit 0 -3 ch1 cs0 ,bit 4-7 ch1
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		.cfg_board_common_setting.log_level = 4,
		//4,//LOG_LEVEL_BASIC,
		.cfg_board_common_setting.dbi_enable = 0,
		.cfg_board_common_setting.pll_ssc_mode =
			(1 << 20) | (1 << 8) | (2 << 4) | 0,
		//center_ssc_1000ppm,//SSC_DISABLE,(1 << 20) | (0 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test = {
			0
		},
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_LPDDR4_16BIT,
		//DDR_DMC_REMAP_DDR4_32BIT,
		// .cfg_board_common_setting.ac_pinmux = {
		//2, 3, 1, 0, 5, 4, 0, 0, 0, 0, 1, 3, 5, 2, 4, 0 },
		.cfg_board_common_setting.ddr_dqs_swap = 0,
		//.cfg_board_common_setting.ddr_dq_remap= { 23,
		//20,
		//21,
		//34,
		//16,
		//18,
		//17,
		//19,
		//22,
		//24,
		//28,
		//26,
		//35,
		//25,
		//29,
		//31,
		//30,
		//27,
		//7,
		//6,
		//0,
		//32,
		//1,
		//5,
		//3,
		//4,
		//2,
		//12,
		//14,
		//15,
		//33,
		//9,
		//11,
		//13,
		//8,
		//10, },
		.cfg_board_common_setting.ddr_vddee_setting = {
			0
		},
		.cfg_board_SI_setting_ps.DRAMFreq = CACLU_CLK_LP4,
		// .cfg_ddr_training_delay_ps.DRAMFreq = 600,// 2112,//,cfg_ddr_training_delay_ps
		//.cfg_ddr_training_delay_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		.cfg_board_SI_setting_ps.dfi_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		.cfg_board_SI_setting_ps.clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_0_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = DDR_DRAM_LPDDR4_DRV_40_OHM,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = DDR_DRAM_DDR_WR_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = 120, //240,
		.cfg_board_SI_setting_ps.dram_drv_pull_up_cal_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_range =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		//.cfg_ddr_training_delay_ps.dfe_offset = 0,
		.cfg_board_SI_setting_ps.vref_ac_permil = 260,//420,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,
		//.cfg_ddr_training_delay_ps.training_phase_parameter = {0},
		//.cfg_ddr_training_delay_ps.ac_trace_delay_org = {
		//	128, 128, 128 + 40, 128, 128, 128, 128, 128,
		//	384, 384, 384,      384, 384, 384, 384, 384,
		//	384, 384, 384,      384, 384, 384, 384, 384,
		//	384, 384, 384,      384, 384, 384, 384, 384,
		//	384, 384, 384,      384,
		//},
		//total 36
		.cfg_ddr_training_delay_ps.ac_trace_delay = {
			32 + 0, 32 + 0, 32 + 0, 32 + 0,
			32,	32 + 0, 32 + 0, 32 + 0,
			32 + 0, 32 + 0, 32 + 0, 32 + 0,
			32 + 0, 32 + 0, 32,	32 + 0,
			32 + 0, 32 + 0, 32 + 0, 32 + 0,
			32 + 0, 32 + 0, 32,	32 + 0
		},
// 1 //real chip stk lp4
		.cfg_board_common_setting.dbi_enable = 0x0000000,       // 0,0x00000041
		.cfg_board_common_setting.ddr_rfc_type = 0x0000000d,    // 13,0x00000042
		.cfg_board_common_setting.pll_ssc_mode = 0x00000000,    // 0,0x00000044
//.cfg_board_common_setting.max_core_timmming_frequency=0x00000e10,// 3600,0x0000006a

//.cfg_board_common_setting.lpddr4_x8_mode=0x00000000,// 0,0x00000087
//.cfg_board_common_setting.tdqs2dq=0x00000000,// 0,0x0000008a
//.cfg_board_common_setting.dfe_offset_value=0x00000000,// 0,0x0000008e
//.cfg_board_common_setting.training_offset=0x00000000,// 0,0x0000008f

		.cfg_board_SI_setting_ps.clk_drv_ohm = 40,
		.cfg_board_SI_setting_ps.cs_drv_ohm = 40,
		.cfg_board_SI_setting_ps.ac_drv_ohm = 40,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = 40,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = 40,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = 0,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = 40, //60,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = 40,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = 60,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = 0,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = 120,//240,//120,
		.cfg_board_SI_setting_ps.dram_drv_pull_up_cal_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_range =
			1,//0,//DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		//.cfg_ddr_training_delay_ps.dfe_offset = 0,
		.cfg_board_common_setting.ac_pinmux = {
			1,  0,	2,  3,	4,  5,	8,  9,	29,
			28, 0,	7,  0,	6,  0,
			14, 15, 13, 12, 11, 10, 18, 19, 27,
			26, 16, 17, 0,	0,  0,
		},
		.cfg_ddr_training_delay_ps.rx_offset[0] = (1 << 7) | 0x10,
		.cfg_ddr_training_delay_ps.reserve_para = {//have higher prior over rx_offset
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs0 write dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs1 write dqs,lane0-lane3
			(1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10,
			//cs0 read dqs,lane0-lane3
			(1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10,
			//cs1 read dqs,lane0-lane3
		},
		.cfg_ddr_training_delay_ps.tx_offset[0] = (1 << 7) | 0x8,
		//.cfg_ddr_training_delay_ps.dac_offset[0] = 0,//(1 << 7) | 0x10,
		//.cfg_ddr_training_delay_ps.dac_offset[1] = 0,//(0 << 7) | 0x10,
		.cfg_ddr_training_delay_ps.dac_offset[0] = (1 << 7) | 0x0,
		.cfg_ddr_training_delay_ps.dac_offset[1] = (0 << 7) | 0x0,
		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 256,     //clk
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 256,     //clk
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 256,    //clk
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 256,    //clk
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 0xb1,//177，0xd8
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 0xb6,//182，0xda
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 0x93,//147，0xdc
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 0x90,//144，0xde
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 0x7d,//125，0xe0
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 0x90,//144，0xe2
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 0x0,//0，0xe4
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 0x0,//0，0xe6
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 0x1bf,//447，0xe8
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 0x1bf,//447，0xea
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 0xb6,//182，0xec
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 0xbf,//191，0xee
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 0xb6,//182，0xf0
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 0xbf,//191，0xf2
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 0xb6,//182，0xf4
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 0x7d,//125，0xf6
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 0x90,//144，0xf8
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 0x90,//144，0xfa
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 0x93,//147，0xfc
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 0xb1,//177，0xfe
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 0xb6,//182，0x100
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 0x0,//0，0x102
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 0x0,//0，0x104
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 0x1bf,//447，0x106
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 0x1bf,//447，0x108
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 0xbf,//191，0x10a
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 0xbf,//191，0x10c
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 0xb6,//182，0x10e
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 0xb6,//182，0x110
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 0xb6,//182，0x112
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
		//clk_delay+128+wl_offset*128
		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 128,//384 - 64,
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 128,//384 - 64,
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 128,//384 - 64,
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 128,//384 - 64,
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 128,//384 - 64,
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 128,//384 - 64,
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 128,//384 - 64,
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 128,//384 - 64,
		.cfg_ddr_training_delay_ps.write_wck_delay[0] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[1] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[2] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[3] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[4] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[5] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[6] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[7] = 0x00000200,
		.cfg_ddr_training_delay_ps.wdq_delay[0] = 384 - 64 + 64 + TDQS2DQ_LP4,
		//write dqs+64+tdqs2dq
		.cfg_ddr_training_delay_ps.wdq_delay[1] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[2] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[3] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[4] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[5] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[6] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[7] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[8] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[9] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[10] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[11] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[12] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[13] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[14] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[15] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[16] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[17] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[18] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[19] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[20] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[21] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[22] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[23] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[24] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[25] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[26] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[27] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[28] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[29] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[30] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[31] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[32] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[33] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[34] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[35] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[36] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[37] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[38] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[39] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[40] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[41] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[42] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[43] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[44] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[45] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[46] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[47] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[48] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[49] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[50] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[51] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[52] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[53] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[54] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[55] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[56] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[57] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[58] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[59] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[60] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[61] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[62] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[63] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[64] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[65] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[66] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[67] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[68] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[69] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[70] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.wdq_delay[71] = 384 - 64 + 64 + TDQS2DQ_LP4,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 512,
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 128,
		.cfg_ddr_training_delay_ps.soc_bit_vref0[0] = 0x0000001a,
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

		.cfg_ddr_training_delay_ps.read_dq_delay_t[0] = 0x38,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[1] = 0x48,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[2] = 0x37,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[3] = 0x42,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[4] = 0x4a,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[5] = 0x49,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[6] = 0x4c,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[7] = 0x49,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[8] = 0x34,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[9] = 0x48,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[10] = 0x4f,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[11] = 0x44,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[12] = 0x41,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[13] = 0x4e,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[14] = 0x33,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[15] = 0x33,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[16] = 0x3d,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[17] = 0x3d,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[18] = 0x33,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[19] = 0x4b,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[20] = 0x4c,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[21] = 0x43,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[22] = 0x4d,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[23] = 0x49,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[24] = 0x42,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[25] = 0x40,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[26] = 0x36,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[27] = 0x54,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[28] = 0x50,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[29] = 0x56,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[30] = 0x5a,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[31] = 0x5d,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[32] = 0x57,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[33] = 0x56,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[34] = 0x70,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[35] = 0x63,

		.cfg_ddr_training_delay_ps.read_dq_delay_t[0 + 36] = 0x38,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[1 + 36] = 0x48,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[2 + 36] = 0x37,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[3 + 36] = 0x42,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[4 + 36] = 0x4a,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[5 + 36] = 0x49,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[6 + 36] = 0x4c,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[7 + 36] = 0x49,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[8 + 36] = 0x34,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[9 + 36] = 0x48,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[10 + 36] = 0x4f,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[11 + 36] = 0x44,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[12 + 36] = 0x41,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[13 + 36] = 0x4e,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[14 + 36] = 0x33,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[15 + 36] = 0x33,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[16 + 36] = 0x3d,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[17 + 36] = 0x3d,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[18 + 36] = 0x33,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[19 + 36] = 0x4b,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[20 + 36] = 0x4c,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[21 + 36] = 0x43,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[22 + 36] = 0x4d,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[23 + 36] = 0x49,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[24 + 36] = 0x42,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[25 + 36] = 0x40,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[26 + 36] = 0x36,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[27 + 36] = 0x54,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[28 + 36] = 0x50,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[29 + 36] = 0x56,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[30 + 36] = 0x5a,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[31 + 36] = 0x5d,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[32 + 36] = 0x57,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[33 + 36] = 0x56,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[34 + 36] = 0x70,
		.cfg_ddr_training_delay_ps.read_dq_delay_t[35 + 36] = 0x63,

		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x80,
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x80,
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x80,
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x80,
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x80,
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x80,
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x80,
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x80,

		//cxmt
		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 0xb1,//177，0xd8
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 0xb6,//182，0xda
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 0x93,//147，0xdc
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 0x90,//144，0xde
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 0x7d,//125，0xe0
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 0x90,//144，0xe2
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 0x0,//0，0xe4
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 0x0,//0，0xe6
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 0x1bf,//447，0xe8
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 0x1bf,//447，0xea
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 0xb6,//182，0xec
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 0xbf,//191，0xee
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 0xb6,//182，0xf0
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 0xbf,//191，0xf2
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 0xb6,//182，0xf4
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 0x7d,//125，0xf6
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 0x90,//144，0xf8
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 0x90,//144，0xfa
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 0x93,//147，0xfc
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 0xb1,//177，0xfe
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 0xb6,//182，0x100
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 0x0,//0，0x102
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 0x0,//0，0x104
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 0x1bf,//447，0x106
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 0x1bf,//447，0x108
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 0xbf,//191，0x10a
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 0xbf,//191，0x10c
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 0xb6,//182，0x10e
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 0xb6,//182，0x110
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 0xb6,//182，0x112
		.cfg_ddr_training_delay_ps.read_dq_delay_t[0] = 0x3c,//60，0x114
		.cfg_ddr_training_delay_ps.read_dq_delay_t[1] = 0x38,//56，0x116
		.cfg_ddr_training_delay_ps.read_dq_delay_t[2] = 0x2f,//47，0x118
		.cfg_ddr_training_delay_ps.read_dq_delay_t[3] = 0x39,//57，0x11a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[4] = 0x38,//56，0x11c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[5] = 0x3f,//63，0x11e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[6] = 0x40,//64，0x120
		.cfg_ddr_training_delay_ps.read_dq_delay_t[7] = 0x3b,//59，0x122
		.cfg_ddr_training_delay_ps.read_dq_delay_t[8] = 0x33,//51，0x124
		.cfg_ddr_training_delay_ps.read_dq_delay_t[9] = 0x3c,//60，0x126
		.cfg_ddr_training_delay_ps.read_dq_delay_t[10] = 0x3d,//61，0x128
		.cfg_ddr_training_delay_ps.read_dq_delay_t[11] = 0x38,//56，0x12a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[12] = 0x36,//54，0x12c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[13] = 0x3e,//62，0x12e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[14] = 0x30,//48，0x130
		.cfg_ddr_training_delay_ps.read_dq_delay_t[15] = 0x2f,//47，0x132
		.cfg_ddr_training_delay_ps.read_dq_delay_t[16] = 0x34,//52，0x134
		.cfg_ddr_training_delay_ps.read_dq_delay_t[17] = 0x35,//53，0x136
		.cfg_ddr_training_delay_ps.read_dq_delay_t[18] = 0x37,//55，0x138
		.cfg_ddr_training_delay_ps.read_dq_delay_t[19] = 0x49,//73，0x13a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[20] = 0x4a,//74，0x13c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[21] = 0x3d,//61，0x13e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[22] = 0x47,//71，0x140
		.cfg_ddr_training_delay_ps.read_dq_delay_t[23] = 0x39,//57，0x142
		.cfg_ddr_training_delay_ps.read_dq_delay_t[24] = 0x38,//56，0x144
		.cfg_ddr_training_delay_ps.read_dq_delay_t[25] = 0x37,//55，0x146
		.cfg_ddr_training_delay_ps.read_dq_delay_t[26] = 0x35,//53，0x148
		.cfg_ddr_training_delay_ps.read_dq_delay_t[27] = 0x48,//72，0x14a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[28] = 0x45,//69，0x14c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[29] = 0x46,//70，0x14e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[30] = 0x4d,//77，0x150
		.cfg_ddr_training_delay_ps.read_dq_delay_t[31] = 0x54,//84，0x152
		.cfg_ddr_training_delay_ps.read_dq_delay_t[32] = 0x4b,//75，0x154
		.cfg_ddr_training_delay_ps.read_dq_delay_t[33] = 0x48,//72，0x156
		.cfg_ddr_training_delay_ps.read_dq_delay_t[34] = 0x56,//86，0x158
		.cfg_ddr_training_delay_ps.read_dq_delay_t[35] = 0x59,//89，0x15a

		.cfg_ddr_training_delay_ps.read_dq_delay_t[0 + 36] = 0x3c,//60，0x114
		.cfg_ddr_training_delay_ps.read_dq_delay_t[1 + 36] = 0x38,//56，0x116
		.cfg_ddr_training_delay_ps.read_dq_delay_t[2 + 36] = 0x2f,//47，0x118
		.cfg_ddr_training_delay_ps.read_dq_delay_t[3 + 36] = 0x39,//57，0x11a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[4 + 36] = 0x38,//56，0x11c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[5 + 36] = 0x3f,//63，0x11e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[6 + 36] = 0x40,//64，0x120
		.cfg_ddr_training_delay_ps.read_dq_delay_t[7 + 36] = 0x3b,//59，0x122
		.cfg_ddr_training_delay_ps.read_dq_delay_t[8 + 36] = 0x33,//51，0x124
		.cfg_ddr_training_delay_ps.read_dq_delay_t[9 + 36] = 0x3c,//60，0x126
		.cfg_ddr_training_delay_ps.read_dq_delay_t[10 + 36] = 0x3d,//61，0x128
		.cfg_ddr_training_delay_ps.read_dq_delay_t[11 + 36] = 0x38,//56，0x12a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[12 + 36] = 0x36,//54，0x12c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[13 + 36] = 0x3e,//62，0x12e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[14 + 36] = 0x30,//48，0x130
		.cfg_ddr_training_delay_ps.read_dq_delay_t[15 + 36] = 0x2f,//47，0x132
		.cfg_ddr_training_delay_ps.read_dq_delay_t[16 + 36] = 0x34,//52，0x134
		.cfg_ddr_training_delay_ps.read_dq_delay_t[17 + 36] = 0x35,//53，0x136
		.cfg_ddr_training_delay_ps.read_dq_delay_t[18 + 36] = 0x37,//55，0x138
		.cfg_ddr_training_delay_ps.read_dq_delay_t[19 + 36] = 0x49,//73，0x13a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[20 + 36] = 0x4a,//74，0x13c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[21 + 36] = 0x3d,//61，0x13e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[22 + 36] = 0x47,//71，0x140
		.cfg_ddr_training_delay_ps.read_dq_delay_t[23 + 36] = 0x39,//57，0x142
		.cfg_ddr_training_delay_ps.read_dq_delay_t[24 + 36] = 0x38,//56，0x144
		.cfg_ddr_training_delay_ps.read_dq_delay_t[25 + 36] = 0x37,//55，0x146
		.cfg_ddr_training_delay_ps.read_dq_delay_t[26 + 36] = 0x35,//53，0x148
		.cfg_ddr_training_delay_ps.read_dq_delay_t[27 + 36] = 0x48,//72，0x14a
		.cfg_ddr_training_delay_ps.read_dq_delay_t[28 + 36] = 0x45,//69，0x14c
		.cfg_ddr_training_delay_ps.read_dq_delay_t[29 + 36] = 0x46,//70，0x14e
		.cfg_ddr_training_delay_ps.read_dq_delay_t[30 + 36] = 0x4d,//77，0x150
		.cfg_ddr_training_delay_ps.read_dq_delay_t[31 + 36] = 0x54,//84，0x152
		.cfg_ddr_training_delay_ps.read_dq_delay_t[32 + 36] = 0x4b,//75，0x154
		.cfg_ddr_training_delay_ps.read_dq_delay_t[33 + 36] = 0x48,//72，0x156
		.cfg_ddr_training_delay_ps.read_dq_delay_t[34 + 36] = 0x56,//86，0x158
		.cfg_ddr_training_delay_ps.read_dq_delay_t[35 + 36] = 0x59,//89，0x15a
		.cfg_ddr_training_delay_ps.read_dq_delay_c[0] = 0x0,//0，0x1a4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[1] = 0x0,//0，0x1a6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[2] = 0x0,//0，0x1a8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[3] = 0x0,//0，0x1aa
		.cfg_ddr_training_delay_ps.read_dq_delay_c[4] = 0x0,//0，0x1ac
		.cfg_ddr_training_delay_ps.read_dq_delay_c[5] = 0x0,//0，0x1ae
		.cfg_ddr_training_delay_ps.read_dq_delay_c[6] = 0x0,//0，0x1b0
		.cfg_ddr_training_delay_ps.read_dq_delay_c[7] = 0x0,//0，0x1b2
		.cfg_ddr_training_delay_ps.read_dq_delay_c[8] = 0x0,//0，0x1b4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[9] = 0x0,//0，0x1b6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[10] = 0x0,//0，0x1b8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[11] = 0x0,//0，0x1ba
		.cfg_ddr_training_delay_ps.read_dq_delay_c[12] = 0x0,//0，0x1bc
		.cfg_ddr_training_delay_ps.read_dq_delay_c[13] = 0x0,//0，0x1be
		.cfg_ddr_training_delay_ps.read_dq_delay_c[14] = 0x0,//0，0x1c0
		.cfg_ddr_training_delay_ps.read_dq_delay_c[15] = 0x0,//0，0x1c2
		.cfg_ddr_training_delay_ps.read_dq_delay_c[16] = 0x0,//0，0x1c4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[17] = 0x0,//0，0x1c6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[18] = 0x0,//0，0x1c8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[19] = 0x0,//0，0x1ca
		.cfg_ddr_training_delay_ps.read_dq_delay_c[20] = 0x0,//0，0x1cc
		.cfg_ddr_training_delay_ps.read_dq_delay_c[21] = 0x0,//0，0x1ce
		.cfg_ddr_training_delay_ps.read_dq_delay_c[22] = 0x0,//0，0x1d0
		.cfg_ddr_training_delay_ps.read_dq_delay_c[23] = 0x0,//0，0x1d2
		.cfg_ddr_training_delay_ps.read_dq_delay_c[24] = 0x0,//0，0x1d4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[25] = 0x0,//0，0x1d6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[26] = 0x0,//0，0x1d8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[27] = 0x0,//0，0x1da
		.cfg_ddr_training_delay_ps.read_dq_delay_c[28] = 0x0,//0，0x1dc
		.cfg_ddr_training_delay_ps.read_dq_delay_c[29] = 0x0,//0，0x1de
		.cfg_ddr_training_delay_ps.read_dq_delay_c[30] = 0x0,//0，0x1e0
		.cfg_ddr_training_delay_ps.read_dq_delay_c[31] = 0x0,//0，0x1e2
		.cfg_ddr_training_delay_ps.read_dq_delay_c[32] = 0x0,//0，0x1e4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[33] = 0x0,//0，0x1e6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[34] = 0x0,//0，0x1e8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[35] = 0x0,//0，0x1ea
		.cfg_ddr_training_delay_ps.read_dq_delay_c[36] = 0x0,//0，0x1ec
		.cfg_ddr_training_delay_ps.read_dq_delay_c[37] = 0x0,//0，0x1ee
		.cfg_ddr_training_delay_ps.read_dq_delay_c[38] = 0x0,//0，0x1f0
		.cfg_ddr_training_delay_ps.read_dq_delay_c[39] = 0x0,//0，0x1f2
		.cfg_ddr_training_delay_ps.read_dq_delay_c[40] = 0x0,//0，0x1f4
		.cfg_ddr_training_delay_ps.read_dq_delay_c[41] = 0x0,//0，0x1f6
		.cfg_ddr_training_delay_ps.read_dq_delay_c[42] = 0x0,//0，0x1f8
		.cfg_ddr_training_delay_ps.read_dq_delay_c[43] = 0x0,//0，0x1fa
		.cfg_ddr_training_delay_ps.read_dq_delay_c[44] = 0x0,//0，0x1fc
		.cfg_ddr_training_delay_ps.read_dq_delay_c[45] = 0x0,//0，0x1fe
		.cfg_ddr_training_delay_ps.read_dq_delay_c[46] = 0x0,//0，0x200
		.cfg_ddr_training_delay_ps.read_dq_delay_c[47] = 0x0,//0，0x202
		.cfg_ddr_training_delay_ps.read_dq_delay_c[48] = 0x0,//0，0x204
		.cfg_ddr_training_delay_ps.read_dq_delay_c[49] = 0x0,//0，0x206
		.cfg_ddr_training_delay_ps.read_dq_delay_c[50] = 0x0,//0，0x208
		.cfg_ddr_training_delay_ps.read_dq_delay_c[51] = 0x0,//0，0x20a
		.cfg_ddr_training_delay_ps.read_dq_delay_c[52] = 0x0,//0，0x20c
		.cfg_ddr_training_delay_ps.read_dq_delay_c[53] = 0x0,//0，0x20e
		.cfg_ddr_training_delay_ps.read_dq_delay_c[54] = 0x0,//0，0x210
		.cfg_ddr_training_delay_ps.read_dq_delay_c[55] = 0x0,//0，0x212
		.cfg_ddr_training_delay_ps.read_dq_delay_c[56] = 0x0,//0，0x214
		.cfg_ddr_training_delay_ps.read_dq_delay_c[57] = 0x0,//0，0x216
		.cfg_ddr_training_delay_ps.read_dq_delay_c[58] = 0x0,//0，0x218
		.cfg_ddr_training_delay_ps.read_dq_delay_c[59] = 0x0,//0，0x21a
		.cfg_ddr_training_delay_ps.read_dq_delay_c[60] = 0x0,//0，0x21c
		.cfg_ddr_training_delay_ps.read_dq_delay_c[61] = 0x0,//0，0x21e
		.cfg_ddr_training_delay_ps.read_dq_delay_c[62] = 0x0,//0，0x220
		.cfg_ddr_training_delay_ps.read_dq_delay_c[63] = 0x0,//0，0x222
		.cfg_ddr_training_delay_ps.read_dq_delay_c[64] = 0x0,//0，0x224
		.cfg_ddr_training_delay_ps.read_dq_delay_c[65] = 0x0,//0，0x226
		.cfg_ddr_training_delay_ps.read_dq_delay_c[66] = 0x0,//0，0x228
		.cfg_ddr_training_delay_ps.read_dq_delay_c[67] = 0x0,//0，0x22a
		.cfg_ddr_training_delay_ps.read_dq_delay_c[68] = 0x0,//0，0x22c
		.cfg_ddr_training_delay_ps.read_dq_delay_c[69] = 0x0,//0，0x22e
		.cfg_ddr_training_delay_ps.read_dq_delay_c[70] = 0x0,//0，0x230
		.cfg_ddr_training_delay_ps.read_dq_delay_c[71] = 0x0,//0，0x232
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x70,//112，0x234
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x70,//112，0x236
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x70,//112，0x238
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x70,//112，0x23a
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x70,//112，0x23c
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x70,//112，0x23e
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x70,//112，0x240
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x70,//112，0x242
		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x136,//310，0x244
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x10e,//270，0x246
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x142,//322，0x248
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0xea,//234，0x24a
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x100,//256，0x24c
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x100,//256，0x24e
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x100,//256，0x250
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x128,//296，0x252
		.cfg_ddr_training_delay_ps.write_wck_delay[0] = 0x0,//0，0x254
		.cfg_ddr_training_delay_ps.write_wck_delay[1] = 0x0,//0，0x256
		.cfg_ddr_training_delay_ps.write_wck_delay[2] = 0x0,//0，0x258
		.cfg_ddr_training_delay_ps.write_wck_delay[3] = 0x0,//0，0x25a
		.cfg_ddr_training_delay_ps.write_wck_delay[4] = 0x0,//0，0x25c
		.cfg_ddr_training_delay_ps.write_wck_delay[5] = 0x0,//0，0x25e
		.cfg_ddr_training_delay_ps.write_wck_delay[6] = 0x0,//0，0x260
		.cfg_ddr_training_delay_ps.write_wck_delay[7] = 0x0,//0，0x262
		.cfg_ddr_training_delay_ps.wdq_delay[0] = 0x15b,//347，0x264
		.cfg_ddr_training_delay_ps.wdq_delay[1] = 0x15c,//348，0x266
		.cfg_ddr_training_delay_ps.wdq_delay[2] = 0x14a,//330，0x268
		.cfg_ddr_training_delay_ps.wdq_delay[3] = 0x159,//345，0x26a
		.cfg_ddr_training_delay_ps.wdq_delay[4] = 0x162,//354，0x26c
		.cfg_ddr_training_delay_ps.wdq_delay[5] = 0x160,//352，0x26e
		.cfg_ddr_training_delay_ps.wdq_delay[6] = 0x160,//352，0x270
		.cfg_ddr_training_delay_ps.wdq_delay[7] = 0x15b,//347，0x272
		.cfg_ddr_training_delay_ps.wdq_delay[8] = 0x151,//337，0x274
		.cfg_ddr_training_delay_ps.wdq_delay[9] = 0x13b,//315，0x276
		.cfg_ddr_training_delay_ps.wdq_delay[10] = 0x139,//313，0x278
		.cfg_ddr_training_delay_ps.wdq_delay[11] = 0x134,//308，0x27a
		.cfg_ddr_training_delay_ps.wdq_delay[12] = 0x132,//306，0x27c
		.cfg_ddr_training_delay_ps.wdq_delay[13] = 0x139,//313，0x27e
		.cfg_ddr_training_delay_ps.wdq_delay[14] = 0x126,//294，0x280
		.cfg_ddr_training_delay_ps.wdq_delay[15] = 0x12b,//299，0x282
		.cfg_ddr_training_delay_ps.wdq_delay[16] = 0x130,//304，0x284
		.cfg_ddr_training_delay_ps.wdq_delay[17] = 0x12f,//303，0x286
		.cfg_ddr_training_delay_ps.wdq_delay[18] = 0x14f,//335，0x288
		.cfg_ddr_training_delay_ps.wdq_delay[19] = 0x161,//353，0x28a
		.cfg_ddr_training_delay_ps.wdq_delay[20] = 0x168,//360，0x28c
		.cfg_ddr_training_delay_ps.wdq_delay[21] = 0x15a,//346，0x28e
		.cfg_ddr_training_delay_ps.wdq_delay[22] = 0x166,//358，0x290
		.cfg_ddr_training_delay_ps.wdq_delay[23] = 0x15c,//348，0x292
		.cfg_ddr_training_delay_ps.wdq_delay[24] = 0x157,//343，0x294
		.cfg_ddr_training_delay_ps.wdq_delay[25] = 0x153,//339，0x296
		.cfg_ddr_training_delay_ps.wdq_delay[26] = 0x14e,//334，0x298
		.cfg_ddr_training_delay_ps.wdq_delay[27] = 0x10e,//270，0x29a
		.cfg_ddr_training_delay_ps.wdq_delay[28] = 0x10e,//270，0x29c
		.cfg_ddr_training_delay_ps.wdq_delay[29] = 0x113,//275，0x29e
		.cfg_ddr_training_delay_ps.wdq_delay[30] = 0x115,//277，0x2a0
		.cfg_ddr_training_delay_ps.wdq_delay[31] = 0x11a,//282，0x2a2
		.cfg_ddr_training_delay_ps.wdq_delay[32] = 0x113,//275，0x2a4
		.cfg_ddr_training_delay_ps.wdq_delay[33] = 0x10f,//271，0x2a6
		.cfg_ddr_training_delay_ps.wdq_delay[34] = 0x121,//289，0x2a8
		.cfg_ddr_training_delay_ps.wdq_delay[35] = 0x11c,//284，0x2aa

		.cfg_ddr_training_delay_ps.wdq_delay[0 + 36] = 0x15b,//347，0x264
		.cfg_ddr_training_delay_ps.wdq_delay[1 + 36] = 0x15c,//348，0x266
		.cfg_ddr_training_delay_ps.wdq_delay[2 + 36] = 0x14a,//330，0x268
		.cfg_ddr_training_delay_ps.wdq_delay[3 + 36] = 0x159,//345，0x26a
		.cfg_ddr_training_delay_ps.wdq_delay[4 + 36] = 0x162,//354，0x26c
		.cfg_ddr_training_delay_ps.wdq_delay[5 + 36] = 0x160,//352，0x26e
		.cfg_ddr_training_delay_ps.wdq_delay[6 + 36] = 0x160,//352，0x270
		.cfg_ddr_training_delay_ps.wdq_delay[7 + 36] = 0x15b,//347，0x272
		.cfg_ddr_training_delay_ps.wdq_delay[8 + 36] = 0x151,//337，0x274
		.cfg_ddr_training_delay_ps.wdq_delay[9 + 36] = 0x13b,//315，0x276
		.cfg_ddr_training_delay_ps.wdq_delay[10 + 36] = 0x139,//313，0x278
		.cfg_ddr_training_delay_ps.wdq_delay[11 + 36] = 0x134,//308，0x27a
		.cfg_ddr_training_delay_ps.wdq_delay[12 + 36] = 0x132,//306，0x27c
		.cfg_ddr_training_delay_ps.wdq_delay[13 + 36] = 0x139,//313，0x27e
		.cfg_ddr_training_delay_ps.wdq_delay[14 + 36] = 0x126,//294，0x280
		.cfg_ddr_training_delay_ps.wdq_delay[15 + 36] = 0x12b,//299，0x282
		.cfg_ddr_training_delay_ps.wdq_delay[16 + 36] = 0x130,//304，0x284
		.cfg_ddr_training_delay_ps.wdq_delay[17 + 36] = 0x12f,//303，0x286
		.cfg_ddr_training_delay_ps.wdq_delay[18 + 36] = 0x14f,//335，0x288
		.cfg_ddr_training_delay_ps.wdq_delay[19 + 36] = 0x161,//353，0x28a
		.cfg_ddr_training_delay_ps.wdq_delay[20 + 36] = 0x168,//360，0x28c
		.cfg_ddr_training_delay_ps.wdq_delay[21 + 36] = 0x15a,//346，0x28e
		.cfg_ddr_training_delay_ps.wdq_delay[22 + 36] = 0x166,//358，0x290
		.cfg_ddr_training_delay_ps.wdq_delay[23 + 36] = 0x15c,//348，0x292
		.cfg_ddr_training_delay_ps.wdq_delay[24 + 36] = 0x157,//343，0x294
		.cfg_ddr_training_delay_ps.wdq_delay[25 + 36] = 0x153,//339，0x296
		.cfg_ddr_training_delay_ps.wdq_delay[26 + 36] = 0x14e,//334，0x298
		.cfg_ddr_training_delay_ps.wdq_delay[27 + 36] = 0x10e,//270，0x29a
		.cfg_ddr_training_delay_ps.wdq_delay[28 + 36] = 0x10e,//270，0x29c
		.cfg_ddr_training_delay_ps.wdq_delay[29 + 36] = 0x113,//275，0x29e
		.cfg_ddr_training_delay_ps.wdq_delay[30 + 36] = 0x115,//277，0x2a0
		.cfg_ddr_training_delay_ps.wdq_delay[31 + 36] = 0x11a,//282，0x2a2
		.cfg_ddr_training_delay_ps.wdq_delay[32 + 36] = 0x113,//275，0x2a4
		.cfg_ddr_training_delay_ps.wdq_delay[33 + 36] = 0x10f,//271，0x2a6
		.cfg_ddr_training_delay_ps.wdq_delay[34 + 36] = 0x121,//289，0x2a8
		.cfg_ddr_training_delay_ps.wdq_delay[35 + 36] = 0x11c,//284，0x2aa

		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0x12a,//298，0x2f4
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0x12d,//301，0x2f6
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0x12c,//300，0x2f8
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0x171,//369，0x2fa
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 0x120,//0，0x2fc
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 0x120,//0，0x2fe
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 0x120,//0，0x300
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 0x120,//0，0x302
//#endif
	},
#endif
//};

#define  CACLU_CLK_DDR4    1584 //1792//600 //1200 //(1900)// (1440)//(1008)
//ddr_set_ps0_only_t ddr_set_t_default[2] = {
//ddr_set_ps0_only_t __ddr_setting[]
//__attribute__ ((section(".ddr_param"))) = {
#if S5_DDR4_SKT
	{
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs = MAX_VALID_CONFIGS,
		//sizeof(__ddr_setting) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,
		.cfg_board_common_setting.fast_boot = {
			0, 0, 0, 0
		},
		.cfg_board_common_setting.ddr_func = 0,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_DDR4,
		.cfg_board_common_setting.enable_lpddr4x_mode = 0,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
		//.cfg_board_common_setting.dram_cs0_base_add = 0,
		//.cfg_board_common_setting.dram_cs1_base_add = 0,
		.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.DisabledDbyte[0] = 0xf0,
		//bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
		.cfg_board_common_setting.DisabledDbyte[1] = 0xf0,
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
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR4_32BIT,
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
		.cfg_board_SI_setting_ps.DRAMFreq = CACLU_CLK_DDR4,
		// .cfg_ddr_training_delay_ps.DRAMFreq = 600,
		// 2112,//1176,cfg_ddr_training_delay_ps
		//.cfg_ddr_training_delay_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		.cfg_board_SI_setting_ps.dfi_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		.cfg_board_SI_setting_ps.clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = DDR_DRAM_LPDDR4_DRV_40_OHM,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = DDR_DRAM_DDR_WR_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = 120,      //240,
		.cfg_board_SI_setting_ps.dram_drv_pull_up_cal_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_range =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		//.cfg_ddr_training_delay_ps.dfe_offset = 0,
		.cfg_board_SI_setting_ps.vref_ac_permil = 0,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,
		//.cfg_ddr_training_delay_ps.training_phase_parameter = {0},
		//.cfg_ddr_training_delay_ps.ac_trace_delay_org = {
		//	128, 128, 128 + 40, 128, 128, 128, 128, 128,
		//	384, 384, 384,      384, 384, 384, 384, 384,
		//	384, 384, 384,      384, 384, 384, 384, 384,
		//	384, 384, 384,      384, 384, 384, 384, 384,
		//	384, 384, 384,      384,
		//},
		//total 36
		.cfg_ddr_training_delay_ps.ac_trace_delay = {
			32 + 0, 32 + 0, 32 + 0, 32 + 0,
			32,	32 + 0, 32 + 0, 32 + 0,
			32 + 0, 32 + 0, 32 + 0, 32 + 0,
			32 + 0, 32 + 0, 32,	32 + 0,
			32 + 0, 32 + 0, 32 + 0, 32 + 0,
			32 + 0, 32 + 0, 32,	32 + 0
		},
// 1 //real chip stk lp4
		.cfg_board_common_setting.dbi_enable = 0x00000000,
		.cfg_board_common_setting.ddr_rfc_type = 0x0000000d,
		.cfg_board_common_setting.pll_ssc_mode = 0x00000000,
//.cfg_board_common_setting.max_core_timmming_frequency=0x00000e10,// 3600,0x0000006a

//.cfg_board_common_setting.lpddr4_x8_mode=0x00000000,// 0,0x00000087
//.cfg_board_common_setting.tdqs2dq=0x00000000,// 0,0x0000008a
//.cfg_board_common_setting.dfe_offset_value=0x00000000,// 0,0x0000008e
//.cfg_board_common_setting.training_offset=0x00000000,// 0,0x0000008f

#define W_LEV  ((0 * 128 * CACLU_CLK_DDR4 * 2) / 1000000)
//#define W_LEV  ((480 * 128 * CACLU_CLK_DDR4 * 2) / 1000000)
#ifdef CONFIG_PXP_TIMMING
//pxp
#define TDQS2DQ  (0 + ((0 * 128 * CACLU_CLK_DDR4 * 2) / 1000000))      //
//#define TDQSCK   64
#define CLK_DELAY 0                                             // (64)
//#define BOARD_DQS_DELAY   64
#define TDQSCK_ADD_DQS   64                                     //128   //clk should use 64 steps
#define PHY_TDQS2DQ  0
#endif
#ifdef CONFIG_RTL_TIMMING
//rtl
#define TDQS2DQ  ((0 * 128 * CACLU_CLK_DDR4) / 1000000)  //
//#define TDQSCK   128 //
//#define BOARD_DQS_DELAY   64+32
#define TDQSCK_ADD_DQS   ((0 * 128 * CACLU_CLK_DDR4) / 1000000)   //clk should use 64 steps
#define CLK_DELAY  (0)
#define PHY_TDQS2DQ  ((200 * 128 * CACLU_CLK_DDR4) / 1000000)
#endif
#ifdef CONFIG_BOARD_TIMMING //skt lp4 board
#define TDQS2DQ  ((0 * 128 * CACLU_CLK_DDR4) / 1000000)
//#define BOARD_DQS_DELAY   64+32
#define TDQSCK_ADD_DQS   ((0 * 128 * CACLU_CLK_DDR4) / 1000000) //clk should use 64 steps
#define CLK_DELAY  (0)
#define PHY_TDQS2DQ  ((0 * 128 * CACLU_CLK_DDR4) / 1000000)
#endif
		.cfg_board_SI_setting_ps.clk_drv_ohm = 40,
		.cfg_board_SI_setting_ps.cs_drv_ohm = 40,
		.cfg_board_SI_setting_ps.ac_drv_ohm = 40,
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

		.cfg_board_common_setting.ac_pinmux = {
			5,  1,	19, 27, 3,  9,	7,  13, 10, 0,
			11, 18, 12, 4,	20,
			16, 6,	14, 8,	15, 2,
			24, 25, 28, 29, 23, 22, 26, 17, 21,
		},
		//.cfg_board_common_setting.ddr_dq_remap= { 3, 0, 2, 1, 4, 6, 5, 7,  14, 12, 13, 15,
		.cfg_ddr_training_delay_ps.rx_offset[0] = (0 << 7) | 0x0,
		//.cfg_ddr_training_delay_ps.dac_offset[0] = 0,//(1 << 7) | 0x10,
		//.cfg_ddr_training_delay_ps.dac_offset[1] = 0,//(0 << 7) | 0x10,
		//.cfg_ddr_training_delay_ps.dac_offset[0] = (1 << 7) | 0x5,
		//.cfg_ddr_training_delay_ps.dac_offset[1] = (0 << 7) | 0x5,
		#define ac_offset (128)

		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 256 + ac_offset,
		//cke 256 only 1UI margin
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 256 + ac_offset,      //cke
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 128 + ac_offset,     //clk
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 128 + ac_offset,     //clk
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 128 + ac_offset + 30,  //cs
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 128 + ac_offset + 30,  //cs
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 256 + ac_offset,
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 256 + ac_offset,
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
		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_wck_delay[0] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[1] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[2] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[3] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[4] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[5] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[6] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[7] = 0x00000200,
		.cfg_ddr_training_delay_ps.wdq_delay[0] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[1] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[2] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[3] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[4] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[5] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[6] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[7] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[8] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[9] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[10] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[11] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[12] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[13] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[14] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[15] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[16] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[17] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[18] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[19] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[20] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[21] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[22] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[23] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[24] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[25] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[26] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[27] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[28] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[29] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[30] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[31] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[32] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[33] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[34] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[35] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[36] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[37] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[38] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[39] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[40] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[41] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[42] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[43] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[44] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[45] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[46] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[47] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[48] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[49] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[50] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[51] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[52] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[53] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[54] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[55] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[56] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[57] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[58] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[59] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[60] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[61] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[62] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[63] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[64] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[65] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[66] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[67] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[68] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[69] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[70] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[71] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 128,

		.cfg_ddr_training_delay_ps.soc_bit_vref0[0] = 0x0000000,
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
	{
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs = MAX_VALID_CONFIGS,
		//sizeof(__ddr_setting) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,
		.cfg_board_common_setting.fast_boot = {
			0, 0, 0, 0
		},
		.cfg_board_common_setting.ddr_func = 0,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_DDR4,
		.cfg_board_common_setting.enable_lpddr4x_mode = 0,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
		//.cfg_board_common_setting.dram_cs0_base_add = 0,
		//.cfg_board_common_setting.dram_cs1_base_add = 0,
		.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.DisabledDbyte[0] = 0xf0,
		//bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
		.cfg_board_common_setting.DisabledDbyte[1] = 0xf0,
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
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR4_32BIT,
		//DDR_DMC_REMAP_DDR4_32BIT,
		//DDR_DMC_REMAP_DDR4_32BIT,
		// .cfg_board_common_setting.ac_pinmux = {
		//2, 3, 1, 0, 5, 4, 0, 0, 0, 0, 1, 3, 5, 2, 4, 0 },
		.cfg_board_common_setting.ddr_dqs_swap = 0,
		// .cfg_board_common_setting.ddr_dq_remap = { 3,
		// 2, 6, 5, 7, 4, 0, 1, 10, 8, 14, 13, 15,
		//12, 9, 11, 23, 21, 17, 18, 16, 19, 20, 22, 31, 28, 24, 25, 27, 26, 30, 29 },
		.cfg_board_common_setting.ddr_vddee_setting = {
			0
		},
		.cfg_board_SI_setting_ps.DRAMFreq = CACLU_CLK_DDR4,
		// .cfg_ddr_training_delay_ps.DRAMFreq = 600,
		// 2112,//1176,cfg_ddr_training_delay_ps
		//.cfg_ddr_training_delay_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		.cfg_board_SI_setting_ps.dfi_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		.cfg_board_SI_setting_ps.clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = DDR_DRAM_LPDDR4_DRV_40_OHM,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = DDR_DRAM_DDR_WR_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = 120,      //240,
		.cfg_board_SI_setting_ps.dram_drv_pull_up_cal_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_range =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		//.cfg_ddr_training_delay_ps.dfe_offset = 0,
		.cfg_board_SI_setting_ps.vref_ac_permil = 0,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,
		//.cfg_ddr_training_delay_ps.training_phase_parameter = {0},
		//.cfg_ddr_training_delay_ps.ac_trace_delay_org = {
		//	128, 128, 128 + 40, 128, 128, 128, 128, 128,
		//	384, 384, 384,      384, 384, 384, 384, 384,
		//	384, 384, 384,      384, 384, 384, 384, 384,
		//	384, 384, 384,      384, 384, 384, 384, 384,
		//	384, 384, 384,      384,
		//},
		//total 36
		.cfg_ddr_training_delay_ps.ac_trace_delay = {
			32 + 0, 32 + 0, 32 + 0, 32 + 0,
			32,	32 + 0, 32 + 0, 32 + 0,
			32 + 0, 32 + 0, 32 + 0, 32 + 0,
			32 + 0, 32 + 0, 32,	32 + 0,
			32 + 0, 32 + 0, 32 + 0, 32 + 0,
			32 + 0, 32 + 0, 32,	32 + 0
		},
// 1 //real chip stk lp4
		.cfg_board_common_setting.dbi_enable = 0x00000000,
		.cfg_board_common_setting.ddr_rfc_type = 0x0000000d,
		.cfg_board_common_setting.pll_ssc_mode = 0x00000000,
//.cfg_board_common_setting.max_core_timmming_frequency=0x00000e10,// 3600,0x0000006a

//.cfg_board_common_setting.lpddr4_x8_mode=0x00000000,// 0,0x00000087
//.cfg_board_common_setting.tdqs2dq=0x00000000,// 0,0x0000008a
//.cfg_board_common_setting.dfe_offset_value=0x00000000,// 0,0x0000008e
//.cfg_board_common_setting.training_offset=0x00000000,// 0,0x0000008f

#define W_LEV  ((0 * 128 * CACLU_CLK_DDR4 * 2) / 1000000)///DDR4
//#define W_LEV  ((480 * 128 * CACLU_CLK_DDR4 * 2) / 1000000)
#ifdef CONFIG_PXP_TIMMING
//pxp
#define TDQS2DQ  (0 + ((0 * 128 * CACLU_CLK_DDR4 * 2) / 1000000))      //
//#define TDQSCK   64
#define CLK_DELAY 0                                             // (64)
//#define BOARD_DQS_DELAY   64
#define TDQSCK_ADD_DQS   64                                     //128   //clk should use 64 steps
#define PHY_TDQS2DQ  0
#endif
#ifdef CONFIG_RTL_TIMMING
//rtl
#define TDQS2DQ  ((0 * 128 * CACLU_CLK_DDR4) / 1000000)  //
//#define TDQSCK   128 //
//#define BOARD_DQS_DELAY   64+32
#define TDQSCK_ADD_DQS   ((0 * 128 * CACLU_CLK_DDR4) / 1000000)   //clk should use 64 steps
#define CLK_DELAY  (0)
#define PHY_TDQS2DQ  ((200 * 128 * CACLU_CLK_DDR4) / 1000000)
#endif
#ifdef CONFIG_BOARD_TIMMING //skt lp4 board
#define TDQS2DQ  ((0 * 128 * CACLU_CLK_DDR4) / 1000000)
//#define BOARD_DQS_DELAY   64+32
#define TDQSCK_ADD_DQS   ((0 * 128 * CACLU_CLK_DDR4) / 1000000) //clk should use 64 steps
#define CLK_DELAY  (0)
#define PHY_TDQS2DQ  ((0 * 128 * CACLU_CLK_DDR4) / 1000000)
#endif
		.cfg_board_SI_setting_ps.clk_drv_ohm = 40,
		.cfg_board_SI_setting_ps.cs_drv_ohm = 40,
		.cfg_board_SI_setting_ps.ac_drv_ohm = 40,
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
		.cfg_board_common_setting.ac_pinmux = {
			9,  3,	16, 14, 0,  13,
			12, 1,	15, 5,
			2,  27, 21, 26, 10, 11,
			8,  19, 7,  18, 20,
			24, 25, 29, 28, 22, 23,
			4,  6,	17,
		},
		//.cfg_board_common_setting.ddr_dq_remap= { 3, 0, 2, 1, 4, 6, 5, 7,  14, 12, 13,
		// 15, 10, 9, 8, 11,  23, 21, 17, 18, 16,
		// 19, 20, 22, 31, 28, 24, 25, 27, 26, 30, 29 },
		.cfg_ddr_training_delay_ps.rx_offset[0] = (0 << 7) | 0x0,
		//.cfg_ddr_training_delay_ps.dac_offset[0] = 0,//(1 << 7) | 0x10,
		//.cfg_ddr_training_delay_ps.dac_offset[1] = 0,//(0 << 7) | 0x10,
		//.cfg_ddr_training_delay_ps.dac_offset[0] = (1 << 7) | 0x5,
		//.cfg_ddr_training_delay_ps.dac_offset[1] = (0 << 7) | 0x5,
		#define ac_offset (128)
		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = ac_offset + 256,
		//cke 256 only 1UI margin
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = ac_offset + 256,    //cke
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = ac_offset + 128,     //clk
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = ac_offset + 128,      //clk
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = ac_offset + 128 + 30,  //cs
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = ac_offset + 128 + 30,  //cs
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = ac_offset + 256,
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = ac_offset + 256,
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
		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 128 + ac_offset + W_LEV,
		.cfg_ddr_training_delay_ps.write_wck_delay[0] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[1] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[2] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[3] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[4] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[5] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[6] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[7] = 0x00000200,
		.cfg_ddr_training_delay_ps.wdq_delay[0] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[1] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[2] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[3] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[4] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[5] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[6] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[7] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[8] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[9] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[10] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[11] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[12] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[13] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[14] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[15] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[16] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[17] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[18] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[19] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[20] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[21] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[22] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[23] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[24] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[25] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[26] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[27] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[28] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[29] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[30] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[31] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[32] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[33] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[34] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[35] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[36] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[37] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[38] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[39] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[40] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[41] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[42] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[43] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[44] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[45] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[46] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[47] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[48] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[49] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[50] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[51] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[52] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[53] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[54] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[55] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[56] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[57] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[58] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[59] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[60] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[61] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[62] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[63] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[64] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[65] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[66] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[67] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[68] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[69] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[70] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.wdq_delay[71] = ac_offset + 64 + TDQS2DQ + W_LEV,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = ac_offset,
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 128,

		.cfg_ddr_training_delay_ps.soc_bit_vref0[0] = 0x0000000,
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
#endif
};

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

/* VDDEE_VAL_REG ax209 board by  measured  value */
#if   (VDDEE_VAL == 720)
#define VDDEE_VAL_REG   0x120000
#elif (VDDEE_VAL == 730)
#define VDDEE_VAL_REG   0x110001
#elif (VDDEE_VAL == 740)
#define VDDEE_VAL_REG   0x100002
#elif (VDDEE_VAL == 750)
#define VDDEE_VAL_REG   0xf0003
#elif (VDDEE_VAL == 760)
#define VDDEE_VAL_REG   0xe0004
#elif (VDDEE_VAL == 770)
#define VDDEE_VAL_REG   0xd0005
#elif (VDDEE_VAL == 780)
#define VDDEE_VAL_REG   0xc0006
#elif (VDDEE_VAL == 790)
#define VDDEE_VAL_REG   0xb0007
#elif (VDDEE_VAL == 800)
#define VDDEE_VAL_REG   0xa0008
#elif (VDDEE_VAL == 810)
#define VDDEE_VAL_REG   0x90009
#elif (VDDEE_VAL == 820)
#define VDDEE_VAL_REG   0x8000a
#elif (VDDEE_VAL == 830)
#define VDDEE_VAL_REG   0x7000b
#elif (VDDEE_VAL == 840)
#define VDDEE_VAL_REG   0x6000c
#elif (VDDEE_VAL == 850)
#define VDDEE_VAL_REG   0x5000d
#elif (VDDEE_VAL == 860)
#define VDDEE_VAL_REG   0x4000e
#elif (VDDEE_VAL == 870)
#define VDDEE_VAL_REG   0x3000f
#elif (VDDEE_VAL == 880)
#define VDDEE_VAL_REG   0x20010
#elif (VDDEE_VAL == 890)
#define VDDEE_VAL_REG   0x10011
#elif (VDDEE_VAL == 900)
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
#if   (VDD_NPU_VAL == 690)
#define VDD_NPU_VAL_REG 0x120000
#elif (VDD_NPU_VAL == 700)
#define VDD_NPU_VAL_REG 0x110001
#elif (VDD_NPU_VAL == 710)
#define VDD_NPU_VAL_REG 0x100002
#elif (VDD_NPU_VAL == 720)
#define VDD_NPU_VAL_REG 0xf0003
#elif (VDD_NPU_VAL == 730)
#define VDD_NPU_VAL_REG 0xe0004
#elif (VDD_NPU_VAL == 740)
#define VDD_NPU_VAL_REG 0xd0005
#elif (VDD_NPU_VAL == 750)
#define VDD_NPU_VAL_REG 0xc0006
#elif (VDD_NPU_VAL == 760)
#define VDD_NPU_VAL_REG 0xb0007
#elif (VDD_NPU_VAL == 770)
#define VDD_NPU_VAL_REG 0xa0008
#elif (VDD_NPU_VAL == 780)
#define VDD_NPU_VAL_REG 0x90009
#elif (VDD_NPU_VAL == 790)
#define VDD_NPU_VAL_REG 0x8000a
#elif (VDD_NPU_VAL == 800)
#define VDD_NPU_VAL_REG 0x7000b
#elif (VDD_NPU_VAL == 810)
#define VDD_NPU_VAL_REG 0x6000c
#elif (VDD_NPU_VAL == 820)
#define VDD_NPU_VAL_REG 0x5000d
#elif (VDD_NPU_VAL == 830)
#define VDD_NPU_VAL_REG 0x4000e
#elif (VDD_NPU_VAL == 840)
#define VDD_NPU_VAL_REG 0x3000f
#elif (VDD_NPU_VAL == 850)
#define VDD_NPU_VAL_REG 0x20010
#elif (VDD_NPU_VAL == 860)
#define VDD_NPU_VAL_REG 0x10011
#elif (VDD_NPU_VAL == 870)
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
