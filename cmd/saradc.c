/*
 * Command for SARADC.
 *
 * Copyright (C) 2012 Amlogic.
 * Elvis Yu <elvis.yu@amlogic.com>
 *
 * Copyright (C) 2018 Amlogic.
 * Xingyu Chen <xingyu.chen@amlogic.com>
 */


#include <common.h>
#include <command.h>
#include <dm/uclass.h>
#include <asm/arch/saradc.h>

#define ENV_SARADC_VALUE "saradc_val"

static const char * const ch7_voltage[] = {
	"gnd",
	"vdd/4",
	"vdd/2",
	"vdd*3/4",
	"vdd",
};

static int current_channel = -1;

static int do_saradc_open(cmd_tbl_t *cmdtp, int flag, int argc,
		char * const argv[])
{
	int channel;

	channel = simple_strtoul(argv[1], NULL, 10);
	if ((channel < 0) || (channel >= MESON_SARADC_CH_MAX))
	{
		pr_err("No such channel(%d) in SARADC! open failed!\n",
				channel);
		return -1;
	}

	current_channel = channel;

	return 0;
}

static int do_saradc_close(cmd_tbl_t *cmdtp, int flag, int argc,
		char * const argv[])
{
	current_channel = -1;

	printf("SARADC closed.\n");

	return 0;
}

static int do_saradc_getval(cmd_tbl_t *cmdtp, int flag, int argc,
		char * const argv[])
{
	char value_str[10];
	unsigned int val;
	int ret;

	if (current_channel < 0) {
		pr_err("SARADC channel[%d] is invalid\n", current_channel);
		return -EINVAL;
	};

	ret = adc_channel_single_shot("adc", current_channel, &val);
	if (ret)
		return ret;

	printf("SARADC channel(%d) is %d.\n", current_channel, val);

	sprintf(value_str, "0x%x", val);

	env_set(ENV_SARADC_VALUE, value_str);

	return 0;
}

static int do_saradc_test(cmd_tbl_t *cmdtp, int flag, int argc,
		char * const argv[])
{
	struct udevice *dev;
	unsigned int val;
	int ret;
	int i;

	ret = uclass_get_device_by_name(UCLASS_ADC, "adc", &dev);
	if (ret)
		return ret;

	ret = adc_set_mode(dev, SARADC_CH_SELF_TEST, ADC_MODE_AVERAGE);
	if (ret)
		return ret;

	printf("saradc self-test by ch7:\n");

	for (i = 0; i < ARRAY_SIZE(ch7_voltage); i++) {
		ret = adc_select_input_voltage(dev, SARADC_CH_SELF_TEST, i);
		if (ret)
			return ret;

		udelay(10);

		ret = adc_start_channel(dev, SARADC_CH_SELF_TEST);
		if (ret)
			return ret;

		ret = adc_channel_data(dev, SARADC_CH_SELF_TEST, &val);
		if (ret)
			return ret;

		printf("%-7s : %d\n", ch7_voltage[i], val);
	}

	return 0;
}

static int do_saradc_get_in_range(cmd_tbl_t *cmdtp, int flag,
		int argc, char * const argv[])
{
	char value_str[10];
	int max, min;
	unsigned int val;
	int ret;

	ret = adc_channel_single_shot("adc", current_channel, &val);
	if (ret)
		return ret;

	min = simple_strtoul(argv[1], NULL, 10);
	max = simple_strtoul(argv[2], NULL, 10);
	int donot_setenv = 0;
	if (argc > 3) {
		donot_setenv = simple_strtoul(argv[2], NULL, 10);
	}
	if ((val < min) || (val > max)) {
		debug("SARADC channel(%d) is %d, Out of range(%d~%d)!\n",
			current_channel, val, min, max);
		return -1;
	}
	debug("SARADC channel(%d) is %d (%d~%d).\n",
		current_channel, val, min, max);
	sprintf(value_str, "0x%x", val);
	if (!donot_setenv)
		env_set(ENV_SARADC_VALUE, value_str);

	return 0;
}

static cmd_tbl_t cmd_saradc_sub[] = {
	U_BOOT_CMD_MKENT(open, 2, 0, do_saradc_open, "", ""),
	U_BOOT_CMD_MKENT(close, 1, 0, do_saradc_close, "", ""),
	U_BOOT_CMD_MKENT(getval, 1, 0, do_saradc_getval, "", ""),
	U_BOOT_CMD_MKENT(test, 1, 0, do_saradc_test, "", ""),
	U_BOOT_CMD_MKENT(get_in_range, 3, 0, do_saradc_get_in_range, "", ""),
};

static int do_saradc(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	cmd_tbl_t *c;

	/* Strip off leading 'bmp' command argument */
	argc--;
	argv++;
	c = find_cmd_tbl(argv[0], &cmd_saradc_sub[0],
				ARRAY_SIZE(cmd_saradc_sub));
	if (c) {
		return	c->cmd(cmdtp, flag, argc, argv);
	} else {
		cmd_usage(cmdtp);
		return 1;
	}
}

U_BOOT_CMD(
	saradc,	CONFIG_SYS_MAXARGS, 0, do_saradc,
	"saradc sub-system",
	"saradc open <channel> - open a SARADC channel\n"
	"saradc close  - close the SARADC\n"
	"saradc getval - get the value in current channel\n"
	"saradc test   - test the SARADC by channel-7\n"
	"saradc get_in_range <min> <max> - \
		return 0 if current value in the range of current channel\n"
);
