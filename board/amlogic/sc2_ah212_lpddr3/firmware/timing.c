// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
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
 *            #define CONFIG_DDR_CHL_AUTO					0xF
 *            #define CONFIG_DDR0_16BIT_CH0					0x1
 *            #define CONFIG_DDR0_16BIT_RANK01_CH0			0x4
 *            #define CONFIG_DDR0_32BIT_RANK0_CH0			0x2
 *            #define CONFIG_DDR0_32BIT_RANK01_CH01			0x3
 *            #define CONFIG_DDR0_32BIT_16BIT_RANK0_CH0		0x5
 *            #define CONFIG_DDR0_32BIT_16BIT_RANK01_CH0	0x6
 * DramType:
 *            #define CONFIG_DDR_TYPE_DDR3					0
 *            #define CONFIG_DDR_TYPE_DDR4					1
 *            #define CONFIG_DDR_TYPE_LPDDR4				2
 *            #define CONFIG_DDR_TYPE_LPDDR3				3
 * DRAMFreq:
 *            {pstate0, pstate1, pstate2, pstate3} //more than one pstate means use dynamic freq
 *
 */

ddr_set_t __ddr_setting[] __attribute__ ((section(".ddr_param"))) = {
{
	// 2layer lpddr3 rank01
	.board_id				= CONFIG_BOARD_ID_MASK,
	.version				= 1,
	//.dram_rank_config		= CONFIG_DDR0_32BIT_RANK01_CH0,
	.dram_rank_config		= CONFIG_DDR0_32BIT_RANK01_CH0,
	.ddr_rfc_type			= DDR_RFC_TYPE_LPDDR4_4Gbx1,
	.DramType				= CONFIG_DDR_TYPE_LPDDR3,
	.DRAMFreq				= {400, 0, 0, 0},
	.ddr_base_addr			= CFG_DDR_BASE_ADDR,
	.ddr_start_offset		= CFG_DDR_START_OFFSET,
	//.imem_load_addr			= 0xFFFC0000, //sram
	//.dmem_load_size			= 0x1000, //4K

	.DisabledDbyte			= 0xf0,
	.Is2Ttiming				= 0,
	.HdtCtrl				= 0xa,
	.dram_cs0_size_MB		= 0xffff,//1024,
	.dram_cs1_size_MB		= 0xffff,//1024,
	.training_SequenceCtrl	= {0x131f,0}, //ddr3 0x21f 0x31f
	.phy_odt_config_rank	= {0x23,0x13}, //use 0x23 0x13  compatibility with 1rank and 2rank //targeting rank 0. [3:0] is used //for write ODT [7:4] is used for //read ODT
	.dfi_odt_config			= 0x0d0d,  //use 0d0d compatibility with 1rank and 2rank  //0808
	.PllBypassEn			= 0, //bit0-ps0,bit1-ps1
	.ddr_rdbi_wr_enable		= 0,
	.clk_drv_ohm			= 60,
	.cs_drv_ohm				= 60,
	.ac_drv_ohm				= 60,
	.soc_data_drv_ohm_p		= 40,
	.soc_data_drv_ohm_n		= 40,
	.soc_data_odt_ohm_p		= 60,
	.soc_data_odt_ohm_n		= 0,
	.dram_data_drv_ohm		= 30, //
	.dram_data_odt_ohm		= 120,
	.dram_ac_odt_ohm		= 0,
	.soc_clk_slew_rate		= 0x3ff,//0x253,
	.soc_cs_slew_rate		= 0x3ff,//0x253,
	.soc_ac_slew_rate		= 0x3ff,//0x253,
	.soc_data_slew_rate		= 0x2ff,
	.vref_output_permil		= 800,//200,
	.vref_receiver_permil	= 700,//875,  //700 for drv 40 odt 60 is better ,why?
	.vref_dram_permil		= 600,//875,
	//.vref_reverse			= 0,
	.ac_trace_delay			= {10,2,10,10,10,0x0,0x0,0x0,0x0,0x0},
	.ac_pinmux				= {00,00},
	.ddr_dmc_remap			= {
							[0] = ( 5 |  6 << 5 |  7 << 10 |  8<< 15 | 9<< 20 | 10 << 25 ),
							[1] = ( 11|  29 << 5 |  0 << 10 | 15 << 15 | 16 << 20 | 17 << 25 ),
							[2] = ( 18| 19 << 5 | 20 << 10 | 21 << 15 | 22 << 20 | 23 << 25 ),
							[3] = ( 24| 25 << 5 | 26 << 10 | 27 << 15 | 28 << 20 | 30 << 25 ),
							[4] = ( 31| 12 << 5 | 13 << 10 |  14<< 15 |  0 << 20 |  0 << 25 ),
	},
	.ddr_lpddr34_ca_remap	= {21/8,8/8,31/8,1/8},// {2,7,1,4,5,6,0,3,9,8},
	.ddr_lpddr34_dq_remap	= {1,2,7,4,0,3,5,6, 8,12,14,9,11,10,15,13, 21,22,16,17,23,20,19,18, 31,29,26,27,30,28,25,24},
	//{21,22,16,17,23,20,19,18,8,12,14,9,11,10,15,13,31,29,26,27,30,28,25,24,1,2,7,4,0,3,5,6},
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
	.ddr_func				= DDR_FUNC,
	.magic					= DRAM_CFG_MAGIC,
	.diagnose				= CONFIG_DIAGNOSE_DISABLE,
	.training_offset 		= (0<<3) | (4<<0),//read dqs offset after training,bit3=0 right move,bit3=1 left move,bit[2:0] offset step
},
{
	// 2layer lpddr3 rank0
	.board_id				= CONFIG_BOARD_ID_MASK,
	.version				= 1,
	//.dram_rank_config		= CONFIG_DDR0_32BIT_RANK01_CH0,
	.dram_rank_config		= CONFIG_DDR0_32BIT_RANK0_CH0,
	.ddr_rfc_type			= DDR_RFC_TYPE_LPDDR4_4Gbx1,
	.DramType				= CONFIG_DDR_TYPE_LPDDR3,
	.DRAMFreq				= {600, 0, 0, 0},
	.ddr_base_addr			= CFG_DDR_BASE_ADDR,
	.ddr_start_offset		= CFG_DDR_START_OFFSET,
	//.imem_load_addr			= 0xFFFC0000, //sram
	//.dmem_load_size			= 0x1000, //4K

	.DisabledDbyte			= 0xf0,
	.Is2Ttiming				= 0,
	.HdtCtrl				= 0xa,
	.dram_cs0_size_MB		= 0xffff,//1024,
	.dram_cs1_size_MB		= 0,//1024,
	.training_SequenceCtrl	= {0x131f,0}, //ddr3 0x21f 0x31f
	.phy_odt_config_rank	= {0x23,0x13}, //use 0x23 0x13  compatibility with 1rank and 2rank //targeting rank 0. [3:0] is used //for write ODT [7:4] is used for //read ODT
	.dfi_odt_config			= 0x0d0d,  //use 0d0d compatibility with 1rank and 2rank  //0808
	.PllBypassEn			= 0, //bit0-ps0,bit1-ps1
	.ddr_rdbi_wr_enable		= 0,
	.clk_drv_ohm			= 60,
	.cs_drv_ohm				= 60,
	.ac_drv_ohm				= 60,
	.soc_data_drv_ohm_p		= 40,
	.soc_data_drv_ohm_n		= 40,
	.soc_data_odt_ohm_p		= 60,
	.soc_data_odt_ohm_n		= 0,
	.dram_data_drv_ohm		= 30, //
	.dram_data_odt_ohm		= 120,
	.dram_ac_odt_ohm		= 0,
	.soc_clk_slew_rate		= 0x3ff,//0x253,
	.soc_cs_slew_rate		= 0x3ff,//0x253,
	.soc_ac_slew_rate		= 0x3ff,//0x253,
	.soc_data_slew_rate		= 0x2ff,
	.vref_output_permil		= 800,//200,
	.vref_receiver_permil	= 700,//875,  //700 for drv 40 odt 60 is better ,why?
	.vref_dram_permil		= 600,//875,
	//.vref_reverse			= 0,
	.ac_trace_delay			= {10,2,10,10,10,0x0,0x0,0x0,0x0,0x0},
	.ac_pinmux				= {00,00},
	.ddr_dmc_remap			= {
							[0] = ( 5 |  6 << 5 |  7 << 10 |  8<< 15 | 9<< 20 | 10 << 25 ),
							[1] = ( 11|  29 << 5 |  0 << 10 | 15 << 15 | 16 << 20 | 17 << 25 ),
							[2] = ( 18| 19 << 5 | 20 << 10 | 21 << 15 | 22 << 20 | 23 << 25 ),
							[3] = ( 24| 25 << 5 | 26 << 10 | 27 << 15 | 28 << 20 | 30 << 25 ),
							[4] = ( 31| 12 << 5 | 13 << 10 |  14<< 15 |  0 << 20 |  0 << 25 ),
	},
	.ddr_lpddr34_ca_remap	= {21/8,8/8,31/8,1/8},// {2,7,1,4,5,6,0,3,9,8},
	.ddr_lpddr34_dq_remap	= {1,2,7,4,0,3,5,6, 8,12,14,9,11,10,15,13, 21,22,16,17,23,20,19,18, 31,29,26,27,30,28,25,24},
	//{21,22,16,17,23,20,19,18,8,12,14,9,11,10,15,13,31,29,26,27,30,28,25,24,1,2,7,4,0,3,5,6},
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
	.ddr_func				= DDR_FUNC,
	.magic					= DRAM_CFG_MAGIC,
	.diagnose				= CONFIG_DIAGNOSE_DISABLE,
	.training_offset 		= (0<<3) | (4<<0),//read dqs offset after training,bit3=0 right move,bit3=1 left move,bit[2:0] offset step
},
};

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
	//need fine tune
	{0,			0,            		0xffffffff,   0, 0, 0},
};

