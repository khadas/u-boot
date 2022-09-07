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
#include <amlogic/media/vout/lcd/lcd_extern.h>
#include "lcd_extern.h"
#include "../lcd_common.h"
#include "../lcd_reg.h"

static int lcd_ext_dev_cnt[LCD_MAX_DRV];
static int lcd_ext_index_lut[LCD_MAX_DRV][LCD_EXTERN_DEV_MAX];
static struct lcd_extern_driver_s *ext_driver[LCD_MAX_DRV];

struct lcd_extern_driver_s *lcd_extern_get_driver(int drv_index)
{
	if (drv_index >= LCD_MAX_DRV)
		return NULL;

	return ext_driver[drv_index];
}

struct lcd_extern_dev_s *lcd_extern_get_dev(struct lcd_extern_driver_s *edrv, int dev_index)
{
	int i = 0;

	if (!edrv)
		return NULL;
	if (dev_index >= LCD_EXTERN_INDEX_INVALID)
		return NULL;

	for (i = 0; i < lcd_ext_dev_cnt[edrv->index]; i++) {
		if (edrv->dev[i] && edrv->dev[i]->dev_index == dev_index)
			return edrv->dev[i];
	}

	EXTERR("[%d]: invalid dev_index: %d\n", edrv->index, dev_index);
	return NULL;
}

static void lcd_extern_init_table_dynamic_print(struct lcd_extern_config_s *econf, int flag)
{
	int i, j, max_len;
	unsigned char type, size;
	unsigned char *table;

	if (flag) {
		printf("power on:\n");
		table = econf->table_init_on;
		max_len = econf->table_init_on_cnt;
	} else {
		printf("power off:\n");
		table = econf->table_init_off;
		max_len = econf->table_init_off_cnt;
	}
	if (!table) {
		EXTERR("init_table %d is NULL\n", flag);
		return;
	}
	i = 0;
	switch (econf->type) {
	case LCD_EXTERN_I2C:
	case LCD_EXTERN_SPI:
		while ((i + 1) < max_len) {
			type = table[i];
			size = table[i + 1];
			if (type == LCD_EXT_CMD_TYPE_END) {
				printf("  0x%02x,%d,\n", type, size);
				break;
			}
			printf("  0x%02x,%d,", type, size);
			if (size == 0)
				goto init_table_dynamic_print_i2c_spi_next;
			if (i + 2 + size > max_len) {
				printf("size out of support\n");
				break;
			}

			if (type == LCD_EXT_CMD_TYPE_GPIO ||
			    type == LCD_EXT_CMD_TYPE_DELAY) {
				for (j = 0; j < size; j++)
					printf("%d,", table[i+2+j]);
			} else if ((type == LCD_EXT_CMD_TYPE_CMD) ||
				   (type == LCD_EXT_CMD_TYPE_CMD2) ||
				   (type == LCD_EXT_CMD_TYPE_CMD3) ||
				   (type == LCD_EXT_CMD_TYPE_CMD4) ||
				   (type == LCD_EXT_CMD_TYPE_CMD_BIN) ||
				   (type == LCD_EXT_CMD_TYPE_CMD2_BIN) ||
				   (type == LCD_EXT_CMD_TYPE_CMD3_BIN) ||
				   (type == LCD_EXT_CMD_TYPE_CMD4_BIN) ||
				   (type == LCD_EXT_CMD_TYPE_CMD_BIN_DATA) ||
				   (type == LCD_EXT_CMD_TYPE_CMD2_BIN_DATA) ||
				   (type == LCD_EXT_CMD_TYPE_CMD3_BIN_DATA) ||
				   (type == LCD_EXT_CMD_TYPE_CMD4_BIN_DATA)) {
				for (j = 0; j < size; j++)
					printf("0x%02x,", table[i + 2 + j]);
			} else if ((type == LCD_EXT_CMD_TYPE_CMD_DELAY) ||
				(type == LCD_EXT_CMD_TYPE_CMD2_DELAY)) {
				for (j = 0; j < (size - 1); j++)
					printf("0x%02x,", table[i+2+j]);
				printf("%d,", table[i + size + 1]);
			} else {
				for (j = 0; j < size; j++)
					printf("0x%02x,", table[i+2+j]);
			}
init_table_dynamic_print_i2c_spi_next:
			printf("\n");
			i += (size + 2);
		}
		break;
	case LCD_EXTERN_MIPI:
		while ((i + 1) < max_len) {
			type = table[i];
			size = table[i + 1];
			if (type == LCD_EXT_CMD_TYPE_END) {
				if (size == 0xff) {
					printf("0x%02x,0x%02x,\n", type, size);
					break;
				}
				if (size == 0) {
					printf("0x%02x,%d,\n", type, size);
					break;
				}
				size = 0;
			}

			printf("0x%02x,%d,", type, table[i + 1]);
			if (size == 0)
				goto init_table_dynamic_print_mipi_next;
			if (i + 2 + size > max_len) {
				printf("size out of support\n");
				break;
			}

			if (type == LCD_EXT_CMD_TYPE_GPIO ||
			    type == LCD_EXT_CMD_TYPE_DELAY) {
				for (j = 0; j < size; j++)
					printf("%d,", table[i+2+j]);
			} else if ((type & 0xf) == 0x0) {
				printf("  init_%s wrong data_type: 0x%02x\n",
					flag ? "on" : "off", type);
				break;
			} else {
				for (j = 0; j < size; j++)
					printf("0x%02x,", table[i+2+j]);
			}
init_table_dynamic_print_mipi_next:
			printf("\n");
			i += (size + 2);
		}
		break;
	default:
		break;
	}
}

static void lcd_extern_init_table_fixed_print(struct lcd_extern_config_s *econf, int flag)
{
	int i, j, max_len;
	unsigned char cmd_size;
	unsigned char *table;

	cmd_size = econf->cmd_size;
	if (flag) {
		printf("power on:\n");
		table = econf->table_init_on;
		max_len = econf->table_init_on_cnt;
	} else {
		printf("power off:\n");
		table = econf->table_init_off;
		max_len = econf->table_init_off_cnt;
	}
	if (table == NULL) {
		EXTERR("init_table %d is NULL\n", flag);
		return;
	}

	i = 0;
	while ((i + cmd_size) <= max_len) {
		printf("  ");
		for (j = 0; j < cmd_size; j++)
			printf("0x%02x,", table[i+j]);
		printf("\n");

		if (table[i] == LCD_EXT_CMD_TYPE_END)
			break;
		i += cmd_size;
	}
}

static void lcd_extern_multi_list_print(struct lcd_extern_dev_s *edev)
{
	struct lcd_extern_multi_list_s *temp_list;
	int i;

	if (!edev->multi_list_header) {
		printf("multi_list: NULL\n");
		return;
	}

	temp_list = edev->multi_list_header;
	while (temp_list) {
		printf("multi_list[%d]:\n", temp_list->index);
		printf("  type: 0x%x\n", temp_list->type);
		printf("  data:");
		for (i = 0; i < temp_list->data_len; i++)
			printf(" %d", temp_list->data_buf[i]);
		printf("\n");
		temp_list = temp_list->next;
	}
}

static void lcd_extern_dev_info_print(struct lcd_extern_dev_s *edev)
{
	printf("lcd_extern device[%d] info:\n", edev->config.index);
	printf("name:             %s\n"
		"type:             %d\n"
		"status:           %d\n",
		edev->config.name,
		edev->config.type, edev->config.status);

	switch (edev->config.type) {
	case LCD_EXTERN_I2C:
		printf("i2c_addr:         0x%02x\n"
			"i2c_addr2:        0x%02x\n"
			"i2c_addr3:        0x%02x\n"
			"i2c_addr4:        0x%02x\n"
			"table_loaded:     %d\n",
			edev->config.i2c_addr, edev->config.i2c_addr2,
			edev->config.i2c_addr3, edev->config.i2c_addr4,
			edev->config.table_init_loaded);
		if (edev->config.cmd_size == 0)
			break;
		printf("init_loaded           = %d\n"
			"cmd_size              = %d\n"
			"table_init_on_cnt:    = %d\n"
			"table_init_off_cnt:   = %d\n",
			edev->config.table_init_loaded,
			edev->config.cmd_size,
			edev->config.table_init_on_cnt,
			edev->config.table_init_off_cnt);
		if (edev->config.cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC) {
			lcd_extern_init_table_dynamic_print(&edev->config, 1);
			lcd_extern_init_table_dynamic_print(&edev->config, 0);
		} else {
			lcd_extern_init_table_fixed_print(&edev->config, 1);
			lcd_extern_init_table_fixed_print(&edev->config, 0);
		}
		lcd_extern_multi_list_print(edev);
		break;
	case LCD_EXTERN_SPI:
		printf("spi_gpio_cs:      %d\n"
			"spi_gpio_clk:     %d\n"
			"spi_gpio_data:    %d\n"
			"spi_clk_freq:     %d\n"
			"spi_clk_pol:      %d\n"
			"table_loaded:     %d\n",
			edev->config.spi_gpio_cs,
			edev->config.spi_gpio_clk,
			edev->config.spi_gpio_data,
			edev->config.spi_clk_freq,
			edev->config.spi_clk_pol,
			edev->config.table_init_loaded);
		if (edev->config.cmd_size == 0)
			break;
		printf("init_loaded           = %d\n"
			"cmd_size              = %d\n"
			"table_init_on_cnt:    = %d\n"
			"table_init_off_cnt:   = %d\n",
			edev->config.table_init_loaded,
			edev->config.cmd_size,
			edev->config.table_init_on_cnt,
			edev->config.table_init_off_cnt);
		if (edev->config.cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC) {
			lcd_extern_init_table_dynamic_print(&edev->config, 1);
			lcd_extern_init_table_dynamic_print(&edev->config, 0);
		} else {
			lcd_extern_init_table_fixed_print(&edev->config, 1);
			lcd_extern_init_table_fixed_print(&edev->config, 0);
		}
		break;
	case LCD_EXTERN_MIPI:
		if (edev->config.cmd_size != LCD_EXT_CMD_SIZE_DYNAMIC)
			break;
		printf("init_loaded           = %d\n"
			"cmd_size              = %d\n"
			"table_init_on_cnt:    = %d\n"
			"table_init_off_cnt:   = %d\n",
			edev->config.table_init_loaded,
			edev->config.cmd_size,
			edev->config.table_init_on_cnt,
			edev->config.table_init_off_cnt);
		lcd_extern_init_table_dynamic_print(&edev->config, 1);
		lcd_extern_init_table_dynamic_print(&edev->config, 0);
		break;
	default:
		printf("not support extern_type\n");
		break;
	}
}

