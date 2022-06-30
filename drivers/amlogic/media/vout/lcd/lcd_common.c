// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <dm.h>
#include <asm/gpio.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "lcd_reg.h"
#include "lcd_common.h"

struct lcd_type_match_s {
	char *name;
	enum lcd_type_e type;
};

static struct lcd_type_match_s lcd_type_match_table[] = {
	{"rgb",      LCD_RGB},
	{"lvds",     LCD_LVDS},
	{"vbyone",   LCD_VBYONE},
	{"mipi",     LCD_MIPI},
	{"minilvds", LCD_MLVDS},
	{"p2p",      LCD_P2P},
	{"edp",      LCD_EDP},
	{"bt656",    LCD_BT656},
	{"bt1120",   LCD_BT1120},
	{"invalid",  LCD_TYPE_MAX},
};

int lcd_type_str_to_type(const char *str)
{
	int type = LCD_TYPE_MAX;
	int i;

	for (i = 0; i < ARRAY_SIZE(lcd_type_match_table); i++) {
		if (!strcmp(str, lcd_type_match_table[i].name)) {
			type = lcd_type_match_table[i].type;
			break;
		}
	}
	return type;
}

char *lcd_type_type_to_str(int type)
{
	char *name = lcd_type_match_table[LCD_TYPE_MAX].name;
	int i;

	for (i = 0; i < ARRAY_SIZE(lcd_type_match_table); i++) {
		if (type == lcd_type_match_table[i].type) {
			name = lcd_type_match_table[i].name;
			break;
		}
	}
	return name;
}

static char *lcd_mode_table[] = {
	"tv",
	"tablet",
	"invalid",
};

int lcd_mode_str_to_mode(const char *str)
{
	int mode;

	for (mode = 0; mode < ARRAY_SIZE(lcd_mode_table); mode++) {
		if (!strcmp(str, lcd_mode_table[mode]))
			break;
	}
	return mode;
}

char *lcd_mode_mode_to_str(int mode)
{
	return lcd_mode_table[mode];
}

static void lcd_config_load_print(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_config_s *pconf = &pdrv->config;
	union lcd_ctrl_config_u *pctrl;

	LCDPR("[%d]: %s, %s, %dbit, %dx%d\n",
		pdrv->index,
		pconf->basic.model_name,
		lcd_type_type_to_str(pconf->basic.lcd_type),
		pconf->basic.lcd_bits,
		pconf->basic.h_active, pconf->basic.v_active);

	if ((lcd_debug_print_flag & LCD_DBG_PR_NORMAL) == 0)
		return;

	LCDPR("h_period = %d\n", pconf->basic.h_period);
	LCDPR("v_period = %d\n", pconf->basic.v_period);

	LCDPR("h_period_min = %d\n", pconf->basic.h_period_min);
	LCDPR("h_period_max = %d\n", pconf->basic.h_period_max);
	LCDPR("v_period_min = %d\n", pconf->basic.v_period_min);
	LCDPR("v_period_max = %d\n", pconf->basic.v_period_max);
	LCDPR("pclk_min = %d\n", pconf->basic.lcd_clk_min);
	LCDPR("pclk_max = %d\n", pconf->basic.lcd_clk_max);

	LCDPR("hsync_width = %d\n", pconf->timing.hsync_width);
	LCDPR("hsync_bp = %d\n", pconf->timing.hsync_bp);
	LCDPR("hsync_pol = %d\n", pconf->timing.hsync_pol);
	LCDPR("vsync_width = %d\n", pconf->timing.vsync_width);
	LCDPR("vsync_bp = %d\n", pconf->timing.vsync_bp);
	LCDPR("vsync_pol = %d\n", pconf->timing.vsync_pol);

	LCDPR("fr_adjust_type = %d\n", pconf->timing.fr_adjust_type);
	LCDPR("ss_level = %d\n", pconf->timing.ss_level);
	LCDPR("clk_auto = %d\n", pconf->timing.clk_auto);
	LCDPR("pixel_clk = %d\n", pconf->timing.lcd_clk);

	LCDPR("custom_pinmux = %d\n", pconf->custom_pinmux);

	pctrl = &pconf->control;
	if (pconf->basic.lcd_type == LCD_RGB) {
		LCDPR("type = %d\n", pctrl->rgb_cfg.type);
		LCDPR("clk_pol = %d\n", pctrl->rgb_cfg.clk_pol);
		LCDPR("de_valid = %d\n", pctrl->rgb_cfg.de_valid);
		LCDPR("sync_valid = %d\n", pctrl->rgb_cfg.sync_valid);
		LCDPR("rb_swap = %d\n", pctrl->rgb_cfg.rb_swap);
		LCDPR("bit_swap = %d\n", pctrl->rgb_cfg.bit_swap);
	} else if (pconf->basic.lcd_type == LCD_LVDS) {
		LCDPR("lvds_repack = %d\n", pctrl->lvds_cfg.lvds_repack);
		LCDPR("pn_swap = %d\n", pctrl->lvds_cfg.pn_swap);
		LCDPR("dual_port = %d\n", pctrl->lvds_cfg.dual_port);
		LCDPR("port_swap = %d\n", pctrl->lvds_cfg.port_swap);
		LCDPR("lane_reverse = %d\n", pctrl->lvds_cfg.lane_reverse);
		LCDPR("phy_vswing = 0x%x\n", pctrl->lvds_cfg.phy_vswing);
		LCDPR("phy_preem = 0x%x\n", pctrl->lvds_cfg.phy_preem);
	} else if (pconf->basic.lcd_type == LCD_VBYONE) {
		LCDPR("lane_count = %d\n", pctrl->vbyone_cfg.lane_count);
		LCDPR("byte_mode = %d\n", pctrl->vbyone_cfg.byte_mode);
		LCDPR("region_num = %d\n", pctrl->vbyone_cfg.region_num);
		LCDPR("color_fmt = %d\n", pctrl->vbyone_cfg.color_fmt);
		LCDPR("phy_vswing = 0x%x\n", pctrl->vbyone_cfg.phy_vswing);
		LCDPR("phy_preem = 0x%x\n", pctrl->vbyone_cfg.phy_preem);
	} else if (pconf->basic.lcd_type == LCD_MLVDS) {
		LCDPR("channel_num = %d\n", pctrl->mlvds_cfg.channel_num);
		LCDPR("channel_sel0 = %d\n", pctrl->mlvds_cfg.channel_sel0);
		LCDPR("channel_sel1 = %d\n", pctrl->mlvds_cfg.channel_sel1);
		LCDPR("clk_phase = %d\n", pctrl->mlvds_cfg.clk_phase);
		LCDPR("phy_vswing = 0x%x\n", pctrl->mlvds_cfg.phy_vswing);
		LCDPR("phy_preem = 0x%x\n", pctrl->mlvds_cfg.phy_preem);
	} else if (pconf->basic.lcd_type == LCD_P2P) {
		LCDPR("p2p_type = %d\n", pctrl->p2p_cfg.p2p_type);
		LCDPR("lane_num = %d\n", pctrl->p2p_cfg.lane_num);
		LCDPR("channel_sel0 = %d\n", pctrl->p2p_cfg.channel_sel0);
		LCDPR("channel_sel1 = %d\n", pctrl->p2p_cfg.channel_sel1);
		LCDPR("phy_vswing = 0x%x\n", pctrl->p2p_cfg.phy_vswing);
		LCDPR("phy_preem = 0x%x\n", pctrl->p2p_cfg.phy_preem);
	} else if (pconf->basic.lcd_type == LCD_MIPI) {
		if (pctrl->mipi_cfg.check_en) {
			LCDPR("check_reg = 0x%02x\n",
				pctrl->mipi_cfg.check_reg);
			LCDPR("check_cnt = %d\n",
				pctrl->mipi_cfg.check_cnt);
		}
		LCDPR("lane_num = %d\n", pctrl->mipi_cfg.lane_num);
		LCDPR("bit_rate_max = %d\n", pctrl->mipi_cfg.bit_rate_max);
		LCDPR("pclk_lanebyteclk_factor = %d\n", pctrl->mipi_cfg.factor_numerator);
		LCDPR("operation_mode_init = %d\n", pctrl->mipi_cfg.operation_mode_init);
		LCDPR("operation_mode_disp = %d\n", pctrl->mipi_cfg.operation_mode_display);
		LCDPR("video_mode_type = %d\n", pctrl->mipi_cfg.video_mode_type);
		LCDPR("clk_always_hs = %d\n", pctrl->mipi_cfg.clk_always_hs);
		LCDPR("phy_switch = %d\n", pctrl->mipi_cfg.phy_switch);
		LCDPR("extern_init = %d\n", pctrl->mipi_cfg.extern_init);
	} else if (pconf->basic.lcd_type == LCD_EDP) {
		LCDPR("max_lane_count      = %d\n", pctrl->edp_cfg.max_lane_count);
		LCDPR("max_link_rate       = %d\n", pctrl->edp_cfg.max_link_rate);
		LCDPR("edid_en             = %d\n", pctrl->edp_cfg.edid_en);
		LCDPR("training_mode       = %d\n", pctrl->edp_cfg.training_mode);
		LCDPR("dpcd_caps_en        = %d\n", pctrl->edp_cfg.dpcd_caps_en);
		LCDPR("sync_clk_mode       = %d\n", pctrl->edp_cfg.sync_clk_mode);

		LCDPR("lane_count          = %d\n", pctrl->edp_cfg.lane_count);
		LCDPR("link_rate           = %d\n", pctrl->edp_cfg.link_rate);
		LCDPR("bit_rate            = %d\n", pctrl->edp_cfg.bit_rate);
		LCDPR("training_settings   = %d\n", pctrl->edp_cfg.training_settings);
		LCDPR("main_stream_enable  = %d\n", pctrl->edp_cfg.main_stream_enable);

		LCDPR("phy_vswing = 0x%x\n", pctrl->edp_cfg.phy_vswing);
		LCDPR("phy_preem  = 0x%x\n", pctrl->edp_cfg.phy_preem);
	}
}

int lcd_base_config_load_from_dts(char *dt_addr, struct aml_lcd_drv_s *pdrv)
{
#ifdef CONFIG_OF_LIBFDT
	struct lcd_config_s *pconf = &pdrv->config;
	int parent_offset;
	char *propdata, *p, snode[10];
	const char *str;
	unsigned int temp;
	int i, j;

	if (pdrv->index == 0)
		sprintf(snode, "/lcd");
	else
		sprintf(snode, "/lcd%d", pdrv->index);
	parent_offset = fdt_path_offset(dt_addr, snode);
	if (parent_offset < 0) {
		LCDERR("[%d]: not find %s node: %s\n",
		       pdrv->index, snode, fdt_strerror(parent_offset));
		return -1;
	}

	/* check lcd_mode & lcd_key_valid */
	propdata = (char *)fdt_getprop(dt_addr, parent_offset, "mode", NULL);
	if (!propdata) {
		LCDERR("[%d]: failed to get mode\n", pdrv->index);
		return -1;
	}
	pdrv->mode = lcd_mode_str_to_mode(propdata);

	propdata = (char *)fdt_getprop(dt_addr, parent_offset, "key_valid", NULL);
	if (!propdata) {
		LCDERR("[%d]: failed to get key_valid\n", pdrv->index);
		pdrv->key_valid = 0;
	} else {
		pdrv->key_valid = (unsigned char)(be32_to_cpup((u32 *)propdata));
	}

	/* check lcd_clk_path */
	propdata = (char *)fdt_getprop(dt_addr, parent_offset, "clk_path", NULL);
	if (!propdata) {
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
			LCDPR("[%d]: failed to get clk_path\n", pdrv->index);
		pdrv->clk_path = 0;
	} else {
		pdrv->clk_path = (unsigned char)(be32_to_cpup((u32 *)propdata));
	}
	LCDPR("[%d]: detect mode: %s, key_valid: %d, clk_path: %d\n",
	      pdrv->index, lcd_mode_mode_to_str(pdrv->mode),
	      pdrv->key_valid, pdrv->clk_path);

	temp = env_get_ulong("lcd_clk_path", 10, 0xffff);
	if (temp != 0xffff) {
		if (temp)
			pdrv->clk_path = 1;
		else
			pdrv->clk_path = 0;
		LCDPR("[%d]: lcd_clk_path flag set clk_path: %d\n",
		      pdrv->index, pdrv->clk_path);
	}

	i = 0;
	propdata = (char *)fdt_getprop(dt_addr, parent_offset,
				       "lcd_cpu_gpio_names", NULL);
	if (!propdata) {
		LCDPR("[%d]: failed to get lcd_cpu_gpio_names\n", pdrv->index);
	} else {
		p = propdata;
		while (i < LCD_CPU_GPIO_NUM_MAX) {
			str = p;
			if (strlen(str) == 0)
				break;
			strncpy(pconf->power.cpu_gpio[i], str, (LCD_CPU_GPIO_NAME_MAX - 1));
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				LCDPR("[%d]: i=%d, gpio=%s\n",
				      pdrv->index, i, pconf->power.cpu_gpio[i]);
			}
			p += strlen(p) + 1;
			i++;
		}
	}
	for (j = i; j < LCD_CPU_GPIO_NUM_MAX; j++)
		strcpy(pconf->power.cpu_gpio[j], "invalid");

#endif
	return 0;
}

int lcd_base_config_load_from_bsp(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_dft_config_s *dft_conf;
	unsigned int temp;
	char (*lcd_gpio)[LCD_CPU_GPIO_NAME_MAX];
	int i, j;

	dft_conf = pdrv->data->dft_conf[pdrv->index];
	if (!dft_conf) {
		LCDERR("%s: dft_conf is NULL\n", __func__);
		return -1;
	}

	pdrv->mode = dft_conf->mode;
	pdrv->key_valid = dft_conf->key_valid;
	pdrv->clk_path = dft_conf->clk_path;
	LCDPR("[%d]: detect mode: %s, key_valid: %d, clk_path: %d\n",
	      pdrv->index, lcd_mode_mode_to_str(pdrv->mode),
	      pdrv->key_valid, pdrv->clk_path);

	temp = env_get_ulong("lcd_clk_path", 10, 0xffff);
	if (temp != 0xffff) {
		if (temp)
			pdrv->clk_path = 1;
		else
			pdrv->clk_path = 0;
		LCDPR("[%d]: lcd_clk_path flag set clk_path: %d\n",
		      pdrv->index, pdrv->clk_path);
	}

	i = 0;
	lcd_gpio = pdrv->data->dft_conf[pdrv->index]->lcd_gpio;
	if (!lcd_gpio) {
		LCDERR("[%d]: %s lcd_gpio is null\n", pdrv->index, __func__);
		return -1;
	}
	while (i < LCD_CPU_GPIO_NUM_MAX) {
		if (strcmp(lcd_gpio[i], "invalid") == 0)
			break;
		strcpy(pdrv->config.power.cpu_gpio[i], lcd_gpio[i]);
		i++;
	}
	for (j = i; j < LCD_CPU_GPIO_NUM_MAX; j++)
		strcpy(pdrv->config.power.cpu_gpio[j], "invalid");

	return 0;
}

