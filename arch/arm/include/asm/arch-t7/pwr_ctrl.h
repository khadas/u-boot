/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#define PWR_ON    1
#define PWR_OFF   0

#define PM_DSPA          0
#define PM_DSPB          1
#define PM_DOS_HCODEC    2
#define PM_DOS_HEVC      3
#define PM_DOS_VDEC      4
#define PM_DOS_WAVE      5
#define PM_VPU_HDMI      6
#define PM_USB_COMB      7
#define PM_PCIE          8
#define PM_GE2D          9
#define PM_SRAMA         10
#define PM_SRAMB         11
#define PM_HDMIRX        12
#define PM_VI_CLK1       13
#define PM_VI_CLK2       14
#define PM_ETH           15
#define PM_ISP           16
#define PM_MIPI_ISP      17
#define PM_GDC           18
#define PM_DEWARP        19
#define PM_SDIO_A        20
#define PM_SDIO_B        21
#define PM_EMMC          22
#define PM_MALI_SC0      23
#define PM_MALI_SC1      24
#define PM_MALI_SC2      25
#define PM_MALI_SC3      26
#define PM_MALI_TOP      27
#define PM_NNA_CORE0     28
#define PM_NNA_CORE1     29
#define PM_NNA_CORE2     30
#define PM_NNA_CORE3     31
#define PM_NNA_TOP       32
#define PM_DDR0          33
#define PM_DDR1          34
#define PM_DMC0          35
#define PM_DMC1          36
#define PM_NOC           37
#define PM_NIC2          38
#define PM_NIC3          39
#define PM_CCI           40
#define PM_MIPI_DSI0     41
#define PM_SPICC0        42
#define PM_SPICC1        43
#define PM_SPICC2        44
#define PM_SPICC3        45
#define PM_SPICC4        46
#define PM_SPICC5        47
#define PM_EDP0          48
#define PM_EDP1          49
#define PM_MIPI_DSI1     50
#define PM_AUDIO         51

#define PM_MAX		52

unsigned long viu_init_psci_smc(void);
unsigned long pwr_ctrl_psci_smc(unsigned int power_domain, bool power_control);
unsigned long pwr_ctrl_status_psci_smc(unsigned int power_domain);
