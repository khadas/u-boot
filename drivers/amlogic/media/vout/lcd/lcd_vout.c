/*
 * drivers/amlogic/media/vout/lcd/lcd_vout.c
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
#include <amlogic/cpu_id.h>
#include <fdtdec.h>
#include <amlogic/keyunify.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#ifdef CONFIG_AML_LCD_EXTERN
#include <amlogic/media/vout/lcd/lcd_extern.h>
#endif
#include "lcd_reg.h"
#include "lcd_common.h"
#ifdef CONFIG_AML_LCD_TABLET
#include "lcd_tablet/mipi_dsi_util.h"
#endif

#define PANEL_NAME	"panel"

DECLARE_GLOBAL_DATA_PTR;

unsigned int lcd_debug_print_flag;
unsigned int lcd_debug_load_flag;
static struct aml_lcd_drv_s aml_lcd_driver;

static void lcd_chip_detect(void)
{
#if 1
	unsigned int cpu_type;

	cpu_type = get_cpu_id().family_id;
	switch (cpu_type) {
	case MESON_CPU_MAJOR_ID_G12A:
		aml_lcd_driver.chip_type = LCD_CHIP_G12A;
		break;
	case MESON_CPU_MAJOR_ID_G12B:
		aml_lcd_driver.chip_type = LCD_CHIP_G12B;
		break;
	case MESON_CPU_MAJOR_ID_TL1:
		aml_lcd_driver.chip_type = LCD_CHIP_TL1;
		break;
	case MESON_CPU_MAJOR_ID_SM1:
		aml_lcd_driver.chip_type = LCD_CHIP_SM1;
		break;
	default:
		aml_lcd_driver.chip_type = LCD_CHIP_MAX;
		break;
	}
#else
	aml_lcd_driver.chip_type = LCD_CHIP_G12B;
#endif
	if (lcd_debug_print_flag)
		LCDPR("check chip: %d\n", aml_lcd_driver.chip_type);
}

static int lcd_check_valid(void)
{
	if (aml_lcd_driver.config_check == NULL) {
		LCDERR("invalid lcd config\n");
		return -1;
	}
	return 0;
}

static void lcd_power_ctrl(int status)
{
	struct aml_lcd_drv_s *lcd_drv = aml_lcd_get_driver();
	struct lcd_power_ctrl_s *lcd_power;
	struct lcd_power_step_s *power_step;
#ifdef CONFIG_AML_LCD_EXTERN
	struct aml_lcd_extern_driver_s *ext_drv;
#endif
	int i;

	i = 0;
	lcd_power = lcd_drv->lcd_config->lcd_power;
	if (status) {
		/* check if factory test */
		if (lcd_drv->factory_lcd_power_on_step) {
			LCDPR("%s: factory test power_on_step!\n", __func__);
			power_step = lcd_drv->factory_lcd_power_on_step;
		} else {
			power_step = &lcd_power->power_on_step[0];
		}
	} else {
		power_step = &lcd_power->power_off_step[0];
	}

	while (i < LCD_PWR_STEP_MAX) {
		if (power_step->type >= LCD_POWER_TYPE_MAX)
			break;
		if (lcd_debug_print_flag) {
			LCDPR("power_ctrl: %d, step %d: type=%d, index=%d, value=%d, delay=%d\n",
				status, i, power_step->type, power_step->index,
				power_step->value, power_step->delay);
		}
		switch (power_step->type) {
		case LCD_POWER_TYPE_CPU:
			lcd_gpio_set(power_step->index, power_step->value);
			break;
		case LCD_POWER_TYPE_PMU:
			LCDPR("to do\n");
			break;
		case LCD_POWER_TYPE_SIGNAL:
			if (status)
				lcd_drv->driver_init();
			else
				lcd_drv->driver_disable();
			break;
#ifdef CONFIG_AML_LCD_EXTERN
		case LCD_POWER_TYPE_EXTERN:
			ext_drv = aml_lcd_extern_get_driver();
			if (ext_drv == NULL) {
				LCDERR("ext_drv is null\n");
				break;
			}
			if (status) {
				if (ext_drv->power_on)
					ext_drv->power_on();
				else
					LCDERR("no ext power on\n");
			} else {
				if (ext_drv->power_off)
					ext_drv->power_off();
				else
					LCDERR("no ext power off\n");
			}
			break;
#endif
		default:
			break;
		}
		if (power_step->delay)
			mdelay(power_step->delay);
		i++;
		power_step++;
	}

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);
}

