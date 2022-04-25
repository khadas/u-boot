/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"



#include <stdio.h>

#include "n200_func.h"
#include "soc.h"
#include "riscv_encoding.h"
#ifdef N200_REVA
//#include "riscv_encoding.h"
#include "n200_pic_tmr.h"
#else
#include "n200_eclic.h"
#endif
#include "n200_timer.h"
#include <riscv_bits.h>


/* Standard Includes */
#include <stdlib.h>
#include <unistd.h>
//#include "printf.h"

/* Each task maintains its own interrupt status in the critical nesting
variable. */
UBaseType_t uxCriticalNesting = 0xaaaaaaaa;

#if USER_MODE_TASKS
	unsigned long MSTATUS_INIT = (MSTATUS_MPIE);
#else
	unsigned long MSTATUS_INIT = (MSTATUS_MPP | MSTATUS_MPIE);
#endif


/*
 * Used to catch tasks that attempt to return from their implementing function.
 */
static void prvTaskExitError( void );
unsigned long ulSynchTrap(unsigned long mcause, unsigned long sp, unsigned long arg1);
#ifdef N200_REVA
uint32_t vPortSysTickHandler(uint32_t int_num);
#else
void vPortSysTickHandler(void);
#endif
void vPortSetupTimer(void);
void vPortSetup(void);
void vPortSysTickHandler_soc(void);

/* System Call Trap */
//ECALL macro stores argument in a2
unsigned long ulSynchTrap(unsigned long mcause, unsigned long sp, unsigned long arg1)	{
	int i = 0;
	uint32_t mstatus_mps_bits;

	switch (mcause&0X00000fff)	{
		//on User and Machine ECALL, handler the request
		case 8:
		case 11:
			if (arg1 == IRQ_DISABLE)	{
				//zero out mstatus.mpie
				clear_csr(mstatus,MSTATUS_MPIE);

			} else if(arg1 == IRQ_ENABLE)	{
				//set mstatus.mpie
				set_csr(mstatus,MSTATUS_MPIE);

			} else if(arg1 == PORT_YIELD)		{
				//always yield from machine mode
				unsigned long submode = read_csr_msubmode;
				if ((submode & 0x300) == 0x100)
					break;

				//fix up mepc on sync trap
				unsigned long epc = read_csr(mepc);
				vPortYield(sp,epc+4); //never returns

			} else if(arg1 == PORT_YIELD_TO_RA)	{

				vPortYield(sp,(*(unsigned long*)(sp+1*sizeof(sp)))); //never returns
			}

			break;
		default:
			mstatus_mps_bits = ((read_csr(mstatus) & 0x00000600) >> 9);
			printf("In trap handler, the msubmode is 0x%lx\n", read_csr_msubmode);
			printf("In trap handler, the mstatus.MPS is 0x%lx\n", mstatus_mps_bits);
			printf("In trap handler, the mcause is %lx\n", mcause);
			printf("In trap handler, the mepc is 0x%lx\n", read_csr(mepc));
			printf("In trap handler, the mtval is 0x%lx\n", read_csr(mbadaddr));
			if (mstatus_mps_bits == 0x1) {
			    printf("The exception is happened from previous Exception mode, hence is Double-Exception-fault!\n");
			} else if (mstatus_mps_bits == 0x2){
			    printf("The exception is happened from previous NMI mode!\n");
			} else if (mstatus_mps_bits == 0x3){
			    printf("The exception is happened from previous IRQ mode!\n");
			}
			for (i = 1; i < 31; i += 2) {
				printf("x%-2d: %08x, x%-2d: %08x\n", i, *(unsigned *)(sp + i * REGBYTES),
				       i + 1, *(unsigned *)(sp + (i + 1) * REGBYTES));
			}

			for (i = 0; i < 31; i += 2)
				printf("0x%lx: %08x, %08x\n", ((read_csr(mepc)/4) *4 + i * REGBYTES -16),
					*(unsigned *)((read_csr(mepc)/4) *4 + i * REGBYTES -16),
				       *(unsigned *)((read_csr(mepc)/4) *4 + (i + 1) * REGBYTES -16));
			printf("Dump Stack: \n");
			vTaskDumpStack(NULL);
			//_exit(mcause);
			do {}while(1);
	}

	//fix mepc and return
	unsigned long epc = read_csr(mepc);

	write_csr(mepc,epc+4);
	return sp;
}

