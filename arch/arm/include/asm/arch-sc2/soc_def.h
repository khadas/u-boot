/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __SC2_DEF_H__
#define __SC2_DEF_H__

#include "secure_apb.h"


#define SEC_AO_SEC_GP_CFG2     SYSCTRL_SEC_STATUS_REG6
#define SEC_AO_SEC_SD_CFG15	SYSCTRL_SEC_STATUS_REG31

#define SEC_AO_RTI_STATUS_REG3		SYSCTRL_SEC_STICKY_REG1


#define AO_SEC_SD_CFG15 SEC_AO_SEC_SD_CFG15

/*for SC2 DSP compatible*/
#define CLKTREE_DSPA_CLK_CTRL0 CLKCTRL_DSPA_CLK_CTRL0
#define CLKTREE_DSPB_CLK_CTRL0 CLKCTRL_DSPB_CLK_CTRL0

#endif /* __SC2_DEF_H__ */
