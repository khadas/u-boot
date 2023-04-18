// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <dt-bindings/gpio/meson-txhd2-gpio.h>

#include "pinctrl-meson.h"
#include "pinctrl-meson-axg-pmx.h"

/* GPIODV func1 */
static const unsigned int i2cm_b_sda_dv0_pins[]			= { GPIODV_0 };
static const unsigned int i2cm_b_scl_dv1_pins[]			= { GPIODV_1 };
static const unsigned int dtv_if_agc_dv2_pins[]			= { GPIODV_2 };
static const unsigned int i2cm_a_sda_dv4_pins[]			= { GPIODV_4 };
static const unsigned int i2cm_a_scl_dv5_pins[]			= { GPIODV_5 };
static const unsigned int tsin_d0_a_dv6_pins[]			= { GPIODV_6 };
static const unsigned int tsin_clk_a_dv7_pins[]			= { GPIODV_7 };
static const unsigned int tsin_sop_a_dv8_pins[]			= { GPIODV_8 };
static const unsigned int tsin_valid_a_dv9_pins[]		= { GPIODV_9 };

/* GPIODV func2 */
static const unsigned int atv_if_agc_dv2_pins[]			= { GPIODV_2 };
static const unsigned int dtv_rf_agc_dv3_pins[]			= { GPIODV_3 };
static const unsigned int uart_rts_a_dv6_pins[]			= { GPIODV_6 };
static const unsigned int uart_cts_a_dv7_pins[]			= { GPIODV_7 };
static const unsigned int uart_tx_a_dv8_pins[]			= { GPIODV_8 };
static const unsigned int uart_rx_a_dv9_pins[]			= { GPIODV_9 };

/* GPIODV func3 */
static const unsigned int spi_ss3_a_pins[]			= { GPIODV_3 };
static const unsigned int spi_ss2_a_pins[]			= { GPIODV_4 };
static const unsigned int spi_ss1_a_dv5_pins[]			= { GPIODV_5 };
static const unsigned int spi_ss0_a_dv6_pins[]			= { GPIODV_6 };
static const unsigned int spi_miso_a_dv7_pins[]			= { GPIODV_7 };
static const unsigned int spi_mosi_a_dv8_pins[]			= { GPIODV_8 };
static const unsigned int spi_clk_a_dv9_pins[]			= { GPIODV_9 };

/* GPIODV func4 */
static const unsigned int pwm_d_dv2_pins[]			= { GPIODV_2 };
static const unsigned int spdif_out_dv3_pins[]			= { GPIODV_3 };
static const unsigned int pwm_a_dv4_pins[]			= { GPIODV_4 };
static const unsigned int pwm_vs_dv5_pins[]			= { GPIODV_5 };
static const unsigned int pdm_din0_dv6_pins[]			= { GPIODV_6 };
static const unsigned int pdm_dclk_dv7_pins[]			= { GPIODV_7 };

/* GPIODV func5 */
static const unsigned int debug_bus_out0_pins[]			= { GPIODV_0 };
static const unsigned int debug_bus_out1_pins[]			= { GPIODV_1 };
static const unsigned int debug_bus_out2_pins[]			= { GPIODV_2 };
static const unsigned int debug_bus_out3_pins[]			= { GPIODV_3 };
static const unsigned int debug_bus_out4_pins[]			= { GPIODV_4 };
static const unsigned int debug_bus_out5_pins[]			= { GPIODV_5 };
static const unsigned int debug_bus_out6_pins[]			= { GPIODV_6 };
static const unsigned int debug_bus_out7_pins[]			= { GPIODV_7 };
static const unsigned int debug_bus_out8_pins[]			= { GPIODV_8 };
static const unsigned int debug_bus_out9_pins[]			= { GPIODV_9 };

/* GPIODV func6 */
static const unsigned int eth_debug_in0_pins[]			= { GPIODV_0 };
static const unsigned int eth_debug_in1_pins[]			= { GPIODV_1 };
static const unsigned int eth_debug_in2_pins[]			= { GPIODV_2 };
static const unsigned int eth_debug_in3_pins[]			= { GPIODV_3 };
static const unsigned int eth_debug_in4_pins[]			= { GPIODV_4 };
static const unsigned int eth_debug_in5_pins[]			= { GPIODV_5 };
static const unsigned int eth_debug_in6_pins[]			= { GPIODV_6 };
static const unsigned int eth_debug_in7_pins[]			= { GPIODV_7 };
static const unsigned int eth_debug_in8_pins[]			= { GPIODV_8 };
static const unsigned int eth_debug_in9_pins[]			= { GPIODV_9 };

/* GPIODV func7 */
static const unsigned int lvds_prod_o_0_pins[]			= { GPIODV_0 };
static const unsigned int lvds_prod_o_1_pins[]			= { GPIODV_1 };
static const unsigned int lvds_prod_o_2_pins[]			= { GPIODV_2 };
static const unsigned int lvds_prod_o_3_pins[]			= { GPIODV_3 };
static const unsigned int lvds_prod_o_4_pins[]			= { GPIODV_4 };
static const unsigned int lvds_prod_o_5_pins[]			= { GPIODV_5 };
static const unsigned int lvds_prod_o_6_pins[]			= { GPIODV_6 };
static const unsigned int lvds_prod_o_7_pins[]			= { GPIODV_7 };
static const unsigned int lvds_prod_o_8_pins[]			= { GPIODV_8 };
static const unsigned int lvds_prod_o_9_pins[]			= { GPIODV_9 };

/* GPIOB func1 */
static const unsigned int emmc_d0_pins[]			= { GPIOB_0 };
static const unsigned int emmc_d1_pins[]			= { GPIOB_1 };
static const unsigned int emmc_d2_pins[]			= { GPIOB_2 };
static const unsigned int emmc_d3_pins[]			= { GPIOB_3 };
static const unsigned int emmc_d4_pins[]			= { GPIOB_4 };
static const unsigned int emmc_d5_pins[]			= { GPIOB_5 };
static const unsigned int emmc_d6_pins[]			= { GPIOB_6 };
static const unsigned int emmc_d7_pins[]			= { GPIOB_7 };
static const unsigned int emmc_clk_pins[]			= { GPIOB_8 };
static const unsigned int emmc_cmd_pins[]			= { GPIOB_10 };
static const unsigned int emmc_ds_pins[]			= { GPIOB_11 };

/* GPIOB func3 */
static const unsigned int spif_hold_pins[]			= { GPIOB_3 };
static const unsigned int spif_mo_pins[]			= { GPIOB_4 };
static const unsigned int spif_mi_pins[]			= { GPIOB_5 };
static const unsigned int spif_clk_pins[]			= { GPIOB_6 };
static const unsigned int spif_wp_pins[]			= { GPIOB_9 };
static const unsigned int spif_cs_pins[]			= { GPIOB_12 };

/* GPIOC func1 */
static const unsigned int tsin_sop_a_c7_pins[]			= { GPIOC_7 };
static const unsigned int tsin_valid_a_c8_pins[]		= { GPIOC_8 };
static const unsigned int tsin_d0_a_c9_pins[]			= { GPIOC_9 };
static const unsigned int tsin_clk_a_c10_pins[]			= { GPIOC_10 };

/* GPIOC func2 */
static const unsigned int jtag_tdo_c0_pins[]			= { GPIOC_0 };
static const unsigned int jtag_tdi_c1_pins[]			= { GPIOC_1 };
static const unsigned int uart_rx_ao_a_c2_pins[]		= { GPIOC_2 };
static const unsigned int uart_tx_ao_a_c3_pins[]		= { GPIOC_3 };
static const unsigned int jtag_clk_c4_pins[]			= { GPIOC_4 };
static const unsigned int jtag_tms_c5_pins[]			= { GPIOC_5 };
static const unsigned int uart_rx_a_c7_pins[]			= { GPIOC_7 };
static const unsigned int uart_tx_a_c8_pins[]			= { GPIOC_8 };
static const unsigned int uart_cts_a_c9_pins[]			= { GPIOC_9 };
static const unsigned int uart_rts_a_c10_pins[]			= { GPIOC_10 };

/* GPIOC func3 */
static const unsigned int uart_tx_ao_a_c2_pins[]		= { GPIOC_2 };
static const unsigned int uart_rx_ao_a_c3_pins[]		= { GPIOC_3 };
static const unsigned int tdm_mclk0_pins[]			= { GPIOC_6 };
static const unsigned int tdm_fs0_c7_pins[]			= { GPIOC_7 };
static const unsigned int tdm_d0_c8_pins[]			= { GPIOC_8 };
static const unsigned int tdm_d1_c9_pins[]			= { GPIOC_9 };
static const unsigned int tdm_sclk0_c10_pins[]			= { GPIOC_10 };

