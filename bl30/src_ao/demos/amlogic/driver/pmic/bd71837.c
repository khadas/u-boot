/*
 * Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
 *
 * All information contained herein is Amlogic confidential.
 *
 * This software is provided to you pursuant to Software License Agreement
 * (SLA) with Amlogic Inc ("Amlogic"). This software may be used
 * only in accordance with the terms of this agreement.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification is strictly prohibited without prior written permission from
 * Amlogic.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "bd71837.h"
#include "meson_i2c.h"
#include "uart.h"
#include "myprintf.h"
#include "gpio.h"

static const struct regulator_linear_range bd718xx_dvs_buck_volts[] = {
	REGULATOR_LINEAR_RANGE(700000, 0x00, 0x3C, 10000),
	REGULATOR_LINEAR_RANGE(1300000, 0x3D, 0x3F, 0),
};

static const struct regulator_linear_range bd71837_buck5_volts[] = {
	/* Ranges when VOLT_SEL bit is 0 */
	REGULATOR_LINEAR_RANGE(700000, 0x00, 0x03, 100000),
	REGULATOR_LINEAR_RANGE(1050000, 0x04, 0x05, 50000),
	REGULATOR_LINEAR_RANGE(1200000, 0x06, 0x07, 150000),
};

static const struct regulator_linear_range bd71837_buck6_volts[] = {
	REGULATOR_LINEAR_RANGE(3000000, 0x00, 0x03, 100000),
};

static const unsigned int bd71837_buck7_volts[] = {
	1605000, 1695000, 1755000, 1800000, 1845000, 1905000, 1950000, 1995000
};

static const struct regulator_linear_range bd71837_buck8_volts[] = {
	REGULATOR_LINEAR_RANGE(680000, 0x00, 0x3C, 11500),
	REGULATOR_LINEAR_RANGE(1370000, 0x3C, 0x3F, 0),
 };

static const struct regulator_linear_range bd718xx_ldo1_volts[] = {
	REGULATOR_LINEAR_RANGE(1600000, 0x00, 0x03, 100000),
};

static const unsigned int ldo_2_volts[] = {
	900000, 800000
};

static const struct regulator_linear_range bd718xx_ldo3_volts[] = {
	REGULATOR_LINEAR_RANGE(1800000, 0x00, 0x0F, 100000),
};

static const struct regulator_linear_range bd718xx_ldo4_volts[] = {
	REGULATOR_LINEAR_RANGE(900000, 0x00, 0x09, 100000),
	REGULATOR_LINEAR_RANGE(1800000, 0xa, 0xf, 0),
};

static const struct regulator_linear_range bd71837_ldo5_volts[] = {
	REGULATOR_LINEAR_RANGE(1800000, 0x00, 0x0F, 100000),
};

static const struct regulator_linear_range bd718xx_ldo6_volts[] = {
	REGULATOR_LINEAR_RANGE(900000, 0x00, 0x09, 100000),
	REGULATOR_LINEAR_RANGE(1800000, 0xA, 0xF, 0),
};

static const struct regulator_linear_range bd71837_ldo7_volts[] = {
	REGULATOR_LINEAR_RANGE(1800000, 0x00, 0x0F, 100000),
};

static int find_index(const unsigned int *p,unsigned int len,unsigned int sel)
{
	for (unsigned int i = 0; i < len; i++)
	{
		if (sel == p[i])
			return i;
	}
	return -1;

}

static int find_index_struct(struct regulator_desc *rdev,unsigned int sel, int id)
{
	int b = 0;
	unsigned int min_sel1 = 0;
	unsigned int step = 0;
	min_sel1 = (rdev->linear_ranges)[id].min_sel;
	step = (rdev->linear_ranges)[id].uV_step;
	b = sel - (rdev->linear_ranges)[id].min_uV;
	b = b/step;
	b = min_sel1 + b;
	return b;

}

static void set_pmic_bd71837_pinmux(struct pmic_i2c *bd71837_i2c_config1)
{
	   // set pinmux
	   iprintf("set %s pinmux\n",bd71837_i2c_config1->name);
	   xPinmuxSet(bd71837_i2c_config1->scl, bd71837_i2c_config1->scl_value);
	   xPinmuxSet(bd71837_i2c_config1->sda, bd71837_i2c_config1->sda_value);
	   //set ds and pull up
	   xPinconfSet(bd71837_i2c_config1->scl, PINF_CONFIG_BIAS_PULL_UP | PINF_CONFIG_DRV_STRENGTH_3);
	   xPinconfSet(bd71837_i2c_config1->sda, PINF_CONFIG_BIAS_PULL_UP | PINF_CONFIG_DRV_STRENGTH_3);
}

