/*
 * IntTest.c
 *
 *  Created on: 2018Äê10ÔÂ17ÈÕ
 *      Author: danialxie
 *
 *        This is an PIC interrupt nesting test for N200 SOC, NUCLEI, Inc.
 *      Attention:
 *         This test need hardware board supporting.
 * 	       A GPIO interrupt is stimulated by another GPIO using wire connection.
 * 	       Wire connection:
 * 	          Source  --> Target
 * 	          GPIO 0  --> GPIO 8
 * 	          GPIO 1  --> GPIO 9
 * 	          GPIO 2  --> GPIO 10
 * 	          GPIO 3  --> GPIO 11
 * 	          GPIO 4  --> GPIO 12
 * 	          GPIO 5  --> GPIO 13
 * 	          GPIO 6  --> GPIO 14
 * 	          GPIO 7  --> GPIO 15
 */

/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "n200_pic_tmr.h"
#include "n200_func.h"
#include "uart.h"
#include "common.h"
#include "mailbox-api.h"
#include "version.h"
#include "hdmi_cec.h"
#include "stick_mem.h"
#include "eth.h"
#define INT_TEST_NEST_DEPTH  6
#define INT_TEST_GPIO_NUM  6
#define INT_TEST_TASK_DELAY  50 // ms
#define TASK_TEST_STACK_DEPTH  200

#define TASK_TEST_QUEUE_NUM  2
#define TASK_TEST_QUEUE_LENGTH  3

//#define GPIO_INT_SOURCE(x) (SOC_PIC_INT_GPIO_BASE + x)

/* Configure board type:
 *   Board under test :        SIGNAL_BOARD_ENABLE     0
 *   Signal generation board : SIGNAL_BOARD_ENABLE     1
 */
#define SIGNAL_BOARD_ENABLE       0

#define INT_TEST_INT_WAVE_ENABLE  1

#if INT_TEST_INT_WAVE_ENABLE
    #define INT_TEST_TIMER_PERIOD  500    // ms
    #define INT_TEST_INT_DELAY    10    // ms
#else
    #define INT_TEST_TIMER_PERIOD  500    // ms
    #define INT_TEST_INT_DELAY    0x3ff    // ms
#endif

#define INT_TEST_MAX_TIMER_PERIOD	100 // ms
#define INT_TEST_MIN_TIMER_PERIOD	50 // ms
#define INT_TEST_MUTE_TIMER_PERIOD	200 // ms

/* Interrupt handler */
void GPIOInterruptHandler(uint32_t num, uint32_t priority);
void vApplicationIdleHook( void );


extern void trap_entry(void);
extern void irq_entry(void);


/* Binary Semaphore */
QueueHandle_t xGPIOSemaphore[INT_TEST_NEST_DEPTH];
QueueHandle_t xMessageQueue[TASK_TEST_QUEUE_NUM];

/* Timer handle */
#if BRINGUP_TEST
TimerHandle_t xSoftTimer = NULL;
#endif
/*
static void vPrintString(const char* msg)
{
	taskENTER_CRITICAL();

	taskEXIT_CRITICAL();
}
*/
/* function: vPICInit */
static void vPICInit(void) {

	// Disable global interrupter
	clear_csr(mstatus, MSTATUS_MIE);

	// Initialize interrupter handler
	for (int i = 0; i < PIC_NUM_INTERRUPTS; i ++) {
		pic_interrupt_handlers[i] = DefaultInterruptHandler;
	}
#if 0
#if !(SIGNAL_BOARD_ENABLE)

   // Enable GPIO interrupter
	enable_interrupt(GPIO_INT_SOURCE(8),  1, GPIOInterruptHandler8);
	enable_interrupt(GPIO_INT_SOURCE(9),  2, GPIOInterruptHandler9);
	enable_interrupt(GPIO_INT_SOURCE(10),  3, GPIOInterruptHandler10);
	enable_interrupt(GPIO_INT_SOURCE(11),  4, GPIOInterruptHandler11);
	enable_interrupt(GPIO_INT_SOURCE(12),  5, GPIOInterruptHandler12);
	enable_interrupt(GPIO_INT_SOURCE(13),  6, GPIOInterruptHandler13);
	//enable_interrupt(GPIO_INT_SOURCE(14),  7, GPIOInterruptHandler14);
	//enable_interrupt(GPIO_INT_SOURCE(15),  7, GPIOInterruptHandler15);

#endif
#endif
	// Enable global interrupt
	set_csr(mstatus, MSTATUS_MIE);
}

#if BRINGUP_TEST
static void vPrintSystemStatus(TimerHandle_t xTimer) {
	xTimer = xTimer;
	taskENTER_CRITICAL();

//	vUartPuts("\nTimer ...\n");
	taskEXIT_CRITICAL();
}

static void vPrintTask1( void *pvParameters )
{
    /*make compiler happy*/
    pvParameters = pvParameters;

	for ( ;; )
	{
//		vUartPuts("\nvPTask1 %d\n");

		vTaskDelay(pdMS_TO_TICKS(50));
	}
}

static void vPrintTask2( void *pvParameters )
{
    /*make compiler happy*/
    pvParameters = pvParameters;
	vTaskDelay(pdMS_TO_TICKS(50));
	for ( ;; )
	{
//		vUartPuts("\nvPTask2 %d\n");
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}
#endif
extern void vMbInit(void);
extern void vCoreFsmIdleInit(void);
extern void vRtcInit(void);
extern void create_str_task(void);
// Test target board
int main(void)
{
	vUartPuts("Starting AOCPU FreeRTOS\n");
	version();

	// Initialize GPIOs, PIC and timer
	//vGPIOInit();
	vPICInit();
	stick_mem_init();
	//write watchdog flag
	stick_mem_write(STICK_REBOOT_FLAG, 0xd);

	// Delay
	for (uint32_t i = 0; i < 0xffff; ++i);

	vMbInit();
	vCoreFsmIdleInit();
	// Create timer
#if BRINGUP_TEST
	xSoftTimer = xTimerCreate("Timer", pdMS_TO_TICKS(INT_TEST_TIMER_PERIOD), pdTRUE, NULL, vPrintSystemStatus);

	vUartPuts("Starting timer ...\n");
	xTimerStart(xSoftTimer, 0);

	xTaskCreate( vPrintTask1, "Print1", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
	xTaskCreate( vPrintTask2, "Print2", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
#endif

	vCecCallbackInit(CEC_CHIP_T5);
	write_csr(mtvec, &trap_entry);
	write_csr_mivec(&irq_entry);

	vRtcInit();
	vETHMailboxCallback();
	create_str_task();

	vUartPuts("Starting task scheduler ...\n");
	vTaskStartScheduler();

	do {}while(1);

	return 0;
}

void vApplicationIdleHook( void )
{
   //vPrintString("enter idle task\n");

   //write_csr(mie, 1); // open mstatue.mie
   //asm volatile ("wfi"); // enter low power mode
}
/*-----------------------------------------------------------*/
void vApplicationMallocFailedHook( void );

void vApplicationMallocFailedHook( void )
{
    /* The malloc failed hook is enabled by setting
    configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

    Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues, software
    timers, and semaphores.  The size of the FreeRTOS heap is set by the
    configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	//write(1,"malloc failed\n", 14);

	vUartPuts("vApplicationMallocFailedHook\n");
	vPrintFreeListAfterMallocFail();
    for ( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName );

void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) xTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected.  pxCurrentTCB can be
    inspected in the debugger if the task name passed into this function is
    corrupt. */
    //write(1, "Stack Overflow\n", 15);
    for ( ;; );
}
/*-----------------------------------------------------------*/
