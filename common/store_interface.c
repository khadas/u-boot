/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/store_interface.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <config.h>
#include <common.h>
#include <command.h>
//#include <watchdog.h>
#include <malloc.h>
#if defined(CONFIG_AML_NAND) || defined (CONFIG_AML_MTD)
#include <nand.h>
#endif
#include <mmc.h>
#include <linux/ctype.h>
#include <asm/byteorder.h>
#include <div64.h>
#include <linux/err.h>
#include <partition_table.h>
#include <emmc_partitions.h>
#include <libfdt.h>
#include <linux/string.h>
#include <asm/cpu_id.h>
#include <asm/arch/bl31_apis.h>
#include <asm/arch/cpu_config.h>
#include <asm/arch/secure_apb.h>

extern int amlmmc_check_and_update_boot_info(void);
#if defined(CONFIG_AML_NAND) || defined (CONFIG_AML_MTD)
extern int amlnf_init(unsigned int flag);
extern void nand_init(void);
//extern int amlnf_key_write(u8 *buf, int len, u32 *actual_length);
//extern int amlnf_key_read(u8 *buf, int len, u32 *actual_length);
extern int amlnf_ddr_parameter_read(u8 *buf, int len);
extern int amlnf_ddr_parameter_write(u8 *buf, int len);
extern int amlnf_ddr_parameter_erase(void);
#endif

#if defined(CONFIG_DISCRETE_BOOTLOADER)
#ifndef CONFIG_TPL_VAL_NUM_MIN
#define CONFIG_TPL_VAL_NUM_MIN  (CONFIG_TPL_COPY_NUM/2)
#endif// #ifndef CONFIG_TPL_VAL_NUM_MIN
#ifndef CONFIG_BL2_VAL_NUM_MIN
#define CONFIG_BL2_VAL_NUM_MIN  (CONFIG_BL2_COPY_NUM/2)
#endif// #ifndef CONFIG_BL2_VAL_NUM_MIN
static u32 _bootloaderOrgCrc[2];  //0 for bl2, 1 for tpl
#endif// #if defined(CONFIG_DISCRETE_BOOTLOADER)

extern int get_partition_from_dts(unsigned char *buffer);

/* key operations of emmc */
extern int mmc_key_read(unsigned char *buf,
		unsigned int size, u32 *actual_length);
extern int mmc_key_write(unsigned char *buf,
		unsigned int size, u32 *actual_length);
extern int mmc_key_erase(void);
extern int find_dev_num_by_partition_name (char *name);
//extern unsigned int emmc_cur_partition;

extern int mmc_ddr_parameter_read(unsigned char *buf, unsigned int size);
extern int mmc_ddr_parameter_write(unsigned char *buf, unsigned int size);
extern int mmc_ddr_parameter_erase(void);

#define debugP(fmt...) //printf("Dbg[store]L%d:", __LINE__),printf(fmt)
#define MsgP(fmt...)   printf("[store]"fmt)
#define ErrP(fmt...)   printf("[store]Err:%s,L%d:", __func__, __LINE__),printf(fmt)

#define NAND_INIT_FAILED 20
#define STORE_BOOT_NORMAL				0
#define STORE_BOOT_UPGRATE				1
#define STORE_BOOT_ERASE_PROTECT_CACHE	2
#define STORE_BOOT_ERASE_ALL			3
#define STORE_BOOT_SCRUB_ALL			4

#define _SPI_FLASH_ERASE_SZ      (CONFIG_ENV_IN_SPI_OFFSET + CONFIG_ENV_SIZE)
#define CONFIG_ENV_IN_SPI_OFFSET 0
//Ignore mbr since mmc driver already handled
//#define MMC_UBOOT_CLEAR_MBR

#ifdef MMC_UBOOT_CLEAR_MBR
static char _mbrFlag[4] ;
#endif

#ifdef CONFIG_AML_MTD
static int mtd_find_phy_off_by_lgc_off(const char *partname,
		const loff_t logicaddr, loff_t *phyaddr)
{
	nand_info_t *mtdpartinf = NULL;
	loff_t off = 0;
	static struct {
		loff_t lastblkphyoff;
		loff_t lastblklgcoff;
		char   partname[64];
	} _map4speedup = {0};
	int canspeedup = 0;

	if (!(NAND_BOOT_FLAG == device_boot_flag || SPI_NAND_FLAG == device_boot_flag)) {
		return 0;
	}

	mtdpartinf = get_mtd_device_nm(partname);
	if (IS_ERR(mtdpartinf)) {
		ErrP("device(%s) is err\n", partname);
		return CMD_RET_FAILURE;
	}
	const unsigned int erasesz = mtdpartinf->erasesize;
	const unsigned int offsetinblk = logicaddr & (erasesz - 1);

	if (!strcmp(partname, _map4speedup.partname) && logicaddr >= _map4speedup.lastblklgcoff) {
		canspeedup = 1;
	} else {
		_map4speedup.lastblklgcoff = 0;
		_map4speedup.lastblkphyoff = 0;
		strncpy(_map4speedup.partname, partname, 63);
	}

	if (canspeedup) {
		if (logicaddr >= _map4speedup.lastblklgcoff &&
				logicaddr < _map4speedup.lastblklgcoff + erasesz) {
			*phyaddr = _map4speedup.lastblkphyoff + offsetinblk;
			return 0;
		}
		_map4speedup.lastblkphyoff += erasesz;
		_map4speedup.lastblklgcoff += erasesz;
		off = _map4speedup.lastblkphyoff;
	}
	for (; off < mtdpartinf->size;
		off += erasesz, _map4speedup.lastblkphyoff += erasesz) {
		if (nand_block_isbad(mtdpartinf, off)) {
			MsgP("  %08llx\n", (unsigned long long)off);
		} else {
			if (logicaddr >= _map4speedup.lastblklgcoff &&
					logicaddr < _map4speedup.lastblklgcoff + erasesz) {
				*phyaddr = _map4speedup.lastblkphyoff + offsetinblk;
				return 0;
			}
			_map4speedup.lastblklgcoff += erasesz;
		}
	}

	return __LINE__;
}
#endif// #ifdef CONFIG_AML_MTD


/* mmcinfo 1 will clear info_disprotect before run_command("mmc erase 1") */
static int _info_disprotect_back_before_mmcinfo1 = 0;
extern int info_disprotect;
/*
static inline int isstring(char *p)
{
	char *endptr = p;
	while (*endptr != '\0') {
		if (!(((*endptr >= '0') && (*endptr <= '9'))
			|| ((*endptr >= 'a') && (*endptr <= 'f'))
			|| ((*endptr >= 'A') && (*endptr <= 'F'))
			|| (*endptr == 'x') || (*endptr == 'X')))
			return 1;
		endptr++;
	}

	return 0;
}
*/
/*
static inline int str2long(char *p, ulong *num)
{
	char *endptr;
	*num = simple_strtoul(p, &endptr, 16);
	return (*p != '\0' && *endptr == '\0') ? 1 : 0;
}
*/

static inline int str2longlong(char *p, unsigned long long *num)
{
	char *endptr;

	*num = simple_strtoull(p, &endptr, 16);
	if (*endptr != '\0')
	{
	    switch (*endptr)
	    {
	        case 'g':
	        case 'G':
	            *num<<=30;
		    endptr++;
		    break;
	        case 'm':
	        case 'M':
	            *num<<=20;
		    endptr++;
		    break;
	        case 'k':
	        case 'K':
	            *num<<=10;
	            endptr++;
	            break;
	    }
	}

	return (*p != '\0' && *endptr == '\0') ? 1 : 0;
}

static int emmc_init(void)
{
	int ret = -1;
	struct mmc *mmc = NULL;
	mmc = find_mmc_device(CONFIG_SYS_MMC_ENV_DEV);
	if (mmc) {
		ret = mmc_init(mmc); // init eMMC/tSD+
	}
	return ret;
}

static int get_device_boot_flag(void)
{
	int ret=0;
	if (1) {//nand and emmc
		//try eMMC init
		device_boot_flag = EMMC_BOOT_FLAG;
		ret = emmc_init();
		if (!ret) {
			printf("XXXXXXX======enter EMMC boot======XXXXXX\n");
			return 0;
		}
		printf("EMMC init failed\n");

#if defined(CONFIG_AML_NAND) || defined(CONFIG_AML_MTD)
		//try nand first
	#if defined(CONFIG_AML_NAND)
		ret = amlnf_init(0x5);
	#elif defined(CONFIG_AML_MTD)
		nand_init();
	#endif
		ret = (device_boot_flag == NAND_BOOT_FLAG) ? 0 : __LINE__;
		if (!ret) {
			printf("XXXXXXX======enter NAND boot======XXXXXX\n");
			return 0;
		}
		printf("NAND init failed\n");
#else
		printf("check again, may error code used!\n");
#endif
	}

	printf("device_boot_flag=%d\n",device_boot_flag);
	return -1;
}

static int get_off_size(int argc, char *argv[],  loff_t *off, loff_t *size)
{
		if (argc >= 1) {
			if (!(str2longlong(argv[0], (unsigned long long*)off))) {
				store_msg("'%s' is not a number\n", argv[0]);
				return -1;
			}
		} else {
				*off = 0;
				*size = 0;
		}

		if (argc >= 2) {
			if (!(str2longlong(argv[1], (unsigned long long *)size))) {
				store_msg("'%s' is not a number\n", argv[1]);
				return -1;
			}
		} else {
			*size = 0;
		}

		store_dbg("offset 0x%llx, size 0x%llx", *off, *size);

		return 0;
}

