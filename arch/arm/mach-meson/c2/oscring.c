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
	writel(0x090009, RING_PWM_EE);/*set ee 0.8v*/
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
			[26] = "osc_ring_clk_top(14_rvt) " ,
			[25] = "osc_ring_clk_top(16_lvt) " ,
			[24] = "osc_ring_clk_top(16_rvt) " ,
			[23] = "osc_ring_clk_isp(14_rvt) " ,
			[22] = "osc_ring_clk_isp(16_lvt) " ,
			[21] = "osc_ring_clk_isp(16_rvt) " ,
			[20] = "osc_ring_clk_ddr(14_slvt) " ,
			[19] = "osc_ring_clk_ddr(14_rvt) " ,
			[18] = "osc_ring_clk_ddr(16_lvt) " ,
			[17] = "osc_ring_clk_sram(14_lvt) " ,
			[16] = "osc_ring_clk_sram(16_slvt) " ,
			[15] = "osc_ring_clk_sram(16_lvt) " ,
			[14] = "osc_ring_clk_dsp(14lvt) " ,
			[13] = "osc_ring_clk_dsp(14rvt) " ,
			[12] = "osc_ring_clk_dsp(16_rvt) " ,
			[11] = "osc_ring_clk_dos(14_lvt) " ,
			[10] = "osc_ring_clk_dos(14_rvt) " ,
			[9] = "osc_ring_clk_dos(16_rvt) " ,
			[8] = "osc_ring_clk_nna(14_lvt) " ,
			[7] = "osc_ring_clk_nna(16_slvt) " ,
			[6] = "osc_ring_clk_nna(16_lvt) " ,
			[5] = "osc_ring_clk_cpu1(9T_14_slvt) " ,
			[4] = "osc_ring_clk_cpu1(9T_14_lvt) " ,
			[3] = "osc_ring_clk_cpu1(9T_16_slvt) " ,
			[2] = "osc_ring_clk_cpu0(9T_14_slvt) " ,
			[1] = "osc_ring_clk_cpu0(9T_14_lvt) " ,
			[0] = "osc_ring_clk_cpu0(9T_16_slvt) " ,
		};
	const int tb[] = {93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
			  108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119};
	unsigned long i;
	uint8_t efuseinfo[8] = {0, 0, 0, 0, 0, 0, 0, 0};

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
	writel(OSCRING_CTL_DATA2, OSCRING_CTL_REG2);

	for (i = 0; i < 27; i++) {
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

	/*efuse to test value*/
	printf("A35_core0(2) (14SLVT), idd_ee, idd_cpu\n");

	printf("%d KHz ", (efuseinfo[1] * 50));

	for (i = 2; i <=3; i++) {
		printf("%d uA ",  (efuseinfo[i] * 400));
	}
	printf("\n");

	return 0;
}
