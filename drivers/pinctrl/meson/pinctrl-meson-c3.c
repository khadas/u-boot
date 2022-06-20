// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <dt-bindings/gpio/meson-c3-gpio.h>

#include "pinctrl-meson.h"
#include "pinctrl-meson-axg-pmx.h"

/* BANK DSI func1 */
static const unsigned  int dsi_clkp_pins[]		= { DSI_CLKP };
static const unsigned  int dsi_clkn_pins[]		= { DSI_CLKN };
static const unsigned  int dsi_d0p_pins[]		= { DSI_D0P };
static const unsigned  int dsi_d0n_pins[]		= { DSI_D0N };
static const unsigned  int dsi_d1p_pins[]		= { DSI_D1P };
static const unsigned  int dsi_d1n_pins[]		= { DSI_D1N };
static const unsigned  int dsi_d2p_pins[]		= { DSI_D2P };
static const unsigned  int dsi_d2n_pins[]		= { DSI_D2N };
static const unsigned  int dsi_d3p_pins[]		= { DSI_D3P };
static const unsigned  int dsi_d3n_pins[]		= { DSI_D3N };

/* BANK DSI func2 */
static const unsigned int pdm_din0_dsi_pins[]		= { DSI_CLKP };
static const unsigned int pdm_dclk_dsi_pins[]		= { DSI_CLKN };
static const unsigned int pwm_i_dsi_pins[]		= { DSI_D0P };
static const unsigned int pwm_j_dsi_pins[]		= { DSI_D0N };
static const unsigned int uart_f_tx_dsi_pins[]		= { DSI_D1P };
static const unsigned int uart_f_rx_dsi_pins[]		= { DSI_D1N };
static const unsigned int i2c1_scl_dsi_pins[]		= { DSI_D2P };
static const unsigned int i2c1_sda_dsi_pins[]		= { DSI_D2N };
static const unsigned int uart_c_tx_dsi_pins[]		= { DSI_D3P };
static const unsigned int uart_c_rx_dsi_pins[]		= { DSI_D3N };

/* BANK SARADC func1 */
static const unsigned  int sar_ch0_pins[]		= { SAR_CH0 };
static const unsigned  int sar_ch1_pins[]		= { SAR_CH1 };
static const unsigned  int sar_ch2_pins[]		= { SAR_CH2 };
static const unsigned  int sar_ch3_pins[]		= { SAR_CH3 };

/* BANK SARADC func2 */
static const unsigned int gen_clk_sar_pins[]		= { SAR_CH1 };

static struct meson_pmx_group meson_c3_analog_groups[] = {
	GPIO_GROUP(DSI_CLKP),
	GPIO_GROUP(DSI_CLKN),
	GPIO_GROUP(DSI_D0P),
	GPIO_GROUP(DSI_D0N),
	GPIO_GROUP(DSI_D1P),
	GPIO_GROUP(DSI_D1N),
	GPIO_GROUP(DSI_D2P),
	GPIO_GROUP(DSI_D2N),
	GPIO_GROUP(DSI_D3P),
	GPIO_GROUP(DSI_D3N),
	GPIO_GROUP(SAR_CH0),
	GPIO_GROUP(SAR_CH1),
	GPIO_GROUP(SAR_CH2),
	GPIO_GROUP(SAR_CH3),

	/* BANK DSI func1 */
	GROUP(dsi_clkp,			1),
	GROUP(dsi_clkn,			1),
	GROUP(dsi_d0p,			1),
	GROUP(dsi_d0n,			1),
	GROUP(dsi_d1p,			1),
	GROUP(dsi_d1n,			1),
	GROUP(dsi_d2p,			1),
	GROUP(dsi_d2n,			1),
	GROUP(dsi_d3p,			1),
	GROUP(dsi_d3n,			1),

	/* BANK DSI func2 */
	GROUP(pdm_din0_dsi,		1),
	GROUP(pdm_dclk_dsi,		1),
	GROUP(pwm_i_dsi,		1),
	GROUP(pwm_j_dsi,		1),
	GROUP(uart_f_tx_dsi,		1),
	GROUP(uart_f_rx_dsi,		1),
	GROUP(i2c1_scl_dsi,		1),
	GROUP(i2c1_sda_dsi,		1),
	GROUP(uart_c_tx_dsi,		1),
	GROUP(uart_c_rx_dsi,		1),

	/* BANK SARADC func1 */
	GROUP(sar_ch0,			1),
	GROUP(sar_ch1,			1),
	GROUP(sar_ch2,			1),
	GROUP(sar_ch3,			1),

	/* BANK SARADC func2 */
	GROUP(gen_clk_sar,		1),
};

static const char * const gpio_analog_groups[] = {
	"DSI_CLKP", "DSI_CLKN", "DSI_D0P", "DSI_D0N", "DSI_D1P",
	"DSI_D1N", "DSI_D2P", "DSI_D2N", "DSI_D3P",
	"DSI_D3N",

	"SAR_CH0", "SAR_CH1", "SAR_CH2", "SAR_CH3"
};

static const char * const dsi_groups[] = {
	"dsi_clkp", "dsi_clkn", "dsi_d0p", "dsi_d0n", "dsi_d1p", "dsi_d1n",
	"dsi_d2p", "dsi_d2n", "dsi_d3p", "dsi_d3n"
};

static const char * const pdm_dsi_groups[] = {
	"pdm_din0_dsi", "pdm_dclk_dsi"
};

static const char * const pwm_i_dsi_groups[] = {
	"pwm_i_dsi"
};

static const char * const pwm_j_dsi_groups[] = {
	"pwm_j_dsi"
};

static const char * const uart_c_dsi_groups[] = {
	"uart_c_tx_dsi", "uart_c_rx_dsi"
};

static const char * const uart_f_dsi_groups[] = {
	"uart_f_tx_dsi", "uart_f_rx_dsi"
};

static const char * const i2c1_dsi_groups[] = {
	"i2c1_scl_dsi", "i2c1_sda_dsi"
};

static const char * const saradc_groups[] = {
	"sar_ch0", "sar_ch1", "sar_ch2", "sar_ch3"
};

static const char * const gen_clk_sar_groups[] = {
	"gen_clk_sar",
};

static struct meson_pmx_func meson_c3_analog_functions[] = {
	FUNCTION(gpio_analog),
	FUNCTION(dsi),
	FUNCTION(pdm_dsi),
	FUNCTION(pwm_i_dsi),
	FUNCTION(pwm_j_dsi),
	FUNCTION(uart_c_dsi),
	FUNCTION(uart_f_dsi),
	FUNCTION(i2c1_dsi),
	FUNCTION(saradc),
	FUNCTION(gen_clk_sar),
};

/* Bank E func1 */
static const unsigned int pwm_a_pins[]			= { GPIOE_0 };
static const unsigned int pwm_b_pins[]			= { GPIOE_1 };
static const unsigned int i2c2_sda_pins[]		= { GPIOE_2 };
static const unsigned int i2c2_scl_pins[]		= { GPIOE_3 };
static const unsigned int gen_clk_e_pins[]		= { GPIOE_4 };

/* Bank E func2 */
static const unsigned int i2c0_sda_e_pins[]		= { GPIOE_0 };
static const unsigned int i2c0_scl_e_pins[]		= { GPIOE_1 };
static const unsigned int clk_32k_in_pins[]		= { GPIOE_4 };

/* Bank E func3 */
static const unsigned int i2c_slave_scl_pins[]		= { GPIOE_0 };
static const unsigned int i2c_slave_sda_pins[]		= { GPIOE_1 };
static const unsigned int clk12_24_e_pins[]		= { GPIOE_4 };

