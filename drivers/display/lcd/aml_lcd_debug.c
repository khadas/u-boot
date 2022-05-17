/*
 * drivers/display/lcd/aml_lcd_debug.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the named License,
 * or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <common.h>
#include <malloc.h>
#include <amlogic/aml_lcd.h>
#include "aml_lcd_reg.h"
#include "aml_lcd_common.h"
#ifdef CONFIG_AML_LCD_TABLET
#include "lcd_tablet/mipi_dsi_util.h"
#endif
#include "aml_lcd_debug.h"

static struct lcd_debug_info_reg_s *lcd_debug_info_reg;
static struct lcd_debug_info_if_s *lcd_debug_info_if;

static void lcd_timing_info_print(struct lcd_config_s * pconf)
{
	unsigned int hs_width, hs_bp, hs_pol, h_period;
	unsigned int vs_width, vs_bp, vs_pol, v_period;
	unsigned int video_on_pixel, video_on_line;

	video_on_pixel = pconf->lcd_timing.video_on_pixel;
	video_on_line = pconf->lcd_timing.video_on_line;
	h_period = pconf->lcd_basic.h_period;
	v_period = pconf->lcd_basic.v_period;

	hs_width = pconf->lcd_timing.hsync_width;
	hs_bp = pconf->lcd_timing.hsync_bp;
	hs_pol = pconf->lcd_timing.hsync_pol;
	vs_width = pconf->lcd_timing.vsync_width;
	vs_bp = pconf->lcd_timing.vsync_bp;
	vs_pol = pconf->lcd_timing.vsync_pol;

	printf("h_period          %d\n"
		"v_period          %d\n"
		"hs_width          %d\n"
		"hs_backporch      %d\n"
		"hs_pol            %d\n"
		"vs_width          %d\n"
		"vs_backporch      %d\n"
		"vs_pol            %d\n"
		"video_on_pixel    %d\n"
		"video_on_line     %d\n\n",
		h_period, v_period, hs_width, hs_bp, hs_pol,
		vs_width, vs_bp, vs_pol, video_on_pixel, video_on_line);

	printf("h_period_min      %d\n"
		"h_period_max      %d\n"
		"v_period_min      %d\n"
		"v_period_max      %d\n"
		"pclk_min          %d\n"
		"pclk_max          %d\n\n",
		pconf->lcd_basic.h_period_min, pconf->lcd_basic.h_period_max,
		pconf->lcd_basic.v_period_min, pconf->lcd_basic.v_period_max,
		pconf->lcd_basic.lcd_clk_min, pconf->lcd_basic.lcd_clk_max);
}

static void lcd_power_info_print(struct aml_lcd_drv_s *lcd_drv, int status)
{
	int i;
	struct lcd_power_step_s *power_step;

	if (status) {
		/* check if factory test */
		if (lcd_drv->factory_lcd_power_on_step) {
			printf("factory test power on step:\n");
			power_step = lcd_drv->factory_lcd_power_on_step;
		} else {
			printf("power on step:\n");
			power_step = &lcd_drv->lcd_config->lcd_power->power_on_step[0];
		}
	} else {
		printf("power off step:\n");
		power_step = &lcd_drv->lcd_config->lcd_power->power_off_step[0];
	}

	i = 0;
	while (i < LCD_PWR_STEP_MAX) {
		if (power_step->type >= LCD_POWER_TYPE_MAX)
			break;
		switch (power_step->type) {
		case LCD_POWER_TYPE_CPU:
		case LCD_POWER_TYPE_PMU:
		case LCD_POWER_TYPE_WAIT_GPIO:
		case LCD_POWER_TYPE_CLK_SS:
			printf("%d: type=%d, index=%d, value=%d, delay=%d\n",
				i, power_step->type, power_step->index,
				power_step->value, power_step->delay);
			break;
		case LCD_POWER_TYPE_EXTERN:
			printf("%d: type=%d, index=%d, delay=%d\n",
				i, power_step->type, power_step->index,
				power_step->delay);
			break;
		case LCD_POWER_TYPE_SIGNAL:
			printf("%d: type=%d, delay=%d\n",
				i, power_step->type, power_step->delay);
			break;
		default:
			break;
		}
		i++;
		power_step++;
	}
}

static void lcd_pinmux_info_print(struct lcd_config_s *pconf)
{
	int i;

	printf("pinmux:\n");

	i = 0;
	while (i < LCD_PINMUX_NUM) {
		if (pconf->pinmux_set[i][0] == LCD_PINMUX_END)
			break;
		printf("pinmux_set: %d, 0x%08x\n",
			pconf->pinmux_set[i][0], pconf->pinmux_set[i][1]);
		i++;
	}
	i = 0;
	while (i < LCD_PINMUX_NUM) {
		if (pconf->pinmux_clr[i][0] == LCD_PINMUX_END)
			break;
		printf("pinmux_clr: %d, 0x%08x\n",
			pconf->pinmux_clr[i][0], pconf->pinmux_clr[i][1]);
		i++;
	}

	printf("\n");
}

static void lcd_info_print_lvds(struct lcd_config_s *pconf)
{
	printf("lvds_repack       %u\n"
		"dual_port         %u\n"
		"pn_swap           %u\n"
		"port_swap         %u\n"
		"lane_reverse      %u\n"
		"phy_vswing        0x%x\n"
		"phy_preem         0x%x\n"
		"phy_clk_vswing    0x%x\n"
		"phy_clk_preem     0x%x\n\n",
		pconf->lcd_control.lvds_config->lvds_repack,
		pconf->lcd_control.lvds_config->dual_port,
		pconf->lcd_control.lvds_config->pn_swap,
		pconf->lcd_control.lvds_config->port_swap,
		pconf->lcd_control.lvds_config->lane_reverse,
		pconf->lcd_control.lvds_config->phy_vswing,
		pconf->lcd_control.lvds_config->phy_preem,
		pconf->lcd_control.lvds_config->phy_clk_vswing,
		pconf->lcd_control.lvds_config->phy_clk_preem);
}

