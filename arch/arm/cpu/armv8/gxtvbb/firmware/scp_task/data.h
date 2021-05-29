/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/gxtvbb/firmware/scp_task/data.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "config.h"

#define TASK_COMMAND_OFFSET 0
#define TASK_RESPONSE_OFFSET  0x200

unsigned char
	*secure_task_share_mem = (unsigned char *)SECURE_TASK_SHARE_MEM_BASE;
unsigned char *high_task_share_mem = (unsigned char *)HIGH_TASK_SHARE_MEM_BASE;
unsigned char *low_task_share_mem = (unsigned char *)LOW_TASK_SHARE_MEM_BASE;

struct resume_param {
/* wakeup method: remote, ..., */
	unsigned int method;
	unsigned int date1;
	unsigned int date2;
	/* [0] for msg len */
	unsigned char cec_otp_msg[17];
	unsigned char cec_as_msg[17];
};
struct resume_param resume_data;