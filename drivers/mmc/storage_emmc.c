// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
#include <amlogic/storage.h>
#include <dm/pinctrl.h>
#include <partition_table.h>
#include <emmc_partitions.h>
#include <amlogic/cpu_id.h>
#include <asm/arch-g12a/bl31_apis.h>
#include <linux/compat.h>
#include <amlogic/aml_mmc.h>

#define USER_PARTITION 0
#define BOOT0_PARTITION 1
#define BOOT1_PARTITION 2
#define RPMB_PARTITION 3

#define NOMAL_INIT 0
#define ERASE_ALL 3
#define ERASE_RESERVED 2

#define GXB_START_BLK   0
#define GXL_START_BLK   1
#define STORAGE_EMMC 1
/* max 2MB for emmc in blks */
#define UBOOT_SIZE  (0x1000)
#define BLOCK_SIZE 512





extern int find_dev_num_by_partition_name (char const *name);
extern struct partitions *get_partition_info_by_num(const int num);
extern bool emmckey_is_protected(struct mmc *mmc);
extern int info_disprotect;
extern int dtb_read(void *addr);
extern int dtb_write(void *addr);
extern int renew_partition_tbl(unsigned char *buffer);

static int storage_range_check(struct mmc *mmc,char const *part_name,loff_t offset, size_t *size,loff_t *off) {

	struct partitions *part_info = NULL;

	cpu_id_t cpu_id = get_cpu_id();


	if (strcmp(part_name, "bootloader") == 0) {
		*off = 0;
		if (cpu_id.family_id >= MESON_CPU_MAJOR_ID_GXL) {
			*off += 512;
		}
		if (*size == 0) {
			*size =mmc->capacity_boot;
			if (cpu_id.family_id >= MESON_CPU_MAJOR_ID_GXL) {
				*size = *size - 512;
			}
		}
	} else {
		part_info = find_mmc_partition_by_name(part_name);
		if (!part_info) {
			printf("error partition name!\n");
			return 1;
		}
		*off = part_info->offset+offset;
		if (offset >= part_info->size) {
			printf("Start address out #%s# partition'address region,(off < 0x%llx)\n",
						part_name, part_info->size);
			return 1;
		}
		if ((*off+*size) > (part_info->size+part_info->offset)) {
			printf("End address exceeds #%s# partition,(offset = 0x%llx,size = 0x%llx)\n",
						part_name, part_info->offset,part_info->size);
			return 1;
		}

		if (*size == 0) {
			*size = part_info->size - offset;
		}
	}
	return 0;
}



static int storage_rsv_range_check(char const *part_name, size_t *size,loff_t *off) {

	struct partitions *part = NULL;
	struct virtual_partition *vpart = NULL;


	vpart = aml_get_virtual_partition_by_name(part_name);

	if (!vpart) {
		printf("error partition name!\n");
		return 1;
	}
	part = aml_get_partition_by_name(MMC_RESERVED_NAME);
	if (!part) {
		printf("error partition name!\n");
		return 1;
	}
	*off = part->offset + vpart->offset;
	if ((*size) > vpart->size) {
		printf("End address exceeds #%s# partition,(offset = 0x%llx,size = 0x%llx)\n",
						part_name, vpart->offset,vpart->size);
		return 1;
	}
	if (*size == 0)
		*size = vpart->size;
	return 0;
}


static int storage_byte_read(struct mmc *mmc,loff_t off, size_t  size,void *addr) {

	int blk_shift = 0;
	u64 cnt = 0, n = 0, blk = 0, sz_byte = 0;
	ulong start_blk;
	void *addr_tmp;
	void *addr_byte;

	blk_shift =  ffs(mmc->read_bl_len) - 1;
	blk = off >>  blk_shift ;
	cnt = size >>  blk_shift ;
	sz_byte = size - ((cnt) << blk_shift) ;
	mmc_init(mmc);

	pr_info("blk:%lld   cnt:%lld \n",blk,cnt);
	n = blk_dread(mmc_get_blk_desc(mmc), blk, cnt, addr);
	if ((n == cnt) && (sz_byte != 0)) {
	   /*printf("sz_byte=%#llx bytes\n",sz_byte);*/
	   addr_tmp = malloc(mmc->read_bl_len);
	   addr_byte = (void *)(addr+cnt*(mmc->read_bl_len));
	   start_blk = blk+cnt;

	   if (addr_tmp == NULL) {
		   printf("mmc read: malloc fail\n");
		   return 1;
	   }

	   if (blk_dread(mmc_get_blk_desc(mmc), start_blk, 1, addr_tmp) != 1) { // read 1 block
		   free(addr_tmp);
		   printf("mmc read 1 block fail\n");
		   return 1;
	   }

	   memcpy(addr_byte, addr_tmp, sz_byte);
	   free(addr_tmp);
	}
	return (n == cnt) ? 0 : 1;


}

