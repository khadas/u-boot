// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <amlogic/storage.h>
#include <div64.h>
#include <linux/math64.h>
#include <amlogic/cpu_id.h>
#include <amlogic/store_wrapper.h>
#include <asm/arch/register.h>
#include <asm/arch/bl31_apis.h>
#include <amlogic/aml_efuse.h>
#include <asm/arch/cpu_config.h>
#include <asm/arch/romboot.h>
#include <asm/arch/secure_apb.h>
#include <amlogic/blxx2bl33_param.h>
#include <amlogic/aml_mtd.h>
#include <mmc.h>

#ifdef CONFIG_SPI_FLASH_MTD
extern int spi_nor_pre(void);
extern int spi_nor_probe(u32 init_flag);
#endif

#ifdef CONFIG_SPI_NAND
extern int spi_nand_pre(void);
extern int spi_nand_probe(u32 init_flag);
#endif

#ifdef CONFIG_MTD_SPI_NAND
extern int spi_nand_pre(void);
extern int spi_nand_probe(u32 init_flag);
#endif

#ifdef CONFIG_AML_NAND
extern int amlnf_pre(void);
extern int amlnf_probe(u32 init_flag);
#endif

#ifdef CONFIG_MESON_NFC
extern int nand_pre(void);
extern int nand_probe(uint32_t init_flag);
#endif

#ifdef CONFIG_MMC_MESON_GX
extern int emmc_pre(void);
extern int emmc_probe(u32 init_flag);
#endif

#ifdef CONFIG_MMC_MESON_GX
extern int sdcard_pre(void);
extern int sdcard_probe(u32 init_flag);
#endif
/* for info protect, fixme later */
int info_disprotect = 0;

static struct storage_t *current;
static struct device_node_t device_list[] = {
#ifdef CONFIG_MESON_NFC
	{BOOT_NAND_MTD, "mtd", nand_pre, nand_probe},
#endif
#ifdef CONFIG_AML_NAND
	{BOOT_NAND_NFTL, "nftl", amlnf_pre, amlnf_probe},
#endif
#ifdef CONFIG_SPI_NAND
	/* old drivers will be removed later */
	{BOOT_SNAND, "spi-nand", spi_nand_pre, spi_nand_probe},
#endif
#ifdef CONFIG_MTD_SPI_NAND
	{BOOT_SNAND, "spi-nand", spi_nand_pre, spi_nand_probe},
#endif
#ifdef CONFIG_SPI_FLASH_MTD
	{BOOT_SNOR, "spi-nor", spi_nor_pre, spi_nor_probe},
#endif
#if 0
	{BOOT_SD, "sd", sdcard_pre, sdcard_probe},
#endif

#ifdef CONFIG_MMC_MESON_GX
	{BOOT_EMMC, "emmc", emmc_pre, emmc_probe},
#endif

};

int store_register(struct storage_t *store_dev)
{
	if (!store_dev)
		return 1;
	if (!current) {
		INIT_LIST_HEAD(&store_dev->list);
		current = store_dev;
		return 0;
	}
	/**
	 * the head node will not be a valid node
	 * usually when we use the list, but in storage
	 * interface module, we init the device node as
	 * a head instead a global list_head pointer,
	 * it should be traver scaled.
	 */
	if (store_dev == current)
		return 0;
	struct storage_t *dev;

	if (store_dev->type == current->type)
		return 1;
	list_for_each_entry(dev, &current->list, list) {
		if (dev == store_dev)
			return 0;
		else if (dev->type == store_dev->type)
			return 1;
	}
	list_add_tail(&store_dev->list, &current->list);
	current = store_dev;
	return 0;
}

void store_unregister(struct storage_t *store_dev)
{
	if (store_dev == current) {
		if (list_empty_careful(&store_dev->list)) {
			current = NULL;
		} else {
			current = list_entry((current->list).next,
					     struct storage_t, list);
			list_del_init(&store_dev->list);
		}
	} else {
		list_del_init(&store_dev->list);
	}
}

int sheader_need(void)
{
	return BOOTLOADER_MODE_ADVANCE_INIT;
}

unsigned char *ubootdata = NULL;
void sheader_load(void *addr)
{
	ubootdata = addr;
}

/*
 * storage header which size is 512B
 * is bind into the tail of bl2.bin.
 * @addr: uboot address.
 */
static p_payload_info_t parse_uboot_sheader(void *addr)
{
	p_payload_info_t pInfo = (p_payload_info_t)(addr + BL2_SIZE);

	if (AML_MAGIC_HDR_L == pInfo->hdr.nMagicL &&
	    AML_MAGIC_HDR_R == pInfo->hdr.nMagicR) {
		printf("aml log : bootloader blxx mode!\n");
		return pInfo;
	}
	return NULL;
}

boot_area_entry_t general_boot_part_entry[MAX_BOOT_AREA_ENTRIES] = {
	{BOOT_BL2, BOOT_AREA_BB1ST, 0, 0},
	{BOOT_BL2E, BOOT_AREA_BL2E, 0, 0},
	{BOOT_BL2X, BOOT_AREA_BL2X, 0, 0},
	{BOOT_DDRFIP, BOOT_AREA_DDRFIP, 0, 0},
	{BOOT_DEVFIP, BOOT_AREA_DEVFIP, 0, 0},
};

struct boot_layout general_boot_layout = {.boot_entry = general_boot_part_entry};
struct storage_startup_parameter g_ssp;
struct storage_bl *g_storage = NULL;

static void storage_boot_layout_debug_info(
			struct boot_layout *boot_layout)
{
	boot_area_entry_t *boot_entry = boot_layout->boot_entry;
	int i;

	printf("boot area list: \n");
	for (i = 0; i < MAX_BOOT_AREA_ENTRIES && boot_entry[i].size; i++) {
		printf("%10s    ", boot_entry[i].name);
		printf("%10llx    ", boot_entry[i].offset);
		printf("%10llx\n", boot_entry[i].size);
	}
}

