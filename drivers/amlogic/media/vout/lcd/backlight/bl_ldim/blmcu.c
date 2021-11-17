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
#include <amlogic/media/vout/lcd/bl_ldim.h>
#include "../../lcd_common.h"
#include "ldim_drv.h"
#include "ldim_dev_drv.h"

#define NORMAL_MSG      (0 << 7)
#define BROADCAST_MSG   BIT(7)
#define BLOCK_DATA      (0 << 6)
#define SINGLE_DATA     BIT(6)
#define BLMCU_CLASS_NAME "blmcu"

#define VSYNC_INFO_FREQUENT        300

struct blmcu_s {
	unsigned int dev_on_flag;
	unsigned short vsync_cnt;
	unsigned int rbuf_size;
	unsigned int tbuf_size;

	/* local dimming driver smr api usage */
	unsigned char *rbuf;

	/* spi api internal used, don't use outside!!! */
	unsigned char *tbuf;
};

struct blmcu_s *bl_mcu;

static int blmcu_hw_init_on(struct ldim_dev_driver_s *dev_drv)
{
	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return -1;
	}

	LDIMPR("%s\n", __func__);

	/* step 1: system power_on */
	ldim_gpio_set(dev_drv, dev_drv->en_gpio, dev_drv->en_gpio_on);

	/* step 2: delay for internal logic stable */
	mdelay(10);

	/* step 3: Generate external VSYNC to VSYNC/PWM pin */
	ldim_set_duty_pwm(&dev_drv->ldim_pwm_config);
	ldim_set_duty_pwm(&dev_drv->analog_pwm_config);
	dev_drv->pinmux_ctrl(dev_drv, 1);

	return 0;
}

static int blmcu_hw_init_off(struct ldim_dev_driver_s *dev_drv)
{
	ldim_gpio_set(dev_drv, dev_drv->en_gpio, dev_drv->en_gpio_off);
	dev_drv->pinmux_ctrl(dev_drv, 0);
	ldim_pwm_off(&dev_drv->ldim_pwm_config);
	ldim_pwm_off(&dev_drv->analog_pwm_config);

	return 0;
}

static inline void ldim_data_mapping(unsigned short *duty_buf,
				     unsigned int max, unsigned int min,
				     unsigned int zone_num)
{
	unsigned int i, j, val, apl, k;

	j = 0;
	apl = 0;
	for (i = 0; i < zone_num; i++) {
		apl += duty_buf[i];
		val = min + ((duty_buf[i] * (max - min)) / LD_DATA_MAX);
		if (i % 2 == 0) {
			bl_mcu->rbuf[j] = (val >> 4) & 0xff;
			bl_mcu->rbuf[j + 1] = ((val & 0xf) << 4) & 0xff;
		} else {
			bl_mcu->rbuf[j + 1] |= (val >> 8) & 0xf;
			bl_mcu->rbuf[j + 2] = val & 0xff;
			j += 3;
		}
	}

	k = (3 * zone_num + 1) / 2;

	for (i = 0; i < k; i++)
		bl_mcu->tbuf[i + 4] = bl_mcu->rbuf[i];

	bl_mcu->tbuf[0] = 0x55;
	bl_mcu->tbuf[1] = 0xaa;
	bl_mcu->tbuf[2] = 0x00;
	bl_mcu->tbuf[3] = 0x14;
	bl_mcu->tbuf[k + 4] = 0xa0;  //PDIM
	bl_mcu->tbuf[k + 5] = 0xa0;  //ADIM
	bl_mcu->tbuf[k + 6] = ((apl / zone_num) >> 4) & 0xff;  //apl

	bl_mcu->tbuf_size = k + 7;
}

