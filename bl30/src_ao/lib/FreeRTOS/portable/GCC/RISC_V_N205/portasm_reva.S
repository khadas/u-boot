/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#include "soc.h"
#include "riscv_encoding.h"
#include "n200_timer.h"
#ifndef N200_REVA
#include "n200_eclic.h"
#else
#include "n200_pic_tmr.h"
//#include "riscv_bits.h"
#endif

//#include "portmacro.h"

# define STORE    sw
# define LOAD     lw
# define REGBYTES 4


#define INT_MASK 0x7FFFFFFF

.section      .text.entry
.align 4

.global portSAVE_CONTEXT
.global portRESTORE_CONTEXT
.global xPortStartScheduler
.global vPortYield
.global vPortEndScheduler
.global DISABLE_MIE

/* Macro for saving task context */
.macro pushREGFILE
	/* make room in stack */
	addi	sp, sp, -REGBYTES * 36

	/* Save Context */
	STORE	x1, 1 * REGBYTES(sp)
	STORE	x2, 2 * REGBYTES(sp)
	//STORE	x3, 3 * REGBYTES(sp)
	STORE	x4, 4 * REGBYTES(sp)
	STORE	x5, 5 * REGBYTES(sp)
	STORE	x6, 6 * REGBYTES(sp)
	STORE	x7, 7 * REGBYTES(sp)
	STORE	x8, 8 * REGBYTES(sp)
	STORE	x9, 9 * REGBYTES(sp)
	STORE	x10, 10 * REGBYTES(sp)
	STORE	x11, 11 * REGBYTES(sp)
	STORE	x12, 12 * REGBYTES(sp)
	STORE	x13, 13 * REGBYTES(sp)
	STORE	x14, 14 * REGBYTES(sp)
	STORE	x15, 15 * REGBYTES(sp)
#ifndef __riscv_32e
	STORE	x16, 16 * REGBYTES(sp)
	STORE	x17, 17 * REGBYTES(sp)
	STORE	x18, 18 * REGBYTES(sp)
	STORE	x19, 19 * REGBYTES(sp)
	STORE	x20, 20 * REGBYTES(sp)
	STORE	x21, 21 * REGBYTES(sp)
	STORE	x22, 22 * REGBYTES(sp)
	STORE	x23, 23 * REGBYTES(sp)
	STORE	x24, 24 * REGBYTES(sp)
	STORE	x25, 25 * REGBYTES(sp)
	STORE	x26, 26 * REGBYTES(sp)
	STORE	x27, 27 * REGBYTES(sp)
	STORE	x28, 28 * REGBYTES(sp)
	STORE	x29, 29 * REGBYTES(sp)
	STORE	x30, 30 * REGBYTES(sp)
	STORE	x31, 31 * REGBYTES(sp)
#endif
.endm

.macro pushREGFILE_Caller
	/* make room in stack */
	addi	sp, sp, -REGBYTES * 36

	/* Save Context */
	STORE	x1, 1 * REGBYTES(sp)
	//STORE	x2, 2 * REGBYTES(sp)
	////STORE	x3, 3 * REGBYTES(sp)
	//STORE	x4, 4 * REGBYTES(sp)
	STORE	x5, 5 * REGBYTES(sp)
	STORE	x6, 6 * REGBYTES(sp)
	STORE	x7, 7 * REGBYTES(sp)
	//STORE	x8, 8 * REGBYTES(sp)
	//STORE	x9, 9 * REGBYTES(sp)
	STORE	x10, 10 * REGBYTES(sp)
	STORE	x11, 11 * REGBYTES(sp)
	STORE	x12, 12 * REGBYTES(sp)
	STORE	x13, 13 * REGBYTES(sp)
	STORE	x14, 14 * REGBYTES(sp)
	STORE	x15, 15 * REGBYTES(sp)
