/*
 * drivers/display/lcd/aml_lcd_tcon_spi.c
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
#include <asm/arch/io.h>
#include <amlogic/aml_lcd.h>
#include "aml_lcd_common.h"
#include "aml_lcd_tcon.h"

static struct lcd_tcon_spi_s tcon_spi = {
	.block_cnt = 0,
	.init_flag = 0,

	.spi_block = NULL,
	.ext_buf = NULL,

	.data_read = NULL,
	.data_conv = NULL,
};

struct lcd_tcon_spi_s *lcd_tcon_spi_get(void)
{
	return &tcon_spi;
}

static void lcd_tcon_spi_print(void)
{
	struct lcd_tcon_spi_block_s *spi_block;
	int i, j;

	if (tcon_spi.version == 0) {
		LCDPR("tcon_spi invalid for version 0\n");
		return;
	}

	printf("lcd_tcon_spi info:\n");
	printf("version           = %d\n", tcon_spi.version);
	printf("block_cnt         = %d\n", tcon_spi.block_cnt);
	printf("init_flag         = 0x%x\n", tcon_spi.init_flag);
	if (tcon_spi.init_flag == 0)
		return;
	for (i = 0; i < tcon_spi.block_cnt; i++) {
		spi_block = tcon_spi.spi_block[i];
		printf("spi_block %d:\n"
			"data_type       0x%02x\n"
			"data_index      %d\n"
			"data_flag       0x%08x\n"
			"spi_offset      0x%08x\n"
			"spi_size        0x%08x\n"
			"param_cnt       0x%08x\n",
			i, spi_block->data_type,
			spi_block->data_index, spi_block->data_flag,
			spi_block->spi_offset, spi_block->spi_size,
			spi_block->param_cnt);
		for (j = 0; j < spi_block->param_cnt; j++) {
			printf("param_%d         0x%08x\n",
				j, spi_block->param[j]);
		}
	}
	printf("\n");
}

#ifdef CONFIG_AML_LCD_EXTERN
static int lcd_tcon_spi_ext_update(struct aml_lcd_extern_driver_s *ext_drv)
{
	unsigned char *buf;
	unsigned int size, crc;

	if (!ext_drv) {
		LCDERR("%s: ext_drv is null\n", __func__);
		return -1;
	}

	buf = tcon_spi.ext_buf;

	/* write lcd_extern unifykey and driver init_on_table */
	memcpy(ext_drv->config->table_init_on,
		&buf[LCD_UKEY_EXT_INIT],
		tcon_spi.ext_init_on_cnt);
	ext_drv->config->table_init_on_cnt = tcon_spi.ext_init_on_cnt;

	/* update size & crc, then write to unifykey */
	size = LCD_UKEY_EXT_INIT + tcon_spi.ext_init_on_cnt +
		tcon_spi.ext_init_off_cnt;
	buf[4] = size & 0xff;
	buf[5] = (size >> 8) & 0xff;
	buf[6] = (size >> 16) & 0xff;
	buf[7] = (size >> 24) & 0xff;


	crc = (unsigned int)(crc32(0, &buf[4], (size - 4)));
	buf[0] = crc & 0xff;
	buf[1] = (crc >> 8) & 0xff;
	buf[2] = (crc >> 16) & 0xff;
	buf[3] = (crc >> 24) & 0xff;

	aml_lcd_unifykey_write("lcd_extern", buf, size);

	return 0;
}

static int lcd_tcon_spi_update_data(struct lcd_tcon_spi_block_s *spi_block,
				    unsigned char *cmp_buf, int flag, int i,
				    unsigned int offset, unsigned int data_len)
{
	if (flag) {
		if (spi_block->data_new_size > offset + data_len) {
			memcpy(&cmp_buf[i + 2], &spi_block->new_buf[offset],
			       data_len - 1);
			spi_block->new_buf[0] = data_len;
		}
	} else {
		memcpy(&cmp_buf[i + 1], spi_block->new_buf,
		       spi_block->data_new_size);
	}

	return 0;
}

