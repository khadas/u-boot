/*
 * drivers/display/lcd/aml_lcd_tcon.c
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
#include "aml_lcd_reg.h"
#include "aml_lcd_common.h"
#include "aml_lcd_tcon.h"
//#include "tcon_ceds.h"

#define TCON_IRQ_TIMEOUT_MAX    (1 << 17)
//static unsigned int tcon_irq_timeout;
//static unsigned int tcon_irq_cnt;
//static void lcd_tcon_p2p_chpi_irq(void);

static struct lcd_tcon_config_s *lcd_tcon_conf;
static struct tcon_rmem_s tcon_rmem;
static struct tcon_mem_map_table_s tcon_mm_table;

struct tcon_rmem_s *get_lcd_tcon_rmem(void)
{
	return &tcon_rmem;
}

struct tcon_mem_map_table_s *get_lcd_tcon_mm_table(void)
{
	return &tcon_mm_table;
}

static int lcd_tcon_valid_check(void)
{
	if (!lcd_tcon_conf) {
		LCDERR("invalid tcon data\n");
		return -1;
	}
	if (lcd_tcon_conf->tcon_valid == 0) {
		LCDERR("invalid tcon\n");
		return -1;
	}

	return 0;
}

static void lcd_tcon_od_check(unsigned char *table)
{
	unsigned int reg, bit;

	if (lcd_tcon_conf->reg_core_od == REG_LCD_TCON_MAX)
		return;

	reg = lcd_tcon_conf->reg_core_od;
	bit = lcd_tcon_conf->bit_od_en;
	if (((table[reg] >> bit) & 1) == 0)
		return;

	if (tcon_rmem.flag == 0) {
		table[reg] &= ~(1 << bit);
		LCDPR("%s: invalid memory, disable od function\n", __func__);
	}
}

unsigned int lcd_tcon_data_size_align(unsigned int size)
{
	unsigned int new_size;

	/* ready for burst 128bit */
	new_size = ((size + 15) / 16) * 16;

	return new_size;
}

unsigned char lcd_tcon_checksum(unsigned char *buf, unsigned int len)
{
	unsigned int temp = 0;
	unsigned int i;

	if (!buf)
		return 0;
	if (len == 0)
		return 0;
	for (i = 0; i < len; i++)
		temp += buf[i];

	return (unsigned char)(temp & 0xff);
}

unsigned char lcd_tcon_lrc(unsigned char *buf, unsigned int len)
{
	unsigned char temp = 0;
	unsigned int i;

	if (!buf)
		return 0xff;
	if (len == 0)
		return 0xff;
	temp = buf[0];
	for (i = 1; i < len; i++)
		temp = temp ^ buf[i];

	return temp;
}

static int lcd_tcon_vac_load(void)
{
	unsigned char *buff = tcon_mm_table.vac_mem_vaddr;
#ifdef CONFIG_CMD_INI
	unsigned int i, data_cnt = 0;
	unsigned char data_checksum, data_lrc, temp_checksum, temp_lrc;
#endif
	int ret = -1;

	if ((tcon_mm_table.vac_mem_size == 0) || (!buff))
		return -1;

#ifdef CONFIG_CMD_INI
	ret = handle_tcon_vac(buff, tcon_mm_table.vac_mem_size);
	if (ret) {
		LCDPR("%s: no vac data\n", __func__);
		return -1;
	}
	data_cnt = (buff[0] |
		(buff[1] << 8) |
		(buff[2] << 16) |
		(buff[3] << 24));
	if (data_cnt == 0) {
		LCDERR("%s: vac_data data_cnt error\n", __func__);
		return -1;
	}
	data_checksum = buff[4];
	data_lrc = buff[5];
	temp_checksum = lcd_tcon_checksum(&buff[8], data_cnt);
	temp_lrc = lcd_tcon_lrc(&buff[8], data_cnt);
	if (data_checksum != temp_checksum) {
		LCDERR("%s: vac_data checksum error\n", __func__);
		return -1;
	}
	if (data_lrc != temp_lrc) {
		LCDERR("%s: vac_data lrc error\n", __func__);
		return -1;
	}
	if ((buff[6] != 0x55) || (buff[7] != 0xaa)) {
		LCDERR("%s: vac_data pattern error\n", __func__);
		return -1;
	}

	if (lcd_debug_print_flag == 3) {
		for (i = 0; i < 30; i++)
			LCDPR("vac_data[%d]: 0x%02x\n", i, buff[i * 1]);
	}
#endif
	return ret;
}

static int lcd_tcon_demura_set_load(void)
{
	unsigned char *buff = tcon_mm_table.demura_set_mem_vaddr;
#ifdef CONFIG_CMD_INI
	unsigned int i, data_cnt = 0;
	unsigned char data_checksum, data_lrc, temp_checksum, temp_lrc;
#endif
	int ret = -1;

	if ((tcon_mm_table.demura_set_mem_size == 0) || (!buff))
		return -1;

#ifdef CONFIG_CMD_INI
	ret = handle_tcon_demura_set(buff, tcon_mm_table.demura_set_mem_size);
	if (ret) {
		LCDPR("%s: no demura_set data\n", __func__);
		return -1;
	}

	data_cnt = (buff[0] |
		(buff[1] << 8) |
		(buff[2] << 16) |
		(buff[3] << 24));
	if (data_cnt == 0) {
		LCDERR("%s: demura_set data_cnt error\n", __func__);
		return -1;
	}
	data_checksum = buff[4];
	data_lrc = buff[5];
	temp_checksum = lcd_tcon_checksum(&buff[8], data_cnt);
	temp_lrc = lcd_tcon_lrc(&buff[8], data_cnt);
	if (data_checksum != temp_checksum) {
		LCDERR("%s: demura_set checksum error\n", __func__);
		return -1;
	}
	if (data_lrc != temp_lrc) {
		LCDERR("%s: demura_set lrc error\n", __func__);
		return -1;
	}
	if ((buff[6] != 0x55) || (buff[7] != 0xaa)) {
		LCDERR("%s: demura_set pattern error\n", __func__);
		return -1;
	}

	if (lcd_debug_print_flag == 3) {
		for (i = 0; i < 100; i++)
			LCDPR("demura_set[%d]: 0x%x\n", i, buff[i]);
	}
#endif
	return ret;
}

static int lcd_tcon_demura_lut_load(void)
{
	unsigned char *buff = tcon_mm_table.demura_lut_mem_vaddr;
#ifdef CONFIG_CMD_INI
	unsigned int i, data_cnt = 0;
	unsigned char data_checksum, data_lrc, temp_checksum, temp_lrc;
#endif
	int ret = -1;

	if ((tcon_mm_table.demura_lut_mem_size == 0) || (!buff))
		return -1;

#ifdef CONFIG_CMD_INI
	ret = handle_tcon_demura_lut(buff, tcon_mm_table.demura_lut_mem_size);
	if (ret) {
		LCDPR("%s: no demura_lut data\n", __func__);
		return -1;
	}
	data_cnt = (buff[0] |
		(buff[1] << 8) |
		(buff[2] << 16) |
		(buff[3] << 24));
	if (data_cnt == 0) {
		LCDERR("%s: demura_lut data_cnt error\n", __func__);
		return -1;
	}
	data_checksum = buff[4];
	data_lrc = buff[5];
	temp_checksum = lcd_tcon_checksum(&buff[8], data_cnt);
	temp_lrc = lcd_tcon_lrc(&buff[8], data_cnt);
	if (data_checksum != temp_checksum) {
		LCDERR("%s: demura_lut checksum error\n", __func__);
		return -1;
	}
	if (data_lrc != temp_lrc) {
		LCDERR("%s: demura_lut lrc error\n", __func__);
		return -1;
	}
	if ((buff[6] != 0x55) || (buff[7] != 0xaa)) {
		LCDERR("%s: demura_lut pattern error\n", __func__);
		return -1;
	}

	if (lcd_debug_print_flag == 3) {
		for (i = 0; i < 100; i++)
			LCDPR("demura_lut[%d]: 0x%02x\n", i, buff[i]);
	}
#endif
	return ret;
}

static int lcd_tcon_acc_lut_load(void)
{
	unsigned char *buff = tcon_mm_table.acc_lut_mem_vaddr;
#ifdef CONFIG_CMD_INI
	unsigned int i, data_cnt = 0;
	unsigned char data_checksum, data_lrc, temp_checksum, temp_lrc;
#endif
	int ret = -1;

	if ((tcon_mm_table.acc_lut_mem_size == 0) || (!buff))
		return -1;

#ifdef CONFIG_CMD_INI
	ret = handle_tcon_acc_lut(buff, tcon_mm_table.acc_lut_mem_size);
	if (ret) {
		LCDPR("%s: no acc_lut data\n", __func__);
		return -1;
	}
	data_cnt = (buff[0] |
		(buff[1] << 8) |
		(buff[2] << 16) |
		(buff[3] << 24));
	if (data_cnt == 0) {
		LCDERR("%s: acc_lut data_cnt error\n", __func__);
		return -1;
	}
	data_checksum = buff[4];
	data_lrc = buff[5];
	temp_checksum = lcd_tcon_checksum(&buff[8], data_cnt);
	temp_lrc = lcd_tcon_lrc(&buff[8], data_cnt);
	if (data_checksum != temp_checksum) {
		LCDERR("%s: acc_lut checksum error\n", __func__);
		return -1;
	}
	if (data_lrc != temp_lrc) {
		LCDERR("%s: acc_lut lrc error\n", __func__);
		return -1;
	}
	if ((buff[6] != 0x55) || (buff[7] != 0xaa)) {
		LCDERR("%s: acc_lut pattern error\n", __func__);
		return -1;
	}

	if (lcd_debug_print_flag == 3) {
		for (i = 0; i < 100; i++)
			LCDPR("acc_lut[%d]: 0x%02x\n", i, buff[i]);
	}
#endif
	return ret;
}

static unsigned int lcd_tcon_reg_read(unsigned int addr, unsigned int flag)
{
	unsigned int val;
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return 0;

	if (flag)
		val = lcd_tcon_read_byte(addr + TCON_CORE_REG_START);
	else
		val = lcd_tcon_read(addr + TCON_CORE_REG_START);

	return val;
}

