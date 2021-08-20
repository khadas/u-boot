
// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <dt-bindings/gpio/meson-p1-gpio.h>

#include "pinctrl-meson.h"
#include "pinctrl-meson-axg-pmx.h"

/* BANK D func1 */
static const unsigned int uart_a_tx_pins[]		= { GPIOD_0 };
static const unsigned int uart_a_rx_pins[]		= { GPIOD_1 };
static const unsigned int i2c9_sck_pins[]		= { GPIOD_2 };
static const unsigned int i2c9_sda_pins[]		= { GPIOD_3 };
static const unsigned int i2c10_sck_pins[]		= { GPIOD_4 };
static const unsigned int i2c10_sda_pins[]		= { GPIOD_5 };
static const unsigned int jtag_a_clk_pins[]		= { GPIOD_6 };
static const unsigned int jtag_a_tms_pins[]		= { GPIOD_7 };
static const unsigned int jtag_a_tdi_pins[]		= { GPIOD_8 };
static const unsigned int jtag_a_tdo_pins[]		= { GPIOD_9 };
static const unsigned int wd_rsto_pins[]		= { GPIOD_10 };

/* BANK D func2 */
static const unsigned int uart_mop1_tx_d_pins[]		= { GPIOD_0 };
static const unsigned int uart_mop1_rx_d_pins[]		= { GPIOD_1 };
static const unsigned int i2c9_slave_sck_pins[]		= { GPIOD_2 };
static const unsigned int i2c9_slave_sda_pins[]		= { GPIOD_3 };
static const unsigned int i2c10_slave_sck_pins[]	= { GPIOD_4 };
static const unsigned int i2c10_slave_sda_pins[]	= { GPIOD_5 };
static const unsigned int gen_clk_out_d_pins[]		= { GPIOD_6 };
static const unsigned int pwm_d_d_pins[]		= { GPIOD_7 };
static const unsigned int pcieck_reqn_d_pins[]		= { GPIOD_11 };

/* BANK D func3 */
static const unsigned int uart_mop2_tx_d_pins[]		= { GPIOD_0 };
static const unsigned int uart_mop2_rx_d_pins[]		= { GPIOD_1 };

/* BANK E func1 */
static const unsigned int pwm_a_e_pins[]		= { GPIOE_0 };
static const unsigned int pwm_b_pins[]			= { GPIOE_1 };
static const unsigned int pwm_c_pins[]			= { GPIOE_2 };
static const unsigned int pwm_d_e_pins[]		= { GPIOE_3 };
static const unsigned int pwm_e_pins[]			= { GPIOE_4 };
static const unsigned int rtc_clk_in_pins[]		= { GPIOE_5 };

/* BANK E func2 */
static const unsigned int i2c7_sck_e_pins[]		= { GPIOE_1 };
static const unsigned int i2c7_sda_e_pins[]		= { GPIOE_2 };
static const unsigned int i2c8_sck_e_pins[]		= { GPIOE_3 };
static const unsigned int i2c8_sda_e_pins[]		= { GPIOE_4 };

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
static const unsigned int emmc_cmd_pins[]		= { GPIOB_10 };
static const unsigned int emmc_nand_ds_pins[]		= { GPIOB_11 };

/* BANK B func2 */
static const unsigned int spif_hold_pins[]		= { GPIOB_3 };
static const unsigned int spif_mo_pins[]		= { GPIOB_4 };
static const unsigned int spif_mi_pins[]		= { GPIOB_5 };
static const unsigned int spif_clk_pins[]		= { GPIOB_6 };
static const unsigned int spif_wp_pins[]		= { GPIOB_7 };
static const unsigned int spif_cs_pins[]		= { GPIOB_12 };

/* BANK C func1 */
static const unsigned int i2c6_sda_c_pins[]		= { GPIOC_0 };
static const unsigned int i2c6_sck_c_pins[]		= { GPIOC_1 };
static const unsigned int i2c7_sda_c_pins[]		= { GPIOC_4 };
static const unsigned int i2c7_sck_c_pins[]		= { GPIOC_5 };
static const unsigned int i2c8_sda_c_pins[]		= { GPIOC_8 };
static const unsigned int i2c8_sck_c_pins[]		= { GPIOC_9 };

/* BANK C func2 */
static const unsigned int i2c6_slave_sda_pins[]		= { GPIOC_0 };
static const unsigned int i2c6_slave_sck_pins[]		= { GPIOC_1 };
static const unsigned int i2c7_slave_sda_pins[]		= { GPIOC_4 };
static const unsigned int i2c7_slave_sck_pins[]		= { GPIOC_5 };
static const unsigned int i2c8_slave_sda_pins[]		= { GPIOC_8 };
static const unsigned int i2c8_slave_sck_pins[]		= { GPIOC_9 };

/* BANK C func4 */
static const unsigned int rt_gpio0_c_pins[]		= { GPIOC_2 };
static const unsigned int rt_gpio1_c_pins[]		= { GPIOC_3 };
static const unsigned int rt_gpio2_c_pins[]		= { GPIOC_6 };
static const unsigned int rt_gpio3_c_pins[]		= { GPIOC_7 };
static const unsigned int rt_gpio4_c_pins[]		= { GPIOC_10 };
static const unsigned int rt_gpio5_c_pins[]		= { GPIOC_11 };
static const unsigned int rt_gpio6_c_pins[]		= { GPIOC_12 };
static const unsigned int rt_gpio7_c_pins[]		= { GPIOC_13 };
static const unsigned int rt_gpio8_c_pins[]		= { GPIOC_14 };
static const unsigned int rt_gpio9_c_pins[]		= { GPIOC_15 };
static const unsigned int rt_gpio10_c_pins[]		= { GPIOC_16 };
static const unsigned int rt_gpio11_c_pins[]		= { GPIOC_17 };

/* BANK T func1 */
static const unsigned int uart_c_rx_pins[]		= { GPIOT_0 };
static const unsigned int uart_c_tx_pins[]		= { GPIOT_1 };
static const unsigned int uart_c_cts_pins[]		= { GPIOT_2 };
static const unsigned int uart_c_rts_pins[]		= { GPIOT_3 };
static const unsigned int uart_d_rx_pins[]		= { GPIOT_4 };
static const unsigned int uart_d_tx_pins[]		= { GPIOT_5 };
static const unsigned int uart_d_cts_pins[]		= { GPIOT_6 };
static const unsigned int uart_d_rts_pins[]		= { GPIOT_7 };
static const unsigned int i2c4_sda_t8_pins[]		= { GPIOT_8 };
static const unsigned int i2c4_sck_t9_pins[]		= { GPIOT_9 };
static const unsigned int spi_f_clk_pins[]		= { GPIOT_10 };
static const unsigned int spi_f_mosi_pins[]		= { GPIOT_11 };
static const unsigned int spi_f_miso_pins[]		= { GPIOT_12 };
static const unsigned int spi_f_ss0_pins[]		= { GPIOT_13 };
static const unsigned int mclk_0_pins[]			= { GPIOT_14 };
static const unsigned int tdm_sclk0_pins[]		= { GPIOT_15 };
static const unsigned int tdm_fs0_pins[]		= { GPIOT_16 };
static const unsigned int tdm_d0_pins[]			= { GPIOT_17 };
static const unsigned int tdm_d1_pins[]			= { GPIOT_18 };
static const unsigned int mclk_1_pins[]			= { GPIOT_19 };
static const unsigned int tdm_sclk1_pins[]		= { GPIOT_20 };
static const unsigned int tdm_fs1_pins[]		= { GPIOT_21 };
static const unsigned int tdm_d2_pins[]			= { GPIOT_22 };
static const unsigned int tdm_d3_pins[]			= { GPIOT_23 };
static const unsigned int pdm_a_dclk_t24_pins[]		= { GPIOT_24 };
static const unsigned int pdm_a_din0_pins[]		= { GPIOT_25 };
static const unsigned int pdm_a_dclk_t26_pins[]		= { GPIOT_26 };
static const unsigned int pdm_a_din1_pins[]		= { GPIOT_27 };
static const unsigned int pdm_a_dclk_t28_pins[]		= { GPIOT_28 };
static const unsigned int pdm_a_din2_pins[]		= { GPIOT_29 };
static const unsigned int pdm_a_dclk_t30_pins[]		= { GPIOT_30 };
static const unsigned int pdm_a_din3_pins[]		= { GPIOT_31 };

/* BANK T func2 */
static const unsigned int uart_mop1_rx_t_pins[]		= { GPIOT_2 };
static const unsigned int uart_mop1_tx_t_pins[]		= { GPIOT_3 };
static const unsigned int i2c4_slave_sda_pins[]		= { GPIOT_8 };
static const unsigned int i2c4_slave_sck_pins[]		= { GPIOT_9 };
static const unsigned int i2c4_sda_t26_pins[]		= { GPIOT_26 };
static const unsigned int i2c4_sck_t28_pins[]		= { GPIOT_28 };
static const unsigned int pwm_a_t_pins[]		= { GPIOT_30 };