/* GPIOC func4 */
static const unsigned int tdm_sclk1_c0_pins[]			= { GPIOC_0 };
static const unsigned int tdm_mclk1_c1_pins[]			= { GPIOC_1 };
static const unsigned int tdm_d3_c2_pins[]			= { GPIOC_2 };
static const unsigned int tdm_d5_c3_pins[]			= { GPIOC_3 };
static const unsigned int tdm_d2_c4_pins[]			= { GPIOC_4 };
static const unsigned int tdm_fs1_c5_pins[]			= { GPIOC_5 };
static const unsigned int spi_ss1_a_c6_pins[]			= { GPIOC_6 };
static const unsigned int spi_ss0_a_c7_pins[]			= { GPIOC_7 };
static const unsigned int spi_mosi_a_c8_pins[]			= { GPIOC_8 };
static const unsigned int spi_miso_a_c9_pins[]			= { GPIOC_9 };
static const unsigned int spi_clk_a_c10_pins[]			= { GPIOC_10 };

/* GPIOC func5 */
static const unsigned int gen_clk_out_pins[]			= { GPIOC_0 };
static const unsigned int i2cm_c_sda_c2_pins[]			= { GPIOC_2 };
static const unsigned int i2cm_c_scl_c3_pins[]			= { GPIOC_3 };
static const unsigned int tdm_d4_c4_pins[]			= { GPIOC_4 };
static const unsigned int pwm_d_c6_pins[]			= { GPIOC_6 };
static const unsigned int pwm_b_c9_pins[]			= { GPIOC_9 };
static const unsigned int pwm_c_c10_pins[]			= { GPIOC_10 };

/* GPIOC func6 */
static const unsigned int pdm_dclk_c1_pins[]			= { GPIOC_1 };
static const unsigned int pdm_din0_c2_pins[]			= { GPIOC_2 };
static const unsigned int eth_debug_in10_pins[]			= { GPIOC_3 };
static const unsigned int eth_debug_in11_pins[]			= { GPIOC_4 };
static const unsigned int eth_debug_in12_pins[]			= { GPIOC_5 };
static const unsigned int eth_debug_in13_pins[]			= { GPIOC_6 };

/* GPIOC func7 */
static const unsigned int lvds_prod_o_10_pins[]			= { GPIOC_0 };
static const unsigned int lvds_prod_o_11_pins[]			= { GPIOC_1 };
static const unsigned int lvds_prod_o_12_pins[]			= { GPIOC_2 };
static const unsigned int lvds_prod_o_13_pins[]			= { GPIOC_3 };
static const unsigned int lvds_prod_o_14_pins[]			= { GPIOC_4 };
static const unsigned int lvds_prod_o_15_pins[]			= { GPIOC_5 };
static const unsigned int lvds_prod_o_16_pins[]			= { GPIOC_6 };
static const unsigned int lvds_prod_o_17_pins[]			= { GPIOC_7 };
static const unsigned int lvds_prod_o_18_pins[]			= { GPIOC_8 };
static const unsigned int lvds_prod_o_19_pins[]			= { GPIOC_9 };
static const unsigned int lvds_prod_o_20_pins[]			= { GPIOC_10 };

/* GPIOH func1 */
static const unsigned int tcon_0_pins[]				= { GPIOH_0 };
static const unsigned int tcon_1_pins[]				= { GPIOH_1 };
static const unsigned int tcon_2_pins[]				= { GPIOH_2 };
static const unsigned int tcon_3_pins[]				= { GPIOH_3 };
static const unsigned int tcon_4_pins[]				= { GPIOH_4 };
static const unsigned int tcon_5_pins[]				= { GPIOH_5 };
static const unsigned int tcon_6_pins[]				= { GPIOH_6 };
static const unsigned int tcon_7_pins[]				= { GPIOH_7 };
static const unsigned int tcon_8_pins[]				= { GPIOH_8 };
static const unsigned int tcon_9_pins[]				= { GPIOH_9 };
static const unsigned int tcon_10_pins[]			= { GPIOH_10 };
static const unsigned int tcon_11_pins[]			= { GPIOH_11 };
static const unsigned int tcon_12_pins[]			= { GPIOH_12 };
static const unsigned int tcon_13_pins[]			= { GPIOH_13 };
static const unsigned int tcon_14_pins[]			= { GPIOH_14 };
static const unsigned int tcon_15_pins[]			= { GPIOH_15 };

/* GPIOH func2 */
static const unsigned int tdm_mclk1_h0_pins[]			= { GPIOH_0 };
static const unsigned int tdm_sclk1_h1_pins[]			= { GPIOH_1 };
static const unsigned int tdm_fs1_h2_pins[]			= { GPIOH_2 };
static const unsigned int tdm_d2_h3_pins[]			= { GPIOH_3 };
static const unsigned int tdm_d3_h4_pins[]			= { GPIOH_4 };
static const unsigned int tdm_d4_h5_pins[]			= { GPIOH_5 };
static const unsigned int tdm_d5_h6_pins[]			= { GPIOH_6 };
static const unsigned int pwm_a_h7_pins[]			= { GPIOH_7 };
static const unsigned int i2cm_c_scl_h8_pins[]			= { GPIOH_8 };
static const unsigned int i2cm_c_sda_h9_pins[]			= { GPIOH_9 };
static const unsigned int i2cm_d_scl_h10_pins[]			= { GPIOH_10 };
static const unsigned int i2cm_d_sda_h11_pins[]			= { GPIOH_11 };
static const unsigned int uart_rts_c_pins[]			= { GPIOH_12 };
static const unsigned int uart_cts_c_pins[]			= { GPIOH_13 };
static const unsigned int uart_rx_c_pins[]			= { GPIOH_14 };
static const unsigned int uart_tx_c_pins[]			= { GPIOH_15 };

/* GPIOH func3 */
static const unsigned int tsin_clk_a_h0_pins[]			= { GPIOH_0 };
static const unsigned int tsin_d0_a_h1_pins[]			= { GPIOH_1 };
static const unsigned int tsin_valid_a_h2_pins[]		= { GPIOH_2 };
static const unsigned int tsin_sop_a_h3_pins[]			= { GPIOH_3 };
static const unsigned int sync_3d_out_pins[]			= { GPIOH_4 };
static const unsigned int tsin_d7_b_pins[]			= { GPIOH_5 };
static const unsigned int tsin_d6_b_pins[]			= { GPIOH_6 };
static const unsigned int tsin_d5_b_pins[]			= { GPIOH_7 };
static const unsigned int tsin_d4_b_pins[]			= { GPIOH_8 };
static const unsigned int tsin_d3_b_pins[]			= { GPIOH_9 };
static const unsigned int tsin_d2_b_pins[]			= { GPIOH_10 };
static const unsigned int tsin_d1_b_pins[]			= { GPIOH_11 };
static const unsigned int tsin_d0_b_pins[]			= { GPIOH_12 };
static const unsigned int tsin_clk_b_pins[]			= { GPIOH_13 };
static const unsigned int tsin_sop_b_pins[]			= { GPIOH_14 };
static const unsigned int tsin_valid_b_pins[]			= { GPIOH_15 };

/* GPIOH func4 */
static const unsigned int tdm_d4_h3_pins[]			= { GPIOH_3 };
static const unsigned int i2cm_a_scl_h4_pins[]			= { GPIOH_4 };
static const unsigned int i2cm_a_sda_h5_pins[]			= { GPIOH_5 };
static const unsigned int tdm_d11_pins[]			= { GPIOH_6 };
static const unsigned int tdm_d10_pins[]			= { GPIOH_7 };
static const unsigned int pwm_vs_h10_pins[]			= { GPIOH_10 };
static const unsigned int pwm_c_h11_pins[]			= { GPIOH_11 };
static const unsigned int tdm_d6_h12_pins[]			= { GPIOH_12 };
static const unsigned int tdm_d7_pins[]				= { GPIOH_13 };
static const unsigned int tdm_d8_pins[]				= { GPIOH_14 };
static const unsigned int tdm_d9_pins[]				= { GPIOH_15 };

/* GPIOH func5 */
static const unsigned int i2cm_d_scl_h0_pins[]			= { GPIOH_0 };
static const unsigned int i2cm_d_sda_h1_pins[]			= { GPIOH_1 };
static const unsigned int tdm_sclk2_h3_pins[]			= { GPIOH_3 };
static const unsigned int tdm_fs0_h4_pins[]			= { GPIOH_4 };
static const unsigned int tdm_d0_h5_pins[]			= { GPIOH_5 };
static const unsigned int tdm_d1_h6_pins[]			= { GPIOH_6 };
static const unsigned int tdm_sclk0_h7_pins[]			= { GPIOH_7 };
static const unsigned int pwm_e_h8_pins[]			= { GPIOH_8 };
static const unsigned int pwm_d_h9_pins[]			= { GPIOH_9 };

/* GPIOH func6 */
static const unsigned int tdm_mclk2_h0_pins[]			= { GPIOH_0 };
static const unsigned int tdm_sclk2_h1_pins[]			= { GPIOH_1 };
static const unsigned int tdm_fs2_h2_pins[]			= { GPIOH_2 };
static const unsigned int tdm_mclk2_h7_pins[]			= { GPIOH_7 };
static const unsigned int tdm_sclk2_h8_pins[]			= { GPIOH_8 };
static const unsigned int tdm_fs2_h9_pins[]			= { GPIOH_9 };
static const unsigned int debug_bus_out10_pins[]		= { GPIOH_10 };
static const unsigned int debug_bus_out11_pins[]		= { GPIOH_11 };
static const unsigned int debug_bus_out12_pins[]		= { GPIOH_12 };
static const unsigned int debug_bus_out13_pins[]		= { GPIOH_13 };
static const unsigned int debug_bus_out14_pins[]		= { GPIOH_14 };
static const unsigned int debug_bus_out15_pins[]		= { GPIOH_15 };

