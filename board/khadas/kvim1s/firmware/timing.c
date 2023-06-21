// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <asm/arch/secure_apb.h>
#include <asm/arch/timing.h>
#include <asm/arch/ddr_define.h>
//[SEI-zhuzw-2021/07/06] support dongle bootloader compatible build {
#include <version.h>
//[SEI-zhuzw-2021/07/06] support dongle bootloader compatible build }
/* board clk defines */
#define CPU_CLK                                 1512
#define DDR_ID_ACS_ADC   ((3 << 6) | (8)) //bit 6 adc_channel bit 0-5 adc value,chan 3 value 8 is layer 2


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
	DDR_TIMMING_TUNE_START(DDR_ID_FROM_ADC,							      DDR_ADC_CH1, DDR_ADC_VALUE1),
	//data start
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_common_setting.Is2Ttiming,				      CONFIG_USE_DDR_2T_MODE),
};
////_ddr_para_2nd_setting
//*/
uint32_t __ddr_parameter_reg_index[] __attribute__ ((section(".ddr_2acs_index"))) = {
	DDR_ID_START_MASK,
	DDR_TIMMING_TUNE_STRUCT_SIZE(__bl2_ddr_reg_data),
	//0,
};
#endif



#define S4_LPDDR4_1RANK 1   //use for 1rank lpddr4
//#define S4_LPDDR4_2RANK 1 //use for 2rank lpddr4
#define S4_LPDDR4_LAYER_4 1   
//#define S4_LPDDR4_LAYER_4 1

ddr_set_ps0_only_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {

#if S4_LPDDR4_2RANK
{
.cfg_board_common_setting.timming_magic = 0,
.cfg_board_common_setting.timming_max_valid_configs = sizeof(__ddr_setting[0]) / sizeof(ddr_set_ps0_only_t),
.cfg_board_common_setting.timming_struct_version = 0,
.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
.cfg_board_common_setting.timming_struct_real_size = 0,
//.cfg_board_common_setting.fast_boot = { 1, 0, 0, 0xc2, },
.cfg_board_common_setting.fast_boot = { 0 },
.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_LPDDR4,
.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK01_CH01,
.cfg_board_common_setting.DisabledDbyte = CONFIG_DISABLE_D32_D63,
.cfg_board_common_setting.dram_cs0_base_add = 0,
.cfg_board_common_setting.dram_cs1_base_add = 0,
.cfg_board_common_setting.dram_cs0_size_MB = CONFIG_DDR0_SIZE_AUTO_SIZE,
.cfg_board_common_setting.dram_cs1_size_MB = CONFIG_DDR0_SIZE_AUTO_SIZE,
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
#ifdef S4_LPDDR4_LAYER_4
.cfg_board_common_setting.ddr_dq_remap = {
	15, 8, 12, 13, 11, 14, 10, 9,
	7, 3, 0, 1, 4, 5, 2, 6,
	16, 19, 23, 21, 18, 22, 20, 17,
	31, 27, 30, 26, 25, 29, 24, 28,
	33, 32, 34, 35
	},
#endif

#ifdef S4_LPDDR4_LAYER_6
.cfg_board_common_setting.ddr_dq_remap = {
	8,  12, 13, 11, 14, 9,	10, 15,
	0,  2,	5,  6,	3,  7,	1,  4,
	27, 31, 28, 24, 25, 29, 26, 30,
	21, 18, 22, 19, 16, 17, 23, 20,
	33, 32, 35, 34
	},
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
.cfg_board_SI_setting_ps.dram_data_drv_pull_up_calibration_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
.cfg_board_SI_setting_ps.lpddr4_dram_vout_voltage_range_setting = DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
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
	256, 256,	256,
	256, 256, 256, 256, 256,
	256, 256, 256,
	256, 256, 256, 256, 256,
	256, 256, 256, 256,
	},
.cfg_ddr_training_delay_ps.ac_trace_delay = {
	 192, 192, 128, 128, 192,
	 192, 128, 256,
	 226+40, 221+40, 272+40,
	 240+40, 256+40, 256+40, 256, 256,
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
	.cfg_ddr_training_delay_ps.dram_bit_vref[0] = 0x4d,

#ifdef S4_LPDDR4_LAYER_4
		.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		(0 << 7) | 4,  (0 << 7) | 7,  (0 << 7) | 5,  (0 << 7) | 10,//cs0 write dqs,lane0-lane3
		(0 << 7) | 5,  (0 << 7) | 8,  (0 << 7) | 5,  (0 << 7) | 10,//cs1 write dqs,lane0-lane3
		(1 << 7) | 10, (1 << 7) | 15, (1 << 7) | 10, (1 << 7) | 13,//cs0 read dqs,lane0-lane3
		(1 << 7) | 12, (1 << 7) | 16, (1 << 7) | 10, (1 << 7) | 13,//cs1 read dqs,lane0-lane3
		},
#endif

#ifdef S4_LPDDR4_LAYER_6
		.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		(0 << 7) | 5,  (0 << 7) | 14, (1 << 7) | 5,  (0 << 7) | 12,//cs0 write dqs,lane0-lane3
		(0 << 7) | 7,  (0 << 7) | 13, (1 << 7) | 5,  (0 << 7) | 15,//cs1 write dqs,lane0-lane3
		(1 << 7) | 15, (1 << 7) | 10, (1 << 7) | 10, (1 << 7) | 12,//cs0 read dqs,lane0-lane3
		(1 << 7) | 15, (1 << 7) | 15, (1 << 7) | 15, (1 << 7) | 10,//cs1 read dqs,lane0-lane3
		},
