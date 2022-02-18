/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __TEE_AML_H
#define __TEE_AML_H

#define TEE_MEM_TYPE_TCON 0xb

u32 tee_protect_tvp_mem(u32 start, u32 size, u32 *handle);

void tee_unprotect_tvp_mem(u32 handle);

u32 tee_protect_mem_by_type(u32 type, u32 start, u32 size, u32 *handle);

void tee_unprotect_mem(u32 handle);

u32 tee_protect_mem(u32 type, u32 level, u32 start, u32 size, u32 *handle);

#endif /* __TEE_AML_H */