static int lcd_power_load_from_dts(struct aml_lcd_drv_s *pdrv,
			    char *dt_addr, int child_offset)
{
	struct lcd_power_step_s *pstep;
	char *propdata;
	unsigned int i, j, temp;

	pstep = pdrv->config.power.power_on_step;
	propdata = (char *)fdt_getprop(dt_addr, child_offset, "power_on_step", NULL);
	if (!propdata) {
		LCDERR("[%d]: failed to get power_on_step\n", pdrv->index);
		return 0;
	} else {
		i = 0;
		while (i < LCD_PWR_STEP_MAX) {
			j = 4 * i;
			temp = be32_to_cpup((((u32*)propdata) + j));
			pstep[i].type = temp;
			if (temp == 0xff)
				break;
			temp = be32_to_cpup((((u32*)propdata) + j + 1));
			pstep[i].index = temp;
			temp = be32_to_cpup((((u32*)propdata) + j + 2));
			pstep[i].value = temp;
			temp = be32_to_cpup((((u32*)propdata) + j + 3));
			pstep[i].delay = temp;
			if (pstep[i].type == LCD_POWER_TYPE_CLK_SS) {
				temp = pstep[i].value;
				pdrv->config.timing.ss_level |= temp << 8;
				if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
					LCDPR("[%d]: clk_ss value=0x%x, final ss_level=0x%x\n",
					      pdrv->index, temp, pdrv->config.timing.ss_level);
				}
			} else if (pstep[i].type == LCD_POWER_TYPE_EXTERN) {
#ifdef CONFIG_AML_LCD_EXTERN
				lcd_extern_drv_index_add(pdrv->index, pstep[i].index);
#endif
			}
			i++;
		}
	}

	pstep = pdrv->config.power.power_off_step;
	propdata = (char *)fdt_getprop(dt_addr, child_offset, "power_off_step", NULL);
	if (!propdata) {
		LCDERR("[%d]: failed to get power_off_step\n", pdrv->index);
		return 0;
	} else {
		i = 0;
		while (i < LCD_PWR_STEP_MAX) {
			j = 4 * i;
			temp = be32_to_cpup((((u32*)propdata) + j));
			pstep[i].type = temp;
			if (temp == 0xff)
				break;
			temp = be32_to_cpup((((u32*)propdata) + j + 1));
			pstep[i].index = temp;
			temp = be32_to_cpup((((u32*)propdata) + j + 2));
			pstep[i].value = temp;
			temp = be32_to_cpup((((u32*)propdata) + j + 3));
			pstep[i].delay = temp;
			if (pstep[i].type == LCD_POWER_TYPE_EXTERN) {
#ifdef CONFIG_AML_LCD_EXTERN
				lcd_extern_drv_index_add(pdrv->index, pstep[i].index);
#endif
			}
			i++;
		}
	}

	return 0;
}

static int lcd_power_load_from_unifykey(struct aml_lcd_drv_s *pdrv,
					unsigned char *buf, int key_len, int len)
{
	struct lcd_power_step_s *pstep;
	int i, j, temp;
	unsigned char *p;
	int ret = 0;

	/* power: (5byte * n) */
	pstep = pdrv->config.power.power_on_step;
	p = buf + len;
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: power_on step:\n", pdrv->index);
	i = 0;
	while (i < LCD_PWR_STEP_MAX) {
		len += 5;
		ret = lcd_unifykey_len_check(key_len, len);
		if (ret) {
			pstep[i].type = 0xff;
			pstep[i].index = 0;
			pstep[i].value = 0;
			pstep[i].delay = 0;
			LCDERR("unifykey power_on length is incorrect\n");
			return -1;
		}
		pstep[i].type = *(p + LCD_UKEY_PWR_TYPE + 5 * i);
		pstep[i].index = *(p + LCD_UKEY_PWR_INDEX + 5 * i);
		pstep[i].value = *(p + LCD_UKEY_PWR_VAL + 5 * i);
		pstep[i].delay = (*(p + LCD_UKEY_PWR_DELAY + 5 * i) |
				  ((*(p + LCD_UKEY_PWR_DELAY + 5 * i + 1)) << 8));

		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
			LCDPR("step %d: type=%d, index=%d, value=%d, delay=%d\n",
				i, pstep[i].type, pstep[i].index,
				pstep[i].value, pstep[i].delay);
		}
		if (pstep[i].type >= LCD_POWER_TYPE_MAX)
			break;

		if (pstep[i].type == LCD_POWER_TYPE_CLK_SS) {
			temp = pstep[i].value;
			pdrv->config.timing.ss_level |= temp << 8;
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				LCDPR("[%d]: clk_ss value=0x%x, final ss_level=0x%x\n",
				      pdrv->index, temp, pdrv->config.timing.ss_level);
			}
		} else if (pstep[i].type == LCD_POWER_TYPE_EXTERN) {
#ifdef CONFIG_AML_LCD_EXTERN
			lcd_extern_drv_index_add(pdrv->index, pstep[i].index);
#endif
		}
		i++;
	}

	pstep = pdrv->config.power.power_off_step;
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: power_off step:\n", pdrv->index);
	p += (5 * (i + 1));
	j = 0;
	while (j < LCD_PWR_STEP_MAX) {
		len += 5;
		ret = lcd_unifykey_len_check(key_len, len);
		if (ret) {
			pstep[j].type = 0xff;
			pstep[j].index = 0;
			pstep[j].value = 0;
			pstep[j].delay = 0;
			LCDERR("unifykey power_off length is incorrect\n");
			return -1;
		}
		pstep[j].type = *(p + LCD_UKEY_PWR_TYPE + 5 * j);
		pstep[j].index = *(p + LCD_UKEY_PWR_INDEX + 5 * j);
		pstep[j].value = *(p + LCD_UKEY_PWR_VAL + 5 * j);
		pstep[j].delay = (*(p + LCD_UKEY_PWR_DELAY + 5 * j) |
				  ((*(p + LCD_UKEY_PWR_DELAY + 5 * j + 1)) << 8));

		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
			LCDPR("step %d: type=%d, index=%d, value=%d, delay=%d\n",
				j, pstep[j].type, pstep[j].index,
				pstep[j].value, pstep[j].delay);
		}
		if (pstep[j].type >= LCD_POWER_TYPE_MAX)
			break;

		if (pstep[j].type == LCD_POWER_TYPE_EXTERN) {
#ifdef CONFIG_AML_LCD_EXTERN
			lcd_extern_drv_index_add(pdrv->index, pstep[j].index);
#endif
		}
		j++;
	}

	return ret;
}

static char *lcd_rgb_pinmux_str[] = {
	"lcd_rgb_on_pin",         /* 0 */
	"lcd_rgb_de_on_pin",      /* 1 */
	"lcd_rgb_sync_on_pin"     /* 2 */
};

static int lcd_pinmux_load_rgb(struct lcd_pinmux_ctrl_s *pinmux, struct lcd_config_s *pconf)
{
	char propname[30];
	int pinmux_index = 0, set_cnt = 0, clr_cnt = 0;
	unsigned int i, j;

	/* data */
	pinmux_index = 0;
	sprintf(propname, "%s", lcd_rgb_pinmux_str[pinmux_index]);
	for (i = 0; i < LCD_PINMX_MAX; i++) {
		if (!pinmux)
			break;
		if (!pinmux->name)
			break;
		if (strncmp(pinmux->name, "invalid", 7) == 0)
			break;
		if (strncmp(pinmux->name, propname,
				strlen(propname)) == 0) {
			for (j = 0; j < LCD_PINMUX_NUM; j++) {
				if (pinmux->pinmux_set[j][0] == LCD_PINMUX_END)
					break;
				pconf->pinmux_set[j][0] = pinmux->pinmux_set[j][0];
				pconf->pinmux_set[j][1] = pinmux->pinmux_set[j][1];
				set_cnt++;
			}
			for (j = 0; j < LCD_PINMUX_NUM; j++) {
				if (pinmux->pinmux_clr[j][0] == LCD_PINMUX_END)
					break;
				pconf->pinmux_clr[j][0] = pinmux->pinmux_clr[j][0];
				pconf->pinmux_clr[j][1] = pinmux->pinmux_clr[j][1];
				clr_cnt++;
			}
			break;
		}
		pinmux++;
	}

	/* DE */
	if (pconf->control.rgb_cfg.de_valid) {
		pinmux_index = 1;
		sprintf(propname, "%s", lcd_rgb_pinmux_str[pinmux_index]);
		for (i = 0; i < LCD_PINMX_MAX; i++) {
			if (!pinmux)
				break;
			if (!pinmux->name)
				break;
			if (strncmp(pinmux->name, "invalid", 7) == 0)
				break;
			if (strncmp(pinmux->name, propname, strlen(propname)) == 0) {
				for (j = 0; j < LCD_PINMUX_NUM; j++) {
					if (pinmux->pinmux_set[j][0] == LCD_PINMUX_END)
						break;
					pconf->pinmux_set[j][0] = pinmux->pinmux_set[j][0];
					pconf->pinmux_set[j][1] = pinmux->pinmux_set[j][1];
					set_cnt++;
				}
				for (j = 0; j < LCD_PINMUX_NUM; j++) {
					if (pinmux->pinmux_clr[j][0] == LCD_PINMUX_END)
						break;
					pconf->pinmux_clr[j][0] = pinmux->pinmux_clr[j][0];
					pconf->pinmux_clr[j][1] = pinmux->pinmux_clr[j][1];
					clr_cnt++;
				}
				break;
			}
			pinmux++;
		}
	}

	/* sync */
	if (pconf->control.rgb_cfg.sync_valid) {
		pinmux_index = 2;
		sprintf(propname, "%s", lcd_rgb_pinmux_str[pinmux_index]);
		for (i = 0; i < LCD_PINMX_MAX; i++) {
			if (!pinmux)
				break;
			if (!pinmux->name)
				break;
			if (strncmp(pinmux->name, "invalid", 7) == 0)
				break;
			if (strncmp(pinmux->name, propname, strlen(propname)) == 0) {
				for (j = 0; j < LCD_PINMUX_NUM; j++) {
					if (pinmux->pinmux_set[j][0] == LCD_PINMUX_END)
						break;
					pconf->pinmux_set[j][0] = pinmux->pinmux_set[j][0];
					pconf->pinmux_set[j][1] = pinmux->pinmux_set[j][1];
					set_cnt++;
				}
				for (j = 0; j < LCD_PINMUX_NUM; j++) {
					if (pinmux->pinmux_clr[j][0] == LCD_PINMUX_END)
						break;
					pconf->pinmux_clr[j][0] = pinmux->pinmux_clr[j][0];
					pconf->pinmux_clr[j][1] = pinmux->pinmux_clr[j][1];
					clr_cnt++;
				}
				break;
			}
			pinmux++;
		}
	}

	if (set_cnt < LCD_PINMUX_NUM) {
		pconf->pinmux_set[set_cnt][0] = LCD_PINMUX_END;
		pconf->pinmux_set[set_cnt][1] = 0x0;
	}
	if (clr_cnt < LCD_PINMUX_NUM) {
		pconf->pinmux_clr[clr_cnt][0] = LCD_PINMUX_END;
		pconf->pinmux_clr[clr_cnt][1] = 0x0;
	}

	return 0;
}

static int lcd_custom_pinmux_load_config(struct lcd_pinmux_ctrl_s *pinmux,
		struct lcd_config_s *pconf)
{
	char propname[35];
	int set_cnt = 0, clr_cnt = 0;
	int i, j;

	sprintf(propname, "%s", pconf->basic.model_name);
	for (i = 0; i < LCD_PINMX_MAX; i++) {
		if (!pinmux)
			break;
		if (!pinmux->name)
			break;
		if (strncmp(pinmux->name, "invalid", 7) == 0)
			break;
		if (strncmp(pinmux->name, propname, strlen(propname)) == 0) {
			for (j = 0; j < LCD_PINMUX_NUM; j++) {
				if (pinmux->pinmux_set[j][0] == LCD_PINMUX_END)
					break;
				pconf->pinmux_set[j][0] = pinmux->pinmux_set[j][0];
				pconf->pinmux_set[j][1] = pinmux->pinmux_set[j][1];
				set_cnt++;
			}
			for (j = 0; j < LCD_PINMUX_NUM; j++) {
				if (pinmux->pinmux_clr[j][0] == LCD_PINMUX_END)
					break;
				pconf->pinmux_clr[j][0] = pinmux->pinmux_clr[j][0];
				pconf->pinmux_clr[j][1] = pinmux->pinmux_clr[j][1];
				clr_cnt++;
			}
			break;
		}
		pinmux++;
	}

	if (set_cnt < LCD_PINMUX_NUM) {
		pconf->pinmux_set[set_cnt][0] = LCD_PINMUX_END;
		pconf->pinmux_set[set_cnt][1] = 0x0;
	}
	if (clr_cnt < LCD_PINMUX_NUM) {
		pconf->pinmux_clr[clr_cnt][0] = LCD_PINMUX_END;
		pconf->pinmux_clr[clr_cnt][1] = 0x0;
	}
	return 0;
}

static int lcd_pinmux_load_config(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_pinmux_ctrl_s *pinmux;
	struct lcd_config_s *pconf = &pdrv->config;
	char propname[30];
	int set_cnt = 0, clr_cnt = 0;
	unsigned int i, j;
	int ret = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	if (!pdrv->data->dft_conf[pdrv->index]) {
		LCDERR("[%d]: %s: dft_conf is NULL\n", pdrv->index, __func__);
		return -1;
	}
	pinmux = pdrv->data->dft_conf[pdrv->index]->lcd_pinmux;
	if (!pinmux) {
		LCDERR("[%d]: %s: lcd_pinmux is NULL\n", pdrv->index, __func__);
		return -1;
	}

	if (pconf->basic.lcd_type == LCD_RGB) {
		ret = lcd_pinmux_load_rgb(pinmux, pconf);
		if (ret)
			return -1;
		goto lcd_pinmux_load_config_next;
	}

	if (pconf->custom_pinmux) {
		ret = lcd_custom_pinmux_load_config(pinmux, pconf);
		if (ret)
			return -1;
		goto lcd_pinmux_load_config_next;
	}

	switch (pconf->basic.lcd_type) {
	case LCD_VBYONE:
		sprintf(propname, "lcd_vbyone_pin");
		break;
	case LCD_MLVDS:
		sprintf(propname, "lcd_minilvds_pin");
		break;
	case LCD_P2P:
		if (pconf->control.p2p_cfg.p2p_type == P2P_USIT)
			sprintf(propname, "lcd_p2p_usit_pin");
		else
			sprintf(propname, "lcd_p2p_pin");
		break;
	case LCD_EDP:
		sprintf(propname, "lcd_edp_pin");
		break;
	default:
		pconf->pinmux_set[0][0] = LCD_PINMUX_END;
		pconf->pinmux_set[0][1] = 0x0;
		pconf->pinmux_clr[0][0] = LCD_PINMUX_END;
		pconf->pinmux_clr[0][1] = 0x0;
		return 0;
	}
	for (i = 0; i < LCD_PINMX_MAX; i++) {
		if (!pinmux->name)
			break;
		if (strncmp(pinmux->name, "invalid", 7) == 0)
			break;
		if (strncmp(pinmux->name, propname, strlen(propname)) == 0) {
			for (j = 0; j < LCD_PINMUX_NUM; j++) {
				if (pinmux->pinmux_set[j][0] == LCD_PINMUX_END)
					break;
				pconf->pinmux_set[j][0] = pinmux->pinmux_set[j][0];
				pconf->pinmux_set[j][1] = pinmux->pinmux_set[j][1];
				set_cnt++;
			}
			for (j = 0; j < LCD_PINMUX_NUM; j++) {
				if (pinmux->pinmux_clr[j][0] == LCD_PINMUX_END)
					break;
				pconf->pinmux_clr[j][0] = pinmux->pinmux_clr[j][0];
				pconf->pinmux_clr[j][1] = pinmux->pinmux_clr[j][1];
				clr_cnt++;
			}
			break;
		}
		pinmux++;
	}
	if (set_cnt < LCD_PINMUX_NUM) {
		pconf->pinmux_set[set_cnt][0] = LCD_PINMUX_END;
		pconf->pinmux_set[set_cnt][1] = 0x0;
	}
	if (clr_cnt < LCD_PINMUX_NUM) {
		pconf->pinmux_clr[clr_cnt][0] = LCD_PINMUX_END;
		pconf->pinmux_clr[clr_cnt][1] = 0x0;
	}

lcd_pinmux_load_config_next:
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (pdrv->config.pinmux_set[i][0] == LCD_PINMUX_END)
				break;
			LCDPR("pinmux_set: %d, 0x%08x\n",
				pdrv->config.pinmux_set[i][0],
				pdrv->config.pinmux_set[i][1]);
			i++;
		}
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (pdrv->config.pinmux_clr[i][0] == LCD_PINMUX_END)
				break;
			LCDPR("pinmux_clr: %d, 0x%08x\n",
				pdrv->config.pinmux_clr[i][0],
				pdrv->config.pinmux_clr[i][1]);
			i++;
		}
	}

	return 0;
}