static int do_decrypt_dtb(char *dtbaddr) {
	int ret = 0;
	unsigned long dtimgaddr = simple_strtoul(dtbaddr, NULL, 16);

	ret = fdt_check_header((char *)dtimgaddr);
	if (!ret) {
		MsgP("Is good fdt check header, no need decrypt!\n");
		return ret;
	}

#ifndef CONFIG_SKIP_KERNEL_DTB_SECBOOT_CHECK
	flush_cache(dtimgaddr, AML_DTB_IMG_MAX_SZ);
	ret = aml_sec_boot_check(AML_D_P_IMG_DECRYPT, dtimgaddr, AML_DTB_IMG_MAX_SZ, 0);
	if (ret) {
		MsgP("decrypt dtb: Sig Check %d\n",ret);
		return ret;
	}

	ulong nCheckOffset;
	nCheckOffset = aml_sec_boot_check(AML_D_Q_IMG_SIG_HDR_SIZE,
			GXB_IMG_LOAD_ADDR, GXB_EFUSE_PATTERN_SIZE, GXB_IMG_DEC_ALL);
	if (AML_D_Q_IMG_SIG_HDR_SIZE == (nCheckOffset & 0xFFFF))
		nCheckOffset = (nCheckOffset >> 16) & 0xFFFF;
	else
		nCheckOffset = 0;

	if (nCheckOffset)
		memmove((char *)dtimgaddr,
				(char *)dtimgaddr + nCheckOffset, AML_DTB_IMG_MAX_SZ);
#endif//#ifndef CONFIG_SKIP_KERNEL_DTB_SECBOOT_CHECK

#ifdef CONFIG_MULTI_DTB
	extern unsigned long get_multi_dt_entry(unsigned long fdt_addr);

	unsigned long fdtaddr = get_multi_dt_entry(dtimgaddr);

	ret = fdt_check_header((char *)fdtaddr);
	if (ret) {
		ErrP("Fail in fdt check header\n");
		return CMD_RET_FAILURE;
	}

	unsigned int fdtsz = fdt_totalsize((char *)fdtaddr);

	if (fdtsz > AML_DTB_IMG_MAX_SZ)
		return CMD_RET_FAILURE;

	memmove((void *)dtimgaddr, (void *)fdtaddr, fdtsz);
#endif// #ifdef CONFIG_MULTI_DTB

	return ret;
}

//store dtb read/write buff size
static int do_store_dtb_ops(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
		int ret = 0;
		char _cmdbuf[128];
		char *ops = argv[2];
		const unsigned int maxdtbsz = 256 * 1024;
		unsigned int actualdtbsz = 0;
		char *devcmd = NULL;
		char *dtbloadaddr = argv[3];

		if (argc < 4)
			return CMD_RET_USAGE;

		const int is_decrypt = !strcmp("decrypt", ops);
		if (is_decrypt) {
			return do_decrypt_dtb(dtbloadaddr);
		}

		const int is_write = !strcmp("write", ops);
		if (!is_write) {
				ret = !strcmp("read", ops) || !strcmp("iread", ops);//must be 0
				if (!ret)
					return CMD_RET_USAGE;
		}

		actualdtbsz = maxdtbsz;
		if (argc > 4) {
			const unsigned int bufsz = simple_strtoul(argv[4], NULL, 0);

			if (bufsz > maxdtbsz) {
				ErrP("bufsz (%s) > max 0x%x\n", argv[4], maxdtbsz);
				return CMD_RET_FAILURE;
			}
		}

		ops = is_write ? "dtb_write" : "dtb_read";

		switch (device_boot_flag)
		{
				case NAND_BOOT_FLAG:
				case SPI_NAND_FLAG:
						{
								devcmd = "amlnf";
						}
						break;

				case EMMC_BOOT_FLAG:
				case SPI_EMMC_FLAG:
						{
								devcmd = "emmc";
						}
						break;

				default:
						ErrP("device_boot_flag=0x%x err\n", device_boot_flag);
						return CMD_RET_FAILURE;
		}

		sprintf(_cmdbuf, "%s %s %s 0x%x", devcmd, ops, dtbloadaddr, actualdtbsz);
		MsgP("To run cmd[%s]\n", _cmdbuf);
		ret = run_command(_cmdbuf, 0);

		unsigned long dtimgaddr = simple_strtoul(dtbloadaddr, NULL, 16);
		//
		//ONLY need decrypting when 'store dtb read'
	   if (!strcmp("read", argv[2]))
	   {
		   flush_cache(dtimgaddr, AML_DTB_IMG_MAX_SZ);
#ifndef CONFIG_SKIP_KERNEL_DTB_SECBOOT_CHECK

		   ret = aml_sec_boot_check(AML_D_P_IMG_DECRYPT, dtimgaddr, AML_DTB_IMG_MAX_SZ, 0);
		   if (ret) {
			   MsgP("decrypt dtb: Sig Check %d\n",ret);
			   return ret;
		   }
#endif
	   }

#ifndef CONFIG_SKIP_KERNEL_DTB_SECBOOT_CHECK
	   if (!is_write && strcmp("iread", argv[2]))
	   {
			ulong nCheckOffset;

			nCheckOffset = aml_sec_boot_check(AML_D_Q_IMG_SIG_HDR_SIZE,
					GXB_IMG_LOAD_ADDR, GXB_EFUSE_PATTERN_SIZE, GXB_IMG_DEC_ALL);
			if (AML_D_Q_IMG_SIG_HDR_SIZE == (nCheckOffset & 0xFFFF))
				nCheckOffset = (nCheckOffset >> 16) & 0xFFFF;
			else
				nCheckOffset = 0;

			if (nCheckOffset)
				memmove((char *)dtimgaddr,
						(char *)dtimgaddr + nCheckOffset,
						AML_DTB_IMG_MAX_SZ);
		}
#endif//#ifndef CONFIG_SKIP_KERNEL_DTB_SECBOOT_CHECK
#ifdef CONFIG_MULTI_DTB
		if (!is_write && strcmp("iread", argv[2]))
		{
				extern unsigned long get_multi_dt_entry(unsigned long fdt_addr);
				unsigned long fdtaddr = get_multi_dt_entry(dtimgaddr);

				ret = fdt_check_header((char *)fdtaddr);
				if (ret) {
						ErrP("Fail in fdt check header\n");
						return CMD_RET_FAILURE;
				}
				unsigned int fdtsz    = fdt_totalsize((char *)fdtaddr);

				if (fdtsz > AML_DTB_IMG_MAX_SZ)
					return CMD_RET_FAILURE;

				memmove((char *)dtimgaddr, (char *)fdtaddr, fdtsz);
		}
#endif// #ifdef CONFIG_MULTI_DTB

		return ret;
}

/*
  write mbr to emmc only.
  store mbr Addr
 */
extern int emmc_update_mbr(unsigned char *buffer);
static int do_store_mbr_ops(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	unsigned char *buffer;
	cpu_id_t cpu_id = get_cpu_id();

	if (cpu_id.family_id < MESON_CPU_MAJOR_ID_GXL ||
			device_boot_flag != EMMC_BOOT_FLAG) {
		ret = -1;
		ErrP("MBR not support, try [store dtb write Addr]\n");
		goto _out;
	}

	if (argc < 3)
		return CMD_RET_USAGE;

	buffer = (unsigned char *)simple_strtoul(argv[2], NULL, 16);
	ret = emmc_update_mbr(buffer);
	if (ret) {
		ErrP("fail to update mbr\n");
		goto _out;
	}
_out:
	return ret;
}

static int do_store_bootlog(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	unsigned int val;

	if (argc < 2)
		return CMD_RET_USAGE;

	val = readl(AO_SEC_GP_CFG2);
	printf("Boot logs:\n\t bl2: %d\n\tfip: %d\n", (val >> 25) & 0x7, (val >> 22) & 0x7);

	return ret;
}

#define IS_STORAGE_EMMC_BOOT(device) (((device) == EMMC_BOOT_FLAG)	\
	|| ((device) == SPI_EMMC_FLAG))

#define STORAGE_BOOT_UNKNOWN(device) (((device) != EMMC_BOOT_FLAG)	\
	&& ((device) != SPI_EMMC_FLAG)	\
	&& ((device) != NAND_BOOT_FLAG)	\
	&& ((device) != SPI_NAND_FLAG))

int store_ddr_parameter_read(u8 *buffer,
			     u32 length)
{
	int ret = 0;

	if (STORAGE_BOOT_UNKNOWN(device_boot_flag)) {
		ErrP("device_boot_flag=0x%x err\n", device_boot_flag);
		return -ENODEV;
	}

	if (IS_STORAGE_EMMC_BOOT(device_boot_flag)) {
		amlmmc_check_and_update_boot_info();
		ret = mmc_ddr_parameter_read(buffer, (int)length);
	}
#if defined(CONFIG_AML_MTD) || defined(CONFIG_AML_NAND)
	else
		ret = amlnf_ddr_parameter_read(buffer, (int)length);
#endif
	return ret;
}

