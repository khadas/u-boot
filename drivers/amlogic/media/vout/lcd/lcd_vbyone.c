/*
 * drivers/amlogic/media/vout/lcd/lcd_venc.c
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
#include <malloc.h>
#include <asm/arch/io.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "lcd_reg.h"
#include "lcd_common.h"

void lcd_vbyone_sw_reset(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg_phy_tx_ctrl0, offset;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	switch (pdrv->index) {
	case 0:
		reg_phy_tx_ctrl0 = COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0;
		break;
	case 1:
		reg_phy_tx_ctrl0 = COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL0;
		break;
	default:
		LCDERR("[%d]: %s: invalid drv_index\n", pdrv->index, __func__);
		return;
	}
	offset = pdrv->data->offset_venc_if[pdrv->index];

	if (pdrv->data->chip_type == LCD_CHIP_T7) {
		/* force PHY to 0 */
		lcd_combo_dphy_setb(reg_phy_tx_ctrl0, 3, 8, 2);
		lcd_vcbus_write(VBO_SOFT_RST + offset, 0x1ff);
		udelay(5);
		/* realease PHY */
		lcd_combo_dphy_setb(reg_phy_tx_ctrl0, 0, 8, 2);
		lcd_vcbus_write(VBO_SOFT_RST + offset, 0);
	} else {
		/* force PHY to 0 */
		lcd_ana_setb(HHI_LVDS_TX_PHY_CNTL0, 3, 8, 2);
		lcd_vcbus_write(VBO_SOFT_RST, 0x1ff);
		udelay(5);
		/* realease PHY */
		lcd_ana_setb(HHI_LVDS_TX_PHY_CNTL0, 0, 8, 2);
		lcd_vcbus_write(VBO_SOFT_RST, 0);
	}
}

void lcd_vbyone_wait_timing_stable(struct aml_lcd_drv_s *pdrv)
{
	unsigned int offset, timing_state;
	int i = 200;

	offset = pdrv->data->offset_venc[pdrv->index];

	timing_state = lcd_vcbus_read(VBO_INTR_STATE + offset) & 0x1ff;
	while ((timing_state) && (i > 0)) {
		/* clear video timing error intr */
		lcd_vcbus_setb(VBO_INTR_STATE_CTRL + offset, 0x7, 0, 3);
		lcd_vcbus_setb(VBO_INTR_STATE_CTRL + offset, 0, 0, 3);
		mdelay(2);
		timing_state = lcd_vcbus_read(VBO_INTR_STATE + offset) & 0x1ff;
		i--;
	};
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("[%d]: vbyone timing state: 0x%03x, i=%d\n",
		      pdrv->index, timing_state, (200 - i));
	}
	mdelay(2);
}

void lcd_vbyone_cdr_training_hold(struct aml_lcd_drv_s *pdrv, int flag)
{
	unsigned int offset, reg;

	offset = pdrv->data->offset_venc[pdrv->index];
	reg = VBO_FSM_HOLDER_H + offset;
	if (flag) {
		LCDPR("[%d]: ctrl_flag for cdr_training_hold\n", pdrv->index);
		lcd_vcbus_setb(reg, 0xffff, 0, 16);
	} else {
		mdelay(pdrv->config.control.vbyone_cfg.cdr_training_hold);
		lcd_vcbus_setb(reg, 0, 0, 16);
	}
}

#define VX1_HPD_WAIT_TIMEOUT    10000 /* 500ms */
void lcd_vbyone_wait_hpd(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg, offset, val;
	int i = 0;

	offset = pdrv->data->offset_venc_if[pdrv->index];
	reg = VBO_STATUS_L + offset;

	LCDPR("[%d]: %s ...\n", pdrv->index, __func__);
	while (i++ < VX1_HPD_WAIT_TIMEOUT) {
		if (lcd_vcbus_getb(reg, 6, 1) == 0)
			break;
		udelay(100);
	}

	val = lcd_vcbus_getb(reg, 6, 1);
	if (val) {
		printf("hpd=%d\n", val);
	} else {
		printf("hpd=%d, i=%d\n", val, i);
		/* force low only activated for actual hpd is low */
		lcd_vcbus_setb(VBO_INSGN_CTRL + offset, 1, 2, 2);
	}

	if (pdrv->config.control.vbyone_cfg.ctrl_flag & 0x2) {
		LCDPR("[%d]: ctrl_flag for hpd_data delay\n", pdrv->index);
		mdelay(pdrv->config.control.vbyone_cfg.hpd_data_delay);
	} else {
		mdelay(10);; /* add 10ms delay for compatibility */
	}
}

