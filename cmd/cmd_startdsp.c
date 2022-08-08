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
#include <asm/arch/pwr_ctrl.h>
#include <serial.h>

#ifndef CONFIG_CLK_P1_TYPE
#ifndef PM_DSPA
#define PM_DSPA PM_DSP
#endif
#ifndef PM_DSPB
#define PM_DSPB 99
#endif
#endif

/*
 * clk_util_set_dsp_clk
 * freq_sel: 0:800MHz  fclk_7
 *           1:400MHz  fclk_5
 *           2:500MHz  fclk_2/2
 *           4:333MHz  fclk_3/2
 *           5:250Mhz  fclk_2/4
 *           6:200Mhz  fclk_5/2
 *           7:100Mhz  fclk_5/4
 *           8:24Mhz   oscin
 *           others:286MHz fclk/7
 */

void set_dsp_clk(uint32_t id, uint32_t freq_sel)
{
	int			control;
	uint32_t	clk_sel;
	uint32_t	clk_div;
	uint32_t	addr = 0;

	switch ( id )
	{
		case 0  :  addr = CLKCTRL_DSPA_CLK_CTRL0; break;
#ifndef CONFIG_CLK_A5_TYPE
		case 1  :  addr = CLKCTRL_DSPB_CLK_CTRL0; break;
#endif
		default :  break;
	}
	// Make sure not busy from last setting and we currently match the last setting

	control = readl(addr);
	printf("control = 0x%x \n", control);
	printf ("CLKCTRL_DSP_CLK_CTRL0 =%x", readl(addr));
	printf ("\n");

#if defined(CONFIG_CLK_P1_TYPE) || defined(CONFIG_CLK_A5_TYPE)
	switch (freq_sel)
	{
		case 0  : clk_sel = 1; clk_div =0; printf ("CLK_UTIL:dsp[]:fclk2p5:800MHz\n")    ; break;
		case 1  : clk_sel = 1; clk_div =1; printf ("CLK_UTIL:dsp[]:fclk2p5/2:400MHz\n")  ; break;
		case 2  : clk_sel = 5; clk_div =0; printf ("CLK_UTIL:dsp[]:fclk4:500MHz\n")      ; break;
		case 3  : clk_sel = 2; clk_div =0; printf ("CLK_UTIL:dsp[]:fclk/3:667MHz\n")     ; break;
		case 4  : clk_sel = 2; clk_div =1; printf ("CLK_UTIL:dsp[]:fclk3/2:333MHz\n")    ; break;
		case 5  : clk_sel = 5; clk_div =1; printf ("CLK_UTIL:dsp[]:fclk4/2:250MHz\n")    ; break;
		case 6  : clk_sel = 1; clk_div =3; printf ("CLK_UTIL:dsp[]:fclk2p5/4:200MHz\n")  ; break;
		case 7  : clk_sel = 1; clk_div =7; printf ("CLK_UTIL:dsp[]:fclk5/4:100MHz\n")    ; break;
		case 8  : clk_sel = 0; clk_div =0; printf ("CLK_UTIL:dsp[]:oscin:24MHz\n")       ; break;
		case 10 : clk_sel = 0; clk_div =7; printf ("CLK_UTIL:dsp[]:oscin/8:3MHz\n")      ; break;
		default : clk_sel = 1; clk_div =0; printf ("CLK_UTIL:dsp[]:fclk2p5:800MHz\n")    ; break;
		}
#else
	switch (freq_sel)
	{
		case 0  : clk_sel = 1; clk_div =0; printf ("CLK_UTIL:dsp[]:fclk2p5:800MHz\n")    ; break;
		case 1  : clk_sel = 3; clk_div =0; printf ("CLK_UTIL:dsp[]:fclk5:400MHz\n")      ; break;
		case 2  : clk_sel = 5; clk_div =0; printf ("CLK_UTIL:dsp[]:fclk4:500MHz\n")      ; break;
		case 3  : clk_sel = 2; clk_div =0; printf ("CLK_UTIL:dsp[]:fclk/3:667MHz\n")     ; break;
		case 4  : clk_sel = 2; clk_div =1; printf ("CLK_UTIL:dsp[]:fclk3/2:333MHz\n")    ; break;
		case 5  : clk_sel = 5; clk_div =1; printf ("CLK_UTIL:dsp[]:fclk4/2:250MHz\n")    ; break;
		case 6  : clk_sel = 3; clk_div =1; printf ("CLK_UTIL:dsp[]:fclk5/2:200MHz\n")    ; break;
		case 7  : clk_sel = 3; clk_div =3; printf ("CLK_UTIL:dsp[]:fclk5/4:100MHz\n")    ; break;
		case 8  : clk_sel = 0; clk_div =0; printf ("CLK_UTIL:dsp[]:oscin:24MHz\n")       ; break;
		case 10 : clk_sel = 0; clk_div =7; printf ("CLK_UTIL:dsp[]:oscin/8:3MHz\n")      ; break;
		default : clk_sel = 3; clk_div =0; printf ("CLK_UTIL:dsp[]:fclk5:400MHz\n")      ; break;
	}
#endif

	if (control & (1 << 15)) {   //if sync_mux ==1, sel mux 0
		control = (control & ~( ( 1<<15) | (0x3ff<<0) | (0x7 <<10) ) ) | (1<<13)| (1<<29) | (clk_div<<0) | (clk_sel<<10);
	} else {
		control = (control & ~( ( 1<<15) | (0x3ff<<16) | (0x7 <<26) ) ) | (1<<13)| (1<<29) | (clk_div<<16) | (clk_sel<<26) | (1<<15);
	}

	writel(control, addr);

	switch ( id )
	{
		case 0  :  printf ("CLKCTRL_DSPA_CLK_CTRL0 =%x\n", readl(addr)); break;
		case 1  :  printf ("CLKCTRL_DSPB_CLK_CTRL0 =%x\n", readl(addr)); break;
		default :  break;
	}
}

static int do_startdsp(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint32_t addr;
	uint32_t dspid;
	uint32_t freq_sel;
	uint32_t cfg0;
	uint32_t StatVectorSel;
	uint32_t strobe = 1;

	int ret=0;

	if (argc <= 1) {
		printf("plese input dsp boot args:id, addrss, clk!\n");
		return CMD_RET_USAGE;
	}
	dspid = simple_strtoul(argv[1], NULL, 16);
	addr = simple_strtoul(argv[2], NULL, 16);
	freq_sel = simple_strtoul(argv[3], NULL, 16);

	printf("dsp%d boot \n",dspid);
	printf("dspboot start address:0x%x\n",addr);
	printf("dsp clk num:%d\n",freq_sel);

	StatVectorSel = (addr != 0xfffa0000);
	cfg0 = 0x1 |  StatVectorSel << 1 | strobe << 2;

	power_set_dsp((dspid == 0) ? PM_DSPA : PM_DSPB, PWR_ON);
	udelay(100);
	set_dsp_clk(dspid,freq_sel);
	udelay(100);
	init_dsp(dspid, addr, cfg0);
	printf("dsp init over! \n");
	return ret;
}


U_BOOT_CMD(
	startdsp, 4, 1, do_startdsp,
	"load dspboot.bin from address",
	"\narg[0]: cmd\n"
	"arg[1]: dspid \n"
	"arg[2]: dspboot.bin load address!\n"
	"arg[3]: dsp clk set 0:800M 1:400M 2:500M 4:333M 5:250M 6:200M 7:100M 8 :24M"
);




