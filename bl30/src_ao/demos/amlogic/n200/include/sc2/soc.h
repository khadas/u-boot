#ifndef __SOC_H
#define __SOC_H
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

/*SoC/Shadow register mapping*/
#define VRTC_PARA_REG		SYSCTRL_STATUS_REG2
#define VRTC_STICKY_REG		SYSCTRL_STICKY_REG2

#define TIMERE_LOW_REG		SYSCTRL_TIMERE
#define TIMERE_HIG_REG		SYSCTRL_TIMERE_HI
#define WAKEUP_REASON_STICK_REG		SYSCTRL_STICKY_REG7
#define FSM_TRIGER_CTRL		SYSCTRL_TIMERB_CTRL
#define FSM_TRIGER_SRC		SYSCTRL_TIMERB
/*
#define SOC_LOCAL_SRAM_LENGTH    64*1024
#define SECPU_LOADER_ENTRY_ZONE_BEGIN \
		(SOC_LOCAL_SRAM_BASE + SOC_LOCAL_SRAM_LENGTH - 2*1024)
*/

#define UART_PORT_CONS UART_B_WFIFO
#endif
