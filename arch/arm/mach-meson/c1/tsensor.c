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

int tsensor_tz_calibration(unsigned int type, unsigned int data)
{
	int ret;

	register long x0 asm("x0") = TSENSOR_CALI_SET;
	register long x1 asm("x1") = type;
	register long x2 asm("x2") = data;
	register long x3 asm("x3") = 0;
	do {
		asm volatile(
			__asmeq("%0", "x0")
			__asmeq("%1", "x1")
			__asmeq("%2", "x2")
			__asmeq("%3", "x3")
			"smc	#0\n"
			: "+r" (x0)
			: "r" (x1), "r" (x2), "r" (x3));
	} while (0);
	ret = x0;

	if (!ret)
		return -1;
	else
		return 0;
}

int r1p1_codetotemp(unsigned long value, unsigned int u_efuse)
{
	int64_t temp;

	temp = (value * ts_m) * (1 << 16) / (100 * (1 << 16) + ts_n * value);
	if (u_efuse & 0x8000) {
		temp = ((temp - (u_efuse & 0x7fff)) * ts_a / (1 << 16) - ts_b) / 10;
	} else {
		temp = ((temp + (u_efuse & 0x7fff)) * ts_a / (1 << 16) - ts_b) / 10;
	}
	return temp;
}

int r1p1_temp_read(int type)
{
	unsigned int ret, u_efuse;
	unsigned int value_ts, value_all_ts;
	int tmp = -1;
	int i, cnt;
	char buf[2];

	switch (type) {
		case 1:
			/*enable thermal1*/
			writel(T_CONTROL_DATA, TS_CFG_REG1);
			writel(T_TSCLK_DATA, CLKTREE_TS_CLK_CTRL);
			ret = readl(SYSCTRL_SEC_STATUS_REG13);/*thermal1 cali data in reg CFG10*/
			mdelay(5);
			buf[0] = (ret) & 0xff;
			buf[1] = (ret >> 8) & 0xff;
			u_efuse = buf[1];
			u_efuse = (u_efuse << 8) | buf[0];
			value_ts = 0;
			value_all_ts = 0;
			cnt = 0;
			for (i = 0; i <= 10; i ++) {
				udelay(50);
				value_ts = readl(TS_STAT0) & 0xffff;
			}
			for (i = 0; i <= T_AVG_NUM; i ++) {
				udelay(T_DLY_TIME);
				value_ts = readl(TS_STAT0) & 0xffff;
				if ((value_ts >= T_VALUE_MIN) &&
						(value_ts <= T_VALUE_MAX)) {
					value_all_ts += value_ts;
					cnt ++;
				}
			}
			value_ts =  value_all_ts / cnt;
			printf("tsensor avg: 0x%x, u_efuse: 0x%x\n", value_ts, u_efuse);
			if (value_ts == 0) {
				printf("tsensor read temp is zero\n");
				return -1;
			}
			tmp = r1p1_codetotemp(value_ts, u_efuse);
			printf("temp1: %d\n", tmp);
			break;
		default:
			printf("r1p1 tsensor trim type not support\n");
			return -1;
		}
	return tmp;
}

unsigned int r1p1_temptocode(unsigned long value, int tempbase)
{
	unsigned long tmp1, tmp2, u_efuse, signbit;

	printf("a b m n: %d, %d, %d, %d\n", ts_a, ts_b, ts_m, ts_n);
	tmp1 = ((tempbase * 10 + ts_b) * (1 << 16)) / ts_a; /*ideal u*/
	printf("%s : tmp1: 0x%lx\n", __func__, tmp1);
	tmp2 = (ts_m * value * (1 << 16)) / ((1 << 16) * 100 + ts_n * value);
	printf("%s : tmp2: 0x%lx\n", __func__, tmp2);
	signbit = ((tmp1 > tmp2) ? 0 : 1);
	u_efuse = (tmp1 > tmp2) ? (tmp1 - tmp2) : (tmp2 - tmp1);
	u_efuse = (signbit << 15) | u_efuse;
	return u_efuse;
}

