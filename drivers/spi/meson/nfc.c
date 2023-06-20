// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include "nfc.h"
#include "page_info.h"

struct nfc_clk_provider clk_provider[MAX_CLK_PROVIDER] = {
	{OSC_CLK_24MHZ, CLK_6MHZ,	1, 4, 0, 1},
	{OSC_CLK_24MHZ, CLK_12MHZ,	1, 2, 0, 2},
	{FIX_PLL_DIV2,	CLK_20MHZ,	12, 4, 0, 3},
	{FIX_PLL_DIV2,	CLK_41MHZ,	6, 4, 0, 5},
	{FIX_PLL_DIV2,	CLK_83MHZ,	3, 4, 0, 7},
	{FIX_PLL_DIV2P5, CLK_100MHZ_400, 2, 4, 0, 11},
	{FIX_PLL_DIV2,	CLK_100MHZ_500,	2, 5, 0, 11},
	{FIX_PLL_DIV2,	CLK_125MHZ,	2, 4, 0, 13},
	{FIX_PLL_DIV2P5, CLK_133MHZ,	2, 3, 0, 13},
	{FIX_PLL_DIV2,	CLK_166MHZ_333,	3, 2, 0, 15},
	{FIX_PLL_DIV3,	CLK_166MHZ_666,	1, 4, 0, 15},
	{FIX_PLL_DIV2P5, CLK_200MHZ_400, 2, 2, 0, 21},
	{FIX_PLL_DIV2P5, CLK_200MHZ_800, 1, 4, 0, 21},
};

struct nfc_clk_provider *g_clk_info = &clk_provider[CLK_83MHZ];

void nfc_pinmux_init(enum NFC_STORAGE_TYPE storage_type)
{
	unsigned int value;

	/*
	 * Bit2 of PADCTRL_MISC_CTRL0: spi_nand_sel_pad
	 * spi_nand_sel_pad == 1, the spi pinmux should select func4.
	 * spi_nand_sel_pad == 0, the spi pinmux should select func1. but it
	 *                        maybe have riskiness
	 */
	switch (storage_type) {
	case SPINAND_FLASH:
	case SPINOR_FLASH:
		/* d0-7 and cs pull up; clk pull down */
		writel(readl(PADCTRL_GPIOB_PULL_EN) | 0x24FF,
		       PADCTRL_GPIOB_PULL_UP);
		value = readl(PADCTRL_GPIOB_PULL_UP) & (~0x24FF);
		writel(value | 0x20FF, PADCTRL_GPIOB_PULL_UP);

		writel(0x44444444, PADCTRL_PIN_MUX_REG0);
		value = readl(PADCTRL_PIN_MUX_REG1) & (~0x00F00F00);
		writel(value | 0x00400400, PADCTRL_PIN_MUX_REG1);
		break;
#ifdef CONFIG_BOOT_SPI_NAND
	case SLCNAND_FLASH:
		writel(readl(PADCTRL_GPIOB_PULL_EN) | 0xFFFF,
		       PADCTRL_GPIOB_PULL_EN);
		writel(0xF500, PADCTRL_GPIOB_PULL_UP);

		writel(0x11111111, PADCTRL_PIN_MUX_REG0);
		writel(0x22222, PADCTRL_PIN_MUX_REG1);
		break;
#endif
	default:
		break;
	}
}

void nfc_send_idle(uint32_t cycle)
{
	writel(CE0 | IDLE | cycle, NAND_CMD);
}

void nfc_deselect_chip(uint32_t cycle)
{
	writel(CEF | IDLE | cycle, NAND_CMD);
}

void nfc_select_mode(int spi_mode)
{
	unsigned int value = 0;

	if (spi_mode) {
		value = readl(SPI_CFG);
		value |= (1 << 31);
	}
	writel(value, SPI_CFG);
}

unsigned int nfc_recalculate_n2m_command(uint32_t size, int no_cal)
{
	uint32_t n2m_cmd, page_size;
	int ecc_size;

	page_size = page_info_get_page_size();
	n2m_cmd = page_info_get_n2m_command();
	if (no_cal)
		return n2m_cmd;

	if (size == page_size)
		return n2m_cmd;
	ecc_size = (GET_BCH_MODE(n2m_cmd) == 1) ? 512 : 1024;
	size = (size + ecc_size - 1) & (~(ecc_size - 1));
	n2m_cmd &= ~0x3F;
	while (size >= ecc_size) {
		n2m_cmd++;
		size -= ecc_size;
	}
	return n2m_cmd;
}

