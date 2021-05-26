/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/display/vout/vdac.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/io.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/cpu.h>
#include <asm/cpu_id.h>
#include "vdac.h"

static struct meson_vdac_ctrl_s *vdac_ctrl;
static unsigned int pri_flag;

#define REG_ADDR_HIU(reg)               (reg + 0L)

static void vdac_write_hiu(unsigned int addr, unsigned int val)
{
	*(volatile unsigned int *)REG_ADDR_HIU(addr) = (val);
}

static unsigned int vdac_read_hiu(unsigned int addr)
{
	return *(volatile unsigned int *)(REG_ADDR_HIU(addr));
}

static void vdac_set_hiu_bits(unsigned int addr, unsigned int val,
			     unsigned int start, unsigned int len)
{
	vdac_write_hiu(addr, ((vdac_read_hiu(addr) &
			~(((1L << (len))-1) << (start))) |
			(((val)&((1L<<(len))-1)) << (start))));
}

static int vdac_ctrl_config(bool on, unsigned int reg, unsigned int bit)
{
	unsigned int val;
	int i = 0;
	int ret = -1;

	if (!vdac_ctrl) {
		printf("%s: vdac_ctrl data is NULL\n", __func__);
		return ret;
	}

	while (i < VDAC_CTRL_MAX) {
		if (vdac_ctrl[i].reg == VDAC_REG_MAX)
			break;
		if ((vdac_ctrl[i].reg == reg) && (vdac_ctrl[i].bit == bit)) {
			if (on)
				val = vdac_ctrl[i].val;
			else
				val = vdac_ctrl[i].val ? 0 : 1;
			vdac_set_hiu_bits(reg, val, bit, vdac_ctrl[i].len);
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
	unsigned int reg_cntl0 = HHI_VDAC_CNTL0;
	unsigned int reg_cntl1 = HHI_VDAC_CNTL1;

	if (on) {
		if (get_cpu_id().family_id <= MESON_CPU_MAJOR_ID_GXLX)
			vdac_set_hiu_bits(reg_cntl0, 0, 12, 4);
		else
			vdac_set_hiu_bits(reg_cntl0, 0x6, 12, 4);
		vdac_ctrl_config(1, reg_cntl1, 3);
		vdac_ctrl_config(1, reg_cntl0, 0);
		vdac_ctrl_config(1, reg_cntl0, 9);
		if (get_cpu_id().family_id == MESON_CPU_MAJOR_ID_TXLX) {
			vdac_ctrl_config(0, reg_cntl0, 13);
			udelay(5);
			vdac_ctrl_config(1, reg_cntl0, 13);
		}
		if (get_cpu_id().family_id < MESON_CPU_MAJOR_ID_G12A)
			vdac_ctrl_config(0, reg_cntl0, 10);
	} else {
		vdac_ctrl_config(0, reg_cntl0, 9);
		vdac_ctrl_config(0, reg_cntl0, 0);
		vdac_ctrl_config(0, reg_cntl1, 3);
	}
}

void vdac_enable(bool on, unsigned int module_sel)
{
	if (!vdac_ctrl) {
		printf("%s: vdac_ctrl data is NULL\n", __func__);
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

int vdac_vref_adj(unsigned int value)
{
	unsigned int reg = HHI_VDAC_CNTL0;
	unsigned int bit = 16;
	int i = 0;
	int ret = -1;

	if (!vdac_ctrl) {
		printf("%s: vdac_ctrl data is NULL\n", __func__);
		return ret;
	}

	while (i < VDAC_CTRL_MAX) {
		if (vdac_ctrl[i].reg == VDAC_REG_MAX)
			break;
		if ((vdac_ctrl[i].reg == reg) && (vdac_ctrl[i].bit == bit)) {
			vdac_set_hiu_bits(reg, value, bit, vdac_ctrl[i].len);
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

static struct meson_vdac_ctrl_s vdac_ctrl_enable_gxl[] = {
	{HHI_VDAC_CNTL0, 0, 9, 1},
	{HHI_VDAC_CNTL0, 1, 10, 1},
	{HHI_VDAC_CNTL0, 1, 0, 1},
	{HHI_VDAC_CNTL0, 0, 16, 5}, /* vref adj */
	{HHI_VDAC_CNTL1, 0, 0, 3},  /*gsw */
	{HHI_VDAC_CNTL1, 0, 3, 1},
	{VDAC_REG_MAX, 0, 0, 0},
};

static struct meson_vdac_ctrl_s vdac_ctrl_enable_txl[] = {
	{HHI_VDAC_CNTL0, 1, 9, 1},
	{HHI_VDAC_CNTL0, 1, 10, 1},
	{HHI_VDAC_CNTL0, 1, 0, 1},
	{HHI_VDAC_CNTL0, 0, 16, 5}, /* vref adj */
	{HHI_VDAC_CNTL1, 0, 0, 3},  /*gsw */
	{HHI_VDAC_CNTL1, 1, 3, 1},
	{VDAC_REG_MAX, 0, 0, 0},
};

static struct meson_vdac_ctrl_s vdac_ctrl_enable_txlx[] = {
	{HHI_VDAC_CNTL0, 1, 9, 1},
	{HHI_VDAC_CNTL0, 1, 10, 1},
	{HHI_VDAC_CNTL0, 1, 0, 1},
	{HHI_VDAC_CNTL0, 0, 13, 1}, /* bandgap */
	{HHI_VDAC_CNTL0, 0, 16, 5}, /* vref adj */
	{HHI_VDAC_CNTL1, 0, 0, 3},  /*gsw */
	{HHI_VDAC_CNTL1, 1, 3, 1},
	{VDAC_REG_MAX, 0, 0, 0},
};

static struct meson_vdac_ctrl_s vdac_ctrl_enable_g12ab[] = {
	{HHI_VDAC_CNTL0, 0, 9, 1},
	{HHI_VDAC_CNTL0, 1, 0, 1},
	{HHI_VDAC_CNTL0, 0, 16, 5}, /* vref adj */
	{HHI_VDAC_CNTL1, 0, 0, 3},  /*gsw */
	{HHI_VDAC_CNTL1, 0, 3, 1},
	{VDAC_REG_MAX, 0, 0, 0},
};

static struct meson_vdac_ctrl_s vdac_ctrl_enable_tl1[] = {
	{HHI_VDAC_CNTL0, 0, 9, 1},
	{HHI_VDAC_CNTL0, 1, 0, 1},
	{HHI_VDAC_CNTL0, 0, 16, 5}, /* vref adj */
	{HHI_VDAC_CNTL1, 0, 0, 3},  /*gsw */
	{HHI_VDAC_CNTL1, 0, 3, 1},
	{HHI_VDAC_CNTL1, 0, 7, 1}, /* bandgap */
	{VDAC_REG_MAX, 0, 0, 0},
};

void vdac_ctrl_config_probe(void)
{
	pri_flag = 0;

	switch (get_cpu_id().family_id) {
	case MESON_CPU_MAJOR_ID_GXL:
	case MESON_CPU_MAJOR_ID_GXM:
	case MESON_CPU_MAJOR_ID_GXLX:
		vdac_ctrl = vdac_ctrl_enable_gxl;
		break;
	case MESON_CPU_MAJOR_ID_TXL:
	case MESON_CPU_MAJOR_ID_TXHD:
		vdac_ctrl = vdac_ctrl_enable_txl;
		break;
	case MESON_CPU_MAJOR_ID_TXLX:
		vdac_ctrl = vdac_ctrl_enable_txlx;
		break;
	case MESON_CPU_MAJOR_ID_G12A:
	case MESON_CPU_MAJOR_ID_G12B:
	case MESON_CPU_MAJOR_ID_SM1:
		vdac_ctrl = vdac_ctrl_enable_g12ab;
		break;
	case MESON_CPU_MAJOR_ID_TL1:
	case MESON_CPU_MAJOR_ID_TM2:
		vdac_ctrl = vdac_ctrl_enable_tl1;
		break;
	default:
		break;
	}

	return;
}
