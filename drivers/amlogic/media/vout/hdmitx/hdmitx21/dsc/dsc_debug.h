/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2021 Amlogic, Inc. All rights reserved.
 */

#ifndef __DSC_DEBUG_H__
#define __DSC_DEBUG_H__

#define DSC_PR(fmt, args...)      pr_info("dsc: " fmt "", ## args)
#define DSC_ERR(fmt, args...)     pr_err("dsc error: " fmt "", ## args)

#endif