/* use STORAGE_ROUND_UP, y must be power of 2 */
#define STORAGE_ROUND_UP_IF_UNALIGN(x, y) ((x) = (((x) + (y) - 1) & (~(y - 1))))
#define NAND_RSV_OFFSET	1024
#define ALIGN_SIZE	(4096)
static int storage_boot_layout_rebuild(struct boot_layout *boot_layout,
				       unsigned int bl2e_size,
				       unsigned int bl2x_size)
{
	struct storage_startup_parameter *ssp = &g_ssp;
	boot_area_entry_t *boot_entry = boot_layout->boot_entry;
	uint64_t align_size, reserved_size = 0, cal_copy = ssp->boot_bakups;
	uint8_t i;

	align_size = ALIGN_SIZE;
	if ((ssp->boot_device == BOOT_NAND_NFTL) ||
		(ssp->boot_device == BOOT_NAND_MTD)) {
		reserved_size = ssp->sip.nsp.layout_reserve_size;
		align_size = ((NAND_RSV_OFFSET / cal_copy) * ssp->sip.nsp.page_size);
		printf("reserved_size:0x%llx 0x%llx\n", reserved_size, align_size);
	} else if (ssp->boot_device == BOOT_SNAND) {
		reserved_size = ssp->sip.snasp.layout_reserve_size;
		align_size = ((NAND_RSV_OFFSET / cal_copy) * ssp->sip.snasp.pagesize);
	} else 	if (ssp->boot_device == BOOT_EMMC) {
		ssp->boot_entry[0].offset = boot_entry[0].offset +=
			BL2_CORE_BASE_OFFSET_EMMC;
		cal_copy = 1;
	}
	STORAGE_ROUND_UP_IF_UNALIGN(boot_entry[0].size, align_size);
	ssp->boot_entry[0].size = boot_entry[0].size;
	printf("ssp->boot_entry[0] offset:0x%x, size:0x%x\n",
			ssp->boot_entry[0].offset, ssp->boot_entry[0].size);
	printf("cal_copy:0x%llx\n", cal_copy);
	printf("align_size:0x%llx\n", align_size);
	printf("reserved_size:0x%llx\n", reserved_size);
	if ((ssp->boot_device == BOOT_NAND_NFTL) ||
		(ssp->boot_device == BOOT_NAND_MTD))
		align_size = ssp->sip.nsp.block_size;
	else if (ssp->boot_device == BOOT_SNAND)
		align_size = (uint64_t)ssp->sip.snasp.pagesize *
			     ssp->sip.snasp.pages_per_eraseblock;
	printf("align_size2:%llu\n", align_size);

	boot_entry[BOOT_AREA_BL2E].size = bl2e_size;
	boot_entry[BOOT_AREA_BL2X].size = bl2x_size;

	for (i = 1; i < MAX_BOOT_AREA_ENTRIES && boot_entry[i - 1].size; i++) {
		STORAGE_ROUND_UP_IF_UNALIGN(boot_entry[i].size, align_size);
		boot_entry[i].offset = boot_entry[i-1].offset +
				boot_entry[i-1].size * cal_copy + reserved_size;
		reserved_size = 0;
		ssp->boot_entry[i].size = boot_entry[i].size;
		ssp->boot_entry[i].offset = boot_entry[i].offset;
	}

	return 0;
}

/* use STORAGE_ROUND_UP, y must be power of 2 */
#define STORAGE_ROUND_UP_IF_UNALIGN(x, y) ((x) = (((x) + (y) - 1) & (~(y - 1))))
#define NAND_RSV_OFFSET	1024
#define ALIGN_SIZE	(4096)
static int storage_boot_layout_general_setting(struct boot_layout *boot_layout,
					       int need_build)
{
	struct storage_startup_parameter *ssp = &g_ssp;
	boot_area_entry_t *boot_entry = boot_layout->boot_entry;
	struct storage_boot_entry *sbentry = ssp->boot_entry;
	p_payload_info_t pInfo = parse_uboot_sheader(ubootdata);;
	p_payload_info_hdr_t hdr = &pInfo->hdr;
	p_payload_info_item_t pItem = pInfo->arrItems;
	int offPayload = 0, szPayload = 0;
	unsigned int bl2e_size = 0, bl2x_size = 0;
	char name[8] = {0};
	int nIndex = 0;

	if (need_build == BOOT_ID_USB) {
		for (nIndex = 1, pItem += 1;
		     nIndex < hdr->byItemNum; ++nIndex, ++pItem) {
			memcpy(name, &pItem->nMagic, sizeof(unsigned int));
			offPayload = pItem->nOffset;
			if (nIndex == BOOT_AREA_BL2E)
				bl2e_size = pItem->nPayLoadSize;
			if (nIndex == BOOT_AREA_BL2X)
				bl2x_size = pItem->nPayLoadSize;
			szPayload = pItem->nPayLoadSize;
			pr_info("Item[%d]%4s offset 0x%08x sz 0x%x\n",
			       nIndex, name, offPayload, szPayload);
		}
		boot_entry[BOOT_AREA_BB1ST].size = ssp->boot_entry[BOOT_AREA_BB1ST].size;
		boot_entry[BOOT_AREA_DDRFIP].size = ssp->boot_entry[BOOT_AREA_DDRFIP].size;
		boot_entry[BOOT_AREA_DEVFIP].size = ssp->boot_entry[BOOT_AREA_DEVFIP].size;
		storage_boot_layout_rebuild(boot_layout, bl2e_size, bl2x_size);
	} else {
		/* may be sdcard boot and also have to rebuild layout */
		if (need_build == BOOT_ID_SDCARD) {
			bl2e_size = sbentry[BOOT_AREA_BL2E].size;
			bl2x_size = sbentry[BOOT_AREA_BL2X].size;
			printf("bl2e_size=%x bl2x_size=%x current->type=%d\n",
				bl2e_size, bl2x_size, current->type);
			boot_entry[BOOT_AREA_BB1ST].size =
				ssp->boot_entry[BOOT_AREA_BB1ST].size;
			boot_entry[BOOT_AREA_DDRFIP].size =
				ssp->boot_entry[BOOT_AREA_DDRFIP].size;
			boot_entry[BOOT_AREA_DEVFIP].size =
				ssp->boot_entry[BOOT_AREA_DEVFIP].size;
			storage_boot_layout_rebuild(boot_layout,
						    bl2e_size, bl2x_size);
			return 0;
		}
		/* normal boot */
		for (nIndex = 0;
		     nIndex < MAX_BOOT_AREA_ENTRIES && sbentry->size;
		     nIndex++, sbentry++) {
			boot_entry[nIndex].size = sbentry->size;
			boot_entry[nIndex].offset = sbentry->offset;
		}
	}

	return 0;
}

