/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/cmd_lcd.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <amlogic/aml_lcd.h>

static unsigned int lcd_parse_vout_name(char *name)
{
	char *p, *frac_str;
	unsigned int frac = 0;

	p = strchr(name, ',');
	if (!p) {
		frac = 0;
	} else {
		frac_str = p + 1;
		*p = '\0';
		if (strcmp(frac_str, "frac") == 0)
			frac = 1;
	}

	return frac;
}

static int do_lcd_probe(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct aml_lcd_drv_s *lcd_drv;

	lcd_drv = aml_lcd_get_driver();
	if (lcd_drv) {
		if (lcd_drv->lcd_probe)
			lcd_drv->lcd_probe();
		else
			printf("no lcd probe\n");
	} else {
		printf("no lcd driver\n");
	}
	return 0;
}

static int do_lcd_enable(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct aml_lcd_drv_s *lcd_drv = aml_lcd_get_driver();
	char *mode, *str;
	unsigned int frac;

	str = getenv("outputmode");
	if (!str) {
		printf("no outputmode\n");
		return -1;
	}

	mode = (char *)malloc(64);
	if (!mode) {
		printf("%s: create mode failed\n", __func__);
		return -1;
	}
	memset(mode, 0, sizeof(mode));
	sprintf(mode, "%s", str);
	frac = lcd_parse_vout_name(mode);

	if (lcd_drv) {
		if (lcd_drv->lcd_enable)
			lcd_drv->lcd_enable(mode, frac);
		else
			printf("no lcd enable\n");
	} else {
		printf("no lcd driver\n");
	}

	free(mode);
	return 0;
}

static int do_lcd_disable(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct aml_lcd_drv_s *lcd_drv;

	lcd_drv = aml_lcd_get_driver();
	if (lcd_drv) {
		if (lcd_drv->lcd_disable)
			lcd_drv->lcd_disable();
		else
			printf("no lcd disable\n");
	} else {
		printf("no lcd driver\n");
	}
	return 0;
}

static int do_lcd_ss(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct aml_lcd_drv_s *lcd_drv;
	int value, temp;
	int ret = 0;

	if (argc == 1) {
		return -1;
	}

	lcd_drv = aml_lcd_get_driver();
	if (lcd_drv == NULL) {
		printf("no lcd driver\n");
		return ret;
	}
	if (strcmp(argv[1], "level") == 0) {
		if (argc == 3) {
			value = (unsigned int)simple_strtoul(argv[2], NULL, 10);
			value &= 0xff;
			if (lcd_drv->lcd_set_ss)
				lcd_drv->lcd_set_ss(value, 0xff, 0xff);
			else
				printf("no lcd lcd_set_ss\n");
		} else {
			ret = -1;
		}
	} else if (strcmp(argv[1], "freq") == 0) {
		if (argc == 3) {
			value = (unsigned int)simple_strtoul(argv[2], NULL, 10);
			value &= 0xf;
			if (lcd_drv->lcd_set_ss)
				lcd_drv->lcd_set_ss(0xff, value, 0xff);
			else
				printf("no lcd lcd_set_ss\n");
		} else {
			ret = -1;
		}
	} else if (strcmp(argv[1], "mode") == 0) {
		if (argc == 3) {
			value = (unsigned int)simple_strtoul(argv[2], NULL, 10);
			value &= 0xf;
			if (lcd_drv->lcd_set_ss)
				lcd_drv->lcd_set_ss(0xff, 0xff, value);
			else
				printf("no lcd lcd_set_ss\n");
		} else {
			ret = -1;
		}
	} else if (strcmp(argv[1], "set") == 0) {
		if (argc == 3) {
			value = (unsigned int)simple_strtoul(argv[2], NULL, 16);
			value &= 0xffff;
			if (lcd_drv->lcd_set_ss) {
				temp = value >> 8;
				lcd_drv->lcd_set_ss((value & 0xff),
					((temp >> LCD_CLK_SS_BIT_FREQ) & 0xf),
					((temp >> LCD_CLK_SS_BIT_MODE) & 0xf));
			} else {
				printf("no lcd lcd_set_ss\n");
			}
		} else {
			ret = -1;
		}
	} else if (strcmp(argv[1], "get") == 0) {
		if (lcd_drv->lcd_get_ss)
			lcd_drv->lcd_get_ss();
		else
			printf("no lcd_get_ss\n");
	} else {
		ret = -1;
	}
	return ret;
}

