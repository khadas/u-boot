// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/io.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "lcd_reg.h"
#include "lcd_common.h"

static void lcd_vbyone_sync_pol(unsigned int offset, int hsync_pol, int vsync_pol)
{
	lcd_vcbus_setb(VBO_VIN_CTRL + offset, hsync_pol, 4, 1);
	lcd_vcbus_setb(VBO_VIN_CTRL + offset, vsync_pol, 5, 1);

	lcd_vcbus_setb(VBO_VIN_CTRL + offset, hsync_pol, 6, 1);
	lcd_vcbus_setb(VBO_VIN_CTRL + offset, vsync_pol, 7, 1);
}

static int lcd_vbyone_lanes_set(unsigned int offset, int lane_num, int byte_mode,
				int region_num, int hsize, int vsize)
{
	int sublane_num;
	int region_size[4];
	int tmp;

	switch (lane_num) {
	case 1:
	case 2:
	case 4:
	case 8:
		break;
	default:
		return -1;
	}
	switch (region_num) {
	case 1:
	case 2:
	case 4:
		break;
	default:
		return -1;
	}
	if (lane_num % region_num)
		return -1;
	switch (byte_mode) {
	case 3:
	case 4:
		break;
	default:
		return -1;
	}
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("byte_mode=%d, lane_num=%d, region_num=%d\n",
			byte_mode, lane_num, region_num);
	}

	sublane_num = lane_num / region_num; /* lane num in each region */
	lcd_vcbus_setb(VBO_LANES + offset, (lane_num - 1), 0, 3);
	lcd_vcbus_setb(VBO_LANES + offset, (region_num - 1), 4, 2);
	lcd_vcbus_setb(VBO_LANES + offset, (sublane_num - 1), 8, 3);
	lcd_vcbus_setb(VBO_LANES + offset, (byte_mode - 1), 11, 2);

	if (region_num > 1) {
		region_size[3] = (hsize / lane_num) * sublane_num;
		tmp = (hsize % lane_num);
		region_size[0] = region_size[3] + (((tmp / sublane_num) > 0) ?
			sublane_num : (tmp % sublane_num));
		region_size[1] = region_size[3] + (((tmp / sublane_num) > 1) ?
			sublane_num : (tmp % sublane_num));
		region_size[2] = region_size[3] + (((tmp / sublane_num) > 2) ?
			sublane_num : (tmp % sublane_num));
		lcd_vcbus_write(VBO_REGION_00 + offset, region_size[0]);
		lcd_vcbus_write(VBO_REGION_01 + offset, region_size[1]);
		lcd_vcbus_write(VBO_REGION_02 + offset, region_size[2]);
		lcd_vcbus_write(VBO_REGION_03 + offset, region_size[3]);
	}
	lcd_vcbus_write(VBO_ACT_VSIZE + offset, vsize);
	/* different from FBC code!!! */
	/* lcd_vcbus_setb(VBO_CTRL_H + offset,0x80,11,5); */
	/* different from simulation code!!! */
	lcd_vcbus_setb(VBO_CTRL_H + offset, 0x0, 0, 4);
	lcd_vcbus_setb(VBO_CTRL_H + offset, 0x1, 9, 1);
	/* lcd_vcbus_setb(VBO_CTRL_L + offset,enable,0,1); */

	return 0;
}

