/*
 * drivers/display/lcd/lcd_bl_ldim/iw7038.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the named License,
 * or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <common.h>
#include <malloc.h>
#include <spi.h>
#include <asm/arch/gpio.h>
#ifdef CONFIG_OF_LIBFDT
#include <libfdt.h>
#endif
#include <amlogic/aml_lcd.h>
#include <amlogic/aml_ldim.h>
#include "../aml_lcd_reg.h"
#include "../aml_lcd_common.h"
#include "ldim_drv.h"
#include "ldim_dev_drv.h"

#define NORMAL_MSG            (0<<7)
#define BROADCAST_MSG         (1<<7)
#define BLOCK_DATA            (0<<6)
#define SINGLE_DATA           (1<<6)
#define IW7038_DEV_ADDR        1


static int iw7038_on_flag;

struct iw7038 {
	int cs_hold_delay;
	int cs_clk_delay;
	unsigned char cmd_size;
	unsigned char *init_data;
	unsigned int init_data_cnt;
	struct spi_slave *spi;
};
static struct iw7038 *bl_iw7038;

#define BRIGHTNESS_OFFSET    3
static unsigned char *val_brightness;

/***** haier use 2 iw7038(32 region)****************/
/*
1.write single data 0x03 (write to reg0x02 of dev0x01/dev0x02)
	0x41  0x02  0x03
	0x42  0x02  0x03 0x00

3.write N data to one device  0x03 0x04 0x05
  (write to reg0x02-reg0x04 of dev0x01/dev0x02)
	devaddr  NrOfBytes   regaddr  data1  data2  data3
	0x01     0x03        0x02     0x03   0x04    0x05
	0x02     0x03        0x82     0x03   0x04    0x05   0x00

4 write different data in same register of all devices single byte
  (write to reg0x02 of dev0x01-dev0x02)
	devaddr  regaddr   data1  data2	 data3
	0xff	 0x02	   0x03   0x04	 0x05   0x00

5 write different data in same register of all devices multiple byte
  (write to reg0x02-reg0x03 of dev0x01-dev0x02)
	dev0x01 write data 0x01 0x02, dev0x02 write data 0x03 0x04,
	devaddr NrOfBytes regaddr data1 data2 data3 data4
	0xbf	0x02	  0x02    0x01  0x02  0x03  0x04   0x00

6 write same data in same register of all devices single byte
  (write to reg0x02 of dev0x01-dev0x02)
	devaddr regaddr  data1
	0xc0    0x02     0x03   0x00

7. write same data in same register of all devices multiple byte
   (write to reg0x21-reg24 of dev0x01-dev0x02)
	all dev write data 0x01 0x02 0x03 0x04
	devaddr  NrOfBytes  regaddr  data1  data2  data3  data4
	0x80	  0x04	    0x21     0x01   0x02   0x03   0x04   0x00

8 read single data
	(read reg0x01 0f dev0x01)
	devaddr  regaddr
	0x41     0x81    0x00  0x00  0x00

	(read reg0x01 0f dev0x02)
	devaddr  regaddr
	0x42     0x81    0x00  0x00  0x00

9 read N data (read to reg 0x02-reg0x04 of dev0x01)
devaddr  NrOfBytes  regaddr                     data1 data2 data3
0x01	 0x03	    0x82     0x00 0x00 0x00     xx    xx     xx

*/

/******************************************
iw7038 register write: R/W(0/1) | reserved(00000) | regaddr[9:0]
*/
static int iw7038_wreg(struct spi_slave *spi,
		       unsigned char devaddr,
		       unsigned int addr,
		       unsigned char val)
{
	unsigned char tbuf[4];
	unsigned char addr_h;
	unsigned char addr_l;
	int ret;

	if (lcd_debug_print_flag)
		LDIMPR("%s: 0x%02x = 0x%02x\n", __func__, addr, val);

	addr_h = (addr >> 8) & 0x3;
	addr_l = addr & 0xff;
	/*broadcast: */
	/*0x00: the same data for all device*/
	/*0x3f: the different data for all device*/
	if ((!devaddr) || (devaddr == 0x3f))
		tbuf[0] = devaddr | BROADCAST_MSG | SINGLE_DATA;
	/*single device: normal data for one single device*/
	else
		tbuf[0] = devaddr | NORMAL_MSG | SINGLE_DATA;

	tbuf[1] = addr_h;
	tbuf[2] = addr_l;
	tbuf[3] = val;
	ret = ldim_spi_write(spi, tbuf, 4);

	return ret;
}