static int lcd_config_load_from_dts(char *dt_addr, struct aml_lcd_drv_s *pdrv)
{
#ifdef CONFIG_OF_LIBFDT
	struct lcd_config_s *pconf = &pdrv->config;
	union lcd_ctrl_config_u *pctrl = &pdrv->config.control;
	struct phy_config_s *phy_cfg = &pdrv->config.phy_cfg;
	int parent_offset;
	int child_offset;
	char parent_str[10], type_str[20], propname[30];
	char *propdata;
	unsigned int temp;
	int i, len;

	LCDPR("config load from dts\n");

	if (pdrv->index == 0) {
		sprintf(parent_str, "/lcd");
		sprintf(type_str, "panel_type");
	} else {
		sprintf(parent_str, "/lcd%d", pdrv->index);
		sprintf(type_str, "panel%d_type", pdrv->index);
	}
	parent_offset = fdt_path_offset(dt_addr, parent_str);
	if (parent_offset < 0) {
		LCDERR("not find %s node: %s\n",
			parent_str, fdt_strerror(parent_offset));
		return -1;
	}

	/* check panel_type */
	char *panel_type = env_get(type_str);
	if (!panel_type) {
		LCDERR("[%d]: no %s\n", pdrv->index, type_str);
		return -1;
	}
	LCDPR("[%d]: use %s=%s\n", pdrv->index, type_str, panel_type);

	snprintf(propname, 30, "%s/%s", parent_str, panel_type);
	child_offset = fdt_path_offset(dt_addr, propname);
	if (child_offset < 0) {
		LCDERR("[%d]: not find %s node: %s\n",
			pdrv->index, propname, fdt_strerror(child_offset));
		return -1;
	}

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "model_name", NULL);
	if (!propdata) {
		LCDERR("[%d]: failed to get model_name\n", pdrv->index);
		strncpy(pconf->basic.model_name, panel_type,
			sizeof(pconf->basic.model_name) - 1);
	} else {
		strncpy(pconf->basic.model_name, propdata,
			sizeof(pconf->basic.model_name) - 1);
	}
	pconf->basic.model_name[sizeof(pconf->basic.model_name) - 1] = '\0';

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "interface", NULL);
	if (!propdata) {
		LCDERR("[%d]: failed to get interface\n", pdrv->index);
		return -1;
	}
	pconf->basic.lcd_type = lcd_type_str_to_type(propdata);
	LCDPR("load dts config: %s, lcd_type: %s(%d)\n",
		pconf->basic.model_name, propdata, pconf->basic.lcd_type);

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "basic_setting", NULL);
	if (!propdata) {
		LCDERR("[%d]: failed to get basic_setting\n", pdrv->index);
		return -1;
	}
	pconf->basic.h_active = be32_to_cpup((u32 *)propdata);
	pconf->basic.v_active = be32_to_cpup((((u32 *)propdata) + 1));
	pconf->basic.h_period = be32_to_cpup((((u32 *)propdata) + 2));
	pconf->basic.v_period = be32_to_cpup((((u32 *)propdata) + 3));
	pconf->basic.lcd_bits = be32_to_cpup((((u32 *)propdata) + 4));
	pconf->basic.screen_width = be32_to_cpup((((u32 *)propdata) + 5));
	pconf->basic.screen_height = be32_to_cpup((((u32 *)propdata) + 6));

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "range_setting", NULL);
	if (!propdata) {
		pconf->basic.h_period_min = pconf->basic.h_period;
		pconf->basic.h_period_max = pconf->basic.h_period;
		pconf->basic.v_period_min = pconf->basic.v_period;
		pconf->basic.v_period_max = pconf->basic.v_period;
		pconf->basic.lcd_clk_min = 0;
		pconf->basic.lcd_clk_max = 0;
	} else {
		pconf->basic.h_period_min = be32_to_cpup((u32 *)propdata);
		pconf->basic.h_period_max = be32_to_cpup((((u32 *)propdata) + 1));
		pconf->basic.v_period_min = be32_to_cpup((((u32 *)propdata) + 2));
		pconf->basic.v_period_max = be32_to_cpup((((u32 *)propdata) + 3));
		pconf->basic.lcd_clk_min = be32_to_cpup((((u32 *)propdata) + 4));
		pconf->basic.lcd_clk_max = be32_to_cpup((((u32 *)propdata) + 5));
	}

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "range_frame_rate", NULL);
	if (!propdata) {
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
			LCDPR("[%d]: no range_frame_rate\n", pdrv->index);
		pconf->basic.frame_rate_min = 0;
		pconf->basic.frame_rate_max = 0;
	} else {
		pconf->basic.frame_rate_min = be32_to_cpup((u32 *)propdata);
		pconf->basic.frame_rate_max = be32_to_cpup((((u32 *)propdata) + 1));
	}

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "lcd_timing", NULL);
	if (!propdata) {
		LCDERR("[%d]: failed to get lcd_timing\n", pdrv->index);
		return -1;
	}
	pconf->timing.hsync_width = (unsigned short)(be32_to_cpup((u32 *)propdata));
	pconf->timing.hsync_bp    = (unsigned short)(be32_to_cpup((((u32 *)propdata) + 1)));
	pconf->timing.hsync_pol   = (unsigned short)(be32_to_cpup((((u32 *)propdata) + 2)));
	pconf->timing.vsync_width = (unsigned short)(be32_to_cpup((((u32 *)propdata) + 3)));
	pconf->timing.vsync_bp    = (unsigned short)(be32_to_cpup((((u32 *)propdata) + 4)));
	pconf->timing.vsync_pol   = (unsigned short)(be32_to_cpup((((u32 *)propdata) + 5)));

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "clk_attr", NULL);
	if (!propdata) {
		LCDERR("[%d]: failed to get clk_attr\n", pdrv->index);
		pconf->timing.fr_adjust_type = 0xff;
		pconf->timing.ss_level = 0;
		pconf->timing.clk_auto = 1;
		pconf->timing.lcd_clk = 60;
	} else {
		pconf->timing.fr_adjust_type = (unsigned char)(be32_to_cpup((u32 *)propdata));
		pconf->timing.ss_level = be32_to_cpup((((u32 *)propdata) + 1));
		pconf->timing.clk_auto = (unsigned char)(be32_to_cpup((((u32 *)propdata) + 2)));
		temp = be32_to_cpup((((u32 *)propdata) + 3));
		if (temp > 0) {
			pconf->timing.lcd_clk = temp;
		} else {
			pconf->timing.lcd_clk = 60;
			LCDPR("[%d]: lcd_clk is 0, default to 60Hz\n", pdrv->index);
		}
	}

	switch (pconf->basic.lcd_type) {
	case LCD_RGB:
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "rgb_attr", NULL);
		if (!propdata) {
			LCDERR("[%d]: failed to get rgb_attr\n", pdrv->index);
			return -1;
		}
		pctrl->rgb_cfg.type = be32_to_cpup((u32 *)propdata);
		pctrl->rgb_cfg.clk_pol = be32_to_cpup((((u32 *)propdata) + 1));
		pctrl->rgb_cfg.de_valid = be32_to_cpup((((u32 *)propdata) + 2));
		pctrl->rgb_cfg.sync_valid = be32_to_cpup((((u32 *)propdata) + 3));
		pctrl->rgb_cfg.rb_swap = be32_to_cpup((((u32 *)propdata) + 4));
		pctrl->rgb_cfg.bit_swap = be32_to_cpup((((u32 *)propdata) + 5));
		break;
	case LCD_BT656:
	case LCD_BT1120:
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "bt_attr", NULL);
		if (!propdata) {
			LCDERR("[%d]: failed to get bt_attr\n", pdrv->index);
			return -1;
		}
		pctrl->bt_cfg.clk_phase = be32_to_cpup((u32 *)propdata);
		pctrl->bt_cfg.field_type = be32_to_cpup((((u32 *)propdata) + 1));
		pctrl->bt_cfg.mode_422 = be32_to_cpup((((u32 *)propdata) + 2));
		pctrl->bt_cfg.yc_swap = be32_to_cpup((((u32 *)propdata) + 3));
		pctrl->bt_cfg.cbcr_swap = be32_to_cpup((((u32 *)propdata) + 4));
		break;
	case LCD_LVDS:
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "lvds_attr", &len);
		if (!propdata) {
			LCDERR("[%d]: failed to get lvds_attr\n", pdrv->index);
			return -1;
		}
		len = len / 4;
		if (len == 5) {
			pctrl->lvds_cfg.lvds_repack = be32_to_cpup((u32 *)propdata);
			pctrl->lvds_cfg.dual_port   = be32_to_cpup((((u32 *)propdata) + 1));
			pctrl->lvds_cfg.pn_swap     = be32_to_cpup((((u32 *)propdata) + 2));
			pctrl->lvds_cfg.port_swap   = be32_to_cpup((((u32 *)propdata) + 3));
			pctrl->lvds_cfg.lane_reverse = be32_to_cpup((((u32 *)propdata) + 4));
		} else if (len == 4) {
			pctrl->lvds_cfg.lvds_repack = be32_to_cpup((u32 *)propdata);
			pctrl->lvds_cfg.dual_port   = be32_to_cpup((((u32 *)propdata) + 1));
			pctrl->lvds_cfg.pn_swap     = be32_to_cpup((((u32 *)propdata) + 2));
			pctrl->lvds_cfg.port_swap   = be32_to_cpup((((u32 *)propdata) + 3));
			pctrl->lvds_cfg.lane_reverse = 0;
		} else {
			LCDERR("[%d]: invalid lvds_attr parameters cnt: %d\n",
			       pdrv->index, len);
			return -1;
		}

		propdata = (char *)fdt_getprop(dt_addr, child_offset, "phy_attr", &len);
		if (!propdata) {
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
				LCDPR("[%d]: failed to get phy_attr\n", pdrv->index);
			pctrl->lvds_cfg.phy_vswing = LVDS_PHY_VSWING_DFT;
			pctrl->lvds_cfg.phy_preem  = LVDS_PHY_PREEM_DFT;
		} else {
			pctrl->lvds_cfg.phy_vswing = be32_to_cpup((u32 *)propdata);
			pctrl->lvds_cfg.phy_preem  = be32_to_cpup((((u32 *)propdata) + 1));
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				LCDPR("[%d]: set phy vswing_level=0x%x, preem_level=0x%x\n",
					pdrv->index,
					pctrl->lvds_cfg.phy_vswing,
					pctrl->lvds_cfg.phy_preem);
			}
		}
		phy_cfg->lane_num = 12;
		phy_cfg->vswing_level = pctrl->lvds_cfg.phy_vswing & 0xf;
		phy_cfg->ext_pullup = (pctrl->lvds_cfg.phy_vswing >> 4) & 0x3;
		phy_cfg->vswing = lcd_phy_vswing_level_to_value(pdrv, phy_cfg->vswing_level);
		phy_cfg->preem_level = pctrl->lvds_cfg.phy_preem;
		temp = lcd_phy_preem_level_to_value(pdrv, phy_cfg->preem_level);
		for (i = 0; i < phy_cfg->lane_num; i++) {
			phy_cfg->lane[i].amp = 0;
			phy_cfg->lane[i].preem = temp;
		}
		break;
	case LCD_VBYONE:
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "vbyone_attr", NULL);
		if (!propdata) {
			LCDERR("[%d]: failed to get vbyone_attr\n", pdrv->index);
			return -1;
		}
		pctrl->vbyone_cfg.lane_count = be32_to_cpup((u32 *)propdata);
		pctrl->vbyone_cfg.region_num = be32_to_cpup((((u32 *)propdata) + 1));
		pctrl->vbyone_cfg.byte_mode  = be32_to_cpup((((u32 *)propdata) + 2));
		pctrl->vbyone_cfg.color_fmt  = be32_to_cpup((((u32 *)propdata) + 3));

		propdata = (char *)fdt_getprop(dt_addr, child_offset, "phy_attr", NULL);
		if (!propdata) {
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
				LCDPR("[%d]: failed to get phy_attr\n", pdrv->index);
			pctrl->vbyone_cfg.phy_vswing = VX1_PHY_VSWING_DFT;
			pctrl->vbyone_cfg.phy_preem  = VX1_PHY_PREEM_DFT;
		} else {
			pctrl->vbyone_cfg.phy_vswing = be32_to_cpup((u32 *)propdata);
			pctrl->vbyone_cfg.phy_preem  = be32_to_cpup((((u32 *)propdata) + 1));
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				LCDPR("set phy vswing_level=0x%x, preem_level=0x%x\n",
					pctrl->vbyone_cfg.phy_vswing,
					pctrl->vbyone_cfg.phy_preem);
			}
		}
		phy_cfg->lane_num = 8;
		phy_cfg->vswing_level = pctrl->vbyone_cfg.phy_vswing & 0xf;
		phy_cfg->ext_pullup = (pctrl->vbyone_cfg.phy_vswing >> 4) & 0x3;
		phy_cfg->vswing = lcd_phy_vswing_level_to_value(pdrv, phy_cfg->vswing_level);
		phy_cfg->preem_level = pctrl->vbyone_cfg.phy_preem;
		temp = lcd_phy_preem_level_to_value(pdrv, phy_cfg->preem_level);
		for (i = 0; i < phy_cfg->lane_num; i++) {
			phy_cfg->lane[i].amp = 0;
			phy_cfg->lane[i].preem = temp;
		}

		propdata = (char *)fdt_getprop(dt_addr, child_offset, "vbyone_ctrl_flag", NULL);
		if (!propdata) {
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
				LCDPR("[%d]: failed to get vbyone_ctrl_flag\n", pdrv->index);
			pctrl->vbyone_cfg.ctrl_flag = 0;
			pctrl->vbyone_cfg.power_on_reset_delay = VX1_PWR_ON_RESET_DLY_DFT;
			pctrl->vbyone_cfg.hpd_data_delay = VX1_HPD_DATA_DELAY_DFT;
			pctrl->vbyone_cfg.cdr_training_hold = VX1_CDR_TRAINING_HOLD_DFT;
		} else {
			pctrl->vbyone_cfg.ctrl_flag = be32_to_cpup((u32 *)propdata);
			LCDPR("vbyone ctrl_flag=0x%x\n", pctrl->vbyone_cfg.ctrl_flag);
		}
		if (pctrl->vbyone_cfg.ctrl_flag & 0x7) {
			propdata = (char *)fdt_getprop(dt_addr, child_offset,
						"vbyone_ctrl_timing", NULL);
			if (!propdata) {
				LCDPR("[%d]: failed to get vbyone_ctrl_timing\n", pdrv->index);
				pctrl->vbyone_cfg.power_on_reset_delay = VX1_PWR_ON_RESET_DLY_DFT;
				pctrl->vbyone_cfg.hpd_data_delay = VX1_HPD_DATA_DELAY_DFT;
				pctrl->vbyone_cfg.cdr_training_hold = VX1_CDR_TRAINING_HOLD_DFT;
			} else {
				pctrl->vbyone_cfg.power_on_reset_delay =
					be32_to_cpup((u32 *)propdata);
				pctrl->vbyone_cfg.hpd_data_delay =
					be32_to_cpup((((u32 *)propdata) + 1));
				pctrl->vbyone_cfg.cdr_training_hold =
					be32_to_cpup((((u32 *)propdata) + 2));
			}
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				LCDPR("[%d]: power_on_reset_delay: %d\n",
				      pdrv->index,
				      pctrl->vbyone_cfg.power_on_reset_delay);
				LCDPR("[%d]: hpd_data_delay: %d\n",
				      pdrv->index,
				      pctrl->vbyone_cfg.hpd_data_delay);
				LCDPR("[%d]: cdr_training_hold: %d\n",
				      pdrv->index,
				      pctrl->vbyone_cfg.cdr_training_hold);
			}
		}
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "hw_filter", NULL);
		if (!propdata) {
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
				LCDPR("[%d]: failed to get hw_filter\n", pdrv->index);
			pctrl->vbyone_cfg.hw_filter_time = 0;
			pctrl->vbyone_cfg.hw_filter_cnt = 0;
		} else {
			pctrl->vbyone_cfg.hw_filter_time = be32_to_cpup((u32 *)propdata);
			pctrl->vbyone_cfg.hw_filter_cnt = be32_to_cpup((((u32 *)propdata) + 1));
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				LCDPR("[%d]: vbyone hw_filter=0x%x 0x%x\n",
					pdrv->index,
					pctrl->vbyone_cfg.hw_filter_time,
					pctrl->vbyone_cfg.hw_filter_cnt);
			}
		}
		break;
	case LCD_MLVDS:
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "minilvds_attr", &len);
		if (!propdata) {
			LCDERR("[%d]: failed to get minilvds_attr\n", pdrv->index);
			return -1;
		}
		pctrl->mlvds_cfg.channel_num  = be32_to_cpup((u32 *)propdata);
		pctrl->mlvds_cfg.channel_sel0 = be32_to_cpup((((u32 *)propdata) + 1));
		pctrl->mlvds_cfg.channel_sel1 = be32_to_cpup((((u32 *)propdata) + 2));
		pctrl->mlvds_cfg.clk_phase    = be32_to_cpup((((u32 *)propdata) + 3));
		pctrl->mlvds_cfg.pn_swap      = be32_to_cpup((((u32 *)propdata) + 4));
		pctrl->mlvds_cfg.bit_swap     = be32_to_cpup((((u32 *)propdata) + 5));

		propdata = (char *)fdt_getprop(dt_addr, child_offset, "phy_attr", &len);
		if (!propdata) {
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
				LCDPR("[%d]: failed to get phy_attr\n", pdrv->index);
			pctrl->mlvds_cfg.phy_vswing = LVDS_PHY_VSWING_DFT;
			pctrl->mlvds_cfg.phy_preem  = LVDS_PHY_PREEM_DFT;
		} else {
			pctrl->mlvds_cfg.phy_vswing = be32_to_cpup((u32 *)propdata);
			pctrl->mlvds_cfg.phy_preem  = be32_to_cpup((((u32 *)propdata) + 1));
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				LCDPR("[%d]: set phy vswing=0x%x, preem=0x%x\n",
				      pdrv->index,
				      pctrl->mlvds_cfg.phy_vswing,
				      pctrl->mlvds_cfg.phy_preem);
			}
		}
		phy_cfg->lane_num = 12;
		phy_cfg->vswing_level = pctrl->mlvds_cfg.phy_vswing & 0xf;
		phy_cfg->ext_pullup = (pctrl->mlvds_cfg.phy_vswing >> 4) & 0x3;
		phy_cfg->vswing = lcd_phy_vswing_level_to_value(pdrv, phy_cfg->vswing_level);
		phy_cfg->preem_level = pctrl->mlvds_cfg.phy_preem;
		temp = lcd_phy_preem_level_to_value(pdrv, phy_cfg->preem_level);
		for (i = 0; i < phy_cfg->lane_num; i++) {
			phy_cfg->lane[i].amp = 0;
			phy_cfg->lane[i].preem = temp;
		}
		break;
	case LCD_P2P:
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "p2p_attr", NULL);
		if (!propdata) {
			LCDERR("[%d]: failed to get p2p_attr\n", pdrv->index);
			return -1;
		}
		pctrl->p2p_cfg.p2p_type = be32_to_cpup((u32 *)propdata);
		pctrl->p2p_cfg.lane_num = be32_to_cpup((((u32 *)propdata) + 1));
		pctrl->p2p_cfg.channel_sel0  = be32_to_cpup((((u32 *)propdata) + 2));
		pctrl->p2p_cfg.channel_sel1  = be32_to_cpup((((u32 *)propdata) + 3));
		pctrl->p2p_cfg.pn_swap  = be32_to_cpup((((u32 *)propdata) + 4));
		pctrl->p2p_cfg.bit_swap  = be32_to_cpup((((u32 *)propdata) + 5));

		propdata = (char *)fdt_getprop(dt_addr, child_offset, "phy_attr", NULL);
		if (!propdata) {
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
				LCDPR("[%d]: failed to get phy_attr\n", pdrv->index);
			pctrl->p2p_cfg.phy_vswing = 0x5;
			pctrl->p2p_cfg.phy_preem  = 0x1;
		} else {
			pctrl->p2p_cfg.phy_vswing = be32_to_cpup((u32 *)propdata);
			pctrl->p2p_cfg.phy_preem  = be32_to_cpup((((u32 *)propdata) + 1));
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				LCDPR("[%d]: set phy vswing=0x%x, preem=0x%x\n",
				      pdrv->index,
				      pctrl->p2p_cfg.phy_vswing,
				      pctrl->p2p_cfg.phy_preem);
			}
		}
		phy_cfg->lane_num = 12;
		phy_cfg->vswing_level = pctrl->p2p_cfg.phy_vswing & 0xf;
		phy_cfg->ext_pullup = (pctrl->p2p_cfg.phy_vswing >> 4) & 0x3;
		phy_cfg->vswing = lcd_phy_vswing_level_to_value(pdrv, phy_cfg->vswing_level);
		phy_cfg->preem_level = pctrl->p2p_cfg.phy_preem;
		temp = lcd_phy_preem_level_to_value(pdrv, phy_cfg->preem_level);
		for (i = 0; i < phy_cfg->lane_num; i++) {
			phy_cfg->lane[i].amp = 0;
			phy_cfg->lane[i].preem = temp;
		}
		break;
	case LCD_MIPI:
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "mipi_attr", NULL);
		if (!propdata) {
			LCDERR("[%d]: failed to get mipi_attr\n", pdrv->index);
			return -1;
		}
		pctrl->mipi_cfg.lane_num = be32_to_cpup((u32 *)propdata);
		pctrl->mipi_cfg.bit_rate_max = be32_to_cpup((((u32 *)propdata) + 1));
		pctrl->mipi_cfg.factor_numerator = be32_to_cpup((((u32 *)propdata) + 2));
		pctrl->mipi_cfg.factor_denominator = 100;
		pctrl->mipi_cfg.operation_mode_init = be32_to_cpup((((u32 *)propdata) + 3));
		pctrl->mipi_cfg.operation_mode_display = be32_to_cpup((((u32 *)propdata) + 4));
		pctrl->mipi_cfg.video_mode_type = be32_to_cpup((((u32 *)propdata) + 5));
		pctrl->mipi_cfg.clk_always_hs = be32_to_cpup((((u32 *)propdata) + 6));
		pctrl->mipi_cfg.phy_switch = be32_to_cpup((((u32 *)propdata) + 7));

		pctrl->mipi_cfg.check_en = 0;
		pctrl->mipi_cfg.check_reg = 0xff;
		pctrl->mipi_cfg.check_cnt = 0;