void lcd_vbyone_enable(struct aml_lcd_drv_s *pdrv)
{
	int lane_count, byte_mode, region_num, hsize, vsize;
	/* int color_fmt; */
	int vin_color, vin_bpp;
	unsigned int offset;

	offset = pdrv->data->offset_venc_if[pdrv->index];

	hsize = pdrv->config.basic.h_active;
	vsize = pdrv->config.basic.v_active;
	lane_count = pdrv->config.control.vbyone_cfg.lane_count; /* 8 */
	region_num = pdrv->config.control.vbyone_cfg.region_num; /* 2 */
	byte_mode = pdrv->config.control.vbyone_cfg.byte_mode; /* 4 */
	/* color_fmt = pdrv->config.control.vbyone_cfg.color_fmt; // 4 */

	vin_color = 4; /* fixed RGB */
	switch (pdrv->config.basic.lcd_bits) {
	case 6:
		vin_bpp = 2; /* 18bbp 4:4:4 */
		break;
	case 8:
		vin_bpp = 1; /* 24bbp 4:4:4 */
		break;
	case 10:
	default:
		vin_bpp = 0; /* 30bbp 4:4:4 */
		break;
	}

	/* set Vbyone vin color format */
	lcd_vcbus_setb(VBO_VIN_CTRL + offset, vin_color, 8, 3);
	lcd_vcbus_setb(VBO_VIN_CTRL + offset, vin_bpp, 11, 2);

	lcd_vbyone_lanes_set(offset, lane_count, byte_mode, region_num, hsize, vsize);
	/*set hsync/vsync polarity to let the polarity is low active
	inside the VbyOne */
	lcd_vbyone_sync_pol(offset, 0, 0);

	/* below line copy from simulation */
	/* gate the input when vsync asserted */
	lcd_vcbus_setb(VBO_VIN_CTRL + offset, 1, 0, 2);
	/* lcd_vcbus_write(VBO_VBK_CTRL_0 + offset,0x13);
	//lcd_vcbus_write(VBO_VBK_CTRL_1 + offset,0x56);
	//lcd_vcbus_write(VBO_HBK_CTRL + offset,0x3478);
	//lcd_vcbus_setb(VBO_PXL_CTRL + offset,0x2,0,4);
	//lcd_vcbus_setb(VBO_PXL_CTRL + offset,0x3,VBO_PXL_CTR1_BIT,VBO_PXL_CTR1_WID);
	//set_vbyone_ctlbits(1,0,0); */
	/* VBO_RGN_GEN clk always on */
	lcd_vcbus_setb(VBO_GCLK_MAIN + offset, 2, 2, 2);

	/* PAD select: */
	if ((lane_count == 1) || (lane_count == 2))
		lcd_vcbus_setb(LCD_PORT_SWAP + offset, 1, 9, 2);
	else if (lane_count == 4)
		lcd_vcbus_setb(LCD_PORT_SWAP + offset, 2, 9, 2);
	else
		lcd_vcbus_setb(LCD_PORT_SWAP + offset, 0, 9, 2);
	/* lcd_vcbus_setb(LCD_PORT_SWAP + offset, 1, 8, 1);//reverse lane output order */

	lcd_vbyone_hw_filter(pdrv, 1);
	lcd_vcbus_setb(VBO_INSGN_CTRL + offset, 0, 2, 2);

	lcd_vcbus_setb(VBO_CTRL_L + offset, 1, 0, 1);

	lcd_vbyone_wait_timing_stable(pdrv);
	lcd_vbyone_sw_reset(pdrv);

	/* training hold */
	if ((pdrv->config.control.vbyone_cfg.ctrl_flag) & 0x4)
		lcd_vbyone_cdr_training_hold(pdrv, 1);
}

void lcd_vbyone_disable(struct aml_lcd_drv_s *pdrv)
{
	unsigned int offset;

	offset = pdrv->data->offset_venc_if[pdrv->index];

	lcd_vcbus_setb(VBO_CTRL_L + offset, 0, 0, 1);
	/* clear insig setting */
	lcd_vcbus_setb(VBO_INSGN_CTRL + offset, 0, 2, 1);
	lcd_vcbus_setb(VBO_INSGN_CTRL + offset, 0, 0, 1);
}

