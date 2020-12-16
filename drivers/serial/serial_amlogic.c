/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/serial/serial_amlogic.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>



__weak struct serial_device *default_serial_console(void)
{
	return NULL;
}


