// See LICENSE file for licence details

#ifndef INTERRUPT_H
#define INTERRUPT_H

typedef void (*function_ptr_t) (void);

void wfe(void);

int EnableIrq(uint32_t ulIrq);

int DisableIrq(uint32_t ulIrq);

int SetIrqPriority(uint32_t ulIrq, uint32_t ulProi);

int ClearPendingIrq(uint32_t ulIrq);

int RegisterIrq(uint32_t int_num, uint32_t int_priority, function_ptr_t handler);

int UnRegisterIrq(uint32_t ulIrq);

unsigned long interrupt_status_get(void);

void interrupt_disable(void);

void interrupt_enable(void);

#endif
