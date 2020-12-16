/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/board_id.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <asm/arch/io.h>
#include <asm/arch/board_id.h>
#include <asm/arch/secure_apb.h>

__weak unsigned int get_board_id(void)
{
	unsigned int board_id = 0;
	board_id = (readl(P_AO_SEC_GP_CFG0) >> 8 ) & 0xFF;
	return board_id;
}