static void lcd_extern_info_print(struct lcd_extern_driver_s *edrv)
{
	struct lcd_extern_dev_s *edev;
	int i;

	EXTPR("[%d]: driver info:\n", edrv->index);
	printf("index:            %d\n"
		"key_valid:        %d\n"
		"dev_cnt:          %d\n"
		"i2c_bus:          %d\n",
		edrv->index, edrv->key_valid,
		edrv->dev_cnt, edrv->i2c_bus);
	if (edrv->i2c_sck_gpio < LCD_EXTERN_GPIO_NUM_MAX) {
		printf("i2c_sck_gpio:     %d\n"
			"i2c_sck_gpio_off: %d\n",
			edrv->i2c_sck_gpio, edrv->i2c_sck_gpio_off);
	}
	if (edrv->i2c_sda_gpio < LCD_EXTERN_GPIO_NUM_MAX) {
		printf("i2c_sda_gpio:     %d\n"
			"i2c_sda_gpio_off: %d\n",
			edrv->i2c_sda_gpio, edrv->i2c_sda_gpio_off);
	}
	if (edrv->pinmux_valid) {
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (edrv->pinmux_set[i][0] == LCD_PINMUX_END)
				break;
			printf("pinmux_set: %d, 0x%08x\n",
				edrv->pinmux_set[i][0],
				edrv->pinmux_set[i][1]);
			i++;
		}
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (edrv->pinmux_clr[i][0] == LCD_PINMUX_END)
				break;
			printf("pinmux_clr: %d, 0x%08x\n",
				edrv->pinmux_clr[i][0],
				edrv->pinmux_clr[i][1]);
			i++;
		}
	}
	i = 0;
	while (i < LCD_EXTERN_GPIO_NUM_MAX) {
		if (strcmp(edrv->gpio_name[i], "invalid") == 0)
			break;
		printf("gpio[%d]=%s\n", i, edrv->gpio_name[i]);
		i++;
	}

	for (i = 0; i < edrv->dev_cnt; i++) {
		edev = edrv->dev[i];
		if (!edev)
			continue;
		lcd_extern_dev_info_print(edev);
	}
}

static void lcd_extern_power_ctrl(struct lcd_extern_driver_s *edrv, int status)
{
	struct lcd_extern_dev_s *edev;
	int i;

	EXTPR("%s: %d\n", __func__, status);

	if (status) {
		for (i = 0; i < edrv->dev_cnt; i++) {
			edev = edrv->dev[i];
			if (edev && edev->power_on) {
				EXTPR("[%d]: %s: %d: dev[%d]:\n",
				      edrv->index, __func__, status, i);
				edev->power_on(edrv, edev);
			}
		}
	} else {
		for (i = 0; i < edrv->dev_cnt; i++) {
			edev = edrv->dev[i];
			if (edev && edev->power_off) {
				EXTPR("[%d]: %s: %d: dev[%d]:\n",
				      edrv->index, __func__, status, i);
				edev->power_off(edrv, edev);
			}
		}
	}
}

int lcd_extern_gpio_get(struct lcd_extern_driver_s *edrv, unsigned char index)
{
	int gpio;
	char *str;

	if (!edrv) {
		EXTERR("%s: ext_drv is null\n", __func__);
		return LCD_GPIO_MAX;
	}
	if (index >= LCD_EXTERN_GPIO_NUM_MAX)
		return LCD_GPIO_MAX;

	str = edrv->gpio_name[index];
	gpio = lcd_gpio_name_map_num(str);
	return gpio;
}

int lcd_ext_gpio_set(struct lcd_extern_driver_s *edrv, unsigned char index, int value)
{
	int gpio;
	int ret;

	gpio = lcd_extern_gpio_get(edrv, index);
	ret = lcd_gpio_set(gpio, value);
	return ret;
}

void lcd_extern_pinmux_set(struct lcd_extern_driver_s *edrv, int status)
{
	int i;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		EXTPR("[%d]: %s: %d\n", edrv->index, __func__, status);

	if (status) {
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (edrv->pinmux_clr[i][0] == LCD_PINMUX_END)
				break;
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				EXTPR("pinmux_clr: %d, 0x%08x\n",
					edrv->pinmux_clr[i][0],
					edrv->pinmux_clr[i][1]);
			}
			lcd_pinmux_clr_mask(edrv->pinmux_clr[i][0],
				edrv->pinmux_clr[i][1]);
			i++;
		}
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (edrv->pinmux_set[i][0] == LCD_PINMUX_END)
				break;
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				EXTPR("pinmux_set: %d, 0x%08x\n",
					edrv->pinmux_set[i][0],
					edrv->pinmux_set[i][1]);
			}
			lcd_pinmux_set_mask(edrv->pinmux_set[i][0],
				edrv->pinmux_set[i][1]);
			i++;
		}
	} else {
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (edrv->pinmux_set[i][0] == LCD_PINMUX_END)
				break;
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				EXTPR("pinmux_clr: %d, 0x%08x\n",
					edrv->pinmux_set[i][0],
					edrv->pinmux_set[i][1]);
			}
			lcd_pinmux_clr_mask(edrv->pinmux_set[i][0],
				edrv->pinmux_set[i][1]);
			i++;
		}
	}
}

#ifdef CONFIG_OF_LIBFDT
int lcd_extern_get_dts_child(char *dtaddr, char *snode, int index)
{
	int nodeoffset;
	char child_node[30];
	char *propdata;

	sprintf(child_node, "%s/extern_%d", snode, index);
	nodeoffset = fdt_path_offset(dtaddr, child_node);
	if (nodeoffset < 0) {
		EXTERR("dts: not find  node %s\n", child_node);
		return nodeoffset;
	}

	propdata = (char *)fdt_getprop(dtaddr, nodeoffset, "index", NULL);
	if (propdata == NULL) {
		EXTERR("get index failed, exit\n");
		return -1;
	} else {
		if (be32_to_cpup((u32*)propdata) != index) {
			EXTERR("index not match, exit\n");
			return -1;
		}
	}
	return nodeoffset;
}

static int lcd_extern_get_init_dts(char *dtaddr, struct lcd_extern_driver_s *edrv)
{
	int parent_offset;
	char *propdata, *p;
	const char *str;
	char snode[15];
	int i;

	if (edrv->index == 0)
		sprintf(snode, "/lcd_extern");
	else
		sprintf(snode, "/lcd%d_extern", edrv->index);

	parent_offset = fdt_path_offset(dtaddr, snode);
	if (parent_offset < 0) {
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
			EXTERR("not find %s node: %s\n",
			       snode, fdt_strerror(parent_offset));
		}
		return -1;
	}

	propdata = (char *)fdt_getprop(dtaddr, parent_offset, "key_valid", NULL);
	if (!propdata) {
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
			EXTPR("[%d]: failed to get key_valid\n", edrv->index);
		edrv->key_valid = 0;
	} else {
		edrv->key_valid = (unsigned char)(be32_to_cpup((u32 *)propdata));
	}

	propdata = (char *)fdt_getprop(dtaddr, parent_offset, "i2c_bus", NULL);
	if (!propdata)
		edrv->i2c_bus = LCD_EXTERN_I2C_BUS_MAX;
	else
		edrv->i2c_bus = aml_lcd_i2c_bus_get_str(propdata);

	i = 0;
	propdata = (char *)fdt_getprop(dtaddr, parent_offset, "extern_gpio_names", NULL);
	if (propdata) {
		EXTPR("[%d]: find extern_gpio_names\n", edrv->index);
		p = propdata;
		while (i < LCD_EXTERN_GPIO_NUM_MAX) {
			if (i > 0)
				p += strlen(p) + 1;
			str = p;
			if (strlen(str) == 0)
				break;
			strncpy(edrv->gpio_name[i], str, (LCD_CPU_GPIO_NAME_MAX - 1));
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				EXTPR("[%d]: gpio[%d]=%s\n",
				      edrv->index, i, edrv->gpio_name[i]);
			}
			i++;
		}
	}
	if (i < LCD_EXTERN_GPIO_NUM_MAX)
		strcpy(edrv->gpio_name[i], "invalid");

	propdata = (char *)fdt_getprop(dtaddr, parent_offset, "i2c_gpio_off", NULL);
	if (!propdata) {
		edrv->i2c_sck_gpio = LCD_EXT_GPIO_INVALID;
		edrv->i2c_sck_gpio_off = 2;
		edrv->i2c_sda_gpio = LCD_EXT_GPIO_INVALID;
		edrv->i2c_sda_gpio_off = 2;
	} else {
		edrv->i2c_sck_gpio = be32_to_cpup((u32 *)propdata);
		edrv->i2c_sck_gpio_off = be32_to_cpup((((u32 *)propdata) + 1));
		edrv->i2c_sda_gpio = be32_to_cpup((((u32 *)propdata) + 2));
		edrv->i2c_sda_gpio_off = be32_to_cpup((((u32 *)propdata) + 3));
	}

	return 0;
}
#endif

