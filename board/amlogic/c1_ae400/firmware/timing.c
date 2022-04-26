// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <asm/arch/secure_apb.h>
#include <asm/arch/timing.h>
#include <asm/arch/ddr_define.h>

/* board clk defines */
#define CPU_CLK					1512

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
 *            #define CONFIG_DDR_CHL_AUTO					0xF
 *            #define CONFIG_DDR0_16BIT_CH0				0x1
 *            #define CONFIG_DDR0_16BIT_RANK01_CH0		0x4
 *            #define CONFIG_DDR0_32BIT_RANK0_CH0			0x2
 *            #define CONFIG_DDR0_32BIT_RANK01_CH01		0x3
 *            #define CONFIG_DDR0_32BIT_16BIT_RANK0_CH0	0x5
 *            #define CONFIG_DDR0_32BIT_16BIT_RANK01_CH0	0x6
 * DramType:
 *            #define CONFIG_DDR_TYPE_DDR3				0
 *            #define CONFIG_DDR_TYPE_DDR4				1
 *            #define CONFIG_DDR_TYPE_LPDDR4				2
 *            #define CONFIG_DDR_TYPE_LPDDR3				3
 * DRAMFreq:
 *            {pstate0, pstate1, pstate2, pstate3} //more than one pstate means use dynamic freq
 *
 */


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