void vPortEnterCritical( void );
void vPortEnterCritical( void )
{
	//printf("vPortEnterCritical\n");
	#if USER_MODE_TASKS
		ECALL(IRQ_DISABLE);
	#else
		portDISABLE_INTERRUPTS();
		//eclic_set_mth ((configMAX_SYSCALL_INTERRUPT_PRIORITY)<<4);
	#endif

	uxCriticalNesting++;
}
/*-----------------------------------------------------------*/

void vPortExitCritical( void );
void vPortExitCritical( void )
{
	configASSERT( uxCriticalNesting );
	uxCriticalNesting--;
	if ( uxCriticalNesting == 0 )
	{
		#if USER_MODE_TASKS
			ECALL(IRQ_ENABLE);
		#else
			//eclic_set_mth (0);
			portENABLE_INTERRUPTS()	;
		#endif
	}
	return;
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/

/* Clear current interrupt mask and set given mask */
void vPortClearInterruptMask(int int_mask)
{
#ifdef N200_REVA
	write_csr(mie,int_mask);
#else
	eclic_set_mth (int_mask);
#endif
}
/*-----------------------------------------------------------*/

/* Set interrupt mask and return current interrupt enable register */
int xPortSetInterruptMask()
{
	int int_mask=0;
#ifdef N200_REVA
	int_mask = read_csr(mie);
	write_csr(mie,0);
#else
	int_mask=eclic_get_mth();
	eclic_set_mth ((configMAX_SYSCALL_INTERRUPT_PRIORITY)<<4);
#endif
	return int_mask;
}

/*-----------------------------------------------------------*/
/*
 * See header file for description.
 */
extern void *memset(void *dest, int c, size_t len);

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
	/* Simulate the stack frame as it would be created by a context switch
	interrupt. */

	memset(pxTopOfStack - 35,0,36*4);
	//register int *tp asm("x3");
	pxTopOfStack -= 3;
	*pxTopOfStack = (portSTACK_TYPE)pxCode;			/* Start address */

	//set the initial mstatus value
	pxTopOfStack--;
	*pxTopOfStack = MSTATUS_INIT;

	pxTopOfStack -= 22;
	*pxTopOfStack = (portSTACK_TYPE)pvParameters;	/* Register a0 */
	//pxTopOfStack -= 7;
	//*pxTopOfStack = (portSTACK_TYPE)tp; /* Register thread pointer */
	//pxTopOfStack -= 2;
	pxTopOfStack -=9;
	*pxTopOfStack = (portSTACK_TYPE)prvTaskExitError; /* Register ra */
	pxTopOfStack--;

	return pxTopOfStack;
}
/*-----------------------------------------------------------*/


void prvTaskExitError( void )
{
	/* A function that implements a task must not exit or attempt to return to
	its caller as there is nothing to return to.  If a task wants to exit it
	should instead call vTaskDelete( NULL ).
	Artificially force an assert() to be triggered if configASSERT() is
	defined, then stop here so application writers can catch the error. */
	configASSERT( uxCriticalNesting == ~0UL );
	portDISABLE_INTERRUPTS();
	printf("prvTaskExitError\n");
	for ( ;; );
}
/*-----------------------------------------------------------*/



