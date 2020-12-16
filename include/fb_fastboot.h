/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * include/fb_fastboot.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef _FASTBOOT_H_
#define _FASTBOOT_H_

#define RESPONSE_LEN	(64 + 1)
void fastboot_fail(const char *s);
void fastboot_okay(const char *s);

#endif/*_FASTBOOT_H_*/
