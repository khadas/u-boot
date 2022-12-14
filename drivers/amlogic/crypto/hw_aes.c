// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <crypto_internal.h>
#include <amlogic/aml_crypto.h>

//#define AES_TEST_CASE

static int32_t aes_ecb_encrypt(void *key, uint32_t keylen,
		const void *pt, void *ct, size_t size)
{
	uint32_t mode = 0;

	if (keylen == AES_KEY_SIZE_128)
		mode = DMA_MODE_AES128;
	else if (keylen == AES_KEY_SIZE_256)
		mode = DMA_MODE_AES256;
	else
		return -1;

	return aes_cipher(key, keylen, NULL, pt, ct,
			1, mode, CIPHER_OP_MODE_ECB, size);
}

static int32_t aes_ecb_decrypt(void *key, uint32_t keylen,
		const void *ct, void *pt, size_t size)
{
	uint32_t mode = 0;

	if (keylen == AES_KEY_SIZE_128)
		mode = DMA_MODE_AES128;
	else if (keylen == AES_KEY_SIZE_256)
		mode = DMA_MODE_AES256;
	else
		return -1;

	return aes_cipher(key, keylen, NULL, ct, pt,
			0, mode, CIPHER_OP_MODE_ECB, size);
}

static int32_t aes_cbc_encrypt(void *key, uint32_t keylen, void *iv,
		const void *pt, void *ct, size_t size)
{
	uint32_t mode = 0;

	if (keylen == AES_KEY_SIZE_128)
		mode = DMA_MODE_AES128;
	else if (keylen == AES_KEY_SIZE_256)
		mode = DMA_MODE_AES256;
	else
		return -1;

	return aes_cipher(key, keylen, iv, pt, ct,
			1, mode, CIPHER_OP_MODE_CBC, size);
}

static int32_t aes_cbc_decrypt(void *key, uint32_t keylen, void *iv,
		const void *ct, void *pt, size_t size)
{
	uint32_t mode = 0;

	if (keylen == AES_KEY_SIZE_128)
		mode = DMA_MODE_AES128;
	else if (keylen == AES_KEY_SIZE_256)
		mode = DMA_MODE_AES256;
	else
		return -1;

	return aes_cipher(key, keylen, iv, ct, pt,
			0, mode, CIPHER_OP_MODE_CBC, size);
}

static int32_t aes_ctr_encrypt(void *key, uint32_t keylen, void *iv,
		const void *pt, void *ct, size_t size)
{
	uint32_t mode = 0;

	if (keylen == AES_KEY_SIZE_128)
		mode = DMA_MODE_AES128;
	else if (keylen == AES_KEY_SIZE_256)
		mode = DMA_MODE_AES256;
	else
		return -1;

	return aes_cipher(key, keylen, iv, pt, ct,
			1, mode, CIPHER_OP_MODE_CTR, size);
}

static int32_t aes_ctr_decrypt(void *key, uint32_t keylen, void *iv,
		const void *ct, void *pt, size_t size)
{
	return aes_ctr_encrypt(key, keylen, iv, ct, pt, size);
}

int32_t aes_ecb_enc_keytbl(uint32_t keylen, const void *input, void *output,
		uint8_t iv[16], size_t nbytes, uint32_t slot)
{
	if (slot > KEYTABLE_MAX_SLOT)
		return -1;

	if (keylen != AES_KEY_SIZE_128 && keylen != AES_KEY_SIZE_256)
		return -1;

	return aes_ecb_encrypt((void *)(uintptr_t)(uintptr_t)(KEYTABLE_BASE | slot),
			keylen, input, output, nbytes);
}

int32_t aes_ecb_dec_keytbl(uint32_t keylen, const void *input, void *output,
		uint8_t iv[16], size_t nbytes, uint32_t slot)
{
	if (slot > KEYTABLE_MAX_SLOT)
		return -1;

	if (keylen != AES_KEY_SIZE_128 && keylen != AES_KEY_SIZE_256)
		return -1;

	return aes_ecb_decrypt((void *)(uintptr_t)(KEYTABLE_BASE | slot),
			keylen, input, output, nbytes);
}

int32_t aes_cbc_enc_keytbl(uint32_t keylen, const void *input, void *output,
		uint8_t iv[16], size_t nbytes, uint32_t slot)
{
	if (slot > KEYTABLE_MAX_SLOT)
		return -1;

	if (keylen != AES_KEY_SIZE_128 && keylen != AES_KEY_SIZE_256)
		return -1;

	return aes_cbc_encrypt((void *)(uintptr_t)(KEYTABLE_BASE | slot),
			keylen, iv, input, output, nbytes);
}

