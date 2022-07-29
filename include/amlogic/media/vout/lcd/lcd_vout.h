/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef INC_AML_LCD_VOUT_H
#define INC_AML_LCD_VOUT_H

#include <common.h>
#include <linux/list.h>
#include <dm.h>
#include <asm/gpio.h>
#ifdef CONFIG_AML_LCD_TCON
#include <amlogic/media/vout/lcd/lcd_tcon_data.h>
#endif

/* **********************************
 * debug print define
 * ********************************** */
//#define LCD_DEBUG_INFO
extern unsigned int lcd_debug_print_flag;
#define LCD_DBG_PR_NORMAL       BIT(0)
#define LCD_DBG_PR_ADV          BIT(1)
#define LCD_DBG_PR_ADV2         BIT(2) //clk calc, tcon_data
#define LCD_DBG_PR_BL_NORMAL    BIT(4)
#define LCD_DBG_PR_BL_ADV       BIT(5) //pwm, isr, ext, ldim
#define LCD_DBG_PR_TEST         BIT(6)
#define LCD_DBG_PR_REG          BIT(7)

#define LCDPR(fmt, args...)     printf("lcd: "fmt"", ## args)
#define LCDERR(fmt, args...)    printf("lcd: error: "fmt"", ## args)

#define LCD_MAX_DRV             3

/* **********************************
 * clk parameter bit define
 * pll_ctrl, div_ctrl, clk_ctrl
 * ********************************** */
/* ******** pll_ctrl ******** */
#define PLL_CTRL_OD3                20 /* [21:20] */
#define PLL_CTRL_OD2                18 /* [19:18] */
#define PLL_CTRL_OD1                16 /* [17:16] */
#define PLL_CTRL_N                  9  /* [13:9] */
#define PLL_CTRL_M                  0  /* [8:0] */

/* ******** div_ctrl ******** */
#define DIV_CTRL_EDP_DIV1           24 /* [26:24] */
#define DIV_CTRL_EDP_DIV0           20 /* [23:20] */
#define DIV_CTRL_DIV_SEL            8 /* [15:8] */
#define DIV_CTRL_XD                 0 /* [7:0] */

/* ******** clk_ctrl ******** */
#define CLK_CTRL_LEVEL              28 /* [30:28] */
#define CLK_CTRL_FRAC_SHIFT         24 /* [24] */
#define CLK_CTRL_FRAC               0  /* [18:0] */

/* **********************************
 * VENC to TCON sync delay
 * ********************************** */
#define RGB_DELAY                   13
#define PRE_DE_DELAY                8

#define LCD_PINMUX_END          0xff
#define LCD_PINMUX_NUM          15

/* **********************************
 * global control define
 * ********************************** */
enum lcd_mode_e {
	LCD_MODE_TV = 0,
	LCD_MODE_TABLET,
	LCD_MODE_MAX,
};

enum lcd_chip_e {
	LCD_CHIP_G12A = 0,
	LCD_CHIP_G12B, 	/* 1 */
	LCD_CHIP_TL1,
	LCD_CHIP_SM1,
	LCD_CHIP_TM2,
	LCD_CHIP_T5,
	LCD_CHIP_T5D,
	LCD_CHIP_T7,
	LCD_CHIP_T3,
	LCD_CHIP_C3,
	LCD_CHIP_MAX,
};

enum lcd_type_e {
	LCD_RGB = 0,
	LCD_LVDS,
	LCD_VBYONE,
	LCD_MIPI,
	LCD_MLVDS,
	LCD_P2P,
	LCD_EDP,
	LCD_BT656,
	LCD_BT1120,
	LCD_TYPE_MAX,
};

#define MOD_LEN_MAX        30
struct lcd_basic_s {
	char model_name[MOD_LEN_MAX];
	enum lcd_type_e lcd_type;
	unsigned char lcd_bits;