#endif
},
#endif  //end T212_DONGLE 4layer LPDDR4 rank01

#if S4_LPDDR4_1RANK
{
.cfg_board_common_setting.timming_magic = 0,
.cfg_board_common_setting.timming_max_valid_configs = sizeof(__ddr_setting[0]) / sizeof(ddr_set_ps0_only_t),
.cfg_board_common_setting.timming_struct_version = 0,
.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
.cfg_board_common_setting.timming_struct_real_size = 0,
.cfg_board_common_setting.fast_boot = { 1, 0, 0, 0xc6, },
//.cfg_board_common_setting.fast_boot = { 0 },
.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_LPDDR4,
.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
.cfg_board_common_setting.DisabledDbyte = CONFIG_DISABLE_D32_D63,
.cfg_board_common_setting.dram_cs0_base_add = 0,
.cfg_board_common_setting.dram_cs1_base_add = 0,
.cfg_board_common_setting.dram_cs0_size_MB = CONFIG_DDR0_SIZE_AUTO_SIZE,
.cfg_board_common_setting.dram_cs1_size_MB = 0,
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
#ifdef S4_LPDDR4_LAYER_4
.cfg_board_common_setting.ddr_dq_remap = {
	15, 8, 12, 13, 11, 14, 10, 9,
	7, 3, 0, 1, 4, 5, 2, 6,
	16, 19, 23, 21, 18, 22, 20, 17,
	31, 27, 30, 26, 25, 29, 24, 28,
	33, 32, 34, 35
	},
#endif

#ifdef S4_LPDDR4_LAYER_6
.cfg_board_common_setting.ddr_dq_remap = {
	8,  12, 13, 11, 14, 9,	10, 15,
	0,  2,	5,  6,	3,  7,	1,  4,
	27, 31, 28, 24, 25, 29, 26, 30,
	21, 18, 22, 19, 16, 17, 23, 20,
	33, 32, 35, 34
	},
#endif

.cfg_board_common_setting.ddr_vddee_setting[0] = 0,
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
.cfg_board_SI_setting_ps.dram_data_drv_pull_up_calibration_ohm = DDR_DRAM_LPDDR4_ODT_40_OHM,
.cfg_board_SI_setting_ps.lpddr4_dram_vout_voltage_range_setting = DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
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
	256, 256,	256,
	256, 256, 256, 256, 256,
	256, 256, 256,
	256, 256, 256, 256, 256,
	256, 256, 256, 256,
	},