#ifndef  __riscv_32e
	STORE	x16, 16 * REGBYTES(sp)
	STORE	x17, 17 * REGBYTES(sp)
	//STORE	x18, 18 * REGBYTES(sp)
	//STORE	x19, 19 * REGBYTES(sp)
	//STORE	x20, 20 * REGBYTES(sp)
	//STORE	x21, 21 * REGBYTES(sp)
	//STORE	x22, 22 * REGBYTES(sp)
	//STORE	x23, 23 * REGBYTES(sp)
	//STORE	x24, 24 * REGBYTES(sp)
	//STORE	x25, 25 * REGBYTES(sp)
	//STORE	x26, 26 * REGBYTES(sp)
	//STORE	x27, 27 * REGBYTES(sp)
	STORE	x28, 28 * REGBYTES(sp)
	STORE	x29, 29 * REGBYTES(sp)
	STORE	x30, 30 * REGBYTES(sp)
	STORE	x31, 31 * REGBYTES(sp)
	#endif
.endm

/* Macro for saving task context */
.macro pushREGFILE_Callee

	/* Save Context */
	//STORE	x1, 1 * REGBYTES(sp)
	STORE	x2, 2 * REGBYTES(sp)
	////STORE	x3, 3 * REGBYTES(sp)
	STORE	x4, 4 * REGBYTES(sp)
//	STORE	x5, 5 * REGBYTES(sp)
	//STORE	x6, 6 * REGBYTES(sp)
	//STORE	x7, 7 * REGBYTES(sp)
	STORE	x8, 8 * REGBYTES(sp)
	STORE	x9, 9 * REGBYTES(sp)
	//STORE	x10, 10 * REGBYTES(sp)
	//STORE	x11, 11 * REGBYTES(sp)
	//STORE	x12, 12 * REGBYTES(sp)
	//STORE	x13, 13 * REGBYTES(sp)
	//STORE	x14, 14 * REGBYTES(sp)
	//STORE	x15, 15 * REGBYTES(sp)
	//STORE	x16, 16 * REGBYTES(sp)
	//STORE	x17, 17 * REGBYTES(sp)
#ifndef __riscv_32e
	STORE	x18, 18 * REGBYTES(sp)
	STORE	x19, 19 * REGBYTES(sp)
	STORE	x20, 20 * REGBYTES(sp)
	STORE	x21, 21 * REGBYTES(sp)
	STORE	x22, 22 * REGBYTES(sp)
	STORE	x23, 23 * REGBYTES(sp)
	STORE	x24, 24 * REGBYTES(sp)
	STORE	x25, 25 * REGBYTES(sp)
	STORE	x26, 26 * REGBYTES(sp)
	STORE	x27, 27 * REGBYTES(sp)
	//STORE	x28, 28 * REGBYTES(sp)
	//STORE	x29, 29 * REGBYTES(sp)
	//STORE	x30, 30 * REGBYTES(sp)
	//STORE	x31, 31 * REGBYTES(sp)
#endif
.endm


/* Macro for restoring task context */
.macro popREGFILE

	/* Restore registers,
	Skip global pointer because that does not change */
	LOAD	x1, 1 * REGBYTES(sp)
	//LOAD 	x3, 3 * REGBYTES(sp)
	LOAD	x4, 4 * REGBYTES(sp)
	LOAD	x5, 5 * REGBYTES(sp)
	LOAD	x6, 6 * REGBYTES(sp)
	LOAD	x7, 7 * REGBYTES(sp)
	LOAD	x8, 8 * REGBYTES(sp)
	LOAD	x9, 9 * REGBYTES(sp)
	LOAD	x10, 10 * REGBYTES(sp)
	LOAD	x11, 11 * REGBYTES(sp)
	LOAD	x12, 12 * REGBYTES(sp)
	LOAD	x13, 13 * REGBYTES(sp)
	LOAD	x14, 14 * REGBYTES(sp)
	LOAD	x15, 15 * REGBYTES(sp)
#ifndef __riscv_32e
	LOAD	x16, 16 * REGBYTES(sp)
	LOAD	x17, 17 * REGBYTES(sp)
	LOAD	x18, 18 * REGBYTES(sp)
	LOAD	x19, 19 * REGBYTES(sp)
	LOAD	x20, 20 * REGBYTES(sp)
	LOAD	x21, 21 * REGBYTES(sp)
	LOAD	x22, 22 * REGBYTES(sp)
	LOAD	x23, 23 * REGBYTES(sp)
	LOAD	x24, 24 * REGBYTES(sp)
	LOAD	x25, 25 * REGBYTES(sp)
	LOAD	x26, 26 * REGBYTES(sp)
	LOAD	x27, 27 * REGBYTES(sp)
	LOAD	x28, 28 * REGBYTES(sp)
	LOAD	x29, 29 * REGBYTES(sp)
	LOAD	x30, 30 * REGBYTES(sp)
	LOAD	x31, 31 * REGBYTES(sp)
