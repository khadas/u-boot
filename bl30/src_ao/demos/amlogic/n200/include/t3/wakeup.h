

#ifndef __WAKEUP_H__
#define __WAKEUP_H__

/*use timerB to wakeup AP FSM*/
static inline void wakeup_ap(void)
{
	uint32_t value;
	//uint32_t time_out = 20;

	/*set alarm timer*/
	REG32(FSM_TRIGER_SRC) = 1000;/*1ms*/

	value = REG32(FSM_TRIGER_CTRL);
	value &= ~((1 << 7) | (0x3) | (1 << 6));
	value |= ((1 << 7) | (0 << 6) | (0x3));
	REG32(FSM_TRIGER_CTRL) = value;
	vTaskDelay(1);
}

static inline void clear_wakeup_trigger(void)
{
	REG32(FSM_TRIGER_SRC) = 0;
	REG32(FSM_TRIGER_CTRL) = 0;
}

static inline void watchdog_reset_system(void)
{
}
#endif
