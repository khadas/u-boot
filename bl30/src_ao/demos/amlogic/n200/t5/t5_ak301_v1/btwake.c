#include <string.h>
#include "FreeRTOS.h"
#include "suspend.h"
#include "task.h"
#include "gpio.h"

#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */

#define UNUSED(x) ((void)x)
#define BTGPIO GPIOB_13  //bt_wake_host pin
#define INFO(fmt, args...) printf("[%s] " fmt "\n", __func__, ##args)
void Bt_GpioIRQRegister(void);
void Bt_GpioIRQFree(void);

static void Bt_IRQHandle(void)
{
	uint32_t buf[4] = {0};

	INFO("bt resume");
	vDisableGpioIRQ(BTGPIO);
	if (!xGpioGetValue(BTGPIO)) {
		buf[0] = BT_WAKEUP;
		INFO("power key");
	}
	STR_Wakeup_src_Queue_Send_FromISR(buf);
}

void Bt_GpioIRQRegister(void)
{
	INFO();
	xGpioSetDir(BTGPIO, GPIO_DIR_IN);
	xRequestGpioIRQ(BTGPIO, Bt_IRQHandle, IRQF_TRIGGER_FALLING);
}

void Bt_GpioIRQFree(void)
{
	vFreeGpioIRQ(BTGPIO);
}


