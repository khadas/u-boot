/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/gxl/sdio.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <config.h>
#include <asm/arch/io.h>
#include <asm/arch/cpu_sdio.h>
#include <asm/arch/secure_apb.h>
#include <asm/cpu_id.h>
#include <common.h>

void  cpu_sd_emmc_pwr_prepare(unsigned port)
{
    /** here you can add io pin voltage operation if you need.
		but now it was used to open emmc gate for gxm
    */

	if ((port == SDIO_PORT_C)
        && (get_cpu_id().family_id >= MESON_CPU_MAJOR_ID_GXM)
        && (readl(HHI_NAND_CLK_CNTL) != 0x80))
        writel(0x80, HHI_NAND_CLK_CNTL);

}
unsigned sd_debug_board_1bit_flag = 0;
int cpu_sd_emmc_init(unsigned port)
{

	//printf("inand sdio  port:%d\n",port);
	switch (port)
	{
	case SDIO_PORT_A:
        setbits_le32(P_PERIPHS_PIN_MUX_5, (0x3f << 26) | (0x1 << 24));
		break;
	case SDIO_PORT_B:
		if (sd_debug_board_1bit_flag == 1)
			setbits_le32(P_PERIPHS_PIN_MUX_6, 0x7 << 2);
        else {
            clrbits_le32(P_PERIPHS_PIN_MUX_6, 0x3f << 6);
			setbits_le32(P_PERIPHS_PIN_MUX_6, 0x3f << 0);
        }
		break;
	case SDIO_PORT_C://SDIOC GPIOB_2~GPIOB_7
		/* pull up data by default */
		setbits_le32(P_PAD_PULL_UP_EN_REG2, 0xffff);
		setbits_le32(P_PAD_PULL_UP_REG2, 0xffff);
		/* set pinmux */
		clrbits_le32(P_PERIPHS_PIN_MUX_7, 0xff | (0xff << 16));
		setbits_le32(P_PERIPHS_PIN_MUX_7, 0x7 << 29);
		/* hare ware reset with pull boot9 */
		clrbits_le32(P_PREG_PAD_GPIO2_EN_N, 1<<9);
		clrbits_le32(P_PREG_PAD_GPIO2_O, 1<<9);
		udelay(10);
		setbits_le32(P_PREG_PAD_GPIO2_O, 1<<9);
		break;
	default:
		return -1;
	}
	return 0;
}

__weak void sd_emmc_para_config(struct sd_emmc_global_regs *reg,
		unsigned int clock, unsigned int port)
{
	return;
}
