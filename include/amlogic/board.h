// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

void board_init_mem(void);

int aml_board_late_init_front(void *arg);

int aml_board_late_init_tail(void *arg);

void board_set_boot_source(void);
