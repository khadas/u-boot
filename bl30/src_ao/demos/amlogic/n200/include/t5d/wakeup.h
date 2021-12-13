

#ifndef __WAKEUP_H__
#define __WAKEUP_H__

inline void wakeup_ap(void)
{
	uint32_t value;

	/*set alarm timer*/
	REG32(ISA_TIMERB) = 1;
	value = REG32(ISA_TIMER_MUX);
	value &= ~((1 << 17) |(1 << 13)| (0x3 << 2));
	value |= ((1 << 17) |(0 << 13)| (0x3 << 2));
	REG32(ISA_TIMER_MUX) = value;
	vTaskDelay(1);
}

inline void clear_wakeup_trigger(void)
{
	uint32_t value;

	value = REG32(ISA_TIMER_MUX);
	value &= ~((1 << 17) |(1 << 13)| (0x3 << 2));
	REG32(ISA_TIMER_MUX) = value;

}

inline void watchdog_reset_system(void)
{
	while (1) {
		REG32(SEC_AO_WATCHDOG_CNTL) = 1 << 26 | 0 << 18;
		//pdMS_TO_TICKS(2);
	}
}


#endif