int store_ddr_parameter_write(u8 *buffer, u32 length)
{
	int ret = 0;

	if (STORAGE_BOOT_UNKNOWN(device_boot_flag)) {
		ErrP("device_boot_flag=0x%x err\n", device_boot_flag);
		return -ENODEV;
	}

	if (IS_STORAGE_EMMC_BOOT(device_boot_flag)) {
		amlmmc_check_and_update_boot_info();
		ret = mmc_ddr_parameter_write(buffer, (int)length);
#if defined(CONFIG_AML_MTD) || defined(CONFIG_AML_NAND)
	} else
		ret = amlnf_ddr_parameter_write(buffer, (int)length);
#else
	}
#endif
	return ret;
}

int store_ddr_parameter_erase(void)
{
	int ret = 0;

	if (STORAGE_BOOT_UNKNOWN(device_boot_flag)) {
		ErrP("device_boot_flag=0x%x err\n", device_boot_flag);
		return -ENODEV;
	}

	if (IS_STORAGE_EMMC_BOOT(device_boot_flag))
		ret = mmc_ddr_parameter_erase();
#if defined(CONFIG_AML_MTD) || defined(CONFIG_AML_NAND)
	else
		ret = amlnf_ddr_parameter_erase();
#endif
	return ret;
}

int store_key_read(u8 *buffer,
		   u32 length, u32 *actual_length)
{
	int ret = 0;

	if (STORAGE_BOOT_UNKNOWN(device_boot_flag)) {
		ErrP("device_boot_flag=0x%x err\n", device_boot_flag);
		return -ENODEV;
	}

	if (IS_STORAGE_EMMC_BOOT(device_boot_flag))
		ret = mmc_key_read(buffer, (int)length, actual_length);
#if defined(CONFIG_AML_NAND) || defined(CONFIG_AML_MTD)
	else
		ret = amlnf_key_read(buffer, (int)length, actual_length);
#endif
	return ret;
}

int store_key_write(u8 *buffer,
		    u32 length, u32 *actual_length)
{
	int ret = 0;

	if (STORAGE_BOOT_UNKNOWN(device_boot_flag)) {
		ErrP("device_boot_flag=0x%x err\n", device_boot_flag);
		return -ENODEV;
	}

	if (IS_STORAGE_EMMC_BOOT(device_boot_flag))
		ret = mmc_key_write(buffer, (int)length, actual_length);
#if defined(CONFIG_AML_NAND) || defined(CONFIG_AML_MTD)
	else
		ret = amlnf_key_write(buffer, (int)length, actual_length);
#endif
	return ret;
}

static int do_store_key_ops(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	char _cmdbuf[128];
	char *ops = argv[2];
	const unsigned int maxkyesz = 256 * 1024;
	unsigned int actualdtbsz = 0;
	char *devcmd = NULL;

	if (argc < 4)
		return CMD_RET_USAGE;

	const int is_write = !strcmp("write", ops);
	if (!is_write) {
		ret = strcmp("read", ops);//must be 0
		if (ret)
			return CMD_RET_USAGE;
	}

	actualdtbsz = maxkyesz;
	if (argc > 4) {
		const unsigned int bufsz = simple_strtoul(argv[4], NULL, 0);

		if (bufsz > maxkyesz) {
			ErrP("bufsz (%s) > max 0x%x\n", argv[4], maxkyesz);
			return CMD_RET_FAILURE;
		}
	}

	ops = is_write ? "key_write" : "key_read";

	switch (device_boot_flag) {
	case NAND_BOOT_FLAG:
	case SPI_NAND_FLAG:
		devcmd = "amlnf";
		break;
	case EMMC_BOOT_FLAG:
	case SPI_EMMC_FLAG:
		devcmd = "emmc";
		break;
	default:
		ErrP("device_boot_flag=0x%x err\n", device_boot_flag);
		return CMD_RET_FAILURE;
	}

	sprintf(_cmdbuf, "%s %s %s 0x%x", devcmd, ops, argv[3], actualdtbsz);
	MsgP("To run cmd[%s]\n", _cmdbuf);
	ret = run_command(_cmdbuf, 0);

	return ret;
}

static int do_store_ddr_parameter_ops(cmd_tbl_t *cmdtp,
				      int flag, int argc, char * const argv[])
{
	unsigned long addr;
	int ret = 0;
	char *ops = argv[2];
	const unsigned int maxddrpsz = 2 * 1024;
	unsigned int actualdtbsz = 0;

	if (!strcmp("erase", ops) && (argc == 3)) {
		 ret = store_ddr_parameter_erase();
		 return ret;
	}

	if (argc < 4)
		return CMD_RET_USAGE;

	const int is_write = !strcmp("write", ops);
	if (!is_write) {
		ret = strcmp("read", ops);//must be 0
		if (ret)
			return CMD_RET_USAGE;
	}
	actualdtbsz = maxddrpsz;
	if (argc > 4) {
		const unsigned int bufsz = simple_strtoul(argv[4], NULL, 0);

		if (bufsz > maxddrpsz) {
			ErrP("bufsz (%s) > max 0x%x\n", argv[4], maxddrpsz);
			return CMD_RET_FAILURE;
		}
	}

	addr = (ulong)simple_strtoul(argv[3], NULL, 16);
	if (is_write)
		store_ddr_parameter_write((u8 *)addr, actualdtbsz);
	else
		store_ddr_parameter_read((u8 *)addr, actualdtbsz);

	return ret;
}

static int do_store_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int i, init_flag=0, ret = 0;
	char	str[128];

	init_flag = (argc > 2) ? (int)simple_strtoul(argv[2], NULL, 16) : 0;
	store_dbg("init_flag %d",init_flag);

	//Forcing updating device_boot_flag every time 'store init'
	if (device_boot_flag == _AML_DEVICE_BOOT_FLAG_DEFAULT || 1) {
		i = get_device_boot_flag();
		if (i) {
			MsgP("ERR:FAILED in get_device_boot_flag\n");
			return __LINE__;
		}
	}

	switch (device_boot_flag)
	{
#if defined(CONFIG_AML_NAND)
		case NAND_BOOT_FLAG:
			{
				if (init_flag >= STORE_BOOT_ERASE_PROTECT_CACHE &&
						init_flag <= STORE_BOOT_SCRUB_ALL) {
					sprintf(str, "amlnf  init  %d ",init_flag);
					run_command(str, 0);
				}

				sprintf(str, "amlnf  init  %d ",1);
				printf("command:	%s <- %d\n", str, init_flag);
				device_boot_flag = NAND_BOOT_FLAG;
				ret = run_command(str, 0);
				if (ret != 0) {
					return -1;
				}
				return ret;
			}
			break;
#endif// #if defined(CONFIG_AML_NAND)
#ifdef	CONFIG_AML_MTD
		case NAND_BOOT_FLAG:
			{
				ret = run_command("nand init", 0);
				if (init_flag >= STORE_BOOT_ERASE_PROTECT_CACHE) {
					ret |= run_command("amlnf rom_erase",0);
					ret |= run_command("nand device 1", 0);
					ret |= run_command("nand erase.chip", 0);
				}
			}
			return ret;
			break;
#endif// #ifdef	CONFIG_AML_MTD
		case EMMC_BOOT_FLAG:
			{
				store_dbg("MMC BOOT, %s %d\n", __func__, __LINE__);
				device_boot_flag = EMMC_BOOT_FLAG;
				sprintf(str, "mmc dev %d", CONFIG_SYS_MMC_ENV_DEV);
				run_command(str,0);
				ret = run_command("mmcinfo", 0);
				if (ret != 0) {
					store_msg("amlmmc cmd failed \n");
					return -1;
				}
				if (init_flag == STORE_BOOT_ERASE_PROTECT_CACHE) {
					ret = run_command("amlmmc erase non_cache", 0);
				} else if (init_flag >= STORE_BOOT_ERASE_ALL) {
					if (_info_disprotect_back_before_mmcinfo1 & DISPROTECT_KEY) {
						MsgP("amlmmc key\n");
						run_command("amlmmc key", 0);
					}
					sprintf(str, "amlmmc erase %d", CONFIG_SYS_MMC_ENV_DEV);
					MsgP("amlmmc erase %d", CONFIG_SYS_MMC_ENV_DEV);
					ret = run_command(str, 0);
				}

				return ret;
			}
			break;
		case SPI_EMMC_FLAG:
			{
				store_dbg("spi+mmc , %s %d ", __func__, __LINE__);
				ret = run_command("mmcinfo 1", 0);

				if (init_flag == STORE_BOOT_ERASE_PROTECT_CACHE) {
					store_msg("amlmmc erase non_cache \n");
					ret = run_command("amlmmc erase non_cache", 0);
				} else if (init_flag == STORE_BOOT_ERASE_ALL) {
					if (_info_disprotect_back_before_mmcinfo1 & DISPROTECT_KEY) {
						run_command("mmc key", 0);
					}
					MsgP("amlmmc erase 1 \n");
					ret = run_command("amlmmc erase 1", 0);
				}
				if (init_flag > STORE_BOOT_ERASE_PROTECT_CACHE &&
						init_flag <= STORE_BOOT_SCRUB_ALL) {
					ret = run_command("sf probe 2", 0);
					if (ret != 0) {
						store_msg("sf probe failed");
						return -1;
					}
					sprintf(str, "sf erase  0 0x%x", _SPI_FLASH_ERASE_SZ);
					ret = run_command(str,0);
				}
				if (ret != 0) {
					store_msg("cmd failed \n");
					return -1;
				}

				return ret;
			}
			break;
		case SPI_NAND_FLAG:
			{
				store_dbg("spi+nand , %s %d ", __func__, __LINE__);
#if defined(CONFIG_AML_NAND)
				if (init_flag >= STORE_BOOT_ERASE_PROTECT_CACHE &&
						init_flag <= STORE_BOOT_SCRUB_ALL) {
					sprintf(str, "amlnf  init  %d ",init_flag);
					run_command(str, 0);
				}
				sprintf(str, "amlnf  init  %d ",1);
				store_dbg("command:	%s", str);
				ret = run_command(str, 0);
#else
				ret = NAND_INIT_FAILED;
#endif

				if (ret == NAND_INIT_FAILED) {
					return -1;
#if defined(CONFIG_AML_NAND)
				} else {
					if (init_flag > STORE_BOOT_ERASE_PROTECT_CACHE &&
							init_flag <= STORE_BOOT_SCRUB_ALL) {
						ret = run_command("sf probe 2", 0);
						sprintf(str, "sf erase  0 0x%x", _SPI_FLASH_ERASE_SZ);
						ret = run_command(str,0);
					}

					if (ret != 0) {
						store_msg("cmd failed \n");
						return -1;
					}

					return ret;
#endif
				}
			}
		default:
			store_dbg("CARD BOOT, %s %d", __func__, __LINE__);
			return CMD_RET_FAILURE;
	}

	return 0;
}

