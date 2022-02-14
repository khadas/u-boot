// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/gpio.h>
#include <fdtdec.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include <amlogic/media/vout/lcd/lcd_extern.h>
#include "lcd_extern.h"
#include "../lcd_common.h"
#include "../lcd_reg.h"

#define EXT_DEFAULT_NAME	"ext_default"

static unsigned char check_state[4] = {0, 0, 0, 0};
static unsigned char check_flag;
static unsigned char check_offset;
static unsigned char check_len;

static void set_lcd_csb(struct lcd_extern_driver_s *edrv,
			struct lcd_extern_dev_s *edev, unsigned int v)
{
	lcd_ext_gpio_set(edrv, edev->config.spi_gpio_cs, v);
	udelay(edev->config.spi_delay_us);
}

static void set_lcd_scl(struct lcd_extern_driver_s *edrv,
			struct lcd_extern_dev_s *edev, unsigned int v)
{
	lcd_ext_gpio_set(edrv, edev->config.spi_gpio_clk, v);
	udelay(edev->config.spi_delay_us);
}

static void set_lcd_sda(struct lcd_extern_driver_s *edrv,
			struct lcd_extern_dev_s *edev, unsigned int v)
{
	lcd_ext_gpio_set(edrv, edev->config.spi_gpio_data, v);
	udelay(edev->config.spi_delay_us);
}

static void spi_gpio_init(struct lcd_extern_driver_s *edrv,
			  struct lcd_extern_dev_s *edev)
{
	set_lcd_csb(edrv, edev, 1);
	set_lcd_scl(edrv, edev, 1);
	set_lcd_sda(edrv, edev, 1);
}

static void spi_gpio_off(struct lcd_extern_driver_s *edrv,
			 struct lcd_extern_dev_s *edev)
{
	set_lcd_sda(edrv, edev, 0);
	set_lcd_scl(edrv, edev, 0);
	set_lcd_csb(edrv, edev, 0);
}

static void spi_write_byte(struct lcd_extern_driver_s *edrv,
			   struct lcd_extern_dev_s *edev, unsigned char data)
{
	int i;

	for (i = 0; i < 8; i++) {
		set_lcd_scl(edrv, edev, 0);
		if (data & 0x80)
			set_lcd_sda(edrv, edev, 1);
		else
			set_lcd_sda(edrv, edev, 0);
		data <<= 1;
		set_lcd_scl(edrv, edev, 1);
	}
}

static int lcd_extern_spi_read(struct lcd_extern_driver_s *edrv,
			       struct lcd_extern_dev_s *edev,
			       unsigned char reg, unsigned char *buf)
{
	EXTERR("[%d]: %s: %s(%d): extern_type %d is not support\n",
	       edrv->index, __func__, edev->config.name,
	       edev->config.index, edev->config.type);

	return -1;
}

static int lcd_extern_spi_write(struct lcd_extern_driver_s *edrv,
				struct lcd_extern_dev_s *edev,
				unsigned char *buf, int len)
{
	int i;

	if (len < 2) {
		EXTERR("[%d]: %s: len %d error\n", edrv->index, __func__, len);
		return -1;
	}

	set_lcd_csb(edrv, edev, 0);

	for (i = 0; i < len; i++)
		spi_write_byte(edrv, edev, buf[i]);

	set_lcd_csb(edrv, edev, 1);
	set_lcd_scl(edrv, edev, 1);
	set_lcd_sda(edrv, edev, 1);
	udelay(edev->config.spi_delay_us);

	return 0;
}

static int lcd_extern_reg_read(struct lcd_extern_driver_s *edrv,
			       struct lcd_extern_dev_s *edev,
			       unsigned char reg, unsigned char *buf)
{
	int ret = 0;

	if (!buf) {
		EXTERR("[%d]: %s: buf is null\n", edrv->index, __func__);
		return -1;
	}

	switch (edev->config.type) {
	case LCD_EXTERN_I2C:
		buf[0] = reg;
		ret = aml_lcd_i2c_read(edrv->i2c_bus,
				       edev->config.i2c_addr, buf, 1);
		break;
	case LCD_EXTERN_SPI:
		ret = lcd_extern_spi_read(edrv, edev, reg, buf);
		break;
	default:
		EXTERR("[%d]: %s: %s(%d): extern_type %d is not support\n",
		       edrv->index, __func__, edev->config.name,
		       edev->config.index, edev->config.type);
		ret = -1;
		break;
	}
	if (ret)
		EXTERR("[%d]: %s: failed\n", edrv->index, __func__);

	return ret;
}

