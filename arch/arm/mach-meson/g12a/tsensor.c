#include <common.h>
#include <asm/io.h>
#include <command.h>
#include <asm/arch/secure_apb.h>
#include <asm/arch/mailbox.h>
#include <asm/arch/tsensor.h>

int r1p1_codetotemp(unsigned long value, unsigned int u_efuse)
{
	int64_t temp;

	/* T = 727.8*(u_real+u_efuse/(1<<16)) - 274.7 */
	/* u_readl = (5.05*YOUT)/((1<<16)+ 4.05*YOUT) */
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
			writel(T_CONTROL_DATA, TS_PLL_CFG_REG1);
			writel(T_TSCLK_DATA, HHI_TS_CLK_CNTL);
			ret = readl(AO_SEC_GP_CFG10);/*thermal1 cali data in reg CFG10*/
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
				value_ts = readl(TS_PLL_STAT0) & 0xffff;
			}
			for (i = 0; i < T_READ_NUM; i ++) {
				udelay(T_DLY_TIME);
				value_ts = readl(TS_PLL_STAT0) & 0xffff;
				if ((value_ts >= T_VALUE_MIN) &&
						(value_ts <= T_VALUE_MAX)) {
					value_all_ts += value_ts;
					cnt ++;
				}
			}
			if (cnt) {
				value_ts =  value_all_ts / cnt;
			} else {
				value_ts = 0;
				printf("tsensor read temp cnt is zero");
			}
			printf("pll tsensor avg: 0x%x, u_efuse: 0x%x\n", value_ts, u_efuse);
			if (value_ts == 0) {
				printf("tsensor read temp is zero\n");
				return -1;
			}
			tmp = r1p1_codetotemp(value_ts, u_efuse);
			printf("temp1: %d\n", tmp);
			break;
		case 2:
			/*enable thermal2*/
			writel(T_CONTROL_DATA,TS_DDR_CFG_REG1);
			writel(T_TSCLK_DATA,HHI_TS_CLK_CNTL);
			mdelay(5);
			ret = readl(AO_SEC_SD_CFG12);/*thermal1 cali data in reg CFG10*/
			buf[0] = (ret) & 0xff;
			buf[1] = (ret >> 8) & 0xff;
			u_efuse = buf[1];
			u_efuse = (u_efuse << 8) | buf[0];
			value_ts = 0;
			value_all_ts = 0;
			cnt = 0;
			for (i = 0; i <= 10; i ++) {
				udelay(50);
				value_ts = readl(TS_DDR_STAT0) & 0xffff;
			}
			for (i = 0; i < T_READ_NUM; i ++) {
				udelay(T_DLY_TIME);
				value_ts = readl(TS_DDR_STAT0) & 0xffff;
				if ((value_ts >= T_VALUE_MIN) &&
						(value_ts <= T_VALUE_MAX)) {
					value_all_ts += value_ts;
					cnt ++;
				}
			}
			if (cnt) {
				value_ts =  value_all_ts / cnt;
			} else {
				value_ts = 0;
				printf("tsensor read temp cnt is zero");
			}
			printf("ddr tsensor avg: 0x%x, u_efuse: 0x%x\n", value_ts, u_efuse);
			if (value_ts == 0) {
				printf("tsensor read temp is zero\n");
				return -1;
			}
			tmp = r1p1_codetotemp(value_ts, u_efuse);
			printf("temp2: %d\n", tmp);
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
	/* T = (727.8*(5.05*Yout)/((1<<16)+4.05*Yout)) - 274.7 */
	/* u_efuse = u_ideal - u_real */
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
			if (thermal_calibration(index_ver, tempver) < 0)
				printf("version tsensor thermal_calibration send error\n");
		break;
		case 1:
			value_ts = 0;
			value_all_ts = 0;
			index_ts = 6;
			cnt = 0;
			/*enable thermal1*/
			writel(T_CONTROL_DATA, TS_PLL_CFG_REG1);
			writel(T_TSCLK_DATA, HHI_TS_CLK_CNTL);
			for (i = 0; i <= 10; i ++) {
				udelay(50);
				value_ts = readl(TS_PLL_STAT0) & 0xffff;
			}
			for (i = 0; i <= T_AVG_NUM; i ++) {
				udelay(T_DLY_TIME);
				value_ts = readl(TS_PLL_STAT0) & 0xffff;
				printf("pll tsensor read: 0x%x\n", value_ts);
				if ((value_ts >= T_VALUE_MIN) &&
						(value_ts <= T_VALUE_MAX)) {
					value_all_ts += value_ts;
					cnt ++;
				}
			}
			value_ts =  value_all_ts / cnt;
			printf("pll tsensor avg: 0x%x\n", value_ts);
			if (value_ts == 0) {
				printf("pll tsensor read temp is zero\n");
				return -1;
			}
			u_efuse = r1p1_temptocode(value_ts, tempbase);
			printf("pll ts efuse:%d\n", u_efuse);
			printf("pll ts efuse:0x%x, index: %d\n", u_efuse, index_ts);
			if (thermal_calibration(index_ts, u_efuse) < 0) {
				printf("pll tsensor thermal_calibration send error\n");
				return -1;
			}
			break;
		case 2:
			value_ts = 0;
			value_all_ts = 0;
			index_ts = 7;
			cnt = 0;
			/*enable thermal2*/
			writel(T_CONTROL_DATA, TS_DDR_CFG_REG1);
			writel(T_TSCLK_DATA, HHI_TS_CLK_CNTL);
			for (i = 0; i <= 10; i ++) {
				udelay(50);
				value_ts = readl(TS_DDR_STAT0) & 0xffff;
			}
			for (i = 0; i <= T_AVG_NUM; i ++) {
				udelay(T_DLY_TIME);
				value_ts = readl(TS_DDR_STAT0) & 0xffff;
				printf("ddr tsensor read: 0x%x\n", value_ts);
				if ((value_ts >= T_VALUE_MIN) &&
						(value_ts <= T_VALUE_MAX)) {
					value_all_ts += value_ts;
					cnt ++;
				}
			}
			value_ts =  value_all_ts / cnt;
			printf("ddr tsensor avg: 0x%x\n", value_ts);
			if (value_ts == 0) {
				printf("ddr tsensor read temp is zero\n");
				return -1;
			}
			u_efuse = r1p1_temptocode(value_ts, tempbase);
			printf("ddr ts efuse:%d\n", u_efuse);
			printf("ddr ts efuse:0x%x, index: %d\n", u_efuse, index_ts);
			if (thermal_calibration(index_ts, u_efuse) < 0) {
				printf("ddr tsensor thermal_calibration send error\n");
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
	unsigned int ret, pllver, ddrver;

	ret = readl(AO_SEC_GP_CFG10);
	pllver = (ret >> 24) & 0xff;
	ret = readl(AO_SEC_SD_CFG12);
	ddrver = (ret >> 24) & 0xff;
	if (0 == (pllver & ddrver & T_VER_MASK)) {
		printf("tsensor no trimmed: calidata:0x%x, 0x%x\n",
			readl(AO_SEC_GP_CFG10), readl(AO_SEC_SD_CFG12));
		return -1;
	}
	ret = (pllver & 0xf) >> 2;
	switch (ret) {
		case 0x2:
			r1p1_temp_read(1);
			r1p1_temp_read(2);
			printf("read the thermal1 and thermal2\n");
		break;
		case 0x0:
		case 0x1:
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
	unsigned int ret, pllver, ddrver;

	ret = readl(AO_SEC_GP_CFG10);
	pllver = (ret >> 24) & 0xff;
	ret = readl(AO_SEC_SD_CFG12);
	ddrver = (ret >> 24) & 0xff;
	if ((pllver & T_VER_MASK) || (ddrver & T_VER_MASK)) {
		printf("tsensor trimmed: cali data: 0x%x, 0x%x\n",
			readl(AO_SEC_GP_CFG10), readl(AO_SEC_SD_CFG12));
		return -1;
	}

	printf("tsensor input trim tempver, tempver:0x%x\n", tempver);
	switch (tempver) {
		case 0x88:
			r1p1_temp_trim(tempbase, tempver, 1);
			r1p1_temp_trim(tempbase, tempver, 2);
			r1p1_temp_trim(tempbase, tempver, 0);
			printf("triming the thermal1 and thermal2 by bbt-sw\n");
		break;
		case 0x89:
			r1p1_temp_trim(tempbase, tempver, 1);
			r1p1_temp_trim(tempbase, tempver, 2);
			r1p1_temp_trim(tempbase, tempver, 0);
			printf("triming the thermal1 and thermal2 by bbt-ops\n");
		break;
		case 0x8b:
			r1p1_temp_trim(tempbase, tempver, 1);
			r1p1_temp_trim(tempbase, tempver, 2);
			r1p1_temp_trim(tempbase, tempver, 0);
			printf("triming the thermal1 and thermal2 by slt\n");
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
	int temp, temp1, temp2;

	while (1) {
		temp1 = r1p1_temp_read(1);
		temp2 = r1p1_temp_read(2);
		temp = temp1 > temp2 ? temp1 : temp2;
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