#ifdef CONFIG_AML_LCD_TABLET
		lcd_mipi_dsi_init_table_detect(dt_addr, child_offset, &pctrl->mipi_cfg, 1);
		lcd_mipi_dsi_init_table_detect(dt_addr, child_offset, &pctrl->mipi_cfg, 0);
#endif

		propdata = (char *)fdt_getprop(dt_addr, child_offset, "extern_init", NULL);
		if (propdata) {
			pctrl->mipi_cfg.extern_init = be32_to_cpup((u32 *)propdata);
			if (pctrl->mipi_cfg.extern_init < 0xff) {
				LCDPR("[%d]: find extern_init: %d\n",
				      pdrv->index, pctrl->mipi_cfg.extern_init);
			}
#ifdef CONFIG_AML_LCD_EXTERN
			lcd_extern_drv_index_add(pdrv->index, pctrl->mipi_cfg.extern_init);
#endif
		}
#ifdef CONFIG_AML_LCD_TABLET
		mipi_dsi_config_init(pconf);
#endif
		break;
	case LCD_EDP:
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "edp_attr", NULL);
		if (!propdata) {
			LCDERR("[%d]: failed to get edp_attr\n", pdrv->index);
			return -1;
		}
		pctrl->edp_cfg.max_lane_count =
			(unsigned char)be32_to_cpup((u32 *)propdata);
		pctrl->edp_cfg.max_link_rate =
			(unsigned char)be32_to_cpup((((u32 *)propdata) + 1));
		pctrl->edp_cfg.training_mode =
			(unsigned char)be32_to_cpup((((u32 *)propdata) + 2));
		pctrl->edp_cfg.edid_en =
			(unsigned char)be32_to_cpup((((u32 *)propdata) + 3));
		pctrl->edp_cfg.dpcd_caps_en =
			(unsigned char)be32_to_cpup((((u32 *)propdata) + 4));
		pctrl->edp_cfg.sync_clk_mode =
			(unsigned char)be32_to_cpup((((u32 *)propdata) + 5));
		pctrl->edp_cfg.scramb_mode =
			(unsigned char)be32_to_cpup((((u32 *)propdata) + 6));
		pctrl->edp_cfg.enhanced_framing_en =
			(unsigned char)be32_to_cpup((((u32 *)propdata) + 7));
		pctrl->edp_cfg.pn_swap =
			(unsigned char)be32_to_cpup((((u32 *)propdata) + 8));

		pctrl->edp_cfg.lane_count = pctrl->edp_cfg.max_lane_count;
		pctrl->edp_cfg.link_rate = pctrl->edp_cfg.max_link_rate;

		propdata = (char *)fdt_getprop(dt_addr, child_offset, "phy_attr", NULL);
		if (!propdata) {
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
				LCDPR("[%d]: failed to get phy_attr\n", pdrv->index);
			pctrl->edp_cfg.phy_vswing = 0x5;
			pctrl->edp_cfg.phy_preem  = 0x1;
		} else {
			pctrl->edp_cfg.phy_vswing = be32_to_cpup((u32 *)propdata);
			pctrl->edp_cfg.phy_preem  = be32_to_cpup((((u32 *)propdata) + 1));
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				LCDPR("[%d]: set phy vswing=0x%x, preem=0x%x\n",
				      pdrv->index,
				      pctrl->edp_cfg.phy_vswing,
				      pctrl->edp_cfg.phy_preem);
			}
		}
		phy_cfg->lane_num = 4;
		phy_cfg->vswing_level = pctrl->edp_cfg.phy_vswing & 0xf;
		phy_cfg->preem_level = pctrl->edp_cfg.phy_preem;
		phy_cfg->vswing = phy_cfg->vswing_level;
		break;
	default:
		LCDERR("invalid lcd type\n");
		break;
	}

	/* check power_step */
	lcd_power_load_from_dts(pdrv, dt_addr, child_offset);

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "backlight_index", NULL);
	if (!propdata) {
		LCDERR("[%d]: failed to get backlight_index\n", pdrv->index);
		pconf->backlight_index = 0xff;
	} else {
		pconf->backlight_index = be32_to_cpup((u32 *)propdata);
#ifdef CONFIG_AML_LCD_BACKLIGHT
		aml_bl_index_add(pdrv->index, pconf->backlight_index);
#endif
	}
#endif

	return 0;
}