static int lcd_extern_reg_write(struct lcd_extern_driver_s *edrv,
				struct lcd_extern_dev_s *edev,
				unsigned char *buf, unsigned int len)
{
	int ret = 0;

	if (!buf) {
		EXTERR("[%d]: %s: buf is null\n", edrv->index, __func__);
		return -1;
	}

	if (!len) {
		EXTERR("[%d]: %s: invalid len\n", edrv->index, __func__);
		return -1;
	}

	switch (edev->config.type) {
	case LCD_EXTERN_I2C:
		ret = aml_lcd_i2c_write(edrv->i2c_bus,
				        edev->config.i2c_addr, buf, len);
		break;
	case LCD_EXTERN_SPI:
		ret = lcd_extern_spi_write(edrv, edev, buf, len);
		break;
	default:
		EXTERR("[%d]: %s: %s(%d): extern_type %d is not support\n",
		       edrv->index, __func__, edev->config.name,
		       edev->config.index, edev->config.type);
		ret = -1;
		break;
	}
	if (ret)
		EXTERR("[%d]: %s: failed\n", edrv->index, __func__);

	return ret;
}

static void lcd_extern_init_reg_check(unsigned char i2c_bus,
				      unsigned char i2c_addr,
				      unsigned char type,
				      unsigned char *raw_table,
				      unsigned char data_len)
{
	unsigned char *chk_table, *chk_buf, *raw_buf;
	unsigned char index;
	unsigned char temp_flag = check_flag;
	int ret = 0;

	/* if not need to check return */
	if (check_flag == 0)
		return;

	index = type & 0x0f;
	if (index >= 4 || data_len < 1 || !raw_table)
		goto parameter_err0;
	check_state[index] = 0;
	check_flag = 0;
	chk_table = malloc(data_len * sizeof(unsigned char));
	if (!chk_table)
		goto parameter_err0;
	memset(chk_table, 0, data_len);
	if (((type & 0xf0) == LCD_EXT_CMD_TYPE_CMD) ||
	    ((type & 0xf0) == LCD_EXT_CMD_TYPE_CMD_BIN)) {
		if (data_len < 2)
			goto parameter_err1;
		chk_table[0] = raw_table[0];
		data_len--;
		raw_table++;
	}
	ret = aml_lcd_i2c_read(i2c_bus, i2c_addr, chk_table, data_len);
	if (!check_len || check_len > data_len)
		check_len = data_len;
	if (check_offset > data_len)
		check_offset = 0;
	if ((ret) || (check_offset + check_len) > data_len)
		goto parameter_err1;
	chk_buf = chk_table + check_offset;
	raw_buf = raw_table + check_offset;
	ret = memcmp(chk_buf, raw_buf, check_len);
	if (ret == 0)
		check_state[index] = temp_flag;
	if (lcd_debug_print_flag)
		EXTERR("%s: ret : %d\n", __func__, ret);
	free(chk_table);
	return;
parameter_err1:
	free(chk_table);
parameter_err0:
	EXTERR("%s: error parameters\n", __func__);
}

static void lcd_extern_init_reg_check2(unsigned char i2c_bus,
				       unsigned char i2c_addr,
				       unsigned char type,
				       unsigned char *raw_table,
				       unsigned char data_len)
{
	unsigned char *chk_table, *chk_buf, *raw_buf;
	unsigned char index;
	unsigned char temp_flag = check_flag;
	int ret = 0;

	/* if not need to check return */
	if (check_flag == 0)
		return;

	index = type & 0x0f;
	if (index >= 4 || data_len < 1 || !raw_table)
		goto parameter_err0;
	check_state[index] = 0;
	check_flag = 0;
	chk_table = malloc(data_len * sizeof(unsigned char));
	if (!chk_table)
		goto parameter_err0;
	memset(chk_table, 0, data_len);
	ret = aml_lcd_i2c_read(i2c_bus, i2c_addr, chk_table,
			       check_offset + data_len);
	if (!check_len || check_len > data_len)
		check_len = data_len;
	if (ret)
		goto parameter_err1;
	chk_buf = chk_table + check_offset;
	raw_buf = raw_table + 1;
	ret = memcmp(chk_buf, raw_buf, check_len);
	if (ret == 0)
		check_state[index] = temp_flag;
	if (lcd_debug_print_flag)
		EXTPR("%s: ret : %d\n", __func__, ret);
	free(chk_table);
	return;
parameter_err1:
	free(chk_table);
parameter_err0:
	EXTERR("%s: error parameters\n", __func__);
}