/* BANK T func3 */
static const unsigned int uart_mop2_rx_t_pins[]		= { GPIOT_2 };
static const unsigned int uart_mop2_tx_t_pins[]		= { GPIOT_3 };

/* BANK M func1 */
static const unsigned int sdio_d0_pins[]		= { GPIOM_0 };
static const unsigned int sdio_d1_pins[]		= { GPIOM_1 };
static const unsigned int sdio_d2_pins[]		= { GPIOM_2 };
static const unsigned int sdio_d3_pins[]		= { GPIOM_3 };
static const unsigned int sdio_clk_pins[]		= { GPIOM_4 };
static const unsigned int sdio_cmd_pins[]		= { GPIOM_5 };

/* BANK N func1 */
static const unsigned int eth_mdio_pins[]		= { GPION_0 };
static const unsigned int eth_mdc_pins[]		= { GPION_1 };
static const unsigned int eth_rgmii_rx_clk_pins[]	= { GPION_2 };
static const unsigned int eth_rx_dv_pins[]		= { GPION_3 };
static const unsigned int eth_rxd0_pins[]		= { GPION_4 };
static const unsigned int eth_rxd1_pins[]		= { GPION_5 };
static const unsigned int eth_rxd2_rgmii_pins[]		= { GPION_6 };
static const unsigned int eth_rxd3_rgmii_pins[]		= { GPION_7 };
static const unsigned int eth_rgmii_tx_clk_pins[]	= { GPION_8 };
static const unsigned int eth_txen_pins[]		= { GPION_9 };
static const unsigned int eth_txd0_pins[]		= { GPION_10 };
static const unsigned int eth_txd1_pins[]		= { GPION_11 };
static const unsigned int eth_txd2_rgmii_pins[]		= { GPION_12 };
static const unsigned int eth_txd3_rgmii_pins[]		= { GPION_13 };

/* BANK N func2 */
static const unsigned int clk25m_pins[]			= { GPION_8 };

/* BANK N func3 */
static const unsigned int gen_clk_out_n_pins[]		= { GPION_8 };

/* BANK H func1 */
static const unsigned int spi_a_clk_pins[]		= { GPIOH_6 };
static const unsigned int spi_a_mosi_pins[]		= { GPIOH_7 };
static const unsigned int spi_a_miso_pins[]		= { GPIOH_8 };
static const unsigned int spi_a_ss0_pins[]		= { GPIOH_9 };
static const unsigned int spi_b_clk_pins[]		= { GPIOH_10 };
static const unsigned int spi_b_mosi_pins[]		= { GPIOH_11 };
static const unsigned int spi_b_miso_pins[]		= { GPIOH_12 };
static const unsigned int spi_b_ss0_pins[]		= { GPIOH_13 };
static const unsigned int spi_c_clk_h_pins[]		= { GPIOH_14 };
static const unsigned int spi_c_mosi_h_pins[]		= { GPIOH_15 };
static const unsigned int spi_c_miso_h_pins[]		= { GPIOH_16 };
static const unsigned int spi_c_ss0_h_pins[]		= { GPIOH_17 };
static const unsigned int uart_b_rx_h_pins[]		= { GPIOH_18 };
static const unsigned int uart_b_tx_h_pins[]		= { GPIOH_19 };
static const unsigned int uart_b_cts_h_pins[]		= { GPIOH_20 };
static const unsigned int uart_b_rts_h_pins[]		= { GPIOH_21 };
static const unsigned int i2c3_sda_h_pins[]		= { GPIOH_22 };
static const unsigned int i2c3_sck_h_pins[]		= { GPIOH_23 };

/* BANK H func2 */
static const unsigned int spi_c_ss1_h_pins[]		= { GPIOH_20 };
static const unsigned int i2c3_slave_sda_pins[]		= { GPIOH_22 };
static const unsigned int i2c3_slave_sck_pins[]		= { GPIOH_23 };

/* BANK H func4 */
static const unsigned int rt_gpio12_h_pins[]		= { GPIOH_0 };
static const unsigned int rt_gpio13_h_pins[]		= { GPIOH_1 };
static const unsigned int rt_gpio14_h_pins[]		= { GPIOH_2 };
static const unsigned int rt_gpio15_h_pins[]		= { GPIOH_3 };
static const unsigned int rt_gpio0_h_pins[]		= { GPIOH_4 };
static const unsigned int rt_gpio1_h_pins[]		= { GPIOH_5 };

/* BANK K func1 */
static const unsigned int spi_d_clk_k_pins[]		= { GPIOK_0 };
static const unsigned int spi_d_mosi_k_pins[]		= { GPIOK_1 };
static const unsigned int spi_d_miso_k_pins[]		= { GPIOK_2 };
static const unsigned int spi_d_ss0_k_pins[]		= { GPIOK_3 };
static const unsigned int spi_e_clk_k_pins[]		= { GPIOK_4 };
static const unsigned int spi_e_mosi_k_pins[]		= { GPIOK_5 };
static const unsigned int spi_e_miso_k_pins[]		= { GPIOK_6 };
static const unsigned int spi_e_ss0_k_pins[]		= { GPIOK_7 };
static const unsigned int mclk_2_pins[]			= { GPIOK_8 };
static const unsigned int tdm_sclk2_pins[]		= { GPIOK_9 };
static const unsigned int tdm_fs2_pins[]		= { GPIOK_10 };
static const unsigned int tdm_d4_pins[]			= { GPIOK_11 };
static const unsigned int tdm_d5_pins[]			= { GPIOK_12 };
static const unsigned int pdm_b_dclk_k13_pins[]		= { GPIOK_13 };
static const unsigned int pdm_b_din0_pins[]		= { GPIOK_14 };
static const unsigned int pdm_b_dclk_k15_pins[]		= { GPIOK_15 };
static const unsigned int pdm_b_din1_pins[]		= { GPIOK_16 };
static const unsigned int i2c1_sda_k_pins[]		= { GPIOK_17 };
static const unsigned int i2c1_sck_k_pins[]		= { GPIOK_18 };
static const unsigned int i2c2_sda_k_pins[]		= { GPIOK_19 };
static const unsigned int i2c2_sck_k_pins[]		= { GPIOK_20 };
static const unsigned int i2c5_sda_pins[]		= { GPIOK_21 };
static const unsigned int i2c5_sck_pins[]		= { GPIOK_22 };
static const unsigned int uart_e_rx_k_pins[]		= { GPIOK_23 };
static const unsigned int uart_e_tx_k_pins[]		= { GPIOK_24 };
static const unsigned int uart_e_cts_k_pins[]		= { GPIOK_25 };
static const unsigned int uart_e_rts_k_pins[]		= { GPIOK_26 };
static const unsigned int uart_f_rx_k_pins[]		= { GPIOK_27 };
static const unsigned int uart_f_tx_k_pins[]		= { GPIOK_28 };
static const unsigned int uart_f_cts_k_pins[]		= { GPIOK_29 };
static const unsigned int uart_f_rts_k_pins[]		= { GPIOK_30 };

/* BANK K func2 */
static const unsigned int i2c1_slave_sda_pins[]		= { GPIOK_17 };
static const unsigned int i2c1_slave_sck_pins[]		= { GPIOK_18 };
static const unsigned int i2c2_slave_sda_pins[]		= { GPIOK_19 };
static const unsigned int i2c2_slave_sck_pins[]		= { GPIOK_20 };
static const unsigned int i2c5_slave_sda_pins[]		= { GPIOK_21 };
static const unsigned int i2c5_slave_sck_pins[]		= { GPIOK_22 };

/* BANK W func1 */
static const unsigned int mclk_3_w_pins[]		= { GPIOW_0 };
static const unsigned int tdm_sclk3_w_pins[]		= { GPIOW_1 };
static const unsigned int tdm_fs3_w_pins[]		= { GPIOW_2 };
static const unsigned int tdm_d6_w_pins[]		= { GPIOW_3 };
static const unsigned int tdm_d7_w_pins[]		= { GPIOW_4 };
static const unsigned int pwm_f_pins[]			= { GPIOW_5 };
static const unsigned int pwm_g_pins[]			= { GPIOW_6 };
static const unsigned int pwm_h_pins[]			= { GPIOW_7 };
static const unsigned int pwm_i_pins[]			= { GPIOW_8 };
static const unsigned int pwm_j_pins[]			= { GPIOW_9 };

/* BANK W func2 */
static const unsigned int i2c3_sda_w_pins[]		= { GPIOW_5 };
static const unsigned int i2c3_sck_w_pins[]		= { GPIOW_6 };
static const unsigned int uart_b_rx_w_pins[]		= { GPIOW_10 };
static const unsigned int uart_b_tx_w_pins[]		= { GPIOW_11 };
static const unsigned int uart_b_cts_w_pins[]		= { GPIOW_12 };
static const unsigned int uart_b_rts_w_pins[]		= { GPIOW_13 };
static const unsigned int i2c6_sda_w_pins[]		= { GPIOW_14 };
static const unsigned int i2c6_sck_w_pins[]		= { GPIOW_15 };

/* BANK X func1 */
static const unsigned int i2c1_sda_x_pins[]		= { GPIOX_5 };
static const unsigned int i2c1_sck_x_pins[]		= { GPIOX_6 };