.cfg_ddr_training_delay_ps.ac_trace_delay = {
	 192, 192, 128, 128, 192,
	 192, 128, 256,
	 256, 256, 256,
	 256, 256, 256, 256, 256,
	 256, 256, 256,
	 256, 256, 256, 256, 256,
	 256, 256, 256,
	 256, 256, 256, 256, 256,
	 256, 256, 256, 256,
	 },

#if 1
.cfg_ddr_training_delay_ps.write_dqs_delay[0]=0x000000d2,// 210
.cfg_ddr_training_delay_ps.write_dqs_delay[1]=0x000000b1,// 177
.cfg_ddr_training_delay_ps.write_dqs_delay[2]=0x000000d5,// 213
.cfg_ddr_training_delay_ps.write_dqs_delay[3]=0x000000bb,// 187
.cfg_ddr_training_delay_ps.write_dqs_delay[4]=0x00000090,// 144
.cfg_ddr_training_delay_ps.write_dqs_delay[5]=0x000000a4,// 164
.cfg_ddr_training_delay_ps.write_dqs_delay[6]=0x000000a5,// 165
.cfg_ddr_training_delay_ps.write_dqs_delay[7]=0x00000095,// 149
.cfg_ddr_training_delay_ps.write_dq_bit_delay[0]=0x000001ab,// 427
.cfg_ddr_training_delay_ps.write_dq_bit_delay[1]=0x000001c1,// 449
.cfg_ddr_training_delay_ps.write_dq_bit_delay[2]=0x000001ba,// 442
.cfg_ddr_training_delay_ps.write_dq_bit_delay[3]=0x000001ab,// 427
.cfg_ddr_training_delay_ps.write_dq_bit_delay[4]=0x0000019a,// 410
.cfg_ddr_training_delay_ps.write_dq_bit_delay[5]=0x000001ae,// 430
.cfg_ddr_training_delay_ps.write_dq_bit_delay[6]=0x000001a3,// 419
.cfg_ddr_training_delay_ps.write_dq_bit_delay[7]=0x00000197,// 407
.cfg_ddr_training_delay_ps.write_dq_bit_delay[8]=0x000001aa,// 426
.cfg_ddr_training_delay_ps.write_dq_bit_delay[9]=0x00000187,// 391
.cfg_ddr_training_delay_ps.write_dq_bit_delay[10]=0x0000018a,// 394
.cfg_ddr_training_delay_ps.write_dq_bit_delay[11]=0x0000018d,// 397
.cfg_ddr_training_delay_ps.write_dq_bit_delay[12]=0x0000017d,// 381
.cfg_ddr_training_delay_ps.write_dq_bit_delay[13]=0x00000187,// 391
.cfg_ddr_training_delay_ps.write_dq_bit_delay[14]=0x00000183,// 387
.cfg_ddr_training_delay_ps.write_dq_bit_delay[15]=0x00000196,// 406
.cfg_ddr_training_delay_ps.write_dq_bit_delay[16]=0x00000195,// 405
.cfg_ddr_training_delay_ps.write_dq_bit_delay[17]=0x0000018a,// 394
.cfg_ddr_training_delay_ps.write_dq_bit_delay[18]=0x000001b0,// 432
.cfg_ddr_training_delay_ps.write_dq_bit_delay[19]=0x0000019e,// 414
.cfg_ddr_training_delay_ps.write_dq_bit_delay[20]=0x0000019c,// 412
.cfg_ddr_training_delay_ps.write_dq_bit_delay[21]=0x0000019c,// 412
.cfg_ddr_training_delay_ps.write_dq_bit_delay[22]=0x0000018f,// 399
.cfg_ddr_training_delay_ps.write_dq_bit_delay[23]=0x00000193,// 403
.cfg_ddr_training_delay_ps.write_dq_bit_delay[24]=0x0000019c,// 412
.cfg_ddr_training_delay_ps.write_dq_bit_delay[25]=0x00000192,// 402
.cfg_ddr_training_delay_ps.write_dq_bit_delay[26]=0x0000019a,// 410
.cfg_ddr_training_delay_ps.write_dq_bit_delay[27]=0x00000195,// 405
.cfg_ddr_training_delay_ps.write_dq_bit_delay[28]=0x00000193,// 403
.cfg_ddr_training_delay_ps.write_dq_bit_delay[29]=0x00000193,// 403
.cfg_ddr_training_delay_ps.write_dq_bit_delay[30]=0x00000199,// 409
.cfg_ddr_training_delay_ps.write_dq_bit_delay[31]=0x00000193,// 403
.cfg_ddr_training_delay_ps.write_dq_bit_delay[32]=0x0000018e,// 398
.cfg_ddr_training_delay_ps.write_dq_bit_delay[33]=0x000001ab,// 427
.cfg_ddr_training_delay_ps.write_dq_bit_delay[34]=0x000001ad,// 429
.cfg_ddr_training_delay_ps.write_dq_bit_delay[35]=0x00000199,// 409
.cfg_ddr_training_delay_ps.write_dq_bit_delay[36]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[37]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[38]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[39]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[40]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[41]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[42]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[43]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[44]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[45]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[46]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[47]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[48]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[49]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[50]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[51]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[52]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[53]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[54]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[55]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[56]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[57]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[58]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[59]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[60]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[61]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[62]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[63]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[64]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[65]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[66]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[67]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[68]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[69]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[70]=0x00000000,// 0
.cfg_ddr_training_delay_ps.write_dq_bit_delay[71]=0x00000000,// 0
.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0]=0x000000e2,// 226
.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1]=0x000000f0,// 240
.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2]=232,// 232
.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3]=232,// 245
.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4]=0x00000000,// 0
.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5]=0x00000000,// 0
.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6]=0x00000000,// 0
.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7]=0x00000000,// 0
.cfg_ddr_training_delay_ps.read_dqs_delay[0]=0x0000007f,// 127
.cfg_ddr_training_delay_ps.read_dqs_delay[1]=0x00000075,// 117
.cfg_ddr_training_delay_ps.read_dqs_delay[2]=0x00000081,// 129
.cfg_ddr_training_delay_ps.read_dqs_delay[3]=0x00000072,// 114
.cfg_ddr_training_delay_ps.read_dqs_delay[4]=0x000000f0,// 240
.cfg_ddr_training_delay_ps.read_dqs_delay[5]=0x000000f0,// 240
.cfg_ddr_training_delay_ps.read_dqs_delay[6]=0x000000f3,// 243
.cfg_ddr_training_delay_ps.read_dqs_delay[7]=0x000000f6,// 246
.cfg_ddr_training_delay_ps.read_dq_bit_delay[0]=0x00000048,// 72
.cfg_ddr_training_delay_ps.read_dq_bit_delay[1]=0x0000005f,// 95
.cfg_ddr_training_delay_ps.read_dq_bit_delay[2]=0x00000059,// 89
.cfg_ddr_training_delay_ps.read_dq_bit_delay[3]=0x0000004b,// 75
.cfg_ddr_training_delay_ps.read_dq_bit_delay[4]=0x00000039,// 57
.cfg_ddr_training_delay_ps.read_dq_bit_delay[5]=0x00000051,// 81
.cfg_ddr_training_delay_ps.read_dq_bit_delay[6]=0x00000047,// 71
.cfg_ddr_training_delay_ps.read_dq_bit_delay[7]=0x00000036,// 54
.cfg_ddr_training_delay_ps.read_dq_bit_delay[8]=0x0000004a,// 74
.cfg_ddr_training_delay_ps.read_dq_bit_delay[9]=0x0000003f,// 63
.cfg_ddr_training_delay_ps.read_dq_bit_delay[10]=0x0000003f,// 63
.cfg_ddr_training_delay_ps.read_dq_bit_delay[11]=0x00000046,// 70
.cfg_ddr_training_delay_ps.read_dq_bit_delay[12]=0x00000033,// 51
.cfg_ddr_training_delay_ps.read_dq_bit_delay[13]=0x00000049,// 73
.cfg_ddr_training_delay_ps.read_dq_bit_delay[14]=0x00000038,// 56
.cfg_ddr_training_delay_ps.read_dq_bit_delay[15]=0x0000004c,// 76
.cfg_ddr_training_delay_ps.read_dq_bit_delay[16]=0x0000004d,// 77
.cfg_ddr_training_delay_ps.read_dq_bit_delay[17]=0x00000042,// 66
.cfg_ddr_training_delay_ps.read_dq_bit_delay[18]=0x0000004e,// 78
.cfg_ddr_training_delay_ps.read_dq_bit_delay[19]=0x00000034,// 52
.cfg_ddr_training_delay_ps.read_dq_bit_delay[20]=0x0000003d,// 61
.cfg_ddr_training_delay_ps.read_dq_bit_delay[21]=0x0000003f,// 63
.cfg_ddr_training_delay_ps.read_dq_bit_delay[22]=0x00000038,// 56
.cfg_ddr_training_delay_ps.read_dq_bit_delay[23]=0x0000003d,// 61
.cfg_ddr_training_delay_ps.read_dq_bit_delay[24]=0x0000004a,// 74
.cfg_ddr_training_delay_ps.read_dq_bit_delay[25]=0x00000034,// 52
.cfg_ddr_training_delay_ps.read_dq_bit_delay[26]=0x0000003e,// 62
.cfg_ddr_training_delay_ps.read_dq_bit_delay[27]=0x00000036,// 54
.cfg_ddr_training_delay_ps.read_dq_bit_delay[28]=0x00000035,// 53
.cfg_ddr_training_delay_ps.read_dq_bit_delay[29]=0x00000040,// 64
.cfg_ddr_training_delay_ps.read_dq_bit_delay[30]=0x0000003d,// 61
.cfg_ddr_training_delay_ps.read_dq_bit_delay[31]=0x00000032,// 50
.cfg_ddr_training_delay_ps.read_dq_bit_delay[32]=0x00000033,// 51
.cfg_ddr_training_delay_ps.read_dq_bit_delay[33]=0x00000057,// 87
.cfg_ddr_training_delay_ps.read_dq_bit_delay[34]=0x0000005a,// 90
.cfg_ddr_training_delay_ps.read_dq_bit_delay[35]=0x0000003f,// 63
.cfg_ddr_training_delay_ps.soc_bit_vref[0]=0,// 20
.cfg_ddr_training_delay_ps.soc_bit_vref[1]=0x00000015,// 21
.cfg_ddr_training_delay_ps.soc_bit_vref[2]=0x00000014,// 20
.cfg_ddr_training_delay_ps.soc_bit_vref[3]=0x00000015,// 21
.cfg_ddr_training_delay_ps.soc_bit_vref[4]=0x00000017,// 23
.cfg_ddr_training_delay_ps.soc_bit_vref[5]=0x00000018,// 24
.cfg_ddr_training_delay_ps.soc_bit_vref[6]=0x00000016,// 22
.cfg_ddr_training_delay_ps.soc_bit_vref[7]=0x00000016,// 22
.cfg_ddr_training_delay_ps.soc_bit_vref[8]=0x00000012,// 18
.cfg_ddr_training_delay_ps.soc_bit_vref[9]=0x00000014,// 20
.cfg_ddr_training_delay_ps.soc_bit_vref[10]=0x00000016,// 22
.cfg_ddr_training_delay_ps.soc_bit_vref[11]=0x00000016,// 22
.cfg_ddr_training_delay_ps.soc_bit_vref[12]=0x00000016,// 22
.cfg_ddr_training_delay_ps.soc_bit_vref[13]=0x00000016,// 22
.cfg_ddr_training_delay_ps.soc_bit_vref[14]=0x00000016,// 22
.cfg_ddr_training_delay_ps.soc_bit_vref[15]=0x00000016,// 22
.cfg_ddr_training_delay_ps.soc_bit_vref[16]=0x00000016,// 22
.cfg_ddr_training_delay_ps.soc_bit_vref[17]=0x00000012,// 18
.cfg_ddr_training_delay_ps.soc_bit_vref[18]=0x00000014,// 20
.cfg_ddr_training_delay_ps.soc_bit_vref[19]=0x00000015,// 21
.cfg_ddr_training_delay_ps.soc_bit_vref[20]=0x00000014,// 20
.cfg_ddr_training_delay_ps.soc_bit_vref[21]=0x00000016,// 22
.cfg_ddr_training_delay_ps.soc_bit_vref[22]=0x00000017,// 23
.cfg_ddr_training_delay_ps.soc_bit_vref[23]=0x00000016,// 22
.cfg_ddr_training_delay_ps.soc_bit_vref[24]=0x00000016,// 22
.cfg_ddr_training_delay_ps.soc_bit_vref[25]=0x00000015,// 21
.cfg_ddr_training_delay_ps.soc_bit_vref[26]=0x00000012,// 18
.cfg_ddr_training_delay_ps.soc_bit_vref[27]=0x00000016,// 22
.cfg_ddr_training_delay_ps.soc_bit_vref[28]=0x00000015,// 21
.cfg_ddr_training_delay_ps.soc_bit_vref[29]=0x00000015,// 21
.cfg_ddr_training_delay_ps.soc_bit_vref[30]=0x00000017,// 23
.cfg_ddr_training_delay_ps.soc_bit_vref[31]=0x00000014,// 20
.cfg_ddr_training_delay_ps.soc_bit_vref[32]=0x00000015,// 21
.cfg_ddr_training_delay_ps.soc_bit_vref[33]=0x00000015,// 21
.cfg_ddr_training_delay_ps.soc_bit_vref[34]=0x00000016,// 22
.cfg_ddr_training_delay_ps.soc_bit_vref[35]=0x00000012,// 18
.cfg_ddr_training_delay_ps.soc_bit_vref[36]=0x00000012,// 18
.cfg_ddr_training_delay_ps.soc_bit_vref[37]=0x00000015,// 21
.cfg_ddr_training_delay_ps.soc_bit_vref[38]=0x00000012,// 18
.cfg_ddr_training_delay_ps.soc_bit_vref[39]=0x00000015,// 21
.cfg_ddr_training_delay_ps.soc_bit_vref[40]=0x00000012,// 18
.cfg_ddr_training_delay_ps.soc_bit_vref[41]=0x00000015,// 21
.cfg_ddr_training_delay_ps.soc_bit_vref[42]=0x00000012,// 18
.cfg_ddr_training_delay_ps.soc_bit_vref[43]=0x00000017,// 23
.cfg_ddr_training_delay_ps.dram_bit_vref[0]=0x00000047,// 0
#endif


