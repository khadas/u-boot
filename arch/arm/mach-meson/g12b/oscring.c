// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <config.h>
#include <common.h>
#include <asm/arch/io.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/mailbox.h>
#include <asm/arch/oscring.h>

unsigned long clk_util_ring_msr(unsigned long clk_mux)
{
	unsigned int regval = 0;

	WRITE_CBUS_REG(MSR_CLK_REG0, 0);
	/* Set the measurement gate to 64uS */
	CLEAR_CBUS_REG_MASK(MSR_CLK_REG0, 0xffff);
	/* 64uS is enough for measure the frequence? */
	SET_CBUS_REG_MASK(MSR_CLK_REG0, (10000 - 1));
	/* Disable continuous measurement */
	/* Disable interrupts */
	CLEAR_CBUS_REG_MASK(MSR_CLK_REG0, ((1 << 18) | (1 << 17)));
	CLEAR_CBUS_REG_MASK(MSR_CLK_REG0, (0x7f << 20));
	SET_CBUS_REG_MASK(MSR_CLK_REG0, (clk_mux << 20) | /* Select MUX */
			(1 << 19) |       /* enable the clock */
			(1 << 16));       /* enable measuring */
	/* Wait for the measurement to be done */
	regval = READ_CBUS_REG(MSR_CLK_REG0);
	do {
		regval = READ_CBUS_REG(MSR_CLK_REG0);
	} while (regval & (1 << 31));

	/* Disable measuring */
	CLEAR_CBUS_REG_MASK(MSR_CLK_REG0, (1 << 16));
	regval = (READ_CBUS_REG(MSR_CLK_REG2) + 31) & 0x000FFFFF;

	return (regval / 10);
}

void ring_powerinit(void)
{
	writel(0x150007, RING_PWM_A); /* set pwm_a 0.8v */
	writel(0x150007, RING_PWM_B); /* set pwm_b 0.8v */
	writel(0x10000c, RING_PWM_EE);/* set ee 0.8v */
}

int ring_msr(int index)
{
	const char* clk_table[] = {
			[11] = "sys_cpu_ring_osc_clk[1] " ,
			[10] = "sys_cpu_ring_osc_clk[0] " ,
			[9] = "am_ring_osc_clk_out_ee[9] " ,
			[8] = "am_ring_osc_clk_out_ee[8] " ,
			[7] = "am_ring_osc_clk_out_ee[7] " ,
			[6] = "am_ring_osc_clk_out_ee[6] " ,
			[5] = "am_ring_osc_clk_out_ee[5] " ,
			[4] = "am_ring_osc_clk_out_ee[4] " ,
			[3] = "am_ring_osc_clk_out_ee[3] " ,
			[2] = "am_ring_osc_clk_out_ee[2] " ,
			[1] = "am_ring_osc_clk_out_ee[1] " ,
			[0] = "am_ring_osc_clk_out_ee[0] " ,
		};
	const int tb[] = {0, 1, 2, 99, 100, 101, 102, 103, 104, 105, 3, 33};
	unsigned long i = 0;
	unsigned char efuseinfo[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	if ((index != 0xff) && (index != 0)) {
		if (oscring_get_value(efuseinfo) != 0) {
			printf("fail get efuse info\n");
			return 0;
		}
		if ((index >= 1) && (index <= 4))
			printf("%d\n", (efuseinfo[index] * 20));
		if ((index >= 5) && (index <= 7)) {
			if (efuseinfo[0] == 1) {
				printf("%d uA ", (efuseinfo[i] * 200));
			} else if (efuseinfo[0] == 2) {
				printf("%d uA ", (efuseinfo[i] * 400));
			}
		}
		if ((index >= 8) && (index <= 11))
			printf("%d\n", efuseinfo[index]);
		if ((index < 0) || (index >= 12))
			printf("input data not support!\n");
		return 0;
	}

	if (index != 0xff) {
		ring_powerinit();
		printf("set vcck vddee 800mv\n");
	}
	/*RING_OSCILLATOR       0x7f: set slow ring*/
	writel(OSCRING_CTL_DATA, OSCRING_CTL_REG);
	for (i = 0; i < 12; i++) {
		printf("%s      :",clk_table[i]);
		printf("%ld     KHz",clk_util_ring_msr(tb[i]));
		printf("\n");
	}

	if (oscring_get_value(efuseinfo) != 0) {
		printf("fail get efuse info\n");
		return 0;
	}

	printf("osc efuse info:\n");
	for (i = 0; i < sizeof(efuseinfo) / sizeof(uint8_t); i++)
		printf("0x%x, ", efuseinfo[i]);
	printf("\n");

	/*efuse to test value*/
	printf("cpub[1], ee[1], ee[0], cpu[1], iddee, iddcpub, iddcpu, sltver, fta53, fta73, slt\n");
	for (i = 1; i <= 4; i++)
		printf("%d KHz ", (efuseinfo[i] * 20));

	for (i = 5; i <= 7; i++) {
		if (efuseinfo[0] == 1) {
			printf("%d uA ", (efuseinfo[i] * 200));
		} else if (efuseinfo[0] == 2) {
			printf("%d uA ", (efuseinfo[i] * 400));
		}
	}

	for (i = 8; i <= 11; i++)
		printf("%d  ", efuseinfo[i]);

	printf("\n");

	return 0;
}
