
#include <common.h>
#include <dm.h>
#include <errno.h>
#include <malloc.h>
#include <spi.h>
#include <spi_flash.h>
#include <linux/mtd/partitions.h>
#include <linux/types.h>
#include <linux/sizes.h>
#include <malloc.h>
#include <linux/errno.h>
#include <mtd.h>
#include <amlogic/aml_mtd.h>

#define SPINOR_BOOTLOADER_SIZE (SZ_1M)
#define SPINOR_RSV_BLOCK_NUM	(4)

static int _spinor_add_partitions(struct mtd_info *mtd,
				  const struct mtd_partition *parts,
				  int nbparts)
{
	int part_num = 0, i = 0;
	struct mtd_partition *temp, *parts_nm;
	loff_t off;
#ifdef CONFIG_DISCRETE_BOOTLOADER
	part_num = nbparts + 2;
#else
	part_num = nbparts + 1;
#endif/* CONFIG_DISCRETE_BOOTLOADER */
	temp = kzalloc(sizeof(*temp) * part_num, GFP_KERNEL);
	memset(temp, 0, sizeof(*temp) * part_num);
	temp[0].name = BOOT_LOADER;
	temp[0].offset = 0;
	/* fixme, yyh */
	temp[0].size = SPINOR_BOOTLOADER_SIZE;
	if (temp[0].size % mtd->erasesize)
		WARN_ON(1);
	off = temp[0].size + SPINOR_RSV_BLOCK_NUM * mtd->erasesize;
#ifdef CONFIG_DISCRETE_BOOTLOADER
	temp[1].name = BOOT_TPL;
	temp[1].offset = off;
	temp[1].size = CONFIG_TPL_SIZE_PER_COPY * CONFIG_TPL_COPY_NUM;
	if (temp[1].size % mtd->erasesize)
		WARN_ON(1);
	parts_nm = &temp[2];
	off += temp[1].size;
#else
	parts_nm = &temp[1];
#endif/* CONFIG_DISCRETE_BOOTLOADER */
	for (; i < nbparts; i++) {
		if (!parts[i].name) {
			pr_err("name can't be null! ");
			pr_err("please check your %d th partition name!\n",
				 i + 1);
			return 1;
		}
		if ((off + parts[i].size) > mtd->size) {
			pr_err("%s %d over nand size!\n",
				__func__, __LINE__);
			return 1;
		}
		parts_nm[i].name = parts[i].name;
		parts_nm[i].offset = off;
		if (parts[i].size % mtd->erasesize) {
			pr_err("%s %d \"%s\" size auto align to block size\n",
				__func__, __LINE__, parts[i].name);
			parts_nm[i].size += parts[i].size % mtd->erasesize;
		}
		/* it's ok "+=" here because size has been set to 0 */
		parts_nm[i].size += parts[i].size;
		off += parts_nm[i].size;
		if (i == (nbparts - 1))
			parts_nm[i].size = mtd->size - off;
	}
	return add_mtd_partitions(mtd, temp, part_num);
}

extern struct mtd_partition *get_partition_table(int *partitions);
int spinor_add_partitions(struct mtd_info *mtd)
{
	struct mtd_partition *spiflash_partitions;
	int partition_count;

	spiflash_partitions = get_partition_table(&partition_count);

	return _spinor_add_partitions(mtd, spiflash_partitions,
			       partition_count);
}

int spinor_del_partitions(struct mtd_info *mtd)
{
	return del_mtd_partitions(mtd);
}