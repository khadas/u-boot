

#ifndef __WAKEUP_H__
#define __WAKEUP_H__

#define WAKEUP_TIMER_CNT	1
#define TIMER_CLK_IDX	3// 0:cts_sys_clk 1:10us 2:100us 3:1ms
/* Calculate the actual value, unit ms */
#ifndef TIMER_CLK_IDX
#error "Not define TIMER_CLK_IDX macro..\n"
#else
	#if !((TIMER_CLK_IDX >= 0) && (TIMER_CLK_IDX <= 3))
	#error "Set invalid value for TIMER"
	#else
		#if (TIMER_CLK_IDX == 0)
		#error "Not support!"
		#elif ((TIMER_CLK_IDX == 1) && (WAKEUP_TIMER_CNT > 100))
		#define SYSTEM_DELAY_VAL	(WAKEUP_TIMER_CNT / 100)
		#elif ((TIMER_CLK_IDX == 2) && (WAKEUP_TIMER_CNT > 10))
		#define SYSTEM_DELAY_VAL	(WAKEUP_TIMER_CNT / 10)
		#elif (TIMER_CLK_IDX == 3)
		#define SYSTEM_DELAY_VAL	(WAKEUP_TIMER_CNT)
		#else
		#define SYSTEM_DELAY_VAL        1
		#endif
	#endif //!((TIMER_CLK_IDX >= 0) && (TIMER_CLK_IDX <=3))
#endif // TIMER_CLK_IDX

inline void wakeup_ap(void)
{
	uint32_t value;

	/*set alarm timer*/
	REG32(ISA_TIMERB) = WAKEUP_TIMER_CNT;
	value = REG32(ISA_TIMER_MUX);
	value &= ~((1 << 17) |(1 << 13)| (TIMER_CLK_IDX << 2));
	value |= ((1 << 17) |(0 << 13)| (TIMER_CLK_IDX << 2));
	REG32(ISA_TIMER_MUX) = value;
	vTaskDelay(3);    /* must set to more than 1 tick */
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