/* GPIOH func7 */
static const unsigned int tst_clk_out0_pins[]			= { GPIOH_0 };
static const unsigned int tst_clk_out1_pins[]			= { GPIOH_1 };
static const unsigned int tst_clk_out2_pins[]			= { GPIOH_2 };
static const unsigned int tst_clk_out3_pins[]			= { GPIOH_3 };
static const unsigned int tst_clk_out4_pins[]			= { GPIOH_4 };
static const unsigned int tst_clk_out5_pins[]			= { GPIOH_5 };
static const unsigned int tst_clk_out6_pins[]			= { GPIOH_6 };
static const unsigned int tst_clk_out7_pins[]			= { GPIOH_7 };
static const unsigned int tst_clk_out8_pins[]			= { GPIOH_8 };
static const unsigned int tst_clk_out9_pins[]			= { GPIOH_9 };
static const unsigned int tst_clk_out10_pins[]			= { GPIOH_10 };
static const unsigned int tst_clk_out11_pins[]			= { GPIOH_11 };
static const unsigned int tst_clk_out12_pins[]			= { GPIOH_12 };
static const unsigned int tst_clk_out13_pins[]			= { GPIOH_13 };
static const unsigned int tst_clk_out14_pins[]			= { GPIOH_14 };
static const unsigned int tst_clk_out15_pins[]			= { GPIOH_15 };

/* GPIOZ func1 */
static const unsigned int i2cm_b_sda_z0_pins[]			= { GPIOZ_0 };
static const unsigned int i2cm_b_scl_z1_pins[]			= { GPIOZ_1 };
static const unsigned int pwm_a_z5_pins[]			= { GPIOZ_5 };
static const unsigned int eth_link_led_pins[]			= { GPIOZ_6 };
static const unsigned int eth_act_led_pins[]			= { GPIOZ_7 };

/* GPIOZ func2 */
static const unsigned int tdm_d6_z0_pins[]			= { GPIOZ_0 };
static const unsigned int tdm_d3_z1_pins[]			= { GPIOZ_1 };
static const unsigned int tdm_d2_z2_pins[]			= { GPIOZ_2 };
static const unsigned int tdm_fs1_z3_pins[]			= { GPIOZ_3 };
static const unsigned int tdm_sclk1_z4_pins[]			= { GPIOZ_4 };
static const unsigned int tdm_mclk1_z5_pins[]			= { GPIOZ_5 };
static const unsigned int i2cm_a_sda_z6_pins[]			= { GPIOZ_6 };
static const unsigned int i2cm_a_scl_z7_pins[]			= { GPIOZ_7 };

/* GPIOZ func3 */
static const unsigned int pwm_vs_z0_pins[]			= { GPIOZ_0 };
static const unsigned int pwm_vs_z1_pins[]			= { GPIOZ_1 };
static const unsigned int dtv_rf_agc_z2_pins[]			= { GPIOZ_2 };
static const unsigned int uart_rts_b_pins[]			= { GPIOZ_4 };
static const unsigned int uart_cts_b_pins[]			= { GPIOZ_5 };
static const unsigned int uart_rx_b_pins[]			= { GPIOZ_6 };
static const unsigned int uart_tx_b_pins[]			= { GPIOZ_7 };

/* GPIOZ func4 */
static const unsigned int pwm_c_z0_pins[]			= { GPIOZ_0 };
static const unsigned int pwm_b_z1_pins[]			= { GPIOZ_1 };
static const unsigned int pwm_d_z2_pins[]			= { GPIOZ_2 };
static const unsigned int spdif_out_z4_pins[]			= { GPIOZ_4 };
static const unsigned int atv_if_agc_z5_pins[]			= { GPIOZ_5 };
static const unsigned int pwm_a_z7_pins[]			= { GPIOZ_7 };

/* GPIOZ func5 */
static const unsigned int pwm_f_pins[]				= { GPIOZ_3 };
static const unsigned int pwm_e_z4_pins[]			= { GPIOZ_4 };
static const unsigned int dtv_if_agc_z5_pins[]			= { GPIOZ_5 };
static const unsigned int uart_tx_a_z6_pins[]			= { GPIOZ_6 };
static const unsigned int uart_rx_a_z7_pins[]			= { GPIOZ_7 };

/* GPIOZ func6 */
static const unsigned int debug_bus_out16_pins[]		= { GPIOZ_0 };
static const unsigned int debug_bus_out17_pins[]		= { GPIOZ_1 };
static const unsigned int dcon_led_z5_pins[]			= { GPIOZ_5 };

/* GPIOZ func7 */
static const unsigned int tst_clk_out16_pins[]			= { GPIOZ_0 };
static const unsigned int tst_clk_out17_pins[]			= { GPIOZ_1 };
static const unsigned int tst_clk_out18_pins[]			= { GPIOZ_2 };
static const unsigned int tst_clk_out19_pins[]			= { GPIOZ_3 };

/* GPIOW func1 */
static const unsigned int hdmirx_a_hpd_pins[]			= { GPIOW_0 };
static const unsigned int hdmirx_a_5vdet_pins[]			= { GPIOW_1 };
static const unsigned int hdmirx_a_sda_pins[]			= { GPIOW_2 };
static const unsigned int hdmirx_a_scl_pins[]			= { GPIOW_3 };
static const unsigned int hdmirx_b_hpd_pins[]			= { GPIOW_4 };
static const unsigned int hdmirx_b_5vdet_pins[]			= { GPIOW_5 };
static const unsigned int hdmirx_b_sda_pins[]			= { GPIOW_6 };
static const unsigned int hdmirx_b_scl_pins[]			= { GPIOW_7 };

/* GPIOW func2 */
static const unsigned int uart_tx_ao_a_w2_pins[]		= { GPIOW_2 };
static const unsigned int uart_rx_ao_a_w3_pins[]		= { GPIOW_3 };
static const unsigned int uart_tx_ao_a_w6_pins[]		= { GPIOW_6 };
static const unsigned int uart_rx_ao_a_w7_pins[]		= { GPIOW_7 };

static struct meson_pmx_group meson_txhd2_periphs_groups[] __initdata = {
	/* func0 as GPIO */
	GPIO_GROUP(GPIODV_0),
	GPIO_GROUP(GPIODV_1),
	GPIO_GROUP(GPIODV_2),
	GPIO_GROUP(GPIODV_3),
	GPIO_GROUP(GPIODV_4),
	GPIO_GROUP(GPIODV_5),
	GPIO_GROUP(GPIODV_6),
	GPIO_GROUP(GPIODV_7),
	GPIO_GROUP(GPIODV_8),
	GPIO_GROUP(GPIODV_9),
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
	GPIO_GROUP(GPIOC_0),
	GPIO_GROUP(GPIOC_1),
	GPIO_GROUP(GPIOC_2),
	GPIO_GROUP(GPIOC_3),
	GPIO_GROUP(GPIOC_4),
	GPIO_GROUP(GPIOC_5),
	GPIO_GROUP(GPIOC_6),
	GPIO_GROUP(GPIOC_7),
	GPIO_GROUP(GPIOC_8),
	GPIO_GROUP(GPIOC_9),
	GPIO_GROUP(GPIOC_10),
	GPIO_GROUP(GPIOH_0),
	GPIO_GROUP(GPIOH_1),
	GPIO_GROUP(GPIOH_2),
	GPIO_GROUP(GPIOH_3),
	GPIO_GROUP(GPIOH_4),
	GPIO_GROUP(GPIOH_5),
	GPIO_GROUP(GPIOH_6),
	GPIO_GROUP(GPIOH_7),
	GPIO_GROUP(GPIOH_8),
	GPIO_GROUP(GPIOH_9),
	GPIO_GROUP(GPIOH_10),
	GPIO_GROUP(GPIOH_11),
	GPIO_GROUP(GPIOH_12),
	GPIO_GROUP(GPIOH_13),
	GPIO_GROUP(GPIOH_14),
	GPIO_GROUP(GPIOH_15),
	GPIO_GROUP(GPIOZ_0),
	GPIO_GROUP(GPIOZ_1),
	GPIO_GROUP(GPIOZ_2),
	GPIO_GROUP(GPIOZ_3),
	GPIO_GROUP(GPIOZ_4),
	GPIO_GROUP(GPIOZ_5),
	GPIO_GROUP(GPIOZ_6),
	GPIO_GROUP(GPIOZ_7),
	GPIO_GROUP(GPIOW_0),
	GPIO_GROUP(GPIOW_1),
	GPIO_GROUP(GPIOW_2),
	GPIO_GROUP(GPIOW_3),
	GPIO_GROUP(GPIOW_4),
	GPIO_GROUP(GPIOW_5),
	GPIO_GROUP(GPIOW_6),
	GPIO_GROUP(GPIOW_7),
	GPIO_GROUP(GPIOAO_0),
	GPIO_GROUP(GPIOAO_1),
	GPIO_GROUP(GPIOAO_2),
	GPIO_GROUP(GPIOAO_3),
	GPIO_GROUP(GPIOAO_4),
	GPIO_GROUP(GPIOAO_5),
	GPIO_GROUP(GPIOAO_6),
	GPIO_GROUP(GPIOAO_7),
	GPIO_GROUP(GPIOAO_8),
	GPIO_GROUP(GPIOAO_9),
	GPIO_GROUP(GPIOAO_10),
	GPIO_GROUP(GPIOAO_11),
	GPIO_GROUP(GPIOAO_12),
	GPIO_GROUP(GPIOAO_13),

