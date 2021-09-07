// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <malloc.h>
#include <libavb.h>

typedef struct AvbRSAPublicKey {
	uint32_t key_num_bits;
	uint32_t n0inv;
	/* payload contains n and rr separated by modulus length
	 * Since we support rsa2048/4096, the max length = 256
	 */
	uint32_t payload[256];
} AVB_ATTR_PACKED AvbRSAPublicKey;

typedef struct __scs_cmn_hdr {
	uint64_t magic_u64;
	uint16_t version_u16;
	uint16_t flags_u16;
	uint32_t length_u32;
} scs_cmn_hdr_t;

typedef struct __scs_pubrsa_key {
	scs_cmn_hdr_t cmn_hdr;

	struct {
		uint32_t nword32;
		uint32_t e;
		uint8_t rsvd_0008[8];
		uint8_t n[512];
		uint8_t rr[512];
		uint8_t ninv128[16];
	} pubrsa_key;
} scs_pubrsa_key_t;

extern scs_pubrsa_key_t __scs_bl33_devkey;

static int convert_fipkey_to_avbkey(scs_pubrsa_key_t *rsapub, AvbRSAPublicKey *avb_key)
{
	int32_t i = 0;
	const uint64_t magic = 0x38a41024204c4d40;
	uint32_t n0inv_tmp = 0;

	if (!rsapub || !avb_key)
		return -1;

	if (memcmp(&rsapub->cmn_hdr.magic_u64, &magic, sizeof(magic))) {
		printf("invalid magic for rsapub\n");
		return -1;
	}

	if (rsapub->pubrsa_key.e != 0x10001) {
		printf("AVB2 only supports exponent 0x10001, but got 0x%x\n", rsapub->pubrsa_key.e);
		return -1;
	}

	if (rsapub->pubrsa_key.nword32 != 64 && rsapub->pubrsa_key.nword32 != 128) {
		printf("AVB2 only supports RSA 2048/4096, but got %ld\n",
			rsapub->pubrsa_key.nword32 * sizeof(uint32_t) * 8);
		return -1;
	}

	avb_key->key_num_bits = avb_htobe32(rsapub->pubrsa_key.nword32 * sizeof(uint32_t) * 8);
	memcpy(&n0inv_tmp, rsapub->pubrsa_key.ninv128, sizeof(n0inv_tmp));
	avb_key->n0inv = avb_htobe32(n0inv_tmp);
	memset(avb_key->payload, 0, sizeof(avb_key->payload));
	for (i = 0; i < rsapub->pubrsa_key.nword32; i++) {
		uint32_t n = 0;
		uint32_t nword = rsapub->pubrsa_key.nword32;
		uint8_t *n_key = &rsapub->pubrsa_key.n[(nword - i - 1) * sizeof(uint32_t)];

		memcpy(&n, n_key, sizeof(uint32_t));
		avb_key->payload[i] = avb_htobe32(n);
	}
	for (i = 0; i < rsapub->pubrsa_key.nword32; i++) {
		uint32_t rr = 0;
		uint32_t nword = rsapub->pubrsa_key.nword32;
		uint8_t *rr_key = &rsapub->pubrsa_key.rr[(nword - i - 1) * sizeof(uint32_t)];

		memcpy(&rr, rr_key, sizeof(uint32_t));
		avb_key->payload[rsapub->pubrsa_key.nword32 + i] = avb_htobe32(rr);
	}

	return 0;
}

int compare_avbkey_with_fipkey(const uint8_t *public_key_data, size_t public_key_length)
{
	AvbRSAPublicKey avb_key;
	uint8_t *public_tmp = NULL;
	int err = -1;

	if (convert_fipkey_to_avbkey(&__scs_bl33_devkey, &avb_key)) {
		printf("cannot find fipkey\n");
		return -1;
	}

	public_tmp = malloc(public_key_length);
	if (!public_tmp) {
		printf("malloc tmp failed\n");
		return -1;
	}
	memcpy(public_tmp, public_key_data, public_key_length);

	if (!avb_safe_memcmp(public_tmp, &avb_key, public_key_length))
		err = 0;
	else
		err = -2;
	free(public_tmp);

	return err;
}
