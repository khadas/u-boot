// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <dm.h>
#include <nand.h>
#include <asm/io.h>
#include <malloc.h>
#include <linux/err.h>
#include <asm/cache.h>
//#include <asm/arch/secure_apb.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/rawnand.h>
#include <linux/mtd/nand_ecc.h>
#include <dm/device-internal.h>
#include <dm/uclass-internal.h>
#include <dm/root.h>
#include <dm/lists.h>
#include <dm/util.h>

#include <fdtdec.h>
#include <memalign.h>
#include <linux/io.h>
#include <asm/gpio.h>
#include <asm/arch/clock.h>
#include <clk.h>
#include <dm/pinctrl.h>
#include <asm/arch/register.h>

#include "aml_nand.h"
#include "version.h"
#include <time.h>

struct hw_controller *controller = NULL;

extern struct mtd_info *nand_info[CONFIG_SYS_MAX_NAND_DEVICE];
extern void mtd_store_set(struct mtd_info *mtd, int dev);
extern struct aml_pre_scan *pre_scan;;

#ifdef CONFIG_MTD_DEVICE
static __attribute__((unused)) char dev_name[CONFIG_SYS_MAX_NAND_DEVICE][8];
#endif

static struct aml_nand_platform aml_nand_mid_platform[] = {
	{
		.name = NAND_BOOT_NAME,
		.chip_enable_pad = AML_NAND_CE0,
		.ready_busy_pad = AML_NAND_CE0,
		.platform_nand_data = {
		    .chip =  {
			.nr_chips = 1,
			.options = (NAND_TIMING_MODE5 | NAND_ECC_BCH60_1K_MODE),
		    },
		},
		.rbpin_mode=1,
		.short_pgsz=384,
		.ran_mode=0,
		.T_REA = 20,
		.T_RHOH = 15,
	},
	{
		.name = NAND_NORMAL_NAME,
		.chip_enable_pad = (AML_NAND_CE0) | (AML_NAND_CE1 << 4),
		.ready_busy_pad = (AML_NAND_CE0) | (AML_NAND_CE1 << 4),
		.platform_nand_data = {
		    .chip =  {
			.nr_chips = 1,
			/* moved to board config as different board
			 * may have different partition table
			 */
			/* .nr_partitions = ARRAY_SIZE(normal_partition_info),
			.partitions = normal_partition_info,*/
			.options = (NAND_TIMING_MODE5
				| NAND_ECC_BCH60_1K_MODE
				| NAND_TWO_PLANE_MODE),
		    },
		},
		.rbpin_mode = 1,
		.short_pgsz = 0,
		.ran_mode = 0,
		.T_REA = 20,
		.T_RHOH = 15,
	}
};

struct aml_nand_device aml_nand_mid_device = {
    .aml_nand_platform = aml_nand_mid_platform,
    .dev_num = ARRAY_SIZE(aml_nand_mid_platform),
};

#define ECC_INFORMATION(name_a, bch_a, size_a, parity_a, user_a) \
	{\
		.name=name_a,\
		.bch_mode=bch_a,\
		.bch_unit_size=size_a,\
		.bch_bytes=parity_a,\
		.user_byte_mode=user_a\
	}

static struct aml_nand_bch_desc m3_bch_list[] = {
	[0] = ECC_INFORMATION("NAND_RAW_MODE",
		NAND_ECC_SOFT_MODE,
		0,
		0,
		0),
	[1] = ECC_INFORMATION("NAND_BCH8_MODE",
		NAND_ECC_BCH8_MODE,
		NAND_ECC_UNIT_SIZE,
		NAND_BCH8_ECC_SIZE,
		2),
	[2] = ECC_INFORMATION("NAND_BCH8_1K_MODE" ,
		NAND_ECC_BCH8_1K_MODE,
		NAND_ECC_UNIT_1KSIZE,
		NAND_BCH8_1K_ECC_SIZE,
		2),
	[3] = ECC_INFORMATION("NAND_BCH24_1K_MODE" ,
		NAND_ECC_BCH24_1K_MODE,
		NAND_ECC_UNIT_1KSIZE,
		NAND_BCH24_1K_ECC_SIZE,
		2),
	[4] = ECC_INFORMATION("NAND_BCH30_1K_MODE" ,
		NAND_ECC_BCH30_1K_MODE,
		NAND_ECC_UNIT_1KSIZE,
		NAND_BCH30_1K_ECC_SIZE,
		2),
	[5] = ECC_INFORMATION("NAND_BCH40_1K_MODE" ,
		NAND_ECC_BCH40_1K_MODE,
		NAND_ECC_UNIT_1KSIZE,
		NAND_BCH40_1K_ECC_SIZE,
		2),
	[6] = ECC_INFORMATION("NAND_BCH50_1K_MODE" ,
		NAND_ECC_BCH50_1K_MODE,
		NAND_ECC_UNIT_1KSIZE,
		NAND_BCH50_1K_ECC_SIZE,
		2),
	[7] = ECC_INFORMATION("NAND_BCH60_1K_MODE" ,
		NAND_ECC_BCH60_1K_MODE,
		NAND_ECC_UNIT_1KSIZE,
		NAND_BCH60_1K_ECC_SIZE,
		2),
	[8] = ECC_INFORMATION("NAND_SHORT_MODE" ,
		NAND_ECC_SHORT_MODE,
		NAND_ECC_UNIT_SHORT,
		NAND_BCH60_1K_ECC_SIZE,
		2),
};

