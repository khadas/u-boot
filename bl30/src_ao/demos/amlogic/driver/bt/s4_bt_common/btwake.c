#include <string.h>
#include "FreeRTOS.h"
#include "suspend.h"
#include "task.h"
#include "gpio.h"
#include "btwake.h"

#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */

static TaskHandle_t btTask = NULL;

static void bt_wakeup_Task(void *args)
{
	uint32_t buf[4] = {0};
	int cnt = 0;
	int flag_p = 0;
	int flag_n = 0;

restart_task:
	UNUSED(args);
	INFO();
	vTaskSuspend(btTask);
	vTaskDelay(pdMS_TO_TICKS(100));
	if (!xGpioGetValue(BT_WAKE_HOST)) {
		vTaskDelay(pdMS_TO_TICKS(100));
		do {
			vTaskDelay(pdMS_TO_TICKS(20));
			if (xGpioGetValue(BT_WAKE_HOST)) {
				flag_p++;
			}
			else if (!xGpioGetValue(BT_WAKE_HOST)) {
				flag_n++;
			}
			cnt++;
		} while (cnt < 10);

		if (flag_p >= 7) {
			INFO("power key: %d", flag_p);
			buf[0] = BT_WAKEUP;
		}
		else if (flag_n >= 7) {
			vTaskDelay(pdMS_TO_TICKS(120));
			if (!xGpioGetValue(BT_WAKE_HOST)) {
				INFO("suspend any key");
				buf[0] = BT_WAKEUP;
			}
			else {
				INFO("netflix key: %d", flag_n);
				buf[0] = REMOTE_CUS_WAKEUP;
			}
		}
		else
		{
			INFO("flag_p= %d, flag_n = %d",flag_p, flag_n);
			buf[0] = BT_WAKEUP;
		}

	}
	if (buf[0] != 0) {
		STR_Wakeup_src_Queue_Send(buf);
		while (1) {
			vTaskSuspend(NULL);
			INFO("suspend null");
		}
	}
	vEnableGpioIRQ(BT_WAKE_HOST);
	goto restart_task;
}

static void vBTWakeup(void)
{
	vDisableGpioIRQ(BT_WAKE_HOST);
	xTaskResumeFromISR(btTask);
}

static void bt_task_create(void)
{
	int ret = 0;
	ret = xTaskCreate(bt_wakeup_Task, "bt_wakeup", configMINIMAL_STACK_SIZE, NULL, 2, &btTask);
	if (ret != pdPASS)
		INFO("bt_wakeup_Task create failed!");
}

void bt_task_init(void)
{
	bt_task_create();
	xGpioSetDir(BT_WAKE_HOST, GPIO_DIR_IN);
	xRequestGpioIRQ(BT_WAKE_HOST, vBTWakeup, IRQF_TRIGGER_LOW);
}

void bt_task_deinit(void)
{
	if (btTask != NULL)
	{
		INFO("deinit");
		vTaskDelete(btTask);
		btTask = NULL;
	}
	vFreeGpioIRQ(BT_WAKE_HOST);
}