/* Bank B func1 */
static const unsigned int emmc_nand_d0_pins[]		= { GPIOB_0 };
static const unsigned int emmc_nand_d1_pins[]		= { GPIOB_1 };
static const unsigned int emmc_nand_d2_pins[]		= { GPIOB_2 };
static const unsigned int emmc_nand_d3_pins[]		= { GPIOB_3 };
static const unsigned int emmc_nand_d4_pins[]		= { GPIOB_4 };
static const unsigned int emmc_nand_d5_pins[]		= { GPIOB_5 };
static const unsigned int emmc_nand_d6_pins[]		= { GPIOB_6 };
static const unsigned int emmc_nand_d7_pins[]		= { GPIOB_7 };
static const unsigned int emmc_clk_pins[]		= { GPIOB_8 };
static const unsigned int emmc_rst_pins[]		= { GPIOB_9 };
static const unsigned int emmc_cmd_pins[]		= { GPIOB_10 };
static const unsigned int emmc_nand_ds_pins[]		= { GPIOB_11 };

/* Bank B func2 */
static const unsigned int nand_wen_clk_pins[]		= { GPIOB_8 };
static const unsigned int nand_ale_pins[]		= { GPIOB_9 };
static const unsigned int nand_ren_wr_pins[]		= { GPIOB_10 };
static const unsigned int nand_cle_pins[]		= { GPIOB_11 };
static const unsigned int nand_ce0_pins[]		= { GPIOB_12 };

/* Bank B func3 */
static const unsigned int pwm_g_b_pins[]		= { GPIOB_0 };
static const unsigned int pwm_h_b_pins[]		= { GPIOB_1 };
static const unsigned int pwm_i_b_pins[]		= { GPIOB_2 };
static const unsigned int spif_hold_pins[]		= { GPIOB_3 };
static const unsigned int spif_mo_pins[]		= { GPIOB_4 };
static const unsigned int spif_mi_pins[]		= { GPIOB_5 };
static const unsigned int spif_clk_pins[]		= { GPIOB_6 };
static const unsigned int spif_wp_pins[]		= { GPIOB_7 };
static const unsigned int pwm_j_b_pins[]		= { GPIOB_8  };
static const unsigned int pwm_k_b_pins[]		= { GPIOB_9  };
static const unsigned int pwm_l_b_pins[]		= { GPIOB_10 };
static const unsigned int pwm_m_b_pins[]		= { GPIOB_11 };
static const unsigned int pwm_n_b_pins[]		= { GPIOB_12 };
static const unsigned int spif_cs_pins[]		= { GPIOB_13 };
static const unsigned int spif_clk_loop_pins[]		= { GPIOB_14 };

/* Bank B func4 */
static const unsigned int lcd_d0_pins[]			= { GPIOB_0 };
static const unsigned int lcd_d1_pins[]			= { GPIOB_1 };
static const unsigned int lcd_d2_pins[]			= { GPIOB_2 };
static const unsigned int lcd_d3_pins[]			= { GPIOB_8  };
static const unsigned int lcd_d4_pins[]			= { GPIOB_9  };
static const unsigned int lcd_d5_pins[]			= { GPIOB_10 };
static const unsigned int lcd_d6_pins[]			= { GPIOB_11 };
static const unsigned int lcd_d7_pins[]			= { GPIOB_12 };

/* Bank B func5 */
static const unsigned int spi_a_mosi_b_pins[]		= { GPIOB_0 };
static const unsigned int spi_a_miso_b_pins[]		= { GPIOB_1 };
static const unsigned int spi_a_clk_b_pins[]		= { GPIOB_2 };
static const unsigned int spi_a_ss0_b_pins[]		= { GPIOB_8 };
static const unsigned int spi_a_ss1_b_pins[]		= { GPIOB_9 };
static const unsigned int spi_a_ss2_b_pins[]		= { GPIOB_10 };
static const unsigned int i2c1_sda_b_pins[]		= { GPIOB_11 };
static const unsigned int i2c1_scl_b_pins[]		= { GPIOB_12 };

/* Bank B func6 */
static const unsigned int uart_a_tx_b_pins[]		= { GPIOB_0 };
static const unsigned int uart_a_rx_b_pins[]		= { GPIOB_1 };
static const unsigned int uart_a_cts_b_pins[]		= { GPIOB_2 };
static const unsigned int uart_a_rts_b_pins[]		= { GPIOB_8 };
static const unsigned int uart_d_tx_b_pins[]		= { GPIOB_9 };
static const unsigned int uart_d_rx_b_pins[]		= { GPIOB_10 };
static const unsigned int pdm_dclk_b_pins[]		= { GPIOB_11 };
static const unsigned int pdm_din0_b_pins[]		= { GPIOB_12 };

/* Bank C func1 */
static const unsigned int sdcard_d0_pins[]		= { GPIOC_0 };
static const unsigned int sdcard_d1_pins[]		= { GPIOC_1 };
static const unsigned int sdcard_d2_pins[]		= { GPIOC_2 };
static const unsigned int sdcard_d3_pins[]		= { GPIOC_3 };
static const unsigned int sdcard_clk_pins[]		= { GPIOC_4 };
static const unsigned int sdcard_cmd_pins[]		= { GPIOC_5 };
static const unsigned int sdcard_cd_pins[]		= { GPIOC_6 };

/* Bank C func2 */
static const unsigned int jtag_b_tdo_pins[]		= { GPIOC_0 };
static const unsigned int jtag_b_tdi_pins[]		= { GPIOC_1 };
static const unsigned int uart_b_rx_c_pins[]		= { GPIOC_2 };
static const unsigned int uart_b_tx_c_pins[]		= { GPIOC_3 };
static const unsigned int jtag_b_clk_pins[]		= { GPIOC_4 };
static const unsigned int jtag_b_tms_pins[]		= { GPIOC_5 };
static const unsigned int gen_clk_c_pins[]		= { GPIOC_6 };

/* Bank C func3 */
static const unsigned int tdm_d3_pins[]			= { GPIOC_0 };
static const unsigned int tdm_d2_pins[]			= { GPIOC_1 };
static const unsigned int mclk_1_pins[]			= { GPIOC_2 };
static const unsigned int tdm_sclk1_pins[]		= { GPIOC_3 };
static const unsigned int tdm_fs1_pins[]		= { GPIOC_4 };
static const unsigned int pdm_dclk_c_pins[]		= { GPIOC_5 };
static const unsigned int pdm_din0_c_pins[]		= { GPIOC_6 };

/* Bank C func4 */
static const unsigned int spi_a_mosi_c_pins[]		= { GPIOC_0 };
static const unsigned int spi_a_miso_c_pins[]		= { GPIOC_1 };
static const unsigned int spi_a_clk_c_pins[]		= { GPIOC_2 };
static const unsigned int spi_a_ss0_c_pins[]		= { GPIOC_3 };
static const unsigned int spi_a_ss1_c_pins[]		= { GPIOC_4 };

/* Bank C func5 */
static const unsigned int pwm_g_c_pins[]		= { GPIOC_0 };
static const unsigned int pwm_h_c_pins[]		= { GPIOC_1 };
static const unsigned int pwm_i_c_pins[]		= { GPIOC_2 };
static const unsigned int pwm_j_c_pins[]		= { GPIOC_3 };
static const unsigned int pwm_k_c_pins[]		= { GPIOC_4 };
static const unsigned int pwm_l_c_pins[]		= { GPIOC_5 };
static const unsigned int pwm_m_c_pins[]		= { GPIOC_6 };

