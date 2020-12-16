/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/gxtvbb/sdio.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <config.h>
#include <asm/arch/io.h>
#include <asm/arch/cpu_sdio.h>
#include <asm/arch/secure_apb.h>

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
        case SDIO_PORT_A:
			//setbits_le32(P_PERIPHS_PIN_MUX_8,0x3f);
			break;
        case SDIO_PORT_B:
			if (sd_debug_board_1bit_flag == 1) {
                clrbits_le32(P_PERIPHS_PIN_MUX_5,(0x1<<7));
				setbits_le32(P_PERIPHS_PIN_MUX_5,(0x7<<2));
			} else {
                clrbits_le32(P_PERIPHS_PIN_MUX_5,(0x1f<<7));
				clrbits_le32(P_PERIPHS_PIN_MUX_10,(0x1<<9));
				setbits_le32(P_PERIPHS_PIN_MUX_5,(0x3f<<1));
			}
			break;
        case SDIO_PORT_C://SDIOC GPIOB_2~GPIOB_7
			clrbits_le32(P_PERIPHS_PIN_MUX_9,(0x3f<<18));
			clrbits_le32(P_PERIPHS_PIN_MUX_10,(0x1<<9));
			setbits_le32(P_PERIPHS_PIN_MUX_5,(0xf<<12));
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
