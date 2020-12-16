/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * drivers/display/minui/minui_log.h
 *
 * Copyright (C) 2020 Amlogic, Inc. All rights reserved.
 *
 */

#ifndef _MINUI_LOG_H_
#define _MINUI_LOG_H_

#include <common.h>

#define UI_LOG_TAG "[UI]"

#define UI_LOG_LEVEL_NULL 0
#define UI_LOG_LEVEL_DEBUG 1
#define UI_LOG_LEVEL_DEBUG2 2
#define UI_LOG_LEVEL_DEBUG3 3

extern unsigned int ui_log_level;

#define ui_logl() \
	printf(UI_LOG_TAG "%s:%d\n", __func__, __LINE__)

#define ui_logv(fmt, ...) \
	printf(UI_LOG_TAG "%s:%d " fmt, __func__, __LINE__, ##__VA_ARGS__)

#define ui_logi(fmt, ...) \
	printf(UI_LOG_TAG fmt, ##__VA_ARGS__)

#define ui_loge(fmt, ...) \
	printf(UI_LOG_TAG "ERR: " fmt, ##__VA_ARGS__)

#define ui_logd(fmt, ...) \
	do { \
		if (ui_log_level >= UI_LOG_LEVEL_DEBUG) { \
			printf(UI_LOG_TAG fmt, ##__VA_ARGS__); \
		} \
	} while (0)

#define ui_logd2(fmt, ...) \
	do { \
		if (ui_log_level >= UI_LOG_LEVEL_DEBUG2) { \
			printf(UI_LOG_TAG fmt, ##__VA_ARGS__); \
		} \
	} while (0)

#define ui_logd3(fmt, ...) \
	do { \
		if (ui_log_level >= UI_LOG_LEVEL_DEBUG3) { \
			printf(UI_LOG_TAG fmt, ##__VA_ARGS__); \
		} \
	} while (0)

#endif
