/*
* Copyright (C) 2017 Amlogic, Inc. All rights reserved.
* *
This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* *
This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
* *
You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
* *
Description:
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
#define CC_MAX_TCON_BIN_SIZE             (0x5dc0)  /* max:24000 */
#define CC_MAX_TCON_VAC_SIZE             (0x1000)
#define CC_MAX_TCON_DEMURA_SET_SIZE      (0x1000)
#define CC_MAX_TCON_DEMURA_LUT_SIZE      (0x300000)
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
