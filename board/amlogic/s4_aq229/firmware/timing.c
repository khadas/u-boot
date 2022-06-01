// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <asm/arch/secure_apb.h>
#include <asm/arch/timing.h>
#include <asm/arch/ddr_define.h>

/* board clk defines */
#define CPU_CLK                                 1512
//bit 6 adc_channel bit 0-5 adc value,chan 3 value 8 is layer 2
#define DDR_ID_ACS_ADC   ((3 << 6) | (8))

//*/
#define DDR_RESV_CHECK_ID_ENABLE  0Xfe
#define SAR_ADC_DDR_ID_BASE   0
#define SAR_ADC_DDR_ID_STEP   80
#define CARMEL_BOARD_1G_1G_ADC_ID   SAR_ADC_DDR_ID_BASE + SAR_ADC_DDR_ID_STEP                           //85  0-125 step 0
#define CARMEL_BOARD_2G_1G_ADC_ID   SAR_ADC_DDR_ID_BASE + SAR_ADC_DDR_ID_STEP + SAR_ADC_DDR_ID_STEP     //167 126-200 step 1
#define DDR_TIMMING_OFFSET(X) (unsigned int)(unsigned long)(&(((ddr_set_ps0_only_t *)(0))->X))
#define DDR_TIMMING_OFFSET_SIZE(X) sizeof(((ddr_set_ps0_only_t *)(0))->X)
#define DDR_TIMMING_TUNE_TIMMING0(DDR_ID, PARA, VALUE) { DDR_ID, DDR_TIMMING_OFFSET(PARA), VALUE, DDR_TIMMING_OFFSET_SIZE(PARA), 0, DDR_RESV_CHECK_ID_ENABLE }
#define DDR_TIMMING_TUNE_TIMMING1(DDR_ID, PARA, VALUE) { DDR_ID, sizeof(((ddr_set_ps0_only_t) + DDR_TIMMING_OFFSET(PARA), VALUE, DDR_TIMMING_OFFSET_SIZE(PARA), 0, DDR_RESV_CHECK_ID_ENABLE }

//bit24-31 define ID and size
#define DDR_ID_FROM_EFUSE  (0Xff<<24)
#define DDR_ID_FROM_ADC  (0Xfeu<<24)
#define DDR_ID_FROM_GPIO_CONFIG1  (0Xfd<<24)
#define DDR_ID_START_MASK  (0XFFDDCCBB)

#define DDR_ADC_CH0  (0X0<<6)
#define DDR_ADC_CH1  (0X1<<6)
#define DDR_ADC_CH2  (0X2<<6)
#define DDR_ADC_CH3  (0X3<<6)

#define DDR_ADC_VALUE0  (0X0<<0)
#define DDR_ADC_VALUE1  (0X1<<0)
#define DDR_ADC_VALUE2  (0X2<<0)
#define DDR_ADC_VALUE3  (0X3<<0)
#define DDR_ADC_VALUE4  (0X4<<0)
#define DDR_ADC_VALUE5  (0X5<<0)
#define DDR_ADC_VALUE6  (0X6<<0)
#define DDR_ADC_VALUE7  (0X7<<0)
#define DDR_ADC_VALUE8  (0X8<<0)
#define DDR_ADC_VALUE9  (0X9<<0)
#define DDR_ADC_VALUE10  (0Xa<<0)
#define DDR_ADC_VALUE11  (0Xb<<0)
#define DDR_ADC_VALUE12  (0Xc<<0)
#define DDR_ADC_VALUE13 (0Xd<<0)
#define DDR_ADC_VALUE14  (0Xe<<0)
#define DDR_ADC_VALUE15  (0Xf<<0)

typedef  struct   ddr_para_data {
	//start from	DDR_ID_START_MASK,ddr_id;//bit0-23 ddr_id value,bit 24-31 ddr_id source  ,0xfe source from adc ,0xfd source from gpio_default_config
	//reg_offset    //bit 0-15 parameter offset value,bit16-23 overrid size,bit24-31 mux ddr_id source
	//unsigned int	reg_offset;
	//unsigned int	value;
	uint32_t	value : 16;  //bit0-15 only support data size =1byte or 2bytes,no support int value
	uint32_t	reg_offset : 12;//bit16-27
	uint32_t	data_size : 4;//bit28-31 if data size =15,then  will mean DDR_ID start

}  ddr_para_data_t;

typedef  struct   ddr_para_data_start {

	uint32_t	id_value : 24;  //bit0-23  efuse id or ddr id
	//uint32_t	id_adc_ch : 2;//bit6-7
	uint32_t	id_src_from : 8;//bit24-31 ddr id from adc or gpio
}  ddr_para_data_start_t;
//#define DDR_TIMMING_OFFSET(X) (unsigned int)(unsigned long)(&(((ddr_set_ps0_only_t *)(0))->X))
//#define DDR_TIMMING_OFFSET_SIZE(X) sizeof(((ddr_set_ps0_only_t *)(0))->X)
//#define DDR_TIMMING_TUNE_TIMMING0(DDR_ID, PARA, VALUE) { DDR_ID, DDR_TIMMING_OFFSET(PARA), VALUE, DDR_TIMMING_OFFSET_SIZE(PARA), 0, DDR_RESV_CHECK_ID_ENABLE }
//#define DDR_TIMMING_TUNE_TIMMING1(DDR_ID, PARA, VALUE) { DDR_ID, sizeof(((ddr_set_ps0_only_t) + DDR_TIMMING_OFFSET(PARA), VALUE, DDR_TIMMING_OFFSET_SIZE(PARA), 0, DDR_RESV_CHECK_ID_ENABLE }
#define DDR_TIMMING_TUNE_TIMMING0_F(PARA, VALUE) (((DDR_TIMMING_OFFSET(PARA))<<16)|((DDR_TIMMING_OFFSET_SIZE(PARA))<<28)|VALUE)
#define DDR_TIMMING_TUNE_TIMMING1_F(PARA, VALUE) (((sizeof(ddr_set_ps0_only_t) +DDR_TIMMING_OFFSET(PARA))<<16)|((DDR_TIMMING_OFFSET_SIZE(PARA))<<28)|VALUE)

#define DDR_TIMMING_TUNE_START(id_src_from, id_adc_ch, id_value)  (id_src_from|id_adc_ch|id_value)
#define DDR_TIMMING_TUNE_STRUCT_SIZE(a)  sizeof(a)

#if 1
uint32_t __bl2_ddr_reg_data[] __attribute__ ((section(".ddr_2acs_data"))) = {
	DDR_TIMMING_TUNE_START(DDR_ID_FROM_ADC, DDR_ADC_CH1, DDR_ADC_VALUE1),
	//data start
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_common_setting.Is2Ttiming, CONFIG_USE_DDR_2T_MODE),
};
////_ddr_para_2nd_setting
//*/
uint32_t __ddr_parameter_reg_index[] __attribute__ ((section(".ddr_2acs_index"))) = {
	DDR_ID_START_MASK,
	DDR_TIMMING_TUNE_STRUCT_SIZE(__bl2_ddr_reg_data),
	//0,
};
#endif

//#define BOARD_AP223_LPDDR4 1
//#define S4_LPDDR4_1RANK 1   //use for 1rank lpddr4
//#define S4_LPDDR4_1RANK_RAYSON_2GB 1

//#define S4_LPDDR4_DONGLE_LAYER_4 1   //use for 1rank lpddr4
//#define S4_LPDDR4_DONGLE_LAYER_6 1  //use for 1rank lpddr4

//#define S4_LPDDR4_2RANK 1
//#define S4_LPDDR4_2RANK_RAYSON_2GB 1
//#define S4_LPDDR4_2RANK_FORESEE_2GB 1

//#define S4_DDR4_2RANK 1
#define S4_DDR4_1RANK 1
//#define S4_DDR4_1RANK_PCB_MODE2_GANGZHENG 1
//#define S4_DDR3  1

