/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * board/amlogic/t3_ar311/firmware/timing.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <asm/arch/secure_apb.h>
#include <asm/arch/timing.h>
#include <asm/arch/ddr_define.h>

/* board cpu & dus init
 * When changing CPU, DSU may need to be changed,
 * please confirm with dvfs owner.
 */
#define CPU_CLK                                 1920
#define DSU_CLK                                 1512

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

// 600 //1176 //1320 //1464 //1584 //1800 //792 //912 //1008
#define  CACLU_CLK_DDR0   1584
#define  CACLU_CLK_DDR1   1584
#define  CONFIG_BOARD_TIMMING

#define ENABLE_T3X_SKT_BOARD 1  //SKT BC309 6layer 4pcs ddr4
//#define ENABLE_T3X_REF_BOARD 1  //BC302 2layer 4pcs ddr4

#define BOARD_DDR4_SKT_BOARD 1
//#define BOARD_DDR4_REF_BOARD 1 //delay line deferent

#define T3_2GB_SAM_DDR4_X4_EID       0x66C6A

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
#define DDR_TIMMING_TUNE_TIMMING0(DDR_ID, PARA, VALUE) { DDR_ID, DDR_TIMMING_OFFSET(PARA), VALUE, DDR_TIMMING_OFFSET_SIZE(PARA), 0, DDR_RESV_CHECK_ID_ENABLE }
#define DDR_TIMMING_TUNE_TIMMING1(DDR_ID, PARA, VALUE) { DDR_ID, sizeof(((ddr_set_ps0_only_t) + DDR_TIMMING_OFFSET(PARA), VALUE, DDR_TIMMING_OFFSET_SIZE(PARA), 0, DDR_RESV_CHECK_ID_ENABLE }

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
	//start from	DDR_ID_START_MASK,ddr_id;//bit0-23 ddr_id value,bit 24-31 ddr_id source,
	//0xfe source from adc ,0xfd source from gpio_default_config
	//reg_offset
	//bit 0-15 parameter offset value,bit16-23 overrid size,bit24-31 mux ddr_id source
	//unsigned int	reg_offset;
	//unsigned int	value;

	//bit0-15 only support data size = 1byte or 2bytes,no support int value
	uint32_t	value : 16;
	uint32_t	reg_offset : 12;        //bit16-27
	uint32_t	data_size : 4;//bit28-31 if data size =15,then  will mean DDR_ID start
}  ddr_para_data_t;

typedef  struct   ddr_para_data_start {
	uint32_t	id_value : 24;          //bit0-23  efuse id or ddr id
	//uint32_t	id_adc_ch : 2;//bit6-7
	uint32_t	id_src_from : 8;        //bit24-31 ddr id from adc or gpio
}  ddr_para_data_start_t;
//#define DDR_TIMMING_OFFSET(X) (unsigned int)(unsigned long)(&(((ddr_set_ps0_only_t *)(0))->X))
//#define DDR_TIMMING_OFFSET_SIZE(X) sizeof(((ddr_set_ps0_only_t *)(0))->X)
//#define DDR_TIMMING_TUNE_TIMMING0(DDR_ID, PARA, VALUE) { DDR_ID, DDR_TIMMING_OFFSET(PARA), VALUE, DDR_TIMMING_OFFSET_SIZE(PARA), 0, DDR_RESV_CHECK_ID_ENABLE }
//#define DDR_TIMMING_TUNE_TIMMING1(DDR_ID, PARA, VALUE) { DDR_ID, sizeof(((ddr_set_ps0_only_t) + DDR_TIMMING_OFFSET(PARA), VALUE, DDR_TIMMING_OFFSET_SIZE(PARA), 0, DDR_RESV_CHECK_ID_ENABLE }
#define DDR_TIMMING_TUNE_TIMMING0_F(PARA, VALUE) (((DDR_TIMMING_OFFSET(PARA)) << 16) | ((DDR_TIMMING_OFFSET_SIZE(PARA)) << 28) | (VALUE))
#define DDR_TIMMING_TUNE_TIMMING1_F(PARA, VALUE) (((sizeof(ddr_set_ps0_only_t) + DDR_TIMMING_OFFSET(PARA)) << 16) | ((DDR_TIMMING_OFFSET_SIZE(PARA)) << 28) | (VALUE))