extern uint8_t nand_boot_flag;

unsigned char pagelist_hynix256[128] = {
	0x00, 0x01, 0x02, 0x03, 0x06, 0x07, 0x0A, 0x0B,
	0x0E, 0x0F, 0x12, 0x13, 0x16, 0x17, 0x1A, 0x1B,
	0x1E, 0x1F, 0x22, 0x23, 0x26, 0x27, 0x2A, 0x2B,
	0x2E, 0x2F, 0x32, 0x33, 0x36, 0x37, 0x3A, 0x3B,

	0x3E, 0x3F, 0x42, 0x43, 0x46, 0x47, 0x4A, 0x4B,
	0x4E, 0x4F, 0x52, 0x53, 0x56, 0x57, 0x5A, 0x5B,
	0x5E, 0x5F, 0x62, 0x63, 0x66, 0x67, 0x6A, 0x6B,
	0x6E, 0x6F, 0x72, 0x73, 0x76, 0x77, 0x7A, 0x7B,

	0x7E, 0x7F, 0x82, 0x83, 0x86, 0x87, 0x8A, 0x8B,
	0x8E, 0x8F, 0x92, 0x93, 0x96, 0x97, 0x9A, 0x9B,
	0x9E, 0x9F, 0xA2, 0xA3, 0xA6, 0xA7, 0xAA, 0xAB,
	0xAE, 0xAF, 0xB2, 0xB3, 0xB6, 0xB7, 0xBA, 0xBB,

	0xBE, 0xBF, 0xC2, 0xC3, 0xC6, 0xC7, 0xCA, 0xCB,
	0xCE, 0xCF, 0xD2, 0xD3, 0xD6, 0xD7, 0xDA, 0xDB,
	0xDE, 0xDF, 0xE2, 0xE3, 0xE6, 0xE7, 0xEA, 0xEB,
	0xEE, 0xEF, 0xF2, 0xF3, 0xF6, 0xF7, 0xFA, 0xFB,
};
unsigned char pagelist_1ynm_hynix256_mtd[128] = {
	0x00, 0x01, 0x03, 0x05, 0x07, 0x09, 0x0b, 0x0d,
	0x0f, 0x11, 0x13, 0x15, 0x17, 0x19, 0x1b, 0x1d,
	0x1f, 0x21, 0x23, 0x25, 0x27, 0x29, 0x2b, 0x2d,
	0x2f, 0x31, 0x33, 0x35, 0x37, 0x39, 0x3b, 0x3d,
	0x3f, 0x41, 0x43, 0x45, 0x47, 0x49, 0x4b, 0x4d,
	0x4f, 0x51, 0x53, 0x55, 0x57, 0x59, 0x5b, 0x5d,
	0x5f, 0x61, 0x63, 0x65, 0x67, 0x69, 0x6b, 0x6d,
	0x6f, 0x71, 0x73, 0x75, 0x77, 0x79, 0x7b, 0x7d,
	0x7f, 0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d,
	0x8f, 0x91, 0x93, 0x95, 0x97, 0x99, 0x9b, 0x9d,
	0x9f, 0xa1, 0xA3, 0xA5, 0xA7, 0xA9, 0xAb, 0xAd,
	0xAf, 0xb1, 0xB3, 0xB5, 0xB7, 0xB9, 0xBb, 0xBd,
	0xBf, 0xc1, 0xC3, 0xC5, 0xC7, 0xC9, 0xCb, 0xCd,
	0xCf, 0xd1, 0xD3, 0xD5, 0xD7, 0xD9, 0xDb, 0xDd,
	0xDf, 0xe1, 0xE3, 0xE5, 0xE7, 0xE9, 0xEb, 0xEd,
	0xEf, 0xf1, 0xF3, 0xF5, 0xF7, 0xF9, 0xFb, 0xFd,
};

static int controller_select_chip(struct hw_controller *controller,
	u8 chipnr)
{
	int ret = 0;

	switch (chipnr) {
	case 0:
	case 1:
	case 2:
	case 3:
		controller->chip_selected = controller->ce_enable[chipnr];
		controller->rb_received = controller->rb_enable[chipnr];

		NFC_SEND_CMD_IDLE(controller, 0);
		break;
	default:
		BUG();
		ret = -12;
		break;
	}
	return ret;
}