/* for ext_data, need udpate cmd table when compare */
static int lcd_tcon_spi_ext_cmp(unsigned char index,
				struct lcd_tcon_spi_block_s *spi_block)
{
	unsigned char *cmp_buf, *tmp_buf;
	unsigned int cmp_buf_size, ext_size;
	unsigned char type, cnt;
	int i = 0, j = 0, k;
	unsigned int offset = 0, data_len = 0;
	int flag = 0;

	if (!spi_block->new_buf) {
		LCDERR("%s: new_buf is null\n", __func__);
		return -1;
	}
	if (!tcon_spi.ext_buf) {
		LCDERR("%s: ext_buf is null\n", __func__);
		return -1;
	}

	ext_size = LCD_EXTERN_INIT_ON_MAX + LCD_EXTERN_INIT_OFF_MAX;
	tmp_buf = (unsigned char *)malloc((ext_size * sizeof(unsigned char)));
	if (!tmp_buf) {
		LCDERR("%s: failed to alloc tmp_buf\n", __func__);
		return -1;
	}
	memset(tmp_buf, 0, (ext_size * sizeof(unsigned char)));

	cmp_buf = &tcon_spi.ext_buf[LCD_UKEY_EXT_INIT];
	cmp_buf_size = tcon_spi.ext_init_on_cnt;
	while ((i + 1) < cmp_buf_size) {
		type = cmp_buf[i];
		cnt = cmp_buf[i + 1];
		if (type == 0xff)
			break;
		if ((i + 2 + cnt) > cmp_buf_size)
			break;
		if ((((type >> 4) & 0xf) == 0xb) ||
		    (((type >> 4) & 0xf) == 0xd) ||
		    (((type >> 4) & 0xf) == 0xa)) {
			if (index != (type & 0xf))
				goto lcd_tcon_spi_ext_cmp_next;
			if (((type >> 4) & 0xf) == 0xa) {
				flag = 1;
				data_len = cmp_buf[i + 1];
				offset = cmp_buf[i + 2];
			} else {
				j = i + cnt + 2;
			}

			k = cmp_buf_size - j + tcon_spi.ext_init_off_cnt;
			if (cnt == 0) {
				if (spi_block->new_buf[0]) { /* new cnt */
					/* save data behind */
					memcpy(tmp_buf, &cmp_buf[j], k);
					/* update current data */
					lcd_tcon_spi_update_data(spi_block,
								 cmp_buf, flag,
								 i, offset,
								 data_len);
					/* recover data behind */
					j = i + spi_block->new_buf[0] + 2;
					memcpy(&cmp_buf[j], tmp_buf, k);
					tcon_spi.ext_init_on_cnt += spi_block->new_buf[0];
					goto lcd_tcon_spi_ext_cmp_diff;
				}
				goto lcd_tcon_spi_ext_cmp_no_diff;
			}
			if (memcmp(&cmp_buf[i + 2], spi_block->new_buf, spi_block->data_new_size)) {
				/* save data behind */
				memcpy(tmp_buf, &cmp_buf[j], k);
				/* update current data */
				lcd_tcon_spi_update_data(spi_block,
							 cmp_buf, flag,
							 i, offset,
							 data_len);
				/* recover data behind */
				j = i + spi_block->new_buf[0] + 2;
				memcpy(&cmp_buf[j], tmp_buf, k);
				tcon_spi.ext_init_on_cnt =
					tcon_spi.ext_init_on_cnt - cnt + spi_block->new_buf[0];
				goto lcd_tcon_spi_ext_cmp_diff;
			}
		}
lcd_tcon_spi_ext_cmp_next:
		i += (cnt + 2);
	}

lcd_tcon_spi_ext_cmp_no_diff:
	free(tmp_buf);
	return 0;

lcd_tcon_spi_ext_cmp_diff:
	free(tmp_buf);
	return -1;
}
#endif

static int lcd_tcon_spi_data_cmp(struct lcd_tcon_spi_block_s *spi_block,
				 unsigned char *cmp_buf)
{
	unsigned int raw_data_check;

	raw_data_check = cmp_buf[4] | (cmp_buf[5] << 8) |
			 (cmp_buf[6] << 16) | (cmp_buf[7] << 24);
	if (raw_data_check != spi_block->data_raw_check)
		return -1;

	return 0;
}