	/* GPIODV func1 */
	GROUP(i2cm_b_sda_dv0,				1),
	GROUP(i2cm_b_scl_dv1,				1),
	GROUP(dtv_if_agc_dv2,				1),
	GROUP(i2cm_a_sda_dv4,				1),
	GROUP(i2cm_a_scl_dv5,				1),
	GROUP(tsin_d0_a_dv6,				1),
	GROUP(tsin_clk_a_dv7,				1),
	GROUP(tsin_sop_a_dv8,				1),
	GROUP(tsin_valid_a_dv9,				1),

	/* GPIODV func2 */
	GROUP(atv_if_agc_dv2,				2),
	GROUP(dtv_rf_agc_dv3,				2),
	GROUP(uart_rts_a_dv6,				2),
	GROUP(uart_cts_a_dv7,				2),
	GROUP(uart_tx_a_dv8,				2),
	GROUP(uart_rx_a_dv9,				2),

	/* GPIODV func3 */
	GROUP(spi_ss3_a,				3),
	GROUP(spi_ss2_a,				3),
	GROUP(spi_ss1_a_dv5,				3),
	GROUP(spi_ss0_a_dv6,				3),
	GROUP(spi_miso_a_dv7,				3),
	GROUP(spi_mosi_a_dv8,				3),
	GROUP(spi_clk_a_dv9,				3),

	/* GPIODV func4 */
	GROUP(pwm_d_dv2,				4),
	GROUP(spdif_out_dv3,				4),
	GROUP(pwm_a_dv4,				4),
	GROUP(pwm_vs_dv5,				4),
	GROUP(pdm_din0_dv6,				4),
	GROUP(pdm_dclk_dv7,				4),

	/* GPIODV func5 */
	GROUP(debug_bus_out0,				5),
	GROUP(debug_bus_out1,				5),
	GROUP(debug_bus_out2,				5),
	GROUP(debug_bus_out3,				5),
	GROUP(debug_bus_out4,				5),
	GROUP(debug_bus_out5,				5),
	GROUP(debug_bus_out6,				5),
	GROUP(debug_bus_out7,				5),
	GROUP(debug_bus_out8,				5),
	GROUP(debug_bus_out9,				5),

	/* GPIODV func6 */
	GROUP(eth_debug_in0,				6),
	GROUP(eth_debug_in1,				6),
	GROUP(eth_debug_in2,				6),
	GROUP(eth_debug_in3,				6),
	GROUP(eth_debug_in4,				6),
	GROUP(eth_debug_in5,				6),
	GROUP(eth_debug_in6,				6),
	GROUP(eth_debug_in7,				6),
	GROUP(eth_debug_in8,				6),
	GROUP(eth_debug_in9,				6),

	/* GPIODV func7 */
	GROUP(lvds_prod_o_0,				7),
	GROUP(lvds_prod_o_1,				7),
	GROUP(lvds_prod_o_2,				7),
	GROUP(lvds_prod_o_3,				7),
	GROUP(lvds_prod_o_4,				7),
	GROUP(lvds_prod_o_5,				7),
	GROUP(lvds_prod_o_6,				7),
	GROUP(lvds_prod_o_7,				7),
	GROUP(lvds_prod_o_8,				7),
	GROUP(lvds_prod_o_9,				7),

	/* GPIOB func1 */
	GROUP(emmc_d0,					1),
	GROUP(emmc_d1,					1),
	GROUP(emmc_d2,					1),
	GROUP(emmc_d3,					1),
	GROUP(emmc_d4,					1),
	GROUP(emmc_d5,					1),
	GROUP(emmc_d6,					1),
	GROUP(emmc_d7,					1),
	GROUP(emmc_clk,					1),
	GROUP(emmc_cmd,					1),
	GROUP(emmc_ds,					1),

	/* GPIOB func3 */
	GROUP(spif_hold,				3),
	GROUP(spif_mo,					3),
	GROUP(spif_mi,					3),
	GROUP(spif_clk,					3),
	GROUP(spif_wp,					3),
	GROUP(spif_cs,					3),

	/* GPIOC func1 */
	GROUP(tsin_sop_a_c7,				1),
	GROUP(tsin_valid_a_c8,				1),
	GROUP(tsin_d0_a_c9,				1),
	GROUP(tsin_clk_a_c10,				1),

	/* GPIOC func2 */
	GROUP(jtag_tdo_c0,				2),
	GROUP(jtag_tdi_c1,				2),
	GROUP(uart_rx_ao_a_c2,				2),
	GROUP(uart_tx_ao_a_c3,				2),
	GROUP(jtag_clk_c4,				2),
	GROUP(jtag_tms_c5,				2),
	GROUP(uart_rx_a_c7,				2),
	GROUP(uart_tx_a_c8,				2),
	GROUP(uart_cts_a_c9,				2),
	GROUP(uart_rts_a_c10,				2),

	/* GPIOC func3 */
	GROUP(uart_tx_ao_a_c2,				3),
	GROUP(uart_rx_ao_a_c3,				3),
	GROUP(tdm_mclk0,				3),
	GROUP(tdm_fs0_c7,				3),
	GROUP(tdm_d0_c8,				3),
	GROUP(tdm_d1_c9,				3),
	GROUP(tdm_sclk0_c10,				3),

	/* GPIOC func4 */
	GROUP(tdm_sclk1_c0,				4),
	GROUP(tdm_mclk1_c1,				4),
	GROUP(tdm_d3_c2,				4),
	GROUP(tdm_d5_c3,				4),
	GROUP(tdm_d2_c4,				4),
	GROUP(tdm_fs1_c5,				4),
	GROUP(spi_ss1_a_c6,				4),
	GROUP(spi_ss0_a_c7,				4),
	GROUP(spi_mosi_a_c8,				4),
	GROUP(spi_miso_a_c9,				4),
	GROUP(spi_clk_a_c10,				4),

	/* GPIOC func5 */
	GROUP(gen_clk_out,				5),
	GROUP(i2cm_c_sda_c2,				5),
	GROUP(i2cm_c_scl_c3,				5),
	GROUP(tdm_d4_c4,				5),
	GROUP(pwm_d_c6,					5),
	GROUP(pwm_b_c9,					5),
	GROUP(pwm_c_c10,				5),

	/* GPIOC func6 */
	GROUP(pdm_dclk_c1,				6),
	GROUP(pdm_din0_c2,				6),
	GROUP(eth_debug_in10,				6),
	GROUP(eth_debug_in11,				6),
	GROUP(eth_debug_in12,				6),
	GROUP(eth_debug_in13,				6),

	/* GPIOC func7 */
	GROUP(lvds_prod_o_10,				7),
	GROUP(lvds_prod_o_11,				7),
	GROUP(lvds_prod_o_12,				7),
	GROUP(lvds_prod_o_13,				7),
	GROUP(lvds_prod_o_14,				7),
	GROUP(lvds_prod_o_15,				7),
	GROUP(lvds_prod_o_16,				7),
	GROUP(lvds_prod_o_17,				7),
	GROUP(lvds_prod_o_18,				7),
	GROUP(lvds_prod_o_19,				7),
	GROUP(lvds_prod_o_20,				7),

	/* GPIOH func1 */
	GROUP(tcon_0,					1),
	GROUP(tcon_1,					1),
	GROUP(tcon_2,					1),
	GROUP(tcon_3,					1),
	GROUP(tcon_4,					1),
	GROUP(tcon_5,					1),
	GROUP(tcon_6,					1),
	GROUP(tcon_7,					1),
	GROUP(tcon_8,					1),
	GROUP(tcon_9,					1),
	GROUP(tcon_10,					1),
	GROUP(tcon_11,					1),
	GROUP(tcon_12,					1),
	GROUP(tcon_13,					1),
	GROUP(tcon_14,					1),
	GROUP(tcon_15,					1),

	/* GPIOH func2 */
	GROUP(tdm_mclk1_h0,				2),
	GROUP(tdm_sclk1_h1,				2),
	GROUP(tdm_fs1_h2,				2),
	GROUP(tdm_d2_h3,				2),
	GROUP(tdm_d3_h4,				2),
	GROUP(tdm_d4_h5,				2),
	GROUP(tdm_d5_h6,				2),
	GROUP(pwm_a_h7,					2),
	GROUP(i2cm_c_scl_h8,				2),
	GROUP(i2cm_c_sda_h9,				2),
	GROUP(i2cm_d_scl_h10,				2),
	GROUP(i2cm_d_sda_h11,				2),
	GROUP(uart_rts_c,				2),
	GROUP(uart_cts_c,				2),
	GROUP(uart_rx_c,				2),
	GROUP(uart_tx_c,				2),

