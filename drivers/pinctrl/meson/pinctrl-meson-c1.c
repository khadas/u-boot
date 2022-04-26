// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <dt-bindings/gpio/meson-c1-gpio.h>

#include "pinctrl-meson.h"
#include "pinctrl-meson-axg-pmx.h"

/*bank D func1 */
static const unsigned int uart_b_tx_d_pins[]		= { GPIOD_0 };
static const unsigned int uart_b_rx_d_pins[]		= { GPIOD_1 };
static const unsigned int i2c_a_scl_d_pins[]		= { GPIOD_2 };
static const unsigned int i2c_a_sda_d_pins[]		= { GPIOD_3 };
static const unsigned int ir_out_d4_pins[]		= { GPIOD_4 };
static const unsigned int ir_in_d5_pins[]		= { GPIOD_5 };
static const unsigned int jtag_a_clk_pins[]		= { GPIOD_6 };
static const unsigned int jtag_a_tms_pins[]		= { GPIOD_7 };
static const unsigned int jtag_a_tdi_pins[]		= { GPIOD_8 };
static const unsigned int jtag_a_tdo_pins[]		= { GPIOD_9 };
static const unsigned int gen_clk_d_pins[]		= { GPIOD_11 };
static const unsigned int clk12_24_d_pins[]		= { GPIOD_12 };

/*bank D func2 */
static const unsigned int uart_d_tx_d2_pins[]		= { GPIOD_2 };
static const unsigned int uart_d_rx_d3_pins[]		= { GPIOD_3 };
static const unsigned int swclk_pins[]			= { GPIOD_6 };
static const unsigned int swdio_pins[]			= { GPIOD_7 };
static const unsigned int ir_in_d8_pins[]		= { GPIOD_8 };
static const unsigned int ir_out_d9_pins[]		= { GPIOD_9 };
static const unsigned int clk25_pins[]			= { GPIOD_12 };

/*bank D func3 */
static const unsigned int i2c_slave_scl_pins[]		= { GPIOD_2 };
static const unsigned int i2c_slave_sda_pins[]		= { GPIOD_3 };
static const unsigned int pwm_d_pins[]			= { GPIOD_5 };
static const unsigned int pwm_c_pins[]			= { GPIOD_6 };
static const unsigned int pwm_f_d_pins[]		= { GPIOD_7 };
static const unsigned int uart_d_tx_d8_pins[]		= { GPIOD_8 };
static const unsigned int uart_d_rx_d9_pins[]		= { GPIOD_9 };
static const unsigned int pwm_g_d_pins[]		= { GPIOD_10 };
static const unsigned int clk_32k_in_pins[]		= { GPIOD_12 };

/*bank D func4 */
static const unsigned int pwm_d_hiz_pins[]		= { GPIOD_5 };
static const unsigned int pwm_c_hiz_pins[]		= { GPIOD_6 };
static const unsigned int pwm_f_hiz_pins[]		= { GPIOD_7 };

/*bank E func1 */
static const unsigned int uart_b_cts_pins[]		= { GPIOE_0 };
static const unsigned int uart_b_rts_pins[]		= { GPIOE_1 };

/*bank E func2 */
static const unsigned int uart_d_cts_e_pins[]		= { GPIOE_0 };
static const unsigned int uart_d_rts_e_pins[]		= { GPIOE_1 };

/*bank E func3 */
static const unsigned int pwm_a_pins[]			= { GPIOE_0 };
static const unsigned int pwm_b_pins[]			= { GPIOE_1 };

/*bank E func4 */
static const unsigned int i2c_a_scl_e_pins[]		= { GPIOE_0 };
static const unsigned int i2c_a_sda_e_pins[]		= { GPIOE_1 };

/*bank B func1 */
static const unsigned int emmc_nand_d0_pins[]		= { GPIOB_0 };
static const unsigned int emmc_nand_d1_pins[]		= { GPIOB_1 };
static const unsigned int emmc_nand_d2_pins[]		= { GPIOB_2 };
static const unsigned int emmc_nand_d3_pins[]		= { GPIOB_3 };
static const unsigned int emmc_nand_d4_pins[]		= { GPIOB_4 };
static const unsigned int emmc_nand_d5_pins[]		= { GPIOB_5 };
static const unsigned int emmc_nand_d6_pins[]		= { GPIOB_6 };
static const unsigned int emmc_nand_d7_pins[]		= { GPIOB_7 };
static const unsigned int emmc_clk_pins[]		= { GPIOB_8 };
static const unsigned int emmc_cmd_pins[]		= { GPIOB_10 };
static const unsigned int emmc_nand_ds_pins[]		= { GPIOB_11 };

/*bank B func2 */
static const unsigned int nand_wen_clk_pins[]		= { GPIOB_8 };
static const unsigned int nand_ale_pins[]		= { GPIOB_9 };
static const unsigned int nand_ren_wr_pins[]		= { GPIOB_10 };
static const unsigned int nand_cle_pins[]		= { GPIOB_11 };
static const unsigned int nand_ce0_pins[]		= { GPIOB_12 };

/*bank B func3 */
static const unsigned int spif_hold_pins[]		= { GPIOB_3 };
static const unsigned int spif_mo_pins[]		= { GPIOB_4 };
static const unsigned int spif_mi_pins[]		= { GPIOB_5 };
static const unsigned int spif_clk_pins[]		= { GPIOB_6 };
static const unsigned int spif_wp_n_pins[]		= { GPIOB_7 };
static const unsigned int spif_cs_pins[]		= { GPIOB_13 };

/*bank C func1 */
static const unsigned int sdcard_d0_pins[]		= { GPIOC_0 };
static const unsigned int sdcard_d1_pins[]		= { GPIOC_1 };
static const unsigned int sdcard_d2_pins[]		= { GPIOC_2 };
static const unsigned int sdcard_d3_pins[]		= { GPIOC_3 };
static const unsigned int sdcard_clk_pins[]		= { GPIOC_4 };
static const unsigned int sdcard_cmd_pins[]		= { GPIOC_5 };

/*bank C func2 */
static const unsigned int jtag_b_tdo_pins[]		= { GPIOC_0 };
static const unsigned int jtag_b_tdi_pins[]		= { GPIOC_1 };
static const unsigned int uart_b_rx_c_pins[]		= { GPIOC_2 };
static const unsigned int uart_b_tx_c_pins[]		= { GPIOC_3 };
static const unsigned int jtag_b_clk_pins[]		= { GPIOC_4 };
static const unsigned int jtag_b_tms_pins[]		= { GPIOC_5 };

