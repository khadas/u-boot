/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/include/asm/arch-txl/clock.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef __ARCH_ARM_MESON_CLOCK_H_U_BOOT_
#define __ARCH_ARM_MESON_CLOCK_H_U_BOOT_

/* add define if needed */
#define CLK81                          (7)

#if 0
__u32 get_cpu_clk(void);
__u32 get_clk_ddr(void);
__u32 get_misc_pll_clk(void);
#endif

__u32 get_clk81(void);
int clk_get_rate(unsigned clksrc);
unsigned long clk_util_clk_msr(unsigned long clk_mux);

int clk_msr(int index);

#endif /* __ARCH_ARM_MESON_CLOCK_H_U_BOOT_ */