/* Bank C func6 */
static const unsigned int uart_a_rx_c_pins[]		= { GPIOC_0 };
static const unsigned int uart_a_tx_c_pins[]		= { GPIOC_1 };
static const unsigned int uart_c_rx_c_pins[]		= { GPIOC_2 };
static const unsigned int uart_c_tx_c_pins[]		= { GPIOC_3 };
static const unsigned int i2c3_sda_c_pins[]		= { GPIOC_4 };
static const unsigned int i2c3_scl_c_pins[]		= { GPIOC_5 };
static const unsigned int clk12_24_c_pins[]		= { GPIOC_6 };

/* Bank X func1 */
static const unsigned int sdio_d0_pins[]		= { GPIOX_0 };
static const unsigned int sdio_d1_pins[]		= { GPIOX_1 };
static const unsigned int sdio_d2_pins[]		= { GPIOX_2 };
static const unsigned int sdio_d3_pins[]		= { GPIOX_3 };
static const unsigned int sdio_clk_pins[]		= { GPIOX_4 };
static const unsigned int sdio_cmd_pins[]		= { GPIOX_5 };
static const unsigned int clk12_24_x_pins[]		= { GPIOX_6 };
static const unsigned int uart_e_tx_x_pins[]		= { GPIOX_7 };
static const unsigned int uart_e_rx_x_pins[]		= { GPIOX_8 };
static const unsigned int uart_e_cts_pins[]		= { GPIOX_9 };
static const unsigned int uart_e_rts_pins[]		= { GPIOX_10 };
static const unsigned int pwm_e_pins[]			= { GPIOX_11 };
static const unsigned int pwm_j_x12_pins[]		= { GPIOX_12 };
static const unsigned int pwm_k_x13_pins[]		= { GPIOX_13 };

/* Bank X func2 */
static const unsigned int spi_a_mosi_x_pins[]		= { GPIOX_0 };
static const unsigned int spi_a_miso_x_pins[]		= { GPIOX_1 };
static const unsigned int spi_a_clk_x_pins[]		= { GPIOX_2 };
static const unsigned int spi_a_ss0_x_pins[]		= { GPIOX_3 };
static const unsigned int spi_a_ss1_x_pins[]		= { GPIOX_4 };
static const unsigned int spi_a_ss2_x_pins[]		= { GPIOX_5 };
static const unsigned int spi_b_ss2_x6_pins[]		= { GPIOX_6 };
static const unsigned int spi_b_miso_x_pins[]		= { GPIOX_7 };
static const unsigned int spi_b_clk_x_pins[]		= { GPIOX_8 };
static const unsigned int spi_b_mosi_x_pins[]		= { GPIOX_9 };
static const unsigned int spi_b_ss0_x_pins[]		= { GPIOX_10 };
static const unsigned int spi_b_ss1_x_pins[]		= { GPIOX_11 };
static const unsigned int spi_b_ss2_x12_pins[]		= { GPIOX_12 };
static const unsigned int gen_clk_x_pins[]		= { GPIOX_13 };

/* Bank X func3 */
static const unsigned int tdm_d1_x_pins[]		= { GPIOX_0 };
static const unsigned int tdm_d0_x_pins[]		= { GPIOX_1 };
static const unsigned int mclk_0_x_pins[]		= { GPIOX_2 };
static const unsigned int tdm_sclk0_x_pins[]		= { GPIOX_3 };
static const unsigned int tdm_fs0_x_pins[]		= { GPIOX_4 };
static const unsigned int pdm_dclk_x5_pins[]		= { GPIOX_5 };
static const unsigned int pdm_din0_x6_pins[]		= { GPIOX_6 };
static const unsigned int pdm_din0_x9_pins[]		= { GPIOX_9 };
static const unsigned int pdm_dclk_x10_pins[]		= { GPIOX_10 };
static const unsigned int clk12_24_x13_pins[]		= { GPIOX_13 };

/* Bank X func4 */
static const unsigned int lcd_d8_pins[]			= { GPIOX_0 };
static const unsigned int lcd_d9_pins[]			= { GPIOX_1 };
static const unsigned int lcd_d10_pins[]		= { GPIOX_2 };
static const unsigned int lcd_d11_pins[]		= { GPIOX_3 };
static const unsigned int lcd_d12_pins[]		= { GPIOX_4 };
static const unsigned int lcd_d13_pins[]		= { GPIOX_5 };
static const unsigned int lcd_d14_pins[]		= { GPIOX_6 };
static const unsigned int lcd_d15_pins[]		= { GPIOX_7 };
static const unsigned int lcd_vs_pins[]			= { GPIOX_8 };
static const unsigned int lcd_hs_pins[]			= { GPIOX_9 };
static const unsigned int lcd_den_pins[]		= { GPIOX_10 };
static const unsigned int lcd_d16_pins[]		= { GPIOX_11 };
static const unsigned int lcd_clk_x_pins[]		= { GPIOX_12 };
static const unsigned int lcd_d17_pins[]		= { GPIOX_13 };

/* Bank X func5 */
static const unsigned int pwm_g_x0_pins[]		= { GPIOX_0 };
static const unsigned int pwm_h_x1_pins[]		= { GPIOX_1 };
static const unsigned int pwm_i_x2_pins[]		= { GPIOX_2 };
static const unsigned int pwm_j_x3_pins[]		= { GPIOX_3 };
static const unsigned int pwm_k_x4_pins[]		= { GPIOX_4 };
static const unsigned int pwm_l_x_pins[]		= { GPIOX_5 };
static const unsigned int pwm_m_x_pins[]		= { GPIOX_6 };
static const unsigned int pwm_n_x_pins[]		= { GPIOX_7 };
static const unsigned int pwm_g_x8_pins[]		= { GPIOX_8 };
static const unsigned int pwm_h_x9_pins[]		= { GPIOX_9 };
static const unsigned int pwm_i_x10_pins[]		= { GPIOX_10 };
static const unsigned int clk12_24_x11_pins[]		= { GPIOX_11 };

/* Bank X func6 */
static const unsigned int uart_a_rx_x_pins[]		= { GPIOX_0 };
static const unsigned int uart_a_tx_x_pins[]		= { GPIOX_1 };
static const unsigned int uart_c_rx_x_pins[]		= { GPIOX_2 };
static const unsigned int uart_c_tx_x_pins[]		= { GPIOX_3 };
static const unsigned int i2c3_sda_x_pins[]		= { GPIOX_4 };
static const unsigned int i2c3_scl_x_pins[]		= { GPIOX_5 };
static const unsigned int i2c1_sda_x_pins[]		= { GPIOX_7 };
static const unsigned int i2c1_scl_x_pins[]		= { GPIOX_8 };
static const unsigned int uart_d_tx_x_pins[]		= { GPIOX_9 };
static const unsigned int uart_d_rx_x_pins[]		= { GPIOX_10 };

/* Bank D func1 */
static const unsigned int pwm_g_d_pins[]		= { GPIOD_0 };
static const unsigned int pwm_h_d_pins[]		= { GPIOD_1 };
static const unsigned int eth_led_act_pins[]		= { GPIOD_2 };
static const unsigned int eth_led_link_pins[]		= { GPIOD_3 };
static const unsigned int pwm_d_pins[]			= { GPIOD_4 };
static const unsigned int pwm_f_pins[]			= { GPIOD_5 };
static const unsigned int pwm_k_d_pins[]		= { GPIOD_6 };

/* Bank D func2 */
static const unsigned int uart_a_tx_d_pins[]		= { GPIOD_0 };
static const unsigned int uart_a_rx_d_pins[]		= { GPIOD_1 };
static const unsigned int spi_b_miso_d_pins[]		= { GPIOD_2 };
static const unsigned int spi_b_clk_d_pins[]		= { GPIOD_3 };
static const unsigned int spi_b_mosi_d_pins[]		= { GPIOD_4 };
static const unsigned int spi_b_ss0_d_pins[]		= { GPIOD_5 };
static const unsigned int spi_b_ss1_d_pins[]		= { GPIOD_6 };