static void lcd_module_enable(char *mode)
{
	unsigned int sync_duration;
	struct aml_lcd_drv_s *lcd_drv = aml_lcd_get_driver();
	struct lcd_config_s *pconf = lcd_drv->lcd_config;
	int ret;

	ret = lcd_drv->config_check(mode);
	if (ret) {
		LCDERR("init exit\n");
		return;
	}

	sync_duration = pconf->lcd_timing.sync_duration_num;
	sync_duration = (sync_duration * 10 / pconf->lcd_timing.sync_duration_den);
	LCDPR("enable: %s, %s, %ux%u@%u.%uHz\n", pconf->lcd_basic.model_name,
		lcd_type_type_to_str(pconf->lcd_basic.lcd_type),
		pconf->lcd_basic.h_active, pconf->lcd_basic.v_active,
		(sync_duration / 10), (sync_duration % 10));

	lcd_drv->driver_init_pre();
	lcd_power_ctrl(1);

	pconf->retry_enable_cnt = 0;
	while (pconf->retry_enable_flag) {
		if (pconf->retry_enable_cnt++ >= LCD_ENABLE_RETRY_MAX)
			break;
		LCDPR("retry enable...%d\n", pconf->retry_enable_cnt);
		lcd_power_ctrl(0);
		mdelay(1000);
		lcd_power_ctrl(1);
	}
	pconf->retry_enable_cnt = 0;

	lcd_vcbus_write(VPP_POSTBLEND_H_SIZE, pconf->lcd_basic.h_active);
	lcd_vcbus_write(VENC_INTCTRL, 0x200);

	bl_pwm_config_update(lcd_drv->bl_config);
	bl_set_level(lcd_drv->bl_config->level_default);
	bl_power_ctrl(1);
	lcd_mute_setting(0);

	lcd_drv->lcd_status = 1;
}

static void lcd_module_disable(void)
{
	struct aml_lcd_drv_s *lcd_drv = aml_lcd_get_driver();

	LCDPR("disable: %s\n", lcd_drv->lcd_config->lcd_basic.model_name);

	lcd_mute_setting(1);
	bl_power_ctrl(0);

	lcd_power_ctrl(0);

	lcd_drv->lcd_status = 0;
}

static void lcd_vbyone_filter_flag_print(struct lcd_config_s *pconf)
{
	struct vbyone_config_s *vx1_conf = pconf->lcd_control.vbyone_config;

	LCDPR("vx1_sw_filter_en: %d\n", vx1_conf->vx1_sw_filter_en);
	LCDPR("vx1_sw_filter_time: %d\n", vx1_conf->vx1_sw_filter_time);
	LCDPR("vx1_sw_filter_cnt: %d\n", vx1_conf->vx1_sw_filter_cnt);
	LCDPR("vx1_sw_filter_retry_cnt: %d\n", vx1_conf->vx1_sw_filter_retry_cnt);
	LCDPR("vx1_sw_filter_retry_delay: %d\n", vx1_conf->vx1_sw_filter_retry_delay);
	LCDPR("vx1_sw_cdr_detect_time: %d\n", vx1_conf->vx1_sw_cdr_detect_time);
	LCDPR("vx1_sw_cdr_detect_cnt: %d\n", vx1_conf->vx1_sw_cdr_detect_cnt);
	LCDPR("vx1_sw_cdr_timeout_cnt: %d\n", vx1_conf->vx1_sw_cdr_timeout_cnt);
}

