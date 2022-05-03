// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <amlogic/media/vpu/vpu.h>
#include "vpu_reg.h"
#include "vpu.h"

static unsigned int get_vpu_clk_level(struct vpu_conf_s *vconf, unsigned int video_clk)
{
	unsigned clk_level;
	int i;

	for (i = 0; i < vconf->data->clk_level_max; i++) {
		if (video_clk <= (vconf->data->vpu_clk_table + i)->freq)
			break;
	}
	clk_level = i;

	return clk_level;
}

static unsigned long get_fclk_div_freq(struct vpu_conf_s *vconf, unsigned int mux_id)
{
	struct fclk_div_s *fclk_div;
	unsigned long fclk, div, clk_source = 0;
	unsigned int i;

	fclk = vconf->fclk_freq * 1000000;

	for (i = 0; i < FCLK_DIV_MAX; i++) {
		fclk_div = vconf->data->fclk_div_table + i;
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

static unsigned int get_vpu_clk_mux_id(struct vpu_conf_s *vconf)
{
	struct fclk_div_s *fclk_div;
	unsigned int i, mux, mux_id;

	mux = vpu_hiu_getb(vconf->data->vpu_clk_reg, 9, 3);
	mux_id = mux;
	for (i = 0; i < FCLK_DIV_MAX; i++) {
		fclk_div = vconf->data->fclk_div_table + i;
		if (fclk_div->fclk_mux == mux) {
			mux_id = fclk_div->fclk_id;
			break;
		}
		if (fclk_div->fclk_id == FCLK_DIV_MAX)
			break;
	}

	return mux_id;
}

static unsigned int get_vpu_clk_mux(struct vpu_conf_s *vconf, unsigned int mux_id)
{
	struct fclk_div_s *fclk_div;
	unsigned int i, mux;

	mux = mux_id;
	for (i = 0; i < FCLK_DIV_MAX; i++) {
		fclk_div = vconf->data->fclk_div_table + i;
		if (fclk_div->fclk_id == mux_id) {
			mux = fclk_div->fclk_mux;
			break;
		}
		if (fclk_div->fclk_id == FCLK_DIV_MAX)
			break;
	}

	return mux;
}

unsigned int get_vpu_clk(struct vpu_conf_s *vconf)
{
	unsigned long clk_freq;
	unsigned long clk_source, div;
	unsigned int mux_id;

	mux_id = get_vpu_clk_mux_id(vconf);
	switch (mux_id) {
	case FCLK_DIV4:
	case FCLK_DIV3:
	case FCLK_DIV5:
	case FCLK_DIV7:
		clk_source = get_fclk_div_freq(vconf, mux_id);
		break;
	case GPLL_CLK:
		clk_source = (vconf->data->vpu_clk_table + 8)->freq;
		break;
	default:
		clk_source = 0;
		break;
	}

	div = vpu_hiu_getb(vconf->data->vpu_clk_reg, 0, 7) + 1;
	clk_freq = ((clk_source * 100 / div) + 99) / 100;

	return (unsigned int)clk_freq;
}

static int switch_gp_pll(struct vpu_conf_s *vconf, int flag)
{
	int ret = -1;

	if (vconf->data->gp_pll_valid == 0)
		return ret;

	switch (vconf->data->chip_type) {
	default:
		break;
	}

	return ret;
}

static void vpu_clk_switch(struct vpu_conf_s *vconf)
{
	struct vpu_clk_s *clk_table;
	unsigned int mux, div, clk_reg;

	clk_reg = vconf->data->vpu_clk_reg;

	/* step 1: switch to 2nd vpu clk patch */
	clk_table = vconf->data->vpu_clk_table + vconf->data->clk_level_dft;
	mux = get_vpu_clk_mux(vconf, clk_table->mux);
	vpu_hiu_setb(clk_reg, mux, 25, 3);
	div = clk_table->div;
	vpu_hiu_setb(clk_reg, div, 16, 7);
	vpu_hiu_setb(clk_reg, 1, 24, 1);
	vpu_hiu_setb(clk_reg, 1, 31, 1);
	udelay(10);
	/* step 2: adjust 1st vpu clk frequency */
	clk_table = vconf->data->vpu_clk_table + vconf->clk_level;
	vpu_hiu_setb(clk_reg, 0, 8, 1);
	mux = get_vpu_clk_mux(vconf, clk_table->mux);
	vpu_hiu_setb(clk_reg, mux, 9, 3);
	div = clk_table->div;
	vpu_hiu_setb(clk_reg, div, 0, 7);
	vpu_hiu_setb(clk_reg, 1, 8, 1);
	udelay(20);
	/* step 3: switch back to 1st vpu clk patch */
	vpu_hiu_setb(clk_reg, 0, 31, 1);
	vpu_hiu_setb(clk_reg, 0, 24, 1);
}

int change_vpu_clk(struct vpu_conf_s *vconf, unsigned int vclk)
{
	struct vpu_clk_s *clk_table;
	unsigned int vpu_clk;

	if (vclk >= 100) /* regard as vpu_clk */
		vclk = get_vpu_clk_level(vconf, vclk);

	if (vclk >= vconf->data->clk_level_max) {
		clk_table = vconf->data->vpu_clk_table + (vconf->data->clk_level_max - 1);
		VPUPR("clk out of supported range! clk max level: %u(%uHz)\n",
			(vconf->data->clk_level_max - 1),
			clk_table->freq);
		return -1;
	}

	vconf->clk_level = vclk;
	clk_table = vconf->data->vpu_clk_table + vconf->clk_level;
	vpu_clk = clk_table->freq;

	vpu_clk_switch(vconf);

	VPUPR("set clk: %uHz, readback: %uHz(0x%x)\n",
		vpu_clk, get_vpu_clk(vconf), vpu_hiu_read(vconf->data->vpu_clk_reg));
	return 0;
}

static int adjust_vpu_clk(struct vpu_conf_s *vconf, unsigned int clk_level)
{
	struct vpu_clk_s *clk_table;
	unsigned int vpu_clk;
	unsigned int mux, div, clk_reg, vapb_reg;
	int ret = 0;

	/* vpu clk */
	clk_table = vconf->data->vpu_clk_table + clk_level;
	mux = get_vpu_clk_mux(vconf, clk_table->mux);
	if (mux == GPLL_CLK) {
		ret = switch_gp_pll(vconf, 1);
		if (ret) {
			clk_level = vconf->data->clk_level_dft;
			VPUERR("%s: gp pll error, use default clk\n", __func__);
		}
	}
	vconf->clk_level = clk_level;
	clk_table = vconf->data->vpu_clk_table + clk_level;

	vpu_clk = clk_table->freq;
	mux = get_vpu_clk_mux(vconf, clk_table->mux);
	div = clk_table->div;
	clk_reg = vconf->data->vpu_clk_reg;
	vapb_reg = vconf->data->vapb_clk_reg;

	vpu_hiu_setb(clk_reg, 0, 8, 1);
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
		vpu_clk, get_vpu_clk(vconf), (vpu_hiu_read(clk_reg)));
	return ret;
}

int set_vpu_clk(struct vpu_conf_s *vconf, unsigned int vclk)
{
	int ret = 0;
	unsigned int clk_level;

#ifdef VPU_DEBUG_PRINT
	VPUPR("%s\n", __func__);
#endif

	if (vclk >= 100) /* regard as vpu_clk */
		clk_level = get_vpu_clk_level(vconf, vclk);
	else /* regard as clk_level */
		clk_level = vclk;

	if (clk_level >= vconf->data->clk_level_max) {
		clk_level = vconf->data->clk_level_dft;
		VPUPR("clk out of supported range, set to default\n");
	}

	ret = adjust_vpu_clk(vconf, clk_level);

#ifdef VPU_DEBUG_PRINT
	VPUPR("%s finish\n", __func__);
#endif

	return ret;
}

int set_vpu_clkb(struct vpu_conf_s *vconf, unsigned int vclk)
{
	unsigned int clk_level;
	unsigned int clkb_reg;
	int ret = 0;

	/* vpu clkb */
	clkb_reg = vconf->data->vpu_clkb_reg;
	if (!clkb_reg)
		return 0;

	if (vclk >= 100) /* regard as vpu_clk */
		clk_level = get_vpu_clk_level(vconf, vclk);
	else /* regard as clk_level */
		clk_level = vclk;

	if (clk_level >= vconf->data->clk_level_max) {
		clk_level = vconf->data->clk_level_dft;
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

