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

#define NORMAL_MSG            (0 << 7)
#define BROADCAST_MSG         BIT(7)
#define BLOCK_DATA            (0 << 6)
#define SINGLE_DATA           BIT(6)

#define IW7027_REG_MAX        0x100

struct iw7027_s {
	unsigned int dev_on_flag;

	unsigned int reg_buf_size;
	unsigned int tbuf_size;
	unsigned int rbuf_size;

	unsigned char *reg_buf; /* local dimming driver smr api usage */

	/* spi api internal used, don't use outside!!! */
	unsigned char *tbuf;
	unsigned char *rbuf;
};

static struct iw7027_s *bl_iw7027;

/* write single device */
static int spi_wregs(struct spi_slave *spi, unsigned char chip_id, unsigned int chip_cnt,
		       unsigned char reg, unsigned char *data_buf, unsigned int tlen)
{
	unsigned char *tbuf;
	int n, xlen, ret;

	if (!bl_iw7027 || !bl_iw7027->tbuf) {
		LDIMERR("%s: bl_iw7027 or tbuf is null\n", __func__);
		return -1;
	}
	tbuf = bl_iw7027->tbuf;

	if (chip_id == 0x00 || chip_id == 0x3f) {
		LDIMERR("%s: chip_id 0x%02x is invalid\n", __func__, chip_id);
		return -1;
	}
	if (tlen == 0) {
		LDIMERR("%s: tlen is 0\n", __func__);
		return -1;
	}
	n = chip_id - 1;
	if (tlen == 1)
		xlen = 2 + 1 + n;
	else
		xlen = 3 + tlen + n;
	if (bl_iw7027->tbuf_size < xlen) {
		LDIMERR("%s: tbuf_size %d is not enough\n", __func__, bl_iw7027->tbuf_size);
		return -1;
	}

	if (tlen == 1) {
		tbuf[0] = NORMAL_MSG | SINGLE_DATA | (chip_id & 0x3f);
		tbuf[1] = reg & 0x7f;
		tbuf[2] = data_buf[0];
		memset(&tbuf[3], 0, n);
	} else {
		tbuf[0] = NORMAL_MSG | BLOCK_DATA | (chip_id & 0x3f);
		tbuf[1] = tlen;
		tbuf[2] = reg & 0x7f;
		memcpy(&tbuf[3], data_buf, tlen);
		memset(&tbuf[3 + tlen], 0, n);
	}
	ret = ldim_spi_write(spi, tbuf, xlen);

	return ret;
}

static int spi_high_reg_switch(struct spi_slave *spi, unsigned char chip_id, unsigned int chip_cnt,
			       int flag)
{
	unsigned char temp;
	int ret;

	if (flag)
		temp = 0x80;
	else
		temp = 0x00;
	ret = spi_wregs(spi, chip_id, chip_cnt, 0x78, &temp, 1);
	if (ret)
		return -1;

	return 0;
}

