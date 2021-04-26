/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * board/amlogic/tl1_x301_v1/lcd_tconless_spi_data.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <stdlib.h>
#ifdef CONFIG_AML_SPICC
#include <amlogic/spicc.h>
#endif
#include <spi.h>
#include <spi_flash.h>
#include <amlogic/aml_lcd.h>
#include "lcd_tconless_spi_data.h"

#define LCDSPI_PR(fmt, args...)     printf("lcd_spi: "fmt"", ## args)
#define LCDSPI_ERR(fmt, args...)    printf("lcd_spi: error: "fmt"", ## args)

#define LCD_TCONLESS_SPI_RETRY_MAX    5

struct lcd_tconless_spi_s {
	/* spi flash */
	struct udevice *devp;
	struct spi_flash *flash;
	unsigned int spi_bus;
	unsigned int spi_cs;
	unsigned int spi_speed;
	unsigned int spi_mode;
};

static struct lcd_tconless_spi_s lcd_spi = {
	.devp = NULL,
	.flash = NULL,
	.spi_bus = CONFIG_SF_DEFAULT_BUS,
	.spi_cs = CONFIG_SF_DEFAULT_CS,
	.spi_speed = CONFIG_SF_DEFAULT_SPEED,
	.spi_mode = CONFIG_SF_DEFAULT_MODE,
};

static int lcd_tconless_spi_data_load(unsigned int offset, unsigned int len,
				      unsigned char *buf)
{
	int ret, i, n = 0;

	if (len == 0) {
		if (lcd_debug_print_flag)
			LCDSPI_PR("%s: no spi data, len is zero\n", __func__);
		return -1;
	}

	if (!buf) {
		LCDSPI_ERR("%s: buf is NULL\n", __func__);
		return -1;
	}

	if (!lcd_spi.devp) {
spi_flash_probe_retry:
		ret = spi_flash_probe_bus_cs(lcd_spi.spi_bus,
				lcd_spi.spi_cs,
				lcd_spi.spi_speed,
				lcd_spi.spi_mode,
				&lcd_spi.devp);
		if (ret) {
			if (n++ < LCD_TCONLESS_SPI_RETRY_MAX) {
				LCDSPI_ERR("%s:probe sf flash fail %d\n", __func__, n);
				mdelay(10);
				goto spi_flash_probe_retry;
			}
			LCDSPI_ERR("%s:probe sf flash fail timeout\n", __func__);
			return -1;
		}
	}

	if (!lcd_spi.devp) {
		LCDSPI_ERR("%s: get spi devp fail\n", __func__);
		return -1;
	}

	lcd_spi.flash = lcd_spi.devp->uclass_priv;
	ret = spi_flash_read(lcd_spi.flash, offset, len, (void *)buf);
	if (ret) {
		LCDSPI_ERR("%s: spi flash read fail\n", __func__);
		return -1;
	}

	if (lcd_debug_print_flag) {
		LCDSPI_PR("%s: spi flash read [offset=0x%x, len=%d]:\n",
			__func__, offset, len);
		for (i = 0; i < (len + 2); i++)
			printf(" 0x%02x", buf[i]);
		printf("\n");
	}

	return 0;
}

