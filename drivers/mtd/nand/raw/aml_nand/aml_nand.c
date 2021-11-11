// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <environment.h>
#include <nand.h>
#include <asm/io.h>
#include <malloc.h>
#include <linux/err.h>
#include <asm/cache.h>
#include <amlogic/storage.h>
#include <linux/log2.h>
#include <dm/pinctrl.h>
#include <dm/uclass.h>
#include <dm/device.h>

#include "aml_nand.h"
#include <time.h> /*test*/

#define aml_nand_debug(a...)

extern int aml_nand_scan_shipped_bbt(struct mtd_info *mtd);

uint8_t nand_boot_flag = 0;
extern unsigned char pagelist_1ynm_hynix256_mtd[128];
extern struct hw_controller *controller;

#define NAND_CMD_SANDISK_DSP_OFF 0x25
#define	SANDISK_A19NM_4G 53
#define	INTEL_20NM 60

#define	SZ_1M	0x100000
extern struct mtd_info *nand_info[CONFIG_SYS_MAX_NAND_DEVICE];

struct aml_nand_ecclayout {
	struct nand_ecclayout *ptr_ecclayout;
	unsigned int oobsize;
};

#define NAND_ECCLAYOUT(__name, _eccbytes, _offset, _length)	\
static struct nand_ecclayout aml_nand_oob_##__name = {	\
	.eccbytes = _eccbytes,					\
	.oobfree = {{						\
		.offset = _offset,				\
		.length = _length				\
	}}							\
}

NAND_ECCLAYOUT(uboot, 84, 0, 6);
NAND_ECCLAYOUT(64, 56, 0, 8);
NAND_ECCLAYOUT(128, 120, 0, 8);
NAND_ECCLAYOUT(218, 200, 0, 8);
NAND_ECCLAYOUT(224, 208, 0, 8);
NAND_ECCLAYOUT(256, 240, 0, 16);
NAND_ECCLAYOUT(376, 352, 0, 16);
NAND_ECCLAYOUT(436, 352, 0, 16);
NAND_ECCLAYOUT(448, 416, 0, 16);
NAND_ECCLAYOUT(640, 608, 0, 16);
NAND_ECCLAYOUT(744, 700, 0, 16);
NAND_ECCLAYOUT(1280, 1200, 0, 32);
NAND_ECCLAYOUT(1664, 1584, 0, 32);

static struct aml_nand_ecclayout ecclayout[16] = {
	{&aml_nand_oob_64,  64 },
	{&aml_nand_oob_128, 128},
	{&aml_nand_oob_218, 218},
	{&aml_nand_oob_224, 224},
	{&aml_nand_oob_256, 256},
	{&aml_nand_oob_376, 376},
	{&aml_nand_oob_436, 436},
	{&aml_nand_oob_448, 448},
	{&aml_nand_oob_640, 640},
	{&aml_nand_oob_744, 744},
	{&aml_nand_oob_1280,1280},
	{&aml_nand_oob_1664,1664}
};

struct nand_ecclayout *aml_ecclayout_get(unsigned int oobsize)
{
	struct aml_nand_ecclayout *entry = ecclayout;
	int i;

	for (i = 0; i < 16; i++, entry++)
		if (entry->oobsize == oobsize)
			return entry->ptr_ecclayout;
	return NULL;
}

void aml_platform_get_user_byte(struct aml_nand_chip *aml_chip,
	unsigned char *oob_buf, int byte_num)
{
	int read_times = 0;
	unsigned int len = PER_INFO_BYTE / sizeof(unsigned int);

	while (byte_num > 0) {
		*oob_buf++ = (aml_chip->user_info_buf[read_times*len] & 0xff);
		byte_num--;
		if (aml_chip->user_byte_mode == 2) {
			*oob_buf++ =
			((aml_chip->user_info_buf[read_times*len] >> 8) & 0xff);
			byte_num--;
		}
		read_times++;
	}
}

void aml_platform_set_user_byte(struct aml_nand_chip *aml_chip,
	unsigned char *oob_buf, int byte_num)
{
	int write_times = 0;
	unsigned int len = PER_INFO_BYTE/sizeof(unsigned int);

	while (byte_num > 0) {
		aml_chip->user_info_buf[write_times*len] = *oob_buf++;
		byte_num--;
		if (aml_chip->user_byte_mode == 2) {
			aml_chip->user_info_buf[write_times*len] |=
				(*oob_buf++ << 8);
			byte_num--;
		}
		write_times++;
	}
}

int aml_nand_block_bad_scrub_update_bbt(struct mtd_info *mtd)
{
	return 0;
}

extern struct mtd_partition *get_aml_mtd_partition(void);
extern int get_aml_partition_count(void);
extern struct storage_startup_parameter g_ssp;
static int aml_nand_add_partition(struct aml_nand_chip *aml_chip)
{
	struct nand_chip *chip = &aml_chip->chip;
	struct mtd_info *mtd = &chip->mtd;
	struct aml_nand_platform *plat = aml_chip->platform;
#ifdef CONFIG_MTD_PARTITIONS
	struct mtd_partition *temp_parts = NULL;
	struct mtd_partition *parts;
	int nr, i, ret = 0;
	loff_t adjust_offset = 0;
	uint64_t part_size = 0;
	int reserved_part_blk_num = NAND_RSV_BLOCK_NUM;
	uint64_t fip_part_size = 0;
#ifndef CONFIG_NOT_SKIP_BAD_BLOCK
	int phys_erase_shift, error = 0, internal_part_count = 0;
	uint64_t start_blk = 0, part_blk = 0;
	loff_t offset;

	phys_erase_shift = fls(mtd->erasesize) - 1;
	if (phys_erase_shift < 0) {
		pr_info("%s %d can not get erase shift\n",
				__func__, __LINE__);
		return -EINVAL;
	}
#endif
	if (!strncmp((char*)plat->name, NAND_BOOT_NAME, strlen((const char*)NAND_BOOT_NAME))) {
		/* boot partition must be set as this because of romboot restrict */
		parts = kzalloc(sizeof(struct mtd_partition),
				GFP_KERNEL);
		if (!parts)
			return -ENOMEM;
		parts->name = NAND_BOOT_NAME;
		parts->offset = 0;
		parts->size = (mtd->writesize * 1024);
		nr = 1;
	} else {
		/* normal partitions */
		parts = get_aml_mtd_partition();
		nr = get_aml_partition_count();
		adjust_offset = 1024 * mtd->writesize + (loff_t)reserved_part_blk_num * mtd->erasesize;

		if (store_get_device_bootloader_mode() == COMPACT_BOOTLOADER)
			goto _COMPAT_BOOTLOADER;

		if (store_get_device_bootloader_mode() == ADVANCE_BOOTLOADER) {
			fip_part_size = g_ssp.boot_entry[BOOT_AREA_DEVFIP].size * CONFIG_NAND_TPL_COPY_NUM;
			adjust_offset = g_ssp.boot_entry[BOOT_AREA_DEVFIP].offset + fip_part_size;
			internal_part_count = 4;
		} else {
			fip_part_size = CONFIG_TPL_SIZE_PER_COPY * CONFIG_NAND_TPL_COPY_NUM;
			internal_part_count = 1;
		}

		for (i = 0; i < internal_part_count; i++) {
			temp_parts = parts + i;
			if (store_get_device_bootloader_mode() == ADVANCE_BOOTLOADER) {
				temp_parts->offset = g_ssp.boot_entry[i + 1].offset;
				if (i == internal_part_count -1)
					temp_parts->size = fip_part_size;
				else
					temp_parts->size = g_ssp.boot_entry[i + 1].size * g_ssp.boot_bakups;
			} else {
				temp_parts->offset = adjust_offset;
				temp_parts->size = fip_part_size;
				adjust_offset += fip_part_size;
			}
		}

_COMPAT_BOOTLOADER:
		for (i = internal_part_count; i < nr; i++) {
			temp_parts = parts + i;
			if (mtd->size < adjust_offset) {
				printf("%s %d error : over the nand size!!!\n",
				       __func__, __LINE__);
				return -ENOMEM;
			}
			temp_parts->offset = adjust_offset;
			part_size = temp_parts->size;
			if (i == nr - 1)
				part_size = mtd->size - adjust_offset;
#ifndef CONFIG_NOT_SKIP_BAD_BLOCK
			offset = 0;
			start_blk = 0;
			part_blk = part_size >> phys_erase_shift;

			do {
				offset = adjust_offset + start_blk *
					mtd->erasesize;
				error = mtd->_block_isbad(mtd, offset);
				if (error) {
					pr_info("%s:%d factory bad addr=%llx\n",
							__func__, __LINE__,
							(uint64_t)(offset >> phys_erase_shift));
					if (i != nr - 1) {
						adjust_offset += mtd->erasesize;
						continue;
					}
				}
				start_blk++;
			} while (start_blk < part_blk);
#endif
			if (temp_parts->name == NULL) {
				temp_parts->name = kzalloc(MAX_MTD_PART_NAME_LEN, GFP_KERNEL);
				if (!temp_parts->name)
					return -ENOMEM;
				sprintf((char *)temp_parts->name, "mtd%d", nr);
			}
			adjust_offset += part_size;
			temp_parts->size = adjust_offset - temp_parts->offset;
		}
	}
	ret = add_mtd_partitions(mtd, parts, nr);
	if (nr == 1)
		kfree(parts);
	return ret;
#else
	return add_mtd_device(mtd);
#endif
}

/*
void nand_get_chip(void *chip)
{

	struct aml_nand_chip *aml_chip = (struct aml_nand_chip *)chip;
	struct hw_controller *controller = aml_chip->controller;
	int ret = 0;

	ret = pinctrl_select_state(controller->device, "default");
	if (ret) {
		printf("ERROR get pinmux failed\n");
	}
	return;
}
*/

