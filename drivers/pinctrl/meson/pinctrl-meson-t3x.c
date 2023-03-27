// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <dt-bindings/gpio/meson-t3x-gpio.h>
#include "pinctrl-meson.h"
#include "pinctrl-meson-axg-pmx.h"

/* BANK W func1 */
static const unsigned int hdmirx_a_hpd_pins[]		= { GPIOW_0 };
static const unsigned int hdmirx_a_det_pins[]		= { GPIOW_1 };
static const unsigned int hdmirx_a_sda_pins[]		= { GPIOW_2 };
static const unsigned int hdmirx_a_sck_pins[]		= { GPIOW_3 };
static const unsigned int hdmirx_b_hpd_pins[]		= { GPIOW_4 };
static const unsigned int hdmirx_b_det_pins[]		= { GPIOW_5 };
static const unsigned int hdmirx_b_sda_pins[]		= { GPIOW_6 };
static const unsigned int hdmirx_b_sck_pins[]		= { GPIOW_7 };
static const unsigned int hdmirx_c_hpd_pins[]		= { GPIOW_8 };
static const unsigned int hdmirx_c_det_pins[]		= { GPIOW_9 };
static const unsigned int hdmirx_c_sda_pins[]		= { GPIOW_10 };
static const unsigned int hdmirx_c_sck_pins[]		= { GPIOW_11 };
static const unsigned int hdmirx_d_hpd_pins[]		= { GPIOW_12 };
static const unsigned int hdmirx_d_det_pins[]		= { GPIOW_13 };
static const unsigned int hdmirx_d_sda_pins[]		= { GPIOW_14 };
static const unsigned int hdmirx_d_sck_pins[]		= { GPIOW_15 };
static const unsigned int cec_pins[]			= { GPIOW_16 };

/* BANK W func2 */
static const unsigned int uart_b_tx_w2_pins[]		= { GPIOW_2 };
static const unsigned int uart_b_rx_w3_pins[]		= { GPIOW_3 };
static const unsigned int uart_b_tx_w6_pins[]		= { GPIOW_6 };
static const unsigned int uart_b_rx_w7_pins[]		= { GPIOW_7 };
static const unsigned int uart_b_tx_w10_pins[]		= { GPIOW_10 };
static const unsigned int uart_b_rx_w11_pins[]		= { GPIOW_11 };
static const unsigned int uart_b_tx_w14_pins[]		= { GPIOW_14 };
static const unsigned int uart_b_rx_w15_pins[]		= { GPIOW_15 };

/* BANK D func1 */
static const unsigned int uart_b_tx_d_pins[]		= { GPIOD_0 };
static const unsigned int uart_b_rx_d_pins[]		= { GPIOD_1 };
static const unsigned int i2c1_sck_d2_pins[]		= { GPIOD_2 };
static const unsigned int i2c1_sda_d3_pins[]		= { GPIOD_3 };
static const unsigned int clk_32k_in_pins[]		= { GPIOD_4 };
static const unsigned int remote_input_pins[]		= { GPIOD_5 };
static const unsigned int jtag_a_clk_pins[]		= { GPIOD_6 };
static const unsigned int jtag_a_tms_pins[]		= { GPIOD_7 };
static const unsigned int jtag_a_tdi_pins[]		= { GPIOD_8 };
static const unsigned int jtag_a_tdo_pins[]		= { GPIOD_9 };
static const unsigned int clk12_24M_d_pins[]		= { GPIOD_10 };
static const unsigned int pwm_a_d_pins[]		= { GPIOD_11 };
static const unsigned int pwm_f_d12_pins[]		= { GPIOD_12 };
static const unsigned int i2c1_sck_d13_pins[]		= { GPIOD_13 };
static const unsigned int i2c1_sda_d14_pins[]		= { GPIOD_14 };

/* BANK D func2 */
static const unsigned int remote_out_d1_pins[]		= { GPIOD_1 };
static const unsigned int i2c_slave_sck_pins[]		= { GPIOD_2 };
static const unsigned int i2c_slave_sda_pins[]		= { GPIOD_3 };
static const unsigned int atv_if_agc_d_pins[]		= { GPIOD_4 };
static const unsigned int pwm_c_d5_pins[]		= { GPIOD_5 };
static const unsigned int pwm_f_d6_pins[]		= { GPIOD_6 };
static const unsigned int pwm_c_d7_pins[]		= { GPIOD_7 };
static const unsigned int spdif_out_a_d_pins[]		= { GPIOD_8 };
static const unsigned int pwm_d_d9_pins[]		= { GPIOD_9 };
static const unsigned int pwm_e_d10_pins[]		= { GPIOD_10 };
static const unsigned int pwm_c_d11_pins[]		= { GPIOD_11 };
static const unsigned int pcieck_reqin_pins[]		= { GPIOD_12 };
static const unsigned int uart_d_tx_d_pins[]		= { GPIOD_13 };
static const unsigned int uart_d_rx_d_pins[]		= { GPIOD_14 };

/* BANK D func3 */
static const unsigned int cicam_waitn_d_pins[]		= { GPIOD_0 };
static const unsigned int cicam_irqn_pins[]		= { GPIOD_1 };
static const unsigned int demod_uart_tx_d_pins[]	= { GPIOD_2 };
static const unsigned int demod_uart_rx_d_pins[]	= { GPIOD_3 };
static const unsigned int dtv_if_agc_d_pins[]		= { GPIOD_4 };
static const unsigned int pwm_d_hiz_pins[]		= { GPIOD_6 };
static const unsigned int pwm_c_hiz_pins[]		= { GPIOD_7 };
static const unsigned int spdif_in_d_pins[]		= { GPIOD_8 };
static const unsigned int remote_out_d9_pins[]		= { GPIOD_9 };
static const unsigned int gen_clk_out_d_pins[]		= { GPIOD_10 };
static const unsigned int pwm_i_d11_pins[]		= { GPIOD_11 };
static const unsigned int pwm_j_d_pins[]		= { GPIOD_12 };

/* BANK D func4 */
static const unsigned int uart_c_tx_d_pins[]		= { GPIOD_6 };
static const unsigned int uart_c_rx_d_pins[]		= { GPIOD_7 };
static const unsigned int tdm_d2_d_pins[]		= { GPIOD_8 };
static const unsigned int tdm_d3_d_pins[]		= { GPIOD_9 };

/* BANK D func5 */
static const unsigned int pwm_i_d6_pins[]		= { GPIOD_6 };
static const unsigned int dcon_led_d7_pins[]		= { GPIOD_7 };
static const unsigned int dcon_led_d8_pins[]		= { GPIOD_8 };
static const unsigned int dcon_led_d9_pins[]		= { GPIOD_9 };

/* BANK D func6 */
static const unsigned int pwm_h_d_pins[]		= { GPIOD_6 };
static const unsigned int mclk_1_d_pins[]		= { GPIOD_7 };
static const unsigned int spdif_out_b_d_pins[]		= { GPIOD_8 };
static const unsigned int pwm_vs_d_pins[]		= { GPIOD_9 };

/* BANK D func7 */
static const unsigned int i2c3_sck_d_pins[]		= { GPIOD_6 };
static const unsigned int i2c3_sda_d_pins[]		= { GPIOD_7 };
static const unsigned int i2c4_sck_d_pins[]		= { GPIOD_8 };
static const unsigned int i2c4_sda_d_pins[]		= { GPIOD_9 };

/* BANK E func1 */
static const unsigned int pwm_a_e_pins[]		= { GPIOE_0 };
static const unsigned int pwm_b_e_pins[]		= { GPIOE_1 };
static const unsigned int pwm_e_e_pins[]		= { GPIOE_2 };

/* BANK E func2 */
static const unsigned int i2c2_sck_e_pins[]		= { GPIOE_0 };
static const unsigned int i2c2_sda_e_pins[]		= { GPIOE_1 };
static const unsigned int clk12_24M_e_pins[]		= { GPIOE_2 };

/* BANK B func1 */
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
static const unsigned int loop_gpio_b12_pins[]		= { GPIOB_13 };

/* BANK B func2 */
static const unsigned int loop_gpio_h21_pins[]		= { GPIOB_13 };

/* BANK B func3 */
static const unsigned int spif_hold_pins[]		= { GPIOB_3 };
static const unsigned int spif_mo_pins[]		= { GPIOB_4 };
static const unsigned int spif_mi_pins[]		= { GPIOB_5 };
static const unsigned int spif_clk_pins[]		= { GPIOB_6 };
static const unsigned int spif_wp_pins[]		= { GPIOB_7 };
static const unsigned int remote_out_b_pins[]		= { GPIOB_12 };
static const unsigned int spif_cs_pins[]		= { GPIOB_13 };

/* BANK C func1 */
static const unsigned int sdcard_d0_c_pins[]		= { GPIOC_0 };
static const unsigned int sdcard_d1_c_pins[]		= { GPIOC_1 };
static const unsigned int sdcard_d2_c_pins[]		= { GPIOC_2 };
static const unsigned int sdcard_d3_c_pins[]		= { GPIOC_3 };
static const unsigned int sdcard_clk_c_pins[]		= { GPIOC_4 };
static const unsigned int sdcard_cmd_c_pins[]		= { GPIOC_5 };
static const unsigned int uart_a_tx_c_pins[]		= { GPIOC_6 };
static const unsigned int uart_a_rx_c_pins[]		= { GPIOC_7 };
static const unsigned int uart_a_cts_c_pins[]		= { GPIOC_8 };
static const unsigned int uart_a_rts_c_pins[]		= { GPIOC_9 };
static const unsigned int pwm_f_c_pins[]		= { GPIOC_10 };

/* BANK C func2 */
static const unsigned int spi2_miso_c_pins[]		= { GPIOC_0 };
static const unsigned int spi2_mosi_c_pins[]		= { GPIOC_1 };
static const unsigned int spi2_clk_c_pins[]		= { GPIOC_2 };
static const unsigned int spi2_ss0_c_pins[]		= { GPIOC_3 };
static const unsigned int spi2_ss1_c_pins[]		= { GPIOC_4 };
static const unsigned int spi2_ss2_c_pins[]		= { GPIOC_5 };

/* BANK C func3 */
static const unsigned int cicam_a2_c_pins[]		= { GPIOC_0 };
static const unsigned int cicam_a3_c_pins[]		= { GPIOC_1 };
static const unsigned int cicam_a4_c_pins[]		= { GPIOC_2 };
static const unsigned int cicam_a5_c_pins[]		= { GPIOC_3 };
static const unsigned int cicam_a6_c_pins[]		= { GPIOC_4 };
static const unsigned int cicam_a7_c_pins[]		= { GPIOC_5 };
static const unsigned int cicam_a8_c_pins[]		= { GPIOC_6 };
static const unsigned int cicam_a9_c_pins[]		= { GPIOC_7 };
static const unsigned int cicam_a10_c_pins[]		= { GPIOC_8 };
static const unsigned int cicam_a11_c_pins[]		= { GPIOC_9 };

/* BANK C func4 */
static const unsigned int tsin_c_clk_c_pins[]		= { GPIOC_0 };
static const unsigned int tsin_c_sop_c_pins[]		= { GPIOC_1 };
static const unsigned int tsin_c_valid_c_pins[]		= { GPIOC_2 };
static const unsigned int tsin_c_d0_c_pins[]		= { GPIOC_3 };
static const unsigned int spi2_dq2_c_pins[]		= { GPIOC_4 };
static const unsigned int spi2_dq3_c_pins[]		= { GPIOC_5 };
static const unsigned int demod_uart_tx_c_pins[]	= { GPIOC_6 };
static const unsigned int demod_uart_rx_c_pins[]	= { GPIOC_7 };

/* BANK C func5 */
static const unsigned int spi1_ss1_c_pins[]		= { GPIOC_0 };
static const unsigned int tdm_sclk2_c_pins[]		= { GPIOC_2 };
static const unsigned int tdm_fs2_c_pins[]		= { GPIOC_3 };
static const unsigned int tdm_d6_c_pins[]		= { GPIOC_4 };
static const unsigned int tdm_d7_c_pins[]		= { GPIOC_5 };
static const unsigned int tdm_fs1_c_pins[]		= { GPIOC_6 };
static const unsigned int tdm_sclk1_c_pins[]		= { GPIOC_7 };
static const unsigned int tdm_d4_c_pins[]		= { GPIOC_8 };
static const unsigned int tdm_d5_c_pins[]		= { GPIOC_9 };

/* BANK C func6 */
static const unsigned int spi3_miso_c_pins[]		= { GPIOC_0 };
static const unsigned int spi3_mosi_c_pins[]		= { GPIOC_1 };
static const unsigned int spi3_clk_c_pins[]		= { GPIOC_2 };
static const unsigned int spi3_ss0_c_pins[]		= { GPIOC_3 };
static const unsigned int spi4_miso_c_pins[]		= { GPIOC_4 };
static const unsigned int spi4_clk_c_pins[]		= { GPIOC_5 };
static const unsigned int spi4_ss0_c_pins[]		= { GPIOC_6 };
static const unsigned int spi5_mosi_c_pins[]		= { GPIOC_7 };
static const unsigned int spi5_clk_c_pins[]		= { GPIOC_9 };

/* BANK C func7 */