/*Entry Point for Machine Timer Interrupt Handler*/
//Bob: add the function argument int_num
#ifdef N200_REVA
uint32_t vPortSysTickHandler(uint32_t int_num){
#else
void vPortSysTickHandler(void){
#endif
#ifdef configSOC_TIMER_AS_TICK
#else

	uint64_t then = 0;
#ifdef N200_REVA
	pic_disable_interrupt(PIC_INT_TMR);
#endif
    volatile uint64_t * mtime       = (uint64_t*) (TIMER_CTRL_ADDR + TIMER_MTIME);
    volatile uint64_t * mtimecmp    = (uint64_t*) (TIMER_CTRL_ADDR + TIMER_MTIMECMP);
    uint64_t now = *mtime;

    then = now + (configRTC_CLOCK_HZ / configTICK_RATE_HZ);
    *mtimecmp = then;

	/* Increment the RTOS tick. */
	if ( xTaskIncrementTick() != pdFALSE )
	{
		//portYIELD();
		vTaskSwitchContext();
	}
#ifdef N200_REVA
       pic_enable_interrupt(PIC_INT_TMR);
       return int_num;
#endif

#endif
}
/*-----------------------------------------------------------*/

/*Entry Point for SOC Timer Interrupt Handler*/
void vPortSysTickHandler_soc(void)
{
    /* Increment the RTOS tick. */
    if ( xTaskIncrementTick() != pdFALSE )
    {
        vTaskSwitchContext();
    }
}

void vPortSetupTimer(void) {
#ifdef configSOC_TIMER_AS_TICK
    uint32_t threshold;
    uint64_t reg;
    int ret;

    /* Register SoC Timer as systick soure timer */
    ret = RegisterIrq(IRQ_NUM_TIMER, IRQ_TIMER_PROI, vPortSysTickHandler_soc);
    if (ret)
    printf("[%s]: RegisterIrq error, ret = %d\n",
           __func__, ret);
    EnableIrq(IRQ_NUM_TIMER);

    /* Set timer interrupt frequency */
    threshold = configRTC_CLOCK_HZ / configTICK_RATE_HZ;
    REG32(SYSCTRL_TIMER) = (threshold & 0xffff);

    /*Enable timer, set timer to periodic irq timer mode, set clock.*/
    reg = REG32(SYSTICK_TIMER_CTRL);
    reg &= ~(TIMER_EN | TIMER_MODE_IRQ_PERIO);
    reg |= SYSTICK_TIMER_CTRL_PARM;
    REG32(SYSTICK_TIMER_CTRL) = reg;
#else
    // Set the machine timer
    //Bob: update it to TMR
    volatile uint64_t * mtime       = (uint64_t*) (TIMER_CTRL_ADDR + TIMER_MTIME);
    volatile uint64_t * mtimecmp    = (uint64_t*) (TIMER_CTRL_ADDR + TIMER_MTIMECMP);
    uint64_t now = *mtime;
    uint64_t then = now + (configRTC_CLOCK_HZ / configTICK_RATE_HZ);

    *mtimecmp = then;
    //print_eclic();
#ifdef N200_REVA
    pic_enable_interrupt(PIC_INT_TMR);
    pic_set_priority(PIC_INT_TMR, 0x1);//Bob: set the TMR priority to the lowest
#else
    uint8_t mtime_intattr;
    mtime_intattr=eclic_get_intattr (ECLIC_INT_MTIP);
    mtime_intattr|=ECLIC_INT_ATTR_SHV | ECLIC_INT_ATTR_MACH_MODE;
    mtime_intattr|= ECLIC_INT_ATTR_TRIG_EDGE;
    eclic_set_intattr(ECLIC_INT_MTIP,mtime_intattr);
    eclic_enable_interrupt (ECLIC_INT_MTIP);

    //eclic_set_nlbits(4);
    //eclic_set_irq_lvl_abs(ECLIC_INT_MTIP,1);
    eclic_set_intctrl(ECLIC_INT_MTIP, 10 << 4);

    set_csr(mstatus, MSTATUS_MIE);
#endif
#endif
}
/*-----------------------------------------------------------*/

void vPortSetup(void)	{
	vPortSetupTimer();
	uxCriticalNesting = 0;
}
/*-----------------------------------------------------------*/
