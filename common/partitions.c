// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <malloc.h>
#include <linux/err.h>
#include <partition_table.h>
#include <linux/libfdt.h>
#include <asm/arch/bl31_apis.h>
#include <amlogic/aml_efuse.h>
#include <part_efi.h>
#include <blk.h>
#include <compiler.h>
#include <mmc.h>
#include <emmc_partitions.h>
#include <version.h>
#include <amlogic/image_check.h>

#ifdef CONFIG_MULTI_DTB
	extern unsigned long get_multi_dt_entry(unsigned long fdt_addr);
#endif

#define GPT_SPACE 0X2000

struct partitions_data{
	int nr;
	struct partitions *parts;
};

struct partitions *part_table = NULL;
int parts_total_num;
int has_boot_slot = 0;
int has_system_slot = 0;
bool dynamic_partition = false;
bool vendor_boot_partition = false;
bool is_partition_checked = false;
#if CONFIG_IS_ENABLED(EFI_PARTITION)
bool gpt_partition;
#endif

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

/*
  return 0 if dts is valid
  other value are falure.
*/
int check_valid_dts(unsigned char *buffer)
{
	int ret = -__LINE__;
	char *dt_addr;
	/* fixme, a work around way */
#ifndef CONFIG_SKIP_KERNEL_DTB_SECBOOT_CHECK
	unsigned char *sbuffer = (unsigned char *)env_get_hex("loadaddr", 0x1000000 + 0x100000);
	ulong ncheckoffset = 0;
	/* g12a merge to trunk, use trunk code */
	//unsigned char *sbuffer = (unsigned char *)0x1000000;

	if (IS_FEAT_BOOT_VERIFY()) {
		memcpy(sbuffer, buffer, AML_DTB_IMG_MAX_SZ);
		flush_cache((unsigned long)sbuffer, AML_DTB_IMG_MAX_SZ);
#ifndef CONFIG_IMAGE_CHECK
		ret = aml_sec_boot_check(AML_D_P_IMG_DECRYPT, (long unsigned)sbuffer, AML_DTB_IMG_MAX_SZ, 0);
#else
		ret = secure_image_check((uint8_t *)(unsigned long)sbuffer, AML_DTB_IMG_MAX_SZ, 0);
		ncheckoffset = sizeof(struct aml_boot_header_t);
#endif
		if (ret) {
			printf("\n %s() %d: Decrypt dtb: Sig Check %d\n", __func__, __LINE__, ret);
			return -__LINE__;
		}
		memcpy(buffer, sbuffer + ncheckoffset, AML_DTB_IMG_MAX_SZ);
	}
#endif
#ifdef CONFIG_MULTI_DTB
	dt_addr = (char *)get_multi_dt_entry((unsigned long)buffer);
#else
	dt_addr = (char *)buffer;
#endif
	pr_debug("start dts,buffer=%p,dt_addr=%p\n", buffer, dt_addr);
	ret = fdt_check_header(dt_addr);
	if ( ret < 0 )
		printf("%s: %s\n",__func__,fdt_strerror(ret));
	/* fixme, is it 0 when ok? */
	return ret;
}