	/* GPIOH func3 */
	GROUP(tsin_clk_a_h0,				3),
	GROUP(tsin_d0_a_h1,				3),
	GROUP(tsin_valid_a_h2,				3),
	GROUP(tsin_sop_a_h3,				3),
	GROUP(sync_3d_out,				3),
	GROUP(tsin_d7_b,				3),
	GROUP(tsin_d6_b,				3),
	GROUP(tsin_d5_b,				3),
	GROUP(tsin_d4_b,				3),
	GROUP(tsin_d3_b,				3),
	GROUP(tsin_d2_b,				3),
	GROUP(tsin_d1_b,				3),
	GROUP(tsin_d0_b,				3),
	GROUP(tsin_clk_b,				3),
	GROUP(tsin_sop_b,				3),
	GROUP(tsin_valid_b,				3),

	/* GPIOH func4 */
	GROUP(tdm_d4_h3,				4),
	GROUP(i2cm_a_scl_h4,				4),
	GROUP(i2cm_a_sda_h5,				4),
	GROUP(tdm_d11,					4),
	GROUP(tdm_d10,					4),
	GROUP(pwm_vs_h10,				4),
	GROUP(pwm_c_h11,				4),
	GROUP(tdm_d6_h12,				4),
	GROUP(tdm_d7,					4),
	GROUP(tdm_d8,					4),
	GROUP(tdm_d9,					4),

	/* GPIOH func5 */
	GROUP(i2cm_d_scl_h0,				5),
	GROUP(i2cm_d_sda_h1,				5),
	GROUP(tdm_sclk2_h3,				5),
	GROUP(tdm_fs0_h4,				5),
	GROUP(tdm_d0_h5,				5),
	GROUP(tdm_d1_h6,				5),
	GROUP(tdm_sclk0_h7,				5),
	GROUP(pwm_e_h8,					5),
	GROUP(pwm_d_h9,					5),

	/* GPIOH func6 */
	GROUP(tdm_mclk2_h0,				6),
	GROUP(tdm_sclk2_h1,				6),
	GROUP(tdm_fs2_h2,				6),
	GROUP(tdm_mclk2_h7,				6),
	GROUP(tdm_sclk2_h8,				6),
	GROUP(tdm_fs2_h9,				6),
	GROUP(debug_bus_out10,				6),
	GROUP(debug_bus_out11,				6),
	GROUP(debug_bus_out12,				6),
	GROUP(debug_bus_out13,				6),
	GROUP(debug_bus_out14,				6),
	GROUP(debug_bus_out15,				6),

	/* GPIOH func7 */
	GROUP(tst_clk_out0,				7),
	GROUP(tst_clk_out1,				7),
	GROUP(tst_clk_out2,				7),
	GROUP(tst_clk_out3,				7),
	GROUP(tst_clk_out4,				7),
	GROUP(tst_clk_out5,				7),
	GROUP(tst_clk_out6,				7),
	GROUP(tst_clk_out7,				7),
	GROUP(tst_clk_out8,				7),
	GROUP(tst_clk_out9,				7),
	GROUP(tst_clk_out10,				7),
	GROUP(tst_clk_out11,				7),
	GROUP(tst_clk_out12,				7),
	GROUP(tst_clk_out13,				7),
	GROUP(tst_clk_out14,				7),
	GROUP(tst_clk_out15,				7),

	/* GPIOZ func1 */
	GROUP(i2cm_b_sda_z0,				1),
	GROUP(i2cm_b_scl_z1,				1),
	GROUP(pwm_a_z5,					1),
	GROUP(eth_link_led,				1),
	GROUP(eth_act_led,				1),

	/* GPIOZ func2 */
	GROUP(tdm_d6_z0,				2),
	GROUP(tdm_d3_z1,				2),
	GROUP(tdm_d2_z2,				2),
	GROUP(tdm_fs1_z3,				2),
	GROUP(tdm_sclk1_z4,				2),
	GROUP(tdm_mclk1_z5,				2),
	GROUP(i2cm_a_sda_z6,				2),
	GROUP(i2cm_a_scl_z7,				2),

	/* GPIOZ func3 */
	GROUP(pwm_vs_z0,				3),
	GROUP(pwm_vs_z1,				3),
	GROUP(dtv_rf_agc_z2,				3),
	GROUP(uart_rts_b,				3),
	GROUP(uart_cts_b,				3),
	GROUP(uart_rx_b,				3),
	GROUP(uart_tx_b,				3),

	/* GPIOZ func4 */
	GROUP(pwm_c_z0,					4),
	GROUP(pwm_b_z1,					4),
	GROUP(pwm_d_z2,					4),
	GROUP(spdif_out_z4,				4),
	GROUP(atv_if_agc_z5,				4),
	GROUP(pwm_a_z7,					4),

	/* GPIOZ func5 */
	GROUP(pwm_f,					5),
	GROUP(pwm_e_z4,					5),
	GROUP(dtv_if_agc_z5,				5),
	GROUP(uart_tx_a_z6,				5),
	GROUP(uart_rx_a_z7,				5),

	/* GPIOZ func6 */
	GROUP(debug_bus_out16,				6),
	GROUP(debug_bus_out17,				6),
	GROUP(dcon_led_z5,				6),

	/* GPIOZ func7 */
	GROUP(tst_clk_out16,				7),
	GROUP(tst_clk_out17,				7),
	GROUP(tst_clk_out18,				7),
	GROUP(tst_clk_out19,				7),

	/* GPIOW func1 */
	GROUP(hdmirx_a_hpd,				1),
	GROUP(hdmirx_a_5vdet,				1),
	GROUP(hdmirx_a_sda,				1),
	GROUP(hdmirx_a_scl,				1),
	GROUP(hdmirx_b_hpd,				1),
	GROUP(hdmirx_b_5vdet,				1),
	GROUP(hdmirx_b_sda,				1),
	GROUP(hdmirx_b_scl,				1),

	/* GPIOW func2 */
	GROUP(uart_tx_ao_a_w2,				2),
	GROUP(uart_rx_ao_a_w3,				2),
	GROUP(uart_tx_ao_a_w6,				2),
	GROUP(uart_rx_ao_a_w7,				2),
};

/* GPIOAO func1 */
static const unsigned int uart_tx_ao_a_ao0_pins[]		= { GPIOAO_0 };
static const unsigned int uart_rx_ao_a_ao1_pins[]		= { GPIOAO_1 };
static const unsigned int uart_cts_ao_a_pins[]			= { GPIOAO_2 };
static const unsigned int uart_rts_ao_a_pins[]			= { GPIOAO_3 };
static const unsigned int i2cm_ao_a_scl_ao4_pins[]		= { GPIOAO_4 };
static const unsigned int i2cm_ao_a_sda_ao5_pins[]		= { GPIOAO_5 };
static const unsigned int remote_input_ao6_pins[]		= { GPIOAO_6 };
static const unsigned int i2cm_ao_a_scl_ao10_pins[]		= { GPIOAO_10 };
static const unsigned int i2cm_ao_a_sda_ao11_pins[]		= { GPIOAO_11 };
static const unsigned int gen_clk_ee_ao13_pins[]		= { GPIOAO_13 };

/* GPIOAO func2 */
static const unsigned int uart_tx_ao_b_ao0_pins[]		= { GPIOAO_0 };
static const unsigned int uart_rx_ao_b_ao1_pins[]		= { GPIOAO_1 };
static const unsigned int uart_cts_ao_b_pins[]			= { GPIOAO_2 };
static const unsigned int uart_rts_ao_b_pins[]			= { GPIOAO_3 };
static const unsigned int uart_tx_ao_b_ao4_pins[]		= { GPIOAO_4 };
static const unsigned int uart_rx_ao_b_ao5_pins[]		= { GPIOAO_5 };
static const unsigned int remote_out_ao6_pins[]			= { GPIOAO_6 };
static const unsigned int cec_ao_b_ao7_pins[]			= { GPIOAO_7 };
static const unsigned int cec_ao_b_ao8_pins[]			= { GPIOAO_8 };
static const unsigned int uart_tx_ao_b_ao10_pins[]		= { GPIOAO_10 };
static const unsigned int uart_rx_ao_b_ao11_pins[]		= { GPIOAO_11 };

/* GPIOAO func3 */
static const unsigned int jtag_tdi_ao3_pins[]			= { GPIOAO_3 };
static const unsigned int i2cs_ao_a_scl_ao4_pins[]		= { GPIOAO_4 };
static const unsigned int i2cs_ao_a_sda_ao5_pins[]		= { GPIOAO_5 };
static const unsigned int jtag_tms_ao7_pins[]			= { GPIOAO_7 };
static const unsigned int clk_32k_in_pins[]			= { GPIOAO_8 };
static const unsigned int i2cs_ao_a_scl_ao10_pins[]		= { GPIOAO_10 };
static const unsigned int i2cs_ao_a_sda_ao11_pins[]		= { GPIOAO_11 };

