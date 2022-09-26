/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _CRYPTO_INTERNAL_H
#define _CRYPTO_INTERNAL_H

#include <common.h>
#include <u-boot/sha256.h>

#define USE_SHA_OLD_CODE
//#define HASH_FLUSH_INPUT

/* DMA operation mode */
#define CIPHER_OP_MODE_ECB 0
#define CIPHER_OP_MODE_CBC 1
#define CIPHER_OP_MODE_CTR 2

#define HASH_OP_MODE_SHA1     1
#define HASH_OP_MODE_SHA256   2
#define HASH_OP_MODE_SHA224   3

/* DMA mode */
#define DMA_MODE_DMA     0x0
#define DMA_MODE_KEY     0x1
#define DMA_MODE_MEMSET  0x2
/* 0x3  skipped */
/* 0x4  skipped */
#define DMA_MODE_SHA1    0x5
#define DMA_MODE_SHA256  0x6
#define DMA_MODE_SHA224  0x7
#define DMA_MODE_AES128  0x8
#define DMA_MODE_AES192  0x9
#define DMA_MODE_AES256  0xa
#define DMA_MODE_S17     0xb
#define DMA_MODE_DES     0xc
/* 0xd  skipped */
#define DMA_MODE_TDES_2K 0xe
#define DMA_MODE_TDES_3K 0xf

#define DMA_BLOCK_SIZE (0x200)
#define MAX_BLOCK_TRANSFER (0x1ffff)

#define KEYTABLE_BASE (0xffffff00)
#define KEYTABLE_MAX_SLOT (0xff)

struct dma_dsc {
	union {
		uint32_t d32;
		struct {
		    unsigned length:17;
		    unsigned irq:1;
		    unsigned eoc:1;
		    unsigned loop:1;
		    unsigned mode:4;
		    unsigned begin:1;
		    unsigned end:1;
		    unsigned op_mode:2;
		    unsigned enc_sha_only:1;
		    unsigned block:1;
		    unsigned error:1;
		    unsigned owner:1;
		} b;
	} dsc_cfg;
	uint32_t src_addr;
	uint32_t tgt_addr;
};

/*
 * aes_cipher - aes cipher
 *
 * @key - key pointer, can be actually key buffer, or key table addr
 * @keylen - length of key
 * @iv - AES IV
 * @src - src pointer
 * @dst - dst pointer
 * @encrypt - encrypt or decrypt
 * @mode - DMA mode
 * @op_mode - DMA operation mode
 * @size - data length
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t aes_cipher(void *key, uint32_t keylen, uint8_t iv[16],
		   const void *src, void *dst, uint8_t encrypt, uint8_t mode,
		   uint8_t op_mode, size_t size);

/*
 * des_tdes_cipher - des and tdes cipher
 *
 * @key - key pointer, can be actually key buffer, or key table addr
 * @keylen - length of key
 * @iv - DES/TDES IV
 * @src - src pointer
 * @dst - dst pointer
 * @encrypt - encrypt or decrypt
 * @mode - DMA mode
 * @op_mode - DMA operation mode
 * @size - data length
 * @return - on successful, 0 and negative value, otherwise.
 */

int32_t des_tdes_cipher(void *key, uint32_t keylen, uint8_t iv[8],
		   const void *src, void *dst, uint8_t encrypt, uint8_t mode,
		   uint8_t op_mode, size_t size);

/*
 * hw_update_internal - Internal logic for SHA update
 *
 * @cur_ctx - SHA context
 * @input - input pointer
 * @ilen - input length
 * @hash - hash buffer
 * @last_update - to finalize context
 * @return - on successful, 0 and negative value, otherwise.
 */
int32_t hw_update_internal(sha2_ctx *cur_ctx, const uint8_t *input,
		uint32_t ilen, uint8_t *hash, uint8_t last_update);
#endif
