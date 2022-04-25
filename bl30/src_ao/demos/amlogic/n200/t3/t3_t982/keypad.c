#define MTAG "keypad"
#include <string.h>
#include "FreeRTOS.h"
#include "ir.h"
#include "keypad.h"
#include "gpio.h"
#include "saradc.h"
#include "suspend.h"
#include "n200_func.h"
#include "wakeup.h"
#include "common.h"
#include "pm.h"

/*KEY ID*/
#define GPIO_KEY_ID_POWER	GPIOD_3

#define ADC_KEY_ID_MENU		520
#define ADC_KEY_ID_VOL_DEC	521
#define ADC_KEY_ID_VOL_INC	522
#define ADC_KEY_ID_ESC		523
#define ADC_KEY_ID_HOME		524

extern uint32_t suspend_flag;
static ws_t keypad_ws;
static void vGpioKeyCallBack(struct xReportEvent event)
{
	uint32_t buf[4] = {0};

	switch (event.ulCode) {
	case GPIO_KEY_ID_POWER:
		buf[0] = POWER_KEY_WAKEUP;
		STR_Wakeup_src_Queue_Send_FromISR(buf);
		break;
	default:
		break;
	}

	printf("GPIO key event 0x%x, key code %d, responseTicks %d\n",
		event.event, event.ulCode, event.responseTime);
}

extern void set_reason_flag(char exit_reason);

static void vAdcKeyCallBack(struct xReportEvent event)
{
	uint32_t buf[4] = {0};

	switch (event.ulCode) {
	case ADC_KEY_ID_MENU:
		if (suspend_flag) {
			buf[0] = POWER_KEY_WAKEUP;
			STR_Wakeup_src_Queue_Send_FromISR(buf);
		} else {
			//vAdcKeyDisable();
			//vDestoryAdcKey();
			printf("adc key wakeup ap\n");
			//wakeup_ap();
			set_reason_flag(POWER_KEY_WAKEUP);
			pm_wake_up(keypad_ws);
		}
		break;
	default:
		break;
	}

	printf("ADC key event 0x%x, key code %d, responseTime %d\n",
		event.event, event.ulCode, event.responseTime);
}

struct xGpioKeyInfo gpioKeyInfo[] = {
	GPIO_KEY_INFO(GPIO_KEY_ID_POWER, HIGH, EVENT_SHORT,
			vGpioKeyCallBack, NULL)
};

struct xAdcKeyInfo adcKeyInfo[] = {
	ADC_KEY_INFO(ADC_KEY_ID_MENU, 0, SARADC_CH1,
		     EVENT_SHORT,
		     vAdcKeyCallBack, NULL),
	ADC_KEY_INFO(ADC_KEY_ID_VOL_DEC, 574, SARADC_CH1,
		     EVENT_SHORT,
		     vAdcKeyCallBack, NULL),
	ADC_KEY_INFO(ADC_KEY_ID_VOL_INC, 1065, SARADC_CH1,
		     EVENT_SHORT,
		     vAdcKeyCallBack, NULL),
	ADC_KEY_INFO(ADC_KEY_ID_ESC, 1557, SARADC_CH1,
		     EVENT_SHORT | EVENT_LONG,
		     vAdcKeyCallBack, NULL),
	ADC_KEY_INFO(ADC_KEY_ID_HOME, 2048, SARADC_CH1,
		     EVENT_SHORT,
		     vAdcKeyCallBack, NULL)
};

void vKeyPadInit(void)
{
	if (vAdcKeyIsEmpty())
		vCreateAdcKey(adcKeyInfo,
				sizeof(adcKeyInfo)/sizeof(struct xAdcKeyInfo));
	vAdcKeyEnable();
}

void vKeyPadDeinit(void)
{
	vAdcKeyDisable();
	//vDestoryAdcKey();
}

static int keypad_enter(void *arg)
{
	(void) arg;
	logi("keypad enter\n");
	vKeyPadInit();
	return 0;
}

static int keypad_restore(void *arg)
{
	(void) arg;
	logi("keypad restore\n");
	vKeyPadDeinit();
	return 0;
}

static struct dev_power_ops ops = {
	.enter = keypad_enter,
	.restore = keypad_restore,
};

static int keypad_init(void)
{
	logi("Entry %s\n", __func__);
	keypad_ws = dev_register_ws(MTAG, &ops, NULL, WS_NEED_WAKEUP);
	return 0;
}

device_initcall(keypad_init)
