/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/cpu_id.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <asm/cpu_id.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/io.h>

/**
 * TODO: This should be moved to a RO region of registers or SRAM that
 * provides static system information that's needed across all software
 *
 * Chip ID copy to avoid unnecessary message exchange.
 */

cpu_id_t get_cpu_id(void)
{
	cpu_id_t cpu_id = {0};
	unsigned int cpu_id_reg = readl(P_AO_SEC_SD_CFG8);
	cpu_id.reserve = 0;
	cpu_id.family_id = (cpu_id_reg >> 24) & (0XFF);
	cpu_id.package_id = (cpu_id_reg) & (0X07);
	cpu_id.chip_rev = (cpu_id_reg >> 8) & (0XFF);
	return cpu_id;
}