static void lcd_vbyone_filter_env_init(struct lcd_config_s *pconf)
{
	struct vbyone_config_s *vx1_conf = pconf->lcd_control.vbyone_config;
	char *str;
	unsigned int temp = 0;

	str = env_get("lcd_debug_vx1_sw_filter");
	if (str)
		temp = simple_strtoul(str, NULL, 10);
	if (temp == 0)
		return;

	LCDPR("%s\n", __func__);
	str = env_get("vx1_sw_filter_en");
	if (str) {
		vx1_conf->vx1_sw_filter_en = simple_strtoul(str, NULL, 10);
		LCDPR("vx1_sw_filter_en: %d\n", vx1_conf->vx1_sw_filter_en);
	}

	str = env_get("vx1_sw_filter_time"); /* 100us */
	if (str) {
		vx1_conf->vx1_sw_filter_time = simple_strtoul(str, NULL, 10);
		LCDPR("vx1_sw_filter_time: %d\n", vx1_conf->vx1_sw_filter_time);
	}

	str = env_get("vx1_sw_filter_cnt");
	if (str) {
		vx1_conf->vx1_sw_filter_cnt = simple_strtoul(str, NULL, 10);
		LCDPR("vx1_sw_filter_cnt: %d\n", vx1_conf->vx1_sw_filter_cnt);
	}

	str = env_get("vx1_sw_filter_retry_cnt");
	if (str) {
		vx1_conf->vx1_sw_filter_retry_cnt = simple_strtoul(str, NULL, 10);
		LCDPR("vx1_sw_filter_retry_cnt: %d\n", vx1_conf->vx1_sw_filter_retry_cnt);
	}

	str = env_get("vx1_sw_filter_retry_delay"); /* ms */
	if (str) {
		vx1_conf->vx1_sw_filter_retry_delay = simple_strtoul(str, NULL, 10);
		LCDPR("vx1_sw_filter_retry_delay: %d\n", vx1_conf->vx1_sw_filter_retry_delay);
	}

	str = env_get("vx1_sw_cdr_detect_time"); /* us * 100 */
	if (str) {
		vx1_conf->vx1_sw_cdr_detect_time = simple_strtoul(str, NULL, 10);
		LCDPR("vx1_sw_cdr_detect_time: %d\n", vx1_conf->vx1_sw_cdr_detect_time);
	}

	str = env_get("vx1_sw_cdr_detect_cnt");
	if (str) {
		vx1_conf->vx1_sw_cdr_detect_cnt = simple_strtoul(str, NULL, 10);
		LCDPR("vx1_sw_cdr_detect_cnt: %d\n", vx1_conf->vx1_sw_cdr_detect_cnt);
	}

	str = env_get("vx1_sw_cdr_timeout_cnt");
	if (str) {
		vx1_conf->vx1_sw_cdr_timeout_cnt = simple_strtoul(str, NULL, 10);
		LCDPR("vx1_sw_cdr_timeout_cnt: %d\n", vx1_conf->vx1_sw_cdr_timeout_cnt);
	}
}

#ifdef CONFIG_AML_LCD_EXTERN
static int lcd_extern_load_config(const void *dt_blob, struct lcd_config_s *pconf)
{
	struct lcd_power_step_s *power_step;
	int index, i;

	/* mipi extern_init is special */
	if (pconf->lcd_basic.lcd_type == LCD_MIPI) {
		index = pconf->lcd_control.mipi_config->extern_init;
		if (index < LCD_EXTERN_INDEX_INVALID)
			aml_lcd_extern_probe(dt_blob, index);
	}

	i = 0;
	while (i < LCD_PWR_STEP_MAX) {
		power_step = &pconf->lcd_power->power_on_step[i];
		if (power_step->type >= LCD_POWER_TYPE_MAX)
			break;
		if (power_step->type == LCD_POWER_TYPE_EXTERN) {
			if (lcd_debug_print_flag) {
				LCDPR("power_on: step %d: type=%d, index=%d\n",
					i, power_step->type, power_step->index);
			}
			index = power_step->index;
			if (index < LCD_EXTERN_INDEX_INVALID)
				aml_lcd_extern_probe(dt_blob, index);
		}
		i++;
	}

	return 0;
}
#endif