ddr_set_t __ddr_setting[] __attribute__ ((section(".ddr_settings"))) = {
{
	// c1 4layer 2pcs ddr3 rank0
	.board_id				= CONFIG_BOARD_ID_MASK,
	.version				= 1,
	.dram_rank_config		= CONFIG_DDR0_32BIT_RANK0_CH0,
	.DramType				= CONFIG_DDR_TYPE_DDR3,
	.DRAMFreq				= {912, 0, 0, 0},
	.ddr_base_addr			= CFG_DDR_BASE_ADDR,
	.ddr_start_offset		= CFG_DDR_START_OFFSET,
	//.dmem_load_size			= 0x1000, //4K

	.DisabledDbyte			= 0xf0,
	.Is2Ttiming				= 1,
	.HdtCtrl				= 0xC8,
	.dram_cs0_size_MB		= 0xffff,
	.dram_cs1_size_MB		= 0,
	.training_SequenceCtrl	= {0x31f,0}, //ddr3 0x21f 0x31f
	.phy_odt_config_rank	= {0x23,0x13}, //use 0x23 0x13  compatibility with 1rank and 2rank //targeting rank 0. [3:0] is used //for write ODT [7:4] is used for //read ODT
	.dfi_odt_config			= 0x0d0d,  //use 0d0d compatibility with 1rank and 2rank  //0808
	.PllBypassEn			= 0, //bit0-ps0,bit1-ps1
	.ddr_rdbi_wr_enable		= 0,
	.clk_drv_ohm			= 40,
	.cs_drv_ohm				= 40,
	.ac_drv_ohm				= 40,
	.soc_data_drv_ohm_p		= 34,
	.soc_data_drv_ohm_n		= 34,
	.soc_data_odt_ohm_p		= 60, //48,
	.soc_data_odt_ohm_n		= 0,
	.dram_data_drv_ohm		= 34, //ddr4 sdram only 34 or 48, skt board use 34 better
	.dram_data_odt_ohm		= 60,
	.dram_ac_odt_ohm		= 0,
	.soc_clk_slew_rate		= 0x300,
	.soc_cs_slew_rate		= 0x300,
	.soc_ac_slew_rate		= 0x300,
	.soc_data_slew_rate		= 0x200,
	.vref_output_permil		= 500,
	.vref_receiver_permil	= 500, //700,
	.vref_dram_permil		= 500, //700,
	//.vref_reverse			= 0,
	.ac_trace_delay			= {32,32,32,32,32,32,32,32,32,32},
	//{00,00},
	.ac_pinmux				= {00,00},
#if 1
	.ddr_dmc_remap			= {
							[0] = ( 5 |  7 << 5 |  8 << 10 |  9 << 15 | 10 << 20 | 11 << 25 ),
							[1] = ( 12|  0 << 5 |  0 << 10 | 14 << 15 | 15 << 20 | 16 << 25 ),
							[2] = ( 17| 18 << 5 | 19 << 10 | 21 << 15 | 22 << 20 | 23 << 25 ),
							[3] = ( 24| 25 << 5 | 26 << 10 | 27 << 15 | 28 << 20 | 29 << 25 ),
							[4] = ( 30| 13 << 5 | 20 << 10 |  6 << 15 |  0 << 20 |  0 << 25 ),
	},
#else
	//16bit
	.ddr_dmc_remap			= {
							[0] = ( 0 |  5 << 5 |  6<< 10 |  7 << 15 | 8 << 20 | 9 << 25 ),
							[1] = ( 10|  0 << 5 |  0 << 10 | 14 << 15 | 15 << 20 | 16 << 25 ),
							[2] = ( 17|( 18 << 5) |( 19 << 10) |( 20 << 15) |( 21 << 20) | (22 << 25 )),
							[3] = ( 23| 24 << 5 | 25 << 10 | 26 << 15 | 27 << 20 | 28 << 25 ),
							[4] = ( 29| 11<< 5 | 12 << 10 |  13<< 15 |  0 << 20 |  0 << 25 ),
	},
#endif
	.ddr_lpddr34_ca_remap	= {00,00},
	.ddr_lpddr34_dq_remap	= {00,00},
	.dram_rtt_nom_wr_park	= {00,00},

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
	.pll_ssc_mode			= (1<<20) | (1<<8) | (2<<4) | 0,//center_ssc_1000ppm
	.ddr_func				= DDR_FUNC | DDR_FUNC_CONFIG_DFE_FUNCTION,
	.magic					= DRAM_CFG_MAGIC,
	.fast_boot[0]			= 1,
},
{
	// c1 4layer 2pcs ddr3 rank0 768MB
	.board_id				= CONFIG_BOARD_ID_MASK,
	.version				= 1,
	.dram_rank_config		= CONFIG_DDR0_32BIT_16BIT_RANK0_CH0,//CONFIG_DDR0_16BIT_CH0,
	.DramType				= CONFIG_DDR_TYPE_DDR3,
	/* DRAMFreq = 192, 256, 384, 512, 768-1536 */
	.DRAMFreq				= {912, 0, 0, 0},
	.ddr_base_addr			= CFG_DDR_BASE_ADDR,
	.ddr_start_offset		= CFG_DDR_START_OFFSET,
	//.dmem_load_size			= 0x1000, //4K

	.DisabledDbyte			= 0xf0,
	.Is2Ttiming				= 1,
	.HdtCtrl				= 0xC8,
	.dram_cs0_size_MB		= 768,
	.dram_cs1_size_MB		= 0,
	.training_SequenceCtrl	= {0x31f,0}, //ddr3 0x21f 0x31f
	.phy_odt_config_rank	= {0x23,0x13}, //use 0x23 0x13  compatibility with 1rank and 2rank //targeting rank 0. [3:0] is used //for write ODT [7:4] is used for //read ODT
	.dfi_odt_config			= 0x0d0d,  //use 0d0d compatibility with 1rank and 2rank  //0808
	.PllBypassEn			= 0, //bit0-ps0,bit1-ps1
	.ddr_rdbi_wr_enable		= 0,
	.clk_drv_ohm			= 40,
	.cs_drv_ohm				= 40,
	.ac_drv_ohm				= 40,
	.soc_data_drv_ohm_p		= 34,
	.soc_data_drv_ohm_n		= 34,
	.soc_data_odt_ohm_p		= 60, //48,
	.soc_data_odt_ohm_n		= 0,
	.dram_data_drv_ohm		= 34, //ddr4 sdram only 34 or 48, skt board use 34 better
	.dram_data_odt_ohm		= 60,
	.dram_ac_odt_ohm		= 0,
	.soc_clk_slew_rate		= 0x300,
	.soc_cs_slew_rate		= 0x300,
	.soc_ac_slew_rate		= 0x300,
	.soc_data_slew_rate		= 0x200,
	.vref_output_permil		= 500,
	.vref_receiver_permil	= 500, //700,
	.vref_dram_permil		= 500, //700,
	//.vref_reverse			= 0,
	.ac_trace_delay			= {32,32,32,32,32,32,32,32,32,32},
	//{00,00},
	.ac_pinmux				= {00,00},
#if 1
	.ddr_dmc_remap			= {
							[0] = ( 5 |  7 << 5 |  8 << 10 |  9 << 15 | 10 << 20 | 11 << 25 ),
							[1] = ( 12|  0 << 5 |  0 << 10 | 14 << 15 | 15 << 20 | 16 << 25 ),
							[2] = ( 17| 18 << 5 | 19 << 10 | 21 << 15 | 22 << 20 | 23 << 25 ),
							[3] = ( 24| 25 << 5 | 26 << 10 | 27 << 15 | 28 << 20 | 29 << 25 ),
							[4] = ( 30| 13 << 5 | 20 << 10 |  6 << 15 |  0 << 20 |  0 << 25 ),
	},
#else
	//16bit
	.ddr_dmc_remap			= {
							[0] = ( 0 |  5 << 5 |  6<< 10 |  7 << 15 | 8 << 20 | 9 << 25 ),
							[1] = ( 10|  0 << 5 |  0 << 10 | 14 << 15 | 15 << 20 | 16 << 25 ),
							[2] = ( 17|( 18 << 5) |( 19 << 10) |( 20 << 15) |( 21 << 20) | (22 << 25 )),
							[3] = ( 23| 24 << 5 | 25 << 10 | 26 << 15 | 27 << 20 | 28 << 25 ),
							[4] = ( 29| 11<< 5 | 12 << 10 |  13<< 15 |  0 << 20 |  0 << 25 ),
	},
#endif
	.ddr_lpddr34_ca_remap	= {00,00},
	.ddr_lpddr34_dq_remap	= {00,00},
	.dram_rtt_nom_wr_park	= {00,00},

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
	.pll_ssc_mode			= (1<<20) | (1<<8) | (2<<4) | 0,//center_ssc_1000ppm
	.ddr_func				= DDR_FUNC | DDR_FUNC_CONFIG_DFE_FUNCTION,
	.magic					= DRAM_CFG_MAGIC,
	.fast_boot[0]			= 1,
},
{
	// c1 4layer 2pcs ddr4 rank0
	.board_id				= CONFIG_BOARD_ID_MASK,
	.version				= 1,
	.dram_rank_config		= CONFIG_DDR0_32BIT_RANK0_CH0,
	.ddr_rfc_type			= DDR_RFC_TYPE_DDR4_2Gbx8,
	.DramType				= CONFIG_DDR_TYPE_DDR4,
	.DRAMFreq				= {1176, 0, 0, 0},
	.ddr_base_addr			= CFG_DDR_BASE_ADDR,
	.ddr_start_offset		= CFG_DDR_START_OFFSET,
	//.dmem_load_size			= 0x1000, //4K

	.DisabledDbyte			= 0xf0,
	.Is2Ttiming				= 1,
	.HdtCtrl				= 0xC8,
	.dram_cs0_size_MB		= 0xffff,
	.dram_cs1_size_MB		= 0,
	.training_SequenceCtrl	= {0x31f,0x61}, //ddr3 0x21f 0x31f
	.phy_odt_config_rank	= {0x23,0x13}, //use 0x23 0x13  compatibility with 1rank and 2rank //targeting rank 0. [3:0] is used //for write ODT [7:4] is used for //read ODT
	.dfi_odt_config			= 0x0d0d,  //use 0d0d compatibility with 1rank and 2rank  //0808
	.PllBypassEn			= 0, //bit0-ps0,bit1-ps1
	.ddr_rdbi_wr_enable		= 0,
	.clk_drv_ohm			= 40,
	.cs_drv_ohm				= 40,
	.ac_drv_ohm				= 40,
	.soc_data_drv_ohm_p		= 40,
	.soc_data_drv_ohm_n		= 40,
	.soc_data_odt_ohm_p		= 60,
	.soc_data_odt_ohm_n		= 0,
	.dram_data_drv_ohm		= 34,//48, //34, //ddr4 sdram only 34 or 48, skt board use 34 better
	.dram_data_odt_ohm		= 60, //60,
	.dram_ac_odt_ohm		= 0,
	.soc_clk_slew_rate		= 0x3ff,
	.soc_cs_slew_rate		= 0x3ff,
	.soc_ac_slew_rate		= 0x3ff,
	.soc_data_slew_rate		= 0x2ff,
	.vref_output_permil		= 500,
	.vref_receiver_permil	= 0,//700,
	.vref_dram_permil		= 0,//700,
	//.vref_reverse			= 0,
	//.ac_trace_delay		= {0x0,0x0},// {0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40},
	.ac_trace_delay			= {16,32,32,32,32,32,32,32,32,32},
	.ddr_dmc_remap			= {
							[0] = ( 5 |  7 << 5 |  8 << 10 |  9 << 15 | 10 << 20 | 11 << 25 ),
							[1] = ( 12|  0 << 5 |  0 << 10 | 14 << 15 | 15 << 20 | 16 << 25 ),
							[2] = ( 17| 18 << 5 | 19 << 10 | 21 << 15 | 22 << 20 | 23 << 25 ),
							[3] = ( 24| 25 << 5 | 26 << 10 | 27 << 15 | 28 << 20 | 29 << 25 ),
							[4] = ( 30| 13 << 5 | 20 << 10 |  6 << 15 |  0 << 20 |  0 << 25 ),
	},
	.ddr_lpddr34_ca_remap	= {00,00},
	.ddr_lpddr34_dq_remap	= {00,00},
	.dram_rtt_nom_wr_park	= {00,00},

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
	.pll_ssc_mode			= (1<<20) | (1<<8) | (2<<4) | 0,//center_ssc_1000ppm
	.ddr_func				= DDR_FUNC | DDR_FUNC_CONFIG_DFE_FUNCTION,
	.magic					= DRAM_CFG_MAGIC,
	.fast_boot[0]			= 1,
},
};

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
#if   (VCCK_VAL == 1040)
	#define VCCK_VAL_REG	0x00000022