static int lcd_tconless_spi_data_read(struct lcd_tcon_spi_block_s *spi_block)
{
	unsigned int offset, len, i, new_size;
	unsigned char crc_buf[8] = {0};
	int ret;

	if (!spi_block) {
		LCDSPI_ERR("%s: spi_block is null\n", __func__);
		return -1;
	}
	if (spi_block->spi_size == 0) {
		LCDSPI_ERR("spi_size is 0\n");
		return -1;
	}

	new_size = lcd_tcon_data_size_align(spi_block->spi_size);
	spi_block->raw_buf = (unsigned char *)malloc(new_size);
	if (!spi_block->raw_buf) {
		LCDSPI_ERR("failed to alloc raw_buf\n");
		return -1;
	}
	memset(spi_block->raw_buf, 0, new_size);

	/* load lut data from spi */
	offset = spi_block->spi_offset;
	len = spi_block->spi_size;
	ret = lcd_tconless_spi_data_load(offset, len, spi_block->raw_buf);
	if (ret)
		return -1;

	/* load crc data from spi */
	if (spi_block->data_type == LCD_TCON_DATA_BLOCK_TYPE_DEMURA_LUT) {
		if (spi_block->param_cnt >= 2) {
			offset = spi_block->param[0];
			len = spi_block->param[1];
			if (len == 0)
				return 0;
			if (len > 4) {
				LCDSPI_ERR("%s: crc len %d invalid\n", __func__, len);
				return -1;
			}
			ret = lcd_tconless_spi_data_load(offset, len, crc_buf);
			if (ret)
				return -1;
			spi_block->data_raw_check = 0;
			for (i = 0; i < len; i++)
				spi_block->data_raw_check |= (crc_buf[i] << (i * 8));
			if (lcd_debug_print_flag) {
				LCDSPI_PR("%s: data_raw_check: 0x%x\n",
					  __func__, spi_block->data_raw_check);
			}
		}
	}

	return 0;
}

