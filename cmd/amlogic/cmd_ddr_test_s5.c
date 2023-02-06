// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>

#define USE_FOR_UBOOT_2018
//#define USE_FOR_UBOOT_2015
#define GET_CHIP_ID

#ifdef USE_FOR_UBOOT_2018
#define  DISABLE_ENV
#define USE_FOR_UBOOT_2018
#include <amlogic/cpu_id.h>
#endif
#ifdef USE_FOR_UBOOT_2015
#include <asm/cpu_id.h>
#endif

#define ENABLE_OLD_EXTRA_TEST_CMD  1
#define ENABLE_G12_PHY_TEST_CMD  1

//uint32_t  do_read_s5_ddr_bdlr_steps(void);

struct ddr_base_address_table {
	char		soc_family_name[16];
	unsigned int	chip_id;
	unsigned int	preg_sticky_reg0;
	unsigned int	ddr_phy_base_address;
	unsigned int	ddr_pctl_timing_base_address;
	unsigned int	ddr_pctl_timing_end_address;
	unsigned int	ddr_dmc_sticky0;
	unsigned int	sys_watchdog_add;
	unsigned int	sys_watchdog_enable_value;
	unsigned int	ddr_pll_base_address;
	unsigned int	ee_timer_base_address;
	unsigned int	ee_pwm_base_address;
	unsigned int	ddr_dmc_apd_address;
	unsigned int	ddr_dmc_asr_address;
	unsigned int	ddr_boot_reason_address;
	unsigned int	ddr_dmc_lpdd4_retraining_address;
	unsigned int	ddr_dmc_refresh_ctrl_address;

	unsigned int	ddr_dmc_sticky0_1;
	unsigned int	ddr_phy_base_address_1;
	unsigned int	ddr_pctl_timing_base_address_1;
	unsigned int	ddr_pctl_timing_end_address_1;
	unsigned int	ddr_dmc_apd_address_1;
	unsigned int	ddr_dmc_asr_address_1;
	unsigned int	ddr_dmc_lpdd4_retraining_address_1;
	unsigned int	ddr_dmc_refresh_ctrl_address_1;
};

typedef struct  ddr_base_address_table ddr_base_address_table_t;

#define MESON_CPU_MAJOR_ID_GXBB         0x1F
#define MESON_CPU_MAJOR_ID_GXTVBB       0x20
#define MESON_CPU_MAJOR_ID_GXLBB        0x21
#define MESON_CPU_MAJOR_ID_GXM          0x22
#define MESON_CPU_MAJOR_ID_TXL          0x23
#define MESON_CPU_MAJOR_ID_TXLX         0x24
#define MESON_CPU_MAJOR_ID_AXG          0x25
#define MESON_CPU_MAJOR_ID_GXLX         0x26
#define MESON_CPU_MAJOR_ID_TXHD         0x27
#define MESON_CPU_MAJOR_ID_G12A         0x28
#define MESON_CPU_MAJOR_ID_G12B         0x29

#define MESON_CPU_MAJOR_ID_SM1          0x2B

#define MESON_CPU_MAJOR_ID_A1           0x2C

#define MESON_CPU_MAJOR_ID_TL1          0x2E
#define MESON_CPU_MAJOR_ID_TM2          0x2F
#define MESON_CPU_MAJOR_ID_C1           0x30
#define MESON_CPU_MAJOR_ID_SC2          0x32
#define MESON_CPU_MAJOR_ID_C2           0x33
#define MESON_CPU_MAJOR_ID_T5           0x34
#define MESON_CPU_MAJOR_ID_T5D          0x35
#define MESON_CPU_MAJOR_ID_T7                   0x36
#define MESON_CPU_MAJOR_ID_S4                   0x37
#define MESON_CPU_MAJOR_ID_T3                   0x38
#define MESON_CPU_MAJOR_ID_P1                   0x39
#undef  MESON_CPU_MAJOR_ID_S4D
#define MESON_CPU_MAJOR_ID_S4D                  0x3a
#define MESON_CPU_MAJOR_ID_A5                   0x3c
#define DDR_MESON_CPU_MAJOR_ID_C3                   0x3d
#ifdef MESON_CPU_MAJOR_ID_S5
#undef MESON_CPU_MAJOR_ID_S5
#define MESON_CPU_MAJOR_ID_S5                   0x3e
#endif
#define MESON_CPU_VERSION_LVL_MAJOR     0
#define MESON_CPU_VERSION_LVL_MINOR     1
#define MESON_CPU_VERSION_LVL_PACK      2
#define MESON_CPU_VERSION_LVL_MISC      3
#define MESON_CPU_VERSION_LVL_MAX       MESON_CPU_VERSION_LVL_MISC

#define CHIP_ID_MASK  0xff
char global_chip_id[12] = { 0 };

#define MESON_CPU_CHIP_ID_SIZE    12 //4//12byte
int ddr_get_chip_id(void)
{
	int soc_family_id = 0;

	soc_family_id = get_cpu_id().family_id;
	printf("\nsoc_family_id=0x%8x", soc_family_id);

	unsigned char chipid[16];

#ifdef GET_CHIP_ID
	get_chip_id(chipid, 16);
#endif
	int count = 0;

	for (count = 0; count < 16; count++)
		if (count > 3)
			global_chip_id[16 - 1 - count] = chipid[count];

	return soc_family_id;
}

char CMD_VER[] = "Ver_14";
ddr_base_address_table_t __ddr_base_address_table[] = {
	//g12a
	{
		.soc_family_name = "G12A",
		.chip_id = MESON_CPU_MAJOR_ID_G12A,
		.preg_sticky_reg0 = (0xff634400 + (0x070 << 2)), //PREG_STICKY_G12A_REG0
		.ddr_phy_base_address = 0xfe000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xff638400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xff638400),
		.ddr_dmc_sticky0 = 0xff638800,
		.sys_watchdog_add = ((0x3c34 << 2) + 0xffd00000),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xff638c00),
		.ee_timer_base_address = ((0x3c62 << 2) + 0xffd00000),
		.ee_pwm_base_address = (0xff807000 + (0x001 << 2)),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xff638400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xff638400),
		.ddr_boot_reason_address = (0xff800000 + (0x08f << 2)), //AO_SEC_SD_CFG15
		.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xff638400),
	},
	//g12b
	{
		.soc_family_name = "G12B",
		.chip_id = MESON_CPU_MAJOR_ID_G12B,
		.preg_sticky_reg0 = (0xff634400 + (0x070 << 2)), //PREG_STICKY_G12A_REG0
		.ddr_phy_base_address = 0xfe000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xff638400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xff638400),
		.ddr_dmc_sticky0 = 0xff638800,
		.sys_watchdog_add = ((0x3c34 << 2) + 0xffd00000),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xff638c00),
		.ee_timer_base_address = ((0x3c62 << 2) + 0xffd00000),
		.ee_pwm_base_address = (0xff807000 + (0x001 << 2)),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xff638400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xff638400),
		.ddr_boot_reason_address = (0xff800000 + (0x08f << 2)), //AO_SEC_SD_CFG15
		.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xff638400),
	},
	//tl1
	{
		.soc_family_name = "TL1",
		.chip_id = MESON_CPU_MAJOR_ID_TL1,
		.preg_sticky_reg0 = (0xff634400 + (0x070 << 2)), //PREG_STICKY_G12A_REG0
		.ddr_phy_base_address = 0xfe000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xff638400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xff638400),
		.ddr_dmc_sticky0 = 0xff638800,
		.sys_watchdog_add = ((0x3c34 << 2) + 0xffd00000),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xff638c00),
		.ee_timer_base_address = ((0x3c62 << 2) + 0xffd00000),
		.ee_pwm_base_address = (0xff807000 + (0x001 << 2)),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xff638400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xff638400),
		.ddr_boot_reason_address = (0xff800000 + (0x08f << 2)), //AO_SEC_SD_CFG15
		.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xff638400),
	},
	//sm1
	{
		.soc_family_name = "SM1",
		.chip_id = MESON_CPU_MAJOR_ID_SM1,
		.preg_sticky_reg0 = (0xff634400 + (0x070 << 2)), //PREG_STICKY_G12A_REG0
		.ddr_phy_base_address = 0xfe000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xff638400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xff638400),
		.ddr_dmc_sticky0 = 0xff638800,
		.sys_watchdog_add = ((0x3c34 << 2) + 0xffd00000),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xff638c00),
		.ee_timer_base_address = ((0x3c62 << 2) + 0xffd00000),
		.ee_pwm_base_address = (0xff807000 + (0x001 << 2)),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xff638400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xff638400),
		.ddr_boot_reason_address = (0xff800000 + (0x08f << 2)), //AO_SEC_SD_CFG15
		.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xff638400),
	},
	//tm2
	{
		.soc_family_name = "TM2",
		.chip_id = MESON_CPU_MAJOR_ID_TM2,
		.preg_sticky_reg0 = (0xff634400 + (0x070 << 2)), //PREG_STICKY_G12A_REG0
		.ddr_phy_base_address = 0xfe000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xff638400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xff638400),
		.ddr_dmc_sticky0 = 0xff638800,
		.sys_watchdog_add = ((0x3c34 << 2) + 0xffd00000),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xff638c00),
		.ee_timer_base_address = ((0x3c62 << 2) + 0xffd00000),
		.ee_pwm_base_address = (0xff807000 + (0x001 << 2)),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xff638400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xff638400),
		.ddr_boot_reason_address = (0xff800000 + (0x08f << 2)), //AO_SEC_SD_CFG15
		.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xff638400),
	},
	//a1
	{
		.soc_family_name = "A1",
		.chip_id = MESON_CPU_MAJOR_ID_A1,
		.preg_sticky_reg0 = 0xfffff400,
		//use sram  A1,((0x00b0  << 2) + 0xfe005800),
		//SYSCTRL_STICKY_REG0
		.ddr_phy_base_address = 0xfc000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xfd020400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xfd020400),
		.ddr_dmc_sticky0 = 0xfd020800,
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xfd020400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xfd020400),
		.sys_watchdog_add = 0,
		//((0x0040  << 2) + 0xfe000000),
		.sys_watchdog_enable_value = 0x03c401ff,
		.ddr_boot_reason_address = ((0x00e1 << 2) + 0xfe005800),
		//SYSCTRL_SEC_STICKY_REG1
		.ee_timer_base_address = ((0x0041 << 2) + 0xfe005800),
	},
	//c1
	{
		.soc_family_name = "C1",
		.chip_id = MESON_CPU_MAJOR_ID_C1,
		.preg_sticky_reg0 = 0xfffff400,
		//SYSCTRL_STICKY_REG0
		.ddr_phy_base_address = 0xfd000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xfe024400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xfe024400),
		.ddr_dmc_sticky0 = ((0x0000 << 2) + 0xfe024800),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xfe024c00),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xfe024400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xfe024400),
		.sys_watchdog_add = 0,
		//((0x0040  << 2) + 0xfe000000),
		.sys_watchdog_enable_value = 0x03c401ff,
		.ddr_boot_reason_address = ((0x00e1 << 2) + 0xfe005800),
		//SYSCTRL_SEC_STICKY_REG1
		.ee_timer_base_address = ((0x0041 << 2) + 0xfe005800),
	},
	//c2
	{
		.soc_family_name = "C2",
		.chip_id = MESON_CPU_MAJOR_ID_C2,
		.preg_sticky_reg0 = ((0x0000 << 2) + 0xfe024800),
		//use sram  A1,((0x00b0  << 2) + 0xfe005800),//SYSCTRL_STICKY_REG0
		.ddr_phy_base_address = 0xfd000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xfe024400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xfe024400),
		.ddr_dmc_sticky0 = ((0x0000 << 2) + 0xfe024800),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xfe024c00),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xfe024400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xfe024400),
		.sys_watchdog_add = 0,
		//((0x0040  << 2) + 0xfe000000),
		.sys_watchdog_enable_value = 0x03c401ff,
		.ddr_boot_reason_address = ((0x00e1 << 2) + 0xfe005800),
		//SYSCTRL_SEC_STICKY_REG1
		.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xfe024400),
		.ddr_dmc_refresh_ctrl_address = ((0x0092 << 2) + 0xfe024400),
		//DMC_DRAM_REFR_CTRL
		.ee_timer_base_address = ((0x0041 << 2) + 0xfe005800),
	},
	//sc2
	{
		.soc_family_name = "SC2",
		.chip_id = MESON_CPU_MAJOR_ID_SC2,
		.preg_sticky_reg0 = ((0x0000 << 2) + 0xfe036800), //
		.ddr_phy_base_address = 0xfc000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xfe036400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xfe036400),
		.ddr_dmc_sticky0 = 0xfe036800,
		.sys_watchdog_add = ((0x3c34 << 2) + 0xffd00000),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xfe036c00),
		.ee_timer_base_address = ((0x003b << 2) + 0xfe010000),
		.ee_pwm_base_address = ((0x0001 << 2) + 0xfe05e000),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xfe036400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xfe036400),
		//.ddr_boot_reason_address = ((0x00e1 << 2) + 0xfe010000),
		//SYSCTRL_SEC_STICKY_REG1,20210204,0xfe010384, confirm
	},
	//T5
	{
		.soc_family_name = "T5",
		.chip_id = MESON_CPU_MAJOR_ID_T5,
		.preg_sticky_reg0 = ((0x0000 << 2) + 0xff638800),
		.ddr_phy_base_address = 0xfe000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xff638400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xff638400),
		.ddr_dmc_sticky0 = ((0x0000 << 2) + 0xff638800),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xff638c00),
		.ddr_boot_reason_address = (0xff800000 + (0x003 << 2)),
		//#define SEC_AO_RTI_STATUS_REG3
		//.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xfe024400),

		.sys_watchdog_add = 0,
		.sys_watchdog_enable_value = 0x03c401ff,
		.ee_timer_base_address = 0xffd0f188,
		//#define P_EE_TIMER_E		( uint32_t *)0xffd0f188
		.ee_pwm_base_address = ((0x001 << 2) + 0xff807000),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xff638400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xff638400),
		.ddr_dmc_refresh_ctrl_address = ((0x0092 << 2) + 0xff638400),
		// DMC_DRAM_REFR_CTRL ((0x0092 << 2) + 0xff638400)
	},
	//T5D
	{
		.soc_family_name = "T5D",
		.chip_id = MESON_CPU_MAJOR_ID_T5D,
		.preg_sticky_reg0 = ((0x0000 << 2) + 0xff638800),
		.ddr_phy_base_address = 0xfe000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xff638400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xff638400),
		.ddr_dmc_sticky0 = ((0x0000 << 2) + 0xff638800),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xff638c00),
		.ddr_boot_reason_address = (0xff800000 + (0x003 << 2)),
		//#define SEC_AO_RTI_STATUS_REG3
		//.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xfe024400),

		.sys_watchdog_add = 0,
		.sys_watchdog_enable_value = 0x03c401ff,
		.ee_timer_base_address = 0xffd0f188,
		//#define P_EE_TIMER_E		( uint32_t *)0xffd0f188
		.ee_pwm_base_address = ((0x001 << 2) + 0xff807000),
		//AO_PWM_PWM_B
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xff638400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xff638400),
		.ddr_dmc_refresh_ctrl_address = ((0x0092 << 2) + 0xff638400),
		// DMC_DRAM_REFR_CTRL ((0x0092 << 2) + 0xff638400)
	},
	//T7
	{
		.soc_family_name = "T7",
		.chip_id = MESON_CPU_MAJOR_ID_T7,
		.preg_sticky_reg0 = ((0x0000 << 2) + 0xfe036800), //
		.ddr_phy_base_address = 0xfc000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xfe036400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xfe036400),
		.ddr_dmc_sticky0 = ((0x200 << 2) + 0xfe036000),
		.sys_watchdog_add = ((0x3c34 << 2) + 0xffd00000),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xfe036c00),
		.ee_timer_base_address = ((0x003b << 2) + 0xfe010000),
		.ee_pwm_base_address = ((0x0001 << 2) + 0xfe05e000),
		.ddr_dmc_apd_address = ((0x018c << 2) + 0xfe036000),
		.ddr_dmc_asr_address = ((0x018d << 2) + 0xfe036000),
		//.ddr_boot_reason_address = ((0x00c1 << 2) + 0xfe010000),
		//SYSCTRL_SEC_STATUS_REG1,20210204,0xfe010304, confirm

		.ddr_dmc_lpdd4_retraining_address = ((0x0197 << 2) + 0xfe036000),
		.ddr_dmc_refresh_ctrl_address = ((0x0192 << 2) + 0xfe036000),

		.ddr_dmc_sticky0_1 = ((0x200 << 2) + 0xfe034000),
		.ddr_dmc_refresh_ctrl_address_1 = ((0x0192 << 2) + 0xfe036000),
		.ddr_phy_base_address_1 = 0xfb000000,
		.ddr_pctl_timing_base_address_1 = ((0x0000 << 2) + 0xfe034400),
		.ddr_pctl_timing_end_address_1 = ((0x00bb << 2) + 0xfe034400),
		.ddr_dmc_apd_address_1 = ((0x018c << 2) + 0xfe034000),
		.ddr_dmc_asr_address_1 = ((0x018d << 2) + 0xfe034000),
		.ddr_dmc_lpdd4_retraining_address_1 = ((0x0197 << 2) + 0xfe034000),
		.ddr_dmc_refresh_ctrl_address_1 = ((0x0192 << 2) + 0xfe034000),
	},
	//S4
	{
		.soc_family_name = "S4",
		.chip_id = MESON_CPU_MAJOR_ID_S4,
		.preg_sticky_reg0 = ((0x0000 << 2) + 0xfe036800),
		.ddr_phy_base_address = 0xfc000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xfe036400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xfe036400),
		.ddr_dmc_sticky0 = ((0x0000 << 2) + 0xfe036800),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xfe036c00),
		//AM_DDR_PLL_CNTL0//
		//.ddr_boot_reason_address = ((0x00e1 << 2) + 0xfe010000),
		//SYSCTRL_SEC_STICKY_REG1,20210204,0xfe010384, confirm
		//.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xfe024400),

		.sys_watchdog_add = 0,
		.sys_watchdog_enable_value = 0x03c401ff,
		.ee_timer_base_address = ((0x003b << 2) + 0xfe010000),

		.ee_pwm_base_address = ((0x001 << 2) + 0xff807000),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xfe036400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xfe036400),
		.ddr_dmc_refresh_ctrl_address = ((0x0092 << 2) + 0xfe036400),
		// DMC_DRAM_REFR_CTRL ((0x0092 << 2) + 0xff638400)
	},
	//T3
	{
		.soc_family_name = "T3",
		.chip_id = MESON_CPU_MAJOR_ID_T3,
		.preg_sticky_reg0 = ((0x0000 << 2) + 0xfe036800),
		.ddr_phy_base_address = 0xfc000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xfe036400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xfe036400),
		.ddr_dmc_sticky0 = ((0x0000 << 2) + 0xfe036800),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xfe0a0000),
		//AM_DDR_PLL_CNTL0//
		//.ddr_boot_reason_address = ((0x00e1 << 2) + 0xfe010000),
		//SYSCTRL_SEC_STICKY_REG1,20210204,0xfe010384, confirm
		//.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xfe024400),

		.sys_watchdog_add = 0,
		.sys_watchdog_enable_value = 0x03c401ff,
		.ee_timer_base_address = ((0x003b << 2) + 0xfe010000),

		.ee_pwm_base_address = ((0x001 << 2) + 0xff807000),     //AO_PWM_PWM_B
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xfe036400),    //DMC_DRAM_APD_CTRL
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xfe036400),    //DMC_DRAM_ASR_CTRL
		.ddr_dmc_refresh_ctrl_address = ((0x0092 << 2) + 0xfe036400),
		// DMC_DRAM_REFR_CTRL ((0x0092 << 2) + 0xff638400)

		.ddr_dmc_sticky0_1 = ((0x200 << 2) + 0xfe034000),
		.ddr_dmc_refresh_ctrl_address_1 = ((0x0192 << 2) + 0xfe036000),
		.ddr_phy_base_address_1 = 0xfb000000,
		.ddr_pctl_timing_base_address_1 = ((0x0000 << 2) + 0xfe034400),
		.ddr_pctl_timing_end_address_1 = ((0x00bb << 2) + 0xfe034400),
		.ddr_dmc_apd_address_1 = ((0x018c << 2) + 0xfe034000),
		.ddr_dmc_asr_address_1 = ((0x018d << 2) + 0xfe034000),
		.ddr_dmc_lpdd4_retraining_address_1 = ((0x0197 << 2) + 0xfe034000),
		.ddr_dmc_refresh_ctrl_address_1 = ((0x0192 << 2) + 0xfe034000),
	},
	//P1
	{
		.soc_family_name = "P1",
		.chip_id = MESON_CPU_MAJOR_ID_P1,
		.preg_sticky_reg0 = ((0x0000 << 2) + 0xfe036800),
		.ddr_phy_base_address = 0xfc000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xfe036400),   //DMC_DRAM_TRFC
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xfe036400),    //DMC_DRAM_DFI
		.ddr_dmc_sticky0 = ((0x0000 << 2) + 0xfe036800),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xfe0a0000),
		//AM_DDR_PLL_CNTL0//
		//.ddr_boot_reason_address = ((0x00e1 << 2) + 0xfe010000),
		//SYSCTRL_SEC_STICKY_REG1,20210204,0xfe010384, confirm
		//.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xfe024400),

		.sys_watchdog_add = 0,
		.sys_watchdog_enable_value = 0x03c401ff,
		.ee_timer_base_address = ((0x003b << 2) + 0xfe010000),
		//SYSCTRL_TIMERE                             ((0x003b  << 2) + 0xfe010000)
		.ee_pwm_base_address = ((0x001 << 2) + 0xff807000),     //AO_PWM_PWM_B
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xfe036400),    //DMC_DRAM_APD_CTRL
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xfe036400),    //DMC_DRAM_ASR_CTRL
		.ddr_dmc_refresh_ctrl_address = ((0x0092 << 2) + 0xfe036400),
		// DMC_DRAM_REFR_CTRL ((0x0092 << 2) + 0xff638400)

		.ddr_dmc_sticky0_1 = ((0x200 << 2) + 0xfe034000),
		.ddr_dmc_refresh_ctrl_address_1 = ((0x0192 << 2) + 0xfe036000),
		.ddr_phy_base_address_1 = 0xfb000000,
		.ddr_pctl_timing_base_address_1 = ((0x0000 << 2) + 0xfe034400),
		.ddr_pctl_timing_end_address_1 = ((0x00bb << 2) + 0xfe034400),
		.ddr_dmc_apd_address_1 = ((0x018c << 2) + 0xfe034000),
		.ddr_dmc_asr_address_1 = ((0x018d << 2) + 0xfe034000),
		.ddr_dmc_lpdd4_retraining_address_1 = ((0x0197 << 2) + 0xfe034000),
		.ddr_dmc_refresh_ctrl_address_1 = ((0x0192 << 2) + 0xfe034000),
	},
//S5
	{
		.soc_family_name = "S5",
		.chip_id = MESON_CPU_MAJOR_ID_S5,
		.preg_sticky_reg0 = ((0x0000 << 2) + 0xfe036800),
		.ddr_phy_base_address = 0xfc000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xfe036400),   //DMC_DRAM_TRFC
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xfe036400),    //DMC_DRAM_DFI
		.ddr_dmc_sticky0 = ((0x0000 << 2) + 0xfe036800),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xfc0e0000),
		//AM_DDR_PLL_CNTL0//
		//.ddr_boot_reason_address = ((0x00e1 << 2) + 0xfe010000),
		//SYSCTRL_SEC_STICKY_REG1,20210204,0xfe010384, confirm
		//.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xfe024400),

		.sys_watchdog_add = 0,
		.sys_watchdog_enable_value = 0x03c401ff,
		.ee_timer_base_address = ((0x003b << 2) + 0xfe010000),
		//SYSCTRL_TIMERE                             ((0x003b  << 2) + 0xfe010000)
		.ee_pwm_base_address = ((0x001 << 2) + 0xff807000),     //AO_PWM_PWM_B
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xfe036400),    //DMC_DRAM_APD_CTRL
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xfe036400),    //DMC_DRAM_ASR_CTRL
		.ddr_dmc_refresh_ctrl_address = ((0x0092 << 2) + 0xfe036400),
		// DMC_DRAM_REFR_CTRL ((0x0092 << 2) + 0xff638400)

		.ddr_dmc_sticky0_1 = ((0x200 << 2) + 0xfe034000),
		.ddr_dmc_refresh_ctrl_address_1 = ((0x0192 << 2) + 0xfe036000),
		.ddr_phy_base_address_1 = 0xfb000000,
		.ddr_pctl_timing_base_address_1 = ((0x0000 << 2) + 0xfe034400),
		.ddr_pctl_timing_end_address_1 = ((0x00bb << 2) + 0xfe034400),
		.ddr_dmc_apd_address_1 = ((0x018c << 2) + 0xfe034000),
		.ddr_dmc_asr_address_1 = ((0x018d << 2) + 0xfe034000),
		.ddr_dmc_lpdd4_retraining_address_1 = ((0x0197 << 2) + 0xfe034000),
		.ddr_dmc_refresh_ctrl_address_1 = ((0x0192 << 2) + 0xfe034000),
	},
	// force id use id mask
	{
		.soc_family_name = "UNKNOWN",
		.chip_id = CHIP_ID_MASK,                                //MESON_CPU_MAJOR_ID_G12A,
		.preg_sticky_reg0 = (0xff634400 + (0x070 << 2)),        //PREG_STICKY_G12A_REG0
		.ddr_phy_base_address = 0xfe000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xff638400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xff638400),
		.ddr_dmc_sticky0 = 0xff638800,
		.sys_watchdog_add = ((0x3c34 << 2) + 0xffd00000),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xff638c00),
		.ee_timer_base_address = ((0x3c62 << 2) + 0xffd00000),
		.ee_pwm_base_address = (0xff807000 + (0x001 << 2)),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xff638400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xff638400),
		.ddr_boot_reason_address = (0xff800000 + (0x08f << 2)), //AO_SEC_SD_CFG15
		.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xff638400),
	},
};

ddr_base_address_table_t *p_ddr_base = { 0 };
unsigned int phy_base_array_add[4] = { 0, 0, };
unsigned int pll_base_array_add[4] = { 0, 0, };
unsigned int dmc_base_array_add[4] = { 0, 0, };

unsigned int ddr_dmc_sticky[4] = { 0, 0, };
unsigned int ddr_dmc_apd_address[4] = { 0, 0, };
unsigned int ddr_dmc_asr_address[4] = { 0, 0, };
unsigned int dmc_retraining_ctrl_address[4] = { 0, 0, };

char dmc_enable_nums;
uint32_t ddr_pll_base_add;
uint32_t dmc_base_add;
uint32_t phy_base_add;
// 3'b000 : 64Byte. 3'b001 : 128Byte.//   3'b010 : 256Byte.
uint8_t dmc_ddr_channel_interleave_size_id;
// 3'b1 : only 1. 3'b010 : 2 dmc.//   3'b011 : 4 dmc.
uint8_t dmc_ddr_channel_strip_mode;
uint8_t dmc_ddr_config_channel_id;

//#ifdef CONFIG_ENV_IS_NOWHERE
#ifdef DISABLE_ENV
int setenv(const char *varname, const char *varvalue)
{
	return 1;
}

char *getenv(const char *name)
{
	return NULL;
}

#endif

#define REGISTER_READ_SKIP_FLAG (0xffff)
#define REGISTER_READ 1
#define REGISTER_WRITE 0
#define TEST_MIN 0
#define TEST_MAX 1

#define DDR_PRINT_DISABLE 0
#define DDR_PRINT_ENABLE 1

#define DDR_AMLOGIC_AC_PINMUX_TOTAL  11
//#define DMC_TEST_WINDOW_INDEX_ATXDLY 1                                  //cmd 0x1
//#define DMC_TEST_WINDOW_INDEX_TXDQSDLY                          2       //cmd 0x2
#define DMC_TEST_WINDOW_INDEX_WCK_CK                          3         //cmd 0x4
//#define DMC_TEST_WINDOW_INDEX_TXDQDLY                           4       //cmd 0x8
#define DMC_TEST_WINDOW_INDEX_RX_CLK_T_DLY                           5  //cmd 0x10
#define DMC_TEST_WINDOW_INDEX_RX_CLK_C_DLY                           6  //cmd 0x20
//#define DMC_TEST_WINDOW_INDEX_RXENDLY                           7       //cmd 0x40

#define CONFIG_CHIP_TYPE_DRAM                                     0x1F
#define CONFIG_CHIP_TYPE_PSRAM                                    0x2F
#define CONFIG_DDR_TYPE_DDR3                                      0
#define CONFIG_DDR_TYPE_DDR4                                      1
#define CONFIG_DDR_TYPE_LPDDR4                                    2
#define CONFIG_DDR_TYPE_LPDDR3                                    3
#define CONFIG_DDR_TYPE_LPDDR2                                    4
#define CONFIG_DDR_TYPE_LPDDR4X                                   5
#define CONFIG_DDR_TYPE_LPDDR5                                    6

#define CONFIG_DDR_TYPE_AUTO                                      0xf

#define DMC_TEST_SOC_VREF_DFE_DAC0   8                          //cmd 0x80  //p
#define DMC_TEST_SOC_VREF_DFE_DAC1   9                          //cmd 0x100  //n
#define DMC_TEST_SOC_VREF_DFE_DAC2   10                         //cmd 0x200  //p
#define DMC_TEST_SOC_VREF_DFE_DAC3   11                         //cmd 0x400  //n
#define DMC_TEST_SOC_VREF_DFE_DAC0_PB   12                      //cmd 0x80  //p
#define DMC_TEST_SOC_VREF_DFE_DAC1_PB   13                      //cmd 0x100  //n
#define DMC_TEST_SOC_VREF_DFE_DAC2_PB   14                      //cmd 0x200  //p
#define DMC_TEST_SOC_VREF_DFE_DAC3_PB   15                      //cmd 0x400  //n

#define DMC_TEST_WINDOW_INDEX_RX_DIGSTRB_DLY   16
//#define DMC_TEST_WINDOW_INDEX_DFIMRL                            13
//#define DMC_TEST_WINDOW_INDEX_RETRAINING                        14
//#define DMC_TEST_WINDOW_INDEX_EXTERA_COMMON                     15
//#define DMC_TEST_WINDOW_INDEX_EXTERA_PS                         16

#define DMC_TEST_WINDOW_INDEX_ATXDLY 1
#define DMC_TEST_WINDOW_INDEX_TXDQSDLY                          2
#define DMC_TEST_WINDOW_INDEX_RXCLKDLY                          3
#define DMC_TEST_WINDOW_INDEX_TXDQDLY                           4
#define DMC_TEST_WINDOW_INDEX_RXPBDLY                           5
#define DMC_TEST_WINDOW_INDEX_RXENDLY                           6
#define DMC_TEST_WINDOW_INDEX_RXPBDLY_2                         7
#define DMC_TEST_WINDOW_INDEX_DFIMRL                            8
//#define DMC_TEST_WINDOW_INDEX_VREF                              9
#define DMC_TEST_WINDOW_INDEX_RETRAINING                        10
#define DMC_TEST_WINDOW_INDEX_EXTERA_COMMON                     11
#define DMC_TEST_WINDOW_INDEX_EXTERA_PS                         12

#define DMC_TEST_WINDOW_INDEX_RXCLKDLY_DAC1                     12
#define DMC_TEST_WINDOW_INDEX_RXPBDLY_DAC1                      13
#define DMC_TEST_WINDOW_INDEX_RXPBDLY_2_DAC1                    14
#define DMC_TEST_WINDOW_INDEX_RXEN_WINDOW_DLY                   15

#define DMC_TEST_WINDOW_INDEX_EE_VOLTAGE                        0x11
//#define DMC_TEST_SOC_VREF_DFE_DAC0  0x12
#define WINDOW_TEST_SOC_VREF_DAC0  0x12
#define WINDOW_TEST_SOC_VREF_DAC1  0x13
#define WINDOW_TEST_SOC_VREF_DAC2  0x14
#define WINDOW_TEST_SOC_VREF_DAC3  0x15
#define DMC_TEST_WINDOW_INDEX_DRAM_VREF    0x16
#define DMC_TEST_WINDOW_INDEX_TXDQDLY_WINDOW   0x17
#define DMC_TEST_WINDOW_INDEX_RXDQDLY_WINDOW   0x18
#define DMC_TEST_WINDOW_INDEX_RXDQDBI   0x19
#define DMC_TEST_WINDOW_INDEX_RXDQDBI_WINDOW   0x1a
#define DMC_TEST_WINDOW_INDEX_WDQDBI   0x1b
#define DMC_TEST_WINDOW_INDEX_WDQDBI_WINDOW   0x1c
#define DMC_TEST_WINDOW_INDEX_RXDQDBI_VREF0   0x1d
#define DMC_TEST_WINDOW_INDEX_RXDQDBI_VREF1   0x1e

#define DMC_TEST_WINDOW_INDEX_ATXDLY_RO           (0x20 + DMC_TEST_WINDOW_INDEX_ATXDLY)
#define DMC_TEST_WINDOW_INDEX_TXDQSDLY_RO         (0x20 + DMC_TEST_WINDOW_INDEX_TXDQSDLY)
#define DMC_TEST_WINDOW_INDEX_RXCLKDLY_RO         (0x20 + DMC_TEST_WINDOW_INDEX_RXCLKDLY)
#define DMC_TEST_WINDOW_INDEX_TXDQDLY_RO          (0x20 + DMC_TEST_WINDOW_INDEX_TXDQDLY)
#define DMC_TEST_WINDOW_INDEX_RXPBDLY_RO          (0x20 + DMC_TEST_WINDOW_INDEX_RXPBDLY)
#define DMC_TEST_WINDOW_INDEX_RXENDLY_RO          (0x20 + DMC_TEST_WINDOW_INDEX_RXENDLY)
#define DMC_TEST_WINDOW_INDEX_RXEN_WINDOW_DLY_RO  (0x20 + DMC_TEST_WINDOW_INDEX_RXEN_WINDOW_DLY)
//#define DMC_TEST_SOC_VREF_DFE_DAC0_RO         (0x20 + DMC_TEST_SOC_VREF_DFE_DAC0)
#define WINDOW_TEST_SOC_VREF_DAC0_RO    (0x20 + WINDOW_TEST_SOC_VREF_DAC0)
#define WINDOW_TEST_SOC_VREF_DAC1_RO    (0x20 + WINDOW_TEST_SOC_VREF_DAC1)
#define WINDOW_TEST_SOC_VREF_DAC2_RO    (0x20 + WINDOW_TEST_SOC_VREF_DAC2)
#define WINDOW_TEST_SOC_VREF_DAC3_RO    (0x20 + WINDOW_TEST_SOC_VREF_DAC3)
#define DMC_TEST_WINDOW_INDEX_DRAM_VREF_RO        (0x20 + DMC_TEST_WINDOW_INDEX_DRAM_VREF)
#define DMC_TEST_WINDOW_INDEX_TXDQDLY_WINDOW_RO   (0x20 + DMC_TEST_WINDOW_INDEX_TXDQDLY_WINDOW)
#define DMC_TEST_WINDOW_INDEX_RXDQDLY_WINDOW_RO   (0x20 + DMC_TEST_WINDOW_INDEX_RXDQDLY_WINDOW)
#define DMC_TEST_WINDOW_INDEX_RXDQDBI_RO   (0x20 + DMC_TEST_WINDOW_INDEX_RXDQDBI)
#define DMC_TEST_WINDOW_INDEX_RXDQDBI_WINDOW_RO   (0x20 + DMC_TEST_WINDOW_INDEX_RXDQDBI_WINDOW)
#define DMC_TEST_WINDOW_INDEX_WDQDBI_RO   (0x20 + DMC_TEST_WINDOW_INDEX_WDQDBI)
#define DMC_TEST_WINDOW_INDEX_WDQDBI_WINDOW_RO   (0x20 + DMC_TEST_WINDOW_INDEX_WDQDBI_WINDOW)
#define DMC_TEST_WINDOW_INDEX_RXDQDBI_VREF0_RO   (0x20 + DMC_TEST_WINDOW_INDEX_RXDQDBI_VREF0)
#define DMC_TEST_WINDOW_INDEX_RXDQDBI_VREF1_RO   (0x20 + DMC_TEST_WINDOW_INDEX_RXDQDBI_VREF1)
//#define DMC_TEST_WINDOW_INDEX_RXCLKDLY_DAC1                     12
//#define DMC_TEST_WINDOW_INDEX_RXPBDLY_DAC1                      13
//#define DMC_TEST_WINDOW_INDEX_RXPBDLY_2_DAC1                    14

//#define DMC_TEST_WINDOW_INDEX_EE_VOLTAGE                        17//0x11
//#define DMC_TEST_SOC_VREF_DFE_DAC0  0x12
//#define WINDOW_TEST_SOC_VREF_DAC1  0x13
//#define DMC_TEST_WINDOW_INDEX_DRAM_VREF    18

#define DMC_TEST_WINDOW_INDEX_DDR3_WRITE_VREF_RANG0             0x21
#define DMC_TEST_WINDOW_INDEX_DDR3_WRITE_VREF_RANG1             0x22
#define DMC_TEST_WINDOW_INDEX_DDR3_READ_VREF_RANG0              0x23
#define DMC_TEST_WINDOW_INDEX_DDR3_READ_VREF_RANG1              0x24

#define DMC_TEST_WINDOW_INDEX_DDR4_WRITE_VREF_RANG0             0x31
#define DMC_TEST_WINDOW_INDEX_DDR4_WRITE_VREF_RANG1             0x32
#define DMC_TEST_WINDOW_INDEX_DDR4_READ_VREF_RANG0              0x33
#define DMC_TEST_WINDOW_INDEX_DDR4_READ_VREF_RANG1              0x34

#define DMC_TEST_WINDOW_INDEX_LPDDR4_WRITE_VREF_RANG0           0x41
#define DMC_TEST_WINDOW_INDEX_LPDDR4_WRITE_VREF_RANG1           0x42
#define DMC_TEST_WINDOW_INDEX_LPDDR4_READ_VREF_RANG0            0x43
#define DMC_TEST_WINDOW_INDEX_LPDDR4_READ_VREF_RANG1            0x44

#define DMC_TEST_WINDOW_INDEX_LPDDR3_WRITE_VREF_RANG0           0x51
#define DMC_TEST_WINDOW_INDEX_LPDDR3_WRITE_VREF_RANG1           0x52
#define DMC_TEST_WINDOW_INDEX_LPDDR3_READ_VREF_RANG0            0x53
#define DMC_TEST_WINDOW_INDEX_LPDDR3_READ_VREF_RANG1            0x54

#define DMC_TEST_WINDOW_RANGE_TXDQDLY  0x61
#define DMC_TEST_WINDOW_RANGE_RXPBDLY  0x62
#define DMC_TEST_WINDOW_RANGE_RXENDLY  0x63

#define DMC_TEST_SOC_VREF_DFE_DAC0_T   0x70
//#define DMC_TEST_SOC_VREF_DFE_DAC1_T   0x74
#define DMC_TEST_SOC_VREF_DFE_DAC2_T   0x71
//#define DMC_TEST_SOC_VREF_DFE_DAC3_T   0x75
//#define DMC_TEST_SOC_VREF_DFE_DAC0_C   0x76
#define DMC_TEST_SOC_VREF_DFE_DAC1_C   0x72
//#define DMC_TEST_SOC_VREF_DFE_DAC2_C   0x77
#define DMC_TEST_SOC_VREF_DFE_DAC3_C   0x73
#define DMC_TEST_WINDOW_INDEX_SOC_DAC_MAX_NUM   4

#define DMC_TEST_WINDOW_INDEX_DCA_SOC_WCK_TX_T   0x79
#define DMC_TEST_WINDOW_INDEX_DCA_SOC_WCK_TX_C    0x7a
#define DMC_TEST_WINDOW_INDEX_DCA_SOC_WCK_RX_T    0x7b
#define DMC_TEST_WINDOW_INDEX_DCA_SOC_WCK_RX_C   0x7c
#define DMC_TEST_WINDOW_INDEX_DCA_SOC_DQS_TX_T    0x7d
#define DMC_TEST_WINDOW_INDEX_DCA_SOC_DQS_TX_C    0x7e
#define DMC_TEST_WINDOW_INDEX_DCA_SOC_DQ_TX   0x7f
#define DMC_TEST_WINDOW_INDEX_HWTMRL   0x80
#define AML_AC_PINMUX_G0_TOTAL 6
#define AML_AC_PINMUX_G1_TOTAL 29
#define AML_DFI_PINMUX_TOTAL 26
#define AML_DQ_PINMUX_TOTAL 36
#define AML_DQ_BYTES_TOTAL 4
#define DWC_AC_PINMUX_TOTAL                                             30
#define DWC_DFI_PINMUX_TOTAL                                    10
#define DWC_DQ_PINMUX_TOTAL                                             32
#define DWC_TOTAL_CA_NUM_PER_CH  12
#define DWC_TOTAL_DATA_NIBBLE_PER_PHY  4
#define DWC_TOTAL_DATA_NIBBLE_PER_CH  2

#define DMC_WINDOW_CMD   20180010 //g12_d2pll 1584 0 0 0 0 0x8
#define G12_D2PLL_CMD_DMC_FULL_TEST   0x01
#define G12_D2PLL_CMD_OVER_RIDE   0x02
#define G12_D2PLL_CMD_OVER_RIDE_PLUS_FULLTEST  0x03
#define G12_D2PLL_CMD_OVER_RIDE_TRAINING_HDTL  0x04
#define G12_D2PLL_CMD_OFFSET 0x06
#define G12_D2PLL_CMD_WINDOW_TEST  0x11
#define G12_D2PLL_CMD_WINDOW_TEST_AND_STICKY_OVERRIDE  0x12
#define G12_D2PLL_CMD_SUSPEND_TEST  0x21
#define G12_D2PLL_CMD_SWEEP_EE_VOLTAGE_FREQUENCY_TABLE_TEST  0x32
#define G12_D2PLL_CMD_DDR_EYE_TEST  0x41
#define G12_D2PLL_CMD_DDR_EYE_TEST_AND_STICKY_OVERRIDE    0x42
#define G12_D2PLL_CMD_DDR_EYE_TEST_DAC1  0x43
#define G12_D2PLL_CMD_DDR_EYE_TEST_AND_STICKY_OVERRIDE_DAC1    0x44

#define G12_D2PLL_CMD_DDR_DVFS_TEST  0x51

#define DMC_TEST_WINDOW_INDEX_ATXDLY 1
#define DMC_TEST_WINDOW_INDEX_TXDQSDLY 2
#define DMC_TEST_WINDOW_INDEX_RXCLKDLY  3
#define DMC_TEST_WINDOW_INDEX_TXDQDLY  4
#define DMC_TEST_WINDOW_INDEX_RXPBDLY  5

#define DMC_TEST_WINDOW_INDEX_EE_VOLTAGE  0x11

typedef struct board_common_setting {
	unsigned int	timming_magic;
	unsigned short	timming_max_valid_configs;
	unsigned short	timming_struct_version;
	unsigned short	timming_struct_org_size;
	unsigned short	timming_struct_real_size;
	unsigned char	fast_boot[4];
	// 0   fastboot enable  1 window test margin
	// 2 auto offset after window test 3 auto window test enable
	unsigned int	ddr_func;
	unsigned char	board_id;
	unsigned char	DramType;
	//support DramType should confirm with amlogic
	//#define CONFIG_DDR_TYPE_DDR3				0
	//#define CONFIG_DDR_TYPE_DDR4				1
	//#define CONFIG_DDR_TYPE_LPDDR4				2
	//#define CONFIG_DDR_TYPE_LPDDR3				3
	//#define CONFIG_DDR_TYPE_LPDDR2				4
	//#define CONFIG_DDR_TYPE_LPDDR4x
	unsigned char dram_rank_config;
	//support Dram connection type should confirm with amlogic
	//#define CONFIG_DDR0_16BIT_CH0				0x1
	//dram total bus width 16bit only use cs0
	//#define CONFIG_DDR0_16BIT_RANK01_CH0		0x4
	//dram total bus width 16bit  use cs0 cs1
	//#define CONFIG_DDR0_32BIT_RANK0_CH0			0x2
	//dram total bus width 32bit  use cs0
	//#define CONFIG_DDR0_32BIT_RANK01_CH01		0x3
	//only for lpddr4,dram total bus width 32bit  use channel a cs0 cs1 channel b cs0 cs1
	//#define CONFIG_DDR0_32BIT_16BIT_RANK0_CH0		0x5
	//dram total bus width 32bit only use cs0,but high address use 16bit mode
	//#define CONFIG_DDR0_32BIT_16BIT_RANK01_CH0	0x6
	//dram total bus width 32bit  use cs0 cs1,but cs1 use 16bit mode ,
	//current phy not support reserve
	//#define CONFIG_DDR0_32BIT_RANK01_CH0		0x7
	//dram total bus width 32bit  use cs0 cs1
	//#define CONFIG_DDR0_32BIT_RANK0_CH01		0x8
	//only for lpddr4,dram total bus width 32bit  use channel a cs0  channel b cs0

	/* rsv_char0. update for diagnose type define */
	unsigned char	rsv_char0;

	unsigned char	DisabledDbyte[2];            //ch0 and ch1
	//use for dram bus 16bit or 32bit,if use 16bit mode ,should disable bit 2,3
	//bit 0 ---cs0 use byte 0 ,1 disable byte 0,
	//bit 1 ---cs0 use byte 1 ,1 disable byte 1,
	//bit 2 ---cs0 use byte 2 ,1 disable byte 2,
	//bit 3 ---cs0 use byte 3 ,1 disable byte 3,
	//bit 4 ---cs1 use byte 0 ,1 disable byte 0,
	//bit 5 ---cs1 use byte 1 ,1 disable byte 1,
	//bit 6 ---cs1 use byte 2 ,1 disable byte 2,
	//bit 7 ---cs1 use byte 3 ,1 disable byte 3,
	unsigned short	dram_ch0_size_MB;
	//config cs0 dram size ,like 1G DRAM ,setting 1024
	unsigned short	dram_ch1_size_MB; //
	//config cs1 dram size,like 512M DRAM ,setting 512
	/* align8 */

	unsigned char	dram_x4x8x16_mode;
	unsigned char	Is2Ttiming;
	unsigned char	log_level;
	unsigned char	dbi_enable;
	//system reserve,do not modify
	unsigned char	ddr_rfc_type;
	//config dram rfc type,according dram type,also can use same dram type max config
	//#define DDR_RFC_TYPE_DDR3_512Mbx1				0
	//#define DDR_RFC_TYPE_DDR3_512Mbx2				1
	//#define DDR_RFC_TYPE_DDR3_512Mbx4				2
	//#define DDR_RFC_TYPE_DDR3_512Mbx8				3
	//#define DDR_RFC_TYPE_DDR3_512Mbx16				4
	//#define DDR_RFC_TYPE_DDR4_2Gbx1					5
	//#define DDR_RFC_TYPE_DDR4_2Gbx2					6
	//#define DDR_RFC_TYPE_DDR4_2Gbx4					7
	//#define DDR_RFC_TYPE_DDR4_2Gbx8					8
	//#define DDR_RFC_TYPE_LPDDR4_2Gbx1				9
	//#define DDR_RFC_TYPE_LPDDR4_3Gbx1				10
	//#define DDR_RFC_TYPE_LPDDR4_4Gbx1				11
	unsigned char	enable_lpddr4x_mode;
	//system reserve,do not modify
	/* align8 */

	unsigned int	pll_ssc_mode;
	//
	//pll ssc config:
	//
	//  pll_ssc_mode = (1<<20) | (1<<8) | ([strength] << 4) | [mode],
	//     ppm = strength * 500
	//     mode: 0=center, 1=up, 2=down
	//
	//  eg:
	//    1. config 1000ppm center ss. then mode=0, strength=2
	//       .pll_ssc_mode = (1<<20) | (1<<8) | (2 << 4) | 0,
	//    2. config 3000ppm down ss. then mode=2, strength=6
	//       .pll_ssc_mode = (1<<20) | (1<<8) | (6 << 4) | 2,
	//
	unsigned short	org_tdqs2dq;
	unsigned char	reserve1_test[2];
	unsigned int	ddr_dmc_remap[5];
	unsigned char	lpddr34_ca_remap[4];
	unsigned char	ddr_dq_remap[36];
	unsigned char	ac_pinmux[DWC_AC_PINMUX_TOTAL]; //24 35
	unsigned char	dfi_pinmux[DWC_DFI_PINMUX_TOTAL];
	unsigned char	ddr_dqs_swap;
	unsigned char	rsv_char1;
	unsigned char	rsv_char2;
	unsigned char	rsv_char3;
	unsigned int	ddr_vddee_setting[4];   // add,default-value,default-voltage,
}  __packed board_common_setting_t;
typedef struct board_SI_setting_ps {
	unsigned short	DRAMFreq;
	unsigned char	PllBypassEn;
	unsigned char	training_SequenceCtrl;
	unsigned int	dfi_odt_config;
	//normal go status od config,use for normal status
	//bit 12.  rank1 ODT default. default value for ODT[1] pins if theres
	//no read/write activity.
	//bit 11.  rank1 ODT write sel.  enable ODT[1] if there's write  occurred in rank1.
	//bit 10.  rank1 ODT write nsel. enable ODT[1] if theres's write  occurred in rank0.
	//bit 9.   rank1 odt read sel.   enable ODT[1] if there's read  occurred in rank1.
	//bit 8.   rank1 odt read nsel.  enable ODT[1] if there's read  occurred in rank0.
	//bit 4.   rank0 ODT default.    default value for ODT[0] pins if
	//theres no read/write activity.
	//bit 3.   rank0 ODT write sel.  enable ODT[0] if there's write  occurred in rank0.
	//bit 2.   rank0 ODT write nsel. enable ODT[0] if theres's write  occurred in rank1.
	//bit 1.   rank0 odt read sel.   enable ODT[0] if there's read  occurred in rank0.
	//bit 0.   rank0 odt read nsel.  enable ODT[0] if there's read  occurred in rank1.
	unsigned char	phy_odt_config_rank[2];
	unsigned short	clk_drv_ohm;
	//config soc clk pin signal driver strength ,select 20,30,40,60ohm
	unsigned short	cs_drv_ohm;
	//config soc cs0 cs1 pin signal driver strength ,select 20,30,40,60ohm
	unsigned short	ac_drv_ohm;
	//config soc  normal address command pin driver strength ,select 20,30,40,60ohm
	unsigned short	soc_data_drv_ohm_p;
	//config soc data pin pull up driver strength,
	//select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short	soc_data_drv_ohm_n;
	//config soc data pin pull down driver strength,
	//select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short	soc_data_odt_ohm_p;
	//config soc data pin odt pull up strength,
	//select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short	soc_data_odt_ohm_n;
	//config soc data pin odt pull down strength,
	//select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short	dram_data_drv_ohm;
	//config dram data pin pull up pull down driver strength,
	//ddr3 select 34,40ohm,ddr4 select 34,48ohm,lpddr4 select 40,48,60,80,120,240ohm
	unsigned short	dram_data_odt_ohm;
	//config dram data pin odt pull up down strength,ddr3 select 40,60,120ohm,
	//ddr4 select 34,40,48,60,120,240ohm,lpddr4 select 40,48,60,80,120,240ohm
	unsigned short	dram_data_wr_odt_ohm;
	//config dram data pin odt pull up down strength,ddr3 select 40,60,120ohm,
	//ddr4 select 34,40,48,60,120,240ohm,lpddr4 select 40,48,60,80,120,240ohm
	unsigned short	dram_ac_odt_ohm;
	//config dram ac pin odt pull up down strength,use for lpddr4, select 40,48,60,80,120,240ohm
	unsigned short	soc_clk_slew_rate;
	//system reserve,do not modify
	unsigned short	soc_cs_slew_rate;
	//system reserve,do not modify
	unsigned short	soc_ac_slew_rate;
	//system reserve,do not modify
	unsigned short	soc_data_slew_rate;
	//system reserve,do not modify
	unsigned char	dram_drv_pull_up_cal_ohm;
	//config soc data pin odt pull up strength,select 40,60,80,120ohm
	unsigned char	lpddr4_dram_vout_range;
	//use for lpddr4 read vout voltage  setting 0 --->2/5VDDQ ,1--->1/3VDDQ
	unsigned char	char_rev0;                      //dfe_offset_value;       //char_rev1;
	unsigned char	char_rev1;                      //training_offset;        //char_rev2;

	unsigned short	vref_ac_permil;
	//soc init dram ac vref ,config like 500 means 0.5VDDQ,take care ,please follow SI
	unsigned short	vref_soc_data_permil;
	//soc init SOC receiver vref ,config like 500 means 0.5VDDQ,take care ,please follow SI
	unsigned short	vref_dram_data_permil;
	//soc init DRAM receiver vref ,config like 500 means 0.5VDDQ,take care ,please follow SI
	unsigned short	max_core_timmming_frequency;
	//use for limited ddr speed core timmming parameter,
	//for some old dram maybe have no over speed register
	//unsigned short training_phase_parameter[2];
	//unsigned short ac_trace_delay_org[36];
}  __packed board_SI_setting_ps_t;
typedef struct board_phase_setting_ps {
	unsigned short	ac_trace_delay[DWC_AC_PINMUX_TOTAL];
	//unsigned char ac_trace_delay_rev[5];
	unsigned short	read_dq_delay_t[72];
	unsigned short	read_dq_delay_c[72];
	unsigned short	read_dqs_delay[8];
	unsigned short	write_dqs_delay[8];
	unsigned short	write_wck_delay[8];
	unsigned short	wdq_delay[72];
	unsigned short	read_dqs_gate_delay[8];
	unsigned char	soc_bit_vref0[36];
	unsigned char	soc_bit_vref1[36];
	unsigned char	soc_bit_vref2[36];
	unsigned char	soc_bit_vref3[36];
	unsigned char	dram_vref[32];

	unsigned short	dca_wck_tx_t[8];        //t and c
	unsigned short	dca_wck_rx_t[8];        //t and c
	unsigned short	dca_dqs_tx_t[8];        //t and c
	unsigned short	dca_wck_tx_c[8];        //t and c
	unsigned short	dca_wck_rx_c[8];        //t and c
	unsigned short	dca_dqs_tx_c[8];        //t and c
	unsigned short	dca_dq_tx[8];           //common

	unsigned char	dfi_mrl[4];
	unsigned char	dfi_hwtmrl;
	unsigned char	csr_hwtctrl;
	unsigned char	rever1;
	unsigned char	rever2;
	//unsigned	char	ARdPtrInitVal;
	//unsigned	short	csr_vrefinglobal;
	//unsigned	short	csr_dqsrcvcntrl[4];
	unsigned short	pptdqscnttg0[4];
	unsigned short	pptdqscnttg1[4];
	unsigned short	PptWck2DqoCntTg0[4];
	unsigned short	PptWck2DqoCntTg1[4];
	unsigned short	RxReplicaPhase[4][5];
	//unsigned	short	csr_RxReplicaCtl03[4];
	//unsigned	short	csr_seq0bgpr[9];
	//unsigned	short	csr_dllgainctl;
	//unsigned	short	csr_dlllockpara;
	unsigned char	dac_offset[4];          //bit 7 offset direction 0 ++  1 --
	unsigned char	rx_offset[2];           //bit 7 offset direction 0 ++  1 --
	unsigned char	tx_offset[2];           //bit 7 offset direction 0 ++  1 --
	unsigned char	reserve_para[16];
	// 0-7 write dqs offset,8-15 read dqs offset,MSB bit 7 use 0 mean right offset
}  __packed board_phase_setting_ps_t;
typedef struct ddr_set {
	board_common_setting_t		cfg_board_common_setting;
	board_SI_setting_ps_t		cfg_board_SI_setting_ps[2];
	board_phase_setting_ps_t	cfg_ddr_training_delay_ps[2];
}  __packed ddr_set_t;

typedef struct ddr_set_ps0_only {
	board_common_setting_t		cfg_board_common_setting;
	board_SI_setting_ps_t		cfg_board_SI_setting_ps;
	board_phase_setting_ps_t	cfg_ddr_training_delay_ps;
}  __packed ddr_set_ps0_only_t;
typedef struct ddr_channel_struct {
	uint32_t	ddr_pll_base_add_ch;
	uint32_t	dmc_base_add_ch;
	uint32_t	phy_base_add_ch;
	uint32_t	dram_cs0_size_MB_ch;
	uint32_t	dram_cs1_size_MB_ch;
	uint32_t	dram_cs0_byte_disable_ch;
	uint32_t	dram_cs1_byte_disable_ch;
	ddr_set_t *p_ddrs_ch;
} ddr_channel_struct_t;
//ddr_channel_struct_t ddr_channel_base_add[4];
//ddr_channel_struct_t *ddr_channel_base_add_p;
ddr_set_ps0_only_t ddr_set_array[4] = { { { 0, }, }, };
ddr_set_ps0_only_t *p_ddrs;
#define DDR_TIMMING_OFFSET(X) (unsigned int)(unsigned long)(&(((ddr_set_ps0_only_t *)(0))->X))
#define DDR_TIMMING_OFFSET_SIZE(X) sizeof(((ddr_set_ps0_only_t *)(0))->X)

unsigned int dmc_sticky[64];
unsigned int sticky_reg_base_add;

unsigned int ddr_max(unsigned int a, unsigned int b)
{
	return (a > b) ? a : b;
}

unsigned int ddr_min(unsigned int a, unsigned int b)
{
	return (a > b) ? b : a;
}

void wr_reg(uint32_t base_add, uint32_t dat)
{
	*(volatile uint32_t *)(((uintptr_t)(0) << 2) + base_add) = (dat);
}

uint32_t rd_reg(uint32_t base_add)
{
	return *(volatile uint32_t *)(((uintptr_t)(0) << 2) + base_add);
}

void wr_reg_x4(uint32_t base_add, uint32_t offset, uint32_t dat)
{
	*(volatile uint32_t *)(((uintptr_t)(offset) << 2) + base_add) = (dat);
}

uint32_t rd_reg_x4(uint32_t base_add, uint32_t offset)
{
	return *(volatile uint32_t *)(((uintptr_t)(offset) << 2) + base_add);
}

void wr_reg_x2(uint32_t base_add, uint32_t offset, uint32_t dat)
{
	*(volatile uint32_t *)(((uintptr_t)(offset) << 1) + base_add) = (dat);
}

uint32_t rd_reg_x2(uint32_t base_add, uint32_t offset)
{
	return *(volatile uint32_t *)(((uintptr_t)(offset) << 1) + base_add);
}

void wr_reg_x1(uint32_t base_add, uint32_t offset, uint32_t dat)
{
	*(volatile uint32_t *)(((uintptr_t)(offset) << 0) + base_add) = (dat);
}

uint32_t rd_reg_x1(uint32_t base_add, uint32_t offset)
{
	return *(volatile uint32_t *)(((uintptr_t)(offset) << 0) + base_add);
}

//char *str_rtl_debug_pre ="\nrtl_debug_";
void wr_reg_dmc(uint32_t add_base, uint32_t add_offset, uint32_t value)
{
	//wr_reg(add_base + (add_offset << 0), value);
	wr_reg(add_base + (add_offset << 2), value);
#ifdef FOR_RTL_DEBUG
	if (skip_rtl_line_print) {
		;
	} else {
		uint32_t dmc1_offset = 0;

		if (dmc_ddr_config_channel_id == 1)
			dmc1_offset = 0x4000;
		if (add_base == 0xfe036000 || add_base == 0xfe034000) {
			if (add_base == 0xfe034000)
				dmc1_offset = 0x4000;
			ddr_log_info(LOG_CHL_0, "\nrtl_debug_ `REG_CTRL.wr_reg(32'h%x, 32'h%x);//",
				     dmc1_offset + add_base + (add_offset << 2), value);
		}
	}
#endif
}

uint32_t poll_reg_phy(uint32_t add_base, uint32_t add_offset, uint32_t offset_scale,
		      uint32_t mask, uint32_t value)
{
	uint32_t add = (add_base + (add_offset << offset_scale));

	while (((rd_reg(add)) & mask) != value)
		;
#ifdef FOR_RTL_DEBUG
	if (skip_rtl_line_print) {
		;
	} else {
		if (dmc_ddr_config_channel_id <= 1) {
			ddr_log_info(LOG_CHL_0, "\nrtl_debug_ `REG_CTRL.rd_reg(32'h%x, rddata);//",
				     add);
			ddr_log_info(LOG_CHL_0, "\nrtl_debug_ rddata  = 32'h%x;//", value);
			ddr_log_info(LOG_CHL_0,
				     "\nrtl_debug_ `REG_CTRL.poll_reg(32'h%x, 32'h%x,rddata);//",
				     add, mask);
		}
	}

#endif
	//return rd_reg(add_base + (add_offset << 0));
	return rd_reg(add);
}

uint32_t poll_reg_dmc(uint32_t add_base, uint32_t add_offset, uint32_t offset_scale,
		      uint32_t mask, uint32_t value)
{
	uint32_t dmc1_offset = 0;
	uint32_t add = (dmc1_offset + add_base + (add_offset << offset_scale));

	while (((rd_reg(add)) & mask) != value)
		;
#ifdef FOR_RTL_DEBUG
	if (skip_rtl_line_print) {
		;
	} else {
		if (dmc_ddr_config_channel_id == 1)
			dmc1_offset = 0x4000;
		if (add_base == 0xfe036000 || add_base == 0xfe034000) {
			if (add_base == 0xfe034000)
				dmc1_offset = 0x4000;
			add = (dmc1_offset + add_base + (add_offset << offset_scale));
			ddr_log_info(LOG_CHL_0, "\nrtl_debug_ `REG_CTRL.rd_reg(32'h%x, rddata);//",
				     add);
			ddr_log_info(LOG_CHL_0, "\nrtl_debug_ rddata  = 32'h%x;//", value);
			ddr_log_info(LOG_CHL_0,
				     "\nrtl_debug_ `REG_CTRL.poll_reg(32'h%x, 32'h%x,rddata);//",
				     add, mask);
		}
	}
#endif
	//return rd_reg(add_base + (add_offset << 0));
	return rd_reg(add_base + ((uintptr_t)add_offset << offset_scale));
}

uint32_t rd_reg_dmc(uint32_t add_base, uint32_t add_offset)
{
#ifdef FOR_RTL_DEBUG
	if (skip_rtl_line_print) {
		;
	} else {
		uint32_t dmc1_offset = 0;

		if (dmc_ddr_config_channel_id == 1)
			dmc1_offset = 0x4000;
		if (add_base == 0xfe036000 || add_base == 0xfe034000) {
			if (add_base == 0xfe034000)
				dmc1_offset = 0x4000;
			ddr_log_info(LOG_CHL_0, "\nrtl_debug_ rddata  = 32'h0;//");
			ddr_log_info(LOG_CHL_0, "\nrtl_debug_ `REG_CTRL.rd_reg(32'h%x, rddata);//",
				     dmc1_offset + add_base + (add_offset << 2));
//ddr_log_info(LOG_CHL_0,"\nrtl_debug_  #20000 //");//delay 20ns
		}
	}
#endif
	//return rd_reg(add_base + (add_offset << 0));
	return rd_reg(add_base + (add_offset << 2));
}

void wr_reg_ddr_phy(uint32_t add_base, uint32_t add_offset, uint32_t value)
{
	//wr_reg(add_base + (add_offset << 0), value);
	wr_reg(add_base + (add_offset << 0), value);
#ifdef FOR_RTL_DEBUG
	if (skip_rtl_line_print) {
	} else {
		ddr_log_info(LOG_CHL_0, "\nrtl_debug_ dwc_ddrphy_apb_wr(32'h%x, 32'h%x);//",
			     0 + (add_offset << 0), value);
	}
#endif
}

uint32_t rd_reg_ddr_phy(uint32_t add_base, uint32_t add_offset)
{
#ifdef FOR_RTL_DEBUG
	if (skip_rtl_line_print) {
	} else {
		ddr_log_info(LOG_CHL_0, "\nrtl_debug_ rddata  = 32'h0;//");
		ddr_log_info(LOG_CHL_0, "\nrtl_debug_ dwc_ddrphy_apb_rd(32'h%x, rddata);//",
			     0 + (add_offset << 0));
//ddr_log_info(LOG_CHL_0,"\n #10000 //");//delay 20ns
	}
#endif

	//return rd_reg(add_base + (add_offset << 0));
	return rd_reg(add_base + (add_offset << 0));
}

uint32_t rd_reg_ddr_phy_nop(uint32_t add_base, uint32_t add_offset, uint32_t rdat)
{
#ifdef FOR_RTL_DEBUG
	ddr_log_info(LOG_CHL_0, "\nrtl_debug_ rddata  = 32'h0;//");
	ddr_log_info(LOG_CHL_0, "\nrtl_debug_ dwc_ddrphy_apb_rd(32'h%x, rddata);//",
		     0 + (add_offset << 0));
#endif

	//return rd_reg(add_base + (add_offset << 0));
	rdat = rd_reg(add_base + (add_offset << 0));
	return rdat;
}

void wr_reg_ddr_phy_x2(uint32_t add_base, uint32_t add_offset, uint32_t value)
{
	//wr_reg(add_base + (add_offset << 0), value);

#ifdef FOR_RTL_DEBUG
	uint32_t old_offset = add_offset;
	uint32_t old_value = value;
#endif
	add_base = add_base + ((add_offset & 0xff000) << 2);
	add_offset = add_offset & 0xfff;
	uint32_t add_offset_m = ((add_offset >> 1) << 2);
	uint32_t add_offset_s = (add_offset << 1) - add_offset_m;
	uint32_t temp_value = rd_reg(add_base + add_offset_m);

	if (add_offset_s == 0)
		value = (temp_value & 0xffff0000) | (value << 0);
	else
		value = (temp_value & 0xffff) | (value << 16);
	wr_reg(add_base + add_offset_m, value);

	//uint32_t old_offset=add_offset;

#ifdef FOR_RTL_DEBUG
	//ddr_log_info(LOG_CHL_0,"\nacsm reg=%x,%x",add_base + add_offset_m,
	//rd_reg(add_base + add_offset_m));
	//  ddr_log_info(LOG_CHL_0," %x",(old_offset&0xff000)+(((old_offset&0xfff) >> 1)<<0));
	ddr_log_info(LOG_CHL_0, "\nrtl_debug_ rddata  = 32'h0;//");
	ddr_log_info(LOG_CHL_0, "\nrtl_debug_ dwc_ddrphy_apb_rd(32'h%x, rddata);//",
		     (old_offset & 0xff000) + (((old_offset & 0xfff) >> 1) << 0));
	if (old_offset & 1)
		//ddr_log_info(LOG_CHL_0,"\nrtl_debug_ rddata[31:16]  = 32'h%x;",old_value);
		ddr_log_info(LOG_CHL_0, "\nrtl_debug_ rddata=(rddata&32'hffff)|32'h%x;//",
			     (old_value << 16));
	// rddata=(rddata&32'hffff0000)| 32'h00002066;
	else
		//ddr_log_info(LOG_CHL_0,"\nrtl_debug_ rddata[15:0]  = 32'h%x;",old_value);
		ddr_log_info(LOG_CHL_0, "\nrtl_debug_ rddata=(rddata&32'hffff0000)|32'h%x;//",
			     (old_value << 0));
	ddr_log_info(LOG_CHL_0, "\nrtl_debug_ dwc_ddrphy_apb_wr(32'h%x, rddata);//",
		     (old_offset & 0xff000) + (((old_offset & 0xfff) >> 1) << 0));
#endif
}

uint32_t rd_reg_ddr_phy_x2(uint32_t add_base, uint32_t add_offset)
{
#ifdef FOR_RTL_DEBUG
	uint32_t old_offset = add_offset;
	//uint32_t old_value=value;
#endif
	add_base = add_base + ((add_offset & 0xff000) << 2);
	add_offset = add_offset & 0xfff;
	uint32_t add_offset_m = ((add_offset >> 1) << 2);
	uint32_t add_offset_s = (add_offset << 1) - add_offset_m;
	uint32_t temp_value = rd_reg(add_base + add_offset_m);
	uint32_t value = 0;

	if (add_offset_s == 2)
		value = ((temp_value >> 16) & 0xffff);
	else
		value = (temp_value & 0xffff);
#ifdef FOR_RTL_DEBUG
	ddr_log_info(LOG_CHL_0, "\nrtl_debug_ rddata  = 32'h0;//");
	ddr_log_info(LOG_CHL_0, "\nrtl_debug_ 'dwc_ddrphy_apb_rd(32'h%x, rddata);//",
		     (old_offset & 0xff000) + (((old_offset & 0xfff) >> 1) << 0));
#endif
	return value;
}

uint32_t ddr_cacl_phy_delay_all_step(char test_index, uint32_t value)
{
	uint32_t result = 0;
	uint32_t coarse_value = 0;
	uint32_t fine_value = 0;

	if (value)
		coarse_value = (value >> 16);
	else
		coarse_value = 0;
	fine_value = (value & 0xffff);
	result = (coarse_value * 128 + fine_value);
	return result;
} /* ddr_cacl_phy_delay_all_step */

uint32_t ddr_cacl_phy_over_ride_back_reg(char test_index, uint32_t value)
{
	uint32_t result = 0;

	//bit0-15 fine value ,bit 16-32 coarse value

	if (test_index == DMC_TEST_WINDOW_INDEX_ATXDLY) {
		if (value > (3 * 128 + 127))
			value = (3 * 128 + 127);
		result = (value % 128) + ((value / 128) << 16);
	} else if (test_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY ||
		   test_index == DMC_TEST_WINDOW_INDEX_TXDQDLY) {
		if (value > (7 * 128 + 127))
			value = (7 * 128 + 127);
		result = (value % 128) + ((value / 128) << 16);
	} else if (test_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY ||
		   test_index == DMC_TEST_WINDOW_INDEX_RXPBDLY) {
		if (value > (1 * 128 + 127))
			value = (1 * 128 + 127);
		result = value;
	} else if (test_index == DMC_TEST_WINDOW_INDEX_RXENDLY) {
		if (value > (31 * 128 + 127))
			value = (31 * 128 + 127);
		result = (value % 128) + ((value / 128) << 16);
	} else if (test_index == WINDOW_TEST_SOC_VREF_DAC0 ||
		   test_index == WINDOW_TEST_SOC_VREF_DAC1) {
		if (value > (127))
			value = (127);
		result = value;
	}
	return result;
} /* ddr_cacl_phy_over_ride_back_reg */

#ifndef _SHA256_H_DDR
#define _SHA256_H_DDR

#define SHA_SUM_LEN  32
#define SHA256_DER_LEN  19

/* Reset watchdog each time we process this many bytes */
#define CHUNKSZ_SHA256  (64 * 1024)

typedef struct {
	uint32_t	total[2];
	uint32_t	state[8];
	uint8_t		buffer[64];
} sha256_context_ddr;

const uint8_t sha256_der_prefix_ddr[SHA256_DER_LEN] = {
	0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86,
	0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05,
	0x00, 0x04, 0x20
};

/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n, b, i) {                          \
		(n) = ((unsigned long)(b)[(i)] << 24)    \
		      | ((unsigned long)(b)[(i) + 1] << 16)    \
		      | ((unsigned long)(b)[(i) + 2] << 8)    \
		      | ((unsigned long)(b)[(i) + 3]);   \
}
#endif
#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n, b, i) {                          \
		(b)[(i)] = (unsigned char)((n) >> 24);   \
		(b)[(i) + 1] = (unsigned char)((n) >> 16);   \
		(b)[(i) + 2] = (unsigned char)((n) >> 8);   \
		(b)[(i) + 3] = (unsigned char)((n));   \
}
#endif

void sha256_starts_internal(sha256_context_ddr *ctx)
{
	ctx->total[0] = 0;
	ctx->total[1] = 0;

	ctx->state[0] = 0x6A09E667;
	ctx->state[1] = 0xBB67AE85;
	ctx->state[2] = 0x3C6EF372;
	ctx->state[3] = 0xA54FF53A;
	ctx->state[4] = 0x510E527F;
	ctx->state[5] = 0x9B05688C;
	ctx->state[6] = 0x1F83D9AB;
	ctx->state[7] = 0x5BE0CD19;
}

static void sha256_process_internal(sha256_context_ddr *ctx, const uint8_t data[64])
{
	uint32_t temp1, temp2;
	uint32_t W[64];
	uint32_t A, B, C, D, E, F, G, H;

	GET_UINT32_BE(W[0], data, 0);
	GET_UINT32_BE(W[1], data, 4);
	GET_UINT32_BE(W[2], data, 8);
	GET_UINT32_BE(W[3], data, 12);
	GET_UINT32_BE(W[4], data, 16);
	GET_UINT32_BE(W[5], data, 20);
	GET_UINT32_BE(W[6], data, 24);
	GET_UINT32_BE(W[7], data, 28);
	GET_UINT32_BE(W[8], data, 32);
	GET_UINT32_BE(W[9], data, 36);
	GET_UINT32_BE(W[10], data, 40);
	GET_UINT32_BE(W[11], data, 44);
	GET_UINT32_BE(W[12], data, 48);
	GET_UINT32_BE(W[13], data, 52);
	GET_UINT32_BE(W[14], data, 56);
	GET_UINT32_BE(W[15], data, 60);

#define SHR(x, n) ((x & 0xFFFFFFFF) >> n)
#define ROTR(x, n) (SHR(x, n) | (x << (32 - n)))

#define S0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3))
#define S1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

#define S2(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define S3(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))

#define F0(x, y, z) ((x & y) | (z & (x | y)))
#define F1(x, y, z) (z ^ (x & (y ^ z)))

#define R(t)                                    \
	(                                               \
		W[t] = S1(W[t - 2]) + W[t - 7] +        \
		       S0(W[t - 15]) + W[t - 16]       \
	)

#define P(a, b, c, d, e, f, g, h, x, K) {                \
		temp1 = h + S3(e) + F1(e, f, g) + K + x;  \
		temp2 = S2(a) + F0(a, b, c);              \
		d += temp1; h = temp1 + temp2;          \
}

	A = ctx->state[0];
	B = ctx->state[1];
	C = ctx->state[2];
	D = ctx->state[3];
	E = ctx->state[4];
	F = ctx->state[5];
	G = ctx->state[6];
	H = ctx->state[7];

	P(A, B, C, D, E, F, G, H, W[0], 0x428A2F98);
	P(H, A, B, C, D, E, F, G, W[1], 0x71374491);
	P(G, H, A, B, C, D, E, F, W[2], 0xB5C0FBCF);
	P(F, G, H, A, B, C, D, E, W[3], 0xE9B5DBA5);
	P(E, F, G, H, A, B, C, D, W[4], 0x3956C25B);
	P(D, E, F, G, H, A, B, C, W[5], 0x59F111F1);
	P(C, D, E, F, G, H, A, B, W[6], 0x923F82A4);
	P(B, C, D, E, F, G, H, A, W[7], 0xAB1C5ED5);
	P(A, B, C, D, E, F, G, H, W[8], 0xD807AA98);
	P(H, A, B, C, D, E, F, G, W[9], 0x12835B01);
	P(G, H, A, B, C, D, E, F, W[10], 0x243185BE);
	P(F, G, H, A, B, C, D, E, W[11], 0x550C7DC3);
	P(E, F, G, H, A, B, C, D, W[12], 0x72BE5D74);
	P(D, E, F, G, H, A, B, C, W[13], 0x80DEB1FE);
	P(C, D, E, F, G, H, A, B, W[14], 0x9BDC06A7);
	P(B, C, D, E, F, G, H, A, W[15], 0xC19BF174);
	P(A, B, C, D, E, F, G, H, R(16), 0xE49B69C1);
	P(H, A, B, C, D, E, F, G, R(17), 0xEFBE4786);
	P(G, H, A, B, C, D, E, F, R(18), 0x0FC19DC6);
	P(F, G, H, A, B, C, D, E, R(19), 0x240CA1CC);
	P(E, F, G, H, A, B, C, D, R(20), 0x2DE92C6F);
	P(D, E, F, G, H, A, B, C, R(21), 0x4A7484AA);
	P(C, D, E, F, G, H, A, B, R(22), 0x5CB0A9DC);
	P(B, C, D, E, F, G, H, A, R(23), 0x76F988DA);
	P(A, B, C, D, E, F, G, H, R(24), 0x983E5152);
	P(H, A, B, C, D, E, F, G, R(25), 0xA831C66D);
	P(G, H, A, B, C, D, E, F, R(26), 0xB00327C8);
	P(F, G, H, A, B, C, D, E, R(27), 0xBF597FC7);
	P(E, F, G, H, A, B, C, D, R(28), 0xC6E00BF3);
	P(D, E, F, G, H, A, B, C, R(29), 0xD5A79147);
	P(C, D, E, F, G, H, A, B, R(30), 0x06CA6351);
	P(B, C, D, E, F, G, H, A, R(31), 0x14292967);
	P(A, B, C, D, E, F, G, H, R(32), 0x27B70A85);
	P(H, A, B, C, D, E, F, G, R(33), 0x2E1B2138);
	P(G, H, A, B, C, D, E, F, R(34), 0x4D2C6DFC);
	P(F, G, H, A, B, C, D, E, R(35), 0x53380D13);
	P(E, F, G, H, A, B, C, D, R(36), 0x650A7354);
	P(D, E, F, G, H, A, B, C, R(37), 0x766A0ABB);
	P(C, D, E, F, G, H, A, B, R(38), 0x81C2C92E);
	P(B, C, D, E, F, G, H, A, R(39), 0x92722C85);
	P(A, B, C, D, E, F, G, H, R(40), 0xA2BFE8A1);
	P(H, A, B, C, D, E, F, G, R(41), 0xA81A664B);
	P(G, H, A, B, C, D, E, F, R(42), 0xC24B8B70);
	P(F, G, H, A, B, C, D, E, R(43), 0xC76C51A3);
	P(E, F, G, H, A, B, C, D, R(44), 0xD192E819);
	P(D, E, F, G, H, A, B, C, R(45), 0xD6990624);
	P(C, D, E, F, G, H, A, B, R(46), 0xF40E3585);
	P(B, C, D, E, F, G, H, A, R(47), 0x106AA070);
	P(A, B, C, D, E, F, G, H, R(48), 0x19A4C116);
	P(H, A, B, C, D, E, F, G, R(49), 0x1E376C08);
	P(G, H, A, B, C, D, E, F, R(50), 0x2748774C);
	P(F, G, H, A, B, C, D, E, R(51), 0x34B0BCB5);
	P(E, F, G, H, A, B, C, D, R(52), 0x391C0CB3);
	P(D, E, F, G, H, A, B, C, R(53), 0x4ED8AA4A);
	P(C, D, E, F, G, H, A, B, R(54), 0x5B9CCA4F);
	P(B, C, D, E, F, G, H, A, R(55), 0x682E6FF3);
	P(A, B, C, D, E, F, G, H, R(56), 0x748F82EE);
	P(H, A, B, C, D, E, F, G, R(57), 0x78A5636F);
	P(G, H, A, B, C, D, E, F, R(58), 0x84C87814);
	P(F, G, H, A, B, C, D, E, R(59), 0x8CC70208);
	P(E, F, G, H, A, B, C, D, R(60), 0x90BEFFFA);
	P(D, E, F, G, H, A, B, C, R(61), 0xA4506CEB);
	P(C, D, E, F, G, H, A, B, R(62), 0xBEF9A3F7);
	P(B, C, D, E, F, G, H, A, R(63), 0xC67178F2);

	ctx->state[0] += A;
	ctx->state[1] += B;
	ctx->state[2] += C;
	ctx->state[3] += D;
	ctx->state[4] += E;
	ctx->state[5] += F;
	ctx->state[6] += G;
	ctx->state[7] += H;
}

void sha256_update_internal(sha256_context_ddr *ctx, const uint8_t *input, uint32_t length)
{
	uint32_t left, fill;

	if (!length)
		return;

	left = ctx->total[0] & 0x3F;
	fill = 64 - left;

	ctx->total[0] += length;
	ctx->total[0] &= 0xFFFFFFFF;

	if (ctx->total[0] < length)
		ctx->total[1]++;

	if (left && length >= fill) {
		memcpy((void *)(ctx->buffer + left), (void *)input, fill);
		sha256_process_internal(ctx, ctx->buffer);
		length -= fill;
		input += fill;
		left = 0;
	}

	while (length >= 64) {
		sha256_process_internal(ctx, input);
		length -= 64;
		input += 64;
	}

	if (length)
		memcpy((void *)(ctx->buffer + left), (void *)input, length);
}

static uint8_t sha256_padding[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void sha256_finish_internal(sha256_context_ddr *ctx, uint8_t digest[32])
{
	uint32_t last, padn;
	uint32_t high, low;
	uint8_t msglen[8];

	high = ((ctx->total[0] >> 29)
		| (ctx->total[1] << 3));
	low = (ctx->total[0] << 3);

	PUT_UINT32_BE(high, msglen, 0);
	PUT_UINT32_BE(low, msglen, 4);

	last = ctx->total[0] & 0x3F;
	padn = (last < 56) ? (56 - last) : (120 - last);

	sha256_update_internal(ctx, sha256_padding, padn);
	sha256_update_internal(ctx, msglen, 8);

	PUT_UINT32_BE(ctx->state[0], digest, 0);
	PUT_UINT32_BE(ctx->state[1], digest, 4);
	PUT_UINT32_BE(ctx->state[2], digest, 8);
	PUT_UINT32_BE(ctx->state[3], digest, 12);
	PUT_UINT32_BE(ctx->state[4], digest, 16);
	PUT_UINT32_BE(ctx->state[5], digest, 20);
	PUT_UINT32_BE(ctx->state[6], digest, 24);
	PUT_UINT32_BE(ctx->state[7], digest, 28);
}

/*
 * Output = SHA-256( input buffer ). Trigger the watchdog every 'chunk_sz'
 * bytes of input processed.
 */
void sha256_csum_wd_internal(const unsigned char *input, unsigned int ilen,
			     unsigned char *output, unsigned int chunk_sz)
{
	sha256_context_ddr ctx;

	sha256_starts_internal(&ctx);
	sha256_update_internal(&ctx, input, ilen);
	sha256_finish_internal(&ctx, output);
}
#endif

//typedef struct ddr_sha_s {
//	unsigned char	sha2[SHA_SUM_LEN];
//	ddr_set_t	ddrs;
//	unsigned char	sha_chip_id[MESON_CPU_CHIP_ID_SIZE];
//} ddr_sha_t;

//ddr_sha_t ddr_sha = { { 0 } };
//ddr_set_t *ddr_set_t_p_array = &ddr_sha_s5.ddrs;

typedef struct ddr_sha_s_s5 {
	unsigned char		sha2[SHA_SUM_LEN];
	ddr_set_ps0_only_t	ddrs;
	unsigned char		sha_chip_id[MESON_CPU_CHIP_ID_SIZE];
} ddr_sha_t_s5;

ddr_sha_t_s5 ddr_sha_s5 = { { 0 } };
ddr_set_ps0_only_t *ddr_set_t_p_array_s5 = &ddr_sha_s5.ddrs;
ddr_set_ps0_only_t *ddr_set_t_p;

void dmc_change_channel(uint32_t ch)
{
	//aml_phy_base_add = phy_base_add;
	//ddr_pll_base_add = ddr_pll_base_add_ch[ch];
	dmc_ddr_config_channel_id = ch;
	//skip_rtl_line_print = 0;
	//if (dmc_ddr_config_channel_id > 1)
	//	skip_rtl_line_print = 1;
	//ddr_channel_base_add_p = &(ddr_channel_base_add[ch]);
	ddr_pll_base_add = pll_base_array_add[dmc_ddr_config_channel_id];
	dmc_base_add = dmc_base_array_add[dmc_ddr_config_channel_id];
	phy_base_add = phy_base_array_add[dmc_ddr_config_channel_id];
	sticky_reg_base_add = ddr_dmc_sticky[dmc_ddr_config_channel_id];
	p_ddrs = &ddr_set_array[dmc_ddr_config_channel_id];

//#endif
}

int check_base_address(void)
{
	ddr_set_t_p = (ddr_set_ps0_only_t *)(ddr_set_t_p_array_s5);
	unsigned int table_max =
		(sizeof(__ddr_base_address_table)) / (sizeof(ddr_base_address_table_t));
	unsigned int table_index = 0;
	char chip_id = 0;

	chip_id = ddr_get_chip_id();
	p_ddr_base = (ddr_base_address_table_t *)(&__ddr_base_address_table);

	printf("\ntable_max=%8x,p_ddr_base_add=%8x,chip_id=%8x",
	       table_max, (unsigned int)(unsigned long)p_ddr_base, chip_id);
	if (chip_id == 0)
		chip_id = CHIP_ID_MASK;
	if (chip_id) {
		if (chip_id == MESON_CPU_MAJOR_ID_S4D || chip_id == MESON_CPU_MAJOR_ID_A5 ||
		    chip_id == DDR_MESON_CPU_MAJOR_ID_C3)
			chip_id = MESON_CPU_MAJOR_ID_S4;
		for (table_index = 0; table_index < table_max; table_index++) {
			printf("\ntable_index=%8x,p_ddr_base_add=%8x,(p_ddr_base->chip_id==%8x",
			       table_index, (unsigned int)(unsigned long)p_ddr_base,
			       (p_ddr_base->chip_id));
			if (p_ddr_base->chip_id == chip_id && chip_id < CHIP_ID_MASK) {
				printf("\nfind match chip id=0x%8x ,%s",
				       chip_id, p_ddr_base->soc_family_name);
				break;
			}
			// {
			printf("\nno find match chip id=0x%8x, ,%s will use default value",
			       chip_id, p_ddr_base->soc_family_name);
			//}
			p_ddr_base = (p_ddr_base + 1);
		}
	}

	int count = 0;

	for (count = 0; count < 12; count++)
		ddr_sha_s5.sha_chip_id[count] = global_chip_id[count];

	if (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S5) {
		phy_base_array_add[0] = p_ddr_base->ddr_phy_base_address;
		phy_base_array_add[1] = p_ddr_base->ddr_phy_base_address;
		phy_base_array_add[2] = p_ddr_base->ddr_phy_base_address_1;
		phy_base_array_add[3] = p_ddr_base->ddr_phy_base_address_1;
		ddr_dmc_sticky[0] = p_ddr_base->ddr_dmc_sticky0;
		ddr_dmc_sticky[1] = p_ddr_base->ddr_dmc_sticky0;
		ddr_dmc_sticky[2] = p_ddr_base->ddr_dmc_sticky0_1;
		ddr_dmc_sticky[3] = p_ddr_base->ddr_dmc_sticky0_1;
		ddr_dmc_apd_address[0] = p_ddr_base->ddr_dmc_apd_address;
		ddr_dmc_apd_address[1] = p_ddr_base->ddr_dmc_apd_address;
		ddr_dmc_asr_address[0] = p_ddr_base->ddr_dmc_asr_address_1;
		ddr_dmc_asr_address[1] = p_ddr_base->ddr_dmc_asr_address_1;
		dmc_retraining_ctrl_address[0] = p_ddr_base->ddr_dmc_lpdd4_retraining_address;
		dmc_retraining_ctrl_address[1] = p_ddr_base->ddr_dmc_lpdd4_retraining_address;
		dmc_retraining_ctrl_address[2] = p_ddr_base->ddr_dmc_lpdd4_retraining_address_1;
		dmc_retraining_ctrl_address[3] = p_ddr_base->ddr_dmc_lpdd4_retraining_address_1;
	}
	dmc_change_channel(0);
	return (unsigned int)(unsigned long)(p_ddr_base);
}

char *itoa_ddr_test(int num, char *str, int radix)
{
	printf("\nitoa_ddr_test 1\n");
	char index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	unsigned unum;
	char temp;
	int i = 0, j, k;

	if (radix == 10 && num < 0) {
		unum = (unsigned)-num;
		str[i++] = '-';
	} else {
		unum = (unsigned)num;
	}

	printf("\nitoa_ddr_test 2\n");
	printf("\nunum=0x%8x\n", unum);
	printf("\nunum2=0x%8x\n", (unum % (unsigned)radix));
	printf("\nradix=0x%8x\n", radix);
	str[0] = index[0];
	printf("\nitoa_ddr_test 22\n");
	unum /= radix;
	printf("\nitoa_ddr_test 23\n");
	do {
		str[i++] = index[unum % (unsigned)radix];
		unum /= radix;
	} while (unum);
	printf("\nitoa_ddr_test 3\n");
	str[i] = '\0';

	if (str[0] == '-')
		k = 1;
	else
		k = 0;
	printf("\nitoa_ddr_test 4\n");
	for (j = k; j <= (i - 1) / 2; j++) {
		temp = str[j];
		str[j] = str[i - 1 + k - j];
		str[i - 1 + k - j] = temp;
	}
	return str;
}

int TOLOWER(int ch)
{
	if ((unsigned int)(ch - 'A') < 26u)
		ch += 'a' - 'A';
	return ch;
}

int isxdigit(int ch)
{
	return (unsigned int)(ch - '0') < 10u || (unsigned int)((ch | 0x20) - 'a') < 6u;
}

int isdigit(int ch)
{
	return (unsigned int)(ch - '0') < 10u;
}

unsigned int simple_guess_base(const char *cp)
{
	if (cp[0] == '0') {
		if (TOLOWER(cp[1]) == 'x' && isxdigit(cp[2]))
			return 16;
		else
			return 10;
	} else {
		return 10;
	}
}

unsigned int simple_strtoull_ddr(const char *cp, char **endp, unsigned int base)
{
	unsigned int result = 0;

	if (!cp) // add 20170616
		return 0;
	if (!base)
		base = simple_guess_base(cp);
	if (base == 16 && cp[0] == '0' && TOLOWER(cp[1]) == 'x')
		cp += 2;
	if (base == 10)
		while ((*cp) == '0')
			cp++;
	while (isxdigit(*cp)) {
		unsigned int value;

		value = isdigit(*cp) ? *cp - '0' : TOLOWER(*cp) - 'a' + 10;
		if (value >= base)
			break;
		result = result * base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

unsigned int env_to_a_num(const char *env_name)
{
	char *str_buf = NULL;
	char buf[48];

	str_buf = (char *)(&buf);
	memset(str_buf, 0, sizeof(buf));
	printf("sizeof(str_buf)==%d\n", (unsigned int)(sizeof(buf)));
	str_buf = getenv(env_name);
	unsigned int a_num = 0;
	char *endp;

	if (!str_buf)
		return 0;

	printf("str==%s\n", str_buf);

	a_num = simple_strtoull_ddr(str_buf, &endp, 0);
	printf("%s=0x%8x\n", str_buf, a_num);

	return a_num;
}

unsigned int a_num_to_env(const char *env_name, unsigned int *a_num)
{
	char *str_buf = NULL;
	char buf[1024];

	str_buf = (char *)(&buf);
	memset(str_buf, 0, sizeof(buf));
	printf("sizeof(str_buf)==%d\n", (unsigned int)(sizeof(buf)));
	str_buf = getenv(env_name);

	if (!str_buf)
		return 0;

	printf("str==%s\n", str_buf);

	sprintf(buf, "0x%8x", *a_num);

	printf("%s=0x%8x", buf, *a_num);
	setenv(env_name, buf);

	run_command("save", 0);
	return 1;
}

unsigned int env_to_num(const char *env_name, unsigned int *num_arry)
{
	char *str_buf = NULL;
	char buf[1024];
	unsigned int str_to_numarry[48];

	str_buf = (char *)(&buf);
	memset(str_buf, 0, sizeof(buf));
	printf("sizeof(str_buf)==%d\n", (unsigned int)(sizeof(buf)));
	str_buf = getenv(env_name);

	if (!str_buf)
		return 0;

	char *str[48];
	char *endp;
	int i;

	for (i = 0; i < 48; i++)
		str_to_numarry[i] = 0;
	printf("str==%s\n", str_buf);
	for (i = 0; i < 48; i++) {
		str[i] = strsep(&str_buf, ";");
		if (!str[i])
			break;
		str_to_numarry[i] = simple_strtoull_ddr(str[i], &endp, 0);
	}
	for (i = 0; i < 48; i++) {
		printf("str_to_numarry[%d]==%d\n", i, str_to_numarry[i]);
		num_arry[i] = str_to_numarry[i];
	}
	return 1;
}

unsigned int num_to_env(const char *env_name, unsigned int *num_arry)
{
	char *str_buf = NULL;
	char buf[512];
	char buf_f[512];
	int i;

	str_buf = (char *)(&buf);
	memset(str_buf, 0, sizeof(buf));
	printf("sizeof(str_buf)==%d\n", (unsigned int)(sizeof(buf)));
	str_buf = getenv(env_name);

	if (!str_buf)
		return 0;

	printf("str==%s\n", str_buf);

	sprintf(buf, "0x%8x", num_arry[0]);
	for (i = 1; i < 48; i++) {
		sprintf(buf_f, "%s;0x%8x", buf, num_arry[i]);
		printf("%d  %d\n", i, num_arry[i]);
	}
	printf("%s", buf_f);
	setenv(env_name, buf_f);

	run_command("save", 0);
	return 1;
}

#define TDATA32F 0xffffffff
#define TDATA32A 0xaaaaaaaa
#define TDATA325 0x55555555

#define DMC_STICKY_MAGIC_0                              0x12345678
#define DMC_STICKY_MAGIC_1                              0xabcdbead
#define DMC_STICKY_UBOOT_WINDOW_MAGIC_1                              0x22
#define DMC_STICKY_AUTO_TEST_CMD_INDEX_MAGIC_1                              0x33

unsigned int global_boot_times;
unsigned int watchdog_time_s = 20;
unsigned int global_ddr_clk = 1;
unsigned int bdlr_100step;
unsigned int ui_1_32_100step;
unsigned int error_count;
unsigned int error_outof_count_flag;
unsigned int copy_test_flag;
unsigned int training_pattern_flag;
unsigned int test_start_addr = 0x1080000;

unsigned int dq_lcd_bdl_value_aclcdlr_org_a;
unsigned int dq_lcd_bdl_value_bdlr0_org_a;
unsigned int dq_lcd_bdl_value_aclcdlr_min_a;
unsigned int dq_lcd_bdl_value_bdlr0_min_a;
unsigned int dq_lcd_bdl_value_aclcdlr_max_a;
unsigned int dq_lcd_bdl_value_bdlr0_max_a;
unsigned int dq_lcd_bdl_value_aclcdlr_status_a;
unsigned int dq_lcd_bdl_value_bdlr0_status_a;

unsigned int dq_lcd_bdl_value_aclcdlr_org_b;
unsigned int dq_lcd_bdl_value_bdlr0_org_b;
unsigned int dq_lcd_bdl_value_aclcdlr_min_b;
unsigned int dq_lcd_bdl_value_bdlr0_min_b;
unsigned int dq_lcd_bdl_value_aclcdlr_max_b;
unsigned int dq_lcd_bdl_value_bdlr0_max_b;

unsigned int dq_lcd_bdl_value_wdq_org_a[4];
unsigned int dq_lcd_bdl_value_rdqs_org_a[4];
unsigned int dq_lcd_bdl_value_wdq_min_a[4];
unsigned int dq_lcd_bdl_value_wdq_max_a[4];
unsigned int dq_lcd_bdl_value_rdqs_min_a[4];
unsigned int dq_lcd_bdl_value_rdqs_max_a[4];
unsigned int dq_lcd_bdl_value_wdq_status_a[4];
unsigned int dq_lcd_bdl_value_rdqs_status_a[4];

unsigned int dq_lcd_bdl_value_wdq_org_b[4];
unsigned int dq_lcd_bdl_value_rdqs_org_b[4];
unsigned int dq_lcd_bdl_value_wdq_min_b[4];
unsigned int dq_lcd_bdl_value_wdq_max_b[4];
unsigned int dq_lcd_bdl_value_rdqs_min_b[4];
unsigned int dq_lcd_bdl_value_rdqs_max_b[4];
unsigned int dq_lcd_bdl_value_wdq_status_b[4];
unsigned int dq_lcd_bdl_value_rdqs_status_b[4];
unsigned int acbdlr0_9_reg_org[10];
unsigned int acbdlr0_9_reg_setup_max[40];
unsigned int acbdlr0_9_reg_hold_max[40];
unsigned int acbdlr0_9_reg_setup_time[40];
unsigned int acbdlr0_9_reg_hold_time[40];
unsigned int data_bdlr0_5_reg_org[28];          //4//4lane
unsigned int bdlr0_9_reg_setup_max[24 * 4];     //4//4 lane 96 bdlr
unsigned int bdlr0_9_reg_hold_max[24 * 4];
unsigned int bdlr0_9_reg_setup_time[24 * 4];
unsigned int bdlr0_9_reg_hold_time[24 * 4];

unsigned int pre_fetch_enable;

#define readl(addr)    (unsigned int)(*((volatile unsigned int *)((unsigned long)addr)))
#define writel(data, addr)  (*((volatile unsigned int *)((unsigned long)addr))) = (data)

#define wr_reg(addr, data) (*((volatile unsigned int *)((unsigned long)addr))) = (data)
#define rd_reg(addr)  (unsigned int)(*((volatile unsigned int *)((unsigned long)addr)))

#ifndef CONFIG_CHIP
#define CHIP_OLD           0
#define CHIP_TXLX           1
#define CHIP_A113           2
#define CHIP_G12         3
#define CONFIG_CHIP   CHIP_G12 // CHIP_OLD//
#endif

#define P_DDR_PHY_DEFAULT           0
#define P_DDR_PHY_GX_BABY             1
#define P_DDR_PHY_GX_TV_BABY     2
#define P_DDR_PHY_905X        3

#define P_DDR_PHY_G12        4
#define CONFIG_DDR_PHY   P_DDR_PHY_G12

#define PATTERN_USE_DDR_DES
#define USE_64BIT_POINTER
#ifdef USE_64BIT_POINTER
#define p_convert_int(a)  (unsigned int)(unsigned long)(a)
#define int_convert_p(a)  (unsigned long)(a)

#else
#define p_convert_int(a)  (unsigned int)(a)
#define int_convert_p(a)  (unsigned int)(a)
#endif

#ifdef PATTERN_USE_DDR_DES
#define des_pattern(a, b, c, d)  (des[a] ^ pattern_ ## b[c][d])
#define des_inv_pattern(a, b, c, d)   (des[a] ^ (~(pattern_ ## b[c][d])))
#define des_xor_pattern(a, b)   (a ^ b)
#else
#define des_pattern(a, b, c, d)  (des[a] & 0) + pattern_ ## b[c][d]
#define des_inv_pattern(a, b, c, d)  (des[a] & 0) + ~(pattern_ ## b[c][d])
#define des_xor_pattern(a, b)  (a & 0 + b)
#endif

#define DDR_LCDLR_CK_USE_FAST_PATTERN
#if (CONFIG_DDR_PHY > P_DDR_PHY_DEFAULT)
#define DDR_PREFETCH_CACHE
#endif
#ifdef DDR_PREFETCH_CACHE
#define ddr_pld_cache(P)   asm ("prfm PLDL1KEEP, [%0, #376]" ::"r" (P))
#else
#define ddr_pld_cache(P)
#endif

#define DDR_TEST_START_ADDR  0x1080000 //  0x10000000//CONFIG_SYS_MEMTEST_START
#define DDR_TEST_SIZE 0x2000000

#if (CONFIG_CHIP >= CHIP_TXLX)

#endif

#define get_us_time()    (rd_reg(p_ddr_base->ee_timer_base_address)) // (readl(P_ISA_TIMERE))

unsigned int dwc_ddrphy_apb_wr(unsigned int addr, unsigned int dat)
{
	*(volatile uint32_t *)(int_convert_p(((addr) << 1) +
					     (phy_base_add))) = ((uint16_t)dat);
	return 1;
}

unsigned int dwc_ddrphy_apb_rd(unsigned int addr)
{
	return *(volatile uint32_t *)(int_convert_p(addr << 1) + phy_base_add);
}

void ddr_udelay(unsigned int us)
{
	unsigned int t0 = rd_reg(p_ddr_base->ee_timer_base_address);

	while (rd_reg(p_ddr_base->ee_timer_base_address) - t0 <= us)
		;
}

void ddr_udelay_dummy(unsigned int us)
{
	if (p_ddr_base->ee_timer_base_address) {
		ddr_udelay(us);
	} else {
		if (us == 0)
			us = 10000;
		while (us--)
			;
	}
}

#define DDR_PARAMETER_SOURCE_FROM_DMC_STICKY    1
#define DDR_PARAMETER_SOURCE_FROM_UBOOT_ENV     2
#define DDR_PARAMETER_SOURCE_FROM_UBOOT_IDME    3
#define DDR_PARAMETER_SOURCE_FROM_ORG_STICKY  4

#define         DDR_PARAMETER_READ              1
#define         DDR_PARAMETER_WRITE             2
#define         DDR_PARAMETER_LEFT              1
#define         DDR_PARAMETER_RIGHT             2

#define   REGISTER_READ    1
#define   REGISTER_WRITE  0

typedef struct ddr_test_struct {
	unsigned int	ddr_data_source;
	unsigned int	ddr_data_test_size;
	unsigned int	ddr_address_test_size;
	unsigned int	ddr_test_watchdog_times_s;
	unsigned int	ddr_test_lane_disable;

	unsigned int	ddr_test_window_flag[8];
	unsigned int	ddr_test_window_data[100];
} ddr_test_struct_t;
ddr_test_struct_t *g_ddr_test_struct;

unsigned int read_write_window_test_parameter(unsigned int	source_index,
					      unsigned int	parameter_index,
					      unsigned int	parameter_value,
					      unsigned int	read_write_flag)
{
	if (source_index == DDR_PARAMETER_SOURCE_FROM_DMC_STICKY) {
		sticky_reg_base_add = (p_ddr_base->ddr_dmc_sticky0 & 0xffff);

		if (read_write_flag == DDR_PARAMETER_WRITE)
			wr_reg((sticky_reg_base_add + (parameter_index << 2)), parameter_value);
		if (read_write_flag == DDR_PARAMETER_READ)
			parameter_value = rd_reg((sticky_reg_base_add + (parameter_index << 2)));
	}

	if (source_index == DDR_PARAMETER_SOURCE_FROM_UBOOT_ENV) {
		char *pre_env_name = "ddr_test_data_num";
		char *env_name = "ddr_test_data_num_0000";
		char *str_buf = NULL;
		char *temp_s = NULL;
		char *endp = NULL;
		char buf[1024];

		str_buf = (char *)(&buf);
		memset(str_buf, 0, sizeof(buf));
		sprintf(env_name, "%s_%04d", pre_env_name, parameter_index);
		sprintf(buf, "0x%8x", parameter_value);

		if (read_write_flag == DDR_PARAMETER_WRITE) {
			setenv(env_name, buf);
			run_command("save", 0);
		}
		if (read_write_flag == DDR_PARAMETER_READ) {
			temp_s = getenv(env_name);
			if (temp_s)
				parameter_value = simple_strtoull_ddr(temp_s, &endp, 0);
			else
				parameter_value = 0;
		}
	}

	if (source_index == DDR_PARAMETER_SOURCE_FROM_ORG_STICKY) {
		sticky_reg_base_add = ((p_ddr_base->preg_sticky_reg0));

		if (read_write_flag == DDR_PARAMETER_WRITE)
			wr_reg((sticky_reg_base_add + (parameter_index << 2)), parameter_value);
		if (read_write_flag == DDR_PARAMETER_READ)
			parameter_value = rd_reg((sticky_reg_base_add + (parameter_index << 2)));
	}
	return parameter_value;
}

unsigned int read_write_window_test_flag(unsigned int	source_index,
					 unsigned int	parameter_index,
					 unsigned int	parameter_value,
					 unsigned int	read_write_flag)
{
	if (source_index == DDR_PARAMETER_SOURCE_FROM_ORG_STICKY) {
		sticky_reg_base_add = p_ddr_base->preg_sticky_reg0;

		if (read_write_flag == DDR_PARAMETER_WRITE)
			wr_reg((sticky_reg_base_add + (parameter_index << 2)), parameter_value);
		if (read_write_flag == DDR_PARAMETER_READ)
			parameter_value = rd_reg((sticky_reg_base_add + (parameter_index << 2)));
	}

	if (source_index == DDR_PARAMETER_SOURCE_FROM_DMC_STICKY) {
		sticky_reg_base_add = (((p_ddr_base->ddr_dmc_sticky0)) & 0xffff);

		if (read_write_flag == DDR_PARAMETER_WRITE)
			wr_reg((sticky_reg_base_add + (parameter_index << 2)), parameter_value);
		if (read_write_flag == DDR_PARAMETER_READ)
			parameter_value = rd_reg((sticky_reg_base_add + (parameter_index << 2)));
	}

	if (source_index == DDR_PARAMETER_SOURCE_FROM_UBOOT_ENV) {
		char *pre_env_name = "ddr_test_data_num";
		char *env_name = "ddr_test_data_num_0000";
		char *str_buf = NULL;
		char *temp_s = NULL;
		char *endp = NULL;
		char buf[1024];

		str_buf = (char *)(&buf);
		memset(str_buf, 0, sizeof(buf));
		sprintf(env_name, "%s_%04d", pre_env_name, parameter_index);
		sprintf(buf, "0x%8x", parameter_value);

		if (read_write_flag == DDR_PARAMETER_WRITE) {
			setenv(env_name, buf);
			run_command("save", 0);
		}
		if (read_write_flag == DDR_PARAMETER_READ) {
			temp_s = getenv(env_name);
			if (temp_s)
				parameter_value = simple_strtoull_ddr(temp_s, &endp, 0);
			else
				parameter_value = 0;
		}
	}

	return parameter_value;
}

void ddr_test_watchdog_init(uint32_t msec)
{
	// src: 24MHz
	// div: 24000 for 1ms
	// reset ao-22 and ee-21
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_add) =
		(1 << 24) | (1 << 25) | (1 << 22) | (1 << 21) | (24000 - 1);

	// set timeout
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_add + 8) =
	msec;
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_add + 12) =
	 0;

	// enable
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_add) |=
	 (1 << 18);
}

void ddr_test_watchdog_clear(void)
{
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_add + 12) =
	 0;
}

void ddr_test_watchdog_disable(void)
{
	// turn off internal counter and disable
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_add) &=
		~((1 << 18) | (1 << 25));
}

void ddr_test_watchdog_enable(uint32_t sec)
{
	// src: 24MHz
	// div: 24000 for 1ms
	// reset ao-22 and ee-21
#if (CONFIG_DDR_PHY > P_DDR_PHY_DEFAULT)
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_add) =
		(1 << 24) | (1 << 25) | (1 << 23) | (1 << 21) | (240000 - 1); //10ms
	if (sec * 100 > 0xffff)
		*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_add + 8) =
			0xffff;
	else
		*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_add + 8) =
			sec * 100;  //max 655s
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_add + 12) = 0;

	// enable
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_add) =
		(*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_add)) |
		(1 << 18);
#endif
	printf("\nP_WATCHDOG_ENABLE\n");
}

void ddr_test_watchdog_reset_system(void)
{
	int i;

	if (p_ddr_base->sys_watchdog_enable_value == 0) {
		p_ddr_base->sys_watchdog_enable_value = 0x3 | (1 << 21) // sys reset en
							| (1 << 23)     // interrupt en
							| (1 << 24)     // clk en
							| (1 << 25)     // clk div en
							| (1 << 26);    // sys reset now
	}
	writel(0, (p_ddr_base->sys_watchdog_add + 12));
	while (1) {
		writel(p_ddr_base->sys_watchdog_enable_value
		       , (p_ddr_base->sys_watchdog_add));
		writel(0, (p_ddr_base->sys_watchdog_add + 12));
		writel((p_ddr_base->sys_watchdog_enable_value) | (1 << 18),
		       (p_ddr_base->sys_watchdog_add)); // watchdog en
		for (i = 0; i < 100; i++)
			readl((p_ddr_base->sys_watchdog_add));
	}
}

static void ddr_write(void *buff, unsigned int m_length)
{
	unsigned int *p;
	unsigned int i, j, n;
	unsigned int m_len = m_length;

	p = (unsigned int *)buff;

	while (m_len) {
		for (j = 0; j < 32; j++) {
			if (m_len >= 128)
				n = 32;
			else
				n = m_len >> 2;

			for (i = 0; i < n; i++) {
#ifdef DDR_PREFETCH_CACHE
				ddr_pld_cache(p);
#endif
				switch (i) {
				case 0:
				case 9:
				case 14:
				case 25:
				case 30:
					*(p + i) = TDATA32F;
					break;
				case 1:
				case 6:
				case 8:
				case 17:
				case 22:
					*(p + i) = 0;
					break;
				case 16:
				case 23:
				case 31:
					*(p + i) = TDATA32A;
					break;
				case 7:
				case 15:
				case 24:
					*(p + i) = TDATA325;
					break;
				case 2:
				case 4:
				case 10:
				case 12:
				case 19:
				case 21:
				case 27:
				case 29:
					*(p + i) = 1 << j;
					break;
				case 3:
				case 5:
				case 11:
				case 13:
				case 18:
				case 20:
				case 26:
				case 28:
					*(p + i) = ~(1 << j);
					break;
				}
			}

			if (m_len > 128) {
				m_len -= 128;
				p += 32;
			} else {
				p += (m_len >> 2);
				m_len = 0;
				break;
			}
		}
	}
}

static void ddr_read(void *buff, unsigned int m_length)
{
	unsigned int *p;
	unsigned int i, j, n;
	unsigned int m_len = m_length;

	p = (unsigned int *)buff;

	while (m_len) {
		for (j = 0; j < 32; j++) {
			if (m_len >= 128)
				n = 32;
			else
				n = m_len >> 2;

			for (i = 0; i < n; i++) {
#ifdef DDR_PREFETCH_CACHE
				ddr_pld_cache(p);
#endif
				if ((error_outof_count_flag) && (error_count)) {
					printf("Error data out of count");
					m_len = 0;
					break;
				}
				switch (i) {
				case 0:
				case 9:
				case 14:
				case 25:
				case 30:
					if (*(p + i) != TDATA32F) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), TDATA32F);
					}
					break;
				case 1:
				case 6:
				case 8:
				case 17:
				case 22:
					if (*(p + i) != 0) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0);
					}
					break;
				case 16:
				case 23:
				case 31:
					if (*(p + i) != TDATA32A) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), TDATA32A);
					}
					break;
				case 7:
				case 15:
				case 24:
					if (*(p + i) != TDATA325) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), TDATA325);
					}
					break;
				case 2:
				case 4:
				case 10:
				case 12:
				case 19:
				case 21:
				case 27:
				case 29:
					if (*(p + i) != 1 << j) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 1 << j);
					}
					break;
				case 3:
				case 5:
				case 11:
				case 13:
				case 18:
				case 20:
				case 26:
				case 28:
					if (*(p + i) != ~(1 << j)) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~(1 << j));
					}
					break;
				}
			}

			if (m_len > 128) {
				m_len -= 128;
				p += 32;
			} else {
				p += (m_len >> 2);
				m_len = 0;
				break;
			}
		}
	}
}

static void ddr_write4(void *buff, unsigned int m_length)
{
	unsigned int *p;
	unsigned int i, j, n;
	unsigned int m_len = m_length;

	p = (unsigned int *)buff;

	while (m_len) {
		for (j = 0; j < 32; j++) {
			if (m_len >= 128)
				n = 32;
			else
				n = m_len >> 2;

			for (i = 0; i < n; i++) {
#ifdef DDR_PREFETCH_CACHE
				ddr_pld_cache(p);
#endif
				switch (i) {
				case 0:
				case 1:
				case 2:
				case 3:
					*(p + i) = 0xff00ff00;
					break;
				case 4:
				case 5:
				case 6:
				case 7:
					*(p + i) = ~0xff00ff00;
					break;
				case 8:
				case 9:
				case 10:
				case 11:
					*(p + i) = 0xaa55aa55;
					break;
				case 12:
				case 13:
				case 14:
				case 15:
					*(p + i) = ~0xaa55aa55;
					break;
				case 16:
				case 17:
				case 18:
				case 19:

				case 24:
				case 25:
				case 26:
				case 27:
					*(p + i) = 1 << j;
					break;
				case 20:
				case 21:
				case 22:
				case 23:
				case 28:
				case 29:
				case 30:
				case 31:
					*(p + i) = ~(1 << j);
					break;
				}
			}

			if (m_len > 128) {
				m_len -= 128;
				p += 32;
			} else {
				p += (m_len >> 2);
				m_len = 0;
				break;
			}
		}
	}
}

static void ddr_read4(void *buff, unsigned int m_length)
{
	unsigned int *p;
	unsigned int i, j, n;
	unsigned int m_len = m_length;

	p = (unsigned int *)buff;

	while (m_len) {
		for (j = 0; j < 32; j++) {
			if (m_len >= 128)
				n = 32;
			else
				n = m_len >> 2;

			for (i = 0; i < n; i++) {
#ifdef DDR_PREFETCH_CACHE
				ddr_pld_cache(p);
#endif
				if ((error_outof_count_flag) && (error_count)) {
					printf("Error data out of count");
					m_len = 0;
					break;
				}
				switch (i) {
				case 0:
				case 1:
				case 2:
				case 3:
					if (*(p + i) != 0xff00ff00) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), TDATA32F);
					}
					break;
				case 4:
				case 5:
				case 6:
				case 7:
					if (*(p + i) != ~0xff00ff00) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), TDATA32F);
					}
					break;
				case 8:
				case 9:
				case 10:
				case 11:
					if (*(p + i) != 0xaa55aa55) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), TDATA32F);
					}
					break;
				case 12:
				case 13:
				case 14:
				case 15:
					if (*(p + i) != ~0xaa55aa55) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), TDATA32F);
					}
					break;
				case 16:
				case 17:
				case 18:
				case 19:

				case 24:
				case 25:
				case 26:
				case 27:
					if (*(p + i) != (1 << j)) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), TDATA32F);
					}
					break;
				case 20:
				case 21:
				case 22:
				case 23:
				case 28:
				case 29:
				case 30:
				case 31:
					if (*(p + i) != ~(1 << j)) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), TDATA32F);
					}
					break;
				}
			}

			if (m_len > 128) {
				m_len -= 128;
				p += 32;
			} else {
				p += (m_len >> 2);
				m_len = 0;
				break;
			}
		}
	}
}

static void ddr_read_full(void *buff, unsigned int m_length, unsigned int start_pattern,
			  unsigned int pattern_offset)
{
	unsigned int *p;
	unsigned int i = 0;
	unsigned int m_len = m_length & 0xfffffffc;

	p = (unsigned int *)buff;
	while (m_len) {
		m_len = m_len - 4;

#ifdef DDR_PREFETCH_CACHE
		ddr_pld_cache(p + i);
#endif
		if ((error_outof_count_flag) && (error_count)) {
			printf("Error data out of count");
			m_len = 0;
			break;
		}
		if ((*(p + i)) != (start_pattern + pattern_offset * i)) {
			error_count++;
			printf("Error data [0x%8x] add 0x%8x[0x%8x]\n", *(p + i),
			       p_convert_int(p + i),
			       (start_pattern + pattern_offset * i));
		}
		//break;
		i++;
	}
}

static void ddr_write_full(void *buff, unsigned int m_length, unsigned int start_pattern,
			   unsigned int pattern_offset)
{
	unsigned int *p;
	unsigned int i = 0;
	unsigned int m_len = m_length & 0xfffffffc;

	p = (unsigned int *)buff;
	while (m_len) {
		m_len = m_len - 4;
		*(p + i) = start_pattern + pattern_offset * i;
		i++;
	}
}

static void ddr_test_copy(void *addr_dest, void *addr_src, unsigned int memcpy_size)
{
	unsigned int *p_dest;
	unsigned int *p_src;

	unsigned int m_len = memcpy_size;

	p_dest = (unsigned int *)addr_dest;
	p_src = (unsigned int *)addr_src;
	m_len = m_len / 4; //assume it's multiple of 4
	while (m_len--) {
		ddr_pld_cache(p_src);
		//#define ddr_pld_cache(P)   asm ("prfm PLDL1KEEP, [%0, #376]"::"r" (P))
		*p_dest++ = *p_src++;
		*p_dest++ = *p_src++;
		*p_dest++ = *p_src++;
		*p_dest++ = *p_src++;
	}
}

int do_ddr_test_copy(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	check_base_address();
	char *endp = NULL;
	unsigned long loop = 1;
	unsigned int print_flag = 1;
	unsigned int src_addr = DDR_TEST_START_ADDR;
	unsigned int dec_addr = DDR_TEST_START_ADDR + 0x8000000;
	unsigned int test_size = DDR_TEST_SIZE;

	print_flag = 1;

	printf("\nargc== 0x%8x\n", argc);
	int i;

	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);

	if (argc == 1) {
		src_addr = DDR_TEST_START_ADDR;
		loop = 1;
	}

	if (argc > 2)
		if (*argv[2] == 0)
			src_addr = DDR_TEST_START_ADDR;

	if (argc > 3) {
		src_addr = simple_strtoull_ddr(argv[1], &endp, 16);
		dec_addr = simple_strtoull_ddr(argv[2], &endp, 16);
		test_size = simple_strtoull_ddr(argv[3], &endp, 16);
		loop = 1;
		if (*argv[3] == 0 || *endp != 0)
			test_size = DDR_TEST_SIZE;
	}
	if (test_size < 0x1000)
		test_size = DDR_TEST_SIZE;
	if (argc > 4) {
		loop = simple_strtoull_ddr(argv[4], &endp, 16);
		if (*argv[4] == 0 || *endp != 0)
			loop = 1;
	}
	if (argc > 5) {
		print_flag = simple_strtoull_ddr(argv[5], &endp, 16);
		if (*argv[5] == 0 || *endp != 0)
			print_flag = 1;
	}

	unsigned long time_start, time_end, test_loops;

	test_loops = loop;
	unsigned long size_count = 0;

	size_count = loop * test_size;
	time_start = get_us_time(); //us

	do {
		ddr_test_copy((void *)(int_convert_p(dec_addr)),
			      (void *)(int_convert_p(src_addr)), test_size);
		if (print_flag) {
			printf("\nloop=0x%8x", (unsigned int)loop);
			printf("\n\n");
		}
	} while (--loop);
	time_end = get_us_time(); //us
	printf("\ncopy %d times use %dus\n\n",
	       (unsigned int)test_loops, (unsigned int)(time_end - time_start));

	printf("\nddr copy bandwidth==%d MBYTE/S\n\n",
	       (unsigned int)(size_count / (time_end - time_start)));
	printf("\rEnd ddr test.\n");

	unsigned int m_len = 0, counter = 0;
	unsigned int *p_dest;

	p_dest = (void *)(int_convert_p(dec_addr));
	m_len = test_size / 4; //assume it's multiple of 4
	counter = (unsigned int)test_loops;
	size_count = counter * test_size;
	time_start = get_us_time(); //us
	do {
		loop = 1;
		m_len = test_size / 4;
		while (m_len--) {
			ddr_pld_cache(p_dest);
			*p_dest++ = 0x12345678;
			*p_dest++ = 0x12345678;
			*p_dest++ = 0x12345678;
			*p_dest++ = 0x12345678;
		}
	} while (--counter);
	time_end = get_us_time(); //us
	printf("\nwrite %d bytes use %dus\n\n",
	       (unsigned int)test_size, (unsigned int)(time_end - time_start));

	printf("\nddr write bandwidth==%d MBYTE/S\n\n",
	       (unsigned int)(size_count / (time_end - time_start)));

	unsigned int *p_src;

	p_src = (void *)(int_convert_p(src_addr));
	m_len = test_size / 4; //assume it's multiple of 4
	unsigned int temp0 = 0;

	counter = (unsigned int)test_loops;
	size_count = counter * test_size;

	time_start = get_us_time(); //us
	do {
		loop = 1;
		m_len = test_size / 4;
		while (m_len--) {
#ifdef DDR_PREFETCH_CACHE
			__asm__("prfm PLDL1KEEP, [%0, #376]" : : "r" (p_src));
#endif
			p_src++;
			temp0 = (*p_src);
			m_len--;
			m_len--;
			m_len--;
			m_len--;
			m_len--;
			m_len--;
			m_len--;
		}
	} while (--counter);
	*p_dest++ = temp0;
	*p_dest++ = *p_src;
	*p_dest++ = *p_src;
	*p_dest++ = *p_src;
	time_end = get_us_time(); //us

	printf("\nread %d Kbytes use %dus\n\n",
	       (unsigned int)(size_count / 1000), (unsigned int)(time_end - time_start));
	printf("\nddr read bandwidth==%d MBYTE/S\n\n",
	       (unsigned int)(size_count / (time_end - time_start)));

	return 0;
}

U_BOOT_CMD(ddr_test_copy, 7, 1, do_ddr_test_copy,
	   "ddr_test_copy function",
	   "ddr_test_copy  0x08000000 0x10000000 0x02000000 1 0 ?\n");

#define DDR_PATTERN_LOOP_1 32
#define DDR_PATTERN_LOOP_2 64
#define DDR_PATTERN_LOOP_3 96

static void ddr_write_pattern4_cross_talk_p(void *buff, unsigned int m_length)
{
	unsigned int *p;
	unsigned int i, n;
	unsigned int m_len = m_length;

	p = (unsigned int *)buff;

	while (m_len) {
		{
			if (m_len >= 128 * 4)
				n = 32 * 4;
			else
				n = m_len >> 2;

			for (i = 0; i < n; i++) {
#ifdef DDR_PREFETCH_CACHE
				ddr_pld_cache(p);
#endif
				switch (i) {
				case 0:
				case 1:
				case 2:
				case 3:
				case 8:
				case 9:
				case 10:
				case 11:
				case 16:
				case 17:
				case 18:
				case 19:
				case 24:
				case 25:
				case 26:
				case 27:
					*(p + i) = TDATA32F;
					break;
				case 4:
				case 5:
				case 6:
				case 7:
				case 12:
				case 13:
				case 14:
				case 15:
				case 20:
				case 21:
				case 22:
				case 23:
				case 28:
				case 29:
				case 30:
				case 31:
					*(p + i) = 0;
					break;
				case DDR_PATTERN_LOOP_1 + 0:
				case DDR_PATTERN_LOOP_1 + 1:
				case DDR_PATTERN_LOOP_1 + 2:
				case DDR_PATTERN_LOOP_1 + 3:
				case DDR_PATTERN_LOOP_1 + 8:
				case DDR_PATTERN_LOOP_1 + 9:
				case DDR_PATTERN_LOOP_1 + 10:
				case DDR_PATTERN_LOOP_1 + 11:
				case DDR_PATTERN_LOOP_1 + 16:
				case DDR_PATTERN_LOOP_1 + 17:
				case DDR_PATTERN_LOOP_1 + 18:
				case DDR_PATTERN_LOOP_1 + 19:
				case DDR_PATTERN_LOOP_1 + 24:
				case DDR_PATTERN_LOOP_1 + 25:
				case DDR_PATTERN_LOOP_1 + 26:
				case DDR_PATTERN_LOOP_1 + 27:
					*(p + i) = TDATA32A;
					break;
				case DDR_PATTERN_LOOP_1 + 4:
				case DDR_PATTERN_LOOP_1 + 5:
				case DDR_PATTERN_LOOP_1 + 6:
				case DDR_PATTERN_LOOP_1 + 7:
				case DDR_PATTERN_LOOP_1 + 12:
				case DDR_PATTERN_LOOP_1 + 13:
				case DDR_PATTERN_LOOP_1 + 14:
				case DDR_PATTERN_LOOP_1 + 15:
				case DDR_PATTERN_LOOP_1 + 20:
				case DDR_PATTERN_LOOP_1 + 21:
				case DDR_PATTERN_LOOP_1 + 22:
				case DDR_PATTERN_LOOP_1 + 23:
				case DDR_PATTERN_LOOP_1 + 28:
				case DDR_PATTERN_LOOP_1 + 29:
				case DDR_PATTERN_LOOP_1 + 30:
				case DDR_PATTERN_LOOP_1 + 31:
					*(p + i) = TDATA325;
					break;
				case DDR_PATTERN_LOOP_2 + 0:
				case DDR_PATTERN_LOOP_2 + 1:
				case DDR_PATTERN_LOOP_2 + 2:
				case DDR_PATTERN_LOOP_2 + 3:
					*(p + i) = 0xfe01fe01;
					break;
				case DDR_PATTERN_LOOP_2 + 4:
				case DDR_PATTERN_LOOP_2 + 5:
				case DDR_PATTERN_LOOP_2 + 6:
				case DDR_PATTERN_LOOP_2 + 7:
					*(p + i) = 0xfd02fd02;
					break;
				case DDR_PATTERN_LOOP_2 + 8:
				case DDR_PATTERN_LOOP_2 + 9:
				case DDR_PATTERN_LOOP_2 + 10:
				case DDR_PATTERN_LOOP_2 + 11:
					*(p + i) = 0xfb04fb04;
					break;
				case DDR_PATTERN_LOOP_2 + 12:
				case DDR_PATTERN_LOOP_2 + 13:
				case DDR_PATTERN_LOOP_2 + 14:
				case DDR_PATTERN_LOOP_2 + 15:
					*(p + i) = 0xf708f708;
					break;
				case DDR_PATTERN_LOOP_2 + 16:
				case DDR_PATTERN_LOOP_2 + 17:
				case DDR_PATTERN_LOOP_2 + 18:
				case DDR_PATTERN_LOOP_2 + 19:
					*(p + i) = 0xef10ef10;
					break;
				case DDR_PATTERN_LOOP_2 + 20:
				case DDR_PATTERN_LOOP_2 + 21:
				case DDR_PATTERN_LOOP_2 + 22:
				case DDR_PATTERN_LOOP_2 + 23:
					*(p + i) = 0xdf20df20;
					break;
				case DDR_PATTERN_LOOP_2 + 24:
				case DDR_PATTERN_LOOP_2 + 25:
				case DDR_PATTERN_LOOP_2 + 26:
				case DDR_PATTERN_LOOP_2 + 27:
					*(p + i) = 0xbf40bf40;
					break;
				case DDR_PATTERN_LOOP_2 + 28:
				case DDR_PATTERN_LOOP_2 + 29:
				case DDR_PATTERN_LOOP_2 + 30:
				case DDR_PATTERN_LOOP_2 + 31:
					*(p + i) = 0x7f807f80;
					break;
				case DDR_PATTERN_LOOP_3 + 0:
				case DDR_PATTERN_LOOP_3 + 1:
				case DDR_PATTERN_LOOP_3 + 2:
				case DDR_PATTERN_LOOP_3 + 3:
					*(p + i) = 0x00000100;
					break;
				case DDR_PATTERN_LOOP_3 + 4:
				case DDR_PATTERN_LOOP_3 + 5:
				case DDR_PATTERN_LOOP_3 + 6:
				case DDR_PATTERN_LOOP_3 + 7:
					*(p + i) = 0x00000200;
					break;
				case DDR_PATTERN_LOOP_3 + 8:
				case DDR_PATTERN_LOOP_3 + 9:
				case DDR_PATTERN_LOOP_3 + 10:
				case DDR_PATTERN_LOOP_3 + 11:
					*(p + i) = 0x00000400;
					break;
				case DDR_PATTERN_LOOP_3 + 12:
				case DDR_PATTERN_LOOP_3 + 13:
				case DDR_PATTERN_LOOP_3 + 14:
				case DDR_PATTERN_LOOP_3 + 15:
					*(p + i) = 0x00000800;
					break;
				case DDR_PATTERN_LOOP_3 + 16:
				case DDR_PATTERN_LOOP_3 + 17:
				case DDR_PATTERN_LOOP_3 + 18:
				case DDR_PATTERN_LOOP_3 + 19:
					*(p + i) = 0x00001000;
					break;
				case DDR_PATTERN_LOOP_3 + 20:
				case DDR_PATTERN_LOOP_3 + 21:
				case DDR_PATTERN_LOOP_3 + 22:
				case DDR_PATTERN_LOOP_3 + 23:
					*(p + i) = 0x00002000;
					break;
				case DDR_PATTERN_LOOP_3 + 24:
				case DDR_PATTERN_LOOP_3 + 25:
				case DDR_PATTERN_LOOP_3 + 26:
				case DDR_PATTERN_LOOP_3 + 27:
					*(p + i) = 0x00004000;
					break;
				case DDR_PATTERN_LOOP_3 + 28:
				case DDR_PATTERN_LOOP_3 + 29:
				case DDR_PATTERN_LOOP_3 + 30:
				case DDR_PATTERN_LOOP_3 + 31:
					*(p + i) = 0x00008000;
					break;
				}
			}

			if (m_len > (128 * 4)) {
				m_len -= (128 * 4);
				p += 32 * 4;
			} else {
				p += (m_len >> 2);
				m_len = 0;
				break;
			}
		}
	}
}

static void ddr_write_pattern4_cross_talk_p2(void *buff, unsigned int m_length)
{
	unsigned int *p;
	unsigned int i, n;
	unsigned int m_len = m_length;

	p = (unsigned int *)buff;

	while (m_len) {
		{
			if (m_len >= 128 * 4)
				n = 32 * 4;
			else
				n = m_len >> 2;

			for (i = 0; i < n; i++) {
#ifdef DDR_PREFETCH_CACHE
				ddr_pld_cache(p);
#endif

				switch (i) {
				case 0:
				case DDR_PATTERN_LOOP_1 + 1:
				case DDR_PATTERN_LOOP_2 + 2:
				case DDR_PATTERN_LOOP_3 + 3:
					*(p + i) = 0xfe01fe01;
					break;
				case 4:
				case DDR_PATTERN_LOOP_1 + 5:
				case DDR_PATTERN_LOOP_2 + 6:
				case DDR_PATTERN_LOOP_3 + 7:
					*(p + i) = 0xfd02fd02;
					break;

				case 8:
				case DDR_PATTERN_LOOP_1 + 9:
				case DDR_PATTERN_LOOP_2 + 10:
				case DDR_PATTERN_LOOP_3 + 11:
					*(p + i) = 0xfb04fb04;
					break;

				case 12:
				case DDR_PATTERN_LOOP_1 + 13:
				case DDR_PATTERN_LOOP_2 + 14:
				case DDR_PATTERN_LOOP_3 + 15:
					*(p + i) = 0xf708f708;
					break;

				case 16:
				case DDR_PATTERN_LOOP_1 + 17:
				case DDR_PATTERN_LOOP_2 + 18:
				case DDR_PATTERN_LOOP_3 + 19:
					*(p + i) = 0xef10ef10;
					break;

				case 20:
				case DDR_PATTERN_LOOP_1 + 21:
				case DDR_PATTERN_LOOP_2 + 22:
				case DDR_PATTERN_LOOP_3 + 23:
					*(p + i) = 0xdf20df20;
					break;

				case 24:
				case DDR_PATTERN_LOOP_1 + 25:
				case DDR_PATTERN_LOOP_2 + 26:
				case DDR_PATTERN_LOOP_3 + 27:
					*(p + i) = 0xbf40bf40;
					break;

				case 28:
				case DDR_PATTERN_LOOP_1 + 29:
				case DDR_PATTERN_LOOP_2 + 30:
				case DDR_PATTERN_LOOP_3 + 31:
					*(p + i) = 0x7f807f80;
					break;

				default:

					*(p + i) = 0xff00ff00;
					break;

					break;
				}
			}

			if (m_len > (128 * 4)) {
				m_len -= (128 * 4);
				p += 32 * 4;
			} else {
				p += (m_len >> 2);
				m_len = 0;
				break;
			}
		}
	}
}

static void ddr_read_pattern4_cross_talk_p(void *buff, unsigned int m_length)
{
	unsigned int *p;
	unsigned int i, n;
	unsigned int m_len = m_length;

	p = (unsigned int *)buff;

	while (m_len) {
		{
			if (m_len >= 128 * 4)
				n = 32 * 4;
			else
				n = m_len >> 2;

			for (i = 0; i < n; i++) {
#ifdef DDR_PREFETCH_CACHE
				ddr_pld_cache(p);
#endif
				if ((error_outof_count_flag) && (error_count)) {
					printf("Error data out of count");
					m_len = 0;
					break;
				}

				switch (i) {
				case 0:
				case 1:
				case 2:
				case 3:
				case 8:
				case 9:
				case 10:
				case 11:
				case 16:
				case 17:
				case 18:
				case 19:
				case 24:
				case 25:
				case 26:
				case 27:
					if (*(p + i) != TDATA32F) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), TDATA32F);
						break;
					}
					break;
				case 4:
				case 5:
				case 6:
				case 7:
				case 12:
				case 13:
				case 14:
				case 15:
				case 20:
				case 21:
				case 22:
				case 23:
				case 28:
				case 29:
				case 30:
				case 31:
					if (*(p + i) != 0) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_1 + 0:
				case DDR_PATTERN_LOOP_1 + 1:
				case DDR_PATTERN_LOOP_1 + 2:
				case DDR_PATTERN_LOOP_1 + 3:
				case DDR_PATTERN_LOOP_1 + 8:
				case DDR_PATTERN_LOOP_1 + 9:
				case DDR_PATTERN_LOOP_1 + 10:
				case DDR_PATTERN_LOOP_1 + 11:
				case DDR_PATTERN_LOOP_1 + 16:
				case DDR_PATTERN_LOOP_1 + 17:
				case DDR_PATTERN_LOOP_1 + 18:
				case DDR_PATTERN_LOOP_1 + 19:
				case DDR_PATTERN_LOOP_1 + 24:
				case DDR_PATTERN_LOOP_1 + 25:
				case DDR_PATTERN_LOOP_1 + 26:
				case DDR_PATTERN_LOOP_1 + 27:
					if (*(p + i) != TDATA32A) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), TDATA32A);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_1 + 4:
				case DDR_PATTERN_LOOP_1 + 5:
				case DDR_PATTERN_LOOP_1 + 6:
				case DDR_PATTERN_LOOP_1 + 7:
				case DDR_PATTERN_LOOP_1 + 12:
				case DDR_PATTERN_LOOP_1 + 13:
				case DDR_PATTERN_LOOP_1 + 14:
				case DDR_PATTERN_LOOP_1 + 15:
				case DDR_PATTERN_LOOP_1 + 20:
				case DDR_PATTERN_LOOP_1 + 21:
				case DDR_PATTERN_LOOP_1 + 22:
				case DDR_PATTERN_LOOP_1 + 23:
				case DDR_PATTERN_LOOP_1 + 28:
				case DDR_PATTERN_LOOP_1 + 29:
				case DDR_PATTERN_LOOP_1 + 30:
				case DDR_PATTERN_LOOP_1 + 31:
					if (*(p + i) != TDATA325) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), TDATA325);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 0:
				case DDR_PATTERN_LOOP_2 + 1:
				case DDR_PATTERN_LOOP_2 + 2:
				case DDR_PATTERN_LOOP_2 + 3:
					if (*(p + i) != 0xfe01fe01) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xfe01fe01);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 4:
				case DDR_PATTERN_LOOP_2 + 5:
				case DDR_PATTERN_LOOP_2 + 6:
				case DDR_PATTERN_LOOP_2 + 7:
					if (*(p + i) != 0xfd02fd02) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xfd02fd02);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 8:
				case DDR_PATTERN_LOOP_2 + 9:
				case DDR_PATTERN_LOOP_2 + 10:
				case DDR_PATTERN_LOOP_2 + 11:
					if (*(p + i) != 0xfb04fb04) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xfb04fb04);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 12:
				case DDR_PATTERN_LOOP_2 + 13:
				case DDR_PATTERN_LOOP_2 + 14:
				case DDR_PATTERN_LOOP_2 + 15:
					if (*(p + i) != 0xf708f708) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xf708f708);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 16:
				case DDR_PATTERN_LOOP_2 + 17:
				case DDR_PATTERN_LOOP_2 + 18:
				case DDR_PATTERN_LOOP_2 + 19:
					if (*(p + i) != 0xef10ef10) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xef10ef10);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 20:
				case DDR_PATTERN_LOOP_2 + 21:
				case DDR_PATTERN_LOOP_2 + 22:
				case DDR_PATTERN_LOOP_2 + 23:
					if (*(p + i) != 0xdf20df20) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xdf20df20);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 24:
				case DDR_PATTERN_LOOP_2 + 25:
				case DDR_PATTERN_LOOP_2 + 26:
				case DDR_PATTERN_LOOP_2 + 27:
					if (*(p + i) != 0xbf40bf40) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xbf40bf40);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 28:
				case DDR_PATTERN_LOOP_2 + 29:
				case DDR_PATTERN_LOOP_2 + 30:
				case DDR_PATTERN_LOOP_2 + 31:
					if (*(p + i) != 0x7f807f80) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0x7f807f80);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_3 + 0:
				case DDR_PATTERN_LOOP_3 + 1:
				case DDR_PATTERN_LOOP_3 + 2:
				case DDR_PATTERN_LOOP_3 + 3:
					if (*(p + i) != 0x00000100) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0x00000100);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_3 + 4:
				case DDR_PATTERN_LOOP_3 + 5:
				case DDR_PATTERN_LOOP_3 + 6:
				case DDR_PATTERN_LOOP_3 + 7:
					if (*(p + i) != 0x00000200) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0x00000200);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_3 + 8:
				case DDR_PATTERN_LOOP_3 + 9:
				case DDR_PATTERN_LOOP_3 + 10:
				case DDR_PATTERN_LOOP_3 + 11:
					if (*(p + i) != 0x00000400) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0x00000400);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_3 + 12:
				case DDR_PATTERN_LOOP_3 + 13:
				case DDR_PATTERN_LOOP_3 + 14:
				case DDR_PATTERN_LOOP_3 + 15:
					if (*(p + i) != 0x00000800) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0x00000800);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_3 + 16:
				case DDR_PATTERN_LOOP_3 + 17:
				case DDR_PATTERN_LOOP_3 + 18:
				case DDR_PATTERN_LOOP_3 + 19:
					if (*(p + i) != 0x00001000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0x00001000);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_3 + 20:
				case DDR_PATTERN_LOOP_3 + 21:
				case DDR_PATTERN_LOOP_3 + 22:
				case DDR_PATTERN_LOOP_3 + 23:
					if (*(p + i) != 0x00002000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0x00002000);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 24:
				case DDR_PATTERN_LOOP_3 + 25:
				case DDR_PATTERN_LOOP_3 + 26:
				case DDR_PATTERN_LOOP_3 + 27:
					if (*(p + i) != 0x00004000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0x00004000);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_3 + 28:
				case DDR_PATTERN_LOOP_3 + 29:
				case DDR_PATTERN_LOOP_3 + 30:
				case DDR_PATTERN_LOOP_3 + 31:
					if (*(p + i) != 0x00008000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0x00008000);
						break;
					}
					break;
				}
			}

			if (m_len > 128 * 4) {
				m_len -= 128 * 4;
				p += 32 * 4;
			} else {
				p += (m_len >> 2);
				m_len = 0;
				break;
			}
		}
	}
}

static void ddr_read_pattern4_cross_talk_p2(void *buff, unsigned int m_length)
{
	unsigned int *p;
	unsigned int i, n;
	unsigned int m_len = m_length;

	p = (unsigned int *)buff;

	while (m_len) {
		{
			if (m_len >= 128 * 4)
				n = 32 * 4;
			else
				n = m_len >> 2;

			for (i = 0; i < n; i++) {
#ifdef DDR_PREFETCH_CACHE
				ddr_pld_cache(p);
#endif
				if ((error_outof_count_flag) && (error_count)) {
					printf("Error data out of count");
					m_len = 0;
					break;
				}

				switch (i) {
				case 0:
				case DDR_PATTERN_LOOP_1 + 1:
				case DDR_PATTERN_LOOP_2 + 2:
				case DDR_PATTERN_LOOP_3 + 3:
					if (*(p + i) != 0xfe01fe01) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xfe01fe01);
						break;
					}
					break;
				case 4:
				case DDR_PATTERN_LOOP_1 + 5:
				case DDR_PATTERN_LOOP_2 + 6:
				case DDR_PATTERN_LOOP_3 + 7:
					if (*(p + i) != 0xfd02fd02) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xfd02fd02);
						break;
					}
					break;

				case 8:
				case DDR_PATTERN_LOOP_1 + 9:
				case DDR_PATTERN_LOOP_2 + 10:
				case DDR_PATTERN_LOOP_3 + 11:
					if (*(p + i) != 0xfb04fb04) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xfb04fb04);
						break;
					}
					break;

				case 12:
				case DDR_PATTERN_LOOP_1 + 13:
				case DDR_PATTERN_LOOP_2 + 14:
				case DDR_PATTERN_LOOP_3 + 15:
					if (*(p + i) != 0xf708f708) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xf708f708);
						break;
					}
					break;

				case 16:
				case DDR_PATTERN_LOOP_1 + 17:
				case DDR_PATTERN_LOOP_2 + 18:
				case DDR_PATTERN_LOOP_3 + 19:
					if (*(p + i) != 0xef10ef10) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xef10ef10);
						break;
					}
					break;

				case 20:
				case DDR_PATTERN_LOOP_1 + 21:
				case DDR_PATTERN_LOOP_2 + 22:
				case DDR_PATTERN_LOOP_3 + 23:
					if (*(p + i) != 0xdf20df20) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xdf20df20);
						break;
					}
					break;

				case 24:
				case DDR_PATTERN_LOOP_1 + 25:
				case DDR_PATTERN_LOOP_2 + 26:
				case DDR_PATTERN_LOOP_3 + 27:
					if (*(p + i) != 0xbf40bf40) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xbf40bf40);
						break;
					}
					break;
				case 28:
				case DDR_PATTERN_LOOP_1 + 29:
				case DDR_PATTERN_LOOP_2 + 30:
				case DDR_PATTERN_LOOP_3 + 31:
					if (*(p + i) != 0x7f807f80) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0x7f807f80);
						break;
					}
					break;

				default:
					if (*(p + i) != 0xff00ff00) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xff00ff00);
						break;
					}
					break;

					break;
				}
			}

			if (m_len > 128 * 4) {
				m_len -= 128 * 4;
				p += 32 * 4;
			} else {
				p += (m_len >> 2);
				m_len = 0;
				break;
			}
		}
	}
}

static void ddr_write_pattern4_cross_talk_n(void *buff, unsigned int m_length)
{
	unsigned int *p;
	unsigned int i, n;
	unsigned int m_len = m_length;

	p = (unsigned int *)buff;

	while (m_len) {
		{
			if (m_len >= 128 * 4)
				n = 32 * 4;
			else
				n = m_len >> 2;

			for (i = 0; i < n; i++) {
#ifdef DDR_PREFETCH_CACHE
				ddr_pld_cache(p);
#endif
				switch (i) {
				case 0:
				case 1:
				case 2:
				case 3:
				case 8:
				case 9:
				case 10:
				case 11:
				case 16:
				case 17:
				case 18:
				case 19:
				case 24:
				case 25:
				case 26:
				case 27:
					*(p + i) = ~TDATA32F;
					break;
				case 4:
				case 5:
				case 6:
				case 7:
				case 12:
				case 13:
				case 14:
				case 15:
				case 20:
				case 21:
				case 22:
				case 23:
				case 28:
				case 29:
				case 30:
				case 31:
					*(p + i) = ~0;
					break;
				case DDR_PATTERN_LOOP_1 + 0:
				case DDR_PATTERN_LOOP_1 + 1:
				case DDR_PATTERN_LOOP_1 + 2:
				case DDR_PATTERN_LOOP_1 + 3:
				case DDR_PATTERN_LOOP_1 + 8:
				case DDR_PATTERN_LOOP_1 + 9:
				case DDR_PATTERN_LOOP_1 + 10:
				case DDR_PATTERN_LOOP_1 + 11:
				case DDR_PATTERN_LOOP_1 + 16:
				case DDR_PATTERN_LOOP_1 + 17:
				case DDR_PATTERN_LOOP_1 + 18:
				case DDR_PATTERN_LOOP_1 + 19:
				case DDR_PATTERN_LOOP_1 + 24:
				case DDR_PATTERN_LOOP_1 + 25:
				case DDR_PATTERN_LOOP_1 + 26:
				case DDR_PATTERN_LOOP_1 + 27:
					*(p + i) = ~TDATA32A;
					break;
				case DDR_PATTERN_LOOP_1 + 4:
				case DDR_PATTERN_LOOP_1 + 5:
				case DDR_PATTERN_LOOP_1 + 6:
				case DDR_PATTERN_LOOP_1 + 7:
				case DDR_PATTERN_LOOP_1 + 12:
				case DDR_PATTERN_LOOP_1 + 13:
				case DDR_PATTERN_LOOP_1 + 14:
				case DDR_PATTERN_LOOP_1 + 15:
				case DDR_PATTERN_LOOP_1 + 20:
				case DDR_PATTERN_LOOP_1 + 21:
				case DDR_PATTERN_LOOP_1 + 22:
				case DDR_PATTERN_LOOP_1 + 23:
				case DDR_PATTERN_LOOP_1 + 28:
				case DDR_PATTERN_LOOP_1 + 29:
				case DDR_PATTERN_LOOP_1 + 30:
				case DDR_PATTERN_LOOP_1 + 31:
					*(p + i) = ~TDATA325;
					break;
				case DDR_PATTERN_LOOP_2 + 0:
				case DDR_PATTERN_LOOP_2 + 1:
				case DDR_PATTERN_LOOP_2 + 2:
				case DDR_PATTERN_LOOP_2 + 3:
					*(p + i) = ~0xfe01fe01;
					break;
				case DDR_PATTERN_LOOP_2 + 4:
				case DDR_PATTERN_LOOP_2 + 5:
				case DDR_PATTERN_LOOP_2 + 6:
				case DDR_PATTERN_LOOP_2 + 7:
					*(p + i) = ~0xfd02fd02;
					break;
				case DDR_PATTERN_LOOP_2 + 8:
				case DDR_PATTERN_LOOP_2 + 9:
				case DDR_PATTERN_LOOP_2 + 10:
				case DDR_PATTERN_LOOP_2 + 11:
					*(p + i) = ~0xfb04fb04;
					break;
				case DDR_PATTERN_LOOP_2 + 12:
				case DDR_PATTERN_LOOP_2 + 13:
				case DDR_PATTERN_LOOP_2 + 14:
				case DDR_PATTERN_LOOP_2 + 15:
					*(p + i) = ~0xf708f708;
					break;
				case DDR_PATTERN_LOOP_2 + 16:
				case DDR_PATTERN_LOOP_2 + 17:
				case DDR_PATTERN_LOOP_2 + 18:
				case DDR_PATTERN_LOOP_2 + 19:
					*(p + i) = ~0xef10ef10;
					break;
				case DDR_PATTERN_LOOP_2 + 20:
				case DDR_PATTERN_LOOP_2 + 21:
				case DDR_PATTERN_LOOP_2 + 22:
				case DDR_PATTERN_LOOP_2 + 23:
					*(p + i) = ~0xdf20df20;
					break;
				case DDR_PATTERN_LOOP_2 + 24:
				case DDR_PATTERN_LOOP_2 + 25:
				case DDR_PATTERN_LOOP_2 + 26:
				case DDR_PATTERN_LOOP_2 + 27:
					*(p + i) = ~0xbf40bf40;
					break;
				case DDR_PATTERN_LOOP_2 + 28:
				case DDR_PATTERN_LOOP_2 + 29:
				case DDR_PATTERN_LOOP_2 + 30:
				case DDR_PATTERN_LOOP_2 + 31:
					*(p + i) = ~0x7f807f80;
					break;
				case DDR_PATTERN_LOOP_3 + 0:
				case DDR_PATTERN_LOOP_3 + 1:
				case DDR_PATTERN_LOOP_3 + 2:
				case DDR_PATTERN_LOOP_3 + 3:
					*(p + i) = ~0x00000100;
					break;
				case DDR_PATTERN_LOOP_3 + 4:
				case DDR_PATTERN_LOOP_3 + 5:
				case DDR_PATTERN_LOOP_3 + 6:
				case DDR_PATTERN_LOOP_3 + 7:
					*(p + i) = ~0x00000200;
					break;
				case DDR_PATTERN_LOOP_3 + 8:
				case DDR_PATTERN_LOOP_3 + 9:
				case DDR_PATTERN_LOOP_3 + 10:
				case DDR_PATTERN_LOOP_3 + 11:
					*(p + i) = ~0x00000400;
					break;
				case DDR_PATTERN_LOOP_3 + 12:
				case DDR_PATTERN_LOOP_3 + 13:
				case DDR_PATTERN_LOOP_3 + 14:
				case DDR_PATTERN_LOOP_3 + 15:
					*(p + i) = ~0x00000800;
					break;
				case DDR_PATTERN_LOOP_3 + 16:
				case DDR_PATTERN_LOOP_3 + 17:
				case DDR_PATTERN_LOOP_3 + 18:
				case DDR_PATTERN_LOOP_3 + 19:
					*(p + i) = ~0x00001000;
					break;
				case DDR_PATTERN_LOOP_3 + 20:
				case DDR_PATTERN_LOOP_3 + 21:
				case DDR_PATTERN_LOOP_3 + 22:
				case DDR_PATTERN_LOOP_3 + 23:
					*(p + i) = ~0x00002000;
					break;
				case DDR_PATTERN_LOOP_3 + 24:
				case DDR_PATTERN_LOOP_3 + 25:
				case DDR_PATTERN_LOOP_3 + 26:
				case DDR_PATTERN_LOOP_3 + 27:
					*(p + i) = ~0x00004000;
					break;
				case DDR_PATTERN_LOOP_3 + 28:
				case DDR_PATTERN_LOOP_3 + 29:
				case DDR_PATTERN_LOOP_3 + 30:
				case DDR_PATTERN_LOOP_3 + 31:
					*(p + i) = ~0x00008000;
					break;
				}
			}

			if (m_len > (128 * 4)) {
				m_len -= (128 * 4);
				p += 32 * 4;
			} else {
				p += (m_len >> 2);
				m_len = 0;
				break;
			}
		}
	}
}

static void ddr_write_pattern4_cross_talk_n2(void *buff, unsigned int m_length)
{
	unsigned int *p;
	unsigned int i, n;
	unsigned int m_len = m_length;

	p = (unsigned int *)buff;

	while (m_len) {
		{
			if (m_len >= 128 * 4)
				n = 32 * 4;
			else
				n = m_len >> 2;

			for (i = 0; i < n; i++) {
#ifdef DDR_PREFETCH_CACHE
				ddr_pld_cache(p);
#endif

				switch (i) {
				case 0:
				case DDR_PATTERN_LOOP_1 + 1:
				case DDR_PATTERN_LOOP_2 + 2:
				case DDR_PATTERN_LOOP_3 + 3:
					*(p + i) = ~0xfe01fe01;
					break;
				case 4:
				case DDR_PATTERN_LOOP_1 + 5:
				case DDR_PATTERN_LOOP_2 + 6:
				case DDR_PATTERN_LOOP_3 + 7:
					*(p + i) = ~0xfd02fd02;
					break;

				case 8:
				case DDR_PATTERN_LOOP_1 + 9:
				case DDR_PATTERN_LOOP_2 + 10:
				case DDR_PATTERN_LOOP_3 + 11:
					*(p + i) = ~0xfb04fb04;
					break;

				case 12:
				case DDR_PATTERN_LOOP_1 + 13:
				case DDR_PATTERN_LOOP_2 + 14:
				case DDR_PATTERN_LOOP_3 + 15:
					*(p + i) = ~0xf708f708;
					break;

				case 16:
				case DDR_PATTERN_LOOP_1 + 17:
				case DDR_PATTERN_LOOP_2 + 18:
				case DDR_PATTERN_LOOP_3 + 19:
					*(p + i) = ~0xef10ef10;
					break;

				case 20:
				case DDR_PATTERN_LOOP_1 + 21:
				case DDR_PATTERN_LOOP_2 + 22:
				case DDR_PATTERN_LOOP_3 + 23:
					*(p + i) = ~0xdf20df20;
					break;

				case 24:
				case DDR_PATTERN_LOOP_1 + 25:
				case DDR_PATTERN_LOOP_2 + 26:
				case DDR_PATTERN_LOOP_3 + 27:
					*(p + i) = ~0xbf40bf40;
					break;
				case 28:
				case DDR_PATTERN_LOOP_1 + 29:
				case DDR_PATTERN_LOOP_2 + 30:
				case DDR_PATTERN_LOOP_3 + 31:
					*(p + i) = ~0x7f807f80;
					break;

				default:

					*(p + i) = ~0xff00ff00;
					break;

					break;
				}
			}

			if (m_len > (128 * 4)) {
				m_len -= (128 * 4);
				p += 32 * 4;
			} else {
				p += (m_len >> 2);
				m_len = 0;
				break;
			}
		}
	}
}

static void ddr_read_pattern4_cross_talk_n(void *buff, unsigned int m_length)
{
	unsigned int *p;
	unsigned int i, n;
	unsigned int m_len = m_length;

	p = (unsigned int *)buff;

	while (m_len) {
		{
			if (m_len >= 128 * 4)
				n = 32 * 4;
			else
				n = m_len >> 2;

			for (i = 0; i < n; i++) {
#ifdef DDR_PREFETCH_CACHE
				ddr_pld_cache(p);
#endif
				if ((error_outof_count_flag) && (error_count)) {
					printf("Error data out of count");
					m_len = 0;
					break;
				}
				switch (i) {
				case 0:
				case 1:
				case 2:
				case 3:
				case 8:
				case 9:
				case 10:
				case 11:
				case 16:
				case 17:
				case 18:
				case 19:
				case 24:
				case 25:
				case 26:
				case 27:
					if (*(p + i) != ~TDATA32F) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~TDATA32F);
						break;
					}
					break;
				case 4:
				case 5:
				case 6:
				case 7:
				case 12:
				case 13:
				case 14:
				case 15:
				case 20:
				case 21:
				case 22:
				case 23:
				case 28:
				case 29:
				case 30:
				case 31:
					if (*(p + i) != ~0) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 0:
				case DDR_PATTERN_LOOP_1 + 1:
				case DDR_PATTERN_LOOP_1 + 2:
				case DDR_PATTERN_LOOP_1 + 3:
				case DDR_PATTERN_LOOP_1 + 8:
				case DDR_PATTERN_LOOP_1 + 9:
				case DDR_PATTERN_LOOP_1 + 10:
				case DDR_PATTERN_LOOP_1 + 11:
				case DDR_PATTERN_LOOP_1 + 16:
				case DDR_PATTERN_LOOP_1 + 17:
				case DDR_PATTERN_LOOP_1 + 18:
				case DDR_PATTERN_LOOP_1 + 19:
				case DDR_PATTERN_LOOP_1 + 24:
				case DDR_PATTERN_LOOP_1 + 25:
				case DDR_PATTERN_LOOP_1 + 26:
				case DDR_PATTERN_LOOP_1 + 27:
					if (*(p + i) != ~TDATA32A) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~TDATA32A);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 4:
				case DDR_PATTERN_LOOP_1 + 5:
				case DDR_PATTERN_LOOP_1 + 6:
				case DDR_PATTERN_LOOP_1 + 7:
				case DDR_PATTERN_LOOP_1 + 12:
				case DDR_PATTERN_LOOP_1 + 13:
				case DDR_PATTERN_LOOP_1 + 14:
				case DDR_PATTERN_LOOP_1 + 15:
				case DDR_PATTERN_LOOP_1 + 20:
				case DDR_PATTERN_LOOP_1 + 21:
				case DDR_PATTERN_LOOP_1 + 22:
				case DDR_PATTERN_LOOP_1 + 23:
				case DDR_PATTERN_LOOP_1 + 28:
				case DDR_PATTERN_LOOP_1 + 29:
				case DDR_PATTERN_LOOP_1 + 30:
				case DDR_PATTERN_LOOP_1 + 31:
					if (*(p + i) != ~TDATA325) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~TDATA325);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 0:
				case DDR_PATTERN_LOOP_2 + 1:
				case DDR_PATTERN_LOOP_2 + 2:
				case DDR_PATTERN_LOOP_2 + 3:
					if (*(p + i) != ~0xfe01fe01) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xfe01fe01);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 4:
				case DDR_PATTERN_LOOP_2 + 5:
				case DDR_PATTERN_LOOP_2 + 6:
				case DDR_PATTERN_LOOP_2 + 7:
					if (*(p + i) != ~0xfd02fd02) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xfd02fd02);
					}
					break;

				case DDR_PATTERN_LOOP_2 + 8:
				case DDR_PATTERN_LOOP_2 + 9:
				case DDR_PATTERN_LOOP_2 + 10:
				case DDR_PATTERN_LOOP_2 + 11:
					if (*(p + i) != ~0xfb04fb04) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xfb04fb04);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 12:
				case DDR_PATTERN_LOOP_2 + 13:
				case DDR_PATTERN_LOOP_2 + 14:
				case DDR_PATTERN_LOOP_2 + 15:
					if (*(p + i) != ~0xf708f708) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xf708f708);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 16:
				case DDR_PATTERN_LOOP_2 + 17:
				case DDR_PATTERN_LOOP_2 + 18:
				case DDR_PATTERN_LOOP_2 + 19:
					if (*(p + i) != ~0xef10ef10) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xef10ef10);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 20:
				case DDR_PATTERN_LOOP_2 + 21:
				case DDR_PATTERN_LOOP_2 + 22:
				case DDR_PATTERN_LOOP_2 + 23:
					if (*(p + i) != ~0xdf20df20) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xdf20df20);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 24:
				case DDR_PATTERN_LOOP_2 + 25:
				case DDR_PATTERN_LOOP_2 + 26:
				case DDR_PATTERN_LOOP_2 + 27:
					if (*(p + i) != ~0xbf40bf40) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xbf40bf40);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 28:
				case DDR_PATTERN_LOOP_2 + 29:
				case DDR_PATTERN_LOOP_2 + 30:
				case DDR_PATTERN_LOOP_2 + 31:
					if (*(p + i) != ~0x7f807f80) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0x7f807f80);
					}
					break;
					break;
				case DDR_PATTERN_LOOP_3 + 0:
				case DDR_PATTERN_LOOP_3 + 1:
				case DDR_PATTERN_LOOP_3 + 2:
				case DDR_PATTERN_LOOP_3 + 3:
					if (*(p + i) != ~0x00000100) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0x00000100);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 4:
				case DDR_PATTERN_LOOP_3 + 5:
				case DDR_PATTERN_LOOP_3 + 6:
				case DDR_PATTERN_LOOP_3 + 7:
					if (*(p + i) != ~0x00000200) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0x00000200);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 8:
				case DDR_PATTERN_LOOP_3 + 9:
				case DDR_PATTERN_LOOP_3 + 10:
				case DDR_PATTERN_LOOP_3 + 11:
					if (*(p + i) != ~0x00000400) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0x00000400);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 12:
				case DDR_PATTERN_LOOP_3 + 13:
				case DDR_PATTERN_LOOP_3 + 14:
				case DDR_PATTERN_LOOP_3 + 15:
					if (*(p + i) != ~0x00000800) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0x00000800);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 16:
				case DDR_PATTERN_LOOP_3 + 17:
				case DDR_PATTERN_LOOP_3 + 18:
				case DDR_PATTERN_LOOP_3 + 19:
					if (*(p + i) != ~0x00001000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0x00001000);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 20:
				case DDR_PATTERN_LOOP_3 + 21:
				case DDR_PATTERN_LOOP_3 + 22:
				case DDR_PATTERN_LOOP_3 + 23:
					if (*(p + i) != ~0x00002000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0x00002000);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 24:
				case DDR_PATTERN_LOOP_3 + 25:
				case DDR_PATTERN_LOOP_3 + 26:
				case DDR_PATTERN_LOOP_3 + 27:
					if (*(p + i) != ~0x00004000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0x00004000);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 28:
				case DDR_PATTERN_LOOP_3 + 29:
				case DDR_PATTERN_LOOP_3 + 30:
				case DDR_PATTERN_LOOP_3 + 31:
					if (*(p + i) != ~0x00008000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0x00008000);
					}
					break;
				}
			}

			if (m_len > 128 * 4) {
				m_len -= 128 * 4;
				p += 32 * 4;
			} else {
				p += (m_len >> 2);
				m_len = 0;
				break;
			}
		}
	}
}

static void ddr_read_p4_cross_talk_n2(void *buff, unsigned int m_length)
{
	unsigned int *p;
	unsigned int i, n;
	unsigned int m_len = m_length;

	p = (unsigned int *)buff;

	while (m_len) {
		{
			if (m_len >= 128 * 4)
				n = 32 * 4;
			else
				n = m_len >> 2;

			for (i = 0; i < n; i++) {
#ifdef DDR_PREFETCH_CACHE
				ddr_pld_cache(p);
#endif
				if ((error_outof_count_flag) && (error_count)) {
					printf("Error data out of count");
					m_len = 0;
					break;
				}

				switch (i) {
				case 0:
				case DDR_PATTERN_LOOP_1 + 1:
				case DDR_PATTERN_LOOP_2 + 2:
				case DDR_PATTERN_LOOP_3 + 3:
					if (*(p + i) != ~0xfe01fe01) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xfe01fe01);
						break;
					}
					break;
				case 4:
				case DDR_PATTERN_LOOP_1 + 5:
				case DDR_PATTERN_LOOP_2 + 6:
				case DDR_PATTERN_LOOP_3 + 7:
					if (*(p + i) != ~0xfd02fd02) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xfd02fd02);
						break;
					}
					break;

				case 8:
				case DDR_PATTERN_LOOP_1 + 9:
				case DDR_PATTERN_LOOP_2 + 10:
				case DDR_PATTERN_LOOP_3 + 11:
					if (*(p + i) != ~0xfb04fb04) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xfb04fb04);
						break;
					}
					break;

				case 12:
				case DDR_PATTERN_LOOP_1 + 13:
				case DDR_PATTERN_LOOP_2 + 14:
				case DDR_PATTERN_LOOP_3 + 15:
					if (*(p + i) != ~0xf708f708) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xf708f708);
						break;
					}
					break;

				case 16:
				case DDR_PATTERN_LOOP_1 + 17:
				case DDR_PATTERN_LOOP_2 + 18:
				case DDR_PATTERN_LOOP_3 + 19:
					if (*(p + i) != ~0xef10ef10) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xef10ef10);
						break;
					}
					break;

				case 20:
				case DDR_PATTERN_LOOP_1 + 21:
				case DDR_PATTERN_LOOP_2 + 22:
				case DDR_PATTERN_LOOP_3 + 23:
					if (*(p + i) != ~0xdf20df20) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xdf20df20);
						break;
					}
					break;

				case 24:
				case DDR_PATTERN_LOOP_1 + 25:
				case DDR_PATTERN_LOOP_2 + 26:
				case DDR_PATTERN_LOOP_3 + 27:
					if (*(p + i) != ~0xbf40bf40) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xbf40bf40);
						break;
					}
					break;
				case 28:
				case DDR_PATTERN_LOOP_1 + 29:
				case DDR_PATTERN_LOOP_2 + 30:
				case DDR_PATTERN_LOOP_3 + 31:
					if (*(p + i) != ~0x7f807f80) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0x7f807f80);
						break;
					}
					break;

				default:
					if (*(p + i) != ~0xff00ff00) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xff00ff00);
						break;
					}
					break;

					break;
				}
			}

			if (m_len > 128 * 4) {
				m_len -= 128 * 4;
				p += 32 * 4;
			} else {
				p += (m_len >> 2);
				m_len = 0;
				break;
			}
		}
	}
}

static void ddr_write_pattern4_no_cross_talk(void *buff, unsigned int m_length)
{
	unsigned int *p;
	unsigned int i, n;
	unsigned int m_len = m_length;

	p = (unsigned int *)buff;

	while (m_len) {
		{
			if (m_len >= 128 * 4)
				n = 32 * 4;
			else
				n = m_len >> 2;

			for (i = 0; i < n; i++) {
#ifdef DDR_PREFETCH_CACHE
				ddr_pld_cache(p);
#endif
				switch (i) {
				case 0:
				case 1:
				case 2:
				case 3:
					*(p + i) = 0xff00ff00;
					break;
				case 4:
				case 5:
				case 6:
				case 7:
					*(p + i) = 0xffff0000;
					break;

				case 8:
				case 9:
				case 10:
				case 11:
					*(p + i) = 0xff000000;
					break;
				case 12:
				case 13:
				case 14:
				case 15:
					*(p + i) = 0xff00ffff;
					break;

				case 16:
				case 17:
				case 18:
				case 19:
					*(p + i) = 0xff00ffff;
					break;
				case 20:
				case 21:
				case 22:
				case 23:
					*(p + i) = 0xff0000ff;
					break;
				case 24:
				case 25:
				case 26:
				case 27:
					*(p + i) = 0xffff0000;
					break;

				case 28:
				case 29:
				case 30:
				case 31:
					*(p + i) = 0x00ff00ff;
					break;
				case DDR_PATTERN_LOOP_1 + 0:
				case DDR_PATTERN_LOOP_1 + 1:
				case DDR_PATTERN_LOOP_1 + 2:
				case DDR_PATTERN_LOOP_1 + 3:
					*(p + i) = ~0xff00ff00;
					break;
				case DDR_PATTERN_LOOP_1 + 4:
				case DDR_PATTERN_LOOP_1 + 5:
				case DDR_PATTERN_LOOP_1 + 6:
				case DDR_PATTERN_LOOP_1 + 7:
					*(p + i) = ~0xffff0000;
					break;
				case DDR_PATTERN_LOOP_1 + 8:
				case DDR_PATTERN_LOOP_1 + 9:
				case DDR_PATTERN_LOOP_1 + 10:
				case DDR_PATTERN_LOOP_1 + 11:
					*(p + i) = ~0xff000000;
					break;
				case DDR_PATTERN_LOOP_1 + 12:
				case DDR_PATTERN_LOOP_1 + 13:
				case DDR_PATTERN_LOOP_1 + 14:
				case DDR_PATTERN_LOOP_1 + 15:
					*(p + i) = ~0xff00ffff;
					break;
				case DDR_PATTERN_LOOP_1 + 16:
				case DDR_PATTERN_LOOP_1 + 17:
				case DDR_PATTERN_LOOP_1 + 18:
				case DDR_PATTERN_LOOP_1 + 19:
					*(p + i) = ~0xff00ffff;
					break;
				case DDR_PATTERN_LOOP_1 + 20:
				case DDR_PATTERN_LOOP_1 + 21:
				case DDR_PATTERN_LOOP_1 + 22:
				case DDR_PATTERN_LOOP_1 + 23:
					*(p + i) = ~0xff00ffff;
					break;
				case DDR_PATTERN_LOOP_1 + 24:
				case DDR_PATTERN_LOOP_1 + 25:
				case DDR_PATTERN_LOOP_1 + 26:
				case DDR_PATTERN_LOOP_1 + 27:
					*(p + i) = ~0xffff0000;
					break;
				case DDR_PATTERN_LOOP_1 + 28:
				case DDR_PATTERN_LOOP_1 + 29:
				case DDR_PATTERN_LOOP_1 + 30:
				case DDR_PATTERN_LOOP_1 + 31:
					*(p + i) = ~0x00ff00ff;
					break;

				case DDR_PATTERN_LOOP_2 + 0:
				case DDR_PATTERN_LOOP_2 + 1:
				case DDR_PATTERN_LOOP_2 + 2:
				case DDR_PATTERN_LOOP_2 + 3:
					*(p + i) = 0x00ff0000;
					break;
				case DDR_PATTERN_LOOP_2 + 4:
				case DDR_PATTERN_LOOP_2 + 5:
				case DDR_PATTERN_LOOP_2 + 6:
				case DDR_PATTERN_LOOP_2 + 7:
					*(p + i) = 0xff000000;
					break;
				case DDR_PATTERN_LOOP_2 + 8:
				case DDR_PATTERN_LOOP_2 + 9:
				case DDR_PATTERN_LOOP_2 + 10:
				case DDR_PATTERN_LOOP_2 + 11:
					*(p + i) = 0x0000ffff;
					break;
				case DDR_PATTERN_LOOP_2 + 12:
				case DDR_PATTERN_LOOP_2 + 13:
				case DDR_PATTERN_LOOP_2 + 14:
				case DDR_PATTERN_LOOP_2 + 15:
					*(p + i) = 0x000000ff;
					break;
				case DDR_PATTERN_LOOP_2 + 16:
				case DDR_PATTERN_LOOP_2 + 17:
				case DDR_PATTERN_LOOP_2 + 18:
				case DDR_PATTERN_LOOP_2 + 19:
					*(p + i) = 0x00ff00ff;
					break;
				case DDR_PATTERN_LOOP_2 + 20:
				case DDR_PATTERN_LOOP_2 + 21:
				case DDR_PATTERN_LOOP_2 + 22:
				case DDR_PATTERN_LOOP_2 + 23:
					*(p + i) = 0xff00ff00;
					break;
				case DDR_PATTERN_LOOP_2 + 24:
				case DDR_PATTERN_LOOP_2 + 25:
				case DDR_PATTERN_LOOP_2 + 26:
				case DDR_PATTERN_LOOP_2 + 27:
					*(p + i) = 0xff00ffff;
					break;
				case DDR_PATTERN_LOOP_2 + 28:
				case DDR_PATTERN_LOOP_2 + 29:
				case DDR_PATTERN_LOOP_2 + 30:
				case DDR_PATTERN_LOOP_2 + 31:
					*(p + i) = 0xff00ff00;
					break;
				case DDR_PATTERN_LOOP_3 + 0:
				case DDR_PATTERN_LOOP_3 + 1:
				case DDR_PATTERN_LOOP_3 + 2:
				case DDR_PATTERN_LOOP_3 + 3:
					*(p + i) = ~0x00ff0000;
					break;
				case DDR_PATTERN_LOOP_3 + 4:
				case DDR_PATTERN_LOOP_3 + 5:
				case DDR_PATTERN_LOOP_3 + 6:
				case DDR_PATTERN_LOOP_3 + 7:
					*(p + i) = ~0xff000000;
					break;
				case DDR_PATTERN_LOOP_3 + 8:
				case DDR_PATTERN_LOOP_3 + 9:
				case DDR_PATTERN_LOOP_3 + 10:
				case DDR_PATTERN_LOOP_3 + 11:
					*(p + i) = ~0x0000ffff;
					break;
				case DDR_PATTERN_LOOP_3 + 12:
				case DDR_PATTERN_LOOP_3 + 13:
				case DDR_PATTERN_LOOP_3 + 14:
				case DDR_PATTERN_LOOP_3 + 15:
					*(p + i) = ~0x000000ff;
					break;
				case DDR_PATTERN_LOOP_3 + 16:
				case DDR_PATTERN_LOOP_3 + 17:
				case DDR_PATTERN_LOOP_3 + 18:
				case DDR_PATTERN_LOOP_3 + 19:
					*(p + i) = ~0x00ff00ff;
					break;
				case DDR_PATTERN_LOOP_3 + 20:
				case DDR_PATTERN_LOOP_3 + 21:
				case DDR_PATTERN_LOOP_3 + 22:
				case DDR_PATTERN_LOOP_3 + 23:
					*(p + i) = ~0xff00ff00;
					break;
				case DDR_PATTERN_LOOP_3 + 24:
				case DDR_PATTERN_LOOP_3 + 25:
				case DDR_PATTERN_LOOP_3 + 26:
				case DDR_PATTERN_LOOP_3 + 27:
					*(p + i) = ~0xff00ffff;
					break;
				case DDR_PATTERN_LOOP_3 + 28:
				case DDR_PATTERN_LOOP_3 + 29:
				case DDR_PATTERN_LOOP_3 + 30:
				case DDR_PATTERN_LOOP_3 + 31:
					*(p + i) = ~0xff00ff00;
					break;
				}
			}

			if (m_len > (128 * 4)) {
				m_len -= (128 * 4);
				p += 32 * 4;
			} else {
				p += (m_len >> 2);
				m_len = 0;
				break;
			}
		}
	}
}

static void ddr_read_pattern4_no_cross_talk(void *buff, unsigned int m_length)
{
	unsigned int *p;
	unsigned int i, n;
	unsigned int m_len = m_length;

	p = (unsigned int *)buff;
	while (m_len) {
		{
			if (m_len >= 128 * 4)
				n = 32 * 4;
			else
				n = m_len >> 2;

			for (i = 0; i < n; i++) {
#ifdef DDR_PREFETCH_CACHE
				ddr_pld_cache(p);
#endif
				if ((error_outof_count_flag) && (error_count)) {
					printf("Error data out of count");
					m_len = 0;
					break;
				}
				switch (i) {
				case 0:
				case 1:
				case 2:
				case 3:
					if (*(p + i) != 0xff00ff00) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xff00ff00);
					}
					break;
				case 4:
				case 5:
				case 6:
				case 7:
					if (*(p + i) != 0xffff0000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xffff0000);
					}
					break;

				case 8:
				case 9:
				case 10:
				case 11:
					if (*(p + i) != 0xff000000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xff000000);
					}
					break;
				case 12:
				case 13:
				case 14:
				case 15:
					if (*(p + i) != 0xff00ffff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xff00ffff);
					}
					break;

				case 16:
				case 17:
				case 18:
				case 19:
					if (*(p + i) != 0xff00ffff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xff00ffff);
					}
					break;
				case 20:
				case 21:
				case 22:
				case 23:
					if (*(p + i) != 0xff0000ff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xff0000ff);
					}
					break;
				case 24:
				case 25:
				case 26:
				case 27:
					if (*(p + i) != 0xffff0000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xffff0000);
					}
					break;

				case 28:
				case 29:
				case 30:
				case 31:
					if (*(p + i) != 0x00ff00ff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0x00ff00ff);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 0:
				case DDR_PATTERN_LOOP_1 + 1:
				case DDR_PATTERN_LOOP_1 + 2:
				case DDR_PATTERN_LOOP_1 + 3:
					if (*(p + i) != ~0xff00ff00) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xff00ff00);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 4:
				case DDR_PATTERN_LOOP_1 + 5:
				case DDR_PATTERN_LOOP_1 + 6:
				case DDR_PATTERN_LOOP_1 + 7:
					if (*(p + i) != ~0xffff0000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xffff0000);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 8:
				case DDR_PATTERN_LOOP_1 + 9:
				case DDR_PATTERN_LOOP_1 + 10:
				case DDR_PATTERN_LOOP_1 + 11:
					if (*(p + i) != ~0xff000000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xff000000);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 12:
				case DDR_PATTERN_LOOP_1 + 13:
				case DDR_PATTERN_LOOP_1 + 14:
				case DDR_PATTERN_LOOP_1 + 15:
					if (*(p + i) != ~0xff00ffff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xff00ffff);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 16:
				case DDR_PATTERN_LOOP_1 + 17:
				case DDR_PATTERN_LOOP_1 + 18:
				case DDR_PATTERN_LOOP_1 + 19:
					if (*(p + i) != ~0xff00ffff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xff00ffff);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 20:
				case DDR_PATTERN_LOOP_1 + 21:
				case DDR_PATTERN_LOOP_1 + 22:
				case DDR_PATTERN_LOOP_1 + 23:
					if (*(p + i) != ~0xff00ffff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xff00ffff);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 24:
				case DDR_PATTERN_LOOP_1 + 25:
				case DDR_PATTERN_LOOP_1 + 26:
				case DDR_PATTERN_LOOP_1 + 27:
					if (*(p + i) != ~0xffff0000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xffff0000);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 28:
				case DDR_PATTERN_LOOP_1 + 29:
				case DDR_PATTERN_LOOP_1 + 30:
				case DDR_PATTERN_LOOP_1 + 31:
					if (*(p + i) != ~0x00ff00ff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0x00ff00ff);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 0:
				case DDR_PATTERN_LOOP_2 + 1:
				case DDR_PATTERN_LOOP_2 + 2:
				case DDR_PATTERN_LOOP_2 + 3:
					if (*(p + i) != 0x00ff0000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0x00ff0000);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 4:
				case DDR_PATTERN_LOOP_2 + 5:
				case DDR_PATTERN_LOOP_2 + 6:
				case DDR_PATTERN_LOOP_2 + 7:
					if (*(p + i) != 0xff000000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xff000000);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 8:
				case DDR_PATTERN_LOOP_2 + 9:
				case DDR_PATTERN_LOOP_2 + 10:
				case DDR_PATTERN_LOOP_2 + 11:
					if (*(p + i) != 0x0000ffff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0x0000ffff);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 12:
				case DDR_PATTERN_LOOP_2 + 13:
				case DDR_PATTERN_LOOP_2 + 14:
				case DDR_PATTERN_LOOP_2 + 15:
					if (*(p + i) != 0x000000ff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0x000000ff);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 16:
				case DDR_PATTERN_LOOP_2 + 17:
				case DDR_PATTERN_LOOP_2 + 18:
				case DDR_PATTERN_LOOP_2 + 19:
					if (*(p + i) != 0x00ff00ff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0x00ff00ff);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 20:
				case DDR_PATTERN_LOOP_2 + 21:
				case DDR_PATTERN_LOOP_2 + 22:
				case DDR_PATTERN_LOOP_2 + 23:
					if (*(p + i) != 0xff00ff00) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xff00ff00);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 24:
				case DDR_PATTERN_LOOP_2 + 25:
				case DDR_PATTERN_LOOP_2 + 26:
				case DDR_PATTERN_LOOP_2 + 27:
					if (*(p + i) != 0xff00ffff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xff00ffff);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 28:
				case DDR_PATTERN_LOOP_2 + 29:
				case DDR_PATTERN_LOOP_2 + 30:
				case DDR_PATTERN_LOOP_2 + 31:
					if (*(p + i) != 0xff00ff00) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), 0xff00ff00);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 0:
				case DDR_PATTERN_LOOP_3 + 1:
				case DDR_PATTERN_LOOP_3 + 2:
				case DDR_PATTERN_LOOP_3 + 3:
					if (*(p + i) != ~0x00ff0000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0x00ff0000);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 4:
				case DDR_PATTERN_LOOP_3 + 5:
				case DDR_PATTERN_LOOP_3 + 6:
				case DDR_PATTERN_LOOP_3 + 7:
					if (*(p + i) != ~0xff000000) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xff000000);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 8:
				case DDR_PATTERN_LOOP_3 + 9:
				case DDR_PATTERN_LOOP_3 + 10:
				case DDR_PATTERN_LOOP_3 + 11:
					if (*(p + i) != ~0x0000ffff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0x0000ffff);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 12:
				case DDR_PATTERN_LOOP_3 + 13:
				case DDR_PATTERN_LOOP_3 + 14:
				case DDR_PATTERN_LOOP_3 + 15:
					if (*(p + i) != ~0x000000ff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0x000000ff);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 16:
				case DDR_PATTERN_LOOP_3 + 17:
				case DDR_PATTERN_LOOP_3 + 18:
				case DDR_PATTERN_LOOP_3 + 19:
					if (*(p + i) != ~0x00ff00ff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0x00ff00ff);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 20:
				case DDR_PATTERN_LOOP_3 + 21:
				case DDR_PATTERN_LOOP_3 + 22:
				case DDR_PATTERN_LOOP_3 + 23:
					if (*(p + i) != ~0xff00ff00) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xff00ff00);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 24:
				case DDR_PATTERN_LOOP_3 + 25:
				case DDR_PATTERN_LOOP_3 + 26:
				case DDR_PATTERN_LOOP_3 + 27:
					if (*(p + i) != ~0xff00ffff) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xff00ffff);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 28:
				case DDR_PATTERN_LOOP_3 + 29:
				case DDR_PATTERN_LOOP_3 + 30:
				case DDR_PATTERN_LOOP_3 + 31:
					if (*(p + i) != ~0xff00ff00) {
						error_count++;
						printf("Error data [0x%8x] add 0x%8x[0x%8x]\n",
						       *(p + i), p_convert_int(p + i), ~0xff00ff00);
					}
					break;
				}
			}

			if (m_len > (128 * 4)) {
				m_len -= (128 * 4);
				p += 32 * 4;
			} else {
				p += (m_len >> 2);
				m_len = 0;
				break;
			}
		}
	}
}

int do_ddr_test(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char *endp;
	unsigned int loop = 1;
	unsigned int lflag = 0;
	unsigned int start_addr = DDR_TEST_START_ADDR;
	unsigned int test_size = DDR_TEST_SIZE;
	unsigned int simple_pattern_flag = 1;
	unsigned int cross_talk_pattern_flag = 1;
	unsigned int old_pattern_flag = 1;
	unsigned int print_flag = 1;

	print_flag = 1;
	error_outof_count_flag = 0;
	error_count = 0;
	printf("\nargc== 0x%8x\n", argc);
	int i;

	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);
	if (!argc)
		goto DDR_TEST_START;
	if (argc > 1) {
		if (strcmp(argv[1], "l") == 0) {
			lflag = 1;
		} else if (strcmp(argv[1], "h") == 0) {
			goto usage;
		} else {
			loop = simple_strtoull_ddr(argv[1], &endp, 10);
			if (*argv[1] == 0 || *endp != 0)
				loop = 1;
		}
	}
	if (argc == 1) {
		start_addr = DDR_TEST_START_ADDR;
		loop = 1;
	}
	if (argc > 2) {
		start_addr = simple_strtoull_ddr(argv[2], &endp, 16);
		if (*argv[2] == 0 || *endp != 0)
			start_addr = DDR_TEST_START_ADDR;
	}
	if (argc > 3) {
		test_size = simple_strtoull_ddr(argv[3], &endp, 16);
		if (*argv[3] == 0 || *endp != 0)
			test_size = DDR_TEST_SIZE;
	}
	if (test_size < 0x1000)
		test_size = DDR_TEST_SIZE;

	old_pattern_flag = 1;
	simple_pattern_flag = 1;
	cross_talk_pattern_flag = 1;
	if (argc == 2) {
		if ((strcmp(argv[1], "s") == 0)) {
			simple_pattern_flag = 1;
			old_pattern_flag = 0;
			cross_talk_pattern_flag = 0;
		} else if ((strcmp(argv[1], "c") == 0)) {
			simple_pattern_flag = 0;
			old_pattern_flag = 0;
			cross_talk_pattern_flag = 1;
		} else if ((strcmp(argv[1], "e") == 0)) {
			error_outof_count_flag = 1;
		}
	}
	if (argc > 2) {
		if ((strcmp(argv[1], "n") == 0) || (strcmp(argv[2], "n") == 0))
			print_flag = 0;
		if ((strcmp(argv[1], "p") == 0) || (strcmp(argv[2], "p") == 0))
			copy_test_flag = 1;
		if ((strcmp(argv[1], "s") == 0) || (strcmp(argv[2], "s") == 0)) {
			simple_pattern_flag = 1;
			old_pattern_flag = 0;
			cross_talk_pattern_flag = 0;
		} else if ((strcmp(argv[1], "c") == 0) || (strcmp(argv[2], "c") == 0)) {
			simple_pattern_flag = 0;
			old_pattern_flag = 0;
			cross_talk_pattern_flag = 1;
		} else if ((strcmp(argv[1], "e") == 0) || (strcmp(argv[2], "e") == 0)) {
			error_outof_count_flag = 1;
		}
	}
	if (argc > 3) {
		if ((strcmp(argv[1], "p") == 0) || (strcmp(argv[2], "p") == 0) ||
		    (strcmp(argv[3], "p") == 0))
			copy_test_flag = 1;
		if ((strcmp(argv[1], "n") == 0) || (strcmp(argv[2], "n") == 0) ||
		    (strcmp(argv[3], "n") == 0))
			print_flag = 0;
		if ((strcmp(argv[1], "s") == 0) || (strcmp(argv[2], "s") == 0) ||
		    (strcmp(argv[3], "s") == 0)) {
			simple_pattern_flag = 1;
			old_pattern_flag = 0;
			cross_talk_pattern_flag = 0;
		}
		if ((strcmp(argv[1], "c") == 0) || (strcmp(argv[2], "c") == 0) ||
		    (strcmp(argv[3], "c") == 0)) {
			simple_pattern_flag = 0;
			old_pattern_flag = 0;
			cross_talk_pattern_flag = 1;
		}
		if ((strcmp(argv[1], "e") == 0) || (strcmp(argv[2], "e") == 0) ||
		    (strcmp(argv[3], "e") == 0))
			error_outof_count_flag = 1;
	}

DDR_TEST_START:

	do {
		if (lflag)
			loop = 888;

		if (old_pattern_flag == 1) {
			{
				if (print_flag)
					printf("\nStart writing at 0x%8x - 0x%8x...",
					       start_addr, start_addr + test_size);
				ddr_write((void *)(int_convert_p(start_addr)), test_size);
				if (print_flag) {
					printf("\nEnd write.                                 ");
					printf("\nStart 1st reading...                       ");
				}
				ddr_read((void *)(int_convert_p(start_addr)), test_size);
				if (print_flag) {
					printf("\nEnd 1st read.                              ");
					printf("\nStart 2nd reading...                       ");
				}
				ddr_read((void *)(int_convert_p(start_addr)), test_size);
				if (print_flag) {
					printf("\nEnd 2nd read.                              ");
					printf("\nStart 3rd reading...                       ");
				}
				ddr_read((void *)(int_convert_p(start_addr)), test_size);
				if (print_flag)
					printf("\nEnd 3rd read.\n");

				if (copy_test_flag) {
					if (print_flag)
						printf("\n copy_test_flag = 1,start copy test.\n");
					ddr_test_copy((void *)(int_convert_p(start_addr +
									     test_size / 2)),
						      (void *)(int_convert_p(start_addr)),
						      test_size / 2);
					ddr_read((void *)(int_convert_p(start_addr +
									test_size / 2)),
						 test_size / 2);
					ddr_read((void *)(int_convert_p(start_addr +
									test_size / 2)),
						 test_size / 2);
				}
			}
			{
				if (print_flag) {
					printf("\nStart x4 normal pattern.                  ");
					printf("\nStart writing at 0x%8x - 0x%8x...", start_addr,
					       start_addr + test_size);
				}
				ddr_write4((void *)(int_convert_p(start_addr)), test_size);
				if (print_flag) {
					printf("\nEnd write.                                 ");
					printf("\nStart 1st reading...                       ");
				}
				ddr_read4((void *)(int_convert_p(start_addr)), test_size);
				if (print_flag) {
					printf("\nEnd 1st read.                              ");
					printf("\nStart 2nd reading...                       ");
				}
				ddr_read4((void *)(int_convert_p(start_addr)), test_size);
				if (print_flag) {
					printf("\nEnd 2nd read.                              ");
					printf("\nStart 3rd reading...                       ");
				}
				ddr_read4((void *)(int_convert_p(start_addr)), test_size);
				if (print_flag)
					printf("\rEnd 3rd read.\n");
				if (copy_test_flag) {
					ddr_test_copy((void *)(int_convert_p(start_addr +
									     test_size / 2)),
						      (void *)(int_convert_p(start_addr)),
						      test_size / 2);
					ddr_read4((void *)(int_convert_p(start_addr +
									 test_size / 2)),
						  test_size / 2);
					ddr_read4((void *)(int_convert_p(start_addr +
									 test_size / 2)),
						  test_size / 2);
				}
			}
		}

		if (simple_pattern_flag == 1) {
			if (print_flag) {
				printf("\nStart x4 no cross talk pattern.     ");
				printf("\nStart writing at 0x%8x - 0x%8x...", start_addr,
				       start_addr + test_size);
			}
			ddr_write_pattern4_no_cross_talk((void *)(int_convert_p(start_addr)),
							 test_size);
			if (print_flag) {
				printf("\rEnd write.                                 ");
				printf("\rStart 1st reading...                       ");
			}
			ddr_read_pattern4_no_cross_talk((void *)(int_convert_p(start_addr)),
							test_size);
			if (print_flag) {
				printf("\rEnd 1st read.                              ");
				printf("\rStart 2nd reading...                       ");
			}
			ddr_read_pattern4_no_cross_talk((void *)(int_convert_p(start_addr)),
							test_size);
			if (print_flag) {
				printf("\rEnd 2nd read.                              ");
				printf("\rStart 3rd reading...                       ");
			}
			ddr_read_pattern4_no_cross_talk((void *)(int_convert_p(start_addr)),
							test_size);
			if (print_flag)
				printf("\rEnd 3rd read.\n");

			if (copy_test_flag) {
				ddr_test_copy((void *)(int_convert_p(start_addr + test_size / 2)),
					      (void *)(int_convert_p(start_addr)), test_size / 2);
				ddr_read_pattern4_no_cross_talk((void *)(int_convert_p(start_addr +
										       test_size /
										       2)),
								test_size / 2);
				ddr_read_pattern4_no_cross_talk((void *)(int_convert_p(start_addr +
										       test_size /
										       2)),
								test_size / 2);
			}
		}

		if (cross_talk_pattern_flag == 1) {
			if (print_flag) {
				printf("\nStart x4  cross talk pattern p.                ");
				printf("\nStart writing at 0x%8x - 0x%8x...", start_addr,
				       start_addr + test_size);
			}
			ddr_write_pattern4_cross_talk_p((void *)(int_convert_p(start_addr)),
							test_size);
			if (print_flag) {
				printf("\rEnd write.                                 ");
				printf("\rStart 1st reading...                       ");
			}
			ddr_read_pattern4_cross_talk_p((void *)(int_convert_p(start_addr)),
						       test_size);
			if (print_flag) {
				printf("\rEnd 1st read.                              ");
				printf("\rStart 2nd reading...                       ");
			}
			ddr_read_pattern4_cross_talk_p((void *)(int_convert_p(start_addr)),
						       test_size);
			if (print_flag) {
				printf("\rEnd 2nd read.                              ");
				printf("\rStart 3rd reading...                       ");
			}
			ddr_read_pattern4_cross_talk_p((void *)(int_convert_p(start_addr)),
						       test_size);
			if (print_flag) {
				printf("\rEnd 3rd read.\n");

				printf("\nStart x4  cross talk pattern n.                   ");
				printf("\nStart writing at 0x%8x - 0x%8x...", start_addr,
				       start_addr + test_size);
			}
			ddr_write_pattern4_cross_talk_n((void *)(int_convert_p(start_addr)),
							test_size);
			if (print_flag) {
				printf("\rEnd write.                                 ");
				printf("\rStart 1st reading...                       ");
			}
			ddr_read_pattern4_cross_talk_n((void *)(int_convert_p(start_addr)),
						       test_size);
			if (print_flag) {
				printf("\rEnd 1st read.                              ");
				printf("\rStart 2nd reading...                       ");
			}
			ddr_read_pattern4_cross_talk_n((void *)(int_convert_p(start_addr)),
						       test_size);
			if (print_flag) {
				printf("\rEnd 2nd read.                              ");
				printf("\rStart 3rd reading...                       ");
			}
			ddr_read_pattern4_cross_talk_n((void *)(int_convert_p(start_addr)),
						       test_size);
			if (print_flag) {
				printf("\rEnd 3rd read.\n");

				printf("\nStart x4  cross talk pattern p2.                   ");
				printf("\nStart writing at 0x%8x - 0x%8x...", start_addr,
				       start_addr + test_size);
			}
			ddr_write_pattern4_cross_talk_p2((void *)(int_convert_p(start_addr)),
							 test_size);
			if (print_flag) {
				printf("\rEnd write.                                 ");
				printf("\rStart 1st reading...                       ");
			}
			ddr_read_pattern4_cross_talk_p2((void *)(int_convert_p(start_addr)),
							test_size);
			if (print_flag) {
				printf("\rEnd 1st read.                              ");
				printf("\rStart 2nd reading...                       ");
			}
			ddr_read_pattern4_cross_talk_p2((void *)(int_convert_p(start_addr)),
							test_size);
			if (print_flag) {
				printf("\rEnd 2nd read.                              ");
				printf("\rStart 3rd reading...                       ");
			}
			ddr_read_pattern4_cross_talk_p2((void *)(int_convert_p(start_addr)),
							test_size);
			if (print_flag) {
				printf("\rEnd 3rd read.\n");

				printf("\nStart x4  cross talk pattern n2.              ");
				printf("\nStart writing at 0x%8x - 0x%8x...", start_addr,
				       start_addr + test_size);
			}
			ddr_write_pattern4_cross_talk_n2((void *)(int_convert_p(start_addr)),
							 test_size);
			if (print_flag) {
				printf("\rEnd write.                                 ");
				printf("\rStart 1st reading...                       ");
			}
			ddr_read_p4_cross_talk_n2((void *)(int_convert_p(start_addr)),
						  test_size);
			if (print_flag) {
				printf("\rEnd 1st read.                              ");
				printf("\rStart 2nd reading...                       ");
			}
			ddr_read_p4_cross_talk_n2((void *)(int_convert_p(start_addr)),
						  test_size);
			if (print_flag) {
				printf("\rEnd 2nd read.                              ");
				printf("\rStart 3rd reading...                       ");
			}
			ddr_read_p4_cross_talk_n2((void *)(int_convert_p(start_addr)),
						  test_size);
			if (print_flag)
				printf("\rEnd 3rd read.\n");

			if (copy_test_flag) {
				ddr_test_copy((void *)(int_convert_p(start_addr + test_size / 2)),
					      (void *)(int_convert_p(start_addr)), test_size / 2);
				ddr_read_p4_cross_talk_n2((void *)(int_convert_p(start_addr +
										 test_size / 2)),
							  test_size / 2);
				ddr_read_p4_cross_talk_n2((void *)(int_convert_p(start_addr +
										 test_size / 2)),
							  test_size / 2);
			}
		}

		if (print_flag)
			printf("\nError count=0x%8x", error_count);
	} while (--loop);

	printf("\rEnd ddr test.\n");

	return 0;

usage:
	cmd_usage(cmdtp);
	return 1;
}

U_BOOT_CMD(ddrtest, 5, 1, do_ddr_test,
	   "DDR test function",
	   "ddrtest [LOOP] [ADDR].Default address is 0x8d000000\n");

int do_ddr_special_test(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char *endp;
	unsigned int loop = 1;
	unsigned int lflag = 0;
	unsigned int start_addr = DDR_TEST_START_ADDR;
	unsigned int test_addr = DDR_TEST_START_ADDR;
	unsigned int test_size = DDR_TEST_SIZE;
	unsigned int write_times = 1;
	unsigned int read_times = 3;

	unsigned int print_flag = 1;

	print_flag = 1;
	error_outof_count_flag = 0;
	error_count = 0;
	printf("\nargc== 0x%8x\n", argc);
	int i;

	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);

	if (strcmp(argv[1], "l") == 0) {
		lflag = 1;
	} else if (strcmp(argv[1], "h") == 0) {
		//goto usage;
	} else {
		loop = simple_strtoull_ddr(argv[1], &endp, 10);
		if (*argv[1] == 0 || *endp != 0)
			loop = 1;
	}

	if (argc == 1) {
		start_addr = DDR_TEST_START_ADDR;
		loop = 1;
	}
	if (argc > 2) {
		start_addr = simple_strtoull_ddr(argv[2], &endp, 16);
		if (*argv[2] == 0 || *endp != 0)
			start_addr = DDR_TEST_START_ADDR;
	}
	if (argc > 3) {
		test_size = simple_strtoull_ddr(argv[3], &endp, 16);
		if (*argv[3] == 0 || *endp != 0)
			test_size = DDR_TEST_SIZE;
	}
	if (test_size < 0x1000)
		test_size = DDR_TEST_SIZE;
	if (argc > 4) {
		write_times = simple_strtoull_ddr(argv[4], &endp, 16);
		if (*argv[4] == 0 || *endp != 0)
			write_times = 0;
	}
	if (argc > 5) {
		read_times = simple_strtoull_ddr(argv[5], &endp, 16);
		if (*argv[5] == 0 || *endp != 0)
			read_times = 0;
	}
	unsigned int base_pattern = 1;
	unsigned int inc_flag = 1;

	if (argc > 6) {
		base_pattern = simple_strtoull_ddr(argv[6], &endp, 16);
		if (*argv[6] == 0 || *endp != 0)
			base_pattern = 0;
	}
	if (argc > 7) {
		inc_flag = simple_strtoull_ddr(argv[7], &endp, 16);
		if (*argv[7] == 0 || *endp != 0)
			inc_flag = 0;
	}

	unsigned int count = 1;
	unsigned int test_val = 1;

	do {
		if (lflag)
			loop = 888;

		if (1) {
			for (i = 0; i < write_times; ) {
				i++;
				printf("\nwrite_times=0x%8x\n", ((unsigned int)i));
				test_addr = start_addr;
				test_val = base_pattern;
				count = (test_size >> 2);
				do {
					writel(test_val, (unsigned long)test_addr);
					test_addr = test_addr + 4;
					if (inc_flag)
						test_val = test_val + 1;
				} while (count--);
			}

			for (i = 0; i < read_times; ) {
				i++;
				printf("\nread_times=0x%8x\n", ((unsigned int)i));
				test_addr = start_addr;
				test_val = base_pattern;
				count = (test_size >> 2);

				do {
					if (test_val != (readl((unsigned long)test_addr)))
						printf("\nadd=0x%8x,pattern=0x%8x,read=0x%8x\n",
						       ((unsigned int)test_addr),
						       ((unsigned int)test_val),
						       (readl((unsigned int)test_addr)));
					test_addr = test_addr + 4;
					if (inc_flag)
						test_val = test_val + 1;
				} while (count--);
			}
		}

		if (print_flag)
			printf("\nError count=0x%8x", error_count);
	} while (--loop);

	printf("\rEnd ddr test.\n");

	return 0;

//usage:
	//cmd_usage(cmdtp);
	//return 1;
}

U_BOOT_CMD(ddr_spec_test, 8, 1, do_ddr_special_test,
	   "DDR test function ddr_spec_test 1 0x1080000 0x200000 1  3 1 1",
	   "ddrtest [LOOP] [ADDR] [size] [write_times] [read times] [pattern] [inc].\n");

int ddr_test_s_cross_talk_pattern(int ddr_test_size)
{
#define TEST_OFFSET  0 //0X40000000
	unsigned int start_addr = test_start_addr;

	error_outof_count_flag = 1;

	error_count = 0;

#if (CONFIG_DDR_PHY == P_DDR_PHY_905X)
	training_pattern_flag = 0;
#endif
	if (training_pattern_flag) {
#if (CONFIG_DDR_PHY == P_DDR_PHY_GX_BABY)
		ddr_test_gx_training_pattern(ddr_test_size);
#endif
		if (error_count)
			return 1;
		else
			return 0;
	} else {
#if (CONFIG_DDR_PHY == P_DDR_PHY_GX_BABY)
		ddr_test_gx_training_pattern(ddr_test_size);
#endif
	}

	{
		printf("\nStart writing at 0x%8x - 0x%8x...", start_addr, start_addr +
		       ddr_test_size);
		ddr_write((void *)(int_convert_p(start_addr)), ddr_test_size);
		printf("\nEnd write.                                 ");
		printf("\nStart 1st reading...                       ");
		ddr_read((void *)(int_convert_p(start_addr)), ddr_test_size);
		printf("\nEnd 1st read.                              ");
		printf("\nStart 2nd reading...                       ");
		ddr_read((void *)(int_convert_p(start_addr)), ddr_test_size);
		if (error_count)
			return error_count;
		printf("\nStart writing pattern4 at 0x%8x - 0x%8x...", start_addr, start_addr +
		       ddr_test_size);
		ddr_write4((void *)(int_convert_p(start_addr)), ddr_test_size);
		printf("\nEnd write.                                 ");
		printf("\nStart 1st reading...                       ");
		ddr_read4((void *)(int_convert_p(start_addr)), ddr_test_size);
		printf("\nEnd 1st read.                              ");
		printf("\nStart 2nd reading...                       ");
		ddr_read4((void *)(int_convert_p(start_addr)), ddr_test_size);

		if (error_count)
			return error_count;
		printf("\nStart x4 no cross talk pattern.                                 ");
		printf("\nStart writing at 0x%8x - 0x%8x...", start_addr, start_addr +
		       ddr_test_size);
		ddr_write_pattern4_no_cross_talk((void *)(int_convert_p(start_addr)),
						 ddr_test_size);
		printf("\nEnd write.                                 ");
		printf("\nStart 1st reading...                       ");
		ddr_read_pattern4_no_cross_talk((void *)(int_convert_p(start_addr)),
						ddr_test_size);
		printf("\nEnd 1st read.                              ");
		printf("\nStart 2nd reading...                       ");
		ddr_read_pattern4_no_cross_talk((void *)(int_convert_p(start_addr)),
						ddr_test_size);
	}

	if (error_count)
		return error_count;
	{
		printf("\nStart x4  cross talk pattern p.                                 ");
		printf("\nStart writing at 0x%8x - 0x%8x...", start_addr, start_addr +
		       ddr_test_size);
		ddr_write_pattern4_cross_talk_p((void *)(int_convert_p(start_addr)),
						ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_pattern4_cross_talk_p((void *)(int_convert_p(start_addr)),
					       ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_pattern4_cross_talk_p((void *)(int_convert_p(start_addr)),
					       ddr_test_size);
		printf("\rEnd 2nd read.                              ");

		if (error_count)
			return error_count;
		printf("\nStart x4  cross talk pattern n.                                 ");
		printf("\nStart writing at 0x%8x - 0x%8x...", start_addr, start_addr +
		       ddr_test_size);
		ddr_write_pattern4_cross_talk_n((void *)(int_convert_p(start_addr)),
						ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_pattern4_cross_talk_n((void *)(int_convert_p(start_addr)), ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_pattern4_cross_talk_n((void *)(int_convert_p(start_addr)), ddr_test_size);
		printf("\rEnd 2nd read.                              ");
	}
	if (error_count)
		return error_count;
	{
		printf("\nStart x4  cross talk pattern p2.                                 ");
		printf("\nStart writing at 0x%8x - 0x%8x...", start_addr, start_addr +
		       ddr_test_size);
		ddr_write_pattern4_cross_talk_p2((void *)(int_convert_p(start_addr)),
						 ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_pattern4_cross_talk_p2((void *)(int_convert_p(start_addr)),
						ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_pattern4_cross_talk_p2((void *)(int_convert_p(start_addr)),
						ddr_test_size);
		printf("\rEnd 2nd read.                              ");

		if (error_count)
			return error_count;
		printf("\nStart x4  cross talk pattern n.                                 ");
		printf("\nStart writing at 0x%8x - 0x%8x...", start_addr, start_addr +
		       ddr_test_size);
		ddr_write_pattern4_cross_talk_n2((void *)(int_convert_p(start_addr)),
						 ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_p4_cross_talk_n2((void *)(int_convert_p(start_addr)),
					  ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_p4_cross_talk_n2((void *)(int_convert_p(start_addr)),
					  ddr_test_size);
		printf("\rEnd 2nd read.                              ");

		if (copy_test_flag) {
			if (error_count)
				return error_count;
			printf("\n start copy test  ...                            ");
			ddr_test_copy((void *)(int_convert_p(start_addr + ddr_test_size / 2)),
				      (void *)(int_convert_p(start_addr)), ddr_test_size / 2);
			ddr_read_p4_cross_talk_n2((void *)(int_convert_p(start_addr +
									 ddr_test_size / 2)),
						  ddr_test_size / 2);
			ddr_read_p4_cross_talk_n2((void *)(int_convert_p(start_addr +
									 ddr_test_size / 2)),
						  ddr_test_size / 2);
		}
	}

	if (error_count)
		return 1;
	else
		return 0;
}

int ddr_test_s_cross_talk_pattern_quick_return(int ddr_test_size)
{
	error_outof_count_flag = 1;
#define TEST_OFFSET  0 //0X40000000
	unsigned int start_addr = test_start_addr;

	error_outof_count_flag = 1;

	error_count = 0;

#if (CONFIG_DDR_PHY == P_DDR_PHY_905X)
	training_pattern_flag = 0;
#endif
	if (training_pattern_flag) {
#if (CONFIG_DDR_PHY == P_DDR_PHY_GX_BABY)
		ddr_test_gx_training_pattern(ddr_test_size);
#endif
		if (error_count)
			return 1;
		else
			return 0;
	} else {
#if (CONFIG_DDR_PHY == P_DDR_PHY_GX_BABY)
		ddr_test_gx_training_pattern(ddr_test_size);
#endif
	}

	{
		printf("\nStart writing at 0x%8x - 0x%8x...", start_addr, start_addr +
		       ddr_test_size);
		ddr_write((void *)(int_convert_p(start_addr)), ddr_test_size);
		printf("\nEnd write.                                 ");
		printf("\nStart 1st reading...                       ");
		ddr_read((void *)(int_convert_p(start_addr)), ddr_test_size);
		printf("\nEnd 1st read.                              ");
		printf("\nStart 2nd reading...                       ");
		ddr_read((void *)(int_convert_p(start_addr)), ddr_test_size);

		printf("\nStart writing pattern4 at 0x%8x - 0x%8x...", start_addr,
		       start_addr + ddr_test_size);
		ddr_write4((void *)(int_convert_p(start_addr)), ddr_test_size);
		printf("\nEnd write.                                 ");
		printf("\nStart 1st reading...                       ");
		ddr_read4((void *)(int_convert_p(start_addr)), ddr_test_size);
		printf("\nEnd 1st read.                              ");
		printf("\nStart 2nd reading...                       ");
		ddr_read4((void *)(int_convert_p(start_addr)), ddr_test_size);

		printf("\nStart x4 no cross talk pattern.                                 ");
		printf("\nStart writing at 0x%8x - 0x%8x...", start_addr,
		       start_addr + ddr_test_size);
		ddr_write_pattern4_no_cross_talk((void *)(int_convert_p(start_addr)),
						 ddr_test_size);
		printf("\nEnd write.                                 ");
		printf("\nStart 1st reading...                       ");
		ddr_read_pattern4_no_cross_talk((void *)(int_convert_p(start_addr)),
						ddr_test_size);
		printf("\nEnd 1st read.                              ");
		printf("\nStart 2nd reading...                       ");
		ddr_read_pattern4_no_cross_talk((void *)(int_convert_p(start_addr)),
						ddr_test_size);
	}
	{
		printf("\nStart x4  cross talk pattern p.                                 ");
		printf("\nStart writing at 0x%8x - 0x%8x...", start_addr,
		       start_addr + ddr_test_size);
		ddr_write_pattern4_cross_talk_p((void *)(int_convert_p(start_addr)),
						ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_pattern4_cross_talk_p((void *)(int_convert_p(start_addr)),
					       ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_pattern4_cross_talk_p((void *)(int_convert_p(start_addr)),
					       ddr_test_size);
		printf("\rEnd 2nd read.                              ");

		printf("\nStart x4  cross talk pattern n.                                 ");
		printf("\nStart writing at 0x%8x - 0x%8x...", start_addr,
		       start_addr + ddr_test_size);
		ddr_write_pattern4_cross_talk_n((void *)(int_convert_p(start_addr)),
						ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_pattern4_cross_talk_n((void *)(int_convert_p(start_addr)),
					       ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_pattern4_cross_talk_n((void *)(int_convert_p(start_addr)),
					       ddr_test_size);
		printf("\rEnd 2nd read.                              ");
	}

	{
		printf("\nStart x4  cross talk pattern p2.                                 ");
		printf("\nStart writing at 0x%8x - 0x%8x...", start_addr,
		       start_addr + ddr_test_size);
		ddr_write_pattern4_cross_talk_p2((void *)(int_convert_p(start_addr)),
						 ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_pattern4_cross_talk_p2((void *)(int_convert_p(start_addr)),
						ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_pattern4_cross_talk_p2((void *)(int_convert_p(start_addr)),
						ddr_test_size);
		printf("\rEnd 2nd read.                              ");

		printf("\nStart x4  cross talk pattern n.                                 ");
		printf("\nStart writing at 0x%8x - 0x%8x...", start_addr,
		       start_addr + ddr_test_size);
		ddr_write_pattern4_cross_talk_n2((void *)(int_convert_p(start_addr)),
						 ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_p4_cross_talk_n2((void *)(int_convert_p(start_addr)),
					  ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_p4_cross_talk_n2((void *)(int_convert_p(start_addr)),
					  ddr_test_size);
		printf("\rEnd 2nd read.                              ");
		if (copy_test_flag) {
			printf("\n start copy test  ...                            ");
			ddr_test_copy((void *)(int_convert_p(start_addr + ddr_test_size / 2)),
				      (void *)(int_convert_p(start_addr)), ddr_test_size / 2);
			ddr_read_p4_cross_talk_n2((void *)(int_convert_p(start_addr +
									 ddr_test_size / 2)),
						  ddr_test_size / 2);
			ddr_read_p4_cross_talk_n2((void *)(int_convert_p(start_addr +
									 ddr_test_size / 2)),
						  ddr_test_size / 2);
		}
	}

	if (error_count)
		return 1;
	else
		return 0;
}

//int do_ddr2pll_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
//{
//#if (CONFIG_DDR_PHY >= P_DDR_PHY_G12)
//	//extern int do_ddr2pll_g12_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[]);
//	do_ddr2pll_g12_cmd(cmdtp, flag, argc, argv);
//	return 1;
//#endif
//}

unsigned int do_test_address_bus(unsigned int *baseAddress, unsigned int nBytes)
{
	unsigned int addressMask = (nBytes / sizeof(unsigned int) - 1);
	unsigned int offset;
	unsigned int testOffset;

	unsigned int pattern = 0xAAAAAAAAU;
	unsigned int antipattern = 0x55555555U;

	unsigned int data1, data2;

	unsigned int ret = 0;

	/*
	 * Write the default pattern at each of the power-of-two offsets.
	 */
	for (offset = 1; (offset & addressMask) != 0; offset <<= 1)
		baseAddress[offset] = pattern;

	/*
	 * Check for address bits stuck high.
	 */
	testOffset = 0;
	baseAddress[testOffset] = antipattern;

	for (offset = 1; (offset & addressMask) != 0; offset <<= 1) {
		data1 = baseAddress[offset];
		data2 = baseAddress[offset];
		if (data1 != data2) {
			printf("  memTestAddressBus - read twice different[offset]: 0x%8x-0x%8x\n",
			       data1, data2);
			ret = 1;
		}
		if (data1 != pattern) {
			printf("  memTestAddressBus - write[0x%8x]: 0x%8x, read[0x%8x]: 0x%8x\n",
			       offset, pattern, offset, data1);
			ret = 1;
		}
	}

	baseAddress[testOffset] = pattern;

	/*
	 * Check for address bits stuck low or shorted.
	 */
	for (testOffset = 1; (testOffset & addressMask) != 0; testOffset <<= 1) {
		baseAddress[testOffset] = antipattern;

		if (baseAddress[0] != pattern) {
			printf("memTestAdd2-write Add[0x%8x]: 0x%8x,read Add[0]: 0x%8x\n",
			       testOffset, antipattern, baseAddress[0]);
			ret = 1;
		}

		for (offset = 1; (offset & addressMask) != 0; offset <<= 1) {
			data1 = baseAddress[offset];
			if (data1 != pattern && offset != testOffset) {
				printf("memTestAdd3-write Add[0x%8x]: 0x%8x,read Add[0]: 0x%8x\n",
				       testOffset, antipattern, data1);
				ret = 1;
			}
		}

		baseAddress[testOffset] = pattern;
	}

	for (offset = 0x1; (offset <= addressMask); offset++)
		if (~offset <= addressMask) {
			baseAddress[offset] = pattern;
			baseAddress[(~offset)] = antipattern;
		}

	for (offset = 0x1; (offset <= addressMask); offset++) {
		if (~offset <= addressMask) {
			if (baseAddress[offset] != pattern) {
				printf("memTestAdd4-write Add[0x%8x]: 0x%8x,read Add[0]: 0x%8x\n",
				       offset, pattern, baseAddress[offset]);

				ret = 1;
				break;
			}

			if (baseAddress[~offset] != antipattern) {
				printf("memTestAdd5-write Add[0x%8x]: 0x%8x,read Add[0]: 0x%8x\n",
				       ~offset, antipattern,
				       baseAddress[~offset]);
				ret = 1;
				break;
			}
		}
	}

	if (ret)
		return ret;

	for (offset = 0x1; (offset <= addressMask); offset++) {
		{
			pattern = ((offset << 2) - offset);
			baseAddress[offset] = pattern;
		}
	}

	for (offset = 0x1; (offset <= addressMask); offset++) {
		{
			pattern = ((offset << 2) - offset);
			if (baseAddress[offset] != pattern) {
				printf("memTestAdd6-write Add[0x%8x]: 0x%8x,read Add[0]: 0x%8x\n",
				       offset, pattern, baseAddress[offset]);
				ret = 1;
				break;
			}
		}
	}

	if (ret)
		return ret;

	for (offset = 0x1; (offset <= addressMask); offset++) {
		{
			pattern = ~((offset << 2) - offset);
			baseAddress[offset] = pattern;
		}
	}

	for (offset = 0x1; (offset <= addressMask); offset++) {
		{
			pattern = ~((offset << 2) - offset);
			if (baseAddress[offset] != pattern) {
				printf("memTestAdd7-write Add[0x%8x]: 0x%8x,read Add[0]: 0x%8x\n",
				       offset, pattern, baseAddress[offset]);
				ret = 1;
				break;
			}
		}
	}

	return ret;
}   /* memTestAddressBus() */

int ddr_test_s_add_cross_talk_pattern(int ddr_test_size)
{
	unsigned int start_addr = test_start_addr;

	error_outof_count_flag = 1;
	error_count = 0;
	printf("\rStart writing at 0x%8x - 0x%8x...", start_addr, start_addr + ddr_test_size);
	ddr_write((void *)(int_convert_p(start_addr)), ddr_test_size);
	printf("\rEnd write.                                 ");
	printf("\nStart 1st reading...                       ");
	ddr_read((void *)(int_convert_p(start_addr)), ddr_test_size);
	printf("\rEnd 1st read.                              ");
	printf("\rStart 2nd reading...                       ");
	ddr_read((void *)(int_convert_p(start_addr)), ddr_test_size);
	ddr_write_full((void *)(int_convert_p(start_addr)), ddr_test_size, 0x0, 0x3);
	printf("\rEnd write.                                 ");
	printf("\rStart 1st reading...                       ");
	ddr_read_full((void *)(int_convert_p(start_addr)), ddr_test_size, 0, 3);
	printf("\rEnd 1st read.                              ");
	printf("\rStart 2nd reading...                       ");
	ddr_read_full((void *)(int_convert_p(start_addr)), ddr_test_size, 0, 3);

	printf("\rStart writing add pattern                                 ");
	if (do_test_address_bus((void *)(int_convert_p(start_addr)), ddr_test_size))
		error_count++;

	if (error_count)
		return 1;
	else
		return 0;
}

int pll_convert_to_ddr_clk_g12a(unsigned int ddr_pll)
{
	unsigned int ddr_clk = 0;
	unsigned int od_div = 0xfff;

	ddr_pll = ddr_pll & 0xfffff;
	if (((ddr_pll >> 16) & 7) == 0)
		od_div = 2;
	if (((ddr_pll >> 16) & 7) == 1)
		od_div = 3;

	if (((ddr_pll >> 16) & 7) == 2)
		od_div = 4;

	if (((ddr_pll >> 16) & 7) == 3)
		od_div = 6;

	if (((ddr_pll >> 16) & 7) == 4)
		od_div = 8;

	if (((ddr_pll >> 10) & 0x1f))
		ddr_clk = 2 * ((((24 * ((ddr_pll >> 0) & 0x1ff)) / ((ddr_pll >> 10) & 0x1f)) >>
				((((ddr_pll >> 19) & 0x1) == 1) ? (2) : (1)))) / od_div;

	return ddr_clk;
}

int ddr_clk_convert_to_pll_g12a(unsigned int ddr_clk, unsigned char pll_bypass_en)
{
	uint32_t ddr_pll_vco_ctrl = 0;
	uint32_t ddr_pll_vco_m = 0;
	uint32_t ddr_pll_vco_n = 0;
	uint32_t ddr_pll_vco_ctrl_od = 0;
	uint32_t ddr_pll_vco_ctrl_od1 = 0;

	ddr_pll_vco_n = 1;
	if (pll_bypass_en == 0) {
		if ((ddr_clk >= 4800 / 4)) {
			ddr_pll_vco_ctrl_od = 1;
			ddr_pll_vco_ctrl_od1 = 0x2;             //0
			ddr_pll_vco_m = (ddr_clk * 3) / 24;     //6
		} else if ((ddr_clk > 4800 / 6)) {
			ddr_pll_vco_ctrl_od = 2;
			ddr_pll_vco_ctrl_od1 = 0x2;             //0
			ddr_pll_vco_m = (ddr_clk * 4) / 24;     //8
		} else if ((ddr_clk > 4800 / 8)) {
			ddr_pll_vco_ctrl_od = 3;
			ddr_pll_vco_ctrl_od1 = 0x2;             //0
			ddr_pll_vco_m = (ddr_clk * 6) / 24;     //12
		} else if ((ddr_clk > 4800 / 12)) {
			ddr_pll_vco_ctrl_od = 4;
			ddr_pll_vco_ctrl_od1 = 0x2;             //0
			ddr_pll_vco_m = (ddr_clk * 8) / 24;     //16
		} else if ((ddr_clk > 360)) {
			ddr_pll_vco_ctrl_od = 3;
			ddr_pll_vco_ctrl_od1 = 0x3; //0
			ddr_pll_vco_m = (ddr_clk * 12) / 24;
		} else {
			ddr_pll_vco_ctrl_od = 4;
			ddr_pll_vco_ctrl_od1 = 0x3; //0
			ddr_pll_vco_m = (ddr_clk * 16) / 24;
		}
	}

	ddr_pll_vco_ctrl = ddr_pll_vco_m | (ddr_pll_vco_n << 10) |
			   (ddr_pll_vco_ctrl_od << 16) | (ddr_pll_vco_ctrl_od1 << 19);
	return ddr_pll_vco_ctrl;
}

int pll_convert_to_ddr_clk(unsigned int ddr_pll)
{
	unsigned int ddr_clk = 0;

	ddr_pll = ddr_pll & 0xfffff;

#if (CONFIG_DDR_PHY >= P_DDR_PHY_G12)
	ddr_clk = pll_convert_to_ddr_clk_g12a(ddr_pll);
	return ddr_clk;
#else
#if (CONFIG_DDR_PHY == P_DDR_PHY_905X)
	if (((ddr_pll >> 16) & 0x1f)) {
		ddr_clk = 2 * ((((24 * ((ddr_pll >> 4) & 0x1ff)) /
				 ((ddr_pll >> 16) & 0x1f)) >> ((((ddr_pll >> 0) & 0x3) == 3) ?
							       (2) : (((ddr_pll >> 0) & 0x3)))) >>
			       ((((ddr_pll >> 2) & 0x3) == 3) ?
				(2) : (((ddr_pll >> 2) & 0x3))));
	}

#else
	if ((ddr_pll >> 9) & 0x1f)
		ddr_clk = 2 * (((24 * (ddr_pll & 0x1ff)) /
				((ddr_pll >> 9) & 0x1f)) >> ((ddr_pll >> 16) & 0x3));

#endif
	return ddr_clk;
#endif
#if (CONFIG_DDR_PHY == P_DDR_PHY_DEFAULT)
	if ((ddr_pll >> 9) & 0x1f)
		ddr_clk = 2 * ((24 * (ddr_pll & 0x1ff)) /
			       ((ddr_pll >> 9) & 0x1f)) >> ((ddr_pll >> 16) & 0x3);
	return ddr_clk;
#endif

	//return ddr_clk;
}

int ddr_clk_convert_to_pll(unsigned int ddr_clk)
{
	unsigned int ddr_pll = 0x10221;

	ddr_pll = ddr_clk_convert_to_pll_g12a(ddr_clk, 0);
	return ddr_pll;
}

int get_ddr_clk(void)
{
	unsigned int ddr_clk = 10;
	uint64_t stick_ddr_clk_add1 = 0;
	uint64_t stick_ddr_clk_add2 = 0;

	stick_ddr_clk_add1 = (uint64_t)(&ddr_set_array[0].cfg_board_SI_setting_ps.DRAMFreq);
	stick_ddr_clk_add2 = (uint64_t)(&ddr_set_array[0]);
	stick_ddr_clk_add2 = sticky_reg_base_add + stick_ddr_clk_add1 - stick_ddr_clk_add2;
	if (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S5) {
		ddr_clk = rd_reg(stick_ddr_clk_add2);
		printf("\nget ddr clk from add 0x%x", (uint32_t)stick_ddr_clk_add2);
		ddr_clk = ddr_clk & 0xffff;
	}
	return ddr_clk;
}

void ddr_memcpy(void *dst, const void *src, uint32_t len)
{
	len = (len >> 3);
	const long long *s = src;
	long long *d = dst;

#ifdef TEST_L1_CACHE
	void *bound = (void *)src + 16 * 1024;
	//debug for test L1 cache ,if only read write small aread
#endif
	if (pre_fetch_enable) {
		while (len) {
			//for best test efficiency  not inclued much more code in the while loop
			ddr_pld_cache(s);
			///1 times   len==33554432   copy time==18192 us   1.2g  bandwidth 3688M/S
			// 4times   len==33554432   copy time==11844 us   1.2g  bandwidth 5666M/S
			// 8times   len==33554432   copy time==11844 us   1.2g  bandwidth 5666M/S
			*d++ = *s++;
			*d++ = *s++;
			*d++ = *s++;
			*d++ = *s++;
			len = len - 4;
#ifdef TEST_L1_CACHE
			if ((void *)s >= bound) {
				s = src;
				d = dst;
			}
#endif
		}
	} else {
		while (len) {
			//for best test efficiency  not inclued much more code in the while loop
			///1 times   len==33554432   copy time==18192 us   1.2g  bandwidth 3688M/S
			// 4times   len==33554432   copy time==11844 us   1.2g  bandwidth 5666M/S
			// 8times   len==33554432   copy time==11844 us   1.2g  bandwidth 5666M/S
			*d++ = *s++;
			*d++ = *s++;
			*d++ = *s++;
			*d++ = *s++;
			len = len - 4;
#ifdef TEST_L1_CACHE
			if ((void *)s >= bound) {
				s = src;
				d = dst;
			}
#endif
		}
	}
}

#define PATTERN_MATRIX_X   (3 + 32 + 16 + 17) //68*32==2176///2.2k -0x880-1 loop
#define PATTERN_MATRIX_Y  (32)
#define PATTERN_MATRIX_LOOP_SIZE   ((PATTERN_MATRIX_X) * (PATTERN_MATRIX_Y) * 4)

unsigned int cpu_ddr_test_init_pattern_generater(unsigned int add_offset)
{
	unsigned int pattern_select = 0;
	unsigned int pattern_value = 0;
	uint32_t martix_x_select = 0;
	uint32_t martix_y_select = 0;
	unsigned int pattern_value_temp_16 = 0;

	{
		{
			pattern_select = ((add_offset) % ((PATTERN_MATRIX_Y) * (PATTERN_MATRIX_X)));
			martix_x_select = pattern_select / (PATTERN_MATRIX_Y);
			martix_y_select = pattern_select % (PATTERN_MATRIX_Y);
			{
				if (martix_x_select == 0)
					pattern_value = 0xaaaa5555;  //for 16 bit bus pattern

				if (martix_x_select == 1)
					pattern_value = 0x0000ffff;  //for 16 bit bus pattern

				if (martix_x_select == 2)
					pattern_value = 0;

				if (martix_x_select > 2 && (martix_x_select < (3 + 32)))
					pattern_value = 1 << (martix_x_select - 3);
				if ((martix_x_select > (2 + 32)) &&
				    (martix_x_select < (3 + 32 + 16))) {
					//for 16 bit bus pattern
					pattern_value_temp_16 = (1 << (martix_x_select - 3 - 32));
					pattern_value = pattern_value_temp_16 |
							((~pattern_value_temp_16) << 16);
				}
				if ((martix_x_select > (2 + 32 + 16)) &&
				    (martix_x_select < (3 + 32 + 16 + 17))) {
					//for dbi bus pattern  17 group
					pattern_value_temp_16 = (0x0f0f +
								 0xf0f * (martix_x_select - 3 -
									  32 - 16));
					pattern_value = pattern_value_temp_16 |
							((~pattern_value_temp_16) << 16);
				}
			}
			if (martix_y_select % 2)
				pattern_value = ~pattern_value;
		}
	}

	return pattern_value;
}

void cpu_ddr_test_init_pattern_area(unsigned int test_init_start, unsigned int test_size,
				    unsigned int parttern_frequency_setting)
{
	if (parttern_frequency_setting == 0)
		parttern_frequency_setting = 1;  //for different  frequency pattern
	test_size = (test_size > ((PATTERN_MATRIX_LOOP_SIZE) * (parttern_frequency_setting))) ?
		    test_size : ((PATTERN_MATRIX_LOOP_SIZE) * (parttern_frequency_setting));
	unsigned int write_add = test_init_start;
	unsigned int size_loop = 0;
	unsigned int size_loop_max = 0;

	for (; (size_loop < ((PATTERN_MATRIX_LOOP_SIZE) * (parttern_frequency_setting))); ) {
		{
			write_add = (uint32_t)(size_loop + test_init_start);
			wr_reg((unsigned long)write_add,
			       cpu_ddr_test_init_pattern_generater((size_loop >> 2) /
								   parttern_frequency_setting));
			size_loop = size_loop + 4;
		}
	}
	size_loop = 1;
	size_loop_max = ((test_size /
			  (((PATTERN_MATRIX_LOOP_SIZE) * (parttern_frequency_setting)))) + 1);
	for (; (size_loop < size_loop_max); ) {
		ddr_memcpy((void *)(uint64_t)(test_init_start +
					      ((PATTERN_MATRIX_LOOP_SIZE) *
					       ((uint64_t)parttern_frequency_setting)) *
					      (size_loop)),
			   (void *)(uint64_t)test_init_start,
			   (uint32_t)((PATTERN_MATRIX_LOOP_SIZE) * (parttern_frequency_setting)));
		size_loop++;
	}
}

unsigned int cpu_ddr_test(unsigned test_init_start, unsigned int start_add,
			  unsigned int test_size, unsigned int test_data_bit_enable,
			  unsigned int parttern_frequency_setting)
{
	unsigned int src_add = test_init_start;
	unsigned int pattern_value = 0;
	unsigned int size_loop = 0;
	unsigned int ddr_test_error = 0;
	unsigned int read_add = 0;
	unsigned int read_value = 0;

	test_size = (test_size > 0x2000) ? (test_size) : (0x2000);
	uint32_t ddr_test_start_time_us = get_us_time(); // check cost time

	ddr_memcpy((void *)(uint64_t)start_add, (void *)(uint64_t)src_add, test_size);
	uint32_t ddr_test_end_time_us = get_us_time(); // check cost time

	printf("\ncpu_ddr_test_test_copy_bandwidth==%d Mbyte/s\n", (1 * test_size * 2) /
	       (ddr_test_end_time_us - ddr_test_start_time_us));

	for (; size_loop < (test_size); ) {
		read_add = (uint32_t)(size_loop + start_add);
		read_value = (rd_reg((unsigned long)read_add));
		pattern_value = (cpu_ddr_test_init_pattern_generater((size_loop >> 2) /
								     parttern_frequency_setting));
		if (((test_data_bit_enable) & read_value) !=
		    ((test_data_bit_enable) & pattern_value)) {
			printf("error data  enable %8x read_value %8x  pattern_value %8x",
			       test_data_bit_enable, read_value, pattern_value);
			ddr_test_error++;
			return ddr_test_error;
		}
		size_loop = size_loop + (1 << 2);
		// use big step will fast test ,but lose accuracy.
	}
	return ddr_test_error;
}

int do_cpu_ddr_test(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	check_base_address();
	int i = 0;

	printf("\nargc== 0x%8x\n", argc);
	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);
	unsigned int init_start_add = 0;
	unsigned int test_add = 0;
	unsigned int test_size = 0;
	unsigned int test_data_bit_enable = 0;
	unsigned int test_loops = 0;
	unsigned int test_loop = 0;
	unsigned int test_errors = 0;
	unsigned int parttern_frequency_setting = 1;
	char *endp;

	if (argc == 1) {
		printf("\nplease read help\n");
	} else {
		if (argc >= 2)
			init_start_add = simple_strtoull_ddr(argv[1], &endp, 0);

		if (argc >= 3)
			test_add = simple_strtoull_ddr(argv[2], &endp, 0);
		if (argc >= 4)
			test_size = simple_strtoull_ddr(argv[3], &endp, 0);
		if (argc >= 5)
			test_data_bit_enable = simple_strtoull_ddr(argv[4], &endp, 0);
		if (argc >= 6) {
			test_loops = simple_strtoull_ddr(argv[5], &endp, 0);
			if (test_loops == 0)
				test_loops = 1;
		}
		if (argc >= 7) {
			parttern_frequency_setting = simple_strtoull_ddr(argv[6], &endp, 0);
			if (parttern_frequency_setting == 0)
				parttern_frequency_setting = 1;
		}
		if (argc >= 8)
			pre_fetch_enable = simple_strtoull_ddr(argv[7], &endp, 0);
	}
	uint32_t ddr_test_start_time_us = get_us_time(); // check cost time

	cpu_ddr_test_init_pattern_area(init_start_add, test_size, parttern_frequency_setting);
	for (test_loop = 0; test_loop < test_loops; ) {
		test_errors = test_errors + cpu_ddr_test(init_start_add, test_add, test_size,
							 test_data_bit_enable,
							 parttern_frequency_setting);
		test_loop++;

		printf("\ncpu_ddr_test_test_times==%d  test_errors==%d", test_loop, test_errors);
	}
	uint32_t ddr_test_end_time_us = get_us_time(); // check cost time

	printf("\ncpu_ddr_test_test_and compare_bandwidth==%d Mbyte/s\n",
	       (test_loops * test_size * 2) / (ddr_test_end_time_us - ddr_test_start_time_us));
	return test_errors;
}

U_BOOT_CMD(ddr_cpu_test, 30, 1, do_cpu_ddr_test,
	   "ddr_test_cmd cmd arg1 arg2 arg3...",
	   "ddr_test_cmd cmd arg1 arg2 arg3...\n dcache off ?\n");

int do_ddr_test_write_read(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	{
		printf("\nEnter do_ddr_test_ddr_write_read_current\n");
		int i = 0;

		printf("\nargc== 0x%8x\n", argc);
		for (i = 0; i < argc; i++)
			printf("\nargv[%d]=%s\n", i, argv[i]);

		char *endp;

		unsigned int pattern_id = 1;
		unsigned int pattern[4] = { 0 };
		unsigned int write_read = 0;
		unsigned int read_pattern[4] = { 0 };
		unsigned int loop = 1;
		unsigned int start_addr = DDR_TEST_START_ADDR;
		unsigned int test_size = DDR_TEST_SIZE;
		unsigned int copy_offset = DDR_TEST_SIZE;
		unsigned int no_show_info = 0;
		unsigned int us_delay_counter = 0;

		if (argc == 1) {
			printf("\nplease read help\n");
		} else {
			if (argc >= 2)
				write_read = simple_strtoull_ddr(argv[1], &endp, 0);
			if (argc >= 3)
				pattern_id = simple_strtoull_ddr(argv[2], &endp, 0);
			if (argc >= 4)
				loop = simple_strtoull_ddr(argv[3], &endp, 0);
			if (argc >= 5)
				start_addr = simple_strtoull_ddr(argv[4], &endp, 0);
			if (argc >= 6)
				test_size = simple_strtoull_ddr(argv[5], &endp, 0);
			if (argc >= 7)
				no_show_info = simple_strtoull_ddr(argv[6], &endp, 0);
			if (argc >= 8)
				us_delay_counter = simple_strtoull_ddr(argv[7], &endp, 0);
		}
		printf("\nwrite_read== %8d\n", write_read);
		printf("\npattern_id== %8d\n", pattern_id);
		printf("\nloop== %8d\n", loop);
		printf("\nstart_addr== 0x%8x\n", start_addr);
		printf("\ntest_size== 0x%8x\n", test_size);
		printf("\nus_delay_counter== %d\n", us_delay_counter);
		copy_offset = test_size;

		unsigned int *p;
		unsigned int j;

		p = (unsigned int *)(int_convert_p(start_addr));

		if (pattern_id == 0) {
			pattern[0] = 0;
			pattern[1] = 0;
			pattern[2] = 0;
			pattern[3] = 0;
		}
		if (pattern_id == 1) {
			pattern[0] = 0xffffffff;
			pattern[1] = 0xffffffff;
			pattern[2] = 0xffffffff;
			pattern[3] = 0xffffffff;
		}

		do {
			if (write_read == 0) {
				if (!no_show_info)
					printf("\nloop:0x%8x:Start writing at 0x%8x - 0x%8x...",
					       loop, start_addr, start_addr + test_size);
				for (j = 0; j < test_size / 4; ) {
					*(p + j) = (pattern[0]);
					*(p + j + 1) = (pattern[1]);
					*(p + j + 2) = (pattern[2]);
					*(p + j + 3) = (pattern[3]);
					j = j + 4;
				}
			}
			if (write_read == 1) {
				if (!no_show_info)
					printf("\nloop:0x%8x:Start reading at 0x%8x - 0x%8x...",
					       loop, start_addr, start_addr + test_size);
				for (j = 0; j < test_size / 4; ) {
					read_pattern[0] = *(p + j);
					read_pattern[1] = *(p + j + 1);
					read_pattern[2] = *(p + j + 2);
					read_pattern[3] = *(p + j + 3);
					j = j + 4;
				}

				if (loop == 1) {
					if (!no_show_info) {
						printf("\nStart reading read_pattern[0] loop:0x");
						printf("%x 0x%x, p1 0x%x,p2 0x%x,p3 0x%x",
						       loop, read_pattern[0], read_pattern[1],
						       read_pattern[2], read_pattern[3]);
					}
				}
			}
			if (write_read == 2) {
				if (!no_show_info)
					printf("\nloop:0x%8x:Start copying at 0x%8x - 0x%8x...",
					       loop, start_addr, start_addr + test_size);
				for (j = 0; j < test_size / 4; ) {
					*(p + j + copy_offset / 4) = *(p + j);
					*(p + j + 1 + copy_offset / 4) = *(p + j + 1);
					*(p + j + 2 + copy_offset / 4) = *(p + j + 2);
					*(p + j + 3 + copy_offset / 4) = *(p + j + 3);
					j = j + 4;
				}
			}
			if (us_delay_counter)
				ddr_udelay(us_delay_counter);
		} while (loop--);

		printf("\ntest end\n");

		return 1;
	}
}

#if (CONFIG_DDR_PHY >= P_DDR_PHY_G12)

#define TEST_MIN_DDR_EE_VOLTAGE  681
#define TEST_MAX_DDR_EE_VOLTAGE  962
static int pwm_voltage_table_ee[][2] = {
	{ 0x1c0000, 681 },
	{ 0x1b0001, 691 },
	{ 0x1a0002, 701 },
	{ 0x190003, 711 },
	{ 0x180004, 721 },
	{ 0x170005, 731 },
	{ 0x160006, 741 },
	{ 0x150007, 751 },
	{ 0x140008, 761 },
	{ 0x130009, 772 },
	{ 0x12000a, 782 },
	{ 0x11000b, 792 },
	{ 0x10000c, 802 },
	{ 0x0f000d, 812 },
	{ 0x0e000e, 822 },
	{ 0x0d000f, 832 },
	{ 0x0c0010, 842 },
	{ 0x0b0011, 852 },
	{ 0x0a0012, 862 },
	{ 0x090013, 872 },
	{ 0x080014, 882 },
	{ 0x070015, 892 },
	{ 0x060016, 902 },
	{ 0x050017, 912 },
	{ 0x040018, 922 },
	{ 0x030019, 932 },
	{ 0x02001a, 942 },
	{ 0x01001b, 952 },
	{ 0x00001c, 962 }
};

uint32_t find_vddee_voltage_index(unsigned int target_voltage)
{
	unsigned int to;

	for (to = 0; to < ARRAY_SIZE(pwm_voltage_table_ee); to++)
		if (pwm_voltage_table_ee[to][1] >= target_voltage)
			break;

	if (to >= ARRAY_SIZE(pwm_voltage_table_ee))
		to = ARRAY_SIZE(pwm_voltage_table_ee) - 1;
	return to;
}

void set_ee_voltage(uint32_t ee_over_ride_voltage)
{
	unsigned int to;

	for (to = (ARRAY_SIZE(pwm_voltage_table_ee) - 1); (to > 0); to--)
		if (pwm_voltage_table_ee[to - 1][1] < ee_over_ride_voltage &&
		    pwm_voltage_table_ee[to][1] >= ee_over_ride_voltage)
			break;
	if (to == ARRAY_SIZE(pwm_voltage_table_ee))
		to = (ARRAY_SIZE(pwm_voltage_table_ee) - 1);
	if (ee_over_ride_voltage) {
		writel(pwm_voltage_table_ee[to][0], (p_ddr_base->ee_pwm_base_address));
		printf("\nDDR_override_EE_voltage ==%d mv /n", pwm_voltage_table_ee[to][1]);
	}
}

unsigned int read_ee_voltage(void)
{
	unsigned int to;
	unsigned int reg_value = 0;

	reg_value = readl((p_ddr_base->ee_pwm_base_address));
	to = reg_value & 0xff;
	return pwm_voltage_table_ee[to][1];
}

uint32_t get_bdlr_100step(uint32_t ddr_frequency)
{
	uint32_t bdlr_100step = 0;

	//if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_C2) ||
	//    (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5) ||
	//    (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5D) ||
	//    (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S4)
	//    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)) {
	//	bdlr_100step = do_read_c2_ddr_bdlr_steps();
	//} else

	// com_message_p->cur_pstate =
	// 0;//(rd_reg_dmc(dmc_base_add,DMC_DRAM_FREQ_CTRL)) & 1;
	//wr_reg_ddr_phy(phy_base_add, 0x0434), 0xc00);
	uint32_t dll_counter = 0;

	dll_counter = ((rd_reg_ddr_phy(phy_base_add, 0x434)) & 0x1ff);
	dll_counter = dll_counter ? dll_counter : 1;
	bdlr_100step =
		(100000000 / (2 * (p_ddrs->cfg_board_SI_setting_ps.DRAMFreq))) /
		(dll_counter);
	return bdlr_100step;
}

int do_ddr_test_pwm_bdlr(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char *endp;

	printf("\nEnter g12 do_ddr_test_pwm_bdl function\n");
	int i = 0;

	printf("\nargc== 0x%8x\n", argc);
	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);

	unsigned int argc_count = 1;
	unsigned int para_meter[30] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,  0,
	};

	while (argc_count < argc) {
		para_meter[argc_count - 1] = simple_strtoul(argv[argc_count], &endp, 0);
		if (*argv[argc_count] == 0 || *endp != 0)
			para_meter[argc_count - 1] = 0;
		argc_count++;
	}

	uint32_t loop = para_meter[0];
	uint32_t voltage_min = para_meter[1];
	uint32_t voltage_max = para_meter[2];
	uint32_t show_count_message = para_meter[3];

#define PWM_LOOP_DEFAULT   (10 << 0)
#define PWM_VOLTAGE_MIN_DEFAULT   TEST_MIN_DDR_EE_VOLTAGE
#define PWM_VOLTAGE_MAX_DEFAULT   TEST_MAX_DDR_EE_VOLTAGE

	loop = loop ? loop : PWM_LOOP_DEFAULT;
	voltage_min = (voltage_min < PWM_VOLTAGE_MIN_DEFAULT) ?
		      PWM_VOLTAGE_MIN_DEFAULT : voltage_min;
	voltage_max = (voltage_max > PWM_VOLTAGE_MAX_DEFAULT) ?
		      PWM_VOLTAGE_MAX_DEFAULT : voltage_max;
	voltage_max = (voltage_max < PWM_VOLTAGE_MIN_DEFAULT) ?
		      PWM_VOLTAGE_MAX_DEFAULT : voltage_max;

	uint16_t bdlr_100_min = 0;
	uint16_t bdlr_100_average = 0;
	uint16_t bdlr_100_max = 0;
	uint16_t bdlr_100_cur = 0;
	uint32_t count = 1;

	bdlr_100_cur = get_bdlr_100step(global_ddr_clk);
	bdlr_100_min = bdlr_100_cur;
	bdlr_100_max = bdlr_100_cur;
	bdlr_100_average = bdlr_100_cur;

	unsigned int to = 0;
	unsigned int to_min = 0;
	unsigned int to_max = (ARRAY_SIZE(pwm_voltage_table_ee)) - 1;

	printf("\nread org_EE_voltage %d mv\n", read_ee_voltage());
	to_min = find_vddee_voltage_index(voltage_min);
	to_max = find_vddee_voltage_index(voltage_max);
	for (to = (to_max + 1); (to > to_min); to--) {
		writel(pwm_voltage_table_ee[to - 1][0], (p_ddr_base->ee_pwm_base_address));
		udelay(1000);
		bdlr_100_cur = get_bdlr_100step(global_ddr_clk);
		bdlr_100_min = bdlr_100_cur;
		bdlr_100_max = bdlr_100_cur;
		bdlr_100_average = bdlr_100_cur;
		count = 1;

		do {
			bdlr_100_cur = (100000000 / (2 * global_ddr_clk)) /
				       ((dwc_ddrphy_apb_rd((((0 << 20) |
							     (2 << 16) | (0 << 12) |
							     (0xe4))))) & 0x3ff);
			bdlr_100_min = (bdlr_100_cur < bdlr_100_min) ? bdlr_100_cur : bdlr_100_min;
			bdlr_100_max = (bdlr_100_cur > bdlr_100_max) ? bdlr_100_cur : bdlr_100_max;
			bdlr_100_average = (bdlr_100_cur + bdlr_100_average * count) / (count + 1);
			count++;
			if (show_count_message)
				printf("%d\n", bdlr_100_cur);
		} while (count < loop);
		printf("\nset EE_vol %d  bdlr_100_ave %d bdlr_100_min %d bdlr_100_max %d count %d",
		       pwm_voltage_table_ee[to - 1][1],
		       bdlr_100_average, bdlr_100_min, bdlr_100_max, count);
	}
	return 1;
}

int printf_log(char log_level, const char *fmt, ...)
{
	if (log_level < 1) {
		va_list args;

		va_start(args, fmt);
		vprintf(fmt, args); //
		va_end(args);
		return 0;
	} else {
		return 1;
	}
}

uint32_t ddr_mask_convert_offset(uint32_t mask)
{
	uint32_t offset = 0;

	while ((mask & 1)) {
		offset++;
		mask = (mask >> 1);
	}
	return offset;
}

uint32_t ddr_disable_update_delay_line_s5(void)
{
	//config phy update use register change and
	// ctrl update req and condition,power on default is or condition
	//disable ctrl update req
	return 1;
}

uint32_t ddr_enable_update_delay_line_s5(void)
{
	//config phy update use register change and ctrl
	//update req and condition,power on default is or condition
	//enable ctrl update req
	return 1;
}

uint32_t ddr_training_reg_rw(ddr_set_ps0_only_t *p_ddrs, char index,
			     uint32_t sub_index, uint32_t read_write_value,
			     char read_write_enable, char ps)
{
	//read_write_value	bit0-15 fine value ,bit 16-32 coarse value
	uint32_t delay_old_value = 0;
	uint32_t delay_new_value = 0;
	uint32_t delay_reg_coarse_value = 0;
	uint32_t delay_reg_fine_value = 0;
	uint64_t reg_add_coarse = 0;
	uint32_t reg_add_coarse_bit_mask = 0;
	uint64_t reg_add_fine = 0;
	uint32_t reg_add_fine_bit_mask = 0;
	uint64_t add_base = 0;
	char sub_ch_mask0 = 2;
	char disable_ch_1_3 = 0;

	//char sub_ch_mask1 = 2;
	if (disable_ch_1_3)
		sub_ch_mask0 = 4;
	//sub_ch_mask1 = 4;
	//phy_base_add;//p_dev->p_ddr_fw_inter_message->p_ddr_common_message
	//    ->ddr_common_reg_base_add_p->aml_phy_base_add;
	uint32_t return_value = REGISTER_READ_SKIP_FLAG;

	if (!index)
		return return_value;

	//ac group0 then ac group1
	if (index == DMC_TEST_WINDOW_INDEX_ATXDLY || index == DMC_TEST_WINDOW_INDEX_ATXDLY_RO) {
		reg_add_coarse = (add_base + ((ps << 16) | (0x9 << 12) |
					      (0x0 + ((sub_index / 16) << 2))));
		reg_add_fine = (add_base + ((ps << 16) | (0x9 << 12) |
					    (0x8 + ((sub_index / 4) << 2))));
		if (index == DMC_TEST_WINDOW_INDEX_ATXDLY_RO) {
			reg_add_coarse = (add_base + ((ps << 16) | (0x1 << 12) |
						      (((sub_index / 16) << 13) & 0xffff) |
						      (0x470 + 0)));
			reg_add_fine = (add_base + ((ps << 16) | (0x1 << 12) |
						    (((sub_index / 16) << 13) & 0xffff) |
						    (0x474 + ((sub_index / 4) << 2))));
		}
		reg_add_coarse_bit_mask = (~(3 << ((sub_index % 16) * 2)));
		reg_add_fine_bit_mask = (~(0x7f << ((sub_index % 4) * 8)));
	} else if (index == DMC_TEST_WINDOW_INDEX_TXDQSDLY ||
		   index == DMC_TEST_WINDOW_INDEX_TXDQSDLY_RO) {
		reg_add_coarse = (add_base +
				  ((ps << 16) |
				   ((0xa + (sub_index % DWC_TOTAL_DATA_NIBBLE_PER_PHY)) << 12) |
				   (0x40 + (sub_index / DWC_TOTAL_DATA_NIBBLE_PER_PHY) *
				    (0x90 - 0x40))));
		reg_add_fine = (add_base +
				((ps << 16) |
				 ((0xa + (sub_index % DWC_TOTAL_DATA_NIBBLE_PER_PHY)) << 12) |
				 (0x40 + (sub_index / DWC_TOTAL_DATA_NIBBLE_PER_PHY) *
				  (0x90 - 0x40))));
		if (index == DMC_TEST_WINDOW_INDEX_TXDQSDLY_RO) {
			{
				reg_add_coarse =
					(add_base + ((ps << 16) |
						     (0x1 << 12) |
						     (((sub_index % 4) / sub_ch_mask0) << 13) |
						     (0x540 + (((sub_index % 4) % sub_ch_mask0) *
							       (0x100)) +
						      ((sub_index / 4) * (0x50)))));
				reg_add_fine =
					(add_base + ((ps << 16) |
						     (0x1 << 12) |
						     (((sub_index % 4) / sub_ch_mask0) << 13) |
						     (0x540 + (((sub_index % 4) % sub_ch_mask0) *
							       (0x100)) +
						      ((sub_index / 4) * (0x50)))));
			}
		}

		reg_add_coarse_bit_mask = (~(7 << 0));
		reg_add_fine_bit_mask = (~(0x7f << 8));
	} else if (index == DMC_TEST_WINDOW_INDEX_RXCLKDLY ||
		   index == DMC_TEST_WINDOW_INDEX_RXCLKDLY_RO) {
		reg_add_coarse = 0;
		reg_add_fine = (add_base +
				((ps << 16) |
				 ((0xa + sub_index % DWC_TOTAL_DATA_NIBBLE_PER_PHY) << 12) |
				 (0x1c + (sub_index / DWC_TOTAL_DATA_NIBBLE_PER_PHY) *
				  (0x6c - 0x1c))));
		reg_add_coarse_bit_mask = 0;
		reg_add_fine_bit_mask = (~(0xff << 0));
		if (index == DMC_TEST_WINDOW_INDEX_RXCLKDLY_RO) {
			reg_add_coarse = 0;
			reg_add_fine = (add_base +
					((ps << 16) | (0x1 << 12) | (((sub_index % 4) /
								      sub_ch_mask0) << 13) |
					 (0x51c + (((sub_index % 4) % sub_ch_mask0) * (0x100)) +
					  ((sub_index / 4) * (0x50)))));
		}
	}
#if PSEUDO_DISABLE
	else if (index == DMC_TEST_WINDOW_INDEX_RXDQDBI ||
		 index == DMC_TEST_WINDOW_INDEX_RXDQDBI_RO) {
		reg_add_coarse = 0;
		reg_add_fine = (add_base +
				((ps << 16) |
				 ((0xa + sub_index % DWC_TOTAL_DATA_NIBBLE_PER_PHY) << 12) |
				 (0x18 + (sub_index / DWC_TOTAL_DATA_NIBBLE_PER_PHY) *
				  (0x68 - 0x18))));
		reg_add_coarse_bit_mask = 0;
		reg_add_fine_bit_mask = (~(0xff << 0));
		if (index == DMC_TEST_WINDOW_INDEX_RXDQDBI_RO) {
			reg_add_coarse = 0;
			reg_add_fine = (add_base +
					((ps << 16) | (0x1 << 12) | (((sub_index % 4) /
								      sub_ch_mask0) << 13) |
					 (0x518 + (((sub_index % 4) % sub_ch_mask0) * (0x100)) +
					  ((sub_index / 4) * (0x50)))));
		}
	} else if (index == DMC_TEST_WINDOW_INDEX_RXDQDBI_WINDOW ||
		   index == DMC_TEST_WINDOW_INDEX_RXDQDBI_WINDOW_RO) {
		reg_add_coarse_bit_mask = 0;
		reg_add_fine_bit_mask = (~(0xff << 0));
		reg_add_coarse = 0;
		reg_add_fine = (add_base +
				((ps << 16) | (0x1 << 12) | (((sub_index % 4) /
							      sub_ch_mask0) << 13) |
				 (0x9d4 + (((sub_index % 4) % sub_ch_mask0) * (0x100)) +
				  ((sub_index / 4) * (0xa54 - 0x9d4)))));
	} else if (index == DMC_TEST_WINDOW_INDEX_WDQDBI ||
		   index == DMC_TEST_WINDOW_INDEX_WDQDBI_RO) {
		reg_add_coarse = (add_base +
				  ((ps << 16) |
				   ((0xa + sub_index % DWC_TOTAL_DATA_NIBBLE_PER_PHY) << 12) |
				   (0x028 + (sub_index / DWC_TOTAL_DATA_NIBBLE_PER_PHY) *
				    (0x078 - 0x028))));
		reg_add_fine = (add_base +
				((ps << 16) |
				 ((0xa + sub_index % DWC_TOTAL_DATA_NIBBLE_PER_PHY) << 12) |
				 (0x038 + (sub_index / DWC_TOTAL_DATA_NIBBLE_PER_PHY) *
				  (0x088 - 0x038))));
		reg_add_coarse_bit_mask = 0x7;
		reg_add_fine_bit_mask = (~(0xff << 0));
		if (index == DMC_TEST_WINDOW_INDEX_WDQDBI_RO) {
			reg_add_coarse = (add_base +
					  ((ps << 16) | (0x1 << 12) | (((sub_index % 4) /
									sub_ch_mask0) << 13) |
					   (0x528 + (((sub_index % 4) % sub_ch_mask0) * (0x100)) +
					    ((sub_index / 4) * (0x578 - 0x528)))));
			reg_add_fine = (add_base +
					((ps << 16) | (0x1 << 12) | (((sub_index % 4) /
								      sub_ch_mask0) << 13) |
					 (0x538 + (((sub_index % 4) % sub_ch_mask0) * (0x100)) +
					  ((sub_index / 4) * (0x588 - 0x538)))));
		}
	} else if (index == DMC_TEST_WINDOW_INDEX_WDQDBI_WINDOW ||
		   index == DMC_TEST_WINDOW_INDEX_WDQDBI_WINDOW_RO) {
		reg_add_coarse_bit_mask = 0;
		reg_add_fine_bit_mask = (~(0xff << 0));
		reg_add_coarse = 0;
		reg_add_fine = (add_base +
				((ps << 16) | (0x1 << 12) | (((sub_index % 4) /
							      sub_ch_mask0) << 13) |
				 (0x9d0 + (((sub_index % 4) % sub_ch_mask0) * (0x100)) +
				  ((sub_index / 4) * (0xa50 - 0x9d0)))));
	} else if (index == DMC_TEST_WINDOW_INDEX_RXDQDBI_VREF0 ||
		   index == DMC_TEST_WINDOW_INDEX_RXDQDBI_VREF0_RO) {
		reg_add_coarse = 0;
		reg_add_fine = (add_base +
				((ps << 16) |
				 ((0xa + sub_index % DWC_TOTAL_DATA_NIBBLE_PER_PHY) << 12) |
				 (0xa8 + (sub_index / DWC_TOTAL_DATA_NIBBLE_PER_PHY) *
				  (0xa8 - 0xa8))));
		reg_add_coarse_bit_mask = 0;
		reg_add_fine_bit_mask = (~(0xff << 0));
		if (index == DMC_TEST_WINDOW_INDEX_RXDQDBI_RO) {
			reg_add_coarse = 0;
			reg_add_fine = (add_base +
					((ps << 16) | (0x1 << 12) | (((sub_index % 4) /
								      sub_ch_mask0) << 13) |
					 (0x5a8 + (((sub_index % 4) % sub_ch_mask0) * (0x100)) +
					  ((sub_index / 4) * (0x0)))));
		}
	} else if (index == DMC_TEST_WINDOW_INDEX_RXDQDBI_VREF1 ||
		   index == DMC_TEST_WINDOW_INDEX_RXDQDBI_VREF1_RO) {
		reg_add_coarse = 0;
		reg_add_fine = (add_base +
				((ps << 16) |
				 ((0xa + sub_index % DWC_TOTAL_DATA_NIBBLE_PER_PHY) << 12) |
				 (0xb8 + (sub_index / DWC_TOTAL_DATA_NIBBLE_PER_PHY) *
				  (0xb8 - 0xb8))));
		reg_add_coarse_bit_mask = 0;
		reg_add_fine_bit_mask = (~(0xff << 0));
		if (index == DMC_TEST_WINDOW_INDEX_RXDQDBI_RO) {
			reg_add_coarse = 0;
			reg_add_fine = (add_base +
					((ps << 16) | (0x1 << 12) | (((sub_index % 4) /
								      sub_ch_mask0) << 13) |
					 (0x5b8 + (((sub_index % 4) % sub_ch_mask0) * (0x100)) +
					  ((sub_index / 4) * (0x0)))));
		}
	}
#endif
	else if (index == DMC_TEST_WINDOW_INDEX_TXDQDLY ||
		 index == DMC_TEST_WINDOW_INDEX_TXDQDLY_RO) {
		reg_add_coarse = (add_base + ((ps << 16) | ((0xa + (sub_index % 36) / 9) << 12) |
					      (0x20 + (sub_index / 36) * (0x70 - 0x20))) +
				  (((sub_index % 9) / 4) * 4));
		reg_add_fine = (add_base + ((ps << 16) | ((0xa + (sub_index % 36) / 9) << 12) |
					    (0x30 + (sub_index / 36) * (0x80 - 0x30))) +
				(((sub_index % 9) / 4) * 4));
		reg_add_coarse_bit_mask = (~(0x7 << (((sub_index % 9) % 4) * 4)));
		reg_add_fine_bit_mask = (~(0x7f << (((sub_index % 9) % 4) * 8)));
		if (index == DMC_TEST_WINDOW_INDEX_TXDQDLY_RO) {
			reg_add_coarse = (add_base +
					  ((ps << 16) | (0x1 << 12) |
					   (((sub_index % 36) / (9 * sub_ch_mask0)) << 13) |
					   (0x520 + (((sub_index % (9 * sub_ch_mask0)) / 9) *
						     (0x100)) + ((sub_index / 36) * (0x50)) +
					    (((sub_index % 9) / 4) * 4))));
			reg_add_fine = (add_base +
					((ps << 16) |
					 (0x1 << 12) |
					 (((sub_index % 36) / (9 * sub_ch_mask0)) << 13) |
					 (0x530 + (((sub_index % (9 * sub_ch_mask0)) / 9) *
						   (0x100)) + ((sub_index / 36) * (0x50)) +
					  (((sub_index % 9) / 4) * 4))));
		}
	} else if (index == DMC_TEST_WINDOW_INDEX_TXDQDLY_WINDOW ||
		   index == DMC_TEST_WINDOW_INDEX_TXDQDLY_WINDOW_RO) {
		reg_add_coarse = 0;
		reg_add_fine = (add_base +
				((ps << 16) | (0x1 << 12) |
				 (((sub_index % 36) / (9 * sub_ch_mask0)) << 13) |
				 (0x9b0 + (((sub_index % (9 * sub_ch_mask0)) / 9) * (0x8)) +
				  ((sub_index / 36) * (0xa30 - 0x9b0)) +
				  (((sub_index % 9) / 4) * 4))));
		reg_add_coarse_bit_mask = 0;
		reg_add_fine_bit_mask = (~(0xff << (((sub_index % 9) % 4) * 8)));
	} else if (index == DMC_TEST_WINDOW_INDEX_RXDQDLY_WINDOW ||
		   index == DMC_TEST_WINDOW_INDEX_RXDQDLY_WINDOW_RO) {
		reg_add_coarse = 0;
		reg_add_fine = (add_base +
				((ps << 16) | (0x1 << 12) |
				 (((sub_index % 36) / (9 * sub_ch_mask0)) << 13) |
				 (0x990 + (((sub_index % (9 * sub_ch_mask0)) / 9) * (0x8)) +
				  ((sub_index / 36) * (0xa10 - 0x990)) +
				  (((sub_index % 9) / 4) * 4))));
		reg_add_coarse_bit_mask = 0;
		reg_add_fine_bit_mask = (~(0xff << (((sub_index % 9) % 4) * 8)));
	} else if (index == DMC_TEST_WINDOW_INDEX_RXPBDLY ||
		   index == DMC_TEST_WINDOW_INDEX_RXPBDLY_RO) {
		reg_add_coarse = 0;
		reg_add_fine = (add_base + ((ps << 16) | ((0xa + (sub_index % 36) / 9) << 12) |
					    (0x10 + (sub_index / 36) * (0x60 - 0x10))) +
				(((sub_index % 9) / 4) * 4));
		if (index == DMC_TEST_WINDOW_INDEX_RXPBDLY_RO) {
			reg_add_coarse = 0;
			reg_add_fine = (add_base +
					((ps << 16) | (0x1 << 12) |
					 (((sub_index % 36) / (9 * sub_ch_mask0)) << 13) |
					 (0x510 + (((sub_index % (9 * sub_ch_mask0)) / 9) *
						   (0x100)) + (sub_index / 36) * (0x50) +
					  (((sub_index % 9) / 4) * 4))));
		}
		reg_add_coarse_bit_mask = 0;
		reg_add_fine_bit_mask = (~(0xff << (((sub_index % 9) % 4) * 8)));
		//ddr_debug_serial_puts("DDR_debug", __FILE__, __LINE__);
	} else if (index == DMC_TEST_WINDOW_INDEX_RXENDLY ||
		   index == DMC_TEST_WINDOW_INDEX_RXENDLY_RO) {
		reg_add_coarse = (add_base +
				  ((ps << 16) |
				   ((0xa + sub_index % DWC_TOTAL_DATA_NIBBLE_PER_PHY) << 12) |
				   (0x0 + (sub_index / DWC_TOTAL_DATA_NIBBLE_PER_PHY) *
				    (0x50 - 0x0))));
		reg_add_fine = (add_base +
				((ps << 16) |
				 ((0xa + sub_index % DWC_TOTAL_DATA_NIBBLE_PER_PHY) << 12) |
				 (0x0 + (sub_index / DWC_TOTAL_DATA_NIBBLE_PER_PHY) *
				  (0x50 - 0x0))));
		if (index == DMC_TEST_WINDOW_INDEX_RXENDLY_RO) {
			reg_add_coarse = (add_base +
					  ((ps << 16) |
					   (0x1 << 12) | (((sub_index % 4) /
							   (1 * sub_ch_mask0)) << 13) |
					   (0x500 + (((sub_index % 4) % (1 * sub_ch_mask0)) *
						     (0x100)) +
					    ((sub_index / 4) * (0x50)))));
			reg_add_fine = (add_base +
					((ps << 16) | (0x1 << 12) | (((sub_index % 4) /
								      (1 * sub_ch_mask0)) << 13) |
					 (0x500 + (((sub_index % 4) % (1 * sub_ch_mask0)) *
						   (0x100)) + ((sub_index / 4) * (0x50)))));
		}
		reg_add_coarse_bit_mask = (~(0x1f << 0));
		reg_add_fine_bit_mask = (~(0x7f << 8));
	} else if (index ==
		   DMC_TEST_WINDOW_INDEX_RXEN_WINDOW_DLY || index ==
		   DMC_TEST_WINDOW_INDEX_RXEN_WINDOW_DLY_RO) {
		if (index == DMC_TEST_WINDOW_INDEX_RXEN_WINDOW_DLY_RO) {
			reg_add_coarse = 0;
			reg_add_fine = (add_base +
					((ps << 16) | (0x1 << 12) |
					 (((sub_index % 4) / (1 * sub_ch_mask0)) << 13) |
					 (0x980 + ((((sub_index % 4) %
						     (1 * sub_ch_mask0)) >> 1) << 2) +
					  ((sub_index / 4) * (0x80)))));
		}
		reg_add_coarse_bit_mask = 0; // (~(0x1f << 0));
		reg_add_fine_bit_mask = (~(0xfff << (((sub_index % 2)) * 16)));
	} else if (index == WINDOW_TEST_SOC_VREF_DAC0 ||
		   index == WINDOW_TEST_SOC_VREF_DAC0_RO) {
		if (sub_index < 36) {
			//vref dq and dbi
			reg_add_coarse = 0;
			reg_add_fine =
				(add_base + ((ps << 16) |
					     ((0xa + (sub_index % 36) / 9) << 12) | 0xa0) +
				 (((sub_index % 9) / 4) * 4));
			if (index == WINDOW_TEST_SOC_VREF_DAC0_RO) {
				reg_add_coarse = 0;
				reg_add_fine =
					(add_base +
					 ((ps << 16) | (0x1 << 12) |
					  (((sub_index % 36) / (9 * sub_ch_mask0)) << 13) |
					  (0x5a0 + (((sub_index % (9 * sub_ch_mask0)) / 9) *
						    (0x100)) + ((sub_index / 36) *
								(0x50)) +
					   (((sub_index % 9) / 4) * 4))));
			}
			reg_add_coarse_bit_mask = 0;
			reg_add_fine_bit_mask = (~(0x7f << (((sub_index % 9) % 4) * 8)));
		} else if (sub_index < 44) {
			//vref dqs and dqsn
			reg_add_coarse = 0;
			reg_add_fine = (add_base +
					((ps << 16) | ((0xa + ((sub_index - 36) /
							       2)) << 12) | 0xc0));
			reg_add_coarse_bit_mask = 0;
			reg_add_fine_bit_mask = (~(0x7f << (((sub_index - 36) % 2) * 8)));
		}
	} else if (index ==
		   WINDOW_TEST_SOC_VREF_DAC1 ||
		   index == WINDOW_TEST_SOC_VREF_DAC1_RO) {
		//vref dq and dbi
		reg_add_coarse = 0;
		reg_add_fine = add_base +
			       ((((ps << 16) | ((0xa + (sub_index % 36) /
						 9) << 12) | 0xb0) + (((sub_index % 9) / 4) * 4)));

		if (index == WINDOW_TEST_SOC_VREF_DAC1_RO) {
			reg_add_coarse = 0;
			reg_add_fine =
				(add_base + ((ps << 16) | (0x1 << 12) |
					     (((sub_index % 36) / (9 * sub_ch_mask0)) << 13) |
					     (0x5b0 + (((sub_index % (9 * sub_ch_mask0)) / 9) *
						       (0x100)) + (0) +
					      (((sub_index % 9) / 4) * 4))));
		}
		reg_add_coarse_bit_mask = 0;
		reg_add_fine_bit_mask = (~(0x7f << (((sub_index % 9) % 4) * 8)));
	} else if (index == DMC_TEST_WINDOW_INDEX_DRAM_VREF || index ==
		   DMC_TEST_WINDOW_INDEX_DRAM_VREF_RO) {
		if (sub_index < 36) {
			//DDR_X32_F0_AD30	DDR_X32_F0_A930
			reg_add_coarse = 0;
			reg_add_fine = 0;
			if (index == DMC_TEST_WINDOW_INDEX_DRAM_VREF_RO) {
				reg_add_coarse = 0;
				reg_add_fine =
					(add_base +
					 ((ps << 16) |
					  (0x1 << 12) |
					  (((sub_index % 36) / (9 * sub_ch_mask0)) << 13) |
					  (0xa60 + (((sub_index % (9 * sub_ch_mask0)) /
						     9) * (8)) +
					   (0) + (((sub_index % 9) / 4) * 4))));
			}
			reg_add_coarse_bit_mask = 0;
			reg_add_fine_bit_mask = (~(0x7f << (((sub_index % 9) % 4) * 8)));
		}
	}

	if (reg_add_coarse)
		delay_reg_coarse_value = (((rd_reg_ddr_phy(phy_base_add,
							   reg_add_coarse)) &
					   (~reg_add_coarse_bit_mask))
					  >> (ddr_mask_convert_offset(reg_add_coarse_bit_mask)));

	if (reg_add_fine)
		delay_reg_fine_value = (((rd_reg_ddr_phy(phy_base_add, reg_add_fine)) &
					 (~reg_add_fine_bit_mask))
					>> (ddr_mask_convert_offset(reg_add_fine_bit_mask)));
	delay_old_value = ((delay_reg_coarse_value << 16) | delay_reg_fine_value);
	delay_old_value = ddr_cacl_phy_delay_all_step(index, delay_old_value);

	if (read_write_enable == REGISTER_READ) {
		read_write_value = delay_old_value;

		if (reg_add_coarse == 0 && reg_add_fine == 0)
			return return_value;
	} else if (read_write_enable == REGISTER_WRITE) {
		delay_new_value = read_write_value;
		delay_new_value = ddr_cacl_phy_over_ride_back_reg(index, delay_new_value);

		if (reg_add_coarse) {
			wr_reg_ddr_phy(phy_base_add,
				       reg_add_coarse,
				       ((rd_reg_ddr_phy(phy_base_add, reg_add_coarse)) &
					(reg_add_coarse_bit_mask)) |
				       ((delay_new_value >> 16) <<
					(ddr_mask_convert_offset(reg_add_coarse_bit_mask))));
		}

		if (reg_add_fine) {
			wr_reg_ddr_phy(phy_base_add, reg_add_fine,
				       ((rd_reg_ddr_phy(phy_base_add, reg_add_fine)) &
					(reg_add_fine_bit_mask))
				       | ((delay_new_value & 0xffff) <<
					  (ddr_mask_convert_offset(reg_add_fine_bit_mask))));
		}
		//should >1 refresh interleave time for controller update
	}
	printf("delay_old_value,%8x,value,%8x,index,%8x,sub_index,%8x,reg_c,%8x,reg_fine,%8x\n",
	       delay_old_value, read_write_value, index, sub_index, (uint32_t)reg_add_coarse,
	       (uint32_t)reg_add_fine);
	return read_write_value;
}

void ddr_read_write_training_value(ddr_set_ps0_only_t *p_ddrs, char over_ride_index,
				   char read_write, uint32_t ps, void *input, char print)
{
	//	ddr_debug_serial_puts("DDR_debug", __FILE__, __LINE__);
	uint16_t t_count = 0;
	char count_max = 72;
	uint16_t t_count_value = 0;
	uint16_t delay_temp = 0;
	char *input_uint8_p = input;
	uint16_t *input_uint16_p = input;
	char p_size = 1;
	char read_skip = 0;
	char over_ride_index_t = (over_ride_index % 0x20);

//ddr_log_info(LOG_CHL_0, "efuse sipinfo[0]=%x\n", sipinfo[0]);
	for (t_count = 0; t_count < count_max; t_count++) {
		if (over_ride_index >= DMC_TEST_WINDOW_INDEX_ATXDLY &&
		    over_ride_index <= DMC_TEST_WINDOW_INDEX_RXDQDBI_VREF1_RO) {
			if (over_ride_index_t == DMC_TEST_WINDOW_INDEX_ATXDLY) {
				p_size = 2;
				count_max = 30;
			} else if (over_ride_index_t == DMC_TEST_WINDOW_INDEX_TXDQSDLY) {
				p_size = 2;
				count_max = 8;
			} else if (over_ride_index_t == DMC_TEST_WINDOW_INDEX_RXPBDLY) {
				p_size = 2;
				count_max = 72;
			} else if (over_ride_index_t == DMC_TEST_WINDOW_INDEX_RXENDLY) {
				p_size = 2;
				count_max = 8;
			} else if (over_ride_index_t == DMC_TEST_WINDOW_INDEX_RXCLKDLY) {
				p_size = 2;
				count_max = 8;
			} else if (over_ride_index_t == DMC_TEST_WINDOW_INDEX_TXDQDLY) {
				p_size = 2;
				count_max = 72;
			} else if (over_ride_index_t == DMC_TEST_WINDOW_INDEX_TXDQDLY_WINDOW ||
				   over_ride_index_t == DMC_TEST_WINDOW_INDEX_RXDQDLY_WINDOW) {
				p_size = 1;
				count_max = 72;
			} else if (over_ride_index_t == WINDOW_TEST_SOC_VREF_DAC0) {
				p_size = 1;
				count_max = 36; // 44;
			} else if (over_ride_index_t == WINDOW_TEST_SOC_VREF_DAC1) {
				p_size = 1;
				count_max = 36;
			} else if (over_ride_index_t == DMC_TEST_WINDOW_INDEX_RXEN_WINDOW_DLY) {
				p_size = 2;
				count_max = 8;
			} else if (over_ride_index_t == DMC_TEST_WINDOW_INDEX_DRAM_VREF) {
				p_size = 1;
				count_max = 36;
			} else if ((over_ride_index_t >= DMC_TEST_WINDOW_INDEX_RXDQDBI) &&
				   (over_ride_index_t <= DMC_TEST_WINDOW_INDEX_WDQDBI_WINDOW)) {
				p_size = 2;
				count_max = 72; //8;
			} else if ((over_ride_index_t >= DMC_TEST_WINDOW_INDEX_RXDQDBI_VREF0) &&
				   (over_ride_index_t <= DMC_TEST_WINDOW_INDEX_RXDQDBI_VREF1)) {
				p_size = 1;
				count_max = 36; //8;
			}
			if (read_write == REGISTER_READ) {
				read_skip = 0;

				if (ps == 3 && (over_ride_index_t ==
						WINDOW_TEST_SOC_VREF_DAC0 ||
						over_ride_index_t ==
						WINDOW_TEST_SOC_VREF_DAC1)) {
					if (t_count > 35)
						read_skip = 1;
					else if ((t_count % 9) == 8)
						read_skip = 1;
				}
				if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXDQDBI_RO) {
					over_ride_index_t = DMC_TEST_WINDOW_INDEX_RXPBDLY_RO;
					if ((t_count % 9) == 8) {
						t_count_value =
							ddr_training_reg_rw(p_ddrs,
									    over_ride_index_t,
									    t_count,
									    t_count_value,
									    read_write, ps);
					} else {
						read_skip = 1;
					}
				} else if (over_ride_index ==
					   DMC_TEST_WINDOW_INDEX_RXDQDBI_VREF0_RO) {
					over_ride_index_t = WINDOW_TEST_SOC_VREF_DAC0_RO;
					if ((t_count % 9) == 8) {
						t_count_value =
							ddr_training_reg_rw(p_ddrs,
									    over_ride_index_t,
									    t_count,
									    t_count_value,
									    read_write, ps);
					} else {
						read_skip = 1;
					}
				} else if (over_ride_index ==
					   DMC_TEST_WINDOW_INDEX_RXDQDBI_VREF1_RO) {
					over_ride_index_t = WINDOW_TEST_SOC_VREF_DAC1_RO;
					if ((t_count % 9) == 8) {
						t_count_value =
							ddr_training_reg_rw(p_ddrs,
									    over_ride_index_t,
									    t_count,
									    t_count_value,
									    read_write, ps);
					} else {
						read_skip = 1;
					}
				} else if (over_ride_index == DMC_TEST_WINDOW_INDEX_WDQDBI) {
					over_ride_index_t = DMC_TEST_WINDOW_INDEX_TXDQDLY_RO;
					if ((t_count % 9) == 8) {
						t_count_value =
							ddr_training_reg_rw(p_ddrs,
									    over_ride_index_t,
									    t_count,
									    t_count_value,
									    read_write, ps);
					} else {
						read_skip = 1;
					}
				} else {
					t_count_value =
						ddr_training_reg_rw(p_ddrs,
								    over_ride_index,
								    t_count,
								    t_count_value,
								    read_write, ps);
				}
				delay_temp = t_count_value;

				if (delay_temp == REGISTER_READ_SKIP_FLAG)
					read_skip = 1;

				if (read_skip) {
				} else {
					if (p_size == 1)
						(*input_uint8_p) = delay_temp;

					if (p_size == 2)
						(*input_uint16_p) = delay_temp;
				}
			} else {
				if (p_size == 1)
					delay_temp = (*input_uint8_p);

				if (p_size == 2)
					delay_temp = (*input_uint16_p);
				t_count_value = delay_temp;
				ddr_training_reg_rw(p_ddrs, over_ride_index, t_count,
						    t_count_value, read_write, ps);
			}

			if (p_size == 1)
				input_uint8_p++;

			if (p_size == 2)
				input_uint16_p++;

			if (print == DDR_PRINT_ENABLE)
				printf("index %d sub_index %d phase %d\n",
				       over_ride_index, t_count, t_count_value);
		}
	}
}

uint32_t ddr_get_s5_bdlr_100step_min(void)
{
	uint32_t bdlr_100step = 0;

	bdlr_100step = get_bdlr_100step(global_ddr_clk);
	return bdlr_100step;
}

uint32_t ddr_get_s5_bdlr_100step_max(void)
{
	uint32_t bdlr_100step = 0;

	bdlr_100step = get_bdlr_100step(global_ddr_clk);
	return bdlr_100step;
}

uint32_t ddr_get_s5_bdlr_100step(void)
{
	uint32_t bdlr_100step = 0;

	bdlr_100step = get_bdlr_100step(global_ddr_clk);
	return bdlr_100step;
}

uint32_t ddr_get_s5_bdlr_100step_cur(void)
{
	uint32_t bdlr_100step = 0;

	bdlr_100step = get_bdlr_100step(global_ddr_clk);
	return bdlr_100step;
}

uint32_t ddr_get_ui_1_128_100step(void)
{
	return (1000000 * 100 / (2 * 128)) / ((global_ddr_clk));
}

uint32_t do_read_s5_ddr_bdlr_steps(void)
{
	uint32_t DRAMFreq = 0;

	DRAMFreq = get_ddr_clk();
	printf("\ns5_chip_DRAMFreq MHz,100min_bdlr ps,100max_bdlr ps ");
	printf("%d MHz,%d ps,%d ps,ave_100_bdlr=%d ps,bdlr_var=%d thousand,cur_100_bdlr=%d ps\n",
	       DRAMFreq, ddr_get_s5_bdlr_100step_min(), ddr_get_s5_bdlr_100step_max(),
	       ddr_get_s5_bdlr_100step(),
	       2000 * (ddr_get_s5_bdlr_100step_max() - ddr_get_s5_bdlr_100step_min()) /
	       (ddr_get_s5_bdlr_100step_max() + ddr_get_s5_bdlr_100step_min()),
	       ddr_get_s5_bdlr_100step_cur());

	return ddr_get_s5_bdlr_100step();
}

void ddr_read_write_training_all_delay_value(ddr_set_ps0_only_t *p_ddrs,
					     char read_write, char print)
{
	uint32_t ps = 0;
	board_phase_setting_ps_t *board_phase_setting_p;

	board_phase_setting_p = &p_ddrs->cfg_ddr_training_delay_ps;

	//for (ps = 0; ps < 2;)
	//{
	ddr_read_write_training_value(p_ddrs,
				      DMC_TEST_WINDOW_INDEX_ATXDLY, read_write, ps,
				      &board_phase_setting_p->ac_trace_delay[0], print);

	ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_TXDQSDLY, read_write, ps,
				      &board_phase_setting_p->write_dqs_delay[0], print);
	//ddr_read_write_training_value(p_ddrs,
	//DMC_TEST_WINDOW_INDEX_WCK_CK, read_write, ps,
	//	&(board_phase_setting_p->write_wck_delay, print);
	ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_RX_CLK_T_DLY, read_write, ps,
				      &board_phase_setting_p->read_dq_delay_t[0], print);
	//ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_RX_CLK_C_DLY, read_write, ps,
	//	&(board_phase_setting_p->read_dq_delay_c, print);
	ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_RXCLKDLY, read_write, ps,
				      &board_phase_setting_p->read_dqs_delay[0], print);
	ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_TXDQDLY, read_write, ps,
				      &board_phase_setting_p->wdq_delay[0], print);
	ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_RXENDLY, read_write, ps,
				      &board_phase_setting_p->read_dqs_gate_delay[0], print);
	ddr_read_write_training_value(p_ddrs, WINDOW_TEST_SOC_VREF_DAC0, read_write, ps,
				      &board_phase_setting_p->soc_bit_vref0[0], print);
	ddr_read_write_training_value(p_ddrs, WINDOW_TEST_SOC_VREF_DAC1, read_write, ps,
				      &board_phase_setting_p->soc_bit_vref1[0], print);
	//ddr_read_write_training_value(p_ddrs,
	// DMC_TEST_SOC_VREF_DFE_DAC2, read_write, ps,
	//	&(board_phase_setting_p->soc_bit_vref2, print);
	//ddr_read_write_training_value(p_ddrs, DMC_TEST_SOC_VREF_DFE_DAC3,
	// read_write, ps,&(board_phase_setting_p->soc_bit_vref3, print);
	//	ps = ps + 1;
	//}

	//ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_EXTERA_COMMON, read_write, 0,
	// 0, print);
}

void ddr_read_write_training_all_vref_value(ddr_set_ps0_only_t *p_ddrs,
					    char read_write, char print)
{
	uint32_t ps;

	ps = 0;
	ddr_read_write_training_value(p_ddrs,
				      WINDOW_TEST_SOC_VREF_DAC0, read_write, ps,
				      &p_ddrs->cfg_ddr_training_delay_ps.soc_bit_vref0,
				      print);
	ddr_read_write_training_value(p_ddrs,
				      WINDOW_TEST_SOC_VREF_DAC1, read_write, ps,
				      &p_ddrs->cfg_ddr_training_delay_ps.soc_bit_vref1,
				      print);
	ddr_read_write_training_value(p_ddrs,
				      WINDOW_TEST_SOC_VREF_DAC2, read_write, ps,
				      &p_ddrs->cfg_ddr_training_delay_ps.soc_bit_vref2,
				      print);
	ddr_read_write_training_value(p_ddrs,
				      WINDOW_TEST_SOC_VREF_DAC3, read_write, ps,
				      &p_ddrs->cfg_ddr_training_delay_ps.soc_bit_vref3,
				      print);
}

int do_read_s5_ddr_training_data(char log_level, ddr_set_ps0_only_t *ddr_set_t_p)
{
	//uint32_t stick_store_sticky_f0_reg_base_t = (phy_base_add + 0x0128);
	//uint32_t stick_store_sticky_f1_reg_base_t = (phy_base_add + 0x1128);

	printf_log(log_level, "\nddr_set_t_p=0x%8x\n", (uint32_t)(uint64_t)(ddr_set_t_p));
	uint32_t loop = 0;
	uint32_t loop_max = (4 + (0x3f << 2)); //((DMC_STICKY_63-DMC_STICKY_0));

	for (loop = 0; loop < loop_max; loop += 4)
		wr_reg(((uint64_t)(ddr_set_t_p) + loop),
		       rd_reg((p_ddr_base->ddr_dmc_sticky0) + loop));
	loop_max = sizeof(board_SI_setting_ps_t);
	printf_log(log_level, "\nsizeof(board_SI_setting_ps_t)=0x%8x\n", loop_max);
	//for (loop = 0; loop < loop_max; loop += 4)
	//	wr_reg(((uint64_t)(&ddr_set_t_p->cfg_board_SI_setting_ps) + loop),
	//	       rd_reg((stick_store_sticky_f0_reg_base_t + loop)));
	//for (loop = 0; loop < loop_max; loop += 4)
	//	wr_reg(((uint64_t)(&ddr_set_t_p->cfg_board_SI_setting_ps) + loop),
	//	       rd_reg((stick_store_sticky_f1_reg_base_t + loop)));
	for (loop = 0; loop < MESON_CPU_CHIP_ID_SIZE; loop++) //update chip id

		ddr_sha_s5.sha_chip_id[loop] = global_chip_id[loop];
	{
		bdlr_100step = get_bdlr_100step(global_ddr_clk);
		ui_1_32_100step = (1000000 * 100 / (global_ddr_clk * 2 * 32));
		ddr_read_write_training_all_delay_value(ddr_set_t_p, REGISTER_READ, ~log_level);
	}
	return 1;
}

int do_ddr_display_s5_ddr_information(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int i = 0;

	//unsigned int ps = 0;

	printf("\nargc== 0x%8x\n", argc);
	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);

	do_read_s5_ddr_training_data(0, ddr_set_t_p);

	{
		uint32_t count = 0;
		uint32_t reg_add_offset = 0;

		printf("\n PCTL timming: 0x");

		for (count = 0; count < ((p_ddr_base->ddr_pctl_timing_end_address) -
					 (p_ddr_base->ddr_pctl_timing_base_address));) {
			reg_add_offset = ((p_ddr_base->ddr_pctl_timing_base_address) + (count));
			printf("\n reg_add_offset: %8x %8x %8x ", reg_add_offset,
			       readl(reg_add_offset), reg_add_offset);
			count = count + 4;
		}
		printf("\n mrs register: ");
		printf("\n mrs register: base +DDR_PHY_BASE,%8x  byte offset\n", (phy_base_add));

		printf("\n sticky register: ");
		{
			uint32_t loop_max = 0;

			loop_max = 64 << 2; //sizeof(ddr_set_t);
			for (count = 0; count < loop_max; count += 4)
				printf("\n reg_add_offset: %8x %8x %8x", count,
				       rd_reg((uint64_t)((p_ddr_base->ddr_dmc_sticky0)) + count),
				       (((p_ddr_base->ddr_dmc_sticky0)) + count));
		}

		{
			uint32_t loop_max = 0;

			loop_max = sizeof(ddr_set_ps0_only_t);
			uint32_t count = 0;

			for (count = 0; count < loop_max; ) {
				printf("\n%8x %8x", count,
				       rd_reg((uint64_t)(ddr_set_t_p) + count));
				count = count + 4;
			}
		}
	}

	printf("\n {");

	uint32_t count = 0;

	//board_common_setting_t *board_common_p;
	//board_common_p = ddr_set_t_p->cfg_board_common_setting;
	//{
	printf("\n//old fast_boot[%d]=0x%8x,// %d", 0,
	       ddr_set_t_p->cfg_board_common_setting.fast_boot[0],
	       ddr_set_t_p->cfg_board_common_setting.fast_boot[0]);
	ddr_set_t_p->cfg_board_common_setting.fast_boot[0] = 0xfd;
	//add for auto copy to  code test
	printf("\n.cfg_board_common_setting.timming_magic=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.timming_magic,
	       ddr_set_t_p->cfg_board_common_setting.timming_magic,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.timming_magic));
	printf("\n.cfg_board_common_setting.timming_max_valid_configs=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.timming_max_valid_configs,
	       ddr_set_t_p->cfg_board_common_setting.timming_max_valid_configs,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.timming_max_valid_configs));
	printf("\n.cfg_board_common_setting.timming_struct_version=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.timming_struct_version,
	       ddr_set_t_p->cfg_board_common_setting.timming_struct_version,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.timming_struct_version));
	printf("\n.cfg_board_common_setting.timming_struct_org_size=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.timming_struct_org_size,
	       ddr_set_t_p->cfg_board_common_setting.timming_struct_org_size,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.timming_struct_org_size));
	printf("\n.cfg_board_common_setting.timming_struct_real_size=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.timming_struct_real_size,
	       ddr_set_t_p->cfg_board_common_setting.timming_struct_real_size,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.timming_struct_real_size));
	for (count = 0; count < 4; count++)
		printf("\n.cfg_board_common_setting.fast_boot[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_board_common_setting.fast_boot[count],
		       ddr_set_t_p->cfg_board_common_setting.fast_boot[count],
		       DDR_TIMMING_OFFSET(cfg_board_common_setting.fast_boot[count]));
	printf("\n.cfg_board_common_setting.ddr_func=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.ddr_func,
	       ddr_set_t_p->cfg_board_common_setting.ddr_func,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.ddr_func));
	printf("\n.cfg_board_common_setting.board_id=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.board_id,
	       ddr_set_t_p->cfg_board_common_setting.board_id,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.board_id));
	printf("\n.cfg_board_common_setting.DramType=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.DramType,
	       ddr_set_t_p->cfg_board_common_setting.DramType,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.DramType));
	printf("\n.cfg_board_common_setting.dram_rank_config=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.dram_rank_config,
	       ddr_set_t_p->cfg_board_common_setting.dram_rank_config,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.dram_rank_config));
	printf("\n.cfg_board_common_setting.rsv_char0=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.rsv_char0,
	       ddr_set_t_p->cfg_board_common_setting.rsv_char0,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.rsv_char0));
	for (count = 0; count < 2; count++)
		printf("\n.cfg_board_common_setting.DisabledDbyte[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_board_common_setting.DisabledDbyte[count],
		       ddr_set_t_p->cfg_board_common_setting.DisabledDbyte[count],
		       DDR_TIMMING_OFFSET(cfg_board_common_setting.DisabledDbyte[count]));
	printf("\n.cfg_board_common_setting.dram_ch0_size_MB=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.dram_ch0_size_MB,
	       ddr_set_t_p->cfg_board_common_setting.dram_ch0_size_MB,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.dram_ch0_size_MB));
	printf("\n.cfg_board_common_setting.dram_ch1_size_MB=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.dram_ch1_size_MB,
	       ddr_set_t_p->cfg_board_common_setting.dram_ch1_size_MB,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.dram_ch1_size_MB));
	printf("\n.cfg_board_common_setting.dram_x4x8x16_mode=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.dram_x4x8x16_mode,
	       ddr_set_t_p->cfg_board_common_setting.dram_x4x8x16_mode,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.dram_x4x8x16_mode));
	printf("\n.cfg_board_common_setting.Is2Ttiming=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.Is2Ttiming,
	       ddr_set_t_p->cfg_board_common_setting.Is2Ttiming,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.Is2Ttiming));
	printf("\n.cfg_board_common_setting.log_level=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.log_level,
	       ddr_set_t_p->cfg_board_common_setting.log_level,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.log_level));
	printf("\n.cfg_board_common_setting.dbi_enable=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.dbi_enable,
	       ddr_set_t_p->cfg_board_common_setting.dbi_enable,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.dbi_enable));
	printf("\n.cfg_board_common_setting.ddr_rfc_type=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.ddr_rfc_type,
	       ddr_set_t_p->cfg_board_common_setting.ddr_rfc_type,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.ddr_rfc_type));
	printf("\n.cfg_board_common_setting.enable_lpddr4x_mode=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.enable_lpddr4x_mode,
	       ddr_set_t_p->cfg_board_common_setting.enable_lpddr4x_mode,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.enable_lpddr4x_mode));
	printf("\n.cfg_board_common_setting.pll_ssc_mode=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.pll_ssc_mode,
	       ddr_set_t_p->cfg_board_common_setting.pll_ssc_mode,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.pll_ssc_mode));
	printf("\n.cfg_board_common_setting.org_tdqs2dq=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.org_tdqs2dq,
	       ddr_set_t_p->cfg_board_common_setting.org_tdqs2dq,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.org_tdqs2dq));
	for (count = 0; count < 2; count++)
		printf("\n.cfg_board_common_setting.reserve1_test[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_board_common_setting.reserve1_test[count],
		       ddr_set_t_p->cfg_board_common_setting.reserve1_test[count],
		       DDR_TIMMING_OFFSET(cfg_board_common_setting.reserve1_test[count]));
	for (count = 0; count < 5; count++)
		printf("\n.cfg_board_common_setting.ddr_dmc_remap[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_board_common_setting.ddr_dmc_remap[count],
		       ddr_set_t_p->cfg_board_common_setting.ddr_dmc_remap[count],
		       DDR_TIMMING_OFFSET(cfg_board_common_setting.ddr_dmc_remap[count]));
	for (count = 0; count < 4; count++)
		printf("\n.cfg_board_common_setting.lpddr34_ca_remap[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_board_common_setting.lpddr34_ca_remap[count],
		       ddr_set_t_p->cfg_board_common_setting.lpddr34_ca_remap[count],
		       DDR_TIMMING_OFFSET(cfg_board_common_setting.lpddr34_ca_remap[count]));
	for (count = 0; count < 36; count++)
		printf("\n.cfg_board_common_setting.ddr_dq_remap[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_board_common_setting.ddr_dq_remap[count],
		       ddr_set_t_p->cfg_board_common_setting.ddr_dq_remap[count],
		       DDR_TIMMING_OFFSET(cfg_board_common_setting.ddr_dq_remap[count]));
	for (count = 0; count < 30; count++)
		printf("\n.cfg_board_common_setting.ac_pinmux[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_board_common_setting.ac_pinmux[count],
		       ddr_set_t_p->cfg_board_common_setting.ac_pinmux[count],
		       DDR_TIMMING_OFFSET(cfg_board_common_setting.ac_pinmux[count]));
	for (count = 0; count < 10; count++)
		printf("\n.cfg_board_common_setting.dfi_pinmux[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_board_common_setting.dfi_pinmux[count],
		       ddr_set_t_p->cfg_board_common_setting.dfi_pinmux[count],
		       DDR_TIMMING_OFFSET(cfg_board_common_setting.dfi_pinmux[count]));
	printf("\n.cfg_board_common_setting.ddr_dqs_swap=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.ddr_dqs_swap,
	       ddr_set_t_p->cfg_board_common_setting.ddr_dqs_swap,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.ddr_dqs_swap));
	printf("\n.cfg_board_common_setting.rsv_char1=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.rsv_char1,
	       ddr_set_t_p->cfg_board_common_setting.rsv_char1,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.rsv_char1));
	printf("\n.cfg_board_common_setting.rsv_char2=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.rsv_char2,
	       ddr_set_t_p->cfg_board_common_setting.rsv_char2,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.rsv_char2));
	printf("\n.cfg_board_common_setting.rsv_char3=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_common_setting.rsv_char3,
	       ddr_set_t_p->cfg_board_common_setting.rsv_char3,
	       DDR_TIMMING_OFFSET(cfg_board_common_setting.rsv_char3));
	for (count = 0; count < 4; count++)
		printf("\n.cfg_board_common_setting.ddr_vddee_setting[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_board_common_setting.ddr_vddee_setting[count],
		       ddr_set_t_p->cfg_board_common_setting.ddr_vddee_setting[count],
		       DDR_TIMMING_OFFSET(cfg_board_common_setting.ddr_vddee_setting[count]));
	printf("\n.cfg_board_SI_setting_ps.DRAMFreq=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.DRAMFreq,
	       ddr_set_t_p->cfg_board_SI_setting_ps.DRAMFreq,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.DRAMFreq));
	printf("\n.cfg_board_SI_setting_ps.PllBypassEn=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.PllBypassEn,
	       ddr_set_t_p->cfg_board_SI_setting_ps.PllBypassEn,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.PllBypassEn));
	printf("\n.cfg_board_SI_setting_ps.training_SequenceCtrl=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.training_SequenceCtrl,
	       ddr_set_t_p->cfg_board_SI_setting_ps.training_SequenceCtrl,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.training_SequenceCtrl));
	printf("\n.cfg_board_SI_setting_ps.dfi_odt_config=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.dfi_odt_config,
	       ddr_set_t_p->cfg_board_SI_setting_ps.dfi_odt_config,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.dfi_odt_config));
	for (count = 0; count < 2; count++)
		printf("\n.cfg_board_SI_setting_ps.phy_odt_config_rank[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_board_SI_setting_ps.phy_odt_config_rank[count],
		       ddr_set_t_p->cfg_board_SI_setting_ps.phy_odt_config_rank[count],
		       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.phy_odt_config_rank[count]));
	printf("\n.cfg_board_SI_setting_ps.clk_drv_ohm=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.clk_drv_ohm,
	       ddr_set_t_p->cfg_board_SI_setting_ps.clk_drv_ohm,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.clk_drv_ohm));
	printf("\n.cfg_board_SI_setting_ps.cs_drv_ohm=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.cs_drv_ohm,
	       ddr_set_t_p->cfg_board_SI_setting_ps.cs_drv_ohm,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.cs_drv_ohm));
	printf("\n.cfg_board_SI_setting_ps.ac_drv_ohm=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.ac_drv_ohm,
	       ddr_set_t_p->cfg_board_SI_setting_ps.ac_drv_ohm,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.ac_drv_ohm));
	printf("\n.cfg_board_SI_setting_ps.soc_data_drv_ohm_p=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_data_drv_ohm_p,
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_data_drv_ohm_p,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.soc_data_drv_ohm_p));
	printf("\n.cfg_board_SI_setting_ps.soc_data_drv_ohm_n=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_data_drv_ohm_n,
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_data_drv_ohm_n,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.soc_data_drv_ohm_n));
	printf("\n.cfg_board_SI_setting_ps.soc_data_odt_ohm_p=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_data_odt_ohm_p,
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_data_odt_ohm_p,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.soc_data_odt_ohm_p));
	printf("\n.cfg_board_SI_setting_ps.soc_data_odt_ohm_n=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_data_odt_ohm_n,
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_data_odt_ohm_n,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.soc_data_odt_ohm_n));
	printf("\n.cfg_board_SI_setting_ps.dram_data_drv_ohm=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.dram_data_drv_ohm,
	       ddr_set_t_p->cfg_board_SI_setting_ps.dram_data_drv_ohm,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.dram_data_drv_ohm));
	printf("\n.cfg_board_SI_setting_ps.dram_data_odt_ohm=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.dram_data_odt_ohm,
	       ddr_set_t_p->cfg_board_SI_setting_ps.dram_data_odt_ohm,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.dram_data_odt_ohm));
	printf("\n.cfg_board_SI_setting_ps.dram_data_wr_odt_ohm=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.dram_data_wr_odt_ohm,
	       ddr_set_t_p->cfg_board_SI_setting_ps.dram_data_wr_odt_ohm,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.dram_data_wr_odt_ohm));
	printf("\n.cfg_board_SI_setting_ps.dram_ac_odt_ohm=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.dram_ac_odt_ohm,
	       ddr_set_t_p->cfg_board_SI_setting_ps.dram_ac_odt_ohm,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.dram_ac_odt_ohm));
	printf("\n.cfg_board_SI_setting_ps.soc_clk_slew_rate=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_clk_slew_rate,
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_clk_slew_rate,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.soc_clk_slew_rate));
	printf("\n.cfg_board_SI_setting_ps.soc_cs_slew_rate=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_cs_slew_rate,
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_cs_slew_rate,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.soc_cs_slew_rate));
	printf("\n.cfg_board_SI_setting_ps.soc_ac_slew_rate=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_ac_slew_rate,
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_ac_slew_rate,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.soc_ac_slew_rate));
	printf("\n.cfg_board_SI_setting_ps.soc_data_slew_rate=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_data_slew_rate,
	       ddr_set_t_p->cfg_board_SI_setting_ps.soc_data_slew_rate,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.soc_data_slew_rate));
	printf("\n.cfg_board_SI_setting_ps.dram_drv_pull_up_cal_ohm=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.dram_drv_pull_up_cal_ohm,
	       ddr_set_t_p->cfg_board_SI_setting_ps.dram_drv_pull_up_cal_ohm,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.dram_drv_pull_up_cal_ohm));
	printf("\n.cfg_board_SI_setting_ps.lpddr4_dram_vout_range=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.lpddr4_dram_vout_range,
	       ddr_set_t_p->cfg_board_SI_setting_ps.lpddr4_dram_vout_range,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.lpddr4_dram_vout_range));
	printf("\n.cfg_board_SI_setting_ps.char_rev0=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.char_rev0,
	       ddr_set_t_p->cfg_board_SI_setting_ps.char_rev0,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.char_rev0));
	printf("\n.cfg_board_SI_setting_ps.char_rev1=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.char_rev1,
	       ddr_set_t_p->cfg_board_SI_setting_ps.char_rev1,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.char_rev1));
	printf("\n.cfg_board_SI_setting_ps.vref_ac_permil=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.vref_ac_permil,
	       ddr_set_t_p->cfg_board_SI_setting_ps.vref_ac_permil,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.vref_ac_permil));
	printf("\n.cfg_board_SI_setting_ps.vref_soc_data_permil=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.vref_soc_data_permil,
	       ddr_set_t_p->cfg_board_SI_setting_ps.vref_soc_data_permil,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.vref_soc_data_permil));
	printf("\n.cfg_board_SI_setting_ps.vref_dram_data_permil=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.vref_dram_data_permil,
	       ddr_set_t_p->cfg_board_SI_setting_ps.vref_dram_data_permil,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.vref_dram_data_permil));
	printf("\n.cfg_board_SI_setting_ps.max_core_timmming_frequency=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_board_SI_setting_ps.max_core_timmming_frequency,
	       ddr_set_t_p->cfg_board_SI_setting_ps.max_core_timmming_frequency,
	       DDR_TIMMING_OFFSET(cfg_board_SI_setting_ps.max_core_timmming_frequency));
	for (count = 0; count < 30; count++)
		printf("\n.cfg_ddr_training_delay_ps.ac_trace_delay[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.ac_trace_delay[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.ac_trace_delay[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.ac_trace_delay[count]));
	for (count = 0; count < 72; count++)
		printf("\n.cfg_ddr_training_delay_ps.read_dq_delay_t[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.read_dq_delay_t[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.read_dq_delay_t[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.read_dq_delay_t[count]));
	for (count = 0; count < 72; count++)
		printf("\n.cfg_ddr_training_delay_ps.read_dq_delay_c[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.read_dq_delay_c[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.read_dq_delay_c[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.read_dq_delay_c[count]));
	for (count = 0; count < 8; count++)
		printf("\n.cfg_ddr_training_delay_ps.read_dqs_delay[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.read_dqs_delay[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.read_dqs_delay[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.read_dqs_delay[count]));
	for (count = 0; count < 8; count++)
		printf("\n.cfg_ddr_training_delay_ps.write_dqs_delay[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.write_dqs_delay[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.write_dqs_delay[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.write_dqs_delay[count]));
	for (count = 0; count < 8; count++)
		printf("\n.cfg_ddr_training_delay_ps.write_wck_delay[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.write_wck_delay[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.write_wck_delay[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.write_wck_delay[count]));
	for (count = 0; count < 72; count++)
		printf("\n.cfg_ddr_training_delay_ps.wdq_delay[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.wdq_delay[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.wdq_delay[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.wdq_delay[count]));
	for (count = 0; count < 8; count++)
		printf("\n.cfg_ddr_training_delay_ps.read_dqs_gate_delay[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.read_dqs_gate_delay[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.read_dqs_gate_delay[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.read_dqs_gate_delay[count]));
	for (count = 0; count < 36; count++)
		printf("\n.cfg_ddr_training_delay_ps.soc_bit_vref0[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.soc_bit_vref0[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.soc_bit_vref0[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.soc_bit_vref0[count]));
	for (count = 0; count < 36; count++)
		printf("\n.cfg_ddr_training_delay_ps.soc_bit_vref1[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.soc_bit_vref1[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.soc_bit_vref1[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.soc_bit_vref1[count]));
	for (count = 0; count < 36; count++)
		printf("\n.cfg_ddr_training_delay_ps.soc_bit_vref2[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.soc_bit_vref2[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.soc_bit_vref2[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.soc_bit_vref2[count]));
	for (count = 0; count < 36; count++)
		printf("\n.cfg_ddr_training_delay_ps.soc_bit_vref3[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.soc_bit_vref3[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.soc_bit_vref3[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.soc_bit_vref3[count]));
	for (count = 0; count < 32; count++)
		printf("\n.cfg_ddr_training_delay_ps.dram_vref[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dram_vref[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dram_vref[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.dram_vref[count]));
	for (count = 0; count < 8; count++)
		printf("\n.cfg_ddr_training_delay_ps.dca_wck_tx_t[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dca_wck_tx_t[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dca_wck_tx_t[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.dca_wck_tx_t[count]));
	for (count = 0; count < 8; count++)
		printf("\n.cfg_ddr_training_delay_ps.dca_wck_rx_t[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dca_wck_rx_t[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dca_wck_rx_t[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.dca_wck_rx_t[count]));
	for (count = 0; count < 8; count++)
		printf("\n.cfg_ddr_training_delay_ps.dca_dqs_tx_t[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dca_dqs_tx_t[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dca_dqs_tx_t[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.dca_dqs_tx_t[count]));
	for (count = 0; count < 8; count++)
		printf("\n.cfg_ddr_training_delay_ps.dca_wck_tx_c[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dca_wck_tx_c[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dca_wck_tx_c[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.dca_wck_tx_c[count]));
	for (count = 0; count < 8; count++)
		printf("\n.cfg_ddr_training_delay_ps.dca_wck_rx_c[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dca_wck_rx_c[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dca_wck_rx_c[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.dca_wck_rx_c[count]));
	for (count = 0; count < 8; count++)
		printf("\n.cfg_ddr_training_delay_ps.dca_dqs_tx_c[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dca_dqs_tx_c[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dca_dqs_tx_c[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.dca_dqs_tx_c[count]));
	for (count = 0; count < 8; count++)
		printf("\n.cfg_ddr_training_delay_ps.dca_dq_tx[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dca_dq_tx[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dca_dq_tx[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.dca_dq_tx[count]));
	for (count = 0; count < 4; count++)
		printf("\n.cfg_ddr_training_delay_ps.dfi_mrl[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dfi_mrl[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dfi_mrl[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.dfi_mrl[count]));
	printf("\n.cfg_ddr_training_delay_ps.dfi_hwtmrl=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_ddr_training_delay_ps.dfi_hwtmrl,
	       ddr_set_t_p->cfg_ddr_training_delay_ps.dfi_hwtmrl,
	       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.dfi_hwtmrl));
	printf("\n.cfg_ddr_training_delay_ps.csr_hwtctrl=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_ddr_training_delay_ps.csr_hwtctrl,
	       ddr_set_t_p->cfg_ddr_training_delay_ps.csr_hwtctrl,
	       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.csr_hwtctrl));
	printf("\n.cfg_ddr_training_delay_ps.rever1=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_ddr_training_delay_ps.rever1,
	       ddr_set_t_p->cfg_ddr_training_delay_ps.rever1,
	       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.rever1));
	printf("\n.cfg_ddr_training_delay_ps.rever2=0x%x,//%d，0x%x",
	       ddr_set_t_p->cfg_ddr_training_delay_ps.rever2,
	       ddr_set_t_p->cfg_ddr_training_delay_ps.rever2,
	       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.rever2));
	for (count = 0; count < 4; count++)
		printf("\n.cfg_ddr_training_delay_ps.pptdqscnttg0[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.pptdqscnttg0[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.pptdqscnttg0[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.pptdqscnttg0[count]));
	for (count = 0; count < 4; count++)
		printf("\n.cfg_ddr_training_delay_ps.pptdqscnttg1[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.pptdqscnttg1[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.pptdqscnttg1[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.pptdqscnttg1[count]));
	for (count = 0; count < 4; count++)
		printf("\n.cfg_ddr_training_delay_ps.PptWck2DqoCntTg0[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.PptWck2DqoCntTg0[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.PptWck2DqoCntTg0[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.PptWck2DqoCntTg0[count]));
	for (count = 0; count < 4; count++)
		printf("\n.cfg_ddr_training_delay_ps.PptWck2DqoCntTg1[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.PptWck2DqoCntTg1[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.PptWck2DqoCntTg1[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.PptWck2DqoCntTg1[count]));
	//for (count = 0; count < 20; count++)
	//	printf("\n.cfg_ddr_training_delay_ps.RxReplicaPhase[%d] = 0x%x,//%d，0x%x",
	//	       count,
	//	       ddr_set_t_p->cfg_ddr_training_delay_ps.RxReplicaPhase[count],
	//	       ddr_set_t_p->cfg_ddr_training_delay_ps.RxReplicaPhase[count],
	//	       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.RxReplicaPhase[count]));
	for (count = 0; count < 4; count++)
		printf("\n.cfg_ddr_training_delay_ps.dac_offset[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dac_offset[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.dac_offset[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.dac_offset[count]));
	for (count = 0; count < 2; count++)
		printf("\n.cfg_ddr_training_delay_ps.rx_offset[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.rx_offset[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.rx_offset[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.rx_offset[count]));
	for (count = 0; count < 2; count++)
		printf("\n.cfg_ddr_training_delay_ps.tx_offset[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.tx_offset[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.tx_offset[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.tx_offset[count]));
	for (count = 0; count < 16; count++)
		printf("\n.cfg_ddr_training_delay_ps.reserve_para[%d] = 0x%x,//%d，0x%x",
		       count,
		       ddr_set_t_p->cfg_ddr_training_delay_ps.reserve_para[count],
		       ddr_set_t_p->cfg_ddr_training_delay_ps.reserve_para[count],
		       DDR_TIMMING_OFFSET(cfg_ddr_training_delay_ps.reserve_para[count]));

	printf("\n},\n");
	//}

	return 1;
}

#ifdef USE_FOR_UBOOT_2018
#include <amlogic/storage.h>
extern struct storage_t *current;

static int ddr_do_store_ddr_parameter_ops(uint8_t *buffer, uint32_t length)
{
	char str[1024] = "";

	if (!current) {
		sprintf(str, "store init");
		run_command(str, 0);
	}

	{
		printf("\nstore rsv write ddr-parameter 0x%8x 0x%8x\n",
		       (uint32_t)(uint64_t)buffer, length);
		sprintf(str, "store rsv write ddr-parameter 0x%8x 0x%8x\n",
			(uint32_t)(uint64_t)buffer, length);
		run_command(str, 0);
	}

	return 1;
}
#else
static int ddr_do_store_ddr_parameter_ops(uint8_t *buffer, uint32_t length)
{
	//extern int store_ddr_parameter_write(uint8_t *buffer, uint32_t length);

	printf("\nstore ddr_parameter write 0x%8x 0x%8x\n", (uint32_t)(uint64_t)buffer, length);
	store_ddr_parameter_write((uint8_t *)buffer, length);

	return 1;
}
#endif

int do_ddr_fastboot_config(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	check_base_address();
	int i = 0;
	int count = 0;
	char *endp;
	unsigned int enable_ddr_fast_boot = 0; // 0 pause 1,resume
	char *out_sha2;

	printf("\nargc== 0x%8x\n", argc);
	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);
	if (argc == 1) {
		printf("\nplease read help\n");
	} else if (argc > 1) {
		count = 0;
		enable_ddr_fast_boot = simple_strtoull_ddr(argv[count + 1], &endp, 0);
		if (*argv[count + 1] == 0 || *endp != 0)
			enable_ddr_fast_boot = 0;
	}
	if (!enable_ddr_fast_boot)
		return 1;

	uint32_t dmc_retraining_ctrl = 0;

	dmc_retraining_ctrl = rd_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address);

	if (p_ddr_base->ddr_dmc_lpdd4_retraining_address)
		wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address),
		       dmc_retraining_ctrl & (~(1 << 31)));
	//ddr_set_t *ddr_set_t_p = NULL;

	//ddr_set_t_p = (ddr_set_t *)(ddr_set_t_p_array);
	uint32_t ddr_set_add = 0;
	uint32_t ddr_set_size = 0;

	out_sha2 = (char *)ddr_sha_s5.sha2;

	ddr_set_ps0_only_t *ddr_set_t_p_s5 = NULL;

	ddr_set_t_p_s5 = (ddr_set_ps0_only_t *)(ddr_set_t_p_array_s5);

	uint32_t write_size = 0;

	//if (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S5) {
	ddr_set_add = (uint32_t)(uint64_t)(ddr_set_t_p_s5);
	ddr_set_size = sizeof(ddr_set_ps0_only_t);
	out_sha2 = (char *)ddr_sha_s5.sha2;
	do_read_s5_ddr_training_data(1, ddr_set_t_p_s5);
	//uint32_t loop = 0;
//#if 1
//		//for C2 T5/T5D ddr window fast boot no support dfe vref1
//		if (((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_C2) &&
//		     (p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_T5D))) {
//			ddr_set_size = sizeof(ddr_set_ps0_only_t) - (44 * 2);
//			//sizeof(board_phase_setting_ps_t.soc_bit_vref_dac1);
//			unsigned char *sha_t_chip_id;
//
//			sha_t_chip_id =
//				(unsigned char *)((uint64_t)(&ddr_sha_s5.sha_chip_id) - (44 * 2));
//			for (loop = 0; loop < MESON_CPU_CHIP_ID_SIZE; loop++)
//				//update chip id
//				sha_t_chip_id[loop] = global_chip_id[loop];
//		}
//#endif
	if (enable_ddr_fast_boot == 1)
		ddr_set_t_p_s5->cfg_board_common_setting.fast_boot[0] = 0xff;
	if (enable_ddr_fast_boot == 2)
		ddr_set_t_p_s5->cfg_board_common_setting.fast_boot[0] = 0;
	//}

	write_size = ((ddr_set_size + SHA_SUM_LEN + MESON_CPU_CHIP_ID_SIZE + 511) / 512) * 512;

	{
		//printf("&ddr_sha_s5.ddrs : 0x%x\n", (uint32_t)(uint64_t)&ddr_sha_s5.ddrs);
		//printf("&ddr_sha_s5.sha2 : 0x%x\n", (uint32_t)(uint64_t)out_sha2);
		printf("ddr_set_add : 0x%x   sizeof(ddr_set_t):0x%x\n ",
		       (uint32_t)(uint64_t)ddr_set_add, (uint32_t)(uint64_t)ddr_set_size);
		printf("ddr_set_add_chip_id : 0x%x\n",
		       (uint32_t)(uint64_t)(ddr_set_add + ddr_set_size));
		sha256_csum_wd_internal((unsigned char *)(uint64_t)ddr_set_add,
					ddr_set_size, (unsigned char *)out_sha2, 0);
		printf("print sha\n");
		ddr_do_store_ddr_parameter_ops((uint8_t *)(unsigned long)(ddr_set_add -
									  SHA_SUM_LEN),
					       write_size);
	}
	return 1;
}
#endif

int do_ddr_set_watchdog_value(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char *endp;
	int i = 0;

	printf("\nargc== 0x%8x\n", argc);
	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);

	if (argc == 1) {
		printf("\nplease read help\n");
	} else if (argc > 1) {
		{
			watchdog_time_s = simple_strtoull_ddr(argv[1], &endp, 0);
			if (*argv[1] == 0 || *endp != 0)
				watchdog_time_s = 20;
		}
		printf("watchdog_time_s==%d\n", watchdog_time_s);
	}

	return 1;
}

typedef struct training_delay_information {
	uint16_t ac_delay[10];
	uint16_t txdqs_delay[16];
	uint16_t rxdqs_delay[16];
	uint16_t txdq_delay[72];
	uint16_t rxdq_delay[72];
	uint16_t gate_rxdq_delay[72];
} training_delay_t;
training_delay_t training_delay_t_p;

uint16_t lcd_bdl_value[72][4]; //org min max status

#if (CONFIG_DDR_PHY >= P_DDR_PHY_G12)

unsigned int do_ddr_g12_read_write_ddr_add_window_lcdlr(unsigned int	rank_index,
							unsigned int	add_index,
							unsigned int	lcdlr_value,
							unsigned int	read_write_flag)
{
	dwc_ddrphy_apb_wr(0xd0000, 0); //mw fe1a0000  0
	if (read_write_flag == DDR_PARAMETER_READ)
		lcdlr_value = dwc_ddrphy_apb_rd((0 << 20) | (0 << 16) |
						(add_index << 12) | (0x80));
	if (read_write_flag == DDR_PARAMETER_WRITE)
		dwc_ddrphy_apb_wr(((0 << 20) | (0 << 16) |
				   (add_index << 12) | (0x80)), lcdlr_value);

	printf("rank_index %d   add_index %d  lcdlr== %d\n",
	       rank_index, add_index, lcdlr_value);
	return lcdlr_value;
}

void dwc_window_reg_after_training_update_increase_dq(char	over_ride_index,
						      uint32_t	over_ride_sub_index,
						      uint32_t	over_ride_increase_decrease,
						      uint32_t	step_value)
{
	uint32_t delay_old_value = 0;
	uint32_t delay_reg_value = 0;

	uint64_t reg_add = 0;

	if (!over_ride_index)
		return;

	if (over_ride_index == DMC_TEST_WINDOW_INDEX_TXDQDLY) {
		reg_add = ((0 << 20) | (1 << 16) |
			   (((over_ride_sub_index % 36) / 9) << 12) |
			   (0xc0 + ((over_ride_sub_index % 9) << 8) +
			    (over_ride_sub_index / 36)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		delay_reg_value = ddr_cacl_phy_delay_all_step(over_ride_index,
							      delay_old_value);
		if (over_ride_increase_decrease == 0) {
			delay_reg_value = delay_reg_value + step_value;
			if (delay_reg_value > 255)
				delay_reg_value = 255;
		}
		if (over_ride_increase_decrease != 0) {
			if (delay_reg_value > step_value)
				delay_reg_value = delay_reg_value - step_value;
			else
				delay_reg_value = 0;
		}
		delay_reg_value = ddr_cacl_phy_over_ride_back_reg(over_ride_index,
								  delay_reg_value);
		dwc_ddrphy_apb_wr(reg_add, delay_reg_value);
	}

	printf("reg_add %8x old_value %8x update_to %8x dec %d to %d\n",
	       ((unsigned int)(((reg_add) << 1) + (phy_base_add))),
	       delay_old_value, dwc_ddrphy_apb_rd(reg_add),
	       ddr_cacl_phy_delay_all_step(over_ride_index, delay_old_value),
	       (unsigned int)ddr_cacl_phy_delay_all_step(over_ride_index,
							 dwc_ddrphy_apb_rd(reg_add)));
}

int do_ddr2pll_g12_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	check_base_address();
	char *endp;
	unsigned int pll;
	unsigned int window_test_stick_cmd_value = 0;

	/* need at least two arguments */
	//if (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_C2 ||
	//    p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5 ||
	//    p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5D ||
	//    p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S5 ||
	//    p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)
	do_read_s5_ddr_bdlr_steps();
	if (argc < 2)
		//goto usage;
		return 1;

	pll = simple_strtoul(argv[1], &endp, 0);
	if (*argv[1] == 0 || *endp != 0) {
		printf("Error: Wrong format parament!pll=0x%8x\n", pll);
		return 1;
	}
	unsigned int argc_count = 1;
	unsigned int para_meter[30] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0,  0, 0, 0,
	};

	while (argc_count < argc) {
		para_meter[argc_count - 1] = simple_strtoul(argv[argc_count], &endp, 0);
		if (*argv[argc_count] == 0 || *endp != 0)
			para_meter[argc_count - 1] = 0;
		argc_count++;
	}

	argc_count = 2;
	window_test_stick_cmd_value = para_meter[argc_count - 1];
	window_test_stick_cmd_value = (window_test_stick_cmd_value & 0xff);
	if (window_test_stick_cmd_value == G12_D2PLL_CMD_OVER_RIDE ||
	    window_test_stick_cmd_value == G12_D2PLL_CMD_OVER_RIDE_PLUS_FULLTEST) {
		para_meter[3] = (para_meter[3] << 24) |
				(para_meter[4] << 16) | (para_meter[5] << 0);
		para_meter[4] = (para_meter[6] << 24) |
				(para_meter[7] << 16) | (para_meter[8] << 0);
		para_meter[5] = (para_meter[9] << 24) |
				(para_meter[10] << 16) | (para_meter[11] << 0);
	}
	if (window_test_stick_cmd_value == G12_D2PLL_CMD_WINDOW_TEST ||
	    window_test_stick_cmd_value == G12_D2PLL_CMD_WINDOW_TEST_AND_STICKY_OVERRIDE)
		para_meter[5] = (para_meter[9] << 28) |
				(para_meter[10] << 24) | (para_meter[11] << 20) |
				(para_meter[12] << 21) | (para_meter[13] << 22) |
				(para_meter[14] << 25) | (para_meter[5] << 0);
	if (window_test_stick_cmd_value >= G12_D2PLL_CMD_DDR_EYE_TEST &&
	    window_test_stick_cmd_value <= G12_D2PLL_CMD_DDR_EYE_TEST_AND_STICKY_OVERRIDE_DAC1) {
		para_meter[3] = (para_meter[3] << 0) |
				(para_meter[4] << 8) |
				(para_meter[5] << 16) | (para_meter[6] << 24);
		para_meter[4] = (para_meter[7] << 0) |
				(para_meter[8] << 8) |
				(para_meter[9] << 16) | (para_meter[10] << 24);
		para_meter[5] = para_meter[11];
		//(para_meter[11]<<0)|(para_meter[12]<<8)|
		//(para_meter[13]<<16)|(para_meter[14]<<24);
		para_meter[6] = para_meter[12]; //para_meter[15];
		para_meter[7] = para_meter[13]; //para_meter[16];
		para_meter[8] = para_meter[14]; //para_meter[17];
	}
	wr_reg((p_ddr_base->preg_sticky_reg0),
	       (rd_reg((p_ddr_base->preg_sticky_reg0)) & 0xffff) | (0xf13 << 20));
	argc_count = 0;
	printf("\nP_PREG_STICKY_REG [0x%8x]  [0x%8x]==[0x%8x]\n",
	       argc_count, ((p_ddr_base->preg_sticky_reg0) + (argc_count << 2)),
	       rd_reg((p_ddr_base->preg_sticky_reg0) + (argc_count << 2)));
	argc_count = 1;
	while (argc_count < 10) {
		wr_reg((p_ddr_base->preg_sticky_reg0) + (argc_count << 2),
		       para_meter[argc_count - 1]);
		printf("P_PREG_STICKY_REG [0x%8x]  [0x%8x]==[0x%8x]\n",
		       argc_count, ((p_ddr_base->preg_sticky_reg0) + (argc_count << 2)),
		       rd_reg((p_ddr_base->preg_sticky_reg0) + (argc_count << 2)));
		argc_count++;
	}
	dcache_disable();
	//if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_A1) ||
	//    (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_C1) ||
	//    (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_C2)
	//    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_SC2) ||
	//    (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S4)
	//    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)) {
	printf("reset...\n");
	run_command("reset", 0);
	run_command("reboot", 0);
	//} else {
	//	//G12A/G12B/SM1/TL1/TM2//(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T7)
	//	printf("reboot...\n");
	//	//reset will enter bl2 panic path,so change to "reboot"
	//	run_command("reboot", 0);
	//	run_command("reset", 0);
	//	//ddr_test_watchdog_reset_system();
	//}

	return 0;

//usage:
//	cmd_usage(cmdtp);
//	return 1;
}

U_BOOT_CMD(g12_d2pll, 18, 1, do_ddr2pll_g12_cmd,
	   "g12_d2pll 1300  1 0x10 0",
	   "g12_d2pll  clk delay_index delay_value before_after_training_setting\n");

#endif

#if (CONFIG_DDR_PHY >= P_DDR_PHY_G12)

void ddr_reg_increase_sub(char		over_ride_index,
			  uint32_t	over_ride_sub_index,
			  uint32_t	over_ride_increase_decrease,
			  uint32_t	step)
{
	uint32_t delay_old_value = 0;
	uint32_t delay_reg_value = 0;
	uint32_t ps = 0;

	if (!over_ride_index)
		return;
	if (over_ride_index <= DMC_TEST_WINDOW_INDEX_DRAM_VREF) {
		delay_old_value = ddr_training_reg_rw(ddr_set_t_p,
						      over_ride_index,
						      over_ride_sub_index,
						      delay_old_value,
						      REGISTER_READ, ps);
		if (over_ride_increase_decrease == 0)
			delay_reg_value = delay_old_value + step;
		if (over_ride_increase_decrease != 0) {
			if (delay_old_value > step)
				delay_reg_value = delay_old_value - step;
			else
				delay_reg_value = 0;
		}

		ddr_training_reg_rw(ddr_set_t_p, over_ride_index,
				    over_ride_sub_index, delay_reg_value,
				    REGISTER_WRITE,
				    ps);
	}
}

void ddr_reg_increase(char	over_ride_index,
		      uint32_t	over_ride_sub_index,
		      uint32_t	over_ride_increase_decrease,
		      uint32_t	offset_value)
{
	uint32_t temp_count_3 = 0;

	if (!over_ride_index)
		return;

	//if (over_ride_index == DMC_TEST_WINDOW_INDEX_ATXDLY)
	for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
		ddr_reg_increase_sub(over_ride_index,
				     ((over_ride_sub_index)),
				     over_ride_increase_decrease, 1);
}

int do_ddr_s5_offset_data(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	check_base_address();

	uint32_t dmc_retraining_ctrl = 0;

	dmc_retraining_ctrl = rd_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address));
	wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl & (~(1 << 31)));
	unsigned int ddr_dmc_apd_temp_save, ddr_dmc_asr_temp_save;

	ddr_dmc_apd_temp_save = readl(p_ddr_base->ddr_dmc_apd_address);
	ddr_dmc_asr_temp_save = readl(p_ddr_base->ddr_dmc_asr_address);
#define  G12_DATA_READ_OFFSET_MAX   (0X3F)
#define  G12_DATA_WRITE_OFFSET_MAX   (0X3F + 7 * 32)
	printf("\12nm phy read write register should closed apd and asr function\n");
	int i = 0;

	printf("\nargc== 0x%8x\n", argc);
	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);
	char *endp;
	//rank_index  dq_index  write_read left/right  offset_value
	unsigned int test_index = 0;
	// 1 ac ,0x2, write dqs ,0x4,read dqs,0x8,write dq,0x10 read dq
	unsigned int test_dq_mask_1 = 0;        //each bit mask correspond with dq_index
	unsigned int test_dq_mask_2 = 0;        //each bit mask correspond with dq_index
	unsigned int test_dq_mask_3 = 0;        //each bit mask correspond with dq_index
	unsigned int left_right_flag = 0;       //  1 left ,2 right   #define  DDR_PARAMETER_LEFT
	//1     #define   DDR_PARAMETER_RIGHT		2
	unsigned int offset_value = 0;          //
	unsigned int count = 0;
	unsigned int count_max = 0;
	//unsigned int lcdlr_max = 0;

	global_ddr_clk = get_ddr_clk();
	bdlr_100step = get_bdlr_100step(global_ddr_clk);
	if (global_ddr_clk)
		ui_1_32_100step = (1000000 * 100 / (global_ddr_clk * 2 * 32));

	if (argc == 1) {
		printf("\nplease read aml phy cmd help\n");
	} else if (argc > 6) { //offset_enable=1;
		{
			count = 0;
			test_index = simple_strtoull_ddr(argv[count + 1], &endp, 0);
			if (*argv[count + 1] == 0 || *endp != 0)
				test_index = 0;
		}
		{
			count++;
			test_dq_mask_1 = simple_strtoull_ddr(argv[count + 1], &endp, 0);
			if (*argv[count + 1] == 0 || *endp != 0)
				test_dq_mask_1 = 0;
		}
		{
			count++;
			test_dq_mask_2 = simple_strtoull_ddr(argv[count + 1], &endp, 0);
			if (*argv[count + 1] == 0 || *endp != 0)
				test_dq_mask_2 = 0;
		}
		{
			count++;
			test_dq_mask_3 = simple_strtoull_ddr(argv[count + 1], &endp, 0);
			if (*argv[count + 1] == 0 || *endp != 0)
				test_dq_mask_3 = 0;
		}
		{
			count++;
			left_right_flag = simple_strtoull_ddr(argv[count + 1], &endp, 0);
			if (*argv[count + 1] == 0 || *endp != 0)
				left_right_flag = 0;
		}
		{
			count++;
			offset_value = simple_strtoull_ddr(argv[count + 1], &endp, 0);
			if (*argv[count + 1] == 0 || *endp != 0)
				offset_value = 0;
		}
	} else {
		wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
		return 1;
	}

	//if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)) {
	writel(ddr_dmc_apd_temp_save, p_ddr_base->ddr_dmc_apd_address);
	writel(ddr_dmc_asr_temp_save, p_ddr_base->ddr_dmc_asr_address);
	wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
	count = 0;
	test_index = simple_strtoull_ddr(argv[count + 1], &endp, 0);
	if (*argv[count + 1] == 0 || *endp != 0)
		test_index = 0;
	if (test_index / 100)
		dmc_ddr_config_channel_id = 2;
	else
		dmc_ddr_config_channel_id = 0;
	test_index = (test_index % 100);         //for DDR_PHY 1
	dmc_change_channel(dmc_ddr_config_channel_id);
	dmc_retraining_ctrl = rd_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address);
	wr_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address,
	       dmc_retraining_ctrl & (~(1 << 31)));
	ddr_dmc_apd_temp_save = readl(p_ddr_base->ddr_dmc_apd_address);
	ddr_dmc_asr_temp_save = readl(p_ddr_base->ddr_dmc_asr_address);
	//}

	//printf("lcdlr_max %d,\n", lcdlr_max);
	if (left_right_flag == DDR_PARAMETER_RIGHT)
		printf("offset right ++  left_right_flag %d,\n", left_right_flag);
	if (left_right_flag == DDR_PARAMETER_LEFT)
		printf("offset left --left_right_flag %d,\n", left_right_flag);

	if (test_index == DMC_TEST_WINDOW_INDEX_ATXDLY)
		count_max = 30;
	//	lcdlr_max = 4 * 128 - 1;//0x3ff;
	if (test_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY)
		count_max = 8;
	//	lcdlr_max = 4 * 128 - 1;//0x3ff;
	if (test_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY)
		count_max = 8;
	//	lcdlr_max = 255;//0x3f;

	if (test_index == DMC_TEST_WINDOW_INDEX_TXDQDLY)
		count_max = 36 * 2;
	//	lcdlr_max = 8 * 128 - 1;//0x1ff;

	if (test_index == DMC_TEST_WINDOW_INDEX_RXPBDLY)//DMC_TEST_WINDOW_INDEX_RX_CLK_T_DLY
		count_max = 36 * 2;
	//	lcdlr_max = 255;
	if (test_index == DMC_TEST_WINDOW_INDEX_RXENDLY)
		count_max = 8;
	//	lcdlr_max = 8 * 128 - 1;//0x1ff;
	if (test_index == WINDOW_TEST_SOC_VREF_DAC0) {//WINDOW_TEST_SOC_VREF_DAC0
		count_max = 36 * 1;
		//	lcdlr_max = 0x3f;
		printf(" soc vref rank0 and rank1 share vref dac\n");
	}
	if (test_index == WINDOW_TEST_SOC_VREF_DAC1) {
		count_max = 36 * 1;
		//	lcdlr_max = 0x3f;
		printf(" soc vref rank0 and rank1 share vref dac\n");
	}
	count = 0;
	writel((0), p_ddr_base->ddr_dmc_apd_address);
	writel((0), p_ddr_base->ddr_dmc_asr_address);

	for (; count < count_max; count++) {
		if (count < 32)
			if (test_dq_mask_1 & (1 << (count % 32)))
				continue;

		if (count > 31 && count < 63)
			if (test_dq_mask_2 & (1 << (count % 32)))
				continue;

		if (count > 63)
			if (test_dq_mask_3 & (1 << (count % 32)))
				continue;
		{
			if (left_right_flag == DDR_PARAMETER_RIGHT)
				ddr_reg_increase(test_index,
						 count,
						 0, offset_value);
			if (left_right_flag == DDR_PARAMETER_LEFT)
				ddr_reg_increase(test_index,
						 count,
						 1, offset_value);
		}
	}
	writel(ddr_dmc_apd_temp_save, p_ddr_base->ddr_dmc_apd_address);
	writel(ddr_dmc_asr_temp_save, p_ddr_base->ddr_dmc_asr_address);
	wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);

	//if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)) {
	dmc_ddr_config_channel_id = 0;
	dmc_change_channel(dmc_ddr_config_channel_id);
	//}
	return 1;
}

int do_ddr_g12_offset_data(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	//ddr_g12_offset_data  1 0  0 0  1 3
	check_base_address();

	//if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_s5)
	//    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5)
	//    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5D)
	//    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S5)
	//    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)) {
	do_ddr_s5_offset_data(cmdtp, flag, argc, argv);
	//return 1;
	//}
	return 1;
}

U_BOOT_CMD(ddr_g12_offset_data, 20, 1, do_ddr_g12_offset_data,
	   "ddr_g12_offset_data  1 0  0 0  1 3",
	   "ddr_g12_offset_data  test_index  mask1 mask2 mask3  left/right  offset_value\n");

#endif

int do_ddr_test_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	check_base_address();

	uint32_t dmc_retraining_ctrl = 0;

	dmc_retraining_ctrl = rd_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address));
	unsigned int ddr_dmc_apd_temp_save, ddr_dmc_asr_temp_save;

	ddr_dmc_apd_temp_save = readl(p_ddr_base->ddr_dmc_apd_address);
	ddr_dmc_asr_temp_save = readl(p_ddr_base->ddr_dmc_asr_address);

	printf("\n ddr test cmd version== %s\n", CMD_VER);
	printf("\nargc== 0x%8x\n", argc);
	int i;

	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);

	/* need at least two arguments */
	if (argc < 2)
		//goto usage;
		return 1;
	if ((strcmp(argv[1], "h") == 0))
		//goto usage;
		return 1;
	printf("\12nm phy read write register should closed apd and asr function\n");

	global_ddr_clk = get_ddr_clk();
	printf("\nddr_clk== %dMHz\n", global_ddr_clk);

#define  DDR_TEST_CMD__NONE   0
#define  DDR_TEST_CMD__DDR_TEST         1
#define  DDR_TEST_CMD__DDR_TUNE_ACLCDLR        2
#define  DDR_TEST_CMD__DDR_TUNE_MAX_CLK     3 //ddr_test_cmd 3 0x8000000 3 1
#define  DDR_TEST_CMD__DDR_TUNE_ZQ     4
#define  DDR_TEST_CMD__DDR_TUNE_VREF    5
#define  DDR_TEST_CMD__DDR_GXTVBB_CROSSTALK    6
#define  DDR_TEST_CMD__DDR_BANDWIDTH_TEST   7
#define  DDR_TEST_CMD__DDR_LCDLR_ENV_TUNE   8
#define  DDR_TEST_CMD__DDR_MODIFY_REG_USE_MASK   9
#define  DDR_TEST_CMD__DDR_DDR_TUNE_AC_CLK   0xa

#define  DDR_TEST_CMD__DDR_SETZQ   0x10
#define  DDR_TEST_CMD__DDR_TUNE_DQS  0x11
#define  DDR_TEST_CMD__DDR_SET_TEST_START_ADD  0x12
#define  DDR_TEST_CMD__DDR_TEST_AC_BIT_SETUP_HOLD_MARGIN  0x13
#define  DDR_TEST_CMD__DDR_TEST_DATA_BIT_SETUP_HOLD_MARGIN  0x14
#define  DDR_TEST_CMD__DDR_TEST_AC_LANE_BIT_MARGIN  0x15
#define  DDR_TEST_CMD__DDR_TEST_EE_VOLTAGE_MDLR_STEP  0x16
#define  DDR_TEST_CMD__DDR_TEST_D2PLL_CMD  0x17
#define  DDR_TEST_CMD__DDR_TEST_DATA_LANE_BIT_MARGIN  0x18
#define  DDR_TEST_CMD__DDR4_TUNE_PHY_VREF   0x19
#define  DDR_TEST_CMD__DDR4_TUNE_DRAM_VREF   0x1A
#define  DDR_TEST_CMD__DDR4_TUNE_AC_VREF   0x1b
#define  DDR_TEST_CMD__DDR4_SWEEP_DRAM_CLK_USE_D2PLL   0x1c
#define  DDR_TEST_CMD__DDR4_TEST_SHIFT_DDR_FREQUENCY  0x1d
#define  DDR_TEST_CMD__DDR4_TEST_DATA_WRTIE_READ  0x1e
#define  DDR_TEST_CMD__DDR_TEST_PWM_CMD              0x1f
#define  DDR_TEST_CMD__DDR_TEST_EE_SI             0x20
#define  DDR_TEST_CMD__DDR_TEST_VDDQ_SI             0x21
#define  DDR_TEST_CMD__DDR_TUNE_DDR_DATA_WINDOW_ENV            0x22
#define  DDR_TEST_CMD__DDR4_TEST_SHIFT_DDR_FREQUENCY_TXL  0x23
#define  DDR_TEST_CMD__DISPLAY_DDR_INFORMATION  0x24
#define  DDR_TEST_CMD__OFFSET_LCDLR  0x25
#define  DDR_TEST_CMD__SET_WATCH_DOG_VALUE  0x26
#define  DDR_TEST_CMD__DDR_TUNE_DDR_DATA_WINDOW_STICKY  0x27
#define  DDR_TEST_CMD__DDR4_SWEEP_DRAM_CLK_USE_D2PLL_STICKY  0x28
#define  DDR_TEST_CMD__DDR4_DDR_BIST_TEST_USE_D2PLL_STICKY  0x29
#define  DDR_TEST_CMD__DDR_SET_BIST_TEST_SIZE_STICKY_6  0x30
#define  DDR_TEST_CMD__DDR_SET_UBOOT_STORE_WINDOW  0x31
#define  DDR_TEST_CMD__DDR_SET_UBOOT_STORE_QUICK_WINDOW  0x32
#define  DDR_TEST_CMD__DDR_SET_UBOOT_KERNEL_STORE_QUICK_WINDOW  0x33
#define  DDR_TEST_CMD__DDR_SET_UBOOT_KERNEL_STORE_QUICK_WINDOW_MULTI  0x34
#define  DDR_TEST_CMD__DDR_SET_UBOOT_KERNEL_WINDOW_SAME_CHANGE  0x35
#define  DDR_TEST_CMD__DDR_SET_UBOOT_G12_RECONFIG_CMD  0x36
#define  DDR_TEST_CMD__DISPLAY_G12_DDR_INFORMATION  0x37
#define  DDR_TEST_CMD__DDR_G12_DMC_TEST  0x38
#define  DDR_TEST_CMD__DDR_G12_EE_BDLR_TEST  0x39

	unsigned int ddr_test_cmd = 0;
	unsigned int arg[30] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
	};
	char *endp;

	ddr_test_cmd = simple_strtoull_ddr(argv[1], &endp, 0);
	for (i = 2; i < argc; i++)
		arg[i - 2] = simple_strtoull_ddr(argv[i], &endp, 0);
	printf("\nddr_test_cmd== 0x%8x\n", ddr_test_cmd);

	for (i = 0; i < (argc - 2); i++)
		printf("\narg[%8x]=%8x\n", i, arg[i]);

	int argc2;
	char *argv2[30];

	argc2 = argc - 1;
	for (i = 1; i < (argc); i++)
		argv2[i - 1] = argv[i];
	{
		wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address),
		       dmc_retraining_ctrl & (~(1 << 31)));
		writel((0), p_ddr_base->ddr_dmc_apd_address);
		writel((0), p_ddr_base->ddr_dmc_asr_address);
		run_command("dcache off", 0);
		run_command("dcache on", 0);
		printf("\n cache off on");
		switch (ddr_test_cmd) {
		case (DDR_TEST_CMD__NONE):
		{
			printf("\n  0x0 help\n");
			printf("\n  0x1 ddrtest                            ");
			printf("ddr_test_cmd 0x1 start_add test_size loops  ");
			printf("\n  0x2 test aclcdlr                       ");
			printf("ddr_test_cmd 0x2 start_add test_size loops   ");
			printf("ddr_test_cmd 0x2 a 0 0x8000000  1");
			printf("\n  0x3 test max_pllclk                 ");
			printf("ddr_test_cmd 0x3  test_size loops add_freq sub_freq ");
			printf("\n  0x4 test zq                             ");
			printf("ddr_test_cmd 0x4  test_size loops add_freq sub_freq drv_odt_flag ");
			printf("\n  0x5 test vref                       ddr_test_cmd 0x5   ");
			printf("\n  0x6 test gxtvbb_crosstalk        ");
			printf("ddr_test_cmd 0x6  loops pattern_flag ");
			printf("\n  0x7 test bandwidth                  ");
			printf("ddr_test_cmd 0x7 size loops port sub_id timer_ms ");
			printf("\n  0x8 test lcdlr_use_env_uart     ");
			printf("ddr_test_cmd 0x8 input_src wr_adj_per[] rd_adj_per[][] ");
			printf("\n  0x9 test_reg_use_mask           ");
			printf("ddr_test_cmd 0x9 reg_add value mask  ");
			printf("\n  0xa test ac_clk                       ");
			printf("ddr_test_cmd 0xa start_add test_size loops  ");
			printf("ddr_test_cmd 0xa a 0 0x8000000  1  ");
			printf("\n  0xb ...  ");
			printf("\n  0xc ...  ");
			printf("\n  0xd ...  ");
			printf("\n  0xe ...  ");
			printf("\n  0xf ...  ");
			printf("\n  0x10 test set zq                             ");
			printf("ddr_test_cmd 0x10 zq0pr0 zq1pr0 zq2pr0   ");
			printf("\n  0x11 test tune dqs                            ");
			printf("ddr_test_cmd 0x11 a 0 test_size   ddr_test_cmd 0x11 a 0 0x80000");
			printf("\n  0x12 test set start_add                      ");
			printf("ddr_test_cmd 0x12 start_add   ");
			printf("\n  0x13 test ac_bit_setup_hold time       ");
			printf("ddr_test_cmd 0x13 a 0 size method  pin_id  ");
			printf("ddr_test_cmd 0x13 a 0 0x8000000 0  0xc");
			printf("\n  0x14 test data_bit_setup_hold time     ");
			printf("ddr_test_cmd 0x14 a 0 size setup/hold pin_id  ");
			printf("ddr_test_cmd 0x14 a 0 0x80000 0 3 ");
			printf("\n  0x15 test ac_lane_setup_hold            ");
			printf("ddr_test_cmd 0x15 a 0 size   ");
			printf("\n  0x16 test ee mdlr                            ");
			printf("ddr_test_cmd 0x16  voltage pwm_id loops   ");
			printf("\n  0x17 d2pll                                 ");
			printf("ddr_test_cmd 0x17 clk zq_ac zq_soc_dram soc_vref dram_vref");
			printf("dec_hex zq_vref 0\n");
			printf("example ddr_test_cmd 0x17 1200 0x2aa4a 0x2015995d 50 81 1 50\n");
			printf("or  ddr_test_cmd 0x17 1200 0x2aa4a 0x2015995d 0x09 0x20 0 50\n");
			printf("or  ddr_test_cmd 0x17 1200 6034 60346034 0 0 0 0 1\n");
			printf("\n  0x18 test data_lane_setup_hold         ");
			printf("ddr_test_cmd 0x18 a 0 size range start_pin_id end_pin_id ");
			printf("ddr_test_cmd 0x18 a 0 0x80000 1 0 96 ");
			printf("\n  0x19 test phy vref                            ");
			printf("ddr_test_cmd 0x19 a 0 0x80000  1 seed step vref_all vref_");
			printf("lcdlr_offset test_down_up_step seed_hex_dec ");
			printf("ddr_test_cmd 0x19 a 0 0x1000000  1  63  1 1  0x8 0 1 ");
			printf("\n  0x1a test dram vref                          ");
			printf("ddr_test_cmd 0x1A a 0 0x80000  clear seed step vref_all");
			printf("vref_lcdlr_offset test_down_up_step vref_range seed_hex_dec");
			printf("\n setenv  ddr_test_ddr4ram_vref");
			printf("ddr_test_cmd 0x1A a 0 0x0800000  0  0x14 1  0  0x8 0 0 0 ;");
			printf("setenv  storeboot  run ddr_test_ddr4ram_vref ;save;reset ");
			printf("\n  0x1b test ac vref                              ");
			printf("ddr_test_cmd 0x1B a 0 0x80000 ");
			printf("clear seed step vref_all vref_lcdlr_offset seed_hex_dec");
			printf("\n  0x1c sweep dram clk use d2pll_env    ");
			printf("ddr_test_cmd 0x1c  test_size start_freq end_freq test_loops ");
			printf("ddr_test_cmd 0x1c 0x8000000 800 1500 1");
			printf("\n  0x1d test shift clk                              ");
			printf("ddr_test_cmd 0x1d type delay_ms times");
			printf("\n  0x1e test write_read                         ");
			printf("ddr_test_cmd 0x1e write_read pattern_id loop start_add test_size");
			printf("\n  0x1f test pwm_cmd                          ");
			printf("ddr_test_cmd 0x1f pwmid   pwm_low pwm_high");
			printf("\n  0x22 test ddr_window use env         ");
			printf("ddr_test_cmd 0x22 a 0 test_size watchdog_time");
			printf("lane_disable_mask add_test_size ");
			printf("setenv bootcmd  ddr_test_cmd 0x22 a 0 0x800000 15 0 0x8000000");
			printf("\n defenv;save;");
			printf("setenv bootcmd ddr_test_cmd 0x22 a 0 0x800000 18 0x0 0x8000000");
			printf("\n setenv env_ddrtest_data_lane  0x22;save;reset");
			printf("\n  0x23 test shift ddr frequency          ddr_test_cmd 0x23");
			printf("\n  0x24 display ddr_information          ddr_test_cmd 0x24");
			printf("\n  0x25 offset ddr_lcdlr          ddr_test_cmd 0x25");
			printf("\n  0x26 set watchdog_value        ddr_test_cmd 0x26 30");
			printf("\n  0x27 test ddr_window use sticky register       ");
			printf("ddr_test_cmd 0x27 a 0 test_size watchdog_time");
			printf("lane_disable_mask add_test_size  setenv bootcmd ");
			printf("ddr_test_cmd 0x27 a 0 0x800000 15 0 0x8000000");
			printf("\n  0x28  sweep dram clk use d2pll_sticky    ");
			printf("ddr_test_cmd 0x28  test_size start_freq end_freq test_loops ");
			printf("ddr_test_cmd 0x28 0x8000000 800 1500 1");
		}
			wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address),
			       dmc_retraining_ctrl);
			//return 1;
			break;

		case (DDR_TEST_CMD__DDR_TEST):
		{
			do_ddr_test((cmd_tbl_t *)cmdtp, (int)flag, (int)argc2, (argv2));
			break;
		}
		case (DDR_TEST_CMD__DDR_TUNE_ACLCDLR):
			break;
		case (DDR_TEST_CMD__DDR_DDR_TUNE_AC_CLK):
			break;
		case (DDR_TEST_CMD__DDR_TUNE_ZQ):
			break;
		case (DDR_TEST_CMD__DDR_GXTVBB_CROSSTALK):
			break;
		case (DDR_TEST_CMD__DDR_BANDWIDTH_TEST):
			break;
		case (DDR_TEST_CMD__DDR_LCDLR_ENV_TUNE):
			break;
		case (DDR_TEST_CMD__DDR_MODIFY_REG_USE_MASK):
			break;
		case (DDR_TEST_CMD__DDR_SETZQ):
			break;

		case (DDR_TEST_CMD__DDR_TUNE_DDR_DATA_WINDOW_STICKY):
			break;
		case (DDR_TEST_CMD__DDR4_TEST_DATA_WRTIE_READ):
		{
			printf("\ntest ddr write read\n");

			do_ddr_test_write_read((cmd_tbl_t *)cmdtp,
					       (int)flag, (int)argc2, (argv2));
		}
		break;
		case (DDR_TEST_CMD__DDR_SET_UBOOT_G12_RECONFIG_CMD):
		{
			//printf("\nset do_ddr_uboot_reconfig cmd\n");

			//do_ddr_uboot_new_cmd((cmd_tbl_t *)cmdtp, (int)flag, (int)argc2, (argv2));
		}
		break;

		case (DDR_TEST_CMD__DISPLAY_G12_DDR_INFORMATION):
		{
			printf("\nshow g12 ddr information\n");
			if (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S5) {
				//if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)) {
				dmc_ddr_config_channel_id = 0;
				dmc_change_channel(dmc_ddr_config_channel_id);
				do_ddr_display_s5_ddr_information((cmd_tbl_t *)cmdtp,
								  (int)flag, (int)argc2, (argv2));
				dmc_ddr_config_channel_id = 2;
				dmc_change_channel(dmc_ddr_config_channel_id);
				do_ddr_display_s5_ddr_information((cmd_tbl_t *)cmdtp,
								  (int)flag, (int)argc2, (argv2));
				dmc_ddr_config_channel_id = 0;
				dmc_change_channel(dmc_ddr_config_channel_id);
				//}
			}
		}
		break;

		case (DDR_TEST_CMD__DDR_G12_DMC_TEST):
			break;
		case (DDR_TEST_CMD__DDR_G12_EE_BDLR_TEST):
		{
			printf("\nUboot BDLR test\n");

			do_ddr_test_pwm_bdlr((cmd_tbl_t *)cmdtp, (int)flag, (int)argc2, (argv2));
		}
		break;
		}
		writel(ddr_dmc_apd_temp_save, p_ddr_base->ddr_dmc_apd_address);
		writel(ddr_dmc_asr_temp_save, p_ddr_base->ddr_dmc_asr_address);
		wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
		return 1; //test_start_addr
	}

//usage:
//	cmd_usage(cmdtp);
	//wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
//	return 1;
}

U_BOOT_CMD(ddr_test_cmd, 30, 1, do_ddr_test_cmd,
	   "ddr_test_cmd cmd arg1 arg2 arg3...",
	   "ddr_test_cmd cmd arg1 arg2 arg3...\n dcache off ?\n");

//U_BOOT_CMD(ddr_auto_test_window, 30, 1, do_ddr_auto_test_window,
//	"ddr_test_cmd cmd arg1 arg2 arg3...",
//	"ddr_test_cmd cmd arg1 arg2 arg3...\n dcache off ?\n");

int do_ddr_auto_scan_drv(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	check_base_address();
#define  AUTO_SCAN_DDR3  0
#define  AUTO_SCAN_DDR4  1
#define  AUTO_SCAN_LPDDR3  2
#define  AUTO_SCAN_LPDDR4  3

#define  AUTO_SCAN_CONFIG_RANK0  0
#define  AUTO_SCAN_CONFIG_RANK01  1

	char *string_print_flag = " window-loop\n";

	global_boot_times = rd_reg((p_ddr_base->preg_sticky_reg0));

	printf("\nargc== 0x%8x\n", argc);
	printf("\nargc== 0x%8x\n", argc);
	int i;

	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);

	unsigned int ddr_type = 0;
	unsigned int ddr_channel_rank_config = 0;
	unsigned int loop = 0;
	char *endp;

	if (argc > 1) {
		ddr_type = simple_strtoull_ddr(argv[1], &endp, 0);
		if (*argv[1] == 0 || *endp != 0)
			ddr_type = 0;
	}
	if (argc > 2) {
		ddr_channel_rank_config = simple_strtoull_ddr(argv[2], &endp, 0);
		if (*argv[2] == 0 || *endp != 0)
			ddr_channel_rank_config = 0;
	}
	if (argc > 3) {
		loop = simple_strtoull_ddr(argv[3], &endp, 0);
		if (*argv[3] == 0 || *endp != 0)
			loop = 0;
	}
	//unsigned int temp_reg_add = 0;
	//{
	//	temp_reg_add = (((p_ddr_base->ddr_dmc_sticky0)));
	//}

	char str[1024] = "";

	unsigned int counter_loop = 0;
	unsigned int ddr_frequency = 0;
	unsigned int soc_data_drv_ohm_p = 0;
	//74//config soc data pin pull up driver strength,
	//select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned int soc_data_drv_ohm_n = 0;    //76
	unsigned int soc_data_odt_ohm_p = 0;
	//78//config soc data pin odt pull up strength,
	//select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned int soc_data_odt_ohm_n = 0;    //80
	unsigned int dram_data_drv_ohm = 0;
	//82//config dram data pin pull up pull down driver strength,
	//ddr3 select 34,40ohm,ddr4 select 34,48ohm,lpddr4 select 40,48,60,80,120,240ohm
	unsigned int dram_data_odt_ohm = 0;
	//84//config dram data pin odt pull up down strength,
	//ddr3 select 40,60,120ohm,ddr4 select 34,40,48,60,120,240ohm,
	//lpddr4 select 40,48,60,80,120,240ohm
	unsigned int dram_data_wr_odt_ohm = 0;  //174 char 1

	//i = 74 / 2;
	//soc_data_drv_ohm_p = ddr_rd_8_16bit_on_32reg(temp_reg_add, 16, i);
	//i = 76 / 2;
	//soc_data_drv_ohm_n = ddr_rd_8_16bit_on_32reg(temp_reg_add, 16, i);
	//i = 78 / 2;
	//soc_data_odt_ohm_p = ddr_rd_8_16bit_on_32reg(temp_reg_add, 16, i);
	//i = 80 / 2;
	//soc_data_odt_ohm_n = ddr_rd_8_16bit_on_32reg(temp_reg_add, 16, i);
	//i = 82 / 2;
	//dram_data_drv_ohm = ddr_rd_8_16bit_on_32reg(temp_reg_add, 16, i);
	//i = 84 / 2;
	//dram_data_odt_ohm = ddr_rd_8_16bit_on_32reg(temp_reg_add, 16, i);
	//i = 174 / 1;
	//dram_data_wr_odt_ohm = ddr_rd_8_16bit_on_32reg(temp_reg_add, 8, i);
	//i = 52 / 2;
	//ddr_frequency = ddr_rd_8_16bit_on_32reg(temp_reg_add, 16, i);
	if (global_boot_times == 1) {
		printf("\norg_global_boot_times== %d %s", global_boot_times, string_print_flag);
		printf("\nsoc_data_drv_ohm_p=%d %s", soc_data_drv_ohm_p, string_print_flag);
		printf("\nsoc_data_drv_ohm_n=%d %s", soc_data_drv_ohm_n, string_print_flag);
		printf("\nsoc_data_odt_ohm_p=%d %s", soc_data_odt_ohm_p, string_print_flag);
		printf("\nsoc_data_odt_ohm_n=%d %s", soc_data_odt_ohm_n, string_print_flag);
		printf("\ndram_data_drv_ohm=%d %s", dram_data_drv_ohm, string_print_flag);
		printf("\ndram_data_odt_ohm=%d %s", dram_data_odt_ohm, string_print_flag);
		printf("\ndram_data_wr_odt_ohm=%d %s", dram_data_wr_odt_ohm, string_print_flag);
	}
	unsigned int soc_data_drv_ohm_p_t[] = { 34, 40, 48, 60 };

	unsigned int dram_data_odt_ohm_t_ddr3[] = { 40, 60, 120 };              //ddr3
	unsigned int dram_data_odt_ohm_t_ddr4[] = { 40, 48, 60, 120 };          //ddr4
	unsigned int dram_data_odt_ohm_t_lpddr4[] = { 40, 48, 60, 120 };        //lpddr4

	unsigned int dram_data_drv_ohm_t_ddr3[] = { 34, 40 };                   //ddr3
	unsigned int dram_data_drv_ohm_t_ddr4[] = { 34, 48 };                   //ddr4
	unsigned int dram_data_drv_ohm_t_lpddr4[] = { 40, 48, 60 };             //lpddr4

	unsigned int soc_data_odt_ohm_p_t[] = { 40, 48, 60, 80, 120 };
	unsigned int soc_data_odt_ohm_n_t[] = { 40, 48, 60, 80, 120 };

	unsigned int dram_data_wr_odt_ohm_t_ddr3[] = { 60, 120, 0 };    //ddr3
	unsigned int dram_data_wr_odt_ohm_t_ddr4[] = { 80, 120, 0 };    //ddr4

	unsigned int *p_soc_data_drv_ohm_p = NULL;
	unsigned int *p_soc_data_odt_ohm_p = NULL;
	unsigned int *p_soc_data_odt_ohm_n = NULL;

	unsigned int *p_dram_data_drv_ohm = NULL;
	unsigned int *p_dram_data_odt_ohm = NULL;
	unsigned int *p_dram_data_wr_odt_ohm = NULL;

	p_soc_data_drv_ohm_p = soc_data_drv_ohm_p_t;
	p_soc_data_odt_ohm_p = soc_data_odt_ohm_p_t;
	p_soc_data_odt_ohm_n = soc_data_odt_ohm_n_t;

	p_dram_data_drv_ohm = dram_data_drv_ohm_t_ddr3;
	p_dram_data_odt_ohm = dram_data_odt_ohm_t_ddr3;
	p_dram_data_wr_odt_ohm = dram_data_wr_odt_ohm_t_ddr3;

	unsigned int max_counter_loop_w1 =
		(sizeof(soc_data_drv_ohm_p_t)) / (sizeof(soc_data_drv_ohm_p_t[0]));
	unsigned int max_counter_loop_w2 =
		(sizeof(dram_data_odt_ohm_t_ddr3)) / (sizeof(dram_data_odt_ohm_t_ddr3[0]));
	unsigned int max_counter_loop_r1 =
		(sizeof(dram_data_drv_ohm_t_ddr3)) / (sizeof(dram_data_drv_ohm_t_ddr3[0]));
	unsigned int max_counter_loop_r2 =
		(sizeof(soc_data_odt_ohm_p_t)) / (sizeof(soc_data_odt_ohm_p_t[0]));
	unsigned int max_counter_loop_wr1 = 1;

	if (ddr_channel_rank_config)
		max_counter_loop_wr1 =
			(sizeof(dram_data_wr_odt_ohm_t_ddr3)) /
			(sizeof(dram_data_wr_odt_ohm_t_ddr3[0]));
	if (ddr_type == AUTO_SCAN_DDR4) {
		p_dram_data_drv_ohm = dram_data_drv_ohm_t_ddr4;
		p_dram_data_odt_ohm = dram_data_odt_ohm_t_ddr4;
		p_dram_data_wr_odt_ohm = dram_data_wr_odt_ohm_t_ddr4;
		max_counter_loop_w2 =
			(sizeof(dram_data_odt_ohm_t_ddr4)) / (sizeof(dram_data_odt_ohm_t_ddr4[0]));
		max_counter_loop_r1 =
			(sizeof(dram_data_drv_ohm_t_ddr4)) / (sizeof(dram_data_drv_ohm_t_ddr4[0]));

		if (ddr_channel_rank_config)
			max_counter_loop_wr1 =
				(sizeof(dram_data_wr_odt_ohm_t_ddr4)) /
				(sizeof(dram_data_wr_odt_ohm_t_ddr4[0]));
	}
	if (ddr_type == AUTO_SCAN_LPDDR4) {
		p_dram_data_drv_ohm = dram_data_drv_ohm_t_lpddr4;
		p_dram_data_odt_ohm = dram_data_odt_ohm_t_lpddr4;
		max_counter_loop_w2 = (sizeof(dram_data_odt_ohm_t_lpddr4)) /
				      (sizeof(dram_data_odt_ohm_t_lpddr4[0]));
		max_counter_loop_r1 = (sizeof(dram_data_drv_ohm_t_lpddr4)) /
				      (sizeof(dram_data_drv_ohm_t_lpddr4[0]));
		max_counter_loop_r2 =
			(sizeof(soc_data_odt_ohm_n_t)) / (sizeof(soc_data_odt_ohm_n_t[0]));

		if (ddr_channel_rank_config)
			max_counter_loop_wr1 = 1;
	}

	unsigned int max_counter_total =
		(max_counter_loop_w1 * max_counter_loop_w2 + max_counter_loop_r1 *
		 max_counter_loop_r2 + 2) * max_counter_loop_wr1;
	//add 2 times recover
	//each array test 2 times ,for maybe 1times will fail,then next time will recovery
	// 20181114
	counter_loop = (((global_boot_times - 1) / 2) % max_counter_total);
	dram_data_wr_odt_ohm = 0;
	if (max_counter_loop_wr1 > 1)
		dram_data_wr_odt_ohm = p_dram_data_wr_odt_ohm[(counter_loop /
							       (max_counter_loop_w1 *
								max_counter_loop_w2 +
								max_counter_loop_r1 *
								max_counter_loop_r2 + 2))];
	if ((counter_loop % (max_counter_loop_w1 * max_counter_loop_w2 +
			     max_counter_loop_r1 * max_counter_loop_r2 + 2)) <
	    (max_counter_loop_w1 * max_counter_loop_w2)) {
		soc_data_drv_ohm_p = p_soc_data_drv_ohm_p[(((counter_loop %
							     (max_counter_loop_w1 *
							      max_counter_loop_w2 +
							      max_counter_loop_r1 *
							      max_counter_loop_r2 + 2))) %
							   max_counter_loop_w1)];
		soc_data_drv_ohm_n = soc_data_drv_ohm_p;
		dram_data_odt_ohm = p_dram_data_odt_ohm[(((counter_loop %
							   (max_counter_loop_w1 *
							    max_counter_loop_w2 +
							    max_counter_loop_r1 *
							    max_counter_loop_r2 + 2))) /
							 max_counter_loop_w1) %
							max_counter_loop_w2];
	} else if ((counter_loop % (max_counter_loop_w1 * max_counter_loop_w2 +
				    max_counter_loop_r1 * max_counter_loop_r2 + 2)) ==
		   ((max_counter_loop_w1 * max_counter_loop_w2) + 0)) {
		ddr_test_watchdog_reset_system();
	} else if ((counter_loop % (max_counter_loop_w1 * max_counter_loop_w2 +
				    max_counter_loop_r1 * max_counter_loop_r2 + 2)) <
		   (max_counter_loop_w1 * max_counter_loop_w2 +
		    max_counter_loop_r1 * max_counter_loop_r2 + 2 - 1)) {
		dram_data_drv_ohm = p_dram_data_drv_ohm[((((counter_loop %
							    (max_counter_loop_w1 *
							     max_counter_loop_w2 +
							     max_counter_loop_r1 *
							     max_counter_loop_r2 + 2))) -
							  (max_counter_loop_w1 *
							   max_counter_loop_w2 + 1)) %
							 max_counter_loop_r1)];
		soc_data_odt_ohm_p = p_soc_data_odt_ohm_p[((((counter_loop %
							      (max_counter_loop_w1 *
							       max_counter_loop_w2 +
							       max_counter_loop_r1 *
							       max_counter_loop_r2 + 2))) -
							    (max_counter_loop_w1 *
							     max_counter_loop_w2 + 1)) /
							   max_counter_loop_r1)];
		soc_data_odt_ohm_n = 0;
		if (ddr_type == CONFIG_DDR_TYPE_LPDDR4) {
			soc_data_odt_ohm_p = 0;
			soc_data_odt_ohm_n =
				p_soc_data_odt_ohm_n[((((counter_loop % (max_counter_loop_w1 *
									 max_counter_loop_w2 +
									 max_counter_loop_r1 *
									 max_counter_loop_r2 +
									 2))) -
						       (max_counter_loop_w1 * max_counter_loop_w2 +
							1)) / max_counter_loop_r1)];
		}
	} else if ((counter_loop % (max_counter_loop_w1 * max_counter_loop_w2 +
				    max_counter_loop_r1 * max_counter_loop_r2 + 2)) ==
		   (max_counter_loop_w1 * max_counter_loop_w2 +
		    max_counter_loop_r1 * max_counter_loop_r2 + 2 - 1)) {
		ddr_test_watchdog_reset_system();
	}
	printf("\nglobal_boot_times== %d %s", global_boot_times, string_print_flag);

	if (loop)
		if (((global_boot_times - 1) / 2) > max_counter_total)
			return 1;
	printf("\nmax_counter=%d  %d %s", max_counter_total, max_counter_total * 2,
	       string_print_flag);
	printf("\nsoc_data_drv_ohm_p=%d %s", soc_data_drv_ohm_p, string_print_flag);
	printf("\nsoc_data_drv_ohm_n=%d %s", soc_data_drv_ohm_n, string_print_flag);
	printf("\nsoc_data_odt_ohm_p=%d %s", soc_data_odt_ohm_p, string_print_flag);
	printf("\nsoc_data_odt_ohm_n=%d %s", soc_data_odt_ohm_n, string_print_flag);
	printf("\ndram_data_drv_ohm=%d %s", dram_data_drv_ohm, string_print_flag);
	printf("\ndram_data_odt_ohm=%d %s", dram_data_odt_ohm, string_print_flag);
	printf("\ndram_data_wr_odt_ohm=%d %s", dram_data_wr_odt_ohm, string_print_flag);
	{
		if (soc_data_drv_ohm_p) {
			sprintf(str, "ddr_test_cmd 0x36 0x20180030 0x1 74 %d 2  0   ",
				soc_data_drv_ohm_p);
			printf("\nstr=%s\n", str);
			run_command(str, 0);
		}
		if (soc_data_drv_ohm_n) {
			sprintf(str, "ddr_test_cmd 0x36 0x20180030 0x1 76 %d 2  0   ",
				soc_data_drv_ohm_n);
			printf("\nstr=%s\n", str);
			run_command(str, 0);
		}
		if (soc_data_odt_ohm_p) {
			sprintf(str, "ddr_test_cmd 0x36 0x20180030 0x1 78 %d 2  0   ",
				soc_data_odt_ohm_p);
			printf("\nstr=%s\n", str);
			run_command(str, 0);
		}
		if (soc_data_odt_ohm_n) {
			sprintf(str, "ddr_test_cmd 0x36 0x20180030 0x1 80 %d 2  0   ",
				soc_data_odt_ohm_n);
			printf("\nstr=%s\n", str);
			run_command(str, 0);
		}
		if (dram_data_drv_ohm) {
			sprintf(str, "ddr_test_cmd 0x36 0x20180030 0x1 82 %d 2  0   ",
				dram_data_drv_ohm);
			printf("\nstr=%s\n", str);
			run_command(str, 0);
		}
		if (dram_data_odt_ohm) {
			sprintf(str, "ddr_test_cmd 0x36 0x20180030 0x1 84 %d 2  0   ",
				dram_data_odt_ohm);
			printf("\nstr=%s\n", str);
			run_command(str, 0);
		}
		{
			sprintf(str, "ddr_test_cmd 0x36 0x20180030 0x1 174 %d 1  0   ",
				dram_data_wr_odt_ohm);
			printf("\nstr=%s\n", str);
			run_command(str, 0);
		}

		sprintf(str, "g12_d2pll %d  0x12  0x6 0 0x0 0 0 0 0x800000    0 1   ",
			ddr_frequency);
		printf("\nstr=%s\n", str);
		run_command(str, 0);
	}

	return 1;
}

unsigned char temp_sha2[SHA_SUM_LEN];
int do_verify_flash_ddr_parameter(char log_level)
{
	unsigned count = 0;
	unsigned error = 0;

	//unsigned ret = 0;

	//if (((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_C2) &&
	//     (p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_T5D)) ||
	//    (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S5)
	//    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)) {
	char temp_buf[((sizeof(ddr_sha_t_s5) + 511) / 512) * 512] = { 0 };
	unsigned ddr_set_size = 0;

	ddr_set_size = sizeof(ddr_sha_t_s5);
//#if 1
	uint32_t loop = 0;

	//for C2 T5/T5D ddr window fast boot no support dfe vref1
	//if (((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_C2) &&
	//     (p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_T5D))) {
	ddr_set_size = sizeof(ddr_set_ps0_only_t) - (44 * 2);
	//sizeof(board_phase_setting_ps_t.soc_bit_vref_dac1);
	unsigned char *sha_t_chip_id;

	sha_t_chip_id = (unsigned char *)((uint64_t)(&ddr_sha_s5.sha_chip_id) -
					  (44 * 2));
	for (loop = 0; loop < MESON_CPU_CHIP_ID_SIZE; loop++)                 //update chip id
		ddr_sha_s5.sha_chip_id[loop] = sha_t_chip_id[loop];
	//	}
//#endif

#ifdef USE_FOR_UBOOT_2018
	extern int store_rsv_read(const char *name, size_t size, void *buf);

	store_rsv_read("ddr-parameter", ((sizeof(ddr_sha_t_s5) + 511) / 512) * 512,
		       (uint8_t *)(temp_buf));

#else
	extern int store_ddr_parameter_read(uint8_t *buffer, uint32_t length);

	store_ddr_parameter_read((uint8_t *)(temp_buf), ((sizeof(ddr_sha_t_s5) +
							  511) / 512) * 512);
#endif
	char *s = temp_buf;
	char *d = (char *)(&ddr_sha_s5);

	for (count = 0; count < sizeof(ddr_sha_t_s5); count++) {
		*d = *s;
		s++;
		d++;
	}

	for (count = 0; count < SHA_SUM_LEN; count++)
		temp_sha2[count] = ddr_sha_s5.sha2[count];

	sha256_csum_wd_internal((uint8_t *)(&ddr_sha_s5.ddrs), ddr_set_size,
				ddr_sha_s5.sha2, 0);

	for (count = 0; count < SHA_SUM_LEN; count++) {
		if (temp_sha2[count] != ddr_sha_s5.sha2[count])
			error++;

		if (log_level == 0)
			printf("\nread sha[%8x] %8x,calu %8x", count,
			       (ddr_sha_s5.sha2[count]), (temp_sha2[count]));
	}

#define DDR_FUNC_FAST_BOOT_CHECK_CHIP_ID    BIT(30)
	{
		for (count = 0; count < MESON_CPU_CHIP_ID_SIZE; count++) {
			if (ddr_sha_s5.sha_chip_id[count] != global_chip_id[count])
				error++;

			if (log_level == 0)
				printf("\nglobal_chip_id[%8x] %8x,read %8x", count,
				       global_chip_id[count], ddr_sha_s5.sha_chip_id[count]);
		}
	}

	return error;
	//}

#define DDR_FUNC_FAST_BOOT_CHECK_CHIP_ID    BIT(30)

	if (ddr_sha_s5.ddrs.cfg_board_common_setting.ddr_func &
	    DDR_FUNC_FAST_BOOT_CHECK_CHIP_ID) {
		for (count = 0; count < MESON_CPU_CHIP_ID_SIZE; count++) {
			if (ddr_sha_s5.sha_chip_id[count] != global_chip_id[count])
				error++;

			if (log_level == 0)
				printf("\nglobal_chip_id[%8x] %8x,read %8x",
				       count, (global_chip_id[count]),
				       ddr_sha_s5.sha_chip_id[count]);
		}
	}
	return error;
} /* do_verify_flash_ddr_parameter */

uint32_t enable_ddr_check_boot_reason;
uint32_t boot_reason;
#define HOT_BOOT_MAGIC         0x99887766
#define HOT_BOOT_STICKY_ADD    (p_ddr_base->preg_sticky_reg0 + (6 << 2))
#define AMLOGIC_COLD_BOOT      0
int do_ddr_auto_fastboot_check_s5(char auto_window_test_enable_item,
				  uint32_t auto_window_test_dq_size, char pattern_dis_scramble,
				  uint32_t stick_dmc_ddr_window_test_read_vref_offset_value,
				  uint32_t skip_window_test_enable)
{
	uint32_t ddr_set_size = 0;
	uint32_t need_ddr_window_test = 0;
	char str[1024] = "";
	int verify_error = 0;

	verify_error = do_verify_flash_ddr_parameter(1);

	if (verify_error == 0) {
		if (ddr_sha_s5.ddrs.cfg_board_common_setting.fast_boot[0] == 0xff) {
			printf("\nuboot  auto fast boot check flash data is ok return\n");
			return 1;
		}
	}
	enable_ddr_check_boot_reason = 0;
	enable_ddr_check_boot_reason = env_to_a_num("enable_ddr_check_boot_reason");
	printf("\nenable_ddr_check_boot_reason==%d\n", enable_ddr_check_boot_reason);
	printf("\nddr_fast_boot_enable_flag==%d\n", env_to_a_num("ddr_fast_boot_enable_flag"));
	ddr_set_ps0_only_t *ddr_set_t_p = NULL;

	ddr_set_t_p = (ddr_set_ps0_only_t *)(ddr_set_t_p_array_s5);
	uint32_t ddr_set_add = 0;

	ddr_set_add = (uint32_t)(uint64_t)(ddr_set_t_p);
	ddr_set_size = sizeof(ddr_set_ps0_only_t);
	printf("\nddr_set_t_p=0x%8x\n", ddr_set_add);

	uint32_t dmc_retraining_ctrl = 0;

	dmc_retraining_ctrl = rd_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address);
	wr_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address, dmc_retraining_ctrl & (~(1 << 31)));
	uint32_t write_size = 0;

	do_read_s5_ddr_training_data(1, ddr_set_t_p);
//#if 1
	uint32_t loop = 0;
	//for C2 T5/T5D ddr window fast boot no support dfe vref1
	//if (((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_C2) &&
	//     (p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_T5D))) {
	ddr_set_size = sizeof(ddr_set_ps0_only_t) - (44 * 2);
	//sizeof(board_phase_setting_ps_t.soc_bit_vref_dac1);
	unsigned char *sha_t_chip_id;

	sha_t_chip_id = (unsigned char *)((uint64_t)(&ddr_sha_s5.sha_chip_id) - (44 * 2));
	for (loop = 0; loop < MESON_CPU_CHIP_ID_SIZE; loop++)         //update chip id
		sha_t_chip_id[loop] = global_chip_id[loop];
	//}
//#endif
	write_size = ((ddr_set_size + SHA_SUM_LEN + MESON_CPU_CHIP_ID_SIZE + 511) / 512) * 512;

	if ((ddr_set_t_p->cfg_board_common_setting.fast_boot[3] & 0xc0) &&
	    (ddr_set_t_p->cfg_board_common_setting.fast_boot[3] & 0x3f)) {
		enable_ddr_check_boot_reason = 0;

		if (ddr_set_t_p->cfg_board_common_setting.fast_boot[0] > 0 &&
		    ddr_set_t_p->cfg_board_common_setting.fast_boot[0] < 0xff) {
			ddr_set_t_p->cfg_board_common_setting.fast_boot[0] = 0xfe;
			verify_error = 0;
		}
	}

	if (ddr_set_t_p->cfg_board_common_setting.fast_boot[0]) {
		printf("\nuboot enable auto fast boot function\n");

		if (verify_error) {
			printf("\nuboot  auto fast boot check verify data happen wrong\n");
			ddr_set_t_p->cfg_board_common_setting.fast_boot[0] = 1;
		}
	} else {
		wr_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address, dmc_retraining_ctrl);
		return 1;
	}

	if (enable_ddr_check_boot_reason) {
		boot_reason = 0; //ddr_set_t_p->boot_reason
		{
			if ((((readl(p_ddr_base->ddr_boot_reason_address) >> 0) & 0xf) ==
			     AMLOGIC_COLD_BOOT) && rd_reg(HOT_BOOT_STICKY_ADD) == HOT_BOOT_MAGIC)
				boot_reason = 1;

			if (ddr_set_t_p->cfg_board_common_setting.fast_boot[0] == 0xfe)
				boot_reason = 2;
		}
		printf("\nboot_reason=%8x\n", (boot_reason));
		printf("\nHOT_BOOT_STICKY_ADD=%8x\n", (rd_reg(HOT_BOOT_STICKY_ADD)));
		wr_reg(HOT_BOOT_STICKY_ADD, HOT_BOOT_MAGIC);
		printf("\nHOT_BOOT_STICKY_ADD=%8x\n", (rd_reg(HOT_BOOT_STICKY_ADD)));

		if (boot_reason == 0) {
			if (ddr_set_t_p->cfg_board_common_setting.fast_boot[0] < 0xfe) {
				need_ddr_window_test = 1;
				sprintf(str, "setenv initargs ${initargs} need_ddr_window_test=%d",
					need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
				sprintf(str, "setenv bootargs ${bootargs} need_ddr_window_test=%d",
					need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
			} else {
				sprintf(str, "setenv initargs ${initargs} need_ddr_window_test=%d",
					need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
				sprintf(str, "setenv bootargs ${bootargs} need_ddr_window_test=%d",
					need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
			}
			wr_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address,
			       dmc_retraining_ctrl);
			return 1;
		}
		if (boot_reason != 0) {
			sprintf(str, "setenv initargs ${initargs} need_ddr_window_test=%d",
				need_ddr_window_test);
			printf("\nstr=%s\n", str);
			run_command(str, 0);
			sprintf(str, "setenv bootargs ${bootargs} need_ddr_window_test=%d",
				need_ddr_window_test);
			printf("\nstr=%s\n", str);
			run_command(str, 0);
		}
	}

	if (ddr_set_t_p->cfg_board_common_setting.fast_boot[0] == 0xff) {
		printf("\nuboot  auto fast boot  auto window test is done\n");
		wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
		return 1;
	}

	if (skip_window_test_enable) {
		printf("enable skip window test fast boot mode!\n");
		ddr_set_t_p->cfg_board_common_setting.fast_boot[0] = 0xfe;
	}
	printf("\n(ddr_set_t_p->cfg_board_common_setting.fast_boot[0])=0x%8x\n",
	       ddr_set_t_p->cfg_board_common_setting.fast_boot[0]);

	if (ddr_set_t_p->cfg_board_common_setting.fast_boot[0] < 0xfe) {
		printf("\nuboot  auto fast boot  auto window test begin\n");
		{
			ddr_set_t_p->cfg_board_common_setting.fast_boot[0] = 0xfd;
			//0xfd for check unexcept power off status
			sha256_csum_wd_internal((unsigned char *)(uint64_t)ddr_set_add,
						ddr_set_size, ddr_sha_s5.sha2, 0);
			write_size = ((ddr_set_size + SHA_SUM_LEN +
				       MESON_CPU_CHIP_ID_SIZE + 511) / 512) * 512;
			{
				ddr_do_store_ddr_parameter_ops((uint8_t *)(uint64_t)(ddr_set_add -
										     SHA_SUM_LEN),
							       write_size);
			}
			sprintf(str, "g12_d2pll %d 0x11 %d 0 0 0 0 %d 0x%8x  0 %d",
				ddr_set_t_p->cfg_board_SI_setting_ps.DRAMFreq,
				auto_window_test_enable_item,
				stick_dmc_ddr_window_test_read_vref_offset_value,
				auto_window_test_dq_size, pattern_dis_scramble);
			printf("\nstr=%s\n", str);

			run_command(str, 0);
		}
		return 1;
	}

	if (ddr_set_t_p->cfg_board_common_setting.fast_boot[0] == 0xfe) {
		ddr_set_t_p->cfg_board_common_setting.fast_boot[0] = 0xff;
		{
			printf("\nuboot  auto fast boot  auto window test finish\n");
			char fast_boot_t2 = 0;

			fast_boot_t2 = ddr_set_t_p->cfg_board_common_setting.fast_boot[2];
			if (fast_boot_t2) {
				if (fast_boot_t2 & 0x7) {
					if (fast_boot_t2 & 0x8)
						sprintf(str, "ddr_g12_offset_data 3 0x0  0 0 1 %d ",
							fast_boot_t2 & 0x7);
					else
						sprintf(str, "ddr_g12_offset_data 3 0x0  0 0 2 %d ",
							fast_boot_t2 & 0x7);
					printf("\nstr=%s\n", str);

					run_command(str, 0);
				}

				if (fast_boot_t2 & 0x70) {
					if ((fast_boot_t2 & 0x80))
						sprintf(str, "ddr_g12_offset_data 2 0x0  0 0 1 %d ",
							(fast_boot_t2 >> 4) & 0x7);
					else
						sprintf(str, "ddr_g12_offset_data 2 0x0  0 0 2 %d ",
							(fast_boot_t2 >> 4) & 0x7);
					printf("\nstr=%s\n", str);

					run_command(str, 0);
				}
				sprintf(str, "ddr_fast_boot 1 ");
				printf("\nstr=%s\n", str);
				run_command(str, 0);
			} else {
				sha256_csum_wd_internal((unsigned char *)(uint64_t)ddr_set_add,
							ddr_set_size, ddr_sha_s5.sha2, 0);
				ddr_do_store_ddr_parameter_ops((uint8_t *)(uint64_t)(ddr_set_add -
										     SHA_SUM_LEN),
							       write_size);
			}

			if (enable_ddr_check_boot_reason) {
				if (boot_reason) {
					sprintf(str, "systemoff");
					printf("\nstr=%s\n", str);
					run_command(str, 0);
				}
			}
			wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address),
			       dmc_retraining_ctrl);
			return 1;
		}
	}
	wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
	return 1;
} /* do_ddr_auto_fastboot_check_s5 */

int do_ddr_auto_fastboot_check(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	check_base_address();
#define DMC_TEST_SLT_ENABLE_DDR_AUTO_FAST_BOOT BIT(5)
#define  AUTO_WINDOW_TEST_ENABLE_ITEM   ((1 << 1) | (1 << 2))
	char *endp;
	char auto_window_test_enable_item = DMC_TEST_SLT_ENABLE_DDR_AUTO_FAST_BOOT;
	uint32_t auto_window_test_dq_size = 0;
	char pattern_dis_scramble = 0;
	uint32_t stick_dmc_ddr_window_test_read_vref_offset_value = 0;
	uint32_t ddr_set_size = 0;
	uint32_t need_ddr_window_test = 0;
	uint32_t skip_window_test_enable = 0;

	if (argc > 1) {
		auto_window_test_enable_item = simple_strtoull_ddr(argv[1], &endp, 0);
		if (*argv[1] == 0 || *endp != 0)
			auto_window_test_enable_item = DMC_TEST_SLT_ENABLE_DDR_AUTO_FAST_BOOT;
	}
	if (argc > 2) {
		auto_window_test_dq_size = simple_strtoull_ddr(argv[2], &endp, 0);
		if (*argv[2] == 0 || *endp != 0)
			auto_window_test_dq_size = 0;
	}
	if (argc > 3) {
		pattern_dis_scramble = simple_strtoull_ddr(argv[3], &endp, 0);
		if (*argv[3] == 0 || *endp != 0)
			pattern_dis_scramble = 0;
	}
	if (argc > 4) {
		stick_dmc_ddr_window_test_read_vref_offset_value =
			simple_strtoull_ddr(argv[4],
					    &endp, 0);
		if (*argv[4] == 0 || *endp != 0)
			stick_dmc_ddr_window_test_read_vref_offset_value = 0;
	}
	if (argc > 5) {
		skip_window_test_enable = simple_strtoull_ddr(argv[5], &endp, 0);
		if (*argv[5] == 0 || *endp != 0)
			skip_window_test_enable = 0;
	}
	//if (((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_C2) &&
	//     (p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_T5D)) ||
	//    (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S4)
	//    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)) {
	do_ddr_auto_fastboot_check_s5(auto_window_test_enable_item,
				      auto_window_test_dq_size, pattern_dis_scramble,
				      stick_dmc_ddr_window_test_read_vref_offset_value,
				      skip_window_test_enable);
	return 1;
	//}
	char str[1024] = "";
	int verify_error = 0;

	verify_error = do_verify_flash_ddr_parameter(1);
	if (verify_error == 0) {
		if (ddr_sha_s5.ddrs.cfg_board_common_setting.fast_boot[0] == 0xff) {
			printf("\nuboot  auto fast boot check flash data is ok return\n");
			return 1;
		}
	}
	enable_ddr_check_boot_reason = 0;
	enable_ddr_check_boot_reason = env_to_a_num("enable_ddr_check_boot_reason");
	printf("\nenable_ddr_check_boot_reason==%d\n", enable_ddr_check_boot_reason);
	printf("\nddr_fast_boot_enable_flag==%d\n", env_to_a_num("ddr_fast_boot_enable_flag"));
	ddr_set_ps0_only_t *ddr_set_t_p = NULL;

	ddr_set_t_p = &ddr_set_array[0]; //(ddr_set_t *)(ddr_set_t_p_array);
	uint32_t ddr_set_add = 0;

	ddr_set_add = (uint32_t)(uint64_t)(ddr_set_t_p);
	ddr_set_size = sizeof(ddr_set_t);
	printf("\nddr_set_t_p=0x%8x\n", ddr_set_add);

	uint32_t dmc_retraining_ctrl = 0;

	dmc_retraining_ctrl = rd_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address);
	wr_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address, dmc_retraining_ctrl & (~(1 << 31)));
	uint32_t write_size = 0;

	write_size = ((ddr_set_size + SHA_SUM_LEN + MESON_CPU_CHIP_ID_SIZE + 511) / 512) * 512;
	//do_read_ddr_training_data(1, ddr_set_t_p);
	do_read_s5_ddr_training_data(0, ddr_set_t_p);
	if ((ddr_set_t_p->cfg_board_common_setting.fast_boot[3] & 0xc0) &&
	    (ddr_set_t_p->cfg_board_common_setting.fast_boot[3] & 0x3f)) {
		enable_ddr_check_boot_reason = 0;
		if (ddr_set_t_p->cfg_board_common_setting.fast_boot[0] > 0 &&
		    ddr_set_t_p->cfg_board_common_setting.fast_boot[0] < 0xff) {
			ddr_set_t_p->cfg_board_common_setting.fast_boot[0] = 0xfe;
			verify_error = 0;
		}
	}
	if (ddr_set_t_p->cfg_board_common_setting.fast_boot[0]) {
		printf("\nuboot enable auto fast boot function\n");
		if (verify_error) {
			printf("\nuboot  auto fast boot check verify data happen wrong\n");
			ddr_set_t_p->cfg_board_common_setting.fast_boot[0] = 1;
		}
	} else {
		wr_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address, dmc_retraining_ctrl);
		return 1;
	}

	if (enable_ddr_check_boot_reason) {
		boot_reason = 0; //ddr_set_t_p->boot_reason
		{
			if ((((readl(p_ddr_base->ddr_boot_reason_address) >> 12) & 0xf) ==
			     AMLOGIC_COLD_BOOT) && rd_reg(HOT_BOOT_STICKY_ADD) == HOT_BOOT_MAGIC)
				boot_reason = 1;
			if (ddr_set_t_p->cfg_board_common_setting.fast_boot[0] == 0xfe)
				boot_reason = 2;
		}
		printf("\nboot_reason=%8x\n", (boot_reason));
		printf("\nHOT_BOOT_STICKY_ADD=%8x\n", rd_reg(HOT_BOOT_STICKY_ADD));
		wr_reg(HOT_BOOT_STICKY_ADD, HOT_BOOT_MAGIC);
		printf("\nHOT_BOOT_STICKY_ADD=%8x\n", rd_reg(HOT_BOOT_STICKY_ADD));

		if (boot_reason == 0) {
			if (ddr_set_t_p->cfg_board_common_setting.fast_boot[0] < 0xfe) {
				need_ddr_window_test = 1;
				sprintf(str, "setenv initargs ${initargs} need_ddr_window_test=%d",
					need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
				sprintf(str, "setenv bootargs ${bootargs} need_ddr_window_test=%d",
					need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
			} else {
				sprintf(str, "setenv initargs ${initargs} need_ddr_window_test=%d",
					need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
				sprintf(str, "setenv bootargs ${bootargs} need_ddr_window_test=%d",
					need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
			}
			wr_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address,
			       dmc_retraining_ctrl);
			return 1;
		}
		if (boot_reason != 0)  {
			sprintf(str, "setenv initargs ${initargs} need_ddr_window_test=%d",
				need_ddr_window_test);
			printf("\nstr=%s\n", str);
			run_command(str, 0);
			sprintf(str, "setenv bootargs ${bootargs} need_ddr_window_test=%d",
				need_ddr_window_test);
			printf("\nstr=%s\n", str);
			run_command(str, 0);
		}
	}
	if (ddr_set_t_p->cfg_board_common_setting.fast_boot[0] == 0xff) {
		printf("\nuboot  auto fast boot  auto window test is done\n");
		wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
		return 1;
	}
	if (skip_window_test_enable) {
		printf("enable skip window test fast boot mode!\n");
		ddr_set_t_p->cfg_board_common_setting.fast_boot[0] = 0xfe;
	}
	printf("\n(ddr_set_t_p->cfg_board_common_setting.fast_boot[0])=0x%8x\n",
	       (ddr_set_t_p->cfg_board_common_setting.fast_boot[0]));

	if (ddr_set_t_p->cfg_board_common_setting.fast_boot[0] < 0xfe) {
		printf("\nuboot  auto fast boot  auto window test begin\n");
		{
			ddr_set_t_p->cfg_board_common_setting.fast_boot[0] = 0xfd;
			//0xfd for check unexcept power off status
			sha256_csum_wd_internal((unsigned char *)(uint64_t)ddr_set_add,
						sizeof(ddr_set_t), ddr_sha_s5.sha2, 0);
			write_size = ((ddr_set_size + SHA_SUM_LEN + MESON_CPU_CHIP_ID_SIZE +
				       511) / 512) * 512;
			//{
			ddr_do_store_ddr_parameter_ops((uint8_t *)(unsigned long)(ddr_set_add -
										  SHA_SUM_LEN),
						       write_size);
			//}
			sprintf(str, "g12_d2pll %d 0x11 %d 0 0 0 0 %d 0x%8x  0 %d",
				ddr_set_t_p->cfg_board_SI_setting_ps.DRAMFreq,
				auto_window_test_enable_item,
				stick_dmc_ddr_window_test_read_vref_offset_value,
				auto_window_test_dq_size, pattern_dis_scramble);
			printf("\nstr=%s\n", str);

			run_command(str, 0);
		}
		return 1;
	}

	if (ddr_set_t_p->cfg_board_common_setting.fast_boot[0] == 0xfe) {
		char dmc_test_worst_window_rx = 0;
		char dmc_test_worst_window_tx = 0;

		{
			dwc_ddrphy_apb_wr((0 << 20) | (0xd << 16) | (0 << 12) | (0x0), 0);
			// DWC_DDRPHYA_APBONLY0_MicroContMuxSel

			dmc_test_worst_window_tx =
				dwc_ddrphy_apb_rd((0 << 20) | (1 << 16) | (0 << 12) | (0x0c2));
			dmc_test_worst_window_rx =
				dwc_ddrphy_apb_rd((0 << 20) | (1 << 16) | (0 << 12) | (0x0c3));
			printf("\ndmc_test_worst_window_tx =%d\n", dmc_test_worst_window_tx);
			printf("\ndmc_test_worst_window_rx=%d\n", dmc_test_worst_window_rx);
			if (dmc_test_worst_window_tx > 30)
				dmc_test_worst_window_tx = 30;
			if (dmc_test_worst_window_rx > 30)
				dmc_test_worst_window_rx = 30;
			ddr_set_t_p->cfg_board_common_setting.fast_boot[1] =
				(((dmc_test_worst_window_tx / 2) << 4)) |
				(((dmc_test_worst_window_rx / 2)));
		}

		ddr_set_t_p->cfg_board_common_setting.fast_boot[0] = 0xff;
		//{
		printf("\nuboot  auto fast boot  auto window test finish\n");
		char fast_boot_t2 = 0;

		fast_boot_t2 = ddr_set_t_p->cfg_board_common_setting.fast_boot[2];
		if (fast_boot_t2) {
			if (fast_boot_t2 & 0x7) {
				if ((fast_boot_t2 & 0x8))
					sprintf(str, "ddr_g12_offset_data 3 0x0  0 0 1 %d ",
						fast_boot_t2 & 0x7);
				else
					sprintf(str, "ddr_g12_offset_data 3 0x0  0 0 2 %d ",
						fast_boot_t2 & 0x7);

				printf("\nstr=%s\n", str);

				run_command(str, 0);
			}
			if (fast_boot_t2 & 0x70) {
				if ((fast_boot_t2 & 0x80))
					sprintf(str,
						"ddr_g12_offset_data 2 0x0  0 0 1 %d ",
						(fast_boot_t2 >> 4) & 0x7);
				else
					sprintf(str,
						"ddr_g12_offset_data 2 0x0  0 0 2 %d ",
						(fast_boot_t2 >> 4) & 0x7);

				printf("\nstr=%s\n", str);

				run_command(str, 0);
			}

			sprintf(str, "ddr_fast_boot 1 ");
			printf("\nstr=%s\n", str);
			run_command(str, 0);
		} else {
			sha256_csum_wd_internal((unsigned char *)(uint64_t)ddr_set_add,
						sizeof(ddr_set_t), ddr_sha_s5.sha2, 0);
			ddr_do_store_ddr_parameter_ops((uint8_t *)(unsigned long)(ddr_set_add -
										  SHA_SUM_LEN),
						       write_size);
		}
		if (enable_ddr_check_boot_reason) {
			if (boot_reason) {
				sprintf(str, "systemoff");
				printf("\nstr=%s\n", str);
				run_command(str, 0);
			}
		}
		wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address),
		       dmc_retraining_ctrl);
		return 1;
		//}
	}
	wr_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address, dmc_retraining_ctrl);
	return 1;
}

#ifdef ENABLE_OLD_EXTRA_TEST_CMD
U_BOOT_CMD(ddr_auto_scan_drv, 30, 1, do_ddr_auto_scan_drv,
	   "ddr_test_cmd cmd arg1 arg2 arg3...",
	   "ddr_test_cmd cmd arg1 arg2 arg3...\n dcache off ?\n");
#endif
U_BOOT_CMD(ddr_fast_boot, 30, 1, do_ddr_fastboot_config,
	   "ddr_fastboot_config cmd arg1 arg2 arg3...",
	   "ddr_fastboot_config cmd arg1 arg2 arg3...\n dcache off ?\n");

U_BOOT_CMD(ddr_auto_fast_boot_check, 30, 1, do_ddr_auto_fastboot_check,
	   "ddr_fastboot_config cmd arg1 arg2 arg3...",
	   "ddr_fastboot_config cmd arg1 arg2 arg3...\n dcache off ?\n");
