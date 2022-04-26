/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _STACKTRACE_64_H_
#define _STACKTRACE_64_H_

#include <common.h>

int dump_backtrace(struct pt_regs *regs);

#endif
