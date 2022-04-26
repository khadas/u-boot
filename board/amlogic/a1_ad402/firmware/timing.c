// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <asm/arch/secure_apb.h>
#include <asm/arch/timing.h>
#include <asm/arch/ddr_define.h>

/* board clk defines */
#define DDR_SIZE				0
#define CPU_CLK					1200

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


#define PSRAM_CHIP_LOGIC_INDEX_SOC				1

#define PSRAM_CHIP_LOGIC_INDEX_WINBOND_3_CMD_W955D8MKY	0x20
#define PSRAM_CHIP_LOGIC_INDEX_WINBOND_3_CMD_W956D8MKY	0x21
#define PSRAM_CHIP_LOGIC_INDEX_AP_MEMORY_3_CMD			0x30

//#define PSRAM_ENABLE
#ifdef PSRAM_ENABLE
#define PSRAM_CHIP_LOGIC_INDEX PSRAM_CHIP_LOGIC_INDEX_AP_MEMORY_3_CMD
#endif

/* please config __psram_setting[] if board use psram */
ddr_set_t __ddr_setting[] __attribute__ ((section(".ddr_settings"))) = {
{
	/* ddr3 */
	.board_id				= CONFIG_BOARD_ID_MASK,
	.version				= 1,
	.dram_rank_config		= CONFIG_DDR0_16BIT_CH0,
	.DramType				= CONFIG_DDR_TYPE_DDR3,
	/* DRAMFreq = 192, 256, 384, 512, 768-1536 */
	.DRAMFreq				= {768, 0, 0, 0},
	.ddr_base_addr			= CFG_DDR_BASE_ADDR,
	.ddr_start_offset		= CFG_DDR_START_OFFSET,
	.dmem_load_size			= 0x1000, //4K

	.DisabledDbyte			= 0xfc,
	.Is2Ttiming				= 1,
	.HdtCtrl				= 0xC8,
	.dram_cs0_size_MB		= 128,
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
#if 0
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
	.ddr_func				= DDR_FUNC,
	.magic					= DRAM_CFG_MAGIC,
},
{
	/* ddr4 */
	.board_id				= CONFIG_BOARD_ID_MASK,
	.version				= 1,
	.dram_rank_config		= CONFIG_DDR0_32BIT_RANK0_CH0,
	.DramType				= CONFIG_DDR_TYPE_DDR4,
	/* DRAMFreq = 192, 256, 384, 512, 768-1536 */
	.DRAMFreq				= {768, 0, 0, 0},
	.ddr_rfc_type			= DDR_RFC_TYPE_DDR4_2Gbx8,
	.ddr_base_addr			= CFG_DDR_BASE_ADDR,
	.ddr_start_offset		= CFG_DDR_START_OFFSET,
	.dmem_load_size			= 0x1000, //4K

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
	.ac_trace_delay			= {32,32,32,32,32,32,32,32,32,32},
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
	.ddr_func				= DDR_FUNC,
	.magic					= DRAM_CFG_MAGIC,
},
};

psram_set_t __psram_setting[] = {
#ifdef PSRAM_ENABLE
{
	.psram_mr					= {0}, /* 12 uint8 */
	.psram_board_mask			= 0,
	//.psram_amlogic_protocol_id	= PSRAM_CHIP_LOGIC_INDEX_AP_MEMORY_3_CMD,
	//.psram_amlogic_protocol_id	= PSRAM_CHIP_LOGIC_INDEX_WINBOND_3_CMD_W955D8MKY,
	.psram_amlogic_protocol_id	= PSRAM_CHIP_LOGIC_INDEX,

	.psram_test_function		= {0}, /* 2 uint8 */
	.psram_vendor_id			= 0,
	.psram_device_id			= 0,
	.psram_soc_drv				= 0,
	.psram_dram_drv				= 0,
	.psram_ac_timing0			= 0,
	.psram_ac_timing1			= 0,
	.psram_ac_timing2			= 0,
	.psram_mode_crtl			= 0,
	/* psram_frequency = 24, 64, 96, 128, 192, 256 */
	.psram_frequency			= 0xffff,//192,
	.psram_size					= 8,
	.psram_mode_crtl_bl33		= 0,
	.psram_pin_crtl				= 0,
	.psram_lcd_ctrl				= 0,
	.psram_pin_dq_in_delay		= {0}, /* 8 uint8 */
	.psram_pin_dq_out_delay		= {0}, /* 8 uint8 */
	.psram_pin_dq_out_oe_delay	= {0}, /* 8 uint8 */
	.psram_pin_dm_out_delay		= 0,
	.psram_pin_dm_out_oe_delay	= 0,
	.psram_pin_cs_out_delay		= 0,
	.psram_pin_dqsp_in_delay	= 0,
	.psram_pin_dqsn_in_delay	= 0,
	.psram_pin_ckp_out_delay	= 0,
	.psram_pin_ckn_out_delay	= 0,
	.psram_pin_rden_delay		= 0,
	.psram_bdlr_delay			= 0,
},
#else
	{0},
#endif
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
};

ddr_reg_t __ddr_reg[] = {
	/* demo, user defined override register */
	{0xaabbccdd, 0, 0, 0, 0, 0},
	{0x11223344, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0},
};

#define VDDCORE_VAL		AML_VDDCORE_INIT_VOLTAGE
/* If AML_VDDCORE_INIT_VOLTAGE_SEL is 1, the voltage of vddcore
 * will be controlled by efuse. if 0, it is controlled by
 * AML_VDDCORE_INIT_VOLTAGE
 */
