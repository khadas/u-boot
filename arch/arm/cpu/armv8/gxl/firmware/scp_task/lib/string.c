/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/gxl/firmware/scp_task/lib/string.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

void *memcpy(void *dest, const void *src, unsigned int count)
{
	char *tmp = dest;
	const char *s = src;

	while (count--)
		*tmp++ = *s++;
	return dest;
}
void *memset(void *s, int c, unsigned int count)
{
	char *xs = s;

	while (count--)
		*xs++ = c;
	return s;
}

