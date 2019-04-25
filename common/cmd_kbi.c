#include <common.h>
#include <bootretry.h>
#include <cli.h>
#include <command.h>
#include <dm.h>
#include <edid.h>
#include <environment.h>
#include <errno.h>
#include <i2c.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <linux/compiler.h>
#include <asm/arch/io.h>
#include <asm/u-boot.h>
#include <asm/arch/rkplat.h>
#include <../board/rockchip/common/storage/storage.h>

#define CHIP_ADDR              0x18
#define CHIP_ADDR_CHAR         "0x18"
#define I2C_SPEED              100000

#define REG_PASSWD_VENDOR       0x00
#define REG_MAC                 0x06
#define REG_USID                0x0c
#define REG_VERSION             0x12

#define REG_BOOT_MODE           0x20
#define REG_BOOT_EN_WOL         0x21
#define REG_BOOT_EN_RTC         0x22
#define REG_BOOT_EN_IR          0x24
#define REG_BOOT_EN_DCIN        0x25
#define REG_BOOT_EN_KEY			0x26
#define REG_EN_TST         		0x26
#define REG_BOOT_EN_GPIO        0x23
#define REG_LED_SYSTEM_ON_MODE  0x28
#define REG_LED_SYSTEM_OFF_MODE 0x29
#define REG_ADC                 0x2a
#define REG_MAC_SWITCH          0x2d
#define REG_IR_CODE1            0x2f
#define REG_IR_CODE2            0x32

#define REG_PASSWD_CUSTOM       0x40

#define REG_POWER_OFF           0x80
#define REG_PASSWD_CHECK_STATE  0x81
#define REG_PASSWD_CHECK_VENDOR 0x82
#define REG_PASSWD_CHECK_CUSTOM 0x83
#define REG_POWER_STATE    0x86

#define BOOT_EN_WOL         0
#define BOOT_EN_RTC         1
#define BOOT_EN_IR          2
#define BOOT_EN_DCIN        3
#define BOOT_EN_KEY         4
#define BOOT_EN_GPIO        5

#define LED_OFF_MODE        0
#define LED_ON_MODE         1
#define LED_BREATHE_MODE    2
#define LED_HEARTBEAT_MODE  3

#define LED_SYSTEM_OFF      0
#define LED_SYSTEM_ON       1

#define BOOT_MODE_SPI       0
#define BOOT_MODE_EMMC      1

#define FORCERESET_WOL      0
#define FORCERESET_GPIO     1

#define VERSION_LENGHT        2
#define USID_LENGHT           6
#define MAC_LENGHT            6
#define ADC_LENGHT            2
#define PASSWD_CUSTOM_LENGHT  6
#define PASSWD_VENDOR_LENGHT  6

// Board Detect
#define BOARD_DETECT_ADC_DEVIATION      10
#define BOARD_DETECT_ADC_VALUE_EDGE     1024
#define BOARD_DETECT_ADC_VALUE_EDGEV    204
#define BOARD_DETECT_ADC_VALUE_CAPTAIN  17

#define BOARD_TYPE_UNKNOW   0   /* Unknow board */
#define BOARD_TYPE_EDGE     1   /* Single Edge Board */
#define BOARD_TYPE_EDGEV    2   /* Single Edge V Board */
#define BOARD_TYPE_CAPTAIN  3   /* Edge with Captain */

#define SARADC_BASE             RKIO_SARADC_PHYS
#define read_XDATA32(address)   (*((uint32 volatile*)(address)))
#define write_XDATA32(address, value)   (*((uint32 volatile*)(address)) = value)

static char* LED_MODE_STR[] = { "off", "on", "breathe", "heartbeat"};

extern void DRVDelayUs(uint32 count);

static int kbi_i2c_read(uint reg)
{
	int ret;
	char val[64];
	uchar   linebuf[1];
	uchar chip;


	chip = simple_strtoul(CHIP_ADDR_CHAR, NULL, 16);
	i2c_set_bus_num(8);
	i2c_init(I2C_SPEED, 0);
	ret = i2c_read(chip, reg, 1, linebuf, 1);

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
	int ret;
	int nbytes;
	uint addr;
	uchar chip;

	chip = simple_strtoul(CHIP_ADDR_CHAR, NULL, 16);
	nbytes = count;
	addr = start_reg;
	do {
		unsigned char   linebuf[1];
		ret = i2c_read(chip, addr, 1, linebuf, 1);
	    if (ret)
			printf("Error reading the chip: %d\n",ret);
		else
			val[count-nbytes] =  linebuf[0];

		addr++;
		nbytes--;

	} while (nbytes > 0);

}

static unsigned char chartonum(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'A' && c <= 'F')
		return (c - 'A') + 10;
	if (c >= 'a' && c <= 'f')
		return (c - 'a') + 10;
	return 0;
}

