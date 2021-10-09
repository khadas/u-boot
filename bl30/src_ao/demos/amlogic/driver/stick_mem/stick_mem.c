/*
 * Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
 *
 * All information contained herein is Amlogic confidential.
 *
 * This software is provided to you pursuant to Software License Agreement
 * (SLA) with Amlogic Inc ("Amlogic"). This software may be used
 * only in accordance with the terms of this agreement.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification is strictly prohibited without prior written permission from
 * Amlogic.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "stick_mem.h"
#include "myprintf.h"


extern unsigned int __stick_base;
unsigned int last_stick_reboot_flag;
unsigned int *p_stick_mem = (unsigned int *)configSTICK_REG_ADDR;//&__stick_base;

int stick_mem_read(enum stick_mem_idx index, unsigned int *buf)
{
	if ((p_stick_mem[STICK_FLAG_1] != STICK_MEM_FLAG_1)
		|| (p_stick_mem[STICK_FLAG_2] != STICK_MEM_FLAG_2)) {
		/*cprintf(CC_SYSTEM, "read stick mem bad value!\n");*/
		return -1;
	}

	if (index >= STICK_MAX) {
		printf("read stick mem bad index=%d!\n",index);
		return -2;
	}

	*(buf) = p_stick_mem[index];

	return 0;
}

int stick_mem_write(enum stick_mem_idx index, unsigned int val)
{
	if (index >= STICK_MAX) {
		printf("write stick mem bad index=%d!\n",index);
		return -2;
	}

	if (p_stick_mem[STICK_FLAG_1] != STICK_MEM_FLAG_1)
		p_stick_mem[STICK_FLAG_1] = STICK_MEM_FLAG_1;

	if (p_stick_mem[STICK_FLAG_2] != STICK_MEM_FLAG_2)
		p_stick_mem[STICK_FLAG_2] = STICK_MEM_FLAG_2;

	p_stick_mem[index] = val;

	return 0;
}

void stick_mem_init(void)
{
	unsigned int i;

	/* this is warm boot, get last stick reboot flag */
	if ((p_stick_mem[STICK_FLAG_1] == STICK_MEM_FLAG_1)
		&& (p_stick_mem[STICK_FLAG_2] == STICK_MEM_FLAG_2))
		last_stick_reboot_flag = p_stick_mem[STICK_REBOOT_FLAG];
	/* this is cold boot, so clear stick memory for init */
	else {
		for (i = 0; i < STICK_MAX; i++)
			p_stick_mem[i] = 0;
	}

	return;
}