/* BANK Z func1 */
static const unsigned int tdm_fs2_z_pins[]		= { GPIOZ_0 };
static const unsigned int tdm_sclk2_z_pins[]		= { GPIOZ_1 };
static const unsigned int tdm_d4_z_pins[]		= { GPIOZ_2 };
static const unsigned int tdm_d5_z_pins[]		= { GPIOZ_3 };
static const unsigned int tdm_d6_z_pins[]		= { GPIOZ_4 };
static const unsigned int tdm_d7_z_pins[]		= { GPIOZ_5 };
static const unsigned int mclk_2_z_pins[]		= { GPIOZ_6 };
static const unsigned int tsout_clk_pins[]		= { GPIOZ_7 };
static const unsigned int tsout_sop_pins[]		= { GPIOZ_8 };
static const unsigned int tsout_valid_pins[]		= { GPIOZ_9 };
static const unsigned int tsout_d0_pins[]		= { GPIOZ_10 };
static const unsigned int tsout_d1_pins[]		= { GPIOZ_11 };
static const unsigned int tsout_d2_pins[]		= { GPIOZ_12 };
static const unsigned int tsout_d3_pins[]		= { GPIOZ_13 };
static const unsigned int tsout_d4_pins[]		= { GPIOZ_14 };
static const unsigned int tsout_d5_pins[]		= { GPIOZ_15 };
static const unsigned int tsout_d6_pins[]		= { GPIOZ_16 };
static const unsigned int tsout_d7_pins[]		= { GPIOZ_17 };
static const unsigned int dcon_led_z18_pins[]		= { GPIOZ_18 };
static const unsigned int spdif_out_a_z19_pins[]	= { GPIOZ_19 };

/* BANK Z func2 */
static const unsigned int pwm_j_z0_pins[]		= { GPIOZ_0 };
static const unsigned int tsin_a_sop_z_pins[]		= { GPIOZ_1 };
static const unsigned int tsin_a_valid_z_pins[]		= { GPIOZ_2 };
static const unsigned int tsin_a_d0_z_pins[]		= { GPIOZ_3 };
static const unsigned int dtv_rf_agc_z6_pins[]		= { GPIOZ_6 };
static const unsigned int cicam_a2_z_pins[]		= { GPIOZ_8 };
static const unsigned int cicam_a3_z_pins[]		= { GPIOZ_9 };
static const unsigned int cicam_a4_z_pins[]		= { GPIOZ_10 };
static const unsigned int cicam_a5_z_pins[]		= { GPIOZ_11 };
static const unsigned int cicam_a6_z_pins[]		= { GPIOZ_12 };
static const unsigned int cicam_a7_z_pins[]		= { GPIOZ_13 };
static const unsigned int cicam_a8_z_pins[]		= { GPIOZ_14 };
static const unsigned int cicam_a9_z_pins[]		= { GPIOZ_15 };
static const unsigned int cicam_a10_z_pins[]		= { GPIOZ_16 };
static const unsigned int cicam_a11_z_pins[]		= { GPIOZ_17 };
static const unsigned int dtv_rf_agc_z18_pins[]		= { GPIOZ_18 };
static const unsigned int spdif_in_z_pins[]		= { GPIOZ_19 };

/* BANK Z func3 */
static const unsigned int cicam_a12_z_pins[]		= { GPIOZ_0 };
static const unsigned int iso7816_clk_z_pins[]		= { GPIOZ_1 };
static const unsigned int cicam_a13_pins[]		= { GPIOZ_2 };
static const unsigned int cicam_a14_pins[]		= { GPIOZ_3 };
static const unsigned int i2c0_sck_pins[]		= { GPIOZ_4 };
static const unsigned int i2c0_sda_pins[]		= { GPIOZ_5 };
static const unsigned int atv_if_agc_z6_pins[]		= { GPIOZ_6 };
static const unsigned int atv_if_agc_z18_pins[]		= { GPIOZ_18 };
static const unsigned int spdif_out_b_z_pins[]		= { GPIOZ_19 };

/* BANK Z func4 */
static const unsigned int pwm_vs_z0_pins[]		= { GPIOZ_0 };
static const unsigned int spi1_mosi_z_pins[]		= { GPIOZ_1 };
static const unsigned int spi1_clk_z_pins[]		= { GPIOZ_2 };
static const unsigned int spi1_ss0_z_pins[]		= { GPIOZ_3 };
static const unsigned int spi1_dq2_z_pins[]		= { GPIOZ_4 };
static const unsigned int spi1_dq3_z_pins[]		= { GPIOZ_5 };
static const unsigned int dtv_if_agc_z6_pins[]		= { GPIOZ_6 };
static const unsigned int dtv_if_agc_z18_pins[]		= { GPIOZ_18 };
static const unsigned int pwm_g_z19_pins[]		= { GPIOZ_19 };

/* BANK Z func5 */
static const unsigned int spi5_miso_c_pins[]		= { GPIOZ_0 };
static const unsigned int uart_a_rx_z_pins[]		= { GPIOZ_1 };
static const unsigned int uart_a_cts_z_pins[]		= { GPIOZ_2 };
static const unsigned int uart_a_rts_z_pins[]		= { GPIOZ_3 };
static const unsigned int pwm_b_z_pins[]		= { GPIOZ_4 };
static const unsigned int pwm_d_z_pins[]		= { GPIOZ_5 };
static const unsigned int pwm_e_z_pins[]		= { GPIOZ_6 };
static const unsigned int uart_d_tx_z_pins[]		= { GPIOZ_8 };
static const unsigned int uart_d_rx_z_pins[]		= { GPIOZ_9 };
static const unsigned int pwm_g_z12_pins[]		= { GPIOZ_12 };
static const unsigned int pwm_h_z_pins[]		= { GPIOZ_13 };
static const unsigned int pwm_i_z_pins[]		= { GPIOZ_14 };
static const unsigned int pwm_j_z15_pins[]		= { GPIOZ_15 };
static const unsigned int spi0_ss1_z_pins[]		= { GPIOZ_18 };
static const unsigned int spi1_miso_z_pins[]		= { GPIOZ_19 };

/* BANK Z func6 */
static const unsigned int pcieck_reqn_z_pins[]		= { GPIOZ_0 };
static const unsigned int pdm_din2_z_pins[]		= { GPIOZ_1 };
static const unsigned int pdm_dclk_z_pins[]		= { GPIOZ_2 };
static const unsigned int pdm_din0_z_pins[]		= { GPIOZ_3 };
static const unsigned int pdm_din1_z6_pins[]		= { GPIOZ_6 };
static const unsigned int tdm_d1_z_pins[]		= { GPIOZ_10 };
static const unsigned int tdm_d0_z_pins[]		= { GPIOZ_11 };
static const unsigned int tdm_sclk1_z_pins[]		= { GPIOZ_16 };
static const unsigned int tdm_fs1_z_pins[]		= { GPIOZ_17 };
static const unsigned int pwm_vs_z18_pins[]		= { GPIOZ_18 };
static const unsigned int diseqc_out_z19_pins[]		= { GPIOZ_19 };

/* BANK Z func7 */
static const unsigned int dcon_led_z0_pins[]		= { GPIOZ_0 };
static const unsigned int s2_demod_gpio0_z_pins[]	= { GPIOZ_1 };
static const unsigned int spdif_out_a_z2_pins[]		= { GPIOZ_2 };
static const unsigned int spi0_ss1_z3_pins[]		= { GPIOZ_3 };
static const unsigned int eth_mdio_pins[]		= { GPIOZ_4 };
static const unsigned int eth_mdc_pins[]		= { GPIOZ_5 };
static const unsigned int eth_link_led_z_pins[]		= { GPIOZ_6 };
static const unsigned int eth_rgmii_rx_clk_pins[]	= { GPIOZ_7 };
static const unsigned int eth_rx_dv_pins[]		= { GPIOZ_8 };
static const unsigned int eth_rxd0_pins[]		= { GPIOZ_9 };
static const unsigned int eth_rxd1_pins[]		= { GPIOZ_10 };
static const unsigned int eth_rxd2_rgmii_pins[]		= { GPIOZ_11 };
static const unsigned int eth_rxd3_rgmii_pins[]		= { GPIOZ_12 };
static const unsigned int eth_rgmii_tx_clk_pins[]	= { GPIOZ_13 };
static const unsigned int eth_txen_pins[]		= { GPIOZ_14 };
static const unsigned int eth_txd0_pins[]		= { GPIOZ_15 };
static const unsigned int eth_txd1_pins[]		= { GPIOZ_16 };
static const unsigned int eth_txd2_rgmii_pins[]		= { GPIOZ_17 };
static const unsigned int eth_txd3_rgmii_pins[]		= { GPIOZ_18 };
static const unsigned int eth_act_led_z_pins[]		= { GPIOZ_19 };

/* BANK H func1 */
static const unsigned int tcon_0_pins[]			= { GPIOH_0 };
static const unsigned int tcon_1_pins[]			= { GPIOH_1 };
static const unsigned int tcon_2_pins[]			= { GPIOH_2 };
static const unsigned int tcon_3_pins[]			= { GPIOH_3 };
static const unsigned int tcon_4_pins[]			= { GPIOH_4 };
static const unsigned int tcon_5_pins[]			= { GPIOH_5 };
static const unsigned int tcon_6_pins[]			= { GPIOH_6 };
static const unsigned int tcon_7_pins[]			= { GPIOH_7 };
static const unsigned int tcon_8_pins[]			= { GPIOH_8 };
static const unsigned int tcon_9_pins[]			= { GPIOH_9 };
static const unsigned int tcon_10_pins[]		= { GPIOH_10 };
static const unsigned int tcon_11_pins[]		= { GPIOH_11 };
static const unsigned int tcon_12_pins[]		= { GPIOH_12 };
static const unsigned int tcon_13_pins[]		= { GPIOH_13 };
static const unsigned int tcon_14_pins[]		= { GPIOH_14 };
static const unsigned int tcon_15_pins[]		= { GPIOH_15 };
static const unsigned int hsync_pins[]			= { GPIOH_18 };
static const unsigned int vsync_pins[]			= { GPIOH_19 };
static const unsigned int i2c2_sck_h20_pins[]		= { GPIOH_20 };
static const unsigned int i2c2_sda_h21_pins[]		= { GPIOH_21 };
static const unsigned int i2c3_sck_h22_pins[]		= { GPIOH_22 };
static const unsigned int i2c3_sda_h23_pins[]		= { GPIOH_23 };
static const unsigned int i2c3_sck_h24_pins[]		= { GPIOH_24 };
static const unsigned int i2c3_sda_h25_pins[]		= { GPIOH_25 };
static const unsigned int i2c4_sck_h26_pins[]		= { GPIOH_26 };
static const unsigned int i2c4_sda_h27_pins[]		= { GPIOH_27 };
static const unsigned int i2c3_sck_h28_pins[]		= { GPIOH_28 };
static const unsigned int i2c3_sda_h29_pins[]		= { GPIOH_29 };

/* BANK H func2 */
static const unsigned int tcon_lock_pins[]		= { GPIOH_0 };
static const unsigned int sync_3d_out_pins[]		= { GPIOH_5 };
static const unsigned int tdm_d3_h6_pins[]		= { GPIOH_6 };
static const unsigned int spi0_ss1_h7_pins[]		= { GPIOH_7 };
static const unsigned int spi0_ss0_h8_pins[]		= { GPIOH_8 };
static const unsigned int spi0_miso_h9_pins[]		= { GPIOH_9 };
static const unsigned int spi0_mosi_h10_pins[]		= { GPIOH_10 };
static const unsigned int spi0_clk_h11_pins[]		= { GPIOH_11 };
static const unsigned int spi0_ss2_h12_pins[]		= { GPIOH_12 };
static const unsigned int tdm_d3_h13_pins[]		= { GPIOH_13 };
static const unsigned int mclk_1_h_pins[]		= { GPIOH_14 };
static const unsigned int tdm_fs1_h15_pins[]		= { GPIOH_15 };
static const unsigned int tdm_sclk1_h_pins[]		= { GPIOH_16 };
static const unsigned int tdm_d0_h_pins[]		= { GPIOH_17 };
static const unsigned int tdm_d1_h_pins[]		= { GPIOH_18 };
static const unsigned int tdm_d2_h19_pins[]		= { GPIOH_19 };
static const unsigned int tdm_d3_h20_pins[]		= { GPIOH_20 };
static const unsigned int tdm_d4_h_pins[]		= { GPIOH_21 };
static const unsigned int tdm_d5_h_pins[]		= { GPIOH_22 };
static const unsigned int tdm_d6_h_pins[]		= { GPIOH_23 };
static const unsigned int spi0_ss0_h26_pins[]		= { GPIOH_26 };
static const unsigned int spi0_miso_h27_pins[]		= { GPIOH_27 };
static const unsigned int spi0_mosi_h28_pins[]		= { GPIOH_28 };
static const unsigned int spi0_clk_h29_pins[]		= { GPIOH_29 };