static int storage_byte_write(struct mmc *mmc,loff_t off, size_t  size,void *addr) {

	int blk_shift = 0;
	u64 cnt = 0, n = 0, blk = 0, sz_byte = 0;

	blk_shift =  ffs(mmc->read_bl_len) - 1;
	blk = off >>  blk_shift ;
	cnt = size >>  blk_shift ;
	sz_byte = size - ((cnt) << blk_shift);
	mmc_init(mmc);
	pr_info("blk:%lld   cnt:%lld \n",blk,cnt);

	n = blk_dwrite(mmc_get_blk_desc(mmc), blk, cnt, addr);
	if ((n == cnt) && (sz_byte != 0)) {
		// printf("sz_byte=%#llx bytes\n",sz_byte);
		void *addr_tmp = malloc(mmc->write_bl_len);
		void *addr_byte = (void*)(addr+cnt*(mmc->write_bl_len));
		ulong start_blk = blk+cnt;

		if (addr_tmp == NULL) {
			printf("mmc write: malloc fail\n");
			return 1;
		}

		if (blk_dread(mmc_get_blk_desc(mmc), start_blk, 1, addr_tmp) != 1) { // read 1 block
			free(addr_tmp);
			printf("mmc read 1 block fail\n");
			return 1;
		}

		memcpy(addr_tmp, addr_byte, sz_byte);
		if (blk_dwrite(mmc_get_blk_desc(mmc), start_blk, 1, addr_tmp) != 1) { // write 1 block
			free(addr_tmp);
			printf("mmc write 1 block fail\n");
			return 1;
		}
		free(addr_tmp);
	}
	//printf("%#llx blocks , %#llx bytes written: %s\n", n, sz_byte, (n==cnt) ? "OK" : "ERROR");
	return (n == cnt) ? 0 : 1;
}


static int storage_byte_erase(struct mmc *mmc,loff_t off, size_t  size) {

	int blk_shift = 0;
	u64 cnt = 0, n = 0, blk = 0;

	blk_shift =  ffs(mmc->read_bl_len) - 1;
	blk = off >>  blk_shift ;
	cnt = size >>  blk_shift ;
	mmc_init(mmc);
	pr_info("blk:%lld   cnt:%lld \n",blk,cnt);
	if (cnt)
		n = blk_derase(mmc_get_blk_desc(mmc), blk, cnt);
	printf("%lld blocks erased: %s\n", cnt, (n == 0) ? "OK" : "ERROR");
	return (n == 0) ? 0 : 1;
}

static int storage_erase_in_part(char const *part_name, loff_t off, size_t size)
{
	int ret = 1;
	struct mmc *mmc;
	loff_t offset;

	mmc = find_mmc_device(STORAGE_EMMC);
	if (!mmc)
		return 1;

	ret = storage_range_check(mmc,part_name, off, &size, &offset);
	if (ret)
		return ret;

	ret = storage_byte_erase(mmc, offset, size);
	return (ret == 0) ? 0 : 1;
}



static int storage_read_in_part(char const *part_name, loff_t off, size_t size, void *dest)
{
	int ret =1;
	struct mmc *mmc;
	loff_t offset;

	mmc = find_mmc_device(STORAGE_EMMC);
	if (!mmc)
		return 1;

	ret = storage_range_check(mmc,part_name,off,&size,&offset);

	if (ret) return ret;

	ret = storage_byte_read(mmc, offset, size, dest);

	return ret;
}