#endif
	addi	sp, sp, REGBYTES * 36
.endm

.macro popREGFILE_Caller

	/* Restore registers,
	Skip global pointer because that does not change */
	LOAD	x1, 1 * REGBYTES(sp)
	////LOAD 	x3, 3 * REGBYTES(sp)
	//LOAD	x4, 4 * REGBYTES(sp)
	LOAD	x5, 5 * REGBYTES(sp)
	LOAD	x6, 6 * REGBYTES(sp)
	LOAD	x7, 7 * REGBYTES(sp)
	//LOAD	x8, 8 * REGBYTES(sp)
	//LOAD	x9, 9 * REGBYTES(sp)
	LOAD	x10, 10 * REGBYTES(sp)
	LOAD	x11, 11 * REGBYTES(sp)
	LOAD	x12, 12 * REGBYTES(sp)
	LOAD	x13, 13 * REGBYTES(sp)
	LOAD	x14, 14 * REGBYTES(sp)
	LOAD	x15, 15 * REGBYTES(sp)
#ifndef __riscv_32e
	LOAD	x16, 16 * REGBYTES(sp)
	LOAD	x17, 17 * REGBYTES(sp)
	//LOAD	x18, 18 * REGBYTES(sp)
	//LOAD	x19, 19 * REGBYTES(sp)
	//LOAD	x20, 20 * REGBYTES(sp)
	//LOAD	x21, 21 * REGBYTES(sp)
	//LOAD	x22, 22 * REGBYTES(sp)
	//LOAD	x23, 23 * REGBYTES(sp)
	//LOAD	x24, 24 * REGBYTES(sp)
	//LOAD	x25, 25 * REGBYTES(sp)
	//LOAD	x26, 26 * REGBYTES(sp)
	//LOAD	x27, 27 * REGBYTES(sp)
	LOAD	x28, 28 * REGBYTES(sp)
	LOAD	x29, 29 * REGBYTES(sp)
	LOAD	x30, 30 * REGBYTES(sp)
	LOAD	x31, 31 * REGBYTES(sp)
#endif
	addi	sp, sp, REGBYTES * 36
.endm
###############################################

###############################################
/*saves mstatus and tcb	*/
.macro portSAVE_CONTEXT_EXCP
	.global	pxCurrentTCB
		/* Store mstatus */
	csrr	t0, mstatus	//pointer
	STORE	t0, 32 * REGBYTES(sp)

	csrr	t0, mepc	//pointer
	STORE	t0, 33 * REGBYTES(sp)

	csrr t0,   CSR_MSUBM
	STORE t0,  34*REGBYTES(sp)
	//csrr t0,  CSR_MCAUSE
	//STORE t0,  35*REGBYTES(sp)

	/* Store current stackpointer in task control block (TCB) */
	LOAD	t0, pxCurrentTCB	//pointer
	STORE	sp, 0x0(t0)
.endm


/*restore mstatus/mepc and tcb	*/
.macro portRESTORE_CONTEXT_EXCP
	.global	pxCurrentTCB
	/* Load stack pointer from the current TCB */
	LOAD	sp, pxCurrentTCB
	LOAD	sp, 0x0(sp)
	/* Load task program counter */
	LOAD	t0, 33 * REGBYTES(sp)
	csrw	mepc, t0
	/* Load saved mstatus */
	LOAD	t0, 32 * REGBYTES(sp)
	csrw	mstatus, t0

	//LOAD t0,  35*REGBYTES(sp)
	//csrw  CSR_MCAUSE, t0
	LOAD t0,  34*REGBYTES(sp)
	csrw CSR_MSUBM, t0
.endm