static int do_lcd_bl(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct aml_lcd_drv_s *lcd_drv;
	int level;
	int ret = 0;

	if (argc == 1) {
		return -1;
	}

	lcd_drv = aml_lcd_get_driver();
	if (lcd_drv == NULL) {
		printf("no lcd driver\n");
		return ret;
	}
	if (strcmp(argv[1], "on") == 0) {
		if (lcd_drv->bl_on)
			lcd_drv->bl_on();
		else
			printf("no lcd bl_on\n");
	} else if (strcmp(argv[1], "off") == 0) {
		if (lcd_drv->bl_off)
			lcd_drv->bl_off();
		else
			printf("no lcd bl_off\n");
	} else if (strcmp(argv[1], "set") == 0) {
		if (argc == 3) {
			level = (int)simple_strtoul(argv[2], NULL, 10);
			if (lcd_drv->set_bl_level)
				lcd_drv->set_bl_level(level);
			else
				printf("no lcd set_bl_level\n");
		} else {
			ret = -1;
		}
	} else if (strcmp(argv[1], "get") == 0) {
		if (lcd_drv->get_bl_level) {
			level = lcd_drv->get_bl_level();
			printf("lcd get_bl_level: %d\n", level);
		} else {
			printf("no lcd set_bl_level\n");
		}
	} else if (strcmp(argv[1], "info") == 0) {
		if (lcd_drv->bl_config_print) {
			lcd_drv->bl_config_print();
		} else {
			printf("no lcd bl_config_print\n");
		}
	} else {
		ret = -1;
	}
	return ret;
}

static int do_lcd_clk(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct aml_lcd_drv_s *lcd_drv;

	lcd_drv = aml_lcd_get_driver();
	if (lcd_drv) {
		if (lcd_drv->lcd_info)
			lcd_drv->lcd_clk();
		else
			printf("no lcd clk\n");
	} else {
		printf("no lcd driver\n");
	}
	return 0;
}

static int do_lcd_info(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct aml_lcd_drv_s *lcd_drv;

	lcd_drv = aml_lcd_get_driver();
	if (lcd_drv) {
		if (lcd_drv->lcd_info)
			lcd_drv->lcd_info();
		else
			printf("no lcd info\n");
	} else {
		printf("no lcd driver\n");
	}
	return 0;
}

static int do_lcd_print(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct aml_lcd_drv_s *lcd_drv;
	unsigned int val = 0;

	if (argc == 1)
		return -1;
	val = (unsigned int)simple_strtoul(argv[1], NULL, 10);

	lcd_drv = aml_lcd_get_driver();
	if (!lcd_drv) {
		printf("no lcd driver\n");
		return 0;
	}
	if (lcd_drv->debug_print_set)
		lcd_drv->debug_print_set(val);

	return 0;
}

