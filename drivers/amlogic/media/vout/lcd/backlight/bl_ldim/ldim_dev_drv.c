// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <spi.h>
#include <asm/arch/gpio.h>
#include <fdtdec.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include <amlogic/media/vout/lcd/aml_bl.h>
#include <amlogic/media/vout/lcd/bl_ldim.h>
#include "../../lcd_unifykey.h"
#include "../../lcd_common.h"
#include "../../lcd_reg.h"
#include "../lcd_bl.h"
#include "ldim_drv.h"
#include "ldim_dev_drv.h"

void ldim_gpio_set(struct ldim_dev_driver_s *dev_drv, int index, int value)
{
	int gpio;
	char *str;

	if (index >= BL_GPIO_NUM_MAX) {
		LDIMERR("%s: invalid index %d\n", __func__, index);
		return;
	}
	str = dev_drv->gpio_name[index];
	gpio = lcd_gpio_name_map_num(str);
	switch (value) {
	case LCD_GPIO_OUTPUT_LOW:
	case LCD_GPIO_OUTPUT_HIGH:
		lcd_gpio_set(gpio, value);
		break;
	case LCD_GPIO_INPUT:
	default:
		value = LCD_GPIO_INPUT;
		lcd_gpio_set(gpio, value);
		break;
	}
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		LDIMPR("set gpio %s[%d] value: %d\n", str, index, value);
}

int ldim_gpio_get(struct ldim_dev_driver_s *dev_drv, int index)
{
	int gpio;
	char *str;
	int value;

	if (index >= BL_GPIO_NUM_MAX) {
		LDIMERR("%s: invalid index %d\n", __func__, index);
		return -1;
	}
	str = dev_drv->gpio_name[index];
	gpio = lcd_gpio_name_map_num(str);
	value = lcd_gpio_input_get(gpio);
	return value;
}
/* *************************************** */

void ldim_set_duty_pwm(struct bl_pwm_config_s *bl_pwm)
{
	if (bl_pwm->pwm_port >= BL_PWM_MAX)
		return;
	if (bl_pwm->pwm_duty_max == 0)
		return;

	bl_pwm->pwm_level = (bl_pwm->pwm_cnt * bl_pwm->pwm_duty +
		((bl_pwm->pwm_duty_max + 1) >> 1)) / bl_pwm->pwm_duty_max;

	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
		LDIMPR("pwm_port 0x%x: duty=%d, duty_max=%d, duty_min=%d\n",
		       bl_pwm->pwm_port, bl_pwm->pwm_duty,
		       bl_pwm->pwm_duty_max, bl_pwm->pwm_duty_min);
	}

	bl_set_pwm(bl_pwm);
}

void ldim_pwm_off(struct bl_pwm_config_s *bl_pwm)
{
	if (bl_pwm->pwm_port >= BL_PWM_MAX)
		return;

	bl_pwm_en(bl_pwm, 0);
}

static int ldim_set_pinmux_pwm(int status, struct bl_pwm_config_s *bl_pwm)
{
	int i;

	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		LDIMPR("%s: %d\n", __func__, status);

	if (status) {
		bl_pwm_en(bl_pwm, 1);
		/* set pinmux */
		bl_pwm->pinmux_flag = 1;
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (bl_pwm->pinmux_clr[i][0] == LCD_PINMUX_END)
				break;
			lcd_pinmux_clr_mask(bl_pwm->pinmux_clr[i][0],
					    bl_pwm->pinmux_clr[i][1]);
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
				LDIMPR("%s: port=0x%x, pinmux_clr=%d,0x%08x\n",
				       __func__, bl_pwm->pwm_port,
				       bl_pwm->pinmux_clr[i][0],
				       bl_pwm->pinmux_clr[i][1]);
			}
			i++;
		}
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (bl_pwm->pinmux_set[i][0] == LCD_PINMUX_END)
				break;
			lcd_pinmux_set_mask(bl_pwm->pinmux_set[i][0],
					    bl_pwm->pinmux_set[i][1]);
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
				LDIMPR("%s: port=0x%x, pinmux_set=%d,0x%08x\n",
				       __func__, bl_pwm->pwm_port,
				       bl_pwm->pinmux_set[i][0],
				       bl_pwm->pinmux_set[i][1]);
			}
			i++;
		}
	} else {
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (bl_pwm->pinmux_set[i][0] == LCD_PINMUX_END)
				break;
			lcd_pinmux_clr_mask(bl_pwm->pinmux_set[i][0],
					    bl_pwm->pinmux_set[i][1]);
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
				LDIMPR("%s: port=0x%x, pinmux_clr=%d,0x%08x\n",
				       __func__, bl_pwm->pwm_port,
				       bl_pwm->pinmux_set[i][0],
				       bl_pwm->pinmux_set[i][1]);
			}
			i++;
		}
		bl_pwm->pinmux_flag = 0;

		bl_pwm_en(bl_pwm, 0);
	}

	return 0;
}

static int ldim_pwm_pinmux_ctrl(struct ldim_dev_driver_s *dev_drv, int status)
{
	if (dev_drv->ldim_pwm_config.pwm_port >= BL_PWM_MAX)
		return 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		LDIMPR("%s: %d\n", __func__, status);

	ldim_set_pinmux_pwm(status, &dev_drv->ldim_pwm_config);
	if (dev_drv->analog_pwm_config.pwm_port < BL_PWM_VS)
		ldim_set_pinmux_pwm(status, &dev_drv->analog_pwm_config);

	return 0;
}

static void ldim_dev_init_dynamic_print(struct ldim_dev_driver_s *dev_drv, int flag)
{
	int i, j, max_len;
	unsigned char cmd_size;
	unsigned char *table;

	if (flag) {
		printf("power on:\n");
		table = dev_drv->init_on;
		max_len = dev_drv->init_on_cnt;
	} else {
		printf("power off:\n");
		table = dev_drv->init_off;
		max_len = dev_drv->init_off_cnt;
	}
	if (!table) {
		LDIMERR("init_table %d is NULL\n", flag);
		return;
	}

	i = 0;
	while ((i + 1) < max_len) {
		if (table[i] == LCD_EXT_CMD_TYPE_END) {
			printf("  0x%02x,%d,\n", table[i], table[i+1]);
			break;
		}
		cmd_size = table[i+1];
		printf("  0x%02x,%d,", table[i], cmd_size);
		if (cmd_size == 0)
			goto init_table_dynamic_print_next;
		if (i + 2 + cmd_size > max_len) {
			printf("cmd_size out of support\n");
			break;
		}

		if (table[i] == LCD_EXT_CMD_TYPE_DELAY) {
			for (j = 0; j < cmd_size; j++)
				printf("%d,", table[i+2+j]);
		} else if (table[i] == LCD_EXT_CMD_TYPE_CMD) {
			for (j = 0; j < cmd_size; j++)
				printf("0x%02x,", table[i+2+j]);
		} else if (table[i] == LCD_EXT_CMD_TYPE_CMD_DELAY) {
			for (j = 0; j < (cmd_size - 1); j++)
				printf("0x%02x,", table[i+2+j]);
			printf("%d,", table[i+cmd_size+1]);
		} else {
			for (j = 0; j < cmd_size; j++)
				printf("0x%02x,", table[i+2+j]);
		}
init_table_dynamic_print_next:
		printf("\n");
		i += (cmd_size + 2);
	}
}

