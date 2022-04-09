/*
 * Copyright (C) 2014-2017 Amlogic, Inc. All rights reserved.
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

#ifndef __BL33Z_IO_H_
#define __BL33Z_IO_H_

#include <stdint.h>
#include <stddef.h>

#define writel32(val, reg)		(*((volatile uint32_t *)((uintptr_t)(reg)))) =  (val)
#define readl32(reg)			(*((volatile uint32_t *)((uintptr_t)(reg))))

#define writel	writel32
#define readl	readl32

#define setbits_le32(reg, val)		(*((volatile uint32_t *)((uintptr_t)(reg)))) |= (val)
#define clrbits_le32(reg, val)		(*((volatile uint32_t *)((uintptr_t)(reg)))) &= (~(val))
#define clrsetbits_le32(reg, clr, set)	{					\
						uint32_t __v = readl(reg);	\
						__v &= (~(clr));		\
						__v |= (set);			\
						writel(__v, reg);		\
					}

/*
static inline void writel32(uint32_t value, uintptr_t addr)
{
	*(volatile uint32_t *)addr = value;
}

static inline uint32_t readl32(uintptr_t addr)
{
	return *(volatile uint32_t *)addr;
}
*/

#endif /*__BL33Z_IO_H_*/
