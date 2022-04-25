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
 *//*
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
#include "common.h"
#include "register.h"
#include "FreeRTOS.h"
#include "task.h"
#include "soc.h"
#include "suspend.h"


/* io defines */
#define wr_reg(addr, val)					(*((volatile uint32_t *)(addr)))  = (val)
#define rd_reg(addr)						(*((volatile uint32_t *)(addr)))

/*clear [mask] 0 bits in [addr], set these 0 bits with [value] corresponding bits*/
#define modify_reg(addr, value, mask)       wr_reg(addr, ((rd_reg(addr) & (mask)) | (value)))
#define wait_set(addr, loc)                 do {} while (0 == (rd_reg(addr) & (1 << loc)));
#define wait_clr(addr, loc)                 do {} while (1 == (rd_reg(addr) & (1 << loc)));
#define wait_equal(addr, data)              do {} while (data != (rd_reg(addr)));

#define _udelay(tim)                        vTaskDelay(tim)

unsigned int g_nAPDSet = 0;
#if 0
void vDDR_suspend(uint32_t st_f)
{
	//printf("aml log : DDR suspend...dummy\n");
	//return;

	printf("aml log : DDR suspend...1");

	g_nAPDSet = rd_reg(DMC_DRAM_APD_CTRL);
	wr_reg(DMC_DRAM_APD_CTRL,0);

#ifdef CHECK_DMC_IDLE
	while ((((rd_reg(DMC_DRAM_STAT)) & 0xf0) != 0) &&
		(((rd_reg(DMC_DRAM_STAT)) & 0xf0) != 0x40)) {
	}
#endif

#if 0
	wr_reg(DMC_DRAM_SCFG, 2);
	vTaskDelay(pdMS_TO_TICKS(1));
	while (((((rd_reg(DMC_DRAM_STAT)) >> 4) & 0xf) != 3)) {
	}
#endif

	wr_reg(DMC_DRAM_DFIINITCFG, (1 | (1 << 8)));

	wait_clr(DMC_DRAM_DFIINITCFG, 31);

	vTaskDelay(pdMS_TO_TICKS(1));
	//wr_reg(AM_DDR_PLL_CNTL0,
	//	(rd_reg(AM_DDR_PLL_CNTL0) & (~(0xf << 28))) | (1 << 29));

	printf("done!!\n");

	vTaskDelay(pdMS_TO_TICKS(1000));

	//vDDR_resume(0);

	printf("\naml log : DMC_DRAM_STAT=0x%x",rd_reg(DMC_DRAM_STAT));
	printf("\naml log : DMC_DRAM_DFIINITCFG=0x%x\n",rd_reg(DMC_DRAM_DFIINITCFG));

}

void vDDR_resume(uint32_t st_f)
{
	printf("aml log : DDR resume...2");

	//printf("aml log : DDR resume...dummy\n");
	//return;

	#if 0
	do {
		wr_reg(AM_DDR_PLL_CNTL0, (rd_reg(AM_DDR_PLL_CNTL0) & (~(0xf << 28))) | (0xc << 28));
		vTaskDelay(pdMS_TO_TICKS(1));
		wr_reg(AM_DDR_PLL_CNTL0, (rd_reg(AM_DDR_PLL_CNTL0) & (~(0xf << 28))) | (0xd << 28));
		vTaskDelay(pdMS_TO_TICKS(5));
		wr_reg(AM_DDR_PLL_CNTL0, (rd_reg(AM_DDR_PLL_CNTL0) & (~(0xf << 28))) | (0x5 << 28));
		vTaskDelay(pdMS_TO_TICKS(10));
		wr_reg(AM_DDR_PLL_CNTL0, (rd_reg(AM_DDR_PLL_CNTL0) & (~(0xf << 28))) | (0x7 << 28));
		vTaskDelay(pdMS_TO_TICKS(100));
	} while ((0 == ((rd_reg(AM_DDR_PLL_STS) >> 31) & 0x1)));
	wr_reg(DDR_CLK_CNTL, 0x10000000);
	wr_reg(DDR_CLK_CNTL, 0xb0000007);

	vTaskDelay(pdMS_TO_TICKS(1));
	#endif


	wr_reg(DMC_DRAM_DFIINITCFG, (0 | (1 << 8)));
	vTaskDelay(pdMS_TO_TICKS(1));

	printf("-1");

	wait_set(DMC_DRAM_DFIINITCFG, 31);
	vTaskDelay(pdMS_TO_TICKS(10)); //extra 10us for vt
#if 0
	wr_reg(DMC_DRAM_SCFG, 4);
#endif
	vTaskDelay(pdMS_TO_TICKS(1));

	printf("-2");

	int nLoopFlag = 1;
	do {

		switch ((rd_reg(DMC_DRAM_STAT)) & 0xf0)
		{
		case 0:    //DRAM IDLE
		case 0x20: //DRAM ACCESS
		case 0x40: //DRAM APD
		nLoopFlag = 0;
		break;
		default: break;
		}
	} while(nLoopFlag);

	vTaskDelay(pdMS_TO_TICKS(30));

	printf("-3");

	printf("done\n");

	printf("\naml log : DMC_DRAM_STAT=0x%x",rd_reg(DMC_DRAM_STAT));
	printf("\naml log : DMC_DRAM_DFIINITCFG=0x%x\n",rd_reg(DMC_DRAM_DFIINITCFG));

}
#endif