static int get_forcereset_wol(bool is_print)
{
	int enable;
	enable = kbi_i2c_read(REG_BOOT_EN_WOL);
	if (is_print)
	printf("wol forcereset: %s\n", enable&0x02 ? "enable":"disable");
	setenv("wol_forcereset", enable&0x02 ? "1" : "0");
	return enable;

}

static int get_wol(bool is_print)
{
	int enable;
	enable = kbi_i2c_read(REG_BOOT_EN_WOL);
	if (is_print)
	printf("boot wol: %s\n", enable&0x01 ? "enable":"disable");
	setenv("wol_enable", enable&0x01 ?"1" : "0");
	return enable;
}

static void set_wol(bool is_shutdown, int enable)
{
	char cmd[64];
	int mode;

	if ((enable&0x01) != 0) {

//	int mac_addr[MAC_LENGHT] = {0};
//	if (is_shutdown)
//		run_command("phyreg w 0 0", 0);
//	else
//		run_command("phyreg w 0 0x1040", 0);
//
//	run_command("phyreg w 31 0xd40", 0);
//	run_command("phyreg w 22 0x20", 0);
//	run_command("phyreg w 31 0", 0);

//	mode = kbi_i2c_read(REG_MAC_SWITCH);
//	if (mode == 1) {
//		kbi_i2c_read_block(REG_MAC, MAC_LENGHT, mac_addr);
//	} else {
//		run_command("efuse mac", 0);
//		char *s = getenv("eth_mac");
//		if ((s != NULL) && (strcmp(s, "00:00:00:00:00:00") != 0)) {
//			printf("getmac = %s\n", s);
//			int i = 0;
//			for (i = 0; i < 6 && s[0] != '\0' && s[1] != '\0'; i++) {
//			mac_addr[i] = chartonum(s[0]) << 4 | chartonum(s[1]);
//			s +=3;
//			}
//		} else {
//			kbi_i2c_read_block(REG_MAC, MAC_LENGHT, mac_addr);
//		}
//	}
//	run_command("phyreg w 31 0xd8c", 0);
//	sprintf(cmd, "phyreg w 16 0x%x%x", mac_addr[1], mac_addr[0]);
//	run_command(cmd, 0);
//	sprintf(cmd, "phyreg w 17 0x%x%x", mac_addr[3], mac_addr[2]);
//	run_command(cmd, 0);
//	sprintf(cmd, "phyreg w 18 0x%x%x", mac_addr[5], mac_addr[4]);
//	run_command(cmd, 0);
//	run_command("phyreg w 31 0", 0);
//
//	run_command("phyreg w 31 0xd8a", 0);
//	run_command("phyreg w 17 0x9fff", 0);
//	run_command("phyreg w 31 0", 0);
//
//	run_command("phyreg w 31 0xd8a", 0);
//	run_command("phyreg w 16 0x1000", 0);
//	run_command("phyreg w 31 0", 0);
//
//	run_command("phyreg w 31 0xd80", 0);
//	run_command("phyreg w 16 0x3000", 0);
//	run_command("phyreg w 17 0x0020", 0);
//	run_command("phyreg w 18 0x03c0", 0);
//	run_command("phyreg w 19 0x0000", 0);
//	run_command("phyreg w 20 0x0000", 0);
//	run_command("phyreg w 21 0x0000", 0);
//	run_command("phyreg w 22 0x0000", 0);
//	run_command("phyreg w 23 0x0000", 0);
//	run_command("phyreg w 31 0", 0);
//
//	run_command("phyreg w 31 0xd8a", 0);
//	run_command("phyreg w 19 0x1002", 0);
//	run_command("phyreg w 31 0", 0);
//
// } else {
//	run_command("phyreg w 31 0xd8a", 0);
//	run_command("phyreg w 16 0", 0);
//	run_command("phyreg w 17 0x7fff", 0);
//	run_command("phyreg w 31 0", 0);
  }

	sprintf(cmd, "i2c mw %x %x %d 1", CHIP_ADDR, REG_BOOT_EN_WOL, enable);
	run_command(cmd, 0);
//	printf("%s: %d\n", __func__, enable);
}

static void get_version(void)
{
	char version[VERSION_LENGHT] = {};
	int i;
	kbi_i2c_read_block(REG_VERSION, VERSION_LENGHT, version);
	printf("version: ");
	for (i=0; i< VERSION_LENGHT; i++) {
		printf("%x",version[i]);
	}
	printf("\n");
}

static void get_mac(void)
{
	char mac[64];
	char mac_addr[MAC_LENGHT] = {0};
	int i, mode;

	mode = kbi_i2c_read(REG_MAC_SWITCH);

	if (mode == 1) {
		kbi_i2c_read_block(REG_MAC, MAC_LENGHT, mac_addr);
	} else {
		int ret;
		ret = vendor_storage_init();
		if (ret) {
			printf("KBI: vendor_storage_init failed %d\n", ret);
			return;
		}

		ret = vendor_storage_read(3, mac_addr, MAC_LENGHT);
		if (MAC_LENGHT == ret && !is_zero_ether_addr((const u8 *)mac_addr)) {
			printf("read mac from vendor successfully!\n");
		} else {
			kbi_i2c_read_block(REG_MAC, MAC_LENGHT, mac_addr);
		}
	}
	printf("mac address: ");
	for (i=0; i<MAC_LENGHT; i++) {
		if (i == (MAC_LENGHT-1))
			printf("%02x",mac_addr[i]);
		else
			printf("%02x:",mac_addr[i]);
	}
	printf("\n");
	sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
	setenv("eth_mac", mac);
}