static int lcd_extern_cmd_multi_id(struct lcd_extern_driver_s *edrv,
				   struct lcd_extern_dev_s *edev,
				   unsigned char multi_id)
{
	struct aml_lcd_drv_s *pdrv;
	struct lcd_extern_multi_list_s *temp_list;
	unsigned int frame_rate, frame_rate_max, frame_rate_min;

	pdrv = aml_lcd_get_driver(edrv->index);
	if (!pdrv)
		return -1;
	if (pdrv->config.cus_ctrl.dlg_flag == 0)
		return -1;

	frame_rate = pdrv->config.timing.sync_duration_num /
		pdrv->config.timing.sync_duration_den;

	temp_list = edev->multi_list_header;
	while (temp_list) {
		if (multi_id == temp_list->index) {
			if (temp_list->type == LCD_EXT_CMD_TYPE_MULTI_FR) {
				frame_rate_min = temp_list->data_buf[0];
				frame_rate_max = temp_list->data_buf[1];
				if (frame_rate < frame_rate_min ||
				    frame_rate > frame_rate_max) {
					return -1;
				}
				EXTPR("[%d]: %s: dev_%d: multi_id=%d, type=0x%x, framerate=%d\n",
					edrv->index, __func__, edev->dev_index,
					temp_list->index, temp_list->type, frame_rate);
				return 0;
			}
		}
		temp_list = temp_list->next;
	}
	return -1;
}

