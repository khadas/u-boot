// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <clk.h>
#include <asm/cache.h>
#include <dm/device-internal.h>
#include <dm/uclass-internal.h>
#include <dm/root.h>
#include <dm/lists.h>
#include <dm/util.h>
#include <dm/pinctrl.h>
#include <spi.h>
#include "page_info.h"

//#define __SPI_NFC_DEBUG__

#ifdef __SPI_NFC_DEBUG__
#define SPI_NFC_DEBUG(...)	pr_info(__VA_ARGS__)
#else
#define SPI_NFC_DEBUG(...)
#endif

unsigned char spinand_in = 1;

struct spi_nfc_priv {
	unsigned char save_cmd;
	u32 save_addr;
	u32 save_addr_len;

	u32 spi_cfg;
	u32 nand_cfg;
	u32 sd_emmc_clk;

	u8 *data_buf;
	u8 *info_buf;
};

struct spi_nfc_platdata {
	u32 frequency_index;
};

#define DATA_BUF_SIZE		(4096)
#define OOB_BUF_SIZE		(128)
#define SPI_NFC_BUF_SIZE	(DATA_BUF_SIZE + OOB_BUF_SIZE)

static int spi_nfc_probe(struct udevice *bus)
{
	struct spi_nfc_priv *priv = dev_get_priv(bus);
	int ret;

	ret = pinctrl_select_state(bus, "default");
	if (ret) {
		pr_err("select state %s failed\n", "default");
		return ret;
	}

	priv->data_buf = kzalloc(SPI_NFC_BUF_SIZE, GFP_KERNEL);
	if (!priv->data_buf)
		return -1;

	priv->info_buf = priv->data_buf + DATA_BUF_SIZE;

	page_info_pre_init();
	nfc_set_clock_and_timing(NFC_STATUS_OFF, SPINAND_FLASH);

	return 0;
}

static int spi_nfc_ofdata_to_platdata(struct udevice *bus)
{
	struct spi_nfc_platdata *plat = dev_get_platdata(bus);
	const void *blob = gd->fdt_blob;
	int node = dev_of_offset(bus);

	plat->frequency_index = fdtdec_get_uint(blob, node,
				"clock_index", 0xFF);
	return 0;
}

static void spi_nfc_auto_oob_ops(u8 *info, u8 *oob_pos, int ecc_steps, bool set)
{
	u8 *src, *dst, i;

	for (i = 0; i < ecc_steps; i++) {
		src = (set) ? (info + 2 * i) : (oob_pos + i * (2 + 14));
		dst = (set) ? (oob_pos + i * (2 + 14)) : (info + 2 * i);
		memcpy(dst, src, 2);
	}
}

static void spi_nfc_covert_buf_to_user(struct spi_nfc_priv *priv,
				       u8 *user_buf,
				       u8 *oob_pos, u8 ecc_steps,
				       bool oob_only, bool auto_oob,
				       bool oob_required)
{
	u8 *buf = priv->data_buf, *info_buf = priv->info_buf;
	u8 oob_temp[OOB_BUF_SIZE] = {0};
	u32 page_size = page_info_get_page_size();

	SPI_NFC_DEBUG("oob only %s\n", (oob_only) ? "yes" : "no");
	SPI_NFC_DEBUG("oob is %s\n", (oob_required) ? "required" : "not required");
	SPI_NFC_DEBUG("auto_oob is %s\n", (auto_oob) ? "set" : "not set");
	if (oob_only || oob_required) {
		nfc_get_user_byte(oob_temp, (u64 *)info_buf, ecc_steps);
		DUMP_BUFFER(info_buf, 64, 4, 16);
		if (auto_oob)
			spi_nfc_auto_oob_ops(oob_temp, oob_pos,
				     ecc_steps, true);
		else
			memcpy(oob_pos, oob_temp, ecc_steps * 2);
	}

	if (oob_only)
		return;

	memcpy((u8 *)user_buf, buf, page_size);
	DUMP_BUFFER(user_buf, page_size, page_size / 512, 16);
}

static void spi_nfc_covert_buf_to_host(struct spi_nfc_priv *priv,
				       u8 *user_buf,
				       u8 *oob_pos, u8 ecc_steps,
				       bool oob_only, bool auto_oob,
				       bool oob_required)
{
	u8 *buf = priv->data_buf, *info_buf = priv->info_buf;
	u8 oob_temp[OOB_BUF_SIZE] = {0};
	u32 page_size = page_info_get_page_size();

	SPI_NFC_DEBUG("oob only %s\n", (oob_only) ? "yes" : "no");
	SPI_NFC_DEBUG("oob is %s\n", (oob_required) ? "required" : "not required");
	SPI_NFC_DEBUG("auto_oob is %s\n", (auto_oob) ? "set" : "not set");

	DUMP_BUFFER(oob_pos, OOB_BUF_SIZE, 4, 16);
	if (oob_only || oob_required) {
		if (auto_oob)
			spi_nfc_auto_oob_ops(oob_temp, oob_pos,
					     ecc_steps, false);
		else
			memcpy(oob_temp, oob_pos, ecc_steps * 2);
	}

	memset(info_buf, 0, 64);
	nfc_set_user_byte(oob_temp, (u64 *)info_buf, ecc_steps);

	if (oob_only)
		return;

	memcpy(buf, user_buf, page_size);
	DUMP_BUFFER(buf, page_size, page_size / 512, 16);
}

