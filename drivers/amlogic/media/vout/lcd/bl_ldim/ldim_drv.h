
/*
 * drivers/amlogic/media/vout/lcd/bl_ldim/ldim_drv.h
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

#ifndef __AML_LDIM_DRV_H__
#define __AML_LDIM_DRV_H__
#include <asm/arch/io.h>
#include <amlogic/media/vout/lcd/lcd_vout.h>

#define LDIMPR(fmt, args...)     printf("ldim: "fmt"", ## args)
#define LDIMERR(fmt, args...)    printf("ldim: error: "fmt"", ## args)

#define LD_DATA_DEPTH   12
#define LD_DATA_MAX     0xfff

extern void bl_pwm_ctrl(struct bl_pwm_config_s *bl_pwm, int status);
extern enum bl_pwm_port_e bl_pwm_str_to_pwm(const char *str);

/* ldim device probe function */
extern int aml_ldim_device_probe(char *dt_addr);

#endif
