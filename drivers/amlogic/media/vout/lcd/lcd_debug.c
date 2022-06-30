// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "lcd_reg.h"
#include "lcd_common.h"
#ifdef CONFIG_AML_LCD_TABLET
#include "lcd_tablet/mipi_dsi_util.h"
#endif
#include "lcd_debug.h"

static void lcd_timing_info_print(struct lcd_config_s * pconf)
{
	unsigned int hs_width, hs_bp, hs_pol, h_period;
	unsigned int vs_width, vs_bp, vs_pol, v_period;
	unsigned int video_hstart, video_vstart;

	video_hstart = pconf->timing.hstart;
	video_vstart = pconf->timing.vstart;
	h_period = pconf->basic.h_period;
	v_period = pconf->basic.v_period;

	hs_width = pconf->timing.hsync_width;
	hs_bp = pconf->timing.hsync_bp;
	hs_pol = pconf->timing.hsync_pol;
	vs_width = pconf->timing.vsync_width;
	vs_bp = pconf->timing.vsync_bp;
	vs_pol = pconf->timing.vsync_pol;

	printf("h_period          %d\n"
		"v_period          %d\n"
		"hs_width          %d\n"
		"hs_backporch      %d\n"
		"hs_pol            %d\n"
		"vs_width          %d\n"
		"vs_backporch      %d\n"
		"vs_pol            %d\n"
		"video_hstart      %d\n"
		"video_vstart      %d\n\n",
		h_period, v_period, hs_width, hs_bp, hs_pol,
		vs_width, vs_bp, vs_pol, video_hstart, video_vstart);

	printf("h_period_min      %d\n"
		"h_period_max      %d\n"
		"v_period_min      %d\n"
		"v_period_max      %d\n"
		"frame_rate_min    %d\n"
		"frame_rate_max    %d\n"
		"pclk_min          %d\n"
		"pclk_max          %d\n\n",
		pconf->basic.h_period_min, pconf->basic.h_period_max,
		pconf->basic.v_period_min, pconf->basic.v_period_max,
		pconf->basic.frame_rate_min, pconf->basic.frame_rate_max,
		pconf->basic.lcd_clk_min, pconf->basic.lcd_clk_max);
}

static void lcd_gpio_info_print(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_power_ctrl_s *lcd_power;
	int i = 0;

	lcd_power = &pdrv->config.power;
	printf("\ncpu_gpio:\n");
	while (i < LCD_CPU_GPIO_NUM_MAX) {
		if (strcmp(lcd_power->cpu_gpio[i], "invalid") == 0)
			break;
		printf("%d: gpio name=%s\n", i, lcd_power->cpu_gpio[i]);
		i++;
	}
}

static void lcd_power_info_print(struct aml_lcd_drv_s *pdrv, int status)
{
	struct lcd_power_step_s *power_step;
	int i;

	if (status) {
		/* check if factory test */
		if (pdrv->factory_lcd_power_on_step) {
			printf("factory test power on step:\n");
			power_step = pdrv->factory_lcd_power_on_step;
		} else {
			printf("power on step:\n");
			power_step = &pdrv->config.power.power_on_step[0];
		}
	} else {
		printf("power off step:\n");
		power_step = &pdrv->config.power.power_off_step[0];
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
		"phy_preem         0x%x\n\n",
		pconf->control.lvds_cfg.lvds_repack,
		pconf->control.lvds_cfg.dual_port,
		pconf->control.lvds_cfg.pn_swap,
		pconf->control.lvds_cfg.port_swap,
		pconf->control.lvds_cfg.lane_reverse,
		pconf->control.lvds_cfg.phy_vswing,
		pconf->control.lvds_cfg.phy_preem);
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
		pconf->control.vbyone_cfg.lane_count,
		pconf->control.vbyone_cfg.region_num,
		pconf->control.vbyone_cfg.byte_mode,
		pconf->timing.bit_rate,
		pconf->control.vbyone_cfg.phy_vswing,
		pconf->control.vbyone_cfg.phy_preem,
		pconf->control.vbyone_cfg.hw_filter_time,
		pconf->control.vbyone_cfg.hw_filter_cnt,
		pconf->control.vbyone_cfg.ctrl_flag);
	if (pconf->control.vbyone_cfg.ctrl_flag & 0x1) {
		printf("power_on_reset_en          %u\n"
			"power_on_reset_delay       %ums\n\n",
			(pconf->control.vbyone_cfg.ctrl_flag & 0x1),
			pconf->control.vbyone_cfg.power_on_reset_delay);
	}
	if (pconf->control.vbyone_cfg.ctrl_flag & 0x2) {
		printf("hpd_data_delay_en          %u\n"
			"hpd_data_delay             %ums\n\n",
			((pconf->control.vbyone_cfg.ctrl_flag >> 1) & 0x1),
			pconf->control.vbyone_cfg.hpd_data_delay);
	}
	if (pconf->control.vbyone_cfg.ctrl_flag & 0x4) {
		printf("cdr_training_hold_en       %u\n"
			"cdr_training_hold          %ums\n\n",
			((pconf->control.vbyone_cfg.ctrl_flag >> 2) & 0x1),
			pconf->control.vbyone_cfg.cdr_training_hold);
	}
	lcd_pinmux_info_print(pconf);
}

static void lcd_info_print_rgb(struct lcd_config_s *pconf)
{
	printf("type              %u\n"
		"clk_pol           %u\n"
		"DE_valid          %u\n"
		"sync_valid        %u\n"
		"rb_swap           %u\n"
		"bit_swap          %u\n\n",
		pconf->control.rgb_cfg.type,
		pconf->control.rgb_cfg.clk_pol,
		pconf->control.rgb_cfg.de_valid,
		pconf->control.rgb_cfg.sync_valid,
		pconf->control.rgb_cfg.rb_swap,
		pconf->control.rgb_cfg.bit_swap);
	lcd_pinmux_info_print(pconf);
}

static void lcd_info_print_bt(struct lcd_config_s *pconf)
{
	printf("clk_phase       %u\n"
		"field_type      %u\n"
		"mode_422        %u\n"
		"yc_swap         %u\n"
		"cbcr_swap       %u\n\n",
		pconf->control.bt_cfg.clk_phase,
		pconf->control.bt_cfg.field_type,
		pconf->control.bt_cfg.mode_422,
		pconf->control.bt_cfg.yc_swap,
		pconf->control.bt_cfg.cbcr_swap);
	lcd_pinmux_info_print(pconf);
}

static void lcd_info_print_mipi(struct lcd_config_s *pconf)
{
#ifdef CONFIG_AML_LCD_TABLET
	mipi_dsi_print_info(pconf);
#endif
}

static void lcd_info_print_edp(struct lcd_config_s *pconf)
{
	printf("max_lane_count        %u\n"
		"max_link_rate         %u\n"
		"training_mode         %u\n"
		"edid_en               %u\n"
		"dpcd_caps_en          %u\n"
		"sync_clk_mode         %u\n"
		"lane_count            %u\n"
		"link_rate             %u\n"
		"bit_rate              %u\n"
		"training_settings     %u\n"
		"main_stream_enable    %u\n"
		"phy_vswing            0x%x\n"
		"phy_preem             0x%x\n\n",
		pconf->control.edp_cfg.max_lane_count,
		pconf->control.edp_cfg.max_link_rate,
		pconf->control.edp_cfg.training_mode,
		pconf->control.edp_cfg.edid_en,
		pconf->control.edp_cfg.dpcd_caps_en,
		pconf->control.edp_cfg.sync_clk_mode,
		pconf->control.edp_cfg.lane_count,
		pconf->control.edp_cfg.link_rate,
		pconf->timing.bit_rate,
		pconf->control.edp_cfg.training_settings,
		pconf->control.edp_cfg.main_stream_enable,
		pconf->control.edp_cfg.phy_vswing,
		pconf->control.edp_cfg.phy_preem);
	printf("edid_state            0x%x\n"
		"edid_retry_cnt        %u\n\n",
		pconf->control.edp_cfg.edid_state,
		pconf->control.edp_cfg.edid_retry_cnt);
	lcd_pinmux_info_print(pconf);
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
		pconf->control.mlvds_cfg.channel_num,
		pconf->control.mlvds_cfg.channel_sel0,
		pconf->control.mlvds_cfg.channel_sel1,
		pconf->control.mlvds_cfg.clk_phase,
		pconf->control.mlvds_cfg.pn_swap,
		pconf->control.mlvds_cfg.bit_swap,
		pconf->control.mlvds_cfg.phy_vswing,
		pconf->control.mlvds_cfg.phy_preem,
		pconf->timing.bit_rate,
		pconf->control.mlvds_cfg.pi_clk_sel);
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
		pconf->control.p2p_cfg.p2p_type,
		pconf->control.p2p_cfg.lane_num,
		pconf->control.p2p_cfg.channel_sel0,
		pconf->control.p2p_cfg.channel_sel1,
		pconf->control.p2p_cfg.pn_swap,
		pconf->control.p2p_cfg.bit_swap,
		pconf->timing.bit_rate,
		pconf->control.p2p_cfg.phy_vswing,
		pconf->control.p2p_cfg.phy_preem);
	lcd_tcon_info_print();
	lcd_pinmux_info_print(pconf);
}
#endif