static void m3_nand_select_chip(struct aml_nand_chip *aml_chip, int chipnr)
{
	controller_select_chip(controller, chipnr);
	return;
}

void aml_nfc_get_clk_name(struct hw_controller *controller)
{
	struct udevice *clk_udevice, *dev = controller->device;

	uclass_get_device_by_name(UCLASS_CLK, "amlogic,g12a-clkc", &clk_udevice);

        clk_get_by_name(dev, "fdiv2", &controller->fdiv2);
        clk_get_by_name(dev, "xtal", &controller->xtal);
        clk_get_by_name(dev, "mux", &controller->mux);
        clk_get_by_name(dev, "div", &controller->div);
        clk_get_by_name(dev, "gate", &controller->gate);

	clk_enable(&controller->gate);
}

void get_sys_clk_rate_mtd(struct hw_controller *controller, int *rate)
{
	int bus_cycle, bus_timing;
	unsigned int clk, clk_div, clk_src;
	int clk_freq = *rate * 1000000;
	unsigned int always_on = 0x1 << 28;

	if (clk_freq <=  24) {
		clk = 24000000;
		clk_src = 0;
		clk_enable(&controller->xtal); // for c1 clk architecture compatible. xtal was not controlled by clk driver.
		clk_set_rate(&controller->div, clk);
		clk_div = 1;
		bus_cycle = 4;
		bus_timing = 3;
		goto __NAND_CLK_CFG__;
	} else {
		clk = 1000000000;
		clk_src = 1;
		clk_disable(&controller->xtal);
		clk_set_parent(&controller->mux, &controller->fdiv2);
		clk_set_rate(&controller->div, clk);
		bus_cycle  = 6;
		bus_timing = bus_cycle + 1;
	}
	clk_div = clk /clk_freq;
	if (clk % clk_freq)
		clk_div++;
__NAND_CLK_CFG__:
	AMLNF_WRITE_REG(controller->nand_clk_reg,
		      (clk_src << 6) | clk_div | always_on | (1 << 31) | (2 << 8));
	printf("%s:gclock=%x\n", __func__,
	       AMLNF_READ_REG(controller->nand_clk_reg));
	NFC_SET_CFG(controller, 0);
	NFC_SET_TIMING_ASYC(controller, bus_timing, (bus_cycle - 1));
	NFC_SEND_CMD(controller, 1<<31);
	printf("%s:P_NAND_CFG=%x\n", __func__,
	       AMLNF_READ_REG(controller->reg_base + P_NAND_CFG));
}

static void m3_nand_hw_init(struct aml_nand_chip *aml_chip)
{
	int sys_clk_rate = 200;

	get_sys_clk_rate_mtd(controller, &sys_clk_rate);

	return;
}

static void m3_nand_adjust_timing(struct aml_nand_chip *aml_chip)
{
	int sys_clk_rate;

	if (!aml_chip->T_REA)
		aml_chip->T_REA = 20;
	if (!aml_chip->T_RHOH)
		aml_chip->T_RHOH = 15;

	if (aml_chip->T_REA > 30)
		sys_clk_rate = 112;
	else if (aml_chip->T_REA > 16)
		sys_clk_rate = 200;
	else
		sys_clk_rate = 250;

	get_sys_clk_rate_mtd(controller, &sys_clk_rate);
}