static int lcd_tconless_spi_data_demura_format(struct lcd_tcon_spi_block_s *spi_block)
{
	unsigned int temp;
	unsigned int n, i, j, new_size;
	unsigned int *part_offset;
	unsigned int part_mapping_size, ext_header_size;
	unsigned int temp_data_cnt, new_data_cnt, part_cnt;
	unsigned int block_type, block_flag, raw_crc, chip_id;
	unsigned char *new_buf;

	//temp_data_cnt = 391053;
	chip_id = LCD_CHIP_TL1;
	part_cnt = 7;
	part_mapping_size = 4;
	temp_data_cnt = spi_block->data_temp_size;
	ext_header_size = 16 + part_mapping_size * part_cnt;
	new_data_cnt = LCD_TCON_DATA_BLOCK_HEADER_SIZE + ext_header_size +
		       (LCD_TCON_DATA_PART_NAME_SIZE + 10) * 5 +
		       (LCD_TCON_DATA_PART_NAME_SIZE + 19) +
		       (LCD_TCON_DATA_PART_NAME_SIZE + 16 + temp_data_cnt);
	spi_block->data_new_size = new_data_cnt;

	block_type = spi_block->data_type;
	block_flag = LCD_TCON_DATA_VALID_DEMURA;
	raw_crc = spi_block->data_raw_check;

	part_offset = (unsigned int *)malloc(part_cnt * sizeof (unsigned int));
	if (!part_offset)
		return -1;
	memset(part_offset, 0x0, part_cnt * sizeof (unsigned int));
	new_size = lcd_tcon_data_size_align(new_data_cnt);
	new_buf = (unsigned char *)malloc(new_size);
	if (!new_buf) {
		LCDSPI_ERR("%s: failed to alloc new_buf \n", __func__);
		free(part_offset);
		return -1;
	}
	memset(new_buf, 0x0, new_size);
	spi_block->new_buf = new_buf;

	/* raw data crc */
	new_buf[4] = raw_crc & 0xff;
	new_buf[5] = (raw_crc >> 8) & 0xff;
	new_buf[6] = (raw_crc >> 16) & 0xff;
	new_buf[7] = (raw_crc >> 24) & 0xff;

	/* block size */
	new_buf[8] = new_data_cnt & 0xff;
	new_buf[9] = (new_data_cnt >> 8) & 0xff;
	new_buf[10] = (new_data_cnt >> 16) & 0xff;
	new_buf[11] = (new_data_cnt >> 24) & 0xff;

	/* header size */
	temp = LCD_TCON_DATA_BLOCK_HEADER_SIZE;
	new_buf[12] = temp & 0xff;
	new_buf[13] = (temp >> 8) & 0xff;

	/* ext header size */
	new_buf[14] = ext_header_size & 0xff;
	new_buf[15] = (ext_header_size >> 8) & 0xff;

	new_buf[16] = block_type & 0xff;
	new_buf[17] = (block_type >> 8) & 0xff;
	new_buf[18] = (block_type >> 16) & 0xff;
	new_buf[19] = (block_type >> 24) & 0xff;

	new_buf[20] = block_flag & 0xff;
	new_buf[21] = (block_flag >> 8) & 0xff;
	new_buf[22] = (block_flag >> 16) & 0xff;
	new_buf[23] = (block_flag >> 24) & 0xff;

	temp = spi_block->data_index;
	new_buf[24] = temp & 0xff;
	new_buf[25] = (temp >> 8) & 0xff;

	temp = chip_id;
	new_buf[26] = temp & 0xff;
	new_buf[27] = (temp >> 8) & 0xff;

	temp = LCD_TCON_DATA_BLOCK_NAME_SIZE - 1;
	strncpy((char *)(&new_buf[28]), "demura_lut", temp);

	/*part 0*/
	part_offset[0] = 64 + ext_header_size;
	n = part_offset[0];
	temp = LCD_TCON_DATA_PART_NAME_SIZE - 1;
	strncpy((char *)(&new_buf[n]), "demura_en_check", temp);
	n += LCD_TCON_DATA_PART_NAME_SIZE;
	temp = 0;
	new_buf[n] = temp & 0xff;//part_id
	new_buf[n + 1] = (temp >> 8) & 0xff;
	new_buf[n + 2] = LCD_TCON_DATA_PART_FLAG_FIXED_REG;//tuning_flag
	new_buf[n + 3] = LCD_TCON_DATA_PART_TYPE_CHK_EXIT;//data_type
	new_buf[n + 4] = 0x2;//reg_addr_byte_width
	new_buf[n + 5] = 0x1;//reg_data_byte_width
	temp = 0x23d;
	new_buf[n + 6] = temp & 0xff;//reg_addr_check
	new_buf[n + 7] = (temp >> 8) & 0xff;
	new_buf[n + 8] = 0x01;//data_check_mask
	new_buf[n + 9] = 0x01;//data_checked

	/*part 1*/
	part_offset[1] = n + 10;
	n = part_offset[1];
	temp = LCD_TCON_DATA_PART_NAME_SIZE - 1;
	strncpy((char *)(&new_buf[n]), "demura_disable_first", temp);
	n += LCD_TCON_DATA_PART_NAME_SIZE;
	temp = 1;
	new_buf[n] = temp & 0xff;//part_id
	new_buf[n + 1] = (temp >> 8) & 0xff;
	new_buf[n + 2] = LCD_TCON_DATA_PART_FLAG_FIXED_REG;//tuning_flag
	new_buf[n + 3] = LCD_TCON_DATA_PART_TYPE_WR_MASK;//data_type
	new_buf[n + 4] = 0x2;//reg_addr_byte_width
	new_buf[n + 5] = 0x1;//reg_data_byte_width
	temp = 0x23d;
	new_buf[n + 6] = temp & 0xff;//reg_addr
	new_buf[n + 7] = (temp >> 8) & 0xff;
	new_buf[n + 8] = 0x01; //data_mask
	new_buf[n + 9] = 0x00; //data_value

	/*part 2*/
	part_offset[2] = n + 10;
	n = part_offset[2];
	temp = LCD_TCON_DATA_PART_NAME_SIZE - 1;
	strncpy((char *)(&new_buf[n]), "demura_ctrl_reg_group1", temp);
	n += LCD_TCON_DATA_PART_NAME_SIZE;
	temp = 2;
	new_buf[n] = temp & 0xff;//part_id
	new_buf[n + 1] = (temp >> 8) & 0xff;
	new_buf[n + 2] = LCD_TCON_DATA_PART_FLAG_FIXED_REG;//tuning_flag
	new_buf[n + 3] = LCD_TCON_DATA_PART_TYPE_WR_MASK;//data_type
	new_buf[n + 4] = 0x2;//reg_addr_byte_width
	new_buf[n + 5] = 0x1;//reg_data_byte_width
	temp = 0x178;
	new_buf[n + 6] = temp & 0xff;//reg_addr
	new_buf[n + 7] = (temp >> 8) & 0xff;
	new_buf[n + 8] = 0xff; //data_mask
	new_buf[n + 9] = 0x00; //data_value

	/*part 3*/
	part_offset[3] = n + 10;
	n = part_offset[3];
	temp = LCD_TCON_DATA_PART_NAME_SIZE - 1;
	strncpy((char *)(&new_buf[n]), "demura_ctrl_reg_group2", temp);
	n += LCD_TCON_DATA_PART_NAME_SIZE;
	temp = 3;
	new_buf[n] = temp & 0xff;//part_id
	new_buf[n + 1] = (temp >> 8) & 0xff;
	new_buf[n + 2] = LCD_TCON_DATA_PART_FLAG_FIXED_REG;//tuning_flag
	new_buf[n + 3] = LCD_TCON_DATA_PART_TYPE_WR_MASK;//data_type
	new_buf[n + 4] = 0x2;//reg_addr_byte_width
	new_buf[n + 5] = 0x1;//reg_data_byte_width
	temp = 0x17c;
	new_buf[n + 6] = temp & 0xff;//reg_addr
	new_buf[n + 7] = (temp >> 8) & 0xff;
	new_buf[n + 8] = 0xff; //data_mask
	new_buf[n + 9] = 0x21; //data_value

	/*part 4*/
	part_offset[4] = n + 10;
	n = part_offset[4];
	temp = LCD_TCON_DATA_PART_NAME_SIZE - 1;
	strncpy((char *)(&new_buf[n]), "demura_ctrl_reg_group3", temp);
	n += LCD_TCON_DATA_PART_NAME_SIZE;
	temp = 4;
	new_buf[n] = temp & 0xff;//part_id
	new_buf[n + 1] = (temp >> 8) & 0xff;
	new_buf[n + 2] = LCD_TCON_DATA_PART_FLAG_FIXED_REG;//tuning_flag
	new_buf[n + 3] = LCD_TCON_DATA_PART_TYPE_WR_N;//data_type
	new_buf[n + 4] = 0x2;//reg_addr_byte_width
	new_buf[n + 5] = 0x1;//reg_data_byte_width
	new_buf[n + 6] = 0x1;//reg_auto_inc
	new_buf[n + 7] = 0x1;//reg_cnt
	temp = 5;
	new_buf[n + 8] = temp & 0xff;//data_cnt
	new_buf[n + 9] = (temp >> 8) & 0xff;
	new_buf[n + 10] = (temp >> 16) & 0xff;
	new_buf[n + 11] = (temp >> 24) & 0xff;
	temp = 0x181;
	new_buf[n + 12] = temp & 0xff;//reg_start
	new_buf[n + 13] = (temp >> 8) & 0xff;
	new_buf[n + 14] = 0x01;
	new_buf[n + 15] = 0x01;
	new_buf[n + 16] = 0x86;
	new_buf[n + 17] = 0x01;
	new_buf[n + 18] = 0x87;

	/*part 5*/
	part_offset[5] = n + 19;
	n = part_offset[5];
	temp = LCD_TCON_DATA_PART_NAME_SIZE - 1;
	strncpy((char *)(&new_buf[n]), "demura_lut_data", temp);
	n += LCD_TCON_DATA_PART_NAME_SIZE;
	temp = 5;
	new_buf[n] = temp & 0xff;//part_id
	new_buf[n + 1] = (temp >> 8) & 0xff;
	new_buf[n + 2] = LCD_TCON_DATA_PART_FLAG_FIXED_LUT;//tuning_flag
	new_buf[n + 3] = LCD_TCON_DATA_PART_TYPE_WR_N;//data_type
	new_buf[n + 4] = 0x2;//reg_addr_byte_width
	new_buf[n + 5] = 0x1;//reg_data_byte_width
	new_buf[n + 6] = 0x0;//reg_auto_inc
	new_buf[n + 7] = 0x1;//reg_cnt
	temp = temp_data_cnt + 2;
	new_buf[n + 8] = temp & 0xff;//data_cnt
	new_buf[n + 9] = (temp >> 8) & 0xff;
	new_buf[n + 10] = (temp >> 16) & 0xff;
	new_buf[n + 11] = (temp >> 24) & 0xff;
	temp = 0x187;
	new_buf[n + 12] = temp & 0xff;//reg_start
	new_buf[n + 13] = (temp >> 8) & 0xff;
	new_buf[n + 14] = 0;/* fixed 2 byte 0 for border */
	new_buf[n + 15] = 0;/* fixed 2 byte 0 for border */
	n += 16;
	for (i = 0; i < temp_data_cnt; i++)
		new_buf[n + i] = spi_block->temp_buf[i];

	/*part 6*/
	part_offset[6] = n + temp_data_cnt;
	n = part_offset[6];
	temp = LCD_TCON_DATA_PART_NAME_SIZE - 1;
	strncpy((char *)(&new_buf[n]), "demura_en", temp);
	n += LCD_TCON_DATA_PART_NAME_SIZE;
	temp = 6;
	new_buf[n] = temp & 0xff;//part_id
	new_buf[n + 1] = (temp >> 8) & 0xff;
	new_buf[n + 2] = LCD_TCON_DATA_PART_FLAG_FIXED_REG;//tuning_flag
	new_buf[n + 3] = LCD_TCON_DATA_PART_TYPE_WR_MASK;//data_type
	new_buf[n + 4] = 0x2;//reg_addr_byte_width
	new_buf[n + 5] = 0x1;//reg_data_byte_width
	temp = 0x23d;
	new_buf[n + 6] = temp & 0xff;//reg_addr
	new_buf[n + 7] = (temp >> 8) & 0xff;
	new_buf[n + 8] = 0x01; //data_mask
	new_buf[n + 9] = 0x01; //data_value

	/*ext heaer*/
	n = LCD_TCON_DATA_BLOCK_HEADER_SIZE;
	new_buf[n] = part_cnt & 0xff;
	new_buf[n + 1] = (part_cnt >> 8) & 0xff;
	new_buf[n + 2] = part_mapping_size;
	//reserved 13byts
	/* part offset */
	n += 16;
	for (i = 0; i < part_cnt; i++) {
		temp = n + i * part_mapping_size;
		for (j = 0; j < part_mapping_size; j++)
			new_buf[temp + j] = (part_offset[i] >> (j * 8)) & 0xff;
	}

	/*data check*/
	temp = lcd_tcon_checksum(&new_buf[4], (new_data_cnt - 4));
	new_buf[0] = temp;
	temp = lcd_tcon_lrc(&new_buf[4], (new_data_cnt - 4));
	new_buf[1] = temp;
	new_buf[2] = 0x55;
	new_buf[3] = 0xaa;

	free(part_offset);
	return 0;
}