#define VX1_LOCKN_WAIT_TIMEOUT    500 /* 500ms */
void lcd_vbyone_wait_stable(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg, offset;
	int i = 0;

	offset = pdrv->data->offset_venc_if[pdrv->index];
	reg = VBO_STATUS_L + offset;

	/* training hold release */
	if (pdrv->config.control.vbyone_cfg.ctrl_flag & 0x4)
		lcd_vbyone_cdr_training_hold(pdrv, 0);

	while (i++ < VX1_LOCKN_WAIT_TIMEOUT) {
		if ((lcd_vcbus_read(reg) & 0x3f) == 0x20)
			break;
		mdelay(1);
	}
	LCDPR("[%d]: %s status: 0x%x, i=%d\n",
	      pdrv->index, __func__, lcd_vcbus_read(reg), i);

	/* power on reset */
	if (pdrv->config.control.vbyone_cfg.ctrl_flag & 0x1) {
		LCDPR("[%d]: ctrl_flag for power on reset\n", pdrv->index);
		mdelay(pdrv->config.control.vbyone_cfg.power_on_reset_delay);
		lcd_vbyone_sw_reset(pdrv);
	}
}

void lcd_vbyone_hw_filter(struct aml_lcd_drv_s *pdrv, int flag)
{
	struct vbyone_config_s *vx1_conf;
	unsigned int temp, period, offset;
	unsigned int tick_period[] = {
		0xfff,
		0xff,    /* 1: 0.8us */
		0x1ff,   /* 2: 1.7us */
		0x3ff,   /* 3: 3.4us */
		0x7ff,   /* 4: 6.9us */
		0xfff,   /* 5: 13.8us */
		0x1fff,  /* 6: 27us */
		0x3fff,  /* 7: 55us */
		0x7fff,  /* 8: 110us */
		0xffff,  /* 9: 221us */
		0x1ffff, /* 10: 441us */
		0x3ffff, /* 11: 883us */
		0x7ffff, /* 12: 1.76ms */
		0xfffff, /* 13: 3.53ms */
	};

	offset = pdrv->data->offset_venc_if[pdrv->index];

	vx1_conf = &pdrv->config.control.vbyone_cfg;
	if (flag) {
		period = vx1_conf->hw_filter_time & 0xff;
		if (period >= (sizeof(tick_period) / sizeof(unsigned int)))
			period = tick_period[0];
		else
			period = tick_period[period];
		temp = period & 0xffff;
		lcd_vcbus_write(VBO_INFILTER_TICK_PERIOD_L + offset, temp);
		temp = (period >> 16) & 0xf;
		lcd_vcbus_write(VBO_INFILTER_TICK_PERIOD_H + offset, temp);
		/* hpd */
		temp = vx1_conf->hw_filter_cnt & 0xff;
		if (temp == 0xff) {
			lcd_vcbus_setb(VBO_INSGN_CTRL + offset, 0, 8, 4);
		} else {
			temp = (temp == 0) ? 0x7 : temp;
			lcd_vcbus_setb(VBO_INSGN_CTRL + offset, temp, 8, 4);
		}
		/* lockn */
		temp = (vx1_conf->hw_filter_cnt >> 8) & 0xff;
		if (temp == 0xff) {
			lcd_vcbus_setb(VBO_INSGN_CTRL + offset, 0, 12, 4);
		} else {
			temp = (temp == 0) ? 0x7 : temp;
			lcd_vcbus_setb(VBO_INSGN_CTRL + offset, temp, 12, 4);
		}
	} else {
		temp = (vx1_conf->hw_filter_time >> 8) & 0x1;
		if (temp) {
			lcd_vcbus_write(VBO_INFILTER_TICK_PERIOD_L + offset, 0xff);
			lcd_vcbus_write(VBO_INFILTER_TICK_PERIOD_H + offset, 0x0);
			lcd_vcbus_setb(VBO_INSGN_CTRL + offset, 0, 8, 4);
			lcd_vcbus_setb(VBO_INSGN_CTRL + offset, 0, 12, 4);
			LCDPR("[%d]: %s: %d disable for debug\n",
			      pdrv->index, __func__, flag);
		}
	}
}