static int m3_nand_options_confirm(struct aml_nand_chip *aml_chip)
{
	struct nand_chip *chip = &aml_chip->chip;
	struct mtd_info *mtd = &chip->mtd;
	struct aml_nand_platform *plat = aml_chip->platform;
	struct aml_nand_bch_desc *ecc_supports = aml_chip->bch_desc;
	unsigned int max_bch_mode = aml_chip->max_bch_mode;
	unsigned int options_selected = 0, options_support = 0, options_define;
	unsigned int eep_need_oobsize = 0, ecc_page_num = 0, ecc_bytes;
	int error = 0, i, valid_chip_num = 0;

	if (!strncmp((char*)plat->name,
		NAND_BOOT_NAME,
		strlen((const char*)NAND_BOOT_NAME))) {
		eep_need_oobsize =
		ecc_supports[8].bch_bytes + ecc_supports[8].user_byte_mode;
		ecc_page_num =
		aml_chip->page_size / ecc_supports[8].bch_unit_size;
		aml_chip->boot_oob_fill_cnt = aml_chip->oob_size -
			eep_need_oobsize * ecc_page_num;
	}

	/*select fit ecc mode by flash oob size */
	for (i = max_bch_mode - 1; i > 0; i--) {
		eep_need_oobsize =
		ecc_supports[i].bch_bytes + ecc_supports[i].user_byte_mode;
		ecc_page_num =
			aml_chip->page_size / ecc_supports[i].bch_unit_size;
		ecc_bytes = aml_chip->oob_size / ecc_page_num;
		if (ecc_bytes >= eep_need_oobsize) {
			options_support = ecc_supports[i].bch_mode;
			break;
		}
	}
	/**aml_chip->oob_size equal nand oobsize**/
	aml_chip->oob_fill_cnt =
		aml_chip->oob_size - eep_need_oobsize * ecc_page_num;
	printk("oob_fill_cnt =%d oob_size =%d, bch_bytes =%d\n",
		aml_chip->oob_fill_cnt,
		aml_chip->oob_size,
		ecc_supports[i].bch_bytes);
	printk("ecc mode:%d ecc_page_num=%d eep_need_oobsize=%d\n",
		options_support, ecc_page_num, eep_need_oobsize);

	printk("options_support :%d\n", options_support);
	if (options_support != NAND_ECC_SOFT_MODE) {
		chip->ecc.read_page_raw = aml_nand_read_page_raw;
		chip->ecc.write_page_raw = aml_nand_write_page_raw;
		chip->ecc.read_page = aml_nand_read_page_hwecc;
		chip->ecc.write_page = aml_nand_write_page_hwecc;
		chip->ecc.read_oob  = aml_nand_read_oob;
		chip->ecc.write_oob = aml_nand_write_oob;
		chip->block_bad = aml_nand_block_bad;
		chip->block_markbad = aml_nand_block_markbad;
		chip->ecc.mode = NAND_ECC_HW;
	} else {
		chip->ecc.read_page_raw = aml_nand_read_page_raw;
		chip->ecc.write_page_raw = aml_nand_write_page_raw;
		chip->ecc.mode = NAND_ECC_SOFT;
	}
	chip->write_buf = aml_nand_dma_write_buf;
	chip->read_buf = aml_nand_dma_read_buf;

#ifdef NAND_ECC_ONLY_BCH8_1K
	options_support = NAND_ECC_BCH8_MODE;
	printf("Currently only supports BCH8 1K!\n");
	chip->ecc.strength = 8;
	chip->ecc.size = NAND_ECC_UNIT_SIZE;
	chip->ecc.bytes = NAND_BCH8_ECC_SIZE;
	aml_chip->bch_mode = NAND_ECC_BCH8;
	aml_chip->user_byte_mode = 2;
	aml_chip->ecc_cnt_limit = 6;
	aml_chip->ecc_max = 8;
	chip->ecc.steps = mtd->writesize / chip->ecc.size;

#else
	if (mtd->writesize <= 2048)
		options_support = NAND_ECC_BCH8_MODE;

	switch (options_support) {

		case NAND_ECC_BCH8_MODE:
			chip->ecc.strength = 8;
			chip->ecc.size = NAND_ECC_UNIT_SIZE;
			chip->ecc.bytes = NAND_BCH8_ECC_SIZE;
			aml_chip->bch_mode = NAND_ECC_BCH8;
			aml_chip->user_byte_mode = 2;
			aml_chip->ecc_cnt_limit = 6;
			aml_chip->ecc_max = 8;
			chip->ecc.steps = mtd->writesize / chip->ecc.size;
			break;
		case NAND_ECC_BCH8_1K_MODE:
			chip->ecc.strength = 8;
			chip->ecc.size = NAND_ECC_UNIT_1KSIZE;
			chip->ecc.bytes = NAND_BCH8_1K_ECC_SIZE;
			aml_chip->bch_mode = NAND_ECC_BCH8_1K;
			aml_chip->user_byte_mode = 2;
			aml_chip->ecc_cnt_limit = 6;
			aml_chip->ecc_max = 8;
			chip->ecc.steps = mtd->writesize / chip->ecc.size;
			break;

		case NAND_ECC_BCH24_1K_MODE:
			chip->ecc.strength = 24;
			chip->ecc.size = NAND_ECC_UNIT_1KSIZE;
			chip->ecc.bytes = NAND_BCH24_1K_ECC_SIZE;
			aml_chip->bch_mode = NAND_ECC_BCH24_1K;
			aml_chip->user_byte_mode = 2;
			aml_chip->ecc_cnt_limit = 22;
			aml_chip->ecc_max = 24;
			chip->ecc.steps = mtd->writesize / chip->ecc.size;
			break;

		case NAND_ECC_BCH30_1K_MODE:
			chip->ecc.strength = 30;
			chip->ecc.size = NAND_ECC_UNIT_1KSIZE;
			chip->ecc.bytes = NAND_BCH30_1K_ECC_SIZE;
			aml_chip->bch_mode = NAND_ECC_BCH30_1K;
			aml_chip->user_byte_mode = 2;
			aml_chip->ecc_cnt_limit = 26;
			aml_chip->ecc_max = 30;
			chip->ecc.steps = mtd->writesize / chip->ecc.size;
			break;

		case NAND_ECC_BCH40_1K_MODE:
			chip->ecc.strength = 40;
			chip->ecc.size = NAND_ECC_UNIT_1KSIZE;
			chip->ecc.bytes = NAND_BCH40_1K_ECC_SIZE;
			aml_chip->bch_mode = NAND_ECC_BCH40_1K;
			aml_chip->user_byte_mode = 2;
			aml_chip->ecc_cnt_limit = 34;
			aml_chip->ecc_max = 40;
			chip->ecc.steps = mtd->writesize / chip->ecc.size;
			break;

		case NAND_ECC_BCH50_1K_MODE:
			chip->ecc.strength = 40;
			chip->ecc.size = NAND_ECC_UNIT_1KSIZE;
			chip->ecc.bytes = NAND_BCH50_1K_ECC_SIZE;
			aml_chip->bch_mode = NAND_ECC_BCH50_1K;
			aml_chip->user_byte_mode = 2;
			aml_chip->ecc_cnt_limit = 45;
			aml_chip->ecc_max = 50;
			chip->ecc.steps = mtd->writesize / chip->ecc.size;
			break;

		case NAND_ECC_BCH60_1K_MODE:
			chip->ecc.strength = 60;
			chip->ecc.size = NAND_ECC_UNIT_1KSIZE;
			chip->ecc.bytes = NAND_BCH60_1K_ECC_SIZE;
			aml_chip->bch_mode = NAND_ECC_BCH60_1K;
			aml_chip->user_byte_mode = 2;
			aml_chip->ecc_cnt_limit = 55;
			aml_chip->ecc_max = 60;
			chip->ecc.steps = mtd->writesize / chip->ecc.size;
			break;

		case NAND_ECC_SHORT_MODE:
			chip->ecc.strength = 60;
			chip->ecc.size = NAND_ECC_UNIT_SHORT;
			chip->ecc.bytes = NAND_BCH60_1K_ECC_SIZE;
			aml_chip->bch_mode = NAND_ECC_BCH_SHORT;
			aml_chip->user_byte_mode = 2;
			aml_chip->ecc_cnt_limit = 55;
			aml_chip->ecc_max = 60;
			chip->ecc.steps = mtd->writesize / 512;
			break;
		/*not support for amlogic chip*/
		case NAND_ECC_SOFT_MODE:
			aml_chip->user_byte_mode = 1;
			aml_chip->bch_mode = 0;
			/*don't care*/
			aml_chip->ecc_cnt_limit = 9;
			aml_chip->ecc_max = 16;
			break;
		default :
			printk("unknow ecc mode, error!");
			error = -ENXIO;
			break;
	}
#endif
	options_selected =
	plat->platform_nand_data.chip.options & NAND_INTERLEAVING_OPTIONS_MASK;
	options_define = (aml_chip->options & NAND_INTERLEAVING_OPTIONS_MASK);
	if (options_selected > options_define) {
		printk("INTERLEAVE change! \n");
		options_selected = options_define;
	}
	switch (options_selected) {
		case NAND_INTERLEAVING_MODE:
			aml_chip->ops_mode |= AML_INTERLEAVING_MODE;
			mtd->erasesize *= aml_chip->internal_chipnr;
			mtd->writesize *= aml_chip->internal_chipnr;
			mtd->oobsize *= aml_chip->internal_chipnr;
			break;
		default:
			break;
	}

	options_selected =
		plat->platform_nand_data.chip.options & NAND_PLANE_OPTIONS_MASK;
	options_define = (aml_chip->options & NAND_PLANE_OPTIONS_MASK);
	printf("options_selected: 0x%x, options_define:0x%x\n",options_selected,
		options_define);
	if (options_selected > options_define) {
		printk("PLANE change!\n");
		options_selected = options_define;
	}

	valid_chip_num = 0;
	for (i=0; i < controller->chip_num; i++)
		if (aml_chip->valid_chip[i])
		    valid_chip_num++;
	if (aml_chip->ops_mode & AML_INTERLEAVING_MODE)
		valid_chip_num *= aml_chip->internal_chipnr;

	if (valid_chip_num > 2) {
		aml_chip->plane_num = 1;
			printk("detect valid_chip_num over 2\n");
	} else {
		switch (options_selected) {
			case NAND_TWO_PLANE_MODE:
				aml_chip->plane_num = 2;
				mtd->erasesize *= 2;
				mtd->writesize *= 2;
				mtd->oobsize *= 2;
				printk("two plane!@\n");
				break;
			default:
				aml_chip->plane_num = 1;
				break;
		}
	}

	printk("plane_num=%d writesize=0x%x ecc.size=0x%0x bch_mode=%d\n",
		aml_chip->plane_num,
		mtd->writesize,
		chip->ecc.size,
		aml_chip->bch_mode);

	return error;
}


