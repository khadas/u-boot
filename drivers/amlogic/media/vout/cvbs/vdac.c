// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <asm/arch/cpu.h>
#include <amlogic/cpu_id.h>
#include <amlogic/aml_efuse.h>
#include "vdac.h"
#include "cvbs_reg.h"

static struct vdac_data_s *vdac_data;
static unsigned int pri_flag;

#define REG_ADDR_VDAC(reg)               (reg + 0L)

static void vdac_write_reg(unsigned int addr, unsigned int val)
{
	*(volatile unsigned int *)REG_ADDR_VDAC(addr) = (val);
}

static unsigned int vdac_read_reg(unsigned int addr)
{
	return *(volatile unsigned int *)(REG_ADDR_VDAC(addr));
}

static void vdac_set_reg_bits(unsigned int addr, unsigned int val,
			      unsigned int start, unsigned int len)
{
	vdac_write_reg(addr, ((vdac_read_reg(addr) &
			~(((1L << (len))-1) << (start))) |
			(((val)&((1L<<(len))-1)) << (start))));
}

static inline unsigned int vdac_get_reg_bits(unsigned int reg,
		unsigned int start, unsigned int len)
{
	unsigned int val;

	val = ((vdac_read_reg(reg) >> (start)) & ((1L << (len)) - 1));

	return val;
}

static int vdac_ctrl_config(bool on, unsigned int reg, unsigned int bit)
{
	struct meson_vdac_ctrl_s *vdac_ctrl;
	unsigned int val;
	int i = 0;
	int ret = -1;

	if (!vdac_data) {
		printf("%s: vdac_data is NULL\n", __func__);
		return ret;
	}

	if (!vdac_data->vdac_ctrl) {
		printf("%s: vdac_ctrl is NULL\n", __func__);
		return ret;
	}

	vdac_ctrl = vdac_data->vdac_ctrl;
	while (i < VDAC_CTRL_MAX) {
		if (vdac_ctrl[i].reg == VDAC_REG_MAX)
			break;
		if ((vdac_ctrl[i].reg == reg) && (vdac_ctrl[i].bit == bit)) {
			if (on)
				val = vdac_ctrl[i].val;
			else
				val = vdac_ctrl[i].val ? 0 : 1;
			vdac_set_reg_bits(reg, val, bit, vdac_ctrl[i].len);
			//if (vdac_debug_print) {
			//	printf("vdac: reg=0x%02x set bit%d=%d, readback=0x%08x\n",
			//		reg, bit, val, vdac_hiu_reg_read(reg));
			//}
			ret = 0;
			break;
		}
		i++;
	}

	return ret;
}

static void vdac_enable_dac_input(unsigned int reg_cntl0)
{
	vdac_set_reg_bits(reg_cntl0, 0x2, 0, 3);
	vdac_set_reg_bits(reg_cntl0, 0x1, 4, 1);
	vdac_set_reg_bits(reg_cntl0, 0x1, 6, 1);
	vdac_set_reg_bits(reg_cntl0, 0x3, 13, 3);
	vdac_set_reg_bits(reg_cntl0, 0x10, 18, 5);
}

static void vdac_enable_cvbs_out(bool on)
{
	unsigned int reg_cntl0;
	unsigned int reg_cntl1;

	if (!vdac_data) {
		printf("%s: vdac_data is NULL\n", __func__);
		return;
	}

	reg_cntl0 = vdac_data->reg_ctrl0;
	reg_cntl1 = vdac_data->reg_ctrl1;

	if (on) {
		if (vdac_data->cpu_id >= VDAC_CPU_S4) {
			vdac_enable_dac_input(reg_cntl0);
			vdac_ctrl_config(1, reg_cntl1, 7);
		} else {
			vdac_set_reg_bits(reg_cntl0, 0x6, 12, 4);
			vdac_ctrl_config(1, reg_cntl1, 3);
			vdac_ctrl_config(1, reg_cntl0, 0);
			vdac_ctrl_config(1, reg_cntl0, 9);
		}
	} else {
		if (vdac_data->cpu_id >= VDAC_CPU_S4) {
			vdac_set_reg_bits(reg_cntl0, 0x0, 4, 1);
			vdac_ctrl_config(0, reg_cntl1, 7);
		} else {
			vdac_ctrl_config(0, reg_cntl0, 9);
			vdac_ctrl_config(0, reg_cntl0, 0);
			vdac_ctrl_config(0, reg_cntl1, 3);
		}
	}
}

