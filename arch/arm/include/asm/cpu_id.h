/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * arch/arm/include/asm/cpu_id.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#define MESON_CPU_MAJOR_ID_M6		0x16
#define MESON_CPU_MAJOR_ID_M6TV		0x17
#define MESON_CPU_MAJOR_ID_M6TVL	0x18
#define MESON_CPU_MAJOR_ID_M8		0x19
#define MESON_CPU_MAJOR_ID_MTVD		0x1A
#define MESON_CPU_MAJOR_ID_M8B		0x1B
#define MESON_CPU_MAJOR_ID_MG9TV	0x1C
#define MESON_CPU_MAJOR_ID_M8M2		0x1D
#define MESON_CPU_MAJOR_ID_GXBB		0x1F
#define MESON_CPU_MAJOR_ID_GXTVBB	0x20
#define MESON_CPU_MAJOR_ID_GXL		0x21
#define MESON_CPU_MAJOR_ID_GXM		0x22
#define MESON_CPU_MAJOR_ID_TXL		0x23
#define MESON_CPU_MAJOR_ID_TXLX		0x24
#define MESON_CPU_MAJOR_ID_AXG		0x25
#define MESON_CPU_MAJOR_ID_GXLX		0x26
#define MESON_CPU_MAJOR_ID_TXHD		0x27
#define MESON_CPU_MAJOR_ID_G12A		0x28
#define MESON_CPU_MAJOR_ID_G12B		0x29
#define MESON_CPU_MAJOR_ID_SM1		0x2B
#define MESON_CPU_MAJOR_ID_TL1		0x2E
#define MESON_CPU_MAJOR_ID_TM2		0x2F
#define MESON_CPU_MAJOR_ID_T5		0x34
#define MESON_CPU_MAJOR_ID_T5D		0x35
#define MESON_CPU_MAJOR_ID_T5W		0x3b

#define MESON_CPU_PACKAGE_ID_905D	0X00
#define MESON_CPU_PACKAGE_ID_905M	0x20
#define MESON_CPU_PACKAGE_ID_905X	0X80
#define MESON_CPU_PACKAGE_ID_905L	0XC0
#define MESON_CPU_PACKAGE_ID_905M2	0XE0

#define MESON_CPU_PACKAGE_ID_T962X	0x10
#define MESON_CPU_PACKAGE_ID_T962E	0x20

#define MESON_CPU_PACKAGE_ID_A113X	0x37
#define MESON_CPU_PACKAGE_ID_A113D	0xD0

#define MESON_CPU_PACKAGE_ID_T963	0x01
#define MESON_CPU_PACKAGE_ID_T963S	0x02

#define MESON_CPU_PACKAGE_ID_T921D	0x01
#define MESON_CPU_PACKAGE_ID_T950D4	0x02
#define MESON_CPU_PACKAGE_ID_T950X4	0x03

#define MESON_CPU_CHIP_REVISION_A	0xA
#define MESON_CPU_CHIP_REVISION_B	0xB
#define MESON_CPU_CHIP_REVISION_C	0xC
#define MESON_CPU_CHIP_REVISION_D	0xD

typedef struct cpu_id {
	unsigned int family_id:8; //S905/T968 etc.
	unsigned int package_id:8; //T968/T966 etc.
	unsigned int chip_rev:8; //RevA/RevB etc.
	unsigned int reserve:4;
	unsigned int layout_ver:4;
} cpu_id_t;

typedef struct chip_id {
	unsigned int version;
	unsigned char chipid[16];
} chip_id_t;

cpu_id_t get_cpu_id(void);

extern chip_id_t aml_chip_id;
int get_chip_id(unsigned char *buff, unsigned int size);

/**
 * TODO: Remove this API after move static system information to RO
 * register or SRAM
 *
 * Should *NOT* call this API directly, always use get_chip_id()
 */
int __get_chip_id(unsigned char *buff, unsigned int size);
