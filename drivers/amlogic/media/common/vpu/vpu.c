// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <amlogic/cpu_id.h>
#include <fdtdec.h>
#include <amlogic/media/vpu/vpu.h>
#include "vpu_reg.h"
#include "vpu.h"
#include "vpu_ctrl.h"

/* v20181009: init version */
/* v20190313: add sm1 support */
/* v20200521: add sc2 support */
/* v20201118: add t7 support */
/* v20210114: add s4 support */
/* v20210317: add t3 support */
/* v20210428: add s4d support */
#define VPU_VERION	"v20210427"

DECLARE_GLOBAL_DATA_PTR;

struct vpu_conf_s vpu_conf = {
	.data = NULL,

	.clk_level = 0,
	.fclk_freq = FCLK_2000M,
};

static struct vpu_data_s vpu_data_g12a = {
	.chip_type = VPU_CHIP_G12A,
	.chip_name = "g12a",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.vpu_clk_reg = HHI_VPU_CLK_CNTL,
	.vapb_clk_reg = HHI_VAPBCLK_CNTL,
	.vid_clk_reg = HHI_VID_CLK_CNTL2,

	.pwrctrl_id_table = NULL,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = vpu_mem_pd_g12a,
	.power_table = vpu_pwr_gxb,
	.iso_table = vpu_iso_gxb,
	.reset_table = vpu_reset_g12a,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on,
	.power_off = vpu_power_off,
};

static struct vpu_data_s vpu_data_g12b = {
	.chip_type = VPU_CHIP_G12B,
	.chip_name = "g12b",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.vpu_clk_reg = HHI_VPU_CLK_CNTL,
	.vapb_clk_reg = HHI_VAPBCLK_CNTL,
	.vid_clk_reg = HHI_VID_CLK_CNTL2,

	.pwrctrl_id_table = NULL,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = vpu_mem_pd_g12a,
	.power_table = vpu_pwr_gxb,
	.iso_table = vpu_iso_gxb,
	.reset_table = vpu_reset_g12a,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on,
	.power_off = vpu_power_off,
};

static struct vpu_data_s vpu_data_tl1 = {
	.chip_type = VPU_CHIP_TL1,
	.chip_name = "tl1",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.vpu_clk_reg = HHI_VPU_CLK_CNTL,
	.vapb_clk_reg = HHI_VAPBCLK_CNTL,
	.vid_clk_reg = HHI_VID_CLK_CNTL2,

	.pwrctrl_id_table = NULL,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = vpu_mem_pd_tl1,
	.power_table = vpu_pwr_gxb,
	.iso_table = vpu_iso_gxb,
	.reset_table = vpu_reset_tl1,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on,
	.power_off = vpu_power_off,
};

/* static struct vpu_data_s vpu_data_sm1 = {
	.chip_type = VPU_CHIP_SM1,
	.chip_name = "sm1",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.vpu_clk_reg = HHI_VPU_CLK_CNTL,
	.vapb_clk_reg = HHI_VAPBCLK_CNTL,
	.vid_clk_reg = HHI_VID_CLK_CNTL2,

	.pwrctrl_id_table = NULL,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = vpu_mem_pd_tl1,
	.power_table = vpu_pwr_gxb,
	.iso_table = vpu_iso_sm1,
	.reset_table = vpu_reset_gx,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on,
	.power_off = vpu_power_off,
};
*/

static struct vpu_data_s vpu_data_tm2 = {
	.chip_type = VPU_CHIP_TM2,
	.chip_name = "tm2",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.vpu_clk_reg = HHI_VPU_CLK_CNTL,
	.vapb_clk_reg = HHI_VAPBCLK_CNTL,
	.vid_clk_reg = HHI_VID_CLK_CNTL2,

	.pwrctrl_id_table = NULL,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = vpu_mem_pd_tm2,
	.power_table = vpu_pwr_gxb,
	.iso_table = vpu_iso_sm1,
	.reset_table = vpu_reset_tl1,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on,
	.power_off = vpu_power_off,
};

