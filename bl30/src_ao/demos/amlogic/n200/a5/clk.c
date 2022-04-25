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

#include "FreeRTOS.h"
#include <common.h>
#include "util.h"
#include "register.h"
#include "uart.h"
#include <task.h>
#include <util.h>
#include <soc.h>
#include "timer_source.h"
#include "./include/clk.h"
#include "./include/clk_util.h"
#include "./include/vad_suspend.h"
#include "include/power_domain.h"

void set_time(uint32_t val);
uint32_t get_time(void);
void alt_timebase(int use_clk_src);
void set_sys_div_clk(int sel, int div);
void set_axi_div_clk(int sel, int div);
void clk_util_set_dsp_clk(uint32_t id, uint32_t freq_sel);
void disable_pll(int id);
void vCLK_resume(uint32_t st_f);
void vCLK_suspend(uint32_t st_f);

void set_time(uint32_t val)
{
#ifdef ARM_CPU
	REG32(SYSCTRL_SEC_TIMERE) = 0x0;
#else
	REG32(SYSCTRL_TIMERF) = val;
#endif
}


uint32_t get_time(void)
{
	uint64_t timerE = 0;

#ifdef ARM_CPU
	timerE = REG32(SYSCTRL_SEC_TIMERE);
#else
	timerE = REG32(SYSCTRL_TIMERF);
#endif
	return(timerE);
}

/*
 * use_clk_src
 * 0. osc_clk
 * 1. sys_clk (rtc_pll) clk_div1
 * 2. sys_clk (rtc_pll) clk_div2
 * 3. rtc_clk
 * Note:
 * 1. If select sys_clk, tick clk of 1us/10us/100us/1ms/xtal3 are all same
 * 2. If select rtc_clk, tick clk of 1us/10us/100us/1ms/xtal3 will actually
 *     be 1ms/10ms/100ms/1s/93.747us
 */

void alt_timebase(int use_clk_src)
{
	uint32_t clk_div = 0;
	if (use_clk_src == 0)
		//select osc_clk
		REG32(CLKCTRL_TIMEBASE_CTRL0) = 0x20018;
	else if (use_clk_src == 1) {
		//select sys_clk (rtc_pll) clk_div1
		//when sys_clk < 32MHz
		//if sys_clk is 30MHz
		clk_div = 30;
		REG32(CLKCTRL_TIMEBASE_CTRL0) = (1 << 17) | (clk_div<<19) | (0x2aa<<6);
	} else if (use_clk_src == 2) {
		/* select sys_clk (rtc_pll) clk_div2
		  when sys_clk < 256MHz  */
		// if sys_clk is 122.88MHz
		clk_div = 122;
		REG32(CLKCTRL_TIMEBASE_CTRL0) = (1 << 17) | (clk_div<<24) | (0x3ff<<6);
	} else {
		//select rtc clk
		// 32k/32 = 1k
		REG32(CLKCTRL_TIMEBASE_CTRL0) = 0x20020;
	}
}

/*
 * This is function for axi clk setting:
 * Includes axi_clk(AKA cpu_axi_clk/ACLK), axi_matrix, axi_sram
 *
 */
void set_axi_div_clk(int sel, int div)
{
	uint32_t control;
	uint32_t dyn_pre_mux;
	uint32_t dyn_div;

	// fclk_div4 = 500MHz
	dyn_pre_mux = sel;
	dyn_div = div;/* divide by 1 */

	control = REG32(CLKCTRL_AXI_CLK_CTRL0);
	if (control & (1 << 15)) {//default is preb, need use prea
		control = (control & ~(0x3fff))
					| ((1 << 13)
					| (dyn_pre_mux << 10)
					| (dyn_div << 0));
		REG32(CLKCTRL_AXI_CLK_CTRL0) = control;
		control = control & ~(1 << 15);
		REG32(CLKCTRL_AXI_CLK_CTRL0) = control;
		udelay(25);
	} else {
		//preb
		control = (control & ~((0x3fff) << 16))
					| ((1 << 29)
					| (dyn_pre_mux << 26)
					| (dyn_div << 16));
		REG32(CLKCTRL_AXI_CLK_CTRL0) = control;
		control = control | (1 << 15);
		REG32(CLKCTRL_AXI_CLK_CTRL0) = control;
		udelay(25);
	}

	return;
}

/*
 * This is function for clk81 setting
 *
 */
