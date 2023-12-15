/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _NFC_H_
#define _NFC_H_

#include <common.h>
#include <asm/cache.h>
#include <asm/io.h>
#include <asm/arch/cpu_sdio.h>
#include <timer.h>

#define SD_EMMC_CLK	(SD_EMMC_BASE_C)
#define SD_EMMC_DLY1	(SD_EMMC_BASE_C + 0x04)
#define SD_EMMC_DLY2	(SD_EMMC_BASE_C + 0x08)

#define P_NAND_BASE (SD_EMMC_BASE_C | (1 << 12))

#define PADCTRL_MISC_CTRL0 ((0x002a  << 2) + 0xfe004000)
#define PADCTRL_GPIOB_PULL_EN ((0x0093  << 2) + 0xfe004000)
#define PADCTRL_GPIOB_PULL_UP ((0x0094  << 2) + 0xfe004000)
#define CLKCTRL_SYS_CLK_EN0_REG0 ((0x0011  << 2) + 0xfe000000)
#define CLKCTRL_NAND_CLK_CTRL ((0x005a  << 2) + 0xfe000000)
#define PADCTRL_PIN_MUX_REG0 ((0x0000  << 2) + 0xfe004000)
#define PADCTRL_PIN_MUX_REG1 ((0x0001  << 2) + 0xfe004000)

#define NAND_CMD  (P_NAND_BASE + 0x00)
#define NAND_CFG  (P_NAND_BASE + 0x04)
#define NAND_DADR (P_NAND_BASE + 0x08)
#define NAND_IADR (P_NAND_BASE + 0x0c)
#define NAND_BUF  (P_NAND_BASE + 0x10)
#define NAND_INFO (P_NAND_BASE + 0x14)
#define NAND_DC   (P_NAND_BASE + 0x18)
#define NAND_ADR  (P_NAND_BASE + 0x1c)
#define NAND_DL   (P_NAND_BASE + 0x20)
#define NAND_DH   (P_NAND_BASE + 0x24)
#define NAND_CADR (P_NAND_BASE + 0x28)
#define NAND_SADR (P_NAND_BASE + 0x2c)

#define SPI_RX_IDX (P_NAND_BASE + 0x34)
#define SPI_RX_DAT (P_NAND_BASE + 0x38)
#define SPI_CFG    (P_NAND_BASE + 0x40)

#define CEF (0xf << 10)
#define CE0 (0xe << 10)
#define CE1 (0xd << 10)
#define CE2 (0xb << 10)
#define CE3 (0x7 << 10)

#define IO4 ((0xe << 10) | (1 << 18))
#define IO5 ((0xd << 10) | (1 << 18))
#define IO6 ((0xb << 10) | (1 << 18))

#define CLE  (0x5 << 14)
#define ALE  (0x6 << 14)
#define DWR  (0x4 << 14)
#define DRD  (0x8 << 14)
#define IDLE (0xc << 14)
#define RB   BIT(20)

#define M2N  ((0 << 17) | (2 << 20) | (1 << 19))
#define N2M  ((1 << 17) | (2 << 20) | (1 << 19))
#define STS  ((3 << 17) | (2 << 20))
#define ADL  ((0 << 16) | (3 << 20))
#define ADH  ((1 << 16) | (3 << 20))
#define AIL  ((2 << 16) | (3 << 20))
#define AIH  ((3 << 16) | (3 << 20))
#define ASL  ((4 << 16) | (3 << 20))
#define ASH  ((5 << 16) | (3 << 20))
#define SEED ((8 << 16) | (3 << 20))
#define DUMMY	(0xB << 14)

#define GENCMDDADDRL(adl, addr)	((adl) | ((addr) & 0xffff))
#define GENCMDDADDRH(adh, addr)	((adh) | (((addr) >> 16) & 0xffff))
#define GENCMDIADDRL(ail, addr)	((ail) | ((addr) & 0xffff))
#define GENCMDIADDRH(aih, addr)	((aih) | (((addr) >> 16) & 0xffff))
#define NFC_CMD_ADL	((0 << 16) | (3 << 20))
#define NFC_CMD_ADH	((1 << 16) | (3 << 20))
#define NFC_CMD_AIL	((2 << 16) | (3 << 20))
#define NFC_CMD_AIH	((3 << 16) | (3 << 20))