int nfc_wait_command_fifo_done(uint32_t timeout, int repeat_rb)
{
	unsigned int t0 = get_timer(0), value;

	while (get_timer(t0) < timeout) {
		value = readl(NAND_CMD);
		if (((value >> 22) & 0x1f) == 0)
			return 0;
		if (repeat_rb && ((value >> 27) & 0x01))
			return ERR_NFC_TIMEOUT;
	}

	return ERR_NFC_TIMEOUT;
}

void nfc_raw_size_ext_convert(uint32_t size)
{
	unsigned int raw_ext;

	raw_ext = readl(SPI_CFG);
	raw_ext &= ~(0xFFF << 18);
	raw_ext |= ((size >> NFC_RAW_CHUNK_SHIFT) << 18);
	writel(raw_ext, SPI_CFG);
	NFC_Print("SPI_CFG", readl(SPI_CFG));
}

#define is_fixpll_locked()	(1)
#define IS_FEAT_EN_83MHZ_SPI()	(0)
#define IS_FEAT_EN_41MHZ_SPI()	(1)
#define IS_FEAT_EN_25MHZ_NAND()	(1)
#define otp_get_nfc_rxadj(x)	((*(x)) = 0)
static struct nfc_clk_provider *nfc_get_clock_provider(int init_stage,
					enum NFC_STORAGE_TYPE storage_type)
{
	struct nfc_clk_provider *clk_info;
	unsigned char frequency_idx, cs_deselect_time;
	uint32_t line_delay1, line_delay2;

	if (storage_type == SLCNAND_FLASH) {
		clk_info = &clk_provider[CLK_6MHZ];
		if (is_fixpll_locked() && IS_FEAT_EN_25MHZ_NAND())
			clk_info = &clk_provider[CLK_20MHZ];
		goto _exit_get_clk;
	}

	cs_deselect_time = page_info_get_cs_deselect_time();
	frequency_idx = page_info_get_frequency_index();
	if (init_stage || frequency_idx == 0xFF) {
		if (!is_fixpll_locked())
			clk_info = &clk_provider[CLK_12MHZ];
		else if (IS_FEAT_EN_83MHZ_SPI())
			clk_info = &clk_provider[CLK_83MHZ];
		else if (IS_FEAT_EN_41MHZ_SPI())
			clk_info = &clk_provider[CLK_41MHZ];
		else
			clk_info = &clk_provider[CLK_20MHZ];

		otp_get_nfc_rxadj(&clk_info->adj);
	} else {
		line_delay1 = page_info_get_line_delay1();
		line_delay2 = page_info_get_line_delay2();
		clk_info = &clk_provider[frequency_idx & 0x7F];
		clk_info->adj = page_info_get_adj_index();
		if (frequency_idx & 0x80) {
			clk_info->div1 = page_info_get_core_div();
			clk_info->div2 = page_info_get_bus_cycle();
		}
		writel(line_delay1, SD_EMMC_DLY1);
		writel(line_delay2, SD_EMMC_DLY2);
	}

	if (cs_deselect_time != 0xFF) {
		clk_info->cs_deselect_time = cs_deselect_time;
		NFC_Print("clk_info->cs_deselect_time", clk_info->cs_deselect_time);
	}

_exit_get_clk:
	g_clk_info = clk_info;
	return clk_info;
}

