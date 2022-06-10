/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __IMAGE_CHECK_H__
#define __IMAGE_CHECK_H__

#define SHA256_DIGEST_SIZE	32
typedef struct {
	uint32_t device_locked;
	uint32_t verified_boot_state;
	uint8_t verified_boot_key[SHA256_DIGEST_SIZE];
	uint8_t verified_boot_hash[SHA256_DIGEST_SIZE];
	uint32_t boot_patchlevel;
} keymaster_boot_params;

#endif /* __IMAGE_CHECK_H__ */
