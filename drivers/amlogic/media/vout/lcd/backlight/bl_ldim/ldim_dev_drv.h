/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __LDIM_DEV_DRV_H
#define __LDIM_DEV_DRV_H
#include <spi.h>
#include <amlogic/media/vout/lcd/bl_ldim.h>

void ldim_set_gpio(int index, int value);
unsigned int ldim_get_gpio(int index);
void ldim_set_duty_pwm(struct bl_pwm_config_s *bl);

extern int ldim_spi_write(struct spi_slave *spi, unsigned char *tbuf, int tlen);
extern int ldim_spi_read(struct spi_slave *spi, unsigned char *tbuf, int tlen,
		unsigned char *rbuf, int rlen);
extern int ldim_spi_driver_add(struct aml_ldim_driver_s *ldim_drv);
extern int ldim_spi_driver_remove(struct aml_ldim_driver_s *ldim_drv);

/* ldim device probe function */
#ifdef CONFIG_AML_LOCAL_DIMMING_IW7019
int ldim_dev_iw7019_probe(struct aml_ldim_driver_s *ldim_drv);
int ldim_dev_iw7019_remove(struct aml_ldim_driver_s *ldim_drv);
#endif

#ifdef CONFIG_AML_LOCAL_DIMMING_IW7027
extern int ldim_dev_iw7027_probe(struct aml_ldim_driver_s *ldim_drv);
extern int ldim_dev_iw7027_remove(struct aml_ldim_driver_s *ldim_drv);
#endif

#ifdef CONFIG_AML_LOCAL_DIMMING_IW7027_HE
int ldim_dev_iw7027_he_probe(struct aml_ldim_driver_s *ldim_drv);
int ldim_dev_iw7027_he_remove(struct aml_ldim_driver_s *ldim_drv);
#endif

#ifdef CONFIG_AML_LOCAL_DIMMING_IW7038
int ldim_dev_iw7038_probe(struct aml_ldim_driver_s *ldim_drv);
int ldim_dev_iw7038_remove(struct aml_ldim_driver_s *ldim_drv);
#endif

#ifdef CONFIG_AML_LOCAL_DIMMING_OB3350
extern int ldim_dev_ob3350_probe(struct aml_ldim_driver_s *ldim_drv);
extern int ldim_dev_ob3350_remove(struct aml_ldim_driver_s *ldim_drv);
#endif

#ifdef CONFIG_AML_LOCAL_DIMMING_GLOBAL
int ldim_dev_global_probe(struct aml_ldim_driver_s *ldim_drv);
int ldim_dev_global_remove(struct aml_ldim_driver_s *ldim_drv);
#endif

#endif