/*bank C func3 */
static const unsigned int spi_a_mosi_c_pins[]		= { GPIOC_0 };
static const unsigned int spi_a_miso_c_pins[]		= { GPIOC_1 };
static const unsigned int spi_a_ss0_c_pins[]		= { GPIOC_2 };
static const unsigned int spi_a_sclk_c_pins[]		= { GPIOC_3 };
static const unsigned int i2c_e_sda_c_pins[]		= { GPIOC_5 };
static const unsigned int i2c_e_scl_c_pins[]		= { GPIOC_6 };

/*bank C func4 */
static const unsigned int pdm_din0_c_pins[]		= { GPIOC_0 };
static const unsigned int pdm_din1_c_pins[]		= { GPIOC_1 };
static const unsigned int pdm_dclk_c_pins[]		= { GPIOC_4 };

/*bank X func1*/
static const unsigned int sdio_d0_x_pins[]		= { GPIOX_0 };
static const unsigned int sdio_d1_x_pins[]		= { GPIOX_1 };
static const unsigned int sdio_d2_x_pins[]		= { GPIOX_2 };
static const unsigned int sdio_d3_x_pins[]		= { GPIOX_3 };
static const unsigned int sdio_clk_x_pins[]		= { GPIOX_4 };
static const unsigned int sdio_cmd_x_pins[]		= { GPIOX_5 };
static const unsigned int tdm_a_dout1_pins[]		= { GPIOX_8 };
static const unsigned int tdm_a_dout0_pins[]		= { GPIOX_9 };
static const unsigned int tdm_a_fs_pins[]		= { GPIOX_10 };
static const unsigned int tdm_a_sclk_pins[]		= { GPIOX_11 };
static const unsigned int uart_a_tx_x12_pins[]		= { GPIOX_12 };
static const unsigned int uart_a_rx_x13_pins[]		= { GPIOX_13 };
static const unsigned int uart_a_cts_pins[]		= { GPIOX_14 };
static const unsigned int uart_a_rts_pins[]		= { GPIOX_15 };
static const unsigned int pwm_e_pins[]			= { GPIOX_16 };
static const unsigned int pwm_f_x_pins[]		= { GPIOX_17 };
static const unsigned int i2c_c_sda_x_pins[]		= { GPIOX_18 };
static const unsigned int i2c_c_scl_x_pins[]		= { GPIOX_19 };

/*bank X func2*/
static const unsigned int uart_e_tx_x_pins[]		= { GPIOX_6 };
static const unsigned int uart_e_rx_x_pins[]		= { GPIOX_7 };
static const unsigned int tdm_a_din1_pins[]		= { GPIOX_8 };
static const unsigned int tdm_a_din0_pins[]		= { GPIOX_9 };
static const unsigned int tdm_a_slv_fs_pins[]		= { GPIOX_10 };
static const unsigned int tdm_a_slv_sclk_pins[]		= { GPIOX_11 };
static const unsigned int i2c_d_sda_x_pins[]		= { GPIOX_14 };
static const unsigned int i2c_d_scl_x_pins[]		= { GPIOX_15 };

/*bank X func3*/
static const unsigned int eth_mdio_pins[]		= { GPIOX_0 };
static const unsigned int eth_mdc_pins[]		= { GPIOX_1 };
static const unsigned int eth_rx_clk_pins[]		= { GPIOX_2 };
static const unsigned int eth_rx_dv_pins[]		= { GPIOX_3 };
static const unsigned int eth_rxd0_pins[]		= { GPIOX_4 };
static const unsigned int eth_rxd1_pins[]		= { GPIOX_5 };
static const unsigned int eth_rxd2_rgmii_pins[]		= { GPIOX_6 };
static const unsigned int eth_rxd3_rgmii_pins[]		= { GPIOX_7 };
static const unsigned int eth_tx_clk_pins[]		= { GPIOX_8 };
static const unsigned int eth_txen_pins[]		= { GPIOX_9 };
static const unsigned int eth_txd0_pins[]		= { GPIOX_10 };
static const unsigned int eth_txd1_pins[]		= { GPIOX_11 };
static const unsigned int eth_txd2_rgmii_pins[]		= { GPIOX_12 };
static const unsigned int eth_txd3_rgmii_pins[]		= { GPIOX_13 };
static const unsigned int uart_a_tx_x14_pins[]		= { GPIOX_14 };
static const unsigned int uart_a_rx_x15_pins[]		= { GPIOX_15 };
static const unsigned int eth_link_led_pins[]		= { GPIOX_18 };
static const unsigned int eth_act_led_pins[]		= { GPIOX_19 };

/*bank X func4*/
static const unsigned int spi_b_mosi_x_pins[]		= { GPIOX_8 };
static const unsigned int spi_b_miso_x_pins[]		= { GPIOX_9 };
static const unsigned int spi_b_ss0_x_pins[]		= { GPIOX_10 };
static const unsigned int spi_b_sclk_x_pins[]		= { GPIOX_11 };
static const unsigned int pwm_h_x_pins[]		= { GPIOX_14 };
static const unsigned int pwm_g_x_pins[]		= { GPIOX_15 };

/*bank X func5 */
static const unsigned int pwm_i_x_pins[]		= { GPIOX_8 };
static const unsigned int pwm_j_x_pins[]		= { GPIOX_9 };
static const unsigned int i2c_b_sda_x_pins[]		= { GPIOX_10 };
static const unsigned int i2c_b_scl_x_pins[]		= { GPIOX_11 };

/*bank M func1 */
static const unsigned int sdio_d0_m_pins[]		= { GPIOM_0 };
static const unsigned int sdio_d1_m_pins[]		= { GPIOM_1 };
static const unsigned int sdio_d2_m_pins[]		= { GPIOM_2 };
static const unsigned int sdio_d3_m_pins[]		= { GPIOM_3 };
static const unsigned int sdio_clk_m_pins[]		= { GPIOM_4 };
static const unsigned int sdio_cmd_m_pins[]		= { GPIOM_5 };
static const unsigned int pwm_f_m_pins[]		= { GPIOM_7 };
static const unsigned int pwm_g_m_pins[]		= { GPIOM_8 };
static const unsigned int pwm_h_m_pins[]		= { GPIOM_9 };
static const unsigned int clk12_24_M10_pins[]		= { GPIOM_10 };
static const unsigned int clk12_24_M13_pins[]		= { GPIOM_13 };
static const unsigned int i2c_c_sda_m_pins[]		= { GPIOM_14 };
static const unsigned int i2c_c_scl_m_pins[]		= { GPIOM_15 };

