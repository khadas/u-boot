#include <common.h>
#include <bootretry.h>
#include <cli.h>
#include <command.h>
#include <dm.h>
#include <edid.h>
#include <environment.h>
#include <errno.h>
#include <i2c.h>
#include <adc.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <linux/compiler.h>
#include <asm/u-boot.h>
#include "asm/arch-rockchip/vendor.h"

#define CHIP_ADDR              0x18
#define CHIP_ADDR_CHAR         "0x18"
#define I2C_SPEED              100000
#define MCU_I2C_BUS_NUM         2

#define REG_PASSWD_VENDOR       0x00
#define REG_USID                0x06
#define REG_VERSION             0x12

#define REG_BOOT_MODE           0x20
#define REG_BOOT_EN_DCIN        0x21
#define REG_BOOT_EN_RTC         0x22
#define REG_LED_ON              0x23
#define REG_LED_OFF             0x24
#define REG_LED_RGB_ON_R        0x25
#define REG_LED_RGB_ON_G        0x26
#define REG_LED_RGB_ON_B        0x27
#define REG_LED_RGB_OFF_R       0x28
#define REG_LED_RGB_OFF_G       0x29
#define REG_LED_RGB_OFF_B       0x2A
#define REG_REST_CONF           0x2C
#define REG_SLEEP_EN            0x2E

#define REG_PWD_WRITE           0x81
#define REG_PWD_DATA            0x82
#define REG_LED_ON_RAM          0x89
#define REG_FAN_CTRL            0x8A
#define REG_WDT_EN              0x8B

#define REG_SYS_RST             0x91
#define REG_BOOT_FLAG           0x92
#define REG_FAN_TEST            0x96

#define BOOT_EN_RTC         1
#define BOOT_EN_DCIN        2

#define LED_OFF_MODE            0x0
#define LED_ON_MODE             0x1
#define LED_R_BREATHE_MODE      0x2
#define LED_G_BREATHE_MODE      0x3
#define LED_B_BREATHE_MODE      0x4
#define LED_RG_BREATHE_MODE     0x5
#define LED_RB_BREATHE_MODE     0x6
#define LED_GB_BREATHE_MODE     0x7
#define LED_RGB_BREATHE_MODE    0x8
#define LED_R_HEARTBEAT_MODE    0x9
#define LED_G_HEARTBEAT_MODE    0xa
#define LED_B_HEARTBEAT_MODE    0xb
#define LED_RG_HEARTBEAT_MODE   0xc
#define LED_RB_HEARTBEAT_MODE   0xd
#define LED_GB_HEARTBEAT_MODE   0xe
#define LED_RGB_HEARTBEAT_MODE  0xf

#define LED_SYSTEM_OFF      0
#define LED_SYSTEM_ON       1

#define BOOT_MODE_SPI       0
#define BOOT_MODE_EMMC      1

#define VERSION_LENGHT        2
#define USID_LENGHT           7
#define PASSWD_CUSTOM_LENGHT  6
#define PASSWD_VENDOR_LENGHT  6

#define BOARD_TYPE_EDGE2    1	/* Single Edge2 Board */
#define BOARD_TYPE_EDGEV    2
#define BOARD_TYPE_CAPTAIN  3
#define BOARD_TYPE_UNKNOW   0


extern int vendor_storage_init(void);

static char* LED_MODE_STR[] = {
	"off",
	"on",
	"r_breathe",
	"g_breathe",
	"b_breathe",
	"rg_breathe",
	"rb_breathe",
	"gb_breathe",
	"rgb_breathe",
	"r_heartbeat",
	"g_heartbeat",
	"b_heartbeat",
	"rg_heartbeat",
	"rb_heartbeat",
	"gb_heartbeat",
	"rgb_heartbeat",
	"unknown"
};

static int led_str_to_mode(char *str)
{
	int i = 0;

	if (NULL == str)
		return -1;

	for (i=0; i<sizeof(LED_MODE_STR)/sizeof(char *); i++)
		if (0 == strcmp(str, LED_MODE_STR[i]))
			break;

	if (i == sizeof(LED_MODE_STR)/sizeof(char *))
		return -1;
	else
		return i;
}

#ifdef CONFIG_DM_I2C
static struct udevice *i2c_cur_bus;

static int cmd_i2c_set_bus_num(unsigned int busnum)
{
    struct udevice *bus;
    int ret;

    ret = uclass_get_device_by_seq(UCLASS_I2C, busnum, &bus);
    if (ret) {
        printf("%s: No bus %d\n", __func__, busnum);
        return ret;
    }
    i2c_cur_bus = bus;

    return 0;
}

