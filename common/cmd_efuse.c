/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * common/cmd_efuse.c
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#include <config.h>
#include <common.h>
#include <asm/arch/io.h>
#include <command.h>
#include <malloc.h>
#include <asm/arch/efuse.h>
#include <asm/arch/bl31_apis.h>
#include <asm/cpu_id.h>
#include <amlogic/aml_efuse.h>

#define CMD_EFUSE_WRITE            0
#define CMD_EFUSE_READ             1
#define CMD_EFUSE_READ_CALI        2
#define CMD_EFUSE_READ_CALI_ITEM   3
#define CMD_EFUSE_SECURE_BOOT_SET  6
#define CMD_EFUSE_PASSWORD_SET     7
#define CMD_EFUSE_CUSTOMER_ID_SET  8

#define CMD_EFUSE_CHECK_PATTERN_ITEM   10

#define CMD_EFUSE_AMLOGIC_SET      20


int cmd_efuse(int argc, char * const argv[], char *buf)
{
	//int i, action = -1;
	int i, action;
	loff_t offset;
	uint32_t size = 0, max_size;
	char *end;
	char *s;
	int ret;
	long lAddr1, lAddr2;

	if (strncmp(argv[1], "read", 4) == 0) {
		action = CMD_EFUSE_READ;
	} else if (strncmp(argv[1], "write", 5) == 0) {
		action = CMD_EFUSE_WRITE;
	} else if (strncmp(argv[1], "cali_read", 9) == 0) {
		action = CMD_EFUSE_READ_CALI;
	} else if (strncmp(argv[1], "item_read", 9) == 0) {
		action = CMD_EFUSE_READ_CALI_ITEM;
		goto efuse_action;
	} else if (strncmp(argv[1], "check", 9) == 0) {
		action = CMD_EFUSE_CHECK_PATTERN_ITEM;
		goto efuse_action;
	} else if (strncmp(argv[1], "secure_boot_set", 15) == 0) {
		action = CMD_EFUSE_SECURE_BOOT_SET;
		goto efuse_action;
	} else if (strncmp(argv[1], "password_set", 12) == 0) {
		action = CMD_EFUSE_PASSWORD_SET;
		goto efuse_action;
	} else if (strncmp(argv[1], "customer_id_set", 15) == 0) {
		action = CMD_EFUSE_CUSTOMER_ID_SET;
		goto efuse_action;
	} else if (strncmp(argv[1], "amlogic_set", 11) == 0) {
		action = CMD_EFUSE_AMLOGIC_SET;
		goto efuse_action;
	} else{
		printf("%s arg error\n", argv[1]);
		return CMD_RET_USAGE;
	}

	if ((get_cpu_id().family_id == MESON_CPU_MAJOR_ID_AXG)
		&&((action == CMD_EFUSE_READ) || (action == CMD_EFUSE_WRITE))) {
		printf("error: AXG not support read/write normal efuse\n");
		return -1;
	}

	if (argc < 4)
		return CMD_RET_USAGE;
	/*check efuse user data max size*/
	offset = simple_strtoul(argv[2], &end, 16);
	size = simple_strtoul(argv[3], &end, 16);
	printf("%s: offset is %d  size is  %d\n", __func__, (unsigned int)offset, size);
	max_size = efuse_get_max();
	if (!size) {
		printf("\n error: size is zero!!!\n");
		return -1;
	}
	if (offset > max_size) {
		printf("\n error: offset is too large!!!\n");
		printf("\n offset should be less than %d!\n", max_size);
		return -1;
	}
	if (offset+size > max_size) {
		printf("\n error: offset + size is too large!!!\n");
		printf("\n offset + size should be less than %d!\n", max_size);
		return -1;
	}

efuse_action:

	/* efuse read */
	if (action == CMD_EFUSE_READ) {
		memset(buf, 0, size);
		ret = efuse_read_usr(buf, size, (loff_t *)&offset);
		if (ret == -1) {
			printf("ERROR: efuse read user data fail!\n");
			return -1;
		}

		if (ret != size)
			printf("ERROR: read %d byte(s) not %d byte(s) data\n",
			       ret, size);
		printf("efuse read data");
		for (i = 0; i < size; i++) {
			if (i%16 == 0)
				printf("\n");
			printf(":%02x", buf[i]);
		}
		printf("\n");
	}
	/* efuse write */
	else if (action == CMD_EFUSE_WRITE) {
		if (argc < 5) {
			printf("arg count error\n");
			return CMD_RET_USAGE;
		}
		memset(buf, 0, size);

		s = argv[4];
		memcpy(buf, s, strlen(s));
		if (efuse_write_usr(buf, size, (loff_t *)&offset) < 0) {
			printf("error: efuse write fail.\n");
			return -1;
		} else {
			printf("%s written done.\n", __func__);
		}
	} else if (action == CMD_EFUSE_READ_CALI) {
		memset(buf, 0, size);
		ret = efuse_read_cali(buf, size, offset);
		if (ret == -1) {
			printf("ERROR: efuse read cali data fail!\n");
			return -1;
		}

		if (ret != size)
			printf("ERROR: read %d byte(s) not %d byte(s) data\n",
			       ret, size);
		printf("efuse read cali data");
		for (i = 0; i < size; i++) {
			if (i%16 == 0)
				printf("\n");
			printf(":%02x", buf[i]);
		}
		printf("\n");
	} else if (action == CMD_EFUSE_READ_CALI_ITEM) {
		s = argv[2];
		memset(buf, 0, size);
		ret = efuse_get_cali_item(s);
		if (ret < 0) {
			printf("ERROR: efuse read cali item data fail!\n");
			return -1;
		}
		printf("efuse %s cali data=0x%x\n",s,ret);
	} else if (action == CMD_EFUSE_CHECK_PATTERN_ITEM) {
		s = argv[2];
		ret = efuse_check_pattern_item(s);
		if (ret < 0) {
			printf("ERROR: efuse check pattern fail!\n");
			return -1;
		}
		printf("efuse %s %s\n", s, ret > 0 ? "has been written" : "is not write");
		return ret == 0 ? 1 : 0; //cmd return 0: wrote, 1: not write
	} else if (CMD_EFUSE_SECURE_BOOT_SET == action) {
		/*efuse secure_boot_set*/

		lAddr1 = GXB_IMG_LOAD_ADDR;

		if (argc > 2)
			lAddr1 = simple_strtoul(argv[2], &end, 16);

		lAddr2 = get_sharemem_info(GET_SHARE_MEM_INPUT_BASE);
		memcpy((void *)lAddr2, (void *)lAddr1, GXB_EFUSE_PATTERN_SIZE);
		flush_cache(lAddr2,GXB_EFUSE_PATTERN_SIZE);

		ret = aml_sec_boot_check(AML_D_P_W_EFUSE_SECURE_BOOT, lAddr2,
			GXB_EFUSE_PATTERN_SIZE, 0);

		if (ret)
			printf("aml log : Secure boot EFUSE pattern programming fail [%d]!\n",
			       ret);
		else
			printf("aml log : Secure boot EFUSE pattern programming success!\n");

		return ret;
	} else if (CMD_EFUSE_AMLOGIC_SET == action) {
		/*efuse amlogic_set*/

		lAddr1 = GXB_IMG_LOAD_ADDR;

		if (argc > 2)
			lAddr1 = simple_strtoul(argv[2], &end, 16);

		lAddr2 = get_sharemem_info(GET_SHARE_MEM_INPUT_BASE);
		memcpy((void *)lAddr2, (void *)lAddr1, GXB_EFUSE_PATTERN_SIZE);
		flush_cache(lAddr2,GXB_EFUSE_PATTERN_SIZE);

		ret = aml_sec_boot_check(AML_D_P_W_EFUSE_AMLOGIC, lAddr2,
			GXB_EFUSE_PATTERN_SIZE, 0);

		if (ret)
			printf("aml log : Amlogic EFUSE pattern programming fail [%d]!\n",
			       ret);
		else
			printf("aml log : Amlogic EFUSE pattern programming success!\n");

		return ret;
	} else if(CMD_EFUSE_PASSWORD_SET == action)	{
		/*efuse password_set*/

		lAddr1 = GXB_IMG_LOAD_ADDR;

		if (argc > 2)
			lAddr1 = simple_strtoul(argv[2], &end, 16);

		lAddr2 = get_sharemem_info(GET_SHARE_MEM_INPUT_BASE);
		memcpy((void *)lAddr2, (void *)lAddr1, GXB_EFUSE_PATTERN_SIZE);
		flush_cache(lAddr2,GXB_EFUSE_PATTERN_SIZE);

		ret = aml_sec_boot_check(AML_D_P_W_EFUSE_PASSWORD, lAddr2,
			GXB_EFUSE_PATTERN_SIZE, 0);

		if (ret)
			printf("aml log : Password EFUSE pattern programming fail [%d]!\n",
			       ret);
		else
			printf("aml log : Password EFUSE pattern programming success!\n");

		return ret;
	}else if(CMD_EFUSE_CUSTOMER_ID_SET == action)	{
		/*efuse customer_id_set*/

		lAddr1 = GXB_IMG_LOAD_ADDR;

		if (argc > 2)
			lAddr1 = simple_strtoul(argv[2], &end, 16);

		lAddr2 = get_sharemem_info(GET_SHARE_MEM_INPUT_BASE);
		memcpy((void *)lAddr2, (void *)lAddr1, GXB_EFUSE_PATTERN_SIZE);
		flush_cache(lAddr2,GXB_EFUSE_PATTERN_SIZE);

		ret = aml_sec_boot_check(AML_D_P_W_EFUSE_CUSTOMER_ID, lAddr2,
			GXB_EFUSE_PATTERN_SIZE, 0);

		if (ret)
			printf("aml log : Customer ID EFUSE pattern programming fail [%d]!\n",
			       ret);
		else
			printf("aml log : Customer ID EFUSE pattern programming success!\n");

		return ret;
	}

	return 0;
}