/* BANK H func3 */
static const unsigned int tcon_sfc_h0_pins[]		= { GPIOH_0 };
static const unsigned int tcon_sfc_h1_pins[]		= { GPIOH_1 };
static const unsigned int uart_c_tx_h2_pins[]		= { GPIOH_2 };
static const unsigned int uart_c_rx_h3_pins[]		= { GPIOH_3 };
static const unsigned int uart_c_cts_h4_pins[]		= { GPIOH_4 };
static const unsigned int uart_c_rts_h5_pins[]		= { GPIOH_5 };
static const unsigned int i2c2_sck_h10_pins[]		= { GPIOH_10 };
static const unsigned int i2c2_sda_h11_pins[]		= { GPIOH_11 };
static const unsigned int pwm_vs_h12_pins[]		= { GPIOH_12 };
static const unsigned int pwm_vs_h13_pins[]		= { GPIOH_13 };
static const unsigned int pdm_din1_h14_pins[]		= { GPIOH_14 };
static const unsigned int pdm_din2_h16_pins[]		= { GPIOH_16 };
static const unsigned int pdm_din3_h17_pins[]		= { GPIOH_17 };
static const unsigned int pdm_din0_h18_pins[]		= { GPIOH_18 };
static const unsigned int pdm_dclk_h19_pins[]		= { GPIOH_19 };
static const unsigned int loop_gpio_b13_pins[]		= { GPIOH_21 };
static const unsigned int pdm_dclk_h22_pins[]		= { GPIOH_22 };
static const unsigned int pdm_din2_h23_pins[]		= { GPIOH_23 };
static const unsigned int i2c2_sck_h24_pins[]		= { GPIOH_24 };
static const unsigned int i2c2_sda_h25_pins[]		= { GPIOH_25 };
static const unsigned int pdm_din0_h26_pins[]		= { GPIOH_26 };
static const unsigned int pdm_din1_h27_pins[]		= { GPIOH_27 };
static const unsigned int pdm_din2_h28_pins[]		= { GPIOH_28 };
static const unsigned int pdm_dclk_h29_pins[]		= { GPIOH_29 };

/* BANK H func4 */
static const unsigned int vx1_a_lockn_pins[]		= { GPIOH_0 };
static const unsigned int vx1_a_htpdn_pins[]		= { GPIOH_1 };
static const unsigned int vx1_b_lockn_pins[]		= { GPIOH_2 };
static const unsigned int vx1_b_htpdn_pins[]		= { GPIOH_3 };
static const unsigned int pwm_d_h5_pins[]		= { GPIOH_5 };
static const unsigned int pwm_d_h12_pins[]		= { GPIOH_12 };
static const unsigned int pwm_e_h_pins[]		= { GPIOH_13 };
static const unsigned int tdm_d3_h14_pins[]		= { GPIOH_14 };
static const unsigned int eth_act_led_h_pins[]		= { GPIOH_18 };
static const unsigned int eth_link_led_h_pins[]		= { GPIOH_19 };
static const unsigned int i2c4_sck_h20_pins[]		= { GPIOH_20 };
static const unsigned int i2c4_sda_h21_pins[]		= { GPIOH_21 };
static const unsigned int uart_c_cts_h22_pins[]		= { GPIOH_22 };
static const unsigned int uart_c_rts_h23_pins[]		= { GPIOH_23 };
static const unsigned int uart_c_tx_h24_pins[]		= { GPIOH_24 };
static const unsigned int uart_c_rx_h25_pins[]		= { GPIOH_25 };
static const unsigned int uart_c_rts_h26_pins[]		= { GPIOH_26 };
static const unsigned int uart_c_tx_h27_pins[]		= { GPIOH_27 };
static const unsigned int uart_c_rx_h28_pins[]		= { GPIOH_28 };

/* BANK H func5 */
static const unsigned int clk12_24M_h_pins[]		= { GPIOH_13 };
static const unsigned int spi0_clk_h14_pins[]		= { GPIOH_14 };
static const unsigned int tdm_fs1_h18_pins[]		= { GPIOH_18 };
static const unsigned int spi0_ss1_h19_pins[]		= { GPIOH_19 };
static const unsigned int demod_uart_tx_h_pins[]	= { GPIOH_20 };
static const unsigned int demod_uart_rx_h_pins[]	= { GPIOH_21 };
static const unsigned int spi1_ss0_h_pins[]		= { GPIOH_22 };
static const unsigned int spi1_miso_h_pins[]		= { GPIOH_23 };
static const unsigned int spi1_mosi_h_pins[]		= { GPIOH_24 };
static const unsigned int spi1_clk_h_pins[]		= { GPIOH_25 };
static const unsigned int tdm_d2_h26_pins[]		= { GPIOH_26 };
static const unsigned int tdm_d3_h27_pins[]		= { GPIOH_27 };
static const unsigned int tdm_fs2_h28_pins[]		= { GPIOH_28 };
static const unsigned int tdm_sclk2_h29_pins[]		= { GPIOH_29 };

/* BANK H func6 */
static const unsigned int gen_clk_out_h_pins[]		= { GPIOH_13 };
static const unsigned int s2_demod_gpio0_h_pins[]	= { GPIOH_14 };
static const unsigned int s2_demod_gpio1_h_pins[]	= { GPIOH_15 };
static const unsigned int s2_demod_gpio2_h_pins[]	= { GPIOH_16 };
static const unsigned int s2_demod_gpio3_h_pins[]	= { GPIOH_17 };
static const unsigned int s2_demod_gpio4_h_pins[]	= { GPIOH_18 };
static const unsigned int s2_demod_gpio5_h_pins[]	= { GPIOH_19 };
static const unsigned int s2_demod_gpio6_h_pins[]	= { GPIOH_20 };
static const unsigned int s2_demod_gpio7_h_pins[]	= { GPIOH_21 };
static const unsigned int tdm_fs2_h22_pins[]		= { GPIOH_22 };
static const unsigned int tdm_sclk2_h23_pins[]		= { GPIOH_23 };
static const unsigned int spi0_ss1_h24_pins[]		= { GPIOH_26 };
static const unsigned int dcon_led_h_pins[]		= { GPIOH_27 };
static const unsigned int spi1_dq2_h_pins[]		= { GPIOH_28 };
static const unsigned int spi1_dq3_h_pins[]		= { GPIOH_29 };

/* BANK P func1 */
static const unsigned int spi2_miso_p_pins[]		= { GPIOP_0 };
static const unsigned int spi2_mosi_p_pins[]		= { GPIOP_1 };
static const unsigned int spi2_clk_p_pins[]		= { GPIOP_2 };
static const unsigned int spi2_ss0_p_pins[]		= { GPIOP_3 };
static const unsigned int i2c4_sck_p_pins[]		= { GPIOP_4 };
static const unsigned int i2c4_sda_p_pins[]		= { GPIOP_5 };
static const unsigned int mclk_2_p_pins[]		= { GPIOP_6 };
static const unsigned int tdm_sclk2_p_pins[]		= { GPIOP_7 };
static const unsigned int tdm_fs2_p_pins[]		= { GPIOP_8 };
static const unsigned int tdm_d2_p_pins[]		= { GPIOP_9 };

/* BANK P func2 */
static const unsigned int uart_c_tx_p_pins[]		= { GPIOP_0 };
static const unsigned int uart_c_rx_p_pins[]		= { GPIOP_1 };
static const unsigned int uart_c_cts_p_pins[]		= { GPIOP_2 };
static const unsigned int uart_c_rts_p_pins[]		= { GPIOP_3 };
static const unsigned int pwm_i_p_pins[]		= { GPIOP_4 };
static const unsigned int pwm_j_p_pins[]		= { GPIOP_5 };
static const unsigned int tdm_d4_p_pins[]		= { GPIOP_6 };
static const unsigned int tdm_d3_p_pins[]		= { GPIOP_9 };

/* BANK P func3 */
static const unsigned int spdif_out_a_p_pins[]		= { GPIOP_0 };
static const unsigned int pwm_g_p1_pins[]		= { GPIOP_1 };
static const unsigned int pwm_h_p2_pins[]		= { GPIOP_2 };
static const unsigned int spi0_ss2_p_pins[]		= { GPIOP_3 };
static const unsigned int spi2_dq2_p_pins[]		= { GPIOP_4 };
static const unsigned int spi2_dq3_p_pins[]		= { GPIOP_5 };
static const unsigned int pwm_e_p6_pins[]		= { GPIOP_6 };
static const unsigned int pwm_g_p7_pins[]		= { GPIOP_7 };
static const unsigned int pwm_h_p8_pins[]		= { GPIOP_8 };
static const unsigned int dcon_led_p_pins[]		= { GPIOP_9 };

/* BANK P func4 */
static const unsigned int spi1_ss1_p_pins[]		= { GPIOP_0 };
static const unsigned int pcieck_reqn_p_pins[]		= { GPIOP_5 };
static const unsigned int remote_out_p6_pins[]		= { GPIOP_6 };
static const unsigned int spi4_ss1_p_pins[]		= { GPIOP_7 };
static const unsigned int remote_out_p9_pins[]		= { GPIOP_9 };

/* BANK P func5 */
static const unsigned int spi3_miso_p_pins[]		= { GPIOP_0 };
static const unsigned int spi3_mosi_p_pins[]		= { GPIOP_1 };
static const unsigned int spi3_clk_p_pins[]		= { GPIOP_2 };
static const unsigned int spi3_ss0_p_pins[]		= { GPIOP_3 };
static const unsigned int spi4_mosi_p_pins[]		= { GPIOP_4 };
static const unsigned int spi4_clk_p_pins[]		= { GPIOP_5 };
static const unsigned int spi4_ss0_p_pins[]		= { GPIOP_6 };
static const unsigned int spi5_mosi_p_pins[]		= { GPIOP_7 };
static const unsigned int spi5_clk_p_pins[]		= { GPIOP_8 };
static const unsigned int spi5_ss0_p_pins[]		= { GPIOP_9 };

/* BANK P func6 */
static const unsigned int spdif_out_b_p_pins[]		= { GPIOP_0 };
static const unsigned int tsin_c_clk_p_pins[]		= { GPIOP_6 };
static const unsigned int tsin_c_sop_p_pins[]		= { GPIOP_7 };
static const unsigned int tsin_c_valid_p_pins[]		= { GPIOP_8 };
static const unsigned int tsin_c_d0_p_pins[]		= { GPIOP_9 };

/* BANK M func1 */
static const unsigned int tsin_b_clk_pins[]		= { GPIOM_0 };
static const unsigned int tsin_b_sop_pins[]		= { GPIOM_1 };
static const unsigned int tsin_b_valid_pins[]		= { GPIOM_2 };
static const unsigned int tsin_b_d0_pins[]		= { GPIOM_3 };
static const unsigned int tsin_b_d1_pins[]		= { GPIOM_4 };
static const unsigned int tsin_b_d2_pins[]		= { GPIOM_5 };
static const unsigned int tsin_b_d3_pins[]		= { GPIOM_6 };
static const unsigned int tsin_b_d4_pins[]		= { GPIOM_7 };
static const unsigned int tsin_b_d5_pins[]		= { GPIOM_8 };
static const unsigned int tsin_b_d6_pins[]		= { GPIOM_9 };
static const unsigned int tsin_b_d7_pins[]		= { GPIOM_10 };
static const unsigned int cicam_data0_pins[]		= { GPIOM_11 };
static const unsigned int cicam_data1_pins[]		= { GPIOM_12 };
static const unsigned int cicam_data2_pins[]		= { GPIOM_13 };
static const unsigned int cicam_data3_pins[]		= { GPIOM_14 };
static const unsigned int cicam_data4_pins[]		= { GPIOM_15 };
static const unsigned int cicam_data5_pins[]		= { GPIOM_16 };
static const unsigned int cicam_data6_pins[]		= { GPIOM_17 };
static const unsigned int cicam_data7_pins[]		= { GPIOM_18 };
static const unsigned int cicam_a0_pins[]		= { GPIOM_19 };
static const unsigned int cicam_a1_pins[]		= { GPIOM_20 };
static const unsigned int cicam_cen_pins[]		= { GPIOM_21 };
static const unsigned int cicam_oen_pins[]		= { GPIOM_22 };
static const unsigned int cicam_wen_pins[]		= { GPIOM_23 };
static const unsigned int cicam_iordn_pins[]		= { GPIOM_24 };
static const unsigned int cicam_iowrn_pins[]		= { GPIOM_25 };
static const unsigned int cicam_reset_pins[]		= { GPIOM_26 };
static const unsigned int cicam_cdn_pins[]		= { GPIOM_27 };
static const unsigned int cicam_5ven_pins[]		= { GPIOM_28 };
static const unsigned int cicam_waitn_m_pins[]		= { GPIOM_29 };