/*
#define PATTERN_MATRIX_X   (3 + 32 + 16 + 17)     //68*32==2176 ///2.2k -0x880-1 loop
#define PATTERN_MATRIX_Y  (32)
#define PATTERN_MATRIX_LOOP_SIZE   ((PATTERN_MATRIX_X)*(PATTERN_MATRIX_Y) * 4)
unsigned int cpu_ddr_test_init_pattern_generater(uint32_t martix_x_select, uint32_t martix_y_select)
{
	unsigned int pattern_value = 0;
	unsigned int pattern_value_temp_16 = 0;

	martix_x_select = (martix_x_select % PATTERN_MATRIX_X);
	{
		{
			{
				if (martix_x_select == 0)
					pattern_value = 0xaaaa5555;  //for 16 bit bus pattern

				if (martix_x_select == 1)
					pattern_value = 0x0000ffff;  //for 16 bit bus pattern

				if (martix_x_select == 2)
					pattern_value = 0;

				if ((martix_x_select > 2) && (martix_x_select < (3 + 32)))
					pattern_value = 1 << (martix_x_select - 3);
				if ((martix_x_select > (2 + 32)) && (martix_x_select < (3 + 32 + 16))) { //for 16 bit bus pattern
					pattern_value_temp_16 = (1 << (martix_x_select - 3 - 32));
					pattern_value = pattern_value_temp_16 | ((~pattern_value_temp_16) << 16);
				}
				if ((martix_x_select > (2 + 32 + 16)) && (martix_x_select < (3 + 32 + 16 + 17))) { //for dbi bus pattern  17 group
					pattern_value_temp_16 = (0x0f0f + 0xf0f * (martix_x_select - 3 - 32 - 16));
					pattern_value = pattern_value_temp_16 | ((~pattern_value_temp_16) << 16);
				}
			}
			if ((martix_y_select % 2))
				pattern_value = ~pattern_value;
			if ((martix_y_select % ((PATTERN_MATRIX_Y) / 2)) == (((PATTERN_MATRIX_Y * 1) / 8) - 1)) //for dbi pattern walk 0 and walk 1
				pattern_value = 0;                                                              //insert for dbi pattern jiaxing 20190117
			if ((martix_y_select % ((PATTERN_MATRIX_Y) / 2)) == (((PATTERN_MATRIX_Y * 3) / 8) - 1)) //for dbi pattern walk 0 and walk 1
				pattern_value = ~0;                                                             //insert for dbi pattern jiaxing 20190117
		}
	}
	return pattern_value;
}
*/
#if 0
static unsigned int dmc_ddr_test(unsigned int start_add, unsigned int write_enable, unsigned int read_enable, unsigned int read_compare, unsigned int test_end_add, unsigned int pattern, unsigned int seed)
{
	seed=2;
#define DATA_LOOP_PATTERN_INDEX  4 + 32        //0xff
	unsigned int dmc_test_sts = 0;
	unsigned int dmc_error = 0;
	//unsigned int pattern_select = 0;
	unsigned int pattern_value = 0;
	unsigned int pattern_inv_value = 0;

	dmc_error = 0;
	{
		test_end_add = test_end_add - 4;        //sha must bit 0-6 ==ff;
	}
	wr_reg(DMC_TEST_STA, start_add);                // RESET FIFOS  //0x03d81e3f
	wr_reg(DMC_TEST_EDA, test_end_add);             //0x07d81e3f
	if (pattern == 0) {
		wr_reg(DMC_TEST_WD0, 0xaa5555aa);
		wr_reg(DMC_TEST_WD1, 0x55aaaa55);
		wr_reg(DMC_TEST_WD2, 0);
		wr_reg(DMC_TEST_WD3, 0xffffffff);
		wr_reg(DMC_TEST_WD4, 0);
		wr_reg(DMC_TEST_WD5, 0x0000ffff);
		wr_reg(DMC_TEST_WD6, 0xffff0000);
		wr_reg(DMC_TEST_WD7, 0x33cccc33);
		wr_reg(DMC_TEST_WD0 + 32, 0xaa5555aa);
		wr_reg(DMC_TEST_WD1 + 32, 0x55aaaa55);
		wr_reg(DMC_TEST_WD2 + 32, 0);
		wr_reg(DMC_TEST_WD3 + 32, 0xffffffff);
		wr_reg(DMC_TEST_WD4 + 32, 0);
		wr_reg(DMC_TEST_WD5 + 32, 0x0000ffff);
		wr_reg(DMC_TEST_WD6 + 32, 0xffff0000);
		wr_reg(DMC_TEST_WD7 + 32, 0x33cccc33);
	} else if (pattern < 33) {
		wr_reg(DMC_TEST_WD0, ((1 << (pattern - 1)) + seed));
		wr_reg(DMC_TEST_WD1, ((2 << (pattern - 1)) + seed));
		wr_reg(DMC_TEST_WD2, ((3 << (pattern - 1)) + seed));
		wr_reg(DMC_TEST_WD3, ((4 << (pattern - 1)) + seed));
		wr_reg(DMC_TEST_WD4, ((4 << (pattern - 1))));
		wr_reg(DMC_TEST_WD5, ((4 << (pattern - 1))));
		wr_reg(DMC_TEST_WD6, ((4 << (pattern - 1))));
		wr_reg(DMC_TEST_WD7, ((4 << (pattern - 1))));
		if (pattern > 16) {
			wr_reg(DMC_TEST_WD4, ((4 << (pattern - 1)) + 0x01010101));
			wr_reg(DMC_TEST_WD5, ((4 << (pattern - 1)) + 0x01010101));
			wr_reg(DMC_TEST_WD6, ((4 << (pattern - 1)) + 0x01010101));
			wr_reg(DMC_TEST_WD7, ((4 << (pattern - 1)) + 0x01010101));
		} else if (pattern > 1) {
			wr_reg(DMC_TEST_WD4, ((4 << (pattern - 1)) + 0x01010101));
			wr_reg(DMC_TEST_WD5, ((4 << (pattern - 1)) + 0x01010101));
			wr_reg(DMC_TEST_WD6, ((4 << (pattern - 1)) + 0x01010101));
			wr_reg(DMC_TEST_WD7, ((4 << (pattern - 1)) + 0x01010101));
		}
	} else {
		//pattern_select = pattern - 33;
		pattern_value =1;// cpu_ddr_test_init_pattern_generater(pattern_select, 0);
		pattern_inv_value = ~pattern_value;
		for (char counter = 0; counter < 32; ) {
			wr_reg((DMC_TEST_WD0 + counter), pattern_value);
			counter = counter + 4;
			wr_reg((DMC_TEST_WD0 + counter), pattern_inv_value);
			if (counter == 16)
				wr_reg((DMC_TEST_WD0 + counter), 0);
			counter = counter + 4;
		}
		for (char counter = 0; counter < 32; ) { //for g12b-revb register not continuous
			wr_reg((DMC_TEST_WD8 + counter), pattern_value);
			counter = counter + 4;
			wr_reg((DMC_TEST_WD8 + counter), pattern_inv_value);
			if (counter == 16)
				wr_reg((DMC_TEST_WD0 + counter), ~0);
			counter = counter + 4;
		}
	}


	wr_reg(DMC_TEST_STS, 0x8000001f);                       //must clear watchdog and done flag jiaxing debug 2016_12_07
	wr_reg(DMC_TEST_WDG, 0xf000f000);                       //wdg should >rfc value ,use dmc clk count.
	if ((pattern == 1) || ((pattern < 33) && pattern))      //should repeat 1 times for read ,all will fail when data increase jiaxing debug 20180322
		wr_reg(DMC_TEST_CTRL, (1 << 31) | (read_compare << 27) | (0 << 28) | (0 << 25) | (1 << 24) | (0 << 20) | (1 << 23) | (0x0 << 16) | (0 << 8) | (0x428) | (3 << 18) | (write_enable << 30) | (read_enable << 29));
	else
		wr_reg(DMC_TEST_CTRL, (1 << 31) | (read_compare << 27) | (0 << 28) | (1 << 24) | (0x0 << 16) | (0 << 18) | (0x0 << 0) | (0 << 8) | (0x428) | (3 << 18) | (write_enable << 30) | (read_enable << 29));

	do
		//_udelay(1);
		dmc_test_sts = (rd_reg(DMC_TEST_STS));
	while (!(dmc_test_sts & 0xc0000000));

	wr_reg(DMC_TEST_CTRL, 0x00000000);

	if ((dmc_test_sts & 0x40000000))
		dmc_error = 1;
	else
	if (dmc_test_sts & 0x40000001)         //can not deter write triger ,or can not guickly recover dmc with phy? 2016_12_12
		dmc_error = 1;

	dmc_error = dmc_error + (rd_reg(DMC_TEST_ERR_CNT));
	wr_reg(DMC_TEST_STS, 0x8000001f); //must clear watchdog and done flag jiaxing debug 2016_12_07

	if (dmc_error) {
		for (unsigned int counter1 = 0; counter1 < (DMC_TEST_RDRSP_ADDR+4-DMC_TEST_STA); )
		{
		printf( "\ncounter %08x %08x",counter1,(rd_reg(DMC_TEST_STA+(counter1))));
		counter1=counter1+4;
		}
		wr_reg(DMC_SOFT_RST, (rd_reg(DMC_SOFT_RST)) & (~((1 << 29) | (1 << 24))));  //clear read buffer dmc test reset
		vTaskDelay(pdMS_TO_TICKS(1));
		wr_reg(DMC_SOFT_RST, (rd_reg(DMC_SOFT_RST)) | ((1 << 29)) | (1 << 24));
		vTaskDelay(pdMS_TO_TICKS(1));
	}
	return dmc_error;
}

