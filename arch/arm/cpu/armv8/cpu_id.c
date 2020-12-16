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
chip_id_t aml_chip_id = { 0, { 0 } };

cpu_id_t get_cpu_id(void)
{
	cpu_id_t cpu_id;
	unsigned int cpu_id_reg = readl(P_AO_SEC_SD_CFG8);
	cpu_id.reserve = 0;
	cpu_id.family_id = (cpu_id_reg >> 24) & (0XFF);
	cpu_id.package_id = (cpu_id_reg >> 16) & (0XF0);
	cpu_id.chip_rev = (cpu_id_reg >> 8) & (0XFF);
	cpu_id.layout_ver = (cpu_id_reg) & (0XF);
	return cpu_id;
}

int get_chip_id(unsigned char *buff, unsigned int size)
{
	int rc = 0;

	if (buff == NULL || size < 16)
		return -1;

	if (aml_chip_id.version == 0) {
		/* Chip ID has not been fetched yet */
		rc = __get_chip_id(&aml_chip_id.chipid[0], 16);

		/* Mark version if chip ID is fetched successfully */
		if (rc == 0)
			aml_chip_id.version = 2;
	}

	memcpy(buff, &aml_chip_id.chipid[0], 16);
	return rc;
}
