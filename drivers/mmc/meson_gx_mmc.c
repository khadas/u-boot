// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2016 Carlo Caione <carlo@caione.org>
 */

#include <common.h>
#include <dm.h>
#include <fdtdec.h>
#include <malloc.h>
#include <clk.h>
#include <mmc.h>
#include <asm/io.h>
#include <asm/arch/sd_emmc.h>
#include <linux/log2.h>

/* #define SD_DEBUG_ENABLE */

#ifdef SD_DEBUG_ENABLE
    #define sd_debug(a...) printf(a);
#else
    #define sd_debug(a...)
#endif

static inline void *get_regbase(const struct mmc *mmc)
{
	struct meson_mmc_platdata *pdata = mmc->priv;

	return pdata->regbase;
}

static int mmc_controller_debug(struct mmc_cmd *cmd,uint32_t status)
{
	int ret =0;
	if (status & STATUS_RXD_ERR_MASK) {
		ret |= SD_EMMC_RXD_ERROR;
		printf("emmc/sd read error, cmd%d, status=0x%x\n",
			cmd->cmdidx, status);
	}
	if (status & STATUS_TXD_ERR) {
		ret |= SD_EMMC_TXD_ERROR;
		printf("emmc/sd write error, cmd%d, status=0x%x\n",
			cmd->cmdidx, status);
	}
	if (status & STATUS_DESC_ERR) {
		ret |= SD_EMMC_DESC_ERROR;
		printf("emmc/sd descripter error, cmd%d, status=0x%x\n",
			cmd->cmdidx, status);
	}
	if (status & STATUS_RESP_ERR) {
		ret |= SD_EMMC_RESP_CRC_ERROR;
		printf("emmc/sd response crc error, cmd%d, status=0x%x\n",
			cmd->cmdidx, status);
	}
	if (status & STATUS_RESP_TIMEOUT) {
		ret |= SD_EMMC_RESP_TIMEOUT_ERROR;
		printf("emmc/sd response timeout, cmd%d, status=0x%x\n",
			cmd->cmdidx, status);
	}
	if (status & STATUS_DESC_TIMEOUT) {
		ret |= SD_EMMC_DESC_TIMEOUT_ERROR;
		printf("emmc/sd descripter timeout, cmd%d, status=0x%x\n",
			cmd->cmdidx, status);
	}

	sd_debug("cmd->cmdidx = %d, cmd->cmdarg=0x%x, ret=0x%x\n",cmd->cmdidx,cmd->cmdarg,ret);
	sd_debug("cmd->response[0]=0x%x;\n",cmd->response[0]);
	sd_debug("cmd->response[1]=0x%x;\n",cmd->response[1]);
	sd_debug("cmd->response[2]=0x%x;\n",cmd->response[2]);
	sd_debug("cmd->response[3]=0x%x;\n",cmd->response[3]);

	return ret;

}
static inline uint32_t meson_read(struct mmc *mmc, int offset)
{
	return readl(get_regbase(mmc) + offset);
}

static inline void meson_write(struct mmc *mmc, uint32_t val, int offset)
{
	writel(val, get_regbase(mmc) + offset);
}

static void meson_mmc_config_clock(struct mmc *mmc)
{
	struct clk clk_24, clk_1000, clk_gate, clk_div, clk_mux, core;
	struct udevice *clk_udevice;
	uint32_t meson_mmc_clk = 0;

	if (!mmc->clock)
		return;
	meson_mmc_clk =((0 << Cfg_irq_sdio_sleep_ds) |
					(0 << Cfg_irq_sdio_sleep) |
					(1 << Cfg_always_on) |
					(0 << Cfg_rx_delay) |
					(0 << Cfg_tx_delay) |
					(0 << Cfg_sram_pd) |
					(0 << Cfg_rx_phase) |
					(0 << Cfg_src) |
					(1 << Cfg_div));
	if (mmc->clock<25000000) {
		meson_mmc_clk |= (dev_read_u32_default(mmc->dev, "core-phase0", 2)<<Cfg_co_phase);
		meson_mmc_clk |= (dev_read_u32_default(mmc->dev, "tx-phase0", 0)<<Cfg_tx_phase);
	} else {
		meson_mmc_clk |= (dev_read_u32_default(mmc->dev, "core-phase1", 2)<<Cfg_co_phase);
		meson_mmc_clk |= (dev_read_u32_default(mmc->dev, "tx-phase1", 0)<<Cfg_tx_phase);
	}
	meson_write(mmc, meson_mmc_clk, MESON_SD_EMMC_CLOCK);


	uclass_get_device_by_name(UCLASS_CLK, "amlogic,g12a-clkc", &clk_udevice);


	clk_get_by_name(mmc->dev, "clkin", &clk_1000);
	clk_get_by_name(mmc->dev, "clkin1", &clk_24);
	clk_get_by_name(mmc->dev, "clkin2", &clk_mux);
	clk_get_by_name(mmc->dev, "clkin3", &clk_div);
	clk_get_by_name(mmc->dev, "clkin4", &clk_gate);
	clk_get_by_name(mmc->dev, "core", &core);

	clk_enable(&core);

	if (mmc->clock > 16000000)
		clk_set_parent(&clk_mux, &clk_1000);
	else
		clk_set_parent(&clk_mux, &clk_24);

	clk_set_rate(&clk_div, mmc->clock);
	clk_enable(&clk_gate);

}

