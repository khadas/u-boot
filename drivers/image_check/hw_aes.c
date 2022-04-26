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
#include <amlogic/image_check.h>

#define AML_BLK_SIZE (0x200)
#define SBOOT_BL2_ENCRYPT_KEYSLOT	134
#define SBOOT_BL2_ENCRYPT_KEY_LEN	32	// AES256

struct dma_dsc_t {
	union {
		u32 d32;
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
	u32 src_addr;
	u32 tgt_addr;
};

static void aes256_cbc_decrypt(u8 *key, u8 *iv,
			 u8 *ct, u8 *pt, int size)
{
	struct dma_dsc_t dsc[3];
	u8 iv_zero[16];

	if (!iv) {
		memset(iv_zero, 0, sizeof(iv_zero));
		iv = iv_zero;
	}

	dsc[0].src_addr = (uint64_t)key;
	dsc[0].tgt_addr = 0;
	dsc[0].dsc_cfg.d32 = 0;
	dsc[0].dsc_cfg.b.length = 32;
	dsc[0].dsc_cfg.b.mode = 1; // key
	dsc[0].dsc_cfg.b.owner = 1;

	dsc[1].src_addr = (uint64_t)iv;
	dsc[1].tgt_addr = 32;
	dsc[1].dsc_cfg.d32 = 0;
	dsc[1].dsc_cfg.b.length = 16;
	dsc[1].dsc_cfg.b.mode = 1; // key
	dsc[1].dsc_cfg.b.owner = 1;

	dsc[2].src_addr = (uint64_t)ct;
	dsc[2].tgt_addr = (uint64_t)pt;
	dsc[2].dsc_cfg.d32 = 0;

	if (size > AML_BLK_SIZE && !(size & (AML_BLK_SIZE - 1))) {
		dsc[2].dsc_cfg.b.length = (size >> 9);
		dsc[2].dsc_cfg.b.block = 1;
	} else {
		dsc[2].dsc_cfg.b.length = size;
	}

	dsc[2].dsc_cfg.b.mode = 10;   // aes256
	dsc[2].dsc_cfg.b.op_mode = 1; // cbc
	dsc[2].dsc_cfg.b.eoc = 1;
	dsc[2].dsc_cfg.b.owner = 1;

	flush_dcache_range((unsigned long)&dsc[0].src_addr, (unsigned long)&dsc[0].src_addr + 32);
	flush_dcache_range((unsigned long)&dsc[1].src_addr, (unsigned long)&dsc[1].src_addr + 16);
	flush_dcache_range((unsigned long)&dsc[2].src_addr, (unsigned long)&dsc[2].src_addr + size);
	flush_dcache_range((unsigned long)&dsc, (unsigned long)&dsc + sizeof(dsc));

	*P_DMA_STS0 = 0xf;
	*P_DMA_T0 = (uint64_t)dsc | 2;
	while (*P_DMA_STS0 == 0)
		;

	invalidate_dcache_range(dsc[2].tgt_addr, dsc[2].tgt_addr + size);
}

void aes256dec_keytbl(const u8 *input, u8 *output, size_t nbytes)
{
	aes256_cbc_decrypt((u8 *)(0xffffff00 | SBOOT_BL2_ENCRYPT_KEYSLOT),
			      NULL,
			      (u8 *)input,
			      (u8 *)output,
			      nbytes);
}
