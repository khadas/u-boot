/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/txhd/firmware/bl21/bl21_main.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

//#include "secure_apb.h"
#include "serial.h" //use local serial driver
#include "io.h"
#include "timer.h"
#include "board_init.c"

void bl21_main(void)
{
	/* bl2 customer code */
	board_init();
	return;
}