static int lcd_tcon_spi_data_load(void)
{
	struct tcon_mem_map_table_s *mm_table = get_lcd_tcon_mm_table();
#ifdef CONFIG_AML_LCD_EXTERN
	struct aml_lcd_extern_driver_s *ext_drv = NULL;
	unsigned int ext_index;
	unsigned int ext_need_update = 0;
#endif
	unsigned int i, j, size, new_size;
	int ret;

	if (tcon_spi.version == 0)
		return 0;

	if (mm_table->version == 0)
		return 0;

	if (!tcon_spi.spi_block) {
		LCDERR("%s: spi_block buf is null\n", __func__);
		return -1;
	}

	if (!tcon_spi.data_read) {
		LCDERR("%s: data_read is null\n", __func__);
		return -1;
	}
	if (!tcon_spi.data_conv) {
		LCDERR("%s: data_conv is null\n", __func__);
		return -1;
	}

	for (i = 0; i < tcon_spi.block_cnt; i++) {
		switch (tcon_spi.spi_block[i]->data_type) {
		case LCD_TCON_DATA_BLOCK_TYPE_DEMURA_LUT:
		case LCD_TCON_DATA_BLOCK_TYPE_ACC_LUT:
			if (!mm_table->data_mem_vaddr) {
				LCDERR("%s %d: data_mem error\n", __func__, i);
				continue;
			}
			ret = tcon_spi.data_read(tcon_spi.spi_block[i]);
			if (ret)
				continue;

			j = tcon_spi.spi_block[i]->data_index;

			/* update tcon data buf */
			if (!mm_table->data_mem_vaddr[j]) {
				/* no default bin file exist */
				ret = tcon_spi.data_conv(tcon_spi.spi_block[i]);
				if (ret)
					continue;
				if (!tcon_spi.spi_block[i]->new_buf) {
					LCDERR("%s: spi_block[%d] new_buf is null\n", __func__, i);
					continue;
				}
				/* note: all the tcon data buf size must align to 32byte */
				new_size = lcd_tcon_data_size_align(tcon_spi.spi_block[i]->data_new_size);
				mm_table->data_mem_vaddr[j] = (unsigned char *)malloc(new_size);
				if (!mm_table->data_mem_vaddr[j]) {
					LCDERR("%s: Not enough memory\n", __func__);
					continue;
				}
				memset(mm_table->data_mem_vaddr[j], 0, new_size);
				memcpy(mm_table->data_mem_vaddr[j], tcon_spi.spi_block[i]->new_buf,
				       tcon_spi.spi_block[i]->data_new_size);
			} else {
				ret = lcd_tcon_spi_data_cmp(tcon_spi.spi_block[i],
							    mm_table->data_mem_vaddr[j]);
				if (ret == 0)
					continue;

				ret = tcon_spi.data_conv(tcon_spi.spi_block[i]);
				if (ret) {
					free(mm_table->data_mem_vaddr[j]);
					mm_table->data_mem_vaddr[j] = NULL;
					LCDERR("%s: block_data[%d] disabled\n",
						__func__, i);
					continue;
				}
				if (!tcon_spi.spi_block[i]->new_buf) {
					LCDERR("%s: spi_block[%d] new_buf is null\n", __func__, i);
					continue;
				}
				size = mm_table->data_mem_vaddr[j][8] |
				       (mm_table->data_mem_vaddr[j][9] << 8) |
				       (mm_table->data_mem_vaddr[j][10] << 16) |
				       (mm_table->data_mem_vaddr[j][11] << 24);
				if (tcon_spi.spi_block[i]->data_new_size > size) {
					LCDERR("%s: block_data[%d] size is not match\n",
						__func__, i);
					continue;
				}
				new_size = lcd_tcon_data_size_align(size);
				memset(mm_table->data_mem_vaddr[j], 0, new_size);
				memcpy(mm_table->data_mem_vaddr[j], tcon_spi.spi_block[i]->new_buf,
				       tcon_spi.spi_block[i]->data_new_size);
			}
			break;
		case LCD_TCON_DATA_BLOCK_TYPE_EXT: /* pmu */
#ifdef CONFIG_AML_LCD_EXTERN
			if (!tcon_spi.ext_buf)
				break;
			ext_index = (tcon_spi.spi_block[i]->data_index >> 8) & 0xff;
			if (ext_drv) {
				if (ext_drv->config->index != ext_index) {
					LCDERR("%s: don't support multi ext device for tcon data\n",
					       __func__);
					continue;
				}
			} else {
				ext_drv = aml_lcd_extern_get_driver(ext_index);
				if (!ext_drv)
					break;
			}
			tcon_spi.ext_init_on_cnt = ext_drv->config->table_init_on_cnt;
			tcon_spi.ext_init_off_cnt = ext_drv->config->table_init_off_cnt;

			j = tcon_spi.spi_block[i]->data_index & 0xff;
			ret = tcon_spi.data_read(tcon_spi.spi_block[i]);
			if (ret)
				continue;
			ret = tcon_spi.data_conv(tcon_spi.spi_block[i]);
			if (ret)
				continue;
			if (!tcon_spi.spi_block[i]->new_buf) {
				LCDERR("%s: spi_block[%d] new_buf is null\n", __func__, i);
				continue;
			}
			ret = lcd_tcon_spi_ext_cmp(j, tcon_spi.spi_block[i]);
			if (ret)
				ext_need_update = 1;
#endif
			break;
		default:
			break;
		}
	}
#ifdef CONFIG_AML_LCD_EXTERN
	if (ext_need_update)
		lcd_tcon_spi_ext_update(ext_drv);
#endif

	for (i = 0; i < tcon_spi.block_cnt; i++) {
		if (tcon_spi.spi_block[i]->param) {
			free(tcon_spi.spi_block[i]->param);
			tcon_spi.spi_block[i]->param = NULL;
		}
		if (tcon_spi.spi_block[i]->raw_buf) {
			free(tcon_spi.spi_block[i]->raw_buf);
			tcon_spi.spi_block[i]->raw_buf = NULL;
		}
		if (tcon_spi.spi_block[i]->temp_buf) {
			free(tcon_spi.spi_block[i]->temp_buf);
			tcon_spi.spi_block[i]->temp_buf = NULL;
		}
		if (tcon_spi.spi_block[i]->new_buf) {
			free(tcon_spi.spi_block[i]->new_buf);
			tcon_spi.spi_block[i]->new_buf = NULL;
		}
	}

	return 0;
}