static int meson_dm_mmc_set_ios(struct udevice *dev)
{
	struct mmc *mmc = mmc_get_mmc_dev(dev);
	uint32_t meson_mmc_cfg;

	meson_mmc_config_clock(mmc);

	meson_mmc_cfg = meson_read(mmc, MESON_SD_EMMC_CFG);

	meson_mmc_cfg &= ~CFG_BUS_WIDTH_MASK;
	if (mmc->bus_width == 1)
		meson_mmc_cfg |= CFG_BUS_WIDTH_1;
	else if (mmc->bus_width == 4)
		meson_mmc_cfg |= CFG_BUS_WIDTH_4;
	else if (mmc->bus_width == 8)
		meson_mmc_cfg |= CFG_BUS_WIDTH_8;
	else
		return -EINVAL;

	/* 512 bytes block length */
	meson_mmc_cfg &= ~CFG_BL_LEN_MASK;
	meson_mmc_cfg |= CFG_BL_LEN_512;

	/* Response timeout 256 clk */
	meson_mmc_cfg &= ~CFG_RESP_TIMEOUT_MASK;
	meson_mmc_cfg |= CFG_RESP_TIMEOUT_256;

	/* Command-command gap 16 clk */
	meson_mmc_cfg &= ~CFG_RC_CC_MASK;
	meson_mmc_cfg |= CFG_RC_CC_16;

	meson_write(mmc, meson_mmc_cfg, MESON_SD_EMMC_CFG);

	return 0;
}

static void meson_mmc_setup_cmd(struct mmc *mmc, struct mmc_data *data,
				struct mmc_cmd *cmd)
{
	uint32_t meson_mmc_cmd = 0, cfg;

	meson_mmc_cmd |= cmd->cmdidx << CMD_CFG_CMD_INDEX_SHIFT;

	if (cmd->resp_type & MMC_RSP_PRESENT) {
		if (cmd->resp_type & MMC_RSP_136)
			meson_mmc_cmd |= CMD_CFG_RESP_128;

		if (cmd->resp_type & MMC_RSP_BUSY)
			meson_mmc_cmd |= CMD_CFG_R1B;

		if (!(cmd->resp_type & MMC_RSP_CRC))
			meson_mmc_cmd |= CMD_CFG_RESP_NOCRC;
	} else {
		meson_mmc_cmd |= CMD_CFG_NO_RESP;
	}

	if (data) {
		cfg = meson_read(mmc, MESON_SD_EMMC_CFG);
		cfg &= ~CFG_BL_LEN_MASK;
		cfg |= ilog2(data->blocksize) << CFG_BL_LEN_SHIFT;
		meson_write(mmc, cfg, MESON_SD_EMMC_CFG);

		if (data->flags == MMC_DATA_WRITE)
			meson_mmc_cmd |= CMD_CFG_DATA_WR;

		meson_mmc_cmd |= CMD_CFG_DATA_IO | CMD_CFG_BLOCK_MODE |
				 data->blocks;
	}

	meson_mmc_cmd |= CMD_CFG_TIMEOUT_4S | CMD_CFG_OWNER |
			 CMD_CFG_END_OF_CHAIN;

	meson_write(mmc, meson_mmc_cmd, MESON_SD_EMMC_CMD_CFG);
}

static void meson_mmc_setup_addr(struct mmc *mmc, struct mmc_data *data)
{
	struct meson_mmc_platdata *pdata = mmc->priv;
	unsigned int data_size;
	uint32_t data_addr = 0;

	if (data) {
		data_size = data->blocks * data->blocksize;

		if (data->flags == MMC_DATA_READ) {
			data_addr = (ulong) data->dest;
			invalidate_dcache_range(data_addr,
						data_addr + data_size);
		} else {
			pdata->w_buf = calloc(data_size, sizeof(char));
			data_addr = (ulong) pdata->w_buf;
			memcpy(pdata->w_buf, data->src, data_size);
			flush_dcache_range(data_addr, data_addr + data_size);
		}
	}

	meson_write(mmc, data_addr, MESON_SD_EMMC_CMD_DAT);
}