/* Bank D func3 */
static const unsigned int i2c0_sda_d_pins[]		= { GPIOD_0 };
static const unsigned int i2c0_scl_d_pins[]		= { GPIOD_1 };
static const unsigned int i2c1_sda_d_pins[]		= { GPIOD_2 };
static const unsigned int i2c1_scl_d_pins[]		= { GPIOD_3 };
static const unsigned int pdm_dclk_d_pins[]		= { GPIOD_4 };
static const unsigned int pdm_din0_d_pins[]		= { GPIOD_5 };
static const unsigned int ir_in_d6_pins[]		= { GPIOD_6 };

/* Bank D func4 */
static const unsigned int ir_in_d0_pins[]		= { GPIOD_0 };
static const unsigned int ir_out_pins[]			= { GPIOD_1 };
static const unsigned int pwm_i_d_pins[]		= { GPIOD_2 };
static const unsigned int pwm_j_d_pins[]		= { GPIOD_3 };
static const unsigned int i2c3_sda_d_pins[]		= { GPIOD_4 };
static const unsigned int i2c3_scl_d_pins[]		= { GPIOD_5 };

/* Bank D func5 */
static const unsigned int tdm_fs0_d_pins[]		= { GPIOD_2 };
static const unsigned int tdm_sclk0_d_pins[]		= { GPIOD_3 };
static const unsigned int mclk_0_d_pins[]		= { GPIOD_4 };
static const unsigned int tdm_d1_d_pins[]		= { GPIOD_5 };
static const unsigned int tdm_d0_d_pins[]		= { GPIOD_6 };

/* Bank D func6 */
static const unsigned int uart_d_tx_d_pins[]		= { GPIOD_0 };
static const unsigned int uart_d_rx_d_pins[]		= { GPIOD_1 };
static const unsigned int uart_c_tx_d_pins[]		= { GPIOD_2 };
static const unsigned int uart_c_rx_d_pins[]		= { GPIOD_3 };

/* Bank A func1 */
static const unsigned int uart_b_tx_a_pins[]		= { GPIOA_0 };
static const unsigned int uart_b_rx_a_pins[]		= { GPIOA_1 };
static const unsigned int pwm_c_pins[]			= { GPIOA_2 };
static const unsigned int pwm_l_a_pins[]			= { GPIOA_3 };
static const unsigned int i2c1_sda_a_pins[]		= { GPIOA_4 };
static const unsigned int i2c1_scl_a_pins[]		= { GPIOA_5 };

/* Bank A func2 */
static const unsigned int pwm_c_hiz_pins[]		= { GPIOA_2 };
static const unsigned int gen_clk_a_pins[]		= { GPIOA_3 };
static const unsigned int pdm_dclk_z_pins[]		= { GPIOA_4 };
static const unsigned int pdm_din0_a_pins[]		= { GPIOA_5 };

/* Bank A func3 */
static const unsigned int jtag_a_clk_pins[]		= { GPIOA_2 };
static const unsigned int jtag_a_tms_pins[]		= { GPIOA_3 };
static const unsigned int jtag_a_tdi_pins[]		= { GPIOA_4 };
static const unsigned int jtag_a_tdo_pins[]		= { GPIOA_5 };

/* Bank A func4 */
static const unsigned int lcd_clk_a_pins[]		= { GPIOA_3 };
static const unsigned int uart_f_tx_a_pins[]		= { GPIOA_4 };
static const unsigned int uart_f_rx_a_pins[]		= { GPIOA_5 };

/* Bank A func5 */
static const unsigned int uart_e_tx_a_pins[]		= { GPIOA_2 };
static const unsigned int uart_e_rx_a_pins[]		= { GPIOA_3 };
static const unsigned int pwm_m_a_pins[]		= { GPIOA_4 };
static const unsigned int pwm_n_a_pins[]		= { GPIOA_5 };

/* Bank A func6 */
static const unsigned int spi_a_mosi_a_pins[]		= { GPIOA_3 };
static const unsigned int gen_clk_a4_pins[]		= { GPIOA_4 };
static const unsigned int clk12_24_a_pins[]		= { GPIOA_5 };

static struct meson_pmx_group meson_c3_periphs_groups[] = {
	GPIO_GROUP(GPIOE_0),
	GPIO_GROUP(GPIOE_1),
	GPIO_GROUP(GPIOE_2),
	GPIO_GROUP(GPIOE_3),
	GPIO_GROUP(GPIOE_4),
	GPIO_GROUP(GPIOB_1),
	GPIO_GROUP(GPIOB_2),
	GPIO_GROUP(GPIOB_3),
	GPIO_GROUP(GPIOB_4),
	GPIO_GROUP(GPIOB_5),
	GPIO_GROUP(GPIOB_6),
	GPIO_GROUP(GPIOB_7),
	GPIO_GROUP(GPIOB_8),
	GPIO_GROUP(GPIOB_9),
	GPIO_GROUP(GPIOB_10),
	GPIO_GROUP(GPIOB_11),
	GPIO_GROUP(GPIOB_12),
	GPIO_GROUP(GPIOB_13),
	GPIO_GROUP(GPIOB_14),
	GPIO_GROUP(GPIOC_0),
	GPIO_GROUP(GPIOC_1),
	GPIO_GROUP(GPIOC_2),
	GPIO_GROUP(GPIOC_3),
	GPIO_GROUP(GPIOC_4),
	GPIO_GROUP(GPIOC_5),
	GPIO_GROUP(GPIOC_6),
	GPIO_GROUP(GPIOX_0),
	GPIO_GROUP(GPIOX_1),
	GPIO_GROUP(GPIOX_2),
	GPIO_GROUP(GPIOX_3),
	GPIO_GROUP(GPIOX_4),
	GPIO_GROUP(GPIOX_5),
	GPIO_GROUP(GPIOX_6),
	GPIO_GROUP(GPIOX_7),
	GPIO_GROUP(GPIOX_8),
	GPIO_GROUP(GPIOX_9),
	GPIO_GROUP(GPIOX_10),
	GPIO_GROUP(GPIOX_11),
	GPIO_GROUP(GPIOX_12),
	GPIO_GROUP(GPIOX_13),
	GPIO_GROUP(GPIOD_0),
	GPIO_GROUP(GPIOD_1),
	GPIO_GROUP(GPIOD_2),
	GPIO_GROUP(GPIOD_3),
	GPIO_GROUP(GPIOD_4),
	GPIO_GROUP(GPIOD_5),
	GPIO_GROUP(GPIOD_6),
	GPIO_GROUP(GPIOA_0),
	GPIO_GROUP(GPIOA_1),
	GPIO_GROUP(GPIOA_2),
	GPIO_GROUP(GPIOA_3),
	GPIO_GROUP(GPIOA_4),
	GPIO_GROUP(GPIOA_5),
	GPIO_GROUP(GPIO_TEST_N),

	/* Bank E func1 */
	GROUP(pwm_a,			1),
	GROUP(pwm_b,			1),
	GROUP(i2c2_sda,			1),
	GROUP(i2c2_scl,			1),
	GROUP(gen_clk_e,		1),

	/* Bank E func2 */
	GROUP(i2c0_sda_e,		2),
	GROUP(i2c0_scl_e,		2),
	GROUP(clk_32k_in,		2),

	/* Bank E func3 */
	GROUP(i2c_slave_scl,		3),
	GROUP(i2c_slave_sda,		3),
	GROUP(clk12_24_e,		3),

