// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <stdio.h>
#include <string.h>
#include <dma.h>
#include <asm/arch/regs.h>
#include <asm/arch/secure_apb.h>
#include <u-boot/sha256.h>
#include <amlogic/aml_crypto.h>

static sha2_ctx *cur_ctx;

static void SHA2_HW_init(sha2_ctx *ctx, uint32_t digest_len)
{
	if (ctx == NULL) {
		return ;
	}
	if (cur_ctx != NULL) {
		serial_puts("Err:sha4\n");
		// sha2 usage problem
		return;
	}
	cur_ctx = ctx;

	aml_hw_sha_init(ctx, digest_len == 224);

	ctx->len = 0;
}

static void SHA2_HW_update(sha2_ctx *ctx, const uint8_t *data, uint32_t len)
{

	unsigned int fill_len, data_len, rem_len,offset;

	if (cur_ctx != ctx) {
		serial_puts("Err:sha3\n");
		// sha2 usage problem
		return;
	}
	/* This method updates the hash for the input data in blocks, except the last
	 * partial|full block, which is saved in ctx->block.  The last partial|full
	 * block will be added to the hash in SHA2_final.
	 */
	data_len = len;
	offset = 0;
	/* fill saved block from beginning of input data */
	if (ctx->len) {
		fill_len = SHA256_BLOCK_SIZE - ctx->len;
		memcpy(&ctx->block[ctx->len], data, fill_len);
		flush_dcache_range((unsigned long)ctx->block,(unsigned long)ctx->block+128);
		data_len -= fill_len;
		offset = fill_len;
		ctx->len += fill_len;
	}
	if (ctx->len == SHA256_BLOCK_SIZE && data_len > 0) {
		/* saved block is full and is not last block, hash it */
		aml_hw_sha_update(ctx, ctx->block, SHA256_BLOCK_SIZE, ctx->buf, 0);
		ctx->len = 0;
	}
	if (data_len > SHA256_BLOCK_SIZE) {
		/* still have more than 1 block. hash up until last [partial|full] block */
		rem_len = data_len % SHA256_BLOCK_SIZE;

		if (rem_len == 0) {
			rem_len = SHA256_BLOCK_SIZE;
		}

		data_len -= rem_len;
		aml_hw_sha_update(ctx, &data[offset], data_len, ctx->buf, 0);
		offset += data_len;
	} else {
		rem_len = data_len;
	}
	if (rem_len) {
		/* save the remaining data */
		memcpy(ctx->block, &data[offset], rem_len);
		flush_dcache_range((unsigned long)ctx->block,(unsigned long)ctx->block+128);
		ctx->len = rem_len;
	}
}

static uint8_t *SHA2_HW_final(sha2_ctx *ctx)
{
	if (cur_ctx != ctx) {
		serial_puts("Err:sha1\n");
		// sha2 usage problem
		return ctx->buf;
	}
	if (ctx->len == 0 || ctx->len > SHA256_BLOCK_SIZE) {
		serial_puts("Err:sha2\n");
		// internal sha2 problem
		return ctx->buf;
	}

	aml_hw_sha_update(ctx, ctx->block, ctx->len, ctx->buf, 1);
	cur_ctx = NULL;
	return ctx->buf;
}

 void sha256_starts(sha256_context * ctx)
{
	if (ctx)
		memset(ctx,0,sizeof(sha256_context));
	SHA2_HW_init(ctx, 256);
}

 void sha256_update(sha256_context *ctx, const uint8_t *input, uint32_t length)
{
	int nOffset = 0;
	int nStep = (128<<19); //64MB

	if (length > nStep)
	{
		for (;nOffset < length;)
		{
			SHA2_HW_update(ctx, input+nOffset, nStep);
			nOffset+=nStep;
			if ((length - nOffset ) < nStep)
				nStep = length - nOffset;
		}
	}
	else
	{
		SHA2_HW_update(ctx, input, length);
	}
}

 void sha256_finish(sha256_context * ctx, uint8_t digest[SHA256_SUM_LEN])
{
	SHA2_HW_final(ctx);
	/*before memcpy,must invalidate dcache.so memcpy data from ddr*/
	invalidate_dcache_range((unsigned long)digest,(unsigned long)(digest+32));
	invalidate_dcache_range((unsigned long)ctx->buf,(unsigned long)(ctx->buf+32));

	memcpy(digest,ctx->buf,32);
}

 void sha256_csum_wd(const unsigned char *input, unsigned int ilen,
											unsigned char *output, unsigned int chunk_sz)
{
	flush_dcache_range((unsigned long)input,(unsigned long)input+ilen);

	sha2_ctx sha_ctx;

	sha256_starts(&sha_ctx);

	sha256_update(&sha_ctx, input, ilen);

	sha256_finish(&sha_ctx, output);
}
