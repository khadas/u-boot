// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <libavb.h>
#include <asm/arch/bl31_apis.h>

#define MAX_AVBKEY_LEN (8 + 1024)

int compare_avbkey_with_fipkey(const uint8_t *public_key_data, size_t public_key_length)
{
	uint8_t public_key[MAX_AVBKEY_LEN];
	int32_t err = 0;

	if (public_key_length > MAX_AVBKEY_LEN) {
		printf("public key length is too large\n");
		return -1;
	}

	memset(public_key, 0, MAX_AVBKEY_LEN);
	err = get_avbkey_from_fip(public_key, MAX_AVBKEY_LEN);
	if (err != 0) {
		printf("failed to get avbkey from fip\n");
		return err;
	}

	if (!avb_safe_memcmp(public_key, public_key_data, public_key_length))
		err = 0;
	else
		err = -2;

	return err;
}
