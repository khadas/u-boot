#include <common.h>

//#define USE_FOR_UBOOT_2018
#define USE_FOR_UBOOT_2015
#define GET_CHIP_ID

#ifdef  USE_FOR_UBOOT_2018
#define  DISABLE_ENV
#define USE_FOR_UBOOT_2018
#include <amlogic/cpu_id.h>
#endif
#ifdef  USE_FOR_UBOOT_2015
#include <asm/cpu_id.h>
#endif

#define ENABLE_OLD_EXTRA_TEST_CMD  1
#define ENABLE_G12_PHY_TEST_CMD  1


uint32_t  do_read_c2_ddr_bdlr_steps(void);

struct ddr_base_address_table {
	char		soc_family_name[16];
	unsigned int	chip_id;
	unsigned int	preg_sticky_reg0;
	unsigned int	ddr_phy_base_address;
	unsigned int	ddr_pctl_timing_base_address;
	unsigned int	ddr_pctl_timing_end_address;
	unsigned int	ddr_dmc_sticky0;
	unsigned int	sys_watchdog_base_address;
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
#undef  MESON_CPU_MAJOR_ID_S4D
#define MESON_CPU_MAJOR_ID_S4D                  0x3a
//T5W already define in file<../arch/arm/include/asm/cpu_id.h>
//#define MESON_CPU_MAJOR_ID_T5W                  0x3b
#define MESON_CPU_VERSION_LVL_MAJOR     0
#define MESON_CPU_VERSION_LVL_MINOR     1
#define MESON_CPU_VERSION_LVL_PACK      2
#define MESON_CPU_VERSION_LVL_MISC      3
#define MESON_CPU_VERSION_LVL_MAX       MESON_CPU_VERSION_LVL_MISC

#define CHIP_ID_MASK  0xff
char global_chip_id[12] = { 0 };

#define MESON_CPU_CHIP_ID_SIZE    12   //4  //12byte
int ddr_get_chip_id(void)
{
	int soc_family_id = 0;

	soc_family_id = get_cpu_id().family_id;
	printf("\nsoc_family_id==0x%08x", soc_family_id);

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
ddr_base_address_table_t __ddr_base_address_table[] =
{
	//g12a
	{
		.soc_family_name = "G12A",
		.chip_id = MESON_CPU_MAJOR_ID_G12A,
		.preg_sticky_reg0 = (0xff634400 + (0x070 << 2)), //PREG_STICKY_G12A_REG0
		.ddr_phy_base_address = 0xfe000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xff638400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xff638400),
		.ddr_dmc_sticky0 = 0xff638800,
		.sys_watchdog_base_address = ((0x3c34 << 2) + 0xffd00000),
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
		.sys_watchdog_base_address = ((0x3c34 << 2) + 0xffd00000),
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
		.sys_watchdog_base_address = ((0x3c34 << 2) + 0xffd00000),
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
		.sys_watchdog_base_address = ((0x3c34 << 2) + 0xffd00000),
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
		.sys_watchdog_base_address = ((0x3c34 << 2) + 0xffd00000),
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
		.preg_sticky_reg0 = 0xfffff400, //use sram  A1,((0x00b0  << 2) + 0xfe005800),//SYSCTRL_STICKY_REG0
		.ddr_phy_base_address = 0xfc000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xfd020400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xfd020400),
		.ddr_dmc_sticky0 = 0xfd020800,
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xfd020400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xfd020400),
		.sys_watchdog_base_address = 0,                                 //((0x0040  << 2) + 0xfe000000),
		.sys_watchdog_enable_value = 0x03c401ff,
		.ddr_boot_reason_address = ((0x00e1 << 2) + 0xfe005800),        //SYSCTRL_SEC_STICKY_REG1
		.ee_timer_base_address = ((0x0041 << 2) + 0xfe005800),
	},
	//c1
	{
		.soc_family_name = "C1",
		.chip_id = MESON_CPU_MAJOR_ID_C1,
		.preg_sticky_reg0 = 0xfffff400, //use sram  A1,((0x00b0  << 2) + 0xfe005800),//SYSCTRL_STICKY_REG0
		.ddr_phy_base_address = 0xfd000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xfe024400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xfe024400),
		.ddr_dmc_sticky0 = ((0x0000 << 2) + 0xfe024800),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xfe024c00),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xfe024400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xfe024400),
		.sys_watchdog_base_address = 0,                                 //((0x0040  << 2) + 0xfe000000),
		.sys_watchdog_enable_value = 0x03c401ff,
		.ddr_boot_reason_address = ((0x00e1 << 2) + 0xfe005800),        //SYSCTRL_SEC_STICKY_REG1
		.ee_timer_base_address = ((0x0041 << 2) + 0xfe005800),
	},
	//c2
	{
		.soc_family_name = "C2",
		.chip_id = MESON_CPU_MAJOR_ID_C2,
		.preg_sticky_reg0 = ((0x0000 << 2) + 0xfe024800), //use sram  A1,((0x00b0  << 2) + 0xfe005800),//SYSCTRL_STICKY_REG0
		.ddr_phy_base_address = 0xfd000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xfe024400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xfe024400),
		.ddr_dmc_sticky0 = ((0x0000 << 2) + 0xfe024800),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xfe024c00),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xfe024400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xfe024400),
		.sys_watchdog_base_address = 0,                                 //((0x0040  << 2) + 0xfe000000),
		.sys_watchdog_enable_value = 0x03c401ff,
		.ddr_boot_reason_address = ((0x00e1 << 2) + 0xfe005800),        //SYSCTRL_SEC_STICKY_REG1
		.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xfe024400),
		.ddr_dmc_refresh_ctrl_address = ((0x0092 << 2) + 0xfe024400),   //DMC_DRAM_REFR_CTRL
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
		.sys_watchdog_base_address = ((0x3c34 << 2) + 0xffd00000),      //sc2 can not find
		.ddr_pll_base_address = ((0x0000 << 2) + 0xfe036c00),
		.ee_timer_base_address = ((0x003b << 2) + 0xfe010000),          //sc2 can not find
		.ee_pwm_base_address = ((0x0001 << 2) + 0xfe05e000),            //PWMGH_PWM_B
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xfe036400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xfe036400),
		//.ddr_boot_reason_address = ((0x00e1 << 2) + 0xfe010000), //SYSCTRL_SEC_STICKY_REG1,20210204,0xfe010384,zhiguang confirm
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
		//AM_DDR_PLL_CNTL0
		.ddr_pll_base_address = ((0x0000 << 2) + 0xff638c00),
		//#define SEC_AO_RTI_STATUS_REG3
		.ddr_boot_reason_address = (0xff800000 + (0x003 << 2)),
		//.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xfe024400),

		.sys_watchdog_base_address = 0,
		.sys_watchdog_enable_value = 0x03c401ff,
		//#define P_EE_TIMER_E		(volatile uint32_t *)0xffd0f188
		.ee_timer_base_address = 0xffd0f188,
		//AO_PWM_PWM_B
		.ee_pwm_base_address = ((0x001 << 2) + 0xff807000),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xff638400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xff638400),
		//DMC_DRAM_REFR_CTRL ((0x0092 << 2) + 0xff638400)
		.ddr_dmc_refresh_ctrl_address = ((0x0092 << 2) + 0xff638400),
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
		//AM_DDR_PLL_CNTL0
		.ddr_pll_base_address = ((0x0000 << 2) + 0xff638c00),
		//#define SEC_AO_RTI_STATUS_REG3
		.ddr_boot_reason_address = (0xff800000 + (0x003 << 2)),
		//.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xfe024400),

		.sys_watchdog_base_address = 0,
		.sys_watchdog_enable_value = 0x03c401ff,
		//#define P_EE_TIMER_E		(volatile uint32_t *)0xffd0f188
		.ee_timer_base_address = 0xffd0f188,
		//AO_PWM_PWM_B
		.ee_pwm_base_address = ((0x001 << 2) + 0xff807000),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xff638400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xff638400),
		//DMC_DRAM_REFR_CTRL ((0x0092 << 2) + 0xff638400)
		.ddr_dmc_refresh_ctrl_address = ((0x0092 << 2) + 0xff638400),
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
		.sys_watchdog_base_address = ((0x3c34 << 2) + 0xffd00000),      //sc2 can not find
		.ddr_pll_base_address = ((0x0000 << 2) + 0xfe036c00),
		.ee_timer_base_address = ((0x003b << 2) + 0xfe010000),          //sc2 can not find
		.ee_pwm_base_address = ((0x0001 << 2) + 0xfe05e000),            //PWMGH_PWM_B
		.ddr_dmc_apd_address = ((0x018c << 2) + 0xfe036000),
		.ddr_dmc_asr_address = ((0x018d << 2) + 0xfe036000),
		//.ddr_boot_reason_address = ((0x00c1 << 2) + 0xfe010000), //SYSCTRL_SEC_STATUS_REG1,20210204,0xfe010304,zhiguang confirm

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
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xfe036400),   //DMC_DRAM_TRFC
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xfe036400),    //DMC_DRAM_DFI
		.ddr_dmc_sticky0 = ((0x0000 << 2) + 0xfe036800),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xfe036c00),           //AM_DDR_PLL_CNTL0//
		//.ddr_boot_reason_address = ((0x00e1 << 2) + 0xfe010000), //SYSCTRL_SEC_STICKY_REG1,20210204,0xfe010384,zhiguang confirm
		//.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xfe024400),

		.sys_watchdog_base_address = 0,
		.sys_watchdog_enable_value = 0x03c401ff,
		.ee_timer_base_address = ((0x003b << 2) + 0xfe010000),          //SYSCTRL_TIMERE                             ((0x003b  << 2) + 0xfe010000)
		.ee_pwm_base_address = ((0x001 << 2) + 0xff807000),             //AO_PWM_PWM_B
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xfe036400),            //DMC_DRAM_APD_CTRL
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xfe036400),            //DMC_DRAM_ASR_CTRL
		.ddr_dmc_refresh_ctrl_address = ((0x0092 << 2) + 0xfe036400),   // DMC_DRAM_REFR_CTRL ((0x0092 << 2) + 0xff638400)
	},
	//T3
	{
		.soc_family_name = "T3",
		.chip_id = MESON_CPU_MAJOR_ID_T3,
		.preg_sticky_reg0 = ((0x0000 << 2) + 0xfe036800),
		.ddr_phy_base_address = 0xfc000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xfe036400),   //DMC_DRAM_TRFC
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xfe036400),    //DMC_DRAM_DFI
		.ddr_dmc_sticky0 = ((0x0000 << 2) + 0xfe036800),
		.ddr_pll_base_address = ((0x0000 << 2) + 0xfe0a0000),           //AM_DDR_PLL_CNTL0//
		//.ddr_boot_reason_address = ((0x00e1 << 2) + 0xfe010000), //SYSCTRL_SEC_STICKY_REG1,20210204,0xfe010384,zhiguang confirm
		//.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xfe024400),

		.sys_watchdog_base_address = 0,
		.sys_watchdog_enable_value = 0x03c401ff,
		.ee_timer_base_address = ((0x003b << 2) + 0xfe010000),          //SYSCTRL_TIMERE                             ((0x003b  << 2) + 0xfe010000)
		.ee_pwm_base_address = ((0x001 << 2) + 0xff807000),             //AO_PWM_PWM_B
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xfe036400),            //DMC_DRAM_APD_CTRL
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xfe036400),            //DMC_DRAM_ASR_CTRL
		.ddr_dmc_refresh_ctrl_address = ((0x0092 << 2) + 0xfe036400),   // DMC_DRAM_REFR_CTRL ((0x0092 << 2) + 0xff638400)

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
	//T5W
	{
		.soc_family_name = "T5W",
		.chip_id = MESON_CPU_MAJOR_ID_T5W,
		.preg_sticky_reg0 = ((0x0000 << 2) + 0xff638800),
		.ddr_phy_base_address = 0xfe000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xff638400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xff638400),
		.ddr_dmc_sticky0 = ((0x0000 << 2) + 0xff638800),
		//AM_DDR_PLL_CNTL0
		.ddr_pll_base_address = ((0x0000 << 2) + 0xff638c00),
		//#define SEC_AO_RTI_STATUS_REG3
		.ddr_boot_reason_address = (0xff800000 + (0x003 << 2)),
		//.ddr_dmc_lpdd4_retraining_address = ((0x0097 << 2) + 0xfe024400),

		.sys_watchdog_base_address = 0,
		.sys_watchdog_enable_value = 0x03c401ff,
		//#define P_EE_TIMER_E		(volatile uint32_t *)0xffd0f188
		.ee_timer_base_address = 0xffd0f188,
		//AO_PWM_PWM_B
		.ee_pwm_base_address = ((0x001 << 2) + 0xff807000),
		.ddr_dmc_apd_address = ((0x008c << 2) + 0xff638400),
		.ddr_dmc_asr_address = ((0x008d << 2) + 0xff638400),
		//DMC_DRAM_REFR_CTRL ((0x0092 << 2) + 0xff638400)
		.ddr_dmc_refresh_ctrl_address = ((0x0092 << 2) + 0xff638400),
	},
	// force id use id mask
	{
		.soc_family_name = "UKNOWN",
		.chip_id = CHIP_ID_MASK,                                //MESON_CPU_MAJOR_ID_G12A,
		.preg_sticky_reg0 = (0xff634400 + (0x070 << 2)),        //PREG_STICKY_G12A_REG0
		.ddr_phy_base_address = 0xfe000000,
		.ddr_pctl_timing_base_address = ((0x0000 << 2) + 0xff638400),
		.ddr_pctl_timing_end_address = ((0x00bb << 2) + 0xff638400),
		.ddr_dmc_sticky0 = 0xff638800,
		.sys_watchdog_base_address = ((0x3c34 << 2) + 0xffd00000),
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
unsigned int phy_base_add[2] = { 0, 0, };
unsigned int ddr_dmc_sticky[2] = { 0, 0, };
unsigned int ddr_dmc_apd_address[2] = { 0, 0, };
unsigned int ddr_dmc_asr_address[2] = { 0, 0, };
unsigned int dmc_retraining_ctrl_address[2] = { 0, 0, };
unsigned int dmc_ddr_config_channel_id = 0;
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

#define DWC_AC_PINMUX_TOTAL                                             28
#define DWC_DFI_PINMUX_TOTAL                                    26

#define DDR_STICKY_MAGIC_NUMBER                                 0x20180000
#define DDR_CHIP_ID                                                             0x30
#define DDR_STICKY_SOURCE_DMC_STICKY                    0x1
#define DDR_STICKY_SOURCE_SRAM                                  0x2

#define DDR_STICKY_OVERRIDE_CONFIG_MESSAGE_CMD  0x1             //override config
#define DDR_STICKY_SPECIAL_FUNCTION_CMD                 0x2     //special test such as shift some bdlr or parameter or interleave test

#define DDR_INIT_CONFIG_STICKY_MESSAGE_SRAM_ADDRESS     0x00040000
#define DDR_INIT_CONFIG_GLOBAL_MESSAGE_SRAM_ADDRESS     0x00050000
#define CONFIG_DDR_TYPE_DDR3                            0
#define CONFIG_DDR_TYPE_DDR4                            1
#define CONFIG_DDR_TYPE_LPDDR4                          2
#define CONFIG_DDR_TYPE_LPDDR3                          3
#define CONFIG_DDR_TYPE_LPDDR2                          4
#define CONFIG_DDR_TYPE_AUTO                            0xf
#define CONFIG_DDR_TYPE_AUTO_LIMIT                      CONFIG_DDR_TYPE_DDR4

#define CONFIG_DDR0_16BIT_CH0                           0x1
#define CONFIG_DDR0_16BIT_RANK01_CH0            0x4
#define CONFIG_DDR0_32BIT_RANK0_CH0                     0x2
#define CONFIG_DDR0_32BIT_RANK01_CH01           0x3
#define CONFIG_DDR0_32BIT_16BIT_RANK0_CH0       0x5
#define CONFIG_DDR0_32BIT_16BIT_RANK01_CH0      0x6
#define CONFIG_DDR0_32BIT_RANK01_CH0            0x7
#define CONFIG_DDR0_32BIT_RANK0_CH01            0x8
#define DDR_PRINT_DISABLE       0
#define DDR_PRINT_ENABLE        1

typedef struct board_common_setting {
	unsigned int	timming_magic;
	unsigned short	timming_max_valid_configs;
	unsigned short	timming_struct_version;
	unsigned short	timming_struct_org_size;
	unsigned short	timming_struct_real_size;
	unsigned char	fast_boot[4]; // 0   fastboot enable  1 window test margin  2 auto offset after window test 3 auto window test enable
	unsigned int	ddr_func;
	unsigned char	board_id;
	unsigned char	DramType;
	unsigned char	dram_rank_config;
	unsigned char	DisabledDbyte;
	unsigned int	dram_cs0_base_add;
	unsigned int	dram_cs1_base_add;
	unsigned short	dram_cs0_size_MB;       //dram_ch0_size_MB T3
	unsigned short	dram_cs1_size_MB;       //dram_ch1_size_MB T3
	unsigned char	dram_x4x8x16_mode;
	unsigned char	Is2Ttiming;
	unsigned char	log_level;
	unsigned char	ddr_rdbi_wr_enable;
	unsigned int	pll_ssc_mode;
	unsigned short	org_tdqs2dq;
	unsigned char	reserve1_test_function[2];
	unsigned int	ddr_dmc_remap[5];
	unsigned char	ac_pinmux[35];
	unsigned char	ddr_dqs_swap;
	unsigned char	ddr_dq_remap[36];
	unsigned int	ddr_vddee_setting[4]; //add,default-value,default-voltage,step
}__attribute__ ((packed)) board_common_setting_t;

typedef struct board_SI_setting_ps {
	unsigned short	DRAMFreq;
	unsigned char	PllBypassEn;
	unsigned char	training_SequenceCtrl;
	unsigned short	ddr_odt_config;
	unsigned char	clk_drv_ohm;
	unsigned char	cs_drv_ohm;
	unsigned char	ac_drv_ohm;
	unsigned char	soc_data_drv_ohm_p;
	unsigned char	soc_data_drv_ohm_n;
	unsigned char	soc_data_odt_ohm_p;
	unsigned char	soc_data_odt_ohm_n;
	unsigned char	dram_data_drv_ohm;
	unsigned char	dram_data_odt_ohm;
	unsigned char	dram_data_wr_odt_ohm;
	unsigned char	dram_ac_odt_ohm;
	unsigned char	dram_data_drv_pull_up_calibration_ohm;
	unsigned char	lpddr4_dram_vout_voltage_range_setting;
	unsigned char	dfe_offset;
	unsigned short	vref_ac_permil;         //phy
	unsigned short	vref_soc_data_permil;   //soc
	unsigned short	vref_dram_data_permil;
	unsigned short	max_core_timmming_frequency;
	unsigned short	training_phase_parameter[2];
	unsigned short	ac_trace_delay_org[36];
}__attribute__ ((packed)) board_SI_setting_ps_t;

typedef struct board_phase_setting_ps {
	unsigned short	ac_trace_delay[36];
	unsigned short	write_dqs_delay[8];
	unsigned short	write_dq_bit_delay[72];
	unsigned short	read_dqs_gate_delay[8];
	unsigned char	read_dqs_delay[8];
	unsigned char	read_dq_bit_delay[72];
	unsigned char	soc_bit_vref[44];
	unsigned char	dram_bit_vref[36];
	unsigned char	reserve_training_parameter[16]; //0-7 write dqs offset,8-15 read dqs offset,MSB bit 7 use 0 mean right offset
	unsigned char	soc_bit_vref_dac1[44];
}__attribute__ ((packed)) board_phase_setting_ps_t;

typedef struct ddr_set_c2 {
	board_common_setting_t		cfg_board_common_setting;
	board_SI_setting_ps_t		cfg_board_SI_setting_ps[2];
	board_phase_setting_ps_t	cfg_ddr_training_delay_ps[2];
}__attribute__ ((packed)) ddr_set_t_c2;

static uint32_t ddr_rd_8_16bit_on_32reg(uint32_t base_addr, uint32_t size, uint32_t offset_index)
{
	uint32_t read_value = 0;
	uint32_t addr_t = 0;
	uint32_t offset = 0;

	if (size == 8) {
		offset = ((offset_index % 4) << 3);
		addr_t = (base_addr + ((offset_index >> 2) << 2));
		read_value = (*(volatile uint32_t *)((unsigned long)(addr_t)));
		read_value = (read_value >> offset) & 0xff;
	}
	if (size == 16) {
		offset = ((offset_index % 2) << 4);
		addr_t = (base_addr + ((offset_index >> 1) << 2));
		read_value = (*(volatile uint32_t *)((unsigned long)(addr_t)));
		read_value = (read_value >> offset) & 0xffff;
	}
	return read_value;
}

static uint32_t ddr_wr_8_16bit_on_32reg(uint32_t base_addr, uint32_t size, uint32_t offset_index, uint32_t value)
{
	uint32_t read_value = 0;
	uint32_t write_value = 0;
	uint32_t addr_t = 0;
	uint32_t offset = 0;

	if (size == 8) {
		offset = ((offset_index % 4) << 3);
		addr_t = (base_addr + ((offset_index >> 2) << 2));
		read_value = (*(volatile uint32_t *)((unsigned long)(addr_t)));
		write_value = (value << offset) | (read_value & (~(0xff << offset)));
	}
	if (size == 16) {
		offset = ((offset_index % 2) << 4);
		addr_t = (base_addr + ((offset_index >> 1) << 2));
		read_value = (*(volatile uint32_t *)((unsigned long)(addr_t)));
		write_value = (value << offset) | (read_value & (~(0xffff << offset)));
	}
	*(volatile uint32_t *)((unsigned long)(addr_t)) = write_value;
	return write_value;
}

uint32_t ddr_min(uint32_t a, uint32_t b)
{
	uint32_t min = a;

	if (a < b)
		min = a;
	else
		min = b;
	return min;
}

uint32_t ddr_max(uint32_t a, uint32_t b)
{
	uint32_t max = a;

	if (a < b)
		max = b;
	else
		max = a;
	return max;
}

typedef struct training_delay_set_ps {
	unsigned char	ac_trace_delay[10];
	unsigned char	ac_trace_delay_rev[2];
	unsigned char	read_dqs_delay[16];
	unsigned char	read_dq_bit_delay[72];
	unsigned short	write_dqs_delay[16];
	unsigned short	write_dq_bit_delay[72];
	unsigned short	read_dqs_gate_delay[16];
	unsigned char	soc_bit_vref[36];
	unsigned char	dram_bit_vref[32];
	unsigned char	rever1;        //read_dqs  read_dq,write_dqs, write_dq
	unsigned char	dfi_mrl;
	unsigned char	dfi_hwtmrl;
	unsigned char	ARdPtrInitVal;
	unsigned short	csr_vrefinglobal;
	unsigned short	csr_dqsrcvcntrl[4];
	unsigned short	csr_pptdqscntinvtrntg0[4];
	unsigned short	csr_pptdqscntinvtrntg1[4];
	unsigned short	csr_seq0bgpr[9];
	unsigned short	csr_dllgainctl;
	unsigned short	csr_dlllockpara;
}__attribute__ ((packed)) training_delay_set_ps_t;

typedef struct ddr_phy_common_extra_set {
	unsigned short	csr_pllctrl3;
	unsigned short	csr_pptctlstatic[4];
	unsigned short	csr_trainingincdecdtsmen[4];
	unsigned short	csr_tsmbyte0[4];
	unsigned short	csr_hwtcamode;
	unsigned short	csr_hwtlpcsena;
	unsigned short	csr_hwtlpcsenb;
	unsigned short	csr_acsmctrl13;
	unsigned short	csr_acsmctrl23;
	unsigned char	csr_soc_vref_dac1_dfe[36];
}__attribute__ ((packed)) ddr_phy_common_extra_set_t;

typedef struct retraining_set {
	unsigned short	csr_pllctrl3;
	unsigned short	csr_pptctlstatic[4];
	unsigned short	csr_trainingincdecdtsmen[4];
	unsigned short	csr_tsmbyte0[4];
	unsigned short	csr_vrefinglobal;
	unsigned short	csr_dqsrcvcntrl[4];
	unsigned short	csr_pptdqscntinvtrntg0[4];
	unsigned short	csr_pptdqscntinvtrntg1[4];
	unsigned short	csr_seq0bgpr[9];
	unsigned short	csr_dllgainctl;
	unsigned short	csr_dlllockpara;
	unsigned short	csr_hwtcamode;
	unsigned short	csr_hwtlpcsena;
	unsigned short	csr_hwtlpcsenb;
	unsigned short	csr_acsmctrl13;
	unsigned short	csr_acsmctrl23;
	unsigned char	csr_soc_vref_dac1_dfe[36];
}retraining_set_t;

typedef struct ddr_set {
	unsigned int	magic;
	unsigned char	fast_boot[4];        // 0   fastboot enable  1 window test margin  2 auto offset after window test 3 auto window test
	unsigned int	ddr_func;
	unsigned char	board_id;
	//board id reserve,,do not modify
	unsigned char	version;
	// firmware reserve version,,do not modify
	unsigned char	DramType;
	//support DramType should confirm with amlogic
	//#define CONFIG_DDR_TYPE_DDR3				0
	//#define CONFIG_DDR_TYPE_DDR4				1
	//#define CONFIG_DDR_TYPE_LPDDR4				2
	//#define CONFIG_DDR_TYPE_LPDDR3				3
	//#define CONFIG_DDR_TYPE_LPDDR2				4
	unsigned char	DisabledDbyte;
	//use for dram bus 16bit or 32bit,if use 16bit mode ,should disable bit 2,3
	//bit 0 ---use byte 0 ,1 disable byte 0,
	//bit 1 ---use byte 1 ,1 disable byte 1,
	//bit 2 ---use byte 2 ,1 disable byte 2,
	//bit 3 ---use byte 3 ,1 disable byte 3,
	unsigned char	Is2Ttiming;
	//ddr3/ddr3 use 2t timing,now only support 2t timming
	unsigned char	HdtCtrl;
	//training information control,do not modify
	unsigned char	dram_rank_config;
	//support Dram connection type should confirm with amlogic
	//#define CONFIG_DDR0_16BIT_CH0				0x1  //dram total bus width 16bit only use cs0
	//#define CONFIG_DDR0_16BIT_RANK01_CH0		0x4  //dram total bus width 16bit  use cs0 cs1
	//#define CONFIG_DDR0_32BIT_RANK0_CH0			0x2  //dram total bus width 32bit  use cs0
	//#define CONFIG_DDR0_32BIT_RANK01_CH01		0x3    //only for lpddr4,dram total bus width 32bit  use chanel a cs0 cs1 chanel b cs0 cs1
	//#define CONFIG_DDR0_32BIT_16BIT_RANK0_CH0		0x5    //dram total bus width 32bit only use cs0,but high address use 16bit mode
	//#define CONFIG_DDR0_32BIT_16BIT_RANK01_CH0	0x6   //dram total bus width 32bit  use cs0 cs1,but cs1 use 16bit mode ,current phy not support reserve
	//#define CONFIG_DDR0_32BIT_RANK01_CH0		0x7       //dram total bus width 32bit  use cs0 cs1
	//#define CONFIG_DDR0_32BIT_RANK0_CH01		0x8     //only for lpddr4,dram total bus width 32bit  use chanel a cs0  chanel b cs0

	/* rsv_char0. update for diagnose type define */
	unsigned char	diagnose;

	unsigned short	soc_data_drv_ohm_ps1;
	unsigned short	dram_data_drv_ohm_ps1;
	unsigned short	soc_data_odt_ohm_ps1;
	unsigned short	dram_data_odt_ohm_ps1;
	unsigned short	dram_data_wr_odt_ohm_ps1;
	//system reserve,do not modify
	unsigned short	dmem_load_size;
	//system reserve,do not modify
	unsigned int	ddr_base_addr;
	//system reserve,do not modify
	unsigned int	ddr_start_offset;
	//system reserve,do not modify

	unsigned short	dram_cs0_size_MB;
	//config cs0 dram size ,like 1G DRAM ,setting 1024
	unsigned short	dram_cs1_size_MB;
	//config cs1 dram size,like 512M DRAM ,setting 512
	/* align8 */

	unsigned short	training_SequenceCtrl[2];
	//system reserve,do not modify
	unsigned char	phy_odt_config_rank[2];
	//unsigned	char	 rever1;
	//unsigned	char	 rever2;
	unsigned short	rank1_ca_vref_permil;
	//training odt config ,only use for training
	// [0]Odt pattern for accesses targeting rank 0. [3:0] is used for write ODT [7:4] is used for read ODT
	// [1]Odt pattern for accesses targeting rank 1. [3:0] is used for write ODT [7:4] is used for read ODT
	unsigned int	dfi_odt_config;
	//normal go status od config,use for normal status
	//bit 12.  rank1 ODT default. default value for ODT[1] pins if theres no read/write activity.
	//bit 11.  rank1 ODT write sel.  enable ODT[1] if there's write occur in rank1.
	//bit 10.  rank1 ODT write nsel. enable ODT[1] if theres's write occur in rank0.
	//bit 9.   rank1 odt read sel.   enable ODT[1] if there's read occur in rank1.
	//bit 8.   rank1 odt read nsel.  enable ODT[1] if there's read occure in rank0.
	//bit 4.   rank0 ODT default.    default value for ODT[0] pins if theres no read/write activity.
	//bit 3.   rank0 ODT write sel.  enable ODT[0] if there's write occur in rank0.
	//bit 2.   rank0 ODT write nsel. enable ODT[0] if theres's write occur in rank1.
	//bit 1.   rank0 odt read sel.   enable ODT[0] if there's read occur in rank0.
	//bit 0.   rank0 odt read nsel.  enable ODT[0] if there's read occure in rank1.
	unsigned short	DRAMFreq[4];
	//config dram frequency,use DRAMFreq[0],other reserve
	unsigned char	PllBypassEn;
	//system reserve,do not modify
	unsigned char	ddr_rdbi_wr_enable;
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
	unsigned short			clk_drv_ohm;
	//config soc clk pin signal driver strength ,select 20,30,40,60ohm
	unsigned short			cs_drv_ohm;
	//config soc cs0 cs1 pin signal driver strength ,select 20,30,40,60ohm
	unsigned short			ac_drv_ohm;
	//config soc  normal address command pin driver strength ,select 20,30,40,60ohm
	unsigned short			soc_data_drv_ohm_p;
	//config soc data pin pull up driver strength,select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short			soc_data_drv_ohm_n;
	//config soc data pin pull down driver strength,select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short			soc_data_odt_ohm_p;
	//config soc data pin odt pull up strength,select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short			soc_data_odt_ohm_n;
	//config soc data pin odt pull down strength,select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short			dram_data_drv_ohm;
	//config dram data pin pull up pull down driver strength,ddr3 select 34,40ohm,ddr4 select 34,48ohm,lpddr4 select 40,48,60,80,120,240ohm
	unsigned short			dram_data_odt_ohm;
	//config dram data pin odt pull up down strength,ddr3 select 40,60,120ohm,ddr4 select 34,40,48,60,120,240ohm,lpddr4 select 40,48,60,80,120,240ohm
	unsigned short			dram_ac_odt_ohm;
	//config dram ac pin odt pull up down strength,use for lpddr4, select 40,48,60,80,120,240ohm
	unsigned short			soc_clk_slew_rate;
	//system reserve,do not modify
	unsigned short			soc_cs_slew_rate;
	//system reserve,do not modify
	unsigned short			soc_ac_slew_rate;
	//system reserve,do not modify
	unsigned short			soc_data_slew_rate;
	//system reserve,do not modify
	unsigned short			vref_output_permil;     //phy
	//setting same with vref_dram_permil
	unsigned short			vref_receiver_permil;   //soc
	//soc init SOC receiver vref ,config like 500 means 0.5VDDQ,take care ,please follow SI
	unsigned short			vref_dram_permil;
	//soc init DRAM receiver vref ,config like 500 means 0.5VDDQ,take care ,please follow SI
	unsigned short			max_core_timmming_frequency;
	//use for limited ddr speed core timmming parameter,for some old dram maybe have no over speed register
	/* align8 */

	unsigned char			ac_trace_delay[10];
	unsigned char			lpddr4_dram_vout_voltage_1_3_2_5_setting;
	unsigned char			lpddr4_x8_mode;
	unsigned char			slt_test_function[2]; //[0] slt test function enable,bit 0 enable 4 frequency scan,bit 1 enable force delay line offset ,bit 7 enable skip training function
	//[1],slt test parameter ,use for force delay line offset
	//system reserve,do not modify
	unsigned short			tdqs2dq;
	unsigned char			dram_data_wr_odt_ohm;
	unsigned char			bitTimeControl_2d;
	//system reserve,do not modify
	/* align8 */
	unsigned char			char_rev1;
	unsigned char			char_rev2;
	unsigned int			ddr_dmc_remap[5];
	unsigned int			dram_rtt_nom_wr_park[2];
	//system reserve,do not modify
	/* align8 */
	unsigned char			ddr_lpddr34_ca_remap[4];
	////use for lpddr3 /lpddr4 ca training data byte lane remap
	unsigned char			ddr_lpddr34_dq_remap[32];
	////use for lpddr3 /lpddr4 ca pinmux remap
	unsigned char			ac_pinmux[DWC_AC_PINMUX_TOTAL];
	//use for lpddr3 /lpddr4 ca pinmux remap
	unsigned char			dfi_pinmux[DWC_DFI_PINMUX_TOTAL];
	unsigned char			char_rev3;
	unsigned char			char_rev4;
	ddr_phy_common_extra_set_t	cfg_ddr_phy_common_extra_set_t;
	training_delay_set_ps_t		cfg_ddr_training_delay_ps[2];
}ddr_set_t;

ddr_set_t p_ddr_set_t;


typedef struct ddr_set_t7 {
	unsigned int	magic;
	unsigned char	fast_boot[4];        // 0   fastboot enable  1 window test margin  2 auto offset after window test 3 auto window test
	//unsigned	int		rsv_int0;
	unsigned int	ddr_func;
	unsigned char	board_id;
	//board id reserve,,do not modify
	unsigned char	version;
	// firmware reserve version,,do not modify
	unsigned char	DramType;
	//support DramType should confirm with amlogic
	//#define CONFIG_DDR_TYPE_DDR3				0
	//#define CONFIG_DDR_TYPE_DDR4				1
	//#define CONFIG_DDR_TYPE_LPDDR4				2
	//#define CONFIG_DDR_TYPE_LPDDR3				3
	//#define CONFIG_DDR_TYPE_LPDDR2				4
	//#define CONFIG_DDR_TYPE_LPDDR4x				5
	unsigned char DisabledDbyte[2];              //ch0 and ch1
	//use for dram bus 16bit or 32bit,if use 16bit mode ,should disable bit 2,3
	//bit 0 ---cs0 use byte 0 ,1 disable byte 0,
	//bit 1 ---cs0 use byte 1 ,1 disable byte 1,
	//bit 2 ---cs0 use byte 2 ,1 disable byte 2,
	//bit 3 ---cs0 use byte 3 ,1 disable byte 3,
	//bit 4 ---cs1 use byte 0 ,1 disable byte 0,
	//bit 5 ---cs1 use byte 1 ,1 disable byte 1,
	//bit 6 ---cs1 use byte 2 ,1 disable byte 2,
	//bit 7 ---cs1 use byte 3 ,1 disable byte 3,
	unsigned char	Is2Ttiming;
	//ddr3/ddr3 use 2t timing,now only support 2t timming
	unsigned char	HdtCtrl;
	//training information control,do not modify
	unsigned char	dram_rank_config;
	//support Dram connection type should confirm with amlogic
	//#define CONFIG_DDR0_16BIT_CH0				0x1  //dram total bus width 16bit only use cs0
	//#define CONFIG_DDR0_16BIT_RANK01_CH0		0x4  //dram total bus width 16bit  use cs0 cs1
	//#define CONFIG_DDR0_32BIT_RANK0_CH0			0x2  //dram total bus width 32bit  use cs0
	//#define CONFIG_DDR0_32BIT_RANK01_CH01		0x3    //only for lpddr4,dram total bus width 32bit  use chanel a cs0 cs1 chanel b cs0 cs1
	//#define CONFIG_DDR0_32BIT_16BIT_RANK0_CH0		0x5    //dram total bus width 32bit only use cs0,but high address use 16bit mode
	//#define CONFIG_DDR0_32BIT_16BIT_RANK01_CH0	0x6   //dram total bus width 32bit  use cs0 cs1,but cs1 use 16bit mode ,current phy not support reserve
	//#define CONFIG_DDR0_32BIT_RANK01_CH0		0x7       //dram total bus width 32bit  use cs0 cs1
	//#define CONFIG_DDR0_32BIT_RANK0_CH01		0x8     //only for lpddr4,dram total bus width 32bit  use chanel a cs0  chanel b cs0

	/* rsv_char0. update for diagnose type define */
	//unsigned	char	diagnose;

	unsigned short	soc_data_drv_ohm_ps1;
	unsigned short	dram_data_drv_ohm_ps1;
	unsigned short	soc_data_odt_ohm_ps1;
	unsigned short	dram_data_odt_ohm_ps1;
	unsigned short	dram_data_wr_odt_ohm_ps1;
#if 0
	/* imem/dmem define */
	unsigned int	imem_load_addr;
	//system reserve,do not modify
	unsigned int	dmem_load_addr;
	//system reserve,do not modify
	unsigned short	imem_load_size;
#endif
	//system reserve,do not modify
	unsigned short	soc_data_drv_ohm_ffe;            //dmem_load_size;
	//system reserve,do not modify
	unsigned int	ddr_base_addr;
	//system reserve,do not modify
	unsigned int	ddr_start_offset;
	//system reserve,do not modify

	unsigned short	dram_ch0_size_MB;
	//config cs0 dram size ,like 1G DRAM ,setting 1024
	unsigned short	dram_ch1_size_MB;
	//config cs1 dram size,like 512M DRAM ,setting 512
	/* align8 */

	unsigned short	training_SequenceCtrl[2];
	//system reserve,do not modify
	unsigned char	phy_odt_config_rank[2];
	//unsigned	char	 rever1;
	//unsigned	char	 rever2;
	unsigned short	rank1_ca_vref_permil;
	//training odt config ,only use for training
	// [0]Odt pattern for accesses targeting rank 0. [3:0] is used for write ODT [7:4] is used for read ODT
	// [1]Odt pattern for accesses targeting rank 1. [3:0] is used for write ODT [7:4] is used for read ODT
	unsigned int	dfi_odt_config;
	//normal go status od config,use for normal status
	//bit 12.  rank1 ODT default. default value for ODT[1] pins if theres no read/write activity.
	//bit 11.  rank1 ODT write sel.  enable ODT[1] if there's write occur in rank1.
	//bit 10.  rank1 ODT write nsel. enable ODT[1] if theres's write occur in rank0.
	//bit 9.   rank1 odt read sel.   enable ODT[1] if there's read occur in rank1.
	//bit 8.   rank1 odt read nsel.  enable ODT[1] if there's read occure in rank0.
	//bit 4.   rank0 ODT default.    default value for ODT[0] pins if theres no read/write activity.
	//bit 3.   rank0 ODT write sel.  enable ODT[0] if there's write occur in rank0.
	//bit 2.   rank0 ODT write nsel. enable ODT[0] if theres's write occur in rank1.
	//bit 1.   rank0 odt read sel.   enable ODT[0] if there's read occur in rank0.
	//bit 0.   rank0 odt read nsel.  enable ODT[0] if there's read occure in rank1.
	unsigned short	DRAMFreq[4];
	//config dram frequency,use DRAMFreq[0],other reserve
	unsigned char	PllBypassEn;
	//system reserve,do not modify
	unsigned char	ddr_rdbi_wr_enable;
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
	unsigned short			clk_drv_ohm;
	//config soc clk pin signal driver strength ,select 20,30,40,60ohm
	unsigned short			cs_drv_ohm;
	//config soc cs0 cs1 pin signal driver strength ,select 20,30,40,60ohm
	unsigned short			ac_drv_ohm;
	//config soc  normal address command pin driver strength ,select 20,30,40,60ohm
	unsigned short			soc_data_drv_ohm_p;
	//config soc data pin pull up driver strength,select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short			soc_data_drv_ohm_n;
	//config soc data pin pull down driver strength,select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short			soc_data_odt_ohm_p;
	//config soc data pin odt pull up strength,select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short			soc_data_odt_ohm_n;
	//config soc data pin odt pull down strength,select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned short			dram_data_drv_ohm;
	//config dram data pin pull up pull down driver strength,ddr3 select 34,40ohm,ddr4 select 34,48ohm,lpddr4 select 40,48,60,80,120,240ohm
	unsigned short			dram_data_odt_ohm;
	//config dram data pin odt pull up down strength,ddr3 select 40,60,120ohm,ddr4 select 34,40,48,60,120,240ohm,lpddr4 select 40,48,60,80,120,240ohm
	unsigned short			dram_ac_odt_ohm;
	//config dram ac pin odt pull up down strength,use for lpddr4, select 40,48,60,80,120,240ohm
	unsigned short			soc_clk_slew_rate;
	//system reserve,do not modify
	unsigned short			soc_cs_slew_rate;
	//system reserve,do not modify
	unsigned short			soc_ac_slew_rate;
	//system reserve,do not modify
	unsigned short			soc_data_slew_rate;
	//system reserve,do not modify
	unsigned short			vref_output_permil;     //phy
	//setting same with vref_dram_permil
	unsigned short			vref_receiver_permil;   //soc
	//soc init SOC receiver vref ,config like 500 means 0.5VDDQ,take care ,please follow SI
	unsigned short			vref_dram_permil;
	//soc init DRAM receiver vref ,config like 500 means 0.5VDDQ,take care ,please follow SI
	unsigned short			max_core_timmming_frequency;
	//use for limited ddr speed core timmming parameter,for some old dram maybe have no over speed register
	/* align8 */

	unsigned char			ac_trace_delay[10];
	unsigned char			lpddr4_dram_vout_voltage_1_3_2_5_setting;
	//use for lpddr4 read vout voltage  setting 0 --->2/5VDDQ ,1--->1/3VDDQ
	unsigned char			lpddr4_x8_mode;
	unsigned char			slt_test_function[2]; //[0] slt test function enable,bit 0 enable 4 frequency scan,bit 1 enable force delay line offset ,bit 7 enable skip training function
	//[1],slt test parameter ,use for force delay line offset
	//system reserve,do not modify
	unsigned short			tdqs2dq;
	unsigned char			dram_data_wr_odt_ohm;
	unsigned char			bitTimeControl_2d;
	//system reserve,do not modify
	/* align8 */
	unsigned char			char_rev1;
	unsigned char			training_offset; //char_rev2;
	unsigned int			ddr_dmc_remap[5];
	unsigned int			dram_rtt_nom_wr_park[2];
	//system reserve,do not modify
	/* align8 */
	unsigned char			ddr_lpddr34_ca_remap[4];
	////use for lpddr3 /lpddr4 ca training data byte lane remap
	unsigned char			ddr_lpddr34_dq_remap[32];
	////use for lpddr3 /lpddr4 ca pinmux remap
	unsigned char			ac_pinmux[DWC_AC_PINMUX_TOTAL];
	//use for lpddr3 /lpddr4 ca pinmux remap
	unsigned char			dfi_pinmux[DWC_DFI_PINMUX_TOTAL];
	unsigned char			char_rev3;
	unsigned char			char_rev4;
	ddr_phy_common_extra_set_t	cfg_ddr_phy_common_extra_set_t;
	training_delay_set_ps_t		cfg_ddr_training_delay_ps[2];

	//override read bit delay
}ddr_set_t7;

ddr_set_t7 p_ddr_set_t7;

#ifndef _SHA256_H_DDR
#define _SHA256_H_DDR

#define SHA256_SUM_LEN  32
#define SHA256_DER_LEN  19

/* Reset watchdog each time we process this many bytes */
#define CHUNKSZ_SHA256  (64 * 1024)

typedef struct {
	uint32_t	total[2];
	uint32_t	state[8];
	uint8_t		buffer[64];
} sha256_context_ddr;

const uint8_t sha256_der_prefix_ddr[SHA256_DER_LEN] =
{
	0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86,
	0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05,
	0x00, 0x04, 0x20
};

/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n, b, i) {			  \
		(n) = ((unsigned long)(b)[(i)] << 24)	 \
		      | ((unsigned long)(b)[(i) + 1] << 16)    \
		      | ((unsigned long)(b)[(i) + 2] << 8)    \
		      | ((unsigned long)(b)[(i) + 3]);	 \
}
#endif
#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n, b, i) {			  \
		(b)[(i)] = (unsigned char)((n) >> 24);	 \
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

#define R(t)					\
	(						\
		W[t] = S1(W[t - 2]) + W[t - 7] +	\
		       S0(W[t - 15]) + W[t - 16]       \
	)

#define P(a, b, c, d, e, f, g, h, x, K) {		 \
		temp1 = h + S3(e) + F1(e, f, g) + K + x;  \
		temp2 = S2(a) + F0(a, b, c);		  \
		d += temp1; h = temp1 + temp2;		\
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

static uint8_t sha256_padding[64] =
{
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

typedef struct ddr_sha_s {
	unsigned char	sha2[SHA256_SUM_LEN];
	ddr_set_t	ddrs;
	unsigned char	sha_chip_id[MESON_CPU_CHIP_ID_SIZE];
}ddr_sha_t;

ddr_sha_t ddr_sha = { { 0 } };
ddr_set_t *ddr_set_t_p_arrary = &ddr_sha.ddrs;

typedef struct ddr_sha_s_c2 {
	unsigned char	sha2[SHA256_SUM_LEN];
	ddr_set_t_c2	ddrs;
	unsigned char	sha_chip_id[MESON_CPU_CHIP_ID_SIZE];
}ddr_sha_t_c2;

ddr_sha_t_c2 ddr_sha_c2 = { { 0 } };
ddr_set_t_c2 *ddr_set_t_p_arrary_c2 = &ddr_sha_c2.ddrs;
ddr_set_t_c2 *ddr_set_t_p = NULL;

int check_base_address(void)
{
	ddr_set_t_p = (ddr_set_t_c2 *)(ddr_set_t_p_arrary_c2);
	unsigned int table_max = (sizeof(__ddr_base_address_table)) / (sizeof(ddr_base_address_table_t));
	unsigned int table_index = 0;
	char chip_id = 0;
	chip_id = ddr_get_chip_id();
	p_ddr_base = (ddr_base_address_table_t *)(&__ddr_base_address_table);

	printf("\ntable_max=%08x,p_ddr_base_add=%08x,chip_id=%08x", table_max, (unsigned int)(unsigned long)p_ddr_base, chip_id);
	if (chip_id == 0)
		chip_id = CHIP_ID_MASK;
	if (chip_id) {
		if (chip_id == MESON_CPU_MAJOR_ID_S4D)
			chip_id = MESON_CPU_MAJOR_ID_S4;
		for (table_index = 0; table_index < table_max; table_index++) { //p_ddr_base=(p_ddr_base+1);
			printf("\ntable_index=%08x,p_ddr_base_add=%08x,(p_ddr_base->chip_id==%08x",
			       table_index, (unsigned int)(unsigned long)p_ddr_base, (p_ddr_base->chip_id));
			if ((p_ddr_base->chip_id == chip_id) && (chip_id < CHIP_ID_MASK)) {
				printf("\nfind match chip id=0x%08x ,%s", chip_id, p_ddr_base->soc_family_name);
				break;
			} else {
				printf("\nno find match chip id=0x%08x, ,%s will use default value", chip_id, p_ddr_base->soc_family_name);
			}
			p_ddr_base = (p_ddr_base + 1);
		}
	}

	int count = 0;
	for (count = 0; count < 12; count++)
		ddr_sha.sha_chip_id[count] = global_chip_id[count];

	if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T7) || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)) {
		phy_base_add[0] = p_ddr_base->ddr_phy_base_address;
		phy_base_add[1] = p_ddr_base->ddr_phy_base_address_1;
		ddr_dmc_sticky[0] = p_ddr_base->ddr_dmc_sticky0;
		ddr_dmc_sticky[1] = p_ddr_base->ddr_dmc_sticky0_1;
		ddr_dmc_apd_address[0] = p_ddr_base->ddr_dmc_apd_address;
		ddr_dmc_apd_address[1] = p_ddr_base->ddr_dmc_apd_address_1;
		ddr_dmc_asr_address[0] = p_ddr_base->ddr_dmc_asr_address;
		ddr_dmc_asr_address[1] = p_ddr_base->ddr_dmc_asr_address_1;
		dmc_retraining_ctrl_address[0] = p_ddr_base->ddr_dmc_lpdd4_retraining_address;
		dmc_retraining_ctrl_address[1] = p_ddr_base->ddr_dmc_lpdd4_retraining_address_1;
	}
	return (unsigned int)(unsigned long)(p_ddr_base);
}

void dmc_change_channel(uint32_t ch)
{
	dmc_ddr_config_channel_id = ch;
	//dmc_ddr_config_channel_id=0;
	p_ddr_base->ddr_phy_base_address = phy_base_add[dmc_ddr_config_channel_id];
	p_ddr_base->ddr_dmc_sticky0 = ddr_dmc_sticky[dmc_ddr_config_channel_id];
	p_ddr_base->ddr_dmc_lpdd4_retraining_address = dmc_retraining_ctrl_address[dmc_ddr_config_channel_id];
	p_ddr_base->ddr_dmc_apd_address = ddr_dmc_apd_address[dmc_ddr_config_channel_id];
	p_ddr_base->ddr_dmc_asr_address = ddr_dmc_asr_address[dmc_ddr_config_channel_id];
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
	printf("\nunum=0x%08x\n", unum);
	printf("\nunum2=0x%08x\n", (unum % (unsigned)radix));
	printf("\nradix=0x%08x\n", radix);
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
	return (unsigned int)(ch - '0') < 10u
	       || (unsigned int)((ch | 0x20) - 'a') < 6u;
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

	if (cp == NULL) //jiaxing add 20170616
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

	printf("str==%s\n", str_buf);

	a_num = simple_strtoull_ddr(str_buf, &endp, 0);
	printf("%s==0x%08x\n", str_buf, a_num);

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

	printf("str==%s\n", str_buf);

	sprintf(buf, "0x%08x", *a_num);

	printf("%s==0x%08x", buf, *a_num);
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

	char *str[48];
	char *endp;
	int i;
	for (i = 0; i < 48; i++)
		str_to_numarry[i] = 0;
	printf("str==%s\n", str_buf);
	for (i = 0; i < 48; i++) {
		str[i] = strsep(&str_buf, ";");
		if (str[i] == NULL)
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

	printf("str==%s\n", str_buf);


	sprintf(buf, "0x%08x", num_arry[0]);
	for (i = 1; i < 48; i++) {
		sprintf(buf_f, "%s;0x%08x", buf, num_arry[i]);
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

unsigned int dmc_sticky[64];
unsigned int sticky_reg_base_add = 0;

unsigned int global_boot_times = 0;
unsigned int watchdog_time_s = 20;
unsigned int global_ddr_clk = 1;
unsigned int bdlr_100step = 0;
unsigned int ui_1_32_100step = 0;
unsigned int error_count = 0;
unsigned int error_outof_count_flag = 0;
unsigned int copy_test_flag = 0;
unsigned int training_pattern_flag = 0;
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

unsigned int pre_fetch_enable = 0;

#define readl(addr)    (unsigned int)(*((volatile unsigned int *)((unsigned long)(unsigned int)addr)))                  //rd_reg(addr)
#define writel(data, addr)  (*((volatile unsigned int *)((unsigned long)(unsigned int)addr))) = (data)                  //wr_reg(addr, data)

#define wr_reg(addr, data)      (*((volatile unsigned int *)((unsigned long)(unsigned int)addr))) = (data)              //wr_reg(addr, data)
#define rd_reg(addr)             (unsigned int)(*((volatile unsigned int *)((unsigned long)(unsigned int)addr)))        //rd_reg(addr)

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
#define p_convter_int(a)  (unsigned int)(unsigned long)(a)
#define int_convter_p(a)  (unsigned long)(a)

#else
#define p_convter_int(a)  (unsigned int)(a)
#define int_convter_p(a)  (unsigned int)(a)
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

#define DDR_TEST_START_ADDR  0x1080000 //  0x10000000 //CONFIG_SYS_MEMTEST_START
#define DDR_TEST_SIZE 0x2000000

#if (CONFIG_CHIP >= CHIP_TXLX)

#endif

#define get_us_time()    (rd_reg(p_ddr_base->ee_timer_base_address)) // (readl(P_ISA_TIMERE))

#define ACX_MAX                              0x80

//OVERRIDE_OPTION
#define DMC_TEST_WINDOW_INDEX_ATXDLY 1
#define DMC_TEST_WINDOW_INDEX_TXDQSDLY 2
#define DMC_TEST_WINDOW_INDEX_RXCLKDLY  3
#define DMC_TEST_WINDOW_INDEX_TXDQDLY  4
#define DMC_TEST_WINDOW_INDEX_RXPBDLY  5
#define DMC_TEST_WINDOW_INDEX_RXENDLY  6
#define DMC_TEST_WINDOW_INDEX_RXPBDLY_2 7
#define DMC_TEST_WINDOW_INDEX_DFIMRL 8
#define DMC_TEST_WINDOW_INDEX_VREF 9
#define DMC_TEST_WINDOW_INDEX_RETRAINING 10
#define DMC_TEST_WINDOW_INDEX_EXTERA_COMMON 11
#define DMC_TEST_WINDOW_INDEX_EXTERA_PS 12

#define DMC_TEST_WINDOW_INDEX_RXCLKDLY_DAC1  12
#define DMC_TEST_WINDOW_INDEX_RXPBDLY_DAC1  13
#define DMC_TEST_WINDOW_INDEX_RXPBDLY_2_DAC1 14

#define DMC_TEST_WINDOW_INDEX_EE_VOLTAGE  0x11
#define DMC_TEST_WINDOW_INDEX_SOC_VREF  0x12
#define DMC_TEST_WINDOW_INDEX_SOC_VREF_DAC1  0x13
#define DMC_TEST_WINDOW_INDEX_DRAM_VREF    0x14

#define DMC_TEST_WINDOW_INDEX_DDR3_WRITE_VREF_RANG0 0x21
#define DMC_TEST_WINDOW_INDEX_DDR3_WRITE_VREF_RANG1 0x22
#define DMC_TEST_WINDOW_INDEX_DDR3_READ_VREF_RANG0 0x23
#define DMC_TEST_WINDOW_INDEX_DDR3_READ_VREF_RANG1 0x24

#define DMC_TEST_WINDOW_INDEX_DDR4_WRITE_VREF_RANG0 0x31
#define DMC_TEST_WINDOW_INDEX_DDR4_WRITE_VREF_RANG1 0x32
#define DMC_TEST_WINDOW_INDEX_DDR4_READ_VREF_RANG0 0x33
#define DMC_TEST_WINDOW_INDEX_DDR4_READ_VREF_RANG1 0x34

#define DMC_TEST_WINDOW_INDEX_LPDDR4_WRITE_VREF_RANG0 0x41
#define DMC_TEST_WINDOW_INDEX_LPDDR4_WRITE_VREF_RANG1 0x42
#define DMC_TEST_WINDOW_INDEX_LPDDR4_READ_VREF_RANG0 0x43
#define DMC_TEST_WINDOW_INDEX_LPDDR4_READ_VREF_RANG1 0x44

#define DMC_TEST_WINDOW_INDEX_LPDDR3_WRITE_VREF_RANG0 0x51
#define DMC_TEST_WINDOW_INDEX_LPDDR3_WRITE_VREF_RANG1 0x52
#define DMC_TEST_WINDOW_INDEX_LPDDR3_READ_VREF_RANG0 0x53
#define DMC_TEST_WINDOW_INDEX_LPDDR3_READ_VREF_RANG1 0x54

unsigned int dwc_ddrphy_apb_wr(unsigned int addr, unsigned int dat)
{
	*(volatile uint16_t *)(int_convter_p(((addr) << 1) + (p_ddr_base->ddr_phy_base_address))) = ((uint16_t)dat);
	return 1;
}

unsigned int dwc_ddrphy_apb_rd(unsigned int addr)
{
	return *(volatile uint16_t *)(int_convter_p(((addr) << 1) + (p_ddr_base->ddr_phy_base_address)));
}

void ddr_udelay(unsigned int us)
{
	unsigned int t0 = (rd_reg((p_ddr_base->ee_timer_base_address)));

	while ((rd_reg(((p_ddr_base->ee_timer_base_address)))) - t0 <= us) {
	}
}

void ddr_udelay_dummy(volatile unsigned int us)
{
	if ((p_ddr_base->ee_timer_base_address)) {
		ddr_udelay(us);
	} else {
		if (us == 0)
			us = 10000;

		while (us--) {
		}
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
}ddr_test_struct_t;
ddr_test_struct_t *g_ddr_test_struct;

unsigned int read_write_window_test_parameter(unsigned int source_index, unsigned int parameter_index, unsigned int parameter_value, unsigned int read_write_flag)
{
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
		sprintf(buf, "0x%08x", parameter_value);

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

unsigned int read_write_window_test_flag(unsigned int source_index, unsigned int parameter_index, unsigned int parameter_value, unsigned int read_write_flag)
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
		sprintf(buf, "0x%08x", parameter_value);

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
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_base_address) = (1 << 24) | (1 << 25) | (1 << 22) | (1 << 21) | (24000 - 1);

	// set timeout
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_base_address + 8) = msec;
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_base_address + 12) = 0;

	// enable
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_base_address) |= (1 << 18);
}

void ddr_test_watchdog_clear(void)
{
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_base_address + 12) = 0;
}

void ddr_test_watchdog_disable(void)
{
	// turn off internal counter and disable
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_base_address) &= ~((1 << 18) | (1 << 25));
}

void ddr_test_watchdog_enable(uint32_t sec)
{
	// src: 24MHz
	// div: 24000 for 1ms
	// reset ao-22 and ee-21
#if (CONFIG_DDR_PHY > P_DDR_PHY_DEFAULT)
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_base_address) = (1 << 24) | (1 << 25) | (1 << 23) | (1 << 21) | (240000 - 1); //10ms
	if (sec * 100 > 0xffff)
		*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_base_address + 8) = 0xffff;
	else
		*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_base_address + 8) = sec * 100;  //max 655s
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_base_address + 12) = 0;

	// enable
	*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_base_address) = (*(volatile uint32_t *)(unsigned long)(p_ddr_base->sys_watchdog_base_address)) | (1 << 18);
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
	writel(0, (p_ddr_base->sys_watchdog_base_address + 12));
	while (1) {
		writel(p_ddr_base->sys_watchdog_enable_value
		       , (p_ddr_base->sys_watchdog_base_address));
		writel(0, (p_ddr_base->sys_watchdog_base_address + 12));
		writel((p_ddr_base->sys_watchdog_enable_value) | (1 << 18),
		       (p_ddr_base->sys_watchdog_base_address));                // watchdog en
		for (i = 0; i < 100; i++)
			readl((p_ddr_base->sys_watchdog_base_address));         /*Deceive gcc for waiting some cycles */
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), TDATA32F);
					}
					break;
				case 1:
				case 6:
				case 8:
				case 17:
				case 22:
					if (*(p + i) != 0) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0);
					}
					break;
				case 16:
				case 23:
				case 31:
					if (*(p + i) != TDATA32A) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), TDATA32A);
					}
					break;
				case 7:
				case 15:
				case 24:
					if (*(p + i) != TDATA325) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), TDATA325);
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 1 << j);
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~(1 << j));
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), TDATA32F);
					}
					break;
				case 4:
				case 5:
				case 6:
				case 7:
					if (*(p + i) != ~0xff00ff00) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), TDATA32F);
					}
					break;
				case 8:
				case 9:
				case 10:
				case 11:
					if (*(p + i) != 0xaa55aa55) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), TDATA32F);
					}
					break;
				case 12:
				case 13:
				case 14:
				case 15:
					if (*(p + i) != ~0xaa55aa55) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), TDATA32F);
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), TDATA32F);
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), TDATA32F);
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
			printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i),
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
	m_len = m_len / 4;              //assume it's multiple of 4
	while (m_len--) {
		ddr_pld_cache(p_src);   //#define ddr_pld_cache(P)   asm ("prfm PLDL1KEEP, [%0, #376]"::"r" (P))
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

	printf("\nargc== 0x%08x\n", argc);
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
		ddr_test_copy((void *)(int_convter_p(dec_addr)), (void *)(int_convter_p(src_addr)), test_size);
		if (print_flag) {
			printf("\nloop==0x%08x", (unsigned int)loop);
			printf("\n      \n");
		}
	} while (--loop);
	time_end = get_us_time(); //us
	printf("\ncopy %d times use %dus\n                             \n", (unsigned int)test_loops, (unsigned int)(time_end - time_start));

	printf("\nddr copy bandwidth==%d MBYTE/S \n                             \n", (unsigned int)(size_count / (time_end - time_start)));
	printf("\rEnd ddr test.                              \n");

	unsigned int m_len = 0, counter = 0;
	unsigned int *p_dest;
	p_dest = (void *)(int_convter_p(dec_addr));
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
	printf("\nwrite %d bytes use %dus\n                             \n", (unsigned int)test_size, (unsigned int)(time_end - time_start));

	printf("\nddr write bandwidth==%d MBYTE/S \n                             \n", (unsigned int)(size_count / (time_end - time_start)));

	unsigned int *p_src;
	p_src = (void *)(int_convter_p(src_addr));
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
			__asm__ __volatile__ ("prfm PLDL1KEEP, [%0, #376]" ::"r" (p_src));
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

	printf("\nread %d Kbytes use %dus\n                             \n", (unsigned int)(size_count / 1000), (unsigned int)(time_end - time_start));
	printf("\nddr read bandwidth==%d MBYTE/S \n                             \n", (unsigned int)(size_count / (time_end - time_start)));

	return 0;
}

U_BOOT_CMD(
	ddr_test_copy, 7, 1, do_ddr_test_copy,
	"ddr_test_copy function",
	"ddr_test_copy  0x08000000 0x10000000 0x02000000 1 0 ? \n"
	);


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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), TDATA32F);
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0);
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), TDATA32A);
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), TDATA325);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 0:
				case DDR_PATTERN_LOOP_2 + 1:
				case DDR_PATTERN_LOOP_2 + 2:
				case DDR_PATTERN_LOOP_2 + 3:
					if (*(p + i) != 0xfe01fe01) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xfe01fe01);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 4:
				case DDR_PATTERN_LOOP_2 + 5:
				case DDR_PATTERN_LOOP_2 + 6:
				case DDR_PATTERN_LOOP_2 + 7:
					if (*(p + i) != 0xfd02fd02) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xfd02fd02);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 8:
				case DDR_PATTERN_LOOP_2 + 9:
				case DDR_PATTERN_LOOP_2 + 10:
				case DDR_PATTERN_LOOP_2 + 11:
					if (*(p + i) != 0xfb04fb04) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xfb04fb04);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 12:
				case DDR_PATTERN_LOOP_2 + 13:
				case DDR_PATTERN_LOOP_2 + 14:
				case DDR_PATTERN_LOOP_2 + 15:
					if (*(p + i) != 0xf708f708) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xf708f708);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 16:
				case DDR_PATTERN_LOOP_2 + 17:
				case DDR_PATTERN_LOOP_2 + 18:
				case DDR_PATTERN_LOOP_2 + 19:
					if (*(p + i) != 0xef10ef10) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xef10ef10);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 20:
				case DDR_PATTERN_LOOP_2 + 21:
				case DDR_PATTERN_LOOP_2 + 22:
				case DDR_PATTERN_LOOP_2 + 23:
					if (*(p + i) != 0xdf20df20) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xdf20df20);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 24:
				case DDR_PATTERN_LOOP_2 + 25:
				case DDR_PATTERN_LOOP_2 + 26:
				case DDR_PATTERN_LOOP_2 + 27:
					if (*(p + i) != 0xbf40bf40) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xbf40bf40);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_2 + 28:
				case DDR_PATTERN_LOOP_2 + 29:
				case DDR_PATTERN_LOOP_2 + 30:
				case DDR_PATTERN_LOOP_2 + 31:
					if (*(p + i) != 0x7f807f80) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0x7f807f80);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_3 + 0:
				case DDR_PATTERN_LOOP_3 + 1:
				case DDR_PATTERN_LOOP_3 + 2:
				case DDR_PATTERN_LOOP_3 + 3:
					if (*(p + i) != 0x00000100) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0x00000100);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_3 + 4:
				case DDR_PATTERN_LOOP_3 + 5:
				case DDR_PATTERN_LOOP_3 + 6:
				case DDR_PATTERN_LOOP_3 + 7:
					if (*(p + i) != 0x00000200) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0x00000200);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_3 + 8:
				case DDR_PATTERN_LOOP_3 + 9:
				case DDR_PATTERN_LOOP_3 + 10:
				case DDR_PATTERN_LOOP_3 + 11:
					if (*(p + i) != 0x00000400) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0x00000400);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_3 + 12:
				case DDR_PATTERN_LOOP_3 + 13:
				case DDR_PATTERN_LOOP_3 + 14:
				case DDR_PATTERN_LOOP_3 + 15:
					if (*(p + i) != 0x00000800) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0x00000800);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_3 + 16:
				case DDR_PATTERN_LOOP_3 + 17:
				case DDR_PATTERN_LOOP_3 + 18:
				case DDR_PATTERN_LOOP_3 + 19:
					if (*(p + i) != 0x00001000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0x00001000);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_3 + 20:
				case DDR_PATTERN_LOOP_3 + 21:
				case DDR_PATTERN_LOOP_3 + 22:
				case DDR_PATTERN_LOOP_3 + 23:
					if (*(p + i) != 0x00002000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0x00002000);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 24:
				case DDR_PATTERN_LOOP_3 + 25:
				case DDR_PATTERN_LOOP_3 + 26:
				case DDR_PATTERN_LOOP_3 + 27:
					if (*(p + i) != 0x00004000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0x00004000);
						break;
					}
					break;
				case DDR_PATTERN_LOOP_3 + 28:
				case DDR_PATTERN_LOOP_3 + 29:
				case DDR_PATTERN_LOOP_3 + 30:
				case DDR_PATTERN_LOOP_3 + 31:
					if (*(p + i) != 0x00008000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0x00008000);
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xfe01fe01);
						break;
					}
					break;
				case 4:
				case DDR_PATTERN_LOOP_1 + 5:
				case DDR_PATTERN_LOOP_2 + 6:
				case DDR_PATTERN_LOOP_3 + 7:
					if (*(p + i) != 0xfd02fd02) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xfd02fd02);
						break;
					}
					break;

				case 8:
				case DDR_PATTERN_LOOP_1 + 9:
				case DDR_PATTERN_LOOP_2 + 10:
				case DDR_PATTERN_LOOP_3 + 11:
					if (*(p + i) != 0xfb04fb04) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xfb04fb04);
						break;
					}
					break;

				case 12:
				case DDR_PATTERN_LOOP_1 + 13:
				case DDR_PATTERN_LOOP_2 + 14:
				case DDR_PATTERN_LOOP_3 + 15:
					if (*(p + i) != 0xf708f708) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xf708f708);
						break;
					}
					break;

				case 16:
				case DDR_PATTERN_LOOP_1 + 17:
				case DDR_PATTERN_LOOP_2 + 18:
				case DDR_PATTERN_LOOP_3 + 19:
					if (*(p + i) != 0xef10ef10) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xef10ef10);
						break;
					}
					break;

				case 20:
				case DDR_PATTERN_LOOP_1 + 21:
				case DDR_PATTERN_LOOP_2 + 22:
				case DDR_PATTERN_LOOP_3 + 23:
					if (*(p + i) != 0xdf20df20) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xdf20df20);
						break;
					}
					break;

				case 24:
				case DDR_PATTERN_LOOP_1 + 25:
				case DDR_PATTERN_LOOP_2 + 26:
				case DDR_PATTERN_LOOP_3 + 27:
					if (*(p + i) != 0xbf40bf40) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xbf40bf40);
						break;
					}
					break;
				case 28:
				case DDR_PATTERN_LOOP_1 + 29:
				case DDR_PATTERN_LOOP_2 + 30:
				case DDR_PATTERN_LOOP_3 + 31:
					if (*(p + i) != 0x7f807f80) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0x7f807f80);
						break;
					}
					break;


				default:
					if (*(p + i) != 0xff00ff00) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xff00ff00);
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~TDATA32F);
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0);
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~TDATA32A);
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~TDATA325);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 0:
				case DDR_PATTERN_LOOP_2 + 1:
				case DDR_PATTERN_LOOP_2 + 2:
				case DDR_PATTERN_LOOP_2 + 3:
					if (*(p + i) != ~0xfe01fe01) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xfe01fe01);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 4:
				case DDR_PATTERN_LOOP_2 + 5:
				case DDR_PATTERN_LOOP_2 + 6:
				case DDR_PATTERN_LOOP_2 + 7:
					if (*(p + i) != ~0xfd02fd02) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xfd02fd02);
					}
					break;

				case DDR_PATTERN_LOOP_2 + 8:
				case DDR_PATTERN_LOOP_2 + 9:
				case DDR_PATTERN_LOOP_2 + 10:
				case DDR_PATTERN_LOOP_2 + 11:
					if (*(p + i) != ~0xfb04fb04) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xfb04fb04);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 12:
				case DDR_PATTERN_LOOP_2 + 13:
				case DDR_PATTERN_LOOP_2 + 14:
				case DDR_PATTERN_LOOP_2 + 15:
					if (*(p + i) != ~0xf708f708) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xf708f708);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 16:
				case DDR_PATTERN_LOOP_2 + 17:
				case DDR_PATTERN_LOOP_2 + 18:
				case DDR_PATTERN_LOOP_2 + 19:
					if (*(p + i) != ~0xef10ef10) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xef10ef10);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 20:
				case DDR_PATTERN_LOOP_2 + 21:
				case DDR_PATTERN_LOOP_2 + 22:
				case DDR_PATTERN_LOOP_2 + 23:
					if (*(p + i) != ~0xdf20df20) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xdf20df20);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 24:
				case DDR_PATTERN_LOOP_2 + 25:
				case DDR_PATTERN_LOOP_2 + 26:
				case DDR_PATTERN_LOOP_2 + 27:
					if (*(p + i) != ~0xbf40bf40) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xbf40bf40);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 28:
				case DDR_PATTERN_LOOP_2 + 29:
				case DDR_PATTERN_LOOP_2 + 30:
				case DDR_PATTERN_LOOP_2 + 31:
					if (*(p + i) != ~0x7f807f80) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0x7f807f80);
					}
					break;
					break;
				case DDR_PATTERN_LOOP_3 + 0:
				case DDR_PATTERN_LOOP_3 + 1:
				case DDR_PATTERN_LOOP_3 + 2:
				case DDR_PATTERN_LOOP_3 + 3:
					if (*(p + i) != ~0x00000100) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0x00000100);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 4:
				case DDR_PATTERN_LOOP_3 + 5:
				case DDR_PATTERN_LOOP_3 + 6:
				case DDR_PATTERN_LOOP_3 + 7:
					if (*(p + i) != ~0x00000200) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0x00000200);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 8:
				case DDR_PATTERN_LOOP_3 + 9:
				case DDR_PATTERN_LOOP_3 + 10:
				case DDR_PATTERN_LOOP_3 + 11:
					if (*(p + i) != ~0x00000400) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0x00000400);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 12:
				case DDR_PATTERN_LOOP_3 + 13:
				case DDR_PATTERN_LOOP_3 + 14:
				case DDR_PATTERN_LOOP_3 + 15:
					if (*(p + i) != ~0x00000800) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0x00000800);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 16:
				case DDR_PATTERN_LOOP_3 + 17:
				case DDR_PATTERN_LOOP_3 + 18:
				case DDR_PATTERN_LOOP_3 + 19:
					if (*(p + i) != ~0x00001000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0x00001000);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 20:
				case DDR_PATTERN_LOOP_3 + 21:
				case DDR_PATTERN_LOOP_3 + 22:
				case DDR_PATTERN_LOOP_3 + 23:
					if (*(p + i) != ~0x00002000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0x00002000);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 24:
				case DDR_PATTERN_LOOP_3 + 25:
				case DDR_PATTERN_LOOP_3 + 26:
				case DDR_PATTERN_LOOP_3 + 27:
					if (*(p + i) != ~0x00004000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0x00004000);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 28:
				case DDR_PATTERN_LOOP_3 + 29:
				case DDR_PATTERN_LOOP_3 + 30:
				case DDR_PATTERN_LOOP_3 + 31:
					if (*(p + i) != ~0x00008000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0x00008000);
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

static void ddr_read_pattern4_cross_talk_n2(void *buff, unsigned int m_length)
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xfe01fe01);
						break;
					}
					break;
				case 4:
				case DDR_PATTERN_LOOP_1 + 5:
				case DDR_PATTERN_LOOP_2 + 6:
				case DDR_PATTERN_LOOP_3 + 7:
					if (*(p + i) != ~0xfd02fd02) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xfd02fd02);
						break;
					}
					break;

				case 8:
				case DDR_PATTERN_LOOP_1 + 9:
				case DDR_PATTERN_LOOP_2 + 10:
				case DDR_PATTERN_LOOP_3 + 11:
					if (*(p + i) != ~0xfb04fb04) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xfb04fb04);
						break;
					}
					break;

				case 12:
				case DDR_PATTERN_LOOP_1 + 13:
				case DDR_PATTERN_LOOP_2 + 14:
				case DDR_PATTERN_LOOP_3 + 15:
					if (*(p + i) != ~0xf708f708) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xf708f708);
						break;
					}
					break;

				case 16:
				case DDR_PATTERN_LOOP_1 + 17:
				case DDR_PATTERN_LOOP_2 + 18:
				case DDR_PATTERN_LOOP_3 + 19:
					if (*(p + i) != ~0xef10ef10) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xef10ef10);
						break;
					}
					break;

				case 20:
				case DDR_PATTERN_LOOP_1 + 21:
				case DDR_PATTERN_LOOP_2 + 22:
				case DDR_PATTERN_LOOP_3 + 23:
					if (*(p + i) != ~0xdf20df20) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xdf20df20);
						break;
					}
					break;

				case 24:
				case DDR_PATTERN_LOOP_1 + 25:
				case DDR_PATTERN_LOOP_2 + 26:
				case DDR_PATTERN_LOOP_3 + 27:
					if (*(p + i) != ~0xbf40bf40) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xbf40bf40);
						break;
					}
					break;
				case 28:
				case DDR_PATTERN_LOOP_1 + 29:
				case DDR_PATTERN_LOOP_2 + 30:
				case DDR_PATTERN_LOOP_3 + 31:
					if (*(p + i) != ~0x7f807f80) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0x7f807f80);
						break;
					}
					break;


				default:
					if (*(p + i) != ~0xff00ff00) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xff00ff00);
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
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xff00ff00);
					}
					break;
				case 4:
				case 5:
				case 6:
				case 7:
					if (*(p + i) != 0xffff0000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xffff0000);
					}
					break;

				case 8:
				case 9:
				case 10:
				case 11:
					if (*(p + i) != 0xff000000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xff000000);
					}
					break;
				case 12:
				case 13:
				case 14:
				case 15:
					if (*(p + i) != 0xff00ffff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xff00ffff);
					}
					break;

				case 16:
				case 17:
				case 18:
				case 19:
					if (*(p + i) != 0xff00ffff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xff00ffff);
					}
					break;
				case 20:
				case 21:
				case 22:
				case 23:
					if (*(p + i) != 0xff0000ff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xff0000ff);
					}
					break;
				case 24:
				case 25:
				case 26:
				case 27:
					if (*(p + i) != 0xffff0000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xffff0000);
					}
					break;

				case 28:
				case 29:
				case 30:
				case 31:
					if (*(p + i) != 0x00ff00ff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0x00ff00ff);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 0:
				case DDR_PATTERN_LOOP_1 + 1:
				case DDR_PATTERN_LOOP_1 + 2:
				case DDR_PATTERN_LOOP_1 + 3:
					if (*(p + i) != ~0xff00ff00) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xff00ff00);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 4:
				case DDR_PATTERN_LOOP_1 + 5:
				case DDR_PATTERN_LOOP_1 + 6:
				case DDR_PATTERN_LOOP_1 + 7:
					if (*(p + i) != ~0xffff0000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xffff0000);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 8:
				case DDR_PATTERN_LOOP_1 + 9:
				case DDR_PATTERN_LOOP_1 + 10:
				case DDR_PATTERN_LOOP_1 + 11:
					if (*(p + i) != ~0xff000000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xff000000);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 12:
				case DDR_PATTERN_LOOP_1 + 13:
				case DDR_PATTERN_LOOP_1 + 14:
				case DDR_PATTERN_LOOP_1 + 15:
					if (*(p + i) != ~0xff00ffff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xff00ffff);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 16:
				case DDR_PATTERN_LOOP_1 + 17:
				case DDR_PATTERN_LOOP_1 + 18:
				case DDR_PATTERN_LOOP_1 + 19:
					if (*(p + i) != ~0xff00ffff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xff00ffff);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 20:
				case DDR_PATTERN_LOOP_1 + 21:
				case DDR_PATTERN_LOOP_1 + 22:
				case DDR_PATTERN_LOOP_1 + 23:
					if (*(p + i) != ~0xff00ffff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xff00ffff);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 24:
				case DDR_PATTERN_LOOP_1 + 25:
				case DDR_PATTERN_LOOP_1 + 26:
				case DDR_PATTERN_LOOP_1 + 27:
					if (*(p + i) != ~0xffff0000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xffff0000);
					}
					break;
				case DDR_PATTERN_LOOP_1 + 28:
				case DDR_PATTERN_LOOP_1 + 29:
				case DDR_PATTERN_LOOP_1 + 30:
				case DDR_PATTERN_LOOP_1 + 31:
					if (*(p + i) != ~0x00ff00ff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0x00ff00ff);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 0:
				case DDR_PATTERN_LOOP_2 + 1:
				case DDR_PATTERN_LOOP_2 + 2:
				case DDR_PATTERN_LOOP_2 + 3:
					if (*(p + i) != 0x00ff0000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0x00ff0000);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 4:
				case DDR_PATTERN_LOOP_2 + 5:
				case DDR_PATTERN_LOOP_2 + 6:
				case DDR_PATTERN_LOOP_2 + 7:
					if (*(p + i) != 0xff000000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xff000000);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 8:
				case DDR_PATTERN_LOOP_2 + 9:
				case DDR_PATTERN_LOOP_2 + 10:
				case DDR_PATTERN_LOOP_2 + 11:
					if (*(p + i) != 0x0000ffff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0x0000ffff);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 12:
				case DDR_PATTERN_LOOP_2 + 13:
				case DDR_PATTERN_LOOP_2 + 14:
				case DDR_PATTERN_LOOP_2 + 15:
					if (*(p + i) != 0x000000ff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0x000000ff);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 16:
				case DDR_PATTERN_LOOP_2 + 17:
				case DDR_PATTERN_LOOP_2 + 18:
				case DDR_PATTERN_LOOP_2 + 19:
					if (*(p + i) != 0x00ff00ff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0x00ff00ff);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 20:
				case DDR_PATTERN_LOOP_2 + 21:
				case DDR_PATTERN_LOOP_2 + 22:
				case DDR_PATTERN_LOOP_2 + 23:
					if (*(p + i) != 0xff00ff00) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xff00ff00);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 24:
				case DDR_PATTERN_LOOP_2 + 25:
				case DDR_PATTERN_LOOP_2 + 26:
				case DDR_PATTERN_LOOP_2 + 27:
					if (*(p + i) != 0xff00ffff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xff00ffff);
					}
					break;
				case DDR_PATTERN_LOOP_2 + 28:
				case DDR_PATTERN_LOOP_2 + 29:
				case DDR_PATTERN_LOOP_2 + 30:
				case DDR_PATTERN_LOOP_2 + 31:
					if (*(p + i) != 0xff00ff00) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), 0xff00ff00);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 0:
				case DDR_PATTERN_LOOP_3 + 1:
				case DDR_PATTERN_LOOP_3 + 2:
				case DDR_PATTERN_LOOP_3 + 3:
					if (*(p + i) != ~0x00ff0000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0x00ff0000);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 4:
				case DDR_PATTERN_LOOP_3 + 5:
				case DDR_PATTERN_LOOP_3 + 6:
				case DDR_PATTERN_LOOP_3 + 7:
					if (*(p + i) != ~0xff000000) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xff000000);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 8:
				case DDR_PATTERN_LOOP_3 + 9:
				case DDR_PATTERN_LOOP_3 + 10:
				case DDR_PATTERN_LOOP_3 + 11:
					if (*(p + i) != ~0x0000ffff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0x0000ffff);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 12:
				case DDR_PATTERN_LOOP_3 + 13:
				case DDR_PATTERN_LOOP_3 + 14:
				case DDR_PATTERN_LOOP_3 + 15:
					if (*(p + i) != ~0x000000ff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0x000000ff);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 16:
				case DDR_PATTERN_LOOP_3 + 17:
				case DDR_PATTERN_LOOP_3 + 18:
				case DDR_PATTERN_LOOP_3 + 19:
					if (*(p + i) != ~0x00ff00ff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0x00ff00ff);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 20:
				case DDR_PATTERN_LOOP_3 + 21:
				case DDR_PATTERN_LOOP_3 + 22:
				case DDR_PATTERN_LOOP_3 + 23:
					if (*(p + i) != ~0xff00ff00) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xff00ff00);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 24:
				case DDR_PATTERN_LOOP_3 + 25:
				case DDR_PATTERN_LOOP_3 + 26:
				case DDR_PATTERN_LOOP_3 + 27:
					if (*(p + i) != ~0xff00ffff) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xff00ffff);
					}
					break;
				case DDR_PATTERN_LOOP_3 + 28:
				case DDR_PATTERN_LOOP_3 + 29:
				case DDR_PATTERN_LOOP_3 + 30:
				case DDR_PATTERN_LOOP_3 + 31:
					if (*(p + i) != ~0xff00ff00) {
						error_count++;
						printf("Error data [0x%08x] at offset 0x%08x[0x%08x]\n", *(p + i), p_convter_int(p + i), ~0xff00ff00);
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
	printf("\nargc== 0x%08x\n", argc);
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
		if ((strcmp(argv[1], "p") == 0) || (strcmp(argv[2], "p") == 0) || (strcmp(argv[3], "p") == 0))
			copy_test_flag = 1;
		if ((strcmp(argv[1], "n") == 0) || (strcmp(argv[2], "n") == 0) || (strcmp(argv[3], "n") == 0))
			print_flag = 0;
		if ((strcmp(argv[1], "s") == 0) || (strcmp(argv[2], "s") == 0) || (strcmp(argv[3], "s") == 0)) {
			simple_pattern_flag = 1;
			old_pattern_flag = 0;
			cross_talk_pattern_flag = 0;
		}
		if ((strcmp(argv[1], "c") == 0) || (strcmp(argv[2], "c") == 0) || (strcmp(argv[3], "c") == 0)) {
			simple_pattern_flag = 0;
			old_pattern_flag = 0;
			cross_talk_pattern_flag = 1;
		}
		if ((strcmp(argv[1], "e") == 0) || (strcmp(argv[2], "e") == 0) || (strcmp(argv[3], "e") == 0))
			error_outof_count_flag = 1;
	}

DDR_TEST_START:

	do {
		if (lflag)
			loop = 888;

		if (old_pattern_flag == 1) {
			{
				if (print_flag)
					printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + test_size);
				ddr_write((void *)(int_convter_p(start_addr)), test_size);
				if (print_flag) {
					printf("\nEnd write.                                 ");
					printf("\nStart 1st reading...                       ");
				}
				ddr_read((void *)(int_convter_p(start_addr)), test_size);
				if (print_flag) {
					printf("\nEnd 1st read.                              ");
					printf("\nStart 2nd reading...                       ");
				}
				ddr_read((void *)(int_convter_p(start_addr)), test_size);
				if (print_flag) {
					printf("\nEnd 2nd read.                              ");
					printf("\nStart 3rd reading...                       ");
				}
				ddr_read((void *)(int_convter_p(start_addr)), test_size);
				if (print_flag)
					printf("\nEnd 3rd read.                              \n");

				if (copy_test_flag) {
					if (print_flag)
						printf("\n copy_test_flag = 1,start copy test.                              \n");
					ddr_test_copy((void *)(int_convter_p(start_addr + test_size / 2)), (void *)(int_convter_p(start_addr)), test_size / 2);
					ddr_read((void *)(int_convter_p(start_addr + test_size / 2)), test_size / 2);
					ddr_read((void *)(int_convter_p(start_addr + test_size / 2)), test_size / 2);
				}
			}
			{
				if (print_flag) {
					printf("\nStart *4 normal pattern.                                 ");
					printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + test_size);
				}
				ddr_write4((void *)(int_convter_p(start_addr)), test_size);
				if (print_flag) {
					printf("\nEnd write.                                 ");
					printf("\nStart 1st reading...                       ");
				}
				ddr_read4((void *)(int_convter_p(start_addr)), test_size);
				if (print_flag) {
					printf("\nEnd 1st read.                              ");
					printf("\nStart 2nd reading...                       ");
				}
				ddr_read4((void *)(int_convter_p(start_addr)), test_size);
				if (print_flag) {
					printf("\nEnd 2nd read.                              ");
					printf("\nStart 3rd reading...                       ");
				}
				ddr_read4((void *)(int_convter_p(start_addr)), test_size);
				if (print_flag)
					printf("\rEnd 3rd read.                              \n");
				if (copy_test_flag) {
					ddr_test_copy((void *)(int_convter_p(start_addr + test_size / 2)), (void *)(int_convter_p(start_addr)), test_size / 2);
					ddr_read4((void *)(int_convter_p(start_addr + test_size / 2)), test_size / 2);
					ddr_read4((void *)(int_convter_p(start_addr + test_size / 2)), test_size / 2);
				}
			}
		}

		if (simple_pattern_flag == 1) {
			if (print_flag) {
				printf("\nStart *4 no cross talk pattern.                                 ");
				printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + test_size);
			}
			ddr_write_pattern4_no_cross_talk((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd write.                                 ");
				printf("\rStart 1st reading...                       ");
			}
			ddr_read_pattern4_no_cross_talk((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd 1st read.                              ");
				printf("\rStart 2nd reading...                       ");
			}
			ddr_read_pattern4_no_cross_talk((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd 2nd read.                              ");
				printf("\rStart 3rd reading...                       ");
			}
			ddr_read_pattern4_no_cross_talk((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag)
				printf("\rEnd 3rd read.                              \n");

			if (copy_test_flag) {
				ddr_test_copy((void *)(int_convter_p(start_addr + test_size / 2)), (void *)(int_convter_p(start_addr)), test_size / 2);
				ddr_read_pattern4_no_cross_talk((void *)(int_convter_p(start_addr + test_size / 2)), test_size / 2);
				ddr_read_pattern4_no_cross_talk((void *)(int_convter_p(start_addr + test_size / 2)), test_size / 2);
			}
		}

		if (cross_talk_pattern_flag == 1) {
			if (print_flag) {
				printf("\nStart *4  cross talk pattern p.                                 ");
				printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + test_size);
			}
			ddr_write_pattern4_cross_talk_p((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd write.                                 ");
				printf("\rStart 1st reading...                       ");
			}
			ddr_read_pattern4_cross_talk_p((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd 1st read.                              ");
				printf("\rStart 2nd reading...                       ");
			}
			ddr_read_pattern4_cross_talk_p((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd 2nd read.                              ");
				printf("\rStart 3rd reading...                       ");
			}
			ddr_read_pattern4_cross_talk_p((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd 3rd read.                              \n");

				printf("\nStart *4  cross talk pattern n.                                 ");
				printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + test_size);
			}
			ddr_write_pattern4_cross_talk_n((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd write.                                 ");
				printf("\rStart 1st reading...                       ");
			}
			ddr_read_pattern4_cross_talk_n((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd 1st read.                              ");
				printf("\rStart 2nd reading...                       ");
			}
			ddr_read_pattern4_cross_talk_n((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd 2nd read.                              ");
				printf("\rStart 3rd reading...                       ");
			}
			ddr_read_pattern4_cross_talk_n((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd 3rd read.                              \n");

				///*
				printf("\nStart *4  cross talk pattern p2.                                 ");
				printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + test_size);
			}
			ddr_write_pattern4_cross_talk_p2((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd write.                                 ");
				printf("\rStart 1st reading...                       ");
			}
			ddr_read_pattern4_cross_talk_p2((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd 1st read.                              ");
				printf("\rStart 2nd reading...                       ");
			}
			ddr_read_pattern4_cross_talk_p2((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd 2nd read.                              ");
				printf("\rStart 3rd reading...                       ");
			}
			ddr_read_pattern4_cross_talk_p2((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd 3rd read.                              \n");

				printf("\nStart *4  cross talk pattern n2.                                 ");
				printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + test_size);
			}
			ddr_write_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd write.                                 ");
				printf("\rStart 1st reading...                       ");
			}
			ddr_read_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd 1st read.                              ");
				printf("\rStart 2nd reading...                       ");
			}
			ddr_read_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag) {
				printf("\rEnd 2nd read.                              ");
				printf("\rStart 3rd reading...                       ");
			}
			ddr_read_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr)), test_size);
			if (print_flag)
				printf("\rEnd 3rd read.                              \n");

			if (copy_test_flag) {
				ddr_test_copy((void *)(int_convter_p(start_addr + test_size / 2)), (void *)(int_convter_p(start_addr)), test_size / 2);
				ddr_read_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr + test_size / 2)), test_size / 2);
				ddr_read_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr + test_size / 2)), test_size / 2);
			}
		}

		if (print_flag)
			printf("\nError count==0x%08x", error_count);
	} while (--loop);

	printf("\rEnd ddr test.                              \n");

	return 0;

usage:
	cmd_usage(cmdtp);
	return 1;
}

U_BOOT_CMD(
	ddrtest, 5, 1, do_ddr_test,
	"DDR test function",
	"ddrtest [LOOP] [ADDR].Default address is 0x8d000000\n"
	);

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
	printf("\nargc== 0x%08x\n", argc);
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
				printf("\nwrite_times==0x%08x \n", ((unsigned int)i));
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
				printf("\nread_times==0x%08x \n", ((unsigned int)i));
				test_addr = start_addr;
				test_val = base_pattern;
				count = (test_size >> 2);

				do {
					if (test_val != (readl((unsigned long)test_addr)))

						printf("\nadd==0x%08x,pattern==0x%08x,read==0x%08x \n", ((unsigned int)test_addr), ((unsigned int)test_val), (readl((unsigned int)test_addr)));
					test_addr = test_addr + 4;
					if (inc_flag)
						test_val = test_val + 1;
				} while (count--);
			}
		}



		if (print_flag)
			printf("\nError count==0x%08x", error_count);
	} while (--loop);

	printf("\rEnd ddr test.                              \n");

	return 0;

//usage:
	//cmd_usage(cmdtp);
	//return 1;
}
U_BOOT_CMD(
	ddr_spec_test, 8, 1, do_ddr_special_test,
	"DDR test function",
	"ddrtest [LOOP] [ADDR] [size] [write_times] [read times] [pattern] [inc].ddr_spec_test 1 0x1080000 0x200000 1  3 1 1 \n"
	);

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
		printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + ddr_test_size);
		ddr_write((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\nEnd write.                                 ");
		printf("\nStart 1st reading...                       ");
		ddr_read((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\nEnd 1st read.                              ");
		printf("\nStart 2nd reading...                       ");
		ddr_read((void *)(int_convter_p(start_addr)), ddr_test_size);
		if (error_count)
			return error_count;
		printf("\nStart writing pattern4 at 0x%08x - 0x%08x...", start_addr, start_addr + ddr_test_size);
		ddr_write4((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\nEnd write.                                 ");
		printf("\nStart 1st reading...                       ");
		ddr_read4((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\nEnd 1st read.                              ");
		printf("\nStart 2nd reading...                       ");
		ddr_read4((void *)(int_convter_p(start_addr)), ddr_test_size);

		if (error_count)
			return error_count;
		printf("\nStart *4 no cross talk pattern.                                 ");
		printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + ddr_test_size);
		ddr_write_pattern4_no_cross_talk((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\nEnd write.                                 ");
		printf("\nStart 1st reading...                       ");
		ddr_read_pattern4_no_cross_talk((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\nEnd 1st read.                              ");
		printf("\nStart 2nd reading...                       ");
		ddr_read_pattern4_no_cross_talk((void *)(int_convter_p(start_addr)), ddr_test_size);
	}

	if (error_count)
		return error_count;
	{
		printf("\nStart *4  cross talk pattern p.                                 ");
		printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + ddr_test_size);
		ddr_write_pattern4_cross_talk_p((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_pattern4_cross_talk_p((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_pattern4_cross_talk_p((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 2nd read.                              ");

		if (error_count)
			return error_count;
		printf("\nStart *4  cross talk pattern n.                                 ");
		printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + ddr_test_size);
		ddr_write_pattern4_cross_talk_n((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_pattern4_cross_talk_n((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_pattern4_cross_talk_n((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 2nd read.                              ");
	}
	if (error_count)
		return error_count;
	{
		printf("\nStart *4  cross talk pattern p2.                                 ");
		printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + ddr_test_size);
		ddr_write_pattern4_cross_talk_p2((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_pattern4_cross_talk_p2((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_pattern4_cross_talk_p2((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 2nd read.                              ");

		if (error_count)
			return error_count;
		printf("\nStart *4  cross talk pattern n.                                 ");
		printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + ddr_test_size);
		ddr_write_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 2nd read.                              ");

		if (copy_test_flag) {
			if (error_count)
				return error_count;
			printf("\n start copy test  ...                            ");
			ddr_test_copy((void *)(int_convter_p(start_addr + ddr_test_size / 2)), (void *)(int_convter_p(start_addr)), ddr_test_size / 2);
			ddr_read_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr + ddr_test_size / 2)), ddr_test_size / 2);
			ddr_read_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr + ddr_test_size / 2)), ddr_test_size / 2);
		}
	}

	if (error_count)
		return 1;
	else
		return 0;
}

int ddr_test_s_cross_talk_pattern_quick_retrun(int ddr_test_size)
{
	error_outof_count_flag = 1;
#define TEST_OFFSET  0        //0X40000000
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
		printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + ddr_test_size);
		ddr_write((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\nEnd write.                                 ");
		printf("\nStart 1st reading...                       ");
		ddr_read((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\nEnd 1st read.                              ");
		printf("\nStart 2nd reading...                       ");
		ddr_read((void *)(int_convter_p(start_addr)), ddr_test_size);

		printf("\nStart writing pattern4 at 0x%08x - 0x%08x...", start_addr, start_addr + ddr_test_size);
		ddr_write4((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\nEnd write.                                 ");
		printf("\nStart 1st reading...                       ");
		ddr_read4((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\nEnd 1st read.                              ");
		printf("\nStart 2nd reading...                       ");
		ddr_read4((void *)(int_convter_p(start_addr)), ddr_test_size);

		printf("\nStart *4 no cross talk pattern.                                 ");
		printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + ddr_test_size);
		ddr_write_pattern4_no_cross_talk((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\nEnd write.                                 ");
		printf("\nStart 1st reading...                       ");
		ddr_read_pattern4_no_cross_talk((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\nEnd 1st read.                              ");
		printf("\nStart 2nd reading...                       ");
		ddr_read_pattern4_no_cross_talk((void *)(int_convter_p(start_addr)), ddr_test_size);
	}
	{
		printf("\nStart *4  cross talk pattern p.                                 ");
		printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + ddr_test_size);
		ddr_write_pattern4_cross_talk_p((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_pattern4_cross_talk_p((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_pattern4_cross_talk_p((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 2nd read.                              ");

		printf("\nStart *4  cross talk pattern n.                                 ");
		printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + ddr_test_size);
		ddr_write_pattern4_cross_talk_n((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_pattern4_cross_talk_n((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_pattern4_cross_talk_n((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 2nd read.                              ");
	}

	{
		printf("\nStart *4  cross talk pattern p2.                                 ");
		printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + ddr_test_size);
		ddr_write_pattern4_cross_talk_p2((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_pattern4_cross_talk_p2((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_pattern4_cross_talk_p2((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 2nd read.                              ");

		printf("\nStart *4  cross talk pattern n.                                 ");
		printf("\nStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + ddr_test_size);
		ddr_write_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd write.                                 ");
		printf("\rStart 1st reading...                       ");
		ddr_read_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 1st read.                              ");
		printf("\rStart 2nd reading...                       ");
		ddr_read_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr)), ddr_test_size);
		printf("\rEnd 2nd read.                              ");
		if (copy_test_flag) {
			printf("\n start copy test  ...                            ");
			ddr_test_copy((void *)(int_convter_p(start_addr + ddr_test_size / 2)), (void *)(int_convter_p(start_addr)), ddr_test_size / 2);
			ddr_read_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr + ddr_test_size / 2)), ddr_test_size / 2);
			ddr_read_pattern4_cross_talk_n2((void *)(int_convter_p(start_addr + ddr_test_size / 2)), ddr_test_size / 2);
		}
	}

	if (error_count)
		return 1;
	else
		return 0;
}

int do_ddr2pll_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
#if (CONFIG_DDR_PHY >= P_DDR_PHY_G12)
	extern int do_ddr2pll_g12_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[]);
	do_ddr2pll_g12_cmd(cmdtp, flag, argc, argv);
	return 1;
#endif
}

int do_ddr_uboot_new_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	//ddr_test_cmd 0x36 0x20180030 0x1 cmd_offset cmd_value value_size reset_enable
#define DDR_STICKY_OVERRIDE_CONFIG_MESSAGE_CMD  0x1             //override config
#define DDR_STICKY_SPECIAL_FUNCTION_CMD                 0x2     //special test such as shift some bdlr or parameter or interleave test

	uint32_t magic_chipid = 0;                              //rd_reg(P_PREG_STICKY_REG0);
	uint32_t sticky_cmd = 0;                                //rd_reg(P_PREG_STICKY_REG1);
	uint32_t cmd_offset = 0;
	uint32_t cmd_value = 0;
	uint32_t reset_enable = 0;
	uint32_t value_size = 4;
	char *endp;
	uint32_t read_value = 0;
	//bit 0 trigger effect reset.
	if ((magic_chipid) != ((DDR_STICKY_MAGIC_NUMBER + DDR_CHIP_ID) & 0xffff0000))
		//magic number not match
		printf("sticky0 magic not match\n");

	printf("\nargc== 0x%08x\n", argc);
	int i;
	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);

	if (argc < 2)
		goto usage;

	magic_chipid = simple_strtoull_ddr(argv[1], &endp, 0);
	if (*argv[1] == 0 || *endp != 0) {
		printf("Error: Wrong format parament!\n");
		return 1;
	}
	if (argc > 2) {
		sticky_cmd = simple_strtoull_ddr(argv[2], &endp, 0);
		if (*argv[2] == 0 || *endp != 0)
			sticky_cmd = 0;
	}

	if (argc > 3) {
		cmd_offset = simple_strtoull_ddr(argv[3], &endp, 0);
		if (*argv[3] == 0 || *endp != 0)
			cmd_offset = 0;
	}
	if (argc > 4) {
		cmd_value = simple_strtoull_ddr(argv[4], &endp, 0);
		if (*argv[4] == 0 || *endp != 0)
			cmd_value = 0;
	}
	if (argc > 5) {
		value_size = simple_strtoull_ddr(argv[5], &endp, 0);
		if (*argv[5] == 0 || *endp != 0)
			value_size = 4;
	}
	if (argc > 6) {
		reset_enable = simple_strtoull_ddr(argv[6], &endp, 0);
		if (*argv[6] == 0 || *endp != 0)
			reset_enable = 0;
	}
	printf("cmd_offset[0x%08x}==cmd_value [0x%08x]\n", cmd_offset, cmd_value);
	writel((magic_chipid & 0xffff0000) | (rd_reg((p_ddr_base->preg_sticky_reg0))), (p_ddr_base->preg_sticky_reg0));
	writel(sticky_cmd, (p_ddr_base->preg_sticky_reg0 + 4));


	if (value_size) {
		read_value = rd_reg((p_ddr_base->ddr_dmc_sticky0) + ((cmd_offset / 4) << 2));
		if (value_size == 1)
			wr_reg(((p_ddr_base->ddr_dmc_sticky0) + ((cmd_offset / 4) << 2)), ((cmd_value << ((cmd_offset % 4) * 8)) | (read_value & (~(0xff << ((cmd_offset % 4) * 8))))));
		if (value_size == 2)
			wr_reg(((p_ddr_base->ddr_dmc_sticky0) + ((cmd_offset / 4) << 2)), ((cmd_value << ((cmd_offset % 4) * 8)) | (read_value & (~(0xffff << ((cmd_offset % 4) * 8))))));
		if (value_size == 4)
			wr_reg(((p_ddr_base->ddr_dmc_sticky0) + ((cmd_offset / 4) << 2)), cmd_value);

		printf("DMC_STICKY_0_ offset[0x%08x}== [0x%08x]\n", cmd_offset, readl(((p_ddr_base->ddr_dmc_sticky0) + ((cmd_offset / 4) << 2))));
	}

	printf("(p_ddr_base->preg_sticky_reg0)== [0x%08x]\n", readl((p_ddr_base->preg_sticky_reg0)));

	if (reset_enable) {
		ddr_test_watchdog_reset_system();

		while (1) {
		}
	}
	return 0;

usage:
	cmd_usage(cmdtp);

	return 1;
}

unsigned int do_test_address_bus(volatile unsigned int *baseAddress, unsigned int nBytes)
{
	unsigned int addressMask = (nBytes / sizeof(unsigned int) - 1);
	unsigned int offset;
	unsigned int testOffset;

	unsigned int pattern = (unsigned int)0xAAAAAAAA;
	unsigned int antipattern = (unsigned int)0x55555555;

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
			printf("  memTestAddressBus - read twice different[offset]: 0x%8x-0x%8x\n", data1, data2);
			ret = 1;
		}
		if (data1 != pattern) {
			printf("  memTestAddressBus - write[0x%8x]: 0x%8x, read[0x%8x]: 0x%8x\n", \
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
			printf("  memTestAddressBus2 - write baseAddress[0x%8x]: 0x%8x, read baseAddress[0]: 0x%8x\n", \
			       testOffset, antipattern, baseAddress[0]);
			ret = 1;
		}

		for (offset = 1; (offset & addressMask) != 0; offset <<= 1) {
			data1 = baseAddress[offset];
			if ((data1 != pattern) && (offset != testOffset)) {
				printf("  memTestAddressBus3 - write baseAddress[0x%8x]: 0x%8x, read baseAddress[0x%8x]: 0x%8x\n", \
				       testOffset, antipattern, testOffset, data1);
				ret = 1;
			}
		}

		baseAddress[testOffset] = pattern;
	}

	for (offset = 0x1; (offset <= addressMask); offset++) {
		if (((~offset) <= addressMask)) {
			baseAddress[offset] = pattern;
			baseAddress[(~offset)] = antipattern;
		}
	}

	for (offset = 0x1; (offset <= addressMask); offset++) {
		if (((~offset) <= addressMask)) {
			if (baseAddress[offset] != pattern) {
				printf("  memTestAddressBus4 - write baseAddress[0x%8x]: 0x%8x, read baseAddress[0x%8x]: 0x%8x\n", \
				       offset, pattern, offset, baseAddress[offset]);

				ret = 1;
				break;
			}

			if (baseAddress[(~offset)] != antipattern) {
				printf("  memTestAddressBus5 - write baseAddress[0x%8x]: 0x%8x, read baseAddress[0x%8x]: 0x%8x\n", \
				       ((~offset)), antipattern, ((~offset)), baseAddress[((~offset))]);
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
				printf("  memTestAddressBus6 - write baseAddress[0x%8x]: 0x%8x, read baseAddress[0x%8x]: 0x%8x\n", \
				       offset, pattern, offset, baseAddress[offset]);
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
				printf("  memTestAddressBus7 - write baseAddress[0x%8x]: 0x%8x, read baseAddress[0x%8x]: 0x%8x\n", \
				       offset, pattern, offset, baseAddress[offset]);
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
	printf("\rStart writing at 0x%08x - 0x%08x...", start_addr, start_addr + ddr_test_size);
	ddr_write((void *)(int_convter_p(start_addr)), ddr_test_size);
	printf("\rEnd write.                                 ");
	printf("\nStart 1st reading...                       ");
	ddr_read((void *)(int_convter_p(start_addr)), ddr_test_size);
	printf("\rEnd 1st read.                              ");
	printf("\rStart 2nd reading...                       ");
	ddr_read((void *)(int_convter_p(start_addr)), ddr_test_size);
	ddr_write_full((void *)(int_convter_p(start_addr)), ddr_test_size, 0x0, 0x3);
	printf("\rEnd write.                                 ");
	printf("\rStart 1st reading...                       ");
	ddr_read_full((void *)(int_convter_p(start_addr)), ddr_test_size, 0, 3);
	printf("\rEnd 1st read.                              ");
	printf("\rStart 2nd reading...                       ");
	ddr_read_full((void *)(int_convter_p(start_addr)), ddr_test_size, 0, 3);

	printf("\rStart writing add pattern                                 ");
	if (do_test_address_bus((void *)(int_convter_p(start_addr)), ddr_test_size))
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
		ddr_clk = 2 * ((((24 * ((ddr_pll >> 0) & 0x1ff)) / ((ddr_pll >> 10) & 0x1f)) >> ((((ddr_pll >> 19) & 0x1) == 1) ? (2) : (1)))) / od_div;

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
#if 0
	if (pll_bypass_en == 1) {
		ddr_pll_vco_ctrl_od1 = 0x3; //0
		if ((ddr_clk >= 800)) {
			ddr_pll_vco_ctrl_od = 0;
			ddr_pll_vco_m = (ddr_clk * 4) / 24;
		} else if ((ddr_clk < 4800 / 6)) {
			ddr_pll_vco_ctrl_od = 1;
			ddr_pll_vco_m = (ddr_clk * 2 * 3) / 24;
		} else if ((ddr_clk < 4800 / 8)) {
			ddr_pll_vco_ctrl_od = 2;
			ddr_pll_vco_m = (ddr_clk * 2 * 4) / 24;
		} else if ((ddr_clk < 4800 / 12)) {
			ddr_pll_vco_ctrl_od = 3;
			ddr_pll_vco_m = (ddr_clk * 2 * 6) / 24;
		} else if ((ddr_clk < 4800 / 16)) {
			ddr_pll_vco_ctrl_od = 4;
			ddr_pll_vco_m = (ddr_clk * 2 * 8) / 24;
		}
	}
#endif
	ddr_pll_vco_ctrl = ddr_pll_vco_m | (ddr_pll_vco_n << 10) | (ddr_pll_vco_ctrl_od << 16) | (ddr_pll_vco_ctrl_od1 << 19);
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
	if (((ddr_pll >> 16) & 0x1f))

		ddr_clk = 2 * ((((24 * ((ddr_pll >> 4) & 0x1ff)) / ((ddr_pll >> 16) & 0x1f)) >> ((((ddr_pll >> 0) & 0x3) == 3) ? (2) : (((ddr_pll >> 0) & 0x3)))) >> ((((ddr_pll >> 2) & 0x3) == 3) ? (2) : (((ddr_pll >> 2) & 0x3))));

#else
	if ((ddr_pll >> 9) & 0x1f)
		ddr_clk = 2 * (((24 * (ddr_pll & 0x1ff)) / ((ddr_pll >> 9) & 0x1f)) >> ((ddr_pll >> 16) & 0x3));

#endif
	return ddr_clk;
#endif
#if (CONFIG_DDR_PHY == P_DDR_PHY_DEFAULT)
	if ((ddr_pll >> 9) & 0x1f)
		ddr_clk = 2 * ((24 * (ddr_pll & 0x1ff)) / ((ddr_pll >> 9) & 0x1f)) >> ((ddr_pll >> 16) & 0x3);
	return ddr_clk;
#endif

	//return ddr_clk;
}

int ddr_clk_convert_to_pll(unsigned int ddr_clk)
{
	unsigned int ddr_pll = 0x10221;

#if (CONFIG_DDR_PHY >= P_DDR_PHY_G12)
	ddr_pll = ddr_clk_convert_to_pll_g12a(ddr_clk, 0);
	return ddr_pll;
#else

	/* set ddr pll reg */
	if ((ddr_clk >= 40) && (ddr_clk < 750))
		ddr_pll = (2 << 16) | (1 << 9) | ((((ddr_clk / 6) * 6) / 12) << 0);
	else if ((ddr_clk >= 750) && (ddr_clk < 2000))
		ddr_pll = (1 << 16) | (1 << 9) | ((((ddr_clk / 12) * 12) / 24) << 0);
	return ddr_pll;
#endif
#if (CONFIG_DDR_PHY == P_DDR_PHY_905X)
	ddr_pll = 0x00104c5;
	/* set ddr pll reg */
	/*
	 * if ((ddr_clk >= 40) && (ddr_clk < 750)) {
	 * //							OD			N					M
	 * ddr_pll= (2 << 2) | (1 << 16) | ((((ddr_clk/6)*6)/12) << 4);
	 * }
	 * else if((ddr_clk >= 750) && (ddr_clk < 2000)) {
	 * //							OD			N					M
	 * ddr_pll= (1 << 2) | (1 << 16) | ((((ddr_clk/12)*12)/24) << 4);
	 * }
	 */
	if ((ddr_clk < 200))
		//							OD1			OD			N					M
		ddr_pll = (2 << 0) | (3 << 2) | (1 << 16) | ((((ddr_clk * 6) / 6) / 3) << 4);
	else if ((ddr_clk >= 200) && (ddr_clk < 400))
		//							OD1			OD			N					M
		ddr_pll = (2 << 0) | (1 << 2) | (1 << 16) | ((((ddr_clk * 6) / 6) / 6) << 4);
	else if ((ddr_clk >= 400) && (ddr_clk < 800))
		//							OD1			OD			N					M
		ddr_pll = (1 << 0) | (1 << 2) | (1 << 16) | ((((ddr_clk * 12) / 12) / 12) << 4);
	else if ((ddr_clk >= 800) && (ddr_clk < 2000))
		//							OD1			OD			N					M
		ddr_pll = (0 << 0) | (1 << 2) | (1 << 16) | ((((ddr_clk * 12) / 12) / 24) << 4);
	return ddr_pll;

#endif

#if (CONFIG_DDR_PHY == P_DDR_PHY_DEFAULT)
	{
		if ((ddr_clk < 750))
			//		OD			N		M
			ddr_pll = (2 << 16) | (1 << 9) | (((ddr_clk / 24) * 2) << 0);
		else if ((ddr_clk >= 750))
			//		OD			N		M
			ddr_pll = (1 << 16) | (1 << 9) | ((ddr_clk / 24) << 0);
	}
	return ddr_pll;
#endif

	//return ddr_pll;
}

int get_ddr_clk(void)
{
	unsigned int ddr_clk = 10;
	unsigned int ddr_pll = 0;

	if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_G12A)
	    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_G12B)
	    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_TL1)
	    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_SM1)
	    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_TM2)
	    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_C1)
	    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_SC2)
	    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T7)) {
		ddr_pll = rd_reg(p_ddr_base->ddr_pll_base_address);
		ddr_pll = ddr_pll & 0xfffff;
		ddr_clk = pll_convert_to_ddr_clk_g12a(ddr_pll);
	} else if (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_A1) {
		ddr_clk = 768;
	} else if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_C2) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5D) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S4) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5W)
		   || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)) {
		uint32_t stick_store_sticky_f0_reg_base_t =
		(p_ddr_base->ddr_phy_base_address + 0x0128);
		ddr_clk = rd_reg(stick_store_sticky_f0_reg_base_t);
	} else {
		ddr_clk = 10;
	}
	return ddr_clk;
}

void ddr_memcpy(void *dst, const void *src, uint32_t len)
{
	len = (len >> 3);
	const long long *s = src;
	long long *d = dst;
#ifdef TEST_L1_CACHE
	void *bound = (void *)src + 16 * 1024; //debug for test L1 cache ,if only read write small aread
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
		while (len) { //for best test efficiency  not inclued much more code in the while loop
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

#define PATTERN_MATRIX_X   (3 + 32 + 16 + 17)     //68*32==2176 ///2.2k -0x880-1 loop
#define PATTERN_MATRIX_Y  (32)
#define PATTERN_MATRIX_LOOP_SIZE   ((PATTERN_MATRIX_X)*(PATTERN_MATRIX_Y) * 4)

unsigned int cpu_ddr_test_init_pattern_generater(unsigned int add_offset)
{
	unsigned int pattern_select = 0;
	unsigned int pattern_value = 0;
	uint32_t martix_x_select = 0;
	uint32_t martix_y_select = 0;
	unsigned int pattern_value_temp_16 = 0;

	{
		{
			pattern_select = ((add_offset) % ((PATTERN_MATRIX_Y)*(PATTERN_MATRIX_X)));
			martix_x_select = pattern_select / (PATTERN_MATRIX_Y);
			martix_y_select = pattern_select % (PATTERN_MATRIX_Y);
			{
				if (martix_x_select == 0)
					pattern_value = 0xaaaa5555;  //for 16 bit bus pattern

				if (martix_x_select == 1)
					pattern_value = 0x0000ffff;  //for 16 bit bus pattern

				if (martix_x_select == 2)
					pattern_value = 0;

				if ((martix_x_select > 2) && (martix_x_select < (3 + 32)))
					pattern_value = 1 << (martix_x_select - 3);
				if ((martix_x_select > (2 + 32)) && (martix_x_select < (3 + 32 + 16))) { //for 16 bit bus pattern
					pattern_value_temp_16 = (1 << (martix_x_select - 3 - 32));
					pattern_value = pattern_value_temp_16 | ((~pattern_value_temp_16) << 16);
				}
				if ((martix_x_select > (2 + 32 + 16)) && (martix_x_select < (3 + 32 + 16 + 17))) { //for dbi bus pattern  17 group
					pattern_value_temp_16 = (0x0f0f + 0xf0f * (martix_x_select - 3 - 32 - 16));
					pattern_value = pattern_value_temp_16 | ((~pattern_value_temp_16) << 16);
				}
			}
			if (martix_y_select % 2)
				pattern_value = ~pattern_value;
		}
	}
	return pattern_value;
}

void cpu_ddr_test_init_pattern_area(unsigned int test_init_start, unsigned int test_size, unsigned int parttern_frequency_setting)
{
	if (parttern_frequency_setting == 0)
		parttern_frequency_setting = 1;  //for different  frequency pattern
	test_size = (test_size > ((PATTERN_MATRIX_LOOP_SIZE)*(parttern_frequency_setting))) ? test_size : ((PATTERN_MATRIX_LOOP_SIZE)*(parttern_frequency_setting));
	unsigned int write_add = test_init_start;
	unsigned int size_loop = 0;
	unsigned int size_loop_max = 0;
	for (; (size_loop < ((PATTERN_MATRIX_LOOP_SIZE)*(parttern_frequency_setting))); ) {
		{
			write_add = (uint32_t)(size_loop + test_init_start);
			wr_reg((unsigned long)write_add, cpu_ddr_test_init_pattern_generater((size_loop >> 2) / parttern_frequency_setting));
			size_loop = size_loop + 4;
		}
	}
	size_loop = 1;
	size_loop_max = ((test_size / (((PATTERN_MATRIX_LOOP_SIZE)*(parttern_frequency_setting)))) + 1);
	for (; (size_loop < size_loop_max); ) {
		ddr_memcpy((void *)(uint64_t)(test_init_start + ((PATTERN_MATRIX_LOOP_SIZE)*((uint64_t)parttern_frequency_setting)) * (size_loop)), (void *)(uint64_t)test_init_start, (uint32_t)((PATTERN_MATRIX_LOOP_SIZE)*(parttern_frequency_setting)));
		size_loop++;
	}
}

unsigned int cpu_ddr_test(unsigned test_init_start, unsigned int start_add, unsigned int test_size, unsigned int test_data_bit_enable, unsigned int parttern_frequency_setting)
{
	unsigned int src_add = test_init_start;
	unsigned int pattern_value = 0;
	unsigned int size_loop = 0;
	unsigned int ddr_test_error = 0;
	unsigned int read_add = 0;
	unsigned int read_value = 0;

	test_size = (test_size > 0x2000) ? (test_size) : (0x2000);
	uint32_t ddr_test_start_time_us = get_us_time();        // check cost time
	ddr_memcpy((void *)(uint64_t)start_add, (void *)(uint64_t)src_add, test_size);
	uint32_t ddr_test_end_time_us = get_us_time();          // check cost time
	printf("\ncpu_ddr_test_test_copy_bandwidth==%d Mbyte/s\n", (1 * test_size * 2) / (ddr_test_end_time_us - ddr_test_start_time_us));

	for (; size_loop < (test_size); ) {
		read_add = (uint32_t)(size_loop + start_add);
		read_value = (rd_reg((unsigned long)read_add));
		pattern_value = (cpu_ddr_test_init_pattern_generater((size_loop >> 2) / parttern_frequency_setting));
		if (((test_data_bit_enable) & read_value) != ((test_data_bit_enable) & pattern_value)) {
			printf("error data  enable %08x read_value %08x  pattern_value %08x", test_data_bit_enable, read_value, pattern_value);
			ddr_test_error++;
			return ddr_test_error;
		}
		size_loop = size_loop + (1 << 2); // use big step will fast test ,but lose accuracy.
	}
	return ddr_test_error;
}

int do_cpu_ddr_test(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	check_base_address();
	int i = 0;
	printf("\nargc== 0x%08x\n", argc);
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
	uint32_t ddr_test_start_time_us = get_us_time();  // check cost time
	cpu_ddr_test_init_pattern_area(init_start_add, test_size, parttern_frequency_setting);
	for (test_loop = 0; test_loop < test_loops; ) {
		test_errors = test_errors + cpu_ddr_test(init_start_add, test_add, test_size, test_data_bit_enable, parttern_frequency_setting);
		test_loop++;
		printf("\ncpu_ddr_test_test_times==%d  test_errors==%d", test_loop, test_errors);
	}
	uint32_t ddr_test_end_time_us = get_us_time();  // check cost time
	printf("\ncpu_ddr_test_test_and compare_bandwidth==%d Mbyte/s\n", (test_loops * test_size * 2) / (ddr_test_end_time_us - ddr_test_start_time_us));
	return test_errors;
}

U_BOOT_CMD(
	ddr_cpu_test, 30, 1, do_cpu_ddr_test,
	"ddr_test_cmd cmd arg1 arg2 arg3...",
	"ddr_test_cmd cmd arg1 arg2 arg3... \n dcache off ? \n"
	);

int do_ddr_test_write_read(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	{
		printf("\nEnter do_ddr_test_ddr_write_read_current\n");
		int i = 0;
		printf("\nargc== 0x%08x\n", argc);
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
		printf("\nwrite_read== 0x%08d\n", write_read);
		printf("\npattern_id== 0x%08d\n", pattern_id);
		printf("\nloop== 0x%08d\n", loop);
		printf("\nstart_addr== 0x%08x\n", start_addr);
		printf("\ntest_size== 0x%08x\n", test_size);
		printf("\nus_delay_counter== %d\n", us_delay_counter);
		copy_offset = test_size;

		unsigned int *p;
		unsigned int j;

		p = (unsigned int *)(int_convter_p(start_addr));

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
					printf("\nloop:0x%08x:Start writing at 0x%08x - 0x%08x...", loop, start_addr, start_addr + test_size);
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
					printf("\nloop:0x%08x:Start reading at 0x%08x - 0x%08x...", loop, start_addr, start_addr + test_size);
				for (j = 0; j < test_size / 4; ) {
					read_pattern[0] = *(p + j);
					read_pattern[1] = *(p + j + 1);
					read_pattern[2] = *(p + j + 2);
					read_pattern[3] = *(p + j + 3);
					j = j + 4;
				}

				if (loop == 1) {
					if (!no_show_info) {
						printf(" \nloop:0x%08x:Start reading read_pattern[0] 0x%08x, pattern[1] 0x%08x,pattern[2] 0x%08x,pattern[3] 0x%08x",
						       loop, read_pattern[0], read_pattern[1], read_pattern[2], read_pattern[3]
						       );
					}
				}
			}
			if (write_read == 2) {
				if (!no_show_info)
					printf("\nloop:0x%08x:Start copying at 0x%08x - 0x%08x...", loop, start_addr, start_addr + test_size);
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
static int pwm_voltage_table_ee[][2] =
{
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
		if ((pwm_voltage_table_ee[to - 1][1] < ee_over_ride_voltage) && (pwm_voltage_table_ee[to][1] >= ee_over_ride_voltage))
			break;
	if (to == (ARRAY_SIZE(pwm_voltage_table_ee)))
		to = (ARRAY_SIZE(pwm_voltage_table_ee) - 1);
	if (ee_over_ride_voltage) {
		writel(pwm_voltage_table_ee[to][0], (p_ddr_base->ee_pwm_base_address));
		printf("\nDDR_overide_EE_voltage ==%d mv /n", pwm_voltage_table_ee[to][1]);
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

	if (((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_C2) &&
	(p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_T5D)) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S4) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5W)) {
		bdlr_100step = do_read_c2_ddr_bdlr_steps();
	} else {
		dwc_ddrphy_apb_wr(((((0 << 20) | (2 << 16) | (0 << 12) | (0xe3)))), 0xc00);
		bdlr_100step = (100000000 / (2 * ddr_frequency)) / ((dwc_ddrphy_apb_rd((((0 << 20) | (2 << 16) | (0 << 12) | (0xe4))))) & 0x3ff);
	}
	return bdlr_100step;
}

int do_ddr_test_pwm_bdlr(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char *endp;

	printf("\nEnter g12 do_ddr_test_pwm_bdl function\n");
	int i = 0;
	printf("\nargc== 0x%08x\n", argc);
	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);

	unsigned int argc_count = 1;
	unsigned int para_meter[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
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

#define PWM_LOOP_DEFAULT   10 << 0
#define PWM_VOLTAGE_MIN_DEFAULT   TEST_MIN_DDR_EE_VOLTAGE
#define PWM_VOLTAGE_MAX_DEFAULT   TEST_MAX_DDR_EE_VOLTAGE

	loop = loop ? loop : PWM_LOOP_DEFAULT;
	voltage_min = (voltage_min < PWM_VOLTAGE_MIN_DEFAULT) ? PWM_VOLTAGE_MIN_DEFAULT : voltage_min;
	voltage_max = (voltage_max > PWM_VOLTAGE_MAX_DEFAULT) ? PWM_VOLTAGE_MAX_DEFAULT : voltage_max;
	voltage_max = (voltage_max < PWM_VOLTAGE_MIN_DEFAULT) ? PWM_VOLTAGE_MAX_DEFAULT : voltage_max;

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
	printf("\nread org_EE_voltage %d mv \n", read_ee_voltage());
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
			bdlr_100_cur = (100000000 / (2 * global_ddr_clk)) / ((dwc_ddrphy_apb_rd((((0 << 20) | (2 << 16) | (0 << 12) | (0xe4))))) & 0x3ff);
			bdlr_100_min = (bdlr_100_cur < bdlr_100_min) ? bdlr_100_cur : bdlr_100_min;
			bdlr_100_max = (bdlr_100_cur > bdlr_100_max) ? bdlr_100_cur : bdlr_100_max;
			bdlr_100_average = (bdlr_100_cur + bdlr_100_average * count) / (count + 1);
			count++;
			if (show_count_message)
				printf("%d\n", bdlr_100_cur);
		} while (count < loop);
		printf("\nDDR_set EE_voltage %d  bdlr_100_average %d  bdlr_100_min %d bdlr_100_max %d count %d", pwm_voltage_table_ee[to - 1][1],
		       bdlr_100_average, bdlr_100_min, bdlr_100_max, count);
	}
	return 1;
}


int printf_log(char log_level, const char *fmt, ...)
{
	if (log_level < 1) {
		va_list args;
		va_start(args, fmt);
		vprintf(fmt, args);  //
		va_end(args);
		return 0;
	} else {
		return 1;
	}
}

int do_read_ddr_training_data(char log_level, ddr_set_t *ddr_set_t_p)
{
	printf_log(log_level, "\nddr_set_t_p==0x%08x\n", (uint32_t)(uint64_t)(ddr_set_t_p));
	uint32_t loop = 0;
	uint32_t loop_max = (4 + (0x3f << 2)); //((DMC_STICKY_63-DMC_STICKY_0));
	for (loop = 0; loop < loop_max; loop += 4)
		wr_reg(((uint64_t)(ddr_set_t_p) + loop), rd_reg((p_ddr_base->ddr_dmc_sticky0) + loop));

	for (loop = 0; loop < MESON_CPU_CHIP_ID_SIZE; loop++)   //update chip id
		ddr_sha.sha_chip_id[loop] = global_chip_id[loop];
	{
		uint16_t dq_bit_delay[72];
		unsigned char t_count = 0;
		uint16_t delay_org = 0;
		uint16_t delay_temp = 0;
		uint32_t add_offset = 0;
		dwc_ddrphy_apb_wr(0xd0000, 0x0);
		bdlr_100step = get_bdlr_100step(global_ddr_clk);
		ui_1_32_100step = (1000000 * 100 / (global_ddr_clk * 2 * 32));
		uint32_t ps = 0;
		for (ps = 0; ps < 2; ps++) {
			{
				ddr_set_t_p->cfg_ddr_training_delay_ps[ps].ARdPtrInitVal = 0;
				printf_log(log_level, "\n ARdPtrInitVal ps=%d", ps);
				add_offset = ((ps << 20) | (2 << 16) | (0 << 12) | (0x2e));
				delay_org = dwc_ddrphy_apb_rd(add_offset);
				ddr_set_t_p->cfg_ddr_training_delay_ps[ps].ARdPtrInitVal = delay_org;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", 0, delay_org, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), delay_org);

				printf_log(log_level, "\n dfimrl0 dfimrl1 dfimrl2 dfimrl3 HwtMRL ps=%d", ps);
				add_offset = ((ps << 20) | (1 << 16) | (0 << 12) | (0x20));
				delay_org = dwc_ddrphy_apb_rd(add_offset);
				ddr_set_t_p->cfg_ddr_training_delay_ps[ps].dfi_mrl = delay_org;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", 0, delay_org, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), delay_org);
				add_offset = ((ps << 20) | (1 << 16) | (1 << 12) | (0x20));
				delay_org = dwc_ddrphy_apb_rd(add_offset);
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", 1, delay_org, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), delay_org);
				add_offset = ((ps << 20) | (1 << 16) | (2 << 12) | (0x20));
				delay_org = dwc_ddrphy_apb_rd(add_offset);
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", 2, delay_org, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), delay_org);
				add_offset = ((ps << 20) | (1 << 16) | (3 << 12) | (0x20));
				delay_org = dwc_ddrphy_apb_rd(add_offset);
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", 3, delay_org, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), delay_org);
				add_offset = ((ps << 20) | (2 << 16) | (0 << 12) | (0x20));
				delay_org = dwc_ddrphy_apb_rd(add_offset);

				ddr_set_t_p->cfg_ddr_training_delay_ps[ps].dfi_hwtmrl = delay_org;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", 0, delay_org, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), delay_org);
			}
			{
				printf_log(log_level, "\n count_index     delay_value     register_add     register_value  ps=%d\n ", ps);
				printf_log(log_level, "\n address delay * 1/32UIx100==%d ps bit0-4 fine tune  --step==1/32UI ,bit 6 is coarse  --step==1UI  ps=%d", ui_1_32_100step, ps);
				for (t_count = 0; t_count < 10; t_count++) {
					add_offset = ((ps << 20) | (0 << 16) | (t_count << 12) | (0x80));
					dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
					delay_org = dq_bit_delay[t_count];
					delay_temp = (32 * (((delay_org >> 6) & 0xf) + ((delay_org >> 5) & 1)) + (delay_org & 0x1f));
					ddr_set_t_p->cfg_ddr_training_delay_ps[ps].ac_trace_delay[t_count] = delay_temp;
					printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
				}
			}
			{
				printf_log(log_level, "\n tdqs delay * 1/32UIx100==%d ps bit0-4 fine tune --step==1/32UI ,bit 6-9 is coarse  --step==1UI ps=%d", ui_1_32_100step, ps);
				for (t_count = 0; t_count < 16; t_count++) {
					add_offset = ((ps << 20) | (1 << 16) | (((t_count % 8) >> 1) << 12) | (0xd0 + (t_count / 8) + ((t_count % 2) << 8)));
					dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
					delay_org = dq_bit_delay[t_count];
					delay_temp = (32 * (((delay_org >> 6) & 0xf) + ((delay_org >> 5) & 1)) + (delay_org & 0x1f));

					ddr_set_t_p->cfg_ddr_training_delay_ps[ps].write_dqs_delay[t_count] = delay_temp;

					printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
				}
			}
			{
				printf_log(log_level, "\n rxdqs delay * 1/32UIx100==%d ps bit0-4 fine tune --step==1/32UI,no coarse ps=%d", ui_1_32_100step, ps);
				for (t_count = 0; t_count < 16; t_count++) {
					add_offset = ((ps << 20) | (1 << 16) | (((t_count % 8) >> 1) << 12) | (0x8c + (t_count / 8) + ((t_count % 2) << 8)));
					dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
					delay_org = dq_bit_delay[t_count];
					delay_temp = (32 * (((delay_org >> 6) & 0xf) + ((delay_org >> 5) & 1)) + (delay_org & 0x1f));
					ddr_set_t_p->cfg_ddr_training_delay_ps[ps].read_dqs_delay[t_count] = delay_temp;
					printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
				}
			}
			{
				printf_log(log_level, "\n write dq_bit delay * 1/32UIx100==%d ps bit0-4 fine tune --step==1/32UI ,bit 6-8 is coarse  --step==1U ps=%d", ui_1_32_100step, ps);
				for (t_count = 0; t_count < 72; t_count++) {
					add_offset = ((ps << 20) | (1 << 16) | (((t_count % 36) / 9) << 12) | (0xc0 + ((t_count % 9) << 8) + (t_count / 36)));
					dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
					delay_org = dq_bit_delay[t_count];
					delay_temp = (32 * (((delay_org >> 6) & 0xf) + ((delay_org >> 5) & 1)) + (delay_org & 0x1f));

					ddr_set_t_p->cfg_ddr_training_delay_ps[ps].write_dq_bit_delay[t_count] = delay_temp;
					printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
				}
			}
			{
				printf_log(log_level, "\n read dq_bit delay * BDLRx100==%d ps bit0-4 fine tune --step==bdlr step size about 5ps,no coarse ps=%d", bdlr_100step, ps);
				for (t_count = 0; t_count < 72; t_count++) {
					add_offset = ((0 << 20) | (1 << 16) | (((t_count % 36) / 9) << 12) | (0x68 + ((t_count % 9) << 8) + (t_count / 36)));
					dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
					delay_org = dq_bit_delay[t_count];
					delay_temp = ((delay_org & 0x3f));

					ddr_set_t_p->cfg_ddr_training_delay_ps[ps].read_dq_bit_delay[t_count] = delay_temp;
					printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
				}
			}
			{
				printf_log(log_level, "\n read dqs gate delay * 1/32UIx100==%d ps bit0-4 fine tune ,bit 6-10 is coarse ps=%d", ui_1_32_100step, ps);
				for (t_count = 0; t_count < 16; t_count++) {
					add_offset = ((ps << 20) | (1 << 16) | (((t_count % 8) >> 1) << 12) | (0x80 + (t_count / 8) + ((t_count % 2) << 8)));
					dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
					delay_org = dq_bit_delay[t_count];
					delay_temp = (32 * (((delay_org >> 6) & 0x1f) + ((delay_org >> 5) & 1)) + (delay_org & 0x1f));

					ddr_set_t_p->cfg_ddr_training_delay_ps[ps].read_dqs_gate_delay[t_count] = delay_temp;
					printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
				}

				printf_log(log_level, "\n soc vref : lpddr4-- VREF = VDDQ*(0.047 + VrefDAC0[6:0]*0.00367   DDR4 --VREF = VDDQ*(0.510 + VrefDAC0[6:0]*0.00345 ps=%d", ps);
				uint32_t vref_t_count = 0;
				for (t_count = 0; t_count < 72; t_count++) { //add normal vref0---vrefDac0 for just 1->x transitions
					add_offset = ((0 << 20) | (1 << 16) | (((t_count % 36) / 9) << 12) | (((t_count % 36) % 9) << 8) | (0x40));
					dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
					delay_org = dq_bit_delay[t_count];
					delay_temp = ((delay_org));
					if (t_count < 35) {
						vref_t_count = ((((t_count % 36) / 9) * 8) + (t_count % 9));
						ddr_set_t_p->cfg_ddr_training_delay_ps[ps].soc_bit_vref[vref_t_count] = delay_temp;
					}
					if ((t_count % 9) == 8) {
						vref_t_count = 32 + ((((t_count % 36) / 9)));
						ddr_set_t_p->cfg_ddr_training_delay_ps[ps].soc_bit_vref[vref_t_count] = delay_temp;
					}

					printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
				}
				printf_log(log_level, "\n soc vref-dfe dac1 0--->x : lpddr4-- VREF = VDDQ*(0.047 + VrefDAC0[6:0]*0.00367   DDR4 --VREF = VDDQ*(0.510 + VrefDAC0[6:0]*0.00345 ps=%d", ps);
				for (t_count = 0; t_count < 72; t_count++) { //add dfe vref1---vrefDac1 for just 0->x transitions
					add_offset = ((0 << 20) | (1 << 16) | (((t_count % 36) / 9) << 12) | (((t_count % 36) % 9) << 8) | (0x30));
					dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
					delay_org = dq_bit_delay[t_count];
					delay_temp = ((delay_org));
					if (t_count < 35) {
						vref_t_count = ((((t_count % 36) / 9) * 8) + (t_count % 9));
						ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_soc_vref_dac1_dfe[vref_t_count] = delay_temp;
					}
					if ((t_count % 9) == 8) {
						vref_t_count = 32 + ((((t_count % 36) / 9)));
						ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_soc_vref_dac1_dfe[vref_t_count] = delay_temp;
					}
					printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
				}
				printf_log(log_level, "\n dram vref : lpddr4-- VREF = VDDQ*(0. + VrefDAC0[6:0]*0.   DDR4 --VREF = VDDQ*(0. + VrefDAC0[6:0]*0.  ps=%d", ps);
				add_offset = ((0 << 20) | (1 << 16) | (0 << 12) | (0x082));
				delay_temp = dwc_ddrphy_apb_rd(add_offset);
				for (t_count = 0; t_count < 32; t_count++)
					ddr_set_t_p->cfg_ddr_training_delay_ps[ps].dram_bit_vref[t_count] = delay_temp;

				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", 0, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), delay_temp);
			}
			//add for skip training
			printf_log(log_level, "\n extra retraining setting. ps=%d", ps);
			t_count = 0;
			add_offset = (0 << 20) | (2 << 16) | (0 << 12) | (0xcb);
			dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
			delay_temp = dq_bit_delay[t_count];
			ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_pllctrl3 = delay_temp;
			printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);

			for (t_count = 0; t_count < 4; t_count++) {
				add_offset = ((0 << 20) | (1 << 16) | (t_count << 12) | (0xaa));
				dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
				delay_temp = dq_bit_delay[t_count];
				ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_pptctlstatic[t_count] = delay_temp;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
			}
			for (t_count = 0; t_count < 4; t_count++) {
				add_offset = ((0 << 20) | (1 << 16) | (t_count << 12) | (0x62));
				dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
				delay_temp = dq_bit_delay[t_count];
				ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_trainingincdecdtsmen[t_count] = delay_temp;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
			}
			for (t_count = 0; t_count < 4; t_count++) {
				add_offset = ((0 << 20) | (1 << 16) | (t_count << 12) | (0x01));
				dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
				delay_temp = dq_bit_delay[t_count];
				ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_tsmbyte0[t_count] = delay_temp;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
			}
			for (t_count = 0; t_count < 4; t_count++) {
				add_offset = ((ps << 20) | (1 << 16) | (t_count << 12) | (0x43));
				dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
				delay_temp = dq_bit_delay[t_count];
				ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_dqsrcvcntrl[t_count] = delay_temp;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
			}
			for (t_count = 0; t_count < 4; t_count++) {
				add_offset = ((ps << 20) | (1 << 16) | (t_count << 12) | (0xae));
				dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
				delay_temp = dq_bit_delay[t_count];
				ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_pptdqscntinvtrntg0[t_count] = delay_temp;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
			}
			for (t_count = 0; t_count < 4; t_count++) {
				add_offset = ((ps << 20) | (1 << 16) | (t_count << 12) | (0xaf));
				dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
				delay_temp = dq_bit_delay[t_count];
				ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_pptdqscntinvtrntg1[t_count] = delay_temp;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
			}
			{
				t_count = 0;
				add_offset = ((ps << 20) | (2 << 16) | (t_count << 12) | (0xb2));
				dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
				delay_temp = dq_bit_delay[t_count];
				ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_vrefinglobal = delay_temp;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
			}
			for (t_count = 1; t_count < 9; t_count++) {
				add_offset = ((ps << 20) | (9 << 16) | (0 << 12) | (0x200) | t_count);
				dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
				delay_temp = dq_bit_delay[t_count];
				ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_seq0bgpr[t_count] = delay_temp;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
			}
			{
				t_count = 0;
				add_offset = ((ps << 20) | (2 << 16) | (t_count << 12) | (0x7c));
				dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
				delay_temp = dq_bit_delay[t_count];
				ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_dllgainctl = delay_temp;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
			}
			{
				t_count = 0;
				add_offset = ((ps << 20) | (2 << 16) | (t_count << 12) | (0x7d));
				dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
				delay_temp = dq_bit_delay[t_count];
				ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_dlllockpara = delay_temp;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
			}
			{
				t_count = 0;
				add_offset = ((0 << 20) | (2 << 16) | (t_count << 12) | (0x77));
				dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
				delay_temp = dq_bit_delay[t_count];
				ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_hwtcamode = delay_temp;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
			}
			{
				t_count = 0;
				add_offset = ((0 << 20) | (2 << 16) | (t_count << 12) | (0x72));
				dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
				delay_temp = dq_bit_delay[t_count];
				ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_hwtlpcsena = delay_temp;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
			}
			{
				t_count = 0;
				add_offset = ((0 << 20) | (2 << 16) | (t_count << 12) | (0x73));
				dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
				delay_temp = dq_bit_delay[t_count];
				ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_hwtlpcsenb = delay_temp;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
			}
			{
				t_count = 0;
				add_offset = ((0 << 20) | (4 << 16) | (t_count << 12) | (0xfd));
				dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
				delay_temp = dq_bit_delay[t_count];
				ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_acsmctrl13 = delay_temp;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
			}
			{
				t_count = 0;
				add_offset = ((0 << 20) | (4 << 16) | (t_count << 12) | (0xc0));
				dq_bit_delay[t_count] = dwc_ddrphy_apb_rd(add_offset);
				delay_temp = dq_bit_delay[t_count];
				ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_acsmctrl23 = delay_temp;
				printf_log(log_level, "\n t_count: %04d %04d  %08x %08x", t_count, delay_temp, ((((add_offset) << 1) + (p_ddr_base->ddr_phy_base_address))), dq_bit_delay[t_count]);
			}
		}
	}
	return 1;
}

int do_ddr_display_g12_ddr_information(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
#ifdef ENABLE_G12_PHY_TEST_CMD
	int i = 0;
	unsigned int ps = 0;

	printf("\nargc== 0x%08x\n", argc);
	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);
	ddr_set_t *ddr_set_t_p = NULL;
	ddr_set_t_p = (ddr_set_t *)(ddr_set_t_p_arrary);
	ddr_set_t7 *ddr_set_t_p_t7 = (ddr_set_t7 *)ddr_set_t_p;
	do_read_ddr_training_data(0, ddr_set_t_p);

	{
		uint32_t count = 0;
		uint32_t reg_add_offset = 0;
		uint16_t reg_value = 0;

		printf("\n PCTL timming: 0x");

		for (count = 0; count < ((p_ddr_base->ddr_pctl_timing_end_address) - (p_ddr_base->ddr_pctl_timing_base_address)); ) {
			reg_add_offset = ((p_ddr_base->ddr_pctl_timing_base_address) + (count));
			printf("\n reg_add_offset: %08x %08x %08x ", reg_add_offset, readl(reg_add_offset), reg_add_offset);
			count = count + 4;
		}
		printf("\n mrs register: ");
		printf("\n mrs register: base (0x54000<<1)+DDR_PHY_BASE,%08x  byte offset\n", (0x54000 << 1) + (p_ddr_base->ddr_phy_base_address));
		for (count = 0; count < 0x80; ) {
			reg_add_offset = 0x54000 + count;                                                                                                                       //dwc_ddrphy_apb_wr(0x54008,0x1001);
			reg_value = ((*(volatile uint16_t *)((uint64_t)(((0x54000 + (count >> 1))) << 1) + (p_ddr_base->ddr_phy_base_address))) >> (((count) % 2) ? 8 : 0));    //dwc_ddrphy_apb_rd(0x54000+add_offset+1);
			reg_value = reg_value & 0xff;
			printf("\n reg_add_offset: %08x %08x %08x", reg_add_offset, reg_value, ((((0x54000 + (count >> 1))) << 1) + (p_ddr_base->ddr_phy_base_address)));
			count = count + 1;
		}

		printf("\n sticky register: ");
		{
			uint32_t loop_max = 0;
			loop_max = 64 << 2; //sizeof(ddr_set_t);
			for (count = 0; count < loop_max; count += 4)
				printf("\n reg_add_offset: %08x %08x %08x", count, rd_reg((uint64_t)((p_ddr_base->ddr_dmc_sticky0)) + count), (((p_ddr_base->ddr_dmc_sticky0)) + count));
		}

		{
			uint32_t loop_max = 0;
			loop_max = sizeof(ddr_set_t);
			uint32_t count = 0;
			for (count = 0; count < loop_max; ) {
				printf("\n%08x %08x", count, rd_reg((uint64_t)(ddr_set_t_p) + count));
				count = count + 4;
			}
		}
	}

	printf("\n {");

#define DDR_TIMMING_OFFSET_DDR_SET_T7(X) (unsigned int)(unsigned long)(&(((ddr_set_t7 *)(0))->X))
////#define DDR_TIMMING_OFFSET_SIZE(X) sizeof(((ddr_set_t7 *)(0))->X)
#define DDR_TIMMING_OFFSET_DDR_SET(X) (unsigned int)(unsigned long)(&(((ddr_set_t *)(0))->X))
//#define DDR_TIMMING_OFFSET_SIZE(X) sizeof(((ddr_set_t *)(0))->X)
	uint32_t temp_count = 0;
	{
		printf("\n.magic=0x%08x,// %d,0x%08x", ddr_set_t_p->magic, ddr_set_t_p->magic, DDR_TIMMING_OFFSET_DDR_SET(magic));
		printf("\n//old fast_boot[%d]=0x%08x,// %d,0x%08x", 0, ddr_set_t_p->fast_boot[0], ddr_set_t_p->fast_boot[0], DDR_TIMMING_OFFSET_DDR_SET(fast_boot[0]));
		ddr_set_t_p->fast_boot[0] = 0xfd; //add for auto copy to  code test
		for (temp_count = 0; temp_count < 4; temp_count++)
			printf("\n.fast_boot[%d]=0x%08x,// %d,0x%08x", temp_count, ddr_set_t_p->fast_boot[temp_count], ddr_set_t_p->fast_boot[temp_count], DDR_TIMMING_OFFSET_DDR_SET(fast_boot[temp_count]));
		printf("\n.board_id=0x%08x,// %d,0x%08x", ddr_set_t_p->board_id, ddr_set_t_p->board_id, DDR_TIMMING_OFFSET_DDR_SET(board_id));
		printf("\n.version=0x%08x,// %d,0x%08x", ddr_set_t_p->version, ddr_set_t_p->version, DDR_TIMMING_OFFSET_DDR_SET(version));
		printf("\n.DramType=0x%08x,// %d,0x%08x", ddr_set_t_p->DramType, ddr_set_t_p->DramType, DDR_TIMMING_OFFSET_DDR_SET(DramType));
		if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T7)) {
			printf("\n.DisabledDbyte[0]=0x%08x,// %d,0x%08x", ddr_set_t_p_t7->DisabledDbyte[0], ddr_set_t_p_t7->DisabledDbyte[0], DDR_TIMMING_OFFSET_DDR_SET_T7(DisabledDbyte[0]));
			printf("\n.DisabledDbyte[1]=0x%08x,// %d,0x%08x", ddr_set_t_p_t7->DisabledDbyte[1], ddr_set_t_p_t7->DisabledDbyte[1], DDR_TIMMING_OFFSET_DDR_SET_T7(DisabledDbyte[1]));
			printf("\n.Is2Ttiming=0x%08x,// %d,0x%08x", ddr_set_t_p_t7->Is2Ttiming, ddr_set_t_p_t7->Is2Ttiming, DDR_TIMMING_OFFSET_DDR_SET_T7(Is2Ttiming));
			printf("\n.HdtCtrl=0x%08x,// %d,0x%08x", ddr_set_t_p_t7->HdtCtrl, ddr_set_t_p_t7->HdtCtrl, DDR_TIMMING_OFFSET_DDR_SET_T7(HdtCtrl));
			printf("\n.dram_rank_config=0x%08x,// %d,0x%08x", ddr_set_t_p_t7->dram_rank_config, ddr_set_t_p_t7->dram_rank_config, DDR_TIMMING_OFFSET_DDR_SET_T7(dram_rank_config));
		} else {
			printf("\n.DisabledDbyte=0x%08x,// %d,0x%08x", ddr_set_t_p->DisabledDbyte, ddr_set_t_p->DisabledDbyte, DDR_TIMMING_OFFSET_DDR_SET(DisabledDbyte));
			printf("\n.Is2Ttiming=0x%08x,// %d,0x%08x", ddr_set_t_p->Is2Ttiming, ddr_set_t_p->Is2Ttiming, DDR_TIMMING_OFFSET_DDR_SET(Is2Ttiming));
			printf("\n.HdtCtrl=0x%08x,// %d,0x%08x", ddr_set_t_p->HdtCtrl, ddr_set_t_p->HdtCtrl, DDR_TIMMING_OFFSET_DDR_SET(HdtCtrl));
			printf("\n.dram_rank_config=0x%08x,// %d,0x%08x", ddr_set_t_p->dram_rank_config, ddr_set_t_p->dram_rank_config, DDR_TIMMING_OFFSET_DDR_SET(dram_rank_config));
			printf("\n.diagnose=0x%08x,// %d,0x%08x", ddr_set_t_p->diagnose, ddr_set_t_p->diagnose, DDR_TIMMING_OFFSET_DDR_SET(diagnose));
		}
		printf("\n.soc_data_drv_ohm_ps1=0x%08x,// %d,0x%08x", ddr_set_t_p->soc_data_drv_ohm_ps1, ddr_set_t_p->soc_data_drv_ohm_ps1, DDR_TIMMING_OFFSET_DDR_SET(soc_data_drv_ohm_ps1));
		printf("\n.dram_data_drv_ohm_ps1=0x%08x,// %d,0x%08x", ddr_set_t_p->dram_data_drv_ohm_ps1, ddr_set_t_p->dram_data_drv_ohm_ps1, DDR_TIMMING_OFFSET_DDR_SET(dram_data_drv_ohm_ps1));
		printf("\n.soc_data_odt_ohm_ps1=0x%08x,// %d,0x%08x", ddr_set_t_p->soc_data_odt_ohm_ps1, ddr_set_t_p->soc_data_odt_ohm_ps1, DDR_TIMMING_OFFSET_DDR_SET(soc_data_odt_ohm_ps1));
		printf("\n.dram_data_odt_ohm_ps1=0x%08x,// %d,0x%08x", ddr_set_t_p->dram_data_odt_ohm_ps1, ddr_set_t_p->dram_data_odt_ohm_ps1, DDR_TIMMING_OFFSET_DDR_SET(dram_data_odt_ohm_ps1));
		printf("\n.dram_data_wr_odt_ohm_ps1=0x%08x,// %d,0x%08x", ddr_set_t_p->dram_data_wr_odt_ohm_ps1, ddr_set_t_p->dram_data_wr_odt_ohm_ps1, DDR_TIMMING_OFFSET_DDR_SET(dram_data_wr_odt_ohm_ps1));
		if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T7)) {
			printf("\n.soc_data_drv_ohm_ffe=0x%08x,// %d,0x%08x", ddr_set_t_p_t7->soc_data_drv_ohm_ffe, ddr_set_t_p_t7->soc_data_drv_ohm_ffe, DDR_TIMMING_OFFSET_DDR_SET_T7(soc_data_drv_ohm_ffe));
			printf("\n.ddr_base_addr=0x%08x,// %d,0x%08x", ddr_set_t_p->ddr_base_addr, ddr_set_t_p->ddr_base_addr, DDR_TIMMING_OFFSET_DDR_SET_T7(ddr_base_addr));
			printf("\n.ddr_start_offset=0x%08x,// %d,0x%08x", ddr_set_t_p->ddr_start_offset, ddr_set_t_p->ddr_start_offset, DDR_TIMMING_OFFSET_DDR_SET_T7(ddr_start_offset));
			printf("\n.dram_ch0_size_MB=0x%08x,// %d,0x%08x", ddr_set_t_p_t7->dram_ch0_size_MB, ddr_set_t_p_t7->dram_ch0_size_MB, DDR_TIMMING_OFFSET_DDR_SET_T7(dram_ch0_size_MB));
			printf("\n.dram_ch1_size_MB=0x%08x,// %d,0x%08x", ddr_set_t_p_t7->dram_ch1_size_MB, ddr_set_t_p_t7->dram_ch1_size_MB, DDR_TIMMING_OFFSET_DDR_SET_T7(dram_ch1_size_MB));
		} else {
			printf("\n.dmem_load_size=0x%08x,// %d,0x%08x", ddr_set_t_p->dmem_load_size, ddr_set_t_p->dmem_load_size, DDR_TIMMING_OFFSET_DDR_SET(dmem_load_size));
			printf("\n.ddr_base_addr=0x%08x,// %d,0x%08x", ddr_set_t_p->ddr_base_addr, ddr_set_t_p->ddr_base_addr, DDR_TIMMING_OFFSET_DDR_SET(ddr_base_addr));
			printf("\n.ddr_start_offset=0x%08x,// %d,0x%08x", ddr_set_t_p->ddr_start_offset, ddr_set_t_p->ddr_start_offset, DDR_TIMMING_OFFSET_DDR_SET(ddr_start_offset));
			printf("\n.dram_cs0_size_MB=0x%08x,// %d,0x%08x", ddr_set_t_p->dram_cs0_size_MB, ddr_set_t_p->dram_cs0_size_MB, DDR_TIMMING_OFFSET_DDR_SET(dram_cs0_size_MB));
			printf("\n.dram_cs1_size_MB=0x%08x,// %d,0x%08x", ddr_set_t_p->dram_cs1_size_MB, ddr_set_t_p->dram_cs1_size_MB, DDR_TIMMING_OFFSET_DDR_SET(dram_cs1_size_MB));
		}
		printf("\n.training_SequenceCtrl[0]=0x%08x,// %d,0x%08x", ddr_set_t_p->training_SequenceCtrl[0], ddr_set_t_p->training_SequenceCtrl[0], DDR_TIMMING_OFFSET_DDR_SET(training_SequenceCtrl[0]));
		printf("\n.training_SequenceCtrl[1]=0x%08x,// %d,0x%08x", ddr_set_t_p->training_SequenceCtrl[1], ddr_set_t_p->training_SequenceCtrl[1], DDR_TIMMING_OFFSET_DDR_SET(training_SequenceCtrl[1]));
		printf("\n.phy_odt_config_rank[0]=0x%08x,// %d,0x%08x", ddr_set_t_p->phy_odt_config_rank[0], ddr_set_t_p->phy_odt_config_rank[0], DDR_TIMMING_OFFSET_DDR_SET(phy_odt_config_rank[0]));
		printf("\n.phy_odt_config_rank[1]=0x%08x,// %d,0x%08x", ddr_set_t_p->phy_odt_config_rank[1], ddr_set_t_p->phy_odt_config_rank[1], DDR_TIMMING_OFFSET_DDR_SET(phy_odt_config_rank[1]));
		printf("\n.rank1_ca_vref_permil=0x%08x,// %d,0x%08x", ddr_set_t_p->rank1_ca_vref_permil, ddr_set_t_p->rank1_ca_vref_permil, DDR_TIMMING_OFFSET_DDR_SET(rank1_ca_vref_permil));

		printf("\n.dfi_odt_config=0x%08x,// %d,0x%08x", ddr_set_t_p->dfi_odt_config, ddr_set_t_p->dfi_odt_config, DDR_TIMMING_OFFSET_DDR_SET(dfi_odt_config));
		printf("\n.DRAMFreq[0]=0x%08x,// %d,0x%08x", ddr_set_t_p->DRAMFreq[0], ddr_set_t_p->DRAMFreq[0], DDR_TIMMING_OFFSET_DDR_SET(DRAMFreq[0]));
		printf("\n.DRAMFreq[1]=0x%08x,// %d,0x%08x", ddr_set_t_p->DRAMFreq[1], ddr_set_t_p->DRAMFreq[1], DDR_TIMMING_OFFSET_DDR_SET(DRAMFreq[1]));
		printf("\n.DRAMFreq[2]=0x%08x,// %d,0x%08x", ddr_set_t_p->DRAMFreq[2], ddr_set_t_p->DRAMFreq[2], DDR_TIMMING_OFFSET_DDR_SET(DRAMFreq[2]));
		printf("\n.DRAMFreq[3]=0x%08x,// %d,0x%08x", ddr_set_t_p->DRAMFreq[3], ddr_set_t_p->DRAMFreq[3], DDR_TIMMING_OFFSET_DDR_SET(DRAMFreq[3]));
		printf("\n.PllBypassEn=0x%08x,// %d,0x%08x", ddr_set_t_p->PllBypassEn, ddr_set_t_p->PllBypassEn, DDR_TIMMING_OFFSET_DDR_SET(PllBypassEn));
		printf("\n.ddr_rdbi_wr_enable=0x%08x,// %d,0x%08x", ddr_set_t_p->ddr_rdbi_wr_enable, ddr_set_t_p->ddr_rdbi_wr_enable, DDR_TIMMING_OFFSET_DDR_SET(ddr_rdbi_wr_enable));
		printf("\n.ddr_rfc_type=0x%08x,// %d,0x%08x", ddr_set_t_p->ddr_rfc_type, ddr_set_t_p->ddr_rfc_type, DDR_TIMMING_OFFSET_DDR_SET(ddr_rfc_type));
		printf("\n.enable_lpddr4x_mode=0x%08x,// %d,0x%08x", ddr_set_t_p->enable_lpddr4x_mode, ddr_set_t_p->enable_lpddr4x_mode, DDR_TIMMING_OFFSET_DDR_SET(enable_lpddr4x_mode));
		printf("\n.pll_ssc_mode=0x%08x,// %d,0x%08x", ddr_set_t_p->pll_ssc_mode, ddr_set_t_p->pll_ssc_mode, DDR_TIMMING_OFFSET_DDR_SET(pll_ssc_mode));
		printf("\n.clk_drv_ohm=0x%08x,// %d,0x%08x", ddr_set_t_p->clk_drv_ohm, ddr_set_t_p->clk_drv_ohm, DDR_TIMMING_OFFSET_DDR_SET(clk_drv_ohm));
		printf("\n.cs_drv_ohm=0x%08x,// %d,0x%08x", ddr_set_t_p->cs_drv_ohm, ddr_set_t_p->cs_drv_ohm, DDR_TIMMING_OFFSET_DDR_SET(cs_drv_ohm));
		printf("\n.ac_drv_ohm=0x%08x,// %d,0x%08x", ddr_set_t_p->ac_drv_ohm, ddr_set_t_p->ac_drv_ohm, DDR_TIMMING_OFFSET_DDR_SET(ac_drv_ohm));
		printf("\n.soc_data_drv_ohm_p=0x%08x,// %d,0x%08x", ddr_set_t_p->soc_data_drv_ohm_p, ddr_set_t_p->soc_data_drv_ohm_p, DDR_TIMMING_OFFSET_DDR_SET(soc_data_drv_ohm_p));
		printf("\n.soc_data_drv_ohm_n=0x%08x,// %d,0x%08x", ddr_set_t_p->soc_data_drv_ohm_n, ddr_set_t_p->soc_data_drv_ohm_n, DDR_TIMMING_OFFSET_DDR_SET(soc_data_drv_ohm_n));
		printf("\n.soc_data_odt_ohm_p=0x%08x,// %d,0x%08x", ddr_set_t_p->soc_data_odt_ohm_p, ddr_set_t_p->soc_data_odt_ohm_p, DDR_TIMMING_OFFSET_DDR_SET(soc_data_odt_ohm_p));
		printf("\n.soc_data_odt_ohm_n=0x%08x,// %d,0x%08x", ddr_set_t_p->soc_data_odt_ohm_n, ddr_set_t_p->soc_data_odt_ohm_n, DDR_TIMMING_OFFSET_DDR_SET(soc_data_odt_ohm_n));
		printf("\n.dram_data_drv_ohm=0x%08x,// %d,0x%08x", ddr_set_t_p->dram_data_drv_ohm, ddr_set_t_p->dram_data_drv_ohm, DDR_TIMMING_OFFSET_DDR_SET(dram_data_drv_ohm));
		printf("\n.dram_data_odt_ohm=0x%08x,// %d,0x%08x", ddr_set_t_p->dram_data_odt_ohm, ddr_set_t_p->dram_data_odt_ohm, DDR_TIMMING_OFFSET_DDR_SET(dram_data_drv_ohm));
		printf("\n.dram_ac_odt_ohm=0x%08x,// %d,0x%08x", ddr_set_t_p->dram_ac_odt_ohm, ddr_set_t_p->dram_ac_odt_ohm, DDR_TIMMING_OFFSET_DDR_SET(dram_ac_odt_ohm));
		printf("\n.soc_clk_slew_rate=0x%08x,// %d,0x%08x", ddr_set_t_p->soc_clk_slew_rate, ddr_set_t_p->soc_clk_slew_rate, DDR_TIMMING_OFFSET_DDR_SET(soc_clk_slew_rate));
		printf("\n.soc_cs_slew_rate=0x%08x,// %d,0x%08x", ddr_set_t_p->soc_cs_slew_rate, ddr_set_t_p->soc_cs_slew_rate, DDR_TIMMING_OFFSET_DDR_SET(soc_cs_slew_rate));
		printf("\n.soc_ac_slew_rate=0x%08x,// %d,0x%08x", ddr_set_t_p->soc_ac_slew_rate, ddr_set_t_p->soc_ac_slew_rate, DDR_TIMMING_OFFSET_DDR_SET(soc_ac_slew_rate));
		printf("\n.soc_data_slew_rate=0x%08x,// %d,0x%08x", ddr_set_t_p->soc_data_slew_rate, ddr_set_t_p->soc_data_slew_rate, DDR_TIMMING_OFFSET_DDR_SET(soc_data_slew_rate));
		printf("\n.vref_output_permil =0x%08x,// %d,0x%08x", ddr_set_t_p->vref_output_permil, ddr_set_t_p->vref_output_permil, DDR_TIMMING_OFFSET_DDR_SET(vref_output_permil));
		printf("\n.vref_receiver_permil =0x%08x,// %d,0x%08x", ddr_set_t_p->vref_receiver_permil, ddr_set_t_p->vref_receiver_permil, DDR_TIMMING_OFFSET_DDR_SET(vref_receiver_permil));
		printf("\n.vref_dram_permil=0x%08x,// %d,0x%08x", ddr_set_t_p->vref_dram_permil, ddr_set_t_p->vref_dram_permil, DDR_TIMMING_OFFSET_DDR_SET(vref_dram_permil));
		printf("\n.max_core_timmming_frequency=0x%08x,// %d,0x%08x", ddr_set_t_p->max_core_timmming_frequency, ddr_set_t_p->max_core_timmming_frequency, DDR_TIMMING_OFFSET_DDR_SET(max_core_timmming_frequency));
		for (temp_count = 0; temp_count < 10; temp_count++)
			printf("\n.ac_trace_delay[%d]=0x%08x,// %d,0x%08x", temp_count, ddr_set_t_p->ac_trace_delay[temp_count], ddr_set_t_p->ac_trace_delay[temp_count], DDR_TIMMING_OFFSET_DDR_SET(ac_trace_delay[temp_count]));
		printf("\n.lpddr4_dram_vout_voltage_1_3_2_5_setting=0x%08x,// %d,0x%08x", ddr_set_t_p->lpddr4_dram_vout_voltage_1_3_2_5_setting, ddr_set_t_p->lpddr4_dram_vout_voltage_1_3_2_5_setting, DDR_TIMMING_OFFSET_DDR_SET(lpddr4_dram_vout_voltage_1_3_2_5_setting));
		printf("\n.lpddr4_x8_mode=0x%08x,// %d,0x%08x", ddr_set_t_p->lpddr4_x8_mode, ddr_set_t_p->lpddr4_x8_mode, DDR_TIMMING_OFFSET_DDR_SET(lpddr4_x8_mode));
		for (temp_count = 0; temp_count < 28; temp_count++)
			printf("\n.ac_pinmux[%d]=0x%08x,// %d,0x%08x", temp_count, ddr_set_t_p->ac_pinmux[temp_count], ddr_set_t_p->ac_pinmux[temp_count], DDR_TIMMING_OFFSET_DDR_SET(ac_pinmux[temp_count]));
		for (temp_count = 0; temp_count < 26; temp_count++)
			printf("\n.dfi_pinmux[%d]=0x%08x,// %d,0x%08x", temp_count, ddr_set_t_p->dfi_pinmux[temp_count], ddr_set_t_p->dfi_pinmux[temp_count], DDR_TIMMING_OFFSET_DDR_SET(dfi_pinmux[temp_count]));

		printf("\n.slt_test_function[0]  =0x%08x,// %d,0x%08x", ddr_set_t_p->slt_test_function[0], ddr_set_t_p->slt_test_function[0], DDR_TIMMING_OFFSET_DDR_SET(slt_test_function[0]));
		printf("\n.slt_test_function[1]  =0x%08x,// %d,0x%08x", ddr_set_t_p->slt_test_function[1], ddr_set_t_p->slt_test_function[1], DDR_TIMMING_OFFSET_DDR_SET(slt_test_function[1]));
		printf("\n.tdqs2dq=0x%08x,// %d,0x%08x", ddr_set_t_p->tdqs2dq, ddr_set_t_p->tdqs2dq, DDR_TIMMING_OFFSET_DDR_SET(tdqs2dq));
		printf("\n.dram_data_wr_odt_ohm=0x%08x,// %d,0x%08x", ddr_set_t_p->dram_data_wr_odt_ohm, ddr_set_t_p->dram_data_wr_odt_ohm, DDR_TIMMING_OFFSET_DDR_SET(dram_data_wr_odt_ohm));
		printf("\n.bitTimeControl_2d=0x%08x,// %d,0x%08x", ddr_set_t_p->bitTimeControl_2d, ddr_set_t_p->bitTimeControl_2d, DDR_TIMMING_OFFSET_DDR_SET(bitTimeControl_2d));
		if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T7))
			printf("\n.training_offset=0x%08x,// %d,0x%08x", ddr_set_t_p_t7->training_offset, ddr_set_t_p_t7->training_offset, DDR_TIMMING_OFFSET_DDR_SET_T7(training_offset));
		for (temp_count = 0; temp_count < 5; temp_count++)
			printf("\n.ddr_dmc_remap[%d]=0x%08x,// %d,0x%08x", temp_count, ddr_set_t_p->ddr_dmc_remap[temp_count], ddr_set_t_p->ddr_dmc_remap[temp_count], DDR_TIMMING_OFFSET_DDR_SET(ddr_dmc_remap[temp_count]));
		for (temp_count = 0; temp_count < 4; temp_count++)
			printf("\n.ddr_lpddr34_ca_remap[%d]=0x%08x,// %d,0x%08x", temp_count, ddr_set_t_p->ddr_lpddr34_ca_remap[temp_count], ddr_set_t_p->ddr_lpddr34_ca_remap[temp_count], DDR_TIMMING_OFFSET_DDR_SET(ddr_lpddr34_ca_remap[temp_count]));
		printf("\n.dram_rtt_nom_wr_park[0]=0x%08x,// %d,0x%08x", ddr_set_t_p->dram_rtt_nom_wr_park[0], ddr_set_t_p->dram_rtt_nom_wr_park[0], DDR_TIMMING_OFFSET_DDR_SET(dram_rtt_nom_wr_park[0]));
		printf("\n.dram_rtt_nom_wr_park[1]=0x%08x,// %d,0x%08x", ddr_set_t_p->dram_rtt_nom_wr_park[1], ddr_set_t_p->dram_rtt_nom_wr_park[1], DDR_TIMMING_OFFSET_DDR_SET(dram_rtt_nom_wr_park[1]));
		printf("\n.ddr_func=0x%08x,// %d,0x%08x", ddr_set_t_p->ddr_func, ddr_set_t_p->ddr_func, DDR_TIMMING_OFFSET_DDR_SET(ddr_func));
		for (ps = 0; ps < 2; ps++) {
			for (temp_count = 0; temp_count < 10; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].ac_trace_delay[%d]=0x%08x,// %d,0x%08x", ps, temp_count,
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].ac_trace_delay[temp_count],
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].ac_trace_delay[temp_count],
				       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].ac_trace_delay[temp_count]));

			for (temp_count = 0; temp_count < 16; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].read_dqs_delay[%d]=0x%08x,// %d,0x%08x", ps, temp_count,
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].read_dqs_delay[temp_count],
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].read_dqs_delay[temp_count],
				       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].read_dqs_delay[temp_count]));
			for (temp_count = 0; temp_count < 72; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].read_dq_bit_delay[%d]=0x%08x,// %d,0x%08x", ps, temp_count,
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].read_dq_bit_delay[temp_count],
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].read_dq_bit_delay[temp_count],
				       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].read_dq_bit_delay[temp_count]));
			for (temp_count = 0; temp_count < 16; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].write_dqs_delay[%d]=0x%08x,// %d,0x%08x", ps, temp_count,
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].write_dqs_delay[temp_count],
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].write_dqs_delay[temp_count],
				       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].write_dqs_delay[temp_count]));
			for (temp_count = 0; temp_count < 72; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].write_dq_bit_delay[%d]=0x%08x,// %d,0x%08x", ps, temp_count,
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].write_dq_bit_delay[temp_count],
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].write_dq_bit_delay[temp_count],
				       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].write_dq_bit_delay[temp_count]));
			for (temp_count = 0; temp_count < 16; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].read_dqs_gate_delay[%d]=0x%08x,// %d,0x%08x", ps, temp_count,
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].read_dqs_gate_delay[temp_count],
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].read_dqs_gate_delay[temp_count],
				       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].read_dqs_gate_delay[temp_count]));
			for (temp_count = 0; temp_count < 36; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].soc_bit_vref[%d]=0x%08x,// %d,0x%08x", ps, temp_count,
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].soc_bit_vref[temp_count],
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].soc_bit_vref[temp_count],
				       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].soc_bit_vref[temp_count]));
			for (temp_count = 0; temp_count < 36; temp_count++)
				printf("\n.cfg_ddr_phy_common_extra_set_t.csr_soc_vref_dac1_dfe[%d]=0x%08x,// %d,0x%08x", temp_count,
				       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_soc_vref_dac1_dfe[temp_count],
				       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_soc_vref_dac1_dfe[temp_count],
				       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_phy_common_extra_set_t.csr_soc_vref_dac1_dfe[temp_count]));
			for (temp_count = 0; temp_count < 32; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].dram_bit_vref[%d]=0x%08x,// %d,0x%08x", ps, temp_count,
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].dram_bit_vref[temp_count],
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].dram_bit_vref[temp_count],
				       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].dram_bit_vref[temp_count]));

			printf("\n.cfg_ddr_training_delay_ps[%d].dfi_mrl=0x%08x,// %d,0x%08x", ps,
			       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].dfi_mrl, ddr_set_t_p->cfg_ddr_training_delay_ps[ps].dfi_mrl,
			       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].dfi_mrl));
			printf("\n.cfg_ddr_training_delay_ps[%d].dfi_hwtmrl=0x%08x,// %d,0x%08x", ps,
			       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].dfi_hwtmrl, ddr_set_t_p->cfg_ddr_training_delay_ps[ps].dfi_hwtmrl,
			       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].dfi_hwtmrl));
			printf("\n.cfg_ddr_training_delay_ps[%d].ARdPtrInitVal=0x%08x,// %d,0x%08x", ps,
			       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].ARdPtrInitVal, ddr_set_t_p->cfg_ddr_training_delay_ps[ps].ARdPtrInitVal,
			       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].ARdPtrInitVal));

			printf("\n.cfg_ddr_training_delay_ps[%d].csr_vrefinglobal=0x%08x,// %d,0x%08x", ps,
			       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_vrefinglobal, ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_vrefinglobal,
			       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].csr_vrefinglobal));
			for (temp_count = 0; temp_count < 4; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].csr_dqsrcvcntrl[%d]=0x%08x,// %d,0x%08x", ps, temp_count,
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_dqsrcvcntrl[temp_count], ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_dqsrcvcntrl[temp_count],
				       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].csr_dqsrcvcntrl[temp_count]));
			for (temp_count = 0; temp_count < 4; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].csr_pptdqscntinvtrntg0[%d]=0x%08x,// %d,0x%08x", ps, temp_count,
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_pptdqscntinvtrntg0[temp_count],
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_pptdqscntinvtrntg0[temp_count],
				       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].csr_pptdqscntinvtrntg0[temp_count]));
			for (temp_count = 0; temp_count < 4; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].csr_pptdqscntinvtrntg1[%d]=0x%08x,// %d,0x%08x", ps, temp_count,
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_pptdqscntinvtrntg1[temp_count],
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_pptdqscntinvtrntg1[temp_count],
				       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].csr_pptdqscntinvtrntg1[temp_count]));
			for (temp_count = 0; temp_count < 9; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].csr_seq0bgpr[%d]=0x%08x,// %d,0x%08x", ps, temp_count,
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_seq0bgpr[temp_count],
				       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_seq0bgpr[temp_count],
				       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].csr_seq0bgpr[temp_count]));
			printf("\n.cfg_ddr_training_delay_ps[%d].csr_dllgainctl=0x%08x,// %d,0x%08x", ps,
			       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_dllgainctl,
			       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_dllgainctl,
			       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].csr_dllgainctl));
			printf("\n.cfg_ddr_training_delay_ps[%d].csr_dlllockpara=0x%08x,// %d,0x%08x", ps,
			       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_dlllockpara,
			       ddr_set_t_p->cfg_ddr_training_delay_ps[ps].csr_dlllockpara,
			       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_training_delay_ps[ps].csr_dlllockpara));
		}
		printf("\n.cfg_ddr_phy_common_extra_set_t.csr_pllctrl3=0x%08x,// %d,0x%08x",
		       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_pllctrl3,
		       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_pllctrl3,
		       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_phy_common_extra_set_t.csr_pllctrl3));
		for (temp_count = 0; temp_count < 4; temp_count++)
			printf("\n.cfg_ddr_phy_common_extra_set_t.csr_pptctlstatic[%d]=0x%08x,// %d,0x%08x", temp_count,
			       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_pptctlstatic[temp_count],
			       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_pptctlstatic[temp_count],
			       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_phy_common_extra_set_t.csr_pptctlstatic[temp_count]));
		for (temp_count = 0; temp_count < 4; temp_count++)
			printf("\n.cfg_ddr_phy_common_extra_set_t.csr_trainingincdecdtsmen[%d]=0x%08x,// %d,0x%08x", temp_count,
			       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_trainingincdecdtsmen[temp_count],
			       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_trainingincdecdtsmen[temp_count],
			       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_phy_common_extra_set_t.csr_trainingincdecdtsmen[temp_count]));
		for (temp_count = 0; temp_count < 4; temp_count++)
			printf("\n.cfg_ddr_phy_common_extra_set_t.csr_tsmbyte0[%d]=0x%08x,// %d,0x%08x", temp_count,
			       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_tsmbyte0[temp_count],
			       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_tsmbyte0[temp_count],
			       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_phy_common_extra_set_t.csr_tsmbyte0[temp_count]));

		printf("\n.cfg_ddr_phy_common_extra_set_t.csr_hwtcamode=0x%08x,// %d,0x%08x",
		       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_hwtcamode,
		       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_hwtcamode,
		       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_phy_common_extra_set_t.csr_hwtcamode));
		printf("\n.cfg_ddr_phy_common_extra_set_t.csr_hwtlpcsena=0x%08x,// %d,0x%08x",
		       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_hwtlpcsena,
		       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_hwtlpcsena,
		       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_phy_common_extra_set_t.csr_hwtlpcsena));
		printf("\n.cfg_ddr_phy_common_extra_set_t.csr_hwtlpcsenb=0x%08x,// %d,0x%08x",
		       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_hwtlpcsenb,
		       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_hwtlpcsenb,
		       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_phy_common_extra_set_t.csr_hwtlpcsenb));
		printf("\n.cfg_ddr_phy_common_extra_set_t.csr_acsmctrl13=0x%08x,// %d,0x%08x",
		       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_acsmctrl13,
		       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_acsmctrl13,
		       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_phy_common_extra_set_t.csr_acsmctrl13));
		printf("\n.cfg_ddr_phy_common_extra_set_t.csr_acsmctrl23=0x%08x,// %d,0x%08x",
		       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_acsmctrl23,
		       ddr_set_t_p->cfg_ddr_phy_common_extra_set_t.csr_acsmctrl23,
		       DDR_TIMMING_OFFSET_DDR_SET(cfg_ddr_phy_common_extra_set_t.csr_acsmctrl23));

		printf("\n},\n");
	}
#endif
	return 1;
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

uint32_t ddr_cacl_phy_delay_all_step_c2(char test_index, uint32_t value)
{
	//value	bit0-15 fine value ,bit 16-32 coarse value
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
}

uint32_t ddr_cacl_phy_over_ride_back_reg_c2(char test_index, uint32_t value)
{
	uint32_t result = 0; //bit0-15 fine value ,bit 16-32 coarse value

	if ((test_index == DMC_TEST_WINDOW_INDEX_ATXDLY)) {
		if (value > (3 * 128 + 127))
			value = (3 * 128 + 127);
		result = (value % 128) + ((value / 128) << 16);
	} else if ((test_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY) || (test_index == DMC_TEST_WINDOW_INDEX_TXDQDLY)) {
		if (value > (7 * 128 + 127))
			value = (7 * 128 + 127);
		result = (value % 128) + ((value / 128) << 16);
	} else if ((test_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY) || (test_index == DMC_TEST_WINDOW_INDEX_RXPBDLY)) {
		if (value > (1 * 128 + 127))
			value = (1 * 128 + 127);
		result = value;
	} else if ((test_index == DMC_TEST_WINDOW_INDEX_RXENDLY)) {
		if (value > (31 * 128 + 127))
			value = (31 * 128 + 127);
		result = (value % 128) + ((value / 128) << 16);
	} else if ((test_index == DMC_TEST_WINDOW_INDEX_SOC_VREF) ||
		   (test_index == DMC_TEST_WINDOW_INDEX_SOC_VREF_DAC1)) {
		if (value > (63))
			value = (63);
		result = value;
	}
	return result;
}

uint32_t ddr_disable_update_delay_line_c2(void)
{
	//config phy update use register change and ctrl update req and condition,power on default is or condition
	//disable ctrl update req
	return 1;
}

uint32_t ddr_enable_update_delay_line_c2(void)
{
	//config phy update use register change and ctrl update req and condition,power on default is or condition
	//enable ctrl update req
	return 1;
}

uint32_t ddr_phy_training_reg_read_write(ddr_set_t_c2 *p_ddrs, char index,
					 uint32_t sub_index, uint32_t read_write_value, char read_write_enable, char ps)
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
	uint64_t add_base = (p_ddr_base->ddr_phy_base_address);
	uint32_t reg_offset = 0;
	uint32_t temp_save = 0;

#define         DDR_X32_F0_A800  (0x800)
#define         DDR_X32_F0_A804  (0x804)
#define         DDR_X32_F0_A808  (0x808)
#define         DDR_X32_F0_A810  (0x810)
#define         DDR_X32_F0_A828  (0x828)
#define         DDR_X32_F0_A82C  (0x82c)
#define         DDR_X32_F0_A840  (0x840)
#define         DDR_X32_F0_A844  (0x844)
#define         DDR_X32_F0_A850  (0x850)
#define         DDR_X32_F0_A858  (0x858)
#define         DDR_X32_F0_A890  (0x890)
#define         DDR_X32_F0_A8D0  (0x8d0)
#define         DDR_X32_F0_A8F0  (0x8F0)
#define         DDR_X32_F0_A8F8  (0x8F8)
#define         DDR_X32_F0_A8D4  (0x8d4)
#define         DDR_X32_F0_A930  (0x930)
#define         DDR_X32_F0_AC08  (0xc08)
#define         DDR_X32_F0_AC2C  (0xc2c)
#define         DDR_X32_F0_AC40  (0xc40)
#define         DDR_X32_F0_AC44  (0xc44)
#define         DDR_X32_F0_AC50  (0xc50)
#define         DDR_X32_F0_AC58  (0xc58)
#define         DDR_X32_F0_ACD0  (0xcd0)
#define         DDR_X32_F0_ACD4  (0xcd4)
#define         DDR_X32_F0_ACF0  (0xcf0)
#define         DDR_X32_F0_ACF8  (0xcf8)
#define         DDR_X32_F0_AD30  (0xD30)
	reg_offset = ps * (0x1000);
	if (!index)
		return read_write_value;
	//ac group0 then ac group1
	if (index == DMC_TEST_WINDOW_INDEX_ATXDLY) {
		if (sub_index < 8) {            //ac group 0
			switch (sub_index) {
			case            0:      //cke0
				reg_add_coarse = (add_base + DDR_X32_F0_A808 + reg_offset);
				reg_add_fine = (add_base + DDR_X32_F0_A82C + reg_offset);
				reg_add_coarse_bit_mask = (~(3 << 4));
				reg_add_fine_bit_mask = (~(0x7f << 16));
				break;
			case            1:              //cke1
				reg_add_coarse = (add_base + DDR_X32_F0_AC08 + reg_offset);
				reg_add_fine = (add_base + DDR_X32_F0_AC2C + reg_offset);
				reg_add_coarse_bit_mask = (~(3 << 4));
				reg_add_fine_bit_mask = (~(0x7f << 16));
				break;
			case            2:              //cs0
				reg_add_coarse = (add_base + DDR_X32_F0_A808 + reg_offset);
				reg_add_fine = (add_base + DDR_X32_F0_A82C + reg_offset);
				reg_add_coarse_bit_mask = (~(3 << 2));
				reg_add_fine_bit_mask = (~(0x7f << 8));
				break;
			case            3:              //cs1
				reg_add_coarse = (add_base + DDR_X32_F0_AC08 + reg_offset);
				reg_add_fine = (add_base + DDR_X32_F0_AC2C + reg_offset);
				reg_add_coarse_bit_mask = (~(3 << 2));
				reg_add_fine_bit_mask = (~(0x7f << 8));
				break;
			case            4:              //odt0
				reg_add_coarse = (add_base + DDR_X32_F0_A808 + reg_offset);
				reg_add_fine = (add_base + DDR_X32_F0_A82C + reg_offset);
				reg_add_coarse_bit_mask = (~(3 << 0));
				reg_add_fine_bit_mask = (~(0x7f << 0));
				break;
			case            5:              //odt1
				reg_add_coarse = (add_base + DDR_X32_F0_AC08 + reg_offset);
				reg_add_fine = (add_base + DDR_X32_F0_AC2C + reg_offset);
				reg_add_coarse_bit_mask = (~(3 << 0));
				reg_add_fine_bit_mask = (~(0x7f << 0));
				break;
			case            6:              //clk
				reg_add_coarse = (add_base + DDR_X32_F0_A804 + reg_offset);
				reg_add_fine = (add_base + DDR_X32_F0_A828 + reg_offset);
				reg_add_coarse_bit_mask = (~(1 << 18));
				reg_add_fine_bit_mask = (~(0x7f << 8));
				break;
			case            7: //this pin use for act pin
				//reg_add_coarse = 0;
				//reg_add_fine = 0;
				reg_add_coarse = (add_base + DDR_X32_F0_A800 + (((36 - 8 - 4 - 2 + 2) / 16) << 2) + reg_offset);
				reg_add_fine = (add_base + DDR_X32_F0_A810 + (((36 - 8 - 4 - 2 + 2) / 4) << 2) + reg_offset);
				reg_add_coarse_bit_mask = (~(3 << (((36 - 8 - 4 - 2 + 2) % 16) << 1)));
				reg_add_fine_bit_mask = (~(0x7f << (((36 - 8 - 4 - 2 + 2) % 4) * 8)));

				break;
			}
		} else if (sub_index < (8 + 16)) { ////ac group 1
			reg_add_coarse = (add_base + DDR_X32_F0_A800 + (((sub_index - 8) / 16) << 2) + reg_offset);
			reg_add_fine = (add_base + DDR_X32_F0_A810 + (((sub_index - 8) / 4) << 2) + reg_offset);
			reg_add_coarse_bit_mask = (~(3 << (((sub_index - 8) % 16) << 1)));
			reg_add_fine_bit_mask = (~(0x7f << (((sub_index - 8) % 4) * 8)));
		} else if (sub_index < (8 + 16 + 4)) {
			reg_add_coarse = 0;
			reg_add_fine = 0;
		} else if (sub_index < (8 + 16 + 4 + 3 + 2)) {
			reg_add_coarse = (add_base + DDR_X32_F0_A800 + (((sub_index - 8 - 4) / 16) << 2) + reg_offset);
			reg_add_fine = (add_base + DDR_X32_F0_A810 + (((sub_index - 8 - 4) / 4) << 2) + reg_offset);
			reg_add_coarse_bit_mask = (~(3 << (((sub_index - 8 - 4) % 16) << 1)));
			reg_add_fine_bit_mask = (~(0x7f << (((sub_index - 8 - 4) % 4) * 8)));
		}
#if 0
		else if (sub_index < (8 + 16 + 4 + 3 + 2)) {
			reg_add_coarse = 0;
			reg_add_fine = 0;
		}
#endif
		else if (sub_index < (8 + 28)) {
			reg_add_coarse = (add_base + DDR_X32_F0_A800 + (((sub_index - 8 - 4 - 2 + 2) / 16) << 2) + reg_offset);
			reg_add_fine = (add_base + DDR_X32_F0_A810 + (((sub_index - 8 - 4 - 2 + 2) / 4) << 2) + reg_offset);
			reg_add_coarse_bit_mask = (~(3 << (((sub_index - 8 - 4 - 2 + 2) % 16) << 1)));
			reg_add_fine_bit_mask = (~(0x7f << (((sub_index - 8 - 4 - 2 + 2) % 4) * 8)));
		} else {
			reg_add_coarse = 0;
			reg_add_fine = 0;
		}
	} else if (index == DMC_TEST_WINDOW_INDEX_TXDQSDLY) {
		reg_add_coarse = (add_base + DDR_X32_F0_A8D4 + (sub_index / 4) * (
					  DDR_X32_F0_ACD4 - DDR_X32_F0_A8D4) + (((sub_index % 4) << 1) << 2) + reg_offset);
		reg_add_fine = (add_base + DDR_X32_F0_A8F8 + (sub_index / 4) * (
					DDR_X32_F0_ACF8 - DDR_X32_F0_A8F8) + ((sub_index % 4) << 4) + reg_offset);
		reg_add_coarse_bit_mask = (~(3 << ((4))));
		reg_add_fine_bit_mask = (~(0x7f << 8));
	} else if (index == DMC_TEST_WINDOW_INDEX_RXCLKDLY) {
		reg_add_coarse = 0;
		reg_add_fine = (add_base + DDR_X32_F0_A858 + (sub_index / 4) * (
					DDR_X32_F0_AC58 - DDR_X32_F0_A858) + ((sub_index % 4) << 4) + reg_offset);
		reg_add_coarse_bit_mask = 0;
		reg_add_fine_bit_mask = (~(0xff << (8)));
	}
	//d0-d7 dm0 d8-d15 dm1...
	else if (index == DMC_TEST_WINDOW_INDEX_TXDQDLY) {
		reg_add_coarse = (add_base + DDR_X32_F0_A8D0 + (sub_index / 36) * (DDR_X32_F0_ACD0
										   - DDR_X32_F0_A8D0) + ((((sub_index % 36) / 9) << 1) << 2) + (((((sub_index) % 9) / 8)) << 2) + reg_offset);
		reg_add_fine = (add_base + DDR_X32_F0_A8F0 + (sub_index / 36) * (DDR_X32_F0_ACF0
										 - DDR_X32_F0_A8F0) + ((((sub_index % 36) / 9)) << 4) + (((((sub_index) % 9)) >> 2) << 2) + reg_offset);
		reg_add_coarse_bit_mask = (~(7 << (((((sub_index) % 9) % 8) << 2))));
		reg_add_fine_bit_mask = (~(0x7f << (((((sub_index) % 9) % 4) << 3))));
	}
	//d0-d7 dm0 d8-d15 dm1...
	else if (index == DMC_TEST_WINDOW_INDEX_RXPBDLY) {
		reg_add_coarse = 0;
		reg_add_fine = (add_base + DDR_X32_F0_A850 + (sub_index / 36) * (DDR_X32_F0_AC50
										 - DDR_X32_F0_A850) + ((((sub_index % 36) / 9)) << 4) + (((sub_index % 9) >> 2) << 2) + reg_offset);
		reg_add_coarse_bit_mask = 0;
		reg_add_fine_bit_mask = (~(0xff << (((((sub_index) % 9) % 4) << 3))));
	} else if (index == DMC_TEST_WINDOW_INDEX_RXENDLY) {
		reg_add_coarse = (add_base + DDR_X32_F0_A840 + (sub_index / 4) * (DDR_X32_F0_AC40
										  - DDR_X32_F0_A840) + reg_offset);
		reg_add_fine = (add_base + DDR_X32_F0_A844 + (sub_index / 4) * (DDR_X32_F0_AC44
										- DDR_X32_F0_A844) + reg_offset);
		reg_add_coarse_bit_mask = (~(0x1f << ((sub_index << 3))));
		reg_add_fine_bit_mask = (~(0x7f << ((sub_index << 3))));
	} else if (index == DMC_TEST_WINDOW_INDEX_SOC_VREF) {
		if (sub_index < (36)) {         //vref dq and dbi
			reg_add_coarse = 0;
			reg_add_fine = (add_base + DDR_X32_F0_A890 + (((sub_index / 9) * 4) << 2)
					+ (((sub_index % 9) >> 2) << 2) + reg_offset);
			reg_add_coarse_bit_mask = 0;
			reg_add_fine_bit_mask = (~(0x3f << (((((sub_index) % 9) % 4) << 3))));
		} else if (sub_index < (44)) {  //vref dqs and dqsn
			reg_add_coarse = 0;
			reg_add_fine = (add_base + DDR_X32_F0_A890 + 8 + ((((sub_index - 36) / 2) * 4) << 2) + reg_offset);
			reg_add_coarse_bit_mask = 0;
			reg_add_fine_bit_mask = (~(0x3f << (((((sub_index - 36) % 2)) << 3) + 8)));
		}
	} else if (index == DMC_TEST_WINDOW_INDEX_SOC_VREF_DAC1) {
		if (sub_index < (36)) {         //vref dq and dbi
			reg_add_coarse = 0;
			reg_add_fine = (add_base + DDR_X32_F0_A890 + 0x100 + (((sub_index / 9) * 4) << 2)
					+ (((sub_index % 9) >> 2) << 2) + reg_offset);
			reg_add_coarse_bit_mask = 0;
			reg_add_fine_bit_mask = (~(0x3f << (((((sub_index) % 9) % 4) << 3))));
		} else if (sub_index < (44)) {  //vref dqs and dqsn
			reg_add_coarse = 0;
			reg_add_fine = (add_base + DDR_X32_F0_A890 + 0x100 + 8 + ((((sub_index - 36) / 2) * 4) << 2) + reg_offset);
			reg_add_coarse_bit_mask = 0;
			reg_add_fine_bit_mask = (~(0x3f << (((((sub_index - 36) % 2)) << 3) + 8)));
		}
	} else if (index == DMC_TEST_WINDOW_INDEX_DRAM_VREF) {
		if (sub_index < (36)) {         //DDR_X32_F0_AD30	DDR_X32_F0_A930
			reg_add_coarse = 0;
			reg_add_fine = (add_base + DDR_X32_F0_A930 +
					((DDR_X32_F0_AD30 - DDR_X32_F0_A930) * sub_index % 2) + reg_offset);
			reg_add_coarse_bit_mask = 0;
			reg_add_fine_bit_mask = (~(0xff << 0));
		}
	} else if (index == DMC_TEST_WINDOW_INDEX_EXTERA_PS) {
		if (sub_index == 1) {           //DDR_X32_F0_AD30	DDR_X32_F0_A930
		}

		return read_write_value;
	}

	{
		if (reg_add_coarse)
			delay_reg_coarse_value = (((rd_reg(reg_add_coarse)) & (~reg_add_coarse_bit_mask))
						  >> (ddr_mask_convert_offset(reg_add_coarse_bit_mask)));
		if (reg_add_fine)
			delay_reg_fine_value = (((rd_reg(reg_add_fine)) & (~reg_add_fine_bit_mask))
						>> (ddr_mask_convert_offset(reg_add_fine_bit_mask)));

		delay_old_value = ((delay_reg_coarse_value << 16) | delay_reg_fine_value);
		delay_old_value = ddr_cacl_phy_delay_all_step_c2(index, delay_old_value);
	}

	if (read_write_enable == REGISTER_READ) {
		read_write_value = delay_old_value;
	} else if (read_write_enable == REGISTER_WRITE) {
		delay_new_value = read_write_value;
		delay_new_value = ddr_cacl_phy_over_ride_back_reg_c2(index, delay_new_value);
		temp_save = rd_reg(p_ddr_base->ddr_dmc_refresh_ctrl_address);
		wr_reg((p_ddr_base->ddr_dmc_refresh_ctrl_address), 0x21);       //take care T5 no set bit //bit 22  dmc to control DFI_CTRLUPD_REQ  with zq generation together.
		ddr_udelay_dummy(1);
		wr_reg(p_ddr_base->ddr_phy_base_address + 0x2440, 1);           //detect should update delay when controller update arrive
		ddr_udelay_dummy(1);
		if (reg_add_coarse)
			wr_reg(reg_add_coarse, ((rd_reg(reg_add_coarse)) & (reg_add_coarse_bit_mask))
			       | ((delay_new_value >> 16) << (ddr_mask_convert_offset(reg_add_coarse_bit_mask))));
		if (reg_add_fine)
			wr_reg(reg_add_fine, ((rd_reg(reg_add_fine)) & (reg_add_fine_bit_mask))
			       | ((delay_new_value & 0xffff) << (ddr_mask_convert_offset(reg_add_fine_bit_mask))));
		ddr_udelay_dummy(1);
		//wr_reg(p_ddr_base->ddr_phy_base_address + 0x2440, 1); //no need force release ,because maybe have glitch when ddr read/write ,must upadate on the rfc stage
		//wr_reg(p_ddr_base->ddr_phy_base_address + 0x2440, 2);
		//ddr_udelay_dummy(1);
		//wr_reg(p_ddr_base->ddr_phy_base_address + 0x2440, 0);
		//ddr_udelay_dummy(1);
		wr_reg((p_ddr_base->ddr_dmc_refresh_ctrl_address), (temp_save & (~((1 << 22) | (1 << 4)))));
		ddr_udelay_dummy(1);
		wr_reg((p_ddr_base->ddr_dmc_refresh_ctrl_address), (temp_save & (~((1 << 22)))) | (1 << 4));
		ddr_udelay_dummy(40);        //since we are use 3 refresh time ,so shuld  over 4*7.8us make sure a update command send out ,then maybe fail
		wr_reg((p_ddr_base->ddr_dmc_refresh_ctrl_address), (temp_save & (~((1 << 22)))) | (1 << 4));
		ddr_udelay_dummy(1);
		wr_reg((p_ddr_base->ddr_dmc_refresh_ctrl_address), (temp_save));
	}
	printf("delay_old_value,%08x,read_write_value,%08x,index,%08x,sub_index,%08x\n", delay_old_value, read_write_value, index, sub_index);

	return read_write_value;
}

void ddr_read_write_training_value(ddr_set_t_c2 *p_ddrs, char over_ride_index,
				   char read_write, uint32_t ps, void *input, char print)
{
	uint16_t t_count = 0;
	char count_max = 72;
	uint16_t t_count_value = 0;
	uint16_t delay_temp = 0;
	char *input_uint8_p = input;
	uint16_t *input_uint16_p = input;
	char p_size = 1;
	char read_skip = 0;

	for (t_count = 0; t_count < count_max; t_count++) {
		if ((over_ride_index == DMC_TEST_WINDOW_INDEX_ATXDLY)
		    || (over_ride_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY)
		    || (over_ride_index == DMC_TEST_WINDOW_INDEX_RXPBDLY)
		    || (over_ride_index == DMC_TEST_WINDOW_INDEX_RXENDLY)
		    || (over_ride_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY)
		    || (over_ride_index == DMC_TEST_WINDOW_INDEX_TXDQDLY)
		    || (over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF)
		    || (over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF_DAC1)
		    ) {
			if (over_ride_index == DMC_TEST_WINDOW_INDEX_ATXDLY) {
				p_size = 2;
				count_max = 36;
			} else if (over_ride_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY) {
				p_size = 2;
				count_max = 8;
			} else if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXPBDLY) {
				p_size = 1;
				count_max = 72;
			} else if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXENDLY) {
				p_size = 2;
				count_max = 8;
			} else if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY) {
				p_size = 1;
				count_max = 8;
			} else if (over_ride_index == DMC_TEST_WINDOW_INDEX_TXDQDLY) {
				p_size = 2;
				count_max = 72;
			} else if ((over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF) ||
				   (over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF_DAC1)) {
				p_size = 1;
				count_max = 44;
			} //else if (over_ride_index == DMC_TEST_WINDOW_INDEX_EXTERA_PS) {
			  //	p_size = 2;
			  //	count_max = 8;
			  //}
			if (read_write == REGISTER_READ) {
				read_skip = 0;
				if ((ps == 3) && ((over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF) ||
						  (over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF_DAC1))) {
					if (t_count > 35)
						read_skip = 1;
					else    //ro training no vref value
					if ((t_count % 9) == 8)
						read_skip = 1;
				}
				t_count_value = ddr_phy_training_reg_read_write(p_ddrs,
										over_ride_index, t_count, t_count_value, read_write, ps);
				delay_temp = t_count_value;
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
				ddr_phy_training_reg_read_write(p_ddrs, over_ride_index, t_count, t_count_value, read_write, ps);
			}
			if (p_size == 1)
				input_uint8_p++;
			if (p_size == 2)
				input_uint16_p++;

			if (print == DDR_PRINT_ENABLE) {
				printf("training_index	%d	sub_index	%d	phase	%d\n", over_ride_index, t_count, t_count_value);
			} else {            //maybe function will  be optimize not use the  variable
			}
			//printf("training_index	%d	sub_index	%d	phase	%d\n", over_ride_index, t_count, t_count_value);
		}
	}
} /* ddr_read_write_training_value */

void ddr_read_write_training_all_delay_value(ddr_set_t_c2 *p_ddrs, char read_write, char print)
{
	uint32_t ps = 0;

	for (ps = 0; ps < 2; ) {
		ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_ATXDLY,
					      read_write, ps, &(p_ddrs->cfg_ddr_training_delay_ps[ps].ac_trace_delay), print);
		ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_TXDQSDLY,
					      read_write, ps, &(p_ddrs->cfg_ddr_training_delay_ps[ps].write_dqs_delay), print);
		ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_RXCLKDLY,
					      read_write, ps, &(p_ddrs->cfg_ddr_training_delay_ps[ps].read_dqs_delay), print);
		ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_TXDQDLY,
					      read_write, ps, &(p_ddrs->cfg_ddr_training_delay_ps[ps].write_dq_bit_delay), print);
		ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_RXPBDLY,
					      read_write, ps, &(p_ddrs->cfg_ddr_training_delay_ps[ps].read_dq_bit_delay), print);
		ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_RXENDLY,
					      read_write, ps, &(p_ddrs->cfg_ddr_training_delay_ps[ps].read_dqs_gate_delay), print);
		ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_SOC_VREF,
					      read_write, ps, &(p_ddrs->cfg_ddr_training_delay_ps[ps].soc_bit_vref), print);
		ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_SOC_VREF_DAC1,
					      read_write, ps, &(p_ddrs->cfg_ddr_training_delay_ps[ps].soc_bit_vref_dac1), print);
		ddr_read_write_training_value(p_ddrs, DMC_TEST_WINDOW_INDEX_EXTERA_PS,
					      read_write, ps, 0, print);
		p_ddrs->cfg_ddr_training_delay_ps[ps].dram_bit_vref[0] = ((rd_reg(p_ddr_base->ddr_phy_base_address + 0x3930)) & 0x3f);

		ps = ps + 1;
	}
}

uint32_t ddr_get_c2_bdlr_100step_min(void)
{
	uint32_t bdlr_100step = 0;
	uint32_t DRAMFreq = 0;
	uint32_t stick_store_sticky_f0_reg_base_t = (p_ddr_base->ddr_phy_base_address + 0x0128);

	DRAMFreq = rd_reg(stick_store_sticky_f0_reg_base_t);
	uint32_t dll_counter = 0;
	uint32_t dll_counter_max = 0;

	dll_counter = (((rd_reg(p_ddr_base->ddr_phy_base_address + 0x3130))));
	dll_counter_max = ddr_max((dll_counter & 0xff), ((dll_counter >> 8) & 0xff));
	dll_counter_max = ddr_max(dll_counter_max, ((dll_counter >> 16) & 0xff));
	dll_counter_max = ddr_max(dll_counter_max, ((dll_counter >> 24) & 0xff));
	dll_counter_max = dll_counter_max ? dll_counter_max : 1;
	bdlr_100step = (100000000 / (2 * (DRAMFreq))) / (dll_counter_max);
	return bdlr_100step;
}

uint32_t ddr_get_c2_bdlr_100step_max(void)
{
	uint32_t bdlr_100step = 0;
	uint32_t DRAMFreq = 0;
	uint32_t stick_store_sticky_f0_reg_base_t = (p_ddr_base->ddr_phy_base_address + 0x0128);

	DRAMFreq = rd_reg(stick_store_sticky_f0_reg_base_t);
	uint32_t dll_counter = 0;
	uint32_t dll_counter_min = 0;

	dll_counter = (((rd_reg(p_ddr_base->ddr_phy_base_address + 0x312c))));
	dll_counter_min = ddr_min((dll_counter & 0xff), ((dll_counter >> 8) & 0xff));
	dll_counter_min = ddr_min(dll_counter_min, ((dll_counter >> 16) & 0xff));
	dll_counter_min = ddr_min(dll_counter_min, ((dll_counter >> 24) & 0xff));
	dll_counter_min = dll_counter_min ? dll_counter_min : 1;
	bdlr_100step = (100000000 / (2 * (DRAMFreq))) / (dll_counter_min);
	return bdlr_100step;
}

uint32_t ddr_get_c2_bdlr_100step(void)
{
	uint32_t bdlr_100step = 0;
	uint32_t DRAMFreq = 0;
	uint32_t dll_counter = 0;
	uint32_t stick_store_sticky_f0_reg_base_t = (p_ddr_base->ddr_phy_base_address + 0x0128);

	DRAMFreq = rd_reg(stick_store_sticky_f0_reg_base_t);
	dll_counter = rd_reg(p_ddr_base->ddr_phy_base_address + 0x3128);
	dll_counter = (((dll_counter & 0xff) + ((dll_counter >> 8) & 0xff) + ((dll_counter >> 16) & 0xff) + ((dll_counter >> 24) & 0xff)) >> 2);
	bdlr_100step = (100000000 / (2 * DRAMFreq)) / (dll_counter + 1);
	return bdlr_100step;
}

uint32_t ddr_get_c2_bdlr_100step_cur(void)
{
	uint32_t bdlr_100step = 0;
	uint32_t DRAMFreq = 0;
	uint32_t dll_counter = 0;
	uint32_t stick_store_sticky_f0_reg_base_t = (p_ddr_base->ddr_phy_base_address + 0x0128);

	DRAMFreq = rd_reg(stick_store_sticky_f0_reg_base_t);
	dll_counter = rd_reg(p_ddr_base->ddr_phy_base_address + 0x3100);
	dll_counter = (((dll_counter >> 1) & 0xff));
	bdlr_100step = (100000000 / (2 * DRAMFreq)) / (dll_counter + 1);
	//	printf("\nrdll_counter=%08x ", rd_reg(p_ddr_base->ddr_phy_base_address + 0x3100));
	return bdlr_100step;
}

uint32_t ddr_get_ui_1_128_100step(void)
{
	uint32_t DRAMFreq = 0;
	uint32_t stick_store_sticky_f0_reg_base_t = (p_ddr_base->ddr_phy_base_address + 0x0128);

	DRAMFreq = rd_reg(stick_store_sticky_f0_reg_base_t);
	return (1000000 * 100 / (2 * 128)) / ((DRAMFreq));
}

uint32_t do_read_c2_ddr_bdlr_steps(void)
{
	uint32_t DRAMFreq = 0;

	DRAMFreq = get_ddr_clk();
	printf("\nc2_chip_DRAMFreq=%d MHz,100min_bdlr=%d ps,100max_bdlr=%d ps,ave_100_bdlr=%d ps,bdlr_var=%d thousand,cur_100_bdlr=%d ps\n",
	       DRAMFreq, ddr_get_c2_bdlr_100step_min(), ddr_get_c2_bdlr_100step_max(), ddr_get_c2_bdlr_100step(),
	       2000 * (ddr_get_c2_bdlr_100step_max() - ddr_get_c2_bdlr_100step_min()) / (ddr_get_c2_bdlr_100step_max() + ddr_get_c2_bdlr_100step_min()), ddr_get_c2_bdlr_100step_cur());

	return ddr_get_c2_bdlr_100step();
}

int do_read_c2_ddr_training_data(char log_level, ddr_set_t_c2 *ddr_set_t_p)
{
	uint32_t stick_store_sticky_f0_reg_base_t = (p_ddr_base->ddr_phy_base_address + 0x0128);
	uint32_t stick_store_sticky_f1_reg_base_t = (p_ddr_base->ddr_phy_base_address + 0x1128);

	printf_log(log_level, "\nddr_set_t_p==0x%08x\n", (uint32_t)(uint64_t)(ddr_set_t_p));
	uint32_t loop = 0;
	uint32_t loop_max = (4 + (0x3f << 2)); //((DMC_STICKY_63-DMC_STICKY_0));
	for (loop = 0; loop < loop_max; loop += 4)
		wr_reg(((uint64_t)(ddr_set_t_p) + loop), rd_reg((p_ddr_base->ddr_dmc_sticky0) + loop));
	loop_max = sizeof(board_SI_setting_ps_t);
	for (loop = 0; loop < loop_max; loop += 4)
		wr_reg(((uint64_t)(&(ddr_set_t_p->cfg_board_SI_setting_ps[0])) + loop), rd_reg((stick_store_sticky_f0_reg_base_t + loop)));
	for (loop = 0; loop < loop_max; loop += 4)
		wr_reg(((uint64_t)(&(ddr_set_t_p->cfg_board_SI_setting_ps[1])) + loop), rd_reg((stick_store_sticky_f1_reg_base_t + loop)));
	for (loop = 0; loop < MESON_CPU_CHIP_ID_SIZE; loop++)   //update chip id

		ddr_sha_c2.sha_chip_id[loop] = global_chip_id[loop];
	{
		bdlr_100step = get_bdlr_100step(global_ddr_clk);
		ui_1_32_100step = (1000000 * 100 / (global_ddr_clk * 2 * 32));
		ddr_read_write_training_all_delay_value(ddr_set_t_p, REGISTER_READ, ~log_level);
	}
	return 1;
}

int do_ddr_display_c2_ddr_information(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int i = 0;
	unsigned int ps = 0;

	printf("\nargc== 0x%08x\n", argc);
	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);

	do_read_c2_ddr_training_data(0, ddr_set_t_p);

	{
		uint32_t count = 0;
		uint32_t reg_add_offset = 0;

		printf("\n PCTL timming: 0x");

		for (count = 0; count < ((p_ddr_base->ddr_pctl_timing_end_address) - (p_ddr_base->ddr_pctl_timing_base_address)); ) {
			reg_add_offset = ((p_ddr_base->ddr_pctl_timing_base_address) + (count));
			printf("\n reg_add_offset: %08x %08x %08x ", reg_add_offset, readl(reg_add_offset), reg_add_offset);
			count = count + 4;
		}
		printf("\n mrs register: ");
		printf("\n mrs register: base (0x54000<<1)+DDR_PHY_BASE,%08x  byte offset\n", (0x54000 << 1) + (p_ddr_base->ddr_phy_base_address));

		printf("\n sticky register: ");
		{
			uint32_t loop_max = 0;
			loop_max = 64 << 2; //sizeof(ddr_set_t);
			for (count = 0; count < loop_max; count += 4)
				printf("\n reg_add_offset: %08x %08x %08x", count, rd_reg((uint64_t)((p_ddr_base->ddr_dmc_sticky0)) + count), (((p_ddr_base->ddr_dmc_sticky0)) + count));
		}

		{
			uint32_t loop_max = 0;
			loop_max = sizeof(ddr_set_t_c2);
			uint32_t count = 0;
			for (count = 0; count < loop_max; ) {
				printf("\n%08x %08x", count, rd_reg((uint64_t)(ddr_set_t_p) + count));
				count = count + 4;
			}
		}
	}

	printf("\n {");

	uint32_t temp_count = 0;
	{
		printf("\n//old fast_boot[%d]=0x%08x,// %d", 0, ddr_set_t_p->cfg_board_common_setting.fast_boot[0], ddr_set_t_p->cfg_board_common_setting.fast_boot[0]);
		ddr_set_t_p->cfg_board_common_setting.fast_boot[0] = 0xfd; //add for auto copy to  code test
		for (temp_count = 0; temp_count < 4; temp_count++)
			printf("\n.cfg_board_common_setting.fast_boot[%d]=0x%08x,// %d", temp_count, ddr_set_t_p->cfg_board_common_setting.fast_boot[temp_count], ddr_set_t_p->cfg_board_common_setting.fast_boot[temp_count]);

		printf("\n.cfg_board_common_setting.timming_magic=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.timming_magic, ddr_set_t_p->cfg_board_common_setting.timming_magic);
		printf("\n.cfg_board_common_setting.timming_max_valid_configs=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.timming_max_valid_configs, ddr_set_t_p->cfg_board_common_setting.timming_max_valid_configs);
		printf("\n.cfg_board_common_setting.timming_struct_version=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.timming_struct_version, ddr_set_t_p->cfg_board_common_setting.timming_struct_version);
		printf("\n.cfg_board_common_setting.timming_struct_org_size=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.timming_struct_org_size, ddr_set_t_p->cfg_board_common_setting.timming_struct_org_size);
		printf("\n.cfg_board_common_setting.timming_struct_real_size=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.timming_struct_real_size, ddr_set_t_p->cfg_board_common_setting.timming_struct_real_size);
		printf("\n.cfg_board_common_setting.ddr_func=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.ddr_func, ddr_set_t_p->cfg_board_common_setting.ddr_func);
		printf("\n.cfg_board_common_setting.board_id=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.board_id, ddr_set_t_p->cfg_board_common_setting.board_id);
		printf("\n.cfg_board_common_setting.DramType=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.DramType, ddr_set_t_p->cfg_board_common_setting.DramType);
		printf("\n.cfg_board_common_setting.dram_rank_config=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.dram_rank_config, ddr_set_t_p->cfg_board_common_setting.dram_rank_config);
		printf("\n.cfg_board_common_setting.DisabledDbyte=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.DisabledDbyte, ddr_set_t_p->cfg_board_common_setting.DisabledDbyte);
		printf("\n.cfg_board_common_setting.dram_cs0_base_add=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.dram_cs0_base_add, ddr_set_t_p->cfg_board_common_setting.dram_cs0_base_add);
		printf("\n.cfg_board_common_setting.dram_cs1_base_add=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.dram_cs1_base_add, ddr_set_t_p->cfg_board_common_setting.dram_cs1_base_add);
		if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)) {
			printf("\n.cfg_board_common_setting.dram_ch0_size_MB=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.dram_cs0_size_MB, ddr_set_t_p->cfg_board_common_setting.dram_cs0_size_MB);
			printf("\n.cfg_board_common_setting.dram_ch1_size_MB=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.dram_cs1_size_MB, ddr_set_t_p->cfg_board_common_setting.dram_cs1_size_MB);
		} else {
			printf("\n.cfg_board_common_setting.dram_cs0_size_MB=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.dram_cs0_size_MB, ddr_set_t_p->cfg_board_common_setting.dram_cs0_size_MB);
			printf("\n.cfg_board_common_setting.dram_cs1_size_MB=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.dram_cs1_size_MB, ddr_set_t_p->cfg_board_common_setting.dram_cs1_size_MB);
		}
		printf("\n.cfg_board_common_setting.dram_x4x8x16_mode=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.dram_x4x8x16_mode, ddr_set_t_p->cfg_board_common_setting.dram_x4x8x16_mode);
		printf("\n.cfg_board_common_setting.Is2Ttiming=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.Is2Ttiming, ddr_set_t_p->cfg_board_common_setting.Is2Ttiming);
		printf("\n.cfg_board_common_setting.log_level=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.log_level, ddr_set_t_p->cfg_board_common_setting.log_level);
		printf("\n.cfg_board_common_setting.ddr_rdbi_wr_enable=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.ddr_rdbi_wr_enable, ddr_set_t_p->cfg_board_common_setting.ddr_rdbi_wr_enable);
		printf("\n.cfg_board_common_setting.pll_ssc_mode=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.pll_ssc_mode, ddr_set_t_p->cfg_board_common_setting.pll_ssc_mode);
		printf("\n.cfg_board_common_setting.org_tdqs2dq=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.org_tdqs2dq, ddr_set_t_p->cfg_board_common_setting.org_tdqs2dq);
		for (temp_count = 0; temp_count < 2; temp_count++)
			printf("\n.cfg_board_common_setting.reserve1_test_function[%d]=0x%08x,// %d", temp_count, ddr_set_t_p->cfg_board_common_setting.reserve1_test_function[temp_count], ddr_set_t_p->cfg_board_common_setting.reserve1_test_function[temp_count]);
		for (temp_count = 0; temp_count < 5; temp_count++)
			printf("\n.cfg_board_common_setting.ddr_dmc_remap[%d]=0x%08x,// %d", temp_count, ddr_set_t_p->cfg_board_common_setting.ddr_dmc_remap[temp_count], ddr_set_t_p->cfg_board_common_setting.ddr_dmc_remap[temp_count]);
		for (temp_count = 0; temp_count < 35; temp_count++)
			printf("\n.cfg_board_common_setting.ac_pinmux[%d]=0x%08x,// %d", temp_count, ddr_set_t_p->cfg_board_common_setting.ac_pinmux[temp_count], (uint32_t)ddr_set_t_p->cfg_board_common_setting.ac_pinmux[temp_count]);
		printf("\n.cfg_board_common_setting.ddr_dqs_swap=0x%08x,// %d", ddr_set_t_p->cfg_board_common_setting.ddr_dqs_swap, ddr_set_t_p->cfg_board_common_setting.ddr_dqs_swap);
		for (temp_count = 0; temp_count < 36; temp_count++)
			printf("\n.cfg_board_common_setting.ddr_dq_remap[%d]=0x%08x,// %d", temp_count, ddr_set_t_p->cfg_board_common_setting.ddr_dq_remap[temp_count], (uint32_t)ddr_set_t_p->cfg_board_common_setting.ddr_dq_remap[temp_count]);
		for (temp_count = 0; temp_count < 4; temp_count++)
			printf("\n.cfg_board_common_setting.ddr_vddee_setting[%d]=0x%08x,// %d", temp_count, ddr_set_t_p->cfg_board_common_setting.ddr_vddee_setting[temp_count], (uint32_t)ddr_set_t_p->cfg_board_common_setting.ddr_vddee_setting[temp_count]);




		for (ps = 0; ps < 2; ps++) {
			printf("\n.cfg_board_SI_setting_ps[%d].DRAMFreq=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].DRAMFreq, ddr_set_t_p->cfg_board_SI_setting_ps[ps].DRAMFreq);
			printf("\n.cfg_board_SI_setting_ps[%d].PllBypassEn=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].PllBypassEn, ddr_set_t_p->cfg_board_SI_setting_ps[ps].PllBypassEn);
			printf("\n.cfg_board_SI_setting_ps[%d].training_SequenceCtrl=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].training_SequenceCtrl, ddr_set_t_p->cfg_board_SI_setting_ps[ps].training_SequenceCtrl);
			printf("\n.cfg_board_SI_setting_ps[%d].ddr_odt_config=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].ddr_odt_config, ddr_set_t_p->cfg_board_SI_setting_ps[ps].ddr_odt_config);
			printf("\n.cfg_board_SI_setting_ps[%d].clk_drv_ohm=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].clk_drv_ohm, ddr_set_t_p->cfg_board_SI_setting_ps[ps].clk_drv_ohm);
			printf("\n.cfg_board_SI_setting_ps[%d].cs_drv_ohm=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].cs_drv_ohm, ddr_set_t_p->cfg_board_SI_setting_ps[ps].cs_drv_ohm);
			printf("\n.cfg_board_SI_setting_ps[%d].ac_drv_ohm=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].ac_drv_ohm, ddr_set_t_p->cfg_board_SI_setting_ps[ps].ac_drv_ohm);
			printf("\n.cfg_board_SI_setting_ps[%d].soc_data_drv_ohm_p=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].soc_data_drv_ohm_p, ddr_set_t_p->cfg_board_SI_setting_ps[ps].soc_data_drv_ohm_p);
			printf("\n.cfg_board_SI_setting_ps[%d].soc_data_drv_ohm_n=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].soc_data_drv_ohm_n, ddr_set_t_p->cfg_board_SI_setting_ps[ps].soc_data_drv_ohm_n);
			printf("\n.cfg_board_SI_setting_ps[%d].soc_data_odt_ohm_p=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].soc_data_odt_ohm_p, ddr_set_t_p->cfg_board_SI_setting_ps[ps].soc_data_odt_ohm_p);
			printf("\n.cfg_board_SI_setting_ps[%d].soc_data_odt_ohm_n=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].soc_data_odt_ohm_n, ddr_set_t_p->cfg_board_SI_setting_ps[ps].soc_data_odt_ohm_n);
			printf("\n.cfg_board_SI_setting_ps[%d].dram_data_drv_ohm=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].dram_data_drv_ohm, ddr_set_t_p->cfg_board_SI_setting_ps[ps].dram_data_drv_ohm);
			printf("\n.cfg_board_SI_setting_ps[%d].dram_data_odt_ohm=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].dram_data_odt_ohm, ddr_set_t_p->cfg_board_SI_setting_ps[ps].dram_data_odt_ohm);
			printf("\n.cfg_board_SI_setting_ps[%d].dram_data_wr_odt_ohm=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].dram_data_wr_odt_ohm, ddr_set_t_p->cfg_board_SI_setting_ps[ps].dram_data_wr_odt_ohm);
			printf("\n.cfg_board_SI_setting_ps[%d].dram_ac_odt_ohm=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].dram_ac_odt_ohm, ddr_set_t_p->cfg_board_SI_setting_ps[ps].dram_ac_odt_ohm);
			printf("\n.cfg_board_SI_setting_ps[%d].dram_data_drv_pull_up_calibration_ohm=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].dram_data_drv_pull_up_calibration_ohm, ddr_set_t_p->cfg_board_SI_setting_ps[ps].dram_data_drv_pull_up_calibration_ohm);
			printf("\n.cfg_board_SI_setting_ps[%d].lpddr4_dram_vout_voltage_range_setting=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].lpddr4_dram_vout_voltage_range_setting, ddr_set_t_p->cfg_board_SI_setting_ps[ps].lpddr4_dram_vout_voltage_range_setting);
			if (p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_S4) {
				printf("\n.cfg_board_SI_setting_ps[%d].dfe_offset=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].dfe_offset, ddr_set_t_p->cfg_board_SI_setting_ps[ps].dfe_offset);
			}
			printf("\n.cfg_board_SI_setting_ps[%d].vref_ac_permil =0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].vref_ac_permil, ddr_set_t_p->cfg_board_SI_setting_ps[ps].vref_ac_permil);
			printf("\n.cfg_board_SI_setting_ps[%d].vref_soc_data_permil =0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].vref_soc_data_permil, ddr_set_t_p->cfg_board_SI_setting_ps[ps].vref_soc_data_permil);
			printf("\n.cfg_board_SI_setting_ps[%d].vref_dram_data_permil=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].vref_dram_data_permil, ddr_set_t_p->cfg_board_SI_setting_ps[ps].vref_dram_data_permil);
			printf("\n.cfg_board_SI_setting_ps[%d].max_core_timmming_frequency=0x%08x,// %d", ps, ddr_set_t_p->cfg_board_SI_setting_ps[ps].max_core_timmming_frequency, ddr_set_t_p->cfg_board_SI_setting_ps[ps].max_core_timmming_frequency);
			for (temp_count = 0; temp_count < 2; temp_count++)
				printf("\n.cfg_board_SI_setting_ps[%d].training_phase_parameter[%d]=0x%08x,// %d", ps, temp_count, ddr_set_t_p->cfg_board_SI_setting_ps[ps].training_phase_parameter[temp_count], (uint32_t)ddr_set_t_p->cfg_board_SI_setting_ps[ps].training_phase_parameter[temp_count]);
			for (temp_count = 0; temp_count < 36; temp_count++)
				printf("\n.cfg_board_SI_setting_ps[%d].ac_trace_delay_org[%d]=0x%08x,// %d", ps, temp_count, ddr_set_t_p->cfg_board_SI_setting_ps[ps].ac_trace_delay_org[temp_count], (uint32_t)ddr_set_t_p->cfg_board_SI_setting_ps[ps].ac_trace_delay_org[temp_count]);

			for (temp_count = 0; temp_count < 36; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].ac_trace_delay[%d]=0x%08x,// %d", ps, temp_count, ddr_set_t_p->cfg_ddr_training_delay_ps[ps].ac_trace_delay[temp_count], ddr_set_t_p->cfg_ddr_training_delay_ps[ps].ac_trace_delay[temp_count]);
			for (temp_count = 0; temp_count < 8; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].write_dqs_delay[%d]=0x%08x,// %d", ps, temp_count, ddr_set_t_p->cfg_ddr_training_delay_ps[ps].write_dqs_delay[temp_count], ddr_set_t_p->cfg_ddr_training_delay_ps[ps].write_dqs_delay[temp_count]);
			for (temp_count = 0; temp_count < 72; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].write_dq_bit_delay[%d]=0x%08x,// %d", ps, temp_count, ddr_set_t_p->cfg_ddr_training_delay_ps[ps].write_dq_bit_delay[temp_count], ddr_set_t_p->cfg_ddr_training_delay_ps[ps].write_dq_bit_delay[temp_count]);
			for (temp_count = 0; temp_count < 8; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].read_dqs_gate_delay[%d]=0x%08x,// %d", ps, temp_count, ddr_set_t_p->cfg_ddr_training_delay_ps[ps].read_dqs_gate_delay[temp_count], ddr_set_t_p->cfg_ddr_training_delay_ps[ps].read_dqs_gate_delay[temp_count]);
			for (temp_count = 0; temp_count < 8; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].read_dqs_delay[%d]=0x%08x,// %d", ps, temp_count, ddr_set_t_p->cfg_ddr_training_delay_ps[ps].read_dqs_delay[temp_count], ddr_set_t_p->cfg_ddr_training_delay_ps[ps].read_dqs_delay[temp_count]);
			for (temp_count = 0; temp_count < 72; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].read_dq_bit_delay[%d]=0x%08x,// %d", ps, temp_count, ddr_set_t_p->cfg_ddr_training_delay_ps[ps].read_dq_bit_delay[temp_count], ddr_set_t_p->cfg_ddr_training_delay_ps[ps].read_dq_bit_delay[temp_count]);
			for (temp_count = 0; temp_count < 44; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].soc_bit_vref[%d]=0x%08x,// %d", ps, temp_count, ddr_set_t_p->cfg_ddr_training_delay_ps[ps].soc_bit_vref[temp_count], ddr_set_t_p->cfg_ddr_training_delay_ps[ps].soc_bit_vref[temp_count]);
			for (temp_count = 0; temp_count < 36; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].dram_bit_vref[%d]=0x%08x,// %d", ps, temp_count, ddr_set_t_p->cfg_ddr_training_delay_ps[ps].dram_bit_vref[temp_count], ddr_set_t_p->cfg_ddr_training_delay_ps[ps].dram_bit_vref[temp_count]);
			for (temp_count = 0; temp_count < 16; temp_count++)
				printf("\n.cfg_ddr_training_delay_ps[%d].reserve_training_parameter[%d]=0x%08x,// %d", ps, temp_count, ddr_set_t_p->cfg_ddr_training_delay_ps[ps].reserve_training_parameter[temp_count], ddr_set_t_p->cfg_ddr_training_delay_ps[ps].reserve_training_parameter[temp_count]);
			if (p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_S4) {
				for (temp_count = 0; temp_count < 44; temp_count++)
					printf("\n.cfg_ddr_training_delay_ps[%d].soc_bit_vref_dac1[%d]=0x%08x,// %d", ps, temp_count, ddr_set_t_p->cfg_ddr_training_delay_ps[ps].soc_bit_vref_dac1[temp_count], ddr_set_t_p->cfg_ddr_training_delay_ps[ps].soc_bit_vref_dac1[temp_count]);
			}
		}

		printf("\n},\n");
	}
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
		printf("\nstore rsv write ddr-parameter 0x%08x 0x%08x\n", (uint32_t)(uint64_t)buffer, length);
		sprintf(str, "store rsv write ddr-parameter 0x%08x 0x%08x\n", (uint32_t)(uint64_t)buffer, length);
		run_command(str, 0);
	}

	return 1;
}
#else
static int ddr_do_store_ddr_parameter_ops(uint8_t *buffer, uint32_t length)
{
	extern int store_ddr_parameter_write(uint8_t *buffer, uint32_t length);
	printf("\nstore ddr_parameter write 0x%08x 0x%08x\n", (uint32_t)(uint64_t)buffer, length);
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
	unsigned int enable_ddr_fast_boot = 0;  // 0 pause 1,resume
	char *out_sha2;

	printf("\nargc== 0x%08x\n", argc);
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
	dmc_retraining_ctrl = rd_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address));

	if ((p_ddr_base->ddr_dmc_lpdd4_retraining_address))
		wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl & (~(1 << 31)));
	ddr_set_t *ddr_set_t_p = NULL;
	ddr_set_t_p = (ddr_set_t *)(ddr_set_t_p_arrary);
	uint32_t ddr_set_add = 0;
	uint32_t ddr_set_size = 0;
	out_sha2 = (char *)ddr_sha.sha2;

	ddr_set_t_c2 *ddr_set_t_p_c2 = NULL;
	ddr_set_t_p_c2 = (ddr_set_t_c2 *)(ddr_set_t_p_arrary_c2);

	uint32_t write_size = 0;

	if (((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_C2) &&
	(p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_T5D)) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S4) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5W)) {
		ddr_set_add = (uint32_t)(uint64_t)(ddr_set_t_p_c2);
		ddr_set_size = sizeof(ddr_set_t_c2);
		out_sha2 = (char *)ddr_sha_c2.sha2;
		do_read_c2_ddr_training_data(1, ddr_set_t_p_c2);
		uint32_t loop = 0;
		#if 1
		//for C2 T5/T5D ddr window fast boot no support dfe vref1
		if (((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_C2) &&
		(p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_T5D)) ||
		(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5W)) {
			//sizeof(board_phase_setting_ps_t.soc_bit_vref_dac1);
			ddr_set_size = sizeof(ddr_set_t_c2) - (44 * 2);
			unsigned char *sha_t_chip_id;
			sha_t_chip_id = (unsigned char *)((uint64_t)(&(ddr_sha_c2.sha_chip_id)) - (44 * 2));
			//update chip id
			for (loop = 0; loop < MESON_CPU_CHIP_ID_SIZE; loop++)
				sha_t_chip_id[loop] = global_chip_id[loop];
		}
		#endif

		if (enable_ddr_fast_boot == 1)
			ddr_set_t_p_c2->cfg_board_common_setting.fast_boot[0] = 0xff;

		if (enable_ddr_fast_boot == 2)
			ddr_set_t_p_c2->cfg_board_common_setting.fast_boot[0] = 0;
	}

	//do_read_ddr_training_data(1, ddr_set_t_p);
	if ((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_G12A) && (p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_SC2)) {
		ddr_set_add = (uint32_t)(uint64_t)(ddr_set_t_p);
		ddr_set_size = sizeof(ddr_set_t);
		out_sha2 = (char *)ddr_sha.sha2;
		dwc_ddrphy_apb_wr(0xd0000, 0x0);
		do_read_ddr_training_data(1, ddr_set_t_p);
		char dmc_test_worst_window_rx = 0;
		char dmc_test_worst_window_tx = 0;

		{
			dwc_ddrphy_apb_wr((0 << 20) | (0xd << 16) | (0 << 12) | (0x0), 0); // DWC_DDRPHYA_APBONLY0_MicroContMuxSel

			dmc_test_worst_window_tx = dwc_ddrphy_apb_rd((0 << 20) | (1 << 16) | (0 << 12) | (0x0c2));
			dmc_test_worst_window_rx = dwc_ddrphy_apb_rd((0 << 20) | (1 << 16) | (0 << 12) | (0x0c3));
			if (dmc_test_worst_window_tx > 30)
				dmc_test_worst_window_tx = 30;
			if (dmc_test_worst_window_rx > 30)
				dmc_test_worst_window_rx = 30;
			ddr_set_t_p->fast_boot[1] = (((dmc_test_worst_window_tx / 2) << 4)) | (((dmc_test_worst_window_rx / 2)));
		}
		if (enable_ddr_fast_boot == 1)
			ddr_set_t_p->fast_boot[0] = 0xff;

		if (enable_ddr_fast_boot == 2)
			ddr_set_t_p->fast_boot[0] = 0;
	}
	if (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T7) {
		ddr_set_add = (uint32_t)(uint64_t)(ddr_set_t_p);
		ddr_set_size = sizeof(ddr_set_t);
		out_sha2 = (char *)ddr_sha.sha2;
		dwc_ddrphy_apb_wr(0xd0000, 0x0);
		do_read_ddr_training_data(1, ddr_set_t_p);
		char dmc_test_worst_window_rx = 0;
		char dmc_test_worst_window_tx = 0;

		{
			dwc_ddrphy_apb_wr((0 << 20) | (0xd << 16) | (0 << 12) | (0x0), 0); // DWC_DDRPHYA_APBONLY0_MicroContMuxSel

			dmc_test_worst_window_tx = dwc_ddrphy_apb_rd((0 << 20) | (1 << 16) | (0 << 12) | (0x0c2));
			dmc_test_worst_window_rx = dwc_ddrphy_apb_rd((0 << 20) | (1 << 16) | (0 << 12) | (0x0c3));
			if (dmc_test_worst_window_tx > 30)
				dmc_test_worst_window_tx = 30;
			if (dmc_test_worst_window_rx > 30)
				dmc_test_worst_window_rx = 30;
			ddr_set_t_p->fast_boot[1] = (((dmc_test_worst_window_tx / 2) << 4)) | (((dmc_test_worst_window_rx / 2)));
		}
		if (enable_ddr_fast_boot == 1)
			ddr_set_t_p->fast_boot[0] = 0xff;

		if (enable_ddr_fast_boot == 2)
			ddr_set_t_p->fast_boot[0] = 0;
	}

	write_size = ((ddr_set_size + SHA256_SUM_LEN + MESON_CPU_CHIP_ID_SIZE + 511) / 512) * 512;

	{
		//printf("&ddr_sha.ddrs : 0x%x\n", (uint32_t)(uint64_t)&ddr_sha.ddrs);
		//printf("&ddr_sha.sha2 : 0x%x\n", (uint32_t)(uint64_t)out_sha2);
		printf("ddr_set_add : 0x%x   sizeof(ddr_set_t):0x%x\n ", (uint32_t)(uint64_t)ddr_set_add, (uint32_t)(uint64_t)ddr_set_size);
		printf("ddr_set_add_chip_id : 0x%x\n", (uint32_t)(uint64_t)(ddr_set_add + ddr_set_size));
		sha256_csum_wd_internal((unsigned char *)(uint64_t)ddr_set_add, ddr_set_size, (unsigned char *)out_sha2, 0);
		printf("print sha\n");
		ddr_do_store_ddr_parameter_ops((uint8_t *)(unsigned long)(ddr_set_add - SHA256_SUM_LEN), write_size);
	}
	return 1;
}
#endif

int do_ddr_set_watchdog_value(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char *endp;
	int i = 0;

	printf("\nargc== 0x%08x\n", argc);
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

#define  G12_DATA_READ_OFFSET_MAX   (0X3F)
#define  G12_DATA_WRITE_OFFSET_MAX   (0X3F + 7 * 32)

#define DMC_TEST_WINDOW_INDEX_ATXDLY 1
#define DMC_TEST_WINDOW_INDEX_TXDQSDLY 2
#define DMC_TEST_WINDOW_INDEX_RXCLKDLY  3
#define DMC_TEST_WINDOW_INDEX_TXDQDLY  4
#define DMC_TEST_WINDOW_INDEX_RXPBDLY  5
#define DMC_TEST_WINDOW_INDEX_RXENDLY  6

#define DMC_TEST_WINDOW_INDEX_EE_VOLTAGE  0x11
#define DMC_TEST_WINDOW_INDEX_SOC_VREF  0x12
#define DMC_TEST_WINDOW_INDEX_DRAM_VREF    0x14

typedef struct training_delay_information {
	uint16_t	ac_delay[10];
	uint16_t	txdqs_delay[16];
	uint16_t	rxdqs_delay[16];
	uint16_t	txdq_delay[72];
	uint16_t	rxdq_delay[72];
	uint16_t	gate_rxdq_delay[72];
} training_delay_t;
training_delay_t training_delay_t_p;

uint16_t lcd_bdl_value[72][4];          //org min max status
#define LCD_BDLR_MIN  0
#define LCD_BDLR_MAX   1
#define LCD_BDLR_STATUS  2

//BYTE0-3
#define  TEST_ARG_0_DMC_STICKY_MAGIC  0
#define  TEST_ARG_1_CMD0  1
#define  TEST_ARG_2_STEP  2           // 0 init   1 test ac  2 test tdqs_write
#define  TEST_ARG_3_ALL_TOGHTER  3

//BYTE4-7
#define  TEST_ARG_FREQ_NIBBLE_L  4
#define  TEST_ARG_FREQ_NIBBLE_H  5

//BYTE8-11
#define  TEST_ARG_BOOT_TIMES_L 6
#define  TEST_ARG_BOOT_TIMES_H 7

//BYTE12-15
#define  TEST_ARG_ERROR_FLAG 8           //take 4 byte for kernel test flag

//BYTE16-19
#define  TEST_ARG_CS0_TEST_START_INDEX  12
#define  TEST_ARG_CS0_TEST_SIZE_INDEX  16
#define  TEST_ARG_CS1_TEST_START_INDEX  20
#define  TEST_ARG_CS1_TEST_SIZE_INDEX  24

#define  TEST_ARG_WATCHDOG_TIME_SIZE_INDEX  28
#define  TEST_ARG_TEST_INDEX_ENALBE_INDEX  30

#define  TEST_ARG_ERROR_FLAG_NULL 0
#define  TEST_ARG_ERROR_FLAG_FAIL 1
#define  TEST_ARG_ERROR_FLAG_PASS 2

#define  TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE  32   // 32BYTE
#define  TEST_ARG_NIBBLE_WIDTH_BYTE  3          //3///BYTE

int do_ddr_test_dqs_window_sticky(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	printf("\nEnterddr_test_dqs_window function  ddr_test_cmd 0x27 0x1080000 0x800000  0x40000000 0x800000 15 0x6  0 0 0 0 0 0 1/config\n");
	printf("\nddr_test_cmd 0x27 cs0_test_start  cs0_test_size  cs1_test_start  cs1_test_size  ns test_index_enable  nibble_mask0 nibble_mask1 nibble_mask2 dram_type channel_mode  config_register all_together--- watchdog should >15s\n");
	printf("\n ac write_dqs read_dqs can test together test_index_enable can enable kernel test \n");
#define  DDR_CORSS_TALK_TEST_SIZE   0x20000
	unsigned int temp_test_error = 0;
	unsigned int nibble_save_offset = 0;
	unsigned int nibble_step = 0;
	unsigned int nibble_max = 16;
	unsigned int test_index_enable = 0;
	unsigned int test_index = 0;
	unsigned int test_index_max = 6;
	unsigned int reg_value = 0;
	unsigned int dram_type = 0;
	unsigned int channel_mode = 0;
	unsigned int kernel_watchdog_s = 20; //240;
	unsigned int config_register = 0;
	unsigned int all_toghter_enable = 0;
	unsigned int error_flag_reg_add = 0;
	char *string_print_flag = " uboot-window-loop \n";
	char *endp;
	char *buf;
	buf = "";

	unsigned int cs0_test_start = 0x1080000;
	unsigned int cs0_test_size = DDR_CORSS_TALK_TEST_SIZE;
	unsigned int cs1_test_start = 0;
	unsigned int cs1_test_size = 0;
	unsigned int enable_kernel_test = 0;
	if (argc > 1) {
		cs0_test_start = simple_strtoull_ddr(argv[1], &endp, 16);
		if (*argv[1] == 0 || *endp != 0)
			cs0_test_start = 0x1080000;
	}

	if (argc > 2) {
		cs0_test_size = simple_strtoull_ddr(argv[2], &endp, 16);
		if (*argv[2] == 0 || *endp != 0)
			cs0_test_size = DDR_CORSS_TALK_TEST_SIZE;
	}

	if (argc > 3) {
		cs1_test_start = simple_strtoull_ddr(argv[3], &endp, 16);
		if (*argv[3] == 0 || *endp != 0)
			cs1_test_start = 0;
	}
	if (argc > 4) {
		cs1_test_size = simple_strtoull_ddr(argv[4], &endp, 16);
		if (*argv[4] == 0 || *endp != 0)
			cs1_test_size = 0;
	}

	unsigned int ddr_test_size = DDR_CORSS_TALK_TEST_SIZE;
	ddr_test_size = cs0_test_size;
	if (argc > 5) {
		watchdog_time_s = simple_strtoull_ddr(argv[5], &endp, 0);
		if (*argv[5] == 0 || *endp != 0)
			watchdog_time_s = 20;
	}
	printf("watchdog_time_s==%d\n", watchdog_time_s);

	if (argc > 6) {
		test_index_enable = simple_strtoull_ddr(argv[6], &endp, 0);
		if (*argv[5] == 0 || *endp != 0)
			test_index_enable = 0;
	}
	printf("test_index_enable==0x%08x\n", test_index_enable);
	enable_kernel_test = (test_index_enable >> 7) & 1;
	if (enable_kernel_test)
		printf("enable kernel window test\n");

	unsigned int nibble_mask[3] = { 0, 0, 0 };
	if (argc > 7) {
		nibble_mask[0] = simple_strtoull_ddr(argv[7], &endp, 0);
		if (*argv[7] == 0 || *endp != 0)
			nibble_mask[0] = 0;
	}
	printf("nibble_mask[0]==0x%08x\n", nibble_mask[0]);
	if (argc > 8) {
		nibble_mask[1] = simple_strtoull_ddr(argv[8], &endp, 0);
		if (*argv[8] == 0 || *endp != 0)
			nibble_mask[1] = 0;
	}
	printf("nibble_mask[1]==0x%08x\n", nibble_mask[1]);
	if (argc > 9) {
		nibble_mask[2] = simple_strtoull_ddr(argv[9], &endp, 0);
		if (*argv[9] == 0 || *endp != 0)
			nibble_mask[2] = 0;
	}
	printf("nibble_mask[2]==0x%08x\n", nibble_mask[2]);

	if (argc > 10) {
		dram_type = simple_strtoull_ddr(argv[10], &endp, 0);
		if (*argv[10] == 0 || *endp != 0)
			dram_type = 0;
	}
	if (argc > 11) {
		channel_mode = simple_strtoull_ddr(argv[11], &endp, 0);
		if (*argv[11] == 0 || *endp != 0)
			channel_mode = 0;
	}
	if (argc > 12) {
		config_register = simple_strtoull_ddr(argv[12], &endp, 0);
		if (*argv[12] == 0 || *endp != 0)
			config_register = 0;
	}
	if (argc > 13) {
		all_toghter_enable = simple_strtoull_ddr(argv[13], &endp, 0);
		if (*argv[13] == 0 || *endp != 0)
			all_toghter_enable = 0;
	}
	printf("all_toghter_enable==0x%08x\n", all_toghter_enable);

	if (argc > 14) {
		error_flag_reg_add = simple_strtoull_ddr(argv[14], &endp, 0);
		if (*argv[14] == 0 || *endp != 0)
			error_flag_reg_add = 0;
	}
	printf("error_flag_reg_add==0x%08x\n", error_flag_reg_add);
	printf("\ntest use uboot sticky register\n");

	char str[1024] = "";
	volatile uint16_t num_arry[256] = { 0 }; // NULL;
	int i;

	sticky_reg_base_add = (((p_ddr_base->ddr_dmc_sticky0)) & 0xffff);
	for (i = 0; i < 64 * 4; i++) {
		num_arry[i] = ddr_rd_8_16bit_on_32reg(sticky_reg_base_add, 8, i);
		if ((i == 0) || (i == 32) || (i == (32 + 10 * 3)) || (i == (32 + 10 * 3 + 16 * 3)) || (i == (32 + 10 * 3 + 16 * 3 + 16 * 3)))
			printf("\n numarry[%d]", i);
		printf(" %d ", num_arry[i]);
	}

	uint16_t test_left_max_init_value = 32;
	uint16_t test_right_max_init_value = 32;
	uint16_t test_boot_times = 0;
	uint16_t test_ddr_frequency = 0;

	printf("\nTEST_ARG_0_DMC_STICKY_MAGIC==0x%08x\n", num_arry[TEST_ARG_0_DMC_STICKY_MAGIC]);
	printf("\nTEST_ARG_1_CMD0==0x%08x\n", num_arry[TEST_ARG_1_CMD0]);
	printf("TEST_ARG_2_STEP==0x%08x\n", num_arry[TEST_ARG_2_STEP]);
	printf("TEST_ARG_3_ALL_TOGHTER==0x%08x\n", num_arry[TEST_ARG_3_ALL_TOGHTER]);
	printf("TEST_ARG_FREQ_NIBBLE_L==0x%08x\n", num_arry[TEST_ARG_FREQ_NIBBLE_L]);
	printf("TEST_ARG_FREQ_NIBBLE_H==0x%08x\n", num_arry[TEST_ARG_FREQ_NIBBLE_H]);
	printf("TEST_ARG_BOOT_TIMES_L==0x%08x\n", num_arry[TEST_ARG_BOOT_TIMES_L]);
	printf("TEST_ARG_BOOT_TIMES_H==0x%08x\n", num_arry[TEST_ARG_BOOT_TIMES_H]);
	printf("TEST_ARG_ERROR_FLAG==0x%08x\n", num_arry[TEST_ARG_ERROR_FLAG]);

	printf("TEST_ARG_FREQ==%dM\n", (num_arry[TEST_ARG_FREQ_NIBBLE_H] << 8) | (num_arry[TEST_ARG_FREQ_NIBBLE_L] << 0));
	printf("TEST_ARG_BOOT_TIMES==%d\n", (num_arry[TEST_ARG_BOOT_TIMES_H] << 8) | (num_arry[TEST_ARG_BOOT_TIMES_L] << 0));
	test_boot_times = (num_arry[TEST_ARG_BOOT_TIMES_H] << 8) | (num_arry[TEST_ARG_BOOT_TIMES_L] << 0);
	test_ddr_frequency = (num_arry[TEST_ARG_FREQ_NIBBLE_H] << 8) | (num_arry[TEST_ARG_FREQ_NIBBLE_L] << 0);

	if ((num_arry[TEST_ARG_0_DMC_STICKY_MAGIC] == (DMC_STICKY_UBOOT_WINDOW_MAGIC_1 & 0xff))
	    && (num_arry[TEST_ARG_1_CMD0] == (DMC_STICKY_UBOOT_WINDOW_MAGIC_1 & 0xff))) {  //for check magic number make sume enter test command
		test_boot_times++;
		num_arry[TEST_ARG_BOOT_TIMES_L] = test_boot_times & 0xff;
		num_arry[TEST_ARG_BOOT_TIMES_H] = (test_boot_times >> 8) & 0xff;
		ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_BOOT_TIMES_L, num_arry[TEST_ARG_BOOT_TIMES_L]);
		ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_BOOT_TIMES_H, num_arry[TEST_ARG_BOOT_TIMES_H]);

		if (test_ddr_frequency != global_ddr_clk) {         //
			printf("running ddr freq==%d,but test freq is%d,will reboot use d2pll \n", global_ddr_clk, test_ddr_frequency);
			sprintf(str, "d2pll  %d", test_ddr_frequency);
			printf("\nstr=%s\n", str);
			run_command(str, 0);

			while (1) {
			}
		}
	} else {
		test_boot_times = 0;
		num_arry[TEST_ARG_BOOT_TIMES_L] = test_boot_times & 0xff;
		num_arry[TEST_ARG_BOOT_TIMES_H] = (test_boot_times >> 8) & 0xff;
		ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_BOOT_TIMES_L, num_arry[TEST_ARG_BOOT_TIMES_L]);
		ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_BOOT_TIMES_H, num_arry[TEST_ARG_BOOT_TIMES_H]);
		num_arry[TEST_ARG_2_STEP] = 0;
		ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_2_STEP, num_arry[TEST_ARG_2_STEP]);
	}
	printf("test_sticky is not magic nummber,boot times==%d\n", test_boot_times);

	{
		wr_reg((sticky_reg_base_add + TEST_ARG_CS0_TEST_START_INDEX), cs0_test_start);
		wr_reg((sticky_reg_base_add + TEST_ARG_CS0_TEST_SIZE_INDEX), cs0_test_size);
		wr_reg((sticky_reg_base_add + TEST_ARG_CS1_TEST_START_INDEX), cs1_test_start);
		wr_reg((sticky_reg_base_add + TEST_ARG_CS1_TEST_SIZE_INDEX), cs1_test_size);
		{
			num_arry[TEST_ARG_WATCHDOG_TIME_SIZE_INDEX] = watchdog_time_s & 0xff;
			num_arry[TEST_ARG_WATCHDOG_TIME_SIZE_INDEX + 1] = (watchdog_time_s >> 8) & 0xff;
			ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_WATCHDOG_TIME_SIZE_INDEX, num_arry[TEST_ARG_WATCHDOG_TIME_SIZE_INDEX]);
			ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, (TEST_ARG_WATCHDOG_TIME_SIZE_INDEX + 1), num_arry[TEST_ARG_WATCHDOG_TIME_SIZE_INDEX + 1]);
		}
		num_arry[TEST_ARG_TEST_INDEX_ENALBE_INDEX] = test_index_enable;
		ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_TEST_INDEX_ENALBE_INDEX, num_arry[TEST_ARG_TEST_INDEX_ENALBE_INDEX]);
		if (config_register == 1) {
			num_arry[TEST_ARG_2_STEP] = 0;
			ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_2_STEP, num_arry[TEST_ARG_2_STEP]);
		}
		num_arry[TEST_ARG_3_ALL_TOGHTER] = all_toghter_enable;
		ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_3_ALL_TOGHTER, num_arry[TEST_ARG_3_ALL_TOGHTER]);
	}

	if ((num_arry[TEST_ARG_2_STEP]) == 0) {
		{
			num_arry[TEST_ARG_0_DMC_STICKY_MAGIC] = DMC_STICKY_UBOOT_WINDOW_MAGIC_1;
			ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_0_DMC_STICKY_MAGIC, num_arry[TEST_ARG_0_DMC_STICKY_MAGIC]);
			num_arry[TEST_ARG_1_CMD0] = DMC_STICKY_UBOOT_WINDOW_MAGIC_1;
			ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_1_CMD0, num_arry[TEST_ARG_1_CMD0]);
			num_arry[TEST_ARG_2_STEP] = 1;
			ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_2_STEP, num_arry[TEST_ARG_2_STEP]);
			{
				test_boot_times = 0;
				num_arry[TEST_ARG_BOOT_TIMES_L] = test_boot_times & 0xff;
				num_arry[TEST_ARG_BOOT_TIMES_H] = (test_boot_times >> 8) & 0xff;
				ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_BOOT_TIMES_L, num_arry[TEST_ARG_BOOT_TIMES_L]);
				ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_BOOT_TIMES_H, num_arry[TEST_ARG_BOOT_TIMES_H]);
			}
			{
				test_ddr_frequency = global_ddr_clk;
				num_arry[TEST_ARG_FREQ_NIBBLE_L] = test_ddr_frequency & 0xff;
				num_arry[TEST_ARG_FREQ_NIBBLE_H] = (test_ddr_frequency >> 8) & 0xff;
				ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_FREQ_NIBBLE_L, num_arry[TEST_ARG_FREQ_NIBBLE_L]);
				ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_FREQ_NIBBLE_H, num_arry[TEST_ARG_FREQ_NIBBLE_H]);
			}

			num_arry[TEST_ARG_ERROR_FLAG] = TEST_ARG_ERROR_FLAG_NULL;
			ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_ERROR_FLAG, num_arry[TEST_ARG_ERROR_FLAG]);
		}


		for (nibble_step = 0; nibble_step < 72; nibble_step++) {
			{
				test_left_max_init_value = 16;
				test_right_max_init_value = 16;
				if (nibble_step < 10) {
					test_left_max_init_value = 32;
					test_right_max_init_value = 32;
				}
				num_arry[TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + (nibble_step * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN] = test_left_max_init_value;
				num_arry[TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + (nibble_step * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX] = test_right_max_init_value;
				num_arry[TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + (nibble_step * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS] = 0; //0
			}

			{
				if (nibble_step < 32) {
					if (((nibble_mask[0]) >> nibble_step) & 1)
						num_arry[TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + nibble_step * TEST_ARG_NIBBLE_WIDTH_BYTE + LCD_BDLR_STATUS] = 4;
				} else if (nibble_step < 64) {
					if (((nibble_mask[1]) >> (nibble_step - 32)) & 1)
						num_arry[TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + nibble_step * TEST_ARG_NIBBLE_WIDTH_BYTE + LCD_BDLR_STATUS] = 4;
				} else if (nibble_step < 96) {
					if (((nibble_mask[2]) >> (nibble_step - 64)) & 1)
						num_arry[TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + nibble_step * TEST_ARG_NIBBLE_WIDTH_BYTE + LCD_BDLR_STATUS] = 4;
				}
				if (all_toghter_enable) {
					if ((nibble_step == 0) || (nibble_step == 10) || (nibble_step == (10 + 16)))
						num_arry[TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + nibble_step * TEST_ARG_NIBBLE_WIDTH_BYTE + LCD_BDLR_STATUS] = 0;
					else
						num_arry[TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + nibble_step * TEST_ARG_NIBBLE_WIDTH_BYTE + LCD_BDLR_STATUS] = 4;
				}
			}
		}

		{
			for (i = TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE; i < 72 * TEST_ARG_NIBBLE_WIDTH_BYTE; i++)
				ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, i, num_arry[i]);

			num_arry[TEST_ARG_2_STEP] = 1;
			ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_2_STEP, num_arry[TEST_ARG_2_STEP]);
		}
	}

	if (all_toghter_enable) {
		for (nibble_step = 0; nibble_step < 72; nibble_step++) {
			if ((nibble_step == 0) || (nibble_step == 10) || (nibble_step == (10 + 16))) {
			} else {
				num_arry[TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + nibble_step * TEST_ARG_NIBBLE_WIDTH_BYTE + LCD_BDLR_STATUS] = 4;
			}
		}
	}

	if (config_register == 1) {
		num_arry[TEST_ARG_2_STEP] = 0;
		ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_2_STEP, num_arry[TEST_ARG_2_STEP]);
	}

	if ((num_arry[TEST_ARG_2_STEP])) {
		for (test_index = num_arry[TEST_ARG_2_STEP]; test_index < test_index_max; test_index++) {
			printf("\ntest_index=%d\n", test_index);
			if ((((test_index_enable) >> (test_index - 1)) & 1) == 0) {
				num_arry[TEST_ARG_2_STEP] = ((num_arry[TEST_ARG_2_STEP]) + 1); //why can not use ++
				ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_2_STEP, num_arry[TEST_ARG_2_STEP]);
				continue;
			}
			{
				if (test_index == DMC_TEST_WINDOW_INDEX_ATXDLY) {
					nibble_save_offset = 0;
					nibble_max = 10;
					nibble_mask[0] = 0x30;
					if ((dram_type == CONFIG_DDR_TYPE_LPDDR3))
						nibble_mask[0] = 0x3e3;
					if ((dram_type == CONFIG_DDR_TYPE_LPDDR4)) {
						nibble_mask[0] = 0x273;
						if ((channel_mode == CONFIG_DDR0_32BIT_RANK01_CH0))
							nibble_mask[0] = 0x3f3;
					}
					test_left_max_init_value = 64;
					test_right_max_init_value = 64;
				}
				if (test_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY) {
					nibble_save_offset = 10;
					nibble_max = 16;
					if ((cs1_test_size == 0))
						nibble_mask[0] = 0xff00;
					test_left_max_init_value = 16;
					test_right_max_init_value = 16;
				}
				if (test_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY) {
					nibble_save_offset = (10) + (16);
					if ((cs1_test_size == 0))
						nibble_mask[0] = 0xff00;
					nibble_max = 16;
					test_left_max_init_value = 16;
					test_right_max_init_value = 16;
				}
				if (test_index == DMC_TEST_WINDOW_INDEX_TXDQDLY) {
					nibble_save_offset = 0;
					nibble_max = 72;
					if ((cs1_test_size == 0)) {
						nibble_mask[1] = 0xfffffff0;
						nibble_mask[2] = 0xffffffff;
					}
					test_left_max_init_value = 16;
					test_right_max_init_value = 16;
				}
				if (test_index == DMC_TEST_WINDOW_INDEX_RXPBDLY) {
					nibble_save_offset = 0;
					nibble_max = 72;
					if ((cs1_test_size == 0)) {
						nibble_mask[1] = 0xfffffff0;
						nibble_mask[2] = 0xffffffff;
					}

					test_left_max_init_value = 64;
					test_right_max_init_value = 64;
				}
				for ((nibble_step = 0); (nibble_step < nibble_max); (nibble_step++)) {
					if (nibble_step < 32) {
						if (((nibble_mask[0]) >> nibble_step) & 1)
							num_arry[TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + (nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE + LCD_BDLR_STATUS] = 4;
					} else if (nibble_step < 64) {
						if (((nibble_mask[1]) >> (nibble_step - 32)) & 1)
							num_arry[TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + (nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE + LCD_BDLR_STATUS] = 4;
					} else if (nibble_step < 96) {
						if (((nibble_mask[2]) >> (nibble_step - 64)) & 1)
							num_arry[TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + (nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE + LCD_BDLR_STATUS] = 4;
					}
					ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
								(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS),
								num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)]);
				}
			}

			{
				for ((nibble_step = 0); (nibble_step < nibble_max); (nibble_step++)) {
					if (num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] == 4)
						continue;
					printf("nibble_step ==%d\n", nibble_step);

					if (test_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY) {
						if (nibble_step % 2) {
							ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
										(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN),
										num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step - 1 + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN)]);
							ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
										(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX),
										num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step - 1 + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX)]);
							ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
										(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS),
										num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step - 1 + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)]);
							continue;
						}
					}
					test_start_addr = cs0_test_start;
					ddr_test_size = cs0_test_size;
					if (test_index == DMC_TEST_WINDOW_INDEX_ATXDLY) {
						test_start_addr = cs0_test_start;
						ddr_test_size = cs0_test_size;
					}

					if (test_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY) {
						if (nibble_step > 7) {
							test_start_addr = cs1_test_start;
							ddr_test_size = cs1_test_size;
						}
					}
					if (test_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY) {
						if (nibble_step > 7) {
							test_start_addr = cs1_test_start;
							ddr_test_size = cs1_test_size;
						}
					}
					if (test_index == DMC_TEST_WINDOW_INDEX_TXDQDLY) {
						if (nibble_step > 35) {
							test_start_addr = cs1_test_start;
							ddr_test_size = cs1_test_size;
						}
					}
					if (test_index == DMC_TEST_WINDOW_INDEX_RXPBDLY) {
						if (nibble_step > 35) {
							test_start_addr = cs1_test_start;
							ddr_test_size = cs1_test_size;
						}
					}
					{
						if (nibble_step < 32) {
							nibble_mask[0] = ((0xffffffff) & (~(1 << nibble_step)));
							nibble_mask[1] = ((0xffffffff));
							nibble_mask[2] = ((0xffffffff));
						} else if (nibble_step < 64) {
							nibble_mask[0] = ((0xffffffff));
							nibble_mask[1] = ((0xffffffff) & (~(1 << (nibble_step - 32))));
							nibble_mask[2] = ((0xffffffff));
						} else if (nibble_step < 96) {
							nibble_mask[0] = ((0xffffffff));
							nibble_mask[1] = ((0xffffffff));
							nibble_mask[2] = ((0xffffffff) & (~(1 << (nibble_step - 64))));
						}
					}
					if (all_toghter_enable) {
						if (test_index == DMC_TEST_WINDOW_INDEX_ATXDLY) {
							nibble_save_offset = 0;
							nibble_max = 10;
							nibble_mask[0] = 0x30;
							nibble_mask[1] = 0;
							nibble_mask[2] = 0;
							if ((dram_type == CONFIG_DDR_TYPE_LPDDR3))
								nibble_mask[0] = 0x3e3;
							if ((dram_type == CONFIG_DDR_TYPE_LPDDR4)) {
								nibble_mask[0] = 0x273;
								if ((channel_mode == CONFIG_DDR0_32BIT_RANK01_CH0))
									nibble_mask[0] = 0x3f3;
							}
						} else {
							nibble_mask[0] = 0;
							nibble_mask[1] = 0;
							nibble_mask[2] = 0;
						}
					}

					ddr_test_watchdog_enable(watchdog_time_s); //s
					printf("\nenable %ds watchdog \n", watchdog_time_s);
					if ((num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] == 0xffff)
					    || (num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] == 0)
					    || (num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] == 1)) {
						printf("\nnibble_step  ==%d ", nibble_step);
						if ((num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] == 0xffff)
						    || (num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] == 0)) {
							num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] = 1;
							ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
										(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS),
										num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)]);

							if (num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN)] == 0) {
								num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] = 2;
								ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
											(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS),
											num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)]);
								run_command("reset", 0);
							}
							{
								num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN)] =
									num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN)] - 1;
								ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
											(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN),
											num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN)]);
							}

							{
								num_arry[TEST_ARG_ERROR_FLAG] = TEST_ARG_ERROR_FLAG_FAIL;
								ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
											TEST_ARG_ERROR_FLAG,
											num_arry[TEST_ARG_ERROR_FLAG]);
							}

							sprintf(str, "ddr_g12_offset_data  %d  0x%08x 0x%08x  0x%08x  %d %d", test_index, nibble_mask[0], nibble_mask[1], nibble_mask[2], DDR_PARAMETER_LEFT,
								num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN)]);
							printf("\nstr=%s\n", str);
							ddr_test_watchdog_clear();
							run_command(str, 0);

							temp_test_error = ddr_test_s_cross_talk_pattern(ddr_test_size);
							if (all_toghter_enable && cs1_test_size) {
								test_start_addr = cs1_test_start;
								ddr_test_size = cs1_test_size;
								temp_test_error = temp_test_error + ddr_test_s_cross_talk_pattern(ddr_test_size);
								test_start_addr = cs0_test_start;
								ddr_test_size = cs0_test_size;
							}
							if (temp_test_error) {
								run_command("reset", 0);
							} else {
								if (!enable_kernel_test) {
									num_arry[TEST_ARG_ERROR_FLAG] = TEST_ARG_ERROR_FLAG_PASS;
									ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
												TEST_ARG_ERROR_FLAG,
												num_arry[TEST_ARG_ERROR_FLAG]);
									run_command("reset", 0);
								} else {
									ddr_test_watchdog_enable(kernel_watchdog_s); //s
									printf("\nenable %ds watchdog \n", kernel_watchdog_s);
									run_command("run storeboot", 0);
								}
							}
						} else if (num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] == 1) { //go on find left edge
							if (num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN)] == 0) {
								num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] = 2;
								ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
											(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS),
											num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)]);
								run_command("reset", 0);
							}

							if ((num_arry[TEST_ARG_ERROR_FLAG]) == TEST_ARG_ERROR_FLAG_PASS) {
								{
									num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] = 2;
									ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
												(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS),
												num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)]);
								}
								{
									num_arry[TEST_ARG_ERROR_FLAG] = TEST_ARG_ERROR_FLAG_NULL;
									ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
												TEST_ARG_ERROR_FLAG,
												num_arry[TEST_ARG_ERROR_FLAG]);
								}
								run_command("reset", 0);
							}
							if ((num_arry[TEST_ARG_ERROR_FLAG]) == TEST_ARG_ERROR_FLAG_FAIL) {
								{
									num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN)] =
										num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN)] - 1;
									ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
												(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN),
												num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN)]);
								}
								{
									num_arry[TEST_ARG_ERROR_FLAG] = TEST_ARG_ERROR_FLAG_FAIL;
									ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
												TEST_ARG_ERROR_FLAG,
												num_arry[TEST_ARG_ERROR_FLAG]);
								}
								sprintf(buf, "0x%08x", (num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN)]));
								printf("%s", buf);
								sprintf(str, "ddr_g12_offset_data  %d  0x%08x 0x%08x  0x%08x  %d %d", test_index, nibble_mask[0], nibble_mask[1], nibble_mask[2], DDR_PARAMETER_LEFT,
									(num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN)]));
								printf("\nstr=%s\n", str);
								ddr_test_watchdog_clear();
								run_command(str, 0);
								temp_test_error = ddr_test_s_cross_talk_pattern(ddr_test_size);
								if (all_toghter_enable && cs1_test_size) {
									test_start_addr = cs1_test_start;
									ddr_test_size = cs1_test_size;
									temp_test_error = temp_test_error + ddr_test_s_cross_talk_pattern(ddr_test_size);
									test_start_addr = cs0_test_start;
									ddr_test_size = cs0_test_size;
								}
								if (temp_test_error) {
									run_command("reset", 0);
								} else {
									if (!enable_kernel_test) {
										num_arry[TEST_ARG_ERROR_FLAG] = TEST_ARG_ERROR_FLAG_PASS;
										ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
													TEST_ARG_ERROR_FLAG,
													num_arry[TEST_ARG_ERROR_FLAG]);
										run_command("reset", 0);
									} else {
										ddr_test_watchdog_enable(kernel_watchdog_s); //s
										printf("\nenable %ds watchdog \n", kernel_watchdog_s);
										run_command("run storeboot", 0);
									}
								}
							}
						}
					}

					if ((num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] == 2)
					    || (num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] == 3)) {
						printf("\nnibble_step  ==%d ", nibble_step);

						if ((num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] == 2)
						    || (num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] == 2)) {
							num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] = 3;
							ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
										(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS),
										num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)]);

							if (num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX)] == 0) {
								num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] = 4;
								ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
											(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS),
											num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)]);
								run_command("reset", 0);
							}
							{
								num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX)] =
									num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX)] - 1;
								ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
											(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX),
											num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX)]);
							}

							{
								num_arry[TEST_ARG_ERROR_FLAG] = TEST_ARG_ERROR_FLAG_FAIL;
								ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
											TEST_ARG_ERROR_FLAG,
											num_arry[TEST_ARG_ERROR_FLAG]);
							}
							sprintf(str, "ddr_g12_offset_data  %d  0x%08x 0x%08x  0x%08x  %d %d", test_index, nibble_mask[0], nibble_mask[1], nibble_mask[2], DDR_PARAMETER_RIGHT,
								num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX)]);
							printf("\nstr=%s\n", str);
							ddr_test_watchdog_clear();
							run_command(str, 0);
							temp_test_error = ddr_test_s_cross_talk_pattern(ddr_test_size);
							if (all_toghter_enable && cs1_test_size) {
								test_start_addr = cs1_test_start;
								ddr_test_size = cs1_test_size;
								temp_test_error = temp_test_error + ddr_test_s_cross_talk_pattern(ddr_test_size);
								test_start_addr = cs0_test_start;
								ddr_test_size = cs0_test_size;
							}
							if (temp_test_error) {
								run_command("reset", 0);
							} else {
								if (!enable_kernel_test) {
									num_arry[TEST_ARG_ERROR_FLAG] = TEST_ARG_ERROR_FLAG_PASS;
									ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
												TEST_ARG_ERROR_FLAG,
												num_arry[TEST_ARG_ERROR_FLAG]);
									run_command("reset", 0);
								} else {
									ddr_test_watchdog_enable(kernel_watchdog_s); //s
									printf("\nenable %ds watchdog \n", kernel_watchdog_s);
									run_command("run storeboot", 0);
								}
							}
						} else if (num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] == 3) { //go on find left edge
							if (num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX)] == 0) {
								num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] = 4;
								ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
											(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS),
											num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)]);
								run_command("reset", 0);
							}

							if ((num_arry[TEST_ARG_ERROR_FLAG]) == TEST_ARG_ERROR_FLAG_PASS) {
								{
									num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)] = 4;
									ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
												(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS),
												num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_STATUS)]);
								}
								{
									num_arry[TEST_ARG_ERROR_FLAG] = TEST_ARG_ERROR_FLAG_NULL;
									ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
												TEST_ARG_ERROR_FLAG,
												num_arry[TEST_ARG_ERROR_FLAG]);
								}
								run_command("reset", 0);
							}
							if ((num_arry[TEST_ARG_ERROR_FLAG]) == TEST_ARG_ERROR_FLAG_FAIL) {
								{
									num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX)] =
										num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX)] - 1;
									ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
												(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX),
												num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX)]);
								}
								{
									num_arry[TEST_ARG_ERROR_FLAG] = TEST_ARG_ERROR_FLAG_FAIL;
									ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
												TEST_ARG_ERROR_FLAG,
												num_arry[TEST_ARG_ERROR_FLAG]);
								}
								sprintf(buf, "0x%08x", (num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX)]));
								printf("%s", buf);
								sprintf(str, "ddr_g12_offset_data  %d  0x%08x 0x%08x  0x%08x  %d %d", test_index, nibble_mask[0], nibble_mask[1], nibble_mask[2], DDR_PARAMETER_RIGHT,
									(num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX)]));
								printf("\nstr=%s\n", str);
								ddr_test_watchdog_clear();
								run_command(str, 0);
								temp_test_error = ddr_test_s_cross_talk_pattern(ddr_test_size);
								if (all_toghter_enable && cs1_test_size) {
									test_start_addr = cs1_test_start;
									ddr_test_size = cs1_test_size;
									temp_test_error = temp_test_error + ddr_test_s_cross_talk_pattern(ddr_test_size);
									test_start_addr = cs0_test_start;
									ddr_test_size = cs0_test_size;
								}
								if (temp_test_error) {
									run_command("reset", 0);
								} else {
									if (!enable_kernel_test) {
										num_arry[TEST_ARG_ERROR_FLAG] = TEST_ARG_ERROR_FLAG_PASS;
										ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8,
													TEST_ARG_ERROR_FLAG,
													num_arry[TEST_ARG_ERROR_FLAG]);
										run_command("reset", 0);
									} else {
										ddr_test_watchdog_enable(kernel_watchdog_s); //s
										printf("\nenable %ds watchdog \n", kernel_watchdog_s);
										run_command("run storeboot", 0);
									}
								}
							}
						}
					}

					ddr_test_watchdog_disable(); //s
					{
						printf("close  watchdog\n");
					}
				}

				printf("11num_arry[TEST_ARG_2_STEP]==%d\n", num_arry[TEST_ARG_2_STEP]);
				num_arry[TEST_ARG_2_STEP] = (num_arry[TEST_ARG_2_STEP]) + 1;
				ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_2_STEP, num_arry[TEST_ARG_2_STEP]);
				printf("22num_arry[TEST_ARG_2_STEP]==%d\n", num_arry[TEST_ARG_2_STEP]);

				ddr_test_watchdog_disable(); //s
				printf("close  watchdog\n");
				{
					unsigned int ui_1_32_100step = 0;
					unsigned int bdlr_100step = 0;
					ui_1_32_100step = (1000000 * 100 / (global_ddr_clk * 2 * 32));
					bdlr_100step = get_bdlr_100step(global_ddr_clk);

					printf("\nacmdlr=0x%08x-->dec %d,ddr clk==%d,bdlr_100step=%d ps,ui_1_32_100step=%d ps,\n", 0, 0, global_ddr_clk,
					       bdlr_100step, ui_1_32_100step);

					printf("\n test result index==");
					printf("%08d", test_index);

					if (test_index == DMC_TEST_WINDOW_INDEX_ATXDLY) {
						printf(" ac window:");
						printf(" step_size ps==");
						printf("%08d", ui_1_32_100step);
						printf("/100 ps ");
					}
					if (test_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY) {
						printf(" txdqs window:");
						printf(" step_size ps==");
						printf("%08d", ui_1_32_100step);
						printf("/100 ps ");
					}
					if (test_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY) {
						printf(" rx_clk_window:");
						printf(" step_size ps==");
						printf("%08d", ui_1_32_100step);
						printf("/100 ps ");
					}
					if (test_index == DMC_TEST_WINDOW_INDEX_TXDQDLY) {
						printf(" tx_bit_dq_window:");
						printf(" step_size ps==");
						printf("%08d", ui_1_32_100step);
						printf("/100 ps ");
					}
					if (test_index == DMC_TEST_WINDOW_INDEX_RXPBDLY) {
						printf(" rx_bit_dq_window");
						printf(" step_size ps==");
						printf("%08d", bdlr_100step); //480ps
						printf("/100 ps ");
					}
					printf("ddr clk frequency : ");
					printf("%08d", (global_ddr_clk));
					printf("Mhz ");
					printf(string_print_flag);
					printf("index    org      min      max      left     right    dec vref_range vref_count");
					printf(string_print_flag);

					char delay_left_margin = 0;
					char delay_right_margin = 0;
					if (all_toghter_enable == 1)
						nibble_max = 1;
					for (nibble_step = 0; nibble_step < nibble_max; nibble_step++) {
						printf("%08d", nibble_step);
						printf(" ");
						printf("%08d", 0);
						{
							printf(" ");
							printf("%08d", 0 //num_arry[TEST_ARG_NIBBLE_SAVE_OFFSET+nibble_step*TEST_ARG_NIBBLE_WIDTH+LCD_BDLR_MIN]
							       );
						}
						{
							printf(" ");
							printf("%08d", 0 //num_arry[TEST_ARG_NIBBLE_SAVE_OFFSET+nibble_step*TEST_ARG_NIBBLE_WIDTH+LCD_BDLR_MAX]
							       );
						}

						delay_left_margin = num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MIN)];
						delay_right_margin = num_arry[(TEST_ARG_NIBBLE_SAVE_OFFSET_BYTE + ((nibble_step + nibble_save_offset) * TEST_ARG_NIBBLE_WIDTH_BYTE) + LCD_BDLR_MAX)];
						printf(" ");
						printf("%08d", delay_left_margin);

						printf(" ");
						printf("%08d", delay_right_margin);
						printf("     ");
						printf("%08d", 0);
						printf("   ");
						printf("%08d", 0);
						printf("   2d-eye"); //p_dev->cur_type
						printf(" dramtype ");
						printf("%08d", 0);
						printf(" ");
						printf("%08d", (global_ddr_clk));
						printf(" M bdl ");
						printf("%08d", bdlr_100step); //480ps
						printf(" /100 ps ");
						printf("1/32step== ");
						printf("%08d", ui_1_32_100step);
						printf(" /100 ps ");
						printf(string_print_flag);
					}
				}
			}
		}
	}

	if (config_register == 1) {
		if (num_arry[TEST_ARG_2_STEP] == 0) {
			num_arry[TEST_ARG_2_STEP] = 1;
			ddr_wr_8_16bit_on_32reg(sticky_reg_base_add, 8, TEST_ARG_2_STEP, num_arry[TEST_ARG_2_STEP]);
		}
	}

	if ((enable_kernel_test) && (num_arry[TEST_ARG_2_STEP] > 1))
		run_command("run storeboot", 0);
	return reg_value;
}

#define ACX_MAX                              0x80

#if (CONFIG_DDR_PHY >= P_DDR_PHY_G12)
#define  G12_DATA_READ_OFFSET_MAX   (0X3F)
#define  G12_DATA_WRITE_OFFSET_MAX   (0X3F + 7 * 32)

#define DMC_TEST_WINDOW_INDEX_ATXDLY 1
#define DMC_TEST_WINDOW_INDEX_TXDQSDLY 2
#define DMC_TEST_WINDOW_INDEX_RXCLKDLY  3
#define DMC_TEST_WINDOW_INDEX_TXDQDLY  4
#define DMC_TEST_WINDOW_INDEX_RXPBDLY  5
#define DMC_TEST_WINDOW_INDEX_RXENDLY  6

#define DMC_TEST_WINDOW_INDEX_EE_VOLTAGE  0x11
#define DMC_TEST_WINDOW_INDEX_SOC_VREF  0x12
#define DMC_TEST_WINDOW_INDEX_DRAM_VREF    0x14

uint32_t  ddr_cacl_phy_delay_all_step(char test_index, uint32_t value)
{
	uint32_t result = 0;

	if (test_index == DMC_TEST_WINDOW_INDEX_ATXDLY)
		result = (32 * (((value >> 6) & 1) + ((value >> 5) & 1))) + (value & 0x1f);
	//use for txdqdly register ,because of this register bit 5 is no use jiaxing 20180814
	else if ((test_index == DMC_TEST_WINDOW_INDEX_TXDQDLY) || (test_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY))
		result = (32 * (((value >> 6) & 7))) + (value & 0x3f);
	else    //other register bit5 is effect ,but can not modify coarse delay why  ?  jiaxing 20180814
		result = value & 0x3f;
	if (test_index >= DMC_TEST_WINDOW_INDEX_EE_VOLTAGE)
		result = value;
	return result;
}

uint32_t ddr_cacl_phy_over_ride_back_reg(char test_index, uint32_t value)
{
	uint32_t result = 0;

	if ((test_index == DMC_TEST_WINDOW_INDEX_ATXDLY)) {
		if (value < 64)
			result = ((value / 32) << 6) + value % 32;
		else
			result = (3 << 5) + (value % 32);
	} else if ((test_index == DMC_TEST_WINDOW_INDEX_TXDQDLY)
		   || (test_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY)
		   || (test_index == DMC_TEST_WINDOW_INDEX_RXENDLY)) {
		result = value % 64;
		if ((test_index == DMC_TEST_WINDOW_INDEX_TXDQDLY) || (test_index == DMC_TEST_WINDOW_INDEX_RXENDLY))            //use for txdqdly register ,because of this register bit 5 is no use jiaxing 20180814
			result = ((value / 32) << 6) + value % 32;
	} else if (test_index == DMC_TEST_WINDOW_INDEX_RXPBDLY) {
		result = value & 0x3f;
	} else if (test_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY) {
		result = ((value / 32) << 6) + value % 32;
	} else if (test_index > DMC_TEST_WINDOW_INDEX_RXPBDLY) {
		result = value;
	}

	return result;
}

unsigned int do_ddr_g12_read_write_ddr_add_window_lcdlr(unsigned int rank_index, unsigned int add_index, unsigned int lcdlr_value, unsigned int read_write_flag)
{
	dwc_ddrphy_apb_wr(0xd0000, 0); //mw fe1a0000  0
	if (read_write_flag == DDR_PARAMETER_READ)
		lcdlr_value = dwc_ddrphy_apb_rd((0 << 20) | (0 << 16) | (add_index << 12) | (0x80));
	if (read_write_flag == DDR_PARAMETER_WRITE)
		dwc_ddrphy_apb_wr(((0 << 20) | (0 << 16) | (add_index << 12) | (0x80)), lcdlr_value);

	printf("rank_index %d   add_index %d  lcdlr== %d \n", rank_index, add_index, lcdlr_value);
	return lcdlr_value;
}

void dwc_window_reg_after_training_update_increas_dq(char over_ride_index, uint32_t over_ride_sub_index, uint32_t over_ride_increase_decrease,
						     uint32_t step_value)
{
	uint32_t delay_old_value = 0;
	uint32_t delay_reg_value = 0;

	uint64_t reg_add = 0;

	if (!over_ride_index)
		return;

	if (over_ride_index == DMC_TEST_WINDOW_INDEX_TXDQDLY) {
		reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 36) / 9) << 12) | (0xc0 + ((over_ride_sub_index % 9) << 8) + (over_ride_sub_index / 36)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		delay_reg_value = ddr_cacl_phy_delay_all_step(over_ride_index, delay_old_value);
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
		delay_reg_value = ddr_cacl_phy_over_ride_back_reg(over_ride_index, delay_reg_value);
		dwc_ddrphy_apb_wr(reg_add, delay_reg_value);
	}

	printf("reg_add %08x old_value %08x update_to %08x dec %d to %d \n", ((unsigned int)(((reg_add) << 1) + (p_ddr_base->ddr_phy_base_address))),
	       delay_old_value, dwc_ddrphy_apb_rd(reg_add), ddr_cacl_phy_delay_all_step(over_ride_index, delay_old_value),
	       (unsigned int)ddr_cacl_phy_delay_all_step(over_ride_index, dwc_ddrphy_apb_rd(reg_add)));
}

void dwc_window_reg_after_training_update(char over_ride_index, uint32_t over_ride_sub_index, uint32_t over_ride_value)
{
	uint32_t delay_old_value = 0;
	uint32_t delay_reg_value = 0;
	uint64_t reg_add = 0;

	if (!over_ride_index)
		return;
	delay_reg_value = ddr_cacl_phy_over_ride_back_reg(over_ride_index, over_ride_value);
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_ATXDLY) {
		reg_add = ((0 << 20) | (0 << 16) | (over_ride_sub_index << 12) | (0x80));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		dwc_ddrphy_apb_wr(reg_add, delay_reg_value);
	}

	if (over_ride_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY) {
		reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 8) >> 1) << 12) | (0xd0 + (over_ride_sub_index / 8) + ((over_ride_sub_index % 2) << 8)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);

		dwc_ddrphy_apb_wr(reg_add, delay_reg_value);
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY) {
		reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 8) >> 1) << 12) | (0x8c + (over_ride_sub_index / 8) + ((over_ride_sub_index % 2) << 8)));
		delay_old_value = dwc_ddrphy_apb_rd((0 << 20) | (1 << 16) | (((over_ride_sub_index % 8) >> 1) << 12) | (0x8c + (over_ride_sub_index / 8) + ((over_ride_sub_index % 2) << 8)));
		dwc_ddrphy_apb_wr(reg_add, delay_reg_value);
		dwc_ddrphy_apb_wr(reg_add + 4, delay_reg_value);
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_TXDQDLY) {
		reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 36) / 9) << 12) | (0xc0 + ((over_ride_sub_index % 9) << 8) + (over_ride_sub_index / 36)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		dwc_ddrphy_apb_wr(reg_add, delay_reg_value);
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXPBDLY) {
		reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 36) / 9) << 12) | (0x68 + ((over_ride_sub_index % 9) << 8) + (over_ride_sub_index / 36)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		dwc_ddrphy_apb_wr(reg_add, delay_reg_value);
	}

	if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXENDLY) {
		reg_add = ((0 << 20) | (1 << 16) | ((over_ride_sub_index % 8) << 12) | (0x80 + (over_ride_sub_index / 8)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		dwc_ddrphy_apb_wr(reg_add, delay_reg_value);
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF)
		dwc_ddrphy_apb_wr((0 << 20) | (1 << 16) | (((over_ride_sub_index % 36) / 9) << 12) | (((over_ride_sub_index % 36) % 9) << 8) | (0x40), over_ride_value);
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF_DAC1)
		dwc_ddrphy_apb_wr((0 << 20) | (1 << 16) | (((over_ride_sub_index % 36) / 9) << 12) | (((over_ride_sub_index % 36) % 9) << 8) | (0x30), over_ride_value);

	printf("reg_add %08x old_value %08x update_to %08x dec %d to %d \n", ((unsigned int)(((reg_add) << 1) + (p_ddr_base->ddr_phy_base_address))),
	       delay_old_value, dwc_ddrphy_apb_rd(reg_add), ddr_cacl_phy_delay_all_step(over_ride_index, delay_old_value),
	       (unsigned int)ddr_cacl_phy_delay_all_step(over_ride_index, delay_reg_value));
}

void dwc_window_reg_after_training_update_increas_sub(char over_ride_index, uint32_t over_ride_sub_index, uint32_t over_ride_increase_decrease,
						      uint32_t step_value)
{
	uint32_t delay_old_value = 0;
	uint32_t delay_reg_value = 0;

	uint64_t reg_add = 0;

	if (!over_ride_index)
		return;
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_ATXDLY) {
		reg_add = ((0 << 20) | (0 << 16) | (over_ride_sub_index << 12) | (0x80));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		delay_reg_value = ddr_cacl_phy_delay_all_step(over_ride_index, delay_old_value);
		if (over_ride_increase_decrease == 0) {
			delay_reg_value = delay_reg_value + step_value;
			if (delay_reg_value > 95)
				delay_reg_value = 95;
		}
		if (over_ride_increase_decrease != 0) {
			if (delay_reg_value >= step_value)
				delay_reg_value = delay_reg_value - step_value;
			else
				delay_reg_value = 0;
		}
		delay_reg_value = ddr_cacl_phy_over_ride_back_reg(over_ride_index, delay_reg_value);
		dwc_ddrphy_apb_wr((reg_add), delay_reg_value);
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY) {
		reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 8) >> 1) << 12) | (0xd0 + (over_ride_sub_index / 8) + ((over_ride_sub_index % 2) << 8)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);

		//some case will happen tdqs from 0x1f to 0x0 or 0x0 to 0x1f ,then fast boot write back will happen error ,because
		// fast boot write back will re-calculate coarse UI,then result dq phase fail.
		if ((over_ride_sub_index % 2) == 0) {
			char temp_test_index = DMC_TEST_WINDOW_INDEX_TXDQDLY;
			char temp_count = 0;
			{
				for (temp_count = 0; temp_count < 9; temp_count++)
					dwc_window_reg_after_training_update_increas_dq(temp_test_index
											, (((over_ride_sub_index) >> 1) * 9 + temp_count), (!over_ride_increase_decrease), step_value);
			}
		}
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_TXDQDLY) {
		reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 36) / 9) << 12) | (0xc0 + ((over_ride_sub_index % 9) << 8) + (over_ride_sub_index / 36)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		delay_reg_value = ddr_cacl_phy_delay_all_step(over_ride_index, delay_old_value);
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
		delay_reg_value = ddr_cacl_phy_over_ride_back_reg(over_ride_index, delay_reg_value);
		dwc_ddrphy_apb_wr(reg_add, delay_reg_value);
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY) {
		reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 8) >> 1) << 12) | (0x8c + (over_ride_sub_index / 8) + ((over_ride_sub_index % 2) << 8)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		delay_reg_value = ddr_cacl_phy_delay_all_step(over_ride_index, delay_old_value);
		if (over_ride_increase_decrease == 0) {
			delay_reg_value = delay_reg_value + step_value;
			if (delay_reg_value > 95)
				delay_reg_value = 95;
		}
		if (over_ride_increase_decrease != 0) {
			if (delay_reg_value > step_value)
				delay_reg_value = delay_reg_value - step_value;
			else
				delay_reg_value = 0;
		}
		delay_reg_value = ddr_cacl_phy_over_ride_back_reg(over_ride_index, delay_reg_value);
		dwc_ddrphy_apb_wr(reg_add, delay_reg_value);
		dwc_ddrphy_apb_wr(reg_add + 4, delay_reg_value);
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXPBDLY) {
		reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 36) / 9) << 12) | (0x68 + ((over_ride_sub_index % 9) << 8) + (over_ride_sub_index / 36)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		delay_reg_value = ddr_cacl_phy_delay_all_step(over_ride_index, delay_old_value);
		if (over_ride_increase_decrease == 0) {
			delay_reg_value = delay_reg_value + step_value;
			if (delay_reg_value > 63)
				delay_reg_value = 63;
		}
		if (over_ride_increase_decrease != 0) {
			if (delay_reg_value > step_value)
				delay_reg_value = delay_reg_value - step_value;
			else
				delay_reg_value = 0;
		}
		delay_reg_value = ddr_cacl_phy_over_ride_back_reg(over_ride_index, delay_reg_value);
		dwc_ddrphy_apb_wr(reg_add, delay_reg_value);
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXENDLY) {
		reg_add = ((0 << 20) | (1 << 16) | ((over_ride_sub_index % 8) << 12) | (0x80 + (over_ride_sub_index / 8)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		//delay_reg_value = ddr_cacl_phy_delay_all_step(over_ride_index, delay_old_value);
		if (over_ride_increase_decrease == 0) {
			dwc_ddrphy_apb_wr(reg_add, ((delay_old_value & 0x3f) + step_value) | (delay_old_value & 0xffc0));
			if (((delay_old_value & 0x3f) + step_value) > 0x3f)
				dwc_ddrphy_apb_wr(reg_add, 0x3f | (delay_old_value & 0xffc0));
		} else {
			dwc_ddrphy_apb_wr(reg_add, ((delay_old_value & 0x3f) - step_value) | (delay_old_value & 0xffc0));
			if (((delay_old_value & 0x3f) < step_value))
				dwc_ddrphy_apb_wr(reg_add, (delay_old_value & 0xffc0));
		}
	}

	if ((over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF) || (over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF_DAC1)) {
		reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 36) / 9) << 12) | (((over_ride_sub_index % 36) % 9) << 8) | (0x40));
		if (over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF_DAC1)
			reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 36) / 9) << 12) | (((over_ride_sub_index % 36) % 9) << 8) | (0x30));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		delay_reg_value = delay_old_value;
		if (over_ride_increase_decrease == 0) {
			delay_reg_value = delay_reg_value + step_value;
			if (delay_reg_value > 127)
				delay_reg_value = 127;
		}
		if (over_ride_increase_decrease != 0) {
			if (delay_reg_value >= step_value)
				delay_reg_value = delay_reg_value - step_value;
			else
				delay_reg_value = 0;
		}
		dwc_ddrphy_apb_wr(reg_add, delay_reg_value);
	}
	printf("reg_add %08x old_value %08x update_to %08x dec %d to %d \n", ((unsigned int)(((reg_add) << 1) + (p_ddr_base->ddr_phy_base_address))),
	       delay_old_value, dwc_ddrphy_apb_rd(reg_add), ddr_cacl_phy_delay_all_step(over_ride_index, delay_old_value),
	       (unsigned int)ddr_cacl_phy_delay_all_step(over_ride_index, dwc_ddrphy_apb_rd(reg_add)));
}

void dwc_window_reg_after_training_update_increas(char over_ride_index, uint32_t over_ride_sub_index, uint32_t over_ride_increase_decrease,
						  uint32_t offset_value)
{
	uint32_t delay_old_value = 0;
	uint32_t temp_count_3 = 0;
	uint64_t reg_add = 0;

	if (!over_ride_index)
		return;

	if (over_ride_index == DMC_TEST_WINDOW_INDEX_ATXDLY) {
		reg_add = ((0 << 20) | (0 << 16) | (over_ride_sub_index << 12) | (0x80));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
			dwc_window_reg_after_training_update_increas_sub(over_ride_index
									 , ((over_ride_sub_index)), over_ride_increase_decrease, 1);
	}

	if (over_ride_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY) {
		reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 8) >> 1) << 12) | (0xd0 + (over_ride_sub_index / 8) + ((over_ride_sub_index % 2) << 8)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
			dwc_window_reg_after_training_update_increas_sub(over_ride_index
									 , ((over_ride_sub_index)), over_ride_increase_decrease, 1);
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY) {
		reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 8) >> 1) << 12) | (0x8c + (over_ride_sub_index / 8) + ((over_ride_sub_index % 2) << 8)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
			dwc_window_reg_after_training_update_increas_sub(over_ride_index
									 , ((over_ride_sub_index)), over_ride_increase_decrease, 1);
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_TXDQDLY) {
		reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 36) / 9) << 12) | (0xc0 + ((over_ride_sub_index % 9) << 8) + (over_ride_sub_index / 36)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
			dwc_window_reg_after_training_update_increas_sub(over_ride_index
									 , ((over_ride_sub_index)), over_ride_increase_decrease, 1);
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXPBDLY) {
		reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 36) / 9) << 12) | (0x68 + ((over_ride_sub_index % 9) << 8) + (over_ride_sub_index / 36)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
			dwc_window_reg_after_training_update_increas_sub(over_ride_index
									 , ((over_ride_sub_index)), over_ride_increase_decrease, 1);
	}

	if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXENDLY) {
		reg_add = ((0 << 20) | (1 << 16) | ((over_ride_sub_index % 8) << 12) | (0x80 + (over_ride_sub_index / 8)));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
			dwc_window_reg_after_training_update_increas_sub(over_ride_index
									 , ((over_ride_sub_index)), over_ride_increase_decrease, 1);
	}

	if ((over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF) || (over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF_DAC1)) {
		reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 36) / 9) << 12) | (((over_ride_sub_index % 36) % 9) << 8) | (0x40));
		if (over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF_DAC1)
			reg_add = ((0 << 20) | (1 << 16) | (((over_ride_sub_index % 36) / 9) << 12) | (((over_ride_sub_index % 36) % 9) << 8) | (0x30));
		delay_old_value = dwc_ddrphy_apb_rd(reg_add);
		for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
			dwc_window_reg_after_training_update_increas_sub(over_ride_index
									 , ((over_ride_sub_index)), over_ride_increase_decrease, 1);
	}

	printf("over_ride_increase_decrease==%d\n", over_ride_increase_decrease);

	if (over_ride_increase_decrease == 1) {
		unsigned int org_cacl_value = (delay_old_value) & 0x3f;
		printf("org_cacl_value==%d\n", org_cacl_value);
		printf("offset_value==%d\n", offset_value);
		if ((org_cacl_value & 0x3f) < offset_value) {
			char temp_test_index_2 = 0;
			char temp_count_4 = 0;
			char temp_count_2 = 0;
			if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY) {
				temp_test_index_2 = DMC_TEST_WINDOW_INDEX_RXPBDLY;

				for (temp_count_2 = 0; temp_count_2 < 4; temp_count_2++) {
					for (temp_count_4 = 0; temp_count_4 < (((offset_value - org_cacl_value) * ui_1_32_100step) / bdlr_100step); temp_count_4++) {
						dwc_window_reg_after_training_update_increas_sub(temp_test_index_2
												 , ((over_ride_sub_index / 2) * 9 +
												    temp_count_2 + (over_ride_sub_index % 2) * 4), 0, 1);
					}
				}
			}

			if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXPBDLY) {
				temp_test_index_2 = DMC_TEST_WINDOW_INDEX_RXPBDLY;

				for (temp_count_2 = 0; temp_count_2 < 4; temp_count_2++) {
					if (temp_count_2 == ((over_ride_sub_index % 9) % 4))
						temp_count_2++;

					for (temp_count_4 = 0; temp_count_4 < (offset_value - org_cacl_value); temp_count_4++) {
						dwc_window_reg_after_training_update_increas_sub(temp_test_index_2
												 , ((over_ride_sub_index / 9) * 9 +
												    temp_count_2 + (((over_ride_sub_index % 9) > 3) ? 4 : 0)), 0, 1);
					}
				}

				if ((((offset_value - org_cacl_value) * bdlr_100step) / ui_1_32_100step)) {
					temp_test_index_2 = DMC_TEST_WINDOW_INDEX_RXCLKDLY;

					for (temp_count_4 = 0; temp_count_4 < (((offset_value - org_cacl_value) * bdlr_100step) / ui_1_32_100step); temp_count_4++) {
						dwc_window_reg_after_training_update_increas_sub(temp_test_index_2
												 , (((over_ride_sub_index / 9) << 1) +
												    (((over_ride_sub_index % 9) > 3) ? 1 : 0)
												    ), 0, 1);
					}
				}
			}
		}
	}
	printf("reg_add %08x old_value %08x update_to %08x dec %d to %d \n", ((unsigned int)(((reg_add) << 1) + (p_ddr_base->ddr_phy_base_address))),
	       delay_old_value, dwc_ddrphy_apb_rd(reg_add), ddr_cacl_phy_delay_all_step(over_ride_index, delay_old_value),
	       ddr_cacl_phy_delay_all_step(over_ride_index, dwc_ddrphy_apb_rd(reg_add)));
}

int do_ddr2pll_g12_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	check_base_address();
#define DMC_WINDOW_CMD   20180010          //g12_d2pll 1584 0 0 0 0 0x8
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

	char *endp;
	unsigned int pll;
	unsigned int window_test_stick_cmd_value = 0;
	/* need at least two arguments */
	if (((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_C2) &&
	(p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_T5D)) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S4) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5W))
		do_read_c2_ddr_bdlr_steps();
	if (argc < 2)
		//goto usage;
		return 1;

	pll = simple_strtoul(argv[1], &endp, 0);
	if (*argv[1] == 0 || *endp != 0) {
		printf("Error: Wrong format parament!pll=0x%08x\n", pll);
		return 1;
	}
	unsigned int argc_count = 1;
	unsigned int para_meter[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
	while (argc_count < argc) {
		para_meter[argc_count - 1] = simple_strtoul(argv[argc_count], &endp, 0);
		if (*argv[argc_count] == 0 || *endp != 0)
			para_meter[argc_count - 1] = 0;
		argc_count++;
	}

	argc_count = 2;
	window_test_stick_cmd_value = para_meter[argc_count - 1];
	window_test_stick_cmd_value = (window_test_stick_cmd_value & 0xff);
	if ((window_test_stick_cmd_value == G12_D2PLL_CMD_OVER_RIDE) || (window_test_stick_cmd_value == G12_D2PLL_CMD_OVER_RIDE_PLUS_FULLTEST)) {
		para_meter[3] = (para_meter[3] << 24) | (para_meter[4] << 16) | (para_meter[5] << 0);
		para_meter[4] = (para_meter[6] << 24) | (para_meter[7] << 16) | (para_meter[8] << 0);
		para_meter[5] = (para_meter[9] << 24) | (para_meter[10] << 16) | (para_meter[11] << 0);
	}
	if ((window_test_stick_cmd_value == G12_D2PLL_CMD_WINDOW_TEST) || (window_test_stick_cmd_value == G12_D2PLL_CMD_WINDOW_TEST_AND_STICKY_OVERRIDE))
		para_meter[5] = (para_meter[9] << 28) | (para_meter[10] << 24) | (para_meter[11] << 20) | (para_meter[12] << 21) | (para_meter[13] << 22) | (para_meter[14] << 25) | (para_meter[5] << 0);
	if ((window_test_stick_cmd_value >= G12_D2PLL_CMD_DDR_EYE_TEST) && (window_test_stick_cmd_value <= G12_D2PLL_CMD_DDR_EYE_TEST_AND_STICKY_OVERRIDE_DAC1)) {
		para_meter[3] = (para_meter[3] << 0) | (para_meter[4] << 8) | (para_meter[5] << 16) | (para_meter[6] << 24);
		para_meter[4] = (para_meter[7] << 0) | (para_meter[8] << 8) | (para_meter[9] << 16) | (para_meter[10] << 24);
		para_meter[5] = para_meter[11]; //(para_meter[11]<<0)|(para_meter[12]<<8)|(para_meter[13]<<16)|(para_meter[14]<<24);
		para_meter[6] = para_meter[12]; //para_meter[15];
		para_meter[7] = para_meter[13]; //para_meter[16];
		para_meter[8] = para_meter[14]; //para_meter[17];
	}
	wr_reg((p_ddr_base->preg_sticky_reg0), (rd_reg((p_ddr_base->preg_sticky_reg0)) & 0xffff) | (0xf13 << 20));
	argc_count = 0;
	printf("\nP_PREG_STICKY_REG [0x%08x]  [0x%08x]==[0x%08x]\n", argc_count, ((p_ddr_base->preg_sticky_reg0) + (argc_count << 2)), rd_reg((p_ddr_base->preg_sticky_reg0) + (argc_count << 2)));
	argc_count = 1;
	while (argc_count < 10) {
		wr_reg((p_ddr_base->preg_sticky_reg0) + (argc_count << 2), para_meter[argc_count - 1]);
		printf("P_PREG_STICKY_REG [0x%08x]  [0x%08x]==[0x%08x]\n", argc_count, ((p_ddr_base->preg_sticky_reg0) + (argc_count << 2)), rd_reg((p_ddr_base->preg_sticky_reg0) + (argc_count << 2)));
		argc_count++;
	}
	dcache_disable();
	if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_A1) || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_C1) || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_C2)
	    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_SC2) || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S4)
	    || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)
	    //|| (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5) || (p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5D)
	    ) {
		printf("reset...\n");
		run_command("reset", 0);
	} else {                        //G12A/G12B/SM1/TL1/TM2 //(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T7)
		printf("reboot...\n");  //reset will enter bl2 panic path,so change to "reboot"
		run_command("reboot", 0);
		//ddr_test_watchdog_reset_system();
	}

	return 0;

//usage:
//	cmd_usage(cmdtp);
//	return 1;
}

U_BOOT_CMD(
	g12_d2pll, 18, 1, do_ddr2pll_g12_cmd,
	"g12_d2pll 1300  1 0x10 0",
	"g12_d2pll  clk delay_index delay_value before_after_training_setting\n"
	);

#endif

#if (CONFIG_DDR_PHY >= P_DDR_PHY_G12)

int do_ddr_g12_override_data(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	check_base_address();

	uint32_t dmc_retraining_ctrl = 0;
	dmc_retraining_ctrl = rd_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address));
	unsigned int ddr_dmc_apd_temp_save, ddr_dmc_asr_temp_save;
	ddr_dmc_apd_temp_save = readl((p_ddr_base->ddr_dmc_apd_address));
	ddr_dmc_asr_temp_save = readl((p_ddr_base->ddr_dmc_asr_address));
	printf("\12nm phy read write register should closed apd and asr function\n");

#define  G12_DATA_READ_OFFSET_MAX   (0X3F)
#define  G12_DATA_WRITE_OFFSET_MAX   (0X3F + 7 * 32)

#if 1
	int i = 0;
	printf("\nargc== 0x%08x\n", argc);
	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);
	char *endp;
	//rank_index  dq_index  write_read left/right  offset_value
	unsigned int test_index = 0;    // 1 ac ,0x2, write dqs ,0x4,read dqs,0x8,write dq,0x10 read dq
	unsigned int dq_index = 0;      //0-8 rank0 lane0 ,rank0 9-17 lane1,rank0 18-26 lane2, rank0 27-35 lane3,  36+0-8 rank1 lane0 ,rank1  36+9-17 lane1,rank1  36+18-26 lane2, rank1  36+27-35 lane3
	unsigned int ovrride_value = 0; //

	unsigned int count = 0;
	unsigned int lcdlr_max = 0;

	if (argc == 1) {
		printf("\nplease read help\n");
	} else if (argc > 4) { //offset_enable=1;
		{
			count = 0;
			test_index = simple_strtoull_ddr(argv[count + 1], &endp, 0);
			if (*argv[count + 1] == 0 || *endp != 0)
				test_index = 0;
		}
		{
			count++;
			dq_index = simple_strtoull_ddr(argv[count + 1], &endp, 0);
			if (*argv[count + 1] == 0 || *endp != 0)
				dq_index = 0;
		}

		{
			count++;
			ovrride_value = simple_strtoull_ddr(argv[count + 1], &endp, 0);
			if (*argv[count + 1] == 0 || *endp != 0)
				ovrride_value = 0;
		}
	} else {
		return 1;
	}
	printf("lcdlr_max %d,\n", lcdlr_max);

	wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl & (~(1 << 31)));
	writel((0), p_ddr_base->ddr_dmc_apd_address);
	writel((0), p_ddr_base->ddr_dmc_asr_address);
	{
		dwc_window_reg_after_training_update(test_index, dq_index, ovrride_value);
	}

#endif
	wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
	writel(ddr_dmc_apd_temp_save, p_ddr_base->ddr_dmc_apd_address);
	writel(ddr_dmc_asr_temp_save, p_ddr_base->ddr_dmc_asr_address);
	return 1;
}
U_BOOT_CMD(
	ddr_g12_override_data, 20, 1, do_ddr_g12_override_data,
	"ddr_g12_override_data  1 0  0 0  1 3",
	"ddr_g12_override_data  test_index  dq_index ovrride_value   \n"
	);

void ddr_window_reg_after_training_update_increas_sub_c2(char over_ride_index, uint32_t over_ride_sub_index, uint32_t over_ride_increase_decrease, uint32_t step)
{
	uint32_t delay_old_value = 0;
	uint32_t delay_reg_value = 0;
	uint32_t ps = 0;

	if (!over_ride_index)
		return;
	if (over_ride_index <= DMC_TEST_WINDOW_INDEX_DRAM_VREF) {
		delay_old_value = ddr_phy_training_reg_read_write(ddr_set_t_p, over_ride_index, over_ride_sub_index, delay_old_value, REGISTER_READ, ps);
		if (over_ride_increase_decrease == 0)
			delay_reg_value = delay_old_value + step;
		if (over_ride_increase_decrease != 0) {
			if (delay_old_value > step)
				delay_reg_value = delay_old_value - step;
			else
				delay_reg_value = 0;
		}

		ddr_phy_training_reg_read_write(ddr_set_t_p, over_ride_index, over_ride_sub_index, delay_reg_value, REGISTER_WRITE,
						ps);
	}
}

void dwc_window_reg_after_training_update_increas_c2(char over_ride_index, uint32_t over_ride_sub_index, uint32_t over_ride_increase_decrease,
						     uint32_t offset_value)
{
	uint32_t temp_count_3 = 0;

	if (!over_ride_index)
		return;

	if (over_ride_index == DMC_TEST_WINDOW_INDEX_ATXDLY) {
		for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
			ddr_window_reg_after_training_update_increas_sub_c2(over_ride_index
									    , ((over_ride_sub_index)), over_ride_increase_decrease, 1);
	}

	if (over_ride_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY) {
		for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
			ddr_window_reg_after_training_update_increas_sub_c2(over_ride_index
									    , ((over_ride_sub_index)), over_ride_increase_decrease, 1);
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY) {
		for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
			ddr_window_reg_after_training_update_increas_sub_c2(over_ride_index
									    , ((over_ride_sub_index)), over_ride_increase_decrease, 1);
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_TXDQDLY) {
		for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
			ddr_window_reg_after_training_update_increas_sub_c2(over_ride_index
									    , ((over_ride_sub_index)), over_ride_increase_decrease, 1);
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXPBDLY) {
		for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
			ddr_window_reg_after_training_update_increas_sub_c2(over_ride_index
									    , ((over_ride_sub_index)), over_ride_increase_decrease, 1);
	}

	if (over_ride_index == DMC_TEST_WINDOW_INDEX_RXENDLY) {
		for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
			ddr_window_reg_after_training_update_increas_sub_c2(over_ride_index
									    , ((over_ride_sub_index)), over_ride_increase_decrease, 1);
	}

	if (over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF) {
		for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
			ddr_window_reg_after_training_update_increas_sub_c2(over_ride_index
									    , ((over_ride_sub_index)), over_ride_increase_decrease, 1);
	}
	if (over_ride_index == DMC_TEST_WINDOW_INDEX_SOC_VREF_DAC1) {
		for (temp_count_3 = 0; temp_count_3 < offset_value; temp_count_3++)
			ddr_window_reg_after_training_update_increas_sub_c2(over_ride_index
									    , ((over_ride_sub_index)), over_ride_increase_decrease, 1);
	}
}

int do_ddr_c2_offset_data(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
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

	printf("\nargc== 0x%08x\n", argc);
	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);
	char *endp;
	//rank_index  dq_index  write_read left/right  offset_value
	unsigned int test_index = 0;            // 1 ac ,0x2, write dqs ,0x4,read dqs,0x8,write dq,0x10 read dq
	unsigned int test_dq_mask_1 = 0;        //each bit mask correspond with dq_index
	unsigned int test_dq_mask_2 = 0;        //each bit mask correspond with dq_index
	unsigned int test_dq_mask_3 = 0;        //each bit mask correspond with dq_index
	unsigned int left_right_flag = 0;       //  1 left ,2 right   #define  DDR_PARAMETER_LEFT		1     #define   DDR_PARAMETER_RIGHT		2
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

	if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)) {
		writel(ddr_dmc_apd_temp_save, p_ddr_base->ddr_dmc_apd_address);
		writel(ddr_dmc_asr_temp_save, p_ddr_base->ddr_dmc_asr_address);
		wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
		count = 0;
		test_index = simple_strtoull_ddr(argv[count + 1], &endp, 0);
		if (*argv[count + 1] == 0 || *endp != 0)
			test_index = 0;
		if (test_index / 100)
			dmc_ddr_config_channel_id = 1;
		else
			dmc_ddr_config_channel_id = 0;
		test_index = (test_index % 100);     //for DDR_PHY 1
		dmc_change_channel(dmc_ddr_config_channel_id);
		dmc_retraining_ctrl = rd_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address);
		wr_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address, dmc_retraining_ctrl & (~(1 << 31)));
		ddr_dmc_apd_temp_save = readl(p_ddr_base->ddr_dmc_apd_address);
		ddr_dmc_asr_temp_save = readl(p_ddr_base->ddr_dmc_asr_address);
	}

	//printf("lcdlr_max %d,\n", lcdlr_max);
	if (left_right_flag == DDR_PARAMETER_RIGHT)
		printf("offset right ++  left_right_flag %d,\n", left_right_flag);
	if (left_right_flag == DDR_PARAMETER_LEFT)
		printf("offset left --left_right_flag %d,\n", left_right_flag);

	if (test_index == DMC_TEST_WINDOW_INDEX_ATXDLY)
		count_max = 36;
	//	lcdlr_max = 4 * 128 - 1; //0x3ff;
	if (test_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY)
		count_max = 8;
	//	lcdlr_max = 4 * 128 - 1; //0x3ff;
	if (test_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY)
		count_max = 8;
	//	lcdlr_max = 255; //0x3f;

	if (test_index == DMC_TEST_WINDOW_INDEX_TXDQDLY)
		count_max = 36 * 2;
	//	lcdlr_max = 8 * 128 - 1;          //0x1ff;

	if (test_index == DMC_TEST_WINDOW_INDEX_RXPBDLY)
		count_max = 36 * 2;
	//	lcdlr_max = 255;
	if (test_index == DMC_TEST_WINDOW_INDEX_SOC_VREF) {
		count_max = 36 * 1;
		//	lcdlr_max = 0x3f;
		printf(" soc vref rank0 and rank1 share vref dac\n");
	}
	if (test_index == DMC_TEST_WINDOW_INDEX_SOC_VREF_DAC1) {
		count_max = 36 * 1;
		//	lcdlr_max = 0x3f;
		printf(" soc vref rank0 and rank1 share vref dac\n");
	}
	count = 0;
	writel((0), p_ddr_base->ddr_dmc_apd_address);
	writel((0), p_ddr_base->ddr_dmc_asr_address);

	for (; count < count_max; count++) {
		if ((count < 32))
			if (test_dq_mask_1 & (1 << (count % 32)))
				continue;

		if ((count > 31) && (count < 63))
			if (test_dq_mask_2 & (1 << (count % 32)))
				continue;

		if ((count > 63))
			if (test_dq_mask_3 & (1 << (count % 32)))
				continue;
		{
			if (left_right_flag == DDR_PARAMETER_RIGHT) {
				dwc_window_reg_after_training_update_increas_c2(test_index,
										count,
										0, offset_value);
			}
			if (left_right_flag == DDR_PARAMETER_LEFT) {
				dwc_window_reg_after_training_update_increas_c2(test_index,
										count,
										1, offset_value);
			}
		}
	}
	writel(ddr_dmc_apd_temp_save, p_ddr_base->ddr_dmc_apd_address);
	writel(ddr_dmc_asr_temp_save, p_ddr_base->ddr_dmc_asr_address);
	wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);

	if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)) {
		dmc_ddr_config_channel_id = 0;
		dmc_change_channel(dmc_ddr_config_channel_id);
	}
	return 1;
}

int do_ddr_g12_offset_data(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	//ddr_g12_offset_data  1 0  0 0  1 3
	check_base_address();

	if (((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_C2) &&
	(p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_T5D)) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S4) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5W)) {
		do_ddr_c2_offset_data(cmdtp, flag, argc, argv);
		return 1;
	}

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
	printf("\nargc== 0x%08x\n", argc);
	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);
	char *endp;
	//rank_index  dq_index  write_read left/right  offset_value
	unsigned int test_index = 0;            // 1 ac ,0x2, write dqs ,0x4,read dqs,0x8,write dq,0x10 read dq
	unsigned int test_dq_mask_1 = 0;        //each bit mask correspond with dq_index
	unsigned int test_dq_mask_2 = 0;        //each bit mask correspond with dq_index
	unsigned int test_dq_mask_3 = 0;        //each bit mask correspond with dq_index
	unsigned int left_right_flag = 0;       //  1 left ,2 right   #define  DDR_PARAMETER_LEFT		1     #define   DDR_PARAMETER_RIGHT		2
	unsigned int offset_value = 0;          //
	unsigned int count = 0;
	unsigned int count_max = 0;
	//unsigned int lcdlr_max = 0;

	global_ddr_clk = get_ddr_clk();
	bdlr_100step = get_bdlr_100step(global_ddr_clk);
	if (global_ddr_clk)
		ui_1_32_100step = (1000000 * 100 / (global_ddr_clk * 2 * 32));

	if (argc == 1) {
		printf("\nplease read help\n");
		return 1;
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

	if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T7)) {
		writel(ddr_dmc_apd_temp_save, p_ddr_base->ddr_dmc_apd_address);
		writel(ddr_dmc_asr_temp_save, p_ddr_base->ddr_dmc_asr_address);
		wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
		count = 0;
		test_index = simple_strtoull_ddr(argv[count + 1], &endp, 0);
		if (*argv[count + 1] == 0 || *endp != 0)
			test_index = 0;
		if (test_index / 100)
			dmc_ddr_config_channel_id = 1;
		else
			dmc_ddr_config_channel_id = 0;
		test_index = (test_index % 100);     //for DDR_PHY 1
		dmc_change_channel(dmc_ddr_config_channel_id);
		dmc_retraining_ctrl = rd_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address);
		wr_reg(p_ddr_base->ddr_dmc_lpdd4_retraining_address, dmc_retraining_ctrl & (~(1 << 31)));
		ddr_dmc_apd_temp_save = readl(p_ddr_base->ddr_dmc_apd_address);
		ddr_dmc_asr_temp_save = readl(p_ddr_base->ddr_dmc_asr_address);
	}

	//printf("lcdlr_max %d,\n", lcdlr_max);
	if (left_right_flag == DDR_PARAMETER_RIGHT)
		printf("offset right ++  left_right_flag %d,\n", left_right_flag);
	if (left_right_flag == DDR_PARAMETER_LEFT)
		printf("offset left --left_right_flag %d,\n", left_right_flag);
	if (test_index == DMC_TEST_WINDOW_INDEX_ATXDLY)
		count_max = 10;
	//lcdlr_max = 3 * 32; //0x3ff;

	if (test_index == DMC_TEST_WINDOW_INDEX_TXDQSDLY)
		count_max = 16;
	//lcdlr_max = 16 * 32; //0x3ff;
	if (test_index == DMC_TEST_WINDOW_INDEX_RXCLKDLY)
		count_max = 16;
	//lcdlr_max = 96; //0x3f;
	if (test_index == DMC_TEST_WINDOW_INDEX_TXDQDLY)
		count_max = 36 * 2;
	//lcdlr_max = 8 * 32; //0x1ff;
	if (test_index == DMC_TEST_WINDOW_INDEX_RXPBDLY)
		count_max = 36 * 2;
	//lcdlr_max = 0x3f;
	if (test_index == DMC_TEST_WINDOW_INDEX_SOC_VREF) {
		count_max = 36 * 1;
		//lcdlr_max = 0x3f;
		printf(" soc vref rank0 and rank1 share vref dac\n");
	}
	if (test_index == DMC_TEST_WINDOW_INDEX_SOC_VREF_DAC1) {
		count_max = 36 * 1;
		//lcdlr_max = 0x3f;
		printf(" soc vref rank0 and rank1 share vref dac\n");
	}

	count = 0;
	writel((0), p_ddr_base->ddr_dmc_apd_address);
	writel((0), p_ddr_base->ddr_dmc_asr_address);

	for (; count < count_max; count++) {
		if ((count < 32))
			if (test_dq_mask_1 & (1 << (count % 32)))
				continue;

		if ((count > 31) && (count < 63))
			if (test_dq_mask_2 & (1 << (count % 32)))
				continue;

		if ((count > 63))
			if (test_dq_mask_3 & (1 << (count % 32)))
				continue;
		{
			if (left_right_flag == DDR_PARAMETER_RIGHT) {
				dwc_window_reg_after_training_update_increas(test_index,
									     count,
									     0, offset_value);
			}
			if (left_right_flag == DDR_PARAMETER_LEFT) {
				dwc_window_reg_after_training_update_increas(test_index,
									     count,
									     1, offset_value);
			}
		}
	}

	writel(ddr_dmc_apd_temp_save, p_ddr_base->ddr_dmc_apd_address);
	writel(ddr_dmc_asr_temp_save, p_ddr_base->ddr_dmc_asr_address);
	wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);

	if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T7)) {
		dmc_ddr_config_channel_id = 0;
		dmc_change_channel(dmc_ddr_config_channel_id);
	}
	return 1;
}

U_BOOT_CMD(
	ddr_g12_offset_data, 20, 1, do_ddr_g12_offset_data,
	"ddr_g12_offset_data  1 0  0 0  1 3",
	"ddr_g12_offset_data  test_index  mask1 mask2 mask3  left/right  offset_value \n"
	);

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
	printf("\nargc== 0x%08x\n", argc);
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
#define  DDR_TEST_CMD__DDR_TUNE_MAX_CLK     3   //ddr_test_cmd 3 0x8000000 3 1
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
	unsigned int arg[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
	char *endp;
	ddr_test_cmd = simple_strtoull_ddr(argv[1], &endp, 0);
	for (i = 2; i < argc; i++)
		arg[i - 2] = simple_strtoull_ddr(argv[i], &endp, 0);
	printf("\nddr_test_cmd== 0x%08x\n", ddr_test_cmd);

	for (i = 0; i < (argc - 2); i++)
		printf("\narg[%08x]=%08x\n", i, arg[i]);

	int argc2;
	char *argv2[30];

	argc2 = argc - 1;
	for (i = 1; i < (argc); i++)
		argv2[i - 1] = argv[i];
	{
		wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl & (~(1 << 31)));
		writel((0), p_ddr_base->ddr_dmc_apd_address);
		writel((0), p_ddr_base->ddr_dmc_asr_address);
		run_command("dcache off", 0);
		run_command("dcache on", 0);
		printf("\n cache off on");
		switch (ddr_test_cmd) {
		case (DDR_TEST_CMD__NONE):
		{
			printf("\n  0x0 help\n");
			printf("\n  0x1 ddrtest                             ddr_test_cmd 0x1 start_add test_size loops  ");
			printf("\n  0x2 test aclcdlr                        ddr_test_cmd 0x2 start_add test_size loops    ddr_test_cmd 0x2 a 0 0x8000000  1");
			printf("\n  0x3 test max_pllclk                  ddr_test_cmd 0x3  test_size loops add_freq sub_freq ");
			printf("\n  0x4 test zq                              ddr_test_cmd 0x4  test_size loops add_freq sub_freq drv_odt_flag ");
			printf("\n  0x5 test vref                            ddr_test_cmd 0x5   ");
			printf("\n  0x6 test gxtvbb_crosstalk         ddr_test_cmd 0x6  loops pattern_flag ");
			printf("\n  0x7 test bandwidth                   ddr_test_cmd 0x7 size loops port sub_id timer_ms ");
			printf("\n  0x8 test lcdlr_use_env_uart      ddr_test_cmd 0x8 input_src wr_adj_per[] rd_adj_per[][] ");
			printf("\n  0x9 test_reg_use_mask            ddr_test_cmd 0x9 reg_add value mask  ");
			printf("\n  0xa test ac_clk                        ddr_test_cmd 0xa start_add test_size loops   ddr_test_cmd 0xa a 0 0x8000000  1  ");
			printf("\n  0xb ...  ");
			printf("\n  0xc ...  ");
			printf("\n  0xd ...  ");
			printf("\n  0xe ...  ");
			printf("\n  0xf ...  ");
			printf("\n  0x10 test set zq                                 ddr_test_cmd 0x10 zq0pr0 zq1pr0 zq2pr0   ");
			printf("\n  0x11 test tune dqs                             ddr_test_cmd 0x11 a 0 test_size   ddr_test_cmd 0x11 a 0 0x80000");
			printf("\n  0x12 test set start_add                       ddr_test_cmd 0x12 start_add   ");
			printf("\n  0x13 test ac_bit_setup_hold time        ddr_test_cmd 0x13 a 0 size method  pin_id   ddr_test_cmd 0x13 a 0 0x8000000 0  0xc");
			printf("\n  0x14 test data_bit_setup_hold time      ddr_test_cmd 0x14 a 0 size setup/hold pin_id   ddr_test_cmd 0x14 a 0 0x80000 0 3 ");
			printf("\n  0x15 test ac_lane_setup_hold             ddr_test_cmd 0x15 a 0 size   ");
			printf("\n  0x16 test ee mdlr                              ddr_test_cmd 0x16  voltage pwm_id loops   ");
			printf("\n  0x17 d2pll                                  ddr_test_cmd 0x17 clk zq_ac zq_soc_dram soc_vref dram_vref dec_hex zq_vref 0\n \
						example ddr_test_cmd 0x17 1200 0x2aa4a 0x2015995d 50 81 1 50 \n \
						or  ddr_test_cmd 0x17 1200 0x2aa4a 0x2015995d 0x09 0x20 0 50 \n");
			printf("or  ddr_test_cmd 0x17 1200 6034 60346034 0 0 0 0 1 \n");
			printf("\n  0x18 test data_lane_setup_hold          ddr_test_cmd 0x18 a 0 size range start_pin_id end_pin_id  ddr_test_cmd 0x18 a 0 0x80000 1 0 96 ");
			printf("\n  0x19 test phy vref                             ddr_test_cmd 0x19 a 0 0x80000  1 seed step vref_all vref_lcdlr_offset test_down_up_step seed_hex_dec  \
						ddr_test_cmd 0x19 a 0 0x1000000  1  63  1 1  0x8 0 1 ");
			printf("\n  0x1a test dram vref                           ddr_test_cmd 0x1A a 0 0x80000  clear seed step vref_all vref_lcdlr_offset test_down_up_step vref_range seed_hex_dec \
						\n setenv  ddr_test_ddr4ram_vref ddr_test_cmd 0x1A a 0 0x0800000  0  0x14 1  0  0x8 0 0 0 ; setenv  storeboot  run ddr_test_ddr4ram_vref ;save;reset ");
			printf("\n  0x1b test ac vref                               ddr_test_cmd 0x1B a 0 0x80000  clear seed step vref_all vref_lcdlr_offset seed_hex_dec");
			printf("\n  0x1c sweep dram clk use d2pll_env     ddr_test_cmd 0x1c  test_size start_freq end_freq test_loops  ddr_test_cmd 0x1c 0x8000000 800 1500 1");
			printf("\n  0x1d test shift clk                               ddr_test_cmd 0x1d type delay_ms times");
			printf("\n  0x1e test write_read                          ddr_test_cmd 0x1e write_read pattern_id loop start_add test_size");
			printf("\n  0x1f test pwm_cmd                           ddr_test_cmd 0x1f pwmid   pwm_low pwm_high");
			printf("\n  0x22 test ddr_window use env           ddr_test_cmd 0x22 a 0 test_size watchdog_time \
						lane_disable_masrk add_test_size  setenv bootcmd  ddr_test_cmd 0x22 a 0 0x800000 15 0 0x8000000");
			printf("\n defenv;save;setenv bootcmd ddr_test_cmd 0x22 a 0 0x800000 18 0x0 0x8000000");
			printf("\n setenv env_ddrtest_data_lane  0x22;save;reset");
			printf("\n  0x23 test shift ddr frequency          ddr_test_cmd 0x23");
			printf("\n  0x24 display ddr_information          ddr_test_cmd 0x24");
			printf("\n  0x25 offset ddr_lcdlr          ddr_test_cmd 0x25");
			printf("\n  0x26 set watchdog_value        ddr_test_cmd 0x26 30");
			printf("\n  0x27 test ddr_window use sticky register        ddr_test_cmd 0x27 a 0 test_size watchdog_time \
						lane_disable_masrk add_test_size  setenv bootcmd  ddr_test_cmd 0x27 a 0 0x800000 15 0 0x8000000");
			printf("\n  0x28  sweep dram clk use d2pll_sticky     ddr_test_cmd 0x28  test_size start_freq end_freq test_loops  ddr_test_cmd 0x28 0x8000000 800 1500 1");
		}
			wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
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
			printf("\ntest ddr write read  \n");

			do_ddr_test_write_read((cmd_tbl_t *)cmdtp, (int)flag, (int)argc2, (argv2));
		}
		break;
		case (DDR_TEST_CMD__DDR_SET_UBOOT_G12_RECONFIG_CMD):
		{
			printf("\nset do_ddr_uboot_reconfig cmd\n");

			do_ddr_uboot_new_cmd((cmd_tbl_t *)cmdtp, (int)flag, (int)argc2, (argv2));
		}
		break;

		case (DDR_TEST_CMD__DISPLAY_G12_DDR_INFORMATION):
		{
			printf("\nshow g12 ddr information\n");
			if (((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_C2) &&
			(p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_T5D)) ||
			(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S4) ||
			(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3) ||
			(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5W)) {
				if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3)) {
					dmc_ddr_config_channel_id = 0;
					dmc_change_channel(dmc_ddr_config_channel_id);
					do_ddr_display_c2_ddr_information((cmd_tbl_t *)cmdtp, (int)flag, (int)argc2, (argv2));
					dmc_ddr_config_channel_id = 1;
					dmc_change_channel(dmc_ddr_config_channel_id);
					do_ddr_display_c2_ddr_information((cmd_tbl_t *)cmdtp, (int)flag, (int)argc2, (argv2));
					dmc_ddr_config_channel_id = 0;
					dmc_change_channel(dmc_ddr_config_channel_id);
				} else {
					do_ddr_display_c2_ddr_information((cmd_tbl_t *)cmdtp, (int)flag, (int)argc2, (argv2));
				}
			} else if ((p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T7)) {
				dmc_ddr_config_channel_id = 0;
				dmc_change_channel(dmc_ddr_config_channel_id);
				do_ddr_display_g12_ddr_information((cmd_tbl_t *)cmdtp, (int)flag, (int)argc2, (argv2));
				dmc_ddr_config_channel_id = 1;
				dmc_change_channel(dmc_ddr_config_channel_id);
				do_ddr_display_g12_ddr_information((cmd_tbl_t *)cmdtp, (int)flag, (int)argc2, (argv2));
				dmc_ddr_config_channel_id = 0;
				dmc_change_channel(dmc_ddr_config_channel_id);
			} else {
				do_ddr_display_g12_ddr_information((cmd_tbl_t *)cmdtp, (int)flag, (int)argc2, (argv2));
			}
		}
		break;

		case (DDR_TEST_CMD__DDR_G12_DMC_TEST):
			break;
		case (DDR_TEST_CMD__DDR_G12_EE_BDLR_TEST):
		{
			printf("\nUboot BDLR test \n");

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

U_BOOT_CMD(
	ddr_test_cmd, 30, 1, do_ddr_test_cmd,
	"ddr_test_cmd cmd arg1 arg2 arg3...",
	"ddr_test_cmd cmd arg1 arg2 arg3... \n dcache off ? \n"
	);

#ifdef ENABLE_OLD_EXTRA_TEST_CMD
int do_ddr_auto_test_window(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	check_base_address();
	global_boot_times = rd_reg(p_ddr_base->preg_sticky_reg0);
	printf("\nglobal_boot_times== %d\n", global_boot_times);

	printf("\nargc== 0x%08x\n", argc);
	printf("\nargc== 0x%08x\n", argc);
	int i;
	for (i = 0; i < argc; i++)
		printf("\nargv[%d]=%s\n", i, argv[i]);

	char str[1024] = "";
	unsigned int ddr_test_cmd = 0;
	unsigned int temp_reg_add = 0;
	unsigned int num_arry[32] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	temp_reg_add = (((p_ddr_base->ddr_dmc_sticky0)));
	for (i = 0; i < 32; i++) {
		num_arry[i] = ddr_rd_8_16bit_on_32reg(temp_reg_add, 8, i);
		if ((i == 0) || (i == 16))
			printf("\n numarry[%d]", i);
		printf(" %d ", num_arry[i]);
	}

	ddr_test_cmd = num_arry[0];

	unsigned int cs0_test_start = 0x1080000;
	unsigned int cs0_test_size = DDR_CORSS_TALK_TEST_SIZE;
	unsigned int cs1_test_start = 0;
	unsigned int cs1_test_size = 0;
	unsigned int watchdog_time_s = 0;
	unsigned int test_index_enable = 0;
	unsigned int all_toghter_enable = 0;
	cs0_test_start = ((num_arry[TEST_ARG_CS0_TEST_START_INDEX]) | ((num_arry[TEST_ARG_CS0_TEST_START_INDEX + 1]) << 8) |
			  ((num_arry[TEST_ARG_CS0_TEST_START_INDEX + 2]) << 16) | ((num_arry[TEST_ARG_CS0_TEST_START_INDEX + 3]) << 24));
	cs0_test_size = ((num_arry[TEST_ARG_CS0_TEST_SIZE_INDEX]) | ((num_arry[TEST_ARG_CS0_TEST_SIZE_INDEX + 1]) << 8) |
			 ((num_arry[TEST_ARG_CS0_TEST_SIZE_INDEX + 2]) << 16) | ((num_arry[TEST_ARG_CS0_TEST_SIZE_INDEX + 3]) << 24));
	cs1_test_start = ((num_arry[TEST_ARG_CS1_TEST_START_INDEX]) | ((num_arry[TEST_ARG_CS1_TEST_START_INDEX + 1]) << 8) |
			  ((num_arry[TEST_ARG_CS1_TEST_START_INDEX + 2]) << 16) | ((num_arry[TEST_ARG_CS1_TEST_START_INDEX + 3]) << 24));
	cs1_test_size = ((num_arry[TEST_ARG_CS1_TEST_SIZE_INDEX]) | ((num_arry[TEST_ARG_CS1_TEST_SIZE_INDEX + 1]) << 8) |
			 ((num_arry[TEST_ARG_CS1_TEST_SIZE_INDEX + 2]) << 16) | ((num_arry[TEST_ARG_CS1_TEST_SIZE_INDEX + 3]) << 24));
	watchdog_time_s = ((num_arry[TEST_ARG_WATCHDOG_TIME_SIZE_INDEX]) | ((num_arry[TEST_ARG_WATCHDOG_TIME_SIZE_INDEX + 1]) << 8));
	test_index_enable = ((num_arry[TEST_ARG_TEST_INDEX_ENALBE_INDEX]) | ((num_arry[TEST_ARG_TEST_INDEX_ENALBE_INDEX + 1]) << 8));
	all_toghter_enable = (num_arry[TEST_ARG_3_ALL_TOGHTER]);
	switch (ddr_test_cmd) {
	case (DMC_STICKY_UBOOT_WINDOW_MAGIC_1):
		if (num_arry[1] == DMC_STICKY_UBOOT_WINDOW_MAGIC_1) {
			sprintf(str, "ddr_test_cmd 0x27 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x \
			0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x  0x%08x", cs0_test_start, cs0_test_size, cs1_test_start, cs1_test_size,
				watchdog_time_s, test_index_enable, 0, 0, 0, 0, 0, 0, all_toghter_enable);

			printf("\nstr=%s\n", str);

			run_command(str, 0);
			break;
		}
	}

	return 1;
}

U_BOOT_CMD(
	ddr_auto_test_window, 30, 1, do_ddr_auto_test_window,
	"ddr_test_cmd cmd arg1 arg2 arg3...",
	"ddr_test_cmd cmd arg1 arg2 arg3... \n dcache off ? \n"
	);

int do_ddr_auto_scan_drv(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	check_base_address();
#define  AUTO_SCAN_DDR3  0
#define  AUTO_SCAN_DDR4  1
#define  AUTO_SCAN_LPDDR3  2
#define  AUTO_SCAN_LPDDR4  3

#define  AUTO_SCAN_CONFIG_RANK0  0
#define  AUTO_SCAN_CONFIG_RANK01  1

	char *string_print_flag = " window-loop \n";
	global_boot_times = rd_reg((p_ddr_base->preg_sticky_reg0));

	printf("\nargc== 0x%08x\n", argc);
	printf("\nargc== 0x%08x\n", argc);
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
	unsigned int temp_reg_add = 0;
	{
		temp_reg_add = (((p_ddr_base->ddr_dmc_sticky0)));
	}

	char str[1024] = "";

	unsigned int counter_loop = 0;
	unsigned int ddr_frequency = 0;
	unsigned int soc_data_drv_ohm_p = 0;    //74  //config soc data pin pull up driver strength,select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned int soc_data_drv_ohm_n = 0;    //76
	unsigned int soc_data_odt_ohm_p = 0;    //78  //config soc data pin odt pull up strength,select 0,28,30,32,34,37,40,43,48,53,60,68,80,96,120ohm
	unsigned int soc_data_odt_ohm_n = 0;    //80
	unsigned int dram_data_drv_ohm = 0;     //82  //config dram data pin pull up pull down driver strength,ddr3 select 34,40ohm,ddr4 select 34,48ohm,lpddr4 select 40,48,60,80,120,240ohm
	unsigned int dram_data_odt_ohm = 0;     //84  //config dram data pin odt pull up down strength,ddr3 select 40,60,120ohm,ddr4 select 34,40,48,60,120,240ohm,lpddr4 select 40,48,60,80,120,240ohm
	unsigned int dram_data_wr_odt_ohm = 0;  //174 char 1
	i = 74 / 2;
	soc_data_drv_ohm_p = ddr_rd_8_16bit_on_32reg(temp_reg_add, 16, i);
	i = 76 / 2;
	soc_data_drv_ohm_n = ddr_rd_8_16bit_on_32reg(temp_reg_add, 16, i);
	i = 78 / 2;
	soc_data_odt_ohm_p = ddr_rd_8_16bit_on_32reg(temp_reg_add, 16, i);
	i = 80 / 2;
	soc_data_odt_ohm_n = ddr_rd_8_16bit_on_32reg(temp_reg_add, 16, i);
	i = 82 / 2;
	dram_data_drv_ohm = ddr_rd_8_16bit_on_32reg(temp_reg_add, 16, i);
	i = 84 / 2;
	dram_data_odt_ohm = ddr_rd_8_16bit_on_32reg(temp_reg_add, 16, i);
	i = 174 / 1;
	dram_data_wr_odt_ohm = ddr_rd_8_16bit_on_32reg(temp_reg_add, 8, i);
	i = 52 / 2;
	ddr_frequency = ddr_rd_8_16bit_on_32reg(temp_reg_add, 16, i);
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

	unsigned int max_counter_loop_w1 = (sizeof(soc_data_drv_ohm_p_t)) / (sizeof(soc_data_drv_ohm_p_t[0]));
	unsigned int max_counter_loop_w2 = (sizeof(dram_data_odt_ohm_t_ddr3)) / (sizeof(dram_data_odt_ohm_t_ddr3[0]));
	unsigned int max_counter_loop_r1 = (sizeof(dram_data_drv_ohm_t_ddr3)) / (sizeof(dram_data_drv_ohm_t_ddr3[0]));
	unsigned int max_counter_loop_r2 = (sizeof(soc_data_odt_ohm_p_t)) / (sizeof(soc_data_odt_ohm_p_t[0]));
	unsigned int max_counter_loop_wr1 = 1;
	if (ddr_channel_rank_config)
		max_counter_loop_wr1 = (sizeof(dram_data_wr_odt_ohm_t_ddr3)) / (sizeof(dram_data_wr_odt_ohm_t_ddr3[0]));
	if (ddr_type == AUTO_SCAN_DDR4) {
		p_dram_data_drv_ohm = dram_data_drv_ohm_t_ddr4;
		p_dram_data_odt_ohm = dram_data_odt_ohm_t_ddr4;
		p_dram_data_wr_odt_ohm = dram_data_wr_odt_ohm_t_ddr4;
		max_counter_loop_w2 = (sizeof(dram_data_odt_ohm_t_ddr4)) / (sizeof(dram_data_odt_ohm_t_ddr4[0]));
		max_counter_loop_r1 = (sizeof(dram_data_drv_ohm_t_ddr4)) / (sizeof(dram_data_drv_ohm_t_ddr4[0]));

		if (ddr_channel_rank_config)
			max_counter_loop_wr1 = (sizeof(dram_data_wr_odt_ohm_t_ddr4)) / (sizeof(dram_data_wr_odt_ohm_t_ddr4[0]));
	}
	if (ddr_type == AUTO_SCAN_LPDDR4) {
		p_dram_data_drv_ohm = dram_data_drv_ohm_t_lpddr4;
		p_dram_data_odt_ohm = dram_data_odt_ohm_t_lpddr4;
		max_counter_loop_w2 = (sizeof(dram_data_odt_ohm_t_lpddr4)) / (sizeof(dram_data_odt_ohm_t_lpddr4[0]));
		max_counter_loop_r1 = (sizeof(dram_data_drv_ohm_t_lpddr4)) / (sizeof(dram_data_drv_ohm_t_lpddr4[0]));
		max_counter_loop_r2 = (sizeof(soc_data_odt_ohm_n_t)) / (sizeof(soc_data_odt_ohm_n_t[0]));

		if (ddr_channel_rank_config)
			max_counter_loop_wr1 = 1;
	}

	unsigned int max_counter_total = (max_counter_loop_w1 * max_counter_loop_w2 + max_counter_loop_r1 * max_counter_loop_r2 + 2) * max_counter_loop_wr1;
	//add 2 times recover
	//each arrary test 2 times ,for maybe 1times will fail,then next time will recovery //jiaxing 20181114
	counter_loop = (((global_boot_times - 1) / 2) % max_counter_total);
	dram_data_wr_odt_ohm = 0;
	if (max_counter_loop_wr1 > 1)
		dram_data_wr_odt_ohm = p_dram_data_wr_odt_ohm[(counter_loop / (max_counter_loop_w1 * max_counter_loop_w2 + max_counter_loop_r1 * max_counter_loop_r2 + 2))];
	if ((counter_loop % (max_counter_loop_w1 * max_counter_loop_w2 + max_counter_loop_r1 * max_counter_loop_r2 + 2)) < (max_counter_loop_w1 * max_counter_loop_w2)) {
		soc_data_drv_ohm_p = p_soc_data_drv_ohm_p[(((counter_loop % (max_counter_loop_w1 * max_counter_loop_w2 + max_counter_loop_r1 * max_counter_loop_r2 + 2))) % max_counter_loop_w1)];
		soc_data_drv_ohm_n = soc_data_drv_ohm_p;
		dram_data_odt_ohm = p_dram_data_odt_ohm[(((counter_loop % (max_counter_loop_w1 * max_counter_loop_w2 + max_counter_loop_r1 * max_counter_loop_r2 + 2))) / max_counter_loop_w1) % max_counter_loop_w2];
	} else if ((counter_loop % (max_counter_loop_w1 * max_counter_loop_w2 + max_counter_loop_r1 * max_counter_loop_r2 + 2)) == ((max_counter_loop_w1 * max_counter_loop_w2) + 0)) {
		ddr_test_watchdog_reset_system();
	} else if ((counter_loop % (max_counter_loop_w1 * max_counter_loop_w2 + max_counter_loop_r1 * max_counter_loop_r2 + 2)) < (max_counter_loop_w1 * max_counter_loop_w2 + max_counter_loop_r1 * max_counter_loop_r2 + 2 - 1)) {
		dram_data_drv_ohm = p_dram_data_drv_ohm[((((counter_loop % (max_counter_loop_w1 * max_counter_loop_w2 + max_counter_loop_r1 * max_counter_loop_r2 + 2))) - (max_counter_loop_w1 * max_counter_loop_w2 + 1)) % max_counter_loop_r1)];
		soc_data_odt_ohm_p = p_soc_data_odt_ohm_p[((((counter_loop % (max_counter_loop_w1 * max_counter_loop_w2 + max_counter_loop_r1 * max_counter_loop_r2 + 2))) - (max_counter_loop_w1 * max_counter_loop_w2 + 1)) / max_counter_loop_r1)];
		soc_data_odt_ohm_n = 0;
		if (ddr_type == CONFIG_DDR_TYPE_LPDDR4) {
			soc_data_odt_ohm_p = 0;
			soc_data_odt_ohm_n = p_soc_data_odt_ohm_n[((((counter_loop % (max_counter_loop_w1 * max_counter_loop_w2 + max_counter_loop_r1 * max_counter_loop_r2 + 2))) - (max_counter_loop_w1 * max_counter_loop_w2 + 1)) / max_counter_loop_r1)];
		}
	} else if ((counter_loop % (max_counter_loop_w1 * max_counter_loop_w2 + max_counter_loop_r1 * max_counter_loop_r2 + 2)) == (max_counter_loop_w1 * max_counter_loop_w2 + max_counter_loop_r1 * max_counter_loop_r2 + 2 - 1)) {
		ddr_test_watchdog_reset_system();
	}
	printf("\nglobal_boot_times== %d %s", global_boot_times, string_print_flag);

	if (loop)
		if (((global_boot_times - 1) / 2) > max_counter_total)
			return 1;
	printf("\nmax_counter=%d  %d %s", max_counter_total, max_counter_total * 2, string_print_flag);
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

		sprintf(str, "g12_d2pll %d  0x12  0x6 0 0x0 0 0 0 0x800000    0 1   ", ddr_frequency);
		printf("\nstr=%s\n", str);
		run_command(str, 0);
	}

	return 1;
}
#endif

unsigned char temp_sha2[SHA256_SUM_LEN];
int do_verify_flash_ddr_parameter(char log_level)
{
	unsigned count = 0;
	unsigned error = 0;
	unsigned ret = 0;

	if (((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_C2) &&
	(p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_T5D)) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S4) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5W)) {
		char temp_buf[((sizeof(ddr_sha_t_c2) + 511) / 512) * 512] = { 0 };
		unsigned ddr_set_size = 0;
		ddr_set_size = sizeof(ddr_sha_t_c2);
		#if 1
		uint32_t loop = 0;
		//for C2 T5/T5D ddr window fast boot no support dfe vref1
		if (((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_C2) &&
		(p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_T5D)) ||
			(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5W)) {
			ddr_set_size = sizeof(ddr_set_t_c2) - (44 * 2); //sizeof(board_phase_setting_ps_t.soc_bit_vref_dac1);
			unsigned char *sha_t_chip_id;
			sha_t_chip_id = (unsigned char *)((uint64_t)(&(ddr_sha_c2.sha_chip_id)) - (44 * 2));
			for (loop = 0; loop < MESON_CPU_CHIP_ID_SIZE; loop++)   //update chip id
				ddr_sha_c2.sha_chip_id[loop] = sha_t_chip_id[loop];
		}
		#endif

#ifdef USE_FOR_UBOOT_2018
		extern int store_rsv_read(const char *name, size_t size, void *buf);

		ret = store_rsv_read("ddr-parameter", ((sizeof(ddr_sha_t_c2) + 511) / 512) * 512, (uint8_t *)(temp_buf));

#else
		extern int store_ddr_parameter_read(uint8_t *buffer, uint32_t length);

		ret = store_ddr_parameter_read((uint8_t *)(temp_buf), ((sizeof(ddr_sha_t_c2) + 511) / 512) * 512);
#endif
		char *s = temp_buf;
		char *d = (char *)(&ddr_sha_c2);

		for (count = 0; count < sizeof(ddr_sha_t_c2); count++) {
			*d = *s;
			s++;
			d++;
		}

		for (count = 0; count < SHA256_SUM_LEN; count++)
			((temp_sha2[count]) = (ddr_sha_c2.sha2[count]));

		sha256_csum_wd_internal((uint8_t *)(&(ddr_sha_c2.ddrs)), ddr_set_size, ddr_sha_c2.sha2, 0);

		for (count = 0; count < SHA256_SUM_LEN; count++) {
			if ((temp_sha2[count]) != (ddr_sha_c2.sha2[count]))
				error++;

			if (log_level == 0)
				printf("\nread sha[%08x] %08x,calu %08x", count, (ddr_sha_c2.sha2[count]), (temp_sha2[count]));
		}

#define DDR_FUNC_FAST_BOOT_CHECK_CHIP_ID    (1 << 30)
		//	if ((ddr_sha_c2.ddrs.cfg_board_common_setting.ddr_func) & DDR_FUNC_FAST_BOOT_CHECK_CHIP_ID)
		{
			for (count = 0; count < MESON_CPU_CHIP_ID_SIZE; count++) {
				if ((ddr_sha_c2.sha_chip_id[count]) != (global_chip_id[count]))
					error++;

				if (log_level == 0)
					printf("\nglobal_chip_id[%08x] %08x,read %08x", count, (global_chip_id[count]), (ddr_sha_c2.sha_chip_id[count]));
			}
		}

		return error;
	}
	char temp_buf[((sizeof(ddr_sha_t) + 511) / 512) * 512] = { 0 };

#ifdef USE_FOR_UBOOT_2018
	extern int store_rsv_read(const char *name, size_t size, void *buf);

	ret = store_rsv_read("ddr-parameter", ((sizeof(ddr_sha_t) + 511) / 512) * 512, (uint8_t *)(temp_buf));


#else
	extern int store_ddr_parameter_read(uint8_t *buffer, uint32_t length);

	ret = store_ddr_parameter_read((uint8_t *)(temp_buf), ((sizeof(ddr_sha_t) + 511) / 512) * 512);
#endif
	if (ret)
		printf("\nstore_rsv_read ret=%d", ret);
	char *s = temp_buf;
	char *d = (char *)(&ddr_sha);

	for (count = 0; count < sizeof(ddr_sha_t); count++) {
		*d = *s;
		s++;
		d++;
	}

	for (count = 0; count < SHA256_SUM_LEN; count++)
		((temp_sha2[count]) = (ddr_sha.sha2[count]));

	sha256_csum_wd_internal((uint8_t *)(&(ddr_sha.ddrs)), sizeof(ddr_set_t), ddr_sha.sha2, 0);

	for (count = 0; count < SHA256_SUM_LEN; count++) {
		if ((temp_sha2[count]) != (ddr_sha.sha2[count]))
			error++;

		if (log_level == 0)
			printf("\nread sha[%08x] %08x,calu %08x", count, (ddr_sha.sha2[count]), (temp_sha2[count]));
	}

#define DDR_FUNC_FAST_BOOT_CHECK_CHIP_ID    (1 << 30)

	if ((ddr_sha.ddrs.ddr_func) & DDR_FUNC_FAST_BOOT_CHECK_CHIP_ID) {
		for (count = 0; count < MESON_CPU_CHIP_ID_SIZE; count++) {
			if ((ddr_sha.sha_chip_id[count]) != (global_chip_id[count]))
				error++;

			if (log_level == 0)
				printf("\nglobal_chip_id[%08x] %08x,read %08x", count, (global_chip_id[count]), (ddr_sha.sha_chip_id[count]));
		}
	}
	return error;
} /* do_verify_flash_ddr_parameter */

uint32_t enable_ddr_check_boot_reason = 0;
uint32_t boot_reason = 0;
#define HOT_BOOT_MAGIC         0x99887766
#define HOT_BOOT_STICKY_ADD    (p_ddr_base->preg_sticky_reg0 + (6 << 2))
#define AMLOGIC_COLD_BOOT      0
int do_ddr_auto_fastboot_check_c2(char auto_window_test_enable_item,
	uint32_t auto_window_test_dq_size,
	char pattern_dis_scramble,
	uint32_t stick_dmc_ddr_window_test_read_vref_offset_value,
	uint32_t skip_window_test_enable)
{
	uint32_t ddr_set_size = 0;
	uint32_t need_ddr_window_test = 0;
	char str[1024] = "";
	int verify_error = 0;

	verify_error = do_verify_flash_ddr_parameter(1);

	if ((verify_error) == 0) {
		if ((ddr_sha_c2.ddrs.cfg_board_common_setting.fast_boot[0]) == 0xff) {
			printf("\nuboot  auto fast boot check flash data is ok return \n");
			return 1;
		}
	}
	enable_ddr_check_boot_reason = 0;
	enable_ddr_check_boot_reason = env_to_a_num("enable_ddr_check_boot_reason");
	printf("\nenable_ddr_check_boot_reason==%d \n", enable_ddr_check_boot_reason);
	printf("\nddr_fast_boot_enable_flag==%d \n", env_to_a_num("ddr_fast_boot_enable_flag"));
	ddr_set_t_c2 *ddr_set_t_p = NULL;
	ddr_set_t_p = (ddr_set_t_c2 *)(ddr_set_t_p_arrary_c2);
	uint32_t ddr_set_add = 0;

	ddr_set_add = (uint32_t)(uint64_t)(ddr_set_t_p);
	ddr_set_size = sizeof(ddr_set_t_c2);
	printf("\nddr_set_t_p==0x%08x\n", ddr_set_add);

	uint32_t dmc_retraining_ctrl = 0;
	dmc_retraining_ctrl = rd_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address));
	wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address),
	dmc_retraining_ctrl & (~(1 << 31)));
	uint32_t write_size = 0;

	do_read_c2_ddr_training_data(1, ddr_set_t_p);
	#if 1
	uint32_t loop = 0;
	//for C2 T5/T5D ddr window fast boot no support dfe vref1
	if (((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_C2) &&
	(p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_T5D)) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5W)) {
		//sizeof(board_phase_setting_ps_t.soc_bit_vref_dac1);
		ddr_set_size = sizeof(ddr_set_t_c2) - (44 * 2);
		unsigned char *sha_t_chip_id;
		sha_t_chip_id = (unsigned char *)((uint64_t)(&(ddr_sha_c2.sha_chip_id)) - (44 * 2));
		for (loop = 0; loop < MESON_CPU_CHIP_ID_SIZE; loop++)           //update chip id
			sha_t_chip_id[loop] = global_chip_id[loop];
	}
	#endif
	write_size = ((ddr_set_size + SHA256_SUM_LEN +
	MESON_CPU_CHIP_ID_SIZE + 511) / 512) * 512;

	if (((ddr_set_t_p->cfg_board_common_setting.fast_boot[3]) & 0xc0) &&
	((ddr_set_t_p->cfg_board_common_setting.fast_boot[3]) & 0x3f)) {
		enable_ddr_check_boot_reason = 0;

		if (((ddr_set_t_p->cfg_board_common_setting.fast_boot[0]) > 0) &&
		((ddr_set_t_p->cfg_board_common_setting.fast_boot[0]) < 0xff)) {
			(ddr_set_t_p->cfg_board_common_setting.fast_boot[0]) = 0xfe;
			(verify_error) = 0;
		}
	}

	if ((ddr_set_t_p->cfg_board_common_setting.fast_boot[0])) {
		printf("\nuboot enable auto fast boot function \n");

		if ((verify_error)) {
			printf("\nuboot  auto fast boot check verify data happen wrong \n");
			(ddr_set_t_p->cfg_board_common_setting.fast_boot[0]) = 1;
		}
	} else {
		wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
		return 1;
	}

	if (enable_ddr_check_boot_reason) {
		boot_reason = 0;         //ddr_set_t_p->boot_reason
		{
			if ((((readl(p_ddr_base->ddr_boot_reason_address) >> 0) & 0xf) == AMLOGIC_COLD_BOOT) && ((rd_reg(HOT_BOOT_STICKY_ADD)) == HOT_BOOT_MAGIC))
				boot_reason = 1;

			if (ddr_set_t_p->cfg_board_common_setting.fast_boot[0] == 0xfe)
				boot_reason = 2;
		}
		printf("\nboot_reason=%08x \n", (boot_reason));
		printf("\nHOT_BOOT_STICKY_ADD=%08x \n", (rd_reg(HOT_BOOT_STICKY_ADD)));
		wr_reg(HOT_BOOT_STICKY_ADD, HOT_BOOT_MAGIC);
		printf("\nHOT_BOOT_STICKY_ADD=%08x \n", (rd_reg(HOT_BOOT_STICKY_ADD)));

		if (boot_reason == 0) {
			if ((ddr_set_t_p->cfg_board_common_setting.fast_boot[0]) < 0xfe) {
				need_ddr_window_test = 1;
				sprintf(str, "setenv initargs ${initargs} need_ddr_window_test=%d", need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
				sprintf(str, "setenv bootargs ${bootargs} need_ddr_window_test=%d", need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
			} else {
				sprintf(str, "setenv initargs ${initargs} need_ddr_window_test=%d", need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
				sprintf(str, "setenv bootargs ${bootargs} need_ddr_window_test=%d", need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
			}
			wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
			return 1;
		} else {
			sprintf(str, "setenv initargs ${initargs} need_ddr_window_test=%d", need_ddr_window_test);
			printf("\nstr=%s\n", str);
			run_command(str, 0);
			sprintf(str, "setenv bootargs ${bootargs} need_ddr_window_test=%d", need_ddr_window_test);
			printf("\nstr=%s\n", str);
			run_command(str, 0);
		}
	}

	if ((ddr_set_t_p->cfg_board_common_setting.fast_boot[0]) == 0xff) {
		printf("\nuboot  auto fast boot  auto window test is done \n");
		wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
		return 1;
	}

	if (skip_window_test_enable) {
		printf("enable skip window test fast boot mode! \n");
		ddr_set_t_p->cfg_board_common_setting.fast_boot[0] = 0xfe;
	}
	printf("\n(ddr_set_t_p->cfg_board_common_setting.fast_boot[0])==0x%08x\n", (ddr_set_t_p->cfg_board_common_setting.fast_boot[0]));

	if ((ddr_set_t_p->cfg_board_common_setting.fast_boot[0]) < 0xfe) {
		printf("\nuboot  auto fast boot  auto window test begin \n");
		{
			ddr_set_t_p->cfg_board_common_setting.fast_boot[0] = 0xfd;             //0xfd for check unexcept power off status
			sha256_csum_wd_internal((unsigned char *)(uint64_t)ddr_set_add, ddr_set_size, ddr_sha_c2.sha2, 0);
			write_size = ((ddr_set_size + SHA256_SUM_LEN + MESON_CPU_CHIP_ID_SIZE + 511) / 512) * 512;
			{
				ddr_do_store_ddr_parameter_ops((uint8_t *)(unsigned long)(ddr_set_add - SHA256_SUM_LEN), write_size);
			}
			sprintf(str, "g12_d2pll %d 0x11 %d 0 0 0 0 %d 0x%08x  0 %d", ddr_set_t_p->cfg_board_SI_setting_ps[0].DRAMFreq, auto_window_test_enable_item, stick_dmc_ddr_window_test_read_vref_offset_value, auto_window_test_dq_size, pattern_dis_scramble);
			printf("\nstr=%s\n", str);

			run_command(str, 0);
		}
		return 1;
	}

	if ((ddr_set_t_p->cfg_board_common_setting.fast_boot[0]) == 0xfe) {
#if 0
		char dmc_test_worst_window_rx = 0;
		char dmc_test_worst_window_tx = 0;

		{
			dwc_ddrphy_apb_wr((0 << 20) | (0xd << 16) | (0 << 12) | (0x0), 0);             // DWC_DDRPHYA_APBONLY0_MicroContMuxSel

			dmc_test_worst_window_tx = dwc_ddrphy_apb_rd((0 << 20) | (1 << 16) | (0 << 12) | (0x0c2));
			dmc_test_worst_window_rx = dwc_ddrphy_apb_rd((0 << 20) | (1 << 16) | (0 << 12) | (0x0c3));
			printf("\ndmc_test_worst_window_tx =%d \n", dmc_test_worst_window_tx);
			printf("\ndmc_test_worst_window_rx=%d \n", dmc_test_worst_window_rx);

			if (dmc_test_worst_window_tx > 30)
				dmc_test_worst_window_tx = 30;

			if (dmc_test_worst_window_rx > 30)
				dmc_test_worst_window_rx = 30;
			ddr_set_t_p->fast_boot[1] = (((dmc_test_worst_window_tx / 2) << 4)) | (((dmc_test_worst_window_rx / 2)));
		}
#endif
		ddr_set_t_p->cfg_board_common_setting.fast_boot[0] = 0xff;
		{
			printf("\nuboot  auto fast boot  auto window test finish \n");

			if (ddr_set_t_p->cfg_board_common_setting.fast_boot[2]) {
				if ((ddr_set_t_p->cfg_board_common_setting.fast_boot[2]) & 0x7) {
					if (((ddr_set_t_p->cfg_board_common_setting.fast_boot[2]) & 0x8))
						sprintf(str, "ddr_g12_offset_data 3 0x0  0 0 1 %d ", (ddr_set_t_p->cfg_board_common_setting.fast_boot[2]) & 0x7);
					else
						sprintf(str, "ddr_g12_offset_data 3 0x0  0 0 2 %d ", (ddr_set_t_p->cfg_board_common_setting.fast_boot[2]) & 0x7);
					printf("\nstr=%s\n", str);

					run_command(str, 0);
				}

				if ((ddr_set_t_p->cfg_board_common_setting.fast_boot[2]) & 0x70) {
					if (((ddr_set_t_p->cfg_board_common_setting.fast_boot[2]) & 0x80))
						sprintf(str, "ddr_g12_offset_data 2 0x0  0 0 1 %d ", ((ddr_set_t_p->cfg_board_common_setting.fast_boot[2]) >> 4) & 0x7);
					else
						sprintf(str, "ddr_g12_offset_data 2 0x0  0 0 2 %d ", ((ddr_set_t_p->cfg_board_common_setting.fast_boot[2]) >> 4) & 0x7);
					printf("\nstr=%s\n", str);

					run_command(str, 0);
				}
				sprintf(str, "ddr_fast_boot 1 ");
				printf("\nstr=%s\n", str);
				run_command(str, 0);
			} else {
				sha256_csum_wd_internal((unsigned char *)(uint64_t)ddr_set_add, ddr_set_size, ddr_sha_c2.sha2, 0);
				ddr_do_store_ddr_parameter_ops((uint8_t *)(unsigned long)(ddr_set_add - SHA256_SUM_LEN), write_size);
			}

			if ((enable_ddr_check_boot_reason)) {
				if (boot_reason) {
					sprintf(str, "systemoff");
					printf("\nstr=%s\n", str);
					run_command(str, 0);
				}
			}
			wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
			return 1;
		}
	}
	wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
	return 1;
} /* do_ddr_auto_fastboot_check_c2 */

int do_ddr_auto_fastboot_check(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	check_base_address();
#define DMC_TEST_SLT_ENABLE_DDR_AUTO_FAST_BOOT (1 << 5)
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
		stick_dmc_ddr_window_test_read_vref_offset_value = simple_strtoull_ddr(argv[4], &endp, 0);
		if (*argv[4] == 0 || *endp != 0)
			stick_dmc_ddr_window_test_read_vref_offset_value = 0;
	}
	if (argc > 5) {
		skip_window_test_enable = simple_strtoull_ddr(argv[5], &endp, 0);
		if (*argv[5] == 0 || *endp != 0)
			skip_window_test_enable = 0;
	}
	if (((p_ddr_base->chip_id >= MESON_CPU_MAJOR_ID_C2) &&
	(p_ddr_base->chip_id <= MESON_CPU_MAJOR_ID_T5D)) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_S4) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T3) ||
	(p_ddr_base->chip_id == MESON_CPU_MAJOR_ID_T5W)) {
		do_ddr_auto_fastboot_check_c2(
		auto_window_test_enable_item,
		auto_window_test_dq_size,
		pattern_dis_scramble,
		stick_dmc_ddr_window_test_read_vref_offset_value,
		skip_window_test_enable);
		return 1;
	}
	char str[1024] = "";
	int verify_error = 0;
	verify_error = do_verify_flash_ddr_parameter(1);
	if ((verify_error) == 0) {
		if ((ddr_sha.ddrs.fast_boot[0]) == 0xff) {
			printf("\nuboot  auto fast boot check flash data is ok return \n");
			return 1;
		}
	}
	enable_ddr_check_boot_reason = 0;
	enable_ddr_check_boot_reason = env_to_a_num("enable_ddr_check_boot_reason");
	printf("\nenable_ddr_check_boot_reason==%d \n", enable_ddr_check_boot_reason);
	printf("\nddr_fast_boot_enable_flag==%d \n", env_to_a_num("ddr_fast_boot_enable_flag"));
	ddr_set_t *ddr_set_t_p = NULL;
	ddr_set_t_p = (ddr_set_t *)(ddr_set_t_p_arrary);
	uint32_t ddr_set_add = 0;

	ddr_set_add = (uint32_t)(uint64_t)(ddr_set_t_p);
	ddr_set_size = sizeof(ddr_set_t);
	printf("\nddr_set_t_p==0x%08x\n", ddr_set_add);

	uint32_t dmc_retraining_ctrl = 0;
	dmc_retraining_ctrl = rd_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address));
	wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl & (~(1 << 31)));
	uint32_t write_size = 0;
	write_size = ((ddr_set_size + SHA256_SUM_LEN + MESON_CPU_CHIP_ID_SIZE + 511) / 512) * 512;
	do_read_ddr_training_data(1, ddr_set_t_p);
	if (((ddr_set_t_p->fast_boot[3]) & 0xc0) && ((ddr_set_t_p->fast_boot[3]) & 0x3f)) {
		enable_ddr_check_boot_reason = 0;
		if (((ddr_set_t_p->fast_boot[0]) > 0) && ((ddr_set_t_p->fast_boot[0]) < 0xff)) {
			(ddr_set_t_p->fast_boot[0]) = 0xfe;
			(verify_error) = 0;
		}
	}
	if ((ddr_set_t_p->fast_boot[0])) {
		printf("\nuboot enable auto fast boot function \n");
		if ((verify_error)) {
			printf("\nuboot  auto fast boot check verify data happen wrong \n");
			(ddr_set_t_p->fast_boot[0]) = 1;
		}
	} else {
		wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
		return 1;
	}

	if (enable_ddr_check_boot_reason) {
		boot_reason = 0; //ddr_set_t_p->boot_reason
		{
			if ((((readl(p_ddr_base->ddr_boot_reason_address) >> 12) & 0xf) == AMLOGIC_COLD_BOOT) && ((rd_reg(HOT_BOOT_STICKY_ADD)) == HOT_BOOT_MAGIC))
				boot_reason = 1;
			if (ddr_set_t_p->fast_boot[0] == 0xfe)
				boot_reason = 2;
		}
		printf("\nboot_reason=%08x \n", (boot_reason));
		printf("\nHOT_BOOT_STICKY_ADD=%08x \n", (rd_reg(HOT_BOOT_STICKY_ADD)));
		wr_reg(HOT_BOOT_STICKY_ADD, HOT_BOOT_MAGIC);
		printf("\nHOT_BOOT_STICKY_ADD=%08x \n", (rd_reg(HOT_BOOT_STICKY_ADD)));

		if (boot_reason == 0) {
			if ((ddr_set_t_p->fast_boot[0]) < 0xfe) {
				need_ddr_window_test = 1;
				sprintf(str, "setenv initargs ${initargs} need_ddr_window_test=%d", need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
				sprintf(str, "setenv bootargs ${bootargs} need_ddr_window_test=%d", need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
			} else {
				sprintf(str, "setenv initargs ${initargs} need_ddr_window_test=%d", need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
				sprintf(str, "setenv bootargs ${bootargs} need_ddr_window_test=%d", need_ddr_window_test);
				printf("\nstr=%s\n", str);
				run_command(str, 0);
			}
			wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
			return 1;
		} else {
			sprintf(str, "setenv initargs ${initargs} need_ddr_window_test=%d", need_ddr_window_test);
			printf("\nstr=%s\n", str);
			run_command(str, 0);
			sprintf(str, "setenv bootargs ${bootargs} need_ddr_window_test=%d", need_ddr_window_test);
			printf("\nstr=%s\n", str);
			run_command(str, 0);
		}
	}
	if ((ddr_set_t_p->fast_boot[0]) == 0xff) {
		printf("\nuboot  auto fast boot  auto window test is done \n");
		wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
		return 1;
	}
	if (skip_window_test_enable) {
		printf("enable skip window test fast boot mode! \n");
		ddr_set_t_p->fast_boot[0] = 0xfe;
	}
	printf("\n(ddr_set_t_p->fast_boot[0])==0x%08x\n", (ddr_set_t_p->fast_boot[0]));

	if ((ddr_set_t_p->fast_boot[0]) < 0xfe) {
		printf("\nuboot  auto fast boot  auto window test begin \n");
		{
			ddr_set_t_p->fast_boot[0] = 0xfd; //0xfd for check unexcept power off status
			sha256_csum_wd_internal((unsigned char *)(uint64_t)ddr_set_add, sizeof(ddr_set_t), ddr_sha.sha2, 0);
			write_size = ((ddr_set_size + SHA256_SUM_LEN + MESON_CPU_CHIP_ID_SIZE + 511) / 512) * 512;
			{
				ddr_do_store_ddr_parameter_ops((uint8_t *)(unsigned long)(ddr_set_add - SHA256_SUM_LEN), write_size);
			}
			sprintf(str, "g12_d2pll %d 0x11 %d 0 0 0 0 %d 0x%08x  0 %d", ddr_set_t_p->DRAMFreq[0], auto_window_test_enable_item, stick_dmc_ddr_window_test_read_vref_offset_value, auto_window_test_dq_size, pattern_dis_scramble);
			printf("\nstr=%s\n", str);

			run_command(str, 0);
		}
		return 1;
	}

	if ((ddr_set_t_p->fast_boot[0]) == 0xfe) {
		char dmc_test_worst_window_rx = 0;
		char dmc_test_worst_window_tx = 0;

		{
			dwc_ddrphy_apb_wr((0 << 20) | (0xd << 16) | (0 << 12) | (0x0), 0); // DWC_DDRPHYA_APBONLY0_MicroContMuxSel

			dmc_test_worst_window_tx = dwc_ddrphy_apb_rd((0 << 20) | (1 << 16) | (0 << 12) | (0x0c2));
			dmc_test_worst_window_rx = dwc_ddrphy_apb_rd((0 << 20) | (1 << 16) | (0 << 12) | (0x0c3));
			printf("\ndmc_test_worst_window_tx =%d \n", dmc_test_worst_window_tx);
			printf("\ndmc_test_worst_window_rx=%d \n", dmc_test_worst_window_rx);
			if (dmc_test_worst_window_tx > 30)
				dmc_test_worst_window_tx = 30;
			if (dmc_test_worst_window_rx > 30)
				dmc_test_worst_window_rx = 30;
			ddr_set_t_p->fast_boot[1] = (((dmc_test_worst_window_tx / 2) << 4)) | (((dmc_test_worst_window_rx / 2)));
		}

		ddr_set_t_p->fast_boot[0] = 0xff;
		{
			printf("\nuboot  auto fast boot  auto window test finish \n");

			if (ddr_set_t_p->fast_boot[2]) {
				if ((ddr_set_t_p->fast_boot[2]) & 0x7) {
					if (((ddr_set_t_p->fast_boot[2]) & 0x8))
						sprintf(str, "ddr_g12_offset_data 3 0x0  0 0 1 %d ", (ddr_set_t_p->fast_boot[2]) & 0x7);
					else
						sprintf(str, "ddr_g12_offset_data 3 0x0  0 0 2 %d ", (ddr_set_t_p->fast_boot[2]) & 0x7);

					printf("\nstr=%s\n", str);

					run_command(str, 0);
				}
				if ((ddr_set_t_p->fast_boot[2]) & 0x70) {
					if (((ddr_set_t_p->fast_boot[2]) & 0x80))
						sprintf(str, "ddr_g12_offset_data 2 0x0  0 0 1 %d ", ((ddr_set_t_p->fast_boot[2]) >> 4) & 0x7);
					else
						sprintf(str, "ddr_g12_offset_data 2 0x0  0 0 2 %d ", ((ddr_set_t_p->fast_boot[2]) >> 4) & 0x7);

					printf("\nstr=%s\n", str);

					run_command(str, 0);
				}

				sprintf(str, "ddr_fast_boot 1 ");
				printf("\nstr=%s\n", str);
				run_command(str, 0);
			} else {
				sha256_csum_wd_internal((unsigned char *)(uint64_t)ddr_set_add, sizeof(ddr_set_t), ddr_sha.sha2, 0);
				ddr_do_store_ddr_parameter_ops((uint8_t *)(unsigned long)(ddr_set_add - SHA256_SUM_LEN), write_size);
			}
			if ((enable_ddr_check_boot_reason)) {
				if (boot_reason) {
					sprintf(str, "systemoff");
					printf("\nstr=%s\n", str);
					run_command(str, 0);
				}
			}
			wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
			return 1;
		}
	}
	wr_reg((p_ddr_base->ddr_dmc_lpdd4_retraining_address), dmc_retraining_ctrl);
	return 1;
}

#ifdef ENABLE_OLD_EXTRA_TEST_CMD
U_BOOT_CMD(
	ddr_auto_scan_drv, 30, 1, do_ddr_auto_scan_drv,
	"ddr_test_cmd cmd arg1 arg2 arg3...",
	"ddr_test_cmd cmd arg1 arg2 arg3... \n dcache off ? \n"
	);
#endif
U_BOOT_CMD(
	ddr_fast_boot, 30, 1, do_ddr_fastboot_config,
	"ddr_fastboot_config cmd arg1 arg2 arg3...",
	"ddr_fastboot_config cmd arg1 arg2 arg3... \n dcache off ? \n"
	);

U_BOOT_CMD(
	ddr_auto_fast_boot_check, 30, 1, do_ddr_auto_fastboot_check,
	"ddr_fastboot_config cmd arg1 arg2 arg3...",
	"ddr_fastboot_config cmd arg1 arg2 arg3... \n dcache off ? \n"
	);
