/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/txhd/firmware/scp_task/dvfs.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "config.h"
#include "registers.h"
#include "task_apis.h"

#include "dvfs.h"
#include <dvfs_board.c>

void get_dvfs_info(unsigned int domain,
		unsigned char *info_out, unsigned int *size_out)
{
	get_dvfs_info_board(domain, info_out, size_out);
	return;
}