/* read single device */
static int spi_rregs(struct spi_slave *spi, unsigned char chip_id, unsigned int chip_cnt,
		     unsigned char reg, unsigned char *data_buf, unsigned int rlen)
{
	unsigned char *tbuf, *rbuf;
	int n, xlen, ret;
	int i;

	if (!bl_iw7027 || !bl_iw7027->tbuf || !bl_iw7027->rbuf) {
		LDIMERR("%s: bl_iw7027 or tbuf or rbuf is null\n", __func__);
		return -1;
	}
	tbuf = bl_iw7027->tbuf;
	rbuf = bl_iw7027->rbuf;

	if (chip_id == 0x00 || chip_id == 0x3f) {
		LDIMERR("%s: chip_id 0x%02x is invalid\n", __func__, chip_id);
		return -1;
	}
	if (rlen == 0) {
		LDIMERR("%s: rlen is 0\n", __func__);
		return -1;
	}
	n = chip_cnt + rlen;
	if (rlen == 1)
		xlen = 2 + n;
	else
		xlen = 3 + n;
	if (bl_iw7027->tbuf_size < xlen) {
		LDIMERR("%s: tbuf_size %d is not enough\n", __func__, bl_iw7027->tbuf_size);
		return -1;
	}
	if (bl_iw7027->rbuf_size < xlen) {
		LDIMERR("%s: rbuf_size %d is not enough\n", __func__, bl_iw7027->rbuf_size);
		return -1;
	}

	if (reg >= 0x80)
		spi_high_reg_switch(spi, chip_id, chip_cnt, 1);

	if (rlen == 1) {
		tbuf[0] = NORMAL_MSG | SINGLE_DATA | (chip_id & 0x3f);
		tbuf[1] = (reg & 0x7f) | 0x80;
		memset(&tbuf[2], 0, n);
	} else {
		tbuf[0] = NORMAL_MSG | BLOCK_DATA | (chip_id & 0x3f);
		tbuf[1] = rlen;
		tbuf[2] = (reg & 0x7f) | 0x80;
		memset(&tbuf[3], 0, n);
	}
	ret = ldim_spi_read_sync(spi, tbuf, rbuf, xlen);
	memcpy(data_buf, &rbuf[xlen - rlen], rlen);

	if (reg >= 0x80)
		spi_high_reg_switch(spi, chip_id, chip_cnt, 0);

	if (lcd_debug_print_flag & LCD_DBG_PR_BL_ADV) {
		LDIMPR("%s: reg=0x%02x, rlen=%d, data_buf:\n", __func__, reg, rlen);
		for (i = 0; i < rlen; i++)
			printf(" 0x%02x", data_buf[i]);
		printf("\n");
	}

	return ret;
}

/* write same data to all device */
static int spi_wregs_all(struct spi_slave *spi, unsigned int chip_cnt,
			 unsigned char reg, unsigned char *data_buf, int tlen)
{
	unsigned char *tbuf;
	int n, xlen, ret;

	if (!bl_iw7027 || !bl_iw7027->tbuf) {
		LDIMERR("%s: bl_iw7027 or tbuf is null\n", __func__);
		return -1;
	}
	tbuf = bl_iw7027->tbuf;

	if (tlen == 0) {
		LDIMERR("%s: tlen is 0\n", __func__);
		return -1;
	}
	n = chip_cnt - 1;
	if (tlen == 1)
		xlen = 2 + 1 + n;
	else
		xlen = 3 + tlen + n;
	if (bl_iw7027->tbuf_size < xlen) {
		LDIMERR("%s: tbuf_size %d is not enough\n", __func__, bl_iw7027->tbuf_size);
		return -1;
	}

	if (tlen == 1) {
		tbuf[0] = BROADCAST_MSG | SINGLE_DATA | 0x00;
		tbuf[1] = reg & 0x7f;
		tbuf[2] = data_buf[0];
		memset(&tbuf[3], 0, n);
	} else {
		tbuf[0] = BROADCAST_MSG | BLOCK_DATA | 0x00;
		tbuf[1] = tlen;
		tbuf[2] = reg & 0x7f;
		memcpy(&tbuf[3], data_buf, tlen);
		memset(&tbuf[3 + tlen], 0, n);
	}
	ret = ldim_spi_write(spi, tbuf, xlen);

	return ret;
}

