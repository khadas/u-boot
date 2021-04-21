/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#define CONFIG_BOARD_ID_MASK				0xFF
#define CONFIG_DDR_TYPE_DDR3				0
#define CONFIG_DDR_TYPE_DDR4				1
#define CONFIG_DDR_TYPE_LPDDR4				2
#define CONFIG_DDR_TYPE_LPDDR3				3
#define CONFIG_DDR_TYPE_LPDDR2				4
#define CONFIG_DDR_TYPE_AUTO				0xf

/* ddr channel defines */
#define CONFIG_DDR0_16BIT					1
#define CONFIG_DDR0_RANK0					2
#define CONFIG_DDR0_RANK01					3
#define CONFIG_DDR0_16BIT_2					4
/* CONFIG_DDR_CHL_AUTO mode support RANK0 and RANK0+1 mode auto detect */
#define CONFIG_DDR_CHL_AUTO					0xF
#define CONFIG_DDR0_16BIT_CH0				0x1
#define CONFIG_DDR0_16BIT_RANK01_CH0		0x4
#define CONFIG_DDR0_32BIT_RANK0_CH0			0x2
#define CONFIG_DDR0_32BIT_RANK01_CH01		0x3
#define CONFIG_DDR0_32BIT_16BIT_RANK0_CH0	0x5
#define CONFIG_DDR0_32BIT_16BIT_RANK01_CH0	0x6
#define CONFIG_DDR0_32BIT_RANK01_CH0		0x7
#define CONFIG_DDR0_32BIT_RANK0_CH01		0x8

//CONFIG_CH0_CS0_BYTE_01_SIZE_256 0 mean disable ,1 mean 256M 2 mean 512M 3 mean 768M,...0xf mean auto size det
#define CONFIG_CH0_CS0_BYTE_01_SIZE_256_ID_OFFSET                 0
#define CONFIG_CH0_CS0_BYTE_23_SIZE_256_ID_OFFSET                 4
#define CONFIG_CH0_CS1_BYTE_01_SIZE_256_ID_OFFSET                 8
#define CONFIG_CH0_CS1_BYTE_23_SIZE_256_ID_OFFSET                 12
#define CONFIG_CH1_CS0_BYTE_01_SIZE_256_ID_OFFSET                 16
#define CONFIG_CH1_CS0_BYTE_23_SIZE_256_ID_OFFSET                 20
#define CONFIG_CH1_CS1_BYTE_01_SIZE_256_ID_OFFSET                 24
#define CONFIG_CH1_CS1_BYTE_23_SIZE_256_ID_OFFSET                 28
#define CONFIG_CS0_BYTE_01_SIZE_256_ID_OFFSET                     0
#define CONFIG_CS0_BYTE_23_SIZE_256_ID_OFFSET                     4
#define CONFIG_CS1_BYTE_01_SIZE_256_ID_OFFSET                     8
#define CONFIG_CS1_BYTE_23_SIZE_256_ID_OFFSET                     12

#define DRAM_SIZE_ID_256MBX0                                      0
#define DRAM_SIZE_ID_256MBX1                                      1
#define DRAM_SIZE_ID_256MBX2                                      2
#define DRAM_SIZE_ID_256MBX3                                      3
#define DRAM_SIZE_ID_256MBX4                                      4
#define DRAM_SIZE_ID_256MBX5                                      5
#define DRAM_SIZE_ID_256MBX6                                      6
#define DRAM_SIZE_ID_256MBX7                                      7
#define DRAM_SIZE_ID_256MBX8                                      8
#define DRAM_SIZE_ID_256MBX9                                      9
#define DRAM_SIZE_ID_256MBX10                                     10
#define DRAM_SIZE_ID_256MBX11                                     11
#define DRAM_SIZE_ID_256MBX12                                     12
#define DRAM_SIZE_ID_256MBX13                                     13
#define DRAM_SIZE_ID_256MBX14                                     14
#define DRAM_SIZE_ID_256MBXAUTO                                   15

#define CFG_DDR_BASE_ADDR					0X0
#define CFG_DDR_START_OFFSET				0X00000000 //TXLX SKIP 0MB

/* ddr type identifier */
#define CONFIG_DDR_TIMMING_LPDDR2			0x02
#define CONFIG_DDR_TIMMING_LPDDR3			0x03
#define CONFIG_DDR_TIMMING_DDR3_7			0x07
#define CONFIG_DDR_TIMMING_DDR3_9			0x09
#define CONFIG_DDR_TIMMING_DDR3_11			0x0B
#define CONFIG_DDR_TIMMING_DDR3_12			0x0C
#define CONFIG_DDR_TIMMING_DDR3_13			0x0D
#define CONFIG_DDR_TIMMING_DDR3_14			0x0E

