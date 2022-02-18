/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#define PWR_ON    1
#define PWR_OFF   0

enum pm_e {
	PM_CPU_PWR0,
	PM_CPU_CORE0,
	PM_CPU_CORE1,
	PM_DSP_A = 8,
	PM_DSP_B,
	PM_UART,
	PM_DMC,
	PM_I2C,
	PM_SDEMMC_B,
	PM_ACODEC,
	PM_AUDIO,
	PM_MKL_OTP,
	PM_DMA,
	PM_SDEMMC_A,
	PM_SRAM_A,
	PM_SRAM_B,
	PM_IR,
	PM_SPICC,
	PM_SPIFC,
	PM_USB,
	PM_NIC,
	PM_PDM,
	PM_RSA,
	PM_MIPI_ISP,
	PM_HCODEC,
	PM_WAVE,
	PM_SDEMMC_C,
	PM_SRAM_C,
	PM_GDC,
	PM_GE2D,
	PM_NNA,
	PM_ETH,
	PM_GIC,
	PM_DDR,
	PM_SPICC_B
};

unsigned long viu_init_psci_smc(void);
unsigned long pwr_ctrl_psci_smc(unsigned int power_domain, bool power_control);