int do_efuse(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char buf[EFUSE_BYTES];

	memset(buf, 0, sizeof(buf));

	if (argc < 2)
		return CMD_RET_USAGE;

	return cmd_efuse(argc, argv, buf);
}

static char efuse_help_text[] =
#ifndef CONFIG_AML_MESON_AXG
	"[read/write offset size [data]]\n"
	"  [cali_read]  read from cali\n"
	"                   -example: [efuse cali_read 0 7]; offset is 0,size is 7. \n"
	"  [item_read]  read cali value\n"
	"                  [item_read sensor/saradc/mipicsi/hdmirx/eth/cvbs]\n"
	"  [check]      -check if pattern is write\n"
	"                  [check dgpk1|dgpk2|aud_id]\n"
	"  [read/wirte]  - read or write 'size' data from\n"
	"                  'offset' from efuse user data ;\n"
	"  [offset]      - the offset byte from the beginning\n"
	"                  of efuse user data zone;\n"
	"  [size]        - data size\n"
	"  [data]        - the optional argument for 'write',\n"
	"                  data is treated as characters\n"
	"  examples: efuse write 0xc 0xd abcdABCD1234\n"
#endif
	"[amlogic_set addr]\n";


U_BOOT_CMD(
	efuse,	5,	1,	do_efuse,
	"efuse commands", efuse_help_text
);