#elif (VCCK_VAL == 1030)
	#define VCCK_VAL_REG	0x00010021
#elif (VCCK_VAL == 1020)
	#define VCCK_VAL_REG	0x00020020
#elif (VCCK_VAL == 1010)
	#define VCCK_VAL_REG	0x0003001f
#elif (VCCK_VAL == 1000)
	#define VCCK_VAL_REG	0x0004001e
#elif (VCCK_VAL == 990)
	#define VCCK_VAL_REG	0x0005001d
#elif (VCCK_VAL == 980)
	#define VCCK_VAL_REG	0x0006001c
#elif (VCCK_VAL == 970)
	#define VCCK_VAL_REG	0x0007001b
#elif (VCCK_VAL == 960)
	#define VCCK_VAL_REG	0x0008001a
#elif (VCCK_VAL == 950)
	#define VCCK_VAL_REG	0x00090019
#elif (VCCK_VAL == 940)
	#define VCCK_VAL_REG	0x000a0018
#elif (VCCK_VAL == 930)
	#define VCCK_VAL_REG	0x000b0017
#elif (VCCK_VAL == 920)
	#define VCCK_VAL_REG	0x000c0016
#elif (VCCK_VAL == 910)
	#define VCCK_VAL_REG	0x000d0015
#elif (VCCK_VAL == 900)
	#define VCCK_VAL_REG	0x000e0014