static int lcd_init_load_from_dts(const void *dt_blob)
{
	struct lcd_config_s *pconf = aml_lcd_driver.lcd_config;
	int parent_offset;
	char *propdata, *p;
	const char *str;
	int i, temp;

	/* check bl_key_valid */
	parent_offset = fdt_path_offset(dt_blob, "/backlight");
	if (parent_offset < 0) {
		LCDERR("not find /backlight node: %s\n", fdt_strerror(parent_offset));
		aml_lcd_driver.bl_config->bl_key_valid = 0;
	}
	propdata = (char *)fdt_getprop(dt_blob, parent_offset, "key_valid", NULL);
	if (propdata == NULL) {
		LCDERR("failed to get key_valid\n");
		aml_lcd_driver.bl_config->bl_key_valid = 0;
	} else {
		aml_lcd_driver.bl_config->bl_key_valid = (unsigned char)(be32_to_cpup((u32*)propdata));
	}

	parent_offset = fdt_path_offset(dt_blob, "/lcd");
	if (parent_offset < 0) {
		LCDERR("not find /lcd node: %s\n", fdt_strerror(parent_offset));
		return -1;
	}

	/* check lcd_mode & lcd_key_valid */
	propdata = (char *)fdt_getprop(dt_blob, parent_offset, "mode", NULL);
	if (propdata == NULL) {
		LCDERR("failed to get mode\n");
		return -1;
	} else {
		pconf->lcd_mode = lcd_mode_str_to_mode(propdata);
	}
	str = propdata;
	propdata = (char *)fdt_getprop(dt_blob, parent_offset, "key_valid", NULL);
	if (propdata == NULL) {
		LCDERR("failed to get key_valid\n");
		pconf->lcd_key_valid = 0;
	} else {
		pconf->lcd_key_valid = (unsigned char)(be32_to_cpup((u32*)propdata));
	}
	LCDPR("detect mode: %s, key_valid: %d\n", str, pconf->lcd_key_valid);

	/* check lcd_clk_path */
	propdata = (char *)fdt_getprop(dt_blob, parent_offset, "clk_path", NULL);
	if (propdata == NULL) {
		if (lcd_debug_print_flag)
			LCDPR("failed to get clk_path\n");
		pconf->lcd_clk_path = 0;
	} else {
		pconf->lcd_clk_path = (unsigned char)(be32_to_cpup((u32*)propdata));
		LCDPR("detect lcd_clk_path: %d\n", pconf->lcd_clk_path);
	}
	str = env_get("lcd_clk_path");
	if (str) {
		temp = simple_strtoul(str, NULL, 10);
		if (temp)
			pconf->lcd_clk_path = 1;
		else
			pconf->lcd_clk_path = 0;
		LCDPR("lcd_clk_path flag set clk_path: %d\n", pconf->lcd_clk_path);
	}

	i = 0;
	propdata = (char *)fdt_getprop(dt_blob, parent_offset, "lcd_cpu_gpio_names", NULL);
	if (propdata == NULL) {
		LCDPR("failed to get lcd_cpu_gpio_names\n");
	} else {
		p = propdata;
		while (i < LCD_CPU_GPIO_NUM_MAX) {
			str = p;
			if (strlen(str) == 0)
				break;
			lcd_gpio_probe(str, i);
			p += strlen(p) + 1;
			i++;
		}
	}

	return 0;
}

