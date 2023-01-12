
/*
 * arch/arm/include/asm/arch-txl/efuse.h
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef __EFUSE_H
#define __EFUSE_H

#include <config.h>
#include <common.h>

/* efuse HAL_API arg */
struct efuse_hal_api_arg {
	unsigned int cmd;		/* R/W */
	unsigned int offset;
	unsigned int size;
	unsigned long buffer_phy;
	unsigned long retcnt_phy;
};


#define EFUSE_BYTES				512   /* (EFUSE_BITS/8) */

#define EFUSE_HAL_API_READ	0
#define EFUSE_HAL_API_WRITE 1
#define EFUSE_HAL_API_WRITE_PATTERN 2
#define EFUSE_HAL_API_USER_MAX 3
#define EFUSE_HAL_API_READ_CALI 4
#define EFUSE_HAL_API_READ_CALI_ITEM 5
#define EFUSE_HAL_API_CHECKPATTERN_ITEM  6
#define CONFIG_EFUSE_OBJ_API 1

#define EFUSE_USER_MASK            (0x1 << 16)
#define EFUSE_THERMAL_MASK         (0x1 << 17)
#define EFUSE_THERMAL_VERFLAG_MASK (0x1 << 18)
#define EFUSE_ENCRYPT_MASK         (0x1 << 19)

//#define ASSIST_HW_REV                              0x1f53

typedef enum efuse_obj_status_s {
	EFUSE_OBJ_SUCCESS		= 0,

	EFUSE_OBJ_ERR_OTHER_INTERNAL	= 1,

	EFUSE_OBJ_ERR_INVALID_DATA	= 100,
	EFUSE_OBJ_ERR_NOT_FOUND,
	EFUSE_OBJ_ERR_DEPENDENCY,
	EFUSE_OBJ_ERR_SIZE,
	EFUSE_OBJ_ERR_NOT_SUPPORT,

	EFUSE_OBJ_ERR_ACCESS		= 200,
	EFUSE_OBJ_ERR_WRITE_PROTECTED = 201,

	EFUSE_OBJ_ERR_UNKNOWN		= 300,
	EFUSE_OBJ_ERR_INTERNAL,
} efuse_obj_status_e;

typedef enum efuse_obj_info_s {
	EFUSE_OBJ_EFUSE_DATA	= 0,
	EFUSE_OBJ_LOCK_STATUS,
} efuse_obj_info_e;

typedef struct {
	char name[48];
	unsigned char data[32];
	unsigned int size;
} efuse_obj_field_t;

int efuse_read_usr(char *buf, size_t count, loff_t *ppos);
int efuse_write_usr(char *buf, size_t count, loff_t *ppos);
uint32_t efuse_get_max(void);
ssize_t efuse_read(char *buf, size_t count, loff_t *ppos);
ssize_t efuse_write(const char *buf, size_t count, loff_t *ppos);

int32_t meson_trustzone_efuse(struct efuse_hal_api_arg *arg);
int32_t meson_trustzone_efuse_get_max(struct efuse_hal_api_arg *arg);
int64_t meson_trustzone_efuse_lockitem(const char *str);

#ifdef CONFIG_EFUSE_OBJ_API
uint32_t meson_efuse_obj_write(uint32_t obj_id, uint8_t *buff, uint32_t size);
uint32_t meson_efuse_obj_read(uint32_t obj_id, uint8_t *buff, uint32_t *size);
uint32_t efuse_obj_write(uint32_t obj_id, char *name, uint8_t *buff, uint32_t size);
uint32_t efuse_obj_read(uint32_t obj_id, char *name, uint8_t *buff, uint32_t *size);
#endif /* CONFIG_EFUSE_OBJ_API */
#endif