ddr_set_ps0_only_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {
#if S4_DDR3
	{
		//timing_config,s4 ddr3
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs =
			sizeof(__ddr_setting[0]) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,//0
		.cfg_board_common_setting.fast_boot = { 0 },
		.cfg_board_common_setting.ddr_func = DDR_FUNC,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_DDR3,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
		.cfg_board_common_setting.DisabledDbyte = CONFIG_DISABLE_D32_D63,
		.cfg_board_common_setting.dram_cs0_base_add = 0,
		.cfg_board_common_setting.dram_cs1_base_add = 0,
		.cfg_board_common_setting.dram_cs0_size_MB = CONFIG_DDR0_SIZE_2048MB,
		.cfg_board_common_setting.dram_cs1_size_MB = CONFIG_DDR1_SIZE_0MB,
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		.cfg_board_common_setting.log_level = LOG_LEVEL_BASIC,
		.cfg_board_common_setting.ddr_rdbi_wr_enable = DDR_WRITE_READ_DBI_DISABLE,
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
		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test_function = { 0 },
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR3_32BIT,
		//s4 ddr3 ac pinmux
		.cfg_board_common_setting.ac_pinmux = {
			0,  0,	0,  1, 2,  3,
			11, 14, 13, 5, 25, 6, 27,  0,
			15, 7, 26, 9, 4, 20, 8, 21,
			0, 0, 0, 0, 10, 2, 3, 0,
			0, 1, 12, 22, 0
		},
		.cfg_board_common_setting.ddr_dqs_swap = 0,
		.cfg_board_common_setting.ddr_dq_remap = {
			0,  1,	2,  3,	4,  5,	6,  7,
			8,  9,	10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, 29, 30, 31,
			32, 33, 34, 35
		},//d0-d31 dm0 dm1 dm2 dm3
		.cfg_board_common_setting.ddr_vddee_setting = { 0 },
		.cfg_board_SI_setting_ps.DRAMFreq = 792,
		.cfg_board_SI_setting_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		.cfg_board_SI_setting_ps.ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		.cfg_board_SI_setting_ps.clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_120_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_120_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = DDR_DRAM_DDR3_DRV_40_OHM,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = DDR_DRAM_DDR3_ODT_120_OHM,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = DDR_DRAM_DDR_WR_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_pull_up_calibration_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_voltage_range_setting =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		.cfg_board_SI_setting_ps.dfe_offset = 0,
		.cfg_board_SI_setting_ps.vref_ac_permil = 300,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 330,  //330
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,
		.cfg_board_SI_setting_ps.training_phase_parameter = { 0 },
.cfg_board_SI_setting_ps.ac_trace_delay_org = {
	64 + 128, 64 + 128, 128, 128, 64 + 128, 64 + 128, 0 + 128, 0 + 128,
	256 + 20, 256 + 20, 256 + 60 - 44, 256 + 20 - 36, 256, 256, 256, 256,
	256, 256, 256, 256, 256, 256, 256, 256,
	256, 256, 256, 256, 256, 256, 256, 256,
	256, 256, 256, 256,
},//total 36
.cfg_ddr_training_delay_ps.ac_trace_delay = {
	64 + 128, 64 + 128, 128 - 60, 128, 64 + 128, 64 + 128, 0 + 128, 128 + 128,
	256 + 20, 256 + 20, 256 + 60 - 44, 256 + 20 - 36, 256, 256, 256, 256,
	256, 256, 256, 256, 256, 256, 256, 256,
	256, 256, 256, 256, 256, 256, 256, 256,
	256, 256, 256, 256,
},

#if 1
		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 128,         // 110
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 128 + 0,     // 140
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 128 + 0,     // 110
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 128 + 0,     // 140
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x0000006e,  // 110
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x00000082,  // 130
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x0000006e,  // 110
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x0000008c,  // 140
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[0] = 128 + 64,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[1] = 128 + 64,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[2] = 128 + 64,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[3] = 128 + 64,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[4] = 128 + 64,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[5] = 128 + 64,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[6] = 128 + 64,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[7] = 128 + 64,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[8] = 128 + 64,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[9] = 128 + 64 + 40,    // 366
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[10] = 128 + 64 + 40,   // 325
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[11] = 128 + 64 + 40,   // 343
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[12] = 128 + 64 + 40,   // 345
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[13] = 128 + 64 + 40,   // 361
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[14] = 128 + 64 + 40,   // 344
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[15] = 128 + 64 + 40,   // 354
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[16] = 128 + 64 + 40,   // 344
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[17] = 128 + 64 + 40,   // 348
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[18] = 128 + 64 + 90,   // 349
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[19] = 128 + 64 + 90,   // 322
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[20] = 128 + 64 + 90,   // 327
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[21] = 128 + 64 + 90,   // 316
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[22] = 128 + 64 + 90,   // 335
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[23] = 128 + 64 + 90,   // 319
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[24] = 128 + 64 + 90,   // 330
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[25] = 128 + 64 + 90,   // 331
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[26] = 128 + 64 + 90,   // 333
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[27] = 128 + 64 + 90,   // 366
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[28] = 128 + 64 + 90,   // 334
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[29] = 128 + 64 + 90,   // 356
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[30] = 128 + 64 + 90,   // 350
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[31] = 128 + 64 + 90,   // 363
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[32] = 128 + 64 + 90,   // 345
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[33] = 128 + 64 + 90,   // 355
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[34] = 128 + 64 + 90,   // 345
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[35] = 128 + 64 + 90,   // 350
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[36] = 0x00000155,      // 341
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[37] = 0x00000145,      // 325
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[38] = 0x0000014e,      // 334
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[39] = 0x0000014f,      // 335
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[40] = 0x0000014e,      // 334
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[41] = 0x00000138,      // 312
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[42] = 0x0000014d,      // 333
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[43] = 0x00000154,      // 340
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[44] = 0x00000145,      // 325
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[45] = 0x00000166,      // 358
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[46] = 0x0000013e,      // 318
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[47] = 0x00000151,      // 337
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[48] = 0x00000151,      // 337
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[49] = 0x00000161,      // 353
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[50] = 0x00000152,      // 338
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[51] = 0x0000015c,      // 348
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[52] = 0x00000150,      // 336
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[53] = 0x00000153,      // 339
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[54] = 0x00000160,      // 352
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[55] = 0x00000144,      // 324
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[56] = 0x0000014c,      // 332
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[57] = 0x00000140,      // 320
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[58] = 0x00000152,      // 338
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[59] = 0x00000142,      // 322
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[60] = 0x0000014d,      // 333
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[61] = 0x0000014e,      // 334
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[62] = 0x00000150,      // 336
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[63] = 0x00000172,      // 370
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[64] = 0x00000153,      // 339
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[65] = 0x00000168,      // 360
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[66] = 0x00000162,      // 354
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[67] = 0x0000016e,      // 366
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[68] = 0x0000015c,      // 348
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[69] = 0x00000166,      // 358
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[70] = 0x0000015c,      // 348
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[71] = 0x00000163,      // 355

		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 650,             // 933
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 660,             // 921
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 770,             // 938
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 720,             // 910

		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 700,             // 846
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 700,             // 1092
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 700,             // 852
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 700,             // 823
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x0000007f,           // 127
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x00000085,           // 133
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x00000079,           // 121
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x00000086,           // 134
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x00000081,           // 129
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x00000089,           // 137
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x00000085,           // 133
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x00000092,           // 146
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[0] = 0x0000004a,        // 70
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[1] = 0x0000003a,        // 55
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[2] = 0x0000003a,        // 71
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[3] = 0x0000004e,        // 65
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[4] = 0x00000043,        // 72
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[5] = 0x0000004c,        // 53
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[6] = 0x0000003b,        // 71
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[7] = 0x0000004f,        // 60
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[8] = 0x00000044,        // 64
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[9] = 0x00000052,        // 82
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[10] = 0x00000031,       // 49
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[11] = 0x00000044,       // 68
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[12] = 0x00000039,       // 57
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[13] = 0x00000053,       // 83
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[14] = 0x0000003f,       // 63
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[15] = 0x00000053,       // 83
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[16] = 0x0000003b,       // 59
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[17] = 0x00000044,       // 68
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[18] = 0x00000059,       // 89
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[19] = 0x00000035,       // 53
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[20] = 0x00000043,       // 67
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[21] = 0x00000035,       // 53
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[22] = 0x0000004c,       // 76
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[23] = 0x00000038,       // 56
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[24] = 0x00000045,       // 69
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[25] = 0x0000003f,       // 63
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[26] = 0x00000041,       // 65
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[27] = 0x0000005d,       // 93
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[28] = 0x00000035,       // 53
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[29] = 0x0000004b,       // 75
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[30] = 0x00000040,       // 64
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[31] = 0x0000005e,       // 94
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[32] = 0x00000039,       // 57
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[33] = 0x00000054,       // 84
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[34] = 0x00000042,       // 66
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[35] = 0x00000049,       // 73
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[36] = 0x0000004a,       // 74
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[37] = 0x00000039,       // 57
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[38] = 0x0000004c,       // 76
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[39] = 0x00000046,       // 70
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[40] = 0x0000004b,       // 75
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[41] = 0x00000036,       // 54
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[42] = 0x0000004d,       // 77
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[43] = 0x0000003f,       // 63
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[44] = 0x00000044,       // 68
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[45] = 0x00000055,       // 85
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[46] = 0x00000032,       // 50
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[47] = 0x00000048,       // 72
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[48] = 0x0000003d,       // 61
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[49] = 0x00000057,       // 87
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[50] = 0x00000044,       // 68
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[51] = 0x00000055,       // 85
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[52] = 0x00000040,       // 64
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[53] = 0x00000047,       // 71
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[54] = 0x00000062,       // 98
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[55] = 0x0000003d,       // 61
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[56] = 0x0000004f,       // 79
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[57] = 0x0000003c,       // 60
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[58] = 0x00000056,       // 86
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[59] = 0x00000042,       // 66
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[60] = 0x00000050,       // 80
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[61] = 0x00000049,       // 73
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[62] = 0x0000004b,       // 75
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[63] = 0x00000067,       // 103
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[64] = 0x0000003d,       // 61
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[65] = 0x00000055,       // 85
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[66] = 0x0000004a,       // 74
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[67] = 0x00000066,       // 102
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[68] = 0x00000042,       // 66
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[69] = 0x0000005e,       // 94
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[70] = 0x0000004b,       // 75
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[71] = 0x00000052,       // 82
#endif
#if 1
		.cfg_ddr_training_delay_ps.soc_bit_vref[0] = 0,              //32//0x00000018,// 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[1] = 0x20,           // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[2] = 0x20,           // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[3] = 0x20,           // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[4] = 0x20,           // 25
		.cfg_ddr_training_delay_ps.soc_bit_vref[5] = 0x20,           // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[6] = 0x20,           // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[7] = 0x20,           // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[8] = 0x20,           // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[9] = 0x20,           // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[10] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[11] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[12] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[13] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[14] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[15] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[16] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[17] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[18] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[19] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[20] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[21] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[22] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[23] = 0x20,          // 25
		.cfg_ddr_training_delay_ps.soc_bit_vref[24] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[25] = 0x20,          // 25
		.cfg_ddr_training_delay_ps.soc_bit_vref[26] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[27] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[28] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[29] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[30] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[31] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[32] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[33] = 0x24,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[34] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[35] = 0x24,          // 24
#endif
#if 1
		.cfg_ddr_training_delay_ps.soc_bit_vref[36] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[37] = 0x24,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[38] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[39] = 0x24,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[40] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[41] = 0x24,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[42] = 0x20,          // 24
		.cfg_ddr_training_delay_ps.soc_bit_vref[43] = 0x24,          // 24
#endif
#if 1
		.cfg_ddr_training_delay_ps.dram_bit_vref[0] = 0x00000000,    // 0X5b,// 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[1] = 0x00000000,    // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[2] = 0x00000000,    // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[3] = 0x00000000,    // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[4] = 0x00000000,    // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[5] = 0x00000000,    // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[6] = 0x00000000,    // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[7] = 0x00000000,    // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[8] = 0x00000000,    // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[9] = 0x00000000,    // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[10] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[11] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[12] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[13] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[14] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[15] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[16] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[17] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[18] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[19] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[20] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[21] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[22] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[23] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[24] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[25] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[26] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[27] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[28] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[29] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[30] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[31] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[32] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[33] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[34] = 0x00000000,   // 0
		.cfg_ddr_training_delay_ps.dram_bit_vref[35] = 0x00000000,   // 0
#endif
		.cfg_ddr_training_delay_ps.reserve_training_parameter = {
			//cs0 write dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs1 write dqs,lane0-lane3
			(0 << 7) | 8, (0 << 7) | 8, (0 << 7) | 8, (0 << 7) | 8,
			//cs0 read dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs1 read dqs,lane0-lane3
			(1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16,
		},
	},
#endif  //end ddr3

#if S4_LPDDR4_1RANK
	{
		//timing_config,4layer lpddr4 rank0, ap223.
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs =
			sizeof(__ddr_setting[0]) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,
		.cfg_board_common_setting.fast_boot = {1, 0, 0, 0xc2},
		.cfg_board_common_setting.ddr_func = DDR_FUNC,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_LPDDR4,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
		.cfg_board_common_setting.DisabledDbyte = CONFIG_DISABLE_D32_D63,
		.cfg_board_common_setting.dram_cs0_base_add = 0,
		.cfg_board_common_setting.dram_cs1_base_add = 0,
		.cfg_board_common_setting.dram_cs0_size_MB = CONFIG_DDR0_SIZE_2048MB,
		.cfg_board_common_setting.dram_cs1_size_MB = CONFIG_DDR1_SIZE_0MB,
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_1T_MODE,
		.cfg_board_common_setting.log_level = LOG_LEVEL_BASIC,
		.cfg_board_common_setting.ddr_rdbi_wr_enable = DDR_WRITE_READ_DBI_DISABLE,
		.cfg_board_common_setting.pll_ssc_mode = DDR_PLL_SSC_DISABLE,
		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test_function = { 0 },
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_LPDDR4_32BIT,
		.cfg_board_common_setting.ac_pinmux = {
			0, 0, 0, 1, 0, 1, 2, 0,
			5, 1, 0, 0, 0, 1, 0, 2,
			0, 3, 0, 3, 5, 0, 0, 0,
			0, 0, 4, 4, 0, 0, 0, 0,
			0, 0, 0
			},
		.cfg_board_common_setting.ddr_dqs_swap = 0,
		#if BOARD_AP223_LPDDR4
		.cfg_board_common_setting.ddr_dq_remap = {
			8,  12, 13, 11, 14, 9,	10, 15,
			0,  2,	5,  6,	3,  7,	1,  4,
			27, 31, 28, 24, 25, 29, 26, 30,
			21, 18, 22, 19, 16, 17, 23, 20,
			33, 32, 35, 34
		},//d0-d31 dm0 dm1 dm2 dm3;T212,AP223
		#else
		.cfg_board_common_setting.ddr_dq_remap = {
			15, 8,	12, 13, 11, 14, 10, 9,
			7,  3,	0,  1,	4,  5,	2,  6,
			16, 19, 23, 21, 18, 22, 20, 17,
			31, 27, 30, 26, 25, 29, 24, 28,
			33, 32, 34, 35
		},//d0-d31 dm0 dm1 dm2 dm3;
		#endif
		.cfg_board_common_setting.ddr_vddee_setting = { 0 },
		.cfg_board_SI_setting_ps.DRAMFreq = 1176,
		.cfg_board_SI_setting_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		.cfg_board_SI_setting_ps.ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		.cfg_board_SI_setting_ps.clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_48_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_48_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_0_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_80_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = DDR_DRAM_LPDDR4_DRV_60_OHM,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = DDR_DRAM_LPDDR4_ODT_60_OHM,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = DDR_DRAM_DDR_WR_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = DDR_DRAM_LPDDR4_AC_ODT_120_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_pull_up_calibration_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_voltage_range_setting =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		.cfg_board_SI_setting_ps.dfe_offset = 0,
		.cfg_board_SI_setting_ps.vref_ac_permil = 320,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 180,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,  //330
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,
		.cfg_board_SI_setting_ps.training_phase_parameter = { 0 },
		.cfg_board_SI_setting_ps.ac_trace_delay_org = {
			192, 192, 128, 128, 192,
			192, 128, 128,
			276, 276, 272,
			240, 256,
			256, 256, 256,
			256, 256, 256,
			256, 256, 256, 256, 256,
			256, 256, 256,
			256, 256, 256, 256, 256,
			256, 256, 256, 256,
		},
		.cfg_ddr_training_delay_ps.ac_trace_delay = {
			192, 192, 128, 128, 192,
			192, 128, 256,
			226, 221, 272,
			240, 256, 256, 256, 256,
			256, 256, 256,
			256, 256, 256, 256, 256,
			256, 256, 256,
			256, 256, 256, 256, 256,
			256, 256, 256, 256,
		},
		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x00000080,
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x00000073,
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x00000094,
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0x0000007c,
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x00000080,
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x00000080,
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x0000009b,
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x0000007b,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[0] = 0x00000173,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[1] = 0x00000171,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[2] = 0x0000015b,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[3] = 0x00000159,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[4] = 0x0000016f,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[5] = 0x00000146,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[6] = 0x00000163,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[7] = 0x0000015a,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[8] = 0x0000015a,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[9] = 0x00000160,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[10] = 0x0000015d,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[11] = 0x00000140,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[12] = 0x00000159,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[13] = 0x00000154,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[14] = 0x00000154,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[15] = 0x00000154,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[16] = 0x00000155,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[17] = 0x0000014d,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[18] = 0x0000016b,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[19] = 0x0000016b,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[20] = 0x0000017e,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[21] = 0x00000184,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[22] = 0x00000158,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[23] = 0x00000163,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[24] = 0x00000175,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[25] = 0x00000172,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[26] = 0x0000016e,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[27] = 0x00000159,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[28] = 0x00000164,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[29] = 0x00000166,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[30] = 0x00000153,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[31] = 0x00000171,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[32] = 0x00000158,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[33] = 0x00000165,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[34] = 0x0000016a,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[35] = 0x00000162,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[36] = 0x00000172,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[37] = 0x00000171,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[38] = 0x0000015b,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[39] = 0x00000159,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[40] = 0x00000170,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[41] = 0x00000146,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[42] = 0x00000164,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[43] = 0x00000159,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[44] = 0x0000015a,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[45] = 0x00000169,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[46] = 0x00000166,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[47] = 0x0000014e,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[48] = 0x00000163,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[49] = 0x00000160,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[50] = 0x0000015f,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[51] = 0x0000015f,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[52] = 0x00000162,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[53] = 0x0000015b,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[54] = 0x00000176,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[55] = 0x00000175,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[56] = 0x00000189,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[57] = 0x0000018d,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[58] = 0x00000162,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[59] = 0x0000016d,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[60] = 0x0000017f,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[61] = 0x0000017d,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[62] = 0x00000178,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[63] = 0x00000159,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[64] = 0x00000164,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[65] = 0x00000165,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[66] = 0x00000154,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[67] = 0x00000170,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[68] = 0x0000015a,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[69] = 0x00000164,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[70] = 0x0000016b,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[71] = 0x00000162,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0x00000133,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0x0000014c,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0x0000013d,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0x00000152,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 0x00000133,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 0x0000014c,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 0x0000013d,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 0x00000152,
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x00000076,
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x0000006a,
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x00000071,
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x00000069,
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x0000007c,
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x00000071,
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x00000075,
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x00000073,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[0] = 0x00000057,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[1] = 0x00000058,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[2] = 0x00000045,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[3] = 0x00000036,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[4] = 0x0000005a,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[5] = 0x00000030,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[6] = 0x00000045,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[7] = 0x0000003d,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[8] = 0x00000046,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[9] = 0x0000003c,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[10] = 0x0000003c,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[11] = 0x00000030,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[12] = 0x00000046,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[13] = 0x00000037,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[14] = 0x00000039,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[15] = 0x00000035,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[16] = 0x00000042,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[17] = 0x0000003a,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[18] = 0x00000037,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[19] = 0x00000037,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[20] = 0x00000056,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[21] = 0x00000059,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[22] = 0x0000002d,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[23] = 0x00000032,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[24] = 0x00000043,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[25] = 0x00000049,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[26] = 0x00000041,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[27] = 0x00000036,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[28] = 0x00000045,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[29] = 0x00000046,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[30] = 0x00000031,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[31] = 0x00000054,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[32] = 0x00000033,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[33] = 0x0000003f,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[34] = 0x0000004d,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[35] = 0x00000040,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[36] = 0x0000005f,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[37] = 0x00000060,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[38] = 0x0000004d,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[39] = 0x0000003e,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[40] = 0x00000063,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[41] = 0x00000035,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[42] = 0x0000004d,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[43] = 0x00000045,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[44] = 0x0000004e,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[45] = 0x0000004b,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[46] = 0x0000004b,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[47] = 0x00000037,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[48] = 0x00000053,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[49] = 0x00000045,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[50] = 0x00000046,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[51] = 0x0000003f,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[52] = 0x0000004f,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[53] = 0x00000047,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[54] = 0x0000003e,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[55] = 0x0000003d,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[56] = 0x0000005b,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[57] = 0x00000061,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[58] = 0x0000002f,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[59] = 0x00000038,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[60] = 0x00000049,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[61] = 0x0000004e,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[62] = 0x00000046,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[63] = 0x0000003a,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[64] = 0x0000004d,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[65] = 0x0000004e,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[66] = 0x00000033,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[67] = 0x00000059,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[68] = 0x00000037,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[69] = 0x00000045,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[70] = 0x00000050,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[71] = 0x00000045,
		.cfg_ddr_training_delay_ps.soc_bit_vref[0] = 0,
		.cfg_ddr_training_delay_ps.soc_bit_vref[1] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[2] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[3] = 0x00000018,
		.cfg_ddr_training_delay_ps.soc_bit_vref[4] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[5] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[6] = 0x00000018,
		.cfg_ddr_training_delay_ps.soc_bit_vref[7] = 0x00000016,
		.cfg_ddr_training_delay_ps.soc_bit_vref[8] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[9] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[10] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[11] = 0x00000018,
		.cfg_ddr_training_delay_ps.soc_bit_vref[12] = 0x00000016,
		.cfg_ddr_training_delay_ps.soc_bit_vref[13] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[14] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[15] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[16] = 0x00000016,
		.cfg_ddr_training_delay_ps.soc_bit_vref[17] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[18] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[19] = 0x00000015,
		.cfg_ddr_training_delay_ps.soc_bit_vref[20] = 0x00000015,
		.cfg_ddr_training_delay_ps.soc_bit_vref[21] = 0x00000016,
		.cfg_ddr_training_delay_ps.soc_bit_vref[22] = 0x00000015,
		.cfg_ddr_training_delay_ps.soc_bit_vref[23] = 0x00000015,
		.cfg_ddr_training_delay_ps.soc_bit_vref[24] = 0x00000016,
		.cfg_ddr_training_delay_ps.soc_bit_vref[25] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[26] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[27] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[28] = 0x00000014,
		.cfg_ddr_training_delay_ps.soc_bit_vref[29] = 0x00000018,
		.cfg_ddr_training_delay_ps.soc_bit_vref[30] = 0x00000016,
		.cfg_ddr_training_delay_ps.soc_bit_vref[31] = 0x00000016,
		.cfg_ddr_training_delay_ps.soc_bit_vref[32] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[33] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[34] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[35] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[36] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[37] = 0x00000019,
		.cfg_ddr_training_delay_ps.soc_bit_vref[38] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[39] = 0x00000018,
		.cfg_ddr_training_delay_ps.soc_bit_vref[40] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[41] = 0x00000018,
		.cfg_ddr_training_delay_ps.soc_bit_vref[42] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[43] = 0x00000018,
		.cfg_ddr_training_delay_ps.dram_bit_vref[0] = 0x56,
	#ifdef S4_LPDDR4_DONGLE_LAYER_4
	.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		//cs0 write dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		//cs1 write dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		//cs0 read dqs,lane0-lane3
		(1 << 7) | 8, (1 << 7) | 16, (1 << 7) | 8, (1 << 7) | 16,
		//cs1 read dqs,lane0-lane3
		(1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16,
	},
	#endif
	#ifdef S4_LPDDR4_DONGLE_LAYER_6
	.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		//cs0 write dqs,lane0-lane3
		(0 << 7) | 8, (0 << 7) | 8, (0 << 7) | 8, (0 << 7) | 8,
		//cs1 write dqs,lane0-lane3
		(0 << 7) | 8, (0 << 7) | 8, (0 << 7) | 8, (0 << 7) | 8,
		//cs0 read dqs,lane0-lane3
		(1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16,
		//cs1 read dqs,lane0-lane3
		(1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16,
	},
	#endif
	#if S4_LPDDR4_1RANK_RAYSON_2GB
	.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		//cs0 write dqs,lane0-lane3
		(0 << 7) | 1, (0 << 7) | 2, (0 << 7) | 4, (0 << 7) | 8,
		//cs1 write dqs,lane0-lane3
		(0 << 7) | 8, (0 << 7) | 18, (0 << 7) | 5, (0 << 7) | 13,
		//cs0 read dqs,lane0-lane3
		(1 << 7) | 12, (1 << 7) | 15, (1 << 7) | 10, (1 << 7) | 14,
		//cs1 read dqs,lane0-lane3
		(1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 13, (1 << 7) | 10,
	},//1 RANK RAYSON 2GB
	#endif
	},
#endif  //end S4_LPDDR4_1RANK

#if S4_LPDDR4_2RANK
	{
		//timing_config,4layer lpddr4 rank01, ap223.
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs =
			sizeof(__ddr_setting[0]) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,
		.cfg_board_common_setting.fast_boot = {1, 0, 0, 0xc2},
		.cfg_board_common_setting.ddr_func = DDR_FUNC,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_LPDDR4,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK01_CH0,
		.cfg_board_common_setting.DisabledDbyte = CONFIG_DISABLE_D32_D63,
		.cfg_board_common_setting.dram_cs0_base_add = 0,
		.cfg_board_common_setting.dram_cs1_base_add = 0,
		.cfg_board_common_setting.dram_cs0_size_MB = CONFIG_DDR0_SIZE_1024MB,
		.cfg_board_common_setting.dram_cs1_size_MB = CONFIG_DDR1_SIZE_1024MB,
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_1T_MODE,
		.cfg_board_common_setting.log_level = LOG_LEVEL_BASIC,
		.cfg_board_common_setting.ddr_rdbi_wr_enable = DDR_WRITE_READ_DBI_DISABLE,
		.cfg_board_common_setting.pll_ssc_mode = DDR_PLL_SSC_DISABLE,
		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test_function = { 0 },
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_LPDDR4_32BIT,
		.cfg_board_common_setting.ac_pinmux = {
			0, 0, 0, 1, 0, 1, 2, 0,
			5, 1, 0, 0, 0, 1, 0, 2,
			0, 3, 0, 3, 5, 0, 0, 0,
			0, 0, 4, 4, 0, 0, 0, 0,
			0, 0, 0
			},
		.cfg_board_common_setting.ddr_dqs_swap = 0,
		#if BOARD_AP223_LPDDR4
		.cfg_board_common_setting.ddr_dq_remap = {
			8,  12, 13, 11, 14, 9,	10, 15,
			0,  2,	5,  6,	3,  7,	1,  4,
			27, 31, 28, 24, 25, 29, 26, 30,
			21, 18, 22, 19, 16, 17, 23, 20,
			33, 32, 35, 34
		},//d0-d31 dm0 dm1 dm2 dm3;T212,AP223
		#else
		.cfg_board_common_setting.ddr_dq_remap = {
			15, 8,	12, 13, 11, 14, 10, 9,
			7,  3,	0,  1,	4,  5,	2,  6,
			16, 19, 23, 21, 18, 22, 20, 17,
			31, 27, 30, 26, 25, 29, 24, 28,
			33, 32, 34, 35
		},//d0-d31 dm0 dm1 dm2 dm3;
		#endif

		.cfg_board_common_setting.ddr_vddee_setting = { 0 },
		.cfg_board_SI_setting_ps.DRAMFreq = 1176,
		.cfg_board_SI_setting_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		.cfg_board_SI_setting_ps.ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		.cfg_board_SI_setting_ps.clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_48_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_48_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_0_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_80_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = DDR_DRAM_LPDDR4_DRV_60_OHM,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = DDR_DRAM_LPDDR4_ODT_60_OHM,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = DDR_DRAM_DDR_WR_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = DDR_DRAM_LPDDR4_AC_ODT_120_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_pull_up_calibration_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_voltage_range_setting =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		.cfg_board_SI_setting_ps.dfe_offset = 0,
		.cfg_board_SI_setting_ps.vref_ac_permil = 370,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 310,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,
		.cfg_board_SI_setting_ps.training_phase_parameter = { 0 },
		.cfg_board_SI_setting_ps.ac_trace_delay_org = {
			192, 192, 128, 128, 192,
			192, 128, 128,
			276, 276, 272,
			240, 256,
			256, 256, 256,
			256, 256, 256,
			256, 256, 256, 256, 256,
			256, 256, 256,
			256, 256, 256, 256, 256,
			256, 256, 256, 256,
		},
		.cfg_ddr_training_delay_ps.ac_trace_delay = {
			192, 192, 128, 128, 192,
			192, 128, 256,
			256, 276, 272,
			240, 256, 256, 256, 256,
			256, 256, 256,
			256, 256, 256, 256, 256,
			256, 256, 256,
			256, 256, 256, 256, 256,
			256, 256, 256, 256,
		},
		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x00000080,
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x00000073,
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x00000094,
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0x0000007c,
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x00000080,
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x00000080,
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x0000009b,
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x0000007b,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[0] = 0x00000173,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[1] = 0x00000171,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[2] = 0x0000015b,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[3] = 0x00000159,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[4] = 0x0000016f,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[5] = 0x00000146,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[6] = 0x00000163,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[7] = 0x0000015a,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[8] = 0x0000015a,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[9] = 0x00000160,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[10] = 0x0000015d,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[11] = 0x00000140,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[12] = 0x00000159,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[13] = 0x00000154,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[14] = 0x00000154,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[15] = 0x00000154,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[16] = 0x00000155,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[17] = 0x0000014d,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[18] = 0x0000016b,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[19] = 0x0000016b,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[20] = 0x0000017e,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[21] = 0x00000184,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[22] = 0x00000158,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[23] = 0x00000163,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[24] = 0x00000175,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[25] = 0x00000172,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[26] = 0x0000016e,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[27] = 0x00000159,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[28] = 0x00000164,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[29] = 0x00000166,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[30] = 0x00000153,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[31] = 0x00000171,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[32] = 0x00000158,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[33] = 0x00000165,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[34] = 0x0000016a,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[35] = 0x00000162,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[36] = 0x00000172,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[37] = 0x00000171,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[38] = 0x0000015b,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[39] = 0x00000159,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[40] = 0x00000170,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[41] = 0x00000146,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[42] = 0x00000164,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[43] = 0x00000159,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[44] = 0x0000015a,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[45] = 0x00000169,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[46] = 0x00000166,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[47] = 0x0000014e,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[48] = 0x00000163,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[49] = 0x00000160,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[50] = 0x0000015f,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[51] = 0x0000015f,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[52] = 0x00000162,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[53] = 0x0000015b,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[54] = 0x00000176,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[55] = 0x00000175,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[56] = 0x00000189,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[57] = 0x0000018d,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[58] = 0x00000162,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[59] = 0x0000016d,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[60] = 0x0000017f,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[61] = 0x0000017d,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[62] = 0x00000178,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[63] = 0x00000159,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[64] = 0x00000164,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[65] = 0x00000165,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[66] = 0x00000154,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[67] = 0x00000170,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[68] = 0x0000015a,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[69] = 0x00000164,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[70] = 0x0000016b,
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[71] = 0x00000162,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0x00000133,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0x0000014c,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0x0000013d,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0x00000152,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 0x00000133,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 0x0000014c,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 0x0000013d,
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 0x00000152,
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x00000076,
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x0000006a,
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x00000071,
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x00000069,
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x0000007c,
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x00000071,
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x00000075,
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x00000073,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[0] = 0x00000057,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[1] = 0x00000058,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[2] = 0x00000045,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[3] = 0x00000036,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[4] = 0x0000005a,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[5] = 0x00000030,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[6] = 0x00000045,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[7] = 0x0000003d,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[8] = 0x00000046,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[9] = 0x0000003c,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[10] = 0x0000003c,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[11] = 0x00000030,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[12] = 0x00000046,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[13] = 0x00000037,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[14] = 0x00000039,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[15] = 0x00000035,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[16] = 0x00000042,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[17] = 0x0000003a,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[18] = 0x00000037,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[19] = 0x00000037,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[20] = 0x00000056,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[21] = 0x00000059,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[22] = 0x0000002d,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[23] = 0x00000032,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[24] = 0x00000043,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[25] = 0x00000049,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[26] = 0x00000041,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[27] = 0x00000036,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[28] = 0x00000045,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[29] = 0x00000046,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[30] = 0x00000031,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[31] = 0x00000054,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[32] = 0x00000033,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[33] = 0x0000003f,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[34] = 0x0000004d,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[35] = 0x00000040,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[36] = 0x0000005f,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[37] = 0x00000060,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[38] = 0x0000004d,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[39] = 0x0000003e,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[40] = 0x00000063,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[41] = 0x00000035,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[42] = 0x0000004d,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[43] = 0x00000045,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[44] = 0x0000004e,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[45] = 0x0000004b,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[46] = 0x0000004b,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[47] = 0x00000037,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[48] = 0x00000053,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[49] = 0x00000045,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[50] = 0x00000046,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[51] = 0x0000003f,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[52] = 0x0000004f,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[53] = 0x00000047,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[54] = 0x0000003e,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[55] = 0x0000003d,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[56] = 0x0000005b,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[57] = 0x00000061,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[58] = 0x0000002f,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[59] = 0x00000038,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[60] = 0x00000049,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[61] = 0x0000004e,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[62] = 0x00000046,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[63] = 0x0000003a,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[64] = 0x0000004d,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[65] = 0x0000004e,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[66] = 0x00000033,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[67] = 0x00000059,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[68] = 0x00000037,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[69] = 0x00000045,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[70] = 0x00000050,
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[71] = 0x00000045,
		.cfg_ddr_training_delay_ps.soc_bit_vref[0] = 0,
		.cfg_ddr_training_delay_ps.soc_bit_vref[1] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[2] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[3] = 0x00000018,
		.cfg_ddr_training_delay_ps.soc_bit_vref[4] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[5] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[6] = 0x00000018,
		.cfg_ddr_training_delay_ps.soc_bit_vref[7] = 0x00000016,
		.cfg_ddr_training_delay_ps.soc_bit_vref[8] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[9] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[10] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[11] = 0x00000018,
		.cfg_ddr_training_delay_ps.soc_bit_vref[12] = 0x00000016,
		.cfg_ddr_training_delay_ps.soc_bit_vref[13] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[14] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[15] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[16] = 0x00000016,
		.cfg_ddr_training_delay_ps.soc_bit_vref[17] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[18] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[19] = 0x00000015,
		.cfg_ddr_training_delay_ps.soc_bit_vref[20] = 0x00000015,
		.cfg_ddr_training_delay_ps.soc_bit_vref[21] = 0x00000016,
		.cfg_ddr_training_delay_ps.soc_bit_vref[22] = 0x00000015,
		.cfg_ddr_training_delay_ps.soc_bit_vref[23] = 0x00000015,
		.cfg_ddr_training_delay_ps.soc_bit_vref[24] = 0x00000016,
		.cfg_ddr_training_delay_ps.soc_bit_vref[25] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[26] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[27] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[28] = 0x00000014,
		.cfg_ddr_training_delay_ps.soc_bit_vref[29] = 0x00000018,
		.cfg_ddr_training_delay_ps.soc_bit_vref[30] = 0x00000016,
		.cfg_ddr_training_delay_ps.soc_bit_vref[31] = 0x00000016,
		.cfg_ddr_training_delay_ps.soc_bit_vref[32] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[33] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[34] = 0x00000017,
		.cfg_ddr_training_delay_ps.soc_bit_vref[35] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[36] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[37] = 0x00000019,
		.cfg_ddr_training_delay_ps.soc_bit_vref[38] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[39] = 0x00000018,
		.cfg_ddr_training_delay_ps.soc_bit_vref[40] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[41] = 0x00000018,
		.cfg_ddr_training_delay_ps.soc_bit_vref[42] = 0x00000010,
		.cfg_ddr_training_delay_ps.soc_bit_vref[43] = 0x00000018,
		.cfg_ddr_training_delay_ps.dram_bit_vref[0] = 0x56,
	#ifdef S4_LPDDR4_DONGLE_LAYER_4
	.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		//cs0 write dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		//cs1 write dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		//cs0 read dqs,lane0-lane3
		(1 << 7) | 8, (1 << 7) | 16, (1 << 7) | 8, (1 << 7) | 16,
		//cs1 read dqs,lane0-lane3
		(1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16,
	},
	#endif
	#ifdef S4_LPDDR4_DONGLE_LAYER_6
	.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		//cs0 write dqs,lane0-lane3
		(0 << 7) | 8, (0 << 7) | 8, (0 << 7) | 8, (0 << 7) | 8,
		//cs1 write dqs,lane0-lane3
		(0 << 7) | 8, (0 << 7) | 8, (0 << 7) | 8, (0 << 7) | 8,
		//cs0 read dqs,lane0-lane3
		(1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16,
		//cs1 read dqs,lane0-lane3
		(1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 16,
	},
	#endif
	#if S4_LPDDR4_2RANK_RAYSON_2GB
	.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		//cs0 write dqs,lane0-lane3
		(0 << 7) | 3, (0 << 7) | 3, (0 << 7) | 2, (0 << 7) | 0,
		//cs1 write dqs,lane0-lane3
		(0 << 7) | 3, (0 << 7) | 4, (0 << 7) | 0, (0 << 7) | 3,
		//cs0 read dqs,lane0-lane3
		(1 << 7) | 12, (1 << 7) | 10, (1 << 7) | 16, (1 << 7) | 14,
		//cs1 read dqs,lane0-lane3
		(1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 13, (1 << 7) | 10,
	},//2 RANK RAYSON 2GB
	#endif
	#if S4_LPDDR4_2RANK_FORESEE_2GB
	.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		//cs0 write dqs,lane0-lane3
		(0 << 7) | 2, (0 << 7) | 4, (0 << 7) | 0, (0 << 7) | 2,
		//cs1 write dqs,lane0-lane3
		(0 << 7) | 3, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 3,
		//cs0 read dqs,lane0-lane3
		(1 << 7) | 12, (1 << 7) | 15, (1 << 7) | 9, (1 << 7) | 14,
		//cs1 read dqs,lane0-lane3
		(1 << 7) | 12, (1 << 7) | 18, (1 << 7) | 15, (1 << 7) | 12,
	},//2 RANK foresee 2GB
	#endif
	},
#endif  //end S4_LPDDR4_2RANK

#if S4_DDR4_2RANK
	{
		//timing_config,4layer 4pcs ddr4 rank01, ap222.
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs =
			sizeof(__ddr_setting[0]) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,//0
		.cfg_board_common_setting.fast_boot = {0, 0, (1 << 3) | (2)},
		.cfg_board_common_setting.ddr_func = DDR_FUNC,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_DDR4,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK01_CH0,
		.cfg_board_common_setting.DisabledDbyte = CONFIG_DISABLE_D32_D63,
		.cfg_board_common_setting.dram_cs0_base_add = 0,
		.cfg_board_common_setting.dram_cs1_base_add = 0,
		.cfg_board_common_setting.dram_cs0_size_MB = CONFIG_DDR0_SIZE_1024MB,
		.cfg_board_common_setting.dram_cs1_size_MB = CONFIG_DDR1_SIZE_1024MB,
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		.cfg_board_common_setting.log_level = LOG_LEVEL_BASIC,
		.cfg_board_common_setting.ddr_rdbi_wr_enable = DDR_WRITE_READ_DBI_DISABLE,
		//SSC_DISABLE,(1 << 20) | (0 << 8) | (2 << 4) | 0,
		//center_ssc_1000ppm,
		.cfg_board_common_setting.pll_ssc_mode = (1 << 20) | (1 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test_function = { 0 },
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR4_32BIT,
		//s4 ddr4 ac pinmux
		.cfg_board_common_setting.ac_pinmux = {
			0,	0,	0,	1,	2,	3,
			27,	10,	25,	5,	28,	11,	24,	0,
			26,	7,	4,	21,	2,	20,	8,	13,
			0,	0,	0,	0,	6,	12,	3,	0,
			0,	9,	1,	23,	0
		},
		.cfg_board_common_setting.ddr_dqs_swap = 0,
		.cfg_board_common_setting.ddr_dq_remap = {
			0,  1,	2,  3,	4,  5,	6,  7,
			8,  9,	10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, 29, 30, 31,
			32, 33, 34, 35
		},//d0-d31 dm0 dm1 dm2 dm3
		.cfg_board_common_setting.ddr_vddee_setting = { 0 },
		.cfg_board_SI_setting_ps.DRAMFreq = 1176,
		.cfg_board_SI_setting_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		.cfg_board_SI_setting_ps.ddr_odt_config =
		DDR_DRAM_ODT_W_CS0_ODT01_CS1_ODT01__R_CS0_ODT1_CS1_ODT0,
		.cfg_board_SI_setting_ps.clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_34_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_34_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_120_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_240_OHM,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_pull_up_calibration_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_voltage_range_setting =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		.cfg_board_SI_setting_ps.dfe_offset = 0,
		.cfg_board_SI_setting_ps.vref_ac_permil = 0,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 750, //820,// 750,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 750,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,
		.cfg_board_SI_setting_ps.training_phase_parameter = { 0 },
		.cfg_board_SI_setting_ps.ac_trace_delay_org = {
			128, 128, 128 - 40, 128, 128, 128, 128, 128,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384,
		},//total 36
		.cfg_ddr_training_delay_ps.ac_trace_delay = {
			128, 128, 128 - 40, 128, 128, 128, 128, 384,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384,
		},

		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x000000e1,          // 225
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x000000eb,          // 235
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x000000e1,          // 225
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0x000000d7,          // 215
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x000000d7,          // 215
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x000000e1,          // 225
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x000000e1,          // 225
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x000000d7,          // 215
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[0] = 0x00000130,       // 304
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[1] = 0x00000120,       // 288
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[2] = 0x00000132,       // 306
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[3] = 0x00000125,       // 293
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[4] = 0x0000012b,       // 299
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[5] = 0x00000115,       // 277
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[6] = 0x00000131,       // 305
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[7] = 0x0000012d,       // 301
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[8] = 0x00000124,       // 292
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[9] = 0x00000134,       // 308
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[10] = 0x00000115,      // 277
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[11] = 0x00000127,      // 295
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[12] = 0x0000011f,      // 287
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[13] = 0x00000141,      // 321
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[14] = 0x00000129,      // 297
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[15] = 0x00000137,      // 311
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[16] = 0x00000120,      // 288
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[17] = 0x0000012a,      // 298
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[18] = 0x00000127,      // 295
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[19] = 0x00000112,      // 274
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[20] = 0x00000119,      // 281
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[21] = 0x00000115,      // 277
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[22] = 0x0000012b,      // 299
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[23] = 0x0000011c,      // 284
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[24] = 0x00000116,      // 278
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[25] = 0x00000120,      // 288
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[26] = 0x0000011f,      // 287
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[27] = 0x0000012b,      // 299
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[28] = 0x00000113,      // 275
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[29] = 0x0000011f,      // 287
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[30] = 0x00000117,      // 279
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[31] = 0x00000126,      // 294
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[32] = 0x00000117,      // 279
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[33] = 0x0000011c,      // 284
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[34] = 0x00000113,      // 275
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[35] = 0x0000011f,      // 287
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[36] = 0x0000012d,      // 301
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[37] = 0x0000011f,      // 287
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[38] = 0x00000125,      // 293
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[39] = 0x0000011e,      // 286
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[40] = 0x0000011a,      // 282
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[41] = 0x00000108,      // 264
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[42] = 0x00000121,      // 289
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[43] = 0x0000011f,      // 287
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[44] = 0x0000011b,      // 283
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[45] = 0x0000012a,      // 298
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[46] = 0x0000010c,      // 268
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[47] = 0x00000118,      // 280
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[48] = 0x00000115,      // 277
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[49] = 0x0000012f,      // 303
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[50] = 0x0000011f,      // 287
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[51] = 0x00000126,      // 294
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[52] = 0x00000118,      // 280
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[53] = 0x0000011d,      // 285
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[54] = 0x00000139,      // 313
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[55] = 0x0000011b,      // 283
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[56] = 0x0000011e,      // 286
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[57] = 0x00000123,      // 291
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[58] = 0x00000127,      // 295
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[59] = 0x00000118,      // 280
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[60] = 0x0000011b,      // 283
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[61] = 0x0000011e,      // 286
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[62] = 0x0000012d,      // 301
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[63] = 0x0000012c,      // 300
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[64] = 0x00000117,      // 279
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[65] = 0x00000122,      // 290
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[66] = 0x00000118,      // 280
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[67] = 0x00000127,      // 295
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[68] = 0x00000119,      // 281
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[69] = 0x0000011c,      // 284
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[70] = 0x00000118,      // 280
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[71] = 0x00000122,      // 290
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0x00000376,      // 886
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0x00000372,      // 882
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0x00000366,      // 870
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0x0000036c,      // 876
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 0x00000378,      // 888
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 0x00000373,      // 883
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 0x0000036d,      // 877
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 0x00000371,      // 881
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x00000059,           // 89
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x00000063,           // 99
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x00000075,           // 117
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x00000060,           // 96
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x00000071,           // 113
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x00000086,           // 134
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x00000079,           // 121
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x00000086,           // 134
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[0] = 0x00000035,        // 53
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[1] = 0x00000028,        // 40
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[2] = 0x0000003a,        // 58
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[3] = 0x0000002e,        // 46
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[4] = 0x00000033,        // 51
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[5] = 0x00000028,        // 40
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[6] = 0x00000039,        // 57
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[7] = 0x0000002f,        // 47
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[8] = 0x00000031,        // 49
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[9] = 0x0000003b,        // 59
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[10] = 0x0000002a,       // 42
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[11] = 0x00000035,       // 53
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[12] = 0x0000002c,       // 44
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[13] = 0x0000004a,       // 74
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[14] = 0x00000034,       // 52
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[15] = 0x00000042,       // 66
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[16] = 0x0000002d,       // 45
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[17] = 0x00000036,       // 54
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[18] = 0x0000003b,       // 59
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[19] = 0x0000002b,       // 43
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[20] = 0x00000036,       // 54
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[21] = 0x0000002c,       // 44
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[22] = 0x0000004a,       // 74
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[23] = 0x00000039,       // 57
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[24] = 0x0000003b,       // 59
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[25] = 0x00000038,       // 56
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[26] = 0x00000037,       // 55
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[27] = 0x00000037,       // 55
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[28] = 0x00000027,       // 39
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[29] = 0x00000032,       // 50
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[30] = 0x0000002b,       // 43
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[31] = 0x0000003e,       // 62
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[32] = 0x00000029,       // 41
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[33] = 0x00000031,       // 49
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[34] = 0x0000002a,       // 42
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[35] = 0x0000002f,       // 47
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[36] = 0x0000004a,       // 74
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[37] = 0x00000041,       // 65
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[38] = 0x00000045,       // 69
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[39] = 0x0000003f,       // 63
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[40] = 0x00000044,       // 68
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[41] = 0x00000031,       // 49
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[42] = 0x00000049,       // 73
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[43] = 0x00000040,       // 64
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[44] = 0x00000041,       // 65
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[45] = 0x00000050,       // 80
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[46] = 0x00000035,       // 53
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[47] = 0x0000004c,       // 76
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[48] = 0x0000003a,       // 58
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[49] = 0x00000060,       // 96
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[50] = 0x00000044,       // 68
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[51] = 0x00000055,       // 85
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[52] = 0x00000039,       // 57
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[53] = 0x00000047,       // 71
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[54] = 0x0000005b,       // 91
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[55] = 0x00000044,       // 68
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[56] = 0x00000053,       // 83
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[57] = 0x00000043,       // 67
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[58] = 0x00000052,       // 82
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[59] = 0x00000043,       // 67
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[60] = 0x00000051,       // 81
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[61] = 0x00000037,       // 55
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[62] = 0x0000004a,       // 74
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[63] = 0x00000053,       // 83
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[64] = 0x00000037,       // 55
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[65] = 0x0000004c,       // 76
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[66] = 0x0000003e,       // 62
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[67] = 0x00000057,       // 87
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[68] = 0x0000003e,       // 62
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[69] = 0x0000004a,       // 74
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[70] = 0x00000040,       // 64
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[71] = 0x00000046,       // 70

		.cfg_ddr_training_delay_ps.soc_bit_vref = {
			0,  40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 48, 40, 48,
			40, 48, 40, 48
		},//total 44
		.cfg_ddr_training_delay_ps.dram_bit_vref = {
			0,  32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32
		},//total 36
		.cfg_ddr_training_delay_ps.reserve_training_parameter = {
			//cs0 write dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs1 write dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs0 read dqs,lane0-lane3
			(1 << 7) | 8, (0 << 7) | 0, (1 << 7) | 9, (0 << 7) | 0,
			//cs1 read dqs,lane0-lane3
			(1 << 7) | 13, (1 << 7) | 10, (1 << 7) | 6, (1 << 7) | 12,
		},
	},
#endif

#ifdef S4_DDR4_1RANK

#ifdef S4_DDR4_1RANK_PCB_MODE2_GANGZHENG
	{
		//timing_config,4layer 2pcs ddr4 rank0, gangzhen pcb.
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs =
			sizeof(__ddr_setting[0]) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,//0
		.cfg_board_common_setting.fast_boot = {0, 0, (1 << 3) | (4)},
		.cfg_board_common_setting.ddr_func = DDR_FUNC,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_DDR4,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
		.cfg_board_common_setting.DisabledDbyte = CONFIG_DISABLE_D32_D63,
		.cfg_board_common_setting.dram_cs0_base_add	= 0,
		.cfg_board_common_setting.dram_cs1_base_add = 0,
		.cfg_board_common_setting.dram_cs0_size_MB = CONFIG_DDR0_SIZE_2048MB,
		.cfg_board_common_setting.dram_cs1_size_MB = CONFIG_DDR1_SIZE_0MB,
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		.cfg_board_common_setting.log_level = LOG_LEVEL_BASIC,
		.cfg_board_common_setting.ddr_rdbi_wr_enable = DDR_WRITE_READ_DBI_DISABLE,
		//SSC_DISABLE,(1 << 20) | (0 << 8) | (2 << 4) | 0,
		//center_ssc_1000ppm,
		.cfg_board_common_setting.pll_ssc_mode = (1 << 20) | (1 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test_function = { 0 },
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR4_32BIT,
		//s4 ddr4 ac pinmux
		.cfg_board_common_setting.ac_pinmux = {
			0,	0,	0,	1,	2,	3,
			27,	10,	25,	5,	28,	11,	24,	0,
			26,	7,	4,	21,	2,	20,	8,	13,
			0,	0,	0,	0,	6,	12,	3,	0,
			0,	9,	1,	23,	0
		},
		.cfg_board_common_setting.ddr_dqs_swap = 0,
		.cfg_board_common_setting.ddr_dq_remap = {
			0,  1,	2,  3,	4,  5,	6,  7,
			8,  9,	10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, 29, 30, 31,
			32, 33, 34, 35
		},//d0-d31 dm0 dm1 dm2 dm3
		.cfg_board_common_setting.ddr_vddee_setting = { 0 },
		.cfg_board_SI_setting_ps.DRAMFreq = 1176,
		.cfg_board_SI_setting_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		.cfg_board_SI_setting_ps.ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		.cfg_board_SI_setting_ps.clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_60_OHM,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = DDR_DRAM_DDR_WR_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_pull_up_calibration_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_voltage_range_setting =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		.cfg_board_SI_setting_ps.dfe_offset = 0,
		.cfg_board_SI_setting_ps.vref_ac_permil = 0,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,
		.cfg_board_SI_setting_ps.training_phase_parameter = { 0 },
		.cfg_board_SI_setting_ps.ac_trace_delay_org = {
			128, 128, 128 - 40, 128, 128, 128, 128, 128,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384,
		},//total 36
		.cfg_ddr_training_delay_ps.ac_trace_delay = {
			128, 128, 128 - 40, 128, 128, 128, 128, 384,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384,
		},

		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x000000c8,// 200
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x000000c3,// 195
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x000000b4,// 180
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0x000000af,// 175
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x00000102,// 258
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x00000102,// 258
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x00000102,// 258
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x00000102,// 258
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[0] = 0x0000010b,// 267
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[1] = 0x000000f7,// 247
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[2] = 0x00000105,// 261
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[3] = 0x00000100,// 256
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[4] = 0x000000f5,// 245
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[5] = 0x000000e4,// 228
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[6] = 0x000000ff,// 255
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[7] = 0x000000fe,// 254
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[8] = 0x000000f8,// 248
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[9] = 0x0000010e,// 270
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[10] = 0x000000f3,// 243
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[11] = 0x00000106,// 262
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[12] = 0x000000fa,// 250
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[13] = 0x00000100,// 256
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[14] = 0x000000fa,// 250
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[15] = 0x00000107,// 263
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[16] = 0x000000fd,// 253
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[17] = 0x00000100,// 256
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[18] = 0x00000102,// 258
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[19] = 0x000000e9,// 233
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[20] = 0x000000ff,// 255
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[21] = 0x000000ef,// 239
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[22] = 0x00000100,// 256
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[23] = 0x000000f8,// 248
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[24] = 0x000000f2,// 242
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[25] = 0x000000fa,// 250
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[26] = 0x000000f3,// 243
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[27] = 0x00000111,// 273
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[28] = 0x000000ec,// 236
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[29] = 0x00000103,// 259
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[30] = 0x000000f9,// 249
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[31] = 0x0000010c,// 268
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[32] = 0x000000f3,// 243
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[33] = 0x000000f9,// 249
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[34] = 0x000000f6,// 246
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[35] = 0x000000ff,// 255
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[36] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[37] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[38] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[39] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[40] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[41] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[42] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[43] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[44] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[45] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[46] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[47] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[48] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[49] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[50] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[51] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[52] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[53] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[54] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[55] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[56] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[57] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[58] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[59] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[60] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[61] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[62] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[63] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[64] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[65] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[66] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[67] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[68] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[69] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[70] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[71] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0x0000032d,// 813
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0x0000032a,// 810
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0x00000337,// 823
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0x00000344,// 836
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x0000008c,// 140
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x00000078,// 120
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x0000007a,// 122
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x00000063,// 99
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[0] = 0x00000050,// 80
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[1] = 0x0000003c,// 60
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[2] = 0x0000004b,// 75
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[3] = 0x0000003f,// 63
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[4] = 0x0000004c,// 76
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[5] = 0x00000033,// 51
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[6] = 0x0000004e,// 78
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[7] = 0x0000003a,// 58
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[8] = 0x00000043,// 67
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[9] = 0x00000053,// 83
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[10] = 0x00000035,// 53
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[11] = 0x00000045,// 69
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[12] = 0x0000002e,// 46
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[13] = 0x00000045,// 69
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[14] = 0x0000003a,// 58
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[15] = 0x00000051,// 81
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[16] = 0x0000003c,// 60
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[17] = 0x00000040,// 64
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[18] = 0x00000050,// 80
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[19] = 0x0000002f,// 47
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[20] = 0x00000044,// 68
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[21] = 0x00000035,// 53
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[22] = 0x00000051,// 81
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[23] = 0x00000047,// 71
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[24] = 0x00000043,// 67
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[25] = 0x0000004d,// 77
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[26] = 0x00000044,// 68
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[27] = 0x00000054,// 84
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[28] = 0x00000030,// 48
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[29] = 0x00000043,// 67
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[30] = 0x00000030,// 48
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[31] = 0x00000051,// 81
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[32] = 0x00000032,// 50
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[33] = 0x00000041,// 65
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[34] = 0x00000030,// 48
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[35] = 0x0000003d,// 61
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[36] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[37] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[38] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[39] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[40] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[41] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[42] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[43] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[44] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[45] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[46] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[47] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[48] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[49] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[50] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[51] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[52] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[53] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[54] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[55] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[56] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[57] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[58] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[59] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[60] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[61] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[62] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[63] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[64] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[65] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[66] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[67] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[68] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[69] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[70] = 0x00000000,// 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[71] = 0x00000000,// 0

		.cfg_ddr_training_delay_ps.soc_bit_vref = {
			0,  40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 48, 40, 48,
			40, 48, 40, 48
		},//total 44
		.cfg_ddr_training_delay_ps.dram_bit_vref = {
			0,  32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32
		},//total 36
		.cfg_ddr_training_delay_ps.reserve_training_parameter = {
			//cs0 write dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs1 write dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs0 read dqs,lane0-lane3
			(1 << 7) | 0, (0 << 7) | 0, (1 << 7) | 0, (0 << 7) | 0,
			//cs1 read dqs,lane0-lane3
			(1 << 7) | 0, (1 << 7) | 0, (1 << 7) | 0, (1 << 7) | 0,
		},
	},

#else //AP229
	{
		//timing_config,4layer 2pcs ddr4 rank0, ap229.
		.cfg_board_common_setting.timming_magic = 0,
		.cfg_board_common_setting.timming_max_valid_configs =
			sizeof(__ddr_setting[0]) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,//0
		.cfg_board_common_setting.fast_boot = {0, 0, (1 << 3) | (4)},
		.cfg_board_common_setting.ddr_func = DDR_FUNC,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_DDR4,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
		.cfg_board_common_setting.DisabledDbyte = CONFIG_DISABLE_D32_D63,
		.cfg_board_common_setting.dram_cs0_base_add = 0,
		.cfg_board_common_setting.dram_cs1_base_add = 0,
		.cfg_board_common_setting.dram_cs0_size_MB = CONFIG_DDR0_SIZE_2048MB,
		.cfg_board_common_setting.dram_cs1_size_MB = CONFIG_DDR1_SIZE_0MB,
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		.cfg_board_common_setting.log_level = LOG_LEVEL_BASIC,
		.cfg_board_common_setting.ddr_rdbi_wr_enable = DDR_WRITE_READ_DBI_DISABLE,
		//SSC_DISABLE,(1 << 20) | (0 << 8) | (2 << 4) | 0,
		//center_ssc_1000ppm,
		.cfg_board_common_setting.pll_ssc_mode = (1 << 20) | (1 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test_function = { 0 },
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR4_32BIT,
		//s4 ddr4 ac pinmux
		.cfg_board_common_setting.ac_pinmux = {
			0,	0,	0,	1,	2,	3,
			27,	10,	25,	5,	28,	11,	24,	0,
			26,	7,	4,	21,	2,	20,	8,	13,
			0,	0,	0,	0,	6,	12,	3,	0,
			0,	9,	1,	23,	0
		},
		.cfg_board_common_setting.ddr_dqs_swap = 0,
		.cfg_board_common_setting.ddr_dq_remap = {
			0,  1,	2,  3,	4,  5,	6,  7,
			8,  9,	10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, 29, 30, 31,
			32, 33, 34, 35
		},//d0-d31 dm0 dm1 dm2 dm3
		.cfg_board_common_setting.ddr_vddee_setting = { 0 },
		.cfg_board_SI_setting_ps.DRAMFreq = 1320,
		.cfg_board_SI_setting_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		.cfg_board_SI_setting_ps.ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		.cfg_board_SI_setting_ps.clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_60_OHM,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = DDR_DRAM_DDR_WR_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_pull_up_calibration_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_voltage_range_setting =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		.cfg_board_SI_setting_ps.dfe_offset = 0,
		.cfg_board_SI_setting_ps.vref_ac_permil = 0,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,
		.cfg_board_SI_setting_ps.training_phase_parameter = { 0 },
		.cfg_board_SI_setting_ps.ac_trace_delay_org = {
			128, 128, 128 - 40, 128, 128, 128, 128, 128,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384,
		},//total 36
		.cfg_ddr_training_delay_ps.ac_trace_delay = {
			128, 128, 128 - 40, 128, 128, 128, 128, 384,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384, 384, 384, 384, 384,
			384, 384, 384,	    384,
		},

		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x00000131,          // 305
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x00000127,          // 295
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x00000136,          // 310
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0x00000127,          // 295
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x00000102,          // 258
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x00000102,          // 258
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x00000102,          // 258
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x00000102,          // 258
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[0] = 0x0000017a,       // 378
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[1] = 0x00000164,       // 356
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[2] = 0x0000017d,       // 381
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[3] = 0x00000174,       // 372
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[4] = 0x00000176,       // 374
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[5] = 0x00000160,       // 352
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[6] = 0x0000017c,       // 380
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[7] = 0x0000017a,       // 378
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[8] = 0x0000016e,       // 366
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[9] = 0x00000175,       // 373
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[10] = 0x00000143,      // 323
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[11] = 0x0000015c,      // 348
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[12] = 0x0000015e,      // 350
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[13] = 0x0000017b,      // 379
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[14] = 0x0000015c,      // 348
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[15] = 0x00000172,      // 370
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[16] = 0x0000015c,      // 348
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[17] = 0x0000015e,      // 350
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[18] = 0x00000178,      // 376
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[19] = 0x0000014f,      // 335
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[20] = 0x00000165,      // 357
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[21] = 0x00000160,      // 352
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[22] = 0x00000175,      // 373
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[23] = 0x00000168,      // 360
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[24] = 0x0000015e,      // 350
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[25] = 0x00000173,      // 371
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[26] = 0x00000164,      // 356
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[27] = 0x00000178,      // 376
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[28] = 0x0000015c,      // 348
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[29] = 0x0000015f,      // 351
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[30] = 0x00000153,      // 339
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[31] = 0x00000170,      // 368
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[32] = 0x0000015a,      // 346
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[33] = 0x0000015d,      // 349
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[34] = 0x00000153,      // 339
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[35] = 0x00000164,      // 356
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[36] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[37] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[38] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[39] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[40] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[41] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[42] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[43] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[44] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[45] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[46] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[47] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[48] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[49] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[50] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[51] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[52] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[53] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[54] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[55] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[56] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[57] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[58] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[59] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[60] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[61] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[62] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[63] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[64] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[65] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[66] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[67] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[68] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[69] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[70] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[71] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0x00000421,      // 1057
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0x00000420,      // 1056
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0x0000041a,      // 1050
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0x0000041a,      // 1050
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x00000085,           // 133
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x00000093,           // 147
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x00000099,           // 153
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x0000008a,           // 138
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[0] = 0x00000054,        // 84
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[1] = 0x0000003c,        // 60
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[2] = 0x0000004d,        // 77
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[3] = 0x00000048,        // 72
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[4] = 0x0000004f,        // 79
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[5] = 0x00000030,        // 48
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[6] = 0x00000057,        // 87
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[7] = 0x00000046,        // 70
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[8] = 0x00000048,        // 72
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[9] = 0x00000066,        // 102
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[10] = 0x00000039,       // 57
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[11] = 0x00000052,       // 82
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[12] = 0x00000042,       // 66
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[13] = 0x00000067,       // 103
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[14] = 0x0000004d,       // 77
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[15] = 0x0000005f,       // 95
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[16] = 0x00000046,       // 70
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[17] = 0x00000051,       // 81
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[18] = 0x00000057,       // 87
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[19] = 0x0000003c,       // 60
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[20] = 0x00000052,       // 82
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[21] = 0x00000045,       // 69
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[22] = 0x0000006c,       // 108
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[23] = 0x00000060,       // 96
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[24] = 0x0000004f,       // 79
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[25] = 0x0000006a,       // 106
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[26] = 0x00000055,       // 85
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[27] = 0x00000060,       // 96
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[28] = 0x0000003c,       // 60
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[29] = 0x00000041,       // 65
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[30] = 0x00000035,       // 53
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[31] = 0x00000069,       // 105
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[32] = 0x00000033,       // 51
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[33] = 0x0000003f,       // 63
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[34] = 0x00000033,       // 51
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[35] = 0x00000044,       // 68
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[36] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[37] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[38] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[39] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[40] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[41] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[42] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[43] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[44] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[45] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[46] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[47] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[48] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[49] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[50] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[51] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[52] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[53] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[54] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[55] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[56] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[57] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[58] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[59] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[60] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[61] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[62] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[63] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[64] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[65] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[66] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[67] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[68] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[69] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[70] = 0x00000000,       // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[71] = 0x00000000,       // 0

		//total 72
		.cfg_ddr_training_delay_ps.soc_bit_vref = {
			0,  40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 40, 40, 40,
			40, 40, 40, 40, 40, 48, 40, 48,
			40, 48, 40, 48
		},//total 44
		.cfg_ddr_training_delay_ps.dram_bit_vref = {
			0,  32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32, 32, 32, 32, 32,
			32, 32, 32, 32
		},//total 36
		.cfg_ddr_training_delay_ps.reserve_training_parameter = {
			//cs0 write dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs1 write dqs,lane0-lane3
			(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
			//cs0 read dqs,lane0-lane3
			(1 << 7) | 0, (0 << 7) | 0, (1 << 7) | 0, (0 << 7) | 0,
			//cs1 read dqs,lane0-lane3
			(1 << 7) | 0, (1 << 7) | 0, (1 << 7) | 0, (1 << 7) | 0,
		},
	},
#endif //end S4_DDR4_1RANK_PCB_MODE2_GANGZHENG
#endif //end S4_DDR4_1RANK
};

board_clk_set_t __board_clk_setting
__attribute__ ((section(".clk_param"))) = {
	/* clock settings for bl2 */
	.cpu_clk		= CPU_CLK / 24 * 24,
#ifdef CONFIG_PXP_DDR
	.pxp			= 1,
#else
	.pxp			= 0,
#endif
	.low_console_baud	= CONFIG_LOW_CONSOLE_BAUD,
};


#define VCCK_VAL                                AML_VCCK_INIT_VOLTAGE
#define VDDEE_VAL                               AML_VDDEE_INIT_VOLTAGE
/* VCCK PWM table */
#if   (VCCK_VAL == 1039)
#define VCCK_VAL_REG    0x00000022
#elif (VCCK_VAL == 1029)
#define VCCK_VAL_REG    0x00010021
#elif (VCCK_VAL == 1019)
#define VCCK_VAL_REG    0x00020020
#elif (VCCK_VAL == 1009)
#define VCCK_VAL_REG    0x0003001f
#elif (VCCK_VAL == 999)
#define VCCK_VAL_REG    0x0004001e
#elif (VCCK_VAL == 989)
#define VCCK_VAL_REG    0x0005001d
#elif (VCCK_VAL == 979)
#define VCCK_VAL_REG    0x0006001c
#elif (VCCK_VAL == 969)
#define VCCK_VAL_REG    0x0007001b
#elif (VCCK_VAL == 959)
#define VCCK_VAL_REG    0x0008001a
#elif (VCCK_VAL == 949)
#define VCCK_VAL_REG    0x00090019
#elif (VCCK_VAL == 939)
#define VCCK_VAL_REG    0x000a0018
#elif (VCCK_VAL == 929)
#define VCCK_VAL_REG    0x000b0017
#elif (VCCK_VAL == 919)
#define VCCK_VAL_REG    0x000c0016
#elif (VCCK_VAL == 909)
#define VCCK_VAL_REG    0x000d0015
#elif (VCCK_VAL == 899)
#define VCCK_VAL_REG    0x000e0014
#elif (VCCK_VAL == 889)
#define VCCK_VAL_REG    0x000f0013
#elif (VCCK_VAL == 879)
#define VCCK_VAL_REG    0x00100012
#elif (VCCK_VAL == 869)
#define VCCK_VAL_REG    0x00110011
#elif (VCCK_VAL == 859)
#define VCCK_VAL_REG    0x00120010
#elif (VCCK_VAL == 849)
#define VCCK_VAL_REG    0x0013000f
#elif (VCCK_VAL == 839)
#define VCCK_VAL_REG    0x0014000e
#elif (VCCK_VAL == 829)
#define VCCK_VAL_REG    0x0015000d
#elif (VCCK_VAL == 819)
#define VCCK_VAL_REG    0x0016000c
#elif (VCCK_VAL == 809)
#define VCCK_VAL_REG    0x0017000b
#elif (VCCK_VAL == 799)
#define VCCK_VAL_REG    0x0018000a
#elif (VCCK_VAL == 789)
#define VCCK_VAL_REG    0x00190009
#elif (VCCK_VAL == 779)
#define VCCK_VAL_REG    0x001a0008
#elif (VCCK_VAL == 769)
#define VCCK_VAL_REG    0x001b0007
#elif (VCCK_VAL == 759)
#define VCCK_VAL_REG    0x001c0006
#elif (VCCK_VAL == 749)
#define VCCK_VAL_REG    0x001d0005
#elif (VCCK_VAL == 739)
#define VCCK_VAL_REG    0x001e0004
#elif (VCCK_VAL == 729)
#define VCCK_VAL_REG    0x001f0003
#elif (VCCK_VAL == 719)
#define VCCK_VAL_REG    0x00200002
#elif (VCCK_VAL == 709)
#define VCCK_VAL_REG    0x00210001
#elif (VCCK_VAL == 699)
#define VCCK_VAL_REG    0x00220000
#else
#error "VCCK val out of range\n"
#endif

/* VDDEE_VAL_REG */
#if    (VDDEE_VAL == 700)
#define VDDEE_VAL_REG   0x120000
#elif (VDDEE_VAL == 710)
#define VDDEE_VAL_REG   0x110001
#elif (VDDEE_VAL == 720)
#define VDDEE_VAL_REG   0x100002
#elif (VDDEE_VAL == 730)
#define VDDEE_VAL_REG   0xf0003
#elif (VDDEE_VAL == 740)
#define VDDEE_VAL_REG   0xe0004
#elif (VDDEE_VAL == 750)
#define VDDEE_VAL_REG   0xd0005
#elif (VDDEE_VAL == 760)
#define VDDEE_VAL_REG   0xc0006
#elif (VDDEE_VAL == 770)
#define VDDEE_VAL_REG   0xb0007
#elif (VDDEE_VAL == 780)
#define VDDEE_VAL_REG   0xa0008
#elif (VDDEE_VAL == 790)
#define VDDEE_VAL_REG   0x90009
#elif (VDDEE_VAL == 800)
#define VDDEE_VAL_REG   0x8000a
#elif (VDDEE_VAL == 810)
#define VDDEE_VAL_REG   0x7000b
#elif (VDDEE_VAL == 820)
#define VDDEE_VAL_REG   0x6000c
#elif (VDDEE_VAL == 830)
#define VDDEE_VAL_REG   0x5000d
#elif (VDDEE_VAL == 840)
#define VDDEE_VAL_REG   0x4000e
#elif (VDDEE_VAL == 850)
#define VDDEE_VAL_REG   0x3000f
#elif (VDDEE_VAL == 860)
#define VDDEE_VAL_REG   0x20010
#elif (VDDEE_VAL == 870)
#define VDDEE_VAL_REG   0x10011
#elif (VDDEE_VAL == 880)
#define VDDEE_VAL_REG   0x12
#else
#error "VDDEE val out of range\n"
#endif

bl2_reg_t __bl2_reg[] __attribute__ ((section(".generic_param"))) = {
	//need fine tune
	{ 0, 0, 0xffffffff, 0, 0, 0 },
};

/* gpio/pinmux/pwm init */
register_ops_t __bl2_ops_reg[MAX_REG_OPS_ENTRIES]
__attribute__ ((section(".misc_param"))) = {
	/* config vddee and vcck pwm - pwm_h and pwm_j*/
#ifdef CONFIG_PDVFS_ENABLE
	/* s4 set vddee */
	{PWMGH_PWM_B, 0x8000a, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_1, 0},
	{PWMGH_PWM_B, 0x5000d, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_2, 0},
	{PWMGH_PWM_B, 0x20010, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_3, 0},
	/* s4d set vddee */
	{PWMGH_PWM_B, 0x7000b, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_4, 0},
	{PWMGH_PWM_B, 0x5000d, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_5, 0},
	{PWMGH_PWM_B, 0x3000f, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_6, 0},
#else
	{PWMGH_PWM_B, VDDEE_VAL_REG, 0xffffffff, 0, 0, 0},
#endif
	{PWMIJ_PWM_B, VCCK_VAL_REG, 0xffffffff, 0, 0, 0},
	{PWMGH_MISC_REG_AB, (0x1 << 1), (0x1 << 1), 0, 0, 0},
	{PWMIJ_MISC_REG_AB, (0x1 << 1), (0x1 << 1), 0, 0, 0},
	/* set pwm h and pwm j clock rate to 24M, enable them */
	{ CLKCTRL_PWM_CLK_GH_CTRL, (1 << 24), 0xffffffff, 0, 0, 0 },
	{ CLKCTRL_PWM_CLK_IJ_CTRL, (1 << 24), 0xffffffff, 0, 0, 0 },
	/* set GPIOE_0 GPIOE_1 drive strength to 3 */
	{ PADCTRL_GPIOE_DS, 0xf,	0xf, 0, 0, 0 },
	/* set GPIOE_0 GPIOE_1 mux to pwmh pwmj */
	{ PADCTRL_PIN_MUX_REGI,	   (0x3 << 0),	  (0xf << 0), 0, 0, 0 },
	{ PADCTRL_PIN_MUX_REGI,	   (0x3 << 4),	  (0xf << 4), 0, 0, 0 },
};

#define DEV_FIP_SIZE 0x300000
#define DDR_FIP_SIZE 0x40000
/* for all the storage parameter */
storage_parameter_t __store_para __attribute__ ((section(".store_param"))) = {
	.common					={
		.version			= 0x01,
		.device_fip_container_size	= DEV_FIP_SIZE,
		.device_fip_container_copies	= 4,
		.ddr_fip_container_size		= DDR_FIP_SIZE,
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
						  (8 << 0),
		.reserved_area_blk_cnt		= 48,
		.page_per_block			= 64,
		.use_param_page_list		= 0,
	},
};
