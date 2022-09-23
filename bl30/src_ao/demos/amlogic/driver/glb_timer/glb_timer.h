/*
 * Copyright (C) 2014-2021 Amlogic, Inc. All rights reserved.
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
#include  "register.h"

#define DRIVER_NAME     "meson_glb_timer"

#define TOP_CTRL_BASE		GLOBAL_TOP_CTRL0
#define TRIG_SRC_BASE		GLOBAL_TRIG_SRC0_CTRL0
#define OUTPUT_BASE		GLOBAL_GPIO_OUT0_CTRL0
#define OUTPUT_SEL_BASE		GLOBAL_GPIO_OUT0_SEL

#define SRC_OFFSET(REG, SRCN)   ((REG) + (((SRCN) * 0x08) << 2))

struct meson_glb_timer;

enum {
        TOP_CTRL0                       = 0x00 << 2,
        TOP_CTRL1                       = 0x01 << 2,
        TOP_CTRL2                       = 0x02 << 2,
        TOP_CTRL3                       = 0x03 << 2,
        TOP_CTRL4                       = 0x04 << 2,
        TOP_TS0                         = 0x08 << 2,
        TOP_TS1                         = 0x09 << 2,
};

enum {
        TRIG_SRC0_CTRL0                 = 0x00 << 2,
        TRIG_SRC0_TS_L                  = 0x01 << 2,
        TRIG_SRC0_TS_H                  = 0x02 << 2,
};

enum {
        OUTPUT_CTRL0                    = 0x00 << 2,
        OUTPUT_PULSE_WDITH              = 0x01 << 2,
        OUTPUT_INTERVAL                 = 0x02 << 2,
        OUTPUT_STOP_NUM                 = 0x03 << 2,
        OUTPUT_EXPIRES_TS_L             = 0x04 << 2,
        OUTPUT_EXPIRES_TS_H             = 0x05 << 2,
        OUTPUT_ST0                      = 0x06 << 2,
        OUTPUT_ST1                      = 0x07 << 2,
};

enum {
        RESET_TIMER                     = 0x1,
        HOLD_COUNTER                    = 0x2,
        CLEAR_TIMER                     = 0x3,
        SET_FORCE_COUNT                 = 0x4,
};

enum {
        LOW                             = 0,
        HIGH                            = 1,
};

struct meson_glb_timer {

};