static void lcd_info_print_vbyone(struct lcd_config_s *pconf)
{
	printf("lane_count                 %u\n"
		"region_num                 %u\n"
		"byte_mode                  %u\n"
		"bit_rate                   %uHz\n"
		"phy_vswing                 0x%x\n"
		"phy_preemphasis            0x%x\n"
		"hw_filter_time             0x%x\n"
		"hw_filter_cnt              0x%x\n"
		"ctrl_flag                  0x%x\n\n",
		pconf->lcd_control.vbyone_config->lane_count,
		pconf->lcd_control.vbyone_config->region_num,
		pconf->lcd_control.vbyone_config->byte_mode,
		pconf->lcd_timing.bit_rate,
		pconf->lcd_control.vbyone_config->phy_vswing,
		pconf->lcd_control.vbyone_config->phy_preem,
		pconf->lcd_control.vbyone_config->hw_filter_time,
		pconf->lcd_control.vbyone_config->hw_filter_cnt,
		pconf->lcd_control.vbyone_config->ctrl_flag);
	if (pconf->lcd_control.vbyone_config->ctrl_flag & 0x1) {
		printf("power_on_reset_en          %u\n"
			"power_on_reset_delay       %ums\n\n",
			(pconf->lcd_control.vbyone_config->ctrl_flag & 0x1),
			pconf->lcd_control.vbyone_config->power_on_reset_delay);
	}
	if (pconf->lcd_control.vbyone_config->ctrl_flag & 0x2) {
		printf("hpd_data_delay_en          %u\n"
			"hpd_data_delay             %ums\n\n",
			((pconf->lcd_control.vbyone_config->ctrl_flag >> 1) & 0x1),
			pconf->lcd_control.vbyone_config->hpd_data_delay);
	}
	if (pconf->lcd_control.vbyone_config->ctrl_flag & 0x4) {
		printf("cdr_training_hold_en       %u\n"
			"cdr_training_hold          %ums\n\n",
			((pconf->lcd_control.vbyone_config->ctrl_flag >> 2) & 0x1),
			pconf->lcd_control.vbyone_config->cdr_training_hold);
	}
	if (pconf->lcd_control.vbyone_config->vx1_sw_filter_en) {
		printf("vx1_sw_filter_en           %u\n"
			"vx1_sw_filter_time         %u\n"
			"vx1_sw_filter_cnt          %u\n"
			"vx1_sw_filter_retry_cnt    %u\n"
			"vx1_sw_filter_retry_delay  %u\n"
			"vx1_sw_cdr_detect_time     %u\n"
			"vx1_sw_cdr_detect_cnt      %u\n"
			"vx1_sw_cdr_timeout_cnt     %u\n\n",
			pconf->lcd_control.vbyone_config->vx1_sw_filter_en,
			pconf->lcd_control.vbyone_config->vx1_sw_filter_time,
			pconf->lcd_control.vbyone_config->vx1_sw_filter_cnt,
			pconf->lcd_control.vbyone_config->vx1_sw_filter_retry_cnt,
			pconf->lcd_control.vbyone_config->vx1_sw_filter_retry_delay,
			pconf->lcd_control.vbyone_config->vx1_sw_cdr_detect_time,
			pconf->lcd_control.vbyone_config->vx1_sw_cdr_detect_cnt,
			pconf->lcd_control.vbyone_config->vx1_sw_cdr_timeout_cnt);
	}
	lcd_pinmux_info_print(pconf);
}

static void lcd_info_print_ttl(struct lcd_config_s *pconf)
{
	printf("clk_pol           %u\n"
		"DE_valid          %u\n"
		"hvsync_valid      %u\n"
		"rb_swap           %u\n"
		"bit_swap          %u\n\n",
		pconf->lcd_control.ttl_config->clk_pol,
		(pconf->lcd_control.ttl_config->sync_valid >> 1) & 1,
		(pconf->lcd_control.ttl_config->sync_valid >> 0) & 1,
		(pconf->lcd_control.ttl_config->swap_ctrl >> 1) & 1,
		(pconf->lcd_control.ttl_config->swap_ctrl >> 0) & 1);
	lcd_pinmux_info_print(pconf);
}

static void lcd_info_print_mipi(struct lcd_config_s *pconf)
{
#ifdef CONFIG_AML_LCD_TABLET
	mipi_dsi_print_info(pconf);
#endif
}

#ifdef CONFIG_AML_LCD_TCON
static void lcd_info_print_mlvds(struct lcd_config_s *pconf)
{
	printf("channel_num       %d\n"
		"channel_sel0      0x%08x\n"
		"channel_sel1      0x%08x\n"
		"clk_phase         0x%04x\n"
		"pn_swap           %u\n"
		"bit_swap          %u\n"
		"phy_vswing        0x%x\n"
		"phy_preem         0x%x\n"
		"bit_rate          %uHz\n"
		"pi_clk_sel        0x%03x\n\n",
		pconf->lcd_control.mlvds_config->channel_num,
		pconf->lcd_control.mlvds_config->channel_sel0,
		pconf->lcd_control.mlvds_config->channel_sel1,
		pconf->lcd_control.mlvds_config->clk_phase,
		pconf->lcd_control.mlvds_config->pn_swap,
		pconf->lcd_control.mlvds_config->bit_swap,
		pconf->lcd_control.mlvds_config->phy_vswing,
		pconf->lcd_control.mlvds_config->phy_preem,
		pconf->lcd_timing.bit_rate,
		pconf->lcd_control.mlvds_config->pi_clk_sel);
	lcd_tcon_info_print();
	lcd_pinmux_info_print(pconf);
}

static void lcd_info_print_p2p(struct lcd_config_s *pconf)
{
	printf("p2p_type          0x%x\n"
		"lane_num          %d\n"
		"channel_sel0      0x%08x\n"
		"channel_sel1      0x%08x\n"
		"pn_swap           %u\n"
		"bit_swap          %u\n"
		"bit_rate          %uHz\n"
		"phy_vswing        0x%x\n"
		"phy_preem         0x%x\n\n",
		pconf->lcd_control.p2p_config->p2p_type,
		pconf->lcd_control.p2p_config->lane_num,
		pconf->lcd_control.p2p_config->channel_sel0,
		pconf->lcd_control.p2p_config->channel_sel1,
		pconf->lcd_control.p2p_config->pn_swap,
		pconf->lcd_control.p2p_config->bit_swap,
		pconf->lcd_timing.bit_rate,
		pconf->lcd_control.p2p_config->phy_vswing,
		pconf->lcd_control.p2p_config->phy_preem);
	lcd_tcon_info_print();
	lcd_pinmux_info_print(pconf);
}
#endif