static void BD71837_PMIC_I2C_INIT(struct pmic_i2c *bd71837_i2c_config) {
	set_pmic_bd71837_pinmux(bd71837_i2c_config);
	xI2cMesonPortInit(bd71837_i2c_config->port);
}

static int bd71837_regulator_ctrl(struct regulator_desc *rdev,int status)
{
	int ret = 0;
	unsigned char ctrl_reg = 0x0;
	if ((rdev->id)<= 7) {
		ret = xI2cMesonRead(bd718x7_slave_address,rdev->enable_reg,&ctrl_reg,1);
		if (ret < 0) {
			printf("i2c buck read failed\n");
			return ret;
		}
		ctrl_reg &= (~(rdev->enable_mask));
		if (status) {
			ctrl_reg |= rdev->enable_val;
		} else {
			ctrl_reg |= rdev->disable_val;
		}
		ret = xI2cMesonWrite(bd718x7_slave_address,rdev->enable_reg,&ctrl_reg,1);
		if (ret < 0) {
			printf("i2c buck write failed\n");
			return ret;
		}

	} else {
		ret = xI2cMesonRead(bd718x7_slave_address,rdev->ldo_reg,&ctrl_reg,1);
		if (ret < 0) {
			printf("i2c ldo read failed\n");
			return ret;
		}
		ctrl_reg &= (~(rdev->ldo_mask_ctrl));
		if (status) {
			ctrl_reg |= rdev->ldo_val_ctrl;
		} else {
			ctrl_reg |= rdev->ldo_val_ctrl_disable;
		}
		ret = xI2cMesonWrite(bd718x7_slave_address,rdev->ldo_reg,&ctrl_reg,1);
		if (ret < 0) {
			printf("i2c ldo write failed\n");
			return ret;
		}

	}
	return ret;
}

static int bd71837_regulator_set_voltage(struct regulator_desc *rdev, unsigned int sel)
{
	int a = 0;
	int ret = 0;
	unsigned char ctrl_reg = 0;
	if ((rdev->id)<= 3) { /* buck1-4 */
		if (sel <= 1290000) {
			a = find_index_struct(rdev,sel,0);
		} else {
			a = (rdev->linear_ranges)[1].min_sel;
		}
	} /* buck5 */
	if ((rdev->id) == 4) {
		if (sel <= 1000000) {
			a = find_index_struct(rdev,sel,0);
		} else if (1050000 <= sel && sel <= 1100000) {
			a = find_index_struct(rdev,sel,1);
		} else {
			a = find_index_struct(rdev,sel,2);
		}
	} /*  buck6 */
	if ((rdev->id) == 5) {
		a = find_index_struct(rdev,sel,0);
	} /*buck7 */
	if ((rdev->id) == 6) {
		a = find_index(rdev->volt_table,rdev->n_voltages,sel);
	} /*buck8*/
	if ((rdev->id) == 7) {
		if (sel <= 1358500) {
			a = find_index_struct(rdev,sel,0);
		} else {
			a = (rdev->linear_ranges)[1].min_sel;
		}
	}  /* ldo1 */
	if ((rdev->id) == 8) {
		a = find_index_struct(rdev,sel,0);
	} /* ldo2 */
	if ((rdev->id) == 9) {
		a = find_index(rdev->volt_table,rdev->n_voltages,sel);
		a = a << 5;
	} /* ldo3 */
	if ((rdev->id) == 10) {
		a = find_index_struct(rdev,sel,0);
	} /* ldo4 */
	if ((rdev->id) == 11) {
		if (sel <= 1700000) {
			a = find_index_struct(rdev,sel,0);
		} else {
			a = (rdev->linear_ranges)[1].min_sel;
		}
	} /* ldo5 */
	if ((rdev->id) == 12) {
		a = find_index_struct(rdev,sel,0);
	} /* ldo6 */
	if ((rdev->id) == 13) {
		if (sel <= 1700000) {
			a = find_index_struct(rdev,sel,0);
		} else {
			a = (rdev->linear_ranges)[1].min_sel;
		}
	} /* ldo7 */
	if ((rdev->id) == 14) {
		a = find_index_struct(rdev,sel,0);
	}
	if ((rdev->id) <= 7) {
		ret = xI2cMesonRead(bd718x7_slave_address,rdev->vsel_reg,&ctrl_reg,1);
		if (ret < 0) {
			printf("i2c buck read failed\n");
			return ret;
		}
		ctrl_reg &= (~(rdev->vsel_mask));
		ctrl_reg |= (a<<0);
		ret = xI2cMesonWrite(bd718x7_slave_address,rdev->vsel_reg,&ctrl_reg,1);
		if (ret < 0) {
			printf("i2c buck write failed\n");
			return ret;
		}
	}
	if ((rdev->id) > 7) {
		ret = xI2cMesonRead(bd718x7_slave_address,rdev->ldo_reg,&ctrl_reg,1);
		if (ret < 0) {
			printf("i2c ldo read failed\n");
			return ret;
		}
		ctrl_reg &= (~(rdev->ldo_out_mask));
		ctrl_reg |= a;
		ret = xI2cMesonWrite(bd718x7_slave_address,rdev->ldo_reg,&ctrl_reg,1);
		if (ret < 0) {
			printf("i2c ldo write failed\n");
			return ret;
		}
	}

	return ret;
}

