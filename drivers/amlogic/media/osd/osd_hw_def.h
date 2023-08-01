/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _OSD_HW_DEF_H_
#define	_OSD_HW_DEF_H_

#include "osd_hw.h"

static void osd1_update_color_mode(void);
static void osd1_update_enable(void);
static void osd1_update_color_key(void);
static void osd1_update_color_key_enable(void);
static void osd1_update_gbl_alpha(void);
static void osd1_update_order(void);
static void osd1_update_disp_geometry(void);
static void osd1_update_coef(void);
static void osd1_update_disp_freescale_enable(void);
static void osd1_update_disp_osd_reverse(void);
static void osd1_update_disp_osd_rotate(void);
static void osd1_update_disp_scale_enable(void);
static void osd1_update_disp_3d_mode(void);

static void osd2_update_color_mode(void);
static void osd2_update_enable(void);
static void osd2_update_color_key(void);
static void osd2_update_color_key_enable(void);
static void osd2_update_gbl_alpha(void);
static void osd2_update_order(void);
static void osd2_update_disp_geometry(void);
static void osd2_update_coef(void);
static void osd2_update_disp_freescale_enable(void);
static void osd2_update_disp_osd_reverse(void);
static void osd2_update_disp_osd_rotate(void);
static void osd2_update_disp_scale_enable(void);
static void osd2_update_disp_3d_mode(void);

static void osd3_update_color_mode(void);
static void osd3_update_enable(void);
static void osd3_update_color_key(void);
static void osd3_update_color_key_enable(void);
static void osd3_update_gbl_alpha(void);
static void osd3_update_order(void);
static void osd3_update_disp_geometry(void);
static void osd3_update_coef(void);
static void osd3_update_disp_freescale_enable(void);
static void osd3_update_disp_osd_reverse(void);
static void osd3_update_disp_osd_rotate(void);
static void osd3_update_disp_scale_enable(void);
static void osd3_update_disp_3d_mode(void);

static void osd4_update_color_mode(void);
static void osd4_update_enable(void);
static void osd4_update_color_key(void);
static void osd4_update_color_key_enable(void);
static void osd4_update_gbl_alpha(void);
static void osd4_update_order(void);
static void osd4_update_disp_geometry(void);
static void osd4_update_coef(void);
static void osd4_update_disp_freescale_enable(void);
static void osd4_update_disp_osd_reverse(void);
static void osd4_update_disp_osd_rotate(void);
static void osd4_update_disp_scale_enable(void);
static void osd4_update_disp_3d_mode(void);

extern struct hw_para_s osd_hw;

static update_func_t hw_func_array[HW_OSD_COUNT][HW_REG_INDEX_MAX] = {
	{
		osd1_update_color_mode,
		osd1_update_enable,
		osd1_update_color_key,
		osd1_update_color_key_enable,
		osd1_update_gbl_alpha,
		osd1_update_order,
		osd1_update_coef,
		osd1_update_disp_geometry,
		osd1_update_disp_scale_enable,
		osd1_update_disp_freescale_enable,
		osd1_update_disp_osd_reverse,
		osd1_update_disp_osd_rotate,
	},
	{
		osd2_update_color_mode,
		osd2_update_enable,
		osd2_update_color_key,
		osd2_update_color_key_enable,
		osd2_update_gbl_alpha,
		osd2_update_order,
		osd2_update_coef,
		osd2_update_disp_geometry,
		osd2_update_disp_scale_enable,
		osd2_update_disp_freescale_enable,
		osd2_update_disp_osd_reverse,
		osd2_update_disp_osd_rotate,
	},
	{
		osd3_update_color_mode,
		osd3_update_enable,
		osd3_update_color_key,
		osd3_update_color_key_enable,
		osd3_update_gbl_alpha,
		osd3_update_order,
		osd3_update_coef,
		osd3_update_disp_geometry,
		osd3_update_disp_scale_enable,
		osd3_update_disp_freescale_enable,
		osd3_update_disp_osd_reverse,
		osd3_update_disp_osd_rotate,
	},
	{
		osd4_update_color_mode,
		osd4_update_enable,
		osd4_update_color_key,
		osd4_update_color_key_enable,
		osd4_update_gbl_alpha,
		osd4_update_order,
		osd4_update_coef,
		osd4_update_disp_geometry,
		osd4_update_disp_scale_enable,
		osd4_update_disp_freescale_enable,
		osd4_update_disp_osd_reverse,
		osd4_update_disp_osd_rotate,
	},
};

#define add_to_update_list(osd_idx, cmd_idx) \
	do { \
		osd_hw.updated[osd_idx] |= (1<<cmd_idx); \
	} while (0)

#define remove_from_update_list(osd_idx, cmd_idx) \
	(osd_hw.updated[osd_idx] &= ~(1<<cmd_idx))

#endif
