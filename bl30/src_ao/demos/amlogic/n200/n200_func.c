// See LICENSE for license details.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "riscv_encoding.h"
#include "n200_func.h"
#include "register.h"
#include "common.h"
#include "n200_timer.h"
#include "FreeRTOS.h"

    // Configure PMP to make all the address space accesable and executable
void pmp_open_all_space(void){
    // Config entry0 addr to all 1s to make the range cover all space
    asm volatile ("li x6, 0xffffffff":::"x6");
    asm volatile ("csrw pmpaddr0, x6":::);
    // Config entry0 cfg to make it NAPOT address mode, and R/W/X okay
    asm volatile ("li x6, 0x7f":::"x6");
    asm volatile ("csrw pmpcfg0, x6":::);
}

void switch_m2u_mode(void){
    clear_csr (mstatus,MSTATUS_MPP);
    //printf("\nIn the m2u function, the mstatus is 0x%x\n", read_csr(mstatus));
    //printf("\nIn the m2u function, the mepc is 0x%x\n", read_csr(mepc));
    asm volatile ("la x6, 1f    ":::"x6");
    asm volatile ("csrw mepc, x6":::);
    asm volatile ("mret":::);
    asm volatile ("1:":::);
}

uint32_t mtime_lo(void)
{
#ifdef configSOC_TIMER_AS_TICK
    return *(volatile uint32_t *)TIMERE_LOW_REG;
#else
    return *(volatile uint32_t *)(TIMER_CTRL_ADDR + TIMER_MTIME);
#endif
}


uint32_t mtime_hi(void)
{
#ifdef configSOC_TIMER_AS_TICK
    return *(volatile uint32_t *)TIMERE_HIG_REG;
#else
    return *(volatile uint32_t *)(TIMER_CTRL_ADDR + TIMER_MTIME + 4);
#endif
}

uint64_t get_timer_value(void)
{
  while (1) {
    uint32_t hi = mtime_hi();
    uint32_t lo = mtime_lo();
    if (hi == mtime_hi())
      return ((uint64_t)hi << 32) | lo;
  }
}

uint32_t get_timer_freq(void)
{
  return TIMER_FREQ;
}

uint64_t get_instret_value(void)
{
  while (1) {
    uint32_t hi = read_csr(minstreth);
    uint32_t lo = read_csr(minstret);
    if (hi == read_csr(minstreth))
      return ((uint64_t)hi << 32) | lo;
  }
}

uint64_t get_cycle_value(void)
{
  while (1) {
    uint32_t hi = read_csr(mcycleh);
    uint32_t lo = read_csr(mcycle);
    if (hi == read_csr(mcycleh))
      return ((uint64_t)hi << 32) | lo;
  }
}

unsigned long interrupt_status_get(void)
{
	return read_csr(mstatus) >> 0x3;
}

void interrupt_disable(void)
{
	clear_csr(mstatus, MSTATUS_MIE);
}

void interrupt_enable(void)
{
	set_csr(mstatus, MSTATUS_MIE);
}

#ifndef N200_REVA

uint32_t __attribute__((noinline)) measure_cpu_freq(size_t n)
{
  uint32_t start_mtime, delta_mtime;
  uint32_t mtime_freq = get_timer_freq();

  // Don't start measuring until we see an mtime tick
  uint32_t tmp = mtime_lo();
  do {
    start_mtime = mtime_lo();
  } while (start_mtime == tmp);

  uint32_t start_mcycle = read_csr(mcycle);

  do {
    delta_mtime = mtime_lo() - start_mtime;
  } while (delta_mtime < n);

  uint32_t delta_mcycle = read_csr(mcycle) - start_mcycle;

  return (delta_mcycle / delta_mtime) * mtime_freq
         + ((delta_mcycle % delta_mtime) * mtime_freq) / delta_mtime;
}

uint32_t get_cpu_freq(void)
{
  uint32_t cpu_freq;

  // warm up
  measure_cpu_freq(1);
  // measure for real
  cpu_freq = measure_cpu_freq(100);

  return cpu_freq;
}

unsigned int xPortIsIsrContext(void)
{
	return (read_csr_msubmode & 0xff);
}

#else

unsigned int xPortIsIsrContext(void)
{
	return read_csr_msubmode;
}

#endif