static int lcd_init_load_from_bsp(void)
{
	struct lcd_config_s *pconf = aml_lcd_driver.lcd_config;
	int i, temp;
	char *str;

	/*pconf->lcd_key_valid = 0;
	aml_lcd_driver.bl_config->bl_key_valid = 0;*/
	LCDPR("detect mode: %s, key_valid: %d\n",
		lcd_mode_mode_to_str(pconf->lcd_mode), pconf->lcd_key_valid);

	i = 0;
	while (i < LCD_CPU_GPIO_NUM_MAX) {
		if (strcmp(pconf->lcd_power->cpu_gpio[i], "invalid") == 0)
			break;
		lcd_gpio_probe(pconf->lcd_power->cpu_gpio[i], i);
		i++;
	}

	str = env_get("lcd_clk_path");
	if (str) {
		temp = simple_strtoul(str, NULL, 10);
		if (temp)
			pconf->lcd_clk_path = 1;
		else
			pconf->lcd_clk_path = 0;
		LCDPR("lcd_clk_path flag set clk_path: %d\n", pconf->lcd_clk_path);
	}

	return 0;
}

static int lcd_config_probe(void)
{
	const void *dt_blob;
	int load_id = 0;
	char *str;
	int parent_offset;
	int ret;

	dt_blob = gd->fdt_blob;
	if (dt_blob == NULL) {
		LCDERR("dt_blob is null, load default lcd parameters\n");
	} else {
		ret = fdt_check_header(dt_blob);
		if (ret) {
			LCDERR("check dts: %s, load default lcd parameters\n",
				fdt_strerror(ret));
		} else {
			parent_offset = fdt_path_offset(dt_blob, "/lcd");
			if (parent_offset < 0) {
				LCDERR("not find /lcd node: %s\n",
					fdt_strerror(parent_offset));
			} else {
				load_id = 0x1;
			}
		}
	}

	lcd_debug_load_flag = 0;
	str = env_get("lcd_debug_test");
	if (str == NULL)
		lcd_debug_load_flag = 0;
	else
		lcd_debug_load_flag = simple_strtoul(str, NULL, 10);
	if (lcd_debug_load_flag) {
		load_id = 0x0;
		LCDPR("lcd_debug_load_flag: %d\n", lcd_debug_load_flag);
	}

	/* default setting */
	aml_lcd_driver.lcd_config->retry_enable_flag = 0;
	aml_lcd_driver.lcd_config->retry_enable_cnt = 0;

	if (load_id & 0x1) {
		ret = lcd_init_load_from_dts(dt_blob);
		if (ret)
			return -1;
		if (aml_lcd_driver.unifykey_test_flag) {
			aml_lcd_driver.bl_config->bl_key_valid = 1;
			aml_lcd_driver.lcd_config->lcd_key_valid = 1;
			LCDPR("force bl_key_valid & lcd_key_valid to 1\n");
		}
		if (aml_lcd_driver.lcd_config->lcd_key_valid) {
			ret = aml_lcd_unifykey_check("lcd");
			if (ret == 0) {
				LCDPR("load config from unifykey\n");
				load_id |= 0x10;
			} else {
				LCDPR("load config from dts\n");
			}
		} else {
			LCDPR("load config from dts\n");
		}
	} else {
		ret = lcd_init_load_from_bsp();
		if (ret)
			return -1;
		if (aml_lcd_driver.unifykey_test_flag) {
			aml_lcd_driver.bl_config->bl_key_valid = 1;
			aml_lcd_driver.lcd_config->lcd_key_valid = 1;
			LCDPR("force bl_key_valid & lcd_key_valid to 1\n");
		}
		if (aml_lcd_driver.lcd_config->lcd_key_valid) {
			ret = aml_lcd_unifykey_check("lcd");
			if (ret == 0) {
				LCDPR("load lcd_config from unifykey\n");
				load_id |= 0x10;
			} else {
				LCDPR("load lcd_config from bsp\n");
			}
		} else {
			LCDPR("load config from bsp\n");
		}
	}

	lcd_clk_config_probe();

	/* load lcd config */
	switch (aml_lcd_driver.lcd_config->lcd_mode) {
#ifdef CONFIG_AML_LCD_TV
	case LCD_MODE_TV:
		ret = get_lcd_tv_config(dt_blob, load_id);
		break;
#endif
#ifdef CONFIG_AML_LCD_TABLET
	case LCD_MODE_TABLET:
		ret = get_lcd_tablet_config(dt_blob, load_id);
		break;
#endif
	default:
		LCDERR("invalid lcd mode: %d\n", aml_lcd_driver.lcd_config->lcd_mode);
		break;
	}
	if (ret) {
		aml_lcd_driver.config_check = NULL;
		LCDERR("invalid lcd config\n");
		return -1;
	}
	if (aml_lcd_driver.lcd_config->lcd_basic.lcd_type == LCD_VBYONE)
		lcd_vbyone_filter_env_init(aml_lcd_driver.lcd_config);
#if 0
	if (aml_lcd_driver.chip_type == LCD_CHIP_TXHD)
		lcd_tcon_probe(dt_blob, &aml_lcd_driver, load_id);
#endif

#ifdef CONFIG_AML_LCD_EXTERN
	lcd_extern_load_config(dt_blob, aml_lcd_driver.lcd_config);
#endif

	/* load bl config */
	if (aml_lcd_driver.bl_config->bl_key_valid) {
		ret = aml_lcd_unifykey_check("backlight");
		if (ret == 0) {
			LCDPR("load backlight_config from unifykey\n");
			load_id |= 0x10;
		} else {
			load_id &= ~(0x10);
		}
	} else {
		load_id &= ~(0x10);
	}
	bl_config_load(dt_blob, load_id);

	if (lcd_debug_print_flag) {
		if (aml_lcd_driver.lcd_config->lcd_basic.lcd_type == LCD_VBYONE)
			lcd_vbyone_filter_flag_print(aml_lcd_driver.lcd_config);
	}

	lcd_debug_probe(&aml_lcd_driver);

	return 0;
}

