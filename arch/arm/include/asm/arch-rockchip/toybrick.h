/*
 * (C) Copyright 2021-2022 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __TOYBRICK_BOARD__
#define __TOYBRICK_BOARD__

#include <linux/string.h>
#include <asm/arch/vendor.h>
#include <common.h>
#include <dm.h>
#include <i2c.h>
#include <u-boot/sha256.h>
#include <adc.h>
#include <asm/gpio.h>
#include <optee_include/OpteeClientInterface.h>

#define TOYBRICK_SN_ID		SN_ID
#define TOYBRICK_MAC_ID		LAN_MAC_ID
#define TOYBRICK_ACTCODE_ID	0xa0

#define TOYBRICK_SN_LEN		64
#define TOYBRICK_MAC_LEN	6
#define TOYBRICK_ACTCODE_LEN	264

#define TOYBRICK_DATA_LEN	(TOYBRICK_SN_LEN + TOYBRICK_MAC_LEN + TOYBRICK_ACTCODE_LEN)
#define TOYBRICK_SHA_LEN	(TOYBRICK_DATA_LEN + SHA256_SUM_LEN)

#define TOYBRICK_FLAG_LEN	6
#define BOARD_THRESHOLDS_LEN	9

static inline int toybrick_get_sn(char *sn)
{
	return vendor_storage_read(TOYBRICK_SN_ID, sn, TOYBRICK_SN_LEN);
}

static inline int toybrick_set_sn(char *sn)
{
	return vendor_storage_write(TOYBRICK_SN_ID, sn, TOYBRICK_SN_LEN);
}

static inline int toybrick_get_mac(char *mac)
{
	return vendor_storage_read(TOYBRICK_MAC_ID, mac, TOYBRICK_MAC_LEN);
}

static inline int toybrick_set_mac(char *mac)
{
	return vendor_storage_write(TOYBRICK_MAC_ID, mac, TOYBRICK_MAC_LEN);
}

static inline int toybrick_get_actcode(char *actcode)
{
	return vendor_storage_read(TOYBRICK_ACTCODE_ID, actcode, TOYBRICK_ACTCODE_LEN);
}

static inline int toybrick_set_actcode(char *actcode)
{
	return vendor_storage_write(TOYBRICK_ACTCODE_ID, actcode, TOYBRICK_ACTCODE_LEN);
}

static inline int toybrick_get_flag(char *flag, int *index)
{
	const void *blob = gd->fdt_blob;
	int node, ret, i;
	u32 chns[2], ths[BOARD_THRESHOLDS_LEN], val, id;
	char sn[TOYBRICK_SN_LEN + 1];
	
	if (toybrick_get_sn(sn) <= 0)
		return -EINVAL;
	
	strncpy(flag, sn, TOYBRICK_FLAG_LEN);
	flag[TOYBRICK_SN_LEN] = 0;

	node = fdt_node_offset_by_compatible(blob, 0, "board-id");
	if (node < 0) {
		printf("Get board-id node failed\n");
		*index = -1;
		return 0;
	}

	ret = fdtdec_get_int(blob, node, "adc-io", -1);
	if (ret >= 0) {
		gpio_request(ret, "adc-io");
		val = gpio_get_value(ret);
		gpio_free(ret);
		printf("%s: adc-io(%d) value is %d\n", __func__, ret, val);
		if (val == 0) {
			*index = 0;
			return 0;
		}
	}

	ret = fdtdec_get_int_array(blob, node, "io-channels", chns, 2);
	if (ret) {
		printf("Get io-channels failed\n");
		*index = -1;
		return 0;
	}

	ret = fdtdec_get_int_array(blob, node, "thresholds", ths, BOARD_THRESHOLDS_LEN);
	if (ret) {
		printf("Get threshholds failed\n");
		*index = -1;
		return 0;
	}

	ret = adc_channel_single_shot("saradc", chns[1], &val);
	if (ret) {
		printf("Get adc value failed\n");
		*index = -1;
		return 0;
	}
	printf("%s: adc-value is %d\n", __func__, val);
	id = 1;
	for (i = 0; i < BOARD_THRESHOLDS_LEN; i++) {
		if (ths[i] >= val)
			break;

		id++;
	}
	*index = id;
	return 0;
}

#endif /* _TOYBRICK_BOARD_ */