static void lcd_phy_print(struct lcd_config_s *pconf)
{
	struct phy_config_s *phy = pconf->lcd_control.phy_cfg;
	int i;

	printf("vswing             %u\n"
		"ext_pullup        %u\n"
		"vcm               %u\n"
		"mode              %u\n",
		phy->vswing,
		phy->ext_pullup,
		phy->vcm,
		phy->mode);
	for (i = 0; i < phy->lane_num; i++) {
		printf("lane%d_amp =       %u\n"
			"lane%d_preem =     %u\n",
			i, phy->lane[i].amp,
			i, phy->lane[i].preem);
	}
	printf("\n");
}

static void lcd_reg_print_serializer(void)
{
	unsigned int reg;

	reg = HHI_LVDS_TX_PHY_CNTL0;
	printf("HHI_LVDS_TX_PHY_CNTL0     [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_LVDS_TX_PHY_CNTL1;
	printf("HHI_LVDS_TX_PHY_CNTL1     [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
}

static void lcd_reg_print_ttl(void)
{
	unsigned int reg;

	printf("\nttl registers:\n");
	reg = L_DUAL_PORT_CNTL_ADDR;
	printf("PORT_CNTL           [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = L_STH1_HS_ADDR;
	printf("STH1_HS_ADDR        [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = L_STH1_HE_ADDR;
	printf("STH1_HE_ADDR        [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = L_STH1_VS_ADDR;
	printf("STH1_VS_ADDR        [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = L_STH1_VE_ADDR;
	printf("STH1_VE_ADDR        [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = L_STV1_HS_ADDR;
	printf("STV1_HS_ADDR        [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = L_STV1_HE_ADDR;
	printf("STV1_HE_ADDR        [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = L_STV1_VS_ADDR;
	printf("STV1_VS_ADDR        [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = L_STV1_VE_ADDR;
	printf("STV1_VE_ADDR        [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = L_OEH_HS_ADDR;
	printf("OEH_HS_ADDR         [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = L_OEH_HE_ADDR;
	printf("OEH_HE_ADDR         [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = L_OEH_VS_ADDR;
	printf("OEH_VS_ADDR         [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = L_OEH_VE_ADDR;
	printf("OEH_VE_ADDR         [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
}

static void lcd_reg_print_lvds(void)
{
	unsigned int reg;

	printf("\nlvds registers:\n");
	reg = LVDS_PACK_CNTL_ADDR;
	printf("LVDS_PACK_CNTL      [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = LVDS_GEN_CNTL;
	printf("LVDS_GEN_CNTL       [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = LCD_PORT_SWAP;
	printf("LCD_PORT_SWAP       [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
}

static void lcd_reg_print_lvds_txhd(void)
{
	unsigned int reg;

	printf("\nlvds registers:\n");
	reg = LVDS_PACK_CNTL_ADDR;
	printf("LVDS_PACK_CNTL      [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = LVDS_GEN_CNTL;
	printf("LVDS_GEN_CNTL       [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));

	reg = LVDS_CH_SWAP0;
	printf("LVDS_CH_SWAP0       [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = LVDS_CH_SWAP1;
	printf("LVDS_CH_SWAP1       [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = LVDS_CH_SWAP2;
	printf("LVDS_CH_SWAP2       [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
}

static void lcd_reg_print_lvds_tl1(void)
{
	unsigned int reg;

	printf("\nlvds registers:\n");
	reg = LVDS_PACK_CNTL_ADDR;
	printf("LVDS_PACK_CNTL      [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = LVDS_GEN_CNTL;
	printf("LVDS_GEN_CNTL       [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));

	reg = P2P_CH_SWAP0;
	printf("P2P_CH_SWAP0        [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = P2P_CH_SWAP1;
	printf("P2P_CH_SWAP1        [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
}

static void lcd_reg_print_vbyone(void)
{
	unsigned int reg;

	printf("\nvbyone registers:\n");
	reg = VBO_STATUS_L;
	printf("VX1_STATUS          [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VBO_FSM_HOLDER_L;
	printf("VX1_FSM_HOLDER_L    [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VBO_FSM_HOLDER_H;
	printf("VX1_FSM_HOLDER_H    [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VBO_INTR_STATE_CTRL;
	printf("VX1_INTR_STATE_CTRL [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VBO_INTR_UNMASK;
	printf("VX1_INTR_UNMASK     [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VBO_INTR_STATE;
	printf("VX1_INTR_STATE      [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
}

static void lcd_reg_print_vbyone_txl(void)
{
	unsigned int reg;

	printf("\nvbyone registers:\n");
	reg = VBO_STATUS_L;
	printf("VX1_STATUS          [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VBO_FSM_HOLDER_L;
	printf("VX1_FSM_HOLDER_L    [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VBO_FSM_HOLDER_H;
	printf("VX1_FSM_HOLDER_H    [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VBO_INTR_STATE_CTRL;
	printf("VX1_INTR_STATE_CTRL [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VBO_INTR_UNMASK;
	printf("VX1_INTR_UNMASK     [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VBO_INTR_STATE;
	printf("VX1_INTR_STATE      [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VBO_INSGN_CTRL;
	printf("VBO_INSGN_CTRL      [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
}

#ifdef CONFIG_AML_LCD_TCON
static void lcd_reg_print_tcon(void)
{
	unsigned int reg;

	printf("\ntcon clk registers:\n");
	reg = HHI_TCON_CLK_CNTL;
	printf("HHI_TCON_CLK_CNTL   [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_HPLL_CNTL6;
	printf("HHI_HPLL_CNTL6      [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_TCON_CNTL0;
	printf("HHI_DIF_TCON_CNTL0  [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_TCON_CNTL1;
	printf("HHI_DIF_TCON_CNTL1  [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_TCON_CNTL2;
	printf("HHI_DIF_TCON_CNTL2  [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));

	printf("\ntcon top registers:\n");
	reg = TCON_TOP_CTRL;
	printf("TCON_TOP_CTRL       [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_OUT_CH_SEL0;
	printf("TCON_OUT_CH_SEL0    [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_OUT_CH_SEL1;
	printf("TCON_OUT_CH_SEL1    [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_STATUS0;
	printf("TCON_STATUS0        [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_PLLLOCK_CNTL;
	printf("TCON_PLLLOCK_CNTL   [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_RST_CTRL;
	printf("TCON_RST_CTRL       [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_CLK_CTRL;
	printf("TCON_CLK_CTRL       [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_STATUS1;
	printf("TCON_STATUS1        [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));

	reg = LVDS_CH_SWAP0;
	printf("LVDS_CH_SWAP0       [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = LVDS_CH_SWAP1;
	printf("LVDS_CH_SWAP1       [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = LVDS_CH_SWAP2;
	printf("LVDS_CH_SWAP2       [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
}

static void lcd_reg_print_tcon_tl1(void)
{
	unsigned int reg;

	printf("\ntcon registers:\n");
	reg = HHI_TCON_CLK_CNTL;
	printf("HHI_TCON_CLK_CNTL   [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = TCON_TOP_CTRL;
	printf("TCON_TOP_CTRL       [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_RGB_IN_MUX;
	printf("TCON_RGB_IN_MUX     [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_OUT_CH_SEL0;
	printf("TCON_OUT_CH_SEL0    [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_OUT_CH_SEL1;
	printf("TCON_OUT_CH_SEL1    [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_STATUS0;
	printf("TCON_STATUS0        [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_PLLLOCK_CNTL;
	printf("TCON_PLLLOCK_CNTL   [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_RST_CTRL;
	printf("TCON_RST_CTRL       [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_AXI_OFST0;
	printf("TCON_AXI_OFST0      [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_AXI_OFST1;
	printf("TCON_AXI_OFST1      [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_AXI_OFST2;
	printf("TCON_AXI_OFST2      [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_CLK_CTRL;
	printf("TCON_CLK_CTRL       [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_STATUS1;
	printf("TCON_STATUS1        [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_DDRIF_CTRL1;
	printf("TCON_DDRIF_CTRL1    [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));
	reg = TCON_DDRIF_CTRL2;
	printf("TCON_DDRIF_CTRL2    [0x%04x] = 0x%08x\n",
		reg, lcd_tcon_read(reg));

	reg = P2P_CH_SWAP0;
	printf("P2P_CH_SWAP0        [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = P2P_CH_SWAP1;
	printf("P2P_CH_SWAP1        [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
}
#endif

static void lcd_reg_print_mipi(void)
{
	unsigned int reg;

	printf("\nmipi_dsi registers:\n");
	reg = MIPI_DSI_TOP_CNTL;
	printf("MIPI_DSI_TOP_CNTL            [0x%04x] = 0x%08x\n",
		reg, dsi_host_read(reg));
	reg = MIPI_DSI_TOP_CLK_CNTL;
	printf("MIPI_DSI_TOP_CLK_CNTL        [0x%04x] = 0x%08x\n",
		reg, dsi_host_read(reg));
	reg = MIPI_DSI_DWC_PWR_UP_OS;
	printf("MIPI_DSI_DWC_PWR_UP_OS       [0x%04x] = 0x%08x\n",
		reg, dsi_host_read(reg));
	reg = MIPI_DSI_DWC_PCKHDL_CFG_OS;
	printf("MIPI_DSI_DWC_PCKHDL_CFG_OS   [0x%04x] = 0x%08x\n",
		reg, dsi_host_read(reg));
	reg = MIPI_DSI_DWC_LPCLK_CTRL_OS;
	printf("MIPI_DSI_DWC_LPCLK_CTRL_OS   [0x%04x] = 0x%08x\n",
		reg, dsi_host_read(reg));
	reg = MIPI_DSI_DWC_CMD_MODE_CFG_OS;
	printf("MIPI_DSI_DWC_CMD_MODE_CFG_OS [0x%04x] = 0x%08x\n",
		reg, dsi_host_read(reg));
	reg = MIPI_DSI_DWC_VID_MODE_CFG_OS;
	printf("MIPI_DSI_DWC_VID_MODE_CFG_OS [0x%04x] = 0x%08x\n",
		reg, dsi_host_read(reg));
	reg = MIPI_DSI_DWC_MODE_CFG_OS;
	printf("MIPI_DSI_DWC_MODE_CFG_OS     [0x%04x] = 0x%08x\n",
		reg, dsi_host_read(reg));
	reg = MIPI_DSI_DWC_PHY_STATUS_OS;
	printf("MIPI_DSI_DWC_PHY_STATUS_OS   [0x%04x] = 0x%08x\n",
		reg, dsi_host_read(reg));
	reg = MIPI_DSI_DWC_INT_ST0_OS;
	printf("MIPI_DSI_DWC_INT_ST0_OS      [0x%04x] = 0x%08x\n",
		reg, dsi_host_read(reg));
	reg = MIPI_DSI_DWC_INT_ST1_OS;
	printf("MIPI_DSI_DWC_INT_ST1_OS      [0x%04x] = 0x%08x\n",
		reg, dsi_host_read(reg));
	reg = MIPI_DSI_TOP_STAT;
	printf("MIPI_DSI_TOP_STAT            [0x%04x] = 0x%08x\n",
		reg, dsi_host_read(reg));
	reg = MIPI_DSI_TOP_INTR_CNTL_STAT;
	printf("MIPI_DSI_TOP_INTR_CNTL_STAT  [0x%04x] = 0x%08x\n",
		reg, dsi_host_read(reg));
	reg = MIPI_DSI_TOP_MEM_PD;
	printf("MIPI_DSI_TOP_MEM_PD          [0x%04x] = 0x%08x\n",
		reg, dsi_host_read(reg));
}

static void lcd_reg_print_phy_analog(void)
{
	unsigned int reg;

	printf("\nphy analog registers:\n");
	lcd_reg_print_serializer();

	reg = HHI_DIF_CSI_PHY_CNTL1;
	printf("PHY_CNTL1           [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL2;
	printf("PHY_CNTL2           [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL3;
	printf("PHY_CNTL3           [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
}

static void lcd_reg_print_phy_analog_tl1(void)
{
	unsigned int reg;

	printf("\nphy analog registers:\n");
	lcd_reg_print_serializer();

	reg = HHI_DIF_CSI_PHY_CNTL1;
	printf("PHY_CNTL1           [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL2;
	printf("PHY_CNTL2           [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL3;
	printf("PHY_CNTL3           [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL4;
	printf("PHY_CNTL4           [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL6;
	printf("PHY_CNTL6           [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL7;
	printf("PHY_CNTL7           [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL8;
	printf("PHY_CNTL8           [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL9;
	printf("PHY_CNTL9           [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL10;
	printf("PHY_CNTL10          [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL11;
	printf("PHY_CNTL11          [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL12;
	printf("PHY_CNTL12          [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL13;
	printf("PHY_CNTL13          [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL14;
	printf("PHY_CNTL14          [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL15;
	printf("PHY_CNTL15          [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL16;
	printf("PHY_CNTL16          [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
}

static void lcd_reg_print_mipi_phy_analog(void)
{
	unsigned int reg;

	printf("\nphy analog registers:\n");
	reg = HHI_MIPI_CNTL0;
	printf("PHY_CNTL1   [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_MIPI_CNTL1;
	printf("PHY_CNTL2   [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
	reg = HHI_MIPI_CNTL2;
	printf("PHY_CNTL3   [0x%08x] = 0x%08x\n",
		reg, lcd_hiu_read(reg));
}

#define TV_LCD_ENC_TST_NUM_MAX    9
static char *lcd_enc_tst_str[] = {
	"0-None",        /* 0 */
	"1-Color Bar",   /* 1 */
	"2-Thin Line",   /* 2 */
	"3-Dot Grid",    /* 3 */
	"4-Gray",        /* 4 */
	"5-Red",         /* 5 */
	"6-Green",       /* 6 */
	"7-Blue",        /* 7 */
	"8-Black",       /* 8 */
};

static unsigned int lcd_enc_tst[][7] = {
/*tst_mode,    Y,       Cb,     Cr,     tst_en,  vfifo_en  rgbin*/
	{0,    0x200,   0x200,  0x200,   0,      1,        3},  /* 0 */
	{1,    0x200,   0x200,  0x200,   1,      0,        1},  /* 1 */
	{2,    0x200,   0x200,  0x200,   1,      0,        1},  /* 2 */
	{3,    0x200,   0x200,  0x200,   1,      0,        1},  /* 3 */
	{0,    0x1ff,   0x1ff,  0x1ff,   1,      0,        3},  /* 4 */
	{0,    0x3ff,     0x0,    0x0,   1,      0,        3},  /* 5 */
	{0,      0x0,   0x3ff,    0x0,   1,      0,        3},  /* 6 */
	{0,      0x0,     0x0,  0x3ff,   1,      0,        3},  /* 7 */
	{0,      0x0,     0x0,    0x0,   1,      0,        3},  /* 8 */
};

/* **********************************
 * lcd debug function api
 * **********************************
 */
void aml_lcd_debug_test(unsigned int num)
{
	struct aml_lcd_drv_s *lcd_drv = aml_lcd_get_driver();
	unsigned int start, width;

	start = lcd_drv->lcd_config->lcd_timing.video_on_pixel;
	width = lcd_drv->lcd_config->lcd_basic.h_active / 9;
	num = (num >= TV_LCD_ENC_TST_NUM_MAX) ? 0 : num;

	lcd_wait_vsync();
	lcd_vcbus_write(ENCL_VIDEO_RGBIN_CTRL, lcd_enc_tst[num][6]);
	lcd_vcbus_write(ENCL_TST_MDSEL, lcd_enc_tst[num][0]);
	lcd_vcbus_write(ENCL_TST_Y, lcd_enc_tst[num][1]);
	lcd_vcbus_write(ENCL_TST_CB, lcd_enc_tst[num][2]);
	lcd_vcbus_write(ENCL_TST_CR, lcd_enc_tst[num][3]);
	lcd_vcbus_write(ENCL_TST_CLRBAR_STRT, start);
	lcd_vcbus_write(ENCL_TST_CLRBAR_WIDTH, width);
	lcd_vcbus_write(ENCL_TST_EN, lcd_enc_tst[num][4]);
	lcd_vcbus_setb(ENCL_VIDEO_MODE_ADV, lcd_enc_tst[num][5], 3, 1);
	if (num > 0) {
		LCDPR("show test pattern: %s\n", lcd_enc_tst_str[num]);
	} else {
		LCDPR("disable test pattern\n");
	}
}

void aml_lcd_mute_setting(unsigned char flag)
{
	lcd_wait_vsync();
	if (flag) {
		lcd_vcbus_write(ENCL_VIDEO_RGBIN_CTRL, 3);
		lcd_vcbus_write(ENCL_TST_MDSEL, 0);
		lcd_vcbus_write(ENCL_TST_Y, 0);
		lcd_vcbus_write(ENCL_TST_CB, 0);
		lcd_vcbus_write(ENCL_TST_CR, 0);
		lcd_vcbus_write(ENCL_TST_EN, 1);
		lcd_vcbus_setb(ENCL_VIDEO_MODE_ADV, 0, 3, 1);
		LCDPR("set mute\n");
	} else {
		lcd_vcbus_setb(ENCL_VIDEO_MODE_ADV, 1, 3, 1);
		lcd_vcbus_write(ENCL_TST_EN, 0);
		LCDPR("clear mute\n");
	}
}

#define CLK_CHK_MAX    2  /*MHz*/
static unsigned int lcd_prbs_performed, lcd_prbs_err;
static unsigned long lcd_encl_clk_check_std = 121;
static unsigned long lcd_fifo_clk_check_std = 42;

static unsigned long lcd_abs(unsigned long a, unsigned long b)
{
	unsigned long val;

	if (a >= b)
		val = a - b;
	else
		val = b - a;

	return val;
}

static int lcd_prbs_clk_check(unsigned long encl_clk, unsigned long fifo_clk,
			      unsigned int cnt)
{
	struct aml_lcd_drv_s *lcd_drv = aml_lcd_get_driver();
	unsigned int encl_clk_msr_id, fifo_clk_msr_id;
	unsigned long clk_check, temp;

	switch (lcd_drv->chip_type) {
	case LCD_CHIP_T5W:
		encl_clk_msr_id = 6;
		fifo_clk_msr_id = 129;
		break;
	default:
		encl_clk_msr_id = 9;
		fifo_clk_msr_id = 129;
		break;
	}

	clk_check = clk_util_clk_msr(encl_clk_msr_id);
	if (clk_check != encl_clk) {
		temp = lcd_abs(clk_check, encl_clk);
		if (temp >= CLK_CHK_MAX) {
			if (lcd_debug_print_flag == 6) {
				LCDERR("encl clkmsr error %ld, cnt: %d\n",
				       clk_check, cnt);
			}
			return -1;
		}
	}

	clk_check = clk_util_clk_msr(fifo_clk_msr_id);
	if (clk_check != fifo_clk) {
		temp = lcd_abs(clk_check, fifo_clk);
		if (temp >= CLK_CHK_MAX) {
			if (lcd_debug_print_flag == 6) {
				LCDERR("fifo clkmsr error %ld, cnt:%d\n",
				       clk_check, cnt);
			}
			return -1;
		}
	}

	return 0;
}

int aml_lcd_prbs_test(unsigned int s, unsigned int mode_flag)
{
	struct lcd_clk_config_s *cconf = get_lcd_clk_config();
	unsigned int lcd_prbs_mode;
	unsigned int val1, val2, timeout;
	unsigned int cnt = 0;
	unsigned int clk_err_cnt = 0;
	int i, j, ret;

	s = (s == 0) ? 1 : ((s > 1800) ? 1800 : s);
	timeout = s * 200;
	for (i = 0; i < LCD_PRBS_MODE_MAX; i++) {
		if ((mode_flag & (1 << i)) == 0)
			continue;

		lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL0, 0);
		lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL1, 0);

		cnt = 0;
		clk_err_cnt = 0;
		lcd_prbs_mode = (1 << i);
		LCDPR("lcd_prbs_mode: %d\n", lcd_prbs_mode);
		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_encl_clk_check_std = 136;
			lcd_fifo_clk_check_std = 48;
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_encl_clk_check_std = 594;
			lcd_fifo_clk_check_std = 297;
		}
		if (cconf->data->prbs_clk_config) {
			cconf->data->prbs_clk_config(lcd_prbs_mode);
		} else {
			LCDERR("%s: prbs_clk_config is null\n", __func__);
			goto lcd_prbs_test_end;
		}
		mdelay(20);

		lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL0, 0x000000c0);
		lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 0xfff, 16, 12);
		lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 1, 2, 1);
		lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL1, 0x41000000);
		lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL1, 1, 31, 1);

		lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL0, 0xfff20c4);
		lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 1, 12, 1);
		val1 = lcd_hiu_getb(HHI_LVDS_TX_PHY_CNTL1, 12, 12);

		while (cnt++ < timeout) {
			mdelay(5);
			ret = 1;
			for (j = 0; j < 5; j++) {
				val2 = lcd_hiu_getb(HHI_LVDS_TX_PHY_CNTL1,
						    12, 12);
				if (val2 != val1) {
					ret = 0;
					break;
				}
			}
			if (ret) {
				LCDERR(
				"prbs check error 1, val:0x%03x, cnt:%d\n",
				       val2, cnt);
				goto lcd_prbs_test_err;
			}
			val1 = val2;
			if (lcd_hiu_getb(HHI_LVDS_TX_PHY_CNTL1, 0, 12)) {
				LCDERR("prbs check error 2, cnt:%d\n", cnt);
				goto lcd_prbs_test_err;
			}
			if (lcd_prbs_clk_check(lcd_encl_clk_check_std,
					       lcd_fifo_clk_check_std,
					       cnt))
				clk_err_cnt++;
			else
				clk_err_cnt = 0;
			if (clk_err_cnt >= 10) {
				LCDERR("prbs check error 3(clkmsr), cnt:%d\n",
				       cnt);
				goto lcd_prbs_test_err;
			}
		}

		lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL0, 0);
		lcd_hiu_write(HHI_LVDS_TX_PHY_CNTL1, 0);

		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_prbs_performed |= LCD_PRBS_MODE_LVDS;
			lcd_prbs_err &= ~(LCD_PRBS_MODE_LVDS);
			LCDPR("lvds prbs check ok\n");
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_prbs_performed |= LCD_PRBS_MODE_VX1;
			lcd_prbs_err &= ~(LCD_PRBS_MODE_VX1);
			LCDPR("vx1 prbs check ok\n");
		} else {
			LCDPR("prbs check: unsupport mode\n");
		}
		continue;

lcd_prbs_test_err:
		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_prbs_performed |= LCD_PRBS_MODE_LVDS;
			lcd_prbs_err |= LCD_PRBS_MODE_LVDS;
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_prbs_performed |= LCD_PRBS_MODE_VX1;
			lcd_prbs_err |= LCD_PRBS_MODE_VX1;
		}
	}

lcd_prbs_test_end:
	lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 0, 12, 2);

	printf("\n[lcd prbs result]:\n");
	printf("  lvds prbs performed: %d, error: %d\n"
	       "  vx1 prbs performed: %d, error: %d\n",
	       (lcd_prbs_performed & LCD_PRBS_MODE_LVDS) ? 1 : 0,
	       (lcd_prbs_err & LCD_PRBS_MODE_LVDS) ? 1 : 0,
	       (lcd_prbs_performed & LCD_PRBS_MODE_VX1) ? 1 : 0,
	       (lcd_prbs_err & LCD_PRBS_MODE_VX1) ? 1 : 0);

	return 0;
}

void aml_lcd_info_print(void)
{
	unsigned int lcd_clk;
	unsigned int sync_duration;
	struct aml_lcd_drv_s *lcd_drv = aml_lcd_get_driver();
	struct lcd_config_s *pconf;

	pconf = lcd_drv->lcd_config;
	LCDPR("driver version: %s\n", lcd_drv->version);
	LCDPR("key_valid: %d\n", pconf->lcd_key_valid);
	LCDPR("mode: %s, status: %d\n",
		lcd_mode_mode_to_str(pconf->lcd_mode),
		lcd_drv->lcd_status);

	lcd_clk = (pconf->lcd_timing.lcd_clk / 1000);
	sync_duration = pconf->lcd_timing.sync_duration_num;
	sync_duration = (sync_duration * 100 / pconf->lcd_timing.sync_duration_den);
	LCDPR("%s, %s %ubit, %ux%u@%u.%2uHz\n"
		"fr_adj_type       %d\n"
		"lcd_clk           %u.%03uMHz\n"
		"ss_level          %u\n\n",
		pconf->lcd_basic.model_name,
		lcd_type_type_to_str(pconf->lcd_basic.lcd_type),
		pconf->lcd_basic.lcd_bits,
		pconf->lcd_basic.h_active, pconf->lcd_basic.v_active,
		(sync_duration / 100), (sync_duration % 100),
		pconf->lcd_timing.fr_adjust_type,
		(lcd_clk / 1000), (lcd_clk % 1000),
		pconf->lcd_timing.ss_level);

	lcd_timing_info_print(pconf);

	if (lcd_debug_info_if) {
		if (lcd_debug_info_if->interface_print)
			lcd_debug_info_if->interface_print(pconf);
		else
			LCDERR("%s: interface_print is null\n", __func__);
	} else {
		LCDERR("%s: lcd_debug_info_if is null\n", __func__);
	}

	lcd_phy_print(pconf);

	lcd_power_info_print(lcd_drv, 1);
	lcd_power_info_print(lcd_drv, 0);
}

void aml_lcd_reg_print(void)
{
	int i = 0;
	unsigned int *table;

	if (lcd_debug_info_reg) {
		if (lcd_debug_info_reg->reg_clk_table) {
			table = lcd_debug_info_reg->reg_clk_table;
			while (i < LCD_DEBUG_REG_CNT_MAX) {
				if (table[i] == LCD_DEBUG_REG_END)
					break;
				printf("hiu     [0x%08x] = 0x%08x\n",
					table[i], lcd_hiu_read(table[i]));
				i++;
			}
		} else {
			LCDERR("%s: reg_clk_table is null\n", __func__);
		}

		if (lcd_debug_info_reg->reg_encl_table) {
			printf("\nencl regs:\n");
			table = lcd_debug_info_reg->reg_encl_table;
			i = 0;
			while (i < LCD_DEBUG_REG_CNT_MAX) {
				if (table[i] == LCD_DEBUG_REG_END)
					break;
				printf("vcbus   [0x%04x] = 0x%08x\n",
					table[i], lcd_vcbus_read(table[i]));
				i++;
			}
		} else {
			LCDERR("%s: reg_encl_table is null\n", __func__);
		}

		if (lcd_debug_info_reg->reg_pinmux_table) {
			printf("\npinmux regs:\n");
			table = lcd_debug_info_reg->reg_pinmux_table;
			i = 0;
			while (i < LCD_DEBUG_REG_CNT_MAX) {
				if (table[i] == LCD_DEBUG_REG_END)
					break;
				printf("PERIPHS_PIN_MUX  [0x%08x] = 0x%08x\n",
					table[i], lcd_periphs_read(table[i]));
				i++;
			}
		}
	} else {
		LCDERR("%s: lcd_debug_info_reg is null\n", __func__);
	}

	if (lcd_debug_info_if) {
		if (lcd_debug_info_if->reg_dump_interface)
			lcd_debug_info_if->reg_dump_interface();
		else
			LCDERR("%s: reg_dump_interface is null\n", __func__);

		if (lcd_debug_info_if->reg_dump_phy)
			lcd_debug_info_if->reg_dump_phy();
	} else {
		LCDERR("%s: lcd_debug_info_if is null\n", __func__);
	}
}

void aml_lcd_vbyone_rst(void)
{
	/* force PHY to 0 */
	lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 3, 8, 2);
	lcd_vcbus_write(VBO_SOFT_RST, 0x1ff);
	udelay(5);
	/* realease PHY */
	if (lcd_vcbus_read(VBO_INSGN_CTRL) & 0x1) {
		LCDPR("clr force lockn input\n");
		lcd_vcbus_setb(VBO_INSGN_CTRL, 0, 0, 1);
	}
	lcd_hiu_setb(HHI_LVDS_TX_PHY_CNTL0, 0, 8, 2);
	lcd_vcbus_write(VBO_SOFT_RST, 0);
	LCDPR("vybone reset\n");
}

void aml_lcd_vbyone_cdr(void)
{
	/*[5:0]: vx1 fsm status*/
	lcd_vcbus_setb(VBO_INSGN_CTRL, 7, 0, 4);
	mdelay(100);
	LCDPR("vx1 fsm status: 0x%08x\n", lcd_vcbus_read(VBO_STATUS_L));
}

/* **********************************
 * lcd debug match data
 * **********************************
 */
/* chip_type data */
static struct lcd_debug_info_reg_s lcd_debug_info_reg_gxtvbb = {
	.reg_clk_table = lcd_reg_dump_clk_dft,
	.reg_encl_table = lcd_reg_dump_encl_dft,
	.reg_pinmux_table = lcd_reg_dump_pinmux_gxtvbb,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_gxl = {
	.reg_clk_table = lcd_reg_dump_clk_dft,
	.reg_encl_table = lcd_reg_dump_encl_dft,
	.reg_pinmux_table = lcd_reg_dump_pinmux_gxl,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_txl = {
	.reg_clk_table = lcd_reg_dump_clk_dft,
	.reg_encl_table = lcd_reg_dump_encl_dft,
	.reg_pinmux_table = lcd_reg_dump_pinmux_txl,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_txlx = {
	.reg_clk_table = lcd_reg_dump_clk_dft,
	.reg_encl_table = lcd_reg_dump_encl_dft,
	.reg_pinmux_table = lcd_reg_dump_pinmux_txlx,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_txhd = {
	.reg_clk_table = lcd_reg_dump_clk_dft,
	.reg_encl_table = lcd_reg_dump_encl_dft,
	.reg_pinmux_table = lcd_reg_dump_pinmux_txhd,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_axg = {
	.reg_clk_table = lcd_reg_dump_clk_axg,
	.reg_encl_table = lcd_reg_dump_encl_dft,
	.reg_pinmux_table = NULL,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_g12a_clk_path0 = {
	.reg_clk_table = lcd_reg_dump_clk_hpll_g12a,
	.reg_encl_table = lcd_reg_dump_encl_dft,
	.reg_pinmux_table = NULL,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_g12a_clk_path1 = {
	.reg_clk_table = lcd_reg_dump_clk_gp0_g12a,
	.reg_encl_table = lcd_reg_dump_encl_dft,
	.reg_pinmux_table = NULL,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_tl1 = {
	.reg_clk_table = lcd_reg_dump_clk_tl1,
	.reg_encl_table = lcd_reg_dump_encl_tl1,
	.reg_pinmux_table = lcd_reg_dump_pinmux_tl1,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_t5w = {
	.reg_clk_table = lcd_reg_dump_clk_tl1,
	.reg_encl_table = lcd_reg_dump_encl_t5w,
	.reg_pinmux_table = lcd_reg_dump_pinmux_t5w,
};

/* interface data */
static struct lcd_debug_info_if_s lcd_debug_info_if_ttl = {
	.interface_print = lcd_info_print_ttl,
	.reg_dump_interface = lcd_reg_print_ttl,
	.reg_dump_phy = NULL,
};

static struct lcd_debug_info_if_s lcd_debug_info_if_lvds = {
	.interface_print = lcd_info_print_lvds,
	.reg_dump_interface = lcd_reg_print_lvds,
	.reg_dump_phy = lcd_reg_print_phy_analog,
};

static struct lcd_debug_info_if_s lcd_debug_info_if_vbyone = {
	.interface_print = lcd_info_print_vbyone,
	.reg_dump_interface = lcd_reg_print_vbyone,
	.reg_dump_phy = lcd_reg_print_phy_analog,
};

static struct lcd_debug_info_if_s lcd_debug_info_if_mipi = {
	.interface_print = lcd_info_print_mipi,
	.reg_dump_interface = lcd_reg_print_mipi,
	.reg_dump_phy = lcd_reg_print_mipi_phy_analog,
};

#ifdef CONFIG_AML_LCD_TCON
static struct lcd_debug_info_if_s lcd_debug_info_if_mlvds = {
	.interface_print = lcd_info_print_mlvds,
	.reg_dump_interface = lcd_reg_print_tcon,
	.reg_dump_phy = lcd_reg_print_phy_analog,
};

static struct lcd_debug_info_if_s lcd_debug_info_if_p2p = {
	.interface_print = lcd_info_print_p2p,
	.reg_dump_interface = lcd_reg_print_tcon_tl1,
	.reg_dump_phy = lcd_reg_print_phy_analog,
};
#endif

void aml_lcd_debug_probe(struct aml_lcd_drv_s *lcd_drv)
{
	int lcd_type;

	lcd_type = lcd_drv->lcd_config->lcd_basic.lcd_type;

	switch (lcd_drv->chip_type) {
	case LCD_CHIP_T5W:
		lcd_debug_info_reg = &lcd_debug_info_reg_t5w;
		lcd_debug_info_if_lvds.reg_dump_interface =
			lcd_reg_print_lvds_tl1;
		lcd_debug_info_if_lvds.reg_dump_phy =
			lcd_reg_print_phy_analog_tl1;
		lcd_debug_info_if_vbyone.reg_dump_interface =
			lcd_reg_print_vbyone_txl;
		lcd_debug_info_if_vbyone.reg_dump_phy =
			lcd_reg_print_phy_analog_tl1;
#ifdef CONFIG_AML_LCD_TCON
		lcd_debug_info_if_mlvds.reg_dump_interface =
			lcd_reg_print_tcon_tl1;
		lcd_debug_info_if_mlvds.reg_dump_phy =
			lcd_reg_print_phy_analog_tl1;
		lcd_debug_info_if_p2p.reg_dump_interface =
			lcd_reg_print_tcon_tl1;
		lcd_debug_info_if_p2p.reg_dump_phy =
			lcd_reg_print_phy_analog_tl1;
#endif
		break;
	case LCD_CHIP_TL1:
	case LCD_CHIP_TM2:
	case LCD_CHIP_T5:
	case LCD_CHIP_T5D:
		lcd_debug_info_reg = &lcd_debug_info_reg_tl1;
		lcd_debug_info_if_lvds.reg_dump_interface =
			lcd_reg_print_lvds_tl1;
		lcd_debug_info_if_lvds.reg_dump_phy =
			lcd_reg_print_phy_analog_tl1;
		lcd_debug_info_if_vbyone.reg_dump_interface =
			lcd_reg_print_vbyone_txl;
		lcd_debug_info_if_vbyone.reg_dump_phy =
			lcd_reg_print_phy_analog_tl1;
#ifdef CONFIG_AML_LCD_TCON
		lcd_debug_info_if_mlvds.reg_dump_interface =
			lcd_reg_print_tcon_tl1;
		lcd_debug_info_if_mlvds.reg_dump_phy =
			lcd_reg_print_phy_analog_tl1;
		lcd_debug_info_if_p2p.reg_dump_interface =
			lcd_reg_print_tcon_tl1;
		lcd_debug_info_if_p2p.reg_dump_phy =
			lcd_reg_print_phy_analog_tl1;
#endif
		break;
	case LCD_CHIP_G12A:
	case LCD_CHIP_G12B:
	case LCD_CHIP_SM1:
		if (lcd_drv->lcd_config->lcd_clk_path)
			lcd_debug_info_reg = &lcd_debug_info_reg_g12a_clk_path1;
		else
			lcd_debug_info_reg = &lcd_debug_info_reg_g12a_clk_path0;
		break;
	case LCD_CHIP_AXG:
		lcd_debug_info_reg = &lcd_debug_info_reg_axg;
		break;
	case LCD_CHIP_TXHD:
		lcd_debug_info_reg = &lcd_debug_info_reg_txhd;
		lcd_debug_info_if_lvds.reg_dump_interface =
			lcd_reg_print_lvds_txhd;
		break;
	case LCD_CHIP_TXLX:
		lcd_debug_info_reg = &lcd_debug_info_reg_txlx;
		lcd_debug_info_if_vbyone.reg_dump_interface =
			lcd_reg_print_vbyone_txl;
		break;
	case LCD_CHIP_TXL:
		lcd_debug_info_reg = &lcd_debug_info_reg_txl;
		lcd_debug_info_if_vbyone.reg_dump_interface =
			lcd_reg_print_vbyone_txl;
		break;
	case LCD_CHIP_GXL:
	case LCD_CHIP_GXM:
		lcd_debug_info_reg = &lcd_debug_info_reg_gxl;
		break;
	case LCD_CHIP_GXTVBB:
		lcd_debug_info_reg = &lcd_debug_info_reg_gxtvbb;
		break;
	default:
		lcd_debug_info_reg = NULL;
		break;
	}

	switch (lcd_type) {
	case LCD_TTL:
		lcd_debug_info_if = &lcd_debug_info_if_ttl;
		break;
	case LCD_LVDS:
		lcd_debug_info_if = &lcd_debug_info_if_lvds;
		break;
	case LCD_VBYONE:
		lcd_debug_info_if = &lcd_debug_info_if_vbyone;
		break;
	case LCD_MIPI:
		lcd_debug_info_if = &lcd_debug_info_if_mipi;
		break;
#ifdef CONFIG_AML_LCD_TCON
	case LCD_MLVDS:
		lcd_debug_info_if = &lcd_debug_info_if_mlvds;
		break;
	case LCD_P2P:
		lcd_debug_info_if = &lcd_debug_info_if_p2p;
		break;
#endif
	default:
		lcd_debug_info_if = NULL;
		break;
	}
}