/*bank M func2 */
static const unsigned int pwm_i_m_pins[]		= { GPIOM_0 };
static const unsigned int pwm_j_m_pins[]		= { GPIOM_1 };
static const unsigned int pwm_g_hiz_pins[]		= { GPIOM_8 };
static const unsigned int pwm_h_hiz_pins[]		= { GPIOM_9 };
static const unsigned int gen_clk_m10_pins[]		= { GPIOM_10 };
static const unsigned int gen_clk_m13_pins[]		= { GPIOM_13 };

/*bank M func3 */
static const unsigned int pdm_dclk_m_pins[]		= { GPIOM_2 };
static const unsigned int pdm_din0_m_pins[]		= { GPIOM_3 };
static const unsigned int pdm_din1_m_pins[]		= { GPIOM_4 };
static const unsigned int cmclkout1_pins[]		= { GPIOM_10 };
static const unsigned int cmclkout2_pins[]		= { GPIOM_13 };

/*bank M func4 */
static const unsigned int i2c_e_sda_m_pins[]		= { GPIOM_0 };
static const unsigned int i2c_e_scl_m_pins[]		= { GPIOM_1 };
static const unsigned int i2c_b_sda_m_pins[]		= { GPIOM_4 };
static const unsigned int i2c_b_scl_m_pins[]		= { GPIOM_5 };
static const unsigned int i2c_d_sda_m_pins[]		= { GPIOM_8 };
static const unsigned int i2c_d_scl_m_pins[]		= { GPIOM_9 };
static const unsigned int xtal_m10_pins[]		= { GPIOM_10 };
static const unsigned int xtal_m13_pins[]		= { GPIOM_13 };

/*bank M func5 */
static const unsigned int uart_e_cts_pins[]		= { GPIOM_2 };
static const unsigned int uart_e_rts_pins[]		= { GPIOM_3 };
static const unsigned int uart_e_tx_m_pins[]		= { GPIOM_4 };
static const unsigned int uart_e_rx_m_pins[]		= { GPIOM_5 };
static const unsigned int mic_mute_key_pins[]		= { GPIOM_6 };
static const unsigned int mic_mute_led_pins[]		= { GPIOM_7 };

/*bank A func1 */
static const unsigned int mclk_0_pins[]			= { GPIOA_0 };
static const unsigned int tdm_b_sclk_pins[]		= { GPIOA_1 };
static const unsigned int tdm_b_fs_pins[]		= { GPIOA_2 };
static const unsigned int tdm_b_dout0_pins[]		= { GPIOA_3 };
static const unsigned int tdm_b_dout1_pins[]		= { GPIOA_4 };
static const unsigned int tdm_b_dout2_pins[]		= { GPIOA_5 };
static const unsigned int tdm_b_dout3_pins[]		= { GPIOA_6 };
static const unsigned int pwm_g_a7_pins[]		= { GPIOA_7 };
static const unsigned int pwm_h_a8_pins[]		= { GPIOA_8 };
static const unsigned int pwm_i_a9_pins[]		= { GPIOA_9 };
static const unsigned int pwm_j_a10_pins[]		= { GPIOA_10 };
static const unsigned int uart_c_tx_a12_pins[]		= { GPIOA_12 };
static const unsigned int uart_c_rx_a13_pins[]		= { GPIOA_13 };
static const unsigned int i2c_d_sda_a_pins[]		= { GPIOA_14 };
static const unsigned int i2c_d_scl_a_pins[]		= { GPIOA_15 };

/*bank A func2 */
static const unsigned int tdm_b_slv_sclk_pins[]		= { GPIOA_1 };
static const unsigned int tdm_b_slv_fs_pins[]		= { GPIOA_2 };
static const unsigned int tdm_b_din0_pins[]		= { GPIOA_3 };
static const unsigned int tdm_b_din1_pins[]		= { GPIOA_4 };
static const unsigned int tdm_b_din2_pins[]		= { GPIOA_5 };
static const unsigned int tdm_b_din3_pins[]		= { GPIOA_6 };

/*bank A func3 */
static const unsigned int pwm_g_a1_pins[]		= { GPIOA_1 };
static const unsigned int pwm_h_a2_pins[]		= { GPIOA_2 };
static const unsigned int pwm_i_a3_pins[]		= { GPIOA_3 };
static const unsigned int pwm_j_a4_pins[]		= { GPIOA_4 };
static const unsigned int spi_a_mosi_a_pins[]		= { GPIOA_5 };
static const unsigned int spi_a_miso_a_pins[]		= { GPIOA_6 };
static const unsigned int spi_a_sclk_a_pins[]		= { GPIOA_7 };
static const unsigned int spi_a_ss0_a_pins[]		= { GPIOA_8 };
static const unsigned int spi_a_ss1_pins[]		= { GPIOA_9 };
static const unsigned int spi_a_ss2_pins[]		= { GPIOA_10 };
static const unsigned int spi_b_mosi_a_pins[]		= { GPIOA_11 };
static const unsigned int spi_b_miso_a_pins[]		= { GPIOA_12 };
static const unsigned int spi_b_sclk_a_pins[]		= { GPIOA_13 };
static const unsigned int spi_b_ss0_a_pins[]		= { GPIOA_14 };
static const unsigned int spi_b_ss1_pins[]		= { GPIOA_15 };

/*bank A func4 */
static const unsigned int uart_c_tx_a1_pins[]		= { GPIOA_1 };
static const unsigned int uart_c_rx_a2_pins[]		= { GPIOA_2 };
static const unsigned int uart_c_cts_pins[]		= { GPIOA_3 };
static const unsigned int uart_c_rts_pins[]		= { GPIOA_4 };
static const unsigned int uart_d_tx_a_pins[]		= { GPIOA_5 };
static const unsigned int uart_d_rx_a_pins[]		= { GPIOA_6 };
static const unsigned int uart_d_cts_a_pins[]		= { GPIOA_7 };
static const unsigned int uart_d_rts_a_pins[]		= { GPIOA_8 };
static const unsigned int i2c_e_sda_a_pins[]		= { GPIOA_12 };
static const unsigned int i2c_e_scl_a_pins[]		= { GPIOA_13 };

/*bank A func5 */
static const unsigned int mclk_vad_pins[]		= { GPIOA_0 };
static const unsigned int tdm_vad_sclk_a1_pins[]	= { GPIOA_1 };
static const unsigned int tdm_vad_fs_a2_pins[]		= { GPIOA_2 };
static const unsigned int tdm_vad_sclk_a5_pins[]	= { GPIOA_5 };
static const unsigned int tdm_vad_fs_a6_pins[]		= { GPIOA_6 };
static const unsigned int i2c_b_sda_a_pins[]		= { GPIOA_7 };
static const unsigned int i2c_b_scl_a_pins[]		= { GPIOA_8 };
static const unsigned int i2c_c_sda_a_pins[]		= { GPIOA_9 };
static const unsigned int i2c_c_scl_a_pins[]		= { GPIOA_10 };