	unsigned short h_active; /* Horizontal display area */
	unsigned short v_active; /* Vertical display area */
	unsigned short h_period; /* Horizontal total period time */
	unsigned short v_period; /* Vertical total period time */
	unsigned short h_period_min;
	unsigned short h_period_max;
	unsigned short v_period_min;
	unsigned short v_period_max;
	unsigned char frame_rate_min;
	unsigned char frame_rate_max;
	unsigned int lcd_clk_min;
	unsigned int lcd_clk_max;

	unsigned short screen_width;  /* screen physical width in "mm" unit */
	unsigned short screen_height; /* screen physical height in "mm" unit */
};

#define LCD_CLK_FRAC_UPDATE     (1 << 0)
#define LCD_CLK_PLL_CHANGE      (1 << 1)
struct lcd_timing_s {
	unsigned char clk_auto; /* clk parameters auto generation */
	unsigned char fr_adjust_type; /* 0=clock, 1=htotal, 2=vtotal */
	unsigned char clk_change; /* internal used */
	unsigned int lcd_clk;   /* pixel clock(unit: Hz) */
	unsigned int lcd_clk_dft; /* internal used */
	unsigned int h_period_dft; /* internal used */
	unsigned int v_period_dft; /* internal used */
	unsigned int pll_ctrl;  /* pll settings */
	unsigned int div_ctrl;  /* divider settings */
	unsigned int clk_ctrl;  /* clock settings */
	unsigned int bit_rate; /* Hz */

	unsigned int ss_level; /* [15:12]: ss_freq, [11:8]: ss_mode,
				* [7:0]: ss_level
				*/

	unsigned short sync_duration_num;
	unsigned short sync_duration_den;

	unsigned int hstart;
	unsigned int hend;
	unsigned int vstart;
	unsigned int vend;

	unsigned short hsync_width;
	unsigned short hsync_bp;
	unsigned short hsync_pol;
	unsigned short vsync_width;
	unsigned short vsync_bp;
	unsigned short vsync_pol;
	/* unsigned int vsync_h_phase; // [31]sign, [15:0]value */
	unsigned int h_offset;
	unsigned int v_offset;

	unsigned short de_hs_addr;
	unsigned short de_he_addr;
	unsigned short de_vs_addr;
	unsigned short de_ve_addr;

	unsigned short hs_hs_addr;
	unsigned short hs_he_addr;
	unsigned short hs_vs_addr;
	unsigned short hs_ve_addr;

	unsigned short vs_hs_addr;
	unsigned short vs_he_addr;
	unsigned short vs_vs_addr;
	unsigned short vs_ve_addr;
};

struct rgb_config_s {
	unsigned int type;
	unsigned int clk_pol;
	unsigned int de_valid;
	unsigned int sync_valid;
	unsigned int rb_swap;
	unsigned int bit_swap;
};

struct bt_config_s {
	unsigned int clk_phase;
	unsigned int field_type;
	unsigned int mode_422;
	unsigned int yc_swap;
	unsigned int cbcr_swap;
};

#define LVDS_PHY_VSWING_DFT        3
#define LVDS_PHY_PREEM_DFT         0
#define LVDS_PHY_CLK_VSWING_DFT    0
#define LVDS_PHY_CLK_PREEM_DFT     0
struct lvds_config_s {
	unsigned int lvds_repack;
	unsigned int dual_port;
	unsigned int pn_swap;
	unsigned int port_swap;
	unsigned int lane_reverse;
	unsigned int port_sel;
	unsigned int phy_vswing;
	unsigned int phy_preem;
	unsigned int phy_clk_vswing;
	unsigned int phy_clk_preem;
};

#define VX1_PHY_VSWING_DFT           3
#define VX1_PHY_PREEM_DFT            0

#define VX1_PWR_ON_RESET_DLY_DFT     500 /* 500ms */
#define VX1_HPD_DATA_DELAY_DFT       10 /* 10ms */
#define VX1_CDR_TRAINING_HOLD_DFT    200 /* 200ms */

