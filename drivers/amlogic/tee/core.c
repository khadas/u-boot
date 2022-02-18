// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <tee.h>
#include <linux/arm-smccc.h>
#include <amlogic/tee_aml.h>

#include "../../tee/optee/optee_smc.h"

#define TEE_SMC_FUNCID_PROTECT_TVP_MEM             0xE020
#define TEE_SMC_PROTECT_TVP_MEM \
		OPTEE_SMC_FAST_CALL_VAL(TEE_SMC_FUNCID_PROTECT_TVP_MEM)

#define TEE_SMC_FUNCID_UNPROTECT_TVP_MEM           0xE021
#define TEE_SMC_UNPROTECT_TVP_MEM \
		OPTEE_SMC_FAST_CALL_VAL(TEE_SMC_FUNCID_UNPROTECT_TVP_MEM)

#define TEE_SMC_FUNCID_PROTECT_MEM_BY_TYPE         0xE023
#define TEE_SMC_PROTECT_MEM_BY_TYPE \
		OPTEE_SMC_FAST_CALL_VAL(TEE_SMC_FUNCID_PROTECT_MEM_BY_TYPE)

#define TEE_SMC_FUNCID_UNPROTECT_MEM               0xE024
#define TEE_SMC_UNPROTECT_MEM \
		OPTEE_SMC_FAST_CALL_VAL(TEE_SMC_FUNCID_UNPROTECT_MEM)

u32 tee_protect_tvp_mem(u32 start, u32 size, u32 *handle)
{
	struct arm_smccc_res res;

	if (!handle)
		return 0xFFFF0006;

	arm_smccc_smc(TEE_SMC_PROTECT_TVP_MEM,
		start, size, 0, 0, 0, 0, 0, &res);

	*handle = res.a1;

	return res.a0;
}

void tee_unprotect_tvp_mem(u32 handle)
{
	struct arm_smccc_res res;

	arm_smccc_smc(TEE_SMC_UNPROTECT_TVP_MEM,
		handle, 0, 0, 0, 0, 0, 0, &res);
}

u32 tee_protect_mem_by_type(u32 type, u32 start, u32 size, u32 *handle)
{
	struct arm_smccc_res res;

	if (!handle)
		return 0xFFFF0006;

	arm_smccc_smc(TEE_SMC_PROTECT_MEM_BY_TYPE,
		type, start, size, 0, 0, 0, 0, &res);

	*handle = res.a1;

	return res.a0;
}

u32 tee_protect_mem(u32 type, u32 level,
				u32 start, u32 size, u32 *handle)
{
	struct arm_smccc_res res;

	if (!handle)
		return 0xFFFF0006;

	arm_smccc_smc(TEE_SMC_PROTECT_MEM_BY_TYPE,
		type, start, size, level, 0, 0, 0, &res);

	*handle = res.a1;

	return res.a0;
}

void tee_unprotect_mem(u32 handle)
{
	struct arm_smccc_res res;

	arm_smccc_smc(TEE_SMC_UNPROTECT_MEM,
		handle, 0, 0, 0, 0, 0, 0, &res);
}
