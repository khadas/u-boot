// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <dm.h>
#include <asm/gpio.h>
#include <fdtdec.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#ifdef CONFIG_AML_LCD_BL_LDIM
#include <amlogic/media/vout/lcd/bl_ldim.h>
#endif
#ifdef CONFIG_AML_LCD_BL_EXTERN
#include <amlogic/media/vout/lcd/bl_extern.h>
#endif
#include "lcd_bl.h"
#include "../lcd_reg.h"
#include "../lcd_common.h"
#include "../lcd_unifykey.h"

static int bl_index_lut[LCD_MAX_DRV];
static struct aml_bl_drv_s *bl_driver[LCD_MAX_DRV];

struct aml_bl_drv_s *aml_bl_get_driver(int index)
{
	if (index >= LCD_MAX_DRV)
		return NULL;
	if (!bl_driver[index])
		return NULL;
	if (bl_driver[index]->config.method >= BL_CTRL_MAX)
		return NULL;

	return bl_driver[index];
}

static struct bl_config_s *bl_check_valid(struct aml_bl_drv_s *bdrv)
{
	struct bl_config_s *bconf;
	unsigned int bconf_flag = 1;
#ifdef CONFIG_AML_LCD_BL_EXTERN
	struct aml_bl_extern_driver_s *bl_ext;
#endif
#ifdef CONFIG_AML_LCD_BL_LDIM
	struct aml_ldim_driver_s *ldim_drv;
#endif

	if (!bdrv)
		return NULL;

	bconf = &bdrv->config;
	switch (bconf->method) {
	case BL_CTRL_PWM:
		if (!bconf->bl_pwm) {
			BLERR("no bl_pwm struct\n");
			bconf_flag = 0;
		}
		break;
	case BL_CTRL_PWM_COMBO:
		if (!bconf->bl_pwm_combo0) {
			BLERR("no bl_pwm_combo_0 struct\n");
			bconf_flag = 0;
		}
		if (!bconf->bl_pwm_combo1) {
			BLERR("no bl_pwm_combo_1 struct\n");
			bconf_flag = 0;
		}
		break;
	case BL_CTRL_GPIO:
		break;
#ifdef CONFIG_AML_LCD_BL_LDIM
	case BL_CTRL_LOCAL_DIMMING:
		if (bdrv->index > 0) {
			BLERR("no ldim driver\n");
			bconf_flag = 0;
			break;
		}
		ldim_drv = aml_ldim_get_driver();
		if (!ldim_drv) {
			BLERR("no ldim driver\n");
			bconf_flag = 0;
		}
		break;
#endif
#ifdef CONFIG_AML_LCD_BL_EXTERN
	case BL_CTRL_EXTERN:
		bl_ext = aml_bl_extern_get_driver();
		if (!bl_ext) {
			BLERR("no bl_extern driver\n");
			bconf_flag = 0;
		}
		break;
#endif
	default:
		if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
			BLPR("invalid control_method: %d\n", bconf->method);
		bconf_flag = 0;
		break;
	}

	if (!bconf_flag)
		bconf = NULL;

	return bconf;
}

static void bl_pwm_pinmux_gpio_set(struct aml_bl_drv_s *bdrv, int pwm_index, int gpio_level)
{
	struct bl_config_s *bconf;
	struct bl_pwm_config_s *bl_pwm = NULL;
	int gpio;
	char *str;
	int i;

	bconf = bl_check_valid(bdrv);
	if (!bconf)
		return;

	switch (bconf->method) {
	case BL_CTRL_PWM:
		bl_pwm = bconf->bl_pwm;
		break;
	case BL_CTRL_PWM_COMBO:
		if (pwm_index == 0)
			bl_pwm = bconf->bl_pwm_combo0;
		else
			bl_pwm = bconf->bl_pwm_combo1;
		break;
	default:
		BLERR("%s: invalid method %d\n", __func__, bconf->method);
		break;
	}
	if (!bl_pwm)
		return;

	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
		BLPR("%s: pwm_port=0x%x, pinmux_flag=%d\n",
			__func__, bl_pwm->pwm_port, bl_pwm->pinmux_flag);
	}
	if (bl_pwm->pinmux_flag > 0) {
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (bl_pwm->pinmux_set[i][0] == LCD_PINMUX_END)
				break;
			lcd_pinmux_clr_mask(bl_pwm->pinmux_set[i][0],
				bl_pwm->pinmux_set[i][1]);
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
			BLPR("%s: port=0x%x, pinmux_clr=%d,0x%08x\n",
				__func__, bl_pwm->pwm_port,
				bl_pwm->pinmux_set[i][0], bl_pwm->pinmux_set[i][1]);
			}
			i++;
		}
		bl_pwm->pinmux_flag = 0;
	}
	/* set gpio */
	if (bl_pwm->pwm_gpio >= BL_GPIO_NUM_MAX) {
		gpio = LCD_GPIO_MAX;
	} else {
		str = bconf->gpio_name[bl_pwm->pwm_gpio];
		gpio = lcd_gpio_name_map_num(str);
	}
	if (gpio < LCD_GPIO_MAX)
		lcd_gpio_set(gpio, gpio_level);
}

static void bl_pwm_pinmux_gpio_clr(struct aml_bl_drv_s *bdrv, unsigned int pwm_index)
{
	struct bl_config_s *bconf;
	struct bl_pwm_config_s *bl_pwm = NULL;
	int i;

	bconf = bl_check_valid(bdrv);
	if (!bconf)
		return;

	switch (bconf->method) {
	case BL_CTRL_PWM:
		bl_pwm = bconf->bl_pwm;
		break;
	case BL_CTRL_PWM_COMBO:
		if (pwm_index == 0)
			bl_pwm = bconf->bl_pwm_combo0;
		else
			bl_pwm = bconf->bl_pwm_combo1;
		break;
	default:
		BLERR("%s: invalid method %d\n", __func__, bconf->method);
		break;
	}
	if (!bl_pwm)
		return;

	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
		BLPR("%s: pwm_port=0x%x, pinmux_flag=%d\n",
			__func__, bl_pwm->pwm_port, bl_pwm->pinmux_flag);
	}
	if (bl_pwm->pinmux_flag > 0)
		return;

	/* set pinmux */
	i = 0;
	while (i < LCD_PINMUX_NUM) {
		if (bl_pwm->pinmux_set[i][0] == LCD_PINMUX_END)
			break;
		lcd_pinmux_set_mask(bl_pwm->pinmux_set[i][0],
			bl_pwm->pinmux_set[i][1]);
		if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
			BLPR("%s: port=0x%x, pinmux_set=%d,0x%08x\n",
				__func__, bl_pwm->pwm_port,
				bl_pwm->pinmux_set[i][0], bl_pwm->pinmux_set[i][1]);
		}
		i++;
	}
	bl_pwm->pinmux_flag = 1;
}

void bl_set_pwm_gpio_check(struct aml_bl_drv_s *bdrv, struct bl_pwm_config_s *bl_pwm)
{
	unsigned int pwm_index, gpio_level;

	pwm_index = bl_pwm->index;

	/* pwm duty 100% or 0% special control */
	if (bl_pwm->pwm_duty_max > 255) {
		if (bl_pwm->pwm_duty == 0 || bl_pwm->pwm_duty == 4095) {
			switch (bl_pwm->pwm_method) {
			case BL_PWM_POSITIVE:
				if (bl_pwm->pwm_duty == 0)
					gpio_level = 0;
				else
					gpio_level = 1;
				break;
			case BL_PWM_NEGATIVE:
				if (bl_pwm->pwm_duty == 0)
					gpio_level = 1;
				else
					gpio_level = 0;
				break;
			default:
				BLERR("%s: port=0x%x: invalid pwm_method %d\n",
				      __func__, bl_pwm->pwm_port,
				      bl_pwm->pwm_method);
				gpio_level = 0;
				break;
			}
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
				BLPR("%s: pwm port=0x%x, duty=%d%%, switch to gpio %d\n",
				     __func__, bl_pwm->pwm_port,
				     bl_pwm->pwm_duty * 100 / 4095, gpio_level);
			}
			bl_pwm_pinmux_gpio_set(bdrv, pwm_index, gpio_level);
		} else {
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
				BLPR("%s: pwm_port=0x%x set as pwm\n",
				      __func__, bl_pwm->pwm_port);
			}
			bl_pwm_pinmux_gpio_clr(bdrv, pwm_index);
		}
	} else if (bl_pwm->pwm_duty_max > 100) {
		if (bl_pwm->pwm_duty == 0 || bl_pwm->pwm_duty == 255) {
			switch (bl_pwm->pwm_method) {
			case BL_PWM_POSITIVE:
				if (bl_pwm->pwm_duty == 0)
					gpio_level = 0;
				else
					gpio_level = 1;
				break;
			case BL_PWM_NEGATIVE:
				if (bl_pwm->pwm_duty == 0)
					gpio_level = 1;
				else
					gpio_level = 0;
				break;
			default:
				BLERR("%s: port=0x%x: invalid pwm_method %d\n",
				      __func__, bl_pwm->pwm_port,
				      bl_pwm->pwm_method);
				gpio_level = 0;
				break;
			}
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
				BLPR("%s: pwm port=0x%x, duty=%d%%, switch to gpio %d\n",
				     __func__, bl_pwm->pwm_port,
				     bl_pwm->pwm_duty * 100 / 255, gpio_level);
			}
			bl_pwm_pinmux_gpio_set(bdrv, pwm_index, gpio_level);
		} else {
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
				BLPR("%s: pwm_port=0x%x set as pwm\n",
				      __func__, bl_pwm->pwm_port);
			}
			bl_pwm_pinmux_gpio_clr(bdrv, pwm_index);
		}
	} else {
		if (bl_pwm->pwm_duty == 0 || bl_pwm->pwm_duty == 100) {
			switch (bl_pwm->pwm_method) {
			case BL_PWM_POSITIVE:
				if (bl_pwm->pwm_duty == 0)
					gpio_level = 0;
				else
					gpio_level = 1;
				break;
			case BL_PWM_NEGATIVE:
				if (bl_pwm->pwm_duty == 0)
					gpio_level = 1;
				else
					gpio_level = 0;
				break;
			default:
				BLERR("%s: port=0x%x: invalid pwm_method %d\n",
				       __func__, bl_pwm->pwm_port, bl_pwm->pwm_method);
				gpio_level = 0;
				break;
			}
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
				BLPR("%s: pwm port=0x%x, duty=%d%%, switch to gpio %d\n",
				     __func__, bl_pwm->pwm_port, bl_pwm->pwm_duty,
				     gpio_level);
			}
			bl_pwm_pinmux_gpio_set(bdrv, pwm_index, gpio_level);
		} else {
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
				BLPR("%s: pwm_port=0x%x set as pwm\n",
				      __func__, bl_pwm->pwm_port);
			}
			bl_pwm_pinmux_gpio_clr(bdrv, pwm_index);
		}
	}
}