/* BANK M func2 */
static const unsigned int spi2_miso_m_pins[]		= { GPIOM_0 };
static const unsigned int spi2_mosi_m_pins[]		= { GPIOM_1 };
static const unsigned int spi2_clk_m_pins[]		= { GPIOM_2 };
static const unsigned int spi2_ss0_m_pins[]		= { GPIOM_3 };
static const unsigned int pdm_din2_m_pins[]		= { GPIOM_8 };
static const unsigned int pdm_din3_m_pins[]		= { GPIOM_9 };
static const unsigned int pdm_din0_m_pins[]		= { GPIOM_10 };
static const unsigned int pdm_dclk_m_pins[]		= { GPIOM_11 };
static const unsigned int mclk_2_m_pins[]		= { GPIOM_12 };
static const unsigned int tdm_fs2_m_pins[]		= { GPIOM_13 };
static const unsigned int tdm_sclk2_m_pins[]		= { GPIOM_14 };
static const unsigned int tdm_d4_m_pins[]		= { GPIOM_15 };
static const unsigned int tdm_d5_m_pins[]		= { GPIOM_16 };
static const unsigned int tdm_d6_m_pins[]		= { GPIOM_17 };
static const unsigned int tdm_d7_m_pins[]		= { GPIOM_18 };
static const unsigned int iso7816_clk_m_pins[]		= { GPIOM_19 };
static const unsigned int iso7816_data_m_pins[]		= { GPIOM_20 };
static const unsigned int mclk_1_m_pins[]		= { GPIOM_21 };
static const unsigned int tdm_fs1_m_pins[]		= { GPIOM_22 };
static const unsigned int tdm_sclk1_m_pins[]		= { GPIOM_23 };
static const unsigned int tdm_d0_m_pins[]		= { GPIOM_24 };
static const unsigned int tdm_d1_m_pins[]		= { GPIOM_25 };
static const unsigned int tdm_d2_m_pins[]		= { GPIOM_26 };
static const unsigned int tdm_d3_m_pins[]		= { GPIOM_29 };

/* BANK M func3 */
static const unsigned int uart_a_tx_m_pins[]		= { GPIOM_0 };
static const unsigned int uart_a_rx_m_pins[]		= { GPIOM_1 };
static const unsigned int uart_a_cts_m_pins[]		= { GPIOM_2 };
static const unsigned int uart_a_rts_m_pins[]		= { GPIOM_3 };
static const unsigned int sdcard_d0_m_pins[]		= { GPIOM_4 };
static const unsigned int sdcard_d1_m_pins[]		= { GPIOM_5 };
static const unsigned int sdcard_d2_m_pins[]		= { GPIOM_6 };
static const unsigned int sdcard_d3_m_pins[]		= { GPIOM_7 };
static const unsigned int sdcard_clk_m_pins[]		= { GPIOM_8 };
static const unsigned int sdcard_cmd_m_pins[]		= { GPIOM_9 };
static const unsigned int pwm_f_m10_pins[]		= { GPIOM_10 };
static const unsigned int pwm_i_m_pins[]		= { GPIOM_11 };
static const unsigned int pwm_j_m_pins[]		= { GPIOM_12 };
static const unsigned int i2c4_sck_m_pins[]		= { GPIOM_17 };
static const unsigned int i2c4_sda_m_pins[]		= { GPIOM_18 };
static const unsigned int spi0_miso_m_pins[]		= { GPIOM_19 };
static const unsigned int spi0_mosi_m_pins[]		= { GPIOM_20 };
static const unsigned int spi0_clk_m_pins[]		= { GPIOM_21 };
static const unsigned int spi0_ss0_m_pins[]		= { GPIOM_22 };
static const unsigned int spi0_ss1_m_pins[]		= { GPIOM_23 };
static const unsigned int spi0_ss2_m_pins[]		= { GPIOM_24 };
static const unsigned int i2c2_sck_m_pins[]		= { GPIOM_25 };
static const unsigned int i2c2_sda_m_pins[]		= { GPIOM_26 };
static const unsigned int i2c3_sck_m_pins[]		= { GPIOM_27 };
static const unsigned int i2c3_sda_m_pins[]		= { GPIOM_28 };
static const unsigned int pcieck_reqn_m_pins[]		= { GPIOM_29 };

/* BANK M func4 */
static const unsigned int pwm_d_m1_pins[]		= { GPIOM_1 };
static const unsigned int uart_d_tx_m5_pins[]		= { GPIOM_5 };
static const unsigned int uart_d_rx_m6_pins[]		= { GPIOM_6 };
static const unsigned int pwm_g_m_pins[]		= { GPIOM_8 };
static const unsigned int pwm_h_m_pins[]		= { GPIOM_9 };
static const unsigned int uart_d_tx_m17_pins[]		= { GPIOM_17 };
static const unsigned int uart_d_rx_m17_pins[]		= { GPIOM_18 };
static const unsigned int uart_c_tx_m_pins[]		= { GPIOM_19 };
static const unsigned int uart_c_rx_m_pins[]		= { GPIOM_20 };
static const unsigned int uart_c_cts_m_pins[]		= { GPIOM_21 };
static const unsigned int uart_c_rts_m_pins[]		= { GPIOM_22 };
static const unsigned int pwm_d_m23_pins[]		= { GPIOM_23 };
static const unsigned int pwm_e_m_pins[]		= { GPIOM_24 };
static const unsigned int pwm_f_m26_pins[]		= { GPIOM_26 };
static const unsigned int gen_clk_out_m_pins[]		= { GPIOM_29 };

/* BANK M func5 */
static const unsigned int tsin_a_clk_m_pins[]		= { GPIOM_19 };
static const unsigned int tsin_a_sop_m_pins[]		= { GPIOM_20 };
static const unsigned int tsin_a_valid_m_pins[]		= { GPIOM_21 };
static const unsigned int tsin_a_d0_m_pins[]		= { GPIOM_22 };
static const unsigned int demod_uart_tx_m23_pins[]	= { GPIOM_23 };
static const unsigned int demod_uart_rx_m24_pins[]	= { GPIOM_24 };
static const unsigned int demod_uart_tx_m25_pins[]	= { GPIOM_25 };
static const unsigned int demod_uart_rx_m26_pins[]	= { GPIOM_26 };
static const unsigned int spdif_in_m_pins[]		= { GPIOM_29 };

static struct meson_pmx_group meson_t3x_periphs_groups[] = {
	GPIO_GROUP(GPIOW_0),
	GPIO_GROUP(GPIOW_1),
	GPIO_GROUP(GPIOW_2),
	GPIO_GROUP(GPIOW_3),
	GPIO_GROUP(GPIOW_4),
	GPIO_GROUP(GPIOW_5),
	GPIO_GROUP(GPIOW_6),
	GPIO_GROUP(GPIOW_7),
	GPIO_GROUP(GPIOW_8),
	GPIO_GROUP(GPIOW_9),
	GPIO_GROUP(GPIOW_10),
	GPIO_GROUP(GPIOW_11),
	GPIO_GROUP(GPIOW_12),
	GPIO_GROUP(GPIOW_13),
	GPIO_GROUP(GPIOW_14),
	GPIO_GROUP(GPIOW_15),
	GPIO_GROUP(GPIOW_16),

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
	GPIO_GROUP(GPIOD_13),
	GPIO_GROUP(GPIOD_14),

	GPIO_GROUP(GPIOE_0),
	GPIO_GROUP(GPIOE_1),
	GPIO_GROUP(GPIOE_2),

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
	GPIO_GROUP(GPIOC_8),
	GPIO_GROUP(GPIOC_9),
	GPIO_GROUP(GPIOC_10),

	GPIO_GROUP(GPIOZ_0),
	GPIO_GROUP(GPIOZ_1),
	GPIO_GROUP(GPIOZ_2),
	GPIO_GROUP(GPIOZ_3),
	GPIO_GROUP(GPIOZ_4),
	GPIO_GROUP(GPIOZ_5),
	GPIO_GROUP(GPIOZ_6),
	GPIO_GROUP(GPIOZ_7),
	GPIO_GROUP(GPIOZ_8),
	GPIO_GROUP(GPIOZ_9),
	GPIO_GROUP(GPIOZ_10),
	GPIO_GROUP(GPIOZ_11),
	GPIO_GROUP(GPIOZ_12),
	GPIO_GROUP(GPIOZ_13),
	GPIO_GROUP(GPIOZ_14),
	GPIO_GROUP(GPIOZ_15),
	GPIO_GROUP(GPIOZ_16),
	GPIO_GROUP(GPIOZ_17),
	GPIO_GROUP(GPIOZ_18),
	GPIO_GROUP(GPIOZ_19),

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
	GPIO_GROUP(GPIOH_16),
	GPIO_GROUP(GPIOH_17),
	GPIO_GROUP(GPIOH_18),
	GPIO_GROUP(GPIOH_19),
	GPIO_GROUP(GPIOH_20),
	GPIO_GROUP(GPIOH_21),
	GPIO_GROUP(GPIOH_22),
	GPIO_GROUP(GPIOH_23),
	GPIO_GROUP(GPIOH_24),
	GPIO_GROUP(GPIOH_25),
	GPIO_GROUP(GPIOH_26),
	GPIO_GROUP(GPIOH_27),
	GPIO_GROUP(GPIOH_28),
	GPIO_GROUP(GPIOH_29),

	GPIO_GROUP(GPIOP_0),
	GPIO_GROUP(GPIOP_1),
	GPIO_GROUP(GPIOP_2),
	GPIO_GROUP(GPIOP_3),
	GPIO_GROUP(GPIOP_4),
	GPIO_GROUP(GPIOP_5),
	GPIO_GROUP(GPIOP_6),
	GPIO_GROUP(GPIOP_7),
	GPIO_GROUP(GPIOP_8),
	GPIO_GROUP(GPIOP_9),

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
	GPIO_GROUP(GPIOM_16),
	GPIO_GROUP(GPIOM_17),
	GPIO_GROUP(GPIOM_18),
	GPIO_GROUP(GPIOM_19),
	GPIO_GROUP(GPIOM_20),
	GPIO_GROUP(GPIOM_21),
	GPIO_GROUP(GPIOM_22),
	GPIO_GROUP(GPIOM_23),
	GPIO_GROUP(GPIOM_24),
	GPIO_GROUP(GPIOM_25),
	GPIO_GROUP(GPIOM_26),
	GPIO_GROUP(GPIOM_27),
	GPIO_GROUP(GPIOM_28),
	GPIO_GROUP(GPIOM_29),
	GPIO_GROUP(GPIO_TEST_N),

	/* BANK W func1 */
	GROUP(hdmirx_a_hpd,		1),
	GROUP(hdmirx_a_det,		1),
	GROUP(hdmirx_a_sda,		1),
	GROUP(hdmirx_a_sck,		1),
	GROUP(hdmirx_b_hpd,		1),
	GROUP(hdmirx_b_det,		1),
	GROUP(hdmirx_b_sda,		1),
	GROUP(hdmirx_b_sck,		1),
	GROUP(hdmirx_c_hpd,		1),
	GROUP(hdmirx_c_det,		1),
	GROUP(hdmirx_c_sda,		1),
	GROUP(hdmirx_c_sck,		1),
	GROUP(hdmirx_d_hpd,		1),
	GROUP(hdmirx_d_det,		1),
	GROUP(hdmirx_d_sda,		1),
	GROUP(hdmirx_d_sck,		1),
	GROUP(cec,			1),

	/* BANK W func2 */
	GROUP(uart_b_tx_w2,		2),
	GROUP(uart_b_rx_w3,		2),
	GROUP(uart_b_tx_w6,		2),
	GROUP(uart_b_rx_w7,		2),
	GROUP(uart_b_tx_w10,		2),
	GROUP(uart_b_rx_w11,		2),
	GROUP(uart_b_tx_w14,		2),
	GROUP(uart_b_rx_w15,		2),

	/* BANK D func1 */
	GROUP(uart_b_tx_d,		1),
	GROUP(uart_b_rx_d,		1),
	GROUP(i2c1_sck_d2,		1),
	GROUP(i2c1_sda_d3,		1),
	GROUP(clk_32k_in,		1),
	GROUP(remote_input,		1),
	GROUP(jtag_a_clk,		1),
	GROUP(jtag_a_tms,		1),
	GROUP(jtag_a_tdi,		1),
	GROUP(jtag_a_tdo,		1),
	GROUP(clk12_24M_d,		1),
	GROUP(pwm_a_d,			1),
	GROUP(pwm_f_d12,		1),
	GROUP(i2c1_sck_d13,		1),
	GROUP(i2c1_sda_d14,		1),

	/* BANK D func2 */
	GROUP(remote_out_d1,		2),
	GROUP(i2c_slave_sck,		2),
	GROUP(i2c_slave_sda,		2),
	GROUP(atv_if_agc_d,		2),
	GROUP(pwm_c_d5,			2),
	GROUP(pwm_f_d6,			2),
	GROUP(pwm_c_d7,			2),
	GROUP(spdif_out_a_d,		2),
	GROUP(pwm_d_d9,			2),
	GROUP(pwm_e_d10,		2),
	GROUP(pwm_c_d11,		2),
	GROUP(pcieck_reqin,		2),
	GROUP(uart_d_tx_d,		2),
	GROUP(uart_d_rx_d,		2),

	/* BANK D func3 */
	GROUP(cicam_waitn_d,		3),
	GROUP(cicam_irqn,		3),
	GROUP(demod_uart_tx_d,		3),
	GROUP(demod_uart_rx_d,		3),
	GROUP(dtv_if_agc_d,		3),
	GROUP(pwm_d_hiz,		3),
	GROUP(pwm_c_hiz,		3),
	GROUP(spdif_in_d,		3),
	GROUP(remote_out_d9,		3),
	GROUP(gen_clk_out_d,		3),
	GROUP(pwm_i_d11,		3),
	GROUP(pwm_j_d,			3),

	/* BANK D func4 */
	GROUP(uart_c_tx_d,		4),
	GROUP(uart_c_rx_d,		4),
	GROUP(tdm_d2_d,			4),
	GROUP(tdm_d3_d,			4),