/*restore mstatus/mepc and tcb	*/
.macro portRESTORE_CONTEXT_EXCP_2
	.global	pxCurrentTCB
	/* Load stack pointer from the current TCB */
	LOAD	sp, pxCurrentTCB
	LOAD	sp, 0x0(sp)
	/* Load task program counter */
	LOAD	t0, 33 * REGBYTES(sp)
	csrw	mepc, t0
	/* Load saved mstatus */
	LOAD	t0, 32 * REGBYTES(sp)
	csrw	mstatus, t0

	LOAD t0,  34*REGBYTES(sp)
	csrw CSR_MSUBM, t0
.endm


/*saves istatus and tcb */
.macro portSAVE_CONTEXT_IRQ
	.global pxCurrentTCB

	csrr t0,   CSR_MSUBM
	STORE t0,  34*REGBYTES(sp)
	//csrr t0,  CSR_MCAUSE
	//STORE t0,  35*REGBYTES(sp)

	/* Store current stackpointer in task control block (TCB) */
	LOAD	 t0, pxCurrentTCB	 //pointer
	STORE	 sp, 0x0(t0)
.endm

 /*restore istatus/ipc and tcb	*/
.macro portRESTORE_CONTEXT_IRQ
	.global	pxCurrentTCB
	/* Load stack pointer from the current TCB */
	LOAD	sp, pxCurrentTCB
	LOAD	sp, 0x0(sp)

	//LOAD t0,  35*REGBYTES(sp)
 	//csrw  CSR_MCAUSE, t0
	LOAD t0,  34*REGBYTES(sp)
	csrw CSR_MSUBM, t0
.endm


/* Saves current error program counter (EPC) as task program counter */
.macro portSAVE_EPC_MSTATUS
	csrr	t0, mepc
	STORE	t0, 33 * REGBYTES(sp)
	csrr	t0, mstatus	//pointer
	STORE	t0, 32 * REGBYTES(sp)
.endm

.macro portRESTORE_EPC_MSTATUS
	LOAD	t0, 33 * REGBYTES(sp)
	csrw	mepc, t0
		/* Load saved istatus */
	LOAD	t0, 32 * REGBYTES(sp)
	csrw	mstatus, t0
.endm

/* Saves current interrupt program counter (IPC) as task program counter */
.macro portSAVE_IPC_ISTATUS
	csrr	t0, CSR_MIPC
	STORE	t0, 33 * REGBYTES(sp)
	/* Store istatus */
	csrr	 t0, CSR_ISTATUS //pointer
	STORE	 t0, 32 * REGBYTES(sp)
.endm

/* Restore current interrupt program counter (IPC) as task program counter */
.macro portRESTORE_IPC_ISTATUS
	LOAD	t0, 33 * REGBYTES(sp)
	csrw	CSR_MIPC, t0
		/* Load saved istatus */
	LOAD	t0, 32 * REGBYTES(sp)
	csrw	CSR_ISTATUS, t0
.endm


/* Saves current return adress (RA) as task program counter */
.macro portSAVE_RA
	LOAD	t0, 1 * REGBYTES(sp)
	STORE	t0, 33 * REGBYTES(sp)

.endm

###############################################
# Disable Interrupt
#
.macro DISABLE_MIE
  	csrc CSR_MSTATUS, MSTATUS_MIE
.endm


###############################################

/* Interrupt entry point */
.global trap_entry
.align 6
trap_entry:
	/* Check for interrupt */
	pushREGFILE
	csrr	a0, mcause
    	//Bob: we dont need to check interrupt here because N200 have seperated entry to IRQ
	//blt		a0,zero,interrupt

	/* synchronous interrupt*/
	/* pass mcause in a0 */
	/* pass sp in a1 */
	mv a1,sp
	jal ulSynchTrap
	/*  adjust stack pointer back to where it was prior to ulSynchTrap  */
	mv		sp,a0
	popREGFILE
	mret

###############################################
###############################################

/* async interrupt, this function is called */
//Bob: Rename this to irq_entry to make it override the weak symbol from BSPs entry.S
//interrupt:
.align 2
#ifndef N200_REVA
.global irq_entry
irq_entry: // -------------> This label will be set to MTVT2 register
  	// Allocate the stack space
  	pushREGFILE
	portSAVE_EPC_MSTATUS
	portSAVE_CONTEXT_IRQ
