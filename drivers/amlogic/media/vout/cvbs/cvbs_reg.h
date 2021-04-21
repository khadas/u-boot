/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __CVBS_REG_H__
#define __CVBS_REG_H__
#include <asm/arch/io.h>
#include <asm/arch/secure_apb.h>
#include "cvbs.h"
#include "cvbs_dummy_reg.h"

// encoder registers
#define BUS_TYPE_CBUS	0
#define BUS_TYPE_HIU	1
#define BUS_TYPE_VCBUS	2

#define DAC0_CLK_SEL            28
#define DAC1_CLK_SEL            24
#define DAC2_CLK_SEL            20
#define VCLK2_XD_RST            17
#define VCLK2_XD_EN             16
#define ENCL_CLK_SEL            12
#define VCLK2_XD                 0
#define VCLK_XD0                 0
#define VCLK_CLK_IN_SEL         16
#define VCLK_EN0                19
#define VCLK_XD_EN              16
#define VCLK_DIV1_EN             0
#define VCLK_SOFT_RST           15

#define VCLK2_EN                19
#define VCLK2_CLK_IN_SEL        16
#define VCLK2_SOFT_RST          15
#define VCLK2_DIV12_EN           4
#define VCLK2_DIV6_EN            3
#define VCLK2_DIV4_EN            2
#define VCLK2_DIV2_EN            1
#define VCLK2_DIV1_EN            0

/* ********************************
 * register define
 * ********************************* */

#define CVBS_REG_OFFSET_VCBUS(reg)    (((reg) << 2))

#define CVBS_REG_ADDR(reg)            (reg + 0L)
#define CVBS_REG_ADDR_HIU(reg)        (reg + 0L)
#define CVBS_REG_ADDR_VCBUS(reg)      (REG_BASE_VCBUS + CVBS_REG_OFFSET_VCBUS(reg))

/* ********************************
 * register access api
 * ********************************* */
static inline int cvbs_write_hiu(unsigned int addr, unsigned int value)
{
	*(volatile unsigned int *)CVBS_REG_ADDR_HIU(addr) = (value);
	return 0;
}

static inline int cvbs_read_hiu(unsigned int addr)
{
	unsigned int val = 0;

	val = *(volatile unsigned int *)(CVBS_REG_ADDR_HIU(addr));
	return val;
}

static inline int cvbs_set_hiu_bits(unsigned int addr, unsigned int value, unsigned int start, unsigned int len)
{
	cvbs_write_hiu(addr, ((cvbs_read_hiu(addr) &
			~(((1L << (len))-1) << (start))) |
			(((value)&((1L<<(len))-1)) << (start))));
	return 0;
}

static inline int cvbs_get_hiu_bits(unsigned int addr, unsigned int start, unsigned int len)
{
	return (cvbs_read_hiu(addr) >> (start)) & ((1L << (len)) - 1);
}

static inline  unsigned int cvbs_read_vcbus(unsigned int addr_offset)
{
	unsigned int val = 0;

	if (addr_offset > 0x10000)
		val = *(volatile unsigned int *)(CVBS_REG_ADDR(addr_offset));
	else
		val = *(volatile unsigned int *)(CVBS_REG_ADDR_VCBUS(addr_offset));
	return val;
}

static inline  int cvbs_write_vcbus(unsigned int addr_offset, unsigned int value)
{
	if (addr_offset > 0x10000)
		*(volatile unsigned int *)CVBS_REG_ADDR(addr_offset) = (value);
	else
		*(volatile unsigned int *)CVBS_REG_ADDR_VCBUS(addr_offset) = (value);
	return 0;
}

static inline  int cvbs_set_vcbus_bits(unsigned int addr_offset, unsigned int value, unsigned int start, unsigned int len)
{
	cvbs_write_vcbus(addr_offset, ((cvbs_read_vcbus(addr_offset) &
		~(((1L << (len))-1) << (start))) |
		(((value)&((1L<<(len))-1)) << (start))));
	return 0;
}

#endif