static int lcd_config_load_from_unifykey_v2(struct lcd_config_s *pconf,
					    unsigned char *p,
					    unsigned int key_len,
					    unsigned int offset)
{
	struct lcd_unifykey_header_s *header;
	struct phy_config_s *phy_cfg = &pconf->phy_cfg;
	struct cus_ctrl_config_s *cus_ctrl = &pconf->cus_ctrl;
	unsigned int len, temp;
	int i, ret;

	header = (struct lcd_unifykey_header_s *)p;
	LCDPR("unifykey version: 0x%04x\n", header->version);
	if (lcd_debug_print_flag) {
		LCDPR("unifykey header:\n");
		LCDPR("crc32             = 0x%08x\n", header->crc32);
		LCDPR("data_len          = %d\n", header->data_len);
		LCDPR("block_next_flag   = %d\n", header->block_next_flag);
		LCDPR("block_cur_size    = %d\n", header->block_cur_size);
	}

	/* step 2: check lcd parameters */
	len = offset + header->block_cur_size;
	ret = lcd_unifykey_len_check(key_len, len);
	if (ret < 0) {
		LCDERR("unifykey parameters length is incorrect\n");
		return -1;
	}

	/*phy 356byte*/
	phy_cfg->flag = (*(p + LCD_UKEY_PHY_ATTR_FLAG) |
		((*(p + LCD_UKEY_PHY_ATTR_FLAG + 1)) << 8) |
		((*(p + LCD_UKEY_PHY_ATTR_FLAG + 2)) << 16) |
		((*(p + LCD_UKEY_PHY_ATTR_FLAG + 3)) << 24));
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("%s: ctrl_flag=0x%x\n", __func__, phy_cfg->flag);

	phy_cfg->vcm = (*(p + LCD_UKEY_PHY_ATTR_1) |
			*(p + LCD_UKEY_PHY_ATTR_1 + 1) << 8);
	phy_cfg->ref_bias = (*(p + LCD_UKEY_PHY_ATTR_2) |
			*(p + LCD_UKEY_PHY_ATTR_2 + 1) << 8);
	phy_cfg->odt = (*(p + LCD_UKEY_PHY_ATTR_3) |
			*(p + LCD_UKEY_PHY_ATTR_3 + 1) << 8);
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("%s: vcm=0x%x, ref_bias=0x%x, odt=0x%x\n",
		      __func__, phy_cfg->vcm, phy_cfg->ref_bias,
		      phy_cfg->odt);
	}

	if (phy_cfg->flag & (1 << 12)) {
		for (i = 0; i < phy_cfg->lane_num; i++) {
			phy_cfg->lane[i].preem =
				*(p + LCD_UKEY_PHY_LANE_CTRL + 4 * i) |
				(*(p + LCD_UKEY_PHY_LANE_CTRL + 4 * i + 1) << 8);
			phy_cfg->lane[i].amp =
				*(p + LCD_UKEY_PHY_LANE_CTRL + 4 * i + 2) |
				(*(p + LCD_UKEY_PHY_LANE_CTRL + 4 * i + 3) << 8);
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				LCDPR("%s: lane[%d]: preem=0x%x, amp=0x%x\n",
					__func__, i,
					phy_cfg->lane[i].preem,
					phy_cfg->lane[i].amp);
			}
		}
	}

	/* ctrl */
	cus_ctrl->flag = (*(p + LCD_UKEY_CUS_CTRL_ATTR_FLAG) |
		((*(p + LCD_UKEY_CUS_CTRL_ATTR_FLAG + 1)) << 8) |
		((*(p + LCD_UKEY_CUS_CTRL_ATTR_FLAG + 2)) << 16) |
		((*(p + LCD_UKEY_CUS_CTRL_ATTR_FLAG + 3)) << 24));
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("%s: ctrl_flag=0x%x\n", __func__, cus_ctrl->flag);

	if (cus_ctrl->flag & 0x1) {
		temp = (*(p + LCD_UKEY_CUS_CTRL_ATTR_0) |
			*(p + LCD_UKEY_CUS_CTRL_ATTR_0 + 1) << 8);
		cus_ctrl->dlg_flag = temp & 0x3;
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
			LCDPR("%s: dlg_flag=%d\n",
			      __func__, cus_ctrl->dlg_flag);
		}
	}

	return 0;
}

static int lcd_config_load_from_unifykey(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_config_s *pconf = &pdrv->config;
	union lcd_ctrl_config_u *pctrl = &pdrv->config.control;
	struct phy_config_s *phy_cfg = &pdrv->config.phy_cfg;
	struct lcd_unifykey_header_s lcd_header;
	unsigned char *para;
	char key_str[10];
	int key_len, len;
	unsigned char *p;
	const char *str;
	unsigned int temp;
	int ret, i = 0;

	LCDPR("config load from unifykey\n");

	key_len = LCD_UKEY_LCD_SIZE;
	para = (unsigned char *)malloc(sizeof(unsigned char) * key_len);
	if (!para) {
		LCDERR("[%d]: %s: Not enough memory\n", pdrv->index, __func__);
		return -1;
	}
	memset(para, 0, (sizeof(unsigned char) * key_len));

	if (pdrv->index == 0)
		sprintf(key_str, "lcd");
	else
		sprintf(key_str, "lcd%d", pdrv->index);
	ret = lcd_unifykey_get(key_str, para, &key_len);
	if (ret) {
		free(para);
		return -1;
	}

	/* step 1: check header */
	len = LCD_UKEY_HEAD_SIZE;
	ret = lcd_unifykey_len_check(key_len, len);
	if (ret) {
		LCDERR("[%d]: unifykey header length is incorrect\n", pdrv->index);
		free(para);
		return -1;
	}

	lcd_unifykey_header_check(para, &lcd_header);
	LCDPR("[%d]: unifykey version: 0x%04x\n", pdrv->index, lcd_header.version);
	len = LCD_UKEY_DATA_LEN_V1; /*10+36+18+31+20*/
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("unifykey header:\n");
		LCDPR("crc32             = 0x%08x\n", lcd_header.crc32);
		LCDPR("data_len          = %d\n", lcd_header.data_len);
		LCDPR("block_next_flag   = %d\n", lcd_header.block_next_flag);
		LCDPR("block_cur_size    = 0x%04x\n", lcd_header.block_cur_size);
	}

	/* step 2: check lcd parameters */
	ret = lcd_unifykey_len_check(key_len, len);
	if (ret) {
		LCDERR("[%d]: unifykey parameters length is incorrect\n", pdrv->index);
		free(para);
		return -1;
	}

	/* basic: 36byte */
	p = para;
	str = (const char *)(p + LCD_UKEY_HEAD_SIZE);
	strncpy(pconf->basic.model_name, str,
		sizeof(pconf->basic.model_name) - 1);
	pconf->basic.model_name[sizeof(pconf->basic.model_name) - 1] = '\0';
	pconf->basic.lcd_type = *(p + LCD_UKEY_INTERFACE);
	pconf->basic.lcd_bits = *(p + LCD_UKEY_LCD_BITS);
	pconf->basic.screen_width = (*(p + LCD_UKEY_SCREEN_WIDTH) |
		((*(p + LCD_UKEY_SCREEN_WIDTH + 1)) << 8));
	pconf->basic.screen_height = (*(p + LCD_UKEY_SCREEN_HEIGHT) |
		((*(p + LCD_UKEY_SCREEN_HEIGHT + 1)) << 8));
	LCDPR("load ukey config: %s, lcd_type: %s(%d)\n",
		pconf->basic.model_name,
		lcd_type_type_to_str(pconf->basic.lcd_type),
		pconf->basic.lcd_type);

	/* timing: 18byte */
	pconf->basic.h_active = (*(p + LCD_UKEY_H_ACTIVE) |
		((*(p + LCD_UKEY_H_ACTIVE + 1)) << 8));
	pconf->basic.v_active = (*(p + LCD_UKEY_V_ACTIVE)) |
		((*(p + LCD_UKEY_V_ACTIVE + 1)) << 8);
	pconf->basic.h_period = (*(p + LCD_UKEY_H_PERIOD)) |
		((*(p + LCD_UKEY_H_PERIOD + 1)) << 8);
	pconf->basic.v_period = (*(p + LCD_UKEY_V_PERIOD)) |
		((*(p + LCD_UKEY_V_PERIOD + 1)) << 8);
	pconf->timing.hsync_width = (*(p + LCD_UKEY_HS_WIDTH) |
		((*(p + LCD_UKEY_HS_WIDTH + 1)) << 8));
	pconf->timing.hsync_bp = (*(p + LCD_UKEY_HS_BP) |
		((*(p + LCD_UKEY_HS_BP + 1)) << 8));
	pconf->timing.hsync_pol = *(p + LCD_UKEY_HS_POL);
	pconf->timing.vsync_width = (*(p + LCD_UKEY_VS_WIDTH) |
		((*(p + LCD_UKEY_VS_WIDTH + 1)) << 8));
	pconf->timing.vsync_bp = (*(p + LCD_UKEY_VS_BP) |
		((*(p + LCD_UKEY_VS_BP + 1)) << 8));
	pconf->timing.vsync_pol = *(p + LCD_UKEY_VS_POL);

	/* customer: 31byte */
	pconf->timing.fr_adjust_type = *(p + LCD_UKEY_FR_ADJ_TYPE);
	pconf->timing.ss_level = *(p + LCD_UKEY_SS_LEVEL);
	pconf->timing.clk_auto = *(p + LCD_UKEY_CLK_AUTO_GEN);
	pconf->timing.lcd_clk = (*(p + LCD_UKEY_PCLK) |
		((*(p + LCD_UKEY_PCLK + 1)) << 8) |
		((*(p + LCD_UKEY_PCLK + 2)) << 16) |
		((*(p + LCD_UKEY_PCLK + 3)) << 24));
	if (pconf->timing.lcd_clk == 0) {
		pconf->timing.lcd_clk = 60;
		LCDPR("[%d]: lcd_clk is 0, default to 60Hz\n", pdrv->index);
	}
	pconf->basic.h_period_min = (*(p + LCD_UKEY_H_PERIOD_MIN) |
		((*(p + LCD_UKEY_H_PERIOD_MIN + 1)) << 8));
	pconf->basic.h_period_max = (*(p + LCD_UKEY_H_PERIOD_MAX) |
		((*(p + LCD_UKEY_H_PERIOD_MAX + 1)) << 8));
	pconf->basic.v_period_min = (*(p + LCD_UKEY_V_PERIOD_MIN) |
		((*(p  + LCD_UKEY_V_PERIOD_MIN + 1)) << 8));
	pconf->basic.v_period_max = (*(p + LCD_UKEY_V_PERIOD_MAX) |
		((*(p + LCD_UKEY_V_PERIOD_MAX + 1)) << 8));
	pconf->basic.lcd_clk_min = (*(p + LCD_UKEY_PCLK_MIN) |
		((*(p + LCD_UKEY_PCLK_MIN + 1)) << 8) |
		((*(p + LCD_UKEY_PCLK_MIN + 2)) << 16) |
		((*(p + LCD_UKEY_PCLK_MIN + 3)) << 24));
	pconf->basic.lcd_clk_max = (*(p + LCD_UKEY_PCLK_MAX) |
		((*(p + LCD_UKEY_PCLK_MAX + 1)) << 8) |
		((*(p + LCD_UKEY_PCLK_MAX + 2)) << 16) |
		((*(p + LCD_UKEY_PCLK_MAX + 3)) << 24));
	pconf->basic.frame_rate_min = *(p + LCD_UKEY_FRAME_RATE_MIN);
	pconf->basic.frame_rate_max = *(p + LCD_UKEY_FRAME_RATE_MAX);

	pconf->custom_pinmux = *(p + LCD_UKEY_CUST_PINMUX);
	pconf->fr_auto_dis = *(p + LCD_UKEY_FR_AUTO_DIS);

	/* interface: 20byte */
	switch (pconf->basic.lcd_type) {
	case LCD_RGB:
		pctrl->rgb_cfg.type =
			(*(p + LCD_UKEY_IF_ATTR_0) |
			 ((*(p + LCD_UKEY_IF_ATTR_0 + 1)) << 8));
		pctrl->rgb_cfg.clk_pol =
			(*(p + LCD_UKEY_IF_ATTR_1) |
			 ((*(p + LCD_UKEY_IF_ATTR_1 + 1)) << 8));
		pctrl->rgb_cfg.de_valid =
			(*(p + LCD_UKEY_IF_ATTR_2) |
			 ((*(p + LCD_UKEY_IF_ATTR_2 + 1)) << 8));
		pctrl->rgb_cfg.sync_valid =
			(*(p + LCD_UKEY_IF_ATTR_3) |
			 ((*(p + LCD_UKEY_IF_ATTR_3 + 1)) << 8));
		pctrl->rgb_cfg.rb_swap =
			(*(p + LCD_UKEY_IF_ATTR_4) |
			 ((*(p + LCD_UKEY_IF_ATTR_4 + 1)) << 8));
		pctrl->rgb_cfg.bit_swap =
			(*(p + LCD_UKEY_IF_ATTR_5) |
			 ((*(p + LCD_UKEY_IF_ATTR_5 + 1)) << 8));
		break;
	case LCD_LVDS:
		pctrl->lvds_cfg.lvds_repack =
			*(p + LCD_UKEY_IF_ATTR_0) |
			((*(p + LCD_UKEY_IF_ATTR_0 + 1)) << 8);
		pctrl->lvds_cfg.dual_port =
			*(p + LCD_UKEY_IF_ATTR_1) |
			((*(p + LCD_UKEY_IF_ATTR_1 + 1)) << 8);
		pctrl->lvds_cfg.pn_swap =
			*(p + LCD_UKEY_IF_ATTR_2) |
			((*(p + LCD_UKEY_IF_ATTR_2 + 1)) << 8);
		pctrl->lvds_cfg.port_swap =
			*(p + LCD_UKEY_IF_ATTR_3) |
			((*(p + LCD_UKEY_IF_ATTR_3 + 1)) << 8);
		pctrl->lvds_cfg.phy_vswing =
			*(p + LCD_UKEY_IF_ATTR_4) |
			((*(p + LCD_UKEY_IF_ATTR_4 + 1)) << 8);
		pctrl->lvds_cfg.phy_preem =
			*(p + LCD_UKEY_IF_ATTR_5) |
			((*(p + LCD_UKEY_IF_ATTR_5 + 1)) << 8);
		pctrl->lvds_cfg.lane_reverse =
			*(p + LCD_UKEY_IF_ATTR_8) |
			((*(p + LCD_UKEY_IF_ATTR_8 + 1)) << 8);

		phy_cfg->lane_num = 12;
		phy_cfg->vswing_level = pctrl->lvds_cfg.phy_vswing & 0xf;
		phy_cfg->ext_pullup = (pctrl->lvds_cfg.phy_vswing >> 4) & 0x3;
		phy_cfg->vswing = lcd_phy_vswing_level_to_value(pdrv, phy_cfg->vswing_level);
		phy_cfg->preem_level = pctrl->lvds_cfg.phy_preem;
		temp = lcd_phy_preem_level_to_value(pdrv, phy_cfg->preem_level);
		for (i = 0; i < phy_cfg->lane_num; i++) {
			phy_cfg->lane[i].amp = 0;
			phy_cfg->lane[i].preem = temp;
		}
		break;
	case LCD_VBYONE:
		pctrl->vbyone_cfg.lane_count =
			*(p + LCD_UKEY_IF_ATTR_0) |
			((*(p + LCD_UKEY_IF_ATTR_0 + 1)) << 8);
		pctrl->vbyone_cfg.region_num =
			*(p + LCD_UKEY_IF_ATTR_1) |
			((*(p + LCD_UKEY_IF_ATTR_1 + 1)) << 8);
		pctrl->vbyone_cfg.byte_mode  =
			*(p + LCD_UKEY_IF_ATTR_2) |
			((*(p + LCD_UKEY_IF_ATTR_2 + 1)) << 8);
		pctrl->vbyone_cfg.color_fmt  =
			*(p + LCD_UKEY_IF_ATTR_3) |
			((*(p + LCD_UKEY_IF_ATTR_3 + 1)) << 8);
		pctrl->vbyone_cfg.phy_vswing =
			*(p + LCD_UKEY_IF_ATTR_4) |
			((*(p + LCD_UKEY_IF_ATTR_4 + 1)) << 8);
		pctrl->vbyone_cfg.phy_preem =
			*(p + LCD_UKEY_IF_ATTR_5) |
			((*(p + LCD_UKEY_IF_ATTR_5 + 1)) << 8);
		pctrl->vbyone_cfg.hw_filter_time =
			*(p + LCD_UKEY_IF_ATTR_8) |
			((*(p + LCD_UKEY_IF_ATTR_8 + 1)) << 8);
		pctrl->vbyone_cfg.hw_filter_cnt =
			*(p + LCD_UKEY_IF_ATTR_9) |
			((*(p + LCD_UKEY_IF_ATTR_9 + 1)) << 8);
		pctrl->vbyone_cfg.ctrl_flag = 0;
		pctrl->vbyone_cfg.power_on_reset_delay = VX1_PWR_ON_RESET_DLY_DFT;
		pctrl->vbyone_cfg.hpd_data_delay = VX1_HPD_DATA_DELAY_DFT;
		pctrl->vbyone_cfg.cdr_training_hold = VX1_CDR_TRAINING_HOLD_DFT;

		phy_cfg->lane_num = 8;
		phy_cfg->vswing_level = pctrl->vbyone_cfg.phy_vswing & 0xf;
		phy_cfg->ext_pullup = (pctrl->vbyone_cfg.phy_vswing >> 4) & 0x3;
		phy_cfg->vswing = lcd_phy_vswing_level_to_value(pdrv, phy_cfg->vswing_level);
		phy_cfg->preem_level = pctrl->vbyone_cfg.phy_preem;
		temp = lcd_phy_preem_level_to_value(pdrv, phy_cfg->preem_level);
		for (i = 0; i < phy_cfg->lane_num; i++) {
			phy_cfg->lane[i].amp = 0;
			phy_cfg->lane[i].preem = temp;
		}
		break;
	case LCD_MLVDS:
		pctrl->mlvds_cfg.channel_num =
			*(p + LCD_UKEY_IF_ATTR_0) |
			((*(p + LCD_UKEY_IF_ATTR_0 + 1)) << 8);
		pctrl->mlvds_cfg.channel_sel0 =
			*(p + LCD_UKEY_IF_ATTR_1) |
			((*(p + LCD_UKEY_IF_ATTR_1 + 1)) << 8) |
			((*(p + LCD_UKEY_IF_ATTR_2)) << 16) |
			((*(p + LCD_UKEY_IF_ATTR_2 + 1)) << 24);
		pctrl->mlvds_cfg.channel_sel1 =
			*(p + LCD_UKEY_IF_ATTR_3) |
			((*(p + LCD_UKEY_IF_ATTR_3 + 1)) << 8) |
			((*(p + LCD_UKEY_IF_ATTR_4)) << 16) |
			((*(p + LCD_UKEY_IF_ATTR_4 + 1)) << 24);
		pctrl->mlvds_cfg.clk_phase =
			*(p + LCD_UKEY_IF_ATTR_5) |
			((*(p + LCD_UKEY_IF_ATTR_5 + 1)) << 8);
		pctrl->mlvds_cfg.pn_swap =
			*(p + LCD_UKEY_IF_ATTR_6) |
			((*(p + LCD_UKEY_IF_ATTR_6 + 1)) << 8);
		pctrl->mlvds_cfg.bit_swap =
			*(p + LCD_UKEY_IF_ATTR_7) |
			((*(p + LCD_UKEY_IF_ATTR_7 + 1)) << 8);
		pctrl->mlvds_cfg.phy_vswing =
			*(p + LCD_UKEY_IF_ATTR_8) |
			((*(p + LCD_UKEY_IF_ATTR_8 + 1)) << 8);
		pctrl->mlvds_cfg.phy_preem =
			*(p + LCD_UKEY_IF_ATTR_9) |
			((*(p + LCD_UKEY_IF_ATTR_9 + 1)) << 8);

		phy_cfg->lane_num = 12;
		phy_cfg->vswing_level = pctrl->mlvds_cfg.phy_vswing & 0xf;
		phy_cfg->ext_pullup = (pctrl->mlvds_cfg.phy_vswing >> 4) & 0x3;
		phy_cfg->vswing = lcd_phy_vswing_level_to_value(pdrv, phy_cfg->vswing_level);
		phy_cfg->preem_level = pctrl->mlvds_cfg.phy_preem;
		temp = lcd_phy_preem_level_to_value(pdrv, phy_cfg->preem_level);
		for (i = 0; i < phy_cfg->lane_num; i++) {
			phy_cfg->lane[i].amp = 0;
			phy_cfg->lane[i].preem = temp;
		}
		break;
	case LCD_P2P:
		pctrl->p2p_cfg.p2p_type =
			*(p + LCD_UKEY_IF_ATTR_0) |
			((*(p + LCD_UKEY_IF_ATTR_0 + 1)) << 8);
		pctrl->p2p_cfg.lane_num =
			*(p + LCD_UKEY_IF_ATTR_1) |
			((*(p + LCD_UKEY_IF_ATTR_1 + 1)) << 8);
		pctrl->p2p_cfg.channel_sel0 =
			*(p + LCD_UKEY_IF_ATTR_2) |
			((*(p + LCD_UKEY_IF_ATTR_2 + 1)) << 8) |
			(*(p + LCD_UKEY_IF_ATTR_3) << 16) |
			((*(p + LCD_UKEY_IF_ATTR_3 + 1)) << 24);
		pctrl->p2p_cfg.channel_sel1 =
			*(p + LCD_UKEY_IF_ATTR_4) |
			((*(p + LCD_UKEY_IF_ATTR_4 + 1)) << 8) |
			(*(p + LCD_UKEY_IF_ATTR_5) << 16) |
			((*(p + LCD_UKEY_IF_ATTR_5 + 1)) << 24);
		pctrl->p2p_cfg.pn_swap =
			*(p + LCD_UKEY_IF_ATTR_6) |
			((*(p + LCD_UKEY_IF_ATTR_6 + 1)) << 8);
		pctrl->p2p_cfg.bit_swap =
			*(p + LCD_UKEY_IF_ATTR_7) |
			((*(p + LCD_UKEY_IF_ATTR_7 + 1)) << 8);
		pctrl->p2p_cfg.phy_vswing =
			*(p + LCD_UKEY_IF_ATTR_8) |
			((*(p + LCD_UKEY_IF_ATTR_8 + 1)) << 8);
		pctrl->p2p_cfg.phy_preem =
			*(p + LCD_UKEY_IF_ATTR_9) |
			((*(p + LCD_UKEY_IF_ATTR_9 + 1)) << 8);

		phy_cfg->lane_num = 12;
		phy_cfg->vswing_level = pctrl->p2p_cfg.phy_vswing & 0xf;
		phy_cfg->ext_pullup = (pctrl->p2p_cfg.phy_vswing >> 4) & 0x3;
		phy_cfg->vswing = lcd_phy_vswing_level_to_value(pdrv, phy_cfg->vswing_level);
		phy_cfg->preem_level = pctrl->p2p_cfg.phy_preem;
		temp = lcd_phy_preem_level_to_value(pdrv, phy_cfg->preem_level);
		for (i = 0; i < phy_cfg->lane_num; i++) {
			phy_cfg->lane[i].amp = 0;
			phy_cfg->lane[i].preem = temp;
		}
		break;
	default:
		LCDERR("[%d]: unsupport lcd_type: %d\n",
		       pdrv->index, pconf->basic.lcd_type);
		break;
	}

	/* step 3: check power sequence */
	ret = lcd_power_load_from_unifykey(pdrv, para, key_len, len);
	if (ret < 0) {
		free(para);
		return -1;
	}

	if (lcd_header.version == 2) {
		p = para + lcd_header.block_cur_size;
		lcd_config_load_from_unifykey_v2(pconf, p, key_len, lcd_header.block_cur_size);
	}

