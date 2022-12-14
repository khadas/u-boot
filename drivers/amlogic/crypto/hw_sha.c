// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <crypto_internal.h>
#include <amlogic/aml_crypto.h>

int32_t aml_hw_sha_init(sha2_ctx *cur_ctx, uint32_t is224)
{
	cur_ctx->digest_len = is224 ? 224 : 256;
	cur_ctx->tot_len = 0;

	return 0;
}

int32_t aml_hw_sha_update(sha2_ctx *cur_ctx, const uint8_t *input,
		uint32_t ilen, uint8_t *hash, uint8_t last_update)
{
	return hw_update_internal(cur_ctx, input, ilen, hash, last_update);
}
