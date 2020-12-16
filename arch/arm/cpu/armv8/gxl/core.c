/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/gxl/core.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <asm/arch/secure_apb.h>
#include <asm/types.h>
#include <asm/arch/cpu.h>
#include <asm/cpu_id.h>
#include <common.h>

const unsigned int core_map_gxl[] = {
	0x0,
	0x1,
	0x2,
	0x3,
};
const unsigned int core_map_gxm[] = {
	0x0,
	0x1,
	0x2,
	0x3,
	0x100,
	0x101,
	0x102,
	0x103,
};

int get_core_mpidr(unsigned int cpuid)
{
	const unsigned int *pcore_map = core_map_gxl;
	unsigned int coremax = (unsigned int)(sizeof(core_map_gxl)/sizeof(unsigned int));

	if (get_cpu_id().family_id == MESON_CPU_MAJOR_ID_GXM) {
		pcore_map = core_map_gxm;
		coremax = (unsigned int)(sizeof(core_map_gxm)/sizeof(unsigned int));
	}
	if (cpuid >= coremax)
		return -1;
	return pcore_map[cpuid];
}

int get_core_idx(unsigned int mpidr)
{
	unsigned int clusterid, cpuid;

	cpuid = mpidr & 0xff;
	clusterid = mpidr & 0xff00;

	cpuid += (clusterid >> 6);

	if (cpuid >= NR_CPUS)
		return -1;

	return cpuid;
}

int get_core_max(void)
{
	if (get_cpu_id().family_id == MESON_CPU_MAJOR_ID_GXM)
		return (sizeof(core_map_gxm)/sizeof(unsigned int));
	else
		return (sizeof(core_map_gxl)/sizeof(unsigned int));
}

