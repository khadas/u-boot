// See LICENSE for license details.
#include <stdio.h>

#include "riscv_encoding.h"
#include "interrupt_control.h"
#include "register.h"
#include "FreeRTOS.h"

void pic_set_threshold(uint32_t threshold)
{
  volatile uint32_t* threshold_ptr = (uint32_t*) (PIC_CTRL_ADDR +
						  PIC_THRESHOLD_OFFSET);

  *threshold_ptr = threshold;
}

void pic_enable_interrupt(uint32_t source)
{
  volatile uint32_t * current_ptr = (volatile uint32_t *)(PIC_CTRL_ADDR +
                                                        PIC_ENABLE_OFFSET +
                                                        ((source >> 3) & (~0x3))//Source number divide 32 and then multip 4 (bytes)
                                                        );
  uint32_t current = *current_ptr;
  current = current | ( 1 << (source & 0x1f));// Only check the least 5 bits
  *current_ptr = current;
}

void pic_disable_interrupt (uint32_t source){

  volatile uint32_t * current_ptr = (volatile uint32_t *) (PIC_CTRL_ADDR +
                                                         PIC_ENABLE_OFFSET +
                                                         ((source >> 3) & (~0x3))//Source number divide 32 and then multip 4 (bytes)
                                                          );
  uint32_t current = *current_ptr;
  current = current & ~(( 1 << (source & 0x1f)));// Only check the least 5 bits
  *current_ptr = current;
}

void pic_set_priority (uint32_t source, uint32_t priority){

  if (PIC_NUM_PRIORITIES > 0) {
    volatile uint32_t * priority_ptr = (volatile uint32_t *)
      (PIC_CTRL_ADDR +
       PIC_PRIORITY_OFFSET +
       (source << PIC_PRIORITY_SHIFT_PER_SOURCE));// Each priority reg occupy a word, so multiple 2
    *priority_ptr = priority;
  }
}

uint32_t pic_claim_interrupt(void){

  volatile uint32_t * claim_addr = (volatile uint32_t * )
    (PIC_CTRL_ADDR +
     PIC_CLAIM_OFFSET
     );

  return  *claim_addr;
}

uint32_t pic_check_eip(void){

  volatile uint32_t * eip_addr = (volatile uint32_t * )
    (PIC_CTRL_ADDR +
     PIC_EIP_OFFSET
     );

  return  *eip_addr;
}

void pic_complete_interrupt(uint32_t source){

  volatile uint32_t * claim_addr = (volatile uint32_t *) (PIC_CTRL_ADDR +
                                                                PIC_CLAIM_OFFSET
                                                                );
  *claim_addr = source;
}

void DefaultInterruptHandler(void)
{
}

int RegisterIrq(uint32_t int_num, uint32_t int_priority, function_ptr_t handler) {
    pic_interrupt_handlers[int_num] = handler;
    pic_set_priority(int_num, int_priority);
   // pic_enable_interrupt (int_num);
    return 0;
}

int UnRegisterIrq(uint32_t int_num)
{
    pic_interrupt_handlers[int_num] = DefaultInterruptHandler;
    pic_set_priority(int_num, 0);

    return 0;
}

int EnableIrq(uint32_t ulIrq)
{
    UBaseType_t uxSavedInterruptStatus;

    uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
    pic_enable_interrupt(ulIrq);
    portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
    return 0;
}

int DisableIrq(uint32_t ulIrq)
{
    UBaseType_t uxSavedInterruptStatus;

    uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
    pic_disable_interrupt(ulIrq);
    portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
    return 0;
}

int SetIrqPriority(uint32_t ulIrq, uint32_t ulPri)
{
    pic_set_priority(ulIrq, ulPri);
    return 0;
}

static unsigned int irq_setting[IRQ_EN_REG_NUM];
/*N205 does not support clear pending irq.*
 *Need use work around to clear pending:  *
 *1. disable irq (MIE)                    *
 *2. store and disable current enable irq *
 *3. enable target irq                    *
 *4. claim and complete target irq        *
 *5. disable target irq                   *
 *6. restore current irq enable setting   *
 *7. enable irq (MIE)
*/
int ClearPendingIrq(uint32_t ulIrq)
{
	unsigned int i;
	volatile uint32_t * current_ptr;
	unsigned long irq_status;

	irq_status = interrupt_status_get();
	if (irq_status)
		interrupt_disable();

	for (i = 0; i < IRQ_EN_REG_NUM; i++)
	{
		current_ptr =
			(volatile uint32_t *)(PIC_CTRL_ADDR + PIC_ENABLE_OFFSET + i*4);
		irq_setting[i] =
			REG32(current_ptr);
		REG32(current_ptr) = 0;
	}
	pic_enable_interrupt(ulIrq);
	i = pic_claim_interrupt();
	if (i)
		pic_complete_interrupt(i);
	pic_disable_interrupt(ulIrq);

	for (i = 0; i < IRQ_EN_REG_NUM; i++)
	{
		current_ptr =
			(volatile uint32_t *)(PIC_CTRL_ADDR + PIC_ENABLE_OFFSET + i*4);
		REG32(current_ptr) = irq_setting[i];
	}

	if (irq_status)
		interrupt_enable();

	return 0;
}