#define CONFIG_DDR_TIMMING_DDR4_1600		0x0F
#define CONFIG_DDR_TIMMING_DDR4_1866		0x10
#define CONFIG_DDR_TIMMING_DDR4_2133		0x11
#define CONFIG_DDR_TIMMING_DDR4_2400		0x12
#define CONFIG_DDR_TIMMING_DDR4_2666		0x13
#define CONFIG_DDR_TIMMING_DDR4_3200		0x14

#define CONFIG_DDR_FUNC_TEST				(1<<0)

#define CONFIG_DDR_INIT_RETRY_TOTAL			(10)
#define CONFIG_DDR_PCTL_RETRY_TOTAL			(100)

#define DDR_USE_1_RANK(chl_set)	((chl_set == CONFIG_DDR0_RANK0) || \
				(chl_set == CONFIG_DDR0_16BIT))
#define DDR_USE_2_RANK(chl_set)	((chl_set == CONFIG_DDR0_RANK01))

#define DMC_TEST_SLT_ENABLE_DDR_AUTO_FAST_BOOT 1<<5
#define DMC_TEST_SLT_ENABLE_DDR_AUTO_WINDOW_TEST 1<<4
/* DMC_DDR_CTRL defines */
#define DDR_DDR4_ENABLE						(1<<22)
#define DDR_RANK1_ENABLE					(1<<21)
#define DDR_DDR4_BG_ENABLE					(1<<20)
#define DDR_16BIT_ENABLE					(1<<16)

#define DDR_RANK1_SIZE_CTRL					(3)
#define DDR_RANK0_SIZE_CTRL					(0)

/* ddr functions */
/*
#define DDR_FUNC_D2PLL						(1<<0)
#define DDR_FUNC_LP							(1<<1)
#define DDR_FUNC_ZQ_PD						(1<<2)
#define DDR_FUNC_EXT_VREF					(1<<3)
#define DDR_FUNC_DDR4_TIMING_TEST			(1<<4)
#define DDR_FUNC_DDR_PLL_BYPASS				(1<<5)
#define DDR_FUNC_RDBI						(1<<6)
#define DDR_FUNC_LPDDR3_CA					(1<<7)
#define DDR_FUNC_PRINT_WINDOW				(1<<8)
#define DDR_FUNC_FULL_TEST					(1<<10)
#define DDR_FUNC_NONSEC_SCRAMBLE			(1<<11)
#define DDR_FUNC_LPDDR3_CA_CA0_BIT0			(1<<20)
#define DDR_FUNC_LPDDR3_CA_CA0_BIT1			(1<<21)
#define DDR_FUNC_LPDDR3_CA_CA1_BIT0			(1<<22)
#define DDR_FUNC_LPDDR3_CA_CA1_BIT1			(1<<23)
#define DDR_FUNC_LPDDR3_CA_CA0_OFFSET		(20)
#define DDR_FUNC_LPDDR3_CA_CA1_OFFSET		(22)
#define DDR_FUNC_LPDDR3_SOC_ODT_ONLY_UP		(1<<25)
#define DDR_FUNC							(DDR_FUNC_D2PLL					| \
											DDR_FUNC_LP						| \
											DDR_FUNC_ZQ_PD					| \
											DDR_FUNC_EXT_VREF				| \
											DDR_FUNC_DDR4_TIMING_TEST		| \
											DDR_FUNC_DDR_PLL_BYPASS			| \
											DDR_FUNC_RDBI					| \
											DDR_FUNC_LPDDR3_CA				| \
											DDR_FUNC_PRINT_WINDOW			| \
											DDR_FULL_TEST					| \
											DDR_NONSEC_SCRAMBLE				| \
											DDR_FUNC_LPDDR3_CA_TRAINING_CA0_BIT0| \
											DDR_FUNC_LPDDR3_CA_TRAINING_CA0_BIT1| \
											DDR_FUNC_LPDDR3_CA_TRAINING_CA1_BIT0| \
											DDR_FUNC_LPDDR3_CA_TRAINING_CA1_BIT1| \
											DDR_FUNC_LPDDR3_SOC_ODT_ONLY_UP	| \
											(1 << 31) 						\
											)
*/
#define DDR_FUNC_COPY_CHANNEL_CFG					(1<<0)