static int lcd_extern_get_init_bsp(struct lcd_extern_driver_s *edrv)
{
	struct lcd_extern_common_s *ext_common;
	char (*ext_gpio)[LCD_CPU_GPIO_NAME_MAX];
	int i, j;

	ext_common = edrv->data->dft_conf[edrv->index]->ext_common;
	edrv->key_valid = ext_common->key_valid;
	edrv->i2c_bus = ext_common->i2c_bus;

	i = 0;
	ext_gpio = edrv->data->dft_conf[edrv->index]->ext_common->ext_gpio;
	if (!ext_gpio) {
		EXTERR("[%d]: %s: ext_gpio is null\n", edrv->index, __func__);
		return -1;
	}
	while (i < LCD_EXTERN_GPIO_NUM_MAX) {
		if (strcmp(ext_gpio[i], "invalid") == 0)
			break;
		strncpy(edrv->gpio_name[i], ext_gpio[i],
			(LCD_CPU_GPIO_NAME_MAX - 1));
		i++;
	}
	for (j = i; j < LCD_EXTERN_GPIO_NUM_MAX; j++)
		strcpy(edrv->gpio_name[j], "invalid");

	edrv->i2c_sck_gpio = ext_common->i2c_sck_gpio;
	edrv->i2c_sck_gpio_off = ext_common->i2c_sck_gpio_off;
	edrv->i2c_sda_gpio = ext_common->i2c_sda_gpio;
	edrv->i2c_sda_gpio_off = ext_common->i2c_sda_gpio_off;

	return 0;
}

static int lcd_extern_pinmux_load_from_bsp(struct lcd_extern_driver_s *edrv)
{
	struct lcd_pinmux_ctrl_s *pinmux;
	char propname[20];
	unsigned int i, j;
	int set_cnt = 0, clr_cnt = 0;

	if (!edrv->data)
		return -1;
	if (!edrv->data->dft_conf[edrv->index]) {
		EXTERR("[%d]: %s: dft_conf is NULL\n", edrv->index, __func__);
		return -1;
	}
	pinmux = edrv->data->dft_conf[edrv->index]->ext_common->ext_pinmux;
	if (!pinmux) {
		EXTERR("[%d]: %s: ext_pinmux is NULL\n", edrv->index, __func__);
		return -1;
	}

	sprintf(propname, "extern_on");
	for (i = 0; i < LCD_PINMX_MAX; i++) {
		if (!pinmux->name)
			break;
		if (strcmp(pinmux->name, "invalid") == 0)
			break;
		if (strcmp(pinmux->name, propname)) {
			for (j = 0; j < LCD_PINMUX_NUM; j++ ) {
				if (pinmux->pinmux_set[j][0] == LCD_PINMUX_END)
					break;
				edrv->pinmux_set[j][0] = pinmux->pinmux_set[j][0];
				edrv->pinmux_set[j][1] = pinmux->pinmux_set[j][1];
				set_cnt++;
			}
			for (j = 0; j < LCD_PINMUX_NUM; j++ ) {
				if (pinmux->pinmux_clr[j][0] == LCD_PINMUX_END)
					break;
				edrv->pinmux_clr[j][0] = pinmux->pinmux_clr[j][0];
				edrv->pinmux_clr[j][1] = pinmux->pinmux_clr[j][1];
				clr_cnt++;
			}
			break;
		}
		pinmux++;
	}
	if (set_cnt < LCD_PINMUX_NUM) {
		edrv->pinmux_set[set_cnt][0] = LCD_PINMUX_END;
		edrv->pinmux_set[set_cnt][1] = 0x0;
	}
	if (clr_cnt < LCD_PINMUX_NUM) {
		edrv->pinmux_clr[clr_cnt][0] = LCD_PINMUX_END;
		edrv->pinmux_clr[clr_cnt][1] = 0x0;
	}

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (edrv->pinmux_set[i][0] == LCD_PINMUX_END)
				break;
			EXTPR("pinmux set: %d, 0x%08x\n",
			      edrv->pinmux_set[i][0],
			      edrv->pinmux_set[i][1]);
			i++;
		}
		i = 0;
		while (i < LCD_PINMUX_NUM) {
			if (edrv->pinmux_clr[i][0] == LCD_PINMUX_END)
				break;
			EXTPR("pinmux clr: %d, 0x%08x\n",
			      edrv->pinmux_clr[i][0],
			      edrv->pinmux_clr[i][1]);
			i++;
		}
	}

	return 0;
}

static int lcd_extern_init_table_save(struct lcd_extern_config_s *extconf, int flag,
				      unsigned char *table)
{
	if (!extconf || !table) {
		EXTERR("%s: resource error\n", __func__);
		return -1;
	}
	if (flag) {
		if (extconf->table_init_on) {
			free(extconf->table_init_on);
			extconf->table_init_on = NULL;
		}
		extconf->table_init_on = (unsigned char *)
			malloc(sizeof(unsigned char) * extconf->table_init_on_cnt);
		if (!extconf->table_init_on) {
			EXTERR("%s: Not enough memory\n", __func__);
			return -1;
		}
		memcpy(extconf->table_init_on, table, extconf->table_init_on_cnt);
	} else {
		if (extconf->table_init_off) {
			free(extconf->table_init_off);
			extconf->table_init_off = NULL;
		}
		extconf->table_init_off = (unsigned char *)
			malloc(sizeof(unsigned char) * extconf->table_init_off_cnt);
		if (!extconf->table_init_off) {
			EXTERR("%s: Not enough memory\n", __func__);
			return -1;
		}
		memcpy(extconf->table_init_off, table, extconf->table_init_off_cnt);
	}

	return 0;
}

#ifdef CONFIG_OF_LIBFDT
static int lcd_extern_init_table_dynamic_load_dts(char *dtaddr, int nodeoffset,
						  struct lcd_extern_config_s *extconf,
						  int flag)
{
	unsigned char cmd_size, type;
	int i = 0, j, max_len;
	unsigned char *table;
	char propname[20];
	char *propdata;
	int ret;

	if (flag) {
		max_len = LCD_EXTERN_INIT_ON_MAX;
		sprintf(propname, "init_on");
	} else {
		max_len = LCD_EXTERN_INIT_OFF_MAX;
		sprintf(propname, "init_off");
	}
	table = (unsigned char *)malloc(sizeof(unsigned char) * max_len);
	if (!table) {
		EXTERR("%s: Not enough memory\n", __func__);
		return -1;
	}
	table[0] = LCD_EXT_CMD_TYPE_END;
	table[1] = 0;

	propdata = (char *)fdt_getprop(dtaddr, nodeoffset, propname, NULL);
	if (propdata == NULL) {
		EXTERR("%s: get %s failed\n", extconf->name, propname);
		table[0] = LCD_EXT_CMD_TYPE_END;
		table[1] = 0;
		if (flag)
			extconf->table_init_on_cnt = 2;
		else
			extconf->table_init_off_cnt = 2;
		goto lcd_extern_init_table_dynamic_load_dts_abort;
	}

	switch (extconf->type) {
	case LCD_EXTERN_I2C:
	case LCD_EXTERN_SPI:
		while ((i + 1) < max_len) {
			table[i] = (unsigned char)(be32_to_cpup((((u32*)propdata)+i)));
			table[i+1] = (unsigned char)(be32_to_cpup((((u32*)propdata)+i+1)));
			type = table[i];
			cmd_size = table[i+1];
			if (type == LCD_EXT_CMD_TYPE_END)
				break;
			if (cmd_size == 0)
				goto init_table_dynamic_i2c_spi_dts_next;
			if ((i + 2 + cmd_size) > max_len) {
				EXTERR("%s: %s cmd_size out of support\n", extconf->name, propname);
				table[i] = LCD_EXT_CMD_TYPE_END;
				table[i+1] = 0;
				if (flag)
					extconf->table_init_on_cnt = i + 2;
				else
					extconf->table_init_off_cnt = i + 2;
				goto lcd_extern_init_table_dynamic_load_dts_abort;
			}
			for (j = 0; j < cmd_size; j++)
				table[i+2+j] = (unsigned char)(be32_to_cpup((((u32*)propdata)+i+2+j)));

init_table_dynamic_i2c_spi_dts_next:
			i += (cmd_size + 2);
		}
		if (flag)
			extconf->table_init_on_cnt = i + 2;
		else
			extconf->table_init_off_cnt = i + 2;
		break;
	case LCD_EXTERN_MIPI:
		while ((i + 1) < max_len) {
			table[i] = (unsigned char)(be32_to_cpup((((u32*)propdata)+i)));
			table[i+1] = (unsigned char)(be32_to_cpup((((u32*)propdata)+i+1)));
			type = table[i];
			cmd_size = table[i+1];
			if (type == LCD_EXT_CMD_TYPE_END) {
				if ((cmd_size == 0xff) || (cmd_size == 0))
					break;
				cmd_size = 0;
			}
			if (cmd_size == 0)
				goto init_table_dynamic_mipi_dts_next;
			if ((i + 2 + cmd_size) > max_len) {
				EXTERR("%s: %s cmd_size out of support\n", extconf->name, propname);
				table[i] = LCD_EXT_CMD_TYPE_END;
				table[i+1] = 0;
				if (flag)
					extconf->table_init_on_cnt = i + 2;
				else
					extconf->table_init_off_cnt = i + 2;
				goto lcd_extern_init_table_dynamic_load_dts_abort;
			}
			for (j = 0; j < cmd_size; j++)
				table[i+2+j] = (unsigned char)(be32_to_cpup((((u32*)propdata)+i+2+j)));

init_table_dynamic_mipi_dts_next:
			i += (cmd_size + 2);
		}
		if (flag)
			extconf->table_init_on_cnt = i + 2;
		else
			extconf->table_init_off_cnt = i + 2;
		break;
	default:
		if (flag)
			extconf->table_init_on_cnt = 0;
		else
			extconf->table_init_off_cnt = 0;
		goto lcd_extern_init_table_dynamic_load_dts_err;
	}

	ret = lcd_extern_init_table_save(extconf, flag, table);
	if (ret)
		goto lcd_extern_init_table_dynamic_load_dts_err;
	free(table);
	return 0;

lcd_extern_init_table_dynamic_load_dts_abort:
	lcd_extern_init_table_save(extconf, flag, table);
	free(table);
	return -1;

lcd_extern_init_table_dynamic_load_dts_err:
	free(table);
	return -1;
}

