// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/io.h>
#ifdef CONFIG_AML_VPP
#include <amlogic/media/vpp/vpp.h>
#endif
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "../lcd_common.h"
#include "lcd_venc.h"

static struct lcd_venc_op_s lcd_venc_op = {
	.init_flag = 0,
	.wait_vsync = NULL,
	.venc_debug_test = NULL,
	.venc_set_timing = NULL,
	.venc_set = NULL,
	.venc_enable = NULL,
	.mute_set = NULL,
};

void lcd_wait_vsync(struct aml_lcd_drv_s *pdrv)
{
#ifndef CONFIG_AML_LCD_PXP
	return;
#endif
	if (!lcd_venc_op.wait_vsync)
		return;

	lcd_venc_op.wait_vsync(pdrv);
}

void lcd_debug_test(struct aml_lcd_drv_s *pdrv, unsigned int num)
{
	int ret;

	if (!lcd_venc_op.venc_debug_test) {
		LCDERR("[%d]: %s: invalid\n", pdrv->index, __func__);
		return;
	}

	ret = lcd_venc_op.venc_debug_test(pdrv, num);
	if (ret) {
		LCDERR("[%d]: %s: %d not support\n", pdrv->index, __func__, num);
		return;
	}

	if (num == 0)
		LCDPR("[%d]: disable test pattern\n", pdrv->index);
}

static void lcd_gamma_init(struct aml_lcd_drv_s *pdrv)
{
#ifndef CONFIG_AML_LCD_PXP
	return;
#endif

#ifdef CONFIG_AML_VPP
	lcd_wait_vsync(pdrv);
	vpp_disable_lcd_gamma_table(pdrv->index);

	vpp_init_lcd_gamma_table(pdrv->index);

	lcd_wait_vsync(pdrv);
	vpp_enable_lcd_gamma_table(pdrv->index);
#endif
}

void lcd_set_venc_timing(struct aml_lcd_drv_s *pdrv)
{
	if (!lcd_venc_op.venc_set_timing)
		return;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);
	lcd_venc_op.venc_set_timing(pdrv);
}

void lcd_set_venc(struct aml_lcd_drv_s *pdrv)
{
	if (!lcd_venc_op.venc_set) {
		LCDERR("[%d]: %s: invalid\n", pdrv->index, __func__);
		return;
	}

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);
	lcd_venc_op.venc_set(pdrv);

	lcd_gamma_init(pdrv);
}

void lcd_venc_enable(struct aml_lcd_drv_s *pdrv, int flag)
{
	if (!lcd_venc_op.venc_enable) {
		LCDERR("[%d]: %s: invalid\n", pdrv->index, __func__);
		return;
	}

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s: %d\n", pdrv->index, __func__, flag);
	lcd_venc_op.venc_enable(pdrv, flag);
}

void lcd_mute_set(struct aml_lcd_drv_s *pdrv,  unsigned char flag)
{
	if (!lcd_venc_op.mute_set) {
		LCDERR("[%d]: %s: invalid\n", pdrv->index, __func__);
		return;
	}

	lcd_venc_op.mute_set(pdrv, flag);
	LCDPR("[%d]: %s: %d\n", pdrv->index, __func__, flag);
}

int lcd_venc_probe(struct aml_lcd_data_s *pdata)
{
	int ret;

	if (!pdata)
		return -1;

	switch (pdata->chip_type) {
	case LCD_CHIP_T7:
	case LCD_CHIP_T3:
		ret = lcd_venc_op_init_t7(&lcd_venc_op);
		break;
	case LCD_CHIP_C3:
		ret = lcd_venc_op_init_c3(&lcd_venc_op);
		break;
	default:
		ret = lcd_venc_op_init_dft(&lcd_venc_op);
		break;
	}
	if (ret) {
		LCDERR("%s: failed\n", __func__);
		return -1;
	}

	lcd_venc_op.init_flag = 1;

	return 0;
}