static void ldim_device_config_print(struct ldim_dev_driver_s *dev_drv)
{
	struct bl_pwm_config_s *bl_pwm;
	int i;

	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return;
	}

	printf("\ndev_name               = %s\n"
		"key_valid              = %d\n"
		"index                  = %d\n"
		"dim_min                = 0x%03x\n"
		"dim_max                = 0x%03x\n"
		"bl_zone                = %d\n\n"
		"sub_cnt                = %d\n"
		"en_gpio                = %d\n"
		"en_gpio_on             = %d\n"
		"en_gpio_off            = %d\n\n",
		dev_drv->name,
		dev_drv->key_valid,
		dev_drv->index,
		dev_drv->dim_min,
		dev_drv->dim_max,
		dev_drv->zone_num,
		dev_drv->chip_cnt,
		dev_drv->en_gpio,
		dev_drv->en_gpio_on,
		dev_drv->en_gpio_off);

	printf("zone_mapping:\n");
	for (i = 0; i < dev_drv->zone_num; i++)
		printf("%d,", dev_drv->bl_mapping[i]);
	printf("\n\n");

	switch (dev_drv->type) {
	case LDIM_DEV_TYPE_SPI:
		printf("spi_dev                = 0x%p\n"
			"spi_modalias           = %s\n"
			"spi_mode               = %d\n"
			"spi_max_speed_hz       = %d\n"
			"spi_bus_num            = %d\n"
			"spi_chip_select        = %d\n"
			"cs_hold_delay          = %d\n"
			"cs_clk_delay           = %d\n"
			"write_check            = %d\n\n",
			dev_drv->spi_info.spi,
			dev_drv->spi_info.modalias,
			dev_drv->spi_info.mode,
			dev_drv->spi_info.max_speed_hz,
			dev_drv->spi_info.bus_num,
			dev_drv->spi_info.chip_select,
			dev_drv->cs_hold_delay,
			dev_drv->cs_clk_delay,
			dev_drv->write_check);
		break;
	default:
		break;
	}

	bl_pwm = &dev_drv->ldim_pwm_config;
	if (bl_pwm->pwm_port < BL_PWM_MAX) {
		printf("ldim_pwm_port          = 0x%x\n"
			"ldim_pwm_pol           = %d\n"
			"ldim_pwm_freq          = %d\n"
			"ldim_pwm_duty          = %d%%\n"
			"ldim_pwm_pinmux_flag   = %d\n\n",
			bl_pwm->pwm_port, bl_pwm->pwm_method,
			bl_pwm->pwm_freq, bl_pwm->pwm_duty,
			bl_pwm->pinmux_flag);
	}

	bl_pwm = &dev_drv->analog_pwm_config;
	if (bl_pwm->pwm_port < BL_PWM_VS) {
		printf("analog_pwm_port        = 0x%x\n"
			"analog_pwm_pol         = %d\n"
			"analog_pwm_freq        = %d\n"
			"analog_pwm_duty        = %d%%\n"
			"analog_pwm_duty_max    = %d%%\n"
			"analog_pwm_duty_min    = %d%%\n"
			"analog_pwm_pinmux_flag = %d\n\n",
			bl_pwm->pwm_port, bl_pwm->pwm_method,
			bl_pwm->pwm_freq, bl_pwm->pwm_duty,
			bl_pwm->pwm_duty_max, bl_pwm->pwm_duty_min,
			bl_pwm->pinmux_flag);
	}

	if (dev_drv->cmd_size > 0) {
		printf("init_loaded            = %d\n"
			"cmd_size               = %d\n"
			"init_on_cnt            = %d\n"
			"init_off_cnt           = %d\n",
			dev_drv->init_loaded,
			dev_drv->cmd_size,
			dev_drv->init_on_cnt,
			dev_drv->init_off_cnt);
		if (dev_drv->cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC) {
			ldim_dev_init_dynamic_print(dev_drv, 1);
			ldim_dev_init_dynamic_print(dev_drv, 0);
		}
	}
}

static int ldim_pinmux_load_from_bsp(struct aml_ldim_driver_s *ldim_drv, const char *str,
				     struct bl_pwm_config_s *bl_pwm)
{
	unsigned int i, j;
	int set_cnt = 0, clr_cnt = 0;
	struct lcd_pinmux_ctrl_s *pinmux;

	pinmux = ldim_drv->data->dft_conf[0]->ldim_pinmux;
	if (!pinmux) {
		LDIMERR("%s: ldim_pinmux is null\n", __func__);
		return -1;
	}
	for (i = 0; i < LCD_PINMX_MAX; i++) {
		if (strncmp(pinmux->name, "invalid", 7) == 0)
			break;
		if (strncmp(pinmux->name, str, strlen(str)) == 0) {
			for (j = 0; j < LCD_PINMUX_NUM; j++ ) {
				if (pinmux->pinmux_set[j][0] == LCD_PINMUX_END)
					break;
				bl_pwm->pinmux_set[j][0] = pinmux->pinmux_set[j][0];
				bl_pwm->pinmux_set[j][1] = pinmux->pinmux_set[j][1];
				set_cnt++;
			}
			for (j = 0; j < LCD_PINMUX_NUM; j++ ) {
				if (pinmux->pinmux_clr[j][0] == LCD_PINMUX_END)
					break;
				bl_pwm->pinmux_clr[j][0] = pinmux->pinmux_clr[j][0];
				bl_pwm->pinmux_clr[j][1] = pinmux->pinmux_clr[j][1];
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
			LDIMPR("%s set: %d, 0x%08x\n",
			       str,
			       bl_pwm->pinmux_set[i][0],
			       bl_pwm->pinmux_set[i][1]);
			i++;
		}
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (bl_pwm->pinmux_clr[i][0] == LCD_PINMUX_END)
				break;
			LDIMPR("%s clr: %d, 0x%08x\n",
			       str,
			       bl_pwm->pinmux_clr[i][0],
			       bl_pwm->pinmux_clr[i][1]);
			i++;
		}
	}

	return 0;
}

static char *ldim_pinmux_str[] = {
	"ldim_pwm_pin",        /* 0 */
	"ldim_pwm_vs_pin",     /* 1 */
	"analog_pwm_pin",      /* 2 */
	"none",
};

static int ldim_pinmux_load(char *dt_addr, struct aml_ldim_driver_s *ldim_drv)
{
	struct bl_pwm_config_s *bl_pwm;
	char *str;
	int ret = 0;

	if (!ldim_drv->dev_drv)
		return -1;

	/* ldim_pwm */
	bl_pwm = &ldim_drv->dev_drv->ldim_pwm_config;
	if (bl_pwm->pwm_port < BL_PWM_MAX) {
		if (bl_pwm->pwm_port == BL_PWM_VS)
			str = ldim_pinmux_str[1];
		else
			str = ldim_pinmux_str[0];
		ret = ldim_pinmux_load_from_bsp(ldim_drv, str, bl_pwm);
		if (ret)
			return ret;
	}

	/* analog_pwm */
	bl_pwm = &ldim_drv->dev_drv->analog_pwm_config;
	if (bl_pwm->pwm_port < BL_PWM_VS) {
		str = ldim_pinmux_str[2];
		ret = ldim_pinmux_load_from_bsp(ldim_drv, str, bl_pwm);
	}

	return ret;
}

