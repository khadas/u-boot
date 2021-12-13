// See LICENSE file for licence details

#ifndef N200_FUNC_H
#define N200_FUNC_H

__BEGIN_DECLS

#include "n200_timer.h"
#include "interrupt_control.h"

void pmp_open_all_space(void);

void switch_m2u_mode(void);

uint32_t get_mtime_freq(void);

uint32_t mtime_lo(void);

uint32_t mtime_hi(void);

uint64_t get_mtime_value(void);

uint64_t get_instret_value(void);

uint64_t get_cycle_value(void);

uint32_t get_cpu_freq(void);

uint32_t __attribute__((noinline)) measure_cpu_freq(size_t n);

void wfe(void);

uint64_t get_timer_value(void);

uint32_t get_timer_freq(void);

void test_handler(void);

int EnableIrq(uint32_t ulIrq);

int DisableIrq(uint32_t ulIrq);

int SetIrqPriority(uint32_t ulIrq, uint32_t ulProi);

int ClearPendingIrq(uint32_t ulIrq);

int RegisterIrq(uint32_t int_num, uint32_t int_priority, function_ptr_t handler);

int UnRegisterIrq(uint32_t ulIrq);

unsigned long interrupt_status_get(void);

void interrupt_disable(void);

void interrupt_enable(void);

unsigned int xPortIsIsrContext(void);

__END_DECLS

#endif