static int lcd_extern_power_cmd_dynamic_size(struct lcd_extern_driver_s *edrv,
					     struct lcd_extern_dev_s *edev,
					     unsigned char *table, int flag)
{
	int i = 0, j = 0, max_len = 0, step = 0;
	unsigned char type, temp, size, i2c_bus, i2c_addr;
	int delay_ms, ret = 0;

	i2c_bus = edrv->i2c_bus;
	if (flag)
		max_len = edev->config.table_init_on_cnt;
	else
		max_len = edev->config.table_init_off_cnt;

	switch (edev->config.type) {
	case LCD_EXTERN_I2C:
		while ((i + 1) < max_len) {
			type = table[i];
			size = table[i + 1];
			if (type == LCD_EXT_CMD_TYPE_END)
				break;
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				EXTPR("%s: step %d: type=0x%02x, size=%d\n",
				      __func__, step, type, size);
			}
			if (size == 0)
				goto power_cmd_dynamic_i2c_next;
			if ((i + 2 + size) > max_len)
				break;

			if (type == LCD_EXT_CMD_TYPE_NONE) {
				/* do nothing */
			} else if (type == LCD_EXT_CMD_TYPE_GPIO) {
				if (size < 2) {
					EXTERR("step %d: invalid size %d for GPIO\n",
					       step, size);
					goto power_cmd_dynamic_i2c_next;
				}
				lcd_ext_gpio_set(edrv, table[i + 2], table[i + 3]);
				if (size > 2) {
					if (table[i + 4] > 0)
						mdelay(table[i + 4]);
				}
			} else if (type == LCD_EXT_CMD_TYPE_DELAY) {
				delay_ms = 0;
				for (j = 0; j < size; j++)
					delay_ms += table[i + 2 + j];
				if (delay_ms > 0)
					mdelay(delay_ms);
			} else if (type == LCD_EXT_CMD_TYPE_CHECK) {
				if (size == 1) {
					check_flag = table[i + 2];
					check_offset = 0;
					check_len = 0;
				} else if (size == 3) {
					check_flag = table[i + 2];
					check_offset = table[i + 3];
					check_len = table[i + 4];
				} else {
					check_flag = 0;
				}
			} else if ((type == LCD_EXT_CMD_TYPE_CMD) ||
				   (type == LCD_EXT_CMD_TYPE_CMD_BIN) ||
				   (type == LCD_EXT_CMD_TYPE_CMD_BIN_DATA)) {
				i2c_addr = edev->config.i2c_addr;
				lcd_extern_init_reg_check(i2c_bus,
							  i2c_addr,
							  type,
							  &table[i + 2],
							  size);
				if (check_state[0] == 1)
					goto power_cmd_dynamic_i2c_next;
				ret = aml_lcd_i2c_write(i2c_bus,
							i2c_addr,
							&table[i + 2],
							size);
			} else if (type == LCD_EXT_CMD_TYPE_CMD_BIN2) {
				i2c_addr = edev->config.i2c_addr;
				lcd_extern_init_reg_check2(i2c_bus,
							   i2c_addr,
							   type,
							   &table[i + 2],
							   size);
				if (check_state[0] == 1)
					goto power_cmd_dynamic_i2c_next;
				ret = aml_lcd_i2c_write(i2c_bus,
							i2c_addr,
							&table[i + 2],
							size);
			} else if ((type == LCD_EXT_CMD_TYPE_CMD2) ||
				   (type == LCD_EXT_CMD_TYPE_CMD2_BIN) ||
				   (type == LCD_EXT_CMD_TYPE_CMD2_BIN_DATA)) {
				i2c_addr = edev->config.i2c_addr2;
				lcd_extern_init_reg_check(i2c_bus,
							  i2c_addr,
							  type,
							  &table[i + 2],
							  size);
				if (check_state[1] == 1)
					goto power_cmd_dynamic_i2c_next;
				ret = aml_lcd_i2c_write(i2c_bus,
							i2c_addr,
							&table[i + 2],
							size);
			} else if (type == LCD_EXT_CMD_TYPE_CMD2_BIN2) {
				i2c_addr = edev->config.i2c_addr2;
				lcd_extern_init_reg_check2(i2c_bus,
							   i2c_addr,
							   type,
							   &table[i + 2],
							   size);
				if (check_state[1] == 1)
					goto power_cmd_dynamic_i2c_next;
				ret = aml_lcd_i2c_write(i2c_bus,
							i2c_addr,
							&table[i + 2],
							size);
			} else if ((type == LCD_EXT_CMD_TYPE_CMD3) ||
				   (type == LCD_EXT_CMD_TYPE_CMD3_BIN) ||
				   (type == LCD_EXT_CMD_TYPE_CMD3_BIN_DATA)) {
				i2c_addr = edev->config.i2c_addr3;
				lcd_extern_init_reg_check(i2c_bus,
							  i2c_addr,
							  type,
							  &table[i + 2],
							  size);
				if (check_state[2] == 1)
					goto power_cmd_dynamic_i2c_next;
				ret = aml_lcd_i2c_write(i2c_bus,
							i2c_addr,
							&table[i + 2],
							size);
			} else if (type == LCD_EXT_CMD_TYPE_CMD3_BIN2) {
				i2c_addr = edev->config.i2c_addr3;
				lcd_extern_init_reg_check2(i2c_bus,
							   i2c_addr,
							   type,
							   &table[i + 2],
							   size);
				if (check_state[2] == 1)
					goto power_cmd_dynamic_i2c_next;
				ret = aml_lcd_i2c_write(i2c_bus,
							i2c_addr,
							&table[i + 2],
							size);
			} else if ((type == LCD_EXT_CMD_TYPE_CMD4) ||
				   (type == LCD_EXT_CMD_TYPE_CMD4_BIN) ||
				   (type == LCD_EXT_CMD_TYPE_CMD4_BIN_DATA)) {
				i2c_addr = edev->config.i2c_addr4;
				lcd_extern_init_reg_check(i2c_bus,
							  i2c_addr,
							  type,
							  &table[i + 2],
							  size);
				if (check_state[3] == 1)
					goto power_cmd_dynamic_i2c_next;
				ret = aml_lcd_i2c_write(i2c_bus,
							i2c_addr,
							&table[i + 2],
							size);
			} else if (type == LCD_EXT_CMD_TYPE_CMD4_BIN2) {
				i2c_addr = edev->config.i2c_addr4;
				lcd_extern_init_reg_check2(i2c_bus,
							   i2c_addr,
							   type,
							   &table[i + 2],
							   size);
				if (check_state[3] == 1)
					goto power_cmd_dynamic_i2c_next;
				ret = aml_lcd_i2c_write(i2c_bus,
							i2c_addr,
							&table[i + 2],
							size);
			} else if (type == LCD_EXT_CMD_TYPE_MULTI_FR) {
				/* do nothing here */
			} else if (type == LCD_EXT_CMD_TYPE_CMD_MULTI) {
				ret = lcd_extern_cmd_multi_id(edrv, edev, table[i + 2]);
				if (ret)
					goto power_cmd_dynamic_i2c_next;
				i2c_addr = edev->config.i2c_addr;
				temp = (table[i + 3] << 4);
				if (temp == LCD_EXT_CMD_TYPE_CMD_BIN2) {
					lcd_extern_init_reg_check2(i2c_bus, i2c_addr,
						temp, &table[i + 4], (size - 2));
				} else {
					lcd_extern_init_reg_check(i2c_bus, i2c_addr,
						temp, &table[i + 4], (size - 2));
				}
				if (check_state[0] == 1)
					goto power_cmd_dynamic_i2c_next;
				ret = aml_lcd_i2c_write(i2c_bus, i2c_addr,
					&table[i + 4], (size - 2));
			} else if (type == LCD_EXT_CMD_TYPE_CMD2_MULTI) {
				ret = lcd_extern_cmd_multi_id(edrv, edev, table[i + 2]);
				if (ret)
					goto power_cmd_dynamic_i2c_next;
				i2c_addr = edev->config.i2c_addr2;
				temp = (table[i + 3] << 4) | (1 << 0);
				if (temp == LCD_EXT_CMD_TYPE_CMD2_BIN2) {
					lcd_extern_init_reg_check2(i2c_bus, i2c_addr,
						temp, &table[i + 4], (size - 2));
				} else {
					lcd_extern_init_reg_check(i2c_bus, i2c_addr,
						temp, &table[i + 4], (size - 2));
				}
				if (check_state[1] == 1)
					goto power_cmd_dynamic_i2c_next;
				ret = aml_lcd_i2c_write(i2c_bus, i2c_addr,
					&table[i + 4], (size - 2));
			} else if (type == LCD_EXT_CMD_TYPE_CMD3_MULTI) {
				ret = lcd_extern_cmd_multi_id(edrv, edev, table[i + 2]);
				if (ret)
					goto power_cmd_dynamic_i2c_next;
				i2c_addr = edev->config.i2c_addr3;
				temp = (table[i + 3] << 4) | (2 << 0);
				if (temp == LCD_EXT_CMD_TYPE_CMD3_BIN2) {
					lcd_extern_init_reg_check2(i2c_bus, i2c_addr,
						temp, &table[i + 4], (size - 2));
				} else {
					lcd_extern_init_reg_check(i2c_bus, i2c_addr,
						temp, &table[i + 4], (size - 2));
				}
				if (check_state[2] == 1)
					goto power_cmd_dynamic_i2c_next;
				ret = aml_lcd_i2c_write(i2c_bus, i2c_addr,
					&table[i + 4], (size - 2));
			} else if (type == LCD_EXT_CMD_TYPE_CMD4_MULTI) {
				ret = lcd_extern_cmd_multi_id(edrv, edev, table[i + 2]);
				if (ret)
					goto power_cmd_dynamic_i2c_next;
				i2c_addr = edev->config.i2c_addr4;
				temp = (table[i + 3] << 4) | (3 << 0);
				if (temp == LCD_EXT_CMD_TYPE_CMD4_BIN2) {
					lcd_extern_init_reg_check2(i2c_bus, i2c_addr,
						temp, &table[i + 4], (size - 2));
				} else {
					lcd_extern_init_reg_check(i2c_bus, i2c_addr,
						temp, &table[i + 4], (size - 2));
				}
				if (check_state[3] == 1)
					goto power_cmd_dynamic_i2c_next;
				ret = aml_lcd_i2c_write(i2c_bus, i2c_addr,
					&table[i + 4], (size - 2));
			} else if (type == LCD_EXT_CMD_TYPE_CMD_DELAY) {
				ret = aml_lcd_i2c_write(i2c_bus,
					 edev->config.i2c_addr,
					 &table[i + 2], (size - 1));
				if (table[i + 1 + size] > 0)
					mdelay(table[i + 1 + size]);
			} else if (type == LCD_EXT_CMD_TYPE_CMD2_DELAY) {
				ret = aml_lcd_i2c_write(i2c_bus,
					 edev->config.i2c_addr2,
					 &table[i + 2], (size - 1));
				if (table[i + 1 + size] > 0)
					mdelay(table[i + 1 + size]);
			} else if (type == LCD_EXT_CMD_TYPE_CMD3_DELAY) {
				ret = aml_lcd_i2c_write(i2c_bus,
					 edev->config.i2c_addr3,
					 &table[i + 2], (size - 1));
				if (table[i + 1 + size] > 0)
					mdelay(table[i + 1 + size]);
			} else if (type == LCD_EXT_CMD_TYPE_CMD4_DELAY) {
				ret = aml_lcd_i2c_write(i2c_bus,
					 edev->config.i2c_addr4,
					 &table[i + 2], (size - 1));
				if (table[i + 1 + size] > 0)
					mdelay(table[i + 1 + size]);
			} else {
				EXTERR("%s: %s(%d): type 0x%02x invalid\n",
				       __func__, edev->config.name,
				       edev->config.index, type);
			}
power_cmd_dynamic_i2c_next:
			i += (size + 2);
			step++;
		}
		break;
	case LCD_EXTERN_SPI:
		while ((i + 1) < max_len) {
			type = table[i];
			size = table[i + 1];
			if (type == LCD_EXT_CMD_TYPE_END)
				break;
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				EXTPR("%s: step %d: type=0x%02x, size=%d\n",
				      __func__, step, type, size);
			}
			if (size == 0)
				goto power_cmd_dynamic_spi_next;
			if ((i + 2 + size) > max_len)
				break;

			if (type == LCD_EXT_CMD_TYPE_NONE) {
				/* do nothing */
			} else if (type == LCD_EXT_CMD_TYPE_GPIO) {
				if (size < 2) {
					EXTERR("step %d: invalid size %d for GPIO\n",
					       step, size);
					goto power_cmd_dynamic_spi_next;
				}
				lcd_ext_gpio_set(edrv, table[i + 2], table[i + 3]);
				if (size > 2) {
					if (table[i + 4] > 0)
						mdelay(table[i + 4]);
				}
			} else if (type == LCD_EXT_CMD_TYPE_DELAY) {
				delay_ms = 0;
				for (j = 0; j < size; j++)
					delay_ms += table[i + 2 + j];
				if (delay_ms > 0)
					mdelay(delay_ms);
			} else if ((type == LCD_EXT_CMD_TYPE_CMD) ||
				   (type == LCD_EXT_CMD_TYPE_CMD_BIN)) {
				ret = lcd_extern_spi_write(edrv, edev,
							   &table[i + 2], size);
			} else if (type == LCD_EXT_CMD_TYPE_CMD_DELAY) {
				ret = lcd_extern_spi_write(edrv, edev,
							   &table[i + 2],
							   (size - 1));
				if (table[i + 1 + size] > 0)
					mdelay(table[i + 1 + size]);
			} else {
				EXTERR("%s: %s(%d): type 0x%02x invalid\n",
				       __func__, edev->config.name,
				       edev->config.index, type);
			}
power_cmd_dynamic_spi_next:
			i += (size + 2);
			step++;
		}
		break;
	default:
		EXTERR("%s: %s(%d): extern_type %d is not support\n",
		       __func__, edev->config.name,
		       edev->config.index, edev->config.type);
		break;
	}

	return ret;
}

