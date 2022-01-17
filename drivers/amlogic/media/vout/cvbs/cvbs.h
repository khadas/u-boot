/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _AML_CVBS_H
#define _AML_CVBS_H

/* 20220117: cvbsout add dts sva value config */
#define TVAFE_VER "20220117"

#define CONFIG_CVBS_PERFORMANCE_COMPATIBILITY_SUPPORT	1

#define CONFIG_CVBS_CHINASARFT		0x0
#define CONFIG_CVBS_CHINATELECOM	0x1
#define CONFIG_CVBS_CHINAMOBILE		0x2
#define CONFIG_CVBS_PERFORMANCE_ACTIVED	CONFIG_CVBS_CHINASARFT

#define MREG_END_MARKER 0xFFFF

enum cvbs_chip_e {
	CVBS_CHIP_G12A = 0,
	CVBS_CHIP_G12B, /* 1 */
	CVBS_CHIP_SC2, /* 2 */
	CVBS_CHIP_S4, /* 3 */
	CVBS_CHIP_S4D, /* 4 */
	CVBS_CHIP_MAX,
};

enum CVBS_MODE_e {
	VMODE_PAL,
	VMODE_NTSC,
	VMODE_PAL_M,
	VMODE_PAL_N,
	VMODE_NTSC_M,
	VMODE_MAX
};

struct cvbs_data_s {
	enum cvbs_chip_e chip_type;

	unsigned int reg_vid_pll_clk_div;
	unsigned int reg_vid_clk_div;
	unsigned int reg_vid_clk_ctrl;
	unsigned int reg_vid2_clk_div;
	unsigned int reg_vid2_clk_ctrl;
	unsigned int reg_vid_clk_ctrl2;

	unsigned int vdac_vref_adj;
	unsigned int vdac_gsw;
	unsigned int sva_val;
};

struct reg_s {
	unsigned int reg;
	unsigned int val;
};

#define CVBS_PERFORMANCE_CNT_MAX    20
struct performance_config_s {
	unsigned int reg_cnt;
	struct reg_s *reg_table;
};

struct cvbs_drv_s {
	struct cvbs_data_s *data;
	struct performance_config_s perf_conf_pal;
	struct performance_config_s perf_conf_ntsc;
	struct performance_config_s perf_conf_pal_sva;
};

#endif
