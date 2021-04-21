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
	writel(0x90009, RING_PWM_EE);/*set ee 0.8v*/
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
	mdelay(10);
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
			[41] = "top[3] (LVT16) " ,
			[40] = "top[2] (LVT16) " ,
			[39] = "top[1] (ULVT16) " ,
			[38] = "top[0] (ULVT16) " ,
			[37] = "core3[5] (LVT16) " ,
			[36] = "core3[4] (LVT16) " ,
			[35] = "core3[3] (LVT16)" ,
			[34] = "core3[2] (ULVT16) " ,
			[33] = "core3[1] (ULVT16) " ,
			[32] = "core3[0] (ULVT16) " ,
			[31] = "core2[5] (LVT16) " ,
			[30] = "core2[4] (LVT16) " ,
			[29] = "core2[3] (LVT16)" ,
			[28] = "core2[2] (ULVT16) " ,
			[27] = "core2[1] (ULVT16) " ,
			[26] = "core2[0] (ULVT16) " ,
			[25] = "core1[5] (LVT16) " ,
			[24] = "core1[4] (LVT16) " ,
			[23] = "core1[3] (LVT16)" ,
			[22] = "core1[2] (ULVT16) " ,
			[21] = "core1[1] (ULVT16) " ,
			[20] = "core1[0] (ULVT16) " ,
			[19] = "core0[5] (LVT16) " ,
			[18] = "core0[4] (LVT16) " ,
			[17] = "core0[3] (LVT16)" ,
			[16] = "core0[2] (ULVT16) " ,
			[15] = "core0[1] (ULVT16) " ,
			[14] = "core0[0] (ULVT16) " ,
			[13] = "demod[1] (LVT16) " ,
			[12] = "demod[0] (SVT24) " ,
			[11] = "ram (SVT16) " ,
			[10] = "dos (ULVT20) " ,
			[9] = "dos (LVT16) " ,
			[8] = "dos (SVT16) " ,
			[7] = "dos (SVT24) " ,
			[6] = "vpu (LVT16) " ,
			[5] = "vpu (LVT20) " ,
			[4] = "vpu (SVT24) " ,
			[3] = "mali (SVT16) " ,
			[2] = "mali (ULVT16) " ,
			[1] = "mali (LVT16) " ,
			[0] = "dmc_top (LVT16) " ,
		};
	const int tb[] = {180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
			  190, 192, 193, 194, 196, 197, 198, 199, 200, 201,
			  202, 203, 204, 205, 206, 207, 208, 209, 210, 211,
			  212, 213, 214, 215, 216, 217, 218, 219, 220, 221,
			  222, 223, 224, 225, 226, 227, 228, 229, 230, 231,
			  232};
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

	for (i = 0; i < 42; i++) {
		printf("%s      :",clk_table[i]);
		printf("%ld     KHz",clk_util_ring_msr(tb[i]));
		printf("\n");
	}

	if (bl31_get_cornerinfo(efuseinfo, sizeof(efuseinfo) / sizeof(uint8_t)) != 0) {
		printf("fail get efuse info\n");
		return 0;
	}

	printf("osc efuse info:\n");
	for (i = 0; i < 4; i++)
		printf("0x%x, ", efuseinfo[i]);
	printf("\n");

	/*efuse to test value*/
	printf("sys_cpu_osc_ring0(ulvt16), idd_cpu, idd_ee\n");

	printf("%d KHz ", (efuseinfo[1] * 20));

	for (i = 2; i <=3; i++) {
		printf("%d uA ",  (efuseinfo[i] * 400));
	}
	printf("\n");

	return 0;
}
