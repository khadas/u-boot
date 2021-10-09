/*
 * Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
 *
 * All information contained herein is Amlogic confidential.
 *
 * This software is provided to you pursuant to Software License Agreement
 * (SLA) with Amlogic Inc ("Amlogic"). This software may be used
 * only in accordance with the terms of this agreement.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification is strictly prohibited without prior written permission from
 * Amlogic.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __COMMON_H
#define __COMMON_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <errno.h>


/* Macros to access registers */
#define REG32_ADDR(addr) ((volatile uint32_t *)(uintptr_t)(addr))
#define REG16_ADDR(addr) ((volatile uint16_t *)(uintptr_t)(addr))
#define REG8_ADDR(addr)  ((volatile uint8_t  *)(uintptr_t)(addr))

#define REG32(addr) (*REG32_ADDR(addr))
#define REG16(addr) (*REG16_ADDR(addr))
#define REG8(addr)  (*REG8_ADDR(addr))

#define BIT(nr)			(1UL << (nr))
#define REG32_UPDATE_BITS(addr, mask, val)		\
do {							\
	uint32_t _v = REG32((unsigned long)addr);	\
	_v &= (~(mask));				\
	_v |= ((val) & (mask));				\
	REG32((unsigned long)addr) = _v;		\
} while(0)

#ifndef FIELD_PREP
#define FIELD_PREP(_mask, _val) \
	(((typeof(_mask))(_val) << (ffs(_mask) - 1)) & (_mask))
#endif

#ifndef FIELD_GET
#define FIELD_GET(_mask, _reg) \
	((typeof(_mask))(((_reg) & (_mask)) >> (ffs(_mask) - 1)))
#endif

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define BITS_PER_LONG (sizeof(unsigned long) == 8 ? 64 : 32)
#define GENMASK(h, l) \
	(((~0UL) << (l)) & (~0UL >> (BITS_PER_LONG - 1 - (h))))
#define IS_ALIGNED(x, a)		(((unsigned long)(x) & ((unsigned long)(a) - 1)) == 0)
#define _RET_IP_		(unsigned long)__builtin_return_address(0)
#define _THIS_IP_  ({ __label__ __here; __here: (unsigned long)&&__here; })
#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

#ifndef MTAG
#define MTAG "dft"
#endif

#define _sys_log_(arg,...)	do{\
	printf(arg,##__VA_ARGS__);\
	}while(0)

#define sys_log		_sys_log_

#define logi(arg, ...)	sys_log("[30/%s] "arg,\
		MTAG, ##__VA_ARGS__)

#define loge(arg, ...)	sys_log("[30/%s] [error]%s:%d "arg,\
		MTAG, __func__, __LINE__, ##__VA_ARGS__)

typedef int (*initcall_t) (void);
#define ____define_initcall(fn, id) \
	initcall_t __initcall_##id##_##fn = (void *)fn;

#define __define_initcall(fn, id) ____define_initcall(fn, id)

#define device_initcall(fn) __define_initcall(fn, 6)

#ifndef BIT
#define BIT(x) (1 << (x))
#endif

#ifdef __cplusplus
}
#endif
#endif
