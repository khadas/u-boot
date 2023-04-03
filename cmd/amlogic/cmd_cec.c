// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <asm/arch/io.h>
#include <asm/arch/clock.h>
#include <amlogic/aml_cec.h>
#include <asm/arch/secure_apb.h>

static void cec_init(int logic_addr, unsigned char fun_cfg)
{
#if(CONFIG_MESON_T5W == 1 || CONFIG_MESON_TXHD2 == 1)
	printf("%s :%d,%#x\n", __func__, logic_addr, fun_cfg);
	/*cec_hw_init(logic_addr, fun_cfg);*/
	writel(fun_cfg, P_AO_DEBUG_REG0);
	writel(logic_addr, AO_DEBUG_REG1);
	printf("cec func:%#x, parm:%#x\n", readl(P_AO_DEBUG_REG0),
		readl(AO_DEBUG_REG1));
#else
	unsigned int tmp;

	writel(fun_cfg, SYSCTRL_STATUS_REG0);
	tmp = readl(SYSCTRL_STATUS_REG1);
	tmp &= (~0xf0000);
	tmp |= ((logic_addr & 0xf) << 16);
	writel(tmp, SYSCTRL_STATUS_REG1);
	printf("cec func:%#x, parm:%#x\n", readl(SYSCTRL_STATUS_REG0),
		readl(SYSCTRL_STATUS_REG1));
	printf("%s :%d,%#x\n", __func__, logic_addr, fun_cfg);
	/*cec_hw_init(logic_addr, fun_cfg);*/
	writel(fun_cfg, P_AO_DEBUG_REG0);
	writel(logic_addr, AO_DEBUG_REG1);
	printf("cec func:%#x, parm:%#x\n", readl(P_AO_DEBUG_REG0),
		readl(AO_DEBUG_REG1));
#endif
}

static int do_cec(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int logic_addr = 0;
	int fun_cfg = 0x2f;

	if (argc >=  2)
		logic_addr = simple_strtoul(argv[1], NULL, 10);
	if (argc >=  3)
		fun_cfg = simple_strtoul(argv[2], NULL, 16);

	printf("logic_addr=0x%x, fun_cfg=0x%x\n",
		logic_addr, fun_cfg);

	cec_init(logic_addr, (unsigned char)fun_cfg);

	return 0;
}

U_BOOT_CMD(cec, CONFIG_SYS_MAXARGS, 0, do_cec,
		"Amlogic cec",
		"	- hdmi cec function\n"
		"	- param: logic addr;fun_cfg\n"
		"\n"
);

