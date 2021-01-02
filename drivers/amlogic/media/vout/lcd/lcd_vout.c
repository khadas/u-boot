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
unsigned int lcd_debug_test;
static struct lcd_drv_s lcd_driver;
static struct lcd_boot_ctrl_s boot_ctrl;

static void lcd_chip_detect(void)
{
#if 1
	unsigned int cpu_type;
	unsigned int rev_type;

	cpu_type = get_cpu_id().family_id;
	rev_type = get_cpu_id().chip_rev;
	switch (cpu_type) {
	case MESON_CPU_MAJOR_ID_G12A:
		lcd_driver.chip_type = LCD_CHIP_G12A;
		break;
	case MESON_CPU_MAJOR_ID_G12B:
		lcd_driver.chip_type = LCD_CHIP_G12B;
		break;
	case MESON_CPU_MAJOR_ID_TL1:
		lcd_driver.chip_type = LCD_CHIP_TL1;
		lcd_driver.rev_type = rev_type;
		break;
	case MESON_CPU_MAJOR_ID_SM1:
		lcd_driver.chip_type = LCD_CHIP_SM1;
		break;
	case MESON_CPU_MAJOR_ID_TM2:
		lcd_driver.chip_type = LCD_CHIP_TM2;
		lcd_driver.rev_type = rev_type;
		break;
	case MESON_CPU_MAJOR_ID_T5:
		lcd_driver.chip_type = LCD_CHIP_T5;
		lcd_driver.rev_type = rev_type;
		break;
	case MESON_CPU_MAJOR_ID_T5D:
		lcd_driver.chip_type = LCD_CHIP_T5D;
		lcd_driver.rev_type = rev_type;
		break;
	case MESON_CPU_MAJOR_ID_T7:
		lcd_driver.chip_type = LCD_CHIP_T7;
		lcd_driver.rev_type = rev_type;
		break;
	default:
		lcd_driver.chip_type = LCD_CHIP_T7;
		break;
	}
#else
	lcd_driver.chip_type = LCD_CHIP_TM2;
#endif
	if (lcd_debug_print_flag)
		LCDPR("check chip: %d\n", lcd_driver.chip_type);
}

static int lcd_check_valid(void)
{
	if (!lcd_driver.config_check) {
		LCDERR("invalid lcd config\n");
		return -1;
	}
	return 0;
}

static void lcd_power_ctrl(int status)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();
	struct lcd_power_ctrl_s *lcd_power;
	struct lcd_power_step_s *power_step;
	char *str;
	unsigned int i, wait, gpio;
	int value = LCD_PMU_GPIO_NUM_MAX;
#ifdef CONFIG_AML_LCD_EXTERN
	struct lcd_extern_driver_s *ext_drv;
#endif
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
			LCDPR
	("power_ctrl: %d, step %d: type=%d, index=%d, value=%d, delay=%d\n",
	 status, i, power_step->type, power_step->index,
	 power_step->value, power_step->delay);
		}
		switch (power_step->type) {
		case LCD_POWER_TYPE_CPU:
			if (power_step->index < LCD_CPU_GPIO_NUM_MAX) {
				str = lcd_power->cpu_gpio[power_step->index];
				gpio = lcd_gpio_name_map_num(str);
				lcd_gpio_set(gpio, power_step->value);
			} else {
				LCDERR("cpu_gpio index: %d\n",
				       power_step->index);
			}
			break;
		case LCD_POWER_TYPE_PMU:
			if (power_step->index < LCD_PMU_GPIO_NUM_MAX)
				LCDPR("to do\n");
			else
				LCDERR("pmu_gpio index: %d\n",
				       power_step->index);
			break;
		case LCD_POWER_TYPE_SIGNAL:
			if (status)
				lcd_drv->driver_init();
			else
				lcd_drv->driver_disable();
			break;
#ifdef CONFIG_AML_LCD_EXTERN
		case LCD_POWER_TYPE_EXTERN:
			ext_drv = lcd_extern_get_driver(power_step->index);
			if (ext_drv) {
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
			}
			break;
#endif
		case LCD_POWER_TYPE_WAIT_GPIO:
			if (power_step->index < LCD_CPU_GPIO_NUM_MAX) {
				str = lcd_power->cpu_gpio[power_step->index];
				gpio = lcd_gpio_name_map_num(str);
				lcd_gpio_set(gpio, LCD_GPIO_INPUT);
			} else {
				LCDERR
				("wait_gpio index: %d\n", power_step->index);
				break;
			}
			LCDPR("lcd_power_type_wait_gpio wait\n");
			for (wait = 0; wait < power_step->delay; wait++) {
				value = lcd_gpio_input_get(gpio);
				if (value == power_step->value) {
					LCDPR
					("get value: %d, wait ok\n", value);
					break;
				}
				mdelay(1);
			}
			if (wait == power_step->delay)
				LCDERR
				("get value: %d, wait timeout!\n", value);
			break;
		case LCD_POWER_TYPE_CLK_SS:
			break;
#ifdef CONFIG_AML_LCD_TCON
		case LCD_POWER_TYPE_TCON_SPI_DATA_LOAD:
			if (lcd_drv->lcd_tcon_spi_data_load)
				lcd_drv->lcd_tcon_spi_data_load();
			break;
#endif
		default:
			break;
		}

		if (power_step->type != LCD_POWER_TYPE_WAIT_GPIO) {
#ifndef CONFIG_AML_LCD_PXP
			if (power_step->delay > 0)
				mdelay(power_step->delay);
#endif
		}
		i++;
		power_step++;
	}

	if (lcd_debug_print_flag)
		LCDPR("%s: %d\n", __func__, status);
}