	/* BANK D func5 */
	GROUP(pwm_i_d6,			5),
	GROUP(dcon_led_d7,		5),
	GROUP(dcon_led_d8,		5),
	GROUP(dcon_led_d9,		5),

	/* BANK D func6 */
	GROUP(pwm_h_d,			6),
	GROUP(mclk_1_d,			6),
	GROUP(spdif_out_b_d,		6),
	GROUP(pwm_vs_d,			6),

	/* BANK D func7 */
	GROUP(i2c3_sck_d,		7),
	GROUP(i2c3_sda_d,		7),
	GROUP(i2c4_sck_d,		7),
	GROUP(i2c4_sda_d,		7),

	/* BANK E func1 */
	GROUP(pwm_a_e,			1),
	GROUP(pwm_b_e,			1),
	GROUP(pwm_e_e,			1),

	/* BANK E func2 */
	GROUP(i2c2_sck_e,		2),
	GROUP(i2c2_sda_e,		2),
	GROUP(clk12_24M_e,		2),

	/* BANK B func1 */
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
	GROUP(loop_gpio_b12,		1),

	/* BANK B func2 */
	GROUP(loop_gpio_h21,		2),

	/* BANK B func3 */
	GROUP(spif_hold,		3),
	GROUP(spif_mo,			3),
	GROUP(spif_mi,			3),
	GROUP(spif_clk,			3),
	GROUP(spif_wp,			3),
	GROUP(remote_out_b,		3),
	GROUP(spif_cs,			3),

	/* BANK C func1 */
	GROUP(sdcard_d0_c,		1),
	GROUP(sdcard_d1_c,		1),
	GROUP(sdcard_d2_c,		1),
	GROUP(sdcard_d3_c,		1),
	GROUP(sdcard_clk_c,		1),
	GROUP(sdcard_cmd_c,		1),
	GROUP(uart_a_tx_c,		1),
	GROUP(uart_a_rx_c,		1),
	GROUP(uart_a_cts_c,		1),
	GROUP(uart_a_rts_c,		1),
	GROUP(pwm_f_c,			1),

	/* BANK C func2 */
	GROUP(spi2_miso_c,		2),
	GROUP(spi2_mosi_c,		2),
	GROUP(spi2_clk_c,		2),
	GROUP(spi2_ss0_c,		2),
	GROUP(spi2_ss1_c,		2),
	GROUP(spi2_ss2_c,		2),

	/* BANK C func3 */
	GROUP(cicam_a2_c,		3),
	GROUP(cicam_a3_c,		3),
	GROUP(cicam_a4_c,		3),
	GROUP(cicam_a5_c,		3),
	GROUP(cicam_a6_c,		3),
	GROUP(cicam_a7_c,		3),
	GROUP(cicam_a8_c,		3),
	GROUP(cicam_a9_c,		3),
	GROUP(cicam_a10_c,		3),
	GROUP(cicam_a11_c,		3),

	/* BANK C func4 */
	GROUP(tsin_c_clk_c,		4),
	GROUP(tsin_c_sop_c,		4),
	GROUP(tsin_c_valid_c,		4),
	GROUP(tsin_c_d0_c,		4),
	GROUP(spi2_dq2_c,		4),
	GROUP(spi2_dq3_c,		4),
	GROUP(demod_uart_tx_c,		4),
	GROUP(demod_uart_rx_c,		4),

	/* BANK C func5 */
	GROUP(spi1_ss1_c,		5),
	GROUP(tdm_sclk2_c,		5),
	GROUP(tdm_fs2_c,		5),
	GROUP(tdm_d6_c,			5),
	GROUP(tdm_d7_c,			5),
	GROUP(tdm_fs1_c,		5),
	GROUP(tdm_sclk1_c,		5),
	GROUP(tdm_d4_c,			5),
	GROUP(tdm_d5_c,			5),

	/* BANK C func6 */
	GROUP(spi3_miso_c,		6),
	GROUP(spi3_mosi_c,		6),
	GROUP(spi3_clk_c,		6),
	GROUP(spi3_ss0_c,		6),
	GROUP(spi4_miso_c,		6),
	GROUP(spi4_clk_c,		6),
	GROUP(spi4_ss0_c,		6),
	GROUP(spi5_mosi_c,		6),
	GROUP(spi5_clk_c,		6),

	/* BANK C func7 */

	/* BANK Z func1 */
	GROUP(tdm_fs2_z,		1),
	GROUP(tdm_sclk2_z,		1),
	GROUP(tdm_d4_z,			1),
	GROUP(tdm_d5_z,			1),
	GROUP(tdm_d6_z,			1),
	GROUP(tdm_d7_z,			1),
	GROUP(mclk_2_z,			1),
	GROUP(tsout_clk,		1),
	GROUP(tsout_sop,		1),
	GROUP(tsout_valid,		1),
	GROUP(tsout_d0,			1),
	GROUP(tsout_d1,			1),
	GROUP(tsout_d2,			1),
	GROUP(tsout_d3,			1),
	GROUP(tsout_d4,			1),
	GROUP(tsout_d5,			1),
	GROUP(tsout_d6,			1),
	GROUP(tsout_d7,			1),
	GROUP(dcon_led_z18,		1),
	GROUP(spdif_out_a_z19,		1),

	/* BANK Z func2 */
	GROUP(pwm_j_z0,			2),
	GROUP(tsin_a_sop_z,		2),
	GROUP(tsin_a_valid_z,		2),
	GROUP(tsin_a_d0_z,		2),
	GROUP(dtv_rf_agc_z6,		2),
	GROUP(cicam_a2_z,		2),
	GROUP(cicam_a3_z,		2),
	GROUP(cicam_a4_z,		2),
	GROUP(cicam_a5_z,		2),
	GROUP(cicam_a6_z,		2),
	GROUP(cicam_a7_z,		2),
	GROUP(cicam_a8_z,		2),
	GROUP(cicam_a9_z,		2),
	GROUP(cicam_a10_z,		2),
	GROUP(cicam_a11_z,		2),
	GROUP(dtv_rf_agc_z18,		2),
	GROUP(spdif_in_z,		2),

	/* BANK Z func3 */
	GROUP(cicam_a12_z,		3),
	GROUP(iso7816_clk_z,		3),
	GROUP(cicam_a13,		3),
	GROUP(cicam_a14,		3),
	GROUP(i2c0_sck,			3),
	GROUP(i2c0_sda,			3),
	GROUP(atv_if_agc_z6,		3),
	GROUP(atv_if_agc_z18,		3),
	GROUP(spdif_out_b_z,		3),

	/* BANK Z func4 */
	GROUP(pwm_vs_z0,		4),
	GROUP(spi1_miso_z,		4),
	GROUP(spi1_mosi_z,		4),
	GROUP(spi1_clk_z,		4),
	GROUP(spi1_ss0_z,		4),
	GROUP(spi1_dq2_z,		4),
	GROUP(spi1_dq3_z,		4),
	GROUP(dtv_if_agc_z6,		4),
	GROUP(dtv_if_agc_z18,		4),
	GROUP(pwm_g_z19,		4),

	/* BANK Z func5 */
	GROUP(spi5_miso_c,		5),
	GROUP(uart_a_rx_z,		5),
	GROUP(uart_a_cts_z,		5),
	GROUP(uart_a_rts_z,		5),
	GROUP(pwm_b_z,			5),
	GROUP(pwm_d_z,			5),
	GROUP(pwm_e_z,			5),
	GROUP(uart_d_tx_z,		5),
	GROUP(uart_d_rx_z,		5),
	GROUP(pwm_g_z12,		5),
	GROUP(pwm_h_z,			5),
	GROUP(pwm_i_z,			5),
	GROUP(pwm_j_z15,		5),
	GROUP(spi0_ss1_z,		5),
	GROUP(spi1_miso_z,		5),

	/* BANK Z func6 */
	GROUP(pcieck_reqn_z,		6),
	GROUP(pdm_din2_z,		6),
	GROUP(pdm_dclk_z,		6),
	GROUP(pdm_din0_z,		6),
	GROUP(pdm_din1_z6,		6),
	GROUP(tdm_d1_z,			6),
	GROUP(tdm_d0_z,			6),
	GROUP(tdm_sclk1_z,		6),
	GROUP(tdm_fs1_z,		6),
	GROUP(pwm_vs_z18,		6),
	GROUP(diseqc_out_z19,		6),

	/* BANK Z func7 */
	GROUP(dcon_led_z0,		7),
	GROUP(s2_demod_gpio0_z,		7),
	GROUP(spdif_out_a_z2,		7),
	GROUP(spi0_ss1_z3,		7),
	GROUP(eth_mdio,			7),
	GROUP(eth_mdc,			7),
	GROUP(eth_link_led_z,		7),
	GROUP(eth_rgmii_rx_clk,		7),
	GROUP(eth_rx_dv,		7),
	GROUP(eth_rxd0,			7),
	GROUP(eth_rxd1,			7),
	GROUP(eth_rxd2_rgmii,		7),
	GROUP(eth_rxd3_rgmii,		7),
	GROUP(eth_rgmii_tx_clk,		7),
	GROUP(eth_txen,			7),
	GROUP(eth_txd0,			7),
	GROUP(eth_txd1,			7),
	GROUP(eth_txd2_rgmii,		7),
	GROUP(eth_txd3_rgmii,		7),
	GROUP(eth_act_led_z,		7),

	/* BANK H func1 */
	GROUP(tcon_0,			1),
	GROUP(tcon_1,			1),
	GROUP(tcon_2,			1),
	GROUP(tcon_3,			1),
	GROUP(tcon_4,			1),
	GROUP(tcon_5,			1),
	GROUP(tcon_6,			1),
	GROUP(tcon_7,			1),
	GROUP(tcon_8,			1),
	GROUP(tcon_9,			1),
	GROUP(tcon_10,			1),
	GROUP(tcon_11,			1),
	GROUP(tcon_12,			1),
	GROUP(tcon_13,			1),
	GROUP(tcon_14,			1),
	GROUP(tcon_15,			1),
	GROUP(hsync,			1),
	GROUP(vsync,			1),
	GROUP(i2c2_sck_h20,		1),
	GROUP(i2c2_sda_h21,		1),
	GROUP(i2c3_sck_h22,		1),
	GROUP(i2c3_sda_h23,		1),
	GROUP(i2c3_sck_h24,		1),
	GROUP(i2c3_sda_h25,		1),
	GROUP(i2c4_sck_h26,		1),
	GROUP(i2c4_sda_h27,		1),
	GROUP(i2c3_sck_h28,		1),
	GROUP(i2c3_sda_h29,		1),

	/* BANK H func2 */
	GROUP(tcon_lock,		2),
	GROUP(sync_3d_out,		2),
	GROUP(tdm_d3_h6,		2),
	GROUP(spi0_ss1_h7,		2),
	GROUP(spi0_ss0_h8,		2),
	GROUP(spi0_miso_h9,		2),
	GROUP(spi0_mosi_h10,		2),
	GROUP(spi0_clk_h11,		2),
	GROUP(spi0_ss2_h12,		2),
	GROUP(tdm_d3_h13,		2),
	GROUP(mclk_1_h,			2),
	GROUP(tdm_fs1_h15,		2),
	GROUP(tdm_sclk1_h,		2),
	GROUP(tdm_d0_h,			2),
	GROUP(tdm_d1_h,			2),
	GROUP(tdm_d2_h19,		2),
	GROUP(tdm_d3_h20,		2),
	GROUP(tdm_d4_h,			2),
	GROUP(tdm_d5_h,			2),
	GROUP(tdm_d6_h,			2),
	GROUP(spi0_ss0_h26,		2),
	GROUP(spi0_miso_h27,		2),
	GROUP(spi0_mosi_h28,		2),
	GROUP(spi0_clk_h29,		2),

	/* BANK H func3 */
	GROUP(tcon_sfc_h0,		3),
	GROUP(tcon_sfc_h1,		3),
	GROUP(uart_c_tx_h2,		3),
	GROUP(uart_c_rx_h3,		3),
	GROUP(uart_c_cts_h4,		3),
	GROUP(uart_c_rts_h5,		3),
	GROUP(i2c2_sck_h10,		3),
	GROUP(i2c2_sda_h11,		3),
	GROUP(pwm_vs_h12,		3),
	GROUP(pwm_vs_h13,		3),
	GROUP(pdm_din1_h14,		3),
	GROUP(pdm_din2_h16,		3),
	GROUP(pdm_din3_h17,		3),
	GROUP(pdm_din0_h18,		3),
	GROUP(pdm_dclk_h19,		3),
	GROUP(loop_gpio_b13,		3),
	GROUP(pdm_dclk_h22,		3),
	GROUP(pdm_din2_h23,		3),
	GROUP(i2c2_sck_h24,		3),
	GROUP(i2c2_sda_h25,		3),
	GROUP(pdm_din0_h26,		3),
	GROUP(pdm_din1_h27,		3),
	GROUP(pdm_din2_h28,		3),
	GROUP(pdm_dclk_h29,		3),