#ifdef CONFIG_AML_LCD_TCON
static int do_lcd_tcon(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct aml_lcd_drv_s *lcd_drv;
	unsigned int addr, val, len;
	int ret = 0, i;

	if (argc == 1)
		return -1;

	lcd_drv = aml_lcd_get_driver();
	if (lcd_drv == NULL) {
		printf("no lcd driver\n");
		return 0;
	}
	if (strcmp(argv[1], "reg") == 0) {
		if (lcd_drv->lcd_tcon_reg_print)
			lcd_drv->lcd_tcon_reg_print();
		else
			printf("no lcd tcon_reg_print\n");
	} else if (strcmp(argv[1], "table") == 0) {
		if (lcd_drv->lcd_tcon_table_print)
			lcd_drv->lcd_tcon_table_print();
		else
			printf("no lcd tcon_table_print\n");
	} else if (strcmp(argv[1], "tw") == 0) {
		addr = (unsigned int)simple_strtoul(argv[2], NULL, 16);
		val = (unsigned int)simple_strtoul(argv[3], NULL, 16);
		if (lcd_drv->lcd_tcon_table_write) {
			val = lcd_drv->lcd_tcon_table_write(addr, val);
			printf("write tcon table[0x%04x] = 0x%02x\n", addr, val);
		} else {
			printf("no lcd_tcon_table_write\n");
		}
	} else if (strcmp(argv[1], "tr") == 0) {
		addr = (unsigned int)simple_strtoul(argv[2], NULL, 16);
		if (lcd_drv->lcd_tcon_table_read) {
			val = lcd_drv->lcd_tcon_table_read(addr);
			printf("read tcon table[0x%04x] = 0x%02x\n", addr, val);
		} else {
			printf("no lcd_tcon_table_write\n");
		}
	} else if (strcmp(argv[1], "wb") == 0) {
		if (lcd_drv->lcd_tcon_reg_write) {
			addr = (unsigned int)simple_strtoul(argv[2], NULL, 16);
			val = (unsigned int)simple_strtoul(argv[3], NULL, 16);
			lcd_drv->lcd_tcon_reg_write(addr, val, 1);
			printf("tcon byte write: 0x%04x = 0x%02x, readback 0x%02x\n",
				addr, val, lcd_drv->lcd_tcon_reg_read(addr, 1));
		} else {
			printf("no lcd_tcon_reg_write\n");
		}
	} else if (strcmp(argv[1], "rb") == 0) {
		if (lcd_drv->lcd_tcon_reg_read) {
			addr = (unsigned int)simple_strtoul(argv[2], NULL, 16);
			val = lcd_drv->lcd_tcon_reg_read(addr, 1);
			printf("tcon byte read: 0x%04x = 0x%02x\n", addr, val);
		} else {
			printf("no lcd_tcon_reg_read\n");
		}
	} else if (strcmp(argv[1], "db") == 0) {
		if (lcd_drv->lcd_tcon_reg_read) {
			addr = (unsigned int)simple_strtoul(argv[2], NULL, 16);
			len = (unsigned int)simple_strtoul(argv[3], NULL, 10);
			printf("tcon byte reg dump:\n");
			for (i = 0; i < len; i++) {
				val = lcd_drv->lcd_tcon_reg_read((addr + i), 1);
				printf("  0x%04x = 0x%02x\n", (addr + i), val);
			}
		} else {
			printf("no lcd_tcon_reg_read\n");
		}
	} else if (strcmp(argv[1], "w") == 0) {
		if (lcd_drv->lcd_tcon_reg_write) {
			addr = (unsigned int)simple_strtoul(argv[2], NULL, 16);
			val = (unsigned int)simple_strtoul(argv[3], NULL, 16);
			lcd_drv->lcd_tcon_reg_write(addr, val, 0);
			printf("tcon write: 0x%04x = 0x%08x, readback 0x%08x\n",
				addr, val, lcd_drv->lcd_tcon_reg_read(addr, 0));
		} else {
			printf("no lcd_tcon_reg_write\n");
		}
	} else if (strcmp(argv[1], "r") == 0) {
		if (lcd_drv->lcd_tcon_reg_read) {
			addr = (unsigned int)simple_strtoul(argv[2], NULL, 16);
			val = lcd_drv->lcd_tcon_reg_read(addr, 0);
			printf("tcon read: 0x%04x = 0x%08x\n", addr, val);
		} else {
			printf("no lcd_tcon_reg_read\n");
		}
	} else if (strcmp(argv[1], "d") == 0) {
		if (lcd_drv->lcd_tcon_reg_read) {
			addr = (unsigned int)simple_strtoul(argv[2], NULL, 16);
			len = (unsigned int)simple_strtoul(argv[3], NULL, 10);
			printf("tcon reg dump:\n");
			for (i = 0; i < len; i++) {
				val = lcd_drv->lcd_tcon_reg_read((addr + i), 0);
				printf("  0x%04x = 0x%08x\n", (addr + i), val);
			}
		} else {
			printf("no lcd_tcon_reg_read\n");
		}
	} else if (strcmp(argv[1], "vac") == 0) {
		if (lcd_drv->lcd_tcon_vac_print)
			lcd_drv->lcd_tcon_vac_print();
		else
			printf("no lcd tcon_vac_print\n");
	} else if (strcmp(argv[1], "demura") == 0) {
		if (lcd_drv->lcd_tcon_demura_print)
			lcd_drv->lcd_tcon_demura_print();
		else
			printf("no lcd tcon_demura_print\n");
	} else if (strcmp(argv[1], "acc") == 0) {
		if (lcd_drv->lcd_tcon_acc_print)
			lcd_drv->lcd_tcon_acc_print();
		else
			printf("no lcd tcon_acc_print\n");
	} else if (strcmp(argv[1], "data") == 0) {
		if (argc == 3) {
			i = (unsigned char)simple_strtoul(argv[2], NULL, 10);
			if (lcd_drv->lcd_tcon_data_print)
				lcd_drv->lcd_tcon_data_print(i);
			else
				printf("no lcd tcon_data_print\n");
		} else {
			if (lcd_drv->lcd_tcon_data_print)
				lcd_drv->lcd_tcon_data_print(0xff);
			else
				printf("no lcd tcon_data_print\n");
		}
	} else if (strcmp(argv[1], "spi") == 0) {
		if (lcd_drv->lcd_tcon_spi_print)
			lcd_drv->lcd_tcon_spi_print();
		else
			printf("no lcd tcon_spi_print\n");
	} else {
		ret = -1;
	}
	return ret;
}
#endif