unsigned int apb_sec_ctrl = 0;
#define DDR_APB_SEC_CTRL                           ((0x00f0  << 2) + 0xff639000)
#endif
#if 0 //def CFG_ENABLE_DDR_SUSPEND_TEST
static void ddr_suspend_resume_test(uint32_t test_size, uint32_t test_delay_time_ms, uint32_t test_write_loops, uint32_t test_read_loops, uint32_t test_skip_suspend, uint32_t p_dev)
{
	//return;
	uint32_t lock_cnt = 100;
	uint32_t apd_value = 0;
	p_dev = p_dev;
	printf( "enter suspend_n_debug\n");
	apd_value = rd_reg(DMC_DRAM_APD_CTRL);
	wr_reg(DMC_DRAM_APD_CTRL, 0);

	//watchdog_disable();
	printf( "test_size=%08x test_delay_time_ms=%d test_write_loops=%d test_read_loops=%d", \
		     test_size, test_delay_time_ms, test_write_loops, test_read_loops);
	printf( "enter suspend111\n");

#if 1 //def CFG_ENABLE_DDR_DMC_TEST
	uint64_t dram_size = 0, dram_base = 0;
	dram_base = 0x0000000;                                                      // p_ddrs->ddr_base_addr;
	//dram_size = 1024;//p_ddrs->cfg_board_common_setting.dram_cs0_size_MB + p_ddrs->cfg_board_common_setting.dram_cs1_size_MB;
	dram_size =1024;

	if (!test_size)
		test_size = (dram_size << 20) - 4;

	//if (!test_delay_time_ms)
	//	test_delay_time_ms = 3000;

	if (!test_write_loops)
		test_write_loops = 1;

	if (!test_read_loops)
		test_read_loops = 1;
	uint32_t ddr_bist_test_error = 0;
#endif

	while ((test_write_loops) || (test_read_loops)) {
		if (test_write_loops)
			ddr_bist_test_error = dmc_ddr_test(dram_base, 1, 0, 0, test_size, 1, 0) + ddr_bist_test_error;
printf( "enter suspend113\n");
		if (!test_skip_suspend) {
#if  0 //def CHECK_DMC_IDLE
			while ((((rd_reg(DMC_DRAM_STAT)) & 0xf0) != 0) && (((rd_reg(DMC_DRAM_STAT)) & 0xf0) != 0x40)) {
			}
#endif
#if 0
			wr_reg(DMC_DRAM_SCFG, 2);
			vTaskDelay(pdMS_TO_TICKS(1));

			while (((((rd_reg(DMC_DRAM_STAT)) >> 4) & 0xf) != 3)) {
			}
#endif
//printf( "enter suspend114\n");
			wr_reg(DMC_DRAM_DFIINITCFG, (1 | (0 << 1) | (0 << 6) | (0 << 14) | (1 << 8)));
			vTaskDelay(pdMS_TO_TICKS(1));
			wait_clr(DMC_DRAM_DFIINITCFG, 31);                                                                                                                                     //final version, wait_clr
			vTaskDelay(pdMS_TO_TICKS(3));
		//	printf( "enter suspend115\n");
			wr_reg(AM_DDR_PLL_CNTL0, (rd_reg(AM_DDR_PLL_CNTL0) & (~(0xf << 28))) | (1 << 29));
		}
		//_udelay(test_delay_time_ms * 1000);
		vTaskDelay(test_delay_time_ms * pdMS_TO_TICKS(1000));
		//_udelay( 1000);
printf( "enter suspend111..wait\n");
		if (!test_skip_suspend) {
			printf("enter resume\n");

			do {
				wr_reg(AM_DDR_PLL_CNTL0, (rd_reg(AM_DDR_PLL_CNTL0) & (~(0xf << 28))) | (1 << 29));
				vTaskDelay(pdMS_TO_TICKS(1));
				wr_reg(AM_DDR_PLL_CNTL0, (rd_reg(AM_DDR_PLL_CNTL0) & (~(0x1 << 29))) | (1 << 28));

				vTaskDelay(pdMS_TO_TICKS(200));                                                                                                                                                                                                          //must wait some time than to read
			} while ((0 == ((rd_reg(AM_DDR_PLL_CNTL0) >> 31) & 0x1)) && (lock_cnt--));

			printf( "pll relock ok\n");                                                                                                                                         //need extra delay
			vTaskDelay(pdMS_TO_TICKS(1));
			wr_reg(DMC_DRAM_DFIINITCFG, (0 | (0 << 1) | (0 << 6) | (0 << 14) | (1 << 8)));
			vTaskDelay(pdMS_TO_TICKS(1));
			wait_set(DMC_DRAM_DFIINITCFG, 31);
			vTaskDelay(pdMS_TO_TICKS(100));                                                                                                                                    //extra 10us for vt
		}
#if 1 //def CFG_ENABLE_DDR_DMC_TEST
		if (test_read_loops)
			ddr_bist_test_error = dmc_ddr_test(dram_base, 0, 1, 1, test_size, 1, 0) + ddr_bist_test_error;
		 //if (ddr_bist_test_error)
		printf( "dmc full test result = %d\n", ddr_bist_test_error);
#endif

		if (test_write_loops)
			test_write_loops--;

		if (test_read_loops)
			test_read_loops--;
	}
	wr_reg(DMC_DRAM_APD_CTRL, apd_value);

	//printf("end resume test11\n");
} /* ddr_suspend_resume_test */
#endif
	//#define DDR_SUSPEND_MODE_DMC_TRIGGER_SUSPEND_1   1
	#define DDR_SUSPEND_MODE_MANUAL_TRIGGER_DFI_INIT_START   2
