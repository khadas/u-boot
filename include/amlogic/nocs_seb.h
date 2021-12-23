/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __NOCS_SEB_H__
#define __NOCS_SEB_H__

#define CA_NOCS_MODE                    0x05
#define CA_NOCS_SCS                     0X0C00
#define CA_NOCS_MODE_REG                0xfe440070
#define CA_NOCS_SCS_STATUS              0xfe440040
#define MAX_SKIP_NUM                    0x05

/*S905C2 TYPE1*/
#define CMP_OFFSET_TYPE1                0x3f840
#define CMP_SIZE                         72
#define INFO_OFFSET                     0x3f884
#define REPLACE_INFO_OFFSET             0x3f800
#define REPLACE_INFO_SIZE                512

typedef enum {
	NOCS_UNDEFINE = 0,
	NOCS_S905C2_TYPE1,
	LAST_NOCS_TYPE
} nocs_chip_type;

typedef enum {
	SEB_NO_ERROR = 0,
	SEB_ERROR,
	SEB_UNSUPPORT,
	LAST_SEB_STATUS
} nocs_seb_status;

typedef enum {
	NOCS_SCS_DISABLE = 0,
	NOCS_SCS_ENABLE,
} nocs_scs_status;

typedef struct {
	unsigned int    area_num;
	unsigned int    offset[MAX_SKIP_NUM];
	unsigned int    size[MAX_SKIP_NUM];
} nocs_boot_replace;

nocs_seb_status get_nocs_inf(nocs_chip_type *chip_type, nocs_scs_status *scs_status);
nocs_seb_status nocs_secure_boot_modify(nocs_chip_type chip_type,
				unsigned char *flash_boot, unsigned int flash_boot_size,
				unsigned char *update_boot, unsigned int update_boot_size,
				nocs_boot_replace *nocs_replace);
#endif /* __NOCS_SEB_H__ */