/* GPIOAO func4 */
static const unsigned int pwm_ao_d_ao2_pins[]			= { GPIOAO_2 };
static const unsigned int pwm_ao_a_ao3_pins[]			= { GPIOAO_3 };
static const unsigned int jtag_tdo_ao4_pins[]			= { GPIOAO_4 };
static const unsigned int jtag_clk_ao5_pins[]			= { GPIOAO_5 };
static const unsigned int pwm_ao_a_ao7_pins[]			= { GPIOAO_7 };
static const unsigned int pwm_ao_c_ao8_pins[]			= { GPIOAO_8 };
static const unsigned int pwm_ao_b_pins[]			= { GPIOAO_9 };
static const unsigned int remote_input_ao10_pins[]		= { GPIOAO_10 };
static const unsigned int remote_out_ao11_pins[]		= { GPIOAO_11 };
static const unsigned int pwm_ao_c_ao12_pins[]			= { GPIOAO_12 };
static const unsigned int pwm_ao_d_ao13_pins[]			= { GPIOAO_13 };

/* GPIOAO func5 */
static const unsigned int pwm_ao_a_hiz_pins[]			= { GPIOAO_3 };
static const unsigned int pdm_dclk_ao4_pins[]			= { GPIOAO_4 };
static const unsigned int pdm_din0_ao5_pins[]			= { GPIOAO_5 };
static const unsigned int pwm_ao_c_hiz_pins[]			= { GPIOAO_8 };
static const unsigned int gen_clk_ao_pins[]			= { GPIOAO_11 };

/* GPIOAO func6 */
static const unsigned int dcon_led_ao6_pins[]			= { GPIOAO_6 };
static const unsigned int dcon_led_ao7_pins[]			= { GPIOAO_7 };
static const unsigned int dcon_led_ao8_pins[]			= { GPIOAO_8 };
static const unsigned int gen_clk_ee_ao11_pins[]		= { GPIOAO_11 };
static const unsigned int pdm_din0_ao12_pins[]			= { GPIOAO_12 };
static const unsigned int pdm_dclk_ao13_pins[]			= { GPIOAO_13 };

static struct meson_pmx_group meson_txhd2_aobus_groups[] __initdata = {
	/* func0 as GPIO */
	GPIO_GROUP(GPIOAO_0),
	GPIO_GROUP(GPIOAO_1),
	GPIO_GROUP(GPIOAO_2),
	GPIO_GROUP(GPIOAO_3),
	GPIO_GROUP(GPIOAO_4),
	GPIO_GROUP(GPIOAO_5),
	GPIO_GROUP(GPIOAO_6),
	GPIO_GROUP(GPIOAO_7),
	GPIO_GROUP(GPIOAO_8),
	GPIO_GROUP(GPIOAO_9),
	GPIO_GROUP(GPIOAO_10),
	GPIO_GROUP(GPIOAO_11),
	GPIO_GROUP(GPIOAO_12),
	GPIO_GROUP(GPIOAO_13),

	/* GPIOAO func1 */
	GROUP(uart_tx_ao_a_ao0,				1),
	GROUP(uart_rx_ao_a_ao1,				1),
	GROUP(uart_cts_ao_a,				1),
	GROUP(uart_rts_ao_a,				1),
	GROUP(i2cm_ao_a_scl_ao4,			1),
	GROUP(i2cm_ao_a_sda_ao5,			1),
	GROUP(remote_input_ao6,				1),
	GROUP(i2cm_ao_a_scl_ao10,			1),
	GROUP(i2cm_ao_a_sda_ao11,			1),
	GROUP(gen_clk_ee_ao13,				1),

	/* GPIOAO func2 */
	GROUP(uart_tx_ao_b_ao0,				2),
	GROUP(uart_rx_ao_b_ao1,				2),
	GROUP(uart_cts_ao_b,				2),
	GROUP(uart_rts_ao_b,				2),
	GROUP(uart_tx_ao_b_ao4,				2),
	GROUP(uart_rx_ao_b_ao5,				2),
	GROUP(remote_out_ao6,				2),
	GROUP(cec_ao_b_ao7,				2),
	GROUP(cec_ao_b_ao8,				2),
	GROUP(uart_tx_ao_b_ao10,			2),
	GROUP(uart_rx_ao_b_ao11,			2),

	/* GPIOAO func3 */
	GROUP(jtag_tdi_ao3,				3),
	GROUP(i2cs_ao_a_scl_ao4,			3),
	GROUP(i2cs_ao_a_sda_ao5,			3),
	GROUP(jtag_tms_ao7,				3),
	GROUP(clk_32k_in,				3),
	GROUP(i2cs_ao_a_scl_ao10,			3),
	GROUP(i2cs_ao_a_sda_ao11,			3),

	/* GPIOAO func4 */
	GROUP(pwm_ao_d_ao2,				4),
	GROUP(pwm_ao_a_ao3,				4),
	GROUP(jtag_tdo_ao4,				4),
	GROUP(jtag_clk_ao5,				4),
	GROUP(pwm_ao_a_ao7,				4),
	GROUP(pwm_ao_c_ao8,				4),
	GROUP(pwm_ao_b,					4),
	GROUP(remote_input_ao10,			4),
	GROUP(remote_out_ao11,				4),
	GROUP(pwm_ao_c_ao12,				4),
	GROUP(pwm_ao_d_ao13,				4),

	/* GPIOAO func5 */
	GROUP(pwm_ao_a_hiz,				5),
	GROUP(pdm_dclk_ao4,				5),
	GROUP(pdm_din0_ao5,				5),
	GROUP(pwm_ao_c_hiz,				5),
	GROUP(gen_clk_ao,				5),

	/* GPIOAO func6 */
	GROUP(dcon_led_ao6,				6),
	GROUP(dcon_led_ao7,				6),
	GROUP(dcon_led_ao8,				6),
	GROUP(gen_clk_ee_ao11,				6),
	GROUP(pdm_din0_ao12,				6),
	GROUP(pdm_dclk_ao13,				6),
};

static const char * const gpio_periphs_groups[] = {
	"GPIODV_0", "GPIODV_1", "GPIODV_2", "GPIODV_3",
	"GPIODV_4", "GPIODV_5", "GPIODV_6", "GPIODV_7",
	"GPIODV_8", "GPIODV_9", "GPIOB_0", "GPIOB_1",
	"GPIOB_2", "GPIOB_3", "GPIOB_4", "GPIOB_5",
	"GPIOB_6", "GPIOB_7", "GPIOB_8", "GPIOB_9",
	"GPIOB_10", "GPIOB_11", "GPIOB_12", "GPIOC_0",
	"GPIOC_1", "GPIOC_2", "GPIOC_3", "GPIOC_4",
	"GPIOC_5", "GPIOC_6", "GPIOC_7", "GPIOC_8",
	"GPIOC_9", "GPIOC_10", "GPIOH_0", "GPIOH_1",
	"GPIOH_2", "GPIOH_3", "GPIOH_4", "GPIOH_5",
	"GPIOH_6", "GPIOH_7", "GPIOH_8", "GPIOH_9",
	"GPIOH_10", "GPIOH_11", "GPIOH_12", "GPIOH_13",
	"GPIOH_14", "GPIOH_15", "GPIOZ_0", "GPIOZ_1",
	"GPIOZ_2", "GPIOZ_3", "GPIOZ_4", "GPIOZ_5",
	"GPIOZ_6", "GPIOZ_7", "GPIOW_0", "GPIOW_1",
	"GPIOW_2", "GPIOW_3", "GPIOW_4", "GPIOW_5",
	"GPIOW_6", "GPIOW_7",
};

static const char * const gpio_aobus_groups[] = {
	"GPIOAO_0", "GPIOAO_1", "GPIOAO_2", "GPIOAO_3",
	"GPIOAO_4", "GPIOAO_5", "GPIOAO_6", "GPIOAO_7",
	"GPIOAO_8", "GPIOAO_9", "GPIOAO_10", "GPIOAO_11",
	"GPIOAO_12", "GPIOAO_13",
};

static const char * const sync_3d_out_groups[] = {
	"sync_3d_out",
};

static const char * const atv_if_agc_groups[] = {
	"atv_if_agc_dv2", "atv_if_agc_z5",
};

static const char * const cec_ao_b_groups[] = {
	"cec_ao_b_ao7", "cec_ao_b_ao8",
};

static const char * const clk_32k_in_groups[] = {
	"clk_32k_in",
};

static const char * const dcon_led_groups[] = {
	"dcon_led_z5",
};

static const char * const dcon_led_ao_groups[] = {
	"dcon_led_ao6", "dcon_led_ao7", "dcon_led_ao8",
};

static const char * const debug_bus_out_groups[] = {
	"debug_bus_out0", "debug_bus_out1", "debug_bus_out2", "debug_bus_out3",
	"debug_bus_out4", "debug_bus_out5", "debug_bus_out6", "debug_bus_out7",
	"debug_bus_out8", "debug_bus_out9", "debug_bus_out10", "debug_bus_out11",
	"debug_bus_out12", "debug_bus_out13", "debug_bus_out14", "debug_bus_out15",
	"debug_bus_out16", "debug_bus_out17",
};

static const char * const dtv_groups[] = {
	"dtv_if_agc_dv2", "dtv_rf_agc_dv3", "dtv_rf_agc_z2", "dtv_if_agc_z5",
};

