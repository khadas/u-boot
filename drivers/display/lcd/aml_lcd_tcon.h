/*
 * driver/display/lcd/aml_lcd_tcon.h
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

#ifndef _AML_LCD_TCON_H
#define _AML_LCD_TCON_H
#include <amlogic/aml_lcd.h>

#define REG_LCD_TCON_MAX    0xffff

struct lcd_tcon_config_s {
	unsigned char tcon_valid;

	unsigned int core_reg_ver;
	unsigned int core_reg_width;
	unsigned int reg_table_width;
	unsigned int reg_table_len;
	unsigned int core_reg_start;

	unsigned int reg_top_ctrl;
	unsigned int bit_en;

	unsigned int reg_core_od;
	unsigned int bit_od_en;

	unsigned int reg_ctrl_timing_base;
	unsigned int ctrl_timing_offset;
	unsigned int ctrl_timing_cnt;

	unsigned int axi_bank;

	unsigned int rsv_mem_size;
	unsigned int axi_size;
	unsigned int bin_path_size;
	unsigned int vac_size;
	unsigned int demura_set_size;
	unsigned int demura_lut_size;
	unsigned int acc_lut_size;

	unsigned int *axi_reg;
	void (*tcon_axi_mem_config)(void);
	void (*tcon_axi_mem_update)(unsigned int *table);
	int (*tcon_enable)(struct lcd_config_s *pconf);
	int (*tcon_disable)(struct lcd_config_s *pconf);
};

struct tcon_rmem_config_s {
	unsigned int mem_paddr;
	unsigned char *mem_vaddr;
	unsigned int mem_size;
};

struct tcon_rmem_s {
	unsigned int flag;

	unsigned int rsv_mem_paddr;
	unsigned int axi_mem_paddr;
	unsigned int rsv_mem_size;
	unsigned int axi_mem_size;

	struct tcon_rmem_config_s *axi_rmem;
	struct tcon_rmem_config_s bin_path_rmem;

	struct tcon_rmem_config_s vac_rmem;
	struct tcon_rmem_config_s demura_set_rmem;
	struct tcon_rmem_config_s demura_lut_rmem;
	struct tcon_rmem_config_s acc_lut_rmem;
};

struct tcon_data_priority_s {
	unsigned int index;
	unsigned int priority;
};

struct tcon_mem_map_table_s {
	/*header*/
	unsigned int version;
	unsigned char tcon_data_flag;
	unsigned int data_load_level;
	unsigned int block_cnt;
	unsigned char init_load;

	unsigned int valid_flag;
	unsigned char demura_cnt;
	unsigned int block_bit_flag;

	unsigned int core_reg_table_size;
	struct lcd_tcon_init_block_header_s *core_reg_header;
	unsigned char *core_reg_table;

	struct tcon_data_priority_s *data_priority;
	unsigned int *data_size;
	unsigned char **data_mem_vaddr;
};

#define TCON_BIN_VER_LEN    9
struct lcd_tcon_local_cfg_s {
	char bin_ver[TCON_BIN_VER_LEN];
};

/* **********************************
 * tcon config
 * ********************************** */
/* TXHD */
#define LCD_TCON_CORE_REG_WIDTH_TXHD     32
#define LCD_TCON_TABLE_WIDTH_TXHD        8
#define LCD_TCON_TABLE_LEN_TXHD          4096
#define LCD_TCON_AXI_BANK_TXHD           1

#define BIT_TOP_EN_TXHD                  4

#define TCON_CORE_REG_START_TXHD         0x0000
#define REG_CORE_OD_TXHD                 0x5c
#define BIT_OD_EN_TXHD                   6
#define REG_CTRL_TIMING_BASE_TXHD        0x1b
#define CTRL_TIMING_OFFSET_TXHD          12
#define CTRL_TIMING_CNT_TXHD             16