uint8_t emmc_boot_seqs_tbl[8][2] = {
		{0, 3}, {0, 2}, {0, 3}, {0, 1},
		{1, 2}, {1, 1}, {2, 1}, {0, 0}
	};

static int _get_emmc_boot_seqs(void)
{
	uint8_t ebcfg = 0;
	if (IS_FEAT_DIS_EMMC_USER())
		ebcfg |= (1<<2);
	if (IS_FEAT_DIS_EMMC_BOOT_0())
		ebcfg |= (1<<1);
	if (IS_FEAT_DIS_EMMC_BOOT_1())
		ebcfg |= (1<<0);

	return ebcfg;
}

static int storage_get_emmc_boot_seqs(void)
{
	return emmc_boot_seqs_tbl[_get_emmc_boot_seqs()][1];;
}

static int storage_get_emmc_boot_start(void)
{
	return emmc_boot_seqs_tbl[_get_emmc_boot_seqs()][0];;
}

#define NAND_RSV_BLOCK_NUM 48
#define NSP_PAGE0_DISABLE 1
extern unsigned char *ubootdata;
static int storage_get_and_parse_ssp(int *need_build) // boot_device:
{
	struct storage_startup_parameter *ssp = &g_ssp;
	union storage_independent_parameter *sip;
	static struct param_e *storage_param_e;
	int usb_boot = *need_build;

	memset(ssp, 0, sizeof(struct storage_startup_parameter));
	if (!usb_boot) {
		storage_param_e = param_of(STORAGE_PARAM_TPYE);
		if (!storage_param_e)
			return -1;
		memcpy(ssp, storage_param_e->data,
			sizeof(struct storage_startup_parameter));
		/* may be sdcard boot and also have to rebuild layout */
		if (ssp->boot_device == BOOT_ID_SDCARD ||
		    ssp->boot_device == BOOT_ID_USB) {
			/* need change the storage base here */
			*need_build = ssp->boot_device;
		}
	}

	if (*need_build) {
		sip = &ssp->sip;
		ssp->boot_device = current->type;
		switch (ssp->boot_device) {
		case BOOT_EMMC:
			ssp->boot_bakups = storage_get_emmc_boot_seqs();
			break;
		case BOOT_SNOR:
			if (IS_FEAT_EN_4BL2_SNOR())
				ssp->boot_bakups = 4;
			else if (IS_FEAT_DIS_NBL2_SNOR())
				ssp->boot_bakups = 1;
			else
				ssp->boot_bakups = 2; /* Default 2 backup, consistent with rom */
			break;
		case BOOT_SNAND:
			if (IS_FEAT_EN_8BL2_SNAND())
				ssp->boot_bakups = 8;
			else if (IS_FEAT_DIS_NBL2_SNAND())
				ssp->boot_bakups = 1;
			else
				ssp->boot_bakups = 4; /* Default 4 backup, consistent with rom */
			sip->snasp.pagesize = current->info.write_unit;
			sip->snasp.pages_per_eraseblock =
			current->info.erase_unit / current->info.write_unit;
			sip->snasp.eraseblocks_per_lun =
			(current->info.caps >> 20) / current->info.erase_unit;
			sip->snasp.planes_per_lun = 1;
			sip->snasp.luns_per_target = 1;
			sip->snasp.ntargets = 1;
			sip->snasp.layout_reserve_size =
				NAND_RSV_BLOCK_NUM * current->info.erase_unit;
			break;
		case BOOT_NAND_NFTL:
		case BOOT_NAND_MTD:
			ssp->boot_bakups = 8;
			if (IS_FEAT_DIS_8BL2_NAND())
				ssp->boot_bakups = 4;
			if (IS_FEAT_DIS_NBL2_NAND())
				ssp->boot_bakups = 1;
			sip->nsp.page_size =  current->info.write_unit;
			sip->nsp.block_size = current->info.erase_unit;
			sip->nsp.pages_per_block =
			current->info.erase_unit / current->info.write_unit;
			sip->nsp.layout_reserve_size =
				NAND_RSV_BLOCK_NUM * sip->nsp.block_size;
			sip->nsp.page0_disable =  NSP_PAGE0_DISABLE;
			break;
		default:
			/* do nothing. */
			break;
		}

	}

	/* sanity check */

	printf("boot_device:%d\n", ssp->boot_device);
	printf("boot_seq:%d\n", ssp->boot_seq);
	printf("boot_bakups:%d\n", ssp->boot_bakups);
	printf("rebuild_id :%d\n", *need_build);

	return 0;
}

int storage_post_init(void)
{
	int ret = -1;
	int need_build = 0;

	ret = storage_get_and_parse_ssp(&need_build);
	if (ret < 0)
		return -1;
	storage_boot_layout_general_setting(&general_boot_layout, need_build);
	storage_boot_layout_debug_info(&general_boot_layout);

	return ret;
}

int store_init(u32 init_flag)
{
	int i, ret = 0;
	u8 record = 0;

	/*1. pre scan*/
	for (i = 0; i < ARRAY_SIZE(device_list); i++) {
		if (!device_list[i].pre()) {
			record |= BIT(i);
		}
	}

	if (!record) {
		pr_info("No Valid storage device\n");
		return record;
	}

	if (BOOTLOADER_MODE_ADVANCE_INIT)
		storage_post_init();

	/*2. Enter the probe of the valid device*/
	for (i = 0; i < ARRAY_SIZE(device_list); i++) {
		if (record & BIT(i)) {
			ret = device_list[i].probe(init_flag);
			if (ret)
				pr_info("the 0x%x storage device probe failed\n",
			device_list[i].index);
		}
	}

	return record;
}

static struct storage_t *store_get_current(void)
{
	return current;
}

int store_set_device(enum boot_type_e type)
{
	struct list_head *entry;
	struct storage_t *dev, *store_dev = store_get_current();

	if (!store_dev) {
		pr_info("%s %d no current device\n", __func__, __LINE__);
		return 1;
	}
	if (store_dev->type == type)
		return 0;
	list_for_each(entry, &store_dev->list) {
		dev = list_entry(entry, struct storage_t, list);
		if (dev->type == type) {
			current = dev;
			return 0;
		}
	}
	pr_info("%s %d please confirm the %d device is valid\n",
		__func__, __LINE__, type);
	return 1;
}

enum boot_type_e store_get_type(void)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return BOOT_NONE;
	}

	return store->type;
}

