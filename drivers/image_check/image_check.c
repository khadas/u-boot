// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <image.h>
#include <u-boot/sha256.h>
#include <amlogic/aml_efuse.h>
#include <amlogic/image_check.h>
#include <amlogic/aml_crypto.h>

extern struct scs_krnl_key_t __scs_krnl_devkey;

static int is_boot_image(unsigned long img_addr)
{
	if ((android_image_check_header((boot_img_hdr_t *)img_addr) == 0) ||
		(vendor_boot_image_check_header((vendor_boot_img_hdr_t *)img_addr) == 0))
		return 1;

	return 0;
}

int android_image_check_offset(void)
{
	return BOOT_IMG_V3_HDR_SIZE + sizeof(struct aml_boot_header_t);
}

/** kernel/misc image verify [and decrypt].
 * Returns 0 on success
 */
int secure_image_check(u8 *data, u32 size, unsigned long option)
{
	struct aml_boot_header_t hdr;
	int ret = AML_SECURE_BOOT_ERR_NONE;
	u8 *img = NULL;
	u8 buf[SHA256_DIGEST_SIZE];
	int secure_boot_enabled = IS_FEAT_BOOT_VERIFY();
	int encryption_enabled  = IS_FEAT_BOOT_ENCRYPT();
	int isbootimage = is_boot_image((unsigned long)data);
	const u64 magic = 0x38a41024204c4d40;
	struct rsa_public_key key;
	u32 workbuf[4 * MAX_RSANUMWORDS];

	if (!secure_boot_enabled)
		goto exit;

	if (memcmp(&__scs_krnl_devkey.cmn_hdr.magic_u64, &magic, sizeof(magic))) {
		ret =  AML_SECURE_BOOT_ERR_KEY;
		goto exit;
	}

	img = data + sizeof(struct aml_boot_header_t);

	if (isbootimage) {
		memcpy((void *)&hdr, (void *)(data + BOOT_IMG_V3_HDR_SIZE),
			sizeof(struct aml_boot_header_t));
		if (hdr.magic == AML_BOOT_IMAGE_MAGIC) {
			img += BOOT_IMG_V3_HDR_SIZE;
		} else {
			isbootimage = 0;
			memcpy((void *)&hdr, (void *)data, sizeof(struct aml_boot_header_t));
		}
	} else {
		memcpy((void *)&hdr, (void *)data, sizeof(struct aml_boot_header_t));
	}

	if (hdr.magic != AML_BOOT_IMAGE_MAGIC) {
		ret = AML_SECURE_BOOT_ERR_INVALID;
		goto exit;
	}

	/* check img payload is in mem region checked size */
	if (size < hdr.img_size + sizeof(struct aml_boot_header_t)) {
		ret = AML_SECURE_BOOT_ERR_SIZE;
		goto exit;
	}

	if (hdr.version != AML_BOOT_IMAGE_VERSION) {
		ret =  AML_SECURE_BOOT_ERR_VERSION;
		goto exit;
	}

	key.len = __scs_krnl_devkey.pubrsa_key.nword32;
	memcpy(key.n, __scs_krnl_devkey.pubrsa_key.n, key.len * sizeof(u32));
	memcpy(key.rr, __scs_krnl_devkey.pubrsa_key.rr, key.len * sizeof(u32));
	key.e = __scs_krnl_devkey.pubrsa_key.e;
	key.n0inv = *(u32 *)__scs_krnl_devkey.pubrsa_key.ninv128;

	sha256_csum_wd((u8 *)&hdr, sizeof(struct aml_boot_header_t) - 512, buf, 0);
	ret = image_rsa_verify(&key, hdr.rsa_sig, buf, workbuf);

	if (ret != 1) {
		ret = AML_SECURE_BOOT_ERR_HEADER_SIG;
		goto exit;
	} else {
		if (encryption_enabled) {
			flush_dcache_range((unsigned long)img, (unsigned long)img + hdr.img_size);
			aes256cbc_iv0_dec_keytbl(img, img, hdr.img_size,
					SBOOT_BL2_ENCRYPT_KEYSLOT);
		}

		sha256_csum_wd(img, hdr.img_size, buf, 0);

		if (memcmp(hdr.img_hash, buf, 32))
			ret = AML_SECURE_BOOT_ERR_IMG_HASH;
		else
			ret = AML_SECURE_BOOT_ERR_NONE;
	}

exit:

	memset(buf, 0, sizeof(buf));
	memset(&hdr, 0, sizeof(hdr));

	return ret;
}