#define ECC_COMPLETE		BIT(31)
#define ECC_UNCORRECTABLE	0x3f
#define ECC_ERR_CNT(x)		(((x) >> 24) & 0x3f)
#define ECC_ZERO_CNT(x)		(((x) >> 16) & 0x3f)

#define CLK_ALWAYS_ON	BIT(28)
#define CLK_SELECT_NAND	BIT(31)
#define CLK_DEF_CORE_PHASE	(2 << 8)

#define CTRL_NAND_CLK_GATE_ON_BIT	BIT(26)
#define CTRL_SDEMMC_CLK_CTRL_ON_BIT	BIT(7)

#define GET_BCH_MODE(x)		(((x) >> 14) & 0x7)

#define ERROR_MODE(mod, num)	((uint32_t)((((mod) << 8) | (num))))
#define ERR_NFC_WRONG_PARAM	ERROR_MODE(3, 5)
#define ERR_NFC_TIMEOUT		ERROR_MODE(3, 6)
#define ERR_DMA_TIMEOUT		ERROR_MODE(3, 7)
#define ERROR_BLANK_PAGE	ERROR_MODE(3, 8)
#define ERROR_ECC		ERROR_MODE(3, 9)

extern uint64_t *info_buf;

#define ARRAY_SIZE_UL(array)	(sizeof(array) / sizeof(unsigned long))

#define NFC_STATUS_ON	1
#define NFC_STATUS_OFF	0

#define SPI_MODE	1
#define NFC_MODE	0

#define NFC_RAW_CHUNK_SHIFT	14

enum CMD_TYPE {		/* bit shift */
	TYPE_NONE	= 0,
	TYPE_CMD	= 1,
	TYPE_ADDR	= 2,
	TYPE_DATA_DRD	= 4,
	TYPE_DATA_DWR	= 8,
};

#define CMD_ADDR		(TYPE_CMD | TYPE_ADDR)
#define CMD_ADDR_DATA_RD	(TYPE_CMD | TYPE_ADDR | TYPE_DATA_DRD)
#define CMD_ADDR_DATA_WR	(TYPE_CMD | TYPE_ADDR | TYPE_DATA_DWR)

#define NFC_SEND_CMD(cmd)						\
	nfc_send_cmd_addr_and_wait(TYPE_CMD, (cmd),			\
				   NULL, 0, NULL, 0, 0)
#define NFC_SEND_CMD_CTRL(cmd, ctrl)				\
	nfc_send_cmd_addr_and_wait(TYPE_CMD, (cmd),			\
				   NULL, 0, NULL, 0, (ctrl))
#define NFC_SEND_CMD_ADDR(cmd, addr, addr_len)			\
	nfc_send_cmd_addr_and_wait(CMD_ADDR, (cmd),			\
				   (addr), (addr_len),			\
				   NULL, 0, 0)
#define NFC_SEND_CMD_ADDR_CTRL(cmd, addr, addr_len, ctrl)		\
	nfc_send_cmd_addr_and_wait(CMD_ADDR, (cmd),			\
				   (addr), (addr_len),			\
				   NULL, 0, (ctrl))
#define NFC_SEND_CMD_ADDR_DATA_RD(cmd, addr, addr_len, data, data_len)	\
	nfc_send_cmd_addr_and_wait(CMD_ADDR_DATA_RD,			\
				   (cmd), (addr), (addr_len),		\
				   (data), (data_len), 0)
#define NFC_SEND_CMD_ADDR_DATA_WR(cmd, addr, addr_len, data, data_len)	\
	nfc_send_cmd_addr_and_wait(CMD_ADDR_DATA_WR,			\
				   (cmd), (addr), (addr_len),		\
				   (data), (data_len), 0)
#define NFC_SEND_CMD_ADDR_DATA_WR_CTRL(cmd, addr, addr_len, data, data_len, ctrl)	\
	nfc_send_cmd_addr_and_wait(CMD_ADDR_DATA_WR,			\
				   (cmd), (addr), (addr_len),		\
				   (data), (data_len), (ctrl))