static void lcd_gamma_init(void)
{
#ifndef CONFIG_AML_LCD_PXP
	lcd_wait_vsync();
	vpp_disable_lcd_gamma_table();

	vpp_init_lcd_gamma_table();

	lcd_wait_vsync();
	vpp_enable_lcd_gamma_table();
#endif
}

static void lcd_encl_on(void)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();

	lcd_drv->driver_init_pre();
	if (lcd_debug_test)
		aml_lcd_debug_test(lcd_debug_test);
	lcd_gamma_init();

	lcd_vcbus_write(VENC_INTCTRL, 0x200);
	lcd_drv->lcd_status |= LCD_STATUS_ENCL_ON;
}

static void lcd_interface_on(void)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();
	struct lcd_config_s *pconf = lcd_drv->lcd_config;

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
	lcd_drv->lcd_status |= LCD_STATUS_IF_ON;
}

static void lcd_backlight_enable(void)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();

	bl_pwm_config_update(lcd_drv->bl_config);
	bl_set_level(lcd_drv->bl_config->level_default);
	bl_power_ctrl(1, 1);
}

static void lcd_module_enable(char *mode, unsigned int frac)
{
	unsigned int sync_duration;
	struct lcd_drv_s *lcd_drv = lcd_get_driver();
	struct lcd_config_s *pconf = lcd_drv->lcd_config;
	int ret;

	ret = lcd_drv->config_check(mode, frac);
	if (ret) {
		LCDERR("init exit\n");
		return;
	}

	sync_duration = pconf->lcd_timing.sync_duration_num;
	sync_duration = (sync_duration * 100 /
			 pconf->lcd_timing.sync_duration_den);
	LCDPR("enable: %s, %s, %ux%u@%u.%02uHz\n", pconf->lcd_basic.model_name,
	      lcd_type_type_to_str(pconf->lcd_basic.lcd_type),
	      pconf->lcd_basic.h_active, pconf->lcd_basic.v_active,
	      (sync_duration / 100), (sync_duration % 100));

	if ((lcd_drv->lcd_status & LCD_STATUS_ENCL_ON) == 0)
		lcd_encl_on();
	if ((lcd_drv->lcd_status & LCD_STATUS_IF_ON) == 0) {
		if (boot_ctrl.lcd_init_level == LCD_INIT_LEVEL_NORMAL) {
			lcd_interface_on();
			lcd_backlight_enable();
		}
	}
	if (!lcd_debug_test)
		lcd_mute_setting(0);
}

static void lcd_module_disable(void)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();

	LCDPR("disable: %s\n", lcd_drv->lcd_config->lcd_basic.model_name);

	lcd_mute_setting(1);
	if (lcd_drv->lcd_status & LCD_STATUS_IF_ON) {
		bl_power_ctrl(0, 1);
		lcd_power_ctrl(0);
	}

	lcd_vcbus_write(ENCL_VIDEO_EN, 0);
	lcd_clk_disable();
	lcd_drv->lcd_status = 0;
}

