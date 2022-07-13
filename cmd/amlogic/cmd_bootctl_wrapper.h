/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __CMD_WRAPPER_H
#define __CMD_WRAPPER_H

typedef int (*bootctl_cmd_func)(cmd_tbl_t *cmdtp,
				int flag,
				int argc,
				char * const argv[]);

typedef struct _bootctl_func_handles {
	bootctl_cmd_func do_GetValidSlot_func;
	bootctl_cmd_func do_SetActiveSlot_func;
	bootctl_cmd_func do_SetRollFlag_func;
	bootctl_cmd_func do_CopySlot_func;
	bootctl_cmd_func do_SetUpdateTries_func;
	bootctl_cmd_func do_GetSystemMode_func;
	bootctl_cmd_func do_GetAvbMode_func;
} bootctl_func_handles;

bootctl_func_handles *get_bootctl_cmd_func_vab(void);
bootctl_func_handles *get_bootctl_cmd_func_avb(void);
bootctl_func_handles *get_bootctl_cmd_func(void);

#endif