static int blmcu_smr(struct aml_ldim_driver_s *ldim_drv, unsigned short *buf,
		      unsigned char len)
{
	struct ldim_dev_driver_s *dev_drv = ldim_drv->dev_drv;
	int ret = 0;

	if (!bl_mcu)
		return -1;

	if (bl_mcu->vsync_cnt++ >= VSYNC_INFO_FREQUENT)
		bl_mcu->vsync_cnt = 0;

	if (bl_mcu->dev_on_flag == 0) {
		if (bl_mcu->vsync_cnt == 0)
			LDIMPR("%s: on_flag=%d\n", __func__, bl_mcu->dev_on_flag);
		return 0;
	}
	if (len != dev_drv->zone_num) {
		if (bl_mcu->vsync_cnt == 0)
			LDIMERR("%s: data len %d invalid\n", __func__, len);
		return -1;
	}
	if (!bl_mcu->rbuf) {
		if (bl_mcu->vsync_cnt == 0)
			LDIMERR("%s: rbuf is null\n", __func__);
		return -1;
	}
	if (!bl_mcu->tbuf) {
		if (bl_mcu->vsync_cnt == 0)
			LDIMERR("%s: tbuf is null\n", __func__);
		return -1;
	}

	ldim_data_mapping(buf, dev_drv->dim_max, dev_drv->dim_min,
				  dev_drv->zone_num);

	ret = ldim_spi_write(dev_drv->spi_info.spi, bl_mcu->tbuf, bl_mcu->tbuf_size);

	return ret;
}

static int blmcu_power_on(struct aml_ldim_driver_s *ldim_drv)
{
	if (!bl_mcu)
		return -1;

	if (bl_mcu->dev_on_flag) {
		LDIMPR("%s: blmcu is already on, exit\n", __func__);
		return 0;
	}

	blmcu_hw_init_on(ldim_drv->dev_drv);
	bl_mcu->dev_on_flag = 1;
	bl_mcu->vsync_cnt = 0;

	LDIMPR("%s: ok\n", __func__);
	return 0;
}

static int blmcu_power_off(struct aml_ldim_driver_s *ldim_drv)
{
	if (!bl_mcu)
		return -1;

	bl_mcu->dev_on_flag = 0;
	blmcu_hw_init_off(ldim_drv->dev_drv);

	LDIMPR("%s: ok\n", __func__);
	return 0;
}

static int blmcu_ldim_dev_update(struct ldim_dev_driver_s *dev_drv)
{
	dev_drv->power_on = blmcu_power_on;
	dev_drv->power_off = blmcu_power_off;
	dev_drv->dev_smr = blmcu_smr;

	dev_drv->reg_write = NULL;
	dev_drv->reg_read = NULL;
	return 0;
}

int ldim_dev_blmcu_probe(struct aml_ldim_driver_s *ldim_drv)
{
	struct ldim_dev_driver_s *dev_drv = ldim_drv->dev_drv;

	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return -1;
	}

	bl_mcu = (struct blmcu_s *)malloc(sizeof(struct blmcu_s));
	if (!bl_mcu) {
		LDIMERR("blmcu malloc error\n");
		return -1;
	}
	memset(bl_mcu, 0, sizeof(struct blmcu_s));

	bl_mcu->dev_on_flag = 0;
	bl_mcu->vsync_cnt = 0;

	/* each zone 2 bytes */
	bl_mcu->rbuf_size = 2 * dev_drv->zone_num;
	bl_mcu->rbuf = (unsigned char *)malloc(bl_mcu->rbuf_size);
	if (!bl_mcu->rbuf)
		goto ldim_dev_blmcu_probe_err0;
	memset(bl_mcu->rbuf, 0, bl_mcu->rbuf_size);

	/* header + data + suffix */
	/* according custom backlight mcu spi spec */
	bl_mcu->tbuf_size = 7 + 2 * dev_drv->zone_num;
	bl_mcu->tbuf = (unsigned char *)malloc(bl_mcu->tbuf_size);
	if (!bl_mcu->tbuf)
		goto ldim_dev_blmcu_probe_err1;
	memset(bl_mcu->tbuf, 0, bl_mcu->tbuf_size);

	blmcu_ldim_dev_update(dev_drv);

	LDIMPR("%s ok\n", __func__);
	return 0;

ldim_dev_blmcu_probe_err1:
	free(bl_mcu->rbuf);
	bl_mcu->rbuf_size = 0;
ldim_dev_blmcu_probe_err0:
	free(bl_mcu);
	bl_mcu = NULL;
	return -1;
}

int ldim_dev_blmcu_remove(struct aml_ldim_driver_s *ldim_drv)
{
	if (!bl_mcu)
		return 0;

	free(bl_mcu->rbuf);
	bl_mcu->rbuf_size = 0;
	free(bl_mcu->tbuf);
	bl_mcu->tbuf_size = 0;
	free(bl_mcu);
	bl_mcu = NULL;

	return 0;
}