int r1p1_temp_trim(int tempbase, int tempver, int type)
{
	unsigned int u_efuse, index_ts, index_ver;
	unsigned int value_ts, value_all_ts;
	int i, cnt;

	printf("r1p1 temp trim type: 0x%x\n", type);
	switch (type) {
		case 0:
			index_ver = 5;
			if (tsensor_tz_calibration(index_ver, tempver) < 0)
				printf("version tsensor thermal_calibration send error\n");
		break;
		case 1:
			value_ts = 0;
			value_all_ts = 0;
			index_ts = 6;
			cnt = 0;
			/*enable thermal1*/
			writel(T_CONTROL_DATA, TS_CFG_REG1);
			writel(T_TSCLK_DATA, CLKTREE_TS_CLK_CTRL);
			for (i = 0; i <= 10; i ++) {
				udelay(50);
				value_ts = readl(TS_STAT0) & 0xffff;
			}
			for (i = 0; i <= T_AVG_NUM; i ++) {
				udelay(T_DLY_TIME);
				value_ts = readl(TS_STAT0) & 0xffff;
				printf("tsensor read: 0x%x\n", value_ts);
				if ((value_ts >= T_VALUE_MIN) &&
						(value_ts <= T_VALUE_MAX)) {
					value_all_ts += value_ts;
					cnt ++;
				}
			}
			value_ts =  value_all_ts / cnt;
			printf("tsensor avg: 0x%x\n", value_ts);
			if (value_ts == 0) {
				printf("tsensor read temp is zero\n");
				return -1;
			}
			u_efuse = r1p1_temptocode(value_ts, tempbase);
			printf("ts efuse:%d\n", u_efuse);
			printf("ts efuse:0x%x, index: %d\n", u_efuse, index_ts);
			if (tsensor_tz_calibration(index_ts, u_efuse) < 0) {
				printf("tsensor thermal_calibration send error\n");
				return -1;
			}
			break;
		default:
			printf("r1p1 tsensor trim type not support\n");
			return -1;
			break;
	}
	return 0;
}

int temp_read_entry(void)
{
	unsigned int ret, ver;

	ret = readl(SYSCTRL_SEC_STATUS_REG13);
	ver = (ret >> 24) & 0xff;
	if (0 == (ver & T_VER_MASK)) {
		printf("tsensor no trimmed: calidata:0x%x\n",
			readl(SYSCTRL_SEC_STATUS_REG13));
		return -1;
	}
	ret = (ver & 0xf) >> 2;
	switch (ret) {
		case 0x2:
			printf("temp type no support\n");
		break;
		case 0x0:
			printf("temp type no support\n");
		break;
		case 0x1:
			r1p1_temp_read(1);
			printf("read the thermal\n");
		break;
		case 0x3:
			printf("temp type no support\n");
			return -1;
		break;
		default:
			printf("thermal version not support!!!Please check!\n");
			return -1;
		}
	return 0;
}

int temp_trim_entry(int tempbase, int tempver)
{
	unsigned int ret, ver;

	ret = readl(SYSCTRL_SEC_STATUS_REG13);
	ver = (ret >> 24) & 0xff;
	if (ver & T_VER_MASK) {
		printf("tsensor trimmed: cali data: 0x%x\n",
			readl(SYSCTRL_SEC_STATUS_REG13));
		return -1;
	}

	printf("tsensor input trim tempver, tempver:0x%x\n", tempver);
	switch (tempver) {
		case 0x84:
			r1p1_temp_trim(tempbase, tempver, 1);
			r1p1_temp_trim(tempbase, tempver, 0);
			printf("triming the thermal by bbt-sw\n");
		break;
		case 0x85:
			r1p1_temp_trim(tempbase, tempver, 1);
			r1p1_temp_trim(tempbase, tempver, 0);
			printf("triming the thermal by bbt-ops\n");
		break;
		case 0x87:
			r1p1_temp_trim(tempbase, tempver, 1);
			r1p1_temp_trim(tempbase, tempver, 0);
			printf("triming the thermal by slt\n");
		break;
		default:
			printf("thermal version not support!!!Please check!\n");
			return -1;
		}
	return 0;
}

int temp_cooling_entry(void)
{
#ifdef CONFIG_AML_TSENSOR_COOL
	int temp;

	while (1) {
		temp = r1p1_temp_read(1);
		if (temp <= CONFIG_HIGH_TEMP_COOL) {
			printf("device cool done\n");
			break;
		}
		mdelay(2000);
		printf("warning: temp %d over %d, cooling\n", temp,
			CONFIG_HIGH_TEMP_COOL);
	}
#endif
	return 0;
}