#ifdef S4_LPDDR4_LAYER_4
		.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		(0 << 7) | 2,  (0 << 7) | 4,  (0 << 7) | 1,  (0 << 7) | 4,//cs0 write dqs,lane0-lane3
		(0 << 7) | 4,  (0 << 7) | 2,  (0 << 7) | 7,  (0 << 7) | 6,//cs1 write dqs,lane0-lane3
		(1 << 7) | 0, (1 << 7) | 0, (1 << 7) | 0, (1 << 7) | 0,//cs0 read dqs,lane0-lane3
		(1 << 7) | 10, (1 << 7) | 16, (1 << 7) | 16, (1 << 7) | 10,//cs1 read dqs,lane0-lane3
		},
#endif
#ifdef S4_LPDDR4_LAYER_6
		.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		(0 << 7) | 7,  (0 << 7) | 15, (0 << 7) | 0, (0 << 7) | 7,//cs0 write dqs,lane0-lane3
		(0 << 7) | 5,  (0 << 7) | 15, (0 << 7) | 0, (0 << 7) | 8,//cs1 write dqs,lane0-lane3
		(1 << 7) | 13, (1 << 7) | 11, (1 << 7) | 13, (1 << 7) | 10,//cs0 read dqs,lane0-lane3
		(1 << 7) | 12, (1 << 7) | 12, (1 << 7) | 15,  (1 << 7) | 15,//cs1 read dqs,lane0-lane3
		},