static const char *board_type_to_string(char board_type)
{
	switch (board_type) {
		case BOARD_TYPE_EDGEV:
			return "Edge-V";
		case BOARD_TYPE_EDGE:
			return "Edge";
		case BOARD_TYPE_CAPTAIN:
			return "Captain";
		default:
			return "UnknowBorad";
	}
}

static char detect_board(void)
{
	int ret;
	uint32 val;
	char board_type = -1;
	char str[4] = {0};

	uint32 index = 0;
	uint32 adc_data = SARADC_BASE;
	uint32 adc_stas = SARADC_BASE+4;
	uint32 adc_ctrl = SARADC_BASE+8;
	uint32 timeout = 0;

	write_XDATA32(adc_ctrl, 0);
	DRVDelayUs(1);
	write_XDATA32(adc_ctrl,0x0028|index);
	DRVDelayUs(1);
	do {
		val = read_XDATA32(adc_ctrl);
		timeout++;
	} while ((val&0x40) == 0);
	val = read_XDATA32(adc_data);

	if (val >= (BOARD_DETECT_ADC_VALUE_EDGEV - BOARD_DETECT_ADC_DEVIATION) && val <= (BOARD_DETECT_ADC_VALUE_EDGEV + BOARD_DETECT_ADC_DEVIATION)) {
		board_type = BOARD_TYPE_EDGEV;
	} else if (val >= (BOARD_DETECT_ADC_VALUE_CAPTAIN - BOARD_DETECT_ADC_DEVIATION) && val <= (BOARD_DETECT_ADC_VALUE_CAPTAIN + BOARD_DETECT_ADC_DEVIATION)) {
		board_type = BOARD_TYPE_CAPTAIN;
	} else if (val >= (BOARD_DETECT_ADC_VALUE_EDGE - BOARD_DETECT_ADC_DEVIATION) && val <= (BOARD_DETECT_ADC_VALUE_EDGE + BOARD_DETECT_ADC_DEVIATION)) {
		board_type = BOARD_TYPE_EDGE;
	} else {
		board_type = BOARD_TYPE_UNKNOW;
	}
	printf("board type = %s (%d)\n", board_type_to_string(board_type), board_type);

	sprintf(str, "%d", board_type);
	setenv("board_type", str);
	setenv("board_type_name", board_type_to_string(board_type));
	return board_type;
}

static void get_usid(void)
{
	char serial[64];
	char usid[USID_LENGHT] = {};
	int i, mode;
#ifdef CONFIG_USID_FROM_ETH_MAC
	mode = kbi_i2c_read(REG_MAC_SWITCH);

	if (mode == 1) {
		kbi_i2c_read_block(REG_MAC, MAC_LENGHT, usid);
	} else {
		run_command("efuse mac", 0);
		char *s = getenv("eth_mac");
		if ((s != NULL) && (strcmp(s, "00:00:00:00:00:00") != 0)) {
			for (i = 0; i < 6 && s[0] != '\0' && s[1] != '\0'; i++) {
			usid[i] = chartonum(s[0]) << 4 | chartonum(s[1]);
			s +=3;
			}
		} else {
			kbi_i2c_read_block(REG_MAC, MAC_LENGHT, usid);
		}
	}
#else
	kbi_i2c_read_block(REG_USID, USID_LENGHT, usid);
#endif
	printf("usid: ");
	for (i=0; i< USID_LENGHT; i++) {
		printf("%x",usid[i]);
	}
	printf("\n");
	sprintf(serial, "%02x%02x%02x%02x%02x%02x",usid[0],usid[1],usid[2],usid[3],usid[4],usid[5]);
	setenv("usid", serial);
}

static void get_adc(void)
{
	char adc[ADC_LENGHT] = {};
	kbi_i2c_read_block(REG_ADC, ADC_LENGHT, adc);
	printf("adc: 0x%x\n",(adc[0] << 8) | adc[1]);

}


static void get_power_state(void)
{
	int val;
	val = kbi_i2c_read(REG_POWER_STATE);
	if (val == 0) {
		printf("normal power on\n");
		setenv("power_state","0");
	} else if (val == 1) {
		printf("abort power off\n");
		setenv("power_state","1");
	} else if (val == 2) {
		printf("normal power off\n");
		setenv("power_state","2");
	} else {
		printf("state err\n");
		setenv("power_state","f");
	}
}

