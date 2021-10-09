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

#include "FreeRTOS.h"
#include <common.h>
#include "util.h"
#include "meson_i2c.h"

#define HHI_GCLK_MPEG0			((0x050 << 2) + 0xff63c000)

struct xMesonI2cPlatdata t5w_i2c_data[] = {
	{0, 0xffd1f000, 3, 15, 100000, MESON_I2C_CLK_RATE, HHI_GCLK_MPEG0, 9}, /* i2c A */
	{1, 0xffd1e000, 3, 15, 100000, MESON_I2C_CLK_RATE, HHI_GCLK_MPEG0, 9}, /* i2c B */
	{2, 0xffd1d000, 3, 15, 100000, MESON_I2C_CLK_RATE, HHI_GCLK_MPEG0, 9}, /* i2c C */
	{3, 0xffd1c000, 3, 15, 100000, MESON_I2C_CLK_RATE, HHI_GCLK_MPEG0, 9}, /* i2c D */
};

void meson_i2c_plat_init(void)
{
	plat = t5w_i2c_data;
}
