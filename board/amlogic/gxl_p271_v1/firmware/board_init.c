/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * board/amlogic/gxl_p271_v1/firmware/board_init.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "power.c"

/* bl2 customer code */
void board_init(void)
{
	power_init(0);
}