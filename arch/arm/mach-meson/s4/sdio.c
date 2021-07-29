// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <config.h>
#include <asm/arch/io.h>
#include <asm/arch/cpu_sdio.h>
#include <asm/arch/secure_apb.h>
#include <amlogic/cpu_id.h>
#include <common.h>

void  cpu_sd_emmc_pwr_prepare(unsigned port)
{
}
int cpu_sd_emmc_init(unsigned port)
{
	return 0;
}

int emmc_quirks(void)
{
	cpu_id_t cpuid = get_cpu_id();
	char *caps2 = "meson-gx-mmc.caps2_quirks=\"mmc-hs400\"\0";
	char *caps_none = "meson-gx-mmc.caps2_quirks=\"none\"\0";

	if  (cpuid.chip_rev >= 0xB)
		env_set("emmc_quirks", caps2);
	else
		env_set("emmc_quirks", caps_none);
	return 0;
}

/* return:
	0: insert
	1: not insert
 */
__weak int  sd_emmc_detect(unsigned port)
{
	int ret = 0;

	return ret;
}