static int ldim_dev_zone_mapping_load(struct ldim_dev_driver_s *dev_drv,
				      const char *path)
{
	unsigned int size = 0;
	unsigned char *buf;
	int i, j, ret;

	/* 2byte per zone */
	size = dev_drv->zone_num * 2;
	buf = (unsigned char *)malloc(size);
	if (!buf) {
		LDIMERR("%s: zone_mapping buf invalid\n", __func__);
		return -1;
	}
	memset(buf, 0, size);

	ret = handle_ldim_dev_zone_mapping_get(buf, size, path);
	if (ret) {
		LDIMERR("%s: load zone_mapping path: %s error\n", __func__, path);
		free(buf);
		return -1;
	}

	for (i = 0; i < dev_drv->zone_num; i++) {
		j = 2 * i;
		dev_drv->bl_mapping[i] = buf[j] | (buf[j + 1] << 8);
	}

	LDIMPR("%s: load zone_mapping path: %s finish\n", __func__, path);
	free(buf);
	return 0;
}

static int ldim_dev_init_table_save(struct ldim_dev_driver_s *dev_drv, int flag,
				    unsigned char *table)
{
	if (!dev_drv || !table) {
		LDIMERR("%s: resource error\n", __func__);
		return -1;
	}

	if (flag) {
		if (dev_drv->init_on) {
			free(dev_drv->init_on);
			dev_drv->init_on = NULL;
		}
		dev_drv->init_on =
			(unsigned char *)malloc(sizeof(unsigned char) * dev_drv->init_on_cnt);
		if (!dev_drv->init_on) {
			LDIMERR("%s: Not enough memory\n", __func__);
			return -1;
		}
		memcpy(dev_drv->init_on, table, dev_drv->init_on_cnt);
	} else {
		if (dev_drv->init_off) {
			free(dev_drv->init_off);
			dev_drv->init_off = NULL;
		}
		dev_drv->init_off =
			(unsigned char *)malloc(sizeof(unsigned char) * dev_drv->init_off_cnt);
		if (!dev_drv->init_off) {
			LDIMERR("%s: Not enough memory\n", __func__);
			return -1;
		}
		memcpy(dev_drv->init_off, table, dev_drv->init_off_cnt);
	}

	return 0;
}

#ifdef CONFIG_OF_LIBFDT
static int ldim_dev_init_dynamic_load_dts(char *dtaddr, int nodeoffset,
					  struct ldim_dev_driver_s *dev_drv, int flag)
{
	unsigned char cmd_size, type;
	int i = 0, j, max_len;
	unsigned char *table;
	char propname[20], *propdata;
	int ret;

	if (flag) {
		max_len = LDIM_INIT_ON_MAX;
		sprintf(propname, "init_on");
	} else {
		max_len = LDIM_INIT_OFF_MAX;
		sprintf(propname, "init_off");
	}
	table = (unsigned char *)malloc(sizeof(unsigned char) * max_len);
	if (!table) {
		LDIMERR("%s: Not enough memory\n", __func__);
		return -1;
	}
	memset(table, 0, sizeof(unsigned char) * max_len);
	table[0] = LCD_EXT_CMD_TYPE_END;
	table[1] = 0;

	propdata = (char *)fdt_getprop(dtaddr, nodeoffset, propname, NULL);
	if (!propdata) {
		LDIMERR("%s: get %s failed\n", dev_drv->name, propname);
		table[0] = LCD_EXT_CMD_TYPE_END;
		table[1] = 0;
		if (flag)
			dev_drv->init_on_cnt = 2;
		else
			dev_drv->init_off_cnt = 2;
		goto init_table_dynamic_dts_abort;
	}

	while ((i + 1) < max_len) {
		table[i] = (unsigned char)(be32_to_cpup((((u32 *)propdata) + i)));
		table[i + 1] = (unsigned char)(be32_to_cpup((((u32 *)propdata) + i + 1)));
		type = table[i];
		cmd_size = table[i + 1];
		if (type == LCD_EXT_CMD_TYPE_END)
			break;
		if (cmd_size == 0)
			goto init_table_dynamic_dts_next;
		if ((i + 2 + cmd_size) > max_len) {
			LDIMERR("%s: %s cmd_size out of support\n",
				dev_drv->name, propname);
			table[i] = LCD_EXT_CMD_TYPE_END;
			table[i + 1] = 0;
			if (flag)
				dev_drv->init_on_cnt = i + 2;
			else
				dev_drv->init_off_cnt = i + 2;
			goto init_table_dynamic_dts_abort;
		}
		for (j = 0; j < cmd_size; j++) {
			table[i + 2 + j] =
				(unsigned char)(be32_to_cpup((((u32 *)propdata) + i + 2 + j)));
		}

init_table_dynamic_dts_next:
		i += (cmd_size + 2);
	}
	if (flag)
		dev_drv->init_on_cnt = i + 2;
	else
		dev_drv->init_off_cnt = i + 2;

	ret = ldim_dev_init_table_save(dev_drv, flag, table);
	if (ret)
		goto init_table_dynamic_dts_failed;
	free(table);
	return 0;

init_table_dynamic_dts_abort:
	ldim_dev_init_table_save(dev_drv, flag, table);
	free(table);
	return -1;

init_table_dynamic_dts_failed:
	free(table);
	return -1;
}

static int ldim_dev_init_dynamic_load_ukey(struct ldim_dev_driver_s *dev_drv,
					   unsigned char *p, int key_len,
					   int len, int flag)
{
	unsigned char cmd_size = 0;
	int i = 0, j, max_len, ret = 0;
	unsigned char *table, *buf;
	char propname[20];

	if (flag) {
		max_len = LDIM_INIT_ON_MAX;
		sprintf(propname, "init_on");
		buf = p;
	} else {
		max_len = LDIM_INIT_OFF_MAX;
		sprintf(propname, "init_off");
		buf = p + dev_drv->init_on_cnt;
	}
	table = (unsigned char *)malloc(sizeof(unsigned char) * max_len);
	if (!table) {
		LDIMERR("%s: Not enough memory\n", __func__);
		return -1;
	}
	memset(table, 0, sizeof(unsigned char) * max_len);
	table[0] = LCD_EXT_CMD_TYPE_END;
	table[1] = 0;

	while ((i + 1) < max_len) {
		/* type */
		len += 1;
		ret = lcd_unifykey_len_check(key_len, len);
		if (ret) {
			LDIMERR("%s: get %s failed\n", dev_drv->name, propname);
			table[i] = LCD_EXT_CMD_TYPE_END;
			table[i + 1] = 0;
			goto init_table_dynamic_ukey_err;
		}
		table[i] = *(buf + LCD_UKEY_LDIM_DEV_INIT + i);
		/* cmd_size */
		len += 1;
		ret = lcd_unifykey_len_check(key_len, len);
		if (ret) {
			LDIMERR("%s: get %s failed\n", dev_drv->name, propname);
			table[i] = LCD_EXT_CMD_TYPE_END;
			table[i + 1] = 0;
			goto init_table_dynamic_ukey_err;
		}
		table[i + 1] = *(buf + LCD_UKEY_LDIM_DEV_INIT + i + 1);
		cmd_size = table[i + 1];

		if (table[i] == LCD_EXT_CMD_TYPE_END)
			break;
		if (cmd_size == 0)
			goto init_table_dynamic_ukey_next;
		if ((i + 2 + cmd_size) > max_len) {
			LDIMERR("%s: %s cmd_size out of support\n",
				dev_drv->name, propname);
			table[i] = LCD_EXT_CMD_TYPE_END;
			table[i + 1] = 0;
			goto init_table_dynamic_ukey_err;
		}

		/* step3: data */
		len += cmd_size;
		ret = lcd_unifykey_len_check(key_len, len);
		if (ret) {
			LDIMERR("%s: get %s failed\n", dev_drv->name, propname);
			table[i] = LCD_EXT_CMD_TYPE_END;
			table[i + 1] = 0;
			goto init_table_dynamic_ukey_err;
		}
		for (j = 0; j < cmd_size; j++) {
			table[i + 2 + j] =
				*(buf + LCD_UKEY_LDIM_DEV_INIT + i + 2 + j);
		}
init_table_dynamic_ukey_next:
		i += (cmd_size + 2);
	}
	if (flag)
		dev_drv->init_on_cnt = i + 2;
	else
		dev_drv->init_off_cnt = i + 2;

	ret = ldim_dev_init_table_save(dev_drv, flag, table);
	if (ret)
		goto init_table_dynamic_ukey_err;
	free(table);
	return 0;

init_table_dynamic_ukey_err:
	free(table);
	return -1;
}