static void bl_pwm_pinmux_ctrl(struct aml_bl_drv_s *bdrv, int status)
{
	struct bl_config_s *bconf = &bdrv->config;
	int gpio;
	char *str;
	int i;

	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		BLPR("%s: %d\n", __func__, status);
	if (status) {
		/* set pinmux */
		switch (bconf->method) {
		case BL_CTRL_PWM:
			bl_set_pwm_gpio_check(bdrv, bconf->bl_pwm);
			break;
		case BL_CTRL_PWM_COMBO:
			bl_set_pwm_gpio_check(bdrv, bconf->bl_pwm_combo0);
			bl_set_pwm_gpio_check(bdrv, bconf->bl_pwm_combo1);
			break;
		default:
			break;
		}
	} else {
		switch (bconf->method) {
		case BL_CTRL_PWM:
			i = 0;
			while (i < LCD_PINMUX_NUM) {
				if (bconf->bl_pwm->pinmux_set[i][0] == LCD_PINMUX_END)
					break;
				lcd_pinmux_clr_mask(bconf->bl_pwm->pinmux_set[i][0],
					bconf->bl_pwm->pinmux_set[i][1]);
				if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
					BLPR("%s: port=0x%x, pinmux_clr=%d,0x%08x\n",
						__func__, bconf->bl_pwm->pwm_port,
						bconf->bl_pwm->pinmux_set[i][0],
						bconf->bl_pwm->pinmux_set[i][1]);
				}
				i++;
			}
			bconf->bl_pwm->pinmux_flag = 0;

			if (bconf->bl_pwm->pwm_gpio >= BL_GPIO_NUM_MAX) {
				gpio = LCD_GPIO_MAX;
			} else {
				str = bconf->gpio_name[bconf->bl_pwm->pwm_gpio];
				gpio = lcd_gpio_name_map_num(str);
			}
			if (gpio < LCD_GPIO_MAX)
				lcd_gpio_set(gpio, bconf->bl_pwm->pwm_gpio_off);
			break;
		case BL_CTRL_PWM_COMBO:
			i = 0;
			while (i < LCD_PINMUX_NUM) {
				if (bconf->bl_pwm_combo0->pinmux_set[i][0] == LCD_PINMUX_END)
					break;
				lcd_pinmux_clr_mask(bconf->bl_pwm_combo0->pinmux_set[i][0],
					bconf->bl_pwm_combo0->pinmux_set[i][1]);
				if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
					BLPR("%s: port=0x%x, pinmux_clr=%d,0x%08x\n",
						__func__, bconf->bl_pwm_combo0->pwm_port,
						bconf->bl_pwm_combo0->pinmux_set[i][0],
						bconf->bl_pwm_combo0->pinmux_set[i][1]);
				}
				i++;
			}
			i = 0;
			while (i < LCD_PINMUX_NUM) {
				if (bconf->bl_pwm_combo1->pinmux_set[i][0] == LCD_PINMUX_END)
					break;
				lcd_pinmux_clr_mask(bconf->bl_pwm_combo1->pinmux_set[i][0],
					bconf->bl_pwm_combo1->pinmux_set[i][1]);
				if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
					BLPR("%s: port=0x%x, pinmux_clr=%d,0x%08x\n",
						__func__, bconf->bl_pwm_combo1->pwm_port,
						bconf->bl_pwm_combo1->pinmux_set[i][0],
						bconf->bl_pwm_combo1->pinmux_set[i][1]);
				}
				i++;
			}
			bconf->bl_pwm_combo0->pinmux_flag = 0;
			bconf->bl_pwm_combo1->pinmux_flag = 0;

			if (bconf->bl_pwm_combo0->pwm_gpio >= BL_GPIO_NUM_MAX) {
				gpio = LCD_GPIO_MAX;
			} else {
				str = bconf->gpio_name[bconf->bl_pwm_combo0->pwm_gpio];
				gpio = lcd_gpio_name_map_num(str);
			}
			if (gpio < LCD_GPIO_MAX)
				lcd_gpio_set(gpio, bconf->bl_pwm_combo0->pwm_gpio_off);
			if (bconf->bl_pwm_combo1->pwm_gpio >= BL_GPIO_NUM_MAX) {
				gpio = LCD_GPIO_MAX;
			} else {
				str = bconf->gpio_name[bconf->bl_pwm_combo1->pwm_gpio];
				gpio = lcd_gpio_name_map_num(str);
			}
			if (gpio < LCD_GPIO_MAX)
				lcd_gpio_set(gpio, bconf->bl_pwm_combo1->pwm_gpio_off);
			break;
		default:
			break;
		}
	}
}

static void bl_pwm_config_update(struct aml_bl_drv_s *bdrv)
{
#ifdef CONFIG_AML_LCD_BL_LDIM
	struct aml_ldim_driver_s *ldim_drv;
#endif

	switch (bdrv->config.method) {
	case BL_CTRL_PWM:
		bl_pwm_config_init(bdrv->config.bl_pwm);
		break;
	case BL_CTRL_PWM_COMBO:
		bl_pwm_config_init(bdrv->config.bl_pwm_combo0);
		bl_pwm_config_init(bdrv->config.bl_pwm_combo1);
		break;
#ifdef CONFIG_AML_LCD_BL_LDIM
	case BL_CTRL_LOCAL_DIMMING:
		if (bdrv->index > 0) {
			BLERR("no ldim driver\n");
			break;
		}
		ldim_drv = aml_ldim_get_driver();
		if (!ldim_drv || !ldim_drv->dev_drv) {
			BLERR("ldim_drv or dev_drv is null\n");
			break;
		}
		if (ldim_drv->dev_drv->ldim_pwm_config.pwm_port >= BL_PWM_MAX)
			break;
		bl_pwm_config_init(&ldim_drv->dev_drv->ldim_pwm_config);
		if (ldim_drv->dev_drv->analog_pwm_config.pwm_port < BL_PWM_VS)
			bl_pwm_config_init(&ldim_drv->dev_drv->analog_pwm_config);
		break;
#endif
	default:
		break;
	}
}

static void bl_set_level(struct aml_bl_drv_s *bdrv, unsigned int level)
{
	struct bl_config_s *bconf;
	struct bl_pwm_config_s *pwm0, *pwm1;
#ifdef CONFIG_AML_LCD_BL_EXTERN
	struct aml_bl_extern_driver_s *bl_ext;
#endif
#ifdef CONFIG_AML_LCD_BL_LDIM
	struct aml_ldim_driver_s *ldim_drv;
#endif

	bconf = bl_check_valid(bdrv);
	if (!bconf)
		return;

	BLPR("set level: %u, last level: %u\n", level, bdrv->level);
	/* level range check */
	level = (level > bconf->level_max ? bconf->level_max :
			(level < bconf->level_min ? bconf->level_min : level));
	bdrv->level = level;

	switch (bconf->method) {
	case BL_CTRL_GPIO:
		break;
	case BL_CTRL_PWM:
		bl_pwm_set_level(bdrv, bconf->bl_pwm, level);
		break;
	case BL_CTRL_PWM_COMBO:
		pwm0 = bconf->bl_pwm_combo0;
		pwm1 = bconf->bl_pwm_combo1;

		if (level >= pwm0->level_max) {
			bl_pwm_set_level(bdrv, pwm0, pwm0->level_max);
		} else if ((level > pwm0->level_min) &&
			(level < pwm0->level_max)) {
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
				BLPR("pwm0 region, level=%u\n", level);
			bl_pwm_set_level(bdrv, pwm0, level);
		} else {
			bl_pwm_set_level(bdrv, pwm0, pwm0->level_min);
		}

		if (level >= pwm1->level_max) {
			bl_pwm_set_level(bdrv, pwm1, pwm1->level_max);
		} else if ((level > pwm1->level_min) &&
			(level < pwm1->level_max)) {
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
				BLPR("pwm1 region, level=%u\n", level);
			bl_pwm_set_level(bdrv, pwm1, level);
		} else {
			bl_pwm_set_level(bdrv, pwm1, pwm1->level_min);
		}
		break;
#ifdef CONFIG_AML_LCD_BL_LDIM
	case BL_CTRL_LOCAL_DIMMING:
		if (bdrv->index > 0) {
			BLERR("no ldim driver\n");
			break;
		}
		ldim_drv = aml_ldim_get_driver();
		if (ldim_drv->set_level)
			ldim_drv->set_level(ldim_drv, level);
		else
			BLERR("ldim set_level is null\n");
		break;
#endif
#ifdef CONFIG_AML_LCD_BL_EXTERN
	case BL_CTRL_EXTERN:
		bl_ext = aml_bl_extern_get_driver();
		if (bl_ext->set_level)
			bl_ext->set_level(level);
		else
			BLERR("bl_extern set_level is null\n");
		break;
#endif
	default:
		if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
			BLERR("wrong backlight control method\n");
		break;
	}
}

static void bl_power_en_ctrl(struct bl_config_s *bconf, int status)
{
	int gpio;
	char *str;

	if (bconf->en_gpio >= BL_GPIO_NUM_MAX) {
		gpio = LCD_GPIO_MAX;
	} else {
		str = bconf->gpio_name[bconf->en_gpio];
		gpio = lcd_gpio_name_map_num(str);
	}
	if (status) {
		if (gpio < LCD_GPIO_MAX)
			lcd_gpio_set(gpio, bconf->en_gpio_on);
	} else {
		if (gpio < LCD_GPIO_MAX)
			lcd_gpio_set(gpio, bconf->en_gpio_off);
	}
}

static void bl_power_ctrl(struct aml_bl_drv_s *bdrv, int status)
{
	int gpio, value;
	struct bl_config_s *bconf;
#ifdef CONFIG_AML_LCD_BL_EXTERN
	struct aml_bl_extern_driver_s *bl_ext;
#endif
#ifdef CONFIG_AML_LCD_BL_LDIM
	struct aml_ldim_driver_s *ldim_drv;
#endif

	bconf = bl_check_valid(bdrv);
	if (!bconf)
		return;

	gpio = bconf->en_gpio;
	value = status ? bconf->en_gpio_on : bconf->en_gpio_off;
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		BLPR("status=%d gpio=%d value=%d\n", status, gpio, value);

	if (status) {
		/* bl_off_policy */
		if (bdrv->bl_off_policy != BL_OFF_POLICY_NONE) {
			BLPR("bl_off_policy=%d for bl_off\n", bdrv->bl_off_policy);
			return;
		}

		bdrv->state = 1;
		/* check if factory test */
		if (bdrv->factory_bl_on_delay >= 0) {
			BLPR("%s: factory test power_on_delay!\n", __func__);
			if (bdrv->factory_bl_on_delay > 0)
				mdelay(bdrv->factory_bl_on_delay);
		} else {
			if (bconf->power_on_delay > 0)
				mdelay(bconf->power_on_delay);
		}

		switch (bconf->method) {
		case BL_CTRL_GPIO:
			bl_power_en_ctrl(bconf, 1);
			break;
		case BL_CTRL_PWM:
			if (bconf->en_sequence_reverse) {
				/* step 1: power on enable */
				bl_power_en_ctrl(bconf, 1);
				if (bconf->pwm_on_delay > 0)
					mdelay(bconf->pwm_on_delay);
				/* step 2: power on pwm */
				bl_pwm_en(bconf->bl_pwm, 1);
				bl_pwm_pinmux_ctrl(bdrv, 1);
			} else {
				/* step 1: power on pwm */
				bl_pwm_en(bconf->bl_pwm, 1);
				bl_pwm_pinmux_ctrl(bdrv, 1);
				if (bconf->pwm_on_delay > 0)
					mdelay(bconf->pwm_on_delay);
				/* step 2: power on enable */
				bl_power_en_ctrl(bconf, 1);
			}
			break;
		case BL_CTRL_PWM_COMBO:
			if (bconf->en_sequence_reverse) {
				/* step 1: power on enable */
				bl_power_en_ctrl(bconf, 1);
				if (bconf->pwm_on_delay > 0)
					mdelay(bconf->pwm_on_delay);
				/* step 2: power on pwm_combo */
				bl_pwm_en(bconf->bl_pwm_combo0, 1);
				bl_pwm_en(bconf->bl_pwm_combo1, 1);
				bl_pwm_pinmux_ctrl(bdrv, 1);
			} else {
				/* step 1: power on pwm_combo */
				bl_pwm_en(bconf->bl_pwm_combo0, 1);
				bl_pwm_en(bconf->bl_pwm_combo1, 1);
				bl_pwm_pinmux_ctrl(bdrv, 1);
				if (bconf->pwm_on_delay > 0)
					mdelay(bconf->pwm_on_delay);
				/* step 2: power on enable */
				bl_power_en_ctrl(bconf, 1);
			}
			break;
#ifdef CONFIG_AML_LCD_BL_LDIM
		case BL_CTRL_LOCAL_DIMMING:
			if (bdrv->index > 0) {
				BLERR("no ldim driver\n");
				break;
			}
			ldim_drv = aml_ldim_get_driver();
			if (bconf->en_sequence_reverse) {
				/* step 1: power on enable */
				bl_power_en_ctrl(bconf, 1);
				/* step 2: power on ldim */
				if (ldim_drv->power_on)
					ldim_drv->power_on(ldim_drv);
				else
					BLERR("ldim power on is null\n");
			} else {
				/* step 1: power on ldim */
				if (ldim_drv->power_on)
					ldim_drv->power_on(ldim_drv);
				else
					BLERR("ldim power on is null\n");
				/* step 2: power on enable */
				bl_power_en_ctrl(bconf, 1);
			}
			break;
#endif
#ifdef CONFIG_AML_LCD_BL_EXTERN
		case BL_CTRL_EXTERN:
			bl_ext = aml_bl_extern_get_driver();
			if (bconf->en_sequence_reverse) {
				/* step 1: power on enable */
				bl_power_en_ctrl(bconf, 1);
				/* step 2: power on bl_extern */
				if (bl_ext->power_on)
					bl_ext->power_on();
				else
					BLERR("bl_extern power on is null\n");
			} else {
				/* step 1: power on bl_extern */
				if (bl_ext->power_on)
					bl_ext->power_on();
				else
					BLERR("bl_extern power on is null\n");
				/* step 2: power on enable */
				bl_power_en_ctrl(bconf, 1);
			}
			break;
#endif
		default:
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
				BLERR("wrong backlight control method\n");
			break;
		}
	} else {
		bdrv->state = 0;
		switch (bconf->method) {
		case BL_CTRL_GPIO:
			bl_power_en_ctrl(bconf, 0);
			break;
		case BL_CTRL_PWM:
			if (bconf->en_sequence_reverse == 1) {
				/* step 1: power off pwm */
				bl_pwm_pinmux_ctrl(bdrv, 0);
				bl_pwm_en(bconf->bl_pwm, 0);
				if (bconf->pwm_off_delay > 0)
					mdelay(bconf->pwm_off_delay);
				/* step 2: power off enable */
				bl_power_en_ctrl(bconf, 0);
			} else {
				/* step 1: power off enable */
				bl_power_en_ctrl(bconf, 0);
				/* step 2: power off pwm */
				if (bconf->pwm_off_delay > 0)
					mdelay(bconf->pwm_off_delay);
				bl_pwm_pinmux_ctrl(bdrv, 0);
				bl_pwm_en(bconf->bl_pwm, 0);
			}
			break;
		case BL_CTRL_PWM_COMBO:
			if (bconf->en_sequence_reverse == 1) {
				/* step 1: power off pwm_combo */
				bl_pwm_pinmux_ctrl(bdrv, 0);
				bl_pwm_en(bconf->bl_pwm_combo0, 0);
				bl_pwm_en(bconf->bl_pwm_combo1, 0);
				if (bconf->pwm_off_delay > 0)
					mdelay(bconf->pwm_off_delay);
				/* step 2: power off enable */
				bl_power_en_ctrl(bconf, 0);
			} else {
				/* step 1: power off enable */
				bl_power_en_ctrl(bconf, 0);
				/* step 2: power off pwm_combo */
				if (bconf->pwm_off_delay > 0)
					mdelay(bconf->pwm_off_delay);
				bl_pwm_pinmux_ctrl(bdrv, 0);
				bl_pwm_en(bconf->bl_pwm_combo0, 0);
				bl_pwm_en(bconf->bl_pwm_combo1, 0);
			}
			break;
#ifdef CONFIG_AML_LCD_BL_LDIM
		case BL_CTRL_LOCAL_DIMMING:
			if (bdrv->index > 0) {
				BLERR("no ldim driver\n");
				break;
			}
			ldim_drv = aml_ldim_get_driver();
			if (bconf->en_sequence_reverse == 1) {
				/* step 1: power off ldim */
				if (ldim_drv->power_off)
					ldim_drv->power_off(ldim_drv);
				else
					BLERR("ldim power off is null\n");
				/* step 2: power off enable */
				bl_power_en_ctrl(bconf, 0);
			} else {
				/* step 1: power off enable */
				bl_power_en_ctrl(bconf, 0);
				/* step 2: power off ldim */
				if (ldim_drv->power_off)
					ldim_drv->power_off(ldim_drv);
				else
					BLERR("ldim power off is null\n");
			}
			break;
#endif
#ifdef CONFIG_AML_LCD_BL_EXTERN
		case BL_CTRL_EXTERN:
			bl_ext = aml_bl_extern_get_driver();
			if (bconf->en_sequence_reverse == 1) {
				/* step 1: power off bl_extern */
				if (bl_ext->power_off)
					bl_ext->power_off();
				else
					BLERR("bl_extern: power off is null\n");
				/* step 2: power off enable */
				bl_power_en_ctrl(bconf, 0);
			} else {
				/* step 1: power off enable */
				bl_power_en_ctrl(bconf, 0);
				/* step 2: power off bl_extern */
				if (bl_ext->power_off)
					bl_ext->power_off();
				else
					BLERR("bl_extern: power off is null\n");
			}
			break;
#endif
		default:
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
				BLERR("wrong backlight control method\n");
			break;
		}
		if (bconf->power_off_delay > 0)
			mdelay(bconf->power_off_delay);
	}
	BLPR("%s: %d\n", __func__, status);
}

