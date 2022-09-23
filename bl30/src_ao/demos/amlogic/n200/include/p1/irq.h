#ifndef __IRQ_H_
#define __IRQ_H_

extern void eclic_irq20_handler(void);
extern void eclic_irq21_handler(void);
extern void eclic_irq22_handler(void);
extern void eclic_irq23_handler(void);
extern void eclic_irq24_handler(void);

extern void eclic_irq50_handler(void);

#define CONCAT_STAGE_1(w, x, y, z) w ## x ## y ## z
#define CONCAT2(w, x) CONCAT_STAGE_1(w, x, , )
#define CONCAT3(w, x, y) CONCAT_STAGE_1(w, x, y, )
#define CONCAT4(w, x, y, z) CONCAT_STAGE_1(w, x, y, z)

/* Helper macros to build the IRQ handler and priority struct names */
#define IRQ_HANDLER(irqname) CONCAT3(eclic_irq, irqname, _handler)
/*
 * Macro to connect the interrupt handler "routine" to the irq number "irq" and
 * ensure it is enabled in the interrupt controller with the right priority.
 */
#if 0
#define DECLARE_IRQ(irq, routine) DECLARE_IRQ_(irq, routine)
#define DECLARE_IRQ_(irq, routine)                    \
	void IRQ_HANDLER(irq)(void)				\
	{							\
		routine();				\
	}
#endif
#define DECLARE_IRQ(irq, routine)

/*IRQ_NUM define list*/
#define IRQ_NUM_MB_0	50
#define IRQ_NUM_MB_1	49
#define IRQ_NUM_MB_2	48
#define IRQ_NUM_MB_3	47
#define IRQ_NUM_MB_4	46


/*You can add other interrupts num here 46~19*/

/* use for ir */
#define IRQ_NUM_IRIN	22

/* cec */
#define IRQ_NUM_CECA	40
#define IRQ_NUM_CECB	41
#define IRQ_ETH_PMT_NUM 76
#endif
