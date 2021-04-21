/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _ARCH_MESON_A1_MEASURE_H_
#define _ARCH_MESON_A1_MEASURE_H_

static const char* clk_msr_table[] = {
	[0] = "tdmout_b_sclk",
	[1] = "tdmout_a_sclk",
	[2] = "tdmin_lb_sclk",
	[3] = "tdmin_b_sclk",
	[4] = "tdmin_a_sclk",
	[5] = "vad_clk",
	[6] = "resampleA_clk",
	[7] = "pdm_sysclk",
	[8] = "pdm_dclk",
	[9] = "locker_out_clk",
	[10] = "locker_in_clk",
	[11] = "spdifin_clk",
	[12] = "tdmin_vad_clk",
	[13] = "au_adc_clk",
	[14] = "au_dac_clk",
	[16] = "spicc_a_clk",
	[17] = "spifc_clk",
	[18] = "sd_emmc_a_clk",
	[19] = "dmcx4_clk",
	[20] = "dmc_clk",
	[21] = "psram_clk",
	[22] = "cecb_clk",
	[23] = "ceca_clk",
	[24] = "ts_clk",
	[25] = "pwm_f_clk",
	[26] = "pwm_e_clk",
	[27] = "pwm_d_clk",
	[28] = "pwm_c_clk",
	[29] = "pwm_b_clk",
	[30] = "pwm_a_clk",
	[31] = "saradc_clk",
	[32] = "usb_bus",
	[33] = "dsp_b_clk",
	[34] = "dsu_a_clk",
	[35] = "axi_clk",
	[36] = "sys_clk",
	[40] = "rng_ring_osc0",
	[41] = "rng_ring_osc1",
	[42] = "rng_ring_osc2",
	[43] = "rng_ring_osc3",
	[44] = "dds_out",
	[45] = "cpu_clk_div16",
	[46] = "gpio_msr",
	[50] = "osc_ring_cpu0",
	[51] = "osc_ring_cpu1",
	[54] = "osc_ring_top0",
	[55] = "osc_ring_top1",
	[56] = "osc_ring_ddr",
	[57] = "osc_ring_dmc",
	[58] = "osc_ring_dspa",
	[59] = "osc_ring_dspb",
	[60] = "osc_ring_rama",
	[61] = "osc_ring_ramb",
};

unsigned long clk_util_clk_msr(unsigned long clk_mux);
int clk_msr(int index);

#endif