static int lcd_extern_power_cmd_fixed_size(struct lcd_extern_driver_s *edrv,
					   struct lcd_extern_dev_s *edev,
					   unsigned char *table, int flag)
{
	int i = 0, j, max_len = 0, step = 0;
	unsigned char type, cmd_size;
	int delay_ms, ret = 0;

	cmd_size = edev->config.cmd_size;
	if (cmd_size < 2) {
		EXTERR("%s: invalid cmd_size %d\n", __func__, cmd_size);
		return -1;
	}

	if (flag)
		max_len = edev->config.table_init_on_cnt;
	else
		max_len = edev->config.table_init_off_cnt;

	switch (edev->config.type) {
	case LCD_EXTERN_I2C:
		while ((i + cmd_size) <= max_len) {
			type = table[i];
			if (type == LCD_EXT_CMD_TYPE_END)
				break;
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				EXTPR("%s: step %d: type=0x%02x, cmd_size=%d\n",
					__func__, step, type, cmd_size);
			}
			if (type == LCD_EXT_CMD_TYPE_NONE) {
				/* do nothing */
			} else if (type == LCD_EXT_CMD_TYPE_GPIO) {
				lcd_ext_gpio_set(edrv, table[i+1], table[i+2]);
				if (cmd_size > 3) {
					if (table[i+3] > 0)
						mdelay(table[i+3]);
				}
			} else if (type == LCD_EXT_CMD_TYPE_DELAY) {
				delay_ms = 0;
				for (j = 0; j < (cmd_size - 1); j++)
					delay_ms += table[i+1+j];
				if (delay_ms > 0)
					mdelay(delay_ms);
			} else if ((type == LCD_EXT_CMD_TYPE_CMD) ||
				   (type == LCD_EXT_CMD_TYPE_CMD_BIN) ||
				   (type == LCD_EXT_CMD_TYPE_CMD_BIN_DATA)) {
				ret = aml_lcd_i2c_write(edrv->i2c_bus,
					edev->config.i2c_addr,
					&table[i+1], (cmd_size-1));
			} else if ((type == LCD_EXT_CMD_TYPE_CMD2) ||
				   (type == LCD_EXT_CMD_TYPE_CMD2_BIN) ||
				   (type == LCD_EXT_CMD_TYPE_CMD2_BIN_DATA)) {
				ret = aml_lcd_i2c_write(edrv->i2c_bus,
					edev->config.i2c_addr2,
					&table[i + 1], (cmd_size - 1));
			} else if ((type == LCD_EXT_CMD_TYPE_CMD3) ||
				   (type == LCD_EXT_CMD_TYPE_CMD3_BIN) ||
				   (type == LCD_EXT_CMD_TYPE_CMD3_BIN_DATA)) {
				ret = aml_lcd_i2c_write(edrv->i2c_bus,
					edev->config.i2c_addr3,
					&table[i + 1], (cmd_size - 1));
			} else if ((type == LCD_EXT_CMD_TYPE_CMD4) ||
				   (type == LCD_EXT_CMD_TYPE_CMD4_BIN) ||
				   (type == LCD_EXT_CMD_TYPE_CMD4_BIN_DATA)) {
				ret = aml_lcd_i2c_write(edrv->i2c_bus,
					edev->config.i2c_addr4,
					&table[i+1], (cmd_size-1));
			} else if (type == LCD_EXT_CMD_TYPE_CMD_DELAY) {
				ret = aml_lcd_i2c_write(edrv->i2c_bus,
					edev->config.i2c_addr,
					&table[i+1], (cmd_size-2));
				if (table[i+cmd_size-1] > 0)
					mdelay(table[i+cmd_size-1]);
			} else if (type == LCD_EXT_CMD_TYPE_CMD2_DELAY) {
				ret = aml_lcd_i2c_write(edrv->i2c_bus,
					edev->config.i2c_addr2,
					&table[i + 1], (cmd_size - 2));
				if (table[i + cmd_size - 1] > 0)
					mdelay(table[i + cmd_size - 1]);
			} else if (type == LCD_EXT_CMD_TYPE_CMD3_DELAY) {
				ret = aml_lcd_i2c_write(edrv->i2c_bus,
					edev->config.i2c_addr3,
					&table[i + 1], (cmd_size - 2));
				if (table[i + cmd_size - 1] > 0)
					mdelay(table[i + cmd_size - 1]);
			} else if (type == LCD_EXT_CMD_TYPE_CMD4_DELAY) {
				ret = aml_lcd_i2c_write(edrv->i2c_bus,
					edev->config.i2c_addr4,
					&table[i + 1], (cmd_size - 2));
				if (table[i + cmd_size - 1] > 0)
					mdelay(table[i + cmd_size - 1]);
			} else {
				EXTERR("%s: %s(%d): type 0x%02x invalid\n",
					__func__, edev->config.name,
					edev->config.index, type);
			}
			i += cmd_size;
			step++;
		}
		break;
	case LCD_EXTERN_SPI:
		while ((i + cmd_size) <= max_len) {
			type = table[i];
			if (type == LCD_EXT_CMD_TYPE_END)
				break;
			if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
				EXTPR("%s: step %d: type=0x%02x, cmd_size=%d\n",
					__func__, step, type, cmd_size);
			}
			if (type == LCD_EXT_CMD_TYPE_NONE) {
				/* do nothing */
			} else if (type == LCD_EXT_CMD_TYPE_GPIO) {
				lcd_ext_gpio_set(edrv, table[i+1], table[i+2]);
				if (cmd_size > 3) {
					if (table[i+3] > 0)
						mdelay(table[i+3]);
				}
			} else if (type == LCD_EXT_CMD_TYPE_DELAY) {
				delay_ms = 0;
				for (j = 0; j < (cmd_size - 1); j++)
					delay_ms += table[i+1+j];
				if (delay_ms > 0)
					mdelay(delay_ms);
			} else if ((type == LCD_EXT_CMD_TYPE_CMD) ||
				   (type == LCD_EXT_CMD_TYPE_CMD_BIN)) {
				ret = lcd_extern_spi_write(edrv, edev,
							   &table[i+1], (cmd_size-1));
			} else if (type == LCD_EXT_CMD_TYPE_CMD_DELAY) {
				ret = lcd_extern_spi_write(edrv, edev,
							   &table[i+1], (cmd_size-2));
				if (table[i+cmd_size-1] > 0)
					mdelay(table[i+cmd_size-1]);
			} else {
				EXTERR("%s: %s(%d): type 0x%02x invalid\n",
					__func__, edev->config.name,
					edev->config.index, type);
			}
			i += cmd_size;
			step++;
		}
		break;
	default:
		EXTERR("%s: %s(%d): extern_type %d is not support\n",
			__func__, edev->config.name,
			edev->config.index, edev->config.type);
		break;
	}

	return ret;
}