static int ldim_dev_get_config_from_dts(struct ldim_dev_driver_s *dev_drv,
					char *dt_addr, int index)
{
	int child_offset;
	char *propname, *propdata;
	const char *str;
	int temp;
	struct bl_pwm_config_s *bl_pwm;
	int i, ret = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		LDIMPR("load ldim_dev config from dts\n");

	/* get device config */
	propname = (char *)malloc(50);
	if (!propname) {
		LDIMERR("%s: propname malloc failed\n", __func__);
		return -1;
	}
	memset(propname, 0, 50);
	sprintf(propname, "/local_dimming_device/ldim_dev_%d", dev_drv->index);
	child_offset = fdt_path_offset(dt_addr, propname);
	if (child_offset < 0) {
		LDIMERR("not find %s node: %s\n", propname, fdt_strerror(child_offset));
		free(propname);
		return -1;
	}
	free(propname);

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "ldim_dev_name", NULL);
	if (!propdata) {
		LDIMERR("failed to get ldim_dev_name\n");
	} else {
		strncpy(dev_drv->name, propdata, 19);
	}
	LDIMPR("get dev config: %s[%d]\n", dev_drv->name, dev_drv->index);

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "type", NULL);
	if (!propdata) {
		LDIMERR("failed to get type\n");
		return -1;
	}
	dev_drv->type = be32_to_cpup((u32 *)propdata);
	LDIMPR("type: %d\n", dev_drv->type);
	if (dev_drv->type >= LDIM_DEV_TYPE_MAX) {
		LDIMERR("invalid type %d\n", dev_drv->type);
		return -1;
	}

	switch (dev_drv->type) {
	case LDIM_DEV_TYPE_SPI:
		/* get spi config */
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "spi_bus_num", NULL);
		if (!propdata)
			LDIMERR("failed to get spi_bus_num\n");
		else
			dev_drv->spi_info.bus_num = be32_to_cpup((u32 *)propdata);

		propdata = (char *)fdt_getprop(dt_addr, child_offset, "spi_chip_select", NULL);
		if (!propdata)
			LDIMERR("failed to get spi_chip_select\n");
		else
			dev_drv->spi_info.chip_select = be32_to_cpup((u32 *)propdata);

		propdata = (char *)fdt_getprop(dt_addr, child_offset, "spi_max_frequency", NULL);
		if (!propdata)
			LDIMERR("failed to get spi_max_frequency\n");
		else
			dev_drv->spi_info.max_speed_hz = be32_to_cpup((u32 *)propdata);

		propdata = (char *)fdt_getprop(dt_addr, child_offset, "spi_mode", NULL);
		if (!propdata)
			LDIMERR("failed to get spi_mode\n");
		else
			dev_drv->spi_info.mode = be32_to_cpup((u32 *)propdata);

		if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
			LDIMPR("spi bus_num=%d, chip_select=%d",
			       dev_drv->spi_info.bus_num,
			       dev_drv->spi_info.chip_select);
			LDIMPR("max_frequency=%d, mode=%d\n",
			       dev_drv->spi_info.max_speed_hz,
			       dev_drv->spi_info.mode);
		}

		propdata = (char *)fdt_getprop(dt_addr, child_offset, "spi_cs_delay", NULL);
		if (!propdata) {
			LDIMERR("failed to get spi_cs_delay\n");
		} else {
			dev_drv->cs_hold_delay = be32_to_cpup((u32 *)propdata);
			dev_drv->cs_clk_delay = be32_to_cpup((((u32 *)propdata) + 1));
		}
		if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
			LDIMPR("cs_hold_delay=%dus, cs_clk_delay=%dus\n",
			       dev_drv->cs_hold_delay,
			       dev_drv->cs_clk_delay);
		}
		break;
	default:
		break;
	}

	/* ldim pwm */
	bl_pwm = &dev_drv->ldim_pwm_config;
	propdata = (char *)fdt_getprop(dt_addr, child_offset, "ldim_pwm_port", NULL);
	if (!propdata) {
		LDIMERR("failed to get ldim_pwm_port\n");
		bl_pwm->pwm_port = BL_PWM_MAX;
	} else {
		bl_pwm->pwm_port = bl_pwm_str_to_num(propdata);
		LDIMPR("ldim_pwm_port: %s(0x%x)\n", propdata, bl_pwm->pwm_port);
	}
	if (bl_pwm->pwm_port < BL_PWM_MAX) {
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "ldim_pwm_attr", NULL);
		if (!propdata) {
			LDIMERR("failed to get ldim_pwm_attr\n");
			bl_pwm->pwm_method = BL_PWM_POSITIVE;
			if (bl_pwm->pwm_port == BL_PWM_VS)
				bl_pwm->pwm_freq = 1;
			else
				bl_pwm->pwm_freq = 60;
			bl_pwm->pwm_duty = 50;
		} else {
			bl_pwm->pwm_method = be32_to_cpup((u32 *)propdata);
			bl_pwm->pwm_freq = be32_to_cpup((((u32 *)propdata) + 1));
			bl_pwm->pwm_duty = be32_to_cpup((((u32 *)propdata) + 2));
		}
		if (bl_pwm->pwm_port == BL_PWM_VS) {
			if (bl_pwm->pwm_freq > 4) {
				LDIMERR("pwm_vs wrong freq %d\n", bl_pwm->pwm_freq);
				bl_pwm->pwm_freq = BL_FREQ_VS_DEFAULT;
			}
		} else {
			if (bl_pwm->pwm_freq > XTAL_HALF_FREQ_HZ)
				bl_pwm->pwm_freq = XTAL_HALF_FREQ_HZ;
		}
		LDIMPR("get ldim_pwm pol = %d, freq = %d, default duty = %d%%\n",
		       bl_pwm->pwm_method, bl_pwm->pwm_freq, bl_pwm->pwm_duty);
	}

	/* analog pwm */
	bl_pwm = &dev_drv->analog_pwm_config;
	propdata = (char *)fdt_getprop(dt_addr, child_offset, "analog_pwm_port", NULL);
	if (!propdata)
		bl_pwm->pwm_port = BL_PWM_MAX;
	else
		bl_pwm->pwm_port = bl_pwm_str_to_num(propdata);
	if (bl_pwm->pwm_port < BL_PWM_MAX) {
		LDIMPR("find analog_pwm_port: %s(0x%x)\n", propdata, bl_pwm->pwm_port);
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "analog_pwm_attr", NULL);
		if (!propdata) {
			LDIMERR("failed to get analog_pwm_attr\n");
			bl_pwm->pwm_method = BL_PWM_POSITIVE;
			if (bl_pwm->pwm_port == BL_PWM_VS)
				bl_pwm->pwm_freq = 1;
			else
				bl_pwm->pwm_freq = 60;
			bl_pwm->pwm_duty_max = 100;
			bl_pwm->pwm_duty_min = 20;
			bl_pwm->pwm_duty = 50;
		} else {
			bl_pwm->pwm_method = be32_to_cpup((u32 *)propdata);
			bl_pwm->pwm_freq = be32_to_cpup((((u32 *)propdata) + 1));
			bl_pwm->pwm_duty_max = be32_to_cpup((((u32 *)propdata) + 2));
			bl_pwm->pwm_duty_min = be32_to_cpup((((u32 *)propdata) + 3));
			bl_pwm->pwm_duty = be32_to_cpup((((u32 *)propdata) + 4));
		}
		if (bl_pwm->pwm_freq > XTAL_HALF_FREQ_HZ)
			bl_pwm->pwm_freq = XTAL_HALF_FREQ_HZ;
		LDIMPR("get analog_pwm pol = %d, freq = %d, duty_max = %d%%, duty_min = %d%%, default duty = %d%%\n",
		       bl_pwm->pwm_method, bl_pwm->pwm_freq,
		       bl_pwm->pwm_duty_max, bl_pwm->pwm_duty_min,
		       bl_pwm->pwm_duty);
	}

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "ldim_pwm_pinmux_sel", NULL);
	if (propdata) {
		LDIMPR("find custom ldim_pwm_pinmux_sel: %s\n", propdata);
		strncpy(dev_drv->pinmux_name, propdata, (LDIM_DEV_NAME_MAX - 1));
	}

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "en_gpio_on_off", NULL);
	if (!propdata) {
		LDIMERR("failed to get en_gpio_on_off\n");
	} else {
		dev_drv->en_gpio = be32_to_cpup((u32 *)propdata);
		dev_drv->en_gpio_on = be32_to_cpup((((u32 *)propdata) + 1));
		dev_drv->en_gpio_off = be32_to_cpup((((u32 *)propdata) + 2));
	}
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
		LDIMPR("en_gpio=%s(%d), en_gpio_on=%d, en_gpio_off=%d\n",
		       dev_drv->gpio_name[dev_drv->en_gpio],
		       dev_drv->en_gpio, dev_drv->en_gpio_on,
		       dev_drv->en_gpio_off);
	}

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "write_check", NULL);
	if (!propdata)
		dev_drv->write_check = 0;
	else
		dev_drv->write_check = (unsigned char)(be32_to_cpup((u32 *)propdata));
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		LDIMPR("write_check=%d\n", dev_drv->write_check);

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "dim_max_min", NULL);
	if (!propdata) {
		LDIMERR("failed to get dim_max_min\n");
		return -1;
	} else {
		dev_drv->dim_max = be32_to_cpup((u32 *)propdata);
		dev_drv->dim_min = be32_to_cpup((((u32 *)propdata) + 1));
	}
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
		LDIMPR("dim_max=0x%03x, dim_min=0x%03x\n",
		       dev_drv->dim_max, dev_drv->dim_min);
	}

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "mcu_header", NULL);
	if (!propdata) {
		LDIMERR("failed to get mcu_header\n");
		dev_drv->mcu_header = 0;
	} else {
		dev_drv->mcu_header = (unsigned int)(be32_to_cpup((u32 *)propdata));
	}
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		LDIMPR("mcu_header=%d\n", dev_drv->mcu_header);

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "mcu_dim", NULL);
	if (!propdata) {
		LDIMERR("failed to get mcu_dim\n");
		dev_drv->mcu_dim = 0;
	} else {
		dev_drv->mcu_dim = (unsigned int)(be32_to_cpup((u32 *)propdata));
	}
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
		LDIMPR("mcu_header=0x%0x, mcu_dim=0x%x\n",
		       dev_drv->mcu_header, dev_drv->mcu_dim);
	}

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "chip_count", NULL);
	if (!propdata) {
		dev_drv->chip_cnt = 1;
	} else {
		dev_drv->chip_cnt = be32_to_cpup((u32 *)propdata);
		LDIMPR("chip_count: %d\n", dev_drv->chip_cnt);
	}

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "ldim_zone_mapping_path", NULL);
	if (propdata) {
		LDIMPR("%s:find custom ldim_zone_mapping_path\n", __func__);
		str = propdata;
		ret = ldim_dev_zone_mapping_load(dev_drv, str);
		if (ret) {
			for (i = 0; i < dev_drv->zone_num; i++)
				dev_drv->bl_mapping[i] = (unsigned short)i;
		}
		goto ldim_dev_get_config_from_dts_next;
	}
	propdata = (char *)fdt_getprop(dt_addr, child_offset, "ldim_zone_mapping", NULL);
	if (!propdata) {
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "ldim_region_mapping", NULL);
		if (!propdata) {
			for (i = 0; i < dev_drv->zone_num; i++)
				dev_drv->bl_mapping[i] = (unsigned short)i;
			goto ldim_dev_get_config_from_dts_next;
		}
	}
	LDIMPR("%s:find custom ldim_zone_mapping\n", __func__);
	for (i = 0; i < dev_drv->zone_num; i++)
		dev_drv->bl_mapping[i] = (unsigned short)be32_to_cpup((((u32 *)propdata) + i));