static int lcd_tcon_spi_data_parse(void)
{
	unsigned char *para, *p;
#ifdef CONFIG_AML_LCD_EXTERN
	unsigned int ext_size;
#ifdef CONFIG_CMD_INI
	unsigned char *data_buf = (unsigned char *)handle_lcd_ext_buf_get();
	unsigned int data_buf_size;
#endif
#endif
	struct aml_lcd_tcon_spi_unifykey_header_s spi_header;
	unsigned int i, j, n, block_size;
	int key_len, len, ret;

	if (tcon_spi.init_flag) /* already parsed */
		return 0;

	ret = aml_lcd_unifykey_check_exist("lcd_tcon_spi");
	if (ret)
		return -1;

	para = (unsigned char *)malloc(sizeof(unsigned char) * LCD_UKEY_TCON_SPI_SIZE);
	if (!para) {
		LCDERR("%s: Not enough memory\n", __func__);
		return -1;
	}
	key_len = LCD_UKEY_TCON_SPI_SIZE;
	memset(para, 0, (sizeof(unsigned char) * key_len));
	ret = aml_lcd_unifykey_get("lcd_tcon_spi", para, &key_len);
	if (ret)
		goto lcd_tcon_spi_data_parse_err0;

	/* check lcd_tcon_spi unifykey length */
	len = 10;
	ret = aml_lcd_unifykey_len_check(key_len, len);
	if (ret) {
		LCDERR("lcd_tcon_spi unifykey length is not correct\n");
		goto lcd_tcon_spi_data_parse_err0;
	}

	/* header: 16byte */
	memcpy(&spi_header, para, LCD_UKEY_TCON_SPI_HEAD_SIZE);
	if (lcd_debug_print_flag) {
		LCDPR("lcd_tcon_spi unifykey header:\n");
		LCDPR("crc32             = 0x%08x\n", spi_header.crc32);
		LCDPR("data_size         = %d\n", spi_header.data_size);
		LCDPR("version           = %d\n", spi_header.version);
		LCDPR("block_cnt         = %d\n", spi_header.block_cnt);
	}
	tcon_spi.version = spi_header.version;
	tcon_spi.block_cnt = spi_header.block_cnt;
	if (tcon_spi.version == 0) {
		free(para);
		return 0;
	}
	if (tcon_spi.block_cnt == 0) {
		LCDERR("%s: block_cnt 0, exit\n", __func__);
		free(para);
		return 0;
	}
	if (tcon_spi.block_cnt > LCD_UKEY_TCON_SPI_BLOCK_CNT_MAX) {
		LCDERR("%s: lcd_tcon_spi block_cnt %d out of support(max %d), limit to %d\n",
			__func__, tcon_spi.block_cnt,
			LCD_UKEY_TCON_SPI_BLOCK_CNT_MAX,
			LCD_UKEY_TCON_SPI_BLOCK_CNT_MAX);
		tcon_spi.block_cnt = LCD_UKEY_TCON_SPI_BLOCK_CNT_MAX;
	}

	len = LCD_UKEY_TCON_SPI_HEAD_SIZE + LCD_UKEY_TCON_SPI_BLOCK_SIZE_PRE;
	ret = aml_lcd_unifykey_len_check(key_len, len);
	if (ret) {
		LCDERR("lcd_tcon_spi unifykey length is not correct\n");
		goto lcd_tcon_spi_data_parse_err0;
	}

	tcon_spi.spi_block = (struct lcd_tcon_spi_block_s **)malloc(
		tcon_spi.block_cnt * sizeof(struct lcd_tcon_spi_block_s *));
	if (!tcon_spi.spi_block) {
		LCDERR("failed to alloc tcon_spi\n");
		goto lcd_tcon_spi_data_parse_err0;
	}
	memset(tcon_spi.spi_block, 0,
		(tcon_spi.block_cnt * sizeof(struct lcd_tcon_spi_block_s *)));

	len = LCD_UKEY_TCON_SPI_HEAD_SIZE;
	p = para + len;
#ifdef CONFIG_AML_LCD_EXTERN
	ext_size = LCD_UKEY_EXT_INIT + LCD_EXTERN_INIT_ON_MAX + LCD_EXTERN_INIT_OFF_MAX;
#endif
	for (i = 0; i < tcon_spi.block_cnt; i++) {
		tcon_spi.spi_block[i] = (struct lcd_tcon_spi_block_s *)malloc(
			sizeof(struct lcd_tcon_spi_block_s));
		if (!tcon_spi.spi_block[i]) {
			LCDERR("failed to alloc tcon_spi_block\n");
			for (j = 0; j < i; j++) {
				free(tcon_spi.spi_block[j]);
				tcon_spi.spi_block[j] = NULL;
			}
			goto lcd_tcon_spi_data_parse_err1;
		}
		memset(tcon_spi.spi_block[i], 0, sizeof(struct lcd_tcon_spi_block_s));
		memcpy(tcon_spi.spi_block[i], p, LCD_UKEY_TCON_SPI_BLOCK_SIZE_PRE);
		if (lcd_debug_print_flag) {
			LCDPR("lcd_tcon_spi block %d:\n", i);
			LCDPR("  data_type         = 0x%02x\n", tcon_spi.spi_block[i]->data_type);
			LCDPR("  data_index        = %d\n", tcon_spi.spi_block[i]->data_index);
			LCDPR("  data_flag         = %d\n", tcon_spi.spi_block[i]->data_flag);
			LCDPR("  spi_offset        = 0x%08x\n", tcon_spi.spi_block[i]->spi_offset);
			LCDPR("  spi_size          = 0x%08x\n", tcon_spi.spi_block[i]->spi_size);
			LCDPR("  param_cnt         = %d\n", tcon_spi.spi_block[i]->param_cnt);
		}

		block_size = LCD_UKEY_TCON_SPI_BLOCK_SIZE_PRE +
			     tcon_spi.spi_block[i]->param_cnt * 4;
		len += block_size;
		ret = aml_lcd_unifykey_len_check(key_len, len);
		if (ret) {
			LCDERR("lcd_tcon_spi unifykey length is incorrect\n");
			goto lcd_tcon_spi_data_parse_err0;
		}

		if (tcon_spi.spi_block[i]->param_cnt > 0) {
			tcon_spi.spi_block[i]->param = (unsigned int *)malloc(
				tcon_spi.spi_block[i]->param_cnt * sizeof(unsigned int));
			if (!tcon_spi.spi_block[i]->param) {
				LCDERR("failed to alloc spi_block[%d] param\n", i);
				for (j = 0; j <= i; j++) {
					free(tcon_spi.spi_block[j]);
					tcon_spi.spi_block[j] = NULL;
				}
				goto lcd_tcon_spi_data_parse_err1;
			}
			memset(tcon_spi.spi_block[i]->param, 0,
			       tcon_spi.spi_block[i]->param_cnt * sizeof(unsigned int));
			n = LCD_UKEY_TCON_SPI_BLOCK_SIZE_PRE;
			for (j = 0; j < tcon_spi.spi_block[i]->param_cnt; j++) {
				tcon_spi.spi_block[i]->param[j] = p[n] |
								  (p[n + 1] << 8) |
								  (p[n + 2] << 16) |
								  (p[n + 3] << 24);
				n += 4;
			}
		}

#ifdef CONFIG_AML_LCD_EXTERN
		if ((tcon_spi.spi_block[i]->data_type == LCD_TCON_DATA_BLOCK_TYPE_EXT) &&
			(!tcon_spi.ext_buf)) {
			tcon_spi.ext_buf = (unsigned char *)malloc(
				(ext_size * sizeof(unsigned char)));
			if (!tcon_spi.ext_buf) {
				LCDERR("failed to alloc ext_buf\n");
				for (j = 0; j <= i; j++) {
					free(tcon_spi.spi_block[j]->raw_buf);
					tcon_spi.spi_block[j]->raw_buf = NULL;
					if (tcon_spi.spi_block[j]->param) {
						free(tcon_spi.spi_block[j]->param);
						tcon_spi.spi_block[j]->param = NULL;
					}
					free(tcon_spi.spi_block[j]);
					tcon_spi.spi_block[j] = NULL;
				}
				goto lcd_tcon_spi_data_parse_err1;
			}
			memset(tcon_spi.ext_buf, 0, (ext_size * sizeof(unsigned char)));
#ifdef CONFIG_CMD_INI
			if (data_buf) {
				data_buf_size = data_buf[4] |
					(data_buf[5] << 8) |
					(data_buf[6] << 16) |
					(data_buf[7] << 24);
				memcpy(tcon_spi.ext_buf, data_buf, data_buf_size);
			}
#endif
		}
#endif
		p += block_size;
	}

	tcon_spi.init_flag = 1;

	free(para);
	return 0;

lcd_tcon_spi_data_parse_err1:
	free(tcon_spi.spi_block);
	tcon_spi.spi_block = NULL;
lcd_tcon_spi_data_parse_err0:
	free(para);
	return -1;
}

int lcd_tcon_spi_data_probe(struct aml_lcd_drv_s *lcd_drv)
{
	int ret;

	ret = lcd_tcon_spi_data_parse();
	if (ret)
		return -1;

	lcd_drv->lcd_tcon_spi_print = lcd_tcon_spi_print;
	lcd_drv->lcd_tcon_spi_data_load = lcd_tcon_spi_data_load;

	return 0;
}
