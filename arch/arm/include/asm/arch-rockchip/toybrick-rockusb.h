/*
 * (C) Copyright 2021-2022 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __TOYBRICK_ROCKUSB_BOARD__
#define __TOYBRICK_ROCKUSB_BOARD__

#include <optee_include/OpteeClientInterface.h>
#include <asm/arch/toybrick.h>

static inline int toybrick_write_SnMacActcode(struct fsg_lun *curlun, void *data)
{
	int rc;
	uint32_t *p;

	p = (uint32_t *)((char __user *)data + 8 + TOYBRICK_SN_LEN + TOYBRICK_MAC_LEN);
	if (trusty_write_toybrick_seed(p) != 0) {//Seed
		printf("trusty_write_toybrick_seed error!");
		curlun->sense_data = SS_WRITE_ERROR;
		return -EIO;
	}

	rc = toybrick_set_sn((char __user *)data + 8);
	if (rc < 0) {
		curlun->sense_data = SS_WRITE_ERROR;
		return -EIO;
	}

	rc = toybrick_set_mac((char __user *)data + 8 + TOYBRICK_SN_LEN);
	if (rc < 0) {
		curlun->sense_data = SS_WRITE_ERROR;
		return -EIO;
	}

	rc = toybrick_set_actcode((char __user *)data + 8 + TOYBRICK_SN_LEN + TOYBRICK_MAC_LEN + 12 + 16);
	if (rc < 0) {
		curlun->sense_data = SS_WRITE_ERROR;
		return -EIO;
	}

	return rc;
}

static inline int toybrick_write_extrakey(struct fsg_lun *curlun, struct vendor_item *vhead, void *data)
{
	if (vhead->size - 8 != 0x16f) {
		printf("check extrakey size fail!\n");
		curlun->sense_data = SS_WRITE_ERROR;
		return -EIO;
	}

	if (trusty_write_toybrick_extrakey((unsigned char *)data + 8, 0x16f) != 0) {
		printf("trusty_write_toybrick_extrakey error!");
		curlun->sense_data = SS_WRITE_ERROR;
		return -EIO;
	}

	return 0;
}

static inline int toybrick_read_SnMacActcode(struct fsg_common *common, struct fsg_lun *curlun, struct vendor_item *vhead, void *data)
{
	int rc;

	rc = toybrick_get_sn((char __user *)data);
	if (!rc) {
		curlun->sense_data = SS_UNRECOVERED_READ_ERROR;
		return -EIO;
	}

	rc = toybrick_get_mac((char __user *)data + TOYBRICK_SN_LEN);
	if (!rc) {
        	curlun->sense_data = SS_UNRECOVERED_READ_ERROR;
		return -EIO;
	}

	//Seed, not read for security
	if (trusty_read_toybrick_cpu_id((uint8_t *)data + 64 + 6 + 12) != 0) {//Chip id
		printf("trusty_read_toybrick_cpu_id error!");
		curlun->sense_data = SS_UNRECOVERED_READ_ERROR;
		return -EIO;
	}

	rc = toybrick_get_actcode((char __user *)data + TOYBRICK_SN_LEN + TOYBRICK_MAC_LEN + 12 + 16);
	if (!rc) {
		curlun->sense_data = SS_UNRECOVERED_READ_ERROR;
		return -EIO;
	}

	vhead->size = common->data_size - 8;
	return rc;
}

#endif /* _TOYBRICK_BOARD_ */
