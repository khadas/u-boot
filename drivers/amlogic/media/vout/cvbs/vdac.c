/*
 * drivers/amlogic/media/vout/vdac.c
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
 * Author: jets.yan@amlogic.com
 *
*/

#include <common.h>
#include <asm/arch/cpu.h>
#include <amlogic/cpu_id.h>
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
		vdac_set_reg_bits(reg_cntl0, 0x6, 12, 4);
		vdac_ctrl_config(1, reg_cntl1, 3);
		vdac_ctrl_config(1, reg_cntl0, 0);
		vdac_ctrl_config(1, reg_cntl0, 9);
	} else {
		vdac_ctrl_config(0, reg_cntl0, 9);
		vdac_ctrl_config(0, reg_cntl0, 0);
		vdac_ctrl_config(0, reg_cntl1, 3);
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

struct vdac_data_s vdac_data_g12ab = {
	.reg_ctrl0 = HHI_VDAC_CNTL0,
	.reg_ctrl1 = HHI_VDAC_CNTL1,
	.vdac_ctrl = vdac_ctrl_enable_g12ab,
};

struct vdac_data_s vdac_data_sc2 = {
	.reg_ctrl0 = ANACTRL_VDAC_CTRL0,
	.reg_ctrl1 = ANACTRL_VDAC_CTRL1,
	.vdac_ctrl = vdac_ctrl_enable_sc2,
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
	default:
		break;
	}

	return;
}