#if CONFIG_IS_ENABLED(EFI_PARTITION)
int parse_gpt(struct blk_desc *dev_desc, void *buf)
{
	gpt_header *gpt_h;
	gpt_entry *gpt_e;
	size_t efiname_len, dosname_len;
	int parts_num = 0;
	int i, k;

	/* determine start of GPT Header in the buffer */
	gpt_h = buf + (GPT_PRIMARY_PARTITION_TABLE_LBA *
			dev_desc->blksz);

	/* determine start of GPT Entries in the buffer */
	gpt_e = buf + (le64_to_cpu(gpt_h->partition_entry_lba) *
			dev_desc->blksz);

	parts_num = le32_to_cpu(gpt_h->num_partition_entries);
	if (parts_num > 0) {
		part_table = (struct partitions *)
			malloc(sizeof(struct partitions) * parts_num);
		if (!part_table) {
			printf("%s part_table alloc _err\n", __func__);
			return -1;
		}
		memset(part_table, 0, sizeof(struct partitions) * parts_num);
		parts_total_num = parts_num;
	}

	dynamic_partition = false;
	env_set("partition_mode", "normal");
	vendor_boot_partition = false;
	env_set("vendor_boot_mode", "false");

	for (i = 0; i < parts_num; i++) {
		if (!is_pte_valid(&gpt_e[i])) {
			free(part_table);
			return -1;
		}

		part_table[i].offset = le64_to_cpu(gpt_e[i].starting_lba << 9ULL);
		part_table[i].size = ((le64_to_cpu(gpt_e[i].ending_lba) + 1) -
				le64_to_cpu(gpt_e[i].starting_lba)) << 9ULL;
		/* mask flag */
		part_table[i].mask_flags =
			(uint32_t)le64_to_cpu(gpt_e[i].attributes.fields.type_guid_specific);
		/* partition name */
		efiname_len = sizeof(gpt_e[i].partition_name) / sizeof(efi_char16_t);
		dosname_len = sizeof(part_table[i].name);

		memset(part_table[i].name, 0, sizeof(part_table[i].name));
		for (k = 0; k < min(dosname_len, efiname_len); k++)
			part_table[i].name[k] = (char)gpt_e[i].partition_name[k];

		if (strcmp(part_table[i].name, "boot_a") == 0) {
			has_boot_slot = 1;
			printf("set has_boot_slot = 1\n");
		} else if (strcmp(part_table[i].name, "boot") == 0) {
			has_boot_slot = 0;
			printf("set has_boot_slot = 0\n");
		}

		if (strcmp(part_table[i].name, "system_a") == 0)
			has_system_slot = 1;
		else if (strcmp(part_table[i].name, "system") == 0)
			has_system_slot = 0;

		if (strcmp(part_table[i].name, "super") == 0) {
			dynamic_partition = true;
			env_set("partition_mode", "dynamic");
			printf("enable dynamic_partition\n");
		}

		if (strncmp(part_table[i].name, "vendor_boot", 11) == 0) {
			vendor_boot_partition = true;
			env_set("vendor_boot_mode", "true");
			printf("enable vendor_boot\n");
		}
	}
	is_partition_checked = false;
	return 0;
}
#endif

int get_partition_from_dts(unsigned char *buffer)
{
	char *dt_addr;
	int nodeoffset,poffset=0;
	int *parts_num;
	char propname[8];
	const uint32_t *phandle;
	const char *uname;
	const char *usize;
	const char *umask;
	int index;
	int ret = -1;
#if CONFIG_IS_ENABLED(EFI_PARTITION)
	struct blk_desc *dev_desc;

	if (!buffer)
		goto _err;

	dev_desc = (struct blk_desc *)malloc(sizeof(struct blk_desc));
	if (!dev_desc)
		goto _err;

	dev_desc->blksz = MMC_BLOCK_SIZE;
	dev_desc->lba = GPT_SPACE;

	if (!is_valid_gpt_buf(dev_desc, buffer)) {
		if (!parse_gpt(dev_desc, buffer)) {
			gpt_partition = true;
			free(dev_desc);
			return 0;
		}
	}
	free(dev_desc);
#endif

	ret = check_valid_dts(buffer);
	printf("%s() %d: ret %d\n",__func__, __LINE__, ret);
	if ( ret < 0 )
	{
		printf("%s() %d: ret %d\n",__func__, __LINE__, ret);
		goto _err;
	}
#ifdef CONFIG_MULTI_DTB
	dt_addr = (char *)get_multi_dt_entry((unsigned long)buffer);
#else
	dt_addr = (char *)buffer;
#endif
	nodeoffset = fdt_path_offset(dt_addr, "/partitions");
	if (nodeoffset < 0)
	{
		printf("%s: not find /partitions node %s.\n",__func__,fdt_strerror(nodeoffset));
		ret = -1;
		goto _err;
	}
	parts_num = (int *)fdt_getprop(dt_addr, nodeoffset, "parts", NULL);
	printf("parts: %d\n",be32_to_cpup((u32*)parts_num));

	if (parts_num > 0)
	{
		part_table = (struct partitions *)malloc(sizeof(struct partitions)*(be32_to_cpup((u32*)parts_num)));
		if (!part_table) {
			printf("%s part_table alloc _err\n", __func__);
			return -1;
		}
		memset(part_table, 0, sizeof(struct partitions)*(be32_to_cpup((u32*)parts_num)));
		parts_total_num = be32_to_cpup((u32*)parts_num);
	}
	dynamic_partition = false;
	env_set("partition_mode","normal");
	vendor_boot_partition = false;
	env_set("vendor_boot_mode","false");
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
			env_set("partition_mode","dynamic");
			printf("enable dynamic_partition\n");
		}

		if (strncmp(uname, "vendor_boot", 11) == 0) {
			vendor_boot_partition = true;
			env_set("vendor_boot_mode","true");
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