static int do_store_exit(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#if defined(CONFIG_AML_NAND)
	if (device_boot_flag == NAND_BOOT_FLAG) {
		int ret = run_command("amlnf exit", 0);
		if (ret != 0) {
			MsgP("amlnf exit failed");
			return -1;
		}
	}
#endif
	if (device_boot_flag == EMMC_BOOT_FLAG) {
		/* partition table need renew */
		is_partition_checked = false;
	}
	return 0;
}

static int do_store_disprotect(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char *area;

	area = argv[2];
	if (strcmp(area, "key") == 0) {
		MsgP("disprotect key\n");
		info_disprotect |= DISPROTECT_KEY;
		_info_disprotect_back_before_mmcinfo1 |= DISPROTECT_KEY;
	}
	if (strcmp(area, "fbbt") == 0) {
		store_msg("disprotect fbbt");
		info_disprotect |= DISPROTECT_FBBT;
	}
	if (strcmp(area, "hynix") == 0) {
		store_msg("disprotect hynix");
		info_disprotect |= DISPROTECT_HYNIX;
	}

	return 0;
}

static int do_store_size(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	u64 addr;
	char *s = NULL;
	char	str[128];

	if (argc < 4)
		return CMD_RET_USAGE;

	s = argv[2];
	addr = (ulong)simple_strtoul(argv[3], NULL, 16);
	if (!addr) {
		ErrP("addr(%s) is invalid\n", argv[3]);
		return CMD_RET_FAILURE;
	}

	if (device_boot_flag == NAND_BOOT_FLAG) {
#if defined(CONFIG_AML_NAND)
		sprintf(str, "amlnf  size  %s %llx",s,addr);
		store_dbg("command:	%s", str);
		ret = run_command(str, 0);
#elif defined(CONFIG_AML_MTD)
		{//get mtd part logic size (i.e, not including the bad blocks)
			nand_info_t *mtdpartinf = NULL;
			loff_t off = 0;
			u64 partszlgc = 0;
			const char *partname = s;

			mtdpartinf = get_mtd_device_nm(partname);
			if (IS_ERR(mtdpartinf)) {
				ErrP("device(%s) is err\n", partname);
				return CMD_RET_FAILURE;
			}
			const unsigned int erasesz = mtdpartinf->erasesize;
			const u64 partszphy = mtdpartinf->size;

			partszlgc = partszphy;
			for (; off < partszphy; off += erasesz) {
				if (nand_block_isbad(mtdpartinf, off))
					partszlgc -= erasesz;
			}
			u64 *paddr = (u64 *)addr;
			*paddr = partszlgc;
		}
#else
		ret = -1;
#endif// #if defined(CONFIG_AML_NAND)
		if (ret != 0) {
			store_msg("nand cmd failed");
			return -1;
#if defined(CONFIG_AML_NAND) || defined(CONFIG_AML_MTD)
		} else {
			return ret;
#endif
		}
	} else if (device_boot_flag == SPI_NAND_FLAG) {
		#if defined(CONFIG_AML_NAND)
		sprintf(str, "amlnf  size  %s %llx",s,addr);
		store_dbg("command:	%s", str);
		ret = run_command(str, 0);
		#else
		ret = -1;
		#endif
		if (ret != 0) {
			store_msg("nand cmd failed");
			return -1;
#if defined(CONFIG_AML_NAND)
		} else {
			return ret;
#endif
		}
	} else if (device_boot_flag == SPI_EMMC_FLAG) {
		store_dbg("MMC , %s %d ", __func__, __LINE__);
		sprintf(str, "amlmmc  size  %s %llx",s,addr);
		store_dbg("command:	%s", str);
		ret = run_command(str, 0);
		if (ret != 0) {
			store_msg("amlmmc cmd failed");
			return -1;
		}
		return ret;
	} else if (device_boot_flag == EMMC_BOOT_FLAG) {
		store_dbg("MMC , %s %d ", __func__, __LINE__);
		sprintf(str, "amlmmc  size  %s %llx",s,addr);
		store_dbg("command:	%s", str);
		ret = run_command(str, 0);
		if (ret != 0) {
			store_msg("amlmmc cmd failed");
			return -1;
		}
		return ret;
	} else if (device_boot_flag == CARD_BOOT_FLAG) {
		store_dbg("CARD BOOT , %s %d ", __func__, __LINE__);
		return CMD_RET_FAILURE;
	}

	return CMD_RET_FAILURE;
}