static void bd71837_osc_ctrl(int status)
{
	unsigned char ctrl_reg = 0;
	int ret = 0;
	ret = xI2cMesonRead(bd718x7_slave_address,BD718XX_REG_OUT32K,&ctrl_reg,1);
	if (ret < 0) {
		printf("i2c osc read failed\n");
		return;
	}
	if (status) {
		ctrl_reg &= (~0x1);
		ctrl_reg |= 0x1;
	} else {
		ctrl_reg &= (~0x1);
		ctrl_reg |= 0x0;
	}
	ret = xI2cMesonWrite(bd718x7_slave_address,BD718XX_REG_OUT32K,&ctrl_reg,1);
	if (ret < 0) {
		printf("i2c osc write failed\n");
		return;
	}
	return;
}

static const struct regulator_ops bd718xx_dvs_buck_regulator_ops = {
	.ctrl = bd71837_regulator_ctrl,
	.set_voltage = bd71837_regulator_set_voltage,
};

struct regulator_desc bd71837_desc[15] = {
		{
			.name = "buck1",
			.id = BD718XX_BUCK1,
			.ops = &bd718xx_dvs_buck_regulator_ops,
			.linear_ranges = bd718xx_dvs_buck_volts,
			.n_linear_ranges = ARRAY_SIZE(bd718xx_dvs_buck_volts),
			.enable_reg = BD718XX_REG_BUCK1_CTRL,
			.enable_mask = 0x3,   //bit0 = 1, bit1 = 1
			.enable_val = 0x3,
			.disable_val = 0x2,  //bit0 = 0 ,bit1 = 1
			.vsel_reg = BD718XX_REG_BUCK1_VOLT_RUN,
			.vsel_mask = 0x3f,
		},
		{
			.name = "buck2",
			.id = BD718XX_BUCK2,
			.ops = &bd718xx_dvs_buck_regulator_ops,
			.linear_ranges = bd718xx_dvs_buck_volts,
			.n_linear_ranges = ARRAY_SIZE(bd718xx_dvs_buck_volts),
			.enable_reg = BD718XX_REG_BUCK2_CTRL,
			.enable_mask = 0x3,    //bit0 =1 ,bit1 = 1;
			.enable_val = 0x3,
			.disable_val = 0x2,  //bit0 = 0 ,bit1 = 1
			.vsel_reg = BD718XX_REG_BUCK2_VOLT_RUN,
			.vsel_mask = 0x3f,
		},
		{
			.name = "buck3",
			.id = BD718XX_BUCK3,
			.ops = &bd718xx_dvs_buck_regulator_ops,
			.linear_ranges = bd718xx_dvs_buck_volts,
			.n_linear_ranges = ARRAY_SIZE(bd718xx_dvs_buck_volts),
			.enable_reg = BD71837_REG_BUCK3_CTRL,
			.enable_mask = 0x7,    //bit0 =1 ,bit1 = 1 , bit2 = 1;
			.enable_val = 0x7,
			.disable_val = 0x2,  //bit0 = 0 ,bit1 = 1
			.vsel_reg = BD71837_REG_BUCK3_VOLT_RUN,
			.vsel_mask = 0x3f,
		},
		{
			.name = "buck4",
			.id = BD718XX_BUCK4,
			.ops = &bd718xx_dvs_buck_regulator_ops,
			.linear_ranges = bd718xx_dvs_buck_volts,
			.n_linear_ranges = ARRAY_SIZE(bd718xx_dvs_buck_volts),
			.enable_reg = BD71837_REG_BUCK4_CTRL,
			.enable_mask = 0x7, //bit0 =1 ,bit1 = 1 , bit2 = 1;
			.enable_val = 0x7,
			.disable_val = 0x2,  //bit0 = 0 ,bit1 = 1
			.vsel_reg = BD71837_REG_BUCK4_VOLT_RUN,
			.vsel_mask = 0x3f,
		},
		{
			.name = "buck5",
			.id = BD718XX_BUCK5,
			.ops = &bd718xx_dvs_buck_regulator_ops,
			.linear_ranges = bd71837_buck5_volts,
			.n_linear_ranges =
				ARRAY_SIZE(bd71837_buck5_volts),
			.enable_reg = BD718XX_REG_1ST_NODVS_BUCK_CTRL,
			.enable_mask = 0x3,   //bit0 =1 ,bit1 = 1
			.enable_val = 0x3,
			.disable_val = 0x2,  //bit0 = 0 ,bit1 = 1
			.vsel_reg = BD718XX_REG_1ST_NODVS_BUCK_VOLT,
			.vsel_mask = 0x7,  // 某认bit7 = 0

		},
		{
			.name = "buck6",
			.id = BD718XX_BUCK6,
			.ops = &bd718xx_dvs_buck_regulator_ops,
			.linear_ranges = bd71837_buck6_volts,
			.n_linear_ranges =
				ARRAY_SIZE(bd71837_buck6_volts),
			.enable_reg = BD718XX_REG_2ND_NODVS_BUCK_CTRL,
			.enable_mask = 0x3,   //bit0 =1 ,bit1 = 1,
			.enable_val = 0x3,
			.disable_val = 0x2,  //bit0 = 0 ,bit1 = 1
			.vsel_reg = BD718XX_REG_2ND_NODVS_BUCK_VOLT,
			.vsel_mask = 0x3,
		},
		{
			.name = "buck7",
			.id = BD718XX_BUCK7,
			.ops = &bd718xx_dvs_buck_regulator_ops,
			.volt_table = &bd71837_buck7_volts[0],
			.n_voltages = sizeof(bd71837_buck7_volts),
			.enable_reg = BD718XX_REG_3RD_NODVS_BUCK_CTRL,
			.enable_mask = 0x3,   //bit0 =1 ,bit1 = 1,
			.enable_val = 0x3,
			.disable_val = 0x2,  //bit0 = 0 ,bit1 = 1
			.vsel_reg = BD718XX_REG_3RD_NODVS_BUCK_VOLT,
			.vsel_mask = 0x7,
		},
		{
			.name = "buck8",
			.id = BD718XX_BUCK8,
			.ops = &bd718xx_dvs_buck_regulator_ops,
			.linear_ranges = bd71837_buck8_volts,
			.n_linear_ranges =
				ARRAY_SIZE(bd71837_buck8_volts),
			.enable_reg = BD718XX_REG_4TH_NODVS_BUCK_CTRL,
			.enable_mask = 0x3,   //bit0 =1 ,bit1 = 1,
			.enable_val = 0x3,
			.disable_val = 0x2,  //bit0 = 0 ,bit1 = 1
			.vsel_reg = BD718XX_REG_4TH_NODVS_BUCK_VOLT,
			.vsel_mask = 0x3f,
		},
		{
			.name = "ldo1",
			.id = BD718XX_LDO1,
			.ops = &bd718xx_dvs_buck_regulator_ops,
			.linear_ranges = bd718xx_ldo1_volts,
			.n_linear_ranges = ARRAY_SIZE(bd718xx_ldo1_volts),
			.ldo_reg = BD718XX_REG_LDO1_VOLT,
			.ldo_mask_ctrl = 0xe0,   //bit5 = 1,bit6 =1,bit7 =1
			.ldo_val_ctrl = 0xe0,
			.ldo_val_ctrl_disable = 0x80,  //bit6 =0 ,bit7 = 1
			.ldo_out_mask = 0x3,  //bit0-bit1 = 1

		},
		{
			.name = "ldo2",
			.id = BD718XX_LDO2,
			.ops = &bd718xx_dvs_buck_regulator_ops,
			.volt_table = &ldo_2_volts[0],
			.n_voltages =
					ARRAY_SIZE(bd71837_buck8_volts),
			.ldo_reg = BD718XX_REG_LDO2_VOLT,
			.ldo_mask_ctrl = 0xc0,  //bit6 =1,bit7 =1
			.ldo_val_ctrl = 0xc0,
			.ldo_val_ctrl_disable = 0x80,  //bit6 =0 ,bit7 = 1
			.ldo_out_mask = 0x20,  //bit5 = 1
		},
		{
			.name = "ldo3",
			.id = BD718XX_LDO3,
			.ops = &bd718xx_dvs_buck_regulator_ops,
			.linear_ranges = bd718xx_ldo3_volts,
			.n_linear_ranges = ARRAY_SIZE(bd718xx_ldo3_volts),
			.ldo_reg = BD718XX_REG_LDO3_VOLT,
			.ldo_mask_ctrl = 0xc0,  //bit6=1,bi7=1
			.ldo_val_ctrl = 0xc0,
			.ldo_val_ctrl_disable = 0x80,  //bit6 =0 ,bit7 = 1
			.ldo_out_mask = 0xf,  //bit0-bit3 = 1
		},
		{
			.name = "ldo4",
			.id = BD718XX_LDO4,
			.ops = &bd718xx_dvs_buck_regulator_ops,
			.linear_ranges = bd718xx_ldo4_volts,
			.n_linear_ranges = ARRAY_SIZE(bd718xx_ldo4_volts),
			.ldo_reg = BD718XX_REG_LDO4_VOLT,
			.ldo_mask_ctrl = 0xc0,  //bit6=1,bi7=1,
			.ldo_val_ctrl = 0xc0,
			.ldo_val_ctrl_disable = 0x80,  //bit6 =0 ,bit7 = 1
			.ldo_out_mask = 0xf,  //bit0-bit3 = 1
		},
		{
			.name = "ldo5",
			.id = BD718XX_LDO5,
			.ops = &bd718xx_dvs_buck_regulator_ops,
			.linear_ranges = bd71837_ldo5_volts,
			.n_linear_ranges = ARRAY_SIZE(bd71837_ldo5_volts),
			.ldo_reg = BD718XX_REG_LDO5_VOLT,
			.ldo_mask_ctrl = 0xc0,   //bit6=1,bi7=1,
			.ldo_val_ctrl = 0xc0,
			.ldo_val_ctrl_disable = 0x80,  //bit6 =0 ,bit7 = 1
			.ldo_out_mask = 0xf,  //bit0-bit3 = 1
		},
		{
			.name = "ldo6",
			.id = BD718XX_LDO6,
			.ops = &bd718xx_dvs_buck_regulator_ops,
			.linear_ranges = bd718xx_ldo6_volts,
			.n_linear_ranges = ARRAY_SIZE(bd718xx_ldo6_volts),
			.ldo_reg = BD718XX_REG_LDO6_VOLT,
			.ldo_mask_ctrl = 0xc0,   //bit6=1,bi7=1,
			.ldo_val_ctrl = 0xc0,
			.ldo_val_ctrl_disable = 0x80,  //bit6 =0 ,bit7 = 1
			.ldo_out_mask = 0xf,  //bit0-bit3 = 1
		},
		{
			.name = "ldo7",
			.id = BD718XX_LDO7,
			.ops = &bd718xx_dvs_buck_regulator_ops,
			.linear_ranges = bd71837_ldo7_volts,
			.n_linear_ranges = ARRAY_SIZE(bd71837_ldo7_volts),

			.ldo_reg = BD71837_REG_LDO7_VOLT,
			.ldo_mask_ctrl = 0xc0,   //bit6=1,bi7=1,
			.ldo_val_ctrl = 0xc0,
			.ldo_val_ctrl_disable = 0x80,  //bit6 =0 ,bit7 = 1
			.ldo_out_mask = 0xf,  //bit0-bit3 = 1
		},

};

struct pmic_regulator BD71837_PMIC = {
	.pmic_i2c_config = BD71837_PMIC_I2C_INIT,
	.osc_ctrl = bd71837_osc_ctrl,
	.rdev = bd71837_desc,
	.num = ARRAY_SIZE(bd71837_desc),
};




