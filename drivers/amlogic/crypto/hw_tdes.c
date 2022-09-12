// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <crypto_internal.h>
#include <amlogic/aml_crypto.h>

//#define TDES_TEST_CASE

static int32_t des_tdes_ecb_encrypt(void *key, uint32_t keylen,
		const void *pt, void *ct, size_t size)
{
	uint32_t mode = 0;

	if (keylen == TDES_2K_KEY_SIZE)
		mode = DMA_MODE_TDES_2K;
	else if (keylen == TDES_3K_KEY_SIZE)
		mode = DMA_MODE_TDES_3K;
	else
		mode = DMA_MODE_DES;

	return des_tdes_cipher(key, keylen, NULL, pt, ct,
			1, mode, CIPHER_OP_MODE_ECB, size);
}

static int32_t des_tdes_ecb_decrypt(void *key, uint32_t keylen,
		const void *ct, void *pt, size_t size)
{
	uint32_t mode = 0;

	if (keylen == TDES_2K_KEY_SIZE)
		mode = DMA_MODE_TDES_2K;
	else if (keylen == TDES_3K_KEY_SIZE)
		mode = DMA_MODE_TDES_3K;
	else
		mode = DMA_MODE_DES;

	return des_tdes_cipher(key, keylen, NULL, ct, pt,
			0, mode, CIPHER_OP_MODE_ECB, size);
}

static int32_t des_tdes_cbc_encrypt(void *key, uint32_t keylen, void *iv,
		const void *pt, void *ct, size_t size)
{
	uint32_t mode = 0;

	if (keylen == TDES_2K_KEY_SIZE)
		mode = DMA_MODE_TDES_2K;
	else if (keylen == TDES_3K_KEY_SIZE)
		mode = DMA_MODE_TDES_3K;
	else
		mode = DMA_MODE_DES;

	return des_tdes_cipher(key, keylen, iv, pt, ct,
			1, mode, CIPHER_OP_MODE_CBC, size);
}

static int32_t des_tdes_cbc_decrypt(void *key, uint32_t keylen, void *iv,
		const void *ct, void *pt, size_t size)
{
	uint32_t mode = 0;

	if (keylen == TDES_2K_KEY_SIZE)
		mode = DMA_MODE_TDES_2K;
	else if (keylen == TDES_3K_KEY_SIZE)
		mode = DMA_MODE_TDES_3K;
	else
		mode = DMA_MODE_DES;

	return des_tdes_cipher(key, keylen, iv, ct, pt,
			0, mode, CIPHER_OP_MODE_CBC, size);
}

int32_t des_tdes_ecb_enc_keytbl(uint32_t keylen, const void *input,
		void *output, size_t nbytes, uint32_t slot)
{
	if (slot > KEYTABLE_MAX_SLOT)
		return -1;

	if (keylen != DES_KEY_SIZE && keylen != TDES_2K_KEY_SIZE)
		return -1;

	return des_tdes_ecb_encrypt((void *)(uintptr_t)(KEYTABLE_BASE | slot),
			keylen, input, output, nbytes);
}

int32_t des_tdes_ecb_dec_keytbl(uint32_t keylen, const void *input,
		void *output, uint8_t iv[16], size_t nbytes, uint32_t slot)
{
	if (slot > KEYTABLE_MAX_SLOT)
		return -1;

	if (keylen != DES_KEY_SIZE && keylen != TDES_2K_KEY_SIZE)
		return -1;

	return des_tdes_ecb_decrypt((void *)(uintptr_t)(KEYTABLE_BASE | slot),
			keylen, input, output, nbytes);
}

int32_t des_tdes_cbc_enc_keytbl(uint32_t keylen, const void *input,
		void *output, uint8_t iv[8], size_t nbytes, uint32_t slot)
{
	if (slot > KEYTABLE_MAX_SLOT)
		return -1;

	if (keylen != DES_KEY_SIZE && keylen != TDES_2K_KEY_SIZE)
		return -1;

	return des_tdes_cbc_encrypt((void *)(uintptr_t)(KEYTABLE_BASE | slot),
			keylen, iv, input, output, nbytes);
}