static void bl_power_init_off(struct aml_bl_drv_s *bdrv)
{
	struct bl_config_s *bconf;

	bconf = bl_check_valid(bdrv);
	if (!bconf)
		return;

	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
		BLPR("[%d]: init_off: gpio=%d value=%d\n",
		      bdrv->index,
		      bconf->en_gpio, bconf->en_gpio_off);
	}

	bdrv->state = 0;
	switch (bconf->method) {
	case BL_CTRL_PWM:
	case BL_CTRL_PWM_COMBO:
		bl_power_en_ctrl(bconf, 0);
		bl_pwm_pinmux_ctrl(bdrv, 0);
		break;
	default:
		bl_power_en_ctrl(bconf, 0);
		break;
	}

	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		BLPR("%s finish\n", __func__);
}

static void bl_config_print(struct aml_bl_drv_s *bdrv)
{
	struct bl_config_s *bconf = &bdrv->config;
	struct bl_pwm_config_s *bl_pwm;
#ifdef CONFIG_AML_LCD_BL_LDIM
	struct aml_ldim_driver_s *ldim_drv;
#endif
#ifdef CONFIG_AML_LCD_BL_EXTERN
	struct aml_bl_extern_driver_s *bl_extern = aml_bl_extern_get_driver();
#endif

	BLPR("drv_index: %d\n", bdrv->index);
	BLPR("key_valid: %d\n", bdrv->key_valid);
	BLPR("state    : 0x%x\n", bdrv->state);
	BLPR("bl_off_policy: %d\n", bdrv->bl_off_policy);

	BLPR("name: %s\n", bconf->name);
	BLPR("method: %d\n", bconf->method);

	BLPR("level_default     = %d\n", bconf->level_default);
	BLPR("level_min         = %d\n", bconf->level_min);
	BLPR("level_max         = %d\n", bconf->level_max);
	BLPR("level_mid         = %d\n", bconf->level_mid);
	BLPR("level_mid_mapping = %d\n", bconf->level_mid_mapping);

	BLPR("en_gpio           = %s(%d)\n",
	     bconf->gpio_name[bconf->en_gpio], bconf->en_gpio);
	BLPR("en_gpio_on        = %d\n", bconf->en_gpio_on);
	BLPR("en_gpio_off       = %d\n", bconf->en_gpio_off);
	/* check if factory test */
	if (bdrv->factory_bl_on_delay >= 0)
		BLPR("factory test power_on_delay    = %d\n", bdrv->factory_bl_on_delay);
	else
		BLPR("power_on_delay    = %d\n", bconf->power_on_delay);
	BLPR("power_off_delay   = %d\n\n", bconf->power_off_delay);
	switch (bconf->method) {
	case BL_CTRL_PWM:
		if (bconf->bl_pwm) {
			bl_pwm = bconf->bl_pwm;
			BLPR("pwm_index     = %d\n", bl_pwm->index);
			BLPR("pwm_method    = %d\n", bl_pwm->pwm_method);
			BLPR("pwm_port      = %s(0x%x)\n",
			     bl_pwm_num_to_str(bl_pwm->pwm_port), bl_pwm->pwm_port);
			if (bl_pwm->pwm_port == BL_PWM_VS) {
				BLPR("pwm_freq      = %d x vfreq\n", bl_pwm->pwm_freq);
				BLPR("pwm_cnt       = %u\n", bl_pwm->pwm_cnt);
				if (bl_pwm->pwm_duty_max > 100) {
					BLPR("pwm_duty  = %d%%(%d)\n",
					     bl_pwm->pwm_duty * 100 / 255, bl_pwm->pwm_duty);
				} else {
					BLPR("pwm_duty  = %d%%(%d)\n",
					     bl_pwm->pwm_duty, bl_pwm->pwm_duty);
				}

				BLPR("pwm_reg0      = 0x%08x\n", lcd_vcbus_read(VPU_VPU_PWM_V0));
				BLPR("pwm_reg1      = 0x%08x\n", lcd_vcbus_read(VPU_VPU_PWM_V1));
				BLPR("pwm_reg2      = 0x%08x\n", lcd_vcbus_read(VPU_VPU_PWM_V2));
				BLPR("pwm_reg3      = 0x%08x\n", lcd_vcbus_read(VPU_VPU_PWM_V3));
			} else {
				BLPR("pwm_freq      = %uHz\n", bl_pwm->pwm_freq);
				BLPR("pwm_cnt       = %u\n", bl_pwm->pwm_cnt);
				BLPR("pwm_pre_div   = %u\n", bl_pwm->pwm_pre_div);
				if (bl_pwm->pwm_duty_max > 100) {
					BLPR("pwm_duty      = %d%%(%d)\n",
					     bl_pwm->pwm_duty * 100 / 255, bl_pwm->pwm_duty);
				} else {
					BLPR("pwm_duty      = %d%%(%d)\n",
					     bl_pwm->pwm_duty, bl_pwm->pwm_duty);
				}

				bl_pwm_reg_print(bl_pwm);
			}
			BLPR("pwm_duty_max  = %d\n", bl_pwm->pwm_duty_max);
			BLPR("pwm_duty_min  = %d\n", bl_pwm->pwm_duty_min);
			BLPR("pwm_gpio      = %s(%d)\n",
			     bconf->gpio_name[bl_pwm->pwm_gpio], bl_pwm->pwm_gpio);
			BLPR("pwm_gpio_off  = %d\n", bl_pwm->pwm_gpio_off);
		}
		BLPR("pwm_on_delay  = %d\n", bconf->pwm_on_delay);
		BLPR("pwm_off_delay = %d\n", bconf->pwm_off_delay);
		BLPR("en_sequence_reverse = %d\n", bconf->en_sequence_reverse);
		break;
	case BL_CTRL_PWM_COMBO:
		if (bconf->bl_pwm_combo0) {
			bl_pwm = bconf->bl_pwm_combo0;
			BLPR("pwm_combo0_index    = %d\n", bl_pwm->index);
			BLPR("pwm_combo0_method   = %d\n", bl_pwm->pwm_method);
			BLPR("pwm_combo0_port     = %s(0x%x)\n",
			     bl_pwm_num_to_str(bl_pwm->pwm_port), bl_pwm->pwm_port);
			if (bl_pwm->pwm_port == BL_PWM_VS) {
				BLPR("pwm_combo0_freq     = %d x vfreq\n", bl_pwm->pwm_freq);
				BLPR("pwm_combo0_cnt      = %u\n", bl_pwm->pwm_cnt);
				if (bl_pwm->pwm_duty_max > 100)
					BLPR("pwm_combo0_duty = %d%%(%d)\n",
					     bl_pwm->pwm_duty * 100 / 255, bl_pwm->pwm_duty);
				else
					BLPR("pwm_combo0_duty = %d%%(%d)\n",
					     bl_pwm->pwm_duty, bl_pwm->pwm_duty);

				BLPR("pwm_combo0_reg0     = 0x%08x\n", lcd_vcbus_read(VPU_VPU_PWM_V0));
				BLPR("pwm_combo0_reg1     = 0x%08x\n", lcd_vcbus_read(VPU_VPU_PWM_V1));
				BLPR("pwm_combo0_reg2     = 0x%08x\n", lcd_vcbus_read(VPU_VPU_PWM_V2));
				BLPR("pwm_combo0_reg3     = 0x%08x\n", lcd_vcbus_read(VPU_VPU_PWM_V3));
			} else {
				BLPR("pwm_combo0_freq     = %uHz\n", bl_pwm->pwm_freq);
				BLPR("pwm_combo0_cnt      = %u\n", bl_pwm->pwm_cnt);
				BLPR("pwm_combo0_pre_div  = %u\n", bl_pwm->pwm_pre_div);
				if (bl_pwm->pwm_duty_max > 100)
					BLPR("pwm_combo0_duty = %d%%(%d)\n",
					     bl_pwm->pwm_duty * 100 / 255, bl_pwm->pwm_duty);
				else
					BLPR("pwm_combo0_duty = %d%%(%d)\n",
					     bl_pwm->pwm_duty, bl_pwm->pwm_duty);

				bl_pwm_reg_print(bl_pwm);
			}
			BLPR("pwm_combo0_duty_max = %d\n", bl_pwm->pwm_duty_max);
			BLPR("pwm_combo0_duty_min = %d\n", bl_pwm->pwm_duty_min);
			BLPR("pwm_combo0_gpio     = %s(%d)\n",
			     bconf->gpio_name[bl_pwm->pwm_gpio], bl_pwm->pwm_gpio);
			BLPR("pwm_combo0_gpio_off = %d\n", bl_pwm->pwm_gpio_off);
		}
		if (bconf->bl_pwm_combo1) {
			bl_pwm = bconf->bl_pwm_combo1;
			BLPR("pwm_combo1_index    = %d\n", bl_pwm->index);
			BLPR("pwm_combo1_method   = %d\n", bl_pwm->pwm_method);
			BLPR("pwm_combo1_port     = %s(0x%x)\n",
			     bl_pwm_num_to_str(bl_pwm->pwm_port), bl_pwm->pwm_port);
			if (bl_pwm->pwm_port == BL_PWM_VS) {
				BLPR("pwm_combo1_freq     = %d x vfreq\n", bl_pwm->pwm_freq);
				BLPR("pwm_combo1_cnt      = %u\n", bl_pwm->pwm_cnt);
				if (bl_pwm->pwm_duty_max > 100) {
					BLPR("bl:pwm_combo1_duty = %d%% %d\n",
					      bl_pwm->pwm_duty * 100 / 255, bl_pwm->pwm_duty);
				} else {
					BLPR("bl:pwm_combo1_duty = %d%% %d\n",
					      bl_pwm->pwm_duty, bl_pwm->pwm_duty);
				}

				BLPR("pwm_combo1_reg0     = 0x%08x\n",
				      lcd_vcbus_read(VPU_VPU_PWM_V0));
				BLPR("pwm_combo1_reg1     = 0x%08x\n",
				      lcd_vcbus_read(VPU_VPU_PWM_V1));
				BLPR("pwm_combo1_reg2     = 0x%08x\n",
				      lcd_vcbus_read(VPU_VPU_PWM_V2));
				BLPR("pwm_combo1_reg3     = 0x%08x\n",
				      lcd_vcbus_read(VPU_VPU_PWM_V3));
			} else {
				BLPR("pwm_combo1_freq     = %uHz\n",
				      bl_pwm->pwm_freq);
				BLPR("pwm_combo1_cnt      = %u\n",
				      bl_pwm->pwm_cnt);
				BLPR("pwm_combo1_pre_div  = %u\n",
				      bl_pwm->pwm_pre_div);
				if (bl_pwm->pwm_duty_max > 100) {
					BLPR("pwm_combo1_duty = %d%%(%d)\n",
					     bl_pwm->pwm_duty * 100 / 255, bl_pwm->pwm_duty);
				} else {
					BLPR("pwm_combo1_duty = %d%%(%d)\n",
					     bl_pwm->pwm_duty, bl_pwm->pwm_duty);
				}

				bl_pwm_reg_print(bl_pwm);
			}
			BLPR("pwm_combo1_duty_max = %d\n", bl_pwm->pwm_duty_max);
			BLPR("pwm_combo1_duty_min = %d\n", bl_pwm->pwm_duty_min);
			BLPR("pwm_combo1_gpio     = %s(%d)\n",
			      bconf->gpio_name[bl_pwm->pwm_gpio], bl_pwm->pwm_gpio);
			BLPR("pwm_combo1_gpio_off = %d\n", bl_pwm->pwm_gpio_off);
		}
		BLPR("pwm_on_delay        = %d\n", bconf->pwm_on_delay);
		BLPR("pwm_off_delay       = %d\n", bconf->pwm_off_delay);
		BLPR("en_sequence_reverse = %d\n", bconf->en_sequence_reverse);
		break;
#ifdef CONFIG_AML_LCD_BL_LDIM
	case BL_CTRL_LOCAL_DIMMING:
		if (bdrv->index > 0) {
			BLERR("no ldim driver\n");
			break;
		}
		ldim_drv = aml_ldim_get_driver();
		if (!ldim_drv) {
			BLPR("invalid local dimming driver\n");
			break;
		}
		if (ldim_drv->config_print)
			ldim_drv->config_print(ldim_drv);
		break;
#endif
#ifdef CONFIG_AML_LCD_BL_EXTERN
	case BL_CTRL_EXTERN:
		if (!bl_extern) {
			BLPR("invalid bl extern driver\n");
			break;
		}
		if (bl_extern->config_print)
			bl_extern->config_print();
		break;
#endif

	default:
		BLPR("invalid backlight control method\n");
		break;
	}
}

