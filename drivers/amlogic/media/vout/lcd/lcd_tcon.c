/*
 * drivers/amlogic/media/vout/lcd/aml_lcd_tcon.c
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
#include <asm/arch/io.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include "lcd_reg.h"
#include "lcd_common.h"
#include "lcd_tcon.h"

static struct lcd_tcon_data_s *lcd_tcon_data;

static int lcd_tcon_valid_check(void)
{
	if (lcd_tcon_data == NULL) {
		LCDERR("invalid tcon data\n");
		return -1;
	}
	if (lcd_tcon_data->tcon_valid == 0) {
		LCDERR("invalid tcon\n");
		return -1;
	}

	return 0;
}

static void lcd_tcon_config_axi_offset_default(void)
{
	char *str;

	str = env_get("tcon_fb_addr");
	if (str) {
		lcd_tcon_data->axi_offset_addr = simple_strtoul(str, NULL, 16);
	} else {
		LCDERR("can't find env tcon_fb_addr\n");
		lcd_tcon_data->axi_offset_addr = 0;
	}
}

static int lcd_tcon_config(const void *dt_blob, struct lcd_config_s *pconf, int load_id)
{
	int key_len, reg_len;
	int parent_offset;
	char *propdata;
	int ret;

	if (load_id & 0x1) {
		parent_offset = fdt_path_offset(dt_blob, "/lcd");
		if (parent_offset < 0) {
			LCDERR("can't find /lcd node: %s\n", fdt_strerror(parent_offset));
			lcd_tcon_config_axi_offset_default();
		} else {
			propdata = (char *)fdt_getprop(dt_blob,
				parent_offset, "tcon_fb_addr", NULL);
			if (propdata == NULL) {
				LCDERR("failed to get tcon_fb_addr from dts\n");
				lcd_tcon_config_axi_offset_default();
			} else {
				lcd_tcon_data->axi_offset_addr =
					be32_to_cpup(((u32*)propdata));
			}
		}
	} else {
		lcd_tcon_config_axi_offset_default();
	}

	/* get reg table from unifykey */
	reg_len = lcd_tcon_data->reg_table_len;
	if (lcd_tcon_data->reg_table == NULL) {
		lcd_tcon_data->reg_table =
			(unsigned char *)malloc(sizeof(unsigned char) * reg_len);
		if (!lcd_tcon_data->reg_table) {
			LCDERR("%s: Not enough memory\n", __func__);
			return -1;
		}
	}
	memset(lcd_tcon_data->reg_table, 0, (sizeof(unsigned char) * reg_len));
	key_len = reg_len;
	ret = aml_lcd_unifykey_get_no_header("lcd_tcon",
		lcd_tcon_data->reg_table, &key_len);
	if (ret) {
		free(lcd_tcon_data->reg_table);
		lcd_tcon_data->reg_table = NULL;
		LCDERR("%s: !!!!!!!!tcon unifykey load error!!!!!!!!\n", __func__);
		return -1;
	}
	if (key_len != reg_len) {
		free(lcd_tcon_data->reg_table);
		lcd_tcon_data->reg_table = NULL;
		LCDERR("%s: !!!!!!!!tcon unifykey load length error!!!!!!!!\n", __func__);
		return -1;
	}

	LCDPR("tcon: load key len: %d\n", key_len);
	return 0;
}

/* **********************************
 * tcon function api
 * **********************************
 */
#define PR_BUF_MAX    200
void lcd_tcon_reg_table_print(void)
{
	int i, j, n, cnt;
	char *buf;
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return;

	if (lcd_tcon_data->reg_table == NULL) {
		LCDERR("%s: reg_table is null\n", __func__);
		return;
	}

	buf = (char *)malloc(PR_BUF_MAX * sizeof(char));
	if (buf == NULL) {
		LCDERR("%s: buf malloc error\n", __func__);
		return;
	}

	LCDPR("%s:\n", __func__);
	cnt = lcd_tcon_data->reg_table_len;
	for (i = 0; i < cnt; i += 16) {
		n = snprintf(buf, PR_BUF_MAX, "0x%04x: ", i);
		for (j = 0; j < 16; j++) {
			if ((i + j) >= cnt)
				break;
			n += snprintf(buf+n, PR_BUF_MAX, " 0x%02x",
				lcd_tcon_data->reg_table[i+j]);
		}
		buf[n] = '\0';
		printf("%s\n", buf);
	}
	free(buf);
}