	/* BANK H func4 */
	GROUP(vx1_a_lockn,		4),
	GROUP(vx1_a_htpdn,		4),
	GROUP(vx1_b_lockn,		4),
	GROUP(vx1_b_htpdn,		4),
	GROUP(pwm_d_h5,			4),
	GROUP(pwm_d_h12,		4),
	GROUP(pwm_e_h,			4),
	GROUP(tdm_d3_h14,		4),
	GROUP(eth_act_led_h,		4),
	GROUP(eth_link_led_h,		4),
	GROUP(i2c4_sck_h20,		4),
	GROUP(i2c4_sda_h21,		4),
	GROUP(uart_c_cts_h22,		4),
	GROUP(uart_c_rts_h23,		4),
	GROUP(uart_c_tx_h24,		4),
	GROUP(uart_c_rx_h25,		4),
	GROUP(uart_c_rts_h26,		4),
	GROUP(uart_c_tx_h27,		4),
	GROUP(uart_c_rx_h28,		4),

	/* BANK H func5 */
	GROUP(clk12_24M_h,		5),
	GROUP(spi0_clk_h14,		5),
	GROUP(tdm_fs1_h18,		5),
	GROUP(spi0_ss1_h19,		5),
	GROUP(demod_uart_tx_h,		5),
	GROUP(demod_uart_rx_h,		5),
	GROUP(spi1_ss0_h,		5),
	GROUP(spi1_miso_h,		5),
	GROUP(spi1_mosi_h,		5),
	GROUP(spi1_clk_h,		5),
	GROUP(tdm_d2_h26,		5),
	GROUP(tdm_d3_h27,		5),
	GROUP(tdm_fs2_h28,		5),
	GROUP(tdm_sclk2_h29,		5),

	/* BANK H func6 */
	GROUP(gen_clk_out_h,		6),
	GROUP(s2_demod_gpio0_h,		6),
	GROUP(s2_demod_gpio1_h,		6),
	GROUP(s2_demod_gpio2_h,		6),
	GROUP(s2_demod_gpio3_h,		6),
	GROUP(s2_demod_gpio4_h,		6),
	GROUP(s2_demod_gpio5_h,		6),
	GROUP(s2_demod_gpio6_h,		6),
	GROUP(s2_demod_gpio7_h,		6),
	GROUP(tdm_fs2_h22,		6),
	GROUP(tdm_sclk2_h23,		6),
	GROUP(spi0_ss1_h24,		6),
	GROUP(dcon_led_h,		6),
	GROUP(spi1_dq2_h,		6),
	GROUP(spi1_dq3_h,		6),

	/* BANK P func1 */
	GROUP(spi2_miso_p,		1),
	GROUP(spi2_mosi_p,		1),
	GROUP(spi2_clk_p,		1),
	GROUP(spi2_ss0_p,		1),
	GROUP(i2c4_sck_p,		1),
	GROUP(i2c4_sda_p,		1),
	GROUP(mclk_2_p,			1),
	GROUP(tdm_fs2_p,		1),
	GROUP(tdm_sclk2_p,		1),
	GROUP(tdm_d2_p,			1),

	/* BANK P func2 */
	GROUP(uart_c_tx_p,		2),
	GROUP(uart_c_rx_p,		2),
	GROUP(uart_c_cts_p,		2),
	GROUP(uart_c_rts_p,		2),
	GROUP(pwm_i_p,			2),
	GROUP(pwm_j_p,			2),
	GROUP(tdm_d4_p,			2),
	GROUP(tdm_d3_p,			2),

	/* BANK P func3 */
	GROUP(spdif_out_a_p,		3),
	GROUP(pwm_g_p1,			3),
	GROUP(pwm_h_p2,			3),
	GROUP(spi0_ss2_p,		3),
	GROUP(spi2_dq2_p,		3),
	GROUP(spi2_dq3_p,		3),
	GROUP(pwm_e_p6,			3),
	GROUP(pwm_g_p7,			3),
	GROUP(pwm_h_p8,			3),
	GROUP(dcon_led_p,		3),

	/* BANK P func4 */
	GROUP(spi1_ss1_p,		4),
	GROUP(pcieck_reqn_p,		4),
	GROUP(remote_out_p6,		4),
	GROUP(spi4_ss1_p,		4),
	GROUP(remote_out_p9,		4),

	/* BANK P func5 */
	GROUP(spi3_miso_p,		5),
	GROUP(spi3_mosi_p,		5),
	GROUP(spi3_clk_p,		5),
	GROUP(spi3_ss0_p,		5),
	GROUP(spi4_mosi_p,		5),
	GROUP(spi4_clk_p,		5),
	GROUP(spi4_ss0_p,		5),
	GROUP(spi5_mosi_p,		5),
	GROUP(spi5_clk_p,		5),
	GROUP(spi5_ss0_p,		5),

	/* BANK P func6 */
	GROUP(spdif_out_b_p,		6),
	GROUP(tsin_c_clk_p,		6),
	GROUP(tsin_c_sop_p,		6),
	GROUP(tsin_c_valid_p,		6),
	GROUP(tsin_c_d0_p,		6),

	/* BANK M func1 */
	GROUP(tsin_b_clk,		1),
	GROUP(tsin_b_sop,		1),
	GROUP(tsin_b_valid,		1),
	GROUP(tsin_b_d0,		1),
	GROUP(tsin_b_d1,		1),
	GROUP(tsin_b_d2,		1),
	GROUP(tsin_b_d3,		1),
	GROUP(tsin_b_d4,		1),
	GROUP(tsin_b_d5,		1),
	GROUP(tsin_b_d6,		1),
	GROUP(tsin_b_d7,		1),
	GROUP(cicam_data0,		1),
	GROUP(cicam_data1,		1),
	GROUP(cicam_data2,		1),
	GROUP(cicam_data3,		1),
	GROUP(cicam_data4,		1),
	GROUP(cicam_data5,		1),
	GROUP(cicam_data6,		1),
	GROUP(cicam_data7,		1),
	GROUP(cicam_a0,			1),
	GROUP(cicam_a1,			1),
	GROUP(cicam_cen,		1),
	GROUP(cicam_oen,		1),
	GROUP(cicam_wen,		1),
	GROUP(cicam_iordn,		1),
	GROUP(cicam_iowrn,		1),
	GROUP(cicam_reset,		1),
	GROUP(cicam_cdn,		1),
	GROUP(cicam_5ven,		1),
	GROUP(cicam_waitn_m,		1),

	/* BANK M func2 */
	GROUP(spi2_miso_m,		2),
	GROUP(spi2_mosi_m,		2),
	GROUP(spi2_clk_m,		2),
	GROUP(spi2_ss0_m,		2),
	GROUP(pdm_din2_m,		2),
	GROUP(pdm_din3_m,		2),
	GROUP(pdm_din0_m,		2),
	GROUP(pdm_dclk_m,		2),
	GROUP(mclk_2_m,			2),
	GROUP(tdm_fs2_m,		2),
	GROUP(tdm_sclk2_m,		2),
	GROUP(tdm_d4_m,			2),
	GROUP(tdm_d5_m,			2),
	GROUP(tdm_d6_m,			2),
	GROUP(tdm_d7_m,			2),
	GROUP(iso7816_clk_m,		2),
	GROUP(iso7816_data_m,		2),
	GROUP(mclk_1_m,			2),
	GROUP(tdm_fs1_m,		2),
	GROUP(tdm_sclk1_m,		2),
	GROUP(tdm_d0_m,			2),
	GROUP(tdm_d1_m,			2),
	GROUP(tdm_d2_m,			2),
	GROUP(tdm_d3_m,			2),

	/* BANK M func3 */
	GROUP(uart_a_tx_m,		3),
	GROUP(uart_a_rx_m,		3),
	GROUP(uart_a_cts_m,		3),
	GROUP(uart_a_rts_m,		3),
	GROUP(sdcard_d0_m,		3),
	GROUP(sdcard_d1_m,		3),
	GROUP(sdcard_d2_m,		3),
	GROUP(sdcard_d3_m,		3),
	GROUP(sdcard_clk_m,		3),
	GROUP(sdcard_cmd_m,		3),
	GROUP(pwm_f_m10,		3),
	GROUP(pwm_i_m,			3),
	GROUP(pwm_j_m,			3),
	GROUP(i2c4_sck_m,		3),
	GROUP(i2c4_sda_m,		3),
	GROUP(spi0_miso_m,		3),
	GROUP(spi0_mosi_m,		3),
	GROUP(spi0_clk_m,		3),
	GROUP(spi0_ss0_m,		3),
	GROUP(spi0_ss1_m,		3),
	GROUP(spi0_ss2_m,		3),
	GROUP(i2c2_sck_m,		3),
	GROUP(i2c2_sda_m,		3),
	GROUP(i2c3_sck_m,		3),
	GROUP(i2c3_sda_m,		3),
	GROUP(pcieck_reqn_m,		3),

	/* BANK M func4 */
	GROUP(pwm_d_m1,			4),
	GROUP(uart_d_tx_m5,		4),
	GROUP(uart_d_rx_m6,		4),
	GROUP(pwm_g_m,			4),
	GROUP(pwm_h_m,			4),
	GROUP(uart_d_tx_m17,		4),
	GROUP(uart_d_rx_m17,		4),
	GROUP(uart_c_tx_m,		4),
	GROUP(uart_c_rx_m,		4),
	GROUP(uart_c_cts_m,		4),
	GROUP(uart_c_rts_m,		4),
	GROUP(pwm_d_m23,		4),
	GROUP(pwm_e_m,			4),
	GROUP(pwm_f_m26,		4),
	GROUP(gen_clk_out_m,		4),

	/* BANK M func5 */
	GROUP(tsin_a_clk_m,		5),
	GROUP(tsin_a_sop_m,		5),
	GROUP(tsin_a_valid_m,		5),
	GROUP(tsin_a_d0_m,		5),
	GROUP(demod_uart_tx_m23,	5),
	GROUP(demod_uart_rx_m24,	5),
	GROUP(demod_uart_tx_m25,	5),
	GROUP(demod_uart_rx_m26,	5),
	GROUP(spdif_in_m,		5),
};

static const char * const gpio_periphs_groups[] = {
	"GPIOW_0", "GPIOW_1", "GPIOW_2", "GPIOW_3", "GPIOW_4",
	"GPIOW_5", "GPIOW_6", "GPIOW_7", "GPIOW_8", "GPIOW_9", "GPIOW_10",
	"GPIOW_11", "GPIOW_12", "GPIOW_13", "GPIOW_14", "GPIOW_15",
	"GPIOW_16",

	"GPIOD_0", "GPIOD_1", "GPIOD_2", "GPIOD_3", "GPIOD_4", "GPIOD_5",
	"GPIOD_6", "GPIOD_7", "GPIOD_8", "GPIOD_9", "GPIOD_10", "GPIOD_11",
	"GPIOD_12", "GPIOD_13", "GPIOD_14",

	"GPIOE_0", "GPIOE_1",

	"GPIOB_0", "GPIOB_1", "GPIOB_2", "GPIOB_3", "GPIOB_4", "GPIOB_5",
	"GPIOB_6", "GPIOB_7", "GPIOB_8", "GPIOB_9", "GPIOB_10",
	"GPIOB_11", "GPIOB_12", "GPIOB_13",

	"GPIOC_0", "GPIOC_1", "GPIOC_2", "GPIOC_3", "GPIOC_4", "GPIOC_5",
	"GPIOC_6", "GPIOC_7", "GPIOC_8", "GPIOC_9", "GPIOC_10",

	"GPIOZ_0", "GPIOZ_1", "GPIOZ_2", "GPIOZ_3", "GPIOZ_4", "GPIOZ_5",
	"GPIOZ_6", "GPIOZ_7", "GPIOZ_8", "GPIOZ_9", "GPIOZ_10",
	"GPIOZ_11", "GPIOZ_12", "GPIOZ_13", "GPIOZ_14", "GPIOZ_15",
	"GPIOZ_16", "GPIOZ_17", "GPIOZ_18", "GPIOZ_19",

	"GPIOH_0", "GPIOH_1", "GPIOH_2", "GPIOH_3", "GPIOH_4", "GPIOH_5",
	"GPIOH_6", "GPIOH_7", "GPIOH_8", "GPIOH_9", "GPIOH_10",
	"GPIOH_11", "GPIOH_12", "GPIOH_13", "GPIOH_14", "GPIOH_15", "GPIOH_16",
	"GPIOH_17", "GPIOH_18", "GPIOH_19", "GPIOH_20", "GPIOH_21",
	"GPIOH_22", "GPIOH_23", "GPIOH_24", "GPIOH_25", "GPIOH_26",
	"GPIOH_27", "GPIOH_28", "GPIOH_29",

	"GPIOP_0", "GPIOP_1", "GPIOP_2", "GPIOP_3", "GPIOP_4", "GPIOP_5",
	"GPIOP_6", "GPIOP_7", "GPIOP_8", "GPIOP_9",

	"GPIOM_0", "GPIOM_1", "GPIOM_2", "GPIOM_3", "GPIOM_4", "GPIOM_5",
	"GPIOM_6", "GPIOM_7", "GPIOM_8", "GPIOM_9", "GPIOM_10", "GPIOM_11",
	"GPIOM_12", "GPIOM_13", "GPIOM_14", "GPIOM_15", "GPIOM_16", "GPIOM_17",
	"GPIOM_18", "GPIOM_19", "GPIOM_20", "GPIOM_21", "GPIOM_22", "GPIOM_23",
	"GPIOM_24", "GPIOM_25", "GPIOM_26", "GPIOM_27", "GPIOM_28",
	"GPIOM_29",

	"GPIO_TEST_N"
};