#ifdef CONFIG_OF_LIBFDT
static int bl_config_load_from_dts(char *dt_addr, struct aml_bl_drv_s *bdrv)
{
	int parent_offset, child_offset;
	char sname[20], propname[30];
	char *propdata;
	char *p;
	const char *str;
	struct bl_config_s *bconf = &bdrv->config;
	struct bl_pwm_config_s *bl_pwm;
	struct bl_pwm_config_s *pwm_combo0, *pwm_combo1;

	if (bdrv->index == 0)
		sprintf(sname, "/backlight");
	else
		sprintf(sname, "/backlight%d", bdrv->index);

	bconf->method = BL_CTRL_MAX; /* default */
	parent_offset = fdt_path_offset(dt_addr, sname);
	if (parent_offset < 0) {
		BLPR("not find %s node: %s\n",
		      sname, fdt_strerror(parent_offset));
		return -1;
	}
	propdata = (char *)fdt_getprop(dt_addr, parent_offset, "status", NULL);
	if (!propdata) {
		BLPR("not find status, default to disabled\n");
		return -1;
	}
	if (strncmp(propdata, "okay", 2)) {
		BLPR("status disabled\n");
		return -1;
	}

	sprintf(propname,"%s/backlight_%d", sname, bconf->index);
	child_offset = fdt_path_offset(dt_addr, propname);
	if (child_offset < 0) {
		BLERR("not find %s node: %s\n", propname, fdt_strerror(child_offset));
		return -1;
	}

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "bl_name", NULL);
	if (!propdata) {
		BLERR("failed to get bl_name\n");
		sprintf(bconf->name, "backlight_%d", bconf->index);
	} else {
		strncpy(bconf->name, propdata, (BL_NAME_MAX - 1));
	}

	propdata = (char *)fdt_getprop(dt_addr, child_offset,
				       "bl_level_default_uboot_kernel", NULL);
	if (!propdata) {
		BLERR("failed to get bl_level_default_uboot_kernel\n");
		bconf->level_default = BL_LEVEL_DEFAULT;
	} else {
		bconf->level_default = be32_to_cpup((u32*)propdata);
	}
	propdata = (char *)fdt_getprop(dt_addr, child_offset,
				       "bl_level_attr", NULL);
	if (!propdata) {
		BLERR("failed to get bl_level_attr\n");
		bconf->level_max = BL_LEVEL_MAX;
		bconf->level_min = BL_LEVEL_MIN;
		bconf->level_mid = BL_LEVEL_MID;
		bconf->level_mid_mapping = BL_LEVEL_MID_MAPPED;
	} else {
		bconf->level_max = be32_to_cpup((u32 *)propdata);
		bconf->level_min = be32_to_cpup((((u32 *)propdata) + 1));
		bconf->level_mid = be32_to_cpup((((u32 *)propdata) + 2));
		bconf->level_mid_mapping =
				be32_to_cpup((((u32 *)propdata) + 3));
	}

	propdata = (char *)fdt_getprop(dt_addr, child_offset,
				       "bl_ctrl_method", NULL);
	if (!propdata) {
		BLERR("failed to get bl_ctrl_method\n");
		bconf->method = BL_CTRL_MAX;
		return -1;
	} else {
		bconf->method = be32_to_cpup((u32 *)propdata);
	}
	propdata = (char *)fdt_getprop(dt_addr, child_offset,
				       "bl_power_attr", NULL);
	if (!propdata) {
		BLERR("failed to get bl_power_attr\n");
		bconf->en_gpio = BL_GPIO_NUM_MAX;
		bconf->en_gpio_on = LCD_GPIO_OUTPUT_HIGH;
		bconf->en_gpio_off = LCD_GPIO_OUTPUT_LOW;
		bconf->power_on_delay = 100;
		bconf->power_off_delay = 30;
	} else {
		bconf->en_gpio = be32_to_cpup((u32 *)propdata);
		bconf->en_gpio_on = be32_to_cpup((((u32 *)propdata) + 1));
		bconf->en_gpio_off = be32_to_cpup((((u32 *)propdata) + 2));
		bconf->power_on_delay = be32_to_cpup((((u32 *)propdata) + 3));
		bconf->power_off_delay = be32_to_cpup((((u32 *)propdata) + 4));
	}

	switch (bconf->method) {
	case BL_CTRL_PWM:
		if (!bconf->bl_pwm) {
			bconf->bl_pwm = (struct bl_pwm_config_s *)
				malloc(sizeof(struct bl_pwm_config_s));
			if (!bconf->bl_pwm) {
				BLERR("bl_pwm malloc error\n");
				return -1;
			}
		}
		bl_pwm = bconf->bl_pwm;
		memset(bl_pwm, 0, sizeof(struct bl_pwm_config_s));
		bl_pwm->index = 0;

		bl_pwm->level_max = bconf->level_max;
		bl_pwm->level_min = bconf->level_min;

		propdata = (char *)fdt_getprop(dt_addr, child_offset,
					       "bl_pwm_port", NULL);
		if (!propdata) {
			BLERR("failed to get bl_pwm_port\n");
			bl_pwm->pwm_port = BL_PWM_MAX;
		} else {
			bl_pwm->pwm_port = bl_pwm_str_to_num(propdata);
		}
		propdata = (char *)fdt_getprop(dt_addr, child_offset,
					       "bl_pwm_attr", NULL);
		if (!propdata) {
			BLERR("failed to get bl_pwm_attr\n");
			bl_pwm->pwm_method = BL_PWM_POSITIVE;
			if (bl_pwm->pwm_port == BL_PWM_VS)
				bl_pwm->pwm_freq = BL_FREQ_VS_DEFAULT;
			else
				bl_pwm->pwm_freq = BL_FREQ_DEFAULT;
			bl_pwm->pwm_duty_max = 80;
			bl_pwm->pwm_duty_min = 20;
		} else {
			bl_pwm->pwm_method = be32_to_cpup((u32 *)propdata);
			bl_pwm->pwm_freq =
				be32_to_cpup((((u32 *)propdata) + 1));
			bl_pwm->pwm_duty_max =
				be32_to_cpup((((u32 *)propdata) + 2));
			bl_pwm->pwm_duty_min =
				be32_to_cpup((((u32 *)propdata) + 3));
		}
		if (bl_pwm->pwm_port == BL_PWM_VS) {
			if (bl_pwm->pwm_freq > 4) {
				BLERR("bl_pwm_vs wrong freq %d\n",
				       bl_pwm->pwm_freq);
				bl_pwm->pwm_freq = BL_FREQ_VS_DEFAULT;
			}
		} else {
			if (bl_pwm->pwm_freq > XTAL_HALF_FREQ_HZ)
				bl_pwm->pwm_freq = XTAL_HALF_FREQ_HZ;
		}
		propdata = (char *)fdt_getprop(dt_addr, child_offset,
					       "bl_pwm_power", NULL);
		if (!propdata) {
			BLERR("failed to get bl_pwm_power\n");
			bl_pwm->pwm_gpio = BL_GPIO_NUM_MAX;
			bl_pwm->pwm_gpio_off = LCD_GPIO_OUTPUT_LOW;
			bconf->pwm_on_delay = 10;
			bconf->pwm_off_delay = 10;
		} else {
			bl_pwm->pwm_gpio = be32_to_cpup((u32 *)propdata);
			bl_pwm->pwm_gpio_off =
				be32_to_cpup((((u32 *)propdata) + 1));
			bconf->pwm_on_delay =
				be32_to_cpup((((u32 *)propdata) + 2));
			bconf->pwm_off_delay =
				be32_to_cpup((((u32 *)propdata) + 3));
		}
		propdata = (char *)fdt_getprop(dt_addr, child_offset,
					       "bl_pwm_en_sequence_reverse",
					       NULL);
		if (!propdata) {
			propdata = (char *)fdt_getprop(dt_addr, child_offset,
						       "en_sequence_reverse",
						       NULL);
			if (!propdata) {
				bconf->en_sequence_reverse = 0;
			} else {
				bconf->en_sequence_reverse =
					be32_to_cpup((u32 *)propdata);
				BLPR("find en_sequence_reverse: %d\n",
				     bconf->en_sequence_reverse);
			}
		} else {
			bconf->en_sequence_reverse = be32_to_cpup((u32 *)propdata);
			BLPR("find en_sequence_reverse: %d\n",
			     bconf->en_sequence_reverse);
		}

		bl_pwm->pwm_duty = bl_pwm->pwm_duty_min;
		/* bl_pwm_config_init(bl_pwm); */
		break;
	case BL_CTRL_PWM_COMBO:
		if (!bconf->bl_pwm_combo0) {
			bconf->bl_pwm_combo0 = (struct bl_pwm_config_s *)
				malloc(sizeof(struct bl_pwm_config_s));
			if (!bconf->bl_pwm_combo0) {
				BLERR("bl_pwm_combo0 malloc error\n");
				return -1;
			}
		}
		if (!bconf->bl_pwm_combo1) {
			bconf->bl_pwm_combo1 = (struct bl_pwm_config_s *)
				malloc(sizeof(struct bl_pwm_config_s));
			if (!bconf->bl_pwm_combo1) {
				free(bconf->bl_pwm_combo0);
				BLERR("bl_pwm_combo1 struct malloc error\n");
				return -1;
			}
		}
		pwm_combo0 = bconf->bl_pwm_combo0;
		pwm_combo1 = bconf->bl_pwm_combo1;
		memset(pwm_combo0, 0, sizeof(struct bl_pwm_config_s));
		memset(pwm_combo1, 0, sizeof(struct bl_pwm_config_s));
		pwm_combo0->index = 0;
		pwm_combo1->index = 1;

		propdata = (char *)fdt_getprop(dt_addr, child_offset,
					       "bl_pwm_combo_level_mapping",
					       NULL);
		if (!propdata) {
			BLERR("failed to get bl_pwm_combo_level_mapping\n");
			pwm_combo0->level_max = BL_LEVEL_MAX;
			pwm_combo0->level_min = BL_LEVEL_MID;
			pwm_combo1->level_max = BL_LEVEL_MID;
			pwm_combo1->level_min = BL_LEVEL_MIN;
		} else {
			pwm_combo0->level_max = be32_to_cpup((u32 *)propdata);
			pwm_combo0->level_min =
				be32_to_cpup((((u32 *)propdata) + 1));
			pwm_combo1->level_max =
				be32_to_cpup((((u32 *)propdata) + 2));
			pwm_combo1->level_min =
				be32_to_cpup((((u32 *)propdata) + 3));
		}
		propdata = (char *)fdt_getprop(dt_addr, child_offset,
					       "bl_pwm_combo_port", NULL);
		if (!propdata) {
			BLERR("failed to get bl_pwm_combo_port\n");
			pwm_combo0->pwm_port = BL_PWM_MAX;
			pwm_combo1->pwm_port = BL_PWM_MAX;
		} else {
			p = propdata;
			str = p;
			pwm_combo0->pwm_port = bl_pwm_str_to_num(str);
			p += strlen(p) + 1;
			str = p;
			pwm_combo1->pwm_port = bl_pwm_str_to_num(str);
		}
		propdata = (char *)fdt_getprop(dt_addr, child_offset,
					       "bl_pwm_combo_attr", NULL);
		if (!propdata) {
			BLERR("failed to get bl_pwm_combo_attr\n");
			pwm_combo0->pwm_method = BL_PWM_POSITIVE;
			if (pwm_combo0->pwm_port == BL_PWM_VS)
				pwm_combo0->pwm_freq = BL_FREQ_VS_DEFAULT;
			else
				pwm_combo0->pwm_freq = BL_FREQ_DEFAULT;
			pwm_combo0->pwm_duty_max = 80;
			pwm_combo0->pwm_duty_min = 20;
			pwm_combo1->pwm_method = BL_PWM_POSITIVE;
			if (pwm_combo1->pwm_port == BL_PWM_VS)
				pwm_combo1->pwm_freq = BL_FREQ_VS_DEFAULT;
			else
				pwm_combo1->pwm_freq = BL_FREQ_DEFAULT;
			pwm_combo1->pwm_duty_max = 80;
			pwm_combo1->pwm_duty_min = 20;
		} else {
			pwm_combo0->pwm_method = be32_to_cpup((u32 *)propdata);
			pwm_combo0->pwm_freq =
				be32_to_cpup((((u32 *)propdata) + 1));
			pwm_combo0->pwm_duty_max =
				be32_to_cpup((((u32 *)propdata) + 2));
			pwm_combo0->pwm_duty_min =
				be32_to_cpup((((u32 *)propdata) + 3));
			pwm_combo1->pwm_method =
				be32_to_cpup((((u32 *)propdata) + 4));
			pwm_combo1->pwm_freq =
				be32_to_cpup((((u32 *)propdata) + 5));
			pwm_combo1->pwm_duty_max =
				be32_to_cpup((((u32 *)propdata) + 6));
			pwm_combo1->pwm_duty_min =
				be32_to_cpup((((u32 *)propdata) + 7));
		}
		if (pwm_combo0->pwm_port == BL_PWM_VS) {
			if (pwm_combo0->pwm_freq > 4) {
				BLERR("bl_pwm_vs wrong freq %d\n",
				       pwm_combo0->pwm_freq);
				pwm_combo0->pwm_freq = BL_FREQ_VS_DEFAULT;
			}
		} else {
			if (pwm_combo0->pwm_freq > XTAL_HALF_FREQ_HZ)
				pwm_combo0->pwm_freq = XTAL_HALF_FREQ_HZ;
		}
		if (pwm_combo1->pwm_port == BL_PWM_VS) {
			if (pwm_combo1->pwm_freq > 4) {
				BLERR("bl_pwm_vs wrong freq %d\n",
				       pwm_combo1->pwm_freq);
				pwm_combo1->pwm_freq = BL_FREQ_VS_DEFAULT;
			}
		} else {
			if (pwm_combo1->pwm_freq > XTAL_HALF_FREQ_HZ)
				pwm_combo1->pwm_freq = XTAL_HALF_FREQ_HZ;
		}
		propdata = (char *)fdt_getprop(dt_addr, child_offset,
					       "bl_pwm_combo_power", NULL);
		if (!propdata) {
			BLERR("failed to get bl_pwm_combo_power\n");
			pwm_combo0->pwm_gpio = BL_GPIO_NUM_MAX;
			pwm_combo0->pwm_gpio_off = LCD_GPIO_INPUT;
			pwm_combo1->pwm_gpio = BL_GPIO_NUM_MAX;
			pwm_combo1->pwm_gpio_off = LCD_GPIO_INPUT;
			bconf->pwm_on_delay = 10;
			bconf->pwm_off_delay = 10;
		} else {
			pwm_combo0->pwm_gpio =
				be32_to_cpup((u32 *)propdata);
			pwm_combo0->pwm_gpio_off =
				be32_to_cpup((((u32 *)propdata) + 1));
			pwm_combo1->pwm_gpio =
				be32_to_cpup((((u32 *)propdata) + 2));
			pwm_combo1->pwm_gpio_off =
				be32_to_cpup((((u32 *)propdata) + 3));
			bconf->pwm_on_delay =
				be32_to_cpup((((u32 *)propdata) + 4));
			bconf->pwm_off_delay =
				be32_to_cpup((((u32 *)propdata) + 5));
		}
		propdata = (char *)fdt_getprop(dt_addr, child_offset,
					       "bl_pwm_en_sequence_reverse",
					       NULL);
		if (!propdata) {
			propdata = (char *)fdt_getprop(dt_addr, child_offset,
						       "en_sequence_reverse",
						       NULL);
			if (!propdata) {
				bconf->en_sequence_reverse = 0;
			} else {
				bconf->en_sequence_reverse =
					be32_to_cpup((u32 *)propdata);
				BLPR("find en_sequence_reverse: %d\n",
				     bconf->en_sequence_reverse);
			}
		} else {
			bconf->en_sequence_reverse =
					be32_to_cpup((u32 *)propdata);
		}

		pwm_combo0->pwm_duty = pwm_combo0->pwm_duty_min;
		pwm_combo1->pwm_duty = pwm_combo1->pwm_duty_min;
		/* bl_pwm_config_init(pwm_combo0);
		bl_pwm_config_init(pwm_combo1); */
		break;
#ifdef CONFIG_AML_LCD_BL_LDIM
	case BL_CTRL_LOCAL_DIMMING:
		if (bdrv->index > 0) {
			BLERR("no ldim driver\n");
			break;
		}
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "en_sequence_reverse", NULL);
		if (!propdata) {
			bconf->en_sequence_reverse = 0;
		} else {
			bconf->en_sequence_reverse = be32_to_cpup((u32 *)propdata);
			BLPR("find en_sequence_reverse: %d\n", bconf->en_sequence_reverse);
		}

		aml_ldim_probe(dt_addr, child_offset, NULL, 0);
		break;
