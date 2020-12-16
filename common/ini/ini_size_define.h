/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/ini/ini_size_define.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __INI_SIZE_H__
#define __INI_SIZE_H__

#define CC_MAX_TEMP_BUF_SIZE             (0x1000)
#define CC_MAX_PANEL_ALL_DATA_SIZE       (0x100000)

#define CC_MAX_INI_FILE_NAME_LEN         (512)
/* 256->5k for large lcd_ext large init_on table */
#define CC_MAX_INI_FILE_LINE_LEN         (5120)

#define CC_MAX_INI_LINE_NAME_LEN         (128)
#define CC_MAX_INI_FILE_SIZE             (0x400000)

#define CC_MAX_DATA_SIZE                 (0x300000)
#define CC_MAX_TCON_BIN_SIZE             (0xf000)  /* max:24000 */
#define CC_MAX_TCON_VAC_SIZE             (0x1000)
#define CC_MAX_TCON_DEMURA_SET_SIZE      (0x1000)
#define CC_MAX_TCON_DEMURA_LUT_SIZE      (0x120000)
#define CC_MAX_TCON_ACC_LUT_SIZE         (0x1000)
#define CC_MAX_TCON_BIN_PATH_SIZE        (0x2800)
#define CC_MAX_TCON_SPI_SIZE             (0x610)
#define CC_ONE_SECTION_SIZE              (0x10000)


#ifdef CONFIG_INI_MAX_LINE
#define MAX_LINE CONFIG_INI_MAX_LINE
#else
#define MAX_LINE 200
#endif

#ifdef CONFIG_INI_MAX_SECTION
#define MAX_SECTION CONFIG_INI_MAX_SECTION
#else
#define MAX_SECTION 300
#endif

#ifdef CONFIG_INI_MAX_NAME
#define MAX_NAME CONFIG_INI_MAX_NAME
#else
#define MAX_NAME 100
#endif

#endif //__INI_CONFIG_H__