/* BANK X func2 */
static const unsigned int i2c2_sda_x_pins[]		= { GPIOX_12 };
static const unsigned int i2c2_sck_x_pins[]		= { GPIOX_13 };
static const unsigned int spi_d_ss1_pins[]		= { GPIOX_15 };
static const unsigned int spi_d_ss2_pins[]		= { GPIOX_16 };
static const unsigned int spi_d_ss3_pins[]		= { GPIOX_17 };
static const unsigned int spi_c_ss2_pins[]		= { GPIOX_18 };

/* BANK X func3 */
static const unsigned int spi_d_clk_x_pins[]		= { GPIOX_0 };
static const unsigned int spi_d_mosi_x_pins[]		= { GPIOX_1 };
static const unsigned int spi_d_miso_x_pins[]		= { GPIOX_2 };
static const unsigned int spi_d_ss0_x_pins[]		= { GPIOX_3 };
static const unsigned int spi_c_ss3_pins[]		= { GPIOX_6 };
static const unsigned int spi_e_clk_x_pins[]		= { GPIOX_15 };
static const unsigned int spi_e_mosi_x_pins[]		= { GPIOX_16 };
static const unsigned int spi_e_miso_x_pins[]		= { GPIOX_17 };
static const unsigned int spi_e_ss0_x_pins[]		= { GPIOX_18 };
static const unsigned int spi_c_clk_x_pins[]		= { GPIOX_26 };
static const unsigned int spi_c_mosi_x_pins[]		= { GPIOX_27 };
static const unsigned int spi_c_miso_x_pins[]		= { GPIOX_28 };
static const unsigned int spi_c_ss0_x_pins[]		= { GPIOX_29 };
static const unsigned int spi_c_ss1_x_pins[]		= { GPIOX_30 };

/* BANK X func4 */
static const unsigned int rt_gpio2_x7_pins[]		= { GPIOX_7 };
static const unsigned int rt_gpio3_x8_pins[]		= { GPIOX_8 };
static const unsigned int rt_gpio4_x9_pins[]		= { GPIOX_9  };
static const unsigned int rt_gpio5_x_pins[]		= { GPIOX_10 };
static const unsigned int rt_gpio6_x_pins[]		= { GPIOX_11 };
static const unsigned int rt_gpio7_x_pins[]		= { GPIOX_12 };
static const unsigned int rt_gpio8_x_pins[]		= { GPIOX_13 };
static const unsigned int rt_gpio9_x_pins[]		= { GPIOX_14 };
static const unsigned int rt_gpio10_x_pins[]		= { GPIOX_15 };
static const unsigned int rt_gpio11_x_pins[]		= { GPIOX_16 };
static const unsigned int rt_gpio12_x_pins[]		= { GPIOX_17 };
static const unsigned int rt_gpio13_x_pins[]		= { GPIOX_18 };
static const unsigned int rt_gpio14_x_pins[]		= { GPIOX_19 };
static const unsigned int rt_gpio15_x_pins[]		= { GPIOX_20 };
static const unsigned int rt_gpio0_x_pins[]		= { GPIOX_21 };
static const unsigned int rt_gpio1_x_pins[]		= { GPIOX_22 };
static const unsigned int rt_gpio2_x23_pins[]		= { GPIOX_23 };
static const unsigned int rt_gpio3_x24_pins[]		= { GPIOX_24 };
static const unsigned int rt_gpio4_x25_pins[]		= { GPIOX_25 };

/* BANK Z func1 */
static const unsigned int i2c0_sda_pins[]		= { GPIOZ_0 };
static const unsigned int i2c0_sck_pins[]		= { GPIOZ_1 };

/* BANK Z func2 */
static const unsigned int i2c0_slave_sda_pins[]		= { GPIOZ_0 };
static const unsigned int i2c0_slave_sck_pins[]		= { GPIOZ_1 };
static const unsigned int uart_e_tx_z_pins[]		= { GPIOZ_2 };
static const unsigned int uart_e_rx_z_pins[]		= { GPIOZ_3 };
static const unsigned int uart_e_cts_z_pins[]		= { GPIOZ_4 };
static const unsigned int uart_e_rts_z_pins[]		= { GPIOZ_5 };

/* BANK Z func3 */
static const unsigned int uart_mop1_rx_z_pins[]		= { GPIOZ_0 };
static const unsigned int uart_mop1_tx_z_pins[]		= { GPIOZ_1 };
static const unsigned int pcieck_reqn_z_pins[]		= { GPIOZ_4 };

/* BANK Z func4 */
static const unsigned int uart_mop2_rx_z_pins[]		= { GPIOZ_0 };
static const unsigned int uart_mop2_tx_z_pins[]		= { GPIOZ_1 };

/* BANK Y func2 */
static const unsigned int uart_mop1_rx_y_pins[]		= { GPIOY_0 };
static const unsigned int uart_mop1_tx_y_pins[]		= { GPIOY_1 };

/* BANK Y func3 */
static const unsigned int uart_mop2_rx_y_pins[]		= { GPIOY_0 };
static const unsigned int uart_mop2_tx_y_pins[]		= { GPIOY_1 };
static const unsigned int tdm_d6_y_pins[]		= { GPIOY_2 };
static const unsigned int tdm_d7_y_pins[]		= { GPIOY_3 };
static const unsigned int tdm_fs3_y_pins[]		= { GPIOY_4 };
static const unsigned int tdm_sclk3_y_pins[]		= { GPIOY_5 };
static const unsigned int mclk_3_y_pins[]		= { GPIOY_6 };
static const unsigned int uart_f_tx_y_pins[]		= { GPIOY_10 };
static const unsigned int uart_f_rx_y_pins[]		= { GPIOY_11 };
static const unsigned int uart_f_cts_y_pins[]		= { GPIOY_12 };
static const unsigned int uart_f_rts_y_pins[]		= { GPIOY_13 };

static struct meson_pmx_group meson_p1_periphs_groups[] = {
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
	GPIO_GROUP(GPIOE_2),
	GPIO_GROUP(GPIOE_3),
	GPIO_GROUP(GPIOE_4),
	GPIO_GROUP(GPIOE_5),

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
	GPIO_GROUP(GPIOC_11),
	GPIO_GROUP(GPIOC_12),
	GPIO_GROUP(GPIOC_13),
	GPIO_GROUP(GPIOC_14),
	GPIO_GROUP(GPIOC_15),
	GPIO_GROUP(GPIOC_16),
	GPIO_GROUP(GPIOC_17),

	GPIO_GROUP(GPIOT_0),
	GPIO_GROUP(GPIOT_1),
	GPIO_GROUP(GPIOT_2),
	GPIO_GROUP(GPIOT_3),
	GPIO_GROUP(GPIOT_4),
	GPIO_GROUP(GPIOT_5),
	GPIO_GROUP(GPIOT_6),
	GPIO_GROUP(GPIOT_7),
	GPIO_GROUP(GPIOT_8),
	GPIO_GROUP(GPIOT_9),
	GPIO_GROUP(GPIOT_10),
	GPIO_GROUP(GPIOT_11),
	GPIO_GROUP(GPIOT_12),
	GPIO_GROUP(GPIOT_13),
	GPIO_GROUP(GPIOT_14),
	GPIO_GROUP(GPIOT_15),
	GPIO_GROUP(GPIOT_16),
	GPIO_GROUP(GPIOT_17),
	GPIO_GROUP(GPIOT_18),
	GPIO_GROUP(GPIOT_19),
	GPIO_GROUP(GPIOT_20),
	GPIO_GROUP(GPIOT_21),
	GPIO_GROUP(GPIOT_22),
	GPIO_GROUP(GPIOT_23),
	GPIO_GROUP(GPIOT_24),
	GPIO_GROUP(GPIOT_25),
	GPIO_GROUP(GPIOT_26),
	GPIO_GROUP(GPIOT_27),
	GPIO_GROUP(GPIOT_28),
	GPIO_GROUP(GPIOT_29),
	GPIO_GROUP(GPIOT_30),
	GPIO_GROUP(GPIOT_31),

	GPIO_GROUP(GPIOM_0),
	GPIO_GROUP(GPIOM_1),
	GPIO_GROUP(GPIOM_2),
	GPIO_GROUP(GPIOM_3),
	GPIO_GROUP(GPIOM_4),
	GPIO_GROUP(GPIOM_5),

	GPIO_GROUP(GPION_0),
	GPIO_GROUP(GPION_1),
	GPIO_GROUP(GPION_2),
	GPIO_GROUP(GPION_3),
	GPIO_GROUP(GPION_4),
	GPIO_GROUP(GPION_5),
	GPIO_GROUP(GPION_6),
	GPIO_GROUP(GPION_7),
	GPIO_GROUP(GPION_8),
	GPIO_GROUP(GPION_9),
	GPIO_GROUP(GPION_10),
	GPIO_GROUP(GPION_11),
	GPIO_GROUP(GPION_12),
	GPIO_GROUP(GPION_13),

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