/* TL1 */
#define LCD_TCON_CORE_REG_WIDTH_TL1      8
#define LCD_TCON_TABLE_WIDTH_TL1         8
#define LCD_TCON_TABLE_LEN_TL1           24000
#define LCD_TCON_AXI_BANK_TL1            3

#define BIT_TOP_EN_TL1                   4

#define TCON_CORE_REG_START_TL1          0x0000
#define REG_CORE_OD_TL1                  0x247
#define BIT_OD_EN_TL1                    0
#define REG_CTRL_TIMING_BASE_TL1         0x1b
#define CTRL_TIMING_OFFSET_TL1           12
#define CTRL_TIMING_CNT_TL1              0

/* T5 */
#define LCD_TCON_CORE_REG_WIDTH_T5       32
#define LCD_TCON_TABLE_WIDTH_T5          32
#define LCD_TCON_TABLE_LEN_T5            0x18d4 /* 0x635*4 */
#define LCD_TCON_AXI_BANK_T5             2

#define BIT_TOP_EN_T5                    4

#define TCON_CORE_REG_START_T5           0x0100
#define REG_CORE_OD_T5                   0x263
#define BIT_OD_EN_T5                     31
#define REG_CTRL_TIMING_BASE_T5          0x300
#define CTRL_TIMING_OFFSET_T5            12
#define CTRL_TIMING_CNT_T5               0

/* T5D */
#define LCD_TCON_CORE_REG_WIDTH_T5D       32
#define LCD_TCON_TABLE_WIDTH_T5D          32
#define LCD_TCON_TABLE_LEN_T5D            0x102c /* 0x40b*4 */
#define LCD_TCON_AXI_BANK_T5D             1

#define BIT_TOP_EN_T5D                    4

#define TCON_CORE_REG_START_T5D           0x0100
#define REG_CORE_OD_T5D                   0x263
#define BIT_OD_EN_T5D                     31
#define REG_CTRL_TIMING_BASE_T5D          0x1b
#define CTRL_TIMING_OFFSET_T5D            12
#define CTRL_TIMING_CNT_T5D               0

#ifdef CONFIG_CMD_INI
extern void *handle_lcd_ext_buf_get(void);
extern void *handle_tcon_path_mem_get(unsigned int size);
void *handle_tcon_path_resv_mem_get(unsigned int size);
int handle_tcon_vac(unsigned char *vac_data, unsigned int vac_mem_size);
extern int handle_tcon_demura_set(unsigned char *demura_set_data,
				  unsigned int demura_set_size);
extern int handle_tcon_demura_lut(unsigned char *demura_lut_data,
				  unsigned int demura_lut_size);
extern int handle_tcon_acc_lut(unsigned char *acc_lut_data,
			       unsigned int acc_lut_size);
extern int handle_tcon_data_load(unsigned char **buf, unsigned int index);
#endif

#define TCON_VAC_SET_PARAM_NUM    3
#define TCON_VAC_LUT_PARAM_NUM    256

void lcd_tcon_init_data_version_update(char *data_buf);
int lcd_tcon_data_multi_match_find(unsigned char *data_buf);
int lcd_tcon_spi_data_probe(struct aml_lcd_drv_s *lcd_drv);

int lcd_tcon_valid_check(void);
struct lcd_tcon_config_s *get_lcd_tcon_config(void);
struct tcon_rmem_s *get_lcd_tcon_rmem(void);
struct tcon_mem_map_table_s *get_lcd_tcon_mm_table(void);
struct lcd_tcon_local_cfg_s *get_lcd_tcon_local_cfg(void);

int lcd_tcon_enable_txhd(struct lcd_config_s *pconf);
int lcd_tcon_enable_tl1(struct lcd_config_s *pconf);
int lcd_tcon_disable_tl1(struct lcd_config_s *pconf);
int lcd_tcon_enable_t5(struct lcd_config_s *pconf);
int lcd_tcon_disable_t5(struct lcd_config_s *pconf);

#endif

