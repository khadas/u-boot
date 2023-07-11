// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2014 Marek Vasut <marex@denx.de>
 *
 * Command for en/de-crypting block of memory with AES-[128/192/256]-CBC cipher.
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <aes.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <linux/compiler.h>

u32 aes_get_key_len(char *command)
{
	u32 key_len = AES128_KEY_LENGTH;

	if (!strcmp(command, "aes.192"))
		key_len = AES192_KEY_LENGTH;
	else if (!strcmp(command, "aes.256"))
		key_len = AES256_KEY_LENGTH;

	return key_len;
}

/**
 * do_aes() - Handle the "aes" command-line command
 * @cmdtp:	Command data struct pointer
 * @flag:	Command flag
 * @argc:	Command-line argument count
 * @argv:	Array of command-line arguments
 *
 * Returns zero on success, CMD_RET_USAGE in case of misuse and negative
 * on error.
 */
static int do_aes(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	u32 key_addr, iv_addr, src_addr, dst_addr, len;
	u8 *key_ptr, *iv_ptr, *src_ptr, *dst_ptr;
	u8 key_exp[AES256_EXPAND_KEY_LENGTH];
	u32 aes_blocks, key_len;
	int enc;

	if (argc != 7)
		return CMD_RET_USAGE;

	key_len = aes_get_key_len(argv[0]);

	if (!strncmp(argv[1], "enc", 3))
		enc = 1;
	else if (!strncmp(argv[1], "dec", 3))
		enc = 0;
	else
		return CMD_RET_USAGE;

	key_addr = simple_strtoul(argv[2], NULL, 16);
	iv_addr = simple_strtoul(argv[3], NULL, 16);
	src_addr = simple_strtoul(argv[4], NULL, 16);
	dst_addr = simple_strtoul(argv[5], NULL, 16);
	len = simple_strtoul(argv[6], NULL, 16);

	key_ptr = (uint8_t *)map_sysmem(key_addr, key_len);
	iv_ptr = (uint8_t *)map_sysmem(iv_addr, 128 / 8);
	src_ptr = (uint8_t *)map_sysmem(src_addr, len);
	dst_ptr = (uint8_t *)map_sysmem(dst_addr, len);

	/* First we expand the key. */
	aes_expand_key(key_ptr, key_len, key_exp);

	/* Calculate the number of AES blocks to encrypt. */
	aes_blocks = DIV_ROUND_UP(len, AES_BLOCK_LENGTH);

	if (enc)
		aes_cbc_encrypt_blocks(key_len, key_exp, iv_ptr, src_ptr,
				       dst_ptr, aes_blocks);
	else
		aes_cbc_decrypt_blocks(key_len, key_exp, iv_ptr, src_ptr,
				       dst_ptr, aes_blocks);

	return 0;
}

/***************************************************/
#ifdef CONFIG_SYS_LONGHELP
static char aes_help_text[] =
	"[.128,.192,.256] enc key iv src dst len - Encrypt block of data $len bytes long\n"
	"                             at address $src using a key at address\n"
	"                             $key with initialization vector at address\n"
	"                             $iv. Store the result at address $dst.\n"
	"                             The $len size must be multiple of 16 bytes.\n"
	"                             The $key and $iv must be 16 bytes long.\n"
	"aes [.128,.192,.256] dec key iv src dst len - Decrypt block of data $len bytes long\n"
	"                             at address $src using a key at address\n"
	"                             $key with initialization vector at address\n"
	"                             $iv. Store the result at address $dst.\n"
	"                             The $len size must be multiple of 16 bytes.\n"
	"                             The $key and $iv must be 16 bytes long.";
#endif

U_BOOT_CMD(
	aes, 7, 1, do_aes,
	"AES 128/192/256 CBC encryption",
	aes_help_text
);

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

