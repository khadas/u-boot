
/*
 * arch/arm/include/asm/arch-g12a/tsensor.h
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#ifndef __TSENSOR_H__
#define __TSENSOR_H__

#define CONFIG_HIGH_TEMP_COOL	90

#define T_AVG_NUM	16 /*trim temp cnt*/
#define T_READ_NUM	2 /*read temp cnt*/
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