	GPIO_GROUP(GPIOK_0),
	GPIO_GROUP(GPIOK_1),
	GPIO_GROUP(GPIOK_2),
	GPIO_GROUP(GPIOK_3),
	GPIO_GROUP(GPIOK_4),
	GPIO_GROUP(GPIOK_5),
	GPIO_GROUP(GPIOK_6),
	GPIO_GROUP(GPIOK_7),
	GPIO_GROUP(GPIOK_8),
	GPIO_GROUP(GPIOK_9),
	GPIO_GROUP(GPIOK_10),
	GPIO_GROUP(GPIOK_11),
	GPIO_GROUP(GPIOK_12),
	GPIO_GROUP(GPIOK_13),
	GPIO_GROUP(GPIOK_14),
	GPIO_GROUP(GPIOK_15),
	GPIO_GROUP(GPIOK_16),
	GPIO_GROUP(GPIOK_17),
	GPIO_GROUP(GPIOK_18),
	GPIO_GROUP(GPIOK_19),
	GPIO_GROUP(GPIOK_20),
	GPIO_GROUP(GPIOK_21),
	GPIO_GROUP(GPIOK_22),
	GPIO_GROUP(GPIOK_23),
	GPIO_GROUP(GPIOK_24),
	GPIO_GROUP(GPIOK_25),
	GPIO_GROUP(GPIOK_26),
	GPIO_GROUP(GPIOK_27),
	GPIO_GROUP(GPIOK_28),
	GPIO_GROUP(GPIOK_29),
	GPIO_GROUP(GPIOK_30),

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
	GPIO_GROUP(GPIOW_17),

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
	GPIO_GROUP(GPIOX_20),
	GPIO_GROUP(GPIOX_21),
	GPIO_GROUP(GPIOX_22),
	GPIO_GROUP(GPIOX_23),
	GPIO_GROUP(GPIOX_24),
	GPIO_GROUP(GPIOX_25),
	GPIO_GROUP(GPIOX_26),
	GPIO_GROUP(GPIOX_27),
	GPIO_GROUP(GPIOX_28),
	GPIO_GROUP(GPIOX_29),
	GPIO_GROUP(GPIOX_30),
	GPIO_GROUP(GPIOX_31),

	GPIO_GROUP(GPIOZ_0),
	GPIO_GROUP(GPIOZ_1),
	GPIO_GROUP(GPIOZ_2),
	GPIO_GROUP(GPIOZ_3),
	GPIO_GROUP(GPIOZ_4),
	GPIO_GROUP(GPIOZ_5),
	GPIO_GROUP(GPIOZ_6),
	GPIO_GROUP(GPIOZ_7),

	GPIO_GROUP(GPIOY_0),
	GPIO_GROUP(GPIOY_1),
	GPIO_GROUP(GPIOY_2),
	GPIO_GROUP(GPIOY_3),
	GPIO_GROUP(GPIOY_4),
	GPIO_GROUP(GPIOY_5),
	GPIO_GROUP(GPIOY_6),
	GPIO_GROUP(GPIOY_7),
	GPIO_GROUP(GPIOY_8),
	GPIO_GROUP(GPIOY_9),
	GPIO_GROUP(GPIOY_10),
	GPIO_GROUP(GPIOY_11),
	GPIO_GROUP(GPIOY_12),
	GPIO_GROUP(GPIOY_13),
	GPIO_GROUP(GPIOY_14),

	GPIO_GROUP(GPIO_TEST_N),

	/* BANK D func1 */
	GROUP(uart_a_tx,		1),
	GROUP(uart_a_rx,		1),
	GROUP(i2c9_sck,			1),
	GROUP(i2c9_sda,			1),
	GROUP(i2c10_sck,		1),
	GROUP(i2c10_sda,		1),
	GROUP(jtag_a_clk,		1),
	GROUP(jtag_a_tms,		1),
	GROUP(jtag_a_tdi,		1),
	GROUP(jtag_a_tdo,		1),
	GROUP(wd_rsto,			1),

	/* BANK D func2 */
	GROUP(uart_mop1_tx_d,		2),
	GROUP(uart_mop1_rx_d,		2),
	GROUP(i2c9_slave_sck,		2),
	GROUP(i2c9_slave_sda,		2),
	GROUP(i2c10_slave_sck,		2),
	GROUP(i2c10_slave_sda,		2),
	GROUP(gen_clk_out_d,		2),
	GROUP(pwm_d_d,			2),
	GROUP(pcieck_reqn_d,		2),

	/* BANK D func3 */
	GROUP(uart_mop2_tx_d,		3),
	GROUP(uart_mop2_rx_d,		3),

	/* BANK E func1 */
	GROUP(pwm_a_e,			1),
	GROUP(pwm_b,			1),
	GROUP(pwm_c,			1),
	GROUP(pwm_d_e,			1),
	GROUP(pwm_e,			1),
	GROUP(rtc_clk_in,		1),

	/* BANK E func2 */
	GROUP(i2c7_sck_e,		2),
	GROUP(i2c7_sda_e,		2),
	GROUP(i2c8_sck_e,		2),
	GROUP(i2c8_sda_e,		2),

	/* BANK C func1 */
	GROUP(i2c6_sck_c,		1),
	GROUP(i2c6_sda_c,		1),
	GROUP(i2c7_sck_c,		1),
	GROUP(i2c7_sda_c,		1),
	GROUP(i2c8_sck_c,		1),
	GROUP(i2c8_sda_c,		1),

	/* BANK C func2 */
	GROUP(i2c6_slave_sck,		2),
	GROUP(i2c6_slave_sda,		2),
	GROUP(i2c7_slave_sck,		2),
	GROUP(i2c7_slave_sda,		2),
	GROUP(i2c8_slave_sck,		2),
	GROUP(i2c8_slave_sda,		2),

	/* BANK C func4 */
	GROUP(rt_gpio0_c,		4),
	GROUP(rt_gpio1_c,		4),
	GROUP(rt_gpio2_c,		4),
	GROUP(rt_gpio3_c,		4),
	GROUP(rt_gpio4_c,		4),
	GROUP(rt_gpio5_c,		4),
	GROUP(rt_gpio6_c,		4),
	GROUP(rt_gpio7_c,		4),
	GROUP(rt_gpio8_c,		4),
	GROUP(rt_gpio9_c,		4),
	GROUP(rt_gpio10_c,		4),
	GROUP(rt_gpio11_c,		4),

	/* BANK T func1 */
	GROUP(uart_c_rx,		1),
	GROUP(uart_c_tx,		1),
	GROUP(uart_c_cts,		1),
	GROUP(uart_c_rts,		1),
	GROUP(uart_d_rx,		1),
	GROUP(uart_d_tx,		1),
	GROUP(uart_d_cts,		1),
	GROUP(uart_d_rts,		1),
	GROUP(i2c4_sda_t8,		1),
	GROUP(i2c4_sck_t9,		1),
	GROUP(spi_f_clk,		1),
	GROUP(spi_f_mosi,		1),
	GROUP(spi_f_miso,		1),
	GROUP(spi_f_ss0,		1),
	GROUP(mclk_0,			1),
	GROUP(tdm_sclk0,		1),
	GROUP(tdm_fs0,			1),
	GROUP(tdm_d0,			1),
	GROUP(tdm_d1,			1),
	GROUP(mclk_1,			1),
	GROUP(tdm_sclk1,		1),
	GROUP(tdm_fs1,			1),
	GROUP(tdm_d2,			1),
	GROUP(tdm_d3,			1),
	GROUP(pdm_a_dclk_t24,		1),
	GROUP(pdm_a_din0,		1),
	GROUP(pdm_a_dclk_t26,		1),
	GROUP(pdm_a_din1,		1),
	GROUP(pdm_a_dclk_t28,		1),
	GROUP(pdm_a_din2,		1),
	GROUP(pdm_a_dclk_t30,		1),
	GROUP(pdm_a_din3,		1),

	/* BANK T func2 */
	GROUP(uart_mop1_tx_t,		2),
	GROUP(uart_mop1_rx_t,		2),
	GROUP(i2c4_slave_sck,		2),
	GROUP(i2c4_slave_sda,		2),
	GROUP(i2c4_sda_t26,		2),
	GROUP(i2c4_sck_t28,		2),
	GROUP(pwm_a_t,			2),

	/* BANK T func3 */
	GROUP(uart_mop2_rx_t,		3),
	GROUP(uart_mop2_tx_t,		3),

	/* BANK M func1 */
	GROUP(sdio_d0,			1),
	GROUP(sdio_d1,			1),
	GROUP(sdio_d2,			1),
	GROUP(sdio_d3,			1),
	GROUP(sdio_clk,			1),
	GROUP(sdio_cmd,			1),

	/* BANK N func1 */
	GROUP(eth_mdio,			1),
	GROUP(eth_mdc,			1),
	GROUP(eth_rgmii_rx_clk,		1),
	GROUP(eth_rx_dv,		1),
	GROUP(eth_rxd0,			1),
	GROUP(eth_rxd1,			1),
	GROUP(eth_rxd2_rgmii,		1),
	GROUP(eth_rxd3_rgmii,		1),
	GROUP(eth_rgmii_tx_clk,		1),
	GROUP(eth_txen,			1),
	GROUP(eth_txd0,			1),
	GROUP(eth_txd1,			1),
	GROUP(eth_txd2_rgmii,		1),
	GROUP(eth_txd3_rgmii,		1),

