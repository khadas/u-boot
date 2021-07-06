/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/partitions.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <malloc.h>
#include <linux/err.h>
#include <partition_table.h>
#include <libfdt.h>
#include <asm/arch/bl31_apis.h>
#include <asm/arch/secure_apb.h>

extern int is_dtb_encrypt(unsigned char *buffer);
extern int check_valid_dts(unsigned char *buffer, unsigned char **dts);
#ifdef CONFIG_MULTI_DTB
	extern unsigned long get_multi_dt_entry(unsigned long fdt_addr);
#endif

struct partitions_data{
	int nr;
	struct partitions *parts;
};

struct partitions *part_table = NULL;
static int parts_total_num;
int has_boot_slot = 0;
int has_system_slot = 0;
bool dynamic_partition = false;
bool vendor_boot_partition = false;
extern bool is_partition_checked;


int get_partitions_table(struct partitions **table)
{
	int ret = 0;
	if (part_table && parts_total_num) {
		*table = part_table;
		ret = parts_total_num;
	}
	return ret;
}
int get_partition_count(void)
{
	return parts_total_num;
}
struct partitions *get_partitions(void)
{
	return part_table;
}



void free_partitions(void)
{
	if (part_table)
		free(part_table);
	part_table = NULL;
}


int get_partition_from_dts(unsigned char *buffer)
{
	unsigned char *dt_addr = buffer;
	int nodeoffset,poffset=0;
	int *parts_num;
	char propname[8];
	const uint32_t *phandle;
	const char *uname;
	const char *usize;
	const char *umask;
	int index;
	int ret = -1;

	if ( buffer == NULL)
		goto _err;

	ret = check_valid_dts(buffer, &dt_addr);
	printf("%s() %d: ret %d\n",__func__, __LINE__, ret);
	if ( ret < 0 )
		goto _err;

	nodeoffset = fdt_path_offset(dt_addr, "/partitions");
	if (nodeoffset < 0)
	{
		printf("%s: not find /partitions node %s.\n",__func__,fdt_strerror(nodeoffset));
		ret = -1;
		goto _err;
	}
	parts_num = (int *)fdt_getprop(dt_addr, nodeoffset, "parts", NULL);
	if (!parts_num)
		goto _err;
	printf("parts: %d\n",be32_to_cpup((u32*)parts_num));

	if (parts_num > 0)
	{
		part_table = (struct partitions *)malloc(sizeof(struct partitions)*(be32_to_cpup((u32*)parts_num)));
		if (!part_table) {
			printk("%s part_table alloc _err\n",__func__);
			//kfree(data);
			return -1;
		}
		memset(part_table, 0, sizeof(struct partitions)*(be32_to_cpup((u32*)parts_num)));
		parts_total_num = be32_to_cpup((u32*)parts_num);
	}
	dynamic_partition = false;
	setenv("partition_mode", "normal");
	vendor_boot_partition = false;
	setenv("vendor_boot_mode", "false");
	for (index = 0; index < be32_to_cpup((u32*)parts_num); index++)
	{
		sprintf(propname,"part-%d", index);

		phandle = fdt_getprop(dt_addr, nodeoffset, propname, NULL);
		if (!phandle) {
			printf("don't find  match part-%d\n",index);
			goto _err;
		}
		if (phandle) {
			poffset = fdt_node_offset_by_phandle(dt_addr, be32_to_cpup((u32*)phandle));
			if (!poffset) {
				printf("%s:%d,can't find device node\n",__func__,__LINE__);
				goto _err;
			}
		}
		uname = fdt_getprop(dt_addr, poffset, "pname", NULL);
		//printf("%s:%d  uname: %s\n",__func__,__LINE__, uname);
		/* a string but not */
		usize = fdt_getprop(dt_addr, poffset, "size", NULL);
		//printf("%s:%d size: 0x%x  0x%x\n",__func__,__LINE__, be32_to_cpup((u32*)usize), be32_to_cpup((((u32*)usize)+1)));
		umask = fdt_getprop(dt_addr, poffset, "mask", NULL);
		//printf("%s:%d mask: 0x%x\n",__func__,__LINE__, be32_to_cpup((u32*)umask));
		/* fill parition table */
		if (uname != NULL)
			memcpy(part_table[index].name, uname, strlen(uname));
		part_table[index].size = ((unsigned long)be32_to_cpup((u32*)usize) << 32) | (unsigned long)be32_to_cpup((((u32*)usize)+1));
		part_table[index].mask_flags = be32_to_cpup((u32*)umask);
		printf("%02d:%10s\t%016llx %01x\n", index, uname, part_table[index].size, part_table[index].mask_flags);

		if (strcmp(uname, "boot_a") == 0) {
			has_boot_slot = 1;
			printf("set has_boot_slot = 1\n");
		}
		else if (strcmp(uname, "boot") == 0) {
			has_boot_slot = 0;
			printf("set has_boot_slot = 0\n");
		}
		if (strcmp(uname, "system_a") == 0)
			has_system_slot = 1;
		else if (strcmp(uname, "system") == 0)
			has_system_slot = 0;

		if (strcmp(uname, "super") == 0) {
			dynamic_partition = true;
			setenv("partition_mode", "dynamic");
			printf("enable dynamic_partition\n");
		}

		if (strncmp(uname, "vendor_boot", 11) == 0) {
			vendor_boot_partition = true;
			setenv("vendor_boot_mode", "true");
			printf("enable vendor_boot\n");
		}
	}
	/*enable mmc_device_init when dtb is update from ddr*/
	is_partition_checked = false;
	return 0;

_err:
	if (part_table != NULL) {
		free(part_table);
		part_table = NULL;
	}
	return ret;
}