static void set_poweroff(void)
{
	u8 reg;

	i2c_set_bus_num(4);
	reg = i2c_reg_read(0x1b, 0x4b);
	i2c_reg_write(0x1b, 0x4b, (reg |(0x1 <<3)));
	i2c_set_bus_num(8);
}

static void set_bootmode(int mode)
{
	char cmd[64];
	sprintf(cmd, "i2c mw %x %x %d 1",CHIP_ADDR, REG_BOOT_MODE, mode);
	run_command(cmd, 0);

}

static void get_bootmode(void)
{
	int mode;
	mode = kbi_i2c_read(REG_BOOT_MODE);

	if (mode == BOOT_MODE_EMMC) {
		printf("bootmode: emmc\n");
	} else if (mode == BOOT_MODE_SPI) {
		printf("bootmode: spi\n");
	} else {
		printf("bootmode err: %d\n",mode);
	}
}

static void set_tst(int mode)
{
	char cmd[64];
	sprintf(cmd, "i2c mw %x %x %d 1",CHIP_ADDR, REG_EN_TST, mode);
	run_command(cmd, 0);

}

static void get_tst(void)
{
	int mode;
	mode = kbi_i2c_read(REG_EN_TST);

	if (mode == 1) {
		printf("enable tst\n");
	} else if (mode == 0) {
		printf("disable tst\n");
	} else {
		printf("tst err: %d\n",mode);
	}
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

static void get_ir(void)
{
	int enable;
	enable = kbi_i2c_read(REG_BOOT_EN_IR);
	printf("boot ir: %s\n", enable==1 ? "enable" : "disable" );
}

static void set_ir(int enable)
{
	char cmd[64];
	sprintf(cmd, "i2c mw %x %x %d 1",CHIP_ADDR, REG_BOOT_EN_IR, enable);
	run_command(cmd, 0);
}

static void get_switch_mac(void)
{
	int mode;
	mode = kbi_i2c_read(REG_MAC_SWITCH);
	printf("switch mac from %d\n", mode);
	setenv("switch_mac", mode==1 ? "1" : "0");
}

static void set_switch_mac(int mode)
{
	char cmd[64];
	sprintf(cmd, "i2c mw %x %x %d 1",CHIP_ADDR, REG_MAC_SWITCH, mode);
	printf("set_switch_mac :%d\n", mode);
	run_command(cmd, 0);
	setenv("switch_mac", mode==1 ? "1" : "0");
}

static void get_key(void)
{
	int enable;
	enable = kbi_i2c_read(REG_BOOT_EN_KEY);
	printf("boot key: %s\n", enable==1 ? "enable" : "disable" );
}
static void set_key(int enable)
{
	char cmd[64];
	sprintf(cmd, "i2c mw %x %x %d 1",CHIP_ADDR, REG_BOOT_EN_KEY, enable);
	run_command(cmd, 0);
}

static int get_forcereset_gpio(bool is_print)
{
	int enable;
	enable = kbi_i2c_read(REG_BOOT_EN_GPIO);
	if (is_print)
	printf("gpio forcereset: %s\n", enable&0x02 ? "enable" : "disable" );
	return enable;
}

static int get_gpio(bool is_print)
{
	int enable;
	enable = kbi_i2c_read(REG_BOOT_EN_GPIO);
	if (is_print)
	printf("boot gpio: %s\n", enable&0x01 ? "enable" : "disable" );
	return enable;
}

static void set_gpio(int enable)
{
	char cmd[64];
	sprintf(cmd, "i2c mw %x %x %d 1",CHIP_ADDR, REG_BOOT_EN_GPIO, enable);
	run_command(cmd, 0);
}

static void get_boot_enable(int type)
{
	if (type == BOOT_EN_WOL)
		get_wol(true);
	else if (type == BOOT_EN_RTC)
		get_rtc();
	else if (type == BOOT_EN_IR)
		get_ir();
	else if (type == BOOT_EN_DCIN)
		get_dcin();
	else if (type == BOOT_EN_KEY)
		get_key();
	else
		get_gpio(true);
}

static void set_boot_enable(int type, int enable)
{
	int state = 0;
	if (type == BOOT_EN_WOL)
	{
		state = get_wol(false);
		set_wol(false, enable|(state&0x02));
	}
	else if (type == BOOT_EN_RTC)
		set_rtc(enable);
	else if (type == BOOT_EN_IR)
		set_ir(enable);
	else if (type == BOOT_EN_DCIN)
		set_dcin(enable);
	else if (type == BOOT_EN_KEY)
		set_key(enable);
	else {
		state = get_gpio(false);
		set_gpio(enable|(state&0x02));
	}
}

static void get_forcereset_enable(int type)
{
	if (type == FORCERESET_GPIO)
		get_forcereset_gpio(true);
	else if (type == FORCERESET_WOL)
		get_forcereset_wol(true);
	else
		printf("get forcereset err=%d\n", type);
}

static int set_forcereset_enable(int type, int enable)
{
	int state = 0;
	if (type == FORCERESET_GPIO)
	{
		state = get_forcereset_gpio(false);
		set_gpio((state&0x01)|(enable<<1));
	}
	else if (type == FORCERESET_WOL)
	{
		state = get_forcereset_wol(false);
		set_wol(false, (state&0x01)|(enable<<1));
	} else {
		printf("set forcereset err=%d\n", type);
		return CMD_RET_USAGE;
	}
	return 0;
}

static void get_blue_led_mode(int type)
{
	int mode;
	if (type == LED_SYSTEM_OFF) {
		mode = kbi_i2c_read(REG_LED_SYSTEM_OFF_MODE);
		if ((mode >= 0) && (mode <=3) )
		printf("led mode: %s  [systemoff]\n",LED_MODE_STR[mode]);
		else
		printf("read led mode err\n");
	}
	else {
		mode = kbi_i2c_read(REG_LED_SYSTEM_ON_MODE);
		if ((mode >= LED_OFF_MODE) && (mode <= LED_HEARTBEAT_MODE))
		printf("led mode: %s  [systemon]\n",LED_MODE_STR[mode]);
		else
		printf("read led mode err\n");
	}
}

static int set_blue_led_mode(int type, int mode)
{
	char cmd[64];
	if (type == LED_SYSTEM_OFF) {
		sprintf(cmd, "i2c mw %x %x %d 1",CHIP_ADDR, REG_LED_SYSTEM_OFF_MODE, mode);
	} else if (type == LED_SYSTEM_ON) {
		sprintf(cmd, "i2c mw %x %x %d 1",CHIP_ADDR, REG_LED_SYSTEM_ON_MODE, mode);
	} else {
		return CMD_RET_USAGE;
	}

	run_command(cmd, 0);
    return 0;
}

static int do_kbi_init(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	int enable = get_wol(false);
	if ((enable&0x01) != 0)
		set_wol(false, enable);
	return 0;
}

static int do_kbi_resetflag(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 2)
		return CMD_RET_USAGE;

	if (strcmp(argv[1], "1") == 0) {
		run_command("i2c mw 0x18 0x87 1 1", 0);
	} else if (strcmp(argv[1], "0") == 0) {
		run_command("i2c mw 0x18 0x87 0 1", 0);
	} else {
		return CMD_RET_USAGE;
	}
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

static int do_kbi_adc(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	get_adc();
	return 0;
}

static int do_kbi_powerstate(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	get_power_state();
	return 0;

}

static int do_kbi_ethmac(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	get_mac();
	return 0;
}

static int do_kbi_boarddetect(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	detect_board();
	return 0;
}

static int do_kbi_switchmac(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{

	if (argc < 2)
		return CMD_RET_USAGE;

	if (strcmp(argv[1], "w") == 0) {
		if (argc < 3)
			return CMD_RET_USAGE;

		if (strcmp(argv[2], "0") == 0) {
			set_switch_mac(0);
		} else if (strcmp(argv[2], "1") == 0) {
			set_switch_mac(1);
		} else {
			return CMD_RET_USAGE;
		}
	} else if (strcmp(argv[1], "r") == 0) {
		get_switch_mac();
	} else {
		return CMD_RET_USAGE;
	}
	return 0;
}

static int do_kbi_led(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	if (argc < 3)
		return CMD_RET_USAGE;

	if (strcmp(argv[1], "systemoff") ==0) {
		if (strcmp(argv[2], "r") == 0) {
			get_blue_led_mode(LED_SYSTEM_OFF);
		} else if (strcmp(argv[2], "w") == 0) {
			if (argc < 4)
				return CMD_RET_USAGE;
			if (strcmp(argv[3], "breathe") == 0) {
				ret = set_blue_led_mode(LED_SYSTEM_OFF, LED_BREATHE_MODE);
			} else if (strcmp(argv[3], "heartbeat") == 0) {
				ret = set_blue_led_mode(LED_SYSTEM_OFF, LED_HEARTBEAT_MODE);
			} else if (strcmp(argv[3], "on") == 0) {
				ret = set_blue_led_mode(LED_SYSTEM_OFF, LED_ON_MODE);
			} else if (strcmp(argv[3], "off") == 0) {
				ret = set_blue_led_mode(LED_SYSTEM_OFF, LED_OFF_MODE);
			} else {
				ret =  CMD_RET_USAGE;
			}
		}
	} else if (strcmp(argv[1], "systemon") ==0) {

		if (strcmp(argv[2], "r") == 0) {
			get_blue_led_mode(LED_SYSTEM_ON);
		} else if (strcmp(argv[2], "w") == 0) {
			if (argc <4)
				return CMD_RET_USAGE;
			if (strcmp(argv[3], "breathe") == 0) {
				ret = set_blue_led_mode(LED_SYSTEM_ON, LED_BREATHE_MODE);
			} else if (strcmp(argv[3], "heartbeat") == 0) {
				ret = set_blue_led_mode(LED_SYSTEM_ON, LED_HEARTBEAT_MODE);
			} else if (strcmp(argv[3], "on") == 0) {
				ret = set_blue_led_mode(LED_SYSTEM_ON, LED_ON_MODE);
			} else if (strcmp(argv[3], "off") == 0) {
				ret = set_blue_led_mode(LED_SYSTEM_ON, LED_OFF_MODE);
			} else {
				ret =  CMD_RET_USAGE;
			}
		}

	} else {
		return CMD_RET_USAGE;
	}
	return ret;
}

static int do_kbi_forcereset(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	if (argc < 3)
		return CMD_RET_USAGE;

	if (strcmp(argv[1], "wol") ==0) {
		if (strcmp(argv[2], "r") == 0) {
			get_forcereset_enable(FORCERESET_WOL);
		} else if (strcmp(argv[2], "w") == 0) {
			if (argc < 4)
				return CMD_RET_USAGE;
			if (strcmp(argv[3], "1") == 0) {
				ret = set_forcereset_enable(FORCERESET_WOL, 1);
			} else if (strcmp(argv[3], "0") == 0) {
				ret = set_forcereset_enable(FORCERESET_WOL, 0);
			} else {
				ret =  CMD_RET_USAGE;
			}
		}
	} else if (strcmp(argv[1], "gpio") ==0) {

		if (strcmp(argv[2], "r") == 0) {
			get_forcereset_enable(FORCERESET_GPIO);
		} else if (strcmp(argv[2], "w") == 0) {
			if (argc <4)
				return CMD_RET_USAGE;
			if (strcmp(argv[3], "1") == 0) {
				ret = set_forcereset_enable(FORCERESET_GPIO, 1);
			} else if (strcmp(argv[3], "0") == 0) {
				ret = set_forcereset_enable(FORCERESET_GPIO, 0);
			} else {
				ret =  CMD_RET_USAGE;
			}
		}

	} else {
		return CMD_RET_USAGE;
	}
	return ret;
}

static int do_kbi_poweroff(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	printf("System poweroff\n");
	int enable = get_wol(false);
	if ((enable&0x03) != 0)
		set_wol(true, enable);
	set_poweroff();
	return 0;
}

static int do_kbi_trigger(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 3)
		return CMD_RET_USAGE;

	if (strcmp(argv[2], "r") == 0) {

		if (strcmp(argv[1], "wol") == 0)
			get_boot_enable(BOOT_EN_WOL);
		else if (strcmp(argv[1], "rtc") == 0)
			get_boot_enable(BOOT_EN_RTC);
		else if (strcmp(argv[1], "ir") == 0)
			get_boot_enable(BOOT_EN_IR);
		else if (strcmp(argv[1], "dcin") == 0)
			get_boot_enable(BOOT_EN_DCIN);
		//else if (strcmp(argv[1], "key") == 0)
			//get_boot_enable(BOOT_EN_KEY);
		else if (strcmp(argv[1], "gpio") == 0)
		    get_boot_enable(BOOT_EN_GPIO);
		else
			return CMD_RET_USAGE;
	} else if (strcmp(argv[2], "w") == 0) {
		if (argc < 4)
			return CMD_RET_USAGE;
		if ((strcmp(argv[3], "1") != 0) && (strcmp(argv[3], "0") != 0))
			return CMD_RET_USAGE;

		if (strcmp(argv[1], "wol") == 0) {

			if (strcmp(argv[3], "1") == 0)
				set_boot_enable(BOOT_EN_WOL, 1);
			else
				set_boot_enable(BOOT_EN_WOL, 0);

	    } else if (strcmp(argv[1], "rtc") == 0) {

			if (strcmp(argv[3], "1") == 0)
				set_boot_enable(BOOT_EN_RTC, 1);
			else
				set_boot_enable(BOOT_EN_RTC, 0);

		} else if (strcmp(argv[1], "ir") == 0) {

			if (strcmp(argv[3], "1") == 0)
				set_boot_enable(BOOT_EN_IR, 1);
			else
				set_boot_enable(BOOT_EN_IR, 0);

		} else if (strcmp(argv[1], "dcin") == 0) {

			if (strcmp(argv[3], "1") == 0)
				set_boot_enable(BOOT_EN_DCIN, 1);
			else
				set_boot_enable(BOOT_EN_DCIN, 0);

		/*} else if (strcmp(argv[1], "key") == 0) {

			if (strcmp(argv[3], "1") == 0)
				set_boot_enable(BOOT_EN_KEY, 1);
			else
			set_boot_enable(BOOT_EN_KEY, 0);*/

		} else if (strcmp(argv[1], "gpio") == 0) {

			if (strcmp(argv[3], "1") == 0)
				set_boot_enable(BOOT_EN_GPIO, 1);
			else
				set_boot_enable(BOOT_EN_GPIO, 0);

		} else {
			return CMD_RET_USAGE;

		}
	} else {

		return CMD_RET_USAGE;
	}

	return 0;
}


static int do_kbi_bootmode(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 2)
		return CMD_RET_USAGE;
	if (strcmp(argv[1], "w") == 0) {
		if (argc < 3)
			return CMD_RET_USAGE;
		if (strcmp(argv[2], "emmc") == 0) {
			set_bootmode(BOOT_MODE_EMMC);
		} else if (strcmp(argv[2], "spi") == 0) {
			set_bootmode(BOOT_MODE_SPI);
		} else {
			return CMD_RET_USAGE;
		}
	} else if (strcmp(argv[1], "r") == 0) {

		get_bootmode();

	} else {
		return CMD_RET_USAGE;
	}

	return 0;
}

