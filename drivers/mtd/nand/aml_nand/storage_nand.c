#include <common.h>
#include <dm.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/rawnand.h>
#include <amlogic/storage.h>
#include <amlogic/aml_mtd.h>
#include "aml_nand.h"
#include <dm/device.h>

static struct storage_t *slcnand_storage;

extern void mtd_store_init_map(void);
extern void mtd_store_set(struct mtd_info *mtd, int dev);
extern void mtd_store_mount_ops(struct storage_t* store);

static inline void set_slc_nand_storage(struct storage_t *slc_nand)
{
	slcnand_storage = slc_nand;
}

static inline struct storage_t *get_slc_nand_storage(void)
{
	return slcnand_storage;
}


int nand_pre(void)
{
	return 0;
}

extern struct mtd_info *nand_info[CONFIG_SYS_MAX_NAND_DEVICE];
//extern struct aml_nand_flash_dev aml_nand_flash_ids[];
int slcnand_fit_storage(void)
{
	struct storage_t *slc_nand = NULL;

	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(nand_info[0]);
	struct aml_nand_flash_dev *type = NULL;

	if (get_slc_nand_storage())
		return 0;

	type = aml_chip->platform->nand_flash_dev;

	if (slc_nand == NULL) {
		slc_nand = kzalloc(sizeof(struct storage_t), GFP_KERNEL);
		if (!slc_nand) {
		debug("%s %d no enough memory!\n", __func__, __LINE__);
		return -ENOMEM;
		}
	}

	slc_nand->type = BOOT_NAND_MTD;
	slc_nand->init_flag = 0;
	printf("storage dev type: 0x%x, storage device is slc NAND\n",slc_nand->type);

	memcpy(slc_nand->info.name, type->name, 32*sizeof(char));
	memcpy(slc_nand->info.id, type->id, 8);
	printf("name: %s\n",slc_nand->info.name);

	slc_nand->info.read_unit = type->pagesize;
	slc_nand->info.write_unit = type->pagesize;
	slc_nand->info.erase_unit = type->erasesize;
	slc_nand->info.caps = ((type->chipsize) << 20);
	printf("cap: 0x%llx\n", slc_nand->info.caps);
#ifdef CONFIG_DISCRETE_BOOTLOADER
	slc_nand->info.mode = 1;
#else
	slc_nand->info.mode = 0;
#endif

	set_slc_nand_storage(slc_nand);
	mtd_store_mount_ops(slc_nand);

	//mtd_store_set(nand_info[0], 0);

	mtd_store_init_map();

	return store_register(slc_nand);
}

extern int amlmtd_init;
extern void board_nand_init(void);
extern int meson_nfc_probe(struct udevice *dev);
int nand_probe(uint32_t init_flag)
{
	struct storage_t *slc_nand  = get_slc_nand_storage();

	if (slc_nand) {
		slc_nand->init_flag = init_flag;
		printf("nand probe success\n");
		return 0;
	}

	board_nand_init();
	slc_nand = get_slc_nand_storage();
	if (!slc_nand) {
		printf("%s %d can not get slc nand!\n",
			   __func__, __LINE__);
		return 1;
	}
	slc_nand->init_flag = init_flag;
	printf("nand probe success\n");
	return 0;
}

