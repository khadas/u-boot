// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <linux/compat.h>
#include <common.h>
#include <stdio.h>
#include <string.h>
#include <dma.h>
#include <asm/arch/regs.h>
#include <asm/arch/secure_apb.h>
#include <amlogic/aml_crypto.h>
#include <crypto_internal.h>

//#define CRYPTO_DEBUG

/* lock for crypto T0 if needed in the future */
DEFINE_MUTEX(&crypto_lock);

static int32_t cipher_core(void *key, uint32_t keylen, uint8_t *iv, uint32_t ivlen,
		   const void *src, void *dst, uint8_t encrypt, uint8_t mode,
		   uint8_t op_mode, size_t size)
{
	struct dma_dsc dsc[4];
	uint8_t iv0[16] = {0};
	uint32_t blocks = size / DMA_BLOCK_SIZE;
	uint32_t residues = size & (DMA_BLOCK_SIZE - 1);
	int32_t ret = 0;
#ifdef CRYPTO_DEBUG
	uint32_t i = 0;
#endif

	if (blocks > MAX_BLOCK_TRANSFER)
		return CRYPTO_ERROR_BAD_PARAMETERS;

	mutex_lock(&crypto_lock);

	if (!iv)
		iv = iv0;

	dsc[0].src_addr = (uintptr_t)key;
	dsc[0].tgt_addr = 0;
	dsc[0].dsc_cfg.d32 = 0;
	/* HW bug, key has to set to multiple of 16 bytes, even for des */
	dsc[0].dsc_cfg.b.length = 32;
	dsc[0].dsc_cfg.b.mode = DMA_MODE_KEY;
	dsc[0].dsc_cfg.b.owner = 1;

	dsc[1].src_addr = (uintptr_t)iv;
	dsc[1].tgt_addr = 32;
	dsc[1].dsc_cfg.d32 = 0;
	/* HW bug, IV has to set to multiple of 16 bytes, even for des */
	dsc[1].dsc_cfg.b.length = 16;
	dsc[1].dsc_cfg.b.mode = DMA_MODE_KEY;
	dsc[1].dsc_cfg.b.owner = 1;

	if (blocks) {
		dsc[2].src_addr = (uintptr_t)src;
		dsc[2].tgt_addr = (uintptr_t)dst;
		dsc[2].dsc_cfg.d32 = 0;
		dsc[2].dsc_cfg.b.length = blocks;
		dsc[2].dsc_cfg.b.eoc = 0;
		dsc[2].dsc_cfg.b.mode = mode;
		dsc[2].dsc_cfg.b.op_mode = op_mode;
		dsc[2].dsc_cfg.b.enc_sha_only = encrypt;
		dsc[2].dsc_cfg.b.block = 1;
		dsc[2].dsc_cfg.b.owner = 1;

		dsc[3].src_addr = (uintptr_t)src + blocks * DMA_BLOCK_SIZE;
		dsc[3].tgt_addr = (uintptr_t)dst + blocks * DMA_BLOCK_SIZE;
		dsc[3].dsc_cfg.d32 = 0;
		dsc[3].dsc_cfg.b.length = residues;
		dsc[3].dsc_cfg.b.eoc = 1;
		dsc[3].dsc_cfg.b.mode = mode;
		dsc[3].dsc_cfg.b.op_mode = op_mode;
		dsc[2].dsc_cfg.b.enc_sha_only = encrypt;
		dsc[3].dsc_cfg.b.block = 0;
		dsc[3].dsc_cfg.b.owner = 1;
	} else {
		dsc[2].src_addr = (uintptr_t)src;
		dsc[2].tgt_addr = (uintptr_t)dst;
		dsc[2].dsc_cfg.d32 = 0;
		dsc[2].dsc_cfg.b.length = residues;
		dsc[2].dsc_cfg.b.eoc = 1;
		dsc[2].dsc_cfg.b.mode = mode;
		dsc[2].dsc_cfg.b.op_mode = op_mode;
		dsc[2].dsc_cfg.b.enc_sha_only = encrypt;
		dsc[2].dsc_cfg.b.block = 0;
		dsc[2].dsc_cfg.b.owner = 1;
	}

	/* key */
	if (dsc[0].src_addr < KEYTABLE_BASE)
		flush_dcache_range((uintptr_t)dsc[0].src_addr,
				(uintptr_t)dsc[0].src_addr + 32);
	/* iv */
	flush_dcache_range((uintptr_t)dsc[1].src_addr, (unsigned long)dsc[1].src_addr + 16);
	/* dsc */
	flush_dcache_range((uintptr_t)&dsc, (uintptr_t)&dsc + sizeof(dsc));
	flush_dcache_range((uintptr_t)src, (uintptr_t)src + size);
	flush_dcache_range((uintptr_t)dst, (uintptr_t)dst + size);

	*P_DMA_STS0 = 0xf;
	*P_DMA_T0 = (uintptr_t)dsc | 2;
	while (*P_DMA_STS0 == 0)
		;

	if (*P_DMA_STS0 & 0x1)
		ret = CRYPTO_ERROR_BAD_PROCESS;
	else
		ret = CRYPTO_ERROR_NO_ERROR;

	invalidate_dcache_range((uintptr_t)dst, (uintptr_t)dst + size);

	mutex_unlock(&crypto_lock);
#ifdef CRYPTO_DEBUG
	for (i = 0; i < sizeof(dsc) / sizeof(struct dma_dsc); i++) {
		printf("desc (%4x) (len) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.length);
		printf("desc (%4x) (irq) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.irq);
		printf("desc (%4x) (eoc) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.eoc);
		printf("desc (%4x) (lop) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.loop);
		printf("desc (%4x) (mod) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.mode);
		printf("desc (%4x) (beg) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.begin);
		printf("desc (%4x) (end) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.end);
		printf("desc (%4x) (opm) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.op_mode);
		printf("desc (%4x) (enc) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.enc_sha_only);
		printf("desc (%4x) (blk) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.block);
		printf("desc (%4x) (err) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.error);
		printf("desc (%4x) (own) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.owner);
		printf("desc (%4x) (src) = 0x%8x\n", i,
				dsc[i].src_addr);
		printf("desc (%4x) (tgt) = 0x%8x\n", i,
				dsc[i].tgt_addr);
	}
#endif
	return ret;
}

int32_t aes_cipher(void *key, uint32_t keylen, uint8_t iv[16],
		const void *src, void *dst, uint8_t encrypt, uint8_t mode,
		uint8_t op_mode, size_t size)
{
	if (keylen != AES_KEY_SIZE_128 && keylen != AES_KEY_SIZE_256)
		return CRYPTO_ERROR_BAD_PARAMETERS;

	return cipher_core(key, keylen, iv, 16,
		   src, dst, encrypt, mode, op_mode, size);
}

int32_t des_tdes_cipher(void *key, uint32_t keylen, uint8_t iv[8],
		const void *src, void *dst, uint8_t encrypt, uint8_t mode,
		uint8_t op_mode, size_t size)
{
	if (keylen != DES_KEY_SIZE &&
			keylen != TDES_2K_KEY_SIZE &&
			keylen != TDES_3K_KEY_SIZE)
		return CRYPTO_ERROR_BAD_PARAMETERS;

	return cipher_core(key, keylen, iv, 8,
		   src, dst, encrypt, mode, op_mode, size);
}

#ifdef USE_SHA_OLD_CODE
int32_t hw_update_internal(sha2_ctx *cur_ctx, const uint8_t *input,
		uint32_t ilen, uint8_t *hash, uint8_t last_update)
{
	struct dma_dsc dsc;
	unsigned char szTempSHA2[64];

	if (!last_update && (ilen % 64)) {
		serial_puts("Err:sha5\n");
		// sha2 usage problem
		return CRYPTO_ERROR_BAD_PARAMETERS;
	}

	struct {
		unsigned long lSource;
		unsigned long lTarget;
		unsigned int  nLength;
		unsigned int nBlkFlag;
	} arrSteps[2];

	unsigned int index = 0;

	memset((void *)arrSteps, 0, sizeof(arrSteps));

	mutex_lock(&crypto_lock);

	arrSteps[0].lSource = (unsigned long)input;
	arrSteps[0].lTarget = (unsigned long)hash;
	arrSteps[1].lTarget = (unsigned long)szTempSHA2;

	int nStep_len = (128 << 10) - 64; //17bit length
	/*If input length bigger than (128KB - 64bytes),default use block mode is 1. */

	if (ilen > nStep_len) {
		arrSteps[0].nLength = ilen >> 9;
		arrSteps[0].nBlkFlag = 1;
		arrSteps[1].lSource = arrSteps[0].lSource + (arrSteps[0].nLength << 9);
		arrSteps[1].nLength = ilen % 512;
		arrSteps[1].nBlkFlag = 0;
	} else {
		arrSteps[0].nLength = ilen;
		arrSteps[0].nBlkFlag = 0;
	}

	for (index = 0; index < sizeof(arrSteps) / sizeof(arrSteps[0]); index++) {
		dsc.src_addr = arrSteps[index].lSource;
		dsc.tgt_addr = arrSteps[index].lTarget;
		dsc.dsc_cfg.d32 = 0;
		dsc.dsc_cfg.b.length = arrSteps[index].nLength;
		dsc.dsc_cfg.b.enc_sha_only = 1;
		dsc.dsc_cfg.b.mode = cur_ctx->digest_len == 224 ? 7 : 6;
		dsc.dsc_cfg.b.begin = cur_ctx->tot_len == 0; // first
		dsc.dsc_cfg.b.end = last_update;
		dsc.dsc_cfg.b.eoc = 1;
		dsc.dsc_cfg.b.owner = 1;
		dsc.dsc_cfg.b.block = arrSteps[index].nBlkFlag;

		flush_dcache_range((uintptr_t)&dsc, (uintptr_t)(&dsc + sizeof(dsc)));
		invalidate_dcache_range((uintptr_t)dsc.tgt_addr, (uintptr_t)dsc.tgt_addr + 32);

		*P_DMA_STS0 = 0xf;
		*P_DMA_T0 = (uint64_t)&dsc | 2;

		if (arrSteps[index].nBlkFlag)
			cur_ctx->tot_len += arrSteps[index].nLength << 9;
		else
			cur_ctx->tot_len += arrSteps[index].nLength;

		while (*P_DMA_STS0 == 0)
			;

		if (!dsc.dsc_cfg.b.block) {
			mutex_unlock(&crypto_lock);
			return CRYPTO_ERROR_NO_ERROR;
		}
	}

	mutex_unlock(&crypto_lock);
	return CRYPTO_ERROR_NO_ERROR;
}
#else
int32_t hw_update_internal(sha2_ctx *cur_ctx, const uint8_t *input,
		uint32_t ilen, uint8_t *hash, uint8_t last_update)
{
	struct dma_dsc dsc[2];
	uint32_t blocks = ilen / DMA_BLOCK_SIZE;
	uint32_t residues = ilen & (DMA_BLOCK_SIZE - 1);
	uint8_t hash_tmp[SHA256_BLOCK_SIZE * 2] = {0};
	int32_t ret = 0;
#ifdef CRYPTO_DEBUG
	uint32_t i = 0;
#endif

	if (!last_update && (residues & (0x40 - 1))) {
		serial_puts("Err:sha5\n");
		return CRYPTO_ERROR_BAD_PARAMETERS;
	}

	if (blocks > MAX_BLOCK_TRANSFER) {
		serial_puts("Err:sha too large\n");
		return CRYPTO_ERROR_BAD_PARAMETERS;
	}

	mutex_lock(&crypto_lock);

	if (blocks) {
		dsc[0].src_addr = (uintptr_t)input;
		dsc[0].tgt_addr = 0;
		dsc[0].dsc_cfg.d32 = 0;
		dsc[0].dsc_cfg.b.length = blocks;
		dsc[0].dsc_cfg.b.eoc = 0;
		dsc[0].dsc_cfg.b.mode = cur_ctx->digest_len == 224 ?
			DMA_MODE_SHA224 : DMA_MODE_SHA256;
		dsc[0].dsc_cfg.b.begin = cur_ctx->tot_len == 0;
		dsc[0].dsc_cfg.b.end = 0;
		dsc[0].dsc_cfg.b.enc_sha_only = 1;
		dsc[0].dsc_cfg.b.block = 1;
		dsc[0].dsc_cfg.b.owner = 1;

		dsc[1].src_addr = (uintptr_t)input + blocks * DMA_BLOCK_SIZE;
		dsc[1].tgt_addr = (uintptr_t)hash_tmp;
		dsc[1].dsc_cfg.d32 = 0;
		dsc[1].dsc_cfg.b.length = residues;
		dsc[1].dsc_cfg.b.eoc = 1;
		dsc[1].dsc_cfg.b.mode = cur_ctx->digest_len == 224 ?
			DMA_MODE_SHA224 : DMA_MODE_SHA256;
		dsc[1].dsc_cfg.b.begin = 0;
		dsc[1].dsc_cfg.b.end = last_update;
		dsc[1].dsc_cfg.b.enc_sha_only = 1;
		dsc[1].dsc_cfg.b.block = 0;
		dsc[1].dsc_cfg.b.owner = 1;
	} else {
		dsc[0].src_addr = (uintptr_t)input;
		dsc[0].tgt_addr = (uintptr_t)hash_tmp;
		dsc[0].dsc_cfg.d32 = 0;
		dsc[0].dsc_cfg.b.length = residues;
		dsc[0].dsc_cfg.b.eoc = 1;
		dsc[0].dsc_cfg.b.mode = cur_ctx->digest_len == 224 ?
			DMA_MODE_SHA224 : DMA_MODE_SHA256;
		dsc[0].dsc_cfg.b.begin = cur_ctx->tot_len == 0;
		dsc[0].dsc_cfg.b.end = last_update;
		dsc[0].dsc_cfg.b.enc_sha_only = 1;
		dsc[0].dsc_cfg.b.block = 0;
		dsc[0].dsc_cfg.b.owner = 1;
	}

	flush_dcache_range((uintptr_t)&dsc, (uintptr_t)&dsc + sizeof(dsc));
#ifdef HASH_FLUSH_INPUT
	/* from the implementation of old driver,
	 * the input data is flushed at upper level
	 */
	flush_dcache_range((uintptr_t)&input, (uintptr_t)input + ilen);
#endif
	flush_dcache_range((uintptr_t)hash_tmp, sizeof(hash_tmp));

	*P_DMA_STS0 = 0xf;
	*P_DMA_T0 = (uintptr_t)&dsc | 2;
	while (*P_DMA_STS0 == 0)
		;

	invalidate_dcache_range((uintptr_t)hash_tmp, sizeof(hash_tmp));
	if (cur_ctx->digest_len == 224)
		memcpy(hash, hash_tmp, SHA224_DIGEST_SIZE);
	else
		memcpy(hash, hash_tmp, SHA256_DIGEST_SIZE);
	cur_ctx->tot_len += ilen;

	if (*P_DMA_STS0 & 0x1)
		ret = CRYPTO_ERROR_BAD_PROCESS;
	else
		ret = CRYPTO_ERROR_NO_ERROR;

#ifdef CRYPTO_DEBUG
	for (i = 0; i < sizeof(dsc) / sizeof(struct dma_dsc); i++) {
		printf("desc (%4x) (len) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.length);
		printf("desc (%4x) (irq) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.irq);
		printf("desc (%4x) (eoc) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.eoc);
		printf("desc (%4x) (lop) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.loop);
		printf("desc (%4x) (mod) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.mode);
		printf("desc (%4x) (beg) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.begin);
		printf("desc (%4x) (end) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.end);
		printf("desc (%4x) (opm) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.op_mode);
		printf("desc (%4x) (enc) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.enc_sha_only);
		printf("desc (%4x) (blk) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.block);
		printf("desc (%4x) (err) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.error);
		printf("desc (%4x) (own) = 0x%8x\n", i,
				dsc[i].dsc_cfg.b.owner);
		printf("desc (%4x) (src) = 0x%8x\n", i,
				dsc[i].src_addr);
		printf("desc (%4x) (tgt) = 0x%8x\n", i,
				dsc[i].tgt_addr);
	}
#endif

	mutex_unlock(&crypto_lock);
	return ret;
}
#endif