static int aml_platform_dma_waiting(struct aml_nand_chip *aml_chip)
{
	unsigned time_out_cnt = 0;

	NFC_SEND_CMD_IDLE(controller, 0);
	NFC_SEND_CMD_IDLE(controller, 0);
	do {
		if (NFC_CMDFIFO_SIZE(controller) <= 0)
			break;
	}while (time_out_cnt++ <= AML_DMA_BUSY_TIMEOUT);

	if (time_out_cnt < AML_DMA_BUSY_TIMEOUT)
		return 0;

	return -EBUSY;
}

static int m3_nand_dma_write(struct aml_nand_chip *aml_chip,
	unsigned char *buf, int len, unsigned bch_mode)
{
	int ret = 0;
	unsigned dma_unit_size = 0, count = 0;
	struct nand_chip *chip = &aml_chip->chip;
	struct mtd_info *mtd = &chip->mtd;
	uint32_t temp;

	if (bch_mode == NAND_ECC_NONE)
		count = 1;
	else if (bch_mode == NAND_ECC_BCH_SHORT) {
		dma_unit_size = (chip->ecc.size >> 3);
		/*caculate ecc pages cnt*/
		count = len/chip->ecc.size;
	}
	else
		count = len/chip->ecc.size;

	flush_dcache_range((unsigned long)buf, (unsigned long)buf + len);
	flush_dcache_range((unsigned long)aml_chip->user_info_buf,
		(unsigned long)aml_chip->user_info_buf + count*PER_INFO_BYTE);

	NFC_SEND_CMD_ADL(controller, (u32)(unsigned long)buf);
	NFC_SEND_CMD_ADH(controller, (u32)(unsigned long)buf);
	NFC_SEND_CMD_AIL(controller, (u32)(unsigned long)aml_chip->user_info_buf);
	NFC_SEND_CMD_AIH(controller, (u32)(unsigned long)aml_chip->user_info_buf);

	if (aml_chip->ran_mode) {
		temp = mtd->writesize>>chip->page_shift;
		if (aml_chip->plane_num == 2)
			NFC_SEND_CMD_SEED(controller,
				(aml_chip->page_addr / temp) * temp);
		else
			NFC_SEND_CMD_SEED(controller, aml_chip->page_addr);
	}
	if (!bch_mode)
		NFC_SEND_CMD_M2N_RAW(controller, 0, len);
	else
		NFC_SEND_CMD_M2N(controller, aml_chip->ran_mode,
		((bch_mode == NAND_ECC_BCH_SHORT)?aml_chip->bch_info:bch_mode),
		((bch_mode == NAND_ECC_BCH_SHORT)?1:0), dma_unit_size, count);

	ret = aml_platform_dma_waiting(aml_chip);

	if (aml_chip->oob_fill_cnt >0) {
		NFC_SEND_CMD_M2N_RAW(controller,
			aml_chip->ran_mode, aml_chip->oob_fill_cnt);
		ret = aml_platform_dma_waiting(aml_chip);
	}
	return ret;
}

