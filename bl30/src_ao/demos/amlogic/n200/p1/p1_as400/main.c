/*
 * Copyright (C) 2014-2018 Amlogic, Inc. All rights reserved.
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

/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */
#include "myprintf.h"

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <unistd.h>

#include "n200_eclic.h"
#include "n200_func.h"
#include "uart.h"
#include "common.h"

#include "riscv_encoding.h"

#include "hdmi_cec.h"
#include "suspend.h"
#include "vrtc.h"
#include "mailbox-api.h"
#include "version.h"

//#include "printf.h"
#define INT_TEST_NEST_DEPTH  6
#define INT_TEST_GPIO_NUM  6
#define INT_TEST_TASK_DELAY  50 // ms
#define TASK_TEST_STACK_DEPTH  200

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
void DefaultInterruptHandler(void);
void GPIOInterruptHandler( uint32_t num, uint32_t priority );
void vApplicationIdleHook( void );
void config_eclic_irqs ( void );
void vApplicationIdleHook( void );
void vApplicationMallocFailedHook( void );
void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName );
void power_on_off_cpu(void);

extern void create_str_task(void);

/* Timer handle */
TimerHandle_t xSoftTimer = NULL;

void config_eclic_irqs (void)
{
	eclic_init (ECLIC_NUM_INTERRUPTS);
	//time_init   in port.c
	eclic_set_nlbits(0);
}

void DefaultInterruptHandler(void){}

static void vPrintSystemStatus(TimerHandle_t xTimer) {
	xTimer = xTimer;
	taskENTER_CRITICAL();
//	printf("\nTimer ...\r\n");
	taskEXIT_CRITICAL();
}

static void vPrintTask1( void *pvParameters )
{
    /*make compiler happy*/
	pvParameters = pvParameters;

	for ( ;; )
	{
	//	printf("\nvPTask1 tick=%d\n",(unsigned int)xTaskGetTickCount());
		vTaskDelay(pdMS_TO_TICKS(10000));
		//dump_fsm();
	}
}

static void vPrintTask2( void *pvParameters )
{
    /*make compiler happy*/
    pvParameters = pvParameters;
	vTaskDelay(pdMS_TO_TICKS(500));
	for ( ;; )
	{
	//	printf("\nvPTask2 tick=%d\n",(unsigned int)xTaskGetTickCount());
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void hardware_init(void);
void hardware_init()
{
	config_eclic_irqs();
}

// Test target board
int main(void)
{
	uint32_t i;
	//init_printf(NULL, stdout_putf);

	hardware_init();

	printf("\n[ao]Starting AOCPU FreeRTOS...\r\n");
	printf("\n[ao]Current board p1_as400...\r\n");
	version();

	// Initialize GPIOs, PIC and timer
	//vGPIOInit();

	// Delay
	for (i = 0; i < 4; ++i)
		printf("AOCPU_IRQ_SEL=0x%x\n",REG32(AOCPU_IRQ_SEL0 + i*4));

	vMbInit();


	// Create timer
	xSoftTimer = xTimerCreate("Timer", pdMS_TO_TICKS(5000), pdTRUE, NULL, vPrintSystemStatus);

	printf("Starting timer ...\r\n");
	xTimerStart(xSoftTimer, 0);

	xTaskCreate( vPrintTask1, "Print1", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
	xTaskCreate( vPrintTask2, "Print2", configMINIMAL_STACK_SIZE, NULL, 2, NULL );

	//vCecCallbackInit(CEC_CHIP_T7);
	vRtcInit();
	create_str_task();

	printf("Starting task scheduler ...\r\n");
	vTaskStartScheduler();

	do {}while(1);

	return 0;
}

void vApplicationIdleHook( void )
{
   //printf("enter idle task\n");

   //write_csr(mie, 1); // open mstatue.mie
   asm volatile ("wfi"); // enter low power mode
}
/*-----------------------------------------------------------*/

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

	printf("vApplicationMallocFailedHook\n");
    for ( ;; );
}
/*-----------------------------------------------------------*/

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
	printf("vApplicationStackOverflowHook\n");
	vTaskDumpStack(NULL);
    for ( ;; );
}
/*-----------------------------------------------------------*/