static struct meson_pmx_group meson_c1_periphs_groups[] = {
	GPIO_GROUP(GPIOD_0),
	GPIO_GROUP(GPIOD_1),
	GPIO_GROUP(GPIOD_2),
	GPIO_GROUP(GPIOD_3),
	GPIO_GROUP(GPIOD_4),
	GPIO_GROUP(GPIOD_5),
	GPIO_GROUP(GPIOD_6),
	GPIO_GROUP(GPIOD_7),
	GPIO_GROUP(GPIOD_8),
	GPIO_GROUP(GPIOD_9),
	GPIO_GROUP(GPIOD_10),
	GPIO_GROUP(GPIOD_11),
	GPIO_GROUP(GPIOD_12),
	GPIO_GROUP(GPIOE_0),
	GPIO_GROUP(GPIOE_1),
	GPIO_GROUP(GPIOB_0),
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
	GPIO_GROUP(GPIOC_0),
	GPIO_GROUP(GPIOC_1),
	GPIO_GROUP(GPIOC_2),
	GPIO_GROUP(GPIOC_3),
	GPIO_GROUP(GPIOC_4),
	GPIO_GROUP(GPIOC_5),
	GPIO_GROUP(GPIOC_6),
	GPIO_GROUP(GPIOC_7),
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
	GPIO_GROUP(GPIOX_14),
	GPIO_GROUP(GPIOX_15),
	GPIO_GROUP(GPIOX_16),
	GPIO_GROUP(GPIOX_17),
	GPIO_GROUP(GPIOX_18),
	GPIO_GROUP(GPIOX_19),
	GPIO_GROUP(GPIOM_0),
	GPIO_GROUP(GPIOM_1),
	GPIO_GROUP(GPIOM_2),
	GPIO_GROUP(GPIOM_3),
	GPIO_GROUP(GPIOM_4),
	GPIO_GROUP(GPIOM_5),
	GPIO_GROUP(GPIOM_6),
	GPIO_GROUP(GPIOM_7),
	GPIO_GROUP(GPIOM_8),
	GPIO_GROUP(GPIOM_9),
	GPIO_GROUP(GPIOM_10),
	GPIO_GROUP(GPIOM_11),
	GPIO_GROUP(GPIOM_12),
	GPIO_GROUP(GPIOM_13),
	GPIO_GROUP(GPIOM_14),
	GPIO_GROUP(GPIOM_15),
	GPIO_GROUP(GPIOA_0),
	GPIO_GROUP(GPIOA_1),
	GPIO_GROUP(GPIOA_2),
	GPIO_GROUP(GPIOA_3),
	GPIO_GROUP(GPIOA_4),
	GPIO_GROUP(GPIOA_5),
	GPIO_GROUP(GPIOA_6),
	GPIO_GROUP(GPIOA_7),
	GPIO_GROUP(GPIOA_8),
	GPIO_GROUP(GPIOA_9),
	GPIO_GROUP(GPIOA_10),
	GPIO_GROUP(GPIOA_11),
	GPIO_GROUP(GPIOA_12),
	GPIO_GROUP(GPIOA_13),
	GPIO_GROUP(GPIOA_14),
	GPIO_GROUP(GPIOA_15),
	GPIO_GROUP(GPIO_TEST_N),

	/* bank D func1 */
	GROUP(uart_b_tx_d,		1),
	GROUP(uart_b_rx_d,		1),
	GROUP(i2c_a_scl_d,		1),
	GROUP(i2c_a_sda_d,		1),
	GROUP(ir_out_d4,		1),
	GROUP(ir_in_d5,			1),
	GROUP(jtag_a_clk,		1),
	GROUP(jtag_a_tms,		1),
	GROUP(jtag_a_tdi,		1),
	GROUP(jtag_a_tdo,		1),
	GROUP(gen_clk_d,		1),
	GROUP(clk12_24_d,		1),

	/* bank D func2 */
	GROUP(uart_d_tx_d2,		2),
	GROUP(uart_d_rx_d3,		2),
	GROUP(swclk,			2),
	GROUP(swdio,			2),
	GROUP(ir_in_d8,			2),
	GROUP(ir_out_d9,		2),
	GROUP(clk25,			2),

	/* bank D func3 */
	GROUP(i2c_slave_scl,		3),
	GROUP(i2c_slave_sda,		3),
	GROUP(pwm_d,			3),
	GROUP(pwm_c,			3),
	GROUP(pwm_f_d,			3),
	GROUP(uart_d_tx_d8,		3),
	GROUP(uart_d_rx_d9,		3),
	GROUP(pwm_g_d,			3),
	GROUP(clk_32k_in,		3),

	/* bank D func4 */
	GROUP(pwm_d_hiz,		4),
	GROUP(pwm_c_hiz,		4),
	GROUP(pwm_f_hiz,		4),

	/* bank E func1 */
	GROUP(uart_b_cts,		1),
	GROUP(uart_b_rts,		1),

	/* bank E func2 */
	GROUP(uart_d_cts_e,		2),
	GROUP(uart_d_rts_e,		2),

	/* bank E func3 */
	GROUP(pwm_a,			3),
	GROUP(pwm_b,			3),

	/* bank E func4 */
	GROUP(i2c_a_scl_e,		4),
	GROUP(i2c_a_sda_e,		4),

	/* bank B func1 */
	GROUP(emmc_nand_d0,		1),
	GROUP(emmc_nand_d1,		1),
	GROUP(emmc_nand_d2,		1),
	GROUP(emmc_nand_d3,		1),
	GROUP(emmc_nand_d4,		1),
	GROUP(emmc_nand_d5,		1),
	GROUP(emmc_nand_d6,		1),
	GROUP(emmc_nand_d7,		1),
	GROUP(emmc_clk,			1),
	GROUP(emmc_cmd,			1),
	GROUP(emmc_nand_ds,		1),

	/* bank B func2 */
	GROUP(nand_wen_clk,		2),
	GROUP(nand_ale,			2),
	GROUP(nand_ren_wr,		2),
	GROUP(nand_cle,			2),
	GROUP(nand_ce0,			2),

	/* bank B func3 */
	GROUP(spif_hold,		3),
	GROUP(spif_mo,			3),
	GROUP(spif_mi,			3),
	GROUP(spif_clk,			3),
	GROUP(spif_wp_n,		3),
	GROUP(spif_cs,			3),