static int lcd_tconless_spi_data_acc_format(struct lcd_tcon_spi_block_s *spi_block)
{
	unsigned int temp;
	unsigned int n, i, j, new_size;
	unsigned int *part_offset;
	unsigned int part_mapping_size, ext_header_size;
	unsigned int temp_data_cnt, new_data_cnt, part_cnt;
	unsigned int block_type, block_flag, raw_crc, chip_id;
	unsigned char *new_buf;

	//temp_data_cnt = 391053;
	chip_id = LCD_CHIP_TL1;
	part_cnt = 3;
	part_mapping_size = 4;
	temp_data_cnt = spi_block->data_temp_size;
	ext_header_size = 16 + part_mapping_size * part_cnt;
	new_data_cnt = LCD_TCON_DATA_BLOCK_HEADER_SIZE + ext_header_size +
		       (LCD_TCON_DATA_PART_NAME_SIZE + 10) * 2 +
		       (LCD_TCON_DATA_PART_NAME_SIZE + 14 + temp_data_cnt);
	spi_block->data_new_size = new_data_cnt;

	block_type = spi_block->data_type;
	block_flag = LCD_TCON_DATA_VALID_ACC;
	raw_crc = spi_block->data_raw_check;

	part_offset = (unsigned int *)malloc(part_cnt * sizeof (unsigned int));
	if (!part_offset)
		return -1;
	memset(part_offset, 0x0, part_cnt * sizeof (unsigned int));
	new_size = lcd_tcon_data_size_align(new_data_cnt);
	new_buf = (unsigned char *)malloc(new_size);
	if (!new_buf) {
		LCDSPI_ERR("%s: failed to alloc new_buf \n", __func__);
		free(part_offset);
		return -1;
	}
	spi_block->new_buf = new_buf;
	memset(new_buf, 0x0, new_size);

	/* raw data crc */
	new_buf[4] = raw_crc & 0xff;
	new_buf[5] = (raw_crc >> 8) & 0xff;
	new_buf[6] = (raw_crc >> 16) & 0xff;
	new_buf[7] = (raw_crc >> 24) & 0xff;

	/* block size */
	new_buf[8] = new_data_cnt & 0xff;
	new_buf[9] = (new_data_cnt >> 8) & 0xff;
	new_buf[10] = (new_data_cnt >> 16) & 0xff;
	new_buf[11] = (new_data_cnt >> 24) & 0xff;

	/* header size */
	temp = LCD_TCON_DATA_BLOCK_HEADER_SIZE;
	new_buf[12] = temp & 0xff;
	new_buf[13] = (temp >> 8) & 0xff;

	/* ext header size */
	new_buf[14] = ext_header_size & 0xff;
	new_buf[15] = (ext_header_size >> 8) & 0xff;

	new_buf[16] = block_type & 0xff;
	new_buf[17] = (block_type >> 8) & 0xff;
	new_buf[18] = (block_type >> 16) & 0xff;
	new_buf[19] = (block_type >> 24) & 0xff;

	new_buf[20] = block_flag & 0xff;
	new_buf[21] = (block_flag >> 8) & 0xff;
	new_buf[22] = (block_flag >> 16) & 0xff;
	new_buf[23] = (block_flag >> 24) & 0xff;

	temp = spi_block->data_index;
	new_buf[24] = temp & 0xff;
	new_buf[25] = (temp >> 8) & 0xff;

	temp = chip_id;
	new_buf[26] = temp & 0xff;
	new_buf[27] = (temp >> 8) & 0xff;

	temp = LCD_TCON_DATA_BLOCK_NAME_SIZE - 1;
	strncpy((char *)(&new_buf[28]), "acc_lut", temp);

	/*part 0*/
	part_offset[0] = 64 + ext_header_size;
	n = part_offset[0];
	temp = LCD_TCON_DATA_PART_NAME_SIZE - 1;
	strncpy((char *)(&new_buf[n]), "dg_disable_first", temp);
	n += LCD_TCON_DATA_PART_NAME_SIZE;
	temp = 1;
	new_buf[n] = temp & 0xff;//part_id
	new_buf[n + 1] = (temp >> 8) & 0xff;
	new_buf[n + 2] = LCD_TCON_DATA_PART_FLAG_FIXED_REG;//tuning_flag
	new_buf[n + 3] = LCD_TCON_DATA_PART_TYPE_WR_MASK;//data_type
	new_buf[n + 4] = 0x2;//reg_addr_byte_width
	new_buf[n + 5] = 0x1;//reg_data_byte_width
	temp = 0x262;
	new_buf[n + 6] = temp & 0xff;//reg_addr
	new_buf[n + 7] = (temp >> 8) & 0xff;
	new_buf[n + 8] = 0x03; //data_mask
	new_buf[n + 9] = 0x02; //data_value

	/*part 1*/
	part_offset[1] = n + 10;
	n = part_offset[1];
	temp = LCD_TCON_DATA_PART_NAME_SIZE - 1;
	strncpy((char *)(&new_buf[n]), "acc_lut_data", temp);
	n += LCD_TCON_DATA_PART_NAME_SIZE;
	temp = 5;
	new_buf[n] = temp & 0xff;//part_id
	new_buf[n + 1] = (temp >> 8) & 0xff;
	new_buf[n + 2] = LCD_TCON_DATA_PART_FLAG_TUNINTG_LUT;//tuning_flag
	new_buf[n + 3] = LCD_TCON_DATA_PART_TYPE_WR_N;//data_type
	new_buf[n + 4] = 0x2;//reg_addr_byte_width
	new_buf[n + 5] = 0x1;//reg_data_byte_width
	new_buf[n + 6] = 0x1;//reg_auto_inc
	new_buf[n + 7] = 0x1;//reg_cnt
	temp = temp_data_cnt;
	new_buf[n + 8] = temp & 0xff;//data_cnt
	new_buf[n + 9] = (temp >> 8) & 0xff;
	new_buf[n + 10] = (temp >> 16) & 0xff;
	new_buf[n + 11] = (temp >> 24) & 0xff;
	temp = 0xb50;
	new_buf[n + 12] = temp & 0xff;//reg_start
	new_buf[n + 13] = (temp >> 8) & 0xff;
	n += 14;
	for (i = 0; i < temp_data_cnt; i++)
		new_buf[n + i] = spi_block->temp_buf[i];

	/*part 2*/
	part_offset[2] = n + temp_data_cnt;
	n = part_offset[2];
	temp = LCD_TCON_DATA_PART_NAME_SIZE - 1;
	strncpy((char *)(&new_buf[n]), "dg_en", temp);
	n += LCD_TCON_DATA_PART_NAME_SIZE;
	temp = 3;
	new_buf[n] = temp & 0xff;//part_id
	new_buf[n + 1] = (temp >> 8) & 0xff;
	new_buf[n + 2] = LCD_TCON_DATA_PART_FLAG_FIXED_REG;//tuning_flag
	new_buf[n + 3] = LCD_TCON_DATA_PART_TYPE_WR_MASK;//data_type
	new_buf[n + 4] = 0x2;//reg_addr_byte_width
	new_buf[n + 5] = 0x1;//reg_data_byte_width
	temp = 0x262;
	new_buf[n + 6] = temp & 0xff;//reg_addr
	new_buf[n + 7] = (temp >> 8) & 0xff;
	new_buf[n + 8] = 0x03; //data_mask
	new_buf[n + 9] = 0x03; //data_value

	/*ext heaer*/
	n = LCD_TCON_DATA_BLOCK_HEADER_SIZE;
	new_buf[n] = part_cnt & 0xff;
	new_buf[n + 1] = (part_cnt >> 8) & 0xff;
	new_buf[n + 2] = part_mapping_size;
	//reserved 13byts
	/* part offset */
	n += 16;
	for (i = 0; i < part_cnt; i++) {
		temp = n + i * part_mapping_size;
		for (j = 0; j < part_mapping_size; j++)
			new_buf[temp + j] = (part_offset[i] >> (j * 8)) & 0xff;
	}

	/*data check*/
	temp = lcd_tcon_checksum(&new_buf[4], (new_data_cnt - 4));
	new_buf[0] = temp;
	temp = lcd_tcon_lrc(&new_buf[4], (new_data_cnt - 4));
	new_buf[1] = temp;
	new_buf[2] = 0x55;
	new_buf[3] = 0xaa;

	free(part_offset);
	return 0;
}