static int storage_write_in_part(char const *part_name, loff_t off, size_t size, void *source)
{
	int ret = 1;
	loff_t offset;
	struct mmc *mmc;

	mmc = find_mmc_device(STORAGE_EMMC);
	if (!mmc) {
		printf("Cannot find mmc. \n");
		return 1;
	}
	ret = storage_range_check(mmc,part_name, off, &size, &offset);
	if (ret) return ret;

	ret = storage_byte_write(mmc, offset, size, source);
	return ret;
}

static int storage_mmc_erase_user(struct mmc *mmc) {
	int ret = 0;
	struct partitions *part_info = NULL;

	if (info_disprotect & DISPROTECT_KEY) {//key disprotect,erase all
		ret = blk_derase(mmc_get_blk_desc(mmc), 0, 0);
	} else {//key protect erase all except reserved
		part_info = find_mmc_partition_by_name("reserved");
		if (!part_info) {
			printf("error partition name!\n");
			return 1;
		}
		ret = blk_derase(mmc_get_blk_desc(mmc), 0, part_info->offset / BLOCK_SIZE);
		ret |= blk_derase(mmc_get_blk_desc(mmc), (part_info->offset
				  + part_info->size) / BLOCK_SIZE, 0);
	}
	printf("user partition erased: %s\n", (ret == 0) ? "OK" : "ERROR");
	return ret;
}

static int storage_mmc_erase(int flag, struct mmc *mmc) {

	int ret = 0;
	loff_t off = 0;
	size_t size = 0;

	if (flag >= ERASE_ALL) {//erase all except reserved
		ret = storage_mmc_erase_user(mmc);
		if (ret != 0) {
			return -1;
		}

		ret = blk_select_hwpart_devnum(IF_TYPE_MMC, STORAGE_EMMC, BOOT0_PARTITION);
		if (ret) goto R_SWITCH_BACK;
		ret = blk_derase(mmc_get_blk_desc(mmc), 0, 0);
		printf("boot0 partition erased: %s\n", (ret == 0) ? "OK" : "ERROR");

		ret = blk_select_hwpart_devnum(IF_TYPE_MMC, STORAGE_EMMC, BOOT1_PARTITION);

		if (ret) goto R_SWITCH_BACK;
		ret = blk_derase(mmc_get_blk_desc(mmc), 0, 0);
		printf("boot1 partition erased: %s\n", (ret == 0) ? "OK" : "ERROR");
R_SWITCH_BACK:
		ret = blk_select_hwpart_devnum(IF_TYPE_MMC, STORAGE_EMMC, USER_PARTITION);

	} else if (flag == ERASE_RESERVED) {//erase reserved

		info_disprotect |= DISPROTECT_KEY;
		ret = storage_rsv_range_check("reserved", &size, &off);
		if (ret != 0) {
			return -1;
		}
		ret = storage_erase_in_part("reserved", off, size);
		info_disprotect &= ~DISPROTECT_KEY;
	}

	return ret;


}



int mmc_storage_init(unsigned char init_flag) {

	int ret =1;
	struct mmc *mmc;
	mmc = find_mmc_device(STORAGE_EMMC);
	mmc->has_init=0;
	pinctrl_select_state(mmc->dev, "default");
	if (!mmc) {
		return -1;
	}
	ret = mmc_init(mmc);
	if (ret != 0) {
		return -1;
	}
	ret = storage_mmc_erase(init_flag, mmc);
	return ret;
}





uint64_t mmc_storage_get_part_size(const char *part_name) {

	struct partitions *part_info = NULL;

	part_info = find_mmc_partition_by_name(part_name);

	if (part_info == NULL) {
		printf("get partition info failed !!\n");
		return -1;
	}
	return part_info->size;
}

int mmc_storage_read(const char *part_name, loff_t off, size_t size, void *dest) {

	int ret=1;
	struct mmc *mmc;
	mmc = find_mmc_device(STORAGE_EMMC);

	if (!mmc)
		return 1;

	if (!part_name) {//the operating object is the device,the unit of operation is block.
		info_disprotect |= DISPROTECT_KEY;
		ret = blk_dread(mmc_get_blk_desc(mmc), off, size, dest);
		info_disprotect &= ~DISPROTECT_KEY;
		printf("%d blocks read: %s\n", ret, (ret == size) ? "OK" : "ERROR");
		return (ret == size) ? 0 : 1;

	} else {//the opering object is partition,the unit of operation is byte.
		ret = storage_read_in_part(part_name, off,size, dest);
	}

	return ret;

}


