/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#define PWR_ON    1
#define PWR_OFF   0

typedef enum pdid {
    PM_DSPA,
    PM_DSPB,
    PM_M4A,
    PM_M4B,
    PM_ISP_A,
    PM_ISP_B,
    PM_ISP_C,
    PM_ISP_D,
    PM_MIPI_ISP_TOP,
    PM_USB_COMB,
    PM_PCIE,
    PM_ETH,
    PM_SDIO,
    PM_NAND_EMMC,
    PM_NNA_A,
    PM_NNA_B,
    PM_NNA_C,
    PM_NNA_D,
    PM_NNA_E,
    PM_NNA_F,
    PM_NNA_TOP,
    PM_GE2D,
    PM_DEWA,
    PM_DEWB,
    PM_DEWC,
    PM_FDLE,
    PM_DMC0,
    PM_DMC1,
    PM_NOC_DMC_TOP,
    PM_SMMU,
    PM_DDR0,
    PM_DDR1,
} PM_E;
#define PM_MAX PM_DDR1 + 1

#if 0
char *domain_name[] =
{
    "PM_DSPA",
    "PM_DSPB",
    "PM_M4A",
    "PM_M4B",
    "PM_ISP_A",
    "PM_ISP_B",
    "PM_ISP_C",
    "PM_ISP_D",
    "PM_MIPI_ISP_TOP",
    "PM_USB_COMB",
    "PM_PCIE",
    "PM_ETH",
    "PM_SDIO",
    "PM_NAND_EMMC",
    "PM_NNA_A",
    "PM_NNA_B",
    "PM_NNA_C",
    "PM_NNA_D",
    "PM_NNA_E",
    "PM_NNA_F",
    "PM_NNA_TOP",
    "PM_GE2D",
    "PM_DEWA",
    "PM_DEWB",
    "PM_DEWC",
    "PM_FDLE",
    "PM_DMC0",
    "PM_DMC1",
    "PM_NOC_DMC_TOP",
    "PM_SMMU",
    "PM_DDR0",
    "PM_DDR1",
};
#endif

unsigned long viu_init_psci_smc(void);
unsigned long pwr_ctrl_psci_smc(unsigned int power_domain, bool power_control);
unsigned long pwr_ctrl_status_psci_smc(unsigned int power_domain);
