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

// --------------------------------------------------
//                    set_dsp_clk
// --------------------------------------------------

/*
 * set_dsp_clk
 * freq_sel:
 *      [ for A1 (fclk = 1536M) ]
 *           0:384MHz  fclk_2/2
 *           1:308MHz  fclk_5
 *           2:256MHz  fclk_3/2
 *           3:24MHz   oscin
 *     default:384MHz  fclk_4
 *
 *      [ for C1 and C2 ]
 *           0:400MHz  fclk_5
 *           1:333MHz  fclk_3/2
 *           2:250MHz  fclk_2/4
 *           3:200MHz  fclk_5/2
 *           4:100MHz  fclk_5/4
 *           5:24MHz   oscin
 *           6:3MHz    oscin/8
 *     default:400MHz  fclk_5
 *
 *      [ for Others ]
 *           0:800MHz  fclk_2p5
 *           1:667MHz  fclk_3
 *           2:500MHz  fclk_4
 *           3:400MHz  fclk_2p5/2
 *           4:333MHz  fclk_3/2
 *           5:250MHz  fclk_4/2
 *           6:200MHz  fclk_2p5/2
 *           7:100MHz  fclk_2p5/8
 *           8:24MHz   oscin
 *           9:3MHz    oscin/3
 *     default:800MHz  fclk_2p5
 */

int set_dsp_clk(uint32_t id, uint32_t freq_sel)
{
	uint32_t control;
	uint32_t clk_sel;
	uint32_t clk_div;
	uint32_t addr = 0;
	char *dsp_clk_tag = NULL;
	int ret = 0;

	switch (id) {
	case 0:
		addr = CLKCTRL_DSPA_CLK_CTRL0;
		break;
#if defined(CONFIG_CLK_MESON_A1) || defined(CONFIG_CLK_MESON_C1) || \
		defined(CONFIG_CLK_MESON_T7) || defined(CONFIG_CLK_MESON_P1)
	case 1:
		addr = CLKCTRL_DSPB_CLK_CTRL0;
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
	printf("CLKCTRL_DSP_CLK_CTRL0 = %x\n", readl(addr));

	switch (freq_sel) {
#if defined(CONFIG_CLK_MESON_A1)
	case 0:
		clk_sel = 1;
		clk_div = 1;
		dsp_clk_tag = "fclk2/2:384MHz";
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
#elif defined(CONFIG_CLK_MESON_C1) || defined(CONFIG_CLK_MESON_C2)
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
	default:
		clk_sel = 3;
		clk_div = 0;
		dsp_clk_tag = "fclk5:400MHz (default)";
		break;
#else
	case 0:
		clk_sel = 1;
		clk_div = 0;
		dsp_clk_tag = "fclk2p5:800MHz";
		break;
	case 1:
		clk_sel = 2;
		clk_div = 0;
		dsp_clk_tag = "fclk/3:667MHz";
		break;
	case 2:
		clk_sel = 5;
		clk_div = 0;
		dsp_clk_tag = "fclk4:500MHz";
		break;
	case 3:
		clk_sel = 1;
		clk_div = 1;
		dsp_clk_tag = "fclk2p5/2:400MHz";
		break;
	case 4:
		clk_sel = 2;
		clk_div = 1;
		dsp_clk_tag = "fclk3/2:333MHz";
		break;
	case 5:
		clk_sel = 5;
		clk_div = 1;
		dsp_clk_tag = "fclk4/2:250MHz";
		break;
	case 6:
		clk_sel = 1;
		clk_div = 3;
		dsp_clk_tag = "fclk2p5/4:200MHz";
		break;
	case 7:
		clk_sel = 1;
		clk_div = 7;
		dsp_clk_tag = "fclk2p5/8:100MHz";
		break;
	case 8:
		clk_sel = 0;
		clk_div = 0;
		dsp_clk_tag = "oscin:24MHz";
		break;
	case 9:
		clk_sel = 0;
		clk_div = 7;
		dsp_clk_tag = "oscin/8:3MHz";
		break;
	default:
		clk_sel = 1;
		clk_div = 0;
		dsp_clk_tag = "fclk2p5:800MHz (default)";
		break;
#endif
	}

	printf("CLK_UTIL:dsp[%d]:%s\n", id, dsp_clk_tag);

	if (control & (1 << 15)) {   //if sync_mux ==1, sel mux 0
		control = (control & ~((1 << 15) | (0x3ff << 0) | (0x7 << 10))) |
			(1 << 13) | (1 << 29) | (clk_div << 0) | (clk_sel << 10);
	} else {
		control = (control & ~((1 << 15) | (0x3ff << 16) | (0x7 << 26))) |
			(1 << 13) | (1 << 29) | (clk_div << 16) | (clk_sel << 26) | (1 << 15);
	}

	writel(control, addr);

	if (id == 0)
		printf("CLKCTRL_DSPA_CLK_CTRL0 = %x\n", readl(addr));
	else
		printf("CLKCTRL_DSPB_CLK_CTRL0 = %x\n", readl(addr));

	return ret;
}

static int do_startdsp(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint32_t addr;
	uint32_t dspid;
	uint32_t freq_sel;
	uint32_t cfg0;
	uint32_t StatVectorSel;
	uint32_t strobe = 1;

	if (argc <= 1) {
		printf("please input dsp boot args: id, address, clk!\n");
		return CMD_RET_USAGE;
	}

	dspid = simple_strtoul(argv[1], NULL, 16);
	if (dspid > 1) {
		printf("start dsp failed: invalid id\n");
		return -1;
	}

	addr = simple_strtoul(argv[2], NULL, 16);
	freq_sel = simple_strtoul(argv[3], NULL, 16);

	printf("dsp%d boot\n", dspid);
	printf("dspboot start address: 0x%x\n", addr);
	printf("dsp clk num:%d\n", freq_sel);

	StatVectorSel = (addr != 0xfffa0000);
	cfg0 = 0x1 | StatVectorSel << 1 | strobe << 2;

#if defined(CONFIG_CLK_MESON_A1) || defined(CONFIG_CLK_MESON_C1) || \
		defined(CONFIG_CLK_MESON_T7) || defined(CONFIG_CLK_MESON_P1)
	power_set_dsp((dspid == 0) ? PM_DSPA : PM_DSPB, PWR_ON);
#else
	power_set_dsp(PM_DSPA, PWR_ON);
#endif
	udelay(100);

	if (set_dsp_clk(dspid, freq_sel))
		return -1;

	udelay(100);

	init_dsp(dspid, addr, cfg0);
	printf("dsp init over!\n");

	return 0;
}

U_BOOT_CMD(
	startdsp, 4, 1, do_startdsp,
	"load dspboot.bin from address",
	"\narg[0]: cmd\n"
	"arg[1]: dspid\n"
	"arg[2]: dspboot.bin load address!\n"
	"arg[3]: dsp clk set\nfor A1: 0:384M 1:308M 2:256M 3:24M\nfor C1 and C2: 0:400M 1:333M 2:250M 3:200M 4:100M 5:24M 6:3M\nfor others: 0:800M 1:667M 2:500M 3:400M 4:333M 5:250M 6:200M 7:100M 8:24M 9:3M\n"
);