	/* bank C func1 */
	GROUP(sdcard_d0,		1),
	GROUP(sdcard_d1,		1),
	GROUP(sdcard_d2,		1),
	GROUP(sdcard_d3,		1),
	GROUP(sdcard_clk,		1),
	GROUP(sdcard_cmd,		1),

	/* bank C func2 */
	GROUP(jtag_b_tdo,		2),
	GROUP(jtag_b_tdi,		2),
	GROUP(uart_b_rx_c,		2),
	GROUP(uart_b_tx_c,		2),
	GROUP(jtag_b_clk,		2),
	GROUP(jtag_b_tms,		2),

	/* bank C func3 */
	GROUP(spi_a_mosi_c,		3),
	GROUP(spi_a_miso_c,		3),
	GROUP(spi_a_ss0_c,		3),
	GROUP(spi_a_sclk_c,		3),
	GROUP(i2c_e_sda_c,		3),
	GROUP(i2c_e_scl_c,		3),

	/* bank C func4 */
	GROUP(pdm_din0_c,		4),
	GROUP(pdm_din1_c,		4),
	GROUP(pdm_dclk_c,		4),

	/* bank X func1 */
	GROUP(sdio_d0_x,		1),
	GROUP(sdio_d1_x,		1),
	GROUP(sdio_d2_x,		1),
	GROUP(sdio_d3_x,		1),
	GROUP(sdio_clk_x,		1),
	GROUP(sdio_cmd_x,		1),
	GROUP(tdm_a_dout1,		1),
	GROUP(tdm_a_dout0,		1),
	GROUP(tdm_a_fs,			1),
	GROUP(tdm_a_sclk,		1),
	GROUP(uart_a_tx_x12,		1),
	GROUP(uart_a_rx_x13,		1),
	GROUP(uart_a_cts,		1),
	GROUP(uart_a_rts,		1),
	GROUP(pwm_e,			1),
	GROUP(pwm_f_x,			1),
	GROUP(i2c_c_sda_x,		1),
	GROUP(i2c_c_scl_x,		1),

	/* bank X func2 */
	GROUP(uart_e_tx_x,		2),
	GROUP(uart_e_rx_x,		2),
	GROUP(tdm_a_din1,		2),
	GROUP(tdm_a_din0,		2),
	GROUP(tdm_a_slv_fs,		2),
	GROUP(tdm_a_slv_sclk,		2),
	GROUP(i2c_d_sda_x,		2),
	GROUP(i2c_d_scl_x,		2),

	/* bank X func3 */
	GROUP(eth_mdio,			3),
	GROUP(eth_mdc,			3),
	GROUP(eth_rx_clk,		3),
	GROUP(eth_rx_dv,		3),
	GROUP(eth_rxd0,			3),
	GROUP(eth_rxd1,			3),
	GROUP(eth_rxd2_rgmii,		3),
	GROUP(eth_rxd3_rgmii,		3),
	GROUP(eth_tx_clk,		3),
	GROUP(eth_txen,			3),
	GROUP(eth_txd0,			3),
	GROUP(eth_txd1,			3),
	GROUP(eth_txd2_rgmii,		3),
	GROUP(eth_txd3_rgmii,		3),
	GROUP(uart_a_tx_x14,		3),
	GROUP(uart_a_rx_x15,		3),
	GROUP(eth_link_led,		3),
	GROUP(eth_act_led,		3),

	/* bank X func4 */
	GROUP(spi_b_mosi_x,		4),
	GROUP(spi_b_miso_x,		4),
	GROUP(spi_b_ss0_x,		4),
	GROUP(spi_b_sclk_x,		4),
	GROUP(pwm_h_x,			4),
	GROUP(pwm_g_x,			4),

	/* bank X func5 */
	GROUP(pwm_i_x,			5),
	GROUP(pwm_j_x,			5),
	GROUP(i2c_b_sda_x,		5),
	GROUP(i2c_b_scl_x,		5),

	/* bank M func1 */
	GROUP(sdio_d0_m,		1),
	GROUP(sdio_d1_m,		1),
	GROUP(sdio_d2_m,		1),
	GROUP(sdio_d3_m,		1),
	GROUP(sdio_clk_m,		1),
	GROUP(sdio_cmd_m,		1),
	GROUP(pwm_f_m,			1),
	GROUP(pwm_g_m,			1),
	GROUP(pwm_h_m,			1),
	GROUP(clk12_24_M10,		1),
	GROUP(clk12_24_M13,		1),
	GROUP(i2c_c_sda_m,		1),
	GROUP(i2c_c_scl_m,		1),

	/* bank M func2 */
	GROUP(pwm_i_m,			2),
	GROUP(pwm_j_m,			2),
	GROUP(pwm_g_hiz,		2),
	GROUP(pwm_h_hiz,		2),
	GROUP(gen_clk_m10,		2),
	GROUP(gen_clk_m13,		2),

	/* bank M func3 */
	GROUP(pdm_dclk_m,		3),
	GROUP(pdm_din0_m,		3),
	GROUP(pdm_din1_m,		3),
	GROUP(cmclkout1,		3),
	GROUP(cmclkout2,		3),

	/* bank M func4 */
	GROUP(i2c_e_sda_m,		4),
	GROUP(i2c_e_scl_m,		4),
	GROUP(i2c_b_sda_m,		4),
	GROUP(i2c_b_scl_m,		4),
	GROUP(i2c_d_sda_m,		4),
	GROUP(i2c_d_scl_m,		4),
	GROUP(xtal_m10,			4),
	GROUP(xtal_m13,			4),

	/* bank M func5 */
	GROUP(uart_e_cts,		5),
	GROUP(uart_e_rts,		5),
	GROUP(uart_e_tx_m,		5),
	GROUP(uart_e_rx_m,		5),
	GROUP(mic_mute_key,		5),
	GROUP(mic_mute_led,		5),

	/* bank A func1 */
	GROUP(mclk_0,			1),
	GROUP(tdm_b_sclk,		1),
	GROUP(tdm_b_fs,			1),
	GROUP(tdm_b_dout0,		1),
	GROUP(tdm_b_dout1,		1),
	GROUP(tdm_b_dout2,		1),
	GROUP(tdm_b_dout3,		1),
	GROUP(pwm_g_a7,			1),
	GROUP(pwm_h_a8,			1),
	GROUP(pwm_i_a9,			1),
	GROUP(pwm_j_a10,		1),
	GROUP(uart_c_tx_a12,		1),
	GROUP(uart_c_rx_a13,		1),
	GROUP(i2c_d_sda_a,		1),
	GROUP(i2c_d_scl_a,		1),

