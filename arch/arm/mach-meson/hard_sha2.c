
/*
 * plat/txl/sha2.c
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
*/


#include <common.h>
#include <stdio.h>
#include <string.h>
#include <dma.h>
#include <asm/arch/regs.h>
#include <asm/arch-c1/secure_apb.h>
#include <u-boot/sha256.h>

#ifndef __AP_DMA_H__
#define __AP_DMA_H__

typedef struct dma_dsc {
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
} dma_dsc_t;

#endif /* __AP_DMA_H__ */

static sha2_ctx *cur_ctx;

static void hw_init(uint32_t is224)
{
	cur_ctx->digest_len = is224 ? 224 : 256;
	cur_ctx->tot_len = 0;
}

static void hw_update(const uint8_t *input, uint32_t ilen,
		      uint8_t *hash, uint8_t last_update)
{
	dma_dsc_t dsc;
	unsigned char szTempSHA2[32];

	if (!last_update && (ilen % 64))
	{
		serial_puts("Err:sha5\n");
		// sha2 usage problem
		return;
	}

	struct {
		unsigned long lSource;
		unsigned long lTarget;
		unsigned int  nLength;
		unsigned int nBlkFlag;
	}arrSteps[2];

	unsigned int index = 0;
	memset((void *)arrSteps, 0, sizeof(arrSteps));

	arrSteps[0].lSource = (unsigned long)input;
	arrSteps[0].lTarget = (unsigned long)hash;
	arrSteps[1].lTarget = (unsigned long)szTempSHA2;

	int nStep_len = (128<<10) - 64; //17bit length
	/*If input length bigger than (128KB - 64bytes),default use block mode is 1. */

	if (ilen > nStep_len)
	{
		arrSteps[0].nLength = ilen >> 9;
		arrSteps[0].nBlkFlag = 1;
		arrSteps[1].lSource = arrSteps[0].lSource + (arrSteps[0].nLength << 9);
		arrSteps[1].nLength = ilen % 512;
		arrSteps[1].nBlkFlag = 0;
	}
	else
	{
		arrSteps[0].nLength = ilen;
		arrSteps[0].nBlkFlag = 0;
	}

	for (index = 0; index < sizeof(arrSteps) / sizeof(arrSteps[0]); index++)
	{
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

		flush_dcache_range((unsigned long)&dsc,(unsigned long)&dsc+sizeof(dsc));
		invalidate_dcache_range((unsigned long)dsc.tgt_addr,(unsigned long)dsc.tgt_addr + 32);

		*P_DMA_STS0 = 0xf;
		*P_DMA_T0 = (uint64_t)&dsc | 2;

		if (arrSteps[index].nBlkFlag)
		{
			cur_ctx->tot_len += arrSteps[index].nLength << 9;
		}
		else
			cur_ctx->tot_len += arrSteps[index].nLength;

		while (*P_DMA_STS0 == 0);

		if (!dsc.dsc_cfg.b.block)
			return;
	}
}

static void SHA2_HW_init(sha2_ctx *ctx, uint32_t digest_len)
{
	if (cur_ctx != NULL) {
		serial_puts("Err:sha4\n");
		// sha2 usage problem
		return;
	}
	cur_ctx = ctx;

	hw_init(digest_len == 224);

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
		hw_update(ctx->block, SHA256_BLOCK_SIZE,ctx->buf, 0);
		ctx->len = 0;
	}
	if (data_len > SHA256_BLOCK_SIZE) {
		/* still have more than 1 block. hash up until last [partial|full] block */
		rem_len = data_len % SHA256_BLOCK_SIZE;

		if (rem_len == 0) {
			rem_len = SHA256_BLOCK_SIZE;
		}

		data_len -= rem_len;
		hw_update(&data[offset], data_len,ctx->buf, 0);
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

	hw_update(ctx->block, ctx->len,ctx->buf, 1);
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