struct vbyone_config_s {
	unsigned int lane_count;
	unsigned int region_num;
	unsigned int byte_mode;
	unsigned int color_fmt;
	unsigned int phy_div;
	unsigned int phy_vswing; /*[5:4]:ext_pullup, [3:0]vswing*/
	unsigned int phy_preem;
	unsigned int ctrl_flag;
		/* bit[0]:power_on_reset_en
		 * bit[1]:hpd_data_delay_en
		 * bit[2]:cdr_training_hold_en
		 * bit[3]:hw_filter_en
		 */

	/* ctrl timing */
	unsigned int power_on_reset_delay; /* ms */
	unsigned int hpd_data_delay; /* ms */
	unsigned int cdr_training_hold; /* ms */
	/* hw filter */
	unsigned int hw_filter_time;
	unsigned int hw_filter_cnt;
};

/* mipi-dsi config */
/* Operation mode parameters */
#define OPERATION_VIDEO_MODE     0
#define OPERATION_COMMAND_MODE   1

#define SYNC_PULSE               0x0
#define SYNC_EVENT               0x1
#define BURST_MODE               0x2

/* unit: kHz */
#define MIPI_BIT_RATE_MAX        1500000

/* command config */
#define DSI_CMD_SIZE_INDEX       1  /* byte[1] */
#define DSI_GPIO_INDEX           2  /* byte[2] */

#define DSI_INIT_ON_MAX          2600
#define DSI_INIT_OFF_MAX         30

struct dsi_config_s {
	unsigned char lane_num;
	unsigned int bit_rate_max; /* MHz */
	unsigned int clk_factor; /* bit_rate/pclk */
	unsigned int factor_numerator;
	unsigned int factor_denominator; /* 100 */
	unsigned char operation_mode_init; /* 0=video mode, 1=command mode */
	unsigned char operation_mode_display; /* 0=video mode, 1=command mode */
	unsigned char video_mode_type; /* 0=sync_pulse, 1=sync_event, 2=burst */
	unsigned char clk_always_hs; /* 0=disable, 1=enable */
	unsigned char phy_switch; /* 0=auto, 1=standard, 2=slow */

	unsigned int local_bit_rate_max; /* kHz */
	unsigned int local_bit_rate_min; /* kHz*/
	unsigned int venc_data_width;
	unsigned int dpi_data_format;
	unsigned int data_bits;

	unsigned char *dsi_init_on;
	unsigned char *dsi_init_off;
	unsigned char extern_init;

	unsigned char check_en;
	unsigned char check_reg;
	unsigned char check_cnt;
	unsigned char check_state;
};

#define EDP_EDID_STATE_LOAD     BIT(0)
#define EDP_EDID_STATE_APPLY    BIT(1)
#define EDP_EDID_RETRY_MAX      3
struct edp_config_s {
	unsigned char max_lane_count;
	unsigned char max_link_rate;
	unsigned char training_mode; /* 0=fast training, 1=auto training */
	unsigned char edid_en;
	unsigned char dpcd_caps_en;
	unsigned char sync_clk_mode;
	unsigned char scramb_mode;
	unsigned char enhanced_framing_en;
	unsigned char pn_swap;

	unsigned int phy_vswing;
	unsigned int phy_preem;

	/* internal used */
	unsigned char lane_count;
	unsigned char link_rate;
	unsigned int bit_rate;
	unsigned char edid_state;
	unsigned char edid_retry_cnt;
	unsigned char link_update;
	unsigned char training_settings;
	unsigned char main_stream_enable;

	unsigned char edid_data[128];
};

struct mlvds_config_s {
	unsigned int channel_num;
	unsigned int channel_sel0;
	unsigned int channel_sel1;
	unsigned int clk_phase; /* [13:12]=clk01_sel, [11:8]=pi2, [7:4]=pi1, [3:0]=pi0 */
	unsigned int pn_swap;
	unsigned int bit_swap; /* MSB/LSB reverse */
	unsigned int phy_vswing;
	unsigned int phy_preem;

	/* internal used */
	unsigned int pi_clk_sel; /* bit[9:0] */
};

