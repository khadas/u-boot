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
 *      for A1 (fclk = 1536M)
 *           0:384MHz  fclk_4
 *           1:308MHz  fclk_5
 *           2:256MHz  fclk_3/2
 *           3:24MHz   oscin
 *     default:384MHz  fclk_4
 *
 *      for Others (fclk = 2G)
 *           0:400MHz  fclk_5
 *           1:333MHz  fclk_3/2
 *           2:250MHz  fclk_2/4
 *           3:200MHz  fclk_5/2
 *           4:100MHz  fclk_5/4
 *           5:24MHz   oscin
 *           6:3MHz    oscin/8
 *      and for SC2
 *           7:800MHz  fclk_2p5
 *           8:667MHz  fclk_3
 *           9:500MHz  fclk_4
 *     default:400MHz  fclk_5
 */

// --------------------------------------------------
//              clk_util_set_dsp_clk
// --------------------------------------------------
int clk_util_set_dsp_clk(uint32_t id, uint32_t freq_sel)
{
	uint32_t control;
	uint32_t clk_sel;
	uint32_t clk_div;
	uint32_t addr = 0;
	char *dsp_clk_tag = NULL;
	int ret = 0;

	switch (id) {
	case 0:
		addr = CLKTREE_DSPA_CLK_CTRL0;
		break;
#if !defined(CONFIG_CLK_MESON_C2) && !defined(CONFIG_CLK_MESON_SC2)
	case 1:
		addr = CLKTREE_DSPB_CLK_CTRL0;
		break;
#endif
	default:
		ret = -1;
		break;
	}

	if (ret) {
		printf("dsp clk set failed: invalid id\n");
		return ret;
	}

	// Make sure not busy from last setting and we currently match the last setting

	control = readl(addr);
	printf("CLKTREE_DSP_CLK_CTRL0 value 0x%x\n", control);

	switch (freq_sel) {
#ifdef CONFIG_CLK_MESON_A1
	case 0:
		clk_sel = 5;
		clk_div = 0;
		dsp_clk_tag = "fclk4:384MHz";
		break;
	case 1:
		clk_sel = 3;
		clk_div = 0;
		dsp_clk_tag = "fclk5:308MHz";
		break;
	case 2:
		clk_sel = 2;
		clk_div = 1;
		dsp_clk_tag = "fclk3/2:256MHz";
		break;
	case 3:
		clk_sel = 0;
		clk_div = 0;
		dsp_clk_tag = "oscin:24MHz";
		break;
	default:
		clk_sel = 5;
		clk_div = 0;
		dsp_clk_tag = "fclk4:384MHz (default)";
		break;
#else
	case 0:
		clk_sel = 3;
		clk_div = 0;
		dsp_clk_tag = "fclk5:400MHz";
		break;
	case 1:
		clk_sel = 2;
		clk_div = 1;
		dsp_clk_tag = "fclk3/2:333MHz";
		break;
	case 2:
		clk_sel = 5;
		clk_div = 1;
		dsp_clk_tag = "fclk4/2:250MHz";
		break;
	case 3:
		clk_sel = 3;
		clk_div = 1;
		dsp_clk_tag = "fclk5/2:200MHz";
		break;
	case 4:
		clk_sel = 3;
		clk_div = 3;
		dsp_clk_tag = "fclk5/4:100MHz";
		break;
	case 5:
		clk_sel = 0;
		clk_div = 0;
		dsp_clk_tag = "oscin:24MHz";
		break;
	case 6:
		clk_sel = 0;
		clk_div = 7;
		dsp_clk_tag = "oscin/8:3MHz";
		break;
#ifdef CONFIG_CLK_MESON_SC2
	case 7:
		clk_sel = 1;
		clk_div = 0;
		dsp_clk_tag = "fclk2p5:800MHz";
		break;
	case 8:
		clk_sel = 2;
		clk_div = 0;
		dsp_clk_tag = "fclk3:667MHz";
		break;
	case 9:
		clk_sel = 5;
		clk_div = 0;
		dsp_clk_tag = "fclk4:500MHz";
		break;
#endif
	default:
		clk_sel = 3;
		clk_div = 0;
		dsp_clk_tag = "fclk5:400MHz (default)";
		break;
#endif
	}
	printf("CLK_UTIL:dsp[%d]:%s\n", id, dsp_clk_tag);

	if (control & (1 << 15)) {  //if sync_mux ==1, sel mux 0
		control = (control & ~((1 << 15) | (0x3ff << 0) | (0x7 << 10))) |
			(1 << 13) | (1 << 29) | (clk_div << 0) | (clk_sel << 10);
	} else {
		control = (control & ~((1 << 15) | (0x3ff << 16) | (0x7 << 26))) |
			(1 << 13) | (1 << 29) | (clk_div << 16) | (clk_sel << 26) | (1 << 15);
	}
	printf("CLKTREE_DSP_CLK_CTRL0  value 0x%x\n", control);
	writel(control, addr);
	printf("CLKTREE_DSP_CLK_CTRL0  value 0x%x\n", readl(addr));

	return ret;
}

int dsp_clk_init(unsigned int dspid, uint32_t freq_sel)
{
	return clk_util_set_dsp_clk(dspid, freq_sel);
}

void dsp_power_set(unsigned int dspid, uint32_t powerflag)
{
	power_set_dsp(dspid, powerflag);
}

static int do_dspset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int dspid;
	uint32_t freq_sel;
	uint32_t powerflag;

	if (argc <= 1) {
		printf("plese input dsp boot args: id, address, clk!\n");
		return CMD_RET_USAGE;
	}

	dspid = simple_strtoul(argv[1], NULL, 16);
	if (dspid > 1) {
		printf("dsp set failed: invalid id\n");
		return -1;
	}

	freq_sel = simple_strtoul(argv[2], NULL, 16);
	powerflag = simple_strtoul(argv[3], NULL, 16);
	printf("dsp%d boot\n", dspid);
	printf("dsp clk num:%d\n", freq_sel);

	if (powerflag == 1)
		printf("power on dsp init\n");
	else
		printf("power off dsp init\n");

//	writel32(readl(CLKTREE_SYS_CLK_EN0) | (1<<30),CLKTREE_SYS_CLK_EN0);
//	printf("CLKTREE_SYS_CLK_EN0  value 0x%x \n",readl(CLKTREE_SYS_CLK_EN0));

	if (dsp_clk_init(dspid, freq_sel))
		return -1;
	udelay(10);
	dsp_power_set(dspid, powerflag);
	printf("dsp init CLK, power over!\n");

	return 0;
}

U_BOOT_CMD(
	dspset, 4, 1, do_dspset,
	"set dsp id, clk, power domain",
	"\narg[0]: cmd\n"
	"arg[1]: dspid\n"
	"arg[2]: dsp clk set\nfor A1: 0:384M 1:308M 2:256M 3:24M\nfor others: 0:400M 1:333M 2:250M 3:200M 4:100M 5:24M 6:3M\nfor SC2: others 0~6 and 7:800M 8:667M 9:500M\n"
	"arg[3]: power on [1] / power off [0]"
);