static int do_lcd_vbyone(cmd_tbl_t *cmdtp, int flag, int argc,
			 char * const argv[])
{
	struct aml_lcd_drv_s *lcd_drv;
	int ret = 0;

	if (argc == 1)
		return -1;

	lcd_drv = aml_lcd_get_driver();
	if (lcd_drv == NULL) {
		printf("no lcd driver\n");
		return 0;
	}
	if (strcmp(argv[1], "rst") == 0) {
		if (lcd_drv->lcd_vbyone_rst)
			lcd_drv->lcd_vbyone_rst();
		else
			printf("no lcd vbyone rst\n");
	} else if (strcmp(argv[1], "cdr") == 0) {
		if (lcd_drv->lcd_vbyone_cdr)
			lcd_drv->lcd_vbyone_cdr();
		else
			printf("no lcd vbyone cdr\n");
	} else {
		ret = -1;
	}

	return ret;
}

static int do_lcd_reg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct aml_lcd_drv_s *lcd_drv;

	lcd_drv = aml_lcd_get_driver();
	if (lcd_drv) {
		if (lcd_drv->lcd_reg)
			lcd_drv->lcd_reg();
		else
			printf("no lcd reg\n");
	} else {
		printf("no lcd driver\n");
	}
	return 0;
}

static int do_lcd_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct aml_lcd_drv_s *lcd_drv;
	int num;

	if (argc == 1) {
		return -1;
	}
	num = (int)simple_strtoul(argv[1], NULL, 10);

	lcd_drv = aml_lcd_get_driver();
	if (lcd_drv) {
		if (lcd_drv->lcd_test)
			lcd_drv->lcd_test(num);
	} else {
		printf("no lcd driver\n");
	}
	return 0;
}