	/* BANK N func2 */
	GROUP(clk25m,			2),

	/* BANK N func3 */
	GROUP(gen_clk_out_n,		3),

	/* BANK H func1 */
	GROUP(spi_a_clk,		1),
	GROUP(spi_a_mosi,		1),
	GROUP(spi_a_miso,		1),
	GROUP(spi_a_ss0,		1),
	GROUP(spi_b_clk,		1),
	GROUP(spi_b_mosi,		1),
	GROUP(spi_b_miso,		1),
	GROUP(spi_b_ss0,		1),
	GROUP(spi_c_clk_h,		1),
	GROUP(spi_c_mosi_h,		1),
	GROUP(spi_c_miso_h,		1),
	GROUP(spi_c_ss0_h,		1),
	GROUP(uart_b_rx_h,		1),
	GROUP(uart_b_tx_h,		1),
	GROUP(uart_b_cts_h,		1),
	GROUP(uart_b_rts_h,		1),
	GROUP(i2c3_sck_h,		1),
	GROUP(i2c3_sda_h,		1),

	/* BANK H func2 */
	GROUP(spi_c_ss1_h,		2),
	GROUP(i2c3_slave_sck,		2),
	GROUP(i2c3_slave_sda,		2),

	/* BANK H func4 */
	GROUP(rt_gpio12_h,		4),
	GROUP(rt_gpio13_h,		4),
	GROUP(rt_gpio14_h,		4),
	GROUP(rt_gpio15_h,		4),
	GROUP(rt_gpio0_h,		4),
	GROUP(rt_gpio1_h,		4),

	/* BANK K func1 */
	GROUP(spi_d_clk_k,		1),
	GROUP(spi_d_mosi_k,		1),
	GROUP(spi_d_miso_k,		1),
	GROUP(spi_d_ss0_k,		1),
	GROUP(spi_e_clk_k,		1),
	GROUP(spi_e_mosi_k,		1),
	GROUP(spi_e_miso_k,		1),
	GROUP(spi_e_ss0_k,		1),
	GROUP(mclk_2,			1),
	GROUP(tdm_sclk2,		1),
	GROUP(tdm_fs2,			1),
	GROUP(tdm_d4,			1),
	GROUP(tdm_d5,			1),
	GROUP(pdm_b_dclk_k13,		1),
	GROUP(pdm_b_din0,		1),
	GROUP(pdm_b_dclk_k15,		1),
	GROUP(pdm_b_din1,		1),
	GROUP(i2c1_sck_k,		1),
	GROUP(i2c1_sda_k,		1),
	GROUP(i2c2_sck_k,		1),
	GROUP(i2c2_sda_k,		1),
	GROUP(i2c5_sck,			1),
	GROUP(i2c5_sda,			1),
	GROUP(uart_e_rx_k,		1),
	GROUP(uart_e_tx_k,		1),
	GROUP(uart_e_cts_k,		1),
	GROUP(uart_e_rts_k,		1),
	GROUP(uart_f_rx_k,		1),
	GROUP(uart_f_tx_k,		1),
	GROUP(uart_f_cts_k,		1),
	GROUP(uart_f_rts_k,		1),

	/* BANK K func2 */
	GROUP(i2c1_slave_sck,		2),
	GROUP(i2c1_slave_sda,		2),
	GROUP(i2c2_slave_sck,		2),
	GROUP(i2c2_slave_sda,		2),
	GROUP(i2c5_slave_sck,		2),
	GROUP(i2c5_slave_sda,		2),

	/* BANK W func1 */
	GROUP(mclk_3_w,			1),
	GROUP(tdm_sclk3_w,		1),
	GROUP(tdm_fs3_w,		1),
	GROUP(tdm_d6_w,			1),
	GROUP(tdm_d7_w,			1),
	GROUP(pwm_f,			1),
	GROUP(pwm_g,			1),
	GROUP(pwm_h,			1),
	GROUP(pwm_i,			1),
	GROUP(pwm_j,			1),

	/* BANK W func2 */
	GROUP(i2c3_sck_w,		2),
	GROUP(i2c3_sda_w,		2),
	GROUP(uart_b_rx_w,		2),
	GROUP(uart_b_tx_w,		2),
	GROUP(uart_b_cts_w,		2),
	GROUP(uart_b_rts_w,		2),
	GROUP(i2c6_sck_w,		2),
	GROUP(i2c6_sda_w,		2),

	/* BANK X func1 */
	GROUP(i2c1_sck_x,		1),
	GROUP(i2c1_sda_x,		1),

	/* BANK X func2 */
	GROUP(i2c2_sck_x,		2),
	GROUP(i2c2_sda_x,		2),
	GROUP(spi_d_ss1,		2),
	GROUP(spi_d_ss2,		2),
	GROUP(spi_d_ss3,		2),
	GROUP(spi_c_ss2,		2),

	/* BANK X func3 */
	GROUP(spi_d_clk_x,		3),
	GROUP(spi_d_mosi_x,		3),
	GROUP(spi_d_miso_x,		3),
	GROUP(spi_d_ss0_x,		3),
	GROUP(spi_c_ss3,		3),
	GROUP(spi_e_clk_x,		3),
	GROUP(spi_e_mosi_x,		3),
	GROUP(spi_e_miso_x,		3),
	GROUP(spi_e_ss0_x,		3),
	GROUP(spi_c_clk_x,		3),
	GROUP(spi_c_mosi_x,		3),
	GROUP(spi_c_miso_x,		3),
	GROUP(spi_c_ss0_x,		3),
	GROUP(spi_c_ss1_x,		3),

	/* BANK X func4 */
	GROUP(rt_gpio2_x7,		4),
	GROUP(rt_gpio3_x8,		4),
	GROUP(rt_gpio4_x9,		4),
	GROUP(rt_gpio5_x,		4),
	GROUP(rt_gpio6_x,		4),
	GROUP(rt_gpio7_x,		4),
	GROUP(rt_gpio8_x,		4),
	GROUP(rt_gpio9_x,		4),
	GROUP(rt_gpio10_x,		4),
	GROUP(rt_gpio11_x,		4),
	GROUP(rt_gpio12_x,		4),
	GROUP(rt_gpio13_x,		4),
	GROUP(rt_gpio14_x,		4),
	GROUP(rt_gpio15_x,		4),
	GROUP(rt_gpio0_x,		4),
	GROUP(rt_gpio1_x,		4),
	GROUP(rt_gpio2_x23,		4),
	GROUP(rt_gpio3_x24,		4),
	GROUP(rt_gpio4_x25,		4),

	/* BANK Z func1 */
	GROUP(i2c0_sck,			1),
	GROUP(i2c0_sda,			1),

	/* BANK Z func2 */
	GROUP(i2c0_slave_sck,		2),
	GROUP(i2c0_slave_sda,		2),
	GROUP(uart_e_rx_z,		2),
	GROUP(uart_e_tx_z,		2),
	GROUP(uart_e_cts_z,		2),
	GROUP(uart_e_rts_z,		2),

	/* BANK Z func3 */
	GROUP(uart_mop1_rx_z,		3),
	GROUP(uart_mop1_tx_z,		3),
	GROUP(pcieck_reqn_z,		3),

	/* BANK Z func4 */
	GROUP(uart_mop2_rx_z,		4),
	GROUP(uart_mop2_tx_z,		4),

	/* BANK Y func2 */
	GROUP(uart_mop1_rx_y,		2),
	GROUP(uart_mop1_tx_y,		2),

	/* BANK Y func3 */
	GROUP(uart_mop2_rx_y,		3),
	GROUP(uart_mop2_tx_y,		3),
	GROUP(tdm_d6_y,			3),
	GROUP(tdm_d7_y,			3),
	GROUP(tdm_fs3_y,		3),
	GROUP(tdm_sclk3_y,		3),
	GROUP(mclk_3_y,			3),
	GROUP(uart_f_tx_y,		3),
	GROUP(uart_f_rx_y,		3),
	GROUP(uart_f_cts_y,		3),
	GROUP(uart_f_rts_y,		3)
};

static struct meson_pmx_group meson_p1_storage_groups[] = {
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
	GROUP(emmc_cmd,			1),
	GROUP(emmc_nand_ds,		1),

	/* BANK B func2 */
	GROUP(spif_hold,		2),
	GROUP(spif_mo,			2),
	GROUP(spif_mi,			2),
	GROUP(spif_clk,			2),
	GROUP(spif_wp,			2),
	GROUP(spif_cs,			2)
};

static const char * const gpio_storage_groups[] = {
	"GPIOB_0", "GPIOB_1", "GPIOB_2", "GPIOB_3", "GPIOB_4", "GPIOB_5",
	"GPIOB_6", "GPIOB_7", "GPIOB_8", "GPIOB_9", "GPIOB_10", "GPIOB_11",
	"GPIOB_12",
};

