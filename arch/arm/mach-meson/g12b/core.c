// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <asm/arch/secure_apb.h>
#include <asm/types.h>
#include <asm/arch/cpu.h>
#include <common.h>

const unsigned int core_map[] = {
	0x0,
	0x1,
	0x2,
	0x3,
};

int get_core_mpidr(unsigned int cpuid)
{
	unsigned int coremax = (unsigned int)(sizeof(core_map)/sizeof(unsigned int));

	if (cpuid >= coremax)
		return -1;
	return core_map[cpuid];
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
	return (sizeof(core_map)/sizeof(unsigned int));
}
