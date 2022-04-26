/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __TSENSOR_H__
#define __TSENSOR_H__


struct tsensor_cali_data {
	unsigned int cmd;		/* R/W */
	unsigned int offset;
	unsigned int size;
	unsigned long buffer_phy;
	unsigned long retcnt_phy;
};


#define CONFIG_HIGH_TEMP_COOL	90

#define T_AVG_NUM	16 /*read temp cnt*/
#define T_VER_MASK	0x80
#define T_VALUE_MIN	0x1500
#define T_VALUE_MAX	0x3500
#define T_DLY_TIME	4500 /*update sensor register need 4.2ms*/

#define T_CONTROL_DATA	0x62b
#define T_TSCLK_DATA	0x130

#define ts_b	3159
#define	ts_a	9411
#define	ts_m	424
#define	ts_n	324

#ifdef CONFIG_AML_TSENSOR
int temp_read_entry(void);
int temp_trim_entry(int tempbase, int tempver);
int temp_cooling_entry(void);
#else
int temp_read_entry(void)
{
	return -1;
}
int temp_trim_entry(int tempbase, int tempver)
{
	return -1;
}
int temp_cooling_entry(void)
{
	return 0;
}
#endif
#endif