ldim_dev_get_config_from_dts_next:
	/* get init_cmd */
	propdata = (char *)fdt_getprop(dt_addr, child_offset, "cmd_size", NULL);
	if (!propdata) {
		LDIMPR("no cmd_size\n");
	} else {
		temp = be32_to_cpup((u32 *)propdata);
		dev_drv->cmd_size = (unsigned char)temp;
	}
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		LDIMPR("cmd_size=%d\n", dev_drv->cmd_size);
	if (dev_drv->cmd_size != LCD_EXT_CMD_SIZE_DYNAMIC)
		goto ldim_dev_get_config_from_dts_end;

	ret = ldim_dev_init_dynamic_load_dts(dt_addr, child_offset, dev_drv, 1);
	if (ret)
		goto ldim_dev_get_config_from_dts_end;
	ret = ldim_dev_init_dynamic_load_dts(dt_addr, child_offset, dev_drv, 0);
	if (ret)
		goto ldim_dev_get_config_from_dts_end;
	dev_drv->init_loaded = 1;

ldim_dev_get_config_from_dts_end:
	return ret;
}

static int ldim_dev_get_config_from_ukey(struct ldim_dev_driver_s *dev_drv)
{
	unsigned char *para, *p;
	int key_len, len;
	const char *str;
	unsigned int temp;
	struct lcd_unifykey_header_s ldev_header;
	struct bl_pwm_config_s *bl_pwm;
	int i, ret = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		LDIMPR("load ldim_dev config from unifykey\n");

	key_len = LCD_UKEY_LDIM_DEV_SIZE;
	para = (unsigned char *)malloc(key_len * (sizeof(unsigned char)));
	if (!para)
		return -1;
	memset(para, 0, key_len * (sizeof(unsigned char)));

	ret = lcd_unifykey_get("ldim_dev", para, &key_len);
	if (ret < 0) {
		free(para);
		return -1;
	}

	/* step 1: check header */
	len = LCD_UKEY_HEAD_SIZE;
	ret = lcd_unifykey_len_check(key_len, len);
	if (ret < 0) {
		LDIMERR("unifykey header length is incorrect\n");
		free(para);
		return -1;
	}

	lcd_unifykey_header_check(para, &ldev_header);
	LDIMPR("unifykey version: 0x%04x\n", ldev_header.version);
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
		LDIMPR("unifykey header:\n");
		LDIMPR("crc32             = 0x%08x\n", ldev_header.crc32);
		LDIMPR("data_len          = %d\n", ldev_header.data_len);
	}

	/* step 2: check backlight parameters */
	ret = lcd_unifykey_len_check(key_len, len);
	if (ret < 0) {
		LDIMERR("unifykey length is incorrect\n");
		free(para);
		return -1;
	}

	/* basic: 30byte */
	p = para;
	str = (const char *)(p + LCD_UKEY_HEAD_SIZE);
	strncpy(dev_drv->name, str, (LDIM_DEV_NAME_MAX - 1));

	/* interface (25Byte) */
	dev_drv->type = *(p + LCD_UKEY_LDIM_DEV_IF_TYPE);
	if (dev_drv->type >= LDIM_DEV_TYPE_MAX) {
		LDIMERR("invalid type %d\n", dev_drv->type);
		free(para);
		return -1;
	}

	switch (dev_drv->type) {
	case LDIM_DEV_TYPE_SPI:
		dev_drv->spi_info.bus_num = *(p + LCD_UKEY_LDIM_DEV_IF_ATTR_0);
		dev_drv->spi_info.chip_select = *(p + LCD_UKEY_LDIM_DEV_IF_ATTR_1);
		dev_drv->spi_info.max_speed_hz =
			(*(p + LCD_UKEY_LDIM_DEV_IF_FREQ) |
			((*(p + LCD_UKEY_LDIM_DEV_IF_FREQ + 1)) << 8) |
			((*(p + LCD_UKEY_LDIM_DEV_IF_FREQ + 2)) << 16) |
			((*(p + LCD_UKEY_LDIM_DEV_IF_FREQ + 3)) << 24));
		dev_drv->spi_info.mode = *(p + LCD_UKEY_LDIM_DEV_IF_ATTR_2);
		dev_drv->cs_hold_delay =
			(*(p + LCD_UKEY_LDIM_DEV_IF_ATTR_3) |
			((*(p + LCD_UKEY_LDIM_DEV_IF_ATTR_3 + 1)) << 8));
		dev_drv->cs_clk_delay =
			(*(p + LCD_UKEY_LDIM_DEV_IF_ATTR_4) |
			((*(p + LCD_UKEY_LDIM_DEV_IF_ATTR_4 + 1)) << 8));
		if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
			LDIMPR("spi bus_num: %d\n", dev_drv->spi_info.bus_num);
			LDIMPR("spi chip_select: %d\n",
			       dev_drv->spi_info.chip_select);
			LDIMPR("spi max_speed_hz: %d\n",
			       dev_drv->spi_info.max_speed_hz);
			LDIMPR("spi mode: %d\n", dev_drv->spi_info.mode);
			LDIMPR("cs_hold_delay=%dus, cs_clk_delay=%dus\n",
			       dev_drv->cs_hold_delay,
			       dev_drv->cs_clk_delay);
		}
		break;
	default:
		break;
	}

	/* pwm (48Byte) */
	bl_pwm = &dev_drv->ldim_pwm_config;
	bl_pwm->pwm_port = *(p + LCD_UKEY_LDIM_DEV_PWM_VS_PORT);
	if (bl_pwm->pwm_port < BL_PWM_MAX) {
		bl_pwm->pwm_method = *(p + LCD_UKEY_LDIM_DEV_PWM_VS_POL);
		bl_pwm->pwm_freq =
			(*(p + LCD_UKEY_LDIM_DEV_PWM_VS_FREQ) |
			((*(p + LCD_UKEY_LDIM_DEV_PWM_VS_FREQ + 1)) << 8) |
			((*(p + LCD_UKEY_LDIM_DEV_PWM_VS_FREQ + 2)) << 16) |
			((*(p + LCD_UKEY_LDIM_DEV_PWM_VS_FREQ + 3)) << 24));
		bl_pwm->pwm_duty =
			(*(p + LCD_UKEY_LDIM_DEV_PWM_VS_DUTY) |
			((*(p + LCD_UKEY_LDIM_DEV_PWM_VS_DUTY + 1)) << 8));

		if (bl_pwm->pwm_port == BL_PWM_VS) {
			if (bl_pwm->pwm_freq > 4) {
				LDIMERR("pwm_vs wrong freq %d\n", bl_pwm->pwm_freq);
				bl_pwm->pwm_freq = BL_FREQ_VS_DEFAULT;
			}
		} else {
			if (bl_pwm->pwm_freq > XTAL_HALF_FREQ_HZ)
				bl_pwm->pwm_freq = XTAL_HALF_FREQ_HZ;
		}
		LDIMPR("get ldim_pwm pol = %d, freq = %d, dft duty = %d%%\n",
		       bl_pwm->pwm_method, bl_pwm->pwm_freq, bl_pwm->pwm_duty);
	}

	bl_pwm = &dev_drv->analog_pwm_config;
	bl_pwm->pwm_port = *(p + LCD_UKEY_LDIM_DEV_PWM_ADJ_PORT);
	if (bl_pwm->pwm_port < BL_PWM_VS) {
		bl_pwm->pwm_method = *(p + LCD_UKEY_LDIM_DEV_PWM_ADJ_POL);
		bl_pwm->pwm_freq =
			(*(p + LCD_UKEY_LDIM_DEV_PWM_ADJ_FREQ) |
			((*(p + LCD_UKEY_LDIM_DEV_PWM_ADJ_FREQ + 1)) << 8) |
			((*(p + LCD_UKEY_LDIM_DEV_PWM_ADJ_FREQ + 2)) << 16) |
			((*(p + LCD_UKEY_LDIM_DEV_PWM_ADJ_FREQ + 3)) << 24));
		bl_pwm->pwm_duty =
			(*(p + LCD_UKEY_LDIM_DEV_PWM_ADJ_DUTY) |
			((*(p + LCD_UKEY_LDIM_DEV_PWM_ADJ_DUTY + 1)) << 8));
		bl_pwm->pwm_duty_max =
			(*(p + LCD_UKEY_LDIM_DEV_PWM_ADJ_ATTR_0) |
			((*(p + LCD_UKEY_LDIM_DEV_PWM_ADJ_ATTR_0 + 1)) << 8));
		bl_pwm->pwm_duty_min =
			(*(p + LCD_UKEY_LDIM_DEV_PWM_ADJ_ATTR_1) |
			((*(p + LCD_UKEY_LDIM_DEV_PWM_ADJ_ATTR_1 + 1)) << 8));

		if (bl_pwm->pwm_freq > XTAL_HALF_FREQ_HZ)
			bl_pwm->pwm_freq = XTAL_HALF_FREQ_HZ;
		LDIMPR("get analog_pwm pol = %d, freq = %d\n",
			bl_pwm->pwm_method, bl_pwm->pwm_freq);
		LDIMPR("duty max = %d%%, min = %d%%, default = %d%%\n",
			bl_pwm->pwm_duty_max,
			bl_pwm->pwm_duty_min, bl_pwm->pwm_duty);
	}

	str = (const char *)(p + LCD_UKEY_LDIM_DEV_PINMUX_SEL);
	if (strlen(str) == 0) {
		strcpy(dev_drv->pinmux_name, "invalid");
	} else {
		strncpy(dev_drv->pinmux_name, str, (LDIM_DEV_NAME_MAX - 1));
		LDIMPR("find custom ldim_pwm_pinmux_sel: %s\n", str);
	}

	/* ctrl (271Byte) */
	temp = *(p + LCD_UKEY_LDIM_DEV_EN_GPIO);
	if (temp >= BL_GPIO_NUM_MAX)
		dev_drv->en_gpio = LCD_GPIO_MAX;
	else
		dev_drv->en_gpio = temp;
	dev_drv->en_gpio_on = *(p + LCD_UKEY_LDIM_DEV_EN_GPIO_ON);
	dev_drv->en_gpio_off = *(p + LCD_UKEY_LDIM_DEV_EN_GPIO_OFF);

	temp = *(p + LCD_UKEY_LDIM_DEV_ERR_GPIO);
	if (temp >= BL_GPIO_NUM_MAX) {
		dev_drv->lamp_err_gpio = LCD_GPIO_MAX;
		dev_drv->fault_check = 0;
	} else {
		dev_drv->lamp_err_gpio = temp;
		dev_drv->fault_check = 1;
		ldim_gpio_set(dev_drv, dev_drv->lamp_err_gpio, LCD_GPIO_INPUT);
	}

	dev_drv->write_check = *(p + LCD_UKEY_LDIM_DEV_WRITE_CHECK);

	dev_drv->dim_max =
		(*(p + LCD_UKEY_LDIM_DEV_DIM_MAX) |
		((*(p + LCD_UKEY_LDIM_DEV_DIM_MAX + 1)) << 8));
	dev_drv->dim_min =
		(*(p + LCD_UKEY_LDIM_DEV_DIM_MIN) |
		((*(p + LCD_UKEY_LDIM_DEV_DIM_MIN + 1)) << 8));

	dev_drv->mcu_header =
		(*(p + LCD_UKEY_LDIM_DEV_CUST_ATTR_0) |
		((*(p + LCD_UKEY_LDIM_DEV_CUST_ATTR_0 + 1)) << 8) |
		((*(p + LCD_UKEY_LDIM_DEV_CUST_ATTR_0 + 2)) << 16) |
		((*(p + LCD_UKEY_LDIM_DEV_CUST_ATTR_0 + 3)) << 24));
	dev_drv->mcu_dim =
		(*(p + LCD_UKEY_LDIM_DEV_CUST_ATTR_1) |
		((*(p + LCD_UKEY_LDIM_DEV_CUST_ATTR_1 + 1)) << 8) |
		((*(p + LCD_UKEY_LDIM_DEV_CUST_ATTR_1 + 2)) << 16) |
		((*(p + LCD_UKEY_LDIM_DEV_CUST_ATTR_1 + 3)) << 24));

	dev_drv->chip_cnt =
		(*(p + LCD_UKEY_LDIM_DEV_CHIP_COUNT) |
		((*(p + LCD_UKEY_LDIM_DEV_CHIP_COUNT + 1)) << 8));
	LDIMPR("chip_count: %d\n", dev_drv->chip_cnt);

	str = (const char *)(p + LCD_UKEY_LDIM_DEV_ZONE_MAP_PATH);
	if (strlen(str) == 0) {
		for (i = 0; i < dev_drv->zone_num; i++)
			dev_drv->bl_mapping[i] = (unsigned short)i;
	} else {
		LDIMPR("find custom ldim_zone_mapping\n");
		str = env_get("bl_mapping_path");
		LDIMPR("env_get ldim_zone_mapping = %s\n", str);
		ret = ldim_dev_zone_mapping_load(dev_drv, str);
		if (ret) {
			for (i = 0; i < dev_drv->zone_num; i++)
				dev_drv->bl_mapping[i] = (unsigned short)i;
		}
	}

	dev_drv->cmd_size = *(p + LCD_UKEY_LDIM_DEV_CMD_SIZE);
	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		LDIMPR("%s: cmd_size = %d\n", dev_drv->name, dev_drv->cmd_size);
	if (dev_drv->cmd_size != LCD_EXT_CMD_SIZE_DYNAMIC)
		goto ldim_dev_get_config_from_ukey_end;

	ret = ldim_dev_init_dynamic_load_ukey(dev_drv, p, key_len, len, 1);
	if (ret)
		goto ldim_dev_get_config_from_ukey_err;
	ret = ldim_dev_init_dynamic_load_ukey(dev_drv, p, key_len, len, 0);
	if (ret)
		goto ldim_dev_get_config_from_ukey_err;
	dev_drv->init_loaded = 1;

