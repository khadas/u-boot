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
 *
 */
#include "ddr_timing.c"

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
