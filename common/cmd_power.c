// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <asm/arch/pwr_ctrl.h>
#include <command.h>

#ifndef PM_MAX
#define PM_MAX  0
static char *domain_name[PM_MAX];
#else
extern char *domain_name[];
#endif
//extern unsigned long pwr_ctrl_status_psci_smc(unsigned int power_domain);

static int do_powershow(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int i;
	int ret = 0;

	if (argc > 1)
		return CMD_RET_USAGE;

	if (PM_MAX  == 0) {
		printf("Don't support this feature now!\n");
		return ret;
	}

	for (i = 0; i < PM_MAX; i++)
		printf("%s[%d]:		%x\n", domain_name[i], i,
			(pwr_ctrl_status_psci_smc(i) ? 1 : 0));

	return ret;
}

U_BOOT_CMD(
	powershow, 1, 1, do_powershow,
	"show the power domain status , 0: on; 1: off",
	"\n arg[0]: cmd\n"
);

static int do_powerset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int domain_id;
	bool domain_status;
	int ret = 0;

	if (argc <= 1) {
		printf("plese input power set args: domain ID, domain status!\n");
		return CMD_RET_USAGE;
	}

	domain_id = simple_strtoul(argv[1], NULL, 10);
	domain_status = simple_strtoul(argv[2], NULL, 10);
	printf("domain_id: %d \n", domain_id);
	printf("domain_status: %d\n", domain_status);

	pwr_ctrl_psci_smc(domain_id, !domain_status);

	return ret;
}

U_BOOT_CMD(
	powerset, 3, 1, do_powerset,
	"power on/off a certain power domain",
	"\n arg[0]: cmd\n"
	"arg[1]: power domain ID \n"
	"arg[2]: power status to set, 0: on, 1: off\n"
);