static void lcd_tcon_reg_write(unsigned int addr, unsigned int val, unsigned int flag)
{
	unsigned char temp;
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return;

	if (flag) {
		temp = (unsigned char)val;
		lcd_tcon_write_byte((addr + TCON_CORE_REG_START), temp);
	} else {
		lcd_tcon_write((addr + TCON_CORE_REG_START), val);
	}
}

static void lcd_tcon_core_reg_update(void)
{
	unsigned char *table;
	unsigned int len, temp;
	int i;

	len = lcd_tcon_conf->reg_table_len;
	table = lcd_tcon_conf->reg_table;
	if (table == NULL) {
		LCDERR("%s: table is NULL\n", __func__);
		return;
	}
	lcd_tcon_od_check(table);
	if (lcd_tcon_conf->core_reg_width == 8) {
		for (i = 0; i < len; i++)
			lcd_tcon_write_byte((i + TCON_CORE_REG_START), table[i]);
	} else {
		for (i = 0; i < len; i++)
			lcd_tcon_write((i + TCON_CORE_REG_START), table[i]);
	}
	LCDPR("tcon core regs update\n");

	if (lcd_tcon_conf->reg_core_od != REG_LCD_TCON_MAX) {
		i = lcd_tcon_conf->reg_core_od;
		if (lcd_tcon_conf->core_reg_width == 8)
			temp = lcd_tcon_read_byte(i + TCON_CORE_REG_START);
		else
			temp = lcd_tcon_read(i + TCON_CORE_REG_START);
		LCDPR("%s: tcon od reg readback: 0x%04x = 0x%04x\n",
			__func__, i, temp);
	}
}

static int lcd_tcon_top_set_txhd(void)
{
	LCDPR("lcd tcon top set\n");

	if (tcon_rmem.flag == 0) {
		LCDERR("%s: invalid axi mem\n", __func__);
	} else {
		lcd_tcon_write(TCON_AXI_OFST, tcon_rmem.mem_paddr);
		LCDPR("set tcon axi_mem addr: 0x%08x\n", tcon_rmem.mem_paddr);
	}

	lcd_tcon_write(TCON_CLK_CTRL, 0x001f);
	lcd_tcon_write(TCON_TOP_CTRL, 0x0199);
	lcd_tcon_write(TCON_RGB_IN_MUX, 0x24);
	lcd_tcon_write(TCON_PLLLOCK_CNTL, 0x0037);
	lcd_tcon_write(TCON_DDRIF_CTRL0, 0x33ff0004);
	lcd_tcon_write(TCON_RST_CTRL, 0x003f);
	lcd_tcon_write(TCON_RST_CTRL, 0x0000);

	return 0;
}

static int lcd_tcon_enable_txhd(struct lcd_config_s *pconf)
{
	struct mlvds_config_s *mlvds_conf;
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return -1;

	mlvds_conf = pconf->lcd_control.mlvds_config;
	if (mlvds_conf == NULL)
		return -1;

	/* reset apb for tcon */
	lcd_cbus_setb(RESET7_REGISTER, 1, 12, 1);
	udelay(100);

	/* step 1: tcon top */
	lcd_tcon_top_set_txhd();

	/* step 2: tcon_core_reg_update */
	lcd_tcon_core_reg_update();

	/* step 3: tcon_top_output_set */
	lcd_tcon_write(TCON_OUT_CH_SEL0, mlvds_conf->channel_sel0);
	lcd_tcon_write(TCON_OUT_CH_SEL1, mlvds_conf->channel_sel1);
	LCDPR("%s: set tcon ch_sel: 0x%08x, 0x%08x\n",
	      __func__, mlvds_conf->channel_sel0, mlvds_conf->channel_sel1);

	return 0;
}

static void lcd_tcon_vac_set_tl1(unsigned int demura_valid)
{
	int len, i, j, n;
	unsigned int d0, d1, temp, dly0, dly1, set2;
	unsigned char *buf = tcon_mm_table.vac_mem_vaddr;

	if (!tcon_mm_table.vac_mem_vaddr) {
		LCDERR("%s: vac_mem_vaddr is null\n", __func__);
		return;
	}

	n = 8;
	len = TCON_VAC_SET_PARAM_NUM;
	dly0 = buf[n];
	dly1 = buf[n + 2];
	set2 = buf[n + 4];

	n += (len * 2);
	if (lcd_debug_print_flag)
		LCDPR("vac_set:0x%x, 0x%x, 0x%x\n", dly0, dly1, set2);

	lcd_tcon_write_byte(0x0267, lcd_tcon_read_byte(0x0267) | 0xa0);
	/*vac_cntl, 12pipe delay temp for pre_dt*/
	lcd_tcon_write(0x2800, 0x807);
	if (demura_valid) /* vac delay with demura */
		lcd_tcon_write(0x2817, (0x1e | ((dly1 & 0xff) << 8)));
	else /* vac delay without demura */
		lcd_tcon_write(0x2817, (0x1e | ((dly0 & 0xff) << 8)));

	len = TCON_VAC_LUT_PARAM_NUM;
	if (lcd_debug_print_flag)
		LCDPR("%s: start write vac_ramt1~2\n", __func__);
	/*write vac_ramt1: 8bit, 256 regs*/
	for (i = 0; i < len; i++)
		lcd_tcon_write_byte(0xa100 + i, buf[n+i*2]);

	for (i = 0; i < len; i++)
		lcd_tcon_write_byte(0xa200 + i, buf[n+i*2]);

	/*write vac_ramt2: 8bit, 256 regs*/
	n += (len * 2);
	for (i = 0; i < len; i++)
		lcd_tcon_write_byte(0xa300 + i, buf[n+i*2]);

	for (i = 0; i < len; i++)
		lcd_tcon_write_byte(0xbc00 + i, buf[n+i*2]);

	if (lcd_debug_print_flag)
		LCDPR("%s: write vac_ramt1~2 ok\n", __func__);
	for (i = 0; i < len; i++)
		lcd_tcon_read_byte(0xbc00 + i);

	if (lcd_debug_print_flag)
		LCDPR("%s: start write vac_ramt3\n", __func__);
	/*write vac_ramt3_1~6: 24bit({data0[11:0],data1[11:0]},128 regs)*/
	for (j = 0; j < 6; j++) {
		n += (len * 2);
		for (i = 0; i < (len >> 1); i++) {
			d0 = (buf[n + (i * 4)] |
				(buf[n + (i * 4 + 1)] << 8)) & 0xfff;
			d1 = (buf[n + (i * 4 + 2)] |
				(buf[n + (i * 4 + 3)] << 8)) & 0xfff;
			temp = ((d0 << 12) | d1);
			lcd_tcon_write((0x2900 + i + (j * 128)), temp);
		}
	}
	if (lcd_debug_print_flag)
		LCDPR("%s: write vac_ramt3 ok\n", __func__);
	for (i = 0; i < ((len >> 1) * 6); i++)
		lcd_tcon_read(0x2900 + i);

	lcd_tcon_write(0x2801, 0x0f000870); /* vac_size */
	lcd_tcon_write(0x2802, (0x58e00d00 | (set2 & 0xff)));
	lcd_tcon_write(0x2803, 0x80400058);
	lcd_tcon_write(0x2804, 0x58804000);
	lcd_tcon_write(0x2805, 0x80400000);
	lcd_tcon_write(0x2806, 0xf080a032);
	lcd_tcon_write(0x2807, 0x4c08a864);
	lcd_tcon_write(0x2808, 0x10200000);
	lcd_tcon_write(0x2809, 0x18200000);
	lcd_tcon_write(0x280a, 0x18000004);
	lcd_tcon_write(0x280b, 0x735244c2);
	lcd_tcon_write(0x280c, 0x9682383d);
	lcd_tcon_write(0x280d, 0x96469449);
	lcd_tcon_write(0x280e, 0xaf363ce7);
	lcd_tcon_write(0x280f, 0xc71fbb56);
	lcd_tcon_write(0x2810, 0x953885a1);
	lcd_tcon_write(0x2811, 0x7a7a7900);
	lcd_tcon_write(0x2812, 0xc4640708);
	lcd_tcon_write(0x2813, 0x4b14b08a);
	lcd_tcon_write(0x2814, 0x4004b12c);
	lcd_tcon_write(0x2815, 0x0);
	/*vac_cntl,always read*/
	lcd_tcon_write(0x2800, 0x381f);

	LCDPR("tcon vac finish\n");
}

static int lcd_tcon_demura_set_tl1(void)
{
	unsigned char *data_buf;
	unsigned int data_cnt, i;

	if (!tcon_mm_table.demura_set_mem_vaddr) {
		LCDERR("%s: demura_set_mem_vaddr is null\n", __func__);
		return -1;
	}

	if (lcd_tcon_getb_byte(0x23d, 0, 1) == 0) {
		if (lcd_debug_print_flag)
			LCDPR("%s: demura function disabled\n", __func__);
		return 0;
	}

	data_cnt = (tcon_mm_table.demura_set_mem_vaddr[0] |
		(tcon_mm_table.demura_set_mem_vaddr[1] << 8) |
		(tcon_mm_table.demura_set_mem_vaddr[2] << 16) |
		(tcon_mm_table.demura_set_mem_vaddr[3] << 24));
	data_buf = &tcon_mm_table.demura_set_mem_vaddr[8];
	for (i = 0; i < data_cnt; i++)
		lcd_tcon_write_byte(0x186, data_buf[i]);

	LCDPR("tcon demura_set cnt %d\n", data_cnt);

	return 0;
}