	/* Bank B func1 */
	GROUP(emmc_nand_d0,		1),
	GROUP(emmc_nand_d1,		1),
	GROUP(emmc_nand_d2,		1),
	GROUP(emmc_nand_d3,		1),
	GROUP(emmc_nand_d4,		1),
	GROUP(emmc_nand_d5,		1),
	GROUP(emmc_nand_d6,		1),
	GROUP(emmc_nand_d7,		1),
	GROUP(emmc_clk,			1),
	GROUP(emmc_rst,			1),
	GROUP(emmc_cmd,			1),
	GROUP(emmc_nand_ds,		1),

	/* Bank B func2 */
	GROUP(nand_wen_clk,		2),
	GROUP(nand_ale,			2),
	GROUP(nand_ren_wr,		2),
	GROUP(nand_cle,			2),
	GROUP(nand_ce0,			2),

	/* Bank B func3 */
	GROUP(pwm_g_b,			3),
	GROUP(pwm_h_b,			3),
	GROUP(pwm_i_b,			3),
	GROUP(spif_hold,		3),
	GROUP(spif_mo,			3),
	GROUP(spif_mi,			3),
	GROUP(spif_clk,			3),
	GROUP(spif_wp,			3),
	GROUP(pwm_j_b,			3),
	GROUP(pwm_k_b,			3),
	GROUP(pwm_l_b,			3),
	GROUP(pwm_m_b,			3),
	GROUP(pwm_n_b,			3),
	GROUP(spif_cs,			3),
	GROUP(spif_clk_loop,		3),

	/* Bank B func4 */
	GROUP(lcd_d0,			4),
	GROUP(lcd_d1,			4),
	GROUP(lcd_d2,			4),
	GROUP(lcd_d3,			4),
	GROUP(lcd_d4,			4),
	GROUP(lcd_d5,			4),
	GROUP(lcd_d6,			4),
	GROUP(lcd_d7,			4),

	/* Bank B func5 */
	GROUP(spi_a_mosi_b,		5),
	GROUP(spi_a_miso_b,		5),
	GROUP(spi_a_clk_b,		5),
	GROUP(spi_a_ss0_b,		5),
	GROUP(spi_a_ss1_b,		5),
	GROUP(spi_a_ss2_b,		5),
	GROUP(i2c1_sda_b,		5),
	GROUP(i2c1_scl_b,		5),

	/* Bank B func6 */
	GROUP(uart_a_tx_b,		6),
	GROUP(uart_a_rx_b,		6),
	GROUP(uart_a_cts_b,		6),
	GROUP(uart_a_rts_b,		6),
	GROUP(uart_d_tx_b,		6),
	GROUP(uart_d_rx_b,		6),
	GROUP(pdm_dclk_b,		6),
	GROUP(pdm_din0_b,		6),

	/* Bank C func1 */
	GROUP(sdcard_d0,		1),
	GROUP(sdcard_d1,		1),
	GROUP(sdcard_d2,		1),
	GROUP(sdcard_d3,		1),
	GROUP(sdcard_clk,		1),
	GROUP(sdcard_cmd,		1),
	GROUP(sdcard_cd,		1),

	/* Bank C func2 */
	GROUP(jtag_b_tdo,		2),
	GROUP(jtag_b_tdi,		2),
	GROUP(uart_b_rx_c,		2),
	GROUP(uart_b_tx_c,		2),
	GROUP(jtag_b_clk,		2),
	GROUP(jtag_b_tms,		2),
	GROUP(gen_clk_c,		2),

	/* Bank C func3 */
	GROUP(tdm_d3,			3),
	GROUP(tdm_d2,			3),
	GROUP(mclk_1,			3),
	GROUP(tdm_sclk1,		3),
	GROUP(tdm_fs1,			3),
	GROUP(pdm_dclk_c,		3),
	GROUP(pdm_din0_c,		3),

	/* Bank C func4 */
	GROUP(spi_a_mosi_c,		4),
	GROUP(spi_a_miso_c,		4),
	GROUP(spi_a_clk_c,		4),
	GROUP(spi_a_ss0_c,		4),
	GROUP(spi_a_ss1_c,		4),

	/* Bank C func5 */
	GROUP(pwm_g_c,			5),
	GROUP(pwm_h_c,			5),
	GROUP(pwm_i_c,			5),
	GROUP(pwm_j_c,			5),
	GROUP(pwm_k_c,			5),
	GROUP(pwm_l_c,			5),
	GROUP(pwm_m_c,			5),

	/* Bank C func6 */
	GROUP(uart_a_rx_c,		6),
	GROUP(uart_a_tx_c,		6),
	GROUP(uart_c_rx_c,		6),
	GROUP(uart_c_tx_c,		6),
	GROUP(i2c3_sda_c,		6),
	GROUP(i2c3_scl_c,		6),
	GROUP(clk12_24_c,		6),

	/* Bank X func1 */
	GROUP(sdio_d0,			1),
	GROUP(sdio_d1,			1),
	GROUP(sdio_d2,			1),
	GROUP(sdio_d3,			1),
	GROUP(sdio_clk,			1),
	GROUP(sdio_cmd,			1),
	GROUP(clk12_24_x,		1),
	GROUP(uart_e_tx_x,		1),
	GROUP(uart_e_rx_x,		1),
	GROUP(uart_e_cts,		1),
	GROUP(uart_e_rts,		1),
	GROUP(pwm_e,			1),
	GROUP(pwm_j_x12,		1),
	GROUP(pwm_k_x13,		1),

	/* Bank X func2 */
	GROUP(spi_a_mosi_x,		2),
	GROUP(spi_a_miso_x,		2),
	GROUP(spi_a_clk_x,		2),
	GROUP(spi_a_ss0_x,		2),
	GROUP(spi_a_ss1_x,		2),
	GROUP(spi_a_ss2_x,		2),
	GROUP(spi_b_ss2_x6,		2),
	GROUP(spi_b_miso_x,		2),
	GROUP(spi_b_clk_x,		2),
	GROUP(spi_b_mosi_x,		2),
	GROUP(spi_b_ss0_x,		2),
	GROUP(spi_b_ss1_x,		2),
	GROUP(spi_b_ss2_x12,		2),
	GROUP(gen_clk_x,		2),

	/* Bank X func3 */
	GROUP(tdm_d1_x,			3),
	GROUP(tdm_d0_x,			3),
	GROUP(mclk_0_x,			3),
	GROUP(tdm_sclk0_x,		3),
	GROUP(tdm_fs0_x,		3),
	GROUP(pdm_dclk_x5,		3),
	GROUP(pdm_din0_x6,		3),
	GROUP(pdm_din0_x9,		3),
	GROUP(pdm_dclk_x10,		3),
	GROUP(clk12_24_x13,		3),

	/* Bank X func4 */
	GROUP(lcd_d8,			4),
	GROUP(lcd_d9,			4),
	GROUP(lcd_d10,			4),
	GROUP(lcd_d11,			4),
	GROUP(lcd_d12,			4),
	GROUP(lcd_d13,			4),
	GROUP(lcd_d14,			4),
	GROUP(lcd_d15,			4),
	GROUP(lcd_vs,			4),
	GROUP(lcd_hs,			4),
	GROUP(lcd_den,			4),
	GROUP(lcd_d16,			4),
	GROUP(lcd_clk_x,			4),
	GROUP(lcd_d17,			4),