static int do_kbi_tst(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 2)
		return CMD_RET_USAGE;
	if (strcmp(argv[1], "w") == 0) {
		if (argc < 3)
			return CMD_RET_USAGE;
		if (strcmp(argv[2], "on") == 0) {
			set_tst(1);
		} else if (strcmp(argv[2], "off") == 0) {
			set_tst(0);
		} else {
			return CMD_RET_USAGE;
		}
	} else if (strcmp(argv[1], "r") == 0) {

		get_tst();

	} else {
		return CMD_RET_USAGE;
	}

	return 0;
}


static void get_ir_ircode1(void)
{
	char usercode[3] = {};

	kbi_i2c_read_block(REG_IR_CODE1, 3, usercode);
	printf("usercode1:%02x%02x , power key code1:%02x\n",usercode[0],usercode[1],usercode[2]);
}

static void set_ir_ircode1(int usercodeH, int usercodeL,int powerkeycode)
{
	char cmd[64];
	sprintf(cmd, "i2c mw %x %x %02x 1",CHIP_ADDR, REG_IR_CODE1, usercodeH);
	run_command(cmd, 0);
	sprintf(cmd, "i2c mw %x %x %02x 1",CHIP_ADDR, REG_IR_CODE1+1, usercodeL);
	run_command(cmd, 0);
	sprintf(cmd, "i2c mw %x %x %02x 1",CHIP_ADDR, REG_IR_CODE1+2, powerkeycode);
	run_command(cmd, 0);
}

