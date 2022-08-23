/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/include/asm/arch-t5d/efuse.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
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

typedef struct efuse_cali {
	unsigned int revision:2;
	unsigned int reserved1:6;
	unsigned int psensor_data:15;
	unsigned int psensor_flag:1;
	unsigned int saradc_data:6;
	unsigned int saradc_flag:1;
	unsigned int usbphy_data:4;
	unsigned int usbphy_flag:1;
	unsigned int reserved2:4;
	unsigned int mipicsi_data:4;
	unsigned int mipicsi_flag:1;
	unsigned int hdmirx_data:4;
	unsigned int hdmirx_flag:1;
	unsigned int ethernet_data:5;
	unsigned int ethernet_flag:1;
	unsigned int cvbs_data:7;
	unsigned int cvbs_flag:1;
} efuse_cali_t;

#define EFUSE_BYTES				512   /* (EFUSE_BITS/8) */
#define EFUSE_CALI_SIZE			8
#define EFUSE_CALI_CVBS

#define EFUSE_HAL_API_READ	0
#define EFUSE_HAL_API_WRITE 1
#define EFUSE_HAL_API_WRITE_PATTERN 2
#define EFUSE_HAL_API_USER_MAX 3
#define EFUSE_HAL_API_READ_CALI 4
#define EFUSE_HAL_API_READ_CALI_ITEM 5
#define EFUSE_HAL_API_CHECKPATTERN_ITEM  6

#define EFUSE_USER_MASK            (0x1 << 16)
#define EFUSE_THERMAL_MASK         (0x1 << 17)
#define EFUSE_THERMAL_VERFLAG_MASK (0x1 << 18)
#define EFUSE_ENCRYPT_MASK         (0x1 << 19)

//#define ASSIST_HW_REV                              0x1f53

int efuse_read_usr(char *buf, size_t count, loff_t *ppos);
int efuse_write_usr(char *buf, size_t count, loff_t *ppos);
uint32_t efuse_get_max(void);
ssize_t efuse_read(char *buf, size_t count, loff_t *ppos);
ssize_t efuse_write(const char *buf, size_t count, loff_t *ppos);

int32_t meson_trustzone_efuse(struct efuse_hal_api_arg *arg);
int32_t meson_trustzone_efuse_get_max(struct efuse_hal_api_arg *arg);
int64_t meson_trustzone_efuse_lockitem(const char *str);

#endif

