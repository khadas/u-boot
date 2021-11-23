#include <string.h>
#include "FreeRTOS.h"
#include "suspend.h"
#include "task.h"
#include "gpio.h"
#include "tpwake.h"

#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */

void Tp_IRQHandle(void)
{
	uint32_t buf[4] = {0};

	INFO("tp resume");
	vDisableGpioIRQ(TP_WAKE_HOST);
	if (!xGpioGetValue(TP_WAKE_HOST)) {
		buf[0] = TP_WAKEUP;
		INFO("power key");
		STR_Wakeup_src_Queue_Send_FromISR(buf);
	}
}

void Tp_GpioIRQRegister(void)
{
	INFO();
	xGpioSetDir(TP_WAKE_HOST, GPIO_DIR_IN);
	xRequestGpioIRQ(TP_WAKE_HOST, Tp_IRQHandle, IRQF_TRIGGER_FALLING);
}

void Tp_GpioIRQFree(void)
{
	vFreeGpioIRQ(TP_WAKE_HOST);
}


