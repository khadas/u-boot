// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2008 - 2009
 * Windriver, <www.windriver.com>
 * Tom Rix <Tom.Rix@windriver.com>
 *
 * Copyright 2011 Sebastian Andrzej Siewior <bigeasy@linutronix.de>
 *
 * Copyright 2014 Linaro, Ltd.
 * Rob Herring <robh@kernel.org>
 */

#include <common.h>
#include <fastboot.h>
#include <net/fastboot.h>
#include <emmc_partitions.h>
#include <amlogic/storage.h>

#ifndef getenv
#define getenv env_get
#define setenv env_set
#endif//#ifndef getenv

#define CONFIG_FASTBOOT_MAX_DOWN_SIZE        0x8000000

/**
 * fastboot_buf_addr - base address of the fastboot download buffer
 */
void *fastboot_buf_addr;

/**
 * fastboot_buf_size - size of the fastboot download buffer
 */
u32 fastboot_buf_size;

/**
 * fastboot_progress_callback - callback executed during long operations
 */
void (*fastboot_progress_callback)(const char *msg);

/**
 * fastboot_response() - Writes a response of the form "$tag$reason".
 *
 * @tag: The first part of the response
 * @response: Pointer to fastboot response buffer
 * @format: printf style format string
 */
void fastboot_response(const char *tag, char *response,
		       const char *format, ...)
{
	va_list args;

	strlcpy(response, tag, FASTBOOT_RESPONSE_LEN);
	if (format) {
		va_start(args, format);
		vsnprintf(response + strlen(response),
			  FASTBOOT_RESPONSE_LEN - strlen(response) - 1,
			  format, args);
		va_end(args);
	}
}

/**
 * fastboot_fail() - Write a FAIL response of the form "FAIL$reason".
 *
 * @reason: Pointer to returned reason string
 * @response: Pointer to fastboot response buffer
 */
void fastboot_fail(const char *reason, char *response)
{
	fastboot_response("FAIL", response, "%s", reason);
}

/**
 * fastboot_busy() - Write a INFO response of the form "INFO$reason".
 *
 * @reason: Pointer to returned reason string
 * @response: Pointer to fastboot response buffer
 */
void fastboot_busy(const char *reason, char *response)
{
	fastboot_response("INFO", response, "%s", reason);
}

/**
 * fastboot_okay() - Write an OKAY response of the form "OKAY$reason".
 *
 * @reason: Pointer to returned reason string, or NULL to send a bare "OKAY"
 * @response: Pointer to fastboot response buffer
 */
void fastboot_okay(const char *reason, char *response)
{
	if (reason)
		fastboot_response("OKAY", response, "%s", reason);
	else
		fastboot_response("OKAY", response, NULL);
}

/**
 *check lock state
 *return 1 if locked
 *return 0 if unlocked
 */
int check_lock(void)
{
	char *lock_s;
	LockData_t info = {0};

	lock_s = env_get("lock");
	if (!lock_s) {
		printf("lock state is NULL\n");
		lock_s = "10101000";
		env_set("lock", "10101000");
		run_command("defenv_reserv; saveenv;", 0);
	}
	printf("lock state: %s\n", lock_s);

	info.version_major = (int)(lock_s[0] - '0');
	info.version_minor = (int)(lock_s[1] - '0');
	info.unlock_ability = (int)(lock_s[2] - '0');
	info.lock_state = (int)(lock_s[4] - '0');
	info.lock_critical_state = (int)(lock_s[5] - '0');
	info.lock_bootloader = (int)(lock_s[6] - '0');

	if (info.lock_state == 1 || info.lock_critical_state == 1)
		return 1;
	else
		return 0;
}

/**
 *set merge status
*/
int set_mergestatus_cancel(struct misc_virtual_ab_message *message)
{
	char *partition = "misc";
	char vab_buf[1024] = {0};

	if (store_read((const char *)partition,
		SYSTEM_SPACE_OFFSET_IN_MISC, 1024, (unsigned char *)vab_buf) < 0) {
		printf("failed to store read %s.\n", partition);
		return -1;
	}

	memcpy(message, vab_buf, sizeof(struct misc_virtual_ab_message));
	printf("message.merge_status: %d\n", message->merge_status);
	if (message->merge_status == SNAPSHOTTED || message->merge_status == MERGING) {
		message->merge_status = CANCELLED;
		printf("set message.merge_status CANCELLED\n");
	}
	store_write((const char *)partition, SYSTEM_SPACE_OFFSET_IN_MISC, 1024, (unsigned char *)vab_buf);
	return 0;
}

/**
 * fastboot_set_reboot_flag() - Set flag to indicate reboot-bootloader
 *
 * Set flag which indicates that we should reboot into the bootloader
 * following the reboot that fastboot executes after this function.
 *
 * This function should be overridden in your board file with one
 * which sets whatever flag your board specific Android bootloader flow
 * requires in order to re-enter the bootloader.
 */
int __weak fastboot_set_reboot_flag(void)
{
	return -ENOSYS;
}

/**
 * fastboot_get_progress_callback() - Return progress callback
 *
 * Return: Pointer to function called during long operations
 */
void (*fastboot_get_progress_callback(void))(const char *)
{
	return fastboot_progress_callback;
}

/**
 * fastboot_boot() - Execute fastboot boot command
 *
 * If ${fastboot_bootcmd} is set, run that command to execute the boot
 * process, if that returns, then exit the fastboot server and return
 * control to the caller.
 *
 * Otherwise execute "bootm <fastboot_buf_addr>", if that fails, reset
 * the board.
 */
void fastboot_boot(void)
{
	char *s;

	s = env_get("fastboot_bootcmd");
	if (s) {
		run_command(s, CMD_FLAG_ENV);
	} else {
		static char boot_addr_start[24];
		static char *const bootm_args[] = {
			"bootm", boot_addr_start, NULL
		};

		snprintf(boot_addr_start, sizeof(boot_addr_start) - 1,
			 "0x%p", fastboot_buf_addr);
		printf("Booting kernel at %s...\n\n\n", boot_addr_start);

		do_bootm(NULL, 0, 2, bootm_args);

		/*
		 * This only happens if image is somehow faulty so we start
		 * over. We deliberately leave this policy to the invocation
		 * of fastbootcmd if that's what's being run
		 */
		do_reset(NULL, 0, 0, NULL);
	}
}

/**
 * fastboot_set_progress_callback() - set progress callback
 *
 * @progress: Pointer to progress callback
 *
 * Set a callback which is invoked periodically during long running operations
 * (flash and erase). This can be used (for example) by the UDP transport to
 * send INFO responses to keep the client alive whilst those commands are
 * executing.
 */
void fastboot_set_progress_callback(void (*progress)(const char *msg))
{
	fastboot_progress_callback = progress;
}

/*
 * fastboot_init() - initialise new fastboot protocol session
 *
 * @buf_addr: Pointer to download buffer, or NULL for default
 * @buf_size: Size of download buffer, or zero for default
 */
void fastboot_init(void *buf_addr, u32 buf_size)
{
	fastboot_buf_addr = buf_addr ? buf_addr :
				       (void *)CONFIG_FASTBOOT_BUF_ADDR;
	fastboot_buf_size = buf_size ? buf_size : CONFIG_FASTBOOT_BUF_SIZE;

#if defined CONFIG_FASTBOOT_MAX_DOWN_SIZE
	if (fastboot_buf_size > CONFIG_FASTBOOT_MAX_DOWN_SIZE)
		fastboot_buf_size = CONFIG_FASTBOOT_MAX_DOWN_SIZE;
#endif

	fastboot_set_progress_callback(NULL);
}