	/* bank A func2 */
	GROUP(tdm_b_slv_sclk,		2),
	GROUP(tdm_b_slv_fs,		2),
	GROUP(tdm_b_din0,		2),
	GROUP(tdm_b_din1,		2),
	GROUP(tdm_b_din2,		2),
	GROUP(tdm_b_din3,		2),

	/* bank A func3 */
	GROUP(pwm_g_a1,			3),
	GROUP(pwm_h_a2,			3),
	GROUP(pwm_i_a3,			3),
	GROUP(pwm_j_a4,			3),
	GROUP(spi_a_mosi_a,		3),
	GROUP(spi_a_miso_a,		3),
	GROUP(spi_a_sclk_a,		3),
	GROUP(spi_a_ss0_a,		3),
	GROUP(spi_a_ss1,		3),
	GROUP(spi_a_ss2,		3),
	GROUP(spi_b_mosi_a,		3),
	GROUP(spi_b_miso_a,		3),
	GROUP(spi_b_sclk_a,		3),
	GROUP(spi_b_ss0_a,		3),
	GROUP(spi_b_ss1,		3),

	/* bank A func4 */
	GROUP(uart_c_tx_a1,		4),
	GROUP(uart_c_rx_a2,		4),
	GROUP(uart_c_cts,		4),
	GROUP(uart_c_rts,		4),
	GROUP(uart_d_tx_a,		4),
	GROUP(uart_d_rx_a,		4),
	GROUP(uart_d_cts_a,		4),
	GROUP(uart_d_rts_a,		4),
	GROUP(i2c_e_sda_a,		4),
	GROUP(i2c_e_scl_a,		4),

	/* bank A func5 */
	GROUP(mclk_vad,			5),
	GROUP(tdm_vad_sclk_a1,		5),
	GROUP(tdm_vad_fs_a2,		5),
	GROUP(tdm_vad_sclk_a5,		5),
	GROUP(tdm_vad_fs_a6,		5),
	GROUP(i2c_b_sda_a,		5),
	GROUP(i2c_b_scl_a,		5),
	GROUP(i2c_c_sda_a,		5),
	GROUP(i2c_c_scl_a,		5),
};

static const char * const gpio_periphs_groups[] = {
	"GPIO_TEST_N",

	"GPIOD_0", "GPIOD_1", "GPIOD_2", "GPIOD_3", "GPIOD_4",
	"GPIOD_5", "GPIOD_6", "GPIOD_7", "GPIOD_8", "GPIOD_9",
	"GPIOD_10", "GPIOD_11", "GPIOD_12",

	"GPIOE_0", "GPIOE_1",

	"GPIOB_0", "GPIOB_1", "GPIOB_2", "GPIOB_3", "GPIOB_4",
	"GPIOB_5", "GPIOB_6", "GPIOB_7", "GPIOB_8", "GPIOB_9",
	"GPIOB_10", "GPIOB_11", "GPIOB_12", "GPIOB_13",

	"GPIOC_0", "GPIOC_1", "GPIOC_2", "GPIOC_3", "GPIOC_4",
	"GPIOC_5", "GPIOC_6", "GPIOC_7",

	"GPIOX_0", "GPIOX_1", "GPIOX_2", "GPIOX_3", "GPIOX_4",
	"GPIOX_5", "GPIOX_6", "GPIOX_7", "GPIOX_8", "GPIOX_9",
	"GPIOX_10", "GPIOX_11", "GPIOX_12", "GPIOX_13", "GPIOX_14",
	"GPIOX_15", "GPIOX_16", "GPIOX_17", "GPIOX_18", "GPIOX_19",

	"GPIOM_0", "GPIOM_1", "GPIOM_2", "GPIOM_3", "GPIOM_4",
	"GPIOM_5", "GPIOM_6", "GPIOM_7", "GPIOM_8", "GPIOM_9",
	"GPIOM_10", "GPIOM_11", "GPIOM_12", "GPIOM_13", "GPIOM_14",
	"GPIOM_15",

	"GPIOA_0", "GPIOA_1", "GPIOA_2", "GPIOA_3", "GPIOA_4",
	"GPIOA_5", "GPIOA_6", "GPIOA_7", "GPIOA_8", "GPIOA_9",
	"GPIOA_10", "GPIOA_11", "GPIOA_12", "GPIOA_13", "GPIOA_14",
	"GPIOA_15",
};

static const char * const uart_a_groups[] = {
	"uart_a_tx_x12", "uart_a_rx_x13", "uart_a_cts", "uart_a_rts",
	"uart_a_tx_x14", "uart_a_rx_x15",
};

static const char * const uart_b_groups[] = {
	"uart_b_tx_d", "uart_b_rx_d", "uart_b_cts", "uart_b_rts",
	"uart_b_rx_c", "uart_b_tx_c",
};

static const char * const uart_c_groups[] = {
	"uart_c_tx_a12", "uart_c_rx_a13", "uart_c_tx_a1", "uart_c_rx_a2",
	"uart_c_cts", "uart_c_rts",
};

static const char * const uart_d_groups[] = {
	"uart_d_tx_d2", "uart_d_rx_d3", "uart_d_tx_d8", "uart_d_rx_d9",
	"uart_d_cts_e", "uart_d_rts_e", "uart_d_tx_a", "uart_d_tx_a",
	"uart_d_cts_a", "uart_d_rts_a",
};

static const char * const uart_e_groups[] = {
	"uart_e_tx_x", "uart_e_rx_x", "uart_e_cts", "uart_e_rts", "uart_e_tx_m",
	"uart_e_rx_m",
};

static const char * const i2c_a_groups[] = {
	"i2c_a_scl_d", "i2c_a_sda_d", "i2c_a_scl_e", "i2c_a_sda_e",
};

static const char * const i2c_b_groups[] = {
	"i2c_b_sda_x", "i2c_b_scl_x", "i2c_b_sda_a", "i2c_b_scl_a",
	"i2c_b_sda_m", "i2c_b_scl_m",
};

static const char * const i2c_c_groups[] = {
	"i2c_c_sda_x", "i2c_c_scl_x", "i2c_c_sda_m", "i2c_c_scl_m",
	"i2c_c_sda_a", "i2c_c_scl_a",
};

static const char * const i2c_d_groups[] = {
	"i2c_d_sda_x", "i2c_d_scl_x", "i2c_d_sda_a", "i2c_d_scl_a",
	"i2c_d_sda_m", "i2c_d_scl_m",
};

static const char * const i2c_e_groups[] = {
	"i2c_e_sda_c", "i2c_e_scl_c", "i2c_e_sda_m", "i2c_e_scl_m",
	"i2c_e_sda_a", "i2c_e_scl_a",
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
	"pwm_f_d", "pwm_f_m", "pwm_f_x"
};

