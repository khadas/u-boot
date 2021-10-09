#ifndef INTERRUPT_CONTROL_H_
#define INTERRUPT_CONTROL_H_

#include "n200_eclic.h"
#include "interrupt.h"
///////////////////////////////////////////////////////////////////
/////// ECLIC relevant functions
///////
void eclic_init ( uint32_t num_irq );

void eclic_enable_interrupt (uint32_t source);
void eclic_disable_interrupt (uint32_t source);

void eclic_set_pending(uint32_t source);
void eclic_clear_pending(uint32_t source);

void eclic_set_intctrl (uint32_t source, uint8_t intctrl);
uint8_t eclic_get_intctrl  (uint32_t source);

void eclic_set_intattr (uint32_t source, uint8_t intattr);
uint8_t eclic_get_intattr  (uint32_t source);

void eclic_set_cliccfg (uint8_t cliccfg);
uint8_t eclic_get_cliccfg (void);

void eclic_set_mth (uint8_t mth);
uint8_t eclic_get_mth(void);

//sets nlbits
void eclic_set_nlbits(uint8_t nlbits);


//get nlbits
uint8_t eclic_get_nlbits(void);

void eclic_set_irq_lvl(uint32_t source, uint8_t lvl);
uint8_t eclic_get_irq_lvl(uint32_t source);

void eclic_set_irq_lvl_abs(uint32_t source, uint8_t lvl_abs);
uint8_t eclic_get_irq_lvl_abs(uint32_t source);

void eclic_set_irq_pri(uint32_t source, uint8_t pri);

void eclic_mode_enable(void);

void eclic_set_vmode(uint32_t source);
void eclic_set_nonvmode(uint32_t source);

void eclic_set_level_trig(uint32_t source);
void eclic_set_posedge_trig(uint32_t source);
void eclic_set_negedge_trig(uint32_t source);

int eclic_map_interrupt(uint32_t ulIrq, uint32_t src);

void print_eclic(void);

void clean_int_src(void);
int int_src_sel(uint32_t ulIrq, uint32_t src);
int int_src_clean(uint32_t ulIrq);

//typedef void (*function_ptr_t) (void);

#endif
