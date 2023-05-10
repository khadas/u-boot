// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (C) 2014-2022 Amlogic, Inc. All rights reserved.
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

#include <command.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <amlogic/pm.h>
#include <asm/arch/bl31_apis.h>
#include <stdio.h>
#include <malloc.h>

#define PM_SUSPEND_ON		(0)
#define PM_SUSPEND_TO_IDLE	(1)
#define PM_RESUME	(2)
#define PM_SUSPEND_MEM		(3)
#define PM_SUSPEND_MIN		PM_SUSPEND_TO_IDLE
#define PM_SUSPEND_MAX		(4)

#define PM_DEBUG

#ifdef PM_DEBUG
#define debug(fmt, args...)	printf(fmt, ##args)
#else
#define debug(fmt, args...)
#endif /* PM_DEBUG */

static struct list_head devices;

#ifdef PM_DEBUG
static int test_pm_suspend(void *dev_info)
{
	printf("%s\n", __func__);
	return 0;
}

static int test_pm_resume(void *dev_info)
{
	printf("%s\n", __func__);
	return 0;
}

static int test_pm_poweroff(void *dev_info)
{
	printf("%s\n", __func__);
	return 0;
}
#endif

#ifdef PM_DEBUG
struct dev_pm_ops *dev_register_pm(char *name, FUNC_PTR suspend,
			FUNC_PTR resume, FUNC_PTR poweroff);
char name[] = "test_pm_name";
#endif

void pm_initialize(void)
{
	INIT_LIST_HEAD(&devices);
#ifdef PM_DEBUG
	dev_register_pm(name, test_pm_suspend, test_pm_resume, test_pm_poweroff);
#endif
}

extern int cpu_suspend(unsigned long arg);
void pm_suspend(void)
{
	struct list_head *dentry;
	struct dev_pm_ops *pm_ops;

	debug("suspend entry\n");

	list_for_each(dentry, &devices) {
		pm_ops = list_entry(dentry, struct dev_pm_ops, link);
		if (pm_ops->suspend) {
			pm_ops->suspend((void *)pm_ops);
			debug("%s suspend done\n", pm_ops->name);
		} else {
			printf("suspend is not illeage!\n");
		}
	}

	printf("uboot str suspend done!\n");

	cpu_suspend(0);

	list_for_each_prev(dentry, &devices) {
		pm_ops = list_entry(dentry, struct dev_pm_ops, link);
		if (pm_ops->resume) {
			pm_ops->resume((void *)pm_ops);
			debug("%s resume done\n", pm_ops->name);
		} else {
			printf("suspend is not illeage!\n");
		}
	}
	printf("uboot str resume done!\n");
}

void pm_poweroff(void)
{
	struct list_head *dentry;
	struct dev_pm_ops *pm_ops;

	debug("poweroff entry\n");

	list_for_each(dentry, &devices) {
		pm_ops = list_entry(dentry, struct dev_pm_ops, link);
		if (pm_ops->poweroff) {
			pm_ops->poweroff((void *)pm_ops);
			debug("%s poweroff done\n", pm_ops->name);
		} else {
			printf("poweroff is not illeage!\n");
		}
	}
	printf("uboot poweroff done!\n");
	aml_system_off();
}

struct dev_pm_ops *dev_register_pm(char *name, FUNC_PTR suspend,
			FUNC_PTR resume, FUNC_PTR poweroff)
{
	struct dev_pm_ops *ops;

	if (!name || !suspend || !resume || !poweroff) {
		printf("Invalid pass parameters\n");
		return NULL;
	}

	ops = (struct dev_pm_ops *)malloc((size_t)sizeof(struct dev_pm_ops));

	if (!ops) {
		printf("Wakeup source malloc fail!!\n");
		return NULL;
	}

	ops->name = name;
	ops->suspend = suspend;
	ops->resume = resume;
	ops->poweroff = poweroff;

	list_add_tail(&ops->link, &devices);

	return ops;
}

int dev_unregister_pm(struct dev_pm_ops *arg)
{
	struct dev_pm_ops *ops = arg;

	if (!ops) {
		printf("Invalid parameters\n");
		return -1;
	}

	list_del(&ops->link);

	free(ops);
	ops = NULL;
	return 0;
}