	/* Bank X func5 */
	GROUP(pwm_g_x0,			5),
	GROUP(pwm_h_x1,			5),
	GROUP(pwm_i_x2,			5),
	GROUP(pwm_j_x3,			5),
	GROUP(pwm_k_x4,			5),
	GROUP(pwm_l_x,			5),
	GROUP(pwm_m_x,			5),
	GROUP(pwm_n_x,			5),
	GROUP(pwm_g_x8,			5),
	GROUP(pwm_h_x9,			5),
	GROUP(pwm_i_x10,		5),
	GROUP(clk12_24_x11,		5),

	/* Bank X func6 */
	GROUP(uart_a_rx_x,		6),
	GROUP(uart_a_tx_x,		6),
	GROUP(uart_c_rx_x,		6),
	GROUP(uart_c_tx_x,		6),
	GROUP(i2c3_sda_x,		6),
	GROUP(i2c3_scl_x,		6),
	GROUP(i2c1_sda_x,		6),
	GROUP(i2c1_scl_x,		6),
	GROUP(uart_d_tx_x,		6),
	GROUP(uart_d_rx_x,		6),

	/* Bank D func1 */
	GROUP(pwm_g_d,			1),
	GROUP(pwm_h_d,			1),
	GROUP(eth_led_act,		1),
	GROUP(eth_led_link,		1),
	GROUP(pwm_d,			1),
	GROUP(pwm_f,			1),
	GROUP(pwm_k_d,			1),

	/* Bank D func2 */
	GROUP(uart_a_tx_d,		2),
	GROUP(uart_a_rx_d,		2),
	GROUP(spi_b_miso_d,		2),
	GROUP(spi_b_clk_d,		2),
	GROUP(spi_b_mosi_d,		2),
	GROUP(spi_b_ss0_d,		2),
	GROUP(spi_b_ss1_d,		2),

	/* Bank D func3 */
	GROUP(i2c0_sda_d,		3),
	GROUP(i2c0_scl_d,		3),
	GROUP(i2c1_sda_d,		3),
	GROUP(i2c1_scl_d,		3),
	GROUP(pdm_dclk_d,		3),
	GROUP(pdm_din0_d,		3),
	GROUP(ir_in_d6,			3),

	/* Bank D func4 */
	GROUP(ir_in_d0,			4),
	GROUP(ir_out,			4),
	GROUP(pwm_i_d,			4),
	GROUP(pwm_j_d,			4),
	GROUP(i2c3_sda_d,		4),
	GROUP(i2c3_scl_d,		4),

	/* Bank D func5 */
	GROUP(tdm_fs0_d,		5),
	GROUP(tdm_sclk0_d,		5),
	GROUP(mclk_0_d,			5),
	GROUP(tdm_d1_d,			5),
	GROUP(tdm_d0_d,			5),

	/* Bank D func6 */
	GROUP(uart_d_tx_d,		6),
	GROUP(uart_d_rx_d,		6),
	GROUP(uart_c_tx_d,		6),
	GROUP(uart_c_rx_d,		6),

	/* Bank A func1 */
	GROUP(uart_b_tx_a,		1),
	GROUP(uart_b_rx_a,		1),
	GROUP(pwm_c,			1),
	GROUP(pwm_l_a,			1),
	GROUP(i2c1_sda_a,		1),
	GROUP(i2c1_scl_a,		1),

	/* Bank A func2 */
	GROUP(pwm_c_hiz,		2),
	GROUP(gen_clk_a,		2),
	GROUP(pdm_dclk_z,		2),
	GROUP(pdm_din0_a,		2),

	/* Bank A func3 */
	GROUP(jtag_a_clk,		3),
	GROUP(jtag_a_tms,		3),
	GROUP(jtag_a_tdi,		3),
	GROUP(jtag_a_tdo,		3),

	/* Bank A func4 */
	GROUP(lcd_clk_a,		4),
	GROUP(uart_f_tx_a,		4),
	GROUP(uart_f_rx_a,		4),

	/* Bank A func5 */
	GROUP(uart_e_tx_a,		5),
	GROUP(uart_e_rx_a,		5),
	GROUP(pwm_m_a,			5),
	GROUP(pwm_n_a,			5),

	/* Bank A func6 */
	GROUP(spi_a_mosi_a,		6),
	GROUP(gen_clk_a4,		6),
	GROUP(clk12_24_a,		6)
};

static const char * const gpio_periphs_groups[] = {
	"GPIO_TEST_N",

	"GPIOE_0", "GPIOE_1", "GPIOE_2", "GPIOE_3", "GPIOE_4",

	"GPIOB_0", "GPIOB_1", "GPIOB_2", "GPIOB_3", "GPIOB_4",
	"GPIOB_5", "GPIOB_6", "GPIOB_7", "GPIOB_8", "GPIOB_9",
	"GPIOB_10", "GPIOB_11", "GPIOB_12", "GPIOB_13",
	"GPIOB_14",

	"GPIOC_0", "GPIOC_1", "GPIOC_2", "GPIOC_3", "GPIOC_4",
	"GPIOC_5", "GPIOC_6",

	"GPIOX_0", "GPIOX_1", "GPIOX_2", "GPIOX_3", "GPIOX_4",
	"GPIOX_5", "GPIOX_6", "GPIOX_7", "GPIOX_8", "GPIOX_9",
	"GPIOX_10", "GPIOX_11", "GPIOX_12", "GPIOX_13",

	"GPIOD_0", "GPIOD_1", "GPIOD_2", "GPIOD_3", "GPIOD_4",
	"GPIOD_5", "GPIOD_6",

	"GPIOA_0", "GPIOA_1", "GPIOA_2", "GPIOA_3", "GPIOA_4",
	"GPIOA_5",
};

static const char * const uart_a_groups[] = {
	"uart_a_tx_b", "uart_a_rx_b", "uart_a_cts_b", "uart_a_rts_b",
	"uart_a_rx_c", "uart_a_tx_c", "uart_a_rx_x", "uart_a_tx_x",
	"uart_a_tx_d", "uart_a_rx_d",
};

static const char * const uart_b_groups[] = {
	"uart_b_rx_c", "uart_b_tx_c", "uart_b_tx_a", "uart_b_rx_a",
};

static const char * const uart_c_groups[] = {
	"uart_c_rx_c", "uart_c_tx_c",
	"uart_c_rx_x", "uart_c_tx_x",
	"uart_c_tx_d", "uart_c_rx_d",
};

static const char * const uart_d_groups[] = {
	"uart_d_tx_b", "uart_d_rx_b", "uart_d_tx_d", "uart_d_rx_d",
	"uart_d_rx_x", "uart_d_tx_x",
};

static const char * const uart_e_groups[] = {
	"uart_e_cts", "uart_e_tx_x", "uart_e_rx_x", "uart_e_rts",
	"uart_e_tx_a", "uart_e_rx_a",
};

static const char * const i2c0_groups[] = {
	"i2c0_sda_e", "i2c0_scl_e",
	"i2c0_sda_d", "i2c0_scl_d",
};

static const char * const i2c1_groups[] = {
	"i2c1_sda_x", "i2c1_scl_x",
	"i2c1_sda_d", "i2c1_scl_d",
	"i2c1_sda_a", "i2c1_scl_a",
	"i2c1_sda_b", "i2c1_scl_b",
};

static const char * const i2c2_groups[] = {
	"i2c2_sda", "i2c2_scl"
};

static const char * const i2c3_groups[] = {
	"i2c3_sda_c", "i2c3_scl_c",
	"i2c3_sda_x", "i2c3_scl_x",
	"i2c3_sda_d", "i2c3_scl_d"
};

static const char * const i2c_slave_groups[] = {
	"i2c_slave_scl", "i2c_slave_sda",
};

static const char * const pwm_a_groups[] = {
	"pwm_a",
};

static const char * const pwm_b_groups[] = {
	"pwm_b",
};

