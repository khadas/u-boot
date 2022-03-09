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

static struct tcon_rmem_s tcon_rmem = {
	.flag = 0,
	.mem_paddr = 0,
	.mem_size = 0,
	.core_mem_paddr = 0,
	.core_mem_size = 0,
	.vac_mem_paddr = 0,
	.vac_mem_size = 0,
	.demura_set_paddr = 0,
	.demura_set_mem_size = 0,
	.demura_lut_paddr = 0,
	.demura_lut_mem_size = 0,
};

struct tcon_rmem_s *get_lcd_tcon_rmem(void)
{
	return &tcon_rmem;
}

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

static void lcd_tcon_od_check(unsigned char *table)
{
	unsigned int reg, bit;

	if (lcd_tcon_data->reg_core_od == REG_LCD_TCON_MAX)
		return;

	reg = lcd_tcon_data->reg_core_od;
	bit = lcd_tcon_data->bit_od_en;
	/* disable od function*/
	table[reg] &= ~(1 << bit);
}

static int lcd_tcon_vac_load(void)
{
	unsigned char *vac_data =
		(unsigned char *)(unsigned long)(tcon_rmem.vac_mem_paddr);
	int ret = 0;
#ifdef CONFIG_CMD_INI
	int i, chk_data = 0;
#endif
	if ((!tcon_rmem.vac_mem_size) || (vac_data == NULL))
		return -1;

#ifdef CONFIG_CMD_INI
	memset(vac_data, 0, tcon_rmem.vac_mem_size);
	ret = handle_tcon_vac(vac_data, tcon_rmem.vac_mem_size);
	if (ret) {
		LCDPR("%s: no vac data\n", __func__);
		return -1;
	}
	chk_data |= vac_data[0];
	chk_data |= vac_data[1] << 8;
	chk_data |= vac_data[2] << 16;
	chk_data |= vac_data[3] << 24;
	if (!chk_data) {
		LCDPR("%s: vac_data check error\n", __func__);
		return -1;
	}

	if (lcd_debug_print_flag == 3) {
		for (i = 0; i < 30; i++)
			LCDPR("vac_data[%d]: 0x%02x\n", i, vac_data[i * 1]);
	}
#endif
	return ret;
}

static int lcd_tcon_demura_set_load(void)
{
	unsigned char *demura_setting = (unsigned char *)(unsigned long)
			       (tcon_rmem.demura_set_paddr);
	int ret = 0;
#ifdef CONFIG_CMD_INI
	int i, chk_data = 0;
#endif
	if ((!tcon_rmem.demura_set_mem_size) || (demura_setting == NULL))
		return -1;

#ifdef CONFIG_CMD_INI
	memset(demura_setting, 0, tcon_rmem.demura_set_mem_size);
	ret = handle_tcon_demura_set(demura_setting,
				     tcon_rmem.demura_set_mem_size);
	if (ret) {
		LCDPR("%s: no demura_set data\n", __func__);
		return -1;
	}

	chk_data |= demura_setting[0];
	chk_data |= demura_setting[1] << 8;
	chk_data |= demura_setting[2] << 16;
	chk_data |= demura_setting[3] << 24;
	if (!chk_data) {
		LCDPR("%s: demura_setting check error\n", __func__);
		return -1;
	}

	if (lcd_debug_print_flag == 3) {
		for (i = 0; i < 100; i++)
			LCDPR("demura_set[%d]: 0x%x\n",
			      i, demura_setting[i]);
	}
#endif
	return ret;
}