void vDDR_suspend(uint32_t st_f)
{
	//printf("aml log : DDR suspend...dummy\n");
	//return;

	st_f = st_f;
	//unsigned int time_start, time_end;
	printf("Enter ddr suspend\n");

	//return ;

	while (0xfffffff != rd_reg(DMC_CHAN_STS)) {
		printf("DMC_CHAN_STS: 0x%x\n", rd_reg(DMC_CHAN_STS));
		vTaskDelay(pdMS_TO_TICKS(100000));
	}

	//time_start = rd_reg(P_ISA_TIMERE);

	/* open DMC reg access for M3 */
	//apb_sec_ctrl = rd_reg(DDR_APB_SEC_CTRL);
	//wr_reg(DDR_APB_SEC_CTRL,0x91911);

	wr_reg(DMC_REQ_CTRL, 0); //bit0: A53.
	_udelay(1);

	/* suspend flow */
	while ((((rd_reg(DMC_DRAM_STAT))&0xf0) != 0) && (((rd_reg(DMC_DRAM_STAT))&0xf0) != 0x40)) {
	//	printf("DMC_DRAM_STAT11: 0x%x\n", rd_reg(DMC_DRAM_STAT));
		vTaskDelay(pdMS_TO_TICKS(1));
	}
#ifdef DDR_SUSPEND_MODE_DMC_TRIGGER_SUSPEND_1
	wr_reg(DMC_DRAM_ASR_CTRL,(1<<18));  //bit 18 will auto trigger dfi init start cmd when scfg set to value 2
	wr_reg(DMC_DRAM_SCFG, 2);
	while (((((rd_reg(DMC_DRAM_STAT))>>4)&0xf) != 3)) {
		//printf("DMC_DRAM_STAT22: 0x%x\n", readl(DMC_DRAM_STAT));
		//_udelay(1);//do not add any delay,since use ao cpu maybe speed too slow
	}

#endif

#ifdef DDR_SUSPEND_MODE_MANUAL_TRIGGER_DFI_INIT_START
	wr_reg(DMC_DRAM_SCFG, 1);
	while (((((rd_reg(DMC_DRAM_STAT))>>4)&0xf) != 1)) {
		//printf("DMC_DRAM_STAT22: 0x%x\n", readl(DMC_DRAM_STAT));
		//_udelay(1);//do not add any delay,since use ao cpu maybe speed too slow
	}

			wr_reg(DMC_DRAM_DFIINITCFG, (1 | (0 << 1) | (0 << 6) | (0 << 14) | (1 << 8)));
			vTaskDelay(pdMS_TO_TICKS(1));
			wait_clr(DMC_DRAM_DFIINITCFG, 31);
#endif                                                                                                                                  //final version, wait_clr
			vTaskDelay(pdMS_TO_TICKS(3));
			wr_reg(AM_DDR_PLL_CNTL0, (rd_reg(AM_DDR_PLL_CNTL0) & (~(0xf << 28))) | (1 << 29));


	/* print time consumption */
	//time_end = rd_reg(P_ISA_TIMERE);
	//printf("ddr suspend time: %dus\n", time_end - time_start);
	printf("\nddr suspend is done\n");
	//ddr_suspend_resume_test((1024<<20), 100, 3, 3, 0, 0);
	//ddr_suspend_resume_test((80<<20), 10000000, 0, 3, 0, 0);
}

