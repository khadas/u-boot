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

/* return:
	0: insert
	1: not insert
 */
__weak int  sd_emmc_detect(unsigned port)
{
	int ret = 0;

	return ret;
}


