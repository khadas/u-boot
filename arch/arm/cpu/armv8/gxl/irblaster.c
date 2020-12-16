/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/gxl/irblaster.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <asm/arch/io.h>
#include <asm/cpu_id.h>
#include <common.h>
#include <config.h>
#include <asm/arch/secure_apb.h>

void irblaster_pinmux_config(void)
{
	int val;

	/*GPIOAO_7|REMOTE_INPUT-ao_reg0|REMOTE_OUTPUT-ao_reg21*/
	val = (readl(AO_RTI_PIN_MUX_REG) & ~(1 << 0)) | (1 <<21);
	writel(val, AO_RTI_PIN_MUX_REG);
	printf("gxl config irblaster pinmux well\n");

}
