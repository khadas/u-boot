/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright (C) 2018 ROHM Semiconductors */

#ifndef __PMIC_H__

#define __PMIC_H__

//#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define NULL1 ((void *)0)

#define PMIC_ENBALE   1
#define PMIC_DISABLE  0
#define PMIC_OSC_ENABLE 1
#define PMIC_OSC_DISENABLE 0
#define PMIC_MAXNUM 5

#define REGULATOR_LINEAR_RANGE(_min_uV, _min_sel, _max_sel, _step_uV)	\
{									\
	.min_uV		= _min_uV,					\
	.min_sel	= _min_sel,					\
	.max_sel	= _max_sel,					\
	.uV_step	= _step_uV,					\
}

/*
 * Regulators can either control voltage or current.
 */
enum regulator_type {
	REGULATOR_VOLTAGE,
	REGULATOR_CURRENT,
};

struct regulator_desc {
	const char *name;
	int id;
	const struct regulator_ops *ops;

/* buck ctrl reg */
	unsigned int enable_reg;
	unsigned int enable_mask;
	unsigned int enable_val;
	unsigned int disable_val;
/* buck out */
	unsigned int vsel_reg;
	unsigned int vsel_mask;
/* ldo ctrl */
	unsigned int ldo_reg;
	unsigned int ldo_mask_ctrl;
	unsigned int ldo_val_ctrl;
	unsigned int ldo_val_ctrl_disable;
	unsigned int ldo_out_mask;
	unsigned int n_linear_ranges;
	const struct regulator_linear_range *linear_ranges;
	const unsigned int *volt_table;
	unsigned int n_voltages;
};

struct regulator_ops {
	/* enable/disable regulator */
	int (*ctrl) (struct regulator_desc *rdev,int status);
	/* get/set regulator voltage */
	int (*set_voltage) (struct regulator_desc *rdev,unsigned int sel);
};

struct regulator_linear_range {
	unsigned int min_uV;
	unsigned int min_sel;
	unsigned int max_sel;
	unsigned int uV_step;
};

struct pmic_i2c {
	char *name;
	int scl;
	int scl_value;
	int sda;
	int sda_value;
	int port;
};

struct pmic_regulator {
	void (*pmic_i2c_config)(struct pmic_i2c *pmic_i2c);
	/* set pmic enable/disable */
	void (*osc_ctrl)(int status);
	struct regulator_desc *rdev;
	unsigned int num;
};

/**
 *@ pmic_regulators_register() - Pmic i2c config and enable
 *@ dev_id: regulator dev_id
 */
extern void pmic_i2c_init(int dev_id,struct pmic_i2c *pmic_i2c);

/**
 * pmic_regulators_register() - Pmic regulators register
 * @PmicRegulator: regulator device
 * @dev_id: regulator dev_id
 */
extern int pmic_regulators_register(struct pmic_regulator *PmicRegulator, int *dev_id);


/**
 * pmic_regulator_ctrl() - Pmic regulators enable/disable
 * @dev_id: regulator dev_id
 * @id: buck/ldo id
 * @status: regulators status, enable/disable
 */

extern int pmic_regulator_ctrl(int dev_id, int id, int status);


/**
 * pmic_regulator_set_voltage() - Pmic regulators set voltage
 * @dev_id: regulator dev_id
 * @id: buck/ldo id
 * @sel: buck/ldo voltage(uv)
 */
extern int pmic_regulator_set_voltage(int dev_id, int id,int sel);

/**
 * pmic_osc() - Pmic Crystal oscillator
 * @dev_id: regulator dev_id
 * @status: regulators status, enable/disable
 */
extern void pmic_osc(int dev_id, int status);

#endif /* __PMIC_H__ */
