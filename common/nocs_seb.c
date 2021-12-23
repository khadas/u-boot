// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <common.h>
#include <amlogic/cpu_id.h>
#include <amlogic/nocs_seb.h>
#include <asm/io.h>
#include <asm/arch/cpu_config.h>

#ifndef DEBUG_NOCS_SEB
#define DEBUG_NOCS_SEB 0
#endif
#define debug_print(fmt, args...)		\
	debug_cond(DEBUG_NOCS_SEB, fmt, ##args)

#ifdef CONFIG_UPDATE_UBOOT_NOCS
static unsigned int array_to_uint32_little(unsigned char *array)
{
	return ((array)[3] << 24 | (array)[2] << 16 | (array)[1] << 8 | (array)[0]);
}
#endif

nocs_seb_status get_nocs_inf(nocs_chip_type *chip_type, nocs_scs_status *scs_status)
{
#ifdef CONFIG_UPDATE_UBOOT_NOCS
	unsigned int reg_value = 0;
	int soc_family_id = 0;

	soc_family_id = get_cpu_id().family_id;
	debug_print("family_id:0x%x\n", soc_family_id);
	if (soc_family_id != MESON_CPU_MAJOR_ID_SC2) {
		*chip_type = NOCS_UNDEFINE;
		*scs_status = NOCS_SCS_DISABLE;
		return SEB_NO_ERROR;
	}
	reg_value = readl(CA_NOCS_MODE_REG);
	debug_print("reg_value0:0x%x\n", reg_value);
	if ((reg_value & 0x0f) == CA_NOCS_MODE) {
		*chip_type = NOCS_S905C2_TYPE1;
		reg_value = readl(CA_NOCS_SCS_STATUS);
		debug_print("reg_value1:0x%x\n", reg_value);
		if ((reg_value & CA_NOCS_SCS) != 0)
			*scs_status = NOCS_SCS_ENABLE;
		else
			*scs_status = NOCS_SCS_DISABLE;
	} else {
		*chip_type = NOCS_UNDEFINE;
		*scs_status = NOCS_SCS_DISABLE;
	}
	debug_print("chiptype:%d scs_status:%d\n", *chip_type, *scs_status);
#else
	*chip_type = NOCS_UNDEFINE;
	*scs_status = NOCS_SCS_DISABLE;
	debug_print("UnConfig chiptype:%d scs_status:%d\n", *chip_type, *scs_status);
#endif
	return SEB_NO_ERROR;
}

nocs_seb_status nocs_secure_boot_modify(nocs_chip_type chip_type,
				unsigned char *flash_boot, unsigned int flash_boot_size,
				unsigned char *update_boot, unsigned int update_boot_size,
				nocs_boot_replace *nocs_replace)
{
	int ret = SEB_NO_ERROR;

#ifdef CONFIG_UPDATE_UBOOT_NOCS
	unsigned int i;

	switch (chip_type) {
	case NOCS_S905C2_TYPE1:
	{
		for (i = 0; i < CMP_SIZE; i++)
			if (flash_boot[CMP_OFFSET_TYPE1 + i] != update_boot[CMP_OFFSET_TYPE1 + i]) {
				ret = SEB_UNSUPPORT;
				break;
		}
		nocs_replace->area_num = 2;
		nocs_replace->offset[0] = REPLACE_INFO_OFFSET;
		nocs_replace->size[0] = REPLACE_INFO_SIZE;
		nocs_replace->offset[1] = array_to_uint32_little(&update_boot[INFO_OFFSET]);
		nocs_replace->size[1] = array_to_uint32_little(&update_boot[INFO_OFFSET + 4]);
	} break;
	default:
	{
		printf("Unsupport chip type!\n");
		ret = SEB_ERROR;
	} break;
	}
#else
	nocs_replace->area_num = 0;
#endif
	return ret;
}

#if DEBUG_NOCS_SEB
static int do_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	nocs_chip_type chip_type;
	nocs_scs_status scs_status;
	nocs_seb_status ret;

	ret = get_nocs_inf(&chip_type, &scs_status);
	debug_print("test chiptype:%d scs_status:%d ret:%d\n", chip_type, scs_status, ret);
	return 0;
}

U_BOOT_CMD(dnocs, 1, 0, do_test,
	"determines whether NOCS are included",
	"read chip id and cas mode to determines whether NOCS are included\n"
	);
#endif