int32_t aes_cbc_dec_keytbl(uint32_t keylen, const void *input, void *output, uint8_t iv[16],
		size_t nbytes, uint32_t slot)
{
	if (slot > KEYTABLE_MAX_SLOT)
		return -1;

	if (keylen != AES_KEY_SIZE_128 && keylen != AES_KEY_SIZE_256)
		return -1;

	return aes_cbc_decrypt((void *)(uintptr_t)(KEYTABLE_BASE | slot),
			keylen, iv, input, output, nbytes);
}

int32_t aes_ctr_encrypt_keytbl(uint32_t keylen, const void *input, void *output,
		uint8_t iv[16], size_t nbytes, uint32_t slot)
{
	if (slot > KEYTABLE_MAX_SLOT)
		return -1;

	if (keylen != AES_KEY_SIZE_128 && keylen != AES_KEY_SIZE_256)
		return -1;

	return aes_ctr_encrypt((void *)(uintptr_t)(KEYTABLE_BASE | slot),
			keylen, iv, input, output, nbytes);
}

int32_t aes_ctr_decrypt_keytbl(uint32_t keylen, const void *input, void *output,
		uint8_t iv[16], size_t nbytes, uint32_t slot)
{
	if (slot > KEYTABLE_MAX_SLOT)
		return -1;

	if (keylen != AES_KEY_SIZE_128 && keylen != AES_KEY_SIZE_256)
		return -1;

	return aes_ctr_decrypt((void *)(uintptr_t)(KEYTABLE_BASE | slot),
			keylen, iv, input, output, nbytes);
}

int32_t aes256cbc_iv0_dec_keytbl(const void *input, void *output, size_t nbytes,
		uint32_t slot)
{
	if (slot > KEYTABLE_MAX_SLOT)
		return -1;

	return aes_cbc_decrypt((void *)(uintptr_t)(KEYTABLE_BASE | slot),
			      AES_KEY_SIZE_256, NULL, input, output, nbytes);
}

#ifdef AES_TEST_CASE
static uint8_t aes128ecb_key[16] = {
	0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
	0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};

static uint8_t aes128ecb_plain[16] = {
	0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
	0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a
};

static uint8_t aes128ecb_cipher[16] = {
	0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60,
	0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97
};

