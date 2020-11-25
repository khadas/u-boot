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
	PM_MMC,
	PM_I2C,
	PM_PSRAM,
	PM_ACODEC,
	PM_AUDIO,
	PM_MKL_OTP,
	PM_DMA,
	PM_SDEMMC,
	PM_SRAM_A,
	PM_SRAM_B,
	PM_IR,
	PM_SPICC,
	PM_SPIFC,
	PM_USB,
	PM_NIC,
	PM_PDM,
	PM_RSA
};

unsigned long pwr_ctrl_psci_smc(unsigned int power_domain, bool power_control);