static void inline nand_release_chip(void)
{
	NFC_SEND_CMD_STANDBY(controller, 5);

	return;
}

static void aml_nand_select_chip(struct mtd_info *mtd, int chipnr)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);

	switch (chipnr) {
		case -1:
			nand_release_chip();
			break;
		case 0:
			//nand_get_chip(aml_chip);
			aml_chip->aml_nand_select_chip(aml_chip, chipnr);
			break;
		case 1:
		case 2:
		case 3:
			aml_chip->aml_nand_select_chip(aml_chip, chipnr);
			break;

		default:
			BUG();
	}
	return;
}

void aml_platform_cmd_ctrl(struct aml_nand_chip *aml_chip,
	int cmd, unsigned int ctrl)
{
	if (cmd == NAND_CMD_NONE)
		return;

	if (ctrl & NAND_CLE)
		cmd=NFC_CMD_CLE(controller->chip_selected, cmd);
	else
		cmd=NFC_CMD_ALE(controller->chip_selected, cmd);

	NFC_SEND_CMD(controller, cmd);
}

int aml_platform_wait_devready(struct aml_nand_chip *aml_chip, int chipnr)
{
	struct nand_chip *chip = &aml_chip->chip;
	struct mtd_info *mtd = &chip->mtd;
	unsigned time_out_cnt = 0;
	int status;

	/* wait until command is processed or timeout occures */
	aml_chip->aml_nand_select_chip(aml_chip, chipnr);

	NFC_SEND_CMD_IDLE(controller, 0);
	NFC_SEND_CMD_IDLE(controller, 0);
	while (NFC_CMDFIFO_SIZE(controller) > 0);

	if (aml_chip->ops_mode & AML_CHIP_NONE_RB) {
		aml_chip->aml_nand_command(aml_chip,
			NAND_CMD_STATUS, -1, -1, chipnr);
		udelay(2);
		NFC_SEND_CMD(controller, controller->chip_selected | IDLE | 0);
		NFC_SEND_CMD(controller, controller->chip_selected | IDLE | 0);
		while (NFC_CMDFIFO_SIZE(controller) > 0) ;

		do {
			status = (int)chip->read_byte(mtd);
			if (status & NAND_STATUS_READY)
				break;
			udelay(1);
		} while(time_out_cnt++ <= 0x1000); /*10ms max*/

		if (time_out_cnt > 0x1000)
		    return 0;
	} else {
		do {
			if (chip->dev_ready(mtd))
				break;
		} while(time_out_cnt++ <= 0x40000);

		if (time_out_cnt > 0x40000)
		return 0;
	}
	return 1;
}

void aml_nand_cmd_ctrl(struct mtd_info *mtd, int cmd,  unsigned int ctrl)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);

	aml_chip->aml_nand_cmd_ctrl(aml_chip, cmd, ctrl);
}

int aml_nand_wait(struct mtd_info *mtd, struct nand_chip *chip)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	int status[MAX_CHIP_NUM], i = 0, time_cnt = 0;
	struct aml_nand_platform *plat = aml_chip->platform;
	int read_status =0;
	/* Apply this short delay always to ensure that we do wait tWB in
	 * any case on any machine. */

	memset(status, 0, MAX_CHIP_NUM);
	ndelay(100);
	/*SET_CBUS_REG_MASK(PREG_PAD_GPIO3_O, 1 << 11);*/
	for (i = 0; i < controller->chip_num; i++) {
		if (aml_chip->valid_chip[i]) {
			/*active ce for operation chip and send cmd*/
			aml_chip->aml_nand_select_chip(aml_chip, i);

			NFC_SEND_CMD(controller,
				controller->chip_selected | IDLE | 0);
			NFC_SEND_CMD(controller,
				controller->chip_selected | IDLE | 0);
			while (NFC_CMDFIFO_SIZE(controller)>0) ;

			/*if ((state == FL_ERASING)
				&& (chip->options & NAND_IS_AND))
				aml_chip->aml_nand_command(aml_chip,
					NAND_CMD_STATUS_MULTI, -1, -1, i);
			else*/
			aml_chip->aml_nand_command(aml_chip,
				NAND_CMD_STATUS, -1, -1, i);

			NFC_SEND_CMD(controller,
				controller->chip_selected | IDLE | 0);
			NFC_SEND_CMD(controller,
				controller->chip_selected | IDLE | 0);
			while (NFC_CMDFIFO_SIZE(controller)>0) ;

			time_cnt = 0;
retry_status:
			while (time_cnt++ < 0x40000) {
				if (chip->dev_ready) {
					if (chip->dev_ready(mtd))
						break;
					udelay(2);
				} else {
					/*if(time_cnt == 1)*/
				udelay(2);
				if (chip->read_byte(mtd) & NAND_STATUS_READY)
					break;
					/*aml_chip->aml_nand_command(aml_chip,
						NAND_CMD_STATUS, -1, -1, i);
					udelay(50);*/

				}
				/*udelay(200);*/
			}
				status[i] = (int)chip->read_byte(mtd);
			/*printk("s:%x\n", status[i]);*/
			if ((read_status++ < 3) && (!(status[i] & NAND_STATUS_READY))) {
				printk("after wirte,read %d status =%d fail\n",
					read_status,status[i]);
				goto retry_status;
			}
			status[0] |= status[i];
		}
	}
	if (!strncmp((char*)plat->name,
		NAND_BOOT_NAME, strlen((const char*)NAND_BOOT_NAME)))
		status[0] = 0xe0;

	return status[0];
}

/*
 * CONFIG_SYS_NAND_RESET_CNT is used as a timeout mechanism when resetting
 * a flash.  NAND flash is initialized prior to interrupts so standard timers
 * can't be used.  CONFIG_SYS_NAND_RESET_CNT should be set to a value
 * which is greater than (max NAND reset time / NAND status read time).
 * A conservative default of 200000 (500 us / 25 ns) is used as a default.
 */
