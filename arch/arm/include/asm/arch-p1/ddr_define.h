
/*
 * arch/arm/include/asm/arch-p1/ddr_define.h
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

/* board id */
#define CONFIG_BOARD_ID_MASK					0xFF
#define CONFIG_FW_INFO_MASK						0xFFFFFFFF

#define RESULT_OK							0x0
#define RESULT_FAIL							0xFF
#define RESULT_PLL_LOCK_FAIL				0x0F
#define RESULT_USB_BOOT_NEVER				0xF0

#define DDR_HDTCTRL_ENABLE_AMLOGIC_DDR4_PHY_VREF_CORRECTION       0x7
#define DDR_HDTCTRL_SHOW_DDR3_LPDDR3_TRAINING_MARGIN              0x9
#define DDR_HDTCTRL_SHOW_DDR3_LPDDR3_TRAINING_MARGIN_WRITE_DQ     0x00210005
#define DDR_HDTCTRL_SHOW_DDR3_LPDDR3_TRAINING_MARGIN_WRITE_DQM    0x00290005
#define DDR_HDTCTRL_SHOW_DDR3_LPDDR3_TRAINING_MARGIN_READ_DQ      0x003a0005

#define DDR_HDTCTRL_SHOW_DDR4_TRAINING_EYE_DATA_READ_START_1      0x00200004
#define DDR_HDTCTRL_SHOW_DDR4_TRAINING_EYE_DATA_READ_START_2      0x00240004
#define DDR_HDTCTRL_SHOW_DDR4_TRAINING_EYE_DATA_READ_VREF         0x0014001f
#define DDR_HDTCTRL_SHOW_DDR4_TRAINING_EYE_DATA_READ_VREF_END     0x00710000

#define DDR_HDTCTRL_SHOW_DDR4_TRAINING_EYE_DATA_WRITE_START_1     0x001d0004
#define DDR_HDTCTRL_SHOW_DDR4_TRAINING_EYE_DATA_WRITE_START_2     0x00270004
#define DDR_HDTCTRL_SHOW_LPDDR4_TRAINING_TDQS2DQ                  0x00e10002
//#define DDR_HDTCTRL_SHOW_DDR4_TRAINING_EYE_DATA_WRITE_VREF				0x0014001f
//#define DDR_HDTCTRL_SHOW_DDR4_TRAINING_EYE_DATA_WRITE_VREF_END			0x00610000
#define DMC_PSTATE_MAX                                            4

#define CONFIG_CHIP_TYPE_DRAM                                     0x1F
#define CONFIG_CHIP_TYPE_PSRAM                                    0x2F
#define CONFIG_DDR_TYPE_DDR3                                      0
#define CONFIG_DDR_TYPE_DDR4                                      1
#define CONFIG_DDR_TYPE_LPDDR4                                    2
#define CONFIG_DDR_TYPE_LPDDR3                                    3
#define CONFIG_DDR_TYPE_LPDDR2                                    4
//#define CONFIG_DDR_TYPE_LPDDR4X				5
#define CONFIG_DDR_TYPE_LPDDR5                                    6

#define CONFIG_DDR_TYPE_AUTO                                      0xf
#define CONFIG_DDR_TYPE_AUTO_LIMIT                                CONFIG_DDR_TYPE_DDR4

/* lpddr3 related define */
#define CONFIG_LPDDR3_CA_TRAINING_USE_LANE0                       0
#define CONFIG_LPDDR3_CA_TRAINING_USE_LANE1                       1
#define CONFIG_LPDDR3_CA_TRAINING_USE_LANE2                       2
#define CONFIG_LPDDR3_CA_TRAINING_USE_LANE3                       3

#define LPDDR_DIE_ROW_COL_R13_C9                                  0
#define LPDDR_DIE_ROW_COL_R14_C9                                  1
#define LPDDR_DIE_ROW_COL_R14_C10                                 2
#define LPDDR_DIE_ROW_COL_R13_C10                                 3
#define LPDDR_DIE_ROW_COL_R14_C11                                 4

/* ddr channel defines */
#define CONFIG_DDR0_16BIT                                         1
#define CONFIG_DDR0_RANK0                                         2
#define CONFIG_DDR0_RANK01                                        3
#define CONFIG_DDR0_16BIT_2                                       4
#define CONFIG_DDR_CHL_AUTO                                       0xF

