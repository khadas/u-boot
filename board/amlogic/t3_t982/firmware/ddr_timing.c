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

#define T3_DDR_CODE_NOT_USE	0

#define T3_2GB_SAM_DDR4_X4_EID       0x66C6A

#define DDR_ID_ACS_ADC   ((3 << 6) | (8)) //bit 6 adc_channel bit 0-5 adc value,chan 3 value 8 is layer 2

#define DDR_RESV_CHECK_ID_ENABLE  0Xfe
#define SAR_ADC_DDR_ID_BASE   0
#define SAR_ADC_DDR_ID_STEP   80
#define BOARD_1G_1G_ADC_ID   (SAR_ADC_DDR_ID_BASE + SAR_ADC_DDR_ID_STEP)
#define BOARD_2G_1G_ADC_ID   (SAR_ADC_DDR_ID_BASE + SAR_ADC_DDR_ID_STEP + SAR_ADC_DDR_ID_STEP)
#define DDR_TIMMING_OFFSET(X) (unsigned int)(unsigned long)(&(((ddr_set_ps0_only_t *)(0))->X))
#define DDR_TIMMING_OFFSET_SIZE(X) sizeof(((ddr_set_ps0_only_t *)(0))->X)
#define DDR_TIMMING_TUNE_TIMMING0(DDR_ID, PARA, VALUE) { DDR_ID, DDR_TIMMING_OFFSET(PARA), VALUE, DDR_TIMMING_OFFSET_SIZE(PARA), 0, DDR_RESV_CHECK_ID_ENABLE }
#define DDR_TIMMING_TUNE_TIMMING1(DDR_ID, PARA, VALUE) { DDR_ID, sizeof(((ddr_set_ps0_only_t) + DDR_TIMMING_OFFSET(PARA), VALUE, DDR_TIMMING_OFFSET_SIZE(PARA), 0, DDR_RESV_CHECK_ID_ENABLE }

//bit24-31 define ID and size
#define DDR_ID_FROM_EFUSE  (0Xffu << 24)
#define DDR_ID_FROM_ADC  (0Xfeu << 24)
#define DDR_ID_FROM_GPIO_CONFIG1  (0Xfdu << 24)
#define DDR_ID_START_MASK  (0XFFDDCCBBu)

#define DDR_ADC_CH0  (0X0u << 6)
#define DDR_ADC_CH1  (0X1u << 6)
#define DDR_ADC_CH2  (0X2u << 6)
#define DDR_ADC_CH3  (0X3u << 6)

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
#define DDR_TIMMING_TUNE_TIMMING0_F(PARA, VALUE) (((DDR_TIMMING_OFFSET(PARA)) << 16) | ((DDR_TIMMING_OFFSET_SIZE(PARA)) << 28) | (VALUE))
#define DDR_TIMMING_TUNE_TIMMING1_F(PARA, VALUE) (((sizeof(ddr_set_ps0_only_t) + DDR_TIMMING_OFFSET(PARA)) << 16) | ((DDR_TIMMING_OFFSET_SIZE(PARA)) << 28) | (VALUE))

#define DDR_TIMMING_TUNE_START(id_src_from, id_adc_ch, id_value)  (id_src_from | id_adc_ch | id_value)
#define DDR_TIMMING_TUNE_STRUCT_SIZE(a)  sizeof(a)

#if 1
uint32_t __bl2_ddr_reg_data[] __attribute__ ((section(".ddr_2acs_data"))) = {
	//DDR_ID
	DDR_TIMMING_TUNE_START(DDR_ID_FROM_ADC, DDR_ADC_CH3, DDR_ADC_VALUE1),
	//data start,2GB,DDR0-1GB,DDR1-1GB
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_common_setting.dram_ch0_size_MB,
		((DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_common_setting.dram_ch1_size_MB,
		((DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_common_setting.dram_ch0_size_MB,
		((DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_common_setting.dram_ch1_size_MB,
		((DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_SI_setting_ps.DRAMFreq, 1176),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_SI_setting_ps.DRAMFreq, 1176),

	DDR_TIMMING_TUNE_START(DDR_ID_FROM_ADC, DDR_ADC_CH3, DDR_ADC_VALUE2),
	//data start,3GB,DDR0-2GB,DDR1-1GB
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_common_setting.dram_ch0_size_MB,
		((DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_common_setting.dram_ch1_size_MB,
		((DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_common_setting.dram_ch0_size_MB,
		((DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_common_setting.dram_ch1_size_MB,
		((DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_SI_setting_ps.DRAMFreq, 1176),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_SI_setting_ps.DRAMFreq, 1176),

	DDR_TIMMING_TUNE_START(DDR_ID_FROM_ADC, DDR_ADC_CH3, DDR_ADC_VALUE7),
	//data start,4GB,DDR0-2GB,DDR1-2GB
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_common_setting.dram_ch0_size_MB,
		((DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_common_setting.dram_ch1_size_MB,
		((DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_common_setting.dram_ch0_size_MB,
		((DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_common_setting.dram_ch1_size_MB,
		((DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_SI_setting_ps.DRAMFreq, 1176),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_SI_setting_ps.DRAMFreq, 1176),

	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_SI_setting_ps.clk_drv_ohm, DDR_SOC_AC_DRV_40_OHM),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_SI_setting_ps.cs_drv_ohm, DDR_SOC_AC_DRV_40_OHM),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_SI_setting_ps.ac_drv_ohm, DDR_SOC_AC_DRV_40_OHM),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_SI_setting_ps.clk_drv_ohm, DDR_SOC_AC_DRV_40_OHM),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_SI_setting_ps.cs_drv_ohm, DDR_SOC_AC_DRV_40_OHM),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_SI_setting_ps.ac_drv_ohm, DDR_SOC_AC_DRV_40_OHM),

	//cs0
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.ac_trace_delay[2], 0x94),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.ac_trace_delay[2], 0x94),

	//dq offset
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.reserve_training_parameter[0], (0 << 7) | 10),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.reserve_training_parameter[1], (0 << 7) | 0),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.reserve_training_parameter[2], (0 << 7) | 4),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.reserve_training_parameter[3], (0 << 7) | 4),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.reserve_training_parameter[8], (1 << 7) | 16),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.reserve_training_parameter[9], (1 << 7) | 16),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.reserve_training_parameter[10], (1 << 7) | 16),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.reserve_training_parameter[11], (1 << 7) | 16),

	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.reserve_training_parameter[0], (0 << 7) | 10),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.reserve_training_parameter[1], (0 << 7) | 0),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.reserve_training_parameter[2], (0 << 7) | 4),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.reserve_training_parameter[3], (0 << 7) | 4),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.reserve_training_parameter[8], (1 << 7) | 16),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.reserve_training_parameter[9], (1 << 7) | 16),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.reserve_training_parameter[10], (1 << 7) | 16),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.reserve_training_parameter[11], (1 << 7) | 16),

	//delay line
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dqs_delay[0] ,0x000000eb),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dqs_delay[1] ,0x000000d7),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dqs_delay[2] ,0x000000ef),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dqs_delay[3] ,0x000000e5),

#if T3_DDR_CODE_NOT_USE
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[0] ,0x00000136),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[1] ,0x0000011d),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[2] ,0x00000132),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[3] ,0x0000011d),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[4] ,0x00000132),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[5] ,0x00000116),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[6] ,0x0000011d),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[7] ,0x00000123),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[8] ,0x00000126),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[9] ,0x00000114),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[10] ,0x00000111),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[11] ,0x0000011a),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[12] ,0x0000010e),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[13] ,0x00000110),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[14] ,0x0000010c),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[15] ,0x00000111),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[16] ,0x0000010d),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[17] ,0x00000114),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[18] ,0x0000012b),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[19] ,0x0000011c),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[20] ,0x0000012f),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[21] ,0x00000122),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[22] ,0x00000128),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[23] ,0x00000118),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[24] ,0x0000012f),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[25] ,0x00000124),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[26] ,0x00000121),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[27] ,0x00000129),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[28] ,0x0000011e),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[29] ,0x0000012f),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[30] ,0x00000118),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[31] ,0x00000133),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[32] ,0x0000011c),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[33] ,0x0000012c),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[34] ,0x00000119),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[35] ,0x00000125),
#endif
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] ,0x000001e6),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] ,0x000001f7),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] ,0x000001e4),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] ,0x000001fc),

	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dqs_delay[0] ,0x0000007a),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dqs_delay[1] ,0x0000007d),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dqs_delay[2] ,0x00000082),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dqs_delay[3] ,0x0000007a),
#if T3_DDR_CODE_NOT_USE
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[0] ,0x00000052),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[1] ,0x0000003d),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[2] ,0x0000004a),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[3] ,0x00000034),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[4] ,0x0000004f),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[5] ,0x00000035),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[6] ,0x00000045),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[7] ,0x0000003d),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[8] ,0x00000042),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[9] ,0x0000004a),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[10] ,0x00000039),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[11] ,0x00000045),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[12] ,0x00000035),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[13] ,0x00000043),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[14] ,0x00000034),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[15] ,0x00000044),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[16] ,0x00000038),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[17] ,0x0000003e),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[18] ,0x00000042),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[19] ,0x00000031),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[20] ,0x00000041),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[21] ,0x00000037),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[22] ,0x0000003c),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[23] ,0x00000032),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[24] ,0x00000056),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[25] ,0x00000045),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[26] ,0x0000003e),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[27] ,0x0000004d),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[28] ,0x0000003c),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[29] ,0x0000004d),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[30] ,0x00000036),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[31] ,0x0000005e),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[32] ,0x00000040),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[33] ,0x0000005f),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[34] ,0x0000003b),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[35] ,0x00000048),
#endif
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dqs_delay[0] ,0x000000cd),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dqs_delay[1] ,0x000000d2),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dqs_delay[2] ,0x000000c8),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dqs_delay[3] ,0x000000c3),
#if T3_DDR_CODE_NOT_USE
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[0] ,0x00000119),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[1] ,0x00000106),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[2] ,0x00000114),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[3] ,0x00000100),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[4] ,0x00000115),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[5] ,0x00000107),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[6] ,0x0000010e),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[7] ,0x00000109),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[8] ,0x0000010c),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[9] ,0x00000119),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[10] ,0x0000010a),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[11] ,0x00000119),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[12] ,0x000000fe),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[13] ,0x00000111),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[14] ,0x000000f8),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[15] ,0x000000ff),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[16] ,0x0000010f),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[17] ,0x00000108),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[18] ,0x00000103),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[19] ,0x000000f1),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[20] ,0x00000111),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[21] ,0x000000f5),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[22] ,0x000000f7),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[23] ,0x000000e7),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[24] ,0x000000f6),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[25] ,0x000000ea),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[26] ,0x000000fc),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[27] ,0x0000010e),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[28] ,0x000000f0),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[29] ,0x00000114),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[30] ,0x00000101),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[31] ,0x00000109),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[32] ,0x000000f4),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[33] ,0x00000101),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[34] ,0x000000fb),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.write_dq_bit_delay[35] ,0x00000100),
#endif
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] ,0x000001e4),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] ,0x000001e6),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] ,0x000001e0),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] ,0x000001e7),

	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dqs_delay[0] ,0x00000084),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dqs_delay[1] ,0x0000008a),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dqs_delay[2] ,0x0000008c),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dqs_delay[3] ,0x00000083),