static int do_store_erase(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int i, erase = 2, ret = 0;
	//loff_t size=0;
	char *area;
	char	str[128];
	//loff_t off = 0;

	if (argc < 3)
		return CMD_RET_USAGE;

	area = argv[2];

	if (strcmp(area, "boot") == 0) {
			//off =  argc > 3 ? simple_strtoul(argv[3], NULL, 16) : 0;
			//size =  argc > 4 ? simple_strtoul(argv[4], NULL, 16) : 0x60000;
		if (device_boot_flag == NAND_BOOT_FLAG) {
			#if defined(CONFIG_AML_NAND)
			store_dbg("NAND BOOT,erase uboot : %s %d  off =%llx ,size=%llx",
					__func__, __LINE__, off, size);

			ret = run_command("amlnf deverase boot 0",0);
			#elif defined(CONFIG_AML_MTD)
			ret = run_command("amlnf rom_erase",0);
			#else
			ret = -1;
			#endif
			if (ret != 0) {
				store_msg("nand cmd failed ");
				return -1;
			}
			return ret;
		} else if ((device_boot_flag == SPI_EMMC_FLAG) ||
				(device_boot_flag == SPI_NAND_FLAG)) {
			store_dbg("SPI BOOT,erase uboot :  %s %d  off =%llx ,size=%llx",
					__func__, __LINE__, off, size);

			ret = run_command("sf probe 2",0);
			if (ret != 0) {
				store_msg("nand cmd failed");
				return -1;
			}
			sprintf(str, "sf erase  0 0x%x", CONFIG_ENV_IN_SPI_OFFSET);//store erase boot should NOT erase ENV in flash!
			ret = run_command(str,0);
			if (ret != 0) {
				store_msg("nand cmd failed");
				return -1;
			}
			return ret;
		} else if (device_boot_flag == EMMC_BOOT_FLAG) {
			store_dbg("MMC BOOT,erase uboot :  %s %d  off =%llx ,size=%llx",
					__func__, __LINE__, off, size);

			sprintf(str, "amlmmc  erase bootloader");
			ret = run_command(str, 0);
			if (ret != 0) {
				store_msg("amlmmc cmd failed");
				return -1;
			}

#ifdef MMC_BOOT_PARTITION_SUPPORT
		#ifdef CONFIG_EMMC_KEEP_BOOT1
			/* do not erase the BOOT1 for TM2 revA ONLY*/
			if (get_cpu_id().family_id != MESON_CPU_MAJOR_ID_TM2) {
				store_msg("WRONG CONFIG_EMMC_KEEP_BOOT1 enabled!\n");
				return -1;
			}
			if (get_cpu_id().chip_rev == 0xA)
				erase = 1;
		#endif
			for (i=0; i < erase; i++) {
				printf("%s() %d, i = %d\n", __func__, __LINE__, i);
				//switch to boot partition here
				sprintf(str, "amlmmc switch 1 boot%d", i);
				store_dbg("command: %s\n", str);
				ret = run_command(str, 0);
				if (ret == -1) {
					//store_msg("mmc cmd failed \n");
					return 0;
				} else if (ret != 0) {
					store_msg("amlmmc cmd failed");
					//return -1;
					goto E_SWITCH_BACK;
				}

				//erase boot partition
				sprintf(str, "amlmmc erase bootloader");
				ret = run_command(str, 0);
				if (ret != 0) {
					store_msg("amlmmc cmd failed");
					//return -1;
					goto E_SWITCH_BACK;
				}
			}

E_SWITCH_BACK:
			//switch back to urs partition
			sprintf(str, "amlmmc switch 1 user");
			store_dbg("command: %s\n", str);
			ret = run_command(str, 0);
			if (ret != 0) {
				store_msg("amlmmc cmd failed \n");
				return -1;
			}

#endif

			return ret;
		} else {
			store_dbg("CARD BOOT,erase uboot :  %s %d  off =%llx ,size=%llx",
					__func__, __LINE__, off, size);
			return 0;
		}
	} else if (strcmp(area, "data") == 0) {
		if (device_boot_flag == NAND_BOOT_FLAG) {
			store_dbg("NAND BOOT,erase data : %s %d  off =%llx ,size=%llx",
					__func__, __LINE__, off,  size);
			#if defined(CONFIG_AML_NAND)
			ret = run_command("amlnf  deverase data 0",0);
			if (ret != 0) {
				store_msg("nand cmd failed ");
				return -1;
			}
			ret = run_command("amlnf  deverase code 0",0);
			if (ret != 0) {
				store_msg("nand cmd failed ");
				return -1;
			}
			ret = run_command("amlnf  deverase cache 0",0);
			if (ret != 0) {
				store_msg("nand cmd failed ");
				return -1;
			}
			#elif defined(CONFIG_AML_MTD)
				ret = run_command("nand device 1", 0);
				ret |= run_command("nand erase.chip", 0);
			#endif
			return ret;
		} else if (device_boot_flag == SPI_NAND_FLAG) {
			store_dbg("spi+nand , %s %d ", __func__, __LINE__);
			#if defined(CONFIG_AML_NAND)
			/*case for uboot in nor flash,system in nand flash*/
			ret = run_command("amlnf  deverase data 0",0);
			if (ret != 0) {
				store_msg("nand cmd failed ");
				return -1;
			}

			ret = run_command("amlnf  deverase code 0",0);
			if (ret != 0) {
				store_msg("nand cmd failed ");
				return -1;
			}
			ret = run_command("amlnf  deverase cache 0",0);
			if (ret != 0) {
				store_msg("nand cmd failed ");
				return -1;
			}
			#endif
			return ret;
		} else if (device_boot_flag == SPI_EMMC_FLAG) {
			store_dbg("spi+mmc , %s %d ", __func__, __LINE__);
			ret = run_command("mmc  erase  1",0); // whole
			if (ret != 0) {
				store_msg("mmc cmd failed ");
				return -1;
			}

			return ret;
		} else if (device_boot_flag == EMMC_BOOT_FLAG) {
			store_dbg("MMC BOOT,erase data : %s %d  off =%llx ,size=%llx",
					__func__, __LINE__, off, size);
			MsgP("amlmmc erase non_loader\n");
			ret = run_command("amlmmc erase non_loader",0); //whole
			if (ret != 0) {
				store_msg("amlmmc cmd failed ");
				return -1;
			}
			return ret;
		} else {
			store_dbg("CARD BOOT,erase data : %s %d  off =%llx ,size=%llx",
					__func__, __LINE__, off, size);
			return 0;
		}
	} else if (strcmp(area, "key") == 0) {
		if (device_boot_flag == EMMC_BOOT_FLAG) {
			sprintf(str, "emmc erase key");
			ret = run_command(str, 0);
			if (ret != 0) {
				store_msg("emmc cmd failed");
				return CMD_RET_USAGE;
			}
		} else if (device_boot_flag == NAND_BOOT_FLAG) {
		#if defined(CONFIG_AML_NAND) || defined(CONFIG_AML_MTD)
			sprintf(str, "amlnf key_erase");
			ret = run_command(str, 0);
			if (ret != 0) {
				store_msg("emmc cmd failed");
				return CMD_RET_USAGE;
			}
		#endif
		}
	} else if (strcmp(area, "dtb") == 0) {
		if (device_boot_flag == EMMC_BOOT_FLAG) {
			sprintf(str, "emmc erase dtb");
			ret = run_command(str, 0);
			if (ret != 0) {
				store_msg("emmc cmd failed");
				return CMD_RET_USAGE;
			}
		} else if (device_boot_flag == NAND_BOOT_FLAG) {
		#if defined(CONFIG_AML_NAND) || defined(CONFIG_AML_MTD)
			sprintf(str, "amlnf dtb_erase");
			ret = run_command(str, 0);
			if (ret != 0) {
				store_msg("emmc cmd failed");
				return CMD_RET_USAGE;
			}
		#endif
		}
	} else if (strcmp(area, "partition") == 0) {
		if (device_boot_flag == EMMC_BOOT_FLAG) {
			int blk_shift;
			int dev, n;
			u64 cnt=0, blk =0;
			struct partitions *part_info;
			struct mmc *mmc = NULL;
			char *p_name = NULL;

			p_name = argv[3];
			if (!p_name)
				return CMD_RET_USAGE;

			dev = find_dev_num_by_partition_name(p_name);
			if (dev < 0)
				return CMD_RET_USAGE;
			mmc = find_mmc_device(dev);
			if (!mmc)
				return CMD_RET_FAILURE;

			mmc_init(mmc);
			if (!ffs(mmc->read_bl_len))
				return CMD_RET_FAILURE;
			blk_shift = ffs(mmc->read_bl_len) -1;
			if (!(info_disprotect & DISPROTECT_KEY)
					&& (strncmp(p_name, MMC_RESERVED_NAME,
							sizeof(MMC_RESERVED_NAME)) == 0x00)) {
				printf("\"%s-partition\" is been protecting and should no be erased!\n",
						MMC_RESERVED_NAME);
				return CMD_RET_FAILURE;
			}

			part_info = find_mmc_partition_by_name(p_name);
			if (part_info == NULL)
				return CMD_RET_FAILURE;

			blk = part_info->offset>> blk_shift;
			if (emmc_cur_partition
					&& !strncmp(p_name, "bootloader", strlen("bootloader")))
				cnt = mmc->boot_size>> blk_shift;
			else
				cnt = part_info->size>> blk_shift;
			n = mmc->block_dev.block_erase(dev, blk, cnt);
			printf("store erase \"%s-partition\" is %s\n", p_name, n ? "fail" : "ok");
			if (n)
				return CMD_RET_FAILURE;
		} else if (device_boot_flag == NAND_BOOT_FLAG) {
#ifdef CONFIG_AML_MTD
			if (argc < 4)
				return CMD_RET_USAGE;
			sprintf(str, "nand erase.part %s", argv[3]);
			return run_command(str, 0);
#else
			return CMD_RET_USAGE;
#endif//#ifdef CONFIG_AML_MTD
		} else {
			return CMD_RET_USAGE;
		}
	} else
		return CMD_RET_USAGE;

	return 0;
}

static int do_store_scrub(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	loff_t off=0;
	char	str[128];

	off = (ulong)simple_strtoul(argv[2], NULL, 16);
	sprintf(str, "amlnf  scrub %d", (int)off);
	if (device_boot_flag == NAND_BOOT_FLAG) {
		#if defined(CONFIG_AML_NAND)
		ret = run_command(str, 0);
		#elif defined(CONFIG_AML_MTD)
		printf("%s() fixme, to do...\n", __func__);
		#else
		ret = -1;
		#endif
		if (ret != 0) {
			store_msg("nand cmd failed");
			return -1;
		}
	} else if (device_boot_flag == SPI_NAND_FLAG) {
		store_dbg("spi+nand , %s %d ", __func__, __LINE__);
		#if defined(CONFIG_AML_NAND)
		ret = run_command(str, 0);
		if (ret != 0) {
			store_msg("nand cmd failed");
			return -1;
		}
		#endif

		ret = run_command("sf probe 2", 0);
		if (ret != 0) {
			store_msg("nand cmd failed");
			return -1;
		}
		sprintf(str, "sf erase  0 0x%x", _SPI_FLASH_ERASE_SZ);
		ret = run_command(str,0);
		if (ret != 0) {
			store_msg("nand cmd failed");
			return -1;
		}
		return ret;
	} else if (device_boot_flag == SPI_EMMC_FLAG) {
		store_dbg("spi+mmc , %s %d\n", __func__, __LINE__);
		ret = run_command("amlmmc erase whole",0);
		if (ret != 0) {
			store_msg("amlmmc cmd failed\n");
			return -1;
		}
		return ret;
	} else if (device_boot_flag == EMMC_BOOT_FLAG) {
		store_dbg("MMC BOOT, %s %d\n", __func__, __LINE__);
		device_boot_flag = EMMC_BOOT_FLAG;
		run_command("mmc dev 1", 0);
		ret = run_command("mmcinfo", 0);
		if (ret != 0) {
			store_msg("amlmmc cmd failed\n");
			return -1;
		}
		if (_info_disprotect_back_before_mmcinfo1 & DISPROTECT_KEY) {
			MsgP("mmc key\n");
			run_command("mmc key", 0);
		}
		MsgP("amlmmc erase 1");
		ret = run_command("amlmmc erase 1", 0);
	}

	return ret;
}

static int do_store_rom_protect(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

#if defined(CONFIG_AML_NAND)
	char	str[128];
	char *area = argv[2];
#endif

	if (argc < 3)
		return CMD_RET_USAGE;

	if (device_boot_flag == NAND_BOOT_FLAG) {
#if defined(CONFIG_AML_NAND)
		sprintf(str, "amlnf  rom_protect  %s", area);
		store_dbg("command:	%s", str);
		int ret = run_command(str, 0);
		if (ret != 0) {
			store_msg("nand cmd failed");
			return -1;
		}
#elif defined(CONFIG_AML_MTD)
		printf("%s() fixme, to do...\n", __func__);
#else
		return -1;
#endif
	}

	return CMD_RET_SUCCESS;
}