int lcd_probe(void)
{
#ifdef LCD_DEBUG_INFO
	lcd_debug_print_flag = 1;
#else
	char *str;

	str = env_get("lcd_debug_print");
	if (str == NULL) {
		lcd_debug_print_flag = 0;
	} else {
		lcd_debug_print_flag = simple_strtoul(str, NULL, 10);
		LCDPR("lcd_debug_print flag: %d\n", lcd_debug_print_flag);
	}
#endif

	lcd_chip_detect();
	lcd_config_bsp_init();
	lcd_config_probe();

	return 0;
}

int lcd_remove(void)
{
#ifdef CONFIG_AML_LCD_EXTERN
	aml_lcd_extern_remove();
#endif

	return 0;
}

#define LCD_WAIT_VSYNC_TIMEOUT    50000
void lcd_wait_vsync(void)
{
	int line_cnt, line_cnt_previous;
	int i = 0;

	line_cnt = 0x1fff;
	line_cnt_previous = lcd_vcbus_getb(ENCL_INFO_READ, 16, 13);
	while (i++ < LCD_WAIT_VSYNC_TIMEOUT) {
		line_cnt = lcd_vcbus_getb(ENCL_INFO_READ, 16, 13);
		if (line_cnt < line_cnt_previous)
			break;
		line_cnt_previous = line_cnt;
		udelay(2);
	}
	/*LCDPR("line_cnt=%d, line_cnt_previous=%d, i=%d\n",
	 *	line_cnt, line_cnt_previous, i);
	 */
}

/* ********************************************** *
  lcd driver API
 * ********************************************** */
static int lcd_outputmode_check(char *mode)
{
	if (aml_lcd_driver.outputmode_check)
		return aml_lcd_driver.outputmode_check(mode);

	LCDERR("invalid lcd config\n");
	return -1;
}

static void lcd_enable(char *mode)
{
	if (lcd_check_valid())
		return;
	if (aml_lcd_driver.lcd_status)
		LCDPR("already enabled\n");
	else
		lcd_module_enable(mode);
}

static void lcd_disable(void)
{
	if (lcd_check_valid())
		return;
	if (aml_lcd_driver.lcd_status)
		lcd_module_disable();
	else
		LCDPR("already disabled\n");
}

static void lcd_set_ss(int level)
{
	struct aml_lcd_drv_s *lcd_drv = aml_lcd_get_driver();

	if (lcd_check_valid())
		return;
	if (aml_lcd_driver.lcd_status) {
		lcd_drv->lcd_config->lcd_timing.ss_level = level;
		lcd_set_spread_spectrum(level);
	} else {
		LCDPR("already disabled\n");
	}
}