static void lcd_phy_print(struct lcd_config_s *pconf)
{
	struct phy_config_s *phy = &pconf->phy_cfg;
	int i;

	switch (pconf->basic.lcd_type) {
	case LCD_LVDS:
	case LCD_VBYONE:
	case LCD_MLVDS:
	case LCD_P2P:
	case LCD_EDP:
		printf("ctrl_flag:         0x%x\n"
		"vswing_level:      %u\n"
		"ext_pullup:        %u\n"
		"preem_level:       %u\n"
		"vcm:               0x%x\n"
		"ref_bias:          0x%x\n"
		"odt:               0x%x\n",
		phy->flag,
		phy->vswing_level,
		phy->ext_pullup,
		phy->preem_level,
		phy->vcm,
		phy->ref_bias,
		phy->odt);
		for (i = 0; i < phy->lane_num; i++) {
			printf("lane%d_amp:        0x%x\n"
			"lane%d_preem:      0x%x\n",
			i, phy->lane[i].amp,
			i, phy->lane[i].preem);
		}
		printf("\n");
		break;
	default:
		break;
	}
}

static void lcd_reg_print_rgb(struct aml_lcd_drv_s *pdrv)
{
	printf("\nrgb regs: todo\n");
}

static void lcd_reg_print_bt(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg;

	printf("\nbt656/1120 regs:\n");
	reg = VPU_VOUT_BT_CTRL;
	printf("VPU_VOUT_BT_CTRL     [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VPU_VOUT_BT_PLD_LINE;
	printf("VPU_VOUT_BT_PLD_LINE  [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VPU_VOUT_BT_PLDIDT0;
	printf("VPU_VOUT_BT_PLDIDT0  [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VPU_VOUT_BT_PLDIDT1;
	printf("VPU_VOUT_BT_PLDIDT1  [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VPU_VOUT_BT_BLK_DATA;
	printf("VPU_VOUT_BT_BLK_DATA  [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VPU_VOUT_BT_DAT_CLPY;
	printf("VPU_VOUT_BT_DAT_CLPY  [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
	reg = VPU_VOUT_BT_DAT_CLPC;
	printf("VPU_VOUT_BT_DAT_CLPC [0x%04x] = 0x%08x\n",
		reg, lcd_vcbus_read(reg));
}

static void lcd_reg_print_lvds(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg, offset;

	offset = pdrv->data->offset_venc_if[pdrv->index];

	printf("\nlvds registers:\n");
	reg = LVDS_PACK_CNTL_ADDR + offset;
	printf("LVDS_PACK_CNTL      [0x%04x] = 0x%08x\n",
	       reg, lcd_vcbus_read(reg));
	reg = LVDS_GEN_CNTL + offset;
	printf("LVDS_GEN_CNTL       [0x%04x] = 0x%08x\n",
	       reg, lcd_vcbus_read(reg));

	reg = P2P_CH_SWAP0 + offset;
	printf("P2P_CH_SWAP0        [0x%04x] = 0x%08x\n",
	       reg, lcd_vcbus_read(reg));
	reg = P2P_CH_SWAP1 + offset;
	printf("P2P_CH_SWAP1        [0x%04x] = 0x%08x\n",
	       reg, lcd_vcbus_read(reg));
}

static void lcd_reg_print_vbyone(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg, offset;

	offset = pdrv->data->offset_venc_if[pdrv->index];

	printf("\nvbyone registers:\n");
	reg = VBO_STATUS_L + offset;
	printf("VX1_STATUS          [0x%04x] = 0x%08x\n",
	       reg, lcd_vcbus_read(reg));
	reg = VBO_FSM_HOLDER_L + offset;
	printf("VX1_FSM_HOLDER_L    [0x%04x] = 0x%08x\n",
	       reg, lcd_vcbus_read(reg));
	reg = VBO_FSM_HOLDER_H + offset;
	printf("VX1_FSM_HOLDER_H    [0x%04x] = 0x%08x\n",
	       reg, lcd_vcbus_read(reg));
	reg = VBO_INTR_STATE_CTRL + offset;
	printf("VX1_INTR_STATE_CTRL [0x%04x] = 0x%08x\n",
	       reg, lcd_vcbus_read(reg));
	reg = VBO_INTR_UNMASK + offset;
	printf("VX1_INTR_UNMASK     [0x%04x] = 0x%08x\n",
	       reg, lcd_vcbus_read(reg));
	reg = VBO_INTR_STATE + offset;
	printf("VX1_INTR_STATE      [0x%04x] = 0x%08x\n",
	       reg, lcd_vcbus_read(reg));
	reg = VBO_INSGN_CTRL + offset;
	printf("VBO_INSGN_CTRL      [0x%04x] = 0x%08x\n",
	       reg, lcd_vcbus_read(reg));
}

#ifdef CONFIG_AML_LCD_TCON
static void lcd_reg_print_tcon_tl1(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg;

	printf("\ntcon registers:\n");
	reg = HHI_TCON_CLK_CNTL;
	printf("HHI_TCON_CLK_CNTL   [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
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

static void lcd_reg_print_tcon_t3(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg;

	printf("\ntcon registers:\n");
	reg = CLKCTRL_TCON_CLK_CNTL;
	printf("CLKCTRL_TCON_CLK_CNTL [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = TCON_TOP_CTRL;
	printf("TCON_TOP_CTRL         [0x%04x] = 0x%08x\n",
	       reg, lcd_tcon_read(reg));
	reg = TCON_RGB_IN_MUX;
	printf("TCON_RGB_IN_MUX       [0x%04x] = 0x%08x\n",
	       reg, lcd_tcon_read(reg));
	reg = TCON_OUT_CH_SEL0;
	printf("TCON_OUT_CH_SEL0      [0x%04x] = 0x%08x\n",
	       reg, lcd_tcon_read(reg));
	reg = TCON_OUT_CH_SEL1;
	printf("TCON_OUT_CH_SEL1      [0x%04x] = 0x%08x\n",
	       reg, lcd_tcon_read(reg));
	reg = TCON_STATUS0;
	printf("TCON_STATUS0          [0x%04x] = 0x%08x\n",
	       reg, lcd_tcon_read(reg));
	reg = TCON_PLLLOCK_CNTL;
	printf("TCON_PLLLOCK_CNTL     [0x%04x] = 0x%08x\n",
	       reg, lcd_tcon_read(reg));
	reg = TCON_RST_CTRL;
	printf("TCON_RST_CTRL         [0x%04x] = 0x%08x\n",
	       reg, lcd_tcon_read(reg));
	reg = TCON_AXI_OFST0;
	printf("TCON_AXI_OFST0        [0x%04x] = 0x%08x\n",
	       reg, lcd_tcon_read(reg));
	reg = TCON_AXI_OFST1;
	printf("TCON_AXI_OFST1        [0x%04x] = 0x%08x\n",
	       reg, lcd_tcon_read(reg));
	reg = TCON_AXI_OFST2;
	printf("TCON_AXI_OFST2        [0x%04x] = 0x%08x\n",
	       reg, lcd_tcon_read(reg));
	reg = TCON_CLK_CTRL;
	printf("TCON_CLK_CTRL         [0x%04x] = 0x%08x\n",
	       reg, lcd_tcon_read(reg));
	reg = TCON_STATUS1;
	printf("TCON_STATUS1          [0x%04x] = 0x%08x\n",
	       reg, lcd_tcon_read(reg));
	reg = TCON_DDRIF_CTRL1;
	printf("TCON_DDRIF_CTRL1      [0x%04x] = 0x%08x\n",
	       reg, lcd_tcon_read(reg));
	reg = TCON_DDRIF_CTRL2;
	printf("TCON_DDRIF_CTRL2      [0x%04x] = 0x%08x\n",
	       reg, lcd_tcon_read(reg));

	reg = P2P_CH_SWAP0;
	printf("P2P_CH_SWAP0          [0x%04x] = 0x%08x\n",
	       reg, lcd_vcbus_read(reg));
	reg = P2P_CH_SWAP1;
	printf("P2P_CH_SWAP1          [0x%04x] = 0x%08x\n",
	       reg, lcd_vcbus_read(reg));
}
#endif

static void lcd_reg_print_mipi(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg;
	int index = pdrv->index;

	printf("\nmipi_dsi registers:\n");
	reg = MIPI_DSI_TOP_CNTL;
	printf("MIPI_DSI_TOP_CNTL            [0x%04x] = 0x%08x\n",
	       reg, dsi_host_read(index, reg));
	reg = MIPI_DSI_TOP_CLK_CNTL;
	printf("MIPI_DSI_TOP_CLK_CNTL        [0x%04x] = 0x%08x\n",
	       reg, dsi_host_read(index, reg));
	reg = MIPI_DSI_DWC_PWR_UP_OS;
	printf("MIPI_DSI_DWC_PWR_UP_OS       [0x%04x] = 0x%08x\n",
	       reg, dsi_host_read(index, reg));
	reg = MIPI_DSI_DWC_PCKHDL_CFG_OS;
	printf("MIPI_DSI_DWC_PCKHDL_CFG_OS   [0x%04x] = 0x%08x\n",
	       reg, dsi_host_read(index, reg));
	reg = MIPI_DSI_DWC_LPCLK_CTRL_OS;
	printf("MIPI_DSI_DWC_LPCLK_CTRL_OS   [0x%04x] = 0x%08x\n",
	       reg, dsi_host_read(index, reg));
	reg = MIPI_DSI_DWC_CMD_MODE_CFG_OS;
	printf("MIPI_DSI_DWC_CMD_MODE_CFG_OS [0x%04x] = 0x%08x\n",
	       reg, dsi_host_read(index, reg));
	reg = MIPI_DSI_DWC_VID_MODE_CFG_OS;
	printf("MIPI_DSI_DWC_VID_MODE_CFG_OS [0x%04x] = 0x%08x\n",
	       reg, dsi_host_read(index, reg));
	reg = MIPI_DSI_DWC_MODE_CFG_OS;
	printf("MIPI_DSI_DWC_MODE_CFG_OS     [0x%04x] = 0x%08x\n",
	       reg, dsi_host_read(index, reg));
	reg = MIPI_DSI_DWC_PHY_STATUS_OS;
	printf("MIPI_DSI_DWC_PHY_STATUS_OS   [0x%04x] = 0x%08x\n",
	       reg, dsi_host_read(index, reg));
	reg = MIPI_DSI_DWC_INT_ST0_OS;
	printf("MIPI_DSI_DWC_INT_ST0_OS      [0x%04x] = 0x%08x\n",
	       reg, dsi_host_read(index, reg));
	reg = MIPI_DSI_DWC_INT_ST1_OS;
	printf("MIPI_DSI_DWC_INT_ST1_OS      [0x%04x] = 0x%08x\n",
	       reg, dsi_host_read(index, reg));
	reg = MIPI_DSI_TOP_STAT;
	printf("MIPI_DSI_TOP_STAT            [0x%04x] = 0x%08x\n",
	       reg, dsi_host_read(index, reg));
	reg = MIPI_DSI_TOP_INTR_CNTL_STAT;
	printf("MIPI_DSI_TOP_INTR_CNTL_STAT  [0x%04x] = 0x%08x\n",
	       reg, dsi_host_read(index, reg));
	reg = MIPI_DSI_TOP_MEM_PD;
	printf("MIPI_DSI_TOP_MEM_PD          [0x%04x] = 0x%08x\n",
	       reg, dsi_host_read(index, reg));
}

static void lcd_reg_print_edp(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg;
	int index = pdrv->index;

	if (index > 1) {
		LCDERR("%s: invalid drv_index %d\n", __func__, index);
		return;
	}

	printf("\nedp registers:\n");
	reg = EDP_TX_LINK_BW_SET;
	printf("EDP_TX_LINK_BW_SET               [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));
	reg = EDP_TX_LINK_COUNT_SET;
	printf("EDP_TX_LINK_COUNT_SET            [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));
	reg = EDP_TX_TRAINING_PATTERN_SET;
	printf("EDP_TX_TRAINING_PATTERN_SET      [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));
	reg = EDP_TX_SCRAMBLING_DISABLE;
	printf("EDP_TX_SCRAMBLING_DISABLE        [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));
	reg = EDP_TX_SCRAMBLING_DISABLE;
	printf("EDP_TX_SCRAMBLING_DISABLE        [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));
	reg = EDP_TX_TRANSMITTER_OUTPUT_ENABLE;
	printf("EDP_TX_TRANSMITTER_OUTPUT_ENABLE [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));
	reg = EDP_TX_MAIN_STREAM_ENABLE;
	printf("EDP_TX_MAIN_STREAM_ENABLE        [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));
	reg = EDP_TX_PHY_RESET;
	printf("EDP_TX_PHY_RESET                 [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));
	reg = EDP_TX_PHY_STATUS;
	printf("EDP_TX_PHY_STATUS                [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));
	reg = EDP_TX_AUX_COMMAND;
	printf("EDP_TX_AUX_COMMAND               [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));
	reg = EDP_TX_AUX_ADDRESS;
	printf("EDP_TX_AUX_ADDRESS               [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));
	reg = EDP_TX_AUX_STATE;
	printf("EDP_TX_AUX_STATE                 [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));
	reg = EDP_TX_AUX_REPLY_CODE;
	printf("EDP_TX_AUX_REPLY_CODE            [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));
	reg = EDP_TX_AUX_REPLY_COUNT;
	printf("EDP_TX_AUX_REPLY_COUNT           [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));
	reg = EDP_TX_AUX_REPLY_DATA_COUNT;
	printf("EDP_TX_AUX_REPLY_DATA_COUNT      [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));
	reg = EDP_TX_AUX_TRANSFER_STATUS;
	printf("EDP_TX_AUX_TRANSFER_STATUS       [0x%04x] = 0x%08x\n",
	       reg, dptx_reg_read(index, reg));

	dptx_dpcd_dump(pdrv);
}

static void lcd_reg_print_serializer(void)
{
	unsigned int reg;

	reg = HHI_LVDS_TX_PHY_CNTL0;
	printf("HHI_LVDS_TX_PHY_CNTL0     [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_LVDS_TX_PHY_CNTL1;
	printf("HHI_LVDS_TX_PHY_CNTL1     [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
}

static void lcd_reg_print_phy_analog(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg;

	printf("\nphy analog registers:\n");
	lcd_reg_print_serializer();

	reg = HHI_DIF_CSI_PHY_CNTL1;
	printf("PHY_CNTL1           [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL2;
	printf("PHY_CNTL2           [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL3;
	printf("PHY_CNTL3           [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
}

static void lcd_reg_print_phy_analog_tl1(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg;

	printf("\nphy analog registers:\n");
	lcd_reg_print_serializer();

	reg = HHI_DIF_CSI_PHY_CNTL1;
	printf("PHY_CNTL1           [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL2;
	printf("PHY_CNTL2           [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL3;
	printf("PHY_CNTL3           [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL4;
	printf("PHY_CNTL4           [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL6;
	printf("PHY_CNTL6           [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL7;
	printf("PHY_CNTL7           [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL8;
	printf("PHY_CNTL8           [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL9;
	printf("PHY_CNTL9           [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL10;
	printf("PHY_CNTL10          [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL11;
	printf("PHY_CNTL11          [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL12;
	printf("PHY_CNTL12          [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL13;
	printf("PHY_CNTL13          [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL14;
	printf("PHY_CNTL14          [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL15;
	printf("PHY_CNTL15          [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_DIF_CSI_PHY_CNTL16;
	printf("PHY_CNTL16          [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
}

static void lcd_reg_print_dphy_t7(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg0, reg1;

	switch (pdrv->index) {
	case 1:
		reg0 = COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL0;
		reg1 = COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL1;
		break;
	case 2:
		reg0 = COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL0;
		reg1 = COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL1;
		break;
	case 0:
	default:
		reg0 = COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0;
		reg1 = COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1;
		break;
	}

	printf("COMBO_DPHY_CNTL0    [0x%08x] = 0x%08x\n",
	       COMBO_DPHY_CNTL0, lcd_combo_dphy_read(COMBO_DPHY_CNTL0));
	printf("COMBO_DPHY_CNTL1    [0x%08x] = 0x%08x\n",
	       COMBO_DPHY_CNTL1, lcd_combo_dphy_read(COMBO_DPHY_CNTL1));
	printf("COMBO_DPHY_EDP_LVDS_TX_PHY%d_CNTL0    [0x%08x] = 0x%08x\n",
	       pdrv->index, reg0, lcd_combo_dphy_read(reg0));
	printf("COMBO_DPHY_EDP_LVDS_TX_PHY%d_CNTL1    [0x%08x] = 0x%08x\n",
	       pdrv->index, reg1, lcd_combo_dphy_read(reg1));
}

static void lcd_reg_print_phy_analog_t7(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg;

	printf("\nphy analog registers:\n");
	lcd_reg_print_dphy_t7(pdrv);

	reg = ANACTRL_DIF_PHY_CNTL1;
	printf("PHY_CNTL1           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL2;
	printf("PHY_CNTL2           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL3;
	printf("PHY_CNTL3           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL4;
	printf("PHY_CNTL4           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL5;
	printf("PHY_CNTL5           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL6;
	printf("PHY_CNTL6           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL7;
	printf("PHY_CNTL7           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL8;
	printf("PHY_CNTL8           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL9;
	printf("PHY_CNTL9           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL10;
	printf("PHY_CNTL10          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL11;
	printf("PHY_CNTL11          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL12;
	printf("PHY_CNTL12          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL13;
	printf("PHY_CNTL13          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL14;
	printf("PHY_CNTL14          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL15;
	printf("PHY_CNTL15          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL16;
	printf("PHY_CNTL16          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL17;
	printf("PHY_CNTL17          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL18;
	printf("PHY_CNTL18          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL19;
	printf("PHY_CNTL19          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL20;
	printf("PHY_CNTL20          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL21;
	printf("PHY_CNTL21          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
}

static void lcd_reg_print_dphy_t3(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg0, reg1;

	switch (pdrv->index) {
	case 1:
		reg0 = ANACTRL_LVDS_TX_PHY_CNTL2;
		reg1 = ANACTRL_LVDS_TX_PHY_CNTL3;
		break;
	case 0:
	default:
		reg0 = ANACTRL_LVDS_TX_PHY_CNTL0;
		reg1 = ANACTRL_LVDS_TX_PHY_CNTL1;
		break;
	}

	printf("ANACTRL_LVDS_TX_PHY_CNTL0    [0x%08x] = 0x%08x\n",
	       reg0, lcd_ana_read(reg0));
	printf("ANACTRL_LVDS_TX_PHY_CNTL1    [0x%08x] = 0x%08x\n",
	       reg1, lcd_ana_read(reg1));
}

static void lcd_reg_print_phy_analog_t3(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg;

	printf("\nphy analog registers:\n");
	lcd_reg_print_dphy_t3(pdrv);

	reg = ANACTRL_DIF_PHY_CNTL1;
	printf("PHY_CNTL1           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL2;
	printf("PHY_CNTL2           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL3;
	printf("PHY_CNTL3           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL4;
	printf("PHY_CNTL4           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL5;
	printf("PHY_CNTL5           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL6;
	printf("PHY_CNTL6           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL7;
	printf("PHY_CNTL7           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL8;
	printf("PHY_CNTL8           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL9;
	printf("PHY_CNTL9           [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL10;
	printf("PHY_CNTL10          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL11;
	printf("PHY_CNTL11          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL12;
	printf("PHY_CNTL12          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL13;
	printf("PHY_CNTL13          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL14;
	printf("PHY_CNTL14          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL15;
	printf("PHY_CNTL15          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
	reg = ANACTRL_DIF_PHY_CNTL16;
	printf("PHY_CNTL16          [0x%08x] = 0x%08x\n",
	       reg, lcd_ana_read(reg));
}

static void lcd_reg_print_mipi_phy_analog(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg;
#ifdef CONFIG_AML_LCD_PXP
	return;
#endif
	printf("\nphy analog registers:\n");
	reg = HHI_MIPI_CNTL0;
	printf("PHY_CNTL0   [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_MIPI_CNTL1;
	printf("PHY_CNTL1   [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = HHI_MIPI_CNTL2;
	printf("PHY_CNTL2   [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
}

static void lcd_reg_print_mipi_phy_analog_c3(struct aml_lcd_drv_s *pdrv)
{
	unsigned int reg;
#ifdef CONFIG_AML_LCD_PXP
	return;
#endif
	printf("\nphy analog registers:\n");
	reg = ANACTRL_MIPIDSI_CTRL0;
	printf("PHY_CNTL0   [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = ANACTRL_MIPIDSI_CTRL1;
	printf("PHY_CNTL1   [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
	reg = ANACTRL_MIPIDSI_CTRL2;
	printf("PHY_CNTL2   [0x%08x] = 0x%08x\n",
	       reg, lcd_clk_read(reg));
}

/* **********************************
 * lcd prbs function
 * **********************************
 */
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

static int lcd_prbs_clk_check(unsigned long encl_clk, int encl_msr_id,
			      unsigned long fifo_clk, int fifo_msr_id,
			      unsigned int cnt)
{
	unsigned long clk_check, temp;

	if (encl_msr_id == -1)
		goto lcd_prbs_clk_check_next;
	clk_check = clk_util_clk_msr(encl_msr_id);
	if (clk_check != encl_clk) {
		temp = lcd_abs(clk_check, encl_clk);
		if (temp >= CLK_CHK_MAX) {
			if (lcd_debug_print_flag & LCD_DBG_PR_TEST) {
				LCDERR("encl clkmsr error %ld, cnt: %d\n",
				       clk_check, cnt);
			}
			return -1;
		}
	}

lcd_prbs_clk_check_next:
	if (encl_msr_id == -1)
		return 0;
	clk_check = clk_util_clk_msr(fifo_msr_id);
	if (clk_check != fifo_clk) {
		temp = lcd_abs(clk_check, fifo_clk);
		if (temp >= CLK_CHK_MAX) {
			if (lcd_debug_print_flag & LCD_DBG_PR_TEST) {
				LCDERR("fifo clkmsr error %ld, cnt:%d\n",
				       clk_check, cnt);
			}
			return -1;
		}
	}

	return 0;
}

static int lcd_prbs_test_tl1(struct aml_lcd_drv_s *pdrv, unsigned int ms,
			     unsigned int mode_flag)
{
	struct lcd_clk_config_s *cconf;
	unsigned int reg_phy_tx_ctrl0, reg_phy_tx_ctrl1;
	int encl_msr_id, fifo_msr_id;
	unsigned int lcd_prbs_mode;
	unsigned int val1, val2, s, timeout;
	unsigned int cnt = 0;
	unsigned int clk_err_cnt = 0;
	int i, j, ret;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return -1;

	reg_phy_tx_ctrl0 = HHI_LVDS_TX_PHY_CNTL0;
	reg_phy_tx_ctrl1 = HHI_LVDS_TX_PHY_CNTL1;
	encl_msr_id = 9;
	fifo_msr_id = 129;

	s = ms / 1000;
	s = (s == 0) ? 1 : ((s > 1800) ? 1800 : s);
	timeout = s * 200;
	for (i = 0; i < LCD_PRBS_MODE_MAX; i++) {
		if ((mode_flag & (1 << i)) == 0)
			continue;

		lcd_ana_write(reg_phy_tx_ctrl0, 0);
		lcd_ana_write(reg_phy_tx_ctrl1, 0);

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
			cconf->data->prbs_clk_config(pdrv, lcd_prbs_mode);
		} else {
			LCDERR("%s: prbs_clk_config is null\n", __func__);
			goto lcd_prbs_test_end;
		}
		mdelay(20);

		lcd_ana_write(reg_phy_tx_ctrl0, 0x000000c0);
		lcd_ana_setb(reg_phy_tx_ctrl0, 0xfff, 16, 12);
		lcd_ana_setb(reg_phy_tx_ctrl0, 1, 2, 1);
		lcd_ana_write(reg_phy_tx_ctrl1, 0x41000000);
		lcd_ana_setb(reg_phy_tx_ctrl1, 1, 31, 1);

		lcd_ana_write(reg_phy_tx_ctrl0, 0xfff20c4);
		lcd_ana_setb(reg_phy_tx_ctrl0, 1, 12, 1);
		val1 = lcd_ana_getb(reg_phy_tx_ctrl1, 12, 12);

		while (cnt++ < timeout) {
			mdelay(5);
			ret = 1;
			for (j = 0; j < 5; j++) {
				val2 = lcd_ana_getb(reg_phy_tx_ctrl1, 12, 12);
				if (val2 != val1) {
					ret = 0;
					break;
				}
			}
			if (ret) {
				LCDERR("prbs check error 1, val:0x%03x, cnt:%d\n",
				       val2, cnt);
				goto lcd_prbs_test_err;
			}
			val1 = val2;
			if (lcd_ana_getb(reg_phy_tx_ctrl1, 0, 12)) {
				LCDERR("prbs check error 2, cnt:%d\n", cnt);
				goto lcd_prbs_test_err;
			}
			if (lcd_prbs_clk_check(lcd_encl_clk_check_std, encl_msr_id,
					       lcd_fifo_clk_check_std, fifo_msr_id,
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

		lcd_ana_write(reg_phy_tx_ctrl0, 0);
		lcd_ana_write(reg_phy_tx_ctrl1, 0);

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
	lcd_ana_setb(reg_phy_tx_ctrl0, 0, 12, 2);

	printf("\n[lcd prbs result]:\n");
	printf("  lvds prbs performed: %d, error: %d\n"
	       "  vx1 prbs performed: %d, error: %d\n",
	       (lcd_prbs_performed & LCD_PRBS_MODE_LVDS) ? 1 : 0,
	       (lcd_prbs_err & LCD_PRBS_MODE_LVDS) ? 1 : 0,
	       (lcd_prbs_performed & LCD_PRBS_MODE_VX1) ? 1 : 0,
	       (lcd_prbs_err & LCD_PRBS_MODE_VX1) ? 1 : 0);

	return 0;
}

static int lcd_prbs_test_t7(struct aml_lcd_drv_s *pdrv, unsigned int ms,
			    unsigned int mode_flag)
{
	struct lcd_clk_config_s *cconf;
	unsigned int reg_phy_tx_ctrl0, reg_phy_tx_ctrl1, reg_ctrl_out, bit_width;
	int encl_msr_id, fifo_msr_id;
	unsigned int lcd_prbs_mode;
	unsigned int val1, val2, timeout;
	unsigned int cnt = 0;
	unsigned int clk_err_cnt = 0;
	int i, j, ret;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return -1;

	switch (pdrv->index) {
	case 0:
		reg_phy_tx_ctrl0 = COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL0;
		reg_phy_tx_ctrl1 = COMBO_DPHY_EDP_LVDS_TX_PHY0_CNTL1;
		reg_ctrl_out = COMBO_DPHY_RO_EDP_LVDS_TX_PHY0_CNTL1;
		bit_width = 8;
		break;
	case 1:
		reg_phy_tx_ctrl0 = COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL0;
		reg_phy_tx_ctrl1 = COMBO_DPHY_EDP_LVDS_TX_PHY1_CNTL1;
		reg_ctrl_out = COMBO_DPHY_RO_EDP_LVDS_TX_PHY1_CNTL1;
		bit_width = 8;
		break;
	case 2:
		reg_phy_tx_ctrl0 = COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL0;
		reg_phy_tx_ctrl1 = COMBO_DPHY_EDP_LVDS_TX_PHY2_CNTL1;
		reg_ctrl_out = COMBO_DPHY_RO_EDP_LVDS_TX_PHY2_CNTL1;
		bit_width = 10;
		break;
	default:
		LCDERR("[%d]: %s: invalid drv_index\n",
			pdrv->index, __func__);
		return -1;
	}
	encl_msr_id = cconf->data->enc_clk_msr_id;
	fifo_msr_id = -1;

	timeout = (ms > 1000) ? 1000 : ms;

	for (i = 0; i < LCD_PRBS_MODE_MAX; i++) {
		if ((mode_flag & (1 << i)) == 0)
			continue;

		lcd_combo_dphy_write(reg_phy_tx_ctrl0, 0);
		lcd_combo_dphy_write(reg_phy_tx_ctrl1, 0);

		cnt = 0;
		clk_err_cnt = 0;
		lcd_prbs_mode = (1 << i);
		LCDPR("[%d]: lcd_prbs_mode: %d\n", pdrv->index, lcd_prbs_mode);
		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_encl_clk_check_std = 136;
			lcd_fifo_clk_check_std = 48;
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_encl_clk_check_std = 594;
			lcd_fifo_clk_check_std = 297;
		}
		if (!cconf->data->prbs_clk_config) {
			LCDERR("[%d]: %s: prbs_clk_config is null\n",
			       pdrv->index, __func__);
			goto lcd_prbs_test_t7_end;

		}
		cconf->data->prbs_clk_config(pdrv, lcd_prbs_mode);
		udelay(500);

		/* set fifo_clk_sel: div 10 */
		lcd_combo_dphy_write(reg_phy_tx_ctrl0, (3 << 5));
		/* set cntl_ser_en:  10-channel */
		lcd_combo_dphy_setb(reg_phy_tx_ctrl0, 0x3ff, 16, 10);
		lcd_combo_dphy_setb(reg_phy_tx_ctrl0, 1, 2, 1);
		/* decoupling fifo enable, gated clock enable */
		lcd_combo_dphy_write(reg_phy_tx_ctrl1, (1 << 6) | (1 << 0));
		/* decoupling fifo write enable after fifo enable */
		lcd_combo_dphy_setb(reg_phy_tx_ctrl1, 1, 7, 1);
		/* prbs_err en */
		lcd_combo_dphy_setb(reg_phy_tx_ctrl0, 1, 13, 1);
		lcd_combo_dphy_setb(reg_phy_tx_ctrl0, 1, 12, 1);

		while (cnt++ < timeout) {
			val1 = lcd_combo_dphy_getb(reg_ctrl_out, bit_width, bit_width);
			udelay(1000);
			ret = 1;
			for (j = 0; j < 20; j++) {
				udelay(5);
				val2 = lcd_combo_dphy_getb(reg_ctrl_out, bit_width, bit_width);
				if (val2 != val1) {
					ret = 0;
					break;
				}
			}
			if (ret) {
				LCDERR("[%d]: prbs check error 1, val:0x%03x, cnt:%d\n",
				       pdrv->index, val2, cnt);
				goto lcd_prbs_test_t7_err;
			}
			if (lcd_combo_dphy_getb(reg_ctrl_out, 0, bit_width)) {
				LCDERR("[%d]: prbs check error 2, cnt:%d\n",
				       pdrv->index, cnt);
				goto lcd_prbs_test_t7_err;
			}
			if (lcd_prbs_clk_check(lcd_encl_clk_check_std, encl_msr_id,
					       lcd_fifo_clk_check_std, fifo_msr_id,
					       cnt))
				clk_err_cnt++;
			else
				clk_err_cnt = 0;
			if (clk_err_cnt >= 10) {
				LCDERR("[%d]: prbs check error 3(clkmsr), cnt:%d\n",
				       pdrv->index, cnt);
				goto lcd_prbs_test_t7_err;
			}
		}

		lcd_combo_dphy_write(reg_phy_tx_ctrl0, 0);
		lcd_combo_dphy_write(reg_phy_tx_ctrl1, 0);

		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_prbs_performed |= LCD_PRBS_MODE_LVDS;
			lcd_prbs_err &= ~(LCD_PRBS_MODE_LVDS);
			LCDPR("[%d]: lvds prbs check ok\n", pdrv->index);
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_prbs_performed |= LCD_PRBS_MODE_VX1;
			lcd_prbs_err &= ~(LCD_PRBS_MODE_VX1);
			LCDPR("[%d]: vx1 prbs check ok\n", pdrv->index);
		} else {
			LCDPR("[%d]: prbs check: unsupport mode\n", pdrv->index);
		}
		continue;

lcd_prbs_test_t7_err:
		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_prbs_performed |= LCD_PRBS_MODE_LVDS;
			lcd_prbs_err |= LCD_PRBS_MODE_LVDS;
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_prbs_performed |= LCD_PRBS_MODE_VX1;
			lcd_prbs_err |= LCD_PRBS_MODE_VX1;
		}
	}

lcd_prbs_test_t7_end:
	lcd_combo_dphy_setb(reg_phy_tx_ctrl0, 0, 12, 2);

	printf("\n[[%d]: lcd prbs result]:\n", pdrv->index);
	printf("  lvds prbs performed: %d, error: %d\n"
	       "  vx1 prbs performed: %d, error: %d\n",
	       (lcd_prbs_performed & LCD_PRBS_MODE_LVDS) ? 1 : 0,
	       (lcd_prbs_err & LCD_PRBS_MODE_LVDS) ? 1 : 0,
	       (lcd_prbs_performed & LCD_PRBS_MODE_VX1) ? 1 : 0,
	       (lcd_prbs_err & LCD_PRBS_MODE_VX1) ? 1 : 0);

	return 0;
}

static int lcd_prbs_test_t3(struct aml_lcd_drv_s *pdrv, unsigned int ms,
			    unsigned int mode_flag)
{
	struct lcd_clk_config_s *cconf;
	unsigned int reg_phy_tx_ctrl0, reg_phy_tx_ctrl1;
	int encl_msr_id, fifo_msr_id;
	unsigned int lcd_prbs_mode;
	unsigned int val1, val2, timeout;
	unsigned int cnt = 0;
	unsigned int clk_err_cnt = 0;
	int i, j, ret;

	cconf = get_lcd_clk_config(pdrv);
	if (!cconf)
		return -1;

	switch (pdrv->index) {
	case 0:
		reg_phy_tx_ctrl0 = ANACTRL_LVDS_TX_PHY_CNTL0;
		reg_phy_tx_ctrl1 = ANACTRL_LVDS_TX_PHY_CNTL1;
		break;
	case 1:
		reg_phy_tx_ctrl0 = ANACTRL_LVDS_TX_PHY_CNTL2;
		reg_phy_tx_ctrl1 = ANACTRL_LVDS_TX_PHY_CNTL3;
		break;
	default:
		LCDERR("[%d]: %s: invalid drv_index\n",
			pdrv->index, __func__);
		return -1;
	}
	encl_msr_id = cconf->data->enc_clk_msr_id;
	fifo_msr_id = -1;

	timeout = (ms > 1000) ? 1000 : ms;

	for (i = 0; i < LCD_PRBS_MODE_MAX; i++) {
		if ((mode_flag & (1 << i)) == 0)
			continue;

		lcd_ana_write(reg_phy_tx_ctrl0, 0);
		lcd_ana_write(reg_phy_tx_ctrl1, 0);

		cnt = 0;
		clk_err_cnt = 0;
		lcd_prbs_mode = (1 << i);
		LCDPR("[%d]: lcd_prbs_mode: %d\n", pdrv->index, lcd_prbs_mode);
		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_encl_clk_check_std = 136;
			lcd_fifo_clk_check_std = 48;
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_encl_clk_check_std = 594;
			lcd_fifo_clk_check_std = 297;
		}
		if (!cconf->data->prbs_clk_config) {
			LCDERR("[%d]: %s: prbs_clk_config is null\n",
			       pdrv->index, __func__);
			goto lcd_prbs_test_t3_end;

		}
		cconf->data->prbs_clk_config(pdrv, lcd_prbs_mode);
		udelay(500);

		/* set fifo_clk_sel: div 10 */
		lcd_ana_write(reg_phy_tx_ctrl0, (3 << 6));
		/* set cntl_ser_en:  8-channel to 1 */
		lcd_ana_setb(reg_phy_tx_ctrl0, 0xfff, 16, 12);
		lcd_ana_setb(reg_phy_tx_ctrl0, 1, 2, 1);
		/* decoupling fifo enable, gated clock enable */
		lcd_ana_write(reg_phy_tx_ctrl1, (1 << 30) | (1 << 24));
		/* decoupling fifo write enable after fifo enable */
		lcd_ana_setb(reg_phy_tx_ctrl1, 1, 31, 1);
		/* prbs_err en */
		lcd_ana_setb(reg_phy_tx_ctrl0, 1, 13, 1);
		lcd_ana_setb(reg_phy_tx_ctrl0, 1, 12, 1);

		while (cnt++ < timeout) {
			val1 = lcd_ana_getb(reg_phy_tx_ctrl1, 12, 12);
			udelay(1000);
			ret = 1;
			for (j = 0; j < 20; j++) {
				udelay(5);
				val2 = lcd_ana_getb(reg_phy_tx_ctrl1, 12, 12);
				if (val2 != val1) {
					ret = 0;
					break;
				}
			}
			if (ret) {
				LCDERR("[%d]: prbs check error 1, val:0x%03x, cnt:%d\n",
				       pdrv->index, val2, cnt);
				goto lcd_prbs_test_t3_err;
			}
			if (lcd_ana_getb(reg_phy_tx_ctrl1, 0, 12)) {
				LCDERR("[%d]: prbs check error 2, cnt:%d\n",
				       pdrv->index, cnt);
				goto lcd_prbs_test_t3_err;
			}
			if (lcd_prbs_clk_check(lcd_encl_clk_check_std, encl_msr_id,
					       lcd_fifo_clk_check_std, fifo_msr_id,
					       cnt))
				clk_err_cnt++;
			else
				clk_err_cnt = 0;
			if (clk_err_cnt >= 10) {
				LCDERR("[%d]: prbs check error 3(clkmsr), cnt:%d\n",
				       pdrv->index, cnt);
				goto lcd_prbs_test_t3_err;
			}
		}

		lcd_ana_write(reg_phy_tx_ctrl0, 0);
		lcd_ana_write(reg_phy_tx_ctrl1, 0);

		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_prbs_performed |= LCD_PRBS_MODE_LVDS;
			lcd_prbs_err &= ~(LCD_PRBS_MODE_LVDS);
			LCDPR("[%d]: lvds prbs check ok\n", pdrv->index);
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_prbs_performed |= LCD_PRBS_MODE_VX1;
			lcd_prbs_err &= ~(LCD_PRBS_MODE_VX1);
			LCDPR("[%d]: vx1 prbs check ok\n", pdrv->index);
		} else {
			LCDPR("[%d]: prbs check: unsupport mode\n", pdrv->index);
		}
		continue;

lcd_prbs_test_t3_err:
		if (lcd_prbs_mode == LCD_PRBS_MODE_LVDS) {
			lcd_prbs_performed |= LCD_PRBS_MODE_LVDS;
			lcd_prbs_err |= LCD_PRBS_MODE_LVDS;
		} else if (lcd_prbs_mode == LCD_PRBS_MODE_VX1) {
			lcd_prbs_performed |= LCD_PRBS_MODE_VX1;
			lcd_prbs_err |= LCD_PRBS_MODE_VX1;
		}
	}

lcd_prbs_test_t3_end:
	lcd_ana_setb(reg_phy_tx_ctrl0, 0, 12, 2);

	printf("\n[[%d]: lcd prbs result]:\n", pdrv->index);
	printf("  lvds prbs performed: %d, error: %d\n"
	       "  vx1 prbs performed: %d, error: %d\n",
	       (lcd_prbs_performed & LCD_PRBS_MODE_LVDS) ? 1 : 0,
	       (lcd_prbs_err & LCD_PRBS_MODE_LVDS) ? 1 : 0,
	       (lcd_prbs_performed & LCD_PRBS_MODE_VX1) ? 1 : 0,
	       (lcd_prbs_err & LCD_PRBS_MODE_VX1) ? 1 : 0);

	return 0;
}

int lcd_prbs_test(struct aml_lcd_drv_s *pdrv, unsigned int ms,
		  unsigned int mode_flag)
{
	struct lcd_debug_info_reg_s *info_reg;
	int ret = -1;

	info_reg = (struct lcd_debug_info_reg_s *)pdrv->debug_info_reg;
	if (info_reg && info_reg->prbs_test)
		ret = info_reg->prbs_test(pdrv, ms, mode_flag);
	else
		LCDERR("[%d]: %s: don't support prbs test\n", pdrv->index, __func__);

	return ret;
}

void lcd_info_print(struct aml_lcd_drv_s *pdrv)
{
	unsigned int lcd_clk;
	unsigned int sync_duration;
	struct lcd_config_s *pconf;
	struct lcd_debug_info_if_s *info_if;

	pconf = &pdrv->config;
	LCDPR("[%d]: lcd driver version: %s\n", pdrv->index, LCD_DRV_VERSION);
	LCDPR("key_valid: %d\n", pdrv->key_valid);
	LCDPR("custom_pinmux: %d\n", pconf->custom_pinmux);
	LCDPR("mode: %s, status: %d\n",
	      lcd_mode_mode_to_str(pdrv->mode), pdrv->status);

	lcd_clk = (pconf->timing.lcd_clk / 1000);
	sync_duration = pconf->timing.sync_duration_num;
	sync_duration = (sync_duration * 100 / pconf->timing.sync_duration_den);
	LCDPR("%s, %s %ubit, %ux%u@%u.%02uHz\n"
		"fr_adj_type       %d\n"
		"lcd_clk           %u.%03uMHz\n"
		"ss_level          0x%x\n\n",
		pconf->basic.model_name,
		lcd_type_type_to_str(pconf->basic.lcd_type),
		pconf->basic.lcd_bits,
		pconf->basic.h_active, pconf->basic.v_active,
		(sync_duration / 100), (sync_duration % 100),
		pconf->timing.fr_adjust_type,
		(lcd_clk / 1000), (lcd_clk % 1000),
		pconf->timing.ss_level);

	lcd_timing_info_print(pconf);

	info_if = (struct lcd_debug_info_if_s *)pdrv->debug_info_if;
	if (info_if) {
		if (info_if->interface_print)
			info_if->interface_print(pconf);
		else
			LCDERR("%s: interface_print is null\n", __func__);
	} else {
		LCDERR("%s: lcd_debug_info_if is null\n", __func__);
	}

	lcd_phy_print(pconf);

	/* cus_ctrl_attr */
	if (pconf->cus_ctrl.flag) {
		LCDPR("\nlcd cus_ctrl:\n"
			"ctrl_flag:         0x%x\n"
			"dlg_flag:          %u\n",
			pconf->cus_ctrl.flag,
			pconf->cus_ctrl.dlg_flag);
	}

	lcd_power_info_print(pdrv, 1);
	lcd_power_info_print(pdrv, 0);

	lcd_gpio_info_print(pdrv);
}

void lcd_reg_print(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_debug_info_reg_s *info_reg;
	struct lcd_debug_info_if_s *info_if;
	unsigned int *table;
	int i = 0;

	info_reg = (struct lcd_debug_info_reg_s *)pdrv->debug_info_reg;
	info_if = (struct lcd_debug_info_if_s *)pdrv->debug_info_if;

	if (!info_reg) {
		LCDERR("%s: lcd_debug_info_reg is null\n", __func__);
		goto lcd_reg_print_next;
	}
	LCDPR("[%d]: lcd regs:\n", pdrv->index);
	if (info_reg->reg_pll_table) {
		table = info_reg->reg_pll_table;
		i = 0;
		while (i < LCD_DEBUG_REG_CNT_MAX) {
			if (table[i] == LCD_DEBUG_REG_END)
				break;
			printf("pll     [0x%08x] = 0x%08x\n",
				table[i], lcd_ana_read(table[i]));
			i++;
		}
	}
	if (info_reg->reg_clk_table) {
		table = info_reg->reg_clk_table;
		i = 0;
		while (i < LCD_DEBUG_REG_CNT_MAX) {
			if (table[i] == LCD_DEBUG_REG_END)
				break;
			printf("clk     [0x%08x] = 0x%08x\n",
				table[i], lcd_clk_read(table[i]));
			i++;
		}
	}

	if (info_reg->reg_encl_table) {
		printf("\nencl regs:\n");
		table = info_reg->reg_encl_table;
		i = 0;
		while (i < LCD_DEBUG_REG_CNT_MAX) {
			if (table[i] == LCD_DEBUG_REG_END)
				break;
			printf("vcbus   [0x%04x] = 0x%08x\n",
				table[i], lcd_vcbus_read(table[i]));
			i++;
		}
	}

	if (info_reg->reg_pinmux_table) {
		printf("\npinmux regs:\n");
		table = info_reg->reg_pinmux_table;
		i = 0;
		while (i < LCD_DEBUG_REG_CNT_MAX) {
			if (table[i] == LCD_DEBUG_REG_END)
				break;
			printf("PERIPHS_PIN_MUX  [0x%08x] = 0x%08x\n",
				table[i], lcd_periphs_read(table[i]));
			i++;
		}
	}

lcd_reg_print_next:
	if (!info_if) {
		LCDERR("%s: lcd_debug_info_if is null\n", __func__);
		return;
	}
	if (info_if->reg_dump_interface)
		info_if->reg_dump_interface(pdrv);

	if (info_if->reg_dump_phy)
		info_if->reg_dump_phy(pdrv);
}

void lcd_vbyone_rst(struct aml_lcd_drv_s *pdrv)
{
	unsigned int offset;

	offset = pdrv->data->offset_venc_if[pdrv->index];

	/* realease PHY */
	if (lcd_vcbus_read(VBO_INSGN_CTRL + offset) & 0x1) {
		LCDPR("[%d]: clr force lockn input\n", pdrv->index);
		lcd_vcbus_setb(VBO_INSGN_CTRL + offset, 0, 0, 1);
	}
	lcd_vbyone_sw_reset(pdrv);
	LCDPR("[%d]: vbyone reset\n", pdrv->index);
}

void lcd_vbyone_cdr(struct aml_lcd_drv_s *pdrv)
{
	unsigned int offset;

	offset = pdrv->data->offset_venc_if[pdrv->index];

	/*[5:0]: vx1 fsm status*/
	lcd_vcbus_setb(VBO_INSGN_CTRL + offset, 7, 0, 4);
	mdelay(100);
	LCDPR("[%d]: vbyone fsm status: 0x%08x\n",
	      pdrv->index, lcd_vcbus_read(VBO_STATUS_L + offset));
}

/* **********************************
 * lcd debug match data
 * **********************************
 */
/* chip_type data */
static struct lcd_debug_info_reg_s lcd_debug_info_reg_g12a_clk_path0 = {
	.reg_pll_table = NULL,
	.reg_clk_table = lcd_reg_dump_clk_hpll_g12a,
	.reg_encl_table = lcd_reg_dump_encl_dft,
	.reg_pinmux_table = NULL,
	.prbs_test = NULL,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_g12a_clk_path1 = {
	.reg_pll_table = NULL,
	.reg_clk_table = lcd_reg_dump_clk_gp0_g12a,
	.reg_encl_table = lcd_reg_dump_encl_dft,
	.reg_pinmux_table = NULL,
	.prbs_test = NULL,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_tl1 = {
	.reg_pll_table = NULL,
	.reg_clk_table = lcd_reg_dump_clk_tl1,
	.reg_encl_table = lcd_reg_dump_encl_tl1,
	.reg_pinmux_table = lcd_reg_dump_pinmux_tl1,
	.prbs_test = lcd_prbs_test_tl1,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_t7_0 = {
	.reg_pll_table = lcd_reg_dump_pll_t7_0,
	.reg_clk_table = lcd_reg_dump_clk_t7_0,
	.reg_encl_table = lcd_reg_dump_encl_t7_0,
	.reg_pinmux_table = lcd_reg_dump_pinmux_t7,
	.prbs_test = lcd_prbs_test_t7,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_t7_1 = {
	.reg_pll_table = lcd_reg_dump_pll_t7_1,
	.reg_clk_table = lcd_reg_dump_clk_t7_1,
	.reg_encl_table = lcd_reg_dump_encl_t7_1,
	.reg_pinmux_table = lcd_reg_dump_pinmux_t7,
	.prbs_test = lcd_prbs_test_t7,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_t7_2 = {
	.reg_pll_table = lcd_reg_dump_pll_t7_2,
	.reg_clk_table = lcd_reg_dump_clk_t7_2,
	.reg_encl_table = lcd_reg_dump_encl_t7_2,
	.reg_pinmux_table = lcd_reg_dump_pinmux_t7,
	.prbs_test = lcd_prbs_test_t7,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_t3_0 = {
	.reg_pll_table = lcd_reg_dump_pll_t3,
	.reg_clk_table = lcd_reg_dump_clk_t7_0,
	.reg_encl_table = lcd_reg_dump_encl_t7_0,
	.reg_pinmux_table = lcd_reg_dump_pinmux_t3,
	.prbs_test = lcd_prbs_test_t3,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_t3_1 = {
	.reg_pll_table = lcd_reg_dump_pll_t3,
	.reg_clk_table = lcd_reg_dump_clk_t7_1,
	.reg_encl_table = lcd_reg_dump_encl_t7_1,
	.reg_pinmux_table = lcd_reg_dump_pinmux_t3,
	.prbs_test = lcd_prbs_test_t3,
};

static struct lcd_debug_info_reg_s lcd_debug_info_reg_c3 = {
	.reg_pll_table = lcd_reg_dump_pll_c3,
	.reg_clk_table = lcd_reg_dump_clk_c3,
	.reg_encl_table = lcd_reg_dump_encl_c3,
	.reg_pinmux_table = lcd_reg_dump_pinmux_c3,
	.prbs_test = NULL,
};

/* interface data */
static struct lcd_debug_info_if_s lcd_debug_info_if_rgb = {
	.interface_print = lcd_info_print_rgb,
	.reg_dump_interface = lcd_reg_print_rgb,
	.reg_dump_phy = NULL,
};

static struct lcd_debug_info_if_s lcd_debug_info_if_bt = {
	.interface_print = lcd_info_print_bt,
	.reg_dump_interface = lcd_reg_print_bt,
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

static struct lcd_debug_info_if_s lcd_debug_info_if_edp = {
	.interface_print = lcd_info_print_edp,
	.reg_dump_interface = lcd_reg_print_edp,
	.reg_dump_phy = lcd_reg_print_phy_analog_t7,
};

#ifdef CONFIG_AML_LCD_TCON
static struct lcd_debug_info_if_s lcd_debug_info_if_mlvds = {
	.interface_print = lcd_info_print_mlvds,
	.reg_dump_interface = lcd_reg_print_tcon_tl1,
	.reg_dump_phy = lcd_reg_print_phy_analog,
};

static struct lcd_debug_info_if_s lcd_debug_info_if_p2p = {
	.interface_print = lcd_info_print_p2p,
	.reg_dump_interface = lcd_reg_print_tcon_tl1,
	.reg_dump_phy = lcd_reg_print_phy_analog,
};
#endif

void lcd_debug_probe(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_debug_info_reg_s *lcd_debug_info_reg = NULL;
	struct lcd_debug_info_if_s *lcd_debug_info_if = NULL;

	switch (pdrv->data->chip_type) {
	case LCD_CHIP_T7:
		switch (pdrv->index) {
		case 1:
			lcd_debug_info_reg = &lcd_debug_info_reg_t7_1;
			break;
		case 2:
			lcd_debug_info_reg = &lcd_debug_info_reg_t7_2;
			break;
		case 0:
		default:
			lcd_debug_info_reg = &lcd_debug_info_reg_t7_0;
			break;
		}
		lcd_debug_info_if_lvds.reg_dump_phy =
			lcd_reg_print_phy_analog_t7;
		lcd_debug_info_if_vbyone.reg_dump_phy =
			lcd_reg_print_phy_analog_t7;
		lcd_debug_info_if_mipi.reg_dump_phy =
			lcd_reg_print_phy_analog_t7;
		break;
	case LCD_CHIP_T3:
		switch (pdrv->index) {
		case 1:
			lcd_debug_info_reg = &lcd_debug_info_reg_t3_1;
			break;
		default:
			lcd_debug_info_reg = &lcd_debug_info_reg_t3_0;
			break;
		}
		lcd_debug_info_if_lvds.reg_dump_phy =
			lcd_reg_print_phy_analog_t3;
		lcd_debug_info_if_vbyone.reg_dump_phy =
			lcd_reg_print_phy_analog_t3;
#ifdef CONFIG_AML_LCD_TCON
		lcd_debug_info_if_mlvds.reg_dump_interface =
			lcd_reg_print_tcon_t3;
		lcd_debug_info_if_mlvds.reg_dump_phy =
			lcd_reg_print_phy_analog_t3;
		lcd_debug_info_if_p2p.reg_dump_interface =
			lcd_reg_print_tcon_t3;
		lcd_debug_info_if_p2p.reg_dump_phy =
			lcd_reg_print_phy_analog_t3;
#endif
		break;
	case LCD_CHIP_TL1:
	case LCD_CHIP_TM2:
	case LCD_CHIP_T5:
	case LCD_CHIP_T5D:
		lcd_debug_info_reg = &lcd_debug_info_reg_tl1;
		lcd_debug_info_if_lvds.reg_dump_phy =
			lcd_reg_print_phy_analog_tl1;
		lcd_debug_info_if_mipi.reg_dump_phy =
			lcd_reg_print_phy_analog_tl1;
#ifdef CONFIG_AML_LCD_TCON
		lcd_debug_info_if_mlvds.reg_dump_phy =
			lcd_reg_print_phy_analog_tl1;
		lcd_debug_info_if_p2p.reg_dump_phy =
			lcd_reg_print_phy_analog_tl1;
#endif
		break;
	case LCD_CHIP_G12A:
	case LCD_CHIP_G12B:
	case LCD_CHIP_SM1:
		if (pdrv->clk_path)
			lcd_debug_info_reg = &lcd_debug_info_reg_g12a_clk_path1;
		else
			lcd_debug_info_reg = &lcd_debug_info_reg_g12a_clk_path0;
		break;
	case LCD_CHIP_C3:
		lcd_debug_info_reg = &lcd_debug_info_reg_c3;
		lcd_debug_info_if_mipi.reg_dump_phy =
			lcd_reg_print_mipi_phy_analog_c3;
		break;
	default:
		lcd_debug_info_reg = NULL;
		break;
	}

	switch (pdrv->config.basic.lcd_type) {
	case LCD_RGB:
		lcd_debug_info_if = &lcd_debug_info_if_rgb;
		break;
	case LCD_BT656:
	case LCD_BT1120:
		lcd_debug_info_if = &lcd_debug_info_if_bt;
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
	case LCD_EDP:
		lcd_debug_info_if = &lcd_debug_info_if_edp;
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

	pdrv->debug_info_reg = (void *)lcd_debug_info_reg;
	pdrv->debug_info_if = (void *)lcd_debug_info_if;
}