void lcd_tcon_reg_readback_print(void)
{
	int i, j, n, cnt;
	char *buf;
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return;

	buf = (char *)malloc(PR_BUF_MAX * sizeof(char));
	if (buf == NULL) {
		LCDERR("%s: buf malloc error\n", __func__);
		return;
	}

	LCDPR("%s:\n", __func__);
	cnt = lcd_tcon_data->reg_table_len;
	for (i = 0; i < cnt; i += 16) {
		n = snprintf(buf, PR_BUF_MAX, "0x%04x: ", i);
		for (j = 0; j < 16; j++) {
			if ((i + j) >= cnt)
				break;
			if (lcd_tcon_data->core_reg_width == 8) {
				n += snprintf(buf+n, PR_BUF_MAX, " 0x%02x",
					lcd_tcon_read_byte(i+j));
			} else {
				n += snprintf(buf+n, PR_BUF_MAX, " 0x%02x",
					lcd_tcon_read(i+j));
			}
		}
		buf[n] = '\0';
		printf("%s\n", buf);
	}
	free(buf);
}

void lcd_tcon_info_print(void)
{
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return;

	LCDPR("%s:\n", __func__);
	printf("core_reg_width:    %d\n", lcd_tcon_data->core_reg_width);
	printf("reg_table_len:     %d\n", lcd_tcon_data->reg_table_len);
	printf("axi_offset_addr:   0x%08x\n", lcd_tcon_data->axi_offset_addr);
}

int lcd_tcon_enable(struct lcd_config_s *pconf)
{
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return -1;

	if (lcd_tcon_data->tcon_enable)
		lcd_tcon_data->tcon_enable(pconf);

	return 0;
}

void lcd_tcon_disable(void)
{
	unsigned int reg, i, cnt, offset, bit;
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return;

	LCDPR("%s\n", __func__);
	/* disable over_drive */
	if (lcd_tcon_data->reg_core_od != REG_LCD_TCON_MAX) {
		reg = lcd_tcon_data->reg_core_od + TCON_CORE_REG_START;
		if (lcd_tcon_data->core_reg_width == 8)
			lcd_tcon_write_byte(reg, 0);
		else
			lcd_tcon_write(reg, 0);
		mdelay(100);
	}

	/* disable all ctrl signal */
	if (lcd_tcon_data->reg_core_ctrl_timing_base != REG_LCD_TCON_MAX) {
		reg = lcd_tcon_data->reg_core_ctrl_timing_base + TCON_CORE_REG_START;
		offset = lcd_tcon_data->ctrl_timing_offset;
		cnt = lcd_tcon_data->ctrl_timing_cnt;
		for (i = 0; i < cnt; i++) {
			if (lcd_tcon_data->core_reg_width == 8)
				lcd_tcon_setb_byte((reg + (i * offset)), 1, 3, 1);
			else
				lcd_tcon_setb((reg + (i * offset)), 1, 3, 1);
		}
	}

	/* disable top */
	if (lcd_tcon_data->reg_top_ctrl != REG_LCD_TCON_MAX) {
		reg = lcd_tcon_data->reg_top_ctrl;
		bit = lcd_tcon_data->bit_en;
		lcd_tcon_setb(reg, 0, bit, 1);
	}
}

/* **********************************
 * tcon match data
 * **********************************
 */
int lcd_tcon_probe(const void *dt_blob, struct aml_lcd_drv_s *lcd_drv, int load_id)
{
	int ret = 0;
	struct lcd_config_s *pconf = lcd_drv->lcd_config;

	LCDPR("%s\n", __func__);

	switch (lcd_drv->chip_type) {
	default:
		lcd_tcon_data = NULL;
		break;
	}
	ret = lcd_tcon_valid_check();
	if (ret)
		return -1;

	ret = lcd_tcon_config(dt_blob, pconf, load_id);

	return ret;
}

