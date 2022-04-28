/*
 * (C) Copyright 2021-2022 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __TOYBRICK_CHECK_BOARD__
#define __TOYBRICK_CHECK_BOARD__

#include <optee_include/OpteeClientInterface.h>
#include <asm/arch/toybrick.h>

static inline int load_SnMacAc_from_vendor(char *sn, char *mac, char *actcode)
{
	int ret;

	memset(sn, 0, TOYBRICK_SN_LEN + 1);
	memset(mac, 0, TOYBRICK_MAC_LEN + 1);
	memset(actcode, 0, TOYBRICK_ACTCODE_LEN + 1);

	ret = toybrick_get_sn(sn);
	if (ret <= 0) {
		printf("Load sn form vendor failed\n");
		return -EIO;
	}

	ret = toybrick_get_mac(mac);
	if (ret != TOYBRICK_MAC_LEN) {
		printf("Load mac form vendor failed\n");
		return -EIO;
	}

	ret = toybrick_get_actcode(actcode);
	if (ret != TOYBRICK_ACTCODE_LEN) {
		printf("Load actcode form vendor failed\n");
		return -EIO;
	}

	printf("Load SnMacAc from vendor: sn %s, mac %2.2x%2.2x%2.2x%2.2x%2.2x%2.2x\n",
			sn, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return 0;
}

static inline int save_SnMacAc_to_vendor(char *sn, char *mac, char *actcode)
{
	int ret;

	ret = toybrick_set_sn(sn);
	if (ret <= 0) {
		printf("Save sn to vendor failed\n");
		return -EIO;
	}

	ret = toybrick_set_mac(mac);
	if (ret != TOYBRICK_MAC_LEN) {
		printf("Save mac to vendor failed\n");
		return -EIO;
	}

	ret = toybrick_set_actcode(actcode);
	if (ret != TOYBRICK_ACTCODE_LEN) {
		printf("Save actcode to vendor failed\n");
		return -EIO;
	}

	return 0;
}

static inline int load_SnMacAc_from_rpmb(char *sn, char *mac, char *actcode)
{
	int ret;
	sha256_context ctx;
	uint8_t digest[SHA256_SUM_LEN + 1] = {0};
	uint8_t hash_pre[SHA256_SUM_LEN + 1] = {0};
	uint8_t data_sha256[TOYBRICK_SHA_LEN + 1]={0};

	memset(sn, 0, TOYBRICK_SN_LEN + 1);
	memset(mac, 0, TOYBRICK_MAC_LEN + 1);
	memset(actcode, 0, TOYBRICK_ACTCODE_LEN + 1);
	ret = trusty_read_toybrick_SnMacAc(data_sha256, TOYBRICK_SHA_LEN);
	if (ret != 0) {
		printf("Load SnMacAc from rpmb failed\n");
		return -EIO;
	}
	memcpy(hash_pre, data_sha256, SHA256_SUM_LEN);
	sha256_starts(&ctx);
	sha256_update(&ctx,(const uint8_t *)(data_sha256 + SHA256_SUM_LEN), TOYBRICK_DATA_LEN);
	sha256_finish(&ctx, digest);
	if (memcmp(digest, hash_pre, SHA256_SUM_LEN) != 0) {
		printf("SnMacAc from rpmb is invalid\n");
		return -EINVAL;
	}
	memcpy(sn, data_sha256 + SHA256_SUM_LEN, TOYBRICK_SN_LEN);
	memcpy(mac, data_sha256 + SHA256_SUM_LEN + TOYBRICK_SN_LEN, TOYBRICK_MAC_LEN);
	memcpy(actcode, data_sha256 + SHA256_SUM_LEN + TOYBRICK_SN_LEN + TOYBRICK_MAC_LEN, TOYBRICK_ACTCODE_LEN);

	if (strlen(sn) == 0) {
		printf("SnMacAc from rpmb is empty\n");
		return -EINVAL;
	}

	printf("Load SnMacAc from rpmb: sn %s, mac %2.2x%2.2x%2.2x%2.2x%2.2x%2.2x\n",
			sn, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return 0;
}

static inline int save_SnMacAc_to_rpmb(char *sn, char *mac, char *actcode)
{
	int ret;
	sha256_context ctx;
	uint8_t digest[SHA256_SUM_LEN + 1] = {0};
	uint8_t data[TOYBRICK_DATA_LEN + 1]={0};
	uint8_t data_sha256[TOYBRICK_SHA_LEN + 1]={0};

	memset(&data, 0, sizeof(data));
	memset(&data_sha256, 0, sizeof(data_sha256));
	memcpy(data, sn, TOYBRICK_SN_LEN);
	memcpy(data + TOYBRICK_SN_LEN, mac, TOYBRICK_MAC_LEN);
	memcpy(data + TOYBRICK_SN_LEN + TOYBRICK_MAC_LEN, actcode, TOYBRICK_ACTCODE_LEN);

	sha256_starts(&ctx);
	sha256_update(&ctx,(const uint8_t *)data, TOYBRICK_DATA_LEN);
	sha256_finish(&ctx, digest);
	memcpy(data_sha256, digest, SHA256_SUM_LEN);
	memcpy(data_sha256 + SHA256_SUM_LEN, data, TOYBRICK_DATA_LEN);
	
	ret = trusty_write_toybrick_SnMacAc(data_sha256, TOYBRICK_SHA_LEN);
	if (ret != 0) {
		printf("Save SnMacAc to rpmb failed\n");
		return -EIO;
	}

	return 0;
}

static inline int toybrick_check_SnMacAc(void)
{
	int ret = 0;
	int ret_vendor, ret_rpmb;
	char vendor_sn[TOYBRICK_SN_LEN + 1];
	char vendor_mac[TOYBRICK_MAC_LEN + 1];
	char vendor_actcode[TOYBRICK_ACTCODE_LEN + 1];
	char rpmb_sn[TOYBRICK_SN_LEN + 1];
	char rpmb_mac[TOYBRICK_MAC_LEN + 1];
	char rpmb_actcode[TOYBRICK_ACTCODE_LEN + 1];

	ret_vendor = load_SnMacAc_from_vendor(vendor_sn, vendor_mac, vendor_actcode);
	ret_rpmb = load_SnMacAc_from_rpmb(rpmb_sn, rpmb_mac, rpmb_actcode);

	if (ret_vendor < 0 && ret_rpmb < 0) {
		printf("No SnMacAc found in vendor and rpmb, goto loader ...\n");
		run_command_list("rockusb 0 ${devtype} ${devnum}", -1, 0);
		//set_back_to_bootrom_dnl_flag();
		do_reset(NULL, 0, 0, NULL);
	} else if (ret_vendor < 0) {
		printf("No SnMacAc found in vendor, load from rpmb and save to vendor\n");
		ret = save_SnMacAc_to_vendor(rpmb_sn, rpmb_mac, rpmb_actcode);
		do_reset(NULL, 0, 0, NULL);
	} else if (ret_rpmb < 0) {
		printf("No SnMacAc found in rpmb, load from vendor and save to rpmb\n");
		ret = save_SnMacAc_to_rpmb(vendor_sn, vendor_mac, vendor_actcode);
	} else if (memcmp(vendor_sn, rpmb_sn, TOYBRICK_SN_LEN) != 0){
		printf("Warn: SN(%s %s) form vendor and rpmb is different!\n",
				vendor_sn, rpmb_sn);
		ret = save_SnMacAc_to_vendor(rpmb_sn, rpmb_mac, rpmb_actcode);
		do_reset(NULL, 0, 0, NULL);
	} else if (memcmp(vendor_mac, rpmb_mac, TOYBRICK_MAC_LEN) != 0){
		printf("Warn: MAC form vendor and rpmb is different!\n");
		ret = save_SnMacAc_to_vendor(rpmb_sn, rpmb_mac, rpmb_actcode);
		do_reset(NULL, 0, 0, NULL);
	} else if (memcmp(vendor_actcode, rpmb_actcode, TOYBRICK_ACTCODE_LEN) != 0){
		printf("Warn: Actcode form vendor and rpmb is different!\n");
		ret = save_SnMacAc_to_vendor(rpmb_sn, rpmb_mac, rpmb_actcode);
		do_reset(NULL, 0, 0, NULL);
	} else {
		printf("Toybrick check SnMacAc OK, sn %s\n", vendor_sn);
		ret = 0;
	}

	return ret;
}

#endif
