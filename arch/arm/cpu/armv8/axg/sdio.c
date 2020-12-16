/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/axg/sdio.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <config.h>
#include <asm/arch/io.h>
#include <asm/arch/cpu_sdio.h>
#include <asm/arch/secure_apb.h>
#include <common.h>

void  cpu_sd_emmc_pwr_prepare(unsigned port)
{
//    switch(port)
//    {
//        case SDIO_PORT_A:
//            clrbits_le32(P_PREG_PAD_GPIO4_EN_N,0x30f);
//            clrbits_le32(P_PREG_PAD_GPIO4_O   ,0x30f);
//            clrbits_le32(P_PERIPHS_PIN_MUX_8,0x3f);
//            break;
//        case SDIO_PORT_B:
//            clrbits_le32(P_PREG_PAD_GPIO5_EN_N,0x3f<<23);
//            clrbits_le32(P_PREG_PAD_GPIO5_O   ,0x3f<<23);
//            clrbits_le32(P_PERIPHS_PIN_MUX_2,0x3f<<10);
//            break;
//        case SDIO_PORT_C:
//            //clrbits_le32(P_PREG_PAD_GPIO3_EN_N,0xc0f);
//            //clrbits_le32(P_PREG_PAD_GPIO3_O   ,0xc0f);
//            //clrbits_le32(P_PERIPHS_PIN_MUX_6,(0x3f<<24));break;
//            break;
//    }

    /**
        do nothing here
    */
}
unsigned sd_debug_board_1bit_flag = 0;
int cpu_sd_emmc_init(unsigned port)
{

	//printf("inand sdio  port:%d\n",port);
	switch (port)
	{
	case SDIO_PORT_B:
		if (sd_debug_board_1bit_flag == 1)
			clrsetbits_le32(P_PERIPHS_PIN_MUX_4, 0xEE000E, 0x110001);
		else
			clrsetbits_le32(P_PERIPHS_PIN_MUX_4, 0xEEEEEE, 0x111111);
		break;
	case SDIO_PORT_C:
		writel(0x7fff, P_PAD_PULL_UP_EN_REG4);
		writel(0x5fff, P_PAD_PULL_UP_REG4);
		clrsetbits_le32(P_PERIPHS_PIN_MUX_0, 0xEEEEEEEE, 0x11111111);
		clrsetbits_le32(P_PERIPHS_PIN_MUX_1, 0xEFFEFE, 0x100101);

		/* hardware reset with pull boot9 */
		clrbits_le32(PREG_PAD_GPIO4_EN_N, 1<<9);
		clrbits_le32(PREG_PAD_GPIO4_O, 1<<9);
		udelay(10);
		setbits_le32(PREG_PAD_GPIO4_O, 1<<9);

		break;
	case SDIO_PORT_A:
		//printf("no port A on axg!\n");
	default:
		return -1;
	}
	return 0;
}

__weak void sd_emmc_para_config(struct sd_emmc_global_regs *reg,
		unsigned int clock, unsigned int port)
{
	unsigned int clk = reg->gclock;

	clk &= ~(3 << Cfg_co_phase);
	clk |= (3 << Cfg_co_phase);
	reg->gclock = clk;
	return;
}
