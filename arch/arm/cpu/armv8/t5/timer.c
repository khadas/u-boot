/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/cpu/armv8/t5/timer.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <asm/arch/secure_apb.h>
#include <asm/arch/timer.h>
#include <asm/types.h>
#include <serial.h>

#define P_EE_TIMER_E		(volatile uint32_t *)0xffd0f188

uint32_t get_time(void)
{
	return readl(P_EE_TIMER_E);
}

void _udelay(unsigned int us)
{
#ifndef CONFIG_PXP_EMULATOR
	unsigned int t0 = get_time();

	while (get_time() - t0 <= us)
		;
#endif
}

#ifdef BL33_BOOT_TIME_PROBE
struct{
const char *pInfo;
unsigned int nTM;
}g_TMArray[100];

int g_nTMCnt = 0;


void TE_time(const char *szInfo)
{
	int i = 0;
	int nFoundFlag = 0;

	for (i = 0;i< sizeof(g_TMArray)/sizeof(g_TMArray[0]);++i)
	{
		if (szInfo == g_TMArray[i].pInfo)
		{
			nFoundFlag = 1;
			break;
		}
	}

	if (!nFoundFlag)
	{
		g_TMArray[g_nTMCnt].pInfo =(void*)szInfo;
		g_TMArray[g_nTMCnt++].nTM   = get_time();
	}
	else
	{
		int nNow = get_time();
		int nUsed = nNow - g_TMArray[i].nTM;
		if (nUsed/1000 >= 50)
			printf("\nTE: %d : %s : used %d\n",nNow,szInfo,nUsed);
		g_nTMCnt--;

		for ( ; i< sizeof(g_TMArray)/sizeof(g_TMArray[0]);++i)
		{
			g_TMArray[i] = g_TMArray[i+1];
			g_TMArray[i+1].pInfo= 0;
			g_TMArray[i+1].nTM = 0;
		}
	}
	printf("\n");
}
#endif