#ifdef CONFIG_AML_LCD_BACKLIGHT
	aml_bl_index_add(pdrv->index, 0);
#endif

	free(para);
	return 0;
}

static int lcd_config_load_from_bsp(struct aml_lcd_drv_s *pdrv)
{
	struct ext_lcd_config_s *ext_lcd;
	struct lcd_config_s *pconf = &pdrv->config;
	union lcd_ctrl_config_u *pctrl = &pdrv->config.control;
	struct phy_config_s *phy_cfg = &pdrv->config.phy_cfg;
	struct lcd_power_step_s *power_step;
	char *panel_type, str[15];
	unsigned int i, done;
	unsigned int temp;

	LCDPR("config load from bsp\n");

	if (pdrv->index >= LCD_MAX_DRV) {
		LCDERR("[%d]: invalid drv index %d\n", pdrv->index, pdrv->index);
		return -1;
	}

	if (pdrv->index == 0)
		sprintf(str, "panel_type");
	else
		sprintf(str, "panel%d_type", pdrv->index);
	panel_type = env_get(str);
	if (!panel_type) {
		LCDERR("[%d]: no %s exist\n", pdrv->index, str);
		return -1;
	}

	if (!pdrv->data->dft_conf[pdrv->index]) {
		LCDERR("[%d]: %s: dft_conf is NULL\n", pdrv->index, __func__);
		return -1;
	}
	ext_lcd = pdrv->data->dft_conf[pdrv->index]->ext_lcd;
	if (!ext_lcd) {
		LCDERR("[%d]: %s: ext_lcd is NULL\n", pdrv->index, __func__);
		return -1;
	}
	done = 0;
	for (i = 0 ; i < LCD_NUM_MAX ; i++) {
		if (strcmp(ext_lcd->panel_type, panel_type) == 0) {
			done = 1;
			break;
		}
		if (strcmp(ext_lcd->panel_type, "invalid") == 0)
			break;
		ext_lcd++;
	}
	if (done == 0) {
		LCDERR("[%d]: can't find %s\n ", pdrv->index, panel_type);
		return -1;
	}
	LCDPR("[%d]: use default %s=%s\n", pdrv->index, str, panel_type);

	strncpy(pconf->basic.model_name, panel_type,
		sizeof(pconf->basic.model_name) - 1);
	pconf->basic.model_name[sizeof(pconf->basic.model_name) - 1] = '\0';

	pconf->basic.lcd_type = ext_lcd->lcd_type;
	pconf->basic.lcd_bits = ext_lcd->lcd_bits;

	pconf->basic.h_active = ext_lcd->h_active;
	pconf->basic.v_active = ext_lcd->v_active;
	pconf->basic.h_period = ext_lcd->h_period;
	pconf->basic.v_period = ext_lcd->v_period;

	pconf->basic.h_period_min = pconf->basic.h_period;
	pconf->basic.h_period_max = pconf->basic.h_period;
	pconf->basic.v_period_min = pconf->basic.v_period;
	pconf->basic.v_period_max = pconf->basic.v_period;
	pconf->basic.lcd_clk_min = 0;
	pconf->basic.lcd_clk_max = 0;
	pconf->basic.frame_rate_min = 0;
	pconf->basic.frame_rate_max = 0;

	pconf->timing.hsync_width = ext_lcd->hsync_width;
	pconf->timing.hsync_bp    = ext_lcd->hsync_bp;
	pconf->timing.hsync_pol   = ext_lcd->hsync_pol;
	pconf->timing.vsync_width = ext_lcd->vsync_width;
	pconf->timing.vsync_bp    = ext_lcd->vsync_bp;
	pconf->timing.vsync_pol   = ext_lcd->vsync_pol;

	/* fr_adjust_type */
	temp = ext_lcd->customer_val_0;
	if (temp == Rsv_val)
		pconf->timing.fr_adjust_type = 0;
	else
		pconf->timing.fr_adjust_type = (unsigned char)temp;
	/* ss_level */
	temp = ext_lcd->customer_val_1;
	if (temp == Rsv_val)
		pconf->timing.ss_level = 0;
	else
		pconf->timing.ss_level = temp;
	/* clk_auto_generate */
	temp = ext_lcd->customer_val_2;
	if (temp == Rsv_val)
		pconf->timing.clk_auto = 1;
	else
		pconf->timing.clk_auto = (unsigned char)temp;
	/* lcd_clk */
	temp = ext_lcd->customer_val_3;
	if (temp == Rsv_val)
		pconf->timing.lcd_clk = 60;
	else
		pconf->timing.lcd_clk = temp;

	switch (pconf->basic.lcd_type) {
	case LCD_RGB:
		pctrl->rgb_cfg.type = ext_lcd->lcd_spc_val0;
		pctrl->rgb_cfg.clk_pol = ext_lcd->lcd_spc_val1;
		pctrl->rgb_cfg.de_valid = ext_lcd->lcd_spc_val2;
		pctrl->rgb_cfg.sync_valid = ext_lcd->lcd_spc_val3;
		pctrl->rgb_cfg.rb_swap = ext_lcd->lcd_spc_val4;
		pctrl->rgb_cfg.bit_swap = ext_lcd->lcd_spc_val5;
		break;
	case LCD_LVDS:
		pctrl->lvds_cfg.lvds_repack = ext_lcd->lcd_spc_val0;
		pctrl->lvds_cfg.dual_port   = ext_lcd->lcd_spc_val1;
		pctrl->lvds_cfg.pn_swap     = ext_lcd->lcd_spc_val2;
		pctrl->lvds_cfg.port_swap   = ext_lcd->lcd_spc_val3;
		pctrl->lvds_cfg.lane_reverse = ext_lcd->lcd_spc_val4;
		pctrl->lvds_cfg.phy_vswing = ext_lcd->lcd_spc_val5;
		pctrl->lvds_cfg.phy_preem  = ext_lcd->lcd_spc_val6;
		pctrl->lvds_cfg.phy_clk_vswing = ext_lcd->lcd_spc_val7;
		pctrl->lvds_cfg.phy_clk_preem  = ext_lcd->lcd_spc_val8;

		phy_cfg->lane_num = 12;
		phy_cfg->vswing_level = pctrl->lvds_cfg.phy_vswing & 0xf;
		phy_cfg->ext_pullup = (pctrl->lvds_cfg.phy_vswing >> 4) & 0x3;
		phy_cfg->vswing = lcd_phy_vswing_level_to_value(pdrv, phy_cfg->vswing_level);
		phy_cfg->preem_level = pctrl->lvds_cfg.phy_preem;
		temp = lcd_phy_preem_level_to_value(pdrv, phy_cfg->preem_level);
		for (i = 0; i < phy_cfg->lane_num; i++) {
			phy_cfg->lane[i].amp = 0;
			phy_cfg->lane[i].preem = temp;
		}
		break;
	case LCD_VBYONE:
		pctrl->vbyone_cfg.lane_count = ext_lcd->lcd_spc_val0;
		pctrl->vbyone_cfg.region_num = ext_lcd->lcd_spc_val1;
		pctrl->vbyone_cfg.byte_mode  = ext_lcd->lcd_spc_val2;
		pctrl->vbyone_cfg.color_fmt  = ext_lcd->lcd_spc_val3;
		pctrl->vbyone_cfg.phy_vswing = ext_lcd->lcd_spc_val4;
		pctrl->vbyone_cfg.phy_preem  = ext_lcd->lcd_spc_val5;
		if (ext_lcd->lcd_spc_val8 == Rsv_val ||
		    ext_lcd->lcd_spc_val9 == Rsv_val) {
			pctrl->vbyone_cfg.hw_filter_time = 0;
			pctrl->vbyone_cfg.hw_filter_cnt = 0;
		} else {
			pctrl->vbyone_cfg.hw_filter_time = ext_lcd->lcd_spc_val8;
			pctrl->vbyone_cfg.hw_filter_cnt  = ext_lcd->lcd_spc_val9;
		}

		pctrl->vbyone_cfg.ctrl_flag = 0;
		pctrl->vbyone_cfg.power_on_reset_delay = VX1_PWR_ON_RESET_DLY_DFT;
		pctrl->vbyone_cfg.hpd_data_delay = VX1_HPD_DATA_DELAY_DFT;
		pctrl->vbyone_cfg.cdr_training_hold = VX1_CDR_TRAINING_HOLD_DFT;

		phy_cfg->lane_num = 8;
		phy_cfg->vswing_level = pctrl->vbyone_cfg.phy_vswing & 0xf;
		phy_cfg->ext_pullup = (pctrl->vbyone_cfg.phy_vswing >> 4) & 0x3;
		phy_cfg->vswing = lcd_phy_vswing_level_to_value(pdrv, phy_cfg->vswing_level);
		phy_cfg->preem_level = pctrl->vbyone_cfg.phy_preem;
		temp = lcd_phy_preem_level_to_value(pdrv, phy_cfg->preem_level);
		for (i = 0; i < phy_cfg->lane_num; i++) {
			phy_cfg->lane[i].amp = 0;
			phy_cfg->lane[i].preem = temp;
		}
		break;
	case LCD_MLVDS:
		pctrl->mlvds_cfg.channel_num = ext_lcd->lcd_spc_val0;
		pctrl->mlvds_cfg.channel_sel0 = ext_lcd->lcd_spc_val1;
		pctrl->mlvds_cfg.channel_sel1 = ext_lcd->lcd_spc_val2;
		pctrl->mlvds_cfg.clk_phase  = ext_lcd->lcd_spc_val3;
		pctrl->mlvds_cfg.pn_swap    = ext_lcd->lcd_spc_val4;
		pctrl->mlvds_cfg.bit_swap   = ext_lcd->lcd_spc_val5;
		pctrl->mlvds_cfg.phy_vswing = ext_lcd->lcd_spc_val6;
		pctrl->mlvds_cfg.phy_preem  = ext_lcd->lcd_spc_val7;

		phy_cfg->lane_num = 12;
		phy_cfg->vswing_level = pctrl->mlvds_cfg.phy_vswing & 0xf;
		phy_cfg->ext_pullup = (pctrl->mlvds_cfg.phy_vswing >> 4) & 0x3;
		phy_cfg->vswing = lcd_phy_vswing_level_to_value(pdrv, phy_cfg->vswing_level);
		phy_cfg->preem_level = pctrl->mlvds_cfg.phy_preem;
		temp = lcd_phy_preem_level_to_value(pdrv, phy_cfg->preem_level);
		for (i = 0; i < phy_cfg->lane_num; i++) {
			phy_cfg->lane[i].amp = 0;
			phy_cfg->lane[i].preem = temp;
		}
		break;
	case LCD_P2P:
		pctrl->p2p_cfg.p2p_type = ext_lcd->lcd_spc_val0;
		pctrl->p2p_cfg.lane_num = ext_lcd->lcd_spc_val1;
		pctrl->p2p_cfg.channel_sel0 = ext_lcd->lcd_spc_val2;
		pctrl->p2p_cfg.channel_sel1 = ext_lcd->lcd_spc_val3;
		pctrl->p2p_cfg.pn_swap    = ext_lcd->lcd_spc_val4;
		pctrl->p2p_cfg.bit_swap   = ext_lcd->lcd_spc_val5;
		pctrl->p2p_cfg.phy_vswing = ext_lcd->lcd_spc_val6;
		pctrl->p2p_cfg.phy_preem  = ext_lcd->lcd_spc_val7;

		phy_cfg->lane_num = 12;
		phy_cfg->vswing_level = pctrl->p2p_cfg.phy_vswing & 0xf;
		phy_cfg->ext_pullup = (pctrl->p2p_cfg.phy_vswing >> 4) & 0x3;
		phy_cfg->vswing = lcd_phy_vswing_level_to_value(pdrv, phy_cfg->vswing_level);
		phy_cfg->preem_level = pctrl->p2p_cfg.phy_preem;
		temp = lcd_phy_preem_level_to_value(pdrv, phy_cfg->preem_level);
		for (i = 0; i < phy_cfg->lane_num; i++) {
			phy_cfg->lane[i].amp = 0;
			phy_cfg->lane[i].preem = temp;
		}
		break;
	case LCD_MIPI:
		pctrl->mipi_cfg.lane_num = ext_lcd->lcd_spc_val0;
		pctrl->mipi_cfg.bit_rate_max   = ext_lcd->lcd_spc_val1;
		pctrl->mipi_cfg.factor_numerator = ext_lcd->lcd_spc_val2;
		pctrl->mipi_cfg.operation_mode_init = ext_lcd->lcd_spc_val3;
		pctrl->mipi_cfg.operation_mode_display = ext_lcd->lcd_spc_val4;
		pctrl->mipi_cfg.video_mode_type = ext_lcd->lcd_spc_val5;
		pctrl->mipi_cfg.clk_always_hs = ext_lcd->lcd_spc_val6;
		pctrl->mipi_cfg.phy_switch = ext_lcd->lcd_spc_val7;
		pctrl->mipi_cfg.factor_denominator = 100;

		pctrl->mipi_cfg.check_en = 0;
		pctrl->mipi_cfg.check_reg = 0xff;
		pctrl->mipi_cfg.check_cnt = 0;
#ifdef CONFIG_AML_LCD_TABLET
		pctrl->mipi_cfg.dsi_init_on = ext_lcd->init_on;
		pctrl->mipi_cfg.dsi_init_off = ext_lcd->init_off;
		lcd_mipi_dsi_init_table_check_bsp(&pctrl->mipi_cfg, 1);
		lcd_mipi_dsi_init_table_check_bsp(&pctrl->mipi_cfg, 0);
#endif

		if (ext_lcd->lcd_spc_val9 == Rsv_val)
			pctrl->mipi_cfg.extern_init = 0xff;
		else
			pctrl->mipi_cfg.extern_init = ext_lcd->lcd_spc_val9;
		if (pctrl->mipi_cfg.extern_init < 0xff) {
			LCDPR("[%d]: find extern_init: %d\n",
			      pdrv->index, pctrl->mipi_cfg.extern_init);
		}
#ifdef CONFIG_AML_LCD_EXTERN
		lcd_extern_drv_index_add(pdrv->index, pctrl->mipi_cfg.extern_init);
#endif
#ifdef CONFIG_AML_LCD_TABLET
		mipi_dsi_config_init(pconf);
#endif
		break;
	case LCD_EDP:
		pctrl->edp_cfg.max_lane_count = ext_lcd->lcd_spc_val0;
		pctrl->edp_cfg.max_link_rate = ext_lcd->lcd_spc_val1;
		pctrl->edp_cfg.training_mode = ext_lcd->lcd_spc_val2;
		pctrl->edp_cfg.dpcd_caps_en = ext_lcd->lcd_spc_val3;
		pctrl->edp_cfg.sync_clk_mode = ext_lcd->lcd_spc_val4;
		pctrl->edp_cfg.scramb_mode = ext_lcd->lcd_spc_val5;
		pctrl->edp_cfg.enhanced_framing_en = ext_lcd->lcd_spc_val6;
		pctrl->edp_cfg.edid_en = ext_lcd->lcd_spc_val7;
		pctrl->edp_cfg.pn_swap = 0;
		pctrl->edp_cfg.phy_vswing = ext_lcd->lcd_spc_val8;
		pctrl->edp_cfg.phy_preem  = ext_lcd->lcd_spc_val9;

		pctrl->edp_cfg.lane_count = pctrl->edp_cfg.max_lane_count;
		pctrl->edp_cfg.link_rate = pctrl->edp_cfg.max_link_rate;

		phy_cfg->lane_num = 4;
		phy_cfg->vswing_level = pctrl->edp_cfg.phy_vswing;
		phy_cfg->preem_level = pctrl->edp_cfg.phy_preem;
		break;
	default:
		break;
	}

	i = 0;
	while (i < LCD_PWR_STEP_MAX) {
		power_step = &ext_lcd->power_on_step[i];
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
			LCDPR("power_on: step %d: type=%d, index=%d, value=%d, delay=%d\n",
			      i, power_step->type, power_step->index,
			      power_step->value, power_step->delay);
		}
		pconf->power.power_on_step[i].type = power_step->type;
		pconf->power.power_on_step[i].index = power_step->index;
		pconf->power.power_on_step[i].value = power_step->value;
		pconf->power.power_on_step[i].delay = power_step->delay;
		if (power_step->type >= LCD_POWER_TYPE_MAX)
			break;
		if (power_step->type == LCD_POWER_TYPE_EXTERN) {
#ifdef CONFIG_AML_LCD_EXTERN
			lcd_extern_drv_index_add(pdrv->index, pconf->power.power_on_step[i].index);
#endif
		}
		i++;
	}

	i = 0;
	while (i < LCD_PWR_STEP_MAX) {
		power_step = &ext_lcd->power_off_step[i];
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
			LCDPR("power_off: step %d: type=%d, index=%d, value=%d, delay=%d\n",
			      i, power_step->type, power_step->index,
			      power_step->value, power_step->delay);
		}
		pconf->power.power_off_step[i].type = power_step->type;
		pconf->power.power_off_step[i].index = power_step->index;
		pconf->power.power_off_step[i].value = power_step->value;
		pconf->power.power_off_step[i].delay = power_step->delay;
		if (power_step->type >= LCD_POWER_TYPE_MAX)
			break;
		if (power_step->type == LCD_POWER_TYPE_EXTERN) {
#ifdef CONFIG_AML_LCD_EXTERN
			lcd_extern_drv_index_add(pdrv->index, pconf->power.power_off_step[i].index);
#endif
		}
		i++;
	}
