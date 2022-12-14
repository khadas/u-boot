// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <asm/arch/bl31_apis.h>
#include <asm/arch/secure_apb.h>
#include <ramdump.h>
#include <emmc_partitions.h>
#include <usb.h>
#include <asm/arch/regs.h>

#define DEBUG_RAMDUMP	0
#define AMLOGIC_KERNEL_PANIC		0x0c
#define AMLOGIC_WATCHDOG_REBOOT		0x0d


unsigned long ramdump_base = 0;
unsigned long ramdump_size = 0;
unsigned int get_reboot_mode(void)
{
	uint32_t reboot_mode_val = ((readl(AO_SEC_SD_CFG15) >> 12) & 0xf);
	return reboot_mode_val;
}

void ramdump_init(void)
{
	unsigned int data;

	ramdump_base = readl(P_AO_SEC_GP_CFG12);
	ramdump_size = readl(P_AO_SEC_GP_CFG13);
	if (ramdump_base & 0x80) {
		/* 0x80: The flag indicates that the addr exceeds 4G. */
		/* real size = size[31:0] + addr[6:0]<<32 */
		ramdump_size += (ramdump_base & 0x7f) << 32;
		/* real addr = addr[31:8] << 8 */
		ramdump_base = (ramdump_base & 0xffffff00) << 8;
	}

	data = readl(PREG_STICKY_REG8);
	writel(data & ~RAMDUMP_STICKY_DATA_MASK, PREG_STICKY_REG8);
	printf("%s, add:%lx, size:%lx\n", __func__, ramdump_base, ramdump_size);

}

static void wait_usb_dev(void)
{
	int print_cnt = 0, ret;

	while (1) {
		run_command("usb start", 1);
		mdelay(2000);
		run_command("usb reset", 1);
		ret = usb_stor_scan(1);
		if (ret) {
			if (!(print_cnt & 0x3f)) {
				print_cnt++;
				printf("ramdump: can't find usb device, please insert Udisk.\n\n");
			}
			mdelay(10000);
			continue;
		} else {
			break;
		}
	}
}

/*
 * NOTE: this is a default implementation for writing compressed ramdump data
 * to /data/ partition for Android platform. You can read out dumpfile in
 * path /data/crashdump-1.bin when enter Android for crash analyze.
 * by default, /data/ partion for android is EXT4 fs.
 *
 * TODO:
 *    If you are using different fs or OS on your platform, implement compress
 *    data save command for your fs and OS in your board.c with same function
 *    name "ramdump_save_compress_data".
 */
__weak int ramdump_save_compress_data(void)
{
	char cmd[128] = {0};
	char *env;

	env = env_get("ramdump_location");
	if (!env)
		return 0;

	printf("ramdump_location:%s\n", env);
	/* currently we only support write to usb disk */
	if (strncmp(env, "usb", 3)) {
		printf("not supported location\n");
		return 0;
	}

	wait_usb_dev();

	sprintf(cmd, "fatwrite usb 0 %lx crashdump-1.bin %lx\n",
		ramdump_base, ramdump_size);
	printf("CMD:%s\n", cmd);
	run_command(cmd, 1);
	printf("run fatwrite usb ok, reboot!\n");
	mdelay(10000);
	run_command("reset", 1);
	return 0;
}

static void ramdump_env_setup(unsigned long addr, unsigned long size)
{
	unsigned int data[10] = {
		0x8E9C929F, 0x9E9C9791,
		0xD28C9191, 0x97949B8D,
		0x888B92,   0xCEBB97,
		0x938E9B90, 0x978D8D97,
		0xC8009B8A, 0xB99CDB
	};
	char *line, *p1, *p2, *o;
	unsigned char *p;
	int i;

	p = (unsigned char *)data;
	for (i = 0; i < 40; i++)
		p[i] = ~(p[i] - 1);

	/*
	 * TODO: Make sure address for fdt_high and initrd_high
	 * are suitable for all boards
	 *
	 * usually kernel load address is 0x010800000
	 * Make sure:
	 * (kernel image size + ramdisk size) <
	 * (initrd_high - 0x010800000)
	 * dts file size < (fdt_high - initrd_high)
	 */
	//env_set("initrd_high", "0x04400000");
	//env_set("fdt_high",    "0x04E00000");
	line = env_get("bootargs");
	if (!line)
		return;

	i = strlen(line);
	o = malloc(i + 128);
	if (!o)
		return;

	memset(o, 0, i + 128);
	memcpy(o, line, i);
	line = o + i + 128;
	p1 = strstr(o, (const char *)p);
	if (p1) {
		p2 = strchr(p1, ' ');
		if (!p2)
			p2 = p1;
		memmove(p1, p2, line - p2);
	}
	i = strlen(o);
	p1 = o + i;
	p1[0] = ' ';
	sprintf(p1 + 1, "%s=%s ramdump=%lx,%lx",
		(char *)data, (char *)(data + 6), addr, size);
	env_set("bootargs", o);

#if DEBUG_RAMDUMP
	run_command("printenv bootargs", 1);
#endif
	free(o);
}

void check_ramdump(void)
{
	unsigned long size = 0;
	unsigned long addr = 0;
	char *env;
	int reboot_mode;

	env = env_get("ramdump_enable");
	printf("%s, ramdump_enable = %s\n", __func__, env);
	if (env) {
		if (!strcmp(env, "1")) {
			reboot_mode = get_reboot_mode();
			if ((reboot_mode == AMLOGIC_WATCHDOG_REBOOT ||
			     reboot_mode == AMLOGIC_KERNEL_PANIC)) {
				addr = ramdump_base;
				size = ramdump_size;
				printf("%s, addr:%lx, size:%lx\n",
					__func__, addr, size);
				if (addr && size) {
					ramdump_env_setup(addr, size);
					ramdump_save_compress_data();
				}
			} else {
				ramdump_env_setup(0, 0);
			}
			ramdump_env_setup(addr, size);
		}
	}
#ifdef CONFIG_SUPPORT_BL33Z
	printf("%s, fdt: rsvmem ramdump_bl33z enable.\n", __func__);
	//run_command("fdt rm /reserved-memory/ramdump_bl33z no-map", 0);
	run_command("fdt set /reserved-memory/ramdump_bl33z status okay", 0);
#endif
}