static int m3_nand_dma_read(struct aml_nand_chip *aml_chip,
	unsigned char *buf, int len, unsigned bch_mode)
{
	volatile unsigned int * info_buf=0;
	volatile int cmp=0;

	struct nand_chip *chip = &aml_chip->chip;
	unsigned dma_unit_size = 0, count = 0, info_times_int_len;
	int ret = 0;
	struct mtd_info *mtd = &chip->mtd;
	uint32_t temp;

	info_times_int_len = PER_INFO_BYTE/sizeof(unsigned int);
	if (bch_mode == NAND_ECC_NONE)
		count = 1;
	else if (bch_mode == NAND_ECC_BCH_SHORT) {
		dma_unit_size = (chip->ecc.size >> 3);
		count = len/chip->ecc.size;
	} else
		count = chip->ecc.steps;
	/*printk("___count: %d, bch_mod:%d",count,bch_mode);*/
	memset((unsigned char *)aml_chip->user_info_buf,
		0, count*PER_INFO_BYTE);
	flush_dcache_range((unsigned long)aml_chip->user_info_buf,
		(unsigned long)aml_chip->user_info_buf + count*PER_INFO_BYTE);
	invalidate_dcache_range((unsigned long)buf, (unsigned long)buf + len);

	NFC_SEND_CMD_ADL(controller, (u32)(unsigned long)buf);
	NFC_SEND_CMD_ADH(controller, (u32)(unsigned long)buf);
	NFC_SEND_CMD_AIL(controller,
		(u32)(unsigned long)aml_chip->user_info_buf);
	NFC_SEND_CMD_AIH(controller,
		(u32)(unsigned long)aml_chip->user_info_buf);
	if (aml_chip->ran_mode) {
		temp = mtd->writesize >> chip->page_shift;
		if (aml_chip->plane_num == 2)
			NFC_SEND_CMD_SEED(controller,
				(aml_chip->page_addr / temp) * temp);
		else
				NFC_SEND_CMD_SEED(controller,
					aml_chip->page_addr);
	}

	if (bch_mode == NAND_ECC_NONE)
		NFC_SEND_CMD_N2M_RAW(controller, 0, len);
	else
		NFC_SEND_CMD_N2M(controller, aml_chip->ran_mode,
		((bch_mode == NAND_ECC_BCH_SHORT)?aml_chip->bch_info:bch_mode),
		((bch_mode == NAND_ECC_BCH_SHORT)?1:0), dma_unit_size, count);

	ret = aml_platform_dma_waiting(aml_chip);
	if (ret)
		return ret;
	do {
		invalidate_dcache_range((unsigned long)aml_chip->user_info_buf,
		(unsigned long)aml_chip->user_info_buf + count*PER_INFO_BYTE);
		info_buf =
(volatile unsigned *)&(aml_chip->user_info_buf[(count-1)*info_times_int_len]);
		cmp = *info_buf;
	} while((cmp)==0);/*judge DMA read over*/

	return 0;
}