static int lcd_extern_power_ctrl(struct lcd_extern_driver_s *edrv,
				 struct lcd_extern_dev_s *edev, int flag)
{
	unsigned char *table;
	int cmd_size;
	int ret = 0;

	cmd_size = edev->config.cmd_size;
	if (cmd_size <= 1) {
		EXTERR("[%d]: %s: %s(%d) cmd_size %d is invalid\n",
		       edrv->index, __func__,
		       edev->config.name, edev->dev_index, cmd_size);
		return -1;
	}

	if (edev->config.type == LCD_EXTERN_SPI)
		spi_gpio_init(edrv, edev);

	/* step 2: power cmd */
	if (flag)
		table = edev->config.table_init_on;
	else
		table = edev->config.table_init_off;
	if (!table) {
		EXTERR("[%d]: %s: %s(%d) table_init_%s is null\n",
		       edrv->index, __func__,
		       edev->config.name, edev->dev_index,
		       (flag) ? "on" : "off");
		ret = -1;
		goto ext_power_ctrl_next;
	}
	if (cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC)
		ret = lcd_extern_power_cmd_dynamic_size(edrv, edev, table, flag);
	else
		ret = lcd_extern_power_cmd_fixed_size(edrv, edev, table, flag);

ext_power_ctrl_next:
	/* step 3: power finish */
	if (edev->config.type == LCD_EXTERN_SPI)
		spi_gpio_off(edrv, edev);