ldim_dev_get_config_from_ukey_end:
	free(para);
	return 0;

ldim_dev_get_config_from_ukey_err:
	free(para);
	return -1;
}

static int ldim_dev_get_config(char *dt_addr, struct aml_ldim_driver_s *ldim_drv)
{
	struct ldim_dev_driver_s *dev_drv = ldim_drv->dev_drv;
	int parent_offset;
	char *propdata;
	char *p;
	const char *str;
	unsigned int val;
	int i, j, ret = 0;

	if (!dt_addr) {
		LDIMERR("%s: dt_addr is NULL\n", __func__);
		return -1;
	}
	if (!dev_drv) {
		LDIMERR("%s: dev_drv is NULL\n", __func__);
		return -1;
	}

	parent_offset = fdt_path_offset(dt_addr, "/local_dimming_device");
	if (parent_offset < 0) {
		parent_offset = fdt_path_offset(dt_addr, "/local_diming_device");
		if (parent_offset < 0) {
			LDIMERR("not find /local_dimming_device node: %s\n",
				fdt_strerror(parent_offset));
			return -1;
		}
	}
	propdata = (char *)fdt_getprop(dt_addr, parent_offset, "status", NULL);
	if (!propdata) {
		LDIMERR("not find local_dimming_device status, default to disabled\n");
		return -1;
	}
	if (strncmp(propdata, "okay", 2)) {
		LDIMPR("local_dimming_device status disabled\n");
		return -1;
	}

	propdata = (char *)fdt_getprop(dt_addr, parent_offset, "key_valid", NULL);
	if (!propdata) {
		LDIMERR("failed to get key_valid\n");
		val = 0;
	} else {
		val = be32_to_cpup((u32 *)propdata);
	}
	dev_drv->key_valid = val;
	LDIMPR("key_valid: %d\n", dev_drv->key_valid);

	/* init gpio */
	i = 0;
	propdata = (char *)fdt_getprop(dt_addr, parent_offset, "ldim_dev_gpio_names", NULL);
	if (!propdata) {
		LDIMERR("failed to get ldim_dev_gpio_names\n");
	} else {
		p = propdata;
		while (i < BL_GPIO_NUM_MAX) {
			if (i > 0)
				p += strlen(p) + 1;
			str = p;
			if (strlen(str) == 0)
				break;
			strncpy(dev_drv->gpio_name[i], str, (LCD_CPU_GPIO_NAME_MAX - 1));
			if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
				LDIMPR("i=%d, gpio=%s\n", i, dev_drv->gpio_name[i]);
			i++;
		}
	}
	for (j = i; j < BL_GPIO_NUM_MAX; j++)
		strcpy(dev_drv->gpio_name[j], "invalid");

	if (dev_drv->key_valid)
		ret = ldim_dev_get_config_from_ukey(dev_drv);
	else
		ret = ldim_dev_get_config_from_dts(dev_drv, dt_addr, dev_drv->index);
	if (ret)
		return ret;

	ret = ldim_pinmux_load(dt_addr, ldim_drv);

	return ret;
}
#endif

