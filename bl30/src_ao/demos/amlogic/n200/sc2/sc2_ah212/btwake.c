#include <string.h>
#include "FreeRTOS.h"
#include "suspend.h"
#include "task.h"
#include "gpio.h"
#include "btwake.h"

#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */

#define UNUSED(x) ((void)x)

static TaskHandle_t btTask = NULL;
static int8_t btTaskCreated;

static void bt_wakeup_Task(void *args)
{
	uint32_t buf[4] = {0};
	int cnt = 0;
	int flag_p = 0;
	int flag_n = 0;

	UNUSED(args);
	printf("bt_wakeup_Task\n");
	vTaskSuspend(btTask);
	vTaskDelay(pdMS_TO_TICKS(100));
	if (!xGpioGetValue(GPIOX_18)) {
		vDisableGpioIRQ(GPIOX_18);
		//printf("bt_wakeup_Task 1\n");
		vTaskDelay(pdMS_TO_TICKS(100));
		do {
			vTaskDelay(pdMS_TO_TICKS(20));
			if (xGpioGetValue(GPIOX_18)) {
				flag_p++;
			}
			else if (!xGpioGetValue(GPIOX_18)) {
				flag_n++;
			}
			cnt++;
		} while (cnt < 10);

		if (flag_p >= 7) {
			printf("power key\n");
			buf[0] = BT_WAKEUP;
		}
		else if (flag_n >= 7) {
			vTaskDelay(pdMS_TO_TICKS(120));
			if (!xGpioGetValue(GPIOX_18)) {
				buf[0] = BT_WAKEUP;
			}
			else {
				printf("netflix key\n");
				buf[0] = REMOTE_CUS_WAKEUP;
			}
		}

	}
	if (buf[0] != 0) {
		STR_Wakeup_src_Queue_Send(buf);
		while (1) {
			vTaskSuspend(NULL);
			printf("suspend null\n");
		}
	}

};

static void vBTWakeup(void)
{
	//printf("bt_wakeup task Resume!\n");
	xTaskResumeFromISR(btTask);
	vDisableGpioIRQ(GPIOX_18);
};

static void bt_task_create(void)
{
	int ret = 0;
	ret=xTaskCreate( bt_wakeup_Task, "bt_wakeup", configMINIMAL_STACK_SIZE, NULL, 2, &btTask );
	if (ret != pdPASS)
		printf("bt_wakeup_Task create failed!\n");
	else
		btTaskCreated = 1;
};

void bt_task_start(void)
{
	bt_task_create();

	xGpioSetDir(GPIOX_18, GPIO_DIR_IN);
	xRequestGpioIRQ(GPIOX_18, vBTWakeup, IRQF_TRIGGER_FALLING);
#ifdef CONFIG_YOCTO
	/*  In case gpio is pull low already, irq might won't triggered,
		wake bt-task directly */
	if (!xGpioGetValue(GPIOX_18)) {
		printf("bt wake pin is already low, wake up soc\n");
		vTaskDelay(pdMS_TO_TICKS(100));
		vTaskResume(btTask);
		return;
	}
#endif
}

void bt_task_disable(void)
{
	vTaskDelete(btTask);
	btTaskCreated = 0;
	btTask = NULL;
	vFreeGpioIRQ(GPIOX_18);
}