void set_sys_div_clk(int sel, int div)
{
	uint32_t control;
	uint32_t dyn_pre_mux;
	uint32_t dyn_div;

	printf("Set sys clock to 167Mhz\n");

	// fclk_div4 = 500MHz
	dyn_pre_mux = sel;
	dyn_div = div;/* divide by 3 */

	control = REG32(CLKCTRL_SYS_CLK_CTRL0);

	if (control & (1 << 15)) {//default is preb, need use prea
		control = (control & ~(0x3fff))
					| ((1 << 13)
					| (dyn_pre_mux << 10)
					| (dyn_div << 0));
		REG32(CLKCTRL_SYS_CLK_CTRL0) = control;
		control = control & ~(1 << 15);
		REG32(CLKCTRL_SYS_CLK_CTRL0) = control;
		udelay(25);
	} else {
		//preb
		control = (control & ~((0x3fff) << 16))
					| ((1 << 29)
					| (dyn_pre_mux << 26)
					| (dyn_div << 16));
		REG32(CLKCTRL_SYS_CLK_CTRL0) = control;
		control = control | (1 << 15);
		REG32(CLKCTRL_SYS_CLK_CTRL0) = control;
		udelay(25);
	}


	/* when bus switched from 24MHz to 166MHz,
	 * the clock tick used to generate OTP waveform
	 * needs 20us waiting time to switch to new clock tick
	 */
	udelay(25);

	return;
}
/*
 * clk_util_set_dsp_clk
 * freq_sel:
 *           0:800MHz  fclk_2p5
 *           1:400MHz  fclk_5
 *           2:500MHz  fclk_4
 *           3:666MHz  fclk_3
 *           4:333Mhz  fclk_3/2
 *           5:250Mhz  fclk_4/2
 *           6:200Mhz  fclk_5/2
 *           7:100Mhz  fclk_5/4
 *           8:24Mhz   oscin
 *           10:3Mhz    oscin/8
 *           others:400MHz  fclk_5
 *
 *.clk0            (cts_oscin_clk          ),
 *.clk1            (fclk_div2p5            ),
 *.clk2            (fclk_div3              ),
 *.clk3            (rtc_pll_clk            ),
 *.clk4            (hifi_pll_clk           ),
 *.clk5            (fclk_div4              ),
 *.clk6            (gp1_pll_clk            ),
 *.clk7            (cts_rtc_clk            ),
 */

/*  --------------------------------------------------
 *               clk_util_set_dsp_clk
 * --------------------------------------------------
 */

void clk_util_set_dsp_clk(uint32_t id, uint32_t freq_sel)
{
	uint32_t    control;
	uint32_t    clk_sel;
	uint32_t    clk_div;
	uint32_t    addr = CLKCTRL_DSPA_CLK_CTRL0;

	switch (id) {
		case 0:   addr = CLKCTRL_DSPA_CLK_CTRL0; break;
		//default : addr = CLKCTRL_DSPB_CLK_CTRL0; break;
		default : printf("CLK_UTIL:Error, no dspb here\n"); break;
	}

	//Make sure not busy from last setting and we currently match the last setting
	control = REG32(addr);

	switch (freq_sel)
	{
		case 0:
			clk_sel = 1;
			clk_div = 0;
			printf("CLK_UTIL:dsp[%d]:fclk2p5:800MHz\n", id);
			break;
		case 1:
			clk_sel = 1;
			clk_div = 1;
			printf("CLK_UTIL:dsp[%d]:fclk5:400MHz\n", id);
			break;
		case 2:
			clk_sel = 5;
			clk_div = 0;
			printf("CLK_UTIL:dsp[%d]:fclk4:500MHz\n", id);
			break;
		case 3:
			clk_sel = 2;
			clk_div = 0;
			printf("CLK_UTIL:dsp[%d]:fclk/3:667MHz\n", id);
			break;
		case 4:
			clk_sel = 2;
			clk_div = 1;
			printf("CLK_UTIL:dsp[%d]:fclk3/2:333MHz\n", id);
			break;
		case 5:
			clk_sel = 5;
			clk_div = 1;
			printf("CLK_UTIL:dsp[%d]:fclk4/2:250MHz\n", id);
			break;
		case 6:
			clk_sel = 1;
			clk_div = 3;
			printf("CLK_UTIL:dsp[%d]:fclk5/2:200MHz\n", id);
			break;
		case 7:
			clk_sel = 1;
			clk_div = 7;
			printf("CLK_UTIL:dsp[%d]:fclk5/4:100MHz\n", id);
			break;
		case 8:
			clk_sel = 0;
			clk_div = 0;
			printf("CLK_UTIL:dsp[%d]:oscin:24MHz\n", id);
			break;
		case 10:
			clk_sel = 0;
			clk_div = 7;
			printf("CLK_UTIL:dsp[%d]:oscin/8:3MHz\n", id);
			break;
		case 11:
			clk_sel = 3;
			clk_div = 0;
			printf("CLK_UTIL:dsp[%d]:rtc pll:122.88MHz\n", id);
			break;
		case 12:
			clk_sel = 7;
			clk_div = 0;
			printf("CLK_UTIL:dsp[%d]:rtc clk:24MHz\n", id);
			break;
		default:
			clk_sel = 1;
			clk_div = 7;
			printf("CLK_UTIL:dsp[%d]:fclk5:400MHz\n", id);
			break;
	}

	//if sync_mux ==1, sel mux 0
	if (control & (1 << 15))
		control = (control & ~( ( 1<<15) | (0x3ff<<0) | (0x7 <<10) ) ) | (1<<13) |
		(1<<29) | (clk_div<<0) | (clk_sel<<10);
	else
		control = (control & ~( ( 1<<15) | (0x3ff<<16) | (0x7 <<26) ) ) | (1<<13) |
		(1<<29) | (clk_div<<16) | (clk_sel<<26) | (1<<15);

	REG32(addr) = control;
}