static struct vpu_data_s vpu_data_sc2 = {
	.chip_type = VPU_CHIP_SC2,
	.chip_name = "sc2",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.vpu_clk_reg = CLKCTRL_VPU_CLK_CTRL,
	.vapb_clk_reg = CLKCTRL_VAPBCLK_CTRL,
	.vid_clk_reg = CLKCTRL_VID_CLK_CTRL2,

	.pwrctrl_id_table = vpu_pwrctrl_id_table,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = NULL,
	.power_table = NULL,
	.iso_table = NULL,
	.reset_table = NULL,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on_new,
	.power_off = vpu_power_off_new,
};

static struct vpu_data_s vpu_data_t5 = {
	.chip_type = VPU_CHIP_T5,
	.chip_name = "t5",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.vpu_clk_reg = HHI_VPU_CLK_CNTL,
	.vapb_clk_reg = HHI_VAPBCLK_CNTL,
	.vid_clk_reg = HHI_VID_CLK_CNTL2,

	.pwrctrl_id_table = vpu_pwrctrl_id_table,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = vpu_mem_pd_tm2,
	.power_table = vpu_pwr_gxb,
	.iso_table = vpu_iso_sm1,
	.reset_table = vpu_reset_tl1,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on_new,
	.power_off = vpu_power_off_new,
};

static struct vpu_data_s vpu_data_t5d = {
	.chip_type = VPU_CHIP_T5D,
	.chip_name = "t5d",
	.clk_level_dft = CLK_LEVEL_DFT_T5D,
	.clk_level_max = CLK_LEVEL_MAX_T5D,
	.gp_pll_valid = 0,

	.vpu_clk_reg = HHI_VPU_CLK_CNTL,
	.vapb_clk_reg = HHI_VAPBCLK_CNTL,
	.vid_clk_reg = HHI_VID_CLK_CNTL2,

	.pwrctrl_id_table = vpu_pwrctrl_id_table,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = NULL,
	.power_table = NULL,
	.iso_table = NULL,
	.reset_table = NULL,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on_new,
	.power_off = vpu_power_off_new,
};

static struct vpu_data_s vpu_data_t7 = {
	.chip_type = VPU_CHIP_T7,
	.chip_name = "t7",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.vpu_clk_reg = CLKCTRL_VPU_CLK_CTRL,
	.vapb_clk_reg = CLKCTRL_VAPBCLK_CTRL,
	.vid_clk_reg = CLKCTRL_VID_CLK0_CTRL2,

	.pwrctrl_id_table = vpu_pwrctrl_id_table_t7,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = NULL,
	.power_table = NULL,
	.iso_table = NULL,
	.reset_table = NULL,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on_new,
	.power_off = vpu_power_off_new,
};

static struct vpu_data_s vpu_data_s4 = {
	.chip_type = VPU_CHIP_S4,
	.chip_name = "s4",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.vpu_clk_reg = CLKCTRL_VPU_CLK_CTRL,
	.vapb_clk_reg = CLKCTRL_VAPBCLK_CTRL,
	.vid_clk_reg = CLKCTRL_VID_CLK_CTRL2,

	.pwrctrl_id_table = vpu_pwrctrl_id_table,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = NULL,
	.power_table = NULL,
	.iso_table = NULL,
	.reset_table = NULL,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on_new,
	.power_off = vpu_power_off_new,
};

static struct vpu_data_s vpu_data_t3 = {
	.chip_type = VPU_CHIP_T3,
	.chip_name = "t3",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.vpu_clk_reg = CLKCTRL_VPU_CLK_CTRL,
	.vpu_clkb_reg = CLKCTRL_VPU_CLKB_CTRL,
	.vapb_clk_reg = CLKCTRL_VAPBCLK_CTRL,
	.vid_clk_reg = CLKCTRL_VID_CLK0_CTRL2,