void lcd_vbyone_sw_reset(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg_phy_tx_ctrl0, offset;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	if (pdrv->data->chip_type == LCD_CHIP_T7) {
		offset = pdrv->data->offset_venc_if[pdrv->index];
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

		/* force PHY to 0 */
		lcd_combo_dphy_setb(reg_phy_tx_ctrl0, 3, 8, 2);
		lcd_vcbus_write(VBO_SOFT_RST + offset, 0x1ff);
		udelay(5);
		/* realease PHY */
		lcd_combo_dphy_setb(reg_phy_tx_ctrl0, 0, 8, 2);
		lcd_vcbus_write(VBO_SOFT_RST + offset, 0);
	} else if (pdrv->data->chip_type == LCD_CHIP_T3) {
		offset = pdrv->data->offset_venc_if[pdrv->index];
		switch (pdrv->index) {
		case 0:
			reg_phy_tx_ctrl0 = ANACTRL_LVDS_TX_PHY_CNTL0;
			break;
		case 1:
			reg_phy_tx_ctrl0 = ANACTRL_LVDS_TX_PHY_CNTL2;
			break;
		default:
			LCDERR("[%d]: %s: invalid drv_index\n", pdrv->index, __func__);
			return;
		}

		/* force PHY to 0 */
		lcd_ana_setb(reg_phy_tx_ctrl0, 3, 8, 2);
		lcd_vcbus_write(VBO_SOFT_RST + offset, 0x1ff);
		udelay(5);
		/* realease PHY */
		lcd_ana_setb(reg_phy_tx_ctrl0, 0, 8, 2);
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

	LCDPR("[%d]: %s ...", pdrv->index, __func__);
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
	unsigned int reg_infilter_l, reg_infilter_h, reg_insig_ctrl;
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

	if (pdrv->data->chip_type == LCD_CHIP_T7 ||
	    pdrv->data->chip_type == LCD_CHIP_T3) {
		offset = pdrv->data->offset_venc_if[pdrv->index];
		reg_infilter_l = VBO_INFILTER_CTRL + offset;
		reg_infilter_h = VBO_INFILTER_CTRL_H + offset;
		reg_insig_ctrl = VBO_INSGN_CTRL + offset;
	} else {
		reg_infilter_l = VBO_INFILTER_TICK_PERIOD_L;
		reg_infilter_h = VBO_INFILTER_TICK_PERIOD_H;
		reg_insig_ctrl = VBO_INSGN_CTRL;
	}

	vx1_conf = &pdrv->config.control.vbyone_cfg;
	if (flag) {
		period = vx1_conf->hw_filter_time & 0xff;
		if (period >= (sizeof(tick_period) / sizeof(unsigned int)))
			period = tick_period[0];
		else
			period = tick_period[period];
		temp = period & 0xffff;
		lcd_vcbus_write(reg_infilter_l, temp);
		temp = (period >> 16) & 0xf;
		lcd_vcbus_write(reg_infilter_h, temp);
		/* hpd */
		temp = vx1_conf->hw_filter_cnt & 0xff;
		if (temp == 0xff) {
			lcd_vcbus_setb(reg_insig_ctrl, 0, 8, 4);
		} else {
			temp = (temp == 0) ? 0x7 : temp;
			lcd_vcbus_setb(reg_insig_ctrl, temp, 8, 4);
		}
		/* lockn */
		temp = (vx1_conf->hw_filter_cnt >> 8) & 0xff;
		if (temp == 0xff) {
			lcd_vcbus_setb(reg_insig_ctrl, 0, 12, 4);
		} else {
			temp = (temp == 0) ? 0x7 : temp;
			lcd_vcbus_setb(reg_insig_ctrl, temp, 12, 4);
		}
	} else {
		temp = (vx1_conf->hw_filter_time >> 8) & 0x1;
		if (temp) {
			lcd_vcbus_write(reg_infilter_l, 0xff);
			lcd_vcbus_write(reg_infilter_h, 0x0);
			lcd_vcbus_setb(reg_insig_ctrl, 0, 8, 4);
			lcd_vcbus_setb(reg_insig_ctrl, 0, 12, 4);
			LCDPR("[%d]: %s: %d disable for debug\n",
			      pdrv->index, __func__, flag);
		}
	}
}