#ifdef CONFIG_AML_LCD_BACKLIGHT
	aml_bl_index_add(pdrv->index, 0);
#endif

	return 0;
}

int lcd_get_config(char *dt_addr, int load_id, struct aml_lcd_drv_s *pdrv)
{
	int ret;

	if (load_id & 0x10)
		ret = lcd_config_load_from_unifykey(pdrv);
	else if (load_id & 0x1)
		ret = lcd_config_load_from_dts(dt_addr, pdrv);
	else
		ret = lcd_config_load_from_bsp(pdrv);
	if (ret)
		return -1;

	lcd_config_load_print(pdrv);
	lcd_pinmux_load_config(pdrv);

#ifdef CONFIG_AML_LCD_TCON
	lcd_tcon_probe(dt_addr, pdrv, load_id);
#endif

	return 0;
}

static unsigned int vbyone_lane_num[] = {
	1,
	2,
	4,
	8,
	8,
};

#define VBYONE_BIT_RATE_MAX		3100 //MHz
#define VBYONE_BIT_RATE_MIN		600
void lcd_vbyone_config_set(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned int band_width, bit_rate, pclk, phy_div;
	unsigned int byte_mode, lane_count, minlane;
	unsigned int temp, i;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	//auto calculate bandwidth, clock
	lane_count = pconf->control.vbyone_cfg.lane_count;
	byte_mode = pconf->control.vbyone_cfg.byte_mode;
	/* byte_mode * byte2bit * 8/10_encoding * pclk =
	   byte_mode * 8 * 10 / 8 * pclk */
	pclk = pconf->timing.lcd_clk / 1000; /* kHz */
	band_width = byte_mode * 10 * pclk;

	temp = VBYONE_BIT_RATE_MAX * 1000;
	temp = (band_width + temp - 1) / temp;
	for (i = 0; i < 4; i++) {
		if (temp <= vbyone_lane_num[i])
			break;
	}
	minlane = vbyone_lane_num[i];
	if (lane_count < minlane) {
		LCDERR("vbyone lane_num(%d) is less than min(%d)\n",
			lane_count, minlane);
		lane_count = minlane;
		pconf->control.vbyone_cfg.lane_count = lane_count;
		LCDPR("change to min lane_num %d\n", minlane);
	}

	bit_rate = band_width / lane_count;
	phy_div = lane_count / lane_count;
	if (phy_div == 8) {
		phy_div /= 2;
		bit_rate /= 2;
	}
	if (bit_rate > (VBYONE_BIT_RATE_MAX * 1000)) {
		LCDERR("vbyone bit rate(%dKHz) is out of max(%dKHz)\n",
			bit_rate, (VBYONE_BIT_RATE_MAX * 1000));
	}
	if (bit_rate < (VBYONE_BIT_RATE_MIN * 1000)) {
		LCDERR("vbyone bit rate(%dKHz) is out of min(%dKHz)\n",
			bit_rate, (VBYONE_BIT_RATE_MIN * 1000));
	}
	bit_rate = bit_rate * 1000; /* Hz */

	pconf->control.vbyone_cfg.phy_div = phy_div;
	pconf->timing.bit_rate = bit_rate;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("lane_count=%u, bit_rate = %uMHz, pclk=%u.%03uMhz\n",
			lane_count, (bit_rate / 1000000),
			(pclk / 1000), (pclk % 1000));
	}
}

void lcd_mlvds_config_set(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned int bit_rate, pclk;
	unsigned int lcd_bits, channel_num;
	unsigned int channel_sel0, channel_sel1, pi_clk_sel = 0;
	unsigned int i, temp;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	lcd_bits = pconf->basic.lcd_bits;
	channel_num = pconf->control.mlvds_cfg.channel_num;
	pclk = pconf->timing.lcd_clk / 1000;
	bit_rate = lcd_bits * 3 * pclk / channel_num;

	pconf->timing.bit_rate = bit_rate * 1000;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("channel_num=%u, bit_rate=%u.%03uMHz, pclk=%u.%03uMhz\n",
		      channel_num, (bit_rate / 1000), (bit_rate % 1000),
		      (pclk / 1000), (pclk % 1000));
	}

	/* pi_clk select */
	channel_sel0 = pconf->control.mlvds_cfg.channel_sel0;
	channel_sel1 = pconf->control.mlvds_cfg.channel_sel1;
	/* mlvds channel:    //tx 12 channels
	 *    0: clk_a
	 *    1: d0_a
	 *    2: d1_a
	 *    3: d2_a
	 *    4: d3_a
	 *    5: d4_a
	 *    6: clk_b
	 *    7: d0_b
	 *    8: d1_b
	 *    9: d2_b
	 *   10: d3_b
	 *   11: d4_b
	 */
	for (i = 0; i < 8; i++) {
		temp = (channel_sel0 >> (i * 4)) & 0xf;
		if (temp == 0 || temp == 6)
			pi_clk_sel |= (1 << i);
	}
	for (i = 0; i < 4; i++) {
		temp = (channel_sel1 >> (i * 4)) & 0xf;
		if (temp == 0 || temp == 6)
			pi_clk_sel |= (1 << (i + 8));
	}

	pconf->control.mlvds_cfg.pi_clk_sel = pi_clk_sel;
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("channel_sel0=0x%08x, channel_sel1=0x%08x, pi_clk_sel=0x%03x\n",
		      channel_sel0, channel_sel1, pi_clk_sel);
	}
}

void lcd_p2p_config_set(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned int bit_rate, pclk;
	unsigned int lcd_bits, lane_num;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s\n", pdrv->index, __func__);

	lcd_bits = pconf->basic.lcd_bits;
	lane_num = pconf->control.p2p_cfg.lane_num;
	pclk = pconf->timing.lcd_clk / 1000;
	switch (pconf->control.p2p_cfg.p2p_type) {
	case P2P_CEDS:
		if (pclk >= 600000)
			bit_rate = pclk * 3 * lcd_bits / lane_num;
		else
			bit_rate = pclk * (3 * lcd_bits + 4) / lane_num;
		break;
	case P2P_CHPI: /* 8/10 coding */
		bit_rate = (pclk * 3 * lcd_bits * 10 / 8) / lane_num;
		break;
	default:
		bit_rate = pclk * 3 * lcd_bits / lane_num;
		break;
	}

	pconf->timing.bit_rate = bit_rate * 1000;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("lane_num=%u, bit_rate=%u.%03uMHz, pclk=%u.%03uMhz\n",
		      lane_num, (bit_rate / 1000), (bit_rate % 1000),
		      (pclk / 1000), (pclk % 1000));
	}
}