/* ======================tcon data conversion========================= */
/*conv of 43-inch demura*/
static int lcd_tconless_demura_conv_cspi43(struct lcd_tcon_spi_block_s *spi_block)
{
	//todo
	return 0;
}

static int lcd_tconless_acc_conv_usit55(struct lcd_tcon_spi_block_s *spi_block)
{
	//todo
	return 0;
}

/* ================================================================ */
static int lcd_tconless_spi_data_conv_tcon(struct lcd_tcon_spi_block_s *spi_block)
{
	unsigned int panel_size, conv_method, panel_vendor;
	int ret = -1;

	switch (spi_block->data_type) {
	case LCD_TCON_DATA_BLOCK_TYPE_DEMURA_LUT:
		panel_size = spi_block->data_flag & 0xff;
		panel_vendor = (spi_block->data_flag >> 8) & 0xff;
		conv_method = (spi_block->data_flag >> 16) & 0xff;

		if (panel_vendor == 1) { /* CSOT */
			if (panel_size == 43) {
				switch (conv_method) {
				case 0:
					ret = lcd_tconless_demura_conv_cspi43(spi_block);
					if (ret) {
						LCDSPI_ERR("%s: 43cspi demura conversion failed!\n", __func__);
						return -1;
					}
					ret = lcd_tconless_spi_data_demura_format(spi_block);
					break;
				default:
					break;
				}
			}
		}
		break;
	case LCD_TCON_DATA_BLOCK_TYPE_ACC_LUT:
		panel_size = spi_block->data_flag & 0xff;
		panel_vendor = (spi_block->data_flag >> 8) & 0xff;
		conv_method = (spi_block->data_flag >> 16) & 0xff;

		if (panel_vendor == 2) { /* SAMSUNG */
			if (panel_size == 55) {
				switch (conv_method) {
				case 0:
					ret = lcd_tconless_acc_conv_usit55(spi_block);
					if (ret) {
						LCDSPI_ERR("%s: usit55 acc conversion failed!\n", __func__);
						return -1;
					}
					ret = lcd_tconless_spi_data_acc_format(spi_block);
					break;
				default:
					break;
				}
			}
		}
		break;
	default:
		break;
	}

	return ret;
}