#endif
#ifdef CONFIG_AML_LCD_BL_EXTERN
	case BL_CTRL_EXTERN:
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "en_sequence_reverse", NULL);
		if (!propdata) {
			bconf->en_sequence_reverse = 0;
		} else {
			bconf->en_sequence_reverse = be32_to_cpup((u32 *)propdata);
			BLPR("find en_sequence_reverse: %d\n", bconf->en_sequence_reverse);
		}

		/* get bl_extern_index from dts */
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "bl_extern_index", NULL);
		if (!propdata) {
			BLERR("failed to get bl_extern_index\n");
		} else {
			bconf->bl_extern_index = be32_to_cpup((u32 *)propdata);
			BLPR("get bl_extern_index = %d\n", bconf->bl_extern_index);
		}
		bl_extern_device_load(dt_addr, bconf->bl_extern_index);
		break;
#endif

	default:
		break;
	}

	return 0;
}
#endif

static int bl_config_load_from_unifykey(char *dt_addr, struct aml_bl_drv_s *bdrv)
{
	unsigned char *para;
	int key_len, len;
	unsigned char *p;
	const char *str;
	char sname[20];
	struct lcd_unifykey_header_s bl_header;
	struct bl_config_s *bconf = &bdrv->config;
	struct bl_pwm_config_s *bl_pwm;
	struct bl_pwm_config_s *pwm_combo0, *pwm_combo1;
	int ret;

	key_len = LCD_UKEY_BL_SIZE;
	para = (unsigned char *)malloc(sizeof(unsigned char) * key_len);
	if (!para) {
		BLERR("%s: Not enough memory\n", __func__);
		return -1;
	}
	memset(para, 0, (sizeof(unsigned char) * key_len));

	if (bdrv->index == 0)
		sprintf(sname, "backlight");
	else
		sprintf(sname, "backlight%d", bdrv->index);
	ret = lcd_unifykey_get(sname, para, &key_len);
	if (ret) {
		free(para);
		return -1;
	}

	/* step 1: check header */
	len = LCD_UKEY_HEAD_SIZE;
	ret = lcd_unifykey_len_check(key_len, len);
	if (ret) {
		BLERR("unifykey header length is incorrect\n");
		free(para);
		return -1;
	}

	lcd_unifykey_header_check(para, &bl_header);
	BLPR("unifykey version: 0x%04x\n", bl_header.version);
	switch (bl_header.version) {
	case 2:
		len = 10 + 30 + 12 + 8 + 32 + 10;
		break;
	default:
		len = 10 + 30 + 12 + 8 + 32;
		break;
	}
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
		BLPR("unifykey header:\n");
		BLPR("crc32             = 0x%08x\n", bl_header.crc32);
		BLPR("data_len          = %d\n", bl_header.data_len);
	}

	/* step 2: check backlight parameters */
	ret = lcd_unifykey_len_check(key_len, len);
	if (ret) {
		BLERR("unifykey length is incorrect\n");
		free(para);
		return -1;
	}

	/* basic: 30byte */
	p = para;
	str = (const char *)(p + LCD_UKEY_HEAD_SIZE);
	strncpy(bconf->name, str, sizeof(bconf->name) - 1);
	bconf->name[sizeof(bconf->name) - 1] = '\0';

	/* level: 12byte */
	bconf->level_default = (*(p + LCD_UKEY_BL_LEVEL_UBOOT) |
		 ((*(p + LCD_UKEY_BL_LEVEL_UBOOT + 1)) << 8));
	bconf->level_max = (*(p + LCD_UKEY_BL_LEVEL_MAX) |
		((*(p + LCD_UKEY_BL_LEVEL_MAX + 1)) << 8));
	bconf->level_min = (*(p + LCD_UKEY_BL_LEVEL_MIN) |
		((*(p  + LCD_UKEY_BL_LEVEL_MIN + 1)) << 8));
	bconf->level_mid = (*(p + LCD_UKEY_BL_LEVEL_MID) |
		((*(p + LCD_UKEY_BL_LEVEL_MID + 1)) << 8));
	bconf->level_mid_mapping = (*(p + LCD_UKEY_BL_LEVEL_MID_MAP) |
		((*(p + LCD_UKEY_BL_LEVEL_MID_MAP + 1)) << 8));

	/* method: 8byte */
	bconf->method = *(p + LCD_UKEY_BL_METHOD);
	bconf->en_gpio = *(p + LCD_UKEY_BL_EN_GPIO);
	bconf->en_gpio_on = *(p + LCD_UKEY_BL_EN_GPIO_ON);
	bconf->en_gpio_off = *(p + LCD_UKEY_BL_EN_GPIO_OFF);
	bconf->power_on_delay = (*(p + LCD_UKEY_BL_ON_DELAY) |
		((*(p + LCD_UKEY_BL_ON_DELAY + 1)) << 8));
	bconf->power_off_delay = (*(p + LCD_UKEY_BL_OFF_DELAY) |
		((*(p + LCD_UKEY_BL_OFF_DELAY + 1)) << 8));

	/* pwm: 32byte */
	switch (bconf->method) {
	case BL_CTRL_PWM:
		bconf->bl_pwm = (struct bl_pwm_config_s *)
			malloc(sizeof(struct bl_pwm_config_s));
		if (!bconf->bl_pwm) {
			BLERR("bl_pwm struct malloc error\n");
			free(para);
			return -1;
		}
		bl_pwm = bconf->bl_pwm;
		bl_pwm->index = 0;

		bl_pwm->level_max = bconf->level_max;
		bl_pwm->level_min = bconf->level_min;

		bconf->pwm_on_delay = (*(p + LCD_UKEY_BL_PWM_ON_DELAY) |
			((*(p + LCD_UKEY_BL_PWM_ON_DELAY + 1)) << 8));
		bconf->pwm_off_delay = (*(p + LCD_UKEY_BL_PWM_OFF_DELAY) |
			((*(p + LCD_UKEY_BL_PWM_OFF_DELAY + 1)) << 8));
		bl_pwm->pwm_method =  *(p + LCD_UKEY_BL_PWM_METHOD);
		bl_pwm->pwm_port = *(p + LCD_UKEY_BL_PWM_PORT);
		bl_pwm->pwm_freq = (*(p + LCD_UKEY_BL_PWM_FREQ) |
			((*(p + LCD_UKEY_BL_PWM_FREQ + 1)) << 8) |
			((*(p + LCD_UKEY_BL_PWM_FREQ + 2)) << 8) |
			((*(p + LCD_UKEY_BL_PWM_FREQ + 3)) << 8));
		if (bl_pwm->pwm_port == BL_PWM_VS) {
			if (bl_pwm->pwm_freq > 4) {
				BLERR("bl_pwm_vs wrong freq %d\n", bl_pwm->pwm_freq);
				bl_pwm->pwm_freq = BL_FREQ_VS_DEFAULT;
			}
		} else {
			if (bl_pwm->pwm_freq > XTAL_HALF_FREQ_HZ)
				bl_pwm->pwm_freq = XTAL_HALF_FREQ_HZ;
		}
		bl_pwm->pwm_duty_max = *(p + LCD_UKEY_BL_PWM_DUTY_MAX);
		bl_pwm->pwm_duty_min = *(p + LCD_UKEY_BL_PWM_DUTY_MIN);
		bl_pwm->pwm_gpio = *(p + LCD_UKEY_BL_PWM_GPIO);
		bl_pwm->pwm_gpio_off = *(p + LCD_UKEY_BL_PWM_GPIO_OFF);

		if (bl_header.version == 2)
			bconf->en_sequence_reverse =
				(*(p + LCD_UKEY_BL_CUST_VAL_0) |
				((*(p + LCD_UKEY_BL_CUST_VAL_0 + 1)) << 8));
		else
			bconf->en_sequence_reverse = 0;

		bl_pwm->pwm_duty = bl_pwm->pwm_duty_min;
		/* bl_pwm_config_init(bl_pwm); */
		break;
	case BL_CTRL_PWM_COMBO:
		bconf->bl_pwm_combo0 = (struct bl_pwm_config_s *)
			malloc(sizeof(struct bl_pwm_config_s));
		if (!bconf->bl_pwm_combo0) {
			BLERR("bl_pwm_combo0 struct malloc error\n");
			free(para);
			return -1;
		}
		bconf->bl_pwm_combo1 = (struct bl_pwm_config_s *)
				malloc(sizeof(struct bl_pwm_config_s));
		if (!bconf->bl_pwm_combo1) {
			BLERR("bl_pwm_combo1 struct malloc error\n");
			free(para);
			return -1;
		}
		pwm_combo0 = bconf->bl_pwm_combo0;
		pwm_combo1 = bconf->bl_pwm_combo1;
		pwm_combo0->index = 0;
		pwm_combo1->index = 1;

		bconf->pwm_on_delay = (*(p + LCD_UKEY_BL_PWM_ON_DELAY) |
			((*(p + LCD_UKEY_BL_PWM_ON_DELAY + 1)) << 8));
		bconf->pwm_off_delay = (*(p + LCD_UKEY_BL_PWM_OFF_DELAY) |
			((*(p + LCD_UKEY_BL_PWM_OFF_DELAY + 1)) << 8));
		pwm_combo0->pwm_method = *(p + LCD_UKEY_BL_PWM_METHOD);
		pwm_combo0->pwm_port = *(p + LCD_UKEY_BL_PWM_PORT);
		pwm_combo0->pwm_freq = (*(p + LCD_UKEY_BL_PWM_FREQ) |
			((*(p + LCD_UKEY_BL_PWM_FREQ + 1)) << 8) |
			((*(p + LCD_UKEY_BL_PWM_FREQ + 2)) << 8) |
			((*(p + LCD_UKEY_BL_PWM_FREQ + 3)) << 8));
		if (pwm_combo0->pwm_port == BL_PWM_VS) {
			if (pwm_combo0->pwm_freq > 4) {
				BLERR("bl_pwm_vs wrong freq %d\n",
				       pwm_combo0->pwm_freq);
				pwm_combo0->pwm_freq = BL_FREQ_VS_DEFAULT;
			}
		} else {
			if (pwm_combo0->pwm_freq > XTAL_HALF_FREQ_HZ)
				pwm_combo0->pwm_freq = XTAL_HALF_FREQ_HZ;
		}
		pwm_combo0->pwm_duty_max = *(p + LCD_UKEY_BL_PWM_DUTY_MAX);
		pwm_combo0->pwm_duty_min = *(p + LCD_UKEY_BL_PWM_DUTY_MIN);
		pwm_combo0->pwm_gpio = *(p + LCD_UKEY_BL_PWM_GPIO);
		pwm_combo0->pwm_gpio_off = *(p + LCD_UKEY_BL_PWM_GPIO_OFF);
		pwm_combo1->pwm_method = *(p + LCD_UKEY_BL_PWM2_METHOD);
		pwm_combo1->pwm_port = *(p + LCD_UKEY_BL_PWM2_PORT);
		pwm_combo1->pwm_freq = (*(p + LCD_UKEY_BL_PWM2_FREQ) |
			((*(p + LCD_UKEY_BL_PWM2_FREQ + 1)) << 8) |
			((*(p + LCD_UKEY_BL_PWM2_FREQ + 2)) << 8) |
			((*(p + LCD_UKEY_BL_PWM2_FREQ + 3)) << 8));
		if (pwm_combo1->pwm_port == BL_PWM_VS) {
			if (pwm_combo1->pwm_freq > 4) {
				BLERR("bl_pwm_vs wrong freq %d\n",
				       pwm_combo1->pwm_freq);
				pwm_combo1->pwm_freq = BL_FREQ_VS_DEFAULT;
			}
		} else {
			if (pwm_combo1->pwm_freq > XTAL_HALF_FREQ_HZ)
				pwm_combo1->pwm_freq = XTAL_HALF_FREQ_HZ;
		}
		pwm_combo1->pwm_duty_max = *(p + LCD_UKEY_BL_PWM2_DUTY_MAX);
		pwm_combo1->pwm_duty_min = *(p + LCD_UKEY_BL_PWM2_DUTY_MIN);
		pwm_combo1->pwm_gpio = *(p + LCD_UKEY_BL_PWM2_GPIO);
		pwm_combo1->pwm_gpio_off = *(p + LCD_UKEY_BL_PWM2_GPIO_OFF);

		pwm_combo0->level_max = (*(p + LCD_UKEY_BL_PWM_LEVEL_MAX) |
			((*(p + LCD_UKEY_BL_PWM_LEVEL_MAX + 1)) << 8));
		pwm_combo0->level_min = (*(p + LCD_UKEY_BL_PWM_LEVEL_MIN) |
			((*(p + LCD_UKEY_BL_PWM_LEVEL_MIN + 1)) << 8));
		pwm_combo1->level_max = (*(p + LCD_UKEY_BL_PWM2_LEVEL_MAX) |
			((*(p + LCD_UKEY_BL_PWM2_LEVEL_MAX + 1)) << 8));
		pwm_combo1->level_min = (*(p + LCD_UKEY_BL_PWM2_LEVEL_MIN) |
			((*(p + LCD_UKEY_BL_PWM2_LEVEL_MIN + 1)) << 8));

		if (bl_header.version == 2)
			bconf->en_sequence_reverse = (*(p + LCD_UKEY_BL_CUST_VAL_0) |
				((*(p + LCD_UKEY_BL_CUST_VAL_0 + 1)) << 8));
		else
			bconf->en_sequence_reverse = 0;

		pwm_combo0->pwm_duty = pwm_combo0->pwm_duty_min;
		pwm_combo1->pwm_duty = pwm_combo1->pwm_duty_min;
		/* bl_pwm_config_init(pwm_combo0);
		bl_pwm_config_init(pwm_combo1); */
		break;
#ifdef CONFIG_AML_LCD_BL_LDIM
	case BL_CTRL_LOCAL_DIMMING:
		if (bdrv->index > 0) {
			BLERR("no ldim driver\n");
			break;
		}
		if (bl_header.version == 2) {
			aml_ldim_probe(dt_addr, 0, para, 2);
		} else {
			BLERR("not support ldim for unifykey version: %d\n",
			      bl_header.version);
		}
		break;
#endif
	default:
		break;
	}

	free(para);
	return 0;
}