static int i2c_get_cur_bus(struct udevice **busp)
{
	if (!i2c_cur_bus) {
		if (cmd_i2c_set_bus_num(MCU_I2C_BUS_NUM)) {
		    printf("Default I2C bus %d not found\n",
		           MCU_I2C_BUS_NUM);
		    return -ENODEV;
		}
	}

    if (!i2c_cur_bus) {
        puts("No I2C bus selected\n");
        return -ENODEV;
    }
    *busp = i2c_cur_bus;

    return 0;
}

static int i2c_get_cur_bus_chip(uint chip_addr, struct udevice **devp)
{
    struct udevice *bus;
    int ret;

    ret = i2c_get_cur_bus(&bus);
    if (ret)
        return ret;

    return i2c_get_chip(bus, chip_addr, 1, devp);
}
#endif

static int kbi_i2c_read(uint reg)
{
	int ret;
	char val[64];
	uchar   linebuf[1];
	uchar chip;
#ifdef CONFIG_DM_I2C
	struct udevice *dev;
#endif


	chip = simple_strtoul(CHIP_ADDR_CHAR, NULL, 16);

#ifdef CONFIG_DM_I2C
	ret = i2c_get_cur_bus_chip(chip, &dev);
	if (!ret)
		ret = dm_i2c_read(dev, reg, (uint8_t *)linebuf, 1);
#else
	ret = i2c_read(chip, reg, 1, linebuf, 1);
#endif

	if (ret)
		printf("Error reading the chip: %d\n",ret);
	else {
		sprintf(val, "%d", linebuf[0]);
		ret = simple_strtoul(val, NULL, 10);

	}
	return ret;
}

static void  kbi_i2c_read_block(uint start_reg, int count, char val[])
{
	uint addr;
	int nbytes;
	int ret;
	uchar chip;
#ifdef CONFIG_DM_I2C
	struct udevice *dev;
#endif

	chip = simple_strtoul(CHIP_ADDR_CHAR, NULL, 16);

	nbytes = count;
	addr = start_reg;
	do {
		unsigned char   linebuf[1];
#ifdef CONFIG_DM_I2C
		ret = i2c_get_cur_bus_chip(chip, &dev);
		if (!ret)
			ret = dm_i2c_read(dev, addr, (uint8_t *)linebuf, 1);
#else
		ret = i2c_read(chip, addr, 1, linebuf, 1);
#endif
		if (ret)
			printf("Error reading the chip: %d\n",ret);
		else
			val[count-nbytes] =  linebuf[0];

		addr++;
		nbytes--;

	} while (nbytes > 0);
}

static void get_version(void)
{
	char version[VERSION_LENGHT] = {};
	int i;

	kbi_i2c_read_block(REG_VERSION, VERSION_LENGHT, version);
	printf("version: ");
	for (i=0; i< VERSION_LENGHT; i++) {
		printf("%02x ",version[i]);
	}
	printf("\n");
}

static void get_usid(void)
{
	char serial[64]={0};
	char usid[USID_LENGHT] = {0};

	kbi_i2c_read_block(REG_USID, USID_LENGHT, usid);
	sprintf(serial, "%02X%02X%02X%02X%02X%02X%02X",usid[0],usid[1],usid[2],usid[3],usid[4],usid[5],usid[6]);
	printf("usid:%s\r\n",serial);
	env_set("usid", serial);
}

static void get_rtc(void)
{
	int enable;
	enable = kbi_i2c_read(REG_BOOT_EN_RTC);
	printf("boot rtc: %s\n", enable==1 ? "enable" : "disable" );
}

static void set_rtc(int enable)
{
	char cmd[64];
	sprintf(cmd, "i2c mw %x %x %d 1",CHIP_ADDR, REG_BOOT_EN_RTC, enable);
	run_command(cmd, 0);

}

static void get_dcin(void)
{
	int enable;
	enable = kbi_i2c_read(REG_BOOT_EN_DCIN);
	printf("boot dcin: %s\n", enable==1 ? "enable" : "disable" );
}

static void set_dcin(int enable)
{
	char cmd[64];
	sprintf(cmd, "i2c mw %x %x %d 1",CHIP_ADDR, REG_BOOT_EN_DCIN, enable);
	run_command(cmd, 0);
}