int store_get_device_info(struct storage_info_t *info)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}

	memcpy((char *)info, (char *)&store->info,
	       sizeof(struct storage_info_t));
	return 0;
}

int store_get_device_bootloader_mode(void)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return -1;
	}
	return store->info.mode;
}

int store_read(const char *name, loff_t off, size_t size, void *buf)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	return store->read(name, off, size, buf);
}

int store_write(const char *name, loff_t off, size_t size, void *buf)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	return store->write(name, off, size, buf);
}

int store_erase(const char *name, loff_t off, size_t size, int scrub)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	return store->erase(name, off, size, scrub);
}

u64 store_part_size(const char *name)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	return store->get_part_size(name);
}

u8 store_boot_copy_num(const char *name)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	return store->get_copies(name);
}


#ifndef  SYSCTRL_SEC_STATUS_REG2
static u32 fake_reg = 0;
#define SYSCTRL_SEC_STATUS_REG2		(&fake_reg)
#endif
u8 store_boot_copy_start(void)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 0;
	}
	if (store->type != BOOT_EMMC)
		return 0;
	/* new arch since sc2 */
	if (BOOTLOADER_MODE_ADVANCE_INIT)
		return storage_get_emmc_boot_start();
	return 0;
}

u8 store_bootup_bootidx(const char *name)
{
	u8 bl2_idx = 0, fip_idx = 0;
	u32 val = 0;

	/* new arch since sc2 */
	if (BOOTLOADER_MODE_ADVANCE_INIT) {
			bl2_idx = readl(SYSCTRL_SEC_STATUS_REG2) & 0xF;
			//TODO: fixme after robust devfip is finished.
			fip_idx = bl2_idx;
	} else {
		/* accroding to the:
			commit 975b4acbcfa686601999d56843471d98e9c0a2cd
			storage: robust boot: record bootlog in SEC_AO_SEC_GP_CFG2 [1/2]
			PD#SWPL-4850
			...
			record the bootup bl2/fip into SEC_AO_SEC_GP_CFG2
			bit[27-25] bl2
			bit[24-22] fip
		*/
		val = readl(SEC_AO_SEC_GP_CFG2);
		bl2_idx = (val >> 25) & 0x7;
		fip_idx = (val >> 22) & 0x7;
	}
	if (!strncmp(name, "bl2", sizeof("bl2")) ||
			!strncmp(name, "spl", sizeof("spl")))
		return bl2_idx;
	else
		return fip_idx;
}

void store_restore_bootidx(void)
{
	/* new arch since sc2 */
	if (BOOTLOADER_MODE_ADVANCE_INIT) {
		extern void aml_set_bootsequence(uint32_t val);
		aml_set_bootsequence(0x55);
	}
	return;
}

u64 store_boot_copy_size(const char *name)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	return store->get_copy_size(name);
}

int store_boot_read(const char *name, u8 copy, size_t size, void *buf)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	return store->boot_read(name, copy, size, buf);
}

int store_boot_write(const char *name, u8 copy, size_t size, void *buf)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	return store->boot_write(name, copy, size, buf);
}

int store_boot_erase(const char *name, u8 copy)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	return store->boot_erase(name, copy);
}

int store_gpt_read(void *buf)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	if (!store->gpt_read)
		return 1;
	return store->gpt_read(buf);
}

int store_gpt_write(void *buf)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	if (!store->gpt_write)
		return 1;
	return store->gpt_write(buf);
}

int store_gpt_erase(void)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	if (!store->gpt_erase)
		return 1;
	return store->gpt_erase();
}

u32 store_rsv_size(const char *name)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	return store->get_rsv_size(name);
}

int store_rsv_read(const char *name, size_t size, void *buf)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	return store->read_rsv(name, size, buf);
}

int store_rsv_write(const char *name, size_t size, void *buf)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	return store->write_rsv(name, size, buf);
}

int store_rsv_erase(const char *name)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	return store->erase_rsv(name);
}

int store_rsv_protect(const char *name, bool ops)
{
	struct storage_t *store = store_get_current();

	if (!store) {
		pr_info("%s %d please init storage device first\n",
			__func__, __LINE__);
		return 1;
	}
	return store->protect_rsv(name, ops);
}

static int do_store_init(cmd_tbl_t *cmdtp,
			 int flag, int argc, char * const argv[])
{
	u32 init_flag = 1;
	u8 ret = 0;

	if (unlikely(argc != 2 && argc != 3))
		return CMD_RET_USAGE;

	if (argc == 3)
		init_flag = simple_strtoul(argv[2], NULL, 10);

	/*Returns a nonzero value: device index*/
	if (store_init(init_flag))
		ret = 0;
	else ret = 1;
	return ret;
}

void store_print_device(struct storage_t *store_dev)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(device_list); i++)
		if (store_dev->type & device_list[i].index)
			pr_info("device type: [%s]\n", device_list[i].type);
	pr_info("name %s\n", store_dev->info.name);
	pr_info("id :");
	for (i = 0; i < ARRAY_SIZE(store_dev->info.id); i++)
		pr_info(" 0x%x", store_dev->info.id[i]);
	pr_info("\n");
	pr_info("read unit %d\n", store_dev->info.read_unit);
	pr_info("write unit %d\n", store_dev->info.write_unit);
	pr_info("erase unit %d\n", store_dev->info.erase_unit);
	pr_info("total size %lld\n", store_dev->info.caps);
	if (store_dev->info.mode)
		pr_info("bootloader in discrete mode : %d\n",
			store_dev->info.mode);
	else
		pr_info("bootloader in compact mode : %d\n",
			store_dev->info.mode);
}

static int do_store_device(cmd_tbl_t *cmdtp,
			int flag, int argc, char * const argv[])
{
	if (argc == 2) {
		struct storage_t *store_dev, *dev;
		struct list_head *entry;

		store_dev = store_get_current();
		pr_info("current device:\n");
		pr_info("----------------------------------\n");
		store_print_device(store_dev);
		pr_info("----------------------------------\n");
		list_for_each(entry, &store_dev->list) {
			dev = list_entry(entry, struct storage_t, list);
			pr_info("valid device:\n");
			pr_info("----------------------------------\n");
			store_print_device(dev);
			pr_info("----------------------------------\n");
		}
		return 0;
	} else if (argc == 3) {
		char *name = NULL;
		int i = 0, ret = 0;
		name = argv[2];
		for (i = 0; i < ARRAY_SIZE(device_list); i++)
			if (!strcmp(name, device_list[i].type)) {

				ret = store_set_device(device_list[i].index);
				if (!ret) {
					pr_info("now current device is: %s\n",
						name);
					return 0;
				}
			}
		pr_info("%s %d no such device: %s\n",
			__func__, __LINE__, name);
		return ret;
	}
	return CMD_RET_USAGE;
}