int_loop:             // 3 instructions in pending-interrupt service loop.
  	csrrw ra, CSR_JALMNXTI, ra

  	csrrsi a0, CSR_MNXTI, MSTATUS_MIE   // Claim any pending interrupt at level > mcause.pil
  	bnez a0, int_loop   // Loop to service any interrupt.

	/*have a chance to switch task*/
//	li	a2,40
//	ecall

  	#---- Critical section with interrupts disabled -----------------------
  	DISABLE_MIE # Disable interrupts
	/*ECALL to switch context*/

	portRESTORE_CONTEXT_IRQ
	portRESTORE_EPC_MSTATUS
  	popREGFILE
  	mret
#else

/* async interrupt, this function is called */
//Bob: Rename this to irq_entry to make it override the weak symbol from BSP's entry.S
//interrupt:
.global irq_entry
irq_entry:
	pushREGFILE_Caller
	portSAVE_IPC_ISTATUS

call_irq:
//Bob: Claim the IRQ
    li t0, (PIC_CTRL_ADDR + PIC_CLAIM_OFFSET)
    lw a0, 0(t0)
	/* Bob: Interupt m-time interrupt */
    li t0, PIC_INT_TMR
	beq a0,t0, MTIME_IRQ

	// a0 as the function argument
	jal 	handle_irq

	//Complete the IRQ
	// a0 as the return value
	li t0, (PIC_CTRL_ADDR + PIC_CLAIM_OFFSET)
	sw a0, 0(t0)

	// Check if there is pending irq to support interrupt tailing
	li t0, (PIC_CTRL_ADDR + PIC_EIP_OFFSET)// Use t0(x5) as the address register
	lw t0, 0(t0)// Read the value from PIC EIP reg
	bnez t0, call_irq // If the EIP reg is not zero, means there is a pending register

	portRESTORE_IPC_ISTATUS
	popREGFILE_Caller
	mret

//Bob: MTIME_IRQ is special which need to used to switch context, hence need to use special macro
	/* Interupt is m-time  */
MTIME_IRQ:
        //Bob: We need to save the callee before switching the context
	pushREGFILE_Callee
	portSAVE_CONTEXT_IRQ
    // Bob: change the MEPC to MIPC
	//portSAVE_EPC
	//portSAVE_IPC
    // a0 as the function argument
	jal		vPortSysTickHandler

  //Complete the IRQ
  // a0 as the return value
  li t0, (PIC_CTRL_ADDR + PIC_CLAIM_OFFSET)
  sw a0, 0(t0)

    // Bob: change the MEPC to MIPC
	portRESTORE_CONTEXT_IRQ
	portRESTORE_IPC_ISTATUS
	popREGFILE
	mret
#endif

.align 2
.globl MTIME_HANDLER
MTIME_HANDLER:
  	pushREGFILE
	portSAVE_EPC_MSTATUS
	portSAVE_CONTEXT_IRQ

//	csrs CSR_MSTATUS, MSTATUS_MIE
	jal		vPortSysTickHandler
//	csrc CSR_MSTATUS, MSTATUS_MIE

	portRESTORE_CONTEXT_IRQ
	portRESTORE_EPC_MSTATUS
	popREGFILE

	mret

#if 0
.align 2
.globl MSIP_HANDLER
MSIP_HANDLER:
	pushREGFILE
	portSAVE_CONTEXT_IRQ

	csrs CSR_MSTATUS, MSTATUS_MIE
	jal 	test_handler
	csrc CSR_MSTATUS, MSTATUS_MIE

	portRESTORE_CONTEXT_IRQ
	popREGFILE

	mret
#endif

xPortStartScheduler:
	jal		vPortSetup
	portRESTORE_CONTEXT_EXCP_2
	popREGFILE
	mret


.align 6
vPortYield:
	mv		sp,a0
	portSAVE_CONTEXT_EXCP

	STORE	a1, 33 * REGBYTES(sp)
	jal		vTaskSwitchContext

	portRESTORE_CONTEXT_EXCP
	popREGFILE

	mret

vPortEndScheduler:
1:
 j 1b



//Bob: we dont need this
//.weak handle_interrupt
//handle_interrupt:
//1:
  j 1b



