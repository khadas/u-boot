// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

﻿#include "../include/phynand.h"
/*#include "storage.h"*/


extern int amlnf_init(u32 flag);

int amlnf_pre(void)
{
	int ret = 0xff;

	ret = amlnf_init(5);

	return ret;
}

int amlnf_probe(uint32_t init_flag)
{
	int ret = 0xff;
	ret = amlnf_init(init_flag);
	if (ret == 0)
		aml_nand_msg("amlnf probe success");
	return ret;
}

