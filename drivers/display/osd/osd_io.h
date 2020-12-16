/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/display/osd/osd_io.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef _OSD_IO_H_
#define _OSD_IO_H_

#include <asm/arch/io.h>
#include <asm/arch/secure_apb.h>

#define REG_INDEX_VCBUS(reg)  (u32)((reg) & REG_BASE_VCBUS ? ((reg - REG_BASE_VCBUS) >> 2) : reg)

#define REG_OFFSET_CBUS(reg)  ((reg) << 2)
#define REG_OFFSET_VCBUS(reg) ((reg) << 2)

#define REG_ADDR_CBUS(reg)    ((reg) & REG_BASE_CBUS ? reg + 0L : REG_BASE_CBUS + REG_OFFSET_CBUS(reg))
#define REG_ADDR_VCBUS(reg)   ((reg) & REG_BASE_VCBUS ? reg + 0L : REG_BASE_VCBUS + REG_OFFSET_VCBUS(reg))
#define REG_ADDR_HIU(reg)     (reg + 0L)

static inline unsigned int osd_hiu_read(unsigned int _reg)
{
	unsigned int val = 0;

	val = *(volatile unsigned int *)(REG_ADDR_HIU(_reg));

	return val;
};

static inline void osd_hiu_write(unsigned int _reg, unsigned int _value)
{
	*(volatile unsigned int *)REG_ADDR_HIU(_reg) = (_value);
};

static inline void osd_hiu_setb(unsigned int _reg, unsigned int _value,
		unsigned int _start, unsigned int _len)
{
	osd_hiu_write(_reg, ((osd_hiu_read(_reg) &
			~(((1L << (_len))-1) << (_start))) |
			(((_value)&((1L<<(_len))-1)) << (_start))));
}


static inline u32 osd_cbus_read(u32 reg)
{
	return (*(volatile unsigned int *)REG_ADDR_CBUS(reg));
}

static inline void osd_cbus_write(u32 reg,
				  const u32 val)
{
	*(volatile unsigned int *)REG_ADDR_CBUS(reg) = (val);
}

static inline u32 osd_reg_read(u32 reg)
{
	return (*(volatile unsigned int *)REG_ADDR_VCBUS(reg));
}

static inline void osd_reg_write(u32 reg,
				 const u32 val)
{
	*(volatile unsigned int *)REG_ADDR_VCBUS(reg) = (val);
}

static inline void osd_reg_set_mask(u32 reg,
				    const u32 mask)
{
	osd_reg_write(reg, (osd_reg_read(reg) | (mask)));
}

static inline void osd_reg_clr_mask(u32 reg,
				    const u32 mask)
{
	osd_reg_write(reg, (osd_reg_read(reg) & (~(mask))));
}

static inline void osd_reg_set_bits(u32 reg,
				    const u32 value,
				    const u32 start,
				    const u32 len)
{
	osd_reg_write(reg, ((osd_reg_read(reg) &
			     ~(((1L << (len)) - 1) << (start))) |
			    (((value) & ((1L << (len)) - 1)) << (start))));
}

#define VSYNCOSD_RD_MPEG_REG(reg) osd_reg_read(reg)
#define VSYNCOSD_WR_MPEG_REG(reg, val) osd_reg_write(reg, val)
#define VSYNCOSD_WR_MPEG_REG_BITS(reg, val, start, len) \
	osd_reg_set_bits(reg, val, start, len)
#define VSYNCOSD_SET_MPEG_REG_MASK(reg, mask) osd_reg_set_mask(reg, mask)
#define VSYNCOSD_CLR_MPEG_REG_MASK(reg, mask) osd_reg_clr_mask(reg, mask)

#endif
