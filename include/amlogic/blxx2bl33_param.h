/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __BLXX2BL33_PARAM_H__
#define __BLXX2BL33_PARAM_H__
#include <common.h>

/* !!! don't try to modify the Micro unless make a consistency with blxx */

#define BL2E2BL33_PARAM_START	(0x0e00000)
#define BL2E2BL33_PARAM_END	(0x1000000)

#define BLXX2BL33_PARAM_DEBUG	(0)

/* define the parameter type here and make sure the same with blxx */
enum {
	STORAGE_PARAM_TPYE = 1,
	BL2PARMA_PARAM_TYPE = 2,
	/* other */
};

struct param_e {
	unsigned int type;
	unsigned int len;	/* must be 8 bytes align */
	unsigned char data[1];	/* user defined parameter */
};

struct param_e *param_of(int type);

#endif