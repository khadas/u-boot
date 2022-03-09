/*
 * board/amlogic/tm2_t962x3_t312_v1/mtk-bt/cmd_btmtk.c
 *
 * Copyright (C) 2017 Amlogic, Inc. All rights reserved.
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
 */

#include <command.h>
#include <common.h>
#include "LD_usbbt.h"

extern int fwGetFileSize(const char *file_path);
extern int fwReadFileToBuffer(const char *file_path, unsigned char data_buf[]);

int vfs_mount(char *volume)
{
	return 0;
}

/* Amlogic need to implement. */
unsigned long vfs_getsize(char *filedir)
{
	int fileSize = 0;
	fileSize = fwGetFileSize(filedir);
	if (fileSize > 0)
	{
		printf("vfs_getsize %s size is %d\n", filedir, fileSize);
		return fileSize;
	}

	printf("vfs_getsize %s failed\n", filedir);
	return 0;
}

/* Amlogic need to implement. */
int vfs_read(void *addr, char *filedir, unsigned int offset, unsigned int size)
{
	int ret = -1;
	ret = fwReadFileToBuffer(filedir, addr);
	if (ret > 0)
	{
		printf("vfs_read load %s to buffer success\n", filedir);
		return 0;
	}
	printf("vfs_read load %s failed\n", filedir);
	return -1;
}
