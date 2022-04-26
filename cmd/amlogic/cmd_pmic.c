// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <i2c.h>
#include <dm.h>

#define I2C_BUS_ID		9
#define BD71888_CHIP_DEV	0x4b
#define BD2657_CHIP_DEV		0x6a

struct udevice *cmd_dev;

/* addr = 8bit, val = 8bit */
int common_write(uint8_t pmic_addr, uint8_t addr, uint8_t val)
{
	uint8_t val_tmp = val;
	int ret;

	ret = i2c_get_chip_for_busnum(I2C_BUS_ID, pmic_addr, 1, &cmd_dev);
	if (ret)
		printf("%s i2c get chip failed\n", __func__);
	ret = dm_i2c_write(cmd_dev, addr, &val_tmp, 1);
	if (ret)
		printf("%s i2c write failed\n", __func__);

    return 0;

}

int common_read(uint8_t pmic_addr, uint8_t addr, uint8_t *val)
{
	int ret;

	ret = i2c_get_chip_for_busnum(I2C_BUS_ID, pmic_addr, 1, &cmd_dev);
	if (ret)
		printf("%s i2c get chip failed\n", __func__);
	ret = dm_i2c_read(cmd_dev, addr, val, 1);
    if (ret < 0) {
        printf("%s: pmic read fault, ret:%d\n", __FUNCTION__, ret);
    }
	return 0;
}

static int do_i2c_write_bd71888(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 100;

	uint8_t write_adress = simple_strtoul(argv[1], NULL, 16);
	uint8_t write_val = simple_strtoul(argv[2], NULL, 16);

	ret = common_write(BD71888_CHIP_DEV,write_adress,write_val);
	if (ret < 0) {
        printf("%s: bd71888 pmic write fault, ret:%d\n", __FUNCTION__, ret);
        return ret;
    }

	printf("write_val = 0x%x\n",write_val);

    return 0;
}

U_BOOT_CMD(
	bd71888_i2c_write,	3,	1,	do_i2c_write_bd71888,
	"test i2c",
	"/N\n"
	"bd71888_i2c_write \n"
	"\n"
);

static int do_i2c_read_bd71888(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 100;
	uint8_t read_val = 0x33;

	uint8_t read_adress = simple_strtoul(argv[1], NULL, 16);

	ret = common_read(BD71888_CHIP_DEV,read_adress,&read_val);
	if (ret < 0) {
        printf("%s: bd71888 aml_pmic read fault, ret:%d\n", __FUNCTION__, ret);
        return ret;
    }

	printf("read_val = 0x%x\n",read_val);

    return 0;
}

U_BOOT_CMD(
	bd71888_i2c_read,	2,	1,	do_i2c_read_bd71888,
	"test i2c",
	"/N\n"
	"bd71888_i2c_read \n"
	"\n"
);


static int do_i2c_write_bd2657(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 100;

	uint8_t write_adress = simple_strtoul(argv[1], NULL, 16);
	uint8_t write_val = simple_strtoul(argv[2], NULL, 16);

	ret = common_write(BD2657_CHIP_DEV,write_adress,write_val);
	if (ret < 0) {
        printf("%s: bd71888 pmic write fault, ret:%d\n", __FUNCTION__, ret);
        return ret;
    }

	printf("write_val = 0x%x\n",write_val);

    return 0;
}

U_BOOT_CMD(
	bd2657_i2c_write,	3,	1,	do_i2c_write_bd2657,
	"test i2c",
	"/N\n"
	"bd2657_i2c_write \n"
	"\n"
);

static int do_i2c_read_bd2657(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 100;
	uint8_t read_val = 0x33;

	uint8_t read_adress = simple_strtoul(argv[1], NULL, 16);

	ret = common_read(BD2657_CHIP_DEV,read_adress,&read_val);
	if (ret < 0) {
        printf("%s: bd71888 aml_pmic read fault, ret:%d\n", __FUNCTION__, ret);
        return ret;
    }

	printf("read_val = 0x%x\n",read_val);

    return 0;
}

U_BOOT_CMD(
	bd2657_i2c_read,	2,	1,	do_i2c_read_bd2657,
	"test i2c",
	"/N\n"
	"bd2657_i2c_read \n"
	"\n"
);