static int bl_config_load_from_bsp(struct aml_bl_drv_s *bdrv)
{
	struct ext_lcd_config_s *ext_lcd = NULL;
	char *panel_type, sname[20];
	unsigned int i = 0, j, done;
	char *str;
	struct bl_config_s *bconf = &bdrv->config;
	struct bl_pwm_config_s *bl_pwm;
	struct bl_pwm_config_s *pwm_combo0, *pwm_combo1;
	char (*bl_gpio)[LCD_CPU_GPIO_NAME_MAX];

	if (!bdrv->data)
		return -1;

	if (bdrv->index == 0)
		sprintf(sname, "panel_type");
	else
		sprintf(sname, "panel%d_type", bdrv->index);
	panel_type = env_get(sname);
	if (!panel_type) {
		BLERR("no %s\n", sname);
		return -1;
	}

	if (!bdrv->data->dft_conf[bdrv->index]) {
		BLERR("no dft_conf\n");
		return -1;
	}

	ext_lcd = bdrv->data->dft_conf[bdrv->index]->ext_lcd;
	if (!ext_lcd) {
		BLERR("%s: ext_lcd is NULL\n", __func__);
		return -1;
	}

	done = 0;
	for (i = 0; i < LCD_NUM_MAX; i++) {
		if (strcmp(ext_lcd->panel_type, panel_type) == 0) {
			done = 1;
			break;
		}
		if (strcmp(ext_lcd->panel_type, "invalid") == 0)
			break;
		ext_lcd++;
	}
	if (done == 0) {
		BLERR("can't find %s\n ", panel_type);
		return -1;
	}

	strncpy(bconf->name, panel_type, sizeof(bconf->name) - 1);
	bconf->name[sizeof(bconf->name) - 1] = '\0';

	bconf->level_default     = ext_lcd->level_default;
	bconf->level_min         = ext_lcd->level_min;
	bconf->level_max         = ext_lcd->level_max;
	bconf->level_mid         = ext_lcd->level_mid;
	bconf->level_mid_mapping = ext_lcd->level_mid_mapping;

	bconf->method = ext_lcd->bl_method;

	if (ext_lcd->bl_en_gpio >= BL_GPIO_NUM_MAX)
		bconf->en_gpio = LCD_GPIO_MAX;
	else
		bconf->en_gpio = ext_lcd->bl_en_gpio;
	bconf->en_gpio_on      = ext_lcd->bl_en_gpio_on;
	bconf->en_gpio_off     = ext_lcd->bl_en_gpio_off;
	bconf->power_on_delay  = ext_lcd->bl_power_on_delay;
	bconf->power_off_delay = ext_lcd->bl_power_off_delay;

	switch (bconf->method) {
	case BL_CTRL_PWM:
		bl_pwm = (struct bl_pwm_config_s *)malloc(sizeof(struct bl_pwm_config_s));
		if (!bl_pwm) {
			BLERR("bl_pwm struct malloc error\n");
			return -1;
		}
		bconf->bl_pwm = bl_pwm;
		bl_pwm->index = 0;

		bl_pwm->level_max     = bconf->level_max;
		bl_pwm->level_min     = bconf->level_min;

		bl_pwm->pwm_method    = ext_lcd->pwm_method;
		bl_pwm->pwm_port      = ext_lcd->pwm_port;
		bl_pwm->pwm_freq      = ext_lcd->pwm_freq;
		bl_pwm->pwm_duty_max  = ext_lcd->pwm_duty_max;
		bl_pwm->pwm_duty_min  = ext_lcd->pwm_duty_min;

		bl_pwm->pwm_gpio      = ext_lcd->pwm_gpio;
		bl_pwm->pwm_gpio_off  = ext_lcd->pwm_gpio_off;
		bconf->pwm_on_delay   = ext_lcd->pwm_on_delay;
		bconf->pwm_off_delay  = ext_lcd->pwm_off_delay;

		bl_pwm->pwm_duty = bl_pwm->pwm_duty_min;
		/* bl_pwm_config_init(bl_pwm); */
		break;
	case BL_CTRL_PWM_COMBO:
		bconf->bl_pwm_combo0 = (struct bl_pwm_config_s *)
			malloc(sizeof(struct bl_pwm_config_s));
		if (!bconf->bl_pwm_combo0) {
			BLERR("bl_pwm_combo0 struct malloc error\n");
			return -1;
		}
		bconf->bl_pwm_combo1 = (struct bl_pwm_config_s *)
			malloc(sizeof(struct bl_pwm_config_s));
		if (!bconf->bl_pwm_combo1) {
			BLERR("bl_pwm_combo1 struct malloc error\n");
			return -1;
		}
		pwm_combo0 = bconf->bl_pwm_combo0;
		pwm_combo1 = bconf->bl_pwm_combo1;
		pwm_combo0->index = 0;
		pwm_combo1->index = 1;

		pwm_combo0->level_max     = ext_lcd->pwm_level_max;
		pwm_combo0->level_min     = ext_lcd->pwm_level_min;
		pwm_combo1->level_max     = ext_lcd->pwm2_level_max;
		pwm_combo1->level_min     = ext_lcd->pwm2_level_min;

		pwm_combo0->pwm_method    = ext_lcd->pwm_method;
		pwm_combo0->pwm_port      = ext_lcd->pwm_port;
		pwm_combo0->pwm_freq      = ext_lcd->pwm_freq;
		pwm_combo0->pwm_duty_max  = ext_lcd->pwm_duty_max;
		pwm_combo0->pwm_duty_min  = ext_lcd->pwm_duty_min;
		if (ext_lcd->pwm_gpio >= BL_GPIO_NUM_MAX) {
			pwm_combo0->pwm_gpio = LCD_GPIO_MAX;
		} else {
			str = bconf->gpio_name[ext_lcd->pwm_gpio];
			pwm_combo0->pwm_gpio = lcd_gpio_name_map_num(str);
		}
		pwm_combo0->pwm_gpio_off  = ext_lcd->pwm_gpio_off;
		pwm_combo1->pwm_method    = ext_lcd->pwm2_method;
		pwm_combo1->pwm_port      = ext_lcd->pwm2_port;
		pwm_combo1->pwm_freq      = ext_lcd->pwm2_freq;
		pwm_combo1->pwm_duty_max  = ext_lcd->pwm2_duty_max;
		pwm_combo1->pwm_duty_min  = ext_lcd->pwm2_duty_min;
		if (ext_lcd->pwm2_gpio >= BL_GPIO_NUM_MAX) {
			pwm_combo1->pwm_gpio = LCD_GPIO_MAX;
		} else {
			str = bconf->gpio_name[ext_lcd->pwm2_gpio];
			pwm_combo1->pwm_gpio = lcd_gpio_name_map_num(str);
		}
		pwm_combo1->pwm_gpio_off  = ext_lcd->pwm2_gpio_off;
		bconf->pwm_on_delay   = ext_lcd->pwm_on_delay;
		bconf->pwm_off_delay  = ext_lcd->pwm_off_delay;

		pwm_combo0->pwm_duty = pwm_combo0->pwm_duty_min;
		pwm_combo1->pwm_duty = pwm_combo1->pwm_duty_min;
		/* bl_pwm_config_init(pwm_combo0);
		bl_pwm_config_init(pwm_combo1); */
		break;
#ifdef CONFIG_AML_LCD_BL_LDIM
	case BL_CTRL_LOCAL_DIMMING:
		if (bdrv->index > 0) {
			BLERR("no ldim driver\n");
			break;
		}
		aml_ldim_probe(NULL, 0, NULL, 1);
		break;
#endif
#ifdef CONFIG_AML_LCD_BL_EXTERN
	case BL_CTRL_EXTERN:
		bl_extern_device_load(NULL, bconf->bl_extern_index);
		break;
#endif
	default:
		if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
			BLPR("invalid backlight control method\n");
		break;
	}

	i = 0;
	bl_gpio = bdrv->data->dft_conf[bdrv->index]->bl_gpio;
	if (!bl_gpio) {
		LCDERR("%s: bl_gpio is null\n", __func__);
		return -1;
	}
	while (i < BL_GPIO_NUM_MAX) {
		if (strcmp(bl_gpio[i], "invalid") == 0)
			break;
		strcpy(bconf->gpio_name[i], bl_gpio[i]);
		i++;
	}
	for (j = i; j < BL_GPIO_NUM_MAX; j++)
		strcpy(bconf->gpio_name[j], "invalid");

	return 0;
}