#endif

	},
#endif  //end T212_DONGLE 4layer LPDDR4 rank01
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
	//hxbao, need fine tune
	{ 0, 0, 0xffffffff, 0, 0, 0 },
};

/* gpio/pinmux/pwm init */
register_ops_t __bl2_ops_reg[MAX_REG_OPS_ENTRIES]
__attribute__ ((section(".misc_param"))) = {
	/* config vddee and vcck pwm - pwm_h and pwm_j*/
//	{ PWMGH_PWM_B,		   VDDEE_VAL_REG, 0xffffffff, 0, 0, 0 },
//	{ PWMIJ_PWM_B,		   VCCK_VAL_REG,  0xffffffff, 0, 0, 0 },
//	{ PWMGH_MISC_REG_AB,	   (0x1 << 1),	  (0x1 << 1), 0, 0, 0 },
//	{ PWMIJ_MISC_REG_AB,	   (0x1 << 1),	  (0x1 << 1), 0, 0, 0 },
#ifdef CONFIG_PDVFS_ENABLE
	{PWMGH_PWM_B, 0x8000a, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_1, 0},
	{PWMGH_PWM_B, 0x5000d, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_2, 0},
	{PWMGH_PWM_B, 0x20010, 0xffffffff, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_3, 0},
#else
	{PWMGH_PWM_B, VDDEE_VAL_REG, 0xffffffff, 0, 0, 0},
#endif
	{PWMIJ_PWM_B, VCCK_VAL_REG, 0xffffffff, 0, 0, 0},
	{PWMGH_MISC_REG_AB, (0x1 << 1),  (0x1 << 1), 0, 0, 0},
	{PWMIJ_MISC_REG_AB, (0x1 << 1),  (0x1 << 1), 0, 0, 0},
	/* set pwm h and pwm j clock rate to 24M, enable them */
	{ CLKCTRL_PWM_CLK_GH_CTRL, (1 << 24),	  0xffffffff, 0, 0, 0 },
	{ CLKCTRL_PWM_CLK_IJ_CTRL, (1 << 24),	  0xffffffff, 0, 0, 0 },
	/* set GPIOE_0 GPIOE_1 drive strength to 3 */
	{ PADCTRL_GPIOE_DS,	   0xf,		  0xf,	      0, 0, 0 },
	/* set GPIOE_0 GPIOE_1 mux to pwmh pwmj */
	{ PADCTRL_PIN_MUX_REGI,	   (0x3 << 0),	  (0xf << 0), 0, 0, 0 },
	{ PADCTRL_PIN_MUX_REGI,	   (0x3 << 4),	  (0xf << 4), 0, 0, 0 },
	{ PADCTRL_GPIOD_PULL_UP,   (0x1 << 2),	  (0x1 << 2), 0, 0, 0 },
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