#ifdef CONFIG_EFUSE_OBJ_API
int do_efuse_obj(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	//uint32_t rc = CMD_RET_FAILURE;
	uint32_t rc;
	uint8_t set = 0;
	uint32_t obj_id = 0;
	uint8_t buff[32];
	uint32_t bufflen = sizeof(buff);
	uint32_t size = 0;

	if (argc < 3 || argc > 4) {
		printf("Invalid number of arguments %d\n", argc);
		return CMD_RET_USAGE;
	}

	memset(&buff[0], 0, sizeof(buff));

	if (strcasecmp(argv[1], "set") == 0) {
		set = 1;
	}
	else if (strcasecmp(argv[1], "get") == 0) {
		set = 0;
	}
	else {
		printf("Unknown command\n");
		return CMD_RET_USAGE;
	}

	if (strcasecmp(argv[2], "usb-boot-disable") == 0) {
		obj_id = EFUSE_OBJ_LICENSE_DISABLE_USB_BOOT;
	}
	else if (strcasecmp(argv[2], "usb-password-protect") == 0) {
		obj_id = EFUSE_OBJ_LICENSE_ENABLE_USB_BOOT_PASSWORD;
	}
	else if (strcasecmp(argv[2], "jtag-disable") == 0) {
		obj_id = EFUSE_OBJ_LICENSE_DISABLE_JTAG_ALL;
	}
	else {
		printf("Unknown object ID\n");
		return CMD_RET_USAGE;
	}

	if (set) {
		//rc = efuse_obj_write(obj_id, (size ? &buff[0] : NULL), size); //coverity error
		rc = efuse_obj_write(obj_id, NULL, size);

		if (rc == EFUSE_OBJ_SUCCESS)
			rc = CMD_RET_SUCCESS;
		else {
			printf("Error setting eFUSE object: %d\n", rc);
			rc = CMD_RET_FAILURE;
		}
	}
	else {
		rc = efuse_obj_read(obj_id, &buff[0], &bufflen);
		if (rc == EFUSE_OBJ_SUCCESS) {
			int i;

			for (i = 0; i < bufflen; i++) {
				printf("%02x%s", buff[i], ((i && i % 16 == 15) || (i == bufflen - 1) ? "\n" : " "));
			}
			rc = CMD_RET_SUCCESS;
		}
		else {
			printf("Error getting eFUSE object: %d\n", rc);
			rc = CMD_RET_FAILURE;
		}
	}

	return rc;
}