static const char *bl_pinmux_str[] = {
	"bl_pwm_on_pin",        /* 0 */
	"bl_pwm_vs_on_pin",     /* 1 */
	"bl_pwm_combo_0_on_pin",  /* 2 */
	"bl_pwm_combo_1_on_pin",  /* 3 */
	"bl_pwm_combo_0_vs_on_pin",  /* 4 */
	"bl_pwm_combo_1_vs_on_pin",  /* 5 */
};

static int bl_pinmux_load_from_bsp(struct aml_bl_drv_s *bdrv)
{
	struct bl_config_s *bconf = &bdrv->config;
	char propname[50];
	struct lcd_pinmux_ctrl_s *pinmux;
	unsigned int i, j;
	int pinmux_index = 0, set_cnt = 0, clr_cnt = 0;
	struct bl_pwm_config_s *bl_pwm;
	struct bl_pwm_config_s *pwm_combo0, *pwm_combo1;

	if (!bdrv->data)
		return -1;
	if (!bdrv->data->dft_conf[bdrv->index]) {
		BLERR("%s: dft_conf is NULL\n", __func__);
		return -1;
	}
	bconf->bl_pinmux = bdrv->data->dft_conf[bdrv->index]->bl_pinmux;
	if (!bconf->bl_pinmux) {
		BLERR("%s: bl_pinmux is NULL\n", __func__);
		return -1;
	}

	switch (bconf->method) {
	case BL_CTRL_PWM:
		bl_pwm = bconf->bl_pwm;
		if (bl_pwm->pwm_port == BL_PWM_VS)
			pinmux_index = 1;
		else
			pinmux_index = 0;
		sprintf(propname,"%s", bl_pinmux_str[pinmux_index]);
		pinmux = bconf->bl_pinmux;
		for (i = 0; i < LCD_PINMX_MAX; i++) {
			if (!pinmux)
				break;
			if (!pinmux->name)
				break;
			if (strncmp(pinmux->name, "invalid", 7) == 0)
				break;
			if (strncmp(pinmux->name, propname, strlen(propname)) == 0) {
				for (j = 0; j < LCD_PINMUX_NUM; j++ ) {
					if (pinmux->pinmux_set[j][0] ==
					    LCD_PINMUX_END)
						break;
					bl_pwm->pinmux_set[j][0] =
						pinmux->pinmux_set[j][0];
					bl_pwm->pinmux_set[j][1] =
						pinmux->pinmux_set[j][1];
					set_cnt++;
				}
				for (j = 0; j < LCD_PINMUX_NUM; j++ ) {
					if (pinmux->pinmux_clr[j][0] ==
					    LCD_PINMUX_END)
						break;
					bl_pwm->pinmux_clr[j][0] =
						pinmux->pinmux_clr[j][0];
					bl_pwm->pinmux_clr[j][1] =
						pinmux->pinmux_clr[j][1];
					clr_cnt++;
				}
				break;
			}
			pinmux++;
		}
		if (set_cnt < LCD_PINMUX_NUM) {
			bl_pwm->pinmux_set[set_cnt][0] = LCD_PINMUX_END;
			bl_pwm->pinmux_set[set_cnt][1] = 0x0;
		}
		if (clr_cnt < LCD_PINMUX_NUM) {
			bl_pwm->pinmux_clr[clr_cnt][0] = LCD_PINMUX_END;
			bl_pwm->pinmux_clr[clr_cnt][1] = 0x0;
		}

		if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
			i = 0;
			while (i < LCD_PINMUX_NUM) {
				if (bl_pwm->pinmux_set[i][0] == LCD_PINMUX_END)
					break;
				BLPR("bl_pinmux set: %d, 0x%08x\n",
				      bl_pwm->pinmux_set[i][0],
				      bl_pwm->pinmux_set[i][1]);
				i++;
			}
			i = 0;
			while (i < LCD_PINMUX_NUM) {
				if (bl_pwm->pinmux_clr[i][0] == LCD_PINMUX_END)
					break;
				BLPR("bl_pinmux clr: %d, 0x%08x\n",
				      bl_pwm->pinmux_clr[i][0],
				      bl_pwm->pinmux_clr[i][1]);
				i++;
			}
		}
		break;
	case BL_CTRL_PWM_COMBO:
		pwm_combo0 = bconf->bl_pwm_combo0;
		pwm_combo1 = bconf->bl_pwm_combo1;
		if (pwm_combo0->pwm_port == BL_PWM_VS)
			sprintf(propname, "%s", bl_pinmux_str[4]);
		else
			sprintf(propname, "%s", bl_pinmux_str[2]);

		pinmux = bconf->bl_pinmux;
		for (i = 0; i < LCD_PINMX_MAX; i++) {
			if (!pinmux)
				break;
			if (!pinmux->name)
				break;
			if (strncmp(pinmux->name, "invalid", 7) == 0)
				break;
			if (strncmp(pinmux->name, propname,
				    strlen(propname)) == 0) {
				for (j = 0; j < LCD_PINMUX_NUM; j++ ) {
					if (pinmux->pinmux_set[j][0] ==
					    LCD_PINMUX_END)
						break;
					pwm_combo0->pinmux_set[j][0] =
						pinmux->pinmux_set[j][0];
					pwm_combo0->pinmux_set[j][1] =
						pinmux->pinmux_set[j][1];
					set_cnt++;
				}
				for (j = 0; j < LCD_PINMUX_NUM; j++ ) {
					if (pinmux->pinmux_clr[j][0] ==
					    LCD_PINMUX_END)
						break;
					pwm_combo0->pinmux_clr[j][0] =
						pinmux->pinmux_clr[j][0];
					pwm_combo0->pinmux_clr[j][1] =
						pinmux->pinmux_clr[j][1];
					clr_cnt++;
				}
				break;
			}
			pinmux++;
		}
		if (set_cnt < LCD_PINMUX_NUM) {
			pwm_combo0->pinmux_set[set_cnt][0] = LCD_PINMUX_END;
			pwm_combo0->pinmux_set[set_cnt][1] = 0x0;
		}
		if (clr_cnt < LCD_PINMUX_NUM) {
			pwm_combo0->pinmux_clr[clr_cnt][0] = LCD_PINMUX_END;
			pwm_combo0->pinmux_clr[clr_cnt][1] = 0x0;
		}

		if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
			i = 0;
			while (i < LCD_PINMUX_NUM) {
				if (pwm_combo0->pinmux_set[i][0] ==
				    LCD_PINMUX_END)
					break;
				BLPR("pwm_combo0 pinmux_set: %d, 0x%08x\n",
				      pwm_combo0->pinmux_set[i][0],
				      pwm_combo0->pinmux_set[i][1]);
				i++;
			}
			i = 0;
			while (i < LCD_PINMUX_NUM) {
				if (pwm_combo0->pinmux_clr[i][0] ==
				    LCD_PINMUX_END)
					break;
				BLPR("pwm_combo0 pinmux_clr: %d, 0x%08x\n",
				      pwm_combo0->pinmux_clr[i][0],
				      pwm_combo0->pinmux_clr[i][1]);
				i++;
			}
		}

		if (pwm_combo1->pwm_port == BL_PWM_VS)
			sprintf(propname, "%s", bl_pinmux_str[5]);
		else
			sprintf(propname, "%s", bl_pinmux_str[3]);

		pinmux = bconf->bl_pinmux;
		set_cnt = 0;
		clr_cnt = 0;
		for (i = 0; i < LCD_PINMX_MAX; i++) {
			if (!pinmux)
				break;
			if (!pinmux->name)
				break;
			if (strncmp(pinmux->name, "invalid", 7) == 0)
				break;
			if (strncmp(pinmux->name, propname,
				    strlen(propname)) == 0) {
				for (j = 0; j < LCD_PINMUX_NUM; j++ ) {
					if (pinmux->pinmux_set[j][0] ==
					    LCD_PINMUX_END)
						break;
					pwm_combo1->pinmux_set[j][0] =
						pinmux->pinmux_set[j][0];
					pwm_combo1->pinmux_set[j][1] =
						pinmux->pinmux_set[j][1];
					set_cnt++;
				}
				for (j = 0; j < LCD_PINMUX_NUM; j++ ) {
					if (pinmux->pinmux_clr[j][0] ==
					    LCD_PINMUX_END)
						break;
					pwm_combo1->pinmux_clr[j][0] =
						pinmux->pinmux_clr[j][0];
					pwm_combo1->pinmux_clr[j][1] =
						pinmux->pinmux_clr[j][1];
					clr_cnt++;
				}
				break;
			}
			pinmux++;
		}
		if (set_cnt < LCD_PINMUX_NUM) {
			pwm_combo1->pinmux_set[set_cnt][0] = LCD_PINMUX_END;
			pwm_combo1->pinmux_set[set_cnt][1] = 0x0;
		}
		if (clr_cnt < LCD_PINMUX_NUM) {
			pwm_combo1->pinmux_clr[clr_cnt][0] = LCD_PINMUX_END;
			pwm_combo1->pinmux_clr[clr_cnt][1] = 0x0;
		}
		if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
			i = 0;
			while (i < LCD_PINMUX_NUM) {
				if (pwm_combo1->pinmux_set[i][0] == LCD_PINMUX_END)
					break;
				BLPR("pwm_combo1 pinmux_set: %d, 0x%08x\n",
				      pwm_combo1->pinmux_set[i][0],
				      pwm_combo1->pinmux_set[i][1]);
				i++;
			}
			i = 0;
			while (i < LCD_PINMUX_NUM) {
				if (pwm_combo1->pinmux_clr[i][0] == LCD_PINMUX_END)
					break;
				BLPR("pwm_combo1 pinmux_clr: %d, 0x%08x\n",
				      pwm_combo1->pinmux_clr[i][0],
				      pwm_combo1->pinmux_clr[i][1]);
				i++;
			}
		}
		break;
	default:
		break;
	}

	return 0;
}

