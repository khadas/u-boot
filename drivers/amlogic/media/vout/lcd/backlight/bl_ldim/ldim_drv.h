/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __AML_LDIM_DRV_H__
#define __AML_LDIM_DRV_H__
#include <asm/arch/io.h>
#include <amlogic/media/vout/lcd/lcd_vout.h>
#include <amlogic/media/vout/lcd/bl_ldim.h>

#define LDIMPR(fmt, args...)     printf("ldim: "fmt"", ## args)
#define LDIMERR(fmt, args...)    printf("ldim: error: "fmt"", ## args)

#define LD_DATA_DEPTH   12
#define LD_DATA_MAX     0xfff

void bl_pwm_ctrl(struct bl_pwm_config_s *bl_pwm, int status);
#ifdef CONFIG_OF_LIBFDT
enum bl_pwm_port_e bl_pwm_str_to_pwm(const char *str);
#endif

/* ldim device probe function */
int aml_ldim_device_probe(char *dt_addr, struct aml_ldim_driver_s *ldim_drv);
int aml_ldim_device_remove(struct aml_ldim_driver_s *ldim_drv);

#endif
