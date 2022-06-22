// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include "key_manage_i.h"
#include <asm/arch/secure_apb.h>
#include <asm/io.h>

#include <amlogic/aml_efuse.h>

#define SECURE_BOOT_KEY_NAME    "secure_boot_set"
#define EFUSE_KEY_DGPK1			"KEY_EFUSE_DGPK1"

extern int efuse_usr_api_init_dtb(const char*  dt_addr);
extern int efuse_usr_api_get_cfg_key_size(const char* keyname, unsigned* pSz);
extern int efuse_usr_api_write_key(const char* keyname, const void* keydata, const unsigned dataSz);
extern int efuse_usr_api_read_key(const char* keyname, void* databuf, const unsigned bufSz);

int keymanage_efuse_init(const char *buf, int len)
{
    char ver;
    int ret = 0;

    const char* dtbLoadAddr = env_get("dtb_mem_addr");
    if (!dtbLoadAddr) {
        env_set("dtb_mem_addr", simple_itoa(CONFIG_SYS_SDRAM_BASE + (16U<<20)));
        dtbLoadAddr = env_get("dtb_mem_addr");
    }
    dtbLoadAddr = (char*)simple_strtoul(dtbLoadAddr, NULL, 0);

    ret = efuse_usr_api_init_dtb(dtbLoadAddr);
    if (ret) {
        KM_ERR("efuse init failed\n");
        return __LINE__;
    }

    ver = unifykey_get_efuse_version();
    if (ver == 0) {
        KM_DBG("efuse version not cfg\n");
        return 0;
    }

    //TODO: program the efuse version

    return ret;
}

int keymange_efuse_exit(void)
{
    return 0;
}

//must be hexdata if stored in efuse
int keymanage_efuse_write(const char *keyname, const void* keydata, unsigned int datalen)
{
	int ret = 0;

	if (!strcmp(SECURE_BOOT_KEY_NAME, keyname) || !strcmp(EFUSE_KEY_DGPK1, keyname)) {
		char _cmdbuf[96];

		sprintf(_cmdbuf, "efuse %s %p", SECURE_BOOT_KEY_NAME, keydata);
		ret = run_command(_cmdbuf, 0);
		if (ret)
			KM_ERR("FAil in cmd:%s\n", _cmdbuf);
	} else {
		ret = efuse_usr_api_write_key(keyname,  keydata, datalen);
	}

	return ret;
}

ssize_t keymanage_efuse_size(const char* keyname)
{
    int ret = 0;
    unsigned cfgSz = 0;

    ret = efuse_usr_api_get_cfg_key_size(keyname, &cfgSz);
    if (ret || !cfgSz) {
        KM_ERR("Fail at get cfg size for efuse key[%s]\n", keyname);
        return 0;
    }

    return cfgSz;
}

int keymanage_efuse_exist(const char* keyname)
{
	if (!strcmp(SECURE_BOOT_KEY_NAME, keyname)) {
		const unsigned long cfg10 = IS_FEAT_BOOT_VERIFY();

		KM_MSG("cfg10=0x%lX\n", cfg10);
		return cfg10;
	} else if (!strcmp(EFUSE_KEY_DGPK1, keyname)) {
		char *pattern = (char *)"dgpk1";
		int ret = efuse_check_pattern_item(pattern);

		KM_MSG("efuse check pattern ret %d\n", ret);
		if (ret < 0) {
			KM_ERR("efuse check pattern fail!\n");
			return -1;
		}
		KM_MSG("efuse %s is %s\n", pattern, ret > 0 ? "written" : "not write");
		return ret > 0;
	}
	{
		int ret = 0;
		const ssize_t cfgSz = keymanage_efuse_size(keyname);
		char *databuf = NULL;
		int isEmpty = 1;
		int i = 0;

		databuf = (char *)malloc(cfgSz);
		if (!databuf) {
			KM_ERR("Fail to alloc bufsz 0x%x for key %s\n", (unsigned)cfgSz, keyname);
			return 0;
		}
		ret = keymanage_efuse_read(keyname, databuf, cfgSz);
		if (ret) {
			KM_ERR("Fail at read key[%s]\n", keyname);
			goto _exit;
		}
		for (i = 0; i < cfgSz && isEmpty; ++i)
			isEmpty = !databuf[i];

_exit:
		free(databuf);
		return !isEmpty;
	}

	return __LINE__;
}

int keymanage_efuse_query_can_read(const char* keyname)
{
	if (!strcmp(SECURE_BOOT_KEY_NAME, keyname) || !strcmp(EFUSE_KEY_DGPK1, keyname))
		return 0;
	return 1;//user space always can be read
}

//data format is hexdata
int keymanage_efuse_read(const char *keyname, void* databuf, const unsigned bufSz)
{
    int ret = 0;
    unsigned cfgSz = 0;

    cfgSz = keymanage_efuse_size(keyname);
    if (cfgSz > bufSz) {
        KM_ERR("buf sz 0x%x < dts cfg sz 0x%x\n", bufSz, cfgSz);
        return __LINE__;
    }

    ret = efuse_usr_api_read_key(keyname, databuf, cfgSz);

    return ret;
}