static int aes_test_128ECB(void)
{
	uint8_t dst[16] = {0};

	printf("AES 128 ECB ENC:");
	aes_ecb_encrypt(aes128ecb_key, sizeof(aes128ecb_key), aes128ecb_plain,
			dst, sizeof(aes128ecb_plain));
	if (!memcmp(dst, aes128ecb_cipher, sizeof(aes128ecb_cipher)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	printf("AES 128 ECB DEC:");
	aes_ecb_decrypt(aes128ecb_key, sizeof(aes128ecb_key), aes128ecb_cipher,
			dst, sizeof(aes128ecb_cipher));
	if (!memcmp(dst, aes128ecb_plain, sizeof(aes128ecb_plain)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	return 0;
}

static uint8_t aes128cbc_key[16] = {
	0xc2, 0x86, 0x69, 0x6d, 0x88, 0x7c, 0x9a, 0xa0,
	0x61, 0x1b, 0xbb, 0x3e, 0x20, 0x25, 0xa4, 0x5a,
};

static uint8_t aes128cbc_iv[16] = {
	0x56, 0x2e, 0x17, 0x99, 0x6d, 0x09, 0x3d, 0x28,
	0xdd, 0xb3, 0xba, 0x69, 0x5a, 0x2e, 0x6f, 0x58
};

static uint8_t aes128cbc_plain[32] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
};

static uint8_t aes128cbc_cipher[32] = {
	0xd2, 0x96, 0xcd, 0x94, 0xc2, 0xcc, 0xcf, 0x8a,
	0x3a, 0x86, 0x30, 0x28, 0xb5, 0xe1, 0xdc, 0x0a,
	0x75, 0x86, 0x60, 0x2d, 0x25, 0x3c, 0xff, 0xf9,
	0x1b, 0x82, 0x66, 0xbe, 0xa6, 0xd6, 0x1a, 0xb1,
};

static int aes_test_128CBC(void)
{
	uint8_t dst[32] = {0};

	printf("AES 128 CBC ENC:");
	aes_cbc_encrypt(aes128cbc_key, sizeof(aes128cbc_key), aes128cbc_iv,
			aes128cbc_plain,
			dst, sizeof(aes128cbc_plain));
	if (!memcmp(dst, aes128cbc_cipher, sizeof(aes128cbc_cipher)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	printf("AES 128 CBC DEC:");
	aes_cbc_decrypt(aes128cbc_key, sizeof(aes128cbc_key), aes128cbc_iv,
			aes128cbc_cipher,
			dst, sizeof(aes128cbc_cipher));
	if (!memcmp(dst, aes128cbc_plain, sizeof(aes128cbc_plain)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	return 0;
}

static uint8_t aes128ctr_key[16] = {
	0x7e, 0x24, 0x06, 0x78, 0x17, 0xfa, 0xe0, 0xd7,
	0x43, 0xd6, 0xce, 0x1f, 0x32, 0x53, 0x91, 0x63,
};

static uint8_t aes128ctr_iv[16] = {
	0x00, 0x6c, 0xb6, 0xdb, 0xc0, 0x54, 0x3b, 0x59,
	0xda, 0x48, 0xd9, 0x0b, 0x00, 0x00, 0x00, 0x01,
};

static uint8_t aes128ctr_plain[32] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
};

static uint8_t aes128ctr_cipher[32] = {
	0x51, 0x04, 0xa1, 0x06, 0x16, 0x8a, 0x72, 0xd9,
	0x79, 0x0d, 0x41, 0xee, 0x8e, 0xda, 0xd3, 0x88,
	0xeb, 0x2e, 0x1e, 0xfc, 0x46, 0xda, 0x57, 0xc8,
	0xfc, 0xe6, 0x30, 0xdf, 0x91, 0x41, 0xbe, 0x28,
};

static int aes_test_128CTR(void)
{
	uint8_t dst[32] = {0};

	printf("AES 128 CTR ENC:");
	aes_ctr_encrypt(aes128ctr_key, sizeof(aes128ctr_key), aes128ctr_iv,
			aes128ctr_plain,
			dst, sizeof(aes128ctr_plain));
	if (!memcmp(dst, aes128ctr_cipher, sizeof(aes128ctr_cipher)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	printf("AES 128 CTR DEC:");
	aes_ctr_decrypt(aes128ctr_key, sizeof(aes128ctr_key), aes128ctr_iv,
			aes128ctr_cipher,
			dst, sizeof(aes128ctr_cipher));
	if (!memcmp(dst, aes128ctr_plain, sizeof(aes128ctr_plain)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	return 0;
}

static uint8_t aes256ecb_key[32] = {
	0x4a, 0xed, 0x92, 0x44, 0x1b, 0x24, 0x6d, 0x5c,
	0xdf, 0x04, 0xcf, 0x9e, 0x57, 0x86, 0x52, 0xc5,
	0xe3, 0xe7, 0x06, 0x82, 0xc2, 0x09, 0x4c, 0xac,
	0x62, 0x9f, 0x6f, 0xbe, 0xd8, 0x2c, 0x07, 0xcd,
};

static uint8_t aes256ecb_plain[32] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
};

static uint8_t aes256ecb_cipher[32] = {
	0x47, 0x9f, 0x34, 0x6c, 0x19, 0xb1, 0xf6, 0x21,
	0x3b, 0x81, 0xab, 0x69, 0xb1, 0x98, 0x38, 0x62,
	0xfb, 0x0e, 0x24, 0xe6, 0x54, 0x95, 0x98, 0x52,
	0xe4, 0xd5, 0x8c, 0xa2, 0x35, 0xca, 0x73, 0x18,
};

static int aes_test_256ECB(void)
{
	uint8_t dst[32] = {0};

	printf("AES 256 ECB ENC:");
	aes_ecb_encrypt(aes256ecb_key, sizeof(aes256ecb_key), aes256ecb_plain,
			dst, sizeof(aes256ecb_plain));
	if (!memcmp(dst, aes256ecb_cipher, sizeof(aes256ecb_cipher)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	printf("AES 256 ECB DEC:");
	aes_ecb_decrypt(aes256ecb_key, sizeof(aes256ecb_key), aes256ecb_cipher,
			dst, sizeof(aes256ecb_cipher));
	if (!memcmp(dst, aes256ecb_plain, sizeof(aes256ecb_plain)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	return 0;
}

static uint8_t aes256cbc_key[32] = {
	0x4a, 0xed, 0x92, 0x44, 0x1b, 0x24, 0x6d, 0x5c,
	0xdf, 0x04, 0xcf, 0x9e, 0x57, 0x86, 0x52, 0xc5,
	0xe3, 0xe7, 0x06, 0x82, 0xc2, 0x09, 0x4c, 0xac,
	0x62, 0x9f, 0x6f, 0xbe, 0xd8, 0x2c, 0x07, 0xcd,
};

static uint8_t aes256cbc_iv[16] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
};

static uint8_t aes256cbc_plain[32] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
};

static uint8_t aes256cbc_cipher[32] = {
	0x5b, 0x74, 0x90, 0x6f, 0x5b, 0xb2, 0x19, 0xac,
	0xf9, 0x02, 0x33, 0xd5, 0xdf, 0xa2, 0xa7, 0x93,
	0xed, 0xbd, 0xe8, 0xee, 0x99, 0x96, 0x4f, 0x0c,
	0x35, 0x61, 0x66, 0x37, 0x4f, 0xf9, 0xb9, 0xee,
};

static int aes_test_256CBC(void)
{
	uint8_t dst[32] = {0};

	printf("AES 256 CBC ENC:");
	aes_cbc_encrypt(aes256cbc_key, sizeof(aes256cbc_key), aes256cbc_iv,
			aes256cbc_plain,
			dst, sizeof(aes256cbc_plain));
	if (!memcmp(dst, aes256cbc_cipher, sizeof(aes256cbc_cipher)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	printf("AES 256 CBC DEC:");
	aes_cbc_decrypt(aes256cbc_key, sizeof(aes256cbc_key), aes256cbc_iv,
			aes256cbc_cipher,
			dst, sizeof(aes256cbc_cipher));
	if (!memcmp(dst, aes256cbc_plain, sizeof(aes256cbc_plain)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	return 0;
}

static uint8_t aes256ctr_key[32] = {
	0x4a, 0xed, 0x92, 0x44, 0x1b, 0x24, 0x6d, 0x5c,
	0xdf, 0x04, 0xcf, 0x9e, 0x57, 0x86, 0x52, 0xc5,
	0xe3, 0xe7, 0x06, 0x82, 0xc2, 0x09, 0x4c, 0xac,
	0x62, 0x9f, 0x6f, 0xbe, 0xd8, 0x2c, 0x07, 0xcd,
};

static uint8_t aes256ctr_iv[16] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
};

static uint8_t aes256ctr_plain[32] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
};

static uint8_t aes256ctr_cipher[32] = {
	0x47, 0x9e, 0x36, 0x6f, 0x1d, 0xb4, 0xf0, 0x26,
	0x33, 0x88, 0xa1, 0x62, 0xbd, 0x95, 0x36, 0x6d,
	0x1e, 0x5a, 0xc7, 0xfe, 0x71, 0xb8, 0x2e, 0xa0,
	0x45, 0xf2, 0xbc, 0x28, 0x36, 0xd1, 0x6a, 0x67,
};

static int aes_test_256CTR(void)
{
	uint8_t dst[32] = {0};

	printf("AES 256 CTR ENC:");
	aes_ctr_encrypt(aes256ctr_key, sizeof(aes256ctr_key), aes256ctr_iv,
			aes256ctr_plain,
			dst, sizeof(aes256ctr_plain));
	if (!memcmp(dst, aes256ctr_cipher, sizeof(aes256ctr_cipher)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	printf("AES 256 CTR DEC:");
	aes_ctr_decrypt(aes256ctr_key, sizeof(aes256ctr_key), aes256ctr_iv,
			aes256ctr_cipher,
			dst, sizeof(aes256ctr_cipher));
	if (!memcmp(dst, aes256ctr_plain, sizeof(aes256ctr_plain)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	return 0;
}

static int do_aes_test(uint32_t test_case)
{
	switch (test_case) {
	case 0:
		aes_test_128ECB();
		aes_test_128CBC();
		aes_test_128CTR();
		aes_test_256ECB();
		aes_test_256CBC();
		aes_test_256CTR();
		return 0;
	case 1:
		return aes_test_128ECB();
	case 2:
		return aes_test_128CBC();
	case 3:
		return aes_test_128CTR();
	case 4:
		return aes_test_256ECB();
	case 5:
		return aes_test_256CBC();
	case 6:
		return aes_test_256CTR();
	default:
		printf("Unknown test case\n");
		return 0;
	}
}

static int do_hw_aes_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint32_t test_case = 0;
	char *endp = 0;

	if (argc != 2) {
		cmd_usage(cmdtp);
		return 1;
	}

	test_case = ustrtoul(argv[1], &endp, 0);

	do_aes_test(test_case);

	return 0;
}

U_BOOT_CMD(aes_test, 2, 0, do_hw_aes_test,
		"aes_test <test case>",
		"run small aes test cases, if case=0, run all\n"
	  );
#endif