#if T3_DDR_CODE_NOT_USE
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[0] ,0x0000005a),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[1] ,0x00000034),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[2] ,0x00000049),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[3] ,0x00000038),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[4] ,0x0000004b),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[5] ,0x00000035),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[6] ,0x0000004f),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[7] ,0x00000040),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[8] ,0x00000043),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[9] ,0x00000061),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[10] ,0x0000004a),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[11] ,0x00000052),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[12] ,0x00000037),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[13] ,0x0000004d),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[14] ,0x00000035),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[15] ,0x00000043),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[16] ,0x0000004e),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[17] ,0x00000048),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[18] ,0x00000051),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[19] ,0x00000040),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[20] ,0x00000064),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[21] ,0x0000003c),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[22] ,0x0000004b),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[23] ,0x00000037),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[24] ,0x00000050),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[25] ,0x00000036),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[26] ,0x00000047),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[27] ,0x00000059),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[28] ,0x00000037),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[29] ,0x00000058),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[30] ,0x00000047),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[31] ,0x00000053),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[32] ,0x0000003c),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[33] ,0x00000052),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[34] ,0x00000043),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.read_dq_bit_delay[35] ,0x0000004a),
#endif
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.soc_bit_vref[0] ,0),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.dram_bit_vref[0] ,0),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.soc_bit_vref[0] ,0),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.dram_bit_vref[0] ,0),

	DDR_TIMMING_TUNE_START(DDR_ID_FROM_ADC, DDR_ADC_CH3, DDR_ADC_VALUE8),
	//data start,4GB,DDR0-2GB,DDR1-2GB
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_common_setting.dram_ch0_size_MB,
		((DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_common_setting.dram_ch1_size_MB,
		((DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_common_setting.dram_ch0_size_MB,
		((DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_common_setting.dram_ch1_size_MB,
		((DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX4 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
		 (DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET))),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_SI_setting_ps.DRAMFreq, 1176),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_SI_setting_ps.DRAMFreq, 1176),

	//HW_ID
	DDR_TIMMING_TUNE_START(DDR_ID_FROM_ADC, DDR_ADC_CH0, DDR_ADC_VALUE2),
	//.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 28 + 30,
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.ac_trace_delay[2], 28 + 30),
	//.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 48 + 30,
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.ac_trace_delay[2], 48 + 30),

	//SIP
	DDR_TIMMING_TUNE_START(DDR_ID_FROM_EFUSE, 0, T3_2GB_SAM_DDR4_X4_EID),
	//data start
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_SI_setting_ps.soc_data_odt_ohm_p,
		DDR_SOC_DATA_DRV_ODT_80_OHM),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_SI_setting_ps.soc_data_odt_ohm_p,
		DDR_SOC_DATA_DRV_ODT_80_OHM),
	//.cfg_board_SI_setting_ps.dfe_offset = 0x91u,
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_SI_setting_ps.dfe_offset, 0x93u),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_SI_setting_ps.dfe_offset, 0x93u),
	//cfg_ddr_training_delay_ps.reserve_training_parameter[13] = (1 << 7) | 6;
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_ddr_training_delay_ps.reserve_training_parameter[9], ((1 << 7) | 0x16) + 6),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_ddr_training_delay_ps.reserve_training_parameter[9], ((1 << 7) | 0x16) + 6),
	DDR_TIMMING_TUNE_TIMMING0_F(cfg_board_SI_setting_ps.DRAMFreq, 1176),
	DDR_TIMMING_TUNE_TIMMING1_F(cfg_board_SI_setting_ps.DRAMFreq, 1176),
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

//#define ENABLE_SKT_BOARD 1		//SKT AR319 6layer 4pcs ddr4
#define ENABLE_REF_BOARD 1      //AR331 2layer 4pcs ddr4
//#define REF_BOARD_USE_4layer 1      //T982/T965D4 4layer 4pcs ddr4
//#define REF_BOARD_USE_6layer 1      //T982/T965D4 6layer 4pcs ddr4