static int lcd_tcon_demura_lut_tl1(void)
{
	unsigned char *data_buf;
	unsigned int data_cnt, i;

	if (!tcon_mm_table.demura_lut_mem_vaddr) {
		LCDERR("%s: demura_lut_mem_vaddr is null\n", __func__);
		return -1;
	}

	if (lcd_tcon_getb_byte(0x23d, 0, 1) == 0)
		return 0;

	/*disable demura when load lut data*/
	lcd_tcon_setb_byte(0x23d, 0, 0, 1);

	lcd_tcon_setb_byte(0x181, 1, 0, 1);
	lcd_tcon_write_byte(0x182, 0x01);
	lcd_tcon_write_byte(0x183, 0x86);
	lcd_tcon_write_byte(0x184, 0x01);
	lcd_tcon_write_byte(0x185, 0x87);

	data_cnt = (tcon_mm_table.demura_lut_mem_vaddr[0] |
		(tcon_mm_table.demura_lut_mem_vaddr[1] << 8) |
		(tcon_mm_table.demura_lut_mem_vaddr[2] << 16) |
		(tcon_mm_table.demura_lut_mem_vaddr[3] << 24));
	data_buf = &tcon_mm_table.demura_lut_mem_vaddr[8];
	/* fixed 2 byte 0 for border */
	lcd_tcon_write_byte(0x187, 0);
	lcd_tcon_write_byte(0x187, 0);
	for (i = 0; i < data_cnt; i++)
		lcd_tcon_write_byte(0x187, data_buf[i]);

	/*enable demura when load lut data finished*/
	lcd_tcon_setb_byte(0x23d, 1, 0, 1);

	LCDPR("tcon demura_lut cnt %d\n", data_cnt);
	if (lcd_debug_print_flag)
		LCDPR("tcon demura 0x23d = 0x%02x\n", lcd_tcon_read_byte(0x23d));

	return 0;
}

static int lcd_tcon_acc_lut_tl1(void)
{
	unsigned char *data_buf;
	unsigned int data_cnt, i;

	if (!tcon_mm_table.acc_lut_mem_vaddr) {
		LCDERR("%s: acc_lut_mem_vaddr is null\n", __func__);
		return -1;
	}

	/* enable lut access, disable gamma en*/
	lcd_tcon_setb_byte(0x262, 0x2, 0, 2);

	/* write gamma lut */
	data_cnt = (tcon_mm_table.acc_lut_mem_vaddr[0] |
		(tcon_mm_table.acc_lut_mem_vaddr[1] << 8) |
		(tcon_mm_table.acc_lut_mem_vaddr[2] << 16) |
		(tcon_mm_table.acc_lut_mem_vaddr[3] << 24));
	if (data_cnt > 1161) { /* 0xb50~0xfd8, 1161 */
		LCDPR("%s: data_cnt %d is invalid, force to 1161\n",
		      __func__, data_cnt);
		data_cnt = 1161;
	}

	data_buf = &tcon_mm_table.acc_lut_mem_vaddr[8];
	for (i = 0; i < data_cnt; i++)
		lcd_tcon_write_byte((0xb50 + i), data_buf[i]);

	/* enable gamma */
	lcd_tcon_setb_byte(0x262, 0x3, 0, 2);

	LCDPR("tcon acc_lut cnt %d\n", data_cnt);

	return 0;
}

static int lcd_tcon_data_common_parse_set(unsigned char *data_buf,
		struct lcd_tcon_data_block_header_s *block_header)
{
	unsigned char *p;
	unsigned short part_cnt;
	unsigned char part_type;
	unsigned int size, reg, data, mask, temp;
	struct lcd_tcon_data_block_ext_header_s *ext_header;
	union lcd_tcon_data_part_u data_part;
	unsigned int data_offset, offset, i, j, k, d, m, n, step = 0;
	int ret;

	p = data_buf + LCD_TCON_DATA_BLOCK_HEADER_SIZE;
	ext_header = (struct lcd_tcon_data_block_ext_header_s *)p;
	part_cnt = ext_header->part_cnt;
	if (lcd_debug_print_flag)
		LCDPR("%s: part_cnt: %d\n", __func__, part_cnt);

	data_offset = LCD_TCON_DATA_BLOCK_HEADER_SIZE + block_header->ext_header_size;
	size = 0;
	for (i = 0; i < part_cnt; i++) {
		p = data_buf + data_offset;
		part_type = p[LCD_TCON_DATA_PART_NAME_SIZE + 3];
		if (lcd_debug_print_flag) {
			LCDPR("%s: start step %d, %s, type=0x%02x\n",
				__func__, step, p, part_type);
		}
		switch (part_type) {
		case LCD_TCON_DATA_PART_TYPE_WR_N:
			data_part.wr_n = (struct lcd_tcon_data_part_wr_n_s *)p;
			offset = LCD_TCON_DATA_PART_WR_N_SIZE_PRE;
			size = offset + (data_part.wr_n->reg_cnt * data_part.wr_n->reg_addr_byte) +
				(data_part.wr_n->data_cnt * data_part.wr_n->reg_data_byte);
			if ((size + data_offset) > block_header->block_size) {
				LCDERR("%s: block %s data_part %d error\n",
					__func__, block_header->name, i);
				return -1;
			}
			m = offset; /* for reg */
			n = m + (data_part.wr_n->reg_cnt * data_part.wr_n->reg_addr_byte); /* for data */
			for (j = 0; j < data_part.wr_n->reg_cnt; j++) {
				reg = 0;
				for (d = 0; d < data_part.wr_n->reg_addr_byte; d++)
					reg |= (p[m + d] << (d * 8));
				if (data_part.wr_n->reg_inc) {
					for (k = 0; k < data_part.wr_n->data_cnt; k++) {
						data = 0;
						for (d = 0; d < data_part.wr_n->reg_data_byte; d++)
							data |= (p[n + d] << (d * 8));
						if (data_part.wr_n->reg_data_byte == 1)
							lcd_tcon_write_byte((reg + k), data);
						else
							lcd_tcon_write((reg + k), data);
						n += data_part.wr_n->reg_data_byte;
					}
				} else {
					for (k = 0; k < data_part.wr_n->data_cnt; k++) {
						data = 0;
						for (d = 0; d < data_part.wr_n->reg_data_byte; d++)
							data |= (p[n + d] << (d * 8));
						if (data_part.wr_n->reg_data_byte == 1)
							lcd_tcon_write_byte(reg, data);
						else
							lcd_tcon_write(reg, data);
						n += data_part.wr_n->reg_data_byte;
					}
				}
				m += data_part.wr_n->reg_addr_byte;
			}
			break;
		case LCD_TCON_DATA_PART_TYPE_WR_MASK:
			data_part.wr_mask = (struct lcd_tcon_data_part_wr_mask_s *)p;
			offset = LCD_TCON_DATA_PART_WR_MASK_SIZE_PRE;
			size = offset + data_part.wr_mask->reg_addr_byte + (2 * data_part.wr_mask->reg_data_byte);
			if ((size + data_offset) > block_header->block_size) {
				LCDERR("%s: block %s data_part %d error\n",
					__func__, block_header->name, i);
				return -1;
			}
			m = offset; /* for reg */
			n = m + data_part.wr_mask->reg_addr_byte; /* for data */
			reg = 0;
			for (d = 0; d < data_part.wr_mask->reg_addr_byte; d++)
				reg |= (p[m + d] << (d * 8));
			mask = 0;
			for (d = 0; d < data_part.wr_mask->reg_data_byte; d++)
				mask |= (p[n + d] << (d * 8));
			n += data_part.wr_mask->reg_data_byte;
			data = 0;
			for (d = 0; d < data_part.wr_mask->reg_data_byte; d++)
				data |= (p[n + d] << (d * 8));
			if (data_part.wr_mask->reg_data_byte == 1)
				lcd_tcon_update_bits_byte(reg, mask, data);
			else
				lcd_tcon_update_bits(reg, mask, data);
			break;
		case LCD_TCON_DATA_PART_TYPE_RD_MASK:
			data_part.rd_mask = (struct lcd_tcon_data_part_rd_mask_s *)p;
			offset = LCD_TCON_DATA_PART_RD_MASK_SIZE_PRE;
			size = offset + data_part.rd_mask->reg_addr_byte + data_part.rd_mask->reg_data_byte;
			if ((size + data_offset) > block_header->block_size) {
				LCDERR("%s: block %s data_part %d error\n",
					__func__, block_header->name, i);
				return -1;
			}
			m = offset; /* for reg */
			n = m + data_part.rd_mask->reg_addr_byte; /* for data */
			reg = 0;
			for (d = 0; d < data_part.rd_mask->reg_addr_byte; d++)
				reg |= (p[m + d] << (d * 8));
			mask = 0;
			for (d = 0; d < data_part.rd_mask->reg_data_byte; d++)
				mask |= (p[n + d] << (d * 8));
			if (data_part.rd_mask->reg_data_byte == 1) {
				data = lcd_tcon_read_byte(reg) & mask;
				if (lcd_debug_print_flag) {
					LCDPR("%s: read reg 0x%04x = 0x%02x, mask = 0x%02x\n",
					      __func__, reg, data, mask);
				}
			} else {
				data = lcd_tcon_read(reg) & mask;
				if (lcd_debug_print_flag) {
					LCDPR("%s: read reg 0x%04x = 0x%08x, mask = 0x%08x\n",
					      __func__, reg, data, mask);
				}
			}
			break;
		case LCD_TCON_DATA_PART_TYPE_CHK_WR_MASK:
			data_part.chk_wr_mask = (struct lcd_tcon_data_part_chk_wr_mask_s *)p;
			offset = LCD_TCON_DATA_PART_CHK_WR_MASK_SIZE_PRE;
			size = offset + data_part.chk_wr_mask->reg_chk_addr_byte +
				data_part.chk_wr_mask->reg_chk_data_byte * (data_part.chk_wr_mask->data_chk_cnt + 1) + //include mask
				data_part.chk_wr_mask->reg_wr_addr_byte +
				data_part.chk_wr_mask->reg_wr_data_byte * (data_part.chk_wr_mask->data_chk_cnt + 2); //include mask, default
			if ((size + data_offset) > block_header->block_size) {
				LCDERR("%s: block %s data_part %d error\n",
					__func__, block_header->name, i);
				return -1;
			}
			m = offset; /* for reg */
			n = m + data_part.chk_wr_mask->reg_chk_addr_byte; /* for data */
			reg = 0;
			for (d = 0; d < data_part.chk_wr_mask->reg_chk_addr_byte; d++)
				reg |= (p[m + d] << (d * 8));
			mask = 0;
			for (d = 0; d < data_part.chk_wr_mask->reg_chk_data_byte; d++)
				mask |= (p[n + d] << (d * 8));
			if (data_part.chk_wr_mask->reg_chk_data_byte == 1)
				temp = lcd_tcon_read_byte(reg) & mask;
			else
				temp = lcd_tcon_read(reg) & mask;
			n += data_part.chk_wr_mask->reg_chk_data_byte;
			for (j = 0; j < data_part.chk_wr_mask->data_chk_cnt; j++) {
				data = 0;
				for (d = 0; d < data_part.chk_wr_mask->reg_chk_data_byte; d++)
					data |= (p[n + d] << (d * 8));
				if ((data & mask) == temp)
					break;
				n += data_part.chk_wr_mask->reg_chk_data_byte;
			}
			k = j;

			m = offset + data_part.chk_wr_mask->reg_chk_addr_byte +
			    data_part.chk_wr_mask->reg_chk_data_byte * (data_part.chk_wr_mask->data_chk_cnt + 1); /* for reg */
			n = m + data_part.chk_wr_mask->reg_wr_addr_byte; /* for data */
			reg = 0;
			for (d = 0; d < data_part.chk_wr_mask->reg_wr_addr_byte; d++)
				reg |= (p[m + d] << (d * 8));
			mask = 0;
			for (d = 0; d < data_part.chk_wr_mask->reg_wr_data_byte; d++)
				mask |= (p[n + d] << (d * 8));
			n += data_part.chk_wr_mask->reg_wr_data_byte;
			n += data_part.chk_wr_mask->reg_wr_data_byte * k;
			data = 0;
			for (d = 0; d < data_part.chk_wr_mask->reg_wr_data_byte; d++)
				data |= (p[n + d] << (d * 8));
			if (data_part.chk_wr_mask->reg_wr_data_byte == 1)
				lcd_tcon_update_bits_byte(reg, mask, data);
			else
				lcd_tcon_update_bits(reg, mask, data);
			break;
		case LCD_TCON_DATA_PART_TYPE_CHK_EXIT:
			data_part.chk_exit = (struct lcd_tcon_data_part_chk_exit_s *)p;
			offset = LCD_TCON_DATA_PART_CHK_EXIT_SIZE_PRE;
			size = offset + data_part.chk_exit->reg_addr_byte + (2 * data_part.chk_exit->reg_data_byte);
			if ((size + data_offset) > block_header->block_size) {
				LCDERR("%s: block %s data_part %d error\n",
					__func__, block_header->name, i);
				return -1;
			}
			m = offset; /* for reg */
			n = m + data_part.chk_exit->reg_addr_byte; /* for data */
			reg = 0;
			for (d = 0; d < data_part.chk_exit->reg_addr_byte; d++)
				reg |= (p[m + d] << (d * 8));
			mask = 0;
			for (d = 0; d < data_part.chk_exit->reg_data_byte; d++)
				mask |= (p[n + d] << (d * 8));
			n += data_part.chk_exit->reg_data_byte;
			data = 0;
			for (d = 0; d < data_part.chk_exit->reg_data_byte; d++)
				data |= (p[n + d] << (d * 8));
			if (data_part.chk_exit->reg_data_byte == 1)
				ret = lcd_tcon_check_bits_byte(reg, mask, data);
			else
				ret = lcd_tcon_check_bits(reg, mask, data);
			if (ret) {
				LCDPR("%s: block %s data_part %d check exit\n",
					__func__, block_header->name, i);
				return 0;
			}
			break;
		case LCD_TCON_DATA_PART_TYPE_DELAY:
			data_part.delay = (struct lcd_tcon_data_part_delay_s *)p;
			size = LCD_TCON_DATA_PART_DELAY_SIZE;
			if ((size + data_offset) > block_header->block_size) {
				LCDERR("%s: block %s data_part %d error\n",
					__func__, block_header->name, i);
				return -1;
			}
			if (data_part.delay->delay_us)
				mdelay(data_part.delay->delay_us);
			break;
		case LCD_TCON_DATA_PART_TYPE_PARAM:
			data_part.param = (struct lcd_tcon_data_part_param_s *)p;
			offset = LCD_TCON_DATA_PART_PARAM_SIZE_PRE;
			size = offset + data_part.param->param_size;
			if ((size + data_offset) > block_header->block_size) {
				LCDERR("%s: block %s data_part %d error\n",
					__func__, block_header->name, i);
				return -1;
			}
			break;
		default:
			LCDERR("%s: unsupport part type 0x%02x\n",
				__func__, part_type);
			break;
		}
		if (lcd_debug_print_flag) {
			LCDPR("%s: end step %d, %s, type=0x%02x, size=%d\n",
				__func__, step, p, part_type, size);
		}
		data_offset += size;
		step++;
	}

