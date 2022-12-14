/*
 * Copyright (C) 2014-2022 Amlogic, Inc. All rights reserved.
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
#ifndef __PM_H__
#define __PM_H__

#include "common.h"

#define WS_NEED_WAKEUP		1
#define WS_NEED_NOT_WAKEUP	0

#define listGET_OWNER_OF_PRE_ENTRY( pxTCB, pxList )	\
{	\
	List_t * const pxConstList = ( pxList );	\
		/* Increment the index to the next item and return the item, ensuring */	\
		/* we don't return the marker used at the end of the list.  */	\
		if( ( void * ) ( pxConstList )->pxIndex == ( void * ) &( ( pxConstList )->xListEnd ) )	\
		{	\
			( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxPrevious;	\
		}	\
		( pxTCB ) = ( pxConstList )->pxIndex->pvOwner;	\
		( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxPrevious;	\
}


typedef void * ws_t;

struct platform_power_ops {
	int (*begin) (void);
	int (*end) (void);
};

struct dev_power_ops {
	int (*enter) (void *arg);
	int (*restore) (void *arg);
};

#if defined (SUPPORT_PM)
int pm_enter(void);
int pm_wake_up(ws_t arg);
int dev_unregister_ws(ws_t arg);
int find_static_power_dev(void);
void wakeup_ap_from_kernel(void);
int set_platform_power_ops(struct platform_power_ops *ops);
struct wakeup_source * dev_register_ws(char * name, struct dev_power_ops *ops, void *data, uint32_t need_wakeup_flag);
#else
static inline int pm_enter(void)
{
	return 0;
}

static inline void wakeup_ap_from_kernel(void)
{
	;
}
#endif
#endif