int mmc_storage_write(const char *part_name, loff_t off, size_t size, void *source) {

	int ret=1;

	struct mmc *mmc;
	mmc = find_mmc_device(STORAGE_EMMC);
	if (!mmc)
		return 1;

	if (!part_name) {//the operating object is the device,the unit of operation is block.
		info_disprotect |= DISPROTECT_KEY;
		ret = blk_dwrite(mmc_get_blk_desc(mmc), off, size, source);
		info_disprotect &= ~DISPROTECT_KEY;
		printf("%d blocks written: %s\n", ret, (ret == size) ? "OK" : "ERROR");
		return (ret == size) ? 0 : 1;
	} else {//the opering object is partition,the unit of operation is byte.
		ret = storage_write_in_part(part_name, off, size, source);
	}
	return ret;
}

int mmc_storage_erase(const char *part_name, loff_t off, size_t size, int scrub_flag) {

	int ret=1;
	struct mmc *mmc;
	mmc = find_mmc_device(STORAGE_EMMC);
	if (!mmc)
		return 1;

	if (!part_name) {//the operating object is the device,the unit of operation is block.
		ret = storage_mmc_erase_user(mmc);
		return (ret == 0) ? 0 : 1;
	} else {//the opering object is partition,the unit of operation is byte.
		ret = storage_erase_in_part(part_name, off, size);
	}
	return ret;
}

uint8_t mmc_storage_get_copies(const char *part_name) {

	char ret=3;

	return ret;

}


uint64_t mmc_get_copy_size(const char *part_name) {

	struct partitions *part_info = NULL;

#ifdef CONFIG_AML_GPT
	return UBOOT_SIZE*512;
#else
	part_info = find_mmc_partition_by_name("bootloader");
	if (part_info == NULL) {
		printf("get partition info failed !!\n");
		return -1;
	}
	return part_info->size;
#endif
}

/* dtb read&write operation with backup updates */
static u32 _calc_boot_info_checksum(struct storage_emmc_boot_info *boot_info)
{
	u32 *buffer = (u32*)boot_info;
	u32 checksum = 0;
	int i = 0;

	do {
		checksum += buffer[i];
	} while (i++ < ((EMMC_BOOT_INFO_SIZE >> 2) - 2));

	return checksum;
}

static int fill_mask8_part(struct part_property *mask8)
{
	struct partitions *part;
	int i = 0, mask8_cnt = 0;

	part = get_partition_info_by_num(i);
	while (part) {
		if ((part->mask_flags == 8)
			&& (mask8_cnt++ < BOOTINFO_MAX_PARTITIONS)) {
			strncpy(mask8->name, part->name, strlen(part->name));
			mask8->addr = part->offset / MMC_BLOCK_SIZE;
			mask8->size = part->size / MMC_BLOCK_SIZE;
			mask8++;
		}
		if (mask8_cnt == BOOTINFO_MAX_PARTITIONS)
			break;
		i++;
		part = get_partition_info_by_num(i);
	}

	return mask8_cnt;
}

static int amlmmc_write_info_sector(struct mmc *mmc)
{
	struct storage_emmc_boot_info *boot_info;
	struct virtual_partition *ddr_part;
	struct partitions *part;
	/* partitons with mask = 8 need to fill to bootinfo */
	struct part_property *mask8;
	int mask8_partition_count;
	u8 *buffer;
	int ret = 0, i;

	buffer = malloc(MMC_BLOCK_SIZE);
	if (!buffer)
		return -ENOMEM;

	memset(buffer, 0, sizeof(*boot_info));
	boot_info = (struct storage_emmc_boot_info *)buffer;
	part = aml_get_partition_by_name(MMC_RESERVED_NAME);
	boot_info->rsv_base_addr = part->offset / MMC_BLOCK_SIZE;
	ddr_part =  aml_get_virtual_partition_by_name(MMC_DDR_PARAMETER_NAME);
	boot_info->ddr.addr = ddr_part->offset / MMC_BLOCK_SIZE;
	boot_info->ddr.size = ddr_part->size / MMC_BLOCK_SIZE;

	mask8 = boot_info->parts;
	mask8_partition_count = fill_mask8_part(mask8);

	boot_info->version = 1;
	boot_info->checksum = _calc_boot_info_checksum(boot_info);

	printf("boot_info.rsv_base_addr:\t%04x\n", boot_info->rsv_base_addr);
	printf("boot_info.ddr.addr:%04x\n", boot_info->ddr.addr);
	printf("boot_info.ddr.size:%04x\n", boot_info->ddr.size);
	printf("boot info: parts %d\n", mask8_partition_count);
	for (i = 0; i < mask8_partition_count; i++) {
		printf("boot_info.part[%d]\n", i);
		printf("\t.name:%s\n", boot_info->parts[i].name);
		printf("\t.addr:%04x\n", boot_info->parts[i].addr);
		printf("\t.size:%04x\n", boot_info->parts[i].size);
	}
	printf("boot_info.version:%04x\n", boot_info->version);
	printf("boot_info.checksum:%04x\n", boot_info->checksum);

	if (blk_dwrite(mmc_get_blk_desc(mmc), 0, 1, buffer) != 1)
		ret = -EIO;

	free(buffer);
	return ret;
}

int mmc_boot_read(const char *part_name, uint8_t cpy, size_t size, void *dest) {

	char ret=1;
	int i;

	if (cpy == 0)
		cpy = 1;
	else if (cpy == 1)
		cpy = 2;
	else if (cpy == 2)
		cpy = 4;
	else if (cpy == 0xff)
		cpy = 7;
	for (i=0;i<3;i++) {//cpy:

		if (cpy & 1) {
			ret = blk_select_hwpart_devnum(IF_TYPE_MMC, STORAGE_EMMC, i);
			if (ret) goto R_SWITCH_BACK;
#ifdef CONFIG_AML_GPT
			if (i == 0)
				continue;
#endif
			ret = storage_read_in_part(part_name, 0, size, dest);

			if (ret != 0) {
				printf("storage read bootloader failed \n");
				goto R_SWITCH_BACK;
			}
		}
		cpy = cpy >> 1;
	}


R_SWITCH_BACK:
	ret = blk_select_hwpart_devnum(IF_TYPE_MMC, STORAGE_EMMC, USER_PARTITION);
	if (ret != 0) {
		printf("switch part  failed \n");
		return -1;
	}

	return ret;

}

int mmc_boot_write(const char *part_name, uint8_t cpy, size_t size, void *source) {

	char ret=1;
	int i = 0;
	struct mmc *mmc;

	mmc = find_mmc_device(STORAGE_EMMC);

	if (cpy == 0)
		cpy = 1;
	else if (cpy == 1)
		cpy = 2;
	else if (cpy == 2)
		cpy = 4;
	else if (cpy == 0xff)
		cpy = 7;

	for (i=0;i<3;i++) {//cpy:bin 100 is oprate boot1,bin 010 is oprate boot0,bin 001 is oprate user bootloader.bin 111 is operate all boot.

		if (cpy & 1) {
			ret = blk_select_hwpart_devnum(IF_TYPE_MMC, STORAGE_EMMC, i);
			if (ret) goto W_SWITCH_BACK;
#ifdef CONFIG_EMMC_BOOT1_TOUCH_REGION
			if (i == 2) {
				size = CONFIG_EMMC_BOOT1_TOUCH_REGION;
			}
#endif
#ifdef CONFIG_AML_GPT
			if (i == 0)
				continue;
#endif
			ret = storage_write_in_part(part_name, 0, size, source);

			if (ret != 0) {
				printf("storage write bootloader failed \n");
				goto W_SWITCH_BACK;
			}
			if (i != 0)
				amlmmc_write_info_sector(mmc);
		}
		cpy = cpy >> 1;
	}


W_SWITCH_BACK:
	ret = blk_select_hwpart_devnum(IF_TYPE_MMC, STORAGE_EMMC, USER_PARTITION);
	if (ret != 0) {
		printf("switch part failed \n");
		return -1;
	}

	return ret;

}