	return 0;
}

static int lcd_tcon_data_set(void)
{
	struct lcd_tcon_data_block_header_s block_header;
	unsigned char *data_buf;
	unsigned char temp_checksum, temp_lrc;
	int i;

	if (!tcon_mm_table.data_mem_vaddr) {
		LCDERR("%s: data_mem error\n", __func__);
		return -1;
	}

	if (!tcon_mm_table.data_priority) {
		LCDERR("%s: data_priority is null\n", __func__);
		return -1;
	}

	for (i = 0; i < tcon_mm_table.block_cnt; i++) {
		if ((tcon_mm_table.data_priority[i].index >= tcon_mm_table.block_cnt) ||
			(tcon_mm_table.data_priority[i].priority == 0xff)) {
			LCDERR("%s: data index or priority is invalid\n", __func__);
			return -1;
		}
		data_buf = tcon_mm_table.data_mem_vaddr[tcon_mm_table.data_priority[i].index];
		if (!data_buf) {
			LCDERR("%s: data %d buf is null\n",
				__func__, tcon_mm_table.data_priority[i].index);
			return -1;
		}
		memcpy(&block_header, data_buf, LCD_TCON_DATA_BLOCK_HEADER_SIZE);
		temp_checksum = lcd_tcon_checksum(&data_buf[4], (block_header.block_size - 4));
		temp_lrc = lcd_tcon_lrc(&data_buf[4], (block_header.block_size - 4));
		if ((temp_checksum != block_header.checksum) ||
			(temp_lrc != block_header.lrc) ||
			(data_buf[2] != 0x55) ||
			(data_buf[3] != 0xaa)) {
			LCDERR("%s: block %d, %s data check error\n",
				__func__, tcon_mm_table.data_priority[i].index,
				block_header.name);
			continue;
		}

		if (lcd_debug_print_flag) {
			LCDPR("%s: block %d, %s, init_priority %d: block_size=0x%x, block_type=0x%02x(%s)\n",
				__func__, tcon_mm_table.data_priority[i].index,
				block_header.name,
				block_header.init_priority,
				block_header.block_size,
				block_header.block_type,
				block_header.name);
		}
		lcd_tcon_data_common_parse_set(data_buf, &block_header);
	}

	LCDPR("%s finish\n", __func__);
	return 0;
}

static int lcd_tcon_top_set_tl1(struct lcd_config_s *pconf)
{
	unsigned int axi_reg[3] = {0x200c, 0x2013, 0x2014};
	unsigned int addr[3] = {0, 0, 0};
	unsigned int size[3] = {4162560, 4162560, 1960440};
	int i;

	LCDPR("%s\n", __func__);

	if (tcon_rmem.flag == 0) {
		LCDERR("%s: invalid axi mem\n", __func__);
	} else {
		addr[0] = tcon_rmem.mem_paddr;
		addr[1] = addr[0] + size[0];
		addr[2] = addr[1] + size[1];
		for (i = 0; i < 3; i++) {
			lcd_tcon_write(axi_reg[i], addr[i]);
			LCDPR("set tcon axi_mem[%d]: 0x%08x\n", i, addr[i]);
		}
	}

	lcd_tcon_write(TCON_CLK_CTRL, 0x001f);
	if (pconf->lcd_basic.lcd_type == LCD_P2P) {
		switch (pconf->lcd_control.p2p_config->p2p_type) {
		case P2P_CHPI:
		case P2P_USIT:
			lcd_tcon_write(TCON_TOP_CTRL, 0x8199);
			break;
		default:
			lcd_tcon_write(TCON_TOP_CTRL, 0x8999);
			break;
		}
	} else {
		lcd_tcon_write(TCON_TOP_CTRL, 0x8999);
	}
	lcd_tcon_write(TCON_PLLLOCK_CNTL, 0x0037);
	lcd_tcon_write(TCON_RST_CTRL, 0x003f);
	lcd_tcon_write(TCON_RST_CTRL, 0x0000);
	lcd_tcon_write(TCON_DDRIF_CTRL0, 0x33fff000);
	lcd_tcon_write(TCON_DDRIF_CTRL1, 0x300300);

	return 0;
}

