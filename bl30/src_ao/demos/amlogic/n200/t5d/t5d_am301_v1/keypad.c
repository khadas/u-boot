#include <string.h>
#include "FreeRTOS.h"
#include "ir.h"
#include "keypad.h"
#include "gpio.h"
#include "saradc.h"
#include "suspend.h"

/*KEY ID*/
#define ADC_KEY_ID_HOME		520

static void vAdcKeyCallBack(struct xReportEvent event)
{
	uint32_t buf[4] = {0};

	switch (event.ulCode) {
	case ADC_KEY_ID_HOME:
		buf[0] = POWER_KEY_WAKEUP;
		STR_Wakeup_src_Queue_Send(buf);
		break;
	default:
		break;
	}

	printf("ADC key event 0x%x, key code %d, responseTicks %d\n",
		event.event, event.ulCode, event.responseTime);
}

struct xAdcKeyInfo adcKeyInfo[] = {
	ADC_KEY_INFO(ADC_KEY_ID_HOME, 0, SARADC_CH1,
		     EVENT_SHORT,
		     vAdcKeyCallBack, NULL)
};

void vKeyPadCreate(void)
{
	vCreateAdcKey(adcKeyInfo,
			sizeof(adcKeyInfo)/sizeof(struct xAdcKeyInfo));

	vDynamicKeypadInit();
}

void vKeyPadInit(void)
{
	if (vAdcKeyIsEmpty())
		vCreateAdcKey(adcKeyInfo,
				sizeof(adcKeyInfo)/sizeof(struct xAdcKeyInfo));

	vAdcKeyEnable();
	vGpioKeyEnable();
}

void vKeyPadDeinit(void)
{
	vAdcKeyDisable();
	vGpioKeyDisable();
}