static int do_kbi_ircode1(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	char cmd[8];
	int usercodeH;
	int usercodeL;
	int powerkeycode;
	int i;
	if (argc < 2){
		return CMD_RET_USAGE;
	}
	if (strcmp(argv[1], "w") == 0) {
		if (argc < 3){
			printf("Error in command format (kbi ircode1 w xxxx:xx)\n");
			return CMD_RET_USAGE;
		}
		if (strstr(argv[2], ":") != NULL && sizeof(argv[2])==8) {
			memcpy(cmd,argv[2],sizeof(argv[2]));
			//for(i=0;i<sizeof(argv[2]);i++)
				//printf("cmd[%d]=%c\n",i,cmd[i]);

			usercodeH = chartonum(cmd[0])<<4|chartonum(cmd[1]);
			usercodeL = chartonum(cmd[2])<<4|chartonum(cmd[3]);
			powerkeycode = chartonum(cmd[5])<<4|chartonum(cmd[6]);
			//printf("usercodeH[%02x] usercodeL[%02x] powerkeycode[%02x]\n",usercodeH,usercodeL,powerkeycode);
			set_ir_ircode1(usercodeH,usercodeL,powerkeycode);
		} else {
			printf("Error in command format (xxxx:xx)\n");
			return CMD_RET_USAGE;
		}
	}
	else if (strcmp(argv[1], "r") == 0) {
		get_ir_ircode1();
	} 
	else {
		return CMD_RET_USAGE;
	}

	return 0;
}

