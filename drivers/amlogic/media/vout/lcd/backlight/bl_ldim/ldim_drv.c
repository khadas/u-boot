// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/gpio.h>
#include <fdtdec.h>
#include <amlogic/media/vout/lcd/aml_lcd.h>
#include <amlogic/media/vout/lcd/bl_ldim.h>
#include "../../lcd_common.h"
#include "ldim_drv.h"

static struct aml_ldim_driver_s *ldim_driver;

struct aml_ldim_driver_s *aml_ldim_get_driver(void)
{
	return ldim_driver;
}

static void ldim_brightness_update(struct aml_ldim_driver_s *ldim_drv, unsigned int level)
{
	struct ldim_dev_driver_s *dev_drv = ldim_drv->dev_drv;
	unsigned int size;
	unsigned int i;

	size = ldim_drv->config.row * ldim_drv->config.col;
	for (i = 0; i < size; i++)
		ldim_drv->bl_matrix[i] = (unsigned short)level;

	if (dev_drv->dev_smr)
		dev_drv->dev_smr(ldim_drv, ldim_drv->bl_matrix, size);
	else
		LDIMPR("%s: dev_smr is null\n", __func__);
}

static int ldim_set_level(struct aml_ldim_driver_s *ldim_drv, unsigned int level)
{
	struct aml_bl_drv_s *bdrv = aml_bl_get_driver(0);
	unsigned int max, min;
	int ret = 0;

	if (!bdrv)
		return -1;

	ldim_drv->brightness_level = level;
	if (ldim_drv->ldim_on_flag == 0)
		return 0;

	max = bdrv->config.level_max;
	min = bdrv->config.level_min;

	level = ((level - min) * (LD_DATA_MAX - LD_DATA_MIN)) / (max - min) + LD_DATA_MIN;
	level &= 0xfff;

	ldim_brightness_update(ldim_drv, level);
	return ret;
}

static int ldim_power_on(struct aml_ldim_driver_s *ldim_drv)
{
	struct ldim_dev_driver_s *dev_drv = ldim_drv->dev_drv;

	if (dev_drv && dev_drv->power_on)
		dev_drv->power_on(ldim_drv);
	else
		LDIMERR("%s: device power_on is null\n", __func__);
	ldim_drv->ldim_on_flag = 1;

	if (ldim_drv->brightness_level > 0)
		ldim_set_level(ldim_drv, ldim_drv->brightness_level);

	return 0;
}
static int ldim_power_off(struct aml_ldim_driver_s *ldim_drv)
{
	struct ldim_dev_driver_s *dev_drv = ldim_drv->dev_drv;

	ldim_drv->ldim_on_flag = 0;
	if (dev_drv && dev_drv->power_off)
		dev_drv->power_off(ldim_drv);
	else
		LDIMERR("%s: device power_off is null\n", __func__);

	return 0;
}

static void ldim_config_print(struct aml_ldim_driver_s *ldim_drv)
{
	struct ldim_dev_driver_s *dev_drv = ldim_drv->dev_drv;

	LDIMPR("%s:\n", __func__);
	printf("valid_flag            = %d\n"
		"ldim_on_flag          = %d\n"
		"bl_row                = %d\n"
		"bl_col                = %d\n"
		"dev_index             = %d\n",
		ldim_drv->valid_flag,
		ldim_drv->ldim_on_flag,
		ldim_drv->config.row,
		ldim_drv->config.col,
		ldim_drv->config.dev_index);
	if (dev_drv && dev_drv->config_print)
		dev_drv->config_print(dev_drv);
}

#ifdef CONFIG_OF_LIBFDT
static int ldim_config_load_from_dts(char *dt_addr, int child_offset,
				     struct aml_ldim_driver_s *ldim_drv)
{
	char *propdata;

	if (!dt_addr) {
		LDIMERR("dt_addr is null\n");
		return -1;
	}