#if 0
#define CONFIG_DDR0_16BIT_CH0                                     0x1
#define CONFIG_DDR0_16BIT_RANK01_CH0                              0x4
#define CONFIG_DDR0_32BIT_RANK0_CH0                               0x2
#define CONFIG_DDR0_32BIT_RANK01_CH01                             0x3
#define CONFIG_DDR0_32BIT_16BIT_RANK0_CH0                         0x5
#define CONFIG_DDR0_32BIT_16BIT_RANK01_CH0                        0x6
#define CONFIG_DDR0_32BIT_RANK01_CH0                              0x7
#define CONFIG_DDR0_32BIT_RANK0_CH01                              0x8
#define CONFIG_DDR0_32BIT_RANK0_16BIT_RANK1_LOW_CH0               0x9
#define CONFIG_DDR0_32BIT_RANK0_16BIT_RANK1_HIGH_CH0              0xa
#define CONFIG_DDR0_32BIT_16BIT_RANK0_CH0_MODE2                   0xB
#define CONFIG_DDR0_32BIT_RANK0_16BIT_RANK1_LOW_CH0_MODE2         0xC
#else

#define ENABLE_CONFIG_CH0_CS0_BYTE_0_0                            (1 << 0)
#define ENABLE_CONFIG_CH0_CS0_BYTE_1_1                            (1 << 1)
#define ENABLE_CONFIG_CH0_CS0_BYTE_2_2                            (1 << 2)
#define ENABLE_CONFIG_CH0_CS0_BYTE_3_3                            (1 << 3)
#define ENABLE_CONFIG_CH0_CS1_BYTE_0_4                            (1 << 4)
#define ENABLE_CONFIG_CH0_CS1_BYTE_1_5                            (1 << 5)
#define ENABLE_CONFIG_CH0_CS1_BYTE_2_6                            (1 << 6)
#define ENABLE_CONFIG_CH0_CS1_BYTE_3_7                            (1 << 7)
#define ENABLE_CONFIG_CH1_CS0_BYTE_0_8                            (1 << 8)
#define ENABLE_CONFIG_CH1_CS0_BYTE_1_9                            (1 << 9)
#define ENABLE_CONFIG_CH1_CS0_BYTE_2_10                           (1 << 10)
#define ENABLE_CONFIG_CH1_CS0_BYTE_3_11                           (1 << 11)
#define ENABLE_CONFIG_CH1_CS1_BYTE_0_12                           (1 << 12)
#define ENABLE_CONFIG_CH1_CS1_BYTE_1_13                           (1 << 13)
#define ENABLE_CONFIG_CH1_CS1_BYTE_2_14                           (1 << 14)
#define ENABLE_CONFIG_CH1_CS1_BYTE_3_15                           (1 << 15)

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

#endif

#define ENABLE_SKIP_1D_MEM_LOAD         0x0
#define ENABLE_SKIP_1D_MEM_TRAINING     0x1
#define ENABLE_SKIP_2D_MEM_LOAD         0x2
#define ENABLE_SKIP_2D_MEM_TRAINING     0x3

#define PHY_IMEM_STEP_1D                0x1
#define PHY_IMEM_STEP_2D                0x2

#define VREF_SOC        0x1
#define VREF_DRAM       0x2


#define CFG_DDR_BASE_ADDR					0X0
#define CFG_DDR_START_OFFSET				0X01000000 //SKIP 16MB

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

#define CONFIG_DDR_INIT_RETRY_TOTAL     (10)
#define CONFIG_DDR_PCTL_RETRY_TOTAL     (100)

#define DDR_USE_1_RANK(chl_set)    ((chl_set == CONFIG_DDR0_RANK0) \
				    || (chl_set == CONFIG_DDR0_16BIT))
#define DDR_USE_2_RANK(chl_set)    ((chl_set == CONFIG_DDR0_RANK01) \
				    || (chl_set == CONFIG_DDR0_16BIT_2))

#ifndef CONFIG_LPDDR3_CA_TRAINING_CA0
#define CONFIG_LPDDR3_CA_TRAINING_CA0    CONFIG_LPDDR3_CA_TRAINING_USE_LANE0
#endif
#ifndef CONFIG_LPDDR3_CA_TRAINING_CA1
#define CONFIG_LPDDR3_CA_TRAINING_CA1    CONFIG_LPDDR3_CA_TRAINING_USE_LANE1
#endif

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

#define DDR_FUNC_CONFIG_RD_ECC_FUNCTION					(1<<22)
#define DDR_FUNC_CONFIG_OFFSET_TRAINING_PER_DELAY		(1 << 23)
#define DDR_FUNC_CONFIG_AXI_NOC_MODE_6		(1<<24)
#define DDR_FUNC_FORCE_DDR_WINDOW_TEST_ON_POWER_OFF_STAGE					(1<<25)
#define DDR_FUNC_CONFIG_DISABLE_FORCE_TRAINING_DQ_INIT_DELAY_VALUE_FUNCTION	(1<<26)
#define DDR_FUNC_CONFIG_DISABLE_FORCE_TRAINING_1D_DEV_INIT_FUNCTION			(1<<27)
#define DDR_FUNC_CONFIG_DDR_DVFS_FUNCTION	(1<<28)
#define DDR_FUNC_CONFIG_DFE_FUNCTION		(1<<29)
#define DDR_FUNC_FAST_BOOT_CHECK_CHIP_ID	(1<<30)
#define DDR_FUNC_SCRAMBLE					(1<<31)