static int iw7038_wreg_MultipleByte(struct spi_slave *spi,
				    unsigned char devaddr,
				    unsigned int addr,
				    unsigned char *val,
				    int len)
{
/* iw7038 be used pwm1-pwm12,register 0x21-0x32, 18byte, 108*1.5+8+3 data*/
/* DevAddr = 0xBF, NrOfByte = 0x12, addr = 0x21*/
	int ret;
	unsigned char addr_h;
	unsigned char addr_l;
	unsigned char tbuf[200];

	if (lcd_debug_print_flag)
		LDIMPR("%s: 0x%02x = 0x%p len = %d\n",
		       __func__, addr, val, len);

	if ((!len) || ((len + 3) > 200))
		return -1;

	addr_h = (addr >> 8) & 0x3;
	addr_l = addr & 0xff;
	if ((!devaddr) || (devaddr == 0x3f))
		tbuf[0] = devaddr | BROADCAST_MSG | BLOCK_DATA;
	else
		tbuf[0] = devaddr | NORMAL_MSG | BLOCK_DATA;

	tbuf[1] = addr_h;
	tbuf[2] = addr_l;
	memcpy(&tbuf[3], val, len);
	ret = ldim_spi_write(spi, tbuf, len + 3);

	return ret;
}

/* iw7038 register read */
static int iw7038_rreg(struct spi_slave *spi, unsigned char devaddr,
		       unsigned int addr, unsigned char *val)
{
	unsigned char tbuf[5], rbuf[5];
	int ret, i;
	unsigned char addr_h, addr_l;

	addr_h = (addr >> 8) & 0x3;
	addr_l = addr & 0xff;

	if ((!devaddr) || (devaddr == 0x3f))
		tbuf[0] = devaddr | BROADCAST_MSG | SINGLE_DATA;
	else
		tbuf[0] = devaddr | NORMAL_MSG | SINGLE_DATA;
	tbuf[1] = addr_h | 0x80;
	tbuf[2] = addr_l;
	for (i = 0; i < 2; i++)
		tbuf[i+3] = 0x00;
	ret = ldim_spi_read(spi, tbuf, 5, rbuf, 1);
	*val = rbuf[4];

	return ret;
}

static int ldim_power_cmd_dynamic_size(void)
{
	unsigned char *table;
	int i = 0, j, step = 0, max_len = 0;
	unsigned char type, cmd_size;
	int delay_ms, ret = 0;

	table = bl_iw7038->init_data;
	max_len = bl_iw7038->init_data_cnt;

	while ((i + 1) < max_len) {
		type = table[i];
		if (type == LCD_EXT_CMD_TYPE_END)
			break;
		if (lcd_debug_print_flag) {
			LDIMPR("%s: step %d: type=0x%02x, cmd_size=%d\n",
			       __func__, step, type, table[i+1]);
		}
		cmd_size = table[i+1];
		if (cmd_size == 0)
			goto power_cmd_dynamic_next;
		if ((i + 2 + cmd_size) > max_len)
			break;

		if (type == LCD_EXT_CMD_TYPE_NONE) {
			/* do nothing */
		} else if (type == LCD_EXT_CMD_TYPE_DELAY) {
			delay_ms = 0;
			for (j = 0; j < cmd_size; j++)
				delay_ms += table[i+2+j];
			if (delay_ms > 0)
				mdelay(delay_ms);
		} else if (type == LCD_EXT_CMD_TYPE_CMD) {
			ret = iw7038_wreg(bl_iw7038->spi, 0x0,
				table[i+2], table[i+3]);
			udelay(1);
		} else if (type == LCD_EXT_CMD_TYPE_CMD_DELAY) {
			ret = iw7038_wreg(bl_iw7038->spi, 0x0,
				table[i+2], table[i+3]);
			udelay(1);
			if (table[i+4] > 0)
				mdelay(table[i+4]);
		} else {
			LDIMERR("%s: type 0x%02x invalid\n", __func__, type);
		}
power_cmd_dynamic_next:
		i += (cmd_size + 2);
		step++;
	}

	return ret;
}

