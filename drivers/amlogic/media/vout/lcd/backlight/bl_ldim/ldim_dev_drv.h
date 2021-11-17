/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __LDIM_DEV_DRV_H
#define __LDIM_DEV_DRV_H
#include <spi.h>
#include <amlogic/media/vout/lcd/bl_ldim.h>

void ldim_gpio_set(struct ldim_dev_driver_s *dev_drv, int index, int value);
int ldim_gpio_get(struct ldim_dev_driver_s *dev_drv, int index);
void ldim_set_duty_pwm(struct bl_pwm_config_s *bl_pwm);
void ldim_pwm_off(struct bl_pwm_config_s *bl_pwm);

int ldim_spi_write(struct spi_slave *spi, unsigned char *tbuf, int tlen);
int ldim_spi_read(struct spi_slave *spi, unsigned char *tbuf, int tlen,
		  unsigned char *rbuf, int rlen);
int ldim_spi_read_sync(struct spi_slave *spi, unsigned char *tbuf,
		       unsigned char *rbuf, int len);
int ldim_spi_driver_add(struct ldim_dev_driver_s *dev_drv);
int ldim_spi_driver_remove(struct ldim_dev_driver_s *dev_drv);

int handle_ldim_dev_zone_mapping_get(unsigned char *buf, unsigned int size,
				     const char *path);

/* ldim device probe function */
#ifdef CONFIG_AML_LCD_BL_LDIM_IW7027
int ldim_dev_iw7027_probe(struct aml_ldim_driver_s *ldim_drv);
int ldim_dev_iw7027_remove(struct aml_ldim_driver_s *ldim_drv);
#endif

#ifdef CONFIG_AML_LCD_BL_LDIM_BLMCU
int ldim_dev_blmcu_probe(struct aml_ldim_driver_s *ldim_drv);
int ldim_dev_blmcu_remove(struct aml_ldim_driver_s *ldim_drv);
#endif

#ifdef CONFIG_AML_LCD_BL_LDIM_OB3350
int ldim_dev_ob3350_probe(struct aml_ldim_driver_s *ldim_drv);
int ldim_dev_ob3350_remove(struct aml_ldim_driver_s *ldim_drv);
#endif

int ldim_dev_global_probe(struct aml_ldim_driver_s *ldim_drv);
int ldim_dev_global_remove(struct aml_ldim_driver_s *ldim_drv);

#endif