static int lcd_tconless_spi_data_conv_pmu(struct lcd_tcon_spi_block_s *spi_block)
{
	unsigned int new_size, i;

	spi_block->data_new_size = spi_block->spi_size + 2;
	new_size = lcd_tcon_data_size_align(spi_block->data_new_size);
	spi_block->new_buf = (unsigned char *)malloc(new_size);
	if (!spi_block->new_buf) {
		LCDSPI_ERR("%s: failed to alloc new_buf \n", __func__);
		return -1;
	}
	memset(spi_block->new_buf, 0x0, new_size);

	spi_block->new_buf[0] = spi_block->data_new_size;
	spi_block->new_buf[1] = 0x00; /* reg */
	for (i = 0; i < spi_block->data_new_size; i++)
		spi_block->new_buf[i + 2] = spi_block->raw_buf[i];

	return 0;
}

static int lcd_tconless_spi_data_conv(struct lcd_tcon_spi_block_s *spi_block)
{
	int ret = -1;

	if (!spi_block) {
		LCDSPI_ERR("%s: spi_block is null\n", __func__);
		return -1;
	}
	if (!spi_block->raw_buf) {
		LCDSPI_ERR("%s: raw_buf is null\n", __func__);
		return -1;
	}

	switch (spi_block->data_type) {
	case LCD_TCON_DATA_BLOCK_TYPE_DEMURA_LUT:
	case LCD_TCON_DATA_BLOCK_TYPE_ACC_LUT:
		/* data convert after compare, for tcon data */
		ret = lcd_tconless_spi_data_conv_tcon(spi_block);
		break;
	case LCD_TCON_DATA_BLOCK_TYPE_EXT:
		/* data convert before compare, for pmu */
		ret = lcd_tconless_spi_data_conv_pmu(spi_block);
		break;
	default:
		break;
	}

	return ret;
}

/* note: all the tcon data buf size must align to 32byte */
void lcd_tconless_spi_data_probe(void)
{
	struct lcd_tcon_spi_s *tcon_spi = lcd_tcon_spi_get();

	tcon_spi->data_read = lcd_tconless_spi_data_read;
	tcon_spi->data_conv = lcd_tconless_spi_data_conv;
}