enum p2p_type_e {
	P2P_CEDS = 0,
	P2P_CMPI,
	P2P_ISP,
	P2P_EPI,
	P2P_CHPI = 0x10, /* low common mode */
	P2P_CSPI,
	P2P_USIT,
	P2P_MAX,
};

struct p2p_config_s {
	unsigned int p2p_type;
	unsigned int lane_num;
	unsigned int channel_sel0;
	unsigned int channel_sel1;
	unsigned int pn_swap;
	unsigned int bit_swap; /* MSB/LSB reverse */
	unsigned int phy_vswing;
	unsigned int phy_preem;
};

struct phy_lane_s {
	unsigned int preem;
	unsigned int amp;
};

#define CH_LANE_MAX 32
struct phy_config_s {
	unsigned int flag;
	unsigned int vswing;
	unsigned int vcm;
	unsigned int odt;
	unsigned int ref_bias;
	unsigned int mode;
	unsigned int weakly_pull_down;
	struct phy_lane_s lane[CH_LANE_MAX];

	unsigned int lane_num;
	unsigned int vswing_level;
	unsigned int ext_pullup;
	unsigned int preem_level;
};

union lcd_ctrl_config_u {
	struct rgb_config_s rgb_cfg;
	struct bt_config_s bt_cfg;
	struct lvds_config_s lvds_cfg;
	struct vbyone_config_s vbyone_cfg;
	struct dsi_config_s mipi_cfg;
	struct edp_config_s edp_cfg;
	struct mlvds_config_s mlvds_cfg;
	struct p2p_config_s p2p_cfg;
};

/* **********************************
 * power control define
 * ********************************** */
enum lcd_power_type_e {
	LCD_POWER_TYPE_CPU = 0,
	LCD_POWER_TYPE_PMU,                 /* 1 */
	LCD_POWER_TYPE_SIGNAL,              /* 2 */
	LCD_POWER_TYPE_EXTERN,              /* 3 */
	LCD_POWER_TYPE_WAIT_GPIO,           /* 4 */
	LCD_POWER_TYPE_CLK_SS,              /* 5 */
	LCD_POWER_TYPE_TCON_SPI_DATA_LOAD,  /* 6 */
	LCD_POWER_TYPE_MAX,
};

enum lcd_pmu_gpio_e {
	LCD_PMU_GPIO0 = 0,
	LCD_PMU_GPIO1,
	LCD_PMU_GPIO2,
	LCD_PMU_GPIO3,
	LCD_PMU_GPIO4,
	LCD_PMU_GPIO_MAX,
};

#define LCD_CLK_SS_BIT_FREQ             0
#define LCD_CLK_SS_BIT_MODE             4

#define LCD_GPIO_MAX                    0xff
#define LCD_GPIO_OUTPUT_LOW             0
#define LCD_GPIO_OUTPUT_HIGH            1
#define LCD_GPIO_INPUT                  2

/* Power Control */
#define LCD_CPU_GPIO_NUM_MAX         10
#define LCD_CPU_GPIO_NAME_MAX        15
#define LCD_PMU_GPIO_NUM_MAX         3

#define LCD_PWR_STEP_MAX             15
struct lcd_power_step_s {
	unsigned char type;
	int index; /* point to lcd_cpu_gpio_s or lcd_pmu_gpio_s or lcd_extern */
	unsigned short value;
	unsigned short delay;
};

struct lcd_power_ctrl_s {
	char cpu_gpio[LCD_CPU_GPIO_NUM_MAX][LCD_CPU_GPIO_NAME_MAX];
	int *pmu_gpio;
	struct lcd_power_step_s power_on_step[LCD_PWR_STEP_MAX];
	struct lcd_power_step_s power_off_step[LCD_PWR_STEP_MAX];
};

#define LCD_PINMX_MAX              10
#define LCD_PINMUX_MAX             LCD_PINMX_MAX
#define BL_PINMUX_MAX              20
#define LCD_PINMUX_NAME_LEN_MAX    30
struct lcd_pinmux_ctrl_s {
	char *name;
	unsigned int pinmux_set[LCD_PINMUX_NUM][2];
	unsigned int pinmux_clr[LCD_PINMUX_NUM][2];
};