#define DDR_TIMMING_TUNE_START(id_src_from, id_adc_ch, id_value)  (id_src_from | id_adc_ch | id_value)
#define DDR_TIMMING_TUNE_STRUCT_SIZE(a)  sizeof(a)

#if 1
uint32_t __bl2_ddr_reg_data[] __attribute__ ((section(".ddr_2acs_data"))) = {
	//DDR_ID
	DDR_TIMMING_TUNE_START(DDR_ID_FROM_ADC, DDR_ADC_CH4, DDR_ADC_VALUE1),
	//data start,2GB,DDR0-1GB,DDR1-1GB

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

#endif

#if ENABLE_T3X_SKT_BOARD    //timing_config,SKT BC309 6layer 4pcs ddr4
ddr_set_ps0_only_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {
	{
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs = 0,
		//	sizeof(ddr_set_t_default) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size =
			sizeof(ddr_set_ps0_only_t),
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
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.DisabledDbyte[0] = CONFIG_DISABLE_D32_D63,
		//bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
		.cfg_board_common_setting.DisabledDbyte[1] = CONFIG_DISABLE_D32_D63,
		//bit 0 -3 ch1 cs0 ,bit 4-7 ch1
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		.cfg_board_common_setting.log_level = LOG_LEVEL_BASIC,
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
		//DDR_DMC_REMAP_DDR4_32BIT_T3X,//DDR_DMC_REMAP_DDR4_16BIT_T3X,
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR4_32BIT_T3X,
		//DDR_DMC_REMAP_DDR4_32BIT,
		// .cfg_board_common_setting.ac_pinmux = {
		//2, 3, 1, 0, 5, 4, 0, 0, 0, 0, 1, 3, 5, 2, 4, 0 },
		.cfg_board_common_setting.ddr_dqs_swap = 0,

		.cfg_board_common_setting.ddr_vddee_setting = { 0 },
		.cfg_board_SI_setting_ps.DRAMFreq = CACLU_CLK_DDR0,

		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		//DDR_DRAM_ODT_DDR4_PARK_ENABLE,
		.cfg_board_SI_setting_ps.dfi_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		//DDR_DRAM_ODT_W_CS0_ODT0,
		//.cfg_ddr_training_delay_ps.dfe_offset = 0,
		.cfg_board_SI_setting_ps.vref_ac_permil = 0,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,
		// 1 //real chip stk lp4
		.cfg_board_common_setting.dbi_enable = 0x00000000,
		.cfg_board_common_setting.ddr_rfc_type = DDR_RFC_TYPE_DDR4_2Gbx8,

#ifdef CONFIG_BOARD_TIMMING     //skt lp4 board
#define TDQS2DQ  ((0 * 128 * CACLU_CLK_DDR0) / 1000000)
//#define BOARD_DQS_DELAY   64+32
//clk should use 64 steps
#define TDQSCK_ADD_DQS   ((0 * 128 * CACLU_CLK_DDR0) / 1000000)
#define CLK_DELAY  (0)
#define PHY_TDQS2DQ  ((0 * 128 * CACLU_CLK_DDR0) / 1000000)
#define WRITE_LEVELING_DELAY  ((400 * 128 * CACLU_CLK_DDR0 * 2) / 1000000)
//for 1584M 100 STEP 250ps //((480 * 128 * CACLU_CLK_DDR0 * 2) / 1000000)
#define  AC_OFFSET  (0)
#endif
		.cfg_board_SI_setting_ps.clk_drv_ohm = 80,
		.cfg_board_SI_setting_ps.cs_drv_ohm = 80,
		.cfg_board_SI_setting_ps.ac_drv_ohm = 80,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = 48,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = 48,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = 60,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = 0,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = 48,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = 60,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = 0,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = 120,
#ifdef BOARD_DDR4_REF_BOARD
		.cfg_board_SI_setting_ps.clk_drv_ohm = 80,
		.cfg_board_SI_setting_ps.cs_drv_ohm = 80,
		.cfg_board_SI_setting_ps.ac_drv_ohm = 80,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = 48,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = 48,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = 60,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = 0,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = 48,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = 60,
#endif
		.cfg_board_SI_setting_ps.dram_drv_pull_up_cal_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_range =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		//.cfg_ddr_training_delay_ps.dfe_offset = 0,

		.cfg_board_common_setting.ac_pinmux = {
			16, 26,	4, 10, 22, 2, 11, 14, 6, 3,
			8, 9, 22, 17, 21, 20, 19, 7, 27, 18, 12,
			0, 15, 5, 13, 1, 24, 25, 28, 29,
		},

		.cfg_board_common_setting.ddr_dq_remap = {
			2, 6, 0, 4, 5, 3, 1, 7, 32,
			15, 10, 12, 14, 33, 8, 9, 13, 11,
			22, 20, 34, 16, 21, 19, 18, 23, 17,
			28, 35, 27, 26, 29, 25, 30, 31, 24,
		},

#ifdef CONFIG_BOARD_TIMMING
	.cfg_ddr_training_delay_ps.reserve_para[0] = (1 << 7) | 0x8,      //cs0 write dqs
	.cfg_ddr_training_delay_ps.reserve_para[1] = (1 << 7) | 0x8,      //cs0 write dqs
	.cfg_ddr_training_delay_ps.reserve_para[2] = (1 << 7) | 0x8,      //cs0 write dqs
	.cfg_ddr_training_delay_ps.reserve_para[3] = (1 << 7) | 0x8,      //cs0 write dqs
	.cfg_ddr_training_delay_ps.reserve_para[8 + 0] = (1 << 7) | 0x10, //cs0 read dqs
	.cfg_ddr_training_delay_ps.reserve_para[8 + 1] = (1 << 7) | 0x10, //cs0 read dqs
	.cfg_ddr_training_delay_ps.reserve_para[8 + 2] = (1 << 7) | 0x10, //cs0 read dqs
	.cfg_ddr_training_delay_ps.reserve_para[8 + 3] = (1 << 7) | 0x10, //cs0 read dqs
	//.cfg_ddr_training_delay_ps.dac_offset[0] = 0,//(1 << 7) | 0x10,
	//.cfg_ddr_training_delay_ps.dac_offset[1] = 0,//(0 << 7) | 0x10,
	//.cfg_ddr_training_delay_ps.dac_offset[0] = (1 << 7) | 0x5,
	//.cfg_ddr_training_delay_ps.dac_offset[1] = (0 << 7) | 0x5,
#endif
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
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 256 + AC_OFFSET,
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

	.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 100 + AC_OFFSET,//cs0
	.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 140 + AC_OFFSET,//cs1 t3 SKT board
	.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 128 + AC_OFFSET,      //ckp
	.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 128 + AC_OFFSET,      //ckn
	.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 128 + AC_OFFSET,      //cke0
	.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 128 + AC_OFFSET,      //cke1
	.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 128 + AC_OFFSET,       //odt0
	.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 128 + AC_OFFSET,      //odt1

		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 200 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 210 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 220 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 170 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 210 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 150 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 150 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 200 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 170 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 200 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 160 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 170 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 210 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 220 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 256 +  AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 170 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 220 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 230 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 235 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 230 + AC_OFFSET,
#ifdef BOARD_DDR4_REF_BOARD
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 100 + AC_OFFSET,//cs0 t3x ref board
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 90 + AC_OFFSET,//cs1 t3x ref board
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 256 - 20 - 100 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 156 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 205 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 156 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 140 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 110 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 210 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 180 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 256 +  AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 250 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 256 + AC_OFFSET,
#endif

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

		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = AC_OFFSET + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = AC_OFFSET + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = AC_OFFSET + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = AC_OFFSET + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = AC_OFFSET + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = AC_OFFSET + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = AC_OFFSET + WRITE_LEVELING_DELAY,
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = AC_OFFSET + WRITE_LEVELING_DELAY,

		.cfg_ddr_training_delay_ps.write_wck_delay[0] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[1] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[2] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[3] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[4] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[5] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[6] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[7] = 0x00000200,

	.cfg_ddr_training_delay_ps.wdq_delay[0] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[1] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[2] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[3] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[4] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[5] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[6] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[7] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[8] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[9] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[10] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[11] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[12] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[13] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[14] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[15] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[16] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[17] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[18] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[19] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[20] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[21] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[22] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[23] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[24] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[25] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[26] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[27] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[28] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[29] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[30] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[31] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[32] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[33] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[34] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[35] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[36] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[37] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[38] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[39] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[40] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[41] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[42] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[43] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[44] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[45] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[46] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[47] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[48] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[49] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[50] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[51] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[52] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[53] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[54] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[55] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[56] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[57] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[58] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[59] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[60] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[61] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[62] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[63] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[64] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[65] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[66] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[67] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[68] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[69] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[70] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,
	.cfg_ddr_training_delay_ps.wdq_delay[71] = 64 + AC_OFFSET + TDQS2DQ + WRITE_LEVELING_DELAY,

		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = (128 << 2) - 128 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = (128 << 2) - 128 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = (128 << 2) - 128 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = (128 << 2) - 128 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = (128 << 2) - 128 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = (128 << 2) - 128 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = (128 << 2) - 128 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = (128 << 2) - 128 + AC_OFFSET,

		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 128,
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 128,

		.cfg_ddr_training_delay_ps.soc_bit_vref0[0] = 0x000000,
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
	{
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs = 0,
		//sizeof(__ddr_setting) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size =
			sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,
		.cfg_board_common_setting.fast_boot = { 0 },
		.cfg_board_common_setting.ddr_func = 0,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_DDR4,
		.cfg_board_common_setting.enable_lpddr4x_mode = 0,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
		//.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_16BIT_CH0,
		//.cfg_board_common_setting.dram_cs0_base_add = 0,
		//.cfg_board_common_setting.dram_cs1_base_add = 0,
		.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.DisabledDbyte[0] = CONFIG_DISABLE_D32_D63,
		//bit 0 -3 ch0 cs0 ,bit 4-7 ch0 cs1,
		.cfg_board_common_setting.DisabledDbyte[1] = CONFIG_DISABLE_D32_D63,
		//bit 0 -3 ch1 cs0 ,bit 4-7 ch1
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		.cfg_board_common_setting.log_level = LOG_LEVEL_BASIC,
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
		//DDR_DMC_REMAP_DDR4_32BIT_T3X,//DDR_DMC_REMAP_DDR4_16BIT_T3X,
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR4_32BIT_T3X,
		//DDR_DMC_REMAP_DDR4_32BIT,
		// .cfg_board_common_setting.ac_pinmux = {
		//2, 3, 1, 0, 5, 4, 0, 0, 0, 0, 1, 3, 5, 2, 4, 0 },
		.cfg_board_common_setting.ddr_dqs_swap = 0,

		.cfg_board_common_setting.ddr_vddee_setting = { 0 },
		.cfg_board_SI_setting_ps.DRAMFreq = CACLU_CLK_DDR1,

		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		//DDR_DRAM_ODT_DDR4_PARK_ENABLE,
		.cfg_board_SI_setting_ps.dfi_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		//DDR_DRAM_ODT_W_CS0_ODT0,
		//.cfg_ddr_training_delay_ps.dfe_offset = 0,
		.cfg_board_SI_setting_ps.vref_ac_permil = 0,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,
		// 1 //real chip stk lp4
		.cfg_board_common_setting.dbi_enable = 0x00000000,
		.cfg_board_common_setting.ddr_rfc_type = DDR_RFC_TYPE_DDR4_2Gbx8,

#ifdef CONFIG_BOARD_TIMMING     //skt lp4 board
#define TDQS2DQ_DDR1  ((0 * 128 * CACLU_CLK_DDR1) / 1000000)
//#define BOARD_DQS_DELAY   64+32
//clk should use 64 steps
#define TDQSCK_ADD_DQS_DDR1   ((0 * 128 * CACLU_CLK_DDR1) / 1000000)
#define CLK_DELAY_DDR1  (0)
#define PHY_TDQS2DQ_DDR1  ((0 * 128 * CACLU_CLK_DDR1) / 1000000)
#define WRITE_LEVELING_DDR1  ((400 * 128 * CACLU_CLK_DDR1 * 2) / 1000000)
//for 1584M 100 STEP 250ps //((480 * 128 * CACLU_CLK_DDR0 * 2) / 1000000)
#define  AC_OFFSET  (0)
#endif
		.cfg_board_SI_setting_ps.clk_drv_ohm = 80,
		.cfg_board_SI_setting_ps.cs_drv_ohm = 80,
		.cfg_board_SI_setting_ps.ac_drv_ohm = 80,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = 48,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = 48,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = 60,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = 0,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = 48,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = 60,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = 0,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = 120,
#ifdef BOARD_DDR4_REF_BOARD
		.cfg_board_SI_setting_ps.clk_drv_ohm = 80,
		.cfg_board_SI_setting_ps.cs_drv_ohm = 80,
		.cfg_board_SI_setting_ps.ac_drv_ohm = 80,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = 48,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = 48,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = 60,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = 0,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = 48,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = 60,
#endif
		.cfg_board_SI_setting_ps.dram_drv_pull_up_cal_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_range =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		//.cfg_ddr_training_delay_ps.dfe_offset = 0,

		.cfg_board_common_setting.ac_pinmux = {
			15, 3, 13, 9, 22, 1, 12, 20, 18, 19,
			27, 5, 22, 4, 0, 16, 17, 10, 6, 21, 14,
			26, 2, 11, 7, 8, 24, 25, 28, 29,
		},
#ifdef CONFIG_BOARD_TIMMING
		//skt board pinmux
//		.cfg_board_common_setting.ddr_dq_remap = {
//			7, 5, 0, 3, 4, 6, 1, 2, 32,
//			15, 8, 13, 10, 9, 11, 12, 14, 33,
//			19, 21, 16, 23, 20, 18, 34, 22, 17,
//			29, 25, 24, 27, 28, 30, 31, 35, 26,
//		},
		//ref board pinmux
		.cfg_board_common_setting.ddr_dq_remap = {
			2, 6, 0, 4, 3, 5, 1, 7, 32,
			15, 8, 13, 10, 9, 11, 12, 14, 33,
			19, 21, 16, 23, 20, 18, 34, 22, 17,
			29, 25, 24, 27, 28, 30, 31, 35, 26,
		},
#endif

#ifdef CONFIG_BOARD_TIMMING
	.cfg_ddr_training_delay_ps.tx_offset[0] = (0 << 7) | 0x0,
	.cfg_ddr_training_delay_ps.rx_offset[0] = (0 << 7) | 0x0,
	.cfg_ddr_training_delay_ps.reserve_para[0] = (1 << 7) | 0x8,      //cs0 write dqs
	.cfg_ddr_training_delay_ps.reserve_para[1] = (1 << 7) | 0x8,      //cs0 write dqs
	.cfg_ddr_training_delay_ps.reserve_para[2] = (1 << 7) | 0x8,      //cs0 write dqs
	.cfg_ddr_training_delay_ps.reserve_para[3] = (1 << 7) | 0x8,      //cs0 write dqs
	.cfg_ddr_training_delay_ps.reserve_para[8 + 0] = (1 << 7) | 0x10, //cs0 read dqs
	.cfg_ddr_training_delay_ps.reserve_para[8 + 1] = (1 << 7) | 0x10, //cs0 read dqs
	.cfg_ddr_training_delay_ps.reserve_para[8 + 2] = (1 << 7) | 0x10, //cs0 read dqs
	.cfg_ddr_training_delay_ps.reserve_para[8 + 3] = (1 << 7) | 0x10, //cs0 read dqs
	//.cfg_ddr_training_delay_ps.dac_offset[0] = 0,//(1 << 7) | 0x10,
	//.cfg_ddr_training_delay_ps.dac_offset[1] = 0,//(0 << 7) | 0x10,
	//.cfg_ddr_training_delay_ps.dac_offset[0] = (1 << 7) | 0x5,
	//.cfg_ddr_training_delay_ps.dac_offset[1] = (0 << 7) | 0x5,
#endif
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
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 256 + AC_OFFSET,
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

	.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 150 + AC_OFFSET,//cs0
	.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 140 + AC_OFFSET,//cs1//skt+70  ref+0
	.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 128 + AC_OFFSET,      //ckp
	.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 128 + AC_OFFSET,      //ckn
	.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 128 + AC_OFFSET,      //cke0
	.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 128 + AC_OFFSET,      //cke1
	.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 128 + AC_OFFSET,      //odt0
	.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 128 + AC_OFFSET,      //odt1

		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 200 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 225 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 220 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 200 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 180 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 200 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 240 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 210 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 240 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 220 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 230 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 230 + AC_OFFSET,
#ifdef BOARD_DDR4_REF_BOARD
		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 140 + AC_OFFSET,//cs0
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 90 + AC_OFFSET,//cs1//skt+70 ref+0
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 110 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 110 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 140 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 90 + AC_OFFSET,// cs0
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 200 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 200 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 256 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 180 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 220 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 300 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 230 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 200 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 160 + AC_OFFSET,
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 190 + AC_OFFSET,
#endif

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

#ifdef CONFIG_BOARD_TIMMING
#ifdef BOARD_DDR4_SKT_BOARD
	.cfg_ddr_training_delay_ps.read_dq_delay_t[0] = 0x3b,//59,0x118
	.cfg_ddr_training_delay_ps.read_dq_delay_t[1] = 0x40,//64,0x11a
	.cfg_ddr_training_delay_ps.read_dq_delay_t[2] = 0x6d,//109,0x11c
	.cfg_ddr_training_delay_ps.read_dq_delay_t[3] = 0x39,//57,0x11e
	.cfg_ddr_training_delay_ps.read_dq_delay_t[4] = 0x61,//97,0x120
	.cfg_ddr_training_delay_ps.read_dq_delay_t[5] = 0x5b,//91,0x122
	.cfg_ddr_training_delay_ps.read_dq_delay_t[6] = 0x5e,//94,0x124
	.cfg_ddr_training_delay_ps.read_dq_delay_t[7] = 0x68,//104,0x126
	.cfg_ddr_training_delay_ps.read_dq_delay_t[8] = 0x54,//84,0x128
	.cfg_ddr_training_delay_ps.read_dq_delay_t[9] = 0x43,//67,0x12a
	.cfg_ddr_training_delay_ps.read_dq_delay_t[10] = 0x3f,//63,0x12c
	.cfg_ddr_training_delay_ps.read_dq_delay_t[11] = 0x42,//66,0x12e
	.cfg_ddr_training_delay_ps.read_dq_delay_t[12] = 0x40,//64,0x130
	.cfg_ddr_training_delay_ps.read_dq_delay_t[13] = 0x4d,//77,0x132
	.cfg_ddr_training_delay_ps.read_dq_delay_t[14] = 0x4d,//77,0x134
	.cfg_ddr_training_delay_ps.read_dq_delay_t[15] = 0x48,//72,0x136
	.cfg_ddr_training_delay_ps.read_dq_delay_t[16] = 0x49,//73,0x138
	.cfg_ddr_training_delay_ps.read_dq_delay_t[17] = 0x45,//69,0x13a
	.cfg_ddr_training_delay_ps.read_dq_delay_t[18] = 0x46,//70,0x13c
	.cfg_ddr_training_delay_ps.read_dq_delay_t[19] = 0x34,//52,0x13e
	.cfg_ddr_training_delay_ps.read_dq_delay_t[20] = 0x6c,//108,0x140
	.cfg_ddr_training_delay_ps.read_dq_delay_t[21] = 0x32,//50,0x142
	.cfg_ddr_training_delay_ps.read_dq_delay_t[22] = 0x44,//68,0x144
	.cfg_ddr_training_delay_ps.read_dq_delay_t[23] = 0x61,//97,0x146
	.cfg_ddr_training_delay_ps.read_dq_delay_t[24] = 0x4c,//76,0x148
	.cfg_ddr_training_delay_ps.read_dq_delay_t[25] = 0x55,//85,0x14a
	.cfg_ddr_training_delay_ps.read_dq_delay_t[26] = 0x51,//81,0x14c
	.cfg_ddr_training_delay_ps.read_dq_delay_t[27] = 0x32,//50,0x14e
	.cfg_ddr_training_delay_ps.read_dq_delay_t[28] = 0x3c,//60,0x150
	.cfg_ddr_training_delay_ps.read_dq_delay_t[29] = 0x49,//73,0x152
	.cfg_ddr_training_delay_ps.read_dq_delay_t[30] = 0x3b,//59,0x154
	.cfg_ddr_training_delay_ps.read_dq_delay_t[31] = 0x50,//80,0x156
	.cfg_ddr_training_delay_ps.read_dq_delay_t[32] = 0x4b,//75,0x158
	.cfg_ddr_training_delay_ps.read_dq_delay_t[33] = 0x39,//57,0x15a
	.cfg_ddr_training_delay_ps.read_dq_delay_t[34] = 0x40,//64,0x15c
	.cfg_ddr_training_delay_ps.read_dq_delay_t[35] = 0x3d,//61,0x15e
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
#endif
#endif

		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0 + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0 + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0 + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0 + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0 + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0 + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0 + WRITE_LEVELING_DDR1,
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0 + WRITE_LEVELING_DDR1,

		.cfg_ddr_training_delay_ps.write_wck_delay[0] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[1] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[2] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[3] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[4] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[5] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[6] = 0x00000200,
		.cfg_ddr_training_delay_ps.write_wck_delay[7] = 0x00000200,

	.cfg_ddr_training_delay_ps.wdq_delay[0] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[1] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[2] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[3] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[4] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[5] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[6] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[7] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[8] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[9] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[10] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[11] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[12] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[13] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[14] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[15] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[16] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[17] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[18] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[19] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[20] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[21] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[22] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[23] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[24] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[25] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[26] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[27] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[28] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[29] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[30] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[31] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[32] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[33] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[34] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[35] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[36] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[37] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[38] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[39] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[40] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[41] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[42] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[43] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[44] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[45] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[46] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[47] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[48] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[49] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[50] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[51] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[52] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[53] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[54] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[55] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[56] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[57] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[58] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[59] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[60] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[61] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[62] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[63] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[64] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[65] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[66] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[67] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[68] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[69] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[70] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,
	.cfg_ddr_training_delay_ps.wdq_delay[71] = 64 + TDQS2DQ_DDR1 + WRITE_LEVELING_DDR1,

		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = (128 << 2) - 128,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = (128 << 2) - 128,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = (128 << 2) - 128,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = (128 << 2) - 128,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = (128 << 2) - 128,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = (128 << 2) - 128,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = (128 << 2) - 128,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = (128 << 2) - 128,

		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0,
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0,
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0,
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0,
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0,
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0,
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0,
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0,

		.cfg_ddr_training_delay_ps.soc_bit_vref0[0] = 0x000000,
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


/* board vmin_value defines */
#define VMIN_FF_VALUE                           770
#define VMIN_TT_VALUE                           800
#define VMIN_SS_VALUE                           810
/* board vddee_value defines */
/* 0x6000c=0.84V */
#define VDDEE_FF_VALUE                          0x6000c
#define VDDEE_TT_VALUE                          0x6000c
#define VDDEE_SS_VALUE                          0x6000c

board_clk_set_t __board_clk_setting
__attribute__ ((section(".clk_param"))) = {
	/* clock settings for bl2 */
	.cpu_clk		= CPU_CLK / 24 * 24,
	.dsu_clk		= DSU_CLK / 24 * 24,
#ifdef CONFIG_PXP_EMULATOR
	.pxp			= 1,
#else
	.pxp			= 0,
#endif
	.high_console_baud	= CONFIG_HIGH_CONSOLE_BAUD,
};


#define VDDEE_VAL                               AML_VDDEE_INIT_VOLTAGE
#define VCCK_A_VAL                              AML_VDD_CPUA_INIT_VOLTAGE
#define VCCK_B_VAL                              AML_VDD_CPUB_INIT_VOLTAGE
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

/* VDDEE_VAL_REG */
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
#if 0
bl2_reg_t __bl2_reg[] __attribute__ ((section(".generic_param"))) = {
	//hxbao, need fine tune
	{ 0, 0, 0xffffffff, 0, 0, 0 },
};
#endif
/* gpio/pinmux/pwm init */
register_ops_t __bl2_ops_reg[MAX_REG_OPS_ENTRIES]
__attribute__ ((section(".misc_param"))) = {
	/* config vmin_ft value */
	{ 0, VMIN_SS_VALUE, 0xffffffff, 0, BL2_INIT_STAGE_VMIN_FLAG_1, 0 },
	{ 0, VMIN_TT_VALUE, 0xffffffff, 0, BL2_INIT_STAGE_VMIN_FLAG_2, 0 },
	{ 0, VMIN_FF_VALUE, 0xffffffff, 0, BL2_INIT_STAGE_VMIN_FLAG_3, 0 },
	/*config vddee and vcck pwm - pwm_a and pwm_b*/
	/*config vddee and vcck pwm - pwm_a and pwm_b*/
	/* PWM_A VDDEE_VAL_REG */
#ifdef CONFIG_PDVFS_ENABLE
	{PWMAB_PWM_A, VDDEE_SS_VALUE, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_1, 0},
	{PWMAB_PWM_A, VDDEE_TT_VALUE, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_2, 0},
	{PWMAB_PWM_A, VDDEE_FF_VALUE, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_3, 0},
#else
	{PWMAB_PWM_A, VDDEE_VAL_REG, 0xffffffff, 0,	0, 0},
#endif
	/* PWM_B VCCK_VAL_REG */
	{ PWMAB_PWM_B,		   VCCK_A_VAL_REG,	       0xffffffff, 0, 0, 0 },
	{ PWMEF_PWM_A,		   VCCK_B_VAL_REG,	       0xffffffff, 0, 0, 0 },
	{ PWMAB_MISC_REG_AB,	   (0x3 << 0),		       (0x3 << 0), 0, 0, 0 },
	{ PWMEF_MISC_REG_AB,	   (0x1 << 0),		       (0x1 << 0), 0, 0, 0 },
	/* disable vddcpu dc-dc, set TEST_N to low */
	{ PADCTRL_TESTN_O,	   (0x0 << 0),		       (0x1 << 0), 0, 0, 0 },
	{ PADCTRL_TESTN_OEN,	   (0x0 << 0),		       (0x1 << 0), 0, 0, 0 },
	/*enable pin,set gpiod_4,enable vddcpu*/
	{ PADCTRL_GPIOD_O,	   (0x1 << 4),		       (0x1 << 4), 0, 0, 0 },
	{ PADCTRL_GPIOD_OEN,	   (0x0 << 4),		       (0x1 << 4), 0, 0, 0 },
	/* enable vddio3v3/vddcpu/vcc3v3, set gpiod_10 high */
	{ PADCTRL_GPIOD_O,	   (0x1 << 10),		       (0x1 << 10), 0, 0, 0 },
	{ PADCTRL_GPIOD_OEN,	   (0x0 << 10),		       (0x1 << 10), 0, 0, 0 },
	/* set pwm a and pwm b pwm e clock rate to 24M, enable them */
	{ CLKCTRL_PWM_CLK_AB_CTRL, ((0x1 << 8) | (0x1 << 24)), 0xffffffff, 0, 0, 0 },
	{ CLKCTRL_PWM_CLK_EF_CTRL, (0x1 << 8), 0xffffffff, 0, 0, 0 },
	/* set GPIOE_0 GPIOE_1 GPIOE_2 drive strength to 3 */
	{ PADCTRL_GPIOE_DS,	   0x3f,			       0x3f,	   0, 0, 0 },
	/* set GPIOE_0 GPIOE_1 GPIOE_2 mux to pwma pwmb pwme*/
	{ PADCTRL_PIN_MUX_REGD,	   (0x1 << 0),		       (0xf << 0), 0, 0, 0 },
	{ PADCTRL_PIN_MUX_REGD,	   (0x1 << 4),		       (0xf << 4), 0, 0, 0 },
	{ PADCTRL_PIN_MUX_REGD,	   (0x1 << 8),		       (0xf << 8), 0, 0, 0 },

	/* According to vlsi & hardware, when GPIOM_29 use as input function, need to set:
	 * 1.Receive enable bit=1, for GPIOM_29,this bit is in PADCTRL_GPIOM_PULL_EN bit28
	 * 2.DS=0, PADCTRL_GPIOM_DS_EXT bit26-27 to 0(DS=3 if set to output and freq < 50Mhz)
	 */
	{ PADCTRL_GPIOM_PULL_EN,   (0x1 << 28),		       (0x1 << 28), 0, 0, 0 },
	{ PADCTRL_GPIOM_DS_EXT,    (0 << 26),		       (0x3 << 26), 0, 0, 0 },
};

#define DEV_FIP_SIZE 0x300000
#define DDR_FIP_SIZE 0x40000
/* for all the storage parameter */
storage_parameter_t __store_para __attribute__ ((section(".store_param"))) = {
	.common = {
		.version			= 0x01,
		.device_fip_container_size	= DEV_FIP_SIZE,
		.device_fip_container_copies	= 4,
		.ddr_fip_container_size		= DDR_FIP_SIZE,
	},
	.nand = {
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
						  (8 << 0),
		.reserved_area_blk_cnt		= 48,
		.page_per_block			= 64,
		.use_param_page_list		= 0,
	},
};