#ifndef CONFIG_SYS_NAND_RESET_CNT
#define CONFIG_SYS_NAND_RESET_CNT 200000
#endif
void aml_nand_base_command(struct aml_nand_chip *aml_chip,
	unsigned command, int column, int page_addr, int chipnr)
{
	struct nand_chip *chip = &aml_chip->chip;
	struct mtd_info *mtd = &chip->mtd;
	unsigned command_temp;
	unsigned pages_per_blk_shift, plane_page_addr = 0, plane_blk_addr = 0;

	pages_per_blk_shift = (chip->phys_erase_shift - chip->page_shift);
	uint32_t rst_sts_cnt = CONFIG_SYS_NAND_RESET_CNT;
	if (page_addr != -1) {
		page_addr /= aml_chip->plane_num;
		plane_page_addr =(page_addr & ((1 << pages_per_blk_shift) - 1));
		plane_blk_addr = (page_addr >> pages_per_blk_shift);
		plane_blk_addr = (plane_blk_addr << 1);
	}

	if (aml_chip->plane_num == 2) {
		switch (command) {
			case NAND_CMD_READ0:
				if ((aml_chip->mfr_type == NAND_MFR_MICRON)
				|| (aml_chip->mfr_type == NAND_MFR_INTEL))
					command_temp = command;
				else {
					command_temp =
					NAND_CMD_TWOPLANE_PREVIOS_READ;
					column = -1;
				}
				plane_page_addr |=
				(plane_blk_addr << pages_per_blk_shift);
				break;

			case NAND_CMD_TWOPLANE_READ1:
				command_temp = NAND_CMD_READ0;
				if ((aml_chip->mfr_type == NAND_MFR_MICRON)
				|| (aml_chip->mfr_type == NAND_MFR_INTEL))
					/*plane_page_addr |=
						((plane_blk_addr + 1) << 8);*/
					return;
				else
					plane_page_addr |=
					(plane_blk_addr << pages_per_blk_shift);
				break;

			case NAND_CMD_TWOPLANE_READ2:
				if ((aml_chip->mfr_type == NAND_MFR_MICRON)
				|| (aml_chip->mfr_type == NAND_MFR_INTEL))
					command_temp =
						NAND_CMD_PLANE2_READ_START;
				else
					command_temp = NAND_CMD_READ0;
				plane_page_addr |=
				((plane_blk_addr + 1) << pages_per_blk_shift);
				break;

			case NAND_CMD_SEQIN:
				command_temp = command;
				plane_page_addr |=
				(plane_blk_addr << pages_per_blk_shift);
				break;

			case NAND_CMD_TWOPLANE_WRITE2:
				if ((aml_chip->mfr_type == NAND_MFR_HYNIX)
				|| (aml_chip->mfr_type == NAND_MFR_SAMSUNG))
					command_temp = command;
				else
					command_temp =
						NAND_CMD_TWOPLANE_WRITE2_MICRO;
				plane_page_addr |=
				((plane_blk_addr + 1) << pages_per_blk_shift);
				break;

			case NAND_CMD_ERASE1:
				command_temp = command;
				plane_page_addr |=
				(plane_blk_addr << pages_per_blk_shift);
				break;

			case NAND_CMD_MULTI_CHIP_STATUS:
				command_temp = command;
				plane_page_addr |=
				(plane_blk_addr << pages_per_blk_shift);
				break;

			default:
				command_temp = command;
				break;

		}
		chip->cmd_ctrl(mtd,
			command_temp & 0xff,
			NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);

		/*
		if ((command_temp == NAND_CMD_SEQIN)
		|| (command_temp == NAND_CMD_TWOPLANE_WRITE2)
		|| (command_temp == NAND_CMD_READ0))
			printk("plane_page_addr:%x plane_blk_addr:%x cmd:%x\n",
				plane_page_addr, plane_blk_addr, command);
		*/

		if (column != -1 || page_addr != -1) {
			int ctrl = NAND_CTRL_CHANGE | NAND_NCE | NAND_ALE;
			/* Serially input address */
			if (column != -1) {
				/* Adjust columns for 16 bit buswidth */
				if (chip->options & NAND_BUSWIDTH_16 &&
				    !nand_opcode_8bits(command))
					column >>= 1;
				chip->cmd_ctrl(mtd, column, ctrl);
				ctrl &= ~NAND_CTRL_CHANGE;
				if (!nand_opcode_8bits(command))
					chip->cmd_ctrl(mtd, column >> 8, ctrl);
			}
			if (page_addr != -1) {
				chip->cmd_ctrl(mtd, plane_page_addr, ctrl);
				chip->cmd_ctrl(mtd,
				plane_page_addr >> 8, NAND_NCE | NAND_ALE);
				/* One more address cycle for devices > 128MiB*/
				if (chip->chipsize > (128 << 20))
					chip->cmd_ctrl(mtd,
				plane_page_addr >> 16, NAND_NCE | NAND_ALE);
			}
		}

		switch (command) {
			case NAND_CMD_READ0:
				plane_page_addr =
					page_addr % (1 << pages_per_blk_shift);

				if ((aml_chip->mfr_type == NAND_MFR_MICRON)
				|| (aml_chip->mfr_type == NAND_MFR_INTEL)) {
					plane_page_addr |=
				((plane_blk_addr + 1) << pages_per_blk_shift);
					command_temp = command;
					chip->cmd_ctrl(mtd,
					command_temp & 0xff,
					NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
				} else {
					command_temp =
					NAND_CMD_TWOPLANE_PREVIOS_READ;
					column = -1;
					plane_page_addr |=
				((plane_blk_addr + 1) << pages_per_blk_shift);
					chip->cmd_ctrl(mtd,
					command_temp & 0xff,
					NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
				}

				break;

			case NAND_CMD_TWOPLANE_READ1:
				if ((aml_chip->mfr_type == NAND_MFR_MICRON)
				|| (aml_chip->mfr_type == NAND_MFR_INTEL)) {
					page_addr = -1;
					column = -1;
				} else {
					command_temp = NAND_CMD_RNDOUT;
					page_addr = -1;
					chip->cmd_ctrl(mtd,
					command_temp & 0xff,
					NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
				}
				break;

			case NAND_CMD_TWOPLANE_READ2:
				if ((aml_chip->mfr_type == NAND_MFR_MICRON)
				|| (aml_chip->mfr_type == NAND_MFR_INTEL)) {
					page_addr = -1;
					column = -1;
				} else {
					command_temp = NAND_CMD_RNDOUT;
					page_addr = -1;
					chip->cmd_ctrl(mtd,
					command_temp & 0xff,
					NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
				}
				break;

			case NAND_CMD_ERASE1:
				if ((aml_chip->mfr_type == NAND_MFR_MICRON)
				|| (aml_chip->mfr_type == NAND_MFR_INTEL)) {
					command_temp = NAND_CMD_ERASE1_END;
					chip->cmd_ctrl(mtd,
					command_temp & 0xff,
					NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
				aml_chip->aml_nand_wait_devready(aml_chip,
					chipnr);
				}

				command_temp = command;
				chip->cmd_ctrl(mtd,
					command_temp & 0xff,
					NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
				plane_page_addr =
					page_addr % (1 << pages_per_blk_shift);
				plane_page_addr |=
				((plane_blk_addr + 1) << pages_per_blk_shift);
				break;

			default:
				column = -1;
				page_addr = -1;
				break;
		}

		if (column != -1 || page_addr != -1) {
			int ctrl = NAND_CTRL_CHANGE | NAND_NCE | NAND_ALE;

			/* Serially input address */
			if (column != -1) {
				/* Adjust columns for 16 bit buswidth */
				if (chip->options & NAND_BUSWIDTH_16 &&
				    !nand_opcode_8bits(command))
					column >>= 1;
				chip->cmd_ctrl(mtd, column, ctrl);
				ctrl &= ~NAND_CTRL_CHANGE;
				if (!nand_opcode_8bits(command))
					chip->cmd_ctrl(mtd, column >> 8, ctrl);
			}
			if (page_addr != -1) {
				/*plane_page_addr |=
					(1 << (pages_per_blk_shift + 1));
				BUG_ON((plane_page_addr & 0x7FF) == 0);*/

				chip->cmd_ctrl(mtd, plane_page_addr, ctrl);
				chip->cmd_ctrl(mtd, plane_page_addr >> 8,
					NAND_NCE | NAND_ALE);
				/* One more address cycle for devices > 128MiB*/
				if (chip->chipsize > (128 << 20))
					chip->cmd_ctrl(mtd,
				plane_page_addr >> 16, NAND_NCE | NAND_ALE);
			}
		}

		if ((command == NAND_CMD_RNDOUT)
		|| (command == NAND_CMD_TWOPLANE_READ2))
			chip->cmd_ctrl(mtd,
				NAND_CMD_RNDOUTSTART,
				NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
		else if ((command == NAND_CMD_TWOPLANE_READ1))
			chip->cmd_ctrl(mtd,
				NAND_CMD_RNDOUTSTART,
				NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);

		else if (command == NAND_CMD_READ0)
			chip->cmd_ctrl(mtd,
				NAND_CMD_READSTART,
				NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);

		chip->cmd_ctrl(mtd, NAND_CMD_NONE, NAND_NCE | NAND_CTRL_CHANGE);
	} else {
		chip->cmd_ctrl(mtd,
			command & 0xff,
			NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);

		if (column != -1 || page_addr != -1) {
			int ctrl = NAND_CTRL_CHANGE | NAND_NCE | NAND_ALE;

			/* Serially input address */
			if (column != -1) {
				/* Adjust columns for 16 bit buswidth */
				if (chip->options & NAND_BUSWIDTH_16 &&
				    !nand_opcode_8bits(command))
					column >>= 1;
				chip->cmd_ctrl(mtd, column, ctrl);
				ctrl &= ~NAND_CTRL_CHANGE;
				/* Only output a single addr
				 * cycle for 8bits opcodes.
				 */
				if (!nand_opcode_8bits(command))
					chip->cmd_ctrl(mtd, column >> 8, ctrl);
			}
			if (page_addr != -1) {

				chip->cmd_ctrl(mtd, page_addr, ctrl);
				chip->cmd_ctrl(mtd, page_addr >> 8,
					NAND_NCE | NAND_ALE);
				/* One more address cycle for devices > 128MiB*/
				if (chip->chipsize > (128 << 20))
					chip->cmd_ctrl(mtd,
					page_addr >> 16, NAND_NCE | NAND_ALE);
			}
		}
		if (command == NAND_CMD_RNDOUT)
			chip->cmd_ctrl(mtd,
				NAND_CMD_RNDOUTSTART,
				NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
		else if (command == NAND_CMD_READ0)
			chip->cmd_ctrl(mtd,
				NAND_CMD_READSTART,
				NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);

		chip->cmd_ctrl(mtd, NAND_CMD_NONE, NAND_NCE | NAND_CTRL_CHANGE);
	}

	/*
	 * program and erase have their own busy handlers
	 * status, sequential in, and deplete1 need no delay
	 */
	switch (command) {

	case NAND_CMD_CACHEDPROG:
	case NAND_CMD_PAGEPROG:
	case NAND_CMD_ERASE1:
	case NAND_CMD_ERASE2:
	case NAND_CMD_SEQIN:
	case NAND_CMD_RNDIN:
	case NAND_CMD_STATUS:
	case NAND_CMD_DEPLETE1:
		return;

		/*
		 * read error status commands require only a short delay
		 */
	case NAND_CMD_STATUS_ERROR:
	case NAND_CMD_STATUS_ERROR0:
	case NAND_CMD_STATUS_ERROR1:
	case NAND_CMD_STATUS_ERROR2:
	case NAND_CMD_STATUS_ERROR3:
		udelay(chip->chip_delay);
		return;

	case NAND_CMD_RESET:
		if (!aml_chip->aml_nand_wait_devready(aml_chip, chipnr))
			printk ("couldn`t found selected chip: %d ready\n",
				chipnr);
		chip->cmd_ctrl(mtd,
			NAND_CMD_STATUS,
			NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
		chip->cmd_ctrl(mtd,
			NAND_CMD_NONE, NAND_NCE | NAND_CTRL_CHANGE);
		while (!(chip->read_byte(mtd) & NAND_STATUS_READY) &&
			(rst_sts_cnt--));
		return;

	default:
		/*
		 * If we don't have access to the busy pin, we apply the given
		 * command delay
		 */
		break;
	}

	/* Apply this short delay always to ensure that we do wait tWB in
	 * any case on any machine. */
	ndelay(100);
}

void aml_nand_command(struct mtd_info *mtd,
	unsigned command, int column, int page_addr)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	struct nand_chip *chip = &aml_chip->chip;
	int i = 0, valid_page_num = 1;

	if (page_addr != -1) {
		valid_page_num = (mtd->writesize >> chip->page_shift);
		valid_page_num /= aml_chip->plane_num;
		aml_chip->page_addr = page_addr / valid_page_num;
	if (unlikely(aml_chip->page_addr >= aml_chip->internal_page_nums)) {
		/*internal_chip =
		aml_chip->page_addr / aml_chip->internal_page_nums;*/
		aml_chip->page_addr -= aml_chip->internal_page_nums;
		aml_chip->page_addr |=
		(1 << aml_chip->internal_chip_shift)*aml_chip->internal_chipnr;
	}
	} else
		aml_chip->page_addr = page_addr;

	/* Emulate NAND_CMD_READOOB */
	if (command == NAND_CMD_READOOB) {
		command = NAND_CMD_READ0;
		aml_chip->aml_nand_wait_devready(aml_chip, 0);
		aml_chip->aml_nand_command(aml_chip, command,
			column, aml_chip->page_addr, 0);
		return;
	}
	if (command == NAND_CMD_PAGEPROG)
		return;

	/*if (command == NAND_CMD_SEQIN) {
		aml_chip->aml_nand_select_chip(aml_chip, 0);
		aml_chip->aml_nand_command(aml_chip,
			command, column, page_addr, 0);
		return;
	}*/

	for (i=0; i<controller->chip_num; i++) {
		if (aml_chip->valid_chip[i]) {
			/*active ce for operation chip and send cmd*/
			aml_chip->aml_nand_wait_devready(aml_chip, i);
			aml_chip->aml_nand_command(aml_chip,
				command, column, aml_chip->page_addr, i);
		}
	}

	return;
}

int aml_nand_erase_cmd(struct mtd_info *mtd, int page)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	struct nand_chip *chip = mtd->priv;
	unsigned pages_per_blk_shift;
	unsigned vt_page_num, internal_chipnr = 1, page_addr, valid_page_num;
	unsigned i = 0, j = 0;
	unsigned block_addr;

	pages_per_blk_shift = (chip->phys_erase_shift - chip->page_shift);

	vt_page_num = (mtd->writesize / (1 << chip->page_shift));
	vt_page_num *= (1 << pages_per_blk_shift);
	/* printk("%s() page 0x%x\n", __func__, page);*/
	if (page % vt_page_num)
		return 1;
	/* fixme, skip bootloader */
	if (page < 1024)
		return 0;
	/* Send commands to erase a block */
	valid_page_num = (mtd->writesize >> chip->page_shift);

	block_addr = ((page / valid_page_num) >> pages_per_blk_shift);

	if (meson_rsv_erase_protect(aml_chip->rsv, block_addr) == -1) {
		printf("%s blk 0x%x is protected\n", __func__, block_addr);
		return 1;
	}

	valid_page_num /= aml_chip->plane_num;

	aml_chip->page_addr = page / valid_page_num;
	if (unlikely(aml_chip->page_addr >= aml_chip->internal_page_nums)) {
		//internal_chipnr =
		//	aml_chip->page_addr / aml_chip->internal_page_nums;
		aml_chip->page_addr -= aml_chip->internal_page_nums;
		aml_chip->page_addr |=
		(1 << aml_chip->internal_chip_shift) *aml_chip->internal_chipnr;
	}

	if (unlikely(aml_chip->ops_mode & AML_INTERLEAVING_MODE))
		internal_chipnr = aml_chip->internal_chipnr;
	else
		internal_chipnr = 1;

	for (i=0; i<controller->chip_num; i++) {
		if (aml_chip->valid_chip[i]) {
			aml_chip->aml_nand_select_chip(aml_chip, i);
			page_addr = aml_chip->page_addr;
			for (j=0; j<internal_chipnr; j++) {
				if (j > 0) {
					page_addr = aml_chip->page_addr;
					page_addr |=
					(1 << aml_chip->internal_chip_shift) *j;
				}
				aml_chip->aml_nand_command(aml_chip,
					NAND_CMD_ERASE1, -1, page_addr, i);
				aml_chip->aml_nand_command(aml_chip,
					NAND_CMD_ERASE2, -1, -1, i);
			}
		}
	}
	return 0;
}

void aml_nand_dma_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);

	aml_chip->aml_nand_dma_read(aml_chip, buf, len, 0);
}

void aml_nand_dma_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);

	aml_chip->aml_nand_dma_write(aml_chip, (unsigned char *)buf, len, 0);
}

int aml_nand_read_page_raw(struct mtd_info *mtd, struct nand_chip *chip,
	uint8_t *buf, int oob_required, int page)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	unsigned nand_page_size = aml_chip->page_size;
	unsigned nand_oob_size = aml_chip->oob_size;
	uint8_t *oob_buf = chip->oob_poi;
	int i, error = 0, j = 0, page_addr, internal_chipnr = 1;

	if (aml_chip->ops_mode & AML_INTERLEAVING_MODE)
		internal_chipnr = aml_chip->internal_chipnr;

	for (i = 0; i < controller->chip_num; i++) {
		if (aml_chip->valid_chip[i]) {
			page_addr = aml_chip->page_addr;
			for (j = 0; j < internal_chipnr; j++) {
				if (j > 0) {
					page_addr = aml_chip->page_addr;
					page_addr |=
				(1 << aml_chip->internal_chip_shift) * j;
					aml_chip->aml_nand_select_chip(aml_chip,
						i);
					aml_chip->aml_nand_command(aml_chip,
						NAND_CMD_READ0, 0, page_addr,i);
				}

				if (!aml_chip->aml_nand_wait_devready(aml_chip,
					i)) {
				printk ("didn't found selected chip:%dready\n",
					i);
					error = -EBUSY;
					goto exit;
				}

				if (aml_chip->ops_mode & AML_CHIP_NONE_RB)
					chip->cmd_ctrl(mtd,
					NAND_CMD_READ0 & 0xff,
					NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
				if (aml_chip->plane_num == 2) {
					aml_chip->aml_nand_command(aml_chip,
						NAND_CMD_TWOPLANE_READ1,
						0x00, page_addr, i);
					chip->read_buf(mtd,
						aml_chip->aml_nand_data_buf,
						(nand_page_size+nand_oob_size));
					memcpy(buf, aml_chip->aml_nand_data_buf,
						(nand_page_size+nand_oob_size));
					memcpy(oob_buf,
				aml_chip->aml_nand_data_buf + nand_page_size,
					nand_oob_size);

					oob_buf += nand_oob_size;
					buf += (nand_page_size + nand_oob_size);

					aml_chip->aml_nand_command(aml_chip,
						NAND_CMD_TWOPLANE_READ2,
						0x00, page_addr, i);
					chip->read_buf(mtd,
						aml_chip->aml_nand_data_buf,
						nand_page_size + nand_oob_size);
					memcpy(buf, aml_chip->aml_nand_data_buf,
						nand_page_size + nand_oob_size);
					memcpy(oob_buf,
				aml_chip->aml_nand_data_buf + nand_page_size,
						nand_oob_size);

					oob_buf += nand_oob_size;
					buf += (nand_page_size + nand_oob_size);
				} else if (aml_chip->plane_num == 1) {
					chip->read_buf(mtd,
						aml_chip->aml_nand_data_buf,
						nand_page_size + nand_oob_size);
					memcpy(buf, aml_chip->aml_nand_data_buf,
						nand_page_size);
					memcpy(oob_buf,
				aml_chip->aml_nand_data_buf + nand_page_size,
						nand_oob_size);
					oob_buf += nand_oob_size;
					buf += nand_page_size;
				} else {
					error = -ENODEV;
					goto exit;
				}
			}
		}
	}

exit:
	return error;
}

int aml_nand_write_page_raw(struct mtd_info *mtd,
	struct nand_chip *chip, const uint8_t *buf, int oob_required,
	int page)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	unsigned nand_page_size = aml_chip->page_size;
	unsigned nand_oob_size = aml_chip->oob_size;
	uint8_t *oob_buf = chip->oob_poi;
	int i, error = 0, j = 0, page_addr, internal_chipnr = 1;

	if (aml_chip->ops_mode & AML_INTERLEAVING_MODE)
		internal_chipnr = aml_chip->internal_chipnr;

	for (i=0; i<controller->chip_num; i++) {
	if (aml_chip->valid_chip[i]) {
		aml_chip->aml_nand_select_chip(aml_chip, i);
		page_addr = aml_chip->page_addr;
		for (j = 0; j < internal_chipnr; j++) {
			if (j > 0) {
				page_addr = aml_chip->page_addr;
				page_addr |=
				(1 << aml_chip->internal_chip_shift) *j;
				aml_chip->aml_nand_command(aml_chip,
				NAND_CMD_SEQIN, 0, page_addr, i);
			}

			if (aml_chip->plane_num == 2) {
				memcpy(aml_chip->aml_nand_data_buf,
					buf, nand_page_size);
		memcpy(aml_chip->aml_nand_data_buf + nand_page_size,
			oob_buf, nand_oob_size);
				chip->write_buf(mtd,
					aml_chip->aml_nand_data_buf,
					nand_page_size + nand_oob_size);
				aml_chip->aml_nand_command(aml_chip,
				NAND_CMD_DUMMY_PROGRAM, -1, -1, i);

				oob_buf += nand_oob_size;
				buf += nand_page_size;

		if (!aml_chip->aml_nand_wait_devready(aml_chip, i)) {
			printk ("didn't found selected chip:%d ready\n",
				i);
			error = -EBUSY;
			goto exit;
		}

		memcpy(aml_chip->aml_nand_data_buf,
			buf, nand_page_size);
		memcpy(aml_chip->aml_nand_data_buf + nand_page_size,
			oob_buf, nand_oob_size);
		aml_chip->aml_nand_command(aml_chip,
			NAND_CMD_TWOPLANE_WRITE2, 0x00, page_addr, i);
		chip->write_buf(mtd, aml_chip->aml_nand_data_buf,
			(nand_page_size + nand_oob_size));
		aml_chip->aml_nand_command(aml_chip,
			NAND_CMD_PAGEPROG, -1, -1, i);

				oob_buf += nand_oob_size;
				buf += nand_page_size;
			} else if (aml_chip->plane_num == 1) {
				memcpy(aml_chip->aml_nand_data_buf,
					buf, nand_page_size);
		memcpy(aml_chip->aml_nand_data_buf + nand_page_size,
			oob_buf, nand_oob_size);
				chip->write_buf(mtd,
					aml_chip->aml_nand_data_buf,
					nand_page_size + nand_oob_size);
			if (chip->cmdfunc == aml_nand_command)
				aml_chip->aml_nand_command(aml_chip,
					NAND_CMD_PAGEPROG,
					-1, -1, i);

			oob_buf += nand_oob_size;
			buf += nand_page_size;
			} else {
				error = -ENODEV;
				goto exit;
			}
		}
	}
	}
exit:
	return error;
}

int aml_nand_read_page_hwecc(struct mtd_info *mtd,
	struct nand_chip *chip, uint8_t *buf, int oob_required, int page)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	uint8_t *oob_buf = chip->oob_poi;
	unsigned nand_page_size = (1 << chip->page_shift);
	unsigned pages_per_blk_shift;
	int user_byte_num;
	int error = 0, i = 0, stat = 0, j = 0, page_addr, internal_chipnr = 1;
	int ran_mode = aml_chip->ran_mode;

	pages_per_blk_shift = (chip->phys_erase_shift - chip->page_shift);
	user_byte_num = (((nand_page_size + chip->ecc.size - 1) /chip->ecc.size)
		* aml_chip->user_byte_mode);

	if (aml_chip->ops_mode & AML_INTERLEAVING_MODE)
		internal_chipnr = aml_chip->internal_chipnr;

	if (nand_page_size > chip->ecc.steps * chip->ecc.size) {
		nand_page_size = chip->ecc.steps * chip->ecc.size;
		user_byte_num = chip->ecc.steps;
	}

	for (i = 0; i < controller->chip_num; i++) {
	if (aml_chip->valid_chip[i]) {
		page_addr = aml_chip->page_addr;
		for (j = 0; j < internal_chipnr; j++) {
		if (j > 0) {
			page_addr = aml_chip->page_addr;
			page_addr |= (1 << aml_chip->internal_chip_shift) * j;
			aml_chip->aml_nand_select_chip(aml_chip, i);
			aml_chip->aml_nand_command(aml_chip, NAND_CMD_READ0,
				0, page_addr, i);
		}
		if (!aml_chip->aml_nand_wait_devready(aml_chip, i)) {
			printk ("read couldn`t found selected chip: %d ready\n",
				i);
			error = -EBUSY;
			goto exit;
		}
		if (aml_chip->ops_mode & AML_CHIP_NONE_RB)
			chip->cmd_ctrl(mtd, NAND_CMD_READ0 & 0xff,
				NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
		if (aml_chip->plane_num == 2) {
			aml_chip->aml_nand_command(aml_chip,
				NAND_CMD_TWOPLANE_READ1, 0x00, page_addr, i);
dma_retry_plane0:
			error = aml_chip->aml_nand_dma_read(aml_chip,
				buf, nand_page_size, aml_chip->bch_mode);
			if (error)
				goto exit;

			aml_chip->aml_nand_get_user_byte(aml_chip,
				oob_buf, user_byte_num);
			stat = aml_chip->aml_nand_hwecc_correct(aml_chip,
				buf, nand_page_size, oob_buf);
			if (stat < 0) {
				if (aml_chip->ran_mode
				&& (aml_chip->zero_cnt <  aml_chip->ecc_max)) {
					memset(buf, 0xff, nand_page_size);
					memset(oob_buf, 0xff, user_byte_num);
					goto plane0_ff;
				}

				if (ran_mode && aml_chip->ran_mode) {
					aml_chip->ran_mode = 0;
					ndelay(300);
					aml_chip->aml_nand_command(aml_chip,
						NAND_CMD_RNDOUT, 0, -1, i);
					ndelay(500);
					goto dma_retry_plane0;
				 }
				memset(buf, 0xff, nand_page_size);
				memset(oob_buf, 0xff, user_byte_num);

				mtd->ecc_stats.failed++;
				printk("read ecc pl0 failed at page%d chip%d\n",
					page_addr, i);
			} else {
			if (aml_chip->ecc_cnt_cur > aml_chip->ecc_cnt_limit) {
	printk("%s %d uncorrect ecc_cnt_cur:%d limit:%d pg:%d,blk:%d chip%d\n",
				__func__, __LINE__,
				aml_chip->ecc_cnt_cur, aml_chip->ecc_cnt_limit,
				page_addr, page_addr >> pages_per_blk_shift, i);
				mtd->ecc_stats.corrected++;
			}
				mtd->ecc_stats.corrected += stat;
			}
plane0_ff:
			aml_chip->ran_mode = ran_mode;
			oob_buf += user_byte_num;
			buf += nand_page_size;

			aml_chip->aml_nand_command(aml_chip,
				NAND_CMD_TWOPLANE_READ2, 0x00, page_addr, i);
dma_retry_plane1:
			error = aml_chip->aml_nand_dma_read(aml_chip,
				buf, nand_page_size, aml_chip->bch_mode);
			if (error)
				goto exit;

			aml_chip->aml_nand_get_user_byte(aml_chip,
					oob_buf, user_byte_num);
			stat = aml_chip->aml_nand_hwecc_correct(aml_chip,
					buf, nand_page_size, oob_buf);
			if (stat < 0) {
				if(aml_chip->ran_mode
				&& (aml_chip->zero_cnt <  aml_chip->ecc_max)) {
					memset(buf, 0xff, nand_page_size);
					memset(oob_buf, 0xff, user_byte_num);
					oob_buf += user_byte_num;
					buf += nand_page_size;
					continue;
				}

				if (ran_mode && aml_chip->ran_mode) {
					aml_chip->ran_mode = 0;
					ndelay(300);
					aml_chip->aml_nand_command(aml_chip,
						NAND_CMD_RNDOUT, 0, -1, i);
					ndelay(500);
					goto dma_retry_plane1;
				 }
				memset(buf, 0xff, nand_page_size);
				memset(oob_buf, 0xff, user_byte_num);

				mtd->ecc_stats.failed++;
				printk("read ecc pl1 failed at page%d chip%d\n",
					page_addr, i);
			} else {
			if (aml_chip->ecc_cnt_cur > aml_chip->ecc_cnt_limit) {
	printk("%s %d uncorrect ecc_cnt_cur:%d limit:%d pg:%d blk:%d chip%d\n",
				__func__, __LINE__,
				aml_chip->ecc_cnt_cur, aml_chip->ecc_cnt_limit,
				page_addr, page_addr >> pages_per_blk_shift, i);
				mtd->ecc_stats.corrected++;
			}
				mtd->ecc_stats.corrected += stat;
			}
			aml_chip->ran_mode = ran_mode;
			oob_buf += user_byte_num;
			buf += nand_page_size;

		} else if (aml_chip->plane_num == 1) {
			error = aml_chip->aml_nand_dma_read(aml_chip,
				buf, nand_page_size, aml_chip->bch_mode);
			if (error)
				goto exit;

			aml_chip->aml_nand_get_user_byte(aml_chip,
				oob_buf, user_byte_num);
			stat = aml_chip->aml_nand_hwecc_correct(aml_chip,
				buf, nand_page_size, oob_buf);
			if (stat < 0) {
				if(aml_chip->ran_mode
				&& (aml_chip->zero_cnt <  aml_chip->ecc_max)) {
					memset(buf, 0xff, nand_page_size);
					memset(oob_buf, 0xff, user_byte_num);
					oob_buf += user_byte_num;
					buf += nand_page_size;
					continue;
				}
				/*memset(buf, 0xff, nand_page_size);*/
				memset(oob_buf, 0x22, user_byte_num);
	printk("%s %d read ecc failed here at at page:%d, blk:%d chip[%d]\n",
		__func__, __LINE__, page_addr,
		(page_addr >> pages_per_blk_shift), i);
				mtd->ecc_stats.failed++;
			} else {
				aml_chip->ran_mode = ran_mode;
				mtd->ecc_stats.corrected += stat;
			}
			oob_buf += user_byte_num;
			buf += nand_page_size;
		} else {
			error = -ENODEV;
			goto exit;
		}
		}
	}
	}
exit:
	return error;
}

int aml_nand_write_page_hwecc(struct mtd_info *mtd,
	struct nand_chip *chip, const uint8_t *buf, int oob_required,
	int page)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	uint8_t *oob_buf = chip->oob_poi;
	unsigned nand_page_size = (1 << chip->page_shift);
	int user_byte_num, temp_value;
	int error = 0, i = 0, j = 0, page_addr, internal_chipnr = 1;

	temp_value = nand_page_size + chip->ecc.size - 1;
	user_byte_num = (temp_value /chip->ecc.size) * aml_chip->user_byte_mode;

	if (aml_chip->ops_mode & AML_INTERLEAVING_MODE)
		internal_chipnr = aml_chip->internal_chipnr;

	memset(oob_buf + mtd->oobavail,
		0xa5, user_byte_num * (mtd->writesize / nand_page_size));

	for (i = 0; i < controller->chip_num; i++) {
		if (aml_chip->valid_chip[i]) {
		page_addr = aml_chip->page_addr;
		for (j = 0; j < internal_chipnr; j++) {
		aml_chip->aml_nand_select_chip(aml_chip, i);
		if (j > 0) {
			page_addr = aml_chip->page_addr;
			page_addr |=
				(1 <<aml_chip->internal_chip_shift) * j;
			aml_chip->aml_nand_command(aml_chip,
			NAND_CMD_SEQIN, 0, page_addr, i);
		}
		if (aml_chip->plane_num == 2) {
			aml_chip->aml_nand_set_user_byte(aml_chip,
				oob_buf, user_byte_num);
			error = aml_chip->aml_nand_dma_write(aml_chip,
				(unsigned char *)buf,
				nand_page_size, aml_chip->bch_mode);
			if (error) {
				printk("dma write 1 err at page %x\n",
					page_addr);
				goto exit;
			}
			aml_chip->aml_nand_command(aml_chip,
				NAND_CMD_DUMMY_PROGRAM, -1, -1, i);

			oob_buf += user_byte_num;
			buf += nand_page_size;

			if (!aml_chip->aml_nand_wait_devready(aml_chip, i)) {
				printk ("write couldn't found chip:%d ready\n",
					i);
				error = -EBUSY;
				goto exit;
			}

			aml_chip->aml_nand_command(aml_chip,
				NAND_CMD_TWOPLANE_WRITE2, 0x00, page_addr, i);
			aml_chip->aml_nand_set_user_byte(aml_chip,
				oob_buf, user_byte_num);
			error = aml_chip->aml_nand_dma_write(aml_chip,
				(u8 *)buf,
				nand_page_size, aml_chip->bch_mode);
			if (error) {
				printk("aml_nand_dma_write 2 err at page %x\n",
					page_addr);
				goto exit;
			}
			if (aml_chip->cached_prog_status)
				aml_chip->aml_nand_command(aml_chip,
					NAND_CMD_CACHEDPROG, -1, -1, i);
			else
				aml_chip->aml_nand_command(aml_chip,
					NAND_CMD_PAGEPROG, -1, -1, i);

			oob_buf += user_byte_num;
			buf += nand_page_size;
		} else if (aml_chip->plane_num == 1) {
			aml_chip->aml_nand_set_user_byte(aml_chip,
				oob_buf, user_byte_num);
			error = aml_chip->aml_nand_dma_write(aml_chip,
				(unsigned char *)buf,
				nand_page_size, aml_chip->bch_mode);
			if (error) {
				printk("aml_nand_dma_write err at page %x\n",
					page_addr);
				goto exit;
			}

			if (chip->cmdfunc == aml_nand_command) {
				if (aml_chip->cached_prog_status)
					aml_chip->aml_nand_command(aml_chip,
						NAND_CMD_CACHEDPROG, -1, -1, i);
				else
					aml_chip->aml_nand_command(aml_chip,
						NAND_CMD_PAGEPROG, -1, -1, i);
			}

			oob_buf += user_byte_num;
			buf += nand_page_size;
		}
		else {
			error = -ENODEV;
			goto exit;
		}
		}
		}
	}
exit:
	return error;
}

int aml_nand_write_page(struct mtd_info *mtd,
	struct nand_chip *chip, uint32_t offset,
	int data_len,
	const uint8_t *buf,
	int oob_required, int page, int raw)
{
	int status;
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);

	chip->cmdfunc(mtd, NAND_CMD_SEQIN, 0x00, page);

	if (chip->options & NAND_CACHEPRG)
		aml_chip->cached_prog_status = 1;
	else
		aml_chip->cached_prog_status = 0;
	if (unlikely(raw))
		chip->ecc.write_page_raw(mtd, chip, buf, 0, 0);
	else
		chip->ecc.write_page(mtd, chip, buf, 0, 0);

	if (!(chip->options & NAND_CACHEPRG)) {
		status = chip->waitfunc(mtd, chip);
		/*
		 * See if operation failed and additional status checks are
		 * available
		 */
		if (status & NAND_STATUS_FAIL) {
			printk("wr page=0x%x, status =  0x%x\n",
				page,status);
			return -EIO;
		}
	} else
		status = chip->waitfunc(mtd, chip);

	aml_chip->cached_prog_status = 0;
	return 0;
}

