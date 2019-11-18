/*
 * common/cmd_reboot.c
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <common.h>
#include <command.h>
#include <asm/arch/cpu.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/timer.h>
#include <asm/arch/bl31_apis.h>
#include <asm/arch/p_register.h>
#include <serial.h>



void dsp_reset(uint32_t id,uint32_t reset_addr)
{
	uint32_t StatVectorSel;
	uint32_t strobe = 1;
	//address remap
	printf("\n start_dsp \n");
	//*P_DSP_REMAP2 = 0x3000fff0; //replace the highest 12bits 0xfffxxxxx with 0x300xxxxx

	StatVectorSel = (reset_addr!= 0xfffa0000);

	//open the Dreset, Breset, irq_clken
	if (id == 0) { //dspa
		init_dsp(id,reset_addr, (0x1 | StatVectorSel<<1 | strobe<<2));
		udelay(50);
		printf("\n *P_DSP_CFG0 : ADDR_0X%p, value_0x%8x \n",P_DSP_CFG0,*P_DSP_CFG0);
		*P_DSP_CFG0 = (*P_DSP_CFG0 & ~(0xffff <<0)) | (0x2018 << 0) | (1<<29) | (0<<0) ;      //irq_clken
		udelay(10);

		*P_DSP_CFG0 = *P_DSP_CFG0 | (1<<31);     //Dreset deassert
		udelay(10);

		*P_DSP_CFG0 = *P_DSP_CFG0 & ~(1<<31);     //Dreset assert
		udelay(10);

		*P_DSP_CFG0 = *P_DSP_CFG0 | (1<<30);    //Breset deassert
		udelay(10);
		*P_DSP_CFG0 = *P_DSP_CFG0 & ~(1<<30);    //Breset
		udelay(10);
		printf("\n *P_DSP_CFG0 : ADDR_0X%p, value_0x%8x \n",P_DSP_CFG0,*P_DSP_CFG0);
	} else {
		init_dsp(id,reset_addr, (0x1 | StatVectorSel<<1 | strobe<<2));
		udelay(50);
		printf("\n *P_DSPB_CFG0 : ADDR_0X%p, value_0x%8x \n",P_DSPB_CFG0,*P_DSPB_CFG0);
		*P_DSPB_CFG0 = (*P_DSPB_CFG0 & ~(0xffff <<0)) | (0x2019 << 0) | (1<<29) | (0<<0) ;      //irq_clken
		udelay(10);
		*P_DSPB_CFG0 = *P_DSPB_CFG0 & ~(1<<31);     //Dreset
		udelay(10);
		*P_DSPB_CFG0 = *P_DSPB_CFG0 & ~(1<<30);    //Breset
		udelay(10);
		printf("\n *P_DSPB_CFG0 : ADDR_0X%p, value_0x%8x \n",P_DSPB_CFG0,*P_DSPB_CFG0);
	}
}

static int do_dsprun(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned long addr;
	unsigned int dspid;
	uint32_t freq_sel;
	int ret=0;
	if (argc <= 1) {
		printf("plese input dsp boot args:id, addrss, clk!\n");
		return CMD_RET_USAGE;
	}
	dspid = simple_strtoul(argv[1], NULL, 16);
	addr = simple_strtoul(argv[2], NULL, 16);
	printf("dsp%d boot \n",dspid);
	printf("dspboot start address:0x%lx\n",addr);
	dsp_reset(dspid, addr);
	printf("dsp init over! \n");
	return ret;
}


U_BOOT_CMD(
	dsprun,	3,	1,	do_dsprun,
	"load dspboot.bin from ddr address",
	"arg[0]: cmd\n"
	"arg[1]: dspid \n"
	"arg[2]: dspboot.bin load address!"
);




