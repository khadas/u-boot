/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/gxb/firmware/scp_task/dvfs.c
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
	unsigned int cnt = ARRAY_SIZE(cpu_dvfs_tbl);
	buf_opp.latency = 200;
	buf_opp.count = cnt;
	memset(&buf_opp.opp[0], 0,
	       MAX_DVFS_OPPS * sizeof(struct scpi_opp_entry));
	memcpy(&buf_opp.opp[0], cpu_dvfs_tbl ,
	       cnt * sizeof(struct scpi_opp_entry));

	memcpy(info_out, &buf_opp, sizeof(struct scpi_opp));
	*size_out = sizeof(struct scpi_opp);
	return;
}