#elif (VCCK_VAL == 890)
	#define VCCK_VAL_REG	0x000f0013
#elif (VCCK_VAL == 880)
	#define VCCK_VAL_REG	0x00100012
#elif (VCCK_VAL == 870)
	#define VCCK_VAL_REG	0x00110011
#elif (VCCK_VAL == 860)
	#define VCCK_VAL_REG	0x00120010
#elif (VCCK_VAL == 850)
	#define VCCK_VAL_REG	0x0013000f
#elif (VCCK_VAL == 840)
	#define VCCK_VAL_REG	0x0014000e
#elif (VCCK_VAL == 830)
	#define VCCK_VAL_REG	0x0015000d
#elif (VCCK_VAL == 820)
	#define VCCK_VAL_REG	0x0016000c
#elif (VCCK_VAL == 810)
	#define VCCK_VAL_REG	0x0017000b
#elif (VCCK_VAL == 800)
	#define VCCK_VAL_REG	0x0018000a
#elif (VCCK_VAL == 790)
	#define VCCK_VAL_REG	0x00190009
#elif (VCCK_VAL == 780)
	#define VCCK_VAL_REG	0x001a0008
#elif (VCCK_VAL == 770)
	#define VCCK_VAL_REG	0x001b0007
#elif (VCCK_VAL == 760)
	#define VCCK_VAL_REG	0x001c0006
