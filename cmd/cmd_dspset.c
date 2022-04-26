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


/*
 * clk_util_set_dsp_clk
 * freq_sel:
 *           1:400MHz  fclk_5
 *           2:500MHz  fclk_2/2
 *           4:333MHz  fclk_3/2
 *           5:250Mhz  fclk_2/4
 *           6:200Mhz  fclk_5/2
 *           7:100Mhz  fclk_5/4
 *           8:24Mhz   oscin
 *           10:3Mhz    oscin/8
 *           others:400MHz  fclk_5
 */

// --------------------------------------------------
//              clk_util_set_dsp_clk
// --------------------------------------------------
void clk_util_set_dsp_clk(uint32_t id, uint32_t freq_sel)
{
	uint32_t    control;
	uint32_t    clk_sel;
	uint32_t    clk_div;
	uint32_t    addr;

	switch ( id ) {
		case 0:   addr = CLKTREE_DSPA_CLK_CTRL0;break;
		default : addr = CLKTREE_DSPB_CLK_CTRL0;break;
	}

	// Make sure not busy from last setting and we currently match the last setting


	control = readl(addr);
	printf("CLKTREE_DSP_CLK_CTRL0  value 0x%x \n",control);

	switch (freq_sel)
	{
		case 1  : clk_sel = 3; clk_div =0; printf ("CLK_UTIL:dsp[%d]:fclk5:400MHz\n" ,id); break;
		case 2  : clk_sel = 1; clk_div =1; printf ("CLK_UTIL:dsp[%d]:fclk2/2:500MHz\n" ,id); break;
		//case 3  : clk_sel = 1; clk_div =0; printf ("CLK_UTIL:dsp[%d]:fclk/3:667MHz\n" ,id); break;
		case 4  : clk_sel = 2; clk_div =1; printf ("CLK_UTIL:dsp[%d]:fclk3/2:333MHz\n" ,id); break;
		case 5  : clk_sel = 1; clk_div =3; printf ("CLK_UTIL:dsp[%d]:fclk2/4:250MHz\n",id); break;
		case 6  : clk_sel = 3; clk_div =1; printf ("CLK_UTIL:dsp[%d]:fclk5/2:200MHz\n",id); break;
		case 7  : clk_sel = 3; clk_div =3; printf ("CLK_UTIL:dsp[%d]:fclk5/4:100MHz\n",id); break;
		case 8  : clk_sel = 0; clk_div =0; printf ("CLK_UTIL:dsp[%d]:oscin:24MHz\n",id); break;
		case 10 : clk_sel = 0; clk_div =7; printf ("CLK_UTIL:dsp[%d]:oscin/8:3MHz\n",id); break;
		default : clk_sel = 3; clk_div =0; printf ("CLK_UTIL:dsp[%d]:fclk5:400MHz\n" ,id); break;
	}

	if (control & (1 << 15)) {  //if sync_mux ==1, sel mux 0
		control = (control & ~( ( 1<<15) | (0x3ff<<0) | (0x7 <<10) ) ) | (1<<13)| (1<<29) | (clk_div<<0) | (clk_sel<<10);
	} else {
		control = (control & ~( ( 1<<15) | (0x3ff<<16) | (0x7 <<26) ) ) | (1<<13)| (1<<29) | (clk_div<<16) | (clk_sel<<26) | (1<<15);
	}
	printf("CLKTREE_DSP_CLK_CTRL0  value 0x%x \n",control);
	writel(control,addr);
	printf("CLKTREE_DSP_CLK_CTRL0  value 0x%x \n",readl(addr));

}

void dsp_clk_init(unsigned int dspid,  uint32_t freq_sel) {
	clk_util_set_dsp_clk(dspid,freq_sel);
}

void dsp_power_set(unsigned int dspid,  uint32_t powerflag) {
	power_set_dsp(dspid,powerflag);
}


static int do_dspset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int dspid;
	uint32_t freq_sel;
	uint32_t powerflag;
	int ret=0;
	if (argc <= 1) {
		printf("plese input dsp boot args:id, addrss, clk!\n");
		return CMD_RET_USAGE;
	}
	dspid = simple_strtoul(argv[1], NULL, 16);
	freq_sel = simple_strtoul(argv[2], NULL, 16);
	powerflag = simple_strtoul(argv[3], NULL, 16);
	printf("dsp%d boot \n",dspid);
	printf("dsp clk num:%d\n",freq_sel);
	if (powerflag == 1)
		printf("power on dsp init \n");
	else
		printf("power off dsp init \n");

//	writel32(readl(CLKTREE_SYS_CLK_EN0) | (1<<30),CLKTREE_SYS_CLK_EN0);
//	printf("CLKTREE_SYS_CLK_EN0  value 0x%x \n",readl(CLKTREE_SYS_CLK_EN0));

	dsp_clk_init(dspid, freq_sel);
	udelay(10);
	dsp_power_set(dspid,  powerflag) ;
	printf("dsp init CLK, power over! \n");

	return ret;
}


U_BOOT_CMD(
	dspset,	4,	1,	do_dspset,
	"set dsp clk, power domain",
	"\n arg[0]: cmd\n"
	"arg[1]: dspid \n"
	"arg[2]: dsp clk set 1:400M 2:500M 4:333M 5:250M 6:200M 7:100M 8 :24M \n"
	"arg[3]: power on [1] / power off [0]"
);




