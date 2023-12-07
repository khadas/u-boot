#include <stdbool.h>
#include <string.h>

#include "FreeRTOS.h"
#include "suspend.h"
#include "task.h"
#include "gpio.h"

#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */
#include "wifi_bt_wake.h"

/* debug log switch */
#define DBG_IO false

#ifndef UNUSED
#define UNUSED(x) ((void)x)
#endif

#ifndef INFO
#define INFO(fmt, args...) printf("[%s] " fmt "\n", __func__, ##args)
#endif

#ifndef DBG
#define DBG(fmt, args...) {if (DBG_IO) {printf("[%s] " fmt "\n", __func__, ##args);}}
#endif

#define DELAY_INTERVAL 40

#ifdef WIFI_WAKE_CFG

#ifndef WIFI_WAKE_HOST
#define WIFI_WAKE_HOST GPIOX_7
#endif

#ifndef WIFI_PWREN
#define WIFI_PWREN GPIOX_6
#endif

#endif

#ifdef BT_WAKE_CFG

#ifndef BT_WAKE_HOST
#define BT_WAKE_HOST GPIOX_18
#endif

#ifndef BT_EN
#define BT_EN GPIOX_17
#endif

#endif

#if defined(WIFI_WAKE_CFG) || defined(BT_WAKE_CFG)
/* In case of wifi/bt wake host gpio is pulled down already; irq won't be triggered, wakeup directly */
static void wakeup_special_handle(uint16_t gpio_en, uint16_t gpio_wake, uint32_t key_val)
{
	if (xGpioGetValue(gpio_en)) {  //enabled
		if (!xGpioGetValue(gpio_wake)) {
			INFO("already low, wakeup:%u", key_val);
			STR_Wakeup_src_Queue_Send(&key_val);
		}
	}
}

static void wakeup_task_init(TaskFunction_t task_callback, const char * const name, TaskHandle_t * const task)
{
	int32_t ret;

	ret = xTaskCreate(task_callback, name, configMINIMAL_STACK_SIZE, NULL, 3, task);
	if (ret != pdPASS) {
		INFO("%s:create failed", name);
	}

	DBG("%s:create success", name);
}

static void wakeup_task_deinit(TaskHandle_t task, uint16_t gpio)
{
	if (task != NULL) {
		DBG("free irq gpio:%u", gpio);
		vFreeGpioIRQ(gpio);
		vTaskDelete(task);
		task = NULL;
	}
}
#endif

#ifdef WIFI_WAKE_CFG
static TaskHandle_t wifi_task = NULL;

static void vWIFIWakeup(void)
{
	vDisableGpioIRQ(WIFI_WAKE_HOST);
	xTaskResumeFromISR(wifi_task);
};

static void wifi_wakeup_task(void *args)
{
	uint32_t key_val;

	UNUSED(args);
	wakeup_special_handle(WIFI_PWREN, WIFI_WAKE_HOST, WIFI_WAKEUP);
	xGpioSetDir(WIFI_WAKE_HOST, GPIO_DIR_IN);
	xRequestGpioIRQ(WIFI_WAKE_HOST, vWIFIWakeup, IRQF_TRIGGER_FALLING);

	while (1) {
		vTaskSuspend(wifi_task);
		vTaskDelay(pdMS_TO_TICKS(DELAY_INTERVAL));  // waiting fall to low level
		if (!xGpioGetValue(WIFI_WAKE_HOST)) {
			INFO("wifi wakeup");
			key_val = WIFI_WAKEUP;
			STR_Wakeup_src_Queue_Send(&key_val);
		} else {
			INFO("abnormal waveform, redetect");
			vEnableGpioIRQ(WIFI_WAKE_HOST);
		}
	}
}
#endif

#ifdef BT_WAKE_CFG
static TaskHandle_t bt_task = NULL;

static void vBTWakeup(void)
{
	vDisableGpioIRQ(BT_WAKE_HOST);
	xTaskResumeFromISR(bt_task);
}

static void bt_wakeup_task(void *args)
{
	uint32_t key_val, flag;

	UNUSED(args);
	wakeup_special_handle(BT_EN, BT_WAKE_HOST, BT_WAKEUP);
	xGpioSetDir(BT_WAKE_HOST, GPIO_DIR_IN);
	xRequestGpioIRQ(BT_WAKE_HOST, vBTWakeup, IRQF_TRIGGER_FALLING);

	while (1) {
		vTaskSuspend(bt_task);
		flag = 0;

		vTaskDelay(pdMS_TO_TICKS(DELAY_INTERVAL));  // waiting fall to low level
		while (flag < 20) {
			if (!xGpioGetValue(BT_WAKE_HOST)) {  // detected as low level
				flag++;
			} else {
				break;
			}
			vTaskDelay(pdMS_TO_TICKS(DELAY_INTERVAL));
		}

/* power key: 200ms low level; judgment range <=240ms; netflix key: 400ms~500ms low level; judgment range 240~560ms;
correcting clock inaccuracies 2 ticks≈40ms */
		if ((flag >0) &&(flag <= 7)) {
			INFO("power key:%u ms", (flag*DELAY_INTERVAL));
			key_val = BT_WAKEUP;
			STR_Wakeup_src_Queue_Send(&key_val);
		} else if ((flag >7) &&(flag <= 14)) {
			INFO("netflix key:%u ms", (flag*DELAY_INTERVAL));
			key_val = REMOTE_CUS_WAKEUP;
			STR_Wakeup_src_Queue_Send(&key_val);
		} else {
			INFO("unknown key:%u ms, redetect", (flag*DELAY_INTERVAL));
			vEnableGpioIRQ(BT_WAKE_HOST);
		}
	}
}
#endif

void wifi_bt_wakeup_init(void)
{
#ifdef WIFI_WAKE_CFG
	wakeup_task_init(wifi_wakeup_task, "wifi_wakeup", &wifi_task);
#endif

#ifdef BT_WAKE_CFG
	wakeup_task_init(bt_wakeup_task, "bt_wakeup", &bt_task);
#endif
}

void wifi_bt_wakeup_deinit(void)
{
#ifdef WIFI_WAKE_CFG
	wakeup_task_deinit(wifi_task, WIFI_WAKE_HOST);
#endif

#ifdef BT_WAKE_CFG
	wakeup_task_deinit(bt_task, BT_WAKE_HOST);
#endif
}