static void get_boot_enable(int type)
{
	if (type == BOOT_EN_RTC)
		get_rtc();
	else if (type == BOOT_EN_DCIN)
		get_dcin();
}

static void set_boot_enable(int type, int enable)
{
	if (type == BOOT_EN_RTC)
		set_rtc(enable);
	else if (type == BOOT_EN_DCIN)
		set_dcin(enable);
}

static void get_led_mode(int type)
{
	int mode;
	if (type == LED_SYSTEM_OFF) {
		mode = kbi_i2c_read(REG_LED_OFF);
		if ((mode >= LED_OFF_MODE) && (mode <= LED_RGB_HEARTBEAT_MODE)) {
			if (LED_ON_MODE == mode || LED_OFF_MODE == mode) {
				printf("[systemoff] led mode: %s[R:0x%02x G:0x%02x B:0x%02x]\n", LED_MODE_STR[mode],
						kbi_i2c_read(REG_LED_RGB_OFF_R),
						kbi_i2c_read(REG_LED_RGB_OFF_G),
						kbi_i2c_read(REG_LED_RGB_OFF_B));
			} else {
				printf("[systemoff] led mode: %s\n", LED_MODE_STR[mode]);
			}
		} else {
			printf("read led mode err\n");
		}
	} else {
		mode = kbi_i2c_read(REG_LED_ON);
		if ((mode >= LED_OFF_MODE) && (mode <= LED_RGB_HEARTBEAT_MODE)) {
			if (LED_ON_MODE == mode || LED_OFF_MODE == mode) {
				printf("[systemon] led mode: %s[R:0x%02x G:0x%02x B:0x%02x]\n", LED_MODE_STR[mode],
					kbi_i2c_read(REG_LED_RGB_ON_R),
					kbi_i2c_read(REG_LED_RGB_ON_G),
					kbi_i2c_read(REG_LED_RGB_ON_B));
			} else {
				printf("[systemon] led mode: %s\n",LED_MODE_STR[mode]);
			}
		} else {
			printf("read led mode err\n");
		}
	}
}

