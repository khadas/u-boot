/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __INI_IO_H__
#define __INI_IO_H__
#include "ini_size_define.h"

#define CS_LCD_ITEM_NAME                          "lcd"
#define CS_LCD_EXT_ITEM_NAME                      "lcd_extern"
#define CS_BACKLIGHT_ITEM_NAME                    "backlight"
#define CS_LDIM_DEV_ITEM_NAME                     "ldim_dev"
#define CS_LCD_TCON_ITEM_NAME                     "lcd_tcon"
#define CS_LCD_TCON_SPI_ITEM_NAME                 "lcd_tcon_spi"
#define CS_LCD_OPTICAL_ITEM_NAME                  "lcd_optical"

#define CS_LCD1_ITEM_NAME                         "lcd1"
#define CS_LCD1_EXT_ITEM_NAME                     "lcd1_extern"
#define CS_BACKLIGHT1_ITEM_NAME                   "backlight1"
#define CS_LCD1_OPTICAL_ITEM_NAME                 "lcd1_optical"

#define CS_LCD2_ITEM_NAME                         "lcd2"
#define CS_LCD2_EXT_ITEM_NAME                     "lcd2_extern"
#define CS_BACKLIGHT2_ITEM_NAME                   "backlight2"
#define CS_LCD2_OPTICAL_ITEM_NAME                 "lcd2_optical"

#define CS_PANEL_INI_PATH_ITEM_NAME               "panel_ini_path"
#define CS_PANEL_PQ_PATH_ITEM_NAME                "panel_pq_path"
#define CS_PANEL_ALL_INFO_ITEM_NAME               "panel_all_info"
#define CS_PANEL_ALL_DATA_ITEM_NAME               "panel_all"

#define CC_HEAD_CHKSUM_LEN                       (9)
#define CC_VERSION_LEN                           (5)

#ifdef __cplusplus
extern "C" {
#endif

int read_lcd_param(int index, unsigned char data_buf[]);
int save_lcd_param(int index, int wr_size, unsigned char data_buf[]);
int read_lcd_extern_param(int index, unsigned char data_buf[]);
int save_lcd_extern_param(int index, int wr_size, unsigned char data_buf[]);
int read_backlight_param(int index, unsigned char data_buf[]);
int save_backlight_param(int index, int wr_size, unsigned char data_buf[]);
int read_ldim_dev_param(unsigned char data_buf[]);
int save_ldim_dev_param(int wr_size, unsigned char data_buf[]);
int ReadTconSpiParam(unsigned char data_buf[]);
int SaveTconSpiParam(int wr_size, unsigned char data_buf[]);
int ReadLcdOpticalParam(int index, unsigned char data_buf[]);
int SaveLcdOpticalParam(int index, int wr_size, unsigned char data_buf[]);
int ReadTconBinParam(unsigned char data_buf[]);
int SaveTconBinParam(int wr_size, unsigned char data_buf[]);
int ReadPanelIniName(char data_buf[]);
int SavePanelIniName(char data_buf[]);
int ReadPanelPQPath(char data_buf[]);
int SavePanelPQPath(char data_buf[]);
int ReadPanelAllInfoData(unsigned char data_buf[]);
int SavePanelAllInfoData(int wr_size, unsigned char data_buf[]);
int ReadPanelAllData(int sec_no, unsigned char data_buf[]);
int SavePanelAllData(int sec_no, int wr_size, unsigned char data_buf[]);

int check_hex_data_no_header_valid(unsigned int* tmp_crc32, int max_len, int buf_len, unsigned char data_buf[]);
int check_hex_data_have_header_valid(unsigned int* tmp_crc32, int max_len, int buf_len, unsigned char data_buf[]);
int check_string_data_have_header_valid(unsigned int* tmp_crc32, char *data_str, int chksum_head_len, int ver_len);
unsigned int CalCRC32(unsigned int crc, const unsigned char *ptr, int buf_len);
void PrintDataBuf(int data_cnt, unsigned char data_buf[]);

#ifdef __cplusplus
}
#endif

#endif //__INI_IO_H__