void nfc_set_clock_and_timing(int init_stage, enum NFC_STORAGE_TYPE storage_type)
{
	struct nfc_clk_provider *clk_info;
	unsigned char data_lanes, cmd_lanes, addr_lanes, mode;
	uint32_t value;

	setbits_le32(CLKCTRL_SYS_CLK_EN0_REG0,
		     CTRL_NAND_CLK_GATE_ON_BIT);
	setbits_le32(CLKCTRL_NAND_CLK_CTRL,
		     CTRL_SDEMMC_CLK_CTRL_ON_BIT);

	clk_info = nfc_get_clock_provider(init_stage, storage_type);
	NFC_Print("clock_index", clk_info->clk_rate);
	value = CLK_DEF_CORE_PHASE | CLK_ALWAYS_ON | CLK_SELECT_NAND;
	value |= clk_info->div1;
	if (clk_info->clk_src_type == FIX_PLL_DIV2) {
		value |= (1 << 6);
		writel(value, SD_EMMC_CLK);
	} else {
		value &= ~(3 << 6);
		writel(value, SD_EMMC_CLK);
		setbits_le32(CLKCTRL_NAND_CLK_CTRL,
			     clk_info->clk_src_type << 9);
	}

	if (storage_type != SLCNAND_FLASH) {
		data_lanes = page_info_get_data_lanes_mode();
		cmd_lanes = page_info_get_cmd_lanes_mode();
		addr_lanes = page_info_get_addr_lanes_mode();
		mode = page_info_get_work_mode();

		writel(data_lanes | (cmd_lanes << 2) |
		       ((clk_info->adj & 0x0F) << 4) |
		       (3 << 12) | (mode << 14) | (addr_lanes << 16) |
		       (1 << 31), SPI_CFG);
		writel((clk_info->div2 - 1) | (3 << 5) | (1 << 31), NAND_CFG);
	} else {
		writel(clk_info->div2 | (3 << 5) | (1 << 31), NAND_CFG);
	}

	NFC_Print("SPI_CFG", readl(SPI_CFG));
	NFC_Print("NAND_CFG", readl(NAND_CFG));
	NFC_Print("SD_EMMC_CLK", readl(SD_EMMC_CLK));
	NFC_Print("CLKCTRL_NAND_CLK_CTRL", readl(CLKCTRL_NAND_CLK_CTRL));
}

void nfc_set_data_bus_width(int bus_width)
{
	uint32_t spi_cfg = readl(SPI_CFG);

	spi_cfg &= (~0x03);
	spi_cfg |= bus_width;
	writel(spi_cfg, SPI_CFG);
	NFC_Print("SPI_CFG", readl(SPI_CFG));
}

uint32_t nfc_send_cmd_addr_and_wait(unsigned char cmd_bitmap, uint8_t cmd,
				    uint8_t *addr, uint8_t addr_len,
				    uint8_t *data, uint8_t data_len,
				    unsigned short state_dummy_ctl)
{
	uint8_t *buffer = data, dummy_cycles = 0, com_cmd;
	enum CMD_TYPE cmd_type;
	enum STATE_AND_DUMMY_CTL action;
	int i = 0, j = 0, ret;

	if (((cmd_bitmap & TYPE_ADDR) && !addr) ||
	    ((cmd_bitmap & TYPE_DATA_DRD) && !buffer) ||
	    ((cmd_bitmap & TYPE_DATA_DWR) && !buffer) ||
	    cmd_bitmap == 0)
		return ERR_NFC_WRONG_PARAM;

	action = (state_dummy_ctl >> STATE_AND_DUMMY_CTL_BIT_SHIFT);
	NFC_Print("cs_deselect_time", g_clk_info->cs_deselect_time);
	writel(CEF | IDLE | g_clk_info->cs_deselect_time, NAND_CMD);

	while (cmd_bitmap != 0 && i < 8) {
		cmd_type = CMD_BITMAP_TEST_AND_GET_BIT_VALUE(cmd_bitmap, i);
		CMD_BITMAP_CLEAR_BIT(cmd_bitmap, i);
		switch (cmd_type) {
		case TYPE_NONE:
			/* skip the zero */
			break;
		case TYPE_CMD:
			NFC_Print("TYPE_CMD", CE0 | CLE | cmd);
			if (action & DONT_SEND_CEF) {
				com_cmd = ~cmd;
				NFC_Print("TYPE_CMD", CE0 | CLE | com_cmd);
				writel(CE0 | CLE | com_cmd, NAND_CMD);
			}
			writel(CE0 | CLE | cmd, NAND_CMD);
			break;
		case TYPE_ADDR:
			if (state_dummy_ctl & 0xFF)
				dummy_cycles = (state_dummy_ctl & 0xFF) - 1;
			NFC_Print("dummy_cycles", dummy_cycles);
			if (action & DUMMY_BEFORE_ADDRESS)
				writel(CE0 | DUMMY | dummy_cycles, NAND_CMD);
			addr += addr_len - 1;
			for (j = 0; j < addr_len; j++, addr--) {
				NFC_Print("TYPE_ADDR", CE0 | ALE | *addr);
				writel(CE0 | ALE | *addr, NAND_CMD);
			}
			if (action & DUMMY_AFTER_ADDRESS)
				writel(CE0 | DUMMY | dummy_cycles, NAND_CMD);
			break;
		case TYPE_DATA_DRD:
			NFC_Print("TYPE_DATA_DRD", i);
			for (j = 0; j < data_len; j++, buffer++) {
				writel(CE0 | DRD | 0, NAND_CMD);
				writel(CE0 | IDLE, NAND_CMD);
				writel(CE0 | IDLE, NAND_CMD);
				ret = nfc_wait_command_fifo_done(NFC_COMMAND_FIFIO_TIMEOUT, 0);
				if (ret)
					return ret;
				*buffer = readl(NAND_BUF);
			}
			return 0;
		case TYPE_DATA_DWR:
			NFC_Print("TYPE_DATA_DWR", i);
			for (j = 0; j < data_len; j++, buffer++) {
				writel(CE0 | IDLE | 10, NAND_CMD);
				writel(CE0 | DWR | *buffer, NAND_CMD);
				writel(CE0 | IDLE | 10, NAND_CMD);
				writel(CE0 | IDLE, NAND_CMD);
				writel(CE0 | IDLE, NAND_CMD);
				ret = nfc_wait_command_fifo_done(NFC_COMMAND_FIFIO_TIMEOUT, 0);
				if (ret)
					return ret;
			}
			return 0;
		default:
			break;
		}
		writel(CE0 | IDLE, NAND_CMD);
		i++;
	}

	return nfc_wait_command_fifo_done(NFC_COMMAND_FIFIO_TIMEOUT, 0);
}