static int lcd_tcon_demura_lut_load(void)
{
	unsigned char *demura_lut_data = (unsigned char *)(unsigned long)
			       (tcon_rmem.demura_lut_paddr);
	int ret = 0;
#ifdef CONFIG_CMD_INI
	int i, chk_data = 0;
#endif
	if ((!tcon_rmem.demura_lut_mem_size) || (demura_lut_data == NULL))
		return -1;

#ifdef CONFIG_CMD_INI
	memset(demura_lut_data, 0, tcon_rmem.demura_lut_mem_size);
	ret = handle_tcon_demura_lut(demura_lut_data,
				     tcon_rmem.demura_lut_mem_size);
	if (ret) {
		LCDPR("%s: no demura_lut data\n", __func__);
		return -1;
	}
	chk_data |= demura_lut_data[0];
	chk_data |= demura_lut_data[1] << 8;
	chk_data |= demura_lut_data[2] << 16;
	chk_data |= demura_lut_data[3] << 24;
	if (!chk_data) {
		LCDPR("%s: demura_lut check error\n", __func__);
		return -1;
	}
	if (lcd_debug_print_flag == 3) {
		for (i = 0; i < 100; i++)
			LCDPR("demura_lut_data[%d]: 0x%02x\n",
			      i, demura_lut_data[i]);
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

	len = lcd_tcon_data->reg_table_len;
	table = lcd_tcon_data->reg_table;
	if (table == NULL) {
		LCDERR("%s: table is NULL\n", __func__);
		return;
	}
	lcd_tcon_od_check(table);
	if (lcd_tcon_data->core_reg_width == 8) {
		for (i = 0; i < len; i++)
			lcd_tcon_write_byte((i + TCON_CORE_REG_START), table[i]);
	} else {
		for (i = 0; i < len; i++)
			lcd_tcon_write((i + TCON_CORE_REG_START), table[i]);
	}
	LCDPR("tcon core regs update\n");

	if (lcd_tcon_data->reg_core_od != REG_LCD_TCON_MAX) {
		i = lcd_tcon_data->reg_core_od;
		if (lcd_tcon_data->core_reg_width == 8)
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
	unsigned int d0, d1, temp, set1, set2;
	unsigned int demura_support;
	unsigned char *vac_data =
		(unsigned char *)(unsigned long)(tcon_rmem.vac_mem_paddr);

	if ((!tcon_rmem.vac_mem_size) || (vac_data == NULL))
		return;

	LCDPR("lcd_tcon_vac_set\n");
	n = 8;
	len = TCON_VAC_SET_PARAM_NUM;
	demura_support = vac_data[n];
	set1 = vac_data[n + 2];
	set2 = vac_data[n + 4];
	n += (len * 2);
	if (lcd_debug_print_flag)
		LCDPR("vac_set: %d, 0x%x, 0x%x\n", demura_support, set1, set2);

	/* vac support demura policy */
	if (demura_support) {
		if (demura_valid == 0) {
			LCDPR("lcd_tcon vac_set exit for demura invalid\n");
			return;
		}
	}

	lcd_tcon_write_byte(0x0267, lcd_tcon_read_byte(0x0267) | 0xa0);
	/*vac_cntl, 12pipe delay temp for pre_dt*/
	lcd_tcon_write(0x2800, 0x807);
	lcd_tcon_write(0x2817, (0x1e | ((set1 & 0xff) << 8)));

	len = TCON_VAC_LUT_PARAM_NUM;
	if (lcd_debug_print_flag)
		LCDPR("%s: start write vac_ramt1~2\n", __func__);
	/*write vac_ramt1: 8bit, 256 regs*/
	for (i = 0; i < len; i++)
		lcd_tcon_write_byte(0xa100 + i, vac_data[n+i*2]);

	for (i = 0; i < len; i++)
		lcd_tcon_write_byte(0xa200 + i, vac_data[n+i*2]);

	/*write vac_ramt2: 8bit, 256 regs*/
	n += (len * 2);
	for (i = 0; i < len; i++)
		lcd_tcon_write_byte(0xa300 + i, vac_data[n+i*2]);

	for (i = 0; i < len; i++)
		lcd_tcon_write_byte(0xbc00 + i, vac_data[n+i*2]);

	if (lcd_debug_print_flag)
		LCDPR("%s: write vac_ramt1~2 ok\n", __func__);
	for (i = 0; i < len; i++)
		lcd_tcon_read_byte(0xbc00 + i);

	if (lcd_debug_print_flag)
		LCDPR("%s: start write vac_ramt3\n", __func__);
	/*write vac_ramt3_1~6: 16bit({data0[11:0],data1[11:0]},128 regs)*/
	for (j = 0; j < 6; j++) {
		n += (len * 2);
		for (i = 0; i < (len >> 1); i++) {
			d0 = (vac_data[n + (i * 4)] |
				(vac_data[n + (i * 4 + 1)] << 8)) & 0xfff;
			d1 = (vac_data[n + (i * 4 + 2)] |
				(vac_data[n + (i * 4 + 3)] << 8)) & 0xfff;
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
}

static int lcd_tcon_demura_set_tl1(void)
{
	unsigned char *demura_setting = (unsigned char *)(unsigned long)
			       (tcon_rmem.demura_set_paddr);
	int i;

	if ((!tcon_rmem.demura_set_mem_size) || (demura_setting == NULL))
		return -1;

	if (lcd_tcon_getb_byte(0x23d, 0, 1) == 0) {
		if (lcd_debug_print_flag)
			LCDPR("%s: demura function disabled\n", __func__);
		return 0;
	}

	LCDPR("lcd_tcon demura_set\n");

	for (i = 8; i < (160 + 8); i++)
		lcd_tcon_write_byte(0x186, demura_setting[i]);

	return 0;
}

static int lcd_tcon_demura_lut_tl1(void)
{
	unsigned char *demura_lut_data = (unsigned char *)(unsigned long)
			       (tcon_rmem.demura_lut_paddr);
	int i;

	if ((!tcon_rmem.demura_lut_mem_size) || (demura_lut_data == NULL))
		return -1;

	if (lcd_tcon_getb_byte(0x23d, 0, 1) == 0)
		return 0;

	LCDPR("lcd_tcon demura_lut\n");

	lcd_tcon_setb_byte(0x181, 1, 0, 1);
	lcd_tcon_write_byte(0x182, 0x01);
	lcd_tcon_write_byte(0x183, 0x86);
	lcd_tcon_write_byte(0x184, 0x01);
	lcd_tcon_write_byte(0x185, 0x87);

	for (i = 8; i < (391053 + 8); i++)
		lcd_tcon_write_byte(0x187, demura_lut_data[i]);

	LCDPR("lcd_tcon 0x23d = 0x%02x\n", lcd_tcon_read_byte(0x23d));

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

static int lcd_tcon_mem_config(void)
{
	unsigned int max_size;

	max_size = lcd_tcon_data->core_size +
		lcd_tcon_data->vac_size +
		lcd_tcon_data->demura_set_size +
		lcd_tcon_data->demura_lut_size;

	if (tcon_rmem.mem_size < max_size) {
		LCDERR("%s: tcon mem size 0x%x is not enough, need 0x%x\n",
			__func__, tcon_rmem.mem_size, max_size);
		return -1;
	}

	tcon_rmem.core_mem_size = lcd_tcon_data->core_size;
	tcon_rmem.core_mem_paddr = tcon_rmem.mem_paddr;
	if (lcd_debug_print_flag)
		LCDPR("core_paddr: 0x%08x, size: 0x%x\n",
		      tcon_rmem.core_mem_paddr, tcon_rmem.core_mem_size);

	tcon_rmem.vac_mem_size = lcd_tcon_data->vac_size;
	tcon_rmem.vac_mem_paddr =
		tcon_rmem.core_mem_paddr + tcon_rmem.core_mem_size;
	if (lcd_debug_print_flag && (tcon_rmem.vac_mem_size > 0))
		LCDPR("vac_paddr: 0x%08x, size: 0x%x\n",
		      tcon_rmem.vac_mem_paddr, tcon_rmem.vac_mem_size);

	tcon_rmem.demura_set_mem_size = lcd_tcon_data->demura_set_size;
	tcon_rmem.demura_set_paddr =
		tcon_rmem.vac_mem_paddr + tcon_rmem.vac_mem_size;
	if (lcd_debug_print_flag && (tcon_rmem.demura_set_mem_size > 0))
		LCDPR("dem_set_paddr: 0x%08x, size: 0x%x\n",
		      tcon_rmem.demura_set_paddr,
		      tcon_rmem.demura_set_mem_size);

	tcon_rmem.demura_lut_mem_size = lcd_tcon_data->demura_lut_size;
	tcon_rmem.demura_lut_paddr =
		tcon_rmem.demura_set_paddr + tcon_rmem.demura_set_mem_size;
	if (lcd_debug_print_flag && (tcon_rmem.demura_lut_mem_size > 0))
		LCDPR("dem_lut_paddr: 0x%08x, size: 0x%x\n",
		      tcon_rmem.demura_lut_paddr,
		      tcon_rmem.demura_lut_mem_size);

	return 0;
}

int lcd_tcon_data_load(int *vac_valid, int *demura_valid)
{
	unsigned char *table = lcd_tcon_data->reg_table;
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return -1;

	ret = lcd_tcon_vac_load();
	if (!ret)
		*vac_valid = 1;
	if (table == NULL)
		return 0;
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
			*demura_valid = 1;
		}
	}
	return 0;
}

static int lcd_tcon_enable_tl1(struct lcd_config_s *pconf)
{
	unsigned int n = 10;
	int ret;
	int vac_valid = 0, demura_valid = 0;

	ret = lcd_tcon_valid_check();
	if (ret)
		return -1;

	n = getenv_ulong("tcon_delay", 10, 10);

	lcd_tcon_data_load(&vac_valid, &demura_valid);

	/* step 1: tcon top */
	lcd_tcon_top_set_tl1(pconf);

	/* step 2: tcon_core_reg_update */
	lcd_tcon_core_reg_update();
	if (pconf->lcd_basic.lcd_type == LCD_P2P) {
		switch (pconf->lcd_control.p2p_config->p2p_type) {
		case P2P_CHPI:
			lcd_phy_tcon_chpi_bbc_init_tl1(n);
			break;
		default:
			break;
		}
	}

	if (vac_valid)
		lcd_tcon_vac_set_tl1(demura_valid);
	if (demura_valid) {
		lcd_tcon_demura_set_tl1();
		lcd_tcon_demura_lut_tl1();
	}

	/* step 3: tcon_top_output_set */
	lcd_tcon_write(TCON_OUT_CH_SEL1, 0xba98); /* out swap for ch8~11 */
	LCDPR("set tcon ch_sel: 0x%08x, 0x%08x\n",
		lcd_tcon_read(TCON_OUT_CH_SEL0),
		lcd_tcon_read(TCON_OUT_CH_SEL1));

	return 0;
}

#if 0
static void lcd_tcon_p2p_chpi_irq(void)
{
	unsigned int temp;

	temp = lcd_tcon_read(TCON_INTR_RO);
	//LCDPR("tcon intr: 0x%x\n", temp);
	lcd_tcon_write(TCON_INTR_CLR, temp);
	temp = lcd_tcon_read_byte(0xb3b + TCON_CORE_REG_START);
	lcd_tcon_read_byte(0xb3a + TCON_CORE_REG_START);
	if (temp & 0x6) {
		lcd_tcon_write_byte(0xb3b + TCON_CORE_REG_START, 0xf);
		lcd_tcon_write_byte(0xb3b + TCON_CORE_REG_START, 0);
		lcd_tcon_read_byte(0xb3a + TCON_CORE_REG_START);
		if (temp & 0x2) {
			//LCDPR("tcon_irq_cnt: %d, set channel pull down\n", tcon_irq_cnt);
			switch (tcon_irq_cnt) {
			case 0:
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL1, 0, 3, 1);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL1, 0, 19, 1);
				break;
			case 2:
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL2, 0, 3, 1);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL2, 0, 19, 1);
				break;
			case 4:
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL3, 0, 3, 1);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL3, 0, 19, 1);
				break;
			case 6:
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL4, 0, 3, 1);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL4, 0, 19, 1);
				break;
			case 8:
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL6, 0, 3, 1);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL6, 0, 19, 1);
				break;
			case 10:
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL7, 0, 3, 1);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL7, 0, 19, 1);
				break;
			default:
				LCDPR("invalid pull_down tcon_irq_cnt: %d, temp=0x%x\n",
					tcon_irq_cnt, temp);
				break;
			}
			if (tcon_irq_cnt < 0xff)
				tcon_irq_cnt++;
		}
		if (temp & 0x4) {
			//LCDPR("tcon_irq_cnt: %d, release channel pull down\n", tcon_irq_cnt);
			switch (tcon_irq_cnt) {
			case 1:
				//udelay(10);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL1, 1, 3, 1);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL1, 1, 19, 1);
				break;
			case 3:
				//udelay(10);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL2, 1, 3, 1);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL2, 1, 19, 1);
				break;
			case 5:
				//udelay(10);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL3, 1, 3, 1);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL3, 1, 19, 1);
				break;
			case 7:
				//udelay(10);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL4, 1, 3, 1);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL4, 1, 19, 1);
				break;
			case 9:
				//udelay(10);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL6, 1, 3, 1);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL6, 1, 19, 1);
				break;
			case 11:
				//udelay(10);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL7, 1, 3, 1);
				lcd_hiu_setb(HHI_DIF_CSI_PHY_CNTL7, 1, 19, 1);
				break;
			default:
				LCDPR("invalid release pull_down tcon_irq_cnt: %d, temp=0x%x\n",
					tcon_irq_cnt, temp);
				break;
			}
			if (tcon_irq_cnt < 0xff)
				tcon_irq_cnt++;
		}
	}
	//udelay(1);
}
#endif
static void lcd_tcon_config_axi_offset_default(void)
{

	tcon_rmem.mem_paddr = getenv_ulong("tcon_mem_addr", 16, 0);
	if (tcon_rmem.mem_paddr) {
		tcon_rmem.mem_size = lcd_tcon_data->axi_mem_size;
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
	int ret;
	int mem_size;

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
					mem_size = be32_to_cpup
							((((u32 *)propdata)+1));
				else
					mem_size = be32_to_cpup
							(((u32 *)propdata));

				if (mem_size < lcd_tcon_data->axi_mem_size) {
					LCDERR("set mem_size too small\n");
					tcon_rmem.mem_paddr = 0;
					tcon_rmem.flag = 0;
				} else {
					tcon_rmem.mem_size =
						lcd_tcon_data->axi_mem_size;
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
	LCDPR("tcon: load unifykey len: %d\n", key_len);
#else
	reg_len = lcd_tcon_data->reg_table_len;
	if (lcd_tcon_data->reg_table == NULL)
		lcd_tcon_data->reg_table = uhd_tcon_setting_ceds_h10;
	key_len = sizeof(uhd_tcon_setting_ceds_h10)/sizeof(unsigned char);
	if (key_len != reg_len) {
		free(lcd_tcon_data->reg_table);
		lcd_tcon_data->reg_table = NULL;
		LCDERR("%s: !!!!!!!!tcon unifykey load length error!!!!!!!!\n",
			__func__);
		return -1;
	}
	LCDPR("tcon: load default table len: %d\n", key_len);
#endif

	return 0;
}

static int lcd_tcon_core_flag(struct aml_lcd_drv_s *lcd_drv)
{
	int ret = 0;

	switch (lcd_drv->chip_type) {
	case LCD_CHIP_TL1:
	case LCD_CHIP_TM2:
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

unsigned char *lcd_tcon_table_get(unsigned int *size)
{
	int ret;

	*size = 0;

	ret = lcd_tcon_valid_check();
	if (ret)
		return NULL;

	if (lcd_tcon_data->reg_table)
		*size = lcd_tcon_data->reg_table_len;
	return lcd_tcon_data->reg_table;
}

void lcd_tcon_info_print(void)
{
	int ret;

	ret = lcd_tcon_valid_check();
	if (ret)
		return;

	LCDPR("%s:\n", __func__);
	printf("core_reg_width:    %d\n"
		"reg_table_len:     %d\n"
		"axi_mem addr:      0x%08x\n"
		"axi_mem size:      0x%08x\n\n",
		lcd_tcon_data->core_reg_width,
		lcd_tcon_data->reg_table_len,
		tcon_rmem.mem_paddr,
		tcon_rmem.mem_size);
	if (tcon_rmem.core_mem_size) {
		printf("core_mem addr:          0x%08x\n"
			"core_mem size:        0x%08x\n",
			tcon_rmem.core_mem_paddr,
			tcon_rmem.core_mem_size);
	}
	if (tcon_rmem.vac_mem_size) {
		printf("vac_mem addr:           0x%08x\n"
			"vac_mem size:         0x%08x\n",
			tcon_rmem.vac_mem_paddr,
			tcon_rmem.vac_mem_size);
	}
	if (tcon_rmem.demura_set_mem_size) {
		printf("demura_set_mem addr:    0x%08x\n"
			"demura_set_mem size:  0x%08x\n",
			tcon_rmem.demura_set_paddr,
			tcon_rmem.demura_set_mem_size);
	}
	if (tcon_rmem.demura_lut_mem_size) {
		printf("demura_lut_mem addr:    0x%08x\n"
			"demura_lut_mem size:  0x%08x\n",
			tcon_rmem.demura_lut_paddr,
			tcon_rmem.demura_lut_mem_size);
	}
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
		bit = lcd_tcon_data->bit_od_en;
		if (lcd_tcon_data->core_reg_width == 8)
			lcd_tcon_setb_byte(reg, 0, bit, 1);
		else
			lcd_tcon_setb(reg, 0, bit, 1);
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
static struct lcd_tcon_data_s tcon_data_txhd = {
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

	.axi_mem_size    = 0x001fe000,
	.core_size       = 0x001fe000,
	.vac_size        = 0,
	.demura_set_size = 0,
	.demura_lut_size = 0,

	.reg_table = NULL,
	.tcon_enable = lcd_tcon_enable_txhd,
};

static struct lcd_tcon_data_s tcon_data_tl1 = {
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

	/*tcon_mem_total: core_mem(10M)  vac(8k) dem_set(4k) dem_lut(400k)
	 * (12M) :      |--------------|-------|-----------|--------|
	 */
	.axi_mem_size    = 0x00c00000,
	.core_size       = 0x00a00000,
	.vac_size        = 0x00002000,
	.demura_set_size = 0x00001000,
	.demura_lut_size = 0x00064000,

	.reg_table = NULL,
	.tcon_enable = lcd_tcon_enable_tl1,
};

int lcd_tcon_probe(char *dt_addr, struct aml_lcd_drv_s *lcd_drv, int load_id)
{
	int ret = 0;
	struct lcd_config_s *pconf = lcd_drv->lcd_config;

	lcd_tcon_data = NULL;
	switch (lcd_drv->chip_type) {
	case LCD_CHIP_TXHD:
		switch (pconf->lcd_basic.lcd_type) {
		case LCD_MLVDS:
			lcd_tcon_data = &tcon_data_txhd;
			lcd_tcon_data->tcon_valid = 1;
			break;
		default:
			break;
		}
		break;
	case LCD_CHIP_TL1:
	case LCD_CHIP_TM2:
		switch (pconf->lcd_basic.lcd_type) {
		case LCD_MLVDS:
		case LCD_P2P:
			lcd_tcon_data = &tcon_data_tl1;
			if (lcd_tcon_core_flag(lcd_drv)) {
				if (lcd_debug_print_flag)
					LCDPR("%s: tcon invalid\n", __func__);
				lcd_tcon_data->tcon_valid = 0;
			} else {
				lcd_tcon_data->tcon_valid = 1;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	if (lcd_tcon_data == NULL)
		return 0;

	if (lcd_debug_print_flag)
		LCDPR("%s\n", __func__);
	ret = lcd_tcon_config(dt_addr, pconf, load_id);

	lcd_drv->lcd_tcon_reg_print = lcd_tcon_reg_readback_print;
	lcd_drv->lcd_tcon_table_print = lcd_tcon_reg_table_print;
	lcd_drv->lcd_tcon_reg_read = lcd_tcon_reg_read;
	lcd_drv->lcd_tcon_reg_write = lcd_tcon_reg_write;

	return ret;
}
