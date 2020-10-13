/*
 * drivers/amlogic/power/power_domain.c
 *
 * Copyright (C) 2017 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
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