#elif (VCCK_VAL == 750)
	#define VCCK_VAL_REG	0x001d0005
#elif (VCCK_VAL == 740)
	#define VCCK_VAL_REG	0x001e0004
#elif (VCCK_VAL == 730)
	#define VCCK_VAL_REG	0x001f0003
#elif (VCCK_VAL == 720)
	#define VCCK_VAL_REG	0x00200002
#elif (VCCK_VAL == 710)
	#define VCCK_VAL_REG	0x00210001
#elif (VCCK_VAL == 700)
	#define VCCK_VAL_REG	0x00220000
#else
	#error "VCCK val out of range\n"
#endif

/* VDDEE_VAL_REG: VDDEE PWM table  0.69v-0.89v*/
#if    (VDDEE_VAL == 741)
	#define VDDEE_VAL_REG	0x120000
#elif (VDDEE_VAL == 751)
	#define VDDEE_VAL_REG	0x110001
#elif (VDDEE_VAL == 761)
	#define VDDEE_VAL_REG	0x100002
#elif (VDDEE_VAL == 771)
	#define VDDEE_VAL_REG	0xf0003
#elif (VDDEE_VAL == 781)
	#define VDDEE_VAL_REG	0xe0004
#elif (VDDEE_VAL == 791)
	#define VDDEE_VAL_REG	0xd0005
#elif (VDDEE_VAL == 801)
	#define VDDEE_VAL_REG	0xc0006
#elif (VDDEE_VAL == 811)
	#define VDDEE_VAL_REG	0xb0007
#elif (VDDEE_VAL == 821)
	#define VDDEE_VAL_REG	0xa0008
#elif (VDDEE_VAL == 831)
	#define VDDEE_VAL_REG	0x90009
#elif (VDDEE_VAL == 841)
	#define VDDEE_VAL_REG	0x8000a
#elif (VDDEE_VAL == 851)
	#define VDDEE_VAL_REG	0x7000b
#elif (VDDEE_VAL == 861)
	#define VDDEE_VAL_REG	0x6000c
#elif (VDDEE_VAL == 871)
	#define VDDEE_VAL_REG	0x5000d
#elif (VDDEE_VAL == 881)
	#define VDDEE_VAL_REG	0x4000e
#elif (VDDEE_VAL == 891)
	#define VDDEE_VAL_REG	0x3000f
#elif (VDDEE_VAL == 902)
	#define VDDEE_VAL_REG	0x20010
#elif (VDDEE_VAL == 912)
	#define VDDEE_VAL_REG	0x10011
#elif (VDDEE_VAL == 922)
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
	{PWMAB_PWM_A,		VDDEE_VAL_REG,  	0xffffffff,	0, BL2_INIT_STAGE_1, 0},
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