static const char * const pwm_g_groups[] = {
	"pwm_g_d", "pwm_g_x", "pwm_g_m", "pwm_g_a7",
	"pwm_g_a1",
};

static const char * const pwm_h_groups[] = {
	"pwm_h_x", "pwm_h_m", "pwm_h_a8", "pwm_h_a2",
};

static const char * const pwm_i_groups[] = {
	"pwm_i_x", "pwm_i_m", "pwm_i_a9", "pwm_i_a3",
};

static const char * const pwm_j_groups[] = {
	"pwm_j_x", "pwm_j_m", "pwm_j_a10", "pwm_j_a4",
};

static const char * const pwm_c_hiz_groups[] = {
	"pwm_c_hiz",
};

static const char * const pwm_d_hiz_groups[] = {
	"pwm_d_hiz",
};

static const char * const pwm_f_hiz_groups[] = {
	"pwm_f_hiz",
};

static const char * const pwm_g_hiz_groups[] = {
	"pwm_g_hiz",
};

static const char * const pwm_h_hiz_groups[] = {
	"pwm_h_hiz",
};

static const char * const ir_out_groups[] = {
	"ir_out_d4", "ir_out_d9",
};

static const char * const ir_in_groups[] = {
	"ir_in_d5", "ir_in_d8",
};

static const char * const jtag_a_groups[] = {
	"jtag_a_clk", "jtag_a_tms", "jtag_a_tdi", "jtag_a_tdo",
};

static const char * const jtag_b_groups[] = {
	"jtag_b_tdo", "jtag_b_tdi", "jtag_b_clk", "jtag_b_tms",
};

static const char * const gen_clk_groups[] = {
	"gen_clk_d", "gen_clk_m10", "gen_clk_m13",
};

static const char * const clk12_24_groups[] = {
	"clk12_24_d", "clk12_24_m10", "clk12_24_m13",
};

static const char * const sw_groups[] = {
	"swclk", "swdio",
};

static const char * const clk25_groups[] = {
	"clk25",
};

static const char * const clk_32k_in_groups[] = {
	"clk_32k_in",
};

static const char * const emmc_groups[] = {
	"emmc_nand_d0", "emmc_nand_d1", "emmc_nand_d2", "emmc_nand_d3",
	"emmc_nand_d4", "emmc_nand_d5", "emmc_nand_d6", "emmc_nand_d7",
	"emmc_clk", "emmc_cmd", "emmc_nand_ds",
};

static const char * const nand_groups[] = {
	"emmc_nand_d0", "emmc_nand_d1", "emmc_nand_d2", "emmc_nand_d3",
	"emmc_nand_d4", "emmc_nand_d5", "emmc_nand_d6", "emmc_nand_d7",
	"emmc_nand_ds", "nand_wen_clk", "nand_ale", "nand_ren_wr",
	"nand_cle", "nand_ce0",
};

static const char * const spif_groups[] = {
	"spif_hold", "spif_mo", "spif_mi", "spif_clk",
	"spif_wp_n", "spif_cs",
};

static const char * const sdcard_groups[] = {
	"sdcard_d0", "sdcard_d1", "sdcard_d2", "sdcard_d3",
	"sdcard_clk", "sdcard_cmd",
};

static const char * const spi_a_groups[] = {
	"spi_a_mosi_c", "spi_a_miso_c", "spi_a_ss0_c", "spi_a_sclk_c",
	"spi_a_mosi_a", "spi_a_miso_a", "spi_a_sclk_a", "spi_a_ss0_a",
	"spi_a_ss1", "spi_a_ss2",
};

static const char * const pdm_groups[] = {
	"pdm_din0_c", "pdm_din1_c", "pdm_dclk_c", "pdm_dlck_m",
	"pdm_din0_m", "pdm_din1_m",
};

static const char * const sdio_groups[] = {
	"sdio_d0_x", "sdio_d1_x", "sdio_d2_x", "sdio_d3_x",
	"sdio_clk_x", "sdio_cmd_x",

	"sdio_d0_m", "sdio_d1_m", "sdio_d2_m", "sdio_d3_m",
	"sdio_clk_m", "sdio_cmd_m",
};

static const char * const eth_groups[] = {
	"eth_mdio", "eth_mdc", "eth_rx_clk", "eth_rx_dv",
	"eth_rxd0", "eth_rxd1", "eth_rxd2_rgmii", "eth_rxd3_rgmii",
	"eth_tx_clk", "eth_txen", "eth_txd0", "eth_txd1",
	"eth_txd2_rgmii", "eth_txd3_rgmii", "eth_link_led", "eth_act_led",
};

static const char * const spi_b_groups[] = {
	"spi_b_mosi_x", "spi_b_miso_x", "spi_b_ss0_x", "spi_b_sclk_x",
	"spi_b_mosi_a", "spi_b_miso_a", "spi_b_ss0_a", "spi_b_sclk_a",
	"spi_b_ss1",
};

static const char * const cmclkout_groups[] = {
	"cmclkout1", "cmclkout2",
};

static const char * const xtal_groups[] = {
	"xtal_m10", "xtal_m13",
};

static const char * const mic_mute_groups[] = {
	"mic_mute_key", "mic_mute_led",
};

static const char * const mclk_0_groups[] = {
	"mclk_0",
};

static const char * const tdm_a_groups[] = {
	"tdm_a_dout1", "tdm_a_dout0", "tdm_a_fs", "tdm_a_sclk",
	"tdm_a_din1", "tdm_a_din0", "tdm_a_slv_fs", "tdm_a_slv_sclk",
};

static const char * const tdm_b_groups[] = {
	"tdm_b_sclk", "tdm_b_fs", "tdm_b_dout0", "tdm_b_dout1",
	"tdm_b_dout2", "tdm_b_dout3",

	"tdm_b_slv_sclk", "tdm_b_slv_fs", "tdm_b_din0", "tdm_b_din1",
	"tdm_b_din2", "tdm_b_din3",
};

static const char * const tdm_vad_groups[] = {
	"tdm_vad_sclk_a1", "tdm_vad_fs_a2", "tdm_vad_sclk_a5", "tdm_vad_fs_a6",
};

static const char * const mclk_vad_groups[] = {
	"mclk_vad",
};

