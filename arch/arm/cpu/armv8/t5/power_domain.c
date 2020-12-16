/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/t5/power_domain.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <asm/arch/io.h>
#include <asm/arch/regs.h>
#include <asm/arch/secure_apb.h>
#include <amlogic/power_domain.h>

static void power_switch(int pwr_domain, bool pwr_switch)
{
}

static void mem_pd_switch(int pwr_domain, bool pwr_switch)
{
}

static void reset_switch(int pwr_domain, bool pwr_switch)
{

}

static void iso_switch(int pwr_domain, bool pwr_switch)
{
}

void power_domain_switch(int pwr_domain, bool pwr_switch)
{
	if (pwr_switch == PWR_ON) {
		/* Powerup Power Domain */
		power_switch(pwr_domain, PWR_ON);
		udelay(50);

		/* Powerup memories */
		mem_pd_switch(pwr_domain, PWR_ON);
		udelay(150);

		reset_switch(pwr_domain, PWR_OFF);

		/* remove isolations */
		iso_switch(pwr_domain, PWR_ON);

		/* deassert reset */
		reset_switch(pwr_domain, PWR_ON);

	} else {
		/* reset */
		reset_switch(pwr_domain, PWR_OFF);

		/* add isolation to domain */
		iso_switch(pwr_domain, PWR_OFF);

		/* Power down memories */
		mem_pd_switch(pwr_domain, PWR_OFF);
		udelay(50);

		/* Power off  domain */
		power_switch(pwr_domain, PWR_OFF);
	}
}