static int spi_nfc_ooblayout_ecc(struct mtd_info *mtd, int section,
			       struct mtd_oob_region *oobregion)
{
	if (section >= 8)
		return -ERANGE;

	oobregion->offset =  2 + (section * (2 + 14));
	oobregion->length = 14;

	return 0;
}

static int spi_nfc_ooblayout_free(struct mtd_info *mtd, int section,
				struct mtd_oob_region *oobregion)
{
	if (section >= 8)
		return -ERANGE;

	oobregion->offset = section * (2 + 14);
	oobregion->length = 2;

	return 0;
}

static const struct mtd_ooblayout_ops spi_nfc_ecc_ooblayout = {
	.ecc = spi_nfc_ooblayout_ecc,
	.free = spi_nfc_ooblayout_free,
};

static void spi_nfc_xfer_prepare(struct udevice *dev)
{
	struct dm_spi_slave_platdata *plat;
	struct mtd_info *mtd;

	plat = dev_get_parent_platdata(dev);
	if (!plat->cs) {
		spinand_in = 0;
		return;
	}

	if (!page_info_get_block_size()) {
		mtd = dev_get_uclass_priv(dev);
		page_info->dev_cfg0.page_size = mtd->writesize;
		page_info->dev_cfg1.block_size = mtd->erasesize;
		page_info->host_cfg.n2m_cmd =
			(DEFAULT_ECC_MODE & (~0x3F)) | mtd->writesize >> 9;
		SPI_NFC_DEBUG("page_size = 0x%x block_size = 0x%x\n",
			      page_info->dev_cfg0.page_size,
			      page_info->dev_cfg1.block_size);
		mtd_set_ooblayout(mtd, &spi_nfc_ecc_ooblayout);
	}
}

static int spi_nfc_dma_xfer(struct spi_nfc_priv *priv,
			    unsigned long flags,
			    u8 *user_buf, int len, bool read)
{
	u32 page_size, n2m_cmd;
	bool raw, oob_only, auto_oob, oob_required;
	u8 *buf, *oob_pos, ecc_steps;
	int ret = 0;

	oob_only = ((flags & SPI_XFER_OOB_ONLY) != 0);
	oob_required = ((flags & SPI_XFER_OOB) != 0);
	raw = ((flags & SPI_XFER_RAW) != 0);
	auto_oob = ((flags & SPI_XFER_AUTO_OOB) != 0);

	SPI_NFC_DEBUG("flags = %lx user_buf = %p len = 0x%x spinand = %d  %s\n",
		       flags, user_buf, len, spinand_in,
		       (read) ? "read" : "write");

	page_size = page_info_get_page_size();
	oob_pos = (oob_only) ? user_buf : (user_buf + page_size);

	if (raw || !spinand_in) {
		buf = user_buf;
		nfc_raw_size_ext_convert(len);
		n2m_cmd = (read) ? N2M_RAW : M2N_RAW;
		n2m_cmd |= (len & ((1 << NFC_RAW_CHUNK_SHIFT) - 1));
		ecc_steps = 1;
	} else {
		buf = priv->data_buf;
		n2m_cmd = nfc_recalculate_n2m_command(len, 1);
		ecc_steps = n2m_cmd & 0x3F;
		len = SPI_NFC_BUF_SIZE;
		if (!read) {
			n2m_cmd &= ~(N2M_RAW ^ M2N_RAW);
			spi_nfc_covert_buf_to_host(priv, user_buf,
						   oob_pos, ecc_steps,
						   oob_only, auto_oob,
						   oob_required);
		}
	}

	flush_dcache_range((unsigned long)buf,
			   (unsigned long)buf + len);

	if (spinand_in)
		priv->save_addr &= ~((page_size << 9) - 1);

	SPI_NFC_DEBUG("n2m_cmd = %x ecc_steps = %x save_addr = %x\n",
		       n2m_cmd, ecc_steps, priv->save_addr);

	ret = NFC_SEND_CMD_ADDR(priv->save_cmd,
				(uint8_t *)&priv->save_addr,
				priv->save_addr_len);
	if (ret)
		return ret;

	nfc_set_dma_mem_and_info((unsigned long)buf,
				 (unsigned long)priv->info_buf);

	ret = nfc_start_dma_and_wait_done(n2m_cmd);
	if (ret || !read)
		return ret;

	invalidate_dcache_range((unsigned long)priv->info_buf,
				(unsigned long)priv->info_buf + OOB_BUF_SIZE);
	ret = nfc_wait_data_and_ecc_engine_done((u64 *)priv->info_buf,
		ecc_steps, raw);
	if (ret) {
		SPI_NFC_DEBUG("ecc error dump:\n");
		DUMP_BUFFER(priv->info_buf, 64, 4, 16);
		DUMP_BUFFER(buf, page_size, page_size / 16, 16);
		return ret;
	}

	if (raw || !spinand_in) {
		DUMP_BUFFER(buf, len, len / 16, 16);
		return 0;
	}

	invalidate_dcache_range((unsigned long)buf,
				(unsigned long)buf + len);
	spi_nfc_covert_buf_to_user(priv, user_buf,
				   oob_pos, ecc_steps,
				   oob_only, auto_oob,
				   oob_required);

	return 0;
}