#if ENABLE_REF_BOARD            //timing_config,AR331 2layer 4pcs ddr4
ddr_set_ps0_only_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {
	{//start ch0
		.cfg_board_common_setting.timming_magic = 0,
		//sizeof(__ddr_setting) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_max_valid_configs = 0,
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,                                                                           //0
		.cfg_board_common_setting.fast_boot = { 0					       },
		.cfg_board_common_setting.ddr_func = DDR_FUNC_CONFIG_DFE_FUNCTION,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_DDR4,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
		.cfg_board_common_setting.DisabledDbyte = CONFIG_DISABLE_D32_D63,
		.cfg_board_common_setting.dram_cs0_base_add = 0,
		.cfg_board_common_setting.dram_cs1_base_add = 0,
		//.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,
		//.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
		.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		.cfg_board_common_setting.log_level = LOG_LEVEL_BASIC,
		//DDR_WRITE_READ_DBI_ENABLE,//DDR_WRITE_READ_DBI_DISABLE,
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
		 //center_ssc_1000ppm,//SSC_DISABLE,(1 << 20) | (0 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.pll_ssc_mode = (1 << 20) | (1 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test_function = { 0						    },
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR4_32BIT,
#if 0
		.cfg_board_common_setting.ac_pinmux = {
			0,  0,	0,  1,	2,  3,	8, 27,
			10, 2,	9,  21, 5,  14, 1, 0,
			25, 13, 12, 4,	7,  22, 0, 0,
			0,  0,	6,  3,	20, 0,	0, 15,
			26, 11,
		},
#endif
#if 0
		//T3 sip ac pinmux
		.cfg_board_common_setting.ac_pinmux = {
			//sip
			0, 0,  0,  1, 2, 3,
			8, 27, 10, 2, 9, 21, 5,	23,
			1, 0, 25, 13, 12, 4, 7, 28,
			0, 0, 0, 0, 6, 3, 20, 0, 0,
			24, 26, 11, 0
		},
#endif
#if 1
		.cfg_board_common_setting.ac_pinmux = {
			//test chip
			0, 0,  0,  1, 2,  3,
			3, 28, 20, 5, 10, 13, 11, 8,
			0, 12, 27, 9, 1, 6, 4, 25,
			0, 0, 0, 0, 2, 7, 23, 0, 0,
			24, 26, 21, 0
		},
#endif

		.cfg_board_common_setting.ddr_dqs_swap = 0,
		.cfg_board_common_setting.ddr_dq_remap = {
			0,  1,	2,  3,	4,  5,	6,  7,
			8,  9,	10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, 29, 30, 31,
			32, 33, 34, 35
		},                                                   //d0-d31 dm0 dm1 dm2 dm3

		.cfg_board_common_setting.ddr_vddee_setting = { 0					       },
		.cfg_board_SI_setting_ps.DRAMFreq = 1176,
		.cfg_board_SI_setting_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		//DDR_DRAM_ODT_W_CS0_ODT0,DDR_DRAM_ODT_DDR4_PARK_ENABLE
		.cfg_board_SI_setting_ps.ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		.cfg_board_SI_setting_ps.clk_drv_ohm = DDR_SOC_AC_DRV_60_OHM,
		.cfg_board_SI_setting_ps.cs_drv_ohm = DDR_SOC_AC_DRV_60_OHM,
		.cfg_board_SI_setting_ps.ac_drv_ohm = DDR_SOC_AC_DRV_60_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                        //for pxp dram weak driver model
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_48_OHM,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_60_OHM,
		//.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_240_OHM,

#if 0
		.cfg_board_SI_setting_ps.ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,                                               //DDR_DRAM_ODT_W_CS0_ODT0,//DDR_DRAM_ODT_W_CS0_ODT0_CS1_ODT1,//DDR_DRAM_ODT_W_CS0_ODT0,
		.cfg_board_SI_setting_ps.clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                        //for pxp dram weak driver model
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_80_OHM,
		.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_0_OHM,
#endif
		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_pull_up_calibration_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_voltage_range_setting =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		.cfg_board_SI_setting_ps.dfe_offset = 0x66u,
		.cfg_board_SI_setting_ps.vref_ac_permil = 0,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,                                                    //0,
		.cfg_board_SI_setting_ps.training_phase_parameter = { 0						},
		.cfg_board_SI_setting_ps.ac_trace_delay_org = {
			128, 128, 128, 128, 128, 128, 128, 128,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384,
		},                                             //total 36
		.cfg_ddr_training_delay_ps.ac_trace_delay = {
			128, 128, 128, 128, 128, 128, 128, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384,
		},

		.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		//cs0 write dqs,lane0-lane3
		//20220806,ch0 lane0 + 9step,for stress fail;
		(0 << 7) | 9, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		//cs1 write dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		//cs0 read dqs,lane0-lane3
		(1 << 7) | 0xb, (1 << 7) | 0xb, (1 << 7) | 0xb, (1 << 7) | 0xb,
		//cs1 read dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		},

		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0x000002a8,          // 680
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0x000002bc,         // 700
		.cfg_board_SI_setting_ps.ac_trace_delay_org[0] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[1] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[2] = 0x00000058,         // 88
		.cfg_board_SI_setting_ps.ac_trace_delay_org[3] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[4] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[5] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[6] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[7] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[8] = 0x00000180,         // 384
		.cfg_board_SI_setting_ps.ac_trace_delay_org[9] = 0x00000180,         // 384
		.cfg_board_SI_setting_ps.ac_trace_delay_org[10] = 0x00000100,        // 256
		.cfg_board_SI_setting_ps.ac_trace_delay_org[11] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[12] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[13] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[14] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[15] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[16] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[17] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[18] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[19] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[20] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[21] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[22] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[23] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[24] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[25] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[26] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[27] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[28] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[29] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[30] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[31] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[32] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[33] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[34] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[35] = 0x00000000,        // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 0x00000080,                   // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 0x00000080,                   // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 0x00000080 - 100,    // 88
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 0x00000080,                   // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 0x00000180 - 40,      // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 0x00000180 - 40,      // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 0x00000180 - 40,      // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[30] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[31] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[32] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[33] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[34] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[35] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x00000105,// 261
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x000000eb,// 235
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x00000114,// 276
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0x000000f5,// 245
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x00000102,          // 258
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x00000102,          // 258
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x00000102,          // 258
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x00000102,          // 258

		.cfg_ddr_training_delay_ps.write_dq_bit_delay[0] = 0x00000130,// 304
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[1] = 0x0000012d,// 301
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[2] = 0x00000139,// 313
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[3] = 0x0000011f,// 287
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[4] = 0x0000013e,// 318
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[5] = 0x00000142,// 322
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[6] = 0x0000014e,// 334
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[7] = 0x0000012c,// 300
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[8] = 0x00000133,// 307
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[9] = 0x00000131,// 305
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[10] = 0x00000123,// 291
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[11] = 0x0000012b,// 299
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[12] = 0x0000011e,// 286
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[13] = 0x00000131,// 305
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[14] = 0x00000137,// 311
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[15] = 0x0000012d,// 301
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[16] = 0x0000012a,// 298
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[17] = 0x00000129,// 297
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[18] = 0x0000014c,// 332
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[19] = 0x00000136,// 310
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[20] = 0x0000014e,// 334
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[21] = 0x0000013d,// 317
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[22] = 0x00000149,// 329
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[23] = 0x0000013a,// 314
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[24] = 0x00000156,// 342
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[25] = 0x0000014d,// 333
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[26] = 0x00000143,// 323
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[27] = 0x0000013e,// 318
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[28] = 0x0000012c,// 300
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[29] = 0x00000147,// 327
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[30] = 0x00000129,// 297
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[31] = 0x0000014c,// 332
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[32] = 0x0000013b,// 315
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[33] = 0x00000149,// 329
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[34] = 0x00000132,// 306
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[35] = 0x00000139,// 313
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
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0x0000022a,// 554
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0x0000023d,// 573
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0x00000213,// 531
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0x00000234,// 564
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x0000008b,// 139
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x0000007f,// 127
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x00000080,// 128
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x0000007d,// 125
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[0] = 0x0000003c,// 60
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[1] = 0x00000031,// 49
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[2] = 0x0000003f,// 63
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[3] = 0x00000034,// 52
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[4] = 0x00000045,// 69
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[5] = 0x00000049,// 73
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[6] = 0x00000058,// 88
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[7] = 0x00000034,// 52
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[8] = 0x0000003f,// 63
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[9] = 0x00000044,// 68
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[10] = 0x00000035,// 53
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[11] = 0x00000040,// 64
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[12] = 0x00000032,// 50
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[13] = 0x00000043,// 67
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[14] = 0x0000004b,// 75
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[15] = 0x00000045,// 69
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[16] = 0x0000003c,// 60
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[17] = 0x0000003f,// 63
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[18] = 0x0000003e,// 62
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[19] = 0x0000002e,// 46
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[20] = 0x00000043,// 67
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[21] = 0x0000002e,// 46
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[22] = 0x0000003d,// 61
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[23] = 0x0000002f,// 47
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[24] = 0x00000049,// 73
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[25] = 0x0000003b,// 59
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[26] = 0x00000039,// 57
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[27] = 0x0000004e,// 78
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[28] = 0x00000038,// 56
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[29] = 0x0000004b,// 75
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[30] = 0x00000035,// 53
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[31] = 0x00000059,// 89
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[32] = 0x00000043,// 67
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[33] = 0x00000051,// 81
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[34] = 0x00000042,// 66
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[35] = 0x00000046,// 70
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
		.cfg_ddr_training_delay_ps.soc_bit_vref[0] = 0x0000000,              // 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[1] = 0x00000026,             // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[2] = 0x00000029,             // 41
		.cfg_ddr_training_delay_ps.soc_bit_vref[3] = 0x00000025,             // 37
		.cfg_ddr_training_delay_ps.soc_bit_vref[4] = 0x00000024,             // 36
		.cfg_ddr_training_delay_ps.soc_bit_vref[5] = 0x0000002b,             // 43
		.cfg_ddr_training_delay_ps.soc_bit_vref[6] = 0x00000027,             // 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[7] = 0x00000027,             // 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[8] = 0x00000026,             // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[9] = 0x00000027,             // 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[10] = 0x00000022,            // 34
		.cfg_ddr_training_delay_ps.soc_bit_vref[11] = 0x00000023,            // 35
		.cfg_ddr_training_delay_ps.soc_bit_vref[12] = 0x00000023,            // 35
		.cfg_ddr_training_delay_ps.soc_bit_vref[13] = 0x00000023,            // 35
		.cfg_ddr_training_delay_ps.soc_bit_vref[14] = 0x00000023,            // 35
		.cfg_ddr_training_delay_ps.soc_bit_vref[15] = 0x00000024,            // 36
		.cfg_ddr_training_delay_ps.soc_bit_vref[16] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[17] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[18] = 0x00000027,            // 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[19] = 0x00000028,            // 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[20] = 0x00000025,            // 37
		.cfg_ddr_training_delay_ps.soc_bit_vref[21] = 0x00000024,            // 36
		.cfg_ddr_training_delay_ps.soc_bit_vref[22] = 0x00000024,            // 36
		.cfg_ddr_training_delay_ps.soc_bit_vref[23] = 0x00000025,            // 37
		.cfg_ddr_training_delay_ps.soc_bit_vref[24] = 0x0000002a,            // 42
		.cfg_ddr_training_delay_ps.soc_bit_vref[25] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[26] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[27] = 0x00000023,            // 35
		.cfg_ddr_training_delay_ps.soc_bit_vref[28] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[29] = 0x00000024,            // 36
		.cfg_ddr_training_delay_ps.soc_bit_vref[30] = 0x00000028,            // 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[31] = 0x00000027,            // 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[32] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[33] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[34] = 0x00000025,            // 37
		.cfg_ddr_training_delay_ps.soc_bit_vref[35] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[36] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[37] = 0x00000030,            // 48
		.cfg_ddr_training_delay_ps.soc_bit_vref[38] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[39] = 0x0000002f,            // 47
		.cfg_ddr_training_delay_ps.soc_bit_vref[40] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[41] = 0x0000002f,            // 47
		.cfg_ddr_training_delay_ps.soc_bit_vref[42] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[43] = 0x0000002b,            // 43
		.cfg_ddr_training_delay_ps.dram_bit_vref[0] = 0x0,                   // 0
#if REF_BOARD_USE_4layer
	.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 0x00000080,// 128
	.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 0x00000080,// 128
	.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 0x00000096,// 150
	.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 0x00000080,// 128
	.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 0x00000080,// 128
	.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 0x00000080,// 128
	.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 0x00000080,// 128
	.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[30] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[31] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[32] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[33] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[34] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[35] = 0x00000158,// 344

	.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x000000eb,// 235
	.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x000000d7,// 215
	.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x000000ef,// 239
	.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0x000000e5,// 229
	.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x00000102,// 258
	.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x00000102,// 258
	.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x00000102,// 258
	.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x00000102,// 258

	.cfg_ddr_training_delay_ps.write_dq_bit_delay[0] = 0x00000136,// 310
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[1] = 0x0000011d,// 285
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[2] = 0x00000132,// 306
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[3] = 0x0000011d,// 285
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[4] = 0x00000132,// 306
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[5] = 0x00000116,// 278
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[6] = 0x0000011d,// 285
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[7] = 0x00000123,// 291
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[8] = 0x00000126,// 294
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[9] = 0x00000114,// 276
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[10] = 0x00000111,// 273
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[11] = 0x0000011a,// 282
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[12] = 0x0000010e,// 270
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[13] = 0x00000110,// 272
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[14] = 0x0000010c,// 268
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[15] = 0x00000111,// 273
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[16] = 0x0000010d,// 269
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[17] = 0x00000114,// 276
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[18] = 0x0000012b,// 299
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[19] = 0x0000011c,// 284
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[20] = 0x0000012f,// 303
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[21] = 0x00000122,// 290
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[22] = 0x00000128,// 296
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[23] = 0x00000118,// 280
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[24] = 0x0000012f,// 303
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[25] = 0x00000124,// 292
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[26] = 0x00000121,// 289
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[27] = 0x00000129,// 297
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[28] = 0x0000011e,// 286
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[29] = 0x0000012f,// 303
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[30] = 0x00000118,// 280
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[31] = 0x00000133,// 307
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[32] = 0x0000011c,// 284
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[33] = 0x0000012c,// 300
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[34] = 0x00000119,// 281
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[35] = 0x00000125,// 293
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0x000001e6,// 486
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0x000001f7,// 503
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0x000001e4,// 484
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0x000001fc,// 508
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 0x00000000,// 0

	.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x0000007a,// 122
	.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x0000007d,// 125
	.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x00000082,// 130
	.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x0000007a,// 122
	.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[0] = 0x00000052,// 82
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[1] = 0x0000003d,// 61
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[2] = 0x0000004a,// 74
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[3] = 0x00000034,// 52
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[4] = 0x0000004f,// 79
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[5] = 0x00000035,// 53
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[6] = 0x00000045,// 69
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[7] = 0x0000003d,// 61
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[8] = 0x00000042,// 66
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[9] = 0x0000004a,// 74
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[10] = 0x00000039,// 57
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[11] = 0x00000045,// 69
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[12] = 0x00000035,// 53
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[13] = 0x00000043,// 67
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[14] = 0x00000034,// 52
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[15] = 0x00000044,// 68
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[16] = 0x00000038,// 56
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[17] = 0x0000003e,// 62
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[18] = 0x00000042,// 66
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[19] = 0x00000031,// 49
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[20] = 0x00000041,// 65
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[21] = 0x00000037,// 55
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[22] = 0x0000003c,// 60
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[23] = 0x00000032,// 50
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[24] = 0x00000056,// 86
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[25] = 0x00000045,// 69
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[26] = 0x0000003e,// 62
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[27] = 0x0000004d,// 77
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[28] = 0x0000003c,// 60
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[29] = 0x0000004d,// 77
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[30] = 0x00000036,// 54
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[31] = 0x0000005e,// 94
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[32] = 0x00000040,// 64
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[33] = 0x0000005f,// 95
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[34] = 0x0000003b,// 59
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[35] = 0x00000048,// 72
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
	.cfg_ddr_training_delay_ps.soc_bit_vref[0] = 0x00000000,
	.cfg_ddr_training_delay_ps.dram_bit_vref[0] = 0x0,

	.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		//cs0 write dqs,lane0-lane3
		(0 << 7) | 0xa, (0 << 7) | 0, (0 << 7) | 0x4, (0 << 7) | 0x4,
		//cs1 write dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		//cs0 read dqs,lane0-lane3
		(1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10,
		//cs1 read dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
	},
#endif //end REF_BOARD_USE_4layer
#if REF_BOARD_USE_6layer
	.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 0x00000080,
	.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 0x00000080,
	.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 0x00000096,
	.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 0x00000080,
	.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 0x00000080,
	.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 0x00000080,
	.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 0x00000080,
	.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 0x00000000,
	.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 0x00000000,
	.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 0x00000000,
	.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 0x00000000,
	.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[30] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[31] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[32] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[33] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[34] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[35] = 0x00000158,
	.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x000000b9,
	.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x000000af,
	.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x000000b9,
	.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0x000000af,
	.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x00000102,
	.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x00000102,
	.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x00000102,
	.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x00000102,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[0] = 0x000000fe,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[1] = 0x000000f1,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[2] = 0x000000fc,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[3] = 0x000000ed,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[4] = 0x000000fe,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[5] = 0x000000ea,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[6] = 0x000000ee,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[7] = 0x000000f8,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[8] = 0x000000f6,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[9] = 0x000000e8,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[10] = 0x000000e2,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[11] = 0x000000ec,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[12] = 0x000000e2,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[13] = 0x000000e2,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[14] = 0x000000e1,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[15] = 0x000000e5,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[16] = 0x000000e4,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[17] = 0x000000e4,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[18] = 0x000000f8,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[19] = 0x000000ed,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[20] = 0x000000f6,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[21] = 0x000000f0,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[22] = 0x000000f2,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[23] = 0x000000e6,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[24] = 0x000000ff,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[25] = 0x000000f6,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[26] = 0x000000f4,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[27] = 0x000000f7,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[28] = 0x000000f1,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[29] = 0x000000fd,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[30] = 0x000000e9,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[31] = 0x00000100,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[32] = 0x000000ed,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[33] = 0x000000fb,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[34] = 0x000000e7,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[35] = 0x000000f3,
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0x000001c3,
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0x000001d0,
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0x000001b1,
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0x000001c7,
	.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x00000085,
	.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x00000081,
	.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x00000091,
	.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x0000007c,
	.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x00000000,
	.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x00000000,
	.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x00000000,
	.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x00000000,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[0] = 0x0000004e,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[1] = 0x0000003e,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[2] = 0x00000045,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[3] = 0x00000037,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[4] = 0x0000004c,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[5] = 0x00000038,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[6] = 0x00000045,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[7] = 0x0000003e,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[8] = 0x00000041,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[9] = 0x00000045,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[10] = 0x00000039,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[11] = 0x00000043,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[12] = 0x00000039,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[13] = 0x00000041,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[14] = 0x00000038,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[15] = 0x00000042,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[16] = 0x0000003b,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[17] = 0x0000003e,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[18] = 0x0000004c,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[19] = 0x00000040,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[20] = 0x0000004b,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[21] = 0x00000049,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[22] = 0x0000004a,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[23] = 0x0000003c,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[24] = 0x00000062,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[25] = 0x00000058,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[26] = 0x0000004c,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[27] = 0x00000045,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[28] = 0x00000038,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[29] = 0x00000041,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[30] = 0x00000035,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[31] = 0x0000004d,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[32] = 0x0000003c,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[33] = 0x0000004d,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[34] = 0x0000003a,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[35] = 0x00000040,
	.cfg_ddr_training_delay_ps.soc_bit_vref[0] = 0x00000000,
	.cfg_ddr_training_delay_ps.dram_bit_vref[0] = 0x0,
	.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		//cs0 write dqs,lane0-lane3
		(0 << 7) | 0xa, (0 << 7) | 0, (0 << 7) | 0x4, (0 << 7) | 0x4,
		//cs1 write dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		//cs0 read dqs,lane0-lane3
		(1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10,
		//cs1 read dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
	},
#endif //end REF_BOARD_USE_6layer
	},//end ch0
	{//start ch1
		.cfg_board_common_setting.timming_magic = 0,
		//sizeof(__ddr_setting) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_max_valid_configs = 0,
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,                                                                           //0
		.cfg_board_common_setting.fast_boot = { 0					       },
		.cfg_board_common_setting.ddr_func = DDR_FUNC_CONFIG_DFE_FUNCTION,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_DDR4,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
		.cfg_board_common_setting.DisabledDbyte = CONFIG_DISABLE_D32_D63,
		.cfg_board_common_setting.dram_cs0_base_add = 0,
		.cfg_board_common_setting.dram_cs1_base_add = 0,
		//.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,
		//.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
		.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		.cfg_board_common_setting.log_level = LOG_LEVEL_BASIC,
		//DDR_WRITE_READ_DBI_ENABLE,//DDR_WRITE_READ_DBI_DISABLE,
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
		 //center_ssc_1000ppm,//SSC_DISABLE,(1 << 20) | (0 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.pll_ssc_mode = (1 << 20) | (1 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test_function = { 0						    },
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR4_32BIT,
#if T3_DDR_CODE_NOT_USE
		.cfg_board_common_setting.ac_pinmux = {
			0,  0,	0,  1,	2,  3,	8, 27,
			10, 2,	9,  21, 5,  14, 1, 0,
			25, 13, 12, 4,	7,  22, 0, 0,
			0,  0,	6,  3,	20, 0,	0, 15,
			26, 11,
		},
#endif
#if T3_DDR_CODE_NOT_USE
		//T3 sip ac pinmux
		.cfg_board_common_setting.ac_pinmux = {
			//sip
			0, 0,  0,  1, 2, 3,
			8, 27, 10, 2, 9, 21, 5,	23,
			1, 0, 25, 13, 12, 4, 7, 28,
			0, 0, 0, 0, 6, 3, 20, 0, 0,
			24, 26, 11, 0
		},
#endif
		.cfg_board_common_setting.ac_pinmux = {
			//test chip
			0, 0,  0, 1,  2, 3,
			5, 28, 0, 26, 1, 20, 3,	10,
			9, 13, 8, 25, 27, 2, 12, 6,
			0, 0, 0, 0, 4, 7, 11, 0, 0,
			24, 23, 21, 0
		},

		.cfg_board_common_setting.ddr_dqs_swap = 0,
		.cfg_board_common_setting.ddr_dq_remap = {
			0,  1,	2,  3,	4,  5,	6,  7,
			8,  9,	10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, 29, 30, 31,
			32, 33, 34, 35
		},                                                   //d0-d31 dm0 dm1 dm2 dm3

		.cfg_board_common_setting.ddr_vddee_setting = { 0					       },
		.cfg_board_SI_setting_ps.DRAMFreq = 1176,
		.cfg_board_SI_setting_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		//DDR_DRAM_ODT_W_CS0_ODT0,DDR_DRAM_ODT_DDR4_PARK_ENABLE
		.cfg_board_SI_setting_ps.ddr_odt_config = DDR_DRAM_ODT_W_CS0_ODT0,
		.cfg_board_SI_setting_ps.clk_drv_ohm = DDR_SOC_AC_DRV_60_OHM,
		.cfg_board_SI_setting_ps.cs_drv_ohm = DDR_SOC_AC_DRV_60_OHM,
		.cfg_board_SI_setting_ps.ac_drv_ohm = DDR_SOC_AC_DRV_60_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_60_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                        //for pxp dram weak driver model
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_48_OHM,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_60_OHM,
		//.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_240_OHM,

		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_pull_up_calibration_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_voltage_range_setting =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		.cfg_board_SI_setting_ps.dfe_offset = 0x66u,
		.cfg_board_SI_setting_ps.vref_ac_permil = 0,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,                                                    //0,
		.cfg_board_SI_setting_ps.training_phase_parameter = { 0						},
		.cfg_board_SI_setting_ps.ac_trace_delay_org = {
			128, 128, 128, 128, 128, 128, 128, 128,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384,
		},                                             //total 36
		.cfg_ddr_training_delay_ps.ac_trace_delay = {
			128, 128, 128, 128, 128, 128, 128, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384,
		},

		.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		//cs0 write dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		//cs1 write dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		//cs0 read dqs,lane0-lane3
		(1 << 7) | 0xb, (1 << 7) | 0xb, (1 << 7) | 0xb, (1 << 7) | 0xb,
		//cs1 read dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		},

		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0x000002a8,          // 680
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0x000002bc,         // 700
		.cfg_board_SI_setting_ps.ac_trace_delay_org[0] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[1] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[2] = 0x00000058,         // 88
		.cfg_board_SI_setting_ps.ac_trace_delay_org[3] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[4] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[5] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[6] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[7] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[8] = 0x00000180,         // 384
		.cfg_board_SI_setting_ps.ac_trace_delay_org[9] = 0x00000180,         // 384
		.cfg_board_SI_setting_ps.ac_trace_delay_org[10] = 0x00000100,        // 256
		.cfg_board_SI_setting_ps.ac_trace_delay_org[11] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[12] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[13] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[14] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[15] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[16] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[17] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[18] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[19] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[20] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[21] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[22] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[23] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[24] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[25] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[26] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[27] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[28] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[29] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[30] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[31] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[32] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[33] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[34] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[35] = 0x00000000,        // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 0x00000080,                   // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 0x00000080,                   // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 0x00000080 - 100,    // 88
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 0x00000080,                   // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 0x00000180 - 40,      // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 0x00000180 - 40,      // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 0x00000180 - 40,      // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[30] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[31] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[32] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[33] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[34] = 0x00000180 - 40,     // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[35] = 0x00000180 - 40,     // 384

		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x000000ff,// 255
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x000000f5,// 245
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x000000e1,// 225
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0x000000d7,// 215
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x00000102,// 258
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x00000102,// 258
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x00000102,// 258
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x00000102,// 258

		.cfg_ddr_training_delay_ps.write_dq_bit_delay[0] = 0x00000151,// 337
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[1] = 0x00000134,// 308
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[2] = 0x00000153,// 339
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[3] = 0x0000012f,// 303
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[4] = 0x0000014c,// 332
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[5] = 0x00000137,// 311
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[6] = 0x00000145,// 325
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[7] = 0x00000148,// 328
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[8] = 0x00000140,// 320
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[9] = 0x0000013c,// 316
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[10] = 0x0000012e,// 302
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[11] = 0x0000013e,// 318
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[12] = 0x0000012a,// 298
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[13] = 0x00000129,// 297
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[14] = 0x0000012b,// 299
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[15] = 0x00000136,// 310
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[16] = 0x00000130,// 304
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[17] = 0x00000132,// 306
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[18] = 0x00000127,// 295
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[19] = 0x00000116,// 278
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[20] = 0x0000012f,// 303
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[21] = 0x00000116,// 278
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[22] = 0x0000011e,// 286
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[23] = 0x00000106,// 262
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[24] = 0x0000011c,// 284
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[25] = 0x0000010f,// 271
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[26] = 0x00000119,// 281
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[27] = 0x00000124,// 292
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[28] = 0x0000010b,// 267
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[29] = 0x00000127,// 295
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[30] = 0x00000116,// 278
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[31] = 0x0000011a,// 282
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[32] = 0x00000102,// 258
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[33] = 0x00000113,// 275
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[34] = 0x00000105,// 261
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[35] = 0x00000116,// 278
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
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0x00000211,// 529
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0x00000258,// 600
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0x00000237,// 567
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0x0000023d,// 573
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x00000084,// 132
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x0000007a,// 122
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x0000007e,// 126
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x0000007e,// 126
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[0] = 0x00000055,// 85
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[1] = 0x00000034,// 52
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[2] = 0x00000057,// 87
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[3] = 0x00000033,// 51
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[4] = 0x00000051,// 81
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[5] = 0x00000033,// 51
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[6] = 0x0000004d,// 77
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[7] = 0x00000044,// 68
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[8] = 0x00000045,// 69
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[9] = 0x00000043,// 67
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[10] = 0x00000031,// 49
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[11] = 0x0000003d,// 61
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[12] = 0x0000002e,// 46
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[13] = 0x00000032,// 50
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[14] = 0x0000002e,// 46
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[15] = 0x00000046,// 70
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[16] = 0x00000032,// 50
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[17] = 0x00000036,// 54
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[18] = 0x00000047,// 71
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[19] = 0x00000031,// 49
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[20] = 0x00000046,// 70
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[21] = 0x0000002f,// 47
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[22] = 0x0000003e,// 62
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[23] = 0x0000002b,// 43
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[24] = 0x0000003f,// 63
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[25] = 0x0000002d,// 45
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[26] = 0x00000038,// 56
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[27] = 0x00000053,// 83
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[28] = 0x0000002e,// 46
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[29] = 0x00000053,// 83
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[30] = 0x00000037,// 55
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[31] = 0x00000048,// 72
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[32] = 0x0000002c,// 44
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[33] = 0x00000044,// 68
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[34] = 0x0000002c,// 44
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[35] = 0x0000003d,// 61
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
		.cfg_ddr_training_delay_ps.soc_bit_vref[0] = 0x0000000,              // 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[1] = 0x00000026,             // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[2] = 0x00000029,             // 41
		.cfg_ddr_training_delay_ps.soc_bit_vref[3] = 0x00000025,             // 37
		.cfg_ddr_training_delay_ps.soc_bit_vref[4] = 0x00000024,             // 36
		.cfg_ddr_training_delay_ps.soc_bit_vref[5] = 0x0000002b,             // 43
		.cfg_ddr_training_delay_ps.soc_bit_vref[6] = 0x00000027,             // 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[7] = 0x00000027,             // 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[8] = 0x00000026,             // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[9] = 0x00000027,             // 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[10] = 0x00000022,            // 34
		.cfg_ddr_training_delay_ps.soc_bit_vref[11] = 0x00000023,            // 35
		.cfg_ddr_training_delay_ps.soc_bit_vref[12] = 0x00000023,            // 35
		.cfg_ddr_training_delay_ps.soc_bit_vref[13] = 0x00000023,            // 35
		.cfg_ddr_training_delay_ps.soc_bit_vref[14] = 0x00000023,            // 35
		.cfg_ddr_training_delay_ps.soc_bit_vref[15] = 0x00000024,            // 36
		.cfg_ddr_training_delay_ps.soc_bit_vref[16] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[17] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[18] = 0x00000027,            // 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[19] = 0x00000028,            // 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[20] = 0x00000025,            // 37
		.cfg_ddr_training_delay_ps.soc_bit_vref[21] = 0x00000024,            // 36
		.cfg_ddr_training_delay_ps.soc_bit_vref[22] = 0x00000024,            // 36
		.cfg_ddr_training_delay_ps.soc_bit_vref[23] = 0x00000025,            // 37
		.cfg_ddr_training_delay_ps.soc_bit_vref[24] = 0x0000002a,            // 42
		.cfg_ddr_training_delay_ps.soc_bit_vref[25] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[26] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[27] = 0x00000023,            // 35
		.cfg_ddr_training_delay_ps.soc_bit_vref[28] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[29] = 0x00000024,            // 36
		.cfg_ddr_training_delay_ps.soc_bit_vref[30] = 0x00000028,            // 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[31] = 0x00000027,            // 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[32] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[33] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[34] = 0x00000025,            // 37
		.cfg_ddr_training_delay_ps.soc_bit_vref[35] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[36] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[37] = 0x00000030,            // 48
		.cfg_ddr_training_delay_ps.soc_bit_vref[38] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[39] = 0x0000002f,            // 47
		.cfg_ddr_training_delay_ps.soc_bit_vref[40] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[41] = 0x0000002f,            // 47
		.cfg_ddr_training_delay_ps.soc_bit_vref[42] = 0x00000026,            // 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[43] = 0x0000002b,            // 43
		.cfg_ddr_training_delay_ps.dram_bit_vref[0] = 0x0,                   // 0
#if REF_BOARD_USE_4layer
	.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 0x00000080,// 128
	.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 0x00000080,// 128
	.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 0x0000008b,// 139
	.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 0x00000080,// 128
	.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 0x00000080,// 128
	.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 0x00000080,// 128
	.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 0x00000080,// 128
	.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[30] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[31] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[32] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[33] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[34] = 0x00000158,// 344
	.cfg_ddr_training_delay_ps.ac_trace_delay[35] = 0x00000158,// 344

	.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x000000cd,// 205
	.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x000000d2,// 210
	.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x000000c8,// 200
	.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0x000000c3,// 195
	.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x00000102,// 258
	.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x00000102,// 258
	.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x00000102,// 258
	.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x00000102,// 258

	.cfg_ddr_training_delay_ps.write_dq_bit_delay[0] = 0x00000119,// 281
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[1] = 0x00000106,// 262
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[2] = 0x00000114,// 276
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[3] = 0x00000100,// 256
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[4] = 0x00000115,// 277
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[5] = 0x00000107,// 263
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[6] = 0x0000010e,// 270
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[7] = 0x00000109,// 265
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[8] = 0x0000010c,// 268
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[9] = 0x00000119,// 281
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[10] = 0x0000010a,// 266
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[11] = 0x00000119,// 281
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[12] = 0x000000fe,// 254
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[13] = 0x00000111,// 273
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[14] = 0x000000f8,// 248
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[15] = 0x000000ff,// 255
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[16] = 0x0000010f,// 271
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[17] = 0x00000108,// 264
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[18] = 0x00000103,// 259
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[19] = 0x000000f1,// 241
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[20] = 0x00000111,// 273
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[21] = 0x000000f5,// 245
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[22] = 0x000000f7,// 247
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[23] = 0x000000e7,// 231
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[24] = 0x000000f6,// 246
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[25] = 0x000000ea,// 234
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[26] = 0x000000fc,// 252
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[27] = 0x0000010e,// 270
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[28] = 0x000000f0,// 240
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[29] = 0x00000114,// 276
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[30] = 0x00000101,// 257
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[31] = 0x00000109,// 265
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[32] = 0x000000f4,// 244
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[33] = 0x00000101,// 257
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[34] = 0x000000fb,// 251
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[35] = 0x00000100,// 256
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

	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0x000001e4,// 484
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0x000001e6,// 486
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0x000001e0,// 480
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0x000001e7,// 487
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 0x00000000,// 0

	.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x00000084,// 132
	.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x0000008a,// 138
	.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x0000008c,// 140
	.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x00000083,// 131
	.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x00000000,// 0
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[0] = 0x0000005a,// 90
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[1] = 0x00000034,// 52
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[2] = 0x00000049,// 73
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[3] = 0x00000038,// 56
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[4] = 0x0000004b,// 75
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[5] = 0x00000035,// 53
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[6] = 0x0000004f,// 79
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[7] = 0x00000040,// 64
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[8] = 0x00000043,// 67
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[9] = 0x00000061,// 97
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[10] = 0x0000004a,// 74
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[11] = 0x00000052,// 82
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[12] = 0x00000037,// 55
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[13] = 0x0000004d,// 77
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[14] = 0x00000035,// 53
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[15] = 0x00000043,// 67
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[16] = 0x0000004e,// 78
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[17] = 0x00000048,// 72
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[18] = 0x00000051,// 81
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[19] = 0x00000040,// 64
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[20] = 0x00000064,// 100
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[21] = 0x0000003c,// 60
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[22] = 0x0000004b,// 75
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[23] = 0x00000037,// 55
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[24] = 0x00000050,// 80
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[25] = 0x00000036,// 54
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[26] = 0x00000047,// 71
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[27] = 0x00000059,// 89
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[28] = 0x00000037,// 55
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[29] = 0x00000058,// 88
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[30] = 0x00000047,// 71
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[31] = 0x00000053,// 83
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[32] = 0x0000003c,// 60
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[33] = 0x00000052,// 82
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[34] = 0x00000043,// 67
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[35] = 0x0000004a,// 74
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
	.cfg_ddr_training_delay_ps.soc_bit_vref[0] = 0x00000000,
	.cfg_ddr_training_delay_ps.dram_bit_vref[0] = 0x0,

	.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		//cs0 write dqs,lane0-lane3
		(0 << 7) | 0xa, (0 << 7) | 0, (0 << 7) | 0x4, (0 << 7) | 0x4,
		//cs1 write dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		//cs0 read dqs,lane0-lane3
		(1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10,
		//cs1 read dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
	},
#endif //end REF_BOARD_USE_4layer
#if REF_BOARD_USE_6layer
	.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 0x00000080,
	.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 0x00000080,
	.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 0x0000008b,
	.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 0x00000080,
	.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 0x00000080,
	.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 0x00000080,
	.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 0x00000080,
	.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 0x00000000,
	.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 0x00000000,
	.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 0x00000000,
	.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 0x00000000,
	.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[30] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[31] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[32] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[33] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[34] = 0x00000158,
	.cfg_ddr_training_delay_ps.ac_trace_delay[35] = 0x00000158,
	.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x0000009b,
	.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x0000009b,
	.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x00000096,
	.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0x00000091,
	.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x00000102,
	.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x00000102,
	.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x00000102,
	.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x00000102,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[0] = 0x000000e7,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[1] = 0x000000d9,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[2] = 0x000000e2,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[3] = 0x000000d8,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[4] = 0x000000db,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[5] = 0x000000d9,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[6] = 0x000000d9,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[7] = 0x000000d8,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[8] = 0x000000df,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[9] = 0x000000ea,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[10] = 0x000000df,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[11] = 0x000000e6,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[12] = 0x000000d5,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[13] = 0x000000e4,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[14] = 0x000000ce,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[15] = 0x000000d6,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[16] = 0x000000e1,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[17] = 0x000000db,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[18] = 0x000000d8,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[19] = 0x000000cd,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[20] = 0x000000e2,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[21] = 0x000000cd,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[22] = 0x000000d1,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[23] = 0x000000bd,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[24] = 0x000000ce,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[25] = 0x000000c1,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[26] = 0x000000ce,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[27] = 0x000000db,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[28] = 0x000000bd,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[29] = 0x000000e2,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[30] = 0x000000cf,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[31] = 0x000000dc,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[32] = 0x000000c5,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[33] = 0x000000d7,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[34] = 0x000000ca,
	.cfg_ddr_training_delay_ps.write_dq_bit_delay[35] = 0x000000cd,
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0x000001ac,
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0x000001b7,
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0x000001b5,
	.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0x000001b5,
	.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x00000085,
	.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x00000081,
	.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x00000091,
	.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x0000007c,
	.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x00000000,
	.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x00000000,
	.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x00000000,
	.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x00000000,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[0] = 0x0000004f,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[1] = 0x00000034,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[2] = 0x0000003e,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[3] = 0x00000034,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[4] = 0x00000042,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[5] = 0x00000033,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[6] = 0x00000042,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[7] = 0x0000003b,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[8] = 0x0000003c,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[9] = 0x00000051,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[10] = 0x00000042,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[11] = 0x0000004c,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[12] = 0x00000034,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[13] = 0x00000047,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[14] = 0x00000035,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[15] = 0x00000040,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[16] = 0x00000046,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[17] = 0x00000042,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[18] = 0x00000046,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[19] = 0x00000036,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[20] = 0x00000057,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[21] = 0x00000035,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[22] = 0x0000003e,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[23] = 0x00000034,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[24] = 0x00000042,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[25] = 0x00000034,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[26] = 0x0000003e,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[27] = 0x0000004f,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[28] = 0x00000035,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[29] = 0x00000050,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[30] = 0x0000003e,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[31] = 0x0000004f,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[32] = 0x00000039,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[33] = 0x0000004c,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[34] = 0x00000040,
	.cfg_ddr_training_delay_ps.read_dq_bit_delay[35] = 0x00000044,
	.cfg_ddr_training_delay_ps.soc_bit_vref[0] = 0x00000000,
	.cfg_ddr_training_delay_ps.dram_bit_vref[0] = 0x0,

	.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		//cs0 write dqs,lane0-lane3
		(0 << 7) | 0xa, (0 << 7) | 0, (0 << 7) | 0x4, (0 << 7) | 0x4,
		//cs1 write dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
		//cs0 read dqs,lane0-lane3
		(1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10, (1 << 7) | 0x10,
		//cs1 read dqs,lane0-lane3
		(0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0, (0 << 7) | 0,
	},
#endif //end REF_BOARD_USE_6layer
	},//end ch1
};
#endif  //ENABLE_REF_BOARD

#if ENABLE_SKT_BOARD    //timing_config,SKT AR319 6layer 4pcs ddr4
ddr_set_ps0_only_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {
	{//start ch0
		.cfg_board_common_setting.timming_magic = 0,
		//sizeof(__ddr_setting) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_max_valid_configs = 0,
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,                                                                           //0
		.cfg_board_common_setting.fast_boot = { 0					       },
		//.cfg_board_common_setting.ddr_func = DDR_FUNC_CONFIG_DFE_FUNCTION,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_DDR4,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
		.cfg_board_common_setting.DisabledDbyte = CONFIG_DISABLE_D32_D63,
		.cfg_board_common_setting.dram_cs0_base_add = 0,
		.cfg_board_common_setting.dram_cs1_base_add = 0,
		//.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,
		//.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
		.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		.cfg_board_common_setting.log_level = LOG_LEVEL_BASIC,
		//DDR_WRITE_READ_DBI_ENABLE,//DDR_WRITE_READ_DBI_DISABLE,
		.cfg_board_common_setting.ddr_rdbi_wr_enable = DDR_WRITE_READ_DBI_DISABLE,
		 //center_ssc_1000ppm,//SSC_DISABLE,(1 << 20) | (0 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.pll_ssc_mode = (1 << 20) | (1 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test_function = { 0						    },
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR4_32BIT,
#if T3_DDR_CODE_NOT_USE
		.cfg_board_common_setting.ac_pinmux = {
			0,  0,	0,  1,	2,  3,	8, 27,
			10, 2,	9,  21, 5,  14, 1, 0,
			25, 13, 12, 4,	7,  22, 0, 0,
			0,  0,	6,  3,	20, 0,	0, 15,
			26, 11,
		},
#endif
#if T3_DDR_CODE_NOT_USE
		//T3 sip ac pinmux
		.cfg_board_common_setting.ac_pinmux = {
			//sip
			0, 0,  0,  1, 2, 3,
			8, 27, 10, 2, 9, 21, 5,	23,
			1, 0, 25, 13, 12, 4, 7, 28,
			0, 0, 0, 0, 6, 3, 20, 0, 0,
			24, 26, 11, 0
		},
#endif

		.cfg_board_common_setting.ac_pinmux = {
			//test chip
			0, 0,  0,  1, 2,  3,
			3, 28, 20, 5, 10, 13, 11, 8,
			0, 12, 27, 9, 1, 6, 4, 25,
			0, 0, 0, 0, 2, 7, 23, 0, 0,
			24, 26, 21, 0
		},

		.cfg_board_common_setting.ddr_dqs_swap = 0,
		.cfg_board_common_setting.ddr_dq_remap = {
			0,  1,	2,  3,	4,  5,	6,  7,
			8,  9,	10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, 29, 30, 31,
			32, 33, 34, 35
		},                                                   //d0-d31 dm0 dm1 dm2 dm3

		.cfg_board_common_setting.ddr_vddee_setting = { 0					       },
		.cfg_board_SI_setting_ps.DRAMFreq = 1320,
		.cfg_board_SI_setting_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		//DDR_DRAM_ODT_W_CS0_ODT0,DDR_DRAM_ODT_DDR4_PARK_ENABLE
		.cfg_board_SI_setting_ps.ddr_odt_config = DDR_DRAM_ODT_DDR4_PARK_ENABLE,
		.cfg_board_SI_setting_ps.clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_48_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                        //for pxp dram weak driver model
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_48_OHM,
		//.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_240_OHM,

		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_pull_up_calibration_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_voltage_range_setting =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		.cfg_board_SI_setting_ps.dfe_offset = 0x66u,
		.cfg_board_SI_setting_ps.vref_ac_permil = 0,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,                                                    //0,
		.cfg_board_SI_setting_ps.training_phase_parameter = { 0						},
		.cfg_board_SI_setting_ps.ac_trace_delay_org = {
			128, 128, 128, 128, 128, 128, 128, 128,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384,
		},                                             //total 36
		.cfg_ddr_training_delay_ps.ac_trace_delay = {
			128, 128, 128, 128, 128, 128, 128, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384,
		},

		.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		//cs0 write dqs,lane0-lane3
		(0 << 7) | 04, (0 << 7) | 04, (0 << 7) | 04, (0 << 7) | 04,
		//cs1 write dqs,lane0-lane3
		(0 << 7) | 00, (0 << 7) | 00, (0 << 7) | 00, (0 << 7) | 00,
		//cs0 read dqs,lane0-lane3
		(1 << 7) | 14, (1 << 7) | 14, (1 << 7) | 14, (1 << 7) | 14,
		//cs1 read dqs,lane0-lane3
		(0 << 7) | 00, (0 << 7) | 00, (0 << 7) | 00, (0 << 7) | 00,
		},
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,//0x000002a8,// 680
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,//0x000002bc,// 700
		.cfg_board_SI_setting_ps.ac_trace_delay_org[0] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[1] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[2] = 0x00000058,         // 88
		.cfg_board_SI_setting_ps.ac_trace_delay_org[3] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[4] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[5] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[6] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[7] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[8] = 0x00000180,         // 384
		.cfg_board_SI_setting_ps.ac_trace_delay_org[9] = 0x00000180,         // 384
		.cfg_board_SI_setting_ps.ac_trace_delay_org[10] = 0x00000100,        // 256
		.cfg_board_SI_setting_ps.ac_trace_delay_org[11] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[12] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[13] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[14] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[15] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[16] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[17] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[18] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[19] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[20] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[21] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[22] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[23] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[24] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[25] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[26] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[27] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[28] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[29] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[30] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[31] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[32] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[33] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[34] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[35] = 0x00000000,        // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 0x00000080,           // 88
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 0x00000180,           // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 0x00000180,           // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 0x00000180,           // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[30] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[31] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[32] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[33] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[34] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[35] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x0000014f,// 335
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x0000013b,// 315
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x0000014f,// 335
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0x00000145,// 325
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x0000014b,// 331
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x00000137,// 311
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x0000014b,// 331
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x00000141,// 321
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[0] = 0x00000185,// 389
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[1] = 0x0000017d,// 381
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[2] = 0x0000018d,// 397
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[3] = 0x00000174,// 372
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[4] = 0x0000018b,// 395
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[5] = 0x0000018c,// 396
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[6] = 0x00000195,// 405
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[7] = 0x00000182,// 386
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[8] = 0x00000185,// 389
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[9] = 0x0000017c,// 380
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[10] = 0x00000174,// 372
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[11] = 0x0000017a,// 378
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[12] = 0x0000016d,// 365
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[13] = 0x0000017b,// 379
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[14] = 0x00000181,// 385
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[15] = 0x0000017b,// 379
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[16] = 0x00000170,// 368
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[17] = 0x00000177,// 375
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[18] = 0x0000018a,// 394
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[19] = 0x00000179,// 377
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[20] = 0x0000018f,// 399
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[21] = 0x0000017f,// 383
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[22] = 0x00000184,// 388
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[23] = 0x00000173,// 371
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[24] = 0x00000191,// 401
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[25] = 0x00000188,// 392
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[26] = 0x00000182,// 386
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[27] = 0x0000018d,// 397
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[28] = 0x00000180,// 384
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[29] = 0x00000192,// 402
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[30] = 0x00000176,// 374
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[31] = 0x00000196,// 406
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[32] = 0x0000017b,// 379
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[33] = 0x0000018e,// 398
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[34] = 0x00000179,// 377
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[35] = 0x00000187,// 391
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
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0x0000011f,// 287
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0x00000120,// 288
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0x00000112,// 274
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0x0000011a,// 282
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 0x0000011f,// 287
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 0x00000120,// 288
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 0x00000112,// 274
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 0x0000011a,// 282
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x0000007b,// 123
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x00000075,// 117
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x00000078,// 120
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x00000070,// 112
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x00000093,// 147
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x0000008d,// 141
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x00000090,// 144
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x00000088,// 136
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[0] = 0x00000045,// 69
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[1] = 0x00000036,// 54
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[2] = 0x0000004a,// 74
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[3] = 0x00000036,// 54
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[4] = 0x0000004d,// 77
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[5] = 0x00000053,// 83
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[6] = 0x0000005e,// 94
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[7] = 0x00000038,// 56
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[8] = 0x00000046,// 70
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[9] = 0x0000004e,// 78
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[10] = 0x00000034,// 52
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[11] = 0x00000043,// 67
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[12] = 0x00000036,// 54
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[13] = 0x00000049,// 73
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[14] = 0x0000004c,// 76
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[15] = 0x0000004b,// 75
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[16] = 0x0000003c,// 60
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[17] = 0x00000042,// 66
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[18] = 0x0000003c,// 60
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[19] = 0x00000030,// 48
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[20] = 0x00000042,// 66
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[21] = 0x00000037,// 55
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[22] = 0x0000003f,// 63
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[23] = 0x00000031,// 49
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[24] = 0x00000057,// 87
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[25] = 0x00000048,// 72
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[26] = 0x0000003e,// 62
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[27] = 0x0000004c,// 76
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[28] = 0x00000038,// 56
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[29] = 0x0000004c,// 76
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[30] = 0x0000002e,// 46
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[31] = 0x00000057,// 87
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[32] = 0x0000003c,// 60
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[33] = 0x00000057,// 87
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[34] = 0x00000039,// 57
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[35] = 0x00000044,// 68
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
		.cfg_ddr_training_delay_ps.soc_bit_vref[0] = 0x0000000,              // 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[1] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[2] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps.soc_bit_vref[3] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[4] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[5] = 0x00000025,// 37
		.cfg_ddr_training_delay_ps.soc_bit_vref[6] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps.soc_bit_vref[7] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps.soc_bit_vref[8] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[9] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[10] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps.soc_bit_vref[11] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps.soc_bit_vref[12] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps.soc_bit_vref[13] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[14] = 0x00000026,// 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[15] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[16] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps.soc_bit_vref[17] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[18] = 0x0000002b,// 43
		.cfg_ddr_training_delay_ps.soc_bit_vref[19] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps.soc_bit_vref[20] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[21] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[22] = 0x0000002b,// 43
		.cfg_ddr_training_delay_ps.soc_bit_vref[23] = 0x0000002b,// 43
		.cfg_ddr_training_delay_ps.soc_bit_vref[24] = 0x00000026,// 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[25] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps.soc_bit_vref[26] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[27] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[28] = 0x00000025,// 37
		.cfg_ddr_training_delay_ps.soc_bit_vref[29] = 0x0000002c,// 44
		.cfg_ddr_training_delay_ps.soc_bit_vref[30] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps.soc_bit_vref[31] = 0x00000026,// 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[32] = 0x00000026,// 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[33] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[34] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps.soc_bit_vref[35] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[36] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[37] = 0x00000033,// 51
		.cfg_ddr_training_delay_ps.soc_bit_vref[38] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[39] = 0x0000002f,// 47
		.cfg_ddr_training_delay_ps.soc_bit_vref[40] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[41] = 0x00000033,// 51
		.cfg_ddr_training_delay_ps.soc_bit_vref[42] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[43] = 0x0000002f,// 47
		.cfg_ddr_training_delay_ps.dram_bit_vref[0] = 0x0,// 0
	},//end ch0
	{//start ch1
		.cfg_board_common_setting.timming_magic = 0,
		//sizeof(__ddr_setting) / sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_max_valid_configs = 0,
		.cfg_board_common_setting.timming_struct_version = 0,
		.cfg_board_common_setting.timming_struct_org_size = sizeof(ddr_set_ps0_only_t),
		.cfg_board_common_setting.timming_struct_real_size = 0,                                                                           //0
		.cfg_board_common_setting.fast_boot = { 0					       },
		//.cfg_board_common_setting.ddr_func = DDR_FUNC_CONFIG_DFE_FUNCTION,
		.cfg_board_common_setting.board_id = CONFIG_BOARD_ID_MASK,
		.cfg_board_common_setting.DramType = CONFIG_DDR_TYPE_DDR4,
		.cfg_board_common_setting.dram_rank_config = CONFIG_DDR0_32BIT_RANK0_CH0,
		.cfg_board_common_setting.DisabledDbyte = CONFIG_DISABLE_D32_D63,
		.cfg_board_common_setting.dram_cs0_base_add = 0,
		.cfg_board_common_setting.dram_cs1_base_add = 0,
		//.cfg_board_common_setting.dram_cs0_size_MB				= CONFIG_DDR0_SIZE_1024MB,
		//.cfg_board_common_setting.dram_cs1_size_MB				= CONFIG_DDR1_SIZE_0MB,
		.cfg_board_common_setting.dram_ch0_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_ch1_size_MB =
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX2 << CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET) +
			(DRAM_SIZE_ID_256MBX0 << CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET),
		.cfg_board_common_setting.dram_x4x8x16_mode = CONFIG_DRAM_MODE_X16,
		.cfg_board_common_setting.Is2Ttiming = CONFIG_USE_DDR_2T_MODE,
		.cfg_board_common_setting.log_level = LOG_LEVEL_BASIC,
		//DDR_WRITE_READ_DBI_ENABLE,//DDR_WRITE_READ_DBI_DISABLE,
		.cfg_board_common_setting.ddr_rdbi_wr_enable = DDR_WRITE_READ_DBI_DISABLE,
		 //center_ssc_1000ppm,//SSC_DISABLE,(1 << 20) | (0 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.pll_ssc_mode = (1 << 20) | (1 << 8) | (2 << 4) | 0,
		.cfg_board_common_setting.org_tdqs2dq = 0,
		.cfg_board_common_setting.reserve1_test_function = { 0						    },
		.cfg_board_common_setting.ddr_dmc_remap = DDR_DMC_REMAP_DDR4_32BIT,

#if T3_DDR_CODE_NOT_USE
		//T3 sip ac pinmux
		.cfg_board_common_setting.ac_pinmux = {
			//sip
			0, 0,  0,  1, 2, 3,
			8, 27, 10, 2, 9, 21, 5,	23,
			1, 0, 25, 13, 12, 4, 7, 28,
			0, 0, 0, 0, 6, 3, 20, 0, 0,
			24, 26, 11, 0
		},
#endif

		.cfg_board_common_setting.ac_pinmux = {
			//test chip
			0, 0,  0, 1,  2, 3,
			5, 28, 0, 26, 1, 20, 3,	10,
			9, 13, 8, 25, 27, 2, 12, 6,
			0, 0, 0, 0, 4, 7, 11, 0, 0,
			24, 23, 21, 0
		},

		.cfg_board_common_setting.ddr_dqs_swap = 0,
		.cfg_board_common_setting.ddr_dq_remap = {
			0,  1,	2,  3,	4,  5,	6,  7,
			8,  9,	10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, 29, 30, 31,
			32, 33, 34, 35
		},                                                   //d0-d31 dm0 dm1 dm2 dm3

		.cfg_board_common_setting.ddr_vddee_setting = { 0					       },
		.cfg_board_SI_setting_ps.DRAMFreq = 1320,
		.cfg_board_SI_setting_ps.PllBypassEn = 0,
		.cfg_board_SI_setting_ps.training_SequenceCtrl = 0,
		//DDR_DRAM_ODT_W_CS0_ODT0,DDR_DRAM_ODT_DDR4_PARK_ENABLE
		.cfg_board_SI_setting_ps.ddr_odt_config = DDR_DRAM_ODT_DDR4_PARK_ENABLE,
		.cfg_board_SI_setting_ps.clk_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.cs_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.ac_drv_ohm = DDR_SOC_AC_DRV_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_p = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_drv_ohm_n = DDR_SOC_DATA_DRV_ODT_40_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_p = DDR_SOC_DATA_DRV_ODT_48_OHM,
		.cfg_board_SI_setting_ps.soc_data_odt_ohm_n = DDR_SOC_DATA_DRV_ODT_0_OHM,                                                        //for pxp dram weak driver model
		.cfg_board_SI_setting_ps.dram_data_drv_ohm = DDR_DRAM_DDR4_DRV_34_OHM,
		.cfg_board_SI_setting_ps.dram_data_odt_ohm = DDR_DRAM_DDR4_ODT_48_OHM,
		//.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm = DDR_DRAM_DDR4_WR_ODT_240_OHM,

		.cfg_board_SI_setting_ps.dram_ac_odt_ohm = DDR_DRAM_DDR_AC_ODT_0_OHM,
		.cfg_board_SI_setting_ps.dram_data_drv_pull_up_calibration_ohm =
			DDR_DRAM_LPDDR4_ODT_40_OHM,
		.cfg_board_SI_setting_ps.lpddr4_dram_vout_voltage_range_setting =
			DDR_DRAM_LPDDR4_OUTPUT_1_3_VDDQ,
		.cfg_board_SI_setting_ps.dfe_offset = 0x66u,
		.cfg_board_SI_setting_ps.vref_ac_permil = 0,
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,
		.cfg_board_SI_setting_ps.max_core_timmming_frequency = 0,                                                    //0,
		.cfg_board_SI_setting_ps.training_phase_parameter = { 0						},
		.cfg_board_SI_setting_ps.ac_trace_delay_org = {
			128, 128, 128, 128, 128, 128, 128, 128,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384,
		},                                             //total 36
		.cfg_ddr_training_delay_ps.ac_trace_delay = {
			128, 128, 128, 128, 128, 128, 128, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384, 384, 384, 384, 384,
			384, 384, 384, 384,
		},

		.cfg_ddr_training_delay_ps.reserve_training_parameter = {
		//cs0 write dqs,lane0-lane3
		(0 << 7) | 14, (0 << 7) | 04, (0 << 7) | 04, (0 << 7) | 04,
		//cs1 write dqs,lane0-lane3
		(0 << 7) | 00, (0 << 7) | 00, (0 << 7) | 00, (0 << 7) | 00,
		//cs0 read dqs,lane0-lane3
		(1 << 7) | 14, (1 << 7) | 14, (1 << 7) | 14, (1 << 7) | 14,
		//cs1 read dqs,lane0-lane3
		(0 << 7) | 00, (0 << 7) | 00, (0 << 7) | 00, (0 << 7) | 00,
		},
		.cfg_board_SI_setting_ps.vref_soc_data_permil = 0,//0x000002a8,// 680
		.cfg_board_SI_setting_ps.vref_dram_data_permil = 0,//0x000002bc,// 700
		.cfg_board_SI_setting_ps.ac_trace_delay_org[0] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[1] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[2] = 0x00000058,         // 88
		.cfg_board_SI_setting_ps.ac_trace_delay_org[3] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[4] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[5] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[6] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[7] = 0x00000080,         // 128
		.cfg_board_SI_setting_ps.ac_trace_delay_org[8] = 0x00000180,         // 384
		.cfg_board_SI_setting_ps.ac_trace_delay_org[9] = 0x00000180,         // 384
		.cfg_board_SI_setting_ps.ac_trace_delay_org[10] = 0x00000100,        // 256
		.cfg_board_SI_setting_ps.ac_trace_delay_org[11] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[12] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[13] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[14] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[15] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[16] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[17] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[18] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[19] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[20] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[21] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[22] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[23] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[24] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[25] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[26] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[27] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[28] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[29] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[30] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[31] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[32] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[33] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[34] = 0x00000000,        // 0
		.cfg_board_SI_setting_ps.ac_trace_delay_org[35] = 0x00000000,        // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[0] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[1] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[2] = 0x00000080,           // 88
		.cfg_ddr_training_delay_ps.ac_trace_delay[3] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[4] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[5] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[6] = 0x00000080,           // 128
		.cfg_ddr_training_delay_ps.ac_trace_delay[7] = 0x00000180,           // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[8] = 0x00000180,           // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[9] = 0x00000180,           // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[10] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[11] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[12] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[13] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[14] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[15] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[16] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[17] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[18] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[19] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[20] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[21] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[22] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[23] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[24] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[25] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[26] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[27] = 0x00000000,          // 0
		.cfg_ddr_training_delay_ps.ac_trace_delay[28] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[29] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[30] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[31] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[32] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[33] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[34] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.ac_trace_delay[35] = 0x00000180,          // 384
		.cfg_ddr_training_delay_ps.write_dqs_delay[0] = 0x00000131,// 305
		.cfg_ddr_training_delay_ps.write_dqs_delay[1] = 0x00000131,// 305
		.cfg_ddr_training_delay_ps.write_dqs_delay[2] = 0x0000011d,// 285
		.cfg_ddr_training_delay_ps.write_dqs_delay[3] = 0x0000011d,// 285
		.cfg_ddr_training_delay_ps.write_dqs_delay[4] = 0x00000102,          // 258
		.cfg_ddr_training_delay_ps.write_dqs_delay[5] = 0x00000102,          // 258
		.cfg_ddr_training_delay_ps.write_dqs_delay[6] = 0x00000102,          // 258
		.cfg_ddr_training_delay_ps.write_dqs_delay[7] = 0x00000102,          // 258
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[0] = 0x00000181,// 385
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[1] = 0x00000165,// 357
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[2] = 0x00000184,// 388
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[3] = 0x0000015a,// 346
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[4] = 0x00000178,// 376
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[5] = 0x00000164,// 356
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[6] = 0x00000171,// 369
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[7] = 0x00000174,// 372
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[8] = 0x0000016e,// 366
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[9] = 0x00000174,// 372
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[10] = 0x0000015f,// 351
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[11] = 0x00000171,// 369
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[12] = 0x0000016b,// 363
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[13] = 0x0000016f,// 367
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[14] = 0x00000162,// 354
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[15] = 0x00000173,// 371
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[16] = 0x00000165,// 357
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[17] = 0x0000016a,// 362
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[18] = 0x0000015f,// 351
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[19] = 0x00000147,// 327
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[20] = 0x0000016b,// 363
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[21] = 0x00000147,// 327
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[22] = 0x00000150,// 336
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[23] = 0x0000013a,// 314
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[24] = 0x00000150,// 336
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[25] = 0x00000143,// 323
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[26] = 0x00000153,// 339
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[27] = 0x00000167,// 359
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[28] = 0x00000136,// 310
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[29] = 0x0000016c,// 364
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[30] = 0x00000154,// 340
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[31] = 0x00000162,// 354
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[32] = 0x00000147,// 327
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[33] = 0x00000158,// 344
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[34] = 0x00000148,// 328
		.cfg_ddr_training_delay_ps.write_dq_bit_delay[35] = 0x0000014d,// 333
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
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[0] = 0x0000010d,// 269
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[1] = 0x00000113,// 275
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[2] = 0x00000117,// 279
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[3] = 0x00000115,// 277
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[4] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[5] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[6] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_gate_delay[7] = 0x00000000,      // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[0] = 0x00000076,// 118
		.cfg_ddr_training_delay_ps.read_dqs_delay[1] = 0x00000076,// 118
		.cfg_ddr_training_delay_ps.read_dqs_delay[2] = 0x0000007c,// 124
		.cfg_ddr_training_delay_ps.read_dqs_delay[3] = 0x00000080,// 128
		.cfg_ddr_training_delay_ps.read_dqs_delay[4] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[5] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[6] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dqs_delay[7] = 0x00000000,           // 0
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[0] = 0x00000064,// 100
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[1] = 0x00000033,// 51
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[2] = 0x0000005d,// 93
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[3] = 0x00000030,// 48
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[4] = 0x0000005a,// 90
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[5] = 0x0000002f,// 47
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[6] = 0x00000057,// 87
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[7] = 0x00000055,// 85
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[8] = 0x0000004b,// 75
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[9] = 0x00000047,// 71
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[10] = 0x00000030,// 48
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[11] = 0x00000043,// 67
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[12] = 0x0000003b,// 59
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[13] = 0x0000003b,// 59
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[14] = 0x00000038,// 56
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[15] = 0x0000004f,// 79
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[16] = 0x00000033,// 51
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[17] = 0x0000003d,// 61
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[18] = 0x00000042,// 66
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[19] = 0x0000002f,// 47
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[20] = 0x00000055,// 85
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[21] = 0x00000030,// 48
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[22] = 0x0000003a,// 58
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[23] = 0x0000002d,// 45
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[24] = 0x0000003f,// 63
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[25] = 0x00000030,// 48
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[26] = 0x00000039,// 57
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[27] = 0x0000005f,// 95
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[28] = 0x00000031,// 49
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[29] = 0x00000061,// 97
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[30] = 0x00000041,// 65
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[31] = 0x00000057,// 87
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[32] = 0x00000039,// 57
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[33] = 0x00000056,// 86
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[34] = 0x00000043,// 67
		.cfg_ddr_training_delay_ps.read_dq_bit_delay[35] = 0x0000004b,// 75
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
		.cfg_ddr_training_delay_ps.soc_bit_vref[0] = 0x0000000,              // 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[1] = 0x0000002b,// 43
		.cfg_ddr_training_delay_ps.soc_bit_vref[2] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps.soc_bit_vref[3] = 0x0000002c,// 44
		.cfg_ddr_training_delay_ps.soc_bit_vref[4] = 0x00000024,// 36
		.cfg_ddr_training_delay_ps.soc_bit_vref[5] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps.soc_bit_vref[6] = 0x00000024,// 36
		.cfg_ddr_training_delay_ps.soc_bit_vref[7] = 0x00000023,// 35
		.cfg_ddr_training_delay_ps.soc_bit_vref[8] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[9] = 0x00000025,// 37
		.cfg_ddr_training_delay_ps.soc_bit_vref[10] = 0x0000002c,// 44
		.cfg_ddr_training_delay_ps.soc_bit_vref[11] = 0x00000025,// 37
		.cfg_ddr_training_delay_ps.soc_bit_vref[12] = 0x00000026,// 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[13] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[14] = 0x00000026,// 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[15] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[16] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps.soc_bit_vref[17] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[18] = 0x00000029,// 41
		.cfg_ddr_training_delay_ps.soc_bit_vref[19] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps.soc_bit_vref[20] = 0x00000026,// 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[21] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[22] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps.soc_bit_vref[23] = 0x0000002d,// 45
		.cfg_ddr_training_delay_ps.soc_bit_vref[24] = 0x00000026,// 38
		.cfg_ddr_training_delay_ps.soc_bit_vref[25] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[26] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[27] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[28] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps.soc_bit_vref[29] = 0x0000002a,// 42
		.cfg_ddr_training_delay_ps.soc_bit_vref[30] = 0x00000025,// 37
		.cfg_ddr_training_delay_ps.soc_bit_vref[31] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[32] = 0x00000027,// 39
		.cfg_ddr_training_delay_ps.soc_bit_vref[33] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[34] = 0x00000024,// 36
		.cfg_ddr_training_delay_ps.soc_bit_vref[35] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[36] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[37] = 0x0000002e,// 46
		.cfg_ddr_training_delay_ps.soc_bit_vref[38] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[39] = 0x0000002d,// 45
		.cfg_ddr_training_delay_ps.soc_bit_vref[40] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[41] = 0x00000031,// 49
		.cfg_ddr_training_delay_ps.soc_bit_vref[42] = 0x00000028,// 40
		.cfg_ddr_training_delay_ps.soc_bit_vref[43] = 0x0000002f,// 47
		.cfg_ddr_training_delay_ps.dram_bit_vref[0] = 0x0,// 0
	},//end ch1
};
#endif  //ENABLE_SKT_BOARD