/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/txlx/firmware/bl21/bl21_main.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include "secure_apb.h"
#include "serial.h" //use local serial driver
#include "io.h"
#include "timer.h"
#include "board_init.c"

void bl21_main(void)
{
	serial_puts("bl21 start\n");
	/* bl2 customer code */
	board_init();
	serial_puts("bl21 end\n");
	return;
}