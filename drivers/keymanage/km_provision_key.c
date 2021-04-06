/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/keymanage/km_provision_key.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "key_manage_i.h"
#define _CMD_BUF_LEN 512
static char _cmdBuf[_CMD_BUF_LEN];

int keymanage_provision_init(const char *buf, int len)
{
	KM_DBG("f(%s)L%d\n", __func__, __LINE__);
	return 0;
}

int keymanage_provision_exit(void)
{
	KM_DBG("f(%s)L%d\n", __func__, __LINE__);
    return 0;
}

//must be hexdata if stored in efuse
int keymanage_provision_write(const char *keyname, const void* keydata, unsigned int datalen)
{
	KM_DBG("f(%s)L%d\n", __func__, __LINE__);

	snprintf(_cmdBuf, _CMD_BUF_LEN, "factory_provision write %s 0x%p 0x%x", keyname, keydata, datalen);
	int iRet = run_command(_cmdBuf, 0);
	KM_MSG("factory provision[%s], query sta=%d\n", keyname, iRet);

	return iRet;
}

ssize_t keymanage_provision_size(const char* keyname)
{
	KM_DBG("f(%s)L%d\n", __func__, __LINE__);
	uint32_t retBuf[12] = {0};
	snprintf(_cmdBuf, _CMD_BUF_LEN, "factory_provision query %s 0x%p", keyname, retBuf);
	int iRet = run_command(_cmdBuf, 0);
	if (iRet) {
		KM_MSG("factory provision[%s], query sta=%d\n", keyname, iRet);
		return 0;
	}

	return retBuf[0];
}

int keymanage_provision_exist(const char* keyname)
{
	KM_DBG("f(%s)L%d\n", __func__, __LINE__);
	snprintf(_cmdBuf, _CMD_BUF_LEN, "factory_provision query %s", keyname);
	int iRet = run_command(_cmdBuf, 0);
	if (iRet) KM_MSG("factory provision[%s], query sta=%d\n", keyname, iRet);

	return !iRet;
}

int keymanage_provision_query_can_read(const char* keyname)
{
	KM_DBG("f(%s)L%d\n", __func__, __LINE__);

	return 0;//always cannot be read
}

//read hash instead of actual value
int keymanage_provision_read(const char *keyname, void* databuf, const unsigned bufSz)
{
	KM_DBG("f(%s)L%d\n", __func__, __LINE__);
	uint32_t retBuf[12] = {0};
	snprintf(_cmdBuf, _CMD_BUF_LEN, "factory_provision query %s 0x%p", keyname, retBuf);
	int iRet = run_command(_cmdBuf, 0);
	if (iRet) {
		KM_MSG("factory provision[%s], query sta=%d\n", keyname, iRet);
		return __LINE__;
	}
	if (bufSz < 32) {
		KM_ERR("bufsz %d not enough for sha256sum\n", bufSz);
		return -__LINE__;
	}

	KM_MSG("provision read return sha256sum, but not original data\n");
	memcpy(databuf, retBuf + 1, 32);
	return 0;//
}

