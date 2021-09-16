/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __MESON_JTAG_H__
#define __MESON_JTAG_H__

#define INVALID_ID		-1

/* ioctl command */
#define JTAG_CLRPINMUX		0
#define JTAG_SETPINMUX		1
#define JTAG_EANBLE		2
#define JTAG_DISABLE		3

/* jtag type */
#define JTAG_A			0
#define JTAG_B			1
#define SWD_A			2

/* core type */
#define	AP			1 /* a35 a53 a72 ... */
#define SCP			2 /* m3 */
#define SP			3 /* m4 */
#define DSPA			4 /* dsp-a */
#define	DSPB			5 /* dsp-b */
#define	DIF			6 /* dif */
#define	AOCPU			7 /* aocpu */
#define	AUCPU			8 /* aucpu */
#define	SOCBRG			9 /* SOC Bridge */
#define	DEMOD			10/* demod */
#define	SECPU			11/* secpu */
#define	MOPA			12
#define	MOPB			13

/*
 * bitmap for jtag select ID:
 *
 *	|---|---|---|---|---|---|---|------|------|
 *	| 9 | 8 | 6 | 5 | 4 | 3 | 2 |   1  |   0  |
 *	|---|---|---|---|---|---|---|------|------|
 *	|   core type   | jtag type |cluster type |
 *	|---------------|-----------|-------------|
 */
#define CORE_TYPE_SHIFT	5
#define JTAG_TYPE_SHIFT	2
#define CLUSTER_TYPE_SHIFT	0

#define CORE_TYPE_MASK	0xf
#define JTAG_TYPE_MASK	0x7
#define CLUSTER_TYPE_MASK	0x3

#define JTAG_SELECT_ID(core_type, jtag_type, cluster_type) \
	(((core_type & CORE_TYPE_MASK) << CORE_TYPE_SHIFT) |\
	((jtag_type & JTAG_TYPE_MASK) << JTAG_TYPE_SHIFT) |\
	((cluster_type & CLUSTER_TYPE_MASK) << CLUSTER_TYPE_SHIFT))

#define CLUSTER_TYPE_UPDATE(id, cluster_type) \
	((id & (~(CLUSTER_TYPE_MASK))) | cluster_type)

#define CORE_TYPE_GET(id) \
	((id >> CORE_TYPE_SHIFT) & CORE_TYPE_MASK)

#define JTAG_TYPE_GET(id) \
	((id >> JTAG_TYPE_SHIFT) & JTAG_TYPE_MASK)

#define CLUSTER_TYPE_GET(id) \
	((id >> CLUSTER_TYPE_SHIFT) & CLUSTER_TYPE_MASK)

#endif /*_MESON_JTAG_H_*/