static void lcd_module_prepare(char *mode, unsigned int frac)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();
	int ret;

	ret = lcd_drv->config_check(mode, frac);
	if (ret) {
		LCDERR("prepare exit\n");
		return;
	}

	if ((lcd_drv->lcd_status & LCD_STATUS_ENCL_ON) == 0)
		lcd_encl_on();
}

#ifdef CONFIG_AML_LCD_EXTERN
static int lcd_extern_load_config(char *dt_addr, struct lcd_config_s *pconf)
{
	struct lcd_power_step_s *power_step;
	int index, i = 0;

	lcd_extern_init();

	/* mipi extern_init is special */
	if (pconf->lcd_basic.lcd_type == LCD_MIPI) {
		index = pconf->lcd_control.mipi_config->extern_init;
		if (index < LCD_EXTERN_INDEX_INVALID)
			lcd_extern_probe(dt_addr, index);
	}

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
				lcd_extern_probe(dt_addr, index);
		}
		i++;
	}

	return 0;
}
#endif

#ifdef CONFIG_OF_LIBFDT
static int lcd_init_load_from_dts(char *dt_addr)
{
	struct lcd_config_s *pconf = lcd_driver.lcd_config;
	int parent_offset;
	char *propdata, *p;
	const char *str;
	int i, j, temp;

	/* check bl_key_valid */
	parent_offset = fdt_path_offset(dt_addr, "/backlight");
	if (parent_offset < 0) {
		LCDERR("not find /backlight node: %s\n", fdt_strerror(parent_offset));
		lcd_driver.bl_config->bl_key_valid = 0;
	}
	propdata = (char *)fdt_getprop(dt_addr, parent_offset, "key_valid", NULL);
	if (propdata == NULL) {
		LCDERR("failed to get key_valid\n");
		lcd_driver.bl_config->bl_key_valid = 0;
	} else {
		lcd_driver.bl_config->bl_key_valid = (unsigned char)(be32_to_cpup((u32*)propdata));
	}

	parent_offset = fdt_path_offset(dt_addr, "/lcd");
	if (parent_offset < 0) {
		LCDERR("not find /lcd node: %s\n", fdt_strerror(parent_offset));
		return -1;
	}

	/* check lcd_mode & lcd_key_valid */
	propdata = (char *)fdt_getprop(dt_addr, parent_offset, "mode", NULL);
	if (propdata == NULL) {
		LCDERR("failed to get mode\n");
		return -1;
	} else {
		pconf->lcd_mode = lcd_mode_str_to_mode(propdata);
	}
	str = propdata;
	propdata = (char *)fdt_getprop(dt_addr, parent_offset, "key_valid", NULL);
	if (propdata == NULL) {
		LCDERR("failed to get key_valid\n");
		pconf->lcd_key_valid = 0;
	} else {
		pconf->lcd_key_valid = (unsigned char)(be32_to_cpup((u32*)propdata));
	}
	LCDPR("detect mode: %s, key_valid: %d\n", str, pconf->lcd_key_valid);

	/* check lcd_clk_path */
	propdata = (char *)fdt_getprop(dt_addr, parent_offset,
				       "clk_path", NULL);
	if (!propdata) {
		if (lcd_debug_print_flag)
			LCDPR("failed to get clk_path\n");
		pconf->lcd_clk_path = 0;
	} else {
		pconf->lcd_clk_path =
			(unsigned char)(be32_to_cpup((u32 *)propdata));
		LCDPR("detect lcd_clk_path: %d\n", pconf->lcd_clk_path);
	}
	temp = env_get_ulong("lcd_clk_path", 10, 0xffff);
	if (temp != 0xffff) {
		if (temp)
			pconf->lcd_clk_path = 1;
		else
			pconf->lcd_clk_path = 0;
		LCDPR("lcd_clk_path flag set clk_path: %d\n",
		      pconf->lcd_clk_path);
	}

	i = 0;
	propdata = (char *)fdt_getprop(dt_addr, parent_offset,
				       "lcd_cpu_gpio_names", NULL);
	if (!propdata) {
		LCDPR("failed to get lcd_cpu_gpio_names\n");
	} else {
		p = propdata;
		while (i < LCD_CPU_GPIO_NUM_MAX) {
			str = p;
			if (strlen(str) == 0)
				break;
			strcpy(pconf->lcd_power->cpu_gpio[i], str);
			if (lcd_debug_print_flag) {
				LCDPR("i=%d, gpio=%s\n",
				      i, pconf->lcd_power->cpu_gpio[i]);
			}
			p += strlen(p) + 1;
			i++;
		}
	}
	for (j = i; j < LCD_CPU_GPIO_NUM_MAX; j++)
		strcpy(pconf->lcd_power->cpu_gpio[j], "invalid");

	return 0;
}
#endif