static int m3_nand_hwecc_correct(struct aml_nand_chip *aml_chip,
	unsigned char *buf, unsigned size, unsigned char *oob_buf)
{
	struct nand_chip *chip = &aml_chip->chip;
	unsigned ecc_step_num, usr_info, tmp_value;
	unsigned info_times_int_len = PER_INFO_BYTE / sizeof(unsigned int);

	if (size % chip->ecc.size) {
		printk ("error parameter size for ecc correct %x\n", size);
		return -EINVAL;
	}

	 for (ecc_step_num = 0;
		ecc_step_num < (size / chip->ecc.size); ecc_step_num++) {
		/* check if there have uncorrectable sector */
		tmp_value = ecc_step_num * info_times_int_len;
		usr_info = *(unsigned *)(&aml_chip->user_info_buf[tmp_value]);
		if (NAND_ECC_CNT(usr_info) == 0x3f) {
					aml_chip->zero_cnt = NAND_ZERO_CNT(usr_info);
			return -EIO;

		} else
			aml_chip->ecc_cnt_cur =
				NAND_ECC_CNT(usr_info);
	}

	return 0;
}

void register_aml_chip_controller(struct aml_nand_chip *aml_chip)
{
	/*register amlogic hw controller functions*/
	aml_chip->aml_nand_hw_init = m3_nand_hw_init;
	aml_chip->aml_nand_adjust_timing = m3_nand_adjust_timing;
	aml_chip->aml_nand_select_chip = m3_nand_select_chip;
	aml_chip->aml_nand_options_confirm = m3_nand_options_confirm;
	aml_chip->aml_nand_dma_read = m3_nand_dma_read;
	aml_chip->aml_nand_dma_write = m3_nand_dma_write;
	aml_chip->aml_nand_hwecc_correct = m3_nand_hwecc_correct;
	aml_chip->aml_nand_cmd_ctrl = aml_platform_cmd_ctrl;
	aml_chip->aml_nand_write_byte = aml_platform_write_byte;
	aml_chip->aml_nand_wait_devready = aml_platform_wait_devready;
	aml_chip->aml_nand_get_user_byte = aml_platform_get_user_byte;
	aml_chip->aml_nand_set_user_byte = aml_platform_set_user_byte;
	aml_chip->aml_nand_command = aml_nand_base_command;
	aml_chip->aml_nand_block_bad_scrub =
		aml_nand_block_bad_scrub_update_bbt;
}

extern int slcnand_fit_storage(void);
static int m3_nand_probe(struct aml_nand_platform *plat, unsigned dev_num)
{
	struct aml_nand_chip *aml_chip = NULL;
	struct nand_chip *chip = NULL;
	struct mtd_info *mtd = NULL;
	int err = 0, i, array_length;
	struct nand_oobfree *oobfree;

	aml_chip = kzalloc(sizeof(*aml_chip), GFP_KERNEL);
	if (aml_chip == NULL) {
		printk("no memory for flash info\n");
		err = -ENOMEM;
		goto exit_error;
	}

	/* initialize mtd info data struct */
	aml_chip->controller = controller;
	aml_chip->platform = plat;
	aml_chip->bch_desc = m3_bch_list;
	aml_chip->max_bch_mode = sizeof(m3_bch_list) / sizeof(m3_bch_list[0]);

	chip = &aml_chip->chip;
	chip->priv = aml_chip;
	mtd = &chip->mtd;
	mtd->priv = chip;
	plat->aml_chip = aml_chip;
	mtd->name = plat->name;

	register_aml_chip_controller(aml_chip);
	aml_chip->ran_mode = plat->ran_mode;
	aml_chip->rbpin_detect = plat->rbpin_detect;
	aml_nfc_get_clk_name(controller);
	chip->IO_ADDR_R = chip->IO_ADDR_W =
		(void __iomem *)((volatile u32 *)(NAND_BASE_APB + P_NAND_BUF));

	err = aml_nand_init(aml_chip);
	if (!err) {
		nand_info[dev_num] = mtd;
		slcnand_fit_storage();
		mtd_store_set(nand_info[dev_num], dev_num);
	}
	if (err || pre_scan->pre_scan_flag)
		goto exit_error;

	if (!strncmp((char*)plat->name,
		NAND_BOOT_NAME, strlen((const char*)NAND_BOOT_NAME))) {
		chip->erase = m3_nand_boot_erase_cmd;
		chip->ecc.read_page = m3_nand_boot_read_page_hwecc;
		chip->ecc.write_page = m3_nand_boot_write_page_hwecc;
		chip->write_page = m3_nand_boot_write_page;
		oobfree = chip->ecc.layout->oobfree;
		array_length = ARRAY_SIZE(chip->ecc.layout->oobfree);
		if (chip->ecc.layout) {
			oobfree[0].length =
			(mtd->writesize / 512) * aml_chip->user_byte_mode;
			chip->ecc.layout->oobavail = 0;
			for (i = 0; oobfree[i].length && i < array_length; i++)
				chip->ecc.layout->oobavail += oobfree[i].length;
			mtd->oobavail = chip->ecc.layout->oobavail;
			mtd->ecclayout = chip->ecc.layout;
		}
	}

	return 0;

exit_error:
	if (aml_chip)
		kfree(aml_chip);
	if (mtd)
		mtd->name = NULL;
	return err;
}

