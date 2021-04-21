// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <asm/arch/cpu.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/timer.h>
#include <asm/arch/bl31_apis.h>
#include <asm/arch/register.h>
#include <serial.h>

void dsp_reset(uint32_t id,uint32_t reset_addr)
{
	uint32_t StatVectorSel;
	uint32_t strobe = 1;
	//address remap
	printf("start_dsp \n");
	//*P_DSP_REMAP2 = 0x3000fff0; //replace the highest 12bits 0xfffxxxxx with 0x300xxxxx

	StatVectorSel = (reset_addr!= 0xfffa0000);

	//open the Dreset, Breset, irq_clken
	if (id == 0) { //dspa
		init_dsp(id,reset_addr, (0x1 | StatVectorSel<<1 | strobe<<2));
		udelay(50);

		writel((readl(DSP_CFG0) &( ~(0xffff <<0))) | (0x2018 << 0) | (1<<29), DSP_CFG0);
		udelay(10);

		writel(readl(DSP_CFG0) | (1<<31), DSP_CFG0); //Dreset deassert
		udelay(10);

		writel(readl(DSP_CFG0) & ~(1<<31), DSP_CFG0); //Dreset assert
		udelay(10);

		writel(readl(DSP_CFG0) | (1<<30), DSP_CFG0); //Breset deassert
		udelay(10);

		writel(readl(DSP_CFG0) & ~(1<<30), DSP_CFG0); //Breset
		udelay(10);
		printf("DSP_CFG0 : value_0x%8x \n",readl(DSP_CFG0));
	} else {
		init_dsp(id,reset_addr, (0x1 | StatVectorSel<<1 | strobe<<2));
		udelay(50);
		writel((readl(DSPB_CFG0) & (~(0xffff <<0))) | (0x2019 << 0) | (1<<29), DSPB_CFG0);
		udelay(10);
		writel(readl(DSPB_CFG0) & ~(1<<31), DSPB_CFG0); //Dreset
		udelay(10);
		writel(readl(DSPB_CFG0) & ~(1<<30), DSPB_CFG0); //Breset
		udelay(10);
		printf("DSPB_CFG0 : value_0x%8x \n",readl(DSPB_CFG0));
	}
}

static int do_dsprun(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned long addr;
	unsigned int dspid;
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