/* write diff data to all device */
static int spi_wregs_diff(struct spi_slave *spi, unsigned int chip_cnt,
			  unsigned char reg, unsigned char *data_buf, int tlen)
{
	unsigned char *tbuf;
	int i, n, xlen, ret;

	if (!bl_iw7027 || !bl_iw7027->tbuf) {
		LDIMERR("%s: bl_iw7027 or tbuf is null\n", __func__);
		return -1;
	}
	tbuf = bl_iw7027->tbuf;

	if (tlen == 0) {
		LDIMERR("%s: tlen is 0\n", __func__);
		return -1;
	}
	n = chip_cnt - 1;
	if (tlen == 1)
		xlen = 2 + chip_cnt + n;
	else
		xlen = 3 + tlen * chip_cnt + n;
	if (bl_iw7027->tbuf_size < xlen) {
		LDIMERR("%s: tbuf_size %d is not enough\n", __func__, bl_iw7027->tbuf_size);
		return -1;
	}

	if (tlen == 1) {
		tbuf[0] = BROADCAST_MSG | SINGLE_DATA | 0x3f;
		tbuf[1] = reg & 0x7f;
		for (i = 0; i < chip_cnt; i++)
			tbuf[2 + i] = data_buf[0];
		memset(&tbuf[2 + chip_cnt], 0, n);
	} else {
		tbuf[0] = BROADCAST_MSG | BLOCK_DATA | 0x3f;
		tbuf[1] = tlen;
		tbuf[2] = reg & 0x7f;
		for (i = 0; i < chip_cnt; i++)
			memcpy(&tbuf[3 + i * tlen], &data_buf[i * tlen], tlen);
		memset(&tbuf[3 + tlen * chip_cnt], 0, n);
	}
	ret = ldim_spi_write(spi, tbuf, xlen);

	return ret;
}

static int iw7027_reg_write(struct ldim_dev_driver_s *dev_drv, unsigned char chip_id,
			    unsigned char reg, unsigned char *buf, unsigned int len)
{
	int ret;

	ret = spi_wregs(dev_drv->spi_info.spi, chip_id, dev_drv->chip_cnt, reg, buf, len);
	if (ret)
		LDIMERR("%s: chip_id[%d] reg 0x%x error\n", __func__, chip_id, reg);

	return ret;
}

static int iw7027_reg_read(struct ldim_dev_driver_s *dev_drv, unsigned char chip_id,
			   unsigned char reg, unsigned char *buf, unsigned int len)
{
	int ret;

	memset(buf, 0, len);
	ret = spi_rregs(dev_drv->spi_info.spi, chip_id, dev_drv->chip_cnt, reg, buf, len);
	if (ret)
		LDIMERR("%s: chip_id[%d] reg 0x%x error\n", __func__, chip_id, reg);

	return ret;
}

static int iw7027_reg_write_all(struct ldim_dev_driver_s *dev_drv,
				unsigned char reg, unsigned char *buf, unsigned int len)
{
	int ret;

	ret = spi_wregs_all(dev_drv->spi_info.spi, dev_drv->chip_cnt, reg, buf, len);
	if (ret)
		LDIMERR("%s: reg 0x%x, len %d error\n", __func__, reg, len);

	return ret;
}

static int iw7027_reg_write_diff(struct ldim_dev_driver_s *dev_drv,
				 unsigned char reg, unsigned char *buf, unsigned int len)
{
	int ret;

	ret = spi_wregs_diff(dev_drv->spi_info.spi, dev_drv->chip_cnt, reg, buf, len);
	if (ret)
		LDIMERR("%s: reg 0x%x, len %d error\n", __func__, reg, len);

	return ret;
}

