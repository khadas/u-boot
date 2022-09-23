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
/* v20220503: add c3 support */
#define VPU_VERION	"v20220503"


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
	.test_reg = vcbus_test_reg,

	.mem_pd_table = vpu_mem_pd_g12a,
	.power_table = vpu_pwr_gxb,
	.iso_table = vpu_iso_gxb,
	.reset_table = vpu_reset_g12a,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on,
	.power_off = vpu_power_off,
	.mem_pd_init_off = vpu_mem_pd_init_off,
	.module_init_config = vpu_module_init_config,
	.change_clk = change_vpu_clk,
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
	.test_reg = vcbus_test_reg,

	.mem_pd_table = vpu_mem_pd_g12a,
	.power_table = vpu_pwr_gxb,
	.iso_table = vpu_iso_gxb,
	.reset_table = vpu_reset_g12a,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on,
	.power_off = vpu_power_off,
	.mem_pd_init_off = vpu_mem_pd_init_off,
	.module_init_config = vpu_module_init_config,
	.change_clk = change_vpu_clk,
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
	.test_reg = vcbus_test_reg,

	.mem_pd_table = vpu_mem_pd_tl1,
	.power_table = vpu_pwr_gxb,
	.iso_table = vpu_iso_gxb,
	.reset_table = vpu_reset_tl1,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on,
	.power_off = vpu_power_off,
	.mem_pd_init_off = vpu_mem_pd_init_off,
	.module_init_config = vpu_module_init_config,
	.change_clk = change_vpu_clk,
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
	.test_reg = vcbus_test_reg,

	.mem_pd_table = vpu_mem_pd_tl1,
	.power_table = vpu_pwr_gxb,
	.iso_table = vpu_iso_sm1,
	.reset_table = vpu_reset_gx,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on,
	.power_off = vpu_power_off,
	.mem_pd_init_off = vpu_mem_pd_init_off,
	.module_init_config = vpu_module_init_config,
	.change_clk = change_vpu_clk,
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
	.test_reg = vcbus_test_reg,

	.mem_pd_table = vpu_mem_pd_tm2,
	.power_table = vpu_pwr_gxb,
	.iso_table = vpu_iso_sm1,
	.reset_table = vpu_reset_tl1,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on,
	.power_off = vpu_power_off,
	.mem_pd_init_off = vpu_mem_pd_init_off,
	.module_init_config = vpu_module_init_config,
	.change_clk = change_vpu_clk,
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
	.test_reg = vcbus_test_reg,

	.mem_pd_table = NULL,
	.power_table = NULL,
	.iso_table = NULL,
	.reset_table = NULL,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on_new,
	.power_off = vpu_power_off_new,
	.mem_pd_init_off = vpu_mem_pd_init_off,
	.module_init_config = vpu_module_init_config,
	.change_clk = change_vpu_clk,
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
	.test_reg = vcbus_test_reg,

	.mem_pd_table = vpu_mem_pd_tm2,
	.power_table = vpu_pwr_gxb,
	.iso_table = vpu_iso_sm1,
	.reset_table = vpu_reset_tl1,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on_new,
	.power_off = vpu_power_off_new,
	.mem_pd_init_off = vpu_mem_pd_init_off,
	.module_init_config = vpu_module_init_config,
	.change_clk = change_vpu_clk,
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
	.test_reg = vcbus_test_reg,

	.mem_pd_table = NULL,
	.power_table = NULL,
	.iso_table = NULL,
	.reset_table = NULL,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on_new,
	.power_off = vpu_power_off_new,
	.mem_pd_init_off = vpu_mem_pd_init_off,
	.module_init_config = vpu_module_init_config,
	.change_clk = change_vpu_clk,
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
	.test_reg = vcbus_test_reg,

	.mem_pd_table = NULL,
	.power_table = NULL,
	.iso_table = NULL,
	.reset_table = NULL,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on_new,
	.power_off = vpu_power_off_new,
	.mem_pd_init_off = vpu_mem_pd_init_off,
	.module_init_config = vpu_module_init_config,
	.change_clk = change_vpu_clk,
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
	.test_reg = vcbus_test_reg,

	.mem_pd_table = NULL,
	.power_table = NULL,
	.iso_table = NULL,
	.reset_table = NULL,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on_new,
	.power_off = vpu_power_off_new,
	.mem_pd_init_off = vpu_mem_pd_init_off,
	.module_init_config = vpu_module_init_config,
	.change_clk = change_vpu_clk,
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
	.test_reg = vcbus_test_reg,

	.mem_pd_table = NULL,
	.power_table = NULL,
	.iso_table = NULL,
	.reset_table = NULL,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on_new,
	.power_off = vpu_power_off_new,
	.mem_pd_init_off = vpu_mem_pd_init_off,
	.module_init_config = vpu_module_init_config,
	.change_clk = change_vpu_clk,
};

