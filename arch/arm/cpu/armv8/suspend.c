// SPDX-License-Identifier: GPL-2.0

#include <asm/types.h>
#include <asm/suspend.h>
#include <linux/errno.h>
#include <stdio.h>
#include <asm/arch/bl31_apis.h>

/*
 * This is allocated by cpu_suspend_init(), and used to store a pointer to
 * the 'struct sleep_stack_data' the contains a particular CPUs state.
 */

unsigned long sleep_save_stash;

/*
 * cpu_suspend
 *
 * arg: argument to pass to the finisher function
 * fn: finisher function pointer
 *
 */
int cpu_suspend(unsigned long arg)
{
	int ret = 0;

	struct sleep_stack_data state;

	printf("enter %s %d\n", __func__, __LINE__);

	if (__cpu_suspend_enter(&state)) {
		printf("enter %s %d\n", __func__, __LINE__);
		/* Call the suspend finisher */
		aml_reboot(0xc400000e, (uint64_t)cpu_resume, 0, 0);

		/*
		 * Never gets here, unless the suspend finisher fails.
		 * Successful cpu_suspend() should return from cpu_resume(),
		 * returning through this code path is considered an error
		 * If the return value is set to 0 force ret = -EOPNOTSUPP
		 * to make sure a proper error condition is propagated
		 */
		if (!ret)
			ret = -EOPNOTSUPP;
	}

	return ret;
}