void vdac_enable(bool on, unsigned int module_sel)
{
	if (!vdac_data) {
		printf("%s: vdac_data is NULL\n", __func__);
		return;
	}

	printf("%s: %d, module_sel:0x%x\n", __func__, on, module_sel);

	switch (module_sel) {
	case VDAC_MODULE_CVBS_OUT:
		if (on) {
			pri_flag |= VDAC_MODULE_CVBS_OUT;
			vdac_enable_cvbs_out(1);
		} else {
			pri_flag &= ~VDAC_MODULE_CVBS_OUT;
			vdac_enable_cvbs_out(0);
		}
		break;
	default:
		printf("%s:module_sel: 0x%x wrong module index !! ",
			__func__, module_sel);
		break;
	}

	if (vdac_data->cpu_id >= VDAC_CPU_S4) {
		if (!vdac_get_reg_bits(vdac_data->reg_ctrl0, 11, 1))
			vdac_set_reg_bits(vdac_data->reg_ctrl0, 1, 11, 1);
	}
}

int vdac_ctrl_vref_adj(unsigned int value)
{
	struct meson_vdac_ctrl_s *vdac_ctrl;
	unsigned int reg;
	unsigned int bit = 16;
	int i = 0;
	int ret = -1;

	if (!vdac_data) {
		printf("%s: vdac_data is NULL\n", __func__);
		return ret;
	}

	if (!vdac_data->vdac_ctrl) {
		printf("%s: vdac_ctrl is NULL\n", __func__);
		return ret;
	}

	vdac_ctrl = vdac_data->vdac_ctrl;
	reg = vdac_data->reg_ctrl0;
	while (i < VDAC_CTRL_MAX) {
		if (vdac_ctrl[i].reg == VDAC_REG_MAX)
			break;
		if ((vdac_ctrl[i].reg == reg) && (vdac_ctrl[i].bit == bit)) {
			vdac_set_reg_bits(reg, value, bit, vdac_ctrl[i].len);
			//if (vdac_debug_print) {
			//	printf("vdac: reg=0x%x set bit%d=0x%x, readback=0x%08x\n",
			//		reg, bit, value, vdac_hiu_reg_read(reg));
			//}
			ret = 0;
			break;
		}
		i++;
	}

	return ret;
}

static void vdac_gsw_init(void)
{
	struct meson_vdac_ctrl_s *vdac_ctrl;
	unsigned int reg;
	unsigned int bit = 0;
	int i = 0;
	int ret;

	if (!vdac_data) {
		printf("%s: vdac_data is NULL\n", __func__);
		return;
	}

	if (!vdac_data->vdac_ctrl) {
		printf("%s: vdac_ctrl is NULL\n", __func__);
		return;
	}

	ret = efuse_get_cali_cvbs();
	if (ret == -1) {
		//printf("%s: no cali_cvbs data\n", __func__);
		return;
	}

	vdac_ctrl = vdac_data->vdac_ctrl;
	reg = vdac_data->reg_ctrl1;

	printf("%s: 0x%x\n", __func__, ret);
	while (i < VDAC_CTRL_MAX) {
		if (vdac_ctrl[i].reg == VDAC_REG_MAX)
			break;
		if ((vdac_ctrl[i].reg == reg) && (vdac_ctrl[i].bit == bit)) {
			vdac_set_reg_bits(reg, ret, bit, vdac_ctrl[i].len);
			//if (vdac_debug_print) {
			//	printf("vdac: reg=0x%x set bit%d=0x%x, readback=0x%08x\n",
			//		reg, bit, ret, vdac_hiu_reg_read(reg));
			//}
			break;
		}
		i++;
	}

	return;
}