static int ldim_dev_add_driver(struct aml_ldim_driver_s *ldim_drv)
{
	struct ldim_dev_driver_s *dev_drv = ldim_drv->dev_drv;
	int ret = 0;

	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return -1;
	}

	switch (dev_drv->type) {
	case LDIM_DEV_TYPE_SPI:
		ret = ldim_spi_driver_add(dev_drv);
		break;
	default:
		break;
	}
	if (ret)
		return -1;

	ret = -1;
	if (strcmp(dev_drv->name, "iw7027") == 0) {
#ifdef CONFIG_AML_LCD_BL_LDIM_IW7027
		ret = ldim_dev_iw7027_probe(ldim_drv);
#endif
	} else if (strcmp(dev_drv->name, "blmcu") == 0) {
#ifdef CONFIG_AML_LCD_BL_LDIM_BLMCU
		ret = ldim_dev_blmcu_probe(ldim_drv);
#endif
	} else if (strcmp(dev_drv->name, "ob3350") == 0) {
#ifdef CONFIG_AML_LCD_BL_LDIM_OB3350
		ret = ldim_dev_ob3350_probe(ldim_drv);
#endif
	} else if (strcmp(dev_drv->name, "global") == 0) {
		ret = ldim_dev_global_probe(ldim_drv);
	} else {
		LDIMERR("invalid device name: %s\n", dev_drv->name);
		ret = -1;
	}

	if (ret) {
		LDIMERR("add device failed: %s[%d]\n", dev_drv->name, dev_drv->index);
	} else {
		dev_drv->probe_flag = 1;
		LDIMPR("add device: %s[%d]\n", dev_drv->name, dev_drv->index);
	}

	return ret;
}