enum STATE_AND_DUMMY_CTL {
	DUMMY_BEFORE_ADDRESS = 0x01,
	DUMMY_AFTER_ADDRESS = 0x02,
	DONT_SEND_CEF = 0x04,
};

#define STATE_AND_DUMMY_CTL_BIT_SHIFT	8

/* timeout in unit us */
/* 10ms */
#define NFC_COMMAND_FIFIO_TIMEOUT	(10000)
/* when transferring data, considering the worse case:
 * 1bit and 6Mhz, all the 202KiB data would cost about
 * 6MByts/s / 8 = 0.75MBytes/s, then the 202KiB would
 * cost about 202/1024*0.75*1000 = 147.9ms.
 * Considering more redundancy, 1s would be a good
 * choice.
 */
#define NFC_DATA_FIFIO_TIMEOUT		(1000000)

enum NFC_CLK_RATE {
	CLK_6MHZ	= 0,
	CLK_12MHZ,
	CLK_20MHZ,
	CLK_41MHZ,
	CLK_83MHZ,
	CLK_100MHZ_400,
	CLK_100MHZ_500,
	CLK_125MHZ,
	CLK_133MHZ,
	CLK_166MHZ_333,
	CLK_166MHZ_666,
	CLK_200MHZ_400,
	CLK_200MHZ_800,
};

enum NFC_CLK_SRC_TYPE {
	OSC_CLK_24MHZ	= 0,
	FIX_PLL_DIV2,
	FIX_PLL_DIV3,
	HIFI_PLL,
	FIX_PLL_DIV2P5,
};

struct nfc_clk_provider {
	enum NFC_CLK_SRC_TYPE clk_src_type;
	enum NFC_CLK_RATE clk_rate;
	unsigned char div1;
	unsigned char div2;
	unsigned char adj;
	unsigned char cs_deselect_time;
};

enum NFC_STORAGE_TYPE {
	SPINAND_FLASH	= 0,
	SPINOR_FLASH	= 1,
	SLCNAND_FLASH	= 2,
};

#define N2M_RAW	(0x00220000)
#define M2N_RAW	(0x00200000)

#define CMD_BITMAP_TEST_AND_GET_BIT_VALUE(x, i)		\
	(__extension__ ({int tmp = 1 << (i); (((x) & tmp) ? tmp : 0); }))
#define CMD_BITMAP_CLEAR_BIT(x, i) ((x) &= ~(1 << (i)))

void nfc_pinmux_init(enum NFC_STORAGE_TYPE storage_type);
void nfc_send_idle(uint32_t cycle);
void nfc_select_mode(int spi_mode);
void nfc_deselect_chip(uint32_t cycle);
void nfc_raw_size_ext_convert(uint32_t size);
uint32_t nfc_send_cmd_addr_and_wait(unsigned char cmd_bitmap, uint8_t cmd,
				    uint8_t *addr, uint8_t addr_len,
				    uint8_t *data, uint8_t data_len,
				    unsigned short state_dummy_ctl);
void nfc_set_clock_and_timing(int init_stage, enum NFC_STORAGE_TYPE storage_type);
void nfc_set_dma_mem_and_info(uint32_t mem, unsigned long info);
int nfc_start_dma_and_wait_done(uint32_t n2m_cmd);
int nfc_wait_data_and_ecc_engine_done(uint64_t *info_buf,
				      unsigned char nsteps, int raw);
void nfc_send_repeat_rb_command(void);
void nfc_set_dma_random_seed(uint32_t page, uint32_t n2m_cmd);
unsigned int nfc_recalculate_n2m_command(uint32_t size, int no_cal);
int nfc_wait_command_fifo_done(uint32_t timeout, int repeat_rb);

void nfc_get_user_byte(u8 *oob_buf, uint64_t *info_buf, int ecc_steps);
void nfc_set_user_byte(u8 *oob_buf, uint64_t *info_buf, int ecc_steps);
void nfc_set_data_bus_width(int bus_width);

#endif
