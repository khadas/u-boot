/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/cmd_startdsp.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <command.h>
#include <asm/arch/cpu.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/timer.h>
#include <asm/arch/bl31_apis.h>
#include <asm/arch/pwr_ctrl.h>
#include <serial.h>
#include <asm/arch-p1/register.h>

#define Wr(addr, data) *(volatile uint32_t *)(addr)=(data)
#define Rd(addr) *(volatile uint32_t *)(addr)

//	 .clk0		(cts_oscin_clk	  ),
//	 .clk1		(fclk_div2p5		),
//	 .clk2		(fclk_div3		  ),
//	 .clk3		(m4_pll_clk		 ),
//	 .clk4		(hifi_pll_clk	   ),
//	 .clk5		(fclk_div4		  ),
//	 .clk6		(gp2_pll_clk		),
//	 .clk7		(cts_rtc_clk		),
void set_m4_clk(int sel, int div)
{
	uint32_t reg_val = 0;

	reg_val = Rd(CLKCTRL_M4_CLK_CTRL);
	// enable osc_clk firstly
	if (reg_val == 0) {
		reg_val = (1 << 13);
		Wr(CLKCTRL_M4_CLK_CTRL, reg_val);	/* pre_a clock gate en */
	}

	reg_val = Rd(CLKCTRL_M4_CLK_CTRL);
	if (reg_val & (1 << 15)) {
		/* Pre_a source select M4_PLL, pre_a clock gate en,	 Pre_a div5	 */
		reg_val = (reg_val & ~((0x7 << 10) | (1 << 13) | (0x3ff << 0)))
			  | ((sel << 10) | (0 << 13) | (div << 0));
		Wr(CLKCTRL_M4_CLK_CTRL, reg_val);
		reg_val = (reg_val & ~((0x7 << 10) | (1 << 13) | (0x3ff << 0)))
			   | ((sel << 10) | (1 << 13) | (div << 0));
		Wr(CLKCTRL_M4_CLK_CTRL, reg_val);
		/* select pre_a */
		reg_val = Rd(CLKCTRL_M4_CLK_CTRL);
		reg_val &=	~(1 << 15);
		Wr(CLKCTRL_M4_CLK_CTRL, reg_val);
	} else {
		/* Pre_b source select M4_PLL, pre_b clock gate en,	 Pre_b div5	 */
		reg_val = (reg_val & ~((0x7 << 26) | (1 << 29) | (0x3ff << 16)))
			   | ((sel << 26) | (0 << 29) | (div << 16));
		Wr(CLKCTRL_M4_CLK_CTRL, reg_val);
		reg_val = (reg_val & ~((0x7 << 26) | (1 << 29) | (0x3ff << 16)))
			   | ((sel << 26) | (1 << 29) | (div << 16));
		Wr(CLKCTRL_M4_CLK_CTRL, reg_val);
		/* select pre_b */
		reg_val = Rd(CLKCTRL_M4_CLK_CTRL);
		reg_val |= (1 << 15);
		Wr(CLKCTRL_M4_CLK_CTRL, reg_val);
	}
}

void set_m4_pll_clk(uint32_t freq)
{
	uint32_t M, N, OD = 0;
	uint32_t reg_cntl0;

	for (int i = 0; i < 6; i++) {
		if ((freq << i) >= 1600 && (freq << i) < 3200) {
			OD = i;
			break;
		}
	}

	M = ((freq + 12) << OD) / 24;
	N = 1;
	reg_cntl0 = (OD << 12) | (N << 16) | M;

	Wr(CLKCTRL_M4PLL_CTRL0, (0x2 << 28) | reg_cntl0); // reset pll
	Wr(CLKCTRL_M4PLL_CTRL0, (0x3 << 28) | reg_cntl0); // reset + en
	Wr(CLKCTRL_M4PLL_CTRL1, 0x1420500f);
	Wr(CLKCTRL_M4PLL_CTRL2, 0x00023041);
	Wr(CLKCTRL_M4PLL_CTRL3, 0x00000000);
	Wr(CLKCTRL_M4PLL_CTRL0, (0x1 << 28) | reg_cntl0); // release reset
	Wr(CLKCTRL_M4PLL_CTRL2, 0x00023001);

	while ((Rd(CLKCTRL_M4PLL_STS) & 0x80000000) == 0)
		;


}
static int do_startm4(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint32_t cpu_id;
	uint32_t bin_addr;
	uint32_t bank_config;
	uint32_t cmd;

	int ret = 0;

	if (argc != 5) {
		printf("Plese input M4 boot args:cpu_id, bin_addr!\n");
		return CMD_RET_USAGE;
	}

	cpu_id = simple_strtoul(argv[1], NULL, 16);
	bin_addr = simple_strtoul(argv[2], NULL, 16);
	bank_config = simple_strtoul(argv[3], NULL, 16);
	cmd = simple_strtoul(argv[4], NULL, 16);

	if (bank_config == 0u) {
		/*default:M4A_ROM=M4A_RAM=M4B_ROM=M4B_RAM=128K */
		bank_config = 0xFCB87430u;
	}

	if (cpu_id >= 2) {
		printf("\nPlese input M4 boot cpu_id less than 2!");
		return CMD_RET_USAGE;
	}

	if (bin_addr & 0x3u) {
		printf("Plese load the bin in ddr address in word(4 bytes) align.\n");
		return CMD_RET_USAGE;
	}

	if (cpu_id == 0) {
		printf("M4A is boot, cpu_id:%d\n", cpu_id);
		printf("M4A bin address:0x%x\n", bin_addr);
	} else {
		printf("M4B is boot, cpu_id:%d\n", cpu_id);
		printf("M4B bin address:0x%x\n", bin_addr);
	}

	if (cmd == 1) {
		/*hold reset M4 */
		start_m4(cpu_id, bin_addr, bank_config, 3);

		if (cpu_id == 0) {
			/*repower on cpu */
			pwr_ctrl_psci_smc(PM_M4A, 1);
		} else {
			/*repower on cpu */
			pwr_ctrl_psci_smc(PM_M4B, 1);
		}

		/* 300MHz */
		set_m4_clk(3u, 4u);
		set_m4_pll_clk(1500);
		start_m4(cpu_id, bin_addr, bank_config, cmd);
	} else {
		start_m4(cpu_id, bin_addr, bank_config, cmd);
	}

	return ret;
}

U_BOOT_CMD(
	startm4, 5, 1, do_startm4,
	"load bin from DDR memory address ",
	"\narg[0]: cmd\n"
	"arg[1]: cpu_id, the cpu id 0 is start M4A, the cpu id 1 is start M4B\n"
	"arg[2]: bin_ddr, load execution bin from ddr memory address\n"
	"arg[3]: bank config, default:0xFCB87430u\n"
	"arg[4]: cmd, 1--START, 2--STOP, 3--RESET\n"
);

