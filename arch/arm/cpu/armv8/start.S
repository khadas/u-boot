/*
 * (C) Copyright 2013
 * David Feng <fenghua@phytium.com.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm-offsets.h>
#include <config.h>
#include <version.h>
#include <linux/linkage.h>
#include <asm/macro.h>
#include <asm/armv8/mmu.h>
#include <asm/system.h>

/*************************************************************************
 *
 * Startup Code (reset vector)
 *
 *************************************************************************/

.globl	_start
_start:
	b	reset

	.align 3

.globl	_TEXT_BASE
_TEXT_BASE:
	.quad	CONFIG_SYS_TEXT_BASE

/*
 * These are defined in the linker script.
 */
.globl	_end_ofs
_end_ofs:
	.quad	_end - _start

.globl	_bss_start_ofs
_bss_start_ofs:
	.quad	__bss_start - _start

.globl	_bss_end_ofs
_bss_end_ofs:
	.quad	__bss_end - _start

reset:
	/*
	 * Could be EL3/EL2/EL1, Initial State:
	 * Little Endian, MMU Disabled, i/dCache Disabled
	 */
	adr	x0, vectors
	switch_el x1, 3f, 2f, 1f
3:	msr	vbar_el3, x0
	mrs	x0, scr_el3
	orr	x0, x0, #0xf			/* SCR_EL3.NS|IRQ|FIQ|EA */
	msr	scr_el3, x0
	msr	cptr_el3, xzr			/* Enable FP/SIMD */
	ldr	x0, =COUNTER_FREQUENCY
	msr	cntfrq_el0, x0			/* Initialize CNTFRQ */
	b	0f
2:	msr	vbar_el2, x0
	mov	x0, #0x33ff
	msr	cptr_el2, x0			/* Enable FP/SIMD */
	b	0f
1:	msr	vbar_el1, x0
	mov	x0, #3 << 20
	msr	cpacr_el1, x0			/* Enable FP/SIMD */
0:

	/*
	 * Cache/BPB/TLB Invalidate
	 * i-cache is invalidated before enabled in icache_enable()
	 * tlb is invalidated before mmu is enabled in dcache_enable()
	 * d-cache is invalidated before enabled in dcache_enable()
	 */

	/* Processor specific initialization */
	bl	lowlevel_init

	branch_if_master x0, x1, master_cpu

	/*
	 * Slave CPUs
	 */
#ifdef CONFIG_CMD_AML_MTEST
.extern get_gd_addr
.extern get_stack_base
.extern enable_caches
.extern get_core_entry_fn
.extern mmu_setup
#endif
slave_cpu:
#ifdef CONFIG_CMD_AML_MTEST
	bl get_gd_addr
	mov x18, x0

	mrs x0, mpidr_el1
	ubfx x1, x0, #24, 1
	cbz x1, slave_cpu_next
	lsr x0, x0, #8
slave_cpu_next:
	and	x1, x0, #0xff
	and	x0, x0, #0xff00
	add	x0, x1, x0, LSR #6

	bl get_stack_base
	mov sp, x0

	//bl enable_caches
	bl __asm_invalidate_icache_all
	switch_el x1, i3f, i2f, i1f
i3f:
	mrs x0, sctlr_el3
	orr x0, x0, #CR_I
	msr sctlr_el3, x0
	isb
	b i0f
i2f:
	mrs x0, sctlr_el2
	orr x0, x0, #CR_I
	msr sctlr_el2, x0
	isb
	b i0f
i1f:
	mrs x0, sctlr_el1
	orr x0, x0, #CR_I
	msr sctlr_el1, x0
	isb
i0f:

	bl __asm_invalidate_dcache_all
	bl __asm_invalidate_tlb_all
	bl mmu_setup
	switch_el x1, d3f, d2f, d1f
d3f:
	mrs x0, sctlr_el3
	orr x0, x0, #CR_C
	msr sctlr_el3, x0
	isb
	b d0f
d2f:
	mrs x0, sctlr_el2
	orr x0, x0, CR_C
	msr sctlr_el2, x0
	isb
	b d0f
d1f:
	mrs x0, sctlr_el1
	orr x0, x0, CR_C
	msr sctlr_el1, x0
	isb
d0f:

	mrs x0, mpidr_el1
	ubfx x1, x0, #24, 1
	cbz x1, slave_cpu_next1
	lsr x0, x0, #8
slave_cpu_next1:
	and	x1, x0, #0xff
	and	x0, x0, #0xff00
	add	x0, x1, x0, LSR #6
	bl get_core_entry_fn
	mov x2, x0
	mrs x0, mpidr_el1
	ubfx x1, x0, #24, 1
	cbz x1, slave_cpu_next2
	lsr x0, x0, #8
slave_cpu_next2:
	and	x1, x0, #0xff
	and	x0, x0, #0xff00
	add	x0, x1, x0, LSR #6
	br x2

	b .
#endif
	wfe
	ldr	x1, =CPU_RELEASE_ADDR
	ldr	x0, [x1]
	cbz	x0, slave_cpu
	br	x0			/* branch to the given address */

	/*
	 * Master CPU
	 */
master_cpu:
	bl	_main

/*-----------------------------------------------------------------------*/

WEAK(lowlevel_init)
	mov	x29, lr			/* Save LR */

#if defined(CONFIG_GICV2) || defined(CONFIG_GICV3)
	branch_if_slave x0, 1f
	ldr	x0, =GICD_BASE
	bl	gic_init_secure
1:
#if defined(CONFIG_GICV3)
	ldr	x0, =GICR_BASE
	bl	gic_init_secure_percpu
#elif defined(CONFIG_GICV2)
	ldr	x0, =GICD_BASE
	ldr	x1, =GICC_BASE
	bl	gic_init_secure_percpu
#endif
#endif

	branch_if_master x0, x1, 2f

	/*
	 * Slave should wait for master clearing spin table.
	 * This sync prevent salves observing incorrect
	 * value of spin table and jumping to wrong place.
	 */
#if defined(CONFIG_GICV2) || defined(CONFIG_GICV3)
#ifdef CONFIG_GICV2
	ldr	x0, =GICC_BASE
#endif
	bl	gic_wait_for_interrupt
#endif

	/*
	 * All slaves will enter EL2 and optionally EL1.
	 */
	bl	armv8_switch_to_el2
#ifdef CONFIG_ARMV8_SWITCH_TO_EL1
	bl	armv8_switch_to_el1
#endif

2:
	mov	lr, x29			/* Restore LR */
	ret
ENDPROC(lowlevel_init)

WEAK(smp_kick_all_cpus)
	/* Kick secondary cpus up by SGI 0 interrupt */
	mov	x29, lr			/* Save LR */
#if defined(CONFIG_GICV2) || defined(CONFIG_GICV3)
	ldr	x0, =GICD_BASE
	bl	gic_kick_secondary_cpus
#endif
	mov	lr, x29			/* Restore LR */
	ret
ENDPROC(smp_kick_all_cpus)

/*-----------------------------------------------------------------------*/

ENTRY(c_runtime_cpu_setup)
	/* Relocate vBAR */
	adr	x0, vectors
	switch_el x1, 3f, 2f, 1f
3:	msr	vbar_el3, x0
	b	0f
2:	msr	vbar_el2, x0
	b	0f
1:	msr	vbar_el1, x0
0:

	ret
ENDPROC(c_runtime_cpu_setup)
