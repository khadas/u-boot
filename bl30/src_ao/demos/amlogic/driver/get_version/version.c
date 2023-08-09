#include "FreeRTOSConfig.h"
#ifdef configBL30_VERSION_SAVE
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "save_version.h"
#endif
#include "version.h"
#include "myprintf.h"

int version(void)
{
	printf("AOCPU image version='%s'\n", VERSION);

	return 0;
}

#ifdef configBL30_VERSION_SAVE
#define TAG_NAME "BL30: "
static int version_string(char *buf)
{
	char *tmp_buf = NULL;
	int size = 0;

	if (buf)
		tmp_buf = buf;
	else
		return 0;

	if (strlen(VERSION)) {
		size += strlen(TAG_NAME);
		memcpy((void *)tmp_buf, (void *)TAG_NAME, strlen(TAG_NAME));
		tmp_buf += strlen(TAG_NAME);

		size += strlen(VERSION);
		memcpy((void *)tmp_buf, (void *)VERSION, strlen(VERSION));
		tmp_buf += strlen(VERSION);

		memcpy((void *)tmp_buf, (void *)"\n", 1);
		tmp_buf++;
		size += 1;
	}

	return size;
}

/*******************************************************************************
 * Save build message
 ******************************************************************************/
int bl30_plat_save_version(void)
{
	struct build_messages_t *build_info;
	char rs[200];

	build_info = (struct build_messages_t *)(uintptr_t)(BL2E_VER_BUFF_BASE_ADDR);

	if (build_info->h.type != PARAM_MESSAGE ||
		build_info->h.version != VERSION_1 ||
		build_info->h.size != sizeof(struct build_messages_t)) {
		printf("invalid build messages info head!\n");
		return -1;
	}

	if ((sizeof(struct build_messages_t)
			+ build_info->bl2_message_size
			+ build_info->bl2e_message_size
			+ build_info->bl2x_message_size
			+ build_info->bl31_message_size
			+ build_info->bl32_message_size
			+ version_string(rs))
			> BL2E_VER_BUFF_SIZE) {
		printf("invalid bl30 build messages size!\n");
		return -1;
	}

	if (build_info->bl32_message_addr && build_info->bl32_message_size)
		build_info->bl30_message_addr = build_info->bl32_message_addr
				+ build_info->bl32_message_size;
	else if (build_info->bl31_message_addr && build_info->bl31_message_size)
		build_info->bl30_message_addr = build_info->bl31_message_addr
				+ build_info->bl31_message_size;
	else if (build_info->bl2x_message_addr && build_info->bl2x_message_size)
		build_info->bl30_message_addr = build_info->bl2x_message_addr
				+ build_info->bl2x_message_size;
	else if (build_info->bl2e_message_addr && build_info->bl2e_message_size)
		build_info->bl30_message_addr = build_info->bl2e_message_addr
				+ build_info->bl2e_message_size;
	else if (build_info->bl2_message_addr && build_info->bl2_message_size)
		build_info->bl30_message_addr = build_info->bl2_message_addr
				+ build_info->bl2_message_size;
	else
		build_info->bl30_message_addr = BL2E_VER_BUFF_BASE_ADDR
				+ sizeof(struct build_messages_t);

	build_info->bl30_message_size = version_string(rs);
#ifdef DEBUG
	printf("\n build_info->bl31_message_addr %d",
			(uint32_t)build_info->bl31_message_addr);
	printf("\n build_info->bl31_message_size %d",
			(uint32_t)build_info->bl31_message_size);
	printf("\n build_info->bl30_message_addr %d",
			(uint32_t)build_info->bl30_message_addr);
	printf("\n build_info->bl30_message_size %d",
			(uint32_t)build_info->bl30_message_size);
#endif
	if (build_info->bl30_message_size)
		memcpy((void *)(uintptr_t)build_info->bl30_message_addr, rs,
			build_info->bl30_message_size);
	return 0;
}
#endif