	.pwrctrl_id_table = vpu_pwrctrl_id_table_t3,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = NULL,
	.power_table = NULL,
	.iso_table = NULL,
	.reset_table = NULL,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on_new,
	.power_off = vpu_power_off_new,
};

static struct vpu_data_s vpu_data_s4d = {
	.chip_type = VPU_CHIP_S4D,
	.chip_name = "s4d",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.vpu_clk_reg = CLKCTRL_VPU_CLK_CTRL,
	.vapb_clk_reg = CLKCTRL_VAPBCLK_CTRL,
	.vid_clk_reg = CLKCTRL_VID_CLK_CTRL2,

	.pwrctrl_id_table = vpu_pwrctrl_id_table,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = NULL,
	.power_table = NULL,
	.iso_table = NULL,
	.reset_table = NULL,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on_new,
	.power_off = vpu_power_off_new,
};

static void vpu_chip_detect(void)
{
	unsigned int cpu_type;

	cpu_type = get_cpu_id().family_id;
	switch (cpu_type) {
	case MESON_CPU_MAJOR_ID_G12A:
		vpu_conf.data = &vpu_data_g12a;
		break;
	case MESON_CPU_MAJOR_ID_G12B:
		vpu_conf.data = &vpu_data_g12b;
		break;
	case MESON_CPU_MAJOR_ID_TL1:
		vpu_conf.data = &vpu_data_tl1;
		break;
	//case MESON_CPU_MAJOR_ID_SM1:
	//	vpu_conf.data = &vpu_data_sm1;
	//	break;
	case MESON_CPU_MAJOR_ID_TM2:
		vpu_conf.data = &vpu_data_tm2;
		break;
	case MESON_CPU_MAJOR_ID_SC2:
		vpu_conf.data = &vpu_data_sc2;
		break;
	case MESON_CPU_MAJOR_ID_T5:
		vpu_conf.data = &vpu_data_t5;
		break;
	case MESON_CPU_MAJOR_ID_T5D:
		vpu_conf.data = &vpu_data_t5d;
		break;
	case MESON_CPU_MAJOR_ID_T7:
		vpu_conf.data = &vpu_data_t7;
		break;
	case MESON_CPU_MAJOR_ID_T3:
		vpu_conf.data = &vpu_data_t3;
		break;
	case MESON_CPU_MAJOR_ID_S4:
		vpu_conf.data = &vpu_data_s4;
		break;
	case MESON_CPU_MAJOR_ID_S4D:
		vpu_conf.data = &vpu_data_s4d;
		break;
	default:
		vpu_conf.data = NULL;
		break;
	}

	strcpy(vpu_conf.drv_version, VPU_VERION);
	if (!vpu_conf.data) {
		VPUERR("invalid chip: 0x%x\n", cpu_type);
		return;
	}

#ifdef AML_VPU_CLK_LEVEL_DFT
	vpu_conf.data->clk_level_dft = AML_VPU_CLK_LEVEL_DFT;
	VPUPR("find AML_VPU_CLK_LEVEL_DFT: %d\n", AML_VPU_CLK_LEVEL_DFT);
#endif

#ifdef VPU_DEBUG_PRINT
	VPUPR("driver version: %s\n", vpu_conf.drv_version);
	VPUPR("detect chip type: %d(%s)\n", vpu_conf.data->chip_type, vpu_conf.data->chip_name);
	VPUPR("clk_level default: %d(%dHz), max: %d(%dHz)\n",
		vpu_conf.data->clk_level_dft,
		(vpu_conf.data->vpu_clk_table + vpu_conf.data->clk_level_dft)->freq,
		(vpu_conf.data->clk_level_max - 1),
		(vpu_conf.data->vpu_clk_table + (vpu_conf.data->clk_level_max - 1))->freq);
#endif
}