static const char * const pwm_c_groups[] = {
	"pwm_c",
};

static const char * const pwm_d_groups[] = {
	"pwm_d",
};

static const char * const pwm_e_groups[] = {
	"pwm_e",
};

static const char * const pwm_f_groups[] = {
	"pwm_f",
};

static const char * const pwm_g_groups[] = {
	"pwm_g_b", "pwm_g_c", "pwm_g_d", "pwm_g_x0", "pwm_g_x8",
};

static const char * const pwm_h_groups[] = {
	"pwm_h_b", "pwm_h_c", "pwm_h_d", "pwm_h_x1", "pwm_h_x9",
};

static const char * const pwm_i_groups[] = {
	"pwm_i_b", "pwm_i_c", "pwm_i_d", "pwm_i_x2", "pwm_i_x10",
};

static const char * const pwm_j_groups[] = {
	"pwm_j_c", "pwm_j_d", "pwm_j_b", "pwm_j_x3", "pwm_j_x12",
};

static const char * const pwm_k_groups[] = {
	"pwm_k_c", "pwm_k_d", "pwm_k_b", "pwm_k_x4", "pwm_k_x13",
};

static const char * const pwm_l_groups[] = {
	"pwm_l_c", "pwm_l_x", "pwm_l_b", "pwm_l_a",
};

static const char * const pwm_m_groups[] = {
	"pwm_m_c", "pwm_m_x", "pwm_m_a", "pwm_m_b",
};

static const char * const pwm_n_groups[] = {
	"pwm_n_x", "pwm_n_a", "pwm_n_b",
};

static const char * const pwm_c_hiz_groups[] = {
	"pwm_c_hiz",
};

static const char * const ir_out_groups[] = {
	"ir_out",
};

static const char * const ir_in_groups[] = {
	"ir_in_d0", "ir_in_d6",
};

static const char * const jtag_a_groups[] = {
	"jtag_a_clk", "jtag_a_tms", "jtag_a_tdi", "jtag_a_tdo",
};

static const char * const jtag_b_groups[] = {
	"jtag_b_tdo", "jtag_b_tdi", "jtag_b_clk", "jtag_b_tms",
};

static const char * const gen_clk_groups[] = {
	"gen_clk_e", "gen_clk_c", "gen_clk_a", "gen_clk_x",
	"gen_clk_a4",
};

static const char * const clk12_24_groups[] = {
	"clk12_24_e", "clk12_24_c", "clk12_24_x", "clk12_24_a",
	"clk12_24_x13", "clk12_24_x11",
};

static const char * const clk_32k_in_groups[] = {
	"clk_32k_in",
};

static const char * const emmc_groups[] = {
	"emmc_nand_d0", "emmc_nand_d1", "emmc_nand_d2", "emmc_nand_d3",
	"emmc_nand_d4", "emmc_nand_d5", "emmc_nand_d6", "emmc_nand_d7",
	"emmc_clk", "emmc_rst", "emmc_cmd", "emmc_nand_ds",
};

static const char * const nand_groups[] = {
	"emmc_nand_d0", "emmc_nand_d1", "emmc_nand_d2", "emmc_nand_d3",
	"emmc_nand_d4", "emmc_nand_d5", "emmc_nand_d6", "emmc_nand_d7",
	"emmc_clk", "emmc_rst", "emmc_cmd", "emmc_nand_ds",
	"nand_wen_clk", "nand_ale", "nand_ren_wr", "nand_cle",
	"nand_ce0",
};

static const char * const spif_groups[] = {
	"spif_mo", "spif_mi", "spif_wp", "spif_cs",
	"spif_clk", "spif_hold", "spif_clk_loop",
};

static const char * const spi_a_groups[] = {
	"spi_a_clk_b", "spi_a_ss0_b", "spi_a_ss1_b", "spi_a_ss2_b",
	"spi_a_mosi_b", "spi_a_miso_b",

	"spi_a_clk_c", "spi_a_ss0_c", "spi_a_ss1_c",
	"spi_a_mosi_c", "spi_a_miso_c",

	"spi_a_clk_x", "spi_a_ss0_x", "spi_a_ss1_x", "spi_a_ss2_x",
	"spi_a_mosi_x", "spi_a_miso_x",
	"spi_a_mosi_a",
};

static const char * const spi_b_groups[] = {
	"spi_b_clk_x", "spi_b_ss0_x", "spi_b_ss1_x", "spi_b_ss2_x6",
	"spi_b_miso_x", "spi_b_mosi_x", "spi_b_ss2_x12",

	"spi_b_clk_d", "spi_b_ss0_d", "spi_b_ss1_d", "spi_b_miso_d",
	"spi_b_mosi_d",
};

static const char * const sdcard_groups[] = {
	"sdcard_d0", "sdcard_d1", "sdcard_d2", "sdcard_d3",
	"sdcard_cd", "sdcard_clk", "sdcard_cmd",
};

static const char * const sdio_groups[] = {
	"sdio_d0", "sdio_d1", "sdio_d2", "sdio_d3",
	"sdio_clk", "sdio_cmd",
};

static const char * const pdm_groups[] = {
	"pdm_dclk_c", "pdm_din0_c", "pdm_dclk_d", "pdm_din0_d",
	"pdm_dclk_z", "pdm_din0_a", "pdm_dclk_b", "pdm_din0_b",
	"pdm_dclk_x5", "pdm_din0_x6", "pdm_din0_x9", "pdm_dclk_x10",
};

static const char * const eth_groups[] = {
	"eth_led_act", "eth_led_link",
};

static const char * const mclk_0_groups[] = {
	"mclk_0_x", "mclk_0_d",
};

static const char * const mclk_1_groups[] = {
	"mclk_1",
};

static const char * const tdm_groups[] = {
	"tdm_d3", "tdm_d2", "tdm_fs1", "tdm_d1_x", "tdm_d0_x",
	"tdm_d1_d", "tdm_d0_d", "tdm_sclk1", "tdm_fs0_x", "tdm_fs0_d",
	"tdm_sclk0_x", "tdm_sclk0_d",
};

static const char * const lcd_groups[] = {
	"lcd_d0", "lcd_d1", "lcd_d2", "lcd_d3", "lcd_d4",
	"lcd_d5", "lcd_d6", "lcd_d7", "lcd_d8", "lcd_d9",
	"lcd_d10", "lcd_d11", "lcd_d12", "lcd_d13", "lcd_d14",
	"lcd_d15", "lcd_d16", "lcd_d17", "lcd_den",
	"lcd_clk_a", "lcd_clk_x", "lcd_hs", "lcd_vs",
};

static struct meson_pmx_func meson_c3_periphs_functions[] = {
	FUNCTION(gpio_periphs),
	FUNCTION(uart_a),
	FUNCTION(uart_b),
	FUNCTION(uart_c),
	FUNCTION(uart_d),
	FUNCTION(uart_e),
	FUNCTION(i2c0),
	FUNCTION(i2c1),
	FUNCTION(i2c2),
	FUNCTION(i2c3),
	FUNCTION(i2c_slave),
	FUNCTION(pwm_a),
	FUNCTION(pwm_b),
	FUNCTION(pwm_c),
	FUNCTION(pwm_d),
	FUNCTION(pwm_e),
	FUNCTION(pwm_f),
	FUNCTION(pwm_g),
	FUNCTION(pwm_h),
	FUNCTION(pwm_i),
	FUNCTION(pwm_j),
	FUNCTION(pwm_k),
	FUNCTION(pwm_l),
	FUNCTION(pwm_m),
	FUNCTION(pwm_n),
	FUNCTION(pwm_c_hiz),
	FUNCTION(ir_out),
	FUNCTION(ir_in),
	FUNCTION(jtag_a),
	FUNCTION(jtag_b),
	FUNCTION(gen_clk),
	FUNCTION(clk12_24),
	FUNCTION(clk_32k_in),
	FUNCTION(emmc),
	FUNCTION(nand),
	FUNCTION(spif),
	FUNCTION(spi_a),
	FUNCTION(spi_b),
	FUNCTION(sdcard),
	FUNCTION(sdio),
	FUNCTION(pdm),
	FUNCTION(eth),
	FUNCTION(mclk_0),
	FUNCTION(mclk_1),
	FUNCTION(tdm),
	FUNCTION(lcd)
};

