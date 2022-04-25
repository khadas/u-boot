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
 * power domain header file
 */

#ifndef _POWER_DOMAIN_H_
#define _POWER_DOMAIN_H_

#define PWR_ON    1
#define PWR_OFF   0

#define EVT_CPU_PWR_ON                          47
#define EVT_CPU_PWR_OFF                         48
#define EVT_DDR_ON                              49
#define EVT_DDR_OFF                             50

#define FSM_NUM   8

#define PWR_STATE_IDLE                 0
#define PWR_STATE_OFF_WAIT_OTH_FSM     1 //wait other pwr_fsm powered down
#define PWR_STATE_OFF_WAIT_PACTIVE     2 //wait pactive
#define PWR_STATE_OFF_START_HS         3 //start handshake
#define PWR_STATE_OFF_SET_PSTATE       4 //set pstate = reg
#define PWR_STATE_OFF_SET_PREQ1        5 //set preq = 1
#define PWR_STATE_OFF_PDENIED          6 //set preq = 0, back to preq1
#define PWR_STATE_OFF_PACCEPT          7 //set preq = 0, jump to OFF_WAIT
#define PWR_STATE_OFF_WAIT             8 //add this because CPU need time to reset
#define PWR_STATE_OFF_S0               9 //set reset = 1
#define PWR_STATE_OFF_S1              10 //set ISO = 1
#define PWR_STATE_OFF_S2              11 //set mempd = 1
#define PWR_STATE_OFF_S3              12 //set pwr off = 1
#define PWR_STATE_WAIT_ON             16 //wait irq
#define PWR_STATE_ON_WAIT_OTH_FSM     17 //wait other pwr_fsm powered on
#define PWR_STATE_ON_S0               18 //set pwr off = 0
#define PWR_STATE_ON_S0B              19 //wait pwr ack = 1
#define PWR_STATE_ON_S1A              20 //mempd
#define PWR_STATE_ON_S1B              21 //mempd
#define PWR_STATE_ON_S1C              22 //mempd
#define PWR_STATE_ON_S1D              23 //mempd
#define PWR_STATE_ON_S2               24 //set ISO = 0
#define PWR_STATE_ON_S3               25 //set reset = 0
#define PWR_STATE_ON_WAIT_PACTIVE     26 //wait pactive
#define PWR_STATE_ON_START_HS         27 //start handshake
#define PWR_STATE_ON_SET_PSTATE       28 //set pstate = reg
#define PWR_STATE_ON_SET_PREQ1        29 //set preq = 1
#define PWR_STATE_ON_PDENIED          30 //set preq = 0, back to preq1
#define PWR_STATE_ON_PACCEPT          31 //set preq = 0, jump to OFF_WAIT

typedef enum a {
    PM_CPU_PWR,
    PM_CPU_CORE0,
    PM_CPU_CORE1,
    PM_CPU_CORE2,
    PM_CPU_CORE3,
    PM_DSPA,
    PM_AOCPU,
    PM_NNA      = FSM_NUM + 1,       // 9
    PM_AUDIO                 ,       // 10
    PM_RESV_SEC              ,       // 11
    PM_SDIOA                 ,       // 12
    PM_EMMC                  ,       // 13
    PM_USB_COMB              ,       // 14
    PM_RESV_SYS_WRAP         ,       // 15
    PM_ETH                   ,       // 16
    PM_RESV0                 ,       // 17
    PM_RSA              = 23 ,       // 23
    PM_AUDIO_PDM             ,       // 24
    PM_DMC              = 26 ,       // 26
    PM_SYS_WRAP         = 32         // 32
} PM_E;

void  power_switch_to_domains(PM_E domain, uint32_t pwr_state);
void start_hw_pwrctrl_cpu_on(int id);
void power_switch_to_wraper(uint32_t pwr_state);

#endif
