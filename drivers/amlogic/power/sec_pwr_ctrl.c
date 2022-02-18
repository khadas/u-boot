// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <linux/arm-smccc.h>
#include <asm/arch/pwr_ctrl.h>

unsigned long pwr_ctrl_psci_smc(unsigned int power_domain, bool power_control)
{
	struct arm_smccc_res res;

	arm_smccc_smc(0x82000093, power_domain, power_control, 0,
		      0, 0, 0, 0, &res);
	return res.a0;
}

unsigned long pwr_ctrl_status_psci_smc(unsigned int power_domain)
{
	struct arm_smccc_res res;

	arm_smccc_smc(0x82000095, power_domain, 0, 0,
		      0, 0, 0, 0, &res);
	return res.a0;
}

unsigned long viu_init_psci_smc(void)
{
	struct arm_smccc_res res;

	arm_smccc_smc(0x82000080, 0, 0, 0,
		      0, 0, 0, 0, &res);
	return res.a0;
}