int aml_nand_read_oob(struct mtd_info *mtd, struct nand_chip *chip, int page)
{
	int32_t page_addr;
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	unsigned char *nand_buffer = aml_chip->aml_nand_data_buf;
	int32_t error=0;
	int oob_required = 1;

	page_addr = page;

	if (chip->cmdfunc == aml_nand_command)
		chip->cmdfunc(mtd, NAND_CMD_READOOB, 0, page_addr);
	else {
		aml_chip->aml_nand_select_chip(aml_chip, 0);
		chip->cmdfunc(mtd, NAND_CMD_READ0, 0, page_addr);
	}

	error = aml_nand_read_page_hwecc(mtd, chip, nand_buffer, oob_required, page);
	if (error) {
		printk("%s, %d,read oob failed\n", __func__, __LINE__);
	}
	return error;
}

int aml_nand_write_oob(struct mtd_info *mtd, struct nand_chip *chip, int page)
{
	printk("our host controller`s structure couldn`t support oob write\n");
	BUG();
	return 0;
}

int aml_nand_block_bad(struct mtd_info *mtd, loff_t ofs)
{
	struct nand_chip * chip = mtd->priv;
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	struct aml_nand_platform *plat = aml_chip->platform;
	struct mtd_oob_ops aml_oob_ops;
	int32_t ret=0, read_cnt, mtd_erase_shift, blk_addr, pages_per_blk;
	loff_t addr;

	if ((!strncmp((char*)plat->name,
		NAND_BOOT_NAME, strlen((const char*)NAND_BOOT_NAME))))
		return 0;

	mtd_erase_shift = fls(mtd->erasesize) - 1;
	/*
	 * This involves the management of BBT, when an exception occurs,
	 * the system should be stopped
	 */
	BUG_ON(mtd_erase_shift < 0);

	blk_addr = (int)(ofs >> mtd_erase_shift);
	if (aml_chip->block_status != NULL) {
		if (aml_chip->block_status[blk_addr] == NAND_BLOCK_BAD) {
			printk(" NAND bbt detect Bad block at %llx \n",
				(uint64_t)ofs);
			return EFAULT;
		}
		if (aml_chip->block_status[blk_addr] == NAND_FACTORY_BAD) {
			printk(" NAND bbt detect factory Bad block at %llx \n",
				(uint64_t)ofs);
			return FACTORY_BAD_BLOCK_ERROR;  //159  EFAULT
		} else if (aml_chip->block_status[blk_addr] ==NAND_BLOCK_GOOD)
			return 0;
	}
	chip->pagebuf = -1;
	pages_per_blk = (1 << (chip->phys_erase_shift - chip->page_shift));

		aml_oob_ops.mode = MTD_OPS_AUTO_OOB;
		aml_oob_ops.len = mtd->writesize;
		aml_oob_ops.ooblen = mtd->oobavail;
		aml_oob_ops.ooboffs = chip->ecc.layout->oobfree[0].offset;
		aml_oob_ops.datbuf = chip->buffers->databuf;
		aml_oob_ops.oobbuf = chip->oob_poi;
		for (read_cnt = 0; read_cnt < 2; read_cnt++) {
			addr =
			ofs + (pages_per_blk - 1) * read_cnt * mtd->writesize;
			ret = mtd->_read_oob(mtd, addr, &aml_oob_ops);
			if (ret == -EUCLEAN)
				ret = 0;
			if (ret < 0) {
				printk("1 NAND detect Bad block:%llx\n",
					(uint64_t)addr);
				return EFAULT;
			}
			if (aml_oob_ops.oobbuf[chip->badblockpos] == 0xFF)
				continue;
			if (aml_oob_ops.oobbuf[chip->badblockpos] == 0) {
				memset(aml_chip->aml_nand_data_buf,
					0, aml_oob_ops.ooblen);
				if (!memcmp(aml_chip->aml_nand_data_buf,
				aml_oob_ops.oobbuf, aml_oob_ops.ooblen)) {
					printk("2 NAND detect Bad block:%llx\n",
						(uint64_t)addr);
					return EFAULT;
				}
			}
		}


	return 0;
}

