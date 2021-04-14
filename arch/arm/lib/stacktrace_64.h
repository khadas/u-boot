#ifndef _STACKTRACE_64_H_
#define _STACKTRACE_64_H_

#include <common.h>

int dump_backtrace(struct pt_regs *regs);

#endif