void nfc_send_repeat_rb_command(void)
{
	writel(IO6 | RB | 13, NAND_CMD);
	writel(CE0 | IDLE | 2, NAND_CMD);
	writel(CE0 | CLE | 0, NAND_CMD);
	writel(CE0 | IDLE, NAND_CMD);
	writel(CE0 | IDLE, NAND_CMD);
}

void nfc_set_dma_mem_and_info(uint32_t mem, unsigned long info)
{
	uint32_t cmd;

	cmd = GENCMDDADDRL(NFC_CMD_ADL, mem);
	writel(cmd, NAND_CMD);
	cmd = GENCMDDADDRH(NFC_CMD_ADH, mem);
	writel(cmd, NAND_CMD);
	cmd = GENCMDIADDRL(NFC_CMD_AIL, info);
	writel(cmd, NAND_CMD);
	cmd = GENCMDIADDRH(NFC_CMD_AIH, info);
	writel(cmd, NAND_CMD);
}

int nfc_start_dma_and_wait_done(uint32_t n2m_cmd)
{
	writel(n2m_cmd, NAND_CMD);
	writel(CE0 | IDLE, NAND_CMD);
	writel(CE0 | IDLE, NAND_CMD);

	return nfc_wait_command_fifo_done(NFC_DATA_FIFIO_TIMEOUT, 0);
}

void nfc_set_user_byte(u8 *oob_buf, uint64_t *info_buf, int ecc_steps)
{
	uint64_t *info;
	int i, count;

	for (i = 0, count = 0; i < ecc_steps; i++, count += 2) {
		info = &info_buf[i];
		*info |= oob_buf[count];
		*info |= oob_buf[count + 1] << 8;
	}
}

void nfc_get_user_byte(u8 *oob_buf, uint64_t *info_buf, int ecc_steps)
{
	uint64_t *info;
	int i, count;

	for (i = 0, count = 0; i < ecc_steps; i++, count += 2) {
		info = &info_buf[i];
		oob_buf[count] = *info;
		oob_buf[count + 1] = *info >> 8;
	}
}

int nfc_wait_data_and_ecc_engine_done(uint64_t *info_buf,
				      unsigned char nsteps, int raw)
{
	volatile uint64_t *info;
	int i, count = 0;

	if (nsteps < 1)
		return ERR_NFC_WRONG_PARAM;

	info = info_buf + nsteps - 1;
	while (count <= 10000) {
		invalidate_dcache_range((unsigned long)info_buf,
			(unsigned long)info_buf + sizeof(uint64_t) * 8);
		if (*info & ECC_COMPLETE)
			break;
		if (count == 10000) {
			writel(1 << 31, NAND_CMD);
			return ERR_DMA_TIMEOUT;
		}
		udelay(1);
		count++;
	}

	if (raw)
		return 0;

	for (i = 0, info = info_buf; i < nsteps; i++, info++) {
		if (ECC_ERR_CNT(*info) != ECC_UNCORRECTABLE)
			continue;

		if (ECC_ZERO_CNT(*info) < 0x0a)
			return 0;

		return ERROR_ECC;
	}

	return 0;
}