static int lcd_extern_init_table_fixed_load_dts(char *dtaddr, int nodeoffset,
						struct lcd_extern_config_s *extconf,
						int flag)
{
	unsigned char cmd_size;
	int i = 0, j, max_len;
	unsigned char *table;
	char propname[20];
	char *propdata;
	int ret;

	cmd_size = extconf->cmd_size;
	if (flag) {
		max_len = LCD_EXTERN_INIT_ON_MAX;
		sprintf(propname, "init_on");
	} else {
		max_len = LCD_EXTERN_INIT_OFF_MAX;
		sprintf(propname, "init_off");
	}
	table = (unsigned char *)malloc(sizeof(unsigned char) * max_len);
	if (!table) {
		EXTERR("%s: Not enough memory\n", __func__);
		return -1;
	}
	table[0] = LCD_EXT_CMD_TYPE_END;
	table[1] = 0;

	propdata = (char *)fdt_getprop(dtaddr, nodeoffset, propname, NULL);
	if (!propdata) {
		EXTERR("%s: get %s failed\n", extconf->name, propname);
		table[0] = LCD_EXT_CMD_TYPE_END;
		if (flag)
			extconf->table_init_on_cnt = 2;
		else
			extconf->table_init_off_cnt = 2;
		goto lcd_extern_init_table_fixed_load_dts_abort;
	}

	while (i < max_len) {
		if ((i + cmd_size) > max_len) {
			EXTERR("%s: %s cmd_size out of support\n",
			       extconf->name, propname);
			table[i] = LCD_EXT_CMD_TYPE_END;
			if (flag)
				extconf->table_init_on_cnt = i + cmd_size;
			else
				extconf->table_init_off_cnt = i + cmd_size;
			goto lcd_extern_init_table_fixed_load_dts_abort;
		}
		for (j = 0; j < cmd_size; j++)
			table[i + j] = (unsigned char)(be32_to_cpup((((u32 *)propdata) + i + j)));

		if (table[i] == LCD_EXT_CMD_TYPE_END)
			break;

		i += cmd_size;
	}
	if (flag)
		extconf->table_init_on_cnt = i + cmd_size;
	else
		extconf->table_init_off_cnt = i + cmd_size;
	ret = lcd_extern_init_table_save(extconf, flag, table);
	if (ret)
		goto lcd_extern_init_table_fixed_load_dts_err;
	free(table);
	return 0;

lcd_extern_init_table_fixed_load_dts_abort:
	lcd_extern_init_table_save(extconf, flag, table);
	free(table);
	return -1;

lcd_extern_init_table_fixed_load_dts_err:
	free(table);
	return -1;
}

static int lcd_extern_get_config_dts(char *dtaddr, char *snode,
				     struct lcd_extern_driver_s *edrv,
				     struct lcd_extern_dev_s *edev)
{
	struct lcd_extern_config_s *extconf = &edev->config;
	int nodeoffset;
	char *propdata;
	const char *str;
	int ret = 0;

	extconf->table_init_loaded = 0;
	nodeoffset = lcd_extern_get_dts_child(dtaddr, snode, edev->dev_index);
	if (nodeoffset < 0)
		return -1;

	propdata = (char *)fdt_getprop(dtaddr, nodeoffset, "index", NULL);
	if (propdata == NULL) {
		extconf->index = LCD_EXTERN_INDEX_INVALID;
		EXTERR("get index failed, exit\n");
		return -1;
	} else {
		extconf->index = (unsigned char)(be32_to_cpup((u32*)propdata));
	}
	if (lcd_debug_print_flag)
		EXTPR("index = %d\n", extconf->index);

	propdata = (char *)fdt_getprop(dtaddr, nodeoffset, "extern_name", NULL);
	if (propdata == NULL) {
		str = "invalid_name";
		strcpy(extconf->name, str);
		EXTERR("get extern_name failed\n");
	} else {
		memset(extconf->name, 0, LCD_EXTERN_NAME_LEN_MAX);
		strncpy(extconf->name, propdata, (LCD_EXTERN_NAME_LEN_MAX - 1));
	}

	propdata = (char *)fdt_getprop(dtaddr, nodeoffset, "status", NULL);
	if (propdata == NULL) {
		EXTERR("get status failed, default to disabled\n");
		extconf->status = 0;
	} else {
		if (strncmp(propdata, "okay", 2) == 0)
			extconf->status = 1;
		else
			extconf->status = 0;
	}
	if (lcd_debug_print_flag)
		EXTPR("%s: status = %d\n", extconf->name, extconf->status);

	propdata = (char *)fdt_getprop(dtaddr, nodeoffset, "type", NULL);
	if (!propdata) {
		extconf->type = LCD_EXTERN_MAX;
		EXTERR("get type failed, exit\n");
		return -1;
	}
	extconf->type = be32_to_cpup((u32 *)propdata);

	if (lcd_debug_print_flag)
		EXTPR("%s: type = %d\n", extconf->name, extconf->type);

