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


/* wake up reason*/
#define	UDEFINED_WAKEUP	0
#define	CHARGING_WAKEUP	1
#define	REMOTE_WAKEUP		2
#define	RTC_WAKEUP			3
#define	BT_WAKEUP			4
#define	WIFI_WAKEUP			5
#define	POWER_KEY_WAKEUP	6
#define	AUTO_WAKEUP			7
#define CEC_WAKEUP		8
#define	REMOTE_CUS_WAKEUP		9
#define ETH_PMT_WAKEUP      10
#define CECB_WAKEUP		11
#define ETH_PHY_GPIO    12
#define VAD_WAKEUP	13
#define HDMI_RX_WAKEUP	14

#define STR_QUEUE_LENGTH    32
#define STR_QUEUE_ITEM_SIZE 4

typedef struct {
	char* name;
} WakeUp_Reason;

void vDDR_suspend(uint32_t st_f);
void vDDR_resume(uint32_t st_f);
uint32_t parse_suspend_msg(void *msg);
void vCLK_suspend(uint32_t st_f);
void vCLK_resume(uint32_t st_f);
extern void create_str_task(void);
extern void STR_Start_Sem_Give_FromISR(void);
extern void STR_Start_Sem_Give(void);
extern void STR_Wakeup_src_Queue_Send_FromISR(uint32_t *src);
extern void STR_Wakeup_src_Queue_Send(uint32_t *src);
extern void *xMboxSuspend_Sem(void *msg);

