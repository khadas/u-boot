/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/efuse/efuse.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <asm/arch/io.h>
#include <asm/arch/efuse.h>
#include "efuse_regs.h"

char efuse_buf[EFUSE_BYTES] = {0};

ssize_t efuse_read(char *buf, size_t count, loff_t *ppos)
{
	unsigned pos = *ppos;

	struct efuse_hal_api_arg arg;
	unsigned int retcnt;
	int ret;
	arg.cmd = EFUSE_HAL_API_READ;
	arg.offset = pos;
	arg.size = count;
	arg.buffer_phy = (unsigned long)buf;
	arg.retcnt_phy = (unsigned long)&retcnt;
	ret = meson_trustzone_efuse(&arg);
	if (ret == 0) {
		*ppos += retcnt;
		return retcnt;
	} else{
		return ret;
	}
}

ssize_t efuse_write(const char *buf, size_t count, loff_t *ppos)
{
	unsigned pos = *ppos;

	if ((pos&0xffff) >= EFUSE_BYTES)
		return 0;	/* Past EOF */
	if (count > EFUSE_BYTES - pos)
		count = EFUSE_BYTES - pos;
	if (count > EFUSE_BYTES)
		return -1;

	struct efuse_hal_api_arg arg;
	unsigned int retcnt;
	arg.cmd = EFUSE_HAL_API_WRITE;
	arg.offset = pos;
	arg.size = count;
	arg.buffer_phy = (unsigned long)buf;
	arg.retcnt_phy = (unsigned long)&retcnt;
	int ret;
	ret = meson_trustzone_efuse(&arg);
	if (ret == 0) {
		*ppos = retcnt;
		return retcnt;
	} else{
		return ret;
	}
}

int efuse_read_usr(char *buf, size_t count, loff_t *ppos)
{
	char data[EFUSE_BYTES];
	char *pdata = NULL;
	int ret;
	loff_t pos;

	memset(data, 0, count);

	pdata = data;
	pos = *ppos;
	ret = efuse_read(pdata, count, (loff_t *)&pos);

	memcpy(buf, data, count);

	return ret;
}

int efuse_check_pattern_item(char *str)
{
#ifdef EFUSE_HAL_API_CHECKPATTERN_ITEM
	s64 ret;

	//extern int64_t meson_trustzone_efuse_lockitem(const char *str);
	ret = meson_trustzone_efuse_lockitem(str);
	return ret;
#else
	return -1;
#endif
}

int efuse_write_usr(char *buf, size_t count, loff_t *ppos)
{
	char data[EFUSE_BYTES];
	char *pdata = NULL;
	char *penc = NULL;
	int ret;
	loff_t pos;

	if (count == 0) {
		printf("data length: 0 is error!\n");
		return -1;
	}

	memset(data, 0, EFUSE_BYTES);
	memset(efuse_buf, 0, EFUSE_BYTES);

	memcpy(data, buf, count);
	pdata = data;
	penc = efuse_buf;

	memcpy(penc, pdata, count);
	pos = *ppos;

	ret = efuse_write(efuse_buf, count, (loff_t *)&pos);

	return ret;
}

uint32_t efuse_get_max(void)
{
	struct efuse_hal_api_arg arg;
	int ret;
	arg.cmd = EFUSE_HAL_API_USER_MAX;

	ret = meson_trustzone_efuse_get_max(&arg);

	if (ret == 0) {
		printf("ERROR: can not get efuse user max bytes!!!\n");
		return -1;
	} else{
		return ret;
	}
}

#ifdef CONFIG_EFUSE_OBJ_API
uint32_t efuse_obj_write(uint32_t obj_id, uint8_t *buff, uint32_t size)
{
	return meson_efuse_obj_write(obj_id, buff, size);
}

uint32_t efuse_obj_read(uint32_t obj_id, uint8_t *buff, uint32_t *size)
{
	return meson_efuse_obj_read(obj_id, buff, size);
}
#endif /* CONFIG_EFUSE_OBJ_API */

int efuse_read_cali(char *buf, size_t count, uint32_t offset)
{
#ifdef EFUSE_HAL_API_READ_CALI
	char data[EFUSE_BYTES];
	struct efuse_hal_api_arg arg;
	unsigned int retcnt;
	int ret;

	memset(data, 0, count);

	arg.cmd = EFUSE_HAL_API_READ_CALI;
	arg.offset = offset;
	arg.size = count;
	arg.buffer_phy = (unsigned long)data;
	arg.retcnt_phy = (unsigned long)&retcnt;
	ret = meson_trustzone_efuse(&arg);
	if (ret == 0) {
		memcpy(buf, data, count);
		return retcnt;
	} else{
		return ret;
	}

	return ret;
#else
	return -1;
#endif
}

int efuse_get_cali_cvbs(void)
{
#ifdef EFUSE_CALI_CVBS
	efuse_cali_t cali_data;
	int ret;

	ret = efuse_read_cali((char *)&(cali_data), EFUSE_CALI_SIZE, 0x00);
	if (ret == EFUSE_CALI_SIZE && cali_data.cvbs_flag)
		return cali_data.cvbs_data;
#endif
	return -1;
}

/*
 * return: >=0:succ and valid data, <0:fail
*/
int efuse_get_cali_item(char *str)
{
#ifdef EFUSE_HAL_API_READ_CALI_ITEM
	int64_t ret;

	extern int64_t meson_trustzone_efuse_caliItem(const char *str);
	ret = meson_trustzone_efuse_caliItem(str);

	return ret;
#else
	return -1;
#endif
}