static int do_lcd_prbs(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct aml_lcd_drv_s *lcd_drv;
	unsigned int s, prbs_mode_flag;

	if (argc == 1) {
		return -1;
	}

	if (strcmp(argv[1], "vx1") == 0) {
		if (argc == 3)
			prbs_mode_flag = LCD_PRBS_MODE_VX1;
		else
			return -1;
	} else if (strcmp(argv[1], "lvds") == 0) {
		if (argc == 3)
			prbs_mode_flag = LCD_PRBS_MODE_LVDS;
		else
			return -1;
	} else {
		prbs_mode_flag = LCD_PRBS_MODE_LVDS | LCD_PRBS_MODE_VX1;
	}

	s = (unsigned int)simple_strtoul(argv[2], NULL, 10);

	lcd_drv = aml_lcd_get_driver();
	if (lcd_drv) {
		if (lcd_drv->lcd_prbs)
			lcd_drv->lcd_prbs(s, prbs_mode_flag);
		else
			printf("no lcd prbs\n");
	} else {
		printf("no lcd driver\n");
	}
	return 0;
}

static int do_lcd_key(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct aml_lcd_drv_s *lcd_drv;
	int tmp = 0;

	if (argc == 1) {
		return -1;
	}

	lcd_drv = aml_lcd_get_driver();
	if (lcd_drv == NULL) {
		printf("no lcd driver\n");
		return 0;
	}
	if (strcmp(argv[1], "flag") == 0) {
		if (argc == 3) {
			tmp = (int)simple_strtoul(argv[2], NULL, 10);
			lcd_drv->unifykey_test_flag = tmp;
			if (tmp) {
				printf("enable lcd unifykey test\n");
				printf("Be Careful!! This test will overwrite lcd unifykeys!!\n");
			} else {
				printf("disable lcd unifykey test\n");
			}
		} else {
			return -1;
		}
	} else if (strcmp(argv[1], "test") == 0) {
		if (lcd_drv->unifykey_test)
			lcd_drv->unifykey_test();
		else
			printf("no lcd unifykey_test\n");
	} else if (strcmp(argv[1], "dump") == 0) {
		if (argc == 3) {
			if (strcmp(argv[2], "tcon") == 0)
				tmp = (1 << 1);
			if (lcd_drv->unifykey_dump)
				lcd_drv->unifykey_dump(tmp);
			else
				printf("no lcd unifykey_dump\n");
		} else {
			tmp = (1 << 0);
			if (lcd_drv->unifykey_dump)
				lcd_drv->unifykey_dump(tmp);
			else
				printf("no lcd unifykey_dump\n");
		}
	}
	return 0;
}

#ifdef CONFIG_AML_LCD_EXTERN
static int do_lcd_ext(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct aml_lcd_drv_s *lcd_drv;
	struct aml_lcd_extern_driver_s *lcd_ext;
	unsigned char buf[2];
	unsigned int index;

	if (argc == 1)
		return -1;

	index = (unsigned int)simple_strtoul(argv[1], NULL, 10);
	lcd_drv = aml_lcd_get_driver();
	if (!lcd_drv) {
		printf("no lcd driver\n");
		return 0;
	}

	lcd_ext = aml_lcd_extern_get_driver(index);
	if (!lcd_ext) {
		printf("no lcd_ext_%d\n", index);
		return 0;
	}

	if (argc < 3)
		return 0;

	if (strcmp(argv[2], "info") == 0) {
		if (lcd_ext->info_print)
			lcd_ext->info_print(lcd_ext);
		else
			printf("no lcd lcd_extern_info\n");
	} else if (strcmp(argv[2], "r") == 0) {
		if (lcd_ext->reg_read) {
			buf[0] = (unsigned char)simple_strtoul(argv[3],
							       NULL,
							       16);
			lcd_ext->reg_read(buf[0], &buf[1]);
			printf("lcd_ext read: 0x%02x = 0x%02x\n",
			       buf[0], buf[1]);
		} else {
			printf("%s: lcd_ext reg_read is null\n",
			       __func__);
		}
	} else if (strcmp(argv[2], "w") == 0) {
		if (lcd_ext->reg_write) {
			buf[0] = (unsigned char)simple_strtoul(argv[3],
							       NULL,
							       16);
			buf[1] = (unsigned char)simple_strtoul(argv[4],
							       NULL,
							       16);
			lcd_ext->reg_write(buf, 2);
			printf("lcd_ext write: 0x%02x = 0x%02x\n",
			       buf[0], buf[1]);
		} else {
			printf("%s: lcd_ext reg_write is null\n",
			       __func__);
		}
	} else if (strcmp(argv[2], "power") == 0) {
		buf[0] = (unsigned char)simple_strtoul(argv[3],
						       NULL, 10);
		if (buf[0]) {
			if (lcd_ext->power_on)
				lcd_ext->power_on();
			else
				printf("%s: lcd_ext power_on is null\n",
				       __func__);
		} else {
			if (lcd_ext->power_off)
				lcd_ext->power_off();
			else
				printf("%s:lcd_ext power_off is null\n",
				       __func__);
		}
	}

	return 0;
}
#endif

