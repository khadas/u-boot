
/*
 * drivers/amlogic/media/common/vpu/vpu.c
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <common.h>
#include <asm/arch/cpu_id.h>
#include <fdtdec.h>
#include <amlogic/media/vpu/vpu.h>
#include "vpu_reg.h"
#include "vpu.h"
#include "vpu_ctrl.h"

/* v20181009: init version */
/* v20190313: add sm1 support */
#define VPU_VERION	"v20190313"

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

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = vpu_mem_pd_g12a,
	.hdmi_iso_pre_table = vpu_hdmi_iso_pre_gxb,
	.hdmi_iso_table = vpu_hdmi_iso_gxb,
	.reset_table = vpu_reset_g12a,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,
};

static struct vpu_data_s vpu_data_g12b = {
	.chip_type = VPU_CHIP_G12B,
	.chip_name = "g12b",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = vpu_mem_pd_g12a,
	.hdmi_iso_pre_table = vpu_hdmi_iso_pre_gxb,
	.hdmi_iso_table = vpu_hdmi_iso_gxb,
	.reset_table = vpu_reset_g12a,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,
};

static struct vpu_data_s vpu_data_tl1 = {
	.chip_type = VPU_CHIP_TL1,
	.chip_name = "tl1",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = vpu_mem_pd_tl1,
	.hdmi_iso_pre_table = vpu_hdmi_iso_pre_gxb,
	.hdmi_iso_table = vpu_hdmi_iso_gxb,
	.reset_table = vpu_reset_tl1,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,
};

/* static struct vpu_data_s vpu_data_sm1 = {
	.chip_type = VPU_CHIP_SM1,
	.chip_name = "sm1",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = vpu_mem_pd_tl1,
	.hdmi_iso_pre_table = vpu_hdmi_iso_pre_gxb,
	.hdmi_iso_table = vpu_hdmi_iso_sm1,
	.reset_table = vpu_reset_gx,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,
};
*/

static struct vpu_data_s vpu_data_tm2 = {
	.chip_type = VPU_CHIP_TM2,
	.chip_name = "tm2",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,

	.mem_pd_table = vpu_mem_pd_tm2,
	.hdmi_iso_pre_table = vpu_hdmi_iso_pre_gxb,
	.hdmi_iso_table = vpu_hdmi_iso_sm1,
	.reset_table = vpu_reset_tl1,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,
};

static void vpu_chip_detect(void)
{
#if 1
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
	default:
		vpu_conf.data = &vpu_data_g12a;
		break;
	}
#else
	vpu_conf.data = &vpu_data_g12a;
#endif

	strcpy(vpu_conf.drv_version, VPU_VERION);
#ifdef AML_VPU_CLK_LEVEL_DFT
	vpu_conf.data->clk_level_dft = AML_VPU_CLK_LEVEL_DFT;
	VPUPR("find AML_VPU_CLK_LEVEL_DFT: %d\n", AML_VPU_CLK_LEVEL_DFT);
#endif