static int ldim_power_cmd_fixed_size(void)
{
	unsigned char *table;
	int i = 0, j, step = 0, max_len = 0;
	unsigned char type, cmd_size;
	int delay_ms, ret = 0;

	cmd_size = bl_iw7038->cmd_size;
	if (cmd_size < 2) {
		LDIMERR("%s: invalid cmd_size %d\n", __func__, cmd_size);
		return -1;
	}

	table = bl_iw7038->init_data;
	max_len = bl_iw7038->init_data_cnt;

	while ((i + cmd_size) <= max_len) {
		type = table[i];
		if (type == LCD_EXT_CMD_TYPE_END)
			break;
		if (lcd_debug_print_flag) {
			LDIMPR("%s: step %d: type=0x%02x, cmd_size=%d\n",
			       __func__, step, type, cmd_size);
		}
		if (type == LCD_EXT_CMD_TYPE_NONE) {
			/* do nothing */
		} else if (type == LCD_EXT_CMD_TYPE_DELAY) {
			delay_ms = 0;
			for (j = 0; j < (cmd_size - 1); j++)
				delay_ms += table[i+1+j];
			if (delay_ms > 0)
				mdelay(delay_ms);
		} else if (type == LCD_EXT_CMD_TYPE_CMD) {
			ret = iw7038_wreg(bl_iw7038->spi, 0x0,
				table[i+1], table[i+2]);
			udelay(1);
		} else if (type == LCD_EXT_CMD_TYPE_CMD_DELAY) {
			ret = iw7038_wreg(bl_iw7038->spi, 0x0,
				table[i+1], table[i+2]);
			udelay(1);
			if (table[i+3] > 0)
				mdelay(table[i+3]);
		} else {
			LDIMERR("%s: type 0x%02x invalid\n", __func__, type);
		}
		i += cmd_size;
		step++;
	}

	return ret;
}

static int iw7038_power_on_init(void)
{
	unsigned char cmd_size;
	int ret = 0;

	cmd_size = bl_iw7038->cmd_size;
	if (cmd_size < 2) {
		LDIMERR("%s: invalid cmd_size %d\n", __func__, cmd_size);
		return -1;
	}
	if (cmd_size == LCD_EXT_CMD_SIZE_DYNAMIC)
		ret = ldim_power_cmd_dynamic_size();
	else
		ret = ldim_power_cmd_fixed_size();

	return ret;
}

static int iw7038_hw_init_on(void)
{
	int i;
	unsigned char reg_duty_chk = 0;
	unsigned char reg_chk = 0, done = 0;
	unsigned int device_count;
	unsigned int spi_device_id;
	struct aml_ldim_driver_s *ldim_drv = aml_ldim_get_driver();

	/* step 1: system power_on */
	LDIMPR("%s: iw7038 system power_on\n", __func__);
	ldim_set_gpio(ldim_drv->ldev_conf->en_gpio,
		      ldim_drv->ldev_conf->en_gpio_on);
	ldim_set_duty_pwm(&(ldim_drv->ldev_conf->ldim_pwm_config));
	ldim_set_duty_pwm(&(ldim_drv->ldev_conf->analog_pwm_config));

	/* step 2: delay for internal logic stable */
	mdelay(10);

	/* step 3: SPI communication check */
	LDIMPR("%s: SPI Communication Check\n", __func__);
	device_count = ldim_drv->ldev_conf->device_count;
	for (spi_device_id = 0; spi_device_id < device_count;
	     spi_device_id++) {
		iw7038_wreg(bl_iw7038->spi, 0x0, 0x00, 0x12);
		for (i = 0; i <= 10; i++) {
			reg_chk = 0;
			iw7038_rreg(bl_iw7038->spi, spi_device_id,
				    0x00, &reg_chk);
			if (reg_chk == 0x12)
				break;
			if (i == 10) {
				LDIMERR
				("%s: SPI communication check dev error\n",
				__func__);
			}
		}
	}

	/* step 4: supply stable vsync */
	LDIMPR("%s: open Vsync\n", __func__);
	ldim_drv->pinmux_ctrl(1);

	/* step 5: configure initial registers */
	LDIMPR("%s: Write initial control registers\n", __func__);
	iw7038_power_on_init();
	 /*lock registers*/
	iw7038_wreg(bl_iw7038->spi, 0x0, 0xa1, 0x00);
	iw7038_wreg(bl_iw7038->spi, 0x0, 0x9a, 0x00);

	/* step 6: delay for system clock and light bar PSU stable */
	mdelay(550);

	/* step 7: start calibration */
	LDIMPR("%s: start calibration\n", __func__);
	iw7038_wreg(bl_iw7038->spi, 0x0, 0x00, 0x13);
	mdelay(200);

	/* step 8: calibration done or not */
	i = 0;
	while (++i < 100) {
		for (spi_device_id = 0; spi_device_id < device_count;
		     spi_device_id++) {
			iw7038_rreg(bl_iw7038->spi, spi_device_id, 0x142,
				    &reg_duty_chk);
			/*VDAC statue reg :FB1=[0x5] FB2=[0x50]*/
			/*The current platform using FB1*/
			if ((reg_duty_chk & 0xf) == 0x4) {
				done = 1;
				break;
			}
			mdelay(10);
		}
	}

	LDIMPR("%s: calibration %s: [%d] = %x\n",
	       __func__, (done ? "OK" : "fail"), i, reg_duty_chk);

	return 0;
}