#define DDR_FUNC_CONFIG_AXI_NOC_MODE_6		(1<<24)
#define DDR_FUNC_FORCE_DDR_WINDOW_TEST_ON_POWER_OFF_STAGE					(1<<25)
#define DDR_FUNC_CONFIG_DISABLE_FORCE_TRAINING_DQ_INIT_DELAY_VALUE_FUNCTION	(1<<26)
#define DDR_FUNC_CONFIG_DISABLE_FORCE_TRAINING_1D_DEV_INIT_FUNCTION			(1<<27)
#define DDR_FUNC_CONFIG_DDR_DVFS_FUNCTION	(1<<28)
#define DDR_FUNC_CONFIG_DFE_FUNCTION		(1<<29)
#define DDR_FUNC_FAST_BOOT_CHECK_CHIP_ID	(1<<30)
#define DDR_FUNC_SCRAMBLE					(1<<31)
#define DDR_FUNC							(0)
/* dram cfg magic */
#define DRAM_CFG_MAGIC							0x2e676663

#define DMC_TEST_SLT_SCAN_FREQUENCY				1
#define DMC_TEST_SLT_OFFSET_DELAY				(1<<1)
#define DMC_TEST_SLT_ENABLE_DDR_SKIP_TRAINING	(1<<6)
#define DMC_TEST_SLT_ENABLE_DDR_DVFS			(1<<7)

/* ddr configs */
#define DDR_RFC_TYPE_DDR3_512Mbx1				0
#define DDR_RFC_TYPE_DDR3_512Mbx2				1
#define DDR_RFC_TYPE_DDR3_512Mbx4				2
#define DDR_RFC_TYPE_DDR3_512Mbx8				3
#define DDR_RFC_TYPE_DDR3_512Mbx16				4
#define DDR_RFC_TYPE_DDR4_2Gbx1					5
#define DDR_RFC_TYPE_DDR4_2Gbx2					6
#define DDR_RFC_TYPE_DDR4_2Gbx4					7
#define DDR_RFC_TYPE_DDR4_2Gbx8					8

#define DDR_RFC_TYPE_LPDDR4_2Gbx1				9
#define DDR_RFC_TYPE_LPDDR4_3Gbx1				10
#define DDR_RFC_TYPE_LPDDR4_4Gbx1				11
#define DDR_RFC_TYPE_LPDDR4_6Gbx1				12
#define DDR_RFC_TYPE_LPDDR4_8Gbx1				13

#define DDR_ENABLE_FINE_TUNE_FLAG_AC_DELAY				(1<<0)
#define DDR_ENABLE_FINE_TUNE_FLAG_WRITE_DQS				(1<<1)
#define DDR_ENABLE_FINE_TUNE_FLAG_READ_DQS				(1<<2)
#define DDR_ENABLE_FINE_TUNE_FLAG_WRITE_DQ				(1<<3)
#define DDR_ENABLE_FINE_TUNE_FLAG_READ_DQ				(1<<4)


/* bl2 reg override stages define */
#define BL2_INIT_STAGE_0							0
#define BL2_INIT_STAGE_1							1
#define BL2_INIT_STAGE_2							2
#define BL2_INIT_STAGE_3							3
#define BL2_INIT_STAGE_4							4
#define BL2_INIT_STAGE_5							5
#define BL2_INIT_STAGE_6							6
#define BL2_INIT_STAGE_7							7
#define BL2_INIT_STAGE_8							8
#define BL2_INIT_STAGE_9							9


/* ddr reg override stages define */
#define DDR_OVERRIDE_STAGE_DDR3_PRE_INIT			0x10
#define DDR_OVERRIDE_STAGE_DDR3_DMC_INIT			0x11

#define DDR_OVERRIDE_STAGE_DDR4_PRE_INIT			0x20
#define DDR_OVERRIDE_STAGE_DDR4_DMC_INIT			0x21

#define DDR_OVERRIDE_STAGE_LPDDR3_PRE_INIT			0x30
#define DDR_OVERRIDE_STAGE_LPDDR3_DMC_INIT			0x31

#define DDR_OVERRIDE_STAGE_LPDDR4_PRE_INIT			0x40
#define DDR_OVERRIDE_STAGE_LPDDR4_DMC_INIT			0x41


#define DWC_AC_PINMUX_TOTAL						28
#define DWC_DFI_PINMUX_TOTAL					26
#define DWC_DQ_PINMUX_TOTAL						32

/* diagnose function defines */
#define CONFIG_DIAGNOSE_DISABLE					0x0
#define CONFIG_DIAGNOSE_1D						0x1
#define CONFIG_DIAGNOSE_2D						0x2
#define CONFIG_DIAGNOSE_1D_2D					0x3