static struct meson_bank meson_c3_analog_banks[] = {
	/* name  first  last  pullen  pull  dir  out  in*/
	BANK("DSI_", DSI_D0P,    DSI_D3N,
		 0x3,  0,  0x4,  0,  0x1,  0,  0x0,  0,  0x5,  0),
	BANK("SAR_CH", SAR_CH0,   SAR_CH3,
		 0x8,  1,  0x9,  1,  0x7,  1,  0x6,  1,  0xb,  1),
};

static struct meson_pmx_bank meson_c3_analog_pmx_banks[] = {
	/* reg j*/
	BANK_PMX("DSI0",      DSI_D0P,    DSI_D0N,    0x0,  0),
	BANK_PMX("DSI1",      DSI_D1P,    DSI_D3N,    0x0,  8),

	/* reg k*/
	BANK_PMX("DSI2",      DSI_CLKP,   DSI_CLKN,   0x1,  0),
	BANK_PMX("SAR",       SAR_CH0,    SAR_CH3,    0x1,  8),
};

static struct meson_bank meson_c3_periphs_banks[] = {
	/* name  first  last  irq  pullen  pull  dir  out  in ds*/
	BANK("GPIOX_", GPIOX_0,    GPIOX_13,
		 0x3,  0,  0x4,  0,  0x2,  0,  0x1,  0,  0x0,  0),
	BANK("GPIOD_", GPIOD_0,    GPIOD_6,
		 0x23, 0,  0x24, 0,  0x22, 0,  0x21, 0,  0x20, 0),
	BANK("GPIOE_", GPIOE_0,    GPIOE_4,
		 0x33, 0,  0x34, 0,  0x32, 0,  0x31, 0,  0x30, 0),
	BANK("GPIOC_", GPIOC_0,    GPIOC_6,
		 0x43, 0,  0x44, 0,  0x42, 0,  0x41, 0,  0x40, 0),
	BANK("GPIOB_", GPIOB_0,    GPIOB_13,
		 0x53, 0,  0x54, 0,  0x52, 0,  0x51, 0,  0x50, 0),
	BANK("GPIOA_", GPIOA_0,    GPIOA_5,
		 0x63, 0,  0x64, 0,  0x62, 0,  0x61, 0,  0x60, 0),
	BANK("GPIO_TEST_N", GPIO_TEST_N,    GPIO_TEST_N,
		 0x73, 0,  0x74, 0,  0x72, 0,  0x71, 0,  0x70, 0),
};

static struct meson_pmx_bank meson_c3_periphs_pmx_banks[] = {
	/*name	            first	 lask        reg offset*/
	BANK_PMX("B",      GPIOB_0,     GPIOB_14,    0x0,  0),
	BANK_PMX("X",      GPIOX_0,     GPIOX_13,    0x3,  0),
	BANK_PMX("C",      GPIOC_0,     GPIOC_6,     0x9,  0),
	BANK_PMX("A",      GPIOA_0,     GPIOA_5,     0xb,  0),
	BANK_PMX("D",      GPIOD_0,     GPIOD_6,     0x10, 0),
	BANK_PMX("E",      GPIOE_0,     GPIOE_4,     0x12, 0),
	BANK_PMX("TEST_N", GPIO_TEST_N, GPIO_TEST_N, 0x2,  0),
};

static struct meson_drive_bank meson_c3_periphs_drive_banks[] = {
	/*  name	    first         lask        reg  offset*/
	BANK_DRIVE("X",     GPIOX_0,     GPIOX_13,    0x7,  0),
	BANK_DRIVE("D",     GPIOD_0,     GPIOD_6,     0x27, 0),
	BANK_DRIVE("E",     GPIOE_0,     GPIOE_4,     0x37, 0),
	BANK_DRIVE("C",     GPIOC_0,     GPIOC_6,     0x47, 0),
	BANK_DRIVE("B",     GPIOB_0,     GPIOB_14,    0x57, 0),
	BANK_DRIVE("A",     GPIOA_0,     GPIOA_5,     0x67, 0),
	BANK_DRIVE("TESTN", GPIO_TEST_N, GPIO_TEST_N, 0x77,  0),
};

static struct meson_axg_pmx_data meson_c3_periphs_pmx_banks_data = {
	.pmx_banks	= meson_c3_periphs_pmx_banks,
	.num_pmx_banks	= ARRAY_SIZE(meson_c3_periphs_pmx_banks),
};

static struct meson_axg_pmx_data meson_c3_analog_pmx_banks_data = {
	.pmx_banks	= meson_c3_analog_pmx_banks,
	.num_pmx_banks	= ARRAY_SIZE(meson_c3_analog_pmx_banks),
};

static struct meson_drive_data meson_c3_periphs_drive_data = {
	.drive_banks	= meson_c3_periphs_drive_banks,
	.num_drive_banks = ARRAY_SIZE(meson_c3_periphs_drive_banks),
};

static struct meson_pinctrl_data meson_c3_periphs_pinctrl_data = {
	.name		= "periphs-banks",
	.groups		= meson_c3_periphs_groups,
	.funcs		= meson_c3_periphs_functions,
	.banks		= meson_c3_periphs_banks,
	.num_pins	= 55,
	.num_groups	= ARRAY_SIZE(meson_c3_periphs_groups),
	.num_funcs	= ARRAY_SIZE(meson_c3_periphs_functions),
	.num_banks	= ARRAY_SIZE(meson_c3_periphs_banks),
	.pmx_data	= &meson_c3_periphs_pmx_banks_data,
	.drv_data	= &meson_c3_periphs_drive_data,
};

static struct meson_pinctrl_data meson_c3_analog_pinctrl_data = {
	.name		= "analog-banks",
	.groups		= meson_c3_analog_groups,
	.funcs		= meson_c3_analog_functions,
	.banks		= meson_c3_analog_banks,
	.num_pins	= 55,
	.num_groups	= ARRAY_SIZE(meson_c3_analog_groups),
	.num_funcs	= ARRAY_SIZE(meson_c3_analog_functions),
	.num_banks	= ARRAY_SIZE(meson_c3_analog_banks),
	.pmx_data	= &meson_c3_analog_pmx_banks_data,
	.drv_data	= NULL,
};

static const struct udevice_id meson_c3_pinctrl_match[] = {
	{
		.compatible = "amlogic,meson-c3-periphs-pinctrl",
		.data = (ulong)&meson_c3_periphs_pinctrl_data,
	},
	{
		.compatible = "amlogic,meson-c3-analog-pinctrl",
		.data = (ulong)&meson_c3_analog_pinctrl_data,
	},
	{ },
};

U_BOOT_DRIVER(meson_c3_pinctrl) = {
	.name	= "meson-c3-pinctrl",
	.id	= UCLASS_PINCTRL,
	.of_match = of_match_ptr(meson_c3_pinctrl_match),
	.probe = meson_pinctrl_probe,
	.priv_auto_alloc_size = sizeof(struct meson_pinctrl),
	.ops = &meson_axg_pinctrl_ops,
};