static int lcd_tcon_data_load(void)
{
	unsigned char *table;
#ifdef CONFIG_CMD_INI
	struct lcd_tcon_data_block_header_s block_header;
	struct tcon_data_priority_s *data_prio;
	int i, j, demura_cnt = 0;
#endif
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return -1;
	table = lcd_tcon_conf->reg_table;
	if (!table)
		return 0;

	if (tcon_mm_table.version == 0) {
		ret = lcd_tcon_vac_load();
		if (ret == 0)
			tcon_mm_table.valid_flag |= LCD_TCON_DATA_VALID_VAC;
		ret = lcd_tcon_demura_set_load();
		if (ret)  {
			table[0x178] = 0x38;
			table[0x17c] = 0x20;
			table[0x181] = 0x00;
			table[0x23d] &= ~(1 << 0);
		} else {
			ret = lcd_tcon_demura_lut_load();
			if (ret) {
				table[0x178] = 0x38;
				table[0x17c] = 0x20;
				table[0x181] = 0x00;
				table[0x23d] &= ~(1 << 0);
			} else {
				tcon_mm_table.valid_flag |= LCD_TCON_DATA_VALID_DEMURA;
			}
		}

		ret = lcd_tcon_acc_lut_load();
		if (ret == 0)
			tcon_mm_table.valid_flag |= LCD_TCON_DATA_VALID_ACC;
	} else {
		if (!tcon_mm_table.data_mem_vaddr) {
			LCDERR("%s: data_mem error\n", __func__);
			return -1;
		}
		if (!tcon_mm_table.data_priority) {
			LCDERR("%s: data_priority error\n", __func__);
			return -1;
		}
#ifdef CONFIG_CMD_INI
		data_prio = tcon_mm_table.data_priority;
		for (i = 0; i < tcon_mm_table.block_cnt; i++) {
			ret = handle_tcon_data_load(tcon_mm_table.data_mem_vaddr, i);
			if (ret)
				continue;

			memcpy(&block_header, tcon_mm_table.data_mem_vaddr[i],
				LCD_TCON_DATA_BLOCK_HEADER_SIZE);
			tcon_mm_table.valid_flag |= block_header.block_flag;
			if (block_header.block_flag == LCD_TCON_DATA_VALID_DEMURA)
				demura_cnt++;

			/* insertion sort for block data init_priority */
			data_prio[i].index = i;
			data_prio[i].priority = block_header.init_priority;
			if (i > 0) {
				j = i - 1;
				while (j >= 0) {
					if (block_header.init_priority > data_prio[j].priority)
						break;
					if (block_header.init_priority == data_prio[j].priority) {
						LCDERR("%s: block %d init_priority same as block %d\n",
							__func__,
							data_prio[i].index,
							data_prio[j].index);
						return -1;
					}
					data_prio[j + 1].index = data_prio[j].index;
					data_prio[j + 1].priority = data_prio[j].priority;
					j--;
				}
				data_prio[j + 1].index = i;
				data_prio[j + 1].priority = block_header.init_priority;
			}

			if (lcd_debug_print_flag) {
				LCDPR("%s %d: block_size=0x%x, block_type=0x%02x, name=%s, init_priority=%d\n",
					__func__, i,
					block_header.block_size,
					block_header.block_type,
					block_header.name,
					block_header.init_priority);
			}
		}

		/* specially check demura setting */
		if (demura_cnt < 2) {
			tcon_mm_table.valid_flag &= ~LCD_TCON_DATA_VALID_DEMURA;
			/* disable demura */
			table[0x178] = 0x38;
			table[0x17c] = 0x20;
			table[0x181] = 0x00;
			table[0x23d] &= ~(1 << 0);
		}
#endif
	}

	return 0;
}

static int lcd_tcon_enable_tl1(struct lcd_config_s *pconf)
{
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return -1;

	/* step 1: tcon top */
	lcd_tcon_top_set_tl1(pconf);

	/* step 2: tcon_core_reg_update */
	lcd_tcon_core_reg_update();
	if (pconf->lcd_basic.lcd_type == LCD_P2P) {
		switch (pconf->lcd_control.p2p_config->p2p_type) {
		case P2P_CHPI:
			lcd_phy_tcon_chpi_bbc_init_tl1(pconf);
			break;
		default:
			break;
		}
	}

	if (tcon_mm_table.valid_flag & LCD_TCON_DATA_VALID_DEMURA) {
		if (!tcon_mm_table.valid_flag & LCD_TCON_DATA_VALID_VAC) {
			/*enable gamma*/
			lcd_tcon_setb_byte(0x262, 0x3, 0, 2);
		}
	} else {
		/*enable gamma*/
		lcd_tcon_setb_byte(0x262, 0x3, 0, 2);
	}

	if (tcon_mm_table.version == 0) {
		if (tcon_mm_table.valid_flag & LCD_TCON_DATA_VALID_VAC) {
			if (tcon_mm_table.valid_flag & LCD_TCON_DATA_VALID_DEMURA)
				lcd_tcon_vac_set_tl1(1);
			else
				lcd_tcon_vac_set_tl1(0);
		}
		if (tcon_mm_table.valid_flag & LCD_TCON_DATA_VALID_DEMURA) {
			lcd_tcon_demura_set_tl1();
			lcd_tcon_demura_lut_tl1();
		}
		if (tcon_mm_table.valid_flag & LCD_TCON_DATA_VALID_ACC)
			lcd_tcon_acc_lut_tl1();
	} else {
		lcd_tcon_data_set();
	}

	/* step 3: tcon_top_output_set */
	lcd_tcon_write(TCON_OUT_CH_SEL1, 0xba98); /* out swap for ch8~11 */
	LCDPR("set tcon ch_sel: 0x%08x, 0x%08x\n",
		lcd_tcon_read(TCON_OUT_CH_SEL0),
		lcd_tcon_read(TCON_OUT_CH_SEL1));

	return 0;
}

static int lcd_tcon_bin_path_resv_mem_set(void)
{
	unsigned char *mem_vaddr;
	unsigned int data_size;

	mem_vaddr = (unsigned char *)(unsigned long)(tcon_rmem.bin_path_mem_paddr);
	if (!tcon_mm_table.bin_path_mem_vaddr) {
		LCDERR("%s: bin_path buf invalid\n", __func__);
		return -1;
	}
	data_size = tcon_mm_table.bin_path_mem_vaddr[4] |
		(tcon_mm_table.bin_path_mem_vaddr[5] << 8) |
		(tcon_mm_table.bin_path_mem_vaddr[6] << 16) |
		(tcon_mm_table.bin_path_mem_vaddr[7] << 24);
	memcpy(mem_vaddr, tcon_mm_table.bin_path_mem_vaddr, data_size);

	return 0;
}

static int lcd_tcon_bin_path_update(unsigned int size)
{
#ifdef CONFIG_CMD_INI
	unsigned char *mem_vaddr;
	unsigned int data_size;
	unsigned char data_checksum, data_lrc, temp_checksum, temp_lrc;

	tcon_mm_table.bin_path_mem_vaddr = handle_tcon_path_mem_get(size);
	if (!tcon_mm_table.bin_path_mem_vaddr) {
		LCDERR("%s: get mem error\n", __func__);
		return -1;
	}
	mem_vaddr = tcon_mm_table.bin_path_mem_vaddr;
	data_size = mem_vaddr[4] |
		(mem_vaddr[5] << 8) |
		(mem_vaddr[6] << 16) |
		(mem_vaddr[7] << 24);
	if (data_size < 4) {
		LCDERR("%s: tcon_bin_path data_size error\n", __func__);
		return -1;
	}
	data_checksum = mem_vaddr[0];
	data_lrc = mem_vaddr[1];
	temp_checksum = lcd_tcon_checksum(&mem_vaddr[4], (data_size - 4));
	temp_lrc = lcd_tcon_lrc(&mem_vaddr[4], (data_size - 4));
	if (data_checksum != temp_checksum) {
		LCDERR("%s: tcon_bin_path checksum error\n", __func__);
		return -1;
	}
	if (data_lrc != temp_lrc) {
		LCDERR("%s: tcon_bin_path lrc error\n", __func__);
		return -1;
	}
	if ((mem_vaddr[2] != 0x55) || (mem_vaddr[3] != 0xaa)) {
		LCDERR("%s: tcon_bin_path pattern error\n", __func__);
		return -1;
	}

	tcon_mm_table.version = mem_vaddr[8] |
		(mem_vaddr[9] << 8) |
		(mem_vaddr[10] << 16) |
		(mem_vaddr[11] << 24);
	tcon_mm_table.data_load_level = mem_vaddr[12] |
		(mem_vaddr[13] << 8) |
		(mem_vaddr[14] << 16) |
		(mem_vaddr[15] << 24);
	tcon_mm_table.block_cnt = mem_vaddr[16] |
		(mem_vaddr[17] << 8) |
		(mem_vaddr[18] << 16) |
		(mem_vaddr[19] << 24);
#endif

	return 0;
}

static int lcd_tcon_mm_table_config_v0(void)
{
	unsigned int max_size;

	/* reserved memory */
	max_size = lcd_tcon_conf->axi_size +
		lcd_tcon_conf->bin_path_size +
		lcd_tcon_conf->vac_size +
		lcd_tcon_conf->demura_set_size +
		lcd_tcon_conf->demura_lut_size +
		lcd_tcon_conf->acc_lut_size;
	if (tcon_rmem.mem_size < max_size) {
		LCDERR("%s: tcon mem size 0x%x is not enough, need 0x%x\n",
			__func__, tcon_rmem.mem_size, max_size);
		return -1;
	}

	tcon_mm_table.vac_mem_size = lcd_tcon_conf->vac_size;
	tcon_rmem.vac_mem_paddr =
		tcon_rmem.bin_path_mem_paddr + tcon_rmem.bin_path_mem_size;
	tcon_mm_table.vac_mem_vaddr =
		(unsigned char *)(unsigned long)(tcon_rmem.vac_mem_paddr);
	if (lcd_debug_print_flag && (tcon_mm_table.vac_mem_size > 0))
		LCDPR("tcon vac paddr: 0x%08x, size: 0x%x\n",
		      tcon_rmem.vac_mem_paddr,
		      tcon_mm_table.vac_mem_size);

	tcon_mm_table.demura_set_mem_size = lcd_tcon_conf->demura_set_size;
	tcon_rmem.demura_set_paddr =
		tcon_rmem.vac_mem_paddr + tcon_mm_table.vac_mem_size;
	tcon_mm_table.demura_set_mem_vaddr =
		(unsigned char *)(unsigned long)(tcon_rmem.demura_set_paddr);
	if (lcd_debug_print_flag && (tcon_mm_table.demura_set_mem_size > 0))
		LCDPR("tcon demura set_paddr: 0x%08x, size: 0x%x\n",
		      tcon_rmem.demura_set_paddr,
		      tcon_mm_table.demura_set_mem_size);

	tcon_mm_table.demura_lut_mem_size = lcd_tcon_conf->demura_lut_size;
	tcon_rmem.demura_lut_paddr =
		tcon_rmem.demura_set_paddr + tcon_mm_table.demura_set_mem_size;
	tcon_mm_table.demura_lut_mem_vaddr =
		(unsigned char *)(unsigned long)(tcon_rmem.demura_lut_paddr);
	if (lcd_debug_print_flag && (tcon_mm_table.demura_lut_mem_size > 0))
		LCDPR("tcon demura lut_paddr: 0x%08x, size: 0x%x\n",
		      tcon_rmem.demura_lut_paddr,
		      tcon_mm_table.demura_lut_mem_size);

	tcon_mm_table.acc_lut_mem_size = lcd_tcon_conf->acc_lut_size;
	tcon_rmem.acc_lut_paddr =
		tcon_rmem.demura_lut_paddr + tcon_mm_table.demura_lut_mem_size;
	tcon_mm_table.acc_lut_mem_vaddr =
		(unsigned char *)(unsigned long)(tcon_rmem.acc_lut_paddr);
	if (lcd_debug_print_flag && (tcon_mm_table.acc_lut_mem_size > 0))
		LCDPR("tcon acc lut_paddr: 0x%08x, size: 0x%x\n",
		      tcon_rmem.acc_lut_paddr,
		      tcon_mm_table.acc_lut_mem_size);

	return 0;
}

