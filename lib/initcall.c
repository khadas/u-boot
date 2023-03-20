/*
 * Copyright (c) 2013 The Chromium OS Authors.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <initcall.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_AMLOGIC_TIME_PROFILE
static void record_init_call_time(void *func, unsigned int time)
{
	int i;

	for (i = 0; i < INITCALL_CNT; i++) {
		if (!gd->ict[i].func) {
			gd->ict[i].func = func;
			gd->ict[i].time = time;
			return;
		}
	}
	printf("Lost init call %p, time:%d\n", func, time);
}

void dump_initcall_time(void)
{
	int i;
	const char *sym;
	unsigned long base;
	unsigned long end;

	for (i = 0; i < INITCALL_CNT; i++) {
		if (gd->ict[i].func && gd->ict[i].time > 1000) {
			sym = symbol_lookup((unsigned long)gd->ict[i].func,
					    &base, &end);
			if (sym)
				printf("init call time:%8d, func:%s\n",
					gd->ict[i].time, sym);
			else
				printf("init call time:%8d, func:%p\n",
					gd->ict[i].time, gd->ict[i].func);
		}
	}
}
#endif

int initcall_run_list(const init_fnc_t init_sequence[])
{
	const init_fnc_t *init_fnc_ptr;
#ifdef CONFIG_AMLOGIC_TIME_PROFILE
	int time;
#endif

	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) {
		unsigned long reloc_ofs = 0;
		int ret;

		if (gd->flags & GD_FLG_RELOC)
			reloc_ofs = gd->reloc_off;
		debug("initcall: %p", (char *)*init_fnc_ptr - reloc_ofs);
		if (gd->flags & GD_FLG_RELOC)
			debug(" (relocated to %p)\n", (char *)*init_fnc_ptr);
		else
			debug("\n");
	#ifdef CONFIG_AMLOGIC_TIME_PROFILE
		time = get_time();
	#endif
		ret = (*init_fnc_ptr)();
	#ifdef CONFIG_AMLOGIC_TIME_PROFILE
		time = get_time() - time;
		record_init_call_time((char *)*init_fnc_ptr - reloc_ofs, time);
	#endif
		if (ret) {
			printf("initcall sequence %p failed at call %p (err=%d)\n",
			       init_sequence,
			       (char *)*init_fnc_ptr - reloc_ofs, ret);
			return -1;
		}
	}
	return 0;
}