static int vpu_check(void)
{
	int ret = -1;

	if (!vpu_conf.data) {
		VPUERR("invalid vpu\n");
		return -1;
	}

	switch (vpu_conf.data->chip_type) {
	case VPU_CHIP_G12A:
	case VPU_CHIP_G12B:
	case VPU_CHIP_TL1:
	case VPU_CHIP_SM1:
	case VPU_CHIP_TM2:
	case VPU_CHIP_SC2:
	case VPU_CHIP_T5:
	case VPU_CHIP_T5D:
	case VPU_CHIP_T7:
	case VPU_CHIP_S4:
	case VPU_CHIP_T3:
	case VPU_CHIP_S4D:
		ret = 0;
		break;
	default:
		VPUERR("invalid vpu for current chip\n");
		break;
	}

	return ret;
}

static unsigned int get_vpu_clk_level(unsigned int video_clk)
{
	unsigned clk_level;
	int i;

	for (i = 0; i < vpu_conf.data->clk_level_max; i++) {
		if (video_clk <= (vpu_conf.data->vpu_clk_table + i)->freq)
			break;
	}
	clk_level = i;

	return clk_level;
}

static unsigned long get_fclk_div_freq(unsigned int mux_id)
{
	struct fclk_div_s *fclk_div;
	unsigned long fclk, div, clk_source = 0;
	unsigned int i;

	fclk = vpu_conf.fclk_freq * 1000000;

	for (i = 0; i < FCLK_DIV_MAX; i++) {
		fclk_div = vpu_conf.data->fclk_div_table + i;
		if (fclk_div->fclk_id == mux_id) {
			div = fclk_div->fclk_div;
			clk_source = ((fclk * 100 / div) + 99) / 100;
			break;
		}
		if (fclk_div->fclk_id == FCLK_DIV_MAX)
			break;
	}

	return clk_source;
}

static unsigned int get_vpu_clk_mux_id(void)
{
	struct fclk_div_s *fclk_div;
	unsigned int i, mux, mux_id;

	mux = vpu_hiu_getb(vpu_conf.data->vpu_clk_reg, 9, 3);
	mux_id = mux;
	for (i = 0; i < FCLK_DIV_MAX; i++) {
		fclk_div = vpu_conf.data->fclk_div_table + i;
		if (fclk_div->fclk_mux == mux) {
			mux_id = fclk_div->fclk_id;
			break;
		}
		if (fclk_div->fclk_id == FCLK_DIV_MAX)
			break;
	}

	return mux_id;
}

static unsigned int get_vpu_clk_mux(unsigned int mux_id)
{
	struct fclk_div_s *fclk_div;
	unsigned int i, mux;

	mux = mux_id;
	for (i = 0; i < FCLK_DIV_MAX; i++) {
		fclk_div = vpu_conf.data->fclk_div_table + i;
		if (fclk_div->fclk_id == mux_id) {
			mux = fclk_div->fclk_mux;
			break;
		}
		if (fclk_div->fclk_id == FCLK_DIV_MAX)
			break;
	}

	return mux;
}

static unsigned int get_vpu_clk(void)
{
	unsigned long clk_freq;
	unsigned long clk_source, div;
	unsigned int mux_id;

	mux_id = get_vpu_clk_mux_id();
	switch (mux_id) {
	case FCLK_DIV4:
	case FCLK_DIV3:
	case FCLK_DIV5:
	case FCLK_DIV7:
		clk_source = get_fclk_div_freq(mux_id);
		break;
	case GPLL_CLK:
		clk_source = (vpu_conf.data->vpu_clk_table + 8)->freq;
		break;
	default:
		clk_source = 0;
		break;
	}

	div = vpu_hiu_getb(vpu_conf.data->vpu_clk_reg, 0, 7) + 1;
	clk_freq = ((clk_source * 100 / div) + 99) / 100;

	return (unsigned int)clk_freq;
}

