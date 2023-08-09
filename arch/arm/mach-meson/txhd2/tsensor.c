// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <asm/io.h>
#include <command.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/mailbox.h>
#include <asm/arch/tsensor.h>
#include <asm/arch/bl31_apis.h>
#include <linux/arm-smccc.h>
#include <asm/arch/pwr_ctrl.h>

#define READ_NUM 2

int r1p1_codetotemp(unsigned long value, unsigned int u_efuse,
			int ts_b, int ts_a, int ts_m, int ts_n)
{
	int64_t temp;

	/* T = 727.8*(u_real+u_efuse/(1<<16)) - 274.7 */
	/* u_readl = (5.05*YOUT)/((1<<16)+ 4.05*YOUT) */
	temp = (value * ts_m) * (1 << 16) / (100 * (1 << 16) + ts_n * value);
	if (u_efuse & 0x8000)
		temp = ((temp - (u_efuse & 0x7fff)) * ts_a / (1 << 16) - ts_b) / 10;
	else
		temp = ((temp + (u_efuse & 0x7fff)) * ts_a / (1 << 16) - ts_b) / 10;

	return temp;
}

int temp_read_entry(void)
{
	unsigned int ret, ver, u_efuse, regdata;
	unsigned int value_ts, value_all_ts;
	int tmp = -1;
	int i, ts_a, ts_b, ts_m, ts_n, cnt;
	char buf[2];

	ret = readl(AO_SEC_GP_CFG10);
	ver = (ret >> 24) & 0xff;
	if ((ver & 0x80) == 0) {
		printf("tsensor no trimmed, ver:0x%x\n", ver);
		return -1;
	}
	ts_b = 2610;
	ts_a = 7243;
	ts_m = 461;
	ts_n = 361;
	/*enable thermal1*/
	regdata = 0x62b;/*enable control*/
	writel(regdata, TS_PLL_CFG_REG1);
	regdata = 0x130;/*enable tsclk*/
	writel(regdata, HHI_TS_CLK_CNTL);
	ret = readl(AO_SEC_GP_CFG10);/*thermal1 cali data in reg CFG10*/
	mdelay(5);
	buf[0] = (ret) & 0xff;
	buf[1] = (ret >> 8) & 0xff;
	u_efuse = buf[1];
	u_efuse = (u_efuse << 8) | buf[0];
	value_ts = 0;
	value_all_ts = 0;
	cnt = 0;
	for (i = 0; i <= 10; i++) {
		udelay(50);
		value_ts = readl(TS_PLL_STAT0) & 0xffff;
	}
	for (i = 0; i < READ_NUM; i++) {
		udelay(4500);
		value_ts = readl(TS_PLL_STAT0) & 0xffff;
		if (value_ts >= 0x1500 && value_ts <= 0x3500) {
			value_all_ts += value_ts;
			cnt++;
		}
	}
	if (cnt) {
		value_ts =  value_all_ts / cnt;
	} else {
		value_ts = 0;
		printf("tsensor read temp cnt is 0\n");
	}
	printf("pll tsensor avg: 0x%x, u_efuse: 0x%x\n", value_ts, u_efuse);
	if (value_ts == 0) {
		printf("tsensor read temp is zero\n");
		return -1;
	}
	tmp = r1p1_codetotemp(value_ts, u_efuse, ts_b, ts_a, ts_m, ts_n);
	printf("temp1: %d\n", tmp);

	return 0;
}

int temp_cooling_entry(void)
{
	printf("not support\n");
	return 0;
}

int temp_trim_entry(int tempbase, int tempver)
{
	printf("not support\n");
	return -1;
}