static int bl_config_load(char *dt_addr, int load_id, struct aml_bl_drv_s *bdrv)
{
	char *bl_off_policy_str, str[30];
	unsigned int temp;
	int ret;

	bdrv->state = 0;

	/* load bl config */
	if (load_id & 0x1) { /* dts */
		if (load_id & 0x10) { /* unifykey */
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
				BLPR("[%d]: load config from unifykey\n", bdrv->index);
			ret = bl_config_load_from_unifykey(dt_addr, bdrv);
		} else { /* dts */
#ifdef CONFIG_OF_LIBFDT
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
				BLPR("[%d]: load config from dts\n", bdrv->index);
			if (bdrv->config.index == 0xff) {
				bdrv->config.method = BL_CTRL_MAX;
				BLPR("[%d]: no backlight exist\n", bdrv->index);
				return -1;
			}
			ret = bl_config_load_from_dts(dt_addr, bdrv);
#endif
		}
	} else { /* bsp */
		if (load_id & 0x10) { /* unifykey */
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
				BLPR("[%d]: load config from unifykey\n", bdrv->index);
			ret = bl_config_load_from_unifykey(dt_addr, bdrv);
		} else { /* bsp */
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
				BLPR("[%d]: load config from bsp\n", bdrv->index);
			ret = bl_config_load_from_bsp(bdrv);
		}
	}
	if (ret) {
		bdrv->config.method = BL_CTRL_MAX;
		BLPR("[%d]: invalid backlight config\n", bdrv->index);
		return -1;
	}
	bl_pinmux_load_from_bsp(bdrv);
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
		bl_config_print(bdrv);
	} else {
		BLPR("[%d]: name: %s, method: %d\n",
		      bdrv->index, bdrv->config.name, bdrv->config.method);
	}

	/* get bl_off_policy */
	bdrv->bl_off_policy = BL_OFF_POLICY_NONE;
	if (bdrv->index == 0)
		sprintf(str, "bl_off");
	else
		sprintf(str, "bl%d_off", bdrv->index);
	bl_off_policy_str = env_get(str);
	if (bl_off_policy_str) {
		if (strncmp(bl_off_policy_str, "none", 2) == 0)
			bdrv->bl_off_policy = BL_OFF_POLICY_NONE;
		else if (strncmp(bl_off_policy_str, "always", 2) == 0)
			bdrv->bl_off_policy = BL_OFF_POLICY_ALWAYS;
		else if (strncmp(bl_off_policy_str, "once", 2) == 0)
			bdrv->bl_off_policy = BL_OFF_POLICY_ONCE;
		BLPR("[%d]: bl_off_policy: %s\n", bdrv->index, bl_off_policy_str);
	}

	/* get bl_level */
	if (bdrv->index == 0)
		sprintf(str, "bl_level");
	else
		sprintf(str, "bl%d_level", bdrv->index);
	temp = env_get_ulong(str, 10, 0xffff);
	if (temp != 0xffff) {
		bdrv->config.level_default = temp;
		BLPR("[%d]: bl_level: %d\n", bdrv->index, bdrv->config.level_default);
	}

	/* get factory_bl_on_delay */
	if (bdrv->index == 0)
		sprintf(str, "factory_bl_on_delay");
	else
		sprintf(str, "factory_bl%d_on_delay", bdrv->index);
	temp = env_get_ulong(str, 10, 0xffff);
	if (temp != 0xffff) {
		bdrv->factory_bl_on_delay = temp;
		BLPR("[%d]: factory_bl_on_delay: %d\n", bdrv->index, bdrv->factory_bl_on_delay);
	}

	return 0;
}

static int lcd_bl_init_load_from_dts(char *dtaddr, struct aml_bl_drv_s *bdrv)
{
#ifdef CONFIG_OF_LIBFDT
	int parent_offset;
	char *propdata, *p, snode[15];
	const char *str;
	int i, j;
	int ret = 0;

	if (bdrv->index == 0)
		sprintf(snode, "/backlight");
	else
		sprintf(snode, "/backlight%d", bdrv->index);

	/* check key_valid */
	parent_offset = fdt_path_offset(dtaddr, snode);
	if (parent_offset < 0) {
		BLERR("not find %s node: %s\n", snode, fdt_strerror(parent_offset));
		return -1;
	}
	propdata = (char *)fdt_getprop(dtaddr, parent_offset, "key_valid", NULL);
	if (!propdata) {
		BLERR("failed to get key_valid\n");
		bdrv->key_valid = 0;
	} else {
		bdrv->key_valid = be32_to_cpup((u32*)propdata);
	}

	propdata = (char *)fdt_getprop(dtaddr, parent_offset, "status", NULL);
	if (!propdata) {
		BLPR("failed to get status, default to disabled\n");
		return -1;
	} else {
		if (strncmp(propdata, "okay", 2)) {
			BLPR("status disabled\n");
			return -1;
		}
	}

	/* gpio */
	i = 0;
	propdata = (char *)fdt_getprop(dtaddr, parent_offset, "bl_gpio_names", NULL);
	if (!propdata) {
		BLERR("failed to get bl_gpio_names\n");
	} else {
		p = propdata;
		while (i < BL_GPIO_NUM_MAX) {
			if (i > 0)
				p += strlen(p) + 1;
			str = p;
			if (strlen(str) == 0)
				break;
			strncpy(bdrv->config.gpio_name[i], str, (LCD_CPU_GPIO_NAME_MAX - 1));
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
				BLPR("i=%d, gpio=%s\n", i, bdrv->config.gpio_name[i]);
			i++;
		}
	}

	for (j = i; j < BL_GPIO_NUM_MAX; j++)
		strcpy(bdrv->config.gpio_name[j], "invalid");

	return ret;
#elif
	return -1;
#endif
}

static int lcd_bl_init_load_from_bsp(struct aml_bl_drv_s *bdrv)
{
	return 0;
}

int aml_bl_probe(char *dtaddr, int load_id)
{
	struct aml_lcd_data_s *pdata = aml_lcd_get_data();
	struct aml_bl_drv_s *bdrv;
	int load_id_bl, load_id_temp;
	int i, ret;

	if (!pdata) {
		BLERR("%s: pdata is NULL\n", __func__);
		return -1;
	}

	aml_bl_pwm_reg_config_init(pdata);

	load_id_bl = load_id;
	for (i = 0; i < LCD_MAX_DRV; i++) {
		if (bl_index_lut[i] >= BL_INDEX_INVALID)
			continue;

		if (!bl_driver[i]) {
			bl_driver[i] = (struct aml_bl_drv_s *)
				malloc(sizeof(struct aml_bl_drv_s));
			if (!bl_driver[i]) {
				BLERR("%s: Not enough memory\n", __func__);
				return -1;
			}
		}
		bdrv = bl_driver[i];
		memset(bdrv, 0, sizeof(struct aml_bl_drv_s));
		bdrv->index = i;
		bdrv->data = pdata;

		/* default config */
		bdrv->config.index = bl_index_lut[i];
		bdrv->config.method = BL_CTRL_MAX;
		bdrv->config.en_gpio = 0xff;
		bdrv->config.extern_index = 0xff;
		bdrv->factory_bl_on_delay = -1;

		if (load_id_bl & 0x1) {
			ret = lcd_bl_init_load_from_dts(dtaddr, bdrv);
			if (ret) {
				free(bl_driver[i]);
				bl_driver[i] = NULL;
				return -1;
			}
		} else {
			ret = lcd_bl_init_load_from_bsp(bdrv);
			if (ret) {
				free(bl_driver[i]);
				bl_driver[i] = NULL;
				return -1;
			}
		}
		load_id_temp = load_id_bl & 0xff;
		if ((load_id_bl & (1 << 8)) == 0) {
			if (bdrv->key_valid)
				load_id_temp |= (1 << 4);
			else
				load_id_temp &= ~(1 << 4);
		}

		/* load bl config */
		bl_config_load(dtaddr, load_id_temp, bdrv);
		bl_power_init_off(bdrv);
	}

	return 0;
}

int aml_bl_remove(void)
{
	int i = 0;

	for (i = 0; i < LCD_MAX_DRV; i++) {
		if (bl_driver[i])
			free(bl_driver[i]);
		bl_driver[i] = NULL;
	}

	return 0;
}

int aml_bl_index_add(int drv_index, int conf_index)
{
	if (drv_index >= LCD_MAX_DRV) {
		BLERR("%s: invalid drv_index: %d\n", __func__, drv_index);
		return -1;
	}

	bl_index_lut[drv_index] = conf_index;
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
		BLPR("%s: drv_index %d, config index: %d\n",
			__func__, drv_index, conf_index);
	}
	return 0;
}

int aml_bl_index_remove(int drv_index)
{
	if (drv_index >= LCD_MAX_DRV) {
		BLERR("%s: invalid drv_index: %d\n", __func__, drv_index);
		return -1;
	}

	bl_index_lut[drv_index] = BL_INDEX_INVALID;
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		BLPR("%s: drv_index %d\n", __func__, drv_index);

	return 0;
}

int aml_bl_init(void)
{
	int i;

	for (i = 0; i < LCD_MAX_DRV; i++) {
		if (bl_driver[i])
			free(bl_driver[i]);
		bl_driver[i] = NULL;
		bl_index_lut[i] = BL_INDEX_INVALID;
	}

	return 0;
}

void aml_bl_driver_enable(int index)
{
	struct aml_bl_drv_s *bdrv;

	bdrv = aml_bl_get_driver(index);
	if (!bdrv)
		return;

	bl_pwm_config_update(bdrv);
	bl_set_level(bdrv, bdrv->config.level_default);
	bl_power_ctrl(bdrv, 1);
}

void aml_bl_driver_disable(int index)
{
	struct aml_bl_drv_s *bdrv;

	bdrv = aml_bl_get_driver(index);
	if (!bdrv)
		return;

	bl_power_ctrl(bdrv, 0);
}

void aml_bl_set_level(int index, unsigned int level)
{
	struct aml_bl_drv_s *bdrv;

	bdrv = aml_bl_get_driver(index);
	if (!bdrv)
		return;

	bl_set_level(bdrv, level);
}

unsigned int aml_bl_get_level(int index)
{
	struct aml_bl_drv_s *bdrv;

	bdrv = aml_bl_get_driver(index);
	if (!bdrv)
		return 0;

	return bdrv->level;
}

void aml_bl_config_print(int index)
{
	struct aml_bl_drv_s *bdrv;

	bdrv = aml_bl_get_driver(index);
	if (!bdrv)
		return;

	bl_config_print(bdrv);
}