	switch (extconf->type) {
	case LCD_EXTERN_I2C:
		propdata = (char *)fdt_getprop(dtaddr, nodeoffset,
					       "i2c_address", NULL);
		if (!propdata) {
			EXTERR("%s: get i2c_address failed, exit\n", extconf->name);
			extconf->i2c_addr = 0xff;
			return -1;
		}
		extconf->i2c_addr = (unsigned char)(be32_to_cpup((u32 *)propdata));

		if (lcd_debug_print_flag)
			EXTPR("%s: i2c_address=0x%02x\n", extconf->name, extconf->i2c_addr);

		propdata = (char *)fdt_getprop(dtaddr, nodeoffset,
					       "i2c_address2", NULL);
		if (!propdata) {
			propdata = (char *)fdt_getprop(dtaddr, nodeoffset,
						       "i2c_second_address",
						       NULL);
			if (!propdata) {
				if (lcd_debug_print_flag)
					EXTPR("%s no i2c_address2 exist\n",
					      extconf->name);
				extconf->i2c_addr2 = 0xff;
			} else {
				extconf->i2c_addr2 =
					(unsigned char)(be32_to_cpup((u32 *)propdata));
			}
		} else {
			extconf->i2c_addr2 =
				(unsigned char)(be32_to_cpup((u32 *)propdata));
		}
		if (lcd_debug_print_flag)
			EXTPR("%s: i2c_address2=0x%02x\n",
			      extconf->name, extconf->i2c_addr2);
		propdata = (char *)fdt_getprop(dtaddr, nodeoffset,
					       "i2c_address3", NULL);
		if (!propdata) {
			if (lcd_debug_print_flag)
				EXTPR("%s no i2c_address3 exist\n", extconf->name);
			extconf->i2c_addr3 = 0xff;
		} else {
			extconf->i2c_addr3 =
				(unsigned char)(be32_to_cpup((u32 *)propdata));
		}
		if (lcd_debug_print_flag)
			EXTPR("%s: i2c_address3=0x%02x\n", extconf->name,
			      extconf->i2c_addr3);
		propdata = (char *)fdt_getprop(dtaddr, nodeoffset,
					       "i2c_address4", NULL);
		if (!propdata) {
			if (lcd_debug_print_flag)
				EXTPR("%s no i2c_address4 exist\n", extconf->name);
			extconf->i2c_addr4 = 0xff;
		} else {
			extconf->i2c_addr4 =
				(unsigned char)(be32_to_cpup((u32 *)propdata));
		}
		if (lcd_debug_print_flag)
			EXTPR("%s: i2c_address4=0x%02x\n", extconf->name,
			      extconf->i2c_addr4);

		propdata = (char *)fdt_getprop(dtaddr, nodeoffset,
					       "cmd_size", NULL);
		if (!propdata) {
			EXTPR("%s: no cmd_size\n", extconf->name);
			extconf->cmd_size = 0;
		} else {
			extconf->cmd_size =
				(unsigned char)(be32_to_cpup((u32 *)propdata));
		}
		if (lcd_debug_print_flag)
			EXTPR("%s: cmd_size=%d\n", extconf->name,
			      extconf->cmd_size);
		if (extconf->cmd_size == 0)
			break;

		if (extconf->cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC) {
			ret = lcd_extern_init_table_dynamic_load_dts(dtaddr, nodeoffset,
					extconf, 1);
			if (ret)
				break;
			ret = lcd_extern_init_table_dynamic_load_dts(dtaddr, nodeoffset,
					extconf, 0);
		} else {
			ret = lcd_extern_init_table_fixed_load_dts(dtaddr, nodeoffset,
					extconf, 1);
			if (ret)
				break;
			ret = lcd_extern_init_table_fixed_load_dts(dtaddr, nodeoffset,
					extconf, 0);
		}
		if (ret == 0)
			extconf->table_init_loaded = 1;
		break;
	case LCD_EXTERN_SPI:
		propdata = (char *)fdt_getprop(dtaddr, nodeoffset,
					       "gpio_spi_cs", NULL);
		if (!propdata) {
			EXTERR("%s: get gpio_spi_cs failed, exit\n",
			       extconf->name);
			extconf->spi_gpio_cs = LCD_EXT_GPIO_INVALID;
			return -1;
		}
		extconf->spi_gpio_cs =
			(unsigned char)(be32_to_cpup((u32 *)propdata));

		propdata = (char *)fdt_getprop(dtaddr, nodeoffset,
					       "gpio_spi_clk", NULL);
		if (!propdata) {
			EXTERR("%s: get gpio_spi_clk failed, exit\n",
			       extconf->name);
			extconf->spi_gpio_clk = LCD_EXT_GPIO_INVALID;
			return -1;
		}
		extconf->spi_gpio_clk =
			(unsigned char)(be32_to_cpup((u32 *)propdata));

		propdata = (char *)fdt_getprop(dtaddr, nodeoffset,
					       "gpio_spi_data", NULL);
		if (!propdata) {
			EXTERR("%s: get gpio_spi_data failed, exit\n",
			       extconf->name);
			extconf->spi_gpio_data = LCD_EXT_GPIO_INVALID;
			return -1;
		}
		extconf->spi_gpio_data =
			(unsigned char)(be32_to_cpup((u32 *)propdata));

		if (lcd_debug_print_flag) {
			EXTPR("%s: gpio_spi cs=%d, clk=%d, data=%d\n",
			      extconf->name, extconf->spi_gpio_cs,
			      extconf->spi_gpio_clk, extconf->spi_gpio_data);
		}
		propdata = (char *)fdt_getprop(dtaddr, nodeoffset,
					       "spi_clk_freq", NULL);
		if (!propdata) {
			EXTERR
			("%s: get spi_clk_freq failed, default to %dKHz\n",
			 extconf->name, LCD_EXT_SPI_CLK_FREQ_DFT);
			extconf->spi_clk_freq = LCD_EXT_SPI_CLK_FREQ_DFT;
		} else {
			extconf->spi_clk_freq =
				be32_to_cpup((u32 *)propdata);
		}

		propdata = (char *)fdt_getprop(dtaddr, nodeoffset,
					       "spi_clk_pol", NULL);
		if (!propdata) {
			EXTERR("%s: get spi_clk_pol failed, default to 1\n",
			       extconf->name);
			extconf->spi_clk_pol = 1;
		} else {
			extconf->spi_clk_pol =
				(unsigned char)(be32_to_cpup((u32 *)propdata));
		}
		if (lcd_debug_print_flag) {
			EXTPR("%s: spi clk=%dKHz, clk_pol=%d\n",
			      extconf->name, extconf->spi_clk_freq,
			      extconf->spi_clk_pol);
		}
		propdata = (char *)fdt_getprop(dtaddr, nodeoffset,
					       "cmd_size", NULL);
		if (!propdata) {
			EXTPR("%s: no cmd_size\n", extconf->name);
			extconf->cmd_size = 0;
		} else {
			extconf->cmd_size =
				(unsigned char)(be32_to_cpup((u32 *)propdata));
		}
		if (lcd_debug_print_flag)
			EXTPR("%s: cmd_size=%d\n", extconf->name,
			      extconf->cmd_size);
		if (extconf->cmd_size == 0)
			break;

		if (extconf->cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC) {
			ret = lcd_extern_init_table_dynamic_load_dts(dtaddr, nodeoffset,
					extconf, 1);
			if (ret)
				break;
			ret = lcd_extern_init_table_dynamic_load_dts(dtaddr, nodeoffset,
					extconf, 0);
		} else {
			ret = lcd_extern_init_table_fixed_load_dts(dtaddr, nodeoffset,
					extconf, 1);
			if (ret)
				break;
			ret = lcd_extern_init_table_fixed_load_dts(dtaddr, nodeoffset,
					extconf, 0);
		}
		if (ret == 0)
			extconf->table_init_loaded = 1;
		break;
	case LCD_EXTERN_MIPI:
		propdata = (char *)fdt_getprop(dtaddr, nodeoffset,
					       "cmd_size", NULL);
		if (!propdata) {
			EXTPR("%s: no cmd_size\n", extconf->name);
			extconf->cmd_size = 0;
		} else {
			extconf->cmd_size =
				(unsigned char)(be32_to_cpup((u32 *)propdata));
		}
		if (lcd_debug_print_flag)
			EXTPR("%s: cmd_size=%d\n", extconf->name,
			      extconf->cmd_size);
		if (extconf->cmd_size == 0)
			break;

		if (extconf->cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC) {
			ret = lcd_extern_init_table_dynamic_load_dts(dtaddr, nodeoffset,
					extconf, 1);
			if (ret)
				break;
			ret = lcd_extern_init_table_dynamic_load_dts(dtaddr, nodeoffset,
					extconf, 0);
			if (ret == 0)
				extconf->table_init_loaded = 1;
		}
		break;
	default:
		break;
	}

	return 0;
}
#endif

