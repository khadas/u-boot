// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <amlogic/blxx2bl33_param.h>

#define param_end(x)	((x) > (BL2E2BL33_PARAM_END))

struct param_e *next_entry(struct param_e *param_pre)
{
	struct param_e *param_next;

	param_next = (struct param_e *)((unsigned long)param_pre + param_pre->len);
	if (param_end((unsigned long)param_next))
		return NULL;

	return param_next;
}

struct param_e *param_of(int type)
{
	struct param_e *param;

	param = (struct param_e *)BL2E2BL33_PARAM_START;

#if BLXX2BL33_PARAM_DEBUG
	{
		int i = 1;
		unsigned char *data = (unsigned char *)BL2E2BL33_PARAM_START;
		for (; i <= 512; i++) {
			printf("%02x ", data[i-1]);
			if (i%16 == 0)
				printf("\n");
		}
	}
#endif

	for (; param && param->type; param = next_entry(param)) {
		if (param->type == STORAGE_PARAM_TPYE)
			return param;
	}
	printf("ERROR param_of(Type=%d) not found\n", param->type);
	return NULL;
}