static int do_store_partition(cmd_tbl_t *cmdtp,
			int flag, int argc, char * const argv[])
{
	struct storage_t *store_dev;
	int i = 0, partitions = 0;
	int ret = 0;
	char name[16];

	if (argc > 2)
		return CMD_RET_USAGE;
	else {
		store_dev = store_get_current();
		if (store_dev->get_part_count)
			partitions = store_dev->get_part_count();
		pr_info("%d partitions of device %s:\n",
			partitions, store_dev->info.name);

		if (store_dev->list_part_name)
			ret = store_dev->list_part_name(i, name);

		return ret;
	}
}

#ifdef CONFIG_AML_MTD
extern int is_mtd_store_boot_area(const char *part_name);
#endif
static int do_store_erase(cmd_tbl_t *cmdtp,
			  int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	unsigned long offset;
	size_t size = 0;
	char *name = NULL;
	char *s;
	int erase_flag = 0, ret;
	unsigned long time;

	const char *scrub =
		"Warning: scrub_flag is 1!!!!"
		"scrub operation!!!\n"
		"will erase oob area\n"
		"There is no reliable way to recover them.\n"
		"		  "
		"are sure of what you are doing!\n"
		"\nReally erase this NAND flash? <y/N>\n";

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
			__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (!strncmp(argv[1], "scrub", 5)) {
		erase_flag |= STORE_SCRUB;
		puts(scrub);
		if (!confirm_yesno()) {
			printf("erase aborted\n");
			return 1;
		}
	}

	/*store erase.chip*/
	s = strchr(argv[1], '.');
	if (s != NULL && strcmp(s, ".chip") == 0) {
		if (argc == 3 && !simple_strtoul(argv[argc - 1], NULL, 16))
			erase_flag |= STORE_ERASE_DATA;
		else if ((argc == 3) && (simple_strtoul(argv[argc - 1], NULL, 16) == 1))
			erase_flag |= STORE_ERASE_RSV;
		else if (argc == 3)
			return CMD_RET_USAGE;

		offset = 0;
	} else {
		/*store erase normal, partition name can't NULL*/
		if (unlikely(argc != 5))
			return CMD_RET_USAGE;

		size = (size_t)simple_strtoul(argv[argc - 1], NULL, 16);
		offset = simple_strtoul(argv[argc - 2], NULL, 16);
		name = argv[2];
#ifdef CONFIG_AML_MTD
		if (is_mtd_store_boot_area(name)) {
			pr_info("%s %d please enter normal partition name except tpl area!\n",
				__func__, __LINE__);
			return CMD_RET_FAILURE;
		}
#endif
	}

	time = get_timer(0);
	ret = store->erase(name, offset, size, erase_flag);
	time = get_timer(time);

	if (size != 0)
		printf("%lu bytes ", size);

	printf("erased in %lu ms", time);
	if ((time > 0) && (size != 0)) {
		puts(" (");
		print_size(div_u64(size, time) * 1000, "/s");
		puts(")");
	}
	puts("\n");

	return ret;
}

static int do_store_read(cmd_tbl_t *cmdtp,
			 int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	unsigned long offset, addr, time;
	size_t size;
	char *name = NULL;
	int ret;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
			__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (unlikely(argc != 5 && argc != 6))
		return CMD_RET_USAGE;

	addr = simple_strtoul(argv[2], NULL, 16);
	size = (size_t)simple_strtoul(argv[argc - 1], NULL, 16);
	offset = simple_strtoul(argv[argc - 2], NULL, 16);
	if (argc == 6)
		name = argv[3];
#ifdef CONFIG_AML_MTD
	if (is_mtd_store_boot_area(name)) {
			pr_info("%s %d please enter normal partition name except tpl area!\n",
				__func__, __LINE__);
			return CMD_RET_FAILURE;
		}
#endif
	time = get_timer(0);
	ret = store->read(name, offset, size, (u_char *)addr);
	time = get_timer(time);

	if (size != 0)
		pr_info("%lu bytes ", size);

	pr_info("read in %lu ms", time);
	if ((time > 0) && (size != 0)) {
		puts(" (");
		print_size(div_u64(size, time) * 1000, "/s");
		puts(")");
	}
	puts("\n");

	return ret;
}

static int name2index(struct boot_layout *boot_layout, const char *img)
{
	boot_area_entry_t *boot_entry = NULL;
	int i;

	boot_entry = boot_layout->boot_entry;
	for (i = 1; i < MAX_BOOT_AREA_ENTRIES && boot_entry[i].size; i++) {
		if (!strncmp(img, boot_entry[i].name, strlen(boot_entry[i].name)))
			return i;
	}

	return -1;
}

static int do_store_write_bl2img(cmd_tbl_t *cmdtp,
			  int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	unsigned long offset, addr;
	size_t size, size_src;
	char *name = NULL;
	int ret = -1, index;
	struct boot_layout *boot_layout = &general_boot_layout;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
			__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	addr = simple_strtoul(argv[2], NULL, 16);
	name = argv[3];
	size = simple_strtoul(argv[4], NULL, 16);

	index = name2index(&general_boot_layout, name);
	offset = boot_layout->boot_entry[index].offset;
	size_src = boot_layout->boot_entry[index].size;
	printf("[%s] offset:0x%lx, index:%d\n", name, offset, index);

	if (size_src != size)
		printf("new img size:0x%lx != img src:0x%lx\n", size, size_src);

	ret = store->boot_write(name, offset, size, (u_char *)addr);

	return ret;
}

int store_write_bl2img(void* addr, const char *name, size_t size)
{
	struct storage_t *store = store_get_current();
	unsigned long offset;
	size_t size_src;
	int ret = -1, index;
	struct boot_layout *boot_layout = &general_boot_layout;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
			__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	index = name2index(&general_boot_layout, name);
	offset = boot_layout->boot_entry[index].offset;
	size_src = boot_layout->boot_entry[index].size;
	printf("[%s] offset:0x%lx, index:%d\n", name, offset, index);

	if (size_src != size)
		printf("new img size:0x%zx != img src:0x%zx\n", size, size_src);

	ret = store->boot_write(name, offset, size, (u_char *)addr);
	if (size != 0)
		printf("[%s][%d]%lx bytes\n", __func__, __LINE__, size);

	return ret;
}