int mmc_boot_erase(const char *part_name, uint8_t cpy) {

	char ret=1;
	int i;
	size_t size = 0;

	if (cpy == 0)
		cpy = 1;
	else if (cpy == 1)
		cpy = 2;
	else if (cpy == 2)
		cpy = 4;
	else if (cpy == 0xff)
		cpy = 7;
	for (i=0;i<3;i++) {//cpy:

		if (cpy & 1) {
			ret = blk_select_hwpart_devnum(IF_TYPE_MMC, STORAGE_EMMC, i);
			if (ret) goto E_SWITCH_BACK;
#ifdef CONFIG_EMMC_BOOT1_TOUCH_REGION
			if (i == 2) {
				size = CONFIG_EMMC_BOOT1_TOUCH_REGION;
			}
#endif
#ifdef CONFIG_AML_GPT
			if (i == 0)
				continue;
#endif
			ret = storage_erase_in_part(part_name, 0, size);

			if (ret != 0) {
				printf("storage read bootloader failed \n");
				goto E_SWITCH_BACK;
			}
		}
		cpy = cpy >> 1;
	}


E_SWITCH_BACK:
	ret = blk_select_hwpart_devnum(IF_TYPE_MMC, STORAGE_EMMC, USER_PARTITION);
	if (ret != 0) {
		printf("switch part faild \n");
		return -1;
	}


	return ret;
}

uint32_t mmc_get_rsv_size(const char *rsv_name) {

	struct virtual_partition *vpart = NULL;
	vpart = aml_get_virtual_partition_by_name(rsv_name);
	printf("the %s partition size is:%llx   byte\n",rsv_name,vpart->size);
	return vpart->size;

}

static inline int env_read(size_t size, void *buf) {
	return storage_read_in_part("env", 0, size, buf);
}

static inline int env_write(size_t size, void *buf) {
	return storage_write_in_part("env", 0, size, buf);
}

int mmc_read_rsv(const char *rsv_name, size_t size, void *buf) {

	char ret=1;
	struct mmc *mmc;
	loff_t off =0;
	/*unsigned long dtImgAddr = simple_strtoul(buf, NULL, 16);*/
	ret = !strcmp("env", rsv_name) || !strcmp("key", rsv_name)
		|| !strcmp("dtb", rsv_name)||!strcmp("fastboot", rsv_name)
		||!strcmp("ddr-parameter", rsv_name);
	if (!ret) return 1;

	mmc = find_mmc_device(STORAGE_EMMC);
	if (!mmc) {
		puts("no mmc devices available\n");
		return 1;
	}

	if (!strcmp("env", rsv_name)) {
		ret = env_read(size, buf);
		return ret;
	}

	ret = storage_rsv_range_check(rsv_name, &size, &off);
	if (ret) return ret;
	if (!strcmp("dtb", rsv_name)) {
		ret = dtb_read(buf);
		return ret;
	}

	if (!strcmp("key", rsv_name))
		info_disprotect |= DISPROTECT_KEY;
	ret = storage_byte_read(mmc, off, size, buf);
	if (!strcmp("key", rsv_name))
		info_disprotect &= ~DISPROTECT_KEY;
	if (ret != 0) {
		printf("read resv failed\n");
	}

	return ret;
}
int mmc_write_rsv(const char *rsv_name, size_t size, void *buf) {

	char ret=1;

	struct mmc *mmc;
	loff_t off = 0;

	ret = !strcmp("env", rsv_name) || !strcmp("key", rsv_name)
		|| !strcmp("dtb", rsv_name)||!strcmp("fastboot", rsv_name)
		||!strcmp("ddr-parameter", rsv_name);
	if (!ret)
		return 1;
	mmc = find_mmc_device(STORAGE_EMMC);
	if (!mmc) {
		puts("no mmc devices available\n");
		return 1;
	}

	if (!strcmp("env", rsv_name)) {
		ret = env_write(size, buf);
		return ret;
	}

	ret = storage_rsv_range_check(rsv_name, &size, &off);
	if (ret) return ret;

	if (!strcmp("dtb", rsv_name)) {
		ret = dtb_write(buf);
		ret |= renew_partition_tbl(buf);
	}

	if (!strcmp("key", rsv_name))
		info_disprotect |= DISPROTECT_KEY;
	ret = storage_byte_write(mmc, off, size, buf);
	if (!strcmp("key", rsv_name))
		info_disprotect &= ~DISPROTECT_KEY;
	if (ret != 0) {
		printf("write resv failed\n");
	}

	return ret;
}
int mmc_erase_rsv(const char *rsv_name) {

	char ret=1;
	struct mmc *mmc;
	loff_t off = 0;
	size_t size = 0;
	ret = !strcmp("key", rsv_name) || !strcmp("dtb", rsv_name)
		||!strcmp("fastboot", rsv_name)
		||!strcmp("ddr-parameter", rsv_name);
	if (!ret) return 1;
	mmc = find_mmc_device(STORAGE_EMMC);
	if (!mmc) {
		puts("no mmc devices available\n");
		return 1;
	}
	ret = storage_rsv_range_check(rsv_name, &size, &off);
	if (ret) return ret;
	if (!strcmp("key", rsv_name))
		info_disprotect |= DISPROTECT_KEY;
	ret = storage_byte_erase(mmc, off, size);
	if (!strcmp("key", rsv_name))
		info_disprotect &= ~DISPROTECT_KEY;
	if (ret != 0) {
		printf("erase resv failed\n");
	}
	return ret;
}

