/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/ini/model_data_check.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "model.h"

unsigned char model_data_checksum(unsigned char *buf, unsigned int len)
{
	unsigned int temp = 0;
	unsigned int i;

	if (!buf)
		return 0;
	if (len == 0)
		return 0;
	for (i = 0; i < len; i++)
		temp += buf[i];

	return (unsigned char)(temp & 0xff);
}

unsigned char model_data_lrc(unsigned char *buf, unsigned int len)
{
	unsigned char temp = 0;
	unsigned int i;

	if (!buf)
		return 0xff;
	if (len == 0)
		return 0xff;
	temp = buf[0];
	for (i = 1; i < len; i++)
		temp = temp ^ buf[i];

	return temp;
}