static int do_store_write(cmd_tbl_t *cmdtp,
			  int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	unsigned long offset, addr, time;
	size_t size;
	char *name = NULL;
	int ret;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
			__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (unlikely(argc != 5 && argc != 6))
		return CMD_RET_USAGE;

	addr = simple_strtoul(argv[2], NULL, 16);
	offset = simple_strtoul(argv[argc - 2], NULL, 16);
	size = (size_t)simple_strtoul(argv[argc - 1], NULL, 16);
	if (argc == 6)
		name = argv[3];
#ifdef CONFIG_AML_MTD
	if (is_mtd_store_boot_area(name)) {
			pr_info("%s %d please enter normal partition name except tpl area!\n",
				__func__, __LINE__);
			return CMD_RET_FAILURE;
		}
#endif
	time = get_timer(0);
	ret = store->write(name, offset, size, (u_char *)addr);
	time = get_timer(time);

	if (size != 0)
		printf("%lu bytes ", size);

	printf("write in %lu ms", time);
	if ((time > 0) && (size != 0)) {
		puts(" (");
		print_size(div_u64(size, time) * 1000, "/s");
		puts(")");
	}
	puts("\n");

	return ret;
}

static int do_store_boot_read(cmd_tbl_t *cmdtp,
			      int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	unsigned long addr;
	size_t size;
	u8 cpy;
	char *name;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
			__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (unlikely(argc != 6))
		return CMD_RET_USAGE;

	name = argv[2];
	addr = (unsigned long)simple_strtoul(argv[3], NULL, 16);
	cpy = (u8)simple_strtoul(argv[4], NULL, 16);
	size = (size_t)simple_strtoul(argv[5], NULL, 16);

	return store->boot_read(name, cpy, size, (u_char *)addr);
}

static int bl2x_mode_check_header(p_payload_info_t pInfo)
{
	p_payload_info_hdr_t hdr    = &pInfo->hdr;
	const int nItemNum = hdr->byItemNum;
	p_payload_info_item_t pItem = pInfo->arrItems;
	u8 i = 0;
	int sz_payload = 0;
	uint64_t align_size = 1;
	struct storage_startup_parameter *ssp = &g_ssp;
	uint64_t cal_copy = ssp->boot_bakups;

	printf("\naml log : info parse...\n");
	printf("\tsztimes : %s\n",hdr->szTimeStamp);
	printf("\tversion : %d\n",hdr->byVersion);
	printf("\tItemNum : %d\n",nItemNum);
	printf("\tSize    : %d(0x%x)\n",    hdr->nSize, hdr->nSize);
	if (nItemNum > 8 || nItemNum < 3) {
		pr_info("illegal nitem num %d\n", nItemNum);
		return __LINE__;
	}
	if (ssp->boot_device == BOOT_NAND_MTD)
		align_size = ((NAND_RSV_OFFSET / cal_copy) * ssp->sip.nsp.page_size);
	else if (ssp->boot_device == BOOT_SNAND)
		align_size = ((NAND_RSV_OFFSET / cal_copy) * ssp->sip.snasp.pagesize);

	sz_payload = pItem->nPayLoadSize;
	STORAGE_ROUND_UP_IF_UNALIGN(sz_payload, align_size);
	if (sz_payload > ssp->boot_entry[0].size)
		return __LINE__;
	if (ssp->boot_device == BOOT_NAND_MTD)
		align_size = ssp->sip.nsp.block_size;
	else if (ssp->boot_device == BOOT_SNAND)
		align_size = (uint64_t)ssp->sip.snasp.pagesize *
		ssp->sip.snasp.pages_per_eraseblock;
	++pItem;

	for (i = 1; i < nItemNum; i++, ++pItem) {
		sz_payload = pItem->nPayLoadSize;
		STORAGE_ROUND_UP_IF_UNALIGN(sz_payload, align_size);
		if (sz_payload > ssp->boot_entry[i].size)
			return __LINE__;
	}

	return 0;
}

static int _store_boot_write(const char *part_name, u8 cpy, size_t size, void *addr)
{
	enum boot_type_e medium_type = store_get_type();
	struct storage_startup_parameter *ssp = &g_ssp;


	int ret = 0;
	struct storage_t *store = store_get_current();
	int bl2_size = BL2_SIZE;
	int bl2_cpynum = 0;
	int tpl_per_size = CONFIG_TPL_SIZE_PER_COPY;
	int tpl_cpynum = 0;
	int bootloader_maxsize = 0;

	if (store_get_device_bootloader_mode() == COMPACT_BOOTLOADER)
		return store->boot_write(part_name, cpy, size, (u_char *)addr);

	if (BOOT_NAND_MTD == medium_type ||  BOOT_SNAND == medium_type)
		tpl_cpynum = CONFIG_NAND_TPL_COPY_NUM;
	else if (medium_type == BOOT_SNOR)
		tpl_cpynum = CONFIG_NOR_TPL_COPY_NUM;

	if (store_get_device_bootloader_mode() == ADVANCE_BOOTLOADER) {
		bl2_cpynum = ssp->boot_bakups;
	} else	{
		bootloader_maxsize = bl2_size + tpl_per_size;
		bl2_cpynum = CONFIG_BL2_COPY_NUM;
		if (size > bootloader_maxsize) {
			pr_info("bootloader sz 0x%lx too large,max sz 0x%x\n",
				size, bootloader_maxsize);
			return CMD_RET_FAILURE;
		}
	}

	if ((cpy >= tpl_cpynum || cpy >= bl2_cpynum) && (cpy != BOOT_OPS_ALL)) {
		pr_info("update copy %d invalid, must < min(%d, %d)\n",
			cpy, tpl_cpynum, bl2_cpynum);
		return CMD_RET_FAILURE;
	}

	p_payload_info_t pinfo = parse_uboot_sheader((u8 *)addr);

	if (!pinfo) {
		ret = store->boot_write("tpl", cpy, size - bl2_size, (u_char *)(addr +bl2_size));
		if (ret) {
			pr_info("failed update tpl\n");
			return CMD_RET_FAILURE;
		}
	} else {
		if (bl2x_mode_check_header(pinfo)) {
			pr_info("!!!warning bl2xx size is bigger than bl2x layout size\n");
			pr_info("please check bl2x,or erase flash and turn off\n");
			pr_info("then turn on, and update uboot again\n");
			return CMD_RET_FAILURE;
		}

		char name[8];
		int nindex = 0;
		p_payload_info_hdr_t hdr    = &pinfo->hdr;
		p_payload_info_item_t pitem = pinfo->arrItems;
		int off_payload = 0;
		int sz_payload = 0;

		memset(name, 0, 8);
		for (nindex = 1, pitem +=1; nindex < hdr->byItemNum; ++nindex, ++pitem) {
			memcpy(name, &pitem->nMagic, sizeof(unsigned int));
			off_payload = pitem->nOffset;
			sz_payload = pitem->nPayLoadSize;
			pr_info("item[%d]%4s offset 0x%08x sz 0x%x\n",
				nindex, name, off_payload, sz_payload);
			if (!sz_payload)
				continue;
			ret = store->boot_write(general_boot_part_entry[nindex].name, cpy, sz_payload, (u_char *)(addr + off_payload));
			if (ret) {
				pr_info("Fail in flash payload %s\n",name);
				return CMD_RET_FAILURE;
			}
		}
	}

	ret =  store->boot_write("bl2", cpy, bl2_size, (u_char *)addr);
	if (ret) {
		pr_info("Fail in flash payload bl2\n");
		return CMD_RET_FAILURE;
	}
	return ret;

}