int mmc_protect_rsv(const char *rsv_name, bool ops) {

	char ret=1;
	ret = strcmp("key", rsv_name);
	if (ret) return 1;

	if (ops) {

		info_disprotect &= ~DISPROTECT_KEY;
		printf("Protect the key partition!\n");
	} else {
		info_disprotect |= DISPROTECT_KEY;
		printf("Disprotect the key partition!\n");
	}
	return ret;

}

DECLARE_GLOBAL_DATA_PTR;
int sdcard_pre(void)
{
	return 0;
}


int sdcard_probe(uint32_t init_flag)
{
	return 0;
}

int emmc_pre(void)
{
	char ret = 1;
	struct mmc *mmc;
	mmc_initialize(gd->bd);
	mmc = find_mmc_device(STORAGE_EMMC);
	mmc->has_init = 0;
	ret = mmc_start_init(mmc);
	if (ret == 0)
			printf("emmc init success!\n");
	else
			printf("emmc init fail!\n");
	return ret;
}


int emmc_probe(uint32_t init_flag)
{
	char ret = 0;
	struct mmc *mmc;
	static struct storage_t *storage_dev = NULL;
	/*struct store_operation *storage_opera = NULL;*/
	storage_dev = kzalloc(sizeof(struct storage_t), GFP_KERNEL);
	if (storage_dev == NULL) {
		printf("malloc failed for storage_dev\n");
		ret = -1;
		goto exit_error;
	}
	mmc = find_mmc_device(STORAGE_EMMC);
	ret = mmc_storage_init(init_flag); /*flag 0*/
	if (ret) {
		printf("mmc init failed ret:%x\n", ret);
		goto exit_error;
	}
	/******basic info*******/
	storage_dev->init_flag = init_flag;
	storage_dev->type = BOOT_EMMC;

	printf("store flag: %d, types: %d\n",storage_dev->init_flag,storage_dev->type);
	/*storage_dev->info.name = mmc->cid[0] & 0xff,
		(mmc->cid[1] >> 24), (mmc->cid[1] >> 16) & 0xff,
		(mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff;
	storage_dev->info.id = mmc->cid[0] >> 24;*/
	storage_dev->info.read_unit = mmc->read_bl_len;
	storage_dev->info.write_unit = mmc->write_bl_len;
	storage_dev->info.erase_unit = mmc->erase_grp_size;
	storage_dev->info.caps = mmc->capacity_user;
	storage_dev->info.mode = COMPACT_BOOTLOADER;

	storage_dev->get_part_size = mmc_storage_get_part_size;
	storage_dev->read = mmc_storage_read;
	storage_dev->write = mmc_storage_write;
	storage_dev->erase = mmc_storage_erase;

	storage_dev->get_copies = mmc_storage_get_copies;
	storage_dev->get_copy_size = mmc_get_copy_size;
	storage_dev->boot_read = mmc_boot_read;
	storage_dev->boot_write = mmc_boot_write;
	storage_dev->boot_erase = mmc_boot_erase;

	storage_dev->get_rsv_size = mmc_get_rsv_size;
	storage_dev->read_rsv = mmc_read_rsv;
	storage_dev->write_rsv = mmc_write_rsv;
	storage_dev->erase_rsv = mmc_erase_rsv;
	storage_dev->protect_rsv = mmc_protect_rsv;
	store_register(storage_dev);
	printf("emmc probe success\n");

exit_error:
	return ret;
}