static void meson_mmc_read_response(struct mmc *mmc, struct mmc_cmd *cmd)
{
	if (cmd->resp_type & MMC_RSP_136) {
		cmd->response[0] = meson_read(mmc, MESON_SD_EMMC_CMD_RSP3);
		cmd->response[1] = meson_read(mmc, MESON_SD_EMMC_CMD_RSP2);
		cmd->response[2] = meson_read(mmc, MESON_SD_EMMC_CMD_RSP1);
		cmd->response[3] = meson_read(mmc, MESON_SD_EMMC_CMD_RSP);
	} else {
		cmd->response[0] = meson_read(mmc, MESON_SD_EMMC_CMD_RSP);
	}
}

static int meson_dm_mmc_send_cmd(struct udevice *dev, struct mmc_cmd *cmd,
				 struct mmc_data *data)
{
	struct mmc *mmc = mmc_get_mmc_dev(dev);
	struct meson_mmc_platdata *pdata = mmc->priv;
	uint32_t status;
	ulong start;
	int ret = 0;

	/* max block size supported by chip is 512 byte */
	if (data && data->blocksize > 512)
		return -EINVAL;

	meson_mmc_setup_cmd(mmc, data, cmd);
	meson_mmc_setup_addr(mmc, data);

	meson_write(mmc, cmd->cmdarg, MESON_SD_EMMC_CMD_ARG);

	/* use 10s timeout */
	start = get_timer(0);
	do {
		status = meson_read(mmc, MESON_SD_EMMC_STATUS);
	} while(!(status & STATUS_END_OF_CHAIN) && get_timer(start) < 10000);

	meson_mmc_read_response(mmc, cmd);

	ret = mmc_controller_debug(cmd,status);
	if (data && data->flags == MMC_DATA_WRITE)
		free(pdata->w_buf);

	/* reset status bits */
	meson_write(mmc, STATUS_MASK, MESON_SD_EMMC_STATUS);
	if (ret) {
			if (status & STATUS_RESP_TIMEOUT)
				return -ETIMEDOUT;
			else
				return ret;
	}
	return ret;
}

static const struct dm_mmc_ops meson_dm_mmc_ops = {
	.send_cmd = meson_dm_mmc_send_cmd,
	.set_ios = meson_dm_mmc_set_ios,
};

static int meson_mmc_ofdata_to_platdata(struct udevice *dev)
{
	struct meson_mmc_platdata *pdata = dev_get_platdata(dev);
	fdt_addr_t addr;

	addr = devfdt_get_addr(dev);
	if (addr == FDT_ADDR_T_NONE)
		return -EINVAL;

	pdata->regbase = (void *)addr;

	return 0;
}

static int meson_mmc_probe(struct udevice *dev)
{
	struct meson_mmc_platdata *pdata = dev_get_platdata(dev);
	struct mmc_uclass_priv *upriv = dev_get_uclass_priv(dev);
	struct mmc *mmc = &pdata->mmc;
	struct mmc_config *cfg = &pdata->cfg;
	uint32_t val;

	cfg->voltages = MMC_VDD_33_34 | MMC_VDD_32_33 |
			MMC_VDD_31_32 | MMC_VDD_165_195;
	cfg->host_caps = MMC_MODE_8BIT | MMC_MODE_4BIT |
			MMC_MODE_HS_52MHz | MMC_MODE_HS;
	cfg->f_min = DIV_ROUND_UP(SD_EMMC_CLKSRC_24M, CLK_MAX_DIV);
	cfg->f_max = 100000000; /* 100 MHz */
	cfg->b_max = 511; /* max 512 - 1 blocks */
	cfg->name = dev->name;

	mmc->priv = pdata;
	upriv->mmc = mmc;

	mmc_set_clock(mmc, cfg->f_min, false);


	/* reset all status bits */
	meson_write(mmc, STATUS_MASK, MESON_SD_EMMC_STATUS);

	/* disable interrupts */
	meson_write(mmc, 0, MESON_SD_EMMC_IRQ_EN);

	/* enable auto clock mode */
	val = meson_read(mmc, MESON_SD_EMMC_CFG);
	val &= ~CFG_SDCLK_ALWAYS_ON;
	val |= CFG_AUTO_CLK;
	meson_write(mmc, val, MESON_SD_EMMC_CFG);
	return 0;
}

int meson_mmc_bind(struct udevice *dev)
{
	struct meson_mmc_platdata *pdata = dev_get_platdata(dev);

	return mmc_bind(dev, &pdata->mmc, &pdata->cfg);
}

static const struct udevice_id meson_mmc_match[] = {
	{ .compatible = "amlogic,meson-gx-mmc" },
	{ .compatible = "amlogic,meson-axg-mmc" },
	{ /* sentinel */ }
};

U_BOOT_DRIVER(meson_mmc) = {
	.name = "meson_gx_mmc",
	.id = UCLASS_MMC,
	.of_match = meson_mmc_match,
	.ops = &meson_dm_mmc_ops,
	.probe = meson_mmc_probe,
	.bind = meson_mmc_bind,
	.ofdata_to_platdata = meson_mmc_ofdata_to_platdata,
	.platdata_auto_alloc_size = sizeof(struct meson_mmc_platdata),
};