static int do_store_boot_write(cmd_tbl_t *cmdtp,
			       int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	unsigned long addr;
	size_t size;
	u8 cpy = BOOT_OPS_ALL;
	char *name;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
			__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (unlikely(argc != 5 && argc != 6))
		return CMD_RET_USAGE;

	name = argv[2];
	addr = (unsigned long)simple_strtoul(argv[3], NULL, 16);
	size = (size_t)simple_strtoul(argv[argc - 1], NULL, 16);
	if (argc == 6)
		cpy = (u8)simple_strtoul(argv[4], NULL, 16);

	if (strcmp(name, "bootloader") == 0) {
		return _store_boot_write(name, cpy, size, (u_char *)addr);
	}

	return store->boot_write(name, cpy, size, (u_char *)addr);
}

static int do_store_boot_erase(cmd_tbl_t *cmdtp,
			       int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	u8 cpy = BOOT_OPS_ALL;
	char *name;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
			__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (unlikely(argc != 3 && argc != 4))
		return CMD_RET_USAGE;

	name = argv[2];
	if (argc == 4)
		cpy = (u8)simple_strtoul(argv[3], NULL, 16);

	return store->boot_erase(name, cpy);
}

static int do_store_gpt_read(cmd_tbl_t *cmdtp,
			 int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	unsigned long addr;
	int ret;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
			__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (unlikely(argc != 3))
		return CMD_RET_USAGE;

	addr = simple_strtoul(argv[2], NULL, 16);

	if (store->gpt_read) {
		ret = store->gpt_read((u_char *)addr);
		return ret;
	}

	printf("read gpt is not prepared\n");
	return CMD_RET_USAGE;
}

static int do_store_gpt_write(cmd_tbl_t *cmdtp,
			 int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	unsigned long addr;
	int ret;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
			__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (unlikely(argc != 3))
		return CMD_RET_USAGE;

	addr = simple_strtoul(argv[2], NULL, 16);

	if (store->gpt_write) {
		ret = store->gpt_write((u_char *)addr);
		return ret;
	}

	printf("write gpt is not prepared\n");
	return CMD_RET_USAGE;
}

static int do_store_gpt_erase(cmd_tbl_t *cmdtp,
			 int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	int ret;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
			__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (unlikely(argc != 2))
		return CMD_RET_USAGE;

	if (store->gpt_erase) {
		ret = store->gpt_erase();
		return ret;
	}

	printf("erase gpt is not prepared\n");
	return CMD_RET_USAGE;
}