	if (child_offset < 0) {
		LDIMERR("not find backlight node %s\n", fdt_strerror(child_offset));
		return -1;
	}

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "bl_ldim_zone_row_col", NULL);
	if (!propdata) {
		propdata = (char *)fdt_getprop(dt_addr, child_offset, "bl_ldim_region_row_col", NULL);
		if (!propdata) {
			LDIMERR("failed to get bl_ldim_zone_row_col\n");
			ldim_drv->config.row = 1;
			ldim_drv->config.col = 1;
		} else {
			ldim_drv->config.row = be32_to_cpup((u32*)propdata);
			ldim_drv->config.col = be32_to_cpup((((u32*)propdata)+1));
		}
	} else {
		ldim_drv->config.row = be32_to_cpup((u32*)propdata);
		ldim_drv->config.col = be32_to_cpup((((u32*)propdata)+1));
	}
	LDIMPR("get bl_zone row = %d, col = %d\n", ldim_drv->config.row, ldim_drv->config.col);

	propdata = (char *)fdt_getprop(dt_addr, child_offset, "ldim_dev_index", NULL);
	if (!propdata) {
		LDIMERR("failed to get ldim_dev_index\n");
		ldim_drv->config.dev_index = 0xff;
	} else {
		ldim_drv->config.dev_index = be32_to_cpup((u32 *)propdata);
	}
	LDIMPR("get dev_index = %d\n", ldim_drv->config.dev_index);

	return 0;
}
#endif

static int ldim_config_load_from_unifykey(unsigned char *para, struct aml_ldim_driver_s *ldim_drv)
{
	unsigned char *p;

	if (!para) {
		LDIMERR("backlight unifykey buf is NULL\n");
		return -1;
	}

	p = para;

	/* ldim: 24byte */
	/* get bl_ldim_region_row_col 4byte*/
	ldim_drv->config.row = *(p + LCD_UKEY_BL_LDIM_ROW);
	ldim_drv->config.col = *(p + LCD_UKEY_BL_LDIM_COL);
	LDIMPR("get bl_zone row = %d, col = %d\n",
	       ldim_drv->config.row, ldim_drv->config.col);

	/* get ldim_dev_index 1byte*/
	ldim_drv->config.dev_index = *(p + LCD_UKEY_BL_LDIM_DEV_INDEX);
	LDIMPR("get dev_index = %d\n", ldim_drv->config.dev_index);

	return 0;
}

int aml_ldim_probe(char *dt_addr, int child_offset, unsigned char *key_buf, int flag)
{
	struct aml_lcd_data_s *pdata = aml_lcd_get_data();
	struct aml_bl_drv_s *bdrv = aml_bl_get_driver(0);
	unsigned int size;
	int ret = -1;

	if (!bdrv)
		return -1;

	ldim_driver = (struct aml_ldim_driver_s *)malloc(sizeof(struct aml_ldim_driver_s));
	if (!ldim_driver) {
		LDIMERR("ldim_driver malloc error\n");
		return -1;
	}
	memset(ldim_driver, 0, sizeof(struct aml_ldim_driver_s));

	ldim_driver->data = pdata;

	switch (flag) {
	case 0: /* dts */
#ifdef CONFIG_OF_LIBFDT
		ret = ldim_config_load_from_dts(dt_addr, child_offset, ldim_driver);
		if (ret)
			break;
#endif
		break;
	case 2: /* unifykey */
		ret = ldim_config_load_from_unifykey(key_buf, ldim_driver);
		if (ret)
			break;
		break;
	case 1: /* bsp */
		LDIMPR("%s: not support bsp config\n", __func__);
		break;
	default:
		break;
	}
	if (ret) {
		LDIMERR("%s failed\n", __func__);
		return -1;
	}

	ret = aml_ldim_device_probe(dt_addr, ldim_driver);
	if (ret)
		return -1;

	size = ldim_driver->config.row * ldim_driver->config.col;
	ldim_driver->bl_matrix = (unsigned short *)malloc(sizeof(unsigned short) * size);
	if (!ldim_driver->bl_matrix) {
		LDIMERR("bl_matrix malloc error\n");
		return -1;
	}
	memset(ldim_driver->bl_matrix, 0, sizeof(unsigned short) * size);

	ldim_driver->power_on = ldim_power_on;
	ldim_driver->power_off = ldim_power_off;
	ldim_driver->set_level = ldim_set_level;
	ldim_driver->config_print = ldim_config_print;

	ldim_driver->valid_flag = 1;

	LDIMPR("%s is ok\n", __func__);

	return ret;
}

