// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <config.h>
#include <common.h>
#include <asm/arch/io.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/clock.h>
#include <asm/arch/oscring.h>

void ring_powerinit(void)
{
	writel(0x18000a, RING_PWM_VCCK);/*set vcck 0.8v*/
	writel(0xc0006, RING_PWM_EE);/*set ee 0.8v*/
}

unsigned long clk_util_ring_msr(unsigned long clk_mux)
{
	unsigned int regval = 0;

	writel(0, MSR_CLK_REG0);
	/* Set the measurement gate to 64uS */
	/* 64uS is enough for measure the frequence? */
	setbits_le32(MSR_CLK_REG0, (10000 - 1));
	/* Disable continuous measurement */
	/* Disable interrupts */
	clrbits_le32(MSR_CLK_REG0, (1 << 17) | (1 << 18));
	clrbits_le32(MSR_CLK_REG0, 0x7f << 20);
	setbits_le32(MSR_CLK_REG0, (clk_mux) << 20 | (1 << 19) | (1 << 16));
	/* Wait for the measurement to be done */
	regval = readl(MSR_CLK_REG0);
	do {
		regval = readl(MSR_CLK_REG0);
	} while (regval & (1 << 31));

	/* Disable measuring */
	clrbits_le32(MSR_CLK_REG0, 1 << 16);
	regval = (readl(MSR_CLK_REG2) + 31) & 0x000fffff;

	return (regval / 10);
}

int ring_msr(int index)
{
	const char* clk_table[] = {
			[14] = "osc_ring_clk_dos3 " ,
			[13] = "osc_ring_clk_dos2 " ,
			[12] = "osc_ring_clk_dos1 " ,
			[11] = "osc_ring_clk_dos0 " ,
			[10] = "osc_ring_clk_ramc " ,
			[9] = "osc_ring_clk_ramb " ,
			[8] = "osc_ring_clk_rama " ,
			[7] = "osc_ring_clk_dspb " ,
			[6] = "osc_ring_clk_dspa " ,
			[5] = "osc_ring_clk_dmc " ,
			[4] = "osc_ring_clk_ddr " ,
			[3] = "osc_ring_clk_cpu3 " ,
			[2] = "osc_ring_clk_cpu2 " ,
			[1] = "osc_ring_clk_cpu1 " ,
			[0] = "osc_ring_clk_cpu0" ,
		};
	const int tb[] = {46,47,48,49,50, 51, 54, 55, 56, 57, 58, 59, 60};
	unsigned long i;
	uint8_t efuseinfo[4] = {0, 0, 0, 0};

	if ((index != 0xff) && (index != 0)) {
		if (bl31_get_cornerinfo(efuseinfo, sizeof(efuseinfo) / sizeof(uint8_t)) != 0) {
			printf("fail get efuse info\n");
			return 0;
		}
		if ((index >= 1) && (index <= 3))
			printf("%d\n", efuseinfo[index]);
		else
			printf("input data not support!\n");

		return 0;
	}
	if (0xff != index) {
		ring_powerinit();
		printf("set vcck vddee to 800mv\n");
	}
	/*RING_OSCILLATOR       0x7f: set slow ring*/
	writel(OSCRING_CTL_DATA0, OSCRING_CTL_REG0);
	writel(OSCRING_CTL_DATA1, OSCRING_CTL_REG1);

	for (i = 0; i < 15; i++) {
		printf("%s      :",clk_table[i]);
		printf("%ld     KHz",clk_util_ring_msr(tb[i]));
		printf("\n");
	}

	if (bl31_get_cornerinfo(efuseinfo, sizeof(efuseinfo) / sizeof(uint8_t)) != 0) {
		printf("fail get efuse info\n");
		return 0;
	}

	printf("osc efuse info:\n");
	for (i = 0; i < sizeof(efuseinfo) / sizeof(uint8_t); i++)
		printf("0x%x, ", efuseinfo[i]);
	printf("\n");

	return 0;
}