static int spi_nfc_xfer(struct udevice *dev,
			u32 bitlen, const void *dout,
			void *din, unsigned long flags)
{
	struct udevice *bus = dev->parent;
	struct spi_nfc_priv *priv = dev_get_priv(bus);
	int len = bitlen >> 3;
	u8 *buf = (u8 *)dout;
	int ret = 0, i = 0;

	writel(priv->spi_cfg, SPI_CFG);
	writel(priv->nand_cfg, NAND_CFG);
	writel(priv->sd_emmc_clk, SD_EMMC_CLK);

	if (flags & SPI_XFER_BEGIN) {
		priv->save_addr = 0;
		priv->save_addr_len = len - 1;
		for (i = 0; i < priv->save_addr_len; i++) {
			priv->save_addr <<= 8;
			priv->save_addr |= buf[i + 1];
		}

		if (flags & SPI_XFER_END) {
			/*  simple case , only command and address */
			if (priv->save_addr_len)
				ret = NFC_SEND_CMD_ADDR(buf[0],
					(uint8_t *)&priv->save_addr,
					len - 1);
			else
				ret = NFC_SEND_CMD(buf[0]);
			return ret;
		}
		/* command with data  */
		priv->save_cmd = buf[0];
		return 0;
	}

	if (din)
		buf = (u8 *)din;

	if (len <= SPINAND_MAX_ID_LEN + 2) {
		nfc_set_data_bus_width(0);
		if (din)
			ret = NFC_SEND_CMD_ADDR_DATA_RD(priv->save_cmd,
				(uint8_t *)&priv->save_addr,
				priv->save_addr_len, buf, len);
		else
			ret = NFC_SEND_CMD_ADDR_DATA_WR(priv->save_cmd,
				(uint8_t *)&priv->save_addr,
				priv->save_addr_len, buf, len);
		return ret;
	}

	spi_nfc_xfer_prepare(dev);
	spi_nfc_dma_xfer(priv, flags, buf, len, din);

	return 0;
}

static int spi_nfc_set_speed(struct udevice *bus, uint hz)
{
	return 0;
}

static int spi_nfc_set_mode(struct udevice *bus, uint mode)
{
	struct spi_nfc_priv *priv = dev_get_priv(bus);

	if (mode & SPI_RX_QUAD)
		page_info->dev_cfg0.bus_width |= 2;
	else if (mode & SPI_RX_DUAL)
		page_info->dev_cfg0.bus_width |= 1;

	nfc_set_clock_and_timing(NFC_STATUS_OFF, SPINAND_FLASH);

	priv->spi_cfg = readl(SPI_CFG);
	priv->nand_cfg = readl(NAND_CFG);
	priv->sd_emmc_clk = readl(SD_EMMC_CLK);

	return 0;
}

static int spi_nfc_set_wordlen(struct udevice *bus, u32 wordlen)
{
	if (wordlen != 8)
		return -1;

	return 0;
}

static const struct udevice_id spi_nfc_ids[] = {
	{ .compatible = "amlogic,spi_nfc" },
	{ }
};

static struct dm_spi_ops spi_nfc_ops = {
	.xfer = spi_nfc_xfer,
	.set_speed = spi_nfc_set_speed,
	.set_mode = spi_nfc_set_mode,
	.set_wordlen = spi_nfc_set_wordlen,
};

U_BOOT_DRIVER(spi_nfc) = {
	.name = "spi_nfc",
	.id = UCLASS_SPI,
	.of_match = spi_nfc_ids,
	.ofdata_to_platdata = spi_nfc_ofdata_to_platdata,
	.platdata_auto_alloc_size = sizeof(struct spi_nfc_platdata),
	.priv_auto_alloc_size = sizeof(struct spi_nfc_priv),
	.per_child_auto_alloc_size = sizeof(struct spi_slave),
	.ops = &spi_nfc_ops,
	.probe = spi_nfc_probe,
};