static int iw7038_hw_init_off(void)
{
	struct aml_ldim_driver_s *ldim_drv = aml_ldim_get_driver();

	ldim_drv->pinmux_ctrl(0);
	ldim_set_gpio(ldim_drv->ldev_conf->en_gpio,
		      ldim_drv->ldev_conf->en_gpio_off);

	return 0;
}

static unsigned int dim_max, dim_min;
static unsigned int iw7038_get_value(unsigned int level)
{
	unsigned int val;

	val = dim_min + ((level * (dim_max - dim_min)) / LD_DATA_MAX);

	return val;
}

static int iw7038_smr(unsigned short *buf, unsigned char len)
{
	unsigned int i, temp;
	unsigned short num;
	struct aml_ldim_driver_s *ldim_drv = aml_ldim_get_driver();

	if (iw7038_on_flag == 0) {
		LDIMPR("%s: on_flag=%d\n", __func__, iw7038_on_flag);
		return 0;
	}
	num = ldim_drv->ldev_conf->bl_regnum;
	if (len != num) {
		LDIMERR("%s: data len %d invalid\n", __func__, len);
		return -1;
	}

	if (val_brightness == NULL) {
		LDIMERR("%s: val_brightness is null\n", __func__);
		return -1;
	}

	dim_max = ldim_drv->ldev_conf->dim_max;
	dim_min = ldim_drv->ldev_conf->dim_min;

	for (i = 0; i < num; i++) {
		temp = iw7038_get_value(buf[i]);
		val_brightness[BRIGHTNESS_OFFSET+2*i] = (temp >> 8) & 0xf;
		val_brightness[BRIGHTNESS_OFFSET+2*i+1] = temp & 0xff;
	}

	iw7038_wreg_MultipleByte(bl_iw7038->spi, 0x01, 0x40,
				 val_brightness, (num * 2) / 4);
	iw7038_wreg_MultipleByte(bl_iw7038->spi, 0x02, 0x40,
				 &val_brightness[32], (num * 2) / 4);
	iw7038_wreg_MultipleByte(bl_iw7038->spi, 0x03, 0x40,
				 &val_brightness[64], (num * 2) / 4);
	iw7038_wreg_MultipleByte(bl_iw7038->spi, 0x04, 0x40,
				 &val_brightness[96], (num * 2) / 4);

	return 0;
}

static int iw7038_power_on(void)
{
	iw7038_hw_init_on();
	iw7038_on_flag = 1;

	LDIMPR("%s: ok\n", __func__);
	return 0;
}

static int iw7038_power_off(void)
{
	iw7038_on_flag = 0;
	iw7038_hw_init_off();

	LDIMPR("%s: ok\n", __func__);
	return 0;
}

static int iw7038_ldim_driver_update(struct aml_ldim_driver_s *ldim_drv)
{
	ldim_drv->device_power_on = iw7038_power_on;
	ldim_drv->device_power_off = iw7038_power_off;
	ldim_drv->device_bri_update = iw7038_smr;
	return 0;
}

int ldim_dev_iw7038_probe(struct aml_ldim_driver_s *ldim_drv)
{
	if (ldim_drv->spi_info->spi == NULL) {
		LDIMERR("%s: spi is null\n", __func__);
		return -1;
	}

	bl_iw7038 = (struct iw7038 *)malloc(sizeof(struct iw7038));
	if (bl_iw7038 == NULL) {
		LDIMERR("iw7038 malloc error\n");
		return -1;
	}
	memset(bl_iw7038, 0, sizeof(struct iw7038));

	iw7038_on_flag = 0;

	bl_iw7038->spi = ldim_drv->spi_info->spi;
	bl_iw7038->cs_hold_delay = ldim_drv->ldev_conf->cs_hold_delay;
	bl_iw7038->cs_clk_delay = ldim_drv->ldev_conf->cs_clk_delay;
	bl_iw7038->cmd_size = ldim_drv->ldev_conf->cmd_size;
	bl_iw7038->init_data = ldim_drv->ldev_conf->init_on;
	bl_iw7038->init_data_cnt = ldim_drv->ldev_conf->init_on_cnt;

	val_brightness = (unsigned char *)malloc(
		(ldim_drv->ldev_conf->bl_regnum * 2) + 4);
	if (val_brightness == NULL) {
		LDIMERR("malloc val_brightness failed\n");
		free(bl_iw7038);
		return -1;
	}

	iw7038_ldim_driver_update(ldim_drv);

	printf("%s: ok\n", __func__);

	return 0;
}

int ldim_dev_iw7038_remove(struct aml_ldim_driver_s *ldim_drv)
{
	if (val_brightness) {
		free(val_brightness);
		val_brightness = NULL;
	}

	if (bl_iw7038) {
		free(bl_iw7038);
		bl_iw7038 = NULL;
	}
	return 0;
}

