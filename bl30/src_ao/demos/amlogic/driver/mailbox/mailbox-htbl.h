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

/**
 * handler table for mailbox
 *
 * Author: Yang Liu <yang.liu@amlogic.com>
 * Version:
 * - 0.1        init
 */

#ifndef _MAILBOX_HTBL_H_
#define _MAILBOX_HTBL_H_

#include <stdint.h>
#include "mailbox-api.h"

/** handler table
 * 0, mailbox init one htbl
 * 1, user could register it's handler on specific cmd to htbl
 * 2, mailbox invoke handler when specific cmd come from peer core
 *
 * For now, ARM and DSP have only one htbl at each side.
 */
void mailbox_htbl_init(void **pHTbl);
void mailbox_htbl_init_size(void **pHTbl, uint32_t tabLen);
uint32_t mailbox_htbl_reg(void *pHTbl, uint32_t cmd, void *(handler)(void *));
uint32_t mailbox_htbl_reg_feedback(void *pHTbl, uint32_t cmd,
                              void *(*handler)(void *), uint8_t needFdBak);
uint32_t mailbox_htbl_unreg(void *pHTbl, uint32_t cmd);
uint32_t mailbox_htbl_invokeCmd(void *pHTbl, uint32_t cmd, void *arg);

/** dispatch mailbox msg from ISR to job task with this struct
 * sample user case:
 * In ISR:
 * 1, get msg(cmd and data) from mailbox, create event
 * 2, push event to Q in ISR
 * 3, notify the job task in ISR
 * In job task:
 * 1, wait until be notified from ISR
 * 2, recieve event from Q
 * 3, registered handler process the event
 * 4, waiting for next event
 */
// ARM side don't support malloc in ISR,
// so change to fixed payload style
// defect:
// - fixed payload length
// - it have to copy to Q when pushing, copy from Q when poping
#define PAYLOAD_LEN         24
struct event {
    uint32_t cmd;
    uint8_t arg[PAYLOAD_LEN];
};
#endif
