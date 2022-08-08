// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <config.h>
#include <common.h>
#include <asm/arch/io.h>
#include <command.h>
#include <malloc.h>

#define LOGBUF_RDFLG 0x80000000
#define RTOSINFO_PHY_ADDR	(0x80000000 + 0x1000)

struct xrtosinfo_t {
	u32	version;
	u32	status;
	u32	cpumask;
	u32	flags;
	u32	logbuf_len;
	u32	logbuf_phy;
	u32	rtos_run_flag;
	u32	android_status;
	u32	reserved0;
	u32	reserved1;
};

struct logbuf_t {
	uint32_t write;
	uint32_t read;
	char buf[0];
};

static struct xrtosinfo_t *rtosinfo;
static struct logbuf_t *logbuf;

static int detect_quit_cmd(void)
{
	int quit = 0;
	int key = 0;

	if (tstc()) {	/* we got a key press	*/
		key = getc(); /* consume input */
		switch (key) {
		case 'q':
		case 'Q':
			quit = 1;
		break;
		}
	}
	return quit;
}

static inline void logbuf_begin_get_log(void)
{
	u32 rd1, rd2;

	rd1 = READ_ONCE(logbuf->read);
	rd2 = (rd1 | LOGBUF_RDFLG);
	WRITE_ONCE(logbuf->read, rd2);
}

static inline int logbuf_get_readptr(const char **pbuf)
{
	const u32 logbuf_len = rtosinfo->logbuf_len - sizeof(struct logbuf_t);
	u32 rd1, rd2, wt;
	int len;

	rd1 = READ_ONCE(logbuf->read);
	wt = READ_ONCE(logbuf->write);
	rd2 = (rd1 & ~LOGBUF_RDFLG);
	if (rd2 <= wt)
		len = wt - rd2;
	else
		len = logbuf_len - rd2;
	if (pbuf)
		*pbuf = (const char *)logbuf->buf + rd2;
	return len;
}

static inline void logbuf_post_get_log(int len, int last)
{
	const u32 logbuf_len = rtosinfo->logbuf_len - sizeof(struct logbuf_t);
	u32 rd1, rd2;

	rd1 = READ_ONCE(logbuf->read);
	rd2 = (rd1 & ~LOGBUF_RDFLG);
	rd2 += len;
	if (rd2 >= logbuf_len)
		rd2 -= logbuf_len;
	if (last == 0)
		rd2 |= (rd1 & LOGBUF_RDFLG);
	WRITE_ONCE(logbuf->read, rd2);
}

static bool map_freertos_logbuf(void)
{
	void *buf;

	buf = map_physmem(RTOSINFO_PHY_ADDR, sizeof(struct xrtosinfo_t), MAP_WRBACK);
	if (!buf) {
		puts("Failed to map RTOSINFO memory\n");
		return false;
	}

	rtosinfo = (struct xrtosinfo_t *)buf;

	logbuf = (struct logbuf_t *)map_physmem(rtosinfo->logbuf_phy,
				rtosinfo->logbuf_len,
				MAP_WRBACK);
	if (!logbuf) {
		puts("Failed to map log buf\n");
		unmap_physmem(buf, sizeof(struct xrtosinfo_t));
		return false;
	}

	return true;
}

static void unmap_freertos_logbuf(void)
{
	unmap_physmem(logbuf, rtosinfo->logbuf_len);
	unmap_physmem(rtosinfo, sizeof(struct xrtosinfo_t));
	logbuf = NULL;
	rtosinfo = NULL;
}

static void output_freertos_logbuf(const char *p, int len)
{
	#define TMP_BUF_LEN 128
	static char tmpbuf[TMP_BUF_LEN + 1];

	if (len > 0) {
		int remain = len;

		do {
			int copylen = remain;

			if (copylen > TMP_BUF_LEN)
				copylen = TMP_BUF_LEN;

			memcpy(tmpbuf, p, copylen);
			tmpbuf[copylen] = 0;
			puts(tmpbuf);

			p += copylen;
			remain -= copylen;
		} while (remain > 0);
	}
}

int do_freertos_log(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int quit = 0;
	const char *p;
	int cnt = 0;

	if (argc < 2)
		return CMD_RET_USAGE;

	if (strncmp(argv[1], "read", 4) == 0) {
		if (!map_freertos_logbuf())
			return CMD_RET_FAILURE;

		logbuf_begin_get_log();

		cnt = logbuf_get_readptr(&p);
		if (cnt > 0)
			output_freertos_logbuf(p, cnt);

		logbuf_post_get_log(cnt, true);
		unmap_freertos_logbuf();
	} else if (strncmp(argv[1], "autoread", 8) == 0) {
		if (!map_freertos_logbuf())
			return CMD_RET_FAILURE;

		do {
			logbuf_begin_get_log();

			cnt = logbuf_get_readptr(&p);
			if (cnt > 0)
				output_freertos_logbuf(p, cnt);

			logbuf_post_get_log(cnt, true);
			quit  =  detect_quit_cmd();
		} while (!quit);

		unmap_freertos_logbuf();
	} else {
		printf("arg error\n");
		return CMD_RET_USAGE;
	}
	return 0;
}

static char freertos_log_help_text[] =
	"read freertos log\n"
	"[read log once ]\n"
	"[autoread  auto read, 'q' exit]\n";

U_BOOT_CMD(freertos_log,  2,  0,  do_freertos_log,
	"freerots_log commands", freertos_log_help_text
);
