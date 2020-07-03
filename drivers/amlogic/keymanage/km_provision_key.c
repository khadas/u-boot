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
	uint32_t retBuf[12];
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
	uint32_t retBuf[12];
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

