/*
* Copyright (C) 2017 Amlogic, Inc. All rights reserved.
* *
This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* *
This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
* *
You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
* *
Description:
*/


#ifndef __RAMDUMP_H__
#define __RAMDUMP_H__

#define RAMDUMP_STICKY_DATA_MASK		(0xFFFF)

extern void check_ramdump(void);
extern void ramdump_init(void);

extern unsigned long ramdump_base;
extern unsigned long ramdump_size;

int ramdump_save_compress_data(void);

#endif /* __RAMDUMP_H__ */