#define DDR_FUNC							(0)

/* dwc imem/dmem */
#define DWC_MEM_LOAD_ADDR					0xFFFE0000
#define DWC_IRAM_ADDR						(0x50000)
#define DWC_IRAM_SIZE						(0x10000) /* 64K */
#define DWC_DRAM_ADDR						(0x58000)
#define DWC_DRAM_SIZE						(0x4000) /* 16K */

/* dwc apb regs */
#define UctWriteProtShadow					0xd0004
#define UctWriteOnlyShadow					0xd0032
#define UctWriteProt						0xd0031
#define UctDatWriteOnlyShadow				0xd0034

/* pctl status */
#define UPCTL_STAT_MASK					(7)
#define UPCTL_STAT_INIT					(0)
#define UPCTL_STAT_CONFIG					(1)
#define UPCTL_STAT_ACCESS					(3)
#define UPCTL_STAT_LOW_POWER				(5)

/* pctl cmds */
#define UPCTL_CMD_INIT						(0)
#define UPCTL_CMD_CONFIG					(1)
#define UPCTL_CMD_GO						(2)
#define UPCTL_CMD_SLEEP						(3)
#define UPCTL_CMD_WAKEUP					(4)


/* name id defined in firmware header */
#define DDR_NAME_ID_AML_FW						0x304c4d41 //AML0
#define DDR_NAME_ID_PIEI						0x61656165 //eaea
#define DDR_NAME_ID_DDR3						0x33333364 //d333
#define DDR_NAME_ID_DDR4						0x34343464 //d444
#define DDR_NAME_ID_DDR4_2D						0x32323464 //d422
#define DDR_NAME_ID_LPDDR3						0x33336C64 //dl33
#define DDR_NAME_ID_LPDDR4						0x34346C64 //dl44
#define DDR_NAME_ID_LPDDR4_2D					0x30346C64 //dl42
#define DDR_NAME_ID_DIAGNOSE1					0x31676464 //ddg1
#define DDR_NAME_ID_DIAGNOSE2					0x32676464 //ddg2
/* PIEI */
#define DDR_PIEI_START_FLAG						0xfeed //eaea

/* dram cfg magic */
#define DRAM_CFG_MAGIC							0x2e676663

/* flow control */
#define DDR_FIRMWARE_VERSION_1					0x00202006
#define FW_LOAD_STEP_AML_FW						0xF
#define FW_LOAD_STEP_PIEI						0
#define FW_LOAD_STEP_1D							1
#define FW_LOAD_STEP_2D							2
#define FW_LOAD_STEP_DIAG1						3
#define FW_LOAD_STEP_DIAG2						4


/* basic defines */
#define DWC_AC_PINMUX_TOTAL						24
#define DWC_DFI_PINMUX_TOTAL					10  //for timming c struct size 32bit align
#define DWC_DQ_PINMUX_TOTAL						32
#define DWC_TOTAL_CA_NUM_PER_CH  12
#define DWC_TOTAL_DATA_NIBBLE_PER_PHY  4

/* d2pll defines */
#define D2PLL_ENABLE									1
#define DDR_STICKY_MAGIC_NUMBER							0x20180000
#define DDR_CHIP_ID										0x30
#define DDR_STICKY_OVERRIDE_CONFIG_MESSAGE_CMD			0x1 //override config
#define DDR_STICKY_SPECIAL_FUNCTION_CMD					0x2 //special test such as shift some bdlr or parameter or interleave test
#define DDR_INIT_CONFIG_STICKY_MESSAGE_SRAM_ADDRESS		0x00040000
#define DDR_INIT_CONFIG_GLOBAL_MESSAGE_SRAM_ADDRESS		0x00050000


/* debug function*/

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


#define DDR_RFC_TYPE_LPDDR5_4Gbx1				15
#define DDR_RFC_TYPE_LPDDR5_6Gbx1				16
#define DDR_RFC_TYPE_LPDDR5_8Gbx1				17


#define DDR_ENABLE_FINE_TUNE_FLAG_AC_DELAY				1<<0
#define DDR_ENABLE_FINE_TUNE_FLAG_WRITE_DQS				1<<1
#define DDR_ENABLE_FINE_TUNE_FLAG_READ_DQS				1<<2
#define DDR_ENABLE_FINE_TUNE_FLAG_WRITE_DQ     1 << 3
#define DDR_ENABLE_FINE_TUNE_FLAG_READ_DQ      1 << 4