static int lcd_extern_init_table_dynamic_load_ukey(struct lcd_extern_config_s *extconf,
		unsigned char *p, int key_len, int len, int flag)
{
	unsigned char cmd_size = 0;
	int i = 0, j, max_len, ret = 0;
	unsigned char *table, *buf;
	char propname[20];

	if (flag) {
		max_len = LCD_EXTERN_INIT_ON_MAX;
		sprintf(propname, "init_on");
		buf = p;
	} else {
		max_len = LCD_EXTERN_INIT_OFF_MAX;
		sprintf(propname, "init_off");
		buf = p + extconf->table_init_on_cnt;
	}
	table = (unsigned char *)malloc(sizeof(unsigned char) * max_len);
	if (!table) {
		EXTERR("%s: Not enough memory\n", __func__);
		return -1;
	}
	table[0] = LCD_EXT_CMD_TYPE_END;
	table[1] = 0;

	switch (extconf->type) {
	case LCD_EXTERN_I2C:
	case LCD_EXTERN_SPI:
		while ((i + 1) < max_len) {
			/* type */
			len += 1;
			ret = lcd_unifykey_len_check(key_len, len);
			if (ret) {
				EXTERR("%s: get %s type failed\n",
				       extconf->name, propname);
				table[i] = LCD_EXT_CMD_TYPE_END;
				table[i+1] = 0;
				if (flag)
					extconf->table_init_on_cnt = i + 2;
				else
					extconf->table_init_off_cnt = i + 2;
				goto lcd_extern_init_table_dynamic_load_ukey_abort;
			}
			table[i] = *(buf + LCD_UKEY_EXT_INIT + i);
			/* cmd_size */
			len += 1;
			ret = lcd_unifykey_len_check(key_len, len);
			if (ret) {
				EXTERR("%s: get %s cmd_size failed\n",
				       extconf->name, propname);
				table[i] = LCD_EXT_CMD_TYPE_END;
				table[i+1] = 0;
				if (flag)
					extconf->table_init_on_cnt = i + 2;
				else
					extconf->table_init_off_cnt = i + 2;
				goto lcd_extern_init_table_dynamic_load_ukey_abort;
			}
			table[i+1] = *(buf + LCD_UKEY_EXT_INIT + i + 1);
			cmd_size = table[i+1];

			if (table[i] == LCD_EXT_CMD_TYPE_END)
				break;
			if (cmd_size == 0)
				goto init_table_dynamic_i2c_spi_ukey_next;
			if ((i + 2 + cmd_size) > max_len) {
				EXTERR("%s: %s cmd_size out of support\n",
				       extconf->name, propname);
				table[i] = LCD_EXT_CMD_TYPE_END;
				table[i+1] = 0;
				if (flag)
					extconf->table_init_on_cnt = i + 2;
				else
					extconf->table_init_off_cnt = i + 2;
				goto lcd_extern_init_table_dynamic_load_ukey_abort;
			}

			/* data */
			len += cmd_size;
			ret = lcd_unifykey_len_check(key_len, len);
			if (ret) {
				EXTERR("%s: get %s data failed\n",
				       extconf->name, propname);
				table[i] = LCD_EXT_CMD_TYPE_END;
				table[i+1] = 0;
				if (flag)
					extconf->table_init_on_cnt = i + 2;
				else
					extconf->table_init_off_cnt = i + 2;
				goto lcd_extern_init_table_dynamic_load_ukey_abort;
			}
			for (j = 0; j < cmd_size; j++)
				table[i + 2 + j] =
				*(buf + LCD_UKEY_EXT_INIT + i + 2 + j);

init_table_dynamic_i2c_spi_ukey_next:
			i += (cmd_size + 2);
		}
		if (flag)
			extconf->table_init_on_cnt = i + 2;
		else
			extconf->table_init_off_cnt = i + 2;
		break;
	case LCD_EXTERN_MIPI:
		while ((i + 1) < max_len) { /* type & cmd_size detect */
			/* type */
			len += 1;
			ret = lcd_unifykey_len_check(key_len, len);
			if (ret) {
				EXTERR("%s: get type %s failed\n",
				       extconf->name, propname);
				table[i] = LCD_EXT_CMD_TYPE_END;
				table[i+1] = 0;
				if (flag)
					extconf->table_init_on_cnt = i + 2;
				else
					extconf->table_init_off_cnt = i + 2;
				goto lcd_extern_init_table_dynamic_load_ukey_abort;
			}
			table[i] = *(buf + LCD_UKEY_EXT_INIT + i);
			/* type */
			len += 1;
			ret = lcd_unifykey_len_check(key_len, len);
			if (ret) {
				EXTERR("%s: get type %s failed\n",
				       extconf->name, propname);
				table[i] = LCD_EXT_CMD_TYPE_END;
				table[i+1] = 0;
				if (flag)
					extconf->table_init_on_cnt = i + 2;
				else
					extconf->table_init_off_cnt = i + 2;
				goto lcd_extern_init_table_dynamic_load_ukey_abort;
			}
			table[i+1] = *(buf + LCD_UKEY_EXT_INIT + i + 1);
			cmd_size = table[i+1];

			if (table[i] == LCD_EXT_CMD_TYPE_END) {
				if ((cmd_size == 0xff) || (cmd_size == 0))
					break;
				cmd_size = 0;
			}
			if (cmd_size == 0)
				goto init_table_dynamic_mipi_ukey_next;
			if ((i + 2 + cmd_size) > max_len) {
				EXTERR("%s: %s cmd_size out of support\n", extconf->name, propname);
				table[i] = LCD_EXT_CMD_TYPE_END;
				table[i+1] = 0;
				break;
			}

			len += cmd_size;
			ret = lcd_unifykey_len_check(key_len, len);
			if (ret) {
				EXTERR("%s: get %s failed\n", extconf->name, propname);
				table[i] = LCD_EXT_CMD_TYPE_END;
				table[i+1] = 0;
				if (flag)
					extconf->table_init_on_cnt = i + 2;
				else
					extconf->table_init_off_cnt = i + 2;
				goto lcd_extern_init_table_dynamic_load_ukey_abort;
			}
			for (j = 0; j < cmd_size; j++)
				table[i+2+j] = *(buf + LCD_UKEY_EXT_INIT + i + 2 + j);

init_table_dynamic_mipi_ukey_next:
			i += (cmd_size + 2);
		}
		if (flag)
			extconf->table_init_on_cnt = i + 2;
		else
			extconf->table_init_off_cnt = i + 2;
		break;
	default:
		if (flag)
			extconf->table_init_on_cnt = 0;
		else
			extconf->table_init_off_cnt = 0;
		goto lcd_extern_init_table_dynamic_load_ukey_err;
	}

	ret = lcd_extern_init_table_save(extconf, flag, table);
	if (ret)
		goto lcd_extern_init_table_dynamic_load_ukey_err;
	free(table);
	return 0;

lcd_extern_init_table_dynamic_load_ukey_abort:
	lcd_extern_init_table_save(extconf, flag, table);
	free(table);
	return -1;

lcd_extern_init_table_dynamic_load_ukey_err:
	free(table);
	return -1;
}

static int lcd_extern_init_table_fixed_load_ukey(struct lcd_extern_config_s *extconf,
		unsigned char *p, int key_len, int len, int flag)
{
	unsigned char cmd_size;
	int i = 0, j, max_len, ret = 0;
	unsigned char *table, *buf;
	char propname[20];

	cmd_size = extconf->cmd_size;
	if (flag) {
		max_len = LCD_EXTERN_INIT_ON_MAX;
		sprintf(propname, "init_on");
		buf = p;
	} else {
		max_len = LCD_EXTERN_INIT_OFF_MAX;
		sprintf(propname, "init_off");
		buf = p + extconf->table_init_on_cnt;
	}
	table = (unsigned char *)malloc(sizeof(unsigned char) * max_len);
	if (!table) {
		EXTERR("%s: Not enough memory\n", __func__);
		return -1;
	}
	table[0] = LCD_EXT_CMD_TYPE_END;
	table[1] = 0;

	while (i < max_len) {
		if ((i + cmd_size) > max_len) {
			EXTERR("%s: %s cmd_size out of support\n", extconf->name, propname);
			table[i] = LCD_EXT_CMD_TYPE_END;
			if (flag)
				extconf->table_init_on_cnt = i + cmd_size;
			else
				extconf->table_init_off_cnt = i + cmd_size;
			goto lcd_extern_init_table_fixed_load_ukey_abort;
		}
		len += cmd_size;
		ret = lcd_unifykey_len_check(key_len, len);
		if (ret) {
			EXTERR("%s: get %s failed\n", extconf->name, propname);
			table[i] = LCD_EXT_CMD_TYPE_END;
			if (flag)
				extconf->table_init_on_cnt = i + cmd_size;
			else
				extconf->table_init_off_cnt = i + cmd_size;
			goto lcd_extern_init_table_fixed_load_ukey_abort;
		}
		for (j = 0; j < cmd_size; j++)
			table[i+j] = *(buf + LCD_UKEY_EXT_INIT + i + j);

		if (table[i] == LCD_EXT_CMD_TYPE_END)
			break;

		i += cmd_size;
	}

	if (flag)
		extconf->table_init_on_cnt = i + cmd_size;
	else
		extconf->table_init_off_cnt = i + cmd_size;
	ret = lcd_extern_init_table_save(extconf, flag, table);
	if (ret)
		goto lcd_extern_init_table_fixed_load_ukey_err;
	free(table);
	return 0;

lcd_extern_init_table_fixed_load_ukey_abort:
	lcd_extern_init_table_save(extconf, flag, table);
	free(table);
	return -1;

lcd_extern_init_table_fixed_load_ukey_err:
	free(table);
	return -1;
}