static int lcd_tcon_mm_table_config_v1(void)
{
	if (tcon_mm_table.block_cnt > 32) {
		LCDERR("%s: tcon data block_cnt %d invalid\n",
			__func__, tcon_mm_table.block_cnt);
		return -1;
	}

	if (tcon_mm_table.data_mem_vaddr)
		return 0;
	if (tcon_mm_table.block_cnt == 0) {
		if (lcd_debug_print_flag)
			LCDPR("%s: block_cnt is zero\n", __func__);
		return 0;
	}

	tcon_mm_table.data_mem_vaddr = (unsigned char **)malloc(
		tcon_mm_table.block_cnt * sizeof(unsigned char *));
	if (!tcon_mm_table.data_mem_vaddr) {
		LCDERR("%s: Not enough memory\n", __func__);
		return -1;
	}
	memset(tcon_mm_table.data_mem_vaddr, 0,
		tcon_mm_table.block_cnt * sizeof(unsigned char *));

	tcon_mm_table.data_priority = (struct tcon_data_priority_s *)malloc(
		tcon_mm_table.block_cnt * sizeof(struct tcon_data_priority_s));
	if (!tcon_mm_table.data_priority) {
		LCDERR("%s: Not enough memory\n", __func__);
		return -1;
	}
	memset(tcon_mm_table.data_priority, 0xff,
		tcon_mm_table.block_cnt * sizeof(struct tcon_data_priority_s));

	return 0;
}

static int lcd_tcon_mem_config(void)
{
	unsigned char *mem_vaddr;
	unsigned int max_size;
	int ret;

	/* reserved memory */
	max_size = lcd_tcon_conf->axi_size +
		lcd_tcon_conf->bin_path_size;
	if (tcon_rmem.mem_size < max_size) {
		LCDERR("%s: tcon mem size 0x%x is not enough, need 0x%x\n",
			__func__, tcon_rmem.mem_size, max_size);
		return -1;
	}

	tcon_rmem.axi_mem_size = lcd_tcon_conf->axi_size;
	tcon_rmem.axi_mem_paddr = tcon_rmem.mem_paddr;
	if (lcd_debug_print_flag)
		LCDPR("tcon axi paddr: 0x%08x, size: 0x%x\n",
		      tcon_rmem.axi_mem_paddr, tcon_rmem.axi_mem_size);

	tcon_rmem.bin_path_mem_size = lcd_tcon_conf->bin_path_size;
	tcon_rmem.bin_path_mem_paddr =
		tcon_rmem.axi_mem_paddr + tcon_rmem.axi_mem_size;
	if (lcd_debug_print_flag && (tcon_rmem.bin_path_mem_size > 0))
		LCDPR("tcon bin_path paddr: 0x%08x, size: 0x%x\n",
		      tcon_rmem.bin_path_mem_paddr, tcon_rmem.bin_path_mem_size);

	/* default clear tcon rmem */
	mem_vaddr = (unsigned char *)(unsigned long)(tcon_rmem.mem_paddr);
	memset(mem_vaddr, 0, tcon_rmem.mem_size);

	ret = lcd_tcon_bin_path_update(tcon_rmem.bin_path_mem_size);
	if (ret)
		return -1;

	/* allocated memory, memory map table config */
	if (lcd_debug_print_flag)
		LCDPR("tcon mm_table version: %d\n", tcon_mm_table.version);
	if (tcon_mm_table.version == 0)
		ret = lcd_tcon_mm_table_config_v0();
	else
		ret = lcd_tcon_mm_table_config_v1();

	return ret;
}

static void lcd_tcon_config_axi_offset_default(void)
{

	tcon_rmem.mem_paddr = getenv_ulong("tcon_mem_addr", 16, 0);
	if (tcon_rmem.mem_paddr) {
		tcon_rmem.mem_size = lcd_tcon_conf->rsv_mem_size;
		LCDPR("get lcd_tcon mem_addr from default\n");
	} else {
		LCDERR("can't find env tcon_mem_addr\n");
	}
}

static int lcd_tcon_config(char *dt_addr, struct lcd_config_s *pconf, int load_id)
{
	int key_len, reg_len, size;
	int parent_offset;
	char *propdata;
	unsigned int mem_size;
	int ret;

	if (load_id & 0x1) {
		parent_offset = fdt_path_offset(dt_addr, "/reserved-memory");
		size = fdt_address_cells(dt_addr, parent_offset);
		parent_offset = fdt_path_offset(dt_addr, "/reserved-memory/linux,lcd_tcon");
		if (parent_offset < 0) {
			LCDERR("can't find node: /reserved-memory/linux,lcd_tcon\n");
		} else {
			propdata = (char *)fdt_getprop(dt_addr, parent_offset,
				"alloc-ranges", NULL);
			if (propdata == NULL) {
				LCDERR("failed to get lcd_tcon reserved-memory from dts\n");
				lcd_tcon_config_axi_offset_default();
			} else {
				if (size == 2)
					tcon_rmem.mem_paddr = be32_to_cpup((((u32*)propdata)+1));
				else
					tcon_rmem.mem_paddr = be32_to_cpup(((u32*)propdata));
			}

			propdata = (char *)fdt_getprop(dt_addr, parent_offset,
				"size", NULL);
			if (propdata == NULL) {
				LCDERR("failed to get tcon size from dts\n");
				lcd_tcon_config_axi_offset_default();
			} else {
				if (size == 2)
					mem_size = be32_to_cpup((((u32 *)propdata)+1));
				else
					mem_size = be32_to_cpup(((u32 *)propdata));

				if (mem_size < lcd_tcon_conf->rsv_mem_size) {
					LCDERR("tcon mem_size is not enough\n");
					tcon_rmem.mem_paddr = 0;
					tcon_rmem.flag = 0;
				} else {
					tcon_rmem.mem_size = lcd_tcon_conf->rsv_mem_size;
				}
			}
		}
	} else {
		lcd_tcon_config_axi_offset_default();
	}
	if (tcon_rmem.mem_paddr) {
		tcon_rmem.flag = 1;
		lcd_tcon_mem_config();
		LCDPR("tcon: axi mem addr: 0x%x\n", tcon_rmem.mem_paddr);
	}

#if 1
	/* get reg table from unifykey */
	reg_len = lcd_tcon_conf->reg_table_len;
	if (lcd_tcon_conf->reg_table == NULL) {
		lcd_tcon_conf->reg_table =
			(unsigned char *)malloc(sizeof(unsigned char) * reg_len);
		if (!lcd_tcon_conf->reg_table) {
			LCDERR("%s: Not enough memory\n", __func__);
			return -1;
		}
	}
	memset(lcd_tcon_conf->reg_table, 0, (sizeof(unsigned char) * reg_len));
	key_len = reg_len;
	ret = aml_lcd_unifykey_get_no_header("lcd_tcon",
		lcd_tcon_conf->reg_table, &key_len);
	if (ret) {
		free(lcd_tcon_conf->reg_table);
		lcd_tcon_conf->reg_table = NULL;
		LCDERR("%s: !!!!!!!!tcon unifykey load error!!!!!!!!\n", __func__);
		return -1;
	}
	if (key_len != reg_len) {
		free(lcd_tcon_conf->reg_table);
		lcd_tcon_conf->reg_table = NULL;
		LCDERR("%s: !!!!!!!!tcon unifykey load length error!!!!!!!!\n", __func__);
		return -1;
	}
	LCDPR("tcon: load unifykey len: %d\n", key_len);
#else
	reg_len = lcd_tcon_conf->reg_table_len;
	if (lcd_tcon_conf->reg_table == NULL)
		lcd_tcon_conf->reg_table = uhd_tcon_setting_ceds_h10;
	key_len = sizeof(uhd_tcon_setting_ceds_h10)/sizeof(unsigned char);
	if (key_len != reg_len) {
		free(lcd_tcon_conf->reg_table);
		lcd_tcon_conf->reg_table = NULL;
		LCDERR("%s: !!!!!!!!tcon unifykey load length error!!!!!!!!\n",
			__func__);
		return -1;
	}
	LCDPR("tcon: load default table len: %d\n", key_len);
#endif

	lcd_tcon_data_load();

	return 0;
}

static int lcd_tcon_core_flag(struct aml_lcd_drv_s *lcd_drv)
{
	int ret = 0;

	switch (lcd_drv->chip_type) {
	case LCD_CHIP_TL1:
	case LCD_CHIP_TM2:
	//case LCD_CHIP_T5:
		ret = (readl(TCON_CORE_FLAG_LIC2) >> 17) & 0x1;
		break;
	default:
		break;
	}

	return ret;
}

/* **********************************
 * tcon function api
 * **********************************
 */