static char efuse_obj_help_text[] =
	"[set | get] <object-id> {<object-value-hexdump-string>}\n"
	"\n"
	"  usb-boot-disable      : Permanently disable USB boot\n"
	"  usb-password-protect  : Enable USB boot password protection\n"
	"  jtag-disable          : Permanently disable JTAG\n"
	"\n";

U_BOOT_CMD(
	efuse_obj,	4,	0,	do_efuse_obj,
	"eFUSE object program commands", efuse_obj_help_text
);
#endif /* CONFIG_EFUSE_OBJ_API */

#include <asm/arch/secure_apb.h>

static int do_query(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int nReturn = CMD_RET_USAGE;

	struct{
		char *szQuery;	unsigned long lAddrConfig;	unsigned int nMask; unsigned int nNegFlag;
	} ArrQuery[] = {
		{"SecureBoot",AO_SEC_SD_CFG10,1<<4,0},//SecureBoot : 1:enabled;0:disabled
		{"Dolby",AO_SEC_SD_CFG10,1<<16,0},    //Dolby : 1:enabled; 0: disabled
		{"DTS",AO_SEC_SD_CFG10,1<<14,0},      //DTS: 1: enabled; 0:disabled
											  //add more query support here ...
		{NULL,0,0,0}
	};

	int nIndex;

	if (argc < 2)
		goto exit;

	for (nIndex = 0;nIndex < sizeof(ArrQuery)/sizeof(ArrQuery[0]);++nIndex)
	{
		if (ArrQuery[nIndex].szQuery)
		{
			if (!strcmp(ArrQuery[nIndex].szQuery,argv[1]))
			{
				nReturn  = (readl(ArrQuery[nIndex].lAddrConfig) & ArrQuery[nIndex].nMask) ? 1 : 0;
				nReturn ^= (ArrQuery[nIndex].nNegFlag ? 1 : 0);
				break;
			}
		}
	}

exit:

	return nReturn;
}

static char query_text[] =
	"[query SecureBoot/Dolby/DTS]\n"
	"  [SecureBoot]  - query SoC is secure boot enabled(1) or not(0)\n"
	"  [Dolby]       - query SoC support Dolby (1) or not(0)\n"
	"  [DTS]         - query SoC support DTS (1) or not(0)\n"
	"  examples: query SecureBoot\n";

U_BOOT_CMD(
	query,	5,	2,	do_query,
	"SoC query commands", query_text
);


/****************************************************/
