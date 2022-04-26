/*
 *  Copyright (C) 2014-2019 Amlogic, Inc. All rights reserved.
 *
 *  All information contained herein is Amlogic confidential.
 *
 *  This software is provided to you pursuant to Software License Agreement
 *  (SLA) with Amlogic Inc ("Amlogic"). This software may be used
 *  only in accordance with the terms of this agreement.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification is strictly prohibited without prior written permission from
 *  Amlogic.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SARADC_DATA_H_
#define _SARADC_DATA_H_

#include <register.h>

#define SAR_CLK_BASE		CLKCTRL_SAR_CLK_CTRL0
#define SARADC_BASE		SAR_ADC_REG0

#define REG11_VREF_EN_VALUE	0
#define REG11_CMV_SEL_VALUE	0
#define REG11_EOC_VALUE		1

/* sc2 saradc interrupt num */
#define SARADC_INTERRUPT_NUM	181

#define SARADC_REG_NUM		(18 + 1)

#endif