struct cus_ctrl_config_s {
	unsigned int flag;
	unsigned char dlg_flag;
};

#define LCD_ENABLE_RETRY_MAX    3
struct lcd_config_s {
	unsigned char retry_enable_flag;
	unsigned char retry_enable_cnt;
	unsigned char custom_pinmux;
	unsigned char fr_auto_dis;
	unsigned int backlight_index;
	struct lcd_basic_s basic;
	struct lcd_timing_s timing;
	union lcd_ctrl_config_u control;
	struct phy_config_s phy_cfg;
	struct cus_ctrl_config_s cus_ctrl;
	struct lcd_power_ctrl_s power;
	struct lcd_pinmux_ctrl_s *pinmux;
	unsigned int pinmux_set[LCD_PINMUX_NUM][2];
	unsigned int pinmux_clr[LCD_PINMUX_NUM][2];
};

struct lcd_duration_s {
	unsigned int frame_rate;
	unsigned int duration_num;
	unsigned int duration_den;
	unsigned int frac;
};

#define LCD_INIT_LEVEL_NORMAL         0
#define LCD_INIT_LEVEL_PWR_OFF        1
#define LCD_INIT_LEVEL_KERNEL_ON      2
/*
 *bit[31:20]: reserved
 *bit[19:18]: lcd_init_level
 *bit[17]: reserved
 *bit[16]: custom pinmux flag
 *bit[15:8]: advanced flag(p2p_type when lcd_type=p2p)
 *bit[7:4]: lcd bits
 *bit[3:0]: lcd_type
 */
struct lcd_boot_ctrl_s {
	unsigned char lcd_type;
	unsigned char lcd_bits;
	unsigned char advanced_flag;
	unsigned char custom_pinmux;
	unsigned char init_level;
};

/*
 *bit[31:30]: lcd mode(0=normal, 1=tv; 2=tablet, 3=TBD)
 *bit[29:28]: lcd debug para source(0=normal, 1=dts, 2=unifykey,
 *                                  3=bsp for uboot)
 *bit[27:16]: reserved
 *bit[15:8]: lcd test pattern
 *bit[7:0]:  lcd debug print flag
 */
struct lcd_debug_ctrl_s {
	unsigned char debug_print_flag;
	unsigned char debug_test_pattern;
	unsigned char debug_para_source;
	unsigned char debug_lcd_mode;
};

struct lcd_dft_config_s {
	char (*lcd_gpio)[LCD_CPU_GPIO_NAME_MAX];

	unsigned char key_valid;
	unsigned char clk_path; /* 0=default, 1=gp0_pll */
	unsigned int mode;
	struct ext_lcd_config_s *ext_lcd;
	struct lcd_pinmux_ctrl_s *lcd_pinmux;

#ifdef CONFIG_AML_LCD_EXTERN
	struct lcd_extern_common_s *ext_common;
	struct lcd_extern_config_s *ext_conf;
#endif

	char (*bl_gpio)[LCD_CPU_GPIO_NAME_MAX];
	struct lcd_pinmux_ctrl_s *bl_pinmux;
#ifdef CONFIG_AML_LCD_BL_LDIM
	char (*ldim_gpio)[LCD_CPU_GPIO_NAME_MAX];
	struct lcd_pinmux_ctrl_s *ldim_pinmux;
#endif
};

struct aml_lcd_data_s {
	enum lcd_chip_e chip_type;
	const char *chip_name;
	unsigned char rev_type;
	unsigned char drv_max;
	unsigned int offset_venc[LCD_MAX_DRV];
	unsigned int offset_venc_if[LCD_MAX_DRV];
	unsigned int offset_venc_data[LCD_MAX_DRV];
	struct lcd_dft_config_s *dft_conf[LCD_MAX_DRV];
};