int32_t des_tdes_cbc_dec_keytbl(uint32_t keylen, const void *input,
		void *output, uint8_t iv[8], size_t nbytes, uint32_t slot)
{
	if (slot > KEYTABLE_MAX_SLOT)
		return -1;

	if (keylen != DES_KEY_SIZE && keylen != TDES_2K_KEY_SIZE)
		return -1;

	return des_tdes_cbc_decrypt((void *)(uintptr_t)(KEYTABLE_BASE | slot),
			keylen, iv, input, output, nbytes);
}

#ifdef TDES_TEST_CASE
static uint8_t des_ecb_key[8] = {
	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef
};

static uint8_t des_ecb_plain[8] = {
	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xe7
};

static uint8_t des_ecb_cipher[8] = {
	0xc9, 0x57, 0x44, 0x25, 0x6a, 0x5e, 0xd3, 0x1d
};

static int des_test_ECB(void)
{
	uint8_t dst[8] = {0};

	printf("DES ECB ENC:");
	des_tdes_ecb_encrypt(des_ecb_key, sizeof(des_ecb_key), des_ecb_plain,
			dst, sizeof(des_ecb_plain));
	if (!memcmp(dst, des_ecb_cipher, sizeof(des_ecb_cipher)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	printf("DES ECB DEC:");
	des_tdes_ecb_decrypt(des_ecb_key, sizeof(des_ecb_key), des_ecb_cipher,
			dst, sizeof(des_ecb_cipher));
	if (!memcmp(dst, des_ecb_plain, sizeof(des_ecb_plain)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	return 0;
}

static uint8_t des_cbc_key[8] = {
	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef
};

static uint8_t des_cbc_iv[8] = {
	0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10
};

static uint8_t des_cbc_plain[24] = {
	0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x20,
	0x4e, 0x6f, 0x77, 0x20, 0x69, 0x73, 0x20, 0x74,
	0x68, 0x65, 0x20, 0x74, 0x69, 0x6d, 0x65, 0x20
};

static uint8_t des_cbc_cipher[24] = {
	0xcc, 0xd1, 0x73, 0xff, 0xab, 0x20, 0x39, 0xf4,
	0xac, 0xd8, 0xae, 0xfd, 0xdf, 0xd8, 0xa1, 0xeb,
	0x46, 0x8e, 0x91, 0x15, 0x78, 0x88, 0xba, 0x68,
};

static int des_test_CBC(void)
{
	uint8_t dst[24] = {0};

	printf("DES CBC ENC:");
	des_tdes_cbc_encrypt(des_cbc_key, sizeof(des_cbc_key), des_cbc_iv,
			des_cbc_plain,
			dst, sizeof(des_cbc_plain));
	if (!memcmp(dst, des_cbc_cipher, sizeof(des_cbc_cipher)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	printf("DES CBC DEC:");
	des_tdes_cbc_decrypt(des_cbc_key, sizeof(des_cbc_key), des_cbc_iv,
			des_cbc_cipher,
			dst, sizeof(des_cbc_cipher));
	if (!memcmp(dst, des_cbc_plain, sizeof(des_cbc_plain)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	return 0;
}

static uint8_t tdes_3k_ecb_key[24] = {
	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10
};

static uint8_t tdes_3k_ecb_plain[8] = {
	0x73, 0x6f, 0x6d, 0x65, 0x64, 0x61, 0x74, 0x61
};

static uint8_t tdes_3k_ecb_cipher[8] = {
	0x18, 0xd7, 0x48, 0xe5, 0x63, 0x62, 0x05, 0x72
};

static int tdes_3k_test_ECB(void)
{
	uint8_t dst[8] = {0};

	printf("TDES 3K ECB ENC:");
	des_tdes_ecb_encrypt(tdes_3k_ecb_key, sizeof(tdes_3k_ecb_key), tdes_3k_ecb_plain,
			dst, sizeof(tdes_3k_ecb_plain));
	if (!memcmp(dst, tdes_3k_ecb_cipher, sizeof(tdes_3k_ecb_cipher)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	printf("TDES 3K ECB DEC:");
	des_tdes_ecb_decrypt(tdes_3k_ecb_key, sizeof(tdes_3k_ecb_key), tdes_3k_ecb_cipher,
			dst, sizeof(tdes_3k_ecb_cipher));
	if (!memcmp(dst, tdes_3k_ecb_plain, sizeof(tdes_3k_ecb_plain)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	return 0;
}

static uint8_t tdes_2k_cbc_key[] = {
	0x38, 0x12, 0xA4, 0x19, 0xC6, 0x3B, 0xE7, 0x71,
	0xAD, 0x9F, 0x61, 0xFE, 0xFA, 0x20, 0xCE, 0x63,
};

static uint8_t tdes_2k_cbc_iv[] = {
	0xCC, 0xF3, 0x45, 0x4D, 0x1E, 0x9C, 0xCD, 0xE0
};

static uint8_t tdes_2k_cbc_plain[40] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
};

static uint8_t tdes_2k_cbc_cipher[40] = {
	0x5d, 0x50, 0x5d, 0xd9, 0x9b, 0xc8, 0xac, 0x65,
	0xb2, 0x5d, 0x48, 0xd2, 0xa6, 0x75, 0x56, 0x30,
	0xd9, 0x87, 0xee, 0xed, 0xd7, 0xe1, 0x0f, 0x63,
	0x0e, 0xae, 0x50, 0x63, 0xec, 0x17, 0xb0, 0xc6,
	0x8e, 0x37, 0x12, 0x4a, 0x8e, 0x4c, 0x6b, 0x13
};

static int tdes_2k_test_CBC(void)
{
	uint8_t dst[40] = {0};

	printf("TDES 2K CBC ENC:");
	des_tdes_cbc_encrypt(tdes_2k_cbc_key, sizeof(tdes_2k_cbc_key), tdes_2k_cbc_iv,
			tdes_2k_cbc_plain,
			dst, sizeof(tdes_2k_cbc_plain));
	if (!memcmp(dst, tdes_2k_cbc_cipher, sizeof(tdes_2k_cbc_cipher)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	printf("TDES 2K CBC DEC:");
	des_tdes_cbc_decrypt(tdes_2k_cbc_key, sizeof(tdes_2k_cbc_key), tdes_2k_cbc_iv,
			tdes_2k_cbc_cipher,
			dst, sizeof(tdes_2k_cbc_cipher));
	if (!memcmp(dst, tdes_2k_cbc_plain, sizeof(tdes_2k_cbc_plain)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	return 0;
}

static uint8_t tdes_3k_cbc_key[] = {
	0xE9, 0xC0, 0xFF, 0x2E, 0x76, 0x0B, 0x64, 0x24,
	0x44, 0x4D, 0x99, 0x5A, 0x12, 0xD6, 0x40, 0xC0,
	0xEA, 0xC2, 0x84, 0xE8, 0x14, 0x95, 0xDB, 0xE8
};

static uint8_t tdes_3k_cbc_iv[] = {
	0x7D, 0x33, 0x88, 0x93, 0x0F, 0x93, 0xB2, 0x42
};

static uint8_t tdes_3k_cbc_plain[128] = {
	0x6f, 0x54, 0x20, 0x6f, 0x61, 0x4d, 0x79, 0x6e,
	0x53, 0x20, 0x63, 0x65, 0x65, 0x72, 0x73, 0x74,
	0x54, 0x20, 0x6f, 0x6f, 0x4d, 0x20, 0x6e, 0x61,
	0x20, 0x79, 0x65, 0x53, 0x72, 0x63, 0x74, 0x65,
	0x20, 0x73, 0x6f, 0x54, 0x20, 0x6f, 0x61, 0x4d,
	0x79, 0x6e, 0x53, 0x20, 0x63, 0x65, 0x65, 0x72,
	0x73, 0x74, 0x54, 0x20, 0x6f, 0x6f, 0x4d, 0x20,
	0x6e, 0x61, 0x20, 0x79, 0x65, 0x53, 0x72, 0x63,
	0x74, 0x65, 0x20, 0x73, 0x6f, 0x54, 0x20, 0x6f,
	0x61, 0x4d, 0x79, 0x6e, 0x53, 0x20, 0x63, 0x65,
	0x65, 0x72, 0x73, 0x74, 0x54, 0x20, 0x6f, 0x6f,
	0x4d, 0x20, 0x6e, 0x61, 0x20, 0x79, 0x65, 0x53,
	0x72, 0x63, 0x74, 0x65, 0x20, 0x73, 0x6f, 0x54,
	0x20, 0x6f, 0x61, 0x4d, 0x79, 0x6e, 0x53, 0x20,
	0x63, 0x65, 0x65, 0x72, 0x73, 0x74, 0x54, 0x20,
	0x6f, 0x6f, 0x4d, 0x20, 0x6e, 0x61, 0x0a, 0x79
};

static uint8_t tdes_3k_cbc_cipher[128] = {
	0x0e, 0x2d, 0xb6, 0x97, 0x3c, 0x56, 0x33, 0xf4,
	0x67, 0x17, 0x21, 0xc7, 0x6e, 0x8a, 0xd5, 0x49,
	0x74, 0xb3, 0x49, 0x05, 0xc5, 0x1c, 0xd0, 0xed,
	0x12, 0x56, 0x5c, 0x53, 0x96, 0xb6, 0x00, 0x7d,
	0x90, 0x48, 0xfc, 0xf5, 0x8d, 0x29, 0x39, 0xcc,
	0x8a, 0xd5, 0x35, 0x18, 0x36, 0x23, 0x4e, 0xd7,
	0x76, 0xd1, 0xda, 0x0c, 0x94, 0x67, 0xbb, 0x04,
	0x8b, 0xf2, 0x03, 0x6c, 0xa8, 0xcf, 0xb6, 0xea,
	0x22, 0x64, 0x47, 0xaa, 0x8f, 0x75, 0x13, 0xbf,
	0x9f, 0xc2, 0xc3, 0xf0, 0xc9, 0x56, 0xc5, 0x7a,
	0x71, 0x63, 0x2e, 0x89, 0x7b, 0x1e, 0x12, 0xca,
	0xe2, 0x5f, 0xaf, 0xd8, 0xa4, 0xf8, 0xc9, 0x7a,
	0xd6, 0xf9, 0x21, 0x31, 0x62, 0x44, 0x45, 0xa6,
	0xd6, 0xbc, 0x5a, 0xd3, 0x2d, 0x54, 0x43, 0xcc,
	0x9d, 0xde, 0xa5, 0x70, 0xe9, 0x42, 0x45, 0x8a,
	0x6b, 0xfa, 0xb1, 0x91, 0x13, 0xb0, 0xd9, 0x19
};

static int tdes_3k_test_CBC(void)
{
	uint8_t dst[128] = {0};

	printf("TDES 3K CBC ENC:");
	des_tdes_cbc_encrypt(tdes_3k_cbc_key, sizeof(tdes_3k_cbc_key), tdes_3k_cbc_iv,
			tdes_3k_cbc_plain,
			dst, sizeof(tdes_3k_cbc_plain));
	if (!memcmp(dst, tdes_3k_cbc_cipher, sizeof(tdes_3k_cbc_cipher)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	printf("TDES 3K CBC DEC:");
	des_tdes_cbc_decrypt(tdes_3k_cbc_key, sizeof(tdes_3k_cbc_key), tdes_3k_cbc_iv,
			tdes_3k_cbc_cipher,
			dst, sizeof(tdes_3k_cbc_cipher));
	if (!memcmp(dst, tdes_3k_cbc_plain, sizeof(tdes_3k_cbc_plain)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	return 0;
}

static int do_des_tdes_test(uint32_t test_case)
{
	switch (test_case) {
	case 0:
		des_test_ECB();
		des_test_CBC();
		tdes_3k_test_ECB();
		tdes_2k_test_CBC();
		tdes_3k_test_CBC();
		return 0;
	case 1:
		return des_test_ECB();
	case 2:
		return des_test_CBC();
	case 3:
		return tdes_3k_test_ECB();
	case 4:
		return tdes_2k_test_CBC();
	case 5:
		return tdes_3k_test_CBC();
	default:
		printf("Unknown test case\n");
		return 0;
	}
}

static int do_hw_des_tdes_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint32_t test_case = 0;
	char *endp = 0;

	if (argc != 2) {
		cmd_usage(cmdtp);
		return 1;
	}

	test_case = ustrtoul(argv[1], &endp, 0);

	do_des_tdes_test(test_case);

	return 0;
}

U_BOOT_CMD(tdes_test, 2, 0, do_hw_des_tdes_test,
		"tdes_test <test case>",
		"run small des/tdes test cases, if case=0, run all\n"
	  );
#endif