#define VDD_VAL_SEL		AML_VDDCORE_INIT_VOLTAGE_SEL
#define BL2_EE_ADD      AML_VDDCORE_INIT_EFUSE_MARGIN       /* margin */
#define FT_VMIN_OFFSET  AML_VDDCORE_INIT_EFUSE_OFFSET    /*vmin efuse offset*/
#define FT_BASE_VOLT    AML_VDDCORE_INIT_EFUSE_BASE_V0LT

/* for PWM use */
/* PWM driver check http://scgit.amlogic.com:8080/#/c/38093/ */
#define GPIO_O_EN_N_REG3	((0xff634400 + (0x19 << 2)))
#define GPIO_O_REG3		((0xff634400 + (0x1a << 2)))
#define GPIO_I_REG3		((0xff634400 + (0x1b << 2)))
#define AO_PIN_MUX_REG0	((0xff800000 + (0x05 << 2)))
#define AO_PIN_MUX_REG1	((0xff800000 + (0x06 << 2)))

#define P_PADCTRL_PIN_MUX_REG7	0xfe00041c
#define P_PADCTRL_GPIOF_DS      0xfe000554
#define CLK_PWM_CLK_EF_CTRL 	((0x0033  << 2) + 0xfe000800)
#define P_PWMEF_MISC_REG_EF     0xfe005408
#define	VDDCORE_TABLE_END		0XFFFFFFFF
#define P_PWMEF_PWM_F           0xfe005404


bl2_reg_t __bl2_reg[] = {
	/* demo, user defined override register */
	{0,        0,            0xffffffff,   0, 0, 0},

	/* power key */
	/* step1: CHK HW */
	{(uint64_t)PADCTRL_GPIOF_I,  9,            0,            0, BL2_INIT_STAGE_POWERKEY_CHK_HW,           0},

	/* step2: match power key config */
	/* GPIOF[BIT9]=0 power key press, =1 power key up */
	{0x0,                 POWERKEY_CFG0,                0,            0, BL2_INIT_STAGE_POWERKEY_CFG_GROUP,        0},
	{0x1,                 POWERKEY_CFG1,                0,            0, BL2_INIT_STAGE_POWERKEY_CFG_GROUP,        0},

	/* step3: config power key */
	/* power key press config*/
	{PADCTRL_PIN_MUX_REG7,        0,          0xf << 12,   0, BL2_INIT_STAGE_POWERKEY_INIT | POWERKEY_CFG0,  0},
	{PADCTRL_GPIOF_OEN,  0,  1 << 11, 0, BL2_INIT_STAGE_POWERKEY_INIT | POWERKEY_CFG0,  0},
	{PADCTRL_GPIOF_O,     1 << 11,               0,  0, BL2_INIT_STAGE_POWERKEY_INIT | POWERKEY_CFG0,  0},
	/* power key up config*/

	/* vddcore voltage init, controled by pwm f,
	 * Do not initialize the pwm duty register here,
	 * initialize in bl2 according to efuse
	 */
	{P_PWMEF_MISC_REG_EF,	((1 << 23) | (1 << 1)), 	(0x7f << 16), 0, BL2_INIT_STAGE_1, 0},
	{CLK_PWM_CLK_EF_CTRL,	1 << 24 , 	((0x3 << 25) | (0xff << 16)), 0, BL2_INIT_STAGE_1, 0},
	{P_PADCTRL_GPIOF_DS,	0x3 << 24, 		0x3 << 24,	0, BL2_INIT_STAGE_1, 0},
	{P_PADCTRL_PIN_MUX_REG7,(0x2 << 16),		(0xf << 16),	0, BL2_INIT_STAGE_1, 0},

	/* Transfer voltage table temporarily use __bl2_reg to transfer */
	{0x00000022,	1041,	0x00010021,	1031,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x00020020,	1021,	0x0003001f,	1011,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x0004001e,	1001,	0x0005001d,	990,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x0006001c,	980,	0x0007001b,	970,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x0008001a,	960,	0x00090019,	950,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x000a0018,	940,	0x000b0017,	930,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x000c0016,	920,	0x000d0015,	910,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x000e0014,	900,	0x000f0013,	890,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x00100012,	880,	0x00110011,	870,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x00120010,	860,	0x0013000f,	850,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x0014000e,	840,	0x0015000d,	830,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x0016000c,	820,	0x0017000b,	810,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x0018000a,	800,	0x00190009,	790,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x001a0008,	780,	0x001b0007,	770,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x001c0006,	760,	0x001d0005,	750,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x001e0004,	740,	0x001f0003,	730,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	{0x00200002,	720,	0x00210001,	710,	BL2_INIT_STAGE_VDDCORE_TABLE, 0},
	/* If there are invalid parameters at the end, use VDDCORE_TABLE_END to fill */
	{0x00220000,	700,	VDDCORE_TABLE_END,	0,		BL2_INIT_STAGE_VDDCORE_TABLE, 0},

	/* Transfer customer voltage, pwm duty register addr, sel flag */
	{P_PWMEF_PWM_F, VDDCORE_VAL, VDD_VAL_SEL, 0, BL2_INIT_STAGE_VDDCORE_CONFIG, 0},
	{BL2_EE_ADD, FT_VMIN_OFFSET, FT_BASE_VOLT, 0, BL2_INIT_STAGE_VDDCORE_CONFIG_1, 0},
};