static void get_ir_ircode2(void)
{
	char usercode[3] = {};

	kbi_i2c_read_block(REG_IR_CODE2, 3, usercode);
	printf("usercode2:%02x%02x , power key code2:%02x\n",usercode[0],usercode[1],usercode[2]);
}

static void set_ir_ircode2(int usercodeH, int usercodeL,int powerkeycode)
{
	char cmd[64];
	sprintf(cmd, "i2c mw %x %x %02x 1",CHIP_ADDR, REG_IR_CODE2, usercodeH);
	run_command(cmd, 0);
	sprintf(cmd, "i2c mw %x %x %02x 1",CHIP_ADDR, REG_IR_CODE2+1, usercodeL);
	run_command(cmd, 0);
	sprintf(cmd, "i2c mw %x %x %02x 1",CHIP_ADDR, REG_IR_CODE2+2, powerkeycode);
	run_command(cmd, 0);
}

static int do_kbi_ircode2(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	char cmd[8];
	int usercodeH;
	int usercodeL;
	int powerkeycode;
	int i;
	if (argc < 2){
		return CMD_RET_USAGE;
	}
	if (strcmp(argv[1], "w") == 0) {
		if (argc < 3){
			printf("Error in command format (kbi ircode2 w xxxx:xx)\n");
			return CMD_RET_USAGE;
		}
		if (strstr(argv[2], ":") != NULL && sizeof(argv[2])==8) {
			memcpy(cmd,argv[2],sizeof(argv[2]));
			//for(i=0;i<sizeof(argv[2]);i++)
				//printf("cmd[%d]=%c\n",i,cmd[i]);

			usercodeH = chartonum(cmd[0])<<4|chartonum(cmd[1]);
			usercodeL = chartonum(cmd[2])<<4|chartonum(cmd[3]);
			powerkeycode = chartonum(cmd[5])<<4|chartonum(cmd[6]);
			//printf("usercodeH[%02x] usercodeL[%02x] powerkeycode[%02x]\n",usercodeH,usercodeL,powerkeycode);
			set_ir_ircode2(usercodeH,usercodeL,powerkeycode);
		} else {
			printf("Error in command format (xxxx:xx)\n");
			return CMD_RET_USAGE;
		}
	}
	else if (strcmp(argv[1], "r") == 0) {
		get_ir_ircode2();
	} 
	else {
		return CMD_RET_USAGE;
	}

	return 0;
}

