/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <arch.h>
#include <platform_def.h>

	.globl	bl33z_entrypoint
	.globl	bl33z_entry

bl33z_entrypoint:
	b	bl33z_entry

bl33z_label:
	.quad 0x504D55444D454D40

bl33z_entry:

	/*---------------------------------------------
	 * Store the extents of the tzram available to
	 * BL33Z for future use. Use the opcode param to
	 * allow implement other functions if needed.
	 * ---------------------------------------------
	 */
	ldr	x9, =PLATFORM_STACK_POSI
	sub	x9, x9, #(14 * 8)

	stp	x19, x20, [x9, #0]
	stp	x21, x22, [x9, #16]
	stp	x23, x24, [x9, #32]
	stp	x25, x26, [x9, #48]
	stp	x27, x28, [x9, #64]
	stp	x29, x30, [x9, #80]
	mov	x20, sp
	stp	x20, x0,  [x9, #96]

	mov	x20, x0
	mov	x21, x1
	mov	x22, x2
	mov	x23, x3

	sub	sp, x9, #16	// set sp

	/* ---------------------------------------------
	 * Set the exception vector to something sane.
	 * ---------------------------------------------
	 */

	/* ---------------------------------------------
	 * Zero out NOBITS sections. There are 2 of them:
	 *   - the .bss section;
	 *   - the coherent memory section.
	 * ---------------------------------------------
	 */
	ldr	x0, =__BSS_START__
	ldr	x1, =__BSS_SIZE__
	bl	zeromem16

#ifdef COHERENT_RAM
	ldr	x0, =__COHERENT_RAM_START__
	ldr	x1, =__COHERENT_RAM_UNALIGNED_SIZE__
	bl	zeromem16
#endif

	/* ---------------------------------------------
	 * Jump to main function.
	 * ---------------------------------------------
	 */
	/*mov	x0, x20
	mov	x1, x21
	mov	x2, x22
	mov	x3, x23*/
	bl	bl33z_main

	add	x9, sp, #16
	ldp	x20, x0,  [x9, #96]
	mov	sp,  x20
	ldp	x19, x20, [x9, #0]
	ldp	x21, x22, [x9, #16]
	ldp	x23, x24, [x9, #32]
	ldp	x25, x26, [x9, #48]
	ldp	x27, x28, [x9, #64]
	ldp	x29, x30, [x9, #80]

	ret x30

	.globl	zeromem16

zeromem16:
	add	x2, x0, x1
/* zero 16 bytes at a time */
z_loop16:
	sub	x3, x2, x0
	cmp	x3, #16
	b.lt	z_loop1
	stp	xzr, xzr, [x0], #16
	b	z_loop16
/* zero byte per byte */
z_loop1:
	cmp	x0, x2
	b.eq	z_end
	strb	wzr, [x0], #1
	b	z_loop1
z_end:	ret
