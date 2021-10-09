#include "FreeRTOS.h"
#include "common.h"
#include "gpio.h"
#include "ir.h"
#include "suspend.h"
#include "soc.h"


#define DEFAULT_VDDCPU		690
#define DEFAULT_VDDEE		770
#define CORNER_VALID_INDX	1

/*
 *  shutdown : ss tt ff
 *  poweroff : ss tt ff
 *  vad	     : ss tt ff
 * */
uint32_t vddee_voltage_matrix[][3] = {
	/*ss  tt   ff*/
	{770, 720, 700},/*shutdown*/
	{770, 770, 770},/*suspend*/
	{0  , 0  , 0  } /*vad*/
};

uint32_t vddcpu_voltage_matrix[][3] = {
	/*ss  tt   ff*/
	{690, 690, 690},/*shutdown*/
	{690, 690, 690},/*suspend*/
	{690, 690, 690} /*vad*/
};


static uint32_t suspend_msg;
enum {
	C_SS = 0,
	C_TT,
	C_FF,
	C_INV = 4
}Cornet_type;

enum {
	M_SHUTDOWN = 0,
	M_SUSPEND,
	M_VAD,
	M_INV
}SYS_MODE;

uint32_t get_vddee_voltage(int is_shutdown);
uint32_t get_vddcpu_voltage(int is_shutdown);

uint32_t parse_suspend_msg(void *msg)
{
	suspend_msg = *(uint32_t *)msg;

	printf("[power]mode:%d, corner:%d\n",
			GET_SUSPEND_FLAG(suspend_msg),
			GET_CORNER(suspend_msg));

	return GET_SUSPEND_FLAG(suspend_msg);
}

uint32_t get_vddee_voltage(int is_shutdown)
{
	uint32_t corner;

	/* check vad function status */
	if (GET_VAD_INFO(suspend_msg)) {
		printf("Vad function is open, prohibit voltage regulation\n");
		return vddee_voltage_matrix[M_VAD][C_TT];
	}

	corner = GET_CORNER(suspend_msg);

	if (!(corner < C_INV) ||
			(corner < CORNER_VALID_INDX)) {
		printf("warning: corner information incorrect,use SS type!!\n");
		corner = C_SS;
	} else {
		corner -= CORNER_VALID_INDX;
	}

	/* check if it's poweroff */
	if (is_shutdown) {
		return vddee_voltage_matrix[M_SHUTDOWN][corner];
	} else {
		return vddee_voltage_matrix[M_SUSPEND][corner];
	}
}

uint32_t get_vddcpu_voltage(int is_shutdown)
{
	uint32_t corner;

	/* check vad function status */
	if (GET_VAD_INFO(suspend_msg)) {
		printf("Vad function is open, prohibit voltage regulation\n");
		return vddcpu_voltage_matrix[M_VAD][C_TT];
	}

	corner = GET_CORNER(suspend_msg);

	if (!(corner < C_INV) ||
			(corner < CORNER_VALID_INDX)) {
		printf("warning: corner information incorrect,use SS type!!\n");
		corner = C_SS;
	} else {
		corner -= CORNER_VALID_INDX;
	}

	/* check if it's poweroff */
	if (is_shutdown) {
		return vddcpu_voltage_matrix[M_SHUTDOWN][corner];
	} else {
		return vddcpu_voltage_matrix[M_SUSPEND][corner];
	}
}