/* gpio/pinmux/pwm init */
register_ops_t __bl2_ops_reg[MAX_REG_OPS_ENTRIES]
__attribute__ ((section(".misc_param"))) = {
	/* demo, user defined override register */
	{PWMGH_PWM_B,		VDDEE_VAL_REG,  	0xffffffff,	0, 0, 0},
	{PWMIJ_PWM_B,		VCCK_VAL_REG,  		0xffffffff,	0, 0, 0},
	{PWMGH_MISC_REG_AB,	(0x1 << 1), 		(0x1 << 1), 0, 0, 0},
	{PWMIJ_MISC_REG_AB,	(0x1 << 1), 		(0x1 << 1), 0, 0, 0},
	/* enable vddcpu dc-dc, set TEST_N to high */
	{PADCTRL_TESTN_O,	(0x1 << 0), 		(0x1 << 0),	0, 0, 0},
	{PADCTRL_TESTN_OEN,	(0x0 << 0), 		(0x1 << 0), 0, 0, 0},
	/* set pwm h and pwm j clock rate to 24M, enable them */
	{CLKCTRL_PWM_CLK_GH_CTRL,	(1 << 8 | 1 << 24) , 	0xffffffff, 	0, 0, 0},
	{CLKCTRL_PWM_CLK_IJ_CTRL,	(1 << 8 | 1 << 24) , 	0xffffffff, 	0, 0, 0},
	/* set GPIOE_0 GPIOE_1 drive strength to 3 */
	{PADCTRL_GPIOE_DS,	0xf, 	0xf,		0, 0, 0},
	/* set GPIOE_0 GPIOE_1 mux to pwmh pwmj */
	{PADCTRL_PIN_MUX_REGI,	(0x3 << 0),		(0xf << 0),	0, 0, 0},
	{PADCTRL_PIN_MUX_REGI,	(0x3 << 4),		(0xf << 4),	0, 0, 0},
};

#define DEV_FIP_SIZE 0x300000
#define DDR_FIP_SIZE 0x40000
/* for all the storage parameter */
storage_parameter_t __store_para __attribute__ ((section(".store_param"))) = {
	.common				= {
		.version = 0x01,
		.device_fip_container_size = DEV_FIP_SIZE,
		.device_fip_container_copies = 4,
		.ddr_fip_container_size = DDR_FIP_SIZE,
	},
	.nand				= {
		.version = 0x01,
		.bbt_pages = 0x01,
		.bbt_start_block = 20,
		.discrete_mode = 1,
		.setup_data.nand_setup_data = (2 << 20) |               \
					(0 << 19) |                     \
					(1 << 17) |                     \
					(1 << 14) |                     \
					(0 << 13) |                     \
					(64 << 6) |                     \
					(4 << 0),
		.reserved_area_blk_cnt = 48,
		.page_per_block = 64,
		.use_param_page_list = 0,
	},
};