static int switch_gp_pll(int flag)
{
	int ret = -1;

	if (vpu_conf.data->gp_pll_valid == 0)
		return ret;

	switch (vpu_conf.data->chip_type) {
	default:
		break;
	}

	return ret;
}

static int adjust_vpu_clk(unsigned int clk_level)
{
	struct vpu_clk_s *clk_table;
	unsigned int vpu_clk;
	unsigned int mux, div, clk_reg, vapb_reg;
	int ret = 0;

	/* vpu clk */
	clk_table = vpu_conf.data->vpu_clk_table + clk_level;
	mux = get_vpu_clk_mux(clk_table->mux);
	if (mux == GPLL_CLK) {
		ret = switch_gp_pll(1);
		if (ret) {
			clk_level = vpu_conf.data->clk_level_dft;
			VPUERR("adjust_vpu_clk: gp pll error, use default clk\n");
		}
	}
	vpu_conf.clk_level = clk_level;
	clk_table = vpu_conf.data->vpu_clk_table + clk_level;

	vpu_clk = clk_table->freq;
	mux = get_vpu_clk_mux(clk_table->mux);
	div = clk_table->div;
	clk_reg = vpu_conf.data->vpu_clk_reg;
	vapb_reg = vpu_conf.data->vapb_clk_reg;

	vpu_hiu_write(clk_reg, ((mux << 9) | (div << 0)));
	vpu_hiu_setb(clk_reg, 1, 8, 1);

	/* vapb clk */
	if (vpu_hiu_getb(vapb_reg, 8, 1) == 0) {
		if (vpu_clk >= 250000000) {
			vpu_hiu_write(vapb_reg, (0 << 9)  | /* clk_sel    //250Mhz */
						(1 << 0));  /* clk_div */
		} else {
			vpu_hiu_write(vapb_reg, (clk_table->mux << 9)  | /* clk_sel */
						(div << 0));  /* clk_div */
		}
		vpu_hiu_setb(vapb_reg, 1, 8, 1);
	}

	VPUPR("set clk: %uHz, readback: %uHz(0x%x)\n",
		vpu_clk, get_vpu_clk(), (vpu_hiu_read(clk_reg)));
	return ret;
}

static int set_vpu_clk(unsigned int vclk)
{
	int ret = 0;
	unsigned int clk_level;

#ifdef VPU_DEBUG_PRINT
	VPUPR("%s\n", __func__);
#endif

	if (vclk >= 100) /* regard as vpu_clk */
		clk_level = get_vpu_clk_level(vclk);
	else /* regard as clk_level */
		clk_level = vclk;

	if (clk_level >= vpu_conf.data->clk_level_max) {
		clk_level = vpu_conf.data->clk_level_dft;
		VPUPR("clk out of supported range, set to default\n");
	}

	ret = adjust_vpu_clk(clk_level);

#ifdef VPU_DEBUG_PRINT
	VPUPR("%s finish\n", __func__);
#endif

	return ret;
}

static int set_vpu_clkb(unsigned int vclk)
{
	unsigned int clk_level;
	unsigned int clkb_reg;
	int ret = 0;

	/* vpu clkb */
	clkb_reg = vpu_conf.data->vpu_clkb_reg;
	if (!clkb_reg)
		return 0;

	if (vclk >= 100) /* regard as vpu_clk */
		clk_level = get_vpu_clk_level(vclk);
	else /* regard as clk_level */
		clk_level = vclk;

	if (clk_level >= vpu_conf.data->clk_level_max) {
		clk_level = vpu_conf.data->clk_level_dft;
		VPUPR("clk out of supported range, set to default\n");
	}
#ifdef VPU_DEBUG_PRINT
	VPUPR("%s\n", __func__);
#endif

	vpu_hiu_setb(clkb_reg, 1, 8, 1);
	vpu_hiu_setb(clkb_reg, 1, 0, 8);
	vpu_hiu_setb(clkb_reg, 1, 24, 1);
	vpu_hiu_setb(clkb_reg, 0, 16, 4);
	vpu_hiu_setb(clkb_reg, 0, 20, 2);

#ifdef VPU_DEBUG_PRINT
	VPUPR("%s finish\n", __func__);
#endif

	return ret;
}

