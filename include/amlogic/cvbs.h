/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * include/amlogic/cvbs.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef _INC_CVBS_H__
#define _INC_CVBS_H__

void cvbs_init(void);
void cvbs_show_valid_vmode(void);
int cvbs_outputmode_check(char *vmode_name, unsigned int frac);
int cvbs_set_vmode(char* vmode_name);
int cvbs_set_bist(char* bist_mode);
int cvbs_set_vdac(int status);
int cvbs_reg_debug(int argc, char* const argv[]);

#endif