extern int meson_rsv_bbt_write(u_char *source, size_t size);
int aml_nand_block_markbad(struct mtd_info *mtd, loff_t ofs)
{
	struct nand_chip * chip = mtd->priv;
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	struct mtd_oob_ops aml_oob_ops;
	int blk_addr, mtd_erase_shift;
	int8_t *buf = NULL;

	mtd_erase_shift = fls(mtd->erasesize) - 1;
	/*
	 * This involves the management of BBT, when an exception occurs,
	 * the system should be stopped
	 */
	BUG_ON(mtd_erase_shift < 0);

	blk_addr = (int)(ofs >> mtd_erase_shift);
	if (aml_chip->block_status != NULL) {
		if ((aml_chip->block_status[blk_addr] == NAND_BLOCK_BAD)
		||(aml_chip->block_status[blk_addr] == NAND_FACTORY_BAD)) {
			goto mark_bad;

		} else if (aml_chip->block_status[blk_addr] ==NAND_BLOCK_GOOD) {
			aml_chip->block_status[blk_addr] = NAND_BLOCK_BAD;
			buf = aml_chip->block_status;
			meson_rsv_bbt_write((u_char *)buf, aml_chip->rsv->bbt->size);
		}
	}
mark_bad:
	/*no erase here, fixit*/
	aml_oob_ops.mode = MTD_OPS_AUTO_OOB;
	aml_oob_ops.len = mtd->writesize;
	aml_oob_ops.ooblen = mtd->oobavail;
	aml_oob_ops.ooboffs = chip->ecc.layout->oobfree[0].offset;
	aml_oob_ops.datbuf = chip->buffers->databuf;
	aml_oob_ops.oobbuf = chip->oob_poi;
	chip->pagebuf = -1;

	memset((unsigned char *)aml_oob_ops.datbuf, 0x0, mtd->writesize);
	memset((unsigned char *)aml_oob_ops.oobbuf, 0x0, aml_oob_ops.ooblen);

	return mtd->_write_oob(mtd, ofs, &aml_oob_ops);
}

