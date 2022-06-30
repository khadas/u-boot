/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _VOUT_REG_H
#define _VOUT_REG_H

#define VOUT_REG_INVALID                0xffffffff

#ifndef REG_BASE_VCBUS
#define REG_BASE_VCBUS                  (0xff900000L)
#endif
#define REG_OFFSET_VCBUS(reg)           ((reg << 2))
#define REG_ADDR_VCBUS(reg)             (REG_BASE_VCBUS + REG_OFFSET_VCBUS(reg) + 0L)

#define REG_CLK_ADDR(reg)               (reg + 0L)
#define REG_VOUT_ADDR(reg)              (reg + 0L)

#ifndef HHI_VPU_CLKC_CNTL
#define HHI_VPU_CLKC_CNTL                          (0xff63c000L + (0x06d << 2))
#endif
#ifndef CLKCTRL_VPU_CLKC_CTRL
#define CLKCTRL_VPU_CLKC_CTRL                      ((0x003c << 2) + 0xfe000000L)
#endif
#ifndef VPU_VIU_VENC_MUX_CTRL
#define VPU_VIU_VENC_MUX_CTRL                      ((0x271a << 2) + 0xff000000L)
#endif
#ifndef VPU_VENCX_CLK_CTRL
#define VPU_VENCX_CLK_CTRL                         ((0x2785 << 2) + 0xff000000L)
#endif
#ifndef VPU_VENC_CTRL
#define VPU_VENC_CTRL                              ((0x1cef << 2) + 0xff000000L)
#endif

static inline unsigned int vout_reg_read(u32 reg)
{
	unsigned int val;

	if (reg > 0x10000)
		val = *(volatile unsigned int *)REG_VOUT_ADDR(reg);
	else
		val = *(volatile unsigned int *)REG_ADDR_VCBUS(reg);

	return val;
}

static inline void vout_reg_write(u32 reg, const u32 val)
{
	if (reg > 0x10000)
		*(volatile unsigned int *)REG_VOUT_ADDR(reg) = (val);
	else
		*(volatile unsigned int *)REG_ADDR_VCBUS(reg) = (val);
}

static inline void vout_reg_setb(unsigned int reg, unsigned int val,
		unsigned int start, unsigned int len)
{
	vout_reg_write(reg, ((vout_reg_read(reg) &
			~(((1L << (len))-1) << (start))) |
			(((val)&((1L<<(len))-1)) << (start))));
}

static inline unsigned int vout_clk_read(u32 reg)
{
	return *(volatile unsigned int *)(REG_CLK_ADDR(reg));
}

static inline void vout_clk_write(u32 reg, const u32 val)
{
	*(volatile unsigned int *)REG_CLK_ADDR(reg) = (val);
}

static inline void vout_clk_setb(unsigned int reg, unsigned int val,
		unsigned int start, unsigned int len)
{
	vout_clk_write(reg, ((vout_clk_read(reg) &
			~(((1L << (len))-1) << (start))) |
			(((val)&((1L<<(len))-1)) << (start))));
}

#endif