static const char * const gpio_periphs_groups[] = {
	"GPIOD_0", "GPIOD_1", "GPIOD_2", "GPIOD_3", "GPIOD_4", "GPIOD_5",
	"GPIOD_6", "GPIOD_7", "GPIOD_8", "GPIOD_9", "GPIOD_10", "GPIOD_11",
	"GPIOD_12",

	"GPIOE_0", "GPIOE_1", "GPIOE_2", "GPIOE_3", "GPIOE_4", "GPIOE_5",

	"GPIOC_0", "GPIOC_1", "GPIOC_2", "GPIOC_3", "GPIOC_4", "GPIOC_5",
	"GPIOC_6", "GPIOC_7", "GPIOC_8", "GPIOC_9", "GPIOC_10", "GPIOC_11",
	"GPIOC_12", "GPIOC_13", "GPIOC_14", "GPIOC_15", "GPIOC_16",
	"GPIOC_17",

	"GPIOT_0", "GPIOT_1", "GPIOT_2", "GPIOT_3", "GPIOT_4", "GPIOT_5",
	"GPIOT_6", "GPIOT_7", "GPIOT_8", "GPIOT_9", "GPIOT_10", "GPIOT_11",
	"GPIOT_12", "GPIOT_13", "GPIOT_14", "GPIOT_15", "GPIOT_16", "GPIOT_17",
	"GPIOT_18", "GPIOT_19", "GPIOT_20", "GPIOT_21", "GPIOT_22", "GPIOT_23",
	"GPIOT_24", "GPIOT_25", "GPIOT_26", "GPIOT_27", "GPIOT_28", "GPIOT_29",
	"GPIOT_30", "GPIOT_31",

	"GPIOM_0", "GPIOM_1", "GPIOM_2", "GPIOM_3", "GPIOM_4", "GPIOM_5",

	"GPION_0", "GPION_1", "GPION_2", "GPION_3", "GPION_4", "GPION_5",
	"GPION_6", "GPION_7", "GPION_8", "GPION_9", "GPION_10", "GPION_11",
	"GPION_12", "GPION_13",

	"GPIOH_0", "GPIOH_1", "GPIOH_2", "GPIOH_3", "GPIOH_4", "GPIOH_5",
	"GPIOH_6", "GPIOH_7", "GPIOH_8", "GPIOH_9", "GPIOH_10", "GPIOH_11",
	"GPIOH_12", "GPIOH_13", "GPIOH_14", "GPIOH_15", "GPIOH_16", "GPIOH_17",
	"GPIOH_18", "GPIOH_19", "GPIOH_20", "GPIOH_21", "GPIOH_22", "GPIOH_23",

	"GPIOK_0", "GPIOK_1", "GPIOK_2", "GPIOK_3", "GPIOK_4", "GPIOK_5",
	"GPIOK_6", "GPIOK_7", "GPIOK_8", "GPIOK_9", "GPIOK_10", "GPIOK_11",
	"GPIOK_12", "GPIOK_13", "GPIOK_14", "GPIOK_15", "GPIOK_16", "GPIOK_17",
	"GPIOK_18", "GPIOK_19", "GPIOK_20", "GPIOK_21", "GPIOK_22", "GPIOK_23",
	"GPIOK_24", "GPIOK_25", "GPIOK_26", "GPIOK_27", "GPIOK_28", "GPIOK_29",
	"GPIOK_30",

	"GPIOW_0", "GPIOW_1", "GPIOW_2", "GPIOW_3", "GPIOW_4", "GPIOW_5",
	"GPIOW_6", "GPIOW_7", "GPIOW_8", "GPIOW_9", "GPIOW_10", "GPIOW_11",
	"GPIOW_12", "GPIOW_13", "GPIOW_14", "GPIOW_15", "GPIOW_16", "GPIOW_17",

	"GPIOX_0", "GPIOX_1", "GPIOX_2", "GPIOX_3", "GPIOX_4", "GPIOX_5",
	"GPIOX_6", "GPIOX_7", "GPIOX_8", "GPIOX_9", "GPIOX_10", "GPIOX_11",
	"GPIOX_12", "GPIOX_13", "GPIOX_14", "GPIOX_15", "GPIOX_16", "GPIOX_17",
	"GPIOX_18", "GPIOX_19", "GPIOX_20", "GPIOX_21", "GPIOX_22", "GPIOX_23",
	"GPIOX_24", "GPIOX_25", "GPIOX_26", "GPIOX_27", "GPIOX_28", "GPIOX_29",
	"GPIOX_30", "GPIOX_31",

	"GPIOZ_0", "GPIOZ_1", "GPIOZ_2", "GPIOZ_3", "GPIOZ_4", "GPIOZ_5",
	"GPIOZ_6", "GPIOZ_7",

	"GPIOY_0", "GPIOY_1", "GPIOY_2", "GPIOY_3", "GPIOY_4", "GPIOY_5",
	"GPIOY_6", "GPIOY_7", "GPIOY_8", "GPIOY_9", "GPIOY_10", "GPIOY_11",
	"GPIOY_12", "GPIOY_13", "GPIOY_14",

	"GPIO_TEST_N"
};

static const char * const emmc_groups[] = {
	"emmc_nand_d0", "emmc_nand_d1", "emmc_nand_d2", "emmc_nand_d3",
	"emmc_nand_d4", "emmc_nand_d5", "emmc_nand_d6", "emmc_nand_d7",
	"emmc_clk", "emmc_cmd", "emmc_nand_ds"
};

static const char * const spif_groups[] = {
	"spif_hold", "spif_mo", "spif_mi", "spif_clk", "spif_wp", "spif_cs",
};

static const char * const sdio_groups[] = {
	"sdio_d0", "sdio_d1", "sdio_d2", "sdio_d3", "sdio_clk", "sdio_cmd"
};

static const char * const gen_clk_groups[] = {
	"gen_clk_out_d", "gen_clk_out_n"
};

static const char * const jtag_a_groups[] = {
	"jtag_a_clk", "jtag_a_tms", "jtag_a_tdi", "jtag_a_tdo"
};

static const char * const uart_a_groups[] = {
	"uart_a_tx", "uart_a_rx"
};

static const char * const uart_b_groups[] = {
	"uart_b_rx_h", "uart_b_tx_h", "uart_b_cts_h", "uart_b_rts_h",
	"uart_b_rx_w", "uart_b_tx_w", "uart_b_cts_w", "uart_b_rts_w"
};

static const char * const uart_c_groups[] = {
	"uart_c_tx", "uart_c_rx", "uart_c_cts", "uart_c_rts"
};

static const char * const uart_d_groups[] = {
	"uart_d_tx", "uart_d_rx", "uart_d_cts", "uart_d_rts"
};

static const char * const uart_e_groups[] = {
	"uart_e_tx_k", "uart_e_rx_k", "uart_e_cts_k", "uart_e_rts_k",
	"uart_e_tx_z", "uart_e_rx_z", "uart_e_cts_z", "uart_e_rts_z"
};

static const char * const uart_f_groups[] = {
	"uart_f_tx_k", "uart_f_rx_k", "uart_f_cts_k", "uart_f_rts_k",
	"uart_f_tx_y", "uart_f_rx_y", "uart_f_cts_y", "uart_f_rts_y"
};

static const char * const uart_mop1_groups[] = {
	"uart_mop1_tx_t", "uart_mop1_rx_t",
	"uart_mop1_tx_d", "uart_mop1_rx_d",
	"uart_mop1_tx_z", "uart_mop1_rx_z",
	"uart_mop1_tx_y", "uart_mop1_rx_y"
};

static const char * const uart_mop2_groups[] = {
	"uart_mop2_tx_t", "uart_mop2_rx_t",
	"uart_mop2_tx_d", "uart_mop2_rx_d",
	"uart_mop2_tx_z", "uart_mop2_rx_z",
	"uart_mop2_tx_y", "uart_mop2_rx_y"
};

static const char * const spi_a_groups[] = {
	"spi_a_clk", "spi_a_mosi", "spi_a_miso", "spi_a_ss0"
};

static const char * const spi_b_groups[] = {
	"spi_b_clk", "spi_b_mosi", "spi_b_miso", "spi_b_ss0"
};

static const char * const spi_c_groups[] = {
	"spi_c_clk_h", "spi_c_mosi_h", "spi_c_miso_h", "spi_c_ss0_h",
	"spi_c_ss1_h",

	"spi_c_clk_x", "spi_c_mosi_x", "spi_c_miso_x", "spi_c_ss0_x",
	"spi_c_ss1_x",

	"spi_c_ss2", "spi_c_ss3"
};

static const char * const spi_d_groups[] = {
	"spi_d_clk_x", "spi_d_mosi_x", "spi_d_miso_x", "spi_d_ss0_x",
	"spi_d_clk_x", "spi_d_mosi_x", "spi_d_miso_x", "spi_d_ss0_x",
	"spi_d_ss1", "spi_d_ss2", "spi_d_ss3"
};

static const char * const spi_e_groups[] = {
	"spi_e_clk_k", "spi_e_mosi_k", "spi_e_miso_k", "spi_e_ss0_k",
	"spi_e_clk_x", "spi_e_mosi_x", "spi_e_miso_x", "spi_e_ss0_x"
};