static const char * const emmc_groups[] = {
	"emmc_nand_d0", "emmc_nand_d1", "emmc_nand_d2", "emmc_nand_d3",
	"emmc_nand_d4", "emmc_nand_d5", "emmc_nand_d6", "emmc_nand_d7",
	"emmc_clk", "emmc_rst", "emmc_cmd", "emmc_nand_ds"
};

static const char * const spif_groups[] = {
	"spif_hold", "spif_mo", "spif_mi", "spif_clk",
	"spif_wp", "spif_cs"
};

static const char * const remote_out_groups[] = {
	"remote_out_b", "remote_out_d1", "remote_out_d9", "remote_out_p6",
	"remote_out_p9"
};

static const char * const hdmirx_a_groups[] = {
	"hdmirx_a_hpd", "hdmirx_a_det", "hdmirx_a_sda", "hdmirx_a_sck"
};

static const char * const hdmirx_b_groups[] = {
	"hdmirx_b_hpd", "hdmirx_b_det", "hdmirx_b_sda", "hdmirx_b_sck"

};

static const char * const hdmirx_c_groups[] = {
	"hdmirx_c_hpd", "hdmirx_c_det", "hdmirx_c_sda", "hdmirx_c_sck"
};

static const char * const hdmirx_d_groups[] = {
	"hdmirx_d_hpd", "hdmirx_d_det", "hdmirx_d_sda", "hdmirx_d_sck"
};

static const char * const cec_groups[] = {
	"cec"
};

static const char * const uart_a_groups[] = {
	"uart_a_tx_c", "uart_a_rx_c", "uart_a_rx_z",
	"uart_a_tx_m", "uart_a_rx_m", "uart_a_cts_c",
	"uart_a_rts_c", "uart_a_cts_z", "uart_a_rts_z",
	"uart_a_cts_m", "uart_a_rts_m"
};

static const char * const uart_b_groups[] = {
	"uart_b_tx_d", "uart_b_rx_d", "uart_b_tx_w2",
	"uart_b_rx_w3", "uart_b_tx_w6", "uart_b_rx_w7",
	"uart_b_tx_w10", "uart_b_rx_w11", "uart_b_tx_w14",
	"uart_b_rx_w15"
};

static const char * const uart_c_groups[] = {
	"uart_c_tx_d", "uart_c_rx_d", "uart_c_tx_p", "uart_c_rx_p",
	"uart_c_tx_m", "uart_c_rx_m", "uart_c_tx_h2", "uart_c_rx_h3",
	"uart_c_cts_p", "uart_c_rts_p", "uart_c_cts_m", "uart_c_rts_m",
	"uart_c_cts_h4", "uart_c_rts_h5", "uart_c_tx_h24", "uart_c_rx_h25",
	"uart_c_tx_h27", "uart_c_rx_h28", "uart_c_cts_h22", "uart_c_rts_h23",
	"uart_c_rts_h26"
};

static const char * const uart_d_groups[] = {
	"uart_d_tx_d", "uart_d_rx_d", "uart_d_tx_z", "uart_d_rx_z",
	"uart_d_tx_m5", "uart_d_rx_m6", "uart_d_tx_m17", "uart_d_rx_m17"
};

static const char * const sdcard_groups[] = {
	"sdcard_d0_c", "sdcard_d1_c", "sdcard_d2_c", "sdcard_d3_c",
	"sdcard_clk_c", "sdcard_cmd_c",

	"sdcard_d0_m", "sdcard_d1_m", "sdcard_d2_m", "sdcard_d3_m",
	"sdcard_clk_m", "sdcard_cmd_m",
};

static const char * const tdm_groups[] = {
	"tdm_d2_d", "tdm_d3_d", "tdm_fs2_c", "tdm_d6_c", "tdm_d7_c",
	"tdm_fs1_c", "tdm_d4_c", "tdm_d5_c", "tdm_fs2_z",
	"tdm_d4_z", "tdm_d5_z", "tdm_d6_z", "tdm_d7_z", "tdm_d1_z",
	"tdm_d0_z", "tdm_fs1_z", "tdm_d3_h6", "tdm_d0_h", "tdm_d1_h",
	"tdm_d4_h", "tdm_d5_h", "tdm_d6_h", "tdm_fs2_p", "tdm_d2_p", "tdm_d4_p",
	"tdm_d3_p", "tdm_fs2_m", "tdm_d4_m", "tdm_d5_m",
	"tdm_d6_m", "tdm_d7_m", "tdm_fs1_m", "tdm_d0_m", "tdm_d1_m",
	"tdm_d2_m", "tdm_d3_m", "tdm_d3_h13", "tdm_d2_h19", "tdm_d3_h20",
	"tdm_d3_h14", "tdm_d2_h26", "tdm_d3_h27", "tdm_sclk2_c", "tdm_sclk1_c",
	"tdm_sclk2_z", "tdm_sclk1_z", "tdm_fs1_h15", "tdm_sclk1_h",
	"tdm_fs1_h18", "tdm_fs2_h28", "tdm_fs2_h22", "tdm_sclk2_p",
	"tdm_sclk2_m", "tdm_sclk1_m", "tdm_sclk2_h29", "tdm_sclk2_h23"
};

static const char * const i2c0_groups[] = {
	"i2c0_sck", "i2c0_sda"
};

static const char * const i2c1_groups[] = {
	"i2c1_sck_d2", "i2c1_sda_d3", "i2c1_sck_d13", "i2c1_sda_d14"
};

static const char * const i2c2_groups[] = {
	"i2c2_sck_e", "i2c2_sda_e",
	"i2c2_sck_m", "i2c2_sda_m",
	"i2c2_sck_h20", "i2c2_sda_h21",
	"i2c2_sck_h10", "i2c2_sda_h11",
	"i2c2_sck_h24", "i2c2_sda_h25"
};

static const char * const i2c3_groups[] = {
	"i2c3_sck_d", "i2c3_sda_d",
	"i2c3_sck_m", "i2c3_sda_m",
	"i2c3_sck_h22", "i2c3_sda_h23",
	"i2c3_sck_h24", "i2c3_sda_h25",
	"i2c3_sck_h28", "i2c3_sda_h29",
};

static const char * const i2c4_groups[] = {
	"i2c4_sck_d", "i2c4_sda_d", "i2c4_sck_p", "i2c4_sda_p",
	"i2c4_sck_m", "i2c4_sda_m", "i2c4_sck_h26", "i2c4_sda_h27",
	"i2c4_sck_h20", "i2c4_sda_h21"
};

static const char * const atv_groups[] = {
	"atv_if_agc_d", "atv_if_agc_z6", "atv_if_agc_z18"
};

static const char * const dtv_groups[] = {
	"dtv_if_agc_d", "dtv_rf_agc_z6", "dtv_if_agc_z6",
	"dtv_if_agc_z18", "dtv_rf_agc_z18"
};

static const char * const pwm_a_groups[] = {
	"pwm_a_d", "pwm_a_e",
};

static const char * const pwm_b_groups[] = {
	"pwm_b_z", "pwm_b_e",
};

static const char * const pwm_c_groups[] = {
	"pwm_c_d5", "pwm_c_d7", "pwm_c_d11"
};

static const char * const pwm_d_groups[] = {
	"pwm_d_z", "pwm_d_d9", "pwm_d_h5", "pwm_d_h12",
	"pwm_d_m1", "pwm_d_m23"
};

static const char * const pwm_e_groups[] = {
	"pwm_e_e", "pwm_e_z", "pwm_e_h", "pwm_e_m",
	"pwm_e_d10", "pwm_e_p6"
};

static const char * const pwm_f_groups[] = {
	"pwm_f_c", "pwm_f_d12", "pwm_f_m10", "pwm_f_m26", "pwm_f_d6"
};

static const char * const pwm_g_groups[] = {
	"pwm_g_m", "pwm_g_z19", "pwm_g_z12", "pwm_g_p1",
	"pwm_g_p7"
};

static const char * const pwm_h_groups[] = {
	"pwm_h_d", "pwm_h_z", "pwm_h_m", "pwm_h_p2",
	"pwm_h_p8"
};

static const char * const pwm_i_groups[] = {
	"pwm_i_z", "pwm_i_p", "pwm_i_m", "pwm_i_d11",
	"pwm_i_d6"
};

static const char * const pwm_j_groups[] = {
	"pwm_j_d", "pwm_j_p", "pwm_j_m", "pwm_j_z0", "pwm_j_z15"
};

static const char * const pwm_c_hiz_groups[] = {
	"pwm_c_hiz"
};

static const char * const pwm_d_hiz_groups[] = {
	"pwm_d_hiz"
};

static const char * const pwm_vs_groups[] = {
	"pwm_vs_d", "pwm_vs_z0", "pwm_vs_z18", "pwm_vs_h12", "pwm_vs_h13"
};

static const char * const pdm_groups[] = {
	"pdm_din2_z", "pdm_dclk_z", "pdm_din0_z",
	"pdm_din2_m", "pdm_din3_m", "pdm_din0_m", "pdm_dclk_m",
	"pdm_din1_z6", "pdm_din1_h14", "pdm_din2_h16", "pdm_din3_h17",
	"pdm_din0_h18", "pdm_dclk_h19", "pdm_dclk_h22", "pdm_din2_h23",
	"pdm_din0_h26", "pdm_din1_h27", "pdm_din2_h28", "pdm_dclk_h29"
};

static const char * const tcon_groups[] = {
	"tcon_0", "tcon_1", "tcon_2", "tcon_3", "tcon_4",
	"tcon_5", "tcon_6", "tcon_7", "tcon_8", "tcon_9",
	"tcon_10", "tcon_11", "tcon_12", "tcon_13", "tcon_14", "tcon_15",
	"tcon_lock", "tcon_sfc_h0", "tcon_sfc_h1"
};

static const char * const hsync_groups[] = {
	"hsync"
};

static const char * const vsync_groups[] = {
	"vsync"
};

static const char * const sync_3d_out_groups[] = {
	"sync_3d_out"
};

static const char * const spi0_groups[] = {
	"spi0_ss1_z", "spi0_ss2_p", "spi0_clk_m", "spi0_ss0_m",
	"spi0_ss1_m", "spi0_ss2_m", "spi0_ss1_z3", "spi0_ss1_h7", "spi0_ss0_h8",
	"spi0_miso_m", "spi0_mosi_m", "spi0_miso_h9", "spi0_clk_h11",
	"spi0_ss2_h12", "spi0_ss0_h26", "spi0_clk_h29", "spi0_clk_h14",
	"spi0_ss1_h19", "spi0_mosi_h10", "spi0_miso_h27", "spi0_mosi_h28",
	"spi0_ss1_h24"
};

static const char * const spi1_groups[] = {
	"spi1_ss1_c", "spi1_clk_z", "spi1_ss0_z", "spi1_dq2_z", "spi1_dq3_z",
	"spi1_ss0_h", "spi1_clk_h", "spi1_ss1_p", "spi1_miso_z", "spi1_mosi_z",
	"spi1_miso_z", "spi1_miso_h", "spi1_mosi_h", "spi1_dq2_h", "spi1_dq3_h"
};

static const char * const spi2_groups[] = {
	"spi2_clk_c", "spi2_ss0_c", "spi2_ss1_c", "spi2_ss2_c", "spi2_dq2_c",
	"spi2_dq3_c", "spi2_clk_p", "spi2_ss0_p", "spi2_dq2_p", "spi2_dq3_p",
	"spi2_clk_m", "spi2_ss0_m", "spi2_miso_c", "spi2_mosi_c",
	"spi2_miso_p", "spi2_mosi_p", "spi2_miso_m", "spi2_mosi_m"
};

static const char * const spi3_groups[] = {
	"spi3_clk_c", "spi3_ss0_c", "spi3_clk_p", "spi3_ss0_p",
	"spi3_miso_c", "spi3_mosi_c", "spi3_miso_p", "spi3_mosi_p"
};

static const char * const spi4_groups[] = {
	"spi4_clk_c", "spi4_ss0_c", "spi4_ss1_p", "spi4_clk_p",
	"spi4_ss0_p", "spi4_miso_c", "spi4_mosi_p"
};

static const char * const spi5_groups[] = {
	"spi5_clk_c", "spi5_clk_p", "spi5_ss0_p", "spi5_mosi_c",
	"spi5_miso_c", "spi5_mosi_p"
};

static const char * const mclk_groups[] = {
	"mclk_1_d", "mclk_2_z", "mclk_1_h", "mclk_2_p",
	"mclk_2_m", "mclk_1_m"
};

static const char * const spdif_out_a_groups[] = {
	"spdif_out_a_d", "spdif_out_a_p", "spdif_out_a_z2", "spdif_out_a_z19"
};

static const char * const spdif_out_b_groups[] = {
	"spdif_out_b_d", "spdif_out_b_z", "spdif_out_p"
};

static const char * const spdif_in_groups[] = {
	"spdif_in_d", "spdif_in_z", "spdif_in_m"
};

static const char * const vx1_a_groups[] = {
	"vx1_a_lockn", "vx1_a_htpdn"
};

static const char * const vx1_b_groups[] = {
	"vx1_b_lockn", "vx1_b_htpdn"
};