static cmd_tbl_t cmd_kbi_sub[] = {
	U_BOOT_CMD_MKENT(init, 1, 1, do_kbi_init, "", ""),
	U_BOOT_CMD_MKENT(resetflag, 2, 1, do_kbi_resetflag, "", ""),
	U_BOOT_CMD_MKENT(usid, 1, 1, do_kbi_usid, "", ""),
	U_BOOT_CMD_MKENT(version, 1, 1, do_kbi_version, "", ""),
	U_BOOT_CMD_MKENT(adc, 1, 1, do_kbi_adc, "", ""),
	U_BOOT_CMD_MKENT(powerstate, 1, 1, do_kbi_powerstate, "", ""),
	U_BOOT_CMD_MKENT(ethmac, 1, 1, do_kbi_ethmac, "", ""),
	U_BOOT_CMD_MKENT(boarddetect, 1, 1, do_kbi_boarddetect, "", ""),
	U_BOOT_CMD_MKENT(poweroff, 1, 1, do_kbi_poweroff, "", ""),
	U_BOOT_CMD_MKENT(switchmac, 3, 1, do_kbi_switchmac, "", ""),
	U_BOOT_CMD_MKENT(led, 4, 1, do_kbi_led, "", ""),
	U_BOOT_CMD_MKENT(trigger, 4, 1, do_kbi_trigger, "", ""),
	U_BOOT_CMD_MKENT(bootmode, 3, 1, do_kbi_bootmode, "", ""),
	U_BOOT_CMD_MKENT(tst, 3, 1, do_kbi_tst, "", ""),
	U_BOOT_CMD_MKENT(ircode1, 3, 1, do_kbi_ircode1, "", ""),
	U_BOOT_CMD_MKENT(ircode2, 3, 1, do_kbi_ircode2, "", ""),
	U_BOOT_CMD_MKENT(forcereset, 4, 1, do_kbi_forcereset, "", ""),
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
//		"kbi adc - read adc value\n"
		"kbi powerstate - read power on state\n"
		"kbi poweroff - power off device\n"
		"kbi ethmac - read ethernet mac address\n"
		"kbi boarddetect - detect board type\n"
		"\n"
		"kbi led [systemoff|systemon] w <off|on|breathe|heartbeat> - set blue led mode\n"
		"kbi led [systemoff|systemon] r - read blue led mode\n"
		"\n"
		"kbi forcereset [wol|gpio] w <0|1> - disable/enable force-reset\n"
		"kbi forcereset [wol|gpio] r - read state of force-reset\n"
		"[notice: the wol|gpio boot trigger must be enabled if you want to enable force-reset]\n"
		"\n"
		"kbi bootmode w <emmc|spi> - set bootmode to emmc or spi\n"
		"kbi bootmode r - read current bootmode\n"
		"kbi ircode1 w <usercode:powerkeycode> - set ir power key code1 as: kbi ircode1 w ff00:eb\n"
		"kbi ircode1 r - read current ir power key code1\n"		
		"kbi ircode2 w <usercode:powerkeycode> - set ir power key code2 as: kbi ircode2 w ff00:eb\n"
		"kbi ircode2 r - read current ir power key code2\n" 	
		"\n"
		"kbi tst w <on|off> - disable/enable tst\n"
		"kbi tst r - read mode of tst\n"		
		"kbi trigger [wol|rtc|ir|dcin|gpio] w <0|1> - disable/enable boot trigger\n"
		"kbi trigger [wol|rtc|ir|dcin|gpio] r - read mode of a boot trigger";


U_BOOT_CMD(
		kbi, 6, 1, do_kbi,
		"Khadas Bootloader Instructions sub-system",
		kbi_help_text
);