void disable_pll(int id)
{
	switch (id) {
		case PLL_SYS:  REG32(ANACTRL_SYSPLL_CTRL0)  = (1<<29);  break;
		case PLL_GP0:  REG32(ANACTRL_GP0PLL_CTRL0)  = (1<<29);  break;
		case PLL_GP1:  REG32(ANACTRL_GP1PLL_CTRL0)  = (1<<29);  break;
		case PLL_FIX:  REG32(ANACTRL_FIXPLL_CTRL0)  = (1<<29);  break;
		case PLL_HIFI: REG32(ANACTRL_HIFIPLL_CTRL0) = (1<<29);  break;
		case PLL_RTC:  REG32(ANACTRL_RTCPLL_CTRL0)  = (1<<29);  break;
		case PLL_DDR:  REG32(AM_DDR_PLL_CNTL0)     = (1<<29);  break;
		default:  printf("Error: PLL is not in the list\n");  break;
	}
}

int oscin_ctrl_reg = 0;
void vCLK_resume(uint32_t st_f)
{
	if ((REG32(SYSCTRL_DEBUG_REG6) != WAIT_SWITCH_TO_RTC_PLL) &&
			(REG32(SYSCTRL_DEBUG_REG6) != DSP_VAD_WAKUP_ARM))
		printf("[AOCPU]:WARNING SYSCTRL_DEBUG_REG6's value is :0x%x!\n",
			REG32(SYSCTRL_DEBUG_REG6));

	if (REG32(SYSCTRL_DEBUG_REG6) == WAIT_SWITCH_TO_RTC_PLL)
		REG32(SYSCTRL_DEBUG_REG6) = WAKEUP_FROM_OTHER_KEY;
	/* switch osc_clk back*/
	REG32(CLKCTRL_SYSOSCIN_CTRL) = 1;
	REG32(CLKCTRL_OSCIN_CTRL) = oscin_ctrl_reg | (1<<31);
	udelay(9000);

	clear_dsp_wakeup_trigger();
	/* switching tick timer (using osc_clk) */
	alt_timebase(0);

	set_sys_div_clk(0, 0);  // osc_clk
	set_axi_div_clk(0, 0);  // osc_clk

	if ((REG32(SYSCTRL_DEBUG_REG6) != DSP_VAD_WAKUP_ARM) &&
			(REG32(SYSCTRL_DEBUG_REG6) != WAKEUP_FROM_OTHER_KEY))
		printf("[AOCPU]:WARNING SYSCTRL_DEBUG_REG6's value is :0x%x!\n",
			REG32(SYSCTRL_DEBUG_REG6));

	REG32(SYSCTRL_DEBUG_REG6) = WAIT_SWITCH_TO_24MHZ;

	vTaskDelay(pdMS_TO_TICKS(90));
//	disable_pll(PLL_RTC);

	if (st_f) {
		power_switch_to_wraper(PWR_ON);
		/* open mem_pd of srama and sramb */
		REG32(PWRCTRL_MEM_PD2) = 0x0;
	}

	// In a55 boot code
	REG32(PWRCTRL_ACCESS_CTRL) = 0x0;  // default access pwrctrl
}

void vCLK_suspend(uint32_t st_f)
{
	if (st_f) {
		/* close mem_pd of srama and sramb */
		REG32(PWRCTRL_MEM_PD2) = 0xfffffffc;
		/* poweroff wrapper */
		power_switch_to_wraper(PWR_OFF);
	}

	udelay(2000);
	/* switch to RTC pll */
	set_sys_div_clk(6, 3);  // rtc pll (30.72MHz)
	set_axi_div_clk(6, 0);  // rtc pll (122.88MHz)
	alt_timebase(1);  // 1us/10us/100us/1ms/xtal3 = 1 us tick  30.72/30

	udelay(2000);

	if (REG32(SYSCTRL_DEBUG_REG6) != 0)
		printf("[AOCPU]:WARNING SYSCTRL_DEBUG_REG6's value is :0x%x!\n",
			REG32(SYSCTRL_DEBUG_REG6));
	REG32(SYSCTRL_DEBUG_REG6) = WAIT_SWITCH_TO_RTC_PLL;
	vTaskDelay(pdMS_TO_TICKS(90));
	/* power off osc_clk */
	REG32(CLKCTRL_SYSOSCIN_CTRL) = 0;
	oscin_ctrl_reg = REG32(CLKCTRL_OSCIN_CTRL);
	REG32(CLKCTRL_OSCIN_CTRL) = 0;

	printf("[AOCPU]: running at 30.72MHz, 24MHz osc clk power off.\n");
	udelay(9000);

	disable_pll(PLL_FIX);
	disable_pll(PLL_GP0);
	disable_pll(PLL_GP1);
	disable_pll(PLL_HIFI);
}