static struct meson_vdac_ctrl_s vdac_ctrl_enable_g12ab[] = {
	{HHI_VDAC_CNTL0, 0, 9, 1},
	{HHI_VDAC_CNTL0, 1, 0, 1},
	{HHI_VDAC_CNTL0, 0, 16, 5}, /* vref adj */
	{HHI_VDAC_CNTL1, 0, 0, 3},  /*gsw */
	{HHI_VDAC_CNTL1, 0, 3, 1},
	{VDAC_REG_MAX, 0, 0, 0},
};

static struct meson_vdac_ctrl_s vdac_ctrl_enable_sc2[] = {
	{ANACTRL_VDAC_CTRL0, 0, 9, 1},
	{ANACTRL_VDAC_CTRL0, 1, 0, 1},
	{ANACTRL_VDAC_CTRL0, 0, 16, 5}, /* vref adj */
	{ANACTRL_VDAC_CTRL1, 0, 0, 3},  /*gsw */
	{ANACTRL_VDAC_CTRL1, 0, 3, 1},
	{ANACTRL_VDAC_CTRL1, 0, 7, 1}, /* bandgap */
	{VDAC_REG_MAX, 0, 0, 0},
};

static struct meson_vdac_ctrl_s vdac_ctrl_enable_s4[] = {
	{ANACTRL_VDAC_CTRL0, 0, 9, 1},
	{ANACTRL_VDAC_CTRL0, 1, 0, 1},
	{ANACTRL_VDAC_CTRL1, 0, 0, 7},  /*gsw */
	{ANACTRL_VDAC_CTRL1, 1, 7, 1}, /* cdac_pwd */
	{VDAC_REG_MAX, 0, 0, 0},
};

struct vdac_data_s vdac_data_g12ab = {
	.cpu_id = VDAC_CPU_G12AB,
	.reg_ctrl0 = HHI_VDAC_CNTL0,
	.reg_ctrl1 = HHI_VDAC_CNTL1,
	.vdac_ctrl = vdac_ctrl_enable_g12ab,
};

struct vdac_data_s vdac_data_sc2 = {
	.cpu_id = VDAC_CPU_SC2,
	.reg_ctrl0 = ANACTRL_VDAC_CTRL0,
	.reg_ctrl1 = ANACTRL_VDAC_CTRL1,
	.vdac_ctrl = vdac_ctrl_enable_sc2,
};

struct vdac_data_s vdac_data_s4 = {
	.cpu_id = VDAC_CPU_S4,
	.reg_ctrl0 = ANACTRL_VDAC_CTRL0,
	.reg_ctrl1 = ANACTRL_VDAC_CTRL1,
	.vdac_ctrl = vdac_ctrl_enable_s4,
};

struct vdac_data_s vdac_data_s4d = {
	.cpu_id = VDAC_CPU_S4D,
	.reg_ctrl0 = ANACTRL_VDAC_CTRL0,
	.reg_ctrl1 = ANACTRL_VDAC_CTRL1,
	.vdac_ctrl = vdac_ctrl_enable_s4,
};

void vdac_ctrl_config_probe(void)
{
	pri_flag = 0;

	switch (get_cpu_id().family_id) {
	case MESON_CPU_MAJOR_ID_G12A:
	case MESON_CPU_MAJOR_ID_G12B:
		vdac_data = &vdac_data_g12ab;
		break;
	case MESON_CPU_MAJOR_ID_SC2:
		vdac_data = &vdac_data_sc2;
		break;
	case MESON_CPU_MAJOR_ID_S4:
		vdac_data = &vdac_data_s4;
		break;
	case MESON_CPU_MAJOR_ID_S4D:
		vdac_data = &vdac_data_s4d;
		break;
	default:
		vdac_data = &vdac_data_s4d;
		break;
	}

	vdac_gsw_init();

	return;
}