#define PR_BUF_MAX    200
static void lcd_tcon_reg_table_print(void)
{
	int i, j, n, cnt;
	char *buf;
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return;

	if (lcd_tcon_conf->reg_table == NULL) {
		LCDERR("%s: reg_table is null\n", __func__);
		return;
	}

	buf = (char *)malloc(PR_BUF_MAX * sizeof(char));
	if (buf == NULL) {
		LCDERR("%s: buf malloc error\n", __func__);
		return;
	}

	LCDPR("%s:\n", __func__);
	cnt = lcd_tcon_conf->reg_table_len;
	for (i = 0; i < cnt; i += 16) {
		n = snprintf(buf, PR_BUF_MAX, "0x%04x: ", i);
		for (j = 0; j < 16; j++) {
			if ((i + j) >= cnt)
				break;
			n += snprintf(buf+n, PR_BUF_MAX, " 0x%02x",
				lcd_tcon_conf->reg_table[i+j]);
		}
		buf[n] = '\0';
		printf("%s\n", buf);
	}
	free(buf);
}

static void lcd_tcon_reg_readback_print(void)
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
	cnt = lcd_tcon_conf->reg_table_len;
	for (i = 0; i < cnt; i += 16) {
		n = snprintf(buf, PR_BUF_MAX, "0x%04x: ", i);
		for (j = 0; j < 16; j++) {
			if ((i + j) >= cnt)
				break;
			if (lcd_tcon_conf->core_reg_width == 8) {
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

static unsigned char lcd_tcon_table_read(unsigned int addr)
{
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return 0;

	if (!lcd_tcon_conf->reg_table) {
		LCDERR("tcon reg_table is null\n");
		return 0;
	}

	if (addr >= lcd_tcon_conf->reg_table_len) {
		LCDERR("invalid tcon reg_table addr: 0x%04x\n", addr);
		return 0;
	}

	return lcd_tcon_conf->reg_table[addr];
}

static unsigned char lcd_tcon_table_write(unsigned int addr, unsigned char val)
{
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return 0;

	if (!lcd_tcon_conf->reg_table) {
		LCDERR("tcon reg_table is null\n");
		return 0;
	}

	if (addr >= lcd_tcon_conf->reg_table_len) {
		LCDERR("invalid tcon reg_table addr: 0x%04x\n", addr);
		return 0;
	}

	lcd_tcon_conf->reg_table[addr] = val;
	return lcd_tcon_conf->reg_table[addr];
}

static void lcd_tcon_vac_print(void)
{
	int i, j, n, size;
	char *buf;
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return;

	if (tcon_mm_table.version) {
		LCDERR("%s: mem map version invalid\n", __func__);
		return;
	}

	if ((tcon_mm_table.valid_flag & LCD_TCON_DATA_VALID_VAC) == 0) {
		LCDERR("%s: vac invalid\n", __func__);
		return;
	}

	buf = (char *)malloc(PR_BUF_MAX * sizeof(char));
	if (buf == NULL) {
		LCDERR("%s: buf malloc error\n", __func__);
		return;
	}

	printf("%s:\n", __func__);
	size = tcon_mm_table.vac_mem_vaddr[0] |
		(tcon_mm_table.vac_mem_vaddr[1] << 8) |
		(tcon_mm_table.vac_mem_vaddr[2] << 16) |
		(tcon_mm_table.vac_mem_vaddr[3] << 24);
	size += 8; /* header for data_cnt & crc */
	for (i = 0; i < size; i += 16) {
		n = snprintf(buf, PR_BUF_MAX, "0x%04x: ", i);
		for (j = 0; j < 16; j++) {
			if ((i + j) >= size)
				break;
			n += snprintf(buf+n, PR_BUF_MAX, " %02x",
				tcon_mm_table.vac_mem_vaddr[i+j]);
		}
		buf[n] = '\0';
		printf("%s\n", buf);
	}

	free(buf);
}

static void lcd_tcon_demura_print(void)
{
	int i, j, n, size;
	char *buf;
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return;

	if (tcon_mm_table.version) {
		LCDERR("%s: mem map version invalid\n", __func__);
		return;
	}

	if ((tcon_mm_table.valid_flag & LCD_TCON_DATA_VALID_DEMURA) == 0) {
		LCDERR("%s: demura invalid\n", __func__);
		return;
	}

	buf = (char *)malloc(PR_BUF_MAX * sizeof(char));
	if (buf == NULL) {
		LCDERR("%s: buf malloc error\n", __func__);
		return;
	}

	printf("%s: demura_set:\n", __func__);
	size = tcon_mm_table.demura_set_mem_vaddr[0] |
		(tcon_mm_table.demura_set_mem_vaddr[1] << 8) |
		(tcon_mm_table.demura_set_mem_vaddr[2] << 16) |
		(tcon_mm_table.demura_set_mem_vaddr[3] << 24);
	size += 8; /* header for data_cnt & crc */
	for (i = 0; i < size; i += 16) {
		n = snprintf(buf, PR_BUF_MAX, "0x%04x: ", i);
		for (j = 0; j < 16; j++) {
			if ((i + j) >= size)
				break;
			n += snprintf(buf+n, PR_BUF_MAX, " %02x",
				tcon_mm_table.demura_set_mem_vaddr[i+j]);
		}
		buf[n] = '\0';
		printf("%s\n", buf);
	}

	printf("\n%s: demura_lut:\n", __func__);
	size = tcon_mm_table.demura_lut_mem_vaddr[0] |
		(tcon_mm_table.demura_lut_mem_vaddr[1] << 8) |
		(tcon_mm_table.demura_lut_mem_vaddr[2] << 16) |
		(tcon_mm_table.demura_lut_mem_vaddr[3] << 24);
	size += 8; /* header for data_cnt & crc */
	for (i = 0; i < size; i += 16) {
		n = snprintf(buf, PR_BUF_MAX, "0x%04x: ", i);
		for (j = 0; j < 16; j++) {
			if ((i + j) >= size)
				break;
			n += snprintf(buf+n, PR_BUF_MAX, " %02x",
				tcon_mm_table.demura_lut_mem_vaddr[i+j]);
		}
		buf[n] = '\0';
		printf("%s\n", buf);
	}

	free(buf);
}

static void lcd_tcon_acc_print(void)
{
	int i, j, n, size;
	char *buf;
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return;

	if (tcon_mm_table.version) {
		LCDERR("%s: mem map version invalid\n", __func__);
		return;
	}

	if ((tcon_mm_table.valid_flag & LCD_TCON_DATA_VALID_ACC) == 0) {
		LCDERR("%s: acc_lut invalid\n", __func__);
		return;
	}

	buf = (char *)malloc(PR_BUF_MAX * sizeof(char));
	if (buf == NULL) {
		LCDERR("%s: buf malloc error\n", __func__);
		return;
	}

	LCDPR("%s\n", __func__);
	size = tcon_mm_table.acc_lut_mem_vaddr[0] |
		(tcon_mm_table.acc_lut_mem_vaddr[1] << 8) |
		(tcon_mm_table.acc_lut_mem_vaddr[2] << 16) |
		(tcon_mm_table.acc_lut_mem_vaddr[3] << 24);
	size += 8; /* header for data_cnt & crc */
	for (i = 0; i < size; i += 16) {
		n = snprintf(buf, PR_BUF_MAX, "0x%04x: ", i);
		for (j = 0; j < 16; j++) {
			if ((i + j) >= size)
				break;
			n += snprintf(buf+n, PR_BUF_MAX, " %02x",
				tcon_mm_table.acc_lut_mem_vaddr[i+j]);
		}
		buf[n] = '\0';
		printf("%s\n", buf);
	}

	free(buf);
}

static void lcd_tcon_data_block_print(char *buf, unsigned char *data_mem)
{
	int i, j, n, size;

	size = data_mem[8] |
		(data_mem[9] << 8) |
		(data_mem[10] << 16) |
		(data_mem[11] << 24);
	for (i = 0; i < size; i += 16) {
		n = snprintf(buf, PR_BUF_MAX, "0x%04x: ", i);
		for (j = 0; j < 16; j++) {
			if ((i + j) >= size)
				break;
			n += snprintf(buf+n, PR_BUF_MAX, " %02x",
				data_mem[i+j]);
		}
		buf[n] = '\0';
		printf("%s\n", buf);
	}
}

static void lcd_tcon_data_print(unsigned char index)
{
	int i;
	char *buf;
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return;

	if (tcon_mm_table.version == 0) {
		LCDERR("%s: mem map version invalid\n", __func__);
		return;
	}

	buf = (char *)malloc(PR_BUF_MAX * sizeof(char));
	if (buf == NULL) {
		LCDERR("%s: buf malloc error\n", __func__);
		return;
	}

	if (index == 0xff) {
		for (i = 0; i < tcon_mm_table.block_cnt; i++) {
			if (!tcon_mm_table.data_mem_vaddr[i])
				continue;

			printf("tcon data[%d] print:\n", i);
			lcd_tcon_data_block_print(buf, tcon_mm_table.data_mem_vaddr[i]);
		}
	} else {
		if (index >= tcon_mm_table.block_cnt) {
			LCDERR("%s: invalid index %d\n", __func__, index);
			free(buf);
			return;
		}
		if (!tcon_mm_table.data_mem_vaddr[index]) {
			LCDERR("%s: invalid data_mem buf\n", __func__);
			free(buf);
			return;
		}

		printf("tcon data[%d] print:\n", index);
		lcd_tcon_data_block_print(buf, tcon_mm_table.data_mem_vaddr[index]);
	}

	free(buf);
}

void lcd_tcon_info_print(void)
{
	unsigned int size, cnt;
	int i, ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return;

	LCDPR("%s:\n", __func__);
	printf("core_reg_width:       %d\n"
		"reg_table_len:        %d\n"
		"rsv_mem addr:         0x%08x\n"
		"rsv_mem size:         0x%08x\n\n",
		lcd_tcon_conf->core_reg_width,
		lcd_tcon_conf->reg_table_len,
		tcon_rmem.mem_paddr,
		tcon_rmem.mem_size);
	if (tcon_rmem.axi_mem_size) {
		printf("axi_mem paddr:        0x%08x\n"
			"axi_mem size:         0x%08x\n",
			tcon_rmem.axi_mem_paddr,
			tcon_rmem.axi_mem_size);
	}
	if (tcon_rmem.bin_path_mem_size) {
		printf("bin_path_mem paddr:   0x%08x\n"
			"bin_path_mem vaddr:   0x%p\n"
			"bin_path_mem size:    0x%08x\n\n",
			tcon_rmem.bin_path_mem_paddr,
			tcon_mm_table.bin_path_mem_vaddr,
			tcon_rmem.bin_path_mem_size);
	}
	if (tcon_mm_table.version == 0) {
		if (tcon_mm_table.vac_mem_size) {
			printf("vac_mem vaddr:        0x%p\n"
				"vac_mem size:         0x%08x\n",
				tcon_mm_table.vac_mem_vaddr,
				tcon_mm_table.vac_mem_size);
		}
		if (tcon_mm_table.demura_set_mem_size) {
			printf("demura_set_mem vaddr: 0x%p\n"
				"demura_set_mem size:  0x%08x\n",
				tcon_mm_table.demura_set_mem_vaddr,
				tcon_mm_table.demura_set_mem_size);
		}
		if (tcon_mm_table.demura_lut_mem_size) {
			printf("demura_lut_mem vaddr: 0x%p\n"
				"demura_lut_mem size:  0x%08x\n",
				tcon_mm_table.demura_lut_mem_vaddr,
				tcon_mm_table.demura_lut_mem_size);
		}
		if (tcon_mm_table.acc_lut_mem_size) {
			printf("acc_lut_mem vaddr:    0x%p\n"
				"acc_lut_mem size:     0x%08x\n",
				tcon_mm_table.acc_lut_mem_vaddr,
				tcon_mm_table.acc_lut_mem_size);
		}
	} else {
		printf("data_mem_block_cnt:   %d\n",
			tcon_mm_table.block_cnt);
		for (i = 0; i < tcon_mm_table.block_cnt; i++) {
			if (tcon_mm_table.data_mem_vaddr[i]) {
				size = tcon_mm_table.data_mem_vaddr[i][8] |
					(tcon_mm_table.data_mem_vaddr[i][9] << 8) |
					(tcon_mm_table.data_mem_vaddr[i][10] << 16) |
					(tcon_mm_table.data_mem_vaddr[i][11] << 24);
				printf("data_mem[%d] vaddr:    0x%p\n"
					"data_mem[%d] size:     0x%08x\n",
					i, tcon_mm_table.data_mem_vaddr[i],
					i, size);
			}
		}
	}
	if (tcon_mm_table.bin_path_mem_vaddr) {
		size = tcon_mm_table.bin_path_mem_vaddr[4] |
			(tcon_mm_table.bin_path_mem_vaddr[5] << 8) |
			(tcon_mm_table.bin_path_mem_vaddr[6] << 16) |
			(tcon_mm_table.bin_path_mem_vaddr[7] << 24);
		cnt = tcon_mm_table.bin_path_mem_vaddr[16] |
			(tcon_mm_table.bin_path_mem_vaddr[17] << 8) |
			(tcon_mm_table.bin_path_mem_vaddr[18] << 16) |
			(tcon_mm_table.bin_path_mem_vaddr[19] << 24);
		if (size < (32 + 256 * cnt))
			return;
		printf("\n");
		for (i = 0; i < cnt; i++) {
			printf("tcon_path %d: %s\n",
				i,
				&tcon_mm_table.bin_path_mem_vaddr[32 + 256 * i]);
		}
	}
	printf("\n");
}

int lcd_tcon_enable(struct lcd_config_s *pconf)
{
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return -1;

	if (lcd_tcon_conf->tcon_enable)
		lcd_tcon_conf->tcon_enable(pconf);

	lcd_tcon_bin_path_resv_mem_set();

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
	if (lcd_tcon_conf->reg_core_od != REG_LCD_TCON_MAX) {
		reg = lcd_tcon_conf->reg_core_od + TCON_CORE_REG_START;
		bit = lcd_tcon_conf->bit_od_en;
		if (lcd_tcon_conf->core_reg_width == 8)
			lcd_tcon_setb_byte(reg, 0, bit, 1);
		else
			lcd_tcon_setb(reg, 0, bit, 1);
		mdelay(100);
	}

	/* disable all ctrl signal */
	if (lcd_tcon_conf->reg_core_ctrl_timing_base != REG_LCD_TCON_MAX) {
		reg = lcd_tcon_conf->reg_core_ctrl_timing_base + TCON_CORE_REG_START;
		offset = lcd_tcon_conf->ctrl_timing_offset;
		cnt = lcd_tcon_conf->ctrl_timing_cnt;
		for (i = 0; i < cnt; i++) {
			if (lcd_tcon_conf->core_reg_width == 8)
				lcd_tcon_setb_byte((reg + (i * offset)), 1, 3, 1);
			else
				lcd_tcon_setb((reg + (i * offset)), 1, 3, 1);
		}
	}

	/* disable top */
	if (lcd_tcon_conf->reg_top_ctrl != REG_LCD_TCON_MAX) {
		reg = lcd_tcon_conf->reg_top_ctrl;
		bit = lcd_tcon_conf->bit_en;
		lcd_tcon_setb(reg, 0, bit, 1);
	}
}

/* **********************************
 * tcon match data
 * **********************************
 */
static struct lcd_tcon_config_s tcon_data_txhd = {
	.tcon_valid = 0,

	.core_reg_width = LCD_TCON_CORE_REG_WIDTH_TXHD,
	.reg_table_len = LCD_TCON_TABLE_LEN_TXHD,

	.reg_top_ctrl = TCON_TOP_CTRL,
	.bit_en = BIT_TOP_EN_TXHD,

	.reg_core_od = REG_CORE_OD_TXHD,
	.bit_od_en = BIT_OD_EN_TXHD,

	.reg_core_ctrl_timing_base = REG_CORE_CTRL_TIMING_BASE_TXHD,
	.ctrl_timing_offset = CTRL_TIMING_OFFSET_TXHD,
	.ctrl_timing_cnt = CTRL_TIMING_CNT_TXHD,

	.rsv_mem_size    = 0x001fe000,
	.axi_size        = 0x001fe000,
	.bin_path_size   = 0,
	.vac_size        = 0,
	.demura_set_size = 0,
	.demura_lut_size = 0,
	.acc_lut_size    = 0,

	.reg_table = NULL,
	.tcon_enable = lcd_tcon_enable_txhd,
};

static struct lcd_tcon_config_s tcon_data_tl1 = {
	.tcon_valid = 0,

	.core_reg_width = LCD_TCON_CORE_REG_WIDTH_TL1,
	.reg_table_len = LCD_TCON_TABLE_LEN_TL1,

	.reg_top_ctrl = TCON_TOP_CTRL,
	.bit_en = BIT_TOP_EN_TL1,

	.reg_core_od = REG_CORE_OD_TL1,
	.bit_od_en = BIT_OD_EN_TL1,

	.reg_core_ctrl_timing_base = REG_LCD_TCON_MAX,
	.ctrl_timing_offset = CTRL_TIMING_OFFSET_TL1,
	.ctrl_timing_cnt = CTRL_TIMING_CNT_TL1,

	.rsv_mem_size    = 0x00c00000, /* 12M */
	.axi_size        = 0x00a00000, /* 10M */
	.bin_path_size   = 0x00002800, /* 10K */
	.vac_size        = 0x00002000, /* 8K */
	.demura_set_size = 0x00001000, /* 4K */
	.demura_lut_size = 0x00120000, /* 1152K */
	.acc_lut_size    = 0x00001000, /* 4K */

	.reg_table = NULL,
	.tcon_enable = lcd_tcon_enable_tl1,
};

int lcd_tcon_probe(char *dt_addr, struct aml_lcd_drv_s *lcd_drv, int load_id)
{
	int ret = 0;
	struct lcd_config_s *pconf = lcd_drv->lcd_config;

	lcd_tcon_conf = NULL;
	switch (lcd_drv->chip_type) {
	case LCD_CHIP_TXHD:
		switch (pconf->lcd_basic.lcd_type) {
		case LCD_MLVDS:
			lcd_tcon_conf = &tcon_data_txhd;
			lcd_tcon_conf->tcon_valid = 1;
			break;
		default:
			break;
		}
		break;
	case LCD_CHIP_TL1:
	case LCD_CHIP_TM2:
	case LCD_CHIP_T5:
		switch (pconf->lcd_basic.lcd_type) {
		case LCD_MLVDS:
		case LCD_P2P:
			lcd_tcon_conf = &tcon_data_tl1;
			if (lcd_tcon_core_flag(lcd_drv)) {
				if (lcd_debug_print_flag)
					LCDPR("%s: tcon invalid\n", __func__);
				lcd_tcon_conf->tcon_valid = 0;
			} else {
				lcd_tcon_conf->tcon_valid = 1;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	if (!lcd_tcon_conf)
		return 0;

	if (lcd_debug_print_flag)
		LCDPR("%s\n", __func__);

	memset(&tcon_rmem, 0, sizeof(struct tcon_rmem_s));
	memset(&tcon_mm_table, 0, sizeof(struct tcon_mem_map_table_s));
	lcd_tcon_spi_data_probe(lcd_drv);/*must before tcon_config, for memory alloc*/
	ret = lcd_tcon_config(dt_addr, pconf, load_id);

	lcd_drv->lcd_tcon_reg_print = lcd_tcon_reg_readback_print;
	lcd_drv->lcd_tcon_table_print = lcd_tcon_reg_table_print;
	lcd_drv->lcd_tcon_vac_print = lcd_tcon_vac_print;
	lcd_drv->lcd_tcon_demura_print = lcd_tcon_demura_print;
	lcd_drv->lcd_tcon_acc_print = lcd_tcon_acc_print;
	lcd_drv->lcd_tcon_data_print = lcd_tcon_data_print;
	lcd_drv->lcd_tcon_reg_read = lcd_tcon_reg_read;
	lcd_drv->lcd_tcon_reg_write = lcd_tcon_reg_write;
	lcd_drv->lcd_tcon_table_read = lcd_tcon_table_read;
	lcd_drv->lcd_tcon_table_write = lcd_tcon_table_write;

	return ret;
}
