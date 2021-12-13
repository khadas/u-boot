// See LICENSE for license details.

#include "riscv_encoding.h"
#include "soc.h"

	.section .init

	.globl _start
	.type _start,@function
_start:
	csrc CSR_MSTATUS, MSTATUS_MIE
#ifndef N200_REVA
	/* Set the the NMI base to share with mtvec by setting CSR_MMISC_CTL */
	li t0, (0x1 << 9);
	csrs CSR_MMISC_CTL, t0

/* Intial the mtvt*/
	la t0, vector_base
	csrw CSR_MTVT, t0

/* Intial the mtvt2 and enable it*/
	la t0, irq_entry
	csrw CSR_MTVT2, t0
	csrs CSR_MTVT2, 0x1

	/* Intial the CSR MTVEC for the Trap ane NMI base addr*/
	la t0, trap_entry
	li t1, 0x3   // use ucliec
	or t0, t0, t1
	csrw CSR_MTVEC, t0

#ifdef MPU_INIT
/* Setup MPU */
	li t0, SOC_PMP_BASE
	/* Enable SRAM full range rwx */
	li t1, SRAM_BEGIN
	li t2, SRAM_END - 0x80
	add t1, t1, 0x0f
	sw t2, 28(t0)
	sw t1, 24(t0)
	/* Enable IO full range rw */
	li t1, IO_BEGIN
	li t2, IO_END - 0x80
	add t1, t1, 0x0d
	sw t2, 20(t0)
	sw t1, 16(t0)
#endif

#ifdef __riscv_flen
	/* Enable FPU */
	li t0, MSTATUS_FS
	csrs mstatus, t0
	csrw fcsr, x0
#endif
#endif
	.option push
	.option norelax
	.option pop
	la sp, _sp

#if defined (SOC_t3)
	/* Fixme T3 axi_ram access issuse */
	lw a0, _data_start
	lw a1, _data_end
	lw a2, _data_img
	bgeu a0, a1, 2f
1:
	lw a3, 0(a2)
	sw a3, (a0)
	addi a0, a0, 4
	addi a2, a2, 4
	bltu a0, a1, 1b
2:
#endif

	/* Clear bss section */
	lw a0, _bss_start
	lw a1, _bss_end
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
2:

	/* argc = argv = 0 */
	li a0, 0
	li a1, 0
	call main

1:
	j 1b

_bss_end:
.long _ebss
_bss_start:
.long _bss
#if defined (SOC_t3)
_data_end:
.long _edata
_data_start:
.long _data
_data_img:
.long _data_img_start
#endif

#ifndef N200_REVA
	.global disable_mcycle_minstret
disable_mcycle_minstret:
        csrsi CSR_MCOUNTINHIBIT, 0x5
	ret

	.global enable_mcycle_minstret
enable_mcycle_minstret:
        csrci CSR_MCOUNTINHIBIT, 0x5
	ret

	.global core_wfe
core_wfe:
    	csrc CSR_MSTATUS, MSTATUS_MIE
    	csrs CSR_WFE, 0x1
    	wfi
    	csrc CSR_WFE, 0x1
    	csrs CSR_MSTATUS, MSTATUS_MIE
	ret
#endif
