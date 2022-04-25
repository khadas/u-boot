#ifndef __CEC_T7_DATA_H__
#define __CEC_T7_DATA_H__

/*t7 data define*/

/*GPIOH_3 FUNC4:ceca, FUNC5:cecb*/
#define CEC_PIN_MX	GPIOW_12
#define CEC_PIN_FUNC	PIN_FUNC1
#define CEC_IP		0	/* 0: cec a, 1: cecb*/

//reg table define
#define CEC_CHIP_SEL_T7
//#define CEC_CHIP_SEL_T5
//#define CEC_CHIP_SEL_SC2
//#define CEC_CHIP_SEL_S4

#define CEC_ON		1
#endif