static int do_store_rom_write(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	u64 addr;
	loff_t off=0, size=0;
	char	str[128];
	int ret = 0;
	if (argc < 5)
		return CMD_RET_USAGE;

	addr = (ulong)simple_strtoul(argv[2], NULL, 16);
	if (get_off_size(argc - 3, (char **)(argv + 3), &off, &size) != 0)
		return CMD_RET_FAILURE;

	if (device_boot_flag == NAND_BOOT_FLAG) {
#if defined(CONFIG_AML_NAND) || defined(CONFIG_AML_MTD)
#ifndef CONFIG_DISCRETE_BOOTLOADER
		sprintf(str, "amlnf rom_write 0x%llx 0x%llx 0x%llx", addr, off, size);
		store_dbg("command:	%s", str);
		ret = run_command(str, 0);
#else
/*
 *store rom_write addr offset size <iCopy>
 //Used to update the whole bootloader, i.e, update 'bl2 + tpl' at the same time
 @iCopy is optional,
	if used, must < min(tplcpynum, bl2cpynum), and update only the specified copy
	if not used, update all the copies of bl2 and tpl
*/
		const int bl2size = BL2_SIZE;
		const int bl2cpynum = CONFIG_BL2_COPY_NUM; //TODO: decided by efuse, no macro
		const int tplcapsize = CONFIG_TPL_SIZE_PER_COPY;
		const int tplcpynum = CONFIG_TPL_COPY_NUM;
		const int bootloadermaxsz = bl2size + tplcapsize;
		const int tplwritesz = size - bl2size;
		loff_t copyoff = 0;
		const int icopy2update = argc > 5 ? simple_strtoul(argv[5], NULL, 0) : -1;
		const int TPL_MIN_SZ = (1U << 16);
		const int updatetpl = tplwritesz > TPL_MIN_SZ;
		int i = 0;

		if (bootloadermaxsz < size) {
			ErrP("bootloader sz 0x%llx too large,max sz 0x%x\n",
					size, bootloadermaxsz);
			return CMD_RET_FAILURE;
		}
		if (!updatetpl)
			MsgP("Warn:tplwritesz 0x%x too small,update bl2 but not tpl\n",
					tplwritesz);
		if (icopy2update >= tplcpynum || icopy2update >= bl2cpynum) {
			ErrP("icopy2update[%s] invalid, must < min(%d, %d)\n",
					argv[5], tplcpynum, bl2cpynum);
			return CMD_RET_FAILURE;
		}
		for (i = 0; i < bl2cpynum; ++i)
		{
			if (icopy2update >= 0 && icopy2update != i)
				continue;

			sprintf(str, "amlnf rom_erase %d", i);
			ret = run_command(str, 0);
			if (ret) {
				ErrP("Failed at erase bl2[%d],ret=%d\n", i, ret);
				return CMD_RET_FAILURE;
			}

			//copyoff = i * bl2size;
			sprintf(str, "amlnf bl2_write 0x%llx %d 0x%x", addr, i, bl2size);
			debugP("runCmd[%s]\n", str);
			ret = run_command(str, 0);
			if (ret) {
				ErrP("Failed at pgram bl2[%d],ret=%d\n", i, ret);
				return CMD_RET_FAILURE;
			}
		}
		addr += bl2size;
		for (i = 0; i < tplcpynum && updatetpl; ++i)
		{
			if (icopy2update >= 0 && icopy2update != i)
				continue;

			sprintf(str, "amlnf fip_erase %d", i);
			ret = run_command(str, 0);
			if (ret) {
				ErrP("Failed at erase tpl[%d],ret=%d\n", i, ret);
				return CMD_RET_FAILURE;
			}

			copyoff = i * tplcapsize;
			sprintf(str, "amlnf fip_write 0x%llx %llx 0x%x", addr, copyoff, tplwritesz);
			debugP("runCmd[%s]\n", str);
			ret = run_command(str, 0);
			if (ret) {
				ErrP("Failed at pgram bl2[%d],ret=%d\n", i, ret);
				return CMD_RET_FAILURE;
			}
		}
#if CONFIG_TPL_VAL_NUM_MIN
		_bootloaderOrgCrc[0] = crc32(0,
				(unsigned char *)(addr - bl2size), bl2size);
		_bootloaderOrgCrc[1] = crc32(0,
				(unsigned char *)addr, tplwritesz);
#endif// #if CONFIG_TPL_VAL_NUM_MIN
#endif//#ifndef CONFIG_DISCRETE_BOOTLOADER
#else
		ret = -1;
#endif
		if (ret != 0) {
			store_msg("nand cmd failed");
			return -1;
		}
		return ret;
	} else if ((device_boot_flag == SPI_EMMC_FLAG) ||
			(device_boot_flag == SPI_NAND_FLAG)) {
		ret = run_command("sf  probe 2",0);
		if (ret != 0) {
			store_msg("nand cmd failed");
			return -1;
		}
		sprintf(str, "sf  erase  0x%llx  0x%llx ", off, size);
		ret = run_command(str, 0);
		if (ret != 0) {
			store_msg("nand cmd failed");
			return -1;
		}
		sprintf(str, "sf  write 0x%llx  0x%llx  0x%llx ",addr, off, size);
		store_dbg("command:	%s", str);
		ret = run_command(str, 0);
		if (ret != 0) {
			store_msg("nand cmd failed");
			return -1;
		}
		return ret;
	} else if (device_boot_flag == EMMC_BOOT_FLAG) {
		store_dbg("MMC BOOT, %s %d\n", __func__, __LINE__);
#ifndef CONFIG_AML_SECU_BOOT_V2
#ifdef MMC_UBOOT_CLEAR_MBR
		//modify the 55 AA info for emmc uboot
		unsigned char *tmp_buf= (unsigned char *)addr;
		_mbrFlag[0] = tmp_buf[510];
		_mbrFlag[1] = tmp_buf[511];
		tmp_buf[510]=0;
		tmp_buf[511]=0;
#endif
#endif// #if defined(CONFIG_AML_SECU_BOOT_V2)
		sprintf(str, "amlmmc  write bootloader 0x%llx  0x%llx  0x%llx",
				addr, off, size);
		store_dbg("command: %s\n", str);
		ret = run_command(str, 0);
		if (ret != 0) {
			store_msg("amlmmc cmd failed \n");
			return -1;
		}
		return ret;
	} else {
		store_dbg("CARD BOOT, %s %d", __func__, __LINE__);
		return 0;
	}
}