static const char * const spi_f_groups[] = {
	"spi_f_clk", "spi_f_mosi", "spi_f_miso", "spi_f_ss0"
};

static const char * const pwm_a_groups[] = {
	"pwm_a_e", "pwm_a_t"
};

static const char * const pwm_b_groups[] = {
	"pwm_b"
};

static const char * const pwm_c_groups[] = {
	"pwm_c",
};

static const char * const pwm_d_groups[] = {
	"pwm_d_d", "pwm_d_e"
};

static const char * const pwm_e_groups[] = {
	"pwm_e",
};

static const char * const pwm_f_groups[] = {
	"pwm_f",
};

static const char * const pwm_g_groups[] = {
	"pwm_g",
};

static const char * const pwm_h_groups[] = {
	"pwm_h",
};

static const char * const pwm_i_groups[] = {
	"pwm_i",
};

static const char * const pwm_j_groups[] = {
	"pwm_j",
};

static const char * const tdm_groups[] = {
	"tdm_d0", "tdm_d1", "tdm_d2", "tdm_d3", "tdm_d4", "tdm_d5",
	"tdm_fs0", "tdm_fs1", "tdm_fs2", "tdm_sclk0", "tdm_sclk1",
	"tdm_sclk2", "tdm_fs3_w", "tdm_d6_w", "tdm_d7_w", "tdm_d6_y",
	"tdm_d7_y", "tdm_fs3_y", "tdm_sclk3_w", "tdm_sclk3_y"
};

static const char * const i2c0_slave_groups[] = {
	"i2c0_slave_sck", "i2c0_slave_sda"
};

static const char * const i2c1_slave_groups[] = {
	"i2c10_slave_sck", "i2c10_slave_sda"
};

static const char * const i2c2_slave_groups[] = {
	"i2c2_slave_sck", "i2c2_slave_sda"
};

static const char * const i2c3_slave_groups[] = {
	"i2c3_slave_sck", "i2c3_slave_sda"
};

static const char * const i2c4_slave_groups[] = {
	"i2c4_slave_sck", "i2c4_slave_sda"
};

static const char * const i2c5_slave_groups[] = {
	"i2c5_slave_sck", "i2c5_slave_sda"
};

static const char * const i2c6_slave_groups[] = {
	"i2c6_slave_sck", "i2c6_slave_sda"
};

static const char * const i2c7_slave_groups[] = {
	"i2c7_slave_sck", "i2c7_slave_sda"
};

static const char * const i2c8_slave_groups[] = {
	"i2c8_slave_sck", "i2c8_slave_sda"
};

static const char * const i2c9_slave_groups[] = {
	"i2c9_slave_sck", "i2c9_slave_sda"
};

static const char * const i2c10_slave_groups[] = {
	"i2c10_slave_sck", "i2c10_slave_sda"
};

static const char * const i2c0_groups[] = {
	"i2c0_sck", "i2c0_sda"
};

static const char * const i2c1_groups[] = {
	"i2c1_sck_x", "i2c1_sda_x", "i2c1_sck_k", "i2c1_sda_k"
};

static const char * const i2c2_groups[] = {
	"i2c2_sck_k", "i2c2_sda_k", "i2c2_sck_x", "i2c2_sda_x"
};

static const char * const i2c3_groups[] = {
	"i2c3_sck_h", "i2c3_sda_h", "i2c3_sck_w", "i2c3_sda_w",
};

static const char * const i2c4_groups[] = {
	"i2c4_sck_t8", "i2c4_sda_t9", "i2c4_sck_t26", "i2c4_sda_t28"
};

static const char * const i2c5_groups[] = {
	"i2c5_sck", "i2c5_sda"
};

static const char * const i2c6_groups[] = {
	"i2c6_sck_c", "i2c6_sda_c", "i2c6_sck_w", "i2c6_sda_w"
};

static const char * const i2c7_groups[] = {
	"i2c7_sck_e", "i2c7_sda_e", "i2c7_sck_c", "i2c7_sda_c"
};

static const char * const i2c8_groups[] = {
	"i2c8_sck_e", "i2c8_sda_e", "i2c8_sck_c", "i2c8_sda_c"
};

static const char * const i2c9_groups[] = {
	"i2c9_sck", "i2c9_sda"
};

static const char * const i2c10_groups[] = {
	"i2c10_sck", "i2c10_sda"
};

static const char * const clk25m_groups[] = {
	"clk25m"
};

static const char * const wd_rsto_groups[] = {
	"wd_rsto"
};

static const char * const rtc_clk_groups[] = {
	"rtc_clk_in"
};

static const char * const eth_groups[] = {
	"eth_mdio", "eth_mdc", "eth_rgmii_rx_clk", "eth_rx_dv", "eth_rxd0",
	"eth_rxd1", "eth_rxd2_rgmii", "eth_rxd3_rgmii", "eth_rgmii_tx_clk",
	"eth_txen", "eth_txd0", "eth_txd1", "eth_txd2_rgmii",
	"eth_txd3_rgmii"
};

static const char * const mclk_groups[] = {
	"mclk_0", "mclk_1", "mclk_2", "mclk_3_w", "mclk_3_y"
};

static const char * const pdm_groups[] = {
	"pdm_a_din0", "pdm_a_din1", "pdm_a_din2", "pdm_a_din3",
	"pdm_a_dclk_t24", "pdm_a_dclk_t26", "pdm_a_dclk_t28", "pdm_a_dclk_t30",
	"pdm_b_din0", "pdm_b_din1", "pdm_b_dclk_k13", "pdm_b_dclk_k15"
};

static const char * const pcieck_groups[] = {
	"pcieck_reqn_d", "pcieck_reqn_z"
};

static const char * const rt_gpio_groups[] = {
	"rt_gpio0_c", "rt_gpio0_h", "rt_gpio0_x", "rt_gpio1_c",
	"rt_gpio1_h", "rt_gpio1_x", "rt_gpio2_c", "rt_gpio2_x7",
	"rt_gpio2_x23", "rt_gpio3_c", "rt_gpio3_x8", "rt_gpio3_x24",
	"rt_gpio4_c", "rt_gpio4_x25" "rt_gpio4_x9", "rt_gpio5_c",
	"rt_gpio5_x", "rt_gpio6_c", "rt_gpio6_x", "rt_gpio7_c",
	"rt_gpio7_x", "rt_gpio8_c", "rt_gpio8_x", "rt_gpio9_c",
	"rt_gpio9_x", "rt_gpio10_c", "rt_gpio10_x", "rt_gpio11_c",
	"rt_gpio11_x", "rt_gpio12_h", "rt_gpio12_x", "rt_gpio13_h",
	"rt_gpio13_x", "rt_gpio14_h", "rt_gpio14_x", "rt_gpio15_h",
	"rt_gpio15_x"
};

static struct meson_pmx_func meson_p1_storage_functions[] = {
	FUNCTION(gpio_storage),
	FUNCTION(emmc),
	FUNCTION(spif)
};

static struct meson_pmx_func meson_p1_periphs_functions[] = {
	FUNCTION(gpio_periphs),
	FUNCTION(sdio),
	FUNCTION(gen_clk),
	FUNCTION(jtag_a),
	FUNCTION(uart_a),
	FUNCTION(uart_b),
	FUNCTION(uart_c),
	FUNCTION(uart_d),
	FUNCTION(uart_e),
	FUNCTION(uart_f),
	FUNCTION(uart_mop1),
	FUNCTION(uart_mop2),
	FUNCTION(spi_a),
	FUNCTION(spi_b),
	FUNCTION(spi_c),
	FUNCTION(spi_b),
	FUNCTION(spi_d),
	FUNCTION(spi_e),
	FUNCTION(spi_f),
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
	FUNCTION(tdm),
	FUNCTION(i2c0_slave),
	FUNCTION(i2c1_slave),
	FUNCTION(i2c2_slave),
	FUNCTION(i2c3_slave),
	FUNCTION(i2c4_slave),
	FUNCTION(i2c5_slave),
	FUNCTION(i2c6_slave),
	FUNCTION(i2c7_slave),
	FUNCTION(i2c8_slave),
	FUNCTION(i2c9_slave),
	FUNCTION(i2c10_slave),
	FUNCTION(i2c0),
	FUNCTION(i2c1),
	FUNCTION(i2c2),
	FUNCTION(i2c3),
	FUNCTION(i2c4),
	FUNCTION(i2c5),
	FUNCTION(i2c6),
	FUNCTION(i2c7),
	FUNCTION(i2c8),
	FUNCTION(i2c9),
	FUNCTION(i2c10),
	FUNCTION(clk25m),
	FUNCTION(wd_rsto),
	FUNCTION(rtc_clk),
	FUNCTION(eth),
	FUNCTION(mclk),
	FUNCTION(pdm),
	FUNCTION(pcieck),
	FUNCTION(rt_gpio)
};

static struct meson_bank meson_p1_storage_banks[] = {
	BANK("GPIOB_", GPIOB_0,    GPIOB_12,
	     0x03,  0,  0x04,  0,  0x02, 0,  0x01, 0,  0x00, 0)
};