#if 0
#define IMPEDENCE_120P3_240P2_480P1            1
#define IMPEDENCE_120P6_1                      2
#define IMPEDENCE_120P3_240P2_480P1_ODT        3
#endif

#define IMPEDENCE_LPDDR4X_ATX_P                4
#define IMPEDENCE_LPDDR4X_ATX_N                5
#define IMPEDENCE_LPDDR4_ATX_P                 6
#define IMPEDENCE_LPDDR4_ATX_N                 7
#define IMPEDENCE_DDR4_ATX_P                   8
#define IMPEDENCE_DDR4_ATX_N                   9

#define IMPEDENCE_LPDDR4X_DATA_P               10
#define IMPEDENCE_LPDDR4X_DATA_N               11
#define IMPEDENCE_LPDDR4_DATA_P                12
#define IMPEDENCE_LPDDR4_DATA_N                13
#define IMPEDENCE_DDR4_DATA_P                  14
#define IMPEDENCE_DDR4_DATA_N                  15
#define IMPEDENCE_DDR4_DATA_ODT_P                  16
#define IMPEDENCE_DDR4_DATA_ODT_N                  17
#define IMPEDENCE_LPDDR4_DATA_ODT_P                  18
#define IMPEDENCE_LPDDR4_DATA_ODT_N                  19
#define IMPEDENCE_LPDDR4X_DATA_ODT_P                  20
#define IMPEDENCE_LPDDR4X_DATA_ODT_N                 21
#define IMPEDENCE_LPDDR5_DATA_ODT_P                  22
#define IMPEDENCE_LPDDR5_DATA_ODT_N                  23
#define IMPEDENCE_LPDDR5_ATX_P                 24
#define IMPEDENCE_LPDDR5_ATX_N                 25

//DDR PHY BLOCK TYPE
#define  DDR_PHY_BLOCK_DBYTE 1
#define  DDR_PHY_BLOCK_MASTER 2
#define  DDR_PHY_BLOCK_AC 3
#define  DDR_PHY_BLOCK_ACSM_MEMORY 4
#define  DDR_PHY_BLOCK_UCTL_MEMORY 5
#define  DDR_PHY_BLOCK_PPGC 7
#define  DDR_PHY_BLOCK_PIE 9
#define  DDR_PHY_BLOCK_PState_MEMORY 10
#define  DDR_PHY_BLOCK_PUB 12
#define  DDR_PHY_BLOCK_APBONLY 13



/* others */ /* useless? */

/* bist */
#define CONFIG_DDR_CODE_INCLUDE_LPDDR3								0
#define CONFIG_DDR_CODE_INCLUDE_DDR4								1
#define CONFIG_DEBUG_INCLUDE_D2PLL									1
#define CONFIG_DEBUG_DDR_BIST_TEST_WINDOWS							1
#define CONFIG_DEBUG_DDR_BIST_ONLY_COMPARE_ONE_LANE					1
#define CONFIG_DEBUG_DDR_BIST_BYTE_EXTRA_PATTERN					1
#define CONFIG_DEBUG_DDR_BIST_NO_USE_FSM_INIT_VALUE					1
#define CONFIG_DEBUG_DDR_BIST_NO_USE_FSM_INIT_VALUE_USE_LINEAR		0
#define CONFIG_DEBUG_DDR_BIST_NO_USE_FSM_INIT_VALUE_USE_FAST_LINEAR	1
#define CONFIG_DEBUG_DDR_BIST_USE_OLD_METHOD_AGAIN					0
#define CONFIG_DDR_DMC_DDR_TEST										0
#define CONFIG_DDR_CPU_DDR_TEST										0
#define CONFIG_BIST_USE_THRESHOLD									0
#define CONFIG_DEBUG_DDR_BIST_TEST_2D_EYE							0

#define CONFIG_DEBUG_DDR_BIST_TEST_DATA
//#define CONFIG_DEBUG_DDR_BIST_TEST_ADD

#define DDR_BIST_LANE_DATA0						0
#define DDR_BIST_LANE_DATA1						1
#define DDR_BIST_LANE_DATA2						2
#define DDR_BIST_LANE_DATA3						3
#define DDR_BIST_LANE_AC						4

#define DDR_BIST_PATTERN_WALK_0					0
#define DDR_BIST_PATTERN_WALK_1					1
#define DDR_BIST_PATTERN_WALK_RANDOM			2
#define DDR_BIST_PATTERN_WALK_PRE				3

/* diagnose function defines */
#define CONFIG_DIAGNOSE_DISABLE					0x0
#define CONFIG_DIAGNOSE_1D						0x1
#define CONFIG_DIAGNOSE_2D						0x2
#define CONFIG_DIAGNOSE_1D_2D					0x3