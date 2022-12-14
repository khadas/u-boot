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
#define MTAG	"pm"
#include "pm.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "common.h"
#include "myprintf.h"
#include "queue.h"
#include "semphr.h"
#include "soc.h"
#include "wakeup.h"

#define PM_SUSPEND_ON		(0)
#define PM_SUSPEND_TO_IDLE	(1)
#define PM_RESUME	(2)
#define PM_SUSPEND_MEM		(3)
#define PM_SUSPEND_MIN		PM_SUSPEND_TO_IDLE
#define PM_SUSPEND_MAX		(4)

#define malloc pvPortMalloc
#define free vPortFree

#define mutex_lock(x)	xSemaphoreTake((x), portMAX_DELAY)
#define mutex_unlock(x) xSemaphoreGive((x))

static List_t pm_dev;

struct wakeup_source {
	ListItem_t list_item;
	char *name;
	uint32_t wake_count;
	uint32_t need_wakeup:1;
	void *data;
	struct dev_power_ops * ops;
};

void pm_main(void *arg);
static SemaphoreHandle_t xPM_wakeup_sem = NULL;
static uint32_t system_status = PM_SUSPEND_ON;
static int default_pm(void);


static struct platform_power_ops * platform_pm_ops =
	&(struct platform_power_ops){
		.begin = default_pm,
		.end = default_pm,
};

static int default_pm(void)
{
	logi("%s\n", __func__);
	return 0;
}

static struct wakeup_source * active_ws;

static QueueHandle_t pm_list_lock;
static TaskHandle_t pm_main_handle;
static struct wakeup_source * kernel_ws;
/*
 * System suspend/freeze event processing therad
 * */

extern void platform_power_interface_register(void);

static int virtual_kernel_ws_enter(void *arg)
{
	(void)arg;
	logi("%s\n", __func__);
	return 0;
}

static int virtual_kernel_ws_restore(void *arg)
{
	(void)arg;
	logi("%s\n", __func__);
	return 0;
}

static struct dev_power_ops ws_kernel_ops = {
	.enter = virtual_kernel_ws_enter,
	.restore = virtual_kernel_ws_restore,
};

void pm_main(void *arg)
{
	(void) arg;
	int i;
	struct wakeup_source *ws;

	xPM_wakeup_sem = xSemaphoreCreateBinary();
	configASSERT(xPM_wakeup_sem);

	logi(" Ready to startup power manage\n");

	platform_power_interface_register();

	kernel_ws = dev_register_ws("kernel", &ws_kernel_ops, NULL, WS_NEED_NOT_WAKEUP);
	for (;;) {
		system_status = PM_SUSPEND_ON;;
		logi("Ready to wait\n");
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		logi("Entry freeze\n");
		taskENTER_CRITICAL();
		active_ws = NULL;
		taskEXIT_CRITICAL();
		system_status = PM_SUSPEND_TO_IDLE;
		mutex_lock(pm_list_lock);
		for (i = 0; i < (int)listCURRENT_LIST_LENGTH(&pm_dev); i++) {
			listGET_OWNER_OF_NEXT_ENTRY(ws, &pm_dev);
			if (ws && ws->ops->enter)
				ws->ops->enter(ws);
		}
		mutex_unlock(pm_list_lock);
		platform_pm_ops->begin();

		xSemaphoreTake(xPM_wakeup_sem, portMAX_DELAY);
		logi("Resume freeze\n");

		platform_pm_ops->end();
		mutex_lock(pm_list_lock);
		for (i = 0; i < (int)listCURRENT_LIST_LENGTH(&pm_dev); i++) {
			listGET_OWNER_OF_PRE_ENTRY(ws, &pm_dev);
			if (ws && ws->ops->restore)
				ws->ops->restore(ws);
		}
		mutex_unlock(pm_list_lock);

		logi("AOCPU already resume done.\n");

		if (active_ws) {
			if (active_ws->need_wakeup) {
				logi("ready to wakeup ap\n");
				wakeup_ap();
			}
		} else {
			loge("Error occurred, should never get here.\n");
		}
	}
}

static int os_is_running(void)
{
	return system_status == PM_SUSPEND_ON;
}

extern uint32_t _module_pm_begin;
extern uint32_t _module_pm_end;
int find_static_power_dev(void)
{
	initcall_t * p = (void *)&_module_pm_begin;
	vListInitialise(&pm_dev);

	pm_list_lock = xSemaphoreCreateMutex();

	if (xTaskCreate( pm_main, "pm_task", configMINIMAL_STACK_SIZE, NULL, 3, &pm_main_handle ) < 0)
		loge("STR_task create fail!!\n");

	for (; p < (initcall_t *)&_module_pm_end; ) {
		int (* fn)(void) = (*p++);
		if (fn)
			fn();
	}
	return 0;
}

int pm_enter(void)
{
	xTaskNotifyGive(pm_main_handle);
	return 0;
}

struct wakeup_source * dev_register_ws(char * name, struct dev_power_ops *ops, void *data, uint32_t need_wakeup_flag)
{
	struct wakeup_source * ws;

	if (!name || !ops) {
		loge("Invalid pass parameters\n");
		return NULL;
	}

	ws = (struct wakeup_source *)malloc((size_t)sizeof(struct wakeup_source));

	if (!ws) {
		loge("Wakeup source malloc fail!!\n");
		return NULL;
	}

	ws->name = name;
	ws->ops = ops;
	ws->data = data;
	ws->need_wakeup = !!need_wakeup_flag;
	ws->list_item.pvOwner = (void *)ws;

	mutex_lock(pm_list_lock);
	vListInsert(&pm_dev, &ws->list_item);
	mutex_unlock(pm_list_lock);

	return ws;
}

int dev_unregister_ws(ws_t  arg)
{
	struct wakeup_source *ws = arg;
	uint32_t ret;

	if (!ws) {
		loge("Invalid parameters\n");
		return -1;
	}

	ret = uxListRemove(&(ws->list_item));
	(void)ret;

	free(ws);
	ws = NULL;
	return 0;
}

int pm_wake_up(ws_t arg)
{
	struct wakeup_source *ws = arg;
	if (!ws) {
		loge("Invalid parameters\n");
		return -1;
	}

	if (!listIS_CONTAINED_WITHIN(&pm_dev, &ws->list_item)) {
		loge("Invalid wakeup source.\n");
		return -1;
	}

	if (active_ws) {
		loge("System is ready to wake up, source is %s\n",
				active_ws->name);
		return -1;
	}

	taskENTER_CRITICAL();
	active_ws = ws;
	taskEXIT_CRITICAL();

	ws->wake_count ++;
	xSemaphoreGive(xPM_wakeup_sem);
	logi("Activation source received:%s\n", ws->name);
	return 0;
}

int set_platform_power_ops(struct platform_power_ops *ops)
{
	if (!ops) {
		loge("Invalid parameters\n");
		return -1;
	}

	platform_pm_ops = ops;
	return 0;
}

/*
 * Wakeup 30 interface, only used for wakeup events sent by the kernel.
 * */
void wakeup_ap_from_kernel(void)
{
	logi("ARM sync with 30...\n");
	pm_wake_up(kernel_ws);
	/* need sync*/
	while (!os_is_running()) {
		logi("ARM sync with 30...\n");
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