static int lcd_extern_get_config_unifykey(struct lcd_extern_driver_s *edrv,
					  struct lcd_extern_dev_s *edev,
					  char *snode)
{
	unsigned char *para, *p;
	int key_len, len;
	const char *str;
	struct lcd_unifykey_header_s ext_header;
	int ret;

	edev->config.table_init_loaded = 0;
	para = (unsigned char *)malloc(sizeof(unsigned char) * LCD_UKEY_LCD_EXT_SIZE);
	if (!para) {
		EXTERR("[%d]: %s: Not enough memory\n", edrv->index, __func__);
		return -1;
	}
	key_len = LCD_UKEY_LCD_EXT_SIZE;
	memset(para, 0, (sizeof(unsigned char) * key_len));
	ret = lcd_unifykey_get(snode, para, &key_len);
	if (ret) {
		free(para);
		return -1;
	}

	/* check lcd_extern unifykey length */
	len = 10 + 33 + 10;
	ret = lcd_unifykey_len_check(key_len, len);
	if (ret) {
		EXTERR("[%d]: unifykey length is not correct\n", edrv->index);
		free(para);
		return -1;
	}

	/* header: 10byte */
	lcd_unifykey_header_check(para, &ext_header);
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		EXTPR("[%d]: unifykey header:\n", edrv->index);
		EXTPR("crc32             = 0x%08x\n", ext_header.crc32);
		EXTPR("data_len          = %d\n", ext_header.data_len);
		EXTPR("version           = 0x%04x\n", ext_header.version);
	}

	/* basic: 33byte */
	p = para;
	str = (const char *)(p + LCD_UKEY_HEAD_SIZE);
	strncpy(edev->config.name, str, LCD_EXTERN_NAME_LEN_MAX - 1);
	edev->config.index = *(p + LCD_UKEY_EXT_INDEX);
	edev->config.type = *(p + LCD_UKEY_EXT_TYPE);
	edev->config.status = *(p + LCD_UKEY_EXT_STATUS);

	if (edev->dev_index != edev->config.index) {
		EXTERR("[%d]: dev_index %d err, config index %d\n",
		       edrv->index, edev->dev_index, edev->config.index);
		free(para);
		return -1;
	}

	/* type: 10byte */
	switch (edev->config.type) {
	case LCD_EXTERN_I2C:
		if (*(p + LCD_UKEY_EXT_TYPE_VAL_0))
			edev->config.i2c_addr = *(p + LCD_UKEY_EXT_TYPE_VAL_0);
		else
			edev->config.i2c_addr = LCD_EXT_I2C_ADDR_INVALID;
		if (*(p + LCD_UKEY_EXT_TYPE_VAL_1))
			edev->config.i2c_addr2 = *(p + LCD_UKEY_EXT_TYPE_VAL_1);
		else
			edev->config.i2c_addr2 = LCD_EXT_I2C_ADDR_INVALID;
		if (*(p + LCD_UKEY_EXT_TYPE_VAL_4))
			edev->config.i2c_addr3 = *(p + LCD_UKEY_EXT_TYPE_VAL_4);
		else
			edev->config.i2c_addr3 = LCD_EXT_I2C_ADDR_INVALID;
		if (*(p + LCD_UKEY_EXT_TYPE_VAL_5))
			edev->config.i2c_addr4 = *(p + LCD_UKEY_EXT_TYPE_VAL_5);
		else
			edev->config.i2c_addr4 = LCD_EXT_I2C_ADDR_INVALID;
		edev->config.cmd_size = *(p + LCD_UKEY_EXT_TYPE_VAL_3);
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
			EXTPR("[%d]: %s(%d): cmd_size = %d\n",
			      edrv->index, edev->config.name,
			      edev->dev_index, edev->config.cmd_size);
		}

		/* init */
		if (edev->config.cmd_size == 0)
			break;
		if (edev->config.cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC) {
			ret = lcd_extern_init_table_dynamic_load_ukey(&edev->config,
					p, key_len, len, 1);
			if (ret)
				break;
			ret = lcd_extern_init_table_dynamic_load_ukey(&edev->config,
					p, key_len, len, 0);
		} else {
			ret = lcd_extern_init_table_fixed_load_ukey(&edev->config,
					p, key_len, len, 1);
			if (ret)
				break;
			ret = lcd_extern_init_table_fixed_load_ukey(&edev->config,
					p, key_len, len, 0);
		}
		if (ret == 0)
			edev->config.table_init_loaded = 1;
		break;
	case LCD_EXTERN_SPI:
		edev->config.spi_gpio_cs = *(p + LCD_UKEY_EXT_TYPE_VAL_0);
		edev->config.spi_gpio_clk = *(p + LCD_UKEY_EXT_TYPE_VAL_1);
		edev->config.spi_gpio_data = *(p + LCD_UKEY_EXT_TYPE_VAL_2);
		edev->config.spi_clk_freq = (*(p + LCD_UKEY_EXT_TYPE_VAL_3) |
			((*(p + LCD_UKEY_EXT_TYPE_VAL_4)) << 8));
		edev->config.spi_clk_pol = *(p + LCD_UKEY_EXT_TYPE_VAL_5);
		edev->config.cmd_size = *(p + LCD_UKEY_EXT_TYPE_VAL_6);
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
			EXTPR("[%d]: %s(%d): cmd_size = %d\n",
			      edrv->index, edev->config.name,
			      edev->dev_index, edev->config.cmd_size);
		}

		/* init */
		if (edev->config.cmd_size == 0)
			break;
		if (edev->config.cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC) {
			ret = lcd_extern_init_table_dynamic_load_ukey(&edev->config,
					p, key_len, len, 1);
			if (ret)
				break;
			ret = lcd_extern_init_table_dynamic_load_ukey(&edev->config,
					p, key_len, len, 0);
		} else {
			ret = lcd_extern_init_table_fixed_load_ukey(&edev->config,
					p, key_len, len, 1);
			if (ret)
				break;
			ret = lcd_extern_init_table_fixed_load_ukey(&edev->config,
					p, key_len, len, 0);
		}
		if (ret == 0)
			edev->config.table_init_loaded = 1;
		break;
	case LCD_EXTERN_MIPI:
		edev->config.cmd_size = *(p + LCD_UKEY_EXT_TYPE_VAL_0);
		if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
			EXTPR("[%d]: %s(%d): cmd_size = %d\n",
			      edrv->index, edev->config.name,
			      edev->dev_index, edev->config.cmd_size);
		}

		if (edev->config.cmd_size != LCD_EXT_CMD_SIZE_DYNAMIC)
			break;
		if (edev->config.cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC) {
			ret = lcd_extern_init_table_dynamic_load_ukey(&edev->config,
					p, key_len, len, 1);
			if (ret)
				break;
			ret = lcd_extern_init_table_dynamic_load_ukey(&edev->config,
					p, key_len, len, 0);
		} else {
			ret = -1;
		}
		if (ret == 0)
			edev->config.table_init_loaded = 1;
		break;
	default:
		break;
	}

	free(para);
	return 0;
}

static void lcd_extern_multi_list_add(struct lcd_extern_dev_s *edev,
		unsigned int index, unsigned int type,
		unsigned char data_len, unsigned char *data_buf)
{
	struct lcd_extern_multi_list_s *temp_list;
	struct lcd_extern_multi_list_s *cur_list;

	/* creat list */
	cur_list = (struct lcd_extern_multi_list_s *)
		malloc(sizeof(struct lcd_extern_multi_list_s));
	if (!cur_list)
		return;
	memset(cur_list, 0, sizeof(struct lcd_extern_multi_list_s));
	cur_list->index = index;
	cur_list->type = type;
	cur_list->data_len = data_len;
	cur_list->data_buf = data_buf;

	if (!edev->multi_list_header) {
		edev->multi_list_header = cur_list;
	} else {
		temp_list = edev->multi_list_header;
		while (temp_list->next) {
			if (temp_list->index == cur_list->index) {
				EXTERR("%s: dev_%d: index=%d(type=%d) already in list\n",
					__func__, edev->dev_index,
					cur_list->index, cur_list->type);
				free(cur_list);
				return;
			}
			temp_list = temp_list->next;
		}
		temp_list->next = cur_list;
	}

	EXTPR("%s: dev_%d: index=%d, type=%d\n",
	       __func__, edev->dev_index, cur_list->index, cur_list->type);
}

static int lcd_extern_multi_list_remove(struct lcd_extern_dev_s *edev)
{
	struct lcd_extern_multi_list_s *cur_list;
	struct lcd_extern_multi_list_s *next_list;

	/* add to exist list */
	cur_list = edev->multi_list_header;
	while (cur_list) {
		next_list = cur_list->next;
		free(cur_list);
		cur_list = next_list;
	}
	edev->multi_list_header = NULL;

	return 0;
}

static void lcd_extern_config_update_dynamic_size(struct lcd_extern_dev_s *edev,
						  int flag)
{
	unsigned char type, size, *table;
	unsigned int max_len = 0, i = 0, j, index;

	if (flag) {
		max_len = edev->config.table_init_on_cnt;
		table = edev->config.table_init_on;
	} else {
		max_len = edev->config.table_init_off_cnt;
		table = edev->config.table_init_off;
	}

	while ((i + 1) < max_len) {
		type = table[i];
		size = table[i + 1];
		if (type == LCD_EXT_CMD_TYPE_END)
			break;
		if (size == 0)
			goto lcd_extern_config_update_dynamic_size_next;
		if ((i + 2 + size) > max_len)
			break;

		if (type == LCD_EXT_CMD_TYPE_MULTI_FR) {
			for (j = 0; j < size; j += 3) {
				index = i + 2 + j;
				lcd_extern_multi_list_add(edev, table[index],
					type, 2, &table[index + 1]);
			}
		}
lcd_extern_config_update_dynamic_size_next:
		i += (size + 2);
	}
}

static void lcd_extern_config_update(struct lcd_extern_dev_s *edev)
{
	if (edev->config.cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC) {
		lcd_extern_config_update_dynamic_size(edev, 1);
		lcd_extern_config_update_dynamic_size(edev, 0);
	}
}

static int lcd_extern_add_dev(struct lcd_extern_driver_s *edrv,
			      struct lcd_extern_dev_s *edev)
{
	int ret = -1;

	if (edev->config.status == 0) {
		EXTERR("[%d]: %s: %s(%d) is disabled\n",
		       edrv->index, __func__,
		       edev->config.name, edev->dev_index);
		return -1;
	}

	if (strcmp(edev->config.name, "ext_default") == 0) {
		if (edev->config.type == LCD_EXTERN_MIPI)
			ret = lcd_extern_mipi_default_probe(edrv, edev);
		else
			ret = lcd_extern_default_probe(edrv, edev);
	} else if (strcmp(edev->config.name, "mipi_default") == 0) {
		ret = lcd_extern_mipi_default_probe(edrv, edev);
#ifdef CONFIG_AML_LCD_EXTERN_I2C_RT6947
	} else if (strcmp(edev->config.name, "i2c_RT6947") == 0) {
		ret = lcd_extern_i2c_RT6947_probe(edrv, edev);
#endif
#ifdef CONFIG_AML_LCD_EXTERN_I2C_ANX6862_7911
	} else if (strcmp(edev->config.name, "i2c_ANX6862_7911") == 0) {
		ret = lcd_extern_i2c_ANX6862_7911_probe(edrv, edev);
#endif
	} else {
		EXTERR("[%d]: %s: invalid dev: %s(%d)\n",
		       edrv->index, __func__,
		       edev->config.name, edev->dev_index);
	}
	if (ret) {
		EXTERR("[%d]: %s: %s(%d) failed\n",
		       edrv->index, __func__,
		       edev->config.name, edev->dev_index);
		return -1;
	}

	EXTPR("[%d]: %s: %s(%d) ok\n",
	      edrv->index, __func__,
	      edev->config.name, edev->dev_index);
	return ret;
}