static int get_vpu_config(void)
{
	const void *dt_blob;
	int node;
	char *propdata;
	int ret;

	dt_blob = gd->fdt_blob;
	if (dt_blob == NULL) {
		VPUERR("dt_blob is null, load default parameters\n");
		goto get_vpu_config_dft;
	}
	ret = fdt_check_header(dt_blob);
	if (ret < 0) {
		VPUERR("vpu: check dts: %s, load default parameters\n",
			fdt_strerror(ret));
		goto get_vpu_config_dft;
	}
	node = fdt_path_offset(dt_blob, "/vpu");
	if (node < 0) {
		VPUERR("not find vpu node in dts: %s\n", fdt_strerror(node));
		goto get_vpu_config_dft;
	}

	propdata = (char *)fdt_getprop(dt_blob, node, "clk_level", NULL);
	if (propdata == NULL) {
		VPUERR("don't find clk_level in dts, set to default\n");
		goto get_vpu_config_dft;
	}
	vpu_conf.clk_level = be32_to_cpup((u32*)propdata);
	if (vpu_conf.clk_level >= vpu_conf.data->clk_level_max) {
		VPUERR("clk_level in dts is out of support, set to default\n");
		goto get_vpu_config_dft;
	}

	VPUPR("clk_level in dts: %u\n", vpu_conf.clk_level);
	return 0;

get_vpu_config_dft:
	vpu_conf.clk_level = vpu_conf.data->clk_level_dft;
	VPUPR("clk_level = %u\n", vpu_conf.clk_level);

	return 0;
}

int vpu_probe(void)
{
	int ret;
	vpu_chip_detect();
	if (vpu_check())
		return -1;

	ret = get_vpu_config();
	if (vpu_conf.data->power_on)
		vpu_conf.data->power_on();
	set_vpu_clk(vpu_conf.clk_level);
	set_vpu_clkb(vpu_conf.clk_level);

	/* vpu module init off, for power save, and special module init */
	vpu_mem_pd_init_off();
	vpu_module_init_config();
	VPUPR("%s\n", __func__);

	return ret;
}

int vpu_remove(void)
{
	if (vpu_check())
		return -1;

	VPUPR("vpu remove\n");
	if (vpu_conf.data->power_off)
		vpu_conf.data->power_off();

	return 0;
}

static void vpu_clk_switch(void)
{
	struct vpu_clk_s *clk_table;
	unsigned int mux, div, clk_reg;

	clk_reg = vpu_conf.data->vpu_clk_reg;

	/* step 1: switch to 2nd vpu clk patch */
	clk_table = vpu_conf.data->vpu_clk_table + vpu_conf.data->clk_level_dft;
	mux = get_vpu_clk_mux(clk_table->mux);
	vpu_hiu_setb(clk_reg, mux, 25, 3);
	div = clk_table->div;
	vpu_hiu_setb(clk_reg, div, 16, 7);
	vpu_hiu_setb(clk_reg, 1, 24, 1);
	vpu_hiu_setb(clk_reg, 1, 31, 1);
	udelay(10);
	/* step 2: adjust 1st vpu clk frequency */
	clk_table = vpu_conf.data->vpu_clk_table + vpu_conf.clk_level;
	vpu_hiu_setb(clk_reg, 0, 8, 1);
	mux = get_vpu_clk_mux(clk_table->mux);
	vpu_hiu_setb(clk_reg, mux, 9, 3);
	div = clk_table->div;
	vpu_hiu_setb(clk_reg, div, 0, 7);
	vpu_hiu_setb(clk_reg, 1, 8, 1);
	udelay(20);
	/* step 3: switch back to 1st vpu clk patch */
	vpu_hiu_setb(clk_reg, 0, 31, 1);
	vpu_hiu_setb(clk_reg, 0, 24, 1);
}