static int ldim_power_cmd_dynamic_size(struct ldim_dev_driver_s *dev_drv)
{
	unsigned char *table;
	int i = 0, j, step = 0, max_len = 0;
	unsigned char type, cmd_size;
	int delay_ms, ret = 0;

	table = dev_drv->init_on;
	max_len = dev_drv->init_on_cnt;

	while ((i + 1) < max_len) {
		type = table[i];
		if (type == LCD_EXT_CMD_TYPE_END)
			break;
		if (lcd_debug_print_flag & LCD_DBG_PR_BL_NORMAL) {
			LDIMPR("%s: step %d: type=0x%02x, cmd_size=%d\n",
				__func__, step, type, table[i + 1]);
		}
		cmd_size = table[i + 1];
		if (cmd_size == 0)
			goto power_cmd_dynamic_next;
		if ((i + 2 + cmd_size) > max_len)
			break;

		if (type == LCD_EXT_CMD_TYPE_NONE) {
			/* do nothing */
		} else if (type == LCD_EXT_CMD_TYPE_DELAY) {
			delay_ms = 0;
			for (j = 0; j < cmd_size; j++)
				delay_ms += table[i + 2 + j];
			if (delay_ms > 0)
				mdelay(delay_ms);
		} else if (type == LCD_EXT_CMD_TYPE_CMD) {
			ret = iw7027_reg_write_all(dev_drv,
				table[i + 2], &table[i + 3], (cmd_size - 1));
			udelay(1);
		} else if (type == LCD_EXT_CMD_TYPE_CMD_DELAY) {
			ret = iw7027_reg_write_all(dev_drv,
				table[i + 2], &table[i + 3], (cmd_size - 2));
			udelay(1);
			if (table[i + 2 + cmd_size - 1] > 0)
				mdelay(table[i + 2 + cmd_size - 1]);
		} else {
			LDIMERR("%s: type 0x%02x invalid\n", __func__, type);
		}
power_cmd_dynamic_next:
		i += (cmd_size + 2);
		step++;
	}

	return ret;
}

static int iw7027_power_on_init(struct ldim_dev_driver_s *dev_drv)
{
	int ret = 0;

	if (dev_drv->cmd_size < 2) {
		LDIMERR("%s: invalid cmd_size %d\n", __func__, dev_drv->cmd_size);
		return -1;
	}
	if (!dev_drv->init_on) {
		LDIMERR("%s: init_on is null\n", __func__);
		return -1;
	}

	if (dev_drv->cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC)
		ret = ldim_power_cmd_dynamic_size(dev_drv);

	return ret;
}

static int iw7027_hw_init_on(struct ldim_dev_driver_s *dev_drv)
{
	unsigned char chip_id, reg, temp[2];
	int i, retry_cnt = 0;

	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return -1;
	}

	LDIMPR("%s\n", __func__);

	chip_id = 0x01;

	/* step 1: system power_on */
	ldim_gpio_set(dev_drv, dev_drv->en_gpio, dev_drv->en_gpio_on);

	/* step 2: Keep Vsync signal at low level */
	/* step 3: delay for internal logic stable */
	mdelay(10);

iw7027_hw_init_on_retry:
	/* SPI communication check */
	for (i = 1; i <= 10; i++) {
		/* step 4: Write 0X00=0X06 */
		reg = 0x00;
		temp[0] = 0x06;
		iw7027_reg_write_all(dev_drv, reg, temp, 1);
		/* step 5: Read if 0x00=0x06,go on to step 6,or step 4 loop until 0x00 = 0x06 */
		iw7027_reg_read(dev_drv, chip_id, reg, temp, 1);
		if (temp[0] == 0x06)
			break;
	}
	if (i == 10)
		LDIMERR("%s: SPI communication check error\n", __func__);

	/* step 6: configure initial registers */
	iw7027_power_on_init(dev_drv);

	/* step 7: Generate external VSYNC to VSYNC/PWM pin */
	ldim_set_duty_pwm(&dev_drv->ldim_pwm_config);
	ldim_set_duty_pwm(&dev_drv->analog_pwm_config);
	dev_drv->pinmux_ctrl(dev_drv, 1);

	/* step 8: delay for system clock and light bar PSU stable */
	mdelay(520);

	/* start calibration */
	/* step 9: set [SOFT_RST_N] = 1 to exit reset state */
	reg = 0x00;
	temp[0] = 0x07;
	iw7027_reg_write_all(dev_drv, reg, temp, 1);
	//mdelay(200);

	/* step 10: Set 0X78[7]=1; no need here */
	/* step 11: Wait until reg0xb3 LSB 4bit is 0101//It means iW7027 enter OTF state */
	i = 0;
	while (i++ < 500) {
		reg = 0xb3;
		iw7027_reg_read(dev_drv, chip_id, reg, temp, 1);
		/*VDAC statue reg :FB1=[0x5] FB2=[0x50]*/
		/*The current platform using FB1*/
		if ((temp[0] & 0xf) == 0x05)
			break;
		mdelay(1);
	}
	/* step 12: Read 0x85,0x86 */
	reg = 0x85;
	iw7027_reg_read(dev_drv, chip_id, reg, temp, 2);

	/* step 13: Set 0X78[7]=0, no need */
	/* step 14: check if power on voltage exist */
	if (temp[0] || temp[1]) {
		LDIMERR("%s: 0x85,0x86 is not zero\n", __func__);
		if (retry_cnt++ < 3) {
			LDIMPR("%s: retry %d\n", __func__, retry_cnt);
			goto iw7027_hw_init_on_retry;
		}
	} else {
		/* step 15: calibration done */
		LDIMPR("%s: calibration done\n", __func__);
	}

	//reg = 0x30;
	//temp[0] = 0x1d;
	//iw7027_reg_write_all(dev_drv, chip_id, reg, temp, 1);
	//reg = 0x29;
	//temp[0] = 0x84;
	//iw7027_reg_write_all(dev_drv, chip_id, reg, temp, 1);

	return 0;
}