static int lcd_extern_dev_probe(char *dtaddr, int load_id,
				struct lcd_extern_driver_s *edrv,
				int n, int dev_index)
{
	struct lcd_dft_config_s *dft_conf;
	struct lcd_extern_dev_s *edev;
	struct lcd_extern_config_s *ext_conf;
	char skey[15], snode[15];
	int ret = -1;

	if (!edrv->dev[n]) {
		edrv->dev[n] = (struct lcd_extern_dev_s *)
			malloc(sizeof(struct lcd_extern_dev_s));
		if (!edrv->dev[n]) {
			EXTERR("[%d]: %s: Not enough memory\n",
			       edrv->index, __func__);
			return -1;
		}
	}
	edev = edrv->dev[n];
	memset(edev, 0, sizeof(struct lcd_extern_dev_s));
	edev->dev_index = dev_index;

	if (edrv->index == 0) {
		sprintf(snode, "/lcd_extern");
		sprintf(skey, "lcd_extern");
	} else {
		sprintf(snode, "/lcd%d_extern", edrv->index);
		sprintf(skey, "lcd%d_extern", edrv->index);
	}

	if (load_id & 0x1) {/* dts */
		/* check unifykey config */
		if (edrv->key_valid) {
			ret = lcd_unifykey_check(skey);
			if (ret == 0) {
				EXTPR("[%d]: load config %d from unifykey\n",
				      edrv->index, dev_index);
				ret = lcd_extern_get_config_unifykey(edrv, edev, skey);
			}
		} else {
			EXTPR("[%d]: load config %d from dts\n",
			      edrv->index, dev_index);
			ret = lcd_extern_get_config_dts(dtaddr, snode, edrv, edev);
		}
	} else {
		if (edrv->key_valid) {
			ret = lcd_unifykey_check(skey);
			if (ret == 0) {
				EXTPR("[%d]: load config %d from unifykey\n",
				      edrv->index, dev_index);
				ret = lcd_extern_get_config_unifykey(edrv, edev, skey);
			}
		} else {
			EXTPR("[%d]: load config %d from bsp\n",
			      edrv->index, dev_index);
			dft_conf = edrv->data->dft_conf[edrv->index];
			if (dev_index >= dft_conf->ext_common->ext_num) {
				EXTERR("[%d]: %s: %d invalid\n",
				       edrv->index, __func__, dev_index);
			} else {
				if (dft_conf->ext_conf) {
					ext_conf = dft_conf->ext_conf + dev_index;
					memcpy(&edev->config, ext_conf,
					       sizeof(struct lcd_extern_config_s));
					ret = 0;
				}
			}
		}
	}
	if (ret == 0) {
		lcd_extern_config_update(edev);
		ret = lcd_extern_add_dev(edrv, edev);
	}

	if (ret) {
		EXTERR("[%d]: %s: %d failed\n", edrv->index, __func__, dev_index);
		lcd_extern_multi_list_remove(edev);
		free(edev);
		edrv->dev[n] = NULL;
		return -1;
	}

	EXTPR("[%d]: %s: %s(%d) ok\n",
	      edrv->index, __func__, edev->config.name, dev_index);
	return 0;
}

static unsigned int lcd_extern_get_drv_cnt_flag_from_dts(struct aml_lcd_data_s *pdata,
							 char *dt_addr)
{
#ifdef CONFIG_OF_LIBFDT
	int parent_offset;
	char str[20];
	unsigned int i, flag = 0;

	for (i = 0; i < pdata->drv_max; i++) {
		if (i == 0)
			sprintf(str, "/lcd_extern");
		else
			sprintf(str, "/lcd%d_extern", i);

		parent_offset = fdt_path_offset(dt_addr, str);
		if (parent_offset < 0) {
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
				EXTPR("not find %s node\n", str);
		} else {
			flag |= (1 << i);
		}
	}

	return flag;
#else
	return 0;
#endif
}

static unsigned int lcd_extern_get_drv_cnt_flag_from_bsp(struct aml_lcd_data_s *pdata)
{
	unsigned int i, flag = 0;

	for (i = 0; i < pdata->drv_max; i++) {
		if (!pdata->dft_conf[i])
			continue;
		if (pdata->dft_conf[i]->ext_common) {
			flag |= (1 << i);
		} else {
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
				EXTPR("not find ext_common[%d]\n", i);
		}
	}

	return flag;
}

int lcd_extern_probe(char *dtaddr, int load_id)
{
	struct aml_lcd_data_s *pdata = aml_lcd_get_data();
	struct lcd_extern_driver_s *edrv;
	unsigned int drv_cnt_flag;
	int load_id_ext, load_id_temp, dev_index;
	int ret = 0, i, j;

	load_id_ext = load_id;
	if (load_id_ext & 0x1)
		drv_cnt_flag = lcd_extern_get_drv_cnt_flag_from_dts(pdata, dtaddr);
	else
		drv_cnt_flag = lcd_extern_get_drv_cnt_flag_from_bsp(pdata);

	for (i = 0; i < LCD_MAX_DRV; i++) {
		if ((drv_cnt_flag & (1 << i)) == 0)
			continue;

		if (!ext_driver[i]) {
			ext_driver[i] = (struct lcd_extern_driver_s *)
				malloc(sizeof(struct lcd_extern_driver_s));
			if (!ext_driver[i]) {
				EXTERR("[%d]: %s: Not enough memory\n", i, __func__);
				return -1;
			}
		}
		edrv = ext_driver[i];
		memset(edrv, 0, sizeof(struct lcd_extern_driver_s));
		edrv->data = pdata;
		edrv->index = i;
		edrv->dev_cnt = lcd_ext_dev_cnt[i];
		edrv->info_print = lcd_extern_info_print;
		edrv->power_ctrl = lcd_extern_power_ctrl;

		if (load_id_ext & 0x1)
			ret = lcd_extern_get_init_dts(dtaddr, edrv);
		else
			ret = lcd_extern_get_init_bsp(edrv);
		load_id_temp = load_id_ext & 0xff;
		if ((load_id_ext & (1 << 8)) == 0) {
			if (edrv->key_valid)
				load_id_temp |= (1 << 4);
			else
				load_id_temp &= ~(1 << 4);
		}

		lcd_extern_pinmux_load_from_bsp(edrv);

		for (j = 0; j < edrv->dev_cnt; j++) {
			dev_index = lcd_ext_index_lut[i][j];
			ret = lcd_extern_dev_probe(dtaddr, load_id_temp,
						   edrv, j, dev_index);
		}
	}

	return ret;
}

int lcd_extern_remove(void)
{
	int i, j;

	for (i = 0; i < LCD_MAX_DRV; i++) {
		for (j = 0; j < LCD_EXTERN_DEV_MAX; j++)
			lcd_ext_index_lut[i][j] = LCD_EXTERN_INDEX_INVALID;
		if (ext_driver[i])
			free(ext_driver[i]);
		ext_driver[i] = NULL;
		lcd_ext_dev_cnt[i] = 0;
	}

	return 0;
}

int lcd_extern_drv_index_add(int drv_index, int dev_index)
{
	int dev_cnt, i;

	if (drv_index >= LCD_MAX_DRV) {
		EXTERR("%s: invalid drv_index: %d\n", __func__, drv_index);
		return -1;
	}
	if (dev_index == 0xff)
		return 0;

	dev_cnt = lcd_ext_dev_cnt[drv_index];
	if (dev_cnt >= LCD_EXTERN_DEV_MAX) {
		EXTERR("[%d]: %s: out off dev_cnt support\n", drv_index, __func__);
		return -1;
	}

	for (i = 0; i < LCD_EXTERN_DEV_MAX; i++) {
		if (lcd_ext_index_lut[drv_index][i] == dev_index) {
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				EXTPR("[%d]: %s: dev_index %d already exist\n",
				      drv_index, __func__, dev_index);
			}
			return 0;
		}
	}

	lcd_ext_index_lut[drv_index][dev_cnt] = dev_index;
	lcd_ext_dev_cnt[drv_index]++;
	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		EXTPR("[%d]: %s: dev_index: %d, dev_cnt: %d\n",
		      drv_index, __func__, dev_index,
		      lcd_ext_dev_cnt[drv_index]);
	}
	return 0;
}

int lcd_extern_drv_index_remove(int drv_index, int dev_index)
{
	int find, i;

	if (drv_index >= LCD_MAX_DRV) {
		BLERR("%s: invalid drv_index: %d\n", __func__, dev_index);
		return -1;
	}
	if (dev_index == 0xff)
		return 0;

	if (lcd_ext_dev_cnt[drv_index] == 0)
		return -1;

	find = 0xff;
	for (i = 0; i < LCD_EXTERN_DEV_MAX; i++) {
		if (lcd_ext_index_lut[drv_index][i] == dev_index)
			find = i;
	}
	if (find == 0xff)
		return 0;

	lcd_ext_index_lut[drv_index][find] = LCD_EXTERN_INDEX_INVALID;
	for (i = (find + 1); i < LCD_EXTERN_DEV_MAX; i++) {
		if (lcd_ext_index_lut[drv_index][i] == LCD_EXTERN_INDEX_INVALID)
			break;
		lcd_ext_index_lut[drv_index][i - 1] = lcd_ext_index_lut[drv_index][i];
		lcd_ext_index_lut[drv_index][i] = LCD_EXTERN_INDEX_INVALID;
	}
	if (lcd_ext_dev_cnt[drv_index])
		lcd_ext_dev_cnt[drv_index]--;

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL)
		EXTPR("[%d]: %s: dev_index: %d\n", drv_index, __func__, dev_index);
	return 0;
}

int lcd_extern_init(void)
{
	int i, j;

	for (i = 0; i < LCD_MAX_DRV; i++) {
		for (j = 0; j < LCD_EXTERN_DEV_MAX; j++)
			lcd_ext_index_lut[i][j] = LCD_EXTERN_INDEX_INVALID;
		if (ext_driver[i])
			free(ext_driver[i]);
		ext_driver[i] = NULL;
		lcd_ext_dev_cnt[i] = 0;
	}

	return 0;
}