static struct meson_pmx_bank meson_p1_storage_pmx_banks[] = {
	BANK_PMX("B",      GPIOB_0,     GPIOB_12,    0x0,   0)
};

static struct meson_drive_bank meson_p1_storage_drive_banks[] = {
	BANK_DRIVE("B",      GPIOB_0,     GPIOB_12,    0x07,   0)
};

static struct meson_axg_pmx_data meson_p1_storage_pmx_banks_data = {
	.pmx_banks	= meson_p1_storage_pmx_banks,
	.num_pmx_banks	= ARRAY_SIZE(meson_p1_storage_pmx_banks),
};

static struct meson_drive_data meson_p1_storage_drive_data = {
	.drive_banks	= meson_p1_storage_drive_banks,
	.num_drive_banks = ARRAY_SIZE(meson_p1_storage_drive_banks),
};

static struct meson_pinctrl_data meson_p1_storage_pinctrl_data = {
	.name		= "storage",
	.groups		= meson_p1_storage_groups,
	.funcs		= meson_p1_storage_functions,
	.banks		= meson_p1_storage_banks,
	.num_pins	= 13,
	.num_groups	= ARRAY_SIZE(meson_p1_storage_groups),
	.num_funcs	= ARRAY_SIZE(meson_p1_storage_functions),
	.num_banks	= ARRAY_SIZE(meson_p1_storage_banks),
	.pmx_data	= &meson_p1_storage_pmx_banks_data,
	.drv_data	= &meson_p1_storage_drive_data,
};

static struct meson_bank meson_p1_periphs_banks[] = {
	/* name  first  last  irq pullen  pull  dir  out  in*/
	BANK("TEST_N", GPIO_TEST_N,    GPIO_TEST_N,
	     0x03,  0,  0x04,  0,  0x02, 0,  0x01,  0, 0x00, 0),
	BANK("GPIOD_", GPIOD_0,    GPIOD_12,
	     0x13,  0,  0x14,  0,  0x12,  0, 0x11, 0,  0x10, 0),
	BANK("GPIOE_", GPIOE_0,    GPIOE_5,
	     0x23,  0,  0x24,  0,  0x22,  0, 0x21, 0,  0x20, 0),
	BANK("GPIOZ_", GPIOZ_0,    GPIOZ_7,
	     0x33,  0,  0x34,  0,  0x32, 0,  0x31, 0,  0x30, 0),
	BANK("GPIOH_", GPIOH_0,    GPIOH_23,
	     0x43,  0,  0x44,  0,  0x42, 0,  0x41, 0,  0x40, 0),
	BANK("GPIOC_", GPIOC_0,    GPIOC_17,
	     0x53,  0,  0x54,  0,  0x52, 0,  0x51, 0,  0x50, 0),
	BANK("GPIOB_", GPIOB_0,    GPIOB_12,
	     0x63,  0,  0x64,  0,  0x62, 0,  0x61, 0,  0x60, 0),
	BANK("GPIOX_", GPIOX_0,    GPIOX_31,
	     0x73,  0,  0x74,  0,  0x72, 0,  0x71, 0,  0x70, 0),
	BANK("GPIOT_", GPIOT_0,    GPIOT_31,
	     0x83,  0,  0x84,  0,  0x82, 0,  0x81, 0,  0x80, 0),
	BANK("GPIOK_", GPIOK_0,    GPIOK_30,
	     0x93,  0,  0x94,  0,  0x92, 0,  0x91, 0,  0x90, 0),
	BANK("GPIOW_", GPIOW_0,    GPIOW_17,
	     0xa3,  0,  0xa4,  0,  0xa2, 0,  0xa1, 0,  0xa0, 0),
	BANK("GPIOM_", GPIOM_0,    GPIOM_5,
	     0xb3,  0,  0xb4,  0,  0xb2, 0,  0xb1, 0,  0xb0, 0),
	BANK("GPIOY_", GPIOY_0,    GPIOY_14,
	     0xc3,  0,  0xc4,  0,  0xc2, 0,  0xc1, 0,  0xc0, 0),
	BANK("GPION_", GPION_0,    GPION_13,
	     0xd3,  0,  0xd4,  0,  0xd2, 0,  0xd1, 0,  0xd0, 0)

};

static struct meson_pmx_bank meson_p1_periphs_pmx_banks[] = {
	/*name	            first	 lask        reg offset*/
	BANK_PMX("D",      GPIOD_0,     GPIOD_12,    0x0,   0),
	BANK_PMX("E",      GPIOE_0,     GPIOE_5,     0x2,   0),
	BANK_PMX("C",      GPIOC_0,     GPIOC_17,    0x3,   0),
	BANK_PMX("T",      GPIOT_0,     GPIOT_31,    0x6,   0),
	BANK_PMX("M",      GPIOM_0,     GPIOM_5,     0xa,   0),
	BANK_PMX("N",      GPION_0,     GPION_13,    0xb,   0),
	BANK_PMX("H",      GPIOH_0,     GPIOH_23,    0xd,   0),
	BANK_PMX("K",      GPIOK_0,     GPIOK_30,    0x10,  0),
	BANK_PMX("W",      GPIOW_0,     GPIOW_17,    0x14,  0),
	BANK_PMX("X",      GPIOX_0,     GPIOX_31,    0x17,  0),
	BANK_PMX("Z",      GPIOZ_0,     GPIOZ_7,     0x1d,  0),
	BANK_PMX("Y",      GPIOY_0,     GPIOY_14,    0x1b,  0),
	BANK_PMX("TEST_N", GPIO_TEST_N, GPIO_TEST_N, 0x7,   0)
};

static struct meson_drive_bank meson_p1_periphs_drive_banks[] = {
	/*name	            first	 lask        reg offset*/
	BANK_DRIVE("TEST_N", GPIO_TEST_N, GPIO_TEST_N, 0x7,    0),
	BANK_DRIVE("D",      GPIOD_0,     GPIOD_12,    0x17,   0),
	BANK_DRIVE("E",      GPIOE_0,     GPIOE_5,     0x27,   0),
	BANK_DRIVE("Z",      GPIOZ_0,     GPIOZ_7,     0x37,   0),
	BANK_DRIVE("H",      GPIOH_0,     GPIOH_23,    0x47,   0),
	BANK_DRIVE("C",      GPIOC_0,     GPIOC_17,    0x57,   0),
	BANK_DRIVE("X",      GPIOX_0,     GPIOX_31,    0x77,   0),
	BANK_DRIVE("T",      GPIOT_0,     GPIOT_31,    0x87,   0),
	BANK_DRIVE("K",      GPIOK_0,     GPIOK_30,    0x97,   0),
	BANK_DRIVE("W",      GPIOW_0,     GPIOW_17,    0xa7,   0),
	BANK_DRIVE("M",      GPIOM_0,     GPIOM_5,     0xb7,   0),
	BANK_DRIVE("Y",      GPIOY_0,     GPIOY_14,    0xc7,   0),
	BANK_DRIVE("N",      GPION_0,     GPION_13,    0xd7,   0)
};

static struct meson_axg_pmx_data meson_p1_periphs_pmx_banks_data = {
	.pmx_banks	= meson_p1_periphs_pmx_banks,
	.num_pmx_banks	= ARRAY_SIZE(meson_p1_periphs_pmx_banks),
};

static struct meson_drive_data meson_p1_periphs_drive_data = {
	.drive_banks	= meson_p1_periphs_drive_banks,
	.num_drive_banks = ARRAY_SIZE(meson_p1_periphs_drive_banks),
};

static struct meson_pinctrl_data meson_p1_periphs_pinctrl_data = {
	.name		= "periphs-banks",
	.groups		= meson_p1_periphs_groups,
	.funcs		= meson_p1_periphs_functions,
	.banks		= meson_p1_periphs_banks,
	.num_pins	= 218,
	.num_groups	= ARRAY_SIZE(meson_p1_periphs_groups),
	.num_funcs	= ARRAY_SIZE(meson_p1_periphs_functions),
	.num_banks	= ARRAY_SIZE(meson_p1_periphs_banks),
	.pmx_data	= &meson_p1_periphs_pmx_banks_data,
	.drv_data	= &meson_p1_periphs_drive_data,
};

static const struct udevice_id meson_p1_pinctrl_match[] = {
	{
		.compatible = "amlogic,meson-p1-periphs-pinctrl",
		.data = (ulong)&meson_p1_periphs_pinctrl_data,
	},
	{
		.compatible = "amlogic,meson-p1-storage-pinctrl",
		.data = (ulong)&meson_p1_storage_pinctrl_data,
	},
	{ }
};

U_BOOT_DRIVER(meson_p1_pinctrl) = {
	.name	= "meson-p1-pinctrl",
	.id	= UCLASS_PINCTRL,
	.of_match = of_match_ptr(meson_p1_pinctrl_match),
	.probe = meson_pinctrl_probe,
	.priv_auto_alloc_size = sizeof(struct meson_pinctrl),
	.ops = &meson_axg_pinctrl_ops,
};