int vpu_clk_change(int level)
{
	struct vpu_clk_s *clk_table;
	unsigned int vpu_clk;

	if (vpu_check())
		return -1;

	if (level >= 100) /* regard as vpu_clk */
		level = get_vpu_clk_level(level);

	if (level >= vpu_conf.data->clk_level_max) {
		clk_table = vpu_conf.data->vpu_clk_table + (vpu_conf.data->clk_level_max - 1);
		VPUPR("clk out of supported range\n");
		VPUPR("clk max level: %u(%uHz)\n",
			(vpu_conf.data->clk_level_max - 1),
			clk_table->freq);
		return -1;
	}

	vpu_conf.clk_level = level;
	clk_table = vpu_conf.data->vpu_clk_table + vpu_conf.clk_level;
	vpu_clk = clk_table->freq;

	vpu_clk_switch();

	VPUPR("set clk: %uHz, readback: %uHz(0x%x)\n",
		vpu_clk, get_vpu_clk(), vpu_hiu_read(vpu_conf.data->vpu_clk_reg));
	return 0;
}

void vpu_clk_get(void)
{
	if (vpu_check())
		return;

	VPUPR("clk_level: %u, clk: %uHz, reg: 0x%x\n",
		vpu_conf.clk_level, get_vpu_clk(),
		vpu_hiu_read(vpu_conf.data->vpu_clk_reg));
}

void vpu_info_print(void)
{
	if (vpu_check())
		return;

	printf("vpu driver version: %s\n", vpu_conf.drv_version);
	printf("detect chip type: %d(%s)\n",
		vpu_conf.data->chip_type,
		vpu_conf.data->chip_name);
	printf("clk_level:         %d(%dHz)\n"
		"clk_level default: %d(%dHz)\n"
		"clk_level max:     %d(%dHz)\n",
		vpu_conf.clk_level,
		(vpu_conf.data->vpu_clk_table + vpu_conf.clk_level)->freq,
		vpu_conf.data->clk_level_dft,
		(vpu_conf.data->vpu_clk_table + vpu_conf.data->clk_level_dft)->freq,
		(vpu_conf.data->clk_level_max - 1),
		(vpu_conf.data->vpu_clk_table + (vpu_conf.data->clk_level_max - 1))->freq);
}

#define VCBUS_REG_CNT_MAX    4
static unsigned int vcbus_reg[VCBUS_REG_CNT_MAX] = {
	VENC_VDAC_TST_VAL,
	ENCP_DVI_HSO_BEGIN,
	VPP_DUMMY_DATA,
	VPU_VPU_PWM_V0,
};

void vcbus_test(void)
{
	unsigned int val;
	unsigned int temp;
	int i,j;

	if (vpu_check())
		return;

	VPUPR("vcbus test:\n");
	for (i = 0; i < VCBUS_REG_CNT_MAX; i++) {
		for (j = 0; j < 24; j++) {
			val = vpu_vcbus_read(vcbus_reg[i]);
			printf("%02d read 0x%04x=0x%08x\n", j, vcbus_reg[i], val);
		}
		printf("\n");
	}
	temp = 0x5a5a5a5a;
	for (i = 0; i < VCBUS_REG_CNT_MAX; i++) {
		vpu_vcbus_write(vcbus_reg[i], temp);
		val = vpu_vcbus_read(vcbus_reg[i]);
		printf("write 0x%04x=0x%08x, readback: 0x%08x\n", vcbus_reg[i], temp, val);
	}
	for (i = 0; i < VCBUS_REG_CNT_MAX; i++) {
		for (j = 0; j < 24; j++) {
			val = vpu_vcbus_read(vcbus_reg[i]);
			printf("%02d read 0x%04x=0x%08x\n", j, vcbus_reg[i], val);
		}
		printf("\n");
	}
}