static struct meson_pmx_func meson_c1_periphs_functions[] = {
	FUNCTION(gpio_periphs),
	FUNCTION(uart_a),
	FUNCTION(uart_b),
	FUNCTION(uart_c),
	FUNCTION(uart_d),
	FUNCTION(uart_e),
	FUNCTION(i2c_a),
	FUNCTION(i2c_b),
	FUNCTION(i2c_c),
	FUNCTION(i2c_d),
	FUNCTION(i2c_e),
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
	FUNCTION(pwm_c_hiz),
	FUNCTION(pwm_d_hiz),
	FUNCTION(pwm_f_hiz),
	FUNCTION(pwm_g_hiz),
	FUNCTION(pwm_h_hiz),
	FUNCTION(ir_out),
	FUNCTION(ir_in),
	FUNCTION(jtag_a),
	FUNCTION(jtag_b),
	FUNCTION(gen_clk),
	FUNCTION(clk12_24),
	FUNCTION(sw),
	FUNCTION(clk25),
	FUNCTION(clk_32k_in),
	FUNCTION(emmc),
	FUNCTION(nand),
	FUNCTION(spif),
	FUNCTION(sdcard),
	FUNCTION(spi_a),
	FUNCTION(pdm),
	FUNCTION(sdio),
	FUNCTION(eth),
	FUNCTION(spi_b),
	FUNCTION(cmclkout),
	FUNCTION(xtal),
	FUNCTION(mic_mute),
	FUNCTION(mclk_0),
	FUNCTION(tdm_a),
	FUNCTION(tdm_b),
	FUNCTION(tdm_vad),
	FUNCTION(mclk_vad),
};

static struct meson_bank meson_c1_periphs_banks[] = {
	/* name  first  last  irq  pullen  pull  dir  out  in */
	BANK("GPIOD_", GPIOD_0,    GPIOD_12,
	     0x3,   0,  0x4,  0,   0x2,  0,  0x1,  0,  0x0,  0),
	BANK("GPIOE_", GPIOE_0,    GPIOE_1,
	     0x13,  0,  0x14,  0,  0x12, 0,  0x11, 0,  0x10, 0),
	BANK("GPIOB_", GPIOB_0,    GPIOB_13,
	     0x23,  0,  0x24,  0,  0x22, 0,  0x21, 0,  0x20, 0),
	BANK("GPIOC_", GPIOC_0,    GPIOC_7,
	     0x33,  0,  0x34,  0,  0x32, 0,  0x31, 0,  0x30, 0),
	BANK("GPIOX_", GPIOX_0,    GPIOX_19,
	     0x43,  0,  0x44,  0,  0x42, 0,  0x41, 0,  0x40, 0),
	BANK("GPIOM_", GPIOM_0,    GPIOM_15,
	     0x53,  0,  0x54,  0,  0x52, 0,  0x51, 0,  0x50, 0),
	BANK("GPIOA_", GPIOA_0,    GPIOA_15,
	     0x63,  0,  0x64,  0,  0x62, 0,  0x61, 0,  0x60, 0),
	BANK("GPIO_TEST_N", GPIO_TEST_N,    GPIO_TEST_N,
	     0x73,  0,  0x74,  0,  0x72, 0,  0x71,  0, 0x70, 0),
};

static struct meson_pmx_bank meson_c1_periphs_pmx_banks[] = {
	/*name	            first	 lask        reg offset*/
	BANK_PMX("D",      GPIOD_0,     GPIOD_12,    0x0, 0),
	BANK_PMX("E",      GPIOE_0,     GPIOE_1,     0x2, 0),
	BANK_PMX("B",      GPIOB_0,     GPIOB_13,    0x3, 0),
	BANK_PMX("C",      GPIOC_0,     GPIOC_7,     0x5, 0),
	BANK_PMX("X",      GPIOX_0,     GPIOX_19,    0x6, 0),
	BANK_PMX("M",      GPIOM_0,     GPIOM_15,    0x9, 0),
	BANK_PMX("A",      GPIOA_0,     GPIOA_15,    0xb, 0),
	BANK_PMX("TEST_N", GPIO_TEST_N, GPIO_TEST_N, 0xf, 0),
};

static struct meson_drive_bank meson_c1_periphs_drive_banks[] = {
	/*  name	    first         lask        reg  offset*/
	BANK_DRIVE("D",     GPIOD_0,     GPIOD_12,    0x5,  0),
	BANK_DRIVE("E",     GPIOE_0,     GPIOE_1,     0x15, 0),
	BANK_DRIVE("B",     GPIOB_0,     GPIOB_13,    0x25, 0),
	BANK_DRIVE("C",     GPIOC_0,     GPIOC_7,     0x35, 0),
	BANK_DRIVE("X",     GPIOX_0,     GPIOX_19,    0x45, 0),
	BANK_DRIVE("M",     GPIOM_0,     GPIOM_15,    0x55, 0),
	BANK_DRIVE("A",     GPIOA_0,     GPIOA_15,    0x65, 0),
	BANK_DRIVE("TESTN", GPIO_TEST_N, GPIO_TEST_N, 0x75,  0),
};

static struct meson_axg_pmx_data meson_c1_periphs_pmx_banks_data = {
	.pmx_banks	= meson_c1_periphs_pmx_banks,
	.num_pmx_banks	= ARRAY_SIZE(meson_c1_periphs_pmx_banks),
};

static struct meson_drive_data meson_c1_periphs_drive_data = {
	.drive_banks	= meson_c1_periphs_drive_banks,
	.num_drive_banks = ARRAY_SIZE(meson_c1_periphs_drive_banks),
};

static struct meson_pinctrl_data meson_c1_periphs_pinctrl_data = {
	.name		= "periphs-banks",
	.groups		= meson_c1_periphs_groups,
	.funcs		= meson_c1_periphs_functions,
	.banks		= meson_c1_periphs_banks,
	.num_pins	= 90,
	.num_groups	= ARRAY_SIZE(meson_c1_periphs_groups),
	.num_funcs	= ARRAY_SIZE(meson_c1_periphs_functions),
	.num_banks	= ARRAY_SIZE(meson_c1_periphs_banks),
	.pmx_data	= &meson_c1_periphs_pmx_banks_data,
	.drv_data	= &meson_c1_periphs_drive_data,
};

static const struct udevice_id meson_c1_pinctrl_match[] = {
	{
		.compatible = "amlogic,meson-c1-periphs-pinctrl",
		.data = (ulong)&meson_c1_periphs_pinctrl_data,
	},
	{ },
};

U_BOOT_DRIVER(meson_c1_pinctrl) = {
	.name	= "meson-c1-pinctrl",
	.id	= UCLASS_PINCTRL,
	.of_match = of_match_ptr(meson_c1_pinctrl_match),
	.probe = meson_pinctrl_probe,
	.priv_auto_alloc_size = sizeof(struct meson_pinctrl),
	.ops = &meson_axg_pinctrl_ops,
};