static int iw7027_hw_init_off(struct ldim_dev_driver_s *dev_drv)
{
	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return -1;
	}

	ldim_gpio_set(dev_drv, dev_drv->en_gpio, dev_drv->en_gpio_off);
	dev_drv->pinmux_ctrl(dev_drv, 0);
	ldim_pwm_off(&dev_drv->ldim_pwm_config);
	ldim_pwm_off(&dev_drv->analog_pwm_config);

	return 0;
}

static inline void ldim_data_mapping(unsigned short *duty_buf, unsigned int max, unsigned int min,
				     unsigned int zone_num, unsigned short *mapping)
{
	unsigned int i, j, val, zone_max;

	zone_max = bl_iw7027->reg_buf_size / 2;
	for (i = 0; i < zone_num; i++) {
		val = min + ((duty_buf[i] * (max - min)) / LD_DATA_MAX);
		j = mapping[i];
		if (j >= zone_max) {
			LDIMPR("%s: mapping[%d]=%d invalid, max %d\n",
			       __func__, i, j, zone_max);
			return;
		}
		bl_iw7027->reg_buf[2 * j] = (val >> 8) & 0xf;
		bl_iw7027->reg_buf[2 * j + 1] = val & 0xff;
	}
}

static int iw7027_smr(struct aml_ldim_driver_s *ldim_drv, unsigned short *buf, unsigned char len)
{
	struct ldim_dev_driver_s *dev_drv = ldim_drv->dev_drv;

	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return -1;
	}
	if (!bl_iw7027)
		return -1;

	if (bl_iw7027->dev_on_flag == 0) {
		LDIMPR("%s: on_flag=%d\n", __func__, bl_iw7027->dev_on_flag);
		return 0;
	}
	if (len != dev_drv->zone_num) {
		LDIMERR("%s: data len %d invalid\n", __func__, len);
		return -1;
	}
	if (!bl_iw7027->reg_buf) {
		LDIMERR("%s: reg_buf is null\n", __func__);
		return -1;
	}

	ldim_data_mapping(buf, dev_drv->dim_max, dev_drv->dim_min,
			  dev_drv->zone_num, dev_drv->bl_mapping);

	iw7027_reg_write_diff(dev_drv, 0x40, bl_iw7027->reg_buf, 0x20);

	return 0;
}

static int iw7027_power_on(struct aml_ldim_driver_s *ldim_drv)
{
	if (!bl_iw7027)
		return -1;

	if (bl_iw7027->dev_on_flag) {
		LDIMPR("%s: iw7027 is already on, exit\n", __func__);
		return 0;
	}

	iw7027_hw_init_on(ldim_drv->dev_drv);
	bl_iw7027->dev_on_flag = 1;

	LDIMPR("%s: ok\n", __func__);
	return 0;
}