static unsigned int pll_lock(void) {
	unsigned int lock_cnt = 100;
	do {
		wr_reg(AM_DDR_PLL_CNTL0, (rd_reg(AM_DDR_PLL_CNTL0) & (~(0xf<<28))) | (1<<29));
		vTaskDelay(pdMS_TO_TICKS(1));
		wr_reg(AM_DDR_PLL_CNTL0, (rd_reg(AM_DDR_PLL_CNTL0) & (~(0x1<<29))) | (1<<28));
		vTaskDelay(pdMS_TO_TICKS(200));
	} while((0 == ((rd_reg(AM_DDR_PLL_CNTL0) >> 31) & 0x1)) && (lock_cnt--));
	return lock_cnt;
}

void vDDR_resume(uint32_t st_f)
{
	//unsigned int time_start, time_end;
	unsigned int ret = 0;

	st_f = st_f;
	printf("Enter ddr resume\n");


	//return;

	//time_start = rd_reg(P_ISA_TIMERE);
	/* resume flow */
	#if 1
	ret = pll_lock();
	if (!ret) {
		printf("ddr pll lock r1\n");
		wr_reg(AM_DDR_PLL_CNTL3, rd_reg(AM_DDR_PLL_CNTL3)|(1<<31));
		ret = pll_lock();
		if (!ret) {
			printf("ddr pll lock r2\n");
			wr_reg(AM_DDR_PLL_CNTL6, 0x55540000);
			ret = pll_lock();
			if (!ret) {
				printf("ddr pll lock r2\n");
				while (1) {};
			}
		}
	}
#endif

#ifdef DDR_SUSPEND_MODE_DMC_TRIGGER_SUSPEND_1

#endif

#ifdef DDR_SUSPEND_MODE_MANUAL_TRIGGER_DFI_INIT_START
			wr_reg(DMC_DRAM_DFIINITCFG, (0 | (0 << 1) | (0 << 6) | (0 << 14) | (1 << 8)));
			vTaskDelay(pdMS_TO_TICKS(1));
			wait_set(DMC_DRAM_DFIINITCFG, 31);
			vTaskDelay(pdMS_TO_TICKS(100));
#endif
	wr_reg(DMC_DRAM_SCFG, 4);
	while (((((rd_reg(DMC_DRAM_STAT))>>4)&0xf) != 2)) {
		//printf("DMC_DRAM_STAT22: 0x%x\n", readl(DMC_DRAM_STAT));
		//_udelay(1);//do not add any delay,since use ao cpu maybe speed too slow
	}

	#if 0
	wr_reg( DMC_DRAM_SCFG, 4);
	vTaskDelay(pdMS_TO_TICKS(1));
	#endif

	wr_reg(DMC_REQ_CTRL, 0xffffffff);
	//wr_reg(DDR_APB_SEC_CTRL, apb_sec_ctrl);
	/* print time consumption */
	//time_end = readl(P_ISA_TIMERE);
	//printf("ddr resume time: %dus\n", time_end - time_start);
	//	unsigned int ddr_bist_test_error = 0;
	//ddr_bist_test_error = dmc_ddr_test(dram_base, 0, 1, 1, test_size, 1, 0) + ddr_bist_test_error;
	//ddr_bist_test_error = dmc_ddr_test(0, 0, 1, 1, (80<<20), 1, 0) + ddr_bist_test_error;
	//printf("ddr_bist_test_error = %d\n", ddr_bist_test_error);
	//wr_reg(0xfe002440, 2);
//	wr_reg(0xfe002440, 0);
//	_udelay(300);
	//ddr_bist_test_error = dmc_ddr_test(0, 1, 0, 0, (1<<20), 1, 0) + ddr_bist_test_error;
	//ddr_bist_test_error = dmc_ddr_test(0, 0, 1, 1, (1<<20), 1, 0) + ddr_bist_test_error;
//	printf("ddr_bist_test_error = %d\n", ddr_bist_test_error);
	//ddr_suspend_resume_test((1<<20), 2, 1, 3, 0, 0);
	//ddr_suspend_resume_test((1<<20), 0, 1, 3, 0, 0);
	//_udelay(300);
//	wr_reg(DMC_REQ_CTRL, 0xffffffff);
	printf("ddr resume done\n");
}


