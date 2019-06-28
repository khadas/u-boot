
/*
 * arch/arm/cpu/armv8/txl/sdio.c
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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