static uint8_t aml_platform_read_byte(struct mtd_info *mtd)
{
	uint8_t status;

	NFC_SEND_CMD_DRD(controller, controller->chip_selected, 0);
	NFC_SEND_CMD_IDLE(controller, NAND_TWB_TIME_CYCLE);

	NFC_SEND_CMD_IDLE(controller, 0);
	NFC_SEND_CMD_IDLE(controller, 0);

	while (NFC_CMDFIFO_SIZE(controller) > 0) ;
	status = amlnf_read_reg32(controller->reg_base + P_NAND_BUF);
	return status;
}

void aml_platform_write_byte(struct aml_nand_chip *aml_chip, uint8_t data)
{
	NFC_SEND_CMD_IDLE(controller, NAND_TWB_TIME_CYCLE);
	NFC_SEND_CMD_DWR(controller, controller->chip_selected, data);
	NFC_SEND_CMD_IDLE(controller, NAND_TWB_TIME_CYCLE);

	NFC_SEND_CMD_IDLE(controller, 0);
	NFC_SEND_CMD_IDLE(controller, 0);

	while (NFC_CMDFIFO_SIZE(controller) > 0)
		;
}

extern struct aml_pre_scan *pre_scan;
int aml_nand_init(struct aml_nand_chip *aml_chip)
{
	struct aml_nand_platform *plat = aml_chip->platform;
	struct nand_chip *chip = &aml_chip->chip;
	struct mtd_info *mtd = &chip->mtd;
	int err = 0, i = 0;
	int oobmul;
	unsigned valid_chip_num = 0;
	struct nand_oobfree *oobfree = NULL;

	chip->ecc.layout = &aml_nand_oob_64;
	chip->select_chip = aml_nand_select_chip;
	chip->cmd_ctrl = aml_nand_cmd_ctrl;
	chip->read_byte = aml_platform_read_byte;

	controller->chip_num = plat->platform_nand_data.chip.nr_chips;
	if (controller->chip_num > MAX_CHIP_NUM) {
		err = -ENXIO;
		goto exit_error;
	}

	for (i=0; i<controller->chip_num; i++)
		aml_chip->valid_chip[i] = 1;

	/*use NO RB mode to detect nand chip num*/
	aml_chip->ops_mode |= AML_CHIP_NONE_RB;
	chip->chip_delay = 100;
	aml_chip->aml_nand_hw_init(aml_chip);
	aml_chip->toggle_mode =0;
#ifdef NAND_ECC_ONLY_BCH8_1K
	aml_chip->bch_info = NAND_ECC_BCH8_1K;
	printf("Currently only supports BCH8 1K!\n");
#else
	aml_chip->bch_info = NAND_ECC_BCH60_1K;
#endif
	chip->options = 0;
	chip->options |=  NAND_SKIP_BBTSCAN;
	chip->options |= NAND_NO_SUBPAGE_WRITE;

	err = aml_nand_scan(mtd, controller->chip_num);
	if (err || (pre_scan->pre_scan_flag)) {
		goto exit_error;
	}

	valid_chip_num = 0;
	for (i=0; i < controller->chip_num; i++) {
		if (aml_chip->valid_chip[i])
		    valid_chip_num++;
	}

	chip->scan_bbt = aml_nand_scan_shipped_bbt;//aml_nand_scan_bbt;
	if (aml_chip->aml_nand_adjust_timing)
		aml_chip->aml_nand_adjust_timing(aml_chip);

	if (aml_chip->aml_nand_options_confirm(aml_chip)) {
		err = -ENXIO;
		goto exit_error;
	}

		oobmul = mtd->oobsize /aml_chip->oob_size ;
		if (!chip->ecc.layout)
			chip->ecc.layout =
			kzalloc(sizeof(struct nand_ecclayout), GFP_KERNEL);
		if (!chip->ecc.layout) {
			err = -ENOMEM;
			goto exit_error ;
		}
		if (!strncmp((char*)plat->name, NAND_BOOT_NAME,
			strlen((const char*)NAND_BOOT_NAME)))
			chip->ecc.layout = &aml_nand_oob_uboot;
		else if (chip->ecc.mode != NAND_ECC_SOFT) {
			chip->ecc.layout = aml_ecclayout_get(aml_chip->oob_size);
			if (!chip->ecc.layout) {
				err = -ENXIO;
				goto exit_error;
			}
			chip->ecc.layout->oobfree[0].length *= oobmul;
			chip->ecc.layout->eccbytes *= oobmul;
			printk("%s :oobmul=%d,oobfree.length=%d,oob_size=%d\n",
				__func__,
				oobmul,
				chip->ecc.layout->oobfree[0].length,
				aml_chip->oob_size);
		}

	/*
	 * The number of bytes available for a client to place data into
	 * the out of band area
	 */
	chip->ecc.layout->oobavail = 0;
	oobfree = chip->ecc.layout->oobfree;
	for (i = 0; oobfree[i].length && i < ARRAY_SIZE(chip->ecc.layout->oobfree); i++)
		chip->ecc.layout->oobavail += oobfree[i].length;
	printk("oob avail size %d\n", chip->ecc.layout->oobavail);
	mtd->oobavail = chip->ecc.layout->oobavail;
	mtd->ecclayout = chip->ecc.layout;

	aml_chip->virtual_page_size = mtd->writesize;
	aml_chip->virtual_block_size = mtd->erasesize;

	aml_chip->aml_nand_data_buf =
		kzalloc((mtd->writesize + mtd->oobsize), GFP_KERNEL);
	if (aml_chip->aml_nand_data_buf == NULL) {
		printk("no memory for flash data buf\n");
		err = -ENOMEM;
		goto exit_error;
	}
	aml_chip->user_info_buf =
		kzalloc((mtd->writesize / chip->ecc.size) * PER_INFO_BYTE,
		GFP_KERNEL);
	if (aml_chip->user_info_buf == NULL) {
		printk("no memory for flash info buf\n");
		err = -ENOMEM;
		goto exit_error;
	}

	if (aml_chip->rsv == NULL) {
		aml_chip->rsv = kzalloc(sizeof (struct meson_rsv_handler_t), GFP_KERNEL);
			if (aml_chip->rsv == NULL) {
				printk("no memory for aml_chip rsv\n");
				err = -ENOMEM;
				goto exit_error;
			}
	}

	if (chip->buffers == NULL) {
		printk("no memory for flash data buf\n");
		err = -ENOMEM;
		goto exit_error;
	}

	chip->oob_poi = chip->buffers->databuf + mtd->writesize;
	chip->options |= NAND_OWN_BUFFERS;

	if (is_power_of_2(mtd->erasesize))
		mtd->erasesize_shift = ffs(mtd->erasesize) - 1;
	else
		mtd->erasesize_shift = 0;

	if (is_power_of_2(mtd->writesize))
		mtd->writesize_shift = ffs(mtd->writesize) - 1;
	else
		mtd->writesize_shift = 0;

	if (strncmp((char*)plat->name,
		NAND_BOOT_NAME, strlen((const char*)NAND_BOOT_NAME))) {
		meson_rsv_init(mtd, aml_chip->rsv);
		/*block status*/
		aml_chip->block_status =
			kzalloc((mtd->size >> mtd->erasesize_shift), GFP_KERNEL);
		if (aml_chip->block_status == NULL) {
			printk("no memory for flash block status\n");
			return -ENOMEM;
		}
		memset(aml_chip->block_status, 0, (mtd->size >> mtd->erasesize_shift));

		err = aml_nand_bbt_check(mtd);
		if (err) {
			printk("invalid nand bbt\n");
			goto exit_error;
		}
#ifndef CONFIG_ENV_IS_IN_NAND
		meson_rsv_check(aml_chip->rsv->env);
#endif
		meson_rsv_check(aml_chip->rsv->key);
		meson_rsv_check(aml_chip->rsv->dtb);
		meson_rsv_check(aml_chip->rsv->ddr_para);
	}

	if (aml_nand_add_partition(aml_chip) != 0) {
		err = -ENXIO;
		goto exit_error;
	}

	printk("%s initialized ok\n", mtd->name);
	return 0;

exit_error:
	if (aml_chip->user_info_buf) {
		kfree(aml_chip->user_info_buf);
		aml_chip->user_info_buf = NULL;
	}
	if (chip->buffers) {
		kfree(chip->buffers);
		chip->buffers = NULL;
	}
	if (aml_chip->aml_nand_data_buf) {
		kfree(aml_chip->aml_nand_data_buf);
		aml_chip->aml_nand_data_buf = NULL;
	}
	if (aml_chip->block_status) {
		kfree(aml_chip->block_status);
		aml_chip->block_status = NULL;
	}

	if (aml_chip->rsv) {
		kfree(aml_chip->rsv);
		aml_chip->rsv = NULL;
	}
	return err;
}

