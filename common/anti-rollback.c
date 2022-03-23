/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/anti-rollback.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <asm/arch/bl31_apis.h>
#include <linux/arm-smccc.h>
#include "anti-rollback.h"

#define FUNCID_ANTIROLLBACK_VERSION_CHECK     0xb2000010

#define FUNCID_AVB_VERSION_SET                0xb2000011
#define FUNCID_AVB_VERSION_GET                0xb2000012
#define FUNCID_AVB_LOCK_STATE_GET             0xb2000013
#define FUNCID_AVB_LOCK                       0xb2000014
#define FUNCID_AVB_UNLOCK                     0xb2000015

#define IMAGE_VERSION_TYPE_BOOTLOADER         0x001
#define IMAGE_VERSION_TYPE_RECOVERY           0x002
#define IMAGE_VERSION_TYPE_BOOT               0x003

#define KERNEL_TYPE_UNKNOWN                   0x00
#define KERNEL_TYPE_BOOT                      0x01
#define KERNEL_TYPE_RECOVERY                  0x02

static uint32_t antirollback_image_version_check(uint32_t type,
							uint32_t version)
{
	struct arm_smccc_res res;

	arm_smccc_smc(FUNCID_ANTIROLLBACK_VERSION_CHECK, type, version, 0, 0, 0, 0, 0, &res);

	return res.a0;
}

bool check_antirollback(uint32_t kernel_version)
{
	bool ret = true;
	uint32_t type = (kernel_version >> 24);
	uint32_t version = ((kernel_version << 8) >> 8);
	if (KERNEL_TYPE_BOOT == type) {
		if (antirollback_image_version_check(IMAGE_VERSION_TYPE_BOOT,
							version) != 0) {
			printf("checking boot.img version failed\n");
			ret = false;
		}
	}
	else if (KERNEL_TYPE_RECOVERY == type) {
		if (antirollback_image_version_check(
					IMAGE_VERSION_TYPE_RECOVERY,
					version) != 0) {
			printf("checking recovery.img version failed\n");
			ret = false;
		}
	}
	else {
		printf("the kernel type is unknown\n");
		ret = false;
	}

	if (ret)
		printf("checking version success\n");

	return ret;
}

bool set_avb_antirollback(uint32_t index, uint32_t version)
{
	struct arm_smccc_res res;

	arm_smccc_smc(FUNCID_AVB_VERSION_SET, index, version, 0, 0, 0, 0, 0, &res);

	return res.a0 == 0;
}

bool get_avb_antirollback(uint32_t index, uint32_t* version)
{
	struct arm_smccc_res res;

	arm_smccc_smc(FUNCID_AVB_VERSION_GET, index, 0, 0, 0, 0, 0, 0, &res);

	if (res.a0 == 0)
		*version = res.a1;

	return res.a0 == 0;
}

bool get_avb_lock_state(uint32_t* lock_state)
{
	struct arm_smccc_res res;

	arm_smccc_smc(FUNCID_AVB_LOCK_STATE_GET, 0, 0, 0, 0, 0, 0, 0, &res);

	if (res.a0 == 0)
		*lock_state = res.a1;

	return res.a0 == 0;
}

bool avb_lock(void)
{
	struct arm_smccc_res res;

	arm_smccc_smc(FUNCID_AVB_LOCK, 0, 0, 0, 0, 0, 0, 0, &res);

	return res.a0 == 0;
}

bool avb_unlock(void)
{
	struct arm_smccc_res res;

	arm_smccc_smc(FUNCID_AVB_UNLOCK, 0, 0, 0, 0, 0, 0, 0, &res);

	return res.a0 == 0;
}