static int iw7027_power_off(struct aml_ldim_driver_s *ldim_drv)
{
	if (!bl_iw7027)
		return -1;

	bl_iw7027->dev_on_flag = 0;
	iw7027_hw_init_off(ldim_drv->dev_drv);

	LDIMPR("%s: ok\n", __func__);
	return 0;
}

static int iw7027_ldim_driver_update(struct ldim_dev_driver_s *dev_drv)
{
	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return -1;
	}

	dev_drv->power_on = iw7027_power_on;
	dev_drv->power_off = iw7027_power_off;
	dev_drv->dev_smr = iw7027_smr;

	dev_drv->reg_write = iw7027_reg_write;
	dev_drv->reg_read = iw7027_reg_read;
	return 0;
}

int ldim_dev_iw7027_probe(struct aml_ldim_driver_s *ldim_drv)
{
	struct ldim_dev_driver_s *dev_drv = ldim_drv->dev_drv;
	unsigned int size;

	if (!dev_drv) {
		LDIMERR("%s: dev_drv is null\n", __func__);
		return -1;
	}
	if (!dev_drv->spi_info.spi) {
		LDIMERR("%s: spi is null\n", __func__);
		return -1;
	}

	bl_iw7027 = (struct iw7027_s *)malloc(sizeof(struct iw7027_s));
	if (!bl_iw7027) {
		LDIMERR("iw7027 malloc error\n");
		return -1;
	}
	memset(bl_iw7027, 0, sizeof(struct iw7027_s));

	bl_iw7027->dev_on_flag = 0;

	/* 16 each device, each zone 2 bytes */
	bl_iw7027->reg_buf_size = 16 * 2 * dev_drv->chip_cnt;
	size = bl_iw7027->reg_buf_size * sizeof(unsigned char);
	bl_iw7027->reg_buf = (unsigned char *)malloc(size);
	if (!bl_iw7027->reg_buf)
		goto ldim_dev_iw7027_probe_err0;
	memset(bl_iw7027->reg_buf, 0, size);

	/* header + reg_max_cnt + chip_cnt */
	bl_iw7027->tbuf_size = 3 + IW7027_REG_MAX + dev_drv->chip_cnt;
	size = bl_iw7027->tbuf_size * sizeof(unsigned char);
	bl_iw7027->tbuf = (unsigned char *)malloc(size);
	if (!bl_iw7027->tbuf)
		goto ldim_dev_iw7027_probe_err1;
	memset(bl_iw7027->tbuf, 0, size);

	/* header + reg_max_cnt + chip_cnt + dev_id_max(=chip_cnt) */
	bl_iw7027->rbuf_size = 3 + IW7027_REG_MAX + dev_drv->chip_cnt * 2;
	size = bl_iw7027->rbuf_size * sizeof(unsigned char);
	bl_iw7027->rbuf = (unsigned char *)malloc(size);
	if (!bl_iw7027->rbuf)
		goto ldim_dev_iw7027_probe_err2;
	memset(bl_iw7027->rbuf, 0, size);

	iw7027_ldim_driver_update(dev_drv);

	LDIMPR("%s ok\n", __func__);
	return 0;

ldim_dev_iw7027_probe_err2:
	free(bl_iw7027->tbuf);
	bl_iw7027->tbuf_size = 0;
ldim_dev_iw7027_probe_err1:
	free(bl_iw7027->reg_buf);
	bl_iw7027->reg_buf_size = 0;
ldim_dev_iw7027_probe_err0:
	free(bl_iw7027);
	bl_iw7027 = NULL;
	return -1;
}

int ldim_dev_iw7027_remove(struct aml_ldim_driver_s *ldim_drv)
{
	if (!bl_iw7027)
		return 0;

	free(bl_iw7027->rbuf);
	bl_iw7027->rbuf_size = 0;
	free(bl_iw7027->tbuf);
	bl_iw7027->tbuf_size = 0;
	free(bl_iw7027->reg_buf);
	bl_iw7027->reg_buf_size = 0;
	free(bl_iw7027);
	bl_iw7027 = NULL;

	return 0;
}