void nand_hw_init(struct aml_nand_platform *plat)
{
	struct aml_nand_chip *aml_chip = NULL;

	aml_chip = plat->aml_chip;

	aml_chip->aml_nand_hw_init(aml_chip);
	if (aml_chip->aml_nand_adjust_timing)
		aml_chip->aml_nand_adjust_timing(aml_chip);
	aml_chip->aml_nand_select_chip(aml_chip, 0);
}

static void meson_nfc_init_dm(void)
{
	struct udevice *dev;

	for (uclass_first_device(UCLASS_MTD, &dev);
	     dev;
	     uclass_next_device(&dev));
}

void board_nand_init(void)
{
	meson_nfc_init_dm();
}

int amlmtd_init = 0;
extern struct udevice *nand_dev;
int meson_nfc_probe(struct udevice *dev)
{
	struct aml_nand_platform *plat = NULL;
	const void *blob = gd->fdt_blob;
	fdt_addr_t regs, clk_regs;
	int node;
	int i, ret = 0;

	if (1 == amlmtd_init) {
		ret = pinctrl_select_state(controller->device, "default");
		if (ret) {
			printf("select state %s failed\n", "default");
			return ret;
		}
		nand_hw_init(&aml_nand_mid_device.aml_nand_platform[0]);
		return 0;
	}

	controller = kzalloc(sizeof(struct hw_controller), GFP_KERNEL);
	if (controller == NULL) {
		printk("%s kzalloc controller failed\n", __func__);
		return 1;
	}
	nand_dev = dev;
	controller->device = dev;

	ret = pinctrl_select_state(controller->device, "default");
	if (ret) {
		printf("ERROR get pinmux failed\n");
	}

	controller->chip_num = 1; /* assume chip num is 1 */
	for (i = 0; i < MAX_CHIP_NUM; i++) {
		controller->ce_enable[i] =
			(((CE_PAD_DEFAULT >> i*4) & 0xf) << 10);
		controller->rb_enable[i] =
			(((RB_PAD_DEFAULT >> i*4) & 0xf) << 10);
	}

	node = fdtdec_next_compatible(blob, 0, COMPAT_MESON_NAND);
	if (node < 0) {
		printk("unable to find nfc node in device tree\n");
		return 1;
	}
	if (!fdtdec_get_is_enabled(blob, node)) {
		printk("nfc disabled in device tree\n");
		return 1;
	}

	regs = fdtdec_get_addr(blob, node, "reg");
	if (regs == FDT_ADDR_T_NONE) {
		printk("unabled to find nfc address in device tree\n");
		return 1;
	}
	controller->reg_base = (void *)regs;

	clk_regs = fdtdec_get_addr(blob, node, "clk_reg");
	if (clk_regs == FDT_ADDR_T_NONE) {
		printk("unabled to find nfc clk address in device tree\n");
		return 1;
	}
	controller->nand_clk_reg = (void *)clk_regs;

	for (i=0; i<aml_nand_mid_device.dev_num; i++) {
		plat = &aml_nand_mid_device.aml_nand_platform[i];
		if (!plat) {
			printk("error for not platform data\n");
			continue;
		}

		ret = m3_nand_probe(plat, i);
		if (pre_scan->pre_scan_flag && !i) {
			free(controller);
			return 0;
		}
		if (ret)
			printk("nand init failed: %d\n", ret);
	}
	nand_curr_device = 1; //fixit
	amlmtd_init = 1;
	if (ret)
		free(controller);

	return 0;
}

static const struct udevice_id aml_nfc_ids[] = {
	{ .compatible = "amlogic,meson-nfc" },
	{}
};

U_BOOT_DRIVER(meson_nfc) = {
	.name	= "meson-nfc",
	.id	= UCLASS_MTD,
	.of_match = aml_nfc_ids,
	.probe = meson_nfc_probe,
};

