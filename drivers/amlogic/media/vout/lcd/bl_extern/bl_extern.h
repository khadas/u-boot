/*
 * drivers/amlogic/media/vout/lcd/bl_extern/bl_extern.h
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

#ifndef _BL_EXTERN_H_
#define _BL_EXTERN_H_
#include <amlogic/media/vout/lcd/bl_extern.h>

#define BLEXT(fmt, args...)       printf("bl extern: "fmt"", ## args)
#define BLEXTERR(fmt, args...)    printf("bl extern: error: "fmt"", ## args)
#define BL_EXTERN_DRIVER         "bl_extern"

extern void bl_extern_i2c_bus_print(unsigned char i2c_bus);
extern int bl_extern_i2c_write(unsigned char i2c_bus, unsigned i2c_addr,
		unsigned char *buff, unsigned len);
extern int bl_extern_i2c_read(unsigned char i2c_bus, unsigned i2c_addr,
		unsigned char *buff, unsigned int len);

#ifdef CONFIG_AML_BL_EXTERN_I2C_LP8556
extern int i2c_lp8556_probe(void);
#endif

#ifdef CONFIG_AML_BL_EXTERN_MIPI_IT070ME05
extern int mipi_lt070me05_probe(void);
#endif


#endif