static cmd_tbl_t cmd_lcd_sub[] = {
	U_BOOT_CMD_MKENT(probe,   2, 0, do_lcd_probe, "", ""),
	U_BOOT_CMD_MKENT(enable,  2, 0, do_lcd_enable, "", ""),
	U_BOOT_CMD_MKENT(disable, 2, 0, do_lcd_disable, "", ""),
	U_BOOT_CMD_MKENT(ss,   4, 0, do_lcd_ss, "", ""),
	U_BOOT_CMD_MKENT(bl,   4, 0, do_lcd_bl,   "", ""),
	U_BOOT_CMD_MKENT(clk , 2, 0, do_lcd_clk, "", ""),
	U_BOOT_CMD_MKENT(info, 2, 0, do_lcd_info, "", ""),
	U_BOOT_CMD_MKENT(print, 3, 0, do_lcd_print, "", ""),
#ifdef CONFIG_AML_LCD_TCON
	U_BOOT_CMD_MKENT(tcon, 3, 0, do_lcd_tcon, "", ""),
#endif
	U_BOOT_CMD_MKENT(vbyone, 3, 0, do_lcd_vbyone, "", ""),
	U_BOOT_CMD_MKENT(reg,  2, 0, do_lcd_reg, "", ""),
	U_BOOT_CMD_MKENT(test, 3, 0, do_lcd_test, "", ""),
	U_BOOT_CMD_MKENT(prbs, 2, 0, do_lcd_prbs, "", ""),
	U_BOOT_CMD_MKENT(key,  4, 0, do_lcd_key, "", ""),
#ifdef CONFIG_AML_LCD_EXTERN
	U_BOOT_CMD_MKENT(ext,  3, 0, do_lcd_ext, "", ""),
#endif
};

static int do_lcd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	cmd_tbl_t *c;

	/* Strip off leading 'bmp' command argument */
	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_lcd_sub[0], ARRAY_SIZE(cmd_lcd_sub));

	if (c) {
		return c->cmd(cmdtp, flag, argc, argv);
	} else {
		cmd_usage(cmdtp);
		return 1;
	}
}

U_BOOT_CMD(
	lcd,	5,	0,	do_lcd,
	"lcd sub-system",
	"lcd probe        - probe lcd parameters\n"
	"lcd enable       - enable lcd module\n"
	"lcd disable      - disable lcd module\n"
	"lcd ss           - lcd pll spread spectrum operation\n"
	"lcd bl           - lcd backlight operation\n"
	"lcd clk          - show lcd pll & clk parameters\n"
	"lcd info         - show lcd parameters\n"
	"lcd print        - enable lcd debug print\n"
	"lcd tcon         - show lcd tcon debug\n"
	"lcd vbyone       - show lcd vbyone debug\n"
	"lcd reg          - dump lcd registers\n"
	"lcd test         - show lcd bist pattern\n"
	"lcd key          - show lcd unifykey test\n"
#ifdef CONFIG_AML_LCD_EXTERN
	"lcd ext          - show lcd extern information\n"
#endif
);