void lcd_mipi_dsi_config_set(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned int pclk, bit_rate;
	unsigned int bit_rate_max;
	struct dsi_config_s *dconf = &pconf->control.mipi_cfg;
	unsigned int temp;

	dconf = &pconf->control.mipi_cfg;

	/* unit in kHz for calculation */
	pclk = pconf->timing.lcd_clk / 1000;
	if (dconf->operation_mode_display == OPERATION_VIDEO_MODE &&
	    dconf->video_mode_type != BURST_MODE) {
		temp = pclk * 4 * dconf->data_bits;
		bit_rate = temp / dconf->lane_num;
	} else {
		temp = pclk * 3 * dconf->data_bits;
		bit_rate = temp / dconf->lane_num;
	}
	temp = bit_rate / pclk;
	if (temp % 2)
		bit_rate += pclk;
	dconf->local_bit_rate_min = bit_rate /* khz */;

	/* bit rate max */
	if (dconf->bit_rate_max == 0) { /* auto calculate */
		bit_rate_max = bit_rate + (pclk / 2);
		if (bit_rate_max > MIPI_BIT_RATE_MAX) {
			LCDERR("[%d]: %s: invalid bit_rate_max %d\n",
				pdrv->index, __func__, bit_rate_max);
			bit_rate_max = MIPI_BIT_RATE_MAX;
		}
		dconf->local_bit_rate_max = bit_rate_max;
		LCDPR("[%d]: mipi dsi bit_rate max=%dkHz\n",
		      pdrv->index, dconf->local_bit_rate_max);
	} else { /* user define */
		dconf->local_bit_rate_max = dconf->bit_rate_max * 1000;
		if (dconf->local_bit_rate_max > MIPI_BIT_RATE_MAX) {
			LCDPR("[%d]: invalid mipi-dsi bit_rate_max %dkHz (max=%dkHz)\n",
			      pdrv->index, dconf->local_bit_rate_max,
			      MIPI_BIT_RATE_MAX);
		}
	}
}

void lcd_edp_config_set(struct aml_lcd_drv_s *pdrv)
{
	//todo
}

void lcd_basic_timing_range_update(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned int sync_duration, h_period, v_period, vmin, vmax;
	unsigned long long temp;

	//for basic timing
	h_period = pconf->basic.h_period;
	v_period = pconf->basic.v_period;
	if (pconf->timing.lcd_clk < 200) { /* regard as frame_rate */
		sync_duration = pconf->timing.lcd_clk;
		pconf->timing.lcd_clk = sync_duration * h_period * v_period;
		pconf->timing.sync_duration_num = sync_duration;
		pconf->timing.sync_duration_den = 1;
	} else { /* regard as pixel clock */
		temp = pconf->timing.lcd_clk;
		temp *= 1000;
		sync_duration = lcd_do_div(temp, (v_period * h_period));
		pconf->timing.sync_duration_num = sync_duration;
		pconf->timing.sync_duration_den = 1000;
	}

	//for vrr range config
	temp = pconf->timing.sync_duration_num;
	temp *= v_period;
	vmin = pconf->basic.v_period_min * pconf->timing.sync_duration_den;
	vmax = pconf->basic.v_period_max * pconf->timing.sync_duration_den;
	if (pconf->basic.frame_rate_max == 0) {
		if (vmin > 0)
			pconf->basic.frame_rate_max = lcd_do_div(temp, vmin);
	}
	if (pconf->basic.frame_rate_min == 0) {
		if (vmax > 0)
			pconf->basic.frame_rate_min = lcd_do_div(temp, vmax);
	}

	//save default config
	pconf->timing.lcd_clk_dft = pconf->timing.lcd_clk;
	pconf->timing.h_period_dft = pconf->basic.h_period;
	pconf->timing.v_period_dft = pconf->basic.v_period;
}

void lcd_timing_init_config(struct lcd_config_s *pconf)
{
	unsigned short h_period, v_period, h_active, v_active;
	unsigned short hsync_bp, hsync_width, vsync_bp, vsync_width;
	unsigned short de_hstart, de_vstart;
	unsigned short hstart, hend, vstart, vend;
	unsigned short h_delay = 0;

	h_period = pconf->basic.h_period;
	v_period = pconf->basic.v_period;
	h_active = pconf->basic.h_active;
	v_active = pconf->basic.v_active;
	hsync_bp = pconf->timing.hsync_bp;
	hsync_width = pconf->timing.hsync_width;
	vsync_bp = pconf->timing.vsync_bp;
	vsync_width = pconf->timing.vsync_width;

	de_hstart = hsync_bp + hsync_width;
	de_vstart = vsync_bp + vsync_width;

	pconf->timing.hstart = de_hstart - h_delay;
	pconf->timing.vstart = de_vstart;
	pconf->timing.hend = h_active + pconf->timing.hstart - 1;
	pconf->timing.vend = v_active + pconf->timing.vstart - 1;

	pconf->timing.de_hs_addr = de_hstart;
	pconf->timing.de_he_addr = de_hstart + h_active;
	pconf->timing.de_vs_addr = de_vstart;
	pconf->timing.de_ve_addr = de_vstart + v_active - 1;

	hstart = (de_hstart + h_period - hsync_bp - hsync_width) % h_period;
	hend = (de_hstart + h_period - hsync_bp) % h_period;
	pconf->timing.hs_hs_addr = hstart;
	pconf->timing.hs_he_addr = hend;
	pconf->timing.hs_vs_addr = 0;
	pconf->timing.hs_ve_addr = v_period - 1;

	pconf->timing.vs_hs_addr = (hstart + h_period) % h_period;
	pconf->timing.vs_he_addr = pconf->timing.vs_hs_addr;
	vstart = (de_vstart + v_period - vsync_bp - vsync_width) % v_period;
	vend = (de_vstart + v_period - vsync_bp) % v_period;
	pconf->timing.vs_vs_addr = vstart;
	pconf->timing.vs_ve_addr = vend;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		LCDPR("hs_hs_addr=%d, hs_he_addr=%d, hs_vs_addr=%d, hs_ve_addr=%d\n",
			pconf->timing.hs_hs_addr, pconf->timing.hs_he_addr,
			pconf->timing.hs_vs_addr, pconf->timing.hs_ve_addr);
		LCDPR("vs_hs_addr=%d, vs_he_addr=%d, vs_vs_addr=%d, vs_ve_addr=%d\n",
			pconf->timing.vs_hs_addr, pconf->timing.vs_he_addr,
			pconf->timing.vs_vs_addr, pconf->timing.vs_ve_addr);
	}
}

int lcd_vmode_change(struct aml_lcd_drv_s *pdrv)
{
	struct lcd_config_s *pconf = &pdrv->config;
	unsigned char type = pconf->timing.fr_adjust_type;
	 /* use default value to avoid offset */
	unsigned int pclk = pconf->timing.lcd_clk_dft;
	unsigned int h_period = pconf->timing.h_period_dft;
	unsigned int v_period = pconf->timing.v_period_dft;
	unsigned int pclk_min = pconf->basic.lcd_clk_min;
	unsigned int pclk_max = pconf->basic.lcd_clk_max;
	unsigned int duration_num = pconf->timing.sync_duration_num;
	unsigned int duration_den = pconf->timing.sync_duration_den;
	unsigned long long temp;
	char str[100];
	int len = 0;

	pconf->timing.clk_change = 0; /* clear clk flag */
	switch (type) {
	case 0: /* pixel clk adjust */
		temp = duration_num;
		temp = temp * h_period * v_period;
		pclk = lcd_do_div(temp, duration_den);
		if (pconf->timing.lcd_clk != pclk)
			pconf->timing.clk_change = LCD_CLK_PLL_CHANGE;
		break;
	case 1: /* htotal adjust */
		temp = pclk;
		temp =  temp * duration_den * 100;
		h_period = v_period * duration_num;
		h_period = lcd_do_div(temp, h_period);
		h_period = (h_period + 99) / 100; /* round off */
		if (pconf->basic.h_period != h_period) {
			/* check clk frac update */
			temp = duration_num;
			temp = temp * h_period * v_period;
			pclk = lcd_do_div(temp, duration_den);
		}
		if (pconf->timing.lcd_clk != pclk)
			pconf->timing.clk_change = LCD_CLK_FRAC_UPDATE;
		break;
	case 2: /* vtotal adjust */
		temp = pclk;
		temp = temp * duration_den * 100;
		v_period = h_period * duration_num;
		v_period = lcd_do_div(temp, v_period);
		v_period = (v_period + 99) / 100; /* round off */
		if (pconf->basic.v_period != v_period) {
			/* check clk frac update */
			temp = duration_num;
			temp = temp * h_period * v_period;
			pclk = lcd_do_div(temp, duration_den);
		}
		if (pconf->timing.lcd_clk != pclk)
			pconf->timing.clk_change = LCD_CLK_FRAC_UPDATE;
		break;
	case 3: /* free adjust, use min/max range to calculate */
		temp = pclk;
		temp = temp * duration_den * 100;
		v_period = h_period * duration_num;
		v_period = lcd_do_div(temp, v_period);
		v_period = (v_period + 99) / 100; /* round off */
		if (v_period > pconf->basic.v_period_max) {
			v_period = pconf->basic.v_period_max;
			h_period = v_period * duration_num;
			h_period = lcd_do_div(temp, h_period);
			h_period = (h_period + 99) / 100; /* round off */
			if (h_period > pconf->basic.h_period_max) {
				h_period = pconf->basic.h_period_max;
				temp = duration_num;
				temp = temp * h_period * v_period;
				pclk = lcd_do_div(temp, duration_den);
				if (pclk > pclk_max) {
					LCDERR("[%d]: %s: invalid vmode\n",
						pdrv->index, __func__);
					return -1;
				}
				if (pconf->timing.lcd_clk != pclk)
					pconf->timing.clk_change = LCD_CLK_PLL_CHANGE;
			}
		} else if (v_period < pconf->basic.v_period_min) {
			v_period = pconf->basic.v_period_min;
			h_period = v_period * duration_num;
			h_period = lcd_do_div(temp, h_period);
			h_period = (h_period + 99) / 100; /* round off */
			if (h_period < pconf->basic.h_period_min) {
				h_period = pconf->basic.h_period_min;
				temp = duration_num;
				temp = temp * h_period * v_period;
				pclk = lcd_do_div(temp, duration_den);
				if (pclk < pclk_min) {
					LCDERR("[%d]: %s: invalid vmode\n",
						pdrv->index, __func__);
					return -1;
				}
				if (pconf->timing.lcd_clk != pclk)
					pconf->timing.clk_change = LCD_CLK_PLL_CHANGE;
			}
		}
		/* check clk frac update */
		if ((pconf->timing.clk_change & LCD_CLK_PLL_CHANGE) == 0) {
			temp = duration_num;
			temp = temp * h_period * v_period;
			pclk = lcd_do_div(temp, duration_den);
			if (pconf->timing.lcd_clk != pclk)
				pconf->timing.clk_change = LCD_CLK_FRAC_UPDATE;
		}
		break;
	case 4: /* hdmi mode */
		if (((duration_num / duration_den) == 59) ||
		    ((duration_num / duration_den) == 47) ||
		    ((duration_num / duration_den) == 119) ||
		    ((duration_num / duration_den) == 95)) {
			/* pixel clk adjust */
			temp = duration_num;
			temp = temp * h_period * v_period;
			pclk = lcd_do_div(temp, duration_den);
			if (pconf->timing.lcd_clk != pclk)
				pconf->timing.clk_change = LCD_CLK_PLL_CHANGE;
		} else {
			/* htotal adjust */
			temp = pclk;
			temp = temp * duration_den * 100;
			h_period = v_period * duration_num;
			h_period = lcd_do_div(temp, h_period);
			h_period = (h_period + 99) / 100; /* round off */
			if (pconf->basic.h_period != h_period) {
				/* check clk frac update */
				temp = duration_num;
				temp = temp * h_period * v_period;
				pclk = lcd_do_div(temp, duration_den);
			}
			if (pconf->timing.lcd_clk != pclk)
				pconf->timing.clk_change = LCD_CLK_FRAC_UPDATE;
		}
		break;
	default:
		LCDERR("[%d]: %s: invalid fr_adjust_type: %d\n",
		       pdrv->index, __func__, type);
		return 0;
	}

	if (pconf->basic.v_period != v_period) {
		len += sprintf(str+len, "v_period %u->%u", pconf->basic.v_period, v_period);
		/* update v_period */
		pconf->basic.v_period = v_period;
	}
	if (pconf->basic.h_period != h_period) {
		if (len > 0)
			len += sprintf(str+len, ", ");
		len += sprintf(str+len, "h_period %u->%u", pconf->basic.h_period, h_period);
		/* update h_period */
		pconf->basic.h_period = h_period;
	}
	if (pconf->timing.lcd_clk != pclk) {
		if (len > 0)
			len += sprintf(str+len, ", ");
		len += sprintf(str + len, "pclk %u.%03uMHz->%u.%03uMHz, clk_change:%d",
			(pconf->timing.lcd_clk / 1000000),
			((pconf->timing.lcd_clk / 1000) % 1000),
			(pclk / 1000000), ((pclk / 1000) % 1000),
			pconf->timing.clk_change);
		pconf->timing.lcd_clk = pclk;
	}
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		if (len > 0)
			LCDPR("[%d]: %s: %s\n", pdrv->index, __func__, str);
	}

	return 0;
}

void lcd_pinmux_set(struct aml_lcd_drv_s *pdrv, int status)
{
	struct lcd_config_s *pconf;
	int i;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		LCDPR("[%d]: %s: %d\n", pdrv->index, __func__, status);

	pconf = &pdrv->config;
	if (status) {
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (pconf->pinmux_clr[i][0] == LCD_PINMUX_END)
				break;
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				LCDPR("pinmux_clr: %d, 0x%08x\n",
					pconf->pinmux_clr[i][0],
					pconf->pinmux_clr[i][1]);
			}
			lcd_pinmux_clr_mask(pconf->pinmux_clr[i][0],
				pconf->pinmux_clr[i][1]);
			i++;
		}
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (pconf->pinmux_set[i][0] == LCD_PINMUX_END)
				break;
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				LCDPR("pinmux_set: %d, 0x%08x\n",
					pconf->pinmux_set[i][0],
					pconf->pinmux_set[i][1]);
			}
			lcd_pinmux_set_mask(pconf->pinmux_set[i][0],
				pconf->pinmux_set[i][1]);
			i++;
		}
	} else {
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (pconf->pinmux_set[i][0] == LCD_PINMUX_END)
				break;
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				LCDPR("pinmux_clr: %d, 0x%08x\n",
					pconf->pinmux_set[i][0],
					pconf->pinmux_set[i][1]);
			}
			lcd_pinmux_clr_mask(pconf->pinmux_set[i][0],
				pconf->pinmux_set[i][1]);
			i++;
		}
	}
}

