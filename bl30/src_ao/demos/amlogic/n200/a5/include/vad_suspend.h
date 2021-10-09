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

/*
 * vad suspend header file
 */

#ifndef _VAD_SUSPEND_H_
#define _VAD_SUSPEND_H_

#ifdef __cplusplus
extern "C" {
#endif

#define VAD_TASK_PRI                      5
#define DSP_VAD_WAKUP_ARM      0x5555AAAA
#define WAIT_SWITCH_TO_24MHZ   0x5A5A5A5A
#define WAIT_SWITCH_TO_RTC_PLL 0xA5A5A5A5
#define WAKEUP_FROM_OTHER_KEY  0xA8A8A8A8

/*use timerI to wakeup dsp FSM*/
inline static void wakeup_dsp(void)
{
	uint32_t value;
	//uint32_t time_out = 20;

	/*set alarm timer*/
	REG32(DSP_FSM_TRIGER_SRC) = 10;/*10us*/

	value = REG32(DSP_FSM_TRIGER_CTRL);
	value &= ~((1 << 7) | (0x3) | (1 << 6));
	value |= ((1 << 7) | (0 << 6) | (0x3));
	REG32(DSP_FSM_TRIGER_CTRL) = value;
	vTaskDelay(1);
}

inline static void clear_dsp_wakeup_trigger(void)
{
	REG32(DSP_FSM_TRIGER_SRC) = 0;
	REG32(DSP_FSM_TRIGER_CTRL) = 0;
}

#ifdef __cplusplus
}
#endif
#endif
