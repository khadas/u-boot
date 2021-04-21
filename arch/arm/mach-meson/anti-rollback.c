// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <asm/arch/bl31_apis.h>
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
	register uint32_t x0 asm("x0") = FUNCID_ANTIROLLBACK_VERSION_CHECK;
	register uint32_t x1 asm("x1") = type;
	register uint32_t x2 asm("x2") = version;

	do {
		asm volatile(
			__asmeq("%0", "x0")
			__asmeq("%1", "x0")
			__asmeq("%2", "x1")
			__asmeq("%3", "x2")
			"smc	#0\n"
			: "=r"(x0)
			: "r"(x0), "r"(x1), "r"(x2));
	} while (0);

	return x0;
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
	register uint32_t x0 asm("x0") = FUNCID_AVB_VERSION_SET;
	register uint32_t x1 asm("x1") = index;
	register uint32_t x2 asm("x2") = version;

	do {
		asm volatile(
			__asmeq("%0", "x0")
			__asmeq("%1", "x0")
			__asmeq("%2", "x1")
			__asmeq("%3", "x2")
			"smc	#0\n"
			: "=r"(x0)
			: "r"(x0), "r"(x1), "r"(x2));
	} while (0);

	return 0 == x0;
}

bool get_avb_antirollback(uint32_t index, uint32_t* version)
{
	register uint32_t x0 asm("x0") = FUNCID_AVB_VERSION_GET;
	register uint32_t x1 asm("x1") = index;

	do {
		asm volatile(
			__asmeq("%0", "x0")
			__asmeq("%1", "x1")
			__asmeq("%2", "x0")
			__asmeq("%3", "x1")
			"smc	#0\n"
			: "=r"(x0), "=r"(x1)
			: "r"(x0), "r"(x1));
	} while (0);

	if (0 == x0)
		*version = x1;

	return 0 == x0;
}

bool get_avb_lock_state(uint32_t* lock_state)
{
	register uint32_t x0 asm("x0") = FUNCID_AVB_LOCK_STATE_GET;
	register uint32_t x1 asm("x1") = 0;

	do {
		asm volatile(
			__asmeq("%0", "x0")
			__asmeq("%1", "x1")
			__asmeq("%2", "x0")
			"smc	#0\n"
			: "=r"(x0), "=r"(x1)
			: "r"(x0));
	} while (0);

	if (0 == x0)
		*lock_state = x1;

	return 0 == x0;
}

bool avb_lock(void)
{
	register uint32_t x0 asm("x0") = FUNCID_AVB_LOCK;

	do {
		asm volatile(
			__asmeq("%0", "x0")
			__asmeq("%1", "x0")
			"smc	#0\n"
			: "=r"(x0)
			: "r"(x0));
	} while (0);

	return 0 == x0;
}

bool avb_unlock(void)
{
	register uint32_t x0 asm("x0") = FUNCID_AVB_UNLOCK;

	do {
		asm volatile(
			__asmeq("%0", "x0")
			__asmeq("%1", "x0")
			"smc	#0\n"
			: "=r"(x0)
			: "r"(x0));
	} while (0);

	return 0 == x0;
}
