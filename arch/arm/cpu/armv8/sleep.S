/* SPDX-License-Identifier: GPL-2.0 */

#include <linux/errno.h>
#include <linux/linkage.h>
#include <asm-offsets.h>

	.macro disable_daif
	msr	daifset, #0xf
	.endm

	.text


/**
 * cpu_do_suspend - save CPU registers context
 *
 * x0: virtual address of context pointer
 *
 * This must be kept in sync with struct cpu_suspend_ctx in <asm/suspend.h>.
 */
SYM_FUNC_START(cpu_do_suspend)

	mrs	x2, sctlr_el2
	mrs	x3, tcr_el2
	mrs	x4, vbar_el2
	mrs	x6, cpacr_el1

	mrs	x7, tcr_el1
	mrs	x8, vbar_el1
	mrs	x9, mdscr_el1
	mrs	x10, oslsr_el1
	mrs	x11, sctlr_el1
	mrs	x12, tpidr_el2
	mrs	x13, sp_el1

	stp	x2, x3, [x0]
	stp	x4, x5, [x0, #16]
	stp	x6, x7, [x0, #32]
	stp	x8, x9, [x0, #48]
	stp	x10, x11, [x0, #64]
	stp	x12, x13, [x0, #80]

	/*
	 * Save x18 as it may be used as a platform register, e.g. by shadow
	 * call stack.
	 */
	str	x18, [x0, #96]
	ret
SYM_FUNC_END(cpu_do_suspend)

/**
 * cpu_do_resume - restore CPU register context
 *
 * x0: Address of context pointer
 */

SYM_FUNC_START(cpu_do_resume)

	ldp	x2, x3, [x0]
	ldp	x4, x5, [x0, #16]
	ldp	x6, x8, [x0, #32]
	ldp	x9, x10, [x0, #48]
	ldp	x11, x12, [x0, #64]
	ldp	x13, x14, [x0, #80]

	/*
	 * Restore x18, as it may be used as a platform register, and clear
	 * the buffer to minimize the risk of exposure when used for shadow
	 * call stack.
	 */
	ldr	x18, [x0, #96]
	str	xzr, [x0, #96]

	msr	tcr_el2, x3
	msr	vbar_el2, x4
	msr	cpacr_el1, x6

	/* Don't change t0sz here, mask those bits when restoring' */
	mrs	x7, tcr_el1
	bfi	x8, x7, #TCR_T0SZ_OFFSET, #TCR_TxSZ_WIDTH

	msr	tcr_el1, x8
	msr	vbar_el1, x9

	/*
	 * __cpu_setup() cleared MDSCR_EL1.MDE and friends, before unmasking
	 * debug exceptions. By restoring MDSCR_EL1 here, we may take a debug
	 * exception. Mask them until local_daif_restore() in cpu_suspend()
	 * resets them.
	 */
	msr	mdscr_el1, x10
	msr	sctlr_el1, x12
	msr	tpidr_el2, x13
	msr	sp_el1, x14
	/*
	 * Restore oslsr_el1 by writing oslar_el1
	 */
	ubfx	x11, x11, #1, #1

	msr	oslar_el1, x11

	isb
	ret
SYM_FUNC_END(cpu_do_resume)

/*
 * Save CPU state in the provided sleep_stack_data area, and publish its
 * location for cpu_resume()'s use in sleep_save_stash.
 *
 * cpu_resume() will restore this saved state, and return. Because the
 * link-register is saved and restored, it will appear to return from this
 * function. So that the caller can tell the suspend/resume paths apart,
 * __cpu_suspend_enter() will always return a non-zero value, whereas the
 * path through cpu_resume() will return 0.
 *
 *  x0 = struct sleep_stack_data area
 */
SYM_FUNC_START(__cpu_suspend_enter)

	stp	x29, x30, [x0, #SLEEP_STACK_DATA_CALLEE_REGS]
	stp	x19, x20, [x0,#(SLEEP_STACK_DATA_CALLEE_REGS+16)]
	stp	x21, x22, [x0,#(SLEEP_STACK_DATA_CALLEE_REGS+32)]
	stp	x23, x24, [x0,#(SLEEP_STACK_DATA_CALLEE_REGS+48)]
	stp	x25, x26, [x0,#(SLEEP_STACK_DATA_CALLEE_REGS+64)]
	stp	x27, x28, [x0,#(SLEEP_STACK_DATA_CALLEE_REGS+80)]

	/* save the sp in cpu_suspend_ctx */
	mov	x2, sp
	str	x2, [x0, #(SLEEP_STACK_DATA_SYSTEM_REGS + CPU_CTX_SP)]

	ldr	x1, =sleep_save_stash
	str x0, [x1]

	add	x0, x0, #SLEEP_STACK_DATA_SYSTEM_REGS
	stp	x29, x30, [sp, #-16]!

	bl	cpu_do_suspend
	ldp	x29, x30, [sp], #16
	mov	x0, #1
	ret
SYM_FUNC_END(__cpu_suspend_enter)

SYM_CODE_START(cpu_resume)

	ldr	x8, =_cpu_resume
	br	x8
SYM_CODE_END(cpu_resume)

SYM_FUNC_START(_cpu_resume)
	mov x7, #0
	/* x7 contains hash index, let's use it to grab context pointer' */
	ldr	x0, =sleep_save_stash
	ldr	x0, [x0]
	add	x29, x0, #SLEEP_STACK_DATA_CALLEE_REGS
	add	x0, x0, #SLEEP_STACK_DATA_SYSTEM_REGS
	/* load sp from context */
	ldr	x2, [x0, #CPU_CTX_SP]
	mov	sp, x2


	/*
	 * cpu_do_resume expects x0 to contain context address pointer
	 */
	bl	cpu_do_resume

#if defined(CONFIG_KASAN) && defined(CONFIG_KASAN_STACK)
	mov	x0, sp
	bl	kasan_unpoison_task_stack_below
#endif

	ldp	x19, x20, [x29, #16]
	ldp	x21, x22, [x29, #32]
	ldp	x23, x24, [x29, #48]
	ldp	x25, x26, [x29, #64]
	ldp	x27, x28, [x29, #80]
	ldp	x29, x30, [x29]
	mov	x0, #0
	ret
SYM_FUNC_END(_cpu_resume)