static const char * const eth_groups[] = {
	"eth_mdc", "eth_mdio", "eth_rx_dv", "eth_rxd0", "eth_rxd1",
	"eth_txen", "eth_txd0", "eth_txd1", "eth_act_led_z",
	"eth_act_led_h", "eth_link_led_z", "eth_rxd2_rgmii", "eth_rxd3_rgmii",
	"eth_txd2_rgmii", "eth_txd3_rgmii", "eth_link_led_h",
	"eth_rgmii_rx_clk", "eth_rgmii_tx_clk"
};

static const char * const cicam_groups[] = {
	"cicam_a13", "cicam_a14", "cicam_a0", "cicam_a1", "cicam_cen",
	"cicam_oen", "cicam_wen", "cicam_cdn", "cicam_irqn", "cicam_a2_c",
	"cicam_a3_c", "cicam_a4_c", "cicam_a5_c", "cicam_a6_c", "cicam_a7_c",
	"cicam_a8_c", "cicam_a9_c", "cicam_a2_z", "cicam_a3_z", "cicam_a4_z",
	"cicam_a5_z", "cicam_a6_z", "cicam_a7_z", "cicam_a8_z", "cicam_a9_z",
	"cicam_5ven", "cicam_a10_c", "cicam_a11_c", "cicam_a10_z",
	"cicam_a11_z", "cicam_a12_z", "cicam_data0", "cicam_data1",
	"cicam_data2", "cicam_data3", "cicam_data4", "cicam_data5",
	"cicam_data6", "cicam_data7", "cicam_iordn", "cicam_iowrn",
	"cicam_reset", "cicam_waitn_d", "cicam_waitn_m"
};

static const char * const iso7816_groups[] = {
	"iso7816_clk_z", "iso7816_clk_m", "iso7816_data_m"
};

static const char * const tsout_groups[] = {
	"tsout_clk", "tsout_sop", "tsout_d0", "tsout_d1", "tsout_d2",
	"tsout_d3", "tsout_d4", "tsout_d5", "tsout_d6", "tsout_d7",
	"tsout_valid"
};

static const char * const tsin_a_groups[] = {
	"tsin_a_d0_z", "tsin_a_d0_m", "tsin_a_clk_z", "tsin_a_sop_z",
	"tsin_a_clk_m", "tsin_a_sop_m", "tsin_a_valid_z", "tsin_a_valid_m"
};

static const char * const tsin_b_groups[] = {
	"tsin_b_d0", "tsin_b_d1", "tsin_b_d2", "tsin_b_d3", "tsin_b_d4",
	"tsin_b_d5", "tsin_b_d6", "tsin_b_d7", "tsin_b_clk", "tsin_b_sop",
	"tsin_b_valid"
};

static const char * const tsin_c_groups[] = {
	"tsin_c_d0_c", "tsin_c_d0_p", "tsin_c_clk_c", "tsin_c_sop_c",
	"tsin_c_clk_p", "tsin_c_sop_p", "tsin_c_valid_c", "tsin_c_valid_p"
};

static const char * const diseqc_out_groups[] = {
	"diseqc_out_z0", "diseqc_out_z19"
};

static const char * const s2_demod_groups[] = {
	"s2_demod_gpio0_z", "s2_demod_gpio0_h", "s2_demod_gpio1_h",
	"s2_demod_gpio2_h", "s2_demod_gpio3_h", "s2_demod_gpio4_h",
	"s2_demod_gpio5_h", "s2_demod_gpio6_h", "s2_demod_gpio7_h"
};

static const char * const demod_uart_groups[] = {
	"demod_uart_tx_d", "demod_uart_rx_d", "demod_uart_tx_c",
	"demod_uart_rx_c", "demod_uart_tx_h", "demod_uart_rx_h",
	"demod_uart_tx_m23", "demod_uart_rx_m24", "demod_uart_tx_m25",
	"demod_uart_rx_m26"
};

static const char * const remote_input_groups[] = {
	"remote_input"
};

static const char * const clk_32k_in_groups[] = {
	"clk_32k_in"
};

static const char * const jtag_a_groups[] = {
	"jtag_a_clk", "jtag_a_tms", "jtag_a_tdi", "jtag_a_tdo"
};

static const char * const clk12_24M_groups[] = {
	"clk12_24M_d", "clk12_24M_e", "clk12_24M_h"
};

static const char * const dcon_led_groups[] = {
	"dcon_led_h", "dcon_led_p", "dcon_led_d7", "dcon_led_d8",
	"dcon_led_d9", "dcon_led_z0", "dcon_led_z18",
};

static const char * const pcieck_groups[] = {
	"pcieck_reqin", "pcieck_reqn_c", "pcieck_reqn_p",
	"pcieck_reqn_m"
};

static const char * const gen_clk_groups[] = {
	"gen_clk_out_d", "gen_clk_out_h", "gen_clk_out_m"
};

static struct meson_pmx_func meson_t3x_periphs_functions[] __initdata = {
	FUNCTION(gpio_periphs),
	FUNCTION(emmc),
	FUNCTION(spif),
	FUNCTION(remote_out),
	FUNCTION(hdmirx_a),
	FUNCTION(hdmirx_b),
	FUNCTION(hdmirx_c),
	FUNCTION(hdmirx_d),
	FUNCTION(cec),
	FUNCTION(uart_a),
	FUNCTION(uart_b),
	FUNCTION(uart_c),
	FUNCTION(uart_d),
	FUNCTION(sdcard),
	FUNCTION(tdm),
	FUNCTION(i2c0),
	FUNCTION(i2c1),
	FUNCTION(i2c2),
	FUNCTION(i2c3),
	FUNCTION(i2c4),
	FUNCTION(atv),
	FUNCTION(dtv),
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
	FUNCTION(pwm_vs),
	FUNCTION(pdm),
	FUNCTION(tcon),
	FUNCTION(hsync),
	FUNCTION(vsync),
	FUNCTION(sync_3d_out),
	FUNCTION(spi0),
	FUNCTION(spi1),
	FUNCTION(spi2),
	FUNCTION(spi3),
	FUNCTION(spi4),
	FUNCTION(spi5),
	FUNCTION(mclk),
	FUNCTION(spdif_out_a),
	FUNCTION(spdif_out_b),
	FUNCTION(spdif_in),
	FUNCTION(vx1_a),
	FUNCTION(vx1_b),
	FUNCTION(eth),
	FUNCTION(cicam),
	FUNCTION(iso7816),
	FUNCTION(tsout),
	FUNCTION(tsin_a),
	FUNCTION(tsin_b),
	FUNCTION(tsin_c),
	FUNCTION(diseqc_out),
	FUNCTION(s2_demod),
	FUNCTION(demod_uart),
	FUNCTION(diseqc_out),
	FUNCTION(remote_input),
	FUNCTION(jtag_a),
	FUNCTION(clk12_24M),
	FUNCTION(dcon_led),
	FUNCTION(clk_32k_in),
	FUNCTION(pcieck),
	FUNCTION(gen_clk)
};

static struct meson_bank meson_t3x_periphs_banks[] = {
	/* name  first  last  pullen  pull  dir  out  in */
	BANK("GPIOD_",    GPIOD_0,    GPIOD_14,
	     0x03,  0,   0x04,  0,   0x02,  0,  0x01,  0,  0x00,  0),
	BANK("GPIOE_",    GPIOE_0,    GPIOE_2,
	     0x0b,  0,   0x0c,  0,   0x0a,  0,  0x09,  0,  0x08,  0),
	BANK("GPIOZ_",    GPIOZ_0,    GPIOZ_19,
	     0x13,  0,   0x14,  0,   0x12,  0,  0x11,  0,  0x10,  0),
	BANK("GPIOH_",    GPIOH_0,    GPIOH_29,
	     0x1b,  0,   0x1c,  0,   0x1a, 0,   0x19,  0,  0x18,  0),
	BANK("GPIOB_",    GPIOB_0,    GPIOB_13,
	     0x2b,  0,   0x2c,  0,   0x2a, 0,   0x29,  0,  0x28,  0),
	BANK("GPIOC_",    GPIOC_0,    GPIOC_10,
	     0x33,  0,   0x34,  0,   0x32, 0,   0x31,  0,  0x30,  0),
	BANK("GPIOP_",    GPIOP_0,    GPIOP_9,
	     0x3b,  0,   0x3c,  0,   0x3a, 0,   0x39,  0,  0x38,  0),
	BANK("GPIOW_",    GPIOW_0,    GPIOW_16,
	     0x63,  0,  0x64,   0,  0x62,   0,  0x61,   0,  0x60, 0),
	BANK("GPIOM_",    GPIOM_0,    GPIOM_29,
	     0x73,  0,  0x74,   0,  0x72,   0,  0x71,   0,  0x70, 0),
	BANK("GPIO_TEST_N", GPIO_TEST_N,    GPIO_TEST_N,
	     0x83,  0,  0x84,  0,  0x82,   0,  0x81,   0,  0x80, 0)
};

static struct meson_pmx_bank meson_t3x_periphs_pmx_banks[] = {
	/*name	 first		last	   reg	offset  */
	BANK_PMX("B",    GPIOB_0,  GPIOB_13, 0x0,  0),
	BANK_PMX("C",	 GPIOC_0,  GPIOC_10, 0x2,  0),
	BANK_PMX("Z",	 GPIOZ_0,  GPIOZ_19, 0x4,  0),
	BANK_PMX("H",	 GPIOH_0,  GPIOH_29, 0x7,  0),
	BANK_PMX("D",    GPIOD_0,  GPIOD_14, 0xb,  0),
	BANK_PMX("E",    GPIOE_0,  GPIOE_2,  0xd,  0),
	BANK_PMX("M",    GPIOM_0,  GPIOM_29, 0xe,  0),
	BANK_PMX("W",    GPIOW_0,  GPIOW_16, 0x12, 0),
	BANK_PMX("P",	 GPIOP_0,  GPIOP_9,  0x14, 0),
	BANK_PMX("TSTN", GPIO_TEST_N,  GPIO_TEST_N, 0x15,  16)
};

static struct meson_drive_bank meson_t3x_periphs_drive_banks[] = {
	/*  name	    first         lask        reg  offset*/
	BANK_DRIVE("D",     GPIOD_0,     GPIOD_14,    0x7,   0),
	BANK_DRIVE("E",     GPIOE_0,     GPIOE_1,     0xf,   0),
	BANK_DRIVE("Z",     GPIOZ_0,     GPIOZ_15,    0x17,  0),
	BANK_DRIVE("Z1",    GPIOZ_16,    GPIOZ_19,    0xc0,  0),
	BANK_DRIVE("H",     GPIOH_0,     GPIOH_15,    0x1f,  0),
	BANK_DRIVE("H1",    GPIOH_16,    GPIOH_29,    0xc1,  0),
	BANK_DRIVE("B",     GPIOB_0,     GPIOB_13,    0x2f,  0),
	BANK_DRIVE("C",     GPIOC_0,     GPIOC_10,    0x37,  0),
	BANK_DRIVE("P",     GPIOP_0,     GPIOP_9,    0x3f,  0),
	BANK_DRIVE("W",     GPIOW_0,     GPIOW_16,    0x67,  0),
	BANK_DRIVE("M",     GPIOM_0,     GPIOM_15,    0x77,  0),
	BANK_DRIVE("M1",    GPIOM_16,    GPIOM_29,    0xc2,  0),
	BANK_DRIVE("TESTN", GPIO_TEST_N, GPIO_TEST_N, 0x87,  0),
};

static struct meson_axg_pmx_data meson_t3x_periphs_pmx_banks_data = {
	.pmx_banks	= meson_t3x_periphs_pmx_banks,
	.num_pmx_banks	= ARRAY_SIZE(meson_t3x_periphs_pmx_banks),
};

static struct meson_drive_data meson_t3x_periphs_drive_data = {
	.drive_banks	= meson_t3x_periphs_drive_banks,
	.num_drive_banks = ARRAY_SIZE(meson_t3x_periphs_drive_banks),
};

static struct meson_pinctrl_data meson_t3x_periphs_pinctrl_data = {
	.name		= "periphs-banks",
	.groups		= meson_t3x_periphs_groups,
	.funcs		= meson_t3x_periphs_functions,
	.banks		= meson_t3x_periphs_banks,
	.num_pins	= 151,
	.num_groups	= ARRAY_SIZE(meson_t3x_periphs_groups),
	.num_funcs	= ARRAY_SIZE(meson_t3x_periphs_functions),
	.num_banks	= ARRAY_SIZE(meson_t3x_periphs_banks),
	.pmx_data	= &meson_t3x_periphs_pmx_banks_data,
	.drv_data	= &meson_t3x_periphs_drive_data,
};

static const struct udevice_id meson_t3x_pinctrl_match[] = {
	{
		.compatible = "amlogic,meson-t3x-periphs-pinctrl",
		.data = (ulong)&meson_t3x_periphs_pinctrl_data,
	},
	{ }
};

U_BOOT_DRIVER(meson_t3x_pinctrl) = {
	.name	= "meson-t3x-pinctrl",
	.id	= UCLASS_PINCTRL,
	.of_match = of_match_ptr(meson_t3x_pinctrl_match),
	.probe = meson_pinctrl_probe,
	.priv_auto_alloc_size = sizeof(struct meson_pinctrl),
	.ops = &meson_axg_pinctrl_ops,
};