static int do_store_rsv_ops(cmd_tbl_t *cmdtp,
			    int flag, int argc, char * const argv[])
{
	struct storage_t *store = store_get_current();
	char *name = NULL;

	if (!store) {
		pr_info("%s %d please init your storage device first!\n",
			__func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	if (!strcmp(argv[2], "erase")) {
		if (argc == 3)
			;
		else if (argc == 4)
			name = argv[3];
		else
			return CMD_RET_USAGE;
		return store->erase_rsv(name);
	} else if (!strcmp(argv[2], "read") ||
			   !strcmp(argv[2], "write")) {
		u8 cmd = strcmp(argv[2], "read") ? 0 : 1;
		unsigned long addr = simple_strtoul(argv[4], NULL, 16);
		size_t size = (size_t)simple_strtoul(argv[5], NULL, 16);

		name = argv[3];
		if (unlikely(argc != 6))
			return CMD_RET_USAGE;
		if (cmd)
			return store->read_rsv(name, size, (u_char *)addr);
		else
			return store->write_rsv(name, size, (u_char *)addr);
	} else if (!strcmp(argv[2], "protect")) {
		char *ops;
		flag = false;

		if (unlikely(argc != 4 && argc != 5))
			return CMD_RET_USAGE;

		name = (argc == 4) ? NULL : argv[3];
		ops = argv[argc - 1];
		if (!strcmp(ops, "on"))
			flag = true;
		else if (!strcmp(ops, "off"))
			flag = false;
		return store->protect_rsv(name, flag);
	}
	return CMD_RET_USAGE;
}

static int do_store_param_ops(cmd_tbl_t *cmdtp,
			    int flag, int argc, char * const argv[])
{
	boot_area_entry_t *boot_entry = general_boot_layout.boot_entry;
	char bufvir[128];
	int lenvir, i, re;
	char *p = bufvir;

	if (store_get_device_bootloader_mode() != ADVANCE_BOOTLOADER)
		return 0;

	lenvir = snprintf(bufvir, sizeof(bufvir), "%s", "mtdbootparts=aml-nand:");
	p += lenvir;
	re = sizeof(bufvir) - lenvir;

	for (i = BOOT_AREA_BL2E; i <= BOOT_AREA_DEVFIP; i++) {
		lenvir = snprintf(p, re, "%dk(%s),",
				 (int)(boot_entry[i].size / 1024),
				 boot_entry[i].name);
		re -= lenvir;
		p += lenvir;
	}
	p = bufvir;
	bufvir[strlen(p) - 1] = 0;	/* delete the last comma */
	env_set("mtdbootparts", p);

	return 0;
}

static cmd_tbl_t cmd_store_sub[] = {
	U_BOOT_CMD_MKENT(init, 4, 0, do_store_init, "", ""),
	U_BOOT_CMD_MKENT(device, 4, 0, do_store_device, "", ""),
	U_BOOT_CMD_MKENT(partition, 3, 0, do_store_partition, "", ""),
	U_BOOT_CMD_MKENT(scrub, 5, 0, do_store_erase, "", ""),
	U_BOOT_CMD_MKENT(erase, 5, 0, do_store_erase, "", ""),
	U_BOOT_CMD_MKENT(read, 6, 0, do_store_read, "", ""),
	U_BOOT_CMD_MKENT(write, 7, 0, do_store_write, "", ""),
	U_BOOT_CMD_MKENT(write_gpt, 3, 0, do_store_gpt_write, "", ""),
	U_BOOT_CMD_MKENT(read_gpt, 3, 0, do_store_gpt_read, "", ""),
	U_BOOT_CMD_MKENT(erase_gpt, 2, 0, do_store_gpt_erase, "", ""),
	U_BOOT_CMD_MKENT(write_bl2img, 5, 0, do_store_write_bl2img, "", ""),
	U_BOOT_CMD_MKENT(boot_read,	6, 0, do_store_boot_read, "", ""),
	U_BOOT_CMD_MKENT(boot_write, 6, 0, do_store_boot_write, "", ""),
	U_BOOT_CMD_MKENT(boot_erase, 4, 0, do_store_boot_erase, "", ""),
	U_BOOT_CMD_MKENT(rsv, 6, 0, do_store_rsv_ops, "", ""),
	U_BOOT_CMD_MKENT(param, 2, 0, do_store_param_ops, "", ""),
};

static int do_store(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	cmd_tbl_t *c;

	if (argc < 2)
		return CMD_RET_USAGE;

	c = find_cmd_tbl(argv[1], cmd_store_sub, ARRAY_SIZE(cmd_store_sub));
	if (c)
		return c->cmd(cmdtp, flag, argc, argv);

	return CMD_RET_USAGE;
}

U_BOOT_CMD(store, CONFIG_SYS_MAXARGS, 1, do_store,
	"STORE sub-system:",
	"store init [flag]\n"
	"	init storage device\n"
	"store device [name]\n"
	"	show or set storage device\n"
	"	'store device' command will list\n"
	"	all valid storage device and print.\n"
	"	'store device [name]' will set the\n"
	"	[name] device to the current device\n"
	"store partition\n"
	"	show partitions of current device\n"
	"store read addr [partition name] off size\n"
	"	read 'size' bytes from offset 'off'\n"
	"	of device/partition 'partition name' to.\n"
	"	address 'addr' of memory.\n"
	"	if partition name not value. read start with\n"
	"	offset in normal logic area,if tpl area exist\n"
	"	read offset at end of tpl area\n"
	"store write addr [partition name] off size\n"
	"	write 'size' bytes to offset 'off' of\n"
	"	device/partition [partition name] from\n"
	"	address 'addr' of memory.\n"
	"	if partition name not value. write start with\n"
	"	offset in normal logic area,if tpl area exist\n"
	"	write offset at end of tpl area\n"
	"store write_gpt addr\n"
	"   write gpt from address 'addr'\n"
	"store read_gpt addr\n"
	"   read gpt to address 'addr'\n"
	"store erase_gpt\n"
	"   erase primary and secondary gpt\n"
	"store erase partition name off size.\n"
	"	erase 'size' bytes from offset 'off'\n"
	"	of device/partition [partition name]\n"
	"	partition name must't NULL\n"
	"store scrub partition name off size.\n"
	"	erase 'size' bytes from offset 'off'\n"
	"	of device/partition [partition name]\n"
	"	includes oob area if the device has.\n"
	"	partition name must't NULL\n"
	"store erase.chip [flag]\n"
	"	erase all nand chip,except bad block\n"
	"	flag 0 erase all nand chip,except bootloader&rsv\n"
	"	flag 1 erase rsv\n"
	"store scrub.chip\n"
	"	erase all nand chip,include bad block\n"
	"store boot_read name addr copy size\n"
	"	read 'size' bytes from 'copy'th backup\n"
	"	in name partition, 'copy' can't be null.\n"
	"	name:\n"
	"	in discrete mode: 'bl2'/'tpl'(fip)\n"
	"	in compact mode: 'bootloader'\n"
	"store boot_write name addr [copy] size\n"
	"	write 'size' bytes to 'copy'th backup\n"
	"	in [name] partition from address\n"
	"	'addr' of memory. when the optional 'copy'\n"
	"	is null, it will writes to all copies\n"
	"	name:\n"
	"	in discrete mode:\n"
	"	'bl2/bl2e/bl2x/ddrfip/tpl(fip), only update part\n"
	"	'bootloader', update whole uboot.bin, in this case\n"
	"	@copy:if used, must < min(tplCpyNum, Bl2CpyNum), update only the specified copy\n"
	"	if not used, update all the copies of bl2 bl2e bl2x ddrfip tpl!\n"
	"	in compact mode: 'bootloader'\n"
	"store boot_erase name [copy]\n"
	"	erase the name info from 'copy'th backup\n"
	"	when the optional 'copy' not value, it\n"
	"	will erase all copies.\n"
	"	name:\n"
	"	in discrete mode: \n"
	"	'bl2'/'tpl'(fip): erase bl2/tpl partition\n"
	"	'bootloader':erase bl2 + tpl partition\n"
	"	in compact mode: 'bootloader'\n"
	"store rsv read name addr size\n"
	"	read 'size' bytes 'name' rsv info\n"
	"	to address 'addr' of memory\n"
	"	'name' could be key/dtb/env etc...\n"
	"store rsv write name addr size\n"
	"	write 'size' bytes 'name' rsv info\n"
	"	from address 'addr' of memory\n"
	"store rsv erase name\n"
	"	erase 'name' rsv info\n"
	"	name must't null\n"
	"store rsv protect name on/off\n"
	"	turn on/off the rsv info protection\n"
	"	name must't null\n"
	"store param\n"
	"	transfer bl2e/x ddrfip devfip size to kernel in such case like sc2"
);