static int aes_test_128ECB_SW(void)
{
	uint8_t dst[16] = {0};
	u8 key_exp[AES256_EXPAND_KEY_LENGTH];

	printf("AES 128 ECB ENC SW:");
	/* First we expand the key. */
	aes_expand_key(aes128ecb_key, sizeof(aes128ecb_key), key_exp);

	aes_ecb_encrypt_blocks(AES128_KEY_LENGTH, key_exp, aes128ecb_plain, dst,
			sizeof(aes128ecb_plain) / AES_BLOCK_LENGTH);
	if (!memcmp(dst, aes128ecb_cipher, sizeof(aes128ecb_cipher)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	printf("AES 128 ECB DEC SW:");
	aes_ecb_decrypt_blocks(AES128_KEY_LENGTH, key_exp, aes128ecb_cipher, dst,
			sizeof(aes128ecb_cipher) / AES_BLOCK_LENGTH);
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

static int aes_test_128CBC_SW(void)
{
	uint8_t dst[32] = {0};
	u8 key_exp[AES256_EXPAND_KEY_LENGTH];

	printf("AES 128 CBC ENC SW:");
	/* First we expand the key. */
	aes_expand_key(aes128cbc_key, sizeof(aes128cbc_key), key_exp);

	aes_cbc_encrypt_blocks(AES128_KEY_LENGTH, key_exp, aes128cbc_iv,
			aes128cbc_plain, dst, sizeof(aes128cbc_plain) / AES_BLOCK_LENGTH);
	if (!memcmp(dst, aes128cbc_cipher, sizeof(aes128cbc_cipher)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	printf("AES 128 CBC DEC SW:");
	aes_cbc_decrypt_blocks(AES128_KEY_LENGTH, key_exp, aes128cbc_iv,
			aes128cbc_cipher, dst, sizeof(aes128cbc_cipher) / AES_BLOCK_LENGTH);
	if (!memcmp(dst, aes128cbc_plain, sizeof(aes128cbc_plain)))
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

static int aes_test_256ECB_SW(void)
{
	uint8_t dst[32] = {0};
	u8 key_exp[AES256_EXPAND_KEY_LENGTH];

	printf("AES 256 ECB ENC SW:");

	/* First we expand the key. */
	aes_expand_key(aes256ecb_key, sizeof(aes256ecb_key), key_exp);

	aes_ecb_encrypt_blocks(AES256_KEY_LENGTH, key_exp, aes256ecb_plain, dst,
			sizeof(aes256ecb_plain) / AES_BLOCK_LENGTH);
	if (!memcmp(dst, aes256ecb_cipher, sizeof(aes256ecb_cipher)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	printf("AES 256 ECB DEC SW:");
	aes_ecb_decrypt_blocks(AES256_KEY_LENGTH, key_exp, aes256ecb_cipher, dst,
			sizeof(aes256ecb_cipher) / AES_BLOCK_LENGTH);
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

static int aes_test_256CBC_SW(void)
{
	uint8_t dst[32] = {0};
	u8 key_exp[AES256_EXPAND_KEY_LENGTH];

	printf("AES 256 CBC ENC SW:");

	/* First we expand the key. */
	aes_expand_key(aes256cbc_key, sizeof(aes256cbc_key), key_exp);

	aes_cbc_encrypt_blocks(AES256_KEY_LENGTH, key_exp, aes256cbc_iv,
			aes256cbc_plain, dst, sizeof(aes256cbc_plain) / AES_BLOCK_LENGTH);
	if (!memcmp(dst, aes256cbc_cipher, sizeof(aes256cbc_cipher)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	printf("AES 256 CBC DEC SW:");
	aes_cbc_decrypt_blocks(AES256_KEY_LENGTH, key_exp, aes256cbc_iv,
			aes256cbc_cipher, dst, sizeof(aes256cbc_cipher) / AES_BLOCK_LENGTH);
	if (!memcmp(dst, aes256cbc_plain, sizeof(aes256cbc_plain)))
		printf("PASS\n");
	else
		printf("FAIL\n");

	return 0;
}

static int aes_test_cases(uint32_t test_case)
{
	switch (test_case) {
	case 7:
		aes_test_128ECB_SW();
		aes_test_128CBC_SW();
		aes_test_256ECB_SW();
		aes_test_256CBC_SW();
		return 0;
	default:
		printf("Unknown test case\n");
		return 0;
	}
}

static int do_aes_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint32_t test_case = 0;
	char *endp = 0;

	if (argc != 2) {
		cmd_usage(cmdtp);
		return 1;
	}

	test_case = ustrtoul(argv[1], &endp, 0);

	aes_test_cases(test_case);

	return 0;
}

U_BOOT_CMD(aes_test, 2, 0, do_aes_test,
		"aes_test <test case>",
		"run small aes test cases, if case=0, run all\n"
	  );
#endif