#ifdef VPU_DEBUG_PRINT
	VPUPR("driver version: %s\n", vpu_conf.drv_version);
	VPUPR("detect chip type: %d\n", vpu_conf.data->chip_type);
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

	if (vpu_conf.data == NULL) {
		VPUERR("invalid vpu\n");
		return -1;
	}

	switch (vpu_conf.data->chip_type) {
	case VPU_CHIP_G12A:
	case VPU_CHIP_G12B:
	case VPU_CHIP_TL1:
	case VPU_CHIP_SM1:
	case VPU_CHIP_TM2:
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

	mux = vpu_hiu_getb(HHI_VPU_CLK_CNTL, 9, 3);
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

	div = vpu_hiu_getb(HHI_VPU_CLK_CNTL, 0, 7) + 1;
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
	unsigned int mux, div;
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

	vpu_hiu_write(HHI_VPU_CLK_CNTL, ((mux << 9) | (div << 0)));
	vpu_hiu_setb(HHI_VPU_CLK_CNTL, 1, 8, 1);

	/* vapb clk */
	if (vpu_clk >= 250000000) {
		vpu_hiu_write(HHI_VAPBCLK_CNTL, (1 << 30) | /* turn on ge2d clock */
						(0 << 9)  | /* clk_sel    //250Mhz */
						(1 << 0));  /* clk_div */
	} else {
		vpu_hiu_write(HHI_VAPBCLK_CNTL, (1 << 30) | /* turn on ge2d clock */
						(clk_table->mux << 9)  | /* clk_sel */
						(div << 0));  /* clk_div */
	}
	vpu_hiu_setb(HHI_VAPBCLK_CNTL, 1, 8, 1);

	VPUPR("set clk: %uHz, readback: %uHz(0x%x)\n",
		vpu_clk, get_vpu_clk(), (vpu_hiu_read(HHI_VPU_CLK_CNTL)));
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
		ret = 1;
		clk_level = vpu_conf.data->clk_level_dft;
		VPUPR("clk out of supported range, set to default\n");
	}

	ret = adjust_vpu_clk(clk_level);

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
	vpu_power_on();
	set_vpu_clk(vpu_conf.clk_level);
	//vpu_power_on();

	/* vpu module init off, for power save, and special module init */
	vpu_mem_pd_init_off();
	vpu_module_init_config();

	return ret;
}

int vpu_remove(void)
{
	if (vpu_check())
		return -1;

	VPUPR("vpu remove\n");
	vpu_power_off();
	return 0;
}

static void vpu_clk_switch(void)
{
	struct vpu_clk_s *clk_table;
	unsigned int mux, div;

	/* step 1: switch to 2nd vpu clk patch */
	clk_table = vpu_conf.data->vpu_clk_table + vpu_conf.data->clk_level_dft;
	mux = get_vpu_clk_mux(clk_table->mux);
	vpu_hiu_setb(HHI_VPU_CLK_CNTL, mux, 25, 3);
	div = clk_table->div;
	vpu_hiu_setb(HHI_VPU_CLK_CNTL, div, 16, 7);
	vpu_hiu_setb(HHI_VPU_CLK_CNTL, 1, 24, 1);
	vpu_hiu_setb(HHI_VPU_CLK_CNTL, 1, 31, 1);
	udelay(10);
	/* step 2: adjust 1st vpu clk frequency */
	clk_table = vpu_conf.data->vpu_clk_table + vpu_conf.clk_level;
	vpu_hiu_setb(HHI_VPU_CLK_CNTL, 0, 8, 1);
	mux = get_vpu_clk_mux(clk_table->mux);
	vpu_hiu_setb(HHI_VPU_CLK_CNTL, mux, 9, 3);
	div = clk_table->div;
	vpu_hiu_setb(HHI_VPU_CLK_CNTL, div, 0, 7);
	vpu_hiu_setb(HHI_VPU_CLK_CNTL, 1, 8, 1);
	udelay(20);
	/* step 3: switch back to 1st vpu clk patch */
	vpu_hiu_setb(HHI_VPU_CLK_CNTL, 0, 31, 1);
	vpu_hiu_setb(HHI_VPU_CLK_CNTL, 0, 24, 1);
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
		vpu_clk, get_vpu_clk(), vpu_hiu_read(HHI_VPU_CLK_CNTL));
	return 0;
}

void vpu_clk_get(void)
{
	if (vpu_check())
		return;

	VPUPR("clk_level: %u, clk: %uHz, reg: 0x%x\n",
		vpu_conf.clk_level, get_vpu_clk(),
		vpu_hiu_read(HHI_VPU_CLK_CNTL));
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
	0x1b7f, /* VENC_VDAC_TST_VAL */
	0x1c30, /* ENCP_DVI_HSO_BEGIN */
	0x1d00, /* VPP_DUMMY_DATA */
	0x2730, /* VPU_VPU_PWM_V0 */
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