/* ==============lcd driver================== */
#define LCD_STATUS_IF_ON      (1 << 0)
#define LCD_STATUS_ENCL_ON    (1 << 1)

struct aml_lcd_drv_s {
	unsigned int index;
	unsigned int status;
	unsigned char mode;
	unsigned char key_valid;
	unsigned char clk_path; /* 0=hpll, 1=gp0_pll */
	unsigned int output_vmode;

	struct lcd_config_s config;
	struct aml_lcd_data_s *data;
	struct lcd_boot_ctrl_s boot_ctrl;
	struct lcd_duration_s *std_duration;
	void *clk_conf;

	int  (*outputmode_check)(struct aml_lcd_drv_s *pdrv, char *mode, unsigned int frac);
	int  (*config_check)(struct aml_lcd_drv_s *pdrv, char *mode, unsigned int frac);
	void (*driver_init_pre)(struct aml_lcd_drv_s *pdrv);
	int  (*driver_init)(struct aml_lcd_drv_s *pdrv);
	void (*driver_disable)(struct aml_lcd_drv_s *pdrv);
	void (*list_support_mode)(struct aml_lcd_drv_s *pdrv);
#ifdef CONFIG_AML_LCD_TCON
	void (*tcon_reg_print)(void);
	void (*tcon_table_print)(void);
	void (*tcon_vac_print)(void);
	void (*tcon_demura_print)(void);
	void (*tcon_acc_print)(void);
	void (*tcon_data_print)(unsigned char index);
	void (*tcon_spi_print)(void);
	int (*tcon_spi_data_load)(void);
	unsigned int (*tcon_reg_read)(unsigned int addr, unsigned int flag);
	void (*tcon_reg_write)(unsigned int addr, unsigned int val,
			       unsigned int flag);
	unsigned int (*tcon_table_read)(unsigned int addr);
	unsigned int (*tcon_table_write)(unsigned int addr, unsigned int val);
#endif
	void *debug_info_reg;
	void *debug_info_if;
	void (*phy_set)(struct aml_lcd_drv_s *pdrv, int status);

	/* for factory test */
	struct lcd_power_step_s *factory_lcd_power_on_step;
};

extern void lcd_config_bsp_init(void);

struct aml_lcd_data_s *aml_lcd_get_data(void);
struct aml_lcd_drv_s *aml_lcd_get_driver(int index);

int lcd_probe(void);

/* global api for cmd */
int aml_lcd_driver_probe(int index);
void aml_lcd_driver_list_support_mode(void);
unsigned int aml_lcd_driver_outputmode_check(char *mode, unsigned int frac);
void aml_lcd_driver_prepare(int index, char *mode, unsigned int frac);
void aml_lcd_driver_enable(int index, char *mode, unsigned int frac);
void aml_lcd_driver_disable(int index);
void aml_lcd_driver_set_ss(int index, unsigned int level, unsigned int freq,
			   unsigned int mode);
void aml_lcd_driver_get_ss(int index);
void aml_lcd_driver_clk_info(int index);
void aml_lcd_driver_debug_print(int index, unsigned int val);
void aml_lcd_driver_info(int index);
void aml_lcd_driver_reg_info(int index);
void aml_lcd_vbyone_rst(int index);
void aml_lcd_vbyone_cdr(int index);
void aml_lcd_edp_edid(int index);
void aml_lcd_driver_test(int index, int num);
int aml_lcd_driver_prbs(int index, unsigned int s, unsigned int mode_flag);
void aml_lcd_driver_unifykey_dump(int index, unsigned int flag);

void aml_lcd_driver_ext_info(int index);
void aml_lcd_driver_ext_power_on(int index);
void aml_lcd_driver_ext_power_off(int index);

void aml_lcd_driver_bl_on(int index);
void aml_lcd_driver_bl_off(int index);
void aml_lcd_driver_set_bl_level(int index, int level);
unsigned int  aml_lcd_driver_get_bl_level(int index);
void aml_lcd_driver_bl_config_print(int index);

#endif /* INC_AML_LCD_VOUT_H */