static int set_led_mode(int type, char *led, int mode)
{
	char cmd[128];
	if (type == LED_SYSTEM_OFF) {

		if (LED_OFF_MODE == mode || LED_ON_MODE == mode) {
			if (LED_OFF_MODE == mode) {
				if (0 == strcmp("r", led)) {
						sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1",
									CHIP_ADDR, REG_LED_OFF, LED_ON_MODE,
									CHIP_ADDR, REG_LED_RGB_OFF_R, 0x0);
				} else if (0 == strcmp("g", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_OFF, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_OFF_G, 0x0);
				} else if (0 == strcmp("b", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_OFF, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_OFF_B, 0x0);
				} else if (0 == strcmp("rg", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_OFF, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_OFF_R, 0x0,
								CHIP_ADDR, REG_LED_RGB_OFF_G, 0x0);
				} else if (0 == strcmp("rb", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_OFF, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_OFF_R, 0x0,
								CHIP_ADDR, REG_LED_RGB_OFF_B, 0x0);
				} else if (0 == strcmp("bg", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_OFF, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_OFF_G, 0x0,
								CHIP_ADDR, REG_LED_RGB_OFF_B, 0x0);
				} else if (0 == strcmp("rgb", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_OFF, LED_OFF_MODE,
								CHIP_ADDR, REG_LED_RGB_OFF_R, 0x0,
								CHIP_ADDR, REG_LED_RGB_OFF_G, 0x0,
								CHIP_ADDR, REG_LED_RGB_OFF_B, 0x0);
				}
			} else {
				if (0 == strcmp("r", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_OFF, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_OFF_R, 0xff);
				} else if (0 == strcmp("g", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_OFF, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_OFF_G, 0xff);
				} else if (0 == strcmp("b", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_OFF, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_OFF_B, 0xff);
				} else if (0 == strcmp("rg", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_OFF, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_OFF_R, 0xff,
								CHIP_ADDR, REG_LED_RGB_OFF_G, 0xff);
				} else if (0 == strcmp("rb", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_OFF, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_OFF_R, 0xff,
								CHIP_ADDR, REG_LED_RGB_OFF_B, 0xff);
				} else if (0 == strcmp("bg", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_OFF, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_OFF_G, 0xff,
								CHIP_ADDR, REG_LED_RGB_OFF_B, 0xff);
				} else if (0 == strcmp("rgb", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_OFF, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_OFF_R, 0xff,
								CHIP_ADDR, REG_LED_RGB_OFF_G, 0xff,
								CHIP_ADDR, REG_LED_RGB_OFF_B, 0xff);
				}
			}
		} else {
			sprintf(cmd, "i2c mw %x %x %x 1",CHIP_ADDR, REG_LED_OFF, mode);
		}
	} else if (type == LED_SYSTEM_ON) {

		if (LED_OFF_MODE == mode || LED_ON_MODE == mode) {
			if (LED_OFF_MODE == mode) {
				if (0 == strcmp("r", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_ON, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_ON_R, 0x0);
				} else if (0 == strcmp("g", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_ON, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_ON_G, 0x0);
				} else if (0 == strcmp("b", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_ON, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_ON_B, 0x0);
				} else if (0 == strcmp("rg", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_ON, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_ON_R, 0x0,
								CHIP_ADDR, REG_LED_RGB_ON_G, 0x0);
				} else if (0 == strcmp("rb", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_ON, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_ON_R, 0x0,
								CHIP_ADDR, REG_LED_RGB_ON_B, 0x0);
				} else if (0 == strcmp("bg", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_ON, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_ON_G, 0x0,
								CHIP_ADDR, REG_LED_RGB_ON_B, 0x0);
				} else if (0 == strcmp("rgb", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_ON, LED_OFF_MODE,
								CHIP_ADDR, REG_LED_RGB_ON_R, 0x0,
								CHIP_ADDR, REG_LED_RGB_ON_G, 0x0,
								CHIP_ADDR, REG_LED_RGB_ON_B, 0x0);
				}
			} else {
				if (0 == strcmp("r", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_ON, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_ON_R, 0xff);
				} else if (0 == strcmp("g", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_ON, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_ON_G, 0xff);
				} else if (0 == strcmp("b", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_ON, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_ON_B, 0xff);
				} else if (0 == strcmp("rg", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_ON, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_ON_R, 0xff,
								CHIP_ADDR, REG_LED_RGB_ON_G, 0xff);
				} else if (0 == strcmp("rb", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_ON, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_ON_R, 0xff,
								CHIP_ADDR, REG_LED_RGB_ON_B, 0xff);
				} else if (0 == strcmp("bg", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_ON, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_ON_G, 0xff,
								CHIP_ADDR, REG_LED_RGB_ON_B, 0xff);
				} else if (0 == strcmp("rgb", led)) {
					sprintf(cmd, "i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1; i2c mw %x %x %x 1",
								CHIP_ADDR, REG_LED_ON, LED_ON_MODE,
								CHIP_ADDR, REG_LED_RGB_ON_R, 0xff,
								CHIP_ADDR, REG_LED_RGB_ON_G, 0xff,
								CHIP_ADDR, REG_LED_RGB_ON_B, 0xff);
				}
			}
		} else {
			sprintf(cmd, "i2c mw %x %x %x 1",CHIP_ADDR, REG_LED_ON, mode);
		}

	} else {
		return CMD_RET_USAGE;
	}

	run_command(cmd, 0);
    return 0;
}

static int do_kbi_init(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	// switch to i2c2
	run_command("i2c dev 2", 0);

	return 0;
}

static int do_kbi_version(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	get_version();
	return 0;

}

static int do_kbi_usid(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	get_usid();
	return 0;
}

