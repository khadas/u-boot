/*
 * (C) Copyright 2025 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _PMIC_RK801_H_
#define _PMIC_RK801_H_

#define DEV_OFF		BIT(0)

#define RK8XX_ID_MSK	0xfff0

enum rk801_reg {
	RK801_ID_DCDC1,
	RK801_ID_DCDC2,
	RK801_ID_DCDC4,
	RK801_ID_DCDC3,
	RK801_ID_LDO1,
	RK801_ID_LDO2,
	RK801_ID_SWITCH,
	RK801_ID_MAX,
};

#define RK801_SLP_REG_OFFSET                     5
#define RK801_NUM_REGULATORS                     7

/* RK801 Register Definitions */
#define RK801_ID_MSB                             0x00
#define RK801_ID_LSB                             0x01
#define RK801_OTP_VER_REG                        0x02
#define RK801_POWER_EN0_REG                      0x03
#define RK801_POWER_EN1_REG                      0x04
#define RK801_POWER_SLP_EN_REG                   0x05
#define RK801_POWER_FPWM_EN_REG                  0x06
#define RK801_SLP_LP_CONFIG_REG                  0x07
#define RK801_BUCK_CONFIG_REG                    0x08
#define RK801_BUCK1_ON_VSEL_REG                  0x09
#define RK801_BUCK2_ON_VSEL_REG                  0x0a
#define RK801_BUCK4_ON_VSEL_REG                  0x0b
#define RK801_LDO1_ON_VSEL_REG                   0x0c
#define RK801_LDO2_ON_VSEL_REG                   0x0d
#define RK801_BUCK1_SLP_VSEL_REG                 0x0e
#define RK801_BUCK2_SLP_VSEL_REG                 0x0f
#define RK801_BUCK4_SLP_VSEL_REG                 0x10
#define RK801_LDO1_SLP_VSEL_REG                  0x11
#define RK801_LDO2_SLP_VSEL_REG                  0x12
#define RK801_LDO_SW_IMAX_REG                    0x13
#define RK801_SYS_STS_REG                        0x14
#define RK801_SYS_CFG0_REG                       0x15
#define RK801_SYS_CFG1_REG                       0x16
#define RK801_SYS_CFG2_REG                       0x17
#define RK801_SYS_CFG3_REG                       0x18
#define RK801_SYS_CFG4_REG                       0x19
#define RK801_SLEEP_CFG_REG                      0x1a
#define RK801_ON_SOURCE_REG                      0x1b
#define RK801_OFF_SOURCE_REG                     0x1c
#define RK801_PWRON_KEY_REG                      0x1d
#define RK801_INT_STS0_REG                       0x1e
#define RK801_INT_MASK0_REG                      0x1f
#define RK801_INT_CONFIG_REG                     0x20
#define RK801_CON_BACK1_REG                      0x21
#define RK801_CON_BACK2_REG                      0x22
#define RK801_DATA_CON0_REG                      0x23
#define RK801_DATA_CON1_REG                      0x24
#define RK801_DATA_CON2_REG                      0x25
#define RK801_DATA_CON3_REG                      0x26
#define RK801_POWER_EXIT_SLP_SEQ0_REG            0x27
#define RK801_POWER_EXIT_SLP_SEQ1_REG            0x28
#define RK801_POWER_EXIT_SLP_SEQ2_REG            0x29
#define RK801_POWER_EXIT_SLP_SEQ3_REG            0x2a
#define RK801_POWER_ENTER_SLP_OR_SHTD_SEQ0_REG   0x2b
#define RK801_POWER_ENTER_SLP_OR_SHTD_SEQ1_REG   0x2c
#define RK801_POWER_ENTER_SLP_OR_SHTD_SEQ2_REG   0x2d
#define RK801_POWER_ENTER_SLP_OR_SHTD_SEQ3_REG   0x2e
#define RK801_BUCK_DEBUG1_REG                    0x2f
#define RK801_BUCK_DEBUG2_REG                    0x30
#define RK801_BUCK_DEBUG3_REG                    0x31
#define RK801_BUCK_DEBUG4_REG                    0x32
#define RK801_BUCK_DEBUG5_REG                    0x33
#define RK801_BUCK_DEBUG7_REG                    0x34
#define RK801_OTP_EN_CON_REG                     0x35
#define RK801_TEST_CON_REG                       0x36
#define RK801_EFUSE_CONTROL_REG                  0x37
#define RK801_SYS_CFG3_OTP_REG                   0x38

/* RK801 IRQ Definitions */
#define RK801_IRQ_PWRON_FALL                     0
#define RK801_IRQ_PWRON_RISE                     1
#define RK801_IRQ_PWRON                          2
#define RK801_IRQ_PWRON_LP                       3
#define RK801_IRQ_HOTDIE                         4
#define RK801_IRQ_VDC_RISE                       5
#define RK801_IRQ_VDC_FALL                       6
#define RK801_IRQ_PWRON_FALL_MSK                 BIT(0)
#define RK801_IRQ_PWRON_RISE_MSK                 BIT(1)
#define RK801_IRQ_PWRON_MSK                      BIT(2)
#define RK801_IRQ_PWRON_LP_MSK                   BIT(3)
#define RK801_IRQ_HOTDIE_MSK                     BIT(4)
#define RK801_IRQ_VDC_RISE_MSK                   BIT(5)
#define RK801_IRQ_VDC_FALL_MSK                   BIT(6)

/* RK801_SLP_LP_CONFIG_REG */
#define RK801_BUCK_SLP_LP_EN                     BIT(3)
#define RK801_PLDO_SLP_LP_EN                     BIT(1)
#define RK801_SLP_LP_MASK                        (RK801_PLDO_SLP_LP_EN | RK801_BUCK_SLP_LP_EN)

/* RK801_SLEEP_CFG_REG */
#define RK801_SLEEP_FUN_MSK                      0x3
#define RK801_NONE_FUN                           0x0
#define RK801_SLEEP_FUN                          0x1
#define RK801_SHUTDOWN_FUN                       0x2
#define RK801_RESET_FUN                          0x3

/* RK801_SYS_CFG2_REG */
#define RK801_SLEEP_POL_MSK                      BIT(1)
#define RK801_SLEEP_ACT_H                        BIT(1)
#define RK801_SLEEP_ACT_L                        0

#define RK801_RST_MSK                            (0x3 << 4)
#define RK801_RST_RESTART_PMU                    (0x0 << 4)
#define RK801_RST_RESTART_REG                    (0x1 << 4)
#define RK801_RST_RESTART_REG_RESETB             (0x2 << 4)

/* RK801_INT_CONFIG_REG */
#define RK801_INT_POL_MSK                        BIT(1)
#define RK801_INT_ACT_H                          BIT(1)
#define RK801_INT_ACT_L                          0

#define RK801_FPWM_MODE                          1
#define RK801_AUTO_PWM_MODE                      0
#define RK801_PLDO_HRDEC_EN                      BIT(6)

struct reg_data {
	u8 reg;
	u8 val;
	u8 mask;
};

struct rk801_priv {
	struct virq_chip *irq_chip;
	struct gpio_desc pwrctrl_gpio;
	bool req_pwrctrl_dvs;
	int variant;
	int irq;
};
#endif