static int lcd_init_load_from_bsp(void)
{
	struct lcd_config_s *pconf = lcd_driver.lcd_config;
	int i, j, temp;
	char *str;

	/*
	 * pconf->lcd_key_valid = 0;
	 * lcd_driver.bl_config->bl_key_valid = 0;
	 */
	LCDPR("detect mode: %s, key_valid: %d\n",
	      lcd_mode_mode_to_str(pconf->lcd_mode), pconf->lcd_key_valid);

	i = 0;
	while (i < LCD_CPU_GPIO_NUM_MAX) {
		if (strcmp(pconf->lcd_power->cpu_gpio[i], "invalid") == 0)
			break;
		i++;
	}

	for (j = i; j < LCD_CPU_GPIO_NUM_MAX; j++) {
		strcpy(pconf->lcd_power->cpu_gpio[j], "invalid");
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

static int lcd_config_load_id_check(char *dt_addr, int load_id)
{
	int ret;

#ifdef CONFIG_OF_LIBFDT
	int parent_offset;

	if (fdt_check_header(dt_addr) < 0) {
		LCDERR
		("check dts: %s, load default lcd parameters\n",
		 fdt_strerror(fdt_check_header(dt_addr)));
	} else {
		parent_offset = fdt_path_offset(dt_addr, "/lcd");
		if (parent_offset < 0) {
			LCDERR
			("not find /lcd node: %s\n",
			 fdt_strerror(parent_offset));
			load_id = 0x0;
		} else {
			load_id = 0x1;
		}
	}
#endif

	switch (boot_ctrl.lcd_debug_para) {
	case 1:
		LCDPR("lcd_debug_para: 1,dts\n");
		load_id = 0x1;
		break;
	case 2:
		LCDPR("lcd_debug_para: 2,unifykey\n");
		break;
	case 3:
		LCDPR("lcd_debug_para: 3,bsp\n");
		load_id = 0x0;
		break;
	default:
		break;
	}

	if (load_id & 0x1) {
#ifdef CONFIG_OF_LIBFDT
		ret = lcd_init_load_from_dts(dt_addr);
		if (ret)
			return -1;
#endif
	} else {
		ret = lcd_init_load_from_bsp();
		if (ret)
			return -1;
	}

	if (boot_ctrl.lcd_debug_para == 1) {
		lcd_driver.bl_config->bl_key_valid = 0;
		lcd_driver.lcd_config->lcd_key_valid = 0;
	} else if (boot_ctrl.lcd_debug_para == 2) {
		lcd_driver.bl_config->bl_key_valid = 1;
		lcd_driver.lcd_config->lcd_key_valid = 1;
	}

	if (lcd_driver.unifykey_test_flag) {
		lcd_driver.bl_config->bl_key_valid = 1;
		lcd_driver.lcd_config->lcd_key_valid = 1;
		LCDPR("force bl_key_valid & lcd_key_valid to 1\n");
	}

	if (lcd_driver.lcd_config->lcd_key_valid) {
		ret = lcd_unifykey_check("lcd");
		if (ret == 0) {
			LCDPR("load lcd_config from unifykey\n");
			load_id |= 0x10;
			return load_id;
		}
	}

	if (load_id & 0x1)
		LCDPR("load config from dts\n");
	else
		LCDPR("load config from bsp\n");

	return load_id;
}

static int lcd_mode_probe(char *dt_addr, int load_id)
{
	int ret = 0;

	switch (boot_ctrl.lcd_debug_mode) {
	case 1:
		LCDPR("lcd_debug_mode: 1,tv\n");
		lcd_driver.lcd_config->lcd_mode = LCD_MODE_TV;
		break;
	case 2:
		LCDPR("lcd_debug_mode: 2,tablet\n");
		lcd_driver.lcd_config->lcd_mode = LCD_MODE_TABLET;
		break;
	default:
		break;
	}

	/* load lcd config */
	switch (lcd_driver.lcd_config->lcd_mode) {
#ifdef CONFIG_AML_LCD_TV
	case LCD_MODE_TV:
		ret = get_lcd_tv_config(dt_addr, load_id);
		break;
#endif
#ifdef CONFIG_AML_LCD_TABLET
	case LCD_MODE_TABLET:
		ret = get_lcd_tablet_config(dt_addr, load_id);
		break;
#endif
	default:
		LCDERR("invalid lcd mode: %d\n",
		       lcd_driver.lcd_config->lcd_mode);
		break;
	}

	if (ret) {
		lcd_driver.config_check = NULL;
		LCDERR("invalid lcd config\n");
		return -1;
	}

#ifdef CONFIG_AML_LCD_TCON
	lcd_tcon_probe(dt_addr, &lcd_driver, load_id);
#endif

#ifdef CONFIG_AML_LCD_EXTERN
	lcd_extern_load_config(dt_addr, lcd_driver.lcd_config);
#endif

	/* load bl config */
	if (lcd_driver.bl_config->bl_key_valid) {
		ret = lcd_unifykey_check("backlight");
		if (ret == 0) {
			LCDPR("load backlight_config from unifykey\n");
			load_id |= 0x10;
		} else {
			load_id &= ~(0x10);
		}
	} else {
		load_id &= ~(0x10);
	}
	bl_config_load(dt_addr, load_id);

	return 0;
}

static int lcd_config_probe(void)
{
	int load_id = 0;
	char *dt_addr;
	dt_addr = NULL;

#ifdef CONFIG_OF_LIBFDT
	#ifdef CONFIG_DTB_MEM_ADDR
		dt_addr = (char *)CONFIG_DTB_MEM_ADDR;
	#else
		dt_addr = (char *)0x01000000;
	#endif
#endif

	load_id = lcd_config_load_id_check(dt_addr, load_id);
	/* default setting */
	lcd_driver.lcd_config->retry_enable_flag = 0;
	lcd_driver.lcd_config->retry_enable_cnt = 0;

	lcd_phy_probe();
	lcd_clk_config_probe();
	lcd_mode_probe(dt_addr, load_id);
	lcd_debug_probe(&lcd_driver);

	return 0;
}

static void lcd_update_boot_ctrl_bootargs(void)
{
	unsigned int value = 0;
	char lcd_boot_ctrl[20];

	value |= lcd_driver.lcd_config->lcd_basic.lcd_type;
	switch (lcd_driver.lcd_config->lcd_basic.lcd_type) {
	case LCD_TTL:
		value |=
(lcd_driver.lcd_config->lcd_control.ttl_config->sync_valid & 0xff) << 8;
		break;
	case LCD_P2P:
		value |=
(lcd_driver.lcd_config->lcd_control.p2p_config->p2p_type & 0xff) << 8;
		break;
	default:
		break;
	}

	/*create new env "lcd_ctrl", define as below:
	 *bit[3:0]: lcd_type
	 *bit[7:4]: lcd bits
	 *bit[15:8]: advanced flag(p2p_type when lcd_type=p2p)
	 *bit[17:16]: reserved
	 *bit[19:18]: lcd_init_level
	 *high 12bit for debug flag
	 *bit[23:20]:  lcd debug print flag
	 *bit[27:24]: lcd test pattern
	 *bit[29:28]: lcd debug para source(0=normal, 1=dts, 2=unifykey,
	 *3=bsp for uboot)
	 *bit[31:30]: lcd mode(0=normal, 1=tv; 2=tablet, 3=TBD)
	 */
	value |= (lcd_driver.lcd_config->lcd_basic.lcd_bits & 0xf) << 4;
	value |= (boot_ctrl.lcd_init_level & 0x3) << 18;
	value |= (lcd_debug_print_flag & 0xf) << 20;
	value |= (lcd_debug_test & 0xf) << 24;
	value |= (boot_ctrl.lcd_debug_para & 0x3) << 28;
	value |= (boot_ctrl.lcd_debug_mode & 0x3) << 30;
	sprintf(lcd_boot_ctrl, "0x%08x", value);
	env_set("lcd_ctrl", lcd_boot_ctrl);
}

int lcd_probe(void)
{
	int ret = 0;

	lcd_debug_print_flag = env_get_ulong("lcd_debug_print", 10, 0);
	LCDPR("lcd_debug_print flag: %d\n", lcd_debug_print_flag);

	lcd_debug_test = env_get_ulong("lcd_debug_test", 10, 0);

	boot_ctrl.lcd_debug_para = env_get_ulong("lcd_debug_para", 10, 0);

	boot_ctrl.lcd_debug_mode = env_get_ulong("lcd_debug_mode", 10, 0);

	boot_ctrl.lcd_init_level = env_get_ulong("lcd_init_level", 10, 0);

	lcd_chip_detect();
	lcd_config_bsp_init();
	ret = lcd_config_probe();
	if (ret)
		return 0;

	lcd_update_boot_ctrl_bootargs();
	bl_power_ctrl(0, 0); /* init backlight ctrl port */
	mdelay(10);

	return 0;
}

int lcd_remove(void)
{
#ifdef CONFIG_AML_LCD_EXTERN
	lcd_extern_remove();
#endif

	return 0;
}

#define LCD_WAIT_VSYNC_TIMEOUT    50000
void lcd_wait_vsync(void)
{
#ifndef CONFIG_AML_LCD_PXP
	int line_cnt, line_cnt_previous;
	int i = 0;

	if (lcd_driver.chip_type == LCD_CHIP_T7)
		return;

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
#endif
}

/* ********************************************** *
  lcd driver API
 * ********************************************** */
static int lcd_outputmode_check(char *mode, unsigned int frac)
{
	if (lcd_driver.outputmode_check)
		return lcd_driver.outputmode_check(mode, frac);

	LCDERR("invalid lcd config\n");
	return -1;
}

static void lcd_prepare(char *mode, unsigned int frac)
{
	if (lcd_check_valid())
		return;
	if (lcd_driver.lcd_status & LCD_STATUS_ENCL_ON)
		LCDPR("already enabled\n");
	else
		lcd_module_prepare(mode, frac);
}

static void lcd_enable(char *mode, unsigned int frac)
{
	if (lcd_check_valid())
		return;

	if (lcd_driver.chip_type == LCD_CHIP_T7) {
		writel(0x6, VPU_VIU_VENC_MUX_CTRL);
		lcd_display_init_test();
		lcd_driver.lcd_status |= LCD_STATUS_ENCL_ON;
		lcd_driver.lcd_status |= LCD_STATUS_IF_ON;
		return;
	}
	if (lcd_driver.lcd_status & LCD_STATUS_IF_ON)
		LCDPR("already enabled\n");
	else
		lcd_module_enable(mode, frac);
}

static void lcd_disable(void)
{
	if (lcd_check_valid())
		return;
	if (lcd_driver.lcd_status & LCD_STATUS_ENCL_ON)
		lcd_module_disable();
	else
		LCDPR("already disabled\n");
}

static void aml_lcd_set_ss(unsigned int level, unsigned int freq,
			   unsigned int mode)
{
	struct lcd_drv_s *lcd_drv = lcd_get_driver();
	unsigned int temp;
	int ret;

	if (lcd_check_valid())
		return;
	if (lcd_driver.lcd_status) {
		temp = lcd_drv->lcd_config->lcd_timing.ss_level;
		ret = lcd_set_ss(level, freq, mode);
		if (ret == 0) {
			if (level < 0xff) {
				temp &= ~(0xff);
				temp |= level;
				lcd_drv->lcd_config->lcd_timing.ss_level = temp;
			}
			if (freq < 0xff) {
				temp &= ~((0xf << LCD_CLK_SS_BIT_FREQ) << 8);
				temp |= ((freq << LCD_CLK_SS_BIT_FREQ) << 8);
				lcd_drv->lcd_config->lcd_timing.ss_level = temp;
			}
			if (mode < 0xff) {
				temp &= ~((0xf << LCD_CLK_SS_BIT_MODE) << 8);
				temp |= ((mode << LCD_CLK_SS_BIT_MODE) << 8);
				lcd_drv->lcd_config->lcd_timing.ss_level = temp;
			}
		}
	} else {
		LCDPR("already disabled\n");
	}
}

static void aml_lcd_get_ss(void)
{
	if (lcd_check_valid())
		return;
	if (lcd_driver.lcd_status)
		lcd_get_ss();
	else
		LCDPR("already disabled\n");
}

static void lcd_test(int num)
{
	if (num >= 10) {
		lcd_display_init_test();
		return;
	}
	if (lcd_check_valid())
		return;
	if (lcd_driver.lcd_status)
		aml_lcd_debug_test(num);
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

static void lcd_vbyone_rst(void)
{
	if (lcd_check_valid())
		return;
	aml_lcd_vbyone_rst();
}

static void lcd_vbyone_cdr(void)
{
	if (lcd_check_valid())
		return;
	aml_lcd_vbyone_cdr();
}

static void set_backlight_level(int level)
{
	bl_set_level(level);
}

static int get_backlight_level(void)
{
	return bl_get_level();
}

static void backlight_power_on(void)
{
	bl_power_ctrl(1, 1);
}

static void backlight_power_off(void)
{
	bl_power_ctrl(0, 1);
}

static void lcd_key_test(void)
{
	if (lcd_driver.unifykey_test_flag) {
		lcd_unifykey_test();
		lcd_config_probe();
	} else {
		printf("lcd unifykey test disabled\n");
	}
}

static void lcd_key_dump(unsigned int flag)
{
	unsigned int key_flag = LCD_UKEY_DEBUG_NORMAL;

	if (flag & (1 << 0)) {
		key_flag = LCD_UKEY_DEBUG_NORMAL;
	} else if (flag & (1 << 1)) {
#ifdef CONFIG_AML_LCD_TCON
		switch (lcd_driver.chip_type) {
		case LCD_CHIP_TL1:
		case LCD_CHIP_TM2:
			key_flag = (LCD_UKEY_DEBUG_TCON |
				    LCD_UKEY_TCON_SIZE_NEW);
			break;
		default:
			break;
		}
#endif
	}
	lcd_unifykey_dump(key_flag);
}

static struct lcd_drv_s lcd_driver = {
	.lcd_status = 0,
	.lcd_config = &lcd_config_dft,
	.bl_config = &bl_config_dft,
	.config_check = NULL,
	.lcd_probe = lcd_probe,
	.lcd_outputmode_check = lcd_outputmode_check,
	.lcd_prepare = lcd_prepare,
	.lcd_enable = lcd_enable,
	.lcd_disable = lcd_disable,
	.lcd_set_ss = aml_lcd_set_ss,
	.lcd_get_ss = aml_lcd_get_ss,
	.lcd_test = lcd_test,
	.lcd_prbs = lcd_prbs_test,
	.lcd_clk = lcd_clk_info,
	.lcd_info = lcd_info,
	.lcd_reg = lcd_reg_info,
#ifdef CONFIG_AML_LCD_TCON
	.lcd_tcon_reg_print = NULL,
	.lcd_tcon_table_print = NULL,
	.lcd_tcon_vac_print = NULL,
	.lcd_tcon_demura_print = NULL,
	.lcd_tcon_acc_print = NULL,
	.lcd_tcon_data_print = NULL,
	.lcd_tcon_spi_print = NULL,
	.lcd_tcon_spi_data_load = NULL,
	.lcd_tcon_reg_read = NULL,
	.lcd_tcon_reg_write = NULL,
#endif
	.lcd_vbyone_rst = lcd_vbyone_rst,
	.lcd_vbyone_cdr = lcd_vbyone_cdr,
	.bl_on = backlight_power_on,
	.bl_off = backlight_power_off,
	.set_bl_level = set_backlight_level,
	.get_bl_level = get_backlight_level,
	.bl_config_print = bl_config_print,
	.unifykey_test_flag = 0, /* default disable unifykey test */
	.unifykey_test = lcd_key_test,
	.unifykey_dump = lcd_key_dump,

	/* for factory test */
	.factory_lcd_power_on_step = NULL,
	.factory_bl_power_on_delay = -1,
};

struct lcd_drv_s *lcd_get_driver(void)
{
	return &lcd_driver;
}
