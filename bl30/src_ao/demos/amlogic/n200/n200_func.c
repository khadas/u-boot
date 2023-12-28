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
#include "lscript.h"

#define PMP_PRINT(...) printf(__VA_ARGS__)
#define PMP_PRINT_DEBUG(...) //printf(__VA_ARGS__)

/*
 * -- Configure PMP --
 * For User mode only.
 * To make all the address space accessible and executable
 */
void pmp_open_all_space(void){
	// Config entry0 addr to all 1s to make the range cover all space
	asm volatile ("li x6, 0xffffffff":::"x6");
	asm volatile ("csrw pmpaddr0, x6":::);
	// Config entry0 cfg to make it NAPOT address mode, and R/W/X okay
	asm volatile ("li x6, 0x7f":::"x6");
	asm volatile ("csrw pmpcfg0, x6":::);
}

#ifndef N200_REVA
/*
 * -- Configure PMP --
 * For both Machine and User mode.
 * To make aocpu text/ro data space readable and executable.
 * To make aocpu data space readable and writable.
 * To make 4G space readable and writable.
 * Note:
 * 	If config_pmp is called, pmp_open_all_space is invalid.
 */
uint32_t config_pmp(void)
{
	uint32_t start_text_addr = (uint32_t)&_text;
	uint32_t end_text_addr = (uint32_t)&_etext;
	uint32_t text_len_left = end_text_addr - start_text_addr;
	uint32_t next_seg_len;
	uint32_t pmp_cfg[8] = {0};
	int pmp_entry_used = 0;
	uint32_t pmp_region_base = start_text_addr;
	uint32_t ao_ram_end = configMEM_START + configMEM_LEN;
	uint32_t all_region_size = 0x1fffffff;

	PMP_PRINT("AOCPU: configure PMP for memory 0x%lx ~ 0x%lx\n",
			start_text_addr, end_text_addr);

	while (text_len_left > 0) {
		for (next_seg_len = 1024; next_seg_len*2 <= text_len_left; next_seg_len *= 2)
			;

		text_len_left -= next_seg_len;
		PMP_PRINT_DEBUG("segment length: %lx\n", next_seg_len);
		PMP_PRINT_DEBUG("length left   : %lx\n", text_len_left);

		/* Configure text/ro data space access privilege */
		switch (pmp_entry_used) {
		case 0:
			write_csr(pmpaddr0, (pmp_region_base >> 2) | NAPOT_SIZE(next_seg_len));
			pmp_cfg[0] = (PMP_CFG_R_EN | PMP_CFG_X_EN | PMP_CFG_A_NAPOT | PMP_CFG_L_EN);
			break;
		case 1:
			write_csr(pmpaddr1, (pmp_region_base >> 2) | NAPOT_SIZE(next_seg_len));
			pmp_cfg[1] = (PMP_CFG_R_EN | PMP_CFG_X_EN | PMP_CFG_A_NAPOT | PMP_CFG_L_EN);
			break;
		case 2:
			write_csr(pmpaddr2, (pmp_region_base >> 2) | NAPOT_SIZE(next_seg_len));
			pmp_cfg[2] = (PMP_CFG_R_EN | PMP_CFG_X_EN | PMP_CFG_A_NAPOT | PMP_CFG_L_EN);
			break;
		case 3:
			write_csr(pmpaddr3, (pmp_region_base >> 2) | NAPOT_SIZE(next_seg_len));
			pmp_cfg[3] = (PMP_CFG_R_EN | PMP_CFG_X_EN | PMP_CFG_A_NAPOT | PMP_CFG_L_EN);
			break;
		case 4:
			write_csr(pmpaddr4, (pmp_region_base >> 2) | NAPOT_SIZE(next_seg_len));
			pmp_cfg[4] = (PMP_CFG_R_EN | PMP_CFG_X_EN | PMP_CFG_A_NAPOT | PMP_CFG_L_EN);
			break;
		case 5:
			write_csr(pmpaddr5, (pmp_region_base >> 2) | NAPOT_SIZE(next_seg_len));
			pmp_cfg[5] = (PMP_CFG_R_EN | PMP_CFG_X_EN | PMP_CFG_A_NAPOT | PMP_CFG_L_EN);
			break;
		default:
			break;
		}
		pmp_entry_used++;
		pmp_region_base += next_seg_len;
		PMP_PRINT_DEBUG("pmp_entry_used : %d\n", pmp_entry_used);
		PMP_PRINT_DEBUG("pmp_region_base: %lx\n", pmp_region_base);
	}

	/* Configure data space access privilege */
	write_csr(pmpaddr6, (pmp_region_base >> 2) | NAPOT_SIZE(ao_ram_end - pmp_region_base));
	pmp_cfg[6] = (PMP_CFG_R_EN | PMP_CFG_W_EN | PMP_CFG_A_NAPOT | PMP_CFG_L_EN);
	/* Configure all 32-bit (4G) address space access privilege */
	write_csr(pmpaddr7, (0x0) | all_region_size);
	pmp_cfg[7] = (PMP_CFG_R_EN | PMP_CFG_W_EN | PMP_CFG_A_NAPOT | PMP_CFG_L_EN);
	/* Set Configuration for text/ro data/data/4G space */
	write_csr(pmpcfg0, (pmp_cfg[3] << 24) | (pmp_cfg[2] << 16) |
				(pmp_cfg[1] << 8) | (pmp_cfg[0] << 0));
	write_csr(pmpcfg1, (pmp_cfg[7] << 24) | (pmp_cfg[6] << 16) |
				(pmp_cfg[5] << 8) | (pmp_cfg[4] << 0));

	PMP_PRINT_DEBUG("pmpaddr0 : %lx\n", read_csr(pmpaddr0));
	PMP_PRINT_DEBUG("pmpaddr1 : %lx\n", read_csr(pmpaddr1));
	PMP_PRINT_DEBUG("pmpaddr2 : %lx\n", read_csr(pmpaddr2));
	PMP_PRINT_DEBUG("pmpaddr3 : %lx\n", read_csr(pmpaddr3));
	PMP_PRINT_DEBUG("pmpaddr4 : %lx\n", read_csr(pmpaddr4));
	PMP_PRINT_DEBUG("pmpaddr5 : %lx\n", read_csr(pmpaddr5));
	PMP_PRINT_DEBUG("pmpaddr6 : %lx\n", read_csr(pmpaddr6));
	PMP_PRINT_DEBUG("pmpaddr7 : %lx\n", read_csr(pmpaddr7));
	PMP_PRINT_DEBUG("pmpcfg0  : %lx\n", read_csr(pmpcfg0));
	PMP_PRINT_DEBUG("pmpcfg1  : %lx\n", read_csr(pmpcfg1));
	PMP_PRINT("AOCPU: configure PMP end\n");

	return 0;
}
#endif

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