static int do_store_rom_read(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	u64 addr;
	loff_t off=0, size=0;
	char	str[128];
	int ret = 0;
	int i = 0, read = 2;
	cpu_id_t cpu_id = get_cpu_id();

	if (argc < 5)
		return CMD_RET_USAGE;

	addr = (ulong)simple_strtoul(argv[2], NULL, 16);
	if (get_off_size(argc - 3, (char **)(argv + 3), &off, &size) != 0)
		return CMD_RET_FAILURE;

	if (device_boot_flag == NAND_BOOT_FLAG) {
#if defined(CONFIG_AML_NAND) || defined(CONFIG_AML_MTD)
#ifndef CONFIG_DISCRETE_BOOTLOADER
		sprintf(str, "amlnf rom_read 0x%llx 0x%llx 0x%llx", addr, off, size);
		store_dbg("command:	%s", str);
		ret = run_command(str, 0);
#else
/*
 *store rom_read addr offset size <iCopy>
 //Used to read the whole bootloader, i.e, update 'bl2 + tpl' at the same time
 @iCopy is optional,
	if used, must < min(tplcpynum, bl2cpynum), and read only the specified copy
	if not used, check if all the copies of 'bl2 + tpl' are same content
*/
		const int bl2size       = BL2_SIZE;
		const int bl2cpynum     = CONFIG_BL2_COPY_NUM; //TODO: decided by efuse, no macro
		const int tplcapsize    = CONFIG_TPL_SIZE_PER_COPY;
		const int tplcpynum     = CONFIG_TPL_COPY_NUM;
		const int bootloadermaxsz = bl2size + tplcapsize;
		const int tplrealsz     = size - bl2size;
		loff_t copyoff = 0;
		int icopy2update  = argc > 5 ? simple_strtoul(argv[5], NULL, 0) : -1;
		char *tmpbuf = NULL;
		int okcrcnum = 0;
		const int verifymode =
			(off == (1ULL << 62) - 1) && (icopy2update < 0); //verify mode

		if (!verifymode && icopy2update < 0)
			icopy2update = 0; //default read copy 0 if no verify mode

		if (bootloadermaxsz < size || tplrealsz < 0) {
			ErrP("bootloader sz 0x%llx invalid,  max sz %d\n", size, bootloadermaxsz);
			return CMD_RET_FAILURE;
		}
		if (icopy2update >= tplcpynum || icopy2update >= bl2cpynum) {
			ErrP("icopy2update[%s] invalid, must < min(%d, %d)\n",
					argv[5], tplcpynum, bl2cpynum);
			return CMD_RET_FAILURE;
		}

		tmpbuf = (char *)malloc(size);
		if (!tmpbuf) {
			ErrP("Failed malloc 0x%llx bytes\n", size);
			return CMD_RET_FAILURE;
		}
		memset(tmpbuf, 0, size);

		char *readbuf = tmpbuf;
		const u32 orgbl2crc = _bootloaderOrgCrc[0];

		for (i = 0; i < bl2cpynum; ++i)
		{
			if (icopy2update >= 0 && icopy2update != i)
				continue;

			sprintf(str, "amlnf bl2_read 0x%p %x 0x%x", readbuf, i, bl2size);
			debugP("runCmd[%s]\n", str);
			ret = run_command(str, 0);
			if (ret) {
				ErrP("Failed at pgram bl2[%d],ret=%d\n", i, ret);
				free(tmpbuf);
				return CMD_RET_FAILURE;
			}
#if CONFIG_BL2_VAL_NUM_MIN
			if (verifymode) {//copy index not specified, need read all copies
				const u32 readcrc = crc32(0, (unsigned char *)readbuf, bl2size);

				if (readcrc == orgbl2crc) {
					okcrcnum += 1;
					if (okcrcnum >= CONFIG_BL2_VAL_NUM_MIN)
						break;
				}
			}
#endif//#if CONFIG_BL2_VAL_NUM_MIN
		}
#if CONFIG_BL2_VAL_NUM_MIN
		if (okcrcnum < CONFIG_BL2_VAL_NUM_MIN && verifymode) {
			ErrP("okcrcnum(%d) < CONFIG_BL2_VAL_NUM_MIN(%d)\n",
					okcrcnum, CONFIG_BL2_VAL_NUM_MIN);
			free(tmpbuf);
			return CMD_RET_FAILURE;
		}
		okcrcnum = 0;
#endif//#if CONFIG_BL2_VAL_NUM_MIN
		memcpy((char *)addr, readbuf, bl2size);

		if (tplrealsz > 0) // to support dump only bl2
		{
			const u32 orgtplcrc = _bootloaderOrgCrc[1];

			for (i = 0; i < tplcpynum && !ret; ++i)
			{
				if (icopy2update >= 0 && icopy2update != i)
					continue;

				copyoff = i * tplcapsize;
				sprintf(str, "amlnf fip_read 0x%p %llx 0x%x",
						readbuf, copyoff, tplrealsz);
				debugP("runCmd[%s]\n", str);
				ret = run_command(str, 0);
				if (ret) {
					ErrP("Failed at pgram bl2[%d],ret=%d\n", i, ret);
					free(tmpbuf);
					return CMD_RET_FAILURE;
				}
#if CONFIG_TPL_VAL_NUM_MIN
				if (verifymode) //copy index not specified, need read all copies
				{
					const u32 readcrc = crc32(0,
							(unsigned char *)readbuf, tplrealsz);

					if (orgtplcrc == readcrc)
						okcrcnum += 1;
					if (okcrcnum >= CONFIG_TPL_VAL_NUM_MIN)
						break;
				}
#endif//#if CONFIG_TPL_VAL_NUM_MIN
			}
#if CONFIG_TPL_VAL_NUM_MIN
			if (okcrcnum < CONFIG_TPL_VAL_NUM_MIN && verifymode) {
				ErrP("okcrcnum(%d) < CONFIG_TPL_VAL_NUM_MIN(%d)\n",
						okcrcnum, CONFIG_TPL_VAL_NUM_MIN);
				free(tmpbuf);
				return CMD_RET_FAILURE;
			}
#endif//#if CONFIG_TPL_VAL_NUM_MIN
			memcpy((char *)addr + bl2size, (unsigned char *)readbuf, tplrealsz);
		}
		free(tmpbuf);
#endif// #ifndef CONFIG_DISCRETE_BOOTLOADER
#else
		ret = -1;
#endif// #if defined(CONFIG_AML_NAND) || defined(CONFIG_AML_MTD)
		if (ret != 0) {
			store_msg("nand cmd failed");
			return -1;
		}
		return ret;
	} else if ((device_boot_flag == SPI_EMMC_FLAG) ||
			(device_boot_flag == SPI_NAND_FLAG)) {
		ret = run_command("sf  probe 2",0);
		if (ret != 0) {
			return -1;
		}
		sprintf(str, "sf read 0x%llx  0x%llx  0x%llx ",
				addr, off, size);
		store_dbg("command:	%s", str);
		ret = run_command(str, 0);
		if (ret != 0) {
			store_msg("nand cmd failed");
			return -1;
		}
		return ret;
	} else if (device_boot_flag == EMMC_BOOT_FLAG) {
		if (cpu_id.family_id >= MESON_CPU_MAJOR_ID_GXL)
			off += 512;
		store_dbg("MMC BOOT, %s %d \n",__func__,__LINE__);
		sprintf(str, "amlmmc  read bootloader 0x%llx  0x%llx  0x%llx",
				addr, off, size);
		store_dbg("command: %s\n", str);
		//tmp_buf= (unsigned char *)addr;
		ret = run_command(str, 0);
		if (ret != 0) {
			store_msg("amlmmc cmd failed \n");
			return -1;
		}

#ifdef MMC_BOOT_PARTITION_SUPPORT
	#ifdef CONFIG_EMMC_KEEP_BOOT1
		if (get_cpu_id().family_id != MESON_CPU_MAJOR_ID_TM2) {
			store_msg("WRONG CONFIG_EMMC_KEEP_BOOT1 enabled!\n");
			return -1;
		}
		if (get_cpu_id().chip_rev == 0xA)
			read = 1;
	#endif
		for (i = 0; i < read; i++) {
			//switch to boot partition here
			sprintf(str, "amlmmc switch 1 boot%d", i);
			store_dbg("command: %s\n", str);
			ret = run_command(str, 0);
			if (ret == -1) {
				return 0;
			} else if (ret != 0) {
				store_msg("amlmmc cmd failed");
				goto R_SWITCH_BACK;
				//return -1;
			}

			//write uboot to boot partition
			sprintf(str, "amlmmc  read bootloader 0x%llx  0x%llx  0x%llx",
					addr, off, size);
			store_dbg("command: %s\n", str);
			ret = run_command(str, 0);
			if (ret != 0) {
				store_msg("amlmmc cmd failed \n");
				//return -1;
				goto R_SWITCH_BACK;
			}
		}

R_SWITCH_BACK:
		//switch back to urs partition
		sprintf(str, "amlmmc switch 1 user");
		store_dbg("command: %s\n", str);
		ret = run_command(str, 0);
		if (ret != 0) {
			store_msg("amlmmc cmd failed \n");
			return -1;
		}

#endif
#ifndef CONFIG_AML_SECU_BOOT_V2
#ifdef MMC_UBOOT_CLEAR_MBR
		unsigned char *tmp_buf= (unsigned char *)addr;
		tmp_buf[510]= _mbrFlag[0];
		tmp_buf[511]= _mbrFlag[1];
#endif
#endif// #ifndef CONFIG_AML_SECU_BOOT_V2
		return ret;
	} else {
		store_dbg("CARD BOOT, %s %d ",__func__,__LINE__);
		return 0;
	}

}

static int do_store_read(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	u64 addr;
	loff_t off=0, size=0;
	char	str[128];
	int ret = 0;
	char *s = argv[2];

	if (argc < 6)
		return CMD_RET_USAGE;

	addr = (ulong)simple_strtoul(argv[3], NULL, 16);

	if (get_off_size(argc - 4, (char **)(argv + 4), &off, &size) != 0)
		return CMD_RET_FAILURE;

	store_dbg("addr = %llx off= 0x%llx  size=0x%llx",addr,off,size);
	if (device_boot_flag == NAND_BOOT_FLAG) {
#if defined(CONFIG_AML_NAND) || defined(CONFIG_AML_MTD)
#if defined(CONFIG_AML_NAND)
		sprintf(str, "amlnf  read_byte %s 0x%llx  0x%llx  0x%llx",
			s, addr, off, size);
#elif defined(CONFIG_AML_MTD)
	#if  defined(CONFIG_DISCRETE_BOOTLOADER)
		if (!strcmp(CONFIG_TPL_PART_NAME, s)) {
			const int tplcapsize    = CONFIG_TPL_SIZE_PER_COPY;
			const int tplcpynum     = CONFIG_TPL_COPY_NUM;
			const int icopy2update  =
				argc > 6 ? simple_strtoul(argv[6], NULL, 0) : 0;//0 copy at default

			if (icopy2update >= tplcpynum) {
				ErrP("icopy2update[%s] invalid, must < max(%d)\n",
						argv[6], tplcpynum);
				return CMD_RET_FAILURE;
			}

			loff_t copyoff = icopy2update * tplcapsize;

			sprintf(str, "amlnf fip_read 0x%llx %llx 0x%llx",
					addr, copyoff, size);
		} else {
	#endif // #if  defined(CONFIG_DISCRETE_BOOTLOADER)
			ret =  mtd_find_phy_off_by_lgc_off(s, off, &off);
			if (ret) {
				ErrP("Fail in find phy addr by logic off (0x%llx),ret(%d)\n",
						off, ret);
				return CMD_RET_FAILURE;
			}
			sprintf(str, "nand  read %s 0x%llx  0x%llx  0x%llx",
					s, addr, off, size);
		}
#endif // #if defined(CONFIG_AML_NAND)
#else
		ret = -1;
#endif
		ret = run_command(str, 0);
		if (ret != 0) {
			store_msg("nand cmd [%s] failed ",str);
			return -1;
		}
		return ret;
	} else if (device_boot_flag == SPI_NAND_FLAG) {
		#if defined(CONFIG_AML_NAND)
		sprintf(str, "amlnf  read_byte %s 0x%llx  0x%llx  0x%llx",
				s, addr, off, size);
		store_dbg("command:	%s\n", str);
		ret = run_command(str, 0);
		#else
		return -1;
		#endif

#if defined(CONFIG_AML_NAND)
		if (ret != 0) {
			store_msg("nand cmd failed \n");
			return -1;
		} else {
			return ret;
		}
#endif
	} else if (device_boot_flag == SPI_EMMC_FLAG) {
		store_dbg("spi+mmc , %s %d ",__func__,__LINE__);
		sprintf(str, "amlmmc  read %s 0x%llx  0x%llx  0x%llx",
				s, addr, off, size);
		store_dbg("command:	%s\n", str);
		ret = run_command(str, 0);
		if (ret != 0) {
			store_msg("amlmmc cmd failed \n");
			return -1;
		}
		return ret;
	} else if (device_boot_flag == EMMC_BOOT_FLAG) {
		store_dbg("MMC BOOT, %s %d \n",__func__,__LINE__);
		sprintf(str, "amlmmc  read %s 0x%llx  0x%llx  0x%llx",
				s, addr, off, size);
		store_dbg("command:	%s\n", str);
		ret = run_command(str, 0);
		if (ret != 0) {
			store_msg("amlmmc cmd failed \n");
			return -1;
		}
		return ret;
	} else {
		store_dbg("CARD BOOT, %s %d ",__func__,__LINE__);

		return 0;
	}
}