static int ldim_dev_remove_driver(struct aml_ldim_driver_s *ldim_drv)
{
	struct ldim_dev_driver_s *dev_drv = ldim_drv->dev_drv;

	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return -1;
	}

	if (dev_drv->probe_flag == 0)
		return 0;

	dev_drv->probe_flag = 0;
	if (strcmp(dev_drv->name, "iw7027") == 0) {
#ifdef CONFIG_AML_LCD_BL_LDIM_IW7027
		ldim_dev_iw7027_remove(ldim_drv);
#endif
	} else if (strcmp(dev_drv->name, "blmcu") == 0) {
#ifdef CONFIG_AML_LCD_BL_LDIM_BLMCU
		ldim_dev_blmcu_remove(ldim_drv);
#endif
	} else if (strcmp(dev_drv->name, "ob3350") == 0) {
#ifdef CONFIG_AML_LCD_BL_LDIM_OB3350
		ldim_dev_ob3350_remove(ldim_drv);
#endif
	} else if (strcmp(dev_drv->name, "global") == 0) {
		ldim_dev_global_remove(ldim_drv);
	} else {
		LDIMERR("invalid device name: %s\n", dev_drv->name);
	}

	LDIMPR("remove device: %s[%d]\n", dev_drv->name, dev_drv->index);

	return 0;
}

int aml_ldim_device_probe(char *dt_addr, struct aml_ldim_driver_s *ldim_drv)
{
	struct ldim_dev_driver_s *dev_drv;
	int ret = 0;

	if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL)
		LDIMPR("%s\n", __func__);

	dev_drv = malloc(sizeof(struct ldim_dev_driver_s));
	if (!dev_drv) {
		LDIMERR("%s: dev_drv malloc failed\n", __func__);
		return -1;
	}
	memset(dev_drv, 0, sizeof(struct ldim_dev_driver_s));
	ldim_drv->dev_drv = dev_drv;

	/* device config default */
	dev_drv->probe_flag = 0;
	strcpy(dev_drv->name, "ldim_dev");
	dev_drv->type = LDIM_DEV_TYPE_MAX;
	dev_drv->chip_cnt = 1;
	dev_drv->en_gpio = 0xff;
	dev_drv->en_gpio_on = 1;
	dev_drv->en_gpio_off = 0;
	dev_drv->ldim_pwm_config.pwm_port = BL_PWM_MAX;
	dev_drv->analog_pwm_config.pwm_port = BL_PWM_MAX;
	dev_drv->ldim_pwm_config.pwm_duty_max = 4095;
	dev_drv->analog_pwm_config.pwm_duty_max = 4095;
	strcpy(dev_drv->pinmux_name, "invalid");

	strcpy(dev_drv->spi_info.modalias, "ldim_dev");
	strcpy(dev_drv->spi_info.spi_name, "none");
	dev_drv->spi_info.mode = SPI_MODE_0;
	dev_drv->spi_info.max_speed_hz = 1000000; /* 1MHz */
	dev_drv->spi_info.bus_num = 0; /* SPI bus No. */
	dev_drv->spi_info.chip_select = 0; /* the device index on the spi bus */
	dev_drv->spi_info.wordlen = 8;
	dev_drv->spi_info.spi = NULL;

	dev_drv->index = ldim_drv->config.dev_index;
	dev_drv->bl_row = ldim_drv->config.row;
	dev_drv->bl_col = ldim_drv->config.col;
	dev_drv->zone_num = dev_drv->bl_row * dev_drv->bl_col;
	dev_drv->bl_mapping = malloc(dev_drv->zone_num * sizeof(unsigned short));
	if (!dev_drv->bl_mapping)
		goto ldim_dev_probe_func_fail0;
	memset(dev_drv->bl_mapping, 0, dev_drv->zone_num * sizeof(unsigned short));

#ifdef CONFIG_OF_LIBFDT
	if (!dt_addr) {
		LDIMERR("%s: dt_addr is null\n", __func__);
		goto ldim_dev_probe_func_fail1;
	}
	ret = ldim_dev_get_config(dt_addr, ldim_drv);
	if (ret)
		goto ldim_dev_probe_func_fail1;
#endif

	/* get configs */
	dev_drv->pinmux_ctrl = ldim_pwm_pinmux_ctrl;
	dev_drv->config_print = ldim_device_config_print;

	/* add device driver */
	ret = ldim_dev_add_driver(ldim_drv);

	return ret;

#ifdef CONFIG_OF_LIBFDT
ldim_dev_probe_func_fail1:
	free(dev_drv->bl_mapping);
#endif
ldim_dev_probe_func_fail0:
	free(dev_drv);
	ldim_drv->dev_drv = NULL;
	pr_info("%s: failed\n", __func__);
	return -1;
}

int aml_ldim_device_remove(struct aml_ldim_driver_s *ldim_drv)
{
	ldim_dev_remove_driver(ldim_drv);

	return 0;
}
