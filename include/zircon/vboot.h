/* SPDX-License-Identifier:     BSD-3-Clause */
/*
 * Copyright (c) 2019 The Fuchsia Authors
 *
 */

#ifndef _ZIRCON_VBOOT_H_
#define _ZIRCON_VBOOT_H_

#ifdef CONFIG_LIBAVB_ATX
#include <libavb_atx/libavb_atx.h>
#endif

/**
 * zircon_vboot_slot_verify() - Verifies a preloaded kernel if the device is locked.
 * If unlocked, this returns 0.
 *
 * @loadaddr: The address of the preloaded kernel.
 * @img_size: The size in bytes of the preloaded kernel.
 * @ab_suffix: The slot suffix of the loaded kernel (eg. "_a", "_r", etc.)
 * @has_successfully_booted: True if this image has successfully booted in the past
 *                           The rollback indices are only updated if this is true.
 *
 * Returns: 0 on successful verification / device unlocked, -1 otherwise.
 */
int zircon_vboot_slot_verify(unsigned char *loadaddr, uint64_t img_size,
			     const char *ab_suffix,
			     bool has_successfully_booted);

#ifdef CONFIG_LIBAVB_ATX
extern AvbIOResult
avb_read_permanent_attributes(AvbAtxOps *atx_ops,
			      AvbAtxPermanentAttributes * attributes);

extern AvbIOResult
avb_read_permanent_attributes_hash(AvbAtxOps *atx_ops,
				   uint8_t hash[AVB_SHA256_DIGEST_SIZE]);

/**
 * zircon_vboot_generate_unlock_challenge() - Generates an unlock challenge.
 *
 * @out_unlock_challenge: The output unlock challenge.
 *
 * Returns: 0 on success, -1 otherwise.
 */
int zircon_vboot_generate_unlock_challenge(AvbAtxUnlockChallenge
		*out_unlock_challenge);

/**
 * zircon_vboot_validate_unlock_credential() - Validates an unlock credential.
 *
 * @unlock_credential: The credential to validate.
 * @out_is_trusted: Set to true if the credential is trusted, false otherwise.
 *
 * Returns: 0 on success, -1 otherwise.
 */
int zircon_vboot_validate_unlock_credential(AvbAtxUnlockCredential
		*unlock_credential, bool *out_is_trusted);
#else

extern const char avb2_kpub_fuchsia[];
extern const int avb2_kpub_fuchsia_len;
extern const char avb2_kpub_production[];
extern const int avb2_kpub_production_len;

#endif

#endif // _ZIRCON_VBOOT_H_