static int do_store_write(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	u64 addr;
	loff_t off=0, size=0;
	char	str[128];
	int ret = -1;
	char *s = argv[2];

	if (argc < 6)
		return CMD_RET_USAGE;

	addr = (ulong)simple_strtoul(argv[3], NULL, 16);
	if (get_off_size(argc - 4, (char **)(argv + 4), &off, &size) != 0)
		return CMD_RET_FAILURE;

	if (device_boot_flag == NAND_BOOT_FLAG) {
#if defined(CONFIG_AML_NAND) || defined(CONFIG_AML_MTD)
	#if defined(CONFIG_AML_NAND)
		sprintf(str, "amlnf write_byte %s 0x%llx  0x%llx  0x%llx",
				s, addr, off, size);
		ret = run_command(str, 0);
	#elif defined(CONFIG_AML_MTD)
		#if  defined(CONFIG_DISCRETE_BOOTLOADER)
		if (!strcmp(CONFIG_TPL_PART_NAME, s)) {
			const int tplcapsize = CONFIG_TPL_SIZE_PER_COPY;
			const int tplcpynum = CONFIG_TPL_COPY_NUM;
			const int icopy2update =
				argc > 6 ? simple_strtoul(argv[6], NULL, 0) : -1;
			int i = 0;

			debugP("icopy2update=%d, tplcpynum=%d\n", icopy2update, tplcpynum);
			if (icopy2update >= tplcpynum) {
				ErrP("icopy2update[%s] invalid, must < max(%d)\n",
						argv[6], tplcpynum);
				return CMD_RET_FAILURE;
			}

			for (i = 0; i < tplcpynum; ++i)
			{
				if (icopy2update >= 0 && icopy2update != i)
					continue;

				sprintf(str, "amlnf fip_erase %d", i);
				ret = run_command(str, 0);
				if (ret) {
					ErrP("Failed at erase tpl[%d],ret=%d\n", i, ret);
					return CMD_RET_FAILURE;
				}

				loff_t copyoff = i * tplcapsize;

				sprintf(str, "amlnf fip_write 0x%llx %llx 0x%llx",
						addr, copyoff, size);
				debugP("runCmd[%s]\n", str);
				ret = run_command(str, 0);
				if (ret) {
					ErrP("Failed at pgram bl2[%d],ret=%d\n", i, ret);
					return CMD_RET_FAILURE;
				}
			}
		} else
		#endif // #if  defined(CONFIG_DISCRETE_BOOTLOADER)
		{
			ret =  mtd_find_phy_off_by_lgc_off(s, off, &off);
			if (ret) {
				ErrP("Fail in find phy addr by logic off (0x%llx),ret(%d)\n", off, ret);
			}
			sprintf(str, "nand write %s 0x%llx  0x%llx  0x%llx",s, addr, off, size);
			ret = run_command(str, 0);
		}
   #endif
#endif
		if (ret != 0) {
			store_msg("nand cmd failed ");
			return -1;
		}
	} else if (device_boot_flag == SPI_NAND_FLAG) {
		store_dbg("spi+nand , %s %d ",__func__,__LINE__);
		#if defined(CONFIG_AML_NAND)
		sprintf(str, "amlnf  write_byte %s 0x%llx  0x%llx  0x%llx", s, addr, off, size);
		store_dbg("command:	%s", str);
		ret = run_command(str, 0);
		#else
		ret = -1;
		#endif

		if (ret != 0) {
			store_msg("nand cmd failed \n");
			return -1;
		#if defined(CONFIG_AML_NAND)
		} else {
			return ret;
		#endif
		}
	} else if (device_boot_flag == SPI_EMMC_FLAG) {
		store_dbg("spi+mmc , %s %d ",__func__,__LINE__);
		sprintf(str, "amlmmc  write %s 0x%llx  0x%llx  0x%llx", s, addr, off, size);
		store_dbg("command: %s\n", str);
		ret = run_command(str, 0);
		if (ret != 0) {
			store_msg("amlmmc cmd failed \n");
			return -1;
		}
	} else if (device_boot_flag == EMMC_BOOT_FLAG) {
		store_dbg("MMC BOOT, %s %d \n",__func__,__LINE__);
		sprintf(str, "amlmmc  write %s 0x%llx  0x%llx  0x%llx", s, addr, off, size);
		store_dbg("command: %s\n", str);
		ret = run_command(str, 0);
		if (ret != 0) {
			store_msg("amlmmc cmd failed \n");
			return -1;
		}
	} else {
		store_dbg("CARD BOOT, %s %d ",__func__,__LINE__);
		ret = CMD_RET_FAILURE;
	}
	return ret;
}

static cmd_tbl_t cmd_store_sub[] = {
	U_BOOT_CMD_MKENT(init,          4, 0, do_store_init, "", ""),
	U_BOOT_CMD_MKENT(exit,          3, 0, do_store_exit, "", ""),
	U_BOOT_CMD_MKENT(disprotect,    3, 0, do_store_disprotect, "", ""),
	U_BOOT_CMD_MKENT(rom_protect,   5, 0, do_store_rom_protect, "", ""),
	U_BOOT_CMD_MKENT(size,          5, 0, do_store_size, "", ""),
	U_BOOT_CMD_MKENT(scrub,         3, 0, do_store_scrub, "", ""),
	U_BOOT_CMD_MKENT(erase,         5, 0, do_store_erase, "", ""),
	U_BOOT_CMD_MKENT(read,          7, 0, do_store_read, "", ""),
	U_BOOT_CMD_MKENT(write,         7, 0, do_store_write, "", ""),
	U_BOOT_CMD_MKENT(rom_read,      5, 0, do_store_rom_read, "", ""),
	U_BOOT_CMD_MKENT(rom_write,     5, 0, do_store_rom_write, "", ""),
	U_BOOT_CMD_MKENT(dtb,           5, 0, do_store_dtb_ops, "", ""),
	U_BOOT_CMD_MKENT(key,           5, 0, do_store_key_ops, "", ""),
	U_BOOT_CMD_MKENT(ddr_parameter, 5, 0, do_store_ddr_parameter_ops, "", ""),
	U_BOOT_CMD_MKENT(mbr,           3, 0, do_store_mbr_ops, "", ""),
	U_BOOT_CMD_MKENT(bootlog,       2, 0, do_store_bootlog, "", ""),
};

static int do_store(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	cmd_tbl_t *c;

	if (argc < 2)
		return CMD_RET_USAGE;
	c = find_cmd_tbl(argv[1], cmd_store_sub, ARRAY_SIZE(cmd_store_sub));
	if (c)
		return	c->cmd(cmdtp, flag, argc, argv);

	return CMD_RET_USAGE;
}

U_BOOT_CMD(store, CONFIG_SYS_MAXARGS, 1, do_store,
	"STORE sub-system",
	"init flag\n"
	"store read name addr off|partition size\n"
	"    read 'size' bytes starting at offset 'off'\n"
	"    to/from memory address 'addr', skipping bad blocks.\n"
	"store write name addr off|partition size\n"
	"    write 'size' bytes starting at offset 'off'\n"
	"    to/from memory address 'addr', skipping bad blocks.\n"
	"store rom_write add off size.\n"
	"	write uboot to the boot device\n"
	"store erase boot/data: \n"
	"	erase the area which is uboot or data \n"
	"store erase partition <partition_name>: \n"
	"	erase the area which partition in u-boot \n"
	"store erase dtb \n"
	"store erase key \n"
	"store disprotect key \n"
	"store rom_protect on/off \n"
	"store scrub off|partition size\n"
	"	scrub the area from offset and size \n"
	"store dtb iread/read/write addr <size>\n"
	"	read/write dtb, size is optional \n"
	"store key read/write addr <size>\n"
	"	read/write key, size is optional \n"
	"store ddr_parameter read/write addr <size>\n"
	"	read/write ddr parameter, size is optional \n"
	"store mbr addr\n"
	"   update mbr/partition table by dtb\n"
	"store bootlog\n"
	"   show boot logs\n"
);