static int do_kbi_led(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	int led_mode = 0;
	char led_mode_str[32] = {0};
	if (argc < 4)
		return CMD_RET_USAGE;

	if (strcmp(argv[1], "systemoff") ==0) {
		if (strcmp(argv[2], "r") == 0) {
			get_led_mode(LED_SYSTEM_OFF);
		} else if (strcmp(argv[2], "w") == 0) {
			if (argc < 5)
				return CMD_RET_USAGE;

			if (0 == strcmp(argv[4], "on") || 0 == strcmp(argv[4], "off")) {
				if (0 != strcmp(argv[3], "r") &&
					0 != strcmp(argv[3], "g") &&
					0 != strcmp(argv[3], "b") &&
					0 != strcmp(argv[3], "rg") &&
					0 != strcmp(argv[3], "rb") &&
					0 != strcmp(argv[3], "gb") &&
					0 != strcmp(argv[3], "rgb"))
					return CMD_RET_USAGE;
				sprintf(led_mode_str, "%s", argv[4]);
			} else
				sprintf(led_mode_str, "%s_%s", argv[3], argv[4]);

			led_mode = led_str_to_mode(led_mode_str);
			if (led_mode < 0)
				return CMD_RET_USAGE;

			set_led_mode(LED_SYSTEM_OFF, argv[3], led_mode);

		}
	} else if (strcmp(argv[1], "systemon") ==0) {

		if (strcmp(argv[2], "r") == 0) {
			get_led_mode(LED_SYSTEM_ON);
		} else if (strcmp(argv[2], "w") == 0) {
			if (argc < 5)
				return CMD_RET_USAGE;

			if (0 == strcmp(argv[4], "on") || 0 == strcmp(argv[4], "off")) {
				if (0 != strcmp(argv[3], "r") &&
					0 != strcmp(argv[3], "g") &&
					0 != strcmp(argv[3], "b") &&
					0 != strcmp(argv[3], "rg") &&
					0 != strcmp(argv[3], "rb") &&
					0 != strcmp(argv[3], "gb") &&
					0 != strcmp(argv[3], "rgb"))
					return CMD_RET_USAGE;
				sprintf(led_mode_str, "%s", argv[4]);
			} else
				sprintf(led_mode_str, "%s_%s", argv[3], argv[4]);

			led_mode = led_str_to_mode(led_mode_str);
			if (led_mode < 0)
				return CMD_RET_USAGE;

			set_led_mode(LED_SYSTEM_ON, argv[3], led_mode);
		}
	} else {
		return CMD_RET_USAGE;
	}
	return ret;
}

static int do_kbi_trigger(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 3)
		return CMD_RET_USAGE;

	if (strcmp(argv[2], "r") == 0) {

		if (strcmp(argv[1], "rtc") == 0)
			get_boot_enable(BOOT_EN_RTC);
		else if (strcmp(argv[1], "dcin") == 0)
			get_boot_enable(BOOT_EN_DCIN);
		else
			return CMD_RET_USAGE;
	} else if (strcmp(argv[2], "w") == 0) {
		if (argc < 4)
			return CMD_RET_USAGE;
		if ((strcmp(argv[3], "1") != 0) && (strcmp(argv[3], "0") != 0)) {
			return CMD_RET_USAGE;
		}

		if (strcmp(argv[1], "rtc") == 0) {

			if (strcmp(argv[3], "1") == 0)
				set_boot_enable(BOOT_EN_RTC, 1);
			else
				set_boot_enable(BOOT_EN_RTC, 0);
		} else if (strcmp(argv[1], "dcin") == 0) {
			if (strcmp(argv[3], "1") == 0)
				set_boot_enable(BOOT_EN_DCIN, 1);
			else
				set_boot_enable(BOOT_EN_DCIN, 0);
		} else {
			return CMD_RET_USAGE;
		}
	} else {
		return CMD_RET_USAGE;
	}

	return 0;
}


static cmd_tbl_t cmd_kbi_sub[] = {
	U_BOOT_CMD_MKENT(init, 1, 1, do_kbi_init, "", ""),
	U_BOOT_CMD_MKENT(usid, 1, 1, do_kbi_usid, "", ""),
	U_BOOT_CMD_MKENT(version, 1, 1, do_kbi_version, "", ""),
	U_BOOT_CMD_MKENT(led, 4, 1, do_kbi_led, "", ""),
	U_BOOT_CMD_MKENT(trigger, 4, 1, do_kbi_trigger, "", ""),
};

static int do_kbi(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	cmd_tbl_t *c;

	if (argc < 2)
		return CMD_RET_USAGE;

	/* Strip off leading 'kbi' command argument */
	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_kbi_sub[0], ARRAY_SIZE(cmd_kbi_sub));

	if (c)
		return c->cmd(cmdtp, flag, argc, argv);
	else
		return CMD_RET_USAGE;

}
static char kbi_help_text[] =
		"[function] [mode] [write|read] <value>\n"
		"\n"
		"kbi version - read version information\n"
		"kbi usid - read usid information\n"
		"\n"
		"kbi led [systemoff|systemon] w [r|g|b|rg|rb|gb|rgb] <off|on|breathe|heartbeat> - set rgb led mode\n"
		"kbi led [systemoff|systemon] r [r|g|b|rg|rb|gb|rgb] - read rgb led mode\n"
		"\n"
		"kbi trigger [rtc|dcin] w <0|1> - disable/enable boot trigger\n"
		"kbi trigger [rtc|dcin] r - read mode of a boot trigger";


U_BOOT_CMD(
		kbi, 6, 1, do_kbi,
		"Khadas Bootloader Instructions sub-system",
		kbi_help_text
);
