#ifndef __SOC_H
#define __SOC_H
/*
#ifndef __ASM
#include "FreeRTOSConfig.h"
#include "riscv_const.h"
#include "irq.h"
#include "register.h"
#endif

#define SOC_ECLIC_NUM_INTERRUPTS 32
#define SOC_TIMER_FREQ           configRTC_CLOCK_HZ
#define SOC_ECLIC_CTRL_ADDR      0x0C000000UL
#define SOC_TIMER_CTRL_ADDR      0x02000000UL
#define SOC_PMP_BASE             0xff100000UL
#define SOC_LOCAL_SRAM_BASE      0x10000000UL
#define SRAM_BEGIN               SOC_LOCAL_SRAM_BASE
#define SRAM_SIZE                (0x20000)//(96*1024)
#define SRAM_END                 (SRAM_BEGIN + SRAM_SIZE)
#define IO_BASE                  0xff000000UL
#define IO_SIZE                  0x00100000
#define IO_BEGIN                 (IO_BASE)
#define IO_END                   (IO_BASE + IO_SIZE)
*/

#define SOC_PIC_NUM_INTERRUPTS 112
#define SOC_PIC_CTRL_ADDR 0x03000000
#define SOC_TMR_CTRL_ADDR 0x02000000

#if (SOC_PIC_NUM_INTERRUPTS/32)*32 == SOC_PIC_NUM_INTERRUPTS
#define IRQ_EN_REG_NUM (SOC_PIC_NUM_INTERRUPTS/32)
#else
#define IRQ_EN_REG_NUM (SOC_PIC_NUM_INTERRUPTS/32 + 1)
#endif

/*SoC/Shadow register mapping*/
#define VRTC_PARA_REG AO_DEBUG_REG2
#define VRTC_STICKY_REG AO_RTI_STICKY_REG2

#define TIMERE_LOW_REG		ISA_TIMERE
#define TIMERE_HIG_REG		ISA_TIMERE_HI
#define WAKEUP_REASON_STICK_REG		PREG_STICKY_REG3
#define FSM_TRIGER_CTRL		AO_TIMER_CTRL
#define FSM_TRIGER_SRC		AO_DEBUG_REG2  //Just for run happy. Need fix it!!!!

#define UART_PORT_CONS UART1_WFIFO
#endif
