/*
 * Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
 *
 * All information contained herein is Amlogic confidential.
 *
 * This software is provided to you pursuant to Software License Agreement
 * (SLA) with Amlogic Inc ("Amlogic"). This software may be used
 * only in accordance with the terms of this agreement.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification is strictly prohibited without prior written permission from
 * Amlogic.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __PRINTF_H
#define __PRINTF_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdarg.h>		/* For va_list */
#include <stdint.h>

#include "common.h"
#include "FreeRTOSConfig.h"
#define KERN_ERROR		1
#define KERN_WARNING	2
#define KERN_DEBUG		3
#define KERN_INFO		4

#ifndef TAG
#define TAG
#endif

#if CONFIG_LOGLEVEL_DEFAULT >= 1
#define AGLOGE(format, ...)		LOGE(KERN_ERROR, TAG format, ##__VA_ARGS__)
#else
#define AGLOGE(format, ...)
#endif

#if CONFIG_LOGLEVEL_DEFAULT >= 2
#define AGLOGW(format, ...)		LOGW(KERN_WARNING, TAG format, ##__VA_ARGS__)
#else
#define AGLOGW(format, ...)
#endif

#if CONFIG_LOGLEVEL_DEFAULT >= 3
#define AGLOGD(format, ...)		LOGD(KERN_DEBUG, TAG format, ##__VA_ARGS__)
#else
#define AGLOGD(format, ...)
#endif

//#define LOGE(KERN_ERROR, format, ...)         aml_log_out(KERN_ERROR, format, ##__VA_ARGS__)
//#define LOGW(KERN_WARNING, format, ...)               aml_log_out(KERN_WARNING, format, ##__VA_ARGS__)
//#define LOGD(KERN_DEBUG, format, ...)         aml_log_out(KERN_DEBUG, format, ##__VA_ARGS__)
#if 0
#define LOGE(LOG_TAG, format, ...)		aml_log_out(KERN_ERROR, LOG_TAG"<E>" format, ##__VA_ARGS__)
#define LOGW(LOG_TAG, format, ...)		aml_log_out(KERN_WARNING, LOG_TAG"<W>" format, ##__VA_ARGS__)
#define LOGD(LOG_TAG, format, ...)		aml_log_out(KERN_DEBUG, LOG_TAG"<D>" format, ##__VA_ARGS__)
#define LOGI(LOG_TAG, format, ...)		aml_log_out(KERN_INFO, LOG_TAG"<I>" format, ##__VA_ARGS__)
#endif

	int vfnprintf(int (*addchar)(void *context, int c), void *context,
		      const char *format, va_list args);

	int sPrintf_ext(char *str, size_t size, const char *format,
			va_list args);
	int iprintf(const char *fmt, ...);
	int sPrintf(char *str, size_t size, const char *fmt, ...);
	int syslog_buf_init(void);
//	int aml_log_out(int level, const char *fmt, ...);
	int iprint_string(char *str);

#ifndef MYPRINTF_OVERRIDE_LIBC
#define MYPRINTF_OVERRIDE_LIBC	1
#endif

#if MYPRINTF_OVERRIDE_LIBC
#define printf iprintf
#endif

#ifdef __cplusplus
}
#endif
#endif				/* __CROS_EC_PRINTF_H */