static struct vpu_data_s vpu_data_s4d = {
	.chip_type = VPU_CHIP_S4D,
	.chip_name = "s4d",
	.clk_level_dft = CLK_LEVEL_DFT_G12A,
	.clk_level_max = CLK_LEVEL_MAX_G12A,
	.gp_pll_valid = 0,

	.vpu_clk_reg = CLKCTRL_VPU_CLK_CTRL,
	.vpu_clkb_reg = VPU_REG_END,
	.vapb_clk_reg = CLKCTRL_VAPBCLK_CTRL,
	.vid_clk_reg = CLKCTRL_VID_CLK_CTRL2,

	.pwrctrl_id_table = vpu_pwrctrl_id_table,

	.fclk_div_table = fclk_div_table_g12a,
	.vpu_clk_table = vpu_clk_table,
	.test_reg = vcbus_test_reg,

	.mem_pd_table = NULL,
	.power_table = NULL,
	.iso_table = NULL,
	.reset_table = NULL,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = vpu_power_on_new,
	.power_off = vpu_power_off_new,
	.mem_pd_init_off = vpu_mem_pd_init_off,
	.module_init_config = vpu_module_init_config,
	.change_clk = change_vpu_clk,
};

static struct vpu_data_s vpu_data_c3 = {
	.chip_type = VPU_CHIP_C3,
	.chip_name = "c3",
	.clk_level_dft = CLK_LEVEL_DFT_C3,
	.clk_level_max = CLK_LEVEL_MAX_C3,
	.gp_pll_valid = 0,

	.vpu_clk_reg = CLKCTRL_VOUTENC_CLK_CTRL,
	.vpu_clkb_reg = VPU_REG_END,
	.vapb_clk_reg = VPU_REG_END,
	.vid_clk_reg = VPU_REG_END,

	.pwrctrl_id_table = NULL,

	.fclk_div_table = fclk_div_table_c3,
	.vpu_clk_table = vpu_clk_table,
	.test_reg = vcbus_test_reg_c3,

	.mem_pd_table = NULL,
	.power_table = NULL,
	.iso_table = NULL,
	.reset_table = NULL,

	.module_init_table_cnt = 0,
	.module_init_table = NULL,

	.power_on = NULL,
	.power_off = vpu_power_off_c3,
	.mem_pd_init_off = NULL,
	.module_init_config = NULL,
	.change_clk = NULL,
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
	case MESON_CPU_MAJOR_ID_C3:
		vpu_conf.data = &vpu_data_c3;
		break;
	default:
		vpu_conf.data = NULL;
		//vpu_conf.data = &vpu_data_t3;
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
	case VPU_CHIP_C3:
		ret = 0;
		break;
	default:
		VPUERR("invalid vpu for current chip\n");
		break;
	}

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
	set_vpu_clk(&vpu_conf, vpu_conf.clk_level);
	set_vpu_clkb(&vpu_conf, vpu_conf.clk_level);

	/* vpu module init off, for power save, and special module init */
	if (vpu_conf.data->mem_pd_init_off)
		vpu_conf.data->mem_pd_init_off();
	if (vpu_conf.data->module_init_config)
		vpu_conf.data->module_init_config();
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

void vpu_clk_get(void)
{
	if (vpu_check())
		return;

	VPUPR("clk_level: %u, clk: %uHz, reg: 0x%x\n",
		vpu_conf.clk_level, get_vpu_clk(&vpu_conf),
		vpu_hiu_read(vpu_conf.data->vpu_clk_reg));
}

int vpu_clk_change(int clk)
{
	int ret;

	if (vpu_check())
		return -1;

	if (!vpu_conf.data->change_clk) {
		VPUERR("%s: don't support change clk\n", __func__);
		return -1;
	}

	ret = vpu_conf.data->change_clk(&vpu_conf, clk);
	return ret;
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

void vcbus_test(void)
{
	unsigned int *test_table = vpu_conf.data->test_reg;
	unsigned int val;
	unsigned int temp;
	int i,j;

	if (vpu_check())
		return;
	if (!test_table)
		return;

	VPUPR("vcbus test:\n");
	for (i = 0; i < VCBUS_REG_CNT_MAX; i++) {
		for (j = 0; j < 24; j++) {
			val = vpu_vcbus_read(test_table[i]);
			printf("%02d read 0x%04x=0x%08x\n", j, test_table[i], val);
		}
		printf("\n");
	}
	temp = 0x5a5a5a5a;
	for (i = 0; i < VCBUS_REG_CNT_MAX; i++) {
		vpu_vcbus_write(test_table[i], temp);
		val = vpu_vcbus_read(test_table[i]);
		printf("write 0x%04x=0x%08x, readback: 0x%08x\n", test_table[i], temp, val);
	}
	for (i = 0; i < VCBUS_REG_CNT_MAX; i++) {
		for (j = 0; j < 24; j++) {
			val = vpu_vcbus_read(test_table[i]);
			printf("%02d read 0x%04x=0x%08x\n", j, test_table[i], val);
		}
		printf("\n");
	}
}
