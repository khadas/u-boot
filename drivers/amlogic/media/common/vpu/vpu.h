/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __VPU_PARA_H__
#define __VPU_PARA_H__
#ifdef CONFIG_SECURE_POWER_CONTROL
#include <asm/arch/pwr_ctrl.h>
#endif

//#define VPU_DEBUG_PRINT
#define VPUPR(fmt, args...)     printf("vpu: "fmt"", ## args)
#define VPUERR(fmt, args...)    printf("vpu: error: "fmt"", ## args)

enum vpu_chip_e {
	VPU_CHIP_G12A = 0,
	VPU_CHIP_G12B, /* 1 */
	VPU_CHIP_TL1, /* 2 */
	VPU_CHIP_SM1, /* 3 */
	VPU_CHIP_TM2, /* 4 */
	VPU_CHIP_SC2, /* 5 */
	VPU_CHIP_T5, /* 6 */
	VPU_CHIP_T5D, /* 7 */
	VPU_CHIP_T7, /* 8 */
	VPU_CHIP_S4,  /* 9 */
	VPU_CHIP_T3, /* 10 */
	VPU_CHIP_S4D, /* 11 */
	VPU_CHIP_C3, /* 12 */
	VPU_CHIP_MAX,
};

#define VPU_PWR_ON             1
#define VPU_PWR_OFF            0
#define VPU_PWR_ID_END         0xffff
#ifndef PM_VPU_HDMI
#define PM_VPU_HDMI            0xffff
#endif
#ifndef PM_VI_CLK1
#define PM_VI_CLK1             0xffff
#endif
#ifndef PM_VI_CLK2
#define PM_VI_CLK2             0xffff
#endif
#ifndef PM_NOC_VPU
#define PM_NOC_VPU             0xffff
#endif
#define VPU_PWR_ID_MAX         10

#define VPU_REG_END            0xffff
#define VPU_MEM_PD_CNT_MAX     10
#define VPU_ISO_CNT_MAX        5
#define VPU_PWR_CNT_MAX        5
#define VPU_RESET_CNT_MAX      10

enum vpu_mux_e {
	FCLK_DIV4 = 0,
	FCLK_DIV3,
	FCLK_DIV5,
	FCLK_DIV7,
	MPLL_CLK1,
	VID_PLL_CLK,
	VID2_PLL_CLK,
	GPLL_CLK,
	FCLK_DIV_MAX,
};

struct fclk_div_s {
	unsigned int fclk_id;
	unsigned int fclk_mux;
	unsigned int fclk_div;
};

struct vpu_clk_s {
	unsigned int freq;
	unsigned int mux;
	unsigned int div;
};

struct vpu_ctrl_s {
	unsigned int reg;
	unsigned int val;
	unsigned int bit;
	unsigned int len;
};

struct vpu_reset_s {
	unsigned int reg;
	unsigned int mask;
};

struct vpu_conf_s;
struct vpu_data_s {
	enum vpu_chip_e chip_type;
	const char *chip_name;
	unsigned char clk_level_dft;
	unsigned char clk_level_max;
	unsigned char gp_pll_valid;

	unsigned int vpu_clk_reg;
	unsigned int vpu_clkb_reg;
	unsigned int vapb_clk_reg;
	unsigned int vid_clk_reg;

	unsigned int *pwrctrl_id_table;

	struct fclk_div_s *fclk_div_table;
	struct vpu_clk_s  *vpu_clk_table;
	unsigned int *test_reg;

	struct vpu_ctrl_s *mem_pd_table;
	struct vpu_ctrl_s *power_table;
	struct vpu_ctrl_s *iso_table;
	struct vpu_reset_s *reset_table;

	unsigned int module_init_table_cnt;
	struct vpu_ctrl_s *module_init_table;

	void (*power_on)(void);
	void (*power_off)(void);
	void (*mem_pd_init_off)(void);
	void (*module_init_config)(void);
	int (*change_clk)(struct vpu_conf_s *vconf, unsigned int vclk);
};

struct vpu_conf_s {
	struct vpu_data_s *data;
	char drv_version[20];

	unsigned int clk_level;
	unsigned int fclk_freq; /* unit: MHz */
};

extern struct vpu_conf_s vpu_conf;

/* ************************************************ */
/* extern function */
/* ************************************************ */
extern void udelay(unsigned long usec);
extern int printf(const char *fmt, ...);
/* ************************************************ */

unsigned int get_vpu_clk(struct vpu_conf_s *vconf);
int change_vpu_clk(struct vpu_conf_s *vconf, unsigned int vclk);
int set_vpu_clk(struct vpu_conf_s *vconf, unsigned int vclk);
int set_vpu_clkb(struct vpu_conf_s *vconf, unsigned int vclk);

void vpu_mem_pd_init_off(void);
void vpu_module_init_config(void);

void vpu_power_on(void);
void vpu_power_off(void);
void vpu_power_on_new(void);
void vpu_power_off_new(void);
void vpu_power_off_c3(void);

#endif