	if (ret) {
		EXTERR("[%d]: %s: %s(%d): %d\n",
		       edrv->index, __func__,
		       edev->config.name, edev->dev_index, flag);
	} else {
		EXTPR("[%d]: %s: %s(%d): %d\n",
		      edrv->index, __func__,
		      edev->config.name, edev->dev_index, flag);
	}
	return ret;
}

static int lcd_extern_power_on(struct lcd_extern_driver_s *edrv,
			       struct lcd_extern_dev_s *edev)
{
	int ret;

	lcd_extern_pinmux_set(edrv, 1);
	ret = lcd_extern_power_ctrl(edrv, edev, 1);
	return ret;
}

static int lcd_extern_power_off(struct lcd_extern_driver_s *edrv,
				struct lcd_extern_dev_s *edev)
{
	int ret;

	ret = lcd_extern_power_ctrl(edrv, edev, 0);
	lcd_extern_pinmux_set(edrv, 0);

	return ret;
}

static int lcd_extern_driver_update(struct lcd_extern_driver_s *edrv, struct lcd_extern_dev_s *edev)
{
	if (edev->config.table_init_loaded == 0) {
		EXTERR("%s(%d): tablet_init is invalid\n",
		       edev->config.name, edev->dev_index);
		return -1;
	}

	if (edev->config.type == LCD_EXTERN_SPI)
		edev->config.spi_delay_us = 1000 / edev->config.spi_clk_freq;

	edev->reg_read  = lcd_extern_reg_read;
	edev->reg_write = lcd_extern_reg_write;
	edev->power_on  = lcd_extern_power_on;
	edev->power_off = lcd_extern_power_off;

	return 0;
}

int lcd_extern_default_probe(struct lcd_extern_driver_s *edrv, struct lcd_extern_dev_s *edev)
{
	int ret = 0;

	if (!edrv) {
		EXTERR("%s: %s ext_drv is null\n", __func__, EXT_DEFAULT_NAME);
		return -1;
	}
	if (!edev) {
		EXTERR("[%d]: %s: %s ext_dev is null\n", edrv->index, __func__, EXT_DEFAULT_NAME);
		return -1;
	}

	if (edev->config.cmd_size < 2) {
		EXTERR("[%d]: %s: %s(%d): cmd_size %d is invalid\n",
			edrv->index, __func__,
			edev->config.name,
			edev->dev_index,
			edev->config.cmd_size);
		return -1;
	}

	ret = lcd_extern_driver_update(edrv, edev);

	if (lcd_debug_print_flag & LCD_DBG_PR_NORMAL) {
		EXTPR("[%d]: %s: %d %s\n",
		      edrv->index, __func__, edev->dev_index,
		      (ret == 0) ? "ok" : "fail");
	}
	return ret;
}