static char *lcd_get_ss(void)
{
	char *str = "invalid";

	if (lcd_check_valid())
		return str;
	if (aml_lcd_driver.lcd_status)
		str = lcd_get_spread_spectrum();
	else
		LCDPR("already disabled\n");

	return str;
}

static void lcd_test(int num)
{
	if (lcd_check_valid())
		return;
	if (aml_lcd_driver.lcd_status)
		lcd_debug_test(num);
	else
		LCDPR("already disabled\n");
}

static void lcd_clk_info(void)
{
	if (lcd_check_valid())
		return;
	lcd_clk_config_print();
}

static void lcd_info(void)
{
	if (lcd_check_valid())
		return;
	lcd_info_print();
}

static void lcd_reg_info(void)
{
	if (lcd_check_valid())
		return;
	lcd_reg_print();
}

static void lcd_tcon_reg_info(void)
{
	if (lcd_check_valid())
		return;
	lcd_tcon_reg_readback_print();
}

static void lcd_tcon_table_info(void)
{
	if (lcd_check_valid())
		return;
	lcd_tcon_reg_table_print();
}

static void lcd_key_test(void)
{
	if (aml_lcd_driver.unifykey_test_flag) {
		aml_lcd_unifykey_test();
		lcd_config_probe();
	} else {
		printf("lcd unifykey test disabled\n");
	}
}

static void lcd_key_tcon_test(void)
{
	if (aml_lcd_driver.unifykey_test_flag) {
		aml_lcd_unifykey_tcon_test(1080);
		lcd_config_probe();
	} else {
		printf("lcd unifykey test disabled\n");
	}
}

static void lcd_key_dump(void)
{
	int flag = LCD_UKEY_DEBUG_NORMAL;
#if 0
	switch (aml_lcd_driver.chip_type) {
	case LCD_CHIP_TXHD:
		flag |= LCD_UKEY_DEBUG_TCON;
		break;
	default:
		break;
	}
#endif
	aml_lcd_unifykey_dump(flag);
}

static void lcd_extern_info(void)
{
#ifdef CONFIG_AML_LCD_EXTERN
	struct aml_lcd_extern_driver_s *ext_drv;

	ext_drv = aml_lcd_extern_get_driver();
	if (ext_drv)
		ext_drv->info_print();
#else
	printf("lcd_extern is not support\n");
#endif
}

static struct aml_lcd_drv_s aml_lcd_driver = {
	.lcd_status = 0,
	.lcd_config = &lcd_config_dft,
	.bl_config = &bl_config_dft,
	.config_check = NULL,
	.lcd_probe = lcd_probe,
	.lcd_outputmode_check = lcd_outputmode_check,
	.lcd_enable = lcd_enable,
	.lcd_disable = lcd_disable,
	.lcd_set_ss = lcd_set_ss,
	.lcd_get_ss = lcd_get_ss,
	.lcd_test = lcd_test,
	.lcd_clk = lcd_clk_info,
	.lcd_info = lcd_info,
	.lcd_reg = lcd_reg_info,
	.lcd_tcon_reg = lcd_tcon_reg_info,
	.lcd_tcon_table = lcd_tcon_table_info,
	.bl_power_ctrl = bl_power_ctrl,
	.bl_set_level = bl_set_level,
	.bl_get_level = bl_get_level,
	.bl_config_print = bl_config_print,
	.unifykey_test_flag = 0, /* default disable unifykey test */
	.unifykey_test = lcd_key_test,
	.unifykey_tcon_test = lcd_key_tcon_test,
	.unifykey_dump = lcd_key_dump,
	.lcd_extern_info = lcd_extern_info,

	/* for factory test */
	.factory_lcd_power_on_step = NULL,
	.factory_bl_power_on_delay = -1,
};

struct aml_lcd_drv_s *aml_lcd_get_driver(void)
{
	return &aml_lcd_driver;
}