static const char * const emmc_groups[] = {
	"emmc_d0", "emmc_d1", "emmc_d2", "emmc_d3",
	"emmc_d4", "emmc_d5", "emmc_d6", "emmc_d7",
	"emmc_clk", "emmc_cmd", "emmc_ds",
};

static const char * const eth_groups[] = {
	"eth_debug_in0", "eth_debug_in1", "eth_debug_in2", "eth_debug_in3",
	"eth_debug_in4", "eth_debug_in5", "eth_debug_in6", "eth_debug_in7",
	"eth_debug_in8", "eth_debug_in9", "eth_debug_in10", "eth_debug_in11",
	"eth_debug_in12", "eth_debug_in13", "eth_link_led", "eth_act_led",
};

static const char * const gen_clk_groups[] = {
	"gen_clk_out", "gen_clk_ao", "gen_clk_ee_ao11", "gen_clk_ee_ao13",
};

static const char * const hdmirx_a_groups[] = {
	"hdmirx_a_hpd", "hdmirx_a_5vdet", "hdmirx_a_sda", "hdmirx_a_scl",
};

static const char * const hdmirx_b_groups[] = {
	"hdmirx_b_hpd", "hdmirx_b_5vdet", "hdmirx_b_sda", "hdmirx_b_scl",
};

static const char * const i2cm_ao_a_groups[] = {
	"i2cm_ao_a_scl_ao4", "i2cm_ao_a_sda_ao5", "i2cm_ao_a_scl_ao10", "i2cm_ao_a_sda_ao11",
};

static const char * const i2cs_ao_a_groups[] = {
	"i2cs_ao_a_scl_ao4", "i2cs_ao_a_sda_ao5", "i2cs_ao_a_scl_ao10", "i2cs_ao_a_sda_ao11",
};

static const char * const i2cm_a_groups[] = {
	"i2cm_a_sda_dv4", "i2cm_a_scl_dv5", "i2cm_a_scl_h4", "i2cm_a_sda_h5",
	"i2cm_a_sda_z6", "i2cm_a_scl_z7",
};

static const char * const i2cm_b_groups[] = {
	"i2cm_b_sda_dv0", "i2cm_b_scl_dv1", "i2cm_b_sda_z0", "i2cm_b_scl_z1",
};

static const char * const i2cm_c_groups[] = {
	"i2cm_c_sda_c2", "i2cm_c_scl_c3", "i2cm_c_scl_h8", "i2cm_c_sda_h9",
};

static const char * const i2cm_d_groups[] = {
	"i2cm_d_scl_h0", "i2cm_d_sda_h1", "i2cm_d_scl_h10", "i2cm_d_sda_h11",
};

static const char * const jtag_groups[] = {
	"jtag_tdo_c0", "jtag_tdi_c1", "jtag_clk_c4", "jtag_tms_c5",
};

static const char * const jtag_ao_groups[] = {
	"jtag_tdi_ao3", "jtag_tdo_ao4", "jtag_clk_ao5", "jtag_tms_ao7",
};

static const char * const lvds_groups[] = {
	"lvds_prod_o_0", "lvds_prod_o_1", "lvds_prod_o_2", "lvds_prod_o_3",
	"lvds_prod_o_4", "lvds_prod_o_5", "lvds_prod_o_6", "lvds_prod_o_7",
	"lvds_prod_o_8", "lvds_prod_o_9", "lvds_prod_o_10", "lvds_prod_o_11",
	"lvds_prod_o_12", "lvds_prod_o_13", "lvds_prod_o_14", "lvds_prod_o_15",
	"lvds_prod_o_16", "lvds_prod_o_17", "lvds_prod_o_18", "lvds_prod_o_19",
	"lvds_prod_o_20",
};

static const char * const pdm_groups[] = {
	"pdm_din0_dv6", "pdm_dclk_dv7", "pdm_dclk_c1", "pdm_din0_c2",
};

static const char * const pdm_ao_groups[] = {
	"pdm_dclk_ao4", "pdm_din0_ao5", "pdm_din0_ao12", "pdm_dclk_ao13",
};

static const char * const pwm_ao_a_hiz_groups[] = {
	"pwm_ao_a_hiz",
};

static const char * const pwm_ao_c_hiz_groups[] = {
	"pwm_ao_c_hiz",
};

static const char * const pwm_ao_a_groups[] = {
	"pwm_ao_a_ao3", "pwm_ao_a_ao7",
};

static const char * const pwm_ao_b_groups[] = {
	"pwm_ao_b",
};

static const char * const pwm_ao_c_groups[] = {
	"pwm_ao_c_ao8", "pwm_ao_c_ao12",
};

static const char * const pwm_ao_d_groups[] = {
	"pwm_ao_d_ao2", "pwm_ao_d_ao13",
};

static const char * const pwm_a_groups[] = {
	"pwm_a_dv4", "pwm_a_h7", "pwm_a_z5", "pwm_a_z7",
};

static const char * const pwm_b_groups[] = {
	"pwm_b_c9", "pwm_b_z1",
};

static const char * const pwm_c_groups[] = {
	"pwm_c_c10", "pwm_c_h11", "pwm_c_z0",
};

static const char * const pwm_d_groups[] = {
	"pwm_d_dv2", "pwm_d_c6", "pwm_d_h9", "pwm_d_z2",
};

static const char * const pwm_e_groups[] = {
	"pwm_e_h8", "pwm_e_z4",
};

static const char * const pwm_f_groups[] = {
	"pwm_f",
};

static const char * const pwm_vs_groups[] = {
	"pwm_vs_dv5", "pwm_vs_h10", "pwm_vs_z0", "pwm_vs_z1",
};

static const char * const remote_groups[] = {
	"remote_input_ao6", "remote_out_ao6", "remote_input_ao10", "remote_out_ao11",
};

static const char * const spdif_groups[] = {
	"spdif_out_dv3", "spdif_out_z4",
};

static const char * const spif_groups[] = {
	"spif_hold", "spif_mo", "spif_mi", "spif_clk",
	"spif_wp", "spif_cs",
};

static const char * const spi_groups[] = {
	"spi_ss3_a", "spi_ss2_a", "spi_ss1_a_dv5", "spi_ss0_a_dv6",
	"spi_miso_a_dv7", "spi_mosi_a_dv8", "spi_clk_a_dv9", "spi_ss1_a_c6",
	"spi_ss0_a_c7", "spi_mosi_a_c8", "spi_miso_a_c9", "spi_clk_a_c10",
};

static const char * const tcon_groups[] = {
	"tcon_0", "tcon_1", "tcon_2", "tcon_3",
	"tcon_4", "tcon_5", "tcon_6", "tcon_7",
	"tcon_8", "tcon_9", "tcon_10", "tcon_11",
	"tcon_12", "tcon_13", "tcon_14", "tcon_15",
};

static const char * const tdm_groups[] = {
	"tdm_sclk1_c0", "tdm_mclk1_c1", "tdm_d3_c2", "tdm_d5_c3",
	"tdm_d2_c4", "tdm_d4_c4", "tdm_fs1_c5", "tdm_mclk0",
	"tdm_fs0_c7", "tdm_d0_c8", "tdm_d1_c9", "tdm_sclk0_c10",
	"tdm_mclk1_h0", "tdm_mclk2_h0", "tdm_sclk1_h1", "tdm_sclk2_h1",
	"tdm_fs1_h2", "tdm_fs2_h2", "tdm_d2_h3", "tdm_d4_h3",
	"tdm_sclk2_h3", "tdm_d3_h4", "tdm_fs0_h4", "tdm_d4_h5",
	"tdm_d0_h5", "tdm_d5_h6", "tdm_d11", "tdm_d1_h6",
	"tdm_d10", "tdm_sclk0_h7", "tdm_mclk2_h7", "tdm_sclk2_h8",
	"tdm_fs2_h9", "tdm_d6_h12", "tdm_d7", "tdm_d8",
	"tdm_d9", "tdm_d6_z0", "tdm_d3_z1", "tdm_d2_z2",
	"tdm_fs1_z3", "tdm_sclk1_z4", "tdm_mclk1_z5",
};

static const char * const tsin_groups[] = {
	"tsin_d0_a_dv6", "tsin_clk_a_dv7", "tsin_sop_a_dv8", "tsin_valid_a_dv9",
	"tsin_sop_a_c7", "tsin_valid_a_c8", "tsin_d0_a_c9", "tsin_clk_a_c10",
	"tsin_clk_a_h0", "tsin_d0_a_h1", "tsin_valid_a_h2", "tsin_sop_a_h3",
	"tsin_d7_b", "tsin_d6_b", "tsin_d5_b", "tsin_d4_b",
	"tsin_d3_b", "tsin_d2_b", "tsin_d1_b", "tsin_d0_b",
	"tsin_clk_b", "tsin_sop_b", "tsin_valid_b",
};

static const char * const tst_groups[] = {
	"tst_clk_out0", "tst_clk_out1", "tst_clk_out2", "tst_clk_out3",
	"tst_clk_out4", "tst_clk_out5", "tst_clk_out6", "tst_clk_out7",
	"tst_clk_out8", "tst_clk_out9", "tst_clk_out10", "tst_clk_out11",
	"tst_clk_out12", "tst_clk_out13", "tst_clk_out14", "tst_clk_out15",
	"tst_clk_out16", "tst_clk_out17", "tst_clk_out18", "tst_clk_out19",
};