/**need fixed**/
int aml_nand_scan_bbt(struct mtd_info *mtd)
{
	return 0;
}

/* fixme, mxic's bad block identify is not checked yet! */
//only read bad block  labeled ops
int aml_nand_scan_shipped_bbt(struct mtd_info *mtd)
{
	struct nand_chip * chip = mtd->priv;
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	unsigned char *data_buf;
	int32_t read_cnt, page, pages_per_blk;
	loff_t addr, offset;
	int  start_blk =0, total_blk =0,i, j, bad_blk_cnt =0, phys_erase_shift;
	int realpage, col0_data=0, col0_oob=0, valid_page_num = 1;
	int col_data_sandisk[6], bad_sandisk_flag=0;

	memset(col_data_sandisk, 0, 6);
	phys_erase_shift = fls(mtd->erasesize) - 1;
	if (phys_erase_shift < 0) {
		pr_info("%s %d can not get erase shift\n",
				__func__, __LINE__);
		return -EINVAL;
	}
	chip->pagebuf = -1;
	pages_per_blk = (1 << (chip->phys_erase_shift - chip->page_shift));

	data_buf = kzalloc(mtd->writesize, GFP_KERNEL);
	if (data_buf == NULL) {
		printk("%s %d malloc failed\n",__func__,__LINE__);
		return -ENOMEM;
	}

	/*need scan factory bad block in bootloader area*/
	start_blk = 0;
	total_blk = (int)(mtd->size >> phys_erase_shift);
	/* fixme, need  check the total block number avoid mtd->size was changed outside! */
	printk("scaning flash total block %d\n", total_blk);
	do {
	offset = mtd->erasesize;
	offset *= start_blk;
	for (i=0; i < controller->chip_num; i++) {
		for (read_cnt = 0; read_cnt < 3; read_cnt++) {
			if (read_cnt == 2) {
				if (aml_chip->mfr_type == NAND_MFR_AMD)
					addr = offset + mtd->writesize;
				else
					break;
		    } else {
				if ((aml_chip->mfr_type == NAND_MFR_SANDISK) ||
					(aml_chip->mfr_type == NAND_ID_ESMT) ||
					(aml_chip->mfr_type == NAND_MFR_MACRONIX) ||
					aml_get_samsung_fbbt_flag()) {
					addr = offset + read_cnt*mtd->writesize;
				} else
					addr = offset +
				(pages_per_blk - 1) * read_cnt * mtd->writesize;
			}

			realpage = (int)(addr >> chip->page_shift);
			page = realpage & chip->pagemask;

			if (page != -1) {
				valid_page_num=mtd->writesize>>chip->page_shift;
				valid_page_num /= aml_chip->plane_num;

				aml_chip->page_addr = page/ valid_page_num;
	if (unlikely(aml_chip->page_addr >= aml_chip->internal_page_nums)) {
		aml_chip->page_addr -= aml_chip->internal_page_nums;
		aml_chip->page_addr |=
		(1 << aml_chip->internal_chip_shift)*aml_chip->internal_chipnr;
	}
			}
			if (aml_chip->plane_num == 2) {
				aml_chip->aml_nand_wait_devready(aml_chip, i);
				if (aml_nand_get_fbb_issue()) {
					chip->cmd_ctrl(mtd,
						NAND_CMD_SEQIN, NAND_CTRL_CLE);
					chip->cmd_ctrl(mtd,
						0, NAND_CTRL_ALE);
				}
				aml_chip->aml_nand_command(aml_chip,
					NAND_CMD_READ0,
					0x00,aml_chip->page_addr, i);

			if (!aml_chip->aml_nand_wait_devready(aml_chip, i))
				printk ("%s, %d,selected chip%d not ready\n",
					__func__, __LINE__, i);

				if (aml_chip->ops_mode & AML_CHIP_NONE_RB)
					chip->cmd_ctrl(mtd,
					NAND_CMD_READ0 & 0xff,
					NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
				udelay(2);
				aml_chip->aml_nand_command(aml_chip,
					NAND_CMD_TWOPLANE_READ1,
					0x00, aml_chip->page_addr, i);
				udelay(2);

		if (aml_chip->mfr_type  == NAND_MFR_SANDISK) {
			for (j = 0; j < 6; j++)
				col_data_sandisk[j] = chip->read_byte(mtd);
		} else
			col0_data = chip->read_byte(mtd);

				aml_chip->aml_nand_command(aml_chip,
					NAND_CMD_TWOPLANE_READ2,
					aml_chip->page_size,
					aml_chip->page_addr, i);
				//aml_chip->aml_nand_wait_devready(aml_chip, i);
				udelay(2);

				if (aml_chip->mfr_type  == NAND_MFR_SANDISK)
					col0_oob = 0x0;
				else
					col0_oob = chip->read_byte(mtd);
				//printk("col0_oob=%x\n",col0_oob);

			} else if (aml_chip->plane_num == 1) {

			if (aml_nand_get_fbb_issue()) {
				chip->cmd_ctrl(mtd,
					NAND_CMD_SEQIN, NAND_CTRL_CLE);
				chip->cmd_ctrl(mtd,
					0, NAND_CTRL_ALE);
				}
				aml_chip->aml_nand_command(aml_chip,
					NAND_CMD_READ0, 0x00,
					aml_chip->page_addr , i);
				udelay(2);

			if (!aml_chip->aml_nand_wait_devready(aml_chip, i))
				printk ("%s, %d,selected chip%d not ready\n",
					__func__, __LINE__, i);

				if (aml_chip->ops_mode & AML_CHIP_NONE_RB)
					chip->cmd_ctrl(mtd,
					NAND_CMD_READ0 & 0xff,
					NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
				udelay(2);

			if (aml_chip->mfr_type  == NAND_MFR_SANDISK) {
				for (j = 0; j < 6; j++)
					col_data_sandisk[j] =
						chip->read_byte(mtd);
			} else
				col0_data = chip->read_byte(mtd);

				//printk("col0_data =%x\n",col0_data);

			if (aml_chip->mfr_type  != NAND_MFR_SANDISK)
				aml_chip->aml_nand_command(aml_chip,
					NAND_CMD_RNDOUT,
					aml_chip->page_size, -1, i);
			udelay(2);

			if (aml_chip->mfr_type  == NAND_MFR_SANDISK)
				col0_oob = 0x0;
			else
				col0_oob = chip->read_byte(mtd);
				//printk("col0_oob =%x\n",col0_oob);
			}

	if ((col0_oob == 0xFF))
		continue;

	if (col0_oob != 0xFF) {
		printk("%s:%d factory ship bbt found\n", __func__, __LINE__);

		if (aml_chip->mfr_type  == NAND_MFR_DOSILICON ||
		    aml_chip->mfr_type  == NAND_MFR_ATO ||
		    aml_chip->mfr_type  == NAND_MFR_HYNIX ||
		    aml_chip->mfr_type  == NAND_ID_WINBOND ||
		    aml_chip->mfr_type == NAND_ID_ESMT ||
		    aml_chip->mfr_type == NAND_MFR_MACRONIX ||
		    aml_chip->mfr_type  == NAND_MFR_AMD ||
		    aml_get_samsung_fbbt_flag()) {
			printk("col0_data =%x col0_oob =%x\n",col0_data,col0_oob);
			printk("detect a fbb:%llx blk=%d chip=%d\n",
				(uint64_t)addr, start_blk, i);
			bad_blk_cnt++;
			aml_chip->block_status[start_blk] =
				NAND_FACTORY_BAD;
			break;
		}

		if ((aml_chip->mfr_type  == NAND_MFR_SANDISK) ) {
			for (j = 0; j < 6; j++) {
				if (col_data_sandisk[j] == 0x0) {
					bad_sandisk_flag = 1;
					break;
				}
			}
			if (bad_sandisk_flag ) {
				printk("detect factory Bad block:%llx blk=%d chip=%d\n",
					(uint64_t)addr, start_blk, i);
				bad_blk_cnt++;
				aml_chip->block_status[start_blk] = NAND_FACTORY_BAD;
				bad_sandisk_flag=0;
				break;
			}
		}

		if ((aml_chip->mfr_type  == NAND_MFR_SAMSUNG ) ) {
			if ((col0_oob != 0xFF) && (col0_data != 0xFF)) {
				printk("detect factory Bad block:%llx blk=%d chip=%d\n",
					(uint64_t)addr, start_blk, i);
				bad_blk_cnt++;
				aml_chip->block_status[start_blk] = NAND_FACTORY_BAD;
					break;
			}
		}

		if ((aml_chip->mfr_type  == NAND_MFR_TOSHIBA )  ) {
			if ((col0_oob != 0xFF) && (col0_data != 0xFF)) {
				printk("detect factory Bad block:%llx blk=%d chip=%d\n",
					(uint64_t)addr, start_blk, i);
				bad_blk_cnt++;
				aml_chip->block_status[start_blk] = NAND_FACTORY_BAD;
				break;
			}
		}

		if (aml_chip->mfr_type  == NAND_MFR_MICRON ) {
			if (col0_oob == 0x0) {
				printk("detect factory Bad block:%llx blk=%d chip=%d\n",
					(uint64_t)addr, start_blk, i);
				bad_blk_cnt++;
				aml_chip->block_status[start_blk] = NAND_FACTORY_BAD;
				break;
			}
		}
	}
		}
	}
	} while((++start_blk) < total_blk);

	printk("aml_nand_scan_bbt: factory Bad block bad_blk_cnt=%d\n",
		bad_blk_cnt);
	kfree(data_buf);
	return 0;
}

int aml_nand_bbt_check(struct mtd_info *mtd)
{
	struct aml_nand_chip *aml_chip = mtd_to_nand_chip(mtd);
	struct aml_nand_chip *aml_chip_boot = mtd_to_nand_chip(nand_info[0]);
	int ret =0;
	int8_t *buf = NULL;

	ret = meson_rsv_scan(aml_chip->rsv->bbt);
	if ((ret !=0) && ((ret != (-1)))) {
		printk("%s %d\n", __func__, __LINE__);
		goto exit_error;
	}

	ret = 0;
	buf = aml_chip->block_status;
	if (aml_chip->rsv->bbt->valid == 1) {
		/*read bbt*/
		printk("%s %d bbt is valid, reading.\n", __func__, __LINE__);
		meson_rsv_read(aml_chip->rsv->bbt, (u_char *)buf);
	} else {
		printk("%s %d bbt is invalid, scanning.\n", __func__, __LINE__);
		/*no bbt haven't been found, abnormal or clean nand! rebuild*/
		aml_nand_scan_shipped_bbt(mtd);
		meson_rsv_bbt_write((u_char *)buf, aml_chip->rsv->bbt->size);
	}

	/*make uboot bbt perspective the same with normal bbt*/
	aml_chip_boot->block_status = aml_chip->block_status;
exit_error:
	return ret;
}

/**read partiton from dtb**/
extern int get_partition_from_dts(unsigned char * buffer);
int amlnf_dtb_init_partitions(struct aml_nand_chip *aml_chip)
{
	int ret = 0;
	u8 *dtb_buf = NULL;

	dtb_buf = kzalloc(aml_chip->dtbsize, GFP_KERNEL);
	if (dtb_buf == NULL) {
		printk("nand malloc for dtb_buf failed\n");
		ret = -1;
		goto exit_err;
	}
	memset(dtb_buf, 0x0, aml_chip->dtbsize);

	/*parse partitions table */
	ret = get_partition_from_dts(dtb_buf);
	if (ret) {
		printk("%s  get_partition_from_dts failed\n", __func__);
	}
exit_err:
	if (dtb_buf) {
		kfree(dtb_buf);
		dtb_buf = NULL;
	}
	return ret;
}


