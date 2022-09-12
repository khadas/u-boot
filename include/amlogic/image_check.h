/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __IMAGE_CHECK_H__
#define __IMAGE_CHECK_H__

#define AML_SECURE_BOOT_ERR_NONE         0
#define AML_SECURE_BOOT_ERR_KEY         -1
#define AML_SECURE_BOOT_ERR_INVALID     -2
#define AML_SECURE_BOOT_ERR_SIZE        -3
#define AML_SECURE_BOOT_ERR_VERSION     -4
#define AML_SECURE_BOOT_ERR_HEADER_SIG  -5
#define AML_SECURE_BOOT_ERR_IMG_HASH    -6

#define AML_BOOT_IMAGE_MAGIC    (0x4C4D4140) //"@AML"
#define AML_BOOT_IMAGE_VERSION   0x1

#define SHA256_DIGEST_SIZE	32
typedef struct {
	uint32_t device_locked;
	uint32_t verified_boot_state;
	uint8_t verified_boot_key[SHA256_DIGEST_SIZE];
	uint8_t verified_boot_hash[SHA256_DIGEST_SIZE];
	uint32_t boot_patchlevel;
} keymaster_boot_params;

#define MAX_RSA_KEY_SIZE 4096
#define MAX_RSANUMBYTES ((MAX_RSA_KEY_SIZE) / 8)
#define MAX_RSANUMWORDS (MAX_RSANUMBYTES / sizeof(u32))

#define SBOOT_BL2_ENCRYPT_KEYSLOT	134
#define SBOOT_BL2_ENCRYPT_KEY_LEN	32	// AES256

/* RSA public key definition */
struct rsa_public_key {
	u32 n[MAX_RSANUMWORDS];  /* modulus as little endian array */
	u32 e;               /* Public exponent e */
	u32 rr[MAX_RSANUMWORDS]; /* R^2 as little endian array */
	u32 n0inv;           /* -1 / n[0] mod 2^32 */
	u32 len;             /* Length of n[] in number of u32 */
};

struct aml_boot_header_t {
	u32 magic;
	u32 version;
	u32 flags;
	u32 img_version;
	u32 img_size;
	u32 img_offset;
	u8 img_hash[32];
	u8 reserved[200];
	u8 rsa_sig[512];
};

struct scs_cmn_hdr_t {
	u64 magic_u64;
	u16 version_u16;
	u16 flags_u16;
	u32 length_u32;
};

struct scs_krnl_key_t {
	struct scs_cmn_hdr_t cmn_hdr;

	struct {
		u32 nword32;
		u32 e;
		u8 rsvd_0008[8];
		u8 n[512];
		u8 rr[512];
		u8 ninv128[16];
	} pubrsa_key;
};

int android_image_check_offset(void);
int secure_image_check(u8 *data, u32 size, unsigned long option);
int image_rsa_verify(const struct rsa_public_key *key, const u8 *signature,
	       const u8 *sha, u32 *workbuf32);
#endif /* __IMAGE_CHECK_H__ */