static const char * const uart_ao_a_groups[] = {
	"uart_rx_ao_a_c2", "uart_tx_ao_a_c2", "uart_tx_ao_a_c3", "uart_rx_ao_a_c3",
	"uart_tx_ao_a_w2", "uart_rx_ao_a_w3", "uart_tx_ao_a_w6", "uart_rx_ao_a_w7",
	"uart_tx_ao_a_ao0", "uart_rx_ao_a_ao1", "uart_cts_ao_a", "uart_rts_ao_a",
};

static const char * const uart_ao_b_groups[] = {
	"uart_tx_ao_b_ao0", "uart_rx_ao_b_ao1", "uart_cts_ao_b", "uart_rts_ao_b",
	"uart_tx_ao_b_ao4", "uart_rx_ao_b_ao5", "uart_tx_ao_b_ao10", "uart_rx_ao_b_ao11",
};

static const char * const uart_a_groups[] = {
	"uart_rts_a_dv6", "uart_cts_a_dv7", "uart_tx_a_dv8", "uart_rx_a_dv9",
	"uart_rx_a_c7", "uart_tx_a_c8", "uart_cts_a_c9", "uart_rts_a_c10",
	"uart_tx_a_z6", "uart_rx_a_z7",
};

static const char * const uart_b_groups[] = {
	"uart_rts_b", "uart_cts_b", "uart_rx_b", "uart_tx_b",
};

static const char * const uart_c_groups[] = {
	"uart_rts_c", "uart_cts_c", "uart_rx_c", "uart_tx_c",
};

static struct meson_pmx_func meson_txhd2_periphs_functions[] __initdata = {
	FUNCTION(gpio_periphs),
	FUNCTION(sync_3d_out),
	FUNCTION(atv_if_agc),
	FUNCTION(dcon_led),
	FUNCTION(debug_bus_out),
	FUNCTION(dtv),
	FUNCTION(emmc),
	FUNCTION(eth),
	FUNCTION(hdmirx_a),
	FUNCTION(hdmirx_b),
	FUNCTION(i2cm_a),
	FUNCTION(i2cm_b),
	FUNCTION(i2cm_c),
	FUNCTION(i2cm_d),
	FUNCTION(jtag),
	FUNCTION(lvds),
	FUNCTION(pdm),
	FUNCTION(pwm_a),
	FUNCTION(pwm_b),
	FUNCTION(pwm_c),
	FUNCTION(pwm_d),
	FUNCTION(pwm_e),
	FUNCTION(pwm_f),
	FUNCTION(pwm_vs),
	FUNCTION(spdif),
	FUNCTION(spif),
	FUNCTION(spi),
	FUNCTION(tcon),
	FUNCTION(tdm),
	FUNCTION(tsin),
	FUNCTION(tst),
	FUNCTION(uart_a),
	FUNCTION(uart_b),
	FUNCTION(uart_c),
};

static struct meson_pmx_func meson_txhd2_aobus_functions[] __initdata = {
	FUNCTION(gpio_aobus),
	FUNCTION(cec_ao_b),
	FUNCTION(i2cm_ao_a),
	FUNCTION(i2cs_ao_a),
	FUNCTION(pwm_ao_a_hiz),
	FUNCTION(pwm_ao_c_hiz),
	FUNCTION(pwm_ao_a),
	FUNCTION(pwm_ao_b),
	FUNCTION(pwm_ao_c),
	FUNCTION(pwm_ao_d),
	FUNCTION(uart_ao_a),
	FUNCTION(uart_ao_b),
	FUNCTION(gen_clk),
	FUNCTION(remote),
	FUNCTION(clk_32k_in),
	FUNCTION(dcon_led_ao),
	FUNCTION(jtag_ao),
	FUNCTION(pdm_ao),
};

static struct meson_bank meson_txhd2_periphs_banks[] = {
	/* name  first  last  pullen  pull  dir  out  in */
	BANK("GPIODV_", GPIODV_0, GPIODV_9,
	     0x000,  0, 0x000,  0, 0x010,  0, 0x011,  0, 0x012,  0),
	BANK("GPIOB_",  GPIOB_0, GPIOB_12,
	     0x002,  0, 0x002,  0, 0x000,  0, 0x001,  0, 0x002,  0),
	BANK("GPIOC_",  GPIOC_0, GPIOC_10,
	     0x002, 16, 0x002, 16, 0x00c,  0, 0x00d,  0, 0x00e,  0),
	BANK("GPIOH_",  GPIOH_0, GPIOH_15,
	     0x001, 16, 0x001, 16, 0x006,  0, 0x007,  0, 0x008,  0),
	BANK("GPIOZ_",  GPIOZ_0,  GPIOZ_7,
	     0x003,  0, 0x003,  0, 0x003,  0, 0x004,  0, 0x005,  0),
	BANK("GPIOW_",  GPIOW_0,  GPIOW_7,
	     0x001,  0, 0x001,  0, 0x009,  0, 0x00a,  0, 0x00b,  0),
};

static struct meson_bank meson_txhd2_aobus_banks[] = {
	/* name  first  last  pullen  pull  dir  out  in */
	BANK("GPIOAO_", GPIOAO_0, GPIOAO_13,
	     0x003,  0, 0x002,  0, 0x000,  0, 0x004,  0, 0x001,  0),
};

static struct meson_pmx_bank meson_txhd2_periphs_pmx_banks[] = {
	/* name  first  last  reg  offset */
	BANK_PMX("DV", GPIODV_0, GPIODV_9, 0x007,  0),
	BANK_PMX("B",  GPIOB_0,  GPIOB_12, 0x000,  0),
	BANK_PMX("C",  GPIOC_0,  GPIOC_10, 0x009,  0),
	BANK_PMX("H",  GPIOH_0,  GPIOH_15, 0x005,  0),
	BANK_PMX("Z",  GPIOZ_0,  GPIOZ_7,  0x004,  0),
	BANK_PMX("W",  GPIOW_0,  GPIOW_7,  0x002,  0),
};

static struct meson_axg_pmx_data meson_txhd2_periphs_pmx_banks_data = {
	.pmx_banks	= meson_txhd2_periphs_pmx_banks,
	.num_pmx_banks	= ARRAY_SIZE(meson_txhd2_periphs_pmx_banks),
};

static struct meson_pmx_bank meson_txhd2_aobus_pmx_banks[] = {
	/* name  first  last  reg  offset */
	BANK_PMX("AO",  GPIOAO_0, GPIOAO_13, 0x000, 0),
};

static struct meson_axg_pmx_data meson_txhd2_aobus_pmx_banks_data = {
	.pmx_banks	= meson_txhd2_aobus_pmx_banks,
	.num_pmx_banks	= ARRAY_SIZE(meson_txhd2_aobus_pmx_banks),
};

static struct meson_pinctrl_data meson_txhd2_periphs_pinctrl_data = {
	.name		= "periphs-banks",
	.groups		= meson_txhd2_periphs_groups,
	.funcs		= meson_txhd2_periphs_functions,
	.banks		= meson_txhd2_periphs_banks,
	.num_pins	= 66,
	.num_groups	= ARRAY_SIZE(meson_txhd2_periphs_groups),
	.num_funcs	= ARRAY_SIZE(meson_txhd2_periphs_functions),
	.num_banks	= ARRAY_SIZE(meson_txhd2_periphs_banks),
	.pmx_data	= &meson_txhd2_periphs_pmx_banks_data,
};

static struct meson_pinctrl_data meson_txhd2_aobus_pinctrl_data = {
	.name		= "aobus-banks",
	.groups		= meson_txhd2_aobus_groups,
	.funcs		= meson_txhd2_aobus_functions,
	.banks		= meson_txhd2_aobus_banks,
	.num_pins	= 14,
	.num_groups	= ARRAY_SIZE(meson_txhd2_aobus_groups),
	.num_funcs	= ARRAY_SIZE(meson_txhd2_aobus_functions),
	.num_banks	= ARRAY_SIZE(meson_txhd2_aobus_banks),
	.pmx_data	= &meson_txhd2_aobus_pmx_banks_data,
};

static const struct udevice_id meson_txhd2_pinctrl_match[] = {
	{
		.compatible = "amlogic,meson-txhd2-periphs-pinctrl",
		.data = (ulong)&meson_txhd2_periphs_pinctrl_data,
	},
	{
		.compatible = "amlogic,meson-txhd2-aobus-pinctrl",
		.data = (ulong)&meson_txhd2_aobus_pinctrl_data,
	},
	{ },
};

U_BOOT_DRIVER(meson_txhd2_pinctrl) = {
	.name	= "meson-txhd2-pinctrl",
	.id	= UCLASS_PINCTRL,
	.of_match = of_match_ptr(meson_txhd2_pinctrl_match),
	.probe = meson_pinctrl_probe,
	.priv_auto_alloc_size = sizeof(struct meson_pinctrl),
	.ops = &meson_axg_pinctrl_ops